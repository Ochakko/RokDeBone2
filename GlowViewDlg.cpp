// GlowViewDlg.cpp : CGlowViewDlg のインプリメンテーション
#include "stdafx.h"
#include "GlowViewDlg.h"

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
#include <texbank.h>

#include <crtdbg.h>

#define RENDERGLOWTIMER	11010

#define ORGSIZE		256
#define SMALLSIZE	64

//------- extern
extern CColDlg g_coldlg;
extern int g_rendertarget;
extern ID3DXEffect*	g_pEffect;
extern D3DXHANDLE g_hmView;
extern D3DXHANDLE g_hmProj;
extern CTexBank* g_texbnk;
extern CIniData* g_inidata;
//------- static
static int s_usertexcnt = 0;

/////////////////////////////////////////////////////////////////////////////
// CGlowViewDlg

CGlowViewDlg::CGlowViewDlg( LPDIRECT3DDEVICE9 pdev, CMyD3DApplication* papp, CIniData* pini, CLightData* pl )
{
	m_timerid = 0;

	m_createflag = 0;
	m_cmdshow = SW_HIDE;

	m_pdev = pdev;
	m_app = papp;
	m_inidata = pini;
	m_light = pl;
	m_d3dpp = papp->m_d3dpp;

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

    m_viewport_org.X = 0;
    m_viewport_org.Y = 0;
    m_viewport_org.Height = 0;
    m_viewport_org.Width =  0;
    m_viewport_org.MaxZ = 1.0f;
    m_viewport_org.MinZ = 0.0f;

    m_viewport_small.X = 0;
    m_viewport_small.Y = 0;
    m_viewport_small.Height = 0;
    m_viewport_small.Width =  0;
    m_viewport_small.MaxZ = 1.0f;
    m_viewport_small.MinZ = 0.0f;

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

	ZeroMemory( &m_RTOrg, sizeof( RTTEXELEM ) );
	ZeroMemory( &m_RTGlow, sizeof( RTTEXELEM ) );
	ZeroMemory( &m_RTGlowSmall, sizeof( RTTEXELEM ) );
	ZeroMemory( &m_RTWork, sizeof( RTTEXELEM ) );
	ZeroMemory( &m_RTGauss, sizeof( RTTEXELEM ) );
	ZeroMemory( &m_RTFinal, sizeof( RTTEXELEM ) );


}

CGlowViewDlg::~CGlowViewDlg()
{
	DestroySwapChain();
}

int CGlowViewDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	m_view_wnd = GetDlgItem( IDC_VIEW );

	return 0;
}

int CGlowViewDlg::ParamsToDlg()
{

	return 0;
}

LRESULT CGlowViewDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//DbgOut( "GlowViewDlg : OnInitDialog \r\n" );

	int ret = 0;
	ret = SetWnd();
	_ASSERT( !ret );

	ret = ParamsToDlg();
	_ASSERT( !ret );

	if( m_createflag == 0 ){
		int ret;
		ret = CreateSwapChain( m_pdev );
		if( ret ){
			DbgOut( "GlowViewDlg : OnInitDailog : CreateSwapChain error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
	}

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CGlowViewDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	ParamsToDlg();

	ShowDlg( SW_HIDE );
	
//	::MessageBox( m_app->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_GLOW, 0 );


	//EndDialog(wID);

	return 0;
}

LRESULT CGlowViewDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ParamsToDlg();

	ShowDlg( SW_HIDE );

//	::MessageBox( m_app->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_GLOW, 0 );

	//EndDialog(wID);

	return 0;
}

int CGlowViewDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;	
	ShowWindow( m_cmdshow );
	
	ParamsToDlg();

	return 0;	
}
LRESULT CGlowViewDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	int timerid = (int)wParam;
	if( timerid == m_timerid ){

		if( (::IsWindow( m_dlg_wnd )) && (::IsWindow( m_view_wnd )) && (m_cmdshow != SW_HIDE) ){
			ret = Render();
			_ASSERT( !ret );
		}

	}

	return 0;
}

