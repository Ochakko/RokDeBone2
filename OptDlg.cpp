// OptDlg.cpp : COptDlg のインプリメンテーション
#include "stdafx.h"
#include "OptDlg.h"
#include "GetDlgParams.h"

#include "resid.h"

#define DBGH
#include <dbg.h>

#include "ColDlg.h"
extern CColDlg g_coldlg;

#include "MouseOptDlg.h"
#include "SetChkDlg.h"


static int s_motstep[4] = {
	1, 2, 3, 4
};
static char s_motstepstr[4][10] = {
	"1",
	"2",
	"3",
	"4"
};

/////////////////////////////////////////////////////////////////////////////
// COptDlg
COptDlg::COptDlg()
{
	m_bonedisp = 1;
	m_polydisp = 1;
	m_grounddisp = 1;
	m_quatype = QUATYPE_NAME;

	m_bmdisp = 1;

//	m_previewfps = s_fps[0];
	m_motstep = s_motstep[0];

	m_ikfk = 0;
	m_iktdisp = 1;

	m_UnselLine = RGB( 229, 127, 178 );
	m_SelLine = RGB( 29, 229, 215 );
	m_UnselPoint = RGB( 229, 127, 178 );
	m_SelPoint = RGB( 29, 229, 215 );
	m_PointSize = 5;

	m_undodlg = CHKDLG_NES;
	m_rdlg = CHKDLG_NES;
	m_infdlg = CHKDLG_NES;
	m_modlg = CHKDLG_NES;

	m_usekinect = 1;
}

COptDlg::~COptDlg()
{
}


LRESULT COptDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND temphwnd;

	m_dlg_wnd = m_hWnd;

	temphwnd = GetDlgItem( IDC_RADIO_BONE1 );
	m_bone_disp_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RADIO_BONE2 );
	m_bone_nodisp_wnd = temphwnd;

	m_bm_on_wnd = GetDlgItem( IDC_R_BM_ON );
	m_bm_off_wnd = GetDlgItem( IDC_R_BM_OFF );

//////////
/***
	m_combo_fps_wnd = GetDlgItem( IDC_COMBO_FPS );
	int combono;
	for( combono = 0; combono < 11; combono++ ){
		m_combo_fps_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)s_fpsstr[combono] );
	}

	int comboindex = 0;
	switch( m_previewfps ){
	case 60:
		comboindex = 0;
		break;
	case 55:
		comboindex = 1;
		break;
	case 50:
		comboindex = 2;
		break;
	case 45:
		comboindex = 3;
		break;
	case 40:
		comboindex = 4;
		break;
	case 35:
		comboindex = 5;
		break;
	case 30:
		comboindex = 6;
		break;
	case 15:
		comboindex = 7;
		break;
	case 10:
		comboindex = 8;
		break;
	case 6:
		comboindex = 9;
		break;
	case 3:
		comboindex = 10;
		break;
	default:
		m_previewfps = 60;//!!!
		comboindex = 0;
		break;
	}
	m_combo_fps_wnd.SendMessage( CB_SETCURSEL, comboindex, 0 );
***/
////////////
	int combono, comboindex;
	m_combo_motstep_wnd = GetDlgItem( IDC_COMBO_MOTSTEP );
	for( combono = 0; combono < 4; combono++ ){
		m_combo_motstep_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)s_motstepstr[combono] );
	}


	comboindex = m_motstep - 1;
	if( comboindex >= 4 ){
		m_motstep = 1;
		comboindex = 0;
	}
	m_combo_motstep_wnd.SendMessage( CB_SETCURSEL, comboindex, 0 );

