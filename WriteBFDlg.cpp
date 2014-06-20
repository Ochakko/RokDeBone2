// WriteBFDlg.cpp : CWriteBFDlg のインプリメンテーション
#include "stdafx.h"
#include "WriteBFDlg.h"

#include <stdlib.h>
#include <coef.h>

/////////////////////////////////////////////////////////////////////////////
// CWriteBFDlg
CWriteBFDlg::CWriteBFDlg()
{
	m_bftype = 1;
	m_relativeflag = 0;
	m_mqoflag = 0;
}

CWriteBFDlg::~CWriteBFDlg()
{
}

LRESULT CWriteBFDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	_ASSERT( !ret );
	ret = Params2Dlg();
	_ASSERT( !ret );

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteBFDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO1 );
	if( ischecked == BST_CHECKED ){
		m_bftype = 0;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO2 );
	if( ischecked == BST_CHECKED ){
		m_bftype = 1;
	}


	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK1 );
	if( checkflag == BST_CHECKED ){
		m_relativeflag = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_relativeflag = 0;
	}

	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_MQO );
	if( checkflag == BST_CHECKED ){
		m_mqoflag = 1;
		m_relativeflag = 1;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}else if( checkflag == BST_UNCHECKED ){
		m_mqoflag = 0;
	}


	EndDialog(wID);
	return 0;
}

LRESULT CWriteBFDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CWriteBFDlg::SetWnd()
{
	HWND temphwnd;

	m_dlg_wnd = m_hWnd;

	temphwnd = GetDlgItem( IDC_RADIO1 );
	m_radio1_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RADIO2 );
	m_radio2_wnd = temphwnd;

	m_rel_wnd = GetDlgItem( IDC_CHECK1 );

	m_mqo_wnd = GetDlgItem( IDC_MQO );

	return 0;
}

int CWriteBFDlg::Params2Dlg()
{
	switch( m_bftype ){
	case 0:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO1 );
		break;
	case 1:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO2 );
		break;
	default:
		_ASSERT( 0 );
		m_dlg_wnd.CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO1 );
		m_bftype = 0;
		break;
	}

	
	if( m_relativeflag == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK1, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK1, BST_UNCHECKED );
	}

	if( m_mqoflag == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_MQO, BST_CHECKED );

		m_relativeflag = 1;
		m_dlg_wnd.CheckDlgButton( IDC_CHECK1, BST_CHECKED );
		m_rel_wnd.EnableWindow( FALSE );

	}else{
		m_dlg_wnd.CheckDlgButton( IDC_MQO, BST_UNCHECKED );

		m_rel_wnd.EnableWindow( TRUE );
	}


	return 0;
}

LRESULT CWriteBFDlg::OnRelative(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK1 );
	if( checkflag == BST_CHECKED ){
		m_relativeflag = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_relativeflag = 0;
	}

	return 0;
}
LRESULT CWriteBFDlg::OnMQO(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_MQO );
	if( checkflag == BST_CHECKED ){
		m_mqoflag = 1;

		m_relativeflag = 1;
		m_dlg_wnd.CheckDlgButton( IDC_CHECK1, BST_CHECKED );
		m_rel_wnd.EnableWindow( FALSE );

	}else if( checkflag == BST_UNCHECKED ){
		m_mqoflag = 0;
		m_rel_wnd.EnableWindow( TRUE );
	}

	return 0;
}

