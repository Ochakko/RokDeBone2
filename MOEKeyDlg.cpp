// MOEKeyDlg.cpp : CMOEKeyDlg のインプリメンテーション
#include "stdafx.h"
#include "MOEKeyDlg.h"

#include <math.h>
#include <stdio.h>
//#include <D3DX9.h>

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

#include <MOEAnimHandler.h>
#include <MOEAnim.h>
#include <MOEKey.h>
#include <MOEEHelper.h>
#include <MState.h>
#include <MOEFile.h>

#include <MCHandler.h>

#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "viewer.h"
#include "MotDlg.h"
#include "MotParamDlg.h"
#include "MotSettingDlg.h"

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
// CMOEKeyDlg

CMOEKeyDlg::CMOEKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp )
//: m_list_wnd( NULL, this, 2 )
{
	InitParams();

	m_dlg = srcmpdlg;
	m_dlg->m_moekeydlg = this;
	m_app = srcapp;
}

int CMOEKeyDlg::InitParams()
{
	m_moeah = 0;
	m_dlg = 0;
	m_app = 0;

	m_current = 0;
	m_ismoving = 0;

	m_sl_mp_rangemax = 59;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_previewframe = 0;

	m_dragmoek = 0;
	m_dragframe = 0;
	m_dragflag = 0;
	m_cpmoek = 0;

	m_animno = 0;

	ZeroMemory( &m_moee, sizeof( MOEELEM ) );
	m_moee.eventno = 0;
	m_moee.listnum = 1;
	m_moee.list[0] = -1;
	m_moee.notlistnum = 0;

	ZeroMemory( &m_zeromoee, sizeof( MOEELEM ) );

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

	m_bone2combo = 0;
	m_combo2bone = 0;
	m_indexleng = 0;
	m_jointnum = 0;

	m_elemindex = 0;

	m_savebmdisp = g_inidata->opt_bmdisp;
	m_savejdisp = g_inidata->opt_jointdisp;

	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

	return 0;
}


CMOEKeyDlg::~CMOEKeyDlg()
{
	DestroyObjs( 1 );
}

int CMOEKeyDlg::DestroyObjs( int delmoeah )
{
	if( m_cpmoek ){
		delete m_cpmoek;
		m_cpmoek = 0;
	}

	if( delmoeah ){
		if( m_moeah ){
			delete m_moeah;
			m_moeah = 0;
		}
	}

	if( m_bone2combo ){
		free( m_bone2combo );
		m_bone2combo = 0;
	}
	if( m_combo2bone ){
		free( m_combo2bone );
		m_combo2bone = 0;
	}

	return 0;
}

int CMOEKeyDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, MOEKeyDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	return 0;
}

LRESULT CMOEKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	if( m_moeah ){
		delete m_moeah;
		m_moeah = 0;
	}
	m_moeah = new CMOEAnimHandler();
	_ASSERT( m_moeah );


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
	timerid = (int)::SetTimer( m_hWnd, MOEKeyDlgTIMER, 500, NULL );
	_ASSERT( timerid == MOEKeyDlgTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}


int CMOEKeyDlg::InitDlg( int atshowflag )
{
	int ret;	
	DbgOut( "MOEKeyDlg : InitDlg\n" );


	if( !m_moeah ){
		m_moeah = new CMOEAnimHandler();
		_ASSERT( m_moeah );
	}

	if( (atshowflag == 1) && (m_moeah->m_animnum >= 1) && (m_animno >= 0) ){
		//ShowDlgから呼ばれた場合、すでにアニメがあれば作らない。
		return 0;
	}

	if( !m_dlg->m_shandler ){
		return 0;
	}

	m_dlg->m_mch->m_mh = m_dlg->m_mhandler;//!!!!!!!!



	char animname[256];
	ZeroMemory( animname, 256 );
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	sprintf_s( animname, 256, "MOE_%d_%d_%d_%d_%d_%d_%d",
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
	CMOEAnim* newanim = 0;
	newanim = m_moeah->AddAnim( m_dlg->m_shandler, animname, animtype, m_sl_mp_rangemax + 1, 0 );
	if( !newanim ){
		DbgOut( "MOEKeyDlg : InitDlg : cah AddAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_animno = newanim->m_motkind;

	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_current = m_startframe;


	if( m_moeah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "MOEKeyDlg : InitDlg : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = newanim->InitFrameData();
	_ASSERT( !ret );

	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );


	m_motid[ m_moeah->m_animnum - 1 ].id = m_animno;
	m_motid[ m_moeah->m_animnum - 1 ].filename[0] = 0;
	m_motid[ m_moeah->m_animnum - 1 ].ev0idle = 0;
	m_motid[ m_moeah->m_animnum - 1 ].commonid = 0;
	m_motid[ m_moeah->m_animnum - 1 ].forbidnum = 0;
	if( m_motid[ m_moeah->m_animnum - 1 ].forbidid ){
		free( m_motid[ m_moeah->m_animnum - 1 ].forbidid );
	}
	m_motid[ m_moeah->m_animnum - 1 ].forbidid = 0;
	m_motid[ m_moeah->m_animnum - 1 ].notfu = 0;

	m_motid[ m_moeah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_moeah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_moeah->m_animnum - 1 ].current = m_startframe;

	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	InterlockedExchange( &m_previewframe, m_startframe );


	::SendMessage( m_dlg->m_apphwnd, WM_USER_MOEMENU, m_moeah->m_animnum - 1, 0 );

	return 0;
}

LRESULT CMOEKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "MOEKeyDlg : OnOK\n" );
	ShowWindow( SW_HIDE );
	return 0;
}

LRESULT CMOEKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowWindow( SW_HIDE );
	return 0;
}
LRESULT CMOEKeyDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	return 0;
}


int CMOEKeyDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	s_dlg_wnd = m_hWnd;

	m_tex_current_wnd = GetDlgItem( IDC_TEX_CURRENT );
	m_sl_mp_wnd = GetDlgItem( IDC_SL_MP );

	m_mp_wnd = GetDlgItem( IDC_MP );
	m_sp_mp_wnd = GetDlgItem( IDC_SP_MP );

	m_preview_wnd = GetDlgItem( IDC_PREVIEW );
	m_stop_wnd = GetDlgItem( IDC_STOP );


	m_eventno_wnd = GetDlgItem( IDC_EVENTNO );
	_ASSERT( m_eventno_wnd );
	
	m_list_wnd = GetDlgItem( IDC_LIST );
	_ASSERT( m_list_wnd );
	m_notlist_wnd = GetDlgItem( IDC_NOTLIST );
	_ASSERT( m_notlist_wnd );
	m_elemlist_wnd = GetDlgItem( IDC_ELEMLIST );
	_ASSERT( m_elemlist_wnd );

	m_comboidle_wnd = GetDlgItem( IDC_COMBOIDLE );
	_ASSERT( m_comboidle_wnd );
	m_combolist_wnd = GetDlgItem( IDC_COMBOLIST );
	_ASSERT( m_combolist_wnd );
	m_combonotlist_wnd = GetDlgItem( IDC_COMBONOTLIST );
	_ASSERT( m_combonotlist_wnd );


	//sliderの初期設定
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	return 0;
}

