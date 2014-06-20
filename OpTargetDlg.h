// OpTargetDlg.h : COpTargetDlg の宣言

#ifndef __OpTargetDlg_H_
#define __OpTargetDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// COpTargetDlg
class COpTargetDlg : 
	public CAxDialogImpl<COpTargetDlg>
{
public:
	COpTargetDlg();
	~COpTargetDlg();

	enum { IDD = IDD_OPTARGETDLG };

	int ShowDlg( int srccmd, int srccurframe );
	int CalcStartEnd( int srccur, int srcmax );

BEGIN_MSG_MAP(COpTargetDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	//NOTIFY_HANDLER(IDC_UNSELCOL_LINE, NM_CUSTOMDRAW, OnCustomUnselLine)
	//MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	int ParamsToDlg();

public:
	int m_ope;
	int m_curframe;
	int m_startframe;
	int m_endframe;

private:
	CWindow m_dlg_wnd;
	CWindow m_curframe_wnd;
	CWindow m_selstart_wnd;
	CWindow m_selend_wnd;

	int m_cmdshow;
	int m_selstart;
	int m_selend;
};

#endif //__OpTargetDlg_H_
