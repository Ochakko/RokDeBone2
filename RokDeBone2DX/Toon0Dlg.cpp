// Toon0Dlg.cpp : CToon0Dlg のインプリメンテーション
#include "stdafx.h"
#include "Toon0Dlg.h"

#include "viewer.h"

#include <commctrl.h>

#include <treehandler2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <treeelem2.h>

#include <polymesh.h>
#include <polymesh2.h>

#include "ColDlg.h"

#include <Toon1Params.h>
#include "GetDlgParams.h"

#include "inidata.h"

#define RENDERTEXTIMER	11005


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// extern
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern CColDlg g_coldlg;
extern CIniData* g_inidata;

/////////////////////////////////////////////////////////////////////////////
// CToon0Dlg

CToon0Dlg::CToon0Dlg( CMyD3DApplication* papp )
{
	m_timerid = 0;

	m_papp = papp;

	m_seldisp = 0;
	m_selmaterial = 0;
	m_toon1 = 0;

	m_cmdshow = SW_HIDE;


	m_colamb_dc = NULL;
	m_coldif_dc = NULL;
	m_colspc_dc = NULL;
	m_diffuse_brush = NULL;
	m_ambient_brush = NULL;
	m_specular_brush = NULL;
	m_edgecol0_brush = NULL;

	m_diffuse_pen = NULL;
	m_ambient_pen = NULL;
	m_specular_pen = NULL;
	m_edgecol0_pen = NULL;

	m_diffuse = RGB( 255, 255, 255 );
	m_ambient = RGB( 255, 255, 255 );
	m_specular = RGB( 255, 255, 255 );
	m_edgecol0 = RGB( 255, 255, 255 );

	m_darkdeg = 66.422f;
	m_brightdeg = 25.842f;

	ZeroMemory( m_name, sizeof( char ) * 32 );

	m_dispedge0 = 1;
	m_invedge0 = 0;
	m_edgewidth0 = 8.0f;

}

CToon0Dlg::~CToon0Dlg()
{
}

LRESULT CToon0Dlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	if( m_timerid != 0 ){
		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;
		//Sleep( 100 );//!!!!!!!!!!!!!!!!!!!
	}


	DestroyObjs();

	if( m_colamb_dc ){
		::ReleaseDC( m_colamb_wnd, m_colamb_dc );
		m_colamb_dc = NULL;
	}
	if( m_coldif_dc ){
		::ReleaseDC( m_coldif_wnd, m_coldif_dc );
		m_coldif_dc = NULL;
	}
	if( m_colspc_dc ){
		::ReleaseDC( m_colspc_wnd, m_colspc_dc );
		m_colspc_dc = NULL;
	}
	if( m_edgecol0_dc ){
		::ReleaseDC( m_edgecol0_wnd, m_edgecol0_dc );
		m_edgecol0_dc = NULL;
	}
	return 0;
}


int CToon0Dlg::DestroyObjs()
{
	return 0;
}


int CToon0Dlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;
	ShowWindow( m_cmdshow );

	if( cmdshow != SW_HIDE ){
		
		CreateList1();
		CreateList2();

		ParamsToDlg();
	}

	return 0;
}

