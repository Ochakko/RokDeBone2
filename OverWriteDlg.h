// OverWriteDlg.h : COverWriteDlg の宣言

#ifndef __OverWriteDlg_H_
#define __OverWriteDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>

class CLoadNameDlg;


/////////////////////////////////////////////////////////////////////////////
// COverWriteDlg
class COverWriteDlg : 
	public CAxDialogImpl<COverWriteDlg>
{
public:
	COverWriteDlg( CLoadNameDlg* namedlg );
	~COverWriteDlg();

	enum { IDD = IDD_OVERWRITEDLG };

BEGIN_MSG_MAP(COverWriteDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_OW_SIG, CheckOWSig)
	COMMAND_ID_HANDLER(IDC_OW_MOA, CheckOWMoa)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT CheckOWSig(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT CheckOWMoa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	int SetWnd();
	int ParamsToDlg();
	int DlgToParams();


public:
	int m_check_sig;
	int m_check_moa;

	CWindow m_dlg_wnd;
	CWindow m_check_sig_wnd;
	CWindow m_check_moa_wnd;
	CWindow m_text_sig_wnd;
	CWindow m_text_moa_wnd;
private:
	CLoadNameDlg* m_namedlg;

};

#endif //__OverWriteDlg_H_
