#include "stdafx.h"

#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <windows.h>

#include "arm.h"
#include "camera.h"
#include "point3f.h"

// Robot's arm controls
static float baseTransX = -0.5f; // 0
static float baseTransZ = 0;
static float baseSpin = 180;	   // 1
static float shoulderAng = 89;  // 2
static float elbowAng = 0;
static float wristAng = 0;	  // 3
static float wristTwistAng = 0;
static float fingerAng = 0;	// 4

// Robot's colors
char arms[] = { 128, 128, 128 };
char joints[] = { 0, 68, 119 };
char fingers[] = { 150, 0, 24 };
char fingerJoints[] = { 128, 128, 128 };

// User interface global variables
static bool leftButtonDown = false;
static bool middleButtonDown = false;

static int robotControl = 1;
static float oldX, oldY, newX, newY;

////////////////////////////////////////////////////////
float cameraRate = 0;
extern Point3f fOrigin;
extern Point3f fFocus;
extern Point3f fUpVector;
////////////////////////////////////////////////////////
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f,lz = -1.0f;
// XZ position of the camera
//float xCamera = 0.0f,zCamera = 5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
////////////////////////////////////////////////////////

#include <math.h>

const float PI = 3.1415926535897932384626433832795f;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float H	= 190.0f;
const float L1	= 178.0f;
const float L2	= 178.0f;
const float LG	= 1.0f;

const float R1	= 0.0f;
const float C	= ( 180.f / PI );

void Robko01_Inverse_Kinematics
(
	float X0,
	float Y0,
	float Z0,
	float R0,
	float P0
);

// SGN(n): Equals the sign of the numeric expression n. It equals 1 if it is positive, 0 if it is zero, and -1 if it is negative.
int SGN(float value);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Robko01_Inverse_Kinematics
(
	float X0,
	float Y0,
	float Z0,
	float R0,
	float P0
)
{
	// Var:
	float RR;
	float LF;
	float RM;
	float GA;
	float AL;

	float R0_temp = R0;
	float P0_temp = P0;

	// Motors [ Rad ]:
	float T1, T2, T3, T4, T5;

	if( 0.0 > Z0 ) {
		Z0 = 0.0f;
	}

	if( 300.0f > Z0 && 0.0f > X0 ) {
		X0 = 100.0f;
	}

	RR = sqrt( pow( X0, 2 ) + pow( Y0, 2 ) );
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
			RM = sqrt( pow( LF, 2 ) - pow( H, 2 ) );
		} else {
			RM = sqrt( pow( LF, 2) - pow( H - Z0, 2 ) );
		}
	}

	if( RR > RM ) {
		RR = RM;
	}

	P0 = P0 / C;
	R0 = R0 / C;

	R0 = RR - LG * cos( R0 );
	Z0 = H - Z0 - LG * sin( P0 );

	if( /* !R0 */
		0.001f >= R0 && -0.001f <= R0
	) {
		GA = SGN( Z0 ) * ( PI / 2 );
	} else {
		GA = atan2( Z0, R0 );
	}

	AL = sqrt( pow( R0, 2 ) + pow( Z0, 2 ) ) / 2;
	AL = atan( sqrt( pow( L1, 2 ) - pow( AL, 2 ) ) / AL );

	if( /* !X0 */
		0.001f >= X0 && -0.001f <= X0
	) {
		T1 = SGN( Y0 ) * PI / 2;
	} else {
		T1 = atan2( Y0, X0 );	
	}

	T2 = GA - AL;
	T3 = GA + AL;
	T4 = P0 + R0 + R1 * T1;
	T5 = P0 - R0 - R1 * T1;
	
//	printf("T1 = %f rad.\n", T1);
//	printf("T2 = %f rad.\n", T2);
//	printf("T3 = %f rad.\n", T3);
//	printf("T4 = %f rad.\n", T4);
//	printf("T5 = %f rad.\n", T5);

	T1 *= ( 180.0f / PI );
	T2 *= ( 180.0f / PI );
	T3 *= ( 180.0f / PI );

