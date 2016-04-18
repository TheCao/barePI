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
void c_irq_handler(void)
{
	RPI_GetArmTimer()->IRQClear = 1;
	uart_sendC("Interrupt");
	hexstring(0x9999);
}
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
	for(volatile UINT32 i = 0; i < 4 ; i++)
		{
			uart_sendC("W petli for");
			hexstring(i);
			wait(DELAY_1_s);
		}

	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
	arm_timer_init();
	_enable_interrupts();
	uart_sendC("Wejscie w petle");
	for(UINT32 i = 0; i < 10 ; i++)
	{
		uart_sendC("W petli for2");
					hexstring(i);
					wait(DELAY_1_s);
	}
	//TODO: Sprawdziæ czy kompilacja dzia³a tak jak dzia³a ostatnio.
	BRANCHTO(0x8000);


}