LRESULT CToon0Dlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//InitCommonControls();

	m_dlg_wnd = m_hWnd;


	m_list1_wnd = GetDlgItem( IDC_LIST1 );
	m_list2_wnd = GetDlgItem( IDC_LIST2 );

	m_colamb_wnd = GetDlgItem( IDC_COLAMB );
	m_colamb_dc = ::GetDC( m_colamb_wnd );
	m_coldif_wnd = GetDlgItem( IDC_COLDIF );
	m_coldif_dc = ::GetDC( m_coldif_wnd );
	m_colspc_wnd = GetDlgItem( IDC_COLSPC );
	m_colspc_dc = ::GetDC( m_colspc_wnd );
	m_edgecol0_wnd = GetDlgItem( IDC_EDGECOL0 );
	m_edgecol0_dc = ::GetDC( m_edgecol0_wnd );

	m_diffuse_brush = ::CreateSolidBrush( m_diffuse );
	m_ambient_brush = ::CreateSolidBrush( m_ambient );
	m_specular_brush = ::CreateSolidBrush( m_specular );
	m_edgecol0_brush = ::CreateSolidBrush( m_edgecol0 );

	m_diffuse_pen = ::CreatePen( PS_SOLID, 0, m_diffuse );
	m_ambient_pen = ::CreatePen( PS_SOLID, 0, m_ambient );
	m_specular_pen = ::CreatePen( PS_SOLID, 0, m_specular );
	m_white_pen = ::CreatePen( PS_SOLID, 0, RGB( 255, 255, 255 ) );
	m_edgecol0_pen = ::CreatePen( PS_SOLID, 0, m_edgecol0 );

	m_name_wnd = GetDlgItem( IDC_NAME );

	m_sl_darkdeg_wnd = GetDlgItem( IDC_SLDARKDEG );
	m_sl_brightdeg_wnd = GetDlgItem( IDC_SLBRIGHTDEG );
	m_darkdeg_wnd = GetDlgItem( IDC_TEXT_DARKDEG );
	m_brightdeg_wnd = GetDlgItem( IDC_TEXT_BRIGHTDEG );

	m_sl_darkdeg_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_darkdeg_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)90000 );
	int darkdeg = (int)( m_darkdeg * 1000.0f );
	m_sl_darkdeg_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)darkdeg );

	m_sl_brightdeg_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_brightdeg_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)90000 );
	int brdeg = (int)( m_brightdeg * 1000.0f );
	m_sl_brightdeg_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)brdeg );


	m_diffuse_wnd = GetDlgItem( IDC_TEXT_DIFFUSE );
	m_ambient_wnd = GetDlgItem( IDC_TEXT_AMBIENT );
	m_specular_wnd = GetDlgItem( IDC_TEXT_SPECULAR );

	m_text_edgecol0_wnd = GetDlgItem( IDC_TEXT_EDGECOL0 );
	m_edgewidth0_wnd = GetDlgItem( IDC_EDGEWIDTH0 );

	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERTEXTIMER, 100 );
		DbgOut( "Toon0Dlg : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERTEXTIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}



	return 1;  // システムにフォーカスを設定させます
}

LRESULT CToon0Dlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);
	return 0;
}

LRESULT CToon0Dlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);
	return 0;
}


int CToon0Dlg::ParamsToDlg()
{

	m_name_wnd.SetWindowText( m_name );


	int darkdeg = (int)( m_darkdeg * 1000.0f );
	m_sl_darkdeg_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)darkdeg );

	int brdeg = (int)( m_brightdeg * 1000.0f );
	m_sl_brightdeg_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)brdeg );

	char tempchar[256];
	sprintf_s( tempchar, 256, "%f度", m_darkdeg );
	m_darkdeg_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%f度", m_brightdeg );
	m_brightdeg_wnd.SetWindowText( tempchar );


	sprintf_s( tempchar, 256, "( %d, %d, %d )", GetRValue( m_diffuse ), GetGValue( m_diffuse ), GetBValue( m_diffuse ) );
	m_diffuse_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "( %d, %d, %d )", GetRValue( m_ambient ), GetGValue( m_ambient ), GetBValue( m_ambient ) );
	m_ambient_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "( %d, %d, %d )", GetRValue( m_specular ), GetGValue( m_specular ), GetBValue( m_specular ) );
	m_specular_wnd.SetWindowText( tempchar );


	sprintf_s( tempchar, 256, "( %d, %d, %d )", GetRValue( m_edgecol0 ), GetGValue( m_edgecol0 ), GetBValue( m_edgecol0 ) );
	m_text_edgecol0_wnd.SetWindowText( tempchar );


	if( m_dispedge0 ){
		m_dlg_wnd.CheckDlgButton( IDC_DISPEDGE0, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_DISPEDGE0, BST_UNCHECKED );
	}
	if( m_invedge0 ){
		m_dlg_wnd.CheckDlgButton( IDC_INVEDGE0, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_INVEDGE0, BST_UNCHECKED );
	}

	sprintf_s( tempchar, 256, "%f", m_edgewidth0 );
	m_edgewidth0_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CToon0Dlg::OnSelchangeList1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( wNotifyCode != LBN_SELCHANGE )
		return 0;

	m_toon1 = 0;//!!!!!!!!!

	int selindex;
	selindex = (int)m_list1_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		m_seldisp = 0;
		return 0;
	}
	m_seldisp = (int)m_list1_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );
	if( (m_seldisp <= 0) || (m_seldisp >= m_papp->m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		m_seldisp = 0;
		return 1;
	}

	CreateList2();

////////////
	if( (m_seldisp > 0) && (m_selmaterial >= 0) ){
		CShdElem* selem;
		selem = (*m_papp->m_shandler)( m_seldisp );
		_ASSERT( selem );

		if( selem->type == SHDPOLYMESH ){
			if( selem->polymesh && selem->polymesh->m_toon1 ){
				if( m_selmaterial < selem->polymesh->m_materialnum ){
					m_toon1 = selem->polymesh->m_toon1 + m_selmaterial;
				}
			}
		}else if( selem->type == SHDPOLYMESH2 ){
			if( selem->polymesh2 && selem->polymesh2->m_toon1 ){
				if( m_selmaterial < selem->polymesh2->m_materialnum ){
					m_toon1 = selem->polymesh2->m_toon1 + m_selmaterial;
				}
			}
		}
	}


////////////
	Toon1ToParams();

	ParamsToDlg();
	
	return 0;
}

