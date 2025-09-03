/*
 * CSubWindow.h
 *
 *  Created on: 2018/08/08
 *      Author: Takashi
 */

#ifndef CSUBWINDOW_H_
#define CSUBWINDOW_H_

class CSubWindow {
public:
	CSubWindow();
	virtual ~CSubWindow();

public:		//	メッセージ処理関連
	void	SetThisPtr();
	static	LRESULT		WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	void	SetHInst( HINSTANCE inHInst ){	m_hInstance = inHInst;	}
	void	SetHWnd( HWND inHWnd ){		m_hMainWnd = inHWnd;	}

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
	//	その他
	bool	GetWndRectToEdit();
	bool	GetEditToWndRect();
	bool	GetAnotherWndRect();
	bool	GetTextToRect( LPCWSTR szSrc, RECT *outRect );

//	内部変数
protected:
	static	CSubWindow	*g_pCreateTarget;
	static	HINSTANCE	m_hInstance;
	HWND	m_hMainWnd, m_hEdtWndRect, m_hOptWndLTRB, m_hOptWndLTWH;
	HWND	m_hBtnWndGet;
	bool	m_bChkLTWH, m_bChgEdit, m_bChgWndRc, m_bDelayTimer;
	long	m_nTimerMode;
//	HMENU	m_hMenu;
};

#endif /* CSUBWINDOW_H_ */
