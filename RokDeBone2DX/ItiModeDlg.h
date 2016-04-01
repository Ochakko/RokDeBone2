// ItiModeDlg.h : CItiModeDlg の宣言

#ifndef __ItiModeDlg_H_
#define __ItiModeDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

/////////////////////////////////////////////////////////////////////////////
// CItiModeDlg
class CItiModeDlg : 
	public CAxDialogImpl<CItiModeDlg>
{
public:
	CItiModeDlg();
	~CItiModeDlg();

	enum { IDD = IDD_ITIMODEDLG };

BEGIN_MSG_MAP(CItiModeDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int m_itimode;

private:
	CWindow m_dlg_wnd;

private:
	void SetWnd();
	int InitParams();

};

#endif //__ItiModeDlg_H_
