/*
 * mymath.h
 *
 *  Created on: 24 maj 2016
 *      Author: Komar
 */

#ifndef SRC_MAIN_MYMATH_H_
#define SRC_MAIN_MYMATH_H_
#include <uspienv/screen.h>
#include <uspienv/periph.h>
#include <uspi.h>

//#define TESTING		 1

boolean isMouseConnected;
boolean isKeyboardConnected;
boolean isGamepadConnected;
boolean isChartPrinted;
int horizontalAxis;
int verticalAxis;


unsigned actMenuPosition, actBasicMotor;
typedef struct simulationParams {
	unsigned startPosX,startPosY;
	unsigned int actualTimeUI,tempOmega; // for chart printing purposes;
	double dt,I,didt,d2thetadt,dthetadt,tk,actualTimeD;
} simulationParams_t;

typedef struct motorParams {
	int number;
	double R,L,Ke,Km,J,B,Mobc,U;
} motorParams_t;


typedef enum enabledmode {
	NONEENABLED,
	DCMOTOR,
	SIMULATION

} enabledMode_t;

enabledMode_t actualEnabledMode;

typedef enum simulationMotorMode {
	FIRSTMOTOR,
	SECONDMOTOR,
	BOTHMOTORS
} simulationMotorMode_t;

simulationMotorMode_t simulationMotor;



unsigned Simulation(TScreenDevice *pThis,motorParams_t *motorParams, simulationParams_t symParams,TScreenColor color,const USPiGamePadState *pState);
unsigned ChangeMotorParam(motorParams_t *structure,unsigned actMenuPosition,signed value);
unsigned ChangeSimulationParam(simulationParams_t *structure,unsigned MenuPosition,signed value);
unsigned PrintActMotorParam(unsigned val);
unsigned PrintActSimulationParam(unsigned val);
void getDefaultValues();
unsigned UartSendString(const char *pMessage,...);
void KeyPressedHandler (const char *pString);
void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState);
void MouseStatusHandler();
#endif /* SRC_MAIN_MYFUNC_H_ */


