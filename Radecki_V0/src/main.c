#include "main.h"
#include "GPIO.h"
#include "SystemTimer.h"
#include "FrameBuffer.h"
#include "raspi.h"
#include "string.h"
#include "Mailbox.h"
#include "periph.h"
#include "armtimer.h"

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
	uart_sendC("FrameBuffer data (Pitch/Pointer/Height) :");
	hexstring(fb_info.Pitch);
	hexstring(fb_info.Pointer);
	hexstring(fb_info.Height);

	memset ( (void*)fb_info.Pointer, 0, 1920UL*1080UL*4);
	for(as=0;as<1080;as++)
	{*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+as*4))=0x0ffffff;
	*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+8)*4))=0x0000ff00;
	*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%30)*4))=0x00000ff;
	*((long*)(fb_info.Pointer+(fb_info.Pitch*as)+(as+rand()%130)*4))=0x0ff0000;
	};



	BRANCHTO(0x8000);


}
