// GPKeyDlg.cpp : CGPKeyDlg のインプリメンテーション
#include "stdafx.h"
#include "GPKeyDlg.h"

#include <math.h>
#include <stdio.h>
//#include <D3DX9.h>

#include <coef.h>
#define DBGH
#include <dbg.h>

#include <shdhandler.h>
#include <shdelem.h>
#include <mothandler.h>
#include <motionctrl.h>
#include <motioninfo.h>

#include <GPAnimHandler.h>
#include <GPAnim.h>
#include <GPKey.h>
#include <GPFile.h>

#include <BoneProp.h>

#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "viewer.h"
#include "MotDlg.h"
#include "MotParamDlg.h"
#include "MotSettingDlg.h"
#include "GraphRotDlg.h"

#include "GetDlgParams.h"
#include "SyncDlg.h"
#include "inidata.h"

#include <crtdbg.h>


//MotionPoint描画時の点の半径
#define MP_SIZE	3

enum {
	GPPLANE_XZ,
	GPPLANE_XY,
	GPPLANE_YZ,
	GPPLANE_MAX
};

/////////////////
// sttic
static LONG s_mpfunc = 0;
static HWND s_sl_mp_wnd = 0;
static HWND s_dlg_wnd = 0;
static HWND s_apphwnd = 0;

static char s_strgraph[ GPG_MAX ][ 10 ] = {
	"角度",
	"位置"
};
static char s_strplane[ GPPLANE_MAX ][ 15 ] = {
	"XZ平面",
	"XY平面",
	"YZ平面"
};
static char s_strong[ GROUND_MAX ][ 20 ] = {
	"接地なし",
	"瞬間接地",
	"キー間接地"
};

/////////////////
//extern HINSTANCE g_resinstance;
extern CMotDlg* g_motdlg;
extern unsigned char g_KeyBuffer[256];
extern int g_endapp;
extern CIniData* g_inidata;

/////////////////////////////////////////////////////////////////////////////
// CGPKeyDlg

CGPKeyDlg::CGPKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp )
//: m_list_wnd( NULL, this, 2 )
{
	InitParams();

	m_dlg = srcmpdlg;
	m_dlg->m_gpkeydlg = this;
	m_app = srcapp;
}

int CGPKeyDlg::InitParams()
{
	m_gpmode = GP_ANIM;

	m_gpah = 0;
	m_dlg = 0;
	m_app = 0;

	m_current = 0;
	m_ismoving = 0;

	m_sl_mp_rangemax = 59;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_previewframe = 0;

	m_draggpk = 0;
	m_dragframe = 0;
	m_dragflag = 0;
	m_cpgpk = 0;

	m_animno = 0;
	m_gpe.ongmode = GROUND_NONE;
	m_gpe.pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_gpe.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_gpe.rayy = 5000.0f;
	m_gpe.rayleng = 8000.0f;
	m_gpe.offsety = 10.0f;

	m_hengpe = m_gpe;

	int motno;
	for( motno = 0; motno < MAXMOTIONNUM; motno++ ){
		m_motid[motno].id = -1;
		ZeroMemory( m_motid[motno].filename, sizeof( char ) * 256 );
		m_motid[motno].ev0idle = 0;
		m_motid[motno].commonid = 0;
		m_motid[motno].forbidnum = 0;
		m_motid[motno].forbidid = 0;
		m_motid[motno].notfu = 0;
		m_motid[motno].startframe = 0;
		m_motid[motno].endframe = 0;
	}

	m_black_pen = NULL;
	m_red_pen = NULL;
	m_blue_pen = NULL;
	m_black_brush = NULL;
	m_white_brush = NULL;
	m_red_brush = NULL;
	m_blue_brush = NULL;


	m_timerworking = 0;
	m_cmdshow = SW_HIDE;
	m_mcap = 0;
	m_moving = 0;

	m_gtype = GPG_ROT;
	m_planetype = GPPLANE_XZ;

	m_center = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_mvdeg = 0.0f;
	m_rotstep = 5.0f;
	m_mvstep = 5.0f;

	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

	m_savebmdisp = g_inidata->opt_bmdisp;
	m_savejdisp = g_inidata->opt_jointdisp;

	return 0;
}


CGPKeyDlg::~CGPKeyDlg()
{
	DestroyObjs( 1 );
}

int CGPKeyDlg::DestroyObjs( int delgpah )
{
	if( m_cpgpk ){
		delete m_cpgpk;
		m_cpgpk = 0;
	}

	if( delgpah ){
		if( m_gpah ){
			delete m_gpah;
			m_gpah = 0;
		}
	}

	return 0;
}

int CGPKeyDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, GPKeyDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	return 0;
}

LRESULT CGPKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	if( m_gpah ){
		delete m_gpah;
		m_gpah = 0;
	}
	m_gpah = new CGPAnimHandler();
	_ASSERT( m_gpah );


	ret = SetWnd();
	_ASSERT( !ret );
	s_sl_mp_wnd = m_sl_mp_wnd;

	ret = InitCombo();
	_ASSERT( !ret );

	ret = SetNewWindowProc();
	_ASSERT( !ret );

	ret = ParamsToDlg();
	_ASSERT( !ret );

	int timerid;
	timerid = (int)::SetTimer( m_hWnd, GPKeyDlgTIMER, 500, NULL );
	_ASSERT( timerid == GPKeyDlgTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}


int CGPKeyDlg::InitDlg( int atshowflag )
{
	int ret;	
	DbgOut( "GPKeyDlg : InitDlg\n" );


	if( !m_gpah ){
		m_gpah = new CGPAnimHandler();
		_ASSERT( m_gpah );
	}

	if( (atshowflag == 1) && (m_gpah->m_animnum >= 1) && (m_animno >= 0) ){
		//ShowDlgから呼ばれた場合、すでにアニメがあれば作らない。
		return 0;
	}

	char animname[256];
	ZeroMemory( animname, 256 );
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	sprintf_s( animname, 256, "GPA_%d_%d_%d_%d_%d_%d_%d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
	);

	int animtype = MOTION_CLAMP;
	CGPAnim* newanim = 0;
	newanim = m_gpah->AddAnim( animname, animtype, m_sl_mp_rangemax + 1, 0 );
	if( !newanim ){
		DbgOut( "GPKeyDlg : InitDlg : cah AddAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_animno = newanim->m_motkind;

	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_current = m_startframe;


	if( m_gpah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "GPKeyDlg : InitDlg : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );


	m_motid[ m_gpah->m_animnum - 1 ].id = m_animno;
	m_motid[ m_gpah->m_animnum - 1 ].filename[0] = 0;
	m_motid[ m_gpah->m_animnum - 1 ].ev0idle = 0;
	m_motid[ m_gpah->m_animnum - 1 ].commonid = 0;
	m_motid[ m_gpah->m_animnum - 1 ].forbidnum = 0;
	if( m_motid[ m_gpah->m_animnum - 1 ].forbidid ){
		free( m_motid[ m_gpah->m_animnum - 1 ].forbidid );
	}
	m_motid[ m_gpah->m_animnum - 1 ].forbidid = 0;
	m_motid[ m_gpah->m_animnum - 1 ].notfu = 0;

	m_motid[ m_gpah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_gpah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_gpah->m_animnum - 1 ].current = m_startframe;

	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	InterlockedExchange( &m_previewframe, m_startframe );


	::SendMessage( m_dlg->m_apphwnd, WM_USER_GPMENU, m_gpah->m_animnum - 1, 0 );

	return 0;
}

LRESULT CGPKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "GPKeyDlg : OnOK\n" );
	ShowWindow( SW_HIDE );
	return 0;
}

LRESULT CGPKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowWindow( SW_HIDE );
	return 0;
}
LRESULT CGPKeyDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	return 0;
}


int CGPKeyDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	s_dlg_wnd = m_hWnd;

	m_tex_current_wnd = GetDlgItem( IDC_TEX_CURRENT );
	m_sl_mp_wnd = GetDlgItem( IDC_SL_MP );

	m_combog_wnd = GetDlgItem( IDC_COMBOG );
	m_comboplane_wnd = GetDlgItem( IDC_COMBOPLANE );
	m_comboong_wnd = GetDlgItem( IDC_COMBOONG );

	m_mp_wnd = GetDlgItem( IDC_MP );
	m_sp_mp_wnd = GetDlgItem( IDC_SP_MP );

	m_posx_wnd = GetDlgItem( IDC_PX );
	m_posy_wnd = GetDlgItem( IDC_PY );
	m_posz_wnd = GetDlgItem( IDC_PZ );

	m_rotx_wnd = GetDlgItem( IDC_ROTX );
	m_roty_wnd = GetDlgItem( IDC_ROTY );
	m_rotz_wnd = GetDlgItem( IDC_ROTZ );
	m_rotstep_wnd = GetDlgItem( IDC_RSTEP );

	m_rayy_wnd = GetDlgItem( IDC_RAYY );
	m_rayleng_wnd = GetDlgItem( IDC_RAYLENG );
	m_offsety_wnd = GetDlgItem( IDC_OFFSETY );

	m_cenx_wnd = GetDlgItem( IDC_CENX );
	m_ceny_wnd = GetDlgItem( IDC_CENY );
	m_cenz_wnd = GetDlgItem( IDC_CENZ );
	m_mvdeg_wnd = GetDlgItem( IDC_DEG );
	//m_mvstep_wnd = GetDlgItem( IDC_DEGSTEP );

	m_preview_wnd = GetDlgItem( IDC_PREVIEW );
	m_stop_wnd = GetDlgItem( IDC_STOP );

	m_drag_wnd = GetDlgItem( IDC_DRAG );

	//sliderの初期設定
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	return 0;
}

int CGPKeyDlg::InitCombo()
{
	m_comboplane_wnd.SendMessageA( CB_RESETCONTENT, 0, 0 );
	int pno;
	for( pno = 0; pno < GPPLANE_MAX; pno++ ){
		m_comboplane_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)(&(s_strplane[pno][0])) );
	}
	m_comboplane_wnd.SendMessageA( CB_SETCURSEL, m_planetype, 0 );

	m_combog_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );
	int gno;
	for( gno = 0; gno < GPG_MAX; gno++ ){
		m_combog_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)(&(s_strgraph[gno][0])) );
	}
	m_combog_wnd.SendMessage( CB_SETCURSEL, m_gtype, 0 );

	m_comboong_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );
	int ongno;
	for( ongno = 0; ongno < GROUND_MAX; ongno++ ){
		m_comboong_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)(&(s_strong[ongno][0])) );
	}
	m_comboong_wnd.SendMessage( CB_SETCURSEL, m_gpe.ongmode, 0 );


//CB_RESETCONTENT
//	m_combo_no_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
//	m_combo_no_wnd.SendMessage( CB_SETCURSEL, m_editswitchno - 1, 0 );
//	int combono;
//	combono = (int)m_combo_no_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
//	if( combono == CB_ERR )
//		return 1;
	
	return 0;
}


int CGPKeyDlg::ParamsToDlg()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 ){
		return 0;
	}

	char tempchar[1024];		


	if( !IsWindow() )
		return 0;

	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

	if( m_cmdshow != SW_SHOW )
		return 0;

//m_current
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "frameno: current %d, start %d, end %d, total %d", 
		m_current, m_startframe, m_endframe, m_sl_mp_rangemax + 1 );
	if( (g_endapp == 1) || !m_tex_current_wnd.IsWindow() )
		return 0;
	m_tex_current_wnd.SetWindowText( tempchar );


	if( m_dlg->m_preview_flag != 0 )
		return 0;


//m_sl_mp_
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() ){
		return 0;
	}
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() ){
		return 0;
	}
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	sprintf_s( tempchar, 1024, "%f", m_gpe.pos.x );
	if( (g_endapp == 1) || !m_posx_wnd.IsWindow() ){
		return 0;
	}
	m_posx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_gpe.pos.y );
	if( (g_endapp == 1) || !m_posy_wnd.IsWindow() ){
		return 0;
	}
	m_posy_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_gpe.pos.z );
	if( (g_endapp == 1) || !m_posz_wnd.IsWindow() ){
		return 0;
	}
	m_posz_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_gpe.rot.x );
	if( (g_endapp == 1) || !m_rotx_wnd.IsWindow() ){
		return 0;
	}
	m_rotx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_gpe.rot.y );
	if( (g_endapp == 1) || !m_roty_wnd.IsWindow() ){
		return 0;
	}
	m_roty_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_gpe.rot.z );
	if( (g_endapp == 1) || !m_rotz_wnd.IsWindow() ){
		return 0;
	}
	m_rotz_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_rotstep );
	if( (g_endapp == 1) || !m_rotstep_wnd.IsWindow() ){
		return 0;
	}
	m_rotstep_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_gpe.rayy );
	if( (g_endapp == 1) || !m_rayy_wnd.IsWindow() ){
		return 0;
	}
	m_rayy_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_gpe.rayleng );
	if( (g_endapp == 1) || !m_rayleng_wnd.IsWindow() ){
		return 0;
	}
	m_rayleng_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_gpe.offsety );
	if( (g_endapp == 1) || !m_offsety_wnd.IsWindow() ){
		return 0;
	}
	m_offsety_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", g_inidata->gpa_move );
	m_drag_wnd.SetWindowTextA( tempchar );


	sprintf_s( tempchar, 1024, "%f", m_center.x );
	if( (g_endapp == 1) || !m_cenx_wnd.IsWindow() ){
		return 0;
	}
	m_cenx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_center.y );
	if( (g_endapp == 1) || !m_ceny_wnd.IsWindow() ){
		return 0;
	}
	m_ceny_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_center.z );
	if( (g_endapp == 1) || !m_cenz_wnd.IsWindow() ){
		return 0;
	}
	m_cenz_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_mvdeg );
	if( (g_endapp == 1) || !m_mvdeg_wnd.IsWindow() ){
		return 0;
	}
	m_mvdeg_wnd.SetWindowText( tempchar );
//	sprintf_s( tempchar, 1024, "%f", m_mvstep );
//	m_mvstep_wnd.SetWindowText( tempchar );

	if( (g_endapp == 1) || !m_combog_wnd.IsWindow() ){
		return 0;
	}
	m_combog_wnd.SendMessage( CB_SETCURSEL, m_gtype, 0 );
	if( (g_endapp == 1) || !m_comboplane_wnd.IsWindow() ){
		return 0;
	}
	m_comboplane_wnd.SendMessage( CB_SETCURSEL, m_planetype, 0 );
	if( (g_endapp == 1) || !m_comboong_wnd.IsWindow() ){
		return 0;
	}
	m_comboong_wnd.SendMessage( CB_SETCURSEL, m_gpe.ongmode, 0 );


	if( m_gpmode == GP_HEN ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKHEN, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKHEN, BST_UNCHECKED );
	}

	if( (g_endapp == 0) && (m_dlg->m_timerworking == 1) && (m_dlg->m_grotdlg) && (m_dlg->m_preview_flag == 0) ){
		m_dlg->m_grotdlg->Redraw();
	}

	return 0;
}

LRESULT CGPKeyDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}