int CToon0Dlg::Toon1ToParams()
{
	if( m_toon1 ){
		m_diffuse = RGB( (int)( m_toon1->diffuse.r * 255.0f ), (int)( m_toon1->diffuse.g * 255.0f ), (int)( m_toon1->diffuse.b * 255.0f ) );
		m_ambient = RGB( (int)( m_toon1->ambient.r * 255.0f ), (int)( m_toon1->ambient.g * 255.0f ), (int)( m_toon1->ambient.b * 255.0f ) );
		m_specular = RGB( (int)( m_toon1->specular.r * 255.0f ), (int)( m_toon1->specular.g * 255.0f ), (int)( m_toon1->specular.b * 255.0f ) );
		m_edgecol0 = RGB( (int)( m_toon1->edgecol0.r * 255.0f ), (int)( m_toon1->edgecol0.g * 255.0f ), (int)( m_toon1->edgecol0.b * 255.0f ) );

		m_darkdeg = (float)acos( m_toon1->toon0dnl ) * (float)PAI2DEG;
		m_brightdeg = (float)acos( m_toon1->toon0bnl ) * (float)PAI2DEG;


		if( m_diffuse_brush )
			DeleteObject( m_diffuse_brush );
		m_diffuse_brush = ::CreateSolidBrush( m_diffuse );

		if( m_ambient_brush )
			DeleteObject( m_ambient_brush );
		m_ambient_brush = ::CreateSolidBrush( m_ambient );

		if( m_specular_brush )
			DeleteObject( m_specular_brush );
		m_specular_brush = ::CreateSolidBrush( m_specular );

		if( m_edgecol0_brush )
			DeleteObject( m_edgecol0_brush );
		m_edgecol0_brush = ::CreateSolidBrush( m_edgecol0 );

		if( m_diffuse_pen )
			DeleteObject( m_diffuse_pen );
		m_diffuse_pen = ::CreatePen( PS_SOLID, 0, m_diffuse );

		if( m_ambient_pen )
			DeleteObject( m_ambient_pen );
		m_ambient_pen = ::CreatePen( PS_SOLID, 0, m_ambient );

		if( m_specular_pen )
			DeleteObject( m_specular_pen );
		m_specular_pen = ::CreatePen( PS_SOLID, 0, m_specular );

		if( m_edgecol0_pen )
			DeleteObject( m_edgecol0_pen );
		m_edgecol0_pen = ::CreatePen( PS_SOLID, 0, m_edgecol0 );


		int leng = (int)strlen( m_toon1->name );
		if( leng < 32 ){
			strcpy_s( m_name, 32, m_toon1->name );
		}

		m_dispedge0 = m_toon1->edgevalid0;
		m_invedge0 = m_toon1->edgeinv0;
		m_edgewidth0 = m_toon1->edgecol0.a;
	}

	return 0;
}

