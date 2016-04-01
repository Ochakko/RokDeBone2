// BoneLineDlg.cpp : CBoneLineDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "BoneLineDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include <shdelem.h>

#include "ColDlg.h"

extern CColDlg g_coldlg;

/////////////////////////////////////////////////////////////////////////////
// CBoneLineDlg

CBoneLineDlg::CBoneLineDlg()
{
	InitParams();
}

CBoneLineDlg::~CBoneLineDlg()
{
}

int CBoneLineDlg::InitParams()
{
	m_selcol = RGB( 255, 255, 255 );
	m_unselcol = RGB( 255, 255, 255 );

	m_selem = 0;
	m_bonelinedisp = 1;
	m_checkchild = 0;

	return 0;
}

int CBoneLineDlg::SetSelemParam( CShdElem* srcselem )
{
	if( !srcselem ){
		_ASSERT( 0 );
		return 0;
	}

	m_selem = srcselem;

	m_bonelinedisp = m_selem->m_bonelinedisp;
	m_checkchild = 0;//!!!!!!!!!!

	return 0;
}



LRESULT CBoneLineDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CBoneLineDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_CHILD );
	if( checkflag == BST_CHECKED ){
		m_checkchild = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_checkchild = 0;
	}

	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_DODISP );
	if( checkflag == BST_CHECKED ){
		m_bonelinedisp = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_bonelinedisp = 0;
	}

////////


	if( m_selem ){
		if( m_checkchild ){
			m_selem->SetBoneLineDispReq( m_bonelinedisp, 0 );
		}else{
			m_selem->m_bonelinedisp = m_bonelinedisp;
		}
	}else{
		DbgOut( "BoneLineDlg : OnOK : selem NULL warning !!!\n" );
		_ASSERT( 0 );
	}

	EndDialog(wID);
	return 0;
}

LRESULT CBoneLineDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CBoneLineDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_selcol_wnd = GetDlgItem( IDC_SELCOL );
	m_unselcol_wnd = GetDlgItem( IDC_UNSELCOL );
	m_checkchild_wnd = GetDlgItem( IDC_CHECK_CHILD );

}

int CBoneLineDlg::ParamsToDlg()
{	

//check
	if( m_checkchild ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_CHILD, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_CHILD, BST_UNCHECKED );
	}

//radio	
	if( m_bonelinedisp == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NOTDISP, IDC_RADIO_DODISP, IDC_RADIO_NOTDISP );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NOTDISP, IDC_RADIO_DODISP, IDC_RADIO_DODISP );
	}


	return 0;
}

LRESULT CBoneLineDlg::OnRefSelCol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_hWnd, &m_selcol );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}
LRESULT CBoneLineDlg::OnRefUnSelCol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_hWnd, &m_unselcol );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}


LRESULT CBoneLineDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;


	int ret;
	
	ret = FillColorGDI( m_selcol, m_selcol_wnd );
	_ASSERT( !ret );

	ret = FillColorGDI( m_unselcol, m_unselcol_wnd );
	_ASSERT( !ret );
	
	return 0;
}

int CBoneLineDlg::FillColorGDI( COLORREF srccol, CWindow srcwnd )
{

	HBRUSH brush;
	brush = ::CreateSolidBrush( srccol );
	_ASSERT( brush != NULL );


	HDC hdc;
	hdc = srcwnd.GetDC();
	RECT clirect;
	srcwnd.GetClientRect( &clirect );
	
	
	HBRUSH hOldBrush = (HBRUSH)SelectObject( hdc, brush );
	BOOL bret;
	bret = ::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
	::SelectObject( hdc, hOldBrush );
	srcwnd.ReleaseDC( hdc );

	return 0;

}

