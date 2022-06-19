// ShdParamDlg.cpp : CShdParamDlg のインプリメンテーション
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

//#define NOTWINDOWSH

#include "ShdParamDlg.h"
#include "TexSettingDlg.h"
#include "SigEditWnd.h"
//#include "SigTreeView2.h"

//#include <stdio.h>
/**
#define STRICT
#include <windowsx.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <d3dx9.h>
**/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <coef.h>
//#include <basedat.h>
#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <mothandler.h>
#include <motionctrl.h>
#include <d3ddisp.h>

#include <polymesh2.h>

#include <Commdlg.h>

#include "ColDlg.h"

extern CColDlg g_coldlg;

//#include <Afxdlgs.h>
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CShdParamDlg

CShdParamDlg::CShdParamDlg( CTreeElem2* lptelem, CShdElem* lpselem, CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, CSigEditWnd* stvptr )
{
	m_shandler = lpsh;
	m_mhandler = lpmh;
	m_thandler = lpth;
	m_telem = lptelem;
	m_selem = lpselem;
	m_stv = stvptr;

	ZeroMemory( m_shdname, 1024 );
	m_serialno = 0;
	m_shdtype = 0;
	m_depth = 0;
	m_clockwise = 0;
	//ZeroMemory( m_texname, 1024 );
	//m_texrepx = 0;
	//m_texrepy = 0;
	//m_transparent = 0;
	//m_uanime = 0.0f;
	//m_vanime = 0.0f;
	//m_texrule = 0;
	m_notuse = 0;
	m_notransik = 0;
	m_ikskip = 0;
	m_invisible = 0;

	m_noskinning = 0;
	m_dispswitchno = 0;

	m_diffuse_brush = NULL;
	m_ambient_brush = NULL;
	m_diffuse_color.SetMem( 1.0f, 1.0f, 1.0f );
	m_ambient_color.SetMem( 0.0f, 0.0f, 0.0f );
	m_specular_color.SetMem( 0.0f, 0.0f, 0.0f );
		

	editflag = 0;

}

CShdParamDlg::~CShdParamDlg()
{
}


LRESULT CShdParamDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_dlg_wnd = m_hWnd;

	HWND temphwnd;
	temphwnd = GetDlgItem( IDC_SHDNAME );
	m_shdname_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SERIALNO );
	m_serialno_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SHDTYPE );
	m_shdtype_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_DEPTH );
	m_depth_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_CLOCKWISE );
	m_clockwise_wnd = temphwnd;


	//temphwnd = GetDlgItem( IDC_COLORDISP );
	//m_colordisp_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_NOTUSE );
	m_notuse_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_NOTRANSIK );
	m_notransik_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_IKSKIP );
	m_ikskip_wnd = temphwnd;

	m_invisible_wnd = GetDlgItem( IDC_INVISIBLE );

	temphwnd = GetDlgItem( IDC_NUMPRIM );
	m_numprim_wnd = temphwnd;

	temphwnd = GetDlgItem( IDOK );
	m_ok_wnd = temphwnd;

	temphwnd = GetDlgItem( IDCANCEL );
	m_cancel_wnd = temphwnd;


	temphwnd = GetDlgItem( IDC_TEXT_STATUS );
	m_text_status_wnd = temphwnd;

	m_check_skinning_wnd = GetDlgItem( IDC_CHECK_SKINNING );
	m_check_skinning_wnd.EnableWindow( 0 );//!!!!!!!!!!!!!!!!!!! 操作禁止に

	m_all_noskinning_wnd = GetDlgItem( IDC_ALL_NOSKINNING );

	m_all_skinning_wnd = GetDlgItem( IDC_ALL_SKINNING );

	m_dispswitchno_wnd = GetDlgItem( IDC_DISPSWITCHNO );
	int dispno;
	char tempchar[ 256 ];
	for( dispno = 0; dispno < DISPSWITCHNUM; dispno++ ){
		ZeroMemory( tempchar, 256 );
		sprintf_s( tempchar, 256, "%d", dispno );

		m_dispswitchno_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	}
	m_dispswitchno_wnd.SendMessage( CB_SETCURSEL, 0, 0 );


////////////
	//addstring ではsortされて格納されるので、1,2,3の番号を先頭につける。
	m_clockwise_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"1: 裏面" );
	m_clockwise_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"2: おもて面" );
	m_clockwise_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"3: 両面" );
//	m_clockwise_wnd.SendMessage( CB_SETCURSEL, (m_clockwise - 1), 0 );

