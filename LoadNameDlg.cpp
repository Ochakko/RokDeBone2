// LoadNameDlg.cpp : CLoadNameDlg �̃C���v�������e�[�V����

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LoadNameDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>


/////////////////////////////////////////////////////////////////////////////
// CLoadNameDlg

CLoadNameDlg::CLoadNameDlg()
{
	ZeroMemory( m_signame, sizeof( char ) * 2048 );
	ZeroMemory( m_quaname, sizeof( char ) * 2048 );
	ZeroMemory( m_im2name, sizeof( char ) * 2048 );
	ZeroMemory( m_bonname, sizeof( char ) * 2048 );
	ZeroMemory( m_moaname, sizeof( char ) * 2048 );
	ZeroMemory( m_recentname, sizeof( char ) * 2048 );
}

CLoadNameDlg::~CLoadNameDlg()
{
}

LRESULT CLoadNameDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

LRESULT CLoadNameDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CLoadNameDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLoadNameDlg::SetWnd()
{
	m_signame_wnd = GetDlgItem( IDC_SIGNAME );
	m_quaname_wnd = GetDlgItem( IDC_QUANAME );
	m_im2name_wnd = GetDlgItem( IDC_IM2NAME );
	m_bonname_wnd = GetDlgItem( IDC_BONNAME );
	m_moaname_wnd = GetDlgItem( IDC_MOANAME );
}

int CLoadNameDlg::ParamsToDlg()
{	
	if( *m_signame ){
		m_signame_wnd.SetWindowText( m_signame );
	}else{
		m_signame_wnd.SetWindowText( "�Ȃ�" );
	}

	if( *m_quaname ){
		m_quaname_wnd.SetWindowText( m_quaname );
	}else{
		m_quaname_wnd.SetWindowText( "�Ȃ�" );
	}

	if( *m_im2name ){
		m_im2name_wnd.SetWindowText( m_im2name );
	}else{
		m_im2name_wnd.SetWindowText( "�Ȃ�" );
	}

	if( *m_bonname ){
		m_bonname_wnd.SetWindowText( m_bonname );
	}else{
		m_bonname_wnd.SetWindowText( "�Ȃ�" );
	}

	if( *m_moaname ){
		m_moaname_wnd.SetWindowText( m_moaname );
	}else{
		m_moaname_wnd.SetWindowText( "�Ȃ�" );
	}

	return 0;
}
