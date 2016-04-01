// AxisDlg.cpp : CAxisDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "AxisDlg.h"

#define DBGH
#include <dbg.h>


#include <Commdlg.h>
//#include <Afxdlgs.h>


#include <shdhandler.h>
#include <mothandler.h>

#include "D3DFont.h"


#define RENDERAXISTIMER	10000

//extern
extern int g_rendertarget;
extern ID3DXEffect*	g_pEffect;
extern D3DXHANDLE g_hmView;
extern D3DXHANDLE g_hmProj;

extern int g_endapp;

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg

CAxisDlg::CAxisDlg( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp )
{
	m_pd3ddev = pd3ddev;
	m_d3dpp = d3dpp;

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


	m_timerid = 0;
	m_timerworking = 0;
	m_createflag = 0;

	m_cmdshow = SW_HIDE;
	m_invalidateflag = 0;

////
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matProjX );

	m_proj_fov = 45.0f * (float)DEG2PAI;

	m_proj_far = 10000.0f;
	m_proj_near = 100.0f;
	
	m_degy = 0.0f;
	m_degxz = 0.0f;
	m_camdist = 2900.0f;
	m_eye_y = 750.0f;

	m_ground_sh = 0;
	m_arrow_sh = 0;

	m_ground_mh = 0;
	m_arrow_mh = 0;

	ZeroMemory( &m_smallgband, sizeof( GUARDBAND ) );

	m_arrow_degxz = 0.0f;
	m_arrow_degy = 0.0f;

	//m_arrowmode = ARROWMODE_VERT;
	m_arrowmode = ARROWMODE_CROSS;


	m_pfont = 0;
}

CAxisDlg::~CAxisDlg()
{

	DestroySwapChain();

}

LRESULT CAxisDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	if( m_arrowmode == ARROWMODE_VERT ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_VERT, IDC_RADIO_CROSS, IDC_RADIO_VERT );
		m_sl_xz_wnd.EnableWindow( FALSE );
		m_sl_y_wnd.EnableWindow( FALSE );
	}else if( m_arrowmode == ARROWMODE_CUSTOM ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_VERT, IDC_RADIO_CROSS, IDC_RADIO_CUSTOM );
		m_sl_xz_wnd.EnableWindow( TRUE );
		m_sl_y_wnd.EnableWindow( TRUE );
	}else if( m_arrowmode == ARROWMODE_CROSS ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_VERT, IDC_RADIO_CROSS, IDC_RADIO_CROSS );
		m_sl_xz_wnd.EnableWindow( FALSE );
		m_sl_y_wnd.EnableWindow( FALSE );
	}

	if( m_createflag == 0 ){
		int ret;
		ret = CreateSwapChain( m_pd3ddev, m_d3dpp );
		if( ret ){
			DbgOut( "axisdlg : OnInitDailog : CreateSwapChain error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
	}


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CAxisDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	/***
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_VERT );
	if( checkflag == BST_CHECKED ){
		m_arrowmode = ARROWMODE_VERT;
	}else{
		m_arrowmode = ARROWMODE_CUSTOM;
	}
	***/

	//EndDialog(wID);
	ShowDlg( SW_HIDE );

	return 0;
}

LRESULT CAxisDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowDlg( SW_HIDE );

	return 0;
}

void CAxisDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	m_view_wnd = GetDlgItem( IDC_VIEW );

	m_sl_xz_wnd = GetDlgItem( IDC_SL_XZ );
	m_sl_y_wnd = GetDlgItem( IDC_SL_Y );

	m_text_xz_wnd = GetDlgItem( IDC_TEXT_SL_XZ );
	m_text_y_wnd = GetDlgItem( IDC_TEXT_SL_Y );

	m_sl_xz_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_xz_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)360 );
	m_sl_xz_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_arrow_degxz );
	SetSliderTic( m_sl_xz_wnd, 30, 0, 360 );

	m_sl_y_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)-90 );
	m_sl_y_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)90 );
	m_sl_y_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_arrow_degy );
	SetSliderTic( m_sl_y_wnd, 30, -90, 90 );

}

int CAxisDlg::ParamsToDlg()
{		
	if( g_endapp == 1 ){
		return 0;
	}

	char str[1024];
	sprintf_s( str, 1024, "XZ平面上の角度(ヨー) %.3f度", m_arrow_degxz );
	if( (g_endapp == 1) || !m_text_xz_wnd.IsWindow() )
		return 0;
	m_text_xz_wnd.SetWindowText( str );

	sprintf_s( str, 1024, "Y軸方向の角度（ピッチ） %.3f度", m_arrow_degy );
	if( (g_endapp == 1) || !m_text_y_wnd.IsWindow() )
		return 0;
	m_text_y_wnd.SetWindowText( str );

	if( (g_endapp == 1) || !m_sl_xz_wnd.IsWindow() )
		return 0;
	m_sl_xz_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_arrow_degxz );
	if( (g_endapp == 1) || !m_sl_y_wnd.IsWindow() )
		return 0;
	m_sl_y_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_arrow_degy );

	return 0;
}

