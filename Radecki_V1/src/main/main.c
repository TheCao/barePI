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

extern boolean isMouseConnected;
extern boolean isKeyboardConnected;
extern boolean isGamepadConnected;
extern boolean isChartPrinted;
extern boolean readyFlag;
extern int horizontalAxis;
extern int verticalAxis;
extern motorParams_t basicMotor, basicMotor2;
extern motorParams_t copyBasicMotor, copyBasicMotor2;
extern simulationParams_t basicSimulation;
USPiGamePadState *pState;
extern unsigned int * fifoBuffer;
extern unsigned int * fifoBuffer2;
extern unsigned int * buforRysunkowy;
extern void KeyPressedHandler (const char *pString);
extern void MouseStatusHandler();
extern void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState);
//int controlRegister;
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
//		LogWrite ("USPi Error", LOG_ERROR, "Keyboard not found");
	}
	else
	{
		isKeyboardConnected = TRUE;
		USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);
	}
	
	int nGamePads = USPiGamePadAvailable ();
	if (nGamePads < 1)
	{
//		LogWrite ("USPi Error", LOG_ERROR, "GamePad not found");

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

				/*LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: Vendor 0x%X Product 0x%X Version 0x%X",
					  nGamePad+1, (unsigned) Info.idVendor, (unsigned) Info.idProduct, (unsigned) Info.bcdDevice);

				LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: Manufacturer \"%s\" Product \"%s\"",
					  nGamePad+1, Info.pManufacturer, Info.pProduct);*/

				 pState = USPiGamePadGetStatus (nGamePad);
				assert (pState != 0);

				/*LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: %d Buttons %d Hats",
					  nGamePad+1, pState->nbuttons, pState->nhats);

				for (int i = 0; i < pState->naxes; i++)
				{
					LogWrite ("USPi Info", LOG_NOTICE, "GamePad %u: Axis %d: Minimum %d Maximum %d",
						  nGamePad+1, i+1, pState->axes[i].minimum, pState->axes[i].maximum);
				}*/
			}

	}

	//Mouse
	int nMouses = USPiMouseAvailable();
	if (nMouses < 1 )
	{
//		LogWrite ("USPi Warning", LOG_ERROR, "Mouse not found");
	}
	else
	{
		USPiMouseRegisterStatusHandler(MouseStatusHandler);
		isMouseConnected = TRUE;
	}
	
//	if(isMouseConnected == FALSE && isKeyboardConnected == FALSE && isGamepadConnected == FALSE)
//	{
//		LogWrite ("USPi Warning", LOG_ERROR, "No USB device connected. USPiEnv Closing and Exit!");
//		USPiEnvClose();
//
//		return EXIT_HALT;
//	}
//	else
//	{
		InterruptSystemDisableIRQ(ARM_IRQ_USB);
		// chart params based on screen width and height
		basicSimulation.startPosX = (USPiEnvGetScreen()->m_nWidth)/10; 		//10% of whole Screen Width
		basicSimulation.startPosY = ((USPiEnvGetScreen()->m_nHeight)*9)/10; 	//90% of whole screen Height
		basicSimulation.lenX = ((unsigned int)((USPiEnvGetScreen()->m_nWidth - 2*basicSimulation.startPosX)/10))*10;
		basicSimulation.lenY = ((basicSimulation.startPosY-(USPiEnvGetScreen()->m_nHeight/10))/10)*10;
		basicSimulation.bufferMax = basicSimulation.lenX*(1/(basicSimulation.dt));
		readyFlag = TRUE;

//	}
	

	// memory for fifo buffer
	fifoBuffer = (unsigned int*) malloc (basicSimulation.bufferMax);
	fifoBuffer2 = (unsigned int*) malloc (basicSimulation.bufferMax);
	buforRysunkowy = (unsigned int*) malloc (basicSimulation.lenX);
	startFlag = TRUE;
	ScreenDeviceClearDisplay(USPiEnvGetScreen());
	if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
	{
		LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
	}
	else isChartPrinted = TRUE;
	ScreenDeviceDrawChartCaptionOXAll(USPiEnvGetScreen(),basicSimulation.startPosX, basicSimulation.startPosY,basicSimulation.lenX,basicSimulation.lenY,basicSimulation.isFirstDraw,basicSimulation.dt,basicSimulation.resolution,basicSimulation.actualTimeD);
	ScreenDeviceDrawChartCaptionOYAll(USPiEnvGetScreen(),basicSimulation.startPosX, basicSimulation.startPosY,basicSimulation.lenY,basicSimulation.isFirstDraw,basicSimulation.resolution,2.0);
	InterruptSystemEnableIRQ(ARM_IRQ_USB);
	while(1)
	{
		Symulator(&pState);
	}
	//USPiEnvClose(); // Zamykanie powoduje crash.
	reboot();
	return EXIT_HALT;

}
