// MouseOptDlg.cpp : CMouseOptDlg のインプリメンテーション
#include "stdafx.h"
#include "MouseOptDlg.h"

#include "resid.h"

#define DBGH
#include <dbg.h>


/////////////////////////////////////////////////////////////////////////////
// CMouseOptDlg
CMouseOptDlg::CMouseOptDlg( int srcml, int srcmr, int srcmw )
{
	m_MouseL = srcml;
	m_MouseR = srcmr;
	m_MouseW = srcmw;
}

CMouseOptDlg::~CMouseOptDlg()
{
}


LRESULT CMouseOptDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{


	m_dlg_wnd = m_hWnd;

/***
	switch( m_MouseL ){
	case MOUSEOPE_NONE:
		m_dlg_wnd.CheckRadioButton( IDC_ML_ROT, IDC_ML_NONE, IDC_ML_NONE );
		break;
	case MOUSEOPE_CAMROT:
		m_dlg_wnd.CheckRadioButton( IDC_ML_ROT, IDC_ML_NONE, IDC_ML_ROT );
		break;
	case MOUSEOPE_CAMMOVE:
		m_dlg_wnd.CheckRadioButton( IDC_ML_ROT, IDC_ML_NONE, IDC_ML_MOVE );
		break;
	default:
		_ASSERT( 0 );
		m_MouseL = MOUSEOPE_CAMROT;
		m_dlg_wnd.CheckRadioButton( IDC_ML_ROT, IDC_ML_NONE, IDC_ML_ROT );
		break;
	}

	switch( m_MouseR ){
	case MOUSEOPE_NONE:
		m_dlg_wnd.CheckRadioButton( IDC_MR_ROT, IDC_MR_NONE, IDC_MR_NONE );
		break;
	case MOUSEOPE_CAMROT:
		m_dlg_wnd.CheckRadioButton( IDC_MR_ROT, IDC_MR_NONE, IDC_MR_ROT );
		break;
	case MOUSEOPE_CAMMOVE:
		m_dlg_wnd.CheckRadioButton( IDC_MR_ROT, IDC_MR_NONE, IDC_MR_MOVE );
		break;
	default:
		_ASSERT( 0 );
		m_MouseR = MOUSEOPE_CAMMOVE;
		m_dlg_wnd.CheckRadioButton( IDC_MR_ROT, IDC_MR_NONE, IDC_MR_MOVE );
		break;
	}
***/
	switch( m_MouseW ){
	case MOUSEOPE_BONETWIST:
		m_dlg_wnd.CheckRadioButton( IDC_MW_TWIST, IDC_MW_DIST, IDC_MW_TWIST );
		break;
	case MOUSEOPE_CAMDIST:
		m_dlg_wnd.CheckRadioButton( IDC_MW_TWIST, IDC_MW_DIST, IDC_MW_DIST );
		break;
	default:
		m_MouseW = MOUSEOPE_BONETWIST;
		m_dlg_wnd.CheckRadioButton( IDC_MW_TWIST, IDC_MW_DIST, IDC_MW_TWIST );
		break;
	}


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CMouseOptDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
//enum {
//	MOUSEOPE_NONE,
//	MOUSEOPE_CAMROT,
//	MOUSEOPE_CAMMOVE,
//	MOUSEOPE_CAMDIST,
//	MOUSEOPE_BONETWIST,
//	MOUSEOPE_MAX
//};

	UINT ischecked;
	/***
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_ML_ROT );
	if( ischecked == BST_CHECKED ){
		m_MouseL = MOUSEOPE_CAMROT;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_ML_MOVE );
	if( ischecked == BST_CHECKED ){
		m_MouseL = MOUSEOPE_CAMMOVE;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_ML_NONE );
	if( ischecked == BST_CHECKED ){
		m_MouseL = MOUSEOPE_NONE;
	}


	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_MR_ROT );
	if( ischecked == BST_CHECKED ){
		m_MouseR = MOUSEOPE_CAMROT;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_MR_MOVE );
	if( ischecked == BST_CHECKED ){
		m_MouseR = MOUSEOPE_CAMMOVE;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_MR_NONE );
	if( ischecked == BST_CHECKED ){
		m_MouseR = MOUSEOPE_NONE;
	}
	***/

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_MW_TWIST );
	if( ischecked == BST_CHECKED ){
		m_MouseW = MOUSEOPE_BONETWIST;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_MW_DIST );
	if( ischecked == BST_CHECKED ){
		m_MouseW = MOUSEOPE_CAMDIST;
	}


	EndDialog(wID);
	return 0;
}

LRESULT CMouseOptDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}
