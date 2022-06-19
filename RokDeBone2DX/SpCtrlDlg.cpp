// SpCtrlDlg.cpp : CSpCtrlDlg のインプリメンテーション
#include "stdafx.h"
#include "SpCtrlDlg.h"

#include <math.h>
#include <stdio.h>
#include <d3dx9.h>

#define DBGH
#include <dbg.h>

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <mothandler.h>
#include <motionctrl.h>
#include <motioninfo.h>
#include <motionpoint2.h>
#include <SPParam.h>

#include <Commdlg.h>

#include <process.h>
#include <advwin32.h>

#include "GetDlgParams.h"

#include <crtdbg.h>


static char s_strmkind[MKIND_MAX][10] = {
	"回転", "移動", "拡大"
};

static char s_straxiskind[INDMAX][10] = {
	"X成分", "Y成分", "Z成分"
};

/////////////////
extern int g_endapp;

/////////////////////////////////////////////////////////////////////////////
// CSpCtrlDlg

CSpCtrlDlg::CSpCtrlDlg()
{
	m_ismoving = 0;

	m_lpth = 0;
	m_lpsh = 0;
	m_lpmh = 0;
	m_boneseri = 0;
	m_frameno = 0;

	m_cmdshow = SW_HIDE;
	m_createcomboflag = 0;
	m_moving = 0;

	m_axiskind = INDX;
	m_mkind = MKIND_ROT;

	m_curspp = 0;
}


CSpCtrlDlg::~CSpCtrlDlg()
{
}

LRESULT CSpCtrlDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	ret = SetWnd();
	_ASSERT( !ret );

	ret = InitDlg();
	_ASSERT( !ret );

	return 1;  // システムにフォーカスを設定させます
}


int CSpCtrlDlg::InitDlg()
{
	int ret;
	
	ret = SetCombo();
	_ASSERT( !ret );

	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CSpCtrlDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DbgOut( "SpCtrlDlg : OnOK\n" );
	ShowWindow( SW_HIDE );
	return 0;
}

LRESULT CSpCtrlDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowWindow( SW_HIDE );
	return 0;
}
LRESULT CSpCtrlDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	return 0;
}


int CSpCtrlDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_bonename_wnd = GetDlgItem( IDC_BONENAME );
	_ASSERT( m_bonename_wnd );
	m_frameno_wnd = GetDlgItem( IDC_FRAMENO );
	_ASSERT( m_frameno_wnd );
	m_axiskind_wnd = GetDlgItem( IDC_AXISKIND );
	_ASSERT( m_axiskind_wnd );
	m_mkind_wnd = GetDlgItem( IDC_MKIND );
	_ASSERT( m_mkind_wnd );
	m_sync_wnd = GetDlgItem( IDC_SYNC );
	_ASSERT( m_sync_wnd );


	m_slts_wnd = GetDlgItem( IDC_SLTS );
	_ASSERT( m_slts_wnd );
	m_editts_wnd = GetDlgItem( IDC_EDITTS );
	_ASSERT( m_editts_wnd );
	m_editmaxs_wnd = GetDlgItem( IDC_EDITMAXS );
	_ASSERT( m_editmaxs_wnd );
	m_slds_wnd = GetDlgItem( IDC_SLDS );
	_ASSERT( m_slds_wnd );
	m_editds_wnd = GetDlgItem( IDC_EDITDS );
	_ASSERT( m_editds_wnd );
	m_editmints_wnd = GetDlgItem( IDC_EDITMINTS );
	_ASSERT( m_editmints_wnd );
	m_editmaxts_wnd = GetDlgItem( IDC_EDITMAXTS );
	_ASSERT( m_editmaxts_wnd );


	m_slte_wnd = GetDlgItem( IDC_SLTE );
	_ASSERT( m_slte_wnd );
	m_editte_wnd = GetDlgItem( IDC_EDITTE );
	_ASSERT( m_editte_wnd );
	m_editmaxe_wnd = GetDlgItem( IDC_EDITMAXE );
	_ASSERT( m_editmaxe_wnd );
	m_slde_wnd = GetDlgItem( IDC_SLDE );
	_ASSERT( m_slde_wnd );
	m_editde_wnd = GetDlgItem( IDC_EDITDE );
	_ASSERT( m_editde_wnd );
	m_editminte_wnd = GetDlgItem( IDC_EDITMINTE );
	_ASSERT( m_editminte_wnd );
	m_editmaxte_wnd = GetDlgItem( IDC_EDITMAXTE );
	_ASSERT( m_editmaxte_wnd );


	//sliderの初期設定
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)m_startframe );
//	m_sl_mp_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)m_endframe);
//	SetSliderTic( m_sl_mp_wnd, 10, m_startframe, m_endframe );//

	m_slts_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_slts_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)100 );
	SetSliderTic( m_slts_wnd, 10, 0, 100 );

	m_slds_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)-100 );
	m_slds_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)100 );
	SetSliderTic( m_slds_wnd, 10, -100, 100 );


	m_slte_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_slte_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)100 );
	SetSliderTic( m_slte_wnd, 10, 0, 100 );

	m_slde_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)-100 );
	m_slde_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)100 );
	SetSliderTic( m_slde_wnd, 10, -100, 100 );

	return 0;
}

