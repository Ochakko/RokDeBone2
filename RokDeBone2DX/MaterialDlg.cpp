// MaterialDlg.cpp : CMaterialDlg のインプリメンテーション
#include "stdafx.h"
#include "MaterialDlg.h"

#include <math.h>
#include <coef.h>

#define DBGH
#include "dbg.h"

#include "ColDlg.h"

#include "viewer.h"
#include "inidata.h"
#include <LightData.h>

#include <treehandler2.h>
#include <shdhandler.h>
#include <mothandler.h>
#include <mqomaterial.h>

#include <crtdbg.h>

#include "GetDlgParams.h"
#include "TexSetting2Dlg.h"


#define RENDERMATTIMER	11003

//------- extern
extern CColDlg g_coldlg;
extern int g_rendertarget;
extern float g_specular;
extern float g_ambient;
extern ID3DXEffect*	g_pEffect;
extern D3DXHANDLE g_hmView;
extern D3DXHANDLE g_hmProj;

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg

CMaterialDlg::CMaterialDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl )
{
	m_modalflag = 0;
	m_timerid = 0;

	m_createflag = 0;
	m_comboflag = 0;
	m_cmdshow = SW_HIDE;

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

	m_walpha = 0.0f;

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

	m_selmaterial = -1;

	m_blendmode = 0;
	m_alphatest0 = 1;
	m_alphatest1 = 0;
	m_alphaval0 = 8;
	m_alphaval1 = 8;
}

CMaterialDlg::~CMaterialDlg()
{
	DestroySwapChain();
}

int CMaterialDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_view_wnd = GetDlgItem( IDC_VIEW );
	m_diffuse_wnd = GetDlgItem( IDC_VDIFFUSE );
	m_specular_wnd = GetDlgItem( IDC_VSPECULAR );
	m_ambient_wnd = GetDlgItem( IDC_VAMBIENT );
	m_emissive_wnd = GetDlgItem( IDC_VEMISSIVE );
	m_sl_power_wnd = GetDlgItem( IDC_SL_POWER );
	m_power_wnd = GetDlgItem( IDC_POWER );
	m_list_wnd = GetDlgItem( IDC_LIST1 );
	m_matno_wnd = GetDlgItem( IDC_MATNO );
	m_alpha_wnd = GetDlgItem( IDC_ALPHA );

	m_sl_power_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_power_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)10000 );
	m_sl_power_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)((int)(m_wmaterial.power * 100.0f)) );
	SetSliderTic( m_sl_power_wnd, 1000, 0, 10000 );


	m_combo1_wnd = GetDlgItem( IDC_COMBO1 );
	if( m_comboflag == 0 ){
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"0 ノーマル: OPE:add, SRC:src, DEST:invsrc" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"1 加算１: OPE:add, SRC:one, DEST:one" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"2 加算２: OPE:add, SRC:src, DEST:one" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"3 乗算： OPE:add, SRC:dest, DEST:src" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"4 減算： OPE:revsub, SRC:zero, DEST:invsrc" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"5 スクリーン： OPE:add, SRC:invdest, DEST:one" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"6 ネガ反転： OPE:add, SRC:invdest, DEST:invsrc" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"7オーバーレイ風:OPE:add, SRC:dstcol, DEST:dstcol" );
		m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"8覆い焼き風:OPE:add, SRC:srcalpha, DEST:dstcol" );
		if( m_blendmode < 100 ){
			m_combo1_wnd.SendMessage( CB_SETCURSEL, m_blendmode, 0 );
		}else{
			m_combo1_wnd.SendMessage( CB_SETCURSEL, m_blendmode - 100, 0 );
		}
		m_comboflag = 1;
	}

	m_alphatest0_wnd = GetDlgItem( IDC_ON_WA0 );
	m_alphatest1_wnd = GetDlgItem( IDC_ON_WA1 );
	m_alphaval0_wnd = GetDlgItem( IDC_TESTVAL0 );
	m_alphaval1_wnd = GetDlgItem( IDC_TESTVAL1 );

////////

	m_diffuse_dc = ::GetDC( m_diffuse_wnd );
	m_specular_dc = ::GetDC( m_specular_wnd );
	m_ambient_dc = ::GetDC( m_ambient_wnd );
	m_emissive_dc = ::GetDC( m_emissive_wnd );

	return 0;
}
void CMaterialDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	// !!! minticとmaxticには、自動的にticが入るので設定はしない。(してはいけない)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		dstwnd.SendMessage( TBM_SETTIC, 0, (LPARAM)(LONG)curtic );
		curtic += ticfreq;
	}
}


int CMaterialDlg::ParamsToDlg()
{

	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", m_wmaterial.power );
	m_power_wnd.SetWindowText( tempchar );



	m_sl_power_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)((int)(m_wmaterial.power * 100.0f)) );



	if( m_selmaterial >= 0 ){
		sprintf_s( tempchar, 256, "マテリアル番号： %d", m_selmaterial );
	}else{
		strcpy_s( tempchar, 256, "マテリアルが選択されていません。" );
	}
	m_matno_wnd.SetWindowText( tempchar );



	sprintf_s( tempchar, 256, "%f", m_walpha );
	m_alpha_wnd.SetWindowTextA( tempchar );


	if( m_blendmode < 100 ){
		m_combo1_wnd.SendMessage( CB_SETCURSEL, m_blendmode, 0 );
	}else{
		m_combo1_wnd.SendMessage( CB_SETCURSEL, m_blendmode - 100, 0 );
	}

	if( m_alphatest0 ){
		m_dlg_wnd.CheckDlgButton( IDC_ON_WA0, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_ON_WA0, BST_UNCHECKED );
	}
	if( m_alphatest1 ){
		m_dlg_wnd.CheckDlgButton( IDC_ON_WA1, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_ON_WA1, BST_UNCHECKED );
	}

	sprintf_s( tempchar, 256, "%d", m_alphaval0 );
	m_alphaval0_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%d", m_alphaval1 );
	m_alphaval1_wnd.SetWindowText( tempchar );


	return 0;
}

LRESULT CMaterialDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//DbgOut( "MaterialDlg : OnInitDialog \r\n" );

	int ret = 0;
	ret = SetWnd();
	_ASSERT( !ret );

	ret = InitList();
	_ASSERT( !ret );

	m_selmaterial = -1;

	ret = ParamsToDlg();
	_ASSERT( !ret );


	if( m_createflag == 0 ){
		int ret;
		ret = CreateSwapChain( m_pdev );
		if( ret ){
			DbgOut( "MaterialDlg : OnInitDailog : CreateSwapChain error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
	}

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CMaterialDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	ParamsToDlg();

	ShowDlg( SW_HIDE );
	
//	::MessageBox( m_app->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
//	SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	//EndDialog(wID);

	return 0;
}

LRESULT CMaterialDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ParamsToDlg();

	ShowDlg( SW_HIDE );

//	::MessageBox( m_app->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
//	SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);

	return 0;
}


LRESULT CMaterialDlg::OnRefDiffuse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
LRESULT CMaterialDlg::OnRefSpecular(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
LRESULT CMaterialDlg::OnRefAmbient(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
LRESULT CMaterialDlg::OnRefEmissive(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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



LRESULT CMaterialDlg::OnPaintOrg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_modalflag == 1 )
		return 0;

	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	PaintRect( m_diffuse_wnd, m_diffuse_dc, m_diffuse_b, m_wmaterial.diffuse );
	PaintRect( m_specular_wnd, m_specular_dc, m_specular_b, m_wmaterial.specular );
	PaintRect( m_ambient_wnd, m_ambient_dc, m_ambient_b, m_wmaterial.ambient );
	PaintRect( m_emissive_wnd, m_emissive_dc, m_emissive_b, m_wmaterial.emissive );
	
	//Render();

	return 0;
}

int CMaterialDlg::PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush, COLORREF srccol )
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

int CMaterialDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;	
	ShowWindow( m_cmdshow );
	
	int ret;
	if( cmdshow == SW_SHOW ){

		ret = InitList();
		_ASSERT( !ret );

		m_selmaterial = -1;
	}else{

	}
	SetIniMaterial2Sph();

	ParamsToDlg();

	return 0;	
}

LRESULT CMaterialDlg::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//HDC hdc;
	//hdc = ((LPDRAWITEMSTRUCT)lParam)->hDC;
	//_ASSERT( hdc );
	//int id;
	//id = (int)wParam;

	return 0;
}

LRESULT CMaterialDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CMaterialDlg::OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	return 0;
}


int CMaterialDlg::CreateSwapChain( LPDIRECT3DDEVICE9 pdev )
{
	HRESULT hres;

	m_pdev = pdev;

	if( m_createflag == 1 )
		return 0;

	if( !m_pdev ){
		DbgOut( "MaterialDlg : CreateSwapChain : 3ddevice NULL error !!!\n" );
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
		DbgOut( "MaterialDlg : CreateSwapChain : dev CreateAdditionalSwapChain error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	hres = m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "MaterialDlg : CreateSwapChain : swapchain GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DFORMAT fmt;
	fmt = D3DFMT_D16;	
	hres = m_pdev->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, fmt, m_d3dpp.MultiSampleType, 0, FALSE, &m_pZSurf, NULL );
	if( hres != D3D_OK ){
		DbgOut( "MaterialDlg : CreateSwapChain : dev CreateDepthStencilSurface error %x !!!\n", hres );
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
int CMaterialDlg::DestroySwapChain()
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

int CMaterialDlg::Render()
{
	if( m_modalflag == 1 )
		return 0;


	HRESULT hres;
	int ret;

//DbgOut( "MaterialDlg : Render : %d %x %d, %d\r\n", m_cmdshow, m_pdev, m_createflag, SW_HIDE );

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
			DbgOut( "MaterialDlg : Render : m_sph_sh->TransformDispData error !!!\n" );
			_ASSERT( 0 );
			m_light->orgDirection = saveldir;
			SetIniMaterial2Sph();
			return 1;
		}

		//ret = m_app->m_sph_sh->UpdateVertexBuffer( 0 );
		//if( ret ){
		//	DbgOut( "MaterialDlg : Render : m_sph_sh->UpdateVertexBuffer error !!!\n" );
		//	_ASSERT( 0 );
		//	m_light->orgDirection = saveldir;
		//	SetIniMaterial2Sph();
		//	return 1;
		//}

	}


/////// render

	hres = m_pdev->SetRenderTarget( 0, m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "MaterialDlg : render : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	hres = m_pdev->SetDepthStencilSurface( m_pZSurf );
	if( hres != D3D_OK ){
		DbgOut( "MaterialDlg : render : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
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
		DbgOut( "MaterialDlg : Render : dev SetViewport error !!!\n" );
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
		DbgOut( "MaterialDlg : Render : dev BeginScene error !!!\n" );
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
		DbgOut( "MaterialDlg : Render : 3ddev Clear error !!!\n" );
		_ASSERT( 0 );
		m_light->orgDirection = saveldir;
		SetIniMaterial2Sph();
		return 1;
	}

	if( m_app->m_sph_sh ){
		ret = m_app->m_sph_sh->Render( 0, 0, m_pdev, 0, POLYDISP_ALL, m_app->m_sph_mh, 0, inimat, m_matView, m_matProjX, vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "MaterialDlg : Render arrow : error !!!\n" );
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
		DbgOut( "MaterialDlg : Render : 3ddev Present error !!!\n" );
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

int CMaterialDlg::SetWorkMaterial2Sph()
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
int CMaterialDlg::SetIniMaterial2Sph()
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

LRESULT CMaterialDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

//int CMaterialDlg::OnClickVert( int clipart, int* clivert, int clivertnum )
/***
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
***/
//	ParamsToDlg();



int CMaterialDlg::CreateTimer()
{
	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERMATTIMER, 100 );
		DbgOut( "MaterialDlg :CreateSwapChain : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERMATTIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}
	return 0;
}
int CMaterialDlg::DestroyTimer()
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

int CMaterialDlg::InitList()
{

	m_list_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	
	if( !m_app->m_shandler ){
		return 0;//!!!!!!!!!!!!!
	}

	CMQOMaterial* curmat;
	curmat = m_app->m_shandler->m_mathead;

	while( curmat ){

		if( curmat->materialno >= 0 ){
			int newindex;
			newindex = (int)m_list_wnd.SendMessageA( LB_ADDSTRING, 0, (LPARAM)curmat->name );
			if( (newindex == LB_ERR) || (newindex == LB_ERRSPACE) ){
				_ASSERT( 0 );
				return 1;
			}

			int ret;
			ret = (int)m_list_wnd.SendMessage( LB_SETITEMDATA, newindex, curmat->materialno );
			if( ret == LB_ERR ){
				_ASSERT( 0 );
				return 1;
			}
		}
		curmat = curmat->next;
	}

	return 0;
}

LRESULT CMaterialDlg::OnSelchangeList1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( wNotifyCode != LBN_SELCHANGE )
		return 0;

	int selindex;
	selindex = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		m_selmaterial = -1;
		ParamsToDlg();
		return 0;
	}
	m_selmaterial = (int)m_list_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );
	if( (m_selmaterial < 0) ){
		_ASSERT( 0 );
		m_selmaterial = -1;
		ParamsToDlg();
		return 1;
	}

	ret = Material2Dlg( m_selmaterial );
	if( ret ){
		DbgOut( "materialdlg : OnSelChangeList1 : Material2Dlg error !!!\n" );
		_ASSERT( 0 );
		m_selmaterial = -1;
		ParamsToDlg();
		return 1;
	}


//	if( m_app->m_shandler ){
//		if( m_app->m_shandler->m_mathead ){
//			CMQOMaterial* curmat;
//			curmat = GetMaterialFromNo( m_app->m_shandler->m_mathead, m_selmaterial );
//			if( !curmat ){
//				_ASSERT( 0 );
//				return 1;
//			}
//
//			ret = EnableAlphaWnd( curmat->exttexmode );
//			_ASSERT( !ret );
//		}
//	}

////////////

	ParamsToDlg();
	
	return 0;
}

int CMaterialDlg::Material2Dlg( int matno )
{
	int ret;

	if( !m_app->m_shandler ){
		return 0;//!!!!!!!!!!!!!!!
	}
	if( !m_app->m_shandler->m_mathead ){
		return 0;//!!!!!!!!!!!!!!!
	}

	CMQOMaterial* curmat;
	curmat = GetMaterialFromNo( m_app->m_shandler->m_mathead, matno );
	if( !curmat ){
		_ASSERT( 0 );
		return 1;
	}

	ret = curmat->GetColorrefDiffuse( &m_wmaterial.diffuse );
	_ASSERT( !ret );

	ret = curmat->GetColorrefSpecular( &m_wmaterial.specular );
	_ASSERT( !ret );

	ret = curmat->GetColorrefAmbient( &m_wmaterial.ambient );
	_ASSERT( !ret );

	ret = curmat->GetColorrefEmissive( &m_wmaterial.emissive );
	_ASSERT( !ret );

	m_wmaterial.power = curmat->power;

	//m_walpha = curmat->col.a;
	m_walpha = curmat->orgalpha;

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

	///////////
	m_blendmode = curmat->blendmode;
	m_alphatest0 = curmat->alphatest0;
	m_alphatest1 = curmat->alphatest1;
	m_alphaval0 = curmat->alphaval0;
	m_alphaval1 = curmat->alphaval1;

	return 0;
}

CMQOMaterial* CMaterialDlg::GetMaterialFromNo( CMQOMaterial* mathead, int matno )
{
	CMQOMaterial* retmat = 0;
	CMQOMaterial* chkmat = mathead;

	while( chkmat ){
		if( chkmat->materialno == matno ){
			retmat = chkmat;
			break;
		}
		chkmat = chkmat->next;
	}
	
	return retmat;
}

LRESULT CMaterialDlg::OnApplyAlpha(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	
	if( !m_app->m_shandler ){
		return 0;
	}
	if( !m_app->m_shandler->m_mathead ){
		return 0;
	}

	if( m_selmaterial < 0 ){
		::MessageBox( m_hWnd, "マテリアル名が選択されていません", "未選択エラー", MB_OK );
		return 0;
	}
	
	float tempfloat = 1.0f;
	ret = GetFloat( m_alpha_wnd, &tempfloat );
	if( ret || ( (tempfloat < 0.0f) || (tempfloat > 1.0f) ) ){
		::MessageBox( m_hWnd, "透明度が不正です。 0.0から1.0", "編集エラー", MB_OK );
		return 0;//!!!!!!!!!
	}

	m_walpha = tempfloat;

	ret = m_app->m_shandler->SetMaterialAlpha( m_selmaterial, m_walpha );
	if( ret ){
		DbgOut( "materialdlg : OnApplyAlpha : sh SetMaterialAlpha error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CMaterialDlg::OnTexture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_app->m_shandler ){
		return 0;
	}
	if( !m_app->m_shandler->m_mathead ){
		return 0;
	}

	if( m_selmaterial < 0 ){
		::MessageBox( m_hWnd, "マテリアル名が選択されていません", "未選択エラー", MB_OK );
		return 0;
	}
	
	CMQOMaterial* curmat;
	curmat = GetMaterialFromNo( m_app->m_shandler->m_mathead, m_selmaterial );
	if( !curmat ){
		DbgOut( "materialdlg : OnTexture : curmat NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CTexSetting2Dlg dlg( curmat );

	int dlgret, ret;

	dlgret = (int)dlg.DoModal( m_hWnd );
	if( dlgret == IDOK ){
		strcpy_s( curmat->tex, 256, dlg.m_texname );
		strcpy_s( curmat->bump, 256, dlg.m_bumpname );

		//ret = m_selem->SetTexRepetition( dlg.m_texrepx, dlg.m_texrepy );
		//_ASSERT( !ret );

		curmat->transparent = dlg.m_transparent;
		
		curmat->uanime = dlg.m_uanime;
		curmat->vanime = dlg.m_vanime;

		curmat->texrule = dlg.m_texrule;
		curmat->texanimtype = dlg.m_texanimtype;//!!!!!!!!

//DbgOut( "ShdParamDlg : OnTexture : exttexnum %d, firstname %s\n", dlg.m_exttexnum, *(dlg.m_exttexlist) );
		ret = curmat->SetExtTex( dlg.m_exttexnum, dlg.m_exttexlist );
		_ASSERT( !ret );

		curmat->exttexmode = dlg.m_exttexmode;
		curmat->exttexrep = dlg.m_exttexrep;//no
		curmat->exttexstep = dlg.m_exttexstep;//no

		curmat->glowmult[0] = dlg.m_glowmult.r;
		curmat->glowmult[1] = dlg.m_glowmult.g;
		curmat->glowmult[2] = dlg.m_glowmult.b;

		//editflag |= TV_EDIT_TEXTURE;

//		ret = EnableAlphaWnd( dlg.m_exttexmode );
//		_ASSERT( !ret );

		::SendMessage( m_app->m_hWnd, WM_USER_REMAKE_TEXTURE, (WPARAM)-1, 0 );
		::SendMessage( m_app->m_hWnd, WM_USER_REMAKE_UV, (WPARAM)-1, 0 );

	}

	return 0;
}



LRESULT CMaterialDlg::OnApplyMaterial(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_app->m_shandler ){
		return 0;
	}
	if( !m_app->m_shandler->m_mathead ){
		return 0;
	}

	if( m_selmaterial < 0 ){
		::MessageBox( m_hWnd, "マテリアル名が選択されていません", "未選択エラー", MB_OK );
		return 0;
	}
	
	CMQOMaterial* curmat;
	curmat = GetMaterialFromNo( m_app->m_shandler->m_mathead, m_selmaterial );
	if( !curmat ){
		DbgOut( "materialdlg : OnApplyMaterial : curmat NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	int setflag = 0;
	ret = m_app->m_shandler->SetMaterialDiffuse( curmat->materialno, setflag, GetRValue( m_wmaterial.diffuse ), GetGValue( m_wmaterial.diffuse ), GetBValue( m_wmaterial.diffuse ) );
	_ASSERT( !ret );

	ret = m_app->m_shandler->SetMaterialSpecular( curmat->materialno, setflag, GetRValue( m_wmaterial.specular ), GetGValue( m_wmaterial.specular ), GetBValue( m_wmaterial.specular ) );
	_ASSERT( !ret );

	ret = m_app->m_shandler->SetMaterialAmbient( curmat->materialno, setflag, GetRValue( m_wmaterial.ambient ), GetGValue( m_wmaterial.ambient ), GetBValue( m_wmaterial.ambient ) );
	_ASSERT( !ret );

	ret = m_app->m_shandler->SetMaterialEmissive( curmat->materialno, setflag, GetRValue( m_wmaterial.emissive ), GetGValue( m_wmaterial.emissive ), GetBValue( m_wmaterial.emissive ) );
	_ASSERT( !ret );

	ret = m_app->m_shandler->SetMaterialPower( curmat->materialno, setflag, m_wmaterial.power );
	_ASSERT( !ret );

	return 0;
}

LRESULT CMaterialDlg::OnApplyAlphaTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_app->m_shandler ){
		return 0;
	}
	if( !m_app->m_shandler->m_mathead ){
		return 0;
	}

	if( m_selmaterial < 0 ){
		::MessageBox( m_hWnd, "マテリアル名が選択されていません", "未選択エラー", MB_OK );
		return 0;
	}
	
	CMQOMaterial* curmat;
	curmat = GetMaterialFromNo( m_app->m_shandler->m_mathead, m_selmaterial );
	if( !curmat ){
		DbgOut( "materialdlg : OnApplyAlphaTest : curmat NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//////////


	int combono;
	combono = (int)m_combo1_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono != CB_ERR ){
		m_blendmode = combono + 100;
	}
	
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_ON_WA0 );
	if( checkflag == BST_CHECKED ){
		m_alphatest0 = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_alphatest0 = 0;
	}

	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_ON_WA1 );
	if( checkflag == BST_CHECKED ){
		m_alphatest1 = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_alphatest1 = 0;
	}

	int ret;
	int tmpint0;
	ret = GetInt( m_alphaval0_wnd, &tmpint0 );
	if( ret || (tmpint0 < 0) || (tmpint0 > 255) ){
		::MessageBox( m_hWnd, "不透明時の閾値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_alphaval0 = tmpint0;

	int tmpint1;
	ret = GetInt( m_alphaval1_wnd, &tmpint1 );
	if( ret || (tmpint1 < 0) || (tmpint1 > 255) ){
		::MessageBox( m_hWnd, "半透明時の閾値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_alphaval1 = tmpint1;

///////////
	curmat->blendmode = m_blendmode;
	curmat->alphatest0 = m_alphatest0;
	curmat->alphaval0 = m_alphaval0;
	curmat->alphatest1 = m_alphatest1;
	curmat->alphaval1 = m_alphaval1;

	return 0;
}

int CMaterialDlg::EnableAlphaWnd( int exttexmode )
{
	if( exttexmode == EXTTEXMODE_GLOW ){
		char str[256];
		sprintf_s( str, 256, "%f", 1.0f );
		m_alpha_wnd.SetWindowTextA( str );

		LRESULT lret;
		BOOL dummy;
		lret = OnApplyAlpha( 0, 0, 0, dummy );
		_ASSERT( !lret );

		m_alpha_wnd.EnableWindow( FALSE );
	}else{
		m_alpha_wnd.EnableWindow( TRUE );
	}
	return 0;
}