//////////////

	if( m_bonedisp ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_BONE1, IDC_RADIO_BONE2, IDC_RADIO_BONE1 );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_BONE1, IDC_RADIO_BONE2, IDC_RADIO_BONE2 );
	}

	if( m_bmdisp ){
		m_dlg_wnd.CheckRadioButton( IDC_R_BM_ON, IDC_R_BM_OFF, IDC_R_BM_ON );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_R_BM_ON, IDC_R_BM_OFF, IDC_R_BM_OFF );
	}

	if( m_iktdisp ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_IKT_ON, IDC_RADIO_IKT_OFF, IDC_RADIO_IKT_ON );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_IKT_ON, IDC_RADIO_IKT_OFF, IDC_RADIO_IKT_OFF );
	}


	temphwnd = GetDlgItem( IDC_RADIO_POLY1 );
	m_poly_alldisp_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RADIO_POLY2 );
	m_poly_seldisp_wnd = temphwnd;

	if( m_polydisp ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_POLY1, IDC_RADIO_POLY2, IDC_RADIO_POLY1 );		
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_POLY1, IDC_RADIO_POLY2, IDC_RADIO_POLY2 );		
	}

	temphwnd = GetDlgItem( IDC_RADIO_GROUND1 );
	m_ground_nodisp_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RADIO_GROUND2 );
	m_ground_disp_wnd = temphwnd;

	if( m_grounddisp ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_GROUND1, IDC_RADIO_GROUND2, IDC_RADIO_GROUND1 );		
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_GROUND1, IDC_RADIO_GROUND2, IDC_RADIO_GROUND2 );		
	}

	if( m_quatype == QUATYPE_NUM ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NUMQUA, IDC_RADIO_NAMEQUA, IDC_RADIO_NUMQUA );		
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NUMQUA, IDC_RADIO_NAMEQUA, IDC_RADIO_NAMEQUA );		
	}


	m_sl_size_wnd = GetDlgItem( IDC_SL_SIZE );
	m_sl_size_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)5 );
	m_sl_size_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)100 );
	m_sl_size_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_jointsize );
	SetSliderTic( m_sl_size_wnd, 5, 5, 100 );

	m_sl_alpha_wnd = GetDlgItem( IDC_SL_ALPHA );
	m_sl_alpha_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_alpha_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)255 );
	m_sl_alpha_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_jointalpha );
	SetSliderTic( m_sl_alpha_wnd, 10, 0, 255 );

	char tempchar[512];

	m_text_size_wnd = GetDlgItem( IDC_TEXT_SIZE );
	sprintf_s( tempchar, 512, "%d", m_jointsize );
	m_text_size_wnd.SetWindowText( tempchar );

	m_text_alpha_wnd = GetDlgItem( IDC_TEXT_ALPHA );
	sprintf_s( tempchar, 512, "%d", m_jointalpha );
	m_text_alpha_wnd.SetWindowText( tempchar );


	if( m_ikfk == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_FK, IDC_RADIO_IK, IDC_RADIO_IK );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_FK, IDC_RADIO_IK, IDC_RADIO_FK );
	}


//////////
	m_unselline_wnd = GetDlgItem( IDC_UNSELCOL_LINE );
	m_selline_wnd = GetDlgItem( IDC_SELCOL_LINE );

	m_unselpoint_wnd = GetDlgItem( IDC_UNSELCOL_POINT );
	m_selpoint_wnd = GetDlgItem( IDC_SELCOL_POINT );


	m_pointsize_wnd = GetDlgItem( IDC_POINTSIZE );
	sprintf_s( tempchar, 512, "%d", m_PointSize );
	m_pointsize_wnd.SetWindowText( tempchar );


	if( m_usekinect ){
		::CheckDlgButton( m_dlg_wnd, IDC_CHKKINECT, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_CHKKINECT, BST_UNCHECKED );
	}

	return 1;  // システムにフォーカスを設定させます
}

LRESULT COptDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_BONE1 );
	if( ischecked == BST_CHECKED ){
		m_bonedisp = 1;
	}else{
		m_bonedisp = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_R_BM_ON );
	if( ischecked == BST_CHECKED ){
		m_bmdisp = 1;
	}else{
		m_bmdisp = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_IKT_ON );
	if( ischecked == BST_CHECKED ){
		m_iktdisp = 1;
	}else{
		m_iktdisp = 0;
	}


//DbgOut( "optdlg : OnOK : m_bonedisp %d, m_bmdisp %d\r\n", m_bonedisp, m_bmdisp );

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_POLY1 );
	if( ischecked == BST_CHECKED ){
		m_polydisp = 1;
	}else{
		m_polydisp = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_GROUND1 );
	if( ischecked == BST_CHECKED ){
		m_grounddisp = 1;
	}else{
		m_grounddisp = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_NAMEQUA );
	if( ischecked == BST_CHECKED ){
		m_quatype = QUATYPE_NAME;
	}else{
		m_quatype = QUATYPE_NUM;
	}

//	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_FPS60 );
//	if( ischecked == BST_CHECKED ){
//		m_previewfps = 60;
//	}else{
//		m_previewfps = 30;
//	}

