// LDSigDlg.cpp : CLDSigDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LDSigDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLDSigDlg

CLDSigDlg::CLDSigDlg( char* srcfilename )
{
	ZeroMemory( name, MAX_PATH );
	if( srcfilename && *srcfilename ){
		strcpy_s( name, MAX_PATH, srcfilename );
	}
	multvec = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
}

CLDSigDlg::~CLDSigDlg()
{
}

LRESULT CLDSigDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CLDSigDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	float tempx = 1.0f;
	float tempy = 1.0f;
	float tempz = 1.0f;
	ret = GetFloat( m_multx_wnd, &tempx );
	if( ret || (tempx < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	multvec.x = tempx;

	ret = GetFloat( m_multy_wnd, &tempy );
	if( ret || (tempy < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	multvec.y = tempy;

	ret = GetFloat( m_multz_wnd, &tempz );
	if( ret || (tempz < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	multvec.z = tempz;

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

LRESULT CLDSigDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLDSigDlg::SetWnd()
{
	m_name_wnd = GetDlgItem( IDC_SIGNAME );

	m_multx_wnd = GetDlgItem( IDC_MULTX );
	m_multy_wnd = GetDlgItem( IDC_MULTY );
	m_multz_wnd = GetDlgItem( IDC_MULTZ );


}

int CLDSigDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( name );


	char tempchar[256];

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", multvec.x );
	m_multx_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%f", multvec.y );
	m_multy_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%f", multvec.z );
	m_multz_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CLDSigDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[MAX_PATH];
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

	ZeroMemory( name, MAX_PATH );
	strcpy_s( name, MAX_PATH, buf );
	m_name_wnd.SetWindowText( buf );

	return 0;
}


