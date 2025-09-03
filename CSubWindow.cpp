/*
 * CSubWindow.cpp
 *
 *  Created on: 2018/08/08
 *      Author: Takashi
 */

#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <windef.h>
#include <tchar.h>
#include "mydef.h"
#include <map>
#include "CSubWindow.h"

using namespace std;





map< HWND, CSubWindow* >	g_pSubDoc;
CSubWindow	*	CSubWindow::g_pCreateTarget = NULL;
HINSTANCE	CSubWindow::m_hInstance = NULL;




CSubWindow::CSubWindow() {
	// TODO 自動生成されたコンストラクター・スタブ
	m_hMainWnd = NULL;
	m_hEdtWndRect = m_hOptWndLTRB = m_hOptWndLTWH = m_hBtnWndGet = NULL;
	m_bChkLTWH = m_bChgEdit = m_bChgWndRc = m_bDelayTimer = false;
	m_nTimerMode = eTimerModeUnDef;
}

CSubWindow::~CSubWindow() {
	// TODO Auto-generated destructor stub
}



void	CSubWindow::SetThisPtr()
{	//	ウィンドウ作成時の WM_CREATE のように、CreateWindow 関数が制御を返す前に呼び出されるメッセージは
	//	hWnd を特定できないため、hWnd の代わりにクラスのアドレスを目印とする。
	g_pCreateTarget = this;
}




