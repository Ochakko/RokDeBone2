// WriteBldDlg.h : CWriteBldDlg の宣言

#ifndef __WriteBldDlg_H_
#define __WriteBldDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

class CMyD3DApplication;
class CMotParamDlg;

/////////////////////////////////////////////////////////////////////////////
// CWriteBldDlg
class CWriteBldDlg : 
	public CAxDialogImpl<CWriteBldDlg>
{
public:
	CWriteBldDlg( CMyD3DApplication* papp, CMotParamDlg* mpdlg );
	~CWriteBldDlg();
	
	int SetFlag();
	int InitMult();

	enum { IDD = IDD_WRITEBLDDLG };

BEGIN_MSG_MAP(CWriteBldDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_REFBLD, OnRefBld)
	COMMAND_ID_HANDLER(IDC_REFSIG, OnRefSig)
	COMMAND_ID_HANDLER(IDC_REFGROUND, OnRefGround)
	COMMAND_ID_HANDLER(IDC_REFMOA, OnRefMoa)
	COMMAND_ID_HANDLER(IDC_REFGPA, OnRefGpa)
	COMMAND_ID_HANDLER(IDC_REFMOE, OnRefMoe)
	COMMAND_ID_HANDLER(IDC_REFCAM, OnRefCam)
	COMMAND_ID_HANDLER(IDC_REFSSF, OnRefSsf)
	COMMAND_ID_HANDLER(IDC_REFSAF, OnRefSaf)

	COMMAND_ID_HANDLER(IDC_CAMMODE, OnCamMode)

	COMMAND_ID_HANDLER(IDC_CHKGPA, OnChkGpa)
	COMMAND_ID_HANDLER(IDC_CHKMOE, OnChkMoe)
	COMMAND_ID_HANDLER(IDC_CHKCAM, OnChkCam)
	COMMAND_ID_HANDLER(IDC_CHKSSF, OnChkSsf)
	COMMAND_ID_HANDLER(IDC_CHKSAF, OnChkSaf)

END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnRefBld(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefSig(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefGround(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefMoa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefGpa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefMoe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefSsf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefSaf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCamMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnChkGpa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChkMoe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChkCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChkSsf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChkSaf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	void SetWnd();
	int ParamsToDlg();
	int InitParams();

public:
	char m_bldname[MAX_PATH];
	char m_signame[MAX_PATH];
	float m_sigmult;
	char m_gname[MAX_PATH];
	float m_gmult;
	char m_moaname[MAX_PATH];
	char m_gpaname[MAX_PATH];
	char m_moename[MAX_PATH];
	char m_camname[MAX_PATH];
	int m_cammode;
	char m_ssfname[MAX_PATH];
	char m_safname[MAX_PATH];

	int m_gflag;
	int m_gpaflag;
	int m_moeflag;
	int m_camflag;
	int m_ssfflag;
	int m_safflag;

	int m_chkgpa;
	int m_chkmoe;
	int m_chkcam;
	int m_chkssf;
	int m_chksaf;

private:
	CMyD3DApplication* m_papp;
	CMotParamDlg* m_mpdlg;

	CWindow m_bld_wnd;
	CWindow m_sig_wnd;
	CWindow m_sigmult_wnd;
	CWindow m_g_wnd;
	CWindow m_gmult_wnd;
	CWindow m_moa_wnd;
	CWindow m_gpa_wnd;
	CWindow m_moe_wnd;
	CWindow m_cam_wnd;
	CWindow m_ssf_wnd;
	CWindow m_saf_wnd;
	CWindow m_cammode_wnd;

	CWindow m_refg_wnd;
	CWindow m_refgpa_wnd;
	CWindow m_refmoe_wnd;
	CWindow m_refcam_wnd;
	CWindow m_refssf_wnd;
	CWindow m_refsaf_wnd;

	CWindow m_chkgpa_wnd;
	CWindow m_chkmoe_wnd;
	CWindow m_chkcam_wnd;
	CWindow m_chkssf_wnd;
	CWindow m_chksaf_wnd;

	CWindow m_dlg_wnd;

	int m_createflag;
};

#endif //__WriteBldDlg_H_