/////////////
	ParamsToDlg();


	return 1;  // システムにフォーカスを設定させます
}

int CShdParamDlg::ParamsToDlg()
{


/////////////
	m_text_status_wnd.SetWindowText( "" );

	ZeroMemory( m_shdname, 1024 );
	strcpy_s( m_shdname, 1024, m_telem->name );
	m_serialno = m_selem->serialno;
	m_shdtype = m_selem->type;
	m_depth = m_selem->depth;
	m_clockwise = m_selem->clockwise;
	
	m_notuse = m_selem->notuse;
	m_notransik = m_selem->m_notransik;
	m_ikskip = m_selem->m_ikskip;
	m_invisible = m_selem->invisibleflag;
	m_noskinning = m_selem->noskinning;
	m_dispswitchno = m_selem->dispswitchno;

///////////////
	char tempchar[1024];
	char tempchar2[50];
	m_shdname_wnd.SetWindowText( m_shdname );
	
	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "serialno : " );
	ZeroMemory( tempchar2, 50 );
	sprintf_s( tempchar2, 50, "%d", m_serialno );
	strcat_s( tempchar, 1024, tempchar2 );
	m_serialno_wnd.SetWindowText( tempchar );

//DbgOut( "ShdParamDlg : OnInitDialog : typechar %d, %s\n", 
//	   m_shdtype, typechar[m_shdtype] );

	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "type : " );
	strcat_s( tempchar, 1024, typechar[m_shdtype] );
	m_shdtype_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "Depth : " );
	ZeroMemory( tempchar2, 50 );
	sprintf_s( tempchar2, 50, "%d", m_depth );
	strcat_s( tempchar, 1024, tempchar2 );
	m_depth_wnd.SetWindowText( tempchar );


	if( m_notuse ){
		m_dlg_wnd.CheckDlgButton( IDC_NOTUSE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_NOTUSE, BST_UNCHECKED );
	}
	if( m_noskinning ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_SKINNING, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_SKINNING, BST_UNCHECKED );
	}
	if( m_notransik ){
		m_dlg_wnd.CheckDlgButton( IDC_NOTRANSIK, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_NOTRANSIK, BST_UNCHECKED );
	}
	if( m_ikskip ){
		m_dlg_wnd.CheckDlgButton( IDC_IKSKIP, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_IKSKIP, BST_UNCHECKED );
	}
	if( m_invisible ){
		m_dlg_wnd.CheckDlgButton( IDC_INVISIBLE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_INVISIBLE, BST_UNCHECKED );
	}

	if( m_selem->IsJoint() && (m_selem->type != SHDMORPH) ){
		m_notransik_wnd.EnableWindow( TRUE );
		m_ikskip_wnd.EnableWindow( TRUE );
	}else{
		m_notransik_wnd.EnableWindow( FALSE );
		m_ikskip_wnd.EnableWindow( TRUE );
	}
	if( (m_selem->type == SHDPOLYMESH) || (m_selem->type == SHDPOLYMESH2) ){
		m_invisible_wnd.EnableWindow( TRUE );
	}else{
		m_invisible_wnd.EnableWindow( FALSE );
	}


//dispswitchno
	//m_dispswitchno_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"0：なし" );
	m_dispswitchno_wnd.SendMessage( CB_SETCURSEL, m_dispswitchno, 0 );

	if( m_selem->IsJoint() || (m_selem->dispflag == 0) ){
		m_dispswitchno_wnd.EnableWindow( FALSE );
	}else{
		m_dispswitchno_wnd.EnableWindow( TRUE );
	}


//clockwise
	m_clockwise_wnd.SendMessage( CB_SETCURSEL, (m_clockwise - 1), 0 );

	if( m_selem->type == SHDMORPH ){
		m_clockwise_wnd.EnableWindow( 0 );
	}else{
		m_clockwise_wnd.EnableWindow( 1 );
	}

//numprim
	char strnumprim[1024];
	ZeroMemory( strnumprim, 1024 );
	if( m_selem->dispflag && m_selem->d3ddisp ){
		DWORD numprim = (m_selem->d3ddisp)->m_numPrim;
		if( m_selem->clockwise == 3 )
			numprim *= 2;
		sprintf_s( strnumprim, 1024, "ポリゴン数　：　%d ポリゴン", numprim );
	}else{
		sprintf_s( strnumprim, 1024, "ポリゴン数　：　0 ポリゴン" );
	}
	m_numprim_wnd.SetWindowText( strnumprim );


