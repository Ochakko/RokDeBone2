// LdEquDlg.cpp : CLdEquDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LdEquDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLdEquDlg

CLdEquDlg::CLdEquDlg()
{
	mult = 1.0f;
}

CLdEquDlg::~CLdEquDlg()
{
}

LRESULT CLdEquDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CLdEquDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	float tempfloat = 0.0f;
	ret = GetFloat( m_mult_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	mult = tempfloat;
/////
	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CLdEquDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLdEquDlg::SetWnd()
{
	m_mult_wnd = GetDlgItem( IDC_MULT );

}

int CLdEquDlg::ParamsToDlg()
{	
	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", mult );
	m_mult_wnd.SetWindowText( tempchar );

	return 0;
}

