// SetCol2Dlg.cpp : CSetCol2Dlg のインプリメンテーション
#include "stdafx.h"
#include "SetCol2Dlg.h"

#include <math.h>
#include <coef.h>

#define DBGH
#include <dbg.h>


#include <crtdbg.h>
#include "GetDlgParams.h"

#define RENDERCOL2TIMER	11002

enum {
	MOPE_W2P,
	MOPE_P2W,
	MOPE_DEL,
	MOPE_NAME,
	MOPE_MAX
};

/////////////////////////////////////////////////////////////////////////////
// CSetCol2Dlg

CSetCol2Dlg::CSetCol2Dlg( MATERIAL0* wmatptr, MPALETTE* palptr, int palno )
{
	m_timerid = 0;
	m_ope = MOPE_W2P;

	m_wmptr = wmatptr;
	m_mpptr = palptr;
	m_palno = palno;

	m_diffuse_dc = NULL;
	m_specular_dc = NULL;
	m_ambient_dc = NULL;
	m_emissive_dc = NULL;

	m_diffuse_b = NULL;
	m_specular_b = NULL;
	m_ambient_b = NULL;
	m_emissive_b = NULL;

}

CSetCol2Dlg::~CSetCol2Dlg()
{
}

int CSetCol2Dlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_diffuse_wnd = GetDlgItem( IDC_DIFFUSE );
	m_specular_wnd = GetDlgItem( IDC_SPECULAR );
	m_ambient_wnd = GetDlgItem( IDC_AMBIENT );
	m_emissive_wnd = GetDlgItem( IDC_EMISSIVE );
	m_power_wnd = GetDlgItem( IDC_POWER );
	m_name_wnd = GetDlgItem( IDC_EDIT1 );

	m_diffuse_dc = ::GetDC( m_diffuse_wnd );
	m_specular_dc = ::GetDC( m_specular_wnd );
	m_ambient_dc = ::GetDC( m_ambient_wnd );
	m_emissive_dc = ::GetDC( m_emissive_wnd );

	return 0;
}


int CSetCol2Dlg::ParamsToDlg()
{
	switch( m_ope ){
	case MOPE_W2P:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_DEL, IDC_RADIO_NAME, IDC_RADIO_W2P );
		break;
	case MOPE_P2W:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_DEL, IDC_RADIO_NAME, IDC_RADIO_P2W );
		break;
	case MOPE_DEL:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_DEL, IDC_RADIO_NAME, IDC_RADIO_DEL );
		break;
	case MOPE_NAME:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_DEL, IDC_RADIO_NAME, IDC_RADIO_NAME );
		break;
	default:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_DEL, IDC_RADIO_NAME, IDC_RADIO_W2P );
		m_ope = MOPE_W2P;
		break;
	}


	char tempchar[256];
	sprintf_s( tempchar, 256, "%f", m_mpptr->mat.power );
	m_power_wnd.SetWindowText( tempchar );

	m_name_wnd.SetWindowText( m_mpptr->name );


	return 0;
}

