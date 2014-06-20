// SndKeyDlg.cpp : CSndKeyDlg のインプリメンテーション
#include "stdafx.h"
#include "SndKeyDlg.h"

#include <math.h>
#include <stdio.h>
//#include <D3DX9.h>

#include <coef.h>
#define DBGH
#include <dbg.h>

#include "HuSound.h"

#include <SndAnimHandler.h>
#include <SndAnim.h>
#include <SndKey.h>
#include <SndFullFrame.h>
#include <SndFrame.h>

#include "SoundBank.h"
#include "SoundSet.h"
#include "SoundElem.h"

#include <SndConv.h>

//#include <SndFile.h>

#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "viewer.h"
#include "MotDlg.h"
#include "MotParamDlg.h"
#include "SndSettingDlg.h"
#include "SyncDlg.h"

#include <SndFile.h>

#include "GetDlgParams.h"
#include "inidata.h" // for ini file
#include "SyncDlg.h"

#include <crtdbg.h>

//MotionPoint描画時の点の半径
#define MP_SIZE	3


/////////////////
// sttic
static LONG s_mpfunc = 0;
static HWND s_sl_mp_wnd = 0;
static HWND s_dlg_wnd = 0;
static HWND s_apphwnd = 0;

/////////////////
//extern HINSTANCE g_resinstance;
extern CMotDlg* g_motdlg;
extern CIniData* g_inidata;
extern unsigned char g_KeyBuffer[256];
extern int g_endapp;

/////////////////////////////////////////////////////////////////////////////
// CSndKeyDlg

CSndKeyDlg::CSndKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp )
//: m_list_wnd( NULL, this, 2 )
{
	InitParams();

	m_dlg = srcmpdlg;
	m_dlg->m_sndkeydlg = this;
	m_app = srcapp;
}

int CSndKeyDlg::InitParams()
{
	m_sndah = 0;
	m_dlg = 0;
	m_app = 0;

	m_current = 0;
	m_ismoving = 0;

	m_sl_mp_rangemax = 59;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_previewframe = 0;

	m_dragsndk = 0;
	m_dragframe = 0;
	m_dragflag = 0;
	m_cpsndk = 0;

	m_animno = 0;

	ZeroMemory( &m_snde, sizeof( SNDELEM ) );
	m_snde.sndid = -1;
	m_snde.sndsetid = -1;
	ZeroMemory( &m_zerosnde, sizeof( SNDELEM ) );
	m_snde.sndid = -1;
	m_snde.sndsetid = -1;

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

	ZeroMemory( m_curssname, sizeof( char ) * 32 );
	m_sscomboindex = -1;
	m_secomboindex = -1;
	m_elemindex = -1;

	m_syncflag = 1;

	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

	m_savebmdisp = g_inidata->opt_bmdisp;
	m_savejdisp = g_inidata->opt_jointdisp;

	return 0;
}


CSndKeyDlg::~CSndKeyDlg()
{
	DestroyObjs( 1 );
}

int CSndKeyDlg::DestroyObjs( int delsndah )
{
	if( m_cpsndk ){
		delete m_cpsndk;
		m_cpsndk = 0;
	}

	if( delsndah ){
		if( m_sndah ){
			delete m_sndah;
			m_sndah = 0;
		}
	}

	return 0;
}

int CSndKeyDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, SndKeyDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	return 0;
}

LRESULT CSndKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	if( m_sndah ){
		delete m_sndah;
		m_sndah = 0;
	}
	m_sndah = new CSndAnimHandler();
	_ASSERT( m_sndah );


	ret = SetWnd();
	_ASSERT( !ret );
	s_sl_mp_wnd = m_sl_mp_wnd;

	ret = InitComboSS();
	_ASSERT( !ret );

	ret = InitComboSE();
	_ASSERT( !ret );

	ret = SetNewWindowProc();
	_ASSERT( !ret );

	ret = ParamsToDlg();
	_ASSERT( !ret );

	int timerid;
	timerid = (int)::SetTimer( m_hWnd, SndKeyDlgTIMER, 500, NULL );
	_ASSERT( timerid == SndKeyDlgTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}


int CSndKeyDlg::InitDlg( int atshowflag )
{
	int ret;	
	DbgOut( "SndKeyDlg : InitDlg\n" );


	if( !m_sndah ){
		m_sndah = new CSndAnimHandler();
		_ASSERT( m_sndah );
	}

	if( (atshowflag == 1) && (m_sndah->m_animnum >= 1) && (m_animno >= 0) ){
		//ShowDlgから呼ばれた場合、すでにアニメがあれば作らない。

		ret = ChangeFPS( m_app->m_syncdlg->m_fps );
		if( ret ){
			DbgOut( "sndkeydlg : InitDlg : ChangeFPS error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		return 0;
	}

	if( !m_dlg->m_shandler ){
		return 0;
	}

	if( m_app->m_soundbank->m_setnum <= 0 ){
		m_secomboindex = -1;
		return 1;
	}

	if( (m_sscomboindex < 0) || (m_sscomboindex >= m_app->m_soundbank->m_setnum) ){
		m_sscomboindex = 0;
	}
	CSoundSet* pss = *( m_app->m_soundbank->m_ppset + m_sscomboindex );


	char animname[256];
	ZeroMemory( animname, 256 );
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	sprintf_s( animname, 256, "Snd_%d_%d_%d_%d_%d_%d_%d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
	);

//_ASSERT( 0 );

	int animtype = MOTION_CLAMP;
	CSndAnim* newanim = 0;
	newanim = m_sndah->AddAnim( animname, m_app->m_syncdlg->m_fps, pss );
	if( !newanim ){
		DbgOut( "SndKeyDlg : InitDlg : cah AddAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_animno = newanim->m_motkind;

	m_sl_mp_rangemax = newanim->m_maxframe;//!!!!!!!!!!!!

	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_current = m_startframe;

	strcpy_s( m_curssname, 32, newanim->m_ssname );

	if( m_sndah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "SndKeyDlg : InitDlg : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


//	ret = newanim->InitFrameData();
//	_ASSERT( !ret );

	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );


	m_motid[ m_sndah->m_animnum - 1 ].id = m_animno;
	m_motid[ m_sndah->m_animnum - 1 ].filename[0] = 0;
	m_motid[ m_sndah->m_animnum - 1 ].ev0idle = 0;
	m_motid[ m_sndah->m_animnum - 1 ].commonid = 0;
	m_motid[ m_sndah->m_animnum - 1 ].forbidnum = 0;
	if( m_motid[ m_sndah->m_animnum - 1 ].forbidid ){
		free( m_motid[ m_sndah->m_animnum - 1 ].forbidid );
	}
	m_motid[ m_sndah->m_animnum - 1 ].forbidid = 0;
	m_motid[ m_sndah->m_animnum - 1 ].notfu = 0;

	m_motid[ m_sndah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_sndah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_sndah->m_animnum - 1 ].current = m_startframe;

	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	InterlockedExchange( &m_previewframe, m_startframe );


//	::SendMessage( m_dlg->m_apphwnd, WM_USER_SNDMENU, m_sndah->m_animnum - 1, 0 );

	return 0;
}

LRESULT CSndKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "SndKeyDlg : OnOK\n" );
	ShowWindow( SW_HIDE );
	return 0;
}

LRESULT CSndKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowWindow( SW_HIDE );
	return 0;
}
LRESULT CSndKeyDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	return 0;
}