//	printf("T1 = %f deg.\n", T1);
//	printf("T2 = %f deg.\n", T2);
//	printf("T3 = %f deg.\n", T3);
//	printf("T4 = %f deg.\n", T4);
//	printf("T5 = %f deg.\n", T5);

	baseSpin = 180.0f - T1;

	shoulderAng = 90.0f + T2;
	elbowAng = (90.0f + T3) - shoulderAng;

	wristAng = (90 + R0_temp) - elbowAng;

	wristTwistAng = 0;//T5;
	fingerAng = 0;	// 4
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SERVO arrServo[8];

void armInitServo(void)
{
	int i;

	//Robko01_Inverse_Kinematics( 300, 0, H, 0, 0 );
	Robko01_Inverse_Kinematics( L1, 0, H, 0, 0 );

	arrServo[0].Angle = arrServo[0].spAngle		=	baseTransX;
	arrServo[1].Angle = arrServo[1].spAngle		=	baseTransZ;
	arrServo[2].Angle = arrServo[2].spAngle		=	baseSpin;
	arrServo[3].Angle = arrServo[3].spAngle		=	shoulderAng;
	arrServo[4].Angle = arrServo[4].spAngle		=	elbowAng;
	arrServo[5].Angle = arrServo[5].spAngle		=	wristAng;
	arrServo[6].Angle = arrServo[6].spAngle		=	wristTwistAng;
	arrServo[7].Angle = arrServo[7].spAngle		=	fingerAng;

	

	for( i = 0; i < 8; i++ ) {
		arrServo[i].bOn = false;
		arrServo[i].busi = false;
		arrServo[i].kSpeed = 0.1;
	}
}

void servoOn(LP_SERVO lpServo, bool isOn)
{
	lpServo->bOn = isOn;
}

void servoSetAngle(LP_SERVO lpServo, float angle)
{
	lpServo->spAngle = angle;
}

bool servoRun(LP_SERVO lpServo)
{
	if( lpServo->bOn ) {
		if( lpServo->spAngle < lpServo->Angle - lpServo->kSpeed ||
			lpServo->spAngle > lpServo->Angle + lpServo->kSpeed
		) {
			lpServo->busi = true;
			if( lpServo->spAngle > lpServo->Angle ) {
				lpServo->Angle += lpServo->kSpeed;
			} else {
				lpServo->Angle -= lpServo->kSpeed;
			}
		} else {
			lpServo->busi = false;
		}
	}

	baseTransX		= arrServo[0].Angle;
	baseTransZ		= arrServo[1].Angle;
	baseSpin		= arrServo[2].Angle;
	shoulderAng		= arrServo[3].Angle;
	elbowAng		= arrServo[4].Angle;
	wristAng		= arrServo[5].Angle;
	wristTwistAng	= arrServo[6].Angle;
	fingerAng		= arrServo[7].Angle;

	return lpServo->busi;
}

void cmd01(LP_SERVO lpServo, int n)
{
	float max;
	int i, max_index = 0;

	max = lpServo[0].spAngle - lpServo[0].Angle;
	if( max < 0 ) {
		max = -max;
	}

	for( i = 1; i < n; i++ ) {
		float temp = lpServo[i].spAngle - lpServo[i].Angle;

		if( temp < 0 ) {
			temp = -temp;
		}

		if(max < temp) {
			max = temp;
			max_index = i;
		}	
	}

	for(i = 0; i < n; i++) {
		if( i != max_index ) {
			float temp = lpServo[i].spAngle - lpServo[i].Angle;

			if( temp < 0 ) {
				temp = -temp;
			}

			lpServo[i].kSpeed *= temp / max;
		}
	}
}

