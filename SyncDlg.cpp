// SyncDlg.cpp : CSyncDlg のインプリメンテーション
#include "stdafx.h"
#include "SyncDlg.h"
#include "GetDlgParams.h"

#include "MotDlg.h"

#include "viewer.h"
#include "SndKeyDlg.h"

#include <SndConv.h>

#include "GetDlgParams.h"

#include "inidata.h"

#define DBGH
#include <dbg.h>

static int s_fps[11] = {
	60, 55, 50, 45, 40, 35, 30, 15, 10, 6, 3
};

static char s_fpsstr[11][4] = {
	"60",
	"55",
	"50",
	"45",
	"40",
	"35",
	"30",
	"15",
	"10",
	"6",
	"3"
};

extern CMotDlg* g_motdlg;
extern int g_endapp;
extern CIniData* g_inidata;

/////////////////////////////////////////////////////////////////////////////
// CSyncDlg
CSyncDlg::CSyncDlg( CMyD3DApplication* appptr )
{
	m_app = appptr;
	m_snddlg = 0;

//	m_fps = s_fps[0];
	m_fps =	g_inidata->opt_previewfps;

	m_bpm = 100;
	m_timing.haku = 0.0f;
	m_timing.sec = 0.0f;
	m_timing.frame = 0.0f;

	m_quaflag = 0;
	m_camflag = 0;
	m_gpaflag = 0;
	m_moeflag = 0;
	m_sndflag = 0;

	m_startframe = 0;
	m_endframe = 60;

}

CSyncDlg::~CSyncDlg()
{
}

int CSyncDlg::SetSndDlg( CSndKeyDlg* sndptr )
{
	m_snddlg = sndptr;
	return 0;
}

LRESULT CSyncDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{


	m_dlg_wnd = m_hWnd;
	m_combo_fps_wnd = GetDlgItem( IDC_FPS );
	int combono;
	for( combono = 0; combono < 11; combono++ ){
		m_combo_fps_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)s_fpsstr[combono] );
	}
	m_bpm_wnd = GetDlgItem( IDC_BPM );
	m_haku_wnd = GetDlgItem( IDC_HAKU );
	m_sec_wnd = GetDlgItem( IDC_SEC );
	m_frame_wnd = GetDlgItem( IDC_FRAME );

	m_qua_wnd = GetDlgItem( IDC_QUA );
	m_cam_wnd = GetDlgItem( IDC_CAM );
	m_gpa_wnd = GetDlgItem( IDC_GPA );
	m_moe_wnd = GetDlgItem( IDC_MOE );
	m_snd_wnd = GetDlgItem( IDC_SND );

	m_start_wnd = GetDlgItem( IDC_START );
	m_end_wnd = GetDlgItem( IDC_END );

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

int CSyncDlg::ParamsToDlg()
{
	int comboindex = 0;
	switch( m_fps ){
	case 60:
		comboindex = 0;
		break;
	case 55:
		comboindex = 1;
		break;
	case 50:
		comboindex = 2;
		break;
	case 45:
		comboindex = 3;
		break;
	case 40:
		comboindex = 4;
		break;
	case 35:
		comboindex = 5;
		break;
	case 30:
		comboindex = 6;
		break;
	case 15:
		comboindex = 7;
		break;
	case 10:
		comboindex = 8;
		break;
	case 6:
		comboindex = 9;
		break;
	case 3:
		comboindex = 10;
		break;
	default:
		m_fps = 60;//!!!
		comboindex = 0;
		break;
	}
	m_combo_fps_wnd.SendMessage( CB_SETCURSEL, comboindex, 0 );


	char tempchar[256];
	sprintf_s( tempchar, 256, "%.1f", m_timing.haku );
	m_haku_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%.1f", m_timing.sec );
	m_sec_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%.1f", m_timing.frame );
	m_frame_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%d", m_bpm );
	m_bpm_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%d", m_startframe );
	m_start_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%d", m_endframe );
	m_end_wnd.SetWindowText( tempchar );


	return 0;
}

int CSyncDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;

	ShowWindow( m_cmdshow );

	if( m_cmdshow == SW_SHOW ){
		SetRangeButton();
		ParamsToDlg();
	}

	return 0;
}

LRESULT CSyncDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)7002, 0 );

//	ShowDlg( SW_HIDE );
	
//	int combono;
//	combono = (int)m_combo_fps_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
//	if( combono == CB_ERR )
//		return 0;
//	m_fps = s_fps[combono];
//	EndDialog(wID);
	return 0;
}

LRESULT CSyncDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_app->m_hWnd, WM_COMMAND, (WPARAM)7002, 0 );

//	ShowDlg( SW_HIDE );

//	EndDialog(wID);
	return 0;
}


LRESULT CSyncDlg::OnFPS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_combo_fps_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	m_fps = s_fps[combono];

	if( m_snddlg ){
		m_snddlg->ChangeFPS( m_fps );
	}

	g_inidata->opt_previewfps = m_fps;

	return 0;
}
LRESULT CSyncDlg::OnApplyBPM(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int tmpint;
	ret = GetInt( m_bpm_wnd, &tmpint );
	if( ret || (tmpint <= 0 ) ){
		::MessageBox( m_hWnd, "BPMの値が不正です。", "エラー", MB_OK );
		return 0;
	}

	m_bpm = tmpint;

	return 0;
}
LRESULT CSyncDlg::OnApplyHaku(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	ret = GetFloat( m_haku_wnd,&tmpfloat );
	if( ret || (tmpfloat < 0.0f) ){
		::MessageBox( m_hWnd, "拍の値が不正です。", "エラー", MB_OK );
		return 0;
	}
	m_timing.haku = tmpfloat;

	ret = SCConvHaku( m_fps, m_bpm, &m_timing );
	_ASSERT( !ret );

	ParamsToDlg();

	return 0;
}
LRESULT CSyncDlg::OnApplySec(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	ret = GetFloat( m_sec_wnd,&tmpfloat );
	if( ret || (tmpfloat < 0.0f) ){
		::MessageBox( m_hWnd, "秒の値が不正です。", "エラー", MB_OK );
		return 0;
	}
	m_timing.sec = tmpfloat;

	ret = SCConvSec( m_fps, m_bpm, &m_timing );
	_ASSERT( !ret );

	ParamsToDlg();

	return 0;
}
LRESULT CSyncDlg::OnApplyFrame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	ret = GetFloat( m_frame_wnd,&tmpfloat );
	if( ret || (tmpfloat < 0.0f) ){
		::MessageBox( m_hWnd, "Frameの値が不正です。", "エラー", MB_OK );
		return 0;
	}
	m_timing.frame = tmpfloat;

	ret = SCConvFrame( m_fps, m_bpm, &m_timing );
	_ASSERT( !ret );

	ParamsToDlg();

	return 0;
}
LRESULT CSyncDlg::OnJumpFrame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int frameno;

	frameno = (int)( m_timing.frame + 0.5f );

	ret = g_motdlg->OnJumpFrame( frameno, m_quaflag, m_camflag, m_gpaflag, m_moeflag, m_sndflag );
	_ASSERT( !ret );

	return 0;
}

