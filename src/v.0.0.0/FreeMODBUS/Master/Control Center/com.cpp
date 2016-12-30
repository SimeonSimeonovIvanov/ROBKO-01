/*
		The^day^of^DooM

	 For AVR-LPC (v.0.0.7):

	Last Update:	20.08.2011 - Add DAC Support.
	Last Update:	08.05.2012
	Full Channge:	09.01.2013 - Add ModBUS Master LIB
	Last Update:	11.01.2013
	Last Update:	26.02.2013

		  For ROBKO 01:

	Full Channge:	19.05.2013 - Add ModBUS Suport
	Last Update:	21.05.2013
	Last Update:	30.05.2013
	Last Update:	01.06.2013
	Last Update:	08.06.2013
	Full Channge:	05.12.2013 - Bug Fix in CNC function G01
	Last Update:	05.12.2013
*/

#include "stdafx.h"
#include "resource.h"

#include "llist.h"
#include "modbus/modbus.h"
#include "modbus/modbus-rtu.h"
#include "modbus/modbus-tcp.h"

#include "gcode.h"
#include "ROBKO_01.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <process.h>
#include <windowsx.h>
#include <commctrl.h>

typedef struct {
	unsigned int step;
	int pos;
	//uint8_t state;
	unsigned char direction;
} BI_STEP_MOTOR, *BI_STEP_MOPTOR;

typedef struct {
	HWND hwnd;
	HINSTANCE hInst;

	HANDLE hComThread;

	BI_STEP_MOTOR arrMotor[6];

	MB mbMaster;
	unsigned char SlaveID;

	unsigned int fButton, idButton;
	
	unsigned int fButtonRun, idButtonRun;

	unsigned char inPort[15], outPort[15];

	// G Code:
	float a, dx, dy, dz, dP;
	int X1, Y1, Z1, P1, R1, G1;
	int X2, Y2, Z2, P2, R2, G2;

	LPLIST lpRootGProgramList, lpRootGProgramListState;
} MAIN_DATA, *LP_MAIN_DATA;

LP_MAIN_DATA mainDataGet( HWND hwnd );
void mainDataSet( HWND hwnd, LP_MAIN_DATA lpMainData );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI comThreadFunc( LPVOID lpParam );

LRESULT CALLBACK mainWndFunc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK AboutWndProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK tcpSetingsWndProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );

int APIENTRY WinMain
(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow
)
{
	InitCommonControls();

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)mainWndFunc);
}

