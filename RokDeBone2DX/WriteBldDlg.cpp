// WriteBldDlg.cpp : CWriteBldDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "WriteBldDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#define DBGH
#include <dbg.h>

#include "viewer.h"
#include "MotParamDlg.h"

#include "MotChangeDlg.h"
#include "GPKeyDlg.h"
#include <GPAnimHandler.h>
#include <GPAnim.h>
#include "MOEKeyDlg.h"
#include <MOEAnimHandler.h>
#include <MOEAnim.h>
#include "CamKeyDlg.h"
#include <CameraAnimHandler.h>
#include <CameraAnim.h>
#include "SoundBank.h"
#include "SoundSet.h"
#include "SndKeyDlg.h"
#include <SndAnimHandler.h>
#include <SndAnim.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CWriteBldDlg

CWriteBldDlg::CWriteBldDlg( CMyD3DApplication* papp, CMotParamDlg* mpdlg )
{
	InitParams();
	m_papp = papp;
	m_mpdlg = mpdlg;
}

CWriteBldDlg::~CWriteBldDlg()
{
}
int CWriteBldDlg::InitMult()
{
	m_sigmult = 1.0f;
	m_gmult = 1.0f;
	return 0;
}

int CWriteBldDlg::InitParams()
{
	InitMult();

	ZeroMemory( m_bldname, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_signame, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_gname, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_moaname, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_gpaname, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_moename, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_camname, sizeof( char ) * MAX_PATH );
	m_cammode = 0;
	ZeroMemory( m_ssfname, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_safname, sizeof( char ) * MAX_PATH );

	m_createflag = 0;

	m_gflag = 0;
	m_gpaflag = 0;
	m_moeflag = 0;
	m_camflag = 0;
	m_ssfflag = 0;
	m_safflag = 0;


	m_chkgpa = 0;
	m_chkmoe = 0;
	m_chkcam = 0;
	m_chkssf = 0;
	m_chksaf = 0;

	return 0;
}

LRESULT CWriteBldDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

//DbgOut( "WriteBldDlg : OnInitDialog\r\n" );

	SetWnd();

	if( m_createflag == 0 ){
		
		m_cammode_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"KAE" );
		m_cammode_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"Default" );

		m_createflag = 1;
	}

	SetFlag();

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteBldDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = GetText( m_bld_wnd, m_bldname, MAX_PATH );
	if( ret ){
		::MessageBox( m_hWnd, "bldのパスが不正です。", "エラー", MB_OK );
		return 0;
	}
	int ch = '\\';
	int ch2 = ':';
	char* lastenptr;
	lastenptr = strrchr( m_bldname, ch );
	char* colonptr;
	colonptr = strchr( m_bldname, ch2 );
	if( !lastenptr || !colonptr ){
		::MessageBox( m_hWnd, "bldのパスがフルパスではありません。フルパスを記述してください。", "エラー", MB_OK );
		return 0;
	}
