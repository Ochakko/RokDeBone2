// EditAlpDlg.cpp : CEditAlpDlg のインプリメンテーション
#include "stdafx.h"
#include "EditAlpDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdelem.h>
#include <part.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CEditAlpDlg

CEditAlpDlg::CEditAlpDlg( char* srcmatname, float srcalpha )
{

	strcpy_s( m_matname, 256, srcmatname );
	m_val = srcalpha;
}

CEditAlpDlg::~CEditAlpDlg()
{
}

LRESULT CEditAlpDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}


int CEditAlpDlg::DlgToParams()
{
	int ret;
	float tempval;
	ret = GetFloat( m_val_wnd, &tempval );
	if( ret || (tempval < 0.0f) || (tempval > 1.0f) ){
		MessageBox( "入力値が不正です（0.0から1.0）", "入力エラー", MB_OK );
		return 1;
	}
	m_val = tempval;

	return 0;
}

LRESULT CEditAlpDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	EndDialog(wID);
	return 0;
}

LRESULT CEditAlpDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CEditAlpDlg::SetWnd()
{

	m_matname_wnd = GetDlgItem( IDC_MATNAME );
	m_val_wnd = GetDlgItem( IDC_VAL );
	return 0;
}

int CEditAlpDlg::ParamsToDlg()
{
	m_matname_wnd.SetWindowText( m_matname );

	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", m_val );
	m_val_wnd.SetWindowText( tempchar );
	
	return 0;
}

