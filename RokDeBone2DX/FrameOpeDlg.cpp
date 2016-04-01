// FrameOpeDlg.cpp : CFrameOpeDlg のインプリメンテーション
#include "stdafx.h"
#include "FrameOpeDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>
/////////////////////////////////////////////////////////////////////////////
// CFrameOpeDlg

CFrameOpeDlg::CFrameOpeDlg( int hascancel, char* textptr, int* list, CTreeHandler2* lpth )
{
	m_hascancel = hascancel;
	m_text = textptr;
	m_list = list;
	m_thandler = lpth;
}

CFrameOpeDlg::~CFrameOpeDlg()
{
}

LRESULT CFrameOpeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = SetWnd();
	_ASSERT( !ret );

	m_text_wnd.SetWindowText( m_text );

	if( m_hascancel == 0 ){
		m_cancel_wnd.EnableWindow( FALSE );
	}

	int treenum = m_thandler->s2e_leng;
	int treeno;
	for( treeno = 1; treeno < treenum; treeno++ ){
		if( *(m_list + treeno) == 1 ){
			CTreeElem2* curte = (*m_thandler)( treeno );
			char* name = curte->name;
			m_list_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)name );
		}
	}

	return 1;  // システムにフォーカスを設定させます
}

int CFrameOpeDlg::SetWnd()
{
	HWND temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT );
	m_text_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_LIST1 );
	m_list_wnd = temphwnd;

	temphwnd = GetDlgItem( 2 );
	m_cancel_wnd = temphwnd;

	return 0;
}

LRESULT CFrameOpeDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CFrameOpeDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

