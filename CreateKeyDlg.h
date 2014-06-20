// CreateKeyDlg.h : CCreateKeyDlg �̐錾

#ifndef __CreateKeyDlg_H_
#define __CreateKeyDlg_H_

#include "resource.h"       // ���C�� �V���{��
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CCreateKeyDlg
class CCreateKeyDlg : 
	public CAxDialogImpl<CCreateKeyDlg>
{
public:
	CCreateKeyDlg();
	~CCreateKeyDlg();

	enum { IDD = IDD_CREATEKEYDLG };

BEGIN_MSG_MAP(CCreateKeyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// �n���h���̃v���g�^�C�v:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	int SetWnd();

public:
	int m_create;

private:
	CWindow m_dlg_wnd;

};

#endif //__CreateKeyDlg_H_
