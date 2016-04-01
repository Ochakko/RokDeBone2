// WriteBmpDlg.cpp : CWriteBmpDlg �̃C���v�������e�[�V����
#include "stdafx.h"
#include "WriteBmpDlg.h"
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CWriteBmpDlg

CWriteBmpDlg::CWriteBmpDlg( int framemax )
{
	m_framemax = framemax;
	m_startframe = 0;
	m_endframe = m_framemax;
	ZeroMemory( m_prefix, 1024 );
	strcpy_s( m_prefix, 1024, "output" );
}

CWriteBmpDlg::~CWriteBmpDlg()
{
}

LRESULT CWriteBmpDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND temphwnd;
	temphwnd = GetDlgItem( IDC_PREFIX );
	m_prefix_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_STARTFRAME );
	m_startframe_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_ENDFRAME );
	m_endframe_wnd = temphwnd;

	m_prefix_wnd.SetWindowText( m_prefix );

	char tempchar[256];
	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%d", m_startframe );
	m_startframe_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%d", m_endframe );
	m_endframe_wnd.SetWindowText( tempchar );

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

LRESULT CWriteBmpDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = GetText( m_prefix_wnd, m_prefix, 1024 );
	if( ret ){
		MessageBox( "�v���t�B�b�N�X�����s���ł��B", "���̓G���[", MB_OK );
		return 0;
	}

	int tempint;
	ret = GetInt( m_startframe_wnd, &tempint );
	if( ret || (tempint < 0) || (tempint > m_framemax) ){
		MessageBox( "�X�^�[�g�t���[���ԍ����s���ł��B", "���̓G���[", MB_OK );
		return 0;
	}
	m_startframe = tempint;

	ret = GetInt( m_endframe_wnd, &tempint );
	if( ret || (tempint < m_startframe) || (tempint > m_framemax) ){
		MessageBox( "�I���t���[���ԍ����s���ł��B", "���̓G���[", MB_OK );
		return 0;
	}
	m_endframe = tempint;

	EndDialog(wID);
	return 0;
}

LRESULT CWriteBmpDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}