LRESULT CALLBACK mainWndFunc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szBuffer[256];
	int wmId	= LOWORD(wParam);
	int wmEvent	= HIWORD(wParam);
	LP_MAIN_DATA lpMainData = mainDataGet(hwnd);

	switch(msg) {
	case WM_INITDIALOG: {
		int i;
		HWND hwndCombo;

		LoadIcon((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), (LPCTSTR)IDI_IDE);

		lpMainData = (LP_MAIN_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MAIN_DATA));
		if(NULL == lpMainData) {	
			EndDialog(hwnd, 0);
			return FALSE;
		}
		mainDataSet(hwnd, lpMainData);

		lpMainData->hwnd = hwnd;
		lpMainData->hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		//////////////////////////////////////////////////////////////////////////////
		lpMainData->SlaveID = 10;

		wsprintf(lpMainData->mbMaster.szIP, "192.168.0.11");
		wsprintf(lpMainData->mbMaster.szTcpPort, "502");

		serialInitDefDCB( &lpMainData->mbMaster.dcb );
		lpMainData->mbMaster.hwnd = lpMainData->hwnd;
		lpMainData->mbMaster.hCom = INVALID_HANDLE_VALUE;
		////////////////////////////////////////////////////////////////////////////////////////
		hwndCombo = GetDlgItem(hwnd, IDC_COMBO_SLAVE_ID);
		for(i = 1; i < 248; i++) {
			sprintf(szBuffer, "%d", i);
			ComboBox_AddString(hwndCombo, szBuffer);
		}
		ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_SLAVE_ID), lpMainData->SlaveID - 1);

		LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_MENU_MAIN));

		for(i = 1; i < 9; i++) {
			sprintf( szBuffer, "COM%d", i );
			ComboBox_AddString( GetDlgItem(hwnd, IDC_COMBO_PORT), szBuffer );
		}

		for( i = 0; i < 6; i++ ) {
			SendDlgItemMessage( hwnd, IDC_SPIN1 + i, UDM_SETRANGE, 0, MAKELPARAM( 1, 0 ) );
			SendDlgItemMessage( hwnd, IDC_SPIN1 + i, UDM_SETPOS, 0, MAKELPARAM( 0, 0 ) );
		}

		const HWND hDesktop = GetDesktopWindow( );
		RECT desktopRect, rect;

		GetWindowRect( hDesktop, &desktopRect );
		GetWindowRect( hwnd, &rect );

		MoveWindow(
			hwnd,
			( desktopRect.right - (rect.right - rect.left) ) / 2,
			( desktopRect.bottom - (rect.bottom - rect.top) ) / 2,
			rect.right - rect.left,
			rect.bottom - rect.top,
			TRUE
		);
	}
	 return TRUE;

	case WM_HSCROLL: {
	}
	 return TRUE;

	case WM_VSCROLL: {
		HWND hwndCtl		= (HWND)lParam;					// handle of control
		int nScrollCode		= (int)LOWORD(wParam);			// scroll bar value
		unsigned int nPos	= (unsigned int)HIWORD(wParam);	// scroll box position
		int i;
		
		for( i = 0; i < 6; i++ ) {
			HWND hwndTrack = GetDlgItem( hwnd, IDC_SPIN1 + i );

			if( hwndTrack == hwndCtl ) {
				lpMainData->idButtonRun = i;
				lpMainData->fButtonRun = nScrollCode;
				lpMainData->arrMotor[ i ].direction = nPos;
				break;
			}
		}
	}
	 return TRUE;

	case WM_COMMAND: {

		switch(wmEvent) {
		
		case CBN_SELCHANGE: {
			HWND hwndCombo = (HWND)lParam;

			switch(wmId) {
			case IDC_COMBO_SLAVE_ID:
				lpMainData->SlaveID = ComboBox_GetCurSel( hwndCombo ) + 1;
			 break;
			}
		}
		 return TRUE;

		case BN_CLICKED: {

			if( wmId >= IDC_BUTTON_MOTOR1_DIRECTION && wmId <= IDC_BUTTON_MOTOR6_DIRECTION) {
				lpMainData->arrMotor[ wmId - IDC_BUTTON_MOTOR1_DIRECTION ].direction ^= 1;
				return TRUE;
			}

			switch(wmId) {
			
			case IDC_BUTTON_EMS: {
				lpMainData->fButton = IDC_BUTTON_EMS;

				Button_Enable( GetDlgItem( lpMainData->hwnd, IDC_BUTTON_RUN_G_CODE ), TRUE );
				Button_Enable( GetDlgItem( lpMainData->hwnd, IDC_BUTTON_SET_IK_G01_L ), TRUE );
			}
			 return TRUE;

			case ID_ROBKO01_SET_ZERO_POSITION: {
				if( !lpMainData->fButton && lpMainData->mbMaster.enable ) {
					lpMainData->fButton = ID_ROBKO01_SET_ZERO_POSITION;
					return TRUE;
				}
			}
			 break;

			case ID_ROBKO01_SAVE_POS_TO_EEPROM: {
				if( !lpMainData->fButton && lpMainData->mbMaster.enable ) {
					lpMainData->fButton = ID_ROBKO01_SAVE_POS_TO_EEPROM;
					return TRUE;
				}
			}
			 break;

			case IDC_BUTTON_SET_IK_G01:
			case IDC_BUTTON_SET_IK_G01_L: {
				if( !lpMainData->fButton && lpMainData->mbMaster.enable ) {

					Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_X ), szBuffer, sizeof(szBuffer) );
					lpMainData->X2 = atoi( szBuffer );
					Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_Y ), szBuffer, sizeof(szBuffer) );
					lpMainData->Y2 = atoi( szBuffer );
					Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_Z ), szBuffer, sizeof(szBuffer) );
					lpMainData->Z2 = atoi( szBuffer );

					Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_P_GRIPER ), szBuffer, sizeof(szBuffer) );
					lpMainData->P2 = atoi( szBuffer );
					Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_R_GRIPER ), szBuffer, sizeof(szBuffer) );
					lpMainData->R2 = atoi( szBuffer );

					Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_GRIPER ), szBuffer, sizeof(szBuffer) );
					lpMainData->G2 = atoi( szBuffer );

					switch(wmId) {
					case IDC_BUTTON_SET_IK_G01:
						lpMainData->fButton = IDC_BUTTON_SET_IK_G01;
					 break;

					case IDC_BUTTON_SET_IK_G01_L:
						lpMainData->fButton = IDC_BUTTON_SET_IK_G01_L;
					 break;
					}

					return TRUE;
				}
			}
			 break;

			case IDC_BUTTON_RUN_G_CODE: {
				if( !lpMainData->fButton ) {

					char *lpBuffer;
					HWND hwndEdit;
					int lineCount;

					hwndEdit = GetDlgItem( hwnd, IDC_EDIT_G_CODE );
					lineCount = Edit_GetLineCount( hwndEdit );

					if( NULL != ( lpBuffer = (char*)malloc( sizeof(char) * 65000 ) ) ) {

						Edit_GetText( hwndEdit, lpBuffer, 65000 );

						gcode_loadstr( lpBuffer, &lpMainData->lpRootGProgramList );
						lpMainData->lpRootGProgramListState = lpMainData->lpRootGProgramList;
						free( lpBuffer );

						LPLIST temp = lpMainData->lpRootGProgramListState;
						while( temp ) {
							gcode *lpGc = (gcode*)temp->lpData;
							temp = temp->next;
						}

						if( lpMainData->mbMaster.enable ) {
							Button_Enable( GetDlgItem( lpMainData->hwnd, IDC_BUTTON_RUN_G_CODE ), FALSE );
							lpMainData->fButton = IDC_BUTTON_RUN_G_CODE;
						}
					}

				}
			}
			 break;

			case IDC_BUTTON_SET_FK_GET: {
				if( !lpMainData->fButton && lpMainData->mbMaster.enable ) {
					lpMainData->fButton = IDC_BUTTON_SET_FK_GET;
					return TRUE;
				}
			}
			 break;
			
			case IDC_BUTTON_SET_FK_SET: {				
				float X, Y, Z, R, P, G;
				int i, arrM[6];

				Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_X ), szBuffer, sizeof(szBuffer) );
				X = (float)atoi( szBuffer );
				Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_Y ), szBuffer, sizeof(szBuffer) );
				Y = (float)atoi( szBuffer );
				Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_Z ), szBuffer, sizeof(szBuffer) );
				Z = (float)atoi( szBuffer );

				Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_P_GRIPER ), szBuffer, sizeof(szBuffer) );
				P = (float)atoi( szBuffer );
				Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_R_GRIPER ), szBuffer, sizeof(szBuffer) );
				R = (float)atoi( szBuffer );

				Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_GRIPER ), szBuffer, sizeof(szBuffer) );
				G = (float)atoi( szBuffer );

				Robko01_Inverse_Kinematics//_fig4_4
				(
					X, Y, Z, P, R, G, 0.0f, arrM
				);

				for( i = 0; i < 6; i++ ) {
					int temp;

					lpMainData->arrMotor[i].pos = arrM[i];

					if( arrM[i] >= 0 ) {
						lpMainData->arrMotor[i].direction = 0;
						temp = +arrM[i];
					} else {
						lpMainData->arrMotor[i].direction = 1;
						temp = -arrM[i];
					}

					sprintf( szBuffer, "%d", temp );
					Edit_SetText( GetDlgItem( hwnd, IDC_EDIT_MOTOR1 + i ), szBuffer );
				}
			}
			 return TRUE;

			case IDC_BUTTON_RUN_ALL: {

				if( !lpMainData->fButton && lpMainData->mbMaster.enable ) {

					for( int i = 0; i < 6; i++ ) {
						Edit_GetText( GetDlgItem( hwnd, IDC_EDIT_MOTOR1 + i ), szBuffer, sizeof(szBuffer) );
						lpMainData->arrMotor[i].step = atoi( szBuffer );
					}

					lpMainData->fButton = IDC_BUTTON_RUN_ALL;

					return TRUE;
				}
			}
			 break;

			case IDM_CONNECTION_SETINGS:
				if( INVALID_HANDLE_VALUE != lpMainData->mbMaster.hCom ) {
					COMMCONFIG CC;

					sprintf(szBuffer, "COM%d", lpMainData->mbMaster.uiComIndex);

					CC.dcb = lpMainData->mbMaster.dcb;
					CommConfigDialog( szBuffer, lpMainData->hwnd, &CC );
					lpMainData->mbMaster.dcb.Parity = CC.dcb.Parity;
					lpMainData->mbMaster.dcb.BaudRate = CC.dcb.BaudRate;
					lpMainData->mbMaster.dcb.StopBits = CC.dcb.StopBits;
					SetCommState(lpMainData->mbMaster.hCom, &lpMainData->mbMaster.dcb);

					return TRUE;
				}
			 break;

			case IDM_CONNECTION_COMSELECT_COM1: case IDM_CONNECTION_COMSELECT_COM2:
			case IDM_CONNECTION_COMSELECT_COM3: case IDM_CONNECTION_COMSELECT_COM4:
			case IDM_CONNECTION_COMSELECT_COM5: case IDM_CONNECTION_COMSELECT_COM6:
			case IDM_CONNECTION_COMSELECT_COM7: case IDM_CONNECTION_COMSELECT_COM8: {
				HMENU hMenu = GetMenu(hwnd);
				unsigned int com = 1 + (wmId - IDM_CONNECTION_COMSELECT_COM1);

				SendMessage(hwnd, WM_COMMAND, BN_CLICKED<<16 | IDM_DISCONNECT, 0);

				if( mbMasterInit(&lpMainData->mbMaster, 1) ) {
					return FALSE;
				}

				if( !mbRTUMasterConnect(&lpMainData->mbMaster, com) ) {
					lpMainData->mbMaster.uiComIndex = com;
					////////////////////////////////////////////////////////////////////////////////////////////
					EnableMenuItem(hMenu, IDM_CONNECTION_SETINGS, MF_ENABLED);
					EnableMenuItem(hMenu, IDM_CONNECTION_DISCONNECT, MF_ENABLED);

					CheckMenuRadioItem( hMenu, IDM_CONNECTION_COMSELECT_COM1,
										IDM_CONNECTION_COMSELECT_COM8,
										wmId, MF_BYCOMMAND | MF_CHECKED
					);
					////////////////////////////////////////////////////////////////////////////////////////////
					DWORD dwThreadId;
					lpMainData->hComThread = CreateThread(NULL, 4096, comThreadFunc, lpMainData, 0, &dwThreadId);
					SetThreadPriority(lpMainData->hComThread,	THREAD_PRIORITY_TIME_CRITICAL);
					////////////////////////////////////////////////////////////////////////////////////////////
					return TRUE;
				}
			}
			 break;

			case IDM_CONNECTION_TCP_IP: {
//				HMENU hMenu = GetMenu(hwnd);

				SendMessage( hwnd, WM_COMMAND, BN_CLICKED<<16 | IDM_DISCONNECT, 0 );

				if( mbMasterInit(&lpMainData->mbMaster, 2) ) {
					return FALSE;
				}

				switch(DialogBox(lpMainData->hInst, MAKEINTRESOURCE(IDD_TCP_SETINGS), hwnd, (DLGPROC)tcpSetingsWndProc)) {
				case 0:
				 return false;
				case 1:
				 break;
				case 2:
				 return false;

				default: return false;
				}

				lpMainData->fButton = IDM_CONNECTION_TCP_IP;

				DWORD dwThreadId;
				lpMainData->hComThread = CreateThread(NULL, 4096, comThreadFunc, lpMainData, 0, &dwThreadId);
				SetThreadPriority(lpMainData->hComThread,	THREAD_PRIORITY_TIME_CRITICAL);
				////////////////////////////////////////////////////////////////////////////////////////////
			}
			 return TRUE;

			case IDM_DISCONNECT: {
				HMENU hMenu = GetMenu(hwnd);

				EnableMenuItem(hMenu, IDM_CONNECTION_SETINGS, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_CONNECTION_DISCONNECT, MF_GRAYED);

				CheckMenuRadioItem( hMenu, IDM_CONNECTION_COMSELECT_COM1,
									IDM_CONNECTION_COMSELECT_TCP_IP,
									wmId, MF_BYCOMMAND | MF_UNCHECKED
				);

				if( INVALID_HANDLE_VALUE != lpMainData->hComThread ) {
					TerminateThread(lpMainData->hComThread, 0);
					CloseHandle(lpMainData->hComThread);
					lpMainData->hComThread = INVALID_HANDLE_VALUE;
				}

				mbMasterDisconnect( &lpMainData->mbMaster );
			}
			 return TRUE;
			// --------------------------------------------------------------------------------------------------------------------
			case IDCANCEL:
				if(NULL != lpMainData) {

					if( INVALID_HANDLE_VALUE != lpMainData->hComThread ) {
						TerminateThread( lpMainData->hComThread, 0 );
						CloseHandle( lpMainData->hComThread );
						lpMainData->hComThread = INVALID_HANDLE_VALUE;
					}

					mbMasterDisconnect( &lpMainData->mbMaster );

					HeapFree(GetProcessHeap (), GWL_USERDATA, lpMainData);
				}

				EndDialog(hwnd, 0);
			 return TRUE;
			case ID_HELP_ABOUT:
				DialogBox(lpMainData->hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, (DLGPROC)AboutWndProc);
			 return TRUE;
			}

		}
		 break;
		} // END: BN_CLICKED

	} // END: WM_COMMAND
	 return TRUE;
	}

	return FALSE;
}

