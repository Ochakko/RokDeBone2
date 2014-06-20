// FrameShiftDlg.h : CFrameShiftDlg の宣言

#ifndef __FrameShiftDlg_H_
#define __FrameShiftDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>


/////////////////////////////////////////////////////////////////////////////
// CFrameShiftDlg
class CFrameShiftDlg : 
	public CAxDialogImpl<CFrameShiftDlg>
{
public:
	CFrameShiftDlg( int srcstart, int srcend );
	~CFrameShiftDlg();

	enum { IDD = IDD_FRAMESHIFTDLG };

BEGIN_MSG_MAP(CFrameShiftDlg)
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
	int m_selstart;
	int m_selend;
	int m_shift;

	int m_allbone;

	CWindow m_dlgwnd;
	CWindow m_edit_selstart_wnd;
	CWindow m_edit_selend_wnd;
	CWindow m_edit_shift_wnd;

};

#endif //__FrameShiftDlg_H_
