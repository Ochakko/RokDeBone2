// LightDlg.h : CLightDlg の宣言

#ifndef __LIGHTDLG_H_
#define __LIGHTDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

#include <D3DX9.h>

/////////////////////////////////////////////////////////////////////////////
// CLightDlg

class CIniData;
class CLightData;
class CMyD3DApplication;

class CLightDlg : 
	public CAxDialogImpl<CLightDlg>
{
public:
	//CLightDlg( int alpha, int beta, COLORREF srccol, int srclblend );
	CLightDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl );
	~CLightDlg();

	enum { IDD = IDD_LIGHTDLG };

BEGIN_MSG_MAP(CLightDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_COLORREF, OnColorRef)

	COMMAND_ID_HANDLER(IDC_INVERSE, OnInverse)

	COMMAND_ID_HANDLER(IDC_MULT, OnMult)
	COMMAND_ID_HANDLER(IDC_SCREEN, OnScreen)
	COMMAND_ID_HANDLER(IDC_OVERLAY, OnOverlay)
	COMMAND_ID_HANDLER(IDC_HARDLIGHT, OnHardlight)
	COMMAND_ID_HANDLER(IDC_DODGE, OnDodge)


	//MESSAGE_HANDLER(WM_PAINT, OnPaint)

	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroyDlg)

	//COMMAND_ID_HANDLER(IDC_VIEW, OnClickView)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)

	MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterMove)
	MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitMove)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnColorRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnInverse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnMult(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnScreen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnOverlay(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHardlight(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDodge(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);



	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	int ShowDlg( int cmdshow );
	int ParamsToLight();
	int CreateSwapChain( LPDIRECT3DDEVICE9 pdev );
	int DestroySwapChain();

private:
	int CalcLightVec( int alpha, int beta, D3DXVECTOR3* dstvec );
	int SetWnd();
	int ParamsToDlg();

	int Render();
	int PickFaceAndSetLight( int srcx, int srcy );
	int SetIniMaterial2Sph();

public:
	D3DXVECTOR3 m_lightvec;
	COLORREF	m_color;
	int m_lblend;

private:
	HDC m_dc;
	HBRUSH m_brush;

	int m_mcap;
	int m_lbutton;
	int m_ref;
	int m_moving;

	int m_timerid;
	int m_createflag;
	int	m_cmdshow;

	CMyD3DApplication* m_app;
	CIniData* m_inidata;
	CLightData* m_light;
	int m_inverse;

	LPDIRECT3DDEVICE9 m_pdev; 
	D3DPRESENT_PARAMETERS m_d3dpp;

	CWindow m_dlg_wnd;
	CWindow m_color_wnd;
	CWindow m_text_col_wnd;
	CWindow m_view_wnd;
	CWindow m_inverse_wnd;

	RECT m_rc;
	IDirect3DSwapChain9* m_pSwapChain;
	IDirect3DSurface9* m_pZSurf;
	IDirect3DSurface9* m_pBackBuffer;
	D3DVIEWPORT9			m_viewport;



	GUARDBAND	m_smallgband;
	D3DXMATRIX	m_matWorld;
	D3DXMATRIX	m_matView;
	D3DXMATRIX	m_matProjX;
	//D3DXMATRIX	m_matProj;
	float		m_proj_fov, m_proj_far, m_proj_near;
	float		m_camdist, m_eye_y;

};

#endif //__LIGHTDLG_H_
