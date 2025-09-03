/*
 * GUI_Make.cpp
 *
 *  Created on: 2016/10/04
 *      Author: Takashi
 */



#include <windows.h>
#include <windef.h>
#include <tchar.h>
#include "Resource.h"
#include "mydef.h"
#include "CMainWindow.h"
#include "CSubWindow.h"





LRESULT	CMainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
{	//	メインウィンドウ作成時に呼び出される。
	RECT	sWndRc;
	DWORD	dwStyle;
	LONG	nHeight;
	::GetClientRect( m_hMainWnd, &sWndRc );
	nHeight = max2(20, sWndRc.bottom/2);

	dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER;// | WS_HSCROLL | WS_VSCROLL;
//	dwStyle |= ES_AUTOHSCROLL | ES_AUTOVSCROLL;
//	dwStyle |= ES_LEFT | ES_MULTILINE;
	m_hEdtWndRect = CreateWindow(
			TEXT("EDIT") , NULL , dwStyle ,
			0 , 25 , 300 , 20 , m_hMainWnd , (HMENU)IDC_EDTWNDRECT ,
			m_hInstance , NULL
		);

	dwStyle = BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE;
	dwStyle |= WS_GROUP;	//	グループ開始の合図。
	m_hOptWndLTRB = CreateWindow(
			TEXT("BUTTON") , _T("L,T,R,B") , dwStyle ,
			0 , 0 , 150 , 20 , m_hMainWnd , (HMENU)IDC_OPTWNDLTRB ,
			m_hInstance , NULL
		);

	dwStyle = BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE;
	m_hOptWndLTWH = CreateWindow(
			TEXT("BUTTON") , _T("L,T,W,H") , dwStyle ,
			160 , 0 , 150 , 20 , m_hMainWnd , (HMENU)IDC_OPTWNDLTWH ,
			m_hInstance , NULL
		);
	m_bChkLTWH = true;
	::CheckRadioButton( m_hMainWnd, IDC_OPTWNDLTRB, IDC_OPTWNDLTWH, IDC_OPTWNDLTWH );

//	dwStyle = BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE;
//	dwStyle |= WS_GROUP;	//	グループ開始（終了）の合図。
//	m_hBtnWndGet = CreateWindow(
//			TEXT("BUTTON") , _T("SubWnd") , dwStyle ,
//			100 , 50 , 100 , 25 , m_hMainWnd , (HMENU)IDC_BTNWNDGET ,
//			m_hInstance , NULL
//		);

	return 0;
}


LRESULT	CSubWindow::OnCreate(WPARAM wParam, LPARAM lParam)
{	//	メインウィンドウ作成時に呼び出される。
	RECT	sWndRc;
	DWORD	dwStyle;
	LONG	nHeight;
	::GetClientRect( m_hMainWnd, &sWndRc );
	nHeight = max2(20, sWndRc.bottom/2);

	dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER;// | WS_HSCROLL | WS_VSCROLL;
//	dwStyle |= ES_AUTOHSCROLL | ES_AUTOVSCROLL;
//	dwStyle |= ES_LEFT | ES_MULTILINE;
	m_hEdtWndRect = CreateWindow(
			TEXT("EDIT") , NULL , dwStyle ,
			0 , 25 , 300 , 20 , m_hMainWnd , (HMENU)IDC_EDTWNDRECT ,
			m_hInstance , NULL
		);

	dwStyle = BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE;
	dwStyle |= WS_GROUP;	//	グループ開始の合図。
	m_hOptWndLTRB = CreateWindow(
			TEXT("BUTTON") , _T("L,T,R,B") , dwStyle ,
			0 , 0 , 150 , 20 , m_hMainWnd , (HMENU)IDC_OPTWNDLTRB ,
			m_hInstance , NULL
		);

	dwStyle = BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE;
	m_hOptWndLTWH = CreateWindow(
			TEXT("BUTTON") , _T("L,T,W,H") , dwStyle ,
			160 , 0 , 150 , 20 , m_hMainWnd , (HMENU)IDC_OPTWNDLTWH ,
			m_hInstance , NULL
		);
	m_bChkLTWH = true;
	::CheckRadioButton( m_hMainWnd, IDC_OPTWNDLTRB, IDC_OPTWNDLTWH, IDC_OPTWNDLTWH );

	dwStyle = BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE;
	dwStyle |= WS_GROUP;	//	グループ開始（終了）の合図。
	m_hBtnWndGet = CreateWindow(
			TEXT("BUTTON") , _T("SubWnd") , dwStyle ,
			100 , 50 , 100 , 25 , m_hMainWnd , (HMENU)IDC_BTNWNDGET ,
			m_hInstance , NULL
		);
	return 0;
}