////////////

	//m_transparent_wnd.EnableWindow( FALSE );
	//m_texrepx_wnd.EnableWindow( FALSE );
	//m_texrepy_wnd.EnableWindow( FALSE );

	
	return 0;
}


int CShdParamDlg::ApplyParams( char* errmsg )
{
	int ret = 0;
	int cmpret = 0;
	//char errmsg[1024];
	ZeroMemory( errmsg, 1024 );

	char tempchar[1024];
	int tempint;
	//float tempfloat;

	//int isedittexrepx, isedittexrepy;
	//int isedituanime, iseditvanime;
	//int iseditcolor;

//m_shdname
	ZeroMemory( tempchar, 1024 );
	ret += GetText( m_shdname_wnd, tempchar, 1024 );
	_ASSERT( !ret );
	if( *tempchar == 0 ){
		ret = 1;
		strcpy_s( errmsg, 1024, "Treeの名前を入力してください。" );
		return 1;
	}
	cmpret = strcmp( m_shdname, tempchar );
	if( cmpret ){
		editflag |= TV_EDIT_SHDNAME;
		strcpy_s( m_shdname, 1024, tempchar );
		//int ret1;
		//ret1 = m_telem->SetName( m_shdname );
		//_ASSERT( !ret1 );
	}

//m_clockwise
	int combono;
	combono = (int)m_clockwise_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		_ASSERT( 0 );
		return 0;
	}
	tempint = combono + 1;
	if( tempint != m_clockwise ){
		editflag |= TV_EDIT_CLOCKWISE;
		m_clockwise = tempint;

		int ret1;
		ret1 = m_selem->SetClockwise( m_clockwise );
		_ASSERT( !ret1 );

	}

//m_texname
//m_texrepx, m_texrepy
//m_transparent
//m_uanime, m_vanime
//m_texrule


//m_notuse
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_NOTUSE );
	if( checkflag == BST_CHECKED ){
		tempint = 1;
	}else if( checkflag == BST_UNCHECKED ){
		tempint = 0;
	}

	if( m_notuse != tempint ){
		editflag |= TV_EDIT_NOTUSE;
		m_notuse = tempint;

		ret += m_selem->SetNotUse( m_notuse );
		_ASSERT( !ret );

	}

//m_notransik
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_NOTRANSIK );
	if( checkflag == BST_CHECKED ){
		tempint = 1;
	}else if( checkflag == BST_UNCHECKED ){
		tempint = 0;
	}

	if( m_notransik != tempint ){
		editflag |= TV_EDIT_NOTRANSIK;
		m_notransik = tempint;

		m_selem->m_notransik = m_notransik;
	}

//m_ikskip
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_IKSKIP );
	if( checkflag == BST_CHECKED ){
		tempint = 1;
	}else if( checkflag == BST_UNCHECKED ){
		tempint = 0;
	}

	if( m_ikskip != tempint ){
		editflag |= TV_EDIT_IKSKIP;
		m_ikskip = tempint;

		m_selem->m_ikskip = m_ikskip;
	}

//m_invisible
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_INVISIBLE );
	if( checkflag == BST_CHECKED ){
		tempint = 1;
	}else if( checkflag == BST_UNCHECKED ){
		tempint = 0;
	}
	m_selem->invisibleflag = tempint;


//m_noskinning
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_SKINNING );
	if( checkflag == BST_CHECKED ){
		tempint = 1;
	}else{
		tempint = 0;
	}
	if( m_noskinning != tempint ){
		editflag |= TV_EDIT_NOSKINNING;
		m_noskinning = tempint;

		ret += m_selem->SetNoSkinning( m_noskinning );
		_ASSERT( !ret );
	}

//dispswitchno
	int dispcombono;
	dispcombono = (int)m_dispswitchno_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( dispcombono == CB_ERR ){
		_ASSERT( 0 );
		return 0;
	}
	tempint = dispcombono;
	if( tempint != m_dispswitchno ){
		editflag |= TV_EDIT_DISPSWITCHNO;
		m_dispswitchno = tempint;

		int ret1;
		ret1 = m_selem->SetDispSwitchNo( m_dispswitchno );
		_ASSERT( !ret1 );

	}


	//treeviewに反映
	m_stv->ChangeTVParams( editflag, m_shdname );//!!!!!!!!!!!!!!!!!!


	return 0;
}


LRESULT CShdParamDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	char errmsg[1024];

	ret = ApplyParams( errmsg );
	if( !ret ){
		EndDialog(wID);
		//DestroyWindow();
	}else{
		::MessageBox( m_hWnd, errmsg, "ダイアログのパラメータ不正", MB_OK );
	}

	return 0;
}

