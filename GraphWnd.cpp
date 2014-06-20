// GraphWnd.cpp : CGraphWnd のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "GraphWnd.h"

#include <Commdlg.h>
#include <commctrl.h>

#include <coef.h>
#define DBGH
#include <dbg.h>

#include "GraphRotDlg.h"
#include "MotDlg.h"

//#include "MotParamDlg.h"

extern CMotDlg* g_motdlg;

/////////////////////////////////////////////////////////////////////////////
// CGraphWnd

CGraphWnd::CGraphWnd( CMyD3DApplication* srcapp )
{
	InitCommonControls();

	m_papp = srcapp;

	m_grotdlg = 0;

	m_disptabno = 0;

	m_createtabflag = 0;

	m_cmdshow = SW_HIDE;
}

CGraphWnd::~CGraphWnd()
{
	DestroyTabWindows();
}


LRESULT CGraphWnd::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE );
	BOOL handled;
	if( m_grotdlg ){
		m_grotdlg->OnCancel( wNotifyCode, wID, hWndCtl, handled );
	}
	return 0;
}


int CGraphWnd::ShowDlg( int cmdshow )
{
	if( cmdshow == SW_HIDE ){
		if( m_grotdlg ){
			m_grotdlg->ShowDlg( SW_HIDE );
		}
		ShowWindow( SW_HIDE );
		m_cmdshow = SW_HIDE;
	}else{
		ShowWindow( SW_SHOW );

		if( m_grotdlg ){
			if( m_disptabno == 0 ){
				m_grotdlg->ShowDlg( SW_SHOW );
			}else{
				_ASSERT( 0 );
			}
		}
		m_cmdshow = SW_SHOW;
	}

	return 0;
}

LRESULT CGraphWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_createtabflag == 0 ){
		CreateTabWindows();
		SetWnd();
		m_createtabflag = 1;
	}

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}


void CGraphWnd::SetWnd()
{
    RECT rc;
    TC_ITEM tcItem;
    int x, y;

    m_tab_wnd.GetClientRect(&rc);

	x = 0;
	y = 0;

    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = "グラフ";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)0, (LPARAM)&tcItem);
//    tcItem.mask = TCIF_TEXT;
//    tcItem.pszText = "ＩＫ軸";
//	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)1, (LPARAM)&tcItem);

//	rc.left = 50;
//	rc.top = 50;
	rc.left = 0;
	rc.top = 25;



//	m_grotdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
//	m_grotdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
//    m_grotdlg->Create( m_tab_wnd, rc );

	RECT winrect;
	m_tab_wnd.GetWindowRect( &winrect );
	m_grotdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
	m_grotdlg->SetWindowPos( HWND_TOP, &winrect, SWP_NOSIZE | SWP_SHOWWINDOW );


	m_grotdlg->ShowDlg(SW_SHOW);

}

int CGraphWnd::ParamsToDlg()
{	

	return 0;
}

int CGraphWnd::CreateTabWindows()
{
	if( m_grotdlg ){
		return 0;
	}

	RECT rect0;
	GetClientRect( &rect0 );
	m_tab_wnd.Create( "SysTabControl32", m_hWnd, rect0, NULL, WS_CHILD | WS_VISIBLE );


	m_grotdlg = new CGraphRotDlg( m_papp, g_motdlg->m_motparamdlg );
	if( !m_grotdlg ){
		DbgOut( "GraphWnd : grotdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CGraphWnd::DestroyTabWindows()
{
	if( m_grotdlg ){
		m_grotdlg->CloseSpDlg();
		m_grotdlg->DestroySpDlg();

		m_grotdlg->DestroyTimer();
		m_grotdlg->DestroyWindow();//!!!!!!!!!
		delete m_grotdlg;
		m_grotdlg = 0;
	}
	return 0;
}

LRESULT CGraphWnd::OnSelchange_tab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int cursel = (int)::SendMessage( m_tab_wnd, TCM_GETCURSEL, 0, 0 );

	m_grotdlg->ShowDlg( SW_HIDE );

	switch(cursel) {
		case 0:
			m_grotdlg->ShowDlg(SW_SHOW);
			m_disptabno = 0;
			break;
//		case 1:
//			m_axisdlg->ShowDlg(SW_SHOW);
//			m_disptabno = 1;
//			break;
		default:
			_ASSERT( 0 );
			break;
	}

	//char selectstr[256];
	//sprintf( selectstr, "selchange : %d", cursel );
	//MessageBox( selectstr, "selchange", MB_OK );

	bHandled = 0;

	return 0;
}

LRESULT CGraphWnd::OnSizeDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rect0;
	GetClientRect( &rect0 );

	m_tab_wnd.MoveWindow( rect0.left, rect0.top, 
		rect0.right - rect0.left,
		rect0.bottom - rect0.top,
		1 );

	RECT rc;
	m_tab_wnd.GetWindowRect( &rc );

//	_ASSERT( 0 );

//	rc.left = 50;
//	rc.top = 50;
//	rc.left = 0;
//	rc.top = 25;

	rc.top += 25;

//	m_grotdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
	m_grotdlg->SetWindowPos( HWND_TOP, &rc, SWP_SHOWWINDOW );


	return 0;
}

int CGraphWnd::Redraw()
{
	return m_grotdlg->Redraw();
}
int CGraphWnd::OnUserSelchange()
{
	m_grotdlg->OnUserSelchange();
	return 0;
}

int CGraphWnd::ShiftRange( int srcdiff )
{
	if( m_grotdlg ){
		m_grotdlg->ShiftRange( srcdiff );
	}
	return 0;
}
int CGraphWnd::CloseSpDlg()
{
	if( m_grotdlg ){
		m_grotdlg->CloseSpDlg();
	}
	return 0;
}