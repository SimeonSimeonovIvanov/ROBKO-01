// OpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include <gl/gl.h>
#include <gl/glut.h>
#include <windows.h>

#include "arm.h"
#include "camera.h"

extern SERVO arrServo[];

BOOL CALLBACK MainDialogProc(
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
);

int WINAPI WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int iCmdShow
)
{
	HWND hWnd;

	glutInit(&iCmdShow, &lpCmdLine);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutGetWindow();
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("The Robot Arm");

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	hWnd = CreateDialog( hInstance, MAKEINTRESOURCE(IDD_MAIN), 0, MainDialogProc );
	ShowWindow(hWnd, 1);

//	glutCreateSubWindow((int)hWnd, 0, 0, 512, 512);
//	cameraSetup(2, 2, 2, 1, 1, 1, 1, 2, 1);
	
	glutIdleFunc(myIdle);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);

	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutKeyboardFunc(KeyboardFunc);

	armInitServo();

	glutMainLoop();

	return 0;
}

BOOL CALLBACK MainDialogProc(
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,		// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam	// second message parameter
)
{
	//HDC hdc;
	//RECT rt;
	//PAINTSTRUCT ps;
	int wmId, wmEvent;

	switch (uMsg) {
	case WM_CREATE:
	 return 0;
	
	case WM_CLOSE:
		PostQuitMessage (0);
	 return 0;

	case WM_DESTROY:
	 return 0;

	case WM_COMMAND: {
		wmId	= LOWORD(wParam);
		wmEvent	= HIWORD(wParam);

		if(BN_CLICKED == wmEvent) {
			switch(wParam) {

			case IDC_BUTTON1:
				arrServo[2].bOn = true;
				arrServo[2].spAngle -= 5;
			 return 0;

			case IDC_BUTTON2:
				arrServo[2].bOn = true;
				arrServo[2].spAngle += 5;
			 return 0;

			case IDOK:
					//arrServo[0].spAngle = -0;
	//arrServo[1].spAngle = 0;

	arrServo[2].spAngle = -100;	// Въртене около централна ос.
	arrServo[3].spAngle = -45;	// Рамо				\ 
	arrServo[4].spAngle = -20;	// Лакет			 > - Взаимно свързани оси
	arrServo[5].spAngle = -90;	// Китка: горе/долу	/ 
	arrServo[6].spAngle = 45;	// Китка: завъртане
	arrServo[7].spAngle = 130;	// Пръсти: отваряне/затваряне
	
	cmd01( &arrServo[2], 6 );

	//arrServo[0].bOn = true;
	//arrServo[1].bOn = true;

	arrServo[2].bOn = true;
	arrServo[3].bOn = true;
	arrServo[4].bOn = true;
	arrServo[5].bOn = true;
	arrServo[6].bOn = true;
	arrServo[7].bOn = true;

			 return 0;

			case IDCANCEL:
				PostQuitMessage(0);
			 return 0;
			}

			return 0;
		}

	}
	 return 0;
	
	case WM_KEYDOWN: {
		switch(wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
		 return 0;
		}
	}
	 return 0;

	}

	return 0;//DefWindowProc(hwndDlg, uMsg, wParam, lParam);
}