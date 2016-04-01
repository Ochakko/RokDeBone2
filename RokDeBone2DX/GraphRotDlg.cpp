// GraphRotDlg.cpp : CGraphRotDlg のインプリメンテーション
#include "stdafx.h"

#include <coef.h>
#include "GraphRotDlg.h"


#include <math.h>
#include <stdio.h>
#define DBGH
#include <dbg.h>

#include "MotDlg.h"
#include "MotParamDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <mothandler.h>
#include <motionctrl.h>
//#include <motioninfo.h>
//#include <motionpoint2.h>
//#include <quaternion2.h>
#include <quaternion.h>
#include <AutoSpline.h>

#include "CamKeyDlg.h"
#include "GPKeyDlg.h"
#include "SpCtrlDlg.h"
#include "SelBoneDlg.h"
#include "SelRTSDlg.h"

#include <Commdlg.h>

#include <BoneProp.h>

#include "viewer.h"
#include "GetDlgParams.h"
#include "FUMenuMain.h"

enum
{
	DRAGMODE_X,
	DRAGMODE_Y,
	DRAGMODE_Z,
	DRAGMODE_MAX
};


extern CIniData* g_inidata;
extern CBonePropDlg*	g_bonepropdlg;
extern unsigned char g_KeyBuffer[256];
extern CMotDlg* g_motdlg;

#define MP_SIZE	3
#define CTRL_SIZE 2

/////////////////////////////////////////////////////////////////////////////
// CGraphRotDlg

CGraphRotDlg::CGraphRotDlg( CMyD3DApplication* papp, CMotParamDlg* srcmpdlg )
{
	m_papp = papp;
	m_dlg = srcmpdlg;
	m_dlg->m_grotdlg = this;
	m_camdlg = 0;
	m_gpdlg = 0;

	m_black_pen = NULL;
	m_red_pen = NULL;
	m_blue_pen = NULL;
	m_green_pen = NULL;
	m_gray_pen = NULL;
	m_yellow_pen = NULL;
	m_black_brush = NULL;
	m_white_brush = NULL;
	m_red_brush = NULL;
	m_blue_brush = NULL;
	m_green_brush = NULL;
	m_gray_brush = NULL;
	m_yellow_brush = NULL;

	m_timerworking = 0;
	m_cmdshow = SW_HIDE;

	m_startframe = 0;
	m_endframe = 80;

	m_ismoving = 0;
	m_dragflag = 0;

	m_gmp = 0;

	m_rotmin = -30.0f;
	m_rotmax = 30.0f;

	m_tatemin = m_rotmin - 20.0f;
	m_tatemax = m_rotmax + 20.0f;

	m_dragyoko = 0;
	m_yokoval = 0;

	m_dragmode = DRAGMODE_X;
	m_dragvalflag = 0;
	m_dragmp = 0;
	m_dragck = 0;
	m_draggpk = 0;

	m_fumenu = 0;
	m_spdlg = 0;
	m_dispctrl = 0;
}


CGraphRotDlg::~CGraphRotDlg()
{
	if( m_gmp ){
		free( m_gmp );
		m_gmp = 0;
	}

	if( m_fumenu ){
		m_fumenu->Destroy();
		delete m_fumenu;
		m_fumenu = 0;
	}
}

int CGraphRotDlg::DestroySpDlg()
{
	if( m_spdlg ){
		m_spdlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0, 0, 0 );
		m_spdlg->DestroyWindow();
		delete m_spdlg;
		m_spdlg = 0;
	}

	return 0;
}

int CGraphRotDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, GraphRotDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	m_dlg = 0;
	m_camdlg = 0;
	m_gpdlg = 0;

	return 0;
}

LRESULT CGraphRotDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	//InitCommonControls();

	ret = SetWnd();
	_ASSERT( !ret );

	ret = CreatePen();
	_ASSERT( !ret );

	if( !m_gmp ){
		int mpnum;
		mpnum = m_endframe - m_startframe + 1;
		m_gmp = (GMP*)malloc( sizeof( GMP ) * mpnum );
		if( !m_gmp ){
			DbgOut( "grotdlg : OnInitDialog : m_gmp alloc error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ZeroMemory( m_gmp, sizeof( GMP ) * mpnum );
	}

	if( !m_spdlg ){
		m_spdlg = new CSpCtrlDlg();
		if( !m_spdlg ){
			_ASSERT( 0 );
			return 1;
		}
		m_spdlg->Create( m_hWnd );
	}


	if( !m_fumenu ){
		m_fumenu = new CFUMenuMain();
		if( !m_fumenu ){
			DbgOut( "grotdlg : OnInitDialog : m_fumenu alloc error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		m_fumenu->Create( m_dlg->m_apphwnd );
	}

	ret = ParamsToDlg();
	_ASSERT( !ret );

	int timerid;
	timerid = (int)::SetTimer( m_hWnd, GraphRotDlgTIMER, 500, NULL );
	_ASSERT( timerid == GraphRotDlgTIMER );
	m_timerworking = 1;

	return 1;  // システムにフォーカスを設定させます
}

int CGraphRotDlg::CreatePen()
{
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
	if( m_green_brush == NULL ){
		COLORREF greencol = RGB( 0, 255, 0 );
		m_green_brush = ::CreateSolidBrush( greencol );
		_ASSERT( m_green_brush != NULL );
	}
	if( m_gray_brush == NULL ){
		COLORREF graycol = RGB( 190, 190, 190 );
		m_gray_brush = ::CreateSolidBrush( graycol );
		_ASSERT( m_gray_brush != NULL );
	}
	if( m_yellow_brush == NULL ){
		COLORREF yellowcol = RGB( 255, 255, 0 );
		m_yellow_brush = ::CreateSolidBrush( yellowcol );
		_ASSERT( m_yellow_brush != NULL );
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
	if( m_green_pen == NULL ){
		COLORREF greencol = RGB( 0, 255, 0 );
		m_green_pen = ::CreatePen( PS_SOLID, 0, greencol );
		_ASSERT( m_green_pen != NULL );
	}
	if( m_gray_pen == NULL ){
		COLORREF graycol = RGB( 190, 190, 190 );
		m_gray_pen = ::CreatePen( PS_SOLID, 0, graycol );
		_ASSERT( m_gray_pen != NULL );
	}
	if( m_yellow_pen == NULL ){
		COLORREF yellowcol = RGB( 255, 255, 0 );
		m_yellow_pen = ::CreatePen( PS_SOLID, 0, yellowcol );
		_ASSERT( m_yellow_pen != NULL );
	}

	return 0;
}

int CGraphRotDlg::InitDlg()
{
	int ret;
	
	DbgOut( "GraphRotDlg : InitDlg\n" );

	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CGraphRotDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "GraphRotDlg : OnOK\n" );
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
//	m_displayflag = 0;
//	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}

LRESULT CGraphRotDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
//	m_displayflag = 0;
//	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}
LRESULT CGraphRotDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
//	m_displayflag = 0;
//	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}


	
int CGraphRotDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	m_graph_wnd = GetDlgItem( IDC_GRAPH );
	m_tate_wnd = GetDlgItem( IDC_TATE );
	m_yoko_wnd = GetDlgItem( IDC_YOKO );

	m_dragx_wnd = GetDlgItem( IDC_DRAGX );
	m_dragy_wnd = GetDlgItem( IDC_DRAGY );
	m_dragz_wnd = GetDlgItem( IDC_DRAGZ );
	m_dragval_wnd = GetDlgItem( IDC_DRAGVAL );

	return 0;
}

int CGraphRotDlg::ParamsToDlg()
{
	//char tempchar[1024];		

	if( !IsWindow() )
		return 0;

	return 0;
}

int CGraphRotDlg::Tate2Windowpos( RECT clirect, int tate, int* posptr )
{
	float step;
	step = (float)( clirect.bottom - clirect.top ) / ( m_tatemax - m_tatemin );
	*posptr = clirect.bottom - (int)( step * ( (float)tate - m_tatemin ) );
	if( *posptr < clirect.top )
		*posptr = clirect.top;
	if( *posptr > clirect.bottom )
		*posptr = clirect.bottom;
	return 0;
}

int CGraphRotDlg::TateFL2Windowpos( RECT clirect, float tate, int* posptr )
{
	float step;
	step = (float)( clirect.bottom - clirect.top ) / ( m_tatemax - m_tatemin );
	*posptr = clirect.bottom - (int)( step * ( tate - m_tatemin ) );
	if( *posptr < clirect.top )
		*posptr = clirect.top;
	if( *posptr > clirect.bottom )
		*posptr = clirect.bottom;


	return 0;
}


/***
int CGraphRotDlg::Tate2Windowpos( RECT clirect, int tate, int* posptr )
{
	float step;
	step = (float)( clirect.bottom - clirect.top ) / (float)( m_tatemax - m_tatemin );
	*posptr = (int)( step * (float)( tate - m_tatemin ) ) + clirect.top;
	if( *posptr < clirect.top )
		*posptr = clirect.top;
	if( *posptr > clirect.bottom )
		*posptr = clirect.bottom;
	return 0;
}
***/

int CGraphRotDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
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
int CGraphRotDlg::FramenoFL2Windowpos( RECT clirect, float frameno, int* posptr )
{
	float step;
	if( m_startframe != m_endframe ){
		step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_endframe - m_startframe );
		*posptr = MP_SIZE + (int)(step * (float)( frameno - (float)m_startframe ) ) + clirect.left;
	}else{
		*posptr = MP_SIZE + clirect.left;
	}
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	return 0;
}

int CGraphRotDlg::Windowpos2TateFL( RECT clirect, int posy, float* tateptr )
{
	float step = (float)(clirect.bottom - clirect.top) / ( m_tatemax - m_tatemin );

	if( (posy > clirect.top) && (posy < clirect.bottom) ){
		float tempfloat;
		tempfloat = m_tatemin + (float)(clirect.bottom - posy) / step; 
		*tateptr = tempfloat;
	}
	
	//範囲外のときは以前の値を保ったまま
	
	//else if( posy <= clirect.top ){
	//	*tateptr = m_tatemax;
	//}else if( posy >= clirect.bottom ){
	//	*tateptr = m_tatemin;
	//}

	return 0;
}


int CGraphRotDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
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


int CGraphRotDlg::Redraw()
{
	//if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
	//	return 0;

	if( m_timerworking != 1 ){
		return 0;
	}

	if( g_motdlg->m_disptabno == TAB_CAM ){
		if( g_motdlg->GetCameraAnimNum() <= 0 ){
			return 0;
		}
		if( m_camdlg->m_camerabutton == CAMB_HEN )
			return 0;
	}else if( g_motdlg->m_disptabno == TAB_GP ){
		if( g_motdlg->GetGPAnimNum() <= 0 ){
			return 0;
		}
	}else{
		if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
			return 0;
		if( m_dlg->m_selectedno == 0 )
			return 0;
	}


	if( m_cmdshow != SW_SHOW )
		return 0;
	if( !IsWindow() )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	CheckDragModeButton();

	return 0;
}

LRESULT CGraphRotDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	if( m_timerworking != 1 ){
		return 0;
	}
	if( m_papp->m_ExistData == 0 )
		return 0;


	if( !IsWindow() )
		return 0;
	if( m_cmdshow != SW_SHOW )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	if( g_motdlg->m_disptabno == TAB_CAM ){
		DrawYokoAxis();
		CalcMPCam();
		DrawTateAxisCam();
		DrawEulCam();
		DrawDragValCam();
	}else if( g_motdlg->m_disptabno == TAB_GP ){
		DrawYokoAxis();
		CalcMPGP();
		DrawTateAxisGP();
		DrawEulGP();
		DrawDragValGP();
	}else if( g_motdlg->m_disptabno == TAB_MOE ){
		InitPaint();
	}else{
		DrawYokoAxis();
		CalcMP();
		DrawTateAxis();
		DrawEul();
		DrawDragVal();
	}

	return 0;
}

LRESULT CGraphRotDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//	if( m_thandler && m_shandler && m_mhandler && (m_selectedno != 0)){
//		::SendMessage( m_apphwnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)m_selectedno );
//	}
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	Redraw();

	DefWindowProc();
	return 0;
}
LRESULT CGraphRotDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "GraphRotDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CGraphRotDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( g_motdlg->m_disptabno == TAB_CAM ){
		if( g_motdlg->GetCameraAnimNum() <= 0 ){
			return 0;
		}
		if( m_camdlg->m_camerabutton == CAMB_HEN )
			return 0;
	}else if( g_motdlg->m_disptabno == TAB_GP ){
		if( g_motdlg->GetGPAnimNum() <= 0 ){
			return 0;
		}
	}else{
		if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
			return 0;
		if( m_dlg->m_selectedno == 0 )
			return 0;
	}

	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;
	//if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
	//	return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;

	m_ismoving = 0;



	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