int CSndKeyDlg::SetWnd()
{
//	CWindow m_dlg_wnd;
//	CWindow m_tex_current_wnd;
//
//	CWindow m_mp_wnd;
//	CWindow m_sp_mp_wnd;
//
//	CWindow m_preview_wnd;
//	CWindow m_stop_wnd;
//
///	CWindow m_sndsync_wnd;
//	
//	CWindow m_sscombo_wnd;
//	CWindow m_secombo_wnd;
//
//	CWindow m_elemlist_wnd;

	m_dlg_wnd = m_hWnd;
	s_dlg_wnd = m_hWnd;

	m_tex_current_wnd = GetDlgItem( IDC_TEX_CURRENT );
	m_sl_mp_wnd = GetDlgItem( IDC_SL_MP );

	m_mp_wnd = GetDlgItem( IDC_MP );
	m_sp_mp_wnd = GetDlgItem( IDC_SP_MP );

	m_preview_wnd = GetDlgItem( IDC_PREVIEW );
	m_stop_wnd = GetDlgItem( IDC_STOP );

	m_sndsync_wnd = GetDlgItem( IDC_SNDSYNC );
	m_sscombo_wnd = GetDlgItem( IDC_SSCOMBO );
	m_secombo_wnd = GetDlgItem( IDC_SECOMBO );
	m_elemlist_wnd = GetDlgItem( IDC_ELEMLIST );
	m_bgm_wnd = GetDlgItem( IDC_BGMNAME );

	//sliderの初期設定
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	return 0;
}


int CSndKeyDlg::InitComboSS()
{
	//int ret;
	
	m_sscombo_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );

	if( m_app->m_soundbank->m_setnum <= 0 ){
		ZeroMemory( m_curssname, sizeof( char ) * 32 );
		m_sscomboindex = -1;
		m_secomboindex = -1;
		return 0;
	}else{
		CSndAnim* anim;
		anim = m_sndah->GetSndAnim( m_animno );
		if( anim ){
			strcpy_s( m_curssname, 32, anim->m_ssname );
		}
	}

	int ssselect = -1;
	int ssno;
	for( ssno = 0; ssno < m_app->m_soundbank->m_setnum; ssno++ ){
		CSoundSet* ssptr = *( m_app->m_soundbank->m_ppset + ssno );

		m_sscombo_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)ssptr->m_setname);

		int cmp0;
		cmp0 = strcmp( ssptr->m_setname, m_curssname );
		if( cmp0 == 0 ){
			ssselect = ssno;
		}
	}
	if( ssselect != -1 ){
		m_sscombo_wnd.SendMessage( CB_SETCURSEL, ssselect, 0 );
		m_sscomboindex = ssselect;
	}else{
		strcpy_s( m_curssname, 32, (*( m_app->m_soundbank->m_ppset ))->m_setname );
		m_sscombo_wnd.SendMessage( CB_SETCURSEL, 0, 0 );
		m_sscomboindex = 0;
	}

	CSoundSet* pss = *( m_app->m_soundbank->m_ppset + m_sscomboindex );

	if( pss->m_soundnum <= 0 ){
		m_secomboindex = -1;
		return 0;
	}



//CB_RESETCONTENT
//	m_combo_no_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
//	m_combo_no_wnd.SendMessage( CB_SETCURSEL, m_editswitchno - 1, 0 );
//	int combono;
//	combono = (int)m_combo_no_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
//	if( combono == CB_ERR )
//		return 1;
	
//CB_GETLBTEXT 


	return 0;
}

int CSndKeyDlg::InitComboSE()
{
	m_secombo_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );

	if( m_app->m_soundbank->m_setnum <= 0 ){
		m_secomboindex = -1;
		return 0;
	}

	if( m_sscomboindex < 0 ){
		return 0;
	}
	if( m_sscomboindex >= m_app->m_soundbank->m_setnum ){
		m_sscomboindex = -1;
		return 0;
	}

	CSoundSet* pss = *( m_app->m_soundbank->m_ppset + m_sscomboindex );

	int seno;
	for( seno = 0; seno < pss->m_soundnum; seno++ ){
		CSoundElem* septr = *( pss->m_ppsound + seno );
		m_secombo_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)septr->m_filename );
	}

	m_secomboindex = 0;
	m_secombo_wnd.SendMessage( CB_SETCURSEL, m_secomboindex, 0 );

	return 0;
}

CSoundSet* CSndKeyDlg::GetCurSS()
{
	if( m_sscomboindex < 0 ){
		return 0;
	}

	if( m_sscomboindex >= m_app->m_soundbank->m_setnum ){
		m_sscomboindex = -1;
		return 0;
	}

	CSoundSet* retss;
	retss = *(m_app->m_soundbank->m_ppset + m_sscomboindex);
	return retss;
}