LRESULT CToon0Dlg::OnSelchangeList2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	// OnChangeNameからも呼ばれるので注意！！！！


	if( wNotifyCode != LBN_SELCHANGE )
		return 0;

	if( m_seldisp <= 0 )
		return 0;//!!!!!!!!!!!

	m_toon1 = 0;//!!!!!!!!!

	int selindex;
	selindex = (int)m_list2_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		m_selmaterial = 0;
		return 0;
	}
	m_selmaterial = selindex;

////////////
	if( (m_seldisp > 0) && (m_selmaterial >= 0) ){
		CShdElem* selem;
		selem = (*m_papp->m_shandler)( m_seldisp );
		_ASSERT( selem );

		if( selem->type == SHDPOLYMESH ){
			if( selem->polymesh && selem->polymesh->m_toon1 ){
				if( m_selmaterial < selem->polymesh->m_materialnum ){
					m_toon1 = selem->polymesh->m_toon1 + m_selmaterial;
				}
			}
		}else if( selem->type == SHDPOLYMESH2 ){
			if( selem->polymesh2 && selem->polymesh2->m_toon1 ){
				if( m_selmaterial < selem->polymesh2->m_materialnum ){
					m_toon1 = selem->polymesh2->m_toon1 + m_selmaterial;
				}
			}
		}
	}

////////////
	Toon1ToParams();

	ParamsToDlg();
	
	return 0;
}

int CToon0Dlg::CreateList1()
{
	m_seldisp = 0;
	m_selmaterial = 0;


	int ret;
///// list1
	m_list1_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );

	int seri;
	for( seri = 1; seri < m_papp->m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_papp->m_shandler)( seri );
		_ASSERT( selem );


		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){

			CTreeElem2* telem;
			telem = (*m_papp->m_thandler)( seri );
			_ASSERT( telem );


			int newindex;
			newindex = (int)m_list1_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)telem->name );
			if( (newindex == LB_ERR) || (newindex == LB_ERRSPACE) ){
				_ASSERT( 0 );
				return 1;
			}

			int ret;
			ret = (int)m_list1_wnd.SendMessage( LB_SETITEMDATA, newindex, selem->serialno );
			if( ret == LB_ERR ){
				_ASSERT( 0 );
				return 1;
			}
		}
	}
	/////////////  先頭項目を選択
	int selindex = 0;
	ret = (int)m_list1_wnd.SendMessage( LB_SETCURSEL, selindex, 0 );
	if( ret == LB_ERR ){
		_ASSERT( 0 );
		return 1;
	}
	m_seldisp = (int)m_list1_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );
	if( (m_seldisp <= 0) || (m_seldisp >= m_papp->m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		m_seldisp = 0;
		return 1;
	}


	return 0;
}

int CToon0Dlg::CreateList2()
{


	m_selmaterial = 0;

	int ret;
//////// list2
	m_list2_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	

	if( m_seldisp <= 0 )
		return 0;//!!!!!!!!!!!


	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_seldisp );
	_ASSERT( selem );

	m_toon1 = 0;//!!!!!!
	int matnum = 0;
	if( selem->type == SHDPOLYMESH ){
		if( selem->polymesh ){
			m_toon1 = selem->polymesh->m_toon1;
			matnum = selem->polymesh->m_materialnum;
		}
	}else if( selem->type == SHDPOLYMESH2 ){
		if( selem->polymesh2 ){
			m_toon1 = selem->polymesh2->m_toon1;
			matnum = selem->polymesh2->m_materialnum;
		}
	}else{
		_ASSERT( 0 );
	}

	if( m_toon1 ){
		int matno;
		for( matno = 0; matno < matnum; matno++ ){

			int newindex;
			newindex = (int)m_list2_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)(m_toon1 + matno)->name );
			if( (newindex == LB_ERR) || (newindex == LB_ERRSPACE) ){
				_ASSERT( 0 );
				return 1;
			}
		}

		/////////////  先頭項目を選択
		m_selmaterial = 0;
		ret = (int)m_list2_wnd.SendMessage( LB_SETCURSEL, m_selmaterial, 0 );
		if( ret == LB_ERR ){
			_ASSERT( 0 );
			return 1;
		}
		
		Toon1ToParams();
		ParamsToDlg();
	}else{
		_ASSERT( 0 );
	}

	return 0;
}

