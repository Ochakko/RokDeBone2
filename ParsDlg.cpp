// ParsDlg.cpp : CParsDlg �̃C���v�������e�[�V����

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "ParsDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#define DBGH
#include <dbg.h>

#include "GetDlgParams.h"

#define SLMIN	30
#define SLMAX	80

/////////////////////////////////////////////////////////////////////////////
// CParsDlg

CParsDlg::CParsDlg( int srcmode, int srcfov, int srcorthosize )
{
	parsmode = srcmode;
	parsfov = srcfov;
	orthosize = srcorthosize;
}

CParsDlg::~CParsDlg()
{

}

LRESULT CParsDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

//DbgOut( "parsdlg : OnInitDialog\r\n" );

	SetWnd();
	ParamsToDlg();

	if( parsmode == PARSMODE_PARS ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_PARS1, IDC_RADIO_ORTHO1, IDC_RADIO_PARS1 );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_PARS1, IDC_RADIO_ORTHO1, IDC_RADIO_ORTHO1 );
	}


	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

LRESULT CParsDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_PARS1 );
	if( checkflag == BST_CHECKED ){
		parsmode = PARSMODE_PARS;
	}else{
		parsmode = PARSMODE_ORTHO;
	}

	int tempint;
	ret = GetInt( m_orthosize_wnd, &tempint );
	if( ret || (tempint < 500) ){
		MessageBox( "���ˉe�̃r���[�̑傫�����s���ł��B", "���̓G���[", MB_OK );
		return 0;
	}
	orthosize = tempint;


	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CParsDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CParsDlg::SetWnd()
{
	HWND temphwnd;

	temphwnd = GetDlgItem( IDC_FOVTEXT );
	m_fovtext_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SLIDER1 );
	m_sl_wnd = temphwnd;

	//slider�̏����ݒ�
	m_sl_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)SLMIN );
	m_sl_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)SLMAX );
	SetSliderTic( m_sl_wnd, 5, SLMIN, SLMAX );

	m_orthosize_wnd = GetDlgItem( IDC_EDIT_ORTHOSIZE );


	m_dlg_wnd = m_hWnd;

}

int CParsDlg::ParamsToDlg()
{	

	char tempchar[256];
	sprintf_s( tempchar, 256, "���݂̎���p�F�@%d�x", parsfov );
	m_fovtext_wnd.SetWindowText( tempchar );
	
	m_sl_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)parsfov );

	sprintf_s( tempchar, 256, "%d", orthosize );
	m_orthosize_wnd.SetWindowText( tempchar );

	return 0;
}


LRESULT CParsDlg::OnReleasedcaptureSl1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	
	// TODO : ���۰ق̒ʒm����חp�̺��ނ�ǉ����Ă��������B
	int curpos;
	//int ret;
	curpos = (int)m_sl_wnd.SendMessage( TBM_GETPOS, 0, 0 );

	parsfov = curpos;

	int ret;
	ret = ParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	//Redraw();
	return 0;
}

void CParsDlg::SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic )
{
	// !!! mintic��maxtic�ɂ́A�����I��tic������̂Őݒ�͂��Ȃ��B(���Ă͂����Ȃ�)
	int curtic = mintic + ticfreq;

	while( curtic < maxtic ){
		dstwnd.SendMessage( TBM_SETTIC, 0, (LPARAM)(LONG)curtic );
		curtic += ticfreq;
	}
}
