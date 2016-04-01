// ChkOWDlg.cpp : CChkOWDlg のインプリメンテーション
#include "stdafx.h"
#include "ChkOWDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CChkOWDlg

CChkOWDlg::CChkOWDlg( char* srcfname )
{

	strcpy_s( m_filename, MAX_PATH, srcfname );
	m_owkind = OW_NO;
}

CChkOWDlg::~CChkOWDlg()
{
}

LRESULT CChkOWDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CChkOWDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	m_owkind = OW_YES;
	EndDialog(wID);
	return 0;
}

LRESULT CChkOWDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_owkind = OW_NO;
	EndDialog(wID);
	return 0;
}
LRESULT CChkOWDlg::OnAllYes(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_owkind = OW_ALLYES;
	EndDialog(wID);
	return 0;
}
LRESULT CChkOWDlg::OnAllNo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_owkind = OW_ALLNO;
	EndDialog(wID);
	return 0;
}


int CChkOWDlg::SetWnd()
{

	m_filename_wnd = GetDlgItem( IDC_FILENAME );

	return 0;
}

int CChkOWDlg::ParamsToDlg()
{
	m_filename_wnd.SetWindowText( m_filename );
	
	return 0;
}

