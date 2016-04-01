// AlpKeyDlg.cpp : CAlpKeyDlg �̃C���v�������e�[�V����
#include "stdafx.h"
#include "AlpKeyDlg.h"

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

#include <AlpKey.h>
#include <AlpChange.h>
#include <mqomaterial.h>


#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "viewer.h"
#include "MotDlg.h"
#include "MotParamDlg.h"

#include "EditAlpDlg.h"
#include "SyncDlg.h"

#include <crtdbg.h>


//MotionPoint�`�掞�̓_�̔��a
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
// CAlpKeyDlg

CAlpKeyDlg::CAlpKeyDlg( CMotParamDlg* srcmpdlg )
//: m_list_wnd( NULL, this, 2 )
{

	m_dlg = srcmpdlg;
	m_dlg->m_alpkeydlg = this;

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

	m_dragalpk = 0;
	m_dragframe = 0;
	m_dragflag = 0;


	m_timerworking = 0;

	m_cmdshow = SW_HIDE;
	m_createlistflag = 0;
	m_mcap = 0;
	m_moving = 0;

	m_remakelistflag = 0;

	m_cpalpk = 0;

	m_ctrldrag = 0;
	m_altdrag = 0;
	m_dragx = 0;

}


CAlpKeyDlg::~CAlpKeyDlg()
{
	if( m_cpalpk ){
		delete m_cpalpk;
		m_cpalpk = 0;
	}
}

int CAlpKeyDlg::DestroyTimer()
{

	if( ::IsWindow( m_hWnd ) ){
		int ret;
		ret = ::KillTimer( m_hWnd, AlpKeyDlgTIMER );
		//_ASSERT( ret );
	}
	m_timerworking = 0;

	return 0;
}

LRESULT CAlpKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	int ret;

	ret = SetWnd();
	_ASSERT( !ret );
	s_sl_mp_wnd = m_sl_mp_wnd;

	ret = SetNewWindowProc();
	_ASSERT( !ret );


	ret = ParamsToDlg();
	_ASSERT( !ret );

	int timerid;
	timerid = (int)::SetTimer( m_hWnd, AlpKeyDlgTIMER, 500, NULL );
	_ASSERT( timerid == AlpKeyDlgTIMER );
	m_timerworking = 1;

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}


int CAlpKeyDlg::InitDlg()
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

LRESULT CAlpKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "AlpKeyDlg : OnOK\n" );
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}

LRESULT CAlpKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}
LRESULT CAlpKeyDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
	ShowWindow( SW_HIDE );
	//m_displayflag = 0;
	//::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	return 0;
}


int CAlpKeyDlg::SetWnd()
{

	HWND temphwnd;

	m_dlg_wnd = m_hWnd;
	s_dlg_wnd = m_hWnd;

	temphwnd = GetDlgItem( IDC_LIST1 );
	m_list_wnd = temphwnd;
	s_list_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEX_CURRENT );
	m_alp_current_wnd = temphwnd;

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

	//slider�̏����ݒ�
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//


	return 0;
}

int CAlpKeyDlg::ParamsToDlg()
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
	sprintf_s( tempchar, 1024, "frameno: current %d, start %d, end %d, total %d", 
		m_dlg->m_current, m_dlg->m_startframe, m_dlg->m_endframe, m_dlg->m_sl_mp_rangemax + 1 );
	if( (g_endapp == 1) ||!m_alp_current_wnd.IsWindow() )
		return 0;
	m_alp_current_wnd.SetWindowText( tempchar );

	if( m_cmdshow != SW_SHOW ){
		return 0;
	}

//name
	if( (g_endapp == 0) && m_dlg->m_thandler ){
		CTreeElem2* curtelem = (*m_dlg->m_thandler)( m_dlg->m_selectedno );
		_ASSERT( curtelem );
		if( (g_endapp == 1) ||!m_sellock_name_wnd.IsWindow() )
			return 0;
		m_sellock_name_wnd.SetWindowText( curtelem->name );
	}else{
		if( (g_endapp == 1) ||!m_sellock_name_wnd.IsWindow() )
			return 0;
		m_sellock_name_wnd.SetWindowText( "" );		
	}


