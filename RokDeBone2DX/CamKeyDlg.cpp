// CamKeyDlg.cpp : CCamKeyDlg のインプリメンテーション
#include "stdafx.h"
#include "CamKeyDlg.h"

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

#include <CameraAnimHandler.h>
#include <CameraAnim.h>
#include <CameraKey.h>
#include <CameraSWKey.h>
#include <CameraFile.h>

#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "viewer.h"
#include "MotDlg.h"
#include "MotParamDlg.h"
#include "MotSettingDlg.h"
#include "GraphRotDlg.h"
#include "TargetBoneDlg.h"

#include "GetDlgParams.h"
#include "SyncDlg.h"
#include "inidata.h"

#include <crtdbg.h>


//MotionPoint描画時の点の半径
#define MP_SIZE	3

enum {
	CAME_CAMERA,
	CAME_SW,
	CAME_MAX
};

/////////////////
// sttic
static LONG s_mpfunc = 0;
static HWND s_sl_mp_wnd = 0;
static HWND s_dlg_wnd = 0;
static HWND s_apphwnd = 0;

static char s_strmode[ CAML_MAX ][ 20 ] = {
	"注視なし",
	"瞬間注視",
	"ずっと注視"
};
static CCamKeyDlg* s_camkeydlg = 0;

/////////////////
//extern HINSTANCE g_resinstance;
extern CMotDlg* g_motdlg;
extern unsigned char g_KeyBuffer[256];
extern int g_endapp;
extern CIniData* g_inidata;
/////////////////////////////////////////////////////////////////////////////
// CCamKeyDlg

CCamKeyDlg::CCamKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp )
//: m_list_wnd( NULL, this, 2 )
{
	InitParams();

	m_dlg = srcmpdlg;
	m_dlg->m_camkeydlg = this;
	m_app = srcapp;
	s_camkeydlg = this;
}

int CCamKeyDlg::InitParams()
{
	m_cah = 0;
	m_dlg = 0;
	m_app = 0;
	m_targetdlg = 0;

	m_current = 0;
	m_ismoving = 0;

	m_sl_mp_rangemax = 59;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_previewframe = 0;

	m_dragck = 0;
	m_dragcamno = 0;
	m_dragframe = 0;
	m_dragflag = 0;
	m_cpck = 0;

	m_dragcswk = 0;
	m_swdragcamno = 0;
	m_swdragframe = 0;
	m_swdragflag = 0;
	m_cpcswk = 0;

	m_animno = 0;
	m_camerabutton = CAMB_HEN;
	m_editbutton = CAME_CAMERA;
	/***
	m_camera.looktype = CAML_NORMAL;
	m_camera.pos = D3DXVECTOR3( 0.0f, 0.0f, -5000.0f );
	m_camera.target = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_camera.up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_camera.dist = 5000.0f;
	m_camera.hsid = -1;
	m_camera.boneno = -1;
	m_camera.nearz = 1000.0f;
	m_camera.farz = 40000.0f;
	m_camera.fov = 45.0f;
	***/
	m_camera.looktype = CAML_NORMAL;
	m_camera.pos = D3DXVECTOR3( 0.0f, EYEYINITVAL, -CAMDISTINITVAL );
	m_camera.target = D3DXVECTOR3( 0.0f, EYEYINITVAL, 0.0f );
	m_camera.up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_camera.dist = CAMDISTINITVAL;
	m_camera.hsid = -1;
	m_camera.boneno = -1;
	m_camera.nearz = 100.0f;
	m_camera.farz = 40000.0f;
	m_camera.fov = 45.0f;
	m_camera.ortho = 0;
	m_camera.orthosize = 3000.0f;

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

//	m_bone2combo = 0;
//	m_combo2bone = 0;
//	m_indexleng = 0;
//	m_jointnum = 0;

	m_gtype = G_TARGET;

	ZeroMemory( m_keyno, sizeof( int* ) * CAMERANUMMAX );
	m_totalkeyno = 0;
	m_swkeyno = 0;
	m_swkeygetnum = 0;
	m_keynoleng = 0;

	m_ortho = 0;


	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

	m_savebmdisp = g_inidata->opt_bmdisp;
	m_savejdisp = g_inidata->opt_jointdisp;

	return 0;

	m_fillupup = 0;
}


CCamKeyDlg::~CCamKeyDlg()
{
	s_camkeydlg = 0;

	DestroyObjs( 1 );

	if( m_targetdlg ){
		m_targetdlg->ShowDlg( SW_HIDE, 0, 0, 0, 0 );
		m_targetdlg->DestroyWindow();

		delete m_targetdlg;
		m_targetdlg = 0;
	}

}

int CCamKeyDlg::DestroyObjs( int delcah )
{
	if( m_cpck ){
		delete m_cpck;
		m_cpck = 0;
	}
	if( m_cpcswk ){
		delete m_cpcswk;
		m_cpcswk = 0;
	}

	if( delcah ){
		if( m_cah ){
			delete m_cah;
			m_cah = 0;
		}
	}

//	if( m_bone2combo ){
//		free( m_bone2combo );
//		m_bone2combo = 0;
//	}
//	if( m_combo2bone ){
//		free( m_combo2bone );
//		m_combo2bone = 0;
//	}
//	m_indexleng = 0;
//	m_jointnum = 0;

	DestroyKeyNo();

	return 0;
}

int CCamKeyDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, CamKeyDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	return 0;
}

LRESULT CCamKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	if( m_cah ){
		delete m_cah;
		m_cah = 0;
	}
	m_cah = new CCameraAnimHandler();
	_ASSERT( m_cah );


	if( m_targetdlg ){
		delete m_targetdlg;
		m_targetdlg = 0;
	}
	m_targetdlg = new CTargetBoneDlg( this );
	if( !m_targetdlg ){
		_ASSERT( 0 );
		return -1;
	}
	m_targetdlg->Create( m_hWnd );


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
	timerid = (int)::SetTimer( m_hWnd, CamKeyDlgTIMER, 500, NULL );
	_ASSERT( timerid == CamKeyDlgTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}


int CCamKeyDlg::InitDlg( int atshowflag )
{
	int ret;	
	DbgOut( "CamKeyDlg : InitDlg\n" );


	if( !m_cah ){
		m_cah = new CCameraAnimHandler();
		_ASSERT( m_cah );
	}

	if( (atshowflag == 1) && (m_cah->m_animnum >= 1) && (m_animno >= 0) ){
		//ShowDlgから呼ばれた場合、すでにアニメがあれば作らない。
		return 0;
	}

	char animname[256];
	ZeroMemory( animname, 256 );
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	sprintf_s( animname, 256, "camera_%d_%d_%d_%d_%d_%d_%d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
	);

	int animtype = MOTION_CLAMP;
	CCameraAnim* newanim = 0;
	newanim = m_cah->AddAnim( animname, animtype, m_sl_mp_rangemax + 1, 0 );
	if( !newanim ){
		DbgOut( "CamKeyDlg : InitDlg : cah AddAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_animno = newanim->m_motkind;

	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;
	m_current = m_startframe;


	if( m_cah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "CamKeyDlg : InitDlg : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );


	m_motid[ m_cah->m_animnum - 1 ].id = m_animno;
	m_motid[ m_cah->m_animnum - 1 ].filename[0] = 0;
	m_motid[ m_cah->m_animnum - 1 ].ev0idle = 0;
	m_motid[ m_cah->m_animnum - 1 ].commonid = 0;
	m_motid[ m_cah->m_animnum - 1 ].forbidnum = 0;
	if( m_motid[ m_cah->m_animnum - 1 ].forbidid ){
		free( m_motid[ m_cah->m_animnum - 1 ].forbidid );
	}
	m_motid[ m_cah->m_animnum - 1 ].forbidid = 0;
	m_motid[ m_cah->m_animnum - 1 ].notfu = 0;

	m_motid[ m_cah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_cah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_cah->m_animnum - 1 ].current = m_startframe;

	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	InterlockedExchange( &m_previewframe, m_startframe );


	::SendMessage( m_dlg->m_apphwnd, WM_USER_CAMERAMENU, m_cah->m_animnum - 1, 0 );

	return 0;
}

LRESULT CCamKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "CamKeyDlg : OnOK\n" );
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}

LRESULT CCamKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}
LRESULT CCamKeyDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}


int CCamKeyDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	s_dlg_wnd = m_hWnd;

	m_tex_current_wnd = GetDlgItem( IDC_TEX_CURRENT );
	m_sl_mp_wnd = GetDlgItem( IDC_SL_MP );
	m_selhen_wnd = GetDlgItem( IDC_SELHEN );
	m_sel1_wnd = GetDlgItem( IDC_SEL1 );
	m_sel2_wnd = GetDlgItem( IDC_SEL2 );
	m_sel3_wnd = GetDlgItem( IDC_SEL3 );
	m_selkae_wnd = GetDlgItem( IDC_SELKAE );

	//m_combomode_wnd = GetDlgItem( IDC_COMBO_MODE );