//	if( m_apphwnd ){
//		::SendMessage( m_apphwnd, WM_USER_MOVE, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
//	}
	DefWindowProc();

	Redraw();

	return 0;
}





int CGraphRotDlg::OnUserSelchange()
{
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	CloseSpDlg();

	ParamsToDlg();
	Redraw();

	return 0;
}

int CGraphRotDlg::ShiftRange( int srcdiff )
{
	if( m_timerworking != 1 ){
		return 0;
	}
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	if( srcdiff > 0 ){
		m_startframe += srcdiff;
		m_endframe += srcdiff;
	}else{
		int tmpstart;
		tmpstart = m_startframe + srcdiff;
		if( tmpstart < 0 ){
			tmpstart = 0;
		}

		int leng;
		leng = m_endframe - m_startframe;

		m_startframe = tmpstart;
		m_endframe = m_startframe + leng;
	}

	Redraw();
	return 0;
}

LRESULT CGraphRotDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	if( m_timerworking != 1 ){
		return 0;
	}

	if( g_motdlg->m_disptabno == TAB_CAM ){
		if( g_motdlg->GetCameraAnimNum() <= 0 ){
			return 0;
		}
		if( m_camdlg->m_camerabutton == CAMB_HEN )
			return 0;
	}else if( g_motdlg->m_disptabno == TAB_GP ){
		if( g_motdlg->GetGPAnimNum() <= 0 ){
			return 0;
		}
	}else{
		if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
			return 0;
		if( m_dlg->m_selectedno == 0 )
			return 0;
	}

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "GraphRotDlg : OnMouseMove : MK_LBUTTON\n" );

	int ret;

	POINT clickpoint;
	clickpoint.x = LOWORD(lParam);
	clickpoint.y = HIWORD(lParam);
	m_dlg_wnd.ClientToScreen( &clickpoint );//!!!!!!!!!!!!!!!!

	POINT gpoint;
	gpoint = clickpoint;
	m_graph_wnd.ScreenToClient( &gpoint );

	if( m_dragyoko ){

		int dx;
		dx = ( clickpoint.x - m_yokoval ) / 10;

		int changex;
		if( dx > 0 ){
			//changex = 1;
			changex = -1;
		}else if( dx < 0 ){
			//changex = -1;
			changex = 1;
		}else{
			return 0;
		}
		m_yokoval = clickpoint.x;


		int mindx;
		mindx = 0 - m_startframe;

		if( changex > 0 ){
			int shiftx;
			//shiftx = min( changex, maxdx );
			shiftx = changex;
			m_startframe += shiftx;
			m_endframe += shiftx;
		}else if( changex < 0 ){
			int shiftx;
			shiftx = max( changex, mindx );
			m_startframe += shiftx;
			m_endframe += shiftx;
		}else{
			return 0;
		}
		
		Redraw();

	}

	if( m_dragvalflag && m_dragmp ){
		RECT grect;
		m_graph_wnd.GetClientRect( &grect );

		static float newval;
		Windowpos2TateFL( grect, gpoint.y, &newval );

		if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				m_dragmp->m_mvx = newval;
				break;
			case DRAGMODE_Y:
				m_dragmp->m_mvy = newval;
				break;
			case DRAGMODE_Z:
				m_dragmp->m_mvz = newval;
				break;
			default:
				_ASSERT( 0 );
				break;
			}
			
			m_dlg->UpdateMatrixSelected( m_dlg->m_selectedno, 0, 0 );
			m_dlg->Redraw();

		}else if( m_papp->m_tbb_s ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				m_dragmp->m_scalex = newval;
				break;
			case DRAGMODE_Y:
				m_dragmp->m_scaley = newval;
				break;
			case DRAGMODE_Z:
				m_dragmp->m_scalez = newval;
				break;
			default:
				_ASSERT( 0 );
				break;
			}
			
			m_dlg->UpdateMatrixSelected( m_dlg->m_selectedno, 0, 0 );
			m_dlg->Redraw();

		}else{
			D3DXVECTOR3 neweul;
			neweul = m_dragmp->m_eul;
			switch( m_dragmode ){
			case DRAGMODE_X:
				neweul.x = newval;
				break;
			case DRAGMODE_Y:
				neweul.y = newval;
				break;
			case DRAGMODE_Z:
				neweul.z = newval;
				break;
			default:
				_ASSERT( 0 );
				break;
			}

			ret = m_dlg->SetMPbyEul( m_dlg->m_selectedno, m_dragmp, neweul );
			_ASSERT( !ret );
		}


		Redraw();

	}

	if( m_dragvalflag && m_dragck ){
		RECT grect;
		m_graph_wnd.GetClientRect( &grect );

		static float newval;
		Windowpos2TateFL( grect, gpoint.y, &newval );

		if( m_camdlg->m_gtype == G_POS ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				m_dragck->m_celem.pos.x = newval;
				break;
			case DRAGMODE_Y:
				m_dragck->m_celem.pos.y = newval;
				break;
			case DRAGMODE_Z:
				m_dragck->m_celem.pos.z = newval;
				break;
			default:
				_ASSERT( 0 );
				break;
			}
			
			m_camdlg->SetCurrentMotion( 0 );
			m_camdlg->ParamsToDlg();
			m_camdlg->Redraw();
		}else if( m_camdlg->m_gtype == G_TARGET ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				m_dragck->m_celem.target.x = newval;
				break;
			case DRAGMODE_Y:
				m_dragck->m_celem.target.y = newval;
				break;
			case DRAGMODE_Z:
				m_dragck->m_celem.target.z = newval;
				break;
			default:
				_ASSERT( 0 );
				break;
			}
			
			m_camdlg->SetCurrentMotion( 0 );
			m_camdlg->ParamsToDlg();
			m_camdlg->Redraw();

		}else{
			switch( m_dragmode ){
			case DRAGMODE_X:
				m_dragck->m_celem.nearz = newval;
				break;
			case DRAGMODE_Y:
				m_dragck->m_celem.farz = newval;
				break;
			case DRAGMODE_Z:
				if( m_camdlg->m_ortho == 0 ){
					m_dragck->m_celem.fov = newval;
				}else{
					m_dragck->m_celem.orthosize = newval;
				}
				break;
			default:
				_ASSERT( 0 );
				break;
			}
			
			m_camdlg->SetCurrentMotion( 0 );
			m_camdlg->ParamsToDlg();
			m_camdlg->Redraw();
		}


		Redraw();

	}

	if( m_dragvalflag && m_draggpk ){
		RECT grect;
		m_graph_wnd.GetClientRect( &grect );

		static float newval;
		Windowpos2TateFL( grect, gpoint.y, &newval );

		if( m_gpdlg->m_gtype == GPG_POS ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				m_draggpk->m_gpe.pos.x = newval;
				break;
			case DRAGMODE_Y:
				m_draggpk->m_gpe.pos.y = newval;
				break;
			case DRAGMODE_Z:
				m_draggpk->m_gpe.pos.z = newval;
				break;
			default:
				_ASSERT( 0 );
				break;
			}
			m_gpdlg->CalcFrameData();
			m_gpdlg->SetCurrentMotion( 0 );
			m_gpdlg->ParamsToDlg();
			m_gpdlg->Redraw();
		}else if( m_camdlg->m_gtype == GPG_ROT ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				m_draggpk->m_gpe.rot.x = newval;
				break;
			case DRAGMODE_Y:
				m_draggpk->m_gpe.rot.y = newval;
				break;
			case DRAGMODE_Z:
				m_draggpk->m_gpe.rot.z = newval;
				break;
			default:
				_ASSERT( 0 );
				break;
			}
			m_gpdlg->CalcFrameData();
			m_gpdlg->SetCurrentMotion( 0 );
			m_gpdlg->ParamsToDlg();
			m_gpdlg->Redraw();
		}

		Redraw();

	}

	return 0;
}

LRESULT CGraphRotDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( m_timerworking != 1 ){
		return 0;
	}

	if( g_motdlg->m_disptabno == TAB_CAM ){
		if( g_motdlg->GetCameraAnimNum() <= 0 ){
			return 0;
		}
		if( m_camdlg->m_camerabutton == CAMB_HEN )
			return 0;
	}else if( g_motdlg->m_disptabno == TAB_GP ){
		if( g_motdlg->GetGPAnimNum() <= 0 ){
			return 0;
		}
	}else{
		if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
			return 0;
		if( m_dlg->m_selectedno == 0 )
			return 0;
	}

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;

	if( !m_yoko_wnd.IsWindow() )
		return 0;
	if( !m_graph_wnd.IsWindow() )
		return 0;
	if( !m_dlg_wnd.IsWindow() )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	POINT clickpoint;