//m_sl_mp_
	if( (g_endapp == 1) ||!m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_dlg->m_startframe );
	if( (g_endapp == 1) ||!m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_dlg->m_endframe );
	if( (g_endapp == 1) ||!m_sl_mp_wnd.IsWindow() )
		return 0;
	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_dlg->m_current );
	if( (g_endapp == 1) ||!m_sl_mp_wnd.IsWindow() )
		return 0;
	SetSliderTic( m_sl_mp_wnd, 10, m_dlg->m_startframe, m_dlg->m_endframe );//

	return 0;
}

LRESULT CAlpKeyDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_sl_mp_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );
	}

	return 0;
}


LRESULT CAlpKeyDlg::OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
LRESULT CAlpKeyDlg::OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{

	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;


	int delta;
	if( idCtrl == 999 ){
		delta = pnmud->iDelta;
	}else{
		// default�ŁA���{�^�����v���X�ɂȂ��Ă���̂ŁA���]����B
		delta = -(pnmud->iDelta);
	}

	// �X�s���R���g���[����iPos���N�����v���Ȃ��悤�ɁA������Ԃɖ߂��B
	m_sp_mp_wnd.SendMessage( UDM_SETPOS, 0, 0 );

	if( delta == 10000 ){
		//�ŏI�t���[��
		m_dlg->m_current = m_dlg->m_endframe;
	}else if( delta == -10000 ){
		//�擪�t���[��
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
void CAlpKeyDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	if( g_endapp == 1 ){
		return;
	}

	dstwnd.SendMessage( TBM_CLEARTICS, 0, 0 );

	// !!! mintic��maxtic�ɂ́A�����I��tic������̂Őݒ�͂��Ȃ��B(���Ă͂����Ȃ�)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		if( g_endapp == 1 ){
			return;
		}
		dstwnd.SendMessage( TBM_SETTIC, 1, (LPARAM)(LONG)curtic );
		curtic += ticfreq;
	}
}


int CAlpKeyDlg::Frameno2Windowpos( RECT clirect, int frameno, int* posptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_dlg->m_endframe - m_dlg->m_startframe );
	*posptr = MP_SIZE + (int)(step * (float)( frameno - m_dlg->m_startframe ) ) + clirect.left;
	if( *posptr < clirect.left )
		*posptr = clirect.left;
	if( *posptr > clirect.right )
		*posptr = clirect.right;
	
	return 0;
}

int CAlpKeyDlg::Windowpos2Frameno( RECT clirect, int posx, int* framenoptr )
{
	
	float step = (float)(clirect.right - clirect.left - 2 * MP_SIZE) / (float)( m_dlg->m_endframe - m_dlg->m_startframe );
	float tempfloat;
	tempfloat = (float)(posx - MP_SIZE - clirect.left) / step; 
	//�l�̌ܓ�����frameno�ɂ���B
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

int CAlpKeyDlg::DrawAlpKeys()
{
	if( g_endapp == 1 ){
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

//�w�i�h��Ԃ�
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

//�Z���^�[���C��
	SelectObject( hdc, m_black_brush );
    ::MoveToEx( hdc, clirect.left, clirect.bottom / 2, NULL ); 
    ::LineTo( hdc, clirect.right, clirect.bottom / 2 );

//current bar
	int barposx;
	Frameno2Windowpos( clirect, m_dlg->m_current, &barposx );

	::MoveToEx( hdc, barposx, clirect.top, NULL );
	::LineTo( hdc, barposx, clirect.bottom );

//�_
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;

	CAlpKey* alpkptr = m_dlg->m_mhandler->GetFirstAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie );
	CAlpKey* curalpk = alpkptr;
	while( curalpk ){
		int frameno = curalpk->frameno;
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

		curalpk = curalpk->next;
		//DbgOut( "AlpKeyDlg : DrawMotionPoint : frame %d\n", frameno );
	}

	
	if( m_dragalpk ){
		int frameno = m_dragalpk->frameno;
		
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

int CAlpKeyDlg::Redraw()
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;

	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}

LRESULT CAlpKeyDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
	bHandled = FALSE;

	if( !IsWindow() )
		return 0;

//	/***
//	if( uMsg != 0 )
//		::DefWindowProc( m_hWnd, uMsg, wParam, lParam );
//	else
//		UpdateAllWnd();
//	***/
	
	DrawAlpKeys();

	return 0;
}

LRESULT CAlpKeyDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
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
LRESULT CAlpKeyDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
//DbgOut( "AlpKeyDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CAlpKeyDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_ismoving = 0;
/***
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B

	if( m_apphwnd ){
		::SendMessage( m_apphwnd, WM_USER_MOVE, (WPARAM)ID_MSG_FROM_MOTDLG, 0 );
	}
***/
	DefWindowProc();

	Redraw();

	return 0;
}



