// MouseOptDlg.h : CMouseOptDlg の宣言

#ifndef __MouseOptDlg_H_
#define __MouseOptDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CMouseOptDlg
class CMouseOptDlg : 
	public CAxDialogImpl<CMouseOptDlg>
{
public:
	CMouseOptDlg( int srcml, int srcmr, int srcmw );
	~CMouseOptDlg();

	enum { IDD = IDD_MOUSEOPTDLG };

	int m_MouseL;
	int m_MouseR;
	int m_MouseW;

BEGIN_MSG_MAP(CMouseOptDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	//NOTIFY_HANDLER(IDC_UNSELCOL_LINE, NM_CUSTOMDRAW, OnCustomUnselLine)
	//NOTIFY_HANDLER(IDC_SELCOL_LINE, NM_CUSTOMDRAW, OnCustomSelLine)
	//NOTIFY_HANDLER(IDC_UNSELCOL_POINT, NM_CUSTOMDRAW, OnCustomUnselPoint)
	//NOTIFY_HANDLER(IDC_SELCOL_POINT, NM_CUSTOMDRAW, OnCustomSelPoint)

	//MESSAGE_HANDLER(WM_PAINT, OnPaint)
	//MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


private:
	CWindow m_dlg_wnd;

};

#endif //__MouseOptDlg_H_
