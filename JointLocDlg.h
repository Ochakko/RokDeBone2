// JointLocDlg.h : CJointLocDlg の宣言

#ifndef __JOINTLOCDLG_H_
#define __JOINTLOCDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>
class CTreeElem2;
class CShdElem;

/////////////////////////////////////////////////////////////////////////////
// CJointLocDlg
class CJointLocDlg : 
	public CAxDialogImpl<CJointLocDlg>
{
public:
	CJointLocDlg( CTreeElem2* srcte, CShdElem* srcse, HWND appwnd );
	~CJointLocDlg();

	enum { IDD = IDD_JOINTLOCDLG };

BEGIN_MSG_MAP(CJointLocDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_APPLY, OnApply)
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

	CVec3f m_firstloc;
	CVec3f m_saveloc;
	CVec3f m_temploc;
	HWND	m_appwnd;
	
	CWindow m_text_name_wnd;
	CWindow m_editx_wnd;
	CWindow m_edity_wnd;
	CWindow m_editz_wnd;
};

#endif //__JOINTLOCDLG_H_
