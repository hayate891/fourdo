#ifndef _INC_SWI_KERNELTYPES_MEM
#define _INC_SWI_KERNELTYPES_MEM


//////////////////////////////////////////////////////////////
// Definitions of memory allocation flags

/* define location, size flags */
#define MEMTYPE_ANY		        ((uint32)0x00000000)

/* low 8 bits are optional fill value */
#define MEMTYPE_MASK		    ((uint32)0xffffff00)
#define MEMTYPE_FILLMASK	    ((uint32)0x000000ff)

#define MEMTYPE_FILL		    ((uint32)0x00000100) /* fill memory with value */
#define MEMTYPE_MYPOOL		    ((uint32)0x00000200) /* do not get more memory from system */
#define MEMTYPE_FROMTOP		    ((uint32)0x00004000) /* allocate from top */
#define MEMTYPE_TASKMEM		    ((uint32)0x00008000)	/* internal use only */

/* memory type bits */
#define MEMTYPE_VRAM		    ((uint32)0x00010000) /* block must be in VRAM */
#define MEMTYPE_DMA		        ((uint32)0x00020000) /* accessable by hardware */
#define MEMTYPE_CEL		        ((uint32)0x00040000) /* accessable by sprite engine */
#define MEMTYPE_DRAM		    ((uint32)0x00080000) /* block must not be in VRAM */

#define MEMTYPE_AUDIO		    ((uint32)0x00100000) /* accessable by audio  */
#define MEMTYPE_DSP		        ((uint32)0x00200000) /* accessable by DSP */

/* alignment bits */
#define MEMTYPE_INPAGE		    ((uint32)0x01000000) /* no page crossings */
#define MEMTYPE_STARTPAGE	    ((uint32)0x02000000) /* block must start on VRAM boundary */
#define MEMTYPE_SYSTEMPAGESIZE	((uint32)0x04000000) /* block must start on system page boundary */

	/* MEMTYPE_VRAM sets PAGESIZE=VRAM PAGESIZE */
	/* otherwise PAGESIZE = physical page size of memory protect system */

#define MEMF_ALIGNMENTS	        ((MEMTYPE_INPAGE|MEMTYPE_STARTPAGE)

/* VRAM bank select bits */
#define MEMTYPE_BANKSELECT	    ((uint32)0x40000000) /* bank required */
#define MEMTYPE_BANKSELECTMSK	((uint32)0x30000000) /* 2 max banks */
#define MEMTYPE_BANK1		    ((uint32)0x10000000) /* first bank */
#define MEMTYPE_BANK2		    ((uint32)0x20000000) /* second bank */

#define GETBANKBITS(a)          ((MEMTYPE_BANKSELECT|MEMTYPE_BANKSELECTMSK)&GetMemType(a))
#define GetBankBits(a)          ((MEMTYPE_BANKSELECT|MEMTYPE_BANKSELECTMSK)&GetMemType(a))

//////////////////////////////////////////////////////////////

#endif