LRESULT CSetCol2Dlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//DbgOut( "SetCol2Dlg : OnInitDialog \r\n" );

	int ret = 0;
	ret = SetWnd();
	_ASSERT( !ret );

	m_diffuse_b = ::CreateSolidBrush( m_mpptr->mat.diffuse );
	m_specular_b = ::CreateSolidBrush(m_mpptr->mat.specular );
	m_ambient_b = ::CreateSolidBrush( m_mpptr->mat.ambient );
	m_emissive_b = ::CreateSolidBrush( m_mpptr->mat.emissive );


	ret = ParamsToDlg();
	_ASSERT( !ret );

	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERCOL2TIMER, 100 );
		DbgOut( "SetCol2Dlg :CreateSwapChain : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERCOL2TIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSetCol2Dlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_W2P );
	if( checkflag == BST_CHECKED ){
		m_ope = MOPE_W2P;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_P2W );
	if( checkflag == BST_CHECKED ){
		m_ope = MOPE_P2W;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_DEL );
	if( checkflag == BST_CHECKED ){
		m_ope = MOPE_DEL;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_NAME );
	if( checkflag == BST_CHECKED ){
		m_ope = MOPE_NAME;
	}

	char tempchar[64];
	int ret;

	switch( m_ope ){
	case MOPE_W2P:
		ret = GetText( m_name_wnd, tempchar, 64 );
		if( ret ){
			return 0;
		}

		strcpy_s( m_mpptr->name, 64, tempchar );
		m_mpptr->mat = *m_wmptr;

		break;
	case MOPE_P2W:
		
		*m_wmptr = m_mpptr->mat;

		break;
	case MOPE_DEL:

		sprintf_s( tempchar, 64, "palette%d", m_palno );
		strcpy_s( m_mpptr->name, 64, tempchar );

		m_mpptr->mat.diffuse = RGB( 255, 255, 255 );
		m_mpptr->mat.specular = RGB( 0, 0, 0 );
		m_mpptr->mat.ambient = RGB( 64, 64, 64 );
		m_mpptr->mat.emissive = RGB( 0, 0, 0 );
		m_mpptr->mat.power = 5.0f;

		break;
	case MOPE_NAME:
		ret = GetText( m_name_wnd, tempchar, 64 );
		if( ret ){
			return 0;
		}

		strcpy_s( m_mpptr->name, 64, tempchar );
		break;
	default:
		break;
	}


	EndDialog(wID);
	

	return 0;
}

LRESULT CSetCol2Dlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	EndDialog(wID);

	return 0;
}


LRESULT CSetCol2Dlg::OnPaintOrg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	PaintRect( m_diffuse_wnd, m_diffuse_dc, m_diffuse_b, m_mpptr->mat.diffuse );
	PaintRect( m_specular_wnd, m_specular_dc, m_specular_b, m_mpptr->mat.specular );
	PaintRect( m_ambient_wnd, m_ambient_dc, m_ambient_b, m_mpptr->mat.ambient );
	PaintRect( m_emissive_wnd, m_emissive_dc, m_emissive_b, m_mpptr->mat.emissive );

	return 0;
}

int CSetCol2Dlg::PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush, COLORREF srccol )
{
	if( !hdc )
		return 0;

	//HBRUSH brush;
	//brush = ::CreateSolidBrush( srccol );
	//_ASSERT( brush != NULL );
	if( !brush )
		return 0;

	BOOL bret;
	RECT winrect;
	bret = srcwnd.GetWindowRect( &winrect );
	if( !bret )
		return 0;

	RECT clirect;
	//clirect.top = 0;
	//clirect.left = 0;
	//clirect.bottom = winrect.bottom - winrect.top;
	//clirect.right = winrect.right - winrect.left;
	bret = srcwnd.GetClientRect( &clirect );
	if( !bret )
		return 0;

	HBRUSH hOldBrush = (HBRUSH)SelectObject( hdc, brush );
	bret = ::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	if( !bret )
		return 0;

	//::SelectObject( hdc, hOldBrush );

	return 0;
}

/***
LRESULT CSetCol2Dlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	int timerid = (int)wParam;
	if( timerid == m_timerid ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );

		ret = Render();
		_ASSERT( !ret );

	}

	return 0;
}
***/

LRESULT CSetCol2Dlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//int ret;

	int timerid = (int)wParam;
	if( timerid == m_timerid ){
		BOOL dummy;
		OnPaintOrg( 0, 0, 0, dummy );
	}

	return 0;
}
LRESULT CSetCol2Dlg::OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	if( m_timerid != 0 ){
		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;
		//Sleep( 100 );//!!!!!!!!!!!!!!!!!!!
	}

	if( m_diffuse_dc ){
		::ReleaseDC( m_diffuse_wnd, m_diffuse_dc );
		m_diffuse_dc = NULL;
	}
	if( m_specular_dc ){
		::ReleaseDC( m_specular_wnd, m_specular_dc );
		m_specular_dc = NULL;
	}
	if( m_ambient_dc ){
		::ReleaseDC( m_ambient_wnd, m_ambient_dc );
		m_ambient_dc = NULL;
	}
	if( m_emissive_dc ){
		::ReleaseDC( m_emissive_wnd, m_emissive_dc );
		m_emissive_dc = NULL;
	}

	return 0;
}
