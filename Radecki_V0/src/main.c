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
extern VUINT16 globalCounter;
__attribute__((no_instrument_function))  VOID not_main(VOID)
{
	long as;
	prepare_led_act();
	prepare_led_pwr();
	wait(DELAY_10_ms); // wymagane ze wzgledu na uart_sendC, ktore nie wyrabia po wyjsciu z funkcji
	uart_init();

	//ARM Timer Interrupt Controller
	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
	arm_timer_init();
	_enable_interrupts();





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
	uart_sendC("Breakpoint 1");
	//memset ( (void*)fb_info.Pointer, 0, fb_info.Height*fb_info.Width*fb_info.Depth); // w przypadku fb_info.Depth = 32 inicjalizacja bufora trwa strasznie dlugo

	// usuniecie pozwolilo na ruszenie programu
	//memset ( (UINT32*)fb_info.Pointer, 0, fb_info.Height*fb_info.Width*4);
	uart_sendC("Breakpoint 2");


	FillScreen(&fb_info,COLOUR_WHITE);
	uart_sendC("Breakpoint 3");
	RPI_GetArmTimer()->Load = 0x800;
	uart_sendC("Breakpoint 4");
	globalCounter = 0 ;
	while(globalCounter < 25)
	{
		//pass
	}

	//na koniec potrzeba zatrzymac timer
	uart_sendC("Breakpoint 5");
	arm_timer_stop();
	uart_sendC("Breakpoint 6");
	FillScreen(&fb_info,COLOUR_BLACK); // poniewaz ClearScreen zawiesza program

	BRANCHTO(0x8000);


}
