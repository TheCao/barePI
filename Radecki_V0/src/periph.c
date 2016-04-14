#include "periph.h"
#include "GPIO.h"

//------------------------------------------------------------------------
unsigned int uart_lcr ( void )
{
    return(GET32(AUX_MU_LSR_REG));
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01) break;
    }
    return(GET32(AUX_MU_IO_REG)&0xFF);
}
//------------------------------------------------------------------------
unsigned int uart_check ( void )
{
    if(GET32(AUX_MU_LSR_REG)&0x01) return(1);
    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);
}
//------------------------------------------------------------------------
void uart_flush ( void )
{
    while(1)
    {
        if((GET32(AUX_MU_LSR_REG)&0x100)==0) break;
    }
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20); //space
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D); // New Line
    uart_send(0x0A);
}
void uart_sendC(char *buff)
{
	while(* buff)
	{
		uart_send(*buff);
		buff++;
	}
	uart_send(0x0D);
	uart_send(0x0A);
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    PUT32(AUX_ENABLES,1); //Mini UART Enable
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);
    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);
    PUT32(GPPUD,0); // disable pull-up/down reg.
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0); // No effect for pull-up down Clock Reg
    PUT32(AUX_MU_CNTL_REG,3); //Transmiter and Receiver enabled
}
//------------------------------------------------------------------------
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
	RPI_GetArmTimer()->Load = 0x100;

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
void prepare_led_ok(void)
{
	UINT32 ra;
	ra=GET32(GPFSEL4); // GPIO Function Select 4
	ra&=~(7<<21); // wyzerowanie 21-23 bitow
    ra|=1<<21; // output na 21 bicie odpowiedzialnym za GPIO47 ( LED_OK )
    PUT32(GPFSEL4,ra);
    uart_sendC("LED_OK_Ready");
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------