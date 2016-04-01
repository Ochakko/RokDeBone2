// ShadowParamDlg.cpp : CShadowParamDlg �̃C���v�������e�[�V����
#include "stdafx.h"
#include "ShadowParamDlg.h"

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CShadowParamDlg

CShadowParamDlg::CShadowParamDlg( float srcbias, float srcdarkrate )
{
	m_bias = srcbias;
	m_darkrate = srcdarkrate;
}

CShadowParamDlg::~CShadowParamDlg()
{
}

LRESULT CShadowParamDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}


int CShadowParamDlg::DlgToParams()
{
	int ret;
	float tempval;
	ret = GetFloat( m_bias_wnd, &tempval );
	if( ret ){
		MessageBox( "�o�C�A�X�̓��͒l���s���ł��i���������Ă��������j", "���̓G���[", MB_OK );
		return 1;
	}
	m_bias = tempval;


	ret = GetFloat( m_darkrate_wnd, &tempval );
	if( ret || (tempval < 0.0f) ){
		MessageBox( "�e�̔Z���̓��͒l���s���ł��i���̏��������Ă��������j", "���̓G���[", MB_OK );
		return 1;
	}
	m_darkrate = tempval;

	return 0;
}

LRESULT CShadowParamDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	EndDialog(wID);
	return 0;
}

LRESULT CShadowParamDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CShadowParamDlg::SetWnd()
{
	m_bias_wnd = GetDlgItem( IDC_BIAS );
	m_darkrate_wnd = GetDlgItem( IDC_DARKRATE );

	return 0;
}

int CShadowParamDlg::ParamsToDlg()
{
	char str1[256];
	sprintf_s( str1, 256, "%f", m_bias );
	m_bias_wnd.SetWindowText( str1 );

	sprintf_s( str1, 256, "%f", m_darkrate );
	m_darkrate_wnd.SetWindowText( str1 );
	
	return 0;
}

