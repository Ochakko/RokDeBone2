
#ifndef __ChkOWDlg_H_
#define __ChkOWDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>
class CTreeElem2;
class CShdElem;

/////////////////////////////////////////////////////////////////////////////
// CChkOWDlg
class CChkOWDlg : 
	public CAxDialogImpl<CChkOWDlg>
{
public:
	CChkOWDlg( char* srcfname );
	~CChkOWDlg();

	enum { IDD = IDD_CHKOWDLG };


BEGIN_MSG_MAP(CChkOWDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_ALLYES, OnAllYes)
	COMMAND_ID_HANDLER(IDC_ALLNO, OnAllNo)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllYes(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllNo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	int SetWnd();
	int ParamsToDlg();

public:
	char m_filename[MAX_PATH];
	int m_owkind;

	CWindow m_filename_wnd;

};

#endif //__ChkOWDlg_H_
