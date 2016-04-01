// PasteTypeDlg.cpp : CPasteTypeDlg のインプリメンテーション
#include "stdafx.h"
#include "PasteTypeDlg.h"

#include <stdlib.h>
#include <coef.h>

/////////////////////////////////////////////////////////////////////////////
// CPasteTypeDlg
CPasteTypeDlg::CPasteTypeDlg()
{
	m_pastetype = 0;
}

CPasteTypeDlg::~CPasteTypeDlg()
{
}

LRESULT CPasteTypeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	_ASSERT( !ret );
	ret = Params2Dlg();
	_ASSERT( !ret );

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CPasteTypeDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO1 );
	if( ischecked == BST_CHECKED ){
		m_pastetype = 0;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO2 );
	if( ischecked == BST_CHECKED ){
		m_pastetype = 1;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CPasteTypeDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CPasteTypeDlg::SetWnd()
{
	HWND temphwnd;

	m_dlg_wnd = m_hWnd;

	temphwnd = GetDlgItem( IDC_RADIO1 );
	m_radio1_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_RADIO2 );
	m_radio2_wnd = temphwnd;


	return 0;
}

int CPasteTypeDlg::Params2Dlg()
{
	switch( m_pastetype ){
	case 0:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO1 );
		break;
	case 1:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO2 );
		break;
	default:
		_ASSERT( 0 );
		m_dlg_wnd.CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO1 );
		m_pastetype = 0;
		break;
	}

	return 0;
}
