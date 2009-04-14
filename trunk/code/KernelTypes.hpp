#ifndef _INC_SWI_KERNELTYPES
#define _INC_SWI_KERNELTYPES

#include "KernelTypes_Mem.hpp"

typedef uint32 Err;

typedef uint32 Item;
typedef uint32 ItemPtr;
typedef uint32 MemPtr;

typedef uint32 MemHdrPtr;
typedef uint32 MemListPtr;
typedef uint32 MemInfoPtr;
typedef uint32 ListPtr;
typedef uint32 TagArgPtr;
typedef uint32 FunctionPtr;
typedef uint32 TaskPtr;
typedef uint32 NodePtr;
typedef uint32 MinNodePtr;
typedef uint32 CharPtr;
typedef uint32 FuncPtr;

typedef void List;
typedef void TagArg;

//////////////////////////////////////////////////////////////////////////

/* Standard Node structure */
typedef struct
{
    NodePtr n_Next;	        /* pointer to next node in list */
    NodePtr n_Prev;	        /* pointer to previous node in list */
    uint8   n_Flags;		/* misc flags, see below */
    uint8   n_Priority;	    /* queueing priority */
    uint8   n_Type;		    /* what type of node for the folio */
    uint8   n_SubsysType;   /* what folio manages this node */
    int32   n_Size;		    /* total size of node including hdr */
    
    /* Optional part starts here */
    CharPtr n_Name;		/* ptr to null terminated string or NULL */
} NodeStruct;

/* Node structure used when the Name is not needed */
typedef struct
{
    MemPtr n_Next;       // (struct NamelessNode *)
    MemPtr n_Prev;       // (struct NamelessNode *)
    uint8  n_Flags;
    uint8  n_Priority;
    uint8  n_Type;
    uint8  n_SubsysType;
    int32  n_Size;
} NamelessNodeStruct;

typedef struct
{
    MemPtr           n_Next;        /* pointer to next itemnode in list */  // (struct ItemNode *)
    MemPtr           n_Prev;        /* pointer to previous itemnode in list */ // (struct ItemNode *)
    uint8            n_Flags;		/* misc flags, see below */
    uint8            n_Priority;	/* queueing priority */
    uint8            n_Type;		/* what type of node for the folio */
    uint8            n_SubsysType;	/* what folio manages this node */
    int32            n_Size;		/* total size of node including hdr */
    CharPtr          n_Name;		/* ptr to null terminated string or NULL */
    uint8            n_ItemFlags;	/* additional system item flags */
    uint8            n_FolioFlags;	/* flags for this item's folio */
    uint8            n_Revision;	/* revision of this itemnode */
    uint8            n_Version;	    /* version of of this itemnode */
    Item             n_Item;		/* ItemNumber for this data structure */
    Item             n_Owner;		/* creator, present owner, disposer */
    MemPtr           n_ReservedP;	/* Reserved pointer */ 
} ItemNodeStruct;


typedef struct
{
    ItemNodeStruct  fn;
    int32           f_OpenCount;
    uint8           f_MaxNodeType;
    uint8           f_MaxUserFunctions;
    uint8           f_MaxSwiFunctions;
    uint8           f_TaskDataIndex;
    MemPtr          f_NodeDB;             // (NodeData *)
    FuncPtr         OpenFolioFunction;    // int32 (*f_OpenFolio)(struct Folio *f)
    FuncPtr         CloseFolioFunction;   // void (*f_CloseFolio)(struct Folio *f)
    FuncPtr         DeleteFolioFunction;  // int32 (*f_DeleteFolio)(struct Folio *f)

    /* Folio helper routines for its Items */
    MemPtr          f_ItemRoutines;  //(ItemRoutines *)
    
    /* routines for task creation/deletion swapping */
    FuncPtr         FolioCreateTaskFunction;  // int32  (*f_FolioCreateTask)(struct Task *t,TagArg *tagpt)
    FuncPtr         FolioDeleteTaskFunction;  // void (*f_FolioDeleteTask)(struct Task *t)
    FuncPtr         FolioRestoreTaskFunction; // void (*f_FolioRestoreTask)(struct Task *t)
    FuncPtr         FolioSaveTaskFunction;    // void (*f_FolioSaveTask)(struct Task *t)

    /* reserved for future expansion */
    MemPtr          f_DebugTable;
    int32        	reserved[7];
} FolioStruct;

typedef struct
{
	MinNodePtr      n_Next;
	MinNodePtr      n_Prev;
} MinNodeStruct;

typedef struct
{
    int32           tv_sec;             /* seconds */
    int32           tv_usec;            /* and microseconds */
} TimeValStruct;

typedef struct MemList
{
	NodeStruct      meml_n;	            /* need to link these together */
	uint32          meml_Types;	        /* copy of meml_mh->memh_Types;*/
	MemPtr          meml_OwnBits;	    /* mem we own */
	MemPtr          meml_WriteBits;	    /* mem we can write to */
	MemHdrPtr       meml_MemHdr;
	ListPtr         meml_l;
	Item            meml_Sema4;
	uint8           meml_OwnBitsSize;	/* in uint32s (fd_set) */
	uint8           meml_Reserved[3];
} MemListStruct;

