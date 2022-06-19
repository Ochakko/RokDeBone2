//-----------------------------------------------------------------------------
// File: D3DApp.cpp
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) 1998-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <D3D9.h>
#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DRes.h"

#include "MotDlg.h"

#include <commctrl.h>

#include "resource.h"

#include <coef.h>

#define DBGH
#include <dbg.h>

#include "inidata.h" // for ini file


//extern rokdebone2.cpp
extern ID3DXEffect*	g_pEffect;       // D3DX effect interface
extern CIniData* g_inidata;
extern HWND g_hStatus;
//extern CAxisDlg* g_axisdlg;
extern CMotDlg* g_motdlg;


// tool bar

#define ID_TOOLBAR	500
extern TBBUTTON tbb[22];
extern TBBUTTON tb_space;
extern HWND g_hToolBar;


//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------

int g_useGPU = 0;
int g_usePS = 0;
int g_chkVS = 0;

int g_bumpEnable = 0;
int g_bumpUse = 0;

int MAXSKINMATRIX = 42;//!!!!!!!!!

//--------- static 
static CD3DApplication* g_pD3DApp = NULL;




//-----------------------------------------------------------------------------
// Name: CD3DApplication()
// Desc: Constructor
//-----------------------------------------------------------------------------
CD3DApplication::CD3DApplication()
{
	m_hbmp = NULL;
	m_InitFlag = 0;

	g_pD3DApp           = this;

    m_dwNumAdapters     = 0;
    m_dwAdapter         = 0L;
    m_pD3D              = NULL;
    m_pd3dDevice        = NULL;
    m_hWnd              = NULL;
    m_hWndFocus         = NULL;
    m_bActive           = FALSE;
    m_bReady            = FALSE;
    m_dwCreateFlags     = 0L;
	m_screensave		= 0;

    m_bFrameMoving      = TRUE;
    m_bSingleStep       = FALSE;
    m_fFPS              = 0.0f;
    m_strDeviceStats[0] = _T('\0');
    m_strFrameStats[0]  = _T('\0');

    m_strWindowTitle    = _T("D3D8 Application");
    //m_dwCreationWidth   = 400;
    //m_dwCreationHeight  = 300;
//m_bUseDepthBuffer   = FALSE;
m_bUseDepthBuffer = TRUE;

	m_dwMinDepthBits    = 16;
    m_dwMinStencilBits  = 0;
    m_bShowCursorWhenFullscreen = FALSE;


	HWND hWnd;
	RECT rect;
    hWnd = ::GetDesktopWindow();
    GetWindowRect(hWnd, &rect);
    //m_dwCreationWidth   = (DWORD)((float)(rect.right - rect.left) * 0.6f);
    //m_dwCreationHeight  = (DWORD)((float)(rect.bottom - rect.top) * 0.6f) ;    


	OpenDbgFile();

	//moduledirのセット
	SetModuleDir();

	g_inidata->LoadParams( "rdb2.ini" );//!!!!!!!!!!!!
	m_dwCreationWidth = (DWORD)( g_inidata->main_width );
	m_dwCreationHeight = (DWORD)( g_inidata->main_height );


	m_bluebrush = NULL;

	m_writebmpflag = 0;

	m_force_render = 0;
	m_rendertime = 0;

	m_lbutton = 0;
	m_movebone = 0;

	m_rbutton = 0;
	m_mbutton = 0;
	m_shiftbone = 0;
	m_shiftpart = -1;
	m_shifvertnum = 0;
	ZeroMemory( m_shifvert, sizeof( int ) * PICKVERTMAX );
	m_shiftface = -1;

	m_onselectplugin = 0;

	m_intormenu = 0;
	m_scalebone = 0;

	m_lrbutton = 0;

	m_vikflag = 0;

	m_pZSurf = 0;
	m_pBackBuffer = 0;
	m_resizeflag = 0;

	m_timerkind = 0;
	m_freq.HighPart = 0;
	m_freq.LowPart = 1;
	m_renderfreq = 0;
	m_rendercnt.HighPart = 0;
	m_rendercnt.LowPart = 0;
	m_curcnt.HighPart = 0;
	m_curcnt.LowPart = 0;

	m_preview_flag = 0;

//	m_MultiSampleType = D3DMULTISAMPLE_NONE;

}




//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return g_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Create( HINSTANCE hInstance )
{
    HRESULT hr;

	QueryPerformanceFrequency( &m_freq );
	if( m_freq.LowPart == 0 ){
		m_timerkind = 0;
		m_renderfreq = 1;
	}else{
		m_timerkind = 1;
		m_renderfreq = m_freq.LowPart / 60;
	}

	DbgOut( "Create : freq : Hight %d, Low %d\n", m_freq.HighPart, m_freq.LowPart );

	InitCommonControls();//!!!!!!!!!!!


    // Create the Direct3D object
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( m_pD3D == NULL )
        return DisplayErrorMsg( D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT );

    // Build a list of Direct3D adapters, modes and devices. The
    // ConfirmDevice() callback is used to confirm that only devices that
    // meet the app's requirements are considered.
    if( FAILED( hr = BuildDeviceList() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Unless a substitute hWnd has been specified, create a window to
    // render into
    if( m_hWnd == NULL)
    {

		if( m_bluebrush == NULL ){
			//COLORREF bluecol = RGB( 0, 0, 255 );
			//m_bluebrush = ::CreateSolidBrush( bluecol );
			//_ASSERT( m_bluebrush != NULL );

			unsigned char colr, colg, colb;
			colr = GetRValue( g_inidata->bg_color );
			colg = GetGValue( g_inidata->bg_color );
			colb = GetBValue( g_inidata->bg_color );
			COLORREF bluecol = RGB( colr, colg, colb );
			m_bluebrush = ::CreateSolidBrush( bluecol );
			_ASSERT( m_bluebrush != NULL );
		}


        // Register the windows class
        WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
                              //LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
							  LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON1) ),
                              LoadCursor( NULL, IDC_ARROW ),
                              //(HBRUSH)GetStockObject(WHITE_BRUSH),
							  m_bluebrush,
                              NULL, _T("D3D Window") };
        RegisterClass( &wndClass );

        // Set the window's initial style
        m_dwWindowStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
                          WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPCHILDREN;

        // Set the window's initial width
        RECT rc;
        SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );
        AdjustWindowRect( &rc, m_dwWindowStyle, TRUE );

        // Create the render window
        m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle, m_dwWindowStyle,
                               //CW_USEDEFAULT, CW_USEDEFAULT,
							   g_inidata->main_posx, g_inidata->main_posy,
                               g_inidata->main_width, g_inidata->main_height, 0L,
                               LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) ),
                               hInstance, 0L );
	
		//UpdateWindow( m_hWnd );

	}

    // The focus window can be a specified to be a different window than the
    // device window.  If not, use the device window as the focus window.
    if( m_hWndFocus == NULL )
        m_hWndFocus = m_hWnd;


	DestroyAppToolBar();
	int ret;
	ret = CreateAppToolBar();
	if( ret ){
		DbgOut( "d3dapp : CreateAppToolBar error !!!\n" );
        return DisplayErrorMsg( -1, MSGERR_APPMUSTEXIT );
	}


    // Save window properties
    m_dwWindowStyle = GetWindowLong( m_hWnd, GWLP_STYLE );
    GetWindowRect( m_hWnd, &m_rcWindowBounds );
    GetClientRect( m_hWnd, &m_rcWindowClient );

    // Initialize the app's custom scene stuff
    if( FAILED( hr = OneTimeSceneInit() ) )
	{
        //SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }
	
    // Initialize the 3D environment for the app
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }


    // Setup the app so it can support single-stepping
    DXUtil_Timer( TIMER_START );

    // The app is ready to go
    m_bReady = TRUE;
	m_InitFlag = 1;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList).
//-----------------------------------------------------------------------------
int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
    D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

    if( p1->Format > p2->Format )   return -1;
    if( p1->Format < p2->Format )   return +1;
    if( p1->Width  < p2->Width )    return -1;
    if( p1->Width  > p2->Width )    return +1;
    if( p1->Height < p2->Height )   return -1;
    if( p1->Height > p2->Height )   return +1;

    return 0;
}




