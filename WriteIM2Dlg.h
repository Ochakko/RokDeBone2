// WriteIM2Dlg.h : CWriteIM2Dlg の宣言

#ifndef __WRITEIM2DLG_H_
#define __WRITEIM2DLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CWriteIM2Dlg
class CWriteIM2Dlg : 
	public CAxDialogImpl<CWriteIM2Dlg>
{
public:
	CWriteIM2Dlg( char* srcfilename );
	~CWriteIM2Dlg();

	enum { IDD = IDD_WRITEIM2Dlg };

BEGIN_MSG_MAP(CWriteIM2Dlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REF_IM2, OnRefIM2)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefIM2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int m_formqo;
	char m_im2name[_MAX_PATH];

private:
	CWindow m_dlg_wnd;
	CWindow m_edit_im2name_wnd;

private:
	void SetWnd();
	int InitParams();
	int ParamsToDlg();

};

#endif //__WRITEIM2DLG_H_
