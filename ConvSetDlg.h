// ConvSetDlg.h : CConvSetDlg の宣言

#ifndef __ConvSetDlg_H_
#define __ConvSetDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <usercoef.h>

class CTxtSetting;
class CMyD3DApplication;

/////////////////////////////////////////////////////////////////////////////
// CConvSetDlg
class CConvSetDlg : 
	public CAxDialogImpl<CConvSetDlg>
{
public:
	CConvSetDlg();
	~CConvSetDlg();

	enum { IDD = IDD_CONVSETDLG };

BEGIN_MSG_MAP(CConvSetDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFKST, OnRefKst)
	COMMAND_ID_HANDLER(IDC_REFKSTIN, OnRefKstIn)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefKst(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefKstIn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	int ShowDlg( int cmdshow, CMyD3DApplication* papp, CTxtSetting* tsptr );

private:
	void SetWnd();
	int SetCombo();
	int ParamsToDlg();

	int GetJointInfo( int arrayleng, JOINTINFO* jiptr, int* getnum );
private:
	int m_cmdshow;
	CMyD3DApplication* m_papp;

	CWindow m_dlg_wnd;
	CWindow m_skel_wnd[ SKEL_MAX ];
	CWindow m_kstname_wnd;
	CWindow m_kstin_wnd;

	CTxtSetting* m_txtsetting;

	char m_kstname[MAX_PATH];
	char m_kstin[MAX_PATH];

	int m_bonenum;
	JOINTINFO* m_jointinfo;

};

#endif //__ConvSetDlg_H_
