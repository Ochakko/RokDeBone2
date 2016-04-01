// NewJointDlg.h : CNewJointDlg の宣言

#ifndef __NEWJOINTDLG_H_
#define __NEWJOINTDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CNewJointDlg
class CNewJointDlg : 
	public CAxDialogImpl<CNewJointDlg>
{
public:
	CNewJointDlg();
	~CNewJointDlg();

	enum { IDD = IDD_NEWJOINTDLG };

BEGIN_MSG_MAP(CNewJointDlg)
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
	int jointnum;
	CWindow m_num_wnd;

};

#endif //__NEWJOINTDLG_H_
