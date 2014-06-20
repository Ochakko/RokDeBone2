// SelBoneDlg.cpp : CSelBoneDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "SelBoneDlg.h"

#define DBGH
#include <dbg.h>

/////////////////////////////////////////////////////////////////////////////
// CSelBoneDlg

CSelBoneDlg::CSelBoneDlg()
{
	m_doall = 1;
}

CSelBoneDlg::~CSelBoneDlg()
{
}

LRESULT CSelBoneDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSelBoneDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_ALLBONE );
	if( checkflag == BST_CHECKED ){
		m_doall = 1;
	}else{
		m_doall = 0;
	}

	EndDialog(wID);

	return 0;
}

LRESULT CSelBoneDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);

	return 0;
}

void CSelBoneDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
}

int CSelBoneDlg::ParamsToDlg()
{		
	if( m_doall == 1 ){
		m_dlg_wnd.CheckRadioButton( IDC_ALLBONE, IDC_ONEBONE, IDC_ALLBONE );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_ALLBONE, IDC_ONEBONE, IDC_ONEBONE );
	}

	return 0;
}