int CSpCtrlDlg::ParamsToDlg()
{
	if( g_endapp == 1 ){
		return 0;
	}
	if( m_cmdshow != SW_SHOW ){
		return 0;
	}
	if( !IsWindow() )
		return 0;
	if( !m_curspp || !m_lpth || !m_lpmh || (m_boneseri <= 0) ){
		return 0;
	}
	if( m_lpmh->m_kindnum <= 0 ){
		return 0;
	}

	char tempchar[1024];
	CTreeElem2* telem;
	telem = (*m_lpth)( m_boneseri );
	_ASSERT( telem );
	sprintf_s( tempchar, 1024, "ボーン名：%s", telem->name );
	m_bonename_wnd.SetWindowText( tempchar );

	
	sprintf_s( tempchar, 1024, "FrameNo：%d", m_frameno );
	m_frameno_wnd.SetWindowText( tempchar );

	m_axiskind_wnd.SendMessage( CB_SETCURSEL, m_axiskind, 0 );

	sprintf_s( tempchar, 1024, "種類：%s", s_strmkind[m_mkind] );
	m_mkind_wnd.SetWindowText( tempchar );

//////

	int tmin = (int)( (m_curspp->ctrlS[m_axiskind].tmin + 0.001f) * 100.0f );
	m_slts_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)tmin );
	int tmax = (int)( (m_curspp->ctrlS[m_axiskind].tmax + 0.001f) * 100.0f );
	m_slts_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)tmax );
	int tvals = (int)( m_curspp->ctrlS[m_axiskind].spt * 100.0f );
	m_slts_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)tvals );
	SetSliderTic( m_slts_wnd, 10, tmin, tmax );//

	int dmaxs = (int)( m_curspp->ctrlS[m_axiskind].distmax * 100.0f );
	m_slds_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)-dmaxs );
	m_slds_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)dmaxs );
	int dvals = (int)( m_curspp->ctrlS[m_axiskind].spdist * 100.0f );
	m_slds_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)dvals );
	int dsstep;
	if( dmaxs / 1000 > 0 ){
		dsstep = 1000;
	}else if( dmaxs / 100 > 0 ){
		dsstep = 100;
	}else{
		dsstep = 10;
	}
	SetSliderTic( m_slds_wnd, dsstep, -dmaxs, dmaxs );//

	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlS[m_axiskind].spt );
	m_editts_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlS[m_axiskind].spdist );
	m_editds_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlS[m_axiskind].distmax );
	m_editmaxs_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlS[m_axiskind].tmin );
	m_editmints_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlS[m_axiskind].tmax );
	m_editmaxts_wnd.SetWindowText( tempchar );
	