LRESULT CAlpKeyDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	int ret;

	//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = ::MessageBox( m_hWnd, "�S�ẴA���t�@�̃L�[�t���[�����폜���܂��B\n��낵���ł����H", "�m�F�_�C�A���O", MB_OKCANCEL ); 
	//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK )
		return 0;

	int i;
	for( i = 0; i < m_dlg->m_mhandler->s2mot_leng; i++ ){
		CAlpKey* alpkptr;
		alpkptr = m_dlg->m_mhandler->GetFirstAlpKey( i, m_dlg->m_mot_cookie );

		CAlpKey* curalpk = alpkptr;
		CAlpKey* nexttk;
		while( curalpk ){
			int frameno = curalpk->frameno;
			nexttk = curalpk->next;
			
			ret = m_dlg->m_mhandler->DeleteAlpKey( i, m_dlg->m_mot_cookie, frameno );
			if( ret ){
				DbgOut( "alpkeydlg : OnAllDelete : mh DeleteAlpKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			curalpk = nexttk;
		}
	}
	
	if( m_dragalpk ){
		delete m_dragalpk;
		m_dragalpk = 0;
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


LRESULT CAlpKeyDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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


LRESULT CAlpKeyDlg::OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0 );


	BOOL dummyb;
	return m_dlg->OnPreview( 0, 0, 0, dummyb );

}

int CAlpKeyDlg::OnUserSelchange()
{
	int ret;
	m_remakelistflag = 1;
	ret = InitList();
	_ASSERT( !ret );
	//SetCapture();

	ParamsToDlg();
	Redraw();


/***
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( m_selectedno == 0 )
		return 0;

	HTREEITEM selitem;
	selitem = *(m_TI + selno );

	if( selitem ){
		m_selectedno = selno;
		m_selecteditem = selitem;

		TreeView_SelectItem( m_tree_mot_wnd,
			m_selecteditem );

		BOOL dummy;
		OnReleasedcaptureSl_mp( 0, 0, dummy );

///////////

//		TreeView_SelectSetFirstVisible( m_tree_mot_wnd, m_selecteditem );

		OnRadioModeJoint(0, 0, 0, dummy);//!!!!!!!!!!!!!!!!!!
		//OnRadioModeOpe(0, 0, 0, dummy);//!!!!!!!!!!!!!!!!!!

		if( m_cmdshow != SW_HIDE ){
			Redraw();
			::UpdateWindow( m_tree_mot_wnd );
		}

	}
***/
	return 0;
}

int CAlpKeyDlg::CaptureM()
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


LRESULT CAlpKeyDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
	bHandled = FALSE;


	CaptureM();


	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	//if( wParam == MK_LBUTTON )
	//	DbgOut( "AlpKeyDlg : OnMouseMove : MK_LBUTTON\n" );

	if( (m_dragflag == 1) && m_dragalpk ){
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

		m_dragalpk->frameno = frameno;

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


LRESULT CAlpKeyDlg::OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	int ret;
	CAlpKey* curalpk = 0;
	ret = m_dlg->m_mhandler->ExistAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curalpk );
	if( ret ){
		DbgOut( "alpkeydlg : OnFrameCopy : mh ExistAlpKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( !curalpk ){
		::MessageBox( m_hWnd, "���̃t���[���ɂ̓R�s�[�ł���L�[������܂���B", "���s", MB_OK );
		return 0;//!!!!!!!!!!!!
	}

	if( m_cpalpk ){
		delete m_cpalpk;
		m_cpalpk = 0;
	}

	m_cpalpk = new CAlpKey();
	if( !m_cpalpk ){
		DbgOut( "alpkeydlg : OnFrameCopy : cpalpk alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_cpalpk->CopyAlpChange( curalpk );
	if( ret ){
		DbgOut( "alpkeydlg : OnFrameCopy : cpalpk CopyAlpChange error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

LRESULT CAlpKeyDlg::OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	

	if( !m_cpalpk ){
		::MessageBox( m_hWnd, "�t���[���R�s�[���s���Ă���Ď��s���Ă��������B", "���s", MB_OK );
		return 0;//!!!!!!!!!!!!!!!
	}

	int ret;
	CAlpKey* curalpk = 0;
	ret = m_dlg->m_mhandler->ExistAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curalpk );
	if( ret ){
		DbgOut( "alpkeydlg : OnFramePaste : mh ExistAlpKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( !curalpk ){
		CAlpKey* addtk;
		addtk = new CAlpKey();
		if( !addtk ){
			DbgOut( "alpkeydlg : OnFramePaste : addtk alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = addtk->CopyAlpChange( m_cpalpk );
		if( ret ){
			DbgOut( "alpkeydlg : OnFramePaste : addtk CopyAlpChange error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		addtk->frameno = m_dlg->m_current;

		ret = m_dlg->m_mhandler->ChainAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, addtk );
		if( ret ){
			DbgOut( "alpkeydlg : OnFramePaste : mh ChainAlpKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{

		ret = curalpk->CopyAlpChange( m_cpalpk );
		if( ret ){
			DbgOut( "alpkeydlg : OnFramePaste : addtk CopyAlpChange 1 error !!!\n" );
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

LRESULT CAlpKeyDlg::OnFrameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;
	
	int ret;
	CAlpKey* curalpk = 0;
	ret = m_dlg->m_mhandler->ExistAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curalpk );
	if( ret ){
		DbgOut( "alpkeydlg : OnFrameDel : mh ExistAlpKey error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( curalpk ){
		ret = m_dlg->m_mhandler->DeleteAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current );
		if( ret ){
			DbgOut( "alpkeydlg : OnFrameDel : mh DeleteAlpKey error !!!\n" );
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


LRESULT CAlpKeyDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B

	bHandled = FALSE;

/***
	if( m_cmdshow == SW_HIDE ){
		return 0;//!!!!!!!!!!!!!!!!!!!
	}


	RECT viewclient;
	m_list_wnd.GetClientRect( &viewclient );

	POINT inview;
	::GetCursorPos( &inview );
	m_list_wnd.ScreenToClient( &inview );

DbgOut( "alpkeydlg : OnLButtonDown : inview ( %d %d )\r\n", inview.x, inview.y );
DbgOut( "alpkeydlg : viewclient : left %d, right %d, top %d, bottom %d\r\n",
	   viewclient.left, viewclient.right, viewclient.top, viewclient.bottom );

	RECT itemrect;
	ZeroMemory( &itemrect, sizeof( RECT ) );
	ListView_GetItemRect( m_list_wnd, 0, &itemrect, LVIR_BOUNDS );

	if( (inview.x >= viewclient.left) && (inview.x <= viewclient.right) && (inview.y >= itemrect.top) && (inview.y <= viewclient.bottom) ){
		//top�́Aitemrect.top�Ƃ̔�r�I�I�I
		OnList1( inview );
	}
***/


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

			CAlpKey* isexist = 0;
			m_dlg->m_mhandler->ExistAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, frameno, &isexist );
			if( isexist ){
				m_dragframe = frameno;
				m_dragflag = 1;
				m_dragalpk = isexist;

				ret = m_dlg->m_mhandler->LeaveFromChainAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dragframe );
				if( ret ){
					DbgOut( "AlpKeyDlg : OnLButtonDown : LeaveFromChainAlpKey error !!!\n" );
					_ASSERT( 0 );
					m_dragframe = 0;
					m_dragflag = 0;
					m_dragalpk = 0;
					return 1;
				}

				DbgOut( "AlpKeyDlg : OnLButtonDown : drag start, posx %d, m_dragframe %d\n",
					posx, m_dragframe );
			}else{
				m_dragframe = 0;
				m_dragflag = 0;
				m_dragalpk = 0;
			}
		}else{
			m_dragframe = 0;
			m_dragflag = 0;
			m_dragalpk = 0;
		}
	}

	return 0;
}


LRESULT CAlpKeyDlg::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
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

	CAlpKey* isexist = 0;
	CAlpKey* isexist2 = 0;

	if( m_ctrldrag || m_altdrag ){
		m_ctrldrag = 0;
		m_altdrag = 0;
		m_dragx = 0;
	}else{

		if( (m_dragflag == 1) && m_dragalpk ){
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

	//DbgOut( "AlpKeyDlg : OnLButtonUp : Windowpos2Frameno : frameno %d\n", frameno );

			ret = m_dlg->m_mhandler->ExistAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, frameno, &isexist );
			_ASSERT( !ret );
			if( isexist ){
				char messtr[1024];
				ZeroMemory( messtr, 1024 );
				sprintf_s( messtr, 1024, "�t���[���ԍ�%d �ɂ͊��Ƀ��[�V�����|�C���g�����݂���̂ŁA�ړ��ł��܂���B",
					frameno );
				//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
				MessageBox( messtr, "�ړ��ł��܂���", MB_OK );
				//::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	//DbgOut( "AlpKeyDlg : OnLButtonUp : already exist : m_dragframe %d, frameno %d\n", m_dragframe, frameno );
				frameno = m_dragframe;//!!!!! ���ɖ߂�
			}
			
			m_dragalpk->frameno = frameno;//!!!!!!!!!!!!!!!!

			ret = m_dlg->m_mhandler->ChainAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dragalpk );
			if( ret ){
				DbgOut( "alpkeydlg : OnLButtonUp : mh ChainAlpKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			m_dragalpk = 0;


			m_remakelistflag = 1;
			ret = InitList();
			_ASSERT( !ret );
			//SetCapture();

			ParamsToDlg();
			Redraw();

			//DbgOut( "AlpKeyDlg : OnLButtonUp : drag end, posx %d, frameno %d, m_dragframe %d\n",
			//	posx, frameno, m_dragframe );
		}

		//if( frameno == m_dragframe ){
		//	DbgOut( "AlpKeyDlg : OnLButtonUp : motion point already exist warning %d, %d !!!\n", frameno, m_dragframe );
		//}
	}

	m_dragflag = 0;
	m_dragframe = 0;

	busyflag = 0;

	return 0;
}
LRESULT CALLBACK MPProcAlp(
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
		//key �͖����ɂ���B

		//if( uMsg == WM_KEYDOWN ){
		//	DbgOut( "AlpKeyDlg : MPProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "AlpKeyDlg : MPProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
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
int CAlpKeyDlg::SetNewWindowProc()
{
	s_mpfunc = ::GetWindowLong( m_sl_mp_wnd, GWL_WNDPROC );
	if( !s_mpfunc ){
		_ASSERT( 0 );
		return 1;
	}

//	s_listfunc = ::GetWindowLong( m_list_wnd, GWL_WNDPROC );
//	if( !s_listfunc ){
//		_ASSERT( 0 );
//		return 1;
//	}


	LONG lret;
	lret = ::SetWindowLong( m_sl_mp_wnd, GWL_WNDPROC, (LONG)MPProcAlp );
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


LRESULT CAlpKeyDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}
		
	bHandled = TRUE;

	if( m_timerworking != 1 )//KillTimer�̂��Ƃ��A�L���[�Ɏc����wm_timer�����������Ȃ��悤�ɁB
		return 0;

	int timerid = (int)wParam;
	if( (timerid == AlpKeyDlgTIMER) && (m_cmdshow == SW_SHOW) ){

		//�h���b�O���ɁA�}�E�X���O�ɏo���ꍇ�́A�h���b�O����������I������B
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


int CAlpKeyDlg::ShowDlg( int cmdshow, int stopflag )
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
		//m_dlg->m_current = 0;//!!!!!!!!!!!!!!!!!!!!!! �ʃX���b�h�ł̃Z�b�g���x��邽�߁A�����ŋ������Z�b�g�B
		OnUserSelchange();
	}


	return 0;
}

LRESULT CAlpKeyDlg::OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CAlpKey* alpkptr = m_dlg->m_mhandler->GetFirstAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie );

	CAlpKey* curalpk;
	CAlpKey* beftk;
	curalpk = alpkptr;
	beftk = curalpk;
	while( curalpk && (curalpk->frameno < m_dlg->m_current) ){
		beftk = curalpk;
		curalpk = curalpk->next;
	}	

	if( beftk ){
		m_dlg->m_current = beftk->frameno;
	}else{
		m_dlg->m_current = 0;
	}

///////

	//m_current��m_startframe��菬�����ꍇ�ł��ARANGEMIN�̐ݒ�ɂ��X���C�_�[�͍ŏ��l��菬�����Ȃ�Ȃ��B
	//OnReleasecaptureSl_mp�ŁA�X���C�_�[�̌��݂̏ꏊ����m_current��ݒ肵�Ȃ����̂Ŗ��Ȃ��B

	m_dlg->m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_dlg->m_current );
	BOOL dummy;	
	m_dlg->OnReleasedcaptureSl_mp( 0, 0, dummy );

	InitList();
	ParamsToDlg();
	Redraw();


	return 0;
}
LRESULT CAlpKeyDlg::OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;

	CAlpKey* alpkptr = m_dlg->m_mhandler->GetFirstAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie );

	CAlpKey* curalpk;
	CAlpKey* findtk;

	curalpk = alpkptr;
	findtk = 0;
	while( curalpk ){
		if( curalpk->frameno > m_dlg->m_current ){
			if( (curalpk->frameno >= m_dlg->m_startframe) && (curalpk->frameno <= m_dlg->m_endframe) ){
				findtk = curalpk;
				break;
			}
		}
		curalpk = curalpk->next;
	}

	if( findtk ){
		m_dlg->m_current = findtk->frameno;
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

LRESULT CAlpKeyDlg::OnCreateKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_dlg->m_thandler || !m_dlg->m_shandler || !m_dlg->m_mhandler )
		return 0;
	if( m_dlg->m_selectedno == 0 )
		return 0;
	if( m_dlg->m_preview_flag == 1 )
		return 0;


	int createid;
	if( wNotifyCode == 999 ){
		createid = (int)wID;
	}else{
		createid = -1;
	}


	int ret;
	if( (m_dlg->m_selectedno > 0) && m_dlg->m_mhandler ){
		CAlpKey* curalpk = 0;
		ret = m_dlg->m_mhandler->ExistAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curalpk );
		if( ret ){
			DbgOut( "alpkeydlg : OnCreateKey : mh ExistAlpKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( !curalpk ){
			CAlpKey* newalpk = 0;
			ret = m_dlg->m_mhandler->CreateAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &newalpk );
			if( ret || !newalpk ){
				DbgOut( "alpkeydlg : OnCreateKey : mh CreateAlpKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			curalpk = newalpk;
		}

		CMQOMaterial* curmat;
		curmat = m_dlg->m_shandler->m_mathead;
		while( curmat ){

			if( (createid == -1) || (createid == curmat->materialno) ){

				float alphaval = 1.0f;
				int existflag = 0;
				ret = m_dlg->m_mhandler->GetAlpAnim( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, curmat, &alphaval, &existflag );
				if( ret ){
					DbgOut( "alpkeydlg : InitList : mh GetAlpAnim error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				ret = curalpk->SetAlpChange( curmat, alphaval );
				if( ret ){
					DbgOut( "alpkeydlg : InitList : alpk SetAlphaChange error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}

			curmat = curmat->next;
		}

		m_remakelistflag = 1;
		ret = InitList();
		_ASSERT( !ret );

		Redraw();
	}

	return 0;
}


int CAlpKeyDlg::InitList()
{
	int ret;
	ListView_DeleteAllItems( m_list_wnd );
	
	if( m_createlistflag == 0 ){
		LV_COLUMN lvcol;
		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 155;
		lvcol.pszText = "�}�e���A����";
		lvcol.iSubItem = ALPSUB_MATNAME;
		ListView_InsertColumn(m_list_wnd, ALPSUB_MATNAME, &lvcol);

		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 160;
		lvcol.pszText = "�A���t�@";
		lvcol.iSubItem = ALPSUB_ALPHA;
		ListView_InsertColumn(m_list_wnd, ALPSUB_ALPHA, &lvcol);
	}
	m_createlistflag = 1;


	if( (m_dlg->m_selectedno > 0) && m_dlg->m_shandler && m_dlg->m_mhandler ){

		int setno = 0;
		CMQOMaterial* curmat;
		curmat = m_dlg->m_shandler->m_mathead;
		while( curmat ){
			float alphaval = 1.0f;
			int existflag = 0;
			ret = m_dlg->m_mhandler->GetAlpAnim( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, curmat, &alphaval, &existflag );
			if( ret ){
				DbgOut( "alpkeydlg : InitList : mh GetAlpAnim error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = AddItem2List( setno, curmat, alphaval, existflag );
			if( ret ){
				DbgOut( "alpkeydlg : InitList : AddItem2List error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			setno++;
			curmat = curmat->next;
		}

	}

	ret = m_dlg->SetCurrentMotion();
	_ASSERT( !ret );

	return 0;
}
int CAlpKeyDlg::AddItem2List( int itemno, CMQOMaterial* srcmat, float srcalpha, int existflag )
{
	char tempchar[256];

	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));

	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.pszText = srcmat->name;
	item.iItem = itemno;
	item.iSubItem = ALPSUB_MATNAME;
	item.lParam = srcmat->materialno;
	ListView_InsertItem(m_list_wnd, &item);

	if( existflag != 0 ){
		sprintf_s( tempchar, 256, "*%f", srcalpha );
	}else{
		sprintf_s( tempchar, 256, "%f", srcalpha );
	}

item.mask = LVIF_TEXT;
	item.pszText = tempchar;
	item.iItem = itemno;
	item.iSubItem = ALPSUB_ALPHA;
//	item.lParam = telem->serialno;
	ListView_SetItem(m_list_wnd, &item);

	return 0;
}

int CAlpKeyDlg::OnList1( int hititem, POINT inview )
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
	lvitem2.iSubItem = ALPSUB_MATNAME;//!!!!!!!!!!!!
	lvitem2.pszText = tempname2;
	lvitem2.cchTextMax = 256;

	lvitem2.mask = LVIF_TEXT | LVIF_PARAM;
				
	BOOL bret2;
	bret2 = ListView_GetItem( m_list_wnd, &lvitem2 );
	if( bret2 == TRUE ){

		int curmatno;
		curmatno = (int)lvitem2.lParam;
		CMQOMaterial* curmat;
		curmat = m_dlg->m_shandler->GetMaterialFromNo( m_dlg->m_shandler->m_mathead, curmatno );
		if( !curmat ){
			DbgOut( "alpkeydlg : OnList1 : curmat NULL error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

//DbgOut( "alpkeydlg : OnList1 : curmoatno %d, curmat %x\r\n", curmatno, curmat );
		BOOL dummyb = 0;
		OnCreateKey( 999, curmat->materialno, 0, dummyb );

		CAlpKey* curalpk = 0;
		ret = m_dlg->m_mhandler->ExistAlpKey( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, &curalpk );
		if( ret || !curalpk ){
			DbgOut( "alpkeydlg : OnList1 : mh ExistAlpKey 1 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		float srcval = 1.0f;
		int existflag = 0;
		ret = m_dlg->m_mhandler->GetAlpAnim( m_dlg->m_selectedno, m_dlg->m_mot_cookie, m_dlg->m_current, curmat, &srcval, &existflag );
		if( ret ){
			DbgOut( "alpkeydlg : OnList1 : mh GetAlpAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		CEditAlpDlg dlg( curmat->name, srcval );
		dlg.DoModal();

		float newval = dlg.m_val;

		ret = curalpk->SetAlpChange( curmat, newval );
		if( ret ){
			DbgOut( "alpkeydlg : OnList1 : tk SetAlpChange error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		char stralpha[256];
		sprintf_s( stralpha, 256, "*%f", newval );

		ListView_SetItemText( m_list_wnd, hititem, ALPSUB_ALPHA, stralpha );


//		m_remakelistflag = 1;
//		InitList();
		
		ret = m_dlg->SetCurrentMotion();
		_ASSERT( !ret );

	}

	Redraw();

	return 0;
}

LRESULT CAlpKeyDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CAlpKeyDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}

LRESULT CAlpKeyDlg::OnSelchangeList1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NMLISTVIEW* nmlistview = (NMLISTVIEW*)pnmh;

//	char testmes[256];
//	sprintf_s( testmes, 256, "flag %d, item %d, uNewState %d, uOldState %d, uChanged %d", 
//		m_remakelistflag, nmlistview->iItem, nmlistview->uNewState, nmlistview->uOldState, nmlistview->uChanged );
//	::MessageBox( m_hWnd, testmes, "�ʒm", MB_OK );

	int ret;

	if( (m_remakelistflag == 0) && (nmlistview->uNewState == 3) ){

//		char testmes[256];
//		sprintf_s( testmes, 256, "item %d, subitem %d, uNewState %d, uOldState %d, uChanged %d", 
//			nmlistview->iItem, nmlistview->iSubItem, nmlistview->uNewState, nmlistview->uOldState, nmlistview->uChanged );
//		::MessageBox( m_hWnd, testmes, "�ʒm", MB_OK );


		ret = OnList1( nmlistview->iItem, nmlistview->ptAction );
		_ASSERT( !ret );

	}else if( nmlistview->uNewState == 3 ){
		m_remakelistflag = 0;
	}

	return 0;
}
int CAlpKeyDlg::OnJumpFrame( int srcframe )
{
	m_dlg->OnJumpFrame( srcframe );

	InitList();
	ParamsToDlg();
	Redraw();

	return 0;
}
