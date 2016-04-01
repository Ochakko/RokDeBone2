// LDSigDlg.h : CLDSigDlg の宣言

#ifndef __LDSIGDLG_H_
#define __LDSIGDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CLDSigDlg
class CLDSigDlg : 
	public CAxDialogImpl<CLDSigDlg>
{
public:
	CLDSigDlg( char* srcfilename );
	~CLDSigDlg();

	enum { IDD = IDD_LDSIG };

BEGIN_MSG_MAP(CLDSigDlg)
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
	char name[MAX_PATH];
	D3DXVECTOR3 multvec;

private:
	CWindow m_name_wnd;
	CWindow m_multx_wnd;
	CWindow m_multy_wnd;
	CWindow m_multz_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__LDSIGDLG_H_
