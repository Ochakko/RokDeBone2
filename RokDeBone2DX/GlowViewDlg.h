// GlowViewDlg.h : CGlowViewDlg の宣言

#ifndef __GlowViewDlg_H_
#define __GlowViewDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

#include <d3dx9.h>

/////////////////////////////////////////////////////////////////////////////
// CGlowViewDlg

class CIniData;
class CLightData;
class CMyD3DApplication;

class CGlowViewDlg : 
	public CAxDialogImpl<CGlowViewDlg>
{
public:
	CGlowViewDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl );
	~CGlowViewDlg();

	enum { IDD = IDD_GLOWVIEWDLG };

BEGIN_MSG_MAP(CGlowViewDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	//MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroyDlg)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	int ShowDlg( int cmdshow );
	int CreateSwapChain( LPDIRECT3DDEVICE9 pdev );
	int DestroySwapChain();

private:
	int SetWnd();
	int ParamsToDlg();

	int Render();

	int CreateTimer();
	int DestroyTimer();
	int DestroyRTTexElem( RTTEXELEM* prtte );

	int CreateRenderTargetTexture( LPDIRECT3DDEVICE9 pdev, SIZE size, int srcfmt, int needz, RTTEXELEM* prtte );


private:
	int m_timerid;
	int m_createflag;
	int	m_cmdshow;

	CMyD3DApplication* m_app;
	CIniData* m_inidata;
	CLightData* m_light;


	LPDIRECT3DDEVICE9 m_pdev; 
	D3DPRESENT_PARAMETERS m_d3dpp;

	CWindow m_dlg_wnd;
	CWindow m_view_wnd;

	RECT m_rc;
	IDirect3DSwapChain9* m_pSwapChain;
	IDirect3DSurface9* m_pZSurf;
	IDirect3DSurface9* m_pBackBuffer;
	D3DVIEWPORT9 m_viewport;
	D3DVIEWPORT9 m_viewport_org;
	D3DVIEWPORT9 m_viewport_small;

	RTTEXELEM	m_RTOrg;
	RTTEXELEM	m_RTGlow;
	RTTEXELEM	m_RTGlowSmall;
	RTTEXELEM	m_RTWork;
	RTTEXELEM	m_RTGauss;
	RTTEXELEM	m_RTFinal;


	GUARDBAND	m_smallgband;
	D3DXMATRIX	m_matWorld;
	D3DXMATRIX	m_matView;
	D3DXMATRIX	m_matProjX;
	float		m_proj_fov, m_proj_far, m_proj_near;
	float		m_camdist, m_eye_y;

};

#endif //__GlowViewDlg_H_
