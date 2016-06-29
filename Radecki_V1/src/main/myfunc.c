#include "myfunc.h"

/*
 * myfunc.c
 *
 *  Created on: 24 maj 2016
 *      Author: Komar
 */
#include <math.h>
#include "myfunc.h"
#include "uspienv/stdarg.h"
#include "uspienv/string.h"
#include <uspienv.h>

#include <uspios.h>
#include <uspienv/string.h>
#include <uspienv/assert.h>
#include <uspienv/util.h>

#include <uspienv/types.h>
#include "gamepad.h"

boolean isMouseConnected = FALSE;
boolean isKeyboardConnected = FALSE;
boolean isGamepadConnected = FALSE;
int horizontalAxis = 0;
int verticalAxis = 0;
boolean isChartPrinted = FALSE;

extern void LogWrite (const char *pSource, unsigned Severity, const char *pMessage, ...);
unsigned actMenuPosition = 1;
unsigned actBasicMotor = 1;

motorParams_t basicMotor = {
		.number = 		1,
		.R = 			2,
		.L =			0.1,
		.Ke =			0.1,
		.Km =			0.1,
		.J =			0.1,
		.B =			0.5,
		.Mobc =			0.0,
		.U = 			12.0
};
motorParams_t basicMotor2 = {
		.number = 		2,
		.R = 			4,
		.L =			0.05,
		.Ke =			0.1,
		.Km =			0.1,
		.J =			0.1,
		.B =			0.3,
		.Mobc =			0.4,
		.U = 			24.0
};


simulationParams_t basicSimulation ={
		.actualTimeUI = 0,
		.actualTimeD =	0.0,
		.dt =			0.01, // value in seconds!
		.tempOmega = 	0,
		.I = 			0.0,
		.didt = 		0.0,
		.d2thetadt =	0.0,
		.dthetadt = 	0.0,
		.tk = 			100.0
};


motorParams_t copyBasicMotor, copyBasicMotor2;

void getDefaultValues()
{
	basicMotor.R = 2;
	basicMotor.L = 0.1;
	basicMotor.Ke = 0.1;
	basicMotor.Km = 0.1;
	basicMotor.J = 0.1;
	basicMotor.B = 0.5;
	basicMotor.Mobc = 0.0;
	basicMotor.U = 12.0;
	basicMotor2.R = 4;
	basicMotor2.L = 0.05;
	basicMotor2.Ke = 0.1;
	basicMotor2.Km = 0.1;
	basicMotor2.J = 0.1;
	basicMotor2.B = 0.3;
	basicMotor2.Mobc = 0.4;
	basicMotor2.U = 24.0;
	basicSimulation.dt =0.01;
	basicSimulation.tk = 10.0;
}

unsigned Simulation(TScreenDevice *pThis,motorParams_t *motorParams, simulationParams_t symParams,TScreenColor color,const USPiGamePadState *pState)
{
	if(motorParams->number == 1) uart_sendC("Simulation 1 has been started");
	else uart_sendC("Simulation 2 has been started");
	for(symParams.actualTimeD=0.0;symParams.actualTimeD<symParams.tk;symParams.actualTimeD+=symParams.dt)
		{
			symParams.didt = (1/motorParams->L)*(motorParams->U-motorParams->R*symParams.I-motorParams->Ke*symParams.dthetadt);
			symParams.d2thetadt = (1/motorParams->J)*(motorParams->Km*symParams.I-motorParams->B*symParams.dthetadt
					-motorParams->Mobc);
			// integrals
			symParams.dthetadt+= symParams.d2thetadt*symParams.dt;
			symParams.I+=symParams.didt*symParams.dt;

			symParams.actualTimeUI = symParams.actualTimeD*10; // for printing purposes
			symParams.tempOmega = symParams.dthetadt*400;
			// plotting function, width 5 px
			for(signed i = -2;i<=2;i++)
			{
				ScreenDeviceSetPixel(pThis, symParams.startPosX + symParams.actualTimeUI, symParams.startPosY
						- symParams.tempOmega+i, color);
			}
			// sending data to PC by UART
			UartSendString("%f \t %f \t %f",symParams.actualTimeD, symParams.I, symParams.dthetadt);
			if(symParams.actualTimeD>=5) motorParams->Mobc = 0.3;
			if(symParams.actualTimeD>=8) motorParams->U = 24;

			//delay
			TimerMsDelay(TimerGet(),(unsigned int)symParams.dt*1000);
		}
	return 0;
}

