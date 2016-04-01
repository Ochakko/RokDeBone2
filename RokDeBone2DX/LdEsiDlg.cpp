// LdEsiDlg.cpp : CLdEsiDlg �̃C���v�������e�[�V����

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LdEsiDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLdEsiDlg

CLdEsiDlg::CLdEsiDlg()
{
	mult = 1.0f;
}

CLdEsiDlg::~CLdEsiDlg()
{
}

LRESULT CLdEsiDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

LRESULT CLdEsiDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CLdEsiDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLdEsiDlg::SetWnd()
{
	m_mult_wnd = GetDlgItem( IDC_MULT );

}

int CLdEsiDlg::ParamsToDlg()
{	
	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", mult );
	m_mult_wnd.SetWindowText( tempchar );

	return 0;
}

