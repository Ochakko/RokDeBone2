#include "stdafx.h"
#include "WriteXFileDlg.h"
#include "GetDlgParams.h"

#include <mothandler.h>

#define DBGH
#include <dbg.h>

#include <crtdbg.h>

//extern HINSTANCE g_resinstance;

/////////////////////////////////////////////////////////////////////////////
// CWriteXFileDlg

CWriteXFileDlg::CWriteXFileDlg( CMotHandler* srclpmh, char* srcfilename, MOTID* srcmid, int srcmotnum )
{
	InitParams();

	if( srcfilename && *srcfilename ){
		strcpy_s( xfilename, MAX_PATH, srcfilename );
	}

	motnum = srcmotnum;
	motidarray = srcmid;
	lpmh = srclpmh;
}

CWriteXFileDlg::~CWriteXFileDlg()
{
	DestroyObjs();
}
	
int CWriteXFileDlg::DestroyObjs()
{
	if( ppni ){
		int nno;
		for( nno = 0; nno < wnum; nno++ ){
			NAMEID* delni = *( ppni + nno );
			free( delni );
		}
		free( ppni );
		ppni = 0;
	}
	wnum = 0;

	return 0;
}



void CWriteXFileDlg::InitParams()
{
	ZeroMemory( xfilename, sizeof( char ) * MAX_PATH );
	mult = 1.0f;
	convface = 1;

	lpmh = 0;

	motnum = 0;
	motidarray = 0;

	wnum = 0;
	ppni = 0;

	m_fullbone = 1;
}

LRESULT CWriteXFileDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	InitCommonControls();

//	InitParams();

	m_dlg_wnd = m_hWnd;

	m_mult_wnd = GetDlgItem( IDC_MULT );
	m_xname_wnd = GetDlgItem( IDC_XNAME );

	m_combo_wnd = GetDlgItem( IDC_COMBO1 );
	m_list_wnd = GetDlgItem( IDC_LIST1 );


	ret = InitCombo();
	_ASSERT( !ret );

	ret = ParamsToDlg();
	_ASSERT( !ret );


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteXFileDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = GetName( m_xname_wnd, xfilename );
	if( ret ){
		MessageBox( "X File名が不正です。", "エラー", MB_OK );
		return 0;
	}

//////////
	float tempfloat;
	ret = GetFloat( m_mult_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		MessageBox( "倍率の値が不正です。正の小数", "エラー", MB_OK );
		return 0;
	}
	mult = tempfloat;


///////////
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_FULLBONE );
	if( checkflag == BST_CHECKED ){
		m_fullbone = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_fullbone = 0;
	}else{
		_ASSERT( 0 );
		m_fullbone = 0;
	}


	EndDialog(wID);
	return 0;
}

LRESULT CWriteXFileDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CWriteXFileDlg::ParamsToDlg()
{
	char tempmes[512];
	sprintf_s( tempmes, 512, "%f", mult );
	m_mult_wnd.SetWindowText( tempmes );

	m_xname_wnd.SetWindowTextA( xfilename );

	if( m_fullbone == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_FULLBONE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_FULLBONE, BST_UNCHECKED );
	}


	int ret;
	ret = InitList();
	_ASSERT( !ret );

	return 0;
}


LRESULT CWriteXFileDlg::OnRefX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.x" );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"X FILE (*.x)\0*.x\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="x";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	//ZeroMemory( name, _MAX_PATH );
	//strcpy( name, buf );
	m_xname_wnd.SetWindowText( buf );

	return 0;
}

int CWriteXFileDlg::InitCombo()
{
	int ret;

	m_combo_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );

	int setno = 0;
	char tempchar[256];
	strcpy_s( tempchar, 256, "全て" );
	m_combo_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	m_combo_wnd.SendMessage( CB_SETITEMDATA, setno, (LPARAM)(MAXMOTIONNUM + 1) );
	setno++;

	int mno;
	for( mno = 0; mno < motnum; mno++ ){
		MOTID* curmotid = motidarray + mno;
		int motid = curmotid->id;
		ZeroMemory( tempchar, sizeof( char ) * 256 );
		ret = lpmh->GetMotionName( motid, tempchar );
		if( ret ){
			DbgOut( "writexfiledlg : InitCombo : GetMotionName error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		m_combo_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
		m_combo_wnd.SendMessage( CB_SETITEMDATA, setno, (LPARAM)motid );
		setno++;
	}

	m_combo_wnd.SendMessage( CB_SETCURSEL, 0, 0 );


	return 0;
}

int CWriteXFileDlg::InitList()
{
	m_list_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );

	int wno;
	for( wno = 0; wno < wnum; wno++ ){
		NAMEID* curni = *( ppni + wno );
		m_list_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)curni->name );
		m_list_wnd.SendMessage( LB_SETITEMDATA, wno, curni->id );
	}
	
	return 0;
}

