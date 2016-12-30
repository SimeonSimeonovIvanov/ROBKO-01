#include <math.h>

#define PI	( 3.14159265f/*35897932384626433832795f*/ )
#define RAD	( 180.0f / PI )

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define H	( 195.0f )
#define L1	( 178.0f )
#define L2	( 178.0f )
#define LG	( 92.0f )

//#define R1	( 0.0f	)
#define C	( 180.f / PI )

// Motor Consts [ Steps / Rad ]:
#define T1const	( 1222.7f )
#define T2const	( 1145.91559026/*16464175359630962827f*/ )	// 1161.4f;
#define T3const	( 668.45076098f/*596041022931180616488f*/ )	// 680.1f;
#define T4const	( 244.7f )
#define T5const	( 244.7f )
#define T6const	( 2 * 270.0f )

// 90.f * [ Steps / Deg ]:
#define T1Max			( +90.0f * ( T1const / RAD ) )
#define T1Min			( -90.0f * ( T1const / RAD ) )

#define T2Max			( +135.0f * ( T2const / RAD ) )
#define T2Min			( -45.0f * ( T2const / RAD ) )
////////////////////////////////////////////////////////
#define T3Max			( +90.0f * ( T3const / RAD ) )
#define T3Min			( -60.0f * ( T3const / RAD ) )

void Robko01_Inverse_Kinematics_fig4_4
(
	float X,
	float Y,
	float Z,
	float P,
	float R,
	float G,
	float R1,
	int arrMotor[5]
);

void Robko01_Inverse_Kinematics
(
	float X0,
	float Y0,
	float Z0,
	float P0,
	float R0,
	float G0,
	float R1,
	int arrMotor[5]
);

void Robko01_Forward_Kinematics_From_Motor_Pos
(
	int arrMotor[6],
	float arrPos[6],
	float R1
);

// SGN(n): Equals the sign of the numeric expression n. It equals 1 if it is positive, 0 if it is zero, and -1 if it is negative.
int SGN(float value);

float radians( float angle );
float degrees( float rad );
