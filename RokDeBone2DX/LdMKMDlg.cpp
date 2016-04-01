// LdMkmDlg.cpp : CLdMkmDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LdMkmDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLdMkmDlg

CLdMkmDlg::CLdMkmDlg()
{
	ZeroMemory( mkmname, _MAX_PATH );
	ZeroMemory( initname, _MAX_PATH );
	mult = 1.0f;
}

CLdMkmDlg::~CLdMkmDlg()
{
}

LRESULT CLdMkmDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CLdMkmDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	float tempfloat = 0.0f;
	ret = GetFloat( m_mult_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	mult = tempfloat;
/////

	char tempchar[_MAX_PATH];
	ret = GetName( m_mkmname_wnd, tempchar );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "モーションmkmのファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( mkmname, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( mkmname, _MAX_PATH, tempchar );
	}


	ret = GetName( m_initname_wnd, tempchar );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "初期モーションmkmのファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( initname, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( initname, _MAX_PATH, tempchar );
	}


	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CLdMkmDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLdMkmDlg::SetWnd()
{

	m_mkmname_wnd = GetDlgItem( IDC_MKMNAME );
	m_initname_wnd = GetDlgItem( IDC_INITNAME );
	m_mult_wnd = GetDlgItem( IDC_MULT );

}

int CLdMkmDlg::ParamsToDlg()
{	
	m_mkmname_wnd.SetWindowText( mkmname );
	m_initname_wnd.SetWindowText( initname );


	char tempchar[256];
	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", mult );
	m_mult_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CLdMkmDlg::OnRefMkmName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"mkm FILE (*.mkm)\0*.mkm\0";
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

	ZeroMemory( mkmname, _MAX_PATH );
	strcpy_s( mkmname, _MAX_PATH, buf );
	m_mkmname_wnd.SetWindowText( buf );

	return 0;
}

LRESULT CLdMkmDlg::OnRefInitName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"mkm FILE (*.mkm)\0*.mkm\0";
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

	ZeroMemory( initname, _MAX_PATH );
	strcpy_s( initname, _MAX_PATH, buf );
	m_initname_wnd.SetWindowText( buf );

	return 0;
}

