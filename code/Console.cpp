#include "Console.h"

#include "KernelFaker.h"
#include "filesystem\file.h"

#define  ROM_LOAD_ADDRESS  0x00100000

Console::Console ()
{
   // Constructor
   m_DMA = new DMAController ();
   
   m_CPU = new ARMCPU ();
   m_CPU->DMA = m_DMA;
   
   Reset();
}

Console::~Console ()
{
   // Destructor
   delete m_DMA;
   delete m_CPU;
}

void Console::Reset()
{
   m_CPU->Reset();
   m_DMA->Reset();
}

ARMCPU* Console::CPU ()
{
   return m_CPU;
}

DMAController* Console::DMA ()
{
   return m_DMA;
}

bool Console::loadIso( wxString fileName )
{
	///////////////////////////////////////////////////
	// Actions taken here should emulate the behavior of
	// a 3DO that has set up the operating system for
	// the loaded game
	
	///////////////////////////////////////////////////
	// Open the Launchme of this CD image
	uint32  bytesRead;
	File f( fileName );

	if( !f.openFile( "/launchme" ) )
		return false;

	// Load it into memory.
	f.read( m_DMA->GetRAMPointer( ROM_LOAD_ADDRESS ), f.getFileSize (), &bytesRead );
	
	///////////////////////////////////////////////////
	// Set up some initial register values.
	m_CPU->ARM.USER[0]  = 0x00000000;
	m_CPU->ARM.USER[1]  = 0x00000000;
	m_CPU->ARM.USER[2]  = 0x00000000;
	m_CPU->ARM.USER[3]  = 0x00000000;
	m_CPU->ARM.USER[4]  = 0xFFFFFFFC;
	m_CPU->ARM.USER[5]  = 0x00000000;
	m_CPU->ARM.USER[6]  = 0x00000000;
	m_CPU->ARM.USER[7]  = KernelFaker::getFakeKernelBase();
	m_CPU->ARM.USER[8]  = 0x00000000;
	m_CPU->ARM.USER[9]  = 0x400002CC;
	m_CPU->ARM.USER[10] = 0x0007EFE0;
	m_CPU->ARM.USER[11] = 0x00000000;
	m_CPU->ARM.USER[12] = 0x00078508;
	m_CPU->ARM.USER[13] = 0x00000000;
	m_CPU->ARM.USER[14] = 0x00000000;
	m_CPU->ARM.USER[15] = ROM_LOAD_ADDRESS;

	// Put the stack pointers somewhere
	m_CPU->ARM.USER[13] = 0x00040000;
	m_CPU->ARM.SVC[0]   = 0x00041000;
	m_CPU->ARM.ABT[0]   = 0x00042000;
	m_CPU->ARM.IRQ[0]   = 0x00043000;
	m_CPU->ARM.UND[0]   = 0x00044000;
	m_CPU->ARM.FIQ[5]   = 0x00045000;	
	
	///////////////////////////////////////////////////
	// Put some crap on the USR mode stack that is 
	// normally there by the time LaunchMe is run
	sprintf( (char*)m_DMA->GetRAMPointer( m_CPU->ARM.USER[13] ), "$app/Launchme" );
	
	///////////////////////////////////////////////////
	// Create a fake kernel table 
	KernelFaker kernelFaker;
	kernelFaker.InitializeFakeKernel( m_DMA );
	
	return true;
}

bool Console::loadBinary( wxString fileName )
{
	// Open a code file.
	wxFile file;
	
	// Load the code into memory
	if( !file.Open( fileName ) )
		return false;
	
	if( file.Read( m_DMA->GetRAMPointer( ROM_LOAD_ADDRESS ), file.Length () ) == wxInvalidOffset )
		return false;
	
	file.Close();

	// Set PC
	m_CPU->SetPC( ROM_LOAD_ADDRESS );
	
	// Set to user mode
	m_CPU->SetCPSR(0x00000010);
	
	return true;
}