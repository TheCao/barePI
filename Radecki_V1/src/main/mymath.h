/*
 * mymath.h
 *
 *  Created on: 24 maj 2016
 *      Author: Komar
 */

#ifndef SRC_MAIN_MYMATH_H_
#define SRC_MAIN_MYMATH_H_
#include <uspienv/screen.h>
//#include <uspios.h>  //problemy z osobnymi funkcjami malloc

const int sinusLookup[628];
const double sinusLookupF[628];
unsigned actMenuPosition;
typedef struct symulationParams {
	unsigned startPosX,startPosY;
	unsigned int actualTime,tempOmega; // for chart printing purposes;
	float dt,I,didt,d2thetadt,dthetadt,tk;
} symulationParams_t;

typedef struct motorParams {
	float R,L,Ke,Km,J,B,Mobc,U;
} motorParams_t;

typedef enum enabledmode {
	NONEENABLED,
	DCMOTOR,
	SIMULATION

} enabledMode_t;

enabledMode_t actualEnabledMode;
unsigned ScreenDeviceDrawSine(TScreenDevice *pThis, unsigned startPosX, unsigned startPosY, unsigned amplitude, TScreenColor color);
unsigned Simulation(TScreenDevice *pThis,motorParams_t *motorParams, symulationParams_t symParams,TScreenColor color);
unsigned ChangeMotorParam(motorParams_t *structure,unsigned actMenuPosition,signed value);


#endif /* SRC_MAIN_MYMATH_H_ */