//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::BuildDeviceList()
{
	HRESULT hr;
    const DWORD dwNumDeviceTypes = 2;
    const TCHAR* strDeviceDescs[] = { _T("HAL"), _T("REF") };
    const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

    BOOL bHALExists = FALSE;
    BOOL bHALIsWindowedCompatible = FALSE;
    BOOL bHALIsDesktopCompatible = FALSE;
    BOOL bHALIsSampleCompatible = FALSE;

	DWORD m, f;

    // Loop through all the adapters on the system (usually, there's just one
    // unless more than one graphics card is present).
    for( UINT iAdapter = 0; iAdapter < m_pD3D->GetAdapterCount(); iAdapter++ )
    {
        // Fill in adapter info
        D3DAdapterInfo* pAdapter  = &m_Adapters[m_dwNumAdapters];
        hr = m_pD3D->GetAdapterIdentifier( iAdapter, 0, &pAdapter->d3dAdapterIdentifier );
		if( hr != D3D_OK ){
			DbgOut( "app : BuildDeviceList : GetAdapterIdentifier error !!! %x\r\n", hr );
			_ASSERT( 0 );
		}
        hr = m_pD3D->GetAdapterDisplayMode( iAdapter, &pAdapter->d3ddmDesktop );
		if( hr != D3D_OK ){
			DbgOut( "app : BuildDeviceList : GetAdapterDisplayMode error !!! %x\r\n", hr );
			_ASSERT( 0 );
		}

        pAdapter->dwNumDevices    = 0;
        pAdapter->dwCurrentDevice = 0;

		D3DFORMAT supportfmt[5] = {
			D3DFMT_A8R8G8B8,
			D3DFMT_X8R8G8B8,
			D3DFMT_A1R5G5B5,
			D3DFMT_R5G6B5,
			D3DFMT_X1R5G5B5
		};
		D3DFORMAT curfmt;


        // Enumerate all display modes on this adapter
        D3DDISPLAYMODE modes[100];
        D3DFORMAT      formats[20];
        DWORD dwNumFormats      = 0;
        DWORD dwNumModes        = 0;
        DWORD dwNumAdapterModes = 0;
		
		int fmtno;
		for( fmtno = 0; fmtno < 5; fmtno++ ){
			curfmt = supportfmt[fmtno];

			dwNumAdapterModes += m_pD3D->GetAdapterModeCount( iAdapter, curfmt );

			// Add the adapter's current desktop format to the list of formats
			formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

			for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
			{
				// Get the display mode attributes
				D3DDISPLAYMODE DisplayMode;
				hr = m_pD3D->EnumAdapterModes( iAdapter, curfmt, iMode, &DisplayMode );
				if( hr != D3D_OK ){
					//DbgOut( "app : BuildDeviceList : EnumAdapterModes error !!! %x\r\n", hr );
					//_ASSERT( 0 );
					continue;
				}


				// Filter out low-resolution modes
				if( DisplayMode.Width  < 640 || DisplayMode.Height < 400 )
					continue;

				// Check if the mode already exists (to filter out refresh rates)
				for( m=0L; m<dwNumModes; m++ )
				{
					if( ( modes[m].Width  == DisplayMode.Width  ) &&
						( modes[m].Height == DisplayMode.Height ) &&
						( modes[m].Format == DisplayMode.Format ) )
						break;
				}

				// If we found a new mode, add it to the list of modes
				if( m == dwNumModes )
				{
					modes[dwNumModes].Width       = DisplayMode.Width;
					modes[dwNumModes].Height      = DisplayMode.Height;
					modes[dwNumModes].Format      = DisplayMode.Format;
					modes[dwNumModes].RefreshRate = 0;					
					
					dwNumModes++;

					// Check if the mode's format already exists
					for( f=0; f<dwNumFormats; f++ )
					{
						if( DisplayMode.Format == formats[f] )
							break;
					}

					// If the format is new, add it to the list
					if( f== dwNumFormats )
						formats[dwNumFormats++] = DisplayMode.Format;
				}
			}
		}
/***
        // Enumerate all display modes on this adapter
        D3DDISPLAYMODE modes[100];
        D3DFORMAT      formats[20];
        DWORD dwNumFormats      = 0;
        DWORD dwNumModes        = 0;
        DWORD dwNumAdapterModes = m_pD3D->GetAdapterModeCount( iAdapter );

        // Add the adapter's current desktop format to the list of formats
        formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

        for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
        {
            // Get the display mode attributes
            D3DDISPLAYMODE DisplayMode;
            m_pD3D->EnumAdapterModes( iAdapter, iMode, &DisplayMode );

            // Filter out low-resolution modes
            if( DisplayMode.Width  < 640 || DisplayMode.Height < 400 )
                continue;

            // Check if the mode already exists (to filter out refresh rates)
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                if( ( modes[m].Width  == DisplayMode.Width  ) &&
                    ( modes[m].Height == DisplayMode.Height ) &&
                    ( modes[m].Format == DisplayMode.Format ) )
                    break;
            }

            // If we found a new mode, add it to the list of modes
            if( m == dwNumModes )
            {
                modes[dwNumModes].Width       = DisplayMode.Width;
                modes[dwNumModes].Height      = DisplayMode.Height;
                modes[dwNumModes].Format      = DisplayMode.Format;
                modes[dwNumModes].RefreshRate = 0;
                dwNumModes++;

                // Check if the mode's format already exists
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( DisplayMode.Format == formats[f] )
                        break;
                }

                // If the format is new, add it to the list
                if( f== dwNumFormats )
                    formats[dwNumFormats++] = DisplayMode.Format;
            }
        }
***/

        // Sort the list of display modes (by format, then width, then height)
        qsort( modes, dwNumModes, sizeof(D3DDISPLAYMODE), SortModesCallback );

        // Add devices to adapter
        for( UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++ )
        {
            // Fill in device info
            D3DDeviceInfo* pDevice;
            pDevice                 = &pAdapter->devices[pAdapter->dwNumDevices];
            pDevice->DeviceType     = DeviceTypes[iDevice];
            m_pD3D->GetDeviceCaps( iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps );
            pDevice->strDesc        = strDeviceDescs[iDevice];
            pDevice->dwNumModes     = 0;
            pDevice->dwCurrentMode  = 0;
            pDevice->bCanDoWindowed = FALSE;
            pDevice->bWindowed      = FALSE;
            pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

            // Examine each format supported by the adapter to see if it will
            // work with this device and meets the needs of the application.
            BOOL  bFormatConfirmed[20];
            DWORD dwBehavior[20];
            D3DFORMAT fmtDepthStencil[20];

            for( f=0; f<dwNumFormats; f++ )
            {
                bFormatConfirmed[f] = FALSE;
                fmtDepthStencil[f] = D3DFMT_UNKNOWN;

                // Skip formats that cannot be used as render targets on this device
                if( FAILED( m_pD3D->CheckDeviceType( iAdapter, pDevice->DeviceType,
                                                     formats[f], formats[f], FALSE ) ) )
                    continue;

                if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                {
                    // This system has a HAL device
                    bHALExists = TRUE;

                    //if( pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED )
                    //{
                        // HAL can run in a window for some mode
                    //    bHALIsWindowedCompatible = TRUE;

                    //    if( f == 0 )
                    //    {
                            // HAL can run in a window for the current desktop mode
                            bHALIsDesktopCompatible = TRUE;
                    //    }
                    //}
                }

                // Confirm the device/format for HW vertex processing
                if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
                {
                    if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
                                        D3DCREATE_PUREDEVICE;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }
                }

                // Confirm the device/format for SW vertex processing
                if( FALSE == bFormatConfirmed[f] )
                {
                    dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

                    if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                  formats[f] ) ) )
                        bFormatConfirmed[f] = TRUE;
                }

                // Find a suitable depth/stencil buffer format for this device/format
                if( bFormatConfirmed[f] && m_bUseDepthBuffer )
                {
                    if( !FindDepthStencilFormat( iAdapter, pDevice->DeviceType,
                        formats[f], &fmtDepthStencil[f] ) )
                    {
                        bFormatConfirmed[f] = FALSE;
                    }
                }
            }

            // Add all enumerated display modes with confirmed formats to the
            // device's list of valid modes
            for( m=0L; m<dwNumModes; m++ )
            {
                for( f=0; f<dwNumFormats; f++ )
                {
                    if( modes[m].Format == formats[f] )
                    {
                        if( bFormatConfirmed[f] == TRUE )
                        {
                            // Add this mode to the device's list of valid modes
                            pDevice->modes[pDevice->dwNumModes].Width      = modes[m].Width;
                            pDevice->modes[pDevice->dwNumModes].Height     = modes[m].Height;
                            pDevice->modes[pDevice->dwNumModes].Format     = modes[m].Format;
                            pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
                            pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
                            pDevice->dwNumModes++;

                            if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                                bHALIsSampleCompatible = TRUE;
                        }
                    }
                }
            }

            // Select any 640x480 mode for default (but prefer a 16-bit mode)
            for( m=0; m<pDevice->dwNumModes; m++ )
            {
                if( pDevice->modes[m].Width==640 && pDevice->modes[m].Height==480 )
                {
                    pDevice->dwCurrentMode = m;
                    if( pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
                        pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
                        pDevice->modes[m].Format == D3DFMT_A1R5G5B5 )
                    {
                        break;
                    }
                }
            }

            // Check if the device is compatible with the desktop display mode
            // (which was added initially as formats[0])
            //if( bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) )
            if( bFormatConfirmed[0] )
			{
                pDevice->bCanDoWindowed = TRUE;
                pDevice->bWindowed      = TRUE;
            }

            // If valid modes were found, keep this device
            if( pDevice->dwNumModes > 0 )
                pAdapter->dwNumDevices++;
        }

        // If valid devices were found, keep this adapter
        if( pAdapter->dwNumDevices > 0 )
            m_dwNumAdapters++;
    }

    // Return an error if no compatible devices were found
    if( 0L == m_dwNumAdapters )
        return D3DAPPERR_NOCOMPATIBLEDEVICES;

    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
    for( DWORD a=0; a<m_dwNumAdapters; a++ )
    {
        for( DWORD d=0; d < m_Adapters[a].dwNumDevices; d++ )
        {
            if( m_Adapters[a].devices[d].bWindowed )
            {
                m_Adapters[a].dwCurrentDevice = d;
                m_dwAdapter = a;
                m_bWindowed = TRUE;

                // Display a warning message
                if( m_Adapters[a].devices[d].DeviceType == D3DDEVTYPE_REF )
                {
                    if( !bHALExists )
                        DisplayErrorMsg( D3DAPPERR_NOHARDWAREDEVICE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsSampleCompatible )
                        DisplayErrorMsg( D3DAPPERR_HALNOTCOMPATIBLE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsWindowedCompatible )
                        DisplayErrorMsg( D3DAPPERR_NOWINDOWEDHAL, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsDesktopCompatible )
                        DisplayErrorMsg( D3DAPPERR_NODESKTOPHAL, MSGWARN_SWITCHEDTOREF );
                    else // HAL is desktop compatible, but not sample compatible
                        DisplayErrorMsg( D3DAPPERR_NOHALTHISMODE, MSGWARN_SWITCHEDTOREF );
                }

                return S_OK;
            }
        }
    }

    return D3DAPPERR_NOWINDOWABLEDEVICES;
}




