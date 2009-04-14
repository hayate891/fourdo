#ifndef _INC_SWIHANDLER
#define _INC_SWIHANDLER

#include <map>

#include "ARMRegisters.h"
#include "DMA.hpp"
#include "types.h"

namespace fourdo { namespace swi
{
	// 
	// types
	// 
	typedef void (*swiHandler)(ARMRegisters *registers, DMA *dma); 

	// 
	// this is a mapping from software interrupt number (i.e. 0x1000e)
	// to swi interrupt handler function pointers
	// 
	extern std::map<uint32, swiHandler> swiToHandlerMap;

	/////////////////////////////////////////////////////////////////////////
	// 
	// ARM Interrupts
	// 
	void SWI_ARM_GetMemoryBaseAddress(ARMRegisters *registers, DMA *dma);
	void SWI_ARM_HaltExecution(ARMRegisters *registers, DMA *dma);
	
	/////////////////////////////////////////////////////////////////////////
	// 
	// Meta Interrupts
	//
	// JMK:
	// These are 4DO-only interrupts. I found that some 3DO code was manually 
	// setting up the LR and PC. It sets the PC to values out of a large table.
	// FourDO has replaced this table so that these manual kernel calls become
	// SWI's.
	// 
	void SWI_META_ImageEntryPoint(ARMRegisters *registers, DMA *dma);

	/////////////////////////////////////////////////////////////////////////
	// 
	// Kernel Interrupts
	// 
	
	// kprintf
	void SWI_KRN_kprintf(ARMRegisters *registers, DMA *dma);
	
	/////////////////////////
	// Memory
	
