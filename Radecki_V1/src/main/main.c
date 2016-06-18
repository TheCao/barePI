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
#include "mymath.h"

static const char FromSample[] = "Message";
boolean isMouseConnected = FALSE;
boolean isKeyboardConnected = FALSE;
boolean isGamepadConnected = FALSE;
int horizontalAxis = 0;
int verticalAxis = 0;
unsigned timerTest = 2; //TODO: Delete
unsigned timerTesting = 0;

motorParams_t basicMotor = {
		.R = 			2,
		.L =			0.1,
		.Ke =			0.1,
		.Km =			0.1,
		.J =			0.1,
		.B =			0.5,
		.Mobc =			0.0,
		.U = 			12.0
};

symulationParams_t basicSymulation ={
		.actualTime = 	0,
		.dt =			0.01,
		.tempOmega = 	0,
		.I = 			0.0,
		.didt = 		0.0,
		.d2thetadt =	0.0,
		.dthetadt = 	0.0,
		.tk = 			10.0
};

motorParams_t basicMotor2 = {
		.R = 			4,
		.L =			0.05,
		.Ke =			0.1,
		.Km =			0.1,
		.J =			0.1,
		.B =			0.3,
		.Mobc =			0.4,
		.U = 			40.0
};

static void KeyPressedHandler (const char *pString)
{
	ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
}
static void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState)
{
	//buttons handlers
	switch(pState->buttons){
	case(START):
		//LogWrite("Przycisk: ", LOG_WARNING, "Start!");
		Simulation(USPiEnvGetScreen(),&basicMotor, basicSymulation,WHITE_COLOR);
		Simulation(USPiEnvGetScreen(),&basicMotor2, basicSymulation,RED_COLOR);
		break;
	case(SELECT):
		//LogWrite("Przycisk: ", LOG_ERROR, "Select!!");
		actualEnabledMode = NONEENABLED;
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
	case(BUTTON1): //DC motor params change
		actualEnabledMode = DCMOTOR;

		break;
	case(BUTTON2): // simulation params change
		actualEnabledMode = SIMULATION;
		break;
	case(BUTTON3): // draw/clear chart
		if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
		{
			LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
		}
		break;
	case(BUTTON4): //recover last used chart


		break;

	//left joystick handling
	default:
		horizontalAxis = pState->axes[0].value;
		verticalAxis = pState->axes[1].value;
		if( horizontalAxis != 127 || verticalAxis !=127 )
		{
			//LogWrite("Joystick: ", LOG_ERROR, "%d / %d ", horizontalAxis, verticalAxis);
			//break;
			switch(actualEnabledMode){
						case(DCMOTOR):
							if(verticalAxis > 127 && horizontalAxis == 127)
							{
								// next from list
								ScreenDeviceClearDisplay(USPiEnvGetScreen());
								if(actMenuPosition >=8) actMenuPosition = 1;
								else actMenuPosition++;
								LogWrite("Motor Params Changing Mode:", LOG_ERROR, "DCMOTOR actualMenuPosition = %u",actMenuPosition);
								TimerMsDelay(TimerGet(),500); // delay
							}
							else if(verticalAxis < 127 && horizontalAxis == 127)
							{
								// go backward
								ScreenDeviceClearDisplay(USPiEnvGetScreen());
								if(actMenuPosition <=1) actMenuPosition = 8;
								else actMenuPosition--;
								LogWrite("Motor Params Changing Mode:", LOG_ERROR, "DCMOTOR actualMenuPosition = %u",actMenuPosition);
								TimerMsDelay(TimerGet(),500); // delay
							}
							else if(horizontalAxis > 127 && verticalAxis == 127)
							{
								// increase
								ScreenDeviceClearDisplay(USPiEnvGetScreen());
								LogWrite("Motor Params Changing Mode:", LOG_ERROR, "DCMOTOR actualMenuPosition = %u",actMenuPosition);
								ChangeMotorParam(&basicMotor,actMenuPosition,1);
								TimerMsDelay(TimerGet(),500); // delay
							}
							else if (horizontalAxis < 127 && verticalAxis == 127 )
							{
								//decrease
								ScreenDeviceClearDisplay(USPiEnvGetScreen());
								LogWrite("Motor Params Changing Mode:", LOG_ERROR, "DCMOTOR actualMenuPosition = %u",actMenuPosition);
								ChangeMotorParam(&basicMotor,actMenuPosition,-1);
								TimerMsDelay(TimerGet(),500); // delay
							}
							else
							{
								// both axis are set
							}
							break;
						case(SIMULATION):
							if(verticalAxis > 127 && horizontalAxis == 127)
								{
									// next from list
									LogWrite("Test:", LOG_ERROR, "SIMULATION switch next");
								}
							else if(verticalAxis < 127 && horizontalAxis == 127)
								{
									// go backward
									LogWrite("Test:", LOG_ERROR, "SIMULATION switch backward");
								}
							else if(horizontalAxis > 127 && verticalAxis == 127)
								{
									// increase
									LogWrite("Test:", LOG_ERROR, "SIMULATION switch increase");
								}
							else if (horizontalAxis < 127 && verticalAxis == 127 )
								{
									//decrease
									LogWrite("Test:", LOG_ERROR, "SIMULATION switch decrease");
								}
							else
								{
									// both axis are set
								}
							break;
						case(NONEENABLED):

							break;


					}
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
	basicSymulation.startPosX = (USPiEnvGetScreen()->m_nWidth)/10; 		//10% of whole Screen Width
	basicSymulation.startPosY = ((USPiEnvGetScreen()->m_nHeight)*9)/10; 	//90% of whole screen Height

	while(1)
	{
		ScreenDeviceRotor (USPiEnvGetScreen (), 0, 0); //TODO: zorientowaæ siê co robi ta funkcja
	}
	//USPiEnvClose(); // Zamykanie powoduje crash.


	reboot();
	return EXIT_HALT;
}
