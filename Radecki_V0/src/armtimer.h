/*
 * armtimer.h
 *
 *  Created on: 14 kwi 2016
 *      Author: Komar
 */

#ifndef ARMTIMER_H_
#define ARMTIMER_H_

#include "DataTypes.h"

#define PBASE 0x3F000000

//ponizsze definicje dostalem
#define ARM_TIMER_CTL   (PBASE+0x0000B408)
#define ARM_TIMER_CNT   (PBASE+0x0000B420)

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

// obsluga ARM Timer interrupt

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
rpi_irq_controller_t* RPI_GetIrqController(void);
rpi_arm_timer_t* RPI_GetArmTimer(void);

void  arm_timer_freerun_init ( void );
unsigned int arm_timer_freerun_tick ( void );


#endif /* ARMTIMER_H_ */
