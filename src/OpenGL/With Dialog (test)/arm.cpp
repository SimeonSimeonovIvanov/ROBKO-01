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
static float baseSpin = 0;	   // 1
static float shoulderAng = -10;  // 2
static float elbowAng = -120;
static float wristAng = 0;	  // 3
static float wristTwistAng = 0;
static float fingerAng = 45;	// 4

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
//float cameraRate = 0;
extern Point3f fOrigin;
extern Point3f fFocus;
extern Point3f fUpVector;

////////////////////////////////////////////////////////
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f,lz = -1.0f;
// XZ position of the camera
float x = 0.0f,z = 5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
//int xOrigin = -1;
////////////////////////////////////////////////////////

SERVO arrServo[8];

void armInitServo(void)
{
	int i;

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
		arrServo[i].kSpeed = 0.05;
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
		glScalef(0.1f, 0.4f, 0.1f);
		glColor3ubv((unsigned char*)arms);
		DrawUnitCylinder(numSegs);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 0.4f, 0);
		DrawJoint(numSegs);
	glPopMatrix();
}

static void DrawArmSegment(int numSegs, int seg)
{
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
		glTranslatef(-0.05f, 0, -0.05f);

		if(1 == seg) glScalef(0.1f, 3.0 * 0.190f, 0.1f);
		if(2 == seg) glScalef(0.1f, 3.0 * 0.178f, 0.1f);

		glColor3ubv((unsigned char*)arms);
		DrawUnitCylinder(numSegs);
	glPopMatrix();
	
	glPushMatrix();
		if(1 == seg) glTranslatef(0, 3.0 * 0.190f, 0);
		if(2 == seg) glTranslatef(0, 3.0 * 0.178f, 0);

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

	glTranslatef(0.0f, 0.4f, 0.0);
	glRotatef(shoulderAng, 0.0f, 0.0f, 90.0f);
	DrawArmSegment(64, 1);

	glTranslatef(0.0f, 3.0 * 0.190f, 0.0f);
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

	if(deltaMove) {
		computePos(deltaMove);
	}
	if(deltaAngle) {
		computeDir(deltaAngle);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt( 0.0f, 2.0f, 4.0f,	// Eye
			   0.0f, 0.5f, 0.0f,	// Center
			   0.0f, 1.0f, 0.0f		// UP
	);

	//cameraMove(cameraRate);
	//gluLookAt( fOrigin.x, fOrigin.y, fOrigin.z,
	//		   fFocus.x, fFocus.y, fFocus.z,
	//		   fUpVector.x, fUpVector.y, fUpVector.z
	//);
	

	/*gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);*/

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
	int flag = 0;

	for(i = 0; i < 10; i++) {
		servoRun(&arrServo[i]);

		if(arrServo[i].busi) flag = 1;
	}

	if(!flag && 0) {
		for(i = 0; i < 10; i++)
		arrServo[i].kSpeed = 0.9;

		arrServo[2].spAngle = -45;
		arrServo[3].spAngle = -45;
		arrServo[4].spAngle = -45;
		arrServo[5].spAngle = 0;
		arrServo[6].spAngle = -5555;
		cmd01(&arrServo[2], 5);
		
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

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.05f;
	z += deltaMove * lz * 0.01f;
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void MouseFunc(int button, int state, int x, int y)
{
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
		case GLUT_UP: /*cameraRate = 0;*/ middleButtonDown = false; break;
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

	float RelX = (newX - oldX) / glutGet(GLUT_WINDOW_WIDTH);
	float RelY = (newY - oldY) / glutGet(GLUT_WINDOW_HEIGHT);

	if(leftButtonDown) {
		switch(robotControl) {
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
