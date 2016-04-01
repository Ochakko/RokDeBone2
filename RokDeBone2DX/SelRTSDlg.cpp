// SelRTSDlg.cpp : CSelRTSDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include <coef.h>
#include "SelRTSDlg.h"

#define DBGH
#include <dbg.h>

/////////////////////////////////////////////////////////////////////////////
// CSelRTSDlg

CSelRTSDlg::CSelRTSDlg( int mkind )
{

	if( mkind == MKIND_ROT ){
		m_rot = 1;
		m_mv = 0;
		m_sc = 0;
	}else if( mkind == MKIND_MV ){
		m_rot = 0;
		m_mv = 1;
		m_sc = 0;
	}else if( mkind == MKIND_SC ){
		m_rot = 0;
		m_mv = 0;
		m_sc = 1;
	}

}

CSelRTSDlg::~CSelRTSDlg()
{
}

LRESULT CSelRTSDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSelRTSDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_ROT );
	if( checkflag == BST_CHECKED ){
		m_rot = 1;
	}else{
		m_rot = 0;
	}

	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_MV );
	if( checkflag == BST_CHECKED ){
		m_mv = 1;
	}else{
		m_mv = 0;
	}

	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_SC );
	if( checkflag == BST_CHECKED ){
		m_sc = 1;
	}else{
		m_sc = 0;
	}

	EndDialog(wID);

	return 0;
}

LRESULT CSelRTSDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);

	return 0;
}

void CSelRTSDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
}

int CSelRTSDlg::ParamsToDlg()
{		
	if( m_rot == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_ROT, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_ROT, BST_UNCHECKED );
	}

	if( m_mv == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_MV, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_MV, BST_UNCHECKED );
	}

	if( m_sc == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_SC, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_SC, BST_UNCHECKED );
	}


	return 0;
}