BOOL	CMainWindow::OnCommand(UINT nId, UINT nEvent, LPARAM lParam)
{
//	TCHAR	*pBuff;
//	long	nTextLen;
	BOOL	bRetVal = FALSE;
	switch (nId){	//	選択されたメニューの解析:
//	case IDC_EDTTEXT:
	case IDC_EDTWNDRECT:		//	テキストボックスが修正された場合
		if (m_nTimerMode == eTimerModeGetWndRc)	return	bRetVal;	//	ウィンドウ枠取得中は無視する。
		if (nEvent == EN_UPDATE  &&  !m_bChgEdit){
//			OnUpdate_Edit(nId - IDC_EDTTEXT);
			if (m_nTimerMode == eTimerModeTextChg)	::KillTimer( m_hMainWnd, TIMER_ID );
			::SetTimer( m_hMainWnd, TIMER_ID, TIMER_ELAPSE, NULL );
			m_nTimerMode = eTimerModeTextChg;
			m_bDelayTimer = true;
		}
		break;

	case IDC_OPTWNDLTRB:	//	オプションボタンの選択が変わった時
	case IDC_OPTWNDLTWH:
		m_bChkLTWH = (nId == IDC_OPTWNDLTWH);
		GetWndRectToEdit();
		break;

//	case IDC_BTNWNDGET:		//	ウィンドウ枠の取得ボタン
//		if (m_nTimerMode != eTimerModeUnDef)	::KillTimer( m_hMainWnd, TIMER_ID );
//		::SetTimer( m_hMainWnd, TIMER_ID, TIMER_ELAPSE * 6, NULL );
//		m_nTimerMode = eTimerModeGetWndRc;
//		m_bDelayTimer = true;
//		CreateSubWnd();
//		break;
	}
	return	bRetVal;
}





void	CMainWindow::EvtTimer(WPARAM wParam)
{	//	タイマメッセージの処理部分
	if (wParam != TIMER_ID)	return;
	::KillTimer( m_hMainWnd, TIMER_ID );
	switch (m_nTimerMode){
	case eTimerModeTextChg:		//
		//::MessageBox(NULL , TEXT("EvtTimer TextChg") , TEXT("MessageBox") , MB_OK);
		GetEditToWndRect();
		break;
	case eTimerModeGetWndRc:	//
		//::MessageBox(NULL , TEXT("EvtTimer WndRc") , TEXT("MessageBox") , MB_OK);
		GetAnotherWndRect();
		break;
	}
	m_nTimerMode = eTimerModeUnDef;
}






BOOL	CSubWindow::OnCommand(UINT nId, UINT nEvent, LPARAM lParam)
{
//	TCHAR	*pBuff;
//	long	nTextLen;
	BOOL	bRetVal = FALSE;
	switch (nId){	//	選択されたメニューの解析:
//	case IDC_EDTTEXT:
	case IDC_EDTWNDRECT:		//	テキストボックスが修正された場合
		if (m_nTimerMode == eTimerModeGetWndRc)	return	bRetVal;	//	ウィンドウ枠取得中は無視する。
		if (nEvent == EN_UPDATE  &&  !m_bChgEdit){
//			OnUpdate_Edit(nId - IDC_EDTTEXT);
			if (m_nTimerMode == eTimerModeTextChg)	::KillTimer( m_hMainWnd, TIMER_ID );
			::SetTimer( m_hMainWnd, TIMER_ID, TIMER_ELAPSE, NULL );
			m_nTimerMode = eTimerModeTextChg;
			m_bDelayTimer = true;
		}
		break;

	case IDC_OPTWNDLTRB:	//	オプションボタンの選択が変わった時
	case IDC_OPTWNDLTWH:
		m_bChkLTWH = (nId == IDC_OPTWNDLTWH);
		GetWndRectToEdit();
		break;

	case IDC_BTNWNDGET:		//	ウィンドウ枠の取得ボタン
		if (m_nTimerMode != eTimerModeUnDef)	::KillTimer( m_hMainWnd, TIMER_ID );
		::SetTimer( m_hMainWnd, TIMER_ID, TIMER_ELAPSE * 6, NULL );
		m_nTimerMode = eTimerModeGetWndRc;
		m_bDelayTimer = true;
		break;
	}
	return	bRetVal;
}





void	CSubWindow::EvtTimer(WPARAM wParam)
{	//	タイマメッセージの処理部分
	if (wParam != TIMER_ID)	return;
	::KillTimer( m_hMainWnd, TIMER_ID );
	switch (m_nTimerMode){
	case eTimerModeTextChg:		//
		//::MessageBox(NULL , TEXT("EvtTimer TextChg") , TEXT("MessageBox") , MB_OK);
		GetEditToWndRect();
		break;
	case eTimerModeGetWndRc:	//
		//::MessageBox(NULL , TEXT("EvtTimer WndRc") , TEXT("MessageBox") , MB_OK);
		GetAnotherWndRect();
		break;
	}
	m_nTimerMode = eTimerModeUnDef;
}





