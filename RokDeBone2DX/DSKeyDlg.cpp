// DSKeyDlg.cpp : CDSKeyDlg のインプリメンテーション
#include "stdafx.h"
#include "DSKeyDlg.h"

#include <math.h>
#include <stdio.h>
#include <d3dx9.h>

#include <coef.h>
#define DBGH
#include <dbg.h>

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <mothandler.h>
#include <motionctrl.h>
#include <motioninfo.h>

#include <DSKey.h>
#include <DSChange.h>
#include <DispSwitch.h>


#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "viewer.h"
#include "MotDlg.h"
#include "MotParamDlg.h"

#include "DSMenuMain.h"
#include "SyncDlg.h"

#include <crtdbg.h>


//MotionPoint描画時の点の半径
#define MP_SIZE	3

/////////////////
// sttic
static LONG s_mpfunc = 0;
static LONG s_listfunc = 0;
static HWND s_sl_mp_wnd = 0;
static HWND s_dlg_wnd = 0;
static HWND s_list_wnd = 0;
static HWND s_apphwnd = 0;

/////////////////
//extern HINSTANCE g_resinstance;
extern CMotDlg* g_motdlg;
extern unsigned char g_KeyBuffer[256];
extern int g_endapp;

/////////////////////////////////////////////////////////////////////////////
// CDSKeyDlg

CDSKeyDlg::CDSKeyDlg( CMotParamDlg* srcmpdlg )
//: m_list_wnd( NULL, this, 2 )
{

	m_dlg = srcmpdlg;
	m_dlg->m_dskeydlg = this;

//	m_current = 0;

	m_black_pen = NULL;
	m_red_pen = NULL;
	m_blue_pen = NULL;
	m_black_brush = NULL;
	m_white_brush = NULL;
	m_red_brush = NULL;
	m_blue_brush = NULL;

	//m_displayflag = 0;
	m_ismoving = 0;

	m_dragdsk = 0;
	m_dragframe = 0;
	m_dragflag = 0;


	m_timerworking = 0;

	m_cmdshow = SW_HIDE;
	m_createlistflag = 0;
	m_mcap = 0;
	m_moving = 0;

	m_remakelistflag = 0;

	m_cpdsk = 0;

	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

}


CDSKeyDlg::~CDSKeyDlg()
{
	if( m_cpdsk ){
		delete m_cpdsk;
		m_cpdsk = 0;
	}
}

int CDSKeyDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, DSKeyDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	return 0;
}

LRESULT CDSKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	ret = SetWnd();
	_ASSERT( !ret );
	s_sl_mp_wnd = m_sl_mp_wnd;

	ret = SetNewWindowProc();
	//_ASSERT( !ret );


	ret = ParamsToDlg();
	_ASSERT( !ret );

	int timerid;
	timerid = (int)::SetTimer( m_hWnd, DSKeyDlgTIMER, 500, NULL );
	_ASSERT( timerid == DSKeyDlgTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}


int CDSKeyDlg::InitDlg()
{
	int ret;
	
	//m_current = m_startframe;

	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CDSKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "DSKeyDlg : OnOK\n" );
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}

LRESULT CDSKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}
LRESULT CDSKeyDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}


int CDSKeyDlg::SetWnd()
{

	HWND temphwnd;

	m_dlg_wnd = m_hWnd;
	s_dlg_wnd = m_hWnd;

	temphwnd = GetDlgItem( IDC_LIST1 );
	m_list_wnd = temphwnd;
	s_list_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEX_CURRENT );
	m_tex_current_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_MP );
	_ASSERT( temphwnd );
	m_mp_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SL_MP );
	m_sl_mp_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SP_MP );
	m_sp_mp_wnd = temphwnd;


	temphwnd = GetDlgItem( IDC_FRAMECOPY );
	m_framecopy_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_FRAMEPASTE );
	m_framepaste_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_FRAMEDEL );
	m_framedel_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_PREVIEW );
	m_preview_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_STOP );
	m_stop_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_ALLDELETE );
	m_alldelete_wnd = temphwnd;



	temphwnd = GetDlgItem( IDC_SELLOCK_NAME );
	m_sellock_name_wnd = temphwnd;

	//sliderの初期設定
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//


	return 0;
}

int CDSKeyDlg::ParamsToDlg()
{
	if( g_endapp == 1 ){
		return 0;
	}

	char tempchar[1024];		

	if( !IsWindow() )
		return 0;

	if( m_timerworking != 1 ){
		return 0;
	}
	if( m_cmdshow != SW_SHOW ){
		return 0;
	}

//m_current
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "frameno: current %d, start %d, end %d, total %d", 
		m_dlg->m_current, m_dlg->m_startframe, m_dlg->m_endframe, m_dlg->m_sl_mp_rangemax + 1 );
	if( (g_endapp == 1) || !m_tex_current_wnd.IsWindow() )
		return 0;
	m_tex_current_wnd.SetWindowText( tempchar );

	if( m_cmdshow != SW_SHOW ){
		return 0;
	}

//name
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_dlg->m_thandler ){
		CTreeElem2* curtelem = (*m_dlg->m_thandler)( m_dlg->m_selectedno );
		_ASSERT( curtelem );
		if( (g_endapp == 1) || !m_sellock_name_wnd.IsWindow() )
			return 0;
		m_sellock_name_wnd.SetWindowText( curtelem->name );
	}else{
		if( (g_endapp == 1) || !m_sellock_name_wnd.IsWindow() )
			return 0;
		m_sellock_name_wnd.SetWindowText( "" );		
	}


//m_sl_mp_
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_dlg->m_startframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_dlg->m_endframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_dlg->m_current );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	SetSliderTic( m_sl_mp_wnd, 10, m_dlg->m_startframe, m_dlg->m_endframe );//

	return 0;
}

