// MorphKeyDlg.cpp : CMorphKeyDlg のインプリメンテーション
#include "stdafx.h"
#include "MorphKeyDlg.h"

#include <math.h>
#include <stdio.h>
#include <D3DX9.h>

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

#include <MMotKey.h>
#include <MMotElem.h>
#include <morph.h>


#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "viewer.h"
#include "MotDlg.h"
#include "MotParamDlg.h"

#include "GetDlgParams.h"
#include "SyncDlg.h"

#include <crtdbg.h>


//MotionPoint描画時の点の半径
#define MP_SIZE	3

/////////////////
// sttic
static LONG s_mpfunc = 0;
static LONG s_listfunc = 0;
static LONG s_valfunc = 0;
static HWND s_dlg_wnd = 0;
static HWND s_apphwnd = 0;
static HWND s_sl_mp_wnd = 0;
static HWND s_sl_val_wnd = 0;
static HWND s_list_wnd = 0;

/////////////////
//extern HINSTANCE g_resinstance;
extern CMotDlg* g_motdlg;
extern unsigned char g_KeyBuffer[256];
extern int g_endapp;

/////////////////////////////////////////////////////////////////////////////
// CMorphKeyDlg

CMorphKeyDlg::CMorphKeyDlg( CMotParamDlg* srcmpdlg )
//: m_list_wnd( NULL, this, 2 )
{

	m_dlg = srcmpdlg;
	m_dlg->m_MorphKeyDlg = this;

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

	m_dragmme = 0;
	m_dragframe = 0;
	m_dropframe = 0;
	m_dragflag = 0;


	m_timerworking = 0;

	m_cmdshow = SW_HIDE;
	m_createlistflag = 0;
	m_mcap = 0;
	m_moving = 0;

	m_remakelistflag = 0;

	m_cpmme = 0;

	m_base = 0;
	m_target = 0;
	m_boneelem = 0;
	m_value = 0.0f;

	m_ppselem = 0;

	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

}


CMorphKeyDlg::~CMorphKeyDlg()
{
	if( m_cpmme ){
		delete m_cpmme;
		m_cpmme = 0;
	}

	if( m_ppselem ){
		free( m_ppselem );
		m_ppselem = 0;
	}
}

int CMorphKeyDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, MorphKeyDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	return 0;
}

LRESULT CMorphKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	ret = SetWnd();
	_ASSERT( !ret );
	s_sl_mp_wnd = m_sl_mp_wnd;
	s_sl_val_wnd = m_sl_val_wnd;

	ret = SetNewWindowProc();
	_ASSERT( !ret );


	ret = ParamsToDlg();
	_ASSERT( !ret );

	int timerid;
	timerid = (int)::SetTimer( m_hWnd, MorphKeyDlgTIMER, 500, NULL );
	_ASSERT( timerid == MorphKeyDlgTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}


int CMorphKeyDlg::InitDlg()
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

LRESULT CMorphKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "MorphKeyDlg : OnOK\n" );
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}

LRESULT CMorphKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}
LRESULT CMorphKeyDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}


int CMorphKeyDlg::SetWnd()
{

	HWND temphwnd;

	m_dlg_wnd = m_hWnd;
	s_dlg_wnd = m_hWnd;

	temphwnd = GetDlgItem( IDC_LIST1 );
	m_list_wnd = temphwnd;
	s_list_wnd = temphwnd;

	m_tex_current_wnd = GetDlgItem( IDC_TEX_CURRENT );

	m_mp_wnd = GetDlgItem( IDC_MP );
	m_sl_mp_wnd = GetDlgItem( IDC_SL_MP );
	m_sl_val_wnd = GetDlgItem( IDC_SL_VAL );
	m_sp_mp_wnd = GetDlgItem( IDC_SP_MP );
	m_combo1_wnd = GetDlgItem( IDC_COMBO1 );
	m_edit_val_wnd = GetDlgItem( IDC_EDIT_VAL );

	//sliderの初期設定
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	m_sl_val_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_val_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)100 );
	SetSliderTic( m_sl_val_wnd, 10, 0, 100 );//


	return 0;
}

int CMorphKeyDlg::ParamsToDlg()
{
	if( g_endapp == 1 ){
		return 0;
	}

	char tempchar[1024];		

	if( m_timerworking != 1 ){
		return 0;
	}

	if( !IsWindow() )
		return 0;

	if( m_dlg->m_motionnum <= 0 ){
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


	if( m_base && m_target && m_boneelem ){
		if( g_endapp == 1 ){
			return 0;
		}

		int ret;
		ret = GetValue( m_base, m_target, m_boneelem, &m_value );
		_ASSERT( !ret );
		sprintf_s( tempchar, 1024, "%f", m_value );
		if( (g_endapp == 1) || !m_edit_val_wnd.IsWindow() )
			return 0;
		m_edit_val_wnd.SetWindowText( tempchar );


		int ival;
		ival = (int)( m_value * 100.0f );
		if( (g_endapp == 1) || !m_sl_val_wnd.IsWindow() )
			return 0;
		m_sl_val_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)ival );
	}

	return 0;
}

