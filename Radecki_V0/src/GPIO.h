#ifndef GPIO_h
#define GPIO_h

#include "DataTypes.h"

// GPIO Registers

// GPIO base address
#define GPBASE	0x3f200000

// GPIO function select registers
#define GPFSEL0 0x3f200000				// R/W
#define GPFSEL1 0x3f200004				// R/W
#define GPFSEL2 0x3f200008				// R/W
#define GPFSEL3 0x3f20000C				// R/W
#define GPFSEL4 0x3f200010				// R/W
#define GPFSEL5 0x3f200014				// R/W

// GPIO output set
#define GPSET0  0x3f20001C				//   W
#define GPSET1  0x3f200020				//   W

// GPIO output clear
#define GPCLR0  0x3f200028				//   W
#define GPCLR1  0x3f20002C				//   W

// GPIO pin level
#define GPLEV0	0x3f200034				// R
#define GPLEV1	0x3f200038				// R

// GPIO pin event detect status
#define GPEDS0	0x3f200040				// R/W
#define GPEDS1	0x3f200044				// R/W

// Useful defines
// GPIO pin functions
#define GPIO_INPUT	0
#define GPIO_OUTPUT 1

// GPIO output states
#define GPIO_SET	1
#define GPIO_CLR	0

// GPIO pin numbers
#define GPIO_PIN_0		0		// Pulled high
#define GPIO_PIN_1		1		// Pulled high
#define GPIO_PIN_2		2		// Pulled high
#define GPIO_PIN_3		3		// Pulled high
#define GPIO_PIN_4		4		// Pulled high
#define GPIO_PIN_5		5		// Pulled high
#define GPIO_PIN_6		6		// Pulled high
#define GPIO_PIN_7		7		// Pulled high
#define GPIO_PIN_8		8		// Pulled high
#define GPIO_PIN_9		9		// Pulled low
#define GPIO_PIN_10		10		// Pulled low
#define GPIO_PIN_11		11		// Pulled low
#define GPIO_PIN_12		12		// Pulled low
#define GPIO_PIN_13		13		// Pulled low
#define GPIO_PIN_14		14		// Pulled low
#define GPIO_PIN_15		15		// Pulled low
#define GPIO_PIN_16		16		// Pulled low
#define GPIO_PIN_17		17		// Pulled low
#define GPIO_PIN_18		18		// Pulled low
#define GPIO_PIN_19		19		// Pulled low
#define GPIO_PIN_20		20		// Pulled low
#define GPIO_PIN_21		21		// Pulled low
#define GPIO_PIN_22		22		// Pulled low
#define GPIO_PIN_23		23		// Pulled low
#define GPIO_PIN_24		24		// Pulled low
#define GPIO_PIN_25		25		// Pulled low
#define GPIO_PIN_26		26		// Pulled low
#define GPIO_PIN_27		27		// Pulled low
#define GPIO_PIN_28		28		// Pulled low
#define GPIO_PIN_29		29		// Pulled low
#define GPIO_PIN_30		30		// Pulled low
#define GPIO_PIN_31		31		// Pulled low
#define GPIO_PIN_32		32		// Pulled low
#define GPIO_PIN_33		33		// Pulled low
#define GPIO_PIN_34		34		// Pulled low
#define GPIO_PIN_35		35		// Pulled low
#define GPIO_PIN_36		36		// Pulled low
#define GPIO_PIN_37		37		// Pulled low
#define GPIO_PIN_38		38		// Pulled low
#define GPIO_PIN_39		39		// Pulled low
#define GPIO_PIN_40		40		// Pulled low
#define GPIO_PIN_41		41		// Pulled low
#define GPIO_PIN_42		42		// Pulled low
#define GPIO_PIN_43		43		// Pulled low
#define GPIO_PIN_44		44		// Pulled low
#define GPIO_PIN_45		45		// Pulled low
#define GPIO_PIN_46		46		// Pulled low
#define GPIO_PIN_47		47		// Pulled low
#define GPIO_PIN_48		48		// Pulled low
#define GPIO_PIN_49		49		// Pulled low
#define GPIO_PIN_50		50		// Pulled low
#define GPIO_PIN_51		51		// Pulled low
#define GPIO_PIN_52		52		// Pulled low
#define GPIO_PIN_53		53		// Pulled low

// function prototypes
VOID SetPinFunction(UINT32 pin_number, UINT32 pin_function);
VOID SetPin(UINT32 pin_number);
VOID ClearPin(UINT32 pin_number);
VOID ErrorFlash(UINT32 number);

#endif