LRESULT CToon0Dlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static int savecurpos = 0;


	HWND scwnd = (HWND)lParam;
	WORD event;

	event = LOWORD( wParam );

	int curpos;
	LRESULT lres;

	if( (event = SB_THUMBTRACK) || (event == SB_THUMBPOSITION ) ){

		if( scwnd == (HWND)m_sl_darkdeg_wnd ){
			curpos = (int)m_sl_darkdeg_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			m_darkdeg = (float)curpos / 1000.0f;

			ParamsToDlg();

			BOOL dummy;
			lres = OnReleasedcaptureSl_darkdeg( 0, 0, dummy );
			_ASSERT( !lres );

		}else if( scwnd == (HWND)m_sl_brightdeg_wnd ){
			curpos = (int)m_sl_brightdeg_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			m_brightdeg = (float)curpos / 1000.0f;

			ParamsToDlg();

			BOOL dummy;
			lres = OnReleasedcaptureSl_brightdeg( 0, 0, dummy );
			_ASSERT( !lres );

		}else{
			return 0;//!!!!!!!!!!!!
		}

	}

	return 0;
}


LRESULT CToon0Dlg::OnReleasedcaptureSl_darkdeg(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	//int ret;
	int curpos;
	curpos = (int)m_sl_darkdeg_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	m_darkdeg = (float)curpos / 1000.0f;

	if( pnmh && (m_darkdeg < m_brightdeg) ){//hscrollから呼ばれた時はpnmhは０
		::MessageBox( m_hWnd, "ハイライト色の上限より、小さい値は設定できません", "確認", MB_OK );
		m_darkdeg = m_brightdeg;

	}
	ParamsToDlg();


	if( m_toon1 ){
		float fcos;
		fcos = (float)cos( m_darkdeg * (float)DEG2PAI );
		fcos = min( fcos, 1.0f );
		fcos = max( fcos, 0.0f );

		m_toon1->toon0dnl = fcos;
	}

	return 0;
}
LRESULT CToon0Dlg::OnReleasedcaptureSl_brightdeg(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	//int ret;
	int curpos;
	curpos = (int)m_sl_brightdeg_wnd.SendMessage( TBM_GETPOS, 0, 0 );
	m_brightdeg = (float)curpos / 1000.0f;

	if( pnmh && (m_brightdeg > m_darkdeg) ){//hscrollから呼ばれた時はpnmhは０
		::MessageBox( m_hWnd, "暗い色の下限より、大きい値は設定できません", "確認", MB_OK );
		m_brightdeg = m_darkdeg;
	}
	ParamsToDlg();


	if( m_toon1 ){
		float fcos;
		fcos = (float)cos( m_brightdeg * (float)DEG2PAI );
		fcos = min( fcos, 1.0f );
		fcos = max( fcos, 0.0f );

		m_toon1->toon0bnl = fcos;
	}

	return 0;
}


int CToon0Dlg::PaintTexture()
{
	if( m_cmdshow == SW_HIDE )
		return 0;

	if( !(m_colamb_wnd.m_hWnd) || !(m_coldif_wnd.m_hWnd) || !(m_colspc_wnd.m_hWnd) )
		return 0;


	BOOL bret;
	RECT clirect;
	bret = m_colamb_wnd.GetClientRect( &clirect );
	if( !bret )
		return 0;
	SelectObject( m_colamb_dc, m_ambient_pen );
	SelectObject( m_colamb_dc, m_ambient_brush );
	bret = ::Rectangle( m_colamb_dc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret )
		return 0;


	bret = m_coldif_wnd.GetClientRect( &clirect );
	if( !bret )
		return 0;
	SelectObject( m_coldif_dc, m_diffuse_pen );
	SelectObject( m_coldif_dc, m_diffuse_brush );
	bret = ::Rectangle( m_coldif_dc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret )
		return 0;


	bret = m_colspc_wnd.GetClientRect( &clirect );
	if( !bret )
		return 0;
	SelectObject( m_colspc_dc, m_specular_pen );
	SelectObject( m_colspc_dc, m_specular_brush );
	bret = ::Rectangle( m_colspc_dc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret )
		return 0;


	//::SelectObject( hdc, hOldBrush );

	return 0;
}


