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
extern void rand();
extern void _enable_interrupts();

__attribute__((no_instrument_function))  VOID not_main(VOID)
{


	long as;
	prepare_led_act();
	prepare_led_pwr();
	wait(DELAY_10_ms); // wymagane ze wzgledu na uart_sendC, ktore nie wyrabia po wyjsciu z funkcji
	uart_init();



	for(UINT32 i = 0; i < 10; i++)
	{
		led_act_ON();
		led_pwr_ON();
		wait(DELAY_100_ms);

		led_act_OFF();
		led_pwr_OFF();

		wait(DELAY_100_ms);

	}
	uart_sendC("ARM Timer interrupts");
	_enable_interrupts;
	arm_timer_init();

	BRANCHTO(0x8000);


}
