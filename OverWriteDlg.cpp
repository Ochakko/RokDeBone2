// OverWriteDlg.cpp : COverWriteDlg のインプリメンテーション
#include "stdafx.h"
#include "OverWriteDlg.h"

#include "GetDlgParams.h"
#include "LoadNameDlg.h"

/////////////////////////////////////////////////////////////////////////////
// COverWriteDlg

COverWriteDlg::COverWriteDlg( CLoadNameDlg* namedlg )
{
	m_namedlg = namedlg;

	m_check_sig = 0;
	m_check_moa = 0;
}

COverWriteDlg::~COverWriteDlg()
{
}

LRESULT COverWriteDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	if( *m_namedlg->m_signame ){
		char sigpat[10];
		strcpy_s( sigpat, 10, ".sig" );
		char* findpat = 0;
		findpat = strstr( m_namedlg->m_signame, sigpat );
		if( findpat ){
			m_check_sig = 1;
			m_check_sig_wnd.EnableWindow( TRUE );
		}else{
			m_check_sig = 0;
			m_check_sig_wnd.EnableWindow( FALSE );
		}

	}else{
		m_check_sig = 0;
		m_check_sig_wnd.EnableWindow( FALSE );
	}

	if( *m_namedlg->m_moaname ){
		m_check_moa = 1;
		m_check_moa_wnd.EnableWindow( TRUE );
	}else{
		m_check_moa = 0;
		m_check_moa_wnd.EnableWindow( FALSE );
	}

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT COverWriteDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_OW_SIG );
	if( checkflag == BST_CHECKED ){
		m_check_sig = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_check_sig = 0;
	}
	

	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_OW_MOA );
	if( checkflag == BST_CHECKED ){
		m_check_moa = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_check_moa = 0;
	}


	EndDialog(wID);
	return 0;
}

LRESULT COverWriteDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	EndDialog(wID);
	return 0;
}

int COverWriteDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_check_sig_wnd = GetDlgItem( IDC_OW_SIG );
	m_check_moa_wnd = GetDlgItem( IDC_OW_MOA );

	m_text_sig_wnd = GetDlgItem( IDC_OWSIGNAME );
	m_text_moa_wnd = GetDlgItem( IDC_OWMOANAME );

	return 0;
}

int COverWriteDlg::ParamsToDlg()
{
	if( m_check_sig ){
		m_dlg_wnd.CheckDlgButton( IDC_OW_SIG, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_OW_SIG, BST_UNCHECKED );
	}
	m_text_sig_wnd.SetWindowText( m_namedlg->m_signame );


	if( m_check_moa ){
		m_dlg_wnd.CheckDlgButton( IDC_OW_MOA, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_OW_MOA, BST_UNCHECKED );
	}
	m_text_moa_wnd.SetWindowText( m_namedlg->m_moaname );
	
	return 0;
}

LRESULT COverWriteDlg::CheckOWSig(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_check_sig ){
		m_check_sig = 0;
	}else{
		m_check_sig = 1;
	}
	return 0;
}
LRESULT COverWriteDlg::CheckOWMoa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_check_moa ){
		m_check_moa = 0;
	}else{
		m_check_moa = 1;
	}

	return 0;
}