//	int combono;
//	combono = (int)m_combo_fps_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
//	if( combono == CB_ERR )
//		return 0;
//	m_previewfps = s_fps[combono];

	int combono;
	combono = (int)m_combo_motstep_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	m_motstep = s_motstep[combono];

///////
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_IK );
	if( ischecked == BST_CHECKED ){
		m_ikfk = 0;
	}else{
		m_ikfk = 1;
	}

///////
	int ret;
	int tempint;
	ret = GetInt( m_pointsize_wnd, &tempint );
	if( ret || (tempint < 0) ){
		::MessageBox( m_hWnd, "点の大きさが不正です。0以上の整数。", "入力エラー", MB_OK );
		return 0;
	}
	m_PointSize = tempint;


	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKKINECT );
	if( ischecked == BST_CHECKED ){
		m_usekinect = 1;
	}else{
		m_usekinect = 0;
	}


	EndDialog(wID);
	return 0;
}

LRESULT COptDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}


LRESULT COptDlg::OnReleasedcaptureSl_size(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	int curpos;
	//int ret;
	curpos = (int)m_sl_size_wnd.SendMessage( TBM_GETPOS, 0, 0 );

	m_jointsize = curpos;

	char tempchar[512];
	sprintf_s( tempchar, 512, "%d", m_jointsize );
	m_text_size_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT COptDlg::OnReleasedcaptureSl_alpha(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	int curpos;
	//int ret;
	curpos = (int)m_sl_alpha_wnd.SendMessage( TBM_GETPOS, 0, 0 );

	m_jointalpha = curpos;

	char tempchar[512];
	sprintf_s( tempchar, 512, "%d", m_jointalpha );
	m_text_alpha_wnd.SetWindowText( tempchar );

	return 0;
}

void COptDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	// !!! minticとmaxticには、自動的にticが入るので設定はしない。(してはいけない)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		dstwnd.SendMessage( TBM_SETTIC, 0, (LPARAM)(LONG)curtic );
		curtic += ticfreq;
	}
}


LRESULT COptDlg::OnRefUnselcolLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_hWnd, &m_UnselLine );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}
LRESULT COptDlg::OnRefSelcolLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_hWnd, &m_SelLine );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}
LRESULT COptDlg::OnRefUnselcolPoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_hWnd, &m_UnselPoint );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}
LRESULT COptDlg::OnRefSelcolPoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_hWnd, &m_SelPoint );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}

LRESULT COptDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	//::DefWindowProc( m_hWnd, uMsg, wParam, lParam );
	//bHandled = TRUE;

	RECT clirect;
	BOOL bret;


	HBRUSH brush1;
	HDC hdc1;
	HBRUSH hOldBrush1;
	brush1 = ::CreateSolidBrush( m_UnselLine );
	_ASSERT( brush1 != NULL );
	hdc1 = m_unselline_wnd.GetDC();
	m_unselline_wnd.GetClientRect( &clirect );
	
	hOldBrush1 = (HBRUSH)SelectObject( hdc1, brush1 );
	bret = ::Rectangle( hdc1, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc1, hOldBrush );
	m_unselline_wnd.ReleaseDC( hdc1 );
//	DeleteObject( brush );

////////
	HBRUSH brush2;
	HDC hdc2;
	HBRUSH hOldBrush2;
	brush2 = ::CreateSolidBrush( m_SelLine );
	_ASSERT( brush2 != NULL );
	hdc2 = m_selline_wnd.GetDC();
	m_selline_wnd.GetClientRect( &clirect );
	
	hOldBrush2 = (HBRUSH)SelectObject( hdc2, brush2 );
	bret = ::Rectangle( hdc2, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc2, hOldBrush );
	m_selline_wnd.ReleaseDC( hdc2 );
//	DeleteObject( brush );

/////////
	HBRUSH brush3;
	HDC hdc3;
	HBRUSH hOldBrush3;
	brush3 = ::CreateSolidBrush( m_UnselPoint );
	_ASSERT( brush3 != NULL );
	hdc3 = m_unselpoint_wnd.GetDC();
	m_unselpoint_wnd.GetClientRect( &clirect );
	
	hOldBrush3 = (HBRUSH)SelectObject( hdc3, brush3 );
	bret = ::Rectangle( hdc3, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc3, hOldBrush );
	m_unselpoint_wnd.ReleaseDC( hdc3 );
