#include "ROBKO_01.h"

#include <math.h>

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
)
{
	//const float L	= L1;
	const float LL	= LG;

	float Q1, Q2, Q3, Q4, Q5;
	float a, b, R0, RR, Z0;
	//////////////////////////////////////////////////////////////////////////////////
	R = radians( R );
	P = radians( P );
	
	Q1 = (float)atan2( Y, X );
	RR = (float)sqrt( pow( X, 2 ) + pow( Y, 2 ) );

	Q4 = P + R + R1 * Q1;
	Q5 = P - R - R1 * Q1;

	R0 = (float)(RR - LL * cos( P ));
	Z0 = (float)(Z - H - LL * sin( P ));

	//////////////////////////////////////////////////////////////////////////////////
	//b = atan2( Z0, R0 );
	//a = sqrt( atan( ( 4.0f * pow( L, 2 ) ) / ( pow( R0, 2 ) + pow( Z0, 2 ) ) - 1 ) );

	if( /* !R0 */
		0.001f >= R0 && -0.001f <= R0
	) {
		b = SGN( Z0 ) * ( PI / 2 );
	} else {
		b = (float)atan2( Z0, R0 );
	}

	a = (float)sqrt( pow( R0, 2 ) + pow( Z0, 2 ) ) / 2;
	a = (float)atan( sqrt( pow( L1, 2 ) - pow( a, 2 ) ) / a );
	//////////////////////////////////////////////////////////////////////////////////
	Q2 = a + b;
	Q3 = b - a;
	//////////////////////////////////////////////////////////////////////////////////
	arrMotor[0] = (int)-( Q1 * T1const );
	arrMotor[1] = (int)-( Q2 * T2const );
	arrMotor[2] = (int)-( Q3 * T3const );
	arrMotor[3] = (int)-( Q4 * T4const );
	arrMotor[4] = (int)-( Q4 * T5const );
	arrMotor[5] = (int)-( radians( G ) * T6const );
	//////////////////////////////////////////////////////////////////////////////////
}

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
)
{
	// Var:
	float RR;
	float LF;
	float RM;
	float GA;
	float AL;

	// Motors [ Rad ]:
	float T1, T2, T3, T4, T5;

	if( 0.0 > Z0 ) {
		Z0 = 0.0f;
	}

	if( 300.0f > Z0 && 0.0f > X0 ) {
		X0 = 100.0f;
	}

	RR = (float)sqrt( pow( X0, 2 ) + pow( Y0, 2 ) );
	LF = L1 + L2 + LG;

	if( /* Z0 == H */
		( Z0 <= (H + 0.001f) ) &&
		( Z0 >= (H - 0.001f) )
	) {
		RM = LF;
	} else {
		if( /* !Z0 */
			0.001f >= Z0 && -0.001f <= Z0
		) {
			RM = (float)sqrt( pow( LF, 2 ) - pow( H, 2 ) );
		} else {
			RM = (float)sqrt( pow( LF, 2) - pow( H - Z0, 2 ) );
		}
	}

	if( RR > RM ) {
		RR = RM;
	}

	// ???
	P0 = -P0;

	P0 = P0 / C;
	R0 = R0 / C;

	float R00 = RR - LG * (float)cos( P0 );
	float Z00 = H - Z0 - LG * (float)sin( P0 );

	if( /* !R0 */
		0.001f >= R00 && -0.001f <= R00
	) {
		GA = SGN( Z00 ) * ( PI / 2 );
	} else {
		GA = (float)atan2( Z00, R00 );
	}

	AL = (float)sqrt( pow( R00, 2 ) + pow( Z00, 2 ) ) / 2;
	AL = (float)atan( sqrt( pow( L1, 2 ) - pow( AL, 2 ) ) / AL );

	if( /* !X0 */
		0.001f >= X0 && -0.001f <= X0
	) {
		T1 = SGN( Y0 ) * PI / 2;
	} else {
		T1 = (float)atan2( Y0, X0 );	
	}

	T2 = GA - AL;
	T3 = GA + AL;
	T4 = P0 + R0 + R1 * T1;
	T5 = P0 - R0 - R1 * T1;
	//////////////////////////////////////////////////////////////////////////////////
	arrMotor[0] = (int)-(T1const * T1);
	arrMotor[1] = (int)+(T2const * T2);
	arrMotor[2] = (int)+(T3const * T3);
	arrMotor[3] = (int)+(T4const * T4);
	arrMotor[4] = (int)+(T5const * T5);
	arrMotor[5] = (int)+(T6const * radians( G0 ));
	//////////////////////////////////////////////////////////////////////////////////
}

// SGN(n): Equals the sign of the numeric expression n. It equals 1 if it is positive, 0 if it is zero, and -1 if it is negative.
int SGN(float value)
{
	if( value <= 0.001 && value >= -0.001 ) {
		return 0;
	}
	if( 0.001 < value ) {
		return 1;
	}
	return -1;
}

void Robko01_Forward_Kinematics_From_Motor_Pos
(
	int arrMotor[6],
	float arrPos[6],
	float R1
)
{
	float XX, YY, ZZ, PP, RR, RP;
	float T1 = arrMotor[0] / T1const;
	float T2 = arrMotor[1] / T2const;
	float T3 = arrMotor[2] / T3const;
	float T4 = arrMotor[3] / T4const;
	float T5 = arrMotor[4] / T5const;
	float T6 = arrMotor[5] / T6const;

	PP = ( T4 + T5 ) / 2;
	RR = ( T4 - T5 ) / 2 - R1 * T1;

	RP = L1 * (float)cos( T2 ) + L2 * (float)cos( T3 ) + LG * (float)cos( PP );
	XX = RP * (float)cos( T1 );
	YY = RP * (float)sin( T1 );
	ZZ = H - L1 * (float)sin( -T2 ) - L2 * (float)sin( -T3 ) - LG * (float)sin( -PP );

	PP = PP * C;
	RR = RR * C;

	arrPos[0] = XX;
	arrPos[1] = YY;
	arrPos[2] = ZZ;
	arrPos[3] = PP;
	arrPos[4] = -RR;
	arrPos[5] = -degrees(T6);
}

float radians( float angle )
{
	float rad = angle / ( 180.0f / PI );
	return rad;
}

float degrees( float rad )
{
	float angle = rad * ( 180.0f / PI );
	return angle;
}
