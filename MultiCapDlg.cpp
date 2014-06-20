// MultiCapDlg.cpp : CMultiCapDlg のインプリメンテーション
#include "stdafx.h"
#include "MultiCapDlg.h"

#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CMultiCapDlg

CMultiCapDlg::CMultiCapDlg( int motleng )
{
	m_motleng = motleng;
	m_framestep = 20;

}

CMultiCapDlg::~CMultiCapDlg()
{
}

LRESULT CMultiCapDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}


int CMultiCapDlg::DlgToParams()
{
	int ret;
	int tempval;
	ret = GetInt( m_frames_wnd, &tempval );
	if( ret || (tempval <= 0) || (tempval >= m_motleng) ){
		MessageBox( "入力値が不正です（０より大きくモーション長より短い値）", "入力エラー", MB_OK );
		return 1;
	}

	m_framestep = tempval;

	return 0;
}

LRESULT CMultiCapDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	EndDialog(wID);
	return 0;
}

LRESULT CMultiCapDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CMultiCapDlg::SetWnd()
{
	m_frames_wnd = GetDlgItem( IDC_FRAMES );
	return 0;
}

int CMultiCapDlg::ParamsToDlg()
{
	char strval[256];
	sprintf_s( strval, 256, "%d", m_framestep );
	m_frames_wnd.SetWindowText( strval );

	return 0;
}

