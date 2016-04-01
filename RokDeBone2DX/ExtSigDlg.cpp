// ExtSigDlg.cpp : CExtSigDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "ExtSigDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CExtSigDlg

CExtSigDlg::CExtSigDlg()
{
	ZeroMemory( name, _MAX_PATH );
	chkbone = 1;
	chkei1 = 1;
	chkei2 = 1;
	chkei3 = 1;
	chkds = 1;
	chknotjpos = 0;
	mult = 1.0f;
}

CExtSigDlg::~CExtSigDlg()
{
}

LRESULT CExtSigDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CExtSigDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	char tempchar[_MAX_PATH];
	ret = GetName( m_name_wnd, tempchar );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "sigのファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( name, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( name, _MAX_PATH, tempchar );
	}

	float tmpfloat = 1.0f;
	ret = GetFloat( m_mult_wnd, &tmpfloat );
	if( ret ){
		MessageBox( "倍率が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	mult = tmpfloat;


	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKBONE );
	if( ischecked == BST_CHECKED ){
		chkbone = 1;
	}else{
		chkbone = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKJPOS );
	if( ischecked == BST_CHECKED ){
		chknotjpos = 1;
	}else{
		chknotjpos = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKEI1 );
	if( ischecked == BST_CHECKED ){
		chkei1 = 1;
	}else{
		chkei1 = 0;
	}
	
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKEI2 );
	if( ischecked == BST_CHECKED ){
		chkei2 = 1;
	}else{
		chkei2 = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKEI3 );
	if( ischecked == BST_CHECKED ){
		chkei3 = 1;
	}else{
		chkei3 = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKDS );
	if( ischecked == BST_CHECKED ){
		chkds = 1;
	}else{
		chkds = 0;
	}


	EndDialog(wID);

	return 0;
}

LRESULT CExtSigDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CExtSigDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	m_name_wnd = GetDlgItem( IDC_EXTSIGNAME );
	m_mult_wnd = GetDlgItem( IDC_MULT );
}

int CExtSigDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( name );

	if( chkbone ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKBONE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKBONE, BST_UNCHECKED );
	}

	if( chknotjpos ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKJPOS, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKJPOS, BST_UNCHECKED );
	}


	if( chkei1 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKEI1, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKEI1, BST_UNCHECKED );
	}

	if( chkei2 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKEI2, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKEI2, BST_UNCHECKED );
	}

	if( chkei3 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKEI3, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKEI3, BST_UNCHECKED );
	}


	if( chkds ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKDS, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKDS, BST_UNCHECKED );
	}


	char strmult[256];
	sprintf_s( strmult, 256, "%f", mult );
	m_mult_wnd.SetWindowText( strmult );

	return 0;
}

LRESULT CExtSigDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"SIG FILE (*.sig)\0*.sig\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( name, _MAX_PATH );
	strcpy_s( name, _MAX_PATH, buf );
	m_name_wnd.SetWindowText( buf );

	return 0;
}


