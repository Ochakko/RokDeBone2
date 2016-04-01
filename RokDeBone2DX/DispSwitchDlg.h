// DISPSWITCHDLG.h : CDispSwitchDlg の宣言

#ifndef __DISPSWITCHDLG_H_
#define __DISPSWITCHDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>

/////////////////////////////////////////////////////////////////////////////
// CDispSwitchDlg

class CMotHandler;
class CMotionPoint2;

class CDispSwitchDlg : 
	public CAxDialogImpl<CDispSwitchDlg>
{
public:
	CDispSwitchDlg( int srcframeno, CMotionPoint2* srcmp, CMotHandler* srclpmh, int srcmcookie );
	~CDispSwitchDlg();

	enum { IDD = IDD_DISPSWITCHDLG };

BEGIN_MSG_MAP(CDispSwitchDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)

	COMMAND_ID_HANDLER(IDC_SWITCHON, OnSwitchon)
	COMMAND_ID_HANDLER(IDC_SWITCHOFF, OnSwitchoff)


END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSwitchon(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSwitchoff(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int m_frameno;
	int m_editswitchno;



private:
	CWindow m_dlg_wnd;
	
	CWindow m_frameno_wnd;
	CWindow m_switch_wnd[15];
	CWindow m_combo_no_wnd;
	CWindow m_switchon_wnd;
	CWindow m_switchoff_wnd;


	CMotionPoint2* m_editmp;
	CMotHandler* m_mh;
	int m_cookie;

private:
	int SetWnd();
	int Params2Dlg();

};

#endif //__MQODLG_H_
