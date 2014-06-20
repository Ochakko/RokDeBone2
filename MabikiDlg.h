// MabikiDlg.h : CMabikiDlg の宣言

#ifndef __MabikiDlg_H_
#define __MabikiDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

/////////////////////////////////////////////////////////////////////////////
// CMabikiDlg
class CMabikiDlg : 
	public CAxDialogImpl<CMabikiDlg>
{
public:
	CMabikiDlg();
	~CMabikiDlg();
	enum { IDD = IDD_MABIKIDLG };

BEGIN_MSG_MAP(CMabikiDlg)
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
	void SetWnd();
	int ParamsToDlg();

public:
	int m_step;
private:
	CWindow m_dlg_wnd;
	CWindow m_step_wnd;
};

#endif //__MabikiDlg_H_