///////
	int tmine = (int)( (m_curspp->ctrlE[m_axiskind].tmin + 0.001f) * 100.0f );
	m_slte_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)tmine );
	int tmaxe = (int)( (m_curspp->ctrlE[m_axiskind].tmax + 0.001f) * 100.0f );
	m_slte_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)tmaxe );
	int tvale = (int)( m_curspp->ctrlE[m_axiskind].spt * 100.0f );
	m_slte_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)tvale );
	SetSliderTic( m_slte_wnd, 10, tmine, tmaxe );//

	int dvale = (int)( m_curspp->ctrlE[m_axiskind].spdist * 100.0f );
	m_slde_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)dvale );
	int dmaxe = (int)( m_curspp->ctrlE[m_axiskind].distmax * 100.0f );
	m_slde_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)-dmaxe );
	m_slde_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)dmaxe );
	int destep;
	if( dmaxe / 1000 > 0 ){
		destep = 1000;
	}else if( dmaxe / 100 > 0 ){
		destep = 100;
	}else{
		destep = 10;
	}
	SetSliderTic( m_slde_wnd, destep, -dmaxe, dmaxe );//

	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlE[m_axiskind].spt );
	m_editte_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlE[m_axiskind].spdist );
	m_editde_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlE[m_axiskind].distmax );
	m_editmaxe_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlE[m_axiskind].tmin );
	m_editminte_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 1024, "%.2f", m_curspp->ctrlE[m_axiskind].tmax );
	m_editmaxte_wnd.SetWindowText( tempchar );
	
//////////
	if( m_curspp->syncflag != 0 ){
		m_dlg_wnd.CheckDlgButton( IDC_SYNC, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_SYNC, BST_UNCHECKED );
	}
///////////
	if( m_curspp->lockflag != 0 ){
		m_dlg_wnd.CheckDlgButton( IDC_LOCK, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_LOCK, BST_UNCHECKED );
	}



	return 0;
}

LRESULT CSpCtrlDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND ctrlwnd;
	ctrlwnd = (HWND)lParam;
	if( ctrlwnd == m_slts_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_TS( 0, 0, dummy );
	}else if( ctrlwnd == m_slds_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_DS( 0, 0, dummy );
	}else if( ctrlwnd == m_slte_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_TE( 0, 0, dummy );
	}else if( ctrlwnd == m_slde_wnd.m_hWnd ){
		BOOL dummy;
		OnReleasedcaptureSl_DE( 0, 0, dummy );
	}

	return 0;
}


