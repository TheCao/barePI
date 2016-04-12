/*
 * periph.h
 *
 *  Created on: 12 kwi 2016
 *      Author: Komar
 */

#ifndef PERIPH_H_
#define PERIPH_H_

#include "DataTypes.h"
// Adres dla RPI v2
#define PBASE 0x3F000000

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

//ponizsze definicje dostalem
#define ARM_TIMER_CTL   (PBASE+0x0000B408)
#define ARM_TIMER_CNT   (PBASE+0x0000B420)

// USUN JESLI WSZYSTKO OKEJ
//#define GPFSEL1         (PBASE+0x00200004)
//#define GPSET0          (PBASE+0x0020001C)
//#define GPCLR0          (PBASE+0x00200028)

#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)

#define AUX_ENABLES     (PBASE+0x00215004)
#define AUX_MU_IO_REG   (PBASE+0x00215040)
#define AUX_MU_IER_REG  (PBASE+0x00215044)
#define AUX_MU_IIR_REG  (PBASE+0x00215048)
#define AUX_MU_LCR_REG  (PBASE+0x0021504C)
#define AUX_MU_MCR_REG  (PBASE+0x00215050)
#define AUX_MU_LSR_REG  (PBASE+0x00215054)
#define AUX_MU_MSR_REG  (PBASE+0x00215058)
#define AUX_MU_SCRATCH  (PBASE+0x0021505C)
#define AUX_MU_CNTL_REG (PBASE+0x00215060)
#define AUX_MU_STAT_REG (PBASE+0x00215064)
#define AUX_MU_BAUD_REG (PBASE+0x00215068)

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1

//---------------------------------------------
unsigned int uart_lcr ( void );
unsigned int uart_recv ( void );
unsigned int uart_check ( void );
void uart_send ( unsigned int c );
void uart_flush ( void );
void hexstrings ( unsigned int d );
void hexstring ( unsigned int d );
void uart_init ( void );
void  arm_timer_freerun_init ( void );
unsigned int arm_timer_freerun_tick ( void );

// moje definicje

// obsluga przerwan ( najpewniej juz zaimplemenotwane )

#define RPI_INTERRUPT_CONTROLLER_BASE ( PBASE + 0xB200)

	// mapa rejestru interrupt controller SET

typedef struct {
	volatile UINT32 IRQ_basic_pending;
	volatile UINT32 IRQ_pending_1;
	volatile UINT32 IRQ_pending_2;
	volatile UINT32 FIQ_control;
	volatile UINT32 Enable_IRQs_1;
	volatile UINT32 Enable_IRQs_2;
	volatile UINT32 Enable_Basic_IRQs;
	volatile UINT32 Disable_IRQs_1;
	volatile UINT32 Disable_IRQs_2;
	volatile UINT32 Disable_Basic_IRQs;
} rpi_irq_controller_t;

// obsluga ARM Timer interrupt ( najpewniej juz zaimplemenotwane )

#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)

// obsluga ARM Timer ( Section 14 BCM@835 Peripherals )

#define RPI_ARM_TIMER_BASE ( PBASE + 0xB400)

// 32 bitowy licznik

#define RPI_ARM_TIMER_CTRL_32BIT ( 1 << 1)

// prescalery

#define RPI_ARM_TIMER_CTRL_PRESCALE_1 ( 0 << 2)
#define RPI_ARM_TIMER_CTRL_PRESCALE_16 ( 1 << 2 )
#define RPI_ARM_TIMER_CTRL_PRESCALE_256 ( 2 << 2 )

// ARM Timer interrupt
#define RPI_ARM_TIMER_CTRL_INT_ENABLE ( 1 << 5 )
#define RPI_ARM_TIMER_CTRL_INT_DISABLE ( 0 << 5 )

// ARM Timer

#define RPI_ARM_TIMER_CTRL_ENABLE ( 1 << 7)
#define RPI_ARM_TIMER_CTRL_DISABLE ( 0 << 7 )


typedef struct {
	volatile UINT32 Load;
	volatile UINT32 Value;
	volatile UINT32 Control;
	volatile UINT32 IRQClear;
	volatile UINT32 RAWIRQ;
	volatile UINT32 MaskedIRQ;
	volatile UINT32 Reload;
	volatile UINT32 PreDivider;
	volatile UINT32 FreeRunCounter;

} rpi_arm_timer_t;

unsigned int arm_timer_tick(void);
void arm_timer_init(void);
extern rpi_irq_controller_t;
rpi_irq_controller_t* RPI_GetIrqController(void)
#endif /* PERIPH_H_ */
