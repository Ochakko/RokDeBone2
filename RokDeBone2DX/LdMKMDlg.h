// LdMkmDlg.h : CLdMkmDlg の宣言

#ifndef __LdMkmDlg_H_
#define __LdMkmDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CLdMkmDlg
class CLdMkmDlg : 
	public CAxDialogImpl<CLdMkmDlg>
{
public:
	CLdMkmDlg();
	~CLdMkmDlg();

	enum { IDD = IDD_LDMKM };

BEGIN_MSG_MAP(CLdMkmDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFMKMNAME, OnRefMkmName)
	COMMAND_ID_HANDLER(IDC_REFINITNAME, OnRefInitName)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefMkmName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefInitName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	char mkmname[_MAX_PATH];
	char initname[_MAX_PATH];
	float mult;

private:
	CWindow m_mkmname_wnd;
	CWindow m_initname_wnd;
	CWindow m_mult_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__LdMkmDlg_H_
