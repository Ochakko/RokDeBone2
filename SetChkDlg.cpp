// SetChkDlg.cpp : CSetChkDlg のインプリメンテーション
#include "stdafx.h"
#include "SetChkDlg.h"

#include <coef.h>
#include "resid.h"

#define DBGH
#include <dbg.h>

/////////////////////////////////////////////////////////////////////////////
// CSetChkDlg
CSetChkDlg::CSetChkDlg( int undodlg, int rdlg, int infdlg, int modlg )
{
	m_undodlg = undodlg;
	m_rdlg = rdlg;
	m_infdlg = infdlg;
	m_modlg = modlg;
}

CSetChkDlg::~CSetChkDlg()
{
}


LRESULT CSetChkDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	m_dlg_wnd = m_hWnd;

	if( m_undodlg == CHKDLG_NES ){
		m_dlg_wnd.CheckRadioButton( IDC_UNDO_NES, IDC_UNDO_YES, IDC_UNDO_NES );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_UNDO_NES, IDC_UNDO_YES, IDC_UNDO_YES );
	}

	if( m_rdlg == CHKDLG_NES ){
		m_dlg_wnd.CheckRadioButton( IDC_R_NES, IDC_R_YES, IDC_R_NES );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_R_NES, IDC_R_YES, IDC_R_YES );
	}

	if( m_infdlg == CHKDLG_NES ){
		m_dlg_wnd.CheckRadioButton( IDC_INF_NES, IDC_INF_NO, IDC_INF_NES );
	}else if( m_infdlg == CHKDLG_YES ){
		m_dlg_wnd.CheckRadioButton( IDC_INF_NES, IDC_INF_NO, IDC_INF_YES );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_INF_NES, IDC_INF_NO, IDC_INF_NO );
	}

	if( m_modlg == CHKDLG_NES ){
		m_dlg_wnd.CheckRadioButton( IDC_MO_NES, IDC_MO_NO, IDC_MO_NES );
	}else if( m_modlg == CHKDLG_YES ){
		m_dlg_wnd.CheckRadioButton( IDC_MO_NES, IDC_MO_NO, IDC_MO_YES );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_MO_NES, IDC_MO_NO, IDC_MO_NO );
	}



	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSetChkDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT chknes, chkyes;
	chknes = m_dlg_wnd.IsDlgButtonChecked( IDC_UNDO_NES );
	if( chknes == BST_CHECKED ){
		m_undodlg = CHKDLG_NES;
	}else{
		m_undodlg = CHKDLG_YES;
	}

	chknes = m_dlg_wnd.IsDlgButtonChecked( IDC_R_NES );
	if( chknes == BST_CHECKED ){
		m_rdlg = CHKDLG_NES;
	}else{
		m_rdlg = CHKDLG_YES;
	}

	chknes = m_dlg_wnd.IsDlgButtonChecked( IDC_INF_NES );
	if( chknes == BST_CHECKED ){
		m_infdlg = CHKDLG_NES;
	}else{
		chkyes = m_dlg_wnd.IsDlgButtonChecked( IDC_INF_YES );
		if( chkyes == BST_CHECKED ){
			m_infdlg = CHKDLG_YES;
		}else{
			m_infdlg = CHKDLG_NO;
		}
	}

	chknes = m_dlg_wnd.IsDlgButtonChecked( IDC_MO_NES );
	if( chknes == BST_CHECKED ){
		m_modlg = CHKDLG_NES;
	}else{
		chkyes = m_dlg_wnd.IsDlgButtonChecked( IDC_MO_YES );
		if( chkyes == BST_CHECKED ){
			m_modlg = CHKDLG_YES;
		}else{
			m_modlg = CHKDLG_NO;
		}
	}

	EndDialog(wID);
	return 0;
}

LRESULT CSetChkDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

