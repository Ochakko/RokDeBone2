// MotParamDlg.cpp : CMotParamDlg のインプリメンテーション
#include "stdafx.h"
#include "MotParamDlg.h"
#include "WriteBmpDlg.h"
#include "LoadBVHDlg.h"
#include "FrameShiftDlg.h"
#include "FramePasteDlg.h"
#include "OpTargetDlg.h"
#include "MotCutDlg.h"
#include "MabikiDlg.h"

#include <math.h>
#include <stdio.h>
#include <D3DX9.h>
#include "D3DFont.h"
//#include "dxutil.h"

#include <coef.h>
#define DBGH
#include <dbg.h>

#include <treehandler2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <mothandler.h>
#include <motionctrl.h>
#include <sigfile.h>
#include <motionpoint2.h>
#include <quaternion2.h>
#include <spparam.h>
#include <AutoSpline.h>

#include <quafile.h>
#include <bvhfile.h>

#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include <vfw.h>

#include <SymmOpe.h>
#include <BoneProp.h>

#include "FrameOpeDlg.h"
#include "MotSettingDlg.h"
//#include "DispSwitchDlg.h"
#include "FrameCopyDlg.h"
#include "PasteTypeDlg.h"
#include "LoadNameDlg.h"
#include "WriteQuaDlg.h"
#include "ImportQuaDlg.h"
#include "BonePropDlg.h"

#include "dibfile.h"
#include <MKMFile.h>
#include <EQUFile.h>

#include "viewer.h"
#include "OptDlg.h"

#include "MotDlg.h"
#include "GetDlgParams.h"

#include "LdQuaDlg.h"

#include "TexKeyDlg.h"
#include "AlpKeyDlg.h"
#include "DSKeyDlg.h"
#include "GraphRotDlg.h"
#include <MMotKey.h>

#include <DispSwitch.h>
#include <MQOMaterial.h>


#include "CamKeyDlg.h"
#include <CameraAnimHandler.h>
#include <CameraAnim.h>

#include "GPKeyDlg.h"
#include <GPAnimHandler.h>
#include <GPAnim.h>

#include "MOEKeyDlg.h"
#include <MOEAnimHandler.h>
#include <MOEAnim.h>

#include "SndKeyDlg.h"
#include <SndAnimHandler.h>
#include <SndAnim.h>

#include "KeyMenuMain.h"

#include <texkey.h>
#include <dskey.h>
#include <alpkey.h>

#include "MotChangeDlg.h"
#include <MCHandler.h>
#include "SyncDlg.h"

#include "SoundBank.h"

//MotionPoint描画時の点の半径
#define MP_SIZE	3

#define MIN_IKBONENUM	1
#define MAX_IKBONENUM	100

#define MIN_ROTRAD	1
#define MAX_ROTRAD	10

#define CPFRAMEMAX	1001

//m_rotkind
enum {
	ROTKIND_NEW,
	ROTKIND_OLD,
	ROTKIND_MAX
};

enum {
	OP_SOUTAI,
	OP_ZETTAI
};

//static char clipboardheader[256] = "RDB2FrameCopyToCB:ver0001:type00\r\n";
static char clipboardheader2[256] = "RDB2FrameCopyToCB:ver5522:type00\r\n";

/////////////////
// sttic
//static LONG s_treefunc = 0;
static CMotParamDlg* s_mpd = 0;
static CCamKeyDlg* s_ckd = 0;
static CGPKeyDlg* s_gpkd = 0;
static CMCHandler* s_mch = 0;
static CMOEKeyDlg* s_moekd = 0;
static CSndKeyDlg* s_sndkd = 0;
static int s_bgmstopcnt = 0;
static int s_bgmstartcnt = 0;
static int s_befframe = 0;

static LONG s_mpfunc = 0;

//static HWND s_tree_mot_wnd = 0;
static HWND s_sl_mp_wnd = 0;

static HWND s_apphwnd = 0;

static HANDLE s_hThread = NULL;

// thread 終了トリガー
// 0 が立つと　_endthreadex で　終了　(　handle は　close しない！！！　)
static LONG s_lThread = 1;

static DWORD s_dwMainId;

static HANDLE	s_hEvent; //手動リセットイベント
static DWORD s_mainthreadid;

static LONG s_frameno = 0;
static LONG s_startframe = 0;

static int s_previewfirst = 1;

/////////////////
//extern HINSTANCE g_resinstance;
extern CMotDlg* g_motdlg;

	//extern rokdebone2.cpp
extern CIniData* g_inidata;

extern CBonePropDlg*	g_bonepropdlg;

extern unsigned char g_KeyBuffer[256];
extern int g_keyboardcnt[256];
extern int g_validkey[256];
extern int g_endapp;

/////////////////////////////////////////////////////////////////////////////
// CMotParamDlg

CMotParamDlg::CMotParamDlg( int iklevelope )
{
	m_loadingflag = 0;
	m_thandler = 0;
	m_shandler = 0;
	m_mhandler = 0;

	m_apphwnd = 0;

	InitRotAndMv();
	
	m_hmp = 0;

	m_current = 0;

//	m_selecteditem = 0;
	m_selectedno = 0;

	m_sellockno = 0;
	//m_ik_bonenum = 5;

	m_sl_mp_rangemax = 59;

	m_black_pen = NULL;
	m_red_pen = NULL;
	m_blue_pen = NULL;
	m_gray_pen = NULL;
	m_black_brush = NULL;
	m_white_brush = NULL;
	m_red_brush = NULL;
	m_blue_brush = NULL;
	m_gray_brush = NULL;

	m_mot_cookie = -1;
	ZeroMemory( m_motionname, 256 );
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	sprintf_s( m_motionname, 256, "motion_%d_%d_%d_%d_%d_%d_%d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
	);
	m_motiontype = MOTION_CLAMP;
	m_motionjump = 0;
	//m_interpolation = INTERPOLATION_SQUAD;
	m_interpolation = INTERPOLATION_SLERP;

//	m_TI = 0;

	m_displayflag = 0;
	m_ismoving = 0;
	m_preview_flag = 0;

	m_invalidate_handler = 0;
	m_remaking_tree = 0;

	m_CEBuff = 0;

	int i;
	for( i = 0; i < MAXUNDONUM; i++ ){
		m_undobuf[i].frameno = -1;
		m_undobuf[i].seriarray = 0;
		m_undobuf[i].mparray = 0;
	}

	m_dragmp = 0;
	m_dragframe = 0;

	m_dragflag = 0;

//	m_hImageList = 0;
//	m_iImage = 0;
//	m_iSelect = 0;

	m_mvx = 0.0f;
	m_mvy = 0.0f;
	m_mvz = 0.0f;

	m_scalex = 1.0f;
	m_scaley = 1.0f;
	m_scalez = 1.0f;

	m_dispswitch = 0xFFFFFFFF;
	m_interp = 0;
	m_q.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
	m_timerworking = 0;

	m_rotrad = 5;

	m_cmdshow = SW_HIDE;

	m_d3dapp = 0;

	m_ikmode = IKMODE_OPE;
	m_iklevel = 5;
	//m_iklevelope = 5;
	m_iklevelope = iklevelope;


	m_underdestroyflag = 0;


	m_motionnum = 0;
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
		m_motid[motno].framerange = 0;
	}

	InitParBoneArray();

	m_defframerange = 100;
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );

	m_texkeydlg = 0;
	m_alpkeydlg = 0;
	m_dskeydlg = 0;
	m_grotdlg = 0;
	m_MorphKeyDlg = 0;
	m_camkeydlg = 0;
	m_gpkeydlg = 0;
	m_moekeydlg = 0;
	m_sndkeydlg = 0;

	graymovestart = -1;
	graymove = -1;

	m_rotkind = ROTKIND_NEW;

	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

	m_previewframe = 0;
	m_previewmotion = 0;
	m_previewcamera = 0;
	m_previewgp = 0;
	m_previewmoa = 0;
	m_previewmoe = 0;

	s_mpd = this;


	m_dsfillupmode = 0;
	m_texfillupmode = 0;

	m_savebmdisp = g_inidata->opt_bmdisp;
	m_savejdisp = g_inidata->opt_jointdisp;

	m_sndframe = 0;
	m_withsndflag = 0;

	m_calcsp = g_inidata->motdlg_calcsp;

	m_soutai = OP_SOUTAI;


	m_optardlg = new COpTargetDlg();
	if( !m_optardlg ){
		_ASSERT( 0 );
	}
	m_optardlg->Create( m_hWnd );

	m_notcreate = 0;
}

int CMotParamDlg::InitParBoneArray()
{
	int bno;
	for( bno = 0; bno <= MAXBONENUM; bno++ ){
		m_parbonearray[bno] = -1;
	}

	return 0;
}
int CMotParamDlg::SetParBoneArray( int mvno )
{
	InitParBoneArray();

	if( mvno <= 0 )
		return 0;


	int setno = 0;
	CShdElem* curselem;

	curselem = (*m_shandler)( mvno );

	while( curselem ){
		if( setno > MAXBONENUM ){
			DbgOut( "motparamdlg : SetParBoneArray : array leng too short error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		m_parbonearray[ setno ] = curselem->serialno;
		setno++;

		curselem = m_shandler->FindUpperJoint( curselem, 0 );

	}

	return 0;
}

int CMotParamDlg::ExistInParBoneArray( int chkno )
{
	int bno;
	int findflag = 0;

	for( bno = 0; bno <= MAXBONENUM; bno++ ){
		if( m_parbonearray[bno] == chkno ){
			findflag = 1;
			break;
		}
	}

	return findflag;
}


int CMotParamDlg::InitUndoBuffer()
{
	int shdnum;
	int bufno;

	shdnum = m_shandler->s2shd_leng;

	for( bufno = 0; bufno < MAXUNDONUM; bufno++ ){
		m_undobuf[bufno].seriarray = (int*)malloc( shdnum * sizeof( int ) );
		if( !m_undobuf[bufno].seriarray ){
			_ASSERT( 0 );
			return 1;
		}
		m_undobuf[bufno].mparray = new CMotionPoint2[ shdnum ];
		if( !m_undobuf[bufno].mparray ){
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CMotParamDlg::DestroyUndoBuffer()
{
	
	int bufno;
	for( bufno = 0; bufno < MAXUNDONUM; bufno++ ){
		m_undobuf[bufno].frameno = -1;
	
		if( m_undobuf[bufno].seriarray ){
			free( m_undobuf[bufno].seriarray );
			m_undobuf[bufno].seriarray = 0;
		}

		if( m_undobuf[bufno].mparray ){
			delete [] m_undobuf[bufno].mparray;
			m_undobuf[bufno].mparray = 0;
		}
	}
	if( m_apphwnd ){
		::SendMessage( m_apphwnd, WM_USER_INIT_UNDOBUF, 0, 1 );
	}
		

	return 0;
}

CMotParamDlg::~CMotParamDlg()
{

	m_underdestroyflag = 1;

	//int ret;
	//ret = ::KillTimer( m_hWnd, MOTPARAMDLGTIMER );
	//_ASSERT( ret );
	//m_timerworking = 0;


	if( m_hmp ){
		DestroyHMPReq( m_hmp );
		m_hmp = 0;
	}


	if( m_optardlg ){
		m_optardlg->ShowDlg( SW_HIDE, 0 );
		m_optardlg->DestroyWindow();
		delete m_optardlg;
		m_optardlg = 0;
	}


	if( m_CEBuff ){
		free( m_CEBuff );
		m_CEBuff = 0;
	}
	DestroyUndoBuffer();

	int motno;
	for( motno = 0; motno < MAXMOTIONNUM; motno++ ){
		m_motid[motno].id = -1;
		ZeroMemory( m_motid[motno].filename, sizeof( char ) * 256 );
		m_motid[motno].ev0idle = 0;
		m_motid[motno].commonid = 0;
		if( m_motid[motno].forbidid ){
			free( m_motid[motno].forbidid );//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			m_motid[motno].forbidid = 0;
		}
		m_motid[motno].forbidnum = 0;
		m_motid[motno].notfu = 0;
	}

	//::DestroyWindow( m_hWnd );//!!!!!

	//InterlockedExchange( &m_preview_flag, 0 );	
	//SetEvent( s_hEvent );//!!! msgwaitを解除。
	//InterlockedExchange( &s_lThread, 0 );

	if( s_hThread != NULL ){
		DWORD dwwait = 0;
		InterlockedExchange( &s_lThread, 0 );
		Sleep( 500 );
		InterlockedExchange( &m_preview_flag, 0 );	
		SetEvent( s_hEvent );//!!! msgwaitを解除。
		
		while( dwwait != WAIT_OBJECT_0 ){
			dwwait = WaitForSingleObject( s_hThread, 1500 );
			//INFINITEで待つと、実行スレッドが無くなってしまい、デッドロックする。
		}

		CloseHandle( s_hThread );	
		DbgOut( "motparamdlg : thread handle close\n" );
	}


	if( s_hEvent != NULL ){
		CloseHandle( s_hEvent );
	}

}

int CMotParamDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, MOTPARAMDLGTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	s_mpd = 0;
	s_ckd = 0;
	s_gpkd = 0;
	s_mch = 0;
	s_moekd = 0;
	s_sndkd = 0;


	m_texkeydlg = 0;
	m_alpkeydlg = 0;
	m_dskeydlg = 0;
	m_grotdlg = 0;
	m_MorphKeyDlg = 0;
	m_camkeydlg = 0;
	m_gpkeydlg = 0;
	m_moekeydlg = 0;
	m_sndkeydlg = 0;

	return 0;
}

LRESULT CMotParamDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	InitCommonControls();

	ret = SetWnd();
	_ASSERT( !ret );
//	s_tree_mot_wnd = m_tree_mot_wnd;
	s_sl_mp_wnd = m_sl_mp_wnd;

	ret = SetNewWindowProc();
	_ASSERT( !ret );

	//CreateImageList();

	ret = ParamsToDlg();
	_ASSERT( !ret );


	s_mainthreadid = GetCurrentThreadId();

		// ( , 手動リセット, ノンシグナル初期化, )
	s_hEvent = CreateEvent( NULL, TRUE, FALSE, NULL ); 

	s_hThread = BEGINTHREADEX(NULL, 0, ThreadFunc, 
		(void*)this,
		0, &s_dwMainId );
		//CREATE_SUSPENDED, &s_dwMainId );
	
	//ResumeThread( s_hThread );

	int timerid;
	timerid = (int)::SetTimer( m_hWnd, MOTPARAMDLGTIMER, 500, NULL );
	_ASSERT( timerid == MOTPARAMDLGTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}


int CMotParamDlg::InitDlg()
{
	int ret;
	
	DbgOut( "MotParamDlg : InitDlg\n" );

	ret = DestroyFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : InitDlg : DestroyFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ZeroMemory( m_motionname, 256 );
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	sprintf_s( m_motionname, 256, "motion_%d_%d_%d_%d_%d_%d_%d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
	);

	m_interpolation = INTERPOLATION_SLERP;//!!!!!!!!
	m_mot_cookie = m_mhandler->AddMotion( m_d3dapp->m_za_kind, m_motionname, m_motiontype, m_sl_mp_rangemax + 1, m_interpolation, m_motionjump );
	if( m_mot_cookie < 0 ){
		DbgOut( "MotParamDlg : InitDlg : mhandler->AddMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = CreateMotionPoints( -1, m_mot_cookie, 0 );
	_ASSERT( !ret );

	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;
	m_current = m_startframe;

	m_motionnum = m_mhandler->m_kindnum;
	if( m_motionnum > MAXMOTIONNUM ){
		DbgOut( "motparamdlg : InitDlg : motionnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_motid[ m_motionnum - 1 ].id = m_mot_cookie;
	m_motid[ m_motionnum - 1 ].filename[0] = 0;
	m_motid[ m_motionnum - 1 ].ev0idle = 0;
	m_motid[ m_motionnum - 1 ].commonid = 0;
	m_motid[ m_motionnum - 1 ].forbidnum = 0;
	if( m_motid[ m_motionnum - 1 ].forbidid ){
		free( m_motid[ m_motionnum - 1 ].forbidid );
	}
	m_motid[ m_motionnum - 1 ].forbidid = 0;
	m_motid[ m_motionnum - 1 ].notfu = 0;

	m_motid[ m_motionnum - 1 ].startframe = m_startframe;
	m_motid[ m_motionnum - 1 ].endframe = m_endframe;
	m_motid[ m_motionnum - 1 ].framerange = m_range;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;



	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = FillTree();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	InterlockedExchange( &m_previewframe, m_startframe );


	ret = CreateFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : InitDlg : CreateFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	::SendMessage( m_apphwnd, WM_USER_ANIMMENU, m_motionnum - 1, 0 );
	if( m_texkeydlg ){
		ret = m_texkeydlg->OnUserSelchange();
		_ASSERT( !ret );
	}
	if( m_alpkeydlg ){
		ret = m_alpkeydlg->OnUserSelchange();
		_ASSERT( !ret );
	}
	if( m_dskeydlg ){
		ret = m_dskeydlg->OnUserSelchange();
		_ASSERT( !ret );
	}
	if( m_MorphKeyDlg ){
		ret = m_MorphKeyDlg->ResetDlg();
		_ASSERT( !ret );
	}
	if( m_camkeydlg ){
		ret = m_camkeydlg->ResetParams( 1 );
		_ASSERT( !ret );
	}
	if( m_gpkeydlg ){
		ret = m_gpkeydlg->ResetParams( 1 );
		_ASSERT( !ret );
	}
	if( m_moekeydlg ){
		ret = m_moekeydlg->ResetParams( 1 );
		_ASSERT( !ret );
	}

	return 0;
}
int CMotParamDlg::RemakeTreeView( int selno )
{
	m_remaking_tree = 1;

	if( m_thandler || m_shandler || m_mhandler ){
		//既存のTreeを壊す。
		if( m_CEBuff ){
			free( m_CEBuff );
			m_CEBuff = 0;
		}
		DestroyUndoBuffer();
	}

	int ret = 0;
	ret += m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	ret += m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : RemakeTreeView : MakeRawmat error" );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : RemakeTreeView : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	SetCurrentMotion();

	ret = FillTree();
	if( ret ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : RemakeTreeView : FillTree error" );
		return 1;
	}

	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	ret = OnUserSelchange( selno );
	if( ret ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : RemakeTreeView : OnUserSelChange error" );
		return 1;
	}

	m_remaking_tree = 0;
	return 0;
}

LRESULT CMotParamDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "MotParamDlg : OnOK\n" );
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	m_displayflag = 0;
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}

LRESULT CMotParamDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	m_displayflag = 0;
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}
LRESULT CMotParamDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	m_displayflag = 0;
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}


	
int CMotParamDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_tex_current_wnd = GetDlgItem( IDC_TEX_CURRENT );
	m_mp_wnd = GetDlgItem( IDC_MP );
	m_sl_mp_wnd = GetDlgItem( IDC_SL_MP );
	m_sp_mp_wnd = GetDlgItem( IDC_SP_MP );
	m_paramset_wnd = GetDlgItem( IDC_PARAMSET );
	m_framecopy_wnd = GetDlgItem( IDC_FRAMECOPY );
	m_framepaste_wnd = GetDlgItem( IDC_FRAMEPASTE );
	m_framedel_wnd = GetDlgItem( IDC_FRAMEDEL );
	m_preview_wnd = GetDlgItem( IDC_PREVIEW );
	m_stop_wnd = GetDlgItem( IDC_STOP );
	m_alldelete_wnd = GetDlgItem( IDC_ALLDELETE );
	m_sp_twist_wnd = GetDlgItem( IDC_SP_TWIST );
	m_reset_rot_wnd = GetDlgItem( IDC_RESET_ROT );
	m_reset_mv_wnd = GetDlgItem( IDC_RESET_MV );
	m_bonedisp_wnd = GetDlgItem( IDC_BONEDISP );
	m_sp_rotx_wnd = GetDlgItem( IDC_SP_ROTX );
	m_sp_roty_wnd = GetDlgItem( IDC_SP_ROTY );
	m_sp_rotz_wnd = GetDlgItem( IDC_SP_ROTZ );
	m_sp_rotrad_wnd = GetDlgItem( IDC_SP_ROTRAD );
	m_tex_rotrad_wnd = GetDlgItem( IDC_TEX_RAD );

	m_combo_iklevel_wnd = GetDlgItem( IDC_COMBO_LEVEL );

	m_radio_modejoint_wnd = GetDlgItem( IDC_RADIO_MODEJOINT );
	m_radio_modeope_wnd = GetDlgItem( IDC_RADIO_MODEOPE );

	m_newrot_wnd = GetDlgItem( IDC_NEWROT );
	m_oldrot_wnd = GetDlgItem( IDC_OLDROT );

	m_soutai_wnd = GetDlgItem( IDC_SOUTAI );
	m_zettai_wnd = GetDlgItem( IDC_ZETTAI );

	m_range_wnd = GetDlgItem( IDC_RANGE );
	m_sl_range_wnd = GetDlgItem( IDC_SL_RANGE );
	m_rangestart_wnd = GetDlgItem( IDC_RANGESTART );
	m_editrange_wnd = GetDlgItem( IDC_EDITRANGE );


	//sliderの初期設定
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//


	//spin
	//m_sp_bonenum_wnd.SendMessage( UDM_SETRANGE, 0, (LPARAM)MAKELONG( MAX_IKBONENUM, MIN_IKBONENUM ) );
	//m_sp_bonenum_wnd.SendMessage( UDM_SETPOS, 0, (LPARAM)MAKELONG( m_ik_bonenum, 0 ) );


	m_sp_rotrad_wnd.SendMessage( UDM_SETRANGE, 0, (LPARAM)MAKELONG( MAX_ROTRAD, MIN_ROTRAD ) );
	m_sp_rotrad_wnd.SendMessage( UDM_SETPOS, 0, (LPARAM)MAKELONG( m_rotrad, 0 ) );


	//combo
	int levno;
	for( levno = 1; levno <= 15; levno++ ){
		char strlev[5];
		sprintf_s( strlev, 5, "%d", levno );
		m_combo_iklevel_wnd.SendMessageA( CB_ADDSTRING, 0, (LPARAM)strlev );
	}
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"255" );


	if( (m_iklevelope >= 1) && (m_iklevelope <= 15) ){
		m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, (m_iklevelope - 1), 0 );
	}else{
		m_iklevelope = 255;
		m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, 15, 0 );
	}
	

	//radio
	m_dlg_wnd.CheckRadioButton( IDC_RADIO_MODEJOINT, IDC_RADIO_MODEOPE, IDC_RADIO_MODEOPE );

	return 0;
}

int CMotParamDlg::ParamsToDlg()
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
//m_current
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "frameno: current %d, start %d, end %d, total %d", m_current, m_startframe, m_endframe, m_sl_mp_rangemax + 1 );
	if( (g_endapp == 1) || !m_tex_current_wnd.IsWindow() )
		return 0;
	m_tex_current_wnd.SetWindowText( tempchar );

//m_sl_mp_
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	if( (g_endapp == 1) || !m_sl_mp_wnd.IsWindow() )
		return 0;
	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//


	if( (g_endapp == 1) || !m_sl_range_wnd.IsWindow() )
		return 0;
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	if( (g_endapp == 1) || !m_sl_range_wnd.IsWindow() )
		return 0;
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	if( (g_endapp == 1) || !m_sl_range_wnd.IsWindow() )
		return 0;
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	if( (g_endapp == 1) || !m_sl_range_wnd.IsWindow() )
		return 0;
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );
	if( (g_endapp == 1) || !m_sl_range_wnd.IsWindow() )
		return 0;


//m_tex_bonenum
	//sprintf( tempchar, "%d 階層", m_ik_bonenum );
	//m_tex_bonenum_wnd.SetWindowText( tempchar );

//m_tex_rotrad
	sprintf_s( tempchar, 1024, "%d度", m_rotrad );
	if( (g_endapp == 1) ||!m_tex_rotrad_wnd.IsWindow() )
		return 0;
	m_tex_rotrad_wnd.SetWindowText( tempchar );


	sprintf_s( tempchar, 1024, "%d", m_startframe );
	m_rangestart_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%d", m_range );
	m_editrange_wnd.SetWindowText( tempchar );


	if( m_notcreate == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_NCRE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_NCRE, BST_UNCHECKED );
	}

	if( m_texkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_texkeydlg->ParamsToDlg();
	}
	if( m_alpkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_alpkeydlg->ParamsToDlg();
	}
	if( m_dskeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_dskeydlg->ParamsToDlg();
	}
	if( m_MorphKeyDlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_MorphKeyDlg->ParamsToDlg();
	}
	if( m_grotdlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_grotdlg->Redraw();
	}

	CheckRotKindButton();
	CheckSoutaiButton();

	return 0;
}

LRESULT CMotParamDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;

	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}


LRESULT CMotParamDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	if( ::IsWindow( (HWND)m_sl_mp_wnd ) == 0 )
		return 0;

	if( !IsWindow() )
		return 0;

	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	int curpos;
	//int ret;
	curpos = (int)m_sl_mp_wnd.SendMessage( TBM_GETPOS, 0, 0 );

	m_current = curpos;

	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	SetCurrentMotion();

	Redraw();

	if( m_texkeydlg ){
		m_texkeydlg->ParamsToDlg();
	}
	if( m_alpkeydlg ){
		m_alpkeydlg->ParamsToDlg();
	}
	if( m_dskeydlg ){
		m_dskeydlg->ParamsToDlg();
	}
	if( m_MorphKeyDlg ){
		m_MorphKeyDlg->ParamsToDlg();
	}
	if( m_grotdlg ){
		m_grotdlg->Redraw();
	}

	if( (idCtrl != 999) && (m_d3dapp->m_syncdlg->m_quaflag != 0) ){
		int camflag = m_d3dapp->m_syncdlg->m_camflag;
		int gpaflag = m_d3dapp->m_syncdlg->m_gpaflag;
		int moeflag = m_d3dapp->m_syncdlg->m_moeflag;
		int sndflag = m_d3dapp->m_syncdlg->m_sndflag;

		g_motdlg->OnJumpFrame( m_current, 0, camflag, gpaflag, moeflag, sndflag );
	}

	return 0;
}

LRESULT CMotParamDlg::OnReleasedcaptureSl_range(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	if( ::IsWindow( (HWND)m_sl_range_wnd ) == 0 )
		return 0;

	if( !IsWindow() )
		return 0;

	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	int curpos;
	//int ret;
	curpos = (int)m_sl_range_wnd.SendMessage( TBM_GETPOS, 0, 0 );

	int tmpend;
	m_startframe = curpos;
	tmpend = curpos + m_range;
	m_endframe = min( tmpend, m_sl_mp_rangemax );
	m_motid[m_mot_cookie].startframe = m_startframe;
	m_motid[m_mot_cookie].endframe = m_endframe;

	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

	BOOL dummy = 0;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

//_ASSERT( 0 );

/***
	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

//_ASSERT( 0 );

	SetCurrentMotion();
	Redraw();
***/
//_ASSERT( 0 );

	return 0;
}


int CMotParamDlg::GetEul( int boneno, D3DXVECTOR3* dsteul )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	CMotionCtrl* mcptr = (*m_mhandler)( boneno );//注意！！引数は、m_selectednoではい。！！
	_ASSERT( mcptr );
		

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		*dsteul = curmp->m_eul;
	}else{

		ret = CreateMotionPoints( mcptr->serialno, m_mot_cookie, m_current );
		if( ret )
			return -1;
		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
		if( !curmp )
			return -1;

		*dsteul = curmp->m_eul;
	}



	return 0;
}
int CMotParamDlg::SetEul( int boneno, D3DXVECTOR3 srceul )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	//int ret;
	CMotionCtrl* mcptr = (*m_mhandler)( boneno );//注意！！引数は、m_selectednoではい。！！
	_ASSERT( mcptr );
		

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		curmp->m_eul = srceul;
	}else{
		_ASSERT( 0 );
	}

	//UpdateMatrix( parboneno );
	//UpdateAllMatrix();
	//SetCurrentMotion();
	//UpdateMatrixSelected( boneno, 1 );

	return 0;
}


int CMotParamDlg::GetQ( int boneno, CQuaternion* dstq )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

	CMotionCtrl* mcptr = (*m_mhandler)( boneno );//注意！！引数は、m_selectednoではい。！！
	_ASSERT( mcptr );
		

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		*dstq = curmp->m_q;
	}else{

		ret = CreateMotionPoints( mcptr->serialno, m_mot_cookie, m_current );
		if( ret )
			return -1;
		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
		if( !curmp )
			return -1;

		*dstq = curmp->m_q;

	}
	
	return 0;
}

int CMotParamDlg::SetQ( int boneno, CQuaternion srcq )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	CMotionCtrl* mcptr = (*m_mhandler)( boneno );//注意！！引数は、m_selectednoではい。！！
	_ASSERT( mcptr );
		

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		m_q = srcq;
		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;

		m_userint1 = curmp->m_userint1;

		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );
	}else{
		m_q = srcq;
		m_mvx = 0.0f;
		m_mvy = 0.0f;
		m_mvz = 0.0f;
		//mcptr->GetDispSwitch( &m_dispswitch, m_mot_cookie, m_current );
		mcptr->GetInterpolation( &m_interp, m_mot_cookie, m_current );

		m_scalex = 1.0f;
		m_scaley = 1.0f;
		m_scalez = 1.0f;

		m_userint1 = 0;

		CMotionPoint2* newmp = mcptr->AddMotionPoint( m_mot_cookie, m_current,
			m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		if( !newmp ){
			_ASSERT( 0 );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : OnReleasedcaptureSl_rotx : AddMotionPoint error" );
			return -1;
		}
	}

	//UpdateMatrix( parboneno );
	//UpdateAllMatrix();
	//SetCurrentMotion();
	UpdateMatrixSelected( boneno, 1, 0 );

	return 0;
}

int CMotParamDlg::SetMotionPointIK( int frameno, D3DXMATRIX matWorld, int mvboneno, CQuaternion multq, BOOL needredraw, int* forcemove, CMotionPoint2* calcmp )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

//	CMotionCtrl* mcptr = (*m_mhandler)( mvboneno );//注意！！引数は、m_selectednoではない。！！
//	_ASSERT( mcptr );
		
	CShdElem* curselem;
	curselem = (*m_shandler)( mvboneno );
	_ASSERT( curselem );
	CShdElem* parselem;
	parselem = m_shandler->FindUpperJoint( curselem );

	CShdElem* savepar = parselem;

	CMotionCtrl* mcptr;
	if( m_mhandler->m_standard == 1 ){
		if( savepar ){
			mcptr = (*m_mhandler)( savepar->serialno );
		}else{
			mcptr = 0;
		}
	}else{
		mcptr = (*m_mhandler)( mvboneno );
	}


// worldmat
	CQuaternion worldq;
	D3DXMATRIX worldm;
	worldm = matWorld;
	worldm._41 = 0.0f;
	worldm._42 = 0.0f;
	worldm._43 = 0.0f;
	D3DXQUATERNION wqx;
	D3DXQuaternionRotationMatrix( &wqx, &worldm );
	worldq.x = wqx.x;
	worldq.y = wqx.y;
	worldq.z = wqx.z;
	worldq.w = wqx.w;

	CQuaternion parq( 1.0f, 0.0f, 0.0f, 0.0f );
	CQuaternion curq( 1.0f, 0.0f, 0.0f, 0.0f );
	CQuaternion invcurq( 1.0f, 0.0f, 0.0f, 0.0f );
	CQuaternion multq2;

//	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
	CMotionPoint2* curmp;
	
	if( calcmp == 0 ){
		if( mcptr ){
			curmp = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
		}else{
			curmp = 0;
		}
	}else{
		curmp = calcmp;
	}

	if( curmp ){
		
		//m_q = invparq * multq;
		//m_q = multq * curmp->m_q;//後に掛ける
		//m_q = curmp->m_q * multq;//最初に掛ける		


		if( m_mhandler->m_standard == 1 ){
			if( parselem ){
				parselem = m_shandler->FindUpperJoint( parselem );
			}
		}

		while( parselem ){
			CMotionCtrl* parmc;
			parmc = (*m_mhandler)( parselem->serialno );
			
			//parmp = parmc->IsExistMotionPoint( m_mot_cookie, frameno );
			//_ASSERT( parmp );
			CMotionPoint2 parmp;
			int hasmpflag = 0;
			ret = parmc->CalcMotionPointOnFrame( parselem, &parmp, m_mot_cookie, frameno, &hasmpflag, m_shandler, m_mhandler );
			if( ret ){
				DbgOut( "motparamdlg : SetMotionPointIK : mc CalcMotionPointOnFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			parq = parmp.m_q * parq;
			//parq = parq * parmp->m_q;//!!!!!!
			parq.normalize();
			parselem = m_shandler->FindUpperJoint( parselem );
		}

		curq = worldq * parq * curmp->m_q;

		curq.normalize();
		curq.inv( &invcurq );
		invcurq.normalize();

//		CQuaternion aftq;
//		aftq = curmp->m_q * invcurq * multq * curq;
//		aftq = invcurq * multq * curq * curmp->m_q;
//		aftq = curq * multq * invcurq * curmp->m_q;
//		aftq = curmp->m_q * curq * multq * invcurq;

		D3DXQUATERNION xmultq;
		multq.CQuaternion2X( &xmultq );

		D3DXVECTOR3 axis, multaxis;
		float rad;
		D3DXQuaternionToAxisAngle( &xmultq, &axis, &rad );
		DXVec3Normalize( &axis, &axis );
		invcurq.Rotate( &multaxis, axis );
		DXVec3Normalize( &multaxis, &multaxis );

		multq2.SetAxisAndRot( multaxis, rad );

		CQuaternion aftq;
		aftq = curmp->m_q * multq2;



		int startout = 0;
		D3DXVECTOR3 neweul;
		ret = g_bonepropdlg->GetLimitedQ( mcptr->serialno, &curmp->m_q, &curmp->m_eul, &aftq, &m_q, &startout, &neweul, 0, 0 );
		if( ret ){
			DbgOut( "motparamdlg : SetMotionPointIK : GetLimitedQ error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		*forcemove = startout;//!!!!!!!

		//m_q = curmp->m_q * multq2;


		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		ret = curmp->SetParams( frameno, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );
		curmp->m_eul = neweul;//!!!!!!!!!!!!!!!!!
	}else{

		if( !mcptr ){
			return 0;//!!!!!!!!!!!!!!!!!!!!!!
		}

		ret = CreateMotionPoints( mcptr->serialno, m_mot_cookie, frameno );
		if( ret )
			return -1;

		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
		if( !curmp )
			return -1;

		//m_q = invparq * multq;
		//m_q = multq * curmp->m_q;//後に掛ける
		//m_q = curmp->m_q * multq;//最初に掛ける
		if( m_mhandler->m_standard == 1 ){
			if( parselem ){
				parselem = m_shandler->FindUpperJoint( parselem );
			}
		}

		while( parselem ){
			CMotionCtrl* parmc;
			parmc = (*m_mhandler)( parselem->serialno );
			
			CMotionPoint2 parmp;
			int hasmpflag = 0;
			ret = parmc->CalcMotionPointOnFrame( parselem, &parmp, m_mot_cookie, frameno, &hasmpflag, m_shandler, m_mhandler );
			if( ret ){
				DbgOut( "motparamdlg : SetMotionPointIK : mc CalcMotionPointOnFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			parq = parmp.m_q * parq;
			//parq = parq * parmp->m_q;//!!!!!!!!!
			parq.normalize();
			parselem = m_shandler->FindUpperJoint( parselem );
		}
		curq = worldq * parq * curmp->m_q;

		curq.normalize();
		curq.inv( &invcurq );
		invcurq.normalize();

//		CQuaternion aftq;
//		aftq = curmp->m_q * invcurq * multq * curq;
//		aftq = invcurq * multq * curq * curmp->m_q;
//		aftq = curq * multq * invcurq * curmp->m_q;
//		aftq = curmp->m_q * curq * multq * invcurq;


		D3DXQUATERNION xmultq;
		multq.CQuaternion2X( &xmultq );

		D3DXVECTOR3 axis, multaxis;
		float rad;
		D3DXQuaternionToAxisAngle( &xmultq, &axis, &rad );
		DXVec3Normalize( &axis, &axis );
		invcurq.Rotate( &multaxis, axis );
		DXVec3Normalize( &multaxis, &multaxis );

		multq2.SetAxisAndRot( multaxis, rad );


		CQuaternion aftq;
		aftq = curmp->m_q * multq2;


		int startout = 0;
		D3DXVECTOR3 neweul;
		ret = g_bonepropdlg->GetLimitedQ( mcptr->serialno, &curmp->m_q, &curmp->m_eul, &aftq, &m_q, &startout, &neweul, 0, 0 );
		if( ret ){
			DbgOut( "motparamdlg : SetMotionPointIK : GetLimitedQ error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		*forcemove = startout;//!!!!!!!

//		m_q = curmp->m_q * multq2;


		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		/***
		m_mvx = 0.0f;
		m_mvy = 0.0f;
		m_mvz = 0.0f;
		***/
		ret = curmp->SetParams( frameno, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );

		curmp->m_eul = neweul;//!!!!!!!!!!!
	}


	//DbgOut( "MotParamDlg : SetMotionPointIK finishflag %d : m_q %f %f %f %f\n",
	//	finishflag,
	//	m_q.w, m_q.x, m_q.y, m_q.z );

	//UpdateAllMatrix();
//	UpdateMatrixSelected( mvboneno, 1 );
	
//	if( mcptr ){
//		UpdateMatrixSelected( mcptr->serialno, 1 );
//	}

	if( needredraw )
		Redraw();

	return 0;
}

int CMotParamDlg::SetMotionPointIK( int frameno, int mvboneno, float srcmvx, float srcmvy, float srcmvz, CMotionPoint2* calcmp )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

	CMotionCtrl* mcptr = (*m_mhandler)( mvboneno );//注意！！
	_ASSERT( mcptr );

	CMotionPoint2* curmp;
	if( calcmp == 0 ){
		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
	}else{
		curmp = calcmp;
	}

	if( curmp ){
		m_q = curmp->m_q;

//DbgOut( "motparamdlg : SetMotionPointIK : befmv %f %f %f\r\n", curmp->m_mvx, curmp->m_mvy, curmp->m_mvz ); 

		m_mvx = curmp->m_mvx + srcmvx;
		m_mvy = curmp->m_mvy + srcmvy;
		m_mvz = curmp->m_mvz + srcmvz;

		//m_mvx = srcmvx;
		//m_mvy = srcmvy;
		//m_mvz = srcmvz;

		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		ret = curmp->SetParams( frameno, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );
	}else{

		ret = CreateMotionPoints( mcptr->serialno, m_mot_cookie, frameno );
		if( ret )
			return -1;

		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
		if( !curmp )
			return -1;

//DbgOut( "motparamdlg : SetMotionPointIK : befmv %f %f %f\r\n", curmp->m_mvx, curmp->m_mvy, curmp->m_mvz ); 


		m_q.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
		//m_mvx = 0.0f;
		//m_mvy = 0.0f;
		//m_mvz = 0.0f;
		m_q = curmp->m_q;
		m_mvx = curmp->m_mvx + srcmvx;
		m_mvy = curmp->m_mvy + srcmvy;
		m_mvz = curmp->m_mvz + srcmvz;

		//m_mvx = srcmvx;
		//m_mvy = srcmvy;
		//m_mvz = srcmvz;


		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		ret = curmp->SetParams( frameno, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );

		
		//DbgOut( "MotParamDlg : SetMotionPointIK : AddMotionPoint %d\n", frameno );
	}


	//DbgOut( "MotParamDlg : SetMotionPointIK finishflag %d : m_q %f %f %f %f\n",
	//	finishflag,
	//	m_q.w, m_q.x, m_q.y, m_q.z );

	//UpdateAllMatrix();


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//	UpdateMatrixSelected( mvboneno, 1 );
//	Redraw();

	return 0;
}


LRESULT CMotParamDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( m_preview_flag == 1 )
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
		m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}else if( delta == -10000 ){
		//先頭フレーム
		m_current = m_startframe;
		m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}else if( delta > 0 ){
		int newframe;
		newframe = m_current + 1;
		if( newframe <= m_endframe ){
			m_current = newframe;							
			m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
			BOOL dummy;
			OnReleasedcaptureSl_mp( 0, 0, dummy );
		}
	}else if( delta < 0 ){
		int newframe;
		newframe = m_current - 1;
		if( newframe >= m_startframe ){
			m_current = newframe;
			m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
			BOOL dummy;
			OnReleasedcaptureSl_mp( 0, 0, dummy );
		}
	}

	return 0;

}

LRESULT CMotParamDlg::OnCusCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD ctrlid;
	WORD notid;

	ctrlid = LOWORD( wParam );
	notid = HIWORD( wParam );

	if( (ctrlid == IDC_COMBO_LEVEL) && (notid == CBN_SELCHANGE) ){
		if( m_preview_flag == 1 )
			return 0;

		int cursel;
		cursel = (int)m_combo_iklevel_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
		if( cursel == CB_ERR ){
			_ASSERT( 0 );	
			return 0;
		}

		int tempint;
		tempint = cursel + 1;

		if( m_ikmode == IKMODE_JOINT ){

			int setno;
			if( m_sellockno != 0 )
				setno = m_sellockno;
			else
				setno = m_selectedno;

			if( setno == 0 )
				return 0;

			CShdElem* selelem;
			selelem = (*m_shandler)( setno );
			if( !selelem )
				return 0;

			if( (tempint >= 1) && (tempint <= 15) ){
				m_iklevel = tempint;
				selelem->m_iklevel = m_iklevel;
			}else{
				selelem->m_iklevel = 255;
			}
		}else{		
			if( (tempint >= 1) && (tempint <= 15) ){
				m_iklevelope = tempint;
			}else{
				m_iklevelope = 255;
			}
		}

//DbgOut( "motparamdlg : Combo selchange: ikmode %d, iklevel %d, ope %d\r\n", m_ikmode, m_iklevel, m_iklevelope );

	}

	return 0;
}
/***
LRESULT CMotParamDlg::OnSpin_Sp_bonenum(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( m_preview_flag == 1 )
		return 0;
	
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
	char tempchar[256];

	int delta = pnmud->iDelta;

	if( delta > 0 ){
		int newnum;
		newnum = m_ik_bonenum + 1;
		if( newnum <= MAX_IKBONENUM ){
			m_ik_bonenum = newnum;
			sprintf( tempchar, "%d 階層", m_ik_bonenum );
			m_tex_bonenum_wnd.SetWindowText( tempchar );
		}
	}else if( delta < 0 ){
		int newnum;
		newnum = m_ik_bonenum - 1;
		if( newnum >= MIN_IKBONENUM ){
			m_ik_bonenum = newnum;
			sprintf( tempchar, "%d 階層", m_ik_bonenum );
			m_tex_bonenum_wnd.SetWindowText( tempchar );
		}
	}

	return 0;
}
***/

int CMotParamDlg::OnTwist( D3DXMATRIX matWorld, int twistno, float twistrad, int za4flag )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	int parno;
	CShdElem* twistelem;
	CShdElem* parelem;


	twistelem = (*m_shandler)( twistno );
	if( !twistelem )
		return 0;

	parelem = m_shandler->FindUpperJoint( twistelem );
	if( !parelem )
		return 0;
	parno = parelem->serialno;

	D3DXVECTOR3 direction;
	float twistx, twisty, twistz;
	float parx, pary, parz;


	//bonematがかかる前の座標を元に計算し、ｑは、左から掛ける

	ret = twistelem->GetBoneOrgPos( &twistx, &twisty, &twistz );
	if( ret )
		return 0;
	ret = parelem->GetBoneOrgPos( &parx, &pary, &parz );
	if( ret )
		return 0;
	direction.x = twistx - parx;
	direction.y = twisty - pary;
	direction.z = twistz - parz;
	DXVec3Normalize( &direction, &direction );



	//CMotionCtrl* mcptr = (*m_mhandler)( parno );
CMotionCtrl* mcptr = (*m_mhandler)( twistno );
	_ASSERT( mcptr );
	
	CQuaternion multq;
	ret = multq.SetAxisAndRot( direction, twistrad );
	_ASSERT( !ret );

	int docre, haskey;
	ret = m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		//m_q = curmp->m_q;
		//m_q = m_q * multq;//一番最初に、multqが作用するように。
		CQuaternion aftq;
		aftq = curmp->m_q * multq;
		int notmvflag = 1;
		int startout = 0;
		D3DXVECTOR3 neweul;
		ret = g_bonepropdlg->GetLimitedQ( twistno, &curmp->m_q, &curmp->m_eul, &aftq, &m_q, &startout, &neweul, notmvflag, 0 );
		if( ret ){
			DbgOut( "motparamdlg : OnTwistQ : GetLimitedQ error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );
		curmp->m_eul = neweul;//!!!!!!!!!!!!!!!!!!!!!

		ret = SetQOpTarget( mcptr->serialno, multq, m_q, 1 );
		_ASSERT( !ret );

	}else if( docre == 1 ){

		ret = CreateMotionPoints( twistno, m_mot_cookie, m_current );
		if( ret )
			return -1;

		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
		if( !curmp )
			return -1;

		/***
		m_q = multq;
		m_mvx = 0.0f;
		m_mvy = 0.0f;
		m_mvz = 0.0f;
		m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = 1.0f;
		m_scaley = 1.0f;
		m_scalez = 1.0f;
		m_userint1 = 0;
		***/

		//m_q = curmp->m_q * multq;
		CQuaternion aftq;
		aftq = curmp->m_q * multq;
		int notmvflag = 1;
		int startout = 0;
		D3DXVECTOR3 neweul;
		ret = g_bonepropdlg->GetLimitedQ( twistno, &curmp->m_q, &curmp->m_eul, &aftq, &m_q, &startout, &neweul, notmvflag, 0 );
		if( ret ){
			DbgOut( "motparamdlg : OnTwistQ : GetLimitedQ error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );
		curmp->m_eul = neweul;//!!!!!!!!!!!!!!!!!!!!!!!

		ret = SetQOpTarget( mcptr->serialno, multq, m_q, 1 );
		_ASSERT( !ret );

	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}

	//UpdateAllMatrix();
	//UpdateMatrixSelected( parno, 0 );
	UpdateMatrixSelected( twistno, 0, 0 );


	if( twistno > 0 ){
		SetParBoneArray( twistno );

		if( za4flag == 0 ){
			D3DXVECTOR3 axisdone( 1.0f, 0.0f, 0.0f );
			Rotate2IKTargetReq( matWorld, twistno, axisdone, 0 );
		}else{
			//ret = m_shandler->CalcZa4LocalQ( m_mhandler, m_mot_cookie, m_current );
			//_ASSERT( !ret );
			Rotate2IKTargetZa4Req( matWorld, twistno, 0 );
		}
	}



	Redraw();


	return 0;
}


LRESULT CMotParamDlg::OnSpin_Sp_twist(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int twistno;
	float twistrad;

	if( m_sellockno != 0 )
		twistno = m_sellockno;
	else
		twistno = m_selectedno;

	if( twistno == 0 )
		return 0;

	//twistrad = 2.0f * (float)DEG2PAI;
	twistrad = (float)m_rotrad * (float)DEG2PAI;
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
	int delta = pnmud->iDelta;
	if( delta < 0 )
		twistrad *= -1.0f;


	int ret;
	if( m_d3dapp ){
		ret = OnTwist( m_d3dapp->m_matWorld, twistno, twistrad, m_d3dapp->m_tbb_za4 );
		_ASSERT( !ret );
	}

	return 0;
}


void CMotParamDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	if( g_endapp == 1 ){
		return;
	}

	if( dstwnd.IsWindow() ){
		dstwnd.SendMessage( TBM_CLEARTICS, 0, 0 );
	}
	// !!! minticとmaxticには、自動的にticが入るので設定はしない。(してはいけない)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		if( g_endapp == 1 ){
			return;
		}
		if( dstwnd.IsWindow() ){
			dstwnd.SendMessage( TBM_SETTIC, 1, (LPARAM)(LONG)curtic );
		}
		curtic += ticfreq;
	}
}


int CMotParamDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	float step;
	if( m_startframe != m_endframe ){
		step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
		*posptr = MP_SIZE + (int)(step * (float)( frameno - m_startframe ) ) + clirect.left;
	}else{
		*posptr = MP_SIZE + clirect.left;
	}
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	return 0;
}

int CMotParamDlg::Frameno2WindowposRange( RECT clirect, int frameno, int* posptr )
{
	float step;
	if( m_endframe != 0 ){
		step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_sl_mp_rangemax - 0 );
		*posptr = MP_SIZE + (int)(step * (float)( frameno - 0 ) ) + clirect.left;
	}else{
		*posptr = MP_SIZE + clirect.left;
	}
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	return 0;
}



int CMotParamDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
{
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
	float tempfloat;
	tempfloat = (float)(posx - MP_SIZE - clirect.left) / step; 
	//四捨五入してframenoにする。
	int tempint;
	/***
	float shousuu;
	tempint = (int)tempfloat;
	shousuu = tempfloat - (float)tempint;
	if( shousuu >= 0.5f )
		tempint += 1;
	***/
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

int CMotParamDlg::DrawMotionPoints()
{
	if( !IsWindow() )
		return 0;
	if( !m_mp_wnd.IsWindow() )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( m_loadingflag == 1 ){
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
	if( m_gray_brush == NULL ){
		COLORREF graycol = RGB( 190, 190, 190 );
		m_gray_brush = ::CreateSolidBrush( graycol );
		_ASSERT( m_gray_brush != NULL );
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
	if( m_gray_pen == NULL ){
		COLORREF graycol = RGB( 190, 190, 190 );
		m_gray_pen = ::CreatePen( PS_SOLID, 0, graycol );
		_ASSERT( m_gray_pen != NULL );
	}


//背景塗りつぶし
	RECT clirect;
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
	Frameno2Windowpos( clirect, m_current, &barposx );

	/***
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)m_sl_mp_rangemax;
	int barposx = MP_SIZE + (int)(step * (float)m_current) + clirect.left;
	if( barposx < clirect.left )
		barposx = clirect.left;
	if( barposx > clirect.right )
		barposx = clirect.right;
	***/

	::MoveToEx( hdc, barposx, clirect.top, NULL );
	::LineTo( hdc, barposx, clirect.bottom );

//点
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	/***
	CMotionCtrl* mcptr = (*m_mhandler)(m_selectedno);
	_ASSERT( mcptr );	
	CMotionPoint2* mpptr = mcptr->GetMotionPoint( m_mot_cookie );

	CMotionPoint2* curmp = mpptr;
	while( curmp ){
		int frameno = curmp->m_frameno;
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
			int posy = (clirect.bottom - clirect.top) / 2;
			::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );
		}

		curmp = curmp->next;
		//DbgOut( "MotParamDlg : DrawMotionPoint : frame %d\n", frameno );
	}
	***/

	KEYFRAMENO* key;
	key = (KEYFRAMENO*)malloc( sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );
	if( !key ){
		DbgOut( "motparamdlg : DrawMotionPoint : key alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( key, sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );

	int ret;
	int keynum = 0;
	ret = m_mhandler->GetKeyframeNo( m_mot_cookie, m_selectedno, key, m_sl_mp_rangemax + 2, &keynum );
	if( ret ){
		DbgOut( "motparamdlg : DrawMotionPoint : mh GetKeyframeNo error !!!\n" );
		_ASSERT( 0 );
		free( key );
		return 1;
	}

	int keyno;
	for( keyno = 0; keyno < keynum; keyno++ ){
		int frameno, kind;
		frameno = (key + keyno)->frameno;
		kind = (key + keyno)->kind;

		if( (frameno >= m_startframe) && (frameno <= m_endframe) ){
			if( frameno != m_current ){
				if( kind == 2 ){
					SelectObject( hdc, m_black_pen );
					SelectObject( hdc, m_black_brush );
				}else if( kind == 1 ){
					SelectObject( hdc, m_gray_pen );
					SelectObject( hdc, m_gray_brush );					
				}
			}else{
				if( kind == 2 ){
					SelectObject( hdc, m_red_pen );
					SelectObject( hdc, m_red_brush );
				}else if( kind == 1 ){
					SelectObject( hdc, m_gray_pen );
					SelectObject( hdc, m_gray_brush );					
				}
			}
			//int posx = MP_SIZE + (int)(step * (float)(frameno));
			int posx;
			Frameno2Windowpos( clirect, frameno, &posx );
			int posy = (clirect.bottom - clirect.top) / 2;
			::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );
		}
	}
	free( key );
	


	if( m_dragmp ){
		int frameno = m_dragmp->m_frameno;
		
		SelectObject( hdc, m_blue_pen );
		SelectObject( hdc, m_blue_brush );
		
		//int posx = MP_SIZE + (int)(step * (float)(frameno));
		int posx;
		Frameno2Windowpos( clirect, frameno, &posx );
		int posy = (clirect.bottom - clirect.top) / 2;
		::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );		
	}else if( graymove >= 0 ){
		SelectObject( hdc, m_blue_pen );
		SelectObject( hdc, m_blue_brush );
		
		//int posx = MP_SIZE + (int)(step * (float)(frameno));
		int posx;
		Frameno2Windowpos( clirect, graymove, &posx );
		int posy = (clirect.bottom - clirect.top) / 2;
		::Ellipse( hdc, posx - MP_SIZE, posy - MP_SIZE, posx + MP_SIZE, posy + MP_SIZE );		
	}
	

	::SelectObject( hdc, hOldPen );
	::SelectObject( hdc, hOldBrush );
	m_mp_wnd.ReleaseDC( hdc );

	return 0;
}

int CMotParamDlg::DrawRange()
{
	if( !IsWindow() )
		return 0;
	if( !m_mp_wnd.IsWindow() )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( m_loadingflag == 1 ){
		return 0;
	}

	HDC hdc;
	hdc = m_range_wnd.GetDC();
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
	if( m_gray_brush == NULL ){
		COLORREF graycol = RGB( 190, 190, 190 );
		m_gray_brush = ::CreateSolidBrush( graycol );
		_ASSERT( m_gray_brush != NULL );
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
	if( m_gray_pen == NULL ){
		COLORREF graycol = RGB( 190, 190, 190 );
		m_gray_pen = ::CreatePen( PS_SOLID, 0, graycol );
		_ASSERT( m_gray_pen != NULL );
	}


//背景塗りつぶし
	RECT clirect;
	m_range_wnd.GetClientRect( &clirect );
	HPEN hOldPen = (HPEN)SelectObject( hdc, m_black_pen );
	HBRUSH hOldBrush = (HBRUSH)SelectObject( hdc, m_white_brush );

	BOOL bret;
	bret = ::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret ){
		_ASSERT( 0 );
		return 1;
	}

//current bar
	int startx, endx;
	Frameno2WindowposRange( clirect, m_startframe, &startx );
	Frameno2WindowposRange( clirect, m_endframe, &endx );




	hOldPen = (HPEN)SelectObject( hdc, m_black_pen );
	hOldBrush = (HBRUSH)SelectObject( hdc, m_gray_brush );
	bret = ::Rectangle( hdc, startx, clirect.top, endx, clirect.bottom );
	if( !bret ){
		_ASSERT( 0 );
		return 1;
	}

	::SelectObject( hdc, hOldPen );
	::SelectObject( hdc, hOldBrush );
	m_range_wnd.ReleaseDC( hdc );

	return 0;
}


int CMotParamDlg::Redraw()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );
	return 0;
}

LRESULT CMotParamDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	if( !IsWindow() )
		return 0;

	if( m_loadingflag == 1 ){
		return 0;
	}

	/***
	if( uMsg != 0 )
		::DefWindowProc( m_hWnd, uMsg, wParam, lParam );
	else
		UpdateAllWnd();
	***/
	DrawMotionPoints();
	DrawRange();

	return 0;
}

LRESULT CMotParamDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	if( m_thandler && m_shandler && m_mhandler && (m_selectedno != 0)){
		::SendMessage( m_apphwnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)m_selectedno );
	}
	Redraw();

	DefWindowProc();
	return 0;
}
LRESULT CMotParamDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "MotParamDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CMotParamDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_ismoving = 0;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	if( m_apphwnd ){
		::SendMessage( m_apphwnd, WM_USER_MOVE, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	}
	DefWindowProc();

	Redraw();

	return 0;
}


int CMotParamDlg::UpdateAllWnd()
{

	m_tex_current_wnd.UpdateWindow();
	m_mp_wnd.UpdateWindow();
	m_sl_mp_wnd.UpdateWindow();
	m_sp_mp_wnd.UpdateWindow();

	m_paramset_wnd.UpdateWindow();

	m_framecopy_wnd.UpdateWindow();
	m_framepaste_wnd.UpdateWindow();
	m_framedel_wnd.UpdateWindow();

	m_preview_wnd.UpdateWindow();
	m_stop_wnd.UpdateWindow();

	m_alldelete_wnd.UpdateWindow();

	//m_tex_bonenum_wnd.UpdateWindow();
	//m_sp_bonenum_wnd.UpdateWindow();
	m_sp_twist_wnd.UpdateWindow();


	m_reset_rot_wnd.UpdateWindow();
	m_reset_mv_wnd.UpdateWindow();


	//m_ok_wnd.UpdateWindow();
	//m_cancel_wnd.UpdateWindow();

	//TreeViewよりも後。(treeviewのcaptionで汚されることがあるため)
	m_dlg_wnd.UpdateWindow();

	return 0;
}


LRESULT CMotParamDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = ::MessageBox( m_hWnd, "全てのキーフレームを削除します。\nよろしいですか？", "確認ダイアログ", MB_OKCANCEL ); 
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK )
		return 0;

	SYSTEMTIME systime;
	GetLocalTime( &systime );
	char autosavename[MAX_PATH];
	sprintf_s( autosavename, MAX_PATH, "%ssave_%d_%d_%d_%d_%d_%d_%d.qua",
		m_d3dapp->m_autosavedir,
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
	);
	
	if( m_mot_cookie >= 0 ){
		// Auto Save
		CQuaFile quafile;
		ret = quafile.SaveQuaFile( g_inidata->opt_namequa, m_thandler, m_shandler, m_mhandler, 
			autosavename, m_mot_cookie, 0, m_sl_mp_rangemax );
		_ASSERT( !ret );
	}

	int i;
	for( i = 1; i < m_mhandler->s2mot_leng; i++ ){
		CMotionCtrl* mcptr = (*m_mhandler)( i );
		_ASSERT( mcptr );	
		CMotionPoint2* mpptr = mcptr->GetMotionPoint( m_mot_cookie );

		CMotionPoint2* curmp = mpptr;
		CMotionPoint2* nextmp;
		while( curmp ){
			int frameno = curmp->m_frameno;
			nextmp = curmp->next;
			
			ret = mcptr->DeleteMotionPoint( m_mot_cookie, frameno );
			if( ret ){
				DbgOut( "MotParamDlg : OnAllDelete : DeleteMotionPoint error !!!\n" );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : OnAllDelete : DeleteMotionPoint error" );
				_ASSERT( 0 );
				return 1;
			}

			curmp = nextmp;
		}
	}
	
	if( m_dragmp ){
		delete m_dragmp;
		m_dragmp = 0;
	}
	graymove = -1;


	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}


	UpdateAllMatrix( 1 );

	Redraw();

	return 0;
}


LRESULT CMotParamDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
//	if( !m_thandler || !m_shandler || !m_mhandler )
//		return 0;
//	if( m_selectedno == 0 )
//		return 0;
	if( g_endapp == 1 ){
		return 0;
	}
	

	if( m_timerworking != 1 ){
		return 0;
	}

	if( m_d3dapp && (m_d3dapp->m_tbb_mo == 1) ){
		StopMoa();
		return 0;//!!!!!!!!!!!!!!!!
	}

	g_inidata->opt_bmdisp = m_savebmdisp;
	g_inidata->opt_jointdisp = m_savejdisp;


//	if( wID == 9999 ){
		InterlockedExchange( &m_previewframe, m_current );//タブの切り替え時にはフレームを移動しない。	
		if( m_camkeydlg ){
			if( g_endapp == 1 ){
				return 0;
			}
			InterlockedExchange( &m_camkeydlg->m_previewframe, m_camkeydlg->m_current );//タブの切り替え時にはフレームを移動しない。	
		}
		if( m_gpkeydlg ){
			if( g_endapp == 1 ){
				return 0;
			}
			InterlockedExchange( &m_gpkeydlg->m_previewframe, m_gpkeydlg->m_current );//タブの切り替え時にはフレームを移動しない。	
		}
		if( m_moekeydlg ){
			if( g_endapp == 1 ){
				return 0;
			}
			InterlockedExchange( &m_moekeydlg->m_previewframe, m_moekeydlg->m_current );//タブの切り替え時にはフレームを移動しない。	
		}
		if( m_sndkeydlg ){
			if( g_endapp == 1 ){
				return 0;
			}
			InterlockedExchange( &m_sndkeydlg->m_previewframe, m_sndkeydlg->m_current );//タブの切り替え時にはフレームを移動しない。	
		}
//	}

	if( m_apphwnd ){
		if( g_endapp == 1 ){
			return 0;
		}
		::SendMessage( m_apphwnd, WM_USER_PREVIEW, 0, 0 );
	}

	InterlockedExchange( &m_preview_flag, 0 );	
	SetEvent( s_hEvent );

	CheckPreviewButton();
	if( m_camkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_camkeydlg->SetPreviewButton();
	}
	if( m_gpkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_gpkeydlg->SetPreviewButton();
	}
	if( m_moekeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_moekeydlg->SetPreviewButton();
	}
	if( m_sndkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_sndkeydlg->SetPreviewButton();
		m_sndkeydlg->StopBGM();//!!!!!!!!!!!!!
		m_sndframe = m_sndkeydlg->m_startframe;
	}

///////////
	if( !m_d3dapp ){
		return 0;//!!!!!!!!!!!!
	}

	int moeupdate = 0;
	int moeframeno = m_current;
	if( m_moekeydlg ){
		moeframeno = m_moekeydlg->m_current;
	}

	int idlingid;
	switch(g_motdlg->m_disptabno) {
		case TAB_CAM:
			if( (m_d3dapp->m_syncdlg->m_moeflag != 0) && (m_d3dapp->m_syncdlg->m_camflag != 0) ){
				moeupdate = 1;
				moeframeno = m_camkeydlg->m_current;
			}
			break;
		case TAB_GP:
			if( (m_d3dapp->m_syncdlg->m_moeflag != 0) && (m_d3dapp->m_syncdlg->m_gpaflag != 0) ){
				moeupdate = 1;
				moeframeno = m_gpkeydlg->m_current;
			}
			break;
		case TAB_MOE:
			if( m_d3dapp->m_mcdlg->m_mch ){
				idlingid = m_d3dapp->m_mcdlg->m_mch->GetIdlingMotID();
			}else{
				idlingid = -1;
			}
			if( idlingid >= 0 ){
				moeupdate = 1;
				moeframeno = m_moekeydlg->m_current;
			}
			break;
		case TAB_SND:
			if( m_d3dapp->m_shandler && (m_d3dapp->m_soundbank->m_setnum > 0) ){
				if( (m_d3dapp->m_syncdlg->m_moeflag != 0) && (m_d3dapp->m_syncdlg->m_sndflag != 0) ){
					moeupdate = 1;
					moeframeno = m_sndkeydlg->m_current;
				}
			}
			break;
		default:
			break;
	}

	if( moeupdate == 1 ){
		m_moekeydlg->OnJumpFrame( moeframeno );
		m_moekeydlg->SetCurrentMotion( 0 );
	}

	//m_grotdlg->Redraw();

	return 0;
}


LRESULT CMotParamDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( m_preview_flag != 0 ){
		return 0;
	}

	s_mpd = this;
	s_ckd = m_camkeydlg;
	s_gpkd = m_gpkeydlg;
	s_moekd = m_moekeydlg;
	s_sndkd = m_sndkeydlg;

	if( m_d3dapp && (m_d3dapp->m_tbb_mo == 1) ){
		PreviewMoa();
		return 0;
	}

	m_savebmdisp = g_inidata->opt_bmdisp;
	m_savejdisp = g_inidata->opt_jointdisp;
	g_inidata->opt_bmdisp = 0;

	if( m_sndkeydlg ){
		m_sndkeydlg->StopBGM();
	}


	LONG previewmotion = 1;
	LONG previewcamera = 1;
	LONG previewgp = 1;
	LONG previewmoe = 1;
	LONG previewsnd = 1;
	LONG withsndflag = 0;

	if( !m_thandler || !m_shandler || !m_mhandler ){
		previewmotion = 0;
	}
//	if( m_selectedno == 0 ){
//		previewmotion = 0;
//	}
	if( m_motionnum <= 0 ){
		previewmotion = 0;
	}

	if( m_camkeydlg->m_cah->m_animnum <= 0 ){
		previewcamera = 0;
	}
	if( m_camkeydlg->m_animno < 0 ){
		previewcamera = 0;
	}

	if( m_gpkeydlg->m_gpah->m_animnum <= 0 ){
		previewgp = 0;
	}
	if( m_gpkeydlg->m_animno < 0 ){
		previewgp = 0;
	}

	if( m_moekeydlg->m_moeah->m_animnum <= 0 ){
		previewmoe = 0;
	}
	if( m_moekeydlg->m_animno < 0 ){
		previewmoe = 0;
	}
	if( previewmoe == 1 ){//キーが１個もない場合は再生しない。
		CMOEAnim* anim;
		anim = m_moekeydlg->m_moeah->GetCurMOEAnim();
		if( anim ){
			if( !anim->m_firstkey ){
				previewmoe = 0;
			}
		}else{
			previewmoe = 0;
		}
	}

	if( m_sndkeydlg->m_sndah->m_animnum <= 0 ){
		previewsnd = 0;
	}
	if( m_sndkeydlg->m_animno < 0 ){
		previewsnd = 0;
	}
	if( m_sndkeydlg->m_syncflag == 0 ){
		previewsnd = 0;
	}


	if( previewmoe == 1 ){
		previewmotion = 0;//!!!!!!!!!
	}
	withsndflag = previewsnd;//!!!!!!


	if( previewmotion == 1 ){
//		InterlockedExchange( &m_previewframe, m_current );
		InterlockedExchange( &s_startframe, m_startframe );
		InterlockedExchange( &s_frameno, m_startframe );
		InterlockedExchange( &m_current, m_startframe );
		ParamsToDlg();
		ret = m_mhandler->SetMotionFrameNo( m_shandler, m_mot_cookie, s_frameno, 1 );
		_ASSERT( !ret );
	}
	if( previewcamera == 1 ){
//		InterlockedExchange( &m_camkeydlg->m_previewframe, m_camkeydlg->m_current );
		InterlockedExchange( &m_camkeydlg->m_current, m_camkeydlg->m_startframe );		
		m_camkeydlg->SetCurrentMotion( 0 );
		m_camkeydlg->ParamsToDlg();
	}
	if( previewgp == 1 ){
//		InterlockedExchange( &m_gpkeydlg->m_previewframe, m_gpkeydlg->m_current );
		InterlockedExchange( &m_gpkeydlg->m_current, m_gpkeydlg->m_startframe );		
		m_gpkeydlg->SetCurrentMotion( 0 );
		m_gpkeydlg->ParamsToDlg();
	}
	if( previewmoe == 1 ){
//		InterlockedExchange( &m_moekeydlg->m_previewframe, m_moekeydlg->m_current );
		InterlockedExchange( &m_moekeydlg->m_current, m_moekeydlg->m_startframe );		
		m_moekeydlg->SetCurrentMotion( 0 );
		m_moekeydlg->ParamsToDlg();
	}
	if( previewsnd == 1 ){
//		InterlockedExchange( &m_sndkeydlg->m_previewframe, m_sndkeydlg->m_current );
		InterlockedExchange( &m_sndkeydlg->m_current, m_sndkeydlg->m_startframe );		
		m_sndframe = m_sndkeydlg->m_startframe;
		m_sndkeydlg->SetCurrentMotion( 0 );
		m_sndkeydlg->ParamsToDlg();
	}


	InterlockedExchange( &m_preview_flag, 1 );
	InterlockedExchange( &m_previewmotion, previewmotion );
	InterlockedExchange( &m_previewcamera, previewcamera );
	InterlockedExchange( &m_previewgp, previewgp );
	InterlockedExchange( &m_previewmoa, 0 );
	InterlockedExchange( &m_previewmoe, previewmoe );
	InterlockedExchange( &m_previewsnd, previewsnd );
	InterlockedExchange( &m_withsndflag, withsndflag );

//DbgOut( "check !!! mpd : OnPreview : m_previewmotion %d, m_previewcamera %d\r\n", m_previewmotion, m_previewcamera );

	if( (m_previewmotion == 1) || (m_previewcamera == 1) || (m_previewgp == 1) || (m_previewmoe == 1) || (m_previewsnd == 1) ){
		if( m_apphwnd )
			::SendMessage( m_apphwnd, WM_USER_PREVIEW, 0, 1 );
	}

DbgOut( "check !!! : motparam : OnPreview : previewmotion %d, previewmoe %d, previewsnd %d\r\n",
	m_previewmotion, m_previewmoe, m_previewsnd );

	CheckPreviewButton();
	m_camkeydlg->SetPreviewButton();
	m_gpkeydlg->SetPreviewButton();
	m_moekeydlg->SetPreviewButton();
	m_sndkeydlg->SetPreviewButton();

	SetEvent( s_hEvent );// msgwait解除 (解除の前に、previewflag 1)


	//SetEvent( s_hEvent );
	SleepEx( 0, FALSE );
//	ResetEvent( s_hEvent );

	return 0;
}

int CMotParamDlg::PreviewMoa()
{
	if( m_preview_flag != 0 ){
		return 0;
	}

	m_d3dapp->ResetKeyboardCnt();

	s_mpd = this;
	s_ckd = m_camkeydlg;
	s_gpkd = m_gpkeydlg;
	s_mch = m_mch;


	LONG previewmotion = 0;
	LONG previewcamera = 0;
	LONG previewgp = 0;
	LONG previewmoa = 1;

	if( !m_thandler || !m_shandler || !m_mhandler ){
		previewmoa = 0;
	}
	if( m_motionnum <= 0 ){
		previewmoa = 0;
	}
	int idlingmotid = -1;
	idlingmotid = m_mch->GetIdlingMotID();
	if( idlingmotid < 0 ){
		previewmoa = 0;
	}

	int ret;
	if( previewmoa == 1 ){
		ret = m_mhandler->SetMotionKind( idlingmotid );
		_ASSERT( !ret );
	}

	InterlockedExchange( &m_preview_flag, 1 );
	InterlockedExchange( &m_previewmotion, 0 );
	InterlockedExchange( &m_previewcamera, 0 );
	InterlockedExchange( &m_previewgp, 0 );
	InterlockedExchange( &m_previewmoa, previewmoa );
	InterlockedExchange( &m_previewmoe, 0 );
	InterlockedExchange( &m_withsndflag, 0 );

DbgOut( "checkplay!!! PreviewMoa : m_previewmoa, m_previewmotion %d, m_previewcamera %d\r\n", 
	   m_previewmoa, m_previewmotion, m_previewcamera );

	if( m_previewmoa == 1 ){
		if( m_apphwnd )
			::SendMessage( m_apphwnd, WM_USER_PREVIEW, 0, 1 );
	}
	CheckPreviewButton();
	SetEvent( s_hEvent );// msgwait解除 (解除の前に、previewflag 1)
	SleepEx( 0, FALSE );

	return 0;
}
int CMotParamDlg::StopMoa()
{

DbgOut( "checkplay!!! : StopMoa\r\n" );
	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_PREVIEW, 0, 0 );

	InterlockedExchange( &m_preview_flag, 0 );	
	SetEvent( s_hEvent );

	CheckPreviewButton();

	return 0;
}

	

LRESULT CMotParamDlg::OnResetRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret, resetno;
	if( m_sellockno != 0 )
		resetno = m_sellockno;
	else
		resetno = m_selectedno;

	if( resetno <= 0 )
		return 0;

	CMotionCtrl* mcptr = (*m_mhandler)( resetno );
	_ASSERT( mcptr );
		
/***
		//確認ダイアログを出す。
	int shdnum = m_shandler->s2shd_leng;
	int* resetlist;
	resetlist = (int*)malloc( sizeof( int ) * shdnum );
	if( !resetlist ){
		_ASSERT( 0 );
		DbgOut( "MotParamDlg : OnResetRot : alloc error !!!\n" );
		return 1;
	}
	ZeroMemory( resetlist, sizeof( int ) * shdnum );
	*( resetlist + resetno ) = 1;


	char confirmtext[1024];
	ZeroMemory( confirmtext, 1024 );
	sprintf_s( confirmtext, 1024,  
		"フレーム番号:%d の\n以下のジョイントの回転を\nリセットします。",
		m_current );
	CFrameOpeDlg dlg( 1, confirmtext, resetlist, m_thandler );

	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal();
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	free( resetlist );

	if( ret != IDOK )
		return 0;
***/

	CQuaternion initq;
	initq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		m_q = initq;
		
		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );

		D3DXVECTOR3 inieul( 0.0f, 0.0f, 0.0f );
		curmp->m_eul = inieul;


		if( m_d3dapp ){
			m_d3dapp->OnPluginPose();
		}


		//UpdateAllMatrix();
		UpdateMatrixSelected( resetno, 0, 1 );

		//if( needredraw )
		//	Redraw();

	}


	return 0;
}

LRESULT CMotParamDlg::OnResetMv(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret, resetno;
	if( m_sellockno != 0 )
		resetno = m_sellockno;
	else
		resetno = m_selectedno;

	if( resetno <= 0 )
		return 0;

	CMotionCtrl* mcptr = (*m_mhandler)( resetno );
	_ASSERT( mcptr );
		
/***
		//確認ダイアログを出す。
	int shdnum = m_shandler->s2shd_leng;
	int* resetlist;
	resetlist = (int*)malloc( sizeof( int ) * shdnum );
	if( !resetlist ){
		_ASSERT( 0 );
		DbgOut( "MotParamDlg : OnResetMv : alloc error !!!\n" );
		return 1;
	}
	ZeroMemory( resetlist, sizeof( int ) * shdnum );
	*( resetlist + resetno ) = 1;

	char confirmtext[1024];
	ZeroMemory( confirmtext, 1024 );
	sprintf_s( confirmtext, 1024,
		"フレーム番号:%d の\n以下のジョイントの移動を\nリセットします。",
		m_current );
	CFrameOpeDlg dlg( 1, confirmtext, resetlist, m_thandler );

	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal();
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	free( resetlist );

	if( ret != IDOK )
		return 0;
***/

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		m_q = curmp->m_q;
		
		m_mvx = 0.0f;
		m_mvy = 0.0f;
		m_mvz = 0.0f;
		
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;
	
//		m_scalex = 1.0f;
//		m_scaley = 1.0f;
//		m_scalez = 1.0f;
//		m_userint1 = 0;
		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;

		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );

		if( m_d3dapp ){
			m_d3dapp->OnPluginPose();
		}


		//UpdateAllMatrix();
		UpdateMatrixSelected( resetno, 0, 1 );

		//if( needredraw )
		//	Redraw();
	}

	return 0;
}

LRESULT CMotParamDlg::OnResetSc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret, resetno;
	if( m_sellockno != 0 )
		resetno = m_sellockno;
	else
		resetno = m_selectedno;

	if( resetno <= 0 )
		return 0;

	CMotionCtrl* mcptr = (*m_mhandler)( resetno );
	_ASSERT( mcptr );
		
/***
		//確認ダイアログを出す。
	int shdnum = m_shandler->s2shd_leng;
	int* resetlist;
	resetlist = (int*)malloc( sizeof( int ) * shdnum );
	if( !resetlist ){
		_ASSERT( 0 );
		DbgOut( "MotParamDlg : OnResetSc : alloc error !!!\n" );
		return 1;
	}
	ZeroMemory( resetlist, sizeof( int ) * shdnum );
	*( resetlist + resetno ) = 1;


	char confirmtext[1024];
	ZeroMemory( confirmtext, 1024 );
	sprintf_s( confirmtext, 1024, 
		"フレーム番号:%d の\n以下のジョイントの拡大縮小を\nリセットします。",
		m_current );
	CFrameOpeDlg dlg( 1, confirmtext, resetlist, m_thandler );

	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal();
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	free( resetlist );

	if( ret != IDOK )
		return 0;
***/

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		m_q = curmp->m_q;
				
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;
	
		m_scalex = 1.0f;
		m_scaley = 1.0f;
		m_scalez = 1.0f;

		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		m_userint1 = curmp->m_userint1;


		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );

		if( m_d3dapp ){
			m_d3dapp->OnPluginPose();
		}


		//UpdateAllMatrix();
		UpdateMatrixSelected( resetno, 0, 1 );

		//if( needredraw )
		//	Redraw();
	}

	return 0;
}

LRESULT CMotParamDlg::OnRadioModeJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_MODEJOINT );
	if( checkflag == BST_CHECKED ){
		m_ikmode = IKMODE_JOINT;
	}else{
		return 0;
	}

	int setno;
	if( m_sellockno != 0 )
		setno = m_sellockno;
	else
		setno = m_selectedno;

	if( setno == 0 ){
		::MessageBox( NULL, "ジョイントをマウスで選択してから、再試行してください。", "未選択エラー", MB_OK );
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_MODEJOINT, IDC_RADIO_MODEOPE, IDC_RADIO_MODEOPE );
		m_ikmode = IKMODE_OPE;//!!!!!!!!!!!!!!!!!1

		return 0;
	}

	CShdElem* selelem;
	selelem = (*m_shandler)( setno );
	if( !selelem )
		return 0;

	m_iklevel = selelem->m_iklevel;

	UpdateComboDlg();


//DbgOut( "motparamdlg : radio joint : mode %d, joint %d, ope %d\r\n", m_ikmode, m_iklevel, m_iklevelope );

	return 0;
}
LRESULT CMotParamDlg::OnRadioModeOpe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_MODEOPE );
	if( checkflag == BST_CHECKED ){
		m_ikmode = IKMODE_OPE;
	}else{
		return 0;
	}

	UpdateComboDlg();

//DbgOut( "motparamdlg : radio joint : mode %d, joint %d, ope %d\r\n", m_ikmode, m_iklevel, m_iklevelope );

	return 0;
}


LRESULT CMotParamDlg::OnBoneDisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;
	if( !m_apphwnd )
		return 0;

	::SendMessage( m_apphwnd, WM_USER_BONEDISP, 0, 0 );

	return 0;
}

LRESULT CMotParamDlg::OnIKTDisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
//	if( m_preview_flag == 1 )
//		return 0;
	if( !m_d3dapp )
		return 0;

	if( g_inidata->opt_iktdisp ){
		g_inidata->opt_iktdisp = 0;
		m_d3dapp->m_optdlg->m_iktdisp = 0;
	}else{
		g_inidata->opt_iktdisp = 1;
		m_d3dapp->m_optdlg->m_iktdisp = 1;
	}

	return 0;
}


int CMotParamDlg::ParamsFromMotionPoint( CMotionPoint2* srcmp )
{
	int frameno = srcmp->m_frameno;
	//_ASSERT( frameno == m_current );

	m_q = srcmp->m_q;

	m_mvx = srcmp->m_mvx;
	m_mvy = srcmp->m_mvy;
	m_mvz = srcmp->m_mvz;

	//m_dispswitch = srcmp->dispswitch;
	m_interp = srcmp->interp;
	return 0;
}

int CMotParamDlg::InitRotAndMv()
{
	
	m_mvx = 0.0f;
	m_mvy = 0.0f;
	m_mvz = 0.0f;
	
	m_q.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

	DbgOut( "MotParamDlg : InitRotAndMv\n" );
	return 0;
}

int CMotParamDlg::UpdateAllMatrix( int spallkind )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	int ret = 0;

	ret += m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	ret += m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( spallkind, -1 );
		if( ret ){
			DbgOut( "motparamdlg : UpdateAllMatrix : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	SetCurrentMotion();

	return 0;
}

int CMotParamDlg::UpdateMatrix( int srcseri, int spallkind )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_mhandler->m_kindnum <= 0 )
		return 0;
	if( m_mot_cookie < 0 )
		return 0;

	CShdElem* selem = (*m_shandler)( srcseri );

	int ret = 0;
	ret += m_mhandler->MakeRawmat( m_mot_cookie, selem, -1, -1, m_shandler );
	ret += m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : UpdateMatrix error" );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( spallkind, srcseri );
		if( ret ){
			DbgOut( "motparamdlg : updateMatirx : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	return 0;
}
int CMotParamDlg::UpdateMatrixSelected( int srcseri, int curframeonly, int spallkind )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	int startframe, endframe;

	if( curframeonly ){
		startframe = m_current;
		endframe = m_current;
	}else{
		startframe = -1;
		endframe = -1;
	}

	CShdElem* selem = (*m_shandler)( srcseri );

	int ret = 0;
	ret = m_mhandler->MakeRawmat( m_mot_cookie, selem, startframe, endframe, m_shandler );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	int errorflag = 0;
	m_mhandler->RawmatToMotionmatReq( m_mot_cookie, m_shandler, srcseri, &errorflag, startframe, endframe, 0 );
	if( errorflag ){
		_ASSERT( 0 );
		return 1;
	}

	if( m_calcsp ){
		ret = CalcSplineParam( spallkind, -1 );
		if( ret ){
			DbgOut( "motparamdlg : UpdateMatrixSelected : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	CMotionCtrl* mcptr;
	int errorflag2 = 0;
	mcptr = (*m_mhandler)( srcseri );
	if( mcptr ){
		m_mhandler->SetCurrentMotionReq( m_shandler->m_mathead, mcptr, m_mot_cookie, m_current, 0, &errorflag2 );
		if( errorflag2 ){
			_ASSERT( 0 );
			return 1;
		}
	}
	return 0;
}


int CMotParamDlg::SetCurrentMotion()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_mot_cookie < 0 )
		return 0;

	int ret;
	ret = m_mhandler->SetCurrentMotion( m_shandler, m_mot_cookie, m_current );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : SetCurrentMotion error" );
		return 1;
	}

	return ret;
}

int CMotParamDlg::SetAppWnd( HWND appwnd )
{
	m_apphwnd = appwnd;
	s_apphwnd = appwnd;//!!!!!
	return 0;
}



int CMotParamDlg::SetParams( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, HWND papp, CMyD3DApplication* d3dapp )
{

	//int ret;
	if( m_thandler || m_shandler || m_mhandler ){
		//既存のTreeを壊す。
		InvalidateTreeView();

		//motionobj削除関数は、viewerのDestroyHanlerから呼ばれる。
		//ret = m_mhandler->DestroyMotionObj();
		//_ASSERT( !ret );
	}
	m_d3dapp = d3dapp;

	m_thandler = lpth;
	m_shandler = lpsh;
	m_mhandler = lpmh;

	m_apphwnd = papp;
	s_apphwnd = m_apphwnd;//!!!!!!!!
	m_invalidate_handler = 0;


	//TreeView_SetItemHeight( m_tree_mot_wnd, 8 );//<---文字サイズは変わらない。

	int ret;
	ret = InitDlg();
	if( ret ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : SetParams : InitDlg error" );
		return 1;
	}



	return 0;
}
int CMotParamDlg::GetMotParams( int* mcookieptr, int* framenoptr )
{
	*mcookieptr = m_mot_cookie;
	*framenoptr = m_current;

	return 0;
}
int CMotParamDlg::GetFrameLeng( int motid, int* lengptr )
{
	if( (motid < 0) || (motid >= m_motionnum) ){
		DbgOut( "motparamdlg : GetFrameLeng : motid error !!!\n" );
		_ASSERT( 0 );
		*lengptr = 0;
		return 1;		
	}else{

		CMotionCtrl* mcptr = (*m_mhandler)( 1 );
		if( !mcptr ){
			_ASSERT( 0 );
			DbgOut( "motparamdlg : GetFrameLeng :  mcptr error" );
			return 1;
		}
		CMotionInfo* miptr = mcptr->motinfo;
		if( !miptr ){
			_ASSERT( 0 );
			DbgOut( "motparamdlg : GetFrameLeng : miptr error" );
			return 1;
		}
		
		*lengptr = *(miptr->motnum + motid);
	}

	return 0;
}

int CMotParamDlg::DestroyMotionObj()
{
	//viewerのDestroyHanlerから呼ばれる。
	int ret = 0;
	if( m_mhandler ){
		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			_ASSERT( 0 );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : DestroyMotionObj error" );
			return 1;
		}
	}
	m_mot_cookie = -1;
	
	return ret;
}



int CMotParamDlg::OnUserSelchange( int selno )
{
	if( m_thandler && m_shandler && m_mhandler ){
		if( m_selectedno != 0 ){

			CShdElem* selem;
			selem = (*m_shandler)( selno );
			if( selem->IsJoint() ){
				m_selectedno = selno;

				BOOL dummy;
				OnReleasedcaptureSl_mp( 0, 0, dummy );

		///////////
				OnRadioModeJoint(0, 0, 0, dummy);//!!!!!!!!!!!!!!!!!!
				//OnRadioModeOpe(0, 0, 0, dummy);//!!!!!!!!!!!!!!!!!!

				if( m_cmdshow != SW_HIDE ){
					Redraw();
				}
			}
		}
	}

	if( m_texkeydlg ){
		m_texkeydlg->OnUserSelchange();//!!!!!!!!!!!!!!!
	}
	if( m_alpkeydlg ){
		m_alpkeydlg->OnUserSelchange();//!!!!!!!!!!!!!!!
	}
	if( m_dskeydlg ){
		m_dskeydlg->OnUserSelchange();//!!!!!!!!!!!!!!!
	}

	return 0;
}

int CMotParamDlg::SetDisplayFlag( int dflag )
{
	m_displayflag = dflag;
	return 0;
}


LRESULT CMotParamDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "MotParamDlg : OnMouseMove : MK_LBUTTON\n" );

	if( (m_dragflag == 1) && m_dragmp ){
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

		m_dragmp->m_frameno = frameno;

		Redraw();
	}

	if( (m_dragflag == 1) && (graymove >= 0) ){
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

		graymove = frameno;

		Redraw();
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

		m_motid[ m_mot_cookie ].endframe = m_endframe;//!!!!!!!!!!!!

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

		m_motid[ m_mot_cookie ].startframe = m_startframe;
		m_motid[ m_mot_cookie ].endframe = m_endframe;


		ParamsToDlg();
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}

int CMotParamDlg::InvalidateHandler()
{
	m_invalidate_handler = 1;
	InvalidateTreeView();
	m_thandler = 0;
	m_shandler = 0;
	m_mhandler = 0;

	m_selectedno = 0;
	//m_selecteditem = 0;


	m_motionnum = 0;
	int motno;
	for( motno = 0; motno < MAXMOTIONNUM; motno++ ){
		m_motid[motno].id = -1;
		ZeroMemory( m_motid[motno].filename, sizeof( char ) * 256 );
		m_motid[motno].ev0idle = 0;
		m_motid[motno].commonid = 0;
		if( m_motid[motno].forbidid ){
			free( m_motid[motno].forbidid );
			m_motid[motno].forbidid = 0;
		}
		m_motid[motno].forbidnum = 0;
		m_motid[motno].notfu = 0;
	}


	return 0;
}

int CMotParamDlg::CopyUndoBuffer( int cpframe )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

	int bufno, destno, shdnum;

	shdnum = m_shandler->s2shd_leng;
	if( shdnum < 1 ){
		return 0;//!!!!!!!!!
	}


	for( bufno = MAXUNDONUM - 2; bufno >= 0; bufno-- ){
		if( bufno < 0 )
			break;
		
		destno = bufno + 1;
		
		m_undobuf[destno].frameno = m_undobuf[bufno].frameno;
		MoveMemory( m_undobuf[destno].seriarray, m_undobuf[bufno].seriarray, sizeof( int ) * shdnum );
		MoveMemory( m_undobuf[destno].mparray, m_undobuf[bufno].mparray, sizeof( CMotionPoint2 ) * shdnum );
	}



	if( cpframe < 0 ){
		cpframe = m_current;//!!!!!!!!!
	}


	//m_undobuf[0].frameno = m_current;
	m_undobuf[0].frameno = cpframe;

	CShdElem* curselem;
	CMotionPoint2* buffptr;
	int serino;
	for( serino = 1; serino < m_shandler->s2shd_leng; serino++ ){
		curselem = (*m_shandler)( serino );
		_ASSERT( curselem );
		buffptr = m_undobuf[0].mparray + serino;
		if( curselem->IsJoint() ){


			*(m_undobuf[0].seriarray + serino) = 1;

			CMotionCtrl* mcptr = (*m_mhandler)( serino );
			_ASSERT( mcptr );
			//CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
			CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, cpframe );
			

			ret = mcptr->PutQuaternionInOrder( m_mot_cookie );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			if( isexist ){
				//buffptr->m_frameno = m_current;
				buffptr->m_frameno = cpframe;
				buffptr->m_mvx = isexist->m_mvx;
				buffptr->m_mvy = isexist->m_mvy;
				buffptr->m_mvz = isexist->m_mvz;
				buffptr->m_q = isexist->m_q;
				buffptr->dispswitch = 0xFFFFFFFF;

				buffptr->interp = isexist->interp;

				buffptr->m_scalex = isexist->m_scalex;
				buffptr->m_scaley = isexist->m_scaley;
				buffptr->m_scalez = isexist->m_scalez;

				buffptr->m_eul = isexist->m_eul;

			}else{
				*(m_undobuf[0].seriarray + serino) = 2;//!!!!!!!!!!
				//buffptr->m_frameno = m_current;
				buffptr->m_frameno = cpframe;
			}

		}else{
			*(m_undobuf[0].seriarray + serino) = 0;
			buffptr->m_frameno = -1;
		}
	}

	return 0;
}


int CMotParamDlg::PasteUndoBuffer( int undocnt, int undodlg )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( (undocnt < 0) || (undocnt >= MAXUNDONUM) )
		return 0;


	int pasteframe, ret;
	pasteframe = m_undobuf[undocnt].frameno;

	if( (pasteframe < 0) || (pasteframe > m_sl_mp_rangemax) )
		return 0;


	if( undodlg == CHKDLG_NES ){
		//確認ダイアログを出す。
		char confirmmes[256];
		sprintf_s( confirmmes, 256, "UNDO %d 回目\nフレーム番号%d へのIK操作をUNDOします。\nよろしいですか？", undocnt + 1, pasteframe );

		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_apphwnd, confirmmes, "確認ダイアログ", MB_OKCANCEL ); 
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK )
			return 0;
	}

	//ペーストを行う。
	int cpnum = 0;
	int serino;
	CMotionPoint2* curmp;
	for( serino = 0; serino < m_shandler->s2shd_leng; serino++ ){
		curmp = m_undobuf[undocnt].mparray + serino;

		if( ( *(m_undobuf[undocnt].seriarray + serino) == 1 ) && ( (m_undobuf[undocnt].mparray + serino)->m_frameno == pasteframe ) ){
			CMotionCtrl* mcptr = (*m_mhandler)( serino );
			_ASSERT( mcptr );

			if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
				CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, pasteframe );
				if( isexist ){
					//isexistにcurmpの値をセット。
					isexist->m_mvx = curmp->m_mvx;
					isexist->m_mvy = curmp->m_mvy;
					isexist->m_mvz = curmp->m_mvz;
					isexist->m_q = curmp->m_q;
					//isexist->dispswitch = curmp->dispswitch;
					isexist->dispswitch = 0xFFFFFFFF;
					isexist->interp = curmp->interp;

					isexist->m_scalex = curmp->m_scalex;
					isexist->m_scaley = curmp->m_scaley;
					isexist->m_scalez = curmp->m_scalez;
					isexist->m_userint1 = curmp->m_userint1;
					isexist->m_eul = curmp->m_eul;

				}else{
					//MotionPointを新しく作り、curmpの値をセット。

					CMotionPoint2* newmp = mcptr->AddMotionPoint( m_mot_cookie, pasteframe,
						curmp->m_q,
						//curmp->m_mvx, curmp->m_mvy, curmp->m_mvz, curmp->dispswitch, curmp->interp, curmp->m_scalex, curmp->m_scaley, curmp->m_scalez, m_userint1 );
						curmp->m_mvx, curmp->m_mvy, curmp->m_mvz, m_dispswitch, curmp->interp, curmp->m_scalex, curmp->m_scaley, curmp->m_scalez, m_userint1 );
					if( !newmp ){
						_ASSERT( 0 );
						ErrorMes( -1, MSGERR_APPMUSTEXIT, "PasteUndoBuffer : AddMotionPoint error" );
						return 1;
					}
					newmp->m_eul = curmp->m_eul;
				}

				if( serino == m_selectedno ){
					ParamsFromMotionPoint( curmp );
				}
				cpnum++;
			}
		}else if( ( *(m_undobuf[undocnt].seriarray + serino) == 2 ) && ( (m_undobuf[undocnt].mparray + serino)->m_frameno == pasteframe ) ){
			//モーションポイントがあれば削除する。

			CMotionCtrl* mcptr = (*m_mhandler)( serino );
			_ASSERT( mcptr );

			if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
				CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, pasteframe );
				if( isexist ){
					ret = mcptr->DeleteMotionPoint( m_mot_cookie, pasteframe );
					if( ret ){
						DbgOut( "motparamdlg : PasteUndoBuffer : mc DeleteMotionPoint error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}
			}
		}
	}

	UpdateAllMatrix( 1 );


	if( cpnum ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	if( m_apphwnd ){
		::SendMessage( m_apphwnd, WM_USER_INIT_UNDOBUF, 0, 0 );
	}

	Redraw();

	return 0;
}
int CMotParamDlg::CopyLowerMotion( CShdElem* srcselem )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( !srcselem )
		return 0;


	int serino;
	int shdnum = m_shandler->s2shd_leng;

	CFrameCopyDlg fcdlg( m_thandler, m_shandler, m_hWnd, m_sl_mp_rangemax );
	int ret;

	ret = fcdlg.CreateLists();
	if( ret ){
		DbgOut( "MotParamDlg : OnFrameCopy : fcdlg CreateLists error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	//int dlgret;
	//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	//dlgret = (int)fcdlg.DoModal( m_hWnd );
	//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

	//if( dlgret != IDOK ){
	//	return 0;
	//}

	*(fcdlg.m_influencelist) = srcselem->serialno;
	fcdlg.m_influencenum = 1;
	BOOL dummy;
	fcdlg.OnOK( 999, 0, 0, dummy );


	InitCEBuff();//!!!!!!!!


	COPYELEM* buffptr;
	int cpnum = 0;
	int invalidnum = 0;
	for( serino = 0; serino < shdnum; serino++ ){

		buffptr = m_CEBuff + serino;

		if( *(fcdlg.m_cplist + serino) == 1 ){

			CMotionCtrl* mcptr = (*m_mhandler)( serino );
			CTreeElem2* teptr = (*m_thandler)( serino );
			_ASSERT( mcptr );
			_ASSERT( teptr );
			CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
			if( isexist ){
				buffptr->mp.m_frameno = m_current;
				buffptr->mp.m_mvx = isexist->m_mvx;
				buffptr->mp.m_mvy = isexist->m_mvy;
				buffptr->mp.m_mvz = isexist->m_mvz;
				buffptr->mp.m_q = isexist->m_q;
				//buffptr->mp.dispswitch = isexist->dispswitch;
				buffptr->mp.dispswitch = 0xFFFFFFFF;

				buffptr->mp.interp = isexist->interp;

				buffptr->mp.m_scalex = isexist->m_scalex;
				buffptr->mp.m_scaley = isexist->m_scaley;
				buffptr->mp.m_scalez = isexist->m_scalez;

				buffptr->mp.m_userint1 = isexist->m_userint1;
				int namelen;
				namelen = (int)strlen( teptr->name );
				if( namelen >= 256 ){
					invalidnum++;
					buffptr->mp.m_frameno = -1;
				}else{
					strcpy_s( buffptr->name, 256, teptr->name );
				}

				cpnum++;
			}else{
				//対称ペーストのためにモーションポイントを計算して格納する
				CShdElem* selem;
				selem = (*m_shandler)( serino );
				int hasmpflag;
				ret = mcptr->CalcMotionPointOnFrame( selem, &buffptr->mp, m_mot_cookie, m_current, &hasmpflag, m_shandler, m_mhandler );
				if( ret ){
					DbgOut( "motparamdlg : CopyLowerMotion : mc CalcMotionPointOnFrame error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				int namelen;
				namelen = (int)strlen( teptr->name );
				if( namelen >= 256 ){
					invalidnum++;
					buffptr->mp.m_frameno = -1;
				}else{
					strcpy_s( buffptr->name, 256, teptr->name );
				}

				buffptr->mp.m_frameno = -1;//!!!!!!!!!!!!!!!!!!!!!!
				*(fcdlg.m_cplist + serino) = 0;//!!!!!!!!!!!!!!! 確認ダイアログのために修正

			}
		}else{
			buffptr->mp.m_frameno = -1;
		}

	}

	if( cpnum != 0 ){
		
		//クリップボードにコピーする。
		HGLOBAL hGlobal;
		ret = CopyToClipBoardData( m_shandler->s2shd_leng, 1, m_CEBuff, &hGlobal );
		if( ret ){
			DbgOut( "motparamdlg : OnFrameCopy : CopyToClipBoardData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( ::OpenClipboard( m_hWnd ) == 0 ){
			DbgOut( "motparamdlg : OnFrameCopy : OpenClipboard error !!!\n" );
			_ASSERT( 0 );
			GlobalFree(hGlobal);
			return 1;
		}
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
		::CloseClipboard();

/***
		//確認ダイアログを出す。
		CFrameOpeDlg* dlg;
		char confirmtext[1024];
		ZeroMemory( confirmtext, 1024 );
		sprintf_s( confirmtext, 1024, 
			"フレーム番号:%d の\n以下のボーンのモーションポイントを\nコピーしました。",
			m_current );
		dlg = new CFrameOpeDlg( 0, confirmtext, fcdlg.m_cplist, m_thandler );
		if( !dlg ){
			_ASSERT( 0 );
			return 1;
		}
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		dlg->DoModal();
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		delete dlg;
***/
	}else{
		char tempchar[1024];
		ZeroMemory( tempchar, 1024 );
		sprintf_s( tempchar, 1024, "フレーム %d には、モーションポイントはありません。",
			m_current );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, tempchar, "コピーできません。", MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	}

	return 0;
}



LRESULT CMotParamDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;
	
	int serino;
	int shdnum = m_shandler->s2shd_leng;

	CFrameCopyDlg fcdlg( m_thandler, m_shandler, m_hWnd, m_sl_mp_rangemax );
	int ret;

	ret = fcdlg.CreateLists();
	if( ret ){
		DbgOut( "MotParamDlg : OnFrameCopy : fcdlg CreateLists error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int dlgret;
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	dlgret = (int)fcdlg.DoModal( m_hWnd );
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( dlgret != IDOK ){
		return 0;
	}

	InitCEBuff();//!!!!!!!!



	int cpstart, cpend;
	if( fcdlg.m_ismulti ){
		cpstart = fcdlg.m_startframe;
		cpend = fcdlg.m_endframe;
		if( (cpend - cpstart + 1) > CPFRAMEMAX ){
			cpend = cpstart + CPFRAMEMAX - 1;
		}
	}else{
		cpstart = m_current;
		cpend = m_current;
	}


	COPYELEM* buffptr;
	int cpnum = 0;
	int invalidnum = 0;
	int cpframe;
	for( cpframe = cpstart; cpframe <= cpend; cpframe++ ){
		for( serino = 0; serino < shdnum; serino++ ){

			buffptr = m_CEBuff + shdnum * (cpframe - cpstart) + serino;

			if( *(fcdlg.m_cplist + serino) == 1 ){

				CMotionCtrl* mcptr = (*m_mhandler)( serino );
				CTreeElem2* teptr = (*m_thandler)( serino );
				_ASSERT( mcptr );
				_ASSERT( teptr );
				CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, cpframe );
				if( isexist ){
					buffptr->mp.m_frameno = cpframe;
					buffptr->mp.m_mvx = isexist->m_mvx;
					buffptr->mp.m_mvy = isexist->m_mvy;
					buffptr->mp.m_mvz = isexist->m_mvz;
					buffptr->mp.m_q = isexist->m_q;
					//buffptr->mp.dispswitch = isexist->dispswitch;
					buffptr->mp.dispswitch = 0xFFFFFFFF;

					buffptr->mp.interp = isexist->interp;

					buffptr->mp.m_scalex = isexist->m_scalex;
					buffptr->mp.m_scaley = isexist->m_scaley;
					buffptr->mp.m_scalez = isexist->m_scalez;

					buffptr->mp.m_eul = isexist->m_eul;

					buffptr->mp.m_userint1 = isexist->m_userint1;
					int namelen;
					namelen = (int)strlen( teptr->name );
					if( namelen >= 256 ){
						invalidnum++;
						buffptr->mp.m_frameno = -1;
					}else{
						strcpy_s( buffptr->name, 256, teptr->name );
					}

					cpnum++;
				}else{

					//対称ペーストのためにモーションポイントを計算して格納する
					CShdElem* selem;
					selem = (*m_shandler)( serino );
					int hasmpflag;
					ret = mcptr->CalcMotionPointOnFrame( selem, &buffptr->mp, m_mot_cookie, m_current, &hasmpflag, m_shandler, m_mhandler );
					if( ret ){
						DbgOut( "motparamdlg : OnFrameCopy : mc CalcMotionPointOnFrame error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					int namelen;
					namelen = (int)strlen( teptr->name );
					if( namelen >= 256 ){
						invalidnum++;
						buffptr->mp.m_frameno = -1;
					}else{
						strcpy_s( buffptr->name, 256, teptr->name );
					}

					buffptr->mp.m_frameno = -1;//!!!!!!!!!!!!!!!!!!!!!!!!
				}
			}else{
				buffptr->mp.m_frameno = -1;
			}

		}
	}

	if( cpnum != 0 ){
		
		//クリップボードにコピーする。
		HGLOBAL hGlobal;
		ret = CopyToClipBoardData( m_shandler->s2shd_leng, cpend - cpstart + 1, m_CEBuff, &hGlobal );
		if( ret ){
			DbgOut( "motparamdlg : OnFrameCopy : CopyToClipBoardData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( ::OpenClipboard( m_hWnd ) == 0 ){
			DbgOut( "motparamdlg : OnFrameCopy : OpenClipboard error !!!\n" );
			_ASSERT( 0 );
			GlobalFree(hGlobal);
			return 1;
		}
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
		::CloseClipboard();

	}else{
		char tempchar[1024];
		ZeroMemory( tempchar, 1024 );
		sprintf_s( tempchar, 1024, "フレーム %d には、モーションポイントはありません。",
			m_current );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, tempchar, "コピーできません。", MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	}
	
	return 0;
}

LRESULT CMotParamDlg::OnFrameCopyAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;
	
	int serino;
	int shdnum = m_shandler->s2shd_leng;

	CFrameCopyDlg fcdlg( m_thandler, m_shandler, m_hWnd, m_sl_mp_rangemax );
	int ret;

	ret = fcdlg.CreateLists();
	if( ret ){
		DbgOut( "MotParamDlg : OnFrameCopyAll : fcdlg CreateLists error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//	int dlgret;
//	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
//	dlgret = (int)fcdlg.DoModal( m_hWnd );
//	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

//	if( dlgret != IDOK ){
//		return 0;
//	}

	InitCEBuff();//!!!!!!!!

	COPYELEM* buffptr;
	int cpnum = 0;
	int invalidnum = 0;
	for( serino = 0; serino < shdnum; serino++ ){

		buffptr = m_CEBuff + serino;

		CMotionCtrl* mcptr = (*m_mhandler)( serino );
		CTreeElem2* teptr = (*m_thandler)( serino );
		_ASSERT( mcptr );
		_ASSERT( teptr );
		CShdElem* selem;
		selem = (*m_shandler)( serino );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			*(fcdlg.m_cplist + serino) = 1;

			int hasmpflag;
			ret = mcptr->CalcMotionPointOnFrame( selem, &buffptr->mp, m_mot_cookie, m_current, &hasmpflag, m_shandler, m_mhandler );
			if( ret ){
				DbgOut( "motparamdlg : OnFrameCopyAll : mc CalcMotionPointOnFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			buffptr->mp.m_frameno = m_current;

			int namelen;
			namelen = (int)strlen( teptr->name );
			if( namelen >= 256 ){
				invalidnum++;
				buffptr->mp.m_frameno = -1;
			}else{
				strcpy_s( buffptr->name, 256, teptr->name );
			}

			cpnum++;
		}else{
			*(fcdlg.m_cplist + serino) = 0;
			buffptr->mp.m_frameno = -1;

		}
	}

	if( cpnum != 0 ){
		
		//クリップボードにコピーする。
		HGLOBAL hGlobal;
		ret = CopyToClipBoardData( m_shandler->s2shd_leng, 1, m_CEBuff, &hGlobal );
		if( ret ){
			DbgOut( "motparamdlg : OnFrameCopyAll : CopyToClipBoardData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( ::OpenClipboard( m_hWnd ) == 0 ){
			DbgOut( "motparamdlg : OnFrameCopyAll : OpenClipboard error !!!\n" );
			_ASSERT( 0 );
			GlobalFree(hGlobal);
			return 1;
		}
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
		::CloseClipboard();

/***
		//確認ダイアログを出す。
		CFrameOpeDlg* dlg;
		char confirmtext[1024];
		ZeroMemory( confirmtext, 1024 );
		sprintf_s( confirmtext, 1024, 
			"フレーム番号:%d の\n以下のボーンのモーションポイントを\nコピーしました。",
			m_current );
		dlg = new CFrameOpeDlg( 0, confirmtext, fcdlg.m_cplist, m_thandler );
		if( !dlg ){
			_ASSERT( 0 );
			return 1;
		}
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		dlg->DoModal();
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		delete dlg;
***/
	}else{
		char tempchar[1024];
		ZeroMemory( tempchar, 1024 );
		sprintf_s( tempchar, 1024, "フレーム %d には、モーションポイントはありません。",
			m_current );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, tempchar, "コピーできません。", MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	}
	
	return 0;
}

LRESULT CMotParamDlg::OnFrameCopyAri(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;
	
	int serino;
	int shdnum = m_shandler->s2shd_leng;

	CFrameCopyDlg fcdlg( m_thandler, m_shandler, m_hWnd, m_sl_mp_rangemax );
	int ret;

	ret = fcdlg.CreateLists();
	if( ret ){
		DbgOut( "MotParamDlg : OnFrameCopyAll : fcdlg CreateLists error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//	int dlgret;
//	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
//	dlgret = (int)fcdlg.DoModal( m_hWnd );
//	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

//	if( dlgret != IDOK ){
//		return 0;
//	}

	InitCEBuff();//!!!!!!!!

	COPYELEM* buffptr;
	int cpnum = 0;
	int invalidnum = 0;
	for( serino = 0; serino < shdnum; serino++ ){

		buffptr = m_CEBuff + serino;

		CMotionCtrl* mcptr = (*m_mhandler)( serino );
		CTreeElem2* teptr = (*m_thandler)( serino );
		_ASSERT( mcptr );
		_ASSERT( teptr );
		CShdElem* selem;
		selem = (*m_shandler)( serino );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){


			CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
			if( isexist ){
				*(fcdlg.m_cplist + serino) = 1;

				buffptr->mp.m_frameno = m_current;
				buffptr->mp.m_mvx = isexist->m_mvx;
				buffptr->mp.m_mvy = isexist->m_mvy;
				buffptr->mp.m_mvz = isexist->m_mvz;
				buffptr->mp.m_q = isexist->m_q;
				//buffptr->mp.dispswitch = isexist->dispswitch;
				buffptr->mp.dispswitch = 0xFFFFFFFF;

				buffptr->mp.interp = isexist->interp;

				buffptr->mp.m_scalex = isexist->m_scalex;
				buffptr->mp.m_scaley = isexist->m_scaley;
				buffptr->mp.m_scalez = isexist->m_scalez;

				buffptr->mp.m_eul = isexist->m_eul;

				buffptr->mp.m_userint1 = isexist->m_userint1;

				int namelen;
				namelen = (int)strlen( teptr->name );
				if( namelen >= 256 ){
					invalidnum++;
					buffptr->mp.m_frameno = -1;
				}else{
					strcpy_s( buffptr->name, 256, teptr->name );
					cpnum++;
				}
			}else{

				//対称ペーストのためにモーションポイントを計算して格納する
				int hasmpflag;
				ret = mcptr->CalcMotionPointOnFrame( selem, &buffptr->mp, m_mot_cookie, m_current, &hasmpflag, m_shandler, m_mhandler );
				if( ret ){
					DbgOut( "motparamdlg : OnFrameCopy : mc CalcMotionPointOnFrame error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				int namelen;
				namelen = (int)strlen( teptr->name );
				if( namelen >= 256 ){
					invalidnum++;
					buffptr->mp.m_frameno = -1;
				}else{
					strcpy_s( buffptr->name, 256, teptr->name );
				}

				buffptr->mp.m_frameno = -1;//!!!!!!!!!!!!!!!!!!!!!!!!
				*(fcdlg.m_cplist + serino) = 0;//!!!!!!!!!!!!!!!!!! 以下で確認ダイアログを出すときに必要
			}
		}else{
			*(fcdlg.m_cplist + serino) = 0;
			buffptr->mp.m_frameno = -1;
		}
	}

	if( cpnum != 0 ){
		
		//クリップボードにコピーする。
		HGLOBAL hGlobal;
		ret = CopyToClipBoardData( m_shandler->s2shd_leng, 1, m_CEBuff, &hGlobal );
		if( ret ){
			DbgOut( "motparamdlg : OnFrameCopyAll : CopyToClipBoardData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( ::OpenClipboard( m_hWnd ) == 0 ){
			DbgOut( "motparamdlg : OnFrameCopyAll : OpenClipboard error !!!\n" );
			_ASSERT( 0 );
			GlobalFree(hGlobal);
			return 1;
		}
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
		::CloseClipboard();

/***
		//確認ダイアログを出す。
		CFrameOpeDlg* dlg;
		char confirmtext[1024];
		ZeroMemory( confirmtext, 1024 );
		sprintf_s( confirmtext, 1024, 
			"フレーム番号:%d の\n以下のボーンのモーションポイントを\nコピーしました。",
			m_current );
		dlg = new CFrameOpeDlg( 0, confirmtext, fcdlg.m_cplist, m_thandler );
		if( !dlg ){
			_ASSERT( 0 );
			return 1;
		}
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		dlg->DoModal();
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		delete dlg;
***/
	}else{
		char tempchar[1024];
		ZeroMemory( tempchar, 1024 );
		sprintf_s( tempchar, 1024, "フレーム %d には、モーションポイントはありません。",
			m_current );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, tempchar, "コピーできません。", MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	}
	
	return 0;
}

LRESULT CMotParamDlg::OnFrameCopyOne(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;
	
	int serino;
	int shdnum = m_shandler->s2shd_leng;

	int ret;

	InitCEBuff();//!!!!!!!!


	COPYELEM* buffptr;
	int cpnum = 0;
	int invalidnum = 0;
	char* cpname = 0;
	for( serino = 0; serino < shdnum; serino++ ){

		buffptr = m_CEBuff + serino;

		if( serino == m_selectedno ){

			CMotionCtrl* mcptr = (*m_mhandler)( serino );
			CTreeElem2* teptr = (*m_thandler)( serino );
			_ASSERT( mcptr );
			_ASSERT( teptr );
			CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
			if( isexist ){
				buffptr->mp.m_frameno = m_current;
				buffptr->mp.m_mvx = isexist->m_mvx;
				buffptr->mp.m_mvy = isexist->m_mvy;
				buffptr->mp.m_mvz = isexist->m_mvz;
				buffptr->mp.m_q = isexist->m_q;
				//buffptr->mp.dispswitch = isexist->dispswitch;
				buffptr->mp.dispswitch = 0xFFFFFFFF;

				buffptr->mp.interp = isexist->interp;

				buffptr->mp.m_scalex = isexist->m_scalex;
				buffptr->mp.m_scaley = isexist->m_scaley;
				buffptr->mp.m_scalez = isexist->m_scalez;

				buffptr->mp.m_eul = isexist->m_eul;

				buffptr->mp.m_userint1 = isexist->m_userint1;
				int namelen;
				namelen = (int)strlen( teptr->name );
				if( namelen >= 256 ){
					invalidnum++;
					buffptr->mp.m_frameno = -1;
				}else{
					strcpy_s( buffptr->name, 256, teptr->name );
					cpname = teptr->name;
				}

				cpnum++;
			}else{
				buffptr->mp.m_frameno = -1;
			}
		}else{
			buffptr->mp.m_frameno = -1;
		}

	}

	if( cpnum != 0 ){
		
		//クリップボードにコピーする。
		HGLOBAL hGlobal;
		ret = CopyToClipBoardData( m_shandler->s2shd_leng, 1, m_CEBuff, &hGlobal );
		if( ret ){
			DbgOut( "motparamdlg : OnFrameCopy : CopyToClipBoardData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( ::OpenClipboard( m_hWnd ) == 0 ){
			DbgOut( "motparamdlg : OnFrameCopy : OpenClipboard error !!!\n" );
			_ASSERT( 0 );
			GlobalFree(hGlobal);
			return 1;
		}
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
		::CloseClipboard();


		//確認ダイアログを出す。
		char confirmtext[1024];
		ZeroMemory( confirmtext, 1024 );
		sprintf_s( confirmtext, 1024, 
			"%sのフレーム番号:%d の\nモーションポイントをコピーしました。",
			cpname, m_current );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, confirmtext, "確認", MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	}else{
		char tempchar[1024];
		ZeroMemory( tempchar, 1024 );
		sprintf_s( tempchar, 1024, "%sのフレーム %d には、\nモーションポイントはありません。",
			cpname, m_current );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, tempchar, "コピーできません。", MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	}
	
	return 0;
}

LRESULT CMotParamDlg::OnFramePasteSym(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( !m_CEBuff )
		return 0;

	int ret;

	//クリップボードからCOPYELEMにペーストする。
    HGLOBAL hGlobal;
    if( !IsClipboardFormatAvailable( CF_TEXT ) ) {
		DbgOut( "motparamdlg : OnFramePaste : IsClipboardFormatAvailable error !!!\n" );
		::MessageBox( m_hWnd, "クリップボードを読めません", "ペーストエラー", MB_OK);
        return 1;
    }

	::OpenClipboard( m_hWnd );
    hGlobal = (HGLOBAL)GetClipboardData( CF_TEXT );
    if( hGlobal == NULL ){
		DbgOut( "motparamdlg : OnFramePaste : GetClipboardData NULL warning !!!\n" );
		_ASSERT( 0 );
		::MessageBox( m_hWnd, "クリップボードにコピーデータがありませんでした。", "ペースト失敗", MB_OK );
		::CloseClipboard();
        return 1;
    }

	int bufshdnum = 0;
	int bufframenum = 0;
	ret = PasteFromClipBoardData( m_CEBuff, &bufshdnum, &bufframenum, hGlobal );
	if( ret ){
		DbgOut( "motparamdlg : OnFramePaste : PastaFromClipBoardData error !!!\n" );
		_ASSERT( 0 );
		::CloseClipboard();
		return 1;
	}

	::CloseClipboard();

	int cpstartframe, cpendframe;
	int onkeyflag = 0;

	if( bufframenum == 1 ){
		CFramePasteDlg dlg( 1, m_current );
		if( hWndCtl != 0 ){
			int dlgret;
			::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
			dlgret = (int)dlg.DoModal();
			::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
			if( dlgret != IDOK ){
				//キャンセル
				return 0;
			}
		}

		onkeyflag = dlg.m_select;
		cpstartframe = dlg.m_start;
		cpendframe = dlg.m_end;

		if( dlg.m_select == 0 ){
			cpstartframe = m_current;
			cpendframe = m_current;
		}else{
			if( cpstartframe < 0 )
				cpstartframe = 0;
			if( cpstartframe > m_sl_mp_rangemax )
				cpstartframe = m_sl_mp_rangemax;

			if( cpendframe < 0 )
				cpendframe = 0;
			if( cpendframe > m_sl_mp_rangemax )
				cpendframe = m_sl_mp_rangemax;
			if( cpendframe < cpstartframe )
				cpendframe = cpstartframe;
		}
	}else{
		cpstartframe = m_current;
		cpendframe = m_current + bufframenum - 1;
		onkeyflag = 0;
	}	

	//確認ダイアログを出す。
	int shdnum = m_shandler->s2shd_leng;
	int* pastelist;
	pastelist = (int*)malloc( sizeof( int ) * shdnum );
	if( !pastelist ){
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( pastelist, sizeof( int ) * shdnum );

	//ペーストを行う。
	if( bufframenum == 1 ){
		ZeroMemory( pastelist, sizeof( int ) * shdnum );

		int serino;
		COPYELEM* curce;
		int pastenum = 0;
		for( serino = 1; serino < shdnum; serino++ ){
			if( serino < bufshdnum ){
				curce = m_CEBuff + serino;
				if( curce->mp.m_frameno >= 0 ){
					int cpsymflag = 0;

					//対称ペーストの場合、対称のボーンをペーストする。
					int symmno = 0;
					int symmtype = SYMMTYPE_NONE;
					ret = SOGetSymmNo( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
					if( ret ){
						DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					if( symmno <= 0 ){
						ret = SOGetSymmNo2( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
						if( ret ){
							DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo2 error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
					}
					if( symmno > 0 ){
						*(pastelist + symmno) = 1;
						pastenum++;
						cpsymflag = 1;
					}

					if( cpsymflag == 0 ){
						*(pastelist + serino) = 1;	
						pastenum++;
					}
				}
			}else{
				*(pastelist + serino) = 0;	
			}
		}

		for( serino = 0; serino < m_shandler->s2shd_leng; serino++ ){
			//curmp = m_MPBuff + serino;

			if( *(pastelist + serino) == 1 ){
				
				CMotionPoint2 cpmp;

				// 軸対称の姿勢情報を取得する。
				ret = GetPasteMP( m_CEBuff + bufshdnum * 0, pastelist, serino, &cpmp );
				if( ret ){
					DbgOut( "MotParamDlg : OnFramePaste : GetPasteMP error !!!\n" );
					_ASSERT( 0 );
					//free( pastelist );
					return 1;
				}
				cpmp.m_spp = 0;

				if( onkeyflag == 0 ){
					ret = PasteOnFrameOne( serino, &cpmp, m_mot_cookie, m_current );
					if( ret ){
						DbgOut( "motparamdlg : PasteOnFrame : PasteOnFrameOne error !!!\n" );
						_ASSERT( 0 );
						free( pastelist );
						return 1;
					}
				}else{


					int* framearray;
					int arrayleng;
					int keyframenum;

					arrayleng = m_sl_mp_rangemax + 1;
					framearray = (int*)malloc( sizeof( int ) * arrayleng );
					if( !framearray ){
						DbgOut( "motparamdlg : OnFramePaste : framearray alloc error !!!\n" );
						_ASSERT( 0 );
						free( pastelist );
						return 1;
					}

					ret = m_mhandler->HuGetKeyframeNo( m_mot_cookie, serino, framearray, arrayleng, &keyframenum );
					if( ret ){
						DbgOut( "motparamdlg : OnFramePaste : HuGetKeyframeNo error !!!\n" );
						_ASSERT( 0 );
						free( pastelist );
						free( framearray );
						return 1;
					}


					int keyno;
					int curkeyframe;
					for( keyno = 0; keyno < keyframenum; keyno++ ){
						curkeyframe = *( framearray + keyno );

						if( (curkeyframe >= cpstartframe) && (curkeyframe <= cpendframe) ){

							ret = PasteOnFrameOne( serino, &cpmp, m_mot_cookie, curkeyframe );
							if( ret ){
								DbgOut( "motparamdlg : PasteOnFrame : PasteOnFrameOne error !!!\n" );
								_ASSERT( 0 );
								free( pastelist );
								free( framearray );
								return 1;
							}
						}
					}

					free( framearray );
				}
			}
		}
	}else{

		int fno;
		int fcnt;
		for( fcnt = 0; fcnt < bufframenum; fcnt++ ){
			fno = m_current + fcnt;

			ZeroMemory( pastelist, sizeof( int ) * shdnum );

			int serino;
			COPYELEM* curce;
			int pastenum = 0;
			for( serino = 1; serino < shdnum; serino++ ){
				if( serino < bufshdnum ){
					curce = m_CEBuff + bufshdnum * fcnt + serino;
					if( curce->mp.m_frameno >= 0 ){
						int cpsymflag = 0;

						//対称ペーストの場合、対称のボーンをペーストする。
						int symmno = 0;
						int symmtype = SYMMTYPE_NONE;
						ret = SOGetSymmNo( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
						if( ret ){
							DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
						if( symmno <= 0 ){
							ret = SOGetSymmNo2( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
							if( ret ){
								DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo2 error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}
						if( symmno > 0 ){
							*(pastelist + symmno) = 1;
							pastenum++;
							cpsymflag = 1;
						}

						if( cpsymflag == 0 ){
							*(pastelist + serino) = 1;	
							pastenum++;
						}
					}
				}
			}

			for( serino = 0; serino < m_shandler->s2shd_leng; serino++ ){
				//curmp = m_MPBuff + serino;

				if( *(pastelist + serino) == 1 ){
				
					CMotionPoint2 cpmp;

					// 軸対称の姿勢情報を取得する。
					ret = GetPasteMP( m_CEBuff + bufshdnum * fcnt, pastelist, serino, &cpmp );
					if( ret ){
						DbgOut( "MotParamDlg : OnFramePaste : GetPasteMP error !!!\n" );
						_ASSERT( 0 );
						//free( pastelist );
						return 1;
					}
					cpmp.m_spp = 0;

					if( (fno >= 0) && (fno <= m_sl_mp_rangemax) ){

						ret = PasteOnFrameOne( serino, &cpmp, m_mot_cookie, fno );
						if( ret ){
							DbgOut( "motparamdlg : PasteOnFrame : PasteOnFrameOne error !!!\n" );
							_ASSERT( 0 );
							free( pastelist );
							return 1;
						}
					}

				}
			}
		}


	}


	free( pastelist );
	
	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}

	UpdateAllMatrix( 1 );
	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );



	return 0;
}



LRESULT CMotParamDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( !m_CEBuff )
		return 0;

	int ret;

	//クリップボードからCOPYELEMにペーストする。
    HGLOBAL hGlobal;
    if( !IsClipboardFormatAvailable( CF_TEXT ) ) {
		DbgOut( "motparamdlg : OnFramePaste : IsClipboardFormatAvailable error !!!\n" );
		::MessageBox( m_hWnd, "クリップボードを読めません", "ペーストエラー", MB_OK);
        return 1;
    }

	::OpenClipboard( m_hWnd );
    hGlobal = (HGLOBAL)GetClipboardData( CF_TEXT );
    if( hGlobal == NULL ){
		DbgOut( "motparamdlg : OnFramePaste : GetClipboardData NULL warning !!!\n" );
		_ASSERT( 0 );
		::MessageBox( m_hWnd, "クリップボードにコピーデータがありませんでした。", "ペースト失敗", MB_OK );
		::CloseClipboard();
        return 1;
    }


	int bufshdnum = 0;
	int bufframenum = 0;
	ret = PasteFromClipBoardData( m_CEBuff, &bufshdnum, &bufframenum, hGlobal );
	if( ret ){
		DbgOut( "motparamdlg : OnFramePaste : PastaFromClipBoardData error !!!\n" );
		_ASSERT( 0 );
		::CloseClipboard();
		return 1;
	}

	::CloseClipboard();

	int cpstartframe, cpendframe;
	int onkeyflag = 0;

	if( bufframenum == 1 ){
		CFramePasteDlg dlg( 1, m_current );
		int dlgret;
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		dlgret = (int)dlg.DoModal();
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( dlgret != IDOK ){
			//キャンセル
			return 0;
		}

		onkeyflag = dlg.m_select;
		cpstartframe = dlg.m_start;
		cpendframe = dlg.m_end;

		if( dlg.m_select == 0 ){
			cpstartframe = m_current;
			cpendframe = m_current;
		}else{
			if( cpstartframe < 0 )
				cpstartframe = 0;
			if( cpstartframe > m_sl_mp_rangemax )
				cpstartframe = m_sl_mp_rangemax;

			if( cpendframe < 0 )
				cpendframe = 0;
			if( cpendframe > m_sl_mp_rangemax )
				cpendframe = m_sl_mp_rangemax;
			if( cpendframe < cpstartframe )
				cpendframe = cpstartframe;
		}
	}else{
		cpstartframe = m_current;
		cpendframe = m_current + bufframenum - 1;
		onkeyflag = 0;
	}


	//ペーストを行う。
	if( bufframenum == 1 ){
		int serino;

		for( serino = 0; serino < bufshdnum; serino++ ){
			COPYELEM* curce = m_CEBuff + serino;
			if( (curce->mp.m_frameno >= 0) && (serino > 0) && (serino < m_shandler->s2shd_leng) ){

				CMotionPoint2 cpmp;
				// そのままの値を取得する。
				cpmp = ( m_CEBuff + serino )->mp;	
				cpmp.m_spp = 0;

				if( onkeyflag == 0 ){
					ret = PasteOnFrameOne( serino, &cpmp, m_mot_cookie, m_current );
					if( ret ){
						DbgOut( "motparamdlg : PasteOnFrame : PasteOnFrameOne error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}else{

					int* framearray;
					int arrayleng;
					int keyframenum;

					arrayleng = m_sl_mp_rangemax + 1;
					framearray = (int*)malloc( sizeof( int ) * arrayleng );
					if( !framearray ){
						DbgOut( "motparamdlg : OnFramePaste : framearray alloc error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					ret = m_mhandler->HuGetKeyframeNo( m_mot_cookie, serino, framearray, arrayleng, &keyframenum );
					if( ret ){
						DbgOut( "motparamdlg : OnFramePaste : HuGetKeyframeNo error !!!\n" );
						_ASSERT( 0 );
						free( framearray );
						return 1;
					}


					int keyno;
					int curkeyframe;
					for( keyno = 0; keyno < keyframenum; keyno++ ){
						curkeyframe = *( framearray + keyno );

						if( (curkeyframe >= cpstartframe) && (curkeyframe <= cpendframe) ){

							ret = PasteOnFrameOne( serino, &cpmp, m_mot_cookie, curkeyframe );
							if( ret ){
								DbgOut( "motparamdlg : PasteOnFrame : PasteOnFrameOne error !!!\n" );
								_ASSERT( 0 );
								free( framearray );
								return 1;
							}
						}
					}
					free( framearray );
				}
			}
		}
	}else{
		int serino;
		int frameno;
		for( frameno = 0; frameno < bufframenum; frameno++ ){
			for( serino = 0; serino < bufshdnum; serino++ ){
				COPYELEM* curce = m_CEBuff + bufshdnum * frameno + serino;
				if( (curce->mp.m_frameno >= 0) && (serino > 0) && (serino < m_shandler->s2shd_leng) ){

					CMotionPoint2 cpmp;
					// そのままの値を取得する。
					cpmp = ( m_CEBuff + bufshdnum * frameno + serino )->mp;	
					cpmp.m_spp = 0;

					int setframe = m_current + frameno;
					cpmp.m_frameno = setframe;
					if( (setframe >= 0) && (setframe <= m_sl_mp_rangemax) ){

						ret = PasteOnFrameOne( serino, &cpmp, m_mot_cookie, setframe );
						if( ret ){
							DbgOut( "motparamdlg : PasteOnFrame : PasteOnFrameOne error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
					}
				}
			}

		}
	}


	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}


	UpdateAllMatrix( 1 );
	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );
	return 0;
}

int CMotParamDlg::PasteOnFrame( int pastetype, int* pastelist, int pasteframe )
{
	int ret;
	int serino;
	for( serino = 0; serino < m_shandler->s2shd_leng; serino++ ){
		//curmp = m_MPBuff + serino;

		if( *(pastelist + serino) == 1 ){
			
			CMotionPoint2 cpmp;

			if( pastetype == 0 ){
				// 軸対称の姿勢情報を取得する。
				ret = GetPasteMP( m_CEBuff, pastelist, serino, &cpmp );
				if( ret ){
					DbgOut( "MotParamDlg : OnFramePaste : GetPasteMP error !!!\n" );
					_ASSERT( 0 );
					//free( pastelist );
					return 1;
				}
			}else{
				// そのままの値を取得する。
				cpmp = ( m_CEBuff + serino )->mp;					
			}
			cpmp.m_spp = 0;

			ret = PasteOnFrameOne( serino, &cpmp, m_mot_cookie, pasteframe );
			if( ret ){
				DbgOut( "motparamdlg : PasteOnFrame : PasteOnFrameOne error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

		}
	}
	return 0;

}

int CMotParamDlg::PasteOnFrameOne( int serino, CMotionPoint2* cpmp, int motid, int pasteframe )
{
	int ret;

	CMotionCtrl* mcptr = (*m_mhandler)( serino );
	_ASSERT( mcptr );

	CShdElem* selem = (*m_shandler)( serino );
	_ASSERT( selem );

	CMotionPoint2* isexist = mcptr->IsExistMotionPoint( motid, pasteframe );
	if( isexist ){
		//isexistにcurmpの値をセット。
		isexist->m_mvx = cpmp->m_mvx;
		isexist->m_mvy = cpmp->m_mvy;
		isexist->m_mvz = cpmp->m_mvz;
		isexist->m_q = cpmp->m_q;
		//isexist->dispswitch = cpmp.dispswitch;
		isexist->dispswitch = 0xFFFFFFFF;
		isexist->interp = cpmp->interp;
			
		isexist->m_scalex = cpmp->m_scalex;
		isexist->m_scaley = cpmp->m_scaley;
		isexist->m_scalez = cpmp->m_scalez;
		isexist->m_userint1 = cpmp->m_userint1;

		//if( ptdlg->m_pastetype == 0 ){
			//軸対称
			if( isexist->prev ){
				D3DXVECTOR3 neweul;
				ret = qToEuler( selem, &isexist->m_q, &neweul );
				_ASSERT( !ret );
				ret = modifyEuler( &neweul, &isexist->prev->m_eul );
				_ASSERT( !ret );
				isexist->m_eul = neweul;
				//cpmp.m_eul = neweul;
			}else{
				D3DXVECTOR3 neweul;
				ret = qToEuler( selem, &isexist->m_q, &neweul );
				_ASSERT( !ret );
				D3DXVECTOR3 befeul( 0.0f, 0.0f, 0.0f );
				ret = modifyEuler( &neweul, &befeul );
				_ASSERT( !ret );
				isexist->m_eul = neweul;
				//cpmp.m_eul = neweul;
			}
		//}else{
			//そのまま
		//	isexist->m_eul = cpmp.m_eul;
		//}

	}else{
		//MotionPointを新しく作り、curmpの値をセット。
		CMotionPoint2* newmp = mcptr->AddMotionPoint( motid, pasteframe,
			cpmp->m_q,
			//cpmp.m_mvx, cpmp.m_mvy, cpmp.m_mvz, cpmp.dispswitch, cpmp.interp, cpmp.m_scalex, cpmp.m_scaley, cpmp.m_scalez, cpmp.m_userint1 );
			cpmp->m_mvx, cpmp->m_mvy, cpmp->m_mvz, m_dispswitch, cpmp->interp, cpmp->m_scalex, cpmp->m_scaley, cpmp->m_scalez, cpmp->m_userint1 );
		if( !newmp ){
			_ASSERT( 0 );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnFramePaste : AddMotionPoint error" );
			//free( pastelist );
			return 1;
		}

		//if( ptdlg->m_pastetype == 0 ){
			//軸対称
			if( newmp->prev ){
				D3DXVECTOR3 neweul;
				ret = qToEuler( selem, &newmp->m_q, &neweul );
				_ASSERT( !ret );
				ret = modifyEuler( &neweul, &newmp->prev->m_eul );
				_ASSERT( !ret );
				newmp->m_eul = neweul;
				//cpmp.m_eul = neweul;
			}else{
				D3DXVECTOR3 neweul;
				ret = qToEuler( selem, &newmp->m_q, &neweul );
				_ASSERT( !ret );
				D3DXVECTOR3 befeul( 0.0f, 0.0f, 0.0f );
				ret = modifyEuler( &neweul, &befeul );
				_ASSERT( !ret );
				newmp->m_eul = neweul;
				//cpmp.m_eul = neweul;
			}
		//}else{
			//そのまま
		//	newmp->m_eul = cpmp.m_eul;
		//}
	}

	if( (serino == m_selectedno) && (pasteframe == m_current) ){
		ParamsFromMotionPoint( cpmp );
	}


	return 0;
}


LRESULT CMotParamDlg::OnFrameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret = 0;
	int mret;
	mret = ::MessageBox( m_hWnd, "編集中のボーンのモーションポイントだけ削除する場合は「はい」を、\n現在のキーフレームの全てのモーションポイントを削除する場合は「いいえ」を\n選択してください。", "選択", MB_YESNOCANCEL );
	if( mret == IDNO ){

		int serino;
		int shdnum = m_shandler->s2shd_leng;
		int* delbuf;
		delbuf = (int*)malloc( sizeof( int ) * shdnum );
		if( !delbuf ){
			_ASSERT( 0 );
			DbgOut( "MotParamDlg : OnFrameDel : malloc error!!!\n" );
			return 1;
		}
		ZeroMemory( delbuf, sizeof( int ) * shdnum );

		for( serino = 1; serino < shdnum; serino++ ){
			CMotionCtrl* mcptr = (*m_mhandler)( serino );
			_ASSERT( mcptr );
			if( mcptr->IsJoint() ){
				CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
				if( isexist ){
					*(delbuf + serino) = 1;
				}else{
					*(delbuf + serino) = 0;
				}
			}
		}

/***
		//確認ダイアログを出す。
		CFrameOpeDlg* dlg;
		char confirmtext[1024];
		ZeroMemory( confirmtext, 1024 );
		sprintf_s( confirmtext, 1024, 
			"フレーム番号:%dの\n以下のボーンのモーションポイントを\n削除します。\nよろしいですか？",
			m_current );

		dlg = new CFrameOpeDlg( 1, confirmtext, delbuf, m_thandler );
		if( !dlg ){
			_ASSERT( 0 );
			return 1;
		}

		int dlgret;
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		dlgret = (int)dlg->DoModal();
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( dlgret != IDOK ){
			free( delbuf );
			delete dlg;	
			return 0;
		}
		delete dlg;
***/
		//deleteする。
		int ret = 0;
		for( serino = 1; serino < shdnum; serino++ ){
			if( *(delbuf + serino) == 1 ){
				CMotionCtrl* mcptr = (*m_mhandler)( serino );
				_ASSERT( mcptr );
				ret = mcptr->DeleteMotionPoint( m_mot_cookie, m_current );
				if( ret ){
					DbgOut( "MotParamDlg : OnFrameDel : DeleteMotionPoint error !!!\n" );
					ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnFrameDel error" );
					_ASSERT( 0 );
					return 1;
				}
			}
		}
		free( delbuf );

	}else if( mret == IDYES ){
		CMotionCtrl* mcptr = (*m_mhandler)( m_selectedno );
		_ASSERT( mcptr );
		CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );

		if( isexist ){
			ret = mcptr->DeleteMotionPoint( m_mot_cookie, m_current );
			if( ret ){
				DbgOut( "MotParamDlg : OnFrameDel : DeleteMotionPoint error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}


	ret += m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	ret += m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : OnFrameDel : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret += InitRotAndMv();
	_ASSERT( !ret );
	ret += ParamsToDlg();
	_ASSERT( !ret );

	if( ret ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnFrameDel MakeRawmat error" );
		return 1;
	}

	SetCurrentMotion();

	Redraw();

	return 0;
}




int CMotParamDlg::InitCEBuff()
{

	int shdnum = m_shandler->s2shd_leng;

	int i;
	for( i = 0; i < (shdnum * CPFRAMEMAX); i++ ){
		(m_CEBuff + i)->name[0] = 0;
		(m_CEBuff + i)->mp.m_frameno = -1;		
	}
	return 0;
}

int CMotParamDlg::LoadBVHFile( char* srcfilename )
{
	m_loadingflag = 1;

	int ret;
	//if( !m_thandler || !m_shandler || !m_mhandler ){
	//	DbgOut( "MotParamDlg : LoadBVHFile : handler NULL !!!\n" );
	//	return 1;
	//}

	CLoadBVHDlg dlg( srcfilename );
	ret = (int)dlg.DoModal();
	if( ret != IDOK ){
		m_loadingflag = 0;
		return 0;
	}


	if( (dlg.m_motonly == 1) && (!m_thandler || !m_shandler || !m_mhandler) ){
		::MessageBox( m_apphwnd, "このオプションでは、形状、ボーンデータが読み込まれていないと、実行できません。", "形状データ無し", MB_OK );
		DbgOut( "motparamdlg : LoadBVHFile : handler NULL warning return !!!\n" );
		m_loadingflag = 0;
		return 0;
	}


	ret = DestroyFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadBVHFile : DestroyFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	CBVHFile bvhfile;
	//LoadBVHFile内部でAddMotionされる。
	//ret = bvhfile.LoadBVHFile( m_thandler, m_shandler, m_mhandler, dlg.m_name, dlg.m_motonly, &m_mot_cookie );
	ret = bvhfile.LoadBVHFile( this, m_apphwnd, m_thandler, m_shandler, m_mhandler, dlg.m_name, dlg.m_mqoname, dlg.m_bonname, dlg.m_mult, dlg.m_motonly, &m_mot_cookie );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			ErrorMes( ret, MSG_NONE, dlg.m_name );

			m_interpolation = INTERPOLATION_SLERP;
			m_mot_cookie = m_mhandler->AddMotion( m_d3dapp->m_za_kind, m_motionname, m_motiontype, m_sl_mp_rangemax + 1, m_interpolation, m_motionjump );
			if( m_mot_cookie < 0 ){
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : AddMotion error" );
				_ASSERT( 0 );
				return 1;
			}

			m_startframe = 0;
			m_endframe = min( m_defframerange, m_sl_mp_rangemax );
			m_current = 0;

			ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

		}else{
			DbgOut( "MotParamDlg : LoadBVHFile : LoadBVHFile error !!!\n" );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : LoadBVHFile error" );
			_ASSERT( 0 );
			return 1;
		}
	}

	m_motionnum = m_mhandler->m_kindnum;
	if( m_motionnum > MAXMOTIONNUM ){
		DbgOut( "motparamdlg : LoadBVHFile : motionnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_motid[ m_motionnum - 1 ].id = m_mot_cookie;
	m_motid[ m_motionnum - 1 ].filename[0] = 0;
	m_motid[ m_motionnum - 1 ].ev0idle = 0;
	m_motid[ m_motionnum - 1 ].commonid = 0;
	m_motid[ m_motionnum - 1 ].forbidnum = 0;
	if( m_motid[ m_motionnum - 1 ].forbidid ){
		free( m_motid[ m_motionnum - 1 ].forbidid );
	}
	m_motid[ m_motionnum - 1 ].forbidid = 0;
	m_motid[ m_motionnum - 1 ].notfu = 0;

/////////////

	CMotionCtrl* mcptr = (*m_mhandler)( 1 );
	if( !mcptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : mcptr error" );
		return 1;
	}
	CMotionInfo* miptr = mcptr->motinfo;
	if( !miptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : miptr error" );
		return 1;
	}

	if( !(miptr->motname) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : motname error" );
		return 1;
	}
	char* nameptr = *(miptr->motname + m_mot_cookie);
	strcpy_s( m_motionname, 256, nameptr );
	if( !(miptr->mottype) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : mottype error" );
		return 1;
	}
	m_motiontype = *(miptr->mottype + m_mot_cookie);
	if( !(miptr->motnum) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : motnum error" );
		return 1;
	}
	m_sl_mp_rangemax = *(miptr->motnum + m_mot_cookie) - 1; //(framenum - 1)
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;


	m_motid[ m_motionnum - 1 ].startframe = m_startframe;
	m_motid[ m_motionnum - 1 ].endframe = m_endframe;
	m_motid[ m_motionnum - 1 ].framerange = m_range;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;


	m_motionjump = *(miptr->motjump + m_mot_cookie);


	m_mhandler->GetDefInterp( m_mot_cookie, &m_interpolation );

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

/***
	CMotionCtrl* firstjoint = 0;
	CMotionCtrl* tmpmc;
	CShdElem* tmpse;
	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		tmpse = (*m_shandler)( seri );
		tmpmc = (*m_mhandler)( seri );
		if( tmpse->IsJoint() && (tmpse->type != SHDMORPH) && tmpse->parent && tmpse->parent->IsJoint() &&
			tmpmc->motinfo && tmpmc->motinfo->firstmp && *(tmpmc->motinfo->firstmp + m_mot_cookie ) ){
			
			firstjoint = (*m_mhandler)( tmpse->serialno );
			break;
		}
	}
***/



//////////////////////////

	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : MakeRawmat error" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadBVHFile : RawmatToMotionmat error" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : LoadBVHFile : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
	if( ret ){
		DbgOut( "motparamdlg : LoadBVHFile : mh SetMotionStep error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );


	ret = CreateFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadBVHFile : CreateFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	::SendMessage( m_apphwnd, WM_USER_ANIMMENU, m_motionnum - 1, 0 );


	m_loadingflag = 0;

	BOOL dummy2 = 0;
	OnPaint( 0, 0, 0, dummy2 );

	return 0;


}

int CMotParamDlg::SetMotionStep( int srcmotstep )
{
	int ret;
	if( !m_thandler || !m_shandler || !m_mhandler ){
		DbgOut( "MotParamDlg : SetMotionStep : handler NULL return !!!\n" );
		return 0;//!!!!!!!!!
	}

	int motid;
	for( motid = 0; motid < m_motionnum; motid++ ){
		ret = m_mhandler->SetMotionStep( motid, g_inidata->opt_motstep );
		if( ret ){
			DbgOut( "motparamdlg : SetMotionStep : mh SetMotionStep error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
/***
	if( m_mot_cookie >= 0 ){
		ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
		if( ret ){
			DbgOut( "motparamdlg :  : mh SetMotionStep error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
***/
	return 0;
}

int CMotParamDlg::LoadMKM( char* srcfilename, CMKMFile* mkmfile, CMKMFile* inimkm, int motno, float tramult )
{

	int ret;
	if( !m_thandler || !m_shandler || !m_mhandler ){
		DbgOut( "MotParamDlg : LoadMKM : handler NULL !!!\n" );
		return 1;
	}
	if( m_selectedno == 0 ){
		//ボーンデータなしのため。
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_apphwnd, 
			"ボーンが一つもないので、モーションをロードできません。", 
			"エラー", MB_OK | MB_APPLMODAL );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	ret = DestroyFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadMKM : DestroyFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = mkmfile->SetMotion( inimkm, m_thandler, m_shandler, m_mhandler, motno, tramult, &m_mot_cookie );
	if( ret ){
		_ASSERT( 0 );
		ErrorMes( ret, MSG_NONE, srcfilename );

		m_interpolation = INTERPOLATION_SLERP;
		m_mot_cookie = m_mhandler->AddMotion( m_d3dapp->m_za_kind, m_motionname, m_motiontype, m_sl_mp_rangemax + 1, m_interpolation, m_motionjump );
		if( m_mot_cookie < 0 ){
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : AddMotion error" );
			_ASSERT( 0 );
			return 1;
		}

		m_startframe = 0;
		m_endframe = min( m_defframerange, m_sl_mp_rangemax );

		ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}

//DbgOut( "check !!! MotParamDlg : LoadMotion : aft LoadQuaFile %s, %d\n", buf, m_mot_cookie );

	m_motionnum = m_mhandler->m_kindnum;
	if( m_motionnum > MAXMOTIONNUM ){
		DbgOut( "motparamdlg : LoadMKM : motionnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_motid[ m_motionnum - 1 ].id = m_mot_cookie;
	char* enptr = 0;
	enptr = strrchr( srcfilename, '\\' );
	if( enptr ){
		strcpy_s( m_motid[ m_motionnum - 1 ].filename, 256, enptr + 1 );
	}else{
		strcpy_s( m_motid[ m_motionnum - 1 ].filename, 256, srcfilename );
	}
	m_motid[ m_motionnum - 1 ].ev0idle = 0;
	m_motid[ m_motionnum - 1 ].commonid = 0;
	m_motid[ m_motionnum - 1 ].forbidnum = 0;
	if( m_motid[ m_motionnum - 1 ].forbidid ){
		free( m_motid[ m_motionnum - 1 ].forbidid );
	}
	m_motid[ m_motionnum - 1 ].forbidid = 0;
	m_motid[ m_motionnum - 1 ].notfu = 0;
	

	CMotionCtrl* mcptr = (*m_mhandler)( 1 );
	if( !mcptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : mcptr error" );
		return 1;
	}
	CMotionInfo* miptr = mcptr->motinfo;
	if( !miptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : miptr error" );
		return 1;
	}

	if( !(miptr->motname) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : motname error" );
		return 1;
	}
	char* nameptr = *(miptr->motname + m_mot_cookie);
	strcpy_s( m_motionname, 256, nameptr );
	if( !(miptr->mottype) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : mottype error" );
		return 1;
	}
	m_motiontype = *(miptr->mottype + m_mot_cookie);
	if( !(miptr->motnum) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : motnum error" );
		return 1;
	}
	m_sl_mp_rangemax = *(miptr->motnum + m_mot_cookie) - 1; //(framenum - 1)
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;

	m_motid[ m_motionnum - 1 ].startframe = m_startframe;
	m_motid[ m_motionnum - 1 ].endframe = m_endframe;
	m_motid[ m_motionnum - 1 ].framerange = m_range;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;


	m_motionjump = *(miptr->motjump + m_mot_cookie);


	m_mhandler->GetDefInterp( m_mot_cookie, &m_interpolation );
	if( (m_interpolation < 0) || (m_interpolation >= INTERPOLATION_MAX) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : interpolation error" );
		return 1;
	}

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

/***
	CMotionCtrl* firstjoint = 0;
	CMotionCtrl* tmpmc;
	CShdElem* tmpse;
	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		tmpse = (*m_shandler)( seri );
		tmpmc = (*m_mhandler)( seri );
		if( tmpse->IsJoint() && (tmpse->type != SHDMORPH) && tmpse->parent && tmpse->parent->IsJoint() &&
			tmpmc->motinfo && tmpmc->motinfo->firstmp && *(tmpmc->motinfo->firstmp + m_mot_cookie ) ){
			
			firstjoint = (*m_mhandler)( tmpse->serialno );
			break;
		}
	}
***/



//////////////////////////


	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : MakeRawmat error" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMKM : RawmatToMotionmat error" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : LoadMKM : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
	if( ret ){
		DbgOut( "motparamdlg : LoadMKM : mh SetMotionStep error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

//	if( namedlg ){
//		strcpy_s( namedlg->m_quaname, 2048, srcfilename );
//	}

	ret = CreateFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadMKM : CreateFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	_ASSERT( m_motionnum >= 2 );
	::SendMessage( m_apphwnd, WM_USER_ANIMMENU, m_motionnum - 1, 0 );
	
	return 0;

}

int CMotParamDlg::LoadEQUMotion( CEQUFile* equfile, int animno, float quamult )
{
	int ret;
	if( !m_thandler || !m_shandler || !m_mhandler ){
		DbgOut( "MotParamDlg : LoadEQUMotion : handler NULL !!!\n" );
		return 1;
	}
	if( m_selectedno == 0 ){
		//ボーンデータなしのため。
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_apphwnd, 
			"ボーンが一つもないので、モーションをロードできません。", 
			"エラー", MB_OK | MB_APPLMODAL );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	ret = DestroyFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadMotion : DestroyFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	//AddEQUMotion内部でAddMotionされる。
	ret = equfile->AddEQUMotion( animno, m_thandler, m_shandler, m_mhandler, &m_mot_cookie, quamult );
	if( ret ){
		DbgOut( "MotParamDlg : LoadEQUMotion : AddEQUMotion error !!!\n" );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : AddEQUMotion error" );
		_ASSERT( 0 );
		return 1;
	}

	m_motionnum = m_mhandler->m_kindnum;
	if( m_motionnum > MAXMOTIONNUM ){
		DbgOut( "motparamdlg : LoadEQUMotion : motionnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_motid[ m_motionnum - 1 ].id = m_mot_cookie;
	strcpy_s( m_motid[ m_motionnum - 1 ].filename, 256, "FromEQUFile" );
	m_motid[ m_motionnum - 1 ].ev0idle = 0;
	m_motid[ m_motionnum - 1 ].commonid = 0;
	m_motid[ m_motionnum - 1 ].forbidnum = 0;
	if( m_motid[ m_motionnum - 1 ].forbidid ){
		free( m_motid[ m_motionnum - 1 ].forbidid );
	}
	m_motid[ m_motionnum - 1 ].forbidid = 0;
	m_motid[ m_motionnum - 1 ].notfu = 0;


	CMotionCtrl* mcptr = (*m_mhandler)( 1 );
	if( !mcptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : mcptr error" );
		return 1;
	}
	CMotionInfo* miptr = mcptr->motinfo;
	if( !miptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : miptr error" );
		return 1;
	}

	if( !(miptr->motname) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : motname error" );
		return 1;
	}
	char* nameptr = *(miptr->motname + m_mot_cookie);
	strcpy_s( m_motionname, 256, nameptr );
	if( !(miptr->mottype) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : mottype error" );
		return 1;
	}
	m_motiontype = *(miptr->mottype + m_mot_cookie);
	if( !(miptr->motnum) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : motnum error" );
		return 1;
	}
	m_sl_mp_rangemax = *(miptr->motnum + m_mot_cookie) - 1; //(framenum - 1)
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;

	m_motid[ m_motionnum - 1 ].startframe = m_startframe;
	m_motid[ m_motionnum - 1 ].endframe = m_endframe;
	m_motid[ m_motionnum - 1 ].framerange = m_range;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;


	m_motionjump = *(miptr->motjump + m_mot_cookie);


	m_mhandler->GetDefInterp( m_mot_cookie, &m_interpolation );
	if( (m_interpolation < 0) || (m_interpolation >= INTERPOLATION_MAX) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : interpolation error" );
		return 1;
	}

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

//////////////////////////


	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : MakeRawmat error" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadEQUMotion : RawmatToMotionmat error" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : LoadEQUMotion : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
	if( ret ){
		DbgOut( "motparamdlg : LoadEQUMotion : mh SetMotionStep error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );


	ret = CreateFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadEQUMotion : CreateFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	::SendMessage( m_apphwnd, WM_USER_ANIMMENU, m_motionnum - 1, 0 );

	return 0;
}

int CMotParamDlg::LoadMotion( CLoadNameDlg* namedlg, char* srcfilename, D3DXVECTOR3 srcquamult, int callmoa )
{
	int ret;
	if( !m_thandler || !m_shandler || !m_mhandler ){
		DbgOut( "MotParamDlg : LoadMotion : handler NULL !!!\n" );
		return 1;
	}
	if( m_selectedno == 0 ){
		//ボーンデータなしのため。
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_apphwnd, 
			"ボーンが一つもないので、モーションをロードできません。", 
			"エラー", MB_OK | MB_APPLMODAL );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	/***
	//OPENFILENAME ofn;
	char buf[_MAX_PATH];
	float quamult = 1.0f;

	if( srcfilename == 0 ){

		CLdQuaDlg dlg;
		ret = (int)dlg.DoModal( m_hWnd );
		if( ret != IDOK )
			return 0;
		strcpy_s( buf, _MAX_PATH, dlg.name );
		quamult = dlg.mult;//!!!!!!

	}else{
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}
	***/

	/***
	ret = m_mhandler->DestroyMotionObj();
	if( ret ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : DestroyMotionObj error" );
		return 1;
	}
	***/

	if( namedlg ){
		*(namedlg->m_quaname) = 0;
	}

	ret = DestroyFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadMotion : DestroyFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	CQuaFile* quafile;
	quafile = new CQuaFile();
	if( !quafile ){
		_ASSERT( 0 );
		ErrorMes( E_OUTOFMEMORY, MSGERR_APPMUSTEXIT, "LoadMotion : quafile alloc error" );
		return 1;
	}

	//LoadQuaFile内部でAddMotionされる。
	ret = quafile->LoadQuaFile( m_thandler, m_shandler, m_mhandler, srcfilename, &m_mot_cookie, srcquamult );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			if( callmoa == 0 ){
				_ASSERT( 0 );
				ErrorMes( ret, MSG_NONE, srcfilename );

				m_interpolation = INTERPOLATION_SLERP;
				m_mot_cookie = m_mhandler->AddMotion( m_d3dapp->m_za_kind, m_motionname, m_motiontype, m_sl_mp_rangemax + 1, m_interpolation, m_motionjump );
				if( m_mot_cookie < 0 ){
					ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : AddMotion error" );
					_ASSERT( 0 );
					delete quafile;
					return 1;
				}

				m_startframe = 0;
				m_endframe = min( m_defframerange, m_sl_mp_rangemax );


				ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
				if( ret ){
					_ASSERT( 0 );
					delete quafile;
					return 1;
				}
			}else{
				char ermes[1024];
				sprintf_s( ermes, 1024, "%sが存在しません。", srcfilename );
				::MessageBox( NULL, ermes, "エラー", MB_OK );
				delete quafile;
				return 1;
			}
		}else{
			delete quafile;
			DbgOut( "MotParamDlg : LoadMotion : LoadQuaFile error !!!\n" );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : LoadQuaFile error" );
			_ASSERT( 0 );
			return 1;
		}
	}

	delete quafile;

//DbgOut( "check !!! MotParamDlg : LoadMotion : aft LoadQuaFile %s, %d\n", buf, m_mot_cookie );

	m_motionnum = m_mhandler->m_kindnum;
	if( m_motionnum > MAXMOTIONNUM ){
		DbgOut( "motparamdlg : LoadMotion : motionnum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	m_motid[ m_motionnum - 1 ].id = m_mot_cookie;
	char* enptr = 0;
	enptr = strrchr( srcfilename, '\\' );
	if( enptr ){
		strcpy_s( m_motid[ m_motionnum - 1 ].filename, 256, enptr + 1 );
	}else{
		strcpy_s( m_motid[ m_motionnum - 1 ].filename, 256, srcfilename );
	}
	m_motid[ m_motionnum - 1 ].ev0idle = 0;
	m_motid[ m_motionnum - 1 ].commonid = 0;
	m_motid[ m_motionnum - 1 ].forbidnum = 0;
	if( m_motid[ m_motionnum - 1 ].forbidid ){
		free( m_motid[ m_motionnum - 1 ].forbidid );
	}
	m_motid[ m_motionnum - 1 ].forbidid = 0;
	m_motid[ m_motionnum - 1 ].notfu = 0;


	CMotionCtrl* mcptr = (*m_mhandler)( 1 );
	if( !mcptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : mcptr error" );
		return 1;
	}
	CMotionInfo* miptr = mcptr->motinfo;
	if( !miptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : miptr error" );
		return 1;
	}

	if( !(miptr->motname) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : motname error" );
		return 1;
	}
	char* nameptr = *(miptr->motname + m_mot_cookie);
	strcpy_s( m_motionname, 256, nameptr );
	if( !(miptr->mottype) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : mottype error" );
		return 1;
	}
	m_motiontype = *(miptr->mottype + m_mot_cookie);
	if( !(miptr->motnum) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : motnum error" );
		return 1;
	}
	m_sl_mp_rangemax = *(miptr->motnum + m_mot_cookie) - 1; //(framenum - 1)
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;

	m_motid[ m_motionnum - 1 ].startframe = m_startframe;
	m_motid[ m_motionnum - 1 ].endframe = m_endframe;
	m_motid[ m_motionnum - 1 ].framerange = m_range;
	//m_motid[ m_motionnum - 1 ].current = m_startframe;


	m_motionjump = *(miptr->motjump + m_mot_cookie);


	m_mhandler->GetDefInterp( m_mot_cookie, &m_interpolation );
	if( (m_interpolation < 0) || (m_interpolation >= INTERPOLATION_MAX) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : interpolation error" );
		return 1;
	}

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

/***
	CMotionCtrl* firstjoint = 0;
	CMotionCtrl* tmpmc;
	CShdElem* tmpse;
	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		tmpse = (*m_shandler)( seri );
		tmpmc = (*m_mhandler)( seri );
		if( tmpse->IsJoint() && (tmpse->type != SHDMORPH) && tmpse->parent && tmpse->parent->IsJoint() &&
			tmpmc->motinfo && tmpmc->motinfo->firstmp && *(tmpmc->motinfo->firstmp + m_mot_cookie ) ){
			
			firstjoint = (*m_mhandler)( tmpse->serialno );
			break;
		}
	}
***/



//////////////////////////


	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : MakeRawmat error" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : RawmatToMotionmat error" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : LoadMotion : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = m_mhandler->SetMotionStep( m_mot_cookie, g_inidata->opt_motstep );
	if( ret ){
		DbgOut( "motparamdlg : LoadMotion : mh SetMotionStep error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	if( namedlg ){
		strcpy_s( namedlg->m_quaname, 2048, srcfilename );
	}

	ret = CreateFUMotion();
	if( ret ){
		DbgOut( "motparamdlg : LoadMotion : CreateFUMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	::SendMessage( m_apphwnd, WM_USER_ANIMMENU, m_motionnum - 1, 0 );
	

	return 0;
}

int CMotParamDlg::ImportMotion( CLoadNameDlg* namedlg )
{
	int ret;
	if( !m_thandler || !m_shandler || !m_mhandler ){
		DbgOut( "MotParamDlg : LoadMotion : handler NULL !!!\n" );
		return 1;
	}
	if( m_selectedno == 0 ){
		//ボーンデータなしのため。
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_apphwnd, 
			"ボーンが一つもないので、モーションをロードできません。", 
			"エラー", MB_OK | MB_APPLMODAL );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	D3DXVECTOR3 mult1 = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
// モーションがない場合は、LoadMotion
	if( m_mhandler->m_kindnum <= 0 ){
		return LoadMotion( namedlg, 0, mult1 );
	}

	int existframenum;
	existframenum = *(m_mhandler->m_motnum + m_mot_cookie);

	if( existframenum < 1 ){
		return LoadMotion( namedlg, 0, mult1 );
	}

//////////////////////

	CImportQuaDlg dlg;
	ret = (int)dlg.DoModal();
	if( ret != IDOK )
		return 0;


	if( namedlg ){
		*(namedlg->m_quaname) = 0;
	}

	CQuaFile* quafile;
	quafile = new CQuaFile();
	if( !quafile ){
		_ASSERT( 0 );
		ErrorMes( E_OUTOFMEMORY, MSGERR_APPMUSTEXIT, "ImportMotion : quafile alloc error" );
		return 1;
	}

	

	ret = quafile->ImportQuaFile( m_thandler, m_shandler, m_mhandler, dlg.m_name, m_mot_cookie, existframenum - 1 + dlg.m_space );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			_ASSERT( 0 );
			ErrorMes( ret, MSG_NONE, dlg.m_name );

		}else{
			delete quafile;
			DbgOut( "MotParamDlg : LoadMotion : ImportQuaFile error !!!\n" );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : ImportQuaFile error" );
			_ASSERT( 0 );
			return 1;
		}
	}

	delete quafile;

//DbgOut( "MotParamDlg : LoadMotion : aft LoadQuaFile %d\n", m_mot_cookie );

	CMotionCtrl* mcptr = (*m_mhandler)( 1 );
	if( !mcptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : mcptr error" );
		return 1;
	}
	CMotionInfo* miptr = mcptr->motinfo;
	if( !miptr ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : miptr error" );
		return 1;
	}

	if( !(miptr->motname) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : motname error" );
		return 1;
	}
	char* nameptr = *(miptr->motname + m_mot_cookie);
	strcpy_s( m_motionname, 256, nameptr );
	if( !(miptr->mottype) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : mottype error" );
		return 1;
	}
	m_motiontype = *(miptr->mottype + m_mot_cookie);
	if( !(miptr->motnum) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : motnum error" );
		return 1;
	}
	m_sl_mp_rangemax = *(miptr->motnum + m_mot_cookie) - 1; //(framenum - 1)
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );




	m_motionjump = *(miptr->motjump + m_mot_cookie);


	m_mhandler->GetDefInterp( m_mot_cookie, &m_interpolation );
	if( (m_interpolation < 0) || (m_interpolation >= INTERPOLATION_MAX) ){
		_ASSERT( 0 );
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : interpolation error" );
		return 1;
	}

	m_current = m_startframe;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

/***
	CMotionCtrl* firstjoint = 0;
	CMotionCtrl* tmpmc;
	CShdElem* tmpse;
	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		tmpse = (*m_shandler)( seri );
		tmpmc = (*m_mhandler)( seri );
		if( tmpse->IsJoint() && (tmpse->type != SHDMORPH) && tmpse->parent && tmpse->parent->IsJoint() &&
			tmpmc->motinfo && tmpmc->motinfo->firstmp && *(tmpmc->motinfo->firstmp + m_mot_cookie ) ){
			
			firstjoint = (*m_mhandler)( tmpse->serialno );
			break;
		}
	}
***/



//////////////////////////


	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : MakeRawmat error" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "LoadMotion : RawmatToMotionmat error" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : LoadMotion : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	if( namedlg ){
		strcpy_s( namedlg->m_quaname, 2048, dlg.m_name );
	}

	return 0;
}

int CMotParamDlg::SaveMotion( CLoadNameDlg* namedlg, char* srcfilename )
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 1;

	if( m_selectedno == 0 ){
		//ボーンデータなしのため。
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_apphwnd, 
			"モーションがないのでセーブできません。", 
			"エラー", MB_OK | MB_APPLMODAL );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	CMotionCtrl* firstjoint = 0;
	ret = m_shandler->GetFirstJoint( &firstjoint, m_mhandler, 1 );
	if( ret ){
		DbgOut( "motparamdlg : SaveMotion : GetFirstJoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( !firstjoint ){
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		MessageBox( "ボーンが１つもないので、出力できません。", "エラー", MB_OK | MB_APPLMODAL );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	::MessageBox( m_apphwnd, "現在、編集中のモーションを１つだけ出力します。\n複数保存したい場合は、モーションメニューで、\n保存したいモーションを選択して書き出し、を繰り返してください。", "確認", MB_OK );


	CWriteQuaDlg dlg( m_sl_mp_rangemax, srcfilename );

	ret = (int)dlg.DoModal();
	if( ret != IDOK )
		return 0;

	if( dlg.m_start < 0 )
		dlg.m_start = 0;
	if( dlg.m_start > m_sl_mp_rangemax )
		dlg.m_start = m_sl_mp_rangemax;

	if( dlg.m_end < 0 )
		dlg.m_end = 0;
	if( dlg.m_end > m_sl_mp_rangemax )
		dlg.m_end = m_sl_mp_rangemax;

	int createstartflag;
	int createendflag;

	if( dlg.m_create ){
		/***
		int tempmpid;
		ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, firstjoint->serialno, dlg.m_start, &tempmpid );
		if( ret ){
			DbgOut( "motparamdlg : SaveMotion : IsExistMotionPoint error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( tempmpid >= 0 ){
			createstartflag = 0;
		}else{
			createstartflag = 1;
		}

		if( dlg.m_start != dlg.m_end ){
			ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, firstjoint->serialno, dlg.m_end, &tempmpid );
			if( ret ){
				DbgOut( "motparamdlg : SaveMotion : IsExistMotionPoint error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			if( tempmpid >= 0 ){
				createendflag = 0;
			}else{
				createendflag = 1;
			}

		}else{
			createendflag = 0;
		}
		***/

		createstartflag = 1;
		createendflag = 1;

	}else{
		createstartflag = 0;
		createendflag = 0;
	}

	if( createstartflag ){

		ret = CreateMotionPoints( -1, m_mot_cookie, dlg.m_start );
		if( ret ){
			DbgOut( "motparamdlg : SaveMotion : CreateMotionPoints error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	if( createendflag ){

		ret = CreateMotionPoints( -1, m_mot_cookie, dlg.m_end );
		if( ret ){
			DbgOut( "motparamdlg : SaveMotion : CreateMotionPoints error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


///////
	CQuaFile* quafile;
	quafile = new CQuaFile();
	if( !quafile ){
		ErrorMes( E_OUTOFMEMORY, MSGERR_APPMUSTEXIT, "SaveMotion quafile alloc error" );
		_ASSERT( 0 );
		return 1;
	}

	ret = quafile->SaveQuaFile( g_inidata->opt_namequa, m_thandler, m_shandler, m_mhandler, dlg.m_name, m_mot_cookie, dlg.m_start, dlg.m_end );
	if( ret ){
		DbgOut( "MotParamDlg : SaveMotion : SaveQuaFile error !!!\n" );
		_ASSERT( 0 );
		delete quafile;
		ErrorMes( -1, MSGERR_APPMUSTEXIT, "SaveMotion SaveQuaFile error" );
		return 1;
	}

	delete quafile;

	if( namedlg && (dlg.m_start == 0) && (dlg.m_end == m_sl_mp_rangemax) ){
		strcpy_s( namedlg->m_quaname, 2048, dlg.m_name );
	}

	if( (dlg.m_start == 0) && (dlg.m_end == m_sl_mp_rangemax) ){
		char* enptr = 0;
		enptr = strrchr( dlg.m_name, '\\' );
		if( enptr ){
			strcpy_s( m_motid[m_mot_cookie].filename, 256, enptr + 1 );
		}else{
			strcpy_s( m_motid[m_mot_cookie].filename, 256, dlg.m_name );
		}
	}

////////
	/***
	if( createstartflag ){
		ret = m_mhandler->DestroyMotionFrame( m_shandler, m_mot_cookie, dlg.m_start );
		if( ret ){
			DbgOut( "motparamdlg : SaveMotion : DestroyMotionFrame error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	if( createendflag ){
		ret = m_mhandler->DestroyMotionFrame( m_shandler, m_mot_cookie, dlg.m_end );
		if( ret ){
			DbgOut( "motparamdlg : SaveMotion : DestroyMotionFrame error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	***/

	return 0;
}
int CMotParamDlg::FillTree()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	int shdnum;
	shdnum = m_shandler->s2shd_leng;

	if( m_CEBuff ){
		free( m_CEBuff );
		m_CEBuff = 0;
	}
	m_CEBuff = (COPYELEM*)malloc( sizeof( COPYELEM ) * shdnum * CPFRAMEMAX );
	if( !m_CEBuff ){
		_ASSERT( 0 );
		return 1;
	}

	InitCEBuff();

	DestroyUndoBuffer();
	InitUndoBuffer();


	CShdElem* firstjoint = 0;
	FindFirstJoint( (*m_shandler)( 0 ), &firstjoint );
	if( firstjoint ){
		m_selectedno = firstjoint->serialno;
	}else{
		m_selectedno = 0;
	}

	return 0;
}

int CMotParamDlg::InvalidateTreeView()
{
	if( m_CEBuff ){
		free( m_CEBuff );
		m_CEBuff = 0;
	}
	DestroyUndoBuffer();

	return 0;
}
LRESULT CMotParamDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( !m_thandler || !m_shandler || !m_mhandler ) 
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
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

		if( m_dragflag )
			return 0;

		int ret;
		m_dlg_wnd.ClientToScreen( &clickpoint );//!!!!!!!!!!!!!!!!
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

			CMotionCtrl* mcptr = (*m_mhandler)( m_selectedno );
			_ASSERT( mcptr );
			CMotionPoint2* isexist = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
			if( isexist ){
				m_dragframe = frameno;
				m_dragflag = 1;
				m_dragmp = isexist;

				ret = mcptr->LeaveFromChainMP( m_mot_cookie, m_dragframe );
				if( ret ){
					DbgOut( "MotParamDlg : OnLButtonDown : LeaveFromChainMP error !!!\n" );
					ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : OnLButtonDown : LeaveFromChainMP error" );
					_ASSERT( 0 );
					m_dragframe = 0;
					m_dragflag = 0;
					m_dragmp = 0;
					return 1;
				}

				DbgOut( "MotParamDlg : OnLButtonDown : drag start, posx %d, m_dragframe %d\n",
					posx, m_dragframe );
			}else{
				m_dragframe = 0;
				m_dragflag = 0;
				m_dragmp = 0;

				//unsigned char keybuf[256];
				//GetKeyboardState( keybuf );
				if( g_KeyBuffer[VK_SHIFT] & 0x80 ){
					int findmp = 0;
					int seri;
					for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
						CShdElem* selem;
						selem = (*m_shandler)( seri );
						_ASSERT( selem );
						if( selem->IsJoint() && (selem->type != SHDMORPH) ){
							CMotionCtrl* mcptr2;
							mcptr2 = (*m_mhandler)( seri );
							_ASSERT( mcptr2 );
							CMotionPoint2* exist2;
							exist2 = mcptr2->IsExistMotionPoint( m_mot_cookie, frameno );
							if( exist2 ){
								findmp = 1;
								break;
							}
						}
					}

					if( findmp == 1 ){
						graymovestart = frameno;
						graymove = frameno;
						m_dragflag = 1;
					}else{
						graymove = -1;
					}
				}
			}
		}else{
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragmp = 0;
		
			graymove = -1;
		}
	
	}

	return 0;
}


LRESULT CMotParamDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	static int busyflag = 0;


	if( busyflag == 1 )
		return 0;

	busyflag = 1;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	if( m_ctrldrag || m_altdrag ){
		m_ctrldrag = 0;
		m_altdrag = 0;
		m_dragx = 0;
	}else{

		int ret;

		int frameno = -1;

		CMotionPoint2* isexist = 0;
		CMotionPoint2* isexist2 = 0;

		if( (m_dragflag == 1) && m_dragmp ){
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

	//DbgOut( "motparamdlg : OnLButtonUp : Windowpos2Frameno : frameno %d\n", frameno );

			CMotionCtrl* mcptr = (*m_mhandler)( m_selectedno );
			_ASSERT( mcptr );
			isexist = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
			if( isexist ){
				ret = mcptr->DeleteMotionPoint( m_mot_cookie, frameno );
				if( ret ){
					DbgOut( "motparamdlg : OnLButtonUp : DeleteMotionPoint error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				//frameno = m_dragframe;//!!!!! 元に戻す
			}
			

			float tempmvx, tempmvy, tempmvz;
			CQuaternion tempq;
			DWORD tempsw;
			int tempinterp;
			float tempscx, tempscy, tempscz;
			int tempuserint1;
			D3DXVECTOR3 tempeul;

			tempmvx = m_dragmp->m_mvx;
			tempmvy = m_dragmp->m_mvy;
			tempmvz = m_dragmp->m_mvz;
			tempq = m_dragmp->m_q;
			//tempsw = m_dragmp->dispswitch;
			tempsw = 0xFFFFFFFF;

			tempinterp = m_dragmp->interp;

			tempscx = m_dragmp->m_scalex;
			tempscy = m_dragmp->m_scaley;
			tempscz = m_dragmp->m_scalez;
			tempuserint1 = m_dragmp->m_userint1;
			tempeul = m_dragmp->m_eul;
			QUASPPARAM savespp;
			ZeroMemory( &savespp, sizeof( QUASPPARAM ) );
			savespp.m_rotparam = m_dragmp->m_spp->m_rotparam;
			savespp.m_mvparam = m_dragmp->m_spp->m_mvparam;
			savespp.m_scparam = m_dragmp->m_spp->m_scparam;
			delete m_dragmp;
			m_dragmp = 0;


			CMotionPoint2* retmp;
			retmp = mcptr->AddMotionPoint( m_mot_cookie, frameno,
				tempq,
				tempmvx, tempmvy, tempmvz, tempsw, tempinterp, tempscx, tempscy, tempscz, tempuserint1 );
			if( !retmp ){
				DbgOut( "MotParamDlg : OnLButtonUp : AddMotionPoint error !!!\n" );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : OnLButtonUp : AddMotionPoint error" );
				_ASSERT( 0 );
				m_dragflag = 0;
				m_dragmp = 0;
				busyflag = 0;
				return 1;
			}
			retmp->m_eul = tempeul;
			if( retmp->m_spp ){
				retmp->m_spp->m_rotparam = savespp.m_rotparam;
				retmp->m_spp->m_mvparam = savespp.m_mvparam;
				retmp->m_spp->m_scparam = savespp.m_scparam;
			}


			//unsigned char keybuf[256];
			//GetKeyboardState( keybuf );
			if( g_KeyBuffer[VK_SHIFT] & 0x80 ){
			
				//m_dragframeの全てのモーションポイントを、framenoに、移動する。
				CMotionPoint2 tempmp;
				if( frameno != m_dragframe ){
					int elemno;
					for( elemno = 0; elemno < m_mhandler->s2mot_leng; elemno++ ){
						mcptr = (*m_mhandler)(elemno );
						if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
							isexist2 = mcptr->IsExistMotionPoint( m_mot_cookie, m_dragframe );
							if( isexist2 ){
								tempmp = *isexist2;
								tempmp.m_spp = 0;
								ZeroMemory( &savespp, sizeof( QUASPPARAM ) );
								savespp.m_rotparam = isexist2->m_spp->m_rotparam;
								savespp.m_mvparam = isexist2->m_spp->m_mvparam;
								savespp.m_scparam = isexist2->m_spp->m_scparam;


								ret = mcptr->DeleteMotionPoint( m_mot_cookie, m_dragframe );
								if( ret ){
									DbgOut( "motparamdlg : OnLButtonUp : DeleteMotionPoint error !!!\n" );
									_ASSERT( 0 );
									m_dragflag = 0;
									m_dragmp = 0;
									busyflag = 0;

									return 1;
								}
								

								//重なり禁止
								CMotionPoint2* exist3;
								exist3 = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
								if( exist3 ){
									ret = mcptr->DeleteMotionPoint( m_mot_cookie, frameno );
									if( ret ){
										DbgOut( "motparamdlg : OnLButoonUp : DeleteMotionPoint error !!!\n" );
										_ASSERT( 0 );
										m_dragflag = 0;
										m_dragmp = 0;
										busyflag = 0;

										return 1;
									}
								}

								//新規
								retmp = mcptr->AddMotionPoint( m_mot_cookie, frameno,
									tempmp.m_q,
									//tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, tempmp.dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
									tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, m_dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
								if( !retmp ){
									DbgOut( "motparamdlg : OnLButtonUp : AddMotionPoint error !!!\n" );
									_ASSERT( 0 );
									m_dragflag = 0;
									m_dragmp = 0;
									busyflag = 0;

									return 1;
								}
								retmp->m_eul = tempmp.m_eul;

								if( retmp->m_spp ){
									retmp->m_spp->m_rotparam = savespp.m_rotparam;
									retmp->m_spp->m_mvparam = savespp.m_mvparam;
									retmp->m_spp->m_scparam = savespp.m_scparam;
								}

							}
						}
					}
				}
			}


			ret = ParamsToDlg();
			_ASSERT( !ret );

			//UpdateMatrix( m_selectedno );
			UpdateAllMatrix( 1 );
			//SetCurrentMotion();

			::SendMessage( m_apphwnd, WM_USER_CALCEUL, 0, 0 );


			Redraw();

			//DbgOut( "MotParamDlg : OnLButtonUp : drag end, posx %d, frameno %d, m_dragframe %d\n",
			//	posx, frameno, m_dragframe );
		}else if( (m_dragflag == 1) && (graymove >= 0) ){

			//graymovestartの全てのモーションポイントを、graymoveに、移動する。
			CMotionCtrl* mcptr;
			CMotionPoint2 tempmp;
			QUASPPARAM savespp;
			if( graymovestart != graymove ){
				int elemno;
				for( elemno = 0; elemno < m_mhandler->s2mot_leng; elemno++ ){
					mcptr = (*m_mhandler)(elemno );
					if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
						isexist2 = mcptr->IsExistMotionPoint( m_mot_cookie, graymovestart );
						if( isexist2 ){
							tempmp = *isexist2;
							tempmp.m_spp = 0;
							ZeroMemory( &savespp, sizeof( QUASPPARAM ) );
							savespp.m_rotparam = isexist2->m_spp->m_rotparam;
							savespp.m_mvparam = isexist2->m_spp->m_mvparam;
							savespp.m_scparam = isexist2->m_spp->m_scparam;

							ret = mcptr->DeleteMotionPoint( m_mot_cookie, graymovestart );
							if( ret ){
								DbgOut( "motparamdlg : OnLButtonUp : DeleteMotionPoint error !!!\n" );
								_ASSERT( 0 );
								m_dragflag = 0;
								m_dragmp = 0;
								graymove = -1;
								busyflag = 0;

								return 1;
							}


							//重なり禁止
							CMotionPoint2* exist3;
							exist3 = mcptr->IsExistMotionPoint( m_mot_cookie, graymove );
							if( exist3 ){
								ret = mcptr->DeleteMotionPoint( m_mot_cookie, graymove );
								if( ret ){
									DbgOut( "motparamdlg : OnLButoonUp : DeleteMotionPoint error !!!\n" );
									_ASSERT( 0 );
									m_dragflag = 0;
									m_dragmp = 0;
									graymove = -1;
									busyflag = 0;

									return 1;
								}
							}

							CMotionPoint2* retmp;
							retmp = mcptr->AddMotionPoint( m_mot_cookie, graymove,
								tempmp.m_q,
								//tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, tempmp.dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
								tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, m_dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
							if( !retmp ){
								DbgOut( "motparamdlg : OnLButtonUp : AddMotionPoint error !!!\n" );
								_ASSERT( 0 );
								m_dragflag = 0;
								m_dragmp = 0;
								graymove = -1;
								busyflag = 0;

								return 1;
							}
							retmp->m_eul = tempmp.m_eul;

							if( retmp->m_spp ){
								retmp->m_spp->m_rotparam = savespp.m_rotparam;
								retmp->m_spp->m_mvparam = savespp.m_mvparam;
								retmp->m_spp->m_scparam = savespp.m_scparam;
							}

						}
					}
				}
			}


			ret = ParamsToDlg();
			_ASSERT( !ret );

			//UpdateMatrix( m_selectedno );
			UpdateAllMatrix( 1 );
			//SetCurrentMotion();

			::SendMessage( m_apphwnd, WM_USER_CALCEUL, 0, 0 );

			graymove = -1;
			Redraw();

		}
		

		//if( frameno == m_dragframe ){
		//	DbgOut( "MotParamDlg : OnLButtonUp : motion point already exist warning %d, %d !!!\n", frameno, m_dragframe );
		//}


		m_dragflag = 0;
		m_dragframe = 0;
		graymove = -1;

	}	
		
	busyflag = 0;

	return 0;
}

LRESULT CMotParamDlg::OnRButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( !m_thandler || !m_shandler || !m_mhandler ) 
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	POINT clickpoint;
	clickpoint.x = LOWORD(lParam);
	clickpoint.y = HIWORD(lParam);
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

		int* findseri;
		findseri = (int*)malloc( sizeof( int ) * m_shandler->s2shd_leng );
		if( !findseri ){
			DbgOut( "motparamdlg : OnRButtonUP : findseri alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( findseri, sizeof( int ) * m_shandler->s2shd_leng );

		int findnum = 0;
		int seri;
		for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
			CMotionCtrl* mcptr;
			mcptr = (*m_mhandler)( seri );
			_ASSERT( mcptr );
			if( mcptr->IsJoint() ){
				CMotionPoint2* isexist = 0;
				isexist = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
				if( isexist ){
					*( findseri + findnum ) = seri;
					findnum++;
				}
			}
		}

		if( findnum > 0 ){

			CKeyMenuMain keymenu;
			ret = keymenu.Create( m_apphwnd, m_thandler, findseri, findnum );
			if( ret ){
				DbgOut( "motparamdlg : OnRButtonUp : keymenu Create error !!!\n" );
				_ASSERT( 0 );
				free( findseri );
				return 1;
			}

			int selectno;
			POINT pt;
			::GetCursorPos( &pt );
			selectno = keymenu.TrackPopupMenu( m_hWnd, pt );
			keymenu.Destroy();
			if( selectno >= 65000 ){
				int newselectseri;

				newselectseri = *( findseri + selectno - 65000 );
				if( (newselectseri > 0) && (newselectseri < m_shandler->s2shd_leng) ){
					
					OnUserSelchange( newselectseri );

					::SendMessage( m_apphwnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)newselectseri );
					Redraw();
				}
			}
		}

		free( findseri );
	}

	return 0;
}


LRESULT CMotParamDlg::OnParamSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret = 0;
	int dlgret;
	m_mhandler->GetDefInterp( m_mot_cookie, &m_interpolation );
	CMotSettingDlg	dlg( m_calcsp, m_motionname, m_sl_mp_rangemax + 1, m_motiontype, m_interpolation, m_motionjump, m_startframe, m_endframe );
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	dlgret = (int)dlg.DoModal();
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( dlgret == IDOK ){
		if( m_motiontype != dlg.m_mottype ){
			m_motiontype = dlg.m_mottype;
			ret = m_mhandler->SetMotionType( m_mot_cookie, m_motiontype );
			if( ret ){
				_ASSERT( 0 );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : SetMotionType error!!!" );
				return 1;
			}
		}

		if( m_motionjump != dlg.m_motjump ){
			m_motionjump = dlg.m_motjump;
			ret = m_mhandler->SetMotionJump( m_mot_cookie, m_motionjump );
			if( ret ){
				_ASSERT( 0 );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : SetMotionJump error!!!" );
				return 1;
			}
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
				::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
				dlgret2 = ::MessageBox( NULL, tempchar, "警告", MB_OKCANCEL );
				::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

			}

			if( dlgret2 == IDOK ){
				ret = m_mhandler->ChangeTotalFrame( m_mot_cookie, newrangemax + 1, (newrangemax > m_sl_mp_rangemax) );
				if( ret ){
					_ASSERT( 0 );
					ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : ChangeTotalFrame error!!!" );
					return 1;
				}
				m_sl_mp_rangemax = newrangemax;


				m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, m_startframe );
				m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
				m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
				SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//


				m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
				m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
				SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
				m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

				ret += m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
				ret += m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
				if( ret ){
					_ASSERT( 0 );
					ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : MakeRawmat error!!!" );
					return 1;
				}

				BOOL dummy;
				m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );
				OnReleasedcaptureSl_mp( 0, 0, dummy );
			}else{
				m_startframe = savestart;
				m_endframe = saveend;
			}
		}

		m_range = m_endframe - m_startframe;

		if( (m_startframe != savestart) || (m_endframe != saveend) ){
			m_current = m_startframe;
			InterlockedExchange( &m_previewframe, m_startframe );

			m_motid[ m_mot_cookie ].startframe = m_startframe;
			m_motid[ m_mot_cookie ].endframe = m_endframe;
			m_motid[ m_mot_cookie ].framerange = m_range;

		}

		int newinterpolation = dlg.m_interpolation;
		if( m_interpolation != newinterpolation ){

			m_interpolation = newinterpolation;

			ret = m_mhandler->SetDefInterp( m_mot_cookie, m_interpolation );
			if( ret ){
				_ASSERT( 0 );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : SetInterpolation error!!!" );
				return 1;
			}
			
			ret += m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
			ret += m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
			if( ret ){
				_ASSERT( 0 );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : MakeRawmat error!!!" );
				return 1;
			}
			ret = SetCurrentMotion();
			if( ret ){
				_ASSERT( 0 );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : SetCurrentMotion error!!!" );
				return 1;
			}

			Redraw();

		}

		int cmp;
		cmp = strcmp( m_motionname, dlg.m_name );
		if( cmp != 0 ){
			strcpy_s( m_motionname, 256, dlg.m_name );

			ret = m_mhandler->SetMotionName( m_mot_cookie, dlg.m_name );
			if( ret ){
				_ASSERT( 0 );
				ErrorMes( -1, MSGERR_APPMUSTEXIT, "OnParamSet : SetMotionName error !!!\n" );
				return 1;
			}

			::SendMessage( m_apphwnd, WM_USER_ANIMMENU, m_mot_cookie, 0 );

		}

		m_calcsp = dlg.m_calcsp;
		g_inidata->motdlg_calcsp = m_calcsp;//!!!!!!!!!!!!
		if( m_calcsp == 1 ){
			ret = CalcSplineParam( 1, -1 );
			_ASSERT( !ret );
		}


		ParamsToDlg();
		DrawMotionPoints();
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );

	}

	int mindex;
	int findindex = 0;
	for( mindex = 0; mindex < m_motionnum; mindex++ ){
		if( m_mot_cookie == m_motid[ mindex ].id ){
			findindex = mindex;
			break;
		}
	}
	::SendMessage( m_apphwnd, WM_USER_ANIMMENU, findindex, 0 );
	


	return 0;
}

void CMotParamDlg::ErrorMes( int errorcode, int type, char* mes )
{
	ERRORMES errormes;
	if( m_apphwnd ){
		errormes.errorcode = errorcode;
		errormes.type = type;
		errormes.mesptr = mes;
		SendMessage( m_apphwnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
	}
}

///////////////
/***
LRESULT CALLBACK TreeMotProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  ){
	if( (uMsg != WM_KEYDOWN) && (uMsg != WM_KEYUP) ){
		return CallWindowProc(
		  (WNDPROC)s_treefunc,  // pointer to previous procedure
		  s_tree_mot_wnd,              // handle to window
		  uMsg,               // message
		  wParam,          // first message parameter
		  lParam           // second message parameter
		);		
	}else{
		//key は無効にする。

		//if( uMsg == WM_KEYDOWN ){
		//	DbgOut( "MotParamDlg : TreeMotProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "MotParamDlg : TreeMotProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}


		return 0;
	}
}
***/
LRESULT CALLBACK MPProc(
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
		//	DbgOut( "MotParamDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "MotParamDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}


		return 0;
	}
}

int CMotParamDlg::SetNewWindowProc()
{
	//s_treefunc = ::GetWindowLong( m_tree_mot_wnd, GWL_WNDPROC );
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWL_WNDPROC );
	if( !s_mpfunc ){
		_ASSERT( 0 );
		return 1;
	}

	LONG lret;
//	lret = ::SetWindowLong( m_tree_mot_wnd, GWL_WNDPROC, (LONG)TreeMotProc );
//	if( lret == 0 ){
//		_ASSERT( 0 );
//		return 1;
//	}
	lret = ::SetWindowLong( m_sl_mp_wnd, GWL_WNDPROC, (LONG)MPProc );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMotParamDlg::InverseMotionXZ()
{
	int ret = 0;
	int serino;
	CShdElem* curselem;

	for( serino = 1; serino < m_shandler->s2shd_leng; serino++ ){
		curselem = (*m_shandler)( serino );
		if( curselem->IsJoint() && (curselem->type != SHDMORPH)){
			CPart* partptr = curselem->part;
			_ASSERT( partptr );
			int bonenum = partptr->bonenum;
			if( bonenum ){

				CMotionCtrl* mcptr = (*m_mhandler)(serino);
				_ASSERT( mcptr );	
				CMotionPoint2* mpptr = mcptr->GetMotionPoint( m_mot_cookie );

				CMotionPoint2* curmp = mpptr;
				while( curmp ){
					//curmp->m_rotx *= -1.0f;
					//curmp->m_rotz *= -1.0f;
					curmp->m_mvz *= -1.0f;

					curmp = curmp->next;
				}

			}
		}
	}

	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : InverseMointXZ : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	SetCurrentMotion();

	Redraw();

	return 0;
}

int CMotParamDlg::SetNewPoseByMoa()
{
	if( s_lThread == 0 )
		return 0;//!!!!!!!!!!!!!!!!!

	int ret;
	int eventno = 0;
	int cno;
	for( cno = 0; cno < 256; cno++ ){
		if( (g_validkey[cno] == 1) && (g_keyboardcnt[cno] != 0) ){
			eventno = m_mhandler->m_ek.GetEventNo( cno, g_keyboardcnt[cno] );
			//_ASSERT( 0 );
			break;
		}
	}
//////////////		

	int curmotid = -1;
	int curframe = 0;
	int curframeleng = 0;
	ret = m_mhandler->GetMotionFrameNo( &curmotid, &curframe );
	if( ret || (curmotid < 0) ){
		DbgOut( "motparamdlg : SetNewPoseByMOA : mh GetMotionFrameNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_mhandler->GetMotionFrameLength( curmotid, &curframeleng );
	if( ret ){
		DbgOut( "motparamdlg : SetNewPoseByMOA : mh GetMotionFrameLength error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int curmottype = -1;
	ret = m_mhandler->GetMotionType( curmotid, &curmottype );
	if( ret ){
		DbgOut( "motparamdlg : SetNewPoseByMOA : GetMotionType error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


//DbgOut( "check !!! : hs : SetNewPoseByMOA : curmotid %d, curframe %d, FILLUPMOTIONID %d\r\n",
//	curmotid, curframe, FILLUPMOTIONID );

	int checkfillup = 0;


	int nextmotid = -1;
	int nextframe = 0;
	int nextframeleng = 0;

	int befmotid = -1;
	int befframe = 0;

	int fillupflag = 0;
	int notfu = 0;
	int ev0idle = 0;
	int nottoidle = 0;

	if( curmotid != m_mhandler->m_fuid ){
		int tmpnottoidle = 0;
		ret = m_mch->GetNextMotion( curmotid, curframe, eventno, &nextmotid, &nextframe, &notfu, &tmpnottoidle );
		if( ret ){
			DbgOut( "motparamdlg : SetNewPoseByMOA : mch GetNextMotion error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( tmpnottoidle < 0 ){
			m_mhandler->GetNotToIdle( -1, &nottoidle );
		}else{
			nottoidle = tmpnottoidle;
			m_mhandler->SetNotToIdle( -1, nottoidle );
		}


		ret = m_mch->GetEv0Idle( curmotid, &ev0idle );
		if( ret ){
			DbgOut( "motparamdlg : SetNewPoseByMOA : mch GetEv0Idle 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		
		if( (eventno == 0) && (ev0idle != 0) ){
			nextmotid = m_mch->GetIdlingMotID();
			nextframe = 0;
			notfu = 0;
			if( nextmotid < 0 ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mch GetIdlingMotID 0 : idling motion not exist error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else{
			if( curmotid == nextmotid ){
				//最終フレームだった場合は、アイドリングへ
				if( (curmottype == MOTION_CLAMP) && (curframe >= (curframeleng - 1)) && (nottoidle == 0) ){
					nextmotid = m_mch->GetIdlingMotID();
					nextframe = 0;
					notfu = 0;
					if( nextmotid < 0 ){
						DbgOut( "motparamdlg : SetNewPoseByMOA : mch GetIdlingMotID 1 : idling motion not exist error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

				}
			}
		}

	}else{
		//補間モーション中だった場合。
		checkfillup = 1;

		ret = m_mhandler->GetNextMotionFrameNo( m_mhandler->m_fuid, &befmotid, &befframe );
		if( ret || (befmotid < 0) ){
			DbgOut( "motparamdlg : SetNewPoseByMOA : mh GetNextMotionFrameNo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int tmpframe;
		tmpframe = max( 0, (befframe - m_mch->m_fillupleng) );
		int tmpnottoidle = 0;
		ret = m_mch->GetNextMotion( befmotid, tmpframe + curframe, eventno, &nextmotid, &nextframe, &notfu, &tmpnottoidle );
		//ret = m_mch->GetNextMotion( befmotid, befframe + curframe, eventno, &nextmotid, &nextframe, &notfu );
		if( ret ){
			DbgOut( "motparamdlg : SetNewPoseByMOA : mch GetNextMotion error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
//DbgOut( "check !!! : hs : SetNewPoseByMOA : curmotid == FILLUPMOTIONID, befmotid %d, befframe %d, nextmotid %d, nextframe %d\r\n",
//	   befmotid, befframe, nextmotid, nextframe );

		if( tmpnottoidle < 0 ){
			m_mhandler->GetNotToIdle( -1, &nottoidle );
		}else{
			nottoidle = tmpnottoidle;
			m_mhandler->SetNotToIdle( -1, nottoidle );
		}
		
		ret = m_mch->GetEv0Idle( befmotid, &ev0idle );
		if( ret ){
			DbgOut( "motparamdlg : SetNewPoseByMOA : mch GetEv0Idle 2 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		
		if( (eventno == 0) && (ev0idle != 0) ){
			nextmotid = m_mch->GetIdlingMotID();
			nextframe = 0;
			notfu = 0;
			if( nextmotid < 0 ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mch GetIdlingMotID 2 : idling motion not exist error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

		}
		
		if( befmotid != nextmotid ){
			fillupflag = 1;
		}

	}


	if( ((checkfillup == 0) && (curmotid != nextmotid)) || fillupflag ){

		if( notfu == 0 ){

			ret = m_mhandler->GetMotionFrameLength( nextmotid, &nextframeleng );
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh GetMotionFrameLength error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			int filluppoint;
			filluppoint = min( (nextframeleng - 1), (nextframe + m_mch->m_fillupleng - 1) );

			ret = m_mhandler->CopyMotionFrame( m_shandler, curmotid, curframe, m_mhandler->m_fuid, 0 );
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh CopyMotionFrame fu0 error %d, %d!!!\n", curmotid, curframe );
				_ASSERT( 0 );
				return 1;
			}

			ret = m_mhandler->CopyMotionFrame( m_shandler, nextmotid, filluppoint, m_mhandler->m_fuid, m_mch->m_fillupleng - 1 );
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh CopyMotionFrame fu last error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			//morph anim
			ret = m_mhandler->CopyMMotAnimFrame( -1, m_shandler, curmotid, curframe, m_mhandler->m_fuid, 0 );
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh CopyMMotAnimFrame 0 error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			ret = m_mhandler->CopyMMotAnimFrame( -1, m_shandler, nextmotid, filluppoint, m_mhandler->m_fuid, m_mch->m_fillupleng - 1 );
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh CopyMMotAnimFrame 0 error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			//dskey, texkeyの設定
			int bno;
			for( bno = 0; bno < m_shandler->s2shd_leng; bno++ ){
				CShdElem* selem;
				selem = (*m_shandler)( bno );
				_ASSERT( selem );
				if( selem->IsJoint() && selem->type != SHDMORPH ){

					ret = m_shandler->InitTempDS();
					_ASSERT( !ret );

					ret = m_mhandler->InitTexAnim( m_shandler );
					_ASSERT( !ret );

					//ret = m_mhandler->InitAlpAnim( m_shandler );
					//_ASSERT( !ret );

					if( m_dsfillupmode == 0 ){
						ret = m_mhandler->GetDSAnim( m_shandler->m_tempds, bno, curmotid, curframe );
						_ASSERT( !ret );
					}else{
						ret = m_mhandler->GetDSAnim( m_shandler->m_tempds, bno, nextmotid, filluppoint );
						_ASSERT( !ret );
					}

					if( m_texfillupmode == 0 ){
						ret = m_mhandler->GetTexAnim( bno, curmotid, curframe );
						_ASSERT( !ret );
					}else{
						ret = m_mhandler->GetTexAnim( bno, nextmotid, filluppoint );
						_ASSERT( !ret );
					}

					int dirtyds = 0;
					int dsno;
					for( dsno = 0; dsno < DISPSWITCHNUM; dsno++ ){
						if( (m_shandler->m_tempds + dsno)->state == 0 ){
							dirtyds++;
							break;
						}
					}

					CMQOMaterial* curmat = m_shandler->m_mathead;
					int dirtytex = 0;
					while( curmat ){
						if( curmat->curtexname != NULL ){
							dirtytex++;
//_ASSERT( 0 );
							break;
						}
						curmat = curmat->next;
					}

					ret = m_mhandler->DeleteDSKey( bno, m_mhandler->m_fuid, 0 );
					_ASSERT( !ret );

					ret = m_mhandler->DeleteTexKey( bno, m_mhandler->m_fuid, 0 );
					_ASSERT( !ret );

					ret = m_mhandler->DeleteAlpKey( bno, m_mhandler->m_fuid, 0 );
					_ASSERT( !ret );


					if( dirtyds ){
						CDSKey* dskptr;
						ret = m_mhandler->CreateDSKey( bno, m_mhandler->m_fuid, 0, &dskptr );
						_ASSERT( !ret );

						for( dsno = 0; dsno < DISPSWITCHNUM; dsno++ ){
							if( (m_shandler->m_tempds + dsno)->state == 0 ){
								ret = dskptr->SetDSChange( m_shandler->m_ds + dsno, 0 );
								_ASSERT( !ret );
							}
						}
					}

					if( dirtytex ){
						CTexKey* tkptr;
						ret = m_mhandler->CreateTexKey( bno, m_mhandler->m_fuid, 0, &tkptr );
						_ASSERT( !ret );

						curmat = m_shandler->m_mathead;
						while( curmat ){
							if( curmat->curtexname ){
								ret = tkptr->SetTexChange( curmat, curmat->curtexname );
								_ASSERT( !ret );
							}
							curmat = curmat->next;
						}
					}

					CAlpKey* alpkptr = 0;
					ret = m_mhandler->CreateAlpKey( bno, m_mhandler->m_fuid, 0, &alpkptr );
					_ASSERT( !ret );

					curmat = m_shandler->m_mathead;
					while( curmat ){
						float alphaval = 1.0f;
						int existflag = 0;
						ret = m_mhandler->GetAlpAnim( bno, curmotid, curframe, curmat, &alphaval, &existflag );
						_ASSERT( !ret );

						if( alphaval != curmat->orgalpha ){
							ret = alpkptr->SetAlpChange( curmat, alphaval );
							_ASSERT( !ret );
						}
						curmat = curmat->next;
					}
				}
			}
			


			///////
			ret = m_mhandler->FillUpMotion( m_shandler, m_mhandler->m_fuid, -1, 0, m_mch->m_fillupleng - 1, 0 );///// initflag 0 !!!!!
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh FillUpMotion fu error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			int nextfirstframe;
			nextfirstframe = min( (filluppoint + 1), (nextframeleng - 1) );

			//ret = m_mhandler->SetNextMotionFrameNo( m_shandler, FILLUPMOTIONID, nextmotid, filluppoint );//// filluppointへジャンプ
			ret = m_mhandler->SetNextMotionFrameNo( m_shandler, m_mhandler->m_fuid, nextmotid, nextfirstframe, -1 );//// filluppoint + 1へジャンプ
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh SetNextMotionFrameNo fu error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = m_mhandler->SetMotionKind( m_mhandler->m_fuid );
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh SetMotionKind fu error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

	//DbgOut( "check !!! : hs : SetNewPoseByMOA : !!!!!!!!!! FillUp !!!!!!!!!!!!, filluppoint %d, nextframeleng %d\r\n",
	//	   filluppoint, nextframeleng );

		}else{

			// notfu != 0  direct change
			ret = m_mhandler->SetMotionFrameNo( m_shandler, nextmotid, nextframe, 1 );
			if( ret ){
				DbgOut( "motparamdlg : SetNewPoseByMOA : mh SetMotionFrameNo error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}
	
	int tmpnextframe;
	tmpnextframe = m_mhandler->SetNewPose( m_shandler );
	if( tmpnextframe < 0 ){
		DbgOut( "motparamdlg : SetNewPoseByMOA : mh SetNewPose error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


//////////////
	SleepEx( 0, FALSE );
	return 0;
}


int CMotParamDlg::SetNewPose()
{

//DbgOut( "check !!! : mpd SetNewPose bef : s_frameno %d, m_current %d\r\n", s_frameno, m_current );
	if( g_endapp == 1 ){
		return 0;
	}


	if( m_timerworking != 1 ){
		return 0;
	}

	BOOL dummy;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)s_frameno );

	//m_current = s_frameno;
	//OnReleasedcaptureSl_mp( 0, 0, dummy );
	//::SendMessage( m_apphwnd, WM_USER_DISPLAY, 0, 0 );

	if( s_lThread == 0 )
		return 0;//!!!!!!!!!!!!!!!!!

//	if( m_preview_flag == 0 ){
//		return 0;
//	}

	int stopmotion = 0;
	int stopcamera = 0;
	int stopgp = 0;
	int stopmoe = 0;

	if( m_previewmotion == 1 ){
		if( (m_motiontype == MOTION_CLAMP) && (s_frameno >= m_endframe) ){
			stopmotion = 1;
		}
		if( ((m_startframe != 0) || (m_endframe != m_sl_mp_rangemax)) && (s_frameno >= m_endframe) ){
			stopmotion = 1;
		}
	}else{
		stopmotion = 1;
	}


	if( !m_camkeydlg ){
		return 0;
	}

	if( m_previewcamera == 1 ){
		CCameraAnim* anim;
		anim = m_camkeydlg->m_cah->GetCameraAnim( m_camkeydlg->m_animno );
		if( anim ){
			if( (anim->m_animtype == MOTION_CLAMP) && (m_camkeydlg->m_current >= m_camkeydlg->m_endframe) ){
				stopcamera = 1;
			}
			if( ((m_camkeydlg->m_startframe != 0) || (m_camkeydlg->m_endframe != m_camkeydlg->m_sl_mp_rangemax)) && 
				(m_camkeydlg->m_current >= m_camkeydlg->m_endframe) ){
				stopcamera = 1;
			}
		}else{
			stopcamera = 1;
		}
	
		if( stopmotion == 1 ){
			if( m_camkeydlg->m_camerabutton == 0 ){
				//CAMB_HENのときはモーションに合わせる
				stopcamera = 1;
			}
		}
	}else{
		stopcamera = 1;
	}

	if( !m_gpkeydlg ){
		return 0;
	}

	if( m_previewgp == 1 ){
		CGPAnim* anim;
		anim = m_gpkeydlg->m_gpah->GetGPAnim( m_gpkeydlg->m_animno );
		if( anim ){
			if( (anim->m_animtype == MOTION_CLAMP) && (m_gpkeydlg->m_current >= m_gpkeydlg->m_endframe) ){
				stopgp = 1;
			}
			if( ((m_gpkeydlg->m_startframe != 0) || (m_gpkeydlg->m_endframe != m_gpkeydlg->m_sl_mp_rangemax)) && 
				(m_gpkeydlg->m_current >= m_gpkeydlg->m_endframe) ){
				stopgp = 1;
			}
		}else{
			stopgp = 1;
		}
	}else{
		stopgp = 1;
	}

	if( !m_moekeydlg ){
		return 0;
	}

	if( m_previewmoe == 1 ){
		CMOEAnim* anim;
		anim = m_moekeydlg->m_moeah->GetMOEAnim( m_moekeydlg->m_animno );
		if( anim ){
			if( (anim->m_animtype == MOTION_CLAMP) && (m_moekeydlg->m_current >= m_moekeydlg->m_endframe) ){
				stopmoe = 1;
			}
			if( ((m_moekeydlg->m_startframe != 0) || (m_moekeydlg->m_endframe != m_moekeydlg->m_sl_mp_rangemax)) && 
				(m_moekeydlg->m_current >= m_moekeydlg->m_endframe) ){
				stopmoe = 1;
			}
		}else{
			stopmoe = 1;
		}
	}else{
		stopmoe = 1;
	}

//DbgOut( "check !!! : mpd SetNewPose : stopmotion %d, stopcamera %d\r\n", stopmotion, stopcamera );


	if( stopmotion == 0 ){
		if( m_mhandler ){
			int retframe;
			retframe = m_mhandler->SetNewPose( m_shandler );
			
			//2003.12.20 SetNewPoseは、実行中に呼ばれると、-1を返すようにしたので、チェックすること！！！
			if( retframe >= 0 ){
				InterlockedExchange( &s_frameno, (LONG)retframe );
				InterlockedExchange( &m_current, s_frameno );
			}else{
				InterlockedExchange( &m_current, s_frameno );
			}
		}
	}else{
		InterlockedExchange( &m_previewmotion, 0 );
	}
	if( stopcamera == 0 ){
		m_camkeydlg->SetNewCamera();
		m_d3dapp->UpdateCameraFromDlg();
	}else{
		InterlockedExchange( &m_previewcamera, 0 );
	}
	if( stopgp == 0 ){
		m_gpkeydlg->SetNewGP();
	}else{
		InterlockedExchange( &m_previewgp, 0 );
	}
	if( stopmoe == 0 ){
		m_moekeydlg->SetNewMOE();
	}else{
		InterlockedExchange( &m_previewmoe, 0 );
	}



	if( (m_previewmotion == 0) && (m_previewcamera == 0) && (m_previewgp == 0) && (m_previewmoe == 0) ){
		OnStop( 0, 0, 0, dummy );
	}


//m_current
	char tempchar[1024];
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "frameno: current %d, start %d, end %d, total %d", m_current, m_startframe, m_endframe, m_sl_mp_rangemax + 1 );
	if( (g_endapp == 1) ||!m_tex_current_wnd.IsWindow() )
		return 0;
	m_tex_current_wnd.SetWindowText( tempchar );

//m_sl_mp_
	if( (g_endapp == 1) ||!m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	if( m_texkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_texkeydlg->ParamsToDlg();
	}
	if( m_alpkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_alpkeydlg->ParamsToDlg();
	}
	if( m_dskeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_dskeydlg->ParamsToDlg();
	}
	if( m_MorphKeyDlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_MorphKeyDlg->ParamsToDlg();
	}
	if( m_grotdlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_grotdlg->Redraw();
	}

	SleepEx( 0, FALSE );

	return 0;
}


int CMotParamDlg::SetNewPoseSnd()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( !m_sndkeydlg ){
		return 0;
	}

	int ret;

	BOOL dummy;
//	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)s_frameno );

	if( m_timerworking != 1 ){
		return 0;
	}

	if( s_lThread == 0 )
		return 0;//!!!!!!!!!!!!!!!!!

	int stopmotion = 0;
	int stopcamera = 0;
	int stopgp = 0;
	int stopmoe = 0;
	int stopsnd = 0;

	CSndAnim* anim;
	anim = m_sndkeydlg->m_sndah->GetSndAnim( m_sndkeydlg->m_animno );

	int bgmstopcnt = 0;
	int bgmstartcnt = 0;
	m_sndkeydlg->GetBGMStartCnt( &bgmstartcnt );
	m_sndkeydlg->GetBGMStopCnt( &bgmstopcnt );

	if( anim ){
		if( anim->m_bgmloop != 0 ){
			if( bgmstopcnt > s_bgmstopcnt ){
				m_sndkeydlg->GetBGMStartCnt( &s_bgmstartcnt );
				bgmstartcnt = s_bgmstartcnt;
				m_sndkeydlg->m_current = m_sndkeydlg->m_startframe;
				m_sndkeydlg->StartPreview( m_sndkeydlg->m_startframe );
				m_sndkeydlg->ParamsToDlg();
				m_sndframe = m_sndkeydlg->m_startframe;
				s_befframe = m_sndframe;
				m_sndkeydlg->GetBGMStopCnt( &s_bgmstopcnt );
				s_previewfirst = 1;
				bgmstopcnt = s_bgmstopcnt;
			}else if( (bgmstartcnt > s_bgmstartcnt) && (m_sndkeydlg->m_current >= m_sndkeydlg->m_endframe) ){
				m_sndkeydlg->StopBGM();

				m_sndkeydlg->GetBGMStartCnt( &s_bgmstartcnt );
				bgmstartcnt = s_bgmstartcnt;
				m_sndkeydlg->m_current = m_sndkeydlg->m_startframe;
				m_sndkeydlg->StartPreview( m_sndkeydlg->m_startframe );
				m_sndkeydlg->ParamsToDlg();
				m_sndframe = m_sndkeydlg->m_startframe;
				s_befframe = m_sndframe;
				m_sndkeydlg->GetBGMStopCnt( &s_bgmstopcnt );
				s_previewfirst = 1;
				bgmstopcnt = s_bgmstopcnt;
			}
		}
	}

	if( m_previewsnd == 1 ){
		if( anim ){
			if( (bgmstartcnt > s_bgmstartcnt) && (anim->m_bgmloop == 0) && (m_sndkeydlg->m_current >= m_sndkeydlg->m_endframe) ){
				stopsnd = 1;
			}

			if( (anim->m_bgmloop == 0) && (bgmstopcnt > s_bgmstopcnt) ){
				stopsnd = 1;
			}
		}else{
			stopsnd = 1;
		}
	}else{
		stopsnd = 1;
	}


	if( stopsnd == 1 ){
		stopmotion = 1;
		stopcamera = 1;
		stopgp = 1;
		stopmoe = 1;
	}

	if( stopsnd == 0 ){
		int sndframe = 0;
		ret = m_sndkeydlg->GetSndFrame( s_bgmstartcnt, &sndframe, &m_sndframe );
		_ASSERT( !ret );

		ret = m_sndkeydlg->PlayAnim( sndframe );
		_ASSERT( !ret );

		PlayAnim( m_sndframe + m_startframe );
		if( m_previewcamera ){
			m_camkeydlg->PlayAnim( m_sndframe + m_camkeydlg->m_startframe );
			m_d3dapp->UpdateCameraFromDlg();
		}
		if( m_previewgp )
			m_gpkeydlg->PlayAnim( m_sndframe + m_gpkeydlg->m_startframe );
		if( m_previewmoe )
			m_moekeydlg->PlayAnim( m_sndframe + m_moekeydlg->m_startframe );


//DbgOut( "checkframe!!! : m_sndframe %d\r\n", m_sndframe );
	}else{
		InterlockedExchange( &m_previewsnd, 0 );
		InterlockedExchange( &m_previewmotion, 0 );
		InterlockedExchange( &m_previewcamera, 0 );
		InterlockedExchange( &m_previewgp, 0 );
		InterlockedExchange( &m_previewmoe, 0 );
	}

	if( (m_previewmotion == 0) && (m_previewcamera == 0) && (m_previewgp == 0) && (m_previewmoe == 0) && (m_previewsnd == 0) ){
		OnStop( 0, 0, 0, dummy );
	}


	if( m_texkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_texkeydlg->ParamsToDlg();
	}
	if( m_alpkeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_alpkeydlg->ParamsToDlg();
	}
	if( m_dskeydlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_dskeydlg->ParamsToDlg();
	}
	if( m_MorphKeyDlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_MorphKeyDlg->ParamsToDlg();
	}
	if( m_grotdlg ){
		if( g_endapp == 1 ){
			return 0;
		}
		m_grotdlg->Redraw();
	}

	s_befframe = m_sndframe;


	SleepEx( 0, FALSE );

	return 0;
}



DWORD WINAPI CMotParamDlg::ThreadFunc( LPVOID	lpThreadParam )
{
	DbgOut( "MotParamDlg : ThreadFunc : Start\n" );

	static int isfirst = 1;
	int ret;

	while( s_lThread ){		

		//CMotParamDlg* mpd = (CMotParamDlg*)lpThreadParam;

		if( s_mpd && (s_mpd->m_preview_flag == 1) ){
			if( s_mpd && (s_mpd->m_underdestroyflag == 0) ){
				if( isfirst == 1 ){
					if( s_mpd && (s_mpd->m_previewmotion == 1) ){
						s_mpd->m_current = s_frameno;// == m_startframe 
						ret = s_mpd->m_mhandler->SetMotionFrameNo( s_mpd->m_shandler, s_mpd->m_mot_cookie, s_frameno, 1 );
						_ASSERT( !ret );
						s_mpd->ParamsToDlg();
					}
					if( s_ckd && (s_mpd->m_previewcamera == 1) ){
						s_ckd->m_current = s_ckd->m_startframe;// == m_startframe 
						s_ckd->SetCurrentMotion( 0 );
						s_ckd->ParamsToDlg();
					}
					if( s_gpkd && (s_mpd->m_previewgp == 1) ){
						s_gpkd->m_current = s_gpkd->m_startframe;// == m_startframe 
						s_gpkd->SetCurrentMotion( 0 );
						s_gpkd->ParamsToDlg();
					}	
					if( s_mpd && s_mch && (s_mpd->m_previewmoa == 1) ){
						int idlingmotid;
						idlingmotid = s_mch->GetIdlingMotID();
						if( idlingmotid >= 0 ){
							ret = s_mpd->m_mhandler->SetMotionKind( idlingmotid );
						}
					}
					if( s_moekd && (s_mpd->m_previewmoe == 1) ){
						s_moekd->m_current = s_moekd->m_startframe;// == m_startframe 
						s_moekd->SetCurrentMotion( 0 );
						s_moekd->ParamsToDlg();
					}	
					if( s_sndkd && (s_mpd->m_previewsnd == 1) ){
						s_sndkd->GetBGMStartCnt( &s_bgmstartcnt );
						s_sndkd->m_current = s_sndkd->m_startframe;// == m_startframe 
						s_sndkd->StartPreview( s_sndkd->m_startframe );
						s_sndkd->ParamsToDlg();
						s_mpd->m_sndframe = s_sndkd->m_startframe;
						s_befframe = s_sndkd->m_startframe;
						s_sndkd->GetBGMStopCnt( &s_bgmstopcnt );

					}	
					s_previewfirst = 1;
				}

//char strchk[256];
//sprintf_s( strchk, 256, "check !!!!! ThreadFUnc : previewframe %d, current %d, s_frameno %d\r\n", 
//	mpd->m_previewframe, mpd->m_current, s_frameno );
//DbgOut( strchk );
				if( s_mpd && s_mpd->IsWindow() ){
					if( s_mpd && (s_mpd->m_withsndflag == 0) ){
						if( s_mpd && (s_mpd->m_previewmoa == 0) ){
							s_mpd->SetNewPose();
						}else if( s_mpd && (s_mpd->m_previewmoa == 1) ){
							s_mpd->SetNewPoseByMoa();
						}
					}else if( s_mpd && (s_mpd->m_withsndflag == 1) ){
						s_mpd->SetNewPoseSnd();
					}
					if( s_mpd && s_apphwnd )
						::SendMessage( s_apphwnd, WM_USER_DISPLAY, 0, 0 );
				}
				isfirst = 0;
			}else{
				isfirst = 1;
				DbgOut( "MotParamDlg : ThreadFunc : End\n" );
				return 0;//!!!!!!!!!!
			}

		}else{

			if( s_mpd && s_mpd->m_underdestroyflag == 0 ){
				if( ::MsgWaitForMultipleObjects( 1, &s_hEvent, FALSE, 500, 0 ) == WAIT_OBJECT_0 ){
					//if( mpd->m_underdestroyflag == 0 ){
						//BOOL dummy;
						//mpd->m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)mpd->m_previewframe );
						//mpd->OnReleasedcaptureSl_mp( 0, 0, dummy );

						//if( s_mpd->m_previewmotion == 1 ){
						/***
						if( s_mpd && s_mpd->IsWindow() && s_mpd->m_mhandler ){
							//s_mpd->m_current = s_mpd->m_previewframe;
							s_mpd->m_previewframe = s_mpd->m_current;
							ret = s_mpd->m_mhandler->SetMotionFrameNo( s_mpd->m_shandler, s_mpd->m_mot_cookie, s_mpd->m_current, 1 );
							_ASSERT( !ret );
							s_mpd->ParamsToDlg();
						}

						//if( s_mpd->m_previewcamera == 1 ){
						if( s_ckd && s_ckd->IsWindow() && (s_ckd->GetCameraAnimNum() > 0) ){
							//s_ckd->m_current = s_ckd->m_previewframe; 
							s_ckd->m_previewframe = s_ckd->m_current;
							s_ckd->SetCurrentMotion( 0 );
							s_ckd->ParamsToDlg();
						}

						//if( s_mpd->m_previewcamera == 1 ){
						if( s_gpkd && s_gpkd->IsWindow() && (s_gpkd->GetGPAnimNum() > 0) ){
							//s_gpkd->m_current = s_gpkd->m_previewframe;
							s_gpkd->m_previewframe = s_gpkd->m_current;
							s_gpkd->SetCurrentMotion( 0 );
							s_gpkd->ParamsToDlg();
						}

						if( s_moekd && s_moekd->IsWindow() && (s_moekd->GetMOEAnimNum() > 0) ){
							CMOEAnim* anim;
							anim = s_moekd->m_moeah->GetCurMOEAnim();
							if( anim ){
								if( anim->m_firstkey ){
									//s_moekd->m_current = s_moekd->m_previewframe; 
									s_moekd->m_previewframe = s_moekd->m_current;
									s_moekd->SetCurrentMotion( 0 );
									s_moekd->ParamsToDlg();
								}
							}
						}
						***/
						if( s_sndkd && s_sndkd->IsWindow() && (s_sndkd->GetSndAnimNum() > 0) ){
							CSndAnim* anim;
							anim = s_sndkd->m_sndah->GetCurSndAnim();
							if( anim ){
								s_sndkd->StopBGM();
								//s_sndkd->m_current = s_moekd->m_previewframe;
								s_moekd->m_previewframe = s_sndkd->m_current;
								//s_sndkd->SetCurrentMotion( 0 );
								//s_sndkd->ParamsToDlg();
							}
						}
					
						if( s_mpd && ::IsWindow( s_mpd->m_hWnd ) && (s_mpd->m_underdestroyflag == 0) ){
							ResetEvent( s_hEvent );
						}

						isfirst = 1;
					//}

				}
			}else{
				DbgOut( "MotParamDlg : ThreadFunc : End\n" );
				return 0;//!!!!!!!!!!!!
			}
		}
	}

	DbgOut( "MotParamDlg : ThreadFunc : End\n" );

	//_endthreadex( 0 );//<----ThreadFuncがreturnするときに、自動的に呼ばれる。

	return 0;
}

int CMotParamDlg::Conv5Digits( int srcseri, char* dstptr )
{
	//if( (srcseri < 0) || (srcseri > 9999) ){
	if( srcseri > 99999 ){
		DbgOut( "BoneFile : Conv4Digits : srcseri range error !!!\n" );
		_ASSERT( 0 );
		*dstptr = 0;
		return 1;
	}
	
	int minusflag;
	if( srcseri < 0 ){
		minusflag = 1;
		srcseri *= -1;//!!!!!!!!
	}else{
		minusflag = 0;
	}

	int zerohead;
	if( srcseri < 10 ){
		zerohead = 4;
	}else if( srcseri < 100 ){
		zerohead = 3;
	}else if( srcseri < 1000 ){
		zerohead = 2;
	}else if( srcseri < 10000 ){
		zerohead = 1;
	}else{
		zerohead = 0;
	}

	*dstptr = 0;//!!!!

	if( minusflag == 1 ){
		strcat_s( dstptr, 6, "-" );//!!!!!!!!!
	}

	int headno;
	for( headno = 0; headno < zerohead; headno++ ){
		strcat_s( dstptr, 6, "0" );
	}

	char numstr[6];
	sprintf_s( numstr, 6, "%d", srcseri );

	strcat_s( dstptr, 6, numstr );

	return 0;
}
int CMotParamDlg::WriteBmp( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont )
{
	
	LONG previewmotion = 1;
	LONG previewcamera = 1;
	LONG previewgp = 1;
	LONG previewmoe = 1;

	if( !m_thandler || !m_shandler || !m_mhandler ){
		previewmotion = 0;
	}
	if( m_motionnum <= 0 ){
		previewmotion = 0;
	}

	if( m_camkeydlg->m_cah->m_animnum <= 0 ){
		previewcamera = 0;
	}
	if( m_camkeydlg->m_animno < 0 ){
		previewcamera = 0;
	}

	if( m_gpkeydlg->m_gpah->m_animnum <= 0 ){
		previewgp = 0;
	}
	if( m_gpkeydlg->m_animno < 0 ){
		previewgp = 0;
	}

	if( m_moekeydlg->m_moeah->m_animnum <= 0 ){
		previewmoe = 0;
	}
	if( m_moekeydlg->m_animno < 0 ){
		previewmoe = 0;
	}
	if( previewmoe == 1 ){//キーが１個もない場合は再生しない。
		CMOEAnim* anim;
		anim = m_moekeydlg->m_moeah->GetCurMOEAnim();
		if( anim ){
			if( !anim->m_firstkey ){
				previewmoe = 0;
			}
		}else{
			previewmoe = 0;
		}
	}
	if( previewmoe == 1 ){
		previewmotion = 0;//!!!!!!!!!
	}

	int maxplayframe = 0;
	if( previewmotion == 1 ){
		maxplayframe = max( maxplayframe, m_sl_mp_rangemax );
	}else{
		maxplayframe = max( maxplayframe, m_moekeydlg->m_sl_mp_rangemax );
	}
	if( previewcamera == 1 ){
		maxplayframe = max( maxplayframe, m_camkeydlg->m_sl_mp_rangemax );
	}
	if( previewgp == 1 ){
		maxplayframe = max( maxplayframe, m_gpkeydlg->m_sl_mp_rangemax );
	}
//////////
	CWriteBmpDlg dlg( maxplayframe );
	int ret;
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal();
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK )
		return 0;

	::SendMessage( m_apphwnd, WM_USER_CREATE_PB, 0, 0 );
	::SendMessage( m_apphwnd, WM_USER_SET_PB, 0, 0 );


	pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF0000FF, 1.0f, 0L );
	pFont->DrawText( 0, 250, D3DCOLOR_ARGB(255,255,255,0), "Under Writing BMP Files" );
	pd3dDevice->Present( NULL, NULL, NULL, NULL );


	LPDIRECT3DSURFACE9 pBack;
	HRESULT hr;
	hr = pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBack );
	if( hr != D3D_OK ){
		DbgOut( "motparamdlg : WriteBMP : GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	D3DSURFACE_DESC desc;
	hr = pBack->GetDesc( &desc );
	if( hr != D3D_OK ){
		DbgOut( "motparamdlg : WriteBMP : GetDesc error !!!\n" );
		_ASSERT( 0 );
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}
	int backformat;
	UINT backWidth, backHeight;
	backformat = desc.Format;
	backWidth = desc.Width;
	backHeight = desc.Height;

	D3DLOCKED_RECT lockedrect;
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = backWidth;
	rect.bottom = backHeight;

	int startframe = dlg.m_startframe;
	int endframe = dlg.m_endframe;
	DbgOut( "MotParamDlg : WriteBmp : startframe %d, endframe %d\n", startframe, endframe );

	int rate;

	int frameno;
	int framecnt = 0;
	for( frameno = startframe; frameno <= endframe; frameno++ ){

		if( endframe != startframe ){
			rate = framecnt * 100 / (endframe - startframe);
		}else{
			rate = 50;
		}
		::SendMessage( m_apphwnd, WM_USER_SET_PB, (WPARAM)rate, 0 );


		PlayAnim( frameno, 1 );
		if( previewcamera )
			m_camkeydlg->PlayAnim( frameno, 1 );
		if( previewgp )
			m_gpkeydlg->PlayAnim( frameno, 1 );
		if( previewmoe )
			m_moekeydlg->PlayAnim( frameno, 1 );
///////////////////


		LRESULT lres;
		lres = ::SendMessage( m_apphwnd, WM_USER_DISPLAY, 0, 0 );
		_ASSERT( !lres );

		//hr = pBack->LockRect( &lockedrect, &rect, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_READONLY );
		hr = pBack->LockRect( &lockedrect, &rect, 0 );
		if( hr != D3D_OK ){
			DbgOut( "motparamdlg : WriteBMP : pback LockRect error !!!\n" );		
			_ASSERT( 0 );
			pBack->Release();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}

		//DbgOut( "MotDlg:WriteBmp : width %d, height %d, pitch %d\n",
		//	backWidth, backHeight, lockedrect.Pitch );

		int bmpwidth, bmpheight;
		bmpheight = backHeight;
		int wcnt;
		wcnt = backWidth / 4;
		bmpwidth = wcnt * 4;

		char moduledir[2048];
		int leng;
		ZeroMemory( moduledir, 2048 );
		leng = GetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPTSTR)moduledir, 2048 );
		if( (leng <= 0) || (leng >= 2048) ){
			DbgOut( "motparamdlg : WriteBMP : GetEnvironmentVariable error !!!\n" );
			_ASSERT( 0 );
			pBack->UnlockRect();
			pBack->Release();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}

		char outputname[2048];
		char nameindex[10];
		ZeroMemory( outputname, 2048 );
		ZeroMemory( nameindex, 10 );
		//sprintf_s( nameindex, 10, "%d", frameno - startframe );
		Conv5Digits( frameno - startframe, nameindex );

		int chkleng;
		chkleng = leng + (int)strlen( dlg.m_prefix ) + (int)strlen( nameindex );

		if( chkleng >= 2044 ){
			DbgOut( "motparamdlg : WriteBMP : chkleng error !!! file name too long !!!\n" );
			_ASSERT( 0 );
			pBack->UnlockRect();
			pBack->Release();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );

			return 1;
		}

		strcpy_s( outputname, 2048, moduledir );
		strcat_s( outputname, 2048, dlg.m_prefix );
		strcat_s( outputname, 2048, nameindex );


		CDibFile* dibfile;
		dibfile = new CDibFile( outputname, bmpwidth, bmpheight, 24 );
		if( !dibfile ){
			DbgOut( "motparamdlg : WriteBMP : dibfile alloc error !!!\n" );
			_ASSERT( 0 );
			pBack->UnlockRect();
			pBack->Release();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}


		BYTE* bitshead;
		int hno;
		for( hno = 0; hno < bmpheight; hno++ ){
			//bitshead = (BYTE*)(lockedrect.pBits) + lockedrect.Pitch * hno;
			//上下反転する。
			bitshead = (BYTE*)(lockedrect.pBits) + lockedrect.Pitch * hno;
			ret = dibfile->SetRGB( bmpheight - 1 - hno, bitshead, backformat );
			if( ret ){
				DbgOut( "motparamdlg : WriteBMP : dibfile SetRGB error !!!\n" );
				_ASSERT( 0 );
				pBack->UnlockRect();
				pBack->Release();
				::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
				return 1;
			}
		}

		ret = dibfile->CreateDibFile();
		if( ret ){
			DbgOut( "motparamdlg : WriteBMP : dibfile CreateFile error !!!\n" );
			_ASSERT( 0 );
			pBack->UnlockRect();
			pBack->Release();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}
		delete dibfile;

		hr = pBack->UnlockRect();
		if( hr != D3D_OK ){
			DbgOut( "motparamdlg : WriteBMP : pback UnlockRect error !!!\n" );
			_ASSERT( 0 );
			pBack->Release();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}
		//pd3dDevice->Present( NULL, NULL, NULL, NULL );

		framecnt++;
	}
	pBack->Release();

	::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );

	switch( backformat ){
	case D3DFMT_X1R5G5B5:
		DbgOut( "MotDlg:WriteBmp : format X1R5G5B5\n" );
		break;
	case D3DFMT_R5G6B5:
		DbgOut( "MotDlg:WriteBmp : format R5G6B5\n" );
		break;
	case D3DFMT_X8R8G8B8:
		DbgOut( "MotDlg:WriteBmp : format X8R8G8B8\n" );
		break;
	case D3DFMT_A8R8G8B8:
		DbgOut( "MotDlg:WriteBmp : format A8R8G8B8\n" );
		break;
	default:
		break;
	}
	
	return 0;
}


int CMotParamDlg::WriteAvi( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont )
{
	LONG previewmotion = 1;
	LONG previewcamera = 1;
	LONG previewgp = 1;
	LONG previewmoe = 1;

	if( !m_thandler || !m_shandler || !m_mhandler ){
		previewmotion = 0;
	}
	if( m_motionnum <= 0 ){
		previewmotion = 0;
	}

	if( m_camkeydlg->m_cah->m_animnum <= 0 ){
		previewcamera = 0;
	}
	if( m_camkeydlg->m_animno < 0 ){
		previewcamera = 0;
	}

	if( m_gpkeydlg->m_gpah->m_animnum <= 0 ){
		previewgp = 0;
	}
	if( m_gpkeydlg->m_animno < 0 ){
		previewgp = 0;
	}

	if( m_moekeydlg->m_moeah->m_animnum <= 0 ){
		previewmoe = 0;
	}
	if( m_moekeydlg->m_animno < 0 ){
		previewmoe = 0;
	}
	if( previewmoe == 1 ){//キーが１個もない場合は再生しない。
		CMOEAnim* anim;
		anim = m_moekeydlg->m_moeah->GetCurMOEAnim();
		if( anim ){
			if( !anim->m_firstkey ){
				previewmoe = 0;
			}
		}else{
			previewmoe = 0;
		}
	}
	if( previewmoe == 1 ){
		previewmotion = 0;//!!!!!!!!!
	}

	int maxplayframe = 0;
	if( previewmotion == 1 ){
		maxplayframe = max( maxplayframe, m_sl_mp_rangemax );
	}else{
		maxplayframe = max( maxplayframe, m_moekeydlg->m_sl_mp_rangemax );
	}
	if( previewcamera == 1 ){
		maxplayframe = max( maxplayframe, m_camkeydlg->m_sl_mp_rangemax );
	}
	if( previewgp == 1 ){
		maxplayframe = max( maxplayframe, m_gpkeydlg->m_sl_mp_rangemax );
	}

/////////
	CWriteBmpDlg dlg( maxplayframe );
	int ret;
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal();
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK )
		return 0;

	::SendMessage( m_apphwnd, WM_USER_CREATE_PB, 0, 0 );
	::SendMessage( m_apphwnd, WM_USER_SET_PB, 0, 0 );

	pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF0000FF, 1.0f, 0L );
	pFont->DrawText( 0, 250, D3DCOLOR_ARGB(255,255,255,0), "Under Writing AVI File" );
	pd3dDevice->Present( NULL, NULL, NULL, NULL );


	LPDIRECT3DSURFACE9 pBack;
	HRESULT hr;
	hr = pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBack );
	if( hr != D3D_OK ){
		DbgOut( "motparamdlg : WriteAVI : GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}
	D3DSURFACE_DESC desc;
	hr = pBack->GetDesc( &desc );
	if( hr != D3D_OK ){
		DbgOut( "motparamdlg : WriteAVI : back GetDesc error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}
	int backformat;
	UINT backWidth, backHeight;
	backformat = desc.Format;
	backWidth = desc.Width;
	backHeight = desc.Height;

	D3DLOCKED_RECT lockedrect;
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = backWidth;
	rect.bottom = backHeight;

	int startframe = dlg.m_startframe;
	int endframe = dlg.m_endframe;
	DbgOut( "MotParamDlg : WriteAvi : startframe %d, endframe %d\n", startframe, endframe );


//////////////////
	//avi fileの作成に必要なので、ここで、サーフェスの大きさを求める。
	//書き出し中は、サーフェスの大きさ変更禁止！！！！！！


	int bmpwidth, bmpheight;
	bmpheight = backHeight;
	int wcnt;
	wcnt = backWidth / 4;// !!!!!!! 4の倍数に切りつめちゃう。
	bmpwidth = wcnt * 4;

	int frameleng;
	frameleng = endframe - startframe + 1;


	CDibFile* firstdib;
	firstdib = new CDibFile( "temp", bmpwidth, bmpheight, 24 );
	if( !firstdib ){
		DbgOut( "motparamdlg : WriteAVI : firstdib alloc error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}
	
	BITMAPINFOHEADER* firstbih;
	firstbih = firstdib->GetBMPInfoHeader();
	if( !firstbih ){
		DbgOut( "motparamdlg : WriteAVI : firstdib error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}

	/***
	char aviname[1024];
	ZeroMemory( aviname, 1024 );
	int leng;
	leng = (int)strlen( dlg.m_prefix );
	if( leng <= 1018 ){
		strcpy( aviname, dlg.m_prefix );
		strcat( aviname, ".avi" );
	}else{
		strncpy( aviname, dlg.m_prefix, 1018 );
		strcat( aviname, ".avi" );
	}
	***/

	char moduledir[2048];
	int leng;
	ZeroMemory( moduledir, 2048 );
	leng = GetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPTSTR)moduledir, 2048 );
	if( (leng <= 0) || (leng >= 2048) ){
		DbgOut( "motparamdlg : WriteAVI : GetEnvironmentVariable error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}

	char aviname[2048];
		ZeroMemory( aviname, 2048 );

	int chkleng;
	chkleng = leng + (int)strlen( dlg.m_prefix ) + 4;

	if( chkleng >= 2048 ){
		DbgOut( "motparamdlg : WriteAVI : chkleng error !!! file name too long !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}

	strcpy_s( aviname, 2048, moduledir );
	strcat_s( aviname, 2048, dlg.m_prefix );
	strcat_s( aviname, 2048, ".avi" );

//g_inidata->opt_previewfps
	//avifile
    AVICOMPRESSOPTIONS opt;
    AVISTREAMINFO si={streamtypeVIDEO,comptypeDIB,0,0,0,0,
		1,g_inidata->opt_previewfps,
		0,frameleng,0,0,(DWORD)-1,0,{0,0,bmpwidth,bmpheight},0,0,_T("Video by rdb2")};

	COMPVARS cv;
    PAVIFILE pavi = 0;
    PAVISTREAM pstm = 0;
	PAVISTREAM ptmp = 0;

    AVIFileInit();

    memset(&cv,0,sizeof(COMPVARS));
    cv.cbSize=sizeof(COMPVARS);
    cv.dwFlags=ICMF_COMPVARS_VALID;//<----- ほんとは、ここはセットしない方が良い。
    cv.fccHandler=comptypeDIB;
    cv.lQ=ICQUALITY_DEFAULT;
	if (!ICCompressorChoose(NULL,ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME,firstbih,NULL,&cv,NULL)){
		DbgOut( "motparamdlg : WriteAVI : ICCompressorChose error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
        return 1;
	}
    si.fccHandler=cv.fccHandler;
    opt.fccType=streamtypeVIDEO;
    opt.fccHandler=cv.fccHandler;
    opt.dwKeyFrameEvery=cv.lKey;
    opt.dwQuality=cv.lQ;
    opt.dwBytesPerSecond=cv.lDataRate;
    opt.dwFlags=(cv.lDataRate>0?AVICOMPRESSF_DATARATE:0)|(cv.lKey>0?AVICOMPRESSF_KEYFRAMES:0);
    opt.lpFormat=NULL;
    opt.cbFormat=0;
    opt.lpParms=cv.lpState;
    opt.cbParms=cv.cbState;
    opt.dwInterleaveEvery=0;

    //opt.lpParms=0;
    //opt.cbParms=0;


/***
{
	DbgOut( "WriteAVI : prefix %s\n", dlg.m_prefix );
	DbgOut( "WriteAVI : cv.fccHandler %c %c %c %c\n",
		(cv.fccHandler >> 24) & 0x000000FF, (cv.fccHandler >> 16) & 0x000000FF, (cv.fccHandler >> 8) & 0x000000FF, cv.fccHandler & 0x000000FF );
	DbgOut( "WriteAVI : cv.lKey %d\n", cv.lKey );
	DbgOut( "WriteAVI : cv.lQ %d  default %d\n", cv.lQ, ICQUALITY_DEFAULT );
	DbgOut( "WriteAVI : cv.lDataRate %d\n", cv.lDataRate );
	DbgOut( "WriteAVI : cv.lpState %d\n", cv.lpState );
	if( cv.lpState ){
		DbgOut( "WriteAVI : *cv.lpState 0x%x\n", *((unsigned long*)cv.lpState) );
		//DbgOut( "0x%x---0x%x---0x%x\n", *((unsigned long*)cv.lpState + 1), *((unsigned long*)cv.lpState + 2), *((unsigned long*)cv.lpState + 3) );
	}
	DbgOut( "WriteAVI : cv.cbState %d\n", cv.cbState );
	DbgOut( "\n\n" );
}
***/

	if (AVIFileOpen(&pavi,aviname,OF_CREATE | OF_WRITE | OF_SHARE_DENY_NONE,NULL)!=0){
		DbgOut( "motparamdlg : WriteAVI : AVIFileOpen error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;

		if( ptmp )
			AVIStreamRelease(ptmp);
		if( pstm )   
		AVIStreamRelease(pstm);
		//if( pavi )
		//AVIFileRelease(pavi);
		ICCompressorFree(&cv);
	    //AVIFileExit();
		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
        return 1;
	}
	if (AVIFileCreateStream(pavi,&pstm,&si)!=0){
		DbgOut( "motparamdlg : WriteAVI : AVIFileCreateStream error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;

		if( ptmp )
			AVIStreamRelease(ptmp);
		if( pstm )   
		AVIStreamRelease(pstm);
		if( pavi )
		AVIFileRelease(pavi);
		ICCompressorFree(&cv);
	    AVIFileExit();

		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
        return 1;
	}
	if (AVIMakeCompressedStream(&ptmp,pstm,&opt,NULL)!=AVIERR_OK){
		DbgOut( "motparamdlg : WriteAVI : AVIMakeCompressedStream error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;

		if( ptmp )
			AVIStreamRelease(ptmp);
		if( pstm )   
		AVIStreamRelease(pstm);
		if( pavi )
		AVIFileRelease(pavi);
		ICCompressorFree(&cv);
	    AVIFileExit();

		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}
	if (AVIStreamSetFormat(ptmp,0,firstbih,sizeof(BITMAPINFOHEADER))!=0){
		DbgOut( "motparamdlg : WriteAVI : AVIStreamSetFormat error !!!\n" );
		_ASSERT( 0 );
		pBack->Release();
		delete firstdib;

		if( ptmp )
			AVIStreamRelease(ptmp);
		if( pstm )   
		AVIStreamRelease(pstm);
		if( pavi )
		AVIFileRelease(pavi);
		ICCompressorFree(&cv);
	    AVIFileExit();

		::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
		return 1;
	}

	////

	delete firstdib;//!!!!!!!


	int rate;

	int frameno;
	int framecnt = 0;
	for( frameno = startframe; frameno <= endframe; frameno++ ){

		if( endframe != startframe ){
			rate = framecnt * 100 / (endframe - startframe);
		}else{
			rate = 50;
		}
		::SendMessage( m_apphwnd, WM_USER_SET_PB, (WPARAM)rate, 0 );

//		BOOL dummy;
//		m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)frameno );
//		OnReleasedcaptureSl_mp( 0, 0, dummy );

		PlayAnim( frameno, 1 );
		if( previewcamera )
			m_camkeydlg->PlayAnim( frameno, 1 );
		if( previewgp )
			m_gpkeydlg->PlayAnim( frameno, 1 );
		if( previewmoe )
			m_moekeydlg->PlayAnim( frameno, 1 );


		LRESULT lres;
		lres = ::SendMessage( m_apphwnd, WM_USER_DISPLAY, 0, 0 );
		_ASSERT( !lres );

		char outputname[1024];
		char nameindex[10];
		ZeroMemory( outputname, 1024 );
		ZeroMemory( nameindex, 10 );
		sprintf_s( nameindex, 10, "%d", frameno - startframe );
		int leng;
		leng = (int)strlen( dlg.m_prefix );
		if( leng <= 1016 ){
			strcpy_s( outputname, 1017, dlg.m_prefix );
			strcat_s( outputname, 1024, nameindex );
		}else{
			strncpy_s( outputname, 1024, dlg.m_prefix, 1016 );
			strcat_s( outputname, 1024, nameindex );
		}

		CDibFile* dibfile;
		dibfile = new CDibFile( outputname, bmpwidth, bmpheight, 24 );
		if( !dibfile ){
			DbgOut( "motparamdlg : WriteAVI : dibfile alloc error !!!\n" );
			_ASSERT( 0 );
			pBack->Release();

			if( ptmp )
				AVIStreamRelease(ptmp);
			if( pstm )   
			AVIStreamRelease(pstm);
			if( pavi )
			AVIFileRelease(pavi);
			ICCompressorFree(&cv);
			AVIFileExit();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}


		hr = pBack->LockRect( &lockedrect, &rect, 0 );
		if( hr != D3D_OK ){
			DbgOut( "motparamdlg : WriteAVI : back LockRect error !!!\n" );
			_ASSERT( 0 );
			pBack->Release();
			delete dibfile;

			if( ptmp )
				AVIStreamRelease(ptmp);
			if( pstm )   
			AVIStreamRelease(pstm);
			if( pavi )
			AVIFileRelease(pavi);
			ICCompressorFree(&cv);
			AVIFileExit();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}


		BYTE* bitshead;
		int hno;
		for( hno = 0; hno < bmpheight; hno++ ){
			//bitshead = (BYTE*)(lockedrect.pBits) + lockedrect.Pitch * hno;
			//上下反転する。
			bitshead = (BYTE*)(lockedrect.pBits) + lockedrect.Pitch * hno;
			ret = dibfile->SetRGB( bmpheight - 1 - hno, bitshead, backformat );
			if( ret ){
				DbgOut( "motparamdlg : WriteAVI : dibfile SetRGB error !!!\n" );
				_ASSERT( 0 );
				pBack->UnlockRect();
				pBack->Release();

				if( ptmp )
					AVIStreamRelease(ptmp);
				if( pstm )   
				AVIStreamRelease(pstm);
				if( pavi )
				AVIFileRelease(pavi);
				ICCompressorFree(&cv);
				AVIFileExit();
				::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
				return 1;
			}
		}

		//BITMAPINFOHEADER* pbmih;
		//pbmih = dibfile->GetBMPInfoHeader();
		//_ASSERT( pbmih );

		BYTE* pBit;
		pBit = dibfile->GetBMPData();
		_ASSERT( pBit );

		if (AVIStreamWrite(ptmp,framecnt,1,pBit,dibfile->sizeimage, AVIIF_KEYFRAME,NULL,NULL)!=0){
			DbgOut( "motparamdlg : WriteAVI : AVIStreamWrite error !!!\n" );
			_ASSERT( 0 );
			pBack->UnlockRect();
			pBack->Release();
			
			delete dibfile;

			if( ptmp )
				AVIStreamRelease(ptmp);
			if( pstm )   
			AVIStreamRelease(pstm);
			if( pavi )
			AVIFileRelease(pavi);
			ICCompressorFree(&cv);
			AVIFileExit();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}
		
		delete dibfile;

		hr = pBack->UnlockRect();
		if( hr != D3D_OK ){
			DbgOut( "motparamdlg : WriteAVI : back UnlockRect error !!!\n" );
			_ASSERT( 0 );
			pBack->Release();

			if( ptmp )
				AVIStreamRelease(ptmp);
			if( pstm )   
			AVIStreamRelease(pstm);
			if( pavi )
			AVIFileRelease(pavi);
			ICCompressorFree(&cv);
			AVIFileExit();
			::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );
			return 1;
		}
		//pd3dDevice->Present( NULL, NULL, NULL, NULL );

		framecnt++;
	}
	pBack->Release();

	if( ptmp )
		AVIStreamRelease(ptmp);
	if( pstm )   
	AVIStreamRelease(pstm);
	if( pavi )
    AVIFileRelease(pavi);
    ICCompressorFree(&cv);
    AVIFileExit();

	::SendMessage( m_apphwnd, WM_USER_DESTROY_PB, 0, 0 );

	switch( backformat ){
	case D3DFMT_X1R5G5B5:
		DbgOut( "MotDlg:WriteAvi : format X1R5G5B5\n" );
		break;
	case D3DFMT_R5G6B5:
		DbgOut( "MotDlg:WriteAvi : format R5G6B5\n" );
		break;
	case D3DFMT_X8R8G8B8:
		DbgOut( "MotDlg:WriteAvi : format X8R8G8B8\n" );
		break;
	case D3DFMT_A8R8G8B8:
		DbgOut( "MotDlg:WriteAvi : format A8R8G8B8\n" );
		break;
	default:
		break;
	}
	
	return 0;
}



int CMotParamDlg::CreateMotionPoints( int srcseri, int motcookie, int frameno )
{
	if( m_mhandler->s2mot_leng < 1 ){
		return 0;//!!!!!!!!
	}

	int ret;
	int elemno;//, framenum;
	CMotionCtrl* mcptr;
	CMotionPoint2* curmp;
	CShdElem* selem;

	CQuaternion initq;
	initq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

	m_mhandler->GetDefInterp( m_mot_cookie, &m_interp );

	if( srcseri < 0 ){

		for( elemno = 1; elemno < m_mhandler->s2mot_leng; elemno++ ){
			mcptr = (*m_mhandler)( elemno );
			if( !mcptr )
				break;

			selem = (*m_shandler)( elemno );
			_ASSERT( selem );

			if( mcptr->IsJoint() && (mcptr->type != SHDMORPH)){
				curmp = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
				if( !curmp ){

					CMotionPoint2 tmpmp;
					int hasmpflag;
					ret = mcptr->CalcMotionPointOnFrame( selem, &tmpmp, m_mot_cookie, frameno, &hasmpflag, m_shandler, m_mhandler );
					if( ret ){
						_ASSERT( 0 );
						ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : CreateMotionPoints : CalcMotionPointOnFrame error" );
						return -1;
					}					

					CMotionPoint2* newmp;
					newmp = mcptr->AddMotionPoint( m_mot_cookie, frameno, tmpmp.m_q, tmpmp.m_mvx, tmpmp.m_mvy, tmpmp.m_mvz,
						m_dispswitch, m_interp, tmpmp.m_scalex, tmpmp.m_scaley, tmpmp.m_scalez, 0 );
					if( !newmp ){
						_ASSERT( 0 );
						ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : CreateMotionPoints : AddMotionPoint error" );
						return -1;
					}

					::SendMessage( m_apphwnd, WM_USER_CALCEUL, 0, 0 );

				}		
			}
		}
	}else{

		//１個だけ作成

		mcptr = (*m_mhandler)( srcseri );
		if( !mcptr ){
			_ASSERT( 0 );
			return 1;
		}

		selem = (*m_shandler)( srcseri );
		_ASSERT( selem );

		if( mcptr->IsJoint() && (mcptr->type != SHDMORPH)){
			curmp = mcptr->IsExistMotionPoint( m_mot_cookie, frameno );
			if( !curmp ){
				CMotionPoint2 tmpmp;
				int hasmpflag;
				ret = mcptr->CalcMotionPointOnFrame( selem, &tmpmp, m_mot_cookie, frameno, &hasmpflag, m_shandler, m_mhandler );
				if( ret ){
					_ASSERT( 0 );
					ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : CreateMotionPoints : CalcMotionPointOnFrame error" );
					return -1;
				}					

				CMotionPoint2* newmp;
				newmp = mcptr->AddMotionPoint( m_mot_cookie, frameno, tmpmp.m_q, tmpmp.m_mvx, tmpmp.m_mvy, tmpmp.m_mvz,
					m_dispswitch, m_interp, tmpmp.m_scalex, tmpmp.m_scaley, tmpmp.m_scalez, 0 );
				if( !newmp ){
					_ASSERT( 0 );
					ErrorMes( -1, MSGERR_APPMUSTEXIT, "MotParamDlg : CreateMotionPoints : AddMotionPoint error" );
					return -1;
				}

				::SendMessage( m_apphwnd, WM_USER_CALCEUL, 0, 0 );

			}		
		}

	}
		
	return 0;
}



LRESULT CMotParamDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}
		
	bHandled = TRUE;

	if( m_timerworking != 1 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;

	int timerid = (int)wParam;
	if( (timerid == MOTPARAMDLGTIMER) && (m_cmdshow != SW_HIDE) ){

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

	}

	return 0;
}


int CMotParamDlg::SetRotation( D3DXVECTOR3 axis, int delta, int mvboneno )
{

	int ret;

	if( m_optardlg )
		m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );

	float rad;
	if( delta > 0 ){
		rad = (float)m_rotrad * (float)DEG2PAI;
	}else{
		rad = (float)m_rotrad * (float)(-DEG2PAI);
	}

	CMotionCtrl* mcptr = (*m_mhandler)( mvboneno );//注意！！引数は、m_selectednoではい。！！
	_ASSERT( mcptr );
		
	CQuaternion multq;
	ret = multq.SetAxisAndRot( axis, rad );
	_ASSERT( !ret );

	int docre, haskey;
	ret = m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}


	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		//m_q = multq * curmp->m_q;
		//m_q = curmp->m_q * multq;

		CQuaternion aftq;
		aftq = curmp->m_q * multq;

		int startout = 0;
		D3DXVECTOR3 neweul;
		ret = g_bonepropdlg->GetLimitedQ( mvboneno, &curmp->m_q, &curmp->m_eul, &aftq, &m_q, &startout, &neweul, 0, 0 );
		if( ret ){
			DbgOut( "motparamdlg : SetRotation : GetLimitedQ error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );
		curmp->m_eul = neweul;//!!!!!!!!!

		ret = SetQOpTarget( mvboneno, multq, m_q, 1 );
		_ASSERT( !ret );

	}else if( docre == 1 ){

		ret = CreateMotionPoints( mcptr->serialno, m_mot_cookie, m_current );
		if( ret )
			return -1;

		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
		if( !curmp )
			return -1;
		
		//m_q = multq * curmp->m_q;
		//m_q = curmp->m_q * multq;

		CQuaternion aftq;
		aftq = curmp->m_q * multq;
		int startout = 0;
		D3DXVECTOR3 neweul;
		ret = g_bonepropdlg->GetLimitedQ( mvboneno, &curmp->m_q, &curmp->m_eul, &aftq, &m_q, &startout, &neweul, 0, 0 );
		if( ret ){
			DbgOut( "motparamdlg : SetRotation : GetLimitedQ error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		m_mvx = curmp->m_mvx;
		m_mvy = curmp->m_mvy;
		m_mvz = curmp->m_mvz;
		//m_dispswitch = curmp->dispswitch;
		m_interp = curmp->interp;

		m_scalex = curmp->m_scalex;
		m_scaley = curmp->m_scaley;
		m_scalez = curmp->m_scalez;
		m_userint1 = curmp->m_userint1;
		//m_q = multq;
		//m_mvx = 0.0f;
		//m_mvy = 0.0f;
		//m_mvz = 0.0f;
		ret = curmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
		_ASSERT( !ret );
		curmp->m_eul = neweul;//!!!!!!!!!!!!!!

		ret = SetQOpTarget( mvboneno, multq, m_q, 1 );
		_ASSERT( !ret );

	}

	//UpdateAllMatrix();
	UpdateMatrixSelected( mvboneno, 0, 0 );

	Redraw();

	return 0;
}

int CMotParamDlg::RotateBone( D3DXMATRIX matWorld, int mvno, int rotkind, float delta, int* lastparent, int localflag, CQuaternion localq, int ikfkflag, int za4flag )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( !m_d3dapp )
		return 0;


	if( m_optardlg )
		m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );

	int ret;

	*lastparent = 0;

	D3DXVECTOR3 axisdone;

	if( ikfkflag == 0 ){
		//IK

		
		int movecntmax;
		if( m_ikmode == IKMODE_JOINT ){
			movecntmax = m_iklevel;
		}else{
			movecntmax = m_iklevelope;
		}
		
		int iktopparent;
		ret = GetIKTopParent( mvno, movecntmax, &iktopparent );
		if( ret ){
			DbgOut( "RotateBone : GetIKTopParent error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( iktopparent < 0 ){
			//_ASSERT( 0 );
			return 0;
		}
		

		/***
		int iktopparent;
		CShdElem* ikparent;
		ikparent = m_shandler->FindUpperJoint( (*m_shandler)( mvno ) );
		if( ikparent ){
			iktopparent = ikparent->serialno;
		}else{
			return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
		***/

		D3DXVECTOR3 mvobj, parobj;

		ret = GetBonePos( mvno, matWorld, 1, &mvobj );
		if( ret ){
			DbgOut( "RotateBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}

		ret = GetBonePos( iktopparent, matWorld, 1, &parobj );
		if( ret ){
			DbgOut( "RotateBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}

		D3DXMATRIX beftramat, afttramat, rotmat;
		D3DXMatrixIdentity( &beftramat );
		D3DXMatrixIdentity( &afttramat );
		D3DXMatrixIdentity( &rotmat );

		beftramat._41 = -parobj.x;
		beftramat._42 = -parobj.y;
		beftramat._43 = -parobj.z;

		afttramat._41 = parobj.x;
		afttramat._42 = parobj.y;
		afttramat._43 = parobj.z;

		D3DXVECTOR3 axis;
		switch( rotkind ){
		case 0:
			if( localflag == 0 ){
				D3DXMatrixRotationX( &rotmat, delta );
				axis.x = 1.0f;
				axis.y = 0.0f;
				axis.z = 0.0f;
			}else{			
				D3DXVECTOR3 befaxis( 1.0f, 0.0f, 0.0f );
				localq.Rotate( &axis, befaxis );

				CQuaternion rotq;
				if( delta != 0.0f ){
					rotq.SetAxisAndRot( axis, -delta );
					rotmat = rotq.MakeRotMatX();
				}else{
					D3DXMatrixIdentity( &rotmat );
				}
			}
			break;
		case 1:
			if( localflag == 0 ){
				D3DXMatrixRotationY( &rotmat, delta );
				axis.x = 0.0f;
				axis.y = 1.0f;
				axis.z = 0.0f;
			}else{
				D3DXVECTOR3 befaxis( 0.0f, 1.0f, 0.0f );
				localq.Rotate( &axis, befaxis );

				CQuaternion rotq;
				if( delta != 0.0f ){
					rotq.SetAxisAndRot( axis, -delta );
					rotmat = rotq.MakeRotMatX();
				}else{
					D3DXMatrixIdentity( &rotmat );
				}
			}
			
			break;
		case 2:
			if( localflag == 0 ){
				D3DXMatrixRotationZ( &rotmat, delta );
				axis.x = 0.0f;
				axis.y = 0.0f;
				axis.z = 1.0f;
			}else{
				D3DXVECTOR3 befaxis( 0.0f, 0.0f, 1.0f );
				localq.Rotate( &axis, befaxis );

				CQuaternion rotq;
				if( delta != 0.0f ){
					rotq.SetAxisAndRot( axis, -delta );
					rotmat = rotq.MakeRotMatX();
				}else{
					D3DXMatrixIdentity( &rotmat );
				}
			}
		
			break;
		default:
			_ASSERT( 0 );
			if( localflag == 0 ){
				D3DXMatrixRotationX( &rotmat, delta );
				axis.x = 1.0f;
				axis.y = 0.0f;
				axis.z = 0.0f;
			}else{
				D3DXVECTOR3 befaxis( 1.0f, 0.0f, 0.0f );
				localq.Rotate( &axis, befaxis );

				CQuaternion rotq;
				rotq.SetAxisAndRot( axis, -delta );
				rotmat = rotq.MakeRotMatX();
			}
			break;
		}
		DXVec3Normalize( &axis, &axis );


		D3DXVECTOR3 farmvobj, diff;
		diff = mvobj - parobj;

//		D3DXVECTOR3 ndiff;
//		DXVec3Normalize( &ndiff, &diff );
//		float axdot;
//		axdot = D3DXVec3Dot( &ndiff, &axis );
//		
//		if( (axdot <= 0.9961946f) && (axdot >= -0.9961946f) ){
			farmvobj = parobj + 1.3f * diff;
			//farmvobj = mvobj;

			D3DXVECTOR3 newtarget;
			D3DXMATRIX mat;

			mat = beftramat * rotmat * afttramat;
			D3DXVec3TransformCoord( &newtarget, &farmvobj, &mat );

			ret = MoveBone( matWorld, mvno, newtarget, lastparent, 1, axis, &axisdone, -1 );
			if( ret ){
				DbgOut( "motparamdlg : RotateBone : MoveBone error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
//		}
//		
//		
//		else{
//			//axisとボーンが5度未満の場合
//			CQuaternion multq;
//			delta *= 10.0f;
//			multq.SetAxisAndRot( axis, -delta );
//
//			int forcemove = 0;
//			ret = SetMotionPointIK( matWorld, mvno, multq, 1, &forcemove );
//			_ASSERT( !ret );
//
//			*lastparent = iktopparent;
//
//			m_d3dapp->SetBoneArray( *lastparent );
//			m_d3dapp->FrameMove( *lastparent, 1 );
//
//			if( fabs( delta ) >= 1.0f ){
//				_ASSERT( 0 );
//			}
//
//		}

	}else{
		//FK

		int docre, haskey;
		ret = m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
		if( m_notcreate == 0 ){
			docre = 1;
		}else{
			if( haskey ){
				docre = 1;
			}else{
				docre = 0;
			}
		}

		if( (docre == 0) && (haskey == 0) ){
			return 0;
		}
		

		CQuaternion multq;
		D3DXVECTOR3 xvec( 1.0f, 0.0f, 0.0f );
		D3DXVECTOR3 yvec( 0.0f, 1.0f, 0.0f );
		D3DXVECTOR3 zvec( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 localvec;

		if( localflag == 0 ){
			switch( rotkind ){
			case 0:
				multq.SetAxisAndRot( xvec, -delta );
				break;
			case 1:
				multq.SetAxisAndRot( yvec, -delta );
				break;
			case 2:
				multq.SetAxisAndRot( zvec, -delta );
				break;
			default:
				_ASSERT( 0 );
				multq.SetAxisAndRot( xvec, -delta );
				break;
			}
		}else{
			switch( rotkind ){
			case 0:
				localq.Rotate( &localvec, xvec );
				multq.SetAxisAndRot( localvec, -delta );
				break;
			case 1:
				localq.Rotate( &localvec, yvec );
				multq.SetAxisAndRot( localvec, -delta );
				break;
			case 2:
				localq.Rotate( &localvec, zvec );
				multq.SetAxisAndRot( localvec, -delta );
				break;
			default:
				_ASSERT( 0 );
				localq.Rotate( &localvec, xvec );
				multq.SetAxisAndRot( localvec, -delta );
				break;
			}
		}

		CShdElem* chilelem;
		chilelem = (*m_shandler)( mvno )->child;
		while( chilelem ){

			if( chilelem->IsJoint() && (chilelem->type != SHDMORPH) ){
				int forcemove = 0;
				ret = SetMotionPointIK( m_current, matWorld, chilelem->serialno, multq, 1, &forcemove, 0 );
				_ASSERT( !ret );

				CQuaternion setq;
				ret = GetStandardQ( chilelem->serialno, &setq );
				_ASSERT( !ret );
				ret = SetQOpTarget( chilelem->serialno, multq, setq, 1 );
				_ASSERT( !ret );
			}

			chilelem = chilelem->brother;
		}

		UpdateAllMatrix( 0 );

		*lastparent = mvno;

		m_d3dapp->SetBoneArray( *lastparent );
		m_d3dapp->FrameMove( *lastparent, 1 );

	}

	if( *lastparent > 0 ){
		SetParBoneArray( mvno );

		if( za4flag == 0 ){
			Rotate2IKTargetReq( matWorld, *lastparent, axisdone, 0 );
		}else{
			//ret = m_shandler->CalcZa4LocalQ( m_mhandler, m_mot_cookie, m_current );
			//_ASSERT( !ret );
			Rotate2IKTargetZa4Req( matWorld, *lastparent, 0 );
		}
	}

	Redraw();


	return 0;
}

/***
int CMotParamDlg::RotateBone( D3DXMATRIX matWorld, int mvno, int rotkind, float delta )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( !m_d3dapp )
		return 0;

	int ret;
	CQuaternion multq;
	D3DXVECTOR3 xvec( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 yvec( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 zvec( 0.0f, 0.0f, 1.0f );

	switch( rotkind ){
	case 0:
		multq.SetAxisAndRot( xvec, delta );
		break;
	case 1:
		multq.SetAxisAndRot( yvec, delta );
		break;
	case 2:
		multq.SetAxisAndRot( zvec, delta );
		break;
	default:
		_ASSERT( 0 );
		multq.SetAxisAndRot( xvec, delta );
		break;
	}

	ret = SetMotionPointIK( matWorld, mvno, multq, 1 );
	_ASSERT( !ret );


	//！！！boneのスクリーン座標を更新するために、FrameMove	が必要！！！
	m_d3dapp->SetBoneArray( mvno );
	m_d3dapp->FrameMove( mvno, 1 );


	return 0;
}
***/


LRESULT CMotParamDlg::OnSpin_Sp_rotx(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( m_rotkind == ROTKIND_NEW ){

		int mvboneno;
		if( m_sellockno != 0 )
			mvboneno = m_sellockno;
		else
			mvboneno = m_selectedno;

		if( mvboneno == 0 )
			return 0;

		bHandled = FALSE;

		NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
		int delta = pnmud->iDelta;

		int ret;
		D3DXVECTOR3 axis( 1.0f, 0.0f, 0.0f );


		ret = SetRotation( axis, delta, mvboneno );
		if( ret ){
			DbgOut( "MotParamDlg : OnSpin_Sp_rotx : SetRotation error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int tmpmvboneno, mvboneno;

		if( m_sellockno != 0 )
			tmpmvboneno = m_sellockno;
		else
			tmpmvboneno = m_selectedno;

		if( tmpmvboneno > 0 ){
			CShdElem* selelem;
			selelem = (*m_shandler)( tmpmvboneno );
			CShdElem* chilelem;
			chilelem = selelem->child;
			if( chilelem ){
				mvboneno = chilelem->serialno;
			}else{
				mvboneno = 0;
			}
		}else{
			mvboneno = 0;
		}

		if( mvboneno == 0 )
			return 0;

		bHandled = FALSE;

		NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
		int delta = pnmud->iDelta;

		int ret;
		D3DXVECTOR3 axis( 1.0f, 0.0f, 0.0f );

		while( mvboneno > 0 ){

			ret = SetRotation( axis, delta, mvboneno );
			if( ret ){
				DbgOut( "MotParamDlg : OnSpin_Sp_rotx : SetRotation error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			CShdElem* curelem;
			curelem = (*m_shandler)( mvboneno );
			CShdElem* broelem;
			broelem = curelem->brother;
			if( broelem ){
				mvboneno = broelem->serialno;
			}else{
				mvboneno = 0;
			}
		}

	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
		UpdateAllMatrix( 0 );
		Redraw();
	}



	return 0;

}

LRESULT CMotParamDlg::OnSpin_Sp_roty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( m_rotkind == ROTKIND_NEW ){
		int mvboneno;
		if( m_sellockno != 0 )
			mvboneno = m_sellockno;
		else
			mvboneno = m_selectedno;

		if( mvboneno == 0 )
			return 0;

		bHandled = FALSE;

		NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
		int delta = pnmud->iDelta;

		int ret;
		D3DXVECTOR3 axis( 0.0f, 1.0f, 0.0f );


		ret = SetRotation( axis, delta, mvboneno );
		if( ret ){
			DbgOut( "MotParamDlg : OnSpin_Sp_roty : SetRotation error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int tmpmvboneno, mvboneno;

		if( m_sellockno != 0 )
			tmpmvboneno = m_sellockno;
		else
			tmpmvboneno = m_selectedno;

		if( tmpmvboneno > 0 ){
			CShdElem* selelem;
			selelem = (*m_shandler)( tmpmvboneno );
			CShdElem* chilelem;
			chilelem = selelem->child;
			if( chilelem ){
				mvboneno = chilelem->serialno;
			}else{
				mvboneno = 0;
			}
		}else{
			mvboneno = 0;
		}

		if( mvboneno == 0 )
			return 0;

		bHandled = FALSE;

		NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
		int delta = pnmud->iDelta;

		int ret;
		D3DXVECTOR3 axis( 0.0f, 1.0f, 0.0f );

		while( mvboneno > 0 ){

			ret = SetRotation( axis, delta, mvboneno );
			if( ret ){
				DbgOut( "MotParamDlg : OnSpin_Sp_roty : SetRotation error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			CShdElem* curelem;
			curelem = (*m_shandler)( mvboneno );
			CShdElem* broelem;
			broelem = curelem->brother;
			if( broelem ){
				mvboneno = broelem->serialno;
			}else{
				mvboneno = 0;
			}
		}
	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
		UpdateAllMatrix( 0 );
		Redraw();
	}


	return 0;
}
LRESULT CMotParamDlg::OnSpin_Sp_rotz(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( m_rotkind == ROTKIND_NEW ){
		int mvboneno;
		if( m_sellockno != 0 )
			mvboneno = m_sellockno;
		else
			mvboneno = m_selectedno;

		if( mvboneno == 0 )
			return 0;

		bHandled = FALSE;

		NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
		int delta = pnmud->iDelta;

		int ret;
		D3DXVECTOR3 axis( 0.0f, 0.0f, 1.0f );


		ret = SetRotation( axis, delta, mvboneno );
		if( ret ){
			DbgOut( "MotParamDlg : OnSpin_Sp_rotz : SetRotation error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int tmpmvboneno, mvboneno;

		if( m_sellockno != 0 )
			tmpmvboneno = m_sellockno;
		else
			tmpmvboneno = m_selectedno;

		if( tmpmvboneno > 0 ){
			CShdElem* selelem;
			selelem = (*m_shandler)( tmpmvboneno );
			CShdElem* chilelem;
			chilelem = selelem->child;
			if( chilelem ){
				mvboneno = chilelem->serialno;
			}else{
				mvboneno = 0;
			}
		}else{
			mvboneno = 0;
		}

		if( mvboneno == 0 )
			return 0;

		bHandled = FALSE;

		NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
		int delta = pnmud->iDelta;

		int ret;
		D3DXVECTOR3 axis( 0.0f, 0.0f, 1.0f );

		while( mvboneno > 0 ){

			ret = SetRotation( axis, delta, mvboneno );
			if( ret ){
				DbgOut( "MotParamDlg : OnSpin_Sp_rotz : SetRotation error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			CShdElem* curelem;
			curelem = (*m_shandler)( mvboneno );
			CShdElem* broelem;
			broelem = curelem->brother;
			if( broelem ){
				mvboneno = broelem->serialno;
			}else{
				mvboneno = 0;
			}
		}


	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
		UpdateAllMatrix( 0 );
		Redraw();
	}

	return 0;
}

LRESULT CMotParamDlg::OnSpin_Sp_rotrad(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if( m_preview_flag == 1 )
		return 0;
	
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;
	char tempchar[256];

	int delta = pnmud->iDelta;

	if( delta > 0 ){
		int newrad;
		newrad = m_rotrad + 1;
		if( newrad <= MAX_ROTRAD ){
			m_rotrad = newrad;
			sprintf_s( tempchar, 256, "%d度", m_rotrad );
			m_tex_rotrad_wnd.SetWindowText( tempchar );
		}
	}else if( delta < 0 ){
		int newrad;
		newrad = m_rotrad - 1;
		if( newrad >= MIN_ROTRAD ){
			m_rotrad = newrad;
			sprintf_s( tempchar, 256, "%d度", m_rotrad );
			m_tex_rotrad_wnd.SetWindowText( tempchar );
		}
	}

	return 0;
}


int CMotParamDlg::GetPasteMP( COPYELEM* cebuf, int* pastelist, int serino, CMotionPoint2* retmpptr )
{

	int ret;
	int symmno = 0;
	int symmtype = SYMMTYPE_NONE;

	ret = SOGetSymmNo( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
	if( ret ){
		DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( symmno <= 0 ){
		ret = SOGetSymmNo2( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
		if( ret ){
			DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo2 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( symmno > 0 ){

		CMotionPoint2 symmmp;
		symmmp = ( cebuf + symmno )->mp;

		ret = SOSetSymMotionPoint( symmtype, &symmmp, retmpptr, 1 );//SymmOpe.cpp
		if( ret ){
			DbgOut( "motparamdlg : GetPasteMP : SetSymMotionPoint error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


	}else{
		// そのままペースト。
		*retmpptr = ( cebuf + serino )->mp;
	}

	return 0;
}


int CMotParamDlg::SetSymMotionPoint( CShdElem* srcselem, int symmtype )
{

	//右クリックメニューから呼ばれる。

	if( (m_mot_cookie < 0) || (m_current < 0) ){
		return 0;//!!!!!!!!!!!
	}

	CMotionCtrl* mcptr;
	mcptr = (*m_mhandler)( srcselem->serialno );
	_ASSERT( mcptr );

	CMotionPoint2* mpptr;
	mpptr = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );

	if( mpptr ){
		int ret;
		ret = SOSetSymMotionPoint( symmtype, mpptr, mpptr, 0 );
		if( ret ){
			DbgOut( "motparamdlg : SetSymMotionPoint : SetSymMotionPoint 2 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		//!!!!!
		UpdateMatrixSelected( srcselem->serialno, 0, 0 );
		Redraw();

	}

	return 0;
}



int CMotParamDlg::ExpandAndShiftMotion( int shiftnum )
{
	int ret;
// expand
	int saverangemax = m_sl_mp_rangemax;//!!!!!!!
	int newrangemax = m_sl_mp_rangemax + shiftnum;
	
	ret = m_mhandler->ChangeTotalFrame( m_mot_cookie, newrangemax + 1, (newrangemax > m_sl_mp_rangemax) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	m_sl_mp_rangemax = newrangemax;
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;

	m_motid[ m_mot_cookie ].startframe = m_startframe;
	m_motid[ m_mot_cookie ].endframe = m_endframe;
	m_motid[ m_mot_cookie ].framerange = m_range;

	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, 0 );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, 0, m_sl_mp_rangemax );//

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

// shift

	int srcframeno;
	int dstframeno;
	for( srcframeno = saverangemax; srcframeno >= 0; srcframeno-- ){
		
		dstframeno = srcframeno + shiftnum;

		CMotionPoint2 tempmp;
		CMotionPoint2* isexist2;
		CMotionPoint2* retmp;		
		CMotionCtrl* mcptr;
		int elemno;

		for( elemno = 0; elemno < m_mhandler->s2mot_leng; elemno++ ){
			mcptr = (*m_mhandler)(elemno );
			if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
				isexist2 = mcptr->IsExistMotionPoint( m_mot_cookie, srcframeno );
				if( isexist2 ){
					tempmp = *isexist2;

					ret = mcptr->DeleteMotionPoint( m_mot_cookie, srcframeno );
					if( ret ){
						DbgOut( "motparamdlg : ExpandAndShiftMotion : DeleteMotionPoint error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					retmp = mcptr->AddMotionPoint( m_mot_cookie, dstframeno,
						tempmp.m_q,
						//tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, tempmp.dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
						tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, m_dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
					if( !retmp ){
						DbgOut( "motparamdlg : ExpandAndShiftMotion : AddMotionPoint error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}
			}
		}
	}


// update
	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : ExpandAndShiftMotion : MakeRawmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : ExpandAndShiftMotion : RawmatToMotionmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : ExpandAndShiftMotion : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	BOOL dummy;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	return 0;
}

int CMotParamDlg::MotCut()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;
	int ret;

	CMotCutDlg dlg( m_sl_mp_rangemax );
	ret = (int)dlg.DoModal();
	if( ret != IDOK ){
		return 0;
	}


	int serino;
	int shdnum = m_shandler->s2shd_leng;

	int frameno;
	for( serino = 1; serino < shdnum; serino++ ){
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( serino );
		_ASSERT( mcptr );

		if( mcptr->motinfo == 0 ){
			continue;
		}

		CMotionPoint2* curmp2 = m_mhandler->GetFirstMotionPoint( serino, m_mot_cookie );
		CMotionPoint2* nextmp = 0;
		while( curmp2 ){
			nextmp = curmp2->next;
			frameno = curmp2->m_frameno;
			if( dlg.m_chkfirst && (curmp2->m_frameno < dlg.m_firstframe) ){
				ret = mcptr->DeleteMotionPoint( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}else if( dlg.m_chklast && (curmp2->m_frameno > dlg.m_lastframe) ){
				ret = mcptr->DeleteMotionPoint( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			curmp2 = nextmp;
		}
//////////////
		CTexKey* tkptr = m_mhandler->GetFirstTexKey( serino, m_mot_cookie );
		CTexKey* nexttk = 0;
		while( tkptr ){
			nexttk = tkptr->next;
			frameno = tkptr->frameno;
			if( dlg.m_chkfirst && (tkptr->frameno < dlg.m_firstframe) ){
				ret = mcptr->motinfo->DeleteTexKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}else if( dlg.m_chklast && (tkptr->frameno > dlg.m_lastframe) ){
				ret = mcptr->motinfo->DeleteTexKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			tkptr = nexttk;
		}
//////////////
		CAlpKey* akptr = m_mhandler->GetFirstAlpKey( serino, m_mot_cookie );
		CAlpKey* nextak = 0;
		while( akptr ){
			nextak = akptr->next;
			frameno = akptr->frameno;
			if( dlg.m_chkfirst && (akptr->frameno < dlg.m_firstframe) ){
				ret = mcptr->motinfo->DeleteAlpKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}else if( dlg.m_chklast && (akptr->frameno > dlg.m_lastframe) ){
				ret = mcptr->motinfo->DeleteAlpKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			akptr = nextak;
		}
//////////////
		CDSKey* dskptr = m_mhandler->GetFirstDSKey( serino, m_mot_cookie );
		CDSKey* nextdsk = 0;
		while( dskptr ){
			nextdsk = dskptr->next;
			frameno = dskptr->frameno;
			if( dlg.m_chkfirst && (dskptr->frameno < dlg.m_firstframe) ){
				ret = mcptr->motinfo->DeleteDSKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}else if( dlg.m_chklast && (dskptr->frameno > dlg.m_lastframe) ){
				ret = mcptr->motinfo->DeleteDSKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			dskptr = nextdsk;
		}
//////////////
		CMMotKey* mmkptr = mcptr->motinfo->GetFirstMMotKey( m_mot_cookie );
		CMMotKey* nextmmk = 0;
		while( mmkptr ){
			nextmmk = mmkptr->next;
			frameno = mmkptr->frameno;
			if( dlg.m_chkfirst && (mmkptr->frameno < dlg.m_firstframe) ){
				ret = mcptr->motinfo->DeleteMMotKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}else if( dlg.m_chklast && (mmkptr->frameno > dlg.m_lastframe) ){
				ret = mcptr->motinfo->DeleteMMotKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			mmkptr = nextmmk;
		}
	}

	if( dlg.m_chkfirst ){
		for( serino = 1; serino < shdnum; serino++ ){
			CMotionCtrl* mcptr;
			mcptr = (*m_mhandler)( serino );
			_ASSERT( mcptr );

			if( mcptr->motinfo == 0 ){
				continue;
			}

			CMotionPoint2* curmp2 = m_mhandler->GetFirstMotionPoint( serino, m_mot_cookie );
			CMotionPoint2* nextmp = 0;
			while( curmp2 ){
				curmp2->m_frameno -= dlg.m_firstframe;
				curmp2 = curmp2->next;
			}

	//////////////
			CTexKey* tkptr = m_mhandler->GetFirstTexKey( serino, m_mot_cookie );
			CTexKey* nexttk = 0;
			while( tkptr ){
				nexttk = tkptr->next;
				tkptr->frameno -= dlg.m_firstframe;
				tkptr = nexttk;
			}
	//////////////
			CAlpKey* akptr = m_mhandler->GetFirstAlpKey( serino, m_mot_cookie );
			CAlpKey* nextak = 0;
			while( akptr ){
				nextak = akptr->next;
				akptr->frameno -= dlg.m_firstframe;
				akptr = nextak;
			}
	//////////////
			CDSKey* dskptr = m_mhandler->GetFirstDSKey( serino, m_mot_cookie );
			CDSKey* nextdsk = 0;
			while( dskptr ){
				nextdsk = dskptr->next;
				dskptr->frameno -= dlg.m_firstframe;
				dskptr = nextdsk;
			}
	//////////////
			CMMotKey* mmkptr = mcptr->motinfo->GetFirstMMotKey( m_mot_cookie );
			CMMotKey* nextmmk = 0;
			while( mmkptr ){
				nextmmk = mmkptr->next;
				mmkptr->frameno -= dlg.m_firstframe;
				mmkptr = nextmmk;
			}
		}
	}


	int saverangemax = m_sl_mp_rangemax;
	int firstleng = 0;
	int lastleng = 0;
	if( dlg.m_chkfirst ){
		firstleng = dlg.m_firstframe;
	}
	if( dlg.m_chklast ){
		lastleng = m_sl_mp_rangemax - dlg.m_lastframe;
		lastleng = max( 0, lastleng );
	}
	int newrangemax = m_sl_mp_rangemax - firstleng - lastleng;
	
	ret = m_mhandler->ChangeTotalFrame( m_mot_cookie, newrangemax + 1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	m_sl_mp_rangemax = newrangemax;
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;
	m_current = m_startframe;
	m_motid[ m_mot_cookie ].startframe = m_startframe;
	m_motid[ m_mot_cookie ].endframe = m_endframe;
	m_motid[ m_mot_cookie ].framerange = m_range;


	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, 0 );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, 0, m_sl_mp_rangemax );//

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );



	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : MotCut : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = InitRotAndMv();
	_ASSERT( !ret );
	ret = ParamsToDlg();
	_ASSERT( !ret );

	SetCurrentMotion();

	Redraw();

	return 0;
}
int CMotParamDlg::MotMabiki()
{
	int ret;
	int serino;
	int shdnum = m_shandler->s2shd_leng;

	CMabikiDlg dlg;
	ret = (int)dlg.DoModal();
	if( ret != IDOK ){
		return 0;
	}



	int frameno;
	int divframe;
	for( serino = 1; serino < shdnum; serino++ ){
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( serino );
		_ASSERT( mcptr );

		if( mcptr->motinfo == 0 ){
			continue;
		}

		CMotionPoint2* curmp2 = m_mhandler->GetFirstMotionPoint( serino, m_mot_cookie );
		CMotionPoint2* nextmp = 0;
		while( curmp2 ){
			nextmp = curmp2->next;
			divframe = curmp2->m_frameno % dlg.m_step;
			frameno = curmp2->m_frameno;
			if( divframe != 0 ){
				ret = mcptr->DeleteMotionPoint( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			curmp2 = nextmp;
		}
//////////////
		CTexKey* tkptr = m_mhandler->GetFirstTexKey( serino, m_mot_cookie );
		CTexKey* nexttk = 0;
		while( tkptr ){
			nexttk = tkptr->next;
			divframe = tkptr->frameno % dlg.m_step;
			frameno = tkptr->frameno;
			if( divframe != 0 ){
				ret = mcptr->motinfo->DeleteTexKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			tkptr = nexttk;
		}
//////////////
		CAlpKey* akptr = m_mhandler->GetFirstAlpKey( serino, m_mot_cookie );
		CAlpKey* nextak = 0;
		while( akptr ){
			nextak = akptr->next;
			divframe = akptr->frameno % dlg.m_step;
			frameno = akptr->frameno;
			if( divframe != 0 ){
				ret = mcptr->motinfo->DeleteAlpKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			akptr = nextak;
		}
//////////////
		CDSKey* dskptr = m_mhandler->GetFirstDSKey( serino, m_mot_cookie );
		CDSKey* nextdsk = 0;
		while( dskptr ){
			nextdsk = dskptr->next;
			divframe = dskptr->frameno % dlg.m_step;
			frameno = dskptr->frameno;
			if( divframe != 0 ){
				ret = mcptr->motinfo->DeleteDSKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			dskptr = nextdsk;
		}
//////////////
		CMMotKey* mmkptr = mcptr->motinfo->GetFirstMMotKey( m_mot_cookie );
		CMMotKey* nextmmk = 0;
		while( mmkptr ){
			nextmmk = mmkptr->next;
			divframe = mmkptr->frameno % dlg.m_step;
			frameno = mmkptr->frameno;
			if( divframe != 0 ){
				ret = mcptr->motinfo->DeleteMMotKey( m_mot_cookie, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
			}
			mmkptr = nextmmk;
		}
	}

	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : MotMabiki : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = InitRotAndMv();
	_ASSERT( !ret );
	ret = ParamsToDlg();
	_ASSERT( !ret );

	SetCurrentMotion();

	Redraw();

	return 0;

}



int CMotParamDlg::MultMotion2_0()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;



	int ret;
// expand
	int saverangemax = m_sl_mp_rangemax;
	int newrangemax = m_sl_mp_rangemax * 2;
	
	ret = m_mhandler->ChangeTotalFrame( m_mot_cookie, newrangemax + 1, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	m_sl_mp_rangemax = newrangemax;
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;
	m_current = m_startframe;
	m_motid[ m_mot_cookie ].startframe = m_startframe;
	m_motid[ m_mot_cookie ].endframe = m_endframe;
	m_motid[ m_mot_cookie ].framerange = m_range;



	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, 0 );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, 0, m_sl_mp_rangemax );//

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

///////////
	int frameno;
	for( frameno = saverangemax; frameno >= 0; frameno-- ){
		int seri;
		for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
			CShdElem* selem;
			selem = (*m_shandler)( seri );
			if( selem->IsJoint() && (selem->type != SHDMORPH) ){
				CMotionPoint2* curmp = 0;
				ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, seri, frameno, &curmp );
				_ASSERT( !ret );
				if( curmp ){
					curmp->m_frameno *= 2;
				}

				CTexKey* curtk = 0;
				ret = m_mhandler->ExistTexKey( seri, m_mot_cookie, frameno, &curtk );
				_ASSERT( !ret );
				if( curtk ){
					curtk->frameno *= 2;
				}

				CAlpKey* curalpk = 0;
				ret = m_mhandler->ExistAlpKey( seri, m_mot_cookie, frameno, &curalpk );
				_ASSERT( !ret );
				if( curalpk ){
					curalpk->frameno *= 2;
				}


				CDSKey* curdsk = 0;
				ret = m_mhandler->ExistDSKey( seri, m_mot_cookie, frameno, &curdsk );
				_ASSERT( !ret );
				if( curdsk ){
					curdsk->frameno *= 2;
				}

				CMotionCtrl* mcptr;
				mcptr = (*m_mhandler)( seri );
				_ASSERT( mcptr );
				_ASSERT( mcptr->motinfo );
				CMMotKey* curmmk = 0;
				ret = mcptr->motinfo->ExistMMotKey( m_mot_cookie, frameno, &curmmk );
				_ASSERT( !ret );
				if( curmmk ){
					curmmk->frameno *= 2;
				}
			}
		}
	}

// update
	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : MultMotion2_0 : MakeRawmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : MultMotion2_0 : RawmatToMotionmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : MultMotion2_0 : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	BOOL dummy;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	m_MorphKeyDlg->OnUserSelchange();//!!!!!!!!!!!	

	return 0;
}
int CMotParamDlg::MultMotion0_5()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;



	int ret;

	int saverangemax = m_sl_mp_rangemax;
	int mask = 0x01;
	int newrangemax;
//	if( saverangemax & mask ){
//		newrangemax = m_sl_mp_rangemax / 2 + 1;
//	}else{
//		newrangemax = m_sl_mp_rangemax / 2;
//	}
	newrangemax = m_sl_mp_rangemax / 2;
	if( newrangemax <= 0 ){
		return 0;
	}

	int* keyframe;
	keyframe = (int*)malloc( sizeof( int ) * ( saverangemax + 1 ) );
	if( !keyframe ){
		DbgOut( "motparamdlg : MultMotion0_5 : keyframe alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	int* aftframe;
	aftframe = (int*)malloc( sizeof( int ) * ( saverangemax + 1 ) );
	if( !aftframe ){
		DbgOut( "motparamdlg : MultMotion0_5 : aftframe alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( seri );
		_ASSERT( mcptr );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			int tmpcnt;
			for( tmpcnt = 0; tmpcnt < (saverangemax + 1); tmpcnt++ ){
				*(keyframe + tmpcnt) = -1;
				*(aftframe + tmpcnt) = -1;
			}

			int framenum = 0;
			ret = m_mhandler->GetKeyframeNoRange( m_mot_cookie, seri, 0, saverangemax, keyframe, saverangemax + 1, &framenum );
			if( ret ){
				DbgOut( "motparamdlg : MultMotion0_5 : mh GetKeyFrameNoRange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( framenum > 0 ){
				int chkcnt;
				for( chkcnt = 0; chkcnt < framenum; chkcnt++ ){
					int curframe;
					curframe = *( keyframe + chkcnt );
					int nextframe, prevframe;
					if( chkcnt != (framenum - 1) ){
						nextframe = *( keyframe + chkcnt + 1 );
					}else{
						nextframe = -1;
					}
					if( chkcnt != 0 ){
						prevframe = *( keyframe + chkcnt - 1 );
					}else{
						prevframe = -1;
					}

					if( ( (nextframe >= 0) && ((nextframe - curframe) == 1) ) || ( (prevframe >= 0) && ((curframe - prevframe) == 1) ) ){
						if( curframe & mask ){
							*(aftframe + chkcnt) = -2;//連続したフレームを見つけた場合、奇数フレームを削除
						}else{
							*(aftframe + chkcnt) = curframe / 2;
						}
					}else{
						*(aftframe + chkcnt) = curframe / 2;
					}
				}


				int changecnt;
				for( changecnt = 0; changecnt < framenum; changecnt++ ){
					int orgframe, changeframe;
					orgframe = *(keyframe + changecnt);
					changeframe = *(aftframe + changecnt);
					
					CMotionPoint2* isexist = 0;			
					isexist = mcptr->IsExistMotionPoint( m_mot_cookie, orgframe );
					if( isexist ){
						if( changeframe == -2 ){
							ret = mcptr->DeleteMotionPoint( m_mot_cookie, orgframe );
							if( ret ){
								DbgOut( "motparamdlg : MultMotion0_5 : mc DeleteMotionPoint error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}else{
							isexist->m_frameno = changeframe;
						}
					}
				}
			}
		}
	}

///// texkey
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( seri );
		_ASSERT( mcptr );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			int tmpcnt;
			for( tmpcnt = 0; tmpcnt < (saverangemax + 1); tmpcnt++ ){
				*(keyframe + tmpcnt) = -1;
				*(aftframe + tmpcnt) = -1;
			}

			int framenum = 0;
			ret = m_mhandler->GetTexAnimKeyframeNoRange( seri, m_mot_cookie, 0, saverangemax, keyframe, saverangemax + 1, &framenum );
			if( ret ){
				DbgOut( "motparamdlg : MultMotion0_5 : mh GetTexAnimKeyframeNoRange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( framenum > 0 ){
				int chkcnt;
				for( chkcnt = 0; chkcnt < framenum; chkcnt++ ){
					int curframe;
					curframe = *( keyframe + chkcnt );
					int nextframe, prevframe;
					if( chkcnt != (framenum - 1) ){
						nextframe = *( keyframe + chkcnt + 1 );
					}else{
						nextframe = -1;
					}
					if( chkcnt != 0 ){
						prevframe = *( keyframe + chkcnt - 1 );
					}else{
						prevframe = -1;
					}

					if( ( (nextframe >= 0) && ((nextframe - curframe) == 1) ) || ( (prevframe >= 0) && ((curframe - prevframe) == 1) ) ){
						if( curframe & mask ){
							*(aftframe + chkcnt) = -2;//連続したフレームを見つけた場合、奇数フレームを削除
						}else{
							*(aftframe + chkcnt) = curframe / 2;
						}
					}else{
						*(aftframe + chkcnt) = curframe / 2;
					}
				}


				int changecnt;
				for( changecnt = 0; changecnt < framenum; changecnt++ ){
					int orgframe, changeframe;
					orgframe = *(keyframe + changecnt);
					changeframe = *(aftframe + changecnt);
					
					CTexKey* tkptr = 0;
					ret = m_mhandler->ExistTexKey( seri, m_mot_cookie, orgframe, &tkptr );
					if( ret ){
						DbgOut( "motparamdlg : MultMotion0_5 : mh ExistTexKey error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					if( tkptr ){
						if( changeframe == -2 ){
							ret = m_mhandler->DeleteTexKey( seri, m_mot_cookie, orgframe );
							if( ret ){
								DbgOut( "motparamdlg : MultMotion0_5 : mh DeleteTexKey error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}else{
							tkptr->frameno = changeframe;
						}
					}
				}
			}
		}
	}
///// alpkey
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( seri );
		_ASSERT( mcptr );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			int tmpcnt;
			for( tmpcnt = 0; tmpcnt < (saverangemax + 1); tmpcnt++ ){
				*(keyframe + tmpcnt) = -1;
				*(aftframe + tmpcnt) = -1;
			}

			int framenum = 0;
			ret = m_mhandler->GetAlpAnimKeyframeNoRange( seri, m_mot_cookie, 0, saverangemax, keyframe, saverangemax + 1, &framenum );
			if( ret ){
				DbgOut( "motparamdlg : MultMotion0_5 : mh GetAlpAnimKeyframeNoRange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( framenum > 0 ){
				int chkcnt;
				for( chkcnt = 0; chkcnt < framenum; chkcnt++ ){
					int curframe;
					curframe = *( keyframe + chkcnt );
					int nextframe, prevframe;
					if( chkcnt != (framenum - 1) ){
						nextframe = *( keyframe + chkcnt + 1 );
					}else{
						nextframe = -1;
					}
					if( chkcnt != 0 ){
						prevframe = *( keyframe + chkcnt - 1 );
					}else{
						prevframe = -1;
					}

					if( ( (nextframe >= 0) && ((nextframe - curframe) == 1) ) || ( (prevframe >= 0) && ((curframe - prevframe) == 1) ) ){
						if( curframe & mask ){
							*(aftframe + chkcnt) = -2;//連続したフレームを見つけた場合、奇数フレームを削除
						}else{
							*(aftframe + chkcnt) = curframe / 2;
						}
					}else{
						*(aftframe + chkcnt) = curframe / 2;
					}
				}


				int changecnt;
				for( changecnt = 0; changecnt < framenum; changecnt++ ){
					int orgframe, changeframe;
					orgframe = *(keyframe + changecnt);
					changeframe = *(aftframe + changecnt);
					
					CAlpKey* alpkptr = 0;
					ret = m_mhandler->ExistAlpKey( seri, m_mot_cookie, orgframe, &alpkptr );
					if( ret ){
						DbgOut( "motparamdlg : MultMotion0_5 : mh ExistAlpKey error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					if( alpkptr ){
						if( changeframe == -2 ){
							ret = m_mhandler->DeleteAlpKey( seri, m_mot_cookie, orgframe );
							if( ret ){
								DbgOut( "motparamdlg : MultMotion0_5 : mh DeleteAlpKey error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}else{
							alpkptr->frameno = changeframe;
						}
					}
				}
			}
		}
	}

///// dskey
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( seri );
		_ASSERT( mcptr );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			int tmpcnt;
			for( tmpcnt = 0; tmpcnt < (saverangemax + 1); tmpcnt++ ){
				*(keyframe + tmpcnt) = -1;
				*(aftframe + tmpcnt) = -1;
			}

			int framenum = 0;
			ret = m_mhandler->GetDSAnimKeyframeNoRange( seri, m_mot_cookie, 0, saverangemax, keyframe, saverangemax + 1, &framenum );
			if( ret ){
				DbgOut( "motparamdlg : MultMotion0_5 : mh GetDSAnimKeyframeNoRange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( framenum > 0 ){
				int chkcnt;
				for( chkcnt = 0; chkcnt < framenum; chkcnt++ ){
					int curframe;
					curframe = *( keyframe + chkcnt );
					int nextframe, prevframe;
					if( chkcnt != (framenum - 1) ){
						nextframe = *( keyframe + chkcnt + 1 );
					}else{
						nextframe = -1;
					}
					if( chkcnt != 0 ){
						prevframe = *( keyframe + chkcnt - 1 );
					}else{
						prevframe = -1;
					}

					if( ( (nextframe >= 0) && ((nextframe - curframe) == 1) ) || ( (prevframe >= 0) && ((curframe - prevframe) == 1) ) ){
						if( curframe & mask ){
							*(aftframe + chkcnt) = -2;//連続したフレームを見つけた場合、奇数フレームを削除
						}else{
							*(aftframe + chkcnt) = curframe / 2;
						}
					}else{
						*(aftframe + chkcnt) = curframe / 2;
					}
				}


				int changecnt;
				for( changecnt = 0; changecnt < framenum; changecnt++ ){
					int orgframe, changeframe;
					orgframe = *(keyframe + changecnt);
					changeframe = *(aftframe + changecnt);
					
					CDSKey* dskptr = 0;
					ret = m_mhandler->ExistDSKey( seri, m_mot_cookie, orgframe, &dskptr );
					if( ret ){
						DbgOut( "motparamdlg : MultMotion0_5 : mh ExistDSKey error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					if( dskptr ){
						if( changeframe == -2 ){
							ret = m_mhandler->DeleteDSKey( seri, m_mot_cookie, orgframe );
							if( ret ){
								DbgOut( "motparamdlg : MultMotion0_5 : mh DeleteDSKey error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}else{
							dskptr->frameno = changeframe;
						}
					}
				}
			}
		}
	}

///// morph anim
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( seri );
		_ASSERT( mcptr );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			int tmpcnt;
			for( tmpcnt = 0; tmpcnt < (saverangemax + 1); tmpcnt++ ){
				*(keyframe + tmpcnt) = -1;
				*(aftframe + tmpcnt) = -1;
			}

			int framenum = 0;
			ret = mcptr->motinfo->GetMMotAnimKeyframeNoRange( m_mot_cookie, 0, saverangemax, keyframe, saverangemax + 1, &framenum );
			if( ret ){
				DbgOut( "motparamdlg : MultMotion0_5 : mi GetMMotAnimKeyframeNoRange error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( framenum > 0 ){
				int chkcnt;
				for( chkcnt = 0; chkcnt < framenum; chkcnt++ ){
					int curframe;
					curframe = *( keyframe + chkcnt );
					int nextframe, prevframe;
					if( chkcnt != (framenum - 1) ){
						nextframe = *( keyframe + chkcnt + 1 );
					}else{
						nextframe = -1;
					}
					if( chkcnt != 0 ){
						prevframe = *( keyframe + chkcnt - 1 );
					}else{
						prevframe = -1;
					}

					if( ( (nextframe >= 0) && ((nextframe - curframe) == 1) ) || ( (prevframe >= 0) && ((curframe - prevframe) == 1) ) ){
						if( curframe & mask ){
							*(aftframe + chkcnt) = -2;//連続したフレームを見つけた場合、奇数フレームを削除
						}else{
							*(aftframe + chkcnt) = curframe / 2;
						}
					}else{
						*(aftframe + chkcnt) = curframe / 2;
					}
				}


				int changecnt;
				for( changecnt = 0; changecnt < framenum; changecnt++ ){
					int orgframe, changeframe;
					orgframe = *(keyframe + changecnt);
					changeframe = *(aftframe + changecnt);
					
					CMMotKey* mmkptr = 0;
					ret = mcptr->motinfo->ExistMMotKey( m_mot_cookie, orgframe, &mmkptr );
					if( ret ){
						DbgOut( "motparamdlg : MultMotion0_5 : mi ExistMMotKey error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					if( mmkptr ){
						if( changeframe == -2 ){
							ret = mcptr->motinfo->DeleteMMotKey( m_mot_cookie, orgframe );
							if( ret ){
								DbgOut( "motparamdlg : MultMotion0_5 : mi DeleteMMotKey error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}else{
							mmkptr->frameno = changeframe;
						}
					}
				}
			}
		}
	}




///// free
	if( keyframe ){
		free( keyframe );
		keyframe = 0;
	}
	if( aftframe ){
		free( aftframe );
		aftframe = 0;
	}

///// change total frame
	ret = m_mhandler->ChangeTotalFrame( m_mot_cookie, newrangemax + 1, 0 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	m_sl_mp_rangemax = newrangemax;
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;
	m_current = m_startframe;
	m_motid[ m_mot_cookie ].startframe = m_startframe;
	m_motid[ m_mot_cookie ].endframe = m_endframe;
	m_motid[ m_mot_cookie ].framerange = m_range;


	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, 0 );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, 0, m_sl_mp_rangemax );//

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

///// update
	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : MultMotion0_5 : MakeRawmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : MultMotion0_5 : RawmatToMotionmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : MultMotion0_5 : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	BOOL dummy;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );
	OnReleasedcaptureSl_mp( 0, 0, dummy );


	return 0;
}


int CMotParamDlg::DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec )
{
	float mag;
	mag = srcvec->x * srcvec->x + srcvec->y * srcvec->y + srcvec->z * srcvec->z;

	float sqmag;
	sqmag = (float)sqrt( mag );

	if( sqmag != 0.0f ){
		float magdiv;
		magdiv = 1.0f / sqmag;
//		dstvec->x *= magdiv;
//		dstvec->y *= magdiv;
//		dstvec->z *= magdiv;
		dstvec->x = srcvec->x * magdiv;
		dstvec->y = srcvec->y * magdiv;
		dstvec->z = srcvec->z * magdiv;

	}else{
		//DbgOut( "viewer : DXVec3Normalize : zero warning !!!\n" );
		dstvec->x = 0.0f;
		dstvec->y = 0.0f;
		dstvec->z = 0.0f;
		//_ASSERT( 0 );
	}

	return 0;
}


int CMotParamDlg::GetBonePos( int srcseri, D3DXMATRIX wmat, int mode, D3DXVECTOR3* dstpos )
{

	int ret;

	ret = m_shandler->GetBonePos( srcseri, mode, m_mot_cookie, m_current, dstpos, m_mhandler, wmat, 1 );
	if( ret ){
		DbgOut( "motparamdlg : GetBonePos : shandler GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CMotParamDlg::GetBonePosOnFrame( int frameno, int srcseri, D3DXMATRIX wmat, int mode, D3DXVECTOR3* dstpos )
{

	int ret;

	ret = m_shandler->GetBonePos( srcseri, mode, m_mot_cookie, frameno, dstpos, m_mhandler, wmat, 1 );
	if( ret ){
		DbgOut( "motparamdlg : GetBonePosOnFrame : shandler GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMotParamDlg::GetSelectedJointPos( int* setflag, D3DXVECTOR3* jposptr )
{

	int ret;

	if( m_selectedno == 0 ){
		*setflag = 0;
		return 0;
	}
	*setflag = m_selectedno;//!!!!!!

//	D3DXMATRIX inimat;
//	D3DXMatrixIdentity( &inimat );//!!!!!!!!
	

	if( m_preview_flag == 0 ){
		ret = m_shandler->GetBonePos( m_selectedno, 1, m_mot_cookie, m_current, jposptr, 
			m_mhandler, m_shandler->m_gpdata.m_matWorld, 1 );
	}else{
		ret = m_shandler->GetBonePos( m_selectedno, 1, m_mot_cookie, s_frameno, jposptr, 
			m_mhandler, m_shandler->m_gpdata.m_matWorld, 1 );
	}
	if( ret ){
		DbgOut( "motparamdlg : GetSelectedJointPos : shandler GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMotParamDlg::ShowDlg( int cmdshow, int stopflag )
{
	m_cmdshow = cmdshow;

	if( (m_cmdshow == SW_HIDE) && m_thandler ){
		BOOL handled;
		if( stopflag )
			OnStop( 0, 9999, 0, handled );
	}

	ShowWindow( m_cmdshow );
	if( m_cmdshow != SW_HIDE ){
		Redraw();
	}

	return 0;
}

int CMotParamDlg::GetSelectedJoint( int* selno, char** selname )
{
	if( m_selectedno > 0 ){
		*selno = m_selectedno;
		CTreeElem2* telem;
		telem = (*m_thandler)( m_selectedno );
		_ASSERT( telem );
		*selname = telem->name;
	}else{
		*selno = m_selectedno;
		*selname = 0;
	}
	return 0;
}

int CMotParamDlg::MoveBoneZa4( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 targetobj, int* lastparent, int iklevel )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	int ret;	

	if( m_optardlg )
		m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );


	int docre, haskey;
	ret = m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}

	if( (docre == 0) && (haskey == 0) ){
		return 0;
	}


	D3DXVECTOR3 mvobj, parobj;
	D3DXVECTOR3 parsdw;

	D3DXVECTOR3 planeaxis;
	D3DXVECTOR3 rotaxis;

	D3DXVECTOR3 diffmvtarget;

	float dist;
	float dist1, dist2;

	int movecntmax;
	int movecnt;

	CQuaternion saveq, saveq1st, saveq2nd;

	if( iklevel <= 0 ){
		if( m_ikmode == IKMODE_JOINT ){
			movecntmax = m_iklevel;
		}else{
			movecntmax = m_iklevelope;
		}
	}else{
		movecntmax = iklevel;
	}

	CShdElem* mvbone = (*m_shandler)( mvno );
	if( !mvbone )
		return 0;

	CShdElem* parbone;
	int parboneno;

	parbone = m_shandler->FindUpperJoint( mvbone );
	if( !parbone )
		return 0;
	parboneno = parbone->serialno;

	CShdElem* curbone;
	curbone = (*m_shandler)( mvno );


	movecnt = 0;

	int ikboneno = parboneno;
	//int ikboneno = mvno;
	int curboneno = mvno;


	while( (movecnt < movecntmax) && parbone ){

//		if( (movecnt != 0) && (curbone->m_notransik != 0) ){//!!!!!!!!!!!!!!!!!!!!!!!!!
//			break;
//		}

		if( (curbone->m_notransik != 0) ){//!!!!!!!!!!!!!!!!!!!!!!!!!
			break;
		}


		*lastparent = parboneno;

		ret = GetBonePos( mvno, matWorld, 1, &mvobj );
		if( ret ){
			DbgOut( "MoveBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}

		ret = GetBonePos( parboneno, matWorld, 1, &parobj );
		if( ret ){
			DbgOut( "MoveBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}


		diffmvtarget = targetobj - mvobj;
		dist = D3DXVec3Length( &diffmvtarget );
		
//////////			
		CQuaternion localq;
		ret = curbone->GetZa4LocalQ( &localq );
		_ASSERT( !ret );

		int za4rotaxis;
		ret = curbone->GetZa4RotAxis( &za4rotaxis );
		_ASSERT( !ret );

		D3DXVECTOR3 befaxis;
		if( za4rotaxis == ROTAXIS_X ){
			befaxis = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		}else if( za4rotaxis == ROTAXIS_Y ){
			befaxis = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		}else if( za4rotaxis == ROTAXIS_Z ){
			befaxis = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		}

		ret = localq.Rotate( &rotaxis, befaxis );
		_ASSERT( !ret );

		DXVec3Normalize( &rotaxis, &rotaxis );


		if( parbone->m_ikskip != 0 ){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			goto moveboneza4next;
		}


		if( (targetobj.x != mvobj.x) || (targetobj.y != mvobj.y) || (targetobj.z != mvobj.z) ){		
			

			D3DXVECTOR3 targetsdw;
			D3DXVECTOR3 parsdw2mv, parsdw2target;
			ret = GetShadow( &parsdw, rotaxis, mvobj, parobj, rotaxis );
			if( ret )
				parsdw = parobj;

			ret = GetShadow( &targetsdw, rotaxis, mvobj, targetobj, rotaxis );
			if( ret )
				targetsdw = targetobj;

			parsdw2mv = mvobj - parsdw;
			DXVec3Normalize( &parsdw2mv, &parsdw2mv );


			parsdw2target = targetsdw - parsdw;
			DXVec3Normalize( &parsdw2target, &parsdw2target );

			CQuaternion multq1, multq2;
//			multq1.RotationArc( parsdw2mv, parsdw2target );
//			multq1.normalize();
//
//			multq2 = -multq1;
//			multq2.w = multq1.w;

			float dot, axrad;
			dot = D3DXVec3Dot( &parsdw2mv, &parsdw2target );
			if( (dot >= -0.999999619f) && (dot <= 0.999999619f) ){
				axrad = acosf( dot );
			}else if( dot > 0.999999619f ){
				axrad = 0.0f;
			}else{
				axrad = (float)PAI;
			}

			multq1.SetAxisAndRot( rotaxis, axrad );
			multq2.SetAxisAndRot( rotaxis, -axrad );


ret = GetQ( curboneno, &saveq );
D3DXVECTOR3 saveeul;
ret = GetEul( curboneno, &saveeul );

			//1回目
			int forcemove = 0;
ret = SetMotionPointIK( m_current, matWorld, curboneno, multq1, (movecnt == (movecntmax - 1)), &forcemove, 0 );
			_ASSERT( !ret );

			UpdateMatrixSelected( curboneno, 1, 0 );


			//！！！boneのスクリーン座標を更新するために、FrameMove	が必要！！！
			m_d3dapp->SetBoneArray( parboneno );
			m_d3dapp->FrameMove( parboneno, 1 );


			ret = GetBonePos( mvno, matWorld, 1, &mvobj );
			if( ret ){
				DbgOut( "MoveBone : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			diffmvtarget = targetobj - mvobj;
			dist1 = D3DXVec3Length( &diffmvtarget );
			

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//if( (forcemove == 0) && (dist <= dist1) ){
//			if( dist <= dist1 ){
//				//近づかなかったら元に戻す。
//				ret = SetQ( curboneno, saveq );
//				ret = SetEul( curboneno, saveeul );
//				m_d3dapp->FrameMove( parboneno, 1 );
//			}


			
			//２回目
ret = SetQ( curboneno, saveq );
ret = SetEul( curboneno, saveeul );
m_d3dapp->FrameMove( parboneno, 1 );

			if( ret )
				return 0;
ret = SetMotionPointIK( m_current, matWorld, curboneno, multq2, (movecnt == (movecntmax - 1)), &forcemove, 0 );
			_ASSERT( !ret );

			UpdateMatrixSelected( curboneno, 1, 0 );

			//SetBoneArray( parboneno );
			m_d3dapp->FrameMove( parboneno, 1 );
						

			ret = GetBonePos( mvno, matWorld, 1, &mvobj );
			if( ret ){
				DbgOut( "MoveBone : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}


			diffmvtarget = targetobj - mvobj;
			dist2 = D3DXVec3Length( &diffmvtarget );





ret = SetQ( curboneno, saveq );
ret = SetEul( curboneno, saveeul );
m_d3dapp->FrameMove( parboneno, 1 );

			if( dist1 < dist ){
				if( dist1 <= dist2 ){
					//１回目
					ret = SetMotionPointIK( m_current, matWorld, curboneno, multq1, (movecnt == (movecntmax - 1)), &forcemove, 0 );
					_ASSERT( !ret );
					UpdateMatrixSelected( curboneno, 1, 0 );
					m_d3dapp->FrameMove( parboneno, 1 );

					CQuaternion tmpq;
					ret = GetStandardQ( curboneno, &tmpq );
					_ASSERT( !ret );
					ret = SetQOpTarget( curboneno, multq1, tmpq, 1 );
					_ASSERT( !ret );
				}else if( dist2 < dist ){
					//２回目
					ret = SetMotionPointIK( m_current, matWorld, curboneno, multq2, (movecnt == (movecntmax - 1)), &forcemove, 0 );
					_ASSERT( !ret );
					UpdateMatrixSelected( curboneno, 1, 0 );
					m_d3dapp->FrameMove( parboneno, 1 );

					CQuaternion tmpq;
					ret = GetStandardQ( curboneno, &tmpq );
					_ASSERT( !ret );
					ret = SetQOpTarget( curboneno, multq2, tmpq, 1 );
					_ASSERT( !ret );

				}else{
					_ASSERT( 0 );
				}
			}else{
				if( dist2 < dist ){
					//２回目
					ret = SetMotionPointIK( m_current, matWorld, curboneno, multq2, (movecnt == (movecntmax - 1)), &forcemove, 0 );
					_ASSERT( !ret );
					UpdateMatrixSelected( curboneno, 1, 0 );
					//SetBoneArray( parboneno );
					m_d3dapp->FrameMove( parboneno, 1 );

					CQuaternion tmpq;
					ret = GetStandardQ( curboneno, &tmpq );
					_ASSERT( !ret );
					ret = SetQOpTarget( curboneno, multq2, tmpq, 1 );
					_ASSERT( !ret );

				}else{
					//動かさない
					ret = SetQ( curboneno, saveq );
					ret = SetEul( curboneno, saveeul );
					m_d3dapp->FrameMove( parboneno, 1 );
				}
			}

		}

		goto moveboneza4next;

moveboneza4next:

		curboneno = parboneno;//!!!!!!!!
		curbone = parbone;

		parbone = m_shandler->FindUpperJoint( parbone );
		if( !parbone )
			break;
		parboneno = parbone->serialno;
		movecnt++;


		ikboneno = parboneno;//!!!!!!

	}


	return 0;
}



int CMotParamDlg::MoveBone( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 targetobj, int* lastparent, int defaxis, D3DXVECTOR3 srcaxis, D3DXVECTOR3* axisdone, int iklevel )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;

	if( m_optardlg )
		m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );


	int docre, haskey;
	m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}

	if( (docre == 0) && (haskey == 0) ){
		return 0;
	}
	int ret;	
	//float mvprerhw, parprerhw;
	//D3DXVECTOR3 mvpre, parpre;

	D3DXVECTOR3 mvobj, parobj;
	D3DXVECTOR3 parsdw;
	//D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	D3DXVECTOR3 planeaxis;
	D3DXVECTOR3 rotaxis;

	D3DXVECTOR3 diffmvtarget;

	//D3DXMATRIX inimat;
	//D3DXMatrixIdentity( &inimat );


	//float dot, rotrad;
	float dist;
	float dist1, dist2;

	int movecntmax;
	int movecnt;

	CQuaternion saveq, saveq1st, saveq2nd;

	//movecntmax = m_ik_bonenum;
	if( iklevel <= 0 ){
		if( m_ikmode == IKMODE_JOINT ){
			movecntmax = m_iklevel;
		}else{
			movecntmax = m_iklevelope;
		}
	}else{
		movecntmax = iklevel;
	}

	CShdElem* mvbone = (*m_shandler)( mvno );
	if( !mvbone )
		return 0;

	CShdElem* parbone;
	int parboneno;

	parbone = m_shandler->FindUpperJoint( mvbone );
	if( !parbone )
		return 0;
	parboneno = parbone->serialno;

	CShdElem* curbone;
	curbone = (*m_shandler)( mvno );


	movecnt = 0;

	int ikboneno = parboneno;
	//int ikboneno = mvno;
	int curboneno = mvno;


	while( (movecnt < movecntmax) && parbone ){

//		if( (movecnt != 0) && (curbone->m_notransik != 0) ){//!!!!!!!!!!!!!!!!!!!!!!!!!
//			break;
//		}
		if( (curbone->m_notransik != 0) ){//!!!!!!!!!!!!!!!!!!!!!!!!!
			break;
		}


		*lastparent = parboneno;

		ret = GetBonePos( mvno, matWorld, 1, &mvobj );
		if( ret ){
			DbgOut( "MoveBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}

		ret = GetBonePos( parboneno, matWorld, 1, &parobj );
		if( ret ){
			DbgOut( "MoveBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}

		if( movecnt == 0 ){

			//float radxz, rady;
			//radxz = m_d3dapp->m_degxz * (float)DEG2PAI;
			//rady = m_d3dapp->m_degy * (float)DEG2PAI;

			//planeaxis.x = sinf(radxz) * cosf( rady );
			//planeaxis.y = sinf( rady );
			//planeaxis.z = -cosf(radxz) * cosf( rady );

			diffmvtarget = targetobj - mvobj;
			dist = D3DXVec3Length( &diffmvtarget );
			
//////////
			float arrowradxz, arrowrady;
			if( defaxis == 0 ){
				if( g_motdlg->AxisGetArrowMode() != ARROWMODE_CROSS ){
					arrowradxz = g_motdlg->AxisGetArrowDegxz() * (float)DEG2PAI;
					arrowrady = g_motdlg->AxisGetArrowDegy() * (float)DEG2PAI;

					rotaxis.x = sinf(arrowradxz) * cosf( arrowrady );
					rotaxis.y = sinf( arrowrady );
					rotaxis.z = -cosf(arrowradxz) * cosf( arrowrady );

				}else{
					
					D3DXVECTOR3 par2mv, par2targ;
					par2mv = mvobj - parobj;
					DXVec3Normalize( &par2mv, &par2mv );
					par2targ = targetobj - parobj;
					DXVec3Normalize( &par2targ, &par2targ );

					D3DXVec3Cross( &rotaxis, &par2mv, &par2targ );
					DXVec3Normalize( &rotaxis, &rotaxis );
					
					/***
					arrowradxz = g_motdlg->AxisGetArrowDegxz() * (float)DEG2PAI;
					arrowrady = g_motdlg->AxisGetArrowDegy() * (float)DEG2PAI;

					planeaxis.x = sinf(arrowradxz) * cosf( arrowrady );
					planeaxis.y = sinf( arrowrady );
					planeaxis.z = -cosf(arrowradxz) * cosf( arrowrady );


					D3DXVECTOR3 par2mv, par2targ, parsdw0;

					ret = GetShadow( &parsdw0, planeaxis, mvobj, parobj, planeaxis );
					if( ret )
						return 0;

					par2mv = mvobj - parsdw0;
					DXVec3Normalize( &par2mv, &par2mv );
					par2targ = targetobj - parsdw0;
					DXVec3Normalize( &par2targ, &par2targ );

					//D3DXVec3Cross( &rotaxis, &par2mv, &par2targ );
					//DXVec3Normalize( &rotaxis, &rotaxis );
					CQuaternion axisq0;
					float rotrad0;
					axisq0.RotationArc( par2mv, par2targ );
					axisq0.GetAxisAndRot( &rotaxis, &rotrad0 );
					DXVec3Normalize( &rotaxis, &rotaxis );
					***/
				}
			}else{
				rotaxis = srcaxis;
			}

			*axisdone = rotaxis;

		}

		if( parbone->m_ikskip != 0 ){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			goto movebonenext;
		}


		if( (targetobj.x != mvobj.x) || (targetobj.y != mvobj.y) || (targetobj.z != mvobj.z) ){		
			

			D3DXVECTOR3 targetsdw;
			D3DXVECTOR3 parsdw2mv, parsdw2target;
			ret = GetShadow( &parsdw, rotaxis, mvobj, parobj, rotaxis );
			if( ret )
				return 0;

			ret = GetShadow( &targetsdw, rotaxis, mvobj, targetobj, rotaxis );
			if( ret )
				return 0;

			parsdw2mv = mvobj - parsdw;
			DXVec3Normalize( &parsdw2mv, &parsdw2mv );


			parsdw2target = targetsdw - parsdw;
			DXVec3Normalize( &parsdw2target, &parsdw2target );

			CQuaternion multq1, multq2;
			multq1.RotationArc( parsdw2mv, parsdw2target );
			multq1.normalize();

//////////////
			multq2 = -multq1;
			multq2.w = multq1.w;


ret = GetQ( curboneno, &saveq );
D3DXVECTOR3 saveeul;
ret = GetEul( curboneno, &saveeul );

			//1回目
			int forcemove = 0;
ret = SetMotionPointIK( m_current, matWorld, curboneno, multq1, (movecnt == (movecntmax - 1)), &forcemove, 0 );
			_ASSERT( !ret );
			UpdateMatrixSelected( curboneno, 1, 0 );

			//！！！boneのスクリーン座標を更新するために、FrameMove	が必要！！！
			m_d3dapp->SetBoneArray( parboneno );
			m_d3dapp->FrameMove( parboneno, 1 );


			ret = GetBonePos( mvno, matWorld, 1, &mvobj );
			if( ret ){
				DbgOut( "MoveBone : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			diffmvtarget = targetobj - mvobj;
			dist1 = D3DXVec3Length( &diffmvtarget );
			

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//if( (forcemove == 0) && (dist <= dist1) ){
			if( dist <= dist1 ){
				//近づかなかったら元に戻す。
				ret = SetQ( curboneno, saveq );
				ret = SetEul( curboneno, saveeul );
				m_d3dapp->FrameMove( parboneno, 1 );
			}else{
				//ope targetにセット
				CQuaternion tmpq;
				GetStandardQ( curboneno, &tmpq );
				ret = SetQOpTarget( curboneno, multq1, tmpq, 0 );
				_ASSERT( !ret );
			}
/***			
			//２回目
ret = SetQ( curboneno, saveq );
			if( ret )
				return 0;
ret = SetMotionPointIK( matWorld, curboneno, multq2, (movecnt == (movecntmax - 1)) );
			_ASSERT( !ret );
			//SetBoneArray( parboneno );
			m_d3dapp->FrameMove( parboneno, 1 );
						

			ret = GetBonePos( mvno, matWorld, 1, &mvobj );
			if( ret ){
				DbgOut( "MoveBone : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}


			diffmvtarget = targetobj - mvobj;
			dist2 = D3DXVec3Length( &diffmvtarget );



			if( dist1 < dist2 ){
ret = SetQ( curboneno, saveq );
				if( ret )
					return 0;
ret = SetMotionPointIK( matWorld, curboneno, multq1, (movecnt == (movecntmax - 1)) );
				_ASSERT( !ret );
				//SetBoneArray( parboneno );
				m_d3dapp->FrameMove( parboneno, 1 );

			}else{
				//2回目の計算結果をそのまま、採用。することなし。
			}
***/			
		}

		//ikboneno = parboneno;

		goto movebonenext;

movebonenext:

		curboneno = parboneno;//!!!!!!!!
		curbone = parbone;

		parbone = m_shandler->FindUpperJoint( parbone );
		if( !parbone )
			break;
		parboneno = parbone->serialno;
		movecnt++;


		ikboneno = parboneno;//!!!!!!

	}


	return 0;
}

int CMotParamDlg::MoveBoneZa4( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent )
{
	*lastparent = 0;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;

	int ret;
	D3DXVECTOR3 mvobj;
//	D3DXMATRIX inimat;
//	D3DXMatrixIdentity( &inimat );

//	ret = GetBonePos( mvno, inimat, 1, &mvobj );
	ret = GetBonePos( mvno, m_shandler->m_gpdata.m_matWorld, 1, &mvobj );
	if( ret ){
		DbgOut( "MoveBoneZa4 : GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 0;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	
	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 0.0f;
	ret = m_d3dapp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 1.0f;
	ret = m_d3dapp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = m_d3dapp->m_degxz * (float)DEG2PAI;
	rady = m_d3dapp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 temptarget;
	ret = GetShadow( &temptarget, planeaxis, mvobj, targetobj0, targetdir );
	if( ret )
		return 0;

	targetobj = mvobj + 0.333f * ( temptarget - mvobj );

	ret = MoveBoneZa4( matWorld, mvno, targetobj, lastparent, -1 );
	if( ret ){
		DbgOut( "motparamdlg : MoveBone : MoveBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( *lastparent > 0 ){
		//ret = m_shandler->CalcZa4LocalQ( m_mhandler, m_mot_cookie, m_current );
		//_ASSERT( !ret );
		SetParBoneArray( mvno );
		Rotate2IKTargetZa4Req( matWorld, *lastparent, 0 );
		//_ASSERT( 0 );
	}

	Redraw();

	return 0;
}


int CMotParamDlg::MoveBone( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent )
{
	*lastparent = 0;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;

	int ret;
	D3DXVECTOR3 mvobj;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	ret = GetBonePos( mvno, inimat, 1, &mvobj );
	if( ret ){
		DbgOut( "MoveBone : GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 0;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	
	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 0.0f;
	ret = m_d3dapp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 1.0f;
	ret = m_d3dapp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = m_d3dapp->m_degxz * (float)DEG2PAI;
	rady = m_d3dapp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 temptarget;
	ret = GetShadow( &temptarget, planeaxis, mvobj, targetobj0, targetdir );
	if( ret )
		return 0;

	targetobj = mvobj + 0.333f * ( temptarget - mvobj );

	D3DXVECTOR3 dummyaxis( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 axisdone;
	//ret = MoveBone( matWorld, mvno, targetobj, lastparent, 0, dummyaxis, &axisdone, -1 );
	ret = MoveBone( inimat, mvno, targetobj, lastparent, 0, dummyaxis, &axisdone, -1 );
	if( ret ){
		DbgOut( "motparamdlg : MoveBone : MoveBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( *lastparent > 0 ){
		SetParBoneArray( mvno );
		//Rotate2IKTargetReq( matWorld, *lastparent, axisdone, 0 );
		Rotate2IKTargetReq( inimat, *lastparent, axisdone, 0 );
		//_ASSERT( 0 );
	}

	Redraw();

	return 0;
}

void CMotParamDlg::Rotate2IKTargetZa4Req( D3DXMATRIX matWorld, int mvno, int broflag )
{

	int ret;
	CShdElem* curselem;
	curselem = (*m_shandler)( mvno );

	if( (ExistInParBoneArray( mvno ) == 0) && (curselem->m_iktarget == 1) ){

		int maxlevel = 0;

		CShdElem* chkselem = curselem;
	
		while( chkselem && (maxlevel < curselem->m_iktargetlevel) ){
			if( (ExistInParBoneArray( chkselem->serialno ) == 1) || ((maxlevel != 0) && (chkselem->m_iktarget == 1)) || 
				(chkselem->m_notransik != 0) ){
				break;
			}
			
			chkselem = m_shandler->FindUpperJoint( chkselem, 0 );
			maxlevel++;
		}

		if( maxlevel > 0 ){
			int calccnt;
			int lastparent;

			for( calccnt = 0; calccnt < curselem->m_iktargetcnt; calccnt++ ){

				//ret = m_shandler->CalcZa4LocalQ( m_mhandler, m_mot_cookie, m_current );
				//_ASSERT( !ret );

				ret = MoveBoneZa4( matWorld, mvno, curselem->m_iktargetpos, &lastparent, maxlevel );
				if( ret ){
					DbgOut( "motparamdlg : Rotate2IKTargetZa4Req : MoveBoneZa4 error !!!\n" );
					_ASSERT( 0 );
					return;
				}
			}
		}
		//_ASSERT( 0 );

	}
///////
	CShdElem* chilselem;
	CShdElem* broselem;

	chilselem = curselem->child;
	broselem = curselem->brother;

	if( broselem && (broflag == 1)){
		Rotate2IKTargetZa4Req( matWorld, broselem->serialno, 1 );
	}

	if( chilselem ){
		Rotate2IKTargetZa4Req( matWorld, chilselem->serialno, 1 );
	}

}


void CMotParamDlg::Rotate2IKTargetReq( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 rotaxis, int broflag )
{

	int ret;
	CShdElem* curselem;
	curselem = (*m_shandler)( mvno );

	if( (ExistInParBoneArray( mvno ) == 0) && (curselem->m_iktarget == 1) ){

		int maxlevel = 0;

		CShdElem* chkselem = curselem;
	
		while( chkselem && (maxlevel < curselem->m_iktargetlevel) ){
			if( (ExistInParBoneArray( chkselem->serialno ) == 1) || ((maxlevel != 0) && (chkselem->m_iktarget == 1)) || 
				(chkselem->m_notransik != 0) ){
				break;
			}
			
			chkselem = m_shandler->FindUpperJoint( chkselem, 0 );
			maxlevel++;
		}

		if( maxlevel > 0 ){
			int calccnt;
			int lastparent;
			D3DXVECTOR3 axisdone;

			for( calccnt = 0; calccnt < curselem->m_iktargetcnt; calccnt++ ){

				//ret = MoveBone( matWorld, mvno, curselem->m_iktargetpos, &lastparent, 1, rotaxis, &axisdone, maxlevel );
				ret = MoveBone( matWorld, mvno, curselem->m_iktargetpos, &lastparent, 0, rotaxis, &axisdone, maxlevel );
				if( ret ){
					DbgOut( "motparamdlg : Rotate2IKTargetReq : MoveBone error !!!\n" );
					_ASSERT( 0 );
					return;
				}
			}
		}
		//_ASSERT( 0 );

	}
///////
	CShdElem* chilselem;
	CShdElem* broselem;

	chilselem = curselem->child;
	broselem = curselem->brother;

	if( broselem && (broflag == 1)){
		Rotate2IKTargetReq( matWorld, broselem->serialno, rotaxis, 1 );
	}

	if( chilselem ){
		Rotate2IKTargetReq( matWorld, chilselem->serialno, rotaxis, 1 );
	}

}



/***
int CMotParamDlg::MoveBone( int mvno, float srcx, float srcy, int* lastparent )
{

	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;

	int ret;	
	//float mvprerhw, parprerhw;
	//D3DXVECTOR3 mvpre, parpre;

	D3DXVECTOR3 mvobj, parobj;
	D3DXVECTOR3 parsdw;
	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	D3DXVECTOR3 planeaxis;
	D3DXVECTOR3 rotaxis;

	D3DXVECTOR3 diffmvtarget;

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );


	//float dot, rotrad;
	float dist;
	float dist1, dist2;

	int movecntmax;
	int movecnt;

	CQuaternion saveq, saveq1st, saveq2nd;

	movecntmax = m_ik_bonenum;

	CShdElem* mvbone = (*m_shandler)( mvno );
	if( !mvbone )
		return 0;

	CShdElem* parbone;
	int parboneno;

	parbone = m_shandler->FindUpperJoint( mvbone );
	if( !parbone )
		return 0;
	parboneno = parbone->serialno;

	movecnt = 0;

	int ikboneno = parboneno;
	//int ikboneno = mvno;
	int curboneno = mvno;


	while( (movecnt < movecntmax) && parbone ){
	
		*lastparent = parboneno;

		ret = GetBonePos( mvno, inimat, 1, &mvobj );
		if( ret ){
			DbgOut( "MoveBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}

		ret = GetBonePos( parboneno, inimat, 1, &parobj );
		if( ret ){
			DbgOut( "MoveBone : GetBonePos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}


		if( movecnt == 0 ){
			// targetobj は、初回のみ計算。
			targetsc.x = srcx;
			targetsc.y = srcy;
			targetsc.z = 0.0f;
			ret = m_d3dapp->ScreenToObj( &targetobj0, targetsc );
			if( ret )
				return 0;

			targetsc.x = srcx;
			targetsc.y = srcy;
			targetsc.z = 1.0f;
			ret = m_d3dapp->ScreenToObj( &targetobj1, targetsc );
			if( ret )
				return 0;

			D3DXVECTOR3 targetdir;
			targetdir = targetobj1 - targetobj0;
			DXVec3Normalize( &targetdir, &targetdir );

			float radxz, rady;
			radxz = m_d3dapp->m_degxz * (float)DEG2PAI;
			rady = m_d3dapp->m_degy * (float)DEG2PAI;

			planeaxis.x = sinf(radxz) * cosf( rady );
			planeaxis.y = sinf( rady );
			planeaxis.z = -cosf(radxz) * cosf( rady );


			D3DXVECTOR3 temptarget;
			ret = GetShadow( &temptarget, planeaxis, mvobj, targetobj0, targetdir );
			if( ret )
				return 0;

			targetobj = mvobj + 0.333f * ( temptarget - mvobj );

			diffmvtarget = targetobj - mvobj;
			dist = D3DXVec3Length( &diffmvtarget );

//////////
			float arrowradxz, arrowrady;
			if( g_motdlg->AxisGetArrowMode() != ARROWMODE_CROSS ){
				arrowradxz = g_motdlg->AxisGetArrowDegxz() * (float)DEG2PAI;
				arrowrady = g_motdlg->AxisGetArrowDegy() * (float)DEG2PAI;

				rotaxis.x = sinf(arrowradxz) * cosf( arrowrady );
				rotaxis.y = sinf( arrowrady );
				rotaxis.z = -cosf(arrowradxz) * cosf( arrowrady );

			}else{
				D3DXVECTOR3 par2mv, par2targ;
				par2mv = mvobj - parobj;
				DXVec3Normalize( &par2mv, &par2mv );
				par2targ = targetobj - parobj;
				DXVec3Normalize( &par2targ, &par2targ );

				D3DXVec3Cross( &rotaxis, &par2mv, &par2targ );
				DXVec3Normalize( &rotaxis, &rotaxis );

			}

		}

		//DbgOut( "viewer : MoveBone : \n" );
		//DbgOut( "\tsrcx %f, srcy %f\n", srcx, srcy );
		//DbgOut( "\tmvpre %f, %f, %f, %f\n", mvpre.x, mvpre.y, mvpre.z, mvprerhw );
		//DbgOut( "\tmvobj %f, %f, %f\n", mvobj.x, mvobj.y, mvobj.z );
		//DbgOut( "\ttargetobj %f, %f, %f\n", targetobj.x, targetobj.y, targetobj.z );
		//DbgOut( "\tparpre %f, %f, %f %f\n", parpre.x, parpre.y, parpre.z, parprerhw );
		//DbgOut( "\tparobj %f, %f, %f\n", parobj.x, parobj.y, parobj.z );
		


		if( (targetobj.x != mvobj.x) || (targetobj.y != mvobj.y) || (targetobj.z != mvobj.z) ){		
			

			D3DXVECTOR3 targetsdw;
			D3DXVECTOR3 parsdw2mv, parsdw2target;
			ret = GetShadow( &parsdw, rotaxis, mvobj, parobj, rotaxis );
			if( ret )
				return 0;

			ret = GetShadow( &targetsdw, rotaxis, mvobj, targetobj, rotaxis );
			if( ret )
				return 0;

			parsdw2mv = mvobj - parsdw;
			DXVec3Normalize( &parsdw2mv, &parsdw2mv );


			parsdw2target = targetsdw - parsdw;
			DXVec3Normalize( &parsdw2target, &parsdw2target );

			CQuaternion multq1, multq2;
			multq1.RotationArc( parsdw2mv, parsdw2target );
			multq1.normalize();

//////////////


			multq2 = -multq1;
			multq2.w = multq1.w;

			

			ret = GetQ( ikboneno, &saveq );


			//1回目
			ret = SetMotionPointIK( ikboneno, multq1, (movecnt == (movecntmax - 1)) );
			_ASSERT( !ret );


			//！！！boneのスクリーン座標を更新するために、FrameMove	が必要！！！
			m_d3dapp->SetBoneArray( parboneno );
			m_d3dapp->FrameMove( parboneno, 1 );


			ret = GetBonePos( mvno, inimat, 1, &mvobj );
			if( ret ){
				DbgOut( "MoveBone : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			diffmvtarget = targetobj - mvobj;
			dist1 = D3DXVec3Length( &diffmvtarget );
			

			
			//２回目
			ret = SetQ( ikboneno, saveq );
			if( ret )
				return 0;
			ret = SetMotionPointIK( ikboneno, multq2, (movecnt == (movecntmax - 1)) );
			_ASSERT( !ret );
			//SetBoneArray( parboneno );
			m_d3dapp->FrameMove( parboneno, 1 );
						

			ret = GetBonePos( mvno, inimat, 1, &mvobj );
			if( ret ){
				DbgOut( "MoveBone : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}


			diffmvtarget = targetobj - mvobj;
			dist2 = D3DXVec3Length( &diffmvtarget );



			if( dist1 < dist2 ){
				ret = SetQ( ikboneno, saveq );
				if( ret )
					return 0;
				ret = SetMotionPointIK( ikboneno, multq1, (movecnt == (movecntmax - 1)) );
				_ASSERT( !ret );
				//SetBoneArray( parboneno );
				m_d3dapp->FrameMove( parboneno, 1 );

			}else{
				//2回目の計算結果をそのまま、採用。することなし。
			}
			
		}

		//ikboneno = parboneno;

		goto movebonenext;

movebonenext:

		curboneno = parboneno;//!!!!!!!!

		parbone = m_shandler->FindUpperJoint( parbone );
		if( !parbone )
			break;
		parboneno = parbone->serialno;
		movecnt++;


		ikboneno = parboneno;//!!!!!!

	}



	////UpdateVertexBuffer();

	return 0;
}
***/

int CMotParamDlg::ScaleBone( int scaleboneno, int scalekind, float delta )
{


	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );


	int docre, haskey = 0;
	m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}

	if( (docre == 0) && (haskey == 0) ){
		return 0;
	}


	D3DXVECTOR3 diffvec;
	switch( scalekind ){
	case 0:
		diffvec.x = delta;
		diffvec.y = 0.0f;
		diffvec.z = 0.0f;
		break;
	case 1:
		diffvec.x = 0.0f;
		diffvec.y = delta;
		diffvec.z = 0.0f;
		break;
	case 2:
		diffvec.x = 0.0f;
		diffvec.y = 0.0f;
		diffvec.z = delta;
		break;
	case 3:
		diffvec.x = delta;
		diffvec.y = delta;
		diffvec.z = delta;
		break;

	default:
		_ASSERT( 0 );
		diffvec.x = 0.0f;
		diffvec.y = 0.0f;
		diffvec.z = 0.0f;
		break;
	}
///////////////
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

	CMotionCtrl* mcptr = (*m_mhandler)( scaleboneno );//注意！！
	_ASSERT( mcptr );

	int frameno;
	CMotionPoint2* firstmp;
	CMotionPoint2* curmp;
	firstmp = m_mhandler->GetFirstMotionPoint( scaleboneno, m_mot_cookie );

	curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( !curmp ){
		ret = CreateMotionPoints( mcptr->serialno, m_mot_cookie, m_current );
		if( ret )
			return -1;
		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
		if( !curmp )
			return -1;
	}
	if( !firstmp ){
		firstmp = curmp;
	}


	D3DXVECTOR3 firstscale;
	int setcnt = 0;
	while( firstmp ){
		frameno = firstmp->m_frameno;
		if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){
			m_q = firstmp->m_q;

			m_mvx = firstmp->m_mvx;
			m_mvy = firstmp->m_mvy;
			m_mvz = firstmp->m_mvz;
			//m_dispswitch = firstmp->dispswitch;
			m_interp = firstmp->interp;

			if( setcnt == 0 ){
				firstscale.x = firstmp->m_scalex;
				firstscale.y = firstmp->m_scaley;
				firstscale.z = firstmp->m_scalez;

				firstscale += diffvec;
			}

			if( m_soutai == OP_SOUTAI ){
				m_scalex = firstmp->m_scalex + diffvec.x;
				m_scaley = firstmp->m_scaley + diffvec.y;
				m_scalez = firstmp->m_scalez + diffvec.z;

			}else{
				m_scalex = firstscale.x;
				m_scaley = firstscale.y;
				m_scalez = firstscale.z;
			}

			if( m_scalex < 0.0001f )
				m_scalex = 0.0001f;//!!!!!!!!!!!!!!!!!!!
			if( m_scaley < 0.0001f )
				m_scaley = 0.0001f;
			if( m_scalez < 0.0001f )
				m_scalez = 0.0001f;
			m_userint1 = firstmp->m_userint1;

			ret = firstmp->SetParams( m_current, m_q, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
			_ASSERT( !ret );
		}else if( frameno > m_optardlg->m_endframe ){
			break;
		}
		setcnt++;
		firstmp = firstmp->next;
	}

	//UpdateAllMatrix();
	UpdateMatrixSelected( scaleboneno, 1, 0 );


	Redraw();

	return 0;

}


int CMotParamDlg::ShiftBone( int shiftno, int shiftkind, float delta, int t2flag, int localflag, CQuaternion localq, int za4flag )
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;


	int docre, haskey;
	ret = m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}

	if( (docre == 0) && (haskey == 0) ){
		return 0;
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!! この関数では、ボーン変形を考慮した、ローカル座標で計算を行う。
	//!!!! matWorldが初期状態でない場合は、diffvecを、invmatworldで、逆変換する。
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	D3DXVECTOR3 diffvec;
	switch( shiftkind ){
	case 0:
		if( localflag == 0 ){
			diffvec.x = delta;
			diffvec.y = 0.0f;
			diffvec.z = 0.0f;
		}else{
			D3DXVECTOR3 befdiff;
			befdiff.x = delta;
			befdiff.y = 0.0f;
			befdiff.z = 0.0f;

			localq.Rotate( &diffvec, befdiff );
		}
		break;
	case 1:
		if( localflag == 0 ){
			diffvec.x = 0.0f;
			diffvec.y = delta;
			diffvec.z = 0.0f;
		}else{
			D3DXVECTOR3 befdiff;
			befdiff.x = 0.0f;
			befdiff.y = delta;
			befdiff.z = 0.0f;

			localq.Rotate( &diffvec, befdiff );
		}
		break;
	case 2:
		if( localflag == 0 ){
			diffvec.x = 0.0f;
			diffvec.y = 0.0f;
			diffvec.z = delta;
		}else{
			D3DXVECTOR3 befdiff;
			befdiff.x = 0.0f;
			befdiff.y = 0.0f;
			befdiff.z = delta;

			localq.Rotate( &diffvec, befdiff );
		}
		break;
	default:
		_ASSERT( 0 );
		if( localflag == 0 ){
			diffvec.x = delta;
			diffvec.y = 0.0f;
			diffvec.z = 0.0f;
		}else{
			D3DXVECTOR3 befdiff;
			befdiff.x = delta;
			befdiff.y = 0.0f;
			befdiff.z = 0.0f;

			localq.Rotate( &diffvec, befdiff );
		}
		break;
	}

	int orgmvno = 0;

	m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );

	if( t2flag == 0 ){

		CMotionPoint2* chkmp = 0;
		ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, shiftno, m_current, &chkmp );
		_ASSERT( !ret );
		if( !chkmp ){
			ret = CreateMotionPoints( shiftno, m_mot_cookie, m_current );
			_ASSERT( !ret );
		}


		D3DXVECTOR3 firsttarget;
		int frameno;

		CMotionPoint2* calcmp;
		calcmp = m_mhandler->GetFirstMotionPoint( shiftno, m_mot_cookie );
		while( calcmp ){
			frameno = calcmp->m_frameno;
			if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){

				D3DXMATRIX inimat;
				D3DXMatrixIdentity( &inimat );
				D3DXVECTOR3 bonepos;
				ret = GetBonePosOnFrame( frameno, shiftno, inimat, 1, &bonepos );
				if( ret ){
					DbgOut( "motparamdlg : ShiftBone vec : GetBonePos error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				if( frameno == m_optardlg->m_startframe ){
					firsttarget = bonepos + diffvec;
				}

				D3DXVECTOR3 target;
				if( m_soutai == OP_SOUTAI ){
					target = bonepos + diffvec;
				}else{
					target = firsttarget;
				}

				ret = ShiftBone( frameno, shiftno, target, &orgmvno, za4flag, calcmp );
				_ASSERT( !ret );
			}else if( frameno > m_optardlg->m_endframe ){
				break;
			}
			calcmp = calcmp->next;
		}
		UpdateAllMatrix( 0 );
		Redraw();

	}else{
		
		int topjointno = 0;
		ret = GetTopJointNo( shiftno, &topjointno );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone delta GetTopJointNo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( topjointno <= 0 )
			return 0;

		CMotionPoint2* chkmp = 0;
		ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, topjointno, m_current, &chkmp );
		_ASSERT( !ret );
		if( !chkmp ){
			ret = CreateMotionPoints( topjointno, m_mot_cookie, m_current );
			_ASSERT( !ret );
		}


		D3DXVECTOR3 bonediff;
		D3DXVECTOR3 firsttarget;
		int frameno;
		CMotionPoint2* calcmp;
		calcmp = m_mhandler->GetFirstMotionPoint( topjointno, m_mot_cookie );
		while( calcmp ){
			frameno = calcmp->m_frameno;
			if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){
				D3DXMATRIX inimat;
				D3DXMatrixIdentity( &inimat );
				D3DXVECTOR3 bonepos;
				ret = GetBonePosOnFrame( frameno, shiftno, inimat, 1, &bonepos );
				if( ret ){
					DbgOut( "motparamdlg : ShiftBone vec : GetBonePos error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				if( frameno == m_optardlg->m_startframe ){
					firsttarget = bonepos + diffvec;
				}

				if( m_soutai == OP_SOUTAI ){
					bonediff = diffvec;
				}else{
					bonediff = firsttarget - bonepos;
				}

				ret = ShiftBoneDiff( frameno, topjointno, bonediff, calcmp );
				if( ret ){
					DbgOut( "motparamdlg : ShiftBone : delta : ShiftBoneDiff error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( frameno > m_optardlg->m_endframe ){
				break;
			}
			calcmp = calcmp->next;
		}
		orgmvno = topjointno;
		UpdateAllMatrix( 0 );
		Redraw();
		//m_d3dapp->UpdateVertexBuffer();

	}

	if( orgmvno > 0 ){
		SetParBoneArray( orgmvno );
		if( za4flag ){
			//Rotate2IKTargetZa4Req( inimat, orgmvno, 0 );
			Rotate2IKTargetZa4Req( m_shandler->m_gpdata.m_matWorld, orgmvno, 0 );
		}else{
			D3DXVECTOR3 axisdone;
			//Rotate2IKTargetReq( inimat, orgmvno, axisdone, 0 );
			Rotate2IKTargetReq( m_shandler->m_gpdata.m_matWorld, orgmvno, axisdone, 0 );
		}
	}

	Redraw();


	return 0;
}

int CMotParamDlg::GetTopJointNo( int srcseri, int* topseri )
{
	if( (srcseri <= 0) || (srcseri >= m_shandler->s2shd_leng) ){
		DbgOut( "motparamdlg : GetTopJointNo : srcseri error !!!\n" );
		_ASSERT( 0 );
		*topseri = 0;
		return 1;
	}

	CShdElem* srcelem;
	srcelem = (*m_shandler)( srcseri );

	CShdElem* curselem;
	CShdElem* upperelem = 0;
	CShdElem* befupperelem = 0;

	curselem = srcelem;
	upperelem = srcelem;
	befupperelem = srcelem;
	while( curselem ){

		befupperelem = upperelem;
		upperelem = m_shandler->FindUpperJoint( curselem, 0 );

		curselem = upperelem;
	}

	if( befupperelem ){
		*topseri = befupperelem->serialno;
	}else{
		//なし
		*topseri = 0;
	}

	return 0;
}
int CMotParamDlg::ShiftBoneDiff( int frameno, int shiftno, D3DXVECTOR3 diff, CMotionPoint2* calcmp )
{
	
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	CShdElem* shiftelem;
	shiftelem = (*m_shandler)( shiftno );

	CMotionCtrl* mcptr;
	mcptr = (*m_mhandler)( shiftno );

	ret = SetMotionPointIK( frameno, shiftno, diff.x, diff.y, diff.z, calcmp );
	_ASSERT( !ret );


	return 0;
}


int CMotParamDlg::ShiftBone( int frameno, int shiftno, D3DXVECTOR3 target, int* setnoptr, int za4flag, CMotionPoint2* calcmp )
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;


	if( frameno < 0 ){
		frameno = m_current;
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!! この関数では、ボーン変形を考慮した、ローカル座標で計算を行う。
	//!!!! matWorldが初期状態でない場合は、targetを、invmatworldで、逆変換してから渡す。
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	D3DXVECTOR3 orgobj;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	ret = GetBonePos( shiftno, inimat, 0, &orgobj );
	if( ret ){
		DbgOut( "motparamdlg : ShiftBone vec : GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXMATRIX mat1, raw1, invraw1, par1, mat2;
	float rate1, rate2;
	int boneno1, boneno2;

	int infnum = 0;
	ret = m_shandler->GetInfElemNum( shiftno, 0, &infnum );
	if( ret || (infnum < 1) ){
		DbgOut( "motparamdlg : ShiftBone vec : sh GetInfElemNum error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_shandler->GetInfElemParam( shiftno, 0, 0, &boneno1, &rate1 );
	if( ret ){
		DbgOut( "motparamdlg : ShiftBone vec : sh GetInfElemParam error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( infnum >= 2 ){
		ret = m_shandler->GetInfElemParam( shiftno, 0, 1, &boneno2, &rate2 );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone vec : sh GetInfElemParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		boneno2 = -1;
		rate2 = 0.0f;
	}


	if( boneno1 > 0 ){
		ret = m_mhandler->HuGetBoneMatrix( boneno1, m_mot_cookie, frameno, &mat1 );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone vec : HuGetBoneMatrix error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = m_mhandler->HuGetRawMatrix( boneno1, m_mot_cookie, frameno, &raw1 );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone vec : HuGetRawMatrix error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		D3DXMatrixIdentity( &mat1 );
		D3DXMatrixIdentity( &raw1 );
	}
	if( boneno2 > 0 ){
		ret = m_mhandler->HuGetBoneMatrix( boneno2, m_mot_cookie, frameno, &mat2 );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone vec : HuGetBoneMatrix error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		D3DXMatrixIdentity( &mat2 );
	}


	D3DXMatrixInverse( &invraw1, NULL, &raw1 );

	//par1 = invraw1 * mat1;

	CShdElem* shiftelem;
	shiftelem = (*m_shandler)( shiftno );

	CShdElem* setelem;
	setelem = m_shandler->FindUpperJoint( shiftelem );
	if( !setelem ){
		setelem = shiftelem;//!!!!!!!!!!!!!!
	}


	/***
	CShdElem* parelem;
	int parno;
	parelem = m_shandler->FindUpperJoint( shiftelem );
	if( parelem ){
		parno = parelem->serialno;
		ret = m_mhandler->HuGetBoneMatrix( parno, &par1 );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone vec : HuGetBoneMatrix error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		D3DXMatrixIdentity( &par1 );
	}
	***/
	CShdElem* bone1;
	bone1 = (*m_shandler)( boneno1 );
	_ASSERT( bone1 );

	CShdElem* parelem;
	int parno;
	parelem = m_shandler->FindUpperJoint( bone1 );
	if( parelem ){
		parno = parelem->serialno;
		ret = m_mhandler->HuGetBoneMatrix( parno, m_mot_cookie, frameno, &par1 );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone vec : HuGetBoneMatrix error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		D3DXMatrixIdentity( &par1 );
	}


///////
	// org * raw1 * mvm * par1 = ( target - mat2 * org * rate2 ) / rate1

	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord( &pos2, &orgobj, &mat2 );

	float alpha, beta, ganma;
	alpha = ( target.x - pos2.x * rate2 ) / rate1;
	beta = ( target.y - pos2.y * rate2 ) / rate1;
	ganma = ( target.z - pos2.z * rate2 ) / rate1;

////////
	float a11, a12, a13, a21, a22, a23, a31, a32, a33, a41, a42, a43;
	a11 = raw1._11;
	a12 = raw1._12;
	a13 = raw1._13;

	a21 = raw1._21;
	a22 = raw1._22;
	a23 = raw1._23;

	a31 = raw1._31;
	a32 = raw1._32;
	a33 = raw1._33;

	a41 = raw1._41;
	a42 = raw1._42;
	a43 = raw1._43;

	float b11, b12, b13, b21, b22, b23, b31, b32, b33, b41, b42, b43;
	b11 = par1._11;
	b12 = par1._12;
	b13 = par1._13;

	b21 = par1._21;
	b22 = par1._22;
	b23 = par1._23;

	b31 = par1._31;
	b32 = par1._32;
	b33 = par1._33;

	b41 = par1._41;
	b42 = par1._42;
	b43 = par1._43;

	float c11, c12, c13, c21, c22, c23, c31, c32, c33, c41, c42, c43;
	c11 = a11*b11 + a12*b21 + a13*b31;
	c12 = a11*b12 + a12*b22 + a13*b32;
	c13 = a11*b13 + a12*b23 + a13*b33;

	c21 = a21*b11 + a22*b21 + a23*b31;
	c22 = a21*b12 + a22*b22 + a23*b32;
	c23 = a21*b13 + a22*b23 + a23*b33;

	c31 = a31*b11 + a32*b21 + a33*b31;
	c32 = a31*b12 + a32*b22 + a33*b32;
	c33 = a31*b13 + a32*b23 + a33*b33;

	c41 = b11*a41 + b21*a42 + b31*a43 + b41;
	c42 = b12*a41 + b22*a42 + b32*a43 + b42;
	c43 = b13*a41 + b23*a42 + b33*a43 + b43;

	//zero div check
	if( b33 == 0.0f )
		return 0;
	float B33 = 1.0f / b33;

	float p, q, r, s, t, v, w;
	float x, y, z;

	x = orgobj.x;
	y = orgobj.y;
	z = orgobj.z;


	//zero div check
	if( (b11 - b31 * B33 * b13) == 0.0f )
		return 0;

	p = B33 * ( ganma - c13 * x - c23 * y - c33 * z - c43 );
	q = 1.0f / (b11 - b31 * B33 * b13);
	//r = q * (alpha - c11 * x - c21 * y - c31 * z - c41 - b31 * p - b21);
	r = q * (alpha - c11 * x - c21 * y - c31 * z - c41 - b31 * p);

	s = b12 - b32 * B33 * b13;
	t = beta - c12 * x - c22 * y - c32 * z - c42 - b32 * p;

	v = b21 - b31 * B33 * b23;
	w = b22 - b32 * B33 * b23;


	////zero div check
	//if( (s * q * b31 * B33 * b23 + b22 - b32 * B33 * b23) == 0.0f )
	//	return 0;
	//u = 1.0f / (s * q * b31 * B33 * b23 + b22 - b32 * B33 * b23);


	float mvx, mvy, mvz;

	//mvy = u * ( t - s * r );
	mvy = ( t -  s * r ) / ( w - s * q * v );

	//mvx = q * b31 * B33 * b23 * u * ( t - s * r ) + r;
	mvx = -q * ( b21 - b31 * B33 * b23 ) * mvy + r;

	mvz = p - B33 * b13 * mvx - B33 * b23 * mvy;


//DbgOut( "motparamdlg : ShiftBone : mv %f %f %f, orgobj %f %f %f\r\n\r\n",
//	   mvx, mvy, mvz,
//	   orgobj.x, orgobj.y, orgobj.z );


	//ret = SetMotionPointIK( shiftno, mvx, mvy, mvz );
	//ret = SetMotionPointIK( setelem->serialno, mvx, mvy, mvz );
	ret = SetMotionPointIK( frameno, bone1->serialno, mvx, mvy, mvz, calcmp );
	_ASSERT( !ret );

	*setnoptr = bone1->serialno;


	return 0;
}


int CMotParamDlg::ShiftBoneTree( int shiftno, D3DXVECTOR3 targetobj, int za4flag )
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;


	int docre, haskey;
	ret = m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}

	if( (docre == 0) && (haskey == 0) ){
		return 0;
	}

	CShdElem* shiftbone = (*m_shandler)( shiftno );
	if( !shiftbone )
		return 0;

	int orgmvno = 0;


	/***
	float shiftscx, shiftscy, shiftscz, wp;	
	ret = shiftbone->GetBoneScreenPos( &shiftscx, &shiftscy, &shiftscz, &wp );
	if( ret != 0 )
		return 0;
	***/

	int topjointno = 0;
	ret = GetTopJointNo( shiftno, &topjointno );
	if( ret ){
		DbgOut( "motparamdlg : ShiftBoneTree : GetTopJointNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( topjointno <= 0 )
		return 0;


	D3DXVECTOR3 bonepos;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	D3DXVECTOR3 firstdiff;
	int frameno;
	CMotionPoint2* calcmp;
	calcmp = m_mhandler->GetFirstMotionPoint( topjointno, m_mot_cookie );
	while( calcmp ){
		frameno = calcmp->m_frameno;
		if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){
			ret = GetBonePosOnFrame( frameno, shiftno, inimat, 1, &bonepos );
			if( ret ){
				DbgOut( "motparamdlg : ShiftBoneTree : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			if( frameno == m_optardlg->m_startframe ){
				firstdiff = targetobj - bonepos;
			}


			D3DXVECTOR3 mvdiff;
			if( m_soutai == OP_SOUTAI ){
				mvdiff = firstdiff;
			}else{
				mvdiff = targetobj - bonepos;
			}

			ret = ShiftBoneDiff( frameno, topjointno, mvdiff, calcmp );
			if( ret ){
				DbgOut( "motparamdlg : ShiftBoneTree : ShiftBoneDiff error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else if( frameno > m_optardlg->m_endframe ){
			break;
		}
		calcmp = calcmp->next;
	}
	orgmvno = topjointno;

	UpdateAllMatrix( 0 );
	Redraw();


	if( orgmvno > 0 ){
		D3DXVECTOR3 axisdone;
		D3DXMATRIX inimat;
		D3DXMatrixIdentity( &inimat );
		SetParBoneArray( orgmvno );
		if( za4flag ){
			//ret = m_shandler->CalcZa4LocalQ( m_mhandler, m_mot_cookie, m_current );
			//_ASSERT( !ret );
			Rotate2IKTargetZa4Req( inimat, orgmvno, 0 );
		}else{
			Rotate2IKTargetReq( inimat, orgmvno, axisdone, 0 );
		}
	}

	return 0;
}

int CMotParamDlg::ShiftBone( int shiftno, float srcx, float srcy, int t2flag, int za4flag )
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;


	int docre, haskey;
	ret = m_mhandler->HasKey( m_mot_cookie, m_current, &haskey );
	if( m_notcreate == 0 ){
		docre = 1;
	}else{
		if( haskey ){
			docre = 1;
		}else{
			docre = 0;
		}
	}

	if( (docre == 0) && (haskey == 0) ){
		return 0;
	}

	CShdElem* shiftbone = (*m_shandler)( shiftno );
	if( !shiftbone )
		return 0;

	int orgmvno = 0;


	if( t2flag == 0 ){
		CMotionPoint2* chkmp = 0;
		ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, shiftno, m_current, &chkmp );
		_ASSERT( !ret );
		if( !chkmp ){
			ret = CreateMotionPoints( shiftno, m_mot_cookie, m_current );
			_ASSERT( !ret );
		}


		D3DXVECTOR3 targetsc, targetobj0, targetobj1;
			
		targetsc.x = srcx;
		targetsc.y = srcy;
		targetsc.z = 0.0f;
		ret = m_d3dapp->ScreenToObj( &targetobj0, targetsc );
		if( ret )
			return 0;

		targetsc.x = srcx;
		targetsc.y = srcy;
		targetsc.z = 1.0f;
		ret = m_d3dapp->ScreenToObj( &targetobj1, targetsc );
		if( ret )
			return 0;

		D3DXVECTOR3 targetdir;
		targetdir = targetobj1 - targetobj0;
		DXVec3Normalize( &targetdir, &targetdir );

		float radxz, rady;
		radxz = m_d3dapp->m_degxz * (float)DEG2PAI;
		rady = m_d3dapp->m_degy * (float)DEG2PAI;

		D3DXVECTOR3 planeaxis;
		planeaxis.x = sinf(radxz) * cosf( rady );
		planeaxis.y = sinf( rady );
		planeaxis.z = -cosf(radxz) * cosf( rady );


		D3DXVECTOR3 bonepos;
		D3DXMATRIX inimat;
		D3DXMatrixIdentity( &inimat );


		m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );

		D3DXVECTOR3 firstdiff;
		int frameno;
		CMotionPoint2* calcmp;
		calcmp = m_mhandler->GetFirstMotionPoint( shiftno, m_mot_cookie );
		while( calcmp ){
			frameno = calcmp->m_frameno;
			if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){
				ret = GetBonePosOnFrame( frameno, shiftno, inimat, 1, &bonepos );
				if( ret ){
					DbgOut( "motparamdlg : ShiftBone : GetBonePos error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				D3DXVECTOR3 target;
				ret = GetShadow( &target, planeaxis, bonepos, targetobj0, targetdir );
				if( ret )
					return 0;

				if( frameno == m_optardlg->m_startframe ){
					firstdiff = target - bonepos;
				}

				D3DXVECTOR3 settarget;
				if( m_soutai == OP_SOUTAI ){
					settarget = bonepos + firstdiff;
				}else{
					settarget = target;
				}

				ret = ShiftBone( frameno, shiftno, settarget, &orgmvno, za4flag, calcmp );
				if( ret ){
					DbgOut( "motparamdlg : ShiftBone : ShiftBone target error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( frameno > m_optardlg->m_endframe ){
				break;
			}
			calcmp = calcmp->next;
		}

		UpdateAllMatrix( 0 );
		Redraw();

	}else{

		int topjointno = 0;
		ret = GetTopJointNo( shiftno, &topjointno );
		if( ret ){
			DbgOut( "motparamdlg : ShiftBone : GetTopJointNo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( topjointno <= 0 )
			return 0;


		CMotionPoint2* chkmp = 0;
		ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, topjointno, m_current, &chkmp );
		_ASSERT( !ret );
		if( !chkmp ){
			ret = CreateMotionPoints( topjointno, m_mot_cookie, m_current );
			_ASSERT( !ret );
		}

		D3DXVECTOR3 targetsc, targetobj0, targetobj1;
			
		targetsc.x = srcx;
		targetsc.y = srcy;
		targetsc.z = 0.0f;
		ret = m_d3dapp->ScreenToObj( &targetobj0, targetsc );
		if( ret )
			return 0;

		targetsc.x = srcx;
		targetsc.y = srcy;
		targetsc.z = 1.0f;
		ret = m_d3dapp->ScreenToObj( &targetobj1, targetsc );
		if( ret )
			return 0;

		D3DXVECTOR3 targetdir;
		targetdir = targetobj1 - targetobj0;
		DXVec3Normalize( &targetdir, &targetdir );

		float radxz, rady;
		radxz = m_d3dapp->m_degxz * (float)DEG2PAI;
		rady = m_d3dapp->m_degy * (float)DEG2PAI;

		D3DXVECTOR3 planeaxis;
		planeaxis.x = sinf(radxz) * cosf( rady );
		planeaxis.y = sinf( rady );
		planeaxis.z = -cosf(radxz) * cosf( rady );


		D3DXVECTOR3 bonepos;
		D3DXMATRIX inimat;
		D3DXMatrixIdentity( &inimat );

		D3DXVECTOR3 firstdiff;
		int frameno;
		CMotionPoint2* calcmp;
		calcmp = m_mhandler->GetFirstMotionPoint( topjointno, m_mot_cookie );
		while( calcmp ){
			frameno = calcmp->m_frameno;
			if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){
				ret = GetBonePosOnFrame( frameno, shiftno, inimat, 1, &bonepos );
				if( ret ){
					DbgOut( "motparamdlg : ShiftBone : GetBonePos error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				D3DXVECTOR3 target;
				ret = GetShadow( &target, planeaxis, bonepos, targetobj0, targetdir );
				if( ret )
					return 0;

				if( frameno == m_optardlg->m_startframe ){
					firstdiff = target - bonepos;
				}

				D3DXVECTOR3 mvdiff;
				if( m_soutai == OP_SOUTAI ){
					mvdiff = firstdiff;
				}else{
					mvdiff = target - bonepos;
				}

				ret = ShiftBoneDiff( frameno, topjointno, mvdiff, 0 );
				if( ret ){
					DbgOut( "motparamdlg : ShiftBone : ShiftBoneDiff error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( frameno > m_optardlg->m_endframe ){
				break;
			}
			calcmp = calcmp->next;
		}
		orgmvno = topjointno;

		UpdateAllMatrix( 0 );
		Redraw();

		//m_d3dapp->UpdateVertexBuffer();
	}

	//DbgOut( "viewer : ShiftBone : \n\tbonepos %f %f %f\n\tnewtarget %f %f %f\r\nn\ttarget %f %f %f\r\n",
	//	bonepos.x, bonepos.y, bonepos.z,
	//	newtarget.x, newtarget.y, newtarget.z, 
	//	target.x, target.y, target.z );

	if( orgmvno > 0 ){
		D3DXVECTOR3 axisdone;
		//D3DXMATRIX inimat;
		//D3DXMatrixIdentity( &inimat );
		SetParBoneArray( orgmvno );

		if( za4flag ){
			//ret = m_shandler->CalcZa4LocalQ( m_mhandler, m_mot_cookie, m_current );
			//_ASSERT( !ret );
			Rotate2IKTargetZa4Req( m_shandler->m_gpdata.m_matWorld, orgmvno, 0 );
		}else{
			Rotate2IKTargetReq( m_shandler->m_gpdata.m_matWorld, orgmvno, axisdone, 0 );
		}
	}

	Redraw();

	return 0;
}


int CMotParamDlg::GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir )
{

		//planepを通り、planedirを法線ベクトルとする平面：ax + by + cz + d = 0;
	float a, b, c, d;
	a = planedir.x; b = planedir.y; c = planedir.z;
	d = -a * planep.x - b * planep.y - c * planep.z;

		//平面ax+by+cz+d=0と、点pobj1を通り方向ベクトルdirectionの直線との交点、targetを求める。
		//OpenGL赤本p402参照
	D3DXMATRIX mat;
	mat._11 = b * srcdir.y + c * srcdir.z;
	mat._12 = -a * srcdir.y;
	mat._13 = -a * srcdir.z;
	mat._14 = 0.0f;

	mat._21 = -b * srcdir.x;
	mat._22 = a * srcdir.x + c * srcdir.z;
	mat._23 = -b * srcdir.z;
	mat._24 = 0.0f;

	mat._31 = -c * srcdir.x;
	mat._32 = -c * srcdir.y;
	mat._33 = a * srcdir.x + b * srcdir.y;
	mat._34 = 0.0f;

	mat._41 = -d * srcdir.x;
	mat._42 = -d * srcdir.y;
	mat._43 = -d * srcdir.z;
	mat._44 = a * srcdir.x + b * srcdir.y + c * srcdir.z;

	if( mat._44 == 0.0f )
		return 1;

	dstshadow->x = (srcp.x * mat._11 + srcp.y * mat._21 + srcp.z * mat._31 + mat._41) / mat._44;
	dstshadow->y = (srcp.x * mat._12 + srcp.y * mat._22 + srcp.z * mat._32 + mat._42) / mat._44;
	dstshadow->z = (srcp.x * mat._13 + srcp.y * mat._23 + srcp.z * mat._33 + mat._43) / mat._44;

	return 0;
}
int CMotParamDlg::CopyToClipBoardData( int shdnum, int cpframenum, COPYELEM* srcce, HGLOBAL* cbhgptr )
{
	int ret = 0;
	HGLOBAL hGlobal = 0;
	//int datasize;
	int headerlen, shdnumlen, framenumlen, celen;
	int dstcharsize;

	headerlen = (int)strlen( clipboardheader2 );
	shdnumlen = sizeof( int );
	framenumlen = sizeof( int );
	celen = sizeof( COPYELEM ) * shdnum * cpframenum;
	//datasize = headerlen + numlen + celen;
	dstcharsize = headerlen + (shdnumlen * 2) + (framenumlen * 2) + (celen * 2) + 1;// char以外の1byte --> 2文字、と終端ＮＵＬＬ


    hGlobal = GlobalAlloc( GHND, dstcharsize );
	if( hGlobal == NULL ){
		DbgOut( "motparamdlg : CopyToClipBoardData : GlobalAlloc error !!!\n" );
		_ASSERT( 0 );     
		ret = 1;
		goto cptocbexit;
	}

	char* dstptr;
    dstptr = (char*)GlobalLock( hGlobal );
	if( !dstptr ){
		DbgOut( "motparamdlg : CopyToClipBoardData : GlobalLock error !!!\n" );
		_ASSERT( 0 );
		ret = 1;
		goto cptocbexit;
	}

	int dstpos = 0;
	strcpy_s( dstptr, dstcharsize - dstpos, clipboardheader2 );
	dstpos += headerlen;

	ret = Bin2Char( (unsigned char*)&shdnum, dstptr + dstpos, shdnumlen );
	if( ret ){
		DbgOut( "motparamdlg : CopyToClipBoardData : Bin2Char shdnum error !!!\n" );
		_ASSERT( 0 );
		ret = 1;
		goto cptocbexit;
	}
	dstpos += shdnumlen * 2;


	ret = Bin2Char( (unsigned char*)&cpframenum, dstptr + dstpos, framenumlen );
	if( ret ){
		DbgOut( "motparamdlg : CopyToClipBoardData : Bin2Char cpframenum error !!!\n" );
		_ASSERT( 0 );
		ret = 1;
		goto cptocbexit;
	}
	dstpos += framenumlen * 2;


	ret = Bin2Char( (unsigned char*)srcce, dstptr + dstpos, celen );
	if( ret ){
		DbgOut( "motparamdlg : CopyToClipBoardData : Bin2Char ce error !!!\n" );
		_ASSERT( 0 );
		ret = 1;
		goto cptocbexit;
	}
	dstpos += celen * 2;

	*( dstptr + dstpos ) = 0;//!!!!!

	if( dstpos > dstcharsize ){
		DbgOut( "motparamdlg : CopyToClipBoardData : dstpos error !!!\n" );
		_ASSERT( 0 );
		ret = 1;
		goto cptocbexit;
	}

		
	goto cptocbexit;

cptocbexit:	
	if( hGlobal )
		GlobalUnlock( hGlobal );
	if( ret && hGlobal )
		GlobalFree( hGlobal );

	*cbhgptr = hGlobal;
	return ret;
}

int CMotParamDlg::Bin2Char( unsigned char* srcptr, char* dstptr, int srclen )
{
	int srcpos;
	int dstpos = 0;
	//int ret;
	unsigned char curuc;
	unsigned char lowuc, hiuc;
	char lowc, hic;
	for( srcpos = 0; srcpos < srclen; srcpos++ ){
		curuc = *( srcptr + srcpos );

		hiuc = curuc >> 4;
		if( hiuc <= 9 ){
			hic = '0' + hiuc;
		}else{
			hic = 'A' + hiuc - 10;
		}

		lowuc = curuc & 0x0F;
		if( lowuc <= 9 ){
			lowc = '0' + lowuc;
		}else{
			lowc = 'A' + lowuc - 10;
		}

		*( dstptr + dstpos ) = hic;
		*( dstptr + dstpos + 1 ) = lowc;
		dstpos += 2;
	}

	return 0;
}



int CMotParamDlg::PasteFromClipBoardData( COPYELEM* dstce, int* bufshdnum, int* bufframenum, HGLOBAL cbhg )
{
	*bufshdnum = 0;
	*bufframenum = 0;

	char* srcptr;

	srcptr = (char*)GlobalLock( cbhg );
	if( !srcptr ){
		DbgOut( "motparamdlg : PasteFromClipBoardData : GlobalLock error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int headerleng;
	headerleng = (int)strlen( clipboardheader2 );

	int srcleng;
	srcleng = (int)strlen( srcptr );
	int minimumleng = headerleng + sizeof( int ) * 2 + sizeof( int ) * 2;
	if( srcleng <= minimumleng ){
		DbgOut( "motparamdlg : PasteFromClipBoardData : srcleng 0 error !!!\n" );
		::MessageBox( m_hWnd, "クリップボードにデータがありませんでした。", "ペースト失敗", MB_OK );
		_ASSERT( 0 );
		GlobalUnlock( cbhg );
		return 1;
	}

	int srcpos = 0;
	int cmp0;
	cmp0 = strncmp( srcptr, clipboardheader2, headerleng );
	if( cmp0 ){
		DbgOut( "motparamdlg : PasteFromClipBoardData : invalid data type error !!!\n" );
		::MessageBox( m_hWnd, "クリップボードのデータの種類が違います。", "ペースト失敗", MB_OK );
		_ASSERT( 0 );
		GlobalUnlock( cbhg );
		return 1;
	}
	srcpos += headerleng;


	int ret;
	int shdnum;
	ret = Char2Hex( srcptr + srcpos, (unsigned char*)&shdnum, sizeof( int ) * 2 );
	if( ret ){
		DbgOut( "motparamdlg : PasteFromClipBoard : Char2Hex shdnum error !!!\n" );
		::MessageBox( m_hWnd, "クリップボードのデータが不正です。", "ペースト失敗", MB_OK );	
		_ASSERT( 0 );
		GlobalUnlock( cbhg );
		return 1;
	}
	srcpos += (sizeof( int ) * 2);

	int framenum;
	ret = Char2Hex( srcptr + srcpos, (unsigned char*)&framenum, sizeof( int ) * 2 );
	if( ret || (framenum <= 0) || (framenum > CPFRAMEMAX) ){
		DbgOut( "motparamdlg : PasteFromClipBoard : Char2Hex framenum error !!!\n" );
		::MessageBox( m_hWnd, "クリップボードのデータが不正です。", "ペースト失敗", MB_OK );	
		_ASSERT( 0 );
		GlobalUnlock( cbhg );
		return 1;
	}
	srcpos += (sizeof( int ) * 2);

	int cesize;
	cesize = shdnum * sizeof( COPYELEM ) * framenum;
	int sizeondata;
	sizeondata = srcleng - headerleng - (sizeof( int ) * 2) - (sizeof( int ) * 2);
	if( sizeondata != (cesize * 2) ){
		DbgOut( "motparamdlg : PasteFromClipBoard : data size error !!!\n" );
		::MessageBox( m_hWnd, "クリップボードのデータ長が不正です。", "ペースト失敗", MB_OK );
		_ASSERT( 0 );
		GlobalUnlock( cbhg );
		return 1;
	}


	InitCEBuff();//!!!!!!!!!!!!

	int framecnt;
	int ceno;
	COPYELEM tempce;
	for( framecnt = 0; framecnt < framenum; framecnt++ ){
		for( ceno = 0; ceno < shdnum; ceno++ ){
			ret = Char2Hex( srcptr + srcpos, (unsigned char*)&tempce, sizeof( COPYELEM ) * 2 );
			if( ret ){
				DbgOut( "motparamdlg : PasteFromClipBoard : Char2Hex ce %d %d error !!!\n", ceno, shdnum );
				::MessageBox( m_hWnd, "データ変換中にエラーが生じました。", "ペースト失敗", MB_OK );
				_ASSERT( 0 );
				GlobalUnlock( cbhg );
				return 1;
			}
			srcpos += (sizeof( COPYELEM ) * 2);

			tempce.mp.m_spp = 0;//!!!!!!!!!!

			if( tempce.mp.m_frameno >= 0 ){
			
				//tempceの名前に対応するserialnoを探し、そのmpにデータをコピーする。
				int chkleng;
				chkleng = (int)strlen( tempce.name );
				if( chkleng >= 256 ){
					DbgOut( "motparamdlg : PasteFromClipBoard : check tempce name error %d !!!\n", ceno );
					::MessageBox( m_hWnd, "不正な名前が見つかりました。１", "ペースト失敗", MB_OK );
					_ASSERT( 0 );
					GlobalUnlock( cbhg );
					return 1;
				}
			
				int findno;
				//ret = m_thandler->GetPartNoByName( tempce.name, &findno );
				ret = m_thandler->GetBoneNoByName( tempce.name, &findno, m_shandler, 0 );
				if( ret ){
					DbgOut( "motparamdlg : PasteFromClipBoard : GetPartNoByName error !!!\n" );
					::MessageBox( m_hWnd, "不正な名前が見つかりました。２", "ペースト失敗", MB_OK );
					_ASSERT( 0 );
					GlobalUnlock( cbhg );
					return 1;
				}

				if( findno > 0 ){
					*( m_CEBuff + shdnum * framecnt + findno ) = tempce;//!!!!!!!!
				}
			}
		}
	}

	GlobalUnlock( cbhg );

	*bufshdnum = shdnum;
	*bufframenum = framenum;

	return 0;
}


int CMotParamDlg::Char2Hex( char* orgdata, unsigned char* donedata, DWORD charleng )
{
	int ret;
	DWORD charno;
	DWORD rest;

	unsigned char ucval;
	unsigned char* doneptr = 0;


	rest = charleng % 2;
	if( rest ){
		_ASSERT( 0 );
		return 1;
	}

	doneptr = donedata;
	for( charno = 0; charno < charleng; charno += 2 ){
		ret = ConvData( orgdata + charno, &ucval, 0 );
		_ASSERT( !ret );

		ret = ConvData( orgdata + charno + 1, &ucval, 1 );
		_ASSERT( !ret );

		*doneptr = ucval;
		doneptr++;
	}

	return 0;
}

int CMotParamDlg::ConvData( char* charptr, unsigned char* ucptr, int pos )
{
	char charval;
	unsigned char convval;

	charval = *charptr;
	if( (charval >= '0') && (charval <= '9') ){
		convval = charval - '0';			
	}else if( (charval >= 'A') && (charval <= 'F') ){
		convval = charval - 'A' + 10;
	}else{
		_ASSERT( 0 );
		return 1;
	}

	if( pos == 0 ){
		*ucptr = convval << 4;
	}else{
		*ucptr |= convval;
	}

	return 0;
}

int CMotParamDlg::UpdateRadioDlg()
{

	if( m_ikmode == IKMODE_JOINT ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_MODEJOINT, IDC_RADIO_MODEOPE, IDC_RADIO_MODEJOINT);
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_MODEJOINT, IDC_RADIO_MODEOPE, IDC_RADIO_MODEOPE );
	}

	m_combo_iklevel_wnd.UpdateWindow();


	return 0;
}
int CMotParamDlg::UpdateComboDlg()
{

	if( m_ikmode == IKMODE_JOINT ){
		if( m_iklevel <= 15 )
			m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, (m_iklevel - 1), 0 );
		else
			m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, 15, 0 );
	}else{
		if( m_iklevel <= 15 )
			m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, (m_iklevelope - 1), 0 );
		else
			m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, 15, 0 );
	}

	m_combo_iklevel_wnd.UpdateWindow();

	return 0;
}

int CMotParamDlg::GetRangeMax( int motid, int* rangemax )
{
	*rangemax = m_sl_mp_rangemax;
	return 0;
}
int CMotParamDlg::GetKeyFrameNo( int motid, int* keyframe, int* framenum )
{
	int ret;

	CMotionCtrl* firstbone = 0;
	ret = m_shandler->GetFirstJoint( &firstbone, m_mhandler, 1 );
	if( ret ){
		DbgOut( "motparamdlg : GetKeyFrameNo : GetFirstJoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !firstbone ){
		DbgOut( "motparamdlg : GetKeyFrameNo : firstbone NULL return\n" );
		*framenum = 0;
		return 0;//!!!!!!!!!!!!
	}

	//CTreeElem2* telem;
	//telem = (*m_thandler)( firstbone->serialno );
	//DbgOut( "motparamdlg : GetKeyFrameNo : firstbone %s\r\n", telem->name );


//DbgOut( "motparamdlg : GetKeyFrameNo : motid %d, rangemax %d\r\n", motid, m_sl_mp_rangemax );

	ret = m_mhandler->HuGetKeyframeNo( motid, firstbone->serialno, keyframe, m_sl_mp_rangemax + 1, framenum );
	if( ret ){
		DbgOut( "motparamdlg : GetKeyFrameNo : mh GetKeyFrameNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CMotParamDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

//	CMotionCtrl* mcptr = (*m_mhandler)(m_selectedno);
//	if( !mcptr )
//		return 0;

	KEYFRAMENO* key;
	key = (KEYFRAMENO*)malloc( sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );
	if( !key ){
		DbgOut( "motparamdlg : OnBeforeMP : key alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( key, sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );

	int ret;
	int keynum = 0;
	ret = m_mhandler->GetKeyframeNo( m_mot_cookie, m_selectedno, key, m_sl_mp_rangemax + 2, &keynum );
	if( ret ){
		DbgOut( "motparamdlg : OnBeforeMP : mh GetKeyframeNo error !!!\n" );
		_ASSERT( 0 );
		free( key );
		return 1;
	}


	int keyno;
	int befkeyno = -1;
	for( keyno = 0; keyno < keynum; keyno++ ){
		int curframe;
		curframe = ( key + keyno )->frameno;
		if( curframe < m_current ){
			befkeyno = keyno;
		}else{
			break;
		}
	}

	if( befkeyno >= 0 ){
		m_current = ( key + befkeyno )->frameno;
	}else{
		m_current = 0;
	}

	if( m_current < m_startframe ){
		m_current = m_startframe;
	}

	free( key );

///////

	//m_currentがm_startframeより小さい場合でも、RANGEMINの設定によりスライダーは最小値より小さくならない。
	//OnReleasecaptureSl_mpで、スライダーの現在の場所からm_currentを設定しなおすので問題ない。

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	BOOL dummy;	
	OnReleasedcaptureSl_mp( 0, 0, dummy );


	return 0;
}
LRESULT CMotParamDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	KEYFRAMENO* key;
	key = (KEYFRAMENO*)malloc( sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );
	if( !key ){
		DbgOut( "motparamdlg : OnNextMP : key alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( key, sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );

	int ret;
	int keynum = 0;
	ret = m_mhandler->GetKeyframeNo( m_mot_cookie, m_selectedno, key, m_sl_mp_rangemax + 2, &keynum );
	if( ret ){
		DbgOut( "motparamdlg : OnNextMP : mh GetKeyframeNo error !!!\n" );
		_ASSERT( 0 );
		free( key );
		return 1;
	}

	int keyno;
	int findkeyno = -1;
	for( keyno = 0; keyno < keynum; keyno++ ){
		int curframe;
		curframe = ( key + keyno )->frameno;

		if( curframe > m_current ){
			if( (curframe >= m_startframe) && (curframe <= m_endframe) ){
				findkeyno = keyno;
				break;
			}
		}
	}

	if( findkeyno >= 0 ){
		m_current = ( key + findkeyno )->frameno;
	}else{
		m_current = m_endframe;
	}

	if( m_current > m_endframe ){
		m_current = m_endframe;
	}

	free( key );

///////////
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
	BOOL dummy;	
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	return 0;
}

/***
int CMotParamDlg::GetIKTopParent( int srcseri, int maxlevel, int* topparptr )
{
	CShdElem* curselem;
	CShdElem* befselem = 0;
	CShdElem* parselem;
	curselem = (*m_shandler)( srcseri );


	int cnt = 1;
	while( curselem && (cnt <= maxlevel ) ){

		parselem = m_shandler->FindUpperJoint( curselem, 0 );

		if( (curselem->m_ikskip == 0) && parselem ){
			befselem = parselem;
		}

		if( curselem->m_notransik ){
			break;
		}

		curselem = parselem;

		cnt++;
	}

	if( befselem ){
		*topparptr = befselem->serialno;
	}else{
		*topparptr = -1;
	}

	return 0;
}
***/

int CMotParamDlg::GetIKTopParent( int srcseri, int maxlevel, int* topparptr )
{
	CShdElem* curselem;
	CShdElem* befselem = 0;

	curselem = (*m_shandler)( srcseri );


	int cnt = 0;
	while( curselem && (cnt <= maxlevel ) ){
		if( curselem->m_ikskip == 0 ){
			befselem = curselem;
		}

		if( curselem->m_notransik ){
			break;
		}

		curselem = m_shandler->FindUpperJoint( curselem, 0 );
		
		cnt++;
	}

	if( befselem ){
		*topparptr = befselem->serialno;
	}else{
		*topparptr = -1;
	}

	return 0;
}


LRESULT CMotParamDlg::OnCreateKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	CMotionCtrl* firstjoint = 0;
	ret = m_shandler->GetFirstJoint( &firstjoint, m_mhandler, 1 );
	if( ret ){
		DbgOut( "motparamdlg : OnCreateKey : GetFirstJoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !firstjoint ){
		MessageBox( "ボーンが１つもないので、キーフレームを作れません。", "失敗", MB_OK );
		return 0;
	}


	if( m_selectedno > 0 ){
		ret = CreateMotionPoints( m_selectedno, m_mot_cookie, m_current );	
		if( ret ){
			DbgOut( "motparamdlg : OnCreateKey : CreateMotionPoints error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}

//	SetCurrentMotion();
	UpdateMatrixSelected( m_selectedno, 0, 1 );


	Redraw();

	return 0;
}

int CMotParamDlg::BakeMotion()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	CMotionCtrl* firstjoint = 0;
	ret = m_shandler->GetFirstJoint( &firstjoint, m_mhandler, 1 );
	if( ret ){
		DbgOut( "motparamdlg : OnCreateKeyAll : GetFirstJoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !firstjoint ){
		MessageBox( "ボーンが１つもないので、キーフレームを作れません。", "失敗", MB_OK );
		return 0;
	}

	int fno;
	for( fno = 0; fno <= m_sl_mp_rangemax; fno++ ){
		ret = CreateMotionPoints( -1, m_mot_cookie, fno );	
		if( ret ){
			DbgOut( "motparamdlg : OnCreateKeyAll : CreateMotionPoints error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}

//	SetCurrentMotion();
	UpdateAllMatrix( 1 );

	Redraw();

	return 0;
}

LRESULT CMotParamDlg::OnCreateKeyAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	CMotionCtrl* firstjoint = 0;
	ret = m_shandler->GetFirstJoint( &firstjoint, m_mhandler, 1 );
	if( ret ){
		DbgOut( "motparamdlg : OnCreateKeyAll : GetFirstJoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !firstjoint ){
		MessageBox( "ボーンが１つもないので、キーフレームを作れません。", "失敗", MB_OK );
		return 0;
	}

	if( m_selectedno > 0 ){
		ret = CreateMotionPoints( -1, m_mot_cookie, m_current );	
		if( ret ){
			DbgOut( "motparamdlg : OnCreateKeyAll : CreateMotionPoints error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}

//	SetCurrentMotion();
	UpdateAllMatrix( 1 );

	Redraw();

	return 0;
}


LRESULT CMotParamDlg::OnFrameShift(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

	CFrameShiftDlg dlg( 0, m_sl_mp_rangemax );
	ret = (int)dlg.DoModal();
	if( (ret != IDOK) || (dlg.m_shift == 0) ){
		return 0;
	}
	if( dlg.m_selstart < 0 )
		dlg.m_selstart = 0;
	if( dlg.m_selstart > m_sl_mp_rangemax )
		dlg.m_selstart = m_sl_mp_rangemax;

	if( dlg.m_selend < 0 )
		dlg.m_selend = 0;
	if( dlg.m_selend > m_sl_mp_rangemax )
		dlg.m_selend = m_sl_mp_rangemax;



// シフト可能かどうか、チェックする。

	int* framearray;
	int* shiftflag;// キーフレームの無いところは-1, 移動しないキーフレームは０，移動するキーフレームは１
	int arrayleng;
	int keyframenum = 0;

	arrayleng = m_sl_mp_rangemax + 1;
	framearray = (int*)malloc( sizeof( int ) * arrayleng );
	if( !framearray ){
		DbgOut( "motparamdlg : OnFrameShift : framearray alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	shiftflag = (int*)malloc( sizeof( int ) * arrayleng );
	if( !shiftflag ){
		DbgOut( "motparamdlg : OnFrameShift : shiftflag alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

// shift

	if( dlg.m_shift >= 0 ){
		//大きいフレーム番号から移動しないと、重なる。

		CMotionPoint2 tempmp;
		CMotionPoint2* isexist2;
		CMotionPoint2* isexist3;
		CMotionPoint2* retmp;		
		CMotionCtrl* mcptr;
		int elemno;

		QUASPPARAM savespp;

		for( elemno = 0; elemno < m_mhandler->s2mot_leng; elemno++ ){
			mcptr = (*m_mhandler)( elemno );
			if( mcptr->IsJoint() && ((dlg.m_allbone == 1) || (elemno == m_selectedno)) ){

				int frameno;
				for( frameno = 0; frameno < arrayleng; frameno++ ){
					*( framearray + frameno ) = 0;
					*( shiftflag + frameno ) = -1;//!!!!!!!!!!!!!!!!!!!
				}
				ret = m_mhandler->HuGetKeyframeNo( m_mot_cookie, elemno, framearray, arrayleng, &keyframenum );
				if( ret ){
					DbgOut( "motparamdlg : OnFrameShift : HuGetKeyframeNo error !!!\n" );
					_ASSERT( 0 );
					ret = 1;
					goto frameshiftend;
				}
				if( keyframenum == 0 ){
					continue;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}

				int keyno;
				int curkeyframe;
				for( keyno = 0; keyno < keyframenum; keyno++ ){
					curkeyframe = *( framearray + keyno );
					
					if( (curkeyframe >= dlg.m_selstart) && (curkeyframe <= dlg.m_selend) ){
						*( shiftflag + curkeyframe ) = 1;
					}else{
						*( shiftflag + curkeyframe ) = 0;
					}
				}

				int newkeyframe;
				for( keyno = keyframenum - 1; keyno >= 0; keyno-- ){
					curkeyframe = *( framearray + keyno );
					if( *( shiftflag + curkeyframe ) == 1 ){
						newkeyframe = curkeyframe + dlg.m_shift;


						isexist2 = mcptr->IsExistMotionPoint( m_mot_cookie, curkeyframe );
						if( isexist2 ){
							tempmp = *isexist2;
							tempmp.m_spp = 0;
							ZeroMemory( &savespp, sizeof( QUASPPARAM ) );
							savespp.m_rotparam = isexist2->m_spp->m_rotparam;
							savespp.m_mvparam = isexist2->m_spp->m_mvparam;
							savespp.m_scparam = isexist2->m_spp->m_scparam;


							ret = mcptr->DeleteMotionPoint( m_mot_cookie, curkeyframe );
							if( ret ){
								DbgOut( "motparamdlg : OnFrameShift : DeleteMotionPoint error !!!\n" );
								_ASSERT( 0 );
								ret = 1;
								goto frameshiftend;
							}

							if( (newkeyframe >= 0) && (newkeyframe <= m_sl_mp_rangemax) ){

								isexist3 = mcptr->IsExistMotionPoint( m_mot_cookie, newkeyframe );
								if( isexist3 ){
									ret = mcptr->DeleteMotionPoint( m_mot_cookie, newkeyframe );
									if( ret ){
										DbgOut( "motparamdlg : OnFrameShift : DeleteMotionPoint 3 error !!!\n" );
										_ASSERT( 0 );
										ret = 1;
										goto frameshiftend;
									}
								}
								
								retmp = mcptr->AddMotionPoint( m_mot_cookie, newkeyframe,
									tempmp.m_q,
									//tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, tempmp.dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
									tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, m_dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
								if( !retmp ){
									DbgOut( "motparamdlg : OnFrameShift : AddMotionPoint error !!!\n" );
									_ASSERT( 0 );
									ret = 1;
									goto frameshiftend;
								}
								retmp->m_eul = tempmp.m_eul;//!!!!!!!!!!!

								retmp->m_spp->m_rotparam = savespp.m_rotparam;
								retmp->m_spp->m_mvparam = savespp.m_mvparam;
								retmp->m_spp->m_scparam = savespp.m_scparam;

							}
						}
					}
				}
			}
		}

	}else{
		//小さいフレーム番号から移動しないと、重なる。
		CMotionPoint2 tempmp;
		CMotionPoint2* isexist2;
		CMotionPoint2* isexist3;
		CMotionPoint2* retmp;		
		CMotionCtrl* mcptr;
		int elemno;
		QUASPPARAM savespp;

		for( elemno = 0; elemno < m_mhandler->s2mot_leng; elemno++ ){
			mcptr = (*m_mhandler)( elemno );
			if( mcptr->IsJoint() && ((dlg.m_allbone == 1) || (elemno == m_selectedno)) ){

				int frameno;
				for( frameno = 0; frameno < arrayleng; frameno++ ){
					*( framearray + frameno ) = 0;
					*( shiftflag + frameno ) = -1;//!!!!!!!!!!!!!!!!!!!
				}
				ret = m_mhandler->HuGetKeyframeNo( m_mot_cookie, elemno, framearray, arrayleng, &keyframenum );
				if( ret ){
					DbgOut( "motparamdlg : OnFrameShift : HuGetKeyframeNo error !!!\n" );
					_ASSERT( 0 );
					ret = 1;
					goto frameshiftend;
				}
				if( keyframenum == 0 ){
					continue;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}

				int keyno;
				int curkeyframe;
				for( keyno = 0; keyno < keyframenum; keyno++ ){
					curkeyframe = *( framearray + keyno );
					
					if( (curkeyframe >= dlg.m_selstart) && (curkeyframe <= dlg.m_selend) ){
						*( shiftflag + curkeyframe ) = 1;
					}else{
						*( shiftflag + curkeyframe ) = 0;
					}
				}

				int newkeyframe;
				for( keyno = 0; keyno < keyframenum; keyno++ ){
					curkeyframe = *( framearray + keyno );
					if( *( shiftflag + curkeyframe ) == 1 ){
						newkeyframe = curkeyframe + dlg.m_shift;


						isexist2 = mcptr->IsExistMotionPoint( m_mot_cookie, curkeyframe );
						if( isexist2 ){
							tempmp = *isexist2;
							tempmp.m_spp = 0;
							tempmp.m_spp = 0;
							ZeroMemory( &savespp, sizeof( QUASPPARAM ) );
							savespp.m_rotparam = isexist2->m_spp->m_rotparam;
							savespp.m_mvparam = isexist2->m_spp->m_mvparam;
							savespp.m_scparam = isexist2->m_spp->m_scparam;


							ret = mcptr->DeleteMotionPoint( m_mot_cookie, curkeyframe );
							if( ret ){
								DbgOut( "motparamdlg : OnFrameShift : DeleteMotionPoint error !!!\n" );
								_ASSERT( 0 );
								ret = 1;
								goto frameshiftend;
							}

							if( (newkeyframe >= 0) && (newkeyframe <= m_sl_mp_rangemax) ){

								isexist3 = mcptr->IsExistMotionPoint( m_mot_cookie, newkeyframe );
								if( isexist3 ){
									ret = mcptr->DeleteMotionPoint( m_mot_cookie, newkeyframe );
									if( ret ){
										DbgOut( "motparamdlg : OnFrameShift : DeleteMotionPoint 3 error !!!\n" );
										_ASSERT( 0 );
										ret = 1;
										goto frameshiftend;
									}
								}
								
								retmp = mcptr->AddMotionPoint( m_mot_cookie, newkeyframe,
									tempmp.m_q,
									//tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, tempmp.dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
									tempmp.m_mvx, tempmp.m_mvy, tempmp.m_mvz, m_dispswitch, tempmp.interp, tempmp.m_scalex, tempmp.m_scaley, tempmp.m_scalez, tempmp.m_userint1 );
								if( !retmp ){
									DbgOut( "motparamdlg : OnFrameShift : AddMotionPoint error !!!\n" );
									_ASSERT( 0 );
									ret = 1;
									goto frameshiftend;
								}
								retmp->m_eul = tempmp.m_eul;//!!!!!!!!!!!

								retmp->m_spp->m_rotparam = savespp.m_rotparam;
								retmp->m_spp->m_mvparam = savespp.m_mvparam;
								retmp->m_spp->m_scparam = savespp.m_scparam;

							}
						}
					}
				}
			}
		}
	}



	goto frameshiftend;

frameshiftend:
////// free
	free( framearray );
	free( shiftflag );

	////// update

	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}


	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : ExpandAndShiftMotion : MakeRawmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : ExpandAndShiftMotion : RawmatToMotionmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : ExpandAndShiftMotion : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	::SendMessage( m_apphwnd, WM_USER_CALCEUL, 0, 0 );

	BOOL dummy;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );
	OnReleasedcaptureSl_mp( 0, 0, dummy );


	return ret;
}

/***
int CMotParamDlg::LookAtQ( CQuaternion2* dstqptr, D3DXVECTOR3 tarvec, D3DXVECTOR3 basevec, int divnum, int upflag )
{
	int ret;
	
	D3DXVECTOR3 newtarget;
	int newdivnum;

	float savetwist = dstqptr->twist;

	int saultflag = 0;

	if( upflag == 2 ){

		D3DXVECTOR3 toppos;
		if( tarvec.y > 0.0f ){
			toppos.x = 0.0f;
			toppos.y = 1.0f;
			toppos.z = 0.0f;
		}else if( tarvec.y < 0.0f ){
			toppos.x = 0.0f;
			toppos.y = -1.0f;
			toppos.z = 0.0f;
		}else{
			toppos.x = 0.0f;//!!!!!!!!!!!! 2004/4/16
			toppos.y = 1.0f;
			toppos.z = 0.0f;
		}


		//saultチェック
		ret = dstqptr->CheckSault( basevec, tarvec, upflag, &saultflag );
		if( ret ){
			DbgOut( "motparamdlg :  LookAtQ : CheckSault error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		// tarvec Ｙシフト
		int shiftflag = 0;
		D3DXVECTOR3 shifttarvec;
		ret = dstqptr->ShiftVecNearY( basevec, tarvec, &shiftflag, &shifttarvec );
		if( ret ){
			DbgOut( "motparamdlg :  LookAtQ : ShiftVecNearY error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( shiftflag == 1 )
			tarvec = shifttarvec;


		// toppos Yシフト
		int shiftflag2 = 0;
		D3DXVECTOR3 shifttarvec2;
		ret = dstqptr->ShiftVecNearY( basevec, toppos, &shiftflag2, &shifttarvec2 );
		if( ret ){
			DbgOut( "motparamdlg :  LookAtQ : ShiftVecNearY  2 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( shiftflag2 == 1 )
			toppos = shifttarvec2;

//DbgOut( "motparamdlg :  E3DLookAtQ : saultflag %d, shift tarvec %f %f %f, toppos %f %f %f\n",
//	   saultflag, tarvec.x, tarvec.y, tarvec.z, toppos.x, toppos.y, toppos.z );


		if( (saultflag == 0) || (saultflag == 2) ){

			// 移動角度が大きい場合は、targetを近くに設定し直す。分割数も、設定し直す。

			// 直接、tarvecを目指して、回転する。

			
			
			ret = dstqptr->GetDividedTarget( basevec, tarvec, tarvec, divnum, &newtarget, &newdivnum );
			if( ret ){
				DbgOut( "motparamdlg :  LookAtQ : GetDividedTarget error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			tarvec = newtarget;
			divnum = newdivnum;
			
			if( tarvec == toppos ){
				_ASSERT( 0 );
			}


//DbgOut( "motparamdlg :  E3DLookAtQ : saultflag %d, tarvec %f %f %f, divnum %d\n",
//	   saultflag, tarvec.x, tarvec.y, tarvec.z, divnum );


		}else if( saultflag == 1 ){
			
				
			// 移動角度が大きい場合は、targetを近くに設定し直す。分割数も、設定し直す。

			// まず、topposをめざして、回転する。

			ret = dstqptr->GetDividedTarget( basevec, toppos, tarvec, divnum, &newtarget, &newdivnum );
			if( ret ){
				DbgOut( "motparamdlg :  LookAtQ : GetDividedTarget error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			tarvec = newtarget;
			divnum = newdivnum;

			if( tarvec == toppos ){
				_ASSERT( 0 );
			}
			
		}
		
	}
		
	//DbgOut( "motparamdlg :  LookAtQ : saultflag %d\n", saultflag );
	//DbgOut( "motparamdlg :  LookAtQ : 0 : divnum %d\n", divnum );

	// targetへの回転処理
	CQuaternion2 finalq;
	CQuaternion2 rot1, rot2;
	D3DXVECTOR3 upvec;


	CQuaternion2 saveq;
	saveq = *dstqptr;


	finalq = *dstqptr;

	ret = finalq.LookAt( tarvec, basevec, 10000.0f, upflag, 0, 0, &upvec, &rot1, &rot2 );
	if( ret ){
		DbgOut( "motparamdlg :  LookAtQ : fianlq LookAt error !!!\n" );
	}

	//
	//if( *dstqptr != finalq ){
	//	ret = finalq.GradUp( *dstqptr, basevec, upvec, rot1, rot2 );
	//	if( ret ){
	//		DbgOut( "motparamdlg :  E3DLookAtQ : GradUp error !!!\n" );
	//		_ASSERT( 0 );
	//		return 1;
	//	}
	//}
	



	CQuaternion2 firstq;
	firstq = *dstqptr;

	if( divnum >= 1 )
		*dstqptr = firstq.Slerp( finalq, divnum, 1 );
	else
		*dstqptr = finalq;



	{
		D3DXVECTOR3 finalvec;
		dstqptr->Rotate( &finalvec, basevec );
		DXVec3Normalize( &finalvec, &finalvec );

		D3DXVECTOR3 trabase;
		saveq.Rotate( &trabase, basevec );
		DXVec3Normalize( &trabase, &trabase );

		//if( finalvec == trabase ){
		//	DbgOut( "motparamdlg :  E3DLookAtQ : not move warning %f %f %f!!!\n", finalvec.x, finalvec.y, finalvec.z );
		//}
		
		//DbgOut( "motparamdlg :  E3DLookAtQ : saultflag %d, tarvec %f %f %f, newtarget %f %f %f\n", saultflag, tarvec.x, tarvec.y, tarvec.z, newtarget.x, newtarget.y, newtarget.z );
	}


	dstqptr->twist = savetwist;


	//DbgOut( "motparamdlg :  E3DLookAtQ : upflag %d, divnum %d\n", upflag, divnum );


	//*dstqptr = finalq;

	//CQuaternion2 firstq;
	//firstq = *dstqptr;
	//*dstqptr = firstq.Slerp( finalq, divnum, 1 );

	dstqptr->twist = savetwist;

	return 0;
}
int CMotParamDlg::DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec )
{
	float mag;
	mag = srcvec->x * srcvec->x + srcvec->y * srcvec->y + srcvec->z * srcvec->z;

	float sqmag;
	sqmag = (float)sqrt( mag );

	if( sqmag != 0.0f ){
		float magdiv;
		magdiv = 1.0f / sqmag;
//		dstvec->x *= magdiv;
//		dstvec->y *= magdiv;
//		dstvec->z *= magdiv;
		dstvec->x = srcvec->x * magdiv;
		dstvec->y = srcvec->y * magdiv;
		dstvec->z = srcvec->z * magdiv;

	}else{
		//DbgOut( "viewer : DXVec3Normalize : zero warning !!!\n" );
		dstvec->x = 0.0f;
		dstvec->y = 0.0f;
		dstvec->z = 0.0f;
		_ASSERT( 0 );
	}

	return 0;
}
***/
int CMotParamDlg::PlayAnim( int srcframe, int totalflag )
{

	int frameno = srcframe;
	if( totalflag == 0 ){
		frameno = max( m_startframe, frameno );
		frameno = min( m_endframe, frameno );
	}else{
		frameno = max( 0, frameno );
		frameno = min( m_sl_mp_rangemax, frameno );
	}
	InterlockedExchange( &m_current, frameno );
	InterlockedExchange( &s_frameno, frameno );


//m_current
	char tempchar[1024];
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "frameno: current %d, start %d, end %d, total %d", m_current, m_startframe, m_endframe, m_sl_mp_rangemax + 1 );
	m_tex_current_wnd.SetWindowText( tempchar );

//m_sl_mp_
	if( m_sl_mp_wnd.IsWindow() ){
		if( !m_sl_mp_wnd.IsWindow() )
			return 0;
		m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );
		if( !m_sl_mp_wnd.IsWindow() )
			return 0;
	}

	SetCurrentMotion();

//	BOOL dummy;
//	OnReleasedcaptureSl_mp( 0, 0, dummy );

	return 0;
}

int CMotParamDlg::SetMotionFrameNo( int motid, int frameno )
{
	int ret;
	ret = SelectCurrentMotion( motid );
	if( ret ){
		DbgOut( "motparamdlg : SetMotionFrameNo : SelectCurrentMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	frameno = max( 0, frameno );
	frameno = min( m_sl_mp_rangemax, frameno );

	if( frameno < m_startframe ){
		m_startframe = frameno;
		m_motid[motid].startframe = frameno;
	}
	if( frameno > m_endframe ){
		m_endframe = frameno;
		m_motid[motid].endframe = frameno;
	}

	InterlockedExchange( &s_startframe, m_startframe );
	InterlockedExchange( &s_frameno, m_startframe );
	InterlockedExchange( &m_current, frameno );

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	return 0;
}



int CMotParamDlg::SelectCurrentMotion( int srcindex )
{

	if( (srcindex < 0) || (srcindex >= m_motionnum) ){
		DbgOut( "motparamdlg : SelectCurrentMotion : srcindex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	int tmpcookie;
	tmpcookie = m_motid[ srcindex ].id;
	if( tmpcookie < 0 ){
		DbgOut( "motparamdlg : SelectCurrentMotion : tmpcookie error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

/////////
	CMotionCtrl* mcptr = (*m_mhandler)( 1 );
	if( !mcptr ){
		_ASSERT( 0 );
		DbgOut( "motparamdlg : SelectCurrentMotion :  mcptr error" );
		return 1;
	}
	CMotionInfo* miptr = mcptr->motinfo;
	if( !miptr ){
		_ASSERT( 0 );
		DbgOut( "motparamdlg : SelectCurrentMotion : miptr error" );
		return 1;
	}

	if( !(miptr->motname) ){
		_ASSERT( 0 );
		DbgOut( "motparamdlg : SelectCurrentMotion : motname error" );
		return 1;
	}
	char* nameptr = *(miptr->motname + tmpcookie);
	strcpy_s( m_motionname, 256, nameptr );
	if( !(miptr->mottype) ){
		_ASSERT( 0 );
		DbgOut( "motparamdlg : SelectCurrentMotion : mottype error" );
		return 1;
	}
	m_motiontype = *(miptr->mottype + tmpcookie);
	if( !(miptr->motnum) ){
		_ASSERT( 0 );
		DbgOut( "motparamdlg : SelectCurrentMotion : motnum error" );
		return 1;
	}
	m_sl_mp_rangemax = *(miptr->motnum + tmpcookie) - 1; //(framenum - 1)
//	m_startframe = 0;
//	m_endframe = m_sl_mp_rangemax;

	m_startframe = m_motid[ srcindex ].startframe;
	m_endframe = m_motid[ srcindex ].endframe;
	m_range = m_motid[ srcindex ].framerange;
//	m_current = m_startframe;


	m_motionjump = *(miptr->motjump + tmpcookie);


	m_mhandler->GetDefInterp( tmpcookie, &m_interpolation );
	if( (m_interpolation < 0) || (m_interpolation >= INTERPOLATION_MAX) ){
		_ASSERT( 0 );
		DbgOut( "motparamdlg : SelectCurrentMotion : interpolation error" );
		return 1;
	}

	//s_startframe = m_startframe;
	//s_frameno = m_startframe;
	//m_current = m_startframe;
	//m_mot_cookie = tmpcookie;//!!!!!!!!!!!!!!
	InterlockedExchange( &s_startframe, m_startframe );
	InterlockedExchange( &s_frameno, m_startframe );
	InterlockedExchange( &m_current, m_startframe );
	InterlockedExchange( &m_mot_cookie, (LONG)tmpcookie );
	InterlockedExchange( &m_previewframe, m_startframe );


	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe );
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );
////////////

	BOOL dummy = 0;
	OnReleasedcaptureSl_range( 0, 0, dummy );
	dummy = 0;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

////////////
	::SendMessage( m_apphwnd, WM_USER_ANIMMENU, srcindex, 0 );


	return 0;

}

int CMotParamDlg::DestroyCurrentMotion()
{
	if( !m_shandler || !m_mhandler ){
		return 0;//!!!!!!!
	}
	if( m_motionnum <= 1 ){
		return 0;
	}

	int delcookie = m_mot_cookie;

	MOTID tempmotid[MAXMOTIONNUM];
	int mindex;
	for( mindex = 0; mindex < MAXMOTIONNUM; mindex++ ){
		tempmotid[mindex].id = -1;
		ZeroMemory( tempmotid[mindex].filename, sizeof( char ) * 256 );
		tempmotid[mindex].forbidnum = 0;
		tempmotid[mindex].forbidid = 0;
		tempmotid[mindex].startframe = 0;
		tempmotid[mindex].endframe = 0;
		tempmotid[mindex].framerange = 0;
	}

	int setno = 0;
	for( mindex = 0; mindex < MAXMOTIONNUM; mindex++ ){
		int curid;
		curid = m_motid[mindex].id;
		if( curid != delcookie ){
			if( curid != -1 ){
				tempmotid[setno].id = setno;
				strcpy_s( tempmotid[setno].filename, 256, m_motid[mindex].filename );
				tempmotid[setno].ev0idle = m_motid[mindex].ev0idle;
				tempmotid[setno].commonid = m_motid[mindex].commonid;
				tempmotid[setno].forbidnum = m_motid[mindex].forbidnum;
				tempmotid[setno].forbidid = m_motid[mindex].forbidid;
				tempmotid[setno].notfu = m_motid[mindex].notfu;
				tempmotid[setno].startframe = m_motid[mindex].startframe;
				tempmotid[setno].endframe = m_motid[mindex].endframe;
				tempmotid[setno].framerange = m_motid[mindex].framerange;
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

	if( setno != (m_motionnum - 1) ){
		_ASSERT( 0 );
		return 1;
	}

	MoveMemory( m_motid, tempmotid, sizeof( MOTID ) * MAXMOTIONNUM );

	m_motionnum -= 1;//!!!!!

///////
	int ret;
	ret = m_mhandler->DestroyMotionObj( delcookie );
	if( ret ){
		DbgOut( "motparamdlg : DestroyCurrentMotion : mh DestroyMotionObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_mhandler->m_fuid -= 1;//!!!!!!!
	_ASSERT( m_mhandler->m_fuid >= 1 );

///////
	ret = SelectCurrentMotion( 0 );
	if( ret ){
		DbgOut( "motparamdlg : DestroyCurrentMotion : SelectCurrentMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}



	return 0;
}

int CMotParamDlg::SetCurrentMotionPoint( int boneno, CMotionPoint2* srcmp, int hasmpflag )
{
	int ret;

	if( hasmpflag != 1 ){

		ret = CreateMotionPoints( boneno, m_mot_cookie, m_current );
		if( ret ){
			DbgOut( "motparamdlg : SetCurrentMotionPoint : CreateMotionPoints error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	CMotionCtrl* mcptr;
	mcptr = (*m_mhandler)( boneno );
	_ASSERT( mcptr );

	CMotionPoint2* curmp;

	curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( !curmp ){
		DbgOut( "motparamdlg : SetCurrentMotionPoint : IsExistMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	curmp->m_q = srcmp->m_q;
	curmp->m_eul = srcmp->m_eul;

	curmp->m_mvx = srcmp->m_mvx;
	curmp->m_mvy = srcmp->m_mvy;
	curmp->m_mvz = srcmp->m_mvz;
	curmp->m_scalex = srcmp->m_scalex;
	curmp->m_scaley = srcmp->m_scaley;
	curmp->m_scalez = srcmp->m_scalez;

	UpdateMatrixSelected( boneno, 0, 0 );

	Redraw();

	return 0;
}

int CMotParamDlg::SetMPbyEul( int boneno, CMotionPoint2* mpptr, D3DXVECTOR3 neweul )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;

	CQuaternion aftq = mpptr->m_q;
	D3DXVECTOR3 afteul = mpptr->m_eul;
	ret = g_bonepropdlg->GetLimitedQ( boneno, mpptr->m_eul, neweul, &aftq, &afteul );
	if( ret ){
		DbgOut( "motparamdlg : SetMPbyEul : bonepropdlg GetLimitedQ error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	mpptr->m_q = aftq;
	mpptr->m_eul = afteul;

	UpdateMatrixSelected( boneno, 0, 0 );

	Redraw();

	return 0;
}



void CMotParamDlg::FindFirstJoint( CShdElem* curselem, CShdElem** findelem )
{
	if( curselem->IsJoint() ){
		CPart* part;
		part = curselem->part;
		if( part && (part->bonenum >= 1) && !(*findelem) ){
			*findelem = curselem;
		}
	}

	if( !(*findelem) ){
		if( curselem->brother ){
			FindFirstJoint( curselem->brother, findelem );
		}
	}
	if( !(*findelem) ){
		if( curselem->child ){
			FindFirstJoint( curselem->child, findelem );
		}
	}
}

int CMotParamDlg::GetRotRad()
{

	return m_rotrad;
}


LRESULT CMotParamDlg::OnNewRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_rotkind = ROTKIND_NEW;
	CheckRotKindButton();

	return 0;
}
LRESULT CMotParamDlg::OnOldRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_rotkind = ROTKIND_OLD;
	CheckRotKindButton();
	return 0;
}

LRESULT CMotParamDlg::OnOpTarget(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_optardlg ){
		_ASSERT( 0 );
		return 0;
	}


	m_optardlg->ShowDlg( SW_SHOW, m_current );

	return 0;
}


LRESULT CMotParamDlg::OnSoutai(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_soutai = OP_SOUTAI;
	CheckSoutaiButton();

	return 0;
}
LRESULT CMotParamDlg::OnZettai(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_soutai = OP_ZETTAI;
	CheckSoutaiButton();
	return 0;
}

int CMotParamDlg::CheckSoutaiButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( !IsWindow() ){
		return 0;
	}

	switch( m_soutai ){
	case OP_SOUTAI:
		if( (g_endapp == 1) ||!m_soutai_wnd.IsWindow() )
			return 0;
		SendMessage( m_soutai_wnd, BM_SETSTATE, TRUE, 0 );
		if( (g_endapp == 1) ||!m_zettai_wnd.IsWindow() )
			return 0;
		SendMessage( m_zettai_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case OP_ZETTAI:
		if( (g_endapp == 1) ||!m_soutai_wnd.IsWindow() )
			return 0;
		SendMessage( m_soutai_wnd, BM_SETSTATE, FALSE, 0 );
		if( (g_endapp == 1) ||!m_zettai_wnd.IsWindow() )
			return 0;
		SendMessage( m_zettai_wnd, BM_SETSTATE, TRUE, 0 );  
		break;
	default:
		_ASSERT( 0 );
		break;
	}
	return 0;
}


int CMotParamDlg::CheckRotKindButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( !IsWindow() ){
		return 0;
	}

	switch( m_rotkind ){
	case ROTKIND_NEW:
		if( (g_endapp == 1) ||!m_newrot_wnd.IsWindow() )
			return 0;
		SendMessage( m_newrot_wnd, BM_SETSTATE, TRUE, 0 );
		if( (g_endapp == 1) ||!m_oldrot_wnd.IsWindow() )
			return 0;
		SendMessage( m_oldrot_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case ROTKIND_OLD:
		if( (g_endapp == 1) ||!m_newrot_wnd.IsWindow() )
			return 0;
		SendMessage( m_newrot_wnd, BM_SETSTATE, FALSE, 0 );  
		if( (g_endapp == 1) ||!m_oldrot_wnd.IsWindow() )
			return 0;
		SendMessage( m_oldrot_wnd, BM_SETSTATE, TRUE, 0 );  
		break;
	default:
		if( (g_endapp == 1) ||!m_newrot_wnd.IsWindow() )
			return 0;
		SendMessage( m_newrot_wnd, BM_SETSTATE, TRUE, 0 );  
		if( (g_endapp == 1) ||!m_oldrot_wnd.IsWindow() )
			return 0;
		SendMessage( m_oldrot_wnd, BM_SETSTATE, FALSE, 0 );  
		m_rotkind = ROTKIND_NEW;
		break;
	}
	return 0;
}

int CMotParamDlg::CheckPreviewButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_preview_flag ){
		SendMessage( m_preview_wnd, BM_SETSTATE, TRUE, 0 );  
	}else{
		SendMessage( m_preview_wnd, BM_SETSTATE, FALSE, 0 );  
	}
	return 0;
}

int CMotParamDlg::SetFillUpMode( int setseri, int setframe, int setfu )
{

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	int ret;
	if( setseri <= 0 ){
		ret = m_mhandler->SetInterpolationAll( m_mot_cookie, setfu );
		if( ret ){
			DbgOut( "mpdlg : mh SetInterPolationAll error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		UpdateAllMatrix( 1 );

	}else{
		CMotionCtrl* mcptr;
		mcptr = (*m_mhandler)( setseri );
		_ASSERT( mcptr );

		if( setframe < 0 ){
			ret = mcptr->SetInterpolationAll( m_mot_cookie, setfu );	
			if( ret ){
				DbgOut( "mpdlg : mc SetInterpolationAll error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

		}else{
			CMotionPoint2* mpptr = 0;
			ret = m_mhandler->IsExistMotionPoint( m_mot_cookie, setseri, setframe, &mpptr );
			_ASSERT( !ret );
			if( mpptr ){
				mpptr->interp = setfu;
			}
		}

		UpdateMatrixSelected( setseri, 0, 1 );

	}

	Redraw();

	return 0;
}

int CMotParamDlg::DeleteInitialKey()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;


	int ret;
	int frameleng = m_sl_mp_rangemax + 1;
	CMotionPoint2** ppdelmp = 0;
	ppdelmp = (CMotionPoint2**)malloc( sizeof( CMotionPoint2* ) * frameleng );
	if( !ppdelmp ){
		_ASSERT( 0 );
		return 1;
	}
	
	int seri;
	CMotionCtrl* mcptr;
	CMotionPoint2* curmp;
	for( seri = 0; seri < m_mhandler->s2mot_leng; seri++ ){
		mcptr = (*m_mhandler)( seri );
		if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
			ZeroMemory( ppdelmp, sizeof( CMotionPoint2*) * frameleng );
			curmp = mcptr->GetMotionPoint( m_mot_cookie );
			int setno = 0;
			while( curmp ){
				if( curmp->IsInitMP() ){
					*( ppdelmp + setno ) = curmp;
					setno++;
				}
				curmp = curmp->next;
			}
			int setnum = setno;
			int delno;
			for( delno = 0; delno < setnum; delno++ ){
				CMotionPoint2* delmp;
				delmp = *( ppdelmp + delno );
				if( delmp ){
					ret = mcptr->DeleteMotionPoint( m_mot_cookie, delmp->m_frameno );
					if( ret ){
						_ASSERT( 0 );
						free( ppdelmp );
						return 1;
					}
				}
			}
		}
	}

	free( ppdelmp );


	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : DeleteInitialKey : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = InitRotAndMv();
	_ASSERT( !ret );
	ret = ParamsToDlg();
	_ASSERT( !ret );

	SetCurrentMotion();

	Redraw();

	return 0;
}
int CMotParamDlg::DeleteSameKey()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;



	int ret;
	int frameleng = m_sl_mp_rangemax + 1;
	CMotionPoint2** ppdelmp = 0;
	ppdelmp = (CMotionPoint2**)malloc( sizeof( CMotionPoint2* ) * frameleng );
	if( !ppdelmp ){
		_ASSERT( 0 );
		return 1;
	}
	
	int seri;
	CMotionCtrl* mcptr;
	CMotionPoint2* curmp;
	CMotionPoint2* befmp;
	CMotionPoint2* nextmp;
	for( seri = 0; seri < m_mhandler->s2mot_leng; seri++ ){
		mcptr = (*m_mhandler)( seri );
		if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
			ZeroMemory( ppdelmp, sizeof( CMotionPoint2*) * frameleng );
			curmp = mcptr->GetMotionPoint( m_mot_cookie );
			befmp = 0;
			if( curmp ){
				befmp = curmp;
				curmp = curmp->next;
			}
	
			int setno = 0;
			while( curmp ){
				if( curmp->IsSameMP( befmp ) ){
					nextmp = curmp->next;
					if( nextmp && curmp->IsSameMP( nextmp ) ){
						*( ppdelmp + setno ) = curmp;
						setno++;
					}
				}
				befmp = curmp;
				curmp = curmp->next;
			}
			int setnum = setno;
			int delno;
			for( delno = 0; delno < setnum; delno++ ){
				CMotionPoint2* delmp;
				delmp = *( ppdelmp + delno );
				if( delmp ){
					ret = mcptr->DeleteMotionPoint( m_mot_cookie, delmp->m_frameno );
					if( ret ){
						_ASSERT( 0 );
						free( ppdelmp );
						return 1;
					}
				}
			}
		}
	}

	free( ppdelmp );

	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	_ASSERT( !ret );
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : DeleteSameKey : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = InitRotAndMv();
	_ASSERT( !ret );
	ret = ParamsToDlg();
	_ASSERT( !ret );

	SetCurrentMotion();

	Redraw();


	return 0;
}

int CMotParamDlg::DestroyFUMotion()
{
	if( !m_mhandler ){
		return 0;
	}
	if( m_mhandler->m_fuid <= 0 ){
		return 0;
	}

	int ret;
	ret = m_mhandler->DestroyMotionObj( m_mhandler->m_fuid );
	if( ret ){
		DbgOut( "motparamdlg : DestroyFUMotion : mh DestroyMotionObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_mhandler->m_fuid = 0;

	return 0;
}
int CMotParamDlg::CreateFUMotion()
{
	char motionname[256];
	ZeroMemory( motionname, 256 );
	strcpy_s( motionname, "for_fill_up" );
	int interpolation = INTERPOLATION_SLERP;//!!!!!!!!
	int motionjump = 0;
	int cookie = m_mhandler->AddMotion( m_d3dapp->m_za_kind, motionname, MOTION_CLAMP, m_mhandler->m_fuleng, interpolation, motionjump );
	if( cookie < 0 ){
		DbgOut( "MotParamDlg : CreateFUMotion : mhandler->AddMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( cookie != m_motionnum ){
		DbgOut( "MotParamDlg : CreateFUMotion : cookie error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	int ret;
	ret = m_mhandler->MakeRawmat( cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( cookie, m_shandler, -1, -1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = m_mhandler->SetMotionStep( cookie, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	m_mhandler->m_fuid = m_motionnum;//!!!!!!!!!!

	return 0;
}

int CMotParamDlg::OnJumpFrame( int srcframe )
{
	if( m_mot_cookie < 0 ){
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


	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );


	m_motid[ m_mot_cookie ].startframe = m_startframe;
	m_motid[ m_mot_cookie ].endframe = m_endframe;

	ParamsToDlg();
	DrawMotionPoints();
	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	return 0;
}

int CMotParamDlg::SetStartAndEnd( int srcstart, int srcend )
{
	if( m_preview_flag == 1 )
		return 0;

	if( m_mot_cookie < 0 ){
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

	m_range = m_endframe - m_startframe;

	if( (m_current < m_startframe) || (m_current > m_endframe) ){
		m_current = m_startframe;
	}

	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, m_startframe );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_current );


	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

//	InterlockedExchange( &m_previewframe, m_startframe );

	m_motid[ m_mot_cookie ].startframe = m_startframe;
	m_motid[ m_mot_cookie ].endframe = m_endframe;
	m_motid[ m_mot_cookie ].framerange = m_range;

	ParamsToDlg();
	DrawMotionPoints();
	BOOL dummy;
	OnReleasedcaptureSl_mp( 999, 0, dummy );

	return 0;
}

int CMotParamDlg::CalcSplineParam( int allmkind, int calcseri, int motcookie )
{
	int ret;
//	if( m_calcsp == 0 ){
//		return 0;
//	}

	int cookie;
	if( motcookie == -1 ){
		cookie = m_mot_cookie;
	}else{
		cookie = motcookie;
	}

	int calcrot, calcmv, calcsc;
	if( allmkind == 1 ){
		calcrot = 1;
		calcmv = 1;
		calcsc = 1;
	}else{
		if( m_d3dapp->m_tbb_t || m_d3dapp->m_tbb_t2 ){
			calcmv = 1;
			calcrot = 0;
			calcsc = 0;
		}else if( m_d3dapp->m_tbb_s ){
			calcsc = 1;
			calcrot = 0;
			calcmv = 0;
		}else{
			calcrot = 1;
			calcmv = 0;
			calcsc = 0;
		}
	}
		
	int changefumode = 0;

	if( calcseri == -1 ){
		int doseri;
		for( doseri = 0; doseri < m_shandler->s2shd_leng; doseri++ ){
			CShdElem* dose;
			dose = (*m_shandler)( doseri );
			_ASSERT( dose );
			if( dose->IsJoint() ){
				if( calcrot == 1 ){
					ret = AutoSplineAll( m_mhandler, cookie, doseri, MKIND_ROT, changefumode );
					_ASSERT( !ret );
				}
				if( calcmv == 1 ){
					ret = AutoSplineAll( m_mhandler, cookie, doseri, MKIND_MV, changefumode );
					_ASSERT( !ret );
				}
				if( calcsc == 1 ){
					ret = AutoSplineAll( m_mhandler, cookie, doseri, MKIND_SC, changefumode );
					_ASSERT( !ret );
				}
			}
		}
		ret = m_mhandler->FillUpMotion( m_shandler, cookie, -1, 0, -1, 0 );
		_ASSERT( !ret );
	}else{
		if( calcrot == 1 ){
			ret = AutoSplineAll( m_mhandler, cookie, calcseri, MKIND_ROT, changefumode );
			_ASSERT( !ret );
		}
		if( calcmv == 1 ){
			ret = AutoSplineAll( m_mhandler, cookie, calcseri, MKIND_MV, changefumode );
			_ASSERT( !ret );
		}
		if( calcsc == 1 ){
			ret = AutoSplineAll( m_mhandler, cookie, calcseri, MKIND_SC, changefumode );
			_ASSERT( !ret );
		}
		ret = m_mhandler->FillUpMotion( m_shandler, cookie, calcseri, 0, -1, 0 );
		_ASSERT( !ret );
	}

	return 0;
}



int CMotParamDlg::SetQOpTarget( int mvboneno, CQuaternion multq, CQuaternion totalq, int skipupdate )
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( !m_optardlg ){
		return 0;
	}
	if( m_optardlg->m_ope == OPTAR_CUR ){
		return 0;
	}

	m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );

	CShdElem* curselem;
	curselem = (*m_shandler)( mvboneno );
	_ASSERT( curselem );
	CShdElem* parselem;
	parselem = m_shandler->FindUpperJoint( curselem );

	CShdElem* savepar = parselem;

	CMotionCtrl* mcptr;
	if( m_mhandler->m_standard == 1 ){
		if( savepar ){
			mcptr = (*m_mhandler)( savepar->serialno );
		}else{
			mcptr = 0;
		}
	}else{
		mcptr = (*m_mhandler)( mvboneno );
	}

	if( !mcptr ){
		return 0;
	}

	int frameno;
	CMotionPoint2* curmp;
	curmp = mcptr->GetMotionPoint( m_mot_cookie );

	while( curmp ){
		frameno = curmp->m_frameno;
		if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){

			if( frameno != m_current ){
			// worldmat
				CQuaternion worldq;
				D3DXMATRIX worldm;
				worldm = m_d3dapp->m_matWorld;
				worldm._41 = 0.0f;
				worldm._42 = 0.0f;
				worldm._43 = 0.0f;
				D3DXQUATERNION wqx;
				D3DXQuaternionRotationMatrix( &wqx, &worldm );
				worldq.x = wqx.x;
				worldq.y = wqx.y;
				worldq.z = wqx.z;
				worldq.w = wqx.w;

				CQuaternion parq( 1.0f, 0.0f, 0.0f, 0.0f );
				CQuaternion curq( 1.0f, 0.0f, 0.0f, 0.0f );
				CQuaternion invcurq( 1.0f, 0.0f, 0.0f, 0.0f );
				CQuaternion multq2;

				//m_q = invparq * multq;
				//m_q = multq * curmp->m_q;//後に掛ける
				//m_q = curmp->m_q * multq;//最初に掛ける		
				if( m_mhandler->m_standard == 1 ){
					if( parselem ){
						parselem = m_shandler->FindUpperJoint( parselem );
					}
				}

				CQuaternion setq;
				D3DXVECTOR3 neweul;
				if( m_soutai == OP_ZETTAI ){
					setq = totalq;
					CShdElem* tmpse = (*m_shandler)( mcptr->serialno );
					if( curmp->prev ){
						ret = qToEuler( tmpse, &curmp->m_q, &neweul );
						_ASSERT( !ret );
						ret = modifyEuler( &neweul, &curmp->prev->m_eul );
						_ASSERT( !ret );
					}else{
						ret = qToEuler( tmpse, &curmp->m_q, &neweul );
						_ASSERT( !ret );
						D3DXVECTOR3 befeul( 0.0f, 0.0f, 0.0f );
						ret = modifyEuler( &neweul, &befeul );
						_ASSERT( !ret );
					}
				}else{
					while( parselem ){
						CMotionCtrl* parmc;
						parmc = (*m_mhandler)( parselem->serialno );
			
						//parmp = parmc->IsExistMotionPoint( m_mot_cookie, m_current );
						//_ASSERT( parmp );
						CMotionPoint2 parmp;
						int hasmpflag = 0;
						ret = parmc->CalcMotionPointOnFrame( parselem, &parmp, m_mot_cookie, frameno, &hasmpflag, m_shandler, m_mhandler );
						if( ret ){
							DbgOut( "motparamdlg : SetMotionPointIK : mc CalcMotionPointOnFrame error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}

						parq = parmp.m_q * parq;
						//parq = parq * parmp->m_q;//!!!!!!
						parq.normalize();
						parselem = m_shandler->FindUpperJoint( parselem );
					}

					curq = worldq * parq * curmp->m_q;

					curq.normalize();
					curq.inv( &invcurq );
					invcurq.normalize();

					D3DXQUATERNION xmultq;
					multq.CQuaternion2X( &xmultq );

					D3DXVECTOR3 axis, multaxis;
					float rad;
					D3DXQuaternionToAxisAngle( &xmultq, &axis, &rad );
					DXVec3Normalize( &axis, &axis );
					invcurq.Rotate( &multaxis, axis );
					DXVec3Normalize( &multaxis, &multaxis );

					multq2.SetAxisAndRot( multaxis, rad );

					CQuaternion aftq;
					aftq = curmp->m_q * multq2;

					int startout = 0;
					ret = g_bonepropdlg->GetLimitedQ( mcptr->serialno, &curmp->m_q, &curmp->m_eul, &aftq, &setq, &startout, &neweul, 0, 0 );
					if( ret ){
						DbgOut( "motparamdlg : SetMotionPointIK : GetLimitedQ error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}

				m_mvx = curmp->m_mvx;
				m_mvy = curmp->m_mvy;
				m_mvz = curmp->m_mvz;
				m_interp = curmp->interp;

				m_scalex = curmp->m_scalex;
				m_scaley = curmp->m_scaley;
				m_scalez = curmp->m_scalez;
				m_userint1 = curmp->m_userint1;
				ret = curmp->SetParams( frameno, setq, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
				_ASSERT( !ret );
				curmp->m_eul = neweul;//!!!!!!!!!!!!!!!!!	
			}
		}
		curmp = curmp->next;
	}

	if( mcptr && (skipupdate == 0 ) ){
		UpdateMatrixSelected( mcptr->serialno, 1, 0 );
	}

	return 0;
}

int CMotParamDlg::GetStandardQ( int boneno, CQuaternion* dstq )
{
	CShdElem* curselem;
	curselem = (*m_shandler)( boneno );
	_ASSERT( curselem );
	CShdElem* parselem;
	parselem = m_shandler->FindUpperJoint( curselem );

	CShdElem* savepar = parselem;

	CMotionCtrl* mcptr;
	if( m_mhandler->m_standard == 1 ){
		if( savepar ){
			mcptr = (*m_mhandler)( savepar->serialno );
		}else{
			mcptr = 0;
			dstq->SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
			return 0;//!!!!!!!!!
		}
	}else{
		mcptr = (*m_mhandler)( boneno );
	}

	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( curmp ){
		*dstq = curmp->m_q;
	}else{
		dstq->SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
	}
	return 0;
}

LRESULT CMotParamDlg::OnApplyStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int tmpstart;
	int ret;
	ret = GetInt( m_rangestart_wnd, &tmpstart );
	if( ret || (tmpstart < 0) ){
		::MessageBoxA( m_hWnd, "開始位置の数値が不正です。正の整数を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_startframe = tmpstart;
	m_sl_range_wnd.SendMessageA( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );

	BOOL dummy = 0;
	OnReleasedcaptureSl_range( 0, 0, dummy );

	return 0;
}
LRESULT CMotParamDlg::OnApplyRange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int tmprange, tmpend;
	int ret;

	ret = GetInt( m_editrange_wnd, &tmprange );
	if( ret || (tmprange < 0) ){
		::MessageBoxA( m_hWnd, "表示幅の数値が不正です。正の整数を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_range = tmprange;

	tmpend = m_startframe + tmprange;
	m_endframe = min( tmpend, m_sl_mp_rangemax );

	m_motid[m_mot_cookie].framerange = m_range;
	m_motid[m_mot_cookie].endframe = m_endframe;


	ParamsToDlg();
	Redraw();

	return 0;
}


LRESULT CMotParamDlg::OnNotCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_NCRE );
	if( checkflag == BST_CHECKED ){
		m_notcreate = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_notcreate = 0;
	}

	ParamsToDlg();

	return 0;
}

int CMotParamDlg::ChangeCurMotTotalFrame( int newleng )
{
	int ret;
// expand
	int saveleng = m_sl_mp_rangemax + 1;//!!!!!!!	
	ret = m_mhandler->ChangeTotalFrame( m_mot_cookie, newleng, (newleng > saveleng) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	m_sl_mp_rangemax = max( 1, newleng - 1 );
	m_startframe = 0;
	m_endframe = min( m_defframerange, m_sl_mp_rangemax );
	m_range = m_defframerange;

	m_motid[ m_mot_cookie ].startframe = m_startframe;
	m_motid[ m_mot_cookie ].endframe = m_endframe;
	m_motid[ m_mot_cookie ].framerange = m_range;

	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, 0 );
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	m_sl_mp_wnd.SendMessage( TBM_CLEARTICS, (WPARAM)TRUE, 0 );
	SetSliderTic( m_sl_mp_wnd, 10, 0, m_sl_mp_rangemax );//

	m_sl_range_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_range_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_sl_mp_rangemax);
	SetSliderTic( m_sl_range_wnd, 10, 0, m_sl_mp_rangemax );//
	m_sl_range_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_startframe );


// update
	ret = m_mhandler->MakeRawmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : ExpandAndShiftMotion : MakeRawmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( m_mot_cookie, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "motparamdlg : ExpandAndShiftMotion : RawmatToMotionmat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_calcsp ){
		ret = CalcSplineParam( 1, -1 );
		if( ret ){
			DbgOut( "motparamdlg : ExpandAndShiftMotion : CalcSplineParam error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	BOOL dummy;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );
	OnReleasedcaptureSl_mp( 0, 0, dummy );

	return 0;
}

int CMotParamDlg::ConvSymMotion()
{
	int ret;
	if( m_mot_cookie < 0 ){
		_ASSERT( 0 );
		return 1;
	}

	KEYFRAMENO* key;
	key = (KEYFRAMENO*)malloc( sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );
	if( !key ){
		DbgOut( "motparamdlg : ConvSymMotion : key alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ZeroMemory( key, sizeof( KEYFRAMENO ) * ( m_sl_mp_rangemax + 2 ) );

	int keynum = 0;
	ret = m_mhandler->GetKeyframeNo( m_mot_cookie, m_selectedno, key, m_sl_mp_rangemax + 2, &keynum );
	if( ret ){
		DbgOut( "motparamdlg : ConvSymMotion : mh GetKeyframeNo error !!!\n" );
		_ASSERT( 0 );
		free( key );
		return 1;
	}

	int keyno;
	for( keyno = 0; keyno < keynum; keyno++ ){
		int frameno, kind;
		frameno = (key + keyno)->frameno;
		kind = (key + keyno)->kind;

		if( (frameno >= 0) && (frameno <= m_sl_mp_rangemax) && ( (kind == 1) || (kind == 2) ) ){
			m_current = frameno;

			BOOL dummy1 = 0;
			ret = OnFrameCopyAri( 0, 0, 0, dummy1 );
			_ASSERT( !ret );

			BOOL dummy2 = 0;
			ret = OnFramePasteSym( 0, 0, 0, dummy2 );
			_ASSERT( !ret );
		}
	}

	free( key );

	BOOL dummy;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );
	OnReleasedcaptureSl_mp( 0, 0, dummy );


	return 0;
}

int CMotParamDlg::HCopy()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( m_hmp ){
		DestroyHMPReq( m_hmp );
		m_hmp = 0;
	}

	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );

	m_hmp = (HMP*)malloc( sizeof( HMP ) );
	CreateHMPReq( selem, m_hmp, 0 );

	return 0;
}

void CMotParamDlg::CreateHMPReq( CShdElem* septr, HMP* hmpptr, HMP* parhmp )
{
	CMotionPoint2 curmp;
	CMotionCtrl* mcptr = (*m_mhandler)( septr->serialno );

	int hasmpflag;
	int ret = mcptr->CalcMotionPointOnFrame( septr, &curmp, m_mot_cookie, m_current, &hasmpflag, m_shandler, m_mhandler );
	if( ret ){
		DbgOut( "motparamdlg : CreateHMPReq : mc CalcMotionPointOnFrame error !!!\n" );
		_ASSERT( 0 );
		return;
	}

	hmpptr->mp = curmp;
	hmpptr->parhmp = parhmp;
	hmpptr->chilhmp = 0;
	hmpptr->brohmp = 0;

	if( septr->child ){
		HMP* newhmp = (HMP*)malloc( sizeof( HMP ) );
		hmpptr->chilhmp = newhmp;
		CreateHMPReq( septr->child, newhmp, hmpptr );
	}
	if( septr->brother ){
		HMP* newhmp = (HMP*)malloc( sizeof( HMP ) );
		hmpptr->brohmp = newhmp;
		CreateHMPReq( septr->brother, newhmp, parhmp );		
	}

}


int CMotParamDlg::HPaste()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );

	if( m_hmp ){
		PasteHMPReq( selem, m_hmp );
	}

/////
	if( m_d3dapp ){
		m_d3dapp->OnPluginPose();
	}
	UpdateAllMatrix( 1 );
	BOOL dummy;
	OnReleasedcaptureSl_mp( 0, 0, dummy );


	return 0;
}

void CMotParamDlg::PasteHMPReq( CShdElem* septr, HMP* hmpptr )
{
	CMotionCtrl* mcptr = (*m_mhandler)( septr->serialno );
	CMotionPoint2* curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
	if( !curmp ){
		int ret = CreateMotionPoints( mcptr->serialno, m_mot_cookie, m_current );
		if( ret )
			return;
		curmp = mcptr->IsExistMotionPoint( m_mot_cookie, m_current );
		if( !curmp )
			return;
	}
	//buffptr->mp.m_frameno = cpframe;
	curmp->m_mvx = hmpptr->mp.m_mvx;
	curmp->m_mvy = hmpptr->mp.m_mvy;
	curmp->m_mvz = hmpptr->mp.m_mvz;
	curmp->m_q = hmpptr->mp.m_q;
	//buffptr->mp.dispswitch = isexist->dispswitch;
	curmp->dispswitch = 0xFFFFFFFF;

	curmp->interp = hmpptr->mp.interp;

	curmp->m_scalex = hmpptr->mp.m_scalex;
	curmp->m_scaley = hmpptr->mp.m_scaley;
	curmp->m_scalez = hmpptr->mp.m_scalez;

	curmp->m_eul = hmpptr->mp.m_eul;
	curmp->m_userint1 = hmpptr->mp.m_userint1;

/////////
	if( septr->child ){
		if( hmpptr->chilhmp ){
			PasteHMPReq( septr->child, hmpptr->chilhmp );
		}else{
			return;
		}
	}

	if( septr->brother ){
		if( hmpptr->brohmp ){
			PasteHMPReq( septr->brother, hmpptr->brohmp );
		}else{
			return;
		}
	}
}

void CMotParamDlg::DestroyHMPReq( HMP* curhmp )
{
	if( curhmp->chilhmp ){
		DestroyHMPReq( curhmp->chilhmp );
	}
	if( curhmp->brohmp ){
		DestroyHMPReq( curhmp->brohmp );
	}

	delete curhmp;
}
