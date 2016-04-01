// LOADPARAMDLG.h : CLoadParamDlg の宣言

#ifndef __LOADPARAMDLG_H_
#define __LOADPARAMDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>

/////////////////////////////////////////////////////////////////////////////
// CLoadParamDlg
class CLoadParamDlg : 
	public CAxDialogImpl<CLoadParamDlg>
{
public:
	CLoadParamDlg( CVec3f minvec, CVec3f maxvec );
	~CLoadParamDlg();

	enum { IDD = IDD_LOADPARAMDLG };

BEGIN_MSG_MAP(CLoadParamDlg)
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
	float m_multiple;
	int m_swapxz;
	int m_centerx;
	int m_centerz;

	int m_invz;
	int m_divmode;
	
	int m_chkinv;
	CVec3f m_minvec;
	CVec3f m_maxvec;

private:
	CWindow m_dlg_wnd;
	CWindow m_multiple_wnd;
	CWindow m_swapxz_wnd;
	CWindow m_centerx_wnd;
	CWindow m_centerz_wnd;

	CWindow m_invz_wnd;
	CWindow m_div_color_wnd;
	CWindow m_div_group_wnd;

	CWindow m_chkinv_wnd;

	CWindow m_rangex_wnd;
	CWindow m_rangey_wnd;
	CWindow m_rangez_wnd;

private:
	int SetWnd();

};

#endif //__LOADPARAMDLG_H_
