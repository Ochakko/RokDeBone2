// GraphWnd.h : CGraphWnd の宣言

#ifndef __GraphWnd_H_
#define __GraphWnd_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>

#include <D3DX9.h>

class CGraphRotDlg;
class CMyD3DApplication;
class CMotDlg;


/////////////////////////////////////////////////////////////////////////////
// CGraphWnd
class CGraphWnd : 
	public CWindowImpl<CGraphWnd>
{
public:
	CGraphWnd( CMyD3DApplication* srcapp );
	~CGraphWnd();

	int ShowDlg( int cmdshow );
	int Redraw();
	int OnUserSelchange();
	int ShiftRange( int srcdiff );
	int CloseSpDlg();

BEGIN_MSG_MAP(CGraphWnd)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)

	NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnSelchange_tab )

	MESSAGE_HANDLER(WM_SIZE, OnSizeDlg)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnSelchange_tab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSizeDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	int DestroyTabWindows();
	int ParamsToDlg();
	void SetWnd();
	int CreateTabWindows();

public:
	CGraphRotDlg* m_grotdlg;

	CWindow m_tab_wnd;

	int		m_disptabno;
	int		m_createtabflag;
private:
	CMyD3DApplication* m_papp;
	int		m_cmdshow;

};

#endif //__GraphWnd_H_