int CMOEKeyDlg::InitBone2Combo()
{
	if( m_bone2combo ){
		free( m_bone2combo );
		m_bone2combo = 0;
	}
	if( m_combo2bone ){
		free( m_combo2bone );
		m_combo2bone = 0;
	}
	m_indexleng = 0;
	m_jointnum = 0;

	if( !m_dlg ){
		return 0;
	}
	if( !m_dlg->m_shandler ){
		return 0;
	}

	int ret;
	ret = m_dlg->m_shandler->GetJointNum( &m_jointnum );
	_ASSERT( !ret );

	if( m_jointnum >= 1 ){
		m_indexleng = m_dlg->m_shandler->s2shd_leng;
		m_bone2combo = (int*)malloc( sizeof( int ) * m_indexleng );
		if( !m_bone2combo ){
			DbgOut( "MOEKeyDlg : InitBone2Combo : bone2combo alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( m_bone2combo, sizeof( int ) * m_indexleng );

		m_combo2bone = (int*)malloc( sizeof( int ) * m_jointnum );
		if( !m_combo2bone ){
			DbgOut( "MOEKeyDlg : InitBone2Combo : combo2bone alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( m_combo2bone, sizeof( int ) * m_jointnum );

		int setno1 = 0;
		int seri;
		CTreeElem2* telem;
		for( seri = 0; seri < m_indexleng; seri++ ){
			telem = (*m_dlg->m_thandler)( seri );
			_ASSERT( telem );
			if( telem->IsJoint() ){
				*( m_bone2combo + seri ) = setno1;
				*( m_combo2bone + setno1 ) = seri;
				setno1++;
			}else{
				*( m_bone2combo + seri ) = -1;
			}
		}
		if( setno1 != m_jointnum ){
			DbgOut( "MOEKeyDlg : InitBone2Combo : setno1 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}


int CMOEKeyDlg::InitCombo()
{
	int ret;
	ret = InitBone2Combo();
	if( ret ){
		DbgOut( "moekeydlg : InitCombo : InitBone2Combo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_combolist_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );
	m_combolist_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"全てのボーン" );
	if( m_dlg && m_dlg->m_thandler && (m_jointnum >= 1) ){
		int jno;
		for( jno = 0; jno < m_jointnum; jno++ ){
			int curseri;
			curseri = *( m_combo2bone + jno );
			if( (curseri <= 0) || (curseri >= m_dlg->m_thandler->s2e_leng) ){
				_ASSERT( 0 );
				return 1;
			}
			CTreeElem2* telem;
			telem = (*m_dlg->m_thandler)( curseri );
			_ASSERT( telem );
			m_combolist_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)telem->name );
		}
	}
	m_combolist_wnd.SendMessage( CB_SETCURSEL, 0, 0 );


	m_combonotlist_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );
	if( m_dlg && m_dlg->m_thandler && (m_jointnum >= 1) ){
		int jno;
		for( jno = 0; jno < m_jointnum; jno++ ){
			int curseri;
			curseri = *( m_combo2bone + jno );
			if( (curseri <= 0) || (curseri >= m_dlg->m_thandler->s2e_leng) ){
				_ASSERT( 0 );
				return 1;
			}
			CTreeElem2* telem;
			telem = (*m_dlg->m_thandler)( curseri );
			_ASSERT( telem );
			m_combonotlist_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)telem->name );
		}
		m_combonotlist_wnd.SendMessage( CB_SETCURSEL, 0, 0 );
	}

	
	m_comboidle_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );
	if( m_dlg->m_motionnum > 0 ){
		int motid;
		char motname[256];
		for( motid = 0; motid < m_dlg->m_motionnum; motid++ ){
			ZeroMemory( motname, sizeof( char ) * 256 );
			ret = m_dlg->m_mhandler->GetMotionName( motid, motname );
			if( ret ){
				DbgOut( "moekeydlg : InitCombo : mh GetMotionName error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_comboidle_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)motname );
		}


		//m_comboidle_wnd.SendMessage( CB_SETCURSEL, 0, 0 );
		int idlingid;
		idlingid = m_dlg->m_mch->GetIdlingMotID();
		if( idlingid < 0 ){
			idlingid = 0;
		}
		m_comboidle_wnd.SendMessage( CB_SETCURSEL, (WPARAM)idlingid, 0 );
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

int CMOEKeyDlg::InitList()
{
	if( !m_elemlist_wnd.IsWindow() ){
		return 0;
	}
	m_elemlist_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	if( !m_list_wnd.IsWindow() ){
		return 0;
	}
	m_list_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	if( !m_notlist_wnd.IsWindow() ){
		return 0;
	}
	m_notlist_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );

	if( !m_dlg->m_mhandler || !m_dlg->m_shandler || !m_dlg->m_thandler ){
		return 0;
	}
	
	int ret;
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	CMOEKey* moek = 0;
	if( anim ){
		ret = anim->CheckAndDelInvalid( m_dlg->m_shandler, m_current, &moek );//seriチェック。不正データ削除。
		if( ret ){
			DbgOut( "moekeydlg : InitList : anim CheckAndDelInvalid error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( moek ){
			if( m_elemindex >= moek->m_moeenum ){
				m_elemindex = 0;
			}
			//m_moee = *( moek->m_pmoee + m_elemindex );

			int eno;
			for( eno = 0; eno < moek->m_moeenum; eno++ ){
				MOEELEM* curmoee = moek->m_pmoee + eno;
				_ASSERT( curmoee );

				char addname[256];
				ZeroMemory( addname, sizeof( char ) * 256 );
				if( curmoee->listnum > 0 ){
					int seri;
					seri = curmoee->list[0];
					if( seri == -1 ){
						strcpy_s( addname, 256, "全てのボーン" );
					}else if( seri > 0 ){
						CTreeElem2* telem;
						telem = (*m_dlg->m_thandler)( seri );
						_ASSERT( telem );
						strcpy_s( addname, 256, telem->name );
					}else{
						_ASSERT( 0 );
						return 1;
					}
				}else{
					strcpy_s( addname, 256, "設定なし" );					
				}

				char strlb[280];
				sprintf_s( strlb, 280, "%d:%s", curmoee->eventno, addname );

				int newindex;
				if( !m_elemlist_wnd.IsWindow() ){
					return 0;
				}
				newindex = (int)m_elemlist_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)strlb );
				if( (newindex == LB_ERR) || (newindex == LB_ERRSPACE) ){
					_ASSERT( 0 );
					return 1;
				}
			}
			if( moek->m_moeenum > 0 ){
				if( !m_elemlist_wnd.IsWindow() ){
					return 0;
				}
				m_elemlist_wnd.SendMessage( LB_SETCURSEL, (WPARAM)m_elemindex, 0 );
			}

		}else{
			m_elemindex = 0;
		}
	}else{
		m_elemindex = 0;

		ret = CheckAndDelInvalidMOEE( m_dlg->m_shandler, &m_moee );
		if( ret ){
			DbgOut( "moekeydlg : InitList : CheckAndDelInvalidMOEE error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	int listno;
	for( listno = 0; listno < m_moee.listnum; listno++ ){
		int seri;
		seri = m_moee.list[listno];
		char addname[256];
		ZeroMemory( addname, sizeof( char ) * 256 );
		if( seri == -1 ){
			strcpy_s( addname, 256, "全てのボーン" );
		}else if( seri > 0 ){
			CTreeElem2* telem;
			telem = (*m_dlg->m_thandler)( seri );
			_ASSERT( telem );
			strcpy_s( addname, 256, telem->name );
		}else{
			_ASSERT( 0 );
			return 1;
		}

		int newindex;
		if( !m_list_wnd.IsWindow() ){
			return 0;
		}
		newindex = (int)m_list_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)addname );
	}


	int notlistno;
	for( notlistno = 0; notlistno < m_moee.notlistnum; notlistno++ ){
		int seri;
		seri = m_moee.notlist[notlistno];
		char addname[256];
		ZeroMemory( addname, sizeof( char ) * 256 );
		if( seri == -1 ){
			strcpy_s( addname, 256, "全てのボーン" );
		}else if( seri > 0 ){
			CTreeElem2* telem;
			telem = (*m_dlg->m_thandler)( seri );
			_ASSERT( telem );
			strcpy_s( addname, 256, telem->name );
		}else{
			_ASSERT( 0 );
			return 1;
		}

		int newindex;
		if( !m_notlist_wnd.IsWindow() ){
			return 0;
		}
		newindex = (int)m_notlist_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)addname );
	}

	
	if( moek ){
		int idlingid;
		idlingid = m_dlg->m_mhandler->GetMotionIDByName( moek->m_idlingname );
		if( idlingid >= 0 ){
			if( !m_comboidle_wnd.IsWindow() ){
				return 0;
			}
			m_comboidle_wnd.SendMessage( CB_SETCURSEL, (WPARAM)idlingid, 0 );
		}
	}

	return 0;
}

LRESULT CMOEKeyDlg::OnSelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	int selindex;
	selindex = (int)m_elemlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		m_elemindex = 0;
		return 0;
	}
	int ret;
	m_elemindex = selindex;
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( anim ){
		CMOEKey* moek = 0;
		ret = anim->ExistMOEKey( m_current, &moek );
		if( moek ){
			if( m_elemindex >= moek->m_moeenum ){
				m_elemindex = 0;
			}
			if( moek->m_moeenum > 0 ){
				m_moee = *( moek->m_pmoee + m_elemindex );
			}else{
				ZeroMemory( &m_moee, sizeof( MOEELEM ) );
			}

			int idlingid;
			idlingid = m_dlg->m_mhandler->GetMotionIDByName( moek->m_idlingname );
			if( idlingid >= 0 ){
				m_comboidle_wnd.SendMessage( CB_SETCURSEL, (WPARAM)idlingid, 0 );
			}
		}
	}

	ParamsToDlg();

	return 0;
}

int CMOEKeyDlg::ParamsToDlg()
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

	sprintf_s( tempchar, 1024, "%d", m_moee.eventno );
	if( (g_endapp == 1) || !m_eventno_wnd.IsWindow() )
		return 0;
	m_eventno_wnd.SetWindowText( tempchar );

//goon
	int goonflag = 0;
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( anim ){
		CMOEKey* moek = 0;
		ret = anim->ExistMOEKey( m_current, &moek );
		if( moek ){
			goonflag = moek->m_goonflag;
		}
	}
	if( goonflag ){
		::CheckDlgButton( m_dlg_wnd, IDC_GOON, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_GOON, BST_UNCHECKED );
	}
	

	return 0;
}

LRESULT CMOEKeyDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}

