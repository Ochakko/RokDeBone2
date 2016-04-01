// DISPSWITCHDLGDLG.cpp : CDispSwitchDlg のインプリメンテーション
#include "stdafx.h"
#include "DispSwitchDlg.h"

#include <mothandler.h>
#include <motionctrl.h>
#include <motionpoint2.h>


/////////////////////////////////////////////////////////////////////////////
// CDispSwitchDlg

static DWORD switchid[15] =
{
	IDC_SWITCH1,
	IDC_SWITCH2,
	IDC_SWITCH3,
	IDC_SWITCH4,
	IDC_SWITCH5,

	IDC_SWITCH6,
	IDC_SWITCH7,
	IDC_SWITCH8,
	IDC_SWITCH9,
	IDC_SWITCH10,

	IDC_SWITCH11,
	IDC_SWITCH12,
	IDC_SWITCH13,
	IDC_SWITCH14,
	IDC_SWITCH15
};


CDispSwitchDlg::CDispSwitchDlg( int srcframeno, CMotionPoint2* srcmp, CMotHandler* srcmh, int srcmcookie )
{
	m_frameno = srcframeno;
	m_editmp = srcmp;
	m_editswitchno = 1;
	m_mh = srcmh;
	m_cookie = srcmcookie;
}

CDispSwitchDlg::~CDispSwitchDlg()
{
}


LRESULT CDispSwitchDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	char tempchar[256];
	int i;
	for( i = 1; i <= 15; i++ ){
		sprintf_s( tempchar, 256, "スイッチ %d", i );
		m_combo_no_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	}
	m_combo_no_wnd.SendMessage( CB_SETCURSEL, m_editswitchno - 1, 0 );
	

	ret = Params2Dlg();
	_ASSERT( !ret );

	return 1;  // システムにフォーカスを設定させます
}

int CDispSwitchDlg::Params2Dlg()
{
	char tempchar[256];
	sprintf_s( tempchar, 256, "フレーム番号 : %d", m_frameno );
	m_frameno_wnd.SetWindowText( tempchar );

	int i;
	for( i = 1; i <= 15; i++ ){
		DWORD switchstate;
		switchstate = m_editmp->dispswitch & Bitno2Mask[i];
		if( switchstate ){
			sprintf_s( tempchar, 256, "スイッチ %d :  !! オン !!", i );
		}else{
			sprintf_s( tempchar, 256, "スイッチ %d : オフ", i );
		}
		m_switch_wnd[i - 1].SetWindowText( tempchar );
	}

	return 0;
}

int CDispSwitchDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_frameno_wnd = GetDlgItem( IDC_FRAMENO );

	int i;
	for( i = 0; i < 15; i++ ){
		m_switch_wnd[i] = GetDlgItem( switchid[i] );
	}

	m_combo_no_wnd = GetDlgItem( IDC_COMBO_NO );

	m_switchon_wnd = GetDlgItem( IDC_SWITCHON );
	m_switchoff_wnd = GetDlgItem( IDC_SWITCHOFF );

	return 0;
}

LRESULT CDispSwitchDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	EndDialog(wID);

	return 0;
}



LRESULT CDispSwitchDlg::OnSwitchon(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_combo_no_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 1;
	
	m_editswitchno = combono + 1;
	if( (m_editswitchno <= 0) || (m_editswitchno > MAXDISPSWITCHNO) ){
		_ASSERT( 0 );
		return 1;
	}

	/***
	CMotionPoint2* curmp = m_editmp;
	while( curmp ){
		curmp->dispswitch |= Bitno2Mask[ m_editswitchno ];
		
		curmp = curmp->next;
	}
	***/

	CMotionCtrl* mcptr;
	CMotionPoint2* mpptr;
	CMotionPoint2* curmp;
	int i;
	for( i = 0; i < m_mh->s2mot_leng; i++ ){
		mcptr = (*m_mh)( i );
		if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
			mpptr = mcptr->IsExistMotionPoint( m_cookie, m_frameno );

			curmp = mpptr;
			while( curmp ){
				curmp->dispswitch |= Bitno2Mask[ m_editswitchno ];

				curmp = curmp->next;
			}


		}

	}


	Params2Dlg();

	return 0;
}
LRESULT CDispSwitchDlg::OnSwitchoff(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_combo_no_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 1;
	
	m_editswitchno = combono + 1;
	if( (m_editswitchno <= 0) || (m_editswitchno > MAXDISPSWITCHNO) ){
		_ASSERT( 0 );
		return 1;
	}

	/***
	CMotionPoint2* curmp = m_editmp;
	while( curmp ){
		curmp->dispswitch &= ~Bitno2Mask[ m_editswitchno ];
		
		curmp = curmp->next;
	}
	***/
	CMotionCtrl* mcptr;
	CMotionPoint2* mpptr;
	CMotionPoint2* curmp;
	int i;
	for( i = 0; i < m_mh->s2mot_leng; i++ ){
		mcptr = (*m_mh)( i );
		if( mcptr->IsJoint() && (mcptr->type != SHDMORPH) ){
			mpptr = mcptr->IsExistMotionPoint( m_cookie, m_frameno );

			curmp = mpptr;
			while( curmp ){
				curmp->dispswitch &= ~(Bitno2Mask[ m_editswitchno ]);

				curmp = curmp->next;
			}
		}

	}

	Params2Dlg();

	return 0;
}
