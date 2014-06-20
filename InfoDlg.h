// InfoDlg.h : CInfoDlg の宣言

#ifndef __InfoDlg_H_
#define __InfoDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <usercoef.h>

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg
class CInfoDlg : 
	public CAxDialogImpl<CInfoDlg>
{
public:
	CInfoDlg();
	~CInfoDlg();

	enum { IDD = IDD_INFODLG };

BEGIN_MSG_MAP(CInfoDlg)
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

};

#endif //__InfoDlg_H_