///////	
	ret = GetText( m_sig_wnd, m_signame, MAX_PATH );
	if( ret || (m_signame[0] == 0) ){
		::MessageBox( m_hWnd, "sigのパスが不正です。", "エラー", MB_OK );
		return 0;
	}
	char relsig[MAX_PATH];
	ZeroMemory( relsig, sizeof( char ) * MAX_PATH );
	ret = m_papp->GetRelatedPath( m_bldname, m_signame, relsig, MAX_PATH );
	if( ret || (relsig[0] == 0) ){
		::MessageBox( m_hWnd, "sigのフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_sigmult_wnd, &m_sigmult );
	if( ret || (m_sigmult <= 0.0f) ){
		::MessageBox( m_hWnd, "sigの倍率は０より大きくないといけません。", "エラー", MB_OK );
		return 0;
	}
////////
	if( m_gflag ){
		ret = GetText( m_g_wnd, m_gname, MAX_PATH );
		if( ret || (m_gname[0] == 0) ){
			::MessageBox( m_hWnd, "地面のパスが不正です。", "エラー", MB_OK );
			return 0;
		}
		char relg[MAX_PATH];
		ZeroMemory( relg, sizeof( char ) * MAX_PATH );
		ret = m_papp->GetRelatedPath( m_bldname, m_gname, relg, MAX_PATH );
		if( ret || (relg[0] == 0) ){
			::MessageBox( m_hWnd, "地面のフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
			return 0;
		}
		ret = GetFloat( m_gmult_wnd, &m_gmult );
		if( ret || (m_gmult <= 0.0f) ){
			::MessageBox( m_hWnd, "地面の倍率は０より大きくないといけません。", "エラー", MB_OK );
			return 0;
		}
	}
///////////
	ret = GetText( m_moa_wnd, m_moaname, MAX_PATH );
	if( ret || (m_moaname[0] == 0) ){
		::MessageBox( m_hWnd, "moaのパスが不正です。", "エラー", MB_OK );
		return 0;
	}
	char relmoa[MAX_PATH];
	ZeroMemory( relmoa, sizeof( char ) * MAX_PATH );
	ret = m_papp->GetRelatedPath( m_bldname, m_moaname, relmoa, MAX_PATH );
	if( ret || (relmoa[0] == 0) ){
		::MessageBox( m_hWnd, "moaのフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
		return 0;
	}
////////////
	if( m_chkgpa ){
		ret = GetText( m_gpa_wnd, m_gpaname, MAX_PATH );
		if( ret || (m_gpaname[0] == 0) ){
			::MessageBox( m_hWnd, "GPAのパスが不正です。", "エラー", MB_OK );
			return 0;
		}
		char relgpa[MAX_PATH];
		ZeroMemory( relgpa, sizeof( char ) * MAX_PATH );
		ret = m_papp->GetRelatedPath( m_bldname, m_gpaname, relgpa, MAX_PATH );
		if( ret || (relgpa[0] == 0) ){
			::MessageBox( m_hWnd, "GPAのフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
			return 0;
		}
	}
///////////
	if( m_chkmoe ){
		ret = GetText( m_moe_wnd, m_moename, MAX_PATH );
		if( ret || (m_moename[0] == 0) ){
			::MessageBox( m_hWnd, "MOEのパスが不正です。", "エラー", MB_OK );
			return 0;
		}
		char relmoe[MAX_PATH];
		ZeroMemory( relmoe, sizeof( char ) * MAX_PATH );
		ret = m_papp->GetRelatedPath( m_bldname, m_moename, relmoe, MAX_PATH );
		if( ret || (relmoe[0] == 0) ){
			::MessageBox( m_hWnd, "MOEのフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
			return 0;
		}
	}
////////////
	if( m_chkcam ){
		ret = GetText( m_cam_wnd, m_camname, MAX_PATH );
		if( ret || (m_camname[0] == 0) ){
			::MessageBox( m_hWnd, "camのパスが不正です。", "エラー", MB_OK );
			return 0;
		}
		char relcam[MAX_PATH];
		ZeroMemory( relcam, sizeof( char ) * MAX_PATH );
		ret = m_papp->GetRelatedPath( m_bldname, m_camname, relcam, MAX_PATH );
		if( ret || (relcam[0] == 0) ){
			::MessageBox( m_hWnd, "CAMのフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
			return 0;
		}

		int combono;
		combono = (int)m_cammode_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
		if( combono == CB_ERR ){
			m_cammode = 0;
		}else{		
			m_cammode = combono;
		}
	}
/////////////

	if( m_chkssf ){
		ret = GetText( m_ssf_wnd, m_ssfname, MAX_PATH );
		if( ret || (m_ssfname[0] == 0) ){
			::MessageBox( m_hWnd, "ssfのパスが不正です。", "エラー", MB_OK );
			return 0;
		}
		char relssf[MAX_PATH];
		ZeroMemory( relssf, sizeof( char ) * MAX_PATH );
		ret = m_papp->GetRelatedPath( m_bldname, m_ssfname, relssf, MAX_PATH );
		if( ret || (relssf[0] == 0) ){
			::MessageBox( m_hWnd, "SSFのフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
			return 0;
		}

		if( m_chksaf ){
			ret = GetText( m_saf_wnd, m_safname, MAX_PATH );
			if( ret || (m_safname[0] == 0) ){
				::MessageBox( m_hWnd, "safのパスが不正です。", "エラー", MB_OK );
				return 0;
			}
			char relsaf[MAX_PATH];
			ZeroMemory( relsaf, sizeof( char ) * MAX_PATH );
			ret = m_papp->GetRelatedPath( m_bldname, m_safname, relsaf, MAX_PATH );
			if( ret || (relsaf[0] == 0) ){
				::MessageBox( m_hWnd, "SAFのフォルダはbldのフォルダと同じかそれより下の階層でなければなりません。", "エラー", MB_OK );
				return 0;
			}
		}
	}

	EndDialog(wID);
	return 0;
}

LRESULT CWriteBldDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	EndDialog(wID);
	return 0;
}

void CWriteBldDlg::SetWnd()
{
	m_bld_wnd = GetDlgItem( IDC_BLD );
	_ASSERT( m_bld_wnd );
	m_sig_wnd = GetDlgItem( IDC_SIG );
	_ASSERT( m_sig_wnd );
	m_sigmult_wnd = GetDlgItem( IDC_SIGMULT );
	_ASSERT( m_sigmult_wnd );
	m_g_wnd = GetDlgItem( IDC_GROUND );
	_ASSERT( m_g_wnd );
	m_gmult_wnd = GetDlgItem( IDC_GROUNDMULT );
	_ASSERT( m_gmult_wnd );
	m_moa_wnd = GetDlgItem( IDC_MOA );
	_ASSERT( m_moa_wnd );
	m_gpa_wnd = GetDlgItem( IDC_GPA );
	_ASSERT( m_gpa_wnd );
	m_moe_wnd = GetDlgItem( IDC_MOE );
	_ASSERT( m_moe_wnd );
	m_cam_wnd = GetDlgItem( IDC_CAM );
	_ASSERT( m_cam_wnd );
	m_ssf_wnd = GetDlgItem( IDC_SSF );
	_ASSERT( m_ssf_wnd );
	m_saf_wnd = GetDlgItem( IDC_SAF );
	_ASSERT( m_saf_wnd );
	m_cammode_wnd = GetDlgItem( IDC_CAMMODE );
	_ASSERT( m_cammode_wnd );


	m_refg_wnd = GetDlgItem( IDC_REFGROUND );
	_ASSERT( m_refg_wnd );
	m_refgpa_wnd = GetDlgItem( IDC_REFGPA );
	_ASSERT( m_refgpa_wnd );
	m_refmoe_wnd = GetDlgItem( IDC_REFMOE );
	_ASSERT( m_refmoe_wnd );
	m_refcam_wnd = GetDlgItem( IDC_REFCAM );
	_ASSERT( m_refcam_wnd );
	m_refssf_wnd = GetDlgItem( IDC_REFSSF );
	_ASSERT( m_refssf_wnd );
	m_refsaf_wnd = GetDlgItem( IDC_REFSAF );
	_ASSERT( m_refsaf_wnd );

	m_chkgpa_wnd = GetDlgItem( IDC_CHKGPA );
	_ASSERT( m_chkgpa_wnd );
	m_chkmoe_wnd = GetDlgItem( IDC_CHKMOE );
	_ASSERT( m_chkmoe_wnd );
	m_chkcam_wnd = GetDlgItem( IDC_CHKCAM );
	_ASSERT( m_chkcam_wnd );
	m_chkssf_wnd = GetDlgItem( IDC_CHKSSF );
	_ASSERT( m_chkssf_wnd );
	m_chksaf_wnd = GetDlgItem( IDC_CHKSAF );
	_ASSERT( m_chksaf_wnd );

	m_dlg_wnd = m_hWnd;

}

int CWriteBldDlg::ParamsToDlg()
{		
	char tempchar[256];
	m_bld_wnd.SetWindowText( m_bldname );
	m_sig_wnd.SetWindowText( m_signame );
	sprintf_s( tempchar, 256, "%f", m_sigmult );
	m_sigmult_wnd.SetWindowText( tempchar );
	m_g_wnd.SetWindowText( m_gname );
	sprintf_s( tempchar, 256, "%f", m_gmult );
	m_gmult_wnd.SetWindowText( tempchar );
	m_moa_wnd.SetWindowText( m_moaname );
	m_gpa_wnd.SetWindowText( m_gpaname );
	m_moe_wnd.SetWindowText( m_moename );
	m_cam_wnd.SetWindowText( m_camname );
	m_ssf_wnd.SetWindowText( m_ssfname );
	m_saf_wnd.SetWindowText( m_safname );


	m_cammode_wnd.SendMessage( CB_SETCURSEL, m_cammode, 0 );

	if( m_chkgpa == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKGPA, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKGPA, BST_UNCHECKED );
	}

	if( m_chkmoe == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKMOE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKMOE, BST_UNCHECKED );
	}

	if( m_chkcam == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKCAM, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKCAM, BST_UNCHECKED );
	}

	if( m_chkssf == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKSSF, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKSSF, BST_UNCHECKED );
	}

	if( m_chksaf == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKSAF, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKSAF, BST_UNCHECKED );
	}

	return 0;
}

int CWriteBldDlg::SetFlag()
{
	if( m_papp->m_g_th && m_papp->m_g_sh ){
		m_gflag = 1;
		m_g_wnd.EnableWindow( TRUE );
		m_refg_wnd.EnableWindow( TRUE );
	}else{
		m_gflag = 0;
		m_g_wnd.EnableWindow( FALSE );
		m_refg_wnd.EnableWindow( FALSE );
		ZeroMemory( m_gname, sizeof( char ) * MAX_PATH );
	}
///////////
	m_camflag = 1;
	if( (m_mpdlg->m_camkeydlg->m_cah->m_animnum <= 0) || (m_mpdlg->m_camkeydlg->m_animno < 0) ){
		m_camflag = 0;
	}else{
		CCameraAnim* anim;
		anim = m_mpdlg->m_camkeydlg->m_cah->GetCurCameraAnim();
		if( anim ){
			if( !anim->m_firstkey ){
				m_camflag = 0;
			}
		}else{
			m_camflag = 0;
		}
	}
	if( m_camflag == 0 ){
		m_cam_wnd.EnableWindow( FALSE );
		m_refcam_wnd.EnableWindow( FALSE );
		ZeroMemory( m_camname, sizeof( char ) * MAX_PATH );
		m_chkcam = 0;
		m_chkcam_wnd.EnableWindow( FALSE );
	}else{
		m_chkcam_wnd.EnableWindow( TRUE );
		if( m_chkcam == 1 ){
			m_cam_wnd.EnableWindow( TRUE );
			m_refcam_wnd.EnableWindow( TRUE );
		}else{
			m_cam_wnd.EnableWindow( FALSE );
			m_refcam_wnd.EnableWindow( FALSE );
		}
	}

/////////////
	m_gpaflag = 1;
	if( (m_mpdlg->m_gpkeydlg->m_gpah->m_animnum <= 0) || (m_mpdlg->m_gpkeydlg->m_animno < 0) ){
		m_gpaflag = 0;
	}else{
		CGPAnim* anim;
		anim = m_mpdlg->m_gpkeydlg->m_gpah->GetCurGPAnim();
		if( anim ){
			if( !anim->m_firstkey ){
				m_gpaflag = 0;
			}
		}else{
			m_gpaflag = 0;
		}
	}
	if( m_gpaflag == 0 ){
		m_gpa_wnd.EnableWindow( FALSE );
		m_refgpa_wnd.EnableWindow( FALSE );
		ZeroMemory( m_gpaname, sizeof( char ) * MAX_PATH );
		m_chkgpa = 0;
		m_chkgpa_wnd.EnableWindow( FALSE );
	}else{
		m_chkgpa_wnd.EnableWindow( TRUE );
		if( m_chkgpa == 1 ){
			m_gpa_wnd.EnableWindow( TRUE );
			m_refgpa_wnd.EnableWindow( TRUE );
		}else{
			m_gpa_wnd.EnableWindow( FALSE );
			m_refgpa_wnd.EnableWindow( FALSE );
		}
	}

/////////
	m_moeflag = 1;
	if( (m_mpdlg->m_moekeydlg->m_moeah->m_animnum <= 0) || (m_mpdlg->m_moekeydlg->m_animno < 0) ){
		m_moeflag = 0;
	}else{
		CMOEAnim* anim;
		anim = m_mpdlg->m_moekeydlg->m_moeah->GetCurMOEAnim();
		if( anim ){
			if( !anim->m_firstkey ){
				m_moeflag = 0;
			}
		}else{
			m_moeflag = 0;
		}
	}
	if( m_moeflag == 0 ){
		m_moe_wnd.EnableWindow( FALSE );
		m_refmoe_wnd.EnableWindow( FALSE );
		ZeroMemory( m_moename, sizeof( char ) * MAX_PATH );
		m_chkmoe = 0;
		m_chkmoe_wnd.EnableWindow( FALSE );
	}else{
		m_chkmoe_wnd.EnableWindow( TRUE );
		if( m_chkmoe == 1 ){
			m_moe_wnd.EnableWindow( TRUE );
			m_refmoe_wnd.EnableWindow( TRUE );
		}else{
			m_moe_wnd.EnableWindow( FALSE );
			m_refmoe_wnd.EnableWindow( FALSE );
		}
	}
///////

	if( m_papp->m_soundbank->m_setnum <= 0 ){
		m_ssfflag = 0;
		m_ssf_wnd.EnableWindow( FALSE );
		m_refssf_wnd.EnableWindow( FALSE );
		ZeroMemory( m_ssfname, sizeof( char ) * MAX_PATH );
		m_chkssf = 0;
		m_chkssf_wnd.EnableWindow( FALSE );
	}else{
		m_ssfflag = 1;
		m_chkssf_wnd.EnableWindow( TRUE );
		if( m_chkssf == 1 ){
			m_ssf_wnd.EnableWindow( TRUE );
			m_refssf_wnd.EnableWindow( TRUE );
		}else{
			m_ssf_wnd.EnableWindow( FALSE );
			m_refssf_wnd.EnableWindow( FALSE );
		}
	}

////////////////

	if( (m_ssfflag == 0) || (m_chkssf == 0) || (m_mpdlg->m_sndkeydlg->m_sndah->m_animnum <= 0) || (m_mpdlg->m_sndkeydlg->m_animno < 0) ){
		m_safflag = 0;
		m_saf_wnd.EnableWindow( FALSE );
		m_refsaf_wnd.EnableWindow( FALSE );
		ZeroMemory( m_safname, sizeof( char ) * MAX_PATH );
		m_chksaf = 0;
		m_chksaf_wnd.EnableWindow( FALSE );
	}else{
		m_safflag = 1;
		m_chksaf_wnd.EnableWindow( TRUE );
		if( m_chksaf == 1 ){
			m_saf_wnd.EnableWindow( TRUE );
			m_refsaf_wnd.EnableWindow( TRUE );
		}else{
			m_saf_wnd.EnableWindow( FALSE );
			m_refsaf_wnd.EnableWindow( FALSE );
		}
	}

	return 0;
}

LRESULT CWriteBldDlg::OnCamMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_cammode_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	
	m_cammode = combono;

	ParamsToDlg();

	return 0;
}

