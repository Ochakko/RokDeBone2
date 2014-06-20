// LdQuaDlg.cpp : CLdQuaDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "LdQuaDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CLdQuaDlg

CLdQuaDlg::CLdQuaDlg()
{
	multvec = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
}

CLdQuaDlg::~CLdQuaDlg()
{
}

LRESULT CLdQuaDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CLdQuaDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;
	float tempfloat = 0.0f;
	ret = GetFloat( m_multx_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	multvec.x = tempfloat;

	ret = GetFloat( m_multy_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	multvec.y = tempfloat;


	ret = GetFloat( m_multz_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "倍率の値が不正です。( float 0.0f以上 )", "入力エラー", MB_OK );
		return 0;
	}
	multvec.z = tempfloat;

	
	
/////

	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CLdQuaDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CLdQuaDlg::SetWnd()
{
	m_multx_wnd = GetDlgItem( IDC_MULTX );
	m_multy_wnd = GetDlgItem( IDC_MULTY );
	m_multz_wnd = GetDlgItem( IDC_MULTZ );

}

int CLdQuaDlg::ParamsToDlg()
{	

	char tempchar[256];

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", multvec.x );
	m_multx_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%f", multvec.y );
	m_multy_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%f", multvec.z );
	m_multz_wnd.SetWindowText( tempchar );


	return 0;
}
