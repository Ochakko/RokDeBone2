// WriteBFDlg.h : CWriteBFDlg の宣言

#ifndef __WRITEBFDLG_H_
#define __WRITEBFDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CWriteBFDlg
class CWriteBFDlg : 
	public CAxDialogImpl<CWriteBFDlg>
{
public:
	CWriteBFDlg();
	~CWriteBFDlg();

	enum { IDD = IDD_WRITEBFDLG };

BEGIN_MSG_MAP(CWriteBFDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_CHECK1, OnRelative)
	COMMAND_ID_HANDLER(IDC_MQO, OnMQO)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnRelative(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnMQO(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	int SetWnd();
	int Params2Dlg();

public:
	int m_bftype;
	int m_relativeflag;
	int m_mqoflag;

private:
	CWindow m_dlg_wnd;
	CWindow m_radio1_wnd;
	CWindow m_radio2_wnd;

	CWindow m_rel_wnd;
	CWindow m_mqo_wnd;
};

#endif //__WRITEBFDLG_H_
