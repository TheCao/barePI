/*
 * mymath.h
 *
 *  Created on: 24 maj 2016
 *      Author: Komar
 */

#ifndef SRC_MAIN_MYMATH_H_
#define SRC_MAIN_MYMATH_H_
#include <uspienv/screen.h>
//#include <uspios.h>  //TODO: delete in final version
#define TESTING		 1

unsigned actMenuPosition, actBasicMotor;
typedef struct symulationParams {
	unsigned startPosX,startPosY;
	unsigned int actualTime,tempOmega; // for chart printing purposes;
	double dt,I,didt,d2thetadt,dthetadt,tk;
} simulationParams_t;

typedef struct motorParams {
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

unsigned Simulation(TScreenDevice *pThis,motorParams_t *motorParams, simulationParams_t symParams,TScreenColor color);
unsigned ChangeMotorParam(motorParams_t *structure,unsigned actMenuPosition,signed value);
unsigned ChangeSimulationParam(simulationParams_t *structure,unsigned MenuPosition,signed value);
unsigned PrintActMotorParam(unsigned val);
unsigned PrintActSimulationParam(unsigned val);

#endif /* SRC_MAIN_MYFUNC_H_ */


