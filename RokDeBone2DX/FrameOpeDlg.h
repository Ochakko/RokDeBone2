// FrameOpeDlg.h : CFrameOpeDlg の宣言

#ifndef __FRAMEOPEDLG_H_
#define __FRAMEOPEDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;

/////////////////////////////////////////////////////////////////////////////
// CFrameOpeDlg
class CFrameOpeDlg : 
	public CAxDialogImpl<CFrameOpeDlg>
{
public:
	CFrameOpeDlg( int hascancel, char* textptr, int* list, CTreeHandler2* lpth );
	~CFrameOpeDlg();

	enum { IDD = IDD_FRAMEOPEDLG };

BEGIN_MSG_MAP(CFrameOpeDlg)
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

private:
	int m_hascancel;
	char* m_text;
	int* m_list;
	CTreeHandler2* m_thandler;

	CWindow m_text_wnd;
	CWindow m_list_wnd;
	CWindow m_cancel_wnd;

};

#endif //__FRAMEOPEDLG_H_
