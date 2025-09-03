/*
 * Main.cpp
 *
 *  Created on: 2016/09/27
 *      Author: Takashi
 */

#include <windows.h>
#include <windef.h>
#include <tchar.h>
//#include "CWindowSizeDoc.h"
#include "CMainWindow.h"


CMainWindow	gMainWnd;




TCHAR szClassName[] = _T("WndRect");
TCHAR szTitleText[] = _T("WindowRect");



int WINAPI _tWinMain(
		HINSTANCE hInstance ,
		HINSTANCE hPrevInstance ,
		LPTSTR lpCmdLine ,
		int nCmdShow ) {
	//
//	HWND hWnd;
	MSG msg;
	WNDCLASS wndcls;
	//	::MessageBox(NULL , TEXT("PrevWindow") , TEXT("MessageBox") , MB_OK);
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = CMainWindow::WndProc;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hInstance = hInstance;
	wndcls.hIcon = NULL;
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = szClassName;
	if (!RegisterClass(&wndcls)) {
		return	FALSE;
	}
	//
	TCHAR	szTitleBuffer[256];
	_tcscpy( szTitleBuffer, szTitleText );
	gMainWnd.SetHInst( hInstance );
	gMainWnd.CreateMainWnd( szClassName, szTitleBuffer, lpCmdLine, nCmdShow );
//	gMainWnd.SetThisPtr();		//	ここから UpdateWindow() までを CMainWindow::CreateMainWnd() に
//	hWnd = CreateWindow( szClassName, szTitleText, WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, hInstance, NULL);
//	gMainWnd.SetHWnd(hWnd);
//	ShowWindow(hWnd, nCmdShow);
//	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//	::MessageBox(NULL , TEXT("AfterWindow") , TEXT("MessageBox") , MB_OK);
	return msg.wParam;
}






