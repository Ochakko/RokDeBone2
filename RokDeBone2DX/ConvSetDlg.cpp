// ConvSetDlg.cpp : CConvSetDlg のインプリメンテーション
#include "stdafx.h"

#include "ConvSetDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "txtsetting.h"

#include "viewer.h"
#include "shdhandler.h"
#include "shdelem.h"
#include "treehandler2.h"
#include "treeelem2.h"

/////////////////////////////////////////////////////////////////////////////
// CConvSetDlg

static int s_chkid[SKEL_MAX] = 
{
	IDC_CHKTOPOFJOINT,
	IDC_CHKTORSO,
	IDC_CHKLEFTHIP,
	IDC_CHKLEFTKNEE,
	IDC_CHKLEFTFOOT,
	IDC_CHKRIGHTHIP,
	IDC_CHKRIGHTKNEE,
	IDC_CHKRIGHTFOOT,
	IDC_CHKNECK,
	IDC_CHKHEAD,
	IDC_CHKLEFTSHOULDER,
	IDC_CHKLEFTELBOW,
	IDC_CHKLEFTHAND,
	IDC_CHKRIGHTSHOULDER,
	IDC_CHKRIGHTELBOW,
	IDC_CHKRIGHTHAND
};

/***
00TOP_OF_JOINT, "TopOfmat1", "twist"
01SKEL_TORSO, "mat1", "twist"
02SKEL_LEFT_HIP, "FLOAT_モモ[L]_X+", "twist"
03SKEL_LEFT_KNEE, "モモ[L]_X+", "nottwist"
04SKEL_LEFT_FOOT, "スネ[L]_X+", "twist"
05SKEL_RIGHT_HIP, "FLOAT_モモ[R]_X-", "twist"
06SKEL_RIGHT_KNEE, "モモ[R]_X-", "nottwist"
07SKEL_RIGHT_FOOT, "スネ[R]_X-", "twist"
08SKEL_NECK, "胸", "twist"
09SKEL_HEAD, "頭", "twist"
10SKEL_LEFT_SHOULDER, "FLOAT_肩[L]_X+", "twist"
11SKEL_LEFT_ELBOW, "肩[L]_X+", "nottwist"
12SKEL_LEFT_HAND, "腕[L]_X+", "nottwist"
13SKEL_RIGHT_SHOULDER, "FLOAT_肩[R]_X-", "twist"
14SKEL_RIGHT_ELBOW, "肩[R]_X-", "nottwist"
15SKEL_RIGHT_HAND, "腕[R]_X-", "nottwist"

***/

int s_deftwist[SKEL_MAX] = {
	1, 1,

	1, 0, 1,
	1, 0, 1,

	1, 1,

	1, 0, 0,
	1, 0, 0
};



CConvSetDlg::CConvSetDlg()
{
	m_papp = 0;
	m_txtsetting = 0;
	m_cmdshow = SW_HIDE;

	ZeroMemory( m_kstname, sizeof( char ) * MAX_PATH );
	strcpy_s( m_kstname, MAX_PATH, "default.kst" );

	m_bonenum = 0;
	m_jointinfo = 0;

}

CConvSetDlg::~CConvSetDlg()
{
	if( m_jointinfo ){
		free( m_jointinfo );
		m_jointinfo = 0;
	}
	m_bonenum = 0;

}

LRESULT CConvSetDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
//	SetCombo();
//	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CConvSetDlg::OnRefKst(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char tmpname[MAX_PATH];
	if( m_kstname[0] != 0 ){
		strcpy_s( tmpname, MAX_PATH, m_kstname );
	}else{
		tmpname[0] = 0;
	}

	OPENFILENAME ofn;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"KST File (*.kst)\0*.kst\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)tmpname;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="kst";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return S_OK;
	}

	strcpy_s( m_kstname, MAX_PATH, tmpname );
	m_kstname_wnd.SetWindowText( m_kstname );


	return 0;
}