//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL CD3DApplication::FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
    D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat )
{
    if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D16;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24S8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X4S4;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D32;
                return TRUE;
            }
        }
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam )
{
    HRESULT hr;
	
	//HINSTANCE hInst;
	//int sb_size[] = {100, 200, 300};
	//int sb_size[2];

    switch( uMsg )
    {
        case WM_PAINT:
            // Handle paint messages when the app is not ready
			/***
            if( m_pd3dDevice && !m_bReady && !m_screensave )
            {
				if( m_bWindowed && !m_writebmpflag 
				&& ( (m_lbutton == 0) || (m_movebone <= 0) )
				&& ( (m_lbutton == 0) || (m_shiftbone <= 0) )
				&& ( (m_lbutton == 0) || (m_scalebone <= 0) )
				&& !m_vikflag
				){
                    m_pd3dDevice->Present( NULL, NULL, m_hWnd, NULL );
				}
            }
			***/

            break;

        case WM_GETMINMAXINFO:
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
            break;

        case WM_ENTERSIZEMOVE:
            // Halt frame movement while the app is sizing or moving
            if( m_bFrameMoving )
                DXUtil_Timer( TIMER_STOP );
            break;

        case WM_SIZE:
            // Check to see if we are losing our window...
            if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
                m_bActive = FALSE;
            else
                m_bActive = TRUE;


			SendMessage( g_hStatus, WM_SIZE, wParam, lParam );
			
			
			break;

        case WM_EXITSIZEMOVE:
            if( m_bFrameMoving )
                DXUtil_Timer( TIMER_START );

			if( g_motdlg )
				g_motdlg->AxisSetInvalidateFlag( 1 );//!!!!!!!!!!!


            if( m_bActive && m_bWindowed )
            {
                RECT rcClientOld;
                rcClientOld = m_rcWindowClient;

                // Update window properties
                GetWindowRect( m_hWnd, &m_rcWindowBounds );
                GetClientRect( m_hWnd, &m_rcWindowClient );

		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//clientエリアが０になると、サーフェスの作成がエラーになるので、小さくなりすぎないようにする。
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				int extwidth = 0;
				int extheight = 0;
				if( (m_rcWindowClient.right - m_rcWindowClient.left) < 100 ){
					extwidth = 1;
				}
				if( (m_rcWindowClient.bottom - m_rcWindowClient.top) < 200 ){
					extheight = 1;
				}
				
				if( (extwidth == 1) || (extheight == 1) ){
					WINDOWPLACEMENT place;
					place.length = sizeof( WINDOWPLACEMENT );
					::GetWindowPlacement( m_hWnd, &place );


					int diffwidth, diffheight;
					diffwidth = (m_rcWindowBounds.right - m_rcWindowBounds.left) - (m_rcWindowClient.right - m_rcWindowClient.left);
					diffheight = (m_rcWindowBounds.bottom - m_rcWindowBounds.top) - (m_rcWindowClient.bottom - m_rcWindowClient.top);

					if( extwidth == 1 ){
						place.rcNormalPosition.right = place.rcNormalPosition.left + diffwidth + 100;
					}
					if( extheight == 1 ){
						place.rcNormalPosition.bottom = place.rcNormalPosition.top + diffheight + 200;
					}

					::SetWindowPlacement( m_hWnd, &place );


					GetWindowRect( m_hWnd, &m_rcWindowBounds );
					GetClientRect( m_hWnd, &m_rcWindowClient );
				}


				::SendMessage( g_hToolBar, TB_AUTOSIZE, 0, 0 );


                if( rcClientOld.right - rcClientOld.left !=
                    m_rcWindowClient.right - m_rcWindowClient.left ||
                    rcClientOld.bottom - rcClientOld.top !=
                    m_rcWindowClient.bottom - m_rcWindowClient.top)
                {
                    // A new window size will require a new backbuffer
                    // size, so the 3D structures must be changed accordingly.
                    m_bReady = FALSE;

                    m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
                    m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

                    // Resize the 3D environment
                    if( FAILED( hr = Resize3DEnvironment() ) )
                    {
                        DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
                        return 0;
                    }

                    m_bReady = TRUE;
				}

			}

			if( g_motdlg )
				g_motdlg->AxisSetInvalidateFlag( 0 );//!!!!!!!!!!!

            break;

        case WM_SETCURSOR:
            // Turn off Windows cursor in fullscreen mode
            if( m_bActive && m_bReady && !m_bWindowed )
            {
                SetCursor( NULL );
                if( m_bShowCursorWhenFullscreen )
                    m_pd3dDevice->ShowCursor( TRUE );
                return TRUE; // prevent Windows from setting cursor to window class cursor
            }
            break;

         case WM_MOUSEMOVE:
            if( m_bActive && m_bReady && m_pd3dDevice != NULL )
            {
                POINT ptCursor;
                GetCursorPos( &ptCursor );
                ScreenToClient( m_hWnd, &ptCursor );
                m_pd3dDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0L );
            }
            break;

		 //case WM_MOUSEWHEEL:

		//	 DbgOut( "d3dapp : Wheel !!!\r\n" );
		//	 break;

       case WM_ENTERMENULOOP:
            // Pause the app when menus are displayed
            //Pause(TRUE);

		    if( g_motdlg && (m_intormenu == 0) ){
			    g_motdlg->OnStop();
				g_motdlg->VIKStopVIK();
		    }
            break;

        case WM_EXITMENULOOP:
            //Pause(FALSE);
            break;

        case WM_CONTEXTMENU:
            // No context menus allowed in fullscreen mode
            if( m_bWindowed == FALSE )
                break;

            // Handle the app's context menu (via right mouse click)
            TrackPopupMenuEx( GetSubMenu( LoadMenu( 0, MAKEINTRESOURCE(IDR_POPUP) ), 0 ),
                              TPM_VERTICAL, LOWORD(lParam), HIWORD(lParam), hWnd, NULL );
            break;

        case WM_NCHITTEST:
            // Prevent the user from selecting the menu in fullscreen mode
            if( !m_bWindowed )
                return HTCLIENT;

            break;

        case WM_POWERBROADCAST:
            switch( wParam )
            {
                #ifndef PBT_APMQUERYSUSPEND
                    #define PBT_APMQUERYSUSPEND 0x0000
                #endif
                case PBT_APMQUERYSUSPEND:
                    // At this point, the app should save any data for open
                    // network connections, files, etc., and prepare to go into
                    // a suspended mode.
                    return TRUE;

                #ifndef PBT_APMRESUMESUSPEND
                    #define PBT_APMRESUMESUSPEND 0x0007
                #endif
                case PBT_APMRESUMESUSPEND:
                    // At this point, the app should recover any data, network
                    // connections, files, etc., and resume running from when
                    // the app was suspended.
                    return TRUE;
            }
            break;

        case WM_SYSCOMMAND:
            // Prevent moving/sizing and power loss in fullscreen mode
            switch( wParam )
            {
                case SC_MOVE:
                case SC_SIZE:
                case SC_MAXIMIZE:
                case SC_KEYMENU:
                case SC_MONITORPOWER:
                    if( FALSE == m_bWindowed )
                        return 1;
                    break;
				case SC_SCREENSAVE:
					DbgOut( "D3DApp : SC_SCREENSAVE %d %d\n", m_bActive, m_bReady );
					m_screensave = 1;
					break;
            }
            break;

        case WM_COMMAND:
/***
            switch( LOWORD(wParam) )
            {
                case IDM_TOGGLESTART:
                    // Toggle frame movement
                    m_bFrameMoving = !m_bFrameMoving;
                    DXUtil_Timer( m_bFrameMoving ? TIMER_START : TIMER_STOP );
                    break;

                case IDM_SINGLESTEP:
                    // Single-step frame movement
                    if( FALSE == m_bFrameMoving )
                        DXUtil_Timer( TIMER_ADVANCE );
                    else
                        DXUtil_Timer( TIMER_STOP );
                    m_bFrameMoving = FALSE;
                    m_bSingleStep  = TRUE;
                    break;

                case IDM_CHANGEDEVICE:
                    // Prompt the user to select a new device or mode
                    if( m_bActive && m_bReady )
                    {
                        Pause(TRUE);

                        if( FAILED( hr = UserSelectNewDevice() ) )
                            return 0;

                        Pause(FALSE);
                    }
                    return 0;

                case IDM_TOGGLEFULLSCREEN:
                    // Toggle the fullscreen/window mode
                    if( m_bActive && m_bReady )
                    {
                        if( FAILED( ToggleFullscreen() ) )
                        {
                            DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
                            return 0;
                        }
                    }
                    return 0;

                case IDM_EXIT:
                    // Recieved key/menu command to exit app
                    SendMessage( hWnd, WM_CLOSE, 0, 0 );
                    return 0;

			}
***/
			break;
		/***
		//！！！　CD3DMyApplicationに移動。
        case WM_CLOSE:
            Cleanup3DEnvironment();
            DestroyMenu( GetMenu(hWnd) );
            DestroyWindow( hWnd );
            PostQuitMessage(0);
            return 0;
			***/
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Initialize3DEnvironment()
{


    HRESULT hr;

    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Prepare window for possible windowed/fullscreen change
    AdjustWindowForChange();


    // Set up the presentation parameters
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
m_d3dpp.BackBufferCount        = 1;
//m_d3dpp.BackBufferCount        = 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!手動 

	//m_d3dpp.MultiSampleType        = pDeviceInfo->MultiSampleType;
	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;

    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
m_d3dpp.EnableAutoDepthStencil = m_bUseDepthBuffer;
//m_d3dpp.EnableAutoDepthStencil = FALSE;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!手動

    m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
    m_d3dpp.hDeviceWindow          = m_hWnd;


	//!!!!!!!!!!!!!!!!!!!!!!!
	// バックバッファをロックするために必要なフラグ。
	//!!!!!!!!!!!!!!!!!!!!!!!
	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;


    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
    }


m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


    // Create the device



// multithread

    hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
                               m_hWndFocus, pModeInfo->dwBehavior | D3DCREATE_MULTITHREADED, &m_d3dpp,
                               &m_pd3dDevice );    
	if( SUCCEEDED(hr) )
    {
        // When moving from fullscreen to windowed mode, it is important to
        // adjust the window size after recreating the device rather than
        // beforehand to ensure that you get the window size you want.  For
        // example, when switching from 640x480 fullscreen to windowed with
        // a 1000x600 window on a 1024x768 desktop, it is impossible to set
        // the window size to 1000x600 until after the display mode has
        // changed to 1024x768, because windows cannot be larger than the
        // desktop.
        if( m_bWindowed )
        {
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                          m_rcWindowBounds.left, m_rcWindowBounds.top,
                          ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                          ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                          SWP_SHOWWINDOW );
      }

        // Store device Caps
        m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );

		if( m_d3dCaps.MaxStreams < 2 ){
			::MessageBox( m_hWnd, "ビデオカードがマルチストリームに対応していないため、\nこのソフトは実行できません。", "終了します", MB_OK );
			return -1;
		}


		if( (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0)) && (m_d3dCaps.MaxVertexShaderConst >= 256)
			&& (m_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0)) && (m_d3dCaps.MaxStreams >= 3) 
			&& (m_d3dCaps.MaxStreamStride >= ( sizeof( SKINVERTEX ) + sizeof( SKINMATERIAL ) + sizeof( SKINTANGENT ) ) ) ){

			g_bumpEnable = 1;
		}else{
			g_bumpEnable = 0;
		}
		DbgOut( "d3dapp : check Bump Enable %d : VS %d, PS %d, streams %d, stride %d\r\n",
			g_bumpEnable, m_d3dCaps.VertexShaderVersion, m_d3dCaps.PixelShaderVersion, m_d3dCaps.MaxStreams, m_d3dCaps.MaxStreamStride );


		DbgOut( "d3dapp : check texture caps : MaxTextureBlendStages %d, MaxSimultaneousTextures %d\r\n",
			m_d3dCaps.MaxTextureBlendStages, m_d3dCaps.MaxSimultaneousTextures );


		if( (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0)) && (m_d3dCaps.MaxVertexShaderConst >= 256) ){
			g_useGPU = 1;
			g_chkVS = 20;	
			if( m_d3dCaps.PixelShaderVersion < D3DPS_VERSION(1, 1) ){
				g_usePS = 0;
				//::MessageBox( m_hWnd, "ビデオカードにピクセルシェーダー1.1がありません。\nトゥーン表示は出来ません。", "確認", MB_OK );
			}else{
				g_usePS = 1;
			}

			MAXSKINMATRIX = 42;

		}else if( (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1)) && (m_d3dCaps.MaxVertexShaderConst >= 96) ){
			g_useGPU = 1;
			g_chkVS = 11;	
			if( m_d3dCaps.PixelShaderVersion < D3DPS_VERSION(1, 1) ){
				g_usePS = 0;
				//::MessageBox( m_hWnd, "ビデオカードにピクセルシェーダー1.1がありません。\nトゥーン表示は出来ません。", "確認", MB_OK );
			}else{
				g_usePS = 1;
			}

			MAXSKINMATRIX = 14;

		}else{
		
			::MessageBox( m_hWnd, "ビデオカードに頂点シェーダーがありません。\nハードウェアが使えないので、ソフトウェア処理します。\n描画速度は遅くなります。", "確認", MB_OK );

			m_pd3dDevice->Release();

			hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
                               m_hWndFocus, 
							   //pModeInfo->dwBehavior | D3DCREATE_MULTITHREADED, 
							   //D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
							   &m_d3dpp,
                               &m_pd3dDevice );    
			if( hr != D3D_OK ){
				DbgOut( "d3dapp : Initialize3DEnvironment : CreateDevice SoftWare error !!!\n" );
				_ASSERT( 0 );
				return hr;//!!!!!!!!!!!!!!!!!!!!!!
			}

			g_useGPU = 1;//!!!!!!!!!!!!!!!!
			g_chkVS = 0;
			g_usePS = 0;

			MAXSKINMATRIX = 42;
		}

		g_bumpUse = g_bumpEnable;//!!!!!!!!!!!

		DbgOut( "Initialize3DEnvironment : MaxStreamStride %d\r\n", m_d3dCaps.MaxStreamStride ); 


        m_dwCreateFlags = pModeInfo->dwBehavior;

        // Store device description
        if( pDeviceInfo->DeviceType == D3DDEVTYPE_REF )
            lstrcpy( m_strDeviceStats, TEXT("REF") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
            lstrcpy( m_strDeviceStats, TEXT("HAL") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_SW )
            lstrcpy( m_strDeviceStats, TEXT("SW") );

        if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
            pModeInfo->dwBehavior & D3DCREATE_PUREDEVICE )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
        {
            lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
        }

        if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
        {
            lstrcat( m_strDeviceStats, TEXT(": ") );
            lstrcat( m_strDeviceStats, (LPCTSTR)pAdapterInfo->d3dAdapterIdentifier.Description );
        }