LRESULT CSpCtrlDlg::OnReleasedcaptureSl_TS(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int curpos;
	curpos = (int)m_slts_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	
	int cmppos;
	cmppos = (int)m_slte_wnd.SendMessage( TBM_GETPOS, 0, 0 );

	if( curpos <= cmppos ){
		m_curspp->ctrlS[m_axiskind].spt = (float)curpos / 100.0f;
	}else{
		::MessageBox( m_hWnd, "後半制御点の横の位置より大きくは出来ません。", "警告", MB_OK );
		m_slts_wnd.SendMessageA( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)cmppos );
		m_curspp->ctrlS[m_axiskind].spt = (float)cmppos / 100.0f;
	}

	if( m_curspp->syncflag != 0 ){
		if( m_axiskind == INDX ){
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDX].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDX].spt;

			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDX].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDX].spt;

			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDX].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDX].spdist;

			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDX].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDX].spdist;

			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDX].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDX].distmax;

			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDX].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDX].distmax;

			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDX].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDX].tmax;

			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDX].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDX].tmax;

		}else if( m_axiskind == INDY ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDY].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDY].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDY].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDY].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDY].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDY].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDY].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDY].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDY].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDY].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDY].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDY].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDY].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDY].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDY].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDY].tmax;

		}else if( m_axiskind == INDZ ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDZ].spt;
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDZ].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDZ].spt;
			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDZ].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDZ].spdist;
			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDZ].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDZ].spdist;
			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDZ].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDZ].distmax;
			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDZ].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDZ].distmax;
			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDZ].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDZ].tmax;
			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDZ].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDZ].tmax;
			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDZ].tmax;
		}

	}


	int ret;
	ret = m_lpmh->FillUpMotion( m_lpsh, m_lpmh->m_curmotkind, m_boneseri, 0, -1, 0 );
	if( ret ){
		DbgOut( "spctrldlg : OnReleasedcaptureSl_TS : mh FillUpMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//	ret = m_lpmh->CalcMPEuler( m_lpsh, m_lpmh->m_curmotkind, m_lpmh->m_zakind );
//	_ASSERT( !ret );
	


	ParamsToDlg();

	return 0;
}
LRESULT CSpCtrlDlg::OnReleasedcaptureSl_TE(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int curpos;
	curpos = (int)m_slte_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	
	int cmppos;
	cmppos = (int)m_slts_wnd.SendMessage( TBM_GETPOS, 0, 0 );

	if( curpos >= cmppos ){
		m_curspp->ctrlE[m_axiskind].spt = (float)curpos / 100.0f;
	}else{
		::MessageBox( m_hWnd, "前半制御点の横の位置より小さくは出来ません。", "警告", MB_OK );
		m_slte_wnd.SendMessageA( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)cmppos );
		m_curspp->ctrlE[m_axiskind].spt = (float)cmppos / 100.0f;
	}

	if( m_curspp->syncflag != 0 ){
		if( m_axiskind == INDX ){
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDX].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDX].spt;

			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDX].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDX].spt;

			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDX].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDX].spdist;

			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDX].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDX].spdist;

			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDX].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDX].distmax;

			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDX].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDX].distmax;

			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDX].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDX].tmax;

			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDX].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDX].tmax;

		}else if( m_axiskind == INDY ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDY].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDY].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDY].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDY].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDY].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDY].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDY].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDY].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDY].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDY].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDY].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDY].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDY].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDY].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDY].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDY].tmax;

		}else if( m_axiskind == INDZ ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDZ].spt;
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDZ].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDZ].spt;
			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDZ].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDZ].spdist;
			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDZ].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDZ].spdist;
			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDZ].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDZ].distmax;
			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDZ].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDZ].distmax;
			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDZ].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDZ].tmax;
			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDZ].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDZ].tmax;
			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDZ].tmax;
		}
	}


	int ret;
	ret = m_lpmh->FillUpMotion( m_lpsh, m_lpmh->m_curmotkind, m_boneseri, 0, -1, 0 );
	if( ret ){
		DbgOut( "spctrldlg : OnReleasedcaptureSl_TE : mh FillUpMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//	ret = m_lpmh->CalcMPEuler( m_lpsh, m_lpmh->m_curmotkind, m_lpmh->m_zakind );
//	_ASSERT( !ret );

	ParamsToDlg();

	return 0;
}
LRESULT CSpCtrlDlg::OnReleasedcaptureSl_DS(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int curpos;
	curpos = (int)m_slds_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	
	m_curspp->ctrlS[m_axiskind].spdist = (float)curpos / 100.0f;

	if( m_curspp->syncflag != 0 ){
		if( m_axiskind == INDX ){
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDX].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDX].spt;

			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDX].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDX].spt;

			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDX].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDX].spdist;

			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDX].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDX].spdist;

			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDX].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDX].distmax;

			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDX].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDX].distmax;

			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDX].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDX].tmax;

			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDX].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDX].tmax;

		}else if( m_axiskind == INDY ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDY].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDY].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDY].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDY].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDY].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDY].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDY].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDY].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDY].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDY].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDY].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDY].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDY].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDY].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDY].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDY].tmax;

		}else if( m_axiskind == INDZ ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDZ].spt;
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDZ].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDZ].spt;
			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDZ].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDZ].spdist;
			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDZ].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDZ].spdist;
			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDZ].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDZ].distmax;
			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDZ].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDZ].distmax;
			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDZ].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDZ].tmax;
			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDZ].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDZ].tmax;
			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDZ].tmax;
		}
	}


	int ret;
	ret = m_lpmh->FillUpMotion( m_lpsh, m_lpmh->m_curmotkind, m_boneseri, 0, -1, 0 );
	if( ret ){
		DbgOut( "spctrldlg : OnReleasedcaptureSl_DS : mh FillUpMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//	ret = m_lpmh->CalcMPEuler( m_lpsh, m_lpmh->m_curmotkind, m_lpmh->m_zakind );
//	_ASSERT( !ret );

	ParamsToDlg();

	return 0;
}
LRESULT CSpCtrlDlg::OnReleasedcaptureSl_DE(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int curpos;
	curpos = (int)m_slde_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	
	m_curspp->ctrlE[m_axiskind].spdist = (float)curpos / 100.0f;

	if( m_curspp->syncflag != 0 ){
		if( m_axiskind == INDX ){
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDX].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDX].spt;

			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDX].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDX].spt;

			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDX].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDX].spdist;

			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDX].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDX].spdist;

			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDX].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDX].distmax;

			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDX].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDX].distmax;

			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDX].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDX].tmax;

			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDX].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDX].tmax;

		}else if( m_axiskind == INDY ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDY].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDY].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDY].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDY].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDY].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDY].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDY].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDY].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDY].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDY].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDY].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDY].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDY].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDY].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDY].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDY].tmax;

		}else if( m_axiskind == INDZ ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDZ].spt;
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDZ].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDZ].spt;
			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDZ].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDZ].spdist;
			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDZ].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDZ].spdist;
			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDZ].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDZ].distmax;
			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDZ].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDZ].distmax;
			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDZ].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDZ].tmax;
			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDZ].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDZ].tmax;
			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDZ].tmax;
		}
	}


	int ret;
	ret = m_lpmh->FillUpMotion( m_lpsh, m_lpmh->m_curmotkind, m_boneseri, 0, -1, 0 );
	if( ret ){
		DbgOut( "spctrldlg : OnReleasedcaptureSl_DE : mh FillUpMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//	ret = m_lpmh->CalcMPEuler( m_lpsh, m_lpmh->m_curmotkind, m_lpmh->m_zakind );
//	_ASSERT( !ret );


	ParamsToDlg();

	return 0;
}

void CSpCtrlDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
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


LRESULT CSpCtrlDlg::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	DefWindowProc();

//	Redraw();

	return 0;
}
LRESULT CSpCtrlDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
//DbgOut( "SpCtrlDlg : OnMoving\n" );
	m_ismoving = 1;
	return 0;
}

