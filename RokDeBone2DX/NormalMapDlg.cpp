// NormalMapDlg.cpp : CNormalMapDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "NormalMapDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"


/////////////////////////////////////////////////////////////////////////////
// CNormalMapDlg

CNormalMapDlg::CNormalMapDlg()
{
	ZeroMemory( hmapname, _MAX_PATH );
	ZeroMemory( nmapname, _MAX_PATH );
	height = 1.0f;
}

CNormalMapDlg::~CNormalMapDlg()
{
}

LRESULT CNormalMapDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CNormalMapDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	float tempfloat = 1.0f;
	ret = GetFloat( m_hparam_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "高さパラメータが不正です。正の小数", "入力エラー", MB_OK );
		return 0;
	}
	height = tempfloat;

	char tempchar[_MAX_PATH];
	
	ret = GetName( m_hmap_wnd, tempchar );
	if( ret ){
		MessageBox( "白黒マップファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( hmapname, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( hmapname, _MAX_PATH, tempchar );
	}


	ret = GetName( m_nmap_wnd, tempchar );
	if( ret ){
		MessageBox( "出力ファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( nmapname, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( nmapname, _MAX_PATH, tempchar );
	}

	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CNormalMapDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CNormalMapDlg::SetWnd()
{
	m_hmap_wnd = GetDlgItem( IDC_HMAP );
	m_nmap_wnd = GetDlgItem( IDC_NMAP );
	m_hparam_wnd = GetDlgItem( IDC_HPARAM );
}

int CNormalMapDlg::ParamsToDlg()
{	
	m_hmap_wnd.SetWindowText( hmapname );
	m_nmap_wnd.SetWindowText( nmapname );

	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", height );
	m_hparam_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CNormalMapDlg::OnRefHMap(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"BMP FILE (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
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

	ZeroMemory( hmapname, _MAX_PATH );
	strcpy_s( hmapname, _MAX_PATH, buf );
	m_hmap_wnd.SetWindowText( buf );

	return 0;
}
LRESULT CNormalMapDlg::OnRefNMap(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.bmp" );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"BMP FILE (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = "bmp";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( nmapname, _MAX_PATH );
	strcpy_s( nmapname, _MAX_PATH, buf );
	m_nmap_wnd.SetWindowText( buf );

	return 0;
}

