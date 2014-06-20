// FramePasteDlg.cpp : CFramePasteDlg �̃C���v�������e�[�V����
#include "stdafx.h"
#include "FramePasteDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CFramePasteDlg

CFramePasteDlg::CFramePasteDlg( int hascancel, int curframe )
{
	m_hascancel = hascancel;
	m_select = 0;

	m_curframe = curframe;
	m_start = curframe;
	m_end = curframe;
}

CFramePasteDlg::~CFramePasteDlg()
{
}

LRESULT CFramePasteDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	_ASSERT( !ret );

	if( m_hascancel == 0 ){
		m_cancel_wnd.EnableWindow( FALSE );
	}

	char tempchar[256];
	sprintf_s( tempchar, 256, "%d", m_start );
	m_edit_start_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%d", m_end );
	m_edit_end_wnd.SetWindowText( tempchar );


	if( m_select == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_CURFRAME, IDC_RADIO_SELECT, IDC_RADIO_CURFRAME );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_CURFRAME, IDC_RADIO_SELECT, IDC_RADIO_SELECT );
	}


	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

int CFramePasteDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_edit_start_wnd = GetDlgItem( IDC_EDIT_START );
	m_edit_end_wnd = GetDlgItem( IDC_EDIT_END );

	m_cancel_wnd = GetDlgItem( 2 );

	return 0;
}

LRESULT CFramePasteDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int tempint;


	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_CURFRAME );
	if( ischecked == BST_CHECKED ){
		m_select = 0;
	}else{
		m_select = 1;
	}


	if( m_select == 1 ){
		ret = GetInt( m_edit_start_wnd, &tempint );
		if( ret ){
			MessageBox( "�J�n�t���[���ԍ����s���ł��B", "���̓G���[", MB_OK );
			return 0;
		}
		m_start = tempint;

		ret = GetInt( m_edit_end_wnd, &tempint );
		if( ret ){
			MessageBox( "�I���t���[���ԍ����s���ł��B", "���̓G���[", MB_OK );
			return 0;
		}
		m_end = tempint;
	
		if( m_start > m_end ){
			MessageBox( "�J�n�t���[���ԍ����A�I���t���[���ԍ����A�傫���ł��B", "���̓G���[", MB_OK );
			return 0;
		}
	}else{
		m_start = m_curframe;
		m_end = m_curframe;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CFramePasteDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