///////////////////////////////
		/***
		//バックバッファとＺばっふぁの作成
		if( !m_pBackBuffer ){
			hr = m_pd3dDevice->CreateRenderTarget( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 
				pModeInfo->Format, m_d3dpp.MultiSampleType, TRUE, &m_pBackBuffer );
			if( hr != D3D_OK ){
				DbgOut( "d3dapp : Initialize3DEnvironment : dev CreateRenderTarget error %x\r\n", hr );
				_ASSERT( 0 );
				return hr;
			}
		}

		if( !m_pZSurf ){
			D3DFORMAT fmt;
			fmt = D3DFMT_D16;	
			hr = m_pd3dDevice->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 
				fmt, m_d3dpp.MultiSampleType, &m_pZSurf );
			if( hr != D3D_OK ){
				DbgOut( "d3dapp : Initialize3DEnvironment : dev CreateDepthStencilSurface error %x !!!\n", hr );
				_ASSERT( 0 );
				return hr;
			}
		}
		***/

        // Store render target surface desc
        LPDIRECT3DSURFACE9 pBackBuffer;
        m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
        pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
        pBackBuffer->Release();

///////!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		hr = m_pd3dDevice->GetDepthStencilSurface( &m_pZSurf );
		if( hr != D3D_OK ){
			DbgOut( "d3dapp : Initialize3DEnvironment : dev GetDepthStencilSurface error !!!\n" );
			_ASSERT( 0 );
			return hr;
		}
		

		hr = m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
		if( hr != D3D_OK ){
			DbgOut( "d3dapp : Initialize3DEnvironment : dev GetBackBuffer error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		
		
		
		// Set up the fullscreen cursor
        if( m_bShowCursorWhenFullscreen && !m_bWindowed )
        {
            HCURSOR hCursor;
#ifdef _WIN64
            hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
            hCursor = (HCURSOR)GetClassLong( m_hWnd, GCL_HCURSOR );
#endif

            D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor );
            m_pd3dDevice->ShowCursor( TRUE );
        }

        // Initialize the app's device-dependent objects
        hr = InitDeviceObjects();
        if( SUCCEEDED(hr) )
        {
            hr = RestoreDeviceObjects( 1 );
            if( SUCCEEDED(hr) )
            {
                m_bActive = TRUE;
                return S_OK;
            }
        }

        // Cleanup before we try again
        InvalidateDeviceObjects();
        DeleteDeviceObjects();
        SAFE_RELEASE( m_pd3dDevice );
    }

    // If that failed, fall back to the reference rasterizer
    if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
    {

		::MessageBox( m_hWnd, "ビデオカードにハードウェアアクセラレータがないため、\nこのソフトは実行できません。", "終了します", MB_OK );
		return -1;

		/***
        // Let the user know we are switching from HAL to the reference rasterizer
        DisplayErrorMsg( hr, MSGWARN_SWITCHEDTOREF );

        // Select the default adapter
        m_dwAdapter = 0L;
        pAdapterInfo = &m_Adapters[m_dwAdapter];

        // Look for a software device
        for( UINT i=0L; i<pAdapterInfo->dwNumDevices; i++ )
        {
            if( pAdapterInfo->devices[i].DeviceType == D3DDEVTYPE_REF )
            {
                pAdapterInfo->dwCurrentDevice = i;
                pDeviceInfo = &pAdapterInfo->devices[i];
                m_bWindowed = pDeviceInfo->bWindowed;
                break;
            }
        }

        // Try again, this time with the reference rasterizer
        if( pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice].DeviceType ==
            D3DDEVTYPE_REF )
        {
            hr = Initialize3DEnvironment();
        }
		***/
    }

    return hr;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Resize3DEnvironment()
{
	m_resizeflag = 1;

	DbgOut( "d3dapp : Resize3DEnvironment\r\n" );
	
    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];


	HRESULT hr;

    // Release all vidmem objects
	int resetflag = 1;
    if( FAILED( hr = InvalidateDeviceObjects( resetflag ) ) )
        return hr;


