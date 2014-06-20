// JointLocDlg.h : CJointLocDlg の宣言

#ifndef __EditInfDlg_H_
#define __EditInfDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>
class CTreeElem2;
class CShdElem;

/////////////////////////////////////////////////////////////////////////////
// CEditInfDlg
class CEditInfDlg : 
	public CAxDialogImpl<CEditInfDlg>
{
public:
	CEditInfDlg( char* srcbonename, int srckind, char* srctext );
	~CEditInfDlg();

	enum { IDD = IDD_EDITINF };

BEGIN_MSG_MAP(CEditInfDlg)
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
	char m_bonename[MAX_PATH];
	int m_kind;

	char m_textval[MAX_PATH];//m_valを文字列にしたもの。
	float m_val;

	CWindow m_bonename_wnd;
	CWindow m_kind_wnd;
	CWindow m_val_wnd;

};

#endif //__EditInfDlg_H_
