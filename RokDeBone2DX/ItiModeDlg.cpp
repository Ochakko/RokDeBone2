// ItiModeDlg.cpp : CItiModeDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "ItiModeDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>


/////////////////////////////////////////////////////////////////////////////
// CItiModeDlg

CItiModeDlg::CItiModeDlg()
{
	m_itimode = 0;
}

CItiModeDlg::~CItiModeDlg()
{
}

LRESULT CItiModeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	if( m_itimode == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_MODE_INIT, IDC_MODE_NOTINIT, IDC_MODE_INIT );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_MODE_INIT, IDC_MODE_NOTINIT, IDC_MODE_NOTINIT );
	}
	
	return 1;  // システムにフォーカスを設定させます
}

LRESULT CItiModeDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_MODE_INIT );
	if( checkflag == BST_CHECKED ){
		m_itimode = 0;
	}else if( checkflag == BST_UNCHECKED ){
		m_itimode = 1;
	}

////////

	EndDialog(wID);
	return 0;
}
/***
LRESULT CItiModeDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}
***/

void CItiModeDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

}

