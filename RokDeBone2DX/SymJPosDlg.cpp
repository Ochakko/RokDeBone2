// SymJPosDlg.cpp : CSymJPosDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "SymJPosDlg.h"

//#include <Commdlg.h>
//#include <Afxdlgs.h>


/////////////////////////////////////////////////////////////////////////////
// CSymJPosDlg

CSymJPosDlg::CSymJPosDlg()
{
	InitParams();
}

CSymJPosDlg::~CSymJPosDlg()
{
}

int CSymJPosDlg::InitParams()
{
	m_createtype = CREATE_MINUS;

	return 0;
}

LRESULT CSymJPosDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSymJPosDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CREATEM );
	if( checkflag == BST_CHECKED ){
		m_createtype = CREATE_MINUS;
	}else if( checkflag == BST_UNCHECKED ){
		m_createtype = CREATE_PLUS;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CSymJPosDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CSymJPosDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

}

int CSymJPosDlg::ParamsToDlg()
{	

//radio	
	if( m_createtype == CREATE_MINUS ){
		m_dlg_wnd.CheckRadioButton( IDC_CREATEM, IDC_CREATEP, IDC_CREATEM );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_CREATEM, IDC_CREATEP, IDC_CREATEP );
	}

	return 0;
}

