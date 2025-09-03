/*
 * CMainWindow.cpp
 *
 *  Created on: 2016/10/02
 *      Author: Takashi
 */

#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <windef.h>
#include <tchar.h>
#include "mydef.h"
#include <map>
#include "CMainWindow.h"


using namespace std;



map< HWND, CMainWindow* >	g_pDoc;
CMainWindow	*	CMainWindow::g_pCreateTarget = NULL;
HINSTANCE	CMainWindow::m_hInstance = NULL;






CMainWindow::CMainWindow() {
	// TODO 自動生成されたコンストラクター・スタブ
	m_hMainWnd = NULL;
	m_hEdtWndRect = m_hOptWndLTRB = m_hOptWndLTWH = NULL;		//	m_hBtnWndGet = m_hSubWnd =
	m_bChkLTWH = m_bChgEdit = m_bChgWndRc = m_bDelayTimer = false;
	m_nTimerMode = eTimerModeUnDef;
	//
/*	m_sSubWndCls.style = CS_HREDRAW | CS_VREDRAW;
	m_sSubWndCls.lpfnWndProc = CSubWindow::WndProc;
	m_sSubWndCls.cbClsExtra = 0;
	m_sSubWndCls.cbWndExtra = 0;
	m_sSubWndCls.hInstance = NULL;
	m_sSubWndCls.hIcon = NULL;
	m_sSubWndCls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	m_sSubWndCls.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	m_sSubWndCls.lpszMenuName = NULL;
	m_sSubWndCls.lpszClassName = _T("SubWndRect");//*/
}

CMainWindow::~CMainWindow() {
	// TODO Auto-generated destructor stub
}




void	CMainWindow::SetThisPtr()
{	//	ウィンドウ作成時の WM_CREATE のように、CreateWindow 関数が制御を返す前に呼び出されるメッセージは
	//	hWnd を特定できないため、hWnd の代わりにクラスのアドレスを目印とする。
	g_pCreateTarget = this;
}




HWND	CMainWindow::CreateMainWnd( LPTSTR szClassName, LPTSTR szTitleText, LPTSTR lpCmdLine, int nCmdShow )
{	//	Main.cpp の _tWinMain() から呼ばれる。 SetHInst() を先に呼び出す事。
	//	メインウィンドウを作成する。
	if (m_hInstance == NULL)	return	NULL;
	//	ウィンドウ作成時の WM_CREATE のように、CreateWindow 関数が制御を返す前に呼び出されるメッセージは
	//	hWnd を特定できないため、hWnd の代わりにクラスのアドレスを目印とする。
	g_pCreateTarget = this;		//	これは CreateWindow() の直前に設定しておく必要がある。
	RECT	rcWnd;
	rcWnd.left = CW_USEDEFAULT;		rcWnd.top = CW_USEDEFAULT;
	rcWnd.right = 320;		rcWnd.bottom = 240;		//	RECT 構造体のメンバとしては right と bottom だが、width, height として扱う
//	CetMainWndRc( lpCmdLine, &rcWnd );
	GetCmdlineSetting( lpCmdLine, &rcWnd, szTitleText );
	//
	HWND hWnd;
	hWnd = CreateWindow( szClassName, szTitleText, WS_OVERLAPPEDWINDOW,
			rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, NULL, NULL, m_hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	m_hMainWnd = hWnd;
	//
	//	以下デバッグ用
//	::SetWindowText( hWnd, lpCmdLine );
//	WCHAR	szTitle[512];
//	long	i, nData[4], nStrLen;
//	::CopyMemory( nData, &rcWnd, sizeof(RECT) );
//	nStrLen = 0;
//	for (i=0; i<4; i++){
//		if (nData[i] == CW_USEDEFAULT){
//			swprintf( szTitle + nStrLen, L",UD" );
//		}else{
//			swprintf( szTitle + nStrLen, L",%d", nData[i] );
//		}
//		nStrLen += wcslen( szTitle + nStrLen );
//	}
//	::SetWindowTextW( hWnd, szTitle + 1 );
	//
	return	m_hMainWnd;
}







LRESULT		CMainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CMainWindow	*pDoc = NULL;
	if (g_pDoc.find(hWnd) != g_pDoc.end())	pDoc = g_pDoc[hWnd];
	if (message == WM_CREATE   &&   pDoc == NULL){
		g_pDoc[hWnd] = pDoc = g_pCreateTarget;
	}
	if (pDoc != NULL){
		return	pDoc->WndProcCore(hWnd, message, wParam, lParam);
	}
	return	DefWindowProc(hWnd, message, wParam, lParam);
}