float fmod(float value)
{
	if( value >= 0.0f ) return value;
	return -value;
}

DWORD WINAPI comThreadFunc(LPVOID lpParam)
{
	LP_MAIN_DATA lpMainData = (LP_MAIN_DATA)lpParam;
	char flag = 0, szBuffer[256];
	unsigned int i = 0;

	unsigned int uiResultReadPos;
	int flagAllMotorsNotMove = 0;
	int flagDrawLine = 0;
	float arrPos[6]; // [ X mm. : Y mm. : Z mm. : P deg. : R deg. : G deg. ]
	float delta;

	while( 1 ) {
		if( 2 == lpMainData->mbMaster.ascii_or_rtu_or_tcp ) {
			char fReconect = 0;
			
			if( 1 != send( lpMainData->mbMaster.tcpSocket, " ", 1, 0 ) ) {
				fReconect = 1;
			}

			if( fReconect || IDM_CONNECTION_TCP_IP == lpMainData->fButton  ) {
				if( mbTCPMasterConnect( &lpMainData->mbMaster, lpMainData->mbMaster.szIP, atoi(lpMainData->mbMaster.szTcpPort)) ) {
					Sleep(1);
					continue;
				} else {
					HMENU hMenu = GetMenu( lpMainData->hwnd );

					EnableMenuItem( hMenu, IDM_CONNECTION_DISCONNECT, MF_ENABLED );
					CheckMenuRadioItem( hMenu,
										IDM_CONNECTION_COMSELECT_COM1,
										IDM_CONNECTION_COMSELECT_TCP_IP,
										IDM_CONNECTION_COMSELECT_TCP_IP,
										MF_BYCOMMAND | MF_CHECKED
					);

					lpMainData->fButton = 0;
				}
			}
		}

		if( IDC_BUTTON_EMS == lpMainData->fButton ) {
			unsigned char arrOut[12] = { 0 };
			unsigned int arrTemp[13] = { 0 };
			
			arrTemp[6] = 3;
			
			i = 0;
			while( ++i < 4 && mbWriteMultipleRegisters( &lpMainData->mbMaster, lpMainData->SlaveID, 0, 13, arrTemp ) );
			//i = 0;
			//while( ++i < 4 && mbWriteMultipleCoils( &lpMainData->mbMaster, lpMainData->SlaveID, 6, 12, arrOut ) );

			freeList( &lpMainData->lpRootGProgramList );

			lpMainData->fButton = 0;
			flagDrawLine = 0;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( !mdReadDiscreteInputs( &lpMainData->mbMaster, lpMainData->SlaveID, (char*)lpMainData->inPort, 0, 3) ) {
			sprintf( szBuffer, "POWER is %s", !lpMainData->inPort[0] ? "GOOD":"BAD" );
			Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_STATIC_POWER ), szBuffer );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned int arr_new[13] = { 0 };

		uiResultReadPos = mbReadInputRegister( &lpMainData->mbMaster, lpMainData->SlaveID, 0, 13, arr_new );
		if( !uiResultReadPos ) {
			int arrMotorPos[6];

			if( !( arr_new[5] | arr_new[4] | arr_new[3] | arr_new[2] | arr_new[1] | arr_new[0] ) ) {
				flagAllMotorsNotMove = 1;
			} else {
				flagAllMotorsNotMove = 0;
			}

			for( i = 0; i < 6; i++ ) {
				if( arr_new[i] ) {
					flag = 1;
				}

				if( 0x8000 & arr_new[6 + i] ) {
					arr_new[6 + i] = ( 0x7FFF & arr_new[6 + i] ) - 32768;
				}

				arrMotorPos[i] = (int)arr_new[6 + i];

				sprintf( szBuffer, "%d", arrMotorPos[ i ] );
				Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_BUTTON_RUN_MOTOR1 + i ), szBuffer );
			}

			if( flag ) {
				for( i = 0; i < 6; i++ ) {
					sprintf( szBuffer, "%d", arr_new[i] );
					Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_EDIT_MOTOR1 + i ), szBuffer );
				}
				--flag;
			}

			Robko01_Forward_Kinematics_From_Motor_Pos( arrMotorPos, arrPos, 0.0f );

			sprintf( szBuffer, "%4.1f", arrPos[ 0 ] );
			Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_STATIC_FK_X ), szBuffer );
			sprintf( szBuffer, "%4.1f", arrPos[ 1 ] );
			Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_STATIC_FK_Y ), szBuffer );
			sprintf( szBuffer, "%4.1f", arrPos[ 2 ] );
			Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_STATIC_FK_Z ), szBuffer );

			sprintf( szBuffer, "%4.1f", arrPos[ 3 ] );
			Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_STATIC_FK_P ), szBuffer );
			sprintf( szBuffer, "%4.1f", arrPos[ 4 ] );
			Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_STATIC_FK_R ), szBuffer );
			sprintf( szBuffer, "%4.1f", arrPos[ 5 ] );
			Edit_SetText( GetDlgItem( lpMainData->hwnd, IDC_STATIC_FK_G ), szBuffer );
		} else {
			flagAllMotorsNotMove = -1;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( !mbReadCoils( &lpMainData->mbMaster, lpMainData->SlaveID, (char*)lpMainData->outPort, 0, 12) ) {

			static const char *szMotorDirection[][2] = {
				{ "+90", "-90" },
				{ "Up", "Down" },
				{ "Up", "Down" },
				{ "Up", "Down" },
				{ "Up", "Down" },
				{ "Open", "Close" }
			};

			for( i = 0; i < 6; i++ ) {
				sprintf( szBuffer, "%s", szMotorDirection[ i ][ lpMainData->outPort[ i ] ] );
				Button_SetText(GetDlgItem(lpMainData->hwnd, IDC_BUTTON_MOTOR1_DIRECTION + i), szBuffer);

				lpMainData->outPort[ i ] = !lpMainData->arrMotor[ i ].direction;
			}

			switch( lpMainData->fButtonRun ) {
			case 4:
				lpMainData->outPort[ 6 + lpMainData->idButtonRun ] = 1;
				//lpMainData->fButtonRun = 0;
			 break;
			case 8:
				lpMainData->outPort[ 6 + lpMainData->idButtonRun ] = 0;
				//lpMainData->fButtonRun = 0;
			 break;
			}

			mbWriteMultipleCoils( &lpMainData->mbMaster, lpMainData->SlaveID, 0, 12, lpMainData->outPort );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( ID_ROBKO01_SET_ZERO_POSITION == lpMainData->fButton ) {
			mbWriteSingleRegister( &lpMainData->mbMaster, lpMainData->SlaveID, 6, 33 );
			lpMainData->fButton = 0;
		}

		if( ID_ROBKO01_SAVE_POS_TO_EEPROM == lpMainData->fButton ) {
			mbWriteSingleRegister( &lpMainData->mbMaster, lpMainData->SlaveID, 6, 5 );
			lpMainData->fButton = 0;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( IDC_BUTTON_SET_FK_GET == lpMainData->fButton ) {
			int arrSP[7];

			arrSP[0] = 2;
			for( i = 0; i < 6; i++ ) {
				arrSP[1 + i] = lpMainData->arrMotor[i].pos;
			}

			mbWriteMultipleRegisters( &lpMainData->mbMaster, lpMainData->SlaveID, 6, 7, (unsigned int*)arrSP );

			lpMainData->fButton = 0;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( IDC_BUTTON_RUN_ALL == lpMainData->fButton ) {
			unsigned int arrSP[7];

			arrSP[6] = 1;
			for( i = 0; i < 6; i++ ) {
				arrSP[i] = lpMainData->arrMotor[i].step;
			}

			mbWriteMultipleRegisters( &lpMainData->mbMaster, lpMainData->SlaveID, 0, 7, arrSP );

			lpMainData->fButton = 0;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( IDC_BUTTON_RUN_G_CODE == lpMainData->fButton ) {
			if( !uiResultReadPos && !( 0x00ff & arr_new[12] ) ) {

				if( lpMainData->lpRootGProgramListState ) {
					gcode gc = *(gcode*)lpMainData->lpRootGProgramListState->lpData;
					int arrPosTemp[7];

					arrPosTemp[0] = 0;
					for( i = 0; i < 6; i++ ) {
						arrPosTemp[ 1 + i ] = (int)arrPos[ i ];
					}

					if( 1 & gc.axesFlag ) {
						arrPosTemp[0] |= 0x0100;
						arrPosTemp[1] = (int)gc.X;
					}

					if( 2 & gc.axesFlag ) {
						arrPosTemp[0] |= 0x0200;
						arrPosTemp[2] = (int)gc.Y;
					}

					if( 4 & gc.axesFlag ) {
						arrPosTemp[0] |= 0x0400;
						arrPosTemp[3] = (int)gc.Z;
					}

					if( 8 & gc.axesFlag ) {
						arrPosTemp[0] |= 0x0800;
						arrPosTemp[4] = (int)gc.U;
					}

					if( 16 & gc.axesFlag ) {
						arrPosTemp[0] |= 0x1000;
						arrPosTemp[5] = (int)gc.V;
					}
					if( 32 & gc.axesFlag ) {
						arrPosTemp[0] |= 0x2000;
						arrPosTemp[6] = (int)gc.W;
					}

					switch( gc.G ) {
					case 0: case 1:
						switch( gc.G ) {
						case 0: {
							arrPosTemp[0] = 2;

							Robko01_Inverse_Kinematics//_fig4_4
							(
								(float)arrPosTemp[1], (float)arrPosTemp[2], (float)arrPosTemp[3],
								(float)arrPosTemp[4], (float)arrPosTemp[5], (float)arrPosTemp[6],
								0.0f, &arrPosTemp[1]
							);
						}
						 break;

						case 1: {
							arrPosTemp[0] |= 4;
						}
						 break;
					}
					 break;

					default:
						lpMainData->fButton = 0;
					 break;
					}

					if( !mbWriteMultipleRegisters( &lpMainData->mbMaster, lpMainData->SlaveID, 6, 7, (unsigned int*)arrPosTemp ) ) {
						lpMainData->lpRootGProgramListState = lpMainData->lpRootGProgramListState->next;
					}
				} else {
					Button_Enable( GetDlgItem( lpMainData->hwnd, IDC_BUTTON_RUN_G_CODE ), TRUE );

					freeList( &lpMainData->lpRootGProgramList );

					lpMainData->fButton = 0;
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( IDC_BUTTON_SET_IK_G01_L == lpMainData->fButton && ( 1 == flagAllMotorsNotMove ) ) {
			int temp[7] = { 0 };

			temp[0] = 4;
			temp[1] = lpMainData->X2;
			temp[2] = lpMainData->Y2;
			temp[3] = lpMainData->Z2;
			temp[4] = lpMainData->P2;
			temp[5] = lpMainData->R2;
			temp[6] = lpMainData->G2;

			if( !mbWriteMultipleRegisters( &lpMainData->mbMaster, lpMainData->SlaveID, 6, 7, (unsigned int*)temp ) ) {
				lpMainData->fButton = 0;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( IDC_BUTTON_SET_IK_G01 == lpMainData->fButton && !uiResultReadPos ) {
			Button_Enable( GetDlgItem( lpMainData->hwnd, IDC_BUTTON_SET_IK_G01 ), FALSE );

			lpMainData->fButton = 0;
			lpMainData->a = 0.0f;
			flagDrawLine = 1;
		}

		if(	flagDrawLine		&&
			!uiResultReadPos	&&
			1 == flagAllMotorsNotMove
		) {
			lpMainData->X1 = (int)arrPos[0];
			lpMainData->Y1 = (int)arrPos[1];
			lpMainData->Z1 = (int)arrPos[2];
			lpMainData->P1 = (int)arrPos[3];
			lpMainData->R1 = (int)arrPos[4];
			lpMainData->G1 = (int)arrPos[5];

			lpMainData->dx = (float)( lpMainData->X2 - lpMainData->X1 );
			lpMainData->dy = (float)( lpMainData->Y2 - lpMainData->Y1 );
			lpMainData->dz = (float)( lpMainData->Z2 - lpMainData->Z1 );
			lpMainData->dP = (float)( lpMainData->P2 - lpMainData->P1 );

			/*if(
				X1 == X2 && Y1 == Y2 && Z1 == Z2 &&
				P1 == P2 && R1 == R2 &&
				G1 == G2
			) {
				uiRegHolding[6]	= 0;
				flagDrawLine = 0;
				a = 0.0f;
				break;
			}*/

			float c = (float)sqrt( pow( lpMainData->dx, 2 ) + pow( lpMainData->dy, 2 ) + pow( lpMainData->dz, 2 ) );

			if( !c ) {
				c = 400;
			}

			delta = 1 / c;

			if( lpMainData->a <= ( 1.0f + 0.001f ) ) {
				float Xn = lpMainData->X1 + lpMainData->a * lpMainData->dx;
				float Yn = lpMainData->Y1 + lpMainData->a * lpMainData->dy;
				float Zn = lpMainData->Z1 + lpMainData->a * lpMainData->dz;
				float Pn = lpMainData->P1 + lpMainData->a * lpMainData->dP;

				int temp[7];

				Robko01_Inverse_Kinematics//_fig4_4
				(
					Xn, Yn, Zn,
					(float)Pn,
					(float)lpMainData->R2,
					(float)lpMainData->G2,
					0.0f,
					&temp[1]
				);

				temp[0] = 2;
				if( !mbWriteMultipleRegisters( &lpMainData->mbMaster, lpMainData->SlaveID, 6, 7, (unsigned int*)temp ) ) {
					lpMainData->a += delta;//0.1f;
				}
			} else {
				Button_Enable( GetDlgItem( lpMainData->hwnd, IDC_BUTTON_SET_IK_G01 ), TRUE );
				flagDrawLine = 0;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Sleep(1);
	}

	mbMasterDisconnect( &lpMainData->mbMaster );

	return false;
}

LP_MAIN_DATA mainDataGet(HWND hwnd)
{
	return (LP_MAIN_DATA)GetWindowLong(hwnd, GWL_USERDATA);
}

void mainDataSet(HWND hwnd, LP_MAIN_DATA lpMainData)
{
	SetWindowLong(hwnd, GWL_USERDATA, (LONG)lpMainData);
}

LRESULT CALLBACK tcpSetingsWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LP_MAIN_DATA lpMainData = mainDataGet(GetParent(hDlg));

	switch(message){
	case WM_INITDIALOG:
		Edit_SetText(GetDlgItem(hDlg, IDC_IPADDRESS), lpMainData->mbMaster.szIP);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDIT_TCP_PORT), lpMainData->mbMaster.szTcpPort);
	 return TRUE;

	case WM_COMMAND:
		
		switch(LOWORD(wParam)) {
		case IDOK: {
			if(NULL != lpMainData) {
				Edit_GetText(GetDlgItem(hDlg, IDC_IPADDRESS), lpMainData->mbMaster.szIP, 16);
				Edit_GetText(GetDlgItem(hDlg, IDC_EDIT_TCP_PORT), lpMainData->mbMaster.szTcpPort, 12);

				EndDialog(hDlg, 1);
			} else {
				EndDialog(hDlg, 0);
			}
		}
		 break;

		case IDCANCEL: EndDialog(hDlg, 2); break;
		}

	 return TRUE;
	}
    return FALSE;
}

LRESULT CALLBACK AboutWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case WM_INITDIALOG: return TRUE;

	case WM_COMMAND:
		if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
    return FALSE;
}