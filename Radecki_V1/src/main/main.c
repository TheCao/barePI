//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/string.h>
#include <uspienv/assert.h>
#include <uspienv/util.h>

#include <uspienv/types.h>
#include "gamepad.h"
#include "myfunc.h"

//static const char FromSample[] = "Message";
extern boolean isMouseConnected;
extern boolean isKeyboardConnected;
extern boolean isGamepadConnected;
extern boolean isChartPrinted;
extern int horizontalAxis;
extern int verticalAxis;
extern motorParams_t basicMotor, basicMotor2;
extern motorParams_t copyBasicMotor, copyBasicMotor2;
extern simulationParams_t basicSimulation;
extern const USPiGamePadState *pStatus;
extern void KeyPressedHandler (const char *pString);
extern void MouseStatusHandler();
extern void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState);

int main (void)
{
	if (!USPiEnvInitialize ())
	{
		return EXIT_HALT;
	}
	
	if (!USPiInitialize ())
	{
		LogWrite ("USPi Error", LOG_ERROR, "Cannot initialize USPi");

		USPiEnvClose ();

		return EXIT_HALT;
	}
	
	if (!USPiKeyboardAvailable ())
	{
		LogWrite ("USPi Error", LOG_ERROR, "Keyboard not found");
	}
	else
	{
		isKeyboardConnected = TRUE;
		USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);
	}
	
	int nGamePads = USPiGamePadAvailable ();
	if (nGamePads < 1)
	{
		LogWrite ("USPi Error", LOG_ERROR, "GamePad not found");

	}
	else
	{
		USPiGamePadRegisterStatusHandler (GamePadStatusHandler);
		isGamepadConnected = TRUE;

		for (unsigned nGamePad = 0; nGamePad < (unsigned) nGamePads; nGamePad++)
			{
				TUSPiDeviceInformation Info;
				if (!USPiDeviceGetInformation (GAMEPAD_CLASS, nGamePad, &Info))
				{
					LogWrite ("USPi Error", LOG_ERROR, "Cannot get device information");

					USPiEnvClose ();

					return EXIT_HALT;
				}

				LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: Vendor 0x%X Product 0x%X Version 0x%X",
					  nGamePad+1, (unsigned) Info.idVendor, (unsigned) Info.idProduct, (unsigned) Info.bcdDevice);

				LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: Manufacturer \"%s\" Product \"%s\"",
					  nGamePad+1, Info.pManufacturer, Info.pProduct);

				const USPiGamePadState *pState = USPiGamePadGetStatus (nGamePad);
				assert (pState != 0);

				LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: %d Buttons %d Hats",
					  nGamePad+1, pState->nbuttons, pState->nhats);

				for (int i = 0; i < pState->naxes; i++)
				{
					LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: Axis %d: Minimum %d Maximum %d",
						  nGamePad+1, i+1, pState->axes[i].minimum, pState->axes[i].maximum);
				}
			}

	}

	//Mouse
	int nMouses = USPiMouseAvailable();
	if (nMouses < 1 )
	{
		LogWrite ("USPi Warning", LOG_ERROR, "Mouse not found");
	}
	else
	{
		USPiMouseRegisterStatusHandler(MouseStatusHandler);
		isMouseConnected = TRUE;
	}
	
	if(isMouseConnected == FALSE && isKeyboardConnected == FALSE && isGamepadConnected == FALSE)
	{
		LogWrite ("USPi Warning", LOG_ERROR, "No USB device connected. USPiEnv Closing and Exit!");
		USPiEnvClose();

		return EXIT_HALT;
	}
	else
	{
		// chart params based on screen width and height
		basicSimulation.startPosX = (USPiEnvGetScreen()->m_nWidth)/10; 		//10% of whole Screen Width
		basicSimulation.startPosY = ((USPiEnvGetScreen()->m_nHeight)*9)/10; 	//90% of whole screen Height
	}
	

	while(1)
	{

	}
	//USPiEnvClose(); // Zamykanie powoduje crash.
	reboot();
	return EXIT_HALT;
}