LRESULT CAxisDlg::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//HDC hdc;
	//hdc = ((LPDRAWITEMSTRUCT)lParam)->hDC;
	//_ASSERT( hdc );
	//int id;
	//id = (int)wParam;

	return 0;
}

int CAxisDlg::CreateSwapChain( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp )
{
	if( m_createflag == 1 )
		return 0;

	if( !pd3ddev ){
		DbgOut( "axisdlg : CreateSwapChain : 3ddevice NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	DestroySwapChain();//!!!! 念のため。


	HRESULT hres;

	m_pd3ddev = pd3ddev;
	m_d3dpp = d3dpp;

	m_view_wnd = GetDlgItem( IDC_VIEW );
	m_view_wnd.GetClientRect( &m_rc );


	m_d3dpp.BackBufferWidth = m_rc.right - m_rc.left;
	m_d3dpp.BackBufferHeight = m_rc.bottom - m_rc.top;
	m_d3dpp.hDeviceWindow = (HWND)m_view_wnd;
	m_d3dpp.EnableAutoDepthStencil = FALSE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.BackBufferCount = 1;//!!!!

	hres = pd3ddev->CreateAdditionalSwapChain( &m_d3dpp, &m_pSwapChain );
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : CreateSwapChain : dev CreateAdditionalSwapChain error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	hres = m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : CreateSwapChain : swapchain GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DFORMAT fmt;
	fmt = D3DFMT_D16;	
	hres = pd3ddev->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, fmt, m_d3dpp.MultiSampleType, 0, FALSE, &m_pZSurf, NULL );
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : CreateSwapChain : dev CreateDepthStencilSurface error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERAXISTIMER, 100 );
		DbgOut( "axisdlg :CreateSwapChain : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERAXISTIMER );
		m_timerworking = 1;
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
int CAxisDlg::DestroySwapChain()
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


int CAxisDlg::Render()
{
	if( g_endapp == 1 ){
		return 0;
	}

	HRESULT hres;
	int ret;

	if( (m_cmdshow != SW_SHOW) || !m_pd3ddev || !m_createflag || m_invalidateflag )
		return 0;

//////// arrow dir
	if( m_arrowmode == ARROWMODE_VERT ){
		m_arrow_degxz = m_degxz;
		m_arrow_degy = m_degy;
		ParamsToDlg();
	}else{
		//そのまま
	}


	float arrowradxz, arrowrady;
	arrowradxz = m_arrow_degxz * (float)DEG2PAI;
	if( (m_arrow_degy != 90.0f) && (m_arrow_degy != -90.0f) )
		arrowrady = m_arrow_degy * (float)DEG2PAI;
	else if(m_arrow_degy == 90.0f)
		arrowrady = 89.995f * (float)DEG2PAI;
	else if( m_arrow_degy == -90.0f)
		arrowrady = -89.995f * (float)DEG2PAI;

	D3DXMATRIX arrowworld;
	D3DXMatrixRotationYawPitchRoll( &arrowworld, -arrowradxz, arrowrady, 0.0f );


////////transform	
    D3DXVECTOR3 vEyePt;
    D3DXVECTOR3 targetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	float radxz, rady;
	radxz = m_degxz * (float)DEG2PAI;
	if( (m_degy != 90.0f) && (m_degy != -90.0f) )
		rady = m_degy * (float)DEG2PAI;
	else if(m_degy == 90.0f)
		rady = 89.995f * (float)DEG2PAI;
	else if( m_degy == -90.0f)
		rady = -89.995f * (float)DEG2PAI;

    vEyePt.x = sinf(radxz) * m_camdist * cosf( rady );// + m_targetpos.x;
    vEyePt.y = m_eye_y + m_camdist * sinf( rady );
	vEyePt.z = -cosf(radxz) * m_camdist * cosf( rady );// + m_targetpos.z;

    // Set the app view matrix for normal viewing
	D3DXMatrixLookAtLH( &m_matView, &vEyePt, &targetpos, &vUpVec );

    DWORD dwClipWidth  = ( m_rc.right - m_rc.left ) /2;
    DWORD dwClipHeight = ( m_rc.bottom - m_rc.top ) /2;

	if( g_endapp == 1 ){
		return 0;
	}
	if( m_ground_sh ){
		//D3DXVECTOR3 vEyePt( 0.0f, 0.0f, 0.0f );

		ret = m_ground_sh->TransformDispData( vEyePt, m_ground_mh, &m_matWorld, &m_matView, &m_matProjX, 1, 0 );
		if( ret ){
			DbgOut( "axisdlg : Render : m_gournd_sh->TransformDispData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		//ret = m_ground_sh->UpdateVertexBuffer( 0 );
		//if( ret ){
		//	DbgOut( "axisdlg : Render : m_gournd_sh->UpdateVertexBuffer error !!!\n" );
		//	_ASSERT( 0 );
		//	return 1;
		//}
	}

	if( g_endapp == 1 ){
		return 0;
	}
	if( m_arrow_sh ){
		//D3DXVECTOR3 vEyePt( 0.0f, 0.0f, 0.0f );
		ret = m_arrow_sh->TransformDispData( vEyePt, m_arrow_mh, &arrowworld, &m_matView, &m_matProjX, 1, 0 );//, bonearray, tra_boneonly );
		if( ret ){
			DbgOut( "axisdlg : Render : m_arrow_sh->TransformDispData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		//ret = m_arrow_sh->UpdateVertexBuffer( 0 );
		//if( ret ){
		//	DbgOut( "axisdlg : Render : m_arrow_sh->UpdateVertexBuffer error !!!\n" );
		//	_ASSERT( 0 );
		//	return 1;
		//}

	}


/////// render

	if( g_endapp == 1 ){
		return 0;
	}
	hres = m_pd3ddev->SetRenderTarget( 0, m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : render : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}

	if( g_endapp == 1 ){
		return 0;
	}
	hres = m_pd3ddev->SetDepthStencilSurface( m_pZSurf );
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : render : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}


	g_rendertarget = TARGET_SUB1;

    m_viewport.X = 0;
    m_viewport.Y = 0;
    m_viewport.Height = m_rc.bottom - m_rc.top;
    m_viewport.Width =  m_rc.right - m_rc.left;
    m_viewport.MaxZ = 1.0f;
    m_viewport.MinZ = 0.0f;

    hres = m_pd3ddev->SetViewport(&m_viewport);
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : Render : dev SetViewport error !!!\n" );
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


	if( g_endapp == 1 ){
		return 0;
	}
	if( FAILED( m_pd3ddev->BeginScene() ) ){
		DbgOut( "axisdlg : Render : dev BeginScene error !!!\n" );
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
	hres = m_pd3ddev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
						 clearcol,
						 1.0f, 0L );
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : Render : 3ddev Clear error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_ground_sh && (m_arrowmode != ARROWMODE_CROSS) ){
		ret = m_ground_sh->Render( 0, 0, m_pd3ddev, 0, POLYDISP_ALL, m_ground_mh, 0, m_matWorld, m_matView, m_matProjX, vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "axisdlg : Render Ground: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_arrow_sh && (m_arrowmode != ARROWMODE_CROSS) ){
		ret = m_arrow_sh->Render( 0, 0, m_pd3ddev, 0, POLYDISP_ALL, m_arrow_mh, 0, arrowworld, m_matView, m_matProjX, vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "axisdlg : Render arrow : error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( (m_arrowmode == ARROWMODE_CROSS) && m_pfont ){
		char text[1024];
		sprintf_s( text, 1024, "no need of display \non this AUTO MODE" );
		m_pfont->DrawText( 2, 30, D3DCOLOR_ARGB(255,255,255,0), text );
	}


    // End the scene.
    m_pd3ddev->EndScene();

	hres = m_pSwapChain->Present( NULL, NULL, (HWND)m_view_wnd, NULL, D3DPRESENT_DONOTWAIT );
	if( hres != D3D_OK ){
		DbgOut( "axisdlg : Render : 3ddev Present error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	//m_view_wnd.ValidateRect( &m_rc );

	return 0;

}

LRESULT CAxisDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}

	int ret;

//DbgOut( "axisdlg : OnTimer\r\n" );
	int timerid = (int)wParam;
	if( (timerid == m_timerid) && (m_cmdshow != SW_HIDE) ){
		if( g_endapp == 1 ){
			return 0;
		}
		ret = Render();
		_ASSERT( !ret );
	}

	return 0;
}

int CAxisDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;

	ShowWindow( m_cmdshow );

	return 0;
}

int CAxisDlg::SetInvalidateFlag( int flag )
{
	m_invalidateflag = flag;

	return 0;
}

LRESULT CAxisDlg::OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_timerworking == 1 ){
		DestroyTimer();
	}

	return 0;
}
int CAxisDlg::DestroyTimer()
{
	int ret;
	if( m_timerid != 0 ){
		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;
	}
	m_timerworking = 0;

	return 0;
}

int CAxisDlg::SetRenderParams( float degy, float degxz, GUARDBAND gband, 
							  CShdHandler* groundsh, CMotHandler* groundmh, CShdHandler* arrowsh, CMotHandler* arrowmh,
							  CD3DFont* pfont )
{
	if( g_endapp == 1 ){
		return 0;
	}


	m_degy = degy;
	m_degxz = degxz;
//////// arrow dir
	if( m_arrowmode == ARROWMODE_VERT ){
		m_arrow_degxz = m_degxz;
		m_arrow_degy = m_degy;
		//ParamsToDlg();
	}

	m_ground_sh = groundsh;
	m_ground_mh = groundmh;
	m_arrow_sh = arrowsh;
	m_arrow_mh = arrowmh;

	float extmult = 1.20f;
	m_smallgband.maxrhw = gband.maxrhw;

	m_smallgband.left = max( gband.left, -extmult * (m_rc.right - m_rc.left) );
	m_smallgband.top = max( gband.top, -extmult * (m_rc.bottom - m_rc.top) );
	m_smallgband.right = min( gband.right, extmult * (m_rc.right - m_rc.left) );
	m_smallgband.bottom = min( gband.bottom, extmult * (m_rc.bottom - m_rc.top) );

	m_pfont = pfont;

	return 0;
}


LRESULT CAxisDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND scwnd = (HWND)lParam;
	WORD event;

	event = LOWORD( wParam );

	int curpos;
	if( (event = SB_THUMBTRACK) || (event == SB_THUMBPOSITION ) ){
		if( scwnd == (HWND)m_sl_xz_wnd ){
			curpos = (int)m_sl_xz_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			m_arrow_degxz = (float)curpos;
		}else if( scwnd == (HWND)m_sl_y_wnd ){
			curpos = (int)m_sl_y_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			m_arrow_degy = (float)curpos;
		}
		ParamsToDlg();
	}

	return 0;
}

void CAxisDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	if( g_endapp == 1 ){
		return;
	}


	// !!! minticとmaxticには、自動的にticが入るので設定はしない。(してはいけない)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		if( g_endapp == 1 ){
			return;
		}
		dstwnd.SendMessage( TBM_SETTIC, 0, (LPARAM)(LONG)curtic );
		curtic += ticfreq;
	}
}

