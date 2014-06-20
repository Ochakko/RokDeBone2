// SndSettingDlg.h : CSndSettingDlg の宣言

#ifndef __SNDSETTINGDLG_H_
#define __SNDSETTINGDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CSndSettingDlg
class CSndSettingDlg : 
	public CAxDialogImpl<CSndSettingDlg>
{
public:
	CSndSettingDlg( char* nameptr, int totalframe, int mottype, int srcstartframe, int srcendframe, int fixleng = 0 );
	~CSndSettingDlg();

	enum { IDD = IDD_SNDSETTINGDLG };

BEGIN_MSG_MAP(CSndSettingDlg)
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
	char m_name[ 256 ];
	int m_totalframe;
	int m_mottype;

	int m_startframe;
	int m_endframe;
	int m_fixlengflag;

private:
	CWindow m_dlg_wnd;
	CWindow m_name_wnd;
	CWindow m_totalframe_wnd;

	CWindow m_start_wnd;
	CWindow m_end_wnd;

};

#endif //__SNDSETTINGDLG_H_
