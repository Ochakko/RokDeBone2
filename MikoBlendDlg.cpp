// MikoBlendDlg.cpp : CMikoBlendDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "MikoBlendDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>


/////////////////////////////////////////////////////////////////////////////
// CMikoBlendDlg

CMikoBlendDlg::CMikoBlendDlg()
{
	InitParams();
}

CMikoBlendDlg::~CMikoBlendDlg()
{
}

int CMikoBlendDlg::InitParams()
{
	m_mikoblendtype = MIKOBLEND_MIX;

	return 0;
}

LRESULT CMikoBlendDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CMikoBlendDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_SKINNING );
	if( checkflag == BST_CHECKED ){
		m_mikoblendtype = MIKOBLEND_SKINNING;
	}else if( checkflag == BST_UNCHECKED ){
		m_mikoblendtype = MIKOBLEND_MIX;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CMikoBlendDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CMikoBlendDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

}

int CMikoBlendDlg::ParamsToDlg()
{	

//radio	
	if( m_mikoblendtype == MIKOBLEND_SKINNING ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SKINNING, IDC_RADIO_MIX, IDC_RADIO_SKINNING );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SKINNING, IDC_RADIO_MIX, IDC_RADIO_MIX );
	}

	return 0;
}

