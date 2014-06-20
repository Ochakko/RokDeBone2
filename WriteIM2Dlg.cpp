// WriteIM2Dlg.cpp : CWriteIM2Dlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "WriteIM2Dlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>


/////////////////////////////////////////////////////////////////////////////
// CWriteIM2Dlg

CWriteIM2Dlg::CWriteIM2Dlg( char* srcfilename )
{
	InitParams();
	if( srcfilename && *srcfilename ){
		strcpy_s( m_im2name, _MAX_PATH, srcfilename );
	}
}

CWriteIM2Dlg::~CWriteIM2Dlg()
{
}

int CWriteIM2Dlg::InitParams()
{
	m_formqo = 0;
	strcpy_s( m_im2name, _MAX_PATH, "output.im2" );

	return 0;
}

LRESULT CWriteIM2Dlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteIM2Dlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_FORSIG );
	if( checkflag == BST_CHECKED ){
		m_formqo = 0;
	}else if( checkflag == BST_UNCHECKED ){
		m_formqo = 1;
	}

////////
	m_im2name[0] = 0;
	m_edit_im2name_wnd.GetWindowText( m_im2name, _MAX_PATH );
	if( m_im2name[0] == 0 ){
		MessageBox( "出力ファイル名を、設定してください。", "入力エラー", MB_OK );
		return 0;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CWriteIM2Dlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CWriteIM2Dlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	m_edit_im2name_wnd = GetDlgItem( IDC_EDIT_IM2NAME );
}

int CWriteIM2Dlg::ParamsToDlg()
{	

//radio	
	if( m_formqo == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_FORSIG, IDC_RADIO_FORMQO, IDC_RADIO_FORSIG );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_FORSIG, IDC_RADIO_FORMQO, IDC_RADIO_FORMQO );
	}
////
	m_edit_im2name_wnd.SetWindowText( m_im2name );

	return 0;
}

LRESULT CWriteIM2Dlg::OnRefIM2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.im2" );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"IM2 FILE (*.im2)\0*.im2\0";
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
	ofn.lpstrDefExt = "im2";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	strcpy_s( m_im2name, _MAX_PATH, buf );
	m_edit_im2name_wnd.SetWindowText( m_im2name );

	return 0;
}

