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
extern void _enable_interrupts();

__attribute__((no_instrument_function))  VOID not_main(VOID)
{


	    long as;
	    prepare_led_ok();
	    wait(DELAY_10_ms); // wymagane ze wzgledu na uart_sendC, ktore nie wyrabia po wyjsciu z funkcji
	    uart_init();
do{
		as=InitialiseFrameBuffer();
		//hexstring(0x2);
hexstring(as);
}
while (as!=0);
uart_sendC("FrameBuffer_Ready:");
hexstring(fb_info.Pitch);
hexstring(fb_info.Pointer);
hexstring(fb_info.Height);


//inicjalizacja timera
_enable_interrupts;
RPI_GetIrqController() -> Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
/*arm_timer_init();

uart_sendC("ARM Timer & Timer Interrupts Ready");
uart_sendC("LED Test");*/
for(UINT32 i = 0; i < 10; i++)
{
	PUT32(GPSET1,1<<15);
	hexstring(0x0002);
	wait(DELAY_100_ms);
	hexstring(0x0003);
	PUT32(GPCLR1,1<<15);
	hexstring(0x0004);
	wait(DELAY_100_ms);
	hexstring(0x0005);
}


uart_sendC("All ready Ready");

/*	//TestPattern();
//	DrawPixel(121, 160, 0xFFFF);
//	DrawPixel(122, 161, 0xFFFF);

//int x,y;
//register unsigned int * ptr=fb_info.Pointer;

//for(y=0;y<1080*1920;y++)		*ptr++=0;*/

/*
memset ( (void*)fb_info.Pointer, 0, 1920UL*1080UL*4);

for(as=0;as<438;as++)
{*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+as*4))=0x0ffffff;
//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+8)*4))=0x0000ff00;
//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%30)*4))=0x00000ff;
//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%130)*4))=0x0ff0000;
};
*/


BRANCHTO(0x8000);


}