LRESULT CWriteXFileDlg::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	if( motnum <= 0 ){
		return 0;
	}

	int combono;
	combono = (int)m_combo_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	


	if( combono == 0 ){
		
		DestroyObjs();//!!!!!!!!!!!!!!!

		ppni = (NAMEID**)malloc( sizeof( NAMEID* ) * motnum );
		if( !ppni ){
			DbgOut( "writexfiledlg : InitList : ppni alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( ppni, sizeof( NAMEID* ) * motnum );

		int mno;
		for( mno = 0; mno < motnum; mno++ ){
			MOTID* curmotid = motidarray + mno;
			int motid = curmotid->id;

			NAMEID* newni;
			newni = (NAMEID*)malloc( sizeof( NAMEID ) );
			if( !newni ){
				DbgOut( "writexfiledlg : InitList : newni alloc error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			ZeroMemory( newni, sizeof( NAMEID ) );

			ret = lpmh->GetMotionName( motid, newni->name );
			if( ret ){
				DbgOut( "writexfiledlg : InitList : mh GetMotionName error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			newni->id = motid;

			*( ppni + mno ) = newni;

		}
		wnum = motnum;

	}else{
		int newwnum = wnum + 1;
		int itemdata;
		itemdata = (int)m_combo_wnd.SendMessage( CB_GETITEMDATA, combono, 0 );
		if( itemdata == CB_ERR ){
			DbgOut( "writexfiledlg : InitList : itemdata error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ppni = (NAMEID**)realloc( ppni, sizeof( NAMEID* ) * newwnum );
		if( !ppni ){
			DbgOut( "writexfiledlg : InitList : ppni alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		NAMEID* newni;
		newni = (NAMEID*)malloc( sizeof( NAMEID ) );
		if( !newni ){
			DbgOut( "writexfiledlg : InitList : newni alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( newni, sizeof( NAMEID ) );

		newni->id = itemdata;
		ret = lpmh->GetMotionName( itemdata, newni->name );
		if( ret ){
			DbgOut( "writexfiledlg : InitList : mh GetMotionName error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		*( ppni + newwnum - 1 ) = newni;
		wnum = newwnum;

//_ASSERT( 0 );

	}



	InitList();

	return 0;
}
LRESULT CWriteXFileDlg::OnDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( motnum <= 0 ){
		return 0;
	}

	if( wnum <= 0 ){
		return 0;
	}

	int listno;
	listno = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= wnum) ){
		DbgOut( "writexfiledlg : OnDel listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int newwnum = wnum - 1;
	NAMEID** newppni;
	newppni = (NAMEID**)malloc( sizeof( NAMEID* ) * newwnum );
	ZeroMemory( newppni, sizeof( NAMEID* ) * newwnum );

	int wno;
	int setno = 0;
	for( wno = 0; wno < wnum; wno++ ){
		if( wno != listno ){
			*( newppni + setno ) = *( ppni + wno );
			setno++;
		}
	}

	free( *( ppni + listno ) );
	free( ppni );
	ppni = newppni;
	wnum = newwnum;

	InitList();

	return 0;
}
LRESULT CWriteXFileDlg::OnUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( motnum <= 0 ){
		return 0;
	}

	if( wnum <= 1 ){
		return 0;
	}

	int listno;
	listno = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= wnum) ){
		DbgOut( "writexfiledlg : OnUp listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( listno == 0 ){
		return 0;
	}


	NAMEID** newppni;
	newppni = (NAMEID**)malloc( sizeof( NAMEID* ) * wnum );
	ZeroMemory( newppni, sizeof( NAMEID* ) * wnum );

	int setno = 0;
	int wno;
	for( wno = 0; wno <= (listno - 2); wno++ ){
		*( newppni + setno ) = *( ppni + wno );
		setno++;
	}

	*( newppni + listno - 1 ) = *( ppni + listno );
	setno++;

	*( newppni + listno ) = *( ppni + listno - 1 );
	setno++;


	for( wno = (listno + 1); wno < wnum; wno++ ){
		*( newppni + setno ) = *( ppni + wno );
		setno++;
	}

	if( setno != wnum ){
		DbgOut( "writexfiledlg : OnUp : setno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	free( ppni );
	ppni = newppni;

	InitList();


	return 0;
}
LRESULT CWriteXFileDlg::OnDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( motnum <= 0 ){
		return 0;
	}

	if( wnum <= 1 ){
		return 0;
	}

	int listno;
	listno = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= wnum) ){
		DbgOut( "writexfiledlg : OnDown listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( listno == (wnum - 1) ){
		return 0;
	}


	NAMEID** newppni;
	newppni = (NAMEID**)malloc( sizeof( NAMEID* ) * wnum );
	ZeroMemory( newppni, sizeof( NAMEID* ) * wnum );

	int setno = 0;
	int wno;
	for( wno = 0; wno <= (listno - 1); wno++ ){
		*( newppni + setno ) = *( ppni + wno );
		setno++;
	}

	*( newppni + listno ) = *( ppni + listno + 1 );
	setno++;

	*( newppni + listno + 1 ) = *( ppni + listno );
	setno++;

	for( wno = (listno + 2); wno < wnum; wno++ ){
		*( newppni + setno ) = *( ppni + wno );
		setno++;
	}

	if( setno != wnum ){
		DbgOut( "writexfiledlg : OnDown : setno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	free( ppni );
	ppni = newppni;

	InitList();

	return 0;
}


