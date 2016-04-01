// FilterDlg.h : CFilterDlg の宣言

#ifndef __FilterDlg_H_
#define __FilterDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <usercoef.h>


class CRpsFile;
class CTxtSetting;

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg
class CFilterDlg : 
	public CAxDialogImpl<CFilterDlg>
{
public:
	CFilterDlg();
	~CFilterDlg();

	enum { IDD = IDD_FILTERDLG };

BEGIN_MSG_MAP(CFilterDlg)
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

//	int ShowDlg( int cmdshow, CRpsFile* rpsfile, int hsid, int motid, int maxframe, CTxtSetting* txtsetting );

private:
	void SetWnd();
	int SetCombo();
	int ParamsToDlg();

public:
	int m_cmdshow;
	int m_filtertype;
	int m_filtersize;

private:
//	CRpsFile* m_rpsfile;
	CWindow m_filtertype_wnd;
	CWindow m_filtersize_wnd;
//	int m_setcomboflag;
//	int m_hsid;
//	int m_motid;
//	int m_maxframe;
//	CTxtSetting* m_txtsetting;
};

#endif //__FilterDlg_H_
