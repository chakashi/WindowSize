/*
 * CMainWindow.h
 *
 *  Created on: 2016/10/02
 *      Author: Takashi
 */

#ifndef CMAINWINDOW_H_
#define CMAINWINDOW_H_


#include "CSubWindow.h"


class CMainWindow {
public:
	CMainWindow();
	virtual ~CMainWindow();

public:		//	メッセージ処理関連
	void	SetThisPtr();
	static	LRESULT		WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	void	SetHInst( HINSTANCE inHInst ){	m_hInstance = inHInst;	}
	HWND	CreateMainWnd( LPTSTR szClassName, LPTSTR szTitleText, LPTSTR lpCmdLine, int nCmdShow );
//	void	SetHWnd( HWND inHWnd ){		m_hMainWnd = inHWnd;	}

//	列挙型
protected:
	enum{
		eTimerModeUnDef,	//	何もしない
		eTimerModeTextChg,	//	テキスト変更
		eTimerModeGetWndRc,	//	ウィンドウ枠取得
		eTimerModeMax
	};


//	内部関数
protected:
	//	メッセージ処理関連
	LRESULT	WndProcCore(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL	OnCommand(UINT nId, UINT nEvent, LPARAM lParam);
	LRESULT	OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT	OnSizeEvents(UINT message, WPARAM wParam, LPARAM lParam);
	void	EvtTimer(WPARAM wParam);
	bool	OnUpdate_Edit(UINT nIndex);
	//	コマンドライン
	bool	CetMainWndRc( LPCTSTR szSrc, RECT *outRect );
	bool	GetCmdlineSetting( LPCTSTR szSrc, RECT *outWndRc, LPTSTR outTitleText=NULL );
	//	その他
	bool	GetWndRectToEdit();
	bool	GetEditToWndRect();
	bool	GetAnotherWndRect();
	bool	CreateSubWnd();
	bool	GetTextToRect( LPCWSTR szSrc, RECT *outRect );

//	内部変数
protected:
	static	CMainWindow	*g_pCreateTarget;
	static	HINSTANCE	m_hInstance;
	HWND	m_hMainWnd, m_hEdtWndRect, m_hOptWndLTRB, m_hOptWndLTWH;
//	HWND	m_hBtnWndGet,  m_hSubWnd;
	bool	m_bChkLTWH, m_bChgEdit, m_bChgWndRc, m_bDelayTimer;
	long	m_nTimerMode;
//	HMENU	m_hMenu;
//	WNDCLASS m_sSubWndCls;
//	CSubWindow	m_cSubWnd;
};

#endif /* CMAINWINDOW_H_ */
