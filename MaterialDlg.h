// MaterialDlg.h : CMaterialDlg の宣言

#ifndef __MaterialDlg_H_
#define __MaterialDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

#include <D3DX9.h>

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg

class CIniData;
class CLightData;
class CMyD3DApplication;
class CMQOMaterial;

class CMaterialDlg : 
	public CAxDialogImpl<CMaterialDlg>
{
public:
	//CMaterialDlg( int alpha, int beta, COLORREF srccol, int srclblend );
	CMaterialDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl );
	~CMaterialDlg();

	enum { IDD = IDD_MATERIALDLG };

BEGIN_MSG_MAP(CMaterialDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFDIFFUSE, OnRefDiffuse)
	COMMAND_ID_HANDLER(IDC_REFSPECULAR, OnRefSpecular)
	COMMAND_ID_HANDLER(IDC_REFAMBIENT, OnRefAmbient)
	COMMAND_ID_HANDLER(IDC_REFEMISSIVE, OnRefEmissive)

	COMMAND_ID_HANDLER(IDC_LIST1, OnSelchangeList1)
	COMMAND_ID_HANDLER(IDC_APPLYALPHA, OnApplyAlpha)
	COMMAND_ID_HANDLER(IDC_APPLYALPHATEST, OnApplyAlphaTest)
	COMMAND_ID_HANDLER(IDC_CHANGEMAT, OnApplyMaterial)
	COMMAND_ID_HANDLER(IDC_TEXTURE, OnTexture)


	//MESSAGE_HANDLER(WM_PAINT, OnPaint)

	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroyDlg)

	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)


END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnRefDiffuse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefSpecular(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefAmbient(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefEmissive(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangeList1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyAlpha(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyAlphaTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMaterial(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnTexture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnPaintOrg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	int ShowDlg( int cmdshow );
	int CreateSwapChain( LPDIRECT3DDEVICE9 pdev );
	int DestroySwapChain();
	CMQOMaterial* GetMaterialFromNo( CMQOMaterial* mathead, int matno );

private:
	int SetWnd();
	int ParamsToDlg();

	int Render();
	int PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush, COLORREF srccol );

	int SetWorkMaterial2Sph();
	int SetIniMaterial2Sph();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int CreateTimer();
	int DestroyTimer();

	int InitList();
	int Material2Dlg( int matno );

	int EnableAlphaWnd( int exttexmode );

public:
	MATERIAL0 m_wmaterial;
	float m_walpha;

	int m_blendmode;
	char m_alphatest0;
	char m_alphatest1;
	unsigned char m_alphaval0;
	unsigned char m_alphaval1;

private:
	HDC m_diffuse_dc;
	HDC m_specular_dc;
	HDC m_ambient_dc;
	HDC m_emissive_dc;

	HBRUSH m_diffuse_b;
	HBRUSH m_specular_b;
	HBRUSH m_ambient_b;
	HBRUSH m_emissive_b;

	int m_modalflag;

	int m_workkind;

	int m_timerid;
	int m_createflag;
	int m_comboflag;
	int	m_cmdshow;

	CMyD3DApplication* m_app;
	CIniData* m_inidata;
	CLightData* m_light;


	LPDIRECT3DDEVICE9 m_pdev; 
	D3DPRESENT_PARAMETERS m_d3dpp;

	CWindow m_dlg_wnd;
	CWindow m_view_wnd;
	CWindow m_diffuse_wnd;
	CWindow m_specular_wnd;
	CWindow m_ambient_wnd;
	CWindow m_emissive_wnd;
	CWindow m_sl_power_wnd;
	CWindow m_power_wnd;
	CWindow m_list_wnd;
	CWindow m_matno_wnd;
	CWindow m_alpha_wnd;

	CWindow m_combo1_wnd;
	CWindow m_alphatest0_wnd;
	CWindow m_alphatest1_wnd;
	CWindow m_alphaval0_wnd;
	CWindow m_alphaval1_wnd;


	int m_selmaterial;

	RECT m_rc;
	IDirect3DSwapChain9* m_pSwapChain;
	IDirect3DSurface9* m_pZSurf;
	IDirect3DSurface9* m_pBackBuffer;
	D3DVIEWPORT9			m_viewport;



	GUARDBAND	m_smallgband;
	D3DXMATRIX	m_matWorld;
	D3DXMATRIX	m_matView;
	D3DXMATRIX	m_matProjX;
//	D3DXMATRIX	m_matProj;
	float		m_proj_fov, m_proj_far, m_proj_near;
	float		m_camdist, m_eye_y;

};

#endif //__MaterialDlg_H_
