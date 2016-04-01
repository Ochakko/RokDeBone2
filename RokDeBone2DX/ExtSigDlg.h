// ExtSigDlg.h : CExtSigDlg の宣言

#ifndef __EXTSIGDLG_H_
#define __EXTSIGDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CExtSigDlg
class CExtSigDlg : 
	public CAxDialogImpl<CExtSigDlg>
{
public:
	CExtSigDlg();
	~CExtSigDlg();

	enum { IDD = IDD_EXTSIGDLG };

BEGIN_MSG_MAP(CExtSigDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFEXTSIG, OnRef)
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
	char name[_MAX_PATH];
	int chkbone;
	int chkei1;
	int chkei2;
	int chkei3;
	int chknotjpos;
	int chkds;
	float mult;
private:
	CWindow m_dlg_wnd;
	CWindow m_name_wnd;
	CWindow m_mult_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__EXTSIGDLG_H_
