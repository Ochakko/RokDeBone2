// WriteBmpDlg.h : CWriteBmpDlg �̐錾

#ifndef __WRITEBMPDLG_H_
#define __WRITEBMPDLG_H_

#include "resource.h"       // ���C�� �V���{��
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CWriteBmpDlg
class CWriteBmpDlg : 
	public CAxDialogImpl<CWriteBmpDlg>
{
public:
	CWriteBmpDlg( int framemax );
	~CWriteBmpDlg();

	enum { IDD = IDD_WRITEBMPDLG };

BEGIN_MSG_MAP(CWriteBmpDlg)
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
	int m_startframe;
	int m_endframe;
	char m_prefix[1024];
	int m_framemax;

	CWindow m_prefix_wnd;
	CWindow m_startframe_wnd;
	CWindow m_endframe_wnd;

};

#endif //__WRITEBMPDLG_H_
