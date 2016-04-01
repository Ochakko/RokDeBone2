// LdEquDlg.h : CLdEquDlg の宣言

#ifndef __LdEquDLG_H_
#define __LdEquDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CLdEquDlg
class CLdEquDlg : 
	public CAxDialogImpl<CLdEquDlg>
{
public:
	CLdEquDlg();
	~CLdEquDlg();

	enum { IDD = IDD_LDEQU };

BEGIN_MSG_MAP(CLdEquDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	float mult;

private:
	CWindow m_mult_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__LdEquDLG_H_