//	m_combobone_wnd = GetDlgItem( IDC_COMBO_BONE );
	m_combog_wnd = GetDlgItem( IDC_COMBO_G );

	m_mp_wnd = GetDlgItem( IDC_MP );
	m_sw_wnd = GetDlgItem( IDC_SW );
	m_sp_mp_wnd = GetDlgItem( IDC_SP_MP );

	m_tarx_wnd = GetDlgItem( IDC_TARX );
	m_tary_wnd = GetDlgItem( IDC_TARY );
	m_tarz_wnd = GetDlgItem( IDC_TARZ );

	m_posx_wnd = GetDlgItem( IDC_PX );
	m_posy_wnd = GetDlgItem( IDC_PY );
	m_posz_wnd = GetDlgItem( IDC_PZ );

	m_dist_wnd = GetDlgItem( IDC_DIST );

	m_upx_wnd = GetDlgItem( IDC_UPX );
	m_upy_wnd = GetDlgItem( IDC_UPY );
	m_upz_wnd = GetDlgItem( IDC_UPZ );

	m_near_wnd = GetDlgItem( IDC_NEAR );
	m_far_wnd = GetDlgItem( IDC_FAR );
	m_fov_wnd = GetDlgItem( IDC_FOV );

	m_preview_wnd = GetDlgItem( IDC_PREVIEW );
	m_stop_wnd = GetDlgItem( IDC_STOP );

	m_editcam_wnd = GetDlgItem( IDC_EDITCAM );
	m_editsw_wnd = GetDlgItem( IDC_EDITSW );

	m_pers_wnd = GetDlgItem( IDC_VPROJ );
	m_ortho_wnd = GetDlgItem( IDC_VORTHO );

	m_drag_wnd = GetDlgItem( IDC_DRAG );

	//sliderの初期設定
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	return 0;
}

int CCamKeyDlg::InitCombo()
{
	//int ret;
/***
	ret = InitBone2Combo();
	if( ret ){
		DbgOut( "camkeydlg : InitCombo : InitBone2Combo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_combobone_wnd.SendMessageA( CB_RESETCONTENT, 0, 0 );
	m_combobone_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"なし" );
	if( m_dlg && m_dlg->m_thandler && (m_jointnum >= 1) ){
		int jno;
		for( jno = 1; jno < (m_jointnum + 1); jno++ ){
			int curseri;
			curseri = *( m_combo2bone + jno );
			if( (curseri <= 0) || (curseri >= m_dlg->m_thandler->s2e_leng) ){
				_ASSERT( 0 );
				return 1;
			}
			CTreeElem2* telem;
			telem = (*m_dlg->m_thandler)( curseri );
			_ASSERT( telem );
			m_combobone_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)telem->name );
		}

		if( (m_camera.boneno > 0) && (m_camera.boneno < m_dlg->m_thandler->s2e_leng) ){
			int combono;
			combono = *( m_bone2combo + m_camera.boneno );
			if( combono > 0 ){
				m_combobone_wnd.SendMessage( CB_SETCURSEL, combono, 0 );
			}else{
				m_camera.boneno = -1;
				m_combobone_wnd.SendMessage( CB_SETCURSEL, 0, 0 );
			}
		}else{
			m_camera.boneno = -1;
			m_combobone_wnd.SendMessage( CB_SETCURSEL, 0, 0 );
		}
	}else{
		m_camera.boneno = -1;
		m_combobone_wnd.SendMessage( CB_SETCURSEL, 0, 0 );
	}
***/	

//	m_combomode_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );
//	int modeno;
//	for( modeno = 0; modeno < CAML_MAX; modeno++ ){
//		m_combomode_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)(&(s_strmode[modeno][0])) );
//	}
//	m_combomode_wnd.SendMessage( CB_SETCURSEL, m_camera.looktype, 0 );


	
	m_combog_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );
	int gno;
	for( gno = 0; gno < G_MAX; gno++ ){
		m_combog_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)(&(strgtype[gno][0])) );
	}
	m_combog_wnd.SendMessage( CB_SETCURSEL, m_gtype, 0 );



//CB_RESETCONTENT
//	m_combo_no_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
//	m_combo_no_wnd.SendMessage( CB_SETCURSEL, m_editswitchno - 1, 0 );
//	int combono;
//	combono = (int)m_combo_no_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
//	if( combono == CB_ERR )
//		return 1;
	
	return 0;
}

/***
int CCamKeyDlg::InitBone2Combo()
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
			DbgOut( "CamKeyDlg : InitBone2Combo : bone2combo alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( m_bone2combo, sizeof( int ) * m_indexleng );

		m_combo2bone = (int*)malloc( sizeof( int ) * (m_jointnum + 1) );
		if( !m_combo2bone ){
			DbgOut( "CamKeyDlg : InitBone2Combo : combo2bone alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( m_combo2bone, sizeof( int ) * (m_jointnum + 1) );

		int setno1 = 0;
		int seri;
		CTreeElem2* telem;
		for( seri = 0; seri < m_indexleng; seri++ ){
			telem = (*m_dlg->m_thandler)( seri );
			_ASSERT( telem );
			if( telem->IsJoint() ){
				*( m_bone2combo + seri ) = setno1;
				setno1++;
			}else{
				*( m_bone2combo + seri ) = -1;
			}
		}
		if( setno1 != m_jointnum ){
			DbgOut( "CamKeyDlg : InitBone2Combo : setno1 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}



		*( m_combo2bone + 0 ) = 0;//"なし"
		int setno2 = 1;
		for( seri = 0; seri < m_indexleng; seri++ ){
			telem = (*m_dlg->m_thandler)( seri );
			_ASSERT( telem );
			if( telem->IsJoint() ){
				*( m_combo2bone + setno2 ) = seri;
				setno2++;
			}
		}
		if( setno2 != (m_jointnum + 1) ){
			DbgOut( "CamKeyDlg : InitBone2Combo : setno2 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}
***/

int CCamKeyDlg::ParamsToDlg()
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


	if( m_dlg->m_preview_flag == 1 )
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


	sprintf_s( tempchar, 1024, "%f", m_camera.target.x );
	if( (g_endapp == 1) || !m_tarx_wnd.IsWindow() ){
		return 0;
	}
	m_tarx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_camera.target.y );
	if( (g_endapp == 1) || !m_tary_wnd.IsWindow() ){
		return 0;
	}
	m_tary_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_camera.target.z );
	if( (g_endapp == 1) || !m_tarz_wnd.IsWindow() ){
		return 0;
	}
	m_tarz_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_camera.pos.x );
	if( (g_endapp == 1) || !m_posx_wnd.IsWindow() ){
		return 0;
	}
	m_posx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_camera.pos.y );
	if( (g_endapp == 1) || !m_posy_wnd.IsWindow() ){
		return 0;
	}
	m_posy_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_camera.pos.z );
	if( (g_endapp == 1) || !m_posz_wnd.IsWindow() ){
		return 0;
	}
	m_posz_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_camera.dist );
	if( (g_endapp == 1) || !m_dist_wnd.IsWindow() ){
		return 0;
	}
	m_dist_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_camera.up.x );
	if( (g_endapp == 1) || !m_upx_wnd.IsWindow() ){
		return 0;
	}
	m_upx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_camera.up.y );
	if( (g_endapp == 1) || !m_upy_wnd.IsWindow() ){
		return 0;
	}
	m_upy_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_camera.up.z );
	if( (g_endapp == 1) || !m_upz_wnd.IsWindow() ){
		return 0;
	}
	m_upz_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_camera.nearz );
	if( (g_endapp == 1) || !m_near_wnd.IsWindow() ){
		return 0;
	}
	m_near_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%f", m_camera.farz );
	if( (g_endapp == 1) || !m_far_wnd.IsWindow() ){
		return 0;
	}
	m_far_wnd.SetWindowText( tempchar );

	m_ortho = m_camera.ortho;//!!!!!!!!

	if( m_ortho == 0 ){
		sprintf_s( tempchar, 1024, "%f", m_camera.fov );
		if( (g_endapp == 1) || !m_fov_wnd.IsWindow() ){
			return 0;
		}
		m_fov_wnd.SetWindowText( tempchar );
	}else{
		sprintf_s( tempchar, 1024, "%f", m_camera.orthosize );
		if( (g_endapp == 1) || !m_fov_wnd.IsWindow() ){
			return 0;
		}
		m_fov_wnd.SetWindowText( tempchar );
	}

	sprintf_s( tempchar, 1024, "%f", g_inidata->camdlg_move );
	m_drag_wnd.SetWindowText( tempchar );


//	if( m_dlg && m_dlg->m_shandler && (m_jointnum > 0) && (m_camera.boneno > 0) && (m_camera.boneno < m_dlg->m_shandler->s2shd_leng) ){
//		if( (g_endapp == 1) || !m_combobone_wnd.IsWindow() ){
//			return 0;
//		}
//		m_combobone_wnd.SendMessage( CB_SETCURSEL, m_camera.boneno, 0 );
//	}else{
//		m_camera.boneno = -1;
//		if( (g_endapp == 1) || !m_combobone_wnd.IsWindow() ){
//			return 0;
//		}
//		m_combobone_wnd.SendMessage( CB_SETCURSEL, 0, 0 );
//	}
//	m_combomode_wnd.SendMessage( CB_SETCURSEL, m_camera.looktype, 0 );
	if( (g_endapp == 1) || !m_combog_wnd.IsWindow() ){
		return 0;
	}
	m_combog_wnd.SendMessage( CB_SETCURSEL, m_gtype, 0 );


	if( m_fillupup ){
		::CheckDlgButton( m_dlg_wnd, IDC_FILLUPUP, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_FILLUPUP, BST_UNCHECKED );
	}
	
	if( (g_endapp == 0) && (m_dlg->m_timerworking == 1) && m_dlg->m_grotdlg ){
		m_dlg->m_grotdlg->Redraw();
	}

	SetEditButton();
	SetProjButton();
	SetCameraButton();

	return 0;
}

