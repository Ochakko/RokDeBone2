// MabikiDlg.cpp : CMabikiDlg のインプリメンテーション
#include "stdafx.h"

#include "MabikiDlg.h"
#include <Commdlg.h>
#define DBGH
#include <dbg.h>

#include "GetDlgParams.h"


static int s_mstep[9] = 
{
	2,3,4,5,6,
	7,8,9,10
};
static char s_strmstep[9][5] =
{
	"2","3","4","5","6",
	"7","8","9","10"
};

/////////////////////////////////////////////////////////////////////////////
// CMabikiDlg

CMabikiDlg::CMabikiDlg()
{
	m_step = s_mstep[0];
}

CMabikiDlg::~CMabikiDlg()
{

}

LRESULT CMabikiDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

//DbgOut( "MabikiDlg : OnInitDialog\r\n" );

	SetWnd();

	int sno;
	for( sno = 0; sno < 9; sno++ ){
		m_step_wnd.SendMessageA( CB_ADDSTRING, 0, (LPARAM)&s_strmstep[sno][0] );
	}
	m_step_wnd.SendMessage( CB_SETCURSEL, 0, 0 );

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CMabikiDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_step_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	
	m_step = s_mstep[combono];

	EndDialog(wID);
	return 0;
}

LRESULT CMabikiDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CMabikiDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;
	m_step_wnd = GetDlgItem( IDC_STEP );
}