LRESULT CMOEKeyDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}


	SetCurrentMotion( 0 );

	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( anim ){
		CMOEKey* moek = 0;
		ret = anim->ExistMOEKey( m_current, &moek );
		if( moek ){
			if( moek->m_moeenum > 0 ){
				if( m_elemindex >= moek->m_moeenum ){
					m_elemindex = 0;
				}
				m_moee = *( moek->m_pmoee + m_elemindex );
			}

			int idlingid;
			idlingid = m_dlg->m_mhandler->GetMotionIDByName( moek->m_idlingname );
			if( idlingid >= 0 ){
				m_comboidle_wnd.SendMessage( CB_SETCURSEL, (WPARAM)idlingid, 0 );
			}
		}
	}

	Redraw();

	if( (idCtrl != 999) && (m_app->m_syncdlg->m_moeflag != 0) ){
		int quaflag = m_app->m_syncdlg->m_quaflag;
		int camflag = m_app->m_syncdlg->m_camflag;
		int gpaflag = m_app->m_syncdlg->m_gpaflag;
		int sndflag = m_app->m_syncdlg->m_sndflag;

		g_motdlg->OnJumpFrame( m_current, quaflag, camflag, gpaflag, 0, sndflag );
	}


	return 0;
}
LRESULT CMOEKeyDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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

	if( m_moeah->m_animnum <= 0 ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!
	}

	ParamsToDlg();
	Redraw();

	return 0;

}
void CMOEKeyDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
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


int CMOEKeyDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
	*posptr = MP_SIZE + (int)(step * (float)( frameno - m_startframe ) ) + clirect.left;
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	
	return 0;
}

int CMOEKeyDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
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

int CMOEKeyDlg::DrawMOEKeys()
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
	if( !m_moeah ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	CMOEKey* curmoek = anim->GetFirstMOEKey();
	while( curmoek ){
		int frameno = curmoek->m_frameno;
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

		curmoek = curmoek->next;
	}
	
	if( m_dragmoek ){
		int frameno = m_dragmoek->m_frameno;
		
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


int CMOEKeyDlg::Redraw()
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
//		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}

LRESULT CMOEKeyDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	
	DrawMOEKeys();

	return 0;
}

LRESULT CMOEKeyDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CMOEKeyDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "MOEKeyDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CMOEKeyDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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




LRESULT CMOEKeyDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 ){
		return 0;
	}

	if( m_moeah->m_animnum <= 0 ){
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


LRESULT CMOEKeyDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );


	BOOL dummyb;
	return m_dlg->OnPreview( 0, 0, 0, dummyb );

}

int CMOEKeyDlg::CaptureM()
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


LRESULT CMOEKeyDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	CaptureM();

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "MOEKeyDlg : OnMouseMove : MK_LBUTTON\n" );

	if( (m_dragflag == 1) && m_dragmoek ){
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

		m_dragmoek->m_frameno = frameno;

		Redraw();
	}

	CMOEAnim* anim;
	int animindex;
	anim = m_moeah->GetMOEAnim( m_animno, &animindex );
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