LRESULT CGlowViewDlg::OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	DestroyTimer();

	return 0;
}


int CGlowViewDlg::CreateSwapChain( LPDIRECT3DDEVICE9 pdev )
{
	HRESULT hres;
	int ret;

	m_pdev = pdev;

	if( m_createflag == 1 )
		return 0;

	if( !m_pdev ){
		DbgOut( "GlowViewDlg : CreateSwapChain : 3ddevice NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	DestroySwapChain();//!!!! 念のため。


	m_view_wnd = GetDlgItem( IDC_VIEW );
	m_view_wnd.GetClientRect( &m_rc );

	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = m_rc.right - m_rc.left;
	m_viewport.Height = m_rc.bottom - m_rc.top;
    m_viewport.MaxZ = 1.0f;
    m_viewport.MinZ = 0.0f;

	m_viewport_org.X = 0;
	m_viewport_org.Y = 0;
	m_viewport_org.Width = ORGSIZE;
	m_viewport_org.Height = ORGSIZE;
    m_viewport_org.MaxZ = 1.0f;
    m_viewport_org.MinZ = 0.0f;

	m_viewport_small.X = 0;
	m_viewport_small.Y = 0;
	m_viewport_small.Width = SMALLSIZE;
	m_viewport_small.Height = SMALLSIZE;
    m_viewport_small.MaxZ = 1.0f;
    m_viewport_small.MinZ = 0.0f;


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
		DbgOut( "GlowViewDlg : CreateSwapChain : dev CreateAdditionalSwapChain error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	hres = m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "GlowViewDlg : CreateSwapChain : swapchain GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DFORMAT fmt;
	fmt = D3DFMT_D16;	
	hres = m_pdev->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, fmt, m_d3dpp.MultiSampleType, 0, FALSE, &m_pZSurf, NULL );
	if( hres != D3D_OK ){
		DbgOut( "GlowViewDlg : CreateSwapChain : dev CreateDepthStencilSurface error %x !!!\n", hres );
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

/////// rendertarget
//	RTTEXELEM	m_RTOrg;
//	RTTEXELEM	m_RTGlow;
//	RTTEXELEM	m_RTGlowSmall;
//	RTTEXELEM	m_RTWork;
//	RTTEXELEM	m_RTGauss;
//	RTTEXELEM	m_RTFinal;
//int CGlowViewDlg::CreateRenderTargetTexture( LPDIRECT3DDEVICE9 pdev, SIZE size, int srcfmt, int needz, RTTEXELEM* prtte )
	SIZE orgsize = { ORGSIZE, ORGSIZE };
	SIZE smallsize = { SMALLSIZE, SMALLSIZE };
	ret = CreateRenderTargetTexture( pdev, orgsize, D3DFMT_A8R8G8B8, 1, &m_RTOrg );
	if( ret ){
		DbgOut( "glowview : CreateSwapChain : CreateRenderTargetTexture RTOrg error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = CreateRenderTargetTexture( pdev, orgsize, D3DFMT_A8R8G8B8, 0, &m_RTGlow );
	if( ret ){
		DbgOut( "glowview : CreateSwapChain : CreateRenderTargetTexture RTGlow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = CreateRenderTargetTexture( pdev, smallsize, D3DFMT_A8R8G8B8, 0, &m_RTGlowSmall );
	if( ret ){
		DbgOut( "glowview : CreateSwapChain : CreateRenderTargetTexture RTGlowSmall error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = CreateRenderTargetTexture( pdev, smallsize, D3DFMT_A8R8G8B8, 0, &m_RTWork );
	if( ret ){
		DbgOut( "glowview : CreateSwapChain : CreateRenderTargetTexture RTWork error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = CreateRenderTargetTexture( pdev, smallsize, D3DFMT_A8R8G8B8, 0, &m_RTGauss );
	if( ret ){
		DbgOut( "glowview : CreateSwapChain : CreateRenderTargetTexture RTGauss error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = CreateRenderTargetTexture( pdev, orgsize, D3DFMT_A8R8G8B8, 0, &m_RTFinal );
	if( ret ){
		DbgOut( "glowview : CreateSwapChain : CreateRenderTargetTexture RTFinal error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_createflag = 1;

	return 0;
}
int CGlowViewDlg::DestroySwapChain()
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

	DestroyRTTexElem( &m_RTOrg );
	DestroyRTTexElem( &m_RTGlow );
	DestroyRTTexElem( &m_RTGlowSmall );
	DestroyRTTexElem( &m_RTWork );
	DestroyRTTexElem( &m_RTGauss );
	DestroyRTTexElem( &m_RTFinal );


	m_createflag = 0;

	return 0;
}

int CGlowViewDlg::DestroyRTTexElem( RTTEXELEM* prtte )
{
	if( !prtte )
		return 0;

	if( prtte->psurf ){
		prtte->psurf->Release();
		prtte->psurf = 0;
	}

	if( prtte->pZsurf ){
		prtte->pZsurf->Release();
		prtte->pZsurf = 0;
	}

	if( prtte->ptex ){
		if( !g_texbnk ){
			DbgOut( "DestroyRTTexElem: texbank NULL error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}
		int delindex = -1;
		delindex = g_texbnk->GetIndexBySerial( prtte->texid );
		if( delindex < 0 ){
			_ASSERT( 0 );
			return 0;
		}
		int ret;
		ret = g_texbnk->DestroyTexture( delindex );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CGlowViewDlg::Render()
{
	HRESULT hres;
	int ret;

	if( (m_cmdshow == SW_HIDE) || !m_pdev || !m_createflag )// || m_invalidateflag )
		return 0;

	int presentflag = 0;

	if( m_app->m_ExistData ){
		ret = m_app->Render3D( m_RTOrg.psurf, m_RTOrg.pZsurf, &m_viewport_org, presentflag );
		if( ret ){
			DbgOut( "GlowViewDlg : Render : app Render3D 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		ret = m_app->RTBeginScene( m_pdev, m_RTOrg.psurf, 0, &m_viewport_org, 0, 0 );
		if( ret ){
			DbgOut( "GlowViewDlg : Render : app RTBeginScene error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = m_pdev->EndScene();
		_ASSERT( !ret );
	}

	int zclear = 0;
	ret = m_app->RenderGlow( m_RTGlow.psurf, m_RTOrg.pZsurf, &m_viewport_org, zclear );
	if( ret ){
		DbgOut( "GlowViewDlg : Render : app RenderGlow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	E3DCOLOR4F col = {1.0f, 1.0f, 1.0f, 1.0f};
	ret = m_app->BlendRTTexture( m_RTGlowSmall.psurf, &m_viewport_small, 
		m_RTGlow.ptex, m_viewport_org.Width, m_viewport_org.Height, col, D3DTEXF_LINEAR,
		0, 0, 0, col, 0, 
		D3DBLENDOP_ADD, D3DBLEND_ONE, D3DBLEND_ONE );
	if( ret ){
		DbgOut( "GlowViewDlg : Render : app BlendRTTexture : glow->glowsmall error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_app->BlurTextureGauss( m_RTGauss.psurf, &m_viewport_small,
		m_RTWork.psurf, m_RTWork.ptex, &m_viewport_small,
		m_RTGlowSmall.ptex, &m_viewport_small, 20.0f );
	if( ret ){
		DbgOut( "GlowViewDlg : Render : app BlurTextureGauss error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_app->BlendRTTexture( m_RTFinal.psurf, &m_viewport_org,
		m_RTOrg.ptex, m_viewport_org.Width, m_viewport_org.Height, col, D3DTEXF_LINEAR,
		m_RTGauss.ptex, m_viewport_small.Width, m_viewport_small.Height, col, D3DTEXF_LINEAR,
		D3DBLENDOP_ADD, D3DBLEND_ONE, D3DBLEND_ONE );
	if( ret ){
		DbgOut( "GlowViewDlg : Render : app BlendRTTexture : org + gauss error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_app->RenderFullScreenTLV( m_pBackBuffer, &m_viewport, m_RTFinal.ptex, m_viewport_org.Width, m_viewport_org.Height, D3DTEXF_LINEAR );
	if( ret ){
		DbgOut( "GlowViewDlg : Render : RenderFullScreenTLV : glow->BackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	hres = m_pSwapChain->Present( NULL, NULL, (HWND)m_view_wnd, NULL, D3DPRESENT_DONOTWAIT );
	if( hres != D3D_OK ){
		DbgOut( "GlowViewDlg : Render : 3ddev Present error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


int CGlowViewDlg::CreateTimer()
{
	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERGLOWTIMER, 30 );
		DbgOut( "GlowViewDlg :CreateSwapChain : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERGLOWTIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}
	return 0;
}
int CGlowViewDlg::DestroyTimer()
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

int CGlowViewDlg::CreateRenderTargetTexture( LPDIRECT3DDEVICE9 pdev, SIZE size, int srcfmt, int needz, RTTEXELEM* prtte )
{
	int ret;
	HRESULT hres;

	if( !g_texbnk ){
		DbgOut( "CreateRenderTargetTexture : texbank NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int newserial = -1;
	int newindex = -1;
	char usertexname[ MAX_PATH ];
	s_usertexcnt++;
	sprintf_s( usertexname, MAX_PATH, "RenderTarget_%d", s_usertexcnt );

	int transparent = 0;
	int pool = D3DPOOL_DEFAULT;

	newserial = g_texbnk->AddName( 0, usertexname, transparent, pool );
	if( newserial <= 0 ){
		DbgOut( "CreateRenderTargetTexture : texbnk AddName error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
		
	newindex = g_texbnk->FindName( usertexname, transparent );
	if( newindex < 0 ){
		DbgOut( "CreateRenderTargetTexture : texbnk FindName error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = g_texbnk->SetFMT( newindex, srcfmt );
	_ASSERT( !ret );

	ret = g_texbnk->CreateTexData( newindex, pdev, size.cx, size.cy );
	if( ret ){
		DbgOut( "CreateRenderTargetTexture : texbnk CreateTexData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
////////////
	ret = g_texbnk->GetTexDataBySerial( newserial, &(prtte->ptex) );
	if( ret || !prtte->ptex ){
		DbgOut( "CreateRenderTargetTexture : texbank GetTexDataBySerial error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	hres = prtte->ptex->GetSurfaceLevel( 0, &(prtte->psurf) );
	if( hres != D3D_OK ){
		DbgOut( "CreateRenderTargetTexture : tex GetSrufaceLevel error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int width, height, pool2, transparent2, format;
	ret = g_texbnk->GetTextureInfoBySerial( newserial, &width, &height, &pool2, &transparent2, &format );
	if( ret ){
		DbgOut( "CreateRenderTargetTexture : texbank GetTextureSizeBySerial error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
		
	if( needz ){
		D3DFORMAT fmt;
		fmt = D3DFMT_D16;
		hres = pdev->CreateDepthStencilSurface( width, height, fmt, 
			D3DMULTISAMPLE_NONE,
			0,
			FALSE, &(prtte->pZsurf), NULL ); 
		if( hres != D3D_OK ){
			DbgOut( "CreateRenderTargetTexture : dev CreateDepthStencilSurface error %x !!!\n", hres );
			_ASSERT( 0 );
			return 1;
		}
	}

	prtte->width = width;
	prtte->height = height;
	prtte->texid = newserial;

	return 0;
}