/// 作り直す。
	if( m_pZSurf ){
		m_pZSurf->Release();
		m_pZSurf = 0;
	}
	if( m_pBackBuffer ){
		m_pBackBuffer->Release();
		m_pBackBuffer = 0;
	}

//	if( g_pEffect ){
		//g_pEffect->Release();
		//g_pEffect = 0;
//	}


/***
	if( m_pd3dDevice != NULL ){
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}

	if( g_chkVS ){
		hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
								   m_hWndFocus, pModeInfo->dwBehavior | D3DCREATE_MULTITHREADED, &m_d3dpp,
								   &m_pd3dDevice );    
	}else{
		hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
                               m_hWndFocus, 
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
							   &m_d3dpp,
                               &m_pd3dDevice );    
	}

	if( hr != D3D_OK ){
		DbgOut( "d3dapp : Resize3DEnvironment : d3d CreateDevice error !!!\r\n" );
		_ASSERT( 0 );
		return hr;
	}
***/

    // Reset the device
	if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) ){
		DbgOut( "d3dapp : Resize3DEnvironment : dev Reset error %x, (%x %x %x)\r\n", hr, D3DERR_INVALIDCALL, D3DERR_OUTOFVIDEOMEMORY, E_OUTOFMEMORY );
		_ASSERT( 0 );
        return hr;
	}


	hr = m_pd3dDevice->GetDepthStencilSurface( &m_pZSurf );
	if( hr != D3D_OK ){
		DbgOut( "d3dapp : Initialize3DEnvironment : dev GetDepthStencilSurface error !!!\n" );
		_ASSERT( 0 );
		return hr;
	}
		

	hr = m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	if( hr != D3D_OK ){
		DbgOut( "d3dapp : Initialize3DEnvironment : dev GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	/***
    // Reset the device
	if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) ){
		DbgOut( "d3dapp : Resize3DEnvironment : dev Reset error %x, (%x %x %x)\r\n", hr, D3DERR_INVALIDCALL, D3DERR_OUTOFVIDEOMEMORY, E_OUTOFMEMORY );
		_ASSERT( 0 );
        return hr;
	}
	***/


	/***
	//バックバッファーを作り直す。
	if( !m_pBackBuffer ){
		hr = m_pd3dDevice->CreateRenderTarget( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 
			pModeInfo->Format, m_d3dpp.MultiSampleType, TRUE, &m_pBackBuffer );
		if( hr != D3D_OK ){
			DbgOut( "d3dapp : Resize3DEnvironment : dev CreateRenderTarget error %x\r\n", hr );
			_ASSERT( 0 );
			return hr;
		}
	}

	//Ｚバッファーを作り直す
	if( !m_pZSurf ){
	D3DFORMAT fmt;
		fmt = D3DFMT_D16;	
		hr = m_pd3dDevice->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 
			fmt, m_d3dpp.MultiSampleType, &m_pZSurf );
		if( hr != D3D_OK ){
			DbgOut( "d3dapp : Resize3DEnvironment : dev CreateDepthStencilSurface error %x !!!\n", hr );
			_ASSERT( 0 );
			return hr;
		}
	}
	***/


	/***
    // Store render target surface desc
    LPDIRECT3DSURFACE9 pBackBuffer;
    m_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
    pBackBuffer->Release();
	***/


    // Set up the fullscreen cursor
    if( m_bShowCursorWhenFullscreen && !m_bWindowed )
    {
        HCURSOR hCursor;
#ifdef _WIN64
        hCursor = (HCURSOR)(GetClassLongPtr( m_hWnd, GCLP_HCURSOR ));
#else
        hCursor = (HCURSOR)(GetClassLong( m_hWnd, GCL_HCURSOR ));
#endif
        D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor );
        m_pd3dDevice->ShowCursor( TRUE );
    }

    // Initialize the app's device-dependent objects
    hr = RestoreDeviceObjects( 0, resetflag );
	if( FAILED(hr) ){
		_ASSERT( 0 );
        return hr;
	}

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

	m_resizeflag = 0;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ToggleFullscreen()
{
    // Get access to current adapter, device, and mode
    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Need device change if going windowed and the current device
    // can only be fullscreen
    if( !m_bWindowed && !pDeviceInfo->bCanDoWindowed )
        return ForceWindowed();

    m_bReady = FALSE;

    // Toggle the windowed state
    m_bWindowed = !m_bWindowed;
    pDeviceInfo->bWindowed = m_bWindowed;

    // Prepare window for windowed/fullscreen change
    AdjustWindowForChange();

    // Set up the presentation parameters
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
    
	//m_d3dpp.MultiSampleType        = pDeviceInfo->MultiSampleType;
	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
    
	m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
    m_d3dpp.hDeviceWindow          = m_hWnd;
    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
    }

    // Resize the 3D device
    if( FAILED( Resize3DEnvironment() ) )
    {
        if( m_bWindowed )
            return ForceWindowed();
        else
            return E_FAIL;
    }

    // When moving from fullscreen to windowed mode, it is important to
    // adjust the window size after resetting the device rather than
    // beforehand to ensure that you get the window size you want.  For
    // example, when switching from 640x480 fullscreen to windowed with
    // a 1000x600 window on a 1024x768 desktop, it is impossible to set
    // the window size to 1000x600 until after the display mode has
    // changed to 1024x768, because windows cannot be larger than the
    // desktop.
    if( m_bWindowed )
    {
        SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                      m_rcWindowBounds.left, m_rcWindowBounds.top,
                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                      SWP_SHOWWINDOW );
    }

    m_bReady = TRUE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ForceWindowed()
{
    HRESULT hr;
    D3DAdapterInfo* pAdapterInfoCur = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfoCur  = &pAdapterInfoCur->devices[pAdapterInfoCur->dwCurrentDevice];
    BOOL bFoundDevice = FALSE;

    if( pDeviceInfoCur->bCanDoWindowed )
    {
        bFoundDevice = TRUE;
    }
    else
    {
        // Look for a windowable device on any adapter
        D3DAdapterInfo* pAdapterInfo;
        DWORD dwAdapter;
        D3DDeviceInfo* pDeviceInfo;
        DWORD dwDevice;
        for( dwAdapter = 0; dwAdapter < m_dwNumAdapters; dwAdapter++ )
        {
            pAdapterInfo = &m_Adapters[dwAdapter];
            for( dwDevice = 0; dwDevice < pAdapterInfo->dwNumDevices; dwDevice++ )
            {
                pDeviceInfo = &pAdapterInfo->devices[dwDevice];
                if( pDeviceInfo->bCanDoWindowed )
                {
                    m_dwAdapter = dwAdapter;
                    pDeviceInfoCur = pDeviceInfo;
                    pAdapterInfo->dwCurrentDevice = dwDevice;
                    bFoundDevice = TRUE;
                    break;
                }
            }
            if( bFoundDevice )
                break;
        }
    }

    if( !bFoundDevice )
        return E_FAIL;

    pDeviceInfoCur->bWindowed = TRUE;
    m_bWindowed = TRUE;

    // Now destroy the current 3D device objects, then reinitialize

    m_bReady = FALSE;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects();
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Create the new device
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    m_bReady = TRUE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Prepare the window for a possible change between windowed mode and
//       fullscreen mode.  This function is virtual and thus can be overridden
//       to provide different behavior, such as switching to an entirely
//       different window for fullscreen mode (as in the MFC sample apps).
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        // Set windowed-mode style
        SetWindowLong( m_hWnd, GWLP_STYLE, m_dwWindowStyle );
    }
    else
    {
        // Set fullscreen-mode style
        SetWindowLong( m_hWnd, GWLP_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::UserSelectNewDevice()
{
    HRESULT hr;

    // Can't display dialogs in fullscreen mode
    if( m_bWindowed == FALSE )
    {
        if( FAILED( ToggleFullscreen() ) )
        {
            DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
            return E_FAIL;
        }
    }

    // Prompt the user to change the mode
    if( IDOK != DialogBoxParam( (HINSTANCE)GetModuleHandle(NULL),
                                MAKEINTRESOURCE(IDD_SELECTDEVICE), m_hWnd,
                                SelectDeviceProc, (LPARAM)this ) )
        return S_OK;

    // Get access to the newly selected adapter, device, and mode
    DWORD dwDevice;
    dwDevice  = m_Adapters[m_dwAdapter].dwCurrentDevice;
    m_bWindowed = m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects();
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Inform the display class of the change. It will internally
    // re-create valid surfaces, a d3ddevice, etc.
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SelectDeviceProc()
// Desc: Windows message handling function for the device select dialog
//-----------------------------------------------------------------------------
INT_PTR CALLBACK CD3DApplication::SelectDeviceProc( HWND hDlg, UINT msg,
                                                    WPARAM wParam, LPARAM lParam )
{
    // Get access to the UI controls
    HWND hwndAdapterList        = GetDlgItem( hDlg, IDC_ADAPTER_COMBO );
    HWND hwndDeviceList         = GetDlgItem( hDlg, IDC_DEVICE_COMBO );
    HWND hwndFullscreenModeList = GetDlgItem( hDlg, IDC_FULLSCREENMODES_COMBO );
    HWND hwndWindowedRadio      = GetDlgItem( hDlg, IDC_WINDOW );
    HWND hwndFullscreenRadio    = GetDlgItem( hDlg, IDC_FULLSCREEN );
    HWND hwndMultiSampleList    = GetDlgItem( hDlg, IDC_MULTISAMPLE_COMBO );
    BOOL bUpdateDlgControls     = FALSE;

    // Static state for adapter/device/mode selection
    static CD3DApplication* pd3dApp;
    static DWORD  dwOldAdapter, dwNewAdapter;
    static DWORD  dwOldDevice,  dwNewDevice;
    static DWORD  dwOldMode,    dwNewMode;
    static BOOL   bOldWindowed, bNewWindowed;
    static D3DMULTISAMPLE_TYPE OldMultiSampleType, NewMultiSampleType;

    // Working variables
    D3DAdapterInfo* pAdapter;
    D3DDeviceInfo*  pDevice;

    // Handle the initialization message
    if( WM_INITDIALOG == msg )
    {
        // Old state
        pd3dApp      = (CD3DApplication*)lParam;
        dwOldAdapter = pd3dApp->m_dwAdapter;
        pAdapter     = &pd3dApp->m_Adapters[dwOldAdapter];

        dwOldDevice  = pAdapter->dwCurrentDevice;
        pDevice      = &pAdapter->devices[dwOldDevice];

        dwOldMode    = pDevice->dwCurrentMode;
        bOldWindowed = pDevice->bWindowed;
        OldMultiSampleType = pDevice->MultiSampleType;

        // New state is initially the same as the old state
        dwNewAdapter = dwOldAdapter;
        dwNewDevice  = dwOldDevice;
        dwNewMode    = dwOldMode;
        bNewWindowed = bOldWindowed;
        NewMultiSampleType = OldMultiSampleType;

        // Set flag to update dialog controls below
        bUpdateDlgControls = TRUE;
    }

    if( WM_COMMAND == msg )
    {
        // Get current UI state
        bNewWindowed  = Button_GetCheck( hwndWindowedRadio );

        if( IDOK == LOWORD(wParam) )
        {
            // Handle the case when the user hits the OK button. Check if any
            // of the options were changed
            if( dwNewAdapter != dwOldAdapter || dwNewDevice  != dwOldDevice  ||
                dwNewMode    != dwOldMode    || bNewWindowed != bOldWindowed ||
                NewMultiSampleType != OldMultiSampleType )
            {
                pd3dApp->m_dwAdapter = dwNewAdapter;

                pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
                pAdapter->dwCurrentDevice = dwNewDevice;

                pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
                pAdapter->devices[dwNewDevice].bWindowed     = bNewWindowed;
                pAdapter->devices[dwNewDevice].MultiSampleType = NewMultiSampleType;

                EndDialog( hDlg, IDOK );
            }
            else
                EndDialog( hDlg, IDCANCEL );

            return TRUE;
        }
        else if( IDCANCEL == LOWORD(wParam) )
        {
            // Handle the case when the user hits the Cancel button
            EndDialog( hDlg, IDCANCEL );
            return TRUE;
        }
        else if( CBN_SELENDOK == HIWORD(wParam) )
        {
            if( LOWORD(wParam) == IDC_ADAPTER_COMBO )
            {
                dwNewAdapter = ComboBox_GetCurSel( hwndAdapterList );
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = pAdapter->dwCurrentDevice;
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_DEVICE_COMBO )
            {
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = ComboBox_GetCurSel( hwndDeviceList );
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_FULLSCREENMODES_COMBO )
            {
                dwNewMode = ComboBox_GetCurSel( hwndFullscreenModeList );
            }
            else if( LOWORD(wParam) == IDC_MULTISAMPLE_COMBO )
            {
                DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
                NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
            }
        }
        // Keep the UI current
        bUpdateDlgControls = TRUE;
    }

    // Update the dialog controls
    if( bUpdateDlgControls )
    {
        // Reset the content in each of the combo boxes
        ComboBox_ResetContent( hwndAdapterList );
        ComboBox_ResetContent( hwndDeviceList );
        ComboBox_ResetContent( hwndFullscreenModeList );
        ComboBox_ResetContent( hwndMultiSampleList );

        pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
        pDevice  = &pAdapter->devices[dwNewDevice];

        // Add a list of adapters to the adapter combo box
        for( DWORD a=0; a < pd3dApp->m_dwNumAdapters; a++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndAdapterList,
                             pd3dApp->m_Adapters[a].d3dAdapterIdentifier.Description );

            // Set the item data to identify this adapter
            ComboBox_SetItemData( hwndAdapterList, dwItem, a );

            // Set the combobox selection on the current adapater
            if( a == dwNewAdapter )
                ComboBox_SetCurSel( hwndAdapterList, dwItem );
        }

        // Add a list of devices to the device combo box
        for( DWORD d=0; d < pAdapter->dwNumDevices; d++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndDeviceList,
                                               pAdapter->devices[d].strDesc );

            // Set the item data to identify this device
            ComboBox_SetItemData( hwndDeviceList, dwItem, d );

            // Set the combobox selection on the current device
            if( d == dwNewDevice )
                ComboBox_SetCurSel( hwndDeviceList, dwItem );
        }

        // Add a list of modes to the mode combo box
		DWORD m;
        for( m=0; m < pDevice->dwNumModes; m++ )
        {
            DWORD BitDepth = 16;
            if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_R8G8B8 )
            {
                BitDepth = 32;
            }

            // Add mode desc to the combo box
            TCHAR strMode[80];
            _stprintf_s( strMode, 80, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
                                                       pDevice->modes[m].Height,
                                                       BitDepth );
            DWORD dwItem = ComboBox_AddString( hwndFullscreenModeList, strMode );

            // Set the item data to identify this mode
            ComboBox_SetItemData( hwndFullscreenModeList, dwItem, m );

            // Set the combobox selection on the current mode
            if( m == dwNewMode )
                ComboBox_SetCurSel( hwndFullscreenModeList, dwItem );
        }

        // Add a list of multisample modes to the multisample combo box
        for( m=0; m <= 16; m++ )
        {
            TCHAR strDesc[50];

            D3DFORMAT fmt;
            if( bNewWindowed )
                fmt = pd3dApp->m_Adapters[dwNewAdapter].d3ddmDesktop.Format;
            else
                fmt = pDevice->modes[dwNewMode].Format;

            if ( m == 1 ) // 1 is not a valid multisample type
                continue;

			DWORD multisampleQ = 0;
            if( SUCCEEDED( pd3dApp->m_pD3D->CheckDeviceMultiSampleType( dwNewAdapter,
                pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m, &multisampleQ ) ) )
            {
                if( (m == 0) || (multisampleQ == 0) )
                    lstrcpy( strDesc, _T("none") );
                else
                    wsprintf( strDesc, _T("%d samples"), m );

                // Add device name to the combo box
                DWORD dwItem = ComboBox_AddString( hwndMultiSampleList, strDesc );

                // Set the item data to identify this multisample type
                ComboBox_SetItemData( hwndMultiSampleList, dwItem, m );

                // Set the combobox selection on the current multisample type
                if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleType || m == 0 )
                    ComboBox_SetCurSel( hwndMultiSampleList, dwItem );
            }
        }
        DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
        NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
        EnableWindow( hwndMultiSampleList, ComboBox_GetCount( hwndMultiSampleList ) > 1);
        EnableWindow( hwndWindowedRadio, pDevice->bCanDoWindowed );

        if( bNewWindowed )
        {
            Button_SetCheck( hwndWindowedRadio,   TRUE );
            Button_SetCheck( hwndFullscreenRadio, FALSE );
            EnableWindow( hwndFullscreenModeList, FALSE );
        }
        else
        {
            Button_SetCheck( hwndWindowedRadio,   FALSE );
            Button_SetCheck( hwndFullscreenRadio, TRUE );
            EnableWindow( hwndFullscreenModeList, TRUE );
        }
        return TRUE;
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: Run()
// Desc:
//-----------------------------------------------------------------------------
INT CD3DApplication::Run()
{
    // Load keyboard accelerators
    HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_ACCELERATOR1) );


    // Now we're ready to recieve and process Windows messages.
    BOOL bGotMsg;
    MSG  msg;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

    while( WM_QUIT != msg.message  )
    {
        // Use PeekMessage() if the app is active, so we can use idle time to
        // render the scene. Else, use GetMessage() to avoid eating CPU time.
        if( m_bActive )
            bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
        else
            bGotMsg = GetMessage( &msg, NULL, 0U, 0U );

        if( bGotMsg )
        {
            // Translate and dispatch the message
            if( 0 == TranslateAccelerator( m_hWnd, hAccel, &msg ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        {
            // Render a frame during idle time (no messages are waiting)
            if( m_bActive && m_bReady && (m_writebmpflag == 0) 
				&& ( (m_lbutton == 0) || (m_movebone <= 0) )
				&& ( (m_lbutton == 0) || (m_shiftbone <= 0) ) 
				&& ( (m_lbutton == 0) || (m_scalebone <= 0) ) 
				&& !m_vikflag 
				&& (m_preview_flag == 0)
				&& (m_onselectplugin == 0) )
            {
                if( FAILED( Render3DEnvironment() ) )
                    SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
				//UpdateWindow( m_hWnd );

            }
        }
    }

    return (INT)msg.wParam;
}




//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Render3DEnvironment( int skipflag )
{
	static int s_callflag = 0;

	if( s_callflag == 1 )
		return 0;

	s_callflag = 1;


    HRESULT hr;

    // Test the cooperative level to see if it's okay to render
    if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
    {
        // If the device was lost, do not render until we get it back
		if( D3DERR_DEVICELOST == hr ){
			s_callflag = 0;
            return S_OK;
		}

        // Check if the device needs to be resized.
        if( D3DERR_DEVICENOTRESET == hr )
        {
            // If we are windowed, read the desktop mode and use the same format for
            // the back buffer
            if( m_bWindowed )
            {
                D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
                m_pD3D->GetAdapterDisplayMode( m_dwAdapter, &pAdapterInfo->d3ddmDesktop );
                m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
            }

			if( FAILED( hr = Resize3DEnvironment() ) ){
				s_callflag = 0;
                return hr;
			}
        }
		s_callflag = 0;
        return hr;
    }

    // Get the app's time, in seconds. Skip rendering if no time elapsed

	float fAppTime        = DXUtil_Timer( TIMER_GETAPPTIME );
    float fElapsedAppTime = DXUtil_Timer( TIMER_GETELAPSEDTIME );
	//if( ( 0.0f == fElapsedAppTime ) && m_bFrameMoving ){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	s_callflag = 0;
    //    return S_OK;
	//}

    // FrameMove (animate) the scene
    if( m_bFrameMoving || m_bSingleStep )
    {
        // Store the time for the app
        m_fTime        = fAppTime;
        m_fElapsedTime = fElapsedAppTime;

        // Frame move the scene
		if( skipflag == 0 ){
			if( FAILED( hr = FrameMove() ) ){
				s_callflag = 0;
				return hr;
			}
		}

        m_bSingleStep = FALSE;
    }


//LARGE_INTEGER beftime, afttime;
//DWORD render_time_low, render_time_high;
//QueryPerformanceCounter( &beftime );


    // Render the scene as normal
	if( FAILED( hr = Render() ) ){
		s_callflag = 0;
        return hr;
	}

    // Keep track of the frame count
    {
        static float fLastTime = 0.0f;
        static DWORD dwFrames  = 0L;
        float fTime = DXUtil_Timer( TIMER_GETABSOLUTETIME );
        ++dwFrames;

        // Update the scene stats once per second
        if( fTime - fLastTime > 1.0f )
        {
            //m_fFPS    = dwFrames / (fTime - fLastTime);//viewer.cppでセット。
            fLastTime = fTime;
            dwFrames  = 0L;

            // Get adapter's current mode so we can report
            // bit depth (back buffer depth may be unknown)
            D3DDISPLAYMODE mode;
            m_pD3D->GetAdapterDisplayMode(m_dwAdapter, &mode);

            _stprintf_s( m_strFrameStats, 40, _T("%.02f fps (%dx%dx%d)"), m_fFPS,
                       m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
                       mode.Format==D3DFMT_X8R8G8B8?32:16 );
            if( m_bUseDepthBuffer )
            {
                D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
                D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
                D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

                switch( pModeInfo->DepthStencilFormat )
                {
                case D3DFMT_D16:
                    lstrcat( m_strFrameStats, _T(" (D16)") );
                    break;
                case D3DFMT_D15S1:
                    lstrcat( m_strFrameStats, _T(" (D15S1)") );
                    break;
                case D3DFMT_D24X8:
                    lstrcat( m_strFrameStats, _T(" (D24X8)") );
                    break;
                case D3DFMT_D24S8:
                    lstrcat( m_strFrameStats, _T(" (D24S8)") );
                    break;
                case D3DFMT_D24X4S4:
                    lstrcat( m_strFrameStats, _T(" (D24X4S4)") );
                    break;
                case D3DFMT_D32:
                    lstrcat( m_strFrameStats, _T(" (D32)") );
                    break;
                }
            }
        }
    }

    // Show the frame on the primary surface.
	//if( m_writebmpflag == 0 )
	//	m_pd3dDevice->Present( NULL, NULL, m_hWnd, NULL );


//QueryPerformanceCounter( &afttime );
//render_time_low = afttime.LowPart - beftime.LowPart;
//render_time_high = afttime.HighPart - beftime.HighPart;

//DbgOut( "Render3DEnv : present : high %d, low %d\n", render_time_high, render_time_low );

	s_callflag = 0;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CD3DApplication::Pause( BOOL bPause )
{
    static DWORD dwAppPausedCount = 0L;

    dwAppPausedCount += ( bPause ? +1 : -1 );
    m_bReady          = ( dwAppPausedCount ? FALSE : TRUE );

    // Handle the first pause request (of many, nestable pause requests)
    if( bPause && ( 1 == dwAppPausedCount ) )
    {
        // Stop the scene from animating
        if( m_bFrameMoving )
            DXUtil_Timer( TIMER_STOP );
    }

    if( 0 == dwAppPausedCount )
    {
        // Restart the timers
        if( m_bFrameMoving )
            DXUtil_Timer( TIMER_START );
    }
}




//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID CD3DApplication::Cleanup3DEnvironment()
{
    m_bActive = FALSE;
    m_bReady  = FALSE;

    InvalidateDeviceObjects();
    DeleteDeviceObjects();


	if( m_pZSurf ){
		m_pZSurf->Release();
		m_pZSurf = 0;
	}
	if( m_pBackBuffer ){
		m_pBackBuffer->Release();
		m_pBackBuffer = 0;
	}

	if( g_pEffect ){
		g_pEffect->Release();
		g_pEffect = 0;
	}

	if( m_pd3dDevice != NULL ){
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
	if( m_pD3D != NULL ){
		m_pD3D->Release();
		m_pD3D = NULL;
	}

    FinalCleanup();
}

/***
VOID CD3DApplication::Cleanup3DEnvironment()
{
    m_bActive = FALSE;
    m_bReady  = FALSE;

    if( m_pd3dDevice )
    {
        InvalidateDeviceObjects();
        DeleteDeviceObjects();

		if( m_d3dxsprite != NULL ){
			m_d3dxsprite->Release();
			m_d3dxsprite = NULL;
		}

        m_pd3dDevice->Release();
        m_pD3D->Release();


        m_pd3dDevice = NULL;
        m_pD3D       = NULL;
    }

    FinalCleanup();
}
***/



//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::DisplayErrorMsg( HRESULT hr, DWORD dwType, char* mesptr )
{
    TCHAR strMsg[512];

    switch( hr )
    {
        case D3DAPPERR_NODIRECT3D:
            _tcscpy_s( strMsg, 512, _T("Could not initialize Direct3D. You may\n")
                             _T("want to check that the latest version of\n")
                             _T("DirectX is correctly installed on your\n")
                             _T("system.  Also make sure that this program\n")
                             _T("was compiled with header files that match\n")
                             _T("the installed DirectX DLLs.") );
            break;

        case D3DAPPERR_NOCOMPATIBLEDEVICES:
            _tcscpy_s( strMsg, 512, _T("Could not find any compatible Direct3D\n")
                             _T("devices.") );
            break;

        case D3DAPPERR_NOWINDOWABLEDEVICES:
            _tcscpy_s( strMsg, 512, _T("This sample cannot run in a desktop\n")
                             _T("window with the current display settings.\n")
                             _T("Please change your desktop settings to a\n")
                             _T("16- or 32-bit display mode and re-run this\n")
                             _T("sample.") );
            break;

        case D3DAPPERR_NOHARDWAREDEVICE:
            _tcscpy_s( strMsg, 512, _T("No hardware-accelerated Direct3D devices\n")
                             _T("were found.") );
            break;

        case D3DAPPERR_HALNOTCOMPATIBLE:
            _tcscpy_s( strMsg, 512, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator.") );
            break;

        case D3DAPPERR_NOWINDOWEDHAL:
            _tcscpy_s( strMsg, 512, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_NODESKTOPHAL:
            _tcscpy_s( strMsg, 512, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window with the current\n")
                             _T("desktop display settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_NOHALTHISMODE:
            _tcscpy_s( strMsg, 512, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator with the current desktop display\n")
                             _T("settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_MEDIANOTFOUND:
            _tcscpy_s( strMsg, 512, _T("Could not load required media." ) );
            break;

        case D3DAPPERR_RESIZEFAILED:
            _tcscpy_s( strMsg, 512, _T("Could not reset the Direct3D device." ) );
            break;

        case D3DAPPERR_NONZEROREFCOUNT:
            _tcscpy_s( strMsg, 512, _T("A D3D object has a non-zero reference\n")
                             _T("count (meaning things were not properly\n")
                             _T("cleaned up).") );
            break;

        case E_OUTOFMEMORY:
            _tcscpy_s( strMsg, 512, _T("Not enough memory.") );
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
            _tcscpy_s( strMsg, 512, _T("Not enough video memory.") );
            break;

        default:
            _tcscpy_s( strMsg, 512, _T("Generic application error. Enable\n")
                             _T("debug output for detailed information.") );
    }

    if( MSGERR_APPMUSTEXIT == dwType )
    {
		if( mesptr ){
			_tcscat_s( strMsg, 512, _T("\n") );
			_tcscat_s( strMsg, 512, mesptr );
		}
        _tcscat_s( strMsg, 512, _T("\n\nThis sample will now exit.") );
        MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONERROR|MB_OK );

        // Close the window, which shuts down the app
        if( m_hWnd )
            SendMessage( m_hWnd, WM_CLOSE, 0, (LPARAM)dwType );
    }
    else
    {
		if( mesptr ){
			_tcscat_s( strMsg, 512, _T("\n") );
			_tcscat_s( strMsg, 512, mesptr );
		}

        if( MSGWARN_SWITCHEDTOREF == dwType )
            _tcscat_s( strMsg, 512, _T("\n\nSwitching to the reference rasterizer,\n")
                             _T("a software device that implements the entire\n")
                             _T("Direct3D feature set, but runs very slowly.") );
        MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONWARNING|MB_OK );
    }

    return hr;
}

int CD3DApplication::SetModuleDir()
{
	char filename[1024];
	ZeroMemory( filename, 1024 );
	int ret = 0;

	ret = GetModuleFileName( NULL, filename, 1024 );
	if( ret == 0 ){
		_ASSERT( 0 );
		DbgOut( "d3dapp : SetModuleDir : GetModuleFileName error !!!\n" );
		return 1;
	}
	int ch = '\\';
	char* lasten;
	lasten = strrchr( filename, ch );
	if( !lasten ){
		_ASSERT( 0 );
		DbgOut( "d3dapp : SetModuleDir : strrchr error !!!\n" );
		return 1;
	}
	int leng;
	char moduledir[1024];
	ZeroMemory( moduledir, 1024 );
	leng = (int)(lasten - filename + 1);
	strncpy_s( moduledir, 1024, filename, leng );

	::SetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPCTSTR)moduledir );
	
	DbgOut( "CMyD3DApplication : SetModuleDir : %s\n", moduledir );


	SetMediaDir();

	return 0;
}

int CD3DApplication::SetMediaDir()
{
	char filename[1024];
	char* endptr = 0;

	ZeroMemory( filename, 1024 );
	int ret;

	ret = GetModuleFileName( NULL, filename, 1024 );
	if( ret == 0 ){
		_ASSERT( 0 );
		DbgOut( "SetMediaDir : GetModuleFileName error !!!\n" );
		return 1;
	}

	int ch = '\\';
	char* lasten = 0;
	lasten = strrchr( filename, ch );
	if( !lasten ){
		_ASSERT( 0 );
		DbgOut( "SetMediaDir : strrchr error !!!\n" );
		return 1;
	}

	*lasten = 0;

	char* last2en = 0;
	char* last3en = 0;
	last2en = strrchr( filename, ch );
	if( last2en ){
		*last2en = 0;
		last3en = strrchr( filename, ch );
		if( last3en ){
			if( (strcmp( last2en + 1, "debug" ) == 0) ||
				(strcmp( last2en + 1, "Debug" ) == 0) ||
				(strcmp( last2en + 1, "DEBUG" ) == 0) ||
				(strcmp( last2en + 1, "release" ) == 0) ||
				(strcmp( last2en + 1, "Release" ) == 0) ||
				(strcmp( last2en + 1, "RELEASE" ) == 0)
				){

				endptr = last2en;
			}else{
				endptr = lasten;
			}
		}else{
			endptr = lasten;
		}
	}else{
		endptr = lasten;
	}

	*lasten = '\\';
	if( last2en )
		*last2en = '\\';
	if( last3en )
		*last3en = '\\';

	int leng;
	char resdir[1024];
	ZeroMemory( resdir, 1024 );
	leng = endptr - filename + 1;
	strncpy_s( resdir, 1024, filename, leng );
	strcat_s( resdir, 1024, "Media\\" );


	::SetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPCTSTR)resdir );

	DbgOut( "SetMediaDir : %s\n", resdir );

	return 0;
}

int CD3DApplication::CreateAppToolBar()
{
	if( g_hToolBar != NULL )
		return 0;

	HINSTANCE hInst;
	hInst = (HINSTANCE)GetModuleHandle(NULL);

	g_hToolBar = CreateWindowEx(
			0,
			TOOLBARCLASSNAME,
			NULL,
			WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | WS_CLIPCHILDREN,
			0, 0,
			0, 0,
			m_hWnd,
			(HMENU)ID_TOOLBAR,
			hInst,
			NULL);

	SendMessage( g_hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );


	char bmpname[2048];
	int leng;
	ZeroMemory( bmpname, 2048 );
	leng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)bmpname, 2048 );
	_ASSERT( leng );
	strcat_s( bmpname, 2048, "tb22.bmp" );



	int buttonnum = 22;//!!!!!!!!!!!!!!!!



	TBADDBITMAP tbab;
    tbab.hInst = NULL;
	m_hbmp = LoadImage( NULL, bmpname, IMAGE_BITMAP, 16 * buttonnum, 15, LR_LOADFROMFILE );
	if( m_hbmp == NULL ){
		DbgOut( "d3dapp : CreateAppToolBar : LoadImage error !!!\n" );
		MessageBox( m_hWnd, "Mediaフォルダにtb22.bmpが無いかもしくは不正です。\nツールバーが作れないので終了します。", "エラー", MB_OK );
		_ASSERT( 0 );
		return 1;
	}
    tbab.nID = (UINT_PTR)m_hbmp;
    SendMessage( g_hToolBar, TB_ADDBITMAP, buttonnum, (LPARAM)&tbab );
    SendMessage( g_hToolBar, TB_ADDBUTTONS, (WPARAM)buttonnum, (LPARAM)&tbb);
    

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                20, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                20, (LPARAM)&tb_space);

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                17, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                17, (LPARAM)&tb_space);

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                13, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                13, (LPARAM)&tb_space);

    //SendMessage(g_hToolBar, TB_ENABLEBUTTON, IDTBB_L, MAKELONG(0, 0));//!!!!!!!!!!!!!!!!!

	return 0;
}


