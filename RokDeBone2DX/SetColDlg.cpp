// SetColDlg.cpp : CSetColDlg のインプリメンテーション
#include "stdafx.h"
#include "SetColDlg.h"

#include <math.h>
#include <coef.h>

#define DBGH
#include "dbg.h"

#include "ColDlg.h"

#include "viewer.h"
#include "inidata.h"
#include <LightData.h>
#include "MaterialPaletteFile.h"
#include "SetCol2Dlg.h"

#include <treehandler2.h>
#include <shdhandler.h>
#include <mothandler.h>

#include <crtdbg.h>

#define RENDERCOLTIMER	11001

//------- extern
extern CIniData* g_inidata;
extern CColDlg g_coldlg;
extern int g_rendertarget;
extern float g_specular;
extern float g_ambient;
extern ID3DXEffect*	g_pEffect;
extern D3DXHANDLE g_hmView;
extern D3DXHANDLE g_hmProj;

//------- static
static int paletteid[ MPALETTENUM ] = {
	IDC_PALETTE1, IDC_PALETTE2, IDC_PALETTE3, IDC_PALETTE4,
	IDC_PALETTE5, IDC_PALETTE6, IDC_PALETTE7, IDC_PALETTE8,
	IDC_PALETTE9, IDC_PALETTE10, IDC_PALETTE11, IDC_PALETTE12,
	IDC_PALETTE13, IDC_PALETTE14, IDC_PALETTE15, IDC_PALETTE16
};

static int pnameid[ MPALETTENUM ] = {
	IDC_PNAME1, IDC_PNAME2, IDC_PNAME3, IDC_PNAME4,
	IDC_PNAME5, IDC_PNAME6, IDC_PNAME7, IDC_PNAME8,
	IDC_PNAME9, IDC_PNAME10, IDC_PNAME11, IDC_PNAME12,
	IDC_PNAME13, IDC_PNAME14, IDC_PNAME15, IDC_PNAME16
};


enum {
	WKIND_PAINT,
	WKIND_SPOIT,
	WKIND_MAX
};

/////////////////////////////////////////////////////////////////////////////
// CSetColDlg

CSetColDlg::CSetColDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl )
{

	m_loadfileflag = 0;

	m_modalflag = 0;
	m_timerid = 0;
	m_workkind = WKIND_PAINT;

	m_createflag = 0;
	m_cmdshow = SW_HIDE;
	m_enabletexture = 1;

	m_pdev = pdev;
	m_app = papp;
	m_inidata = pini;
	m_light = pl;
	m_d3dpp = papp->m_d3dpp;


	m_wmaterial.diffuse = RGB( 255, 255, 255 );
	m_wmaterial.specular = RGB( 0, 0, 0 );
	m_wmaterial.ambient = RGB( 64, 64, 64 );
	m_wmaterial.emissive = RGB( 0, 0, 0 );
	m_wmaterial.power = 5.0f;


////
	m_pSwapChain = 0;
	m_pZSurf = 0;
	m_pBackBuffer = 0;

	m_rc.top = 0;
	m_rc.bottom = 0;
	m_rc.left = 0;
	m_rc.right = 0;

    m_viewport.X = 0;
    m_viewport.Y = 0;
    m_viewport.Height = 0;
    m_viewport.Width =  0;
    m_viewport.MaxZ = 1.0f;
    m_viewport.MinZ = 0.0f;
//////
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matProjX );
	//D3DXMatrixIdentity( &m_matProj );

	m_proj_fov = 45.0f * (float)DEG2PAI;

	m_proj_far = 10000.0f;
	m_proj_near = 100.0f;
	
	//m_degy = 0.0f;
	//m_degxz = 0.0f;
	//m_camdist = 2900.0f;
	//m_eye_y = 750.0f;

	m_camdist = 1100.0f;
	m_eye_y = 0.0f;


	ZeroMemory( &m_smallgband, sizeof( GUARDBAND ) );


	m_diffuse_dc = NULL;
	m_specular_dc = NULL;
	m_ambient_dc = NULL;
	m_emissive_dc = NULL;

	m_diffuse_b = NULL;
	m_specular_b = NULL;
	m_ambient_b = NULL;
	m_emissive_b = NULL;

	int palno;
	for( palno = 0; palno < MPALETTENUM; palno++ ){
		m_palette_dc[ palno ] = NULL;
		m_palette_b[ palno ] = NULL;
	}


///////
	InitUndoBuf();


}

CSetColDlg::~CSetColDlg()
{
	DestroySwapChain();
}

int CSetColDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_view_wnd = GetDlgItem( IDC_VIEW );
	m_diffuse_wnd = GetDlgItem( IDC_VDIFFUSE );
	m_specular_wnd = GetDlgItem( IDC_VSPECULAR );
	m_ambient_wnd = GetDlgItem( IDC_VAMBIENT );
	m_emissive_wnd = GetDlgItem( IDC_VEMISSIVE );
	m_sl_power_wnd = GetDlgItem( IDC_SL_POWER );
	m_power_wnd = GetDlgItem( IDC_POWER );

	int pno;
	for( pno = 0; pno < MPALETTENUM; pno++ ){
		m_palette_wnd[pno] = GetDlgItem( paletteid[pno] );
		_ASSERT( m_palette_wnd[pno] );
		m_pname_wnd[pno] = GetDlgItem( pnameid[pno] );
		_ASSERT( m_pname_wnd[pno] );
	}


	m_sl_power_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_power_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)10000 );
	m_sl_power_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)((int)(m_wmaterial.power * 100.0f)) );
	SetSliderTic( m_sl_power_wnd, 1000, 0, 10000 );

////////

	m_diffuse_dc = ::GetDC( m_diffuse_wnd );
	m_specular_dc = ::GetDC( m_specular_wnd );
	m_ambient_dc = ::GetDC( m_ambient_wnd );
	m_emissive_dc = ::GetDC( m_emissive_wnd );
	for( pno = 0; pno < MPALETTENUM; pno++ ){
		m_palette_dc[ pno ] = ::GetDC( m_palette_wnd[pno] );
	}

	return 0;
}
void CSetColDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	// !!! minticとmaxticには、自動的にticが入るので設定はしない。(してはいけない)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		dstwnd.SendMessage( TBM_SETTIC, 0, (LPARAM)(LONG)curtic );
		curtic += ticfreq;
	}
}


int CSetColDlg::ParamsToDlg()
{

	switch( m_workkind ){
	case WKIND_PAINT:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SET, IDC_RADIO_SPOIT, IDC_RADIO_SET );
		break;
	case WKIND_SPOIT:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SET, IDC_RADIO_SPOIT, IDC_RADIO_SPOIT );
		break;
	default:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SET, IDC_RADIO_SPOIT, IDC_RADIO_SET );
		m_workkind = WKIND_PAINT;
		break;
	}

	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", m_wmaterial.power );
	m_power_wnd.SetWindowText( tempchar );

	m_sl_power_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)((int)(m_wmaterial.power * 100.0f)) );


	if( m_enabletexture == 0 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_TEX, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_TEX, BST_UNCHECKED );
	}

	int palno;
	for( palno = 0; palno < MPALETTENUM; palno++ ){
		m_pname_wnd[palno].SetWindowText( m_mpalette[palno].name );
	}


	return 0;
}

LRESULT CSetColDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//DbgOut( "setcoldlg : OnInitDialog \r\n" );

	int ret = 0;
	ret = SetWnd();
	_ASSERT( !ret );

	ret = ParamsToDlg();
	_ASSERT( !ret );


	if( m_createflag == 0 ){
		int ret;
		ret = CreateSwapChain( m_pdev );
		if( ret ){
			DbgOut( "SetColDlg : OnInitDailog : CreateSwapChain error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
	}

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSetColDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_enabletexture = 1;
	if( m_app->m_shandler ){
		m_app->m_shandler->EnableTexture( -1, m_enabletexture );
	}
	ParamsToDlg();

	//EndDialog(wID);
	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_app->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	return 0;
}

LRESULT CSetColDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_enabletexture = 1;
	if( m_app->m_shandler ){
		m_app->m_shandler->EnableTexture( -1, m_enabletexture );
	}
	ParamsToDlg();

	//EndDialog(wID);
	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_app->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	return 0;
}

LRESULT CSetColDlg::OnRadioSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_workkind = WKIND_PAINT;
	ParamsToDlg();
	return 0;
}
LRESULT CSetColDlg::OnRadioSpoit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_workkind = WKIND_SPOIT;
	ParamsToDlg();
	return 0;
}



LRESULT CSetColDlg::OnRefDiffuse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	DestroyTimer();
	m_modalflag = 1;

	ret = g_coldlg.Choose( m_dlg_wnd, &m_wmaterial.diffuse );
	//if( ret ){
	//	BOOL dummy;
	//	OnPaintOrg( 0, 0, 0, dummy );
	//}

	if( m_diffuse_b )
		DeleteObject( m_diffuse_b );
	m_diffuse_b = ::CreateSolidBrush( m_wmaterial.diffuse );


	m_modalflag = 0;

	ParamsToDlg();
	CreateTimer();

	return 0;
}
LRESULT CSetColDlg::OnRefSpecular(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	DestroyTimer();
	m_modalflag = 1;

	ret = g_coldlg.Choose( m_dlg_wnd, &m_wmaterial.specular );
	//if( ret ){
	//	BOOL dummy;
	//	OnPaintOrg( 0, 0, 0, dummy );
	//}

	if( m_specular_b )
		DeleteObject( m_specular_b );
	m_specular_b = ::CreateSolidBrush( m_wmaterial.specular );


	m_modalflag = 0;
	ParamsToDlg();
	CreateTimer();

	return 0;
}
LRESULT CSetColDlg::OnRefAmbient(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	DestroyTimer();
	m_modalflag = 1;

	ret = g_coldlg.Choose( m_dlg_wnd, &m_wmaterial.ambient );
	//if( ret ){
	//	BOOL dummy;
	//	OnPaintOrg( 0, 0, 0, dummy );
	//}

	if( m_ambient_b )
		DeleteObject( m_ambient_b );
	m_ambient_b = ::CreateSolidBrush( m_wmaterial.ambient );


	m_modalflag = 0;

	ParamsToDlg();
	CreateTimer();
	return 0;
}
LRESULT CSetColDlg::OnRefEmissive(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	DestroyTimer();
	m_modalflag = 1;

	ret = g_coldlg.Choose( m_dlg_wnd, &m_wmaterial.emissive );
	//if( ret ){
	//	BOOL dummy;
	//	OnPaintOrg( 0, 0, 0, dummy );
	//}

	if( m_emissive_b )
		DeleteObject( m_emissive_b );
	m_emissive_b = ::CreateSolidBrush( m_wmaterial.emissive );


	m_modalflag = 0;

	ParamsToDlg();
	CreateTimer();
	return 0;
}



