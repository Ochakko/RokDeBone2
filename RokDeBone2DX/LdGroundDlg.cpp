// LdGroundDlg.cpp : CLdGroundDlg �̃C���v�������e�[�V����

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LdGroundDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLdGroundDlg

CLdGroundDlg::CLdGroundDlg()
{
	ZeroMemory( name, _MAX_PATH );
	mult = 1.0f;
}

CLdGroundDlg::~CLdGroundDlg()
{
}

LRESULT CLdGroundDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

LRESULT CLdGroundDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	float tempfloat = 0.0f;
	ret = GetFloat( m_mult_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "�{���̒l���s���ł��B( float 0.0f�ȏ� )", "���̓G���[", MB_OK );
		return 0;
	}
	mult = tempfloat;
/////

	char tempchar[_MAX_PATH];
	ret = GetName( m_name_wnd, tempchar );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "�n�ʂ̃t�@�C�������s���ł��B", "���̓G���[", MB_OK );
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

LRESULT CLdGroundDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLdGroundDlg::SetWnd()
{
	HWND temphwnd;

	temphwnd = GetDlgItem( IDC_SIGNAME );
	m_name_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_MULT );
	m_mult_wnd = temphwnd;

}

int CLdGroundDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( name );


	char tempchar[256];

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", mult );
	m_mult_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CLdGroundDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"�n��sig,mqo(*.sig,*.mqo)\0*.sig;*.mqo\0";
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
	ofn.lpstrDefExt ="sig";
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

