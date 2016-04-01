// SigEditDlg.cpp : CSigEditDlg のインプリメンテーション
#include "stdafx.h"
#include "SigEditDlg.h"

#include "SigTreeView2.h"
#include "inidata.h"

#define DBGH
#include <dbg.h>

///////////////////
//extern HINSTANCE g_hinstance;
extern CIniData* g_inidata;

/////////////////////////////////////////////////////////////////////////////
// CSigEditDlg

CSigEditDlg::CSigEditDlg()
{
	m_ax = 0;
	m_ctrl = 0;
	m_apphwnd = 0;
	m_menu = NULL;

	m_lpsh = 0;

	m_wx = 0;
	m_wy = 0;
	m_y = 0;
	m_dy = 0;
	m_rangey = 0;
	m_yclient = 0;

	m_x = 0;
	m_dx = 0;
	m_rangex = 0;
	m_xclient = 0;


	m_largex = 1000;
	m_largey = 5000;
}

CSigEditDlg::~CSigEditDlg()
{
	if( m_menu != NULL ){
		DestroyMenu( m_menu );
		m_menu = NULL;
	}

}
LRESULT CSigEditDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DbgOut( "SigEditDlg : OnInitDialog\n" );


//	RECT rcClient;
//	GetClientRect(&rcClient);
//	m_wndView.Create(m_hWnd, rcClient, COLE2T(m_dlgOpen.m_bstrName), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);


	USES_CONVERSION;
  
	OLECHAR szClsid[40];
	ZeroMemory( szClsid, sizeof(OLECHAR) * 40 );
	StringFromGUID2(CLSID_SigTreeView2, szClsid, 40);

	AtlAxWinInit();


	m_menu = LoadMenu( (HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE( IDR_DLGMENU ) );
	if( m_menu == NULL ){
		_ASSERT( 0 );
		return 1;
	}

	BOOL bret;
	bret = ::SetMenu( m_hWnd, m_menu );
	if( bret == 0 ){
		_ASSERT( 0 );
		return 1;
	}

	MoveWindow( 0, 0, m_largex, m_largey, 1 );
	
	// Create the control's host window
	HWND rethwnd;
	RECT    rect;
	::GetClientRect( m_hWnd, &rect);		
	rethwnd = m_ax.Create(m_hWnd, rect, 0, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, WS_EX_CLIENTEDGE, ID_CHILD_CONTROL);
	if( !rethwnd ){
		DbgOut( "SigEditDlg : OnInitDialog : m_ax.Create error !!!\n" );
		ATLTRACE( "SigEditDlg : OnInitDialog : m_ax.Create error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

		
	// Create the control, setting up the IPropertyNotifySink connection point
	HRESULT hr;
	hr = m_ax.CreateControl(szClsid);
	//hr = m_ax.CreateControl(IDR_SIGTREEVIEW);
	if( FAILED( hr ) ){
		DbgOut( "SigEditDlg : OnInitDialog : m_ax.CreateControl error !!!\n" );
		ATLTRACE( "SigEditDlg : OnInitDialog : m_ax.CreateControl error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	hr = m_ax.QueryControl(&m_ctrl);
	if( FAILED( hr ) || !m_ctrl ){
		DbgOut( "SigEditDlg : OnInitDialog : m_ax.QueryControl error !!!\n" );
		ATLTRACE( "SigEditDlg : OnInitDialog : m_ax.QueryControl error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	MoveWindow( g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, 300, 500, 1 );


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSigEditDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );
	return 0;
}

LRESULT CSigEditDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );
	return 0;
}

LRESULT CSigEditDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );
	return 0;
}
int CSigEditDlg::SetAppWnd( long appwnd )
{
	m_apphwnd = (HWND)appwnd;
	return 0;
}
int CSigEditDlg::SetDbgFile( long dbgf )
{
	if( !m_ctrl )
		return 1;
	int ret;
	ret = m_ctrl->SetDbgFile( dbgf );
	_ASSERT( !ret );
	return ret;
}
int CSigEditDlg::SetParams( long lpth, long lpsh, long lpmh, long appwnd )
{
	static int isfirst = 0;


	if( !m_ctrl ){
		_ASSERT( 0 );
		return 1;
	}
	m_apphwnd = (HWND)appwnd;

	m_lpsh = lpsh;

	int ret;

	ScrollWindow( m_x, m_y, NULL, NULL );
	m_x = 0;
	m_y = 0;

	if( m_lpsh ){
		MoveWindow( g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, m_largex, m_largey, 1 );

		ret = m_ctrl->SetParams( m_largex, m_largey, lpth, lpsh, lpmh, appwnd );
		_ASSERT( !ret );

		m_ctrl->SizeChange( g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, m_largex, m_largey );
		m_ctrl->GetCtrlSize( &m_wx, &m_wy );

		//		SetWindowPos( HWND_TOP, g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, 300, 500, SWP_NOZORDER );
		MoveWindow( g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, 300, 500, 1 );

		m_yclient = 500;
		m_xclient = 300;
		m_rangey = m_wy - m_yclient;
		m_rangex = m_wx - m_xclient;
//		m_y = min(m_y, m_rangey);
//		m_x = min(m_x, m_rangex);

		SetScrollRange(SB_VERT, 0, m_rangey, FALSE);
		SetScrollPos(SB_VERT, m_y, TRUE);
		SetScrollRange(SB_HORZ, 0, m_rangex, FALSE);
		SetScrollPos(SB_HORZ, m_x, TRUE);

		char mes[512];
		sprintf_s( mes, 512, "yclient %d, rangey %d, y %d, wx %d, wy %d\nxclient %d, rangex %d, x %d",
			m_yclient, m_rangey, m_y, m_wx, m_wy,
			m_xclient, m_rangex, m_x );
//		MessageBox( mes, "チェック", MB_OK );

		//UpdateWindow();

		m_ctrl->UDWindow();

		isfirst++;
	}


	return ret;
}
int CSigEditDlg::Redraw()
{
	if( !m_ctrl )
		return 0;
	int ret;
	ret = m_ctrl->Redraw();
	_ASSERT( !ret );
	return ret;
}
int CSigEditDlg::InvalidateHandler()
{
	if( !m_ctrl )
		return 0;


	int ret;
	ret = m_ctrl->InvalidateHandler();
	_ASSERT( !ret );

	m_lpsh = 0;



	return ret;
}

int CSigEditDlg::OnUserSelchange( long selno )
{
	int ret;
	if( m_ctrl ){
		ret = m_ctrl->OnUserSelchange( selno );
		_ASSERT( !ret );
	}
	return 0;
}

LRESULT CSigEditDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	if( !m_ctrl )
		return 0;
	int ret;
	ret = m_ctrl->OnMove();
	_ASSERT( !ret );
	
	DefWindowProc();

	RECT rect;
	GetWindowRect( &rect );
	g_inidata->sigdlg_posx = rect.left;
	g_inidata->sigdlg_posy = rect.top;

	return 0;
}