LRESULT CCamKeyDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}

LRESULT CCamKeyDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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

	SetCurrentMotion( 0 );

	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	Redraw();

	if( (idCtrl != 999) && (m_app->m_syncdlg->m_camflag != 0) ){
		int quaflag = m_app->m_syncdlg->m_quaflag;
		int gpaflag = m_app->m_syncdlg->m_gpaflag;
		int moeflag = m_app->m_syncdlg->m_moeflag;
		int sndflag = m_app->m_syncdlg->m_sndflag;

		g_motdlg->OnJumpFrame( m_current, quaflag, 0, gpaflag, moeflag, sndflag );
	}


	return 0;
}
LRESULT CCamKeyDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
void CCamKeyDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
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


int CCamKeyDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
	*posptr = MP_SIZE + (int)(step * (float)( frameno - m_startframe ) ) + clirect.left;
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	
	return 0;
}

int CCamKeyDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
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

int CCamKeyDlg::DrawCameraKeys()
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
	if( !m_mp_wnd.IsWindow() ){
		return 0;
	}

	HDC hdc;
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
	float centerstep;
	centerstep = (float)clirect.bottom / 4.0f;
	int ceny[CAMERANUMMAX];
	int camno;
	SelectObject( hdc, m_black_brush );
	for( camno = 0; camno < CAMERANUMMAX; camno++ ){
		ceny[camno] = (int)( centerstep * (float)(camno + 1) );
		::MoveToEx( hdc, clirect.left, ceny[camno], NULL ); 
		::LineTo( hdc, clirect.right, ceny[camno] );
	}

//current bar
	int barposx;
	Frameno2Windowpos( clirect, m_current, &barposx );

	::MoveToEx( hdc, barposx, clirect.top, NULL );
	::LineTo( hdc, barposx, clirect.bottom );

//点
	if( !m_cah ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_mp_wnd.ReleaseDC( hdc );
		return 0;
	}
	for( camno = 0; camno < CAMERANUMMAX; camno++ ){
		CCameraKey* curck = anim->GetFirstCameraKey( camno );
		while( curck ){
			int frameno = curck->m_frameno;
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
				int posy = ceny[ camno ];
				::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );
			}

			curck = curck->next;
		}
	}
	
	if( m_dragck ){
		int frameno = m_dragck->m_frameno;
		
		SelectObject( hdc, m_blue_pen );
		SelectObject( hdc, m_blue_brush );
		
		//int posx = MP_SIZE + (int)(step * (float)(frameno));
		int posx;
		Frameno2Windowpos( clirect, frameno, &posx );
		int posy = ceny[ m_dragcamno ];
		::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );		
	}
	

	::SelectObject( hdc, hOldPen );
	::SelectObject( hdc, hOldBrush );
	m_mp_wnd.ReleaseDC( hdc );

	return 0;
}

int CCamKeyDlg::DrawSWKeys()
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
	if( !m_sw_wnd.IsWindow() ){
		return 0;
	}
	hdc = m_sw_wnd.GetDC();
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
	if( !m_sw_wnd.IsWindow() ){
		return 0;
	}
	m_sw_wnd.GetClientRect( &clirect );
	HPEN hOldPen = (HPEN)SelectObject( hdc, m_black_pen );
	HBRUSH hOldBrush = (HBRUSH)SelectObject( hdc, m_white_brush );

	BOOL bret;
	bret = ::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret ){
		_ASSERT( 0 );
		return 1;
	}

//センターライン
	float centerstep;
	centerstep = (float)clirect.bottom / 4.0f;
	int ceny[CAMERANUMMAX];
	int camno;
	SelectObject( hdc, m_black_brush );
	for( camno = 0; camno < CAMERANUMMAX; camno++ ){
		ceny[camno] = (int)( centerstep * (float)(camno + 1) );
		::MoveToEx( hdc, clirect.left, ceny[camno], NULL ); 
		::LineTo( hdc, clirect.right, ceny[camno] );
	}

//current bar
	int barposx;
	Frameno2Windowpos( clirect, m_current, &barposx );

	::MoveToEx( hdc, barposx, clirect.top, NULL );
	::LineTo( hdc, barposx, clirect.bottom );

//点
	if( !m_cah ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_sw_wnd.ReleaseDC( hdc );
		return 0;
	}
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		m_sw_wnd.ReleaseDC( hdc );
		return 0;
	}
	CCameraSWKey* curcswk = anim->GetFirstSWKey();
	CCameraSWKey* nextcswk = 0;
	while( curcswk ){
		int frameno = curcswk->m_frameno;
		nextcswk = curcswk->next;

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
			int posy = ceny[ curcswk->m_camerano ];
			::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );


			//次のキーまでの間は太線
			SelectObject( hdc, m_black_pen );
			SelectObject( hdc, m_black_brush );
			int endposx;
			if( nextcswk ){
				int endframe;
				endframe = nextcswk->m_frameno;
				endframe = min( endframe, m_endframe );
				Frameno2Windowpos( clirect, endframe, &endposx );
			}else{
				Frameno2Windowpos( clirect, m_endframe, &endposx );
			}
			::Rectangle( hdc, posx, posy - 1, endposx, posy + 1 );
		}

		curcswk = nextcswk;
	}
	
	if( m_dragcswk ){
		int frameno = m_dragcswk->m_frameno;
		
		SelectObject( hdc, m_blue_pen );
		SelectObject( hdc, m_blue_brush );
		
		//int posx = MP_SIZE + (int)(step * (float)(frameno));
		int posx;
		Frameno2Windowpos( clirect, frameno, &posx );
		int posy = ceny[ m_swdragcamno ];
		::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );		
	}
	

	::SelectObject( hdc, hOldPen );
	::SelectObject( hdc, hOldBrush );
	m_sw_wnd.ReleaseDC( hdc );

	return 0;
}


int CCamKeyDlg::Redraw()
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
//		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}

LRESULT CCamKeyDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	
	DrawCameraKeys();
	DrawSWKeys();

	return 0;
}

LRESULT CCamKeyDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CCamKeyDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "CamKeyDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CCamKeyDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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




LRESULT CCamKeyDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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


LRESULT CCamKeyDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );


	BOOL dummyb;
	return m_dlg->OnPreview( 0, 0, 0, dummyb );

}

int CCamKeyDlg::CaptureM()
{

	RECT viewclient;
	RECT vc1, vc2;
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
	m_sw_wnd.GetWindowRect( &vc2 );
	viewclient.left = vc1.left;
	viewclient.right = vc1.right;
	viewclient.top = vc1.top;
	viewclient.bottom = vc2.bottom;


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


LRESULT CCamKeyDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	CaptureM();

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "CamKeyDlg : OnMouseMove : MK_LBUTTON\n" );

	if( (m_dragflag == 1) && m_dragck ){
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

		m_dragck->m_frameno = frameno;

		Redraw();
	}

	if( (m_swdragflag == 1) && m_dragcswk ){
		POINT movepoint;
		movepoint.x = LOWORD(lParam);
		movepoint.y = HIWORD(lParam);
		m_dlg_wnd.ClientToScreen( &movepoint );
		
		RECT winrect;
		m_sw_wnd.GetWindowRect( &winrect );
		int posx, frameno;
		posx = movepoint.x - winrect.left;
		RECT clirect;
		m_sw_wnd.GetClientRect( &clirect );		
		Windowpos2Frameno( clirect, posx, &frameno );

		m_dragcswk->m_frameno = frameno;

		Redraw();
	}

	CCameraAnim* anim;
	int animindex;
	anim = m_cah->GetCameraAnim( m_animno, &animindex );
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


