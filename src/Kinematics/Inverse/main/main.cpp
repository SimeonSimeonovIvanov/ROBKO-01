// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ROBKO_01.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Inverse_Kinematics_3D( float X, float Y, float Z, float AG );
void Inverse_Kinematics_2D( float Xhand, float Yhand );
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_arm( float x, float y, float z, float grip_angle_d );

int main(int argc, char* argv[])
{
	int arrMotor[6];

	Robko01_Inverse_Kinematics( L1, 0.0f, H, 0.0f, 0.0f, arrMotor );
	Robko01_Inverse_Kinematics_fig4_4( L1, 0.0f, H + L1, 0.0f, 0.0f, 0.0f, arrMotor );

	set_arm( L1, 0.0f, H, 0.0f );

	Robko01_Inverse_Kinematics( 2 * L1, 0.0f, H, 0.0f, 0.0f, arrMotor );

	Robko01_Inverse_Kinematics( L1, 0.0f, L1, 0.0f, 0.0f, arrMotor );

	Inverse_Kinematics_2D( 178.0f, 0.0f );
	Inverse_Kinematics_2D( 178.0f, 178.0f );
	Inverse_Kinematics_2D( 0.0f, 178.0f );

	return 0;
}

void Inverse_Kinematics_3D( float X, float Y, float Z, float AG )
{
	const float L1 = 178.0f;
	const float L2 = 178.0f;
	float L12;

	float w1;
	float z1;
	float w2 = X;
	float z2 = Z;

	float A1;
	float A2;
//	float A3;
	float A12;

	L12 = sqrt( pow( w2, 2 ) + pow( z2, 2 ) );

	A12 = atan( z2 / w2 );

	if( L12 > L1 + L2 ) {
		// then there is no solution as arm can not reach the target
		return;
	}

	A1 = acos( ( pow( L1, 2 ) + pow( L12, 2 ) - pow( L2, 2 ) ) / ( 2 * L1 * L12 ) ) + A12;

	w1 = L1 * cos( A1 );
	z1 = L1 * sin( A1 );

	A2 = atan( ( z2 - z1 ) / ( w2 - w1 ) ) - A1;
//	A3 = AG - A1 - A2;

	A1 *= ( 180.0f / PI );
	A2 *= ( 180.0f / PI );


	A2 = A1 - A2;
//	A3 *= ( 180.0f / PI );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Inverse_Kinematics_2D( float Xhand, float Yhand ) // +++
{
	const float L1 = 178.0f;
	const float L2 = 178.0f;

	float Qhand = acos( Xhand / ( L1 + L2 ) );

	// By the Pythagorean theorem)
	float B = pow( Xhand, 2 ) + pow( Yhand, 2 );
	float q1 = atan2( Yhand, Xhand );

	// By the law of cosines:
	float q2 = acos( ( pow( L1, 2 ) - pow( L2, 2 ) + B ) / ( 2.0f * L1 * sqrt( B ) ) );

	// I know you can handle addition
	float Q1 = q1 + q2;

	// By the law of cosines:
	float Q2 = acos( ( pow( L1, 2 ) + pow( L2, 2 ) - B ) / ( 2.0f * L1 * L2 ) );

	float Q3 = Qhand - Q1 - Q2;

	Q1 *= 180.0f / PI;
	Q2 *= 180.0f / PI;
	Q3 *= 180.0f / PI;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Arm dimensions( mm ) */
#define BASE_HGT	H		// Base hight
#define HUMERUS		L1		// Shoulder to Elbow
#define ULNA		L2		// Elbow to Wrist
#define GRIPPER		LG		// Gripper

/* pre-calculations */
const float hum_sq = pow( HUMERUS, 2 );
const float uln_sq = pow( ULNA, 2 );

/* arm positioning routine utilizing inverse kinematics

	z is height
	y is distance from base center out
	x is side to side.

	y,z can only be positive
*/
void set_arm( float x, float y, float z, float grip_angle_d )
{
	// grip angle in radians for use in calculations
	float grip_angle_r = radians( grip_angle_d );

	/* Base angle and radial distance from x,y coordinates */
	float bas_angle_r = atan2( x, y );
	float bas_angle_d = degrees( bas_angle_r );

	float rdist = sqrt( pow( x, 2 ) + pow( y, 2 ) );
	
	/* rdist is y coordinate for the arm */
	y = rdist;
	
	/* Grip offsets calculated based on grip angle */
	float grip_off_z = sin( grip_angle_r ) * GRIPPER;
	float grip_off_y = cos( grip_angle_r ) * GRIPPER;
	
	/* Wrist position */
	float wrist_z = ( z - grip_off_z ) - BASE_HGT;
	float wrist_y = y - grip_off_y;

	/* Shoulder to wrist distance ( AKA sw ) */
	float s_w = ( wrist_z * wrist_z ) + ( wrist_y * wrist_y );
	float s_w_sqrt = sqrt( s_w );
	
	/* s_w angle to ground */
	//float a1 = atan2( wrist_y, wrist_z );
	float a1 = atan2( wrist_z, wrist_y );

	/* s_w angle to humerus */
	float a2 = acos((( hum_sq - uln_sq ) + s_w ) / ( 2 * HUMERUS * s_w_sqrt ));

	/* shoulder angle */
	float shl_angle_r = a1 + a2;
	float shl_angle_d = degrees( shl_angle_r );

	/* elbow angle */
	float elb_angle_r = acos(( hum_sq + uln_sq - s_w ) / ( 2 * HUMERUS * ULNA ));
	float elb_angle_d = degrees( elb_angle_r );
	float elb_angle_dn = -( 180.0 - elb_angle_d );

	/* wrist angle */
	float wri_angle_d = ( grip_angle_d - elb_angle_dn ) - shl_angle_d;

	/* Servo pulses */
//	float bas_servopulse = 1500.0 - (( degrees( bas_angle_r )) * 11.11 );
//	float shl_servopulse = 1500.0 + (( shl_angle_d - 90.0 ) * 6.6 );
//	float elb_servopulse = 1500.0 -	(( elb_angle_d - 90.0 ) * 6.6 );
//	float wri_servopulse = 1500 + ( wri_angle_d	* 11.1 );

	/* Set servos */
//	servos.setposition( BAS_SERVO, ftl( bas_servopulse ));
//	servos.setposition( WRI_SERVO, ftl( wri_servopulse ));
//	servos.setposition( SHL_SERVO, ftl( shl_servopulse ));
//	servos.setposition( ELB_SERVO, ftl( elb_servopulse ));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////