// JointLocDlg.h : CJointLocDlg の宣言

#ifndef __KFParamDlg_H_
#define __KFParamDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

class CKFParams;
class CMotParamDlg;

/////////////////////////////////////////////////////////////////////////////
// CKFParamDlg
class CKFParamDlg : 
	public CAxDialogImpl<CKFParamDlg>
{
public:
	CKFParamDlg( CMotParamDlg* srcmpdlg );
	~CKFParamDlg();

	enum { IDD = IDD_MOTPARAMDLG };

BEGIN_MSG_MAP(CKFParamDlg)
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

	int InitParams();
	int DestroyObjs();

public:
	CKFParams* m_kfp;
	CMotParamDlg* m_mpdlg;

	CWindow m_mname_wnd;
	CWindow m_mframe_wnd;

	CWindow m_int_wnd[ KFPINUM ];
	CWindow m_double_wnd[ KFPDNUM ];

};

#endif //__KFParamDlg_H_
