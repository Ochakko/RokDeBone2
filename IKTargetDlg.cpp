// IKTargetDlg.cpp : CIKTargetDlg のインプリメンテーション
#include "stdafx.h"
#include "IKTargetDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdelem.h>
#include <part.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CIKTargetDlg

CIKTargetDlg::CIKTargetDlg( CTreeElem2* srcte, CShdElem* srcse, D3DXVECTOR3 srccurpos )
{
	m_te = srcte;
	m_se = srcse;

	m_iktarget = srcse->m_iktarget;
	m_iktargetlevel = srcse->m_iktargetlevel;
	m_iktargetcnt = srcse->m_iktargetcnt;
	m_iktargetpos = srcse->m_iktargetpos;

	m_curpos = srccurpos;
}

CIKTargetDlg::~CIKTargetDlg()
{
}

LRESULT CIKTargetDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	//combo
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"1" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"2" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"3" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"4" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"5" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"6" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"7" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"8" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"9" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"10" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"11" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"12" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"13" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"14" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"15" );
	m_combo_iklevel_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"255" );


	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}


LRESULT CIKTargetDlg::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	
	m_iktargetpos = m_curpos;

	//int ret;
	if( m_iktarget == 0 ){
		//ret = (int)MessageBox( "ＩＫターゲットが無効になっています。\n有効にしますか?", "確認", MB_OKCANCEL );
		//if( ret == IDOK ){
			m_iktarget = 1;//!!!!!!!!!!!!!
		//}
	}

	ParamsToDlg();

	return 0;
}

LRESULT CIKTargetDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_IKT );
	if( checkflag == BST_CHECKED ){
		m_iktarget = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_iktarget = 0;
	}
	
	UINT checkflag2;
	checkflag2 = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_INVALID );
	if( (checkflag == BST_CHECKED) && (checkflag2 == BST_CHECKED) ){
		m_iktarget = 2;
	}
	

/////////
	int cursel;
	cursel = (int)m_combo_iklevel_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( cursel == CB_ERR ){
		_ASSERT( 0 );	
		return 0;
	}

	int tempint;
	tempint = cursel + 1;
	if( (tempint >= 1) && (tempint <= 15) ){
		m_iktargetlevel = tempint;
	}else{
		m_iktargetlevel = 255;
	}

/////////
	ret = GetInt( m_edit_ikcnt_wnd, &tempint );
	if( ret ){
		MessageBox( "計算回数の値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_iktargetcnt = tempint;

/////////
	float tempx, tempy, tempz;
	ret = GetFloat( m_editx_wnd, &tempx );
	if( ret ){
		MessageBox( "ターゲット位置の、X座標の値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_edity_wnd, &tempy );
	if( ret ){
		MessageBox( "ターゲット位置の、Y座標の値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_editz_wnd, &tempz );
	if( ret ){
		MessageBox( "ターゲット位置の、Z座標の値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_iktargetpos.x = tempx;
	m_iktargetpos.y = tempy;
	m_iktargetpos.z = tempz;

////////////
////////////

	/***
	m_se->m_iktarget = m_iktarget;
	m_se->m_iktargetlevel = m_iktargetlevel;
	m_se->m_iktargetcnt = m_iktargetcnt;
	m_se->m_iktargetpos = m_iktargetpos;
	***/

	EndDialog(wID);
	return 0;
}

LRESULT CIKTargetDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	EndDialog(wID);
	return 0;
}

int CIKTargetDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_text_name_wnd = GetDlgItem( IDC_TEXT_JOINTNAME );
	m_text_curpos_wnd = GetDlgItem( IDC_TEXT_JOINTLOC );
	m_edit_ikcnt_wnd = GetDlgItem( IDC_EDIT_IKCNT );
	m_editx_wnd = GetDlgItem( IDC_EDIT_X );
	m_edity_wnd = GetDlgItem( IDC_EDIT_Y );
	m_editz_wnd = GetDlgItem( IDC_EDIT_Z );
	m_combo_iklevel_wnd = GetDlgItem( IDC_COMBO_IKLEVEL );

	return 0;
}

int CIKTargetDlg::ParamsToDlg()
{
	char tempchar[2048];

	if( m_iktarget ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_IKT, BST_CHECKED );
		if( m_iktarget == 2 ){
			m_dlg_wnd.CheckDlgButton( IDC_CHECK_INVALID, BST_CHECKED );
		}
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_IKT, BST_UNCHECKED );
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_INVALID, BST_UNCHECKED );
	}

	if( m_iktargetlevel <= 15 )
		m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, (m_iktargetlevel - 1), 0 );
	else
		m_combo_iklevel_wnd.SendMessage( CB_SETCURSEL, 15, 0 );
	

	sprintf_s( tempchar, 2048, "%d", m_iktargetcnt );
	m_edit_ikcnt_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 2048, "現在のジョイント位置　：　%s", m_te->name );
	m_text_name_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 2048, "X : %f, Y : %f, Z : %f", m_curpos.x, m_curpos.y, m_curpos.z );
	m_text_curpos_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 2048, "%f", m_iktargetpos.x );
	m_editx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 2048, "%f", m_iktargetpos.y );
	m_edity_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 2048, "%f", m_iktargetpos.z );
	m_editz_wnd.SetWindowText( tempchar );
	
	return 0;
}