void DrawUnitCylinder(int numSegs)  // x,y,z in [0,1], Y-axis is up
{
	int i;
	float AngIncr = (2.0f * 3.1415927f) / (float)numSegs;
	float Ang = AngIncr;

	//float *Px = (float*)malloc(numSegs * sizeof(float));
	static float Px[100];
	//float *Py = (float*)malloc(numSegs * sizeof(float));
	static float Py[100];
	
	Px[0] = 1;
	Py[0] = 0;

	for(i = 1; i < numSegs; i++, Ang += AngIncr) {
		Px[i] = (float)cosf(Ang);
		Py[i] = (float)sinf(Ang);
	}

	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();

		glTranslatef(0.5f, 0.5f, 0.5f);
		glScalef(0.5f, 0.5f, 0.5f);

		// Top
		glNormal3f(0, 1, 0);	
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0, 1, 0);
			for(i = 0; i < numSegs; i++) {
				glVertex3f(Px[i], 1, -Py[i]);
			}
			glVertex3f(Px[0], 1, -Py[0]);
		glEnd();

		// Bottom
		glNormal3f(0, -1, 0);
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0, -1, 0);
			for(i = 0; i < numSegs; i++) {
				glVertex3f(Px[i], -1, Py[i]);
			}
			glVertex3f(Px[0], -1, Py[0]);
	
		glEnd();

		// Sides
		glBegin(GL_QUAD_STRIP);
			for(i = 0; i < numSegs; i++) {
				glNormal3f(Px[i], 0, -Py[i]);
				glVertex3f(Px[i], 1, -Py[i]);
				glVertex3f(Px[i], -1, -Py[i]);
			}

			glNormal3f(Px[0], 0, -Py[0]);
			glVertex3f(Px[0], 1, -Py[0]);
			glVertex3f(Px[0], -1, -Py[0]);
		glEnd();

	glPopMatrix();

	//free(Px);
	//free(Py);
}

static void DrawUnitSphere(int numSegs)  // x,y,z in [0,1]
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(0.5f, 0.5f, 0.5f);
		glutSolidSphere(0.5f, numSegs, numSegs);
	glPopMatrix();
}

static void DrawUnitCone(int numSegs)  // x,y,z in [0,1], apex is in +Y direction
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(0.5f, 0, 0.5f);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(0.5f, 1, numSegs, numSegs);
	glPopMatrix();
}

static void DrawGroundPlane(int numSegs)
{
	glColor3f(0.7f, 0.7f, 0.7f);

	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
	glEnd();
}

static void DrawJoint(int numSegs)
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glScalef(0.15f, 0.15f, 0.12f);
		glRotatef(90, 1, 0, 0);
		glTranslatef(-0.5f, -0.5f, -0.5f);
		glColor3ubv((unsigned char*)joints);
		DrawUnitCylinder(numSegs);
	glPopMatrix();
}

static void DrawBase(int numSegs)
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glScalef(0.2f, 0.025f, 0.2f);
		glTranslatef(-0.5f, 0, -0.5f);
		glColor3ubv((unsigned char*)joints);
		DrawUnitCylinder(numSegs);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-0.05f, 0, -0.05f);
		glScalef(0.1f, GL_BASE_HGT, 0.1f);
		glColor3ubv((unsigned char*)arms);
		DrawUnitCylinder(numSegs);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, GL_BASE_HGT, 0);
		DrawJoint(numSegs);
	glPopMatrix();
}

static void DrawArmSegment(int numSegs, int seg)
{
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
		glTranslatef(-0.05f, 0, -0.05f);

		if(1 == seg) glScalef(0.1f, GL_HUMERUS, 0.1f);
		if(2 == seg) glScalef(0.1f, GL_ULNA, 0.1f);

		glColor3ubv((unsigned char*)arms);
		DrawUnitCylinder(numSegs);
	glPopMatrix();
	
	glPushMatrix();
		if(1 == seg) glTranslatef(0, GL_HUMERUS, 0);
		if(2 == seg) glTranslatef(0, GL_ULNA, 0);

		DrawJoint(numSegs);
	glPopMatrix();
}