LRESULT	CMainWindow::WndProcCore(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




LRESULT	CMainWindow::OnSizeEvents(UINT message, WPARAM wParam, LPARAM lParam)
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



bool	CMainWindow::GetWndRectToEdit()
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



bool	CMainWindow::GetEditToWndRect()
{
	RECT	sDtpRc;
	WCHAR	szBuff[1048];
	int		nScanI[4];
	::GetWindowRect( ::GetDesktopWindow(), &sDtpRc );
	::GetWindowTextW( m_hEdtWndRect, szBuff, 1048 );
	//	最初の１文字が t or T ならタイトル変更
	if (0 == _wcsnicmp( szBuff, L"T", 1 )){
		::SetWindowTextW( m_hMainWnd, szBuff + 1 );
		GetWndRectToEdit();
		return	true;
	}
	//	４つの数値を読み取る
	swscanf( szBuff, L"%d,%d,%d,%d", nScanI, nScanI+1, nScanI+2, nScanI+3 );
	if (!m_bChkLTWH){	//	LTRB	選択
		nScanI[2] -= nScanI[0];
		nScanI[3] -= nScanI[1];
	}
	//	不具合のある数値なら、ここではじく
	if (nScanI[2] < 0  ||  nScanI[2] > sDtpRc.right)	return	false;
	if (nScanI[3] < 0  ||  nScanI[3] > sDtpRc.bottom)	return	false;
	if (nScanI[0] + nScanI[2] < 10  ||  nScanI[0] > sDtpRc.right - 10)	return	false;
	if (nScanI[1] + nScanI[3] < 10  ||  nScanI[1] > sDtpRc.bottom- 10)	return	false;
	//	ウィンドウの位置サイズに設定する
	m_bChgWndRc = true;
	::MoveWindow( m_hMainWnd, nScanI[0], nScanI[1], nScanI[2], nScanI[3], TRUE );
	m_bChgWndRc = false;
	return	true;
}


bool	CMainWindow::GetTextToRect( LPCWSTR szSrc, RECT *outRect )
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



bool	CMainWindow::CetMainWndRc( LPCTSTR szSrc, RECT *outRect )
{	//	コマンドラインから /w /mw を見つけ出し、矩形の４要素を抽出する。
	LPCTSTR	szSlash = _T("/"), szRb = _T("rb"), szWh = _T("wh");
	LPCTSTR	szWnd = _T("w"), szMW = _T("mw");
	LPCTSTR	szNumSet = _T("-0123456789");
	long	nStrLen, nSlPos;
	long	nLSPos, nLEPos, nData[4], i, j;
	bool	isMinus, isWh;
	if (szSrc == NULL)	return	false;		//	引数確認
	if (outRect == NULL)	return	false;//
	//
	LPCTSTR	szFirstSl = _tcschr( szSrc, *szSlash );
	if (NULL == szFirstSl)	return	false;
	nStrLen = _tcslen( szFirstSl );		nSlPos = 1;			isWh = true;			//	初期設定は ltwh
	while(nSlPos < nStrLen){
		if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szRb, 2 ) ){		//	ltrb 形式を指定
			isWh = false;
		}else if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szWh, 2 ) ){	//	ltwh 形式を指定
			isWh = true;
		}else if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szWnd, 1 )
				||  0  ==  _tcsnicmp(szFirstSl + nSlPos, szMW, 2 ) ){
			nSlPos += (szFirstSl[nSlPos] == _T('m')  ?  2 : 1);	//	mw なら+2 w なら+1
			nSlPos += (szFirstSl[nSlPos] == _T(':')  ?  1 : 0);	//
			if (NULL  != _tcschr( szNumSet, szFirstSl[nSlPos] ) ){	//	数字が出てくれば矩形の４要素
				nLEPos = nSlPos;
				for (i = 0; i < 4; i++){
					nLSPos = nLEPos + (long)_tcscspn( szFirstSl + nLEPos, szNumSet );	//	数字のみで構成される範囲を
					nLEPos = nLSPos + (long) _tcsspn( szFirstSl + nLSPos, szNumSet );	//	ここで調査する。
					//
					nData[i] = 0;	isMinus = (szFirstSl[nLSPos] == _T('-'));
					if (isMinus)	nLSPos++;
					for (j = nLSPos; j < nLEPos; j++){
						if (szFirstSl[j] == _T('-'))	break;
						nData[i] *= 10;
						nData[i] += (long)(szFirstSl[j] - _T('0'));
					}
					if (isMinus)	nData[i] *= -1;
				}
				//	これを outRect に格納する。
				outRect->left	= nData[0];
				outRect->top	= nData[1];
				outRect->right	= nData[2];
				outRect->bottom	= nData[3];
				if (!isWh){		//	利便性のため、ltrb 形式は ltwh 形式に変換
					outRect->right	= nData[2] - nData[0];
					outRect->bottom	= nData[3] - nData[1];
				}
			}
		}
		//
		nSlPos += (long)_tcscspn( szFirstSl + nSlPos, szSlash )  +  1;
	}
	//
	//
	//
	return	true;
}


