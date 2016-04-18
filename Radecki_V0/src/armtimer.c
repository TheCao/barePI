/*
 * armtimer.c
 *
 *  Created on: 14 kwi 2016
 *      Author: Komar
 */

#include "armtimer.h"
#include "raspi.h"
#include "periph.h"

void  arm_timer_freerun_init ( void )
{
    //0xF9+1 = 250
    //250MHz/250 = 1MHz
    PUT32(ARM_TIMER_CTL,0x00F90000); // // Free-run counter prescaler SET
    PUT32(ARM_TIMER_CTL,0x00F90200); // Free-run counter enabled
}
//-------------------------------------------------------------------------
unsigned int arm_timer_freerun_tick ( void )
{
    return(GET32(ARM_TIMER_CNT));
}

// moje dodatki

// obsluga Interrupts

static rpi_irq_controller_t * rpiIRQController = (rpi_irq_controller_t*) RPI_INTERRUPT_CONTROLLER_BASE;

rpi_irq_controller_t* RPI_GetIrqController(void)
{
	return rpiIRQController;
}

// obsluga ARM Timer
static rpi_arm_timer_t* rpiArmTimer = (rpi_arm_timer_t*)RPI_ARM_TIMER_BASE;

rpi_arm_timer_t* RPI_GetArmTimer(void)
{
	return rpiArmTimer;
}

void arm_timer_init(void)
{
	/* Setup the system timer interrupt */
    /* Timer frequency = Clk/256 */
	// Timer zlicza do zera, a potem leci od nowa
	RPI_GetArmTimer()->Load = 0x5000;

	// Setup ARM Timer
	RPI_GetArmTimer()->Control =
			RPI_ARM_TIMER_CTRL_32BIT |
			RPI_ARM_TIMER_CTRL_ENABLE |
			RPI_ARM_TIMER_CTRL_INT_ENABLE |
			RPI_ARM_TIMER_CTRL_PRESCALE_256;
}

unsigned int arm_timer_tick(void)
{
	return(RPI_GetArmTimer()->Value);
}

void __attribute__ ((interrupt("IRQ"))) interrupt_vector(void)
{
	RPI_GetArmTimer()->IRQClear = 1;
	hexstring(0x100000);
}