//	clickpoint.x = LOWORD(lParam);
//	clickpoint.y = HIWORD(lParam);
//	m_dlg_wnd.ClientToScreen( &clickpoint );//!!!!!!!!!!!!!!!!
	::GetCursorPos( &clickpoint );


	RECT yokorect;
	m_yoko_wnd.GetWindowRect( &yokorect );

	RECT grect;
	m_graph_wnd.GetWindowRect( &grect );

	if( (clickpoint.x >= yokorect.left) && (clickpoint.x <= yokorect.right) &&
		(clickpoint.y >= yokorect.top) && (clickpoint.y <= yokorect.bottom) ){

		m_dragyoko = 1;
		m_yokoval = clickpoint.x;

	}else if( (clickpoint.x > grect.left) && (clickpoint.x < grect.right) &&
		(clickpoint.y > grect.top) && (clickpoint.y < grect.bottom) ){

		POINT gpoint;
		gpoint = clickpoint;
		m_graph_wnd.ScreenToClient( &gpoint );

		m_dragmp = 0;
		m_dragck = 0;
		m_draggpk = 0;

		if( g_motdlg->m_disptabno == TAB_CAM ){
			if( (m_dragmode >= DRAGMODE_X) && (m_dragmode <= DRAGMODE_Z) ){
				PickMPCam( m_dragmode, gpoint, &m_dragck );
			}
			if( m_dragck ){
				m_dragvalflag = 1;
			}
		}else if( g_motdlg->m_disptabno == TAB_GP ){
			if( (m_dragmode >= DRAGMODE_X) && (m_dragmode <= DRAGMODE_Z) ){
				PickMPGP( m_dragmode, gpoint, &m_draggpk );
			}
			if( m_draggpk ){
				m_dragvalflag = 1;
			}
		}else{
			if( (m_dragmode >= DRAGMODE_X) && (m_dragmode <= DRAGMODE_Z) ){
				PickMP( m_dragmode, gpoint, &m_dragmp );
			}
			if( m_dragmp ){
				m_dragvalflag = 1;
				m_papp->CopyUndoBuffer( m_dragmp->m_frameno );
			}
		}
	}

	SetCapture();

	return 0;
}


LRESULT CGraphRotDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	static int busyflag = 0;

	if( m_timerworking != 1 ){
		return 0;
	}

	if( g_motdlg->m_disptabno == TAB_CAM ){
		if( g_motdlg->GetCameraAnimNum() <= 0 ){
			return 0;
		}
		if( m_camdlg->m_camerabutton == CAMB_HEN )
			return 0;
	}else if( g_motdlg->m_disptabno == TAB_GP ){
		if( g_motdlg->GetGPAnimNum() <= 0 ){
			return 0;
		}
	}else{
		if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
			return 0;
		if( m_dlg->m_selectedno == 0 )
			return 0;

		if( m_papp ){
			m_papp->OnPluginPose();
		}
	}


	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	if( busyflag == 1 )
		return 0;

	busyflag = 1;


	m_dragyoko = 0;
	m_yokoval = 0;

	m_dragmp = 0;
	m_dragck = 0;
	m_draggpk = 0;
	m_dragvalflag = 0;

	ReleaseCapture();
		
	busyflag = 0;

	return 0;
}

LRESULT CGraphRotDlg::OnRButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。

	bHandled = FALSE;

	if( g_motdlg->m_disptabno == TAB_CAM ){
		if( g_motdlg->GetCameraAnimNum() <= 0 ){
			return 0;
		}
		if( m_camdlg->m_camerabutton == CAMB_HEN )
			return 0;
	}else if( g_motdlg->m_disptabno == TAB_GP ){
		if( g_motdlg->GetGPAnimNum() <= 0 ){
			return 0;
		}
	}else{
		if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
			return 0;
		if( m_dlg->m_selectedno == 0 )
			return 0;
	}


	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;

	if( m_dragvalflag && ( m_dragmp || m_dragck || m_draggpk ) ){
		return 0;
	}
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	int ret;
	POINT clickpoint;
	clickpoint.x = LOWORD(lParam);
	clickpoint.y = HIWORD(lParam);
	m_dlg_wnd.ClientToScreen( &clickpoint );//!!!!!!!!!!!!!!!!

	RECT grect;
	m_graph_wnd.GetWindowRect( &grect );

	if( (clickpoint.x > grect.left) && (clickpoint.x < grect.right) &&
		(clickpoint.y > grect.top) && (clickpoint.y < grect.bottom) ){

		POINT gpoint;
		gpoint = clickpoint;
		m_graph_wnd.ScreenToClient( &gpoint );

		if( g_motdlg->m_disptabno == TAB_CAM ){
CCameraKey* pickck = 0;
			PickMPCam( DRAGMODE_X, gpoint, &pickck );
			if( !pickck ){
				PickMPCam( DRAGMODE_Y, gpoint, &pickck );
				if( !pickck ){
					PickMPCam( DRAGMODE_Z, gpoint, &pickck );
				}
			}

			if( pickck ){
				CloseSpDlg();

				int selectno;
				POINT pt;
				::GetCursorPos( &pt );
				selectno = m_fumenu->TrackPopupMenu( m_hWnd, pt, 1 );

				int setflag = 0;
				int setcam = -1;
				int setframe = -1;
				int setfu = INTERPOLATION_SLERP;

				if( selectno == 65200 ){
					setcam = -2;
					setframe = pickck->m_frameno;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65201 ){
					setcam = -2;
					setframe = pickck->m_frameno;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;				
				}else if( selectno == 65202 ){
					setcam = -2;
					setframe = -1;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65203 ){
					setcam = -2;
					setframe = -1;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;
				}else if( selectno == 65204 ){
					setcam = -1;
					setframe = -1;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65205 ){
					setcam = -1;
					setframe = -1;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;
				}
				if( setflag ){
					ret = m_camdlg->SetFillUpMode( m_camdlg->m_animno, setcam, setframe, setfu );
					if( ret ){
						DbgOut( "grotdlg : OnRButtonUp : camdlg SetFillUpMode error !!!\n" );
						_ASSERT( 0 );
						return -1;
					}

					Redraw();
				}
			}
		}else if( g_motdlg->m_disptabno == TAB_GP ){
			CGPKey* pickgpk = 0;
			PickMPGP( DRAGMODE_X, gpoint, &pickgpk );
			if( !pickgpk ){
				PickMPGP( DRAGMODE_Y, gpoint, &pickgpk );
				if( !pickgpk ){
					PickMPGP( DRAGMODE_Z, gpoint, &pickgpk );
				}
			}

			if( pickgpk ){
				CloseSpDlg();

				int selectno;
				POINT pt;
				::GetCursorPos( &pt );
				selectno = m_fumenu->TrackPopupMenu( m_hWnd, pt, 2 );

				int setflag = 0;
				int setframe = -1;
				int setfu = INTERPOLATION_SLERP;

				if( selectno == 65200 ){
					setframe = pickgpk->m_frameno;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65201 ){
					setframe = pickgpk->m_frameno;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;				
				}else if( selectno == 65204 ){
					setframe = -1;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65205 ){
					setframe = -1;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;
				}
				if( setflag ){
					ret = m_gpdlg->SetFillUpMode( m_gpdlg->m_animno, setframe, setfu );
					if( ret ){
						DbgOut( "grotdlg : OnRButtonUp : gpdlg SetFillUpMode error !!!\n" );
						_ASSERT( 0 );
						return -1;
					}

					Redraw();
				}
			}
		}else{
			int axiskind = INDX;
			CMotionPoint2* pickmp = 0;
			PickMP( DRAGMODE_X, gpoint, &pickmp );
			if( !pickmp ){
				PickMP( DRAGMODE_Y, gpoint, &pickmp );
				if( !pickmp ){
					PickMP( DRAGMODE_Z, gpoint, &pickmp );
					if( pickmp ){
						axiskind = INDZ;
					}
				}else{
					axiskind = INDY;
				}
			}else{
				axiskind = INDX;
			}

			if( pickmp ){
				CloseSpDlg();

				int selectno;
				POINT pt;
				::GetCursorPos( &pt );
				selectno = m_fumenu->TrackPopupMenu( m_hWnd, pt, 0 );

				int setflag = 0;
				int setseri = -1;
				int setframe = -1;
				int setfu = INTERPOLATION_SLERP;

				if( selectno == 65200 ){
					setseri = m_dlg->m_selectedno;
					setframe = pickmp->m_frameno;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65201 ){
					setseri = m_dlg->m_selectedno;
					setframe = pickmp->m_frameno;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;				
				}else if( selectno == 65202 ){
					setseri = m_dlg->m_selectedno;
					setframe = -1;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65203 ){
					setseri = m_dlg->m_selectedno;
					setframe = -1;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;
				}else if( selectno == 65204 ){
					setseri = -1;
					setframe = -1;
					setfu = INTERPOLATION_SLERP;
					setflag = 1;
				}else if( selectno == 65205 ){
					setseri = -1;
					setframe = -1;
					setfu = INTERPOLATION_SQUAD;
					setflag = 1;
				}else if( selectno == 65206 ){
					if( m_spdlg ){
						int mkind;
						SPPARAM* spp;
						if( pickmp->m_spp ){
							if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
								mkind = MKIND_MV;
								spp = &(pickmp->m_spp->m_mvparam);								
							}else if( m_papp->m_tbb_s ){
								mkind = MKIND_SC;
								spp = &(pickmp->m_spp->m_scparam);
							}else{
								mkind = MKIND_ROT;
								spp = &(pickmp->m_spp->m_rotparam);
							}

							m_spdlg->ShowDlg( SW_SHOW, m_dlg->m_thandler, m_dlg->m_shandler, m_dlg->m_mhandler, 
								m_dlg->m_selectedno, pickmp->m_frameno, mkind, spp );
						}
					}
				}else if( selectno == 65207 ){
					CSelBoneDlg dlg;
					int dlgret;
					dlgret = (int)dlg.DoModal();
					if( dlgret != IDOK ){
						return 0;
					}

					int curmkind;
					if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
						curmkind = MKIND_MV;
					}else if( m_papp->m_tbb_s ){
						curmkind = MKIND_SC;
					}else{
						curmkind = MKIND_ROT;
					}

					CSelRTSDlg dlg2( curmkind );
					dlgret = (int)dlg2.DoModal();
					if( dlgret != IDOK ){
						return 0;
					}

					int changefumode = 1;

					if( dlg.m_doall == 1 ){
						int doseri;
						for( doseri = 0; doseri < m_dlg->m_shandler->s2shd_leng; doseri++ ){
							CShdElem* dose;
							dose = (*m_dlg->m_shandler)( doseri );
							_ASSERT( dose );
							if( dose->IsJoint() ){
								if( dlg2.m_rot == 1 ){
									ret = AutoSplineAll( m_dlg->m_mhandler, m_dlg->m_mot_cookie, doseri, MKIND_ROT, changefumode );
									_ASSERT( !ret );
								}
								if( dlg2.m_mv == 1 ){
									ret = AutoSplineAll( m_dlg->m_mhandler, m_dlg->m_mot_cookie, doseri, MKIND_MV, changefumode );
									_ASSERT( !ret );
								}
								if( dlg2.m_sc == 1 ){
									ret = AutoSplineAll( m_dlg->m_mhandler, m_dlg->m_mot_cookie, doseri, MKIND_SC, changefumode );
									_ASSERT( !ret );
								}
							}
						}
						ret = m_dlg->m_mhandler->FillUpMotion( m_dlg->m_shandler, m_dlg->m_mot_cookie, -1,
							0, -1, 0 );
						_ASSERT( !ret );
					}else{
						if( dlg2.m_rot == 1 ){
							ret = AutoSplineAll( m_dlg->m_mhandler, m_dlg->m_mot_cookie, m_dlg->m_selectedno, MKIND_ROT, changefumode );
							_ASSERT( !ret );
						}
						if( dlg2.m_mv == 1 ){
							ret = AutoSplineAll( m_dlg->m_mhandler, m_dlg->m_mot_cookie, m_dlg->m_selectedno, MKIND_MV, changefumode );
							_ASSERT( !ret );
						}
						if( dlg2.m_sc == 1 ){
							ret = AutoSplineAll( m_dlg->m_mhandler, m_dlg->m_mot_cookie, m_dlg->m_selectedno, MKIND_SC, changefumode );
							_ASSERT( !ret );
						}
						ret = m_dlg->m_mhandler->FillUpMotion( m_dlg->m_shandler, m_dlg->m_mot_cookie, m_dlg->m_selectedno,
							0, -1, 0 );
						_ASSERT( !ret );
					}
				}else if( selectno == 65208 ){
					int mkind;
					if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
						mkind = MKIND_MV;
					}else if( m_papp->m_tbb_s ){
						mkind = MKIND_SC;
					}else{
						mkind = MKIND_ROT;
					}					

					int dlgret;
					CSelRTSDlg dlg2( mkind );
					dlgret = (int)dlg2.DoModal();
					if( dlgret != IDOK ){
						return 0;
					}
					
					int changefumode = 1;

					if( dlg2.m_rot == 1 ){
						ret = AutoSplineOne( MKIND_ROT, pickmp, changefumode );
						_ASSERT( !ret );
					}
					if( dlg2.m_mv == 1 ){
						ret = AutoSplineOne( MKIND_MV, pickmp, changefumode );
						_ASSERT( !ret );
					}
					if( dlg2.m_sc == 1 ){
						ret = AutoSplineOne( MKIND_SC, pickmp, changefumode );
						_ASSERT( !ret );
					}

					ret = m_dlg->m_mhandler->FillUpMotion( m_dlg->m_shandler, m_dlg->m_mot_cookie, m_dlg->m_selectedno,
						0, -1, 0 );
					_ASSERT( !ret );

				}else if( selectno == 65209 ){
					int mkind;
					if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
						mkind = MKIND_MV;
					}else if( m_papp->m_tbb_s ){
						mkind = MKIND_SC;
					}else{
						mkind = MKIND_ROT;
					}					

					int dlgret;
					CSelRTSDlg dlg2( mkind );
					dlgret = (int)dlg2.DoModal();
					if( dlgret != IDOK ){
						return 0;
					}
					
					int ret;
					int doseri;
					for( doseri = 0; doseri < m_dlg->m_shandler->s2shd_leng; doseri++ ){
						CShdElem* dose;
						dose = (*m_dlg->m_shandler)( doseri );
						_ASSERT( dose );
						if( dose->IsJoint() ){
							if( dlg2.m_rot ){
								ret = SetLockFlag( doseri, MKIND_ROT, 1 );
								_ASSERT( !ret );
							}
							if( dlg2.m_mv ){
								ret = SetLockFlag( doseri, MKIND_MV, 1 );
								_ASSERT( !ret );
							}
							if( dlg2.m_sc ){
								ret = SetLockFlag( doseri, MKIND_SC, 1 );
								_ASSERT( !ret );
							}						
						}
					}

				}else if( selectno == 65210 ){
					int mkind;
					if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
						mkind = MKIND_MV;
					}else if( m_papp->m_tbb_s ){
						mkind = MKIND_SC;
					}else{
						mkind = MKIND_ROT;
					}					

					int dlgret;
					CSelRTSDlg dlg2( mkind );
					dlgret = (int)dlg2.DoModal();
					if( dlgret != IDOK ){
						return 0;
					}

					int ret;
					int doseri;
					for( doseri = 0; doseri < m_dlg->m_shandler->s2shd_leng; doseri++ ){
						CShdElem* dose;
						dose = (*m_dlg->m_shandler)( doseri );
						_ASSERT( dose );
						if( dose->IsJoint() ){
							if( dlg2.m_rot ){
								ret = SetLockFlag( doseri, MKIND_ROT, 0 );
								_ASSERT( !ret );
							}
							if( dlg2.m_mv ){
								ret = SetLockFlag( doseri, MKIND_MV, 0 );
								_ASSERT( !ret );
							}
							if( dlg2.m_sc ){
								ret = SetLockFlag( doseri, MKIND_SC, 0 );
								_ASSERT( !ret );
							}						
						}
					}
				}
				if( setflag ){
					ret = m_dlg->SetFillUpMode( setseri, setframe, setfu );
					if( ret ){
						DbgOut( "grotdlg : OnRButtonUp : mpdlg SetFillUpMode error !!!\n" );
						_ASSERT( 0 );
						return -1;
					}

					Redraw();
				}
			}
		}
	}

	return 0;
}



