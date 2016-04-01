// MotSettingDlg.h : CMotSettingDlg の宣言

#ifndef __MOTSETTINGDLG_H_
#define __MOTSETTINGDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CMotSettingDlg
class CMotSettingDlg : 
	public CAxDialogImpl<CMotSettingDlg>
{
public:
	CMotSettingDlg( int calcsp, char* nameptr, int totalframe, int mottype, int srcinterpolation, 
		int srcmotjump, int srcstartframe, int srcendframe, int fixleng = 0 );
	~CMotSettingDlg();

	enum { IDD = IDD_MOTSETTINGDLG };

BEGIN_MSG_MAP(CMotSettingDlg)
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
	int m_interpolation;
	int m_motjump;

	int m_startframe;
	int m_endframe;
	int m_fixlengflag;

	int m_calcsp;
private:
	CWindow m_dlg_wnd;
	CWindow m_name_wnd;
	CWindow m_totalframe_wnd;

	CWindow m_radio_squad_wnd;
	CWindow m_radio_slerp_wnd;

	CWindow m_edit_motjump_wnd;

	CWindow m_start_wnd;
	CWindow m_end_wnd;

};

#endif //__MOTSETTINGDLG_H_