LRESULT CSyncDlg::OnSelQua(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_QUA );
	if( ischecked == BST_CHECKED ){
		m_quaflag = 1;
	}else{
		m_quaflag = 0;
	}

	return 0;
}
LRESULT CSyncDlg::OnSelCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CAM );
	if( ischecked == BST_CHECKED ){
		m_camflag = 1;
	}else{
		m_camflag = 0;
	}

	return 0;
}
LRESULT CSyncDlg::OnSelGpa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_GPA );
	if( ischecked == BST_CHECKED ){
		m_gpaflag = 1;
	}else{
		m_gpaflag = 0;
	}

	return 0;
}
LRESULT CSyncDlg::OnSelMoe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_MOE );
	if( ischecked == BST_CHECKED ){
		m_moeflag = 1;
	}else{
		m_moeflag = 0;
	}

	return 0;
}
LRESULT CSyncDlg::OnSelSnd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_SND );
	if( ischecked == BST_CHECKED ){
		m_sndflag = 1;
	}else{
		m_sndflag = 0;
	}

	return 0;
}
int CSyncDlg::SetRangeButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_quaflag ){
		::CheckDlgButton( m_dlg_wnd, IDC_QUA, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_QUA, BST_UNCHECKED );
	}

	if( m_camflag ){
		::CheckDlgButton( m_dlg_wnd, IDC_CAM, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_CAM, BST_UNCHECKED );
	}

	if( m_gpaflag ){
		::CheckDlgButton( m_dlg_wnd, IDC_GPA, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_GPA, BST_UNCHECKED );
	}

	if( m_moeflag ){
		::CheckDlgButton( m_dlg_wnd, IDC_MOE, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_MOE, BST_UNCHECKED );
	}

	if( m_sndflag ){
		::CheckDlgButton( m_dlg_wnd, IDC_SND, BST_CHECKED );
	}else{
		::CheckDlgButton( m_dlg_wnd, IDC_SND, BST_UNCHECKED );
	}

	return 0;
}

/***
int CSyncDlg::SetRangeButton()
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_quaflag ){
		::SendMessage( m_qua_wnd, BM_SETSTATE, TRUE, 0 );
	}else{
		::SendMessage( m_qua_wnd, BM_SETSTATE, FALSE, 0 );
	}

	if( m_camflag ){
		::SendMessage( m_cam_wnd, BM_SETSTATE, TRUE, 0 );
	}else{
		::SendMessage( m_cam_wnd, BM_SETSTATE, FALSE, 0 );
	}

	if( m_gpaflag ){
		::SendMessage( m_gpa_wnd, BM_SETSTATE, TRUE, 0 );
	}else{
		::SendMessage( m_gpa_wnd, BM_SETSTATE, FALSE, 0 );
	}

	if( m_moeflag ){
		::SendMessage( m_moe_wnd, BM_SETSTATE, TRUE, 0 );
	}else{
		::SendMessage( m_moe_wnd, BM_SETSTATE, FALSE, 0 );
	}

	if( m_sndflag ){
		::SendMessage( m_snd_wnd, BM_SETSTATE, TRUE, 0 );
	}else{
		::SendMessage( m_snd_wnd, BM_SETSTATE, FALSE, 0 );
	}

	return 0;
}
***/

LRESULT CSyncDlg::OnApplyRange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}
	if( !g_motdlg ){
		return 0;
	}

	int ret;
	int tmpstart, tmpend;

	ret = GetInt( m_start_wnd, &tmpstart );
	if( ret || (tmpstart < 0) ){
		::MessageBox( m_hWnd, "表示範囲の開始フレームが不正です。", "エラー", MB_OK );
		return 0;
	}
	ret = GetInt( m_end_wnd, &tmpend );
	if( ret || (tmpend < tmpstart) ){
		::MessageBox( m_hWnd, "表示範囲の終了フレームが不正です。", "エラー", MB_OK );
		return 0;
	}
	m_startframe = tmpstart;
	m_endframe = tmpend;

	if( m_quaflag && g_motdlg->m_motparamdlg ){
		ret = g_motdlg->m_motparamdlg->SetStartAndEnd( m_startframe, m_endframe );
		_ASSERT( !ret );
	}
	if( m_camflag && g_motdlg->m_camdlg ){
		ret = g_motdlg->m_camdlg->SetStartAndEnd( m_startframe, m_endframe );
		_ASSERT( !ret );
	}
	if( m_gpaflag && g_motdlg->m_gpdlg ){
		ret = g_motdlg->m_gpdlg->SetStartAndEnd( m_startframe, m_endframe );
		_ASSERT( !ret );
	}
	if( m_moeflag && g_motdlg->m_moedlg ){
		ret = g_motdlg->m_moedlg->SetStartAndEnd( m_startframe, m_endframe );
		_ASSERT( !ret );
	}
	if( m_sndflag && g_motdlg->m_snddlg ){
		ret = g_motdlg->m_snddlg->SetStartAndEnd( m_startframe, m_endframe );
		_ASSERT( !ret );
	}

	return 0;
}