LRESULT CSpCtrlDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_ismoving = 0;
	DefWindowProc();

//	Redraw();

	return 0;
}


int CSpCtrlDlg::ShowDlg( int cmdshow, CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh,
	int boneseri, int frameno, int mkind, SPPARAM* spp )
{
	if( !IsWindow() ){
		return 0;
	}

	m_cmdshow = cmdshow;

	if( (m_cmdshow == SW_HIDE) ){
		m_moving = 0;
	}

	if( !IsWindow() ){
		return 0;
	}
	ShowWindow( m_cmdshow );

	if( m_cmdshow != SW_HIDE ){
		if( !IsWindow() ){
			return 0;
		}
		m_lpth = lpth;
		m_lpsh = lpsh;
		m_lpmh = lpmh;
		m_boneseri = boneseri;
		m_frameno = frameno;
		m_mkind = mkind;
		m_curspp = spp;

		ParamsToDlg();
	}

	return 0;
}



//LRESULT CSpCtrlDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	m_moving = 1;
//
//	return 0;
//}
//LRESULT CSpCtrlDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	m_moving = 0;
//
//	return 0;
//}

int CSpCtrlDlg::SetCombo()
{

	m_axiskind_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );

	int ikind;
	for( ikind = INDX; ikind <= INDZ; ikind++ ){
		m_axiskind_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)&(s_straxiskind[ikind][0]) );
	}

	m_axiskind_wnd.SendMessage( CB_SETCURSEL, m_axiskind, 0 );


	return 0;
}

LRESULT CSpCtrlDlg::OnLock(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_LOCK );
	if( checkflag == BST_CHECKED ){
		m_curspp->lockflag = 1;
	}else{
		m_curspp->lockflag = 0;
	}

	return 0;
}