	// Allocation
	void SWI_KRN_malloc              (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_AllocMem            (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_AllocMemBlocks      (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_AllocMemFromMemList (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_AllocMemFromMemLists(ARMRegisters *registers, DMA *dma);
	void SWI_KRN_AllocMemList        (ARMRegisters *registers, DMA *dma);

	// Deletion
	void SWI_KRN_free             	 (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_FreeMem             (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_FreeMemList      	 (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_FreeMemToMemList 	 (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_FreeMemToMemLists	 (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_ScavengeMem      	 (ARMRegisters *registers, DMA *dma);

	// Memory info
	void SWI_KRN_AvailMem            (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_ControlMem          (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_GetMemAllocAlignment(ARMRegisters *registers, DMA *dma);
	void SWI_KRN_GetMemTrackSize     (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_GetMemType          (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_GetPageSize         (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_IsMemReadable       (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_IsMemWritable       (ARMRegisters *registers, DMA *dma);

	// Memory debugging
	void SWI_KRN_DumpMemDebug        (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_CreateMemDebug      (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_DeleteMemDebug      (ARMRegisters *registers, DMA *dma);
	void SWI_KRN_SanityCheckMemDebug (ARMRegisters *registers, DMA *dma);

	/////////////////////////
	// Items

    // Creating Items
    void SWI_KRN_CreateBufferedMsg     (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateIOReq           (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateItem            (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateItemVA          (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateMsg             (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateMsgPort         (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateSemaphore       (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateSmallMsg        (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateThread          (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateUniqueMsgPort   (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CreateUniqueSemaphore (ARMRegisters *registers, DMA *dma);

    // Opening Items
    void SWI_KRN_FindAndOpenDevice     (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindAndOpenFolio      (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindAndOpenItem       (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindAndOpenItemVA     (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindAndOpenNamedItem  (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_OpenItem              (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_OpenNamedDevice       (ARMRegisters *registers, DMA *dma);

    // Managing Items
    void SWI_KRN_CheckItem             (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindDevice            (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindFolio             (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindItem              (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindItemVA            (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindMsgPort           (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindNamedItem         (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindSemaphore         (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindTask              (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_FindVersionedItem     (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_LookupItem            (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_SetItemOwner          (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_SetItemPri            (ARMRegisters *registers, DMA *dma);

    // Closing and Deleting Items
    void SWI_KRN_CloseItem             (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_CloseNamedDevice      (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_DeleteIOReq           (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_DeleteItem            (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_DeleteMsg             (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_DeleteMsgPort         (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_DeleteSemaphore       (ARMRegisters *registers, DMA *dma);
    void SWI_KRN_DeleteThread          (ARMRegisters *registers, DMA *dma);

/*
	Item CreateSizedItem(int32 ctype,TagArg *p,int32 size);
	extern int32 WaitSignal(uint32 sigMask);
	extern Err SendSignal(Item task,uint32 sigMask);
	Err DeleteItem(Item i);
	Item FindItem(int32 ctype,TagArg *tp);
	Item OpenItem(Item FoundItem, void *args);
	Err UnlockItem(Item s);
	Err UnlockSemaphore(Item s);
	int32 LockItem(Item s,uint32 flags);
	int32 LockSemaphore(Item s,uint32 flags);
	Err CloseItem(Item i);
	extern void Yield(void);
	int32 SetItemPri(Item i,uint8 newpri);
	void *AllocMemBlocks(int32 size, uint32 typebits);
*/
/*
	extern Item GetThisMsg(Item msg);
	extern Err SendMsg(Item mp,Item msg,void *dataptr, int32 datasize);
	extern Err SendSmallMsg(Item mp,Item msg,uint32 val1, uint32 val2);
	uint32 ReadHardwareRandomNumber(void);
	extern Err ReplyMsg(Item msg, int32 result,void *dataptr, int32 datasize);
	extern Err ReplySmallMsg(Item msg, int32 result,uint32 val1, uint32 val2);
	extern Item GetMsg(Item mp);
	Err ControlMem(void *p,int32 size,int32 cmd,Item task);
	extern int32 AllocSignal(uint32 sigMask);
	extern Err FreeSignal(uint32 sigMask);
	extern void *SetFunction(Item, int32 vnum, int32 vtype, void *newfunc);
	Err SendIO(Item ior, IOInfo *ioiP);
	void AbortIO(Item ior);
	Err SetItemOwner(Item i,Item newOwner);
	int32 SystemScavengeMem(void);
	void CompleteIO(IOReq *ior); 
*/

	// 
	// TODO: missing some kernel interrupts here
	// 

	// 
	// graphics software interrupts
	// 

/*
	Err SetReadAddress( Item bitmapItem, ubyte *buffer, int32 width );
	Err ResetReadAddress( Item bitmapItem );
	Err SetClipOrigin( Item bitmapItem, int32 x, int32 y );
	Err EnableVAVG( Item screenItem );
	Err DisableVAVG( Item screenItem );
	Err EnableHAVG( Item screenItem );
	Err DisableHAVG( Item screenItem );
	Err SetScreenColor( Item screenItem, uint32 colorEntry );
	Err ResetScreenColors( Item screenItem );
	Err SetScreenColors( Item screenItem, uint32 *entries, int32 count );
	int32 AddScreenGroup( Item screenGroup, TagArg *targs );
	Err RemoveScreenGroup( Item screenGroup );
	Item CreateScreenGroup( Item *screenItemArray, TagArg *targs );
	Err DeleteScreenGroup (Item screenGroupItem);
	Err SetClipWidth( Item bitmapItem, int32 clipWidth );
	Err SetClipHeight( Item bitmapItem, int32 clipHeight );
	Err DrawScreenCels( Item screenItem, CCB *ccb );
	Err DrawText16( GrafCon *gcon, Item bitmapItem, uint16 *text );
	Err DrawChar( GrafCon *gcon, Item bitmapItem, uint32 character );
	Err DrawTo( Item bitmapItem, GrafCon *grafcon, Coord x, Coord y );
	Err FillRect( Item bitmapItem, GrafCon *gc, Rect *r );
	Err SetCurrentFontCCB( CCB *ccb );
	Font *GetCurrentFont( void );
	Err DrawText8( GrafCon *gcon, Item bitmapItem, uint8 *text );
	Err DrawCels( Item bitmapItem, CCB *ccb );
	Err SetCEControl( Item bitmapItem, int32 controlWord, int32 controlMask );
	Err SetCEWatchDog( Item bitmapItem, int32 db_ctr );
	Err DisplayScreen( Item screenItem0, Item screenItem1 );
	Err SetVDL( Item screenItem, Item vdlItem );
	Err ModifyVDL (Item vdlItem, TagArg* vdlTags);
*/

	/*
	 * just regular graphics methods?
	 * 
	void *GetPixelAddress( Item screenItem, Coord x, Coord y );
	void MapCel( CCB *ccb, Point *quad );
	void MoveTo( GrafCon *gc, Coord x, Coord y );
	Err OpenGraphicsFolio( void );
	RGB888 ReadCLUTColor( uint32 index );
	Color ReadPixel( Item bitmapItem, GrafCon *gc, Coord x, Coord y );
	Err ResetCurrentFont( void );
	void  SetBGPen( GrafCon *gc, Color c );
	void SetFGPen( GrafCon *gc, Color c );
	Item SubmitVDL( VDLEntry *VDLDataPtr, int32 length, int32 type );
	Err WritePixel( Item bitmapItem, GrafCon *gc, Coord x, Coord y );
	DisplayInfo* GetFirstDisplayInfo (void);
	Item GetVRAMIOReq (void);
	Err SetVRAMPages (Item ioreq, void *dest, int32 val, int32 numpages, int32 mask);
	Err CopyVRAMPages (Item ioreq, void *dest, void *src, uint32 numpages, uint32 mask);
	Err CloneVRAMPages (Item ioreq, void *dest, void *src, uint32 numpages, uint32 mask);
	Err SetVRAMPagesDefer (Item ioreq, void *dest, int32 val, int32 numpages, int32 mask);
	Err CopyVRAMPagesDefer (Item ioreq, void *dest, void *src, uint32 numpages, uint32 mask);
	Err CloneVRAMPagesDefer (Item ioreq, void *dest, void *src, uint32 numpages, uint32 mask);
	Item GetVBLIOReq (void);
	Err WaitVBL (Item ioreq, uint32 numfields);
	Err WaitVBLDefer (Item ioreq, uint32 numfields);
	*/

	// 
	// file software interrupts
	// 

/*
	extern Item  OpenDiskFile(char *path);
	extern int32 CloseDiskFile(Item fileItem);
	extern Item  MountFileSystem(Item deviceItem,int32 unit,uint32 blockOffset);
	extern Item  OpenDiskFileInDir(Item dirItem, char *path);
	extern Item  MountMacFileSystem(char *path);
	extern Item  ChangeDirectory(char *path);
	extern Item  GetDirectory(char *pathBuf, int pathBufLen);
	extern Item  CreateFile(char *path);
	extern Err   DeleteFile(char *path);
	extern Item  CreateAlias(char *aliasPath,char *realPath);
	extern int32 LoadOverlay(void *mumble);
	extern Err   DismountFileSystem(char *name);
*/

	// 
	// audio software interrupts
	// 

/*
	Err TweakKnob( Item KnobItem, int32 Value );
	Err StartInstrument( Item Instrument, TagArg *TagList);
	Err ReleaseInstrument( Item Instrument, TagArg *TagList);
	Err StopInstrument( Item Instrument, TagArg *TagList);
	Err TuneInsTemplate( Item InsTemplate, Item Tuning );
	Err TuneInstrument( Item Instrument, Item Tuning );
	Err ReservedAudioSWI1( void );
	Err TestHack( TagArg *args );
	Err ConnectInstruments( Item SrcIns, char *SrcName, Item DstIns, char *DstName);
	uint32 TraceAudio( int32 Mask );
	int32 AllocAmplitude( int32 Amplitude);
	Err FreeAmplitude( int32 Amplitude);
	Err DisconnectInstruments( Item SrcIns, char *SrcName, Item DstIns, char *DstName);
	Err SignalAtTime( Item Cue, AudioTime Time );
	Err SetAudioRate( Item Owner, frac16 Rate); 
	Err SetAudioDuration( Item Owner, uint32 Frames);
	Err TweakRawKnob( Item KnobItem, int32 Value );
	Err StartAttachment( Item Attachment, TagArg *tp );
	Err ReleaseAttachment( Item Attachment, TagArg *tp );
	Err StopAttachment(  Item Attachment, TagArg *tp );
	Err LinkAttachments( Item At1, Item At2 );
	Err MonitorAttachment( Item Attachment, Item Cue, int32 CueAt );
	Err SetMasterTuning( frac16 Frequency );
	Err AbandonInstrument( Item Instrument );
	Item AdoptInstrument( Item InsTemplate );
	Item ScavengeInstrument( Item InsTemplate, uint8 Priority, int32 MaxActivity, int32 IfSystemWide );
	Err SetAudioItemInfo( Item AnyItem, TagArg *tp );
	Err PauseInstrument( Item Instrument );
	Err ResumeInstrument( Item Instrument );
	int32 WhereAttachment( Item Attachment );
	uint32 IncrementGlobalIndex( void );
	Err BendInstrumentPitch( Item Instrument, frac16 BendFrac );
	Err AbortTimerCue( Item Cue );
*/

	// 
	// TODO: missing some audio swi's here

	// 
	// math software interrupts
	// 

/*
	void MulVec3Mat33_F16 (vec3f16 dest, vec3f16 vec, mat33f16 mat);
	void MulMat33Mat33_F16 (mat33f16 dest, mat33f16 src1, mat33f16 src2);
	void MulManyVec3Mat33_F16 (vec3f16 *dest, vec3f16 *src, mat33f16 mat, int32 count);
	void MulObjectVec3Mat33_F16 (void *objectlist[], ObjOffset1 *offsetstruct, int32 count);
	void MulObjectMat33_F16 (void *objectlist[], ObjOffset2 *offsetstruct, mat33f16 mat, int32 count);
	void MulManyF16 (frac16 *dest, frac16 *src1, frac16 *src2, int32 count);
	void MulScalarF16 (frac16 *dest, frac16 *src, frac16 scalar, int32 count);
	void MulVec4Mat44_F16 (vec4f16 dest, vec4f16 vec, mat44f16 mat);
	void MulMat44Mat44_F16 (mat44f16 dest, mat44f16 src1, mat44f16 src2);
	void MulManyVec4Mat44_F16 (vec4f16 *dest, vec4f16 *src, mat44f16 mat, int32 count);
	void MulObjectVec4Mat44_F16 (void *objectlist[], ObjOffset1 *offsetstruct, int32 count);
	void MulObjectMat44_F16 (void *objectlist[], ObjOffset2 *offsetstruct, mat44f16 mat, int32 count);
	frac16 Dot3_F16 (vec3f16 v1, vec3f16 v2);
	frac16 Dot4_F16 (vec4f16 v1, vec4f16 v2);
	void Cross3_F16 (vec3f16 dest, vec3f16 v1, vec3f16 v2);
	frac16 AbsVec3_F16 (vec3f16 vec);
	frac16 AbsVec4_F16 (vec4f16 vec);
	void MulVec3Mat33DivZ_F16 (vec3f16 dest, vec3f16 vec, mat33f16 mat, frac16 n);
	void MulManyVec3Mat33DivZ_F16 (mmv3m33d *s);
*/
}
}

#endif