LRESULT CWriteBldDlg::OnRefBld(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.bld" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"bld File (*.bld)\0*.bld\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="bld";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_bldname, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefSig(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.sig" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"sig File (*.sig)\0*.sig\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="sig";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_signame, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefGround(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "defaultg.sig" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"sig File (*.sig)\0*.sig\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="sig";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_gname, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefMoa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.moa" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"moa File (*.moa)\0*.moa\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="moa";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_moaname, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefGpa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.gpa" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"gpa File (*.gpa)\0*.gpa\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="gpa";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_gpaname, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefMoe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.moe" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"moe File (*.moe)\0*.moe\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="moe";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_moename, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.cam" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"cam File (*.cam)\0*.cam\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="cam";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_camname, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefSsf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.ssf" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"ssf File (*.ssf)\0*.ssf\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="ssf";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_ssfname, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}
LRESULT CWriteBldDlg::OnRefSaf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[MAX_PATH];
	strcpy_s( buf, MAX_PATH, "default.saf" );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"saf File (*.saf)\0*.saf\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="saf";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	strcpy_s( m_safname, MAX_PATH, buf );

	ParamsToDlg();

	return 0;
}

LRESULT CWriteBldDlg::OnChkGpa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKGPA );
	if( checkflag == BST_CHECKED ){
		m_chkgpa = 1;
		m_gpa_wnd.EnableWindow( TRUE );
		m_refgpa_wnd.EnableWindow( TRUE );

	}else if( checkflag == BST_UNCHECKED ){
		m_chkgpa = 0;
		m_gpa_wnd.EnableWindow( FALSE );
		m_refgpa_wnd.EnableWindow( FALSE );

	}

	return 0;
}
LRESULT CWriteBldDlg::OnChkMoe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKMOE );
	if( checkflag == BST_CHECKED ){
		m_chkmoe = 1;
		m_moe_wnd.EnableWindow( TRUE );
		m_refmoe_wnd.EnableWindow( TRUE );

	}else if( checkflag == BST_UNCHECKED ){
		m_chkmoe = 0;
		m_moe_wnd.EnableWindow( FALSE );
		m_refmoe_wnd.EnableWindow( FALSE );
	}

	return 0;
}
LRESULT CWriteBldDlg::OnChkCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKCAM );
	if( checkflag == BST_CHECKED ){
		m_chkcam = 1;
		m_cam_wnd.EnableWindow( TRUE );
		m_refcam_wnd.EnableWindow( TRUE );

	}else if( checkflag == BST_UNCHECKED ){
		m_chkcam = 0;
		m_cam_wnd.EnableWindow( FALSE );
		m_refcam_wnd.EnableWindow( FALSE );
	}

	return 0;
}
LRESULT CWriteBldDlg::OnChkSsf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKSSF );
	if( checkflag == BST_CHECKED ){
		m_chkssf = 1;
		m_chksaf_wnd.EnableWindow( TRUE );

		m_ssf_wnd.EnableWindow( TRUE );
		m_refssf_wnd.EnableWindow( TRUE );

		if( m_chksaf == 0 ){
			m_saf_wnd.EnableWindow( FALSE );
			m_refsaf_wnd.EnableWindow( FALSE );
		}else{
			m_saf_wnd.EnableWindow( TRUE );
			m_refsaf_wnd.EnableWindow( TRUE );
		}

	}else if( checkflag == BST_UNCHECKED ){
		m_chkssf = 0;
	
		m_ssf_wnd.EnableWindow( FALSE );
		m_refssf_wnd.EnableWindow( FALSE );

		m_chksaf = 0;
		m_chksaf_wnd.EnableWindow( FALSE );

		m_saf_wnd.EnableWindow( FALSE );
		m_refsaf_wnd.EnableWindow( FALSE );
	}
	return 0;
}
LRESULT CWriteBldDlg::OnChkSaf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKSAF );
	if( checkflag == BST_CHECKED ){
		m_chksaf = 1;
		m_saf_wnd.EnableWindow( TRUE );
		m_refsaf_wnd.EnableWindow( TRUE );

	}else if( checkflag == BST_UNCHECKED ){
		m_chksaf = 0;
		m_saf_wnd.EnableWindow( FALSE );
		m_refsaf_wnd.EnableWindow( FALSE );
	}

	return 0;
}

