// LightDlg.cpp : CLightDlg のインプリメンテーション
#include "stdafx.h"
#include "LightDlg.h"

#include <math.h>
#include <coef.h>

#define DBGH
#include <dbg.h>

#include "ColDlg.h"

#include "viewer.h"
#include "inidata.h"
#include <LightData.h>

#include <treehandler2.h>
#include <shdhandler.h>
#include <mothandler.h>

#include <crtdbg.h>

#define RENDERSPHTIMER	11000

//------- extern
extern CColDlg g_coldlg;
extern int g_rendertarget;
extern float g_specular;
extern float g_ambient;
extern ID3DXEffect*	g_pEffect;
extern D3DXHANDLE g_hmView;
extern D3DXHANDLE g_hmProj;

/////////////////////////////////////////////////////////////////////////////
// CLightDlg

CLightDlg::CLightDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl )
{
	m_dc = NULL;
	m_brush = NULL;

	m_mcap = 0;
	m_lbutton = 0;
	m_inverse = 0;
	m_ref = 0;
	m_moving = 0;

	m_timerid = 0;
	m_createflag = 0;
	m_cmdshow = SW_HIDE;

	m_pdev = pdev;
	m_app = papp;
	m_inidata = pini;
	m_light = pl;
	m_d3dpp = papp->m_d3dpp;


	m_lightvec.x = 0.005f;//z軸ぴったりだとなぜかシャドウがうまくいかない。
	m_lightvec.y = 0.0f;
	m_lightvec.z = 1.0f;
	D3DXVec3Normalize( &m_lightvec, &m_lightvec );


	m_color = m_inidata->dirlight_col;
	m_lblend = m_light->lblend;

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

}



CLightDlg::~CLightDlg()
{
	DestroySwapChain();
}

int CLightDlg::CalcLightVec( int alpha, int beta, D3DXVECTOR3* dstvec )
{
	float rad_alpha = (float)alpha * (float)DEG2PAI;
	float rad_beta = (float)beta * (float)DEG2PAI;

	float vecx, vecy, vecz;
	vecx = (float)( sin( rad_alpha ) * cos( rad_beta ) );
	vecy = (float)sin( rad_beta );
	vecz = (float)( -cos( rad_alpha ) * cos( rad_beta ) );

	dstvec->x = vecx;
	dstvec->y = vecy;
	dstvec->z = vecz;

	return 0;
}

int CLightDlg::SetWnd()
{
	HWND temphwnd;

	m_dlg_wnd = m_hWnd;


	temphwnd = GetDlgItem( IDC_COLOR );
	m_color_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT_COL );
	m_text_col_wnd = temphwnd;

	m_view_wnd = GetDlgItem( IDC_VIEW );

///////////
	m_dc = ::GetDC( m_color_wnd );


	return 0;
}


int CLightDlg::ParamsToDlg()
{
	char tempchar[1024];


	sprintf_s( tempchar, 1024, "R: %d, G: %d, B: %d",
		GetRValue( m_color ), GetGValue( m_color ), GetBValue( m_color ) );
	m_text_col_wnd.SetWindowText( tempchar );

	/***
	switch( m_lblend ){
	case LBLEND_MULT:
		m_dlg_wnd.CheckRadioButton( IDC_MULT, IDC_HARDLIGHT, IDC_MULT );
		break;
	case LBLEND_SCREEN:
		m_dlg_wnd.CheckRadioButton( IDC_MULT, IDC_HARDLIGHT, IDC_SCREEN );
		break;
	case LBLEND_OVERLAY:
		m_dlg_wnd.CheckRadioButton( IDC_MULT, IDC_HARDLIGHT, IDC_OVERLAY );
		break;
	case LBLEND_HARDLIGHT:
		m_dlg_wnd.CheckRadioButton( IDC_MULT, IDC_HARDLIGHT, IDC_HARDLIGHT );
		break;
	case LBLEND_DODGE:
		m_dlg_wnd.CheckRadioButton( IDC_MULT, IDC_HARDLIGHT, IDC_DODGE );
		break;
	default:
		break;
	}
	***/

	if( m_inverse == 0 ){
		m_dlg_wnd.CheckDlgButton( IDC_INVERSE, BST_UNCHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_INVERSE, BST_CHECKED );
	}

	return 0;
}

