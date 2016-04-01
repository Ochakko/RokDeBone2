// LOADPARAMDLG.cpp : CLoadParamDlg のインプリメンテーション
#include "stdafx.h"
#include "LoadParamDlg.h"
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadParamDlg

CLoadParamDlg::CLoadParamDlg( CVec3f minvec, CVec3f maxvec )
{
	//m_multiple = 650.0f;
	m_multiple = 1997.0f;
	m_swapxz = 0;
	m_centerx = 0;
	m_centerz = 0;

	m_invz = 0;
	m_divmode = 1;

	m_chkinv = 0;

	m_minvec = minvec;
	m_maxvec = maxvec;
}

CLoadParamDlg::~CLoadParamDlg()
{
}


LRESULT CLoadParamDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	char tempchar[1024];
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%f", m_multiple );
	m_multiple_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "Xの範囲 : %.5f ～ %.5f", m_minvec.x, m_maxvec.x );
	m_rangex_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "Yの範囲 : %.5f ～ %.5f", m_minvec.y, m_maxvec.y );
	m_rangey_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "Zの範囲 : %.5f ～ %.5f", m_minvec.z, m_maxvec.z );
	m_rangez_wnd.SetWindowText( tempchar );


	if( m_divmode == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_DIV_COLOR, IDC_DIV_GROUP, IDC_DIV_COLOR );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_DIV_COLOR, IDC_DIV_GROUP, IDC_DIV_GROUP );
	}

	return 1;  // システムにフォーカスを設定させます
}

int CLoadParamDlg::SetWnd()
{
	HWND	temphwnd;
	temphwnd = GetDlgItem( IDC_MULTIPLE );
	m_multiple_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SWAPXZ );
	m_swapxz_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_CENTERX );
	m_centerx_wnd  = temphwnd;

	temphwnd = GetDlgItem( IDC_CENTERZ );
	m_centerz_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_INVZ );
	m_invz_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_DIV_COLOR );
	m_div_color_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_DIV_GROUP );
	m_div_group_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_CHKINV );
	m_chkinv_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RANGEX );
	m_rangex_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RANGEY );
	m_rangey_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RANGEZ );
	m_rangez_wnd = temphwnd;

	m_dlg_wnd = m_hWnd;

	return 0;
}

LRESULT CLoadParamDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	char errmsg[1024];
	ZeroMemory( errmsg, 1024 );

	//m_multiple
	float tempfloat;
	ret += GetFloat( m_multiple_wnd, &tempfloat );
	if( ret ){
		strcpy_s( errmsg, 1024, "倍率の値が不正です。float" );
		goto endofonok;
	}
	m_multiple = tempfloat;

	//m_swapxz
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_SWAPXZ );
	if( checkflag == BST_CHECKED ){
		m_swapxz = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_swapxz = 0;
	}
	
	//m_centerx
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CENTERX );
	if( checkflag == BST_CHECKED ){
		m_centerx = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_centerx = 0;
	}

	//m_centerz
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CENTERZ );
	if( checkflag == BST_CHECKED ){
		m_centerz = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_centerz = 0;
	}
	
	//m_invz
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_INVZ );
	if( checkflag == BST_CHECKED ){
		m_invz = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_invz = 0;
	}

	//m_divmode
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_DIV_COLOR );
	if( checkflag == BST_CHECKED ){
		m_divmode = 0;
	}else{
		m_divmode = 1;
	}

	//m_chkinv
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKINV );
	if( checkflag == BST_CHECKED ){
		m_chkinv = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_chkinv = 0;
	}

	goto endofonok;
endofonok:
	if( !ret ){
		EndDialog(wID);
		//DestroyWindow();
	}else{
		::MessageBox( m_hWnd, errmsg, "ダイアログのパラメータ不正", MB_OK );
	}
	return 0;
}

LRESULT CLoadParamDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

