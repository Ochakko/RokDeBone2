// LoadBVHDlg.cpp : CLoadBVHDlg のインプリメンテーション
#include "stdafx.h"
#include "LoadBVHDlg.h"
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadBVHDlg

CLoadBVHDlg::CLoadBVHDlg( char* srcfilename )
{
	ZeroMemory( m_name, sizeof( char ) * _MAX_PATH );
	if( srcfilename && *srcfilename ){
		strcpy_s( m_name, _MAX_PATH, srcfilename );
	}
	ZeroMemory( m_mqoname, sizeof( char ) * _MAX_PATH );
	ZeroMemory( m_bonname, sizeof( char ) * _MAX_PATH );


	int ch = '\\';
	char* lasten;

	GetModuleFileName( NULL, m_mqoname, _MAX_PATH );
	lasten = strrchr( m_mqoname, ch );
	if( lasten ){
	*( lasten + 1 ) = 0;
		strcat_s( m_mqoname, _MAX_PATH, "tempbvh.mqo" );
	}else{
		strcpy_s( m_mqoname, _MAX_PATH, "tempbvh.mqo" );
	}
	
	GetModuleFileName( NULL, m_bonname, _MAX_PATH );
	lasten = strrchr( m_bonname, ch );
	if( lasten ){
	*( lasten + 1 ) = 0;
		strcat_s( m_bonname, _MAX_PATH, "tempbvh.bon" );
	}else{
		strcpy_s( m_bonname, _MAX_PATH, "tempbvh.bon" );
	}

	m_mult = 1.0f;

	m_motonly = 0;
}

CLoadBVHDlg::~CLoadBVHDlg()
{
}


LRESULT CLoadBVHDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	m_name_wnd.SetWindowText( m_name );
	m_mqo_wnd.SetWindowText( m_mqoname );
	m_bon_wnd.SetWindowText( m_bonname );

	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", m_mult );
	m_mult_wnd.SetWindowText( tempchar );


	return 1;  // システムにフォーカスを設定させます
}

int CLoadBVHDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_name_wnd = GetDlgItem( IDC_EDIT_BVH );
	m_mqo_wnd = GetDlgItem( IDC_EDIT_MQONAME );
	m_bon_wnd = GetDlgItem( IDC_EDIT_BONNAME );
	m_mult_wnd = GetDlgItem( IDC_EDIT_MULT );
	m_motonly_wnd = GetDlgItem( IDC_CHECK_MOTONLY );

	return 0;
}

LRESULT CLoadBVHDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	char errmsg[1024];
	ZeroMemory( errmsg, 1024 );

	//m_motonly
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_MOTONLY );
	if( checkflag == BST_CHECKED ){
		m_motonly = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_motonly = 0;
	}

	//m_name
	m_name_wnd.GetWindowText( m_name, _MAX_PATH );
	if( m_name[0] == 0 ){
		strcpy_s( errmsg, 1024, "ＢＶＨファイル名を指定してください。" );
		ret = 1;
		goto endofonok;
	}

	m_mqo_wnd.GetWindowText( m_mqoname, _MAX_PATH );
	if( m_mqoname[0] == 0 ){
		strcpy_s( errmsg, 1024, "作業用MQOファイル名を指定してください。" );
		ret = 1;
		goto endofonok;
	}

	m_bon_wnd.GetWindowText( m_bonname, _MAX_PATH );
	if( m_bonname[0] == 0 ){
		strcpy_s( errmsg, 1024, "作業用BONファイル名を指定してください。" );
		ret = 1;
		goto endofonok;
	}

	float tempfloat = 0.0f;
	ret = GetFloat( m_mult_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		strcpy_s( errmsg, 1024, "倍率の値が不正です。( float 0.0f以上 )" );
		goto endofonok;
	}
	m_mult = tempfloat;
	

	goto endofonok;
endofonok:
	if( !ret ){
		EndDialog(wID);
		//DestroyWindow();
	}else{
		::MessageBox( m_hWnd, errmsg, "ダイアログのパラメータ不正", MB_OK );
	}
	return 0;
}

LRESULT CLoadBVHDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CLoadBVHDlg::OnRefBVH(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"BVH FILE (*.bvh)\0*.bvh\0";
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

	ZeroMemory( m_name, _MAX_PATH );
	strcpy_s( m_name, _MAX_PATH, buf );
	m_name_wnd.SetWindowText( buf );

	return 0;
}

LRESULT CLoadBVHDlg::OnRefMQO(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"MQO FILE (*.mqo)\0*.mqo\0";
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
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( m_mqoname, _MAX_PATH );
	strcpy_s( m_mqoname, _MAX_PATH, buf );
	m_mqo_wnd.SetWindowText( buf );

	return 0;
}

LRESULT CLoadBVHDlg::OnRefBON(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"BON FILE (*.bon)\0*.bon\0";
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
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( m_bonname, _MAX_PATH );
	strcpy_s( m_bonname, _MAX_PATH, buf );
	m_bon_wnd.SetWindowText( buf );

	return 0;
}

