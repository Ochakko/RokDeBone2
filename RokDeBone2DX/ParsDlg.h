// ParsDlg.h : CParsDlg の宣言

#ifndef __PARSDLG_H_
#define __PARSDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CParsDlg
class CParsDlg : 
	public CAxDialogImpl<CParsDlg>
{
public:
	CParsDlg( int srcmode, int srcfov, int srcorthosize );
	~CParsDlg();

	enum { IDD = IDD_PARSDLG };

BEGIN_MSG_MAP(CParsDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	NOTIFY_HANDLER(IDC_SLIDER1, NM_RELEASEDCAPTURE, OnReleasedcaptureSl1)

END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnReleasedcaptureSl1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

public:
	int parsmode;//PARSMODE_*
	int parsfov;

	int orthosize;//

private:
	CWindow m_fovtext_wnd;
	CWindow m_sl_wnd;
	CWindow m_orthosize_wnd;
	CWindow m_dlg_wnd;
	

private:
	void SetWnd();
	int ParamsToDlg();

	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

};

#endif //__PARSDLG_H_