LRESULT CSigEditDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_MOVING, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );
	DefWindowProc();
	return 0;
}

LRESULT CSigEditDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

/***
	RECT rect;
	GetClientRect( &rect );

	if( m_lpsh && rect.right && rect.bottom ){
		m_ctrl->SizeChange( rect.right, rect.bottom );
	}
***/
/***
	if( m_lpsh ){
		RECT rect1;
		GetClientRect( &rect1 );

		int cx, cy;
		m_ctrl->GetCtrlSize( &cx, &cy );

		char mes[512];
		sprintf_s( mes, 512, "host %d, %d, ctrl %d, %d",
			rect1.right, rect1.bottom,
			cx, cy );
//		::MessageBox( m_hWnd, mes, "チェック", MB_OK );
	}
***/
	m_yclient = HIWORD(lParam);
	m_xclient = HIWORD(lParam);
//	m_rangey = m_wy - m_yclient;
//	m_y = min(m_y, m_rangey);
//	SetScrollRange(SB_VERT, 0, m_rangey, FALSE);
//	SetScrollPos(SB_VERT, m_y, TRUE);

	DefWindowProc();

	UpdateWindow();


	return 0;
}


LRESULT CSigEditDlg::OnMenuShdParam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->ShowShdParamDlg();

	return ret;
}

LRESULT CSigEditDlg::OnMenuJointLoc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->ShowJointLocDlg();

	return ret;
}

LRESULT CSigEditDlg::OnMenuNewJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->NewJoint();

	return ret;
}
LRESULT CSigEditDlg::OnMenuInfluence(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->ShowInfluenceDlg();

	return ret;
}

LRESULT CSigEditDlg::OnMenuDelJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->DelJoint();

	return ret;
}

LRESULT CSigEditDlg::OnMenuConv2Scope(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->Conv2Scope();

	return ret;
}


int CSigEditDlg::Rename( int seri, char* srcname )
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->Rename( seri, (long)srcname );

	return ret;
}
int CSigEditDlg::GetSelectedNo( int* selnoptr )
{
	int ret = 0;
	if( m_ctrl )
		ret = m_ctrl->GetSelectedNo( selnoptr );

	return ret;
}

LRESULT CSigEditDlg::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if( m_lpsh ){

		m_rangey = m_wy - m_yclient;
		m_y = min(m_y, m_rangey);

		switch (LOWORD(wParam)) {
        case SB_LINEUP:
			m_dy = -2;
            break;
        case SB_LINEDOWN:
			m_dy = 2;
            break;
        case SB_THUMBPOSITION:
			m_dy = HIWORD(wParam) - m_y;
            break;
        case SB_PAGEDOWN:
			m_dy = 10;
            break;
        case SB_PAGEUP:
			m_dy = -10;
			break;
        default:
			m_dy = 0;
			break;
		}
        m_dy = max(-m_y, min(m_dy, m_rangey - m_y));
        if (m_dy != 0) {
			m_y += m_dy;
            ScrollWindow(0, -m_dy, NULL, NULL);
            SetScrollPos(SB_VERT, m_y, TRUE);
            UpdateWindow();
        }
	}

	return 0;
}

LRESULT CSigEditDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if( m_lpsh ){

		m_rangex = m_wx - m_xclient;
		m_x = min(m_x, m_rangex);

		switch (LOWORD(wParam)) {
        case SB_LINEUP:
			m_dx = -2;
            break;
        case SB_LINEDOWN:
			m_dx = 2;
            break;
        case SB_THUMBPOSITION:
			m_dx = HIWORD(wParam) - m_x;
            break;
        case SB_PAGEDOWN:
			m_dx = 10;
            break;
        case SB_PAGEUP:
			m_dx = -10;
			break;
        default:
			m_dx = 0;
			break;
		}
        m_dx = max(-m_x, min(m_dx, m_rangex - m_x));
        if (m_dx != 0) {
			m_x += m_dx;
            ScrollWindow(-m_dx, 0, NULL, NULL);
            SetScrollPos(SB_HORZ, m_x, TRUE);
            UpdateWindow();
        }
	}
	return 0;
}
