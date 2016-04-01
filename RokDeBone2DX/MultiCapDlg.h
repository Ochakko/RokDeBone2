// JointLocDlg.h : CJointLocDlg の宣言

#ifndef __MultiCapDlg_H_
#define __MultiCapDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CMultiCapDlg
class CMultiCapDlg : 
	public CAxDialogImpl<CMultiCapDlg>
{
public:
	CMultiCapDlg( int motleng );
	~CMultiCapDlg();

	enum { IDD = IDD_MULTICAPDLG };

BEGIN_MSG_MAP(CMultiCapDlg)
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
	int ParamsToDlg();
	int DlgToParams();

public:
	int m_motleng;
	int m_framestep;

private:
	CWindow m_frames_wnd;

};

#endif //__MultiCapDlg_H_