LRESULT CMOEKeyDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		return 0;
	}

	int ret;
	CMOEKey* moekptr = 0;
	int existflag = 0;

	int cpok = 0;
	ret = anim->ExistMOEKey( m_current, &moekptr );
	_ASSERT( !ret );
	if( ret || !moekptr ){
		cpok = 0;
	}else{
		cpok = 1;
	}

	if( !cpok ){
		::MessageBox( m_hWnd, "このフレームにはコピーできるキーがありません。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!
	}

	if( m_cpmoek ){
		delete m_cpmoek;
		m_cpmoek = 0;
	}

	m_cpmoek = new CMOEKey();
	if( !m_cpmoek ){
		DbgOut( "MOEKeyDlg : OnFrameCopy : cpmoek alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_cpmoek->CopyMOEElem( moekptr );
	if( ret ){
		DbgOut( "moekydlg : OnFrameCopy : moek CopyMOEElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_cpmoek->m_goonflag = moekptr->m_goonflag;

	return 0;
}

LRESULT CMOEKeyDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		return 0;
	}

	if( !m_cpmoek ){
		::MessageBox( m_hWnd, "フレームコピーを行ってから再試行してください。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!!!!
	}

	int ret;

	ret = anim->SetMOEKey( m_current, m_cpmoek );
	_ASSERT( !ret );

	if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
		ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
		_ASSERT( !ret );
	}else{
		_ASSERT( 0 );
	}
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();
	return 0;
}

LRESULT CMOEKeyDlg::OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		return 0;
	}

	int ret;
	ret = anim->DeleteMOEKey( m_current );
	if( ret ){
		DbgOut( "MOEKeyDlg : OnDelete : ca DeleteMOEKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
		ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
		_ASSERT( !ret );
	}else{
		_ASSERT( 0 );
	}
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CMOEKeyDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		return 0;
	}

	int ret;
	CMOEKey* curmoek = anim->GetFirstMOEKey();
	CMOEKey* nextmoek = 0;
	while( curmoek ){
		nextmoek = curmoek->next;

		ret = anim->DeleteMOEKey( curmoek->m_frameno );
		if( ret ){
			DbgOut( "MOEKeyDlg : OnAllDelete : ca DeleteMOEKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		curmoek = nextmoek;
	}

	if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
		ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
		_ASSERT( !ret );
	}else{
		_ASSERT( 0 );
	}
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

LRESULT CMOEKeyDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}


	if( m_dragflag )
		return 0;

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
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

			CMOEKey* moekptr = 0;
			ret = anim->ExistMOEKey( frameno, &moekptr );
			if( !ret && moekptr ){
				m_dragframe = frameno;
				m_dragflag = 1;
				m_dragmoek = moekptr;

				anim->LeaveFromChainMOEKey( frameno );

			}else{
				m_dragframe = 0;
				m_dragflag = 0;
				m_dragmoek = 0;
			}
		}else{
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragmoek = 0;
		}
	}

	return 0;
}


