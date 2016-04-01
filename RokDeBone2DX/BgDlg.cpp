// BgDlg.cpp : CBgDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "BgDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "ColDlg.h"
#include "GetDlgParams.h"

extern CColDlg g_coldlg;

/////////////////////////////////////////////////////////////////////////////
// CBgDlg

CBgDlg::CBgDlg( char* srcname1, char* srcname2, float srcmvu, float srcmvv, COLORREF srccolor, int srcfixsize )
{
	ZeroMemory( name1, _MAX_PATH );
	ZeroMemory( name2, _MAX_PATH );

	int leng1;
	leng1 = (int)strlen( srcname1 );
	if( leng1 > 0 ){
		strcpy_s( name1, _MAX_PATH, srcname1 );
	}
	int leng2;
	leng2 = (int)strlen( srcname2 );
	if( leng2 > 0 ){
		strcpy_s( name2, _MAX_PATH, srcname2 );
	}

	mvu = srcmvu;
	mvv = srcmvv;

	m_color = srccolor;

	m_fixsize = srcfixsize;
}

CBgDlg::~CBgDlg()
{
}

LRESULT CBgDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CBgDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	float tempfloat = 0.0f;
	ret = GetFloat( m_mvu_wnd, &tempfloat );
	if( ret || (tempfloat < -1.0f) || (tempfloat > 1.0f) ){
		MessageBox( "U方向の移動量が不正です。( float -1.0f ～1.0f )", "入力エラー", MB_OK );
		return 0;
	}
	mvu = tempfloat;

	ret = GetFloat( m_mvv_wnd, &tempfloat );
	if( ret || (tempfloat < -1.0f) || (tempfloat > 1.0f) ){
		MessageBox( "V方向の移動量が不正です。( float -1.0f ～1.0f )", "入力エラー", MB_OK );
		return 0;
	}
	mvv = tempfloat;


	char tempchar[_MAX_PATH];
	
	ret = GetName( m_name1_wnd, tempchar );
	if( ret ){
		MessageBox( "背景１枚目のファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( name1, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( name1, _MAX_PATH, tempchar );
	}


	ret = GetName( m_name2_wnd, tempchar );
	if( ret ){
		MessageBox( "背景２枚目のファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( name2, _MAX_PATH );
	if( tempchar[0] ){
		strcpy_s( name2, _MAX_PATH, tempchar );
	}

	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_FIXSIZE );
	if( ischecked == BST_CHECKED ){
		m_fixsize = 1;
	}else{
		m_fixsize = 0;
	}



	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CBgDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CBgDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_name1_wnd = GetDlgItem( IDC_NAME1 );
	m_name2_wnd = GetDlgItem( IDC_NAME2 );
	m_mvu_wnd = GetDlgItem( IDC_MVU );
	m_mvv_wnd = GetDlgItem( IDC_MVV );
	m_color_wnd = GetDlgItem( IDC_COLOR );
}

int CBgDlg::ParamsToDlg()
{	
	m_name1_wnd.SetWindowText( name1 );
	m_name2_wnd.SetWindowText( name2 );

	char tempchar[256];

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", mvu );
	m_mvu_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", mvv );
	m_mvv_wnd.SetWindowText( tempchar );

	if( m_fixsize ){
		m_dlg_wnd.CheckDlgButton( IDC_FIXSIZE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_FIXSIZE, BST_UNCHECKED );
	}


	return 0;
}

LRESULT CBgDlg::OnRef1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"画像ファイル\0*.bmp;*.dds;*.dib;*.hdr;*.jpg;*.pfm;*.png;*.ppm;*.tga\0All Files\0*.*\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( name1, _MAX_PATH );
	strcpy_s( name1, _MAX_PATH, buf );
	m_name1_wnd.SetWindowText( buf );

	return 0;
}
LRESULT CBgDlg::OnRef2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"画像ファイル\0*.bmp;*.dds;*.dib;*.hdr;*.jpg;*.pfm;*.png;*.ppm;*.tga\0All Files\0*.*\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 )
		return 0;

	ZeroMemory( name2, _MAX_PATH );
	strcpy_s( name2, _MAX_PATH, buf );
	m_name2_wnd.SetWindowText( buf );

	return 0;
}


LRESULT CBgDlg::OnColorRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_hWnd, &m_color );
	if( ret ){
		BOOL dummy;
		OnPaint( 0, 0, 0, dummy );
	}

	return 0;
}

LRESULT CBgDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	bHandled = FALSE;

	HBRUSH brush;
	brush = ::CreateSolidBrush( m_color );
	_ASSERT( brush != NULL );


	HDC hdc;
	hdc = m_color_wnd.GetDC();
	RECT clirect;
	m_color_wnd.GetClientRect( &clirect );
	
	
	HBRUSH hOldBrush = (HBRUSH)SelectObject( hdc, brush );
	BOOL bret;
	bret = ::Rectangle( hdc, clirect.left, clirect.top, clirect.right, clirect.bottom );
	_ASSERT( bret );
	
	::SelectObject( hdc, hOldBrush );
	m_color_wnd.ReleaseDC( hdc );
	
	return 0;
}
