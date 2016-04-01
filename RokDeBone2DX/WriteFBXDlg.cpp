// WriteFBXDlg.cpp : CWriteFBXDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "WriteFBXDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CWriteFBXDlg

CWriteFBXDlg::CWriteFBXDlg()
{
	ZeroMemory( name, MAX_PATH );
	mult = 1.0f;
}

CWriteFBXDlg::~CWriteFBXDlg()
{
}

LRESULT CWriteFBXDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteFBXDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	float temp = 1.0f;
	ret = GetFloat( m_mult_wnd, &temp );
	if( ret || (temp < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	mult = temp;

/////

	char tempchar[MAX_PATH];
	ret = GetName( m_name_wnd, tempchar );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "sigのファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( name, MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( name, MAX_PATH, tempchar );
	}

	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CWriteFBXDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CWriteFBXDlg::SetWnd()
{
	m_name_wnd = GetDlgItem( IDC_FBXNAME );
	_ASSERT( m_name_wnd );

	m_mult_wnd = GetDlgItem( IDC_MULT );
	_ASSERT( m_mult_wnd );

}

int CWriteFBXDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( name );


	char tempchar[256];

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", mult );
	m_mult_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CWriteFBXDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"FBX FILE (*.fbx)\0*.fbx\0";
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
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="fbx";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( name, MAX_PATH );
	strcpy_s( name, MAX_PATH, buf );
	m_name_wnd.SetWindowText( buf );

	return 0;
}