bool	CMainWindow::GetCmdlineSetting( LPCTSTR szSrc, RECT *outWndRc, LPTSTR outTitleText )
{	//	コマンドラインから /w /mw を見つけ出し、矩形の４要素を抽出する。
	LPCTSTR	szSlash = _T("/"), szRb = _T("rb"), szWh = _T("wh");
	LPCTSTR	szWnd = _T("w"), szMW = _T("mw");
	LPCTSTR	szNumSet = _T("-0123456789");
	LPCTSTR	szTitle = _T("t"), szSpc = _T(" "), szQuot = _T("\"");
	long	nStrLen, nSlPos, nTLen;
	long	nLSPos, nLEPos, nData[4], i, j;
	bool	isMinus, isWh;
	if (szSrc == NULL)	return	false;		//	引数確認
	if (outWndRc == NULL)	return	false;//
	//
	LPCTSTR	szFirstSl = _tcschr( szSrc, *szSlash );
	if (NULL == szFirstSl)	return	false;
	nStrLen = _tcslen( szFirstSl );		nSlPos = 1;			isWh = true;			//	初期設定は ltwh
	while(nSlPos < nStrLen){
		if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szRb, 2 ) ){		//	ltrb 形式を指定
			isWh = false;
		}else if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szWh, 2 ) ){	//	ltwh 形式を指定
			isWh = true;
		}else if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szWnd, 1 )
				||  0  ==  _tcsnicmp(szFirstSl + nSlPos, szMW, 2 ) ){
			nSlPos += (szFirstSl[nSlPos] == _T('m')  ?  2 : 1);	//	mw なら+2 w なら+1
			nSlPos += (szFirstSl[nSlPos] == _T(':')  ?  1 : 0);	//
			if (NULL  != _tcschr( szNumSet, szFirstSl[nSlPos] ) ){	//	数字が出てくれば矩形の４要素
				nLEPos = nSlPos;
				for (i = 0; i < 4; i++){
					nLSPos = nLEPos + (long)_tcscspn( szFirstSl + nLEPos, szNumSet );	//	数字のみで構成される範囲を
					nLEPos = nLSPos + (long) _tcsspn( szFirstSl + nLSPos, szNumSet );	//	ここで調査する。
					//
					nData[i] = 0;	isMinus = (szFirstSl[nLSPos] == _T('-'));
					if (isMinus)	nLSPos++;
					for (j = nLSPos; j < nLEPos; j++){
						if (szFirstSl[j] == _T('-'))	break;
						nData[i] *= 10;
						nData[i] += (long)(szFirstSl[j] - _T('0'));
					}
					if (isMinus)	nData[i] *= -1;
				}
				//	これを outRect に格納する。
				outWndRc->left	= nData[0];
				outWndRc->top	= nData[1];
				outWndRc->right	= nData[2];
				outWndRc->bottom	= nData[3];
				if (!isWh){		//	利便性のため、ltrb 形式は ltwh 形式に変換
					outWndRc->right	= nData[2] - nData[0];
					outWndRc->bottom	= nData[3] - nData[1];
				}
			}
		}else if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szTitle, 1 ) ){	//	タイトルを指定
			nSlPos += (szFirstSl[nSlPos+1] == _T(':')  ?  2 : 1);	//	/t: の場合は２つ進む
			if ( 0  ==  _tcsnicmp(szFirstSl + nSlPos, szQuot, 1 ) ){	//	" で囲まれている文字
				nSlPos++;
				nTLen = (long)_tcscspn( szFirstSl + nSlPos, szQuot );
			}else{
				nTLen = (long)_tcscspn( szFirstSl + nSlPos, szSpc );
			}
			if (outTitleText != NULL){
				_tcsncpy( outTitleText, szFirstSl + nSlPos, nTLen );
				outTitleText[nTLen] = outTitleText[nTLen + 1] = 0;
				nSlPos += nTLen;
			}
		}
		//
		nSlPos += (long)_tcscspn( szFirstSl + nSlPos, szSlash )  +  1;
	}
	//
	//
	//
	return	true;
}









bool	CMainWindow::GetAnotherWndRect()
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




bool	CMainWindow::OnUpdate_Edit(UINT nIndex)
{
	return	true;
}


bool	CMainWindow::CreateSubWnd()
{/*
	if (m_sSubWndCls.hInstance == NULL){
		m_sSubWndCls.hInstance = m_hInstance;
		m_sSubWndCls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		m_sSubWndCls.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		if (!RegisterClass(&m_sSubWndCls)) {
			return	false;
		}
		m_cSubWnd.SetHInst( m_hInstance );
		m_cSubWnd.SetThisPtr();
	}
	m_hSubWnd = ::CreateWindow( m_sSubWndCls.lpszClassName, _T("WindowRectSub"), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, m_hInstance, NULL);
	m_cSubWnd.SetHWnd(m_hSubWnd);
	ShowWindow(m_hSubWnd, SW_SHOW);
	UpdateWindow(m_hSubWnd);


	m_sSubWndCls.lpszClassName = _T("SubWndRect");
	//*/
	return	true;
}



