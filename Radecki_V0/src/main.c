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
	memset ( (UINT32*)fb_info.Pointer, 0, fb_info.Height*fb_info.Width*4);



	wait(DELAY_100_ms);
	DrawRectK(&fb_info, 300,300,100,200,COLOUR_GREEN);
	FillScreen(&fb_info,COLOUR_RED);
	DrawRectK(&fb_info, 1000,1000,100,200,COLOUR_WHITE);
	DrawLineK(&fb_info,100,100,300,VERTICAL,COLOUR_BLUE);
	wait(DELAY_100_ms);
	ClearScreen(&fb_info);
	DrawRectK(&fb_info, 300,300,100,200,COLOUR_GREEN);
	uart_sendC("Zakonczono rysowanie");
	wait(DELAY_10_ms);
	FillScreen(&fb_info,COLOUR_BLUE);
	DrawRectK(&fb_info,300,300,300,300,COLOUR_RED);
	DrawLineK(&fb_info,1000,1000,200,HORIZONTAL,COLOUR_WHITE);
	DrawLineK(&fb_info,1000,1000,200,VERTICAL,COLOUR_WHITE);
	ClearScreen(&fb_info);
	FillScreen(&fb_info,COLOUR_RED);
	DrawRectK(&fb_info,200,150,300,300,COLOUR_BLACK);
	ClearScreen(&fb_info);
	FillScreen(&fb_info,COLOUR_YELLOW);
	DrawRectK(&fb_info,200,150,300,300,COLOUR_ORANGE);


	BRANCHTO(0x8000);


}
