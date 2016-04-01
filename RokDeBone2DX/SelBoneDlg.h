// SelBoneDlg.h : CSelBoneDlg の宣言

#ifndef __SelBoneDlg_H_
#define __SelBoneDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>


/////////////////////////////////////////////////////////////////////////////
// CSelBoneDlg
class CSelBoneDlg : 
	public CAxDialogImpl<CSelBoneDlg>
{
public:
	CSelBoneDlg();
	~CSelBoneDlg();

	enum { IDD = IDD_SELBONEDLG };

BEGIN_MSG_MAP(CSelBoneDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()


	//BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);


// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int m_doall;

private:
	CWindow m_dlg_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__SelBoneDlg_H_
