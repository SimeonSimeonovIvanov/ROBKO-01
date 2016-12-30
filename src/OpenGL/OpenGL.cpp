// OpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include <gl/gl.h>
#include <gl/glut.h>
#include <windows.h>

#include <math.h>

#include "arm.h"
#include "camera.h"

extern SERVO arrServo[];

float PI = 3.1415926535897932384626433832795;

//////////////////////////////////////////////////////////////////
/* Servo control for AL5D arm */
 
#define ftl(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))  //float to long conversion
 
/* Servo names/numbers */
/* Base servo HS-485HB */
#define BAS_SERVO 0
/* Shoulder Servo HS-5745-MG */
#define SHL_SERVO 1
/* Elbow Servo HS-5745-MG */
#define ELB_SERVO 2
/* Wrist servo HS-645MG */
#define WRI_SERVO 3
/* Wrist rotate servo HS-485HB */
#define WRO_SERVO 4
/* Gripper servo HS-422 */
#define GRI_SERVO 5
 
/* pre-calculations */
float hum_sq = HUMERUS*HUMERUS;
float uln_sq = ULNA*ULNA;

/*
1 Radian = 57.2957795 degrees. To avoid calculating decimals with arduino, use something like the following:

deg = rad * 57296 / 1000

rad = deg * 1000 / 57296
*/

float radians(float deg)
{
	return deg * 1000.f / 57296.0f;
}

float degrees(float rad)
{
	return rad * 57296.0f / 1000.0f;
}

/*
	ARM positioning routine utilizing inverse kinematics

	x - is side to side
	y - distance from base center out, . 
	z - height

	y and z can only be positive !
*/
void set_arm( float x, float y, float z, float grip_angle_d )
{
	float grip_angle_r = radians( grip_angle_d );    //grip angle in radians for use in calculations

	/* Base angle and radial distance from x,y coordinates */
	float bas_angle_r = atan2( x, y );
	float rdist = sqrt( ( x * x ) + ( y * y ) );

	/* rdist is y coordinate for the arm */
	y = rdist;

	/* Grip offsets calculated based on grip angle */
	float grip_off_z = ( sin( grip_angle_r )) * GL_GRIPPER;
	float grip_off_y = ( cos( grip_angle_r )) * GL_GRIPPER;

	/* Wrist position */
	float wrist_z = ( z - grip_off_z ) - GL_BASE_HGT;
	float wrist_y = y - grip_off_y;

	/* Shoulder to wrist distance ( AKA sw ) */
	float s_w = ( wrist_z * wrist_z ) + ( wrist_y * wrist_y );
	float s_w_sqrt = sqrt( s_w );

	/* s_w angle to ground */
	//float a1 = atan2( wrist_y, wrist_z );
	float a1 = atan2( wrist_z, wrist_y );

	/* s_w angle to humerus */
	float a2 = acos((( hum_sq - uln_sq ) + s_w ) / ( 2 * GL_HUMERUS * s_w_sqrt ));

	/* shoulder angle */
	float shl_angle_r = a1 + a2;
	float shl_angle_d = degrees( shl_angle_r );

	/* elbow angle */
	float elb_angle_r = acos(( hum_sq + uln_sq - s_w ) / ( 2 * GL_HUMERUS * ULNA ));
	float elb_angle_d = degrees( elb_angle_r );
	float elb_angle_dn = -( 180.0 - elb_angle_d );

	/* wrist angle */
	float wri_angle_d = ( grip_angle_d - elb_angle_dn ) - shl_angle_d;
 
	/* Servo pulses */
/*
	float bas_servopulse = 1500.0 - (( degrees( bas_angle_r )) * 11.11 );
	float shl_servopulse = 1500.0 + (( shl_angle_d - 90.0 ) * 6.6 );
	float elb_servopulse = 1500.0 -	(( elb_angle_d - 90.0 ) * 6.6 );
	float wri_servopulse = 1500 + ( wri_angle_d	* 11.1 );
*/
	arrServo[2].spAngle = degrees(bas_angle_r);	// Въртене около централна ос.
	arrServo[3].spAngle = shl_angle_d * 6.6;	// Рамо				\ 
	arrServo[4].spAngle = elb_angle_d * 6.6;	// Лакет			 > - Взаимно свързани оси
	arrServo[5].spAngle = 0;	// Китка: горе/долу	/ 
	arrServo[6].spAngle = 0;	// Китка: завъртане
	arrServo[7].spAngle = 0;

	/* Set servos */
//	servos.setposition( BAS_SERVO, ftl( bas_servopulse ));
//	servos.setposition( WRI_SERVO, ftl( wri_servopulse ));
//	servos.setposition( SHL_SERVO, ftl( shl_servopulse ));
//	servos.setposition( ELB_SERVO, ftl( elb_servopulse ));
}

//////////////////////////////////////////////////////////////////


int WINAPI WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int iCmdShow
)
{
	glutInit(&iCmdShow, &lpCmdLine);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutGetWindow();
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(180, 100);
	glutCreateWindow("The Robot Arm");

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	glutIdleFunc(myIdle);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);

	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutKeyboardFunc(KeyboardFunc);

	cameraSetup(2, 2, 2, 1, 1, 1, 1, 2, 1);

	armInitServo();

	//set_arm( 0, 300, 100, 0 );

//	Robko_set_arm(
//		0, 250, 0,
//		0, 0, 0
//	);

	//arrServo[0].spAngle = -0;
	//arrServo[1].spAngle = 0;

//	arrServo[2].spAngle = -100;	// Въртене около централна ос.
//	arrServo[3].spAngle = -45;	// Рамо				\ 
//	arrServo[4].spAngle = -20;	// Лакет			 > - Взаимно свързани оси
//	arrServo[5].spAngle = -90;	// Китка: горе/долу	/ 
//	arrServo[6].spAngle = 45;	// Китка: завъртане
//	arrServo[7].spAngle = 130;	// Пръсти: отваряне/затваряне

	//cmd01( &arrServo[2], 6 );

	//arrServo[0].bOn = true;
	//arrServo[1].bOn = true;

	arrServo[2].bOn = true;
	arrServo[3].bOn = true;
	arrServo[4].bOn = true;
	arrServo[5].bOn = true;
	arrServo[6].bOn = true;
	arrServo[7].bOn = true;

	glutMainLoop();

	return 0;
}
