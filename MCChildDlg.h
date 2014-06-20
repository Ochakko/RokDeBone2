// MCChildDlg.h : CMCChildDlg の宣言

#ifndef MCCHILDDLGH
#define MCCHILDDLGH

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

/////////////////////////////////////////////////////////////////////////////
// CMCChildDlg

class CMotHandler;

class CMCChildDlg : 
	public CAxDialogImpl<CMCChildDlg>
{
public:
	CMCChildDlg( CMotHandler* srclpmh, MCELEM* srcmce );
	~CMCChildDlg();

	enum { IDD = IDD_MCCHILDDLG };

BEGIN_MSG_MAP(CMCChildDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	//COMMAND_ID_HANDLER(IDC_SWITCHON, OnSwitchon)
	//COMMAND_ID_HANDLER(IDC_SWITCHOFF, OnSwitchoff)
END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//LRESULT OnSwitchon(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//LRESULT OnSwitchoff(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int m_cookie;
	int m_frameno1;
	int m_frameno2;
	int m_eventno1;
	int m_always;
	int m_notfu;
	int m_nottoidle;
private:
	CWindow m_dlg_wnd;
	
	CWindow m_fixframe_wnd;
	CWindow m_jumpto_wnd;
	CWindow m_eventno_wnd;
	CWindow m_name_wnd;

	CMotHandler* m_mh;
	MCELEM* m_mce;

private:
	int SetWnd();
	int Params2Dlg();

};

#endif //MCCHILDDLGH
