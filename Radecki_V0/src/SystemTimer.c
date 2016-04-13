#include "SystemTimer.h"
#include "raspi.h"

VOID wait(UINT32 microseconds)
{
	UINT32 counter_lo = GET32(ST_CLO);
	UINT32 start = counter_lo;
	UINT32 elapsed;
	
	elapsed = GET32(ST_CLO) - start;
	while (elapsed < microseconds){
		elapsed = GET32(ST_CLO) - start;
	}
	return;
}
