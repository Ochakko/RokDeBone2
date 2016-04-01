// MQODLG.h : CMQODlg の宣言

#ifndef __MQODLG_H_
#define __MQODLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>

/////////////////////////////////////////////////////////////////////////////
// CMQODlg
class CMQODlg : 
	public CAxDialogImpl<CMQODlg>
{
public:
	CMQODlg( float srcmult, int srctype );
	~CMQODlg();

	enum { IDD = IDD_MQODLG };

BEGIN_MSG_MAP(CMQODlg)
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
	float m_mult;
	int m_bonetype;

private:
	CWindow m_dlg_wnd;
	CWindow m_mult_wnd;

	CWindow m_radio_rdb2_wnd;
	CWindow m_radio_miko_wnd;

private:
	int SetWnd();

};

#endif //__MQODLG_H_