LRESULT CMorphKeyDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}else if( ctrlwnd == m_sl_val_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_val( 0, 0, dummy );
	}

	return 0;
}


LRESULT CMorphKeyDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
LRESULT CMorphKeyDlg::OnReleasedcaptureSl_val(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( !m_base || !m_target || !m_boneelem ){
		//_ASSERT( 0 );
		return 0;
	}

	int ret;
	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( motinfo ){

		int curpos;
		curpos = (int)m_sl_val_wnd.SendMessage( TBM_GETPOS, 0, 0 );
		float value;
		value = (float)curpos / 100.0f;

		ret = motinfo->SetMMotValue( m_dlg->m_mot_cookie, m_dlg->m_current, m_base, m_target, value );
		if( ret ){
			DbgOut( "morphkeydlg : OnReleasedcaptureSl_val : mi SetMMotValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		InitList();
		ParamsToDlg();
		Redraw();

	}else{
		_ASSERT( 0 );
	}

	return 0;
}


LRESULT CMorphKeyDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
void CMorphKeyDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
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


int CMorphKeyDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_dlg->m_endframe - m_dlg->m_startframe );
	*posptr = MP_SIZE + (int)(step * (float)( frameno - m_dlg->m_startframe ) ) + clirect.left;
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	
	return 0;
}

int CMorphKeyDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
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

int CMorphKeyDlg::DrawMMotKeys()
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
	if( !m_base || !m_target || !m_boneelem ){
		return 0;
	}

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	CMMotKey* mmkptr = motinfo->GetFirstMMotKey( m_dlg->m_mot_cookie );
	CMMotKey* curmmk = mmkptr;
	while( curmmk ){
		CMMotElem* curmme;
		curmme = curmmk->ExistBase( m_base );
		if( curmme ){
			int frameno = curmmk->frameno;
			if( (frameno >= m_dlg->m_startframe) && (frameno <= m_dlg->m_endframe) ){
				if( frameno != m_dlg->m_current ){
					SelectObject( hdc, m_black_pen );
					SelectObject( hdc, m_black_brush );
				}else{
					SelectObject( hdc, m_red_pen );
					SelectObject( hdc, m_red_brush );
				}
				int posx;
				Frameno2Windowpos( clirect, frameno, &posx );
				int posy = (clirect.bottom - clirect.top) / 2;
				::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );
			}
		}
		curmmk = curmmk->next;
	}
	
	if( m_dragmme ){
		int frameno = m_dropframe;
		
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

int CMorphKeyDlg::Redraw()
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}

LRESULT CMorphKeyDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	
	DrawMMotKeys();

	return 0;
}

LRESULT CMorphKeyDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CMorphKeyDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "MorphKeyDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CMorphKeyDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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



LRESULT CMorphKeyDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_base || !m_target || !m_boneelem ){
		return 0;
	}

	int ret;

	ret = ::MessageBox( m_hWnd, "指定しているベースに関するモーフの全てのキーフレームを削除します。\nよろしいですか？", "確認ダイアログ", MB_OKCANCEL ); 
	if( ret != IDOK )
		return 0;

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	CMMotKey* mmkptr;
	mmkptr = motinfo->GetFirstMMotKey( m_dlg->m_mot_cookie );
	CMMotKey* curmmk = mmkptr;
	CMMotKey* nextmmk = 0;

	while( curmmk ){
		nextmmk = curmmk->next;		
		ret = curmmk->DeleteBase( m_base );
		_ASSERT( !ret );

		if( curmmk->mmenum == 0 ){
			//MMotElemが０個になったらキーを削除
			ret = motinfo->DeleteMMotKey( m_dlg->m_mot_cookie, curmmk->frameno );
			_ASSERT( !ret );
		}

		curmmk = nextmmk;
	}
	
	if( m_dragmme ){
		delete m_dragmme;
		m_dragmme = 0;
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


LRESULT CMorphKeyDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

	//Sleep( 100 );

	InitList();
	ParamsToDlg();
	return 0;
}


LRESULT CMorphKeyDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );


	BOOL dummyb;
	return m_dlg->OnPreview( 0, 0, 0, dummyb );

}

