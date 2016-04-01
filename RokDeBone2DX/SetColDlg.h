// SetColDlg.h : CSetColDlg の宣言

#ifndef __SetColDlg_H_
#define __SetColDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

#include <D3DX9.h>

/////////////////////////////////////////////////////////////////////////////
// CSetColDlg

class CIniData;
class CLightData;
class CMyD3DApplication;

#define MATERIALUNDOMAX	20


typedef struct tag_materialundo
{
	int partno;
	int vertno[PICKVERTMAX];
	int vertnum;
	MATERIAL0 mat;
}MATERIALUNDO;


class CSetColDlg : 
	public CAxDialogImpl<CSetColDlg>
{
public:
	//CSetColDlg( int alpha, int beta, COLORREF srccol, int srclblend );
	CSetColDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl );
	~CSetColDlg();

	enum { IDD = IDD_SETCOLDLG };

BEGIN_MSG_MAP(CSetColDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFDIFFUSE, OnRefDiffuse)
	COMMAND_ID_HANDLER(IDC_REFSPECULAR, OnRefSpecular)
	COMMAND_ID_HANDLER(IDC_REFAMBIENT, OnRefAmbient)
	COMMAND_ID_HANDLER(IDC_REFEMISSIVE, OnRefEmissive)

	COMMAND_ID_HANDLER(IDC_RADIO_SET, OnRadioSet)
	COMMAND_ID_HANDLER(IDC_RADIO_SPOIT, OnRadioSpoit)

	COMMAND_ID_HANDLER(IDC_CHECK_TEX, OnCheckTexture)
	COMMAND_ID_HANDLER(IDC_UNDO, OnUndo)


	COMMAND_ID_HANDLER(IDC_PALETTE1, OnPalette1)
	COMMAND_ID_HANDLER(IDC_PALETTE2, OnPalette2)
	COMMAND_ID_HANDLER(IDC_PALETTE3, OnPalette3)
	COMMAND_ID_HANDLER(IDC_PALETTE4, OnPalette4)
	COMMAND_ID_HANDLER(IDC_PALETTE5, OnPalette5)
	COMMAND_ID_HANDLER(IDC_PALETTE6, OnPalette6)
	COMMAND_ID_HANDLER(IDC_PALETTE7, OnPalette7)
	COMMAND_ID_HANDLER(IDC_PALETTE8, OnPalette8)
	COMMAND_ID_HANDLER(IDC_PALETTE9, OnPalette9)
	COMMAND_ID_HANDLER(IDC_PALETTE10, OnPalette10)
	COMMAND_ID_HANDLER(IDC_PALETTE11, OnPalette11)
	COMMAND_ID_HANDLER(IDC_PALETTE12, OnPalette12)
	COMMAND_ID_HANDLER(IDC_PALETTE13, OnPalette13)
	COMMAND_ID_HANDLER(IDC_PALETTE14, OnPalette14)
	COMMAND_ID_HANDLER(IDC_PALETTE15, OnPalette15)
	COMMAND_ID_HANDLER(IDC_PALETTE16, OnPalette16)


	COMMAND_ID_HANDLER(IDC_FILESAVE, OnFileSave)
	COMMAND_ID_HANDLER(IDC_FILELOAD, OnFileLoad)


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


	LRESULT OnRadioSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioSpoit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCheckTexture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnPalette1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette4(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette5(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette6(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette7(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette8(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette9(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette10(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette11(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette12(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette13(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette14(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette15(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPalette16(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnPaintOrg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	int ShowDlg( int cmdshow );
	int CreateSwapChain( LPDIRECT3DDEVICE9 pdev );
	int DestroySwapChain();

	int OnClickVert( int clipart, int* clivert, int clivertnum );
	int InitUndoBuf();


private:
	int SetWnd();
	int ParamsToDlg();

	int Render();
	int PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush, COLORREF srccol );

	int SetWorkMaterial2Sph();
	int SetIniMaterial2Sph();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int AddUndoBuf( int partno, int* vertno, int vertnum, MATERIAL0 undomat );

	int CreateTimer();
	int DestroyTimer();

	int OnPaletteFunc( int palno );

	int FileLoadFunc( char* filename );
	int FileSaveFunc( char* filename );

public:
	MATERIAL0 m_wmaterial;
	MPALETTE m_mpalette[ MPALETTENUM ];

private:
	int m_loadfileflag;

	HDC m_diffuse_dc;
	HDC m_specular_dc;
	HDC m_ambient_dc;
	HDC m_emissive_dc;
	HDC m_palette_dc[ MPALETTENUM ];

	HBRUSH m_diffuse_b;
	HBRUSH m_specular_b;
	HBRUSH m_ambient_b;
	HBRUSH m_emissive_b;
	HBRUSH m_palette_b[ MPALETTENUM ];

	int m_modalflag;

	MATERIALUNDO m_undo[ MATERIALUNDOMAX ];
	int m_undopos;

	int m_workkind;

	int m_timerid;
	int m_createflag;
	int	m_cmdshow;
	int m_enabletexture;

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

	CWindow m_palette_wnd[ MPALETTENUM ];
	CWindow m_pname_wnd[ MPALETTENUM ];


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

#endif //__SetColDlg_H_