LRESULT CCamKeyDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret, ret2;
	CAMERAELEM cpce;
	int cpinterp;
	CCameraKey* ckptr = 0;
	int camno;
	int existflag = 0;

	int cpok = 0;

	switch( m_camerabutton ){
	case CAMB_HEN:
		ret = m_app->GetCameraElem( &cpce );
		_ASSERT( !ret );
		cpinterp = m_cah->m_definterp;
		cpok = 1;
		break;
	case CAMB_0:
	case CAMB_1:
	case CAMB_2:
		ret = anim->ExistCameraKey( m_camerabutton - CAMB_0, m_current, &ckptr );
		_ASSERT( !ret );
		if( ret || !ckptr ){
			cpok = 0;
		}else{
			cpce = ckptr->m_celem;
			cpinterp = ckptr->m_interp;
			cpok = 1;
		}
		break;
	case CAMB_KAE:
		ret = anim->GetSWAnim( &camno, m_current, &existflag );
		if( !ret && (camno >= 0) ){
			ret2 = anim->ExistCameraKey( camno, m_current, &ckptr );
			_ASSERT( !ret2 );
			if( ret2 || !ckptr ){
				cpok = 0;
			}else{
				cpce = ckptr->m_celem;
				cpinterp = ckptr->m_interp;
				cpok = 1;
			}
		}else{
			cpok = 0;
		}
		break;
	default:
		_ASSERT( 0 );
		cpok = 0;
		break;
	}

	if( !cpok ){
		::MessageBox( m_hWnd, "このフレームにはコピーできるキーがありません。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!
	}

	if( m_cpck ){
		delete m_cpck;
		m_cpck = 0;
	}

	m_cpck = new CCameraKey();
	if( !m_cpck ){
		DbgOut( "CamKeyDlg : OnFrameCopy : cpck alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_cpck->SetCameraKey( &cpce );
	_ASSERT( !ret );
	ret = m_cpck->SetInterp( cpinterp );
	_ASSERT( !ret );


	return 0;
}

LRESULT CCamKeyDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	if( !m_cpck ){
		::MessageBox( m_hWnd, "フレームコピーを行ってから再試行してください。", "失敗", MB_OK );
		return 0;//!!!!!!!!!!!!!!!
	}

	int ret, ret2;
	int camno;
	int existflag = 0;
	switch( m_camerabutton ){
	case CAMB_HEN:
		ret = m_app->SetCameraElem( m_cpck->m_celem );
		_ASSERT( !ret );
		break;
	case CAMB_0:
	case CAMB_1:
	case CAMB_2:
		ret = anim->SetCameraKey( m_camerabutton - CAMB_0, m_current, m_cpck->m_celem, m_cpck->m_interp );
		_ASSERT( !ret );
		break;
	case CAMB_KAE:
		ret = anim->GetSWAnim( &camno, m_current, &existflag );
		if( !ret && (camno >= 0) ){
			ret2 = anim->SetCameraKey( camno, m_current, m_cpck->m_celem, m_cpck->m_interp );
			_ASSERT( !ret2 );
		}else{
			_ASSERT( 0 );
		}
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	ParamsToDlg();
	Redraw();
	return 0;
}

LRESULT CCamKeyDlg::OnDel0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = DeleteKey( 0 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnDel0 : DeleteKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}
LRESULT CCamKeyDlg::OnDel1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = DeleteKey( 1 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnDel1 : DeleteKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}
LRESULT CCamKeyDlg::OnDel2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = DeleteKey( 2 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnDel2 : DeleteKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}
int CCamKeyDlg::DeleteKey( int camno )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	if( m_editbutton == CAME_CAMERA ){
		ret = anim->DeleteCameraKey( camno, m_current );
		if( ret ){
			DbgOut( "CamKeyDlg : DeleteKey : ca DeleteCameraKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		ret = anim->DeleteSWKey( m_current );
		if( ret ){
			DbgOut( "CamKeyDlg : DeleteKey : ca DeleteSWKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}


LRESULT CCamKeyDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	if( m_dragflag || m_swdragflag )
		return 0;

	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
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
		RECT winrectsw;
		m_sw_wnd.GetWindowRect( &winrectsw );

		if( (clickpoint.x >= winrectmp.left) && (clickpoint.x <= winrectmp.right) 
			&& (clickpoint.y >= winrectmp.top) && (clickpoint.y <= winrectmp.bottom) ){

			float censtep;
			int centery[3], uppery[3], lowery[3];
			int camno;

			censtep = (float)( winrectmp.bottom - winrectmp.top ) / 4.0f;
			for( camno = 0; camno < 3; camno++ ){
				centery[camno] = winrectmp.top + (int)( censtep * (float)( camno + 1 ) );
				uppery[camno] = centery[camno] - MP_SIZE;
				lowery[camno] = centery[camno] + MP_SIZE;
			}

			int selcamera = -1;
			for( camno = 0; camno < 3; camno++ ){
				if( (clickpoint.y >= uppery[camno]) && (clickpoint.y <= lowery[camno]) ){
					selcamera = camno;
					break;
				}
			}
			if( selcamera >= 0 ){
				RECT clirect;
				m_mp_wnd.GetClientRect( &clirect );
				int posx, frameno;
				posx = clickpoint.x - winrectmp.left;
				Windowpos2Frameno( clirect, posx, &frameno );

				CCameraKey* ckptr = 0;
				ret = anim->ExistCameraKey( selcamera, frameno, &ckptr );
				if( !ret && ckptr ){
					m_dragcamno = selcamera;
					m_dragframe = frameno;
					m_dragflag = 1;
					m_dragck = ckptr;

					anim->LeaveFromChainCameraKey( selcamera, frameno );

				}else{
					m_dragcamno = -1;
					m_dragframe = 0;
					m_dragflag = 0;
					m_dragck = 0;
				}
			}else{
				m_dragcamno = -1;
				m_dragframe = 0;
				m_dragflag = 0;
				m_dragck = 0;
			}

			m_swdragcamno = -1;
			m_swdragframe = 0;
			m_swdragflag = 0;
			m_dragcswk = 0;

		}else if( (clickpoint.x >= winrectsw.left) && (clickpoint.x <= winrectsw.right) 
			&& (clickpoint.y >= winrectsw.top) && (clickpoint.y <= winrectsw.bottom) ){

			float censtep;
			int centery[3], uppery[3], lowery[3];
			int camno;

			censtep = (float)( winrectsw.bottom - winrectsw.top ) / 4.0f;
			for( camno = 0; camno < 3; camno++ ){
				centery[camno] = winrectsw.top + (int)( censtep * (float)( camno + 1 ) );
				uppery[camno] = centery[camno] - MP_SIZE;
				lowery[camno] = centery[camno] + MP_SIZE;
			}

			int selcamera = -1;
			for( camno = 0; camno < 3; camno++ ){
				if( (clickpoint.y >= uppery[camno]) && (clickpoint.y <= lowery[camno]) ){
					selcamera = camno;
					break;
				}
			}
			if( selcamera >= 0 ){
				RECT clirect;
				m_mp_wnd.GetClientRect( &clirect );
				int posx, frameno;
				posx = clickpoint.x - winrectsw.left;
				Windowpos2Frameno( clirect, posx, &frameno );

				CCameraSWKey* cswkptr = 0;
				ret = anim->ExistSWKey( frameno, &cswkptr );
				if( !ret && cswkptr ){
					m_swdragcamno = selcamera;
					m_swdragframe = frameno;
					m_swdragflag = 1;
					m_dragcswk = cswkptr;

					anim->LeaveFromChainSWKey( frameno );

				}else{
					m_swdragcamno = -1;
					m_swdragframe = 0;
					m_swdragflag = 0;
					m_dragcswk = 0;
				}
			}else{
				m_swdragcamno = -1;
				m_swdragframe = 0;
				m_swdragflag = 0;
				m_dragcswk = 0;
			}

			m_dragcamno = -1;
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragck = 0;

		}else{
			m_dragcamno = -1;
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragck = 0;

			m_swdragcamno = -1;
			m_swdragframe = 0;
			m_swdragflag = 0;
			m_dragcswk = 0;
		}
	}

	return 0;
}


LRESULT CCamKeyDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	static int busyflag = 0;


	if( busyflag == 1 )
		return 0;

	busyflag = 1;

	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
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

		if( (m_dragflag == 1) && m_dragck ){
			CCameraKey* isexist = 0;

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

			ret = anim->ExistCameraKey( m_dragcamno, frameno, &isexist );
			_ASSERT( !ret );
			if( isexist ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "フレーム番号%d には既にキーフレームが存在するので、移動できません。",
					frameno );
				MessageBox( messtr, "移動できません", MB_OK );
				frameno = m_dragframe;//!!!!! 元に戻す
			}
			
			m_dragck->m_frameno = frameno;//!!!!!!!!!!!!!!!!

			ret = anim->ChainCameraKey( m_dragcamno, m_dragck );
			if( ret ){
				DbgOut( "CamKeyDlg : OnLButtonUp : ca ChainCameraKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_dragck = 0;

			ParamsToDlg();
			Redraw();
		}else if( (m_swdragflag == 1) && m_dragcswk ){
			CCameraSWKey* isexist = 0;

			POINT droppoint;
			droppoint.x = LOWORD(lParam);
			droppoint.y = HIWORD(lParam);
			m_dlg_wnd.ClientToScreen( &droppoint );
			
			RECT winrect;
			m_sw_wnd.GetWindowRect( &winrect );
			int posx;
			posx = droppoint.x - winrect.left;
			RECT clirect;
			m_sw_wnd.GetClientRect( &clirect );		
			Windowpos2Frameno( clirect, posx, &frameno );

			ret = anim->ExistSWKey( frameno, &isexist );
			_ASSERT( !ret );
			if( isexist ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "フレーム番号%d には既にキーフレームが存在するので、移動できません。",
					frameno );
				MessageBox( messtr, "移動できません", MB_OK );
				frameno = m_dragframe;//!!!!! 元に戻す
			}
			
			m_dragcswk->m_frameno = frameno;//!!!!!!!!!!!!!!!!

			ret = anim->ChainSWKey( m_dragcswk );
			if( ret ){
				DbgOut( "CamKeyDlg : OnLButtonUp : ca ChainSWKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_dragcswk = 0;

			ParamsToDlg();
			Redraw();
		}
	}

	m_dragcamno = -1;
	m_dragflag = 0;
	m_dragframe = 0;

	m_swdragcamno = -1;
	m_swdragflag = 0;
	m_swdragframe = 0;


	busyflag = 0;

	return 0;
}
LRESULT CALLBACK MPProcCam(
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
		//	DbgOut( "CamKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "CamKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}
/***
		int key = (int)wParam;
		if( s_camkeydlg && (uMsg == WM_KEYUP) && ((key == VK_CONTROL) || (key == VK_RMENU) || (key == VK_LMENU)) ){
			s_camkeydlg->SetEditButton();
			s_camkeydlg->SetProjButton();
			s_camkeydlg->SetCameraButton();
		}
***/
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
int CCamKeyDlg::SetNewWindowProc()
{
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWL_WNDPROC );
	if( !s_mpfunc ){
		_ASSERT( 0 );
		return 1;
	}

	LONG lret;
	lret = ::SetWindowLong( m_sl_mp_wnd, GWL_WNDPROC, (LONG)MPProcCam );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}


LRESULT CCamKeyDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		
	bHandled = TRUE;
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;

	int timerid = (int)wParam;
	if( (timerid == CamKeyDlgTIMER) && (m_cmdshow == SW_SHOW) ){

		//ドラッグ中に、マウスが外に出た場合は、ドラッグ操作を強制終了する。
		if( (m_dragflag == 1) || (m_swdragflag == 1) ){
			POINT ptCursor;
			POINT savecursor;
			GetCursorPos( &ptCursor );
			
			savecursor = ptCursor;//
			::ScreenToClient( m_hWnd, &ptCursor );

			RECT dlgrectmp, dlgrectsw;
			//m_dlg_wnd.GetWindowRect( &dlgrect );
			m_mp_wnd.GetWindowRect( &dlgrectmp );
			m_sw_wnd.GetWindowRect( &dlgrectsw );

			if( (savecursor.x < dlgrectmp.left) || (savecursor.x > dlgrectmp.right) 
				|| (savecursor.y < dlgrectmp.top) || (savecursor.y > dlgrectsw.bottom)
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


int CCamKeyDlg::ShowDlg( int cmdshow, int stopflag )
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
		//m_combomode_wnd.EnableWindow( 0 );
		//m_combobone_wnd.EnableWindow( 0 );

		g_inidata->opt_bmdisp = 0;
		g_inidata->opt_jointdisp = 0;

	}


	return 0;
}

LRESULT CCamKeyDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	int ret;
	ret = SetKeyNo();
	if( ret ){
		DbgOut( "CamKeyDlg : OnBeforeMP : SetKeyNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int findframe = -1;
	int chkframe;

	if( m_editbutton == CAME_CAMERA ){
		for( chkframe = (m_current - 1); chkframe >= m_startframe; chkframe-- ){
			if( *( m_totalkeyno + chkframe ) == 1 ){
				findframe = chkframe;
				break;
			}
		}
	}else{
		int chkkey;
		for( chkkey = 0; chkkey < m_swkeygetnum; chkkey++ ){
			chkframe = *( m_swkeyno + chkkey );
			if( chkframe < 0 ){
				break;
			}
			if( chkframe < m_current ){
				findframe = chkframe;
			}
			if( chkframe >= m_current ){
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
LRESULT CCamKeyDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;

	int ret;
	ret = SetKeyNo();
	if( ret ){
		DbgOut( "CamKeyDlg : OnNextMP : SetKeyNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int findframe = -1;
	int chkframe;

	if( m_editbutton == CAME_CAMERA ){
		for( chkframe = (m_current + 1); chkframe <= m_endframe; chkframe++ ){
			if( *( m_totalkeyno + chkframe ) == 1 ){
				findframe = chkframe;
				break;
			}
		}
	}else{
		int chkkey;
		for( chkkey = (m_swkeygetnum - 1); chkkey >= 0; chkkey-- ){
			chkframe = *( m_swkeyno + chkkey );
			if( chkframe < 0 ){
				break;
			}
			if( chkframe > m_current ){
				findframe = chkframe;
			}
			if( chkframe <= m_current ){
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

LRESULT CCamKeyDlg::OnCreate0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = CreateKey( 0 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnCreate0 : CreateKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}
LRESULT CCamKeyDlg::OnCreate1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = CreateKey( 1 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnCreate1 : CreateKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}
LRESULT CCamKeyDlg::OnCreate2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = CreateKey( 2 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnCreate2 : CreateKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}

int CCamKeyDlg::CreateKey( int camno )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	int ret;
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	_ASSERT( camno >= 0 );

	CCameraKey* ckptr = 0;
	ret = anim->ExistCameraKey( camno, m_current, &ckptr );
	if( ret ){
		DbgOut( "camkeydlg : CreateKey : anim ExistCameraKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	int setfu = -1;
	if( ckptr ){
		setfu = ckptr->m_interp;
		if( setfu >= 100 ){
			setfu -= 100;
		}
	}else{
		setfu = *(anim->m_definterptr);
	}
	if( m_fillupup ){
		setfu += 100;
	}

	if( m_editbutton == CAME_CAMERA ){
		ret = anim->SetCameraKey( camno, m_current, m_camera, setfu );
		if( ret ){
			DbgOut( "CamKeyDlg : CreateKey : ca SetCameraKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		ret = anim->SetSWKey( m_current, camno );
		if( ret ){
			DbgOut( "CamKeyDlg : CreateKey : ca SetSWKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();

	Redraw();

	return 0;
}

LRESULT CCamKeyDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CCamKeyDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}
int CCamKeyDlg::SetNewCamera()
{
	int ret;
	static int busyflag = 0;

	if( busyflag != 0 ){
		return 0;
	}

	busyflag = 1;

	CCameraAnim* anim;

	if( m_cah->m_animnum >= 1 ){
		switch( m_camerabutton ){
		case CAMB_HEN:
			break;
		case CAMB_0:
		case CAMB_1:
		case CAMB_2:
			ret = m_cah->SetNewCamera( m_camerabutton - CAMB_0 );
			_ASSERT( !ret );
			m_animno = m_cah->m_animno;
			m_camera = m_cah->m_curcamera;
			anim = m_cah->GetCameraAnim( m_animno );
			if( anim ){
				m_current = anim->m_frameno;
			}
			ParamsToDlg();
			break;
		case CAMB_KAE:
			ret = m_cah->SetNewCamera( -1 );
			_ASSERT( !ret );
			m_animno = m_cah->m_animno;
			m_camera = m_cah->m_curcamera;
			anim = m_cah->GetCameraAnim( m_animno );
			if( anim ){
				m_current = anim->m_frameno;
			}
			ParamsToDlg();
			break;
		default:
			break;
		}
	}

	busyflag = 0;
	return 0;
}


int CCamKeyDlg::SetCurrentMotion( int isfirst )
{
	int ret;
	_ASSERT( m_animno >= 0 );
	int camno = -1;
	int existflag = 0;

	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );

	switch( m_camerabutton ){
	case CAMB_HEN:
		m_app->GetCameraElem( &m_camera );
		break;
	case CAMB_0:
		camno = 0;
		break;
	case CAMB_1:
		camno = 1;
		break;
	case CAMB_2:
		camno = 2;
		break;
	case CAMB_KAE:
		if( anim ){
			ret = anim->GetSWAnim( &camno, m_current, &existflag );
			_ASSERT( !ret );
		}else{
			camno = -1;
		}
		break;
	default:
		m_camerabutton = CAMB_0;
		camno = 0;
		break;
	}

	if( m_camerabutton != CAMB_HEN ){
		ret = m_cah->SetAnimFrameNo( m_animno, camno, m_current, isfirst );
		_ASSERT( !ret );
		m_camera = m_cah->m_curcamera;


		CCameraAnim* anim;
		anim = m_cah->GetCameraAnim( m_animno );
		if( anim ){
			CCameraKey* camkptr = 0;
			if( camno >= 0 ){
				ret = anim->ExistCameraKey( camno, m_current, &camkptr );
				if( ret ){
					DbgOut( "camkeydlg : SetCurrentMotion : anim ExistCameraKey error !!!\n" );
					_ASSERT( 0 );
				}
			}
			if( camkptr ){
				int fudiv;
				fudiv = camkptr->m_interp / 100;
				if( fudiv == 1 ){
					m_fillupup = 1;
				}else{
					m_fillupup = 0;
				}
			}
		}
	}


	return 0;
}

int CCamKeyDlg::CreateKeyNo()
{
//	int* m_keyno[ CAMERANUMMAX ];
//	int* m_totalkeyno;
//	int m_keynoleng;

	int camno;
	
	if( m_keynoleng != (m_sl_mp_rangemax + 1) ){
		DestroyKeyNo();

		m_keynoleng = m_sl_mp_rangemax + 1;

		for( camno = 0; camno < CAMERANUMMAX; camno++ ){
			m_keyno[ camno ] = (int*)malloc( sizeof( int ) * m_keynoleng );
			if( !m_keyno[camno] ){
				DbgOut( "camkeydlg : CreateKeyNo : keyno alloc error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		m_swkeyno = (int*)malloc( sizeof( int ) * m_keynoleng );
		if( !m_swkeyno ){
			DbgOut( "camkeydlg : CreateKeyNo : swkeyno alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		m_totalkeyno = (int*)malloc( sizeof( int ) * m_keynoleng );
		if( !m_totalkeyno ){
			DbgOut( "camkeydlg : CreateKeyNo : totalkeyno alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	InitKeyNo();

	return 0;
}
int CCamKeyDlg::InitKeyNo()
{
	int camno;
	if( m_keynoleng > 0 ){
		for( camno = 0; camno < CAMERANUMMAX; camno++ ){
			int frameno;
			for( frameno = 0; frameno < m_keynoleng; frameno++ ){
				*( m_keyno[camno] + frameno ) = -1;
				*( m_swkeyno + frameno ) = -1;
			}
		}

		ZeroMemory( m_totalkeyno, sizeof( int ) * m_keynoleng );
	}
	m_swkeygetnum = 0;

	return 0;
}

int CCamKeyDlg::DestroyKeyNo()
{
	int camno;
	for( camno = 0; camno < CAMERANUMMAX; camno++ ){
		if( m_keyno[camno] ){
			free( m_keyno[camno] );
			m_keyno[camno] = 0;
		}
	}

	if( m_swkeyno ){
		free( m_swkeyno );
		m_swkeyno = 0;
	}

	if( m_totalkeyno ){
		free( m_totalkeyno );
		m_totalkeyno = 0;
	}

	m_swkeygetnum = 0;
	m_keynoleng = 0;

	return 0;
}
int CCamKeyDlg::SetKeyNo()
{
	int ret;

	ret = CreateKeyNo();
	if( ret ){
		DbgOut( "CamKeyDlg : SetKeyNo : CreateKeyNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		return 0;
	}

	if( m_keynoleng <= 0 ){
		return 0;
	}

	int camno;
	for( camno = 0; camno < CAMERANUMMAX; camno++ ){
		int getnum = 0;
		ret = anim->GetCameraKeyframeNoRange( camno, 0, m_sl_mp_rangemax, m_keyno[camno], m_keynoleng, &getnum );
		if( ret ){
			DbgOut( "CamKeyDlg : SetKeyNo : ca GetCameraKeyframeNoRange error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( getnum > 0 ){
			int keyno;
			for( keyno = 0; keyno < getnum; keyno++ ){
				int frameno = *( m_keyno[camno] + keyno );
				*( m_totalkeyno + frameno ) = 1;
			}
		}
	}

	m_swkeygetnum = 0;
	ret = anim->GetSWKeyframeNoRange( 0, m_sl_mp_rangemax, m_swkeyno, m_keynoleng, &m_swkeygetnum );
	if( ret ){
		DbgOut( "CamKeyDlg : SetKeyNo : ca GetSWKeyframeNoRange error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CCamKeyDlg::ResetParams( int atshowflag )
{
	
	int ret;

	DestroyObjs( 0 );

	ret = InitDlg( atshowflag );
	if( ret ){
		DbgOut( "camkeydlg : ResetParams : InitDlg error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = InitCombo();
	if( ret ){
		DbgOut( "camkeydlg : ResetParams : InitCombo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	SetCurrentMotion( 0 );

	return 0;
}

LRESULT CCamKeyDlg::OnSelHen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_camerabutton = CAMB_HEN;
	SetCameraButton();

	int ret;
	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );
	ParamsToDlg();

	return 0;
}
LRESULT CCamKeyDlg::OnSel1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_camerabutton = CAMB_0;
	SetCameraButton();

	int ret;
	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );
	ParamsToDlg();

	return 0;
}
LRESULT CCamKeyDlg::OnSel2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_camerabutton = CAMB_1;
	SetCameraButton();

	int ret;
	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );
	ParamsToDlg();

	return 0;
}
LRESULT CCamKeyDlg::OnSel3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_camerabutton = CAMB_2;
	SetCameraButton();

	int ret;
	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );
	ParamsToDlg();

	return 0;
}

int CCamKeyDlg::SetCameraKAE()
{
	BOOL dummy;
	OnSelKae( 0, 0, 0, dummy );

	return 0;
}
LRESULT CCamKeyDlg::OnSelKae(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_camerabutton = CAMB_KAE;
	SetCameraButton();

	int ret;
	ret = SetCurrentMotion( 0 );
	_ASSERT( !ret );
	ParamsToDlg();

	return 0;
}

LRESULT CCamKeyDlg::OnProjPers(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_ortho = 0;
	m_camera.ortho = 0;
	ParamsToDlg();
	return 0;
}
LRESULT CCamKeyDlg::OnProjOrtho(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_ortho = 1;
	m_camera.ortho = 1;
	ParamsToDlg();
	return 0;
}

LRESULT CCamKeyDlg::OnEditCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editbutton = CAME_CAMERA;
	SetEditButton();
	return 0;
}
LRESULT CCamKeyDlg::OnEditSW(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editbutton = CAME_SW;
	SetEditButton();
	return 0;
}

/***
LRESULT CCamKeyDlg::OnComboMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_combomode_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	m_camera.looktype = combono;

	return 0;
}
LRESULT CCamKeyDlg::OnComboBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_combobone_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	_ASSERT( m_combo2bone );

	if( combono == 0 ){
		m_camera.boneno = -1;
	}else{
		m_camera.boneno = *( m_combo2bone + combono );
	}

	return 0;
}
***/
LRESULT CCamKeyDlg::OnComboG(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

LRESULT CCamKeyDlg::OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CCameraAnim* anim;
	int animindex;
	anim = m_cah->GetCameraAnim( m_animno, &animindex );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int ret = 0;
	int dlgret;
	
	CMotSettingDlg	dlg( 0, anim->m_animname, m_sl_mp_rangemax + 1, anim->m_animtype, m_cah->m_definterp, anim->m_motjump, m_startframe, m_endframe );
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
				ret = m_cah->SetTotalFrame( m_animno, newrangemax + 1 );
				if( ret ){
					DbgOut( "CamKeyDlg : OnSetting : cah SetTotalFrame error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				m_sl_mp_rangemax = newrangemax;

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
		if( m_cah->m_definterp != dlg.m_interpolation ){
			m_cah->m_definterp = dlg.m_interpolation;
		}

		int cmp;
		cmp = strcmp( anim->m_animname, dlg.m_name );
		if( cmp != 0 ){
			strcpy_s( anim->m_animname, 256, dlg.m_name );
			::SendMessage( m_dlg->m_apphwnd, WM_USER_CAMERAMENU, animindex, 0 );
		}

		ParamsToDlg();
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
		OnReleasedcaptureSl_mp( 0, 0, dummy );

	}

	::SendMessage( m_dlg->m_apphwnd, WM_USER_CAMERAMENU, animindex, 0 );

	return 0;
}
LRESULT CCamKeyDlg::OnApplyDist(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = ApplyParams( 1 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnApplyDist : ApplyParams error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CCamKeyDlg::OnApplyDrag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	ret = GetFloat( m_drag_wnd, &tmpfloat );
	if( ret || (tmpfloat < 1.0f) || (tmpfloat > 10000.0f) ){
		::MessageBox( m_hWnd, "drag量は1.0から10000.0までです。", "入力エラー", MB_OK );
		return 0;
	}

	g_inidata->camdlg_move = tmpfloat;

	return 0;
}


LRESULT CCamKeyDlg::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = ApplyParams( 0 );
	if( ret ){
		DbgOut( "CamKeyDlg : OnApply : ApplyParams error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CCamKeyDlg::ApplyParams( int distflag )
{
	int ret, ret2;
	float tmpfloat, tmpfloat2, tmpfloat3;
	char mes[512];

	//target
	ret = GetFloat( m_tarx_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "target Xの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_tary_wnd, &tmpfloat2 );
	if( ret ){
		strcpy_s( mes, 512, "target Yの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_tarz_wnd, &tmpfloat3 );
	if( ret ){
		strcpy_s( mes, 512, "target Zの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_camera.target = D3DXVECTOR3( tmpfloat, tmpfloat2, tmpfloat3 );


	//up
	ret = GetFloat( m_upx_wnd, &tmpfloat );
	if( ret ){
		strcpy_s( mes, 512, "up Xの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_upy_wnd, &tmpfloat2 );
	if( ret ){
		strcpy_s( mes, 512, "up Yの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_upz_wnd, &tmpfloat3 );
	if( ret ){
		strcpy_s( mes, 512, "up Zの値が不正です。小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	D3DXVECTOR3 nup( tmpfloat, tmpfloat2, tmpfloat3 );
	D3DXVec3Normalize( &nup, &nup );
	m_camera.up = nup;

	if( distflag == 0 ){
		//pos
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
		m_camera.pos = D3DXVECTOR3( tmpfloat, tmpfloat2, tmpfloat3 );

		//dist
		D3DXVECTOR3 diff;
		diff = m_camera.pos - m_camera.target;
		if( diff != D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ){
			m_camera.dist = D3DXVec3Length( &diff );
		}else{
			m_camera.dist = 0.0f;
		}
	}else{
		//dist
		ret = GetFloat( m_dist_wnd, &tmpfloat );
		if( ret || (tmpfloat <= 0.0f) ){
			strcpy_s( mes, 512, "distの値が不正です。０より大きい小数を指定してください。" );
			::MessageBox( m_hWnd, mes, "エラー", MB_OK );
			return 0;
		}
		m_camera.dist = tmpfloat;

		//pos
		D3DXVECTOR3 vec;
		vec = m_camera.pos - m_camera.target;
		if( vec != D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ){
			D3DXVec3Normalize( &vec, &vec );
			m_camera.pos = m_camera.target + vec * m_camera.dist;
		}else{
			D3DXVECTOR3 vec2( 0.0f, 0.0f, -1.0f );
			m_camera.pos = m_camera.target + vec2 * m_camera.dist;
		}
	}

	//near
	//far
	ret = GetFloat( m_near_wnd, &tmpfloat );
	ret2 = GetFloat( m_far_wnd, &tmpfloat2 );
	if( ret || ret2 || (tmpfloat <= 0.0f) || (tmpfloat2 <= 0.0f) ){
		strcpy_s( mes, 512, "proj nearまたはfarの値が不正です。０より大きい小数を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	if( tmpfloat >= tmpfloat2 ){
		strcpy_s( mes, 512, "proj nearとfarの値が不正です。near < far となる値を指定してください。" );
		::MessageBox( m_hWnd, mes, "エラー", MB_OK );
		return 0;
	}
	m_camera.nearz = tmpfloat;
	m_camera.farz = tmpfloat2;

	//fov
	ret = GetFloat( m_fov_wnd, &tmpfloat );

	if( m_ortho == 0 ){
		if( ret || (tmpfloat < 30.0f) || (tmpfloat > 80.0f) ){
			strcpy_s( mes, 512, "proj fovの値が不正です。30より大きく80より小さい小数を指定してください。" );
			::MessageBox( m_hWnd, mes, "エラー", MB_OK );
			return 0;
		}
		m_camera.fov = tmpfloat;
	}else{
		if( ret || (tmpfloat < 500.0f) ){
			strcpy_s( mes, 512, "proj orthosizeの値が不正です。500より大きい小数を指定してください。" );
			::MessageBox( m_hWnd, mes, "エラー", MB_OK );
			return 0;
		}
		m_camera.orthosize = tmpfloat;
	}
	ParamsToDlg();	


	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int camno, existflag;
	switch( m_camerabutton )
	{
	case CAMB_HEN:
		camno = -1;
		break;
	case CAMB_0:
	case CAMB_1:
	case CAMB_2:
		camno = m_camerabutton - CAMB_0;
		break;
	case CAMB_KAE:
		ret = anim->GetSWAnim( &camno, m_current, &existflag );
		if( ret || (camno < 0) ){
			DbgOut( "CamKeyDlg : ApplyParams : ca GetSWAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		break;
	default:
		camno = -1;
		break;
	}

	CCameraKey* ckptr = 0;
	if( camno >= 0 ){
		ret = anim->ExistCameraKey( camno, m_current, &ckptr );
		if( ret ){
			DbgOut( "camkeydlg : ApplyParams : anim ExistCameraKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	int setfu = -1;
	if( ckptr ){
		setfu = ckptr->m_interp;
		if( setfu >= 100 ){
			setfu -= 100;
		}
	}else{
		setfu = *(anim->m_definterptr);
	}
	if( m_fillupup ){
		setfu += 100;
	}

	switch( m_camerabutton ){
	case CAMB_HEN:
		ret = m_app->SetCameraElem( m_camera );
		_ASSERT( !ret );
		break;
	case CAMB_0:
	case CAMB_1:
	case CAMB_2:
	case CAMB_KAE:
		ret = anim->SetCameraKey( camno, m_current, m_camera, setfu );
		if( ret ){
			DbgOut( "CamKeyDlg : ApplyParams : ca SetCameraKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	return 0;
}


int CCamKeyDlg::SetCameraButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	switch( m_camerabutton ){
	case CAMB_HEN:
		::SendMessage( m_selhen_wnd, BM_SETSTATE, TRUE, 0 );
		::SendMessage( m_sel1_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel2_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel3_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_selkae_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case CAMB_0:
		::SendMessage( m_selhen_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel1_wnd, BM_SETSTATE, TRUE, 0 );
		::SendMessage( m_sel2_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel3_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_selkae_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case CAMB_1:
		::SendMessage( m_selhen_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel1_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel2_wnd, BM_SETSTATE, TRUE, 0 );
		::SendMessage( m_sel3_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_selkae_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case CAMB_2:
		::SendMessage( m_selhen_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel1_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel2_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel3_wnd, BM_SETSTATE, TRUE, 0 );
		::SendMessage( m_selkae_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case CAMB_KAE:
		::SendMessage( m_selhen_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel1_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel2_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_sel3_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_selkae_wnd, BM_SETSTATE, TRUE, 0 );
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	return 0;
}
int CCamKeyDlg::SetEditButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	switch( m_editbutton ){
	case CAME_CAMERA:
		::SendMessage( m_editcam_wnd, BM_SETSTATE, TRUE, 0 );
		::SendMessage( m_editsw_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case CAME_SW:
		::SendMessage( m_editcam_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_editsw_wnd, BM_SETSTATE, TRUE, 0 );
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	return 0;
}
int CCamKeyDlg::SetProjButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	switch( m_ortho ){
	case 0:
		::SendMessage( m_pers_wnd, BM_SETSTATE, TRUE, 0 );
		::SendMessage( m_ortho_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case CAME_SW:
		::SendMessage( m_pers_wnd, BM_SETSTATE, FALSE, 0 );
		::SendMessage( m_ortho_wnd, BM_SETSTATE, TRUE, 0 );
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	return 0;
}


int CCamKeyDlg::SetPreviewButton()
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

int CCamKeyDlg::GetCurCamera( CAMERAELEM* dstcamera )
{
	TargetToBone( dstcamera );

	return 0;
}

int CCamKeyDlg::GetCameraAnimNum()
{
	if( !IsWindow() )
		return 0;

	if( !m_cah ){
		return 0;
	}

	int retnum;
	retnum = m_cah->m_animnum;
	return retnum;
}

int CCamKeyDlg::LoadCameraFile( char* filename )
{
	int ret;
	if( !m_cah ){
		_ASSERT( 0 );
		return 0;
	}

	CCameraFile camerafile;
	int animno = -1;
	ret = camerafile.LoadCameraFile( filename, m_cah, &animno );
	if( ret ){
		DbgOut( "CamKeyDlg : LoadCameraFile : camerafile LoadCameraFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_cah->m_animnum > MAXMOTIONNUM ){
		DbgOut( "CamKeyDlg : LoadCameraFile : animnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_animno = animno;
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		DbgOut( "CamKeyDlg : LoadCameraFile : cah GetCameraAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_cah->SetCurrentAnim( m_animno );
	if( ret ){
		DbgOut( "CamKeyDlg : LoadCameraFile : cah SetCurrentAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_motid[ m_cah->m_animnum - 1 ].id = m_animno;
	char* enptr = 0;
	enptr = strrchr( filename, '\\' );
	if( enptr ){
		strcpy_s( m_motid[ m_cah->m_animnum - 1 ].filename, 256, enptr + 1 );
	}else{
		strcpy_s( m_motid[ m_cah->m_animnum - 1 ].filename, 256, filename );
	}

	m_sl_mp_rangemax = anim->m_maxframe;
	m_startframe = 0;
	m_endframe = m_sl_mp_rangemax;

	m_motid[ m_cah->m_animnum - 1 ].startframe = m_startframe;
	m_motid[ m_cah->m_animnum - 1 ].endframe = m_endframe;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	::SendMessage( m_dlg->m_apphwnd, WM_USER_CAMERAMENU, m_cah->m_animnum - 1, 0 );
	
	return 0;
}


MOTID* CCamKeyDlg::GetCameraMotIDArray()
{
	return m_motid;
}
char* CCamKeyDlg::GetCameraAnimName( int animno )
{
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( animno );
	if( !anim ){
		return 0;
	}
	return anim->m_animname;
}
int CCamKeyDlg::GetCurCameraAnimIndex()
{
	CCameraAnim* anim;
	int animindex = 0;
	anim = m_cah->GetCameraAnim( m_animno, &animindex );
	if( anim ){
		return animindex;
	}else{
		return 0;
	}
}
int CCamKeyDlg::GetCurCameraAnimNo()
{
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( anim ){
		return anim->m_motkind;
	}else{
		return 0;
	}
}
int CCamKeyDlg::SelectCameraAnim( int srcindex )
{
	if( !m_cah ){
		_ASSERT( 0 );
		return 0;
	}
	if( m_dlg->m_preview_flag ){
		return 0;
	}

	if( (srcindex < 0) || (srcindex >= m_cah->m_animnum) ){
		DbgOut( "CamKeyDlg : SelectCameraAnim : srcindex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	int tmpcookie;
	tmpcookie = m_motid[ srcindex ].id;
	if( tmpcookie < 0 ){
		DbgOut( "CamKeyDlg : SelectCameraAnim : tmpcookie error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

/////////
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnimByIndex( srcindex );
	if( !anim ){
		DbgOut( "CamKeyDlg : SelectCameraAnim : cah GetCameraAnimByIndex error !!!\n" );
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
	::SendMessage( m_dlg->m_apphwnd, WM_USER_CAMERAMENU, srcindex, 0 );


	return 0;
}

int CCamKeyDlg::DestroyCameraAnim( int delanimno )
{
	int ret;

	if( delanimno < 0 ){
		if( m_cah ){
			delete m_cah;
			m_cah = 0;
		}

		ret = InitDlg( 0 );
		if( ret ){
			DbgOut( "CamKeyDlg : DestroyCameraAnim all : InitDlg error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{
		_ASSERT( m_cah );
		if( m_cah->m_animnum <= 0 ){
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

		if( setno != (m_cah->m_animnum - 1) ){
			_ASSERT( 0 );
			return 1;
		}

		MoveMemory( m_motid, tempmotid, sizeof( MOTID ) * MAXMOTIONNUM );

		//m_motionnum -= 1;//!!!!!

		int ret;
		ret = m_cah->DestroyAnim( delcookie );
		if( ret ){
			DbgOut( "CamKeyDlg : DestroyCameraAnim cur : cah DestroyAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( m_cah->m_animnum <= 0 ){
			ret = InitDlg( 0 );
			if( ret ){
				DbgOut( "CamKeyDlg : DestroyCameraAnim cur : InitDlg error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = SelectCameraAnim( 0 );
		if( ret ){
			DbgOut( "CamKeyDlg : DestroyCameraAnim cur : SelectCameraAnim 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CCamKeyDlg::SetCurCamera( CAMERAELEM srccamera )
{
	int ret;

	m_camera = srccamera;

	if( m_cah->m_animnum <= 0 ){
		ret = InitDlg( 0 );
		if( ret ){
			DbgOut( "CamKeyDlg : SetCurCamera : InitDlg error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( m_animno );
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	int camno, existflag;
	switch( m_camerabutton )
	{
	case CAMB_HEN:
		camno = -1;
		break;
	case CAMB_0:
	case CAMB_1:
	case CAMB_2:
		camno = m_camerabutton - CAMB_0;
		break;
	case CAMB_KAE:
		ret = anim->GetSWAnim( &camno, m_current, &existflag );
		if( ret || (camno < 0) ){
			DbgOut( "CamKeyDlg : SetCurCamera : ca GetSWAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		break;
	default:
		camno = -1;
		break;
	}

	CCameraKey* ckptr = 0;
	if( camno >= 0 ){
		ret = anim->ExistCameraKey( camno, m_current, &ckptr );
		if( ret ){
			DbgOut( "camkeydlg : SetCurCamera : anim ExistCameraKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	int setfu = -1;
	if( ckptr ){
		setfu = ckptr->m_interp;
		if( setfu >= 100 ){
			setfu -= 100;
		}
	}else{
		setfu = *(anim->m_definterptr);
	}
	if( m_fillupup ){
		setfu += 100;
	}

	switch( m_camerabutton ){
	case CAMB_HEN:
		ret = m_app->SetCameraElem( m_camera );
		_ASSERT( !ret );
		break;
	case CAMB_0:
	case CAMB_1:
	case CAMB_2:
	case CAMB_KAE:
		ret = anim->SetCameraKey( camno, m_current, m_camera, setfu );
		_ASSERT( !ret );
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	SetCurrentMotion( 0 );
	ParamsToDlg();
	Redraw();

	return 0;
}

int CCamKeyDlg::CalcMotionPointOnFrame( int animno, int camno, int frameno, CCameraKey* ckptr, int* hasmpflag )
{
	int ret;
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( animno );
	if( !anim ){
		ckptr->m_celem = m_cah->m_defaultcameras[0];
		ckptr->m_interp = m_cah->m_definterp;
		ckptr->m_frameno = frameno;
		*hasmpflag = 0;
		return 0;
	}

	int chkcamno = 0;
	int existflag = 0;
	if( camno == -2 ){
		switch( m_camerabutton ){
		case CAMB_HEN:
			_ASSERT( 0 );
			return 1;
			break;
		case CAMB_0:
		case CAMB_1:
		case CAMB_2:
			chkcamno = m_camerabutton - CAMB_0;
			break;
		case CAMB_KAE:
			ret = anim->GetSWAnim( &chkcamno, frameno, &existflag );
			_ASSERT( !ret );
			break;
		default:
			_ASSERT( 0 );
			return 1;
			break;
		}
	}else if( camno == -1 ){
		ret = anim->GetSWAnim( &chkcamno, frameno, &existflag );
		_ASSERT( !ret );

	}else if( (camno >= 0) && (camno < CAMERANUMMAX) ){
		chkcamno = camno;
	}else{
		_ASSERT( 0 );
		return 1;
	}

	CAMERAELEM ce;
	int interp;
	ret = anim->GetCameraAnim( &ce, &interp, chkcamno, frameno, hasmpflag );
	if( ret ){
		DbgOut( "CamKeyDlg : CalcMotionPointOnFrame : ca GetCameraAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ckptr->m_celem = ce;
	ckptr->m_interp = interp;
	ckptr->m_frameno = frameno;

	return 0;
}


int CCamKeyDlg::ExistCameraKey( int animno, int camno, int frameno, CCameraKey** ppck )
{
	*ppck = 0;

	int ret;
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( animno );
	if( !anim ){
		return 0;
	}

	int chkcamno = 0;
	int existflag = 0;
	if( camno == -2 ){
		switch( m_camerabutton ){
		case CAMB_HEN:
			return 0;//!!!!!!!!!!!!!!!!!!
			break;
		case CAMB_0:
		case CAMB_1:
		case CAMB_2:
			chkcamno = m_camerabutton - CAMB_0;
			break;
		case CAMB_KAE:
			ret = anim->GetSWAnim( &chkcamno, frameno, &existflag );
			_ASSERT( !ret );
			break;
		default:
			_ASSERT( 0 );
			return 1;
			break;
		}
	}else if( camno == -1 ){
		ret = anim->GetSWAnim( &chkcamno, frameno, &existflag );
		_ASSERT( !ret );

	}else if( (camno >= 0) && (camno < CAMERANUMMAX) ){
		chkcamno = camno;
	}else{
		_ASSERT( 0 );
		return 1;
	}

	ret = anim->ExistCameraKey( chkcamno, frameno, ppck );
	if( ret ){
		DbgOut( "CamKeyDlg : ExistCameraKey : ca ExistCameraKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CCamKeyDlg::SetFillUpMode( int animno, int camno, int frameno, int setfu )
{
	int ret;
	CCameraAnim* anim;
	anim = m_cah->GetCameraAnim( animno );
	if( !anim ){
		return 0;
	}

	int chkcamno = 0;
	int existflag = 0;
	if( camno == -2 ){
		switch( m_camerabutton ){
		case CAMB_HEN:
			return 0;//!!!!!!!!!!!!!!!!!!
			break;
		case CAMB_0:
		case CAMB_1:
		case CAMB_2:
			chkcamno = m_camerabutton - CAMB_0;
			break;
		case CAMB_KAE:
			ret = anim->GetSWAnim( &chkcamno, frameno, &existflag );
			_ASSERT( !ret );
			break;
		default:
			_ASSERT( 0 );
			return 1;
			break;
		}
	}else if( camno == -1 ){
		chkcamno = -1;
	}else if( (camno >= 0) && (camno < CAMERANUMMAX) ){
		chkcamno = camno;
	}else{
		_ASSERT( 0 );
		return 1;
	}

	if( chkcamno >= 0 ){
		ret = anim->SetFillUpMode( chkcamno, frameno, setfu );
		_ASSERT( !ret );
	}else{
		int setcamno;
		for( setcamno = 0; setcamno < CAMERANUMMAX; setcamno++ ){
			ret = anim->SetFillUpMode( setcamno, frameno, setfu );
			_ASSERT( !ret );
		}
	}

	return 0;
}

int CCamKeyDlg::OnJumpFrame( int srcframe )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CCameraAnim* anim;
	int animindex;
	anim = m_cah->GetCameraAnim( m_animno, &animindex );
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

int CCamKeyDlg::SetStartAndEnd( int srcstart, int srcend )
{
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	CCameraAnim* anim;
	int animindex;
	anim = m_cah->GetCameraAnim( m_animno, &animindex );
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

int CCamKeyDlg::PlayAnim( int srcframe, int totalflag )
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

	TargetToBone( &m_camera );

	return 0;
}

LRESULT CCamKeyDlg::OnFillupUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_FILLUPUP );
	if( ischecked == BST_CHECKED ){
		m_fillupup = 1;
	}else{
		m_fillupup = 0;
	}

	return 0;
}

LRESULT CCamKeyDlg::OnChuChu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_targetdlg ){
		return 0;
	}

	int ret;
	int srcchk = 0;
	char* bonename = 0;
	
	if( m_camera.looktype == CAML_LOCK ){
		srcchk = 1;

		if( m_camera.hsid > 0 ){
			if( m_app->m_thandler && (m_camera.boneno > 0) && (m_camera.boneno < m_app->m_thandler->s2e_leng) ){
				CTreeElem2* curte = (*m_app->m_thandler)( m_camera.boneno );
				if( curte && curte->IsJoint() ){
					bonename = curte->name;
				}
			}
		}
	}

	ret = m_targetdlg->ShowDlg( SW_SHOW, srcchk, bonename, m_app->m_thandler, m_app->m_shandler );
	_ASSERT( !ret );

	return 0;
}

int CCamKeyDlg::OnUserSelchange( int selno )
{
	char* bonename = 0;
	if( m_app->m_thandler && (selno > 0) && (selno < m_app->m_thandler->s2e_leng) ){
		CTreeElem2* curte = (*m_app->m_thandler)( selno );
		if( curte && curte->IsJoint() ){
			bonename = curte->name;
		}
	}

	if( m_targetdlg && (m_targetdlg->m_cmdshow == SW_SHOW) ){
		m_targetdlg->SetBoneName( bonename );
	}

	return 0;
}

int CCamKeyDlg::ResetCamTargetBone()
{
	m_camera.looktype = CAML_NORMAL;
	m_camera.hsid = -1;
	m_camera.boneno = -1;

	return 0;
}

int CCamKeyDlg::TargetToBone( CAMERAELEM* dstcamera )
{
	if( (m_app->m_shandler) && (m_camera.looktype == CAML_LOCK) ){
		if( (m_camera.boneno > 0) && (m_camera.boneno < m_app->m_shandler->s2shd_leng) ){
			CShdElem* curse = (*m_app->m_shandler)( m_camera.boneno );
			if( curse && curse->IsJoint() ){
				int ret;
				D3DXVECTOR3 bonepos( 0.0f, 0.0f, 0.0f );
				ret = m_app->m_shandler->GetBonePos( m_camera.boneno, 1, 
					m_app->m_mhandler->m_curmotkind, m_app->m_mhandler->m_curframeno,
					&bonepos, m_app->m_mhandler, m_app->m_matWorld, 1 );
				if( !ret ){
					m_camera.target = bonepos;
				}
			}
		}
	}
	
	*dstcamera = m_camera;

	return 0;
}

int CCamKeyDlg::ToggleOrtho()
{
	if( m_ortho == 1 ){
		BOOL dummy;
		OnProjPers( 0, 0, 0, dummy );
	}else{
		BOOL dummy;
		OnProjOrtho( 0, 0, 0, dummy );
	}
	return 0;
}
