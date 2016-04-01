// TargetBoneDlg.cpp : CTargetBoneDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "TargetBoneDlg.h"

#include "CamKeyDlg.h"
#include <treehandler2.h>

#include <Commdlg.h>
//#include <Afxdlgs.h>



/////////////////////////////////////////////////////////////////////////////
// CTargetBoneDlg

CTargetBoneDlg::CTargetBoneDlg( CCamKeyDlg* srccamdlg )
{
	m_cmdshow = SW_HIDE;
	m_camdlg = srccamdlg;

	ZeroMemory( m_name, 256 );
	m_chk = 0;

}

CTargetBoneDlg::~CTargetBoneDlg()
{
}

LRESULT CTargetBoneDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

int CTargetBoneDlg::ShowDlg( int srccmd, int srcchk, char* srcname, CTreeHandler2* srcth, CShdHandler* srcsh )
{
	m_th = srcth;
	m_sh = srcsh;

	m_cmdshow = srccmd;

	if( m_cmdshow != SW_HIDE ){
		m_chk = srcchk;
		if( m_chk ){
			m_dlg_wnd.CheckDlgButton( IDC_CHKCHUCHU, BST_CHECKED );
		}else{
			m_dlg_wnd.CheckDlgButton( IDC_CHKCHUCHU, BST_UNCHECKED );
		}

		SetBoneName( srcname );//内部でParamsToDlgが呼ばれる。
	}

	ShowWindow( m_cmdshow );

	return 0;
}


LRESULT CTargetBoneDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKCHUCHU );
	if( ischecked == BST_CHECKED ){
		m_chk = 1;
	}else{
		m_chk = 0;
	}

	//m_camdlgへセット
	if( m_chk == 1 ){
		int ret;
		int boneno = -1;
		ret = m_th->GetBoneNoByName( m_name, &boneno, m_sh, 0 );
		if( (ret == 0) && (boneno > 0) ){
			m_camdlg->m_camera.looktype = CAML_LOCK;
			m_camdlg->m_camera.hsid = 1;
			m_camdlg->m_camera.boneno = boneno;
		}else{
			m_camdlg->m_camera.looktype = CAML_NORMAL;
			m_camdlg->m_camera.hsid = -1;
			m_camdlg->m_camera.boneno = -1;
		}
	}else{
		m_camdlg->m_camera.looktype = CAML_NORMAL;
		m_camdlg->m_camera.hsid = -1;
		m_camdlg->m_camera.boneno = -1;
	}

	ShowDlg( SW_HIDE, 0, 0, 0, 0 );

	return 0;
}

LRESULT CTargetBoneDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE, 0, 0, 0, 0 );
	return 0;
}

void CTargetBoneDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_bonename_wnd = GetDlgItem( IDC_BONENAME );
	m_chkchuchu_wnd = GetDlgItem( IDC_CHKCHUCHU );
}

int CTargetBoneDlg::ParamsToDlg()
{	
	m_bonename_wnd.SetWindowText( m_name );

	if( m_chk ){
		m_dlg_wnd.CheckDlgButton( IDC_CHKCHUCHU, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHKCHUCHU, BST_UNCHECKED );
	}

	return 0;
}

int CTargetBoneDlg::SetBoneName( char* srcname )
{
	if( srcname && *srcname ){
		strcpy_s( m_name, 256, srcname );
	}else{
		ZeroMemory( m_name, sizeof( char ) * 256 );
	}

	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHKCHUCHU );
	if( ischecked == BST_CHECKED ){
		m_chk = 1;
	}else{
		m_chk = 0;
	}

	ParamsToDlg();

	return 0;
}