LRESULT CGraphRotDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		
	bHandled = TRUE;

	if( m_timerworking == 0 )//KillTimerのあとも、キューに残ったwm_timerが悪さをしないように。
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	if( m_papp->m_ExistData == 0 )
		return 0;

	int timerid = (int)wParam;
	if( (timerid == GraphRotDlgTIMER) && (m_cmdshow != SW_HIDE) ){

		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}


int CGraphRotDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;

	if( (m_cmdshow == SW_HIDE) ){
		CloseSpDlg();

//		ReleaseCapture();
//		m_mcap = 0;
//		m_moving = 0;

	}

	ShowWindow( m_cmdshow );

	//int ret;
	if( m_cmdshow != SW_HIDE ){
//		m_dlg->m_current = 0;//!!!!!!!!!!!!!!!!!!!!!! 別スレッドでのセットが遅れるため、ここで強制リセット。
		OnUserSelchange();
	}


	return 0;
}

int CGraphRotDlg::DrawEul()
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler ) 
//		return 0;
//	if( m_dlg->m_selectedno == 0 )
//		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	//int ret;
	RECT clirect;
	m_graph_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_graph_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}
	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler ) 
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;


	int setno = 0;
	LONG frameno;
	POINT befxp;
	POINT befyp;
	POINT befzp;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;

		if( setno == 0 ){
			befxp = curgmp->xp;
			befyp = curgmp->yp;
			befzp = curgmp->zp;
		}

		if( curgmp->inrangeflag ){
			POINT curxp, curyp, curzp;

			curxp = curgmp->xp;
			SelectObject( hdc, m_red_pen );
			SelectObject( hdc, m_red_brush );
			::MoveToEx( hdc, befxp.x, befxp.y, NULL );
			::LineTo( hdc, curxp.x, curxp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curxp.x - MP_SIZE, curxp.y - MP_SIZE, curxp.x + MP_SIZE, curxp.y + MP_SIZE );
			}

			curyp = curgmp->yp;
			SelectObject( hdc, m_green_pen );
			SelectObject( hdc, m_green_brush );
			::MoveToEx( hdc, befyp.x, befyp.y, NULL );
			::LineTo( hdc, curyp.x, curyp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curyp.x - MP_SIZE, curyp.y - MP_SIZE, curyp.x + MP_SIZE, curyp.y + MP_SIZE );
			}

			curzp = curgmp->zp;
			SelectObject( hdc, m_blue_pen );
			SelectObject( hdc, m_blue_brush );
			::MoveToEx( hdc, befzp.x, befzp.y, NULL );
			::LineTo( hdc, curzp.x, curzp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curzp.x - MP_SIZE, curzp.y - MP_SIZE, curzp.x + MP_SIZE, curzp.y + MP_SIZE );
			}

			if( m_dispctrl == 1 ){
				if( curgmp->isspline == 1 ){
					//SelectObject( hdc, m_yellow_pen );
					//SelectObject( hdc, m_yellow_brush );
					if( curgmp->hasmpflag == 1 ){
						int ikind;
						for( ikind = INDX; ikind <= INDZ; ikind++ ){
							if( ikind == INDX ){
								SelectObject( hdc, m_red_pen );
								SelectObject( hdc, m_red_brush );
							}else if( ikind == INDY ){
								SelectObject( hdc, m_green_pen );
								SelectObject( hdc, m_green_brush );
							}else if( ikind == INDZ ){
								SelectObject( hdc, m_blue_pen );
								SelectObject( hdc, m_blue_brush );
							}
							POINT curctrls = curgmp->ctrls[ikind];
							if( (curctrls.x >= clirect.left) && (curctrls.x <= clirect.right) ){
								::Ellipse( hdc, curctrls.x - CTRL_SIZE, curctrls.y - CTRL_SIZE, curctrls.x + CTRL_SIZE, curctrls.y + CTRL_SIZE );
							}
							POINT curctrle = curgmp->ctrle[ikind];
							if( (curctrle.x >= clirect.left) && (curctrle.x <= clirect.right) ){
								::Ellipse( hdc, curctrle.x - CTRL_SIZE, curctrle.y - CTRL_SIZE, curctrle.x + CTRL_SIZE, curctrle.y + CTRL_SIZE );	
							}
						}
					}
				}
			}

			befxp = curxp;
			befyp = curyp;
			befzp = curzp;
		}
		setno++;
	}

	if( (m_dlg->m_current >= m_startframe) && (m_dlg->m_current <= m_endframe) ){
		SelectObject( hdc, m_gray_pen );
		SelectObject( hdc, m_gray_brush );

		int curyoko;
		Frameno2Windowpos( clirect, m_dlg->m_current, &curyoko );

		::MoveToEx( hdc, curyoko, clirect.top, NULL );
		::LineTo( hdc, curyoko, clirect.bottom );
	}




	m_graph_wnd.ReleaseDC( hdc );
	


	return 0;
}


