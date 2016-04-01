// SndSettingDlg.cpp : CSndSettingDlg のインプリメンテーション
#include "stdafx.h"
#include "SndSettingDlg.h"

#include <stdlib.h>
#include <coef.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CSndSettingDlg
CSndSettingDlg::CSndSettingDlg( char* nameptr, int totalframe, int mottype,  int srcstartframe, int srcendframe, int fixleng )
{
	_ASSERT( nameptr );
	strcpy_s( m_name, 256, nameptr );
	m_totalframe = totalframe;
	m_mottype = mottype;

	m_startframe = srcstartframe;
	m_endframe = srcendframe;

	m_fixlengflag = fixleng;
}

CSndSettingDlg::~CSndSettingDlg()
{
}

LRESULT CSndSettingDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	_ASSERT( !ret );
	ret = Params2Dlg();
	_ASSERT( !ret );

	if( m_fixlengflag != 0 ){
		m_totalframe_wnd.EnableWindow( FALSE );
	}

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSndSettingDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	//ret = GetNameAlNum( m_name_wnd, m_name );
	ret = GetName( m_name_wnd, m_name );
	if( ret ){
		return 0;//!!!!!!!!!! warningは、GetNameAlNumで出す。
	}


	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_R_MOTSTOP );
	if( ischecked == BST_CHECKED ){
		m_mottype = MOTION_CLAMP;
	}else{
		m_mottype = MOTION_ROUND;
	}

	ret = GetInt( m_totalframe_wnd, &m_totalframe );
	//if( ret || (m_totalframe < 5) || (m_totalframe > 3000) ){
	if( ret || (m_totalframe < 1) || (m_totalframe > 36000) ){
		::MessageBox( m_hWnd, "総フレーム数の値が不正です。\n(int の 1 ～ 36000)",
			"入力不正", MB_OK );
		return 0;
	}

	ret = GetInt( m_start_wnd, &m_startframe );
	if( ret ){
		::MessageBox( m_hWnd, "表示開始フレームが不正です。int", "入力エラー", MB_OK );
		return 0;
	}
	ret = GetInt( m_end_wnd, &m_endframe );
	if( ret ){
		::MessageBox( m_hWnd, "表示終了フレームが不正です。int", "入力エラー", MB_OK );
		return 0;
	}

	if( (m_endframe <= 0) || (m_endframe > (m_totalframe - 1)) ){
		m_endframe = m_totalframe - 1;//!!!!!!!!!!!
	}
	if( (m_startframe < 0) || (m_startframe >= m_endframe) ){
		m_startframe = 0;//!!!!!!!!!!!!!!!!!!!!
	}


	EndDialog(wID);
	return 0;
}

LRESULT CSndSettingDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CSndSettingDlg::SetWnd()
{
	HWND temphwnd;

	m_dlg_wnd = m_hWnd;

	temphwnd = GetDlgItem( IDC_NAME );
	m_name_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TOTALFRAME );
	m_totalframe_wnd = temphwnd;

	m_start_wnd = GetDlgItem( IDC_STARTFRAME );
	m_end_wnd = GetDlgItem( IDC_ENDFRAME );

	return 0;
}

int CSndSettingDlg::Params2Dlg()
{
	char tempchar[1024];
	m_name_wnd.SetWindowText( m_name );


	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%d", m_totalframe );
	m_totalframe_wnd.SetWindowText( tempchar );


	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%d", m_startframe );
	m_start_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%d", m_endframe );
	m_end_wnd.SetWindowText( tempchar );

	switch( m_mottype ){
	case MOTION_CLAMP:
		m_dlg_wnd.CheckRadioButton( IDC_R_MOTINV, IDC_R_MOTJUMP, IDC_R_MOTSTOP );
		break;
	case MOTION_ROUND:
		m_dlg_wnd.CheckRadioButton( IDC_R_MOTINV, IDC_R_MOTJUMP, IDC_R_MOTREP );
		break;
	default:
		_ASSERT( 0 );
		m_dlg_wnd.CheckRadioButton( IDC_R_MOTINV, IDC_R_MOTJUMP, IDC_R_MOTSTOP );
		m_mottype = MOTION_CLAMP;
		break;
	}

	return 0;
}