/***
int CD3DApplication::CreateAppToolBar()
{
	if( g_hToolBar != NULL )
		return 0;

	HINSTANCE hInst;
    //hInst = (HINSTANCE)GetWindowLong(m_hWnd, GWLP_HINSTANCE);
	hInst = (HINSTANCE)GetModuleHandle(NULL);

	//char mes2[256];
	//sprintf( mes2, "m_hWnd %x, hInst %x, IDB_TB1 %d", m_hWnd, hInst, IDB_TB1 );
	//MessageBox( NULL, mes2, "check", MB_OK );

	g_hToolBar = CreateToolbarEx(
                m_hWnd, //親ウィンドウ
                WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | WS_CLIPCHILDREN, //ウィンドウスタイル
                ID_TOOLBAR, // コントロールID
                22, //イメージの数!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!5
                hInst,
				IDB_TB11,
                tbb,
                22, // ボタンの数!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!5
                0, //ボタンの幅
                0, //ボタンの高さ
                16, //イメージの幅
                15, //イメージの高さ
                sizeof(TBBUTTON));
	//if( g_hToolBar == NULL ){
	//	DWORD dwerror;
	//	dwerror = GetLastError();
	//	ErrorMessage( "CreateToolbarEx : dwerror", dwerror );
	//}


	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                20, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                20, (LPARAM)&tb_space);

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                17, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                17, (LPARAM)&tb_space);

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                13, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                13, (LPARAM)&tb_space);

    //SendMessage(g_hToolBar, TB_ENABLEBUTTON, IDTBB_L, MAKELONG(0, 0));//!!!!!!!!!!!!!!!!!

	return 0;
}
***/


int CD3DApplication::DestroyAppToolBar()
{

	if( g_hToolBar != NULL ){
		DestroyWindow( g_hToolBar );
		g_hToolBar = NULL;
	}

//	if( m_hbmp != NULL ){
//		DeleteObject( m_hbmp );
//	}

	return 0;
}


