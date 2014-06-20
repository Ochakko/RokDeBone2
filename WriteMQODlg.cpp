// WriteMQODlg.cpp : CWriteMQODlg のインプリメンテーション
#include "stdafx.h"
#include "WriteMQODlg.h"

#define DBGH
#include <dbg.h>
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CWriteMQODlg

enum {
	OUTMODE_ALL,
	OUTMODE_KEY,
	OUTMODE_MAX
};


CWriteMQODlg::CWriteMQODlg( char* srcfilename )
{
	m_outmode = OUTMODE_ALL;
	if( srcfilename && *srcfilename ){
		strcpy_s( m_head, 2048, srcfilename );
		int ch = '.';
		char* extptr;
		extptr = strchr( m_head, ch );
		if( extptr ){
			*extptr = 0;
		}
	}else{
		strcpy_s( m_head, 2048, "output" );
	}
	m_mult = 1.0f;
	m_outbone = 0;

	//strcpy( m_bonefilename, "output.bon" );
	m_outbonefile = 0;

	m_chkmorph = 0;
}

CWriteMQODlg::~CWriteMQODlg()
{
}


LRESULT CWriteMQODlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	m_dlg_wnd = m_hWnd;
	m_edit_head_wnd = GetDlgItem( IDC_EDIT_HEAD );
	m_edit_mult_wnd = GetDlgItem( IDC_EDIT_MULT );
	m_outbone_wnd = GetDlgItem( IDC_CHECK_OUTBONE );
	//m_edit_bonefile_wnd = GetDlgItem( IDC_EDIT_BONEFILE );

	if( m_outmode == OUTMODE_ALL ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ALL, IDC_RADIO_KEY, IDC_RADIO_ALL );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ALL, IDC_RADIO_KEY, IDC_RADIO_KEY );
	}

	m_edit_head_wnd.SetWindowText( m_head );

	char strmult[256];
	sprintf_s( strmult, 256, "%f", m_mult );
	m_edit_mult_wnd.SetWindowText( strmult );

	//m_edit_bonefile_wnd.SetWindowText( m_bonefilename );


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteMQODlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_ALL );
	if( ischecked == BST_CHECKED ){
		m_outmode = OUTMODE_ALL;
	}else{
		m_outmode = OUTMODE_KEY;
	}

	m_edit_head_wnd.GetWindowText( m_head, 2048 );
	int ch;
	ch = '.';
	char* extptr;
	extptr = strrchr( m_head, ch );
	if( extptr ){
		*extptr = 0;
	}

	int ret = 0;
	float tempfloat = 0.0f;
	ret = GetFloat( m_edit_mult_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	m_mult = tempfloat;
	

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_OUTBONE );
	if( checkflag == BST_CHECKED ){
		m_outbone = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_outbone = 0;
	}

	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_OUTBF );
	if( checkflag == BST_CHECKED ){
		m_outbonefile = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_outbonefile = 0;
	}


	//m_edit_bonefile_wnd.GetWindowText( m_bonefilename, 2048 );
	//if( m_outbonefile && (m_bonefilename[0] == 0) ){
	//	MessageBox( "bonファイル名を指定してください。", "入力エラー", MB_OK );
	//	return 0;
	//}


	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKMORPH );
	if( checkflag == BST_CHECKED ){
		m_chkmorph = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_chkmorph = 0;
	}



	EndDialog(wID);
	return 0;
}

LRESULT CWriteMQODlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}


LRESULT CWriteMQODlg::OnRefHead(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.mqo" );
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

	strcpy_s( m_head, 2048, buf );
	m_edit_head_wnd.SetWindowText( m_head );

	return 0;
}

