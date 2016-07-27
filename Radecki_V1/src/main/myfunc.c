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
boolean startFlag = FALSE;
boolean readyFlag = FALSE;
boolean clearFlag = FALSE;
boolean buttonFlag = FALSE; // flaga do blokowania przycisku po wciœniêciu
extern void LogWrite (const char *pSource, unsigned Severity, const char *pMessage, ...);
extern void ScreenDeviceCursorHome (TScreenDevice *pThis);
unsigned actMenuPosition = 1;
unsigned actBasicMotor = 1;
unsigned tempPosX = 0;

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
		.actualTimeTemp = 0.0,
		.dt =			0.01, // value in seconds! DO NOT CHANGE!
		.tempOmega = 	0,
		.I = 			0.0,
		.didt = 		0.0,
		.d2thetadt =	0.0,
		.dthetadt = 	0.0,
		.tk = 			200.0,
		.bufferIndex = 	0,
		.actualPosX = 0,
		.resolution = 2, // co który pomiar bêdzie wyœwietlany na wykresie
		.isFirstDraw = TRUE
};

simulationParams_t basicSimulation2 = {
		.actualTimeUI = 0,
		.actualTimeD =	0.0,
		.dt =			0.01, // value in seconds! DO NOT CHANGE!
		.tempOmega = 	0,
		.I = 			0.0,
		.didt = 		0.0,
		.d2thetadt =	0.0,
		.dthetadt = 	0.0,
		.tk = 			100.0
};


motorParams_t copyBasicMotor, copyBasicMotor2;

void setDefaultValues()
{
	startFlag = FALSE;
	clearFlag = FALSE;
	tempPosX = 0;
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
	basicSimulation.tk = 200.0;
	basicSimulation.I = 0.0;
	basicSimulation.actualTimeD = 0.0;
	basicSimulation.actualTimeUI = 0;
	basicSimulation.actualTimeTemp = 0.0;
	basicSimulation.d2thetadt = 0.0;
	basicSimulation.didt = 0;
	basicSimulation.dthetadt = 0.0;
	basicSimulation.stepEndTime = 0.0;
	basicSimulation.tempOmega = 0.0;
	basicSimulation.I = 0.0;
	basicSimulation.bufferIndex = 0;
	basicSimulation.actualPosX = 0;
	basicSimulation.isFirstDraw = TRUE;
	//second simulation
	basicSimulation2.actualTimeD = 0.0;
	basicSimulation2.actualTimeUI = 0;
	basicSimulation2.d2thetadt = 0.0;
	basicSimulation2.didt = 0;
	basicSimulation2.dthetadt = 0.0;
	basicSimulation2.stepEndTime = 0.0;
	basicSimulation2.tempOmega = 0.0;

}