/***
int CGraphRotDlg::DrawTateAxis()
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler ) 
//		return 0;
//	if( m_dlg->m_selectedno == 0 )
//		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	int ret;
	RECT clirect;
	m_tate_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_tate_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}
	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );

	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_black_brush );


	if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
		int itatemin, itatemax;
		itatemin = (int)m_tatemin;
		itatemax = (int)m_tatemax;

		int tate;
		for( tate = itatemin; tate <= itatemax; tate++ ){
			if( (tate % 500) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 200) && ((itatemax - tate) >= 200) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 250) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}

	}else if( m_papp->m_tbb_s ){

		int itatemin, itatemax;
		itatemin = 0;
		itatemax = (int)( m_tatemax * 100.0f );

		int tate;
		for( tate = itatemin; tate <= itatemax; tate++ ){
			if( (tate % 50) == 0 ){
				int ypos;
				ret = TateFL2Windowpos( clirect, (float)tate / 100.0f, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 10) && ((itatemax - tate) >= 10) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%.2f", (float)tate / 100.0f);
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 25) == 0 ){
				int ypos;
				ret = TateFL2Windowpos( clirect, (float)tate / 100.0f, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}else{
		int itatemin, itatemax;
		itatemin = (int)m_tatemin;
		itatemax = (int)m_tatemax;

		int tate;
		for( tate = itatemin; tate <= itatemax; tate++ ){
			if( (tate % 40) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 10) && ((itatemax - tate) >= 10) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 20) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}
	m_tate_wnd.ReleaseDC( hdc );
	
	return 0;

}
***/
int CGraphRotDlg::DrawTateAxis()
{
//	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler ) 
//		return 0;
//	if( m_dlg->m_selectedno == 0 )
//		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	int ret;
	RECT clirect;
	m_tate_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_tate_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}
	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );

	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_black_brush );


	if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
		int itatemin, itatemax;
		itatemin = (int)m_tatemin;
		itatemax = (int)m_tatemax;

		int memori_step;
		if( itatemax < 100 ){
			memori_step = 20;
		}else if( itatemax < 200 ){
			memori_step = 50;
		}else if( itatemax < 500 ){
			memori_step = 100;
		}else{
			memori_step = 250;
		}

		int tatestart, divnum, taterest;
		divnum = itatemin / memori_step;
		taterest = itatemin % memori_step;
		if( taterest == 0 ){
			tatestart = memori_step * divnum;
		}else{
			tatestart = memori_step * (divnum + 1);
		}

		int tate;
		for( tate = tatestart; tate <= itatemax; tate += memori_step ){
			if( (tate % (memori_step * 2)) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= (memori_step - 30)) && ((itatemax - tate) >= (memori_step - 30)) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % memori_step) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}

	}else if( m_papp->m_tbb_s ){

		int itatemin, itatemax;
		itatemin = 0;
		itatemax = (int)( m_tatemax * 100.0f );

		int tate;
		for( tate = itatemin; tate <= itatemax; tate += 25 ){
			if( (tate % 50) == 0 ){
				int ypos;
				ret = TateFL2Windowpos( clirect, (float)tate / 100.0f, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 10) && ((itatemax - tate) >= 10) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%.2f", (float)tate / 100.0f);
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 25) == 0 ){
				int ypos;
				ret = TateFL2Windowpos( clirect, (float)tate / 100.0f, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}else{
		int itatemin, itatemax;
		itatemin = (int)m_tatemin;
		itatemax = (int)m_tatemax;

		int tatestart, divnum, taterest;
		divnum = itatemin / 20;
		taterest = itatemin % 20;
		if( taterest == 0 ){
			tatestart = 20 * divnum;
		}else{
			tatestart = 20 * (divnum + 1);
		}

		int tate;
		for( tate = tatestart; tate <= itatemax; tate += 20 ){
			if( (tate % 40) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 10) && ((itatemax - tate) >= 10) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 20) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}
	m_tate_wnd.ReleaseDC( hdc );
	
	return 0;

}


int CGraphRotDlg::DrawYokoAxis()
{

	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	int ret;
	RECT clirect;
	m_yoko_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_yoko_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}

	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );


	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_black_brush );

	LONG frameno;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		if( (frameno % 10) == 0 ){
			int xpos;
			ret = Frameno2Windowpos( clirect, frameno, &xpos );
			_ASSERT( !ret );


			::MoveToEx( hdc, xpos, clirect.top, NULL );
			::LineTo( hdc, xpos, clirect.top + 10 );

			if( (m_endframe - frameno) >= 10 ){
				char stryoko[20];
				sprintf_s( stryoko, 20, "%d", frameno );
				int stryokoleng;
				stryokoleng = (int)strlen( stryoko );
				RECT strrect;
				strrect.top = clirect.top + 10;
				strrect.bottom = clirect.bottom;
				strrect.left = xpos;
				strrect.right = xpos + stryokoleng * 8;

				::DrawText( hdc, stryoko, stryokoleng, &strrect, 0 );
			}
		}else if( (frameno % 5) == 0 ){
			int xpos;
			ret = Frameno2Windowpos( clirect, frameno, &xpos );
			_ASSERT( !ret );

			::MoveToEx( hdc, xpos, clirect.top, NULL );
			::LineTo( hdc, xpos, clirect.top + 5 );
		}
	}

	m_yoko_wnd.ReleaseDC( hdc );

	return 0;
}
int CGraphRotDlg::CalcMP()
{

	//0 return 時にもデフォルト値が設定されるように
	if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
		m_rotmin = -50.0f;
		m_rotmax = 50.0f;
	}else if( m_papp->m_tbb_s ){
		m_rotmin = 0.0f;
		m_rotmax = 1.5f;
	}else{
		m_rotmin = -30.0f;
		m_rotmax = 30.0f;
	}
	if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
		m_tatemin = m_rotmin - 50.0f;
		m_tatemax = m_rotmax + 50.0f;
	}else if( m_papp->m_tbb_s ){
		m_tatemin = 0.0f;
		m_tatemax = m_rotmax + 0.25f;
	}else{
		m_tatemin = m_rotmin - 20.0f;
		m_tatemax = m_rotmax + 20.0f;
	}



	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	int ret;

	CMotionCtrl* mcptr;
	mcptr = (*m_dlg->m_mhandler)( m_dlg->m_selectedno );
	_ASSERT( mcptr );
	CShdElem* selem;
	selem = (*m_dlg->m_shandler)( m_dlg->m_selectedno );
	_ASSERT( selem );


	int frameno;
	int setno = 0;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		if( frameno <= m_dlg->m_sl_mp_rangemax ){
			CMotionPoint2* mpptr;
			mpptr = &((m_gmp + setno)->mp);
			ret = mcptr->CalcMotionPointOnFrame( selem, mpptr, m_dlg->m_mot_cookie, frameno, &((m_gmp + setno)->hasmpflag),
				m_dlg->m_shandler, m_dlg->m_mhandler );
			if( ret ){
				DbgOut( "grotdlg : CalcMP : mp CalcMotionPointOnFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			mpptr->m_frameno = frameno;
			(m_gmp + setno)->inrangeflag = 1;


			if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
				if( mpptr->m_mvx > m_rotmax ){
					m_rotmax = mpptr->m_mvx;
				}
				if( mpptr->m_mvx < m_rotmin ){
					m_rotmin = mpptr->m_mvx;
				}

				if( mpptr->m_mvy > m_rotmax ){
					m_rotmax = mpptr->m_mvy;
				}
				if( mpptr->m_mvy < m_rotmin ){
					m_rotmin = mpptr->m_mvy;
				}

				if( mpptr->m_mvz > m_rotmax ){
					m_rotmax = mpptr->m_mvz;
				}
				if( mpptr->m_mvz < m_rotmin ){
					m_rotmin = mpptr->m_mvz;
				}

			}else if( m_papp->m_tbb_s ){
				if( mpptr->m_scalex > m_rotmax ){
					m_rotmax = mpptr->m_scalex;
				}
				if( mpptr->m_scalex < m_rotmin ){
					m_rotmin = mpptr->m_scalex;
				}

				if( mpptr->m_scaley > m_rotmax ){
					m_rotmax = mpptr->m_scaley;
				}
				if( mpptr->m_scaley < m_rotmin ){
					m_rotmin = mpptr->m_scaley;
				}

				if( mpptr->m_scalez > m_rotmax ){
					m_rotmax = mpptr->m_scalez;
				}
				if( mpptr->m_scalez < m_rotmin ){
					m_rotmin = mpptr->m_scalez;
				}

			}else{
				if( mpptr->m_eul.x > m_rotmax ){
					m_rotmax = mpptr->m_eul.x;
				}
				if( mpptr->m_eul.x < m_rotmin ){
					m_rotmin = mpptr->m_eul.x;
				}

				if( mpptr->m_eul.y > m_rotmax ){
					m_rotmax = mpptr->m_eul.y;
				}
				if( mpptr->m_eul.y < m_rotmin ){
					m_rotmin = mpptr->m_eul.y;
				}

				if( mpptr->m_eul.z > m_rotmax ){
					m_rotmax = mpptr->m_eul.z;
				}
				if( mpptr->m_eul.z < m_rotmin ){
					m_rotmin = mpptr->m_eul.z;
				}
			}
		}else{
			(m_gmp + setno)->inrangeflag = 0;
		}
		setno++;
	}

	if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
		m_tatemin = m_rotmin - 50.0f;
		m_tatemax = m_rotmax + 50.0f;
	}else if( m_papp->m_tbb_s ){
		m_tatemin = 0.0f;
		m_tatemax = m_rotmax + 0.25f;
	}else{
		m_tatemin = m_rotmin - 20.0f;
		m_tatemax = m_rotmax + 20.0f;
	}

	RECT yokorect;
	m_yoko_wnd.GetClientRect( &yokorect );
	RECT taterect;
	m_tate_wnd.GetClientRect( &taterect );

	setno = 0;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;
		curgmp->isspline = 0;
		if( curgmp->inrangeflag ){

			CMotionPoint2* orgmpptr = 0;
			if( curgmp->hasmpflag == 1 ){
				ret = m_dlg->m_mhandler->IsExistMotionPoint( m_dlg->m_mot_cookie, m_dlg->m_selectedno, frameno, &orgmpptr );
				if( ret || !orgmpptr ){
					DbgOut( "grotdlg : CalcMP : mh IsExistMotionPoint error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				if( orgmpptr->m_spp && (orgmpptr->interp == INTERPOLATION_SQUAD) ){
					curgmp->isspline = 1;
				}
			}


			int yoko;
			Frameno2Windowpos( yokorect, frameno, &yoko );

			int xtate, ytate, ztate;

			if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
				TateFL2Windowpos( taterect, curgmp->mp.m_mvx, &xtate );
				TateFL2Windowpos( taterect, curgmp->mp.m_mvy, &ytate );
				TateFL2Windowpos( taterect, curgmp->mp.m_mvz, &ztate );

				if( curgmp->isspline == 1 ){
					int ikind;
					for( ikind = INDX; ikind <= INDZ; ikind++ ){
						int tmpx, tmpy;
						FramenoFL2Windowpos( yokorect, orgmpptr->m_spp->m_mvparam.ctrlS[ikind].sppoint.x, &tmpx );
						TateFL2Windowpos( taterect, orgmpptr->m_spp->m_mvparam.ctrlS[ikind].sppoint.y, &tmpy );
						curgmp->ctrls[ikind].x = tmpx;
						curgmp->ctrls[ikind].y = tmpy;

						FramenoFL2Windowpos( yokorect, orgmpptr->m_spp->m_mvparam.ctrlE[ikind].sppoint.x, &tmpx );
						TateFL2Windowpos( taterect, orgmpptr->m_spp->m_mvparam.ctrlE[ikind].sppoint.y, &tmpy );
						curgmp->ctrle[ikind].x = tmpx;
						curgmp->ctrle[ikind].y = tmpy;
					}
				}
			}else if( m_papp->m_tbb_s ){
				TateFL2Windowpos( taterect, curgmp->mp.m_scalex, &xtate );
				TateFL2Windowpos( taterect, curgmp->mp.m_scaley, &ytate );
				TateFL2Windowpos( taterect, curgmp->mp.m_scalez, &ztate );

				if( curgmp->isspline == 1 ){
					int ikind;
					for( ikind = INDX; ikind <= INDZ; ikind++ ){
						int tmpx, tmpy;
						FramenoFL2Windowpos( yokorect, orgmpptr->m_spp->m_scparam.ctrlS[ikind].sppoint.x, &tmpx );
						TateFL2Windowpos( taterect, orgmpptr->m_spp->m_scparam.ctrlS[ikind].sppoint.y, &tmpy );
						curgmp->ctrls[ikind].x = tmpx;
						curgmp->ctrls[ikind].y = tmpy;

						FramenoFL2Windowpos( yokorect, orgmpptr->m_spp->m_scparam.ctrlE[ikind].sppoint.x, &tmpx );
						TateFL2Windowpos( taterect, orgmpptr->m_spp->m_scparam.ctrlE[ikind].sppoint.y, &tmpy );
						curgmp->ctrle[ikind].x = tmpx;
						curgmp->ctrle[ikind].y = tmpy;
					}
				}
			}else{
				TateFL2Windowpos( taterect, curgmp->mp.m_eul.x, &xtate );
				TateFL2Windowpos( taterect, curgmp->mp.m_eul.y, &ytate );
				TateFL2Windowpos( taterect, curgmp->mp.m_eul.z, &ztate );

				if( curgmp->isspline == 1 ){
					int ikind;
					for( ikind = INDX; ikind <= INDZ; ikind++ ){
						int tmpx, tmpy;
						FramenoFL2Windowpos( yokorect, orgmpptr->m_spp->m_rotparam.ctrlS[ikind].sppoint.x, &tmpx );
						TateFL2Windowpos( taterect, orgmpptr->m_spp->m_rotparam.ctrlS[ikind].sppoint.y, &tmpy );
						curgmp->ctrls[ikind].x = tmpx;
						curgmp->ctrls[ikind].y = tmpy;

						FramenoFL2Windowpos( yokorect, orgmpptr->m_spp->m_rotparam.ctrlE[ikind].sppoint.x, &tmpx );
						TateFL2Windowpos( taterect, orgmpptr->m_spp->m_rotparam.ctrlE[ikind].sppoint.y, &tmpy );
						curgmp->ctrle[ikind].x = tmpx;
						curgmp->ctrle[ikind].y = tmpy;
					}
				}
			}
			curgmp->xp.x = yoko;
			curgmp->xp.y = xtate;

			curgmp->yp.x = yoko;
			curgmp->yp.y = ytate;

			curgmp->zp.x = yoko;
			curgmp->zp.y = ztate;

		}else{
			curgmp->xp.x = 0;
			curgmp->xp.y = 0;

			curgmp->yp.x = 0;
			curgmp->yp.y = 0;

			curgmp->zp.x = 0;
			curgmp->zp.y = 0;

		}

		setno++;
	}



	return 0;
}

