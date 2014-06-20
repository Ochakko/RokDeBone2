// InfoDlg.cpp : CInfoDlg のインプリメンテーション
#include "stdafx.h"

#include "InfoDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg

CInfoDlg::CInfoDlg()
{
}

CInfoDlg::~CInfoDlg()
{
}

LRESULT CInfoDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;  // システムにフォーカスを設定させます
}


LRESULT CInfoDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CInfoDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