LRESULT		CSubWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CSubWindow	*pDoc = NULL;
	if (g_pSubDoc.find(hWnd) != g_pSubDoc.end())	pDoc = g_pSubDoc[hWnd];
	if (message == WM_CREATE   &&   pDoc == NULL){
		g_pSubDoc[hWnd] = pDoc = g_pCreateTarget;
	}
	if (pDoc != NULL){
		return	pDoc->WndProcCore(hWnd, message, wParam, lParam);
	}
	return	DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT	CSubWindow::WndProcCore(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if (m_hMainWnd == NULL)	m_hMainWnd = hWnd;
	switch (message){
	case	WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if (!OnCommand(wmId, wmEvent, lParam)){
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		return	OnCreate( wParam, lParam );

	case WM_TIMER:		// タイマメッセージ
		EvtTimer( wParam );
		return	0;
	case WM_MOVING:
	case WM_SIZING:
	case WM_SIZE:
		return	OnSizeEvents( message, wParam, lParam );
	case	WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;
	case	WM_DESTROY:
		//PostQuitMessage(0);
		::DestroyWindow( m_hMainWnd );
		m_hMainWnd = NULL;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




LRESULT	CSubWindow::OnSizeEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
//	LONG	nHeight, nBdrW;
//	SIZE	sWndSz;
	if (m_bChgWndRc)	::DefWindowProc(m_hMainWnd, message, wParam, lParam);
	//
	switch (message){
	case WM_SIZING:
		return	DefWindowProc(m_hMainWnd, message, wParam, lParam);
	case WM_MOVING:
	case WM_SIZE:
		GetWndRectToEdit();
	//	nBdrW = 5;
	//	nHeight = max2(20, (sWndRc.bottom - nBdrW) / 2);
	//	::MoveWindow( m_hEdtText, 0, 0, sWndRc.right, nHeight, TRUE );
	//	::MoveWindow( m_hEdtHexNum, 0, nHeight + nBdrW, sWndRc.right, nHeight, TRUE );
		break;
	default:
		return	::DefWindowProc(m_hMainWnd, message, wParam, lParam);
	}
	return 0;
}



bool	CSubWindow::GetWndRectToEdit()
{
	RECT	sWndRc;
	WCHAR	szBuff[1048];
	::GetWindowRect( m_hMainWnd, &sWndRc );
	//		if (IsDlgButtonChecked( m_hMainWnd, 8003) ){	//	LTWH	選択
	if (m_bChkLTWH){	//	LTWH	選択
		sWndRc.right -= sWndRc.left;
		sWndRc.bottom -= sWndRc.top;
	}
	swprintf( szBuff, L"%d,%d,%d,%d", sWndRc.left, sWndRc.top, sWndRc.right, sWndRc.bottom );
	m_bChgEdit = true;
	::SetWindowTextW( m_hEdtWndRect, szBuff );
	m_bChgEdit = false;
	return	true;
}



bool	CSubWindow::GetEditToWndRect()
{
	RECT	sDtpRc;
	WCHAR	szBuff[1048];
	int		nScanI[4];
	::GetWindowRect( ::GetDesktopWindow(), &sDtpRc );
	::GetWindowTextW( m_hEdtWndRect, szBuff, 1048 );
	swscanf( szBuff, L"%d,%d,%d,%d", nScanI, nScanI+1, nScanI+2, nScanI+3 );
	if (!m_bChkLTWH){	//	LTRB	選択
		nScanI[2] -= nScanI[0];
		nScanI[3] -= nScanI[1];
	}
	//
	if (nScanI[2] < 0  ||  nScanI[2] > sDtpRc.right)	return	false;
	if (nScanI[3] < 0  ||  nScanI[3] > sDtpRc.bottom)	return	false;
	if (nScanI[0] + nScanI[2] < 10  ||  nScanI[0] > sDtpRc.right - 10)	return	false;
	if (nScanI[1] + nScanI[3] < 10  ||  nScanI[1] > sDtpRc.bottom- 10)	return	false;
	//
	m_bChgWndRc = true;
	::MoveWindow( m_hMainWnd, nScanI[0], nScanI[1], nScanI[2], nScanI[3], TRUE );
	m_bChgWndRc = false;
	return	true;
}


bool	CSubWindow::GetTextToRect( LPCWSTR szSrc, RECT *outRect )
{
	LPCWSTR	szNumSet = L"-0123456789";
	long	nLSPos, nLEPos, nData[4], i, j;
	bool	isMinus;
	//
	nLEPos = 0;
	for (i = 0; i < 4; i++){
		nLSPos = nLEPos + (long)wcscspn( szSrc + nLEPos, szNumSet );
		nLEPos = nLSPos + (long) wcsspn( szSrc + nLSPos, szNumSet );
		//
		nData[i] = 0;	isMinus = (szSrc[nLSPos] == L'-');
		if (isMinus)	nLSPos++;
		for (j = nLSPos; j < nLEPos; j++){
			if (szSrc[j] == L'-')	break;
			nData[i] *= 10;
			nData[i] += (long)(szSrc[j] - L'0');
		}
		if (isMinus)	nData[i] *= -1;
	}
	//
	outRect->left	= nData[0];
	outRect->top	= nData[1];
	outRect->right	= nData[2];
	outRect->bottom	= nData[3];
	return	true;
}



bool	CSubWindow::GetAnotherWndRect()
{
	HWND	hTopWnd;
	RECT	sWndRc;
	hTopWnd = ::GetWindow( ::GetDesktopWindow(), GW_CHILD );
	hTopWnd = ::GetWindow( m_hMainWnd, GW_HWNDFIRST );
	::GetWindowRect( hTopWnd, &sWndRc );
	sWndRc.right -= sWndRc.left;		sWndRc.bottom -= sWndRc.top;		//	LTWH に変換
	m_bChgWndRc = false;
	//::MoveWindow( m_hMainWnd, sWndRc.left, sWndRc.top, sWndRc.right, sWndRc.bottom, TRUE );
	//
	if (!m_bChkLTWH){	//	LTRB	選択
		sWndRc.right += sWndRc.left;
		sWndRc.bottom += sWndRc.top;
	}
	WCHAR	szBuff[1048];
//	swprintf( szBuff, L"%d,%d,%d,%d", sWndRc.left, sWndRc.top, sWndRc.right, sWndRc.bottom );
	::GetWindowTextW( hTopWnd, szBuff, 1048 );
	//
	m_bChgEdit = true;
	::SetWindowTextW( m_hEdtWndRect, szBuff );
	m_bChgEdit = false;




	return	true;
}




bool	CSubWindow::OnUpdate_Edit(UINT nIndex)
{
	return	true;
}
























