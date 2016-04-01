
#ifndef __ShadowParamDlg_H_
#define __ShadowParamDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CShadowParamDlg
class CShadowParamDlg : 
	public CAxDialogImpl<CShadowParamDlg>
{
public:
	CShadowParamDlg( float srcbias, float srcdarkrate );
	~CShadowParamDlg();

	enum { IDD = IDD_SHADOWDLG };

BEGIN_MSG_MAP(CShadowParamDlg)
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
	int ParamsToDlg();
	int DlgToParams();

public:
	float m_bias;
	float m_darkrate;

	CWindow m_bias_wnd;
	CWindow m_darkrate_wnd;

};

#endif //__ShadowParamDlg_H_
