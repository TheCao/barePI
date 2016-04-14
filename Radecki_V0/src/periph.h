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



// USUN JESLI WSZYSTKO OKEJ
//#define GPFSEL1         (PBASE+0x00200004)
//#define GPSET0          (PBASE+0x0020001C)
//#define GPCLR0          (PBASE+0x00200028)

#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)
// BCM2835 Arm Peripheral Manual - Section 2.1
#define AUX_ENABLES     (PBASE+0x00215004)
#define AUX_MU_IO_REG   (PBASE+0x00215040) //Mini uart I/O Data
#define AUX_MU_IER_REG  (PBASE+0x00215044) // Interrupt Enable
#define AUX_MU_IIR_REG  (PBASE+0x00215048) // Interrupt Identify
#define AUX_MU_LCR_REG  (PBASE+0x0021504C) // Line Control
#define AUX_MU_MCR_REG  (PBASE+0x00215050) // Modem Control
#define AUX_MU_LSR_REG  (PBASE+0x00215054) // Line Status
#define AUX_MU_MSR_REG  (PBASE+0x00215058) // Modem Status
#define AUX_MU_SCRATCH  (PBASE+0x0021505C) // Uart Scratch
#define AUX_MU_CNTL_REG (PBASE+0x00215060) // Extra Control
#define AUX_MU_STAT_REG (PBASE+0x00215064) // Extra Status
#define AUX_MU_BAUD_REG (PBASE+0x00215068) // Baudrate

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


void uart_sendC(char *buff);
void prepare_led_ok(void);
#endif /* PERIPH_H_ */