unsigned ChangeMotorParam(motorParams_t *structure,unsigned menuPosition,signed value)
{
	switch(menuPosition){
		case(1):
			structure->R+= value*0.1;
			if(structure->R <=0){
				structure->R = 0.0;
			}
			LogWrite("Motor Params Setting Mode", 1, "R = %f",(structure->R));
			break;
		case(2):
			structure->L+= value*0.1;
			if(structure->L <=0){
					structure->L = 0.0;
				}
				LogWrite("Motor Params Setting Mode", 1, "L = %f",(structure->L));
			break;
		case(3):
			structure->Ke+=value*0.1;
			if(structure->Ke <=0){
					structure->Ke = 0.0;
				}
				LogWrite("Motor Params Setting Mode", 1, "Ke = %f",(structure->Ke));
			break;
		case(4):
			structure->Km+=value*0.1;
			if(structure->Km <=0){
				structure->Km = 0.0;
			}
			LogWrite("Motor Params Setting Mode", 1, "Km = %f",(structure->Km));
			break;
		case(5):
			structure->J+=value*0.1;
			if(structure->J <=0){
				structure->J = 0.0;
			}
			LogWrite("Motor Params Setting Mode", 1, "J = %f",(structure->J));
			break;
		case(6):
			structure->B+=value*0.1;
			if(structure->B <=0){
				structure->B = 0.0;
			}
			LogWrite("Motor Params Setting Mode", 1, "B = %f",(structure->B));
			break;
		case(7):
			structure->Mobc+=value*0.1;
			if(structure->Mobc <=0){
				structure->Mobc = 0.0;
			}
			LogWrite("Motor Params Setting Mode", 1, "Mobc = %f",(structure->Mobc));
			break;
		case(8):
			structure->U+=value*0.1;
			if(structure->U <=0){
				structure->U = 0.0;
			}
			LogWrite("Motor Params Setting Mode", 1, "U = %f",(structure->U));
			break;
		default:
			LogWrite("Motor Params Setting Mode ERROR",0,"Wrong Parameter Number");

	}
	return 0;
}

unsigned ChangeSimulationParam(simulationParams_t *structure,unsigned menuPosition,signed value)
{
	switch(menuPosition){
	case(1):
		structure->dt+= value*0.1;
		if(structure->dt <=0){
			structure->dt = 0.0;
		}
		LogWrite("Simulation Params Setting Mode", 1, "dt = %f",(structure->dt));
		break;
	case(2):
		structure->tk+=value*1;
		if(structure->tk <=0){
			structure->tk = 0.0;
		}
		LogWrite("Simulation Params Setting Mode", 1, "tk = %f",(structure->tk));
		break;
	default:
		LogWrite("Simulation Params Setting Mode ERROR",0,"Wrong Parameter Number");
	}


	return 0;
}

unsigned PrintActMotorParam(unsigned val)
{
	switch(val){
	case(1):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: R");
		break;
	case(2):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: L");
		break;
	case(3):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: Ke");
		break;
	case(4):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: Km");
		break;
	case(5):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: J");
		break;
	case(6):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: B");
		break;
	case(7):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: Mobc");
		break;
	case(8):
		LogWrite("Motor Params Setting Mode", 1, "Motor Parameter: U");
		break;
	}
	return 0;
}

unsigned PrintActSimulationParam(unsigned val)
{
	switch(val){
	case(1):
		LogWrite("Simulation Params Setting Mode", 1, "Simulation Parameter: dt");
		break;
	case(2):
		LogWrite("Simulation Params Setting Mode", 1, "Simulation Parameter: tk");
		break;
	}
	return 0;
}
unsigned UartSendString(const char *pMessage,...)
{
	va_list args;
	va_start (args, pMessage);
	TString string;
	String (&string);
	StringFormatV(&string, pMessage,args);
	va_end(args);
	uart_sendC(string.m_pBuffer);
	return 0;
}

