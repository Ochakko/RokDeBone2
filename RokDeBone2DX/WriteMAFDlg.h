// WriteMAFDlg.h : CWriteMAFDlg の宣言

#ifndef __WriteMAFDlg_H_
#define __WriteMAFDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

#define MAFNAMELENG 32

class CLoadNameDlg;

/////////////////////////////////////////////////////////////////////////////
// CWriteMAFDlg
class CWriteMAFDlg : 
	public CAxDialogImpl<CWriteMAFDlg>
{
public:
	CWriteMAFDlg( HWND appwnd, CLoadNameDlg* namedlg );
	~CWriteMAFDlg();

	enum { IDD = IDD_WRITEMAFDLG };

BEGIN_MSG_MAP(CWriteMAFDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFPARENT, OnRef)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	char m_mafname[MAFNAMELENG];
	char m_parent[MAX_PATH];
	int m_savemethod;

	char m_newdirname[MAX_PATH];

private:
	CWindow m_dlg_wnd;
	CWindow m_mafname_wnd;
	CWindow m_parent_wnd;

	HWND m_appwnd;
	CLoadNameDlg* m_namedlg;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__WriteMAFDlg_H_