LRESULT CGPKeyDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
//		return 0;

	if( ::IsWindow( (HWND)m_sl_mp_wnd ) == 0 )
		return 0;

	if( !IsWindow() )
		return 0;

	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	int curpos;
	curpos = (int)m_sl_mp_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	m_current = curpos;


	if( m_gpmode != GP_HEN ){
		SetCurrentMotion( 0 );
	}else{
		m_gpe = m_hengpe;
	}
	
	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	Redraw();


	if( (idCtrl != 999) && (m_app->m_syncdlg->m_gpaflag != 0) ){
		int quaflag = m_app->m_syncdlg->m_quaflag;
		int camflag = m_app->m_syncdlg->m_camflag;
		int moeflag = m_app->m_syncdlg->m_moeflag;
		int sndflag = m_app->m_syncdlg->m_sndflag;

		g_motdlg->OnJumpFrame( m_current, quaflag, camflag, 0, moeflag, sndflag );
	}

	return 0;
}
LRESULT CGPKeyDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
		m_current = m_endframe;
	}else if( delta == -10000 ){
		//先頭フレーム
		m_current = m_startframe;
	}else if( delta > 0 ){
		int newframe;
		newframe = m_current + 1;
		if( newframe <= m_endframe ){
			m_current = newframe;							
		}
	}else if( delta < 0 ){
		int newframe;
		newframe = m_current - 1;
		if( newframe >= m_startframe ){
			m_current = newframe;
		}
	}

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	BOOL dummy;	
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	ParamsToDlg();
	Redraw();

	return 0;

}
void CGPKeyDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
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


int CGPKeyDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
	*posptr = MP_SIZE + (int)(step * (float)( frameno - m_startframe ) ) + clirect.left;
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	
	return 0;
}

int CGPKeyDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
	float tempfloat;
	tempfloat = (float)(posx - MP_SIZE - clirect.left) / step; 
	//四捨五入してframenoにする。
	int tempint;
	tempint = (int)( tempfloat + 0.49999f ) + m_startframe;

	if( tempint < m_startframe ){
		*framenoptr = m_startframe;
	}else if( tempint > m_endframe ){
		*framenoptr = m_endframe;
	}else{
		*framenoptr = tempint;
	}
	
	return 0;
}

int CGPKeyDlg::DrawGPKeys()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_cmdshow != SW_SHOW ){
		return 0;
	}
	if( m_timerworking != 1 ){
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
	int ceny;
	ceny = clirect.top + (clirect.bottom - clirect.top) / 2;
	SelectObject( hdc, m_black_brush );
	::MoveToEx( hdc, clirect.left, ceny, NULL ); 
	::LineTo( hdc, clirect.right, ceny );

//current bar
	int barposx;
	Frameno2Windowpos( clirect, m_current, &barposx );

	::MoveToEx( hdc, barposx, clirect.top, NULL );
	::LineTo( hdc, barposx, clirect.bottom );

//点
	if( !m_gpah ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	CGPKey* curgpk = anim->GetFirstGPKey();
	while( curgpk ){
		int frameno = curgpk->m_frameno;
		if( (frameno >= m_startframe) && (frameno <= m_endframe) ){
			if( frameno != m_current ){
				SelectObject( hdc, m_black_pen );
				SelectObject( hdc, m_black_brush );
			}else{
				SelectObject( hdc, m_red_pen );
				SelectObject( hdc, m_red_brush );
			}
			//int posx = MP_SIZE + (int)(step * (float)(frameno));
			int posx;
			Frameno2Windowpos( clirect, frameno, &posx );
			int posy = ceny;
			::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );
		}

		curgpk = curgpk->next;
	}
	
	if( m_draggpk ){
		int frameno = m_draggpk->m_frameno;
		
		SelectObject( hdc, m_blue_pen );
		SelectObject( hdc, m_blue_brush );
		
		//int posx = MP_SIZE + (int)(step * (float)(frameno));
		int posx;
		Frameno2Windowpos( clirect, frameno, &posx );
		int posy = ceny;
		::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );		
	}
	

	::SelectObject( hdc, hOldPen );
	::SelectObject( hdc, hOldBrush );
	m_mp_wnd.ReleaseDC( hdc );

	return 0;
}


int CGPKeyDlg::Redraw()
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
//		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}

LRESULT CGPKeyDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	if( !IsWindow() )
		return 0;
//	if( m_dlg->m_preview_flag == 1 ){
//		return 0;
//	}

//	/***
//	if( uMsg != 0 )
//		::DefWindowProc( m_hWnd, uMsg, wParam, lParam );
//	else
//		UpdateAllWnd();
//	***/
	
	DrawGPKeys();

	return 0;
}

LRESULT CGPKeyDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CGPKeyDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "GPKeyDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CGPKeyDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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




LRESULT CGPKeyDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 ){
		return 0;
	}

	BOOL dummyb;
//	if( wID == 9999 ){
		m_dlg->OnStop( 0, 9999, 0, dummyb );
//	}else{
//		m_dlg->OnStop( 0, 0, 0, dummyb );
//	}
	ParamsToDlg();
	return 0;
}


LRESULT CGPKeyDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );


	BOOL dummyb;
	return m_dlg->OnPreview( 0, 0, 0, dummyb );

}

int CGPKeyDlg::CaptureM()
{

	RECT viewclient;
	RECT vc1;
	/***
	m_mp_wnd.GetClientRect( &vc1 );
	m_sw_wnd.GetClientRect( &vc2 );
	viewclient.left = vc1.left;
	viewclient.right = vc1.right;
	viewclient.top = vc1.top;
	viewclient.bottom = vc1.bottom + vc2.bottom;
	//m_mp_wnd.GetClientRect( &viewclient );
	m_mp_wnd.ClientToScreen( &viewclient );
	***/
	m_mp_wnd.GetWindowRect( &vc1 );
	viewclient = vc1;


	POINT inview;
	::GetCursorPos( &inview );
	//m_mp_wnd.ScreenToClient( &inview );

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


LRESULT CGPKeyDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	CaptureM();

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "GPKeyDlg : OnMouseMove : MK_LBUTTON\n" );

	if( (m_dragflag == 1) && m_draggpk ){
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

		m_draggpk->m_frameno = frameno;

		Redraw();
	}

	CGPAnim* anim;
	int animindex;
	anim = m_gpah->GetGPAnim( m_animno, &animindex );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	if( m_ctrldrag ){
		int newx;
		newx = LOWORD(lParam);
		int dx;
		float divx;
		divx = 1200.0f / (float)m_sl_mp_rangemax;
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
		newend = m_endframe + changex;

		if( newend > m_sl_mp_rangemax ){
			newend = m_sl_mp_rangemax;
		}else if( newend <= m_startframe ){
			newend = m_startframe;
		}

		m_endframe = newend;
		if( (m_current < m_startframe) || (m_current > m_endframe) ){
			m_current = m_startframe;
		}

		m_motid[ animindex ].endframe = m_endframe;//!!!!!!!!!!!!

		ParamsToDlg();
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );

	}else if( m_altdrag ){

		int newx;
		newx = LOWORD(lParam);
		int dx;
		float divx;
		divx = 1200.0f / (float)m_sl_mp_rangemax;
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
		maxdx = m_sl_mp_rangemax - m_endframe;
		int mindx;
		mindx = 0 - m_startframe;

		if( changex > 0 ){
			int shiftx;
			shiftx = min( changex, maxdx );
			m_startframe += shiftx;
			m_endframe += shiftx;
			if( (m_current < m_startframe) || (m_current > m_endframe) ){
				m_current = m_startframe;
			}
		}else if( changex < 0 ){
			int shiftx;
			shiftx = max( changex, mindx );
			m_startframe += shiftx;
			m_endframe += shiftx;
			if( (m_current < m_startframe) || (m_current > m_endframe) ){
				m_current = m_startframe;
			}			
		}else{
			return 0;
		}

		m_motid[ animindex ].startframe = m_startframe;
		m_motid[ animindex ].endframe = m_endframe;


		ParamsToDlg();
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}


	return 0;
}


