// PasteTypeDlg.h : CPasteTypeDlg の宣言

#ifndef __PASTETYPEDLG_H_
#define __PASTETYPEDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CPasteTypeDlg
class CPasteTypeDlg : 
	public CAxDialogImpl<CPasteTypeDlg>
{
public:
	CPasteTypeDlg();
	~CPasteTypeDlg();

	enum { IDD = IDD_PASTETYPEDLG };

BEGIN_MSG_MAP(CPasteTypeDlg)
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
	int Params2Dlg();

public:
	int m_pastetype;

private:
	CWindow m_dlg_wnd;
	CWindow m_radio1_wnd;
	CWindow m_radio2_wnd;

};

#endif //__PASTETYPEDLG_H_