int CMorphKeyDlg::OnUserSelchange()
{
	int ret;
	m_remakelistflag = 1;


	int combono;
	combono = (int)m_combo1_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		m_base = 0;
		m_boneelem = 0;
	}else{
		if( m_ppselem ){
			CShdElem* melem;
			melem = *( m_ppselem + combono );
			CMorph* morph;
			morph = melem->morph;
			_ASSERT( morph );
			m_base = morph->m_baseelem;
			m_boneelem = morph->m_boneelem;
		}else{
			m_base = 0;
			m_boneelem = 0;
		}
	}


	ret = InitList();
	_ASSERT( !ret );
	//SetCapture();

	ParamsToDlg();
	Redraw();

	return 0;
}

int CMorphKeyDlg::CaptureM()
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


LRESULT CMorphKeyDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


	if( !m_base || !m_target || !m_boneelem )
		return 0;


	if( (m_dragflag == 1) && m_dragmme ){
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

		m_dropframe = frameno;

		Redraw();
	}


	return 0;
}


LRESULT CMorphKeyDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	CMMotKey* curmmk = 0;
	ret = motinfo->ExistMMotKey( m_dlg->m_mot_cookie, m_dlg->m_current, &curmmk );
	_ASSERT( !ret );

	CMMotElem* curmme = 0;
	if( curmmk ){
		curmme = curmmk->ExistBase( m_base );
	}

	if( !curmme ){
		::MessageBox( m_hWnd, "このフレームにはコピーできるキーがありません。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!
	}

	if( m_cpmme ){
		delete m_cpmme;
		m_cpmme = 0;
	}

	m_cpmme = new CMMotElem( m_base );
	if( !m_cpmme ){
		DbgOut( "MorphKeyDlg : OnFrameCopy : cpmme alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_cpmme->CopyTarget( curmme );
	if( ret ){
		DbgOut( "MorphKeyDlg : OnFrameCopy : cpmme CopyTarget error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CMorphKeyDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	if( !m_cpmme ){
		::MessageBox( m_hWnd, "フレームコピーを行ってから再試行してください。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!!!!
	}

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	CMMotKey* curmmk = 0;
	ret = motinfo->ExistMMotKey( m_dlg->m_mot_cookie, m_dlg->m_current, &curmmk );
	if( ret ){
		DbgOut( "morphkeydlg : OnFramePaste : mi ExistMMotKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !curmmk ){
		CMMotKey* addmmk;
		addmmk = new CMMotKey();
		if( !addmmk ){
			DbgOut( "MorphKeyDlg : OnFramePaste : addmmk alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = addmmk->CopyMMotElem( m_cpmme );
		if( ret ){
			DbgOut( "MorphKeyDlg : OnFramePaste : addmmk CopyMMotElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		addmmk->frameno = m_dlg->m_current;

		ret = motinfo->ChainMMotKey( m_dlg->m_mot_cookie, addmmk );
		if( ret ){
			DbgOut( "MorphKeyDlg : OnFramePaste : mi ChainMMotKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{

		ret = curmmk->CopyMMotElem( m_cpmme );
		if( ret ){
			DbgOut( "MorphKeyDlg : OnFramePaste : curmmk CopyMMotElem 1 error !!!\n" );
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

LRESULT CMorphKeyDlg::OnFrameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	CMMotKey* curmmk;
	ret = motinfo->ExistMMotKey( m_dlg->m_mot_cookie, m_dlg->m_current, &curmmk );
	if( ret ){
		DbgOut( "morphkeydlg : OnFrameDel : mi ExistMMotKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !curmmk ){
		return 0;
	}

	CMMotElem* curmme;
	curmme = curmmk->ExistBase( m_base );
	if( !curmme ){
		return 0;
	}

	ret = curmmk->DeleteBase( m_base );
	if( ret ){
		DbgOut( "morphkeydlg : OnFrameDel : mmk DeleteBase error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( curmmk->mmenum == 0 ){
		ret = motinfo->DeleteMMotKey( m_dlg->m_mot_cookie, m_dlg->m_current );
		if( ret ){
			DbgOut( "morphkeydlg : OnFrameDel : mi DeleteMMotKey error !!!\n" );
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


LRESULT CMorphKeyDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		if( !m_base || !m_target || !m_boneelem )
			return 0;
		int ret;
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

			CMotionInfo* motinfo;
			motinfo = m_boneelem->motinfo;
			if( !motinfo ){
				_ASSERT( 0 );
				return 0;
			}

			CMMotKey* curmmk = 0;
			ret = motinfo->ExistMMotKey( m_dlg->m_mot_cookie, frameno, &curmmk );
			_ASSERT( !ret );
			if( curmmk ){
				CMMotElem* curmme = 0;
				curmme = curmmk->ExistBase( m_base );
				if( curmme ){
					m_dragmme = new CMMotElem( m_base );
					if( !m_dragmme ){
						DbgOut( "morphkeydlg : OnLButtonDown : dragmme alloc error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					ret = m_dragmme->CopyTarget( curmme );
					if( ret ){
						DbgOut( "morphkeydlg : OnLButtonDown : mme CopyTarget error !!!\n" );
						_ASSERT( 0 );
						delete m_dragmme;
						m_dragmme = 0;
						return 1;
					}

					ret = curmmk->DeleteBase( m_base );
					if( ret ){
						DbgOut( "morphkeydlg : OnLButtonDown : mmk DeleteBase error !!!\n" );
						_ASSERT( 0 );
						delete m_dragmme;
						m_dragmme = 0;
						return 1;
					}

					m_dragframe = frameno;
					m_dragflag = 1;
					m_dropframe = frameno;
				}else{
					m_dragframe = 0;
					m_dragflag = 0;
					m_dragmme = 0;
					m_dropframe = 0;
				}
			}else{			
				m_dragframe = 0;
				m_dragflag = 0;
				m_dragmme = 0;
				m_dropframe = 0;
			}
		}else{
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragmme = 0;
			m_dropframe = 0;
		}
	}
	return 0;
}


LRESULT CMorphKeyDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	if( m_ctrldrag || m_altdrag ){
		m_ctrldrag = 0;
		m_altdrag = 0;
		m_dragx = 0;
	}else{

		if( !m_base || !m_target || !m_boneelem )
			return 0;

		CMotionInfo* motinfo;
		motinfo = m_boneelem->motinfo;
		if( !motinfo ){
			_ASSERT( 0 );
			return 0;
		}

		if( (m_dragflag == 1) && m_dragmme ){
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

	//DbgOut( "MorphKeyDlg : OnLButtonUp : Windowpos2Frameno : frameno %d\n", frameno );

			CMMotKey* curmmk = 0;
			ret = motinfo->ExistMMotKey( m_dlg->m_mot_cookie, frameno, &curmmk );
			if( ret ){
				DbgOut( "morphkeydlg : OnLButtonUp : mi ExistMMotKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( !curmmk ){
				ret = motinfo->CreateMMotKey( m_dlg->m_mot_cookie, frameno, &curmmk );
				if( ret || !curmmk ){
					DbgOut( "morphkeydlg : OnLButtonUp : mi CreateMMotKey error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}

			CMMotElem* curmme = 0;
			curmme = curmmk->ExistBase( m_base );

			if( curmme ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "フレーム番号%d には既にモーションポイントが存在するので、移動できません。",
					frameno );
				MessageBox( messtr, "移動できません", MB_OK );
				frameno = m_dragframe;//!!!!! 元に戻す
			}
			m_dropframe = frameno;


			CMMotKey* dropmmk = 0;
			ret = motinfo->ExistMMotKey( m_dlg->m_mot_cookie, m_dropframe, &dropmmk );
			if( ret ){
				DbgOut( "morphkeydlg : OnLButtonUp : mi ExistMMotKey dropmmk error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			if( dropmmk ){
				ret = dropmmk->CopyMMotElem( m_dragmme );
				if( ret ){
					DbgOut( "morphkeydlg : OnLButtonUp : mmk CopyMMotElem error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			
			delete m_dragmme;
			m_dragmme = 0;


			m_remakelistflag = 1;
			ret = InitList();
			_ASSERT( !ret );
			//SetCapture();

			ParamsToDlg();
			Redraw();

			//DbgOut( "MorphKeyDlg : OnLButtonUp : drag end, posx %d, frameno %d, m_dragframe %d\n",
			//	posx, frameno, m_dragframe );
		}

		//if( frameno == m_dragframe ){
		//	DbgOut( "MorphKeyDlg : OnLButtonUp : motion point already exist warning %d, %d !!!\n", frameno, m_dragframe );
		//}
	}

	m_dragflag = 0;
	m_dragframe = 0;

	busyflag = 0;

	return 0;
}
LRESULT CALLBACK MPProcM(
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
		//	DbgOut( "MorphKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "MorphKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}


		return 0;
	}
}
LRESULT CALLBACK ValProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  ){
	if( (uMsg != WM_KEYDOWN) && (uMsg != WM_KEYUP) ){
		return CallWindowProc(
		  (WNDPROC)s_mpfunc,  // pointer to previous procedure
		  s_sl_val_wnd,              // handle to window
		  uMsg,               // message
		  wParam,          // first message parameter
		  lParam           // second message parameter
		);		
	}else{
		//key は無効にする。

		//if( uMsg == WM_KEYDOWN ){
		//	DbgOut( "MorphKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "MorphKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
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
int CMorphKeyDlg::SetNewWindowProc()
{
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWL_WNDPROC );
	if( !s_mpfunc ){
		_ASSERT( 0 );
		return 1;
	}

	s_valfunc = ::GetWindowLong( m_sl_val_wnd, GWL_WNDPROC );
	if( !s_valfunc ){
		_ASSERT( 0 );
		return 1;
	}


//	s_listfunc = ::GetWindowLong( m_list_wnd, GWL_WNDPROC );
//	if( !s_listfunc ){
//		_ASSERT( 0 );
//		return 1;
//	}


	LONG lret;
	lret = ::SetWindowLong( m_sl_mp_wnd, GWL_WNDPROC, (LONG)MPProcM );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}

	lret = ::SetWindowLong( m_sl_val_wnd, GWL_WNDPROC, (LONG)ValProc );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}

//	lret = ::SetWindowLong( m_list_wnd, GWL_WNDPROC, (LONG)ListProc );
//	if( lret == 0 ){
//		_ASSERT( 0 );
//		return 1;
//	}


	return 0;
}


LRESULT CMorphKeyDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		
	bHandled = TRUE;
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;

	int timerid = (int)wParam;
	if( (timerid == MorphKeyDlgTIMER) && (m_cmdshow == SW_SHOW) ){

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


int CMorphKeyDlg::ShowDlg( int cmdshow, int stopflag )
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

	ResetDlg();


	return 0;
}

int CMorphKeyDlg::ResetDlg()
{
	if( m_cmdshow != SW_HIDE ){
		int ret;
		ret = SetCombo1();
		_ASSERT( !ret );


		BOOL dummyb;
		OnStop( 0, 9999, 0, dummyb );
		//m_dlg->m_current = 0;//!!!!!!!!!!!!!!!!!!!!!! 別スレッドでのセットが遅れるため、ここで強制リセット。
		OnUserSelchange();
	}
	return 0;
}



LRESULT CMorphKeyDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	CMMotKey* mmkptr;

	mmkptr = motinfo->GetFirstMMotKey( m_dlg->m_mot_cookie );
	if( !mmkptr )
		return 0;

	CMMotKey* curmmk;
	CMMotKey* befmmk;
	curmmk = mmkptr;
	befmmk = mmkptr;

	while( curmmk && (curmmk->frameno < m_dlg->m_current) ){
		CMMotElem* curmme;
		curmme = curmmk->ExistBase( m_base );
		if( curmme ){
			befmmk = curmmk;
		}
		curmmk = curmmk->next;
	}

	if( befmmk ){
		m_dlg->m_current = befmmk->frameno;
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
LRESULT CMorphKeyDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}


	CMMotKey* mmkptr;
	mmkptr = motinfo->GetFirstMMotKey( m_dlg->m_mot_cookie );
	if( !mmkptr ){
		return 0;
	}

	CMMotKey* curmmk;
	CMMotKey* findmmk;

	curmmk = mmkptr;
	findmmk = 0;
	while( curmmk ){
		CMMotElem* curmme;
		curmme = curmmk->ExistBase( m_base );
		if( curmme ){
			if( curmmk->frameno > m_dlg->m_current ){
				if( (curmmk->frameno >= m_dlg->m_startframe) && (curmmk->frameno <= m_dlg->m_endframe) ){
					findmmk = curmmk;
					break;
				}
			}
		}

		curmmk = curmmk->next;
	}

	if( findmmk ){
		m_dlg->m_current = findmmk->frameno;
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

LRESULT CMorphKeyDlg::OnCreateKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	int ret;

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	CMMotElem* bonemme;
	bonemme = m_boneelem->GetCurMMotElem( m_base );
	if( !bonemme ){
		_ASSERT( 0 );
		return 0;
	}

	CMMotKey* curmmk = 0;
	ret = motinfo->ExistMMotKey( m_dlg->m_mot_cookie, m_dlg->m_current, &curmmk );
	if( ret ){
		DbgOut( "morphkeydlg : OnCreateKey : mi ExistMMotKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( !curmmk ){
		ret = motinfo->CreateMMotKey( m_dlg->m_mot_cookie, m_dlg->m_current, &curmmk );
		if( ret ){
			DbgOut( "morphkeydlg : OnCreateKey : mi CreateMMotKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}



	CMMotElem* newmme;
	newmme = new CMMotElem( m_base );
	if( !newmme ){
		DbgOut( "morphkeydlg : OnCreateKey : newmme alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int tno;
	for( tno = 0; tno < bonemme->m_primnum; tno++ ){
		MPRIM* bonemp;
		bonemp = bonemme->m_primarray + tno;
		CShdElem* curtarget;
		curtarget = bonemp->selem;
		_ASSERT( curtarget );

		float curvalue = 0.0f;
		ret = GetValue( m_base, curtarget, m_boneelem, &curvalue );
		if( ret ){
			DbgOut( "morphkeydlg : OnCreateKey : GetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = newmme->SetValue( curtarget, curvalue );
		if( ret ){
			DbgOut( "morphkeydlg : OnCreateKey : newmme SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	ret = curmmk->CopyMMotElem( newmme );
	if( ret ){
		DbgOut( "morphkeydlg : OnCreateKey : curmmk CopyMMotElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	delete newmme;

	Redraw();

	return 0;
}


int CMorphKeyDlg::InitList()
{
	int ret;
	ListView_DeleteAllItems( m_list_wnd );
	
//	m_target = 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if( m_createlistflag == 0 ){
		LV_COLUMN lvcol;
		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 165;
		lvcol.pszText = "ターゲット名";
		lvcol.iSubItem = TEXSUB_MATNAME;
		ListView_InsertColumn(m_list_wnd, TEXSUB_MATNAME, &lvcol);

		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 150;
		lvcol.pszText = "ブレンド率";
		lvcol.iSubItem = TEXSUB_TEXNAME;
		ListView_InsertColumn(m_list_wnd, TEXSUB_TEXNAME, &lvcol);
	}
	m_createlistflag = 1;



	if( m_base && m_boneelem && (m_dlg->m_selectedno > 0) && m_dlg->m_shandler && m_dlg->m_mhandler ){

		CMotionInfo* motinfo;
		motinfo = m_boneelem->motinfo;
		if( !motinfo ){
			_ASSERT( 0 );
			return 1;
		}

		CMMotElem* bonemme;
		bonemme = m_boneelem->GetCurMMotElem( m_base );
		if( bonemme ){
			int tno;
			for( tno = 0; tno < bonemme->m_primnum; tno++ ){
				MPRIM* bonemp;
				bonemp = bonemme->m_primarray + tno;
				CShdElem* addtarget;
				addtarget = bonemp->selem;

				ret = AddItem2List( tno, motinfo, addtarget );
				if( ret ){
					DbgOut( "morphkeydlg : InitList : AddItem2List error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
		}

		if( m_target ){
			if( bonemme ){
				int tarindex;
				tarindex = bonemme->FindPrim( m_target );
				if( tarindex >= 0 ){
					ListView_SetHotItem( m_list_wnd.m_hWnd, tarindex );
					//m_list_wnd.SendMessage( LVM_SETSELECTEDCOLUMN, tarindex, 0 );
				}else{
					m_target = 0;
				}
			}else{
				m_target = 0;
			}
		}

		if( !m_target ){
			if( bonemme ){
				if( bonemme->m_primnum > 0 ){
					ListView_SetHotItem( m_list_wnd.m_hWnd, 0 );
					//m_list_wnd.SendMessage( LVM_SETSELECTEDCOLUMN, 0, 0 );
					m_target = bonemme->m_primarray->selem;
				}
			}
		}


	}


	ret = m_dlg->SetCurrentMotion();
	_ASSERT( !ret );

	return 0;
}
int CMorphKeyDlg::AddItem2List( int itemno, CMotionInfo* motinfo, CShdElem* srctarget )
{
	char tempchar[256];

	CTreeElem2* srctelem;
	srctelem = (*m_dlg->m_thandler)( srctarget->serialno );
	_ASSERT( srctelem );


	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));

	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.pszText = srctelem->name;
	item.iItem = itemno;
	item.iSubItem = MSUB_TARGET;
	item.lParam = srctarget->serialno;
	ListView_InsertItem(m_list_wnd, &item);

	int ret;
	float curvalue = 0.0f;
	int existflag = 0;
	ret = motinfo->GetMMotAnim( &curvalue, m_dlg->m_mot_cookie, m_dlg->m_current, m_base, srctarget, &existflag );
	if( ret ){
		DbgOut( "morphkeydlg : AddItem2List : mi GetMMotAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( existflag == 0 ){
		sprintf_s( tempchar, 256, "%f", curvalue );
	}else{
		sprintf_s( tempchar, 256, "*%f", curvalue );
	}

item.mask = LVIF_TEXT;
	item.pszText = tempchar;
	item.iItem = itemno;
	item.iSubItem = MSUB_VALUE;
//	item.lParam = telem->serialno;
	ListView_SetItem(m_list_wnd, &item);


	return 0;
}


int CMorphKeyDlg::OnList1( int hititem, POINT inview )
{

	if( !m_dlg->m_mhandler || (m_dlg->m_selectedno <= 0) || (m_dlg->m_mot_cookie < 0) ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	if( !m_base || !m_boneelem ){
		return 0;
	}

	int ret;

	LVITEM lvitem2;
	ZeroMemory( &lvitem2, sizeof( LVITEM ) );
	char tempname2[256];
	ZeroMemory( tempname2, 256 );

	lvitem2.iItem = hititem;
	lvitem2.iSubItem = MSUB_TARGET;//!!!!!!!!!!!!
	lvitem2.pszText = tempname2;
	lvitem2.cchTextMax = 256;

	lvitem2.mask = LVIF_TEXT | LVIF_PARAM;
				
	BOOL bret2;
	bret2 = ListView_GetItem( m_list_wnd, &lvitem2 );
	if( bret2 == TRUE ){
		int tarseri;
		tarseri = (int)lvitem2.lParam;
		m_target = (*m_dlg->m_shandler)( tarseri );

		CMMotElem* bonemme;
		bonemme = m_boneelem->GetCurMMotElem( m_base );
		if( bonemme ){
			int tarindex;
			tarindex = bonemme->FindPrim( m_target );
			if( tarindex >= 0 ){
				ListView_SetHotItem( m_list_wnd.m_hWnd, tarindex );
			}else{
				m_target = 0;
			}
		}else{
			m_target = 0;
		}

		ParamsToDlg();

//		m_remakelistflag = 1;
//		InitList();
		
		ret = m_dlg->SetCurrentMotion();
		_ASSERT( !ret );

	}

	Redraw();

	return 0;
}

LRESULT CMorphKeyDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CMorphKeyDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}

LRESULT CMorphKeyDlg::OnSelchangeList1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NMLISTVIEW* nmlistview = (NMLISTVIEW*)pnmh;

//	char testmes[256];
//	sprintf_s( testmes, 256, "flag %d, item %d, uNewState %d, uOldState %d, uChanged %d", 
//		m_remakelistflag, nmlistview->iItem, nmlistview->uNewState, nmlistview->uOldState, nmlistview->uChanged );
//	::MessageBox( m_hWnd, testmes, "通知", MB_OK );

	int ret;

//	if( (m_remakelistflag == 0) && (nmlistview->uNewState == 3) ){

//		char testmes[256];
//		sprintf_s( testmes, 256, "item %d, subitem %d, uNewState %d, uOldState %d, uChanged %d", 
//			nmlistview->iItem, nmlistview->iSubItem, nmlistview->uNewState, nmlistview->uOldState, nmlistview->uChanged );
//		::MessageBox( m_hWnd, testmes, "通知", MB_OK );


		ret = OnList1( nmlistview->iItem, nmlistview->ptAction );
		_ASSERT( !ret );

//	}else if( nmlistview->uNewState == 3 ){
//		m_remakelistflag = 0;
//	}

	return 0;
}

int CMorphKeyDlg::SetCombo1()
{
	int ret;

	m_combo1_wnd.SendMessageA( CB_RESETCONTENT, 0, 0 );

	if( m_ppselem ){
		free( m_ppselem );
		m_ppselem = 0;
		m_base = 0;
		m_boneelem = 0;
	}

	if( !m_dlg->m_shandler ){
		return 0;//!!!!!!!!!!!!!!!!		
	}

	int mnum = 0;
	ret = m_dlg->m_shandler->GetMorphElem( 0, &mnum, 0 );
	if( ret ){
		DbgOut( "morphkeydlg : SetCombo1 : sh GetMorphElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	if( mnum > 0 ){
		m_ppselem = (CShdElem**)malloc( sizeof( CShdElem* ) * mnum );
		if( !m_ppselem ){
			DbgOut( "morphkeydlg : SetCombo1 : ppselem alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( m_ppselem, sizeof( CShdElem* ) * mnum );

		int chknum = 0;
		ret = m_dlg->m_shandler->GetMorphElem( m_ppselem, &chknum, mnum );
		if( ret ){
			DbgOut( "morphkeydlg : SetCombo1 : sh GetMorphElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( mnum != chknum ){
			DbgOut( "morphkeydlg : SetCombo1 : chknum error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int mno;
		for( mno = 0; mno < mnum; mno++ ){
			CShdElem* mselem;
			CTreeElem2* mtelem;
			mselem = *( m_ppselem + mno );
			_ASSERT( mselem );
			mtelem = (*m_dlg->m_thandler)( mselem->serialno );
			_ASSERT( mtelem );

			m_combo1_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)mtelem->name );
		}
		m_combo1_wnd.SendMessage( CB_SETCURSEL, 0, 0 );

		CMorph* morph;
		morph = (*m_ppselem)->morph;
		_ASSERT( morph );
		m_base = morph->m_baseelem;
		m_boneelem = morph->m_boneelem;

	}else{
		m_base = 0;
		m_boneelem = 0;
	}

	return 0;
}

LRESULT CMorphKeyDlg::OnSelchangeCombo1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{


	OnUserSelchange();


	return 0;
}

int CMorphKeyDlg::GetValue( CShdElem* srcbase, CShdElem* srctarget, CMotionCtrl* srcbone, float* valptr )
{
	if( !srcbase || !srctarget || !srcbone ){
		*valptr = 0.0f;
		return 0;
	}

	int ret;
	CMotionInfo* motinfo;
	motinfo = srcbone->motinfo;
	if( !motinfo ){
		*valptr = 0;
		return 0;
	}

	int existflag = 0;
	ret = motinfo->GetMMotAnim( valptr, m_dlg->m_mot_cookie, m_dlg->m_current, srcbase, srctarget, &existflag );
	_ASSERT( !ret );

	return 0;
}

LRESULT CMorphKeyDlg::OnApplyVal(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_base || !m_target || !m_boneelem )
		return 0;


	float tmpval = 0.0f;
	int ret;
	ret = GetFloat( m_edit_val_wnd, &tmpval );
	if( ret || (tmpval < 0.0f) || (tmpval > 1.0f) ){
		::MessageBox( m_hWnd, "ブレンド率の数値には0.0から1.0の値を入れてください。", "入力エラー", MB_OK );
		ParamsToDlg();
		return 0;
	}

	m_value = tmpval;

	int ival;
	ival = (int)( m_value * 100.0f );
	m_sl_val_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)ival );

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}
	ret = motinfo->SetMMotValue( m_dlg->m_mot_cookie, m_dlg->m_current, m_base, m_target, m_value );
	if( ret ){
		DbgOut( "morphkeydlg : OnApplyVal : mi SetMMotValue error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	InitList();
	ParamsToDlg();
	Redraw();


	return 0;
}
LRESULT CMorphKeyDlg::OnZero(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	int ret;
	m_value = 0.0f;

	int ival = 0;
	m_sl_val_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)ival );

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}
	ret = motinfo->SetMMotValue( m_dlg->m_mot_cookie, m_dlg->m_current, m_base, m_target, m_value );
	if( ret ){
		DbgOut( "morphkeydlg : OnZero : mi SetMMotValue error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	InitList();
	ParamsToDlg();
	Redraw();


	return 0;
}
LRESULT CMorphKeyDlg::OnOne(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_base || !m_target || !m_boneelem )
		return 0;

	int ret;
	m_value = 1.0f;

	int ival = 100;
	m_sl_val_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)ival );

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}
	ret = motinfo->SetMMotValue( m_dlg->m_mot_cookie, m_dlg->m_current, m_base, m_target, m_value );
	if( ret ){
		DbgOut( "morphkeydlg : OnOne : mi SetMMotValue error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	InitList();
	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CMorphKeyDlg::OnAllZero(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_base || !m_target || !m_boneelem )
		return 0;

	int ret;
	m_value = 0.0f;

	int ival = 0;
	m_sl_val_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)ival );

	CMMotElem* bonemme;
	bonemme = m_boneelem->GetCurMMotElem( m_base );
	if( !bonemme ){
		return 0;
	}

	CMotionInfo* motinfo;
	motinfo = m_boneelem->motinfo;
	if( !motinfo ){
		_ASSERT( 0 );
		return 0;
	}

	int tno;
	for( tno = 0; tno < bonemme->m_primnum; tno++ ){
		MPRIM* bonemp;
		bonemp = bonemme->m_primarray + tno;
		CShdElem* curtarget;
		curtarget = bonemp->selem;
		if( curtarget ){
			ret = motinfo->SetMMotValue( m_dlg->m_mot_cookie, m_dlg->m_current, m_base, curtarget, m_value );
			if( ret ){
				DbgOut( "morphkeydlg : OnAllZero : mi SetMMotValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	InitList();
	ParamsToDlg();
	Redraw();

	return 0;
}

int CMorphKeyDlg::OnJumpFrame( int srcframe )
{
	m_dlg->OnJumpFrame( srcframe );

	InitList();
	ParamsToDlg();
	Redraw();

	return 0;
}
