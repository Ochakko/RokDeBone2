// MotCutDlg.cpp : CMotCutDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "MotCutDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CMotCutDlg

CMotCutDlg::CMotCutDlg( int maxframe )
{
	m_chkfirst = 0;
	m_chklast = 0;
	m_firstframe = 0;
	m_maxframe = maxframe;
	m_lastframe = maxframe;
}

CMotCutDlg::~CMotCutDlg()
{
}

LRESULT CMotCutDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CMotCutDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;

	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKFIRST );
	if( ischecked == BST_CHECKED ){
		m_chkfirst = 1;

		int tmpint;
		ret = GetInt( m_firstframe_wnd, &tmpint );
		if( ret || (tmpint < 0) ){
			::MessageBoxA( m_hWnd, "削除フレーム（先頭）の数値が不正です。", "エラー", MB_OK );
			return 0;
		}
		m_firstframe = tmpint;

	}else{
		m_chkfirst = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKLAST );
	if( ischecked == BST_CHECKED ){
		m_chklast = 1;

		int tmpint;
		ret = GetInt( m_lastframe_wnd, &tmpint );
		if( ret || (tmpint > m_maxframe) ){
			::MessageBoxA( m_hWnd, "削除フレーム（末尾）の数値が不正です。", "エラー", MB_OK );
			return 0;
		}
		m_lastframe = tmpint;

	}else{
		m_chklast = 0;
	}

	EndDialog(wID);

	return 0;
}

LRESULT CMotCutDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CMotCutDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	m_chkfirst_wnd = GetDlgItem( IDC_CHKFIRST );
	m_chklast_wnd = GetDlgItem( IDC_CHKLAST );
	m_firstframe_wnd = GetDlgItem( IDC_FIRSTFRAME );
	m_lastframe_wnd = GetDlgItem( IDC_LASTFRAME );
}

int CMotCutDlg::ParamsToDlg()
{	
	if( m_chkfirst ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKFIRST, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKFIRST, BST_UNCHECKED );
	}
	if( m_chklast ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKLAST, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKLAST, BST_UNCHECKED );
	}

	char strframe[256];
	sprintf_s( strframe, 256, "%d", m_firstframe );
	m_firstframe_wnd.SetWindowText( strframe );

	sprintf_s( strframe, 256, "%d", m_lastframe );
	m_lastframe_wnd.SetWindowText( strframe );

	return 0;
}

