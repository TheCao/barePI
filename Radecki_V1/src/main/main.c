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
		switch(actWorkingMode){
		case(SIMULATIONMODE):
		//plot motor symulation
			if(isChartPrinted == FALSE){
				if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
					{
						LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
					}
				else isChartPrinted = TRUE;
			}

			switch(simulationMotor){
			case(FIRSTMOTOR):
				if(Simulation(USPiEnvGetScreen(),&basicMotor, basicSimulation,WHITE_COLOR,pStatus) != 0)
				{
					LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
				}
				break;
			case(SECONDMOTOR):
				if(Simulation(USPiEnvGetScreen(),&basicMotor2, basicSimulation,RED_COLOR,pStatus) != 0)
				{
					LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
				}
				break;
			case(BOTHMOTORS):
				if(Simulation(USPiEnvGetScreen(),&basicMotor, basicSimulation,WHITE_COLOR,pStatus) !=0 ||
				Simulation(USPiEnvGetScreen(),&basicMotor2, basicSimulation,RED_COLOR,pStatus) != 0)
				{
					LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
				}
				break;
			}
			//copy structures
			copyBasicMotor = basicMotor;
			copyBasicMotor2 = basicMotor2;
			actWorkingMode = FREEMODE;
			TimerMsDelay(TimerGet(),300); // delay
			break;
		case(SETDEFAULT):
		//default settings
			actualEnabledMode = NONEENABLED;
			actBasicMotor = 1;
			actMenuPosition = 1;
			simulationMotor = FIRSTMOTOR;
			TimerMsDelay(TimerGet(),300); // delay
			getDefaultValues();
			actWorkingMode = FREEMODE;
			break;
		case(CHANGEMOTOR):
		//change actual basic motor
			ScreenDeviceClearDisplay(USPiEnvGetScreen());
			if(actBasicMotor == 1) actBasicMotor = 2;
			else actBasicMotor = 1;
			LogWrite("", LOG_WARNING, "Actual Basic Motor = %u", actBasicMotor);
			isChartPrinted = FALSE;
			actWorkingMode = FREEMODE;
			TimerMsDelay(TimerGet(),300); // delay
			break;
		case(CHANGESIMULATION):
		// change simulationMotorMode for plotting purposes
			ScreenDeviceClearDisplay(USPiEnvGetScreen());
			switch(simulationMotor){
			case(FIRSTMOTOR):
				simulationMotor = SECONDMOTOR;
				LogWrite("",LOG_WARNING,"Only second motor will be plotted");
				break;
			case(SECONDMOTOR):
				simulationMotor = BOTH;
				LogWrite("",LOG_WARNING,"First and second motor will be plotted");
				break;
			case(BOTH):
				simulationMotor = FIRSTMOTOR;
				LogWrite("",LOG_WARNING,"Only first motor will be plotted");
				break;
			}
			isChartPrinted = FALSE;
			actWorkingMode = FREEMODE;
			TimerMsDelay(TimerGet(),300); // delay
			break;
		case(CHANGEMOTORPARAMS):
			actualEnabledMode = DCMOTOR;
			actMenuPosition = 1;
			ScreenDeviceClearDisplay(USPiEnvGetScreen());
			LogWrite("", LOG_WARNING, "Changing Basic Motor %u Parameters", actBasicMotor);
			isChartPrinted = FALSE;
			actWorkingMode = FREEMODE;
			TimerMsDelay(TimerGet(),300); // delay
			break;
		case(CHANGESIMULATIONPARAMS):
			actualEnabledMode = SIMULATION;
			actMenuPosition = 1;
			ScreenDeviceClearDisplay(USPiEnvGetScreen());
			LogWrite("", LOG_WARNING, "Changing Simulation Parameters");
			isChartPrinted = FALSE;
			actWorkingMode = FREEMODE;
			TimerMsDelay(TimerGet(),300); // delay
			break;
		case(DRAWCHART):
			if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
			}
			isChartPrinted = TRUE;
			actWorkingMode = FREEMODE;
			TimerMsDelay(TimerGet(),300); // delay
			break;
		case(RECOVERLAST):
			if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
			}
			switch(simulationMotor){
			case(FIRSTMOTOR):
						if(Simulation(USPiEnvGetScreen(),&copyBasicMotor, basicSimulation,WHITE_COLOR,pStatus) != 0)
						{
							LogWrite("Chart Error ", LOG_ERROR, "Recovering simulation encountered unexpected error!");
						}
						break;
					case(SECONDMOTOR):
						if(Simulation(USPiEnvGetScreen(),&copyBasicMotor2, basicSimulation,RED_COLOR,pStatus) != 0)
						{
							LogWrite("Chart Error ", LOG_ERROR, "Recovering simulation encountered unexpected error!");
						}
						break;
					case(BOTHMOTORS):
						if(Simulation(USPiEnvGetScreen(),&copyBasicMotor, basicSimulation,WHITE_COLOR,pStatus) !=0 ||
						Simulation(USPiEnvGetScreen(),&copyBasicMotor2, basicSimulation,RED_COLOR,pStatus) != 0)
						{
							LogWrite("Chart Error ", LOG_ERROR, "Recovering simulation encountered unexpected error!");
						}
						break;
				}
			isChartPrinted = TRUE;
			actWorkingMode = FREEMODE;
			TimerMsDelay(TimerGet(),300); // delay
			break;
		case(LEFTJOYSTICK):
			if( horizontalAxis != 127 || verticalAxis !=127 )
			{
				switch(actualEnabledMode){
							case(DCMOTOR):
								if(verticalAxis > 127 && horizontalAxis == 127)
								{
									// next from list
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									if(actMenuPosition >=8) actMenuPosition = 1;
									else actMenuPosition++;
									LogWrite("", LOG_WARNING, "Actual Basic Motor = %u", actBasicMotor);
									PrintActMotorParam(actMenuPosition);
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
								}
								else if(verticalAxis < 127 && horizontalAxis == 127)
								{
									// go backward
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									if(actMenuPosition <=1) actMenuPosition = 8;
									else actMenuPosition--;
									LogWrite("", LOG_WARNING, "Actual Basic Motor = %u", actBasicMotor);
									PrintActMotorParam(actMenuPosition);
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
								}
								else if(horizontalAxis > 127 && verticalAxis == 127)
								{
									// increase
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									LogWrite("", LOG_WARNING, "Actual Basic Motor = %u", actBasicMotor);
									PrintActMotorParam(actMenuPosition);
									switch(actBasicMotor){
									case(1):
										ChangeMotorParam(&basicMotor,actMenuPosition,1);
									break;
									case(2):
										ChangeMotorParam(&basicMotor2,actMenuPosition,1);
									break;
									}
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
								}
								else if (horizontalAxis < 127 && verticalAxis == 127 )
								{
									//decrease
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									LogWrite("", LOG_WARNING, "Actual Basic Motor = %u", actBasicMotor);
									PrintActMotorParam(actMenuPosition);
									switch(actBasicMotor){
									case(1):
										ChangeMotorParam(&basicMotor,actMenuPosition,-1);
									break;
									case(2):
										ChangeMotorParam(&basicMotor2,actMenuPosition,-1);
									break;
									}
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
								}
								else
								{
									// both axis are set - do nothing
								}
								break;
							case(SIMULATION):
								if(verticalAxis > 127 && horizontalAxis == 127)
									{
										// next from list
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									if(actMenuPosition >=2) actMenuPosition = 1;
									else actMenuPosition++;
									PrintActSimulationParam(actMenuPosition);
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
									}
								else if(verticalAxis < 127 && horizontalAxis == 127)
									{
										// go backward
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									if(actMenuPosition <=1) actMenuPosition = 2;
									else actMenuPosition--;
									PrintActSimulationParam(actMenuPosition);
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
									}
								else if(horizontalAxis > 127 && verticalAxis == 127)
									{
										// increase
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									PrintActSimulationParam(actMenuPosition);
									ChangeSimulationParam(&basicSimulation,actMenuPosition,1);
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
									}
								else if (horizontalAxis < 127 && verticalAxis == 127 )
									{
										//decrease
									ScreenDeviceClearDisplay(USPiEnvGetScreen());
									PrintActSimulationParam(actMenuPosition);
									ChangeSimulationParam(&basicSimulation,actMenuPosition,-1);
									isChartPrinted = FALSE;
									TimerMsDelay(TimerGet(),300); // delay
									}
								else
									{
										// both axis are set - do nothing
									}
								break;
							case(NONEENABLED):
								//pass or error?
								break;
						}
				actWorkingMode = FREEMODE;
				}
			break;


		case(FREEMODE):
				break;
		}

	}
	//USPiEnvClose(); // Zamykanie powoduje crash.
	reboot();
	return EXIT_HALT;
}
