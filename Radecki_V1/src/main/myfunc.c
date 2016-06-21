#include "myfunc.h"

/*
 * myfunc.c
 *
 *  Created on: 24 maj 2016
 *      Author: Komar
 */
#include <math.h>
#include "myfunc.h"

extern void LogWrite (const char *pSource, unsigned Severity, const char *pMessage, ...);
unsigned actMenuPosition = 1;
unsigned actBasicMotor = 1;

motorParams_t basicMotor = {
		.R = 			2,
		.L =			0.1,
		.Ke =			0.1,
		.Km =			0.1,
		.J =			0.1,
		.B =			0.5,
		.Mobc =			0.0,
		.U = 			12.0
};

simulationParams_t basicSimulation ={
		.actualTime = 	0,
		.dt =			0.01,
		.tempOmega = 	0,
		.I = 			0.0,
		.didt = 		0.0,
		.d2thetadt =	0.0,
		.dthetadt = 	0.0,
		.tk = 			10.0
};

motorParams_t basicMotor2 = {
		.R = 			4,
		.L =			0.05,
		.Ke =			0.1,
		.Km =			0.1,
		.J =			0.1,
		.B =			0.3,
		.Mobc =			0.4,
		.U = 			40.0
};

motorParams_t copyBasicMotor, copyBasicMotor2;


unsigned Simulation(TScreenDevice *pThis,motorParams_t *motorParams, simulationParams_t symParams,TScreenColor color)
{

	for(double i=0.0;i<symParams.tk;i+=symParams.dt)
		{
			symParams.didt = (1/motorParams->L)*(motorParams->U-motorParams->R*symParams.I-motorParams->Ke*symParams.dthetadt);
			symParams.d2thetadt = (1/motorParams->J)*(motorParams->Km*symParams.I-motorParams->B*symParams.dthetadt-motorParams->Mobc);
			// integrals
			symParams.dthetadt+= symParams.d2thetadt*symParams.dt;
			symParams.I+=symParams.didt*symParams.dt;

			symParams.actualTime = i*100;
			symParams.tempOmega = symParams.dthetadt*400; // TODO: SKALOWANIE poniewaz ustalony zakres jest od 0 - 2 a mam dostêpne 512 px wysokosci to rozpinam wyniki na 0-400 px
			// plotting function, width 5 px
			for(signed i = -2;i<=2;i++)
			{
				ScreenDeviceSetPixel(pThis, symParams.startPosX + symParams.actualTime, symParams.startPosY - symParams.tempOmega+i, color);
			}

			if(i>=5) motorParams->Mobc = 0.3;
			if(i>=8) motorParams->U = 24;

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
		structure->tk+=value*0.1;
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
