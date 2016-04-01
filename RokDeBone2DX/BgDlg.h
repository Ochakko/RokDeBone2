// BgDlg.h : CBgDlg の宣言

#ifndef __BGDLG_H_
#define __BGDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CBgDlg
class CBgDlg : 
	public CAxDialogImpl<CBgDlg>
{
public:
	CBgDlg( char* srcname1, char* srcname2, float srcmvu, float srcmvv, COLORREF srccolor, int srcfixsize );
	~CBgDlg();

	enum { IDD = IDD_BGDLG };

BEGIN_MSG_MAP(CBgDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REF1, OnRef1)
	COMMAND_ID_HANDLER(IDC_REF2, OnRef2)
	COMMAND_ID_HANDLER(IDC_COLORREF, OnColorRef)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRef1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRef2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnColorRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	char name1[_MAX_PATH];
	char name2[_MAX_PATH];
	float mvu;
	float mvv;
	COLORREF	m_color;
	int m_fixsize;	
private:
	CWindow m_dlg_wnd;

	CWindow m_name1_wnd;
	CWindow m_name2_wnd;
	CWindow m_mvu_wnd;
	CWindow m_mvv_wnd;
	CWindow m_color_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__BGDLG_H_
