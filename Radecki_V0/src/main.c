#include "main.h"
#include "GPIO.h"
#include "SystemTimer.h"
#include "FrameBuffer.h"
#include "raspi.h"
#include "string.h"
#include "Mailbox.h"
#include "periph.h"

extern struct FrameBufferInfo fb_info;
extern void BRANCHTO();
extern void rand();
__attribute__((no_instrument_function))  VOID not_main(VOID)
{
	UINT32 result;

	  unsigned int ra;
	    unsigned int rb;
	    long as;
	    ra=GET32(GPFSEL4);
	    ra&=~(7<<21);
	    ra|=1<<21;
	    PUT32(GPFSEL4,ra);
	    uart_init();
	    hexstring(0x1);
do{
		as=InitialiseFrameBuffer();
		hexstring(0x2);
hexstring(as);
}
while (as!=0);

hexstring(fb_info.Pitch);
hexstring(fb_info.Pointer);
hexstring(fb_info.Height);

	//TestPattern();
//	DrawPixel(121, 160, 0xFFFF);
//	DrawPixel(122, 161, 0xFFFF);

int x,y;
register unsigned int * ptr=fb_info.Pointer;

//for(y=0;y<1080*1920;y++)		*ptr++=0;

memset ( (void*)fb_info.Pointer, 0, 1920UL*1080UL*4);

for(as=0;as<438;as++)
{*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+as*4))=0x0ffffff;
//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+8)*4))=0x0000ff00;
//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%30)*4))=0x00000ff;
//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%130)*4))=0x0ff0000;
};
	// Flash LED rapidly if complete
BRANCHTO(0x8000);
while(1){
	      PUT32(GPSET1,1<<(47-32));

		wait(DELAY_100_ms);
		  PUT32(GPCLR1,1<<(47-32));
		wait(DELAY_100_ms);
	}
}
