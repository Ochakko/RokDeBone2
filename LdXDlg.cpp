// LdXDlg.cpp : CLdXDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LdXDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>
#include "GetDlgParams.h"


/////////////////////////////////////////////////////////////////////////////
// CLdXDlg

CLdXDlg::CLdXDlg( char* srcfilename )
{
	ZeroMemory( name, _MAX_PATH );
	if( srcfilename && *srcfilename ){
		strcpy_s( name, _MAX_PATH, srcfilename );
	}
	mult = 1.0f;
}

CLdXDlg::~CLdXDlg()
{
}

LRESULT CLdXDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CLdXDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
	ret = GetName( m_name_wnd, tempchar );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "Xのファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( name, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( name, _MAX_PATH, tempchar );
	}

	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CLdXDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLdXDlg::SetWnd()
{
	HWND temphwnd;

	temphwnd = GetDlgItem( IDC_SIGNAME );
	m_name_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_MULT );
	m_mult_wnd = temphwnd;

}

int CLdXDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( name );


	char tempchar[256];

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", mult );
	m_mult_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CLdXDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"X FILE (*.x)\0*.x\0";
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