LRESULT CGPKeyDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	GPELEM cpgpe;
	int cpinterp;
	CGPKey* gpkptr = 0;
	int existflag = 0;

	int cpok = 0;
	ret = anim->ExistGPKey( m_current, &gpkptr );
	_ASSERT( !ret );
	if( ret || !gpkptr ){
		cpok = 0;
	}else{
		cpgpe = gpkptr->m_gpe;
		cpinterp = gpkptr->m_interp;
		cpok = 1;
	}

	if( !cpok ){
		::MessageBox( m_hWnd, "このフレームにはコピーできるキーがありません。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!
	}

	if( m_cpgpk ){
		delete m_cpgpk;
		m_cpgpk = 0;
	}

	m_cpgpk = new CGPKey();
	if( !m_cpgpk ){
		DbgOut( "GPKeyDlg : OnFrameCopy : cpgpk alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_cpgpk->SetGPKey( &cpgpe );
	_ASSERT( !ret );
	ret = m_cpgpk->SetInterp( cpinterp );
	_ASSERT( !ret );


	return 0;
}

LRESULT CGPKeyDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	if( !m_cpgpk ){
		::MessageBox( m_hWnd, "フレームコピーを行ってから再試行してください。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!!!!
	}

	int ret;

	ret = anim->SetGPKey( m_current, m_cpgpk->m_gpe, m_cpgpk->m_interp );
	_ASSERT( !ret );

	ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
	_ASSERT( !ret );
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();
	return 0;
}

LRESULT CGPKeyDlg::OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	ret = anim->DeleteGPKey( m_current );
	if( ret ){
		DbgOut( "GPKeyDlg : OnDelete : ca DeleteGPKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
	_ASSERT( !ret );
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CGPKeyDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	CGPKey* curgpk = anim->GetFirstGPKey();
	CGPKey* nextgpk = 0;
	while( curgpk ){
		nextgpk = curgpk->next;

		ret = anim->DeleteGPKey( curgpk->m_frameno );
		if( ret ){
			DbgOut( "GPKeyDlg : OnAllDelete : ca DeleteGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		curgpk = nextgpk;
	}

	ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
	_ASSERT( !ret );
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CGPKeyDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( m_dragflag )
		return 0;

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

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

		RECT winrectmp;
		m_mp_wnd.GetWindowRect( &winrectmp );

		if( (clickpoint.x >= winrectmp.left) && (clickpoint.x <= winrectmp.right) 
			&& (clickpoint.y >= winrectmp.top) && (clickpoint.y <= winrectmp.bottom) ){

			int centery, uppery, lowery;
			centery = winrectmp.top + ( winrectmp.bottom - winrectmp.top ) / 2;
			uppery = centery - MP_SIZE;
			lowery = centery + MP_SIZE;

			RECT clirect;
			m_mp_wnd.GetClientRect( &clirect );
			int posx, frameno;
			posx = clickpoint.x - winrectmp.left;
			Windowpos2Frameno( clirect, posx, &frameno );

			CGPKey* gpkptr = 0;
			ret = anim->ExistGPKey( frameno, &gpkptr );
			if( !ret && gpkptr ){
				m_dragframe = frameno;
				m_dragflag = 1;
				m_draggpk = gpkptr;

				anim->LeaveFromChainGPKey( frameno );

			}else{
				m_dragframe = 0;
				m_dragflag = 0;
				m_draggpk = 0;
			}
		}else{
			m_dragframe = 0;
			m_dragflag = 0;
			m_draggpk = 0;
		}
	}

	return 0;
}


LRESULT CGPKeyDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	static int busyflag = 0;


	if( busyflag == 1 )
		return 0;

	busyflag = 1;

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	int frameno = -1;

	if( m_ctrldrag || m_altdrag ){
		m_ctrldrag = 0;
		m_altdrag = 0;
		m_dragx = 0;
	}else{

		if( (m_dragflag == 1) && m_draggpk ){
			CGPKey* isexist = 0;

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

			ret = anim->ExistGPKey( frameno, &isexist );
			_ASSERT( !ret );
			if( isexist ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "フレーム番号%d には既にキーフレームが存在するので、移動できません。",
					frameno );
				MessageBox( messtr, "移動できません", MB_OK );
				frameno = m_dragframe;//!!!!! 元に戻す
			}
			
			m_draggpk->m_frameno = frameno;//!!!!!!!!!!!!!!!!

			ret = anim->ChainGPKey( m_draggpk );
			if( ret ){
				DbgOut( "GPKeyDlg : OnLButtonUp : ca ChainGPKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_draggpk = 0;

			ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
			_ASSERT( !ret );

			SetCurrentMotion( 0 );

			ParamsToDlg();
			Redraw();
		}
	}

	m_dragflag = 0;
	m_dragframe = 0;
	m_draggpk = 0;

	busyflag = 0;

	return 0;
}
LRESULT CALLBACK MPProcGP(
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
		//	DbgOut( "GPKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "GPKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
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
int CGPKeyDlg::SetNewWindowProc()
{
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWL_WNDPROC );
	if( !s_mpfunc ){
		_ASSERT( 0 );
		return 1;
	}

	LONG lret;
	lret = ::SetWindowLong( m_sl_mp_wnd, GWL_WNDPROC, (LONG)MPProcGP );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}


LRESULT CGPKeyDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		
	bHandled = TRUE;
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;

	int timerid = (int)wParam;
	if( (timerid == GPKeyDlgTIMER) && (m_cmdshow == SW_SHOW) ){

		//ドラッグ中に、マウスが外に出た場合は、ドラッグ操作を強制終了する。
		if( m_dragflag == 1 ){
			POINT ptCursor;
			POINT savecursor;
			GetCursorPos( &ptCursor );
			
			savecursor = ptCursor;//
			::ScreenToClient( m_hWnd, &ptCursor );

			RECT dlgrectmp;
			//m_dlg_wnd.GetWindowRect( &dlgrect );
			m_mp_wnd.GetWindowRect( &dlgrectmp );

			if( (savecursor.x < dlgrectmp.left) || (savecursor.x > dlgrectmp.right) 
				|| (savecursor.y < dlgrectmp.top) || (savecursor.y > dlgrectmp.bottom)
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


int CGPKeyDlg::ShowDlg( int cmdshow, int stopflag )
{
	m_cmdshow = cmdshow;

	if( (m_cmdshow == SW_HIDE) ){
		BOOL handled;
		if( m_dlg->m_thandler && stopflag )
			OnStop( 0, 9999, 0, handled );
		
		ReleaseCapture();
		m_mcap = 0;
		m_moving = 0;
		
		g_inidata->opt_bmdisp = m_savebmdisp;
		g_inidata->opt_jointdisp = m_savejdisp;

//		HWND capwnd;
//		capwnd = ::GetCapture();
//		if( capwnd == m_dlg_wnd.m_hWnd ){
//			ReleaseCapture();
//		}
	}

	ShowWindow( m_cmdshow );

	//int ret;
	if( m_cmdshow != SW_HIDE ){
		m_savebmdisp = g_inidata->opt_bmdisp;
		m_savejdisp = g_inidata->opt_jointdisp;

		BOOL dummyb;
		OnStop( 0, 9999, 0, dummyb );
		//m_dlg->m_current = 0;//!!!!!!!!!!!!!!!!!!!!!! 別スレッドでのセットが遅れるため、ここで強制リセット。
		//OnUserSelchange();
		ResetParams( 1 );
		ParamsToDlg();

		g_inidata->opt_bmdisp = 0;
		//g_inidata->opt_jointdisp = 0;

	}


	return 0;
}

LRESULT CGPKeyDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;


	int findframe = -1;
	int chkframe;

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( anim && (m_current - 1) > m_startframe ){
		for( chkframe = (m_current - 1); chkframe >= m_startframe; chkframe-- ){
			if( (anim->m_framedata + chkframe)->m_keyflag != 0 ){
				findframe = chkframe;
				break;
			}
		}
	}

	if( findframe >= 0 ){
		m_current = findframe;
	}else{
		m_current = m_startframe;
	}

///////
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	BOOL dummy;	
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CGPKeyDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	int findframe = -1;
	int chkframe;

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( anim && (m_current + 1) < m_endframe ){
		for( chkframe = (m_current + 1); chkframe <= m_endframe; chkframe++ ){
			if( (anim->m_framedata + chkframe)->m_keyflag != 0 ){
				findframe = chkframe;
				break;
			}
		}
	}

	if( findframe >= 0 ){
		m_current = findframe;
	}else{
		m_current = m_endframe;
	}

///////////
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	BOOL dummy;	
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CGPKeyDlg::OnCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	LRESULT ret;
	BOOL dummy;
	ret = OnApply( 0, 0, 0, dummy );

/***
	int ret;
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	ret = anim->SetGPKey( m_current, m_gpe, -1 );
	if( ret ){
		DbgOut( "GPKeyDlg : OnCreate : ca SetGPKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
	_ASSERT( !ret );
***/
//	SetCurrentMotion( 0 );
//	ParamsToDlg();
//	Redraw();

	return 0;
}

LRESULT CGPKeyDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CGPKeyDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}
int CGPKeyDlg::SetNewGP()
{
	int ret;
	static int busyflag = 0;

	if( busyflag != 0 ){
		return 0;
	}

	busyflag = 1;

	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		if( m_gpah->m_animnum >= 1 ){
			ret = m_gpah->SetNewGP();
			_ASSERT( !ret );
			m_animno = m_gpah->m_animno;
			m_gpe = m_gpah->m_curgpe;
			anim = m_gpah->GetGPAnim( m_animno );
			if( anim ){
				m_current = anim->m_frameno;
			}
			ParamsToDlg();
		}
	}else{
		m_gpe = m_hengpe;
		ParamsToDlg();
	}

	busyflag = 0;
	return 0;
}


int CGPKeyDlg::SetCurrentMotion( int isfirst )
{
	int ret;
	if( (m_animno >= 0) && (m_gpmode != GP_HEN) ){
		_ASSERT( m_animno >= 0 );

		ret = m_gpah->SetAnimFrameNo( m_animno, m_current, isfirst );
		_ASSERT( !ret );
		m_gpe = m_gpah->m_curgpe;
	}
	if( m_gpmode == GP_HEN ){
		m_gpe = m_hengpe;
	}

	return 0;
}


int CGPKeyDlg::ResetParams( int atshowflag )
{
	
	int ret;

	DestroyObjs( 0 );

	ret = InitDlg( atshowflag );
	if( ret ){
		DbgOut( "GPKeydlg : ResetParams : InitDlg error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = InitCombo();
	if( ret ){
		DbgOut( "GPKeydlg : ResetParams : InitCombo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	SetCurrentMotion( 0 );

	return 0;
}


LRESULT CGPKeyDlg::OnComboG(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_combog_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	m_gtype = combono;

	return 0;
}
LRESULT CGPKeyDlg::OnComboOnG(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_comboong_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	m_gpe.ongmode = combono;

	if( m_gpmode == GP_HEN ){
		m_hengpe.ongmode = combono;
	}

	return 0;
}
LRESULT CGPKeyDlg::OnComboPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_comboplane_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	m_planetype = combono;

	return 0;
}

LRESULT CGPKeyDlg::OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CGPAnim* anim;
	int animindex;
	anim = m_gpah->GetGPAnim( m_animno, &animindex );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret = 0;
	int dlgret;
	
	CMotSettingDlg	dlg( 0, anim->m_animname, m_sl_mp_rangemax + 1, anim->m_animtype, m_gpah->m_definterp, anim->m_motjump, m_startframe, m_endframe );
	::SendMessage( m_dlg->m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	dlgret = (int)dlg.DoModal();
	::SendMessage( m_dlg->m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( dlgret == IDOK ){
		if( anim->m_animtype != dlg.m_mottype ){
			anim->m_animtype = dlg.m_mottype;
		}

		if( anim->m_motjump != dlg.m_motjump ){
			anim->m_motjump = dlg.m_motjump;
		}

		int savestart = m_startframe;
		int saveend = m_endframe;

		m_startframe = dlg.m_startframe;
		m_endframe = dlg.m_endframe;

		int dlgret2 = IDOK;
		int newrangemax = dlg.m_totalframe - 1;
		if( m_sl_mp_rangemax != newrangemax ){
			if( m_sl_mp_rangemax > newrangemax ){
				char tempchar[1024];
				ZeroMemory( tempchar, 1024 );
				sprintf_s( tempchar, 1024, "総フレーム数が%d から%dに縮小されます。\nフレーム番号%d以降のデータは削除されます。\nよろしいですか？",
					m_sl_mp_rangemax + 1,
					newrangemax + 1,
					newrangemax + 1 );
				::SendMessage( m_dlg->m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
				dlgret2 = ::MessageBox( NULL, tempchar, "警告", MB_OKCANCEL );
				::SendMessage( m_dlg->m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

			}

			if( dlgret2 == IDOK ){
				ret = m_gpah->SetTotalFrame( m_animno, newrangemax + 1 );
				if( ret ){
					DbgOut( "GPKeyDlg : OnSetting : cah SetTotalFrame error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				m_sl_mp_rangemax = newrangemax;
//				_ASSERT( 0 );

				m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, m_startframe );
				m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
				m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
				SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

				BOOL dummy;
				m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );
				OnReleasedcaptureSl_mp( 0, 0, dummy );
			}else{
				m_startframe = savestart;
				m_endframe = saveend;
			}
		}

		if( (m_startframe != savestart) || (m_endframe != saveend) ){
			m_current = m_startframe;
			InterlockedExchange( &m_previewframe, m_startframe );

			m_motid[ animindex ].startframe = m_startframe;
			m_motid[ animindex ].endframe = m_endframe;

		}

		int newinterpolation = dlg.m_interpolation;
		if( m_gpah->m_definterp != dlg.m_interpolation ){
			m_gpah->m_definterp = dlg.m_interpolation;
		}

		int cmp;
		cmp = strcmp( anim->m_animname, dlg.m_name );
		if( cmp != 0 ){
			strcpy_s( anim->m_animname, 256, dlg.m_name );
//			::SendMessage( m_dlg->m_apphwnd, WM_USER_GPMENU, animindex, 0 );
		}

		CalcFrameData();
		SetCurrentMotion( 0 );

		ParamsToDlg();
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
		OnReleasedcaptureSl_mp( 0, 0, dummy );
//		_ASSERT( 0 );
	}

	::SendMessage( m_dlg->m_apphwnd, WM_USER_GPMENU, animindex, 0 );

//	_ASSERT( 0 );

	return 0;
}

LRESULT CGPKeyDlg::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat, tmpfloat2, tmpfloat3;
	char mes[512];

	ret = GetFloat( m_posx_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "pos Xの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_posy_wnd, &tmpfloat2 );
	if( ret ){
		strcpy_s( mes, 512, "pos Yの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_posz_wnd, &tmpfloat3 );
	if( ret ){
		strcpy_s( mes, 512, "pos Zの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_gpe.pos = D3DXVECTOR3( tmpfloat, tmpfloat2, tmpfloat3 );


	ret = GetFloat( m_rotx_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "rot Xの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_roty_wnd, &tmpfloat2 );
	if( ret ){
		strcpy_s( mes, 512, "rot Yの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_rotz_wnd, &tmpfloat3 );
	if( ret ){
		strcpy_s( mes, 512, "rot Zの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_gpe.rot = D3DXVECTOR3( tmpfloat, tmpfloat2, tmpfloat3 );


	ret = GetFloat( m_rayy_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "rayYの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_gpe.rayy = tmpfloat;

	ret = GetFloat( m_rayleng_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "raylengの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_gpe.rayleng = tmpfloat;

	ret = GetFloat( m_offsety_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "offsetyの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_gpe.offsety = tmpfloat;

	int combono;
	combono = (int)m_comboong_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 0;
	}
	m_gpe.ongmode = combono;

	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : ApplyParams : ca SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );

		SetCurrentMotion( 0 );
	}else{
		m_hengpe = m_gpe;

		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : ApplyParams : ca SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );

		int isfirst = 0;
		ret = m_gpah->SetAnimFrameNo( m_animno, m_current, isfirst );
		_ASSERT( !ret );
		m_gpe = m_gpah->m_curgpe;
		m_hengpe = m_gpe;

	}
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CGPKeyDlg::OnApplyDrag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	ret = GetFloat( m_drag_wnd, &tmpfloat );
	if( ret || (tmpfloat < 1.0f) || (tmpfloat > 10000.0f) ){
		::MessageBox( m_hWnd, "ドラッグ量は1.0から10000.0までです。", "入力エラー", MB_OK );
		return 0;
	}

	g_inidata->gpa_move = tmpfloat;

	return 0;
}


int CGPKeyDlg::SetPreviewButton()
{
	if( m_dlg->m_preview_flag ){
		::SendMessage( m_preview_wnd, BM_SETSTATE, TRUE, 0 );  
	}else{
		::SendMessage( m_preview_wnd, BM_SETSTATE, FALSE, 0 );  
	}

	return 0;
}

int CGPKeyDlg::GetCurGP( GPELEM* dstgpe )
{
	*dstgpe = m_gpe;
	return 0;
}

int CGPKeyDlg::GetGPAnimNum()
{
	if( !IsWindow() )
		return 0;

	if( !m_gpah ){
		return 0;
	}

	int retnum;
	retnum = m_gpah->m_animnum;
	return retnum;
}


int CGPKeyDlg::LoadGPFile( char* filename )
{
	int ret;
	if( !m_gpah ){
		_ASSERT( 0 );
		return 0;
	}

	CGPFile gpfile;
	int animno = -1;
	ret = gpfile.LoadGPFile( filename, m_gpah, &animno );
	if( ret ){
		DbgOut( "GPKeyDlg : LoadGPFile : gpfile LoadGPFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_gpah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "GPKeyDlg : LoadGPFile : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_animno = animno;
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		DbgOut( "GPKeyDlg : LoadGPFile : cah GetGPAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_gpah->SetCurrentAnim( m_animno );
	if( ret ){
		DbgOut( "GPKeyDlg : LoadGPFile : cah SetCurrentAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_motid[ m_gpah->m_animnum - 1 ].id = m_animno;
	char* enptr = 0;
	enptr = strrchr( filename, '\\' );
	if( enptr ){
		strcpy_s( m_motid[ m_gpah->m_animnum - 1 ].filename, 256, enptr + 1 );
	}else{
		strcpy_s( m_motid[ m_gpah->m_animnum - 1 ].filename, 256, filename );
	}

	m_sl_mp_rangemax = anim->m_maxframe;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;

	m_motid[ m_gpah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_gpah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	::SendMessage( m_dlg->m_apphwnd, WM_USER_GPMENU, m_gpah->m_animnum - 1, 0 );
	
	return 0;
}


MOTID* CGPKeyDlg::GetGPMotIDArray()
{
	return m_motid;
}
char* CGPKeyDlg::GetGPAnimName( int animno )
{
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( animno );
	if( !anim ){
		return 0;
	}
	return anim->m_animname;
}
int CGPKeyDlg::GetCurGPAnimIndex()
{
	CGPAnim* anim;
	int animindex = 0;
	anim = m_gpah->GetGPAnim( m_animno, &animindex );
	if( anim ){
		return animindex;
	}else{
		return 0;
	}
}
int CGPKeyDlg::GetCurGPAnimNo()
{
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( anim ){
		return anim->m_motkind;
	}else{
		return 0;
	}
}

int CGPKeyDlg::SelectGPAnim( int srcindex )
{
	if( !m_gpah ){
		_ASSERT( 0 );
		return 0;
	}
	if( m_dlg->m_preview_flag ){
		return 0;
	}

	if( (srcindex < 0) || (srcindex >= m_gpah->m_animnum) ){
		DbgOut( "GPKeyDlg : SelectGPAnim : srcindex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	int tmpcookie;
	tmpcookie = m_motid[ srcindex ].id;
	if( tmpcookie < 0 ){
		DbgOut( "GPKeyDlg : SelectGPAnim : tmpcookie error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

/////////
	CGPAnim* anim;
	anim = m_gpah->GetGPAnimByIndex( srcindex );
	if( !anim ){
		DbgOut( "GPKeyDlg : SelectGPAnim : cah GetGPAnimByIndex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_sl_mp_rangemax = anim->m_maxframe;
	m_startframe = m_motid[ srcindex ].startframe;
	m_endframe = m_motid[ srcindex ].endframe;
	
	//InterlockedExchange( &s_startframe, m_startframe );
	//InterlockedExchange( &s_frameno, m_startframe );
	InterlockedExchange( &m_current, m_startframe );
	InterlockedExchange( &m_animno, (LONG)tmpcookie );
	InterlockedExchange( &m_previewframe, m_startframe );


	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

////////////

	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

////////////
	::SendMessage( m_dlg->m_apphwnd, WM_USER_GPMENU, srcindex, 0 );


	return 0;
}

int CGPKeyDlg::DestroyGPAnim( int delanimno )
{
	int ret;

	if( delanimno < 0 ){
		if( m_gpah ){
			delete m_gpah;
			m_gpah = 0;
		}

		ret = InitDlg( 0 );
		if( ret ){
			DbgOut( "GPKeyDlg : DestroyGPAnim all : InitDlg error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{
		_ASSERT( m_gpah );
		if( m_gpah->m_animnum <= 0 ){
			return 0;
		}

		int delcookie = m_animno;

		MOTID tempmotid[MAXMOTIONNUM];
		int mindex;
		for( mindex = 0; mindex < MAXMOTIONNUM; mindex++ ){
			tempmotid[mindex].id = -1;
			ZeroMemory( tempmotid[mindex].filename, sizeof( char ) * 256 );
			tempmotid[mindex].forbidnum = 0;
			tempmotid[mindex].forbidid = 0;
			tempmotid[mindex].startframe = 0;
			tempmotid[mindex].endframe = 0;
		}

		int setno = 0;
		for( mindex = 0; mindex < MAXMOTIONNUM; mindex++ ){
			int curid;
			curid = m_motid[mindex].id;
			if( curid != delcookie ){
				if( curid != -1 ){
					tempmotid[setno].id = curid;//!!!!
					strcpy_s( tempmotid[setno].filename, 256, m_motid[mindex].filename );
					tempmotid[setno].ev0idle = m_motid[mindex].ev0idle;
					tempmotid[setno].commonid = m_motid[mindex].commonid;
					tempmotid[setno].forbidnum = m_motid[mindex].forbidnum;
					tempmotid[setno].forbidid = m_motid[mindex].forbidid;
					tempmotid[setno].notfu = m_motid[mindex].notfu;
					tempmotid[setno].startframe = m_motid[mindex].startframe;
					tempmotid[setno].endframe = m_motid[mindex].endframe;
					setno++;
				}else{
					if( m_motid[mindex].forbidid ){
						free( m_motid[mindex].forbidid );
						m_motid[mindex].forbidid = 0;
					}
					m_motid[mindex].forbidnum = 0;				
				}
			}else{
				if( m_motid[mindex].forbidid ){
					free( m_motid[mindex].forbidid );
					m_motid[mindex].forbidid = 0;
				}
				m_motid[mindex].forbidnum = 0;
			}
		}

		if( setno != (m_gpah->m_animnum - 1) ){
			_ASSERT( 0 );
			return 1;
		}

		MoveMemory( m_motid, tempmotid, sizeof( MOTID ) * MAXMOTIONNUM );

		//m_motionnum -= 1;//!!!!!

		int ret;
		ret = m_gpah->DestroyAnim( delcookie );
		if( ret ){
			DbgOut( "GPKeyDlg : DestroyGPAnim cur : cah DestroyAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( m_gpah->m_animnum <= 0 ){
			ret = InitDlg( 0 );
			if( ret ){
				DbgOut( "GPKeyDlg : DestroyGPAnim cur : InitDlg error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = SelectGPAnim( 0 );
		if( ret ){
			DbgOut( "GPKeyDlg : DestroyGPAnim cur : SelectGPAnim 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CGPKeyDlg::SetCurGP( GPELEM srcgpe )
{
	int ret;

	m_gpe = srcgpe;

	if( m_gpah->m_animnum <= 0 ){
		ret = InitDlg( 0 );
		if( ret ){
			DbgOut( "GPKeyDlg : SetCurGP : InitDlg error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	ret = anim->SetGPKey( m_current, m_gpe, m_gpah->m_definterp );
	_ASSERT( !ret );

	ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
	_ASSERT( !ret );

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

int CGPKeyDlg::CalcMotionPointOnFrame( int animno, int frameno, CGPKey* gpkptr, int* hasmpflag )
{
	int ret;
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( animno );
	if( !anim ){
		gpkptr->m_gpe = m_gpah->m_defaultgpe;
		gpkptr->m_interp = m_gpah->m_definterp;
		gpkptr->m_frameno = frameno;
		*hasmpflag = 0;
		return 0;
	}

	GPELEM gpe;
	int interp;
	ret = anim->GetGPAnim( &gpe, &interp, frameno, hasmpflag );
	if( ret ){
		DbgOut( "GPKeyDlg : CalcMotionPointOnFrame : ca GetGPAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	gpkptr->m_gpe = gpe;
	gpkptr->m_interp = interp;
	gpkptr->m_frameno = frameno;

	return 0;
}


int CGPKeyDlg::ExistGPKey( int animno, int frameno, CGPKey** ppgpk )
{
	*ppgpk = 0;

	int ret;
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( animno );
	if( !anim ){
		return 0;
	}

	ret = anim->ExistGPKey( frameno, ppgpk );
	if( ret ){
		DbgOut( "GPKeyDlg : ExistGPKey : ca ExistGPKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CGPKeyDlg::SetFillUpMode( int animno, int frameno, int setfu )
{
	int ret;
	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( animno );
	if( !anim ){
		return 0;
	}

	ret = anim->SetFillUpMode( frameno, setfu );
	_ASSERT( !ret );

	ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
	_ASSERT( !ret );

	return 0;
}

LRESULT CGPKeyDlg::OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = CalcFrameData();
	if( ret ){
		DbgOut( "gpkeydlg : OnCalc : CalcFrameData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

int CGPKeyDlg::CalcFrameData()
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
	_ASSERT( !ret );

	return 0;
}
int CGPKeyDlg::GetCalcGPKey( int frameno, CGPKey* pgpk )
{
	_ASSERT( pgpk );

	CGPAnim* anim;
	anim = m_gpah->GetGPAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	if( (frameno < 0) || (frameno > anim->m_maxframe) ){
		DbgOut( "gpkeydlg : GetCalcGPKey : frameno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CGPKey* srcgpk = anim->m_framedata + frameno;
	pgpk->m_frameno = frameno;
	pgpk->m_gpe = srcgpk->m_gpe;
	pgpk->m_interp = srcgpk->m_interp;
	pgpk->m_keyflag = srcgpk->m_keyflag;

	return 0;
}

int CGPKeyDlg::RotateGP( int axiskind, float delta, CQuaternion localq )
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( delta == 0.0f )
		return 0;
	if( (m_gpmode != GP_HEN) && (m_gpah->m_animnum <= 0) ){
		return 0;
	}

	int ret;
//	if( m_gpmode == GP_HEN ){
//		m_gpe = m_hengpe;
//	}


	CQuaternion rotq;
	D3DXVECTOR3 befaxis;
	D3DXVECTOR3 axis;
	switch( axiskind ){
	case 0:
		befaxis = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		localq.Rotate( &axis, befaxis );
		D3DXVec3Normalize( &axis, &axis );
		rotq.SetAxisAndRot( axis, -delta );
		break;
	case 1:
		befaxis = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		localq.Rotate( &axis, befaxis );
		D3DXVec3Normalize( &axis, &axis );
		rotq.SetAxisAndRot( axis, -delta );
		break;
	case 2:
		befaxis = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		localq.Rotate( &axis, befaxis );
		D3DXVec3Normalize( &axis, &axis );
		rotq.SetAxisAndRot( axis, -delta );
		break;
	default:
		_ASSERT( 0 );
		return 1;
		break;
	}

	CQuaternion befq;
	ret = Deg2Q( m_gpe.rot, &befq );
	_ASSERT( !ret );

	CQuaternion newq;
//	newq = befq * rotq;
	newq = rotq * befq;

	D3DXVECTOR3 neweul;
	ret = qToEuler( 0, &newq, &neweul );
	_ASSERT( !ret );
	ret = modifyEuler( &neweul, &m_gpe.rot );
	_ASSERT( !ret );
	m_gpe.rot = neweul;

	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : RotateGP : SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );

		SetCurrentMotion( 0 );
	}else{
		m_hengpe = m_gpe;
	}
	ParamsToDlg();
	Redraw();

	return 0;
}
int CGPKeyDlg::ShiftGP( int axiskind, float delta, CQuaternion localq )
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( delta == 0.0f )
		return 0;
	if( (m_gpmode != GP_HEN) && (m_gpah->m_animnum <= 0) ){
		return 0;
	}

	int ret;

//	if( m_gpmode == GP_HEN ){
//		m_gpe = m_hengpe;
//	}


	D3DXVECTOR3 diffvec;
	D3DXVECTOR3 befdiff;
	switch( axiskind ){
	case 0:
		befdiff.x = delta;
		befdiff.y = 0.0f;
		befdiff.z = 0.0f;

		localq.Rotate( &diffvec, befdiff );
		break;
	case 1:
		befdiff.x = 0.0f;
		befdiff.y = delta;
		befdiff.z = 0.0f;

		localq.Rotate( &diffvec, befdiff );
		break;
	case 2:
		befdiff.x = 0.0f;
		befdiff.y = 0.0f;
		befdiff.z = delta;

		localq.Rotate( &diffvec, befdiff );
		break;
	default:
		_ASSERT( 0 );
		return 1;
		break;
	}

	m_gpe.pos += diffvec;

	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : RotateGP : SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );

		SetCurrentMotion( 0 );
	}else{
		m_hengpe = m_gpe;
	}
	ParamsToDlg();
	Redraw();


	return 0;
}

int CGPKeyDlg::Deg2Q( D3DXVECTOR3 rot, CQuaternion* dstq )
{
	CQuaternion qx, qy, qz;
	D3DXVECTOR3 axisX( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 axisY( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 axisZ( 0.0f, 0.0f, 1.0f );
	qx.SetAxisAndRot( axisX, rot.x * (float)DEG2PAI );
	qy.SetAxisAndRot( axisY, rot.y * (float)DEG2PAI );
	qz.SetAxisAndRot( axisZ, rot.z * (float)DEG2PAI );

	*dstq = qy * qx * qz;
	
	return 0;
}

LRESULT CGPKeyDlg::OnDoRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int ret;
	float tmpfloat, tmpfloat2, tmpfloat3;
	char mes[512];

	ret = GetFloat( m_cenx_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "center Xの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_ceny_wnd, &tmpfloat2 );
	if( ret ){
		strcpy_s( mes, 512, "center Yの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_cenz_wnd, &tmpfloat3 );
	if( ret ){
		strcpy_s( mes, 512, "center Zの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_center = D3DXVECTOR3( tmpfloat, tmpfloat2, tmpfloat3 );


	ret = GetFloat( m_mvdeg_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "回転角度の値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_mvdeg = tmpfloat;


	D3DXMATRIX befrotmat, aftrotmat, rotmat, procmat;
	D3DXVECTOR3 axis;
	CQuaternion rotq;

	D3DXMatrixIdentity( &befrotmat );
	D3DXMatrixIdentity( &aftrotmat );
	D3DXMatrixIdentity( &rotmat );
	D3DXMatrixIdentity( &procmat );

	befrotmat._41 = -m_center.x;
	befrotmat._42 = -m_center.y;
	befrotmat._43 = -m_center.z;
	aftrotmat._41 = m_center.x;
	aftrotmat._42 = m_center.y;
	aftrotmat._43 = m_center.z;

	if( m_planetype == GPPLANE_XZ ){
		axis = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	}else if( m_planetype == GPPLANE_XY ){
		axis = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	}else if( m_planetype == GPPLANE_YZ ){
		axis = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	}else{
		_ASSERT( 0 );
		return 1;
	}
	rotq.SetAxisAndRot( axis, m_mvdeg * (float)DEG2PAI );
	rotmat = rotq.MakeRotMatX();
	procmat = befrotmat * rotmat * aftrotmat;
//位置
	D3DXVECTOR3 newpos;
	D3DXVec3TransformCoord( &newpos, &m_gpe.pos, &procmat );
	m_gpe.pos = newpos;

//向き
	CQuaternion orgdirq;
	Deg2Q( m_gpe.rot, &orgdirq );

	procmat._41 = 0.0f;
	procmat._42 = 0.0f;
	procmat._43 = 0.0f;
	D3DXQUATERNION dirqx;
	D3DXQuaternionRotationMatrix( &dirqx, &procmat );

	CQuaternion dirq;
	dirq.x = dirqx.x;
	dirq.y = dirqx.y;
	dirq.z = dirqx.z;
	dirq.w = dirqx.w;

	CQuaternion newdirq;
//	newdirq = orgdirq * dirq;
	newdirq = dirq * orgdirq;

	D3DXVECTOR3 neweul;
	ret = qToEuler( 0, &newdirq, &neweul );
	_ASSERT( !ret );
	ret = modifyEuler( &neweul, &m_gpe.rot );
	_ASSERT( !ret );
	m_gpe.rot = neweul;

//更新
	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : RotateGP : SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );
		SetCurrentMotion( 0 );
	}else{
		m_hengpe = m_gpe;
	}
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CGPKeyDlg::OnSpin_RX(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	char mes[512];

	ret = GetFloat( m_rotstep_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "rot stepの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_rotstep = tmpfloat;

	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
	int delta;
	// defaultで、左ボタンがプラスになっているので、反転する。
	delta = -(pnmud->iDelta);

	float deltadeg = 0.0f;
	if( delta > 0 ){
		deltadeg = m_rotstep;
	}else{
		deltadeg = -m_rotstep;
	}

	m_gpe.rot.x += deltadeg;


//更新
	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : RotateGP : SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );
		SetCurrentMotion( 0 );
	}else{
		m_hengpe = m_gpe;
	}
	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CGPKeyDlg::OnSpin_RY(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	char mes[512];

	ret = GetFloat( m_rotstep_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "rot stepの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_rotstep = tmpfloat;

	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
	int delta;
	// defaultで、左ボタンがプラスになっているので、反転する。
	delta = -(pnmud->iDelta);

	float deltadeg = 0.0f;
	if( delta > 0 ){
		deltadeg = m_rotstep;
	}else{
		deltadeg = -m_rotstep;
	}

	m_gpe.rot.y += deltadeg;


//更新
	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : RotateGP : SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );
		SetCurrentMotion( 0 );
	}else{
		m_hengpe = m_gpe;
	}
	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CGPKeyDlg::OnSpin_RZ(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	char mes[512];

	ret = GetFloat( m_rotstep_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "rot stepの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_rotstep = tmpfloat;


	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
	int delta;
	// defaultで、左ボタンがプラスになっているので、反転する。
	delta = -(pnmud->iDelta);

	float deltadeg = 0.0f;
	if( delta > 0 ){
		deltadeg = m_rotstep;
	}else{
		deltadeg = -m_rotstep;
	}

	m_gpe.rot.z += deltadeg;


//更新
	if( m_gpmode != GP_HEN ){
		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			_ASSERT( 0 );
			return 0;
		}
		ret = anim->SetGPKey( m_current, m_gpe, -1 );
		if( ret ){
			DbgOut( "GPKeyDlg : RotateGP : SetGPKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = anim->CalcFrameData( m_app->m_g_sh, m_app->m_g_mh, &(m_app->m_g_sh->m_gpdata.m_matWorld) );
		_ASSERT( !ret );
		SetCurrentMotion( 0 );
	}else{
		m_hengpe = m_gpe;
	}
	ParamsToDlg();
	Redraw();

	return 0;
}

int CGPKeyDlg::OnJumpFrame( int srcframe )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CGPAnim* anim;
	int animindex;
	anim = m_gpah->GetGPAnim( m_animno, &animindex );
	if( !anim ){
		return 0;
	}

	if( srcframe < 0 ){
		m_current = 0;
	}else if( srcframe > m_sl_mp_rangemax ){
		m_current = m_sl_mp_rangemax;
	}else{
		m_current = srcframe;
	}

	int newstart, newend;
	if( (m_current >= m_startframe) && (m_current <= m_endframe) ){
		newstart = m_startframe;
		newend = m_endframe;
	}else{
		int range;
		range = m_endframe - m_startframe;
		newstart = max( (m_current - range / 2), 0 );
		newend = min( (m_current + range / 2), m_sl_mp_rangemax );
	}

	m_startframe = newstart;
	m_endframe = newend;

	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//


	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	//OnReleasedcaptureSl_mp( 0, 0, dummy );

//	InterlockedExchange( &m_previewframe, m_startframe );

	m_motid[ animindex ].startframe = m_startframe;
	m_motid[ animindex ].endframe = m_endframe;

	BOOL dummy;
	OnReleasedcaptureSl_mp( 999, 0, dummy );

	return 0;
}

int CGPKeyDlg::SetStartAndEnd( int srcstart, int srcend )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CGPAnim* anim;
	int animindex;
	anim = m_gpah->GetGPAnim( m_animno, &animindex );
	if( !anim ){
		return 0;
	}


	if( (srcstart >= 0) && (srcstart <= m_sl_mp_rangemax) ){
		m_startframe = srcstart;
	}else if( srcstart < 0 ){
		m_startframe = 0;
	}else{
		m_startframe = m_sl_mp_rangemax;
	}

	if( (srcend >= 0) && (srcend <= m_sl_mp_rangemax) ){
		m_endframe = srcend;
	}else if( srcend < 0 ){
		m_endframe = 0;
	}else{
		m_endframe = m_sl_mp_rangemax;
	}

	if( m_endframe < m_startframe ){
		m_endframe = m_startframe;
	}

	if( (m_current < m_startframe) || (m_current > m_endframe) ){
		m_current = m_startframe;
	}

	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//


	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	//OnReleasedcaptureSl_mp( 0, 0, dummy );

	InterlockedExchange( &m_previewframe, m_startframe );

	m_motid[ animindex ].startframe = m_startframe;
	m_motid[ animindex ].endframe = m_endframe;

	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	return 0;
}

int CGPKeyDlg::PlayAnim( int srcframe, int totalflag )
{
	int ret;
	int frameno = srcframe;
	if( totalflag == 0 ){
		frameno = max( m_startframe, frameno );
		frameno = min( m_endframe, frameno );
	}else{
		frameno = max( 0, frameno );
		frameno = min( m_sl_mp_rangemax, frameno );
	}
	InterlockedExchange( &m_current, frameno );

	if( m_gpmode != GP_HEN ){
		ret = SetCurrentMotion( 0 );
		_ASSERT( !ret );
	}else{
		m_gpe = m_hengpe;
	}

	ParamsToDlg();

	return 0;
}

LRESULT CGPKeyDlg::OnChkHen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKHEN );
	if( checkflag == BST_CHECKED ){
		m_gpmode = GP_HEN;
		m_gpe = m_hengpe;
		ParamsToDlg();
		Redraw();

	}else{
		m_gpmode = GP_ANIM;

		ParamsToDlg();
		Redraw();

		CGPAnim* anim;
		anim = m_gpah->GetGPAnim( m_animno );
		if( !anim ){
			return 0;
		}
		SetCurrentMotion( 0 );
	}

	return 0;
}
