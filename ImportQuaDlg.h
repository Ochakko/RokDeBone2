// ImportQuaDlg.h : CImportQuaDlg の宣言

#ifndef __ImportQuaDlg_H_
#define __ImportQuaDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CImportQuaDlg
class CImportQuaDlg : 
	public CAxDialogImpl<CImportQuaDlg>
{
public:
	CImportQuaDlg();
	~CImportQuaDlg();

	enum { IDD = IDD_IMPORTQUADLG };

BEGIN_MSG_MAP(CImportQuaDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REF, OnRef)
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
	char m_name[_MAX_PATH];
	int m_space;

private:
	CWindow m_dlg_wnd;
	CWindow m_name_wnd;
	CWindow m_edit_space_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__ImportQuaDlg_H_