LRESULT CLightDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret = 0;
	ret = SetWnd();
	_ASSERT( !ret );

	ret = ParamsToDlg();
	_ASSERT( !ret );

	if( m_createflag == 0 ){
		int ret;
		ret = CreateSwapChain( m_pdev );
		if( ret ){
			DbgOut( "lightdlg : OnInitDailog : CreateSwapChain error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
	}

	/***
	TRACKMOUSEEVENT event;

	event.cbSize = sizeof( TRACKMOUSEEVENT );
	event.dwFlags = TME_HOVER | TME_LEAVE;
	event.hwndTrack = m_hWnd;
	event.dwHoverTime = 100;
	::TrackMouseEvent( &event );
	***/


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CLightDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	/***
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_MULT );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_MULT;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_SCREEN );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_SCREEN;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_OVERLAY );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_OVERLAY;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_HARDLIGHT );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_HARDLIGHT;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_DODGE );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_DODGE;
	}
	***/

	ParamsToLight();


	//EndDialog(wID);
	ShowDlg( SW_HIDE );
	

	return 0;
}

LRESULT CLightDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowDlg( SW_HIDE );

	return 0;
}


LRESULT CLightDlg::OnColorRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	m_ref = 1;
	m_lbutton = 0;

	if( m_mcap == 0 ){
		SetCapture();
		m_mcap = 1;
	}

	ret = g_coldlg.Choose( m_dlg_wnd, &m_color );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	if( m_brush )
		::DeleteObject( m_brush );
	m_brush = ::CreateSolidBrush( m_color );


	ParamsToDlg();
	ParamsToLight();

	m_ref = 0;

	return 0;
}

LRESULT CLightDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	if( !m_dc )
		return 0;
	if( !m_brush )
		return 0;


	//HBRUSH brush;
	//brush = ::CreateSolidBrush( m_color );
	//_ASSERT( brush != NULL );


	//HDC hdc;
	//hdc = m_color_wnd.GetDC();
	RECT clirect;
	m_color_wnd.GetClientRect( &clirect );
	
	
	HBRUSH hOldBrush = (HBRUSH)SelectObject( m_dc, m_brush );
	BOOL bret;
	bret = ::Rectangle( m_dc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
	//::SelectObject( hdc, hOldBrush );
	//m_color_wnd.ReleaseDC( hdc );
	
	return 0;
}
int CLightDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;
	
	if( m_cmdshow == SW_HIDE ){
		ReleaseCapture();
		m_mcap = 0;
		m_lbutton = 0;
		m_moving = 0;
		m_ref = 0;
	}else{
		if( m_brush )
			::DeleteObject( m_brush );
		m_brush = ::CreateSolidBrush( m_color );

	}
	
	SetIniMaterial2Sph();
	
	ShowWindow( m_cmdshow );
		
	ParamsToLight();
	

	return 0;	
}

int CLightDlg::ParamsToLight()
{
	int ret;
	m_inidata->dirlight_col = m_color;


	CVec3f difcol;
	//difcol.x = 1.0f; difcol.y = 1.0f; difcol.z = 1.0f;
	difcol.x = (float)GetRValue( m_inidata->dirlight_col ) / 255.0f;
	difcol.y = (float)GetGValue( m_inidata->dirlight_col ) / 255.0f;
	difcol.z = (float)GetBValue( m_inidata->dirlight_col ) / 255.0f;

	D3DXVECTOR3 setdir;
	D3DXMATRIX invview;

	D3DXMatrixInverse( &invview, NULL, &(m_app->m_matView) );
	invview._41 = 0.0f;
	invview._42 = 0.0f;
	invview._43 = 0.0f;

	D3DXVec3TransformCoord( &setdir, &m_lightvec, &invview );
	D3DXVec3Normalize( &setdir, &setdir );

	ret = m_light->SetDirectionalLight( setdir, difcol );
	if( ret ){
		DbgOut( "lightdlg : ParamsToLight : SetDirectionalLight error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_light->lblend = m_lblend;


/// lightのセット

    D3DLIGHT9 light;                 // Description of the D3D light
    ZeroMemory( &light, sizeof(light) );

	CLightData* curlight = m_light;
	while( curlight ){
		switch( curlight->Type ){
		case D3DLIGHT_DIRECTIONAL:

			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Direction = curlight->orgDirection;
			
			light.Diffuse.r = curlight->Diffuse.x;
			light.Diffuse.g = curlight->Diffuse.y;
			light.Diffuse.b = curlight->Diffuse.z;

			light.Specular.r = g_specular;
			light.Specular.g = g_specular;
			light.Specular.b = g_specular;

			light.Ambient.r = g_ambient;
			light.Ambient.g = g_ambient;
			light.Ambient.b = g_ambient;

			m_pdev->SetLight( curlight->lightid, &light );
			m_pdev->LightEnable( curlight->lightid, TRUE );

			break;
		case D3DLIGHT_POINT:
			light.Type = D3DLIGHT_POINT;
			
			light.Diffuse.r = curlight->Diffuse.x;
			light.Diffuse.g = curlight->Diffuse.y;
			light.Diffuse.b = curlight->Diffuse.z;

			light.Specular.r = g_specular;
			light.Specular.g = g_specular;
			light.Specular.b = g_specular;

			light.Ambient.r = g_ambient;
			light.Ambient.g = g_ambient;
			light.Ambient.b = g_ambient;

			light.Position = curlight->orgPosition;

			if( curlight->Range != 0.0f )
				light.Range = sqrtf( curlight->Range );

			light.Attenuation0 = 0.4f;

			m_pdev->SetLight( curlight->lightid, &light );
			m_pdev->LightEnable( curlight->lightid, TRUE );


			break;
		case D3DLIGHT_SPOT:
			light.Type = D3DLIGHT_SPOT;
			
			light.Diffuse.r = curlight->Diffuse.x;
			light.Diffuse.g = curlight->Diffuse.y;
			light.Diffuse.b = curlight->Diffuse.z;

			light.Specular.r = g_specular;
			light.Specular.g = g_specular;
			light.Specular.b = g_specular;

			light.Ambient.r = g_ambient;
			light.Ambient.g = g_ambient;
			light.Ambient.b = g_ambient;
			
			light.Position = curlight->orgPosition;

			light.Direction = curlight->orgDirection;

			light.Range = sqrtf( curlight->Range );

			light.Falloff = 1.0f;
			light.Attenuation0 = 1.0f;
    
			light.Theta        = curlight->Phi;
			light.Phi          = curlight->Phi;

			m_pdev->SetLight( curlight->lightid, &light );
			m_pdev->LightEnable( curlight->lightid, TRUE );

			
			break;
		default:
			m_pdev->LightEnable( curlight->lightid, FALSE );

			break;
		}

		curlight = curlight->nextlight;
	}


//	ret = m_app->SetShaderConst( 1 );
//	_ASSERT( !ret );


	return 0;
}

LRESULT CLightDlg::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//HDC hdc;
	//hdc = ((LPDRAWITEMSTRUCT)lParam)->hDC;
	//_ASSERT( hdc );
	//int id;
	//id = (int)wParam;

	return 0;
}
LRESULT CLightDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	int timerid = (int)wParam;
	if( timerid == m_timerid ){
		BOOL dummy;
		if( (m_ref == 0) && (m_cmdshow != SW_HIDE) ){
			OnPaint( 0, 0, 0, dummy );

			ret = Render();
			_ASSERT( !ret );
		}

		OnMouseMove( 0, 0, 0, dummy );
	}

	return 0;
}

LRESULT CLightDlg::OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;
	
	::ReleaseDC( m_color_wnd, m_dc );
	m_dc = NULL;

	if( m_timerid != 0 ){
		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;
	}

	return 0;
}


int CLightDlg::CreateSwapChain( LPDIRECT3DDEVICE9 pdev )
{
	HRESULT hres;

	m_pdev = pdev;

	if( m_createflag == 1 )
		return 0;

	if( !m_pdev ){
		DbgOut( "lightdlg : CreateSwapChain : 3ddevice NULL error !!!\n" );
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
		DbgOut( "lightdlg : CreateSwapChain : dev CreateAdditionalSwapChain error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	hres = m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "lightdlg : CreateSwapChain : swapchain GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DFORMAT fmt;
	fmt = D3DFMT_D16;	
	hres = m_pdev->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, fmt, m_d3dpp.MultiSampleType, 0, FALSE, &m_pZSurf, NULL );
	if( hres != D3D_OK ){
		DbgOut( "lightdlg : CreateSwapChain : dev CreateDepthStencilSurface error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERSPHTIMER, 100 );
		DbgOut( "lightdlg :CreateSwapChain : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERSPHTIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}

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
int CLightDlg::DestroySwapChain()
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

int CLightDlg::Render()
{
	HRESULT hres;
	int ret;

//DbgOut( "lightdlg : Render : %d %x %d, %d\r\n", m_cmdshow, m_pdev, m_createflag, SW_HIDE );

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
	radxz = m_app->m_degxz * (float)DEG2PAI;
	if( (m_app->m_degy != 90.0f) && (m_app->m_degy != -90.0f) )
		rady = m_app->m_degy * (float)DEG2PAI;
	else if(m_app->m_degy == 90.0f)
		rady = 89.995f * (float)DEG2PAI;
	else if( m_app->m_degy == -90.0f)
		rady = -89.995f * (float)DEG2PAI;

    vEyePt.x = sinf(radxz) * m_camdist * cosf( rady );// + m_targetpos.x;
    vEyePt.y = m_eye_y + m_camdist * sinf( rady );
	vEyePt.z = -cosf(radxz) * m_camdist * cosf( rady );// + m_targetpos.z;

    // Set the app view matrix for normal viewing
	D3DXMatrixLookAtLH( &m_matView, &vEyePt, &targetpos, &vUpVec );

    DWORD dwClipWidth  = ( m_rc.right - m_rc.left ) /2;
    DWORD dwClipHeight = ( m_rc.bottom - m_rc.top ) /2;

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	if( m_app->m_sph_sh ){
		//D3DXVECTOR3 vEyePt( 0.0f, 0.0f, 0.0f );
		ret = m_app->m_sph_sh->TransformDispData( vEyePt, m_app->m_sph_mh, &inimat, &m_matView, &m_matProjX, 1, 0 );//, bonearray, tra_boneonly );
		if( ret ){
			DbgOut( "lightdlg : Render : m_sph_sh->TransformDispData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		//ret = m_app->m_sph_sh->UpdateVertexBuffer( 0 );
		//if( ret ){
		//	DbgOut( "lightdlg : Render : m_sph_sh->UpdateVertexBuffer error !!!\n" );
		//	_ASSERT( 0 );
		//	return 1;
		//}

	}


/////// render

	hres = m_pdev->SetRenderTarget( 0, m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "lightdlg : render : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	hres = m_pdev->SetDepthStencilSurface( m_pZSurf );
	if( hres != D3D_OK ){
		DbgOut( "lightdlg : render : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
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
		DbgOut( "lightdlg : Render : dev SetViewport error !!!\n" );
		_ASSERT( 0 );
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
		DbgOut( "lightdlg : Render : dev BeginScene error !!!\n" );
		_ASSERT( 0 );
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
		DbgOut( "lightdlg : Render : 3ddev Clear error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_app->m_sph_sh ){
		ret = m_app->m_sph_sh->Render( 0, 0, m_pdev, 0, POLYDISP_ALL, m_app->m_sph_mh, 0, inimat, m_matView, m_matProjX, vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "lightdlg : Render arrow : error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

    // End the scene.
    m_pdev->EndScene();

	hres = m_pSwapChain->Present( NULL, NULL, (HWND)m_view_wnd, NULL, D3DPRESENT_DONOTWAIT );
	if( hres != D3D_OK ){
		DbgOut( "lightdlg : Render : 3ddev Present error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	//m_view_wnd.ValidateRect( &m_rc );

	return 0;
}



LRESULT CLightDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	POINT inview;
	::GetCursorPos( &inview );
	m_view_wnd.ScreenToClient( &inview );


	//DbgOut( "lightdlg : LButtonDown : %d %d\r\n", inview.x, inview.y );

	if( m_ref == 0 )
		m_lbutton = 1;//!!!!!!!!!!!!!!

	return 0;
}
LRESULT CLightDlg::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT inview;
	::GetCursorPos( &inview );
	m_view_wnd.ScreenToClient( &inview );


	//DbgOut( "lightdlg : LButtonUp : %d %d\r\n", inview.x, inview.y );


	m_lbutton = 0;//!!!!!!!!!!!!!
	
	return 0;
}

LRESULT CLightDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	RECT viewclient;
	m_view_wnd.GetClientRect( &viewclient );

	POINT inview;
	::GetCursorPos( &inview );
	m_view_wnd.ScreenToClient( &inview );

	/***
	RECT dlgclient;
	m_dlg_wnd.GetClientRect( &dlgclient );

	POINT indlg;
	::GetCursorPos( &indlg );
	m_dlg_wnd.ScreenToClient( &indlg );
	***/

	if( (m_mcap == 0) && (m_cmdshow != SW_HIDE) ){
		if( m_ref == 0 ){
			if( (inview.x >= viewclient.left) && (inview.x <= viewclient.right) && (inview.y >= viewclient.top) && (inview.y <= viewclient.bottom) ){
				SetCapture();
				//DbgOut( "lightdlg : MouseMove : SetCapture !!!!!!!!!!!!!!\r\n" );
				m_mcap = 1;
			}
		}
	}else{
		if( (m_ref == 0) && (m_moving == 0) && (m_cmdshow != SW_HIDE) ){
			if( (inview.x < viewclient.left) || (inview.x > viewclient.right) || (inview.y < viewclient.top) || (inview.y > viewclient.bottom) ){
			//if( (indlg.x < dlgclient.left) || (indlg.x > dlgclient.right) || (indlg.y < dlgclient.top) || (indlg.y > dlgclient.bottom) ){
				ReleaseCapture();
				//DbgOut( "lightdlg: MouseMove : ReleaseCapture !!!!!!!!!!!\r\n" );
				m_mcap = 0;
				m_lbutton = 0;//!!!!!!!!!!
			}
		}
	}


	if( (m_ref == 0) && (m_mcap == 1) && (m_lbutton == 1) ){
		PickFaceAndSetLight( inview.x, inview.y );
	}

	return 0;
}

int CLightDlg::PickFaceAndSetLight( int srcx, int srcy )
{
	int ret;
	float maxdist = m_proj_far + 10000.0f;
	int part, face;
	D3DXVECTOR3 pos, n;
	float facedist;


	ret = m_app->m_sph_sh->PickFace( maxdist, m_viewport.Width, m_viewport.Height, 0, m_app->m_sph_mh, m_matWorld, m_matView, m_matProjX, 
		0, 0,
		srcx, srcy, &part, &face, &pos, &n, &facedist, 1 );
	if( ret ){
		DbgOut( "lightdlg : PickFaceAndSetLight : shandler PickFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (part >= 0) && (face >= 0) ){
		D3DXVECTOR3 dir;
		if( m_inverse == 0 )
			dir = -pos;
		else
			dir = pos;

		D3DXVec3Normalize( &dir, &dir );

		D3DXMATRIX matview;
		matview = m_matView;
		matview._41 = 0.0f;
		matview._42 = 0.0f;
		matview._43 = 0.0f;
		
		D3DXVec3TransformCoord( &m_lightvec, &dir, &matview );
		D3DXVec3Normalize( &m_lightvec, &m_lightvec );

		ParamsToLight();
	}


	return 0;
}

LRESULT CLightDlg::OnInverse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_INVERSE );
	if( checkflag == BST_CHECKED ){
		m_inverse = 1;
	}else{
		m_inverse = 0;
	}

	m_lightvec *= -1.0f;

	ParamsToLight();

	return 0;
}


LRESULT CLightDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CLightDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}


LRESULT CLightDlg::OnMult(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_MULT );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_MULT;
		ParamsToLight();
	}
	return 0;
}
LRESULT CLightDlg::OnScreen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_SCREEN );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_SCREEN;
		ParamsToLight();
	}

	return 0;
}
LRESULT CLightDlg::OnOverlay(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_OVERLAY );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_OVERLAY;
		ParamsToLight();
	}

	return 0;
}
LRESULT CLightDlg::OnHardlight(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_HARDLIGHT );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_HARDLIGHT;
		ParamsToLight();
	}

	return 0;
}
LRESULT CLightDlg::OnDodge(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_DODGE );
	if( checkflag == BST_CHECKED ){
		m_lblend = LBLEND_DODGE;
		ParamsToLight();
	}

	return 0;
}

int CLightDlg::SetIniMaterial2Sph()
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

