
#ifndef __EditListInfDlg_H_
#define __EditListInfDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>

class CMyD3DApplication;


/////////////////////////////////////////////////////////////////////////////
// CEditListInfDlg
class CEditListInfDlg : 
	public CAxDialogImpl<CEditListInfDlg>
{
public:
	CEditListInfDlg( CMyD3DApplication* papp, int seldisp, int vertno, int selbone, INFELEM* ieptr, int infnum );
	~CEditListInfDlg();

	enum { IDD = IDD_EDITLISTINF };

BEGIN_MSG_MAP(CEditListInfDlg)
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
	int m_seldisp;
	int m_selvert;
	int m_selbone;
	INFELEM* m_ieptr;
	int m_infnum;

	float m_val;

private:
	CMyD3DApplication* m_papp;

	CWindow m_dispobjname_wnd;
	CWindow m_vertno_wnd;
	CWindow m_bonename_wnd;
	CWindow m_infval_wnd;

};

#endif //__EditListInfDlg_H_
