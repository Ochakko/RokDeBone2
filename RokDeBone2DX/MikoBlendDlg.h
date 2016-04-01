// MikoBlendDlg.h : CMikoBlendDlg の宣言

#ifndef __MIKOBLENDDLG_H_
#define __MIKOBLENDDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CMikoBlendDlg
class CMikoBlendDlg : 
	public CAxDialogImpl<CMikoBlendDlg>
{
public:
	CMikoBlendDlg();
	~CMikoBlendDlg();

	enum { IDD = IDD_MIKOBLENDDLG };

BEGIN_MSG_MAP(CMikoBlendDlg)
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
	int m_mikoblendtype;

private:
	CWindow m_dlg_wnd;

private:
	void SetWnd();
	int InitParams();
	int ParamsToDlg();

};

#endif //__MikoBlendDlg_H_
