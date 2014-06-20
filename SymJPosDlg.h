// SymJPosDlg.h : CSymJPosDlg の宣言

#ifndef __SymJPosDlg_H_
#define __SymJPosDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CSymJPosDlg
class CSymJPosDlg : 
	public CAxDialogImpl<CSymJPosDlg>
{
public:
	CSymJPosDlg();
	~CSymJPosDlg();

	enum { IDD = IDD_SYMJPOSDLG };

BEGIN_MSG_MAP(CSymJPosDlg)
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
	int m_createtype;

private:
	CWindow m_dlg_wnd;

private:
	void SetWnd();
	int InitParams();
	int ParamsToDlg();

};

#endif //__SymJPosDlg_H_
