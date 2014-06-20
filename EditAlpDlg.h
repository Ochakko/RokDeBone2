// JointLocDlg.h : CJointLocDlg の宣言

#ifndef __EditAlpDlg_H_
#define __EditAlpDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>
class CTreeElem2;
class CShdElem;

/////////////////////////////////////////////////////////////////////////////
// CEditAlpDlg
class CEditAlpDlg : 
	public CAxDialogImpl<CEditAlpDlg>
{
public:
	CEditAlpDlg( char* srcmatnamez, float srcalpha );
	~CEditAlpDlg();

	enum { IDD = IDD_EDITALP };

BEGIN_MSG_MAP(CEditAlpDlg)
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
	char m_matname[256];
	float m_val;

	CWindow m_matname_wnd;
	CWindow m_val_wnd;

};

#endif //__EditAlpDlg_H_