LRESULT CSetColDlg::OnPaintOrg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_modalflag == 1 )
		return 0;

	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	PaintRect( m_diffuse_wnd, m_diffuse_dc, m_diffuse_b, m_wmaterial.diffuse );
	PaintRect( m_specular_wnd, m_specular_dc, m_specular_b, m_wmaterial.specular );
	PaintRect( m_ambient_wnd, m_ambient_dc, m_ambient_b, m_wmaterial.ambient );
	PaintRect( m_emissive_wnd, m_emissive_dc, m_emissive_b, m_wmaterial.emissive );
	
	int palno;
	for( palno = 0; palno < MPALETTENUM; palno++ ){
		PaintRect( m_palette_wnd[palno], m_palette_dc[palno], m_palette_b[palno], m_mpalette[palno].mat.diffuse );
	}

	//Render();

	return 0;
}

int CSetColDlg::PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush, COLORREF srccol )
{
	if( m_modalflag == 1 )
		return 0;

	if( !hdc )
		return 0;

	//HBRUSH brush;
	//brush = ::CreateSolidBrush( srccol );
	//_ASSERT( brush != NULL );
	if( !brush )
		return 0;

	BOOL bret;
	RECT winrect;
	bret = srcwnd.GetWindowRect( &winrect );
	if( !bret )
		return 0;

	RECT clirect;
	//clirect.top = 0;
	//clirect.left = 0;
	//clirect.bottom = winrect.bottom - winrect.top;
	//clirect.right = winrect.right - winrect.left;
	bret = srcwnd.GetClientRect( &clirect );
	if( !bret )
		return 0;

	HBRUSH hOldBrush = (HBRUSH)SelectObject( hdc, brush );
	bret = ::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret )
		return 0;

	//::SelectObject( hdc, hOldBrush );

	return 0;
}

int CSetColDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;	
	ShowWindow( m_cmdshow );
		
	m_enabletexture = 1;
	if( m_app->m_shandler ){
		m_app->m_shandler->EnableTexture( -1, m_enabletexture );
	}

	if( m_cmdshow != SW_HIDE ){
		InitUndoBuf();

		if( m_app->m_shandler ){
			m_app->m_shandler->SetMorphDispFlag( 0 );
		}

		int ret;
		ret = FileLoadFunc( "MaterialPalette.mpl" );
		_ASSERT( !ret );

		m_loadfileflag = 1;

	}else{
		if( m_app->m_shandler ){
			m_app->m_shandler->SetMorphDispFlag( 1 );
		}

		if( m_loadfileflag == 1 ){
			int ret;
			ret = FileSaveFunc( "MaterialPalette.mpl" );
			_ASSERT( !ret );
		}
	}


	ParamsToDlg();

	return 0;	
}

LRESULT CSetColDlg::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//HDC hdc;
	//hdc = ((LPDRAWITEMSTRUCT)lParam)->hDC;
	//_ASSERT( hdc );
	//int id;
	//id = (int)wParam;

	return 0;
}

LRESULT CSetColDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	int timerid = (int)wParam;
	if( timerid == m_timerid ){

		if( (m_modalflag == 0) && (m_cmdshow != SW_HIDE) ){
			BOOL dummy;
			OnPaintOrg( 0, 0, 0, dummy );

			ret = Render();
			_ASSERT( !ret );
		}

	}

	return 0;
}

LRESULT CSetColDlg::OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	DestroyTimer();

	if( m_diffuse_dc ){
		::ReleaseDC( m_diffuse_wnd, m_diffuse_dc );
		m_diffuse_dc = NULL;
	}
	if( m_specular_dc ){
		::ReleaseDC( m_specular_wnd, m_specular_dc );
		m_specular_dc = NULL;
	}
	if( m_ambient_dc ){
		::ReleaseDC( m_ambient_wnd, m_ambient_dc );
		m_ambient_dc = NULL;
	}
	if( m_emissive_dc ){
		::ReleaseDC( m_emissive_wnd, m_emissive_dc );
		m_emissive_dc = NULL;
	}

	int pno;
	for( pno = 0; pno < MPALETTENUM; pno++ ){
		::ReleaseDC( m_palette_wnd[pno], m_palette_dc[pno] );
		m_palette_dc[pno] = NULL;
	}

	return 0;
}


