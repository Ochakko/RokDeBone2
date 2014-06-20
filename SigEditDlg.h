// SigEditDlg.h : CSigEditDlg の宣言

#ifndef __SIGEDITDLG_H_
#define __SIGEDITDLG_H_

#include <coef.h>
#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#define ID_CHILD_CONTROL 1

#include "axwinfix.h"
#include "RokDeBone2.h"

/////////////////////////////////////////////////////////////////////////////
// CSigEditDlg
//class CSigEditDlg : 
//	public CAxDialogImpl<CSigEditDlg>

class CSigEditDlg : 
	public CWindowImpl<CSigEditDlg, CWindow, CWinTraits<WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_WINDOWEDGE> >
{
public:
	CSigEditDlg();
	~CSigEditDlg();

//	enum { IDD = IDD_SIGEDITDLG };

BEGIN_MSG_MAP(CSigEditDlg)
//	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_MOVING, OnMoving)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	COMMAND_ID_HANDLER(IDMENU_SHDPARAM, OnMenuShdParam)
	COMMAND_ID_HANDLER(IDMENU_JOINTLOC, OnMenuJointLoc)
	COMMAND_ID_HANDLER(IDMENU_NEWJOINT, OnMenuNewJoint)
	COMMAND_ID_HANDLER(IDMENU_INFLUENCE, OnMenuInfluence)
	COMMAND_ID_HANDLER(IDMENU_JOINTDEL, OnMenuDelJoint)
	COMMAND_ID_HANDLER(IDMENU_CONV2SCOPE, OnMenuConv2Scope)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

//	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnMenuShdParam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnMenuJointLoc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnMenuNewJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnMenuInfluence(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnMenuDelJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnMenuConv2Scope(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int SetDbgFile( long dbgf );
	int SetAppWnd( long appwnd );
	int SetParams( long lpth, long lpsh, long lpmh, long appwnd );
	int Redraw();
	int OnUserSelchange( long selno );
	int InvalidateHandler();
	int Rename( int seri, char* srcname );
	int GetSelectedNo( int* selnoptr );

private:
	HWND	m_apphwnd;
	CAxWindowFix  m_ax;
	CComPtr<ISigTreeView2> m_ctrl;
	HMENU m_menu;


	LONG m_lpsh;
	int m_largex, m_largey;

	//scroll bar
    int m_wx, m_wy;//画面の大きさ
    int m_y;//スクロール位置
    int m_dy;//増分
    int m_rangey;//最大スクロール範囲
    int m_yclient;//現在のクライアント領域の高さ

	int m_x;
	int m_dx;
	int m_rangex;
	int m_xclient;
};

#endif //__SIGEDITDLG_H_
