// AxisDlg.h : CAxisDlg の宣言

#ifndef __AXISDLG_H_
#define __AXISDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

//#include <commctrl.h>

class CShdHandler;
class CMotHandler;
class CD3DFont;


enum {
	ARROWMODE_VERT,
	ARROWMODE_CUSTOM,
	ARROWMODE_CROSS,
	ARROWMODE_MAX
};



/////////////////////////////////////////////////////////////////////////////
// CAxisDlg
class CAxisDlg : 
	public CAxDialogImpl<CAxisDlg>
{
public:
	CAxisDlg( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp );
	~CAxisDlg();

	enum { IDD = IDD_AXISDLG };

BEGIN_MSG_MAP(CAxisDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroyDlg)

	//NOTIFY_HANDLER(IDC_SL_XZ, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_XZ)
	//NOTIFY_HANDLER(IDC_SL_Y, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_Y)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)


	COMMAND_ID_HANDLER(IDC_RADIO_VERT, OnRadioVert)
	COMMAND_ID_HANDLER(IDC_RADIO_CUSTOM, OnRadioCustom)
	COMMAND_ID_HANDLER(IDC_RADIO_CROSS, OnRadioCross)


END_MSG_MAP()


	//BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);


// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	//LRESULT OnReleasedcaptureSl_XZ(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	//LRESULT OnReleasedcaptureSl_Y(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnRadioVert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioCustom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioCross(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int CreateSwapChain( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp );
	int DestroySwapChain();
	int Render();

	int ShowDlg( int cmdshow );
	int SetInvalidateFlag( int flag );

	int SetRenderParams( float degy, float degxz, GUARDBAND gband, 
		CShdHandler* groundsh, CMotHandler* groundmh, CShdHandler* arrowsh, CMotHandler* arrowmh,
		CD3DFont* pfont );

	int DestroyTimer();

public:
	float		m_arrow_degxz;
	float		m_arrow_degy;
	int			m_arrowmode;
	int			m_timerworking;

private:
	RECT m_rc;

	CWindow m_dlg_wnd;
	CWindow m_view_wnd;
	CWindow m_sl_xz_wnd;
	CWindow m_sl_y_wnd;
	CWindow m_text_xz_wnd;
	CWindow m_text_y_wnd;

	LPDIRECT3DDEVICE9 m_pd3ddev;
	D3DPRESENT_PARAMETERS m_d3dpp;

	IDirect3DSwapChain9* m_pSwapChain;
	IDirect3DSurface9* m_pZSurf;
	IDirect3DSurface9* m_pBackBuffer;

	D3DVIEWPORT9			m_viewport;

	int m_timerid;
	int m_createflag;
	int m_cmdshow;
	int m_invalidateflag;


	D3DXMATRIX	m_matWorld;
	D3DXMATRIX	m_matView;
	D3DXMATRIX	m_matProjX;
	//D3DXMATRIX	m_matProj;
	float		m_proj_fov, m_proj_far, m_proj_near;
	float		m_degy, m_degxz, m_camdist, m_eye_y;

	CShdHandler* m_ground_sh;
	CShdHandler* m_arrow_sh;

	CMotHandler* m_ground_mh;
	CMotHandler* m_arrow_mh;

	GUARDBAND	m_smallgband;
	CD3DFont*	m_pfont;

private:
	void SetWnd();
	int ParamsToDlg();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );


};

#endif //__AXISDLG_H_