LRESULT CDSKeyDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}

LRESULT CDSKeyDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int curpos;
	curpos = (int)m_sl_mp_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	m_dlg->m_current = curpos;

	m_dlg->m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_dlg->m_current );
	BOOL dummy;	
	m_dlg->OnReleasedcaptureSl_mp( 0, 0, dummy );

	InitList();
	ParamsToDlg();
	Redraw();

	if( m_dlg->m_d3dapp->m_syncdlg->m_quaflag != 0 ){
		int camflag = m_dlg->m_d3dapp->m_syncdlg->m_camflag;
		int gpaflag = m_dlg->m_d3dapp->m_syncdlg->m_gpaflag;
		int moeflag = m_dlg->m_d3dapp->m_syncdlg->m_moeflag;
		int sndflag = m_dlg->m_d3dapp->m_syncdlg->m_sndflag;

		g_motdlg->OnJumpFrame( m_dlg->m_current, 0, camflag, gpaflag, moeflag, sndflag );
	}

	return 0;
}
LRESULT CDSKeyDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;


	int delta;
	if( idCtrl == 999 ){
		delta = pnmud->iDelta;
	}else{
		// defaultで、左ボタンがプラスになっているので、反転する。
		delta = -(pnmud->iDelta);
	}

	// スピンコントロールのiPosがクランプしないように、初期状態に戻す。
	m_sp_mp_wnd.SendMessage( UDM_SETPOS, 0, 0 );

	if( delta == 10000 ){
		//最終フレーム
		m_dlg->m_current = m_dlg->m_endframe;
	}else if( delta == -10000 ){
		//先頭フレーム
		m_dlg->m_current = m_dlg->m_startframe;
	}else if( delta > 0 ){
		int newframe;
		newframe = m_dlg->m_current + 1;
		if( newframe <= m_dlg->m_endframe ){
			m_dlg->m_current = newframe;							
		}
	}else if( delta < 0 ){
		int newframe;
		newframe = m_dlg->m_current - 1;
		if( newframe >= m_dlg->m_startframe ){
			m_dlg->m_current = newframe;
		}
	}

	m_dlg->m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_dlg->m_current );
	BOOL dummy;	
	m_dlg->OnReleasedcaptureSl_mp( 0, 0, dummy );

	InitList();
	ParamsToDlg();
	Redraw();

	return 0;

}
void CDSKeyDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	if( g_endapp == 1 ){
		return;
	}

	dstwnd.SendMessage( TBM_CLEARTICS, 0, 0 );

	// !!! minticとmaxticには、自動的にticが入るので設定はしない。(してはいけない)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		if( g_endapp == 1 ){
			return;
		}
		dstwnd.SendMessage( TBM_SETTIC, 1, (LPARAM)(LONG)curtic );
		curtic += ticfreq;
	}
}


int CDSKeyDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_dlg->m_endframe - m_dlg->m_startframe );
	*posptr = MP_SIZE + (int)(step * (float)( frameno - m_dlg->m_startframe ) ) + clirect.left;
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	
	return 0;
}

int CDSKeyDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_dlg->m_endframe - m_dlg->m_startframe );
	float tempfloat;
	tempfloat = (float)(posx - MP_SIZE - clirect.left) / step; 
	//四捨五入してframenoにする。
	int tempint;
	tempint = (int)( tempfloat + 0.49999f ) + m_dlg->m_startframe;

	if( tempint < m_dlg->m_startframe ){
		*framenoptr = m_dlg->m_startframe;
	}else if( tempint > m_dlg->m_endframe ){
		*framenoptr = m_dlg->m_endframe;
	}else{
		*framenoptr = tempint;
	}
	
	return 0;
}

int CDSKeyDlg::DrawDSKeys()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 ){
		return 0;
	}
	if( m_cmdshow != SW_SHOW ){
		return 0;
	}

	HDC hdc;
	if( !m_mp_wnd.IsWindow() ){
		return 0;
	}
	hdc = m_mp_wnd.GetDC();
	_ASSERT( hdc != NULL );

	if( m_black_brush == NULL ){
		COLORREF blackcol = RGB( 0, 0, 0 );
		m_black_brush = ::CreateSolidBrush( blackcol );
		_ASSERT( m_black_brush != NULL );
	}
	if( m_white_brush == NULL ){
		COLORREF whitecol = RGB( 255, 255, 255 );
		m_white_brush = ::CreateSolidBrush( whitecol );
		_ASSERT( m_white_brush != NULL );
	}
	if( m_red_brush == NULL ){
		COLORREF redcol = RGB( 255, 0, 0 );
		m_red_brush = ::CreateSolidBrush( redcol );
		_ASSERT( m_red_brush != NULL );
	}
	if( m_blue_brush == NULL ){
		COLORREF bluecol = RGB( 0, 0, 255 );
		m_blue_brush = ::CreateSolidBrush( bluecol );
		_ASSERT( m_blue_brush != NULL );
	}

	if( m_black_pen == NULL ){
		COLORREF blackcol = RGB( 0, 0, 0 );
		m_black_pen = ::CreatePen( PS_SOLID, 0, blackcol );
		_ASSERT( m_black_pen != NULL );
	}
	if( m_red_pen == NULL ){
		COLORREF redcol = RGB( 255, 0, 0 );
		m_red_pen = ::CreatePen( PS_SOLID, 0, redcol );
		_ASSERT( m_red_pen != NULL );
	}
	if( m_blue_pen == NULL ){
		COLORREF bluecol = RGB( 0, 0, 255 );
		m_blue_pen = ::CreatePen( PS_SOLID, 0, bluecol );
		_ASSERT( m_blue_pen != NULL );
	}