static void DrawWrist(int numSegs)
{
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
		glTranslatef(-0.04f, 0, -0.04f);
		glScalef(0.08f, 0.2f, 0.08f);
		glColor3ubv((unsigned char*)fingers);
		DrawUnitCylinder(numSegs);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0, 0.2f, 0);
		glScalef(0.12f, 0.12f, 0.12f);
		glTranslatef(-0.5f, -0.5f, -0.5f);
		glColor3ubv((unsigned char*)fingerJoints);
		DrawUnitSphere(numSegs);
	glPopMatrix();
}

static void DrawFingerBase(int numSegs)
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(-0.025f, 0, -0.025f);
		glScalef(0.05f, 0.3f, 0.05f);
		glColor3ubv((unsigned char*)fingers);
		DrawUnitCylinder(numSegs);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0, 0.3f, 0);
		glScalef(0.08f, 0.08f, 0.08f);
		glTranslatef(-0.5f, -0.5f, -0.5f);
		glColor3ubv((unsigned char*)fingerJoints);
		DrawUnitSphere(numSegs);
	glPopMatrix();
}

static void DrawFingerTip(int numSegs)
{
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
		glScalef(0.05f, 0.25f, 0.05f);
		glTranslatef(-0.5f, 0, -0.5f);
		glColor3ubv((unsigned char*)fingers);
		DrawUnitCone(numSegs);
	glPopMatrix();
}

static void DrawRobotArm(int numSegs)
{
	glMatrixMode(GL_MODELVIEW);
	
	//////////////////////////////////////////////
	// Чертае отделните стави (сиви тръби)
	glTranslatef(baseTransX, 0.0f, baseTransZ);
	glRotatef(baseSpin, 0.0f, 360.0f, 0.0f);
	DrawBase(64);

	glTranslatef(0.0f, GL_BASE_HGT, 0.0);
	glRotatef(shoulderAng, 0.0f, 0.0f, 90.0f);
	DrawArmSegment(64, 1);

	glTranslatef(0.0f, GL_HUMERUS, 0.0f);
	glRotatef(elbowAng, 0.0f, 0.0f, 90.0f);
	DrawArmSegment(64, 2);

	glTranslatef(0.0f, 3 * 0.178f, 0.0f);
	glRotatef(wristAng, 0.0f, 0.0f, 90.0f);
	DrawWrist(16);
	///////////////////////////////////////////////
	// Ротация на китката
	glRotatef(wristTwistAng, 0.0f, 180.0f, 0.0f);
	///////////////////////////////////////////////
	glPushMatrix();
		glTranslatef(0.0f, 0.2f, -0.15f);
		glRotatef(180, 0.0f, 90, 90.0f);
		DrawFingerBase(16); // Червен цилиндър

		glTranslatef(0.0f, 0.3f, 0.0f);
		glRotatef(fingerAng, 90.0f, 0.0f, 0.0f);
		DrawFingerTip(16); // Червен конус
	glPopMatrix();
	///////////////////////////////////////////////
	glTranslatef(0.0f, 0.2f, 0.0f);
	glRotatef(180.0f, 0.0f, 90.0f, 90.0f);

	glPushMatrix();
		glScalef(0.08f, 0.08, 0.08);
		glTranslatef(-0.5f, -2.5f, -0.5f);
		glColor3ubv((unsigned char*)fingerJoints);
		DrawUnitSphere(16);
	glPopMatrix();

	// Чертае 2-ри пръст		
	glTranslatef(0.0f, -0.15f, 0);
	glRotatef(180-fingerAng, 90, 0, 0);
	
	DrawFingerTip(16); // Червен конус
	///////////////////////////////////////////////
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt
	(
		0.0f, 2.0f, 4.0f,	// Eye
		0.0f, 0.5f, 0.0f,	// Center
		0.0f, 1.0f, 0.0f		// UP
	);

	///////////////////////////////////////////
	if(deltaMove) {
		computePos( deltaMove );
	}
	if(deltaAngle) {
		computeDir( deltaAngle );
	}

	cameraMove( cameraRate );

	//fOrigin.y = 1.0f;
	//fFocus.x = xCamera + lx;
	//fFocus.y = 1.0f;

	//fUpVector.x = 0.0f;
	//fUpVector.y = 1.0f;
	//fUpVector.z = 0.0f;

	/*gluLookAt
	(
		fOrigin.x, fOrigin.y, fOrigin.z,
		fFocus.x, fFocus.y, fFocus.z,
		fUpVector.x, fUpVector.y, fUpVector.z
	);*/
	///////////////////////////////////////////

	DrawGroundPlane(16);
	DrawRobotArm(16);

	glutSwapBuffers();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, w / h, 0.1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.0f, 0.5f, -7.0f);
}

