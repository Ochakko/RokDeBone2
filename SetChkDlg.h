// SetChkDlg.h : CSetChkDlg の宣言

#ifndef __SetChkDlg_H_
#define __SetChkDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CSetChkDlg
class CSetChkDlg : 
	public CAxDialogImpl<CSetChkDlg>
{
public:
	CSetChkDlg( int undodlg, int rdlg, int infdlg, int modlg );
	~CSetChkDlg();

	enum { IDD = IDD_SETCHKDLG };

BEGIN_MSG_MAP(CSetChkDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	//NOTIFY_HANDLER(IDC_UNSELCOL_LINE, NM_CUSTOMDRAW, OnCustomUnselLine)
	//MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int m_undodlg;
	int m_rdlg;
	int m_infdlg;
	int m_modlg;

private:
	CWindow m_dlg_wnd;

};

#endif //__SetChkDlg_H_
