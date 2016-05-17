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

static const char FromSample[] = "Message";
boolean isMouseConnected = FALSE;
boolean isKeyboardConnected = FALSE;
boolean isGamepadConnected = FALSE;
int horizontalAxis = 0;
int verticalAxis = 0;


static void KeyPressedHandler (const char *pString)
{
	ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
}
static void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState)
{
	// do obs³ugi przerwania od GamePad'a
	/*TString Msg;
	String (&Msg); //tworzenie wskaznika
	StringFormat (&Msg, "GamePad %u: Buttons 0x%X", nDeviceIndex + 1, pState->buttons);

	TString Value;
	String (&Value);

	if (pState->naxes > 0)
	{
		StringAppend (&Msg, " Axes");

		for (unsigned i = 0; i < pState->naxes; i++)
		{
			StringFormat (&Value, " %d", pState->axes[i].value);
			StringAppend (&Msg, StringGet (&Value));
		}
	}

	if (pState->nhats > 0)
	{
		StringAppend (&Msg, " Hats");

		for (unsigned i = 0; i < pState->nhats; i++)
		{
			StringFormat (&Value, " %d", pState->hats[i]);
			StringAppend (&Msg, StringGet (&Value));
		}
	}

	//LogWrite (FromSample, LOG_NOTICE, StringGet (&Msg));

	//"destruktory"
	_String (&Value);
	_String (&Msg);*/

	//LogWrite(FromSample, LOG_WARNING, "Button : %x - %d" ,pState->buttons,pState->buttons);
	switch(pState->buttons){
	case(START):
		LogWrite("Przycisk: ", LOG_WARNING, "Start!");
		break;
	case(SELECT):
		LogWrite("Przycisk: ", LOG_ERROR, "Select!!");
		break;
	case(START+SELECT):
		reboot();
		break;
	case(LEFT1):
		LogWrite("Przycisk: ", LOG_ERROR, "Left 1");
		break;
	case(LEFT2):
		LogWrite("Przycisk: ", LOG_ERROR, "Left 2");
		break;
	case(RIGHT1):
		LogWrite("Przycisk: ", LOG_ERROR, "Right 1");
		break;
	case(RIGHT2):
		LogWrite("Przycisk: ", LOG_ERROR, "Right 2");
		break;
	case(BUTTON1):
		//LogWrite("Przycisk: ", LOG_ERROR, "Przycisk 1");
		ScreenDeviceDrawLine(USPiEnvGetScreen(),400,400,500,GREEN_COLOR,VERTICAL);
		break;
	case(BUTTON2):
		LogWrite("Przycisk: ", LOG_ERROR, "Przycisk 2");
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		break;
	case(BUTTON3):
		LogWrite("Przycisk: ", LOG_ERROR, "Przycisk 3");
		ScreenDeviceFillDisplay(USPiEnvGetScreen(),YELLOW_COLOR);
		break;
	case(BUTTON4):
		LogWrite("Przycisk: ", LOG_ERROR, "Przycisk 4");
		TimerMsDelay(TimerGet(),1000);
		break;

	//obs³uga osi na lewym joyu
	default:
		horizontalAxis = pState->axes[0].value;
		verticalAxis = pState->axes[1].value;
		if( horizontalAxis != 127 || verticalAxis !=127 )
		{
			LogWrite("Joystick: ", LOG_ERROR, "%d / %d ", horizontalAxis, verticalAxis);
			break;
		}

	}


}

/*
static void MouseStatusHandler()
{
	//pass
}
*/
int main (void)
{
	if (!USPiEnvInitialize ())
	{
		return EXIT_HALT;
	}
	
	if (!USPiInitialize ())
	{
		LogWrite (FromSample, LOG_ERROR, "Cannot initialize USPi");

		USPiEnvClose ();

		return EXIT_HALT;
	}
	
	if (!USPiKeyboardAvailable ())
	{
		LogWrite (FromSample, LOG_ERROR, "Keyboard not found");
	}
	else
	{
		isKeyboardConnected = TRUE;
	}
	
	
	int nGamePads = USPiGamePadAvailable ();
	if (nGamePads < 1)
	{
		LogWrite (FromSample, LOG_ERROR, "GamePad not found");

	}
	else
	{
		isGamepadConnected = TRUE;

		for (unsigned nGamePad = 0; nGamePad < (unsigned) nGamePads; nGamePad++)
			{
				TUSPiDeviceInformation Info;
				if (!USPiDeviceGetInformation (GAMEPAD_CLASS, nGamePad, &Info))
				{
					LogWrite (FromSample, LOG_ERROR, "Cannot get device information");

					USPiEnvClose ();

					return EXIT_HALT;
				}

				LogWrite (FromSample, LOG_NOTICE, "GamePad %u: Vendor 0x%X Product 0x%X Version 0x%X",
					  nGamePad+1, (unsigned) Info.idVendor, (unsigned) Info.idProduct, (unsigned) Info.bcdDevice);

				LogWrite (FromSample, LOG_NOTICE, "GamePad %u: Manufacturer \"%s\" Product \"%s\"",
					  nGamePad+1, Info.pManufacturer, Info.pProduct);

				const USPiGamePadState *pState = USPiGamePadGetStatus (nGamePad);
				assert (pState != 0);

				LogWrite (FromSample, LOG_NOTICE, "GamePad %u: %d Buttons %d Hats",
					  nGamePad+1, pState->nbuttons, pState->nhats);

				for (int i = 0; i < pState->naxes; i++)
				{
					LogWrite (FromSample, LOG_NOTICE, "GamePad %u: Axis %d: Minimum %d Maximum %d",
						  nGamePad+1, i+1, pState->axes[i].minimum, pState->axes[i].maximum);
				}
			}

	}

	//Mouse
	int nMouses = USPiMouseAvailable();
	if (nMouses < 1 )
	{
		LogWrite (FromSample, LOG_ERROR, "Mouse not found");
	}
	else
	{
		isMouseConnected = TRUE;
	}
	
	if(isMouseConnected == FALSE && isKeyboardConnected == FALSE && isGamepadConnected == FALSE)
	{
		LogWrite (FromSample, LOG_ERROR, "USPiEnv Closing and Exit!");
		USPiEnvClose();

		return EXIT_HALT;
	}
	USPiGamePadRegisterStatusHandler (GamePadStatusHandler);
	
	USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);
	
	//USPiMouseRegisterStatusHandler(MouseStatusHandler);


	while(1)
	{
		ScreenDeviceRotor (USPiEnvGetScreen (), 0, 0); //TODO: zorientowaæ siê co robi ta funkcja
	}
	//USPiEnvClose(); // Zamykanie powoduje crash.

	reboot();
	return EXIT_HALT;
}
