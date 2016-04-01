// IKTargetDlg.h : CIKTargetDlg の宣言

#ifndef __IKTargetDlg_H_
#define __IKTargetDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>
class CTreeElem2;
class CShdElem;

/////////////////////////////////////////////////////////////////////////////
// CIKTargetDlg
class CIKTargetDlg : 
	public CAxDialogImpl<CIKTargetDlg>
{
public:
	CIKTargetDlg( CTreeElem2* srcte, CShdElem* srcse, D3DXVECTOR3 srccurpos );
	~CIKTargetDlg();

	enum { IDD = IDD_IKTARGETDLG };

BEGIN_MSG_MAP(CIKTargetDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_BUTTON1, OnApply)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	int SetWnd();
	int ParamsToDlg();
	int DlgToParams();

public:
	CTreeElem2* m_te;
	CShdElem* m_se;

	int m_iktarget;
	int m_iktargetlevel;
	int m_iktargetcnt;
	D3DXVECTOR3 m_iktargetpos;

	D3DXVECTOR3 m_curpos;


	CWindow m_dlg_wnd;
	CWindow m_text_name_wnd;
	CWindow m_text_curpos_wnd;
	CWindow m_edit_ikcnt_wnd;
	CWindow m_editx_wnd;
	CWindow m_edity_wnd;
	CWindow m_editz_wnd;

	CWindow m_combo_iklevel_wnd;

};

#endif //__IKTargetDlg_H_
