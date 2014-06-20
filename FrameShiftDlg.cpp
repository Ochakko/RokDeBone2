// FrameShiftDlg.cpp : CFrameShiftDlg �̃C���v�������e�[�V����
#include "stdafx.h"
#include "FrameShiftDlg.h"
#include "GetDlgParams.h"

#include "resid.h"

/////////////////////////////////////////////////////////////////////////////
// CFrameShiftDlg

CFrameShiftDlg::CFrameShiftDlg( int srcstart, int srcend )
{
	m_selstart = srcstart;
	m_selend = srcend;
	m_shift = 0;
	m_allbone = 1;
}

CFrameShiftDlg::~CFrameShiftDlg()
{
}

LRESULT CFrameShiftDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

int CFrameShiftDlg::DlgToParams()
{
	int ret;

	int tempint;
	ret = GetInt( m_edit_selstart_wnd, &tempint );
	if( ret ){
		MessageBox( "�t���[���͈͂́A�J�n�t���[���ԍ����A�s���ł��B", "���̓G���[", MB_OK );
		return 1;
	}
	m_selstart = tempint;

	ret = GetInt( m_edit_selend_wnd, &tempint );
	if( ret ){
		MessageBox( "�t���[���͈͂́A�I���t���[���ԍ����A�s���ł��B", "���̓G���[", MB_OK );
		return 1;
	}
	m_selend = tempint;

	if( m_selstart > m_selend ){
		MessageBox( "�J�n�t���[���ԍ����A�I���t���[���ԍ����A�傫���ł��B\n��蒼���Ă��������B", "���̓G���[", MB_OK );
		return 0;
	}


	ret = GetInt( m_edit_shift_wnd, &tempint );
	if( ret ){
		MessageBox( "�t���[���ړ��ʂ̒l���A�s���ł��B", "���̓G���[", MB_OK );
		return 1;
	}
	m_shift = tempint;


	UINT checkflag;
	checkflag = m_dlgwnd.IsDlgButtonChecked( IDC_SHIFT_ALL );
	if( checkflag == BST_CHECKED ){
		m_allbone = 1;
	}else{
		m_allbone = 0;
	}

	return 0;

}


LRESULT CFrameShiftDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	EndDialog(wID);
	return 0;
}

LRESULT CFrameShiftDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	EndDialog(wID);
	return 0;
}

int CFrameShiftDlg::SetWnd()
{
	m_dlgwnd = m_hWnd;

	m_edit_selstart_wnd = GetDlgItem( IDC_EDIT_SELSTART );
	m_edit_selend_wnd = GetDlgItem( IDC_EDIT_SELEND );
	m_edit_shift_wnd = GetDlgItem( IDC_EDIT_SHIFT );

	return 0;
}

int CFrameShiftDlg::ParamsToDlg()
{

	char tempchar[256];
	
	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%d", m_selstart );
	m_edit_selstart_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%d", m_selend );
	m_edit_selend_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%d", m_shift );
	m_edit_shift_wnd.SetWindowText( tempchar );
	
	if( m_allbone == 1 ){
		m_dlgwnd.CheckRadioButton( IDC_SHIFT_ALL, IDC_SHIFT_ONE, IDC_SHIFT_ALL );
	}else{
		m_dlgwnd.CheckRadioButton( IDC_SHIFT_ALL, IDC_SHIFT_ONE, IDC_SHIFT_ONE );
	}

	return 0;
}





