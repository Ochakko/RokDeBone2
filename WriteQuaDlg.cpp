// WriteQuaDlg.cpp : CWriteQuaDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "WriteQuaDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>
#include "GetDlgParams.h"


/////////////////////////////////////////////////////////////////////////////
// CWriteQuaDlg

CWriteQuaDlg::CWriteQuaDlg( int maxframe, char* srcfilename )
{
	ZeroMemory( m_name, _MAX_PATH );
	if( srcfilename && *srcfilename ){
		strcpy_s( m_name, _MAX_PATH, srcfilename );
	}
	m_start = 0;
	m_end = maxframe;
	m_create = 0;
}

CWriteQuaDlg::~CWriteQuaDlg()
{
}

LRESULT CWriteQuaDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteQuaDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	int tempint;

	ret = GetInt( m_edit_start_wnd, &tempint );
	if( ret ){
		MessageBox( "開始フレーム番号が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_start = tempint;

	ret = GetInt( m_edit_end_wnd, &tempint );
	if( ret ){
		MessageBox( "終了フレーム番号が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_end = tempint;

	if( m_start > m_end ){
		MessageBox( "開始フレーム番号が、終了フレーム番号よりも、大きいです。", "入力エラー", MB_OK );
		return 0;
	}

/////

	char tempchar[_MAX_PATH];
	ret = GetName( m_name_wnd, tempchar );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "quaのファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( m_name, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( m_name, _MAX_PATH, tempchar );
	}

/////
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_CREATE );
	if( checkflag == BST_CHECKED ){
		m_create = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_create = 0;
	}


	
	EndDialog(wID);

	return 0;
}

LRESULT CWriteQuaDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CWriteQuaDlg::SetWnd()
{

	m_dlg_wnd = m_hWnd;

	m_name_wnd = GetDlgItem( IDC_EDIT_NAME );
	m_edit_start_wnd = GetDlgItem( IDC_EDIT_QUASTART );
	m_edit_end_wnd = GetDlgItem( IDC_EDIT_QUAEND );
	m_check_create_wnd = GetDlgItem( IDC_CHECK_CREATE );
}

int CWriteQuaDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( m_name );


	char tempchar[256];

	sprintf_s( tempchar, 256, "%d", m_start );
	m_edit_start_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%d", m_end );
	m_edit_end_wnd.SetWindowText( tempchar );

	if( m_create ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_CREATE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_CREATE, BST_UNCHECKED );
	}
	
	return 0;
}

LRESULT CWriteQuaDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.qua" );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"QUA FILE (*.qua)\0*.qua\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = NULL;
	ofn.lpstrDefExt = "qua";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( m_name, _MAX_PATH );
	strcpy_s( m_name, _MAX_PATH, buf );
	m_name_wnd.SetWindowText( buf );

	return 0;
}