LRESULT CConvSetDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int cno;
	for( cno = 0; cno < SKEL_MAX; cno++ ){
		TSELEM* curts;
		curts = m_txtsetting->m_elem + cno;

		int tmpcombo;
		tmpcombo = m_skel_wnd[cno].SendMessage( CB_GETCURSEL, 0, 0 );
		if( tmpcombo != CB_ERR ){
			UINT ischecked;
			int tmptwist;
			ischecked = m_dlg_wnd.IsDlgButtonChecked( s_chkid[ cno ] );
			if( ischecked == BST_CHECKED ){
				tmptwist = 0;
			}else{
				tmptwist = 1;
			}
			
			_ASSERT( tmpcombo < m_bonenum );

			int chkjno = -1;
			int ret;
			ret = m_papp->m_thandler->GetBoneNoByName( (m_jointinfo + tmpcombo)->name, &chkjno, m_papp->m_shandler, 0 );
			if( ret || (chkjno <= 0) ){
				::MessageBox( m_hWnd, "編集中にボーンの名前が変わった可能性があります。\n一度ダイアログを閉じてから再試行してください。", "エラー", MB_OK );
				return 0;
			}
			strcpy_s( curts->jointname, 256, (m_jointinfo + tmpcombo)->name );
			curts->jointno = (m_jointinfo + tmpcombo)->serialno;
			curts->skelno = cno;
			curts->twistflag = tmptwist;
		}else{
			strcpy_s( curts->jointname, 256, "存在しないジョイント" );
			curts->jointno = -1;
			curts->skelno = cno;
			curts->twistflag = 1;
		}
	}

	int ret;
	ret = m_txtsetting->SaveKstFile( m_kstname );
	if( ret ){
		_ASSERT( 0 );
		//EndDialog(wID);
		ShowDlg( SW_HIDE, 0, 0 );
		return 1;
	}

	ShowDlg( SW_HIDE, 0, 0 );

//	EndDialog(wID);
	return 0;
}

LRESULT CConvSetDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE, 0, 0 );

	//EndDialog(wID);
	return 0;
}

void CConvSetDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_skel_wnd[SKEL_TOPOFJOINT] = GetDlgItem( IDC_TOPOFJOINT );
	m_skel_wnd[SKEL_TORSO] = GetDlgItem( IDC_TORSO );
	
	m_skel_wnd[SKEL_LEFT_HIP] = GetDlgItem( IDC_LEFTHIP );
	m_skel_wnd[SKEL_LEFT_KNEE] = GetDlgItem( IDC_LEFTKNEE );
	m_skel_wnd[SKEL_LEFT_FOOT] = GetDlgItem( IDC_LEFTFOOT );

	m_skel_wnd[SKEL_RIGHT_HIP] = GetDlgItem( IDC_RIGHTHIP );
	m_skel_wnd[SKEL_RIGHT_KNEE] = GetDlgItem( IDC_RIGHTKNEE );
	m_skel_wnd[SKEL_RIGHT_FOOT] = GetDlgItem( IDC_RIGHTFOOT );

	m_skel_wnd[SKEL_NECK] = GetDlgItem( IDC_NECK );
	m_skel_wnd[SKEL_HEAD] = GetDlgItem( IDC_HEAD );

	m_skel_wnd[SKEL_LEFT_SHOULDER] = GetDlgItem( IDC_LEFTSHOULDER );
	m_skel_wnd[SKEL_LEFT_ELBOW] = GetDlgItem( IDC_LEFTELBOW );
	m_skel_wnd[SKEL_LEFT_HAND] = GetDlgItem( IDC_LEFTHAND );

	m_skel_wnd[SKEL_RIGHT_SHOULDER] = GetDlgItem( IDC_RIGHTSHOULDER );
	m_skel_wnd[SKEL_RIGHT_ELBOW] = GetDlgItem( IDC_RIGHTELBOW );
	m_skel_wnd[SKEL_RIGHT_HAND] = GetDlgItem( IDC_RIGHTHAND );

	m_kstname_wnd = GetDlgItem( IDC_KSTNAME );
	m_kstin_wnd = GetDlgItem( IDC_KSTIN );

}

int CConvSetDlg::ParamsToDlg()
{	
	int cno;
	for( cno = 0; cno < SKEL_MAX; cno++ ){
		m_skel_wnd[cno].SendMessage( CB_SETCURSEL, (WPARAM)0, 0 );
	}

	m_kstname_wnd.SetWindowText( m_kstname );

	int skno;
	for( skno = 0; skno < SKEL_MAX; skno++ ){
		if( s_deftwist[skno] == 0 ){
			m_dlg_wnd.CheckDlgButton( s_chkid[skno], BST_CHECKED );
		}else{
			m_dlg_wnd.CheckDlgButton( s_chkid[skno], BST_UNCHECKED );
		}
	}

	return 0;
}