void KeyPressedHandler (const char *pString)
{
	ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
}

void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState)
{
	//buttons handlers
	switch(pState->buttons){
	case(START):
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
			if(Simulation(USPiEnvGetScreen(),&basicMotor, basicSimulation,WHITE_COLOR,pState) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
			}
			break;
		case(SECONDMOTOR):
			if(Simulation(USPiEnvGetScreen(),&basicMotor2, basicSimulation,RED_COLOR,pState) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
			}
			break;
		case(BOTHMOTORS):
			if(Simulation(USPiEnvGetScreen(),&basicMotor, basicSimulation,WHITE_COLOR,pState) !=0 ||
			Simulation(USPiEnvGetScreen(),&basicMotor2, basicSimulation,RED_COLOR,pState) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
			}
			break;
		}
		//copy structures
		copyBasicMotor = basicMotor;
		copyBasicMotor2 = basicMotor2;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(SELECT):
		//default settings
		actualEnabledMode = NONEENABLED;
		actBasicMotor = 1;
		actMenuPosition = 1;
		simulationMotor = FIRSTMOTOR;
		TimerMsDelay(TimerGet(),300); // delay
		getDefaultValues();
		break;
	case(START+SELECT):
		reboot();
		break;
	case(LEFT1):
		break;
	case(LEFT2):
		break;
	case(RIGHT1):
		//change actual basic motor
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		if(actBasicMotor == 1) actBasicMotor = 2;
		else actBasicMotor = 1;
		LogWrite("", LOG_WARNING, "Actual Basic Motor = %u", actBasicMotor);
		isChartPrinted = FALSE;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(RIGHT2):
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
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(BUTTON1): //DC motor params change
		actualEnabledMode = DCMOTOR;
		actMenuPosition = 1;
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		LogWrite("", LOG_WARNING, "Changing Basic Motor %u Parameters", actBasicMotor);
		isChartPrinted = FALSE;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(BUTTON2): // simulation params change
		actualEnabledMode = SIMULATION;
		actMenuPosition = 1;
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		LogWrite("", LOG_WARNING, "Changing Simulation Parameters");
		isChartPrinted = FALSE;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(BUTTON3): // draw/clear chart
		if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
		{
			LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
		}
		isChartPrinted = TRUE;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(BUTTON4): //recover last simulation params
		if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
				{
					LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
				}
		switch(simulationMotor){
		case(FIRSTMOTOR):
					if(Simulation(USPiEnvGetScreen(),&copyBasicMotor, basicSimulation,WHITE_COLOR,pState) != 0)
					{
						LogWrite("Chart Error ", LOG_ERROR, "Recovering simulation encountered unexpected error!");
					}
					break;
				case(SECONDMOTOR):
					if(Simulation(USPiEnvGetScreen(),&copyBasicMotor2, basicSimulation,RED_COLOR,pState) != 0)
					{
						LogWrite("Chart Error ", LOG_ERROR, "Recovering simulation encountered unexpected error!");
					}
					break;
				case(BOTHMOTORS):
					if(Simulation(USPiEnvGetScreen(),&copyBasicMotor, basicSimulation,WHITE_COLOR,pState) !=0 ||
					Simulation(USPiEnvGetScreen(),&copyBasicMotor2, basicSimulation,RED_COLOR,pState) != 0)
					{
						LogWrite("Chart Error ", LOG_ERROR, "Recovering simulation encountered unexpected error!");
					}
					break;
			}
		isChartPrinted = TRUE;
		TimerMsDelay(TimerGet(),300); // delay
		break;

	//left joystick handling
	default:
		horizontalAxis = pState->axes[0].value;
		verticalAxis = pState->axes[1].value;
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
		}

	}
}


void MouseStatusHandler()
{
	//TODO:
}