int CGraphRotDlg::CheckDragModeButton()
{
	switch( m_dragmode ){
	case DRAGMODE_X:
		if( !m_dragx_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragx_wnd, BM_SETSTATE, TRUE, 0 );  
		if( !m_dragy_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragy_wnd, BM_SETSTATE, FALSE, 0 );  
		if( !m_dragz_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragz_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case DRAGMODE_Y:
		if( !m_dragx_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragx_wnd, BM_SETSTATE, FALSE, 0 );  
		if( !m_dragy_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragy_wnd, BM_SETSTATE, TRUE, 0 );  
		if( !m_dragz_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragz_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case DRAGMODE_Z:
		if( !m_dragx_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragx_wnd, BM_SETSTATE, FALSE, 0 );  
		if( !m_dragy_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragy_wnd, BM_SETSTATE, FALSE, 0 );  
		if( !m_dragz_wnd.IsWindow() )
			return 0;
		SendMessage( m_dragz_wnd, BM_SETSTATE, TRUE, 0 );  
		break;
	default:
		break;
	}

	return 0;
}

int CGraphRotDlg::DrawDragVal()
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	char strmes[256];

	if( m_dragvalflag && m_dragmp ){
		if( m_papp->m_tbb_t || m_papp->m_tbb_t2 ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_dragmp->m_mvx );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_dragmp->m_mvy );
				break;
			case DRAGMODE_Z:
				sprintf_s( strmes, 256, "drag Z ( %f )", m_dragmp->m_mvz );
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}
		}else if( m_papp->m_tbb_s ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_dragmp->m_scalex );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_dragmp->m_scaley );
				break;
			case DRAGMODE_Z:
				sprintf_s( strmes, 256, "drag Z ( %f )", m_dragmp->m_scalez );
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}
		}else{
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_dragmp->m_eul.x );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_dragmp->m_eul.y );
				break;
			case DRAGMODE_Z:
				sprintf_s( strmes, 256, "drag Z ( %f )", m_dragmp->m_eul.z );
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}
		}
	}else{
		strcpy_s( strmes, 256, "no drag elem" );
	}

	m_dragval_wnd.SetWindowTextA( strmes );


	return 0;
}

LRESULT CGraphRotDlg::OnDragXButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_dragmode = DRAGMODE_X;
	CheckDragModeButton();

	return 0;
}
LRESULT CGraphRotDlg::OnDragYButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_dragmode = DRAGMODE_Y;
	CheckDragModeButton();

	return 0;
}
LRESULT CGraphRotDlg::OnDragZButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_dragmode = DRAGMODE_Z;
	CheckDragModeButton();

	return 0;
}

int CGraphRotDlg::PickMP( int dragmode, POINT point, CMotionPoint2** ppmp )
{
	*ppmp = 0;

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler ) 
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	int ret;
	int setno = 0;
	int frameno;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;

		if( curgmp->hasmpflag == 1 ){
			POINT curp;
			switch( dragmode ){
			case DRAGMODE_X:
				curp = curgmp->xp;
				break;
			case DRAGMODE_Y:
				curp = curgmp->yp;
				break;
			case DRAGMODE_Z:
				curp = curgmp->zp;
				break;
			default:
				_ASSERT( 0 );
				curp.x = 0;
				curp.y = 0;
				break;
			}

			if( (point.x >= curp.x - MP_SIZE) && (point.x <= curp.x + MP_SIZE) && 
				(point.y >= curp.y - MP_SIZE) && (point.y <= curp.y + MP_SIZE) ){

				ret = m_dlg->m_mhandler->IsExistMotionPoint( m_dlg->m_mot_cookie, m_dlg->m_selectedno, frameno, ppmp );
				_ASSERT( !ret );

				if( *ppmp ){
					break;
				}
			}
		}

		setno++;
	}



	return 0;
}

int CGraphRotDlg::DrawTateAxisCam()
{
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	int ret;
	RECT clirect;
	m_tate_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_tate_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}
	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );

	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_black_brush );


	int itatemin, itatemax;
	itatemin = (int)m_tatemin;
	itatemax = (int)m_tatemax;

	if( m_camdlg->m_gtype != G_PROJ ){
		int tatestart, divnum, taterest;
		divnum = itatemin / 250;
		taterest = itatemin % 250;
		if( taterest == 0 ){
			tatestart = 250 * divnum;
		}else{
			tatestart = 250 * (divnum + 1);
		}

		int tate;
		for( tate = tatestart; tate <= itatemax; tate += 250 ){
			if( (tate % 500) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 200) && ((itatemax - tate) >= 200) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 250) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}else{
		int tatestart, divnum, taterest;
		divnum = itatemin / 2500;
		taterest = itatemin % 2500;
		if( taterest == 0 ){
			tatestart = 2500 * divnum;
		}else{
			tatestart = 2500 * (divnum + 1);
		}


		int tate;
		for( tate = tatestart; tate <= itatemax; tate += 2500 ){
			if( (tate % 5000) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 200) && ((itatemax - tate) >= 200) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 2500) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}
	m_tate_wnd.ReleaseDC( hdc );
	
	return 0;
}

int CGraphRotDlg::CalcMPCam()
{

	// 0 return 時にもデフォルト値が設定されるように
	if( (m_camdlg->m_gtype == G_POS) || (m_camdlg->m_gtype == G_TARGET) ){
		m_rotmin = -50.0f;
		m_rotmax = 50.0f;
	}else{
		//G_PROJ
		m_rotmin = 0.0f;
		m_rotmax = 40500.0f;
	}
	if( (m_camdlg->m_gtype == G_POS) || (m_camdlg->m_gtype == G_TARGET) ){
		m_tatemin = m_rotmin - 50.0f;
		m_tatemax = m_rotmax + 50.0f;
	}else{
		//G_PROJ
		m_tatemin = m_rotmin - 200.0f;
		m_tatemax = m_rotmax + 800.0f;
	}



	if( g_motdlg->GetCameraAnimNum() <= 0 )
		return 0;
	if( m_camdlg->m_camerabutton == CAMB_HEN )
		return 0;

	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	int ret;


	int frameno;
	int setno = 0;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		if( frameno <= m_camdlg->m_sl_mp_rangemax ){
			CCameraKey* ckptr;
			ckptr = &((m_gmp + setno)->ck);
			ret = m_camdlg->CalcMotionPointOnFrame( m_camdlg->m_animno, -2, frameno, ckptr, &((m_gmp + setno)->hasmpflag) );
			if( ret ){
				DbgOut( "grotdlg : CalcMPCam : camdlg CalcMotionPointOnFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ckptr->m_frameno = frameno;

			(m_gmp + setno)->inrangeflag = 1;

			if( m_camdlg->m_gtype == G_POS ){
				m_rotmax = max( ckptr->m_celem.pos.x, m_rotmax );
				m_rotmin = min( ckptr->m_celem.pos.x, m_rotmin );

				m_rotmax = max( ckptr->m_celem.pos.y, m_rotmax );
				m_rotmin = min( ckptr->m_celem.pos.y, m_rotmin );

				m_rotmax = max( ckptr->m_celem.pos.z, m_rotmax );
				m_rotmin = min( ckptr->m_celem.pos.z, m_rotmin );
			}else if( m_camdlg->m_gtype == G_TARGET ){
				m_rotmax = max( ckptr->m_celem.target.x, m_rotmax );
				m_rotmin = min( ckptr->m_celem.target.x, m_rotmin );

				m_rotmax = max( ckptr->m_celem.target.y, m_rotmax );
				m_rotmin = min( ckptr->m_celem.target.y, m_rotmin );

				m_rotmax = max( ckptr->m_celem.target.z, m_rotmax );
				m_rotmin = min( ckptr->m_celem.target.z, m_rotmin );
			}else if( m_camdlg->m_gtype == G_PROJ ){
				m_rotmax = max( ckptr->m_celem.nearz, m_rotmax );
				m_rotmin = min( ckptr->m_celem.nearz, m_rotmin );

				m_rotmax = max( ckptr->m_celem.farz, m_rotmax );
				m_rotmin = min( ckptr->m_celem.farz, m_rotmin );

				if( m_camdlg->m_ortho == 0 ){
					m_rotmax = max( ckptr->m_celem.fov, m_rotmax );
					m_rotmin = min( ckptr->m_celem.fov, m_rotmin );
				}else{
					m_rotmax = max( ckptr->m_celem.orthosize, m_rotmax );
					m_rotmin = min( ckptr->m_celem.orthosize, m_rotmin );
				}
			}else{
				_ASSERT( 0 );
			}


		}else{
			(m_gmp + setno)->inrangeflag = 0;
		}
		setno++;
	}

	if( (m_camdlg->m_gtype == G_POS) || (m_camdlg->m_gtype == G_TARGET) ){
		m_tatemin = m_rotmin - 50.0f;
		m_tatemax = m_rotmax + 50.0f;
	}else{
		//G_PROJ
		m_tatemin = m_rotmin - 200.0f;
		m_tatemax = m_rotmax + 800.0f;
	}


	RECT yokorect;
	m_yoko_wnd.GetClientRect( &yokorect );
	RECT taterect;
	m_tate_wnd.GetClientRect( &taterect );

	setno = 0;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;
		if( curgmp->inrangeflag ){
			int yoko;
			Frameno2Windowpos( yokorect, frameno, &yoko );

			int xtate, ytate, ztate;

			if( m_camdlg->m_gtype == G_POS ){
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.pos.x, &xtate );
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.pos.y, &ytate );
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.pos.z, &ztate );
			}else if( m_camdlg->m_gtype == G_TARGET ){
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.target.x, &xtate );
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.target.y, &ytate );
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.target.z, &ztate );
			}else{
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.nearz, &xtate );
				TateFL2Windowpos( taterect, curgmp->ck.m_celem.farz, &ytate );
				if( m_camdlg->m_ortho == 0 ){
					TateFL2Windowpos( taterect, curgmp->ck.m_celem.fov, &ztate );
				}else{
					TateFL2Windowpos( taterect, curgmp->ck.m_celem.orthosize, &ztate );
				}
			}
			curgmp->xp.x = yoko;
			curgmp->xp.y = xtate;

			curgmp->yp.x = yoko;
			curgmp->yp.y = ytate;

			curgmp->zp.x = yoko;
			curgmp->zp.y = ztate;

		}else{
			curgmp->xp.x = 0;
			curgmp->xp.y = 0;

			curgmp->yp.x = 0;
			curgmp->yp.y = 0;

			curgmp->zp.x = 0;
			curgmp->zp.y = 0;

		}

		setno++;
	}

	return 0;
}

