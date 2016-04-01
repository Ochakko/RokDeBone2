// TargetBoneDlg.h : CTargetBoneDlg の宣言

#ifndef __TargetBoneDlg_H_
#define __TargetBoneDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


class CCamKeyDlg;
class CTreeHandler2;
class CShdHandler;

/////////////////////////////////////////////////////////////////////////////
// CTargetBoneDlg
class CTargetBoneDlg : 
	public CAxDialogImpl<CTargetBoneDlg>
{
public:
	CTargetBoneDlg( CCamKeyDlg* srccamdlg );
	~CTargetBoneDlg();

	int ShowDlg( int srccmd, int srcchk, char* srcname, CTreeHandler2* srcth, CShdHandler* srcsh );
	int SetBoneName( char* srcname );

	enum { IDD = IDD_TARGETBONE };

BEGIN_MSG_MAP(CTargetBoneDlg)
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
public:
	int m_chk;
	char m_name[256];
	int m_cmdshow;
private:
	CCamKeyDlg* m_camdlg;
	CTreeHandler2* m_th;
	CShdHandler* m_sh;

	CWindow m_dlg_wnd;
	CWindow m_chkchuchu_wnd;
	CWindow m_bonename_wnd;
private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__TargetBoneDlg_H_
