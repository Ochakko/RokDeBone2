// NormalMapDlg.h : CNormalMapDlg の宣言

#ifndef __NormalMapDlg_H_
#define __NormalMapDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CNormalMapDlg
class CNormalMapDlg : 
	public CAxDialogImpl<CNormalMapDlg>
{
public:
	CNormalMapDlg();
	~CNormalMapDlg();

	enum { IDD = IDD_NORMALMAP };

BEGIN_MSG_MAP(CNormalMapDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFHMAP, OnRefHMap)
	COMMAND_ID_HANDLER(IDC_REFNMAP, OnRefNMap)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefHMap(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefNMap(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	char hmapname[_MAX_PATH];
	char nmapname[_MAX_PATH];
	float height;

private:
	CWindow m_hmap_wnd;
	CWindow m_nmap_wnd;
	CWindow m_hparam_wnd;


private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__NormalMapDlg_H_
