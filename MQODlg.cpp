// MQODLG.cpp : CMQODlg のインプリメンテーション
#include "stdafx.h"
#include "MQODlg.h"
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CMQODlg

CMQODlg::CMQODlg( float srcmult, int srctype )
{
	m_mult = srcmult;
	m_bonetype = srctype;

	//m_bonetype = BONETYPE_RDB2;
}

CMQODlg::~CMQODlg()
{
}


LRESULT CMQODlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	char tempchar[1024];
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%f", m_mult );
	m_mult_wnd.SetWindowText( tempchar );

	if( m_bonetype == BONETYPE_RDB2 ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_RDB2, IDC_RADIO_MIKO, IDC_RADIO_RDB2 );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_RDB2, IDC_RADIO_MIKO, IDC_RADIO_MIKO );
	}


	//m_radio_miko_wnd.EnableWindow( FALSE );


	return 1;  // システムにフォーカスを設定させます
}

int CMQODlg::SetWnd()
{
	HWND	temphwnd;
	temphwnd = GetDlgItem( IDC_EDIT_MULT );
	m_mult_wnd = temphwnd;


	m_radio_rdb2_wnd = GetDlgItem( IDC_RADIO_RDB2 );
	m_radio_miko_wnd = GetDlgItem( IDC_RADIO_MIKO );


	m_dlg_wnd = m_hWnd;

	return 0;
}

LRESULT CMQODlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	char errmsg[1024];
	ZeroMemory( errmsg, 1024 );

	//m_mult
	float tempfloat;
	ret += GetFloat( m_mult_wnd, &tempfloat );
	if( ret ){
		strcpy_s( errmsg, 1024, "倍率の値が不正です。float" );
		goto endofonok;
	}
	m_mult = tempfloat;

	//
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_RDB2 );
	if( checkflag == BST_CHECKED ){
		m_bonetype = BONETYPE_RDB2;
	}else if( checkflag == BST_UNCHECKED ){
		m_bonetype = BONETYPE_MIKO;
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

LRESULT CMQODlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