LRESULT CAxisDlg::OnRadioVert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_VERT );
	if( checkflag == BST_CHECKED ){
		m_arrowmode = ARROWMODE_VERT;
	}

	m_sl_xz_wnd.EnableWindow( FALSE );
	m_sl_y_wnd.EnableWindow( FALSE );

	//::MessageBox( NULL, "OnRadioVert", "radio", MB_OK );

	return 0;
}
LRESULT CAxisDlg::OnRadioCustom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_CUSTOM );
	if( checkflag == BST_CHECKED ){
		m_arrowmode = ARROWMODE_CUSTOM;
	}

	m_sl_xz_wnd.EnableWindow( TRUE );
	m_sl_y_wnd.EnableWindow( TRUE );

	//::MessageBox( NULL, "OnRadioCustom", "radio", MB_OK );

	return 0;
}
LRESULT CAxisDlg::OnRadioCross(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_CROSS );
	if( checkflag == BST_CHECKED ){
		m_arrowmode = ARROWMODE_CROSS;
	}

	m_sl_xz_wnd.EnableWindow( FALSE );
	m_sl_y_wnd.EnableWindow( FALSE );

	return 0;
}
/***
LRESULT CAxisDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DbgOut( "axisdlg : KeyDown\r\n" );

	return 0;
}
***/

/***
BOOL CAxisDlg::PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
{


DbgOut( "axisdlg : PreTranslateAccelerator\r\n" );

		//if( pMsg->message == WM_KEYDOWN ){
		//	DbgOut( "SigTreeView2 : PreTranslateAccelerator : KEYDOWN : %d, ctrl %d, shift %d\n", pMsg->wParam, VK_CONTROL, VK_SHIFT );
		//}else if( pMsg->message == WM_KEYUP ){
		//	DbgOut( "SigTreeView2 : PreTranslateAccelerator : KEYUP : %d, ctrl %d, shift %d\n", pMsg->wParam, VK_CONTROL, VK_SHIFT );
		//}


		if(pMsg->message == WM_KEYDOWN)
		{
			switch(pMsg->wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
			case VK_HOME:
			case VK_END:
			case VK_NEXT:
			case VK_PRIOR:
				hRet = S_FALSE;
				return TRUE;
			}
		}
		//TODO: アクセラレータ初期コードをここに追加してください。
		return FALSE;
}
***/