LRESULT CMOEKeyDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	static int busyflag = 0;


	if( busyflag == 1 )
		return 0;

	busyflag = 1;

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
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

		if( (m_dragflag == 1) && m_dragmoek ){
			CMOEKey* isexist = 0;

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

			ret = anim->ExistMOEKey( frameno, &isexist );
			_ASSERT( !ret );
			if( isexist ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "フレーム番号%d には既にキーフレームが存在するので、移動できません。",
					frameno );
				MessageBox( messtr, "移動できません", MB_OK );
				frameno = m_dragframe;//!!!!! 元に戻す
			}
			
			m_dragmoek->m_frameno = frameno;//!!!!!!!!!!!!!!!!

			ret = anim->ChainMOEKey( m_dragmoek );
			if( ret ){
				DbgOut( "MOEKeyDlg : OnLButtonUp : ca ChainMOEKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_dragmoek = 0;

			if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
				ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
				_ASSERT( !ret );
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
	m_dragmoek = 0;

	busyflag = 0;

	return 0;
}
LRESULT CALLBACK MPProcMOE(
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
		//	DbgOut( "MOEKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "MOEKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
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
int CMOEKeyDlg::SetNewWindowProc()
{
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWL_WNDPROC );
	if( !s_mpfunc ){
		_ASSERT( 0 );
		return 1;
	}

	LONG lret;
	lret = ::SetWindowLong( m_sl_mp_wnd, GWL_WNDPROC, (LONG)MPProcMOE );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}


LRESULT CMOEKeyDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		
	bHandled = TRUE;
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;

	int timerid = (int)wParam;
	if( (timerid == MOEKeyDlgTIMER) && (m_cmdshow == SW_SHOW) ){

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


int CMOEKeyDlg::ShowDlg( int cmdshow, int stopflag )
{
	m_cmdshow = cmdshow;

	m_elemindex = 0;
	m_moee = m_zeromoee;

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

LRESULT CMOEKeyDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}


	int findframe = -1;
	int chkframe;

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
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
LRESULT CMOEKeyDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	int findframe = -1;
	int chkframe;

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
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

LRESULT CMOEKeyDlg::OnCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}
	int ret;
	int tmpint;
	char mes[512];

	ret = GetInt( m_eventno_wnd, &tmpint );
	if( ret || (tmpint < 0) ){
		strcpy_s( mes, 512, "eventnoの値が不正です。０以上の整数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_moee.eventno = tmpint;

	int combono;
	combono = (int)m_comboidle_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		strcpy_s( mes, 512, "IdlingMotionが不正です。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	char idlingname[256];
	ZeroMemory( idlingname, sizeof( char ) * 256 );
	ret = m_dlg->m_mhandler->GetMotionName( combono, idlingname );
	if( ret ){
		strcpy_s( mes, 512, "IdlingMotionが不正です。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}

	UINT ischecked;
	int goonflag = 0;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_GOON );
	if( ischecked == BST_CHECKED ){
		goonflag = 1;
	}else{
		goonflag = 0;
	}


	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}
	ret = anim->SetKeyIdlingName( m_current, idlingname );
	if( ret ){
		DbgOut( "MOEKeyDlg : OnCreate : anim SetKeyIdlingName error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = anim->AddMOEE( m_current, &m_moee );
	if( ret ){
		DbgOut( "MOEKeyDlg : OnCreate : anim AddMOEE error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMOEKey* moekptr = 0;
	ret = anim->ExistMOEKey( m_current, &moekptr );
	if( ret || !moekptr ){
		DbgOut( "MOEKeyDlg : OnCreate : anim ExistMOEKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	moekptr->m_goonflag = goonflag;


	if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
		ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
		_ASSERT( !ret );
	}else{
		_ASSERT( 0 );
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

//	_ASSERT( 0 );

	return 0;
}

LRESULT CMOEKeyDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CMOEKeyDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}
int CMOEKeyDlg::SetNewMOE()
{
	int ret;
	static int busyflag = 0;

	if( busyflag != 0 ){
		return 0;
	}
	if( !m_dlg->m_mhandler ){
		return 0;
	}

	if( m_dlg->m_preview_flag == 0 ){
		return 0;
	}

	busyflag = 1;

	CMOEAnim* anim;

	if( m_moeah->m_animnum >= 1 ){
		ret = m_moeah->SetNewMOE( m_dlg->m_shandler, m_dlg->m_mhandler );
		_ASSERT( !ret );
		m_animno = m_moeah->m_animno;
		//m_moee = m_moeah->m_curmoee;
		anim = m_moeah->GetMOEAnim( m_animno );
		if( anim ){
			m_current = anim->m_frameno;
		}
		ParamsToDlg();
	}

//DbgOut( "checkf!!! : moekdlg : SetNewMOE : m_current %d, m_animno %d\r\n", m_current, m_animno );


	busyflag = 0;
	return 0;
}


int CMOEKeyDlg::SetCurrentMotion( int isfirst )
{
	int ret;
	_ASSERT( m_animno >= 0 );

	ret = m_moeah->SetAnimFrameNo( m_dlg->m_shandler, m_dlg->m_mhandler, m_animno, m_current, isfirst );
	_ASSERT( !ret );
	//m_moee = m_moeah->m_curmoee;
	m_elemindex = 0;//!!!!!!!!!!

	return 0;
}


int CMOEKeyDlg::ResetParams( int atshowflag )
{
	
	int ret;

	DestroyObjs( 0 );

	ret = InitDlg( atshowflag );
	if( ret ){
		DbgOut( "MOEKeydlg : ResetParams : InitDlg error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = InitCombo();
	if( ret ){
		DbgOut( "MOEKeydlg : ResetParams : InitCombo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	SetCurrentMotion( 0 );

	return 0;
}


LRESULT CMOEKeyDlg::OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}


	CMOEAnim* anim;
	int animindex;
	anim = m_moeah->GetMOEAnim( m_animno, &animindex );
	if( !anim ){
		return 0;
	}

	int ret = 0;
	int dlgret;
	
	CMotSettingDlg	dlg( 0, anim->m_animname, m_sl_mp_rangemax + 1, anim->m_animtype, 0, anim->m_motjump, m_startframe, m_endframe );
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
				ret = m_moeah->SetTotalFrame( m_dlg->m_shandler, m_animno, newrangemax + 1 );
				if( ret ){
					DbgOut( "MOEKeyDlg : OnSetting : cah SetTotalFrame error !!!\n" );
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
//		if( m_moeah->m_definterp != dlg.m_interpolation ){
//			m_moeah->m_definterp = dlg.m_interpolation;
//		}

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

	::SendMessage( m_dlg->m_apphwnd, WM_USER_MOEMENU, animindex, 0 );

//	_ASSERT( 0 );

	return 0;
}



int CMOEKeyDlg::SetPreviewButton()
{
	if( m_dlg->m_preview_flag ){
		::SendMessage( m_preview_wnd, BM_SETSTATE, TRUE, 0 );  
	}else{
		::SendMessage( m_preview_wnd, BM_SETSTATE, FALSE, 0 );  
	}

	return 0;
}

int CMOEKeyDlg::GetCurMOE( MOEELEM* dstmoee )
{
	*dstmoee = m_moee;
	return 0;
}

int CMOEKeyDlg::GetMOEAnimNum()
{
	if( !IsWindow() )
		return 0;

	if( !m_moeah ){
		return 0;
	}

	int retnum;
	retnum = m_moeah->m_animnum;
	return retnum;
}

int CMOEKeyDlg::LoadMOEFile( char* filename )
{
	int ret;
	if( !m_moeah ){
		_ASSERT( 0 );
		return 0;
	}

	CMOEFile moefile;
	int animno = -1;
	ret = moefile.LoadMOEFile( m_dlg->m_thandler, m_dlg->m_shandler, filename, m_moeah, &animno );
	if( ret ){
		DbgOut( "MOEKeyDlg : LoadMOEFile : moefile LoadMOEFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_moeah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "MOEKeyDlg : LoadMOEFile : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_animno = animno;
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		DbgOut( "MOEKeyDlg : LoadMOEFile : cah GetMOEAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_moeah->SetCurrentAnim( m_animno );
	if( ret ){
		DbgOut( "MOEKeyDlg : LoadMOEFile : cah SetCurrentAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
	if( ret ){
		DbgOut( "MOEKeyDlg : LoadMOEFile : anim CalcFrameData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	m_motid[ m_moeah->m_animnum - 1 ].id = m_animno;
	char* enptr = 0;
	enptr = strrchr( filename, '\\' );
	if( enptr ){
		strcpy_s( m_motid[ m_moeah->m_animnum - 1 ].filename, 256, enptr + 1 );
	}else{
		strcpy_s( m_motid[ m_moeah->m_animnum - 1 ].filename, 256, filename );
	}

	m_sl_mp_rangemax = anim->m_maxframe;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;

	m_motid[ m_moeah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_moeah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	::SendMessage( m_dlg->m_apphwnd, WM_USER_MOEMENU, m_moeah->m_animnum - 1, 0 );
	
	return 0;
}


MOTID* CMOEKeyDlg::GetMOEMotIDArray()
{
	return m_motid;
}
char* CMOEKeyDlg::GetMOEAnimName( int animno )
{
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( animno );
	if( !anim ){
		return 0;
	}
	return anim->m_animname;
}
int CMOEKeyDlg::GetCurMOEAnimIndex()
{
	CMOEAnim* anim;
	int animindex = 0;
	anim = m_moeah->GetMOEAnim( m_animno, &animindex );
	if( anim ){
		return animindex;
	}else{
		return 0;
	}
}
int CMOEKeyDlg::GetCurMOEAnimNo()
{
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( anim ){
		return anim->m_motkind;
	}else{
		return 0;
	}
}

int CMOEKeyDlg::SelectMOEAnim( int srcindex )
{
	if( !m_moeah ){
		_ASSERT( 0 );
		return 0;
	}
	if( m_dlg->m_preview_flag ){
		return 0;
	}

	if( (srcindex < 0) || (srcindex >= m_moeah->m_animnum) ){
		DbgOut( "MOEKeyDlg : SelectMOEAnim : srcindex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	int tmpcookie;
	tmpcookie = m_motid[ srcindex ].id;
	if( tmpcookie < 0 ){
		DbgOut( "MOEKeyDlg : SelectMOEAnim : tmpcookie error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

/////////
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnimByIndex( srcindex );
	if( !anim ){
		DbgOut( "MOEKeyDlg : SelectMOEAnim : cah GetMOEAnimByIndex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	int ret;
	ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
	if( ret ){
		DbgOut( "MOEKeyDlg : SelectMOEAnim : anim CalcFrameData error !!!\n" );
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
	::SendMessage( m_dlg->m_apphwnd, WM_USER_MOEMENU, srcindex, 0 );


	return 0;
}


int CMOEKeyDlg::DestroyMOEAnim( int delanimno )
{
	int ret;

	if( delanimno < 0 ){
		if( m_moeah ){
			delete m_moeah;
			m_moeah = 0;
		}

		ret = InitDlg( 0 );
		if( ret ){
			DbgOut( "MOEKeyDlg : DestroyMOEAnim all : InitDlg error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{
		_ASSERT( m_moeah );
		if( m_moeah->m_animnum <= 0 ){
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

		if( setno != (m_moeah->m_animnum - 1) ){
			_ASSERT( 0 );
			return 1;
		}

		MoveMemory( m_motid, tempmotid, sizeof( MOTID ) * MAXMOTIONNUM );

		//m_motionnum -= 1;//!!!!!

		int ret;
		ret = m_moeah->DestroyAnim( delcookie );
		if( ret ){
			DbgOut( "MOEKeyDlg : DestroyMOEAnim cur : cah DestroyAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( m_moeah->m_animnum <= 0 ){
			ret = InitDlg( 0 );
			if( ret ){
				DbgOut( "MOEKeyDlg : DestroyMOEAnim cur : InitDlg error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = SelectMOEAnim( 0 );
		if( ret ){
			DbgOut( "MOEKeyDlg : DestroyMOEAnim cur : SelectMOEAnim 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}


int CMOEKeyDlg::CalcMotionPointOnFrame( int animno, int frameno, CMState** ppms, int* hasmpflag )
{
	int ret;
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( animno );
	if( !anim ){
		*ppms = 0;
		*hasmpflag = 0;
		return 0;
	}


	ret = anim->GetMOEAnim( ppms, frameno );
	if( ret ){
		DbgOut( "MOEKeyDlg : CalcMotionPointOnFrame : ca GetMOEAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	*hasmpflag = (*ppms)->m_keyflag;

	return 0;
}


int CMOEKeyDlg::ExistMOEKey( int animno, int frameno, CMOEKey** ppmoek )
{
	*ppmoek = 0;

	int ret;
	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( animno );
	if( !anim ){
		return 0;
	}

	ret = anim->ExistMOEKey( frameno, ppmoek );
	if( ret ){
		DbgOut( "MOEKeyDlg : ExistMOEKey : ca ExistMOEKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


LRESULT CMOEKeyDlg::OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( !m_dlg->m_shandler || !m_dlg->m_mhandler ){
		return 0;
	}

	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}
	int ret;
	ret = CalcFrameData();
	if( ret ){
		DbgOut( "moekeydlg : OnCalc : CalcFrameData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

int CMOEKeyDlg::CalcFrameData()
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
		ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
		_ASSERT( !ret );
	}else{
		_ASSERT( 0 );
	}

	return 0;
}


LRESULT CMOEKeyDlg::OnAddList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}

	int combono;
	combono = (int)m_combolist_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;

	if( m_moee.listnum >= MOELISTLENG ){
		::MessageBox( m_hWnd, "listには１５個までしか追加できません。", "エラー", MB_OK );
		return 0;
	}

	if( combono == 0 ){
		int newindex;
		newindex = m_moee.listnum;
		_ASSERT( newindex < MOELISTLENG );
		m_moee.list[newindex] = -1;
		m_moee.listnum = newindex + 1;

	}else{
		int seri;
		seri = *( m_combo2bone + combono - 1 );
		if( (seri <= 0) || ( seri >= m_dlg->m_thandler->s2e_leng ) ){
			_ASSERT( 0 );
			return 1;
		}
		int newindex;
		newindex = m_moee.listnum;
		_ASSERT( newindex < MOELISTLENG );
		m_moee.list[newindex] = seri;
		m_moee.listnum = newindex + 1;
	}

	int tmpint;
	char mes[512];
	ret = GetInt( m_eventno_wnd, &tmpint );
	if( ret || (tmpint < 0) ){
		strcpy_s( mes, 512, "eventnoの値が不正です。０以上の整数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
	}else{
		m_moee.eventno = tmpint;
	}


	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CMOEKeyDlg::OnDelList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}

	int selindex;
	selindex = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		return 0;
	}

	int ret;
	ret = DelMOEEListByIndex( selindex, &m_moee );
	if( ret ){
		DbgOut( "moekeydlg : OnDelList : DelListByIndex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int tmpint;
	char mes[512];
	ret = GetInt( m_eventno_wnd, &tmpint );
	if( ret || (tmpint < 0) ){
		strcpy_s( mes, 512, "eventnoの値が不正です。０以上の整数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
	}else{
		m_moee.eventno = tmpint;
	}


	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CMOEKeyDlg::OnAddNotList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}

	int combono;
	combono = (int)m_combonotlist_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;

	if( m_moee.notlistnum >= MOELISTLENG ){
		::MessageBox( m_hWnd, "listには１５個までしか追加できません。", "エラー", MB_OK );
		return 0;
	}

	int seri;
	seri = *( m_combo2bone + combono );
	if( (seri <= 0) || ( seri >= m_dlg->m_thandler->s2e_leng ) ){
		_ASSERT( 0 );
		return 1;
	}
	CTreeElem2* telem;
	telem = (*m_dlg->m_thandler)( seri );
	int newindex;
	newindex = m_moee.notlistnum;
	_ASSERT( newindex < MOELISTLENG );
	m_moee.notlist[newindex] = seri;
	m_moee.notlistnum = newindex + 1;

	int tmpint;
	char mes[512];
	ret = GetInt( m_eventno_wnd, &tmpint );
	if( ret || (tmpint < 0) ){
		strcpy_s( mes, 512, "eventnoの値が不正です。０以上の整数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
	}else{
		m_moee.eventno = tmpint;
	}


	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CMOEKeyDlg::OnDelNotList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}

	int selindex;
	selindex = (int)m_notlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		return 0;
	}

	int ret;
	ret = DelMOEENotListByIndex( selindex, &m_moee );
	if( ret ){
		DbgOut( "moekeydlg : OnDelNotList : DelListByIndex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int tmpint;
	char mes[512];
	ret = GetInt( m_eventno_wnd, &tmpint );
	if( ret || (tmpint < 0) ){
		strcpy_s( mes, 512, "eventnoの値が不正です。０以上の整数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
	}else{
		m_moee.eventno = tmpint;
	}


	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CMOEKeyDlg::OnAddElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//int ret;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}

	BOOL dummy;
	LRESULT lret;
	lret = OnCreate( 0, 0, 0, dummy );
	if( lret ){
		DbgOut( "moekeydlg : OnAddElem : OnCreate error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CMOEKeyDlg::OnDelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( !m_dlg->m_shandler || !m_dlg->m_mhandler || !m_dlg->m_mch ){
		return 0;
	}
	if( m_moeah->m_animnum <= 0 ){
		return 0;
	}

	int selelem;
	selelem = (int)m_elemlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
//	if( (selelem < 0) || (selelem == LB_ERR) ){
//		return 0;
//	}

	CMOEAnim* anim;
	anim = m_moeah->GetMOEAnim( m_animno );
	if( !anim ){
		return 0;
	}
	CMOEKey* moek = 0;
	ret = anim->ExistMOEKey( m_current, &moek );
	if( ret || !moek ){
		return 0;
	}
	if( (selelem >= 0) && (selelem != LB_ERR) ){
		ret = moek->DeleteMOEE( selelem );
		if( ret ){
			DbgOut( "moekeydlg : OnDelElem: moek DeleteMOEE error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	if( moek->m_moeenum <= 0 ){
		ret = anim->DeleteMOEKey( m_current );
		if( ret ){
			DbgOut( "moekeydlg : OnDelElem : anim DeleteMOEKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	m_elemindex = 0;//!!!!!!!!

	if( m_dlg->m_shandler && m_dlg->m_mhandler && m_dlg->m_mch ){
		ret = anim->CalcFrameData( m_dlg->m_shandler, m_dlg->m_mhandler, m_dlg->m_mch );
		_ASSERT( !ret );
	}else{
		_ASSERT( 0 );
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}
LRESULT CMOEKeyDlg::OnSelIdle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return 0;
}

int CMOEKeyDlg::RemakeMOEAH()
{
	if( m_moeah ){
		delete m_moeah;
		m_moeah = 0;
	}

	m_moeah = new CMOEAnimHandler();
	if( !m_moeah ){
		DbgOut( "moekeydlg : RemakeMOEAH moeah malloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_animno = 0;
	return 0;
}

int CMOEKeyDlg::OnJumpFrame( int srcframe )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CMOEAnim* anim;
	int animindex;
	anim = m_moeah->GetMOEAnim( m_animno, &animindex );
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
int CMOEKeyDlg::SetStartAndEnd( int srcstart, int srcend )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CMOEAnim* anim;
	int animindex;
	anim = m_moeah->GetMOEAnim( m_animno, &animindex );
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

int CMOEKeyDlg::PlayAnim( int srcframe, int totalflag )
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

	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );

	ParamsToDlg();

	return 0;
}