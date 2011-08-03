﻿using FourDO.Emulation.FreeDO;
using FourDO.Emulation.Plugins;
using FourDO.Utilities;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Timers;
using System.Threading;

namespace FourDO.Emulation
{
    internal class GameConsole
    {
        public event EventHandler FrameDone;

        public class BadBiosRomException : Exception {};
        public class BadGameRomException : Exception { };
        
        #region Private Variables

        private const int ROM_SIZE = 1 * 1024 * 1024;
        private const int NVRAM_SIZE = 32 * 1024;

        private byte[] biosRomCopy;

        private byte[] frame;
        private IntPtr framePtr;
        private GCHandle frameHandle;

        private Thread workerThread;
        private volatile bool stopWorkerSignal = false;

        private int currentSector = 0;
        private bool isSwapFrameSignaled = false;

        private string gameRomFileName;
        private BinaryReader gameRomReader;

        private volatile FrameSpeedCalculator speedCalculator = new FrameSpeedCalculator(4);

        private IAudioPlugin audioPlugin = PluginLoader.GetAudioPlugin();

        #endregion //Private Variables

        #region Singleton Implementation

        private static GameConsole instance;

        public static GameConsole Instance
        {
            get 
            {
                if (instance == null)
                {
                    instance = new GameConsole();
                }
                return instance;
            }
        }

        #endregion // Singleton Implementation

        private GameConsole()
        {
            FreeDOCore.ReadRomEvent = new FreeDOCore.ReadRomDelegate(ExternalInterface_ReadRom);
            FreeDOCore.ReadNvramEvent = new FreeDOCore.ReadNvramDelegate(ExternalInterface_ReadNvram);
            FreeDOCore.WriteNvramEvent = new FreeDOCore.WriteNvramDelegate(ExternalInterface_WriteNvram);
            FreeDOCore.SwapFrameEvent = new FreeDOCore.SwapFrameDelegate(ExternalInterface_SwapFrame);
            FreeDOCore.PushSampleEvent = new FreeDOCore.PushSampleDelegate(ExternalInterface_PushSample);
            FreeDOCore.GetPbusLengthEvent = new FreeDOCore.GetPbusLengthDelegate(ExternalInterface_GetPbusLength);
            FreeDOCore.GetPbusDataEvent = new FreeDOCore.GetPbusDataDelegate(ExternalInterface_GetPbusData);
            FreeDOCore.KPrintEvent = new FreeDOCore.KPrintDelegate(ExternalInterface_KPrint);
            FreeDOCore.DebugPrintEvent = new FreeDOCore.DebugPrintDelegate(ExternalInterface_DebugPrint);
            FreeDOCore.FrameTriggerEvent = new FreeDOCore.FrameTriggerDelegate(ExternalInterface_FrameTrigger);
            FreeDOCore.Read2048Event = new FreeDOCore.Read2048Delegate(ExternalInterface_Read2048);
            FreeDOCore.GetDiscSizeEvent = new FreeDOCore.GetDiscSizeDelegate(ExternalInterface_GetDiscSize);
            FreeDOCore.OnSectorEvent = new FreeDOCore.OnSectorDelegate(ExternalInterface_OnSector);

            ///////////////
            // Allocate the VDLFrames

            // Get the size of a VDLFrame (must be "unsafe").
            int VDLFrameSize;
            unsafe
            {
                VDLFrameSize = sizeof(VDLFrame);
            }

            frame = new byte[VDLFrameSize];
            frameHandle = GCHandle.Alloc(frame, GCHandleType.Pinned);
            framePtr = frameHandle.AddrOfPinnedObject();
        }

        public void Destroy()
        {
            if (audioPlugin != null)
                audioPlugin.Destroy();
        }

        public bool Running { get; private set; }

        public IntPtr CurrentFrame
        {
            get
            {
                return framePtr;
            }
        }

        public double CurrentFrameSpeed
        {
            get
            {
                return speedCalculator.CurrentAverage;
            }
        }

        public string GameRomFileName
        {
            get
            {
                return gameRomFileName;
            }
        }

        public void Start(string biosRomFileName, string gameRomFileName)
        {
            // Are we already started?
            if (this.workerThread != null)
                return;
            
            //////////////
            // Attempt to load a copy of the rom. Make sure it's the right size!
            try
            {
                this.biosRomCopy = System.IO.File.ReadAllBytes(biosRomFileName);
            }
            catch 
            {
                throw new BadBiosRomException();
            }


            // Also get outta here if the rom file isn't the right length.
            if (this.biosRomCopy.Length != ROM_SIZE)
                throw new BadBiosRomException();

            //////////////
            // Attempt to open a binary reader to the game rom.
            if (string.IsNullOrEmpty(gameRomFileName) == false)
            {
                try
                {
                    this.gameRomReader = new BinaryReader(new FileStream(gameRomFileName, FileMode.Open));
                    this.gameRomFileName = gameRomFileName;
                }
                catch
                {
                    throw new BadGameRomException();
                }
            }
            else
            {
                // It is valid to start without a loaded game.
                this.gameRomReader = null;
                this.gameRomFileName = null;
            }

            /////////////////
            // Start the core.
            
            Running = true;
            FreeDOCore.Initialize();

            this.InternalStart();
        }

        public void Stop()
        {
            // Are we already stopped?
            if (this.workerThread == null)
                return;

            if (this.workerThread.ManagedThreadId == Thread.CurrentThread.ManagedThreadId)
                {} // I dunno what to do. Screw it.

            /////////////////
            // Stop the core.

            this.InternalStop();

            if (this.gameRomReader != null)
                this.gameRomReader.Close();

            // Done!
            FreeDOCore.Destroy();
            Running = false;
        }

        public void SaveState(string saveStateFileName)
        {
            // TODO: Should I raise an error if we're not running?
            
            if (this.Running)
                this.InternalStop();

            BinaryWriter writer = null;
            try
            {
                writer = new BinaryWriter(new FileStream(saveStateFileName, FileMode.Create));

                byte[] saveData = new byte[FreeDOCore.GetSaveSize()];
                unsafe
                {
                    fixed (byte* saveDataPtr = saveData)
                    {
                        IntPtr pointer = new IntPtr(saveDataPtr);
                        FreeDOCore.DoSave(pointer);
                    }
                }
                writer.Write(saveData);
                writer.Close();
            }
            catch
            {
                throw;
            }
            finally
            {
                if (writer != null)
                    writer.Close();

                if (this.Running)
                    this.InternalStart();
            }
        }

        public void LoadState(string saveStateFileName)
        {
            // TODO: Should I raise an error if we're not running?

            if (this.Running)
                this.InternalStop();

            BinaryReader reader = null;
            try
            {
                reader = new BinaryReader(new FileStream(saveStateFileName, FileMode.Open));
                byte[] saveData = reader.ReadBytes((int)FreeDOCore.GetSaveSize());
                unsafe
                {
                    fixed (byte* saveDataPtr = saveData)
                    {
                        IntPtr pointer = new IntPtr(saveDataPtr);
                        FreeDOCore.DoLoad(pointer);
                    }
                }
                reader.Close();
            }
            catch
            {
                throw;
            }
            finally
            {
                if (reader != null)
                    reader.Close();

                if (this.Running)
                    this.InternalStart();
            }
        }

        private void InternalStart()
        {
            stopWorkerSignal = false;
            this.workerThread = new Thread(new ThreadStart(this.WorkerThread));
            this.workerThread.Priority = ThreadPriority.Highest;
            this.workerThread.Start();
        }

        private void InternalStop()
        {
            // Signal a shutdown and wait for it.
            stopWorkerSignal = true;
            this.workerThread.Join();
            this.workerThread = null;
        }

        #region FreeDO External Interface

        private void ExternalInterface_ReadRom(IntPtr romPointer)
        {
            ////////
            // Now copy!
            unsafe
            {
                fixed (byte* sourceRomBytesPointer = this.biosRomCopy)
                {
                    long* sourcePtr = (long*)sourceRomBytesPointer;
                    long* destPtr = (long*)romPointer.ToPointer();
                    for (int x = 0; x < ROM_SIZE / 8; x++)
                    {
                        *destPtr++ = *sourcePtr++;
                    }
                }
            }
        }

        private void ExternalInterface_ReadNvram(IntPtr nvramPointer)
        {
        }

        private void ExternalInterface_WriteNvram(IntPtr nvramPointer)
        {
        }

        private IntPtr ExternalInterface_SwapFrame(IntPtr currentFrame)
        {
            isSwapFrameSignaled = true;
            return currentFrame;
        }