int CSetColDlg::CreateSwapChain( LPDIRECT3DDEVICE9 pdev )
{
	HRESULT hres;

	m_pdev = pdev;

	if( m_createflag == 1 )
		return 0;

	if( !m_pdev ){
		DbgOut( "SetColDlg : CreateSwapChain : 3ddevice NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	DestroySwapChain();//!!!! 念のため。


	m_view_wnd = GetDlgItem( IDC_VIEW );
	m_view_wnd.GetClientRect( &m_rc );

	m_d3dpp = m_app->m_d3dpp;
	m_d3dpp.BackBufferWidth = m_rc.right - m_rc.left;
	m_d3dpp.BackBufferHeight = m_rc.bottom - m_rc.top;
	m_d3dpp.hDeviceWindow = (HWND)m_view_wnd;
	m_d3dpp.EnableAutoDepthStencil = FALSE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.BackBufferCount = 1;//!!!!

	hres = m_pdev->CreateAdditionalSwapChain( &m_d3dpp, &m_pSwapChain );
	if( hres != D3D_OK ){
		DbgOut( "SetColDlg : CreateSwapChain : dev CreateAdditionalSwapChain error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	hres = m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "SetColDlg : CreateSwapChain : swapchain GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DFORMAT fmt;
	fmt = D3DFMT_D16;	
	hres = m_pdev->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, fmt, m_d3dpp.MultiSampleType, 0, FALSE, &m_pZSurf, NULL );
	if( hres != D3D_OK ){
		DbgOut( "SetColDlg : CreateSwapChain : dev CreateDepthStencilSurface error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}

	CreateTimer();
	


    float aspect;
	aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH( &m_matProjX, m_proj_fov, aspect, m_proj_near, m_proj_far );


//	D3DXMatrixIdentity( &m_matProj );
//	float    h, w, Q;
//	w = (float)( 1.0f / tan( m_proj_fov * 0.5f ));
//	h = w;
//	Q = m_proj_far /(m_proj_far - m_proj_near); 
//	m_matProj._11 = w;
//	m_matProj._22 = h;
//	m_matProj._33 = Q;
//	m_matProj._34 = 1.0f;
//	m_matProj._43 = -Q * m_proj_near;//101.01


	m_createflag = 1;

	return 0;
}
int CSetColDlg::DestroySwapChain()
{
	if( m_pZSurf ){
		m_pZSurf->Release();
		m_pZSurf = 0;
	}

	if( m_pBackBuffer ){
		m_pBackBuffer->Release();
		m_pBackBuffer = 0;
	}


	if( m_pSwapChain ){
		m_pSwapChain->Release();
		m_pSwapChain = 0;
	}

	m_createflag = 0;

	return 0;
}

int CSetColDlg::Render()
{
	if( m_modalflag == 1 )
		return 0;


	HRESULT hres;
	int ret;

//DbgOut( "SetColDlg : Render : %d %x %d, %d\r\n", m_cmdshow, m_pdev, m_createflag, SW_HIDE );

	if( (m_cmdshow == SW_HIDE) || !m_pdev || !m_createflag )// || m_invalidateflag )
		return 0;
/////////////////

	float extmult = 1.20f;
	m_smallgband.maxrhw = m_app->m_gband.maxrhw;

	m_smallgband.left = max( m_app->m_gband.left, -extmult * (m_rc.right - m_rc.left) );
	m_smallgband.top = max( m_app->m_gband.top, -extmult * (m_rc.bottom - m_rc.top) );
	m_smallgband.right = min( m_app->m_gband.right, extmult * (m_rc.right - m_rc.left) );
	m_smallgband.bottom = min( m_app->m_gband.bottom, extmult * (m_rc.bottom - m_rc.top) );


////////transform	
    D3DXVECTOR3 vEyePt;
    D3DXVECTOR3 targetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	float radxz, rady;
	//radxz = m_app->m_degxz * (float)DEG2PAI;
	//if( (m_app->m_degy != 90.0f) && (m_app->m_degy != -90.0f) )
	//	rady = m_app->m_degy * (float)DEG2PAI;
	//else if(m_app->m_degy == 90.0f)
	//	rady = 89.995f * (float)DEG2PAI;
	//else if( m_app->m_degy == -90.0f)
	//	rady = -89.995f * (float)DEG2PAI;

	radxz = 0.0f;
	rady = 0.0f;


    vEyePt.x = sinf(radxz) * m_camdist * cosf( rady );// + m_targetpos.x;
    vEyePt.y = m_eye_y + m_camdist * sinf( rady );
	vEyePt.z = -cosf(radxz) * m_camdist * cosf( rady );// + m_targetpos.z;

    // Set the app view matrix for normal viewing
	D3DXMatrixLookAtLH( &m_matView, &vEyePt, &targetpos, &vUpVec );

    DWORD dwClipWidth  = ( m_rc.right - m_rc.left ) /2;
    DWORD dwClipHeight = ( m_rc.bottom - m_rc.top ) /2;

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );


	D3DXVECTOR3 saveldir;
	saveldir = m_light->orgDirection;

	m_light->orgDirection.x = -1.0f;//radxz == 0なので。
	m_light->orgDirection.y = -1.0f;
	m_light->orgDirection.z = 0.5f;
	D3DXVec3Normalize( &m_light->orgDirection, &m_light->orgDirection );



	if( m_app->m_sph_sh ){

		ret = SetWorkMaterial2Sph();
		_ASSERT( !ret );

		//D3DXVECTOR3 vEyePt( 0.0f, 0.0f, 0.0f );
		ret = m_app->m_sph_sh->TransformDispData( vEyePt, m_app->m_sph_mh, &inimat, &m_matView, &m_matProjX, 1, 0 );//, bonearray, tra_boneonly );
		if( ret ){
			DbgOut( "SetColDlg : Render : m_sph_sh->TransformDispData error !!!\n" );
			_ASSERT( 0 );
			m_light->orgDirection = saveldir;
			SetIniMaterial2Sph();
			return 1;
		}

		//ret = m_app->m_sph_sh->UpdateVertexBuffer( 0 );
		//if( ret ){
		//	DbgOut( "SetColDlg : Render : m_sph_sh->UpdateVertexBuffer error !!!\n" );
		//	_ASSERT( 0 );
		//	m_light->orgDirection = saveldir;
		//	SetIniMaterial2Sph();
		//	return 1;
		//}

	}


/////// render

	hres = m_pdev->SetRenderTarget( 0, m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "setcoldlg : render : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	hres = m_pdev->SetDepthStencilSurface( m_pZSurf );
	if( hres != D3D_OK ){
		DbgOut( "setcoldlg : render : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}

	g_rendertarget = TARGET_SUB2;

    m_viewport.X = 0;
    m_viewport.Y = 0;
    m_viewport.Height = m_rc.bottom - m_rc.top;
    m_viewport.Width =  m_rc.right - m_rc.left;
    m_viewport.MaxZ = 1.0f;
    m_viewport.MinZ = 0.0f;

    hres = m_pdev->SetViewport(&m_viewport);
	if( hres != D3D_OK ){
		DbgOut( "SetColDlg : Render : dev SetViewport error !!!\n" );
		_ASSERT( 0 );
		m_light->orgDirection = saveldir;
		SetIniMaterial2Sph();
		return 1;
	}

	hres = g_pEffect->SetMatrix( g_hmView, &m_matView );
	if( hres != D3D_OK ){
		DbgOut( "SetShaderConst : view set error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	hres = g_pEffect->SetMatrix( g_hmProj, &m_matProjX );
	if( hres != D3D_OK ){
		DbgOut( "SetShaderConst : proj set error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	if( FAILED( m_pdev->BeginScene() ) ){
		DbgOut( "SetColDlg : Render : dev BeginScene error !!!\n" );
		_ASSERT( 0 );
		m_light->orgDirection = saveldir;
		SetIniMaterial2Sph();
		return 1;
	}


	D3DCOLOR clearcol;
	clearcol = D3DCOLOR_ARGB( 0, 30, 65, 90 );

	D3DRECT clrc;
	clrc.x1 = m_rc.left;
	clrc.y1 = m_rc.top;
	clrc.x2 = m_rc.right;
	clrc.y2 = m_rc.bottom;

	//hres = m_pd3ddev->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
	hres = m_pdev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
						 clearcol,
						 1.0f, 0L );
	if( hres != D3D_OK ){
		DbgOut( "SetColDlg : Render : 3ddev Clear error !!!\n" );
		_ASSERT( 0 );
		m_light->orgDirection = saveldir;
		SetIniMaterial2Sph();
		return 1;
	}

	if( m_app->m_sph_sh ){
		ret = m_app->m_sph_sh->Render( 0, 0, m_pdev, 0, POLYDISP_ALL, m_app->m_sph_mh, 0, inimat, m_matView, m_matProjX, vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "SetColDlg : Render arrow : error !!!\n" );
			_ASSERT( 0 );
			m_light->orgDirection = saveldir;
			SetIniMaterial2Sph();
			return 1;
		}
	}

    // End the scene.
    m_pdev->EndScene();

	hres = m_pSwapChain->Present( NULL, NULL, (HWND)m_view_wnd, NULL, D3DPRESENT_DONOTWAIT );
	if( hres != D3D_OK ){
		DbgOut( "SetColDlg : Render : 3ddev Present error !!!\n" );
		_ASSERT( 0 );
		m_light->orgDirection = saveldir;
		SetIniMaterial2Sph();
		return 1;
	}

	//m_view_wnd.ValidateRect( &m_rc );

	m_light->orgDirection = saveldir;
	SetIniMaterial2Sph();


	return 0;
}

int CSetColDlg::SetWorkMaterial2Sph()
{
	int ret;
	int seri = -1;
	if( m_app->m_sph_sh ){
		ret = m_app->m_sph_th->GetDispObjNoByName( "obj1", &seri, m_app->m_sph_sh, 0 );
		_ASSERT( !ret );
		if( seri <= 0 ){
			_ASSERT( 0 );
			return 1;
		}

		ret = m_app->m_sph_sh->SetDispDiffuse( seri, GetRValue( m_wmaterial.diffuse ), GetGValue( m_wmaterial.diffuse ), GetBValue( m_wmaterial.diffuse ), 0, -1, 1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispSpecular( seri, GetRValue( m_wmaterial.specular ), GetGValue( m_wmaterial.specular ), GetBValue( m_wmaterial.specular ), 0, -1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispAmbient( seri, GetRValue( m_wmaterial.ambient ), GetGValue( m_wmaterial.ambient ), GetBValue( m_wmaterial.ambient ), 0, -1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispEmissive( seri, GetRValue( m_wmaterial.emissive ), GetGValue( m_wmaterial.emissive ), GetBValue( m_wmaterial.emissive ), 0, -1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispSpecularPower( seri, m_wmaterial.power, 0, -1 );
		_ASSERT( !ret );

	}
	return 0;
}
int CSetColDlg::SetIniMaterial2Sph()
{
	int ret;
	int seri = -1;
	if( m_app->m_sph_sh ){
		ret = m_app->m_sph_th->GetDispObjNoByName( "obj1", &seri, m_app->m_sph_sh, 0 );
		_ASSERT( !ret );
		if( seri <= 0 ){
			_ASSERT( 0 );
			return 1;
		}

		ret = m_app->m_sph_sh->SetDispDiffuse( seri, 255, 255, 255, 0, -1, 1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispSpecular( seri, 0, 0, 0, 0, -1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispAmbient( seri, 0, 0, 0, 0, -1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispEmissive( seri, 0, 0, 0, 0, -1 );
		_ASSERT( !ret );

		ret = m_app->m_sph_sh->SetDispSpecularPower( seri, 5.0f, 0, -1 );
		_ASSERT( !ret );

	}

	return 0;
}

LRESULT CSetColDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND scwnd = (HWND)lParam;
	WORD event;

	event = LOWORD( wParam );

	int curpos;
	if( (event = SB_THUMBTRACK) || (event == SB_THUMBPOSITION ) ){
		if( scwnd == (HWND)m_sl_power_wnd ){
			curpos = (int)m_sl_power_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			m_wmaterial.power = (float)curpos * 0.01f;
		}
		ParamsToDlg();
	}

	return 0;
}

int CSetColDlg::OnClickVert( int clipart, int* clivert, int clivertnum )
{
	if( (!(m_app->m_shandler)) || (clipart < 0) || (clivertnum <= 0) ){
		return 0;//!!!!!!!!!!
	}

	int ret;

	if( m_workkind == WKIND_PAINT ){
		MATERIAL0 undomat;
		int tempr, tempg, tempb;

		ret = m_app->m_shandler->GetDiffuse( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		undomat.diffuse = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetSpecular( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		undomat.specular = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetAmbient( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		undomat.ambient = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetEmissive( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		undomat.emissive = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetSpecularPower( clipart, *clivert, &undomat.power );
		_ASSERT( !ret );


		ret = AddUndoBuf( clipart, clivert, clivertnum, undomat );
		_ASSERT( !ret );


//////////////
		int setno;
		for( setno = 0; setno < clivertnum; setno++ ){

			ret = m_app->m_shandler->SetDispDiffuse( clipart, GetRValue( m_wmaterial.diffuse ), GetGValue( m_wmaterial.diffuse ), GetBValue( m_wmaterial.diffuse ), 0, *( clivert + setno ), 1 );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispSpecular( clipart, GetRValue( m_wmaterial.specular ), GetGValue( m_wmaterial.specular ), GetBValue( m_wmaterial.specular ), 0, *( clivert + setno ) );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispAmbient( clipart, GetRValue( m_wmaterial.ambient ), GetGValue( m_wmaterial.ambient ), GetBValue( m_wmaterial.ambient ), 0, *( clivert + setno ) );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispEmissive( clipart, GetRValue( m_wmaterial.emissive ), GetGValue( m_wmaterial.emissive ), GetBValue( m_wmaterial.emissive ), 0, *( clivert + setno ) );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispSpecularPower( clipart, m_wmaterial.power, 0, *( clivert + setno ) );
			_ASSERT( !ret );

		}

	}else if( m_workkind == WKIND_SPOIT ){
		int tempr, tempg, tempb;

		ret = m_app->m_shandler->GetDiffuse( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		m_wmaterial.diffuse = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetSpecular( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		m_wmaterial.specular = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetAmbient( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		m_wmaterial.ambient = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetEmissive( clipart, *clivert, &tempr, &tempg, &tempb );
		_ASSERT( !ret );
		m_wmaterial.emissive = RGB( tempr, tempg, tempb );

		ret = m_app->m_shandler->GetSpecularPower( clipart, *clivert, &m_wmaterial.power );
		_ASSERT( !ret );

	///////////
		if( m_diffuse_b )
			DeleteObject( m_diffuse_b );
		m_diffuse_b = ::CreateSolidBrush( m_wmaterial.diffuse );

		if( m_specular_b )
			DeleteObject( m_specular_b );
		m_specular_b = ::CreateSolidBrush( m_wmaterial.specular );

		if( m_ambient_b )
			DeleteObject( m_ambient_b );
		m_ambient_b = ::CreateSolidBrush( m_wmaterial.ambient );

		if( m_emissive_b )
			DeleteObject( m_emissive_b );
		m_emissive_b = ::CreateSolidBrush( m_wmaterial.emissive );
	/////////////


		ParamsToDlg();

	}

	return 0;
}

LRESULT CSetColDlg::OnCheckTexture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_TEX );
	if( checkflag == BST_CHECKED ){
		m_enabletexture = 0;
	}else if( checkflag == BST_UNCHECKED ){
		m_enabletexture = 1;
	}

	if( m_app->m_shandler ){
		m_app->m_shandler->EnableTexture( -1, m_enabletexture );
	}

	return 0;
}

int CSetColDlg::InitUndoBuf()
{
	m_undopos = 0;
	int undono;
	for( undono = 0; undono < MATERIALUNDOMAX; undono++ ){
		m_undo[ undono ].partno = -1;
		//m_undo[ undono ].vertno = -1;
		
		int vno;
		for( vno = 0; vno < PICKVERTMAX; vno++ ){
			m_undo[ undono ].vertno[ vno ] = -1;
		}
		m_undo[ undono ].vertnum = 0;

		m_undo[ undono ].mat.diffuse = RGB( 255, 255, 255 );
		m_undo[ undono ].mat.specular = RGB( 0, 0, 0 );
		m_undo[ undono ].mat.ambient = RGB( 64, 64, 64 );
		m_undo[ undono ].mat.emissive = RGB( 0, 0, 0 );
		m_undo[ undono ].mat.power = 5.0f;
	}
	return 0;
}

int CSetColDlg::AddUndoBuf( int partno, int* vertno, int vertnum, MATERIAL0 undomat )
{
	if( m_undopos <= ( MATERIALUNDOMAX - 1 ) ){
		//そのまま		
	}else{
		int undono;
		for( undono = 1; undono < MATERIALUNDOMAX; undono++ ){
			m_undo[undono - 1] = m_undo[undono];
		}
		m_undopos = MATERIALUNDOMAX - 1;
	}

	m_undo[m_undopos].partno = partno;
	//m_undo[m_undopos].vertno = vertno;
	m_undo[m_undopos].vertnum = vertnum;
	MoveMemory( m_undo[m_undopos].vertno, vertno, sizeof( int ) * vertnum );
	m_undo[m_undopos].mat = undomat;

	m_undopos++;

	return 0;
}

LRESULT CSetColDlg::OnUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	if( m_undopos <= 0 ){
		::MessageBox( m_dlg_wnd, "バッファが空です。Undo出来ません。", "Undo出来ません。", MB_OK );
		return 0;
	}
	
	m_undopos--;

	if( m_undo[m_undopos].vertnum <= 0 )
		return 0;//!!!!!!!!!


	int vno;
	for( vno = 0; vno < m_undo[m_undopos].vertnum; vno++ ){

		int setpart, setvert;
		MATERIAL0 setmat;
		setpart = m_undo[m_undopos].partno;
		setvert = m_undo[m_undopos].vertno[vno];
		setmat = m_undo[m_undopos].mat;

		if( m_app->m_shandler && (setpart >= 0) && (setvert >= 0) ){
			ret = m_app->m_shandler->SetDispDiffuse( setpart, GetRValue( setmat.diffuse ), GetGValue( setmat.diffuse ), GetBValue( setmat.diffuse ), 0, setvert, 1 );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispSpecular( setpart, GetRValue( setmat.specular ), GetGValue( setmat.specular ), GetBValue( setmat.specular ), 0, setvert );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispAmbient( setpart, GetRValue( setmat.ambient ), GetGValue( setmat.ambient ), GetBValue( setmat.ambient ), 0, setvert );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispEmissive( setpart, GetRValue( setmat.emissive ), GetGValue( setmat.emissive ), GetBValue( setmat.emissive ), 0, setvert );
			_ASSERT( !ret );

			ret = m_app->m_shandler->SetDispSpecularPower( setpart, setmat.power, 0, setvert );
			_ASSERT( !ret );
			
		}

	}
	return 0;
}

int CSetColDlg::OnPaletteFunc( int palno )
{
	DestroyTimer();
	m_modalflag = 1;

	int ret;
	CSetCol2Dlg dlg( &m_wmaterial, &(m_mpalette[palno - 1]), palno );
	ret = (int)dlg.DoModal( m_hWnd );

	::DeleteObject( m_palette_b[palno - 1] );
	m_palette_b[palno - 1] = ::CreateSolidBrush( m_mpalette[palno - 1].mat.diffuse );


	if( m_diffuse_b )
		DeleteObject( m_diffuse_b );
	m_diffuse_b = ::CreateSolidBrush( m_wmaterial.diffuse );

	if( m_specular_b )
		DeleteObject( m_specular_b );
	m_specular_b = ::CreateSolidBrush( m_wmaterial.specular );

	if( m_ambient_b )
		DeleteObject( m_ambient_b );
	m_ambient_b = ::CreateSolidBrush( m_wmaterial.ambient );

	if( m_emissive_b )
		DeleteObject( m_emissive_b );
	m_emissive_b = ::CreateSolidBrush( m_wmaterial.emissive );


	ParamsToDlg();

	m_modalflag = 0;

	CreateTimer();

	return 0;
}


LRESULT CSetColDlg::OnPalette1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 1 );
	return 0;
}
LRESULT CSetColDlg::OnPalette2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 2 );
	return 0;
}
LRESULT CSetColDlg::OnPalette3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 3 );
	return 0;
}
LRESULT CSetColDlg::OnPalette4(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 4 );
	return 0;
}
LRESULT CSetColDlg::OnPalette5(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 5 );
	return 0;
}
LRESULT CSetColDlg::OnPalette6(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 6 );

	return 0;
}
LRESULT CSetColDlg::OnPalette7(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 7 );

	return 0;
}
LRESULT CSetColDlg::OnPalette8(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 8 );

	return 0;
}
LRESULT CSetColDlg::OnPalette9(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 9 );

	return 0;
}
LRESULT CSetColDlg::OnPalette10(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 10 );

	return 0;
}
LRESULT CSetColDlg::OnPalette11(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 11 );

	return 0;
}
LRESULT CSetColDlg::OnPalette12(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 12 );

	return 0;
}
LRESULT CSetColDlg::OnPalette13(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 13 );

	return 0;
}
LRESULT CSetColDlg::OnPalette14(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 14 );

	return 0;
}
LRESULT CSetColDlg::OnPalette15(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 15 );

	return 0;
}
LRESULT CSetColDlg::OnPalette16(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnPaletteFunc( 16 );

	return 0;
}

