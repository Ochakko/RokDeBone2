// OneOrAllDlg.h : COneOrAllDlg �̐錾

#ifndef __OneOrAllDlg_H_
#define __OneOrAllDlg_H_

#include "resource.h"       // ���C�� �V���{��
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// COneOrAllDlg
class COneOrAllDlg : 
	public CAxDialogImpl<COneOrAllDlg>
{
public:
	COneOrAllDlg();
	~COneOrAllDlg();

	enum { IDD = IDD_ONEORALLDLG };

BEGIN_MSG_MAP(COneOrAllDlg)
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

public:
	int m_isone;

private:
	CWindow m_dlg_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__OneOrAllDlg_H_
