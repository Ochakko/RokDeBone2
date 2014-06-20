// MotCutDlg.h : CMotCutDlg の宣言

#ifndef __MotCutDlg_H_
#define __MotCutDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CMotCutDlg
class CMotCutDlg : 
	public CAxDialogImpl<CMotCutDlg>
{
public:
	CMotCutDlg( int maxframe );
	~CMotCutDlg();

	enum { IDD = IDD_MOTCUTDLG };

BEGIN_MSG_MAP(CMotCutDlg)
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
	int m_chkfirst;
	int m_chklast;
	int m_firstframe;
	int m_lastframe;
	int m_maxframe;
private:
	CWindow m_dlg_wnd;
	CWindow m_chkfirst_wnd;
	CWindow m_chklast_wnd;
	CWindow m_firstframe_wnd;
	CWindow m_lastframe_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__MotCutDlg_H_
