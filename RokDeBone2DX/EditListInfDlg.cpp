// EditListInfDlg.cpp : CEditListInfDlg のインプリメンテーション
#include "stdafx.h"
#include "EditListInfDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <part.h>

#include "viewer.h"

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CEditListInfDlg

CEditListInfDlg::CEditListInfDlg( CMyD3DApplication* papp, int seldisp, int vertno, int selbone, INFELEM* ieptr, int infnum )
{
	m_papp = papp;
	m_seldisp = seldisp;
	m_selvert = vertno;
	m_selbone = selbone;
	m_ieptr = ieptr;
	m_infnum = infnum;
}

CEditListInfDlg::~CEditListInfDlg()
{
}

LRESULT CEditListInfDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}


int CEditListInfDlg::DlgToParams()
{
	int ret;
	float tempval;
	ret = GetFloat( m_infval_wnd, &tempval );
	if( ret || (tempval < 0.0f) ){
		MessageBox( "入力値が不正です（正の小数を入れてください）", "入力エラー", MB_OK );
		return 1;
	}

	m_val = tempval;

	return 0;
}

LRESULT CEditListInfDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	EndDialog(wID);
	return 0;
}

LRESULT CEditListInfDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CEditListInfDlg::SetWnd()
{
	m_dispobjname_wnd = GetDlgItem( IDC_DISPOBJNAME );
	m_vertno_wnd = GetDlgItem( IDC_VERTNO );
	m_bonename_wnd = GetDlgItem( IDC_BONENAME );
	m_infval_wnd = GetDlgItem( IDC_INFVAL );

	return 0;
}

int CEditListInfDlg::ParamsToDlg()
{
	if( !m_papp->m_thandler || !m_papp->m_shandler )
		return 0;

	CTreeElem2* dispte;
	dispte = (*m_papp->m_thandler)( m_seldisp );
	_ASSERT( dispte );
	m_dispobjname_wnd.SetWindowTextA( dispte->name );

	char tempchar[512];
	sprintf_s( tempchar, 512, "頂点番号：%d", m_selvert );
	m_vertno_wnd.SetWindowText( tempchar );

	CTreeElem2* bonete;
	bonete = (*m_papp->m_thandler)( m_selbone );
	_ASSERT( bonete );
	sprintf_s( tempchar, 512, "ボーン名：%s", bonete->name );
	m_bonename_wnd.SetWindowText( tempchar );

	
	int findno = -1;
	int infno;
	for( infno = 0; infno < m_infnum; infno++ ){
		if( (m_ieptr + infno)->childno == m_selbone ){
			findno = infno;
			break;
		}
	}
	if( findno >= 0 ){
		float val;
		val = (m_ieptr + findno)->orginf * (m_ieptr + findno)->userrate;
		sprintf_s( tempchar, 512, "%f", val );
	}else{
		strcpy_s( tempchar, 512, "0.0" );
	}
	m_infval_wnd.SetWindowText( tempchar );
	
	return 0;
}

