#include "GPIO.h"
#include "SystemTimer.h"
#include "raspi.h"

VOID SetPinFunction(UINT32 pin_number, UINT32 pin_function)
{
	UINT32 function_block;
	UINT32 sel;
	UINT32 mask;
	UINT32 function;
	
	// Validate inputs
	if (pin_number > 53){
		return;
	}

	if (pin_function > 7){
		return;
	}

	// Calculate correct register position
	pin_number %= 10;
	function_block = pin_number / 10;
	
	pin_number *= 3;
	pin_function = pin_function << pin_number;
	
	sel = GET32(GPBASE + (function_block * 4));

	mask = 0x7;
	mask = ~(mask << pin_number);
	function = sel;
	function &= mask;
	
	pin_function |= function;

	PUT32(GPBASE + (function_block * 4), pin_function);
	
	return;
}

VOID SetPin(UINT32 pin_number)
{
	PUT32(GPCLR0, 1 << pin_number);
	return;
}

VOID ClearPin(UINT32 pin_number)
{
	PUT32(GPSET0, 1 << pin_number);
	return;
}

VOID ErrorFlash(UINT32 number)
{
	UINT8 i;
	
	while(1){
		for(i = 0; i < number; i++){
			SetPin(GPIO_PIN_16);
			wait(DELAY_250_ms);
			ClearPin(GPIO_PIN_16);
			wait(DELAY_250_ms);
		}
		wait(DELAY_1_s);
	}
	return;
}