int CSndKeyDlg::InitList()
{
	if( !m_elemlist_wnd.IsWindow() ){
		return 0;
	}
	m_elemlist_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );

	if( !m_dlg->m_mhandler || !m_dlg->m_shandler || !m_dlg->m_thandler ){

		return 0;
	}
	
	CSoundSet* curss = GetCurSS();
	if( !curss ){
		m_elemindex = -1;
		return 0;
	}

	int ret;
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( anim ){
		ret = anim->CheckAndDelInvalid( curss );//seriチェック。不正データ削除。
		if( ret ){
			DbgOut( "sndkeydlg : InitList : anim CheckAndDelInvalid error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		CSndKey* sndk = 0;
		ret = ExistSndKey( m_animno, m_current, &sndk );
		_ASSERT( !ret );
		if( sndk ){
			if( m_elemindex >= sndk->m_sndenum ){
				m_elemindex = 0;
			}
			//m_snde = *( sndk->m_psnde + m_elemindex );

			int eno;
			for( eno = 0; eno < sndk->m_sndenum; eno++ ){
				SNDELEM* cursnde = sndk->m_psnde + eno;
				_ASSERT( cursnde );

				int newindex;
				if( !m_elemlist_wnd.IsWindow() ){
					return 0;
				}
				newindex = (int)m_elemlist_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)cursnde->sndname );
				if( (newindex == LB_ERR) || (newindex == LB_ERRSPACE) ){
					_ASSERT( 0 );
					return 1;
				}
			}
			if( sndk->m_sndenum > 0 ){
				if( !m_elemlist_wnd.IsWindow() ){
					return 0;
				}
				m_elemlist_wnd.SendMessage( LB_SETCURSEL, (WPARAM)m_elemindex, 0 );
			}
		}else{
			m_elemindex = -1;
		}
	}else{
		m_elemindex = -1;
	}


	return 0;
}

LRESULT CSndKeyDlg::OnSelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	int selindex;
	selindex = (int)m_elemlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		m_elemindex = -1;
		return 0;
	}
	int ret;
	m_elemindex = selindex;
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( anim ){
		CSndKey* sndk = 0;
		ret = anim->ExistSndKey( m_current, &sndk );
		if( sndk ){
			if( m_elemindex >= sndk->m_sndenum ){
				m_elemindex = 0;
			}
			if( sndk->m_sndenum > 0 ){
				m_snde = *( sndk->m_psnde + m_elemindex );
			}else{
				m_snde = m_zerosnde;
			}
		}
	}

	ParamsToDlg();

	return 0;
}

int CSndKeyDlg::ParamsToDlg()
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

	SetSyncButton();


	int ret;
	ret = InitList();
	_ASSERT( !ret );


//m_sl_mp_
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

//bgm
	CSoundSet* curss = GetCurSS();
	if( curss ){
		strcpy_s( tempchar, 1024, curss->m_bgm->m_filename );
		if( (g_endapp == 1) || !m_bgm_wnd.IsWindow() )
			return 0;
		m_bgm_wnd.SetWindowText( tempchar );
	}

	return 0;
}

LRESULT CSndKeyDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}

LRESULT CSndKeyDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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


	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( m_sndah->m_animnum <= 0 ){
		return 0;
	}


	SetCurrentMotion( 0 );

	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( anim ){
		CSndKey* sndk = 0;
		ret = anim->ExistSndKey( m_current, &sndk );
		if( sndk ){
			if( sndk->m_sndenum > 0 ){
				if( m_elemindex >= sndk->m_sndenum ){
					m_elemindex = 0;
				}
				m_snde = *( sndk->m_psnde + m_elemindex );
			}
		}
	}

	Redraw();

	if( (idCtrl != 999) && (m_app->m_syncdlg->m_sndflag != 0) ){
		int quaflag = m_app->m_syncdlg->m_quaflag;
		int camflag = m_app->m_syncdlg->m_camflag;
		int gpaflag = m_app->m_syncdlg->m_gpaflag;
		int moeflag = m_app->m_syncdlg->m_moeflag;

		g_motdlg->OnJumpFrame( m_current, quaflag, camflag, gpaflag, moeflag, 0 );
	}


	return 0;
}
LRESULT CSndKeyDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	
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

	if( m_sndah->m_animnum <= 0 ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!
	}

	ParamsToDlg();
	Redraw();

	return 0;

}
void CSndKeyDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
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


int CSndKeyDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
	*posptr = MP_SIZE + (int)(step * (float)( frameno - m_startframe ) ) + clirect.left;
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	
	return 0;
}

int CSndKeyDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
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

int CSndKeyDlg::DrawSndKeys()
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
	if( !m_sndah ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	CSndKey* cursndk = anim->GetFirstSndKey();
	while( cursndk ){
		int frameno = cursndk->m_frameno;
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

		cursndk = cursndk->next;
	}
	
	if( m_dragsndk ){
		int frameno = m_dragsndk->m_frameno;
		
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


int CSndKeyDlg::Redraw()
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
//		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}

LRESULT CSndKeyDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	
	DrawSndKeys();

	return 0;
}

LRESULT CSndKeyDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CSndKeyDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "SndKeyDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CSndKeyDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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




LRESULT CSndKeyDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 ){
		return 0;
	}

	if( m_sndah->m_animnum <= 0 ){
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


LRESULT CSndKeyDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( m_sndah->m_animnum <= 0 ){
		return 0;
	}

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );


	BOOL dummyb;
	return m_dlg->OnPreview( 0, 0, 0, dummyb );

}

int CSndKeyDlg::CaptureM()
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


LRESULT CSndKeyDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	CaptureM();

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "SndKeyDlg : OnMouseMove : MK_LBUTTON\n" );

	if( (m_dragflag == 1) && m_dragsndk ){
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

		m_dragsndk->m_frameno = frameno;

		Redraw();
	}

	CSndAnim* anim;
	int animindex;
	anim = m_sndah->GetSndAnim( m_animno, &animindex );
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