void myIdle()
{
	int i;

	for(i = 0; i < 10; i++) {
		servoRun(&arrServo[i]);
	}

	glutPostRedisplay();
}

void KeyboardFunc(unsigned char Key, int x, int y)
{
	char c = (char)Key;

	if(c >= '1' && c <= '6') {
		robotControl = c - '0';
	}

	if(Key == 27) {
	//	Application.Exit();		 // ESC
	}
}

void pressKey(int key, int xx, int yy)
{

	switch (key) {
	case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
	case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
	case GLUT_KEY_UP : deltaMove = 0.5f; break;
	case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT :
	case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
	case GLUT_KEY_UP :
	case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void computePos(float deltaMove)
{
	fOrigin.x += deltaMove * lx * 0.05f;
	fOrigin.z += deltaMove * lz * 0.01f;
}

void computeDir(float deltaAngle)
{
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void MouseFunc(int button, int state, int x, int y)
{
	if( y < 256 ) {
		fOrigin.z += 0.1;
	} else {
		fOrigin.z -= 0.1;
	}

	if( newX != x || newY != y) {
		cameraRate = .001;
	} else {
		cameraRate = 0; 
	}

	newX = x;
	newY = y;

	switch(button) {
	case GLUT_LEFT_BUTTON:
		switch(state) {
		case GLUT_UP: leftButtonDown = false; break;
		case GLUT_DOWN: leftButtonDown = true; break;
		}
	 break;

	case GLUT_MIDDLE_BUTTON:
		switch(state) {
		case GLUT_UP: middleButtonDown = false; break;
		case GLUT_DOWN: middleButtonDown = true; break;
		 break;
		}
	 break;
	}
}

void MotionFunc(int x, int y)
{
	oldX = newX;
	oldY = newY;
	newX = x;
	newY = y;

	float RelX = ( newX - oldX ) / glutGet(GLUT_WINDOW_WIDTH);
	float RelY = ( newY - oldY ) / glutGet(GLUT_WINDOW_HEIGHT);

	if( leftButtonDown ) {
		switch( robotControl ) {
		case 1:
			//baseTransX += RelX;
			//baseTransZ += RelY;
			baseSpin += RelX * 180.0f;
		 break;

		case 2:
			shoulderAng += RelY * -90.0f;

			if( shoulderAng > 45 ) {
				shoulderAng = 45;
				break;
			}
			if( shoulderAng < -90 ) {
				shoulderAng = -90;
				break;
			}
		 //break;

		case 3:
			elbowAng += RelY * 90.0f;

			if( elbowAng > 0 ) {
				elbowAng = 0;
				break;
			}
			if( elbowAng < -135 ) {
				elbowAng = -135;
				break;
			}
		 //break;

		case 4:
			wristAng += RelY * -90.0f;

			if( wristAng > 90 ) wristAng = 90;
			if( wristAng < -90 ) wristAng = -90;
		 break;

		case 5:
			wristTwistAng += RelY * 180.0f;
		 break;

		case 6:
			fingerAng += RelY * 90.0f;

			if(fingerAng < 45) fingerAng = 45;
			if(fingerAng > 130) fingerAng = 130;
		 break;
		};
	}
}

/*
public static void Main(string[] args)
{
	glutInit();
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

	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutIdleFunc(myIdle);

	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);

	glutMainLoop();
}
*/