//背景塗りつぶし
	RECT clirect;
	if( !m_mp_wnd.IsWindow() ){
		return 0;
	}
	m_mp_wnd.GetClientRect( &clirect );
	HPEN hOldPen = (HPEN)SelectObject( hdc, m_black_pen );
	HBRUSH hOldBrush = (HBRUSH)SelectObject( hdc, m_white_brush );

	BOOL bret;
	bret = ::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret ){
		_ASSERT( 0 );
		return 1;
	}

//センターライン
	SelectObject( hdc, m_black_brush );
    ::MoveToEx( hdc, clirect.left, clirect.bottom / 2, NULL ); 
    ::LineTo( hdc, clirect.right, clirect.bottom / 2 );

//current bar
	int barposx;
	Frameno2Windowpos( clirect, m_dlg->m_current, &barposx );

	::MoveToEx( hdc, barposx, clirect.top, NULL );
	::LineTo( hdc, barposx, clirect.bottom );

//点
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;

	CDSKey* dskptr = m_dlg->m_mhandler->GetFirstDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie );
	CDSKey* curdsk = dskptr;
	while( curdsk ){
		int frameno = curdsk->frameno;
		if( (frameno >= m_dlg->m_startframe) && (frameno <= m_dlg->m_endframe) ){
			if( frameno != m_dlg->m_current ){
				SelectObject( hdc, m_black_pen );
				SelectObject( hdc, m_black_brush );
			}else{
				SelectObject( hdc, m_red_pen );
				SelectObject( hdc, m_red_brush );
			}
			//int posx = MP_SIZE + (int)(step * (float)(frameno));
			int posx;
			Frameno2Windowpos( clirect, frameno, &posx );
			int posy = (clirect.bottom - clirect.top) / 2;
			::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );
		}

		curdsk = curdsk->next;
		//DbgOut( "DSKeyDlg : DrawMotionPoint : frame %d\n", frameno );
	}

	
	if( m_dragdsk ){
		int frameno = m_dragdsk->frameno;
		
		SelectObject( hdc, m_blue_pen );
		SelectObject( hdc, m_blue_brush );
		
		//int posx = MP_SIZE + (int)(step * (float)(frameno));
		int posx;
		Frameno2Windowpos( clirect, frameno, &posx );
		int posy = (clirect.bottom - clirect.top) / 2;
		::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );		
	}
	

	::SelectObject( hdc, hOldPen );
	::SelectObject( hdc, hOldBrush );
	m_mp_wnd.ReleaseDC( hdc );

	return 0;
}

int CDSKeyDlg::Redraw()
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}

LRESULT CDSKeyDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	if( !IsWindow() )
		return 0;

//	/***
//	if( uMsg != 0 )
//		::DefWindowProc( m_hWnd, uMsg, wParam, lParam );
//	else
//		UpdateAllWnd();
//	***/
	
	DrawDSKeys();

	return 0;
}

LRESULT CDSKeyDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
/***
	if( m_thandler && m_shandler && m_mhandler && (m_selectedno != 0)){
		::SendMessage( m_apphwnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)m_selectedno );
	}
	Redraw();
***/
	DefWindowProc();

	Redraw();

	return 0;
}
LRESULT CDSKeyDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "DSKeyDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CDSKeyDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_ismoving = 0;
/***
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	if( m_apphwnd ){
		::SendMessage( m_apphwnd, WM_USER_MOVE, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	}
***/
	DefWindowProc();

	Redraw();

	return 0;
}



LRESULT CDSKeyDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	int ret;

	//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = ::MessageBox( m_hWnd, "全てのディスプレイスイッチのキーフレームを削除します。\nよろしいですか？", "確認ダイアログ", MB_OKCANCEL ); 
	//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK )
		return 0;

	int i;
	for( i = 0; i < m_dlg->m_mhandler->s2mot_leng; i++ ){
		CDSKey* dskptr;
		dskptr = m_dlg->m_mhandler->GetFirstDSKey( i, m_dlg->m_mot_cookie );

		CDSKey* curdsk = dskptr;
		CDSKey* nextdsk;
		while( curdsk ){
			int frameno = curdsk->frameno;
			nextdsk = curdsk->next;
			
			ret = m_dlg->m_mhandler->DeleteDSKey( i, m_dlg->m_mot_cookie, frameno );
			if( ret ){
				DbgOut( "DSKeyDlg : OnAllDelete : mh DeleteDSKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			curdsk = nextdsk;
		}
	}
	
	if( m_dragdsk ){
		delete m_dragdsk;
		m_dragdsk = 0;
	}
////////
	m_remakelistflag = 1;
	ret = InitList();
	_ASSERT( !ret );
	//SetCapture();

	ParamsToDlg();
	Redraw();

	return 0;
}


LRESULT CDSKeyDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 ){
		return 0;
	}

	BOOL dummyb;
	if( wID == 9999 ){
		m_dlg->OnStop( 0, 9999, 0, dummyb );
	}else{
		m_dlg->OnStop( 0, 0, 0, dummyb );
	}
	InitList();
	ParamsToDlg();
	return 0;
}


LRESULT CDSKeyDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );


	BOOL dummyb;
	return m_dlg->OnPreview( 0, 0, 0, dummyb );

}

int CDSKeyDlg::OnUserSelchange()
{
	int ret;
	m_remakelistflag = 1;
	ret = InitList();
	_ASSERT( !ret );
	//SetCapture();

	ParamsToDlg();
	Redraw();

	return 0;
}

