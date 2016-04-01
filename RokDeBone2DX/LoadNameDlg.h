// LoadNameDlg.h : CLoadNameDlg の宣言

#ifndef __LOADNAMEDLG_H_
#define __LOADNAMEDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CLoadNameDlg
class CLoadNameDlg : 
	public CAxDialogImpl<CLoadNameDlg>
{
public:
	CLoadNameDlg();
	~CLoadNameDlg();

	enum { IDD = IDD_LOADNAMEDLG };

BEGIN_MSG_MAP(CLoadNameDlg)
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
	char m_signame[2048];
	char m_quaname[2048];
	char m_im2name[2048];
	char m_bonname[2048];
	char m_moaname[2048];

	char m_recentname[2048];
private:
	CWindow m_signame_wnd;
	CWindow m_quaname_wnd;
	CWindow m_im2name_wnd;
	CWindow m_bonname_wnd;
	CWindow m_moaname_wnd;
private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__LOADNAMEDLG_H_
