/*
 * mymath.c
 *
 *  Created on: 24 maj 2016
 *      Author: Komar
 */
#include "mymath.h"
#include <math.h>

// sinus Lookup table for range (0 - 6.28) multiplied 100
const int sinusLookupv1[63] = {0,9,19,29,38,47,56,64,71,78,84,89,93,96,98,99,99,99,97,94,90,86,80,74,67,59,51,42,33,23,14,4,-5,-15,-25,-35,-44,-52,-61,-68,-75,-81,-87,-91,-95,-97,-99,-99,-99,-98,-95,-92,-88,-77,-70,-63,-55,-46,-37,-27,-18,-8,0};

const int sinusLookup[628] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,38,39,40,41,42,43,44,45,46,47,47,48,49,50,51,52,53,53,54,55,56,57,58,58,59,60,61,62,62,63,64,65,65,66,67,68,68,69,70,71,71,72,73,73,74,75,75,76,77,77,78,78,79,80,80,81,81,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,90,91,91,92,92,92,93,93,93,94,94,94,95,95,95,96,96,96,96,97,97,97,97,97,98,98,98,98,98,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,98,98,98,98,98,98,97,97,97,97,96,96,96,96,95,95,95,94,94,94,93,93,93,92,92,92,91,91,90,90,90,89,89,88,88,87,87,86,86,85,85,84,84,83,83,82,82,81,80,80,79,79,78,77,77,76,75,75,74,73,73,72,71,71,70,69,69,68,67,66,66,65,64,63,63,62,61,60,59,59,58,57,56,55,54,54,53,52,51,50,49,48,48,47,46,45,44,43,42,41,40,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,21,20,19,18,17,16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-36,-37,-38,-39,-40,-41,-42,-43,-44,-45,-46,-46,-47,-48,-49,-50,-51,-52,-52,-53,-54,-55,-56,-57,-57,-58,-59,-60,-61,-61,-62,-63,-64,-65,-65,-66,-67,-68,-68,-69,-70,-70,-71,-72,-73,-73,-74,-75,-75,-76,-76,-77,-78,-78,-79,-80,-80,-81,-81,-82,-82,-83,-84,-84,-85,-85,-86,-86,-87,-87,-88,-88,-89,-89,-89,-90,-90,-91,-91,-92,-92,-92,-93,-93,-93,-94,-94,-94,-95,-95,-95,-96,-96,-96,-96,-97,-97,-97,-97,-97,-98,-98,-98,-98,-98,-98,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-98,-98,-98,-98,-98,-98,-97,-97,-97,-97,-96,-96,-96,-96,-95,-95,-95,-94,-94,-94,-94,-93,-93,-92,-92,-92,-91,-91,-90,-90,-90,-89,-89,-88,-88,-87,-87,-86,-86,-85,-85,-84,-84,-83,-83,-82,-82,-81,-80,-80,-79,-79,-78,-77,-77,-76,-75,-75,-74,-74,-73,-72,-71,-71,-70,-69,-69,-68,-67,-66,-66,-65,-64,-63,-63,-62,-61,-60,-59,-59,-58,-57,-56,-55,-55,-54,-53,-52,-51,-50,-49,-49,-48,-47,-46,-45,-44,-43,-42,-41,-41,-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,};

unsigned ScreenDeviceDrawSine(TScreenDevice *pThis, unsigned startPosX, unsigned startPosY, unsigned amplitude, TScreenColor color)
{
	for(unsigned x = 0; x<1000;x++)
		{
			int actualPosY = (int)(amplitude*sinusLookup[5*x%628]); //[odwrotnosc dzielnika okresu*x % liczba punktow
			ScreenDeviceSetPixel(pThis,startPosX+x,startPosY-actualPosY,color);
		}
	return 0;
}