int CGraphRotDlg::DrawEulCam()
{
//	if( g_motdlg->GetCameraAnimNum() <= 0 )
//		return 0;
//	if( m_camdlg->m_camerabutton == CAMB_HEN )
//		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	//int ret;
	RECT clirect;
	m_graph_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_graph_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}
	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );

	if( g_motdlg->GetCameraAnimNum() <= 0 )
		return 0;
	if( m_camdlg->m_camerabutton == CAMB_HEN )
		return 0;


	int setno = 0;
	LONG frameno;
	POINT befxp;
	POINT befyp;
	POINT befzp;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;

		if( setno == 0 ){
			befxp = curgmp->xp;
			befyp = curgmp->yp;
			befzp = curgmp->zp;
		}

		if( curgmp->inrangeflag ){
			POINT curxp, curyp, curzp;

			curxp = curgmp->xp;
			SelectObject( hdc, m_red_pen );
			SelectObject( hdc, m_red_brush );
			::MoveToEx( hdc, befxp.x, befxp.y, NULL );
			::LineTo( hdc, curxp.x, curxp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curxp.x - MP_SIZE, curxp.y - MP_SIZE, curxp.x + MP_SIZE, curxp.y + MP_SIZE );
			}

			curyp = curgmp->yp;
			SelectObject( hdc, m_green_pen );
			SelectObject( hdc, m_green_brush );
			::MoveToEx( hdc, befyp.x, befyp.y, NULL );
			::LineTo( hdc, curyp.x, curyp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curyp.x - MP_SIZE, curyp.y - MP_SIZE, curyp.x + MP_SIZE, curyp.y + MP_SIZE );
			}

			curzp = curgmp->zp;
			SelectObject( hdc, m_blue_pen );
			SelectObject( hdc, m_blue_brush );
			::MoveToEx( hdc, befzp.x, befzp.y, NULL );
			::LineTo( hdc, curzp.x, curzp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curzp.x - MP_SIZE, curzp.y - MP_SIZE, curzp.x + MP_SIZE, curzp.y + MP_SIZE );
			}

			befxp = curxp;
			befyp = curyp;
			befzp = curzp;
		}
		setno++;
	}

	if( (m_camdlg->m_current >= m_startframe) && (m_camdlg->m_current <= m_endframe) ){
		SelectObject( hdc, m_gray_pen );
		SelectObject( hdc, m_gray_brush );

		int curyoko;
		Frameno2Windowpos( clirect, m_camdlg->m_current, &curyoko );

		::MoveToEx( hdc, curyoko, clirect.top, NULL );
		::LineTo( hdc, curyoko, clirect.bottom );
	}

	m_graph_wnd.ReleaseDC( hdc );
	
	return 0;
}

int CGraphRotDlg::DrawDragValCam()
{
	if( g_motdlg->GetCameraAnimNum() <= 0 )
		return 0;
	if( m_camdlg->m_camerabutton == CAMB_HEN )
		return 0;
	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	char strmes[256];
	strcpy_s( strmes, 256, "no drag elem" );

	if( m_dragvalflag && m_dragck ){
		if( m_camdlg->m_gtype == G_POS ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_dragck->m_celem.pos.x );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_dragck->m_celem.pos.y );
				break;
			case DRAGMODE_Z:
				sprintf_s( strmes, 256, "drag Z ( %f )", m_dragck->m_celem.pos.z );
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}
		}else if( m_camdlg->m_gtype == G_TARGET ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_dragck->m_celem.target.x );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_dragck->m_celem.target.y );
				break;
			case DRAGMODE_Z:
				sprintf_s( strmes, 256, "drag Z ( %f )", m_dragck->m_celem.target.z );
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}

		}else if( m_camdlg->m_gtype == G_PROJ ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_dragck->m_celem.nearz );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_dragck->m_celem.farz );
				break;
			case DRAGMODE_Z:
				if( m_camdlg->m_ortho == 0 ){
					sprintf_s( strmes, 256, "drag Z ( %f )", m_dragck->m_celem.fov );
				}else{
					sprintf_s( strmes, 256, "drag Z ( %f )", m_dragck->m_celem.orthosize );
				}
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}

		}
	}else{
		strcpy_s( strmes, 256, "no drag elem" );
	}

	m_dragval_wnd.SetWindowTextA( strmes );

	return 0;
}

int CGraphRotDlg::PickMPCam( int dragmode, POINT point, CCameraKey** ppck )
{
	*ppck = 0;

	if( g_motdlg->GetCameraAnimNum() <= 0 )
		return 0;
	if( m_camdlg->m_camerabutton == CAMB_HEN )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	int ret;
	int setno = 0;
	int frameno;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;

		if( curgmp->hasmpflag == 1 ){
			POINT curp;
			switch( dragmode ){
			case DRAGMODE_X:
				curp = curgmp->xp;
				break;
			case DRAGMODE_Y:
				curp = curgmp->yp;
				break;
			case DRAGMODE_Z:
				curp = curgmp->zp;
				break;
			default:
				_ASSERT( 0 );
				curp.x = 0;
				curp.y = 0;
				break;
			}

			if( (point.x >= curp.x - MP_SIZE) && (point.x <= curp.x + MP_SIZE) && 
				(point.y >= curp.y - MP_SIZE) && (point.y <= curp.y + MP_SIZE) ){

				ret = m_camdlg->ExistCameraKey( m_camdlg->m_animno, -2, frameno, ppck );
				_ASSERT( !ret );

				if( *ppck ){
					break;
				}
			}
		}

		setno++;
	}

	return 0;
}

