// LOADBVHDLG.h : CLoadBVHDlg の宣言

#ifndef __LOADBVHDLG_H_
#define __LOADBVHDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>


/////////////////////////////////////////////////////////////////////////////
// CLoadBVHDlg
class CLoadBVHDlg : 
	public CAxDialogImpl<CLoadBVHDlg>
{
public:
	CLoadBVHDlg( char* srcfilename );
	~CLoadBVHDlg();

	enum { IDD = IDD_LDBVH };

BEGIN_MSG_MAP(CLoadBVHDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REF_BVH, OnRefBVH)
	COMMAND_ID_HANDLER(IDC_REF_MQONAME, OnRefMQO)
	COMMAND_ID_HANDLER(IDC_REF_BONNAME, OnRefBON)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefBVH(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefMQO(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefBON(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	char m_name[_MAX_PATH];
	char m_mqoname[_MAX_PATH];
	char m_bonname[_MAX_PATH];
	int m_motonly;
	float m_mult;

private:
	CWindow m_dlg_wnd;
	CWindow m_name_wnd;
	CWindow m_motonly_wnd;

	CWindow m_mqo_wnd;
	CWindow m_bon_wnd;
	CWindow m_mult_wnd;
private:
	int SetWnd();

};

#endif //__LOADBVHDLG_H_
