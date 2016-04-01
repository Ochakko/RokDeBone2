// OpTargetDlg.cpp : COpTargetDlg のインプリメンテーション
#include "stdafx.h"
#include "OpTargetDlg.h"

#include <coef.h>
#include "resid.h"

#define DBGH
#include <dbg.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// COpTargetDlg
COpTargetDlg::COpTargetDlg()
{
	m_ope = OPTAR_CUR;
	m_curframe = 0;
	m_selstart = 0;
	m_selend = 0;

	m_startframe = 0;
	m_endframe = 0;
}

COpTargetDlg::~COpTargetDlg()
{
}

LRESULT COpTargetDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	m_dlg_wnd = m_hWnd;
	m_curframe_wnd = GetDlgItem( IDC_CURFRAME );
	m_selstart_wnd = GetDlgItem( IDC_SELSTART );
	m_selend_wnd = GetDlgItem( IDC_SELEND );

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

int COpTargetDlg::ParamsToDlg()
{
	switch( m_ope ){
	case OPTAR_CUR:
		m_dlg_wnd.CheckRadioButton( IDC_OPT_CUR, IDC_OPT_SEL, IDC_OPT_CUR );
		break;
	case OPTAR_ALL:
		m_dlg_wnd.CheckRadioButton( IDC_OPT_CUR, IDC_OPT_SEL, IDC_OPT_ALL );
		break;
	case OPTAR_FWD:
		m_dlg_wnd.CheckRadioButton( IDC_OPT_CUR, IDC_OPT_SEL, IDC_OPT_FWD );
		break;
	case OPTAR_SEL:
		m_dlg_wnd.CheckRadioButton( IDC_OPT_CUR, IDC_OPT_SEL, IDC_OPT_SEL );
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	char strframe[256];
	sprintf_s( strframe, 256, "%d", m_selstart );
	m_selstart_wnd.SetWindowText( strframe );

	sprintf_s( strframe, 256, "%d", m_selend );
	m_selend_wnd.SetWindowText( strframe );

	sprintf_s( strframe, 256, "現在のフレーム番号:%d", m_curframe );
	m_curframe_wnd.SetWindowText( strframe );

	return 0;
}

LRESULT COpTargetDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT chkcur, chkall, chkfwd, chksel;
	chkcur = m_dlg_wnd.IsDlgButtonChecked( IDC_OPT_CUR );
	chkall = m_dlg_wnd.IsDlgButtonChecked( IDC_OPT_ALL );
	chkfwd = m_dlg_wnd.IsDlgButtonChecked( IDC_OPT_FWD );
	chksel = m_dlg_wnd.IsDlgButtonChecked( IDC_OPT_SEL );
	if( chkcur == BST_CHECKED ){
		m_ope = OPTAR_CUR;
	}else{
		if( chkall == BST_CHECKED ){
			m_ope = OPTAR_ALL;
		}else{
			if( chkfwd == BST_CHECKED ){
				m_ope = OPTAR_FWD;
			}else{
				if( chksel == BST_CHECKED ){
					m_ope = OPTAR_SEL;

					int tmpstart, tmpend;
					int retstart, retend;
					retstart = GetInt( m_selstart_wnd, &tmpstart );
					if( retstart ){
						::MessageBoxA( m_hWnd, "開始フレーム番号が不正です。", "入力エラー", MB_OK );
						return 0;
					}
					retend = GetInt( m_selend_wnd, &tmpend );
					if( retend ){
						::MessageBoxA( m_hWnd, "終了フレーム番号が不正です。", "入力エラー", MB_OK );
						return 0;
					}
					
					if( tmpstart > tmpend ){
						::MessageBoxA( m_hWnd, "終了フレームには開始フレーム以上の番号を指定してください。", "入力エラー", MB_OK );
						return 0;
					}
					m_selstart = tmpstart;
					m_selend = tmpend;
				}
			}
		}
	}

	ShowDlg( SW_HIDE, 0 );


//	EndDialog(wID);
	return 0;
}

LRESULT COpTargetDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE, 0 );

//	EndDialog(wID);
	return 0;
}

int COpTargetDlg::ShowDlg( int cmdshow, int srcframe )
{

	m_cmdshow = cmdshow;
	ShowWindow( m_cmdshow );

	if( cmdshow != SW_HIDE ){
		m_curframe = srcframe;
	}

	ParamsToDlg();

	return 0;
}

int COpTargetDlg::CalcStartEnd( int srccur, int srcmax )
{
	m_curframe = srccur;

	switch( m_ope ){
	case OPTAR_CUR:
		m_startframe = srccur;
		m_endframe = srccur;
		break;
	case OPTAR_ALL:
		m_startframe = 0;
		m_endframe = srcmax;
		break;
	case OPTAR_FWD:
		m_startframe = srccur;
		m_endframe = srcmax;
		break;
	case OPTAR_SEL:
		m_startframe = min( m_selstart, srcmax );
		m_endframe = min( m_selend, srcmax );
		m_endframe = max( m_endframe, m_startframe );
		break;
	default:
		_ASSERT( 0 );
		m_startframe = srccur;
		m_endframe = srccur;
		break;
	}

	return 0;
}
