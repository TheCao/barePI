#include "main.h"
#include "GPIO.h"
#include "SystemTimer.h"
#include "FrameBuffer.h"
#include "raspi.h"
#include "string.h"
#include "Mailbox.h"
#include "periph.h"

extern void BRANCHTO();
extern void rand();
extern void _enable_interrupts();
extern void takasobiefunkcjadlatest();
__attribute__((no_instrument_function))  VOID not_main(VOID)
{


	long as;
	prepare_led_ok();
	wait(DELAY_10_ms); // wymagane ze wzgledu na uart_sendC, ktore nie wyrabia po wyjsciu z funkcji
	uart_init();



	for(UINT32 i = 0; i < 10; i++)
	{
		PUT32(GPSET1,1<<15);

		wait(DELAY_100_ms);

		PUT32(GPCLR1,1<<15);

		wait(DELAY_100_ms);

	}




	BRANCHTO(0x8000);


}
