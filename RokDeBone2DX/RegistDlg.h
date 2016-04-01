// JointLocDlg.h : CJointLocDlg の宣言

#ifndef __RegistDlg_H_
#define __RegistDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>


/////////////////////////////////////////////////////////////////////////////
// CRegistDlg
class CRegistDlg : 
	public CAxDialogImpl<CRegistDlg>
{
public:
	CRegistDlg();
	~CRegistDlg();

	enum { IDD = IDD_REGISTDLG };

BEGIN_MSG_MAP(RegistDlg)
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
private:
	int SetWnd();
	int ParamsToDlg();
	int DlgToParams();

public:
	char m_regkey[256];
	CWindow m_regkey_wnd;

};

#endif //__RegistDlg_H_