int CToon0Dlg::PaintEdgeCol0()
{
	if( m_cmdshow == SW_HIDE )
		return 0;

	if( !(m_edgecol0_wnd.m_hWnd) )
		return 0;


	BOOL bret;
	RECT clirect;
	bret = m_edgecol0_wnd.GetClientRect( &clirect );
	if( !bret )
		return 0;

	int width;
	width = clirect.right - clirect.left;

	SelectObject( m_edgecol0_dc, m_edgecol0_pen );
	SelectObject( m_edgecol0_dc, m_edgecol0_brush );
	bret = ::Rectangle( m_edgecol0_dc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret )
		return 0;

	//::SelectObject( hdc, hOldBrush );

	return 0;
}


LRESULT CToon0Dlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//int ret;

	int timerid = (int)wParam;
	if( (timerid == m_timerid) && (m_cmdshow != SW_HIDE) ){
		PaintTexture();
		PaintEdgeCol0();
	}

	return 0;
}


LRESULT CToon0Dlg::OnDiffuse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_dlg_wnd, &m_diffuse );

	if( m_diffuse_brush )
		DeleteObject( m_diffuse_brush );
	m_diffuse_brush = ::CreateSolidBrush( m_diffuse );

	if( m_diffuse_pen )
		DeleteObject( m_diffuse_pen );
	m_diffuse_pen = ::CreatePen( PS_SOLID, 0, m_diffuse );

	ParamsToDlg();

	if( m_toon1 ){
		float fr, fg, fb;
		fr = (float)GetRValue( m_diffuse ) / 255.0f;
		fr = min( fr, 1.0f );
		fr = max( fr, 0.0f );

		fg = (float)GetGValue( m_diffuse ) / 255.0f;
		fg = min( fg, 1.0f );
		fg = max( fg, 0.0f );

		fb = (float)GetBValue( m_diffuse ) / 255.0f;
		fb = min( fb, 1.0f );
		fb = max( fb, 0.0f );

		m_toon1->diffuse.r = fr;
		m_toon1->diffuse.g = fg;
		m_toon1->diffuse.b = fb;

	}

	return 0;
}

LRESULT CToon0Dlg::OnAmbient(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_dlg_wnd, &m_ambient );

	if( m_ambient_brush )
		DeleteObject( m_ambient_brush );
	m_ambient_brush = ::CreateSolidBrush( m_ambient );

	if( m_ambient_pen )
		DeleteObject( m_ambient_pen );
	m_ambient_pen = ::CreatePen( PS_SOLID, 0, m_ambient );

	ParamsToDlg();

	if( m_toon1 ){
		float fr, fg, fb;
		fr = (float)GetRValue( m_ambient ) / 255.0f;
		fr = min( fr, 1.0f );
		fr = max( fr, 0.0f );

		fg = (float)GetGValue( m_ambient ) / 255.0f;
		fg = min( fg, 1.0f );
		fg = max( fg, 0.0f );

		fb = (float)GetBValue( m_ambient ) / 255.0f;
		fb = min( fb, 1.0f );
		fb = max( fb, 0.0f );

		m_toon1->ambient.r = fr;
		m_toon1->ambient.g = fg;
		m_toon1->ambient.b = fb;

	}

	return 0;
}
LRESULT CToon0Dlg::OnSpecular(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_dlg_wnd, &m_specular );

	if( m_specular_brush )
		DeleteObject( m_specular_brush );
	m_specular_brush = ::CreateSolidBrush( m_specular );

	if( m_specular_pen )
		DeleteObject( m_specular_pen );
	m_specular_pen = ::CreatePen( PS_SOLID, 0, m_specular );

	ParamsToDlg();

	if( m_toon1 ){
		float fr, fg, fb;
		fr = (float)GetRValue( m_specular ) / 255.0f;
		fr = min( fr, 1.0f );
		fr = max( fr, 0.0f );

		fg = (float)GetGValue( m_specular ) / 255.0f;
		fg = min( fg, 1.0f );
		fg = max( fg, 0.0f );

		fb = (float)GetBValue( m_specular ) / 255.0f;
		fb = min( fb, 1.0f );
		fb = max( fb, 0.0f );

		m_toon1->specular.r = fr;
		m_toon1->specular.g = fg;
		m_toon1->specular.b = fb;

	}

	return 0;
}

