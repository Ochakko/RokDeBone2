// CreateKeyDlg.cpp : CCreateKeyDlg のインプリメンテーション
#include "stdafx.h"
#include "CreateKeyDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CCreateKeyDlg

CCreateKeyDlg::CCreateKeyDlg()
{
	m_create = 0;
}

CCreateKeyDlg::~CCreateKeyDlg()
{
}

LRESULT CCreateKeyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	_ASSERT( !ret );


	if( m_create == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_CREATE0, IDC_CREATE1, IDC_CREATE0 );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_CREATE0, IDC_CREATE1, IDC_CREATE1 );
	}

	return 1;  // システムにフォーカスを設定させます
}

int CCreateKeyDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	return 0;
}

LRESULT CCreateKeyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CREATE0 );
	if( ischecked == BST_CHECKED ){
		m_create = 0;
	}else{
		m_create = 1;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CCreateKeyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