LRESULT CShdParamDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	//DestroyWindow();
	return 0;
}


/***
LRESULT CShdParamDlg::OnAllNoSkinning(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int ret;
	if( m_selem->m_loadbimflag != 0 ){
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"bimファイル読み込み後に、この操作を行うことは出来ません。",
			(LPCTSTR)"確認ダイアログ",
			MB_OK );
		return 0;
	}


	editflag |= TV_EDIT_ALL_NOSKINNING;
	editflag &= ~TV_EDIT_ALL_SKINNING;

	if( m_noskinning != 1 ){
		m_noskinning = 1;

		ret = m_selem->SetNoSkinning( m_noskinning );
		_ASSERT( !ret );
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_SKINNING, BST_CHECKED );
	}


	return 0;
}
	
LRESULT CShdParamDlg::OnAllSkinning(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( m_selem->m_loadbimflag != 0 ){
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"bimファイル読み込み後に、この操作を行うことは出来ません。",
			(LPCTSTR)"確認ダイアログ",
			MB_OK );
		return 0;
	}

	editflag |= TV_EDIT_ALL_SKINNING;
	editflag &= ~TV_EDIT_ALL_NOSKINNING;

	if( m_noskinning != 0 ){
		m_noskinning = 0;

		ret = m_selem->SetNoSkinning( m_noskinning );
		_ASSERT( !ret );
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_SKINNING, BST_UNCHECKED );
	}

	return 0;
}
***/

int CShdParamDlg::EnableButtons( int flag )
{
	static int disablecnt = 0;
	if( flag == 0 ){
		disablecnt++;
	}else{
		disablecnt--;
	}

	if( disablecnt == 1 ){
		m_ok_wnd.EnableWindow( FALSE );
		m_cancel_wnd.EnableWindow( FALSE );
		m_text_status_wnd.SetWindowText( "色の設定とテクスチャのダイアログを閉じるまで、ボタンは選択できません。" );
	}else if( disablecnt == 0 ){
		m_ok_wnd.EnableWindow( TRUE );
		m_cancel_wnd.EnableWindow( TRUE );		
		m_text_status_wnd.SetWindowText( "" );
	}

	return 0;
}




LRESULT CShdParamDlg::OnBefore(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_selem )
		return 0;

	int curseri;
	curseri = m_selem->serialno;

	int nextseri;

	if( m_selem->IsJoint() ){
		CMotionCtrl* curmc;
		curmc = (*m_mhandler)( curseri );
		int curboneno;
		curboneno = curmc->boneno;
		int nextboneno;
		nextboneno = curboneno - 1;

		if( nextboneno >= 0 ){
			nextseri = *(m_shandler->m_boneno2seri + nextboneno);
		}else{
			nextseri = 1;
		}
	}else{
		nextseri = curseri - 1;
		if( nextseri <= 0 ){
			nextseri = 1;
		}
	}

	if( (nextseri >= 1) && (nextseri < m_shandler->s2shd_leng) ){
		char errmsg[1024];
		ApplyParams( errmsg );

		editflag = 0;
		m_telem = (*m_thandler)( nextseri );
		m_selem = (*m_shandler)( nextseri );
		ParamsToDlg();

		m_stv->OnUserSelchange( nextseri );

	}

	return 0;
}
LRESULT CShdParamDlg::OnAfter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_selem )
		return 0;

	int curseri;
	curseri = m_selem->serialno;

	int nextseri;

	if( m_selem->IsJoint() ){
		CMotionCtrl* curmc;
		curmc = (*m_mhandler)( curseri );
		int curboneno;
		curboneno = curmc->boneno;
		int nextboneno;
		nextboneno = curboneno + 1;

		if( nextboneno < m_shandler->m_lastboneno ){
			nextseri = *(m_shandler->m_boneno2seri + nextboneno);
		}else{
			nextseri = 1;
		}
	}else{
		nextseri = curseri + 1;
		if( nextseri >= m_shandler->s2shd_leng ){
			nextseri = 1;
		}
	}

	if( (nextseri >= 1) && (nextseri < m_shandler->s2shd_leng) ){
		char errmsg[1024];
		ApplyParams( errmsg );

		editflag = 0;
		m_telem = (*m_thandler)( nextseri );
		m_selem = (*m_shandler)( nextseri );
		ParamsToDlg();

		m_stv->OnUserSelchange( nextseri );

	}

	return 0;
}

LRESULT CShdParamDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return 0;
}


