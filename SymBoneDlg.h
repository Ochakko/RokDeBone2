// SymBoneDlg.h : CSymBoneDlg の宣言

#ifndef __SymBoneDlg_H_
#define __SymBoneDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CSymBoneDlg
class CSymBoneDlg : 
	public CAxDialogImpl<CSymBoneDlg>
{
public:
	CSymBoneDlg();
	~CSymBoneDlg();

	enum { IDD = IDD_SYMBONEDLG };

BEGIN_MSG_MAP(CSymBoneDlg)
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

#endif //__SymBoneDlg_H_
