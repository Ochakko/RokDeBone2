// FramePasteDlg.h : CFramePasteDlg の宣言

#ifndef __FramePasteDlg_H_
#define __FramePasteDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;

/////////////////////////////////////////////////////////////////////////////
// CFramePasteDlg
class CFramePasteDlg : 
	public CAxDialogImpl<CFramePasteDlg>
{
public:
	CFramePasteDlg( int hascancel, int curframe );
	~CFramePasteDlg();

	enum { IDD = IDD_FRAMEPASTEDLG };

BEGIN_MSG_MAP(CFramePasteDlg)
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

public:
	int m_select;
	int m_start;
	int m_end;

private:
	int m_hascancel;
	int m_curframe;

	CWindow m_dlg_wnd;
	CWindow m_edit_start_wnd;
	CWindow m_edit_end_wnd;
	CWindow m_cancel_wnd;

};

#endif //__FramePasteDlg_H_