int CSetColDlg::CreateTimer()
{
	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERCOLTIMER, 100 );
		DbgOut( "SetColDlg :CreateSwapChain : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERCOLTIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}
	return 0;
}
int CSetColDlg::DestroyTimer()
{
	int ret;
	if( m_timerid != 0 ){
		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;

		//Sleep( 100 );//!!!!!!!!!!!!!!!!!!!
	}

	return 0;
}

LRESULT CSetColDlg::OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.mpl" );

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Material Palette (*.mpl)\0*.mpl\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="mpl";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return S_OK;
	}

	int ret;
	ret = FileSaveFunc( buf );
	_ASSERT( !ret );
	

	return 0;
}
LRESULT CSetColDlg::OnFileLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Material Palette (*.mpl)\0*.mpl\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="mpl";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetOpenFileName(&ofn) ){
		return S_OK;
	}

	int ret;
	ret = FileLoadFunc( buf );
	_ASSERT( !ret );

	return 0;
}

int CSetColDlg::FileLoadFunc( char* filename )
{
	CMaterialPaletteFile mpfile;
	int ret;
	ret = mpfile.LoadMaterialPalette( filename );
	_ASSERT( !ret );

	MoveMemory( m_mpalette, mpfile.m_mpalette, sizeof( MPALETTE ) * MPALETTENUM );

	if( m_diffuse_b )
		DeleteObject( m_diffuse_b );
	m_diffuse_b = ::CreateSolidBrush( m_wmaterial.diffuse );

	if( m_specular_b )
		DeleteObject( m_specular_b );
	m_specular_b = ::CreateSolidBrush( m_wmaterial.specular );

	if( m_ambient_b )
		DeleteObject( m_ambient_b );
	m_ambient_b = ::CreateSolidBrush( m_wmaterial.ambient );

	if( m_emissive_b )
		DeleteObject( m_emissive_b );
	m_emissive_b = ::CreateSolidBrush( m_wmaterial.emissive );

	int pno;
	for( pno = 0; pno < MPALETTENUM; pno++ ){

		if( m_palette_b[pno] )
			DeleteObject( m_palette_b[pno] );
		m_palette_b[pno] = ::CreateSolidBrush( m_mpalette[pno].mat.diffuse );
	}

	return 0;
}

int CSetColDlg::FileSaveFunc( char* filename )
{
	CMaterialPaletteFile mpfile;
	int ret;
	ret = mpfile.WriteMaterialPalette( filename, m_mpalette, MPALETTENUM );
	_ASSERT( !ret );

	return 0;
}