LRESULT CSndKeyDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		return 0;
	}

	int ret;
	CSndKey* sndkptr = 0;
	int existflag = 0;

	int cpok = 0;
	ret = anim->ExistSndKey( m_current, &sndkptr );
	_ASSERT( !ret );
	if( ret || !sndkptr ){
		cpok = 0;
	}else{
		cpok = 1;
	}

	if( !cpok ){
		::MessageBox( m_hWnd, "このフレームにはコピーできるキーがありません。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!
	}

	if( m_cpsndk ){
		delete m_cpsndk;
		m_cpsndk = 0;
	}

	m_cpsndk = new CSndKey();
	if( !m_cpsndk ){
		DbgOut( "SndKeyDlg : OnFrameCopy : cpsndk alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_cpsndk->CopySndElem( sndkptr );
	if( ret ){
		DbgOut( "sndkydlg : OnFrameCopy : sndk CopySndElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CSndKeyDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		return 0;
	}

	if( !m_cpsndk ){
		::MessageBox( m_hWnd, "フレームコピーを行ってから再試行してください。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!!!!
	}

	int ret;

	ret = anim->SetSndKey( m_current, m_cpsndk );
	_ASSERT( !ret );

	if( m_dlg->m_shandler && m_dlg->m_mhandler ){
		CSoundSet* curss = GetCurSS();
		if( curss ){
			ret = anim->CheckAndDelInvalid( curss );
			_ASSERT( !ret );
			ret = anim->SetFrameData();
			_ASSERT( !ret );
		}else{
			_ASSERT( 0 );
		}
	}else{
		_ASSERT( 0 );
	}
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();
	return 0;
}

LRESULT CSndKeyDlg::OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		return 0;
	}

	int ret;
	ret = anim->DeleteSndKey( m_current );
	if( ret ){
		DbgOut( "SndKeyDlg : OnDelete : ca DeleteSndKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_dlg->m_shandler && m_dlg->m_mhandler ){
		CSoundSet* curss = GetCurSS();
		if( curss ){
			ret = anim->CheckAndDelInvalid( curss );
			_ASSERT( !ret );
			ret = anim->SetFrameData();
			_ASSERT( !ret );
		}else{
			_ASSERT( 0 );
		}
	}else{
		_ASSERT( 0 );
	}
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CSndKeyDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		return 0;
	}

	int ret;
	CSndKey* cursndk = anim->GetFirstSndKey();
	CSndKey* nextsndk = 0;
	while( cursndk ){
		nextsndk = cursndk->next;

		ret = anim->DeleteSndKey( cursndk->m_frameno );
		if( ret ){
			DbgOut( "SndKeyDlg : OnAllDelete : ca DeleteSndKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		cursndk = nextsndk;
	}

	if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
		ret = anim->SetFrameData();
		_ASSERT( !ret );
	}else{
		_ASSERT( 0 );
	}
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CSndKeyDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( m_sndah->m_animnum <= 0 ){
		return 0;
	}


	if( m_dragflag )
		return 0;

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
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

			CSndKey* sndkptr = 0;
			ret = anim->ExistSndKey( frameno, &sndkptr );
			if( !ret && sndkptr ){
				m_dragframe = frameno;
				m_dragflag = 1;
				m_dragsndk = sndkptr;

				anim->LeaveFromChainSndKey( frameno );

			}else{
				m_dragframe = 0;
				m_dragflag = 0;
				m_dragsndk = 0;
			}
		}else{
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragsndk = 0;
		}
	}

	return 0;
}


LRESULT CSndKeyDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	static int busyflag = 0;


	if( busyflag == 1 )
		return 0;

	busyflag = 1;

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		busyflag = 0;
		return 0;
	}

	int ret;
	int frameno = -1;

	if( m_ctrldrag || m_altdrag ){
		m_ctrldrag = 0;
		m_altdrag = 0;
		m_dragx = 0;
	}else{


		if( (m_dragflag == 1) && m_dragsndk ){
			CSndKey* isexist = 0;

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

			ret = anim->ExistSndKey( frameno, &isexist );
			_ASSERT( !ret );
			if( isexist ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "フレーム番号%d には既にキーフレームが存在するので、移動できません。",
					frameno );
				MessageBox( messtr, "移動できません", MB_OK );
				frameno = m_dragframe;//!!!!! 元に戻す
			}
			
			m_dragsndk->m_frameno = frameno;//!!!!!!!!!!!!!!!!

			ret = anim->ChainSndKey( m_dragsndk );
			if( ret ){
				DbgOut( "SndKeyDlg : OnLButtonUp : ca ChainSndKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_dragsndk = 0;

			if( m_dlg->m_shandler && m_dlg->m_mhandler ){
				CSoundSet* curss = GetCurSS();
				if( curss ){
					ret = anim->CheckAndDelInvalid( curss );
					_ASSERT( !ret );
					ret = anim->SetFrameData();
					_ASSERT( !ret );
				}else{
					_ASSERT( 0 );
				}
			}else{
				_ASSERT( 0 );
			}
			SetCurrentMotion( 0 );

			ParamsToDlg();
			Redraw();
		}
	}

	m_dragflag = 0;
	m_dragframe = 0;
	m_dragsndk = 0;

	busyflag = 0;

	return 0;
}
LRESULT CALLBACK MPProcSnd(
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
		//	DbgOut( "SndKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "SndKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
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
int CSndKeyDlg::SetNewWindowProc()
{
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWL_WNDPROC );
	if( !s_mpfunc ){
		_ASSERT( 0 );
		return 1;
	}

	LONG lret;
	lret = ::SetWindowLong( m_sl_mp_wnd, GWL_WNDPROC, (LONG)MPProcSnd );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}


LRESULT CSndKeyDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		
	bHandled = TRUE;
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;

	int timerid = (int)wParam;
	if( (timerid == SndKeyDlgTIMER) && (m_cmdshow == SW_SHOW) ){

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


int CSndKeyDlg::ShowDlg( int cmdshow, int stopflag )
{
	m_cmdshow = cmdshow;

	m_elemindex = -1;
	m_snde = m_zerosnde;

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

		CalcFrameData();

		ParamsToDlg();

		Sleep(100);//OnStop後

		g_inidata->opt_bmdisp = 0;
		g_inidata->opt_jointdisp = 0;

	}


	return 0;
}

