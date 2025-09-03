2016/9/27 22:53:04

ファイル(&F)  →  新規(&N)  →  Ｃ＋＋プロジェクト

プロジェクト名：	WindowSize
ロケーション(&L):	E:\Project\ECLIPSE4\TESTPROG\PROG01\WndCtrl\WndSize01

プロジェクト・タイプ
	実行可能
		空のプロジェクト

ツールチェーン
	MinGW GCC

次へ(&N)
構成：
On	Debug
On	Release

完了(&F)

ファイル(&F)  →  新規(&N)  →  ソース・フォルダー
	プロジェクト名(&N)	WindowSize
	フォルダー名(&D)	src


ファイル(&F)  →  新規(&N)  →  ソース・ファイル
	ソース・フォルダー(&D)	WindowSize/src
	ソース・ファイル(&E):	Main.cpp


ファイル(&F)  →  新規(&N)  →  その他(&O)...
一般  →  表題なしのテキスト・ファイル


ファイル(&F)  →  別名保管(A)...
親フォルダーを入力または選択(&E)	WindowSize/src
ファイル(&N)	readme.txt


ファイル(&F)  →  新規(&N)  →  クラス
	ソース・フォルダー(S):	WindowSize/src
	名前空間(N):	""
	クラス名(C):	CMainWindow
	ベース・クラス(B):		無し
	メゾッド・スタブ(M):	そのまま
	ヘッダ(H):	CMainWindow.h
	ソース(E):	CMainWindow.cpp
CWindowSizeDoc


ファイル(&F)  →  新規(&N)  →  ヘッダー・ファイル
	ソース・フォルダ(D):		WindowSize/src
	ヘッダー・ファイル(E):		Resource.h
	テンプレート(T):			デフォルト C++ ヘッダー・テンプレート
mydef.h


ファイル(&F)  →  新規(&N)  →  ソース・ファイル
	ソース・フォルダ(D):		WindowSize/src
	ソース・ファイル(E):		GUI_Make.cpp
	テンプレート(T):			デフォルト C++ ソース・テンプレート


プロジェクト  →  プロパティー
	C/C++ ビルド  →  設定
		MinGW C++ Linker  →  ライブラリー  →  追加ボタン
			gdi32
		MinGW C++ Linker  →  その他  →  リンカー・フラグ
			-mwindows  -static-libstdc++ -static-libgcc
	-static-libgcc		: libgcc_s_sjlj-1.dll
	-static-libstdc++	: libstdc++-6.dll



WinMain（ANSI 版）内部で RegisterClassW を呼び出すと、タイトル等の文字を１文字しか認識しない。
当然 CreateWindow も ANSI 版を呼び出すことになる。


オプションボタンの設定には CheckRadioButton を使う。
この場合必要なのはダイアログボックスの HWND とオプションボタンのＩＤの組み合わせ。
個別であれば SendMessage(hwndCombo, BM_SETCHECK, BST_CHECKED, 0); 等でも可能。






Version: Neon.3 Release (4.6.3)  以降の処置
C:\pleiades\eclipse\mingw\x86_64-w64-mingw32\lib   →   D:\MyDll
	libwinpthread-1.dll

Windowsスタートメニュー（右クリック）  →  システム(Y)
	関連設定  →  システム情報
		システムの詳細設定
			詳細設定ﾀﾌﾞ  →  環境変数(N)...









