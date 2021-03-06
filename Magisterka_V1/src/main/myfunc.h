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
#define INEQUALITYVALUE 0.00001 //used to check double type bit representation inequality
boolean isMouseConnected;
boolean isKeyboardConnected;
boolean isGamepadConnected;
boolean isChartPrinted;
boolean clearFlag;
int horizontalAxis;
int verticalAxis;
unsigned int * fifoBuffer;
unsigned int * fifoBuffer2;
unsigned int * buforRysunkowy;
unsigned int * buforRysunkowy2;
unsigned int * buforMobc;
unsigned int * buforMobcRys;
unsigned int * buforNapiecia;
unsigned int * buforNapieciaRys;


unsigned actMenuPosition, actBasicMotor;
typedef struct simulationParams {
	unsigned startPosX,startPosY,lenX,lenY,actualPosX,resolution;
	unsigned int actualTimeUI,tempOmega; // for chart printing purposes;
	unsigned int bufferIndex;
	double I,didt,d2thetadt,dthetadt,tk,actualTimeD, actualTimeTemp,stepEndTime;
	const double dt;
	double dtCopy, tkCopy;
	unsigned long bufferMax;
	boolean isFirstDraw;
	unsigned actualPosY;
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

boolean startFlag, readyFlag;


unsigned Simulation(TScreenDevice *pThis,motorParams_t *motorParams, simulationParams_t *symParams,TScreenColor color,const USPiGamePadState *pState);
unsigned SimulationBoth(TScreenDevice *pThis,motorParams_t *motorParams,motorParams_t *motorParams2, simulationParams_t *symParams, simulationParams_t *symParams2,TScreenColor color,TScreenColor color2,const USPiGamePadState *pState);
unsigned ChangeMotorParam(motorParams_t *structure,unsigned actMenuPosition,signed value);
unsigned ChangeSimulationParam(simulationParams_t *structure,unsigned MenuPosition,signed value);
unsigned PrintActMotorParam(unsigned val);
unsigned PrintActSimulationParam(unsigned val);
void setDefaultValues();
void finishSimulation();
unsigned getNumberLenght(double x);
int power(int base, int exp);
unsigned UartSendString(const char *pMessage,...);
void KeyPressedHandler (const char *pString);
void KeyboardHandler (const char *pString);
void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState);
void MouseStatusHandler();
void Symulator();
#endif /* SRC_MAIN_MYFUNC_H_ */