LRESULT CSndKeyDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}


	int findframe = -1;
	int chkframe;

	CSndAnim* anim;
	CSndFrame* sframe = 0;
	anim = m_sndah->GetSndAnim( m_animno );
	if( anim && (m_current - 1) > m_startframe ){
		for( chkframe = (m_current - 1); chkframe >= m_startframe; chkframe-- ){
			sframe = anim->m_fullframe->GetFrame( chkframe );
			if( sframe && (sframe->m_keyflag != 0) ){
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
LRESULT CSndKeyDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	int findframe = -1;
	int chkframe;

	CSndAnim* anim;
	CSndFrame* sframe = 0;
	anim = m_sndah->GetSndAnim( m_animno );
	if( anim && (m_current + 1) < m_endframe ){
		for( chkframe = (m_current + 1); chkframe <= m_endframe; chkframe++ ){
			sframe = anim->m_fullframe->GetFrame( chkframe );
			if( sframe && (sframe->m_keyflag != 0) ){
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

LRESULT CSndKeyDlg::OnCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( m_sndah->m_animnum <= 0 ){
		return 0;
	}
	int ret;
	//int tmpint;
	//char mes[512];


	if( m_sscomboindex < 0 ){
		::MessageBox( m_hWnd, "音セットが未定義です。音セットを作成し指定してから再試行してください。", "エラー", MB_OK );
		return 0;
	}

	int combono;
	combono = (int)m_secombo_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		::MessageBox( m_hWnd, "効果音が未定義です。", "エラー", MB_OK );
		return 0;
	}

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	m_snde = m_zerosnde;
	CSoundSet* curss = *( m_app->m_soundbank->m_ppset + m_sscomboindex );
	CSoundElem* curse = *( curss->m_ppsound + combono );
	strcpy_s( m_snde.sndsetname, 32, curss->m_setname );
	strcpy_s( m_snde.sndname, 256, curse->m_filename );

	ret = anim->AddSndE( m_current, &m_snde );
	if( ret ){
		DbgOut( "SndKeyDlg : OnCreate : anim AddSndE error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_dlg->m_shandler && m_dlg->m_mhandler ){
		CSoundSet* curss = GetCurSS();
		if( curss ){
			ret = anim->CheckAndDelInvalid( curss );
			_ASSERT( !ret );
			ret = anim->SetFrameData();
			_ASSERT( !ret );
		}else{
			_ASSERT( 0 );
		}
	}else{
		_ASSERT( 0 );
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

//	_ASSERT( 0 );

	return 0;
}

LRESULT CSndKeyDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CSndKeyDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}

int CSndKeyDlg::SetCurrentMotion( int isfirst )
{
	int ret;
	if( m_animno < 0 )
		return 0;

	ret = m_sndah->SetAnimFrameNo( m_animno, m_current );
	_ASSERT( !ret );
	//m_snde = m_sndah->m_cursnde;
	m_elemindex = 0;//!!!!!!!!!!

	return 0;
}
int CSndKeyDlg::SetFrameNo( int srcframeno )
{
	int ret;
	if( m_animno < 0 )
		return 0;

	int setframe;
	if( srcframeno < 0 ){
		setframe = 0;
	}else if( srcframeno > m_sl_mp_rangemax ){
		setframe = m_sl_mp_rangemax;
	}else{
		setframe = srcframeno;
	}

	ret = m_sndah->SetAnimFrameNo( m_animno, setframe );
	_ASSERT( !ret );
	//m_snde = m_sndah->m_cursnde;
	m_elemindex = 0;//!!!!!!!!!!

	return 0;
}

int CSndKeyDlg::ResetParams( int atshowflag )
{
	
	int ret;

	DestroyObjs( 0 );

	ret = InitDlg( atshowflag );
	if( ret ){
		DbgOut( "SndKeydlg : ResetParams : InitDlg error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( anim ){
		CSoundSet* curss = 0;
		ret = m_app->m_soundbank->GetSoundSet( anim->m_ssname, &curss );
		if( ret || !curss ){
			m_sscomboindex = -1;
		}else{
			ret = m_app->m_soundbank->GetSoundSetIndexByID( curss->m_setid, &m_sscomboindex );
			if( ret ){
				m_sscomboindex = -1;
				_ASSERT( 0 );
			}
		}
	}else{
		m_sscomboindex = -1;
		_ASSERT( 0 );
	}


	ret = InitComboSS();
	if( ret ){
		DbgOut( "SndKeydlg : ResetParams : InitCombo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = InitComboSE();
	if( ret ){
		DbgOut( "SndKeydlg : ResetParams : InitCombo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	SetCurrentMotion( 0 );


	return 0;
}


LRESULT CSndKeyDlg::OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}


	CSndAnim* anim;
	int animindex;
	anim = m_sndah->GetSndAnim( m_animno, &animindex );
	if( !anim ){
		return 0;
	}

	int ret = 0;
	int dlgret;
	
	int mottype = 0;
	if( anim->m_bgmloop == 0 ){
		mottype = MOTION_CLAMP;
	}else{
		mottype = MOTION_ROUND;
	}

	CSndSettingDlg	dlg( anim->m_animname, m_sl_mp_rangemax + 1, mottype, m_startframe, m_endframe, 1 );
	::SendMessage( m_dlg->m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	dlgret = (int)dlg.DoModal();
	::SendMessage( m_dlg->m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( dlgret == IDOK ){
		if( dlg.m_mottype == MOTION_CLAMP ){
			anim->m_bgmloop = 0;
		}else{
			anim->m_bgmloop = 255;
		}

		int savestart = m_startframe;
		int saveend = m_endframe;

		m_startframe = dlg.m_startframe;
		m_endframe = dlg.m_endframe;

		if( (m_startframe != savestart) || (m_endframe != saveend) ){
			m_current = m_startframe;
			InterlockedExchange( &m_previewframe, m_startframe );

			m_motid[ animindex ].startframe = m_startframe;
			m_motid[ animindex ].endframe = m_endframe;

		}

		int cmp;
		cmp = strcmp( anim->m_animname, dlg.m_name );
		if( cmp != 0 ){
			strcpy_s( anim->m_animname, 256, dlg.m_name );
		}

		CalcFrameData();
		SetCurrentMotion( 0 );

		ParamsToDlg();
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
		OnReleasedcaptureSl_mp( 0, 0, dummy );
//		_ASSERT( 0 );
	}

//	::SendMessage( m_dlg->m_apphwnd, WM_USER_SNDMENU, animindex, 0 );

	return 0;
}



int CSndKeyDlg::SetPreviewButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_dlg->m_preview_flag ){
		::SendMessage( m_preview_wnd, BM_SETSTATE, TRUE, 0 );  
	}else{
		::SendMessage( m_preview_wnd, BM_SETSTATE, FALSE, 0 );  
	}

	return 0;
}


int CSndKeyDlg::GetSndAnimNum()
{
	if( !IsWindow() )
		return 0;

	if( !m_sndah ){
		return 0;
	}

	int retnum;
	retnum = m_sndah->m_animnum;
	return retnum;
}

/***
int CSndKeyDlg::LoadSndFile( char* filename )
{
	int ret;
	if( !m_sndah ){
		_ASSERT( 0 );
		return 0;
	}

	CSndFile sndfile;
	int animno = -1;
	ret = sndfile.LoadSndFile( m_dlg->m_thandler, m_dlg->m_shandler, filename, m_sndah, &animno );
	if( ret ){
		DbgOut( "SndKeyDlg : LoadSndFile : sndfile LoadSndFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_sndah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "SndKeyDlg : LoadSndFile : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_animno = animno;
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		DbgOut( "SndKeyDlg : LoadSndFile : cah GetSndAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_sndah->SetCurrentAnim( m_animno );
	if( ret ){
		DbgOut( "SndKeyDlg : LoadSndFile : cah SetCurrentAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
	if( ret ){
		DbgOut( "SndKeyDlg : LoadSndFile : anim CalcFrameData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	m_motid[ m_sndah->m_animnum - 1 ].id = m_animno;
	char* enptr = 0;
	enptr = strrchr( filename, '\\' );
	if( enptr ){
		strcpy_s( m_motid[ m_sndah->m_animnum - 1 ].filename, 256, enptr + 1 );
	}else{
		strcpy_s( m_motid[ m_sndah->m_animnum - 1 ].filename, 256, filename );
	}

	m_sl_mp_rangemax = anim->m_maxframe;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;

	m_motid[ m_sndah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_sndah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	::SendMessage( m_dlg->m_apphwnd, WM_USER_SNDMENU, m_sndah->m_animnum - 1, 0 );
	
	return 0;
}


MOTID* CSndKeyDlg::GetSndMotIDArray()
{
	return m_motid;
}
char* CSndKeyDlg::GetSndAnimName( int animno )
{
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( animno );
	if( !anim ){
		return 0;
	}
	return anim->m_animname;
}
int CSndKeyDlg::GetCurSndAnimIndex()
{
	CSndAnim* anim;
	int animindex = 0;
	anim = m_sndah->GetSndAnim( m_animno, &animindex );
	if( anim ){
		return animindex;
	}else{
		return 0;
	}
}
int CSndKeyDlg::GetCurSndAnimNo()
{
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( anim ){
		return anim->m_motkind;
	}else{
		return 0;
	}
}

int CSndKeyDlg::SelectSndAnim( int srcindex )
{


//これを有効にするときはOnSSComboでm_motid[]をセット必要


	if( !m_sndah ){
		_ASSERT( 0 );
		return 0;
	}
	if( m_dlg->m_preview_flag ){
		return 0;
	}

	if( (srcindex < 0) || (srcindex >= m_sndah->m_animnum) ){
		DbgOut( "SndKeyDlg : SelectSndAnim : srcindex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	int tmpcookie;
	tmpcookie = m_motid[ srcindex ].id;
	if( tmpcookie < 0 ){
		DbgOut( "SndKeyDlg : SelectSndAnim : tmpcookie error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

/////////
	CSndAnim* anim;
	anim = m_sndah->GetSndAnimByIndex( srcindex );
	if( !anim ){
		DbgOut( "SndKeyDlg : SelectSndAnim : cah GetSndAnimByIndex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	int ret;
	ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
	if( ret ){
		DbgOut( "SndKeyDlg : SelectSndAnim : anim CalcFrameData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//これを有効にするときはOnSSComboでm_motid[]をセット必要

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
	::SendMessage( m_dlg->m_apphwnd, WM_USER_SNDMENU, srcindex, 0 );


	return 0;
}


int CSndKeyDlg::DestroySndAnim( int delanimno )
{
	int ret;

	if( delanimno < 0 ){
		if( m_sndah ){
			delete m_sndah;
			m_sndah = 0;
		}

		ret = InitDlg( 0 );
		if( ret ){
			DbgOut( "SndKeyDlg : DestroySndAnim all : InitDlg error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{
		_ASSERT( m_sndah );
		if( m_sndah->m_animnum <= 0 ){
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

		if( setno != (m_sndah->m_animnum - 1) ){
			_ASSERT( 0 );
			return 1;
		}

		MoveMemory( m_motid, tempmotid, sizeof( MOTID ) * MAXMOTIONNUM );

		//m_motionnum -= 1;//!!!!!

		int ret;
		ret = m_sndah->DestroyAnim( delcookie );
		if( ret ){
			DbgOut( "SndKeyDlg : DestroySndAnim cur : cah DestroyAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( m_sndah->m_animnum <= 0 ){
			ret = InitDlg( 0 );
			if( ret ){
				DbgOut( "SndKeyDlg : DestroySndAnim cur : InitDlg error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = SelectSndAnim( 0 );
		if( ret ){
			DbgOut( "SndKeyDlg : DestroySndAnim cur : SelectSndAnim 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}
***/
/***
int CSndKeyDlg::CalcMotionPointOnFrame( int animno, int frameno, CMState** ppms, int* hasmpflag )
{
	int ret;
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( animno );
	if( !anim ){
		*ppms = 0;
		*hasmpflag = 0;
		return 0;
	}


	ret = anim->GetSndAnim( ppms, frameno );
	if( ret ){
		DbgOut( "SndKeyDlg : CalcMotionPointOnFrame : ca GetSndAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	*hasmpflag = (*ppms)->m_keyflag;

	return 0;
}
***/

int CSndKeyDlg::ExistSndKey( int animno, int frameno, CSndKey** ppsndk )
{
	*ppsndk = 0;

	int ret;
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( animno );
	if( !anim ){
		return 0;
	}

	ret = anim->ExistSndKey( frameno, ppsndk );
	if( ret ){
		DbgOut( "SndKeyDlg : ExistSndKey : ca ExistSndKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


LRESULT CSndKeyDlg::OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	if( m_sndah->m_animnum <= 0 ){
		return 0;
	}
	int ret;
	ret = CalcFrameData();
	if( ret ){
		DbgOut( "sndkeydlg : OnCalc : CalcFrameData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

int CSndKeyDlg::CalcFrameData()
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( !m_sndah ){
		return 0;
	}


	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}


	int ret;
	if( m_dlg->m_shandler && m_dlg->m_mhandler ){
		CSoundSet* curss = GetCurSS();
		if( curss ){
			ret = anim->CheckAndDelInvalid( curss );
			_ASSERT( !ret );

			ret = anim->SetFrameData();
			_ASSERT( !ret );
		}else{
			_ASSERT( 0 );
		}
	}else{
		_ASSERT( 0 );
	}

	return 0;
}


LRESULT CSndKeyDlg::OnAddElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//int ret;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( m_sndah->m_animnum <= 0 ){
		return 0;
	}

	BOOL dummy;
	LRESULT lret;
	lret = OnCreate( 0, 0, 0, dummy );
	if( lret ){
		DbgOut( "sndkeydlg : OnAddElem : OnCreate error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CSndKeyDlg::OnDelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_sndah->m_animnum <= 0 ){
		return 0;
	}

	int selelem;
	selelem = (int)m_elemlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
//	if( (selelem < 0) || (selelem == LB_ERR) ){
//		return 0;
//	}

	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		return 0;
	}
	CSndKey* sndk = 0;
	ret = anim->ExistSndKey( m_current, &sndk );
	if( ret || !sndk ){
		return 0;
	}
	if( (selelem >= 0) && (selelem != LB_ERR) ){
		ret = sndk->DeleteSndE( selelem );
		if( ret ){
			DbgOut( "sndkeydlg : OnDelElem: sndk DeleteSndE error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	if( sndk->m_sndenum <= 0 ){
		ret = anim->DeleteSndKey( m_current );
		if( ret ){
			DbgOut( "sndkeydlg : OnDelElem : anim DeleteSndKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	m_elemindex = 0;//!!!!!!!!

	if( m_dlg->m_shandler && m_dlg->m_mhandler ){
		CSoundSet* curss = GetCurSS();
		if( curss ){
			ret = anim->CheckAndDelInvalid( curss );
			_ASSERT( !ret );
			ret = anim->SetFrameData();
			_ASSERT( !ret );
		}else{
			_ASSERT( 0 );
		}
	}else{
		_ASSERT( 0 );
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

int CSndKeyDlg::RemakeSndAH()
{
	if( m_sndah ){
		delete m_sndah;
		m_sndah = 0;
	}

	m_sndah = new CSndAnimHandler();
	if( !m_sndah ){
		DbgOut( "sndkeydlg : RemakeSndAH sndah malloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_animno = 0;
	return 0;
}

LRESULT CSndKeyDlg::OnSSCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int combono;
	combono = (int)m_sscombo_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		m_sscomboindex = -1;
		return 0;
	}else{
		m_sscomboindex = combono;
	}

	if( !m_sndah ){
		return 0;
	}
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		m_sscomboindex = -1;
		_ASSERT( 0 );
		return 0;
	}

	if( m_sscomboindex >= m_app->m_soundbank->m_setnum ){
		_ASSERT( 0 );
		m_sscomboindex = 0;
	}

	CSoundSet* curss;
	curss = *( m_app->m_soundbank->m_ppset + m_sscomboindex );

	if( curss ){
		strcpy_s( anim->m_ssname, 32, curss->m_setname );
		strcpy_s( m_curssname, 32, curss->m_setname );

		double totaltime;
		curss->GetBGMTotalTime( &totaltime );
		double calcframe;
		if( m_app && m_app->m_syncdlg ){
			calcframe = (double)m_app->m_syncdlg->m_fps * totaltime;
		}else{
			return 0;//!!!!!!!!!!!!!!!!!!!
		}
		int setframe = (int)( calcframe + 0.5 ) + 1;

		ret = m_sndah->SetTotalFrame( anim->m_motkind, setframe );
		if( ret ){
			DbgOut( "sndkeydlg : OnSSCombo : sndah SetTotalFrame error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		m_sl_mp_rangemax = anim->m_maxframe;//!!!!!!!!!!!!
		m_startframe = 0;
		m_endframe = m_sl_mp_rangemax;
		m_current = m_startframe;

		m_motid[0].startframe = m_startframe;
		m_motid[0].endframe = m_endframe;

		CalcFrameData();
	}else{
		m_sscomboindex = -1;
		_ASSERT( 0 );
	}

	ret = InitComboSE();
	_ASSERT( !ret );

	ParamsToDlg();

	return 0;
}


int CSndKeyDlg::OnJumpFrame( int srcframe )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CSndAnim* anim;
	int animindex;
	anim = m_sndah->GetSndAnim( m_animno, &animindex );
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

int CSndKeyDlg::SetStartAndEnd( int srcstart, int srcend )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CSndAnim* anim;
	int animindex;
	anim = m_sndah->GetSndAnim( m_animno, &animindex );
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

int CSndKeyDlg::ChangeFPS( int srcfps )
{
	if( !m_sndah || !m_app || !IsWindow() ){
		return 0;
	}

	int ret;
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		m_sscomboindex = -1;
		//_ASSERT( 0 );
		return 0;
	}

	if( (m_sscomboindex < 0) || (m_sscomboindex >= m_app->m_soundbank->m_setnum) ){
		_ASSERT( 0 );
		return 0;
	}

	if( anim->m_fps == srcfps ){
		return 0;
	}
	
	CSoundSet* curss;
	curss = *( m_app->m_soundbank->m_ppset + m_sscomboindex );

	if( curss ){
		double totaltime;
		curss->GetBGMTotalTime( &totaltime );
		double calcframe;
		calcframe = (double)m_app->m_syncdlg->m_fps * totaltime;
		int setframe = (int)( calcframe + 0.5 ) + 1;

		if( setframe != (m_sl_mp_rangemax + 1) ){
			ret = m_sndah->SetTotalFrame( anim->m_motkind, setframe );
			if( ret ){
				DbgOut( "sndkeydlg : OnSSCombo : sndah SetTotalFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			anim->m_fps = m_app->m_syncdlg->m_fps;//!!!!!!!!!!

			m_sl_mp_rangemax = anim->m_maxframe;//!!!!!!!!!!!!
			m_startframe = 0;
			m_endframe = m_sl_mp_rangemax;
			m_current = m_startframe;

			m_motid[0].startframe = m_startframe;
			m_motid[0].endframe = m_endframe;

			CalcFrameData();
		}
	}else{
		m_sscomboindex = -1;
		_ASSERT( 0 );
	}

	ParamsToDlg();

	return 0;
}

int CSndKeyDlg::WriteSndAnimFile( char* filename )
{
	if( !m_sndah || !m_app || !IsWindow() ){
		return 0;
	}

	int ret;
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		m_sscomboindex = -1;
		return 0;
	}

	if( (m_sscomboindex < 0) || (m_sscomboindex >= m_app->m_soundbank->m_setnum) ){
		return 0;
	}
	
	CSoundSet* curss;
	curss = *( m_app->m_soundbank->m_ppset + m_sscomboindex );

	CSndFile sndfile;
	ret = sndfile.WriteSndFile( filename, anim, curss );
	if( ret ){
		DbgOut( "sndkeydlg : WriteSndAnimFile : sndfile WriteSndFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	return 0;
}
int CSndKeyDlg::LoadSndAnimFile( char* filename, int dlgwarnflag )
{
	int ret;

	if( dlgwarnflag == 1 ){
		ret = ::MessageBox( m_hWnd, "編集中の音アニメは破棄されます。よろしいですか？", "確認", MB_OKCANCEL );
		if( ret != IDOK ){
			return 0;
		}
	}

	ret = RemakeSndAH();
	_ASSERT( !ret );

	CSndFile sndfile;
	CSndAnim* anim = 0;
	ret = sndfile.LoadSndFile( filename, m_sndah, m_app->m_soundbank, &anim );
	if( ret ){
		DbgOut( "SndKeyDlg : LoadSndFile : sndfile LoadSndFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_animno = anim->m_motkind;

	CSoundSet* curss = 0;
	ret = m_app->m_soundbank->GetSoundSet( anim->m_ssname, &curss );
	if( ret || !curss ){
		DbgOut( "SndKeyDlg : LoadSndFile : sb GetSoundBank error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_app->m_soundbank->GetSoundSetIndexByID( curss->m_setid, &m_sscomboindex );
	if( ret || (m_sscomboindex < 0) ){
		DbgOut( "SndKeyDlg : LoadSndFile : sb GetSoundSetIndexByID error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	if( m_sndah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "SndKeyDlg : LoadSndFile : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_app->m_syncdlg->m_fps = anim->m_fps;
	m_app->m_syncdlg->ParamsToDlg();


	ret = m_sndah->SetCurrentAnim( m_animno );
	if( ret ){
		DbgOut( "SndKeyDlg : LoadSndFile : cah SetCurrentAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = CalcFrameData();
	if( ret ){
		DbgOut( "SndKeyDlg : LoadSndFile : CalcFrameData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	m_motid[ m_sndah->m_animnum - 1 ].id = m_animno;
	char* enptr = 0;
	enptr = strrchr( filename, '\\' );
	if( enptr ){
		strcpy_s( m_motid[ m_sndah->m_animnum - 1 ].filename, 256, enptr + 1 );
	}else{
		strcpy_s( m_motid[ m_sndah->m_animnum - 1 ].filename, 256, filename );
	}

	m_sl_mp_rangemax = anim->m_maxframe;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;

	m_motid[ m_sndah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_sndah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

//	::SendMessage( m_dlg->m_apphwnd, WM_USER_SNDMENU, m_sndah->m_animnum - 1, 0 );

	return 0;
}

int CSndKeyDlg::GetSndFrame( int befstartcnt, int* dstframe, int* dstdiff )
{
	int ret;

	*dstframe = 0;

	CSoundSet* curss = GetCurSS();
	if( !curss ){
		_ASSERT( 0 );
		return 1;
	}

	int curstartcnt = 0;
	ret = m_app->m_HS->GetStartCnt( curss->m_bgm->m_serialno, &curstartcnt );
	_ASSERT( !ret );

	int curframe = 0;

	if( curstartcnt <= befstartcnt ){
		curframe = 0;//再生が始まっていないので最初のフレームで待機。
	}else{
		double totaltime;
		curss->GetBGMTotalTime( &totaltime );
		double totalsample;
		ret = SCConvTime2Sample( curss, totaltime, &totalsample );
		_ASSERT( !ret );

		double cursample = 0.0;
		ret = curss->m_bgm->GetSamplesPlayed( &cursample );
		_ASSERT( !ret );

		ret = SCConvSample2Frame( curss, m_app->m_syncdlg->m_fps, cursample, &curframe );
		_ASSERT( !ret );
	}

	*dstframe = curframe + m_startframe ;
	*dstdiff = curframe;
	return 0;
}

int CSndKeyDlg::GetBGMStopCnt( int* cntptr )
{
	*cntptr = 0;

	if( !m_app ){
		return 0;
	}
	if( !m_app->m_HS ){
		return 0;
	}

	CSoundSet* curss = GetCurSS();
	if( !curss ){
		return 0;
	}

	int ret;
	ret = m_app->m_HS->GetStopCnt( curss->m_bgm->m_serialno, cntptr );
	_ASSERT( !ret );

	return 0;
}
int CSndKeyDlg::GetBGMStartCnt( int* cntptr )
{
	*cntptr = 0;

	if( !m_app ){
		return 0;
	}
	if( !m_app->m_HS ){
		return 0;
	}

	CSoundSet* curss = GetCurSS();
	if( !curss ){
		return 0;
	}

	int ret;
	ret = m_app->m_HS->GetStartCnt( curss->m_bgm->m_serialno, cntptr );
	_ASSERT( !ret );

	return 0;
}

int CSndKeyDlg::StartPreview( int srcframe )
{
	int ret;

	InterlockedExchange( &m_current, (LONG)( srcframe ) );

	CSoundSet* curss = GetCurSS();
	if( !curss ){
		_ASSERT( 0 );
		return 1;
	}
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 1;
	}

	ret = anim->ResetDoneFlag( m_current );
	_ASSERT( !ret );

	ret = anim->SetSndAnimFrameNo( m_current );
	_ASSERT( !ret );

	CSndFrame* curframe = anim->m_curframe;

	double cursample = 0.0;
	ret = SCConvFrame2Sample( curss, m_app->m_syncdlg->m_fps, m_current, &cursample );
	_ASSERT( !ret );

	ret = m_app->m_HS->PlayFrame( curss->m_bgm->m_serialno, (int)cursample, 0, curframe );
	_ASSERT( !ret );

//DbgOut( "checkframe!!! sndkeydlg : StartPreview : m_current %d, cursample %f, isamp %d\r\n",
//		m_current, cursample, (int)cursample );

	return 0;
}

int CSndKeyDlg::PlayAnim( int srcframe )
{
	int ret;

	CSoundSet* curss = GetCurSS();
	if( !curss ){
		_ASSERT( 0 );
		return 1;
	}
	CSndAnim* anim;
	anim = m_sndah->GetSndAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 1;
	}

	int frameno;
	frameno = srcframe;
	frameno = min( frameno, m_endframe );
	frameno = max( frameno, m_startframe );

	InterlockedExchange( &m_current, (LONG)frameno );

	ret = anim->SetSndAnimFrameNo( m_current );
	_ASSERT( !ret );

	CSndFrame* curframe = anim->m_curframe;

	ret = m_app->m_HS->PlayFrame( -1, 0, 0, curframe );
	_ASSERT( !ret );

	ParamsToDlg();

//DbgOut( "check !!! sndkeydlg : PlayAnim : m_current %d\r\n", m_current );

	return 0;
}


int CSndKeyDlg::StopBGM()
{
	int ret;

	CSoundSet* curss = GetCurSS();
	if( !curss ){
		return 0;
	}

	ret = m_app->m_HS->StopSound( curss->m_bgm->m_serialno );
	_ASSERT( !ret );

//	m_dlg->m_sndframe = m_startframe;

	Sleep( 50 );

	return 0;
}

LRESULT CSndKeyDlg::OnSyncFlag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
/***
	if( m_dlg->m_preview_flag != 0 ){
		return 0;
	}

	int tmpflag;
	if( m_syncflag == 1 ){
		tmpflag = 0;
	}else{
		tmpflag = 1;
	}
	m_syncflag = tmpflag;

	SetSyncButton();
***/
	return 0;
}
int CSndKeyDlg::SetSyncButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_syncflag == 1 ){
		::SendMessage( m_sndsync_wnd, BM_SETSTATE, TRUE, 0 );  
	}else{
		::SendMessage( m_sndsync_wnd, BM_SETSTATE, FALSE, 0 );  
	}

	return 0;
}