LRESULT CToon0Dlg::OnChangeName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int textleng;
	textleng = m_name_wnd.GetWindowTextLength();
	if( (textleng >= 32) || (textleng < 1) ){
		::MessageBox( m_hWnd, "名前は、１バイト以上、３１バイト以下です。", "エラー", MB_OK );
		ParamsToDlg();
		return 0;//!!!!!!!!!
	}

	m_name_wnd.GetWindowText( m_name, textleng + 1 );


	ParamsToDlg();

	if( m_toon1 ){
		strcpy_s( m_toon1->name, 32, m_name );
	}

	int ret;
	int selindex;
	selindex = (int)m_list2_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	_ASSERT( selindex != LB_ERR );

	CreateList2();

	ret = (int)m_list2_wnd.SendMessage( LB_SETCURSEL, selindex, 0 );
	_ASSERT( ret != LB_ERR );

	BOOL dummy;
	OnSelchangeList2( LBN_SELCHANGE, 0, 0, dummy );

	return 0;
}

LRESULT CToon0Dlg::OnEdgeCol0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_dlg_wnd, &m_edgecol0 );

	if( m_edgecol0_brush )
		DeleteObject( m_edgecol0_brush );
	m_edgecol0_brush = ::CreateSolidBrush( m_edgecol0 );

	if( m_edgecol0_pen )
		DeleteObject( m_edgecol0_pen );
	m_edgecol0_pen = ::CreatePen( PS_SOLID, 0, m_edgecol0 );

	ParamsToDlg();

	if( m_toon1 ){
		float fr, fg, fb;
		fr = (float)GetRValue( m_edgecol0 ) / 255.0f;
		fr = min( fr, 1.0f );
		fr = max( fr, 0.0f );

		fg = (float)GetGValue( m_edgecol0 ) / 255.0f;
		fg = min( fg, 1.0f );
		fg = max( fg, 0.0f );

		fb = (float)GetBValue( m_edgecol0 ) / 255.0f;
		fb = min( fb, 1.0f );
		fb = max( fb, 0.0f );

		m_toon1->edgecol0.r = fr;
		m_toon1->edgecol0.g = fg;
		m_toon1->edgecol0.b = fb;

		//ret = m_toon1->CreateTexture( m_papp->m_pd3dDevice );
		//if( ret ){
		//	DbgOut( "Toon0Dlg : OnSpecular : toon1 CreateTexture error !!!\n" );
		//	_ASSERT( 0 );
		//	return 1;
		//}

	}

	return 0;
}
LRESULT CToon0Dlg::OnDispEdge0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_dispedge0 )
		m_dispedge0 = 0;
	else
		m_dispedge0 = 1;

	ParamsToDlg();

	if( m_toon1 ){
		m_toon1->edgevalid0 = m_dispedge0;
	}

	return 0;
}
LRESULT CToon0Dlg::OnInvEdge0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_invedge0 )
		m_invedge0 = 0;
	else
		m_invedge0 = 1;

	ParamsToDlg();

	if( m_toon1 ){
		m_toon1->edgeinv0 = m_invedge0;
	}

	return 0;
}

LRESULT CToon0Dlg::OnEdgeWidth0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	float tmpfloat;
	int ret;

	ret = GetFloat( m_edgewidth0_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "線の太さが不正です。", "入力エラー", MB_OK );
		return 0;//!!!!!!!
	}

	m_edgewidth0 = tmpfloat;

	ParamsToDlg();

	if( m_toon1 ){
		m_toon1->edgecol0.a = m_edgewidth0;
	}

	return 0;
}