//	DeleteObject( brush );
////////
	HBRUSH brush4;
	HDC hdc4;
	HBRUSH hOldBrush4;
	brush4 = ::CreateSolidBrush( m_SelPoint );
	_ASSERT( brush4 != NULL );
	hdc4 = m_selpoint_wnd.GetDC();
	m_selpoint_wnd.GetClientRect( &clirect );
	
	hOldBrush4 = (HBRUSH)SelectObject( hdc4, brush4 );
	bret = ::Rectangle( hdc4, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc4, hOldBrush );
	m_selpoint_wnd.ReleaseDC( hdc4 );
//	DeleteObject( brush );
/////////

	return 0;
}


LRESULT COptDlg::OnMouseDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CMouseOptDlg dlg( m_MouseL, m_MouseR, m_MouseW );
	int ret;

	ret = (int)dlg.DoModal();
	if( ret == IDOK ){
		//m_MouseL = dlg.m_MouseL;
		//m_MouseR = dlg.m_MouseR;
		m_MouseW = dlg.m_MouseW;
	}

	return 0;
}

LRESULT COptDlg::OnSetChkDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CSetChkDlg dlg( m_undodlg, m_rdlg, m_infdlg, m_modlg );
	int ret;

	ret = (int)dlg.DoModal();
	if( ret == IDOK ){
		m_undodlg = dlg.m_undodlg;
		m_rdlg = dlg.m_rdlg;
		m_infdlg = dlg.m_infdlg;
		m_modlg = dlg.m_modlg;
	}

	return 0;
}

/***
LRESULT COptDlg::OnCustomUnselLine(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	RECT clirect;
	BOOL bret;

	HBRUSH brush1;
	HDC hdc1;
	HBRUSH hOldBrush1;
	brush1 = ::CreateSolidBrush( m_UnselLine );
	_ASSERT( brush1 != NULL );
	hdc1 = m_unselline_wnd.GetDC();
	m_unselline_wnd.GetClientRect( &clirect );
	
	hOldBrush1 = (HBRUSH)SelectObject( hdc1, brush1 );
	bret = ::Rectangle( hdc1, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc1, hOldBrush );
	m_unselline_wnd.ReleaseDC( hdc1 );
//	DeleteObject( brush );

	return 0;
}
LRESULT COptDlg::OnCustomSelLine(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	RECT clirect;
	BOOL bret;

	HBRUSH brush2;
	HDC hdc2;
	HBRUSH hOldBrush2;
	brush2 = ::CreateSolidBrush( m_SelLine );
	_ASSERT( brush2 != NULL );
	hdc2 = m_selline_wnd.GetDC();
	m_selline_wnd.GetClientRect( &clirect );
	
	hOldBrush2 = (HBRUSH)SelectObject( hdc2, brush2 );
	bret = ::Rectangle( hdc2, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc2, hOldBrush );
	m_selline_wnd.ReleaseDC( hdc2 );
//	DeleteObject( brush );

	return 0;
}
LRESULT COptDlg::OnCustomUnselPoint(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	RECT clirect;
	BOOL bret;

	HBRUSH brush3;
	HDC hdc3;
	HBRUSH hOldBrush3;
	brush3 = ::CreateSolidBrush( m_UnselPoint );
	_ASSERT( brush3 != NULL );
	hdc3 = m_unselpoint_wnd.GetDC();
	m_unselpoint_wnd.GetClientRect( &clirect );
	
	hOldBrush3 = (HBRUSH)SelectObject( hdc3, brush3 );
	bret = ::Rectangle( hdc3, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc3, hOldBrush );
	m_unselpoint_wnd.ReleaseDC( hdc3 );
//	DeleteObject( brush );

	return 0;
}
LRESULT COptDlg::OnCustomSelPoint(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	RECT clirect;
	BOOL bret;

	HBRUSH brush4;
	HDC hdc4;
	HBRUSH hOldBrush4;
	brush4 = ::CreateSolidBrush( m_SelPoint );
	_ASSERT( brush4 != NULL );
	hdc4 = m_selpoint_wnd.GetDC();
	m_selpoint_wnd.GetClientRect( &clirect );
	
	hOldBrush4 = (HBRUSH)SelectObject( hdc4, brush4 );
	bret = ::Rectangle( hdc4, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
//	::SelectObject( hdc4, hOldBrush );
	m_selpoint_wnd.ReleaseDC( hdc4 );
//	DeleteObject( brush );

	return 0;
}
***/
/***
LRESULT COptDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL dummy;
	OnPaint( 0, 0, 0, dummy );

	return 0;
}
***/