int CDSKeyDlg::CaptureM()
{

	RECT viewclient;
	m_mp_wnd.GetClientRect( &viewclient );

	POINT inview;
	::GetCursorPos( &inview );
	m_mp_wnd.ScreenToClient( &inview );

	HWND capwnd;
	capwnd = ::GetCapture();

	if( (capwnd != m_hWnd ) && (m_cmdshow != SW_HIDE) ){
		if( (inview.x >= viewclient.left) && (inview.x <= viewclient.right) && (inview.y >= viewclient.top) && (inview.y <= viewclient.bottom) ){
			SetCapture();
			//DbgOut( "lightdlg : MouseMove : SetCapture !!!!!!!!!!!!!!\r\n" );
			m_mcap = 1;
		}
	}else{
		if( (m_moving == 0) && (m_cmdshow != SW_HIDE) ){
			if( (inview.x < viewclient.left) || (inview.x > viewclient.right) || (inview.y < viewclient.top) || (inview.y > viewclient.bottom) ){
			//if( (indlg.x < dlgclient.left) || (indlg.x > dlgclient.right) || (indlg.y < dlgclient.top) || (indlg.y > dlgclient.bottom) ){
				ReleaseCapture();
				//DbgOut( "lightdlg: MouseMove : ReleaseCapture !!!!!!!!!!!\r\n" );
				m_mcap = 0;
				//m_lbutton = 0;//!!!!!!!!!!
			}
		}
	}

	return 0;
}


LRESULT CDSKeyDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;


	CaptureM();


	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "DSKeyDlg : OnMouseMove : MK_LBUTTON\n" );

	if( (m_dragflag == 1) && m_dragdsk ){
		POINT movepoint;
		movepoint.x = LOWORD(lParam);
		movepoint.y = HIWORD(lParam);
		m_dlg_wnd.ClientToScreen( &movepoint );
		
		RECT winrect;
		m_mp_wnd.GetWindowRect( &winrect );
		int posx, frameno;
		posx = movepoint.x - winrect.left;
		RECT clirect;
		m_mp_wnd.GetClientRect( &clirect );		
		Windowpos2Frameno( clirect, posx, &frameno );

		m_dragdsk->frameno = frameno;

		Redraw();
	}

	if( m_ctrldrag ){
		int newx;
		newx = LOWORD(lParam);
		int dx;
		float divx;
		divx = 1200.0f / (float)m_dlg->m_sl_mp_rangemax;
		dx = (int)( (float)( m_dragx - newx ) / divx );

		int changex;
		if( dx > 0 ){
			changex = 1;
		}else if( dx < 0 ){
			changex = -1;
		}else{
			return 0;
		}
		m_dragx = newx;

		int newend;
		newend = m_dlg->m_endframe + changex;

		if( newend > m_dlg->m_sl_mp_rangemax ){
			newend = m_dlg->m_sl_mp_rangemax;
		}else if( newend <= m_dlg->m_startframe ){
			newend = m_dlg->m_startframe;
		}

		m_dlg->m_endframe = newend;
		if( (m_dlg->m_current < m_dlg->m_startframe) || (m_dlg->m_current > m_dlg->m_endframe) ){
			m_dlg->m_current = m_dlg->m_startframe;
		}

		m_dlg->m_motid[ m_dlg->m_mot_cookie ].endframe = m_dlg->m_endframe;//!!!!!!!!!!!!

		ParamsToDlg();
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );

	}else if( m_altdrag ){

		int newx;
		newx = LOWORD(lParam);
		int dx;
		float divx;
		divx = 1200.0f / (float)m_dlg->m_sl_mp_rangemax;
		dx = (int)( (float)( newx - m_dragx ) / divx );

		int changex;
		if( dx > 0 ){
			changex = 1;
		}else if( dx < 0 ){
			changex = -1;
		}else{
			return 0;
		}
		m_dragx = newx;


		int maxdx;
		maxdx = m_dlg->m_sl_mp_rangemax - m_dlg->m_endframe;
		int mindx;
		mindx = 0 - m_dlg->m_startframe;

		if( changex > 0 ){
			int shiftx;
			shiftx = min( changex, maxdx );
			m_dlg->m_startframe += shiftx;
			m_dlg->m_endframe += shiftx;
			if( (m_dlg->m_current < m_dlg->m_startframe) || (m_dlg->m_current > m_dlg->m_endframe) ){
				m_dlg->m_current = m_dlg->m_startframe;
			}
		}else if( changex < 0 ){
			int shiftx;
			shiftx = max( changex, mindx );
			m_dlg->m_startframe += shiftx;
			m_dlg->m_endframe += shiftx;
			if( (m_dlg->m_current < m_dlg->m_startframe) || (m_dlg->m_current > m_dlg->m_endframe) ){
				m_dlg->m_current = m_dlg->m_startframe;
			}			
		}else{
			return 0;
		}

		m_dlg->m_motid[ m_dlg->m_mot_cookie ].startframe = m_dlg->m_startframe;
		m_dlg->m_motid[ m_dlg->m_mot_cookie ].endframe = m_dlg->m_endframe;


		ParamsToDlg();
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}


	return 0;
}


