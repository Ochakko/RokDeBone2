// EditInfDlg.cpp : CEditInfDlg のインプリメンテーション
#include "stdafx.h"
#include "EditInfDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdelem.h>
#include <part.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CEditInfDlg

CEditInfDlg::CEditInfDlg( char* srcbonename, int kind, char* srctext )
{

	strcpy_s( m_bonename, MAX_PATH, srcbonename );
	m_kind = kind;
	strcpy_s( m_textval, MAX_PATH, srctext );

	m_val = (float)atof( m_textval );
}

CEditInfDlg::~CEditInfDlg()
{
}

LRESULT CEditInfDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}


int CEditInfDlg::DlgToParams()
{
	int ret;
	float tempval;
	ret = GetFloat( m_val_wnd, &tempval );
	if( ret || (tempval < 0.0f) ){
		MessageBox( "入力値が不正です（正の小数を入れてください）", "入力エラー", MB_OK );
		return 1;
	}

	m_val = tempval;
	sprintf_s( m_textval, MAX_PATH, "%f", m_val );

	return 0;
}

LRESULT CEditInfDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	EndDialog(wID);
	return 0;
}

LRESULT CEditInfDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CEditInfDlg::SetWnd()
{

	m_bonename_wnd = GetDlgItem( IDC_BONENAME );
	m_kind_wnd = GetDlgItem( IDC_KIND );
	m_val_wnd = GetDlgItem( IDC_VAL );
	return 0;
}

int CEditInfDlg::ParamsToDlg()
{
	m_bonename_wnd.SetWindowText( m_bonename );

	switch( m_kind ){
	case SUBID_RATE:
		m_kind_wnd.SetWindowText( "比重％" );
		break;
	case SUBID_ORGINF:
		m_kind_wnd.SetWindowText( "正規化前の影響度" );
		break;
	default:
		_ASSERT( 0 );
		m_kind_wnd.SetWindowText( "Unknown !!!" );
		return 1;
		break;
	}

	m_val_wnd.SetWindowText( m_textval );
	
	return 0;
}