typedef struct
{
    ItemNodeStruct  t;
    TaskPtr         t_ThreadTask;	    /* Am I a thread of what task? */
    ItemPtr         t_ResourceTable;	/* list of Items we need to clean up */
    int32	        t_ResourceCnt;	    /* maxumum number of slots in ResourceTable */
    uint32	        t_WaitBits;	        /* What will wake us up */
    uint32	        t_SigBits;
    uint32	        t_AllocatedSigs;
    MemPtr	        t_StackBase;	    /* ptr to Base of stack */
    int32	        t_StackSize;
    uint32	        t_regs[13];	        /* all current mode registers */
    MemPtr          t_sp;	            /* r13 */
    uint32	        t_lk;	            /* r14 */
    uint32	        t_pc;	            /* r15 */
    uint32	        t_psr;		        /* program status register */
    uint32	        t_Userpsr;	        /* " */
    MemPtr          t_ssp;		        /* ptr to supervisor stack */
    MemPtr          t_Usersp;	        /* in case we are in super mode */
    uint32	        t_Userlk;	        /* " */
    uint32	        t_Reserved;
    int32	        t_SuperStackSize;
    MemPtr          t_SuperStackBase;
    MemPtr          t_ObsoleteMemProtectBits;	/* mem we can write to */
                    /* above moved to MemList */
    ListPtr         t_FreeMemoryLists;	/* task free memory pool */
    MemPtr          t_FolioData;	    /* preallocated ptrs for */
                                        /* the first 4 folios */

    uint32	        t_FolioContext;	    /* 32 bits */
    TimeValStruct   t_ElapsedTime;

    uint32	        t_MaxTicks;	        /* max tics b4 task switch */
    uint32	        t_MaxUSecs;	        /* Equivalent in usecs */
    MemPtr          t_PageTable;	    /* Arm600 page table for this task */
    MemPtr          t_ssl;		        /* ptr to supervisor stack */
    ListPtr         t_UserStackList;   /* List of available user stacks for this process */
    uint32	        t_Flags;	        /* more task specific flags */
    MinNodeStruct   t_TasksLinkNode;    /* Link to the list of all tasks */
} TaskStruct;

typedef struct
{
    FolioStruct  kb;
    ListPtr      kb_RomTags;
    ListPtr      kb_MemFreeLists;
    ListPtr      kb_MemHdrList;
    ListPtr      kb_FolioList;	       /* Libraries */
    ListPtr      kb_Drivers;
    ListPtr      kb_Devices;
    ListPtr      kb_TaskWaitQ;	       /* Tasks waiting for some event */
    ListPtr      kb_TaskReadyQ;        /* Tasks waiting for CPU time */
    ListPtr      kb_MsgPorts;	       /* will we be message based? */
    ListPtr      kb_Semaphores;	       /* will we be message based? */
    TaskPtr      kb_CurrentTask;	   /* Currently executing Task */
    MemPtr       kb_InterruptHandlers; // table of nodes (Node**)
    uint32       kb_TimerBits;	       /* allocated timers/ctrs */
    uint32       kb_ElapsedQuanta;	   /* timerticks for current task */
    MemPtr       kb_VRAMHack;
    MemPtr       kb_ItemTable;	       /* table of ptrs to ItemEntries (ItemEntry**) */
    int32        kb_MaxItem;
    uint32       kb_CPUFlags;	       /* various flags for operation */
    uint8        kb_PleaseReschedule;
    uint8        kb_FolioTableSize;
    uint8        kb_Forbid;	           /* software lockout for task swapping */
    uint8        kb_MaxInterrupts;
    MemPtr       kb_MacPkt;
    uint32       kb_Flags;
    uint32       kb_Reserved;
    uint32       kb_numticks;	       /* convert secs to ticks numerator */
    uint32       kb_denomticks;	       /* convert secs to ticks denominator */
    uint32       kb_Obsolete;	       /* shadow copy of Madam->Msysbits */
    uint8        kb_VRAMSetSize;
    uint8        kb_DRAMSetSize;
    uint8        kb_FolioTaskDataSize; /* lwords */
    uint8        kb_FolioTaskDataCnt;  /* lwords */
    MemPtr       kb_DataFolios;        // (struct Folio **)
    MemPtr       kb_CatchDataAborts;   /* setjmp buf (jmp_buf*) */
    uint32       kb_QuietAborts;	   /* no messages for these bits */
    MemPtr       kb_RamDiskAddr;	   /* kernel needs to help RamDevice */
    int32	     kb_RamDiskSize;
    ListPtr      kb_ExtendedErrors;	   /* list of extended err tables */
    uint8	     kb_Resbyte1;
    uint8	     kb_Resbyte0;
    uint8      	 kb_ClioRev;
    uint8      	 kb_MadamRev;
    Item	     kb_DevSemaphore;	   /* Device List Semaphore */
    ListPtr      kb_SystemStackList;   /* List of System stacks available */
    uint32	     kb_Pad;
    MemPtr       kb_VRAM0;	           /* memory reserved by kernel */
    uint32	     kb_VRAM0Size;
    MemPtr       kb_VRAM1;
    uint32	     kb_VRAM1Size;
    CharPtr      kb_BootVolumeName;
    ListPtr      kb_Tasks;		       /* All Tasks in the system */
} KernelStruct;

#endif