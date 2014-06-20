// LdQuaDlg.h : CLdQuaDlg の宣言

#ifndef __LDQUADLG_H_
#define __LDQUADLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CLdQuaDlg
class CLdQuaDlg : 
	public CAxDialogImpl<CLdQuaDlg>
{
public:
	CLdQuaDlg();
	~CLdQuaDlg();

	enum { IDD = IDD_LDQUADLG };

BEGIN_MSG_MAP(CLdQuaDlg)
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
	D3DXVECTOR3 multvec;

private:
	CWindow m_multx_wnd;
	CWindow m_multy_wnd;
	CWindow m_multz_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__LDQUADLG_H_