LRESULT CSpCtrlDlg::OnSync(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_SYNC );
	if( checkflag == BST_CHECKED ){
		m_curspp->syncflag = 1;

		if( m_axiskind == INDX ){
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDX].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDX].spt;

			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDX].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDX].spt;

			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDX].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDX].spdist;

			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDX].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDX].spdist;

			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDX].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDX].distmax;

			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDX].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDX].distmax;

			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDX].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDX].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDX].tmax;

			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDX].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDX].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDX].tmax;

		}else if( m_axiskind == INDY ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDY].spt;
			m_curspp->ctrlS[INDZ].spt = m_curspp->ctrlS[INDY].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDY].spt;
			m_curspp->ctrlE[INDZ].spt = m_curspp->ctrlE[INDY].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDY].spdist;
			m_curspp->ctrlS[INDZ].spdist = m_curspp->ctrlS[INDY].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDY].spdist;
			m_curspp->ctrlE[INDZ].spdist = m_curspp->ctrlE[INDY].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDY].distmax;
			m_curspp->ctrlS[INDZ].distmax = m_curspp->ctrlS[INDY].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDY].distmax;
			m_curspp->ctrlE[INDZ].distmax = m_curspp->ctrlE[INDY].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDY].tmax;
			m_curspp->ctrlS[INDZ].tmin = m_curspp->ctrlS[INDY].tmin;
			m_curspp->ctrlS[INDZ].tmax = m_curspp->ctrlS[INDY].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDY].tmax;
			m_curspp->ctrlE[INDZ].tmin = m_curspp->ctrlE[INDY].tmin;
			m_curspp->ctrlE[INDZ].tmax = m_curspp->ctrlE[INDY].tmax;

		}else if( m_axiskind == INDZ ){
			m_curspp->ctrlS[INDX].spt = m_curspp->ctrlS[INDZ].spt;
			m_curspp->ctrlS[INDY].spt = m_curspp->ctrlS[INDZ].spt;

			m_curspp->ctrlE[INDX].spt = m_curspp->ctrlE[INDZ].spt;
			m_curspp->ctrlE[INDY].spt = m_curspp->ctrlE[INDZ].spt;

			m_curspp->ctrlS[INDX].spdist = m_curspp->ctrlS[INDZ].spdist;
			m_curspp->ctrlS[INDY].spdist = m_curspp->ctrlS[INDZ].spdist;

			m_curspp->ctrlE[INDX].spdist = m_curspp->ctrlE[INDZ].spdist;
			m_curspp->ctrlE[INDY].spdist = m_curspp->ctrlE[INDZ].spdist;

			m_curspp->ctrlS[INDX].distmax = m_curspp->ctrlS[INDZ].distmax;
			m_curspp->ctrlS[INDY].distmax = m_curspp->ctrlS[INDZ].distmax;

			m_curspp->ctrlE[INDX].distmax = m_curspp->ctrlE[INDZ].distmax;
			m_curspp->ctrlE[INDY].distmax = m_curspp->ctrlE[INDZ].distmax;

			m_curspp->ctrlS[INDX].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDX].tmax = m_curspp->ctrlS[INDZ].tmax;
			m_curspp->ctrlS[INDY].tmin = m_curspp->ctrlS[INDZ].tmin;
			m_curspp->ctrlS[INDY].tmax = m_curspp->ctrlS[INDZ].tmax;

			m_curspp->ctrlE[INDX].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDX].tmax = m_curspp->ctrlE[INDZ].tmax;
			m_curspp->ctrlE[INDY].tmin = m_curspp->ctrlE[INDZ].tmin;
			m_curspp->ctrlE[INDY].tmax = m_curspp->ctrlE[INDZ].tmax;
		}
	}else if( checkflag == BST_UNCHECKED ){
		m_curspp->syncflag = 0;
	}else{
		_ASSERT( 0 );
		m_curspp->syncflag = 0;
	}

	ParamsToDlg();

	return 0;
}
LRESULT CSpCtrlDlg::OnSelchangeAxis(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_axiskind_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 1;
	}
	
	m_axiskind = combono;

	ParamsToDlg();

	return 0;
}
LRESULT CSpCtrlDlg::OnApplyTS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editts_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "前半の横の数値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	float cmpval;
	cmpval = m_curspp->ctrlE[m_axiskind].spt;

	if( tmpfloat > cmpval ){
		::MessageBox( m_hWnd, "前半制御点の横の数値は後半制御点の横の数値よりも小さくなくてはいけません。", "警告", MB_OK );
		tmpfloat = cmpval;
	}

	m_curspp->ctrlS[m_axiskind].spt = tmpfloat;

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_TS( 0, 0, dummy );


	return 0;
}
LRESULT CSpCtrlDlg::OnApplyMAXS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editmaxs_wnd, &tmpfloat );
	if( ret || (tmpfloat < 0.01f) || (tmpfloat > 10000.0f) ){
		::MessageBox( m_hWnd, "前半のmax値には0.01より大きく10000より小さい値を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_curspp->ctrlS[m_axiskind].distmax = tmpfloat;
	if( m_curspp->ctrlS[m_axiskind].spdist > tmpfloat ){
		m_curspp->ctrlS[m_axiskind].spdist = tmpfloat;
	}
	if( m_curspp->ctrlS[m_axiskind].spdist < -tmpfloat ){
		m_curspp->ctrlS[m_axiskind].spdist = -tmpfloat;
	}

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_DS( 0, 0, dummy );

	return 0;
}
LRESULT CSpCtrlDlg::OnApplyMINTS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float cmpval = m_curspp->ctrlS[m_axiskind].tmax;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editmints_wnd, &tmpfloat );
	if( ret || (tmpfloat < -100.0f) || (tmpfloat >= cmpval) ){
		::MessageBox( m_hWnd, "前半のmin値にはmax値より小さい値を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_curspp->ctrlS[m_axiskind].tmin = tmpfloat;
	if( m_curspp->ctrlS[m_axiskind].spt < tmpfloat ){
		m_curspp->ctrlS[m_axiskind].spt = tmpfloat;
	}
	if( m_curspp->ctrlS[m_axiskind].spt > cmpval ){
		m_curspp->ctrlS[m_axiskind].spt = cmpval;
	}

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_TS( 0, 0, dummy );

	return 0;

	return 0;
}
LRESULT CSpCtrlDlg::OnApplyMAXTS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float cmpval = m_curspp->ctrlS[m_axiskind].tmin;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editmaxts_wnd, &tmpfloat );
	if( ret || (tmpfloat > 100.0f) || (tmpfloat <= cmpval) ){
		::MessageBox( m_hWnd, "前半のmax値にはmin値より大きい値を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_curspp->ctrlS[m_axiskind].tmax = tmpfloat;
	if( m_curspp->ctrlS[m_axiskind].spt > tmpfloat ){
		m_curspp->ctrlS[m_axiskind].spt = tmpfloat;
	}
	if( m_curspp->ctrlS[m_axiskind].spt < cmpval ){
		m_curspp->ctrlS[m_axiskind].spt = cmpval;
	}

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_TS( 0, 0, dummy );

	return 0;
}


LRESULT CSpCtrlDlg::OnApplyDS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editds_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "前半制御点の縦値は小数を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	if( tmpfloat > m_curspp->ctrlS[m_axiskind].distmax ){
		m_curspp->ctrlS[m_axiskind].distmax = tmpfloat;
	}
	if( tmpfloat < -m_curspp->ctrlS[m_axiskind].distmax ){
		m_curspp->ctrlS[m_axiskind].distmax = tmpfloat;
	}

	m_curspp->ctrlS[m_axiskind].spdist = tmpfloat;

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_DS( 0, 0, dummy );

	return 0;
}
LRESULT CSpCtrlDlg::OnZeroS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_curspp->ctrlS[m_axiskind].spdist = 0.0f;

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_DS( 0, 0, dummy );

	return 0;
}