int CGraphRotDlg::DrawTateAxisGP()
{
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	int ret;
	RECT clirect;
	m_tate_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_tate_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}
	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );

	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_black_brush );


	int itatemin, itatemax;
	itatemin = (int)m_tatemin;
	itatemax = (int)m_tatemax;

	if( m_gpdlg->m_gtype == GPG_POS ){
		int tatestart, divnum, taterest;
		divnum = itatemin / 250;
		taterest = itatemin % 250;
		if( taterest == 0 ){
			tatestart = 250 * divnum;
		}else{
			tatestart = 250 * (divnum + 1);
		}

		int tate;
		for( tate = tatestart; tate <= itatemax; tate += 250 ){
			if( (tate % 500) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 200) && ((itatemax - tate) >= 200) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 250) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}else if( m_gpdlg->m_gtype == GPG_ROT ){
		int itatemin, itatemax;
		itatemin = (int)m_tatemin;
		itatemax = (int)m_tatemax;

		int tatestart, divnum, taterest;
		divnum = itatemin / 20;
		taterest = itatemin % 20;
		if( taterest == 0 ){
			tatestart = 20 * divnum;
		}else{
			tatestart = 20 * (divnum + 1);
		}

		int tate;
		for( tate = tatestart; tate <= itatemax; tate += 20 ){
			if( (tate % 40) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 10, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );

				if( ((tate - itatemin) >= 10) && ((itatemax - tate) >= 10) ){
					char strtate[20];
					sprintf_s( strtate, 20, "%d", tate );
					int strtateleng;
					strtateleng = (int)strlen( strtate );
					RECT strrect;
					strrect.top = ypos;
					strrect.bottom = ypos + 15;
					strrect.left = clirect.left;
					strrect.right = clirect.right;

					::DrawText( hdc, strtate, strtateleng, &strrect, 0 );
				}
			}else if( (tate % 20) == 0 ){
				int ypos;
				ret = Tate2Windowpos( clirect, tate, &ypos );
				_ASSERT( !ret );

				::MoveToEx( hdc, clirect.right - 5, ypos, NULL );
				::LineTo( hdc, clirect.right, ypos );
			}
		}
	}
	m_tate_wnd.ReleaseDC( hdc );

	return 0;
}
int CGraphRotDlg::CalcMPGP()
{
	// 0 return 時にもデフォルト値が設定されるように
	if( m_gpdlg->m_gtype == GPG_POS ){
		m_rotmin = -50.0f;
		m_rotmax = 50.0f;
	}else{
		m_rotmin = -30.0f;
		m_rotmax = 30.0f;
	}
	if( m_gpdlg->m_gtype == GPG_POS ){
		m_tatemin = m_rotmin - 50.0f;
		m_tatemax = m_rotmax + 50.0f;
	}else{
		m_tatemin = m_rotmin - 20.0f;
		m_tatemax = m_rotmax + 20.0f;
	}


	if( g_motdlg->GetGPAnimNum() <= 0 )
		return 0;

	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	int ret;


	int frameno;
	int setno = 0;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		if( frameno <= m_gpdlg->m_sl_mp_rangemax ){
			CGPKey* gpkptr;
			gpkptr = &((m_gmp + setno)->gpk);
			ret = m_gpdlg->GetCalcGPKey( frameno, gpkptr );
			if( ret ){
				DbgOut( "grotdlg : CalcMPGP : gpdlg GetCalcGPKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			gpkptr->m_frameno = frameno;

			(m_gmp + setno)->inrangeflag = 1;
			(m_gmp + setno)->hasmpflag = gpkptr->m_keyflag;

			if( m_gpdlg->m_gtype == GPG_POS ){
				m_rotmax = max( gpkptr->m_gpe.pos.x, m_rotmax );
				m_rotmin = min( gpkptr->m_gpe.pos.x, m_rotmin );

				m_rotmax = max( gpkptr->m_gpe.pos.y, m_rotmax );
				m_rotmin = min( gpkptr->m_gpe.pos.y, m_rotmin );

				m_rotmax = max( gpkptr->m_gpe.pos.z, m_rotmax );
				m_rotmin = min( gpkptr->m_gpe.pos.z, m_rotmin );
			}else if( m_gpdlg->m_gtype == GPG_ROT ){
				m_rotmax = max( gpkptr->m_gpe.rot.x, m_rotmax );
				m_rotmin = min( gpkptr->m_gpe.rot.x, m_rotmin );

				m_rotmax = max( gpkptr->m_gpe.rot.y, m_rotmax );
				m_rotmin = min( gpkptr->m_gpe.rot.y, m_rotmin );

				m_rotmax = max( gpkptr->m_gpe.rot.z, m_rotmax );
				m_rotmin = min( gpkptr->m_gpe.rot.z, m_rotmin );
			}else{
				_ASSERT( 0 );
			}

		}else{
			(m_gmp + setno)->inrangeflag = 0;
			(m_gmp + setno)->hasmpflag = 0;
		}
		setno++;
	}

	if( m_gpdlg->m_gtype == GPG_POS ){
		m_tatemin = m_rotmin - 50.0f;
		m_tatemax = m_rotmax + 50.0f;
	}else{
		m_tatemin = m_rotmin - 20.0f;
		m_tatemax = m_rotmax + 20.0f;
	}


	RECT yokorect;
	m_yoko_wnd.GetClientRect( &yokorect );
	RECT taterect;
	m_tate_wnd.GetClientRect( &taterect );

	setno = 0;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;
		if( curgmp->inrangeflag ){
			int yoko;
			Frameno2Windowpos( yokorect, frameno, &yoko );

			int xtate, ytate, ztate;

			if( m_gpdlg->m_gtype == GPG_POS ){
				TateFL2Windowpos( taterect, curgmp->gpk.m_gpe.pos.x, &xtate );
				TateFL2Windowpos( taterect, curgmp->gpk.m_gpe.pos.y, &ytate );
				TateFL2Windowpos( taterect, curgmp->gpk.m_gpe.pos.z, &ztate );
			}else if( m_gpdlg->m_gtype == GPG_ROT ){
				TateFL2Windowpos( taterect, curgmp->gpk.m_gpe.rot.x, &xtate );
				TateFL2Windowpos( taterect, curgmp->gpk.m_gpe.rot.y, &ytate );
				TateFL2Windowpos( taterect, curgmp->gpk.m_gpe.rot.z, &ztate );
			}else{
				_ASSERT( 0 );
			}
			curgmp->xp.x = yoko;
			curgmp->xp.y = xtate;

			curgmp->yp.x = yoko;
			curgmp->yp.y = ytate;

			curgmp->zp.x = yoko;
			curgmp->zp.y = ztate;

		}else{
			curgmp->xp.x = 0;
			curgmp->xp.y = 0;

			curgmp->yp.x = 0;
			curgmp->yp.y = 0;

			curgmp->zp.x = 0;
			curgmp->zp.y = 0;

		}

		setno++;
	}

	return 0;
}
int CGraphRotDlg::DrawEulGP()
{
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	//int ret;
	RECT clirect;
	m_graph_wnd.GetClientRect( &clirect );
	
	HDC hdc;
	hdc = m_graph_wnd.GetDC();
	if( hdc == NULL ){
		return 0;
	}
	SelectObject( hdc, m_black_pen );
	SelectObject( hdc, m_white_brush );
	::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );

	if( g_motdlg->GetGPAnimNum() <= 0 )
		return 0;


	int setno = 0;
	LONG frameno;
	POINT befxp;
	POINT befyp;
	POINT befzp;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;

		if( setno == 0 ){
			befxp = curgmp->xp;
			befyp = curgmp->yp;
			befzp = curgmp->zp;
		}

		if( curgmp->inrangeflag ){
			POINT curxp, curyp, curzp;

			curxp = curgmp->xp;
			SelectObject( hdc, m_red_pen );
			SelectObject( hdc, m_red_brush );
			::MoveToEx( hdc, befxp.x, befxp.y, NULL );
			::LineTo( hdc, curxp.x, curxp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curxp.x - MP_SIZE, curxp.y - MP_SIZE, curxp.x + MP_SIZE, curxp.y + MP_SIZE );
			}

			curyp = curgmp->yp;
			SelectObject( hdc, m_green_pen );
			SelectObject( hdc, m_green_brush );
			::MoveToEx( hdc, befyp.x, befyp.y, NULL );
			::LineTo( hdc, curyp.x, curyp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curyp.x - MP_SIZE, curyp.y - MP_SIZE, curyp.x + MP_SIZE, curyp.y + MP_SIZE );
			}

			curzp = curgmp->zp;
			SelectObject( hdc, m_blue_pen );
			SelectObject( hdc, m_blue_brush );
			::MoveToEx( hdc, befzp.x, befzp.y, NULL );
			::LineTo( hdc, curzp.x, curzp.y );
			if( curgmp->hasmpflag == 1 ){
				::Ellipse( hdc, curzp.x - MP_SIZE, curzp.y - MP_SIZE, curzp.x + MP_SIZE, curzp.y + MP_SIZE );
			}

			befxp = curxp;
			befyp = curyp;
			befzp = curzp;
		}
		setno++;
	}

	if( (m_gpdlg->m_current >= m_startframe) && (m_gpdlg->m_current <= m_endframe) ){
		SelectObject( hdc, m_gray_pen );
		SelectObject( hdc, m_gray_brush );

		int curyoko;
		Frameno2Windowpos( clirect, m_gpdlg->m_current, &curyoko );

		::MoveToEx( hdc, curyoko, clirect.top, NULL );
		::LineTo( hdc, curyoko, clirect.bottom );
	}

	m_graph_wnd.ReleaseDC( hdc );

	return 0;
}
int CGraphRotDlg::DrawDragValGP()
{
	if( g_motdlg->GetGPAnimNum() <= 0 )
		return 0;
	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	char strmes[256];
	strcpy_s( strmes, 256, "no drag elem" );

	if( m_dragvalflag && m_draggpk ){
		if( m_gpdlg->m_gtype == GPG_POS ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_draggpk->m_gpe.pos.x );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_draggpk->m_gpe.pos.y );
				break;
			case DRAGMODE_Z:
				sprintf_s( strmes, 256, "drag Z ( %f )", m_draggpk->m_gpe.pos.z );
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}
		}else if( m_gpdlg->m_gtype == GPG_ROT ){
			switch( m_dragmode ){
			case DRAGMODE_X:
				sprintf_s( strmes, 256, "drag X ( %f )", m_draggpk->m_gpe.rot.x );
				break;
			case DRAGMODE_Y:
				sprintf_s( strmes, 256, "drag Y ( %f )", m_draggpk->m_gpe.rot.y );
				break;
			case DRAGMODE_Z:
				sprintf_s( strmes, 256, "drag Z ( %f )", m_draggpk->m_gpe.rot.z );
				break;
			default:
				_ASSERT( 0 );
				strcpy_s( strmes, 256, "no drag elem" );
				break;
			}

		}
	}else{
		strcpy_s( strmes, 256, "no drag elem" );
	}

	m_dragval_wnd.SetWindowTextA( strmes );

	return 0;
}
int CGraphRotDlg::PickMPGP( int dragmode, POINT point, CGPKey** ppgpk )
{
	*ppgpk = 0;

	if( g_motdlg->GetGPAnimNum() <= 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( !m_gmp )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;


	int ret;
	int setno = 0;
	int frameno;
	for( frameno = m_startframe; frameno <= m_endframe; frameno++ ){
		GMP* curgmp;
		curgmp = m_gmp + setno;

		if( curgmp->hasmpflag == 1 ){
			POINT curp;
			switch( dragmode ){
			case DRAGMODE_X:
				curp = curgmp->xp;
				break;
			case DRAGMODE_Y:
				curp = curgmp->yp;
				break;
			case DRAGMODE_Z:
				curp = curgmp->zp;
				break;
			default:
				_ASSERT( 0 );
				curp.x = 0;
				curp.y = 0;
				break;
			}

			if( (point.x >= curp.x - MP_SIZE) && (point.x <= curp.x + MP_SIZE) && 
				(point.y >= curp.y - MP_SIZE) && (point.y <= curp.y + MP_SIZE) ){

				ret = m_gpdlg->ExistGPKey( m_gpdlg->m_animno, frameno, ppgpk );
				_ASSERT( !ret );

				if( *ppgpk ){
					break;
				}
			}
		}

		setno++;
	}

	return 0;
}

int CGraphRotDlg::InitPaint()
{
	if( m_cmdshow == SW_HIDE )
		return 0;
	if( (m_dragmode < 0) || (m_dragmode > DRAGMODE_Z) )
		return 0;

	//int ret;
	RECT clirect;
	HDC hdcyoko;
	m_yoko_wnd.GetClientRect( &clirect );
	hdcyoko = m_yoko_wnd.GetDC();
	if( hdcyoko == NULL ){
		return 0;
	}
	SelectObject( hdcyoko, m_black_pen );
	SelectObject( hdcyoko, m_white_brush );
	::Rectangle( hdcyoko, clirect.left, clirect.top, clirect.right, clirect.bottom );
	m_yoko_wnd.ReleaseDC( hdcyoko );


	m_tate_wnd.GetClientRect( &clirect );	
	HDC hdctate;
	hdctate = m_tate_wnd.GetDC();
	if( hdctate == NULL ){
		return 0;
	}
	SelectObject( hdctate, m_black_pen );
	SelectObject( hdctate, m_white_brush );
	::Rectangle( hdctate, clirect.left, clirect.top, clirect.right, clirect.bottom );
	m_tate_wnd.ReleaseDC( hdctate );


	m_graph_wnd.GetClientRect( &clirect );	
	HDC hdcg;
	hdcg = m_graph_wnd.GetDC();
	if( hdcg == NULL ){
		return 0;
	}
	SelectObject( hdcg, m_black_pen );
	SelectObject( hdcg, m_white_brush );
	::Rectangle( hdcg, clirect.left, clirect.top, clirect.right, clirect.bottom );
	m_graph_wnd.ReleaseDC( hdcg );

	return 0;
}

int CGraphRotDlg::CloseSpDlg()
{
	if( m_spdlg ){
		m_spdlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0, 0, 0 );
	}
	return 0;
}

int CGraphRotDlg::SetLockFlag( int boneno, int mkind, int flag )
{

	CMotionCtrl* mcptr;
	mcptr = (*m_dlg->m_mhandler)( boneno );
	_ASSERT( mcptr );

	CMotionPoint2* mpptr = 0;
	mpptr = mcptr->GetMotionPoint( m_dlg->m_mot_cookie );

	while( mpptr ){
		SPPARAM* curspp = 0;

		switch( mkind ){
		case MKIND_ROT:
			curspp = &(mpptr->m_spp->m_rotparam);
			break;
		case MKIND_MV:
			curspp = &(mpptr->m_spp->m_mvparam);
			break;
		case MKIND_SC:
			curspp = &(mpptr->m_spp->m_scparam);
			break;
		default:
			_ASSERT( 0 );
			break;
		}

		if( curspp ){
			curspp->lockflag = flag;
		}

		mpptr = mpptr->next;
	}


	return 0;
}


LRESULT CGraphRotDlg::OnCheckCtrl(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CTRL );
	if( ischecked == BST_CHECKED ){
		m_dispctrl = 1;
	}else{
		m_dispctrl = 0;
	}
	return 0;
}
