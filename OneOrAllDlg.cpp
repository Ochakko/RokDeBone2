// OneOrAllDlg.cpp : COneOrAllDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "OneOrAllDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>


/////////////////////////////////////////////////////////////////////////////
// COneOrAllDlg

COneOrAllDlg::COneOrAllDlg()
{
	m_isone = 1;
}

COneOrAllDlg::~COneOrAllDlg()
{
}

LRESULT COneOrAllDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT COneOrAllDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_ONE );
	if( checkflag == BST_CHECKED ){
		m_isone = 1;
	}else{
		m_isone = 0;
	}

	EndDialog(wID);

	return 0;
}

LRESULT COneOrAllDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void COneOrAllDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

}

int COneOrAllDlg::ParamsToDlg()
{	
	if( m_isone ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ALL, IDC_RADIO_ONE, IDC_RADIO_ONE );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ALL, IDC_RADIO_ONE, IDC_RADIO_ALL );
	}

	return 0;
}