void finishSimulation()
{
	startFlag = FALSE;
	clearFlag = FALSE;
	tempPosX = 0;
	basicSimulation.tk = basicSimulation.tkCopy;
	basicSimulation.I = 0.0;
	basicSimulation.actualTimeD = 0.0;
	basicSimulation.actualTimeUI = 0;
	basicSimulation.actualTimeTemp = 0.0;
	basicSimulation.d2thetadt = 0.0;
	basicSimulation.didt = 0;
	basicSimulation.dthetadt = 0.0;
	basicSimulation.stepEndTime = 0.0;
	basicSimulation.tempOmega = 0.0;
	basicSimulation.bufferIndex = 0;
	basicSimulation2.actualTimeD = 0.0;
	basicSimulation2.actualTimeUI = 0;
	basicSimulation2.d2thetadt = 0.0;
	basicSimulation2.didt = 0;
	basicSimulation2.dthetadt = 0.0;
	basicSimulation2.stepEndTime = 0.0;
	basicSimulation2.tempOmega = 0.0;
	basicSimulation.actualPosX = 0;
	basicSimulation.isFirstDraw = TRUE;

}
unsigned getNumberLenght(double x) {
    if(x>=1000000000) return 10;
    if(x>=100000000) return 9;
    if(x>=10000000) return 8;
    if(x>=1000000) return 7;
    if(x>=100000) return 6;
    if(x>=10000) return 5;
    if(x>=1000) return 4;
    if(x>=100) return 3;
    if(x>=10) return 2;
    return 1;
}
int power(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}
unsigned Simulation(TScreenDevice *pThis,motorParams_t *motorParams, simulationParams_t *symParams,TScreenColor color,const USPiGamePadState *pState)
{
	if(symParams->actualTimeD < 0.01)
	{
		if(motorParams->number == 1) uart_sendC("Simulation for 1 motor has been started");
		else uart_sendC("Simulation for 2 motor has been started");
	}
	symParams->stepEndTime = symParams->actualTimeD+5*symParams->dt; // set end time for for loop at 5 steps
	for(;symParams->actualTimeD <= symParams->tk && symParams->actualTimeD <= symParams->stepEndTime;symParams->actualTimeD+=symParams->dt)
		{
			symParams->didt = (1/motorParams->L)*(motorParams->U-motorParams->R*symParams->I-motorParams->Ke*symParams->dthetadt);
			symParams->d2thetadt = (1/motorParams->J)*(motorParams->Km*symParams->I-motorParams->B*symParams->dthetadt
					-motorParams->Mobc);
			// integrals
			symParams->dthetadt+= symParams->d2thetadt*symParams->dt;
			symParams->I+=symParams->didt*symParams->dt;

			symParams->tempOmega = symParams->dthetadt*400; //TODO: skalowanie tip: zamiast w pêtli for korzystaæ z obliczonego czasu to mo¿e zajêtoœæ pola wykresu a czas dostosowaæ na koniec i podpisaæ wykres ?
			// fifo buffer - przypadek pracy ci¹g³ej
			if(symParams->bufferIndex < symParams->bufferMax) // indeks bufora miesci siê w zakresie osi X
			{
				fifoBuffer[symParams->bufferIndex] = symParams->tempOmega;
				// indeks bufora miesci siê w zakresie
				if(symParams->actualPosX <= 1020) // czy ekran wymaga przesuniecia
				{
					if(symParams->actualPosX != tempPosX)
					{// plotting function, width 5 px
						for(signed i = -2;i<=2;i++)
						{
							ScreenDeviceSetPixel(pThis, symParams->startPosX + symParams->actualPosX, symParams->startPosY
									- fifoBuffer[symParams->bufferIndex]+i, color);
						}
					}
					tempPosX = symParams->actualPosX;
					if(!((symParams->bufferIndex)%symParams->resolution)) symParams->actualPosX++;  //inkrementacja pozycji co n-ty pomiar okreslony przez resolution
				}
				else
				{
					InterruptSystemDisableIRQ(ARM_IRQ_USB);
					// usuniêcie wydruków na ekranie - 1020p; actualPosX = 1020; bufferIndex ciagle sie zwieksza
					for(unsigned u = 0;u <=1020;u++)
					{
						for(signed i = -2;i<=2;i++)
						{
							ScreenDeviceSetPixel(pThis, symParams->startPosX + u+1, symParams->startPosY
									- fifoBuffer[symParams->bufferIndex-((symParams->resolution)*1020)+(symParams->resolution*u)]+i, BLACK_COLOR); //poniewaz rysowalem z krokiem resolution to zmazywanie tez
						}
					}
					// wyrosowanie t³a i kropkowanych linii
					ScreenDeviceDrawChart(pThis,GREEN_COLOR,BOTH);
					// rysowanie na 1020 px wartosci przesunietego bufora tj 1-1021
					for(unsigned u = 0;u <=1020;u++)
					{
						for(signed i = -2;i<=2;i++)
						{
							ScreenDeviceSetPixel(pThis, symParams->startPosX + u, symParams->startPosY
									- fifoBuffer[symParams->bufferIndex-((symParams->resolution)*1020)+1+(symParams->resolution*u)]+i, color);
						}
					}
					ScreenDeviceDrawChartCaptionAll(pThis,symParams->startPosX, symParams->startPosY,symParams->lenX,symParams->lenY,symParams->isFirstDraw,symParams->dt,symParams->resolution,symParams->actualTimeD);
					if(symParams->isFirstDraw == TRUE) symParams->isFirstDraw = FALSE;
					InterruptSystemEnableIRQ(ARM_IRQ_USB);
				}
				symParams->bufferIndex++;
				// sending data to PC by UART
				UartSendString("%f \t %f \t %f",symParams->actualTimeD, symParams->I, symParams->dthetadt);
				//if(symParams->actualTimeD>=5) motorParams->Mobc = 0.3;
				//if(symParams->actualTimeD>=8) motorParams->U = 24;
				//delay
				//TimerMsDelay(TimerGet(),(unsigned int)10);
			}
			else // przekroczenie bufora
			{
				UartSendString("Buff error: MaxBuff = %u \t Act Buff = %u \t actTime = %f ", symParams->bufferMax, symParams->bufferIndex, symParams->actualTimeD);
			}
		}
	// set startFlag to false and simulation values when simulation ends
	if(symParams->actualTimeD >= symParams->tk)
	{
		finishSimulation();
	}
	return 0;
}

