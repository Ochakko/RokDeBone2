// BonePropDlg.h : CBonePropDlg の宣言

#ifndef __BonePropDlg_H_
#define __BonePropDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>
#include <d3dx9.h>

class CMyD3DApplication;
class CQuaternion;

/////////////////////////////////////////////////////////////////////////////
// CBonePropDlg
class CBonePropDlg : 
	public CAxDialogImpl<CBonePropDlg>
{
public:
	CBonePropDlg( CMyD3DApplication* papp );
	~CBonePropDlg();

	int UpdateParams( int srcseri, int zakind );

	int ShowDlg( int cmdshow );

	int GetLimitedQ( int boneno, CQuaternion* startq, D3DXVECTOR3* befeul, CQuaternion* endq, CQuaternion* dstq, 
		int* startout, D3DXVECTOR3* dsteul, int notmvflag, int initaxisq );

	int GetLimitedQ( int boneno, D3DXVECTOR3 starteul, D3DXVECTOR3 endeul, CQuaternion* aftq, D3DXVECTOR3* afteul );


	enum { IDD = IDD_BONEPROPDLG };

BEGIN_MSG_MAP(CBonePropDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_APPLYROT, OnApplyRot)
	COMMAND_ID_HANDLER(IDC_APPLYMV, OnApplyMv)
	COMMAND_ID_HANDLER(IDC_APPLYSC, OnApplySc)
	COMMAND_ID_HANDLER(IDC_APPLYLIM, OnApplyLim)

	COMMAND_ID_HANDLER(IDC_SETSYMM, OnSetSymm)

END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnApplyRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMv(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplySc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyLim(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSetSymm(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	void SetWnd();
	int ParamsToDlg();
	int InitParams();
	
	int GetLimitedEul( float starteul, float endeul, double lim0, double lim1, float* limeul );


public:
	int m_serialno;

	D3DXVECTOR3 m_rot;
	DVEC3 m_lim0;
	DVEC3 m_lim1;
	D3DXVECTOR3 m_mv;
	D3DXVECTOR3 m_sc;

	char* m_nameptr;
	int m_ignorelim;

	int m_zakind;
private:
	CMyD3DApplication* m_papp;

	CWindow m_rotx_wnd;
	CWindow m_roty_wnd;
	CWindow m_rotz_wnd;

	CWindow m_limx0_wnd;
	CWindow m_limx1_wnd;
	CWindow m_limy0_wnd;
	CWindow m_limy1_wnd;
	CWindow m_limz0_wnd;
	CWindow m_limz1_wnd;


	CWindow m_mvx_wnd;
	CWindow m_mvy_wnd;
	CWindow m_mvz_wnd;

	CWindow m_scx_wnd;
	CWindow m_scy_wnd;
	CWindow m_scz_wnd;

	CWindow m_name_wnd;

	CWindow m_ignlim_wnd;

	CWindow m_za_wnd;

	CWindow m_dlg_wnd;
	
	int m_cmdshow;


};

#endif //__BonePropDlg_H_