        private void ExternalInterface_PushSample(uint dspSample)
        {
            if (audioPlugin != null)
                audioPlugin.PushSample(dspSample);
        }

        private int ExternalInterface_GetPbusLength()
        {
            return 0;
        }

        private IntPtr ExternalInterface_GetPbusData()
        {
            return new IntPtr(0);
        }

        private void ExternalInterface_KPrint(IntPtr value)
        {
            Console.Write((char)value);
        }

        private void ExternalInterface_DebugPrint(IntPtr value)
        {
        }

        private void ExternalInterface_FrameTrigger()
        {
        }

        private void ExternalInterface_Read2048(IntPtr buffer)
        {
            if (this.gameRomReader == null)
                return; // No game loaded.

            this.gameRomReader.BaseStream.Position = 2048 * currentSector;
            byte[] bytesToCopy = this.gameRomReader.ReadBytes(2048);

            ////////
            // Now copy!
            unsafe
            {
                fixed (byte* sourceRomBytesPointer = bytesToCopy)
                {
                    long* sourcePtr = (long*)sourceRomBytesPointer;
                    long* destPtr = (long*)buffer.ToPointer();
                    for (int x = 0; x < 2048 / 8; x++)
                    {
                        *destPtr++ = *sourcePtr++;
                    }
                }
            }
        }

        private int ExternalInterface_GetDiscSize()
        {
            if (this.gameRomReader == null)
                return 0;
            return (int)(this.gameRomReader.BaseStream.Length >> 11);
        }

        private void ExternalInterface_OnSector(int sectorNumber)
        {
            currentSector = sectorNumber;
        }

        #endregion // FreeDO External Interface

        #region Worker Thread

        private void WorkerThread()
        {
            const int MAXIMUM_FRAME_COUNT = 100;
            
            long lastSample = 0;
            long lastTarget = 0;
            long targetPeriod = PerformanceCounter.Frequency / 60;
            int lastFrameCount = 0;

            int sleepTime = 0;
            do
            {
                ///////////
                // Identify how long to sleep.
                if (lastSample == 0)
                {
                    // This is the first sample; this one's a freebee.
                    lastSample = PerformanceCounter.Current;
                    lastTarget = lastSample; // Pretend we're right on target!
                    sleepTime = 0;
                }
                else
                {
                    long currentSample = PerformanceCounter.Current;
                    long currentDelta = currentSample - lastSample;

                    long currentTarget = lastTarget + targetPeriod * lastFrameCount;
                    long currentRemaining = currentTarget - currentSample;

                    speedCalculator.AddSample((currentDelta / (double)lastFrameCount) / (double)PerformanceCounter.Frequency);

                    // Are we behind schedule?
                    if (currentRemaining < 0)
                    {
                        // We ARE behind schedule. Oh crap!
                        if (currentRemaining < -(targetPeriod * lastFrameCount) * 10)
                        {
                            // We're REALLY behind schedule. HOLY SHIT!
                            // There's no way we can catch up. Just give up and give a new target.
                            currentTarget = currentSample;
                            sleepTime = 0;
                        }
                        else
                        {
                            // We're not that far behind schedule. Phew!
                            sleepTime = 0;
                        }
                    }
                    else
                    {
                        // Not behind schedule.
                        double wait = (currentRemaining) / (double)PerformanceCounter.Frequency;
                        sleepTime = (int)(wait * 1000);
                    }

                    // Save the values for next time.
                    lastSample = currentSample;
                    lastTarget = currentTarget;
                }

                // Sleep, but always use a value of at least 0.
                // (We're a high priority thread, but we can't just choke everyone.)
                if (sleepTime < 0)
                    sleepTime = 0;
                Thread.Sleep(sleepTime);

                ///////////
                // We're awake! Wreak havoc!
                
                // Execute a frame.
                isSwapFrameSignaled = false;
                lastFrameCount = 0;
                do
                {
                    FreeDOCore.DoExecuteFrame(this.framePtr);
                    lastFrameCount++;
                } while (isSwapFrameSignaled == false && lastFrameCount < MAXIMUM_FRAME_COUNT);
                
                // Signal completion.
                if (FrameDone != null)
                    FrameDone(this, new EventArgs());
            } while (stopWorkerSignal == false);
        }

        #endregion // Worker Thread
    }
}