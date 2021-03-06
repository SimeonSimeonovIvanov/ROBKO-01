// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>

const float PI = 3.1415926535897932384626433832795f;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float H	= 190.0f;
const float L1	= 178.0f;
const float L2	= 178.0f;
const float LG	= 92.0f;

const float R1	= 0.0f;
const float C	= ( 180.f / PI );

// Motor Consts [ Steps / Rad ]:
const float T1const = 1222.7f;
const float T2const = 1161.4f;
const float T3const = 680.1f;
const float T4const = 244.7f;
const float T5const = 244.7f;
const float T6const = 27.0f;

void Robko01_Inverse_Kinematics_fig4_4
(
	float X,
	float Y,
	float Z,
	float R,
	float P,
	float R1,
	int arrMotor[5]
);

void Robko01_Inverse_Kinematics
(
	float X0,
	float Y0,
	float Z0,
	float R0,
	float P0,
	int arrMotor[5]
);

// SGN(n): Equals the sign of the numeric expression n. It equals 1 if it is positive, 0 if it is zero, and -1 if it is negative.
int SGN(float value);

float radians( float angle );
float degrees( float rad );