LRESULT CDSKeyDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	int ret;
	CDSKey* curdsk = 0;
	ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
	if( ret ){
		DbgOut( "DSKeyDlg : OnFrameCopy : mh ExistDSKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( !curdsk ){
		::MessageBox( m_hWnd, "このフレームにはコピーできるキーがありません。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!
	}

	if( m_cpdsk ){
		delete m_cpdsk;
		m_cpdsk = 0;
	}

	m_cpdsk = new CDSKey();
	if( !m_cpdsk ){
		DbgOut( "DSKeyDlg : OnFrameCopy : cpdsk alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_cpdsk->CopyDSChange( curdsk );
	if( ret ){
		DbgOut( "DSKeyDlg : OnFrameCopy : cpdsk CopyDSChange error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

LRESULT CDSKeyDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	

	if( !m_cpdsk ){
		::MessageBox( m_hWnd, "フレームコピーを行ってから再試行してください。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!!!!
	}

	int ret;
	CDSKey* curdsk = 0;
	ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
	if( ret ){
		DbgOut( "DSKeyDlg : OnFramePaste : mh ExistDSKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !curdsk ){
		CDSKey* adddsk;
		adddsk = new CDSKey();
		if( !adddsk ){
			DbgOut( "DSKeyDlg : OnFramePaste : adddsk alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = adddsk->CopyDSChange( m_cpdsk );
		if( ret ){
			DbgOut( "DSKeyDlg : OnFramePaste : adddsk CopyDSChange error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		adddsk->frameno = m_dlg->m_current;

		ret = m_dlg->m_mhandler->ChainDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, adddsk );
		if( ret ){
			DbgOut( "DSKeyDlg : OnFramePaste : mh ChainDSKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{

		ret = curdsk->CopyDSChange( m_cpdsk );
		if( ret ){
			DbgOut( "DSKeyDlg : OnFramePaste : adddsk CopyDSChange 1 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	m_remakelistflag = 1;
	ret = InitList();
	_ASSERT( !ret );
	//SetCapture();

	ParamsToDlg();
	Redraw();


	return 0;
}

LRESULT CDSKeyDlg::OnFrameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	int ret;
	CDSKey* curdsk = 0;
	ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
	if( ret ){
		DbgOut( "DSKeyDlg : OnFrameDel : mh ExistDSKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( curdsk ){
		ret = m_dlg->m_mhandler->DeleteDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current );
		if( ret ){
			DbgOut( "DSKeyDlg : OnFrameDel : mh DeleteDSKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
/////////
	m_remakelistflag = 1;
	ret = InitList();
	_ASSERT( !ret );
	//SetCapture();

	ParamsToDlg();
	Redraw();

	return 0;
}


LRESULT CDSKeyDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler ) 
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;


	if( m_dragflag )
		return 0;

	int ret;
	POINT clickpoint;
	clickpoint.x = LOWORD(lParam);
	clickpoint.y = HIWORD(lParam);
	if( g_KeyBuffer[VK_CONTROL] & 0x80 ){
		m_ctrldrag = 1;
		m_altdrag = 0;
		m_dragx = clickpoint.x;
	}else if( g_KeyBuffer[VK_MENU] & 0x80 ){
		m_altdrag = 1;
		m_ctrldrag = 0;
		m_dragx = clickpoint.x;
	}else{
		m_dlg_wnd.ClientToScreen( &clickpoint );

		RECT winrect;
		m_mp_wnd.GetWindowRect( &winrect );

		int centery, uppery, lowery;
		centery = winrect.top + (winrect.bottom - winrect.top) / 2;
		uppery = centery - MP_SIZE;
		lowery = centery + MP_SIZE;

		if( (clickpoint.x >= winrect.left) && (clickpoint.x <= winrect.right) 
			&& (clickpoint.y >= uppery) && (clickpoint.y <= lowery)
			){

			RECT clirect;
			m_mp_wnd.GetClientRect( &clirect );
			int posx, frameno;
			posx = clickpoint.x - winrect.left;
			Windowpos2Frameno( clirect, posx, &frameno );

			CDSKey* isexist = 0;
			m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, frameno, &isexist );
			if( isexist ){
				m_dragframe = frameno;
				m_dragflag = 1;
				m_dragdsk = isexist;

				ret = m_dlg->m_mhandler->LeaveFromChainDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dragframe );
				if( ret ){
					DbgOut( "DSKeyDlg : OnLButtonDown : LeaveFromChainDSKey error !!!\n" );
					_ASSERT( 0 );
					m_dragframe = 0;
					m_dragflag = 0;
					m_dragdsk = 0;
					return 1;
				}

				DbgOut( "DSKeyDlg : OnLButtonDown : drag start, posx %d, m_dragframe %d\n",
					posx, m_dragframe );
			}else{
				m_dragframe = 0;
				m_dragflag = 0;
				m_dragdsk = 0;
			}
		}else{
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragdsk = 0;
		}
	}

	return 0;
}


LRESULT CDSKeyDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	static int busyflag = 0;


	if( busyflag == 1 )
		return 0;

	busyflag = 1;

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;

	int ret;

	int frameno = -1;

	CDSKey* isexist = 0;
	CDSKey* isexist2 = 0;

	if( m_ctrldrag || m_altdrag ){
		m_ctrldrag = 0;
		m_altdrag = 0;
		m_dragx = 0;
	}else{

		if( (m_dragflag == 1) && m_dragdsk ){
			POINT droppoint;
			droppoint.x = LOWORD(lParam);
			droppoint.y = HIWORD(lParam);
			m_dlg_wnd.ClientToScreen( &droppoint );
			
			RECT winrect;
			m_mp_wnd.GetWindowRect( &winrect );
			int posx;
			posx = droppoint.x - winrect.left;
			RECT clirect;
			m_mp_wnd.GetClientRect( &clirect );		
			Windowpos2Frameno( clirect, posx, &frameno );

	//DbgOut( "DSKeyDlg : OnLButtonUp : Windowpos2Frameno : frameno %d\n", frameno );

			ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, frameno, &isexist );
			_ASSERT( !ret );
			if( isexist ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "フレーム番号%d には既にモーションポイントが存在するので、移動できません。",
					frameno );
				//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
				MessageBox( messtr, "移動できません", MB_OK );
				//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	//DbgOut( "DSKeyDlg : OnLButtonUp : already exist : m_dragframe %d, frameno %d\n", m_dragframe, frameno );
				frameno = m_dragframe;//!!!!! 元に戻す
			}
			
			m_dragdsk->frameno = frameno;//!!!!!!!!!!!!!!!!

			ret = m_dlg->m_mhandler->ChainDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dragdsk );
			if( ret ){
				DbgOut( "DSKeyDlg : OnLButtonUp : mh ChainDSKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_dragdsk = 0;


			m_remakelistflag = 1;
			ret = InitList();
			_ASSERT( !ret );
			//SetCapture();

			ParamsToDlg();
			Redraw();

			//DbgOut( "DSKeyDlg : OnLButtonUp : drag end, posx %d, frameno %d, m_dragframe %d\n",
			//	posx, frameno, m_dragframe );
		}

		//if( frameno == m_dragframe ){
		//	DbgOut( "DSKeyDlg : OnLButtonUp : motion point already exist warning %d, %d !!!\n", frameno, m_dragframe );
		//}
	}

	m_dragflag = 0;
	m_dragframe = 0;

	busyflag = 0;

	return 0;
}
LRESULT CALLBACK MPProc3(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  ){
	if( (uMsg != WM_KEYDOWN) && (uMsg != WM_KEYUP) ){
		return CallWindowProc(
		  (WNDPROC)s_mpfunc,  // pointer to previous procedure
		  s_sl_mp_wnd,              // handle to window
		  uMsg,               // message
		  wParam,          // first message parameter
		  lParam           // second message parameter
		);		
	}else{
		//key は無効にする。

		//if( uMsg == WM_KEYDOWN ){
		//	DbgOut( "DSKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "DSKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}


		return 0;
	}
}
/***
LRESULT CALLBACK ListProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  ){

	CallWindowProc(
		  (WNDPROC)s_listfunc,  // pointer to previous procedure
		  s_list_wnd,              // handle to window
		  uMsg,               // message
		  wParam,          // first message parameter
		  lParam           // second message parameter
	);		

	if( uMsg == WM_LBUTTONDOWN ){
		::SendMessage( s_dlg_wnd, WM_LBUTTONDOWN, wParam, lParam );
	}

	return 0;
}
***/
int CDSKeyDlg::SetNewWindowProc()
{
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWLP_WNDPROC );
	if( !s_mpfunc ){
		//_ASSERT( 0 );
		return 1;
	}

//	s_listfunc = ::GetWindowLong( m_list_wnd, GWLP_WNDPROC );
//	if( !s_listfunc ){
//		_ASSERT( 0 );
//		return 1;
//	}


	LONG lret;
	lret = ::SetWindowLong( m_sl_mp_wnd, GWLP_WNDPROC, (LONG)MPProc3 );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}

//	lret = ::SetWindowLong( m_list_wnd, GWLP_WNDPROC, (LONG)ListProc );
//	if( lret == 0 ){
//		_ASSERT( 0 );
//		return 1;
//	}


	return 0;
}


LRESULT CDSKeyDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		
	bHandled = TRUE;
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;

	int timerid = (int)wParam;
	if( (timerid == DSKeyDlgTIMER) && (m_cmdshow == SW_SHOW) ){

		//ドラッグ中に、マウスが外に出た場合は、ドラッグ操作を強制終了する。
		if( m_dragflag == 1 ){
			POINT ptCursor;
			POINT savecursor;
			GetCursorPos( &ptCursor );
			
			savecursor = ptCursor;//
			::ScreenToClient( m_hWnd, &ptCursor );

			RECT dlgrect;
			//m_dlg_wnd.GetWindowRect( &dlgrect );
			m_mp_wnd.GetWindowRect( &dlgrect );


			if( (savecursor.x < dlgrect.left) || (savecursor.x > dlgrect.right) 
				|| (savecursor.y < dlgrect.top) || (savecursor.y > dlgrect.bottom)
			){
				LPARAM lParam;
				BOOL bHandled;

				lParam = MAKELPARAM( ptCursor.x, ptCursor.y );
				OnLButtonUP( 0, 0, lParam, bHandled);
			}
		}

		//redraw
		Redraw();

	}

	return 0;
}


int CDSKeyDlg::ShowDlg( int cmdshow, int stopflag )
{
	m_cmdshow = cmdshow;

	if( (m_cmdshow == SW_HIDE) ){
		BOOL handled;
		if( m_dlg->m_thandler && stopflag )
			OnStop( 0, 9999, 0, handled );
		
		ReleaseCapture();
		m_mcap = 0;
		m_moving = 0;

		m_remakelistflag = 0;
		
//		HWND capwnd;
//		capwnd = ::GetCapture();
//		if( capwnd == m_dlg_wnd.m_hWnd ){
//			ReleaseCapture();
//		}
	}

	ShowWindow( m_cmdshow );

	//int ret;
	if( m_cmdshow != SW_HIDE ){
		BOOL dummyb;
		OnStop( 0, 9999, 0, dummyb );
		//m_dlg->m_current = 0;//!!!!!!!!!!!!!!!!!!!!!! 別スレッドでのセットが遅れるため、ここで強制リセット。
		OnUserSelchange();
	}


	return 0;
}

LRESULT CDSKeyDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CDSKey* dskptr = m_dlg->m_mhandler->GetFirstDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie );

	CDSKey* curdsk;
	CDSKey* befdsk;
	curdsk = dskptr;
	befdsk = curdsk;
	while( curdsk && (curdsk->frameno < m_dlg->m_current) ){
		befdsk = curdsk;
		curdsk = curdsk->next;
	}	

	if( befdsk ){
		m_dlg->m_current = befdsk->frameno;
	}else{
		m_dlg->m_current = 0;
	}

///////

	//m_currentがm_startframeより小さい場合でも、RANGEMINの設定によりスライダーは最小値より小さくならない。
	//OnReleasecaptureSl_mpで、スライダーの現在の場所からm_currentを設定しなおすので問題ない。

	m_dlg->m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_dlg->m_current );
	BOOL dummy;	
	m_dlg->OnReleasedcaptureSl_mp( 0, 0, dummy );

	InitList();
	ParamsToDlg();
	Redraw();


	return 0;
}
LRESULT CDSKeyDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CDSKey* dskptr = m_dlg->m_mhandler->GetFirstDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie );

	CDSKey* curdsk;
	CDSKey* finddsk;

	curdsk = dskptr;
	finddsk = 0;
	while( curdsk ){
		if( curdsk->frameno > m_dlg->m_current ){
			if( (curdsk->frameno >= m_dlg->m_startframe) && (curdsk->frameno <= m_dlg->m_endframe) ){
				finddsk = curdsk;
				break;
			}
		}
		curdsk = curdsk->next;
	}

	if( finddsk ){
		m_dlg->m_current = finddsk->frameno;
	}else{
		m_dlg->m_current = m_dlg->m_endframe;
	}
///////////
	m_dlg->m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_dlg->m_current );
	BOOL dummy;	
	m_dlg->OnReleasedcaptureSl_mp( 0, 0, dummy );

	InitList();
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CDSKeyDlg::OnCreateKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;


	int ret;
	if( (m_dlg->m_selectedno > 0) && m_dlg->m_mhandler ){
		CDSKey* curdsk = 0;
		ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
		if( ret ){
			DbgOut( "DSKeyDlg : OnCreateKey : mh ExistDSKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( !curdsk ){
			CDSKey* newdsk = 0;
			ret = m_dlg->m_mhandler->CreateDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &newdsk );
			if( ret || !newdsk ){
				DbgOut( "DSKeyDlg : OnCreateKey : mh CreateDSKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_remakelistflag = 1;
			ret = InitList();
			

			_ASSERT( !ret );
		}

		Redraw();
	}

	return 0;
}


int CDSKeyDlg::InitList()
{
	int ret;
	ListView_DeleteAllItems( m_list_wnd );
	
	if( m_createlistflag == 0 ){
		LV_COLUMN lvcol;
		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 150;
		lvcol.pszText = "スイッチ番号";
		lvcol.iSubItem = DSSUB_SWNO;
		ListView_InsertColumn(m_list_wnd, DSSUB_SWNO, &lvcol);

		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 150;
		lvcol.pszText = "スイッチの状態";
		lvcol.iSubItem = DSSUB_STATE;
		ListView_InsertColumn(m_list_wnd, DSSUB_STATE, &lvcol);
	}
	m_createlistflag = 1;


	if( (m_dlg->m_selectedno > 0) && m_dlg->m_shandler && m_dlg->m_mhandler ){

		CDSKey* curdsk = 0;
		ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
		if( ret ){
			DbgOut( "DSKeyDlg : InitList : mh ExistDSKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int setno = 0;
		CDispSwitch* curds;
		for( setno = 0; setno < DISPSWITCHNUM; setno++ ){
			curds = m_dlg->m_shandler->m_ds + setno;

			CDSChange* curdsc = 0;
			if( curdsk ){
				ret = curdsk->ExistDSChange( curds, &curdsc );
				if( ret ){
					DbgOut( "DSKeyDlg : InitList : dskey ExistDSChange error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}

			ret = AddItem2List( setno, curds, curdsc );
			if( ret ){
				DbgOut( "DSKeyDlg : InitList : AddItem2List error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	ret = m_dlg->SetCurrentMotion();
	_ASSERT( !ret );

	return 0;
}
int CDSKeyDlg::AddItem2List( int itemno, CDispSwitch* srcds, CDSChange* srcdsc )
{
	char tempchar[256];

	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));

	sprintf_s( tempchar, 256, "%d", srcds->switchno );


	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.pszText = tempchar;
	item.iItem = itemno;
	item.iSubItem = DSSUB_SWNO;
	item.lParam = srcds->switchno;
	ListView_InsertItem(m_list_wnd, &item);


	if( srcdsc ){
		if( srcdsc->m_change == 0 ){
			strcpy_s( tempchar, 256, "オフ" );
		}else{
			strcpy_s( tempchar, 256, "オン" );
		}
	}else{
		strcpy_s( tempchar, 256, "設定なし" );
	}

item.mask = LVIF_TEXT;
	item.pszText = tempchar;
	item.iItem = itemno;
	item.iSubItem = DSSUB_STATE;
//	item.lParam = telem->serialno;
	ListView_SetItem(m_list_wnd, &item);

	return 0;
}

int CDSKeyDlg::OnList1( int hititem, POINT inview )
{

	if( !m_dlg->m_mhandler || (m_dlg->m_selectedno <= 0) || (m_dlg->m_mot_cookie < 0) ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	int ret;

	LVITEM lvitem2;
	ZeroMemory( &lvitem2, sizeof( LVITEM ) );
	char tempname2[256];
	ZeroMemory( tempname2, 256 );

	lvitem2.iItem = hititem;
	lvitem2.iSubItem = DSSUB_SWNO;//!!!!!!!!!!!!
	lvitem2.pszText = tempname2;
	lvitem2.cchTextMax = 256;

	lvitem2.mask = LVIF_TEXT | LVIF_PARAM;
				
	BOOL bret2;
	bret2 = ListView_GetItem( m_list_wnd, &lvitem2 );
	if( bret2 == TRUE ){

		int curswno;
		curswno = (int)lvitem2.lParam;
		if( (curswno < 0) || (curswno >= DISPSWITCHNUM) ){
			DbgOut( "DSKeyDlg : OnList1 : curswno error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		CDispSwitch* curds;
		curds = m_dlg->m_shandler->m_ds + curswno;


//DbgOut( "DSKeyDlg : OnList1 : curmoatno %d, curmat %x\r\n", curmatno, curmat );

		CDSMenuMain dsmenu;
		ret = dsmenu.Create( m_dlg->m_apphwnd );
		if( ret ){
			DbgOut( "DSKeyDlg : OnList1 : dsmenu Create error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int selectno;
		POINT pt;
		::GetCursorPos( &pt );
		selectno = dsmenu.TrackPopupMenu( m_hWnd, pt );
		dsmenu.Destroy();

		if( selectno == 65500 ){
			CDSKey* curdsk = 0;
			ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
			if( ret ){
				DbgOut( "DSKeyDlg : OnList1 : mh ExistDSKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			if( curdsk ){
				CDSChange* curdsc = 0;
				ret = curdsk->ExistDSChange( curds, &curdsc );
				if( ret ){
					DbgOut( "DSKeyDlg : OnList1 : dskey ExistDSChange error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				if( curdsc ){
					ret = curdsk->DeleteDSChange( curds );
					if( ret ){
						DbgOut( "DSKeyDlg : OnList1 : dsk DeleteDSChange error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					ListView_SetItemText( m_list_wnd, hititem, DSSUB_STATE, "設定なし" );
				}
			}

		}else if( selectno >= 65001 ){
			BOOL dummyb = 0;
			OnCreateKey( 0, 0, 0,  dummyb );

			CDSKey* curdsk = 0;
			ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
			if( ret || !curdsk ){
				DbgOut( "DSKeyDlg : OnList1 : mh ExistDSKey 1 error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			char tempchar[256];
			int newstate = 0;
			if( selectno == 65501 ){
				newstate = 1;
				sprintf_s( tempchar, 256, "オン" );
			}else if( selectno == 65502 ){
				newstate = 0;
				sprintf_s( tempchar, 256, "オフ" );
			}else{
				_ASSERT( 0 );
				newstate = 0;
				sprintf_s( tempchar, 256, "オフ" );
			}

			ret = curdsk->SetDSChange( curds, newstate );
			if( ret ){
				DbgOut( "DSKeyDlg : OnList1 : dsk SetDSChange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ListView_SetItemText( m_list_wnd, hititem, DSSUB_STATE, tempchar );


		}

//		m_remakelistflag = 1;
//		InitList();
		ret = m_dlg->SetCurrentMotion();
		_ASSERT( !ret );
		

	}

	Redraw();

	return 0;
}

LRESULT CDSKeyDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CDSKeyDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}

LRESULT CDSKeyDlg::OnSelchangeList1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NMLISTVIEW* nmlistview = (NMLISTVIEW*)pnmh;

//	char testmes[256];
//	sprintf_s( testmes, 256, "flag %d, item %d, uNewState %d, uOldState %d, uChanged %d", 
//		m_remakelistflag, nmlistview->iItem, nmlistview->uNewState, nmlistview->uOldState, nmlistview->uChanged );
//	::MessageBox( m_hWnd, testmes, "通知", MB_OK );

	int ret;

	if( (m_remakelistflag == 0) && (nmlistview->uNewState == 3) ){

//		char testmes[256];
//		sprintf_s( testmes, 256, "item %d, subitem %d, uNewState %d, uOldState %d, uChanged %d", 
//			nmlistview->iItem, nmlistview->iSubItem, nmlistview->uNewState, nmlistview->uOldState, nmlistview->uChanged );
//		::MessageBox( m_hWnd, testmes, "通知", MB_OK );


		ret = OnList1( nmlistview->iItem, nmlistview->ptAction );
		_ASSERT( !ret );

	}else if( nmlistview->uNewState == 3 ){
		m_remakelistflag = 0;
	}
	
	return 0;
}


LRESULT CDSKeyDlg::OnAllOn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;


	int ret;
	if( (m_dlg->m_selectedno > 0) && m_dlg->m_mhandler ){
		BOOL dummyb;
		OnCreateKey( 0, 0, 0, dummyb );

		CDSKey* curdsk = 0;
		ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
		if( ret || !curdsk ){
			DbgOut( "DSKeyDlg : OnAllOn : mh ExistDSKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int dsno;
		for( dsno = 0; dsno < DISPSWITCHNUM; dsno++ ){
			ret = curdsk->SetDSChange( m_dlg->m_shandler->m_ds + dsno, 1 );
			if( ret ){
				DbgOut( "DSKeyDlg : OnAllOn : dsk SetDSChange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		InitList();
		ParamsToDlg();
		Redraw();

	}

	return 0;
}
LRESULT CDSKeyDlg::OnAllOff(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;


	int ret;
	if( (m_dlg->m_selectedno > 0) && m_dlg->m_mhandler ){
		BOOL dummyb;
		OnCreateKey( 0, 0, 0, dummyb );

		CDSKey* curdsk = 0;
		ret = m_dlg->m_mhandler->ExistDSKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curdsk );
		if( ret || !curdsk ){
			DbgOut( "DSKeyDlg : OnAllOff : mh ExistDSKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int dsno;
		for( dsno = 0; dsno < DISPSWITCHNUM; dsno++ ){
			ret = curdsk->SetDSChange( m_dlg->m_shandler->m_ds + dsno, 0 );
			if( ret ){
				DbgOut( "DSKeyDlg : OnAllOff : dsk SetDSChange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		InitList();
		ParamsToDlg();
		Redraw();
	}

	return 0;
}
int CDSKeyDlg::OnJumpFrame( int srcframe )
{
	m_dlg->OnJumpFrame( srcframe );

	InitList();
	ParamsToDlg();
	Redraw();

	return 0;
}
