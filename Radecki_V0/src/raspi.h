#include "DataTypes.h"

#ifndef raspi_h
#define raspi_h

#define NULL				0
#define TRUE				1
#define FALSE				0

#define ABS(a)				((a)>0?(a):-(a))
#define MIN(a,b)			((a)<(b)?(a):(b))
#define MAX(a,b)			((a)>(b)?(a):(b))
#define CLAMP(min,x,max)	((x)>(max)?(max):((x)<(min)?(min):(x)))

#define SET_BITS(data, mask)	((data)|=(mask))
#define CLEAR_BITS(data, mask)	((data)&=(~(mask)))
#define CHANGE_BITS(data, mask)	((data)^=(mask))
#define TEST_BITS(data, mask)	(((data)&(mask)) != 0)
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define READ32	(addr) 		(*(VUINT32*)	(addr))
#define READ16	(addr) 		(*(VUINT16*)	(addr))
#define READ8	(addr) 		(*(VUINT8*)		(addr))

#define WRITE32	(addr,value) (*(VUINT32*)	(addr) = (VUINT32)	(value))
#define WRITE16	(addr,value) (*(VUINT16*)	(addr) = (VUINT16)	(value))
#define WRITE8	(addr,value) (*(VUINT8*)	(addr) = (VUINT8)	(value))

// function prototypes
extern VOID 			PUT32 ( UINT32, UINT32 );
extern UINT32		 	GET32 ( UINT32 );
extern VOID				PUT16 ( UINT32, UINT32 );
extern UINT16			GET16 ( UINT32 );
extern VOID				MemoryBarrier();

UINT32 PhysicalToARM(VOID *p);
#endif
