#include "DataTypes.h"

// SystemTimer Registers

#define ST_CS			0x3f003000		// System timer - control/status
#define ST_CLO			0x3f003004		// System timer counter low 32-bits
#define ST_CHI			0x3f003008		// System timer counter high 32-bits
#define ST_C0			0x3f00300C		// System timer compare 0
#define ST_C1			0x3f003010		// System timer compare 1
#define ST_C2			0x3f003014		// System timer compare 2
#define ST_C3			0x3f003018		// System timer compare 3

// Useful defines
#define DELAY_1_s			1000000
#define DELAY_500_ms		 500000
#define DELAY_250_ms		 250000
#define DELAY_100_ms		 100000
#define DELAY_10_ms		 	  10000

// Function prototypes
VOID wait(UINT32 microseconds);