unsigned SimulationBoth(TScreenDevice *pThis,motorParams_t *motorParams,motorParams_t *motorParams2, simulationParams_t *symParams, simulationParams_t *symParams2,TScreenColor color,TScreenColor color2,const USPiGamePadState *pState)
{
	if(symParams->actualTimeD < 0.01)
	{
		 uart_sendC("Simulation for both motors has been started");
	}
	symParams->stepEndTime = symParams->actualTimeD+5*symParams->dt; // set end time for for loop at 5 steps
	for(;symParams->actualTimeD <= symParams->tk && symParams->actualTimeD <= symParams->stepEndTime;symParams->actualTimeD+=symParams->dt)
		{
			symParams->didt = (1/motorParams->L)*(motorParams->U-motorParams->R*symParams->I-motorParams->Ke*symParams->dthetadt);
			symParams->d2thetadt = (1/motorParams->J)*(motorParams->Km*symParams->I-motorParams->B*symParams->dthetadt
					-motorParams->Mobc);

			symParams2->didt = (1/motorParams2->L)*(motorParams2->U-motorParams2->R*symParams2->I-motorParams->Ke*symParams2->dthetadt);
			symParams2->d2thetadt = (1/motorParams2->J)*(motorParams2->Km*symParams2->I-motorParams2->B*symParams2->dthetadt
					-motorParams2->Mobc);
			// integrals
			symParams->dthetadt+= symParams->d2thetadt*symParams->dt;
			symParams->I+=symParams->didt*symParams->dt;

			symParams2->dthetadt+= symParams2->d2thetadt*symParams->dt;
			symParams2->I+=symParams2->didt*symParams->dt;

			//symParams->actualTimeUI = symParams->actualTimeD*10; // for printing purposes
			symParams->tempOmega = symParams->dthetadt*400;
			symParams2->tempOmega = symParams2->dthetadt*400;
			// fifo buffer - przypadek pracy ci¹g³ej
			if(symParams->bufferIndex < symParams->bufferMax)
			{
				fifoBuffer[symParams->bufferIndex] = symParams->tempOmega;
				fifoBuffer2[symParams->bufferIndex] = symParams2->tempOmega;
				// indeks bufora miesci siê w zakresie
				if(symParams->actualPosX <= 1020) // czy ekran wymaga przesuniecia
				{
					if(symParams->actualPosX != tempPosX)
					{// plotting function, width 5 px
						for(signed i = -2;i<=2;i++)
						{
							ScreenDeviceSetPixel(pThis, symParams->startPosX + symParams->actualPosX, symParams->startPosY
									- fifoBuffer[symParams->bufferIndex]+i, color);
							ScreenDeviceSetPixel(pThis, symParams->startPosX + symParams->actualPosX, symParams->startPosY
									- fifoBuffer2[symParams->bufferIndex]+i, color2);
						}
					}
					tempPosX = symParams->actualPosX;
					if(!((symParams->bufferIndex)%symParams->resolution)) symParams->actualPosX++;  //inkrementacja pozycji co n-ty pomiar okreslony przez resolution
				}
				else
				{
					// usuniêcie wydruków na ekranie - 1020p; actualPosX = 1020; bufferIndex ciagle sie zwieksza
					for(unsigned u = 0;u <=1020;u++)
					{
						for(signed i = -2;i<=2;i++)
						{
							ScreenDeviceSetPixel(pThis, symParams->startPosX + u+1, symParams->startPosY
									- fifoBuffer[symParams->bufferIndex-((symParams->resolution)*1020)+(symParams->resolution*u)]+i, BLACK_COLOR); //poniewaz rysowalem z krokiem resolution to zmazywanie tez
							ScreenDeviceSetPixel(pThis, symParams->startPosX + u+1, symParams->startPosY
									- fifoBuffer2[symParams->bufferIndex-((symParams->resolution)*1020)+(symParams->resolution*u)]+i, BLACK_COLOR);
						}
					}
					// wyrosowanie t³a i kropkowanych linii
					ScreenDeviceDrawChart(pThis,GREEN_COLOR,BOTH);
					// rysowanie na 1020 px wartosci przesunietego bufora tj 1-1021
					for(unsigned u = 0;u <=1020;u++)
					{
						for(signed i = -2;i<=2;i++)
						{
							ScreenDeviceSetPixel(pThis, symParams->startPosX + u, symParams->startPosY
									- fifoBuffer[symParams->bufferIndex-((symParams->resolution)*1020)+1+(symParams->resolution*u)]+i, color);
							ScreenDeviceSetPixel(pThis, symParams->startPosX + u, symParams->startPosY
									- fifoBuffer2[symParams->bufferIndex-((symParams->resolution)*1020)+1+(symParams->resolution*u)]+i, color2);

						}
					}
					ScreenDeviceDrawChartCaptionAll(pThis,symParams->startPosX, symParams->startPosY,symParams->lenX,symParams->lenY,symParams->isFirstDraw,symParams->dt,symParams->resolution,symParams->actualTimeD);
					if(symParams->isFirstDraw == TRUE) symParams->isFirstDraw = FALSE;
				}
				symParams->bufferIndex++;
				// sending data to PC by UART
				UartSendString("%f \t %f \t %f",symParams->actualTimeD, symParams->I, symParams->dthetadt);
				//if(symParams->actualTimeD>=5) motorParams->Mobc = 0.3;
				//if(symParams->actualTimeD>=8) motorParams->U = 24;
				//delay
				//TimerMsDelay(TimerGet(),(unsigned int)10);
			}
			else // przekroczenie bufora
			{
				UartSendString("Buff error: MaxBuff = %u \t Act Buff = %u \t actTime = %f ", symParams->bufferMax, symParams->bufferIndex, symParams->actualTimeD);
			}
			// sending data to PC by UART
			UartSendString("%f \t %f \t %f \t %f \t %f",symParams->actualTimeD, symParams->I, symParams->dthetadt, symParams2->I, symParams2->dthetadt);

			//delay
			TimerMsDelay(TimerGet(),(unsigned int)symParams->dt*1000);
		}
	// set startFlag to false and simulation values when simulation ends
	if(symParams->actualTimeD >= symParams->tk)
	{
		finishSimulation();
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
		structure->resolution+= value;
		if(structure->resolution <=1) structure->resolution = 1;
		else if(structure->resolution >=10) structure->resolution = 10;
		LogWrite("Simulation Params Setting Mode", 1, "resolution = %u",(structure->resolution));
		finishSimulation();
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
		break;
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
		LogWrite("Simulation Params Setting Mode", 1, "Simulation Parameter: resolution");
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
	if(readyFlag != TRUE) return;
	switch(pState->buttons){
	case(START):
		startFlag = TRUE;
		clearFlag = FALSE;
		//copy prev. simulation params
		basicSimulation.dtCopy = basicSimulation.dt;
		basicSimulation.tkCopy = basicSimulation.tk;
		//set EnabledMode
		actualEnabledMode = NONEENABLED; //left joystick used for change U or Mobc
		if(isChartPrinted == FALSE){
			ScreenDeviceClearDisplay(USPiEnvGetScreen());
			if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
				{
					LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
				}
			else isChartPrinted = TRUE;
			ScreenDeviceDrawChartCaptionAll(USPiEnvGetScreen(),basicSimulation.startPosX, basicSimulation.startPosY,basicSimulation.lenX,basicSimulation.lenY,basicSimulation.isFirstDraw,basicSimulation.dt,basicSimulation.resolution,basicSimulation.actualTimeD);
		}
//			TimerMsDelay(TimerGet(),300); // delay
		break;

	case(SELECT):
	//clear screen and set default settings
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		isChartPrinted = FALSE;
		actualEnabledMode = NONEENABLED;
		actBasicMotor = 1;
		actMenuPosition = 1;
		simulationMotor = FIRSTMOTOR;
		setDefaultValues();
		LogWrite("", LOG_NOTICE, "Default values has been set!");
		TimerMsDelay(TimerGet(),300); // delay

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
		startFlag = FALSE; //stop simulation
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		if(actBasicMotor == 1) actBasicMotor = 2;
		else actBasicMotor = 1;
		LogWrite("", LOG_WARNING, "Actual Basic Motor = %u", actBasicMotor);
		isChartPrinted = FALSE;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(RIGHT2):
		// change simulationMotorMode for plotting purposes
		startFlag = FALSE; //stop simulation
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
		startFlag = FALSE; //stop simulation
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		LogWrite("", LOG_WARNING, "Changing Basic Motor %u Parameters", actBasicMotor);
		isChartPrinted = FALSE;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(BUTTON2): // simulation params change
		actualEnabledMode = SIMULATION;
		actMenuPosition = 1;
		startFlag = FALSE; //stop simulation
		ScreenDeviceClearDisplay(USPiEnvGetScreen());
		LogWrite("", LOG_WARNING, "Changing Simulation Parameters");
		isChartPrinted = FALSE;
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(BUTTON3): // draw/clear chart
		//check if simulation was stopped earlier
		if(clearFlag == FALSE)
		{
			startFlag = FALSE; //stop simulation
			clearFlag = TRUE;
		}
		else
		{
			setDefaultValues();
			ScreenDeviceClearDisplay(USPiEnvGetScreen());
			if(ScreenDeviceDrawChart(USPiEnvGetScreen(),GREEN_COLOR, BOTH) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Chart was not printed! :(");
			}
			isChartPrinted = TRUE;
			ScreenDeviceDrawChartCaptionAll(USPiEnvGetScreen(),basicSimulation.startPosX, basicSimulation.startPosY,basicSimulation.lenX,basicSimulation.lenY,basicSimulation.isFirstDraw,basicSimulation.dt,basicSimulation.resolution,basicSimulation.actualTimeD);
		}
		TimerMsDelay(TimerGet(),300); // delay
		break;
	case(BUTTON4):
		break;
	default:
		//left joystick handling
		horizontalAxis = pState->axes[0].value;
		verticalAxis = pState->axes[1].value;
		if( (horizontalAxis != 127 || verticalAxis !=127) && buttonFlag == FALSE ) // left joystick and buttonFlag = False
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
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
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
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
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
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
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
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
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
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
							}
						else if(verticalAxis < 127 && horizontalAxis == 127)
							{
								// go backward
							ScreenDeviceClearDisplay(USPiEnvGetScreen());
							if(actMenuPosition <=1) actMenuPosition = 2;
							else actMenuPosition--;
							PrintActSimulationParam(actMenuPosition);
							isChartPrinted = FALSE;
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
							}
						else if(horizontalAxis > 127 && verticalAxis == 127)
							{
								// increase
							ScreenDeviceClearDisplay(USPiEnvGetScreen());
							PrintActSimulationParam(actMenuPosition);
							ChangeSimulationParam(&basicSimulation,actMenuPosition,1);
							isChartPrinted = FALSE;
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
							}
						else if (horizontalAxis < 127 && verticalAxis == 127 )
							{
								//decrease
							ScreenDeviceClearDisplay(USPiEnvGetScreen());
							PrintActSimulationParam(actMenuPosition);
							ChangeSimulationParam(&basicSimulation,actMenuPosition,-1);
							isChartPrinted = FALSE;
							//TimerMsDelay(TimerGet(),300); // delay
							buttonFlag = TRUE;
							}
						else
							{
								// both axis are set - do nothing
							}
						break;
					case(NONEENABLED): //change U or Mobc durinig simulation for Motor1 ONLY
						if(verticalAxis > 127 && horizontalAxis == 127)
						{
							basicMotor.Mobc-=0.1;
							if(basicMotor.Mobc <=0.0) basicMotor.Mobc=0.0;
							UartSendString("Mobc = %f", basicMotor.Mobc);
							ScreenDeviceDrawRect(USPiEnvGetScreen(),0,0,USPiEnvGetScreen()->m_nWidth,(USPiEnvGetScreen()->m_nHeight)/10,BLACK_COLOR);
							ScreenDeviceCursorHome(USPiEnvGetScreen());
							//LogWrite("",LOG_NOTICE,"Mobc = %f", basicMotor.Mobc);
//							TimerMsDelay(TimerGet(),150);
							buttonFlag = TRUE;
						}
						else if(verticalAxis < 127 && horizontalAxis == 127)
						{
							basicMotor.Mobc+=0.1;
							UartSendString("Mobc = %f", basicMotor.Mobc);
							ScreenDeviceDrawRect(USPiEnvGetScreen(),0,0,USPiEnvGetScreen()->m_nWidth,(USPiEnvGetScreen()->m_nHeight)/10,BLACK_COLOR);
							ScreenDeviceCursorHome(USPiEnvGetScreen());
		//					LogWrite("",LOG_NOTICE,"Mobc = %f", basicMotor.Mobc);
//							TimerMsDelay(TimerGet(),150);
							buttonFlag = TRUE;
						}
						else if(verticalAxis == 127 && horizontalAxis > 127)
						{
							basicMotor.U+=0.1;
							UartSendString("U = %f", basicMotor.U);
							ScreenDeviceDrawRect(USPiEnvGetScreen(),0,0,USPiEnvGetScreen()->m_nWidth,(USPiEnvGetScreen()->m_nHeight)/10,BLACK_COLOR);
							ScreenDeviceCursorHome(USPiEnvGetScreen());
		//					LogWrite("",LOG_NOTICE,"U = %f", basicMotor.U);
//							TimerMsDelay(TimerGet(),150);
							buttonFlag = TRUE;
						}
						else if(verticalAxis == 127 && horizontalAxis < 127)
						{
							basicMotor.U-=0.1;
							if(basicMotor.U <=0) basicMotor.U = 0.0;
							UartSendString("U = %f", basicMotor.U);
							ScreenDeviceDrawRect(USPiEnvGetScreen(),0,0,USPiEnvGetScreen()->m_nWidth,(USPiEnvGetScreen()->m_nHeight)/10,BLACK_COLOR);
							ScreenDeviceCursorHome(USPiEnvGetScreen());
		//					LogWrite("",LOG_NOTICE,"U = %f", basicMotor.U);
//							TimerMsDelay(TimerGet(),150);
							buttonFlag = TRUE;
						}
						break;


				}
		}
		else if(horizontalAxis == 127 && verticalAxis ==127)// when buttonFlag = TRUE and left joy is in central pos
		{
			buttonFlag = FALSE;
		}
	}
}


void MouseStatusHandler()
{
	//TODO:
}

void Symulator(USPiGamePadState *pState)
{

	if(startFlag == TRUE) // to jest wykonywane jesli nie ruszam joyem - szybko
	{
	//plot motor symulation


		switch(simulationMotor){
		case(FIRSTMOTOR):
			if(Simulation(USPiEnvGetScreen(),&basicMotor, &basicSimulation,WHITE_COLOR,pState) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
			}
			break;
		case(SECONDMOTOR):
			if(Simulation(USPiEnvGetScreen(),&basicMotor2, &basicSimulation,RED_COLOR,pState) != 0)
			{
				LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
			}
			break;
		case(BOTHMOTORS):
			if(SimulationBoth(USPiEnvGetScreen(),&basicMotor, &basicMotor2, &basicSimulation, &basicSimulation2,RED_COLOR,WHITE_COLOR,pState) !=0 )
			{
				LogWrite("Chart Error ", LOG_ERROR, "Simulation was stopped due to unexpected error!");
			}
			break;
		}
	}
}

