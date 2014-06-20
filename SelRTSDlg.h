// SelRTSDlg.h : CSelRTSDlg の宣言

#ifndef __SelRTSDlg_H_
#define __SelRTSDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>


/////////////////////////////////////////////////////////////////////////////
// CSelRTSDlg
class CSelRTSDlg : 
	public CAxDialogImpl<CSelRTSDlg>
{
public:
	CSelRTSDlg( int mkind );
	~CSelRTSDlg();

	enum { IDD = IDD_SELRTSDLG };

BEGIN_MSG_MAP(CSelRTSDlg)
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
	int m_rot;
	int m_mv;
	int m_sc;

private:
	CWindow m_dlg_wnd;

private:
	void SetWnd();
	int ParamsToDlg();
};

#endif //__SelRTSDlg_H_
