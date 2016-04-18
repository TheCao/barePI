#include "main.h"
#include "GPIO.h"
#include "SystemTimer.h"
#include "FrameBuffer.h"
#include "raspi.h"
#include "string.h"
#include "Mailbox.h"
#include "periph.h"
#include "armtimer.h"
#include "kolory.h"
////////////////////////////////

extern void BRANCHTO();
extern void _enable_interrupts();
extern struct FrameBufferInfo fb_info;

__attribute__((no_instrument_function))  VOID not_main(VOID)
{
	long as;
	prepare_led_act();
	prepare_led_pwr();
	wait(DELAY_10_ms); // wymagane ze wzgledu na uart_sendC, ktore nie wyrabia po wyjsciu z funkcji
	uart_init();

// ARM Timer Interrupt Controller
	/*RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
	arm_timer_init();
	_enable_interrupts();
	RPI_GetArmTimer()->Load = 0x3000;

	//na koniec potrzeba zatrzymac timer
	arm_timer_stop();
*/

	do{
			as=InitialiseFrameBuffer();
	}
	while (as!=0);
	uart_sendC("FrameBuffer_Ready:");
	uart_sendC("Pointer: ");
	hexstring(as);
	uart_sendC("FrameBuffer data (Pitch/Pointer/Height/Width) :");
	hexstring(fb_info.Pitch);
	hexstring(fb_info.Pointer);
	hexstring(fb_info.Height);
	hexstring(fb_info.Width);

	//memset ( (void*)fb_info.Pointer, 0, fb_info.Height*fb_info.Width*fb_info.Depth); // w przypadku fb_info.Depth = 32 inicjalizacja bufora trwa strasznie dlugo
	memset ( (void*)fb_info.Pointer, 0, fb_info.Height*fb_info.Width*1);
	UINT32 x0 = 0x3C0;
	UINT32 y0 = 0x90;
	DrawRect(fb_info, x0,y0,500,300,0x000000ff);
	//for(as=0;as<500;as++) //szerokosc
	//{
		//for(long dy=0;dy<500;dy++) // wysokosc
			//{
			//*((long*)(fb_info.Pointer+x*4+(as*4)+(4*bs*fb_info.Width)))=COLOUR_BLUE;
			//*((long*)(fb_info.Pointer+(x0*4)+(4*y0*fb_info.Width)+(as*4)+(dy*4*fb_info.Width)))=COLOUR_GREEN;
			//}
	//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+8)*4))=0x0000ff00;
	//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%30)*4))=0x00000ff;
	//*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%130)*4))=0x0ff0000;
	//};

	uart_sendC("Zakonczono rysowanie");
	wait(DELAY_10_ms);


	BRANCHTO(0x8000);


}
