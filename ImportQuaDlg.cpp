// ImportQuaDlg.cpp : CImportQuaDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "ImportQuaDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CImportQuaDlg

CImportQuaDlg::CImportQuaDlg()
{
	ZeroMemory( m_name, _MAX_PATH );
	m_space = 1;
}

CImportQuaDlg::~CImportQuaDlg()
{
}

LRESULT CImportQuaDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CImportQuaDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	int tempint;

	ret = GetInt( m_edit_space_wnd, &tempint );
	if( ret || (tempint < 1) || (tempint > 100) ){
		MessageBox( "間隔フレーム数が不正です。（１から１００の整数）", "入力エラー", MB_OK );
		return 0;
	}
	m_space = tempint;

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
	
	EndDialog(wID);

	return 0;
}

LRESULT CImportQuaDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CImportQuaDlg::SetWnd()
{

	m_dlg_wnd = m_hWnd;

	m_name_wnd = GetDlgItem( IDC_EDIT_NAME );
	m_edit_space_wnd = GetDlgItem( IDC_EDIT_SPACE );
}

int CImportQuaDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( m_name );


	char tempchar[256];

	sprintf_s( tempchar, 256, "%d", m_space );
	m_edit_space_wnd.SetWindowText( tempchar );
	
	return 0;
}

LRESULT CImportQuaDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
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
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = NULL;
	ofn.lpstrDefExt = "qua";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( m_name, _MAX_PATH );
	strcpy_s( m_name, _MAX_PATH, buf );
	m_name_wnd.SetWindowText( buf );

	return 0;
}