int CConvSetDlg::SetCombo()
{
	int ret;

	int cno;
	for( cno = 0; cno < SKEL_MAX; cno++ ){
		m_skel_wnd[cno].SendMessage( CB_RESETCONTENT, 0, 0 );
	}

	if( m_jointinfo ){
		free( m_jointinfo );
		m_jointinfo = 0;
	}

	m_bonenum = 0;
	ret = GetJointInfo( 0, 0, &m_bonenum );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	if( m_bonenum <= 0 ){
		return 0;
	}


	m_jointinfo = (JOINTINFO*)malloc( sizeof( JOINTINFO ) * m_bonenum );
	if( !m_jointinfo ){
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( m_jointinfo, sizeof( JOINTINFO ) * m_bonenum );



	int getnum = 0;
	ret = GetJointInfo( m_bonenum, m_jointinfo, &getnum );
	if( ret || (getnum != m_bonenum) ){
		_ASSERT( 0 );
		return 1;
	}

	
	int jno;
	for( cno = 0; cno < SKEL_MAX; cno++ ){
		for( jno = 0; jno < m_bonenum; jno++ ){
			m_skel_wnd[cno].SendMessage( CB_ADDSTRING, 0, (LPARAM)( m_jointinfo + jno)->name );
		}
	}

	return 0;
}

LRESULT CConvSetDlg::OnRefKstIn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	OPENFILENAME ofn;
	char buf[MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"kst FILE (*.kst)\0*.kst\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( m_kstin, sizeof( char ) * MAX_PATH );
	strcpy_s( m_kstin, MAX_PATH, buf );
	m_kstin_wnd.SetWindowText( buf );


	int ret;
	CTxtSetting tmpts( m_papp );
	ret = tmpts.LoadParams( m_kstin );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	char tmpname[MAX_PATH * 2];

	int skno, jno;
	for( skno = 0; skno < SKEL_MAX; skno++ ){
		int findcombo = -1;

		for( jno = 0; jno < m_bonenum; jno++ ){
			ZeroMemory( tmpname, sizeof( char ) * MAX_PATH * 2 );
			m_skel_wnd[skno].SendMessage( CB_GETLBTEXT, jno, (LPARAM)tmpname );
			int cmp;
			cmp = strcmp( tmpts.m_elem[skno].jointname, tmpname );
			if( cmp == 0 ){
				findcombo = jno;
				break;
			}
		}

		if( findcombo >= 0 ){
			m_skel_wnd[skno].SendMessage( CB_SETCURSEL, (WPARAM)findcombo, 0 );
			if( tmpts.m_elem[skno].twistflag == 0 ){
				m_dlg_wnd.CheckDlgButton( s_chkid[skno], BST_CHECKED );
			}else{
				m_dlg_wnd.CheckDlgButton( s_chkid[skno], BST_UNCHECKED );
			}
		}else{
			m_skel_wnd[skno].SendMessage( CB_SETCURSEL, (WPARAM)0, 0 );
			if( s_deftwist[skno] == 0 ){
				m_dlg_wnd.CheckDlgButton( s_chkid[skno], BST_CHECKED );
			}else{
				m_dlg_wnd.CheckDlgButton( s_chkid[skno], BST_UNCHECKED );
			}
		}
	}

	return 0;
}

int CConvSetDlg::GetJointInfo( int arrayleng, JOINTINFO* jiptr, int* getnum )
{

	int seri;
	int setno = 0;

	for( seri = 0; seri < m_papp->m_thandler->s2e_leng; seri++ ){
		CTreeElem2* te = (*m_papp->m_thandler)( seri );
		_ASSERT( te );

		if( te->IsJoint() ){
			if( jiptr ){
				if( setno >= arrayleng ){
					_ASSERT( 0 );
					return 1;
				}
				strcpy_s( (jiptr + setno)->name, 256, te->name );
				(jiptr + setno)->serialno = te->serialno;
			}
			setno++;
		}
	}

	*getnum = setno;

	return 0;
}

int CConvSetDlg::ShowDlg( int cmdshow, CMyD3DApplication* papp, CTxtSetting* tsptr )
{
	m_cmdshow = cmdshow;
	if( m_cmdshow != SW_HIDE ){
		m_papp = papp;
		m_txtsetting = tsptr;
		if( !m_papp->m_shandler ){
			m_cmdshow = SW_HIDE;//!!!!!!!!!!!!!!!
		}
	}else{
		m_papp = 0;
		m_txtsetting = 0;
	}

	if( m_cmdshow != SW_HIDE ){
		SetCombo();
		ParamsToDlg();
	}

	ShowWindow( m_cmdshow );

	return 0;
}

