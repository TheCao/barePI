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
unsigned ScreenDeviceDrawSine(TScreenDevice *pThis, unsigned startPosX, unsigned startPosY, unsigned amplitude, TScreenColor color);
float dt,dthetadt,d2thetadt,didt,I,U,Mobc,B,J,Km,Ke,L,R;
#endif /* SRC_MAIN_MYMATH_H_ */