LRESULT CSpCtrlDlg::OnApplyTE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editte_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "後半の横の数値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	float cmpval;
	cmpval = m_curspp->ctrlS[m_axiskind].spt;

	if( tmpfloat < cmpval ){
		::MessageBox( m_hWnd, "後半制御点の横の数値は前半制御点の横の数値よりも大きくなくてはいけません。", "警告", MB_OK );
		tmpfloat = cmpval;
	}

	m_curspp->ctrlE[m_axiskind].spt = tmpfloat;

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_TE( 0, 0, dummy );


	return 0;
}
LRESULT CSpCtrlDlg::OnApplyMAXE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editmaxe_wnd, &tmpfloat );
	if( ret || (tmpfloat < 0.01f) || (tmpfloat > 10000.0f) ){
		::MessageBox( m_hWnd, "後半max値には0.01より大きく10000より小さい値を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_curspp->ctrlE[m_axiskind].distmax = tmpfloat;
	if( m_curspp->ctrlE[m_axiskind].spdist > tmpfloat ){
		m_curspp->ctrlE[m_axiskind].spdist = tmpfloat;
	}
	if( m_curspp->ctrlE[m_axiskind].spdist < -tmpfloat ){
		m_curspp->ctrlE[m_axiskind].spdist = -tmpfloat;
	}

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_DE( 0, 0, dummy );

	return 0;
}
LRESULT CSpCtrlDlg::OnApplyMINTE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float cmpval = m_curspp->ctrlE[m_axiskind].tmax;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editminte_wnd, &tmpfloat );
	if( ret || (tmpfloat < -100.0f) || (tmpfloat >= cmpval) ){
		::MessageBox( m_hWnd, "後半のmin値にはmax値より小さい値を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_curspp->ctrlE[m_axiskind].tmin = tmpfloat;
	if( m_curspp->ctrlE[m_axiskind].spt < tmpfloat ){
		m_curspp->ctrlE[m_axiskind].spt = tmpfloat;
	}
	if( m_curspp->ctrlE[m_axiskind].spt > cmpval ){
		m_curspp->ctrlE[m_axiskind].spt = cmpval;
	}

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_TE( 0, 0, dummy );

	return 0;

	return 0;
}
LRESULT CSpCtrlDlg::OnApplyMAXTE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float cmpval = m_curspp->ctrlE[m_axiskind].tmin;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editmaxte_wnd, &tmpfloat );
	if( ret || (tmpfloat > 100.0f) || (tmpfloat <= cmpval) ){
		::MessageBox( m_hWnd, "後半のmax値にはmin値より大きい値を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_curspp->ctrlE[m_axiskind].tmax = tmpfloat;
	if( m_curspp->ctrlE[m_axiskind].spt > tmpfloat ){
		m_curspp->ctrlE[m_axiskind].spt = tmpfloat;
	}
	if( m_curspp->ctrlE[m_axiskind].spt < cmpval ){
		m_curspp->ctrlE[m_axiskind].spt = cmpval;
	}

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_TE( 0, 0, dummy );

	return 0;
}


LRESULT CSpCtrlDlg::OnApplyDE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat = 0.0f;
	ret = GetFloat( m_editde_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "後半制御点の縦値は小数を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	if( tmpfloat > m_curspp->ctrlE[m_axiskind].distmax ){
		m_curspp->ctrlE[m_axiskind].distmax = tmpfloat;
	}
	if( tmpfloat < -m_curspp->ctrlE[m_axiskind].distmax ){
		m_curspp->ctrlE[m_axiskind].distmax = tmpfloat;
	}

	m_curspp->ctrlE[m_axiskind].spdist = tmpfloat;

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_DE( 0, 0, dummy );

	return 0;
}
LRESULT CSpCtrlDlg::OnZeroE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_curspp->ctrlE[m_axiskind].spdist = 0.0f;

	ParamsToDlg();

	BOOL dummy;
	OnReleasedcaptureSl_DE( 0, 0, dummy );

	return 0;
}
