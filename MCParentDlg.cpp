// MCParentDlg.cpp : CMCParentDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "MCParentDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#define DBGH
#include <dbg.h>

#include <crtdbg.h>
#include "GetDlgParams.h"



/////////////////////////////////////////////////////////////////////////////
// CMCParentDlg

CMCParentDlg::CMCParentDlg( char* srcmname, char* srcfname, int srctotalframe, int srcev0idle, int srccommonid, int srcforbidnum, int* srcforbidid, int srcnotfu )
{
	strcpy_s( name, 256, srcmname );
	strcpy_s( fname, 256, srcfname );
	totalframe = srctotalframe;

	ev0idle = srcev0idle;
	commonid = srccommonid;


	forbidnum = 0;
	ZeroMemory( forbidid, sizeof( int ) * MAXFORBIDNUM );


	int cpleng;
	cpleng = min( MAXFORBIDNUM, srcforbidnum );	
	if( cpleng > 0 ){
		forbidnum = cpleng;
		memcpy( forbidid, srcforbidid, sizeof( int ) * cpleng );
	}

	notfu = srcnotfu;
}

CMCParentDlg::~CMCParentDlg()
{
}

LRESULT CMCParentDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	int ret;
	ret = InitList1();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CMCParentDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;

	char tempchar[256];
	ret = GetText( m_name_wnd, tempchar, 256 );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "sigのファイル名が不正です。", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( name, 256 );
	if( tempchar[0] ){
		strcpy_s( name, 256, tempchar );
	}

	UINT ischecked;	
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_EV0IDLE );
	if( ischecked == BST_CHECKED ){
		ev0idle = 1;
	}else{
		ev0idle = 0;
	}

	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_NOTFU );
	if( ischecked == BST_CHECKED ){
		notfu = 1;
	}else{
		notfu = 0;
	}


	int tempint = 0;
	ret = GetInt( m_commonid_wnd, &tempint );
	if( ret || (tempint < 0) ){
		MessageBox( "共通分岐イベント番号が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	commonid = tempint;

	if( ret == 0 )
		EndDialog(wID);

	return 0;
}

LRESULT CMCParentDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CMCParentDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_name_wnd = GetDlgItem( IDC_MNAME );

	m_frame_wnd = GetDlgItem( IDC_TOTALFRAME );

	m_fname_wnd = GetDlgItem( IDC_FNAME );

	m_commonid_wnd = GetDlgItem( IDC_COMMONID );

	m_forbid_wnd = GetDlgItem( IDC_FORBIDID );

	m_list1_wnd = GetDlgItem( IDC_LIST1 );

	m_checknotfu_wnd = GetDlgItem( IDC_CHECK_NOTFU );

}

int CMCParentDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( name );

	char tempchar[256];

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%d", totalframe );
	m_frame_wnd.SetWindowText( tempchar );

	m_fname_wnd.SetWindowText( fname );

	if( ev0idle ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_EV0IDLE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_EV0IDLE, BST_UNCHECKED );
	}

	if( notfu ){
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_NOTFU, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHECK_NOTFU, BST_UNCHECKED );
	}

	sprintf_s( tempchar, 256, "%d", commonid );
	m_commonid_wnd.SetWindowText( tempchar );

	m_forbid_wnd.SetWindowText( "" );

	return 0;
}

LRESULT CMCParentDlg::OnAddForbid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int tempint;
	int ret;
	ret = GetInt( m_forbid_wnd, &tempint );
	if( ret || (tempint <= 0 ) ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!
	}

	int findflag = 0;
	int fno;
	for( fno = 0; fno < forbidnum; fno++ ){
		int curforbid = forbidid[ fno ];
		if( curforbid == tempint ){
			findflag = 1;
			break;
		}
	}
	if( findflag ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!
	}

	if( (forbidnum + 1) > MAXFORBIDNUM ){
		MessageBox( "これ以上追加できません。（個数制限）", "エラー", MB_OK );
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!
	}

	forbidid[ forbidnum ] = tempint;
	forbidnum++;

	ret = InitList1();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
LRESULT CMCParentDlg::OnDelForbid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int selindex;
	selindex = (int)m_list1_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( selindex == LB_ERR ){
		
		//何も選択していないときもLB_ERRが返る
		return 0;
	}
	if( (selindex < 0) || (selindex >= forbidnum) ){
		_ASSERT( 0 );
		return 1;
	}

	int tempf[ MAXFORBIDNUM ];
	ZeroMemory( tempf, sizeof( int ) * MAXFORBIDNUM );

	int setno = 0;
	int fno;
	for( fno = 0; fno < forbidnum; fno++ ){
		if( fno != selindex ){
			tempf[ setno ] = forbidid[ fno ];
			setno++;
		}
	}

	forbidnum = setno;

	ZeroMemory( forbidid, sizeof( int ) * MAXFORBIDNUM );
	memcpy( forbidid, tempf, sizeof( int ) * forbidnum );


	ret = InitList1();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}



	int nextselindex;
	if( selindex != 0 ){
		nextselindex = selindex - 1;
	}else{
		nextselindex = 0;
	}
	m_list1_wnd.SendMessage( LB_SETCURSEL, (WPARAM)nextselindex, 0 );

	return 0;
}

int CMCParentDlg::InitList1()
{
	char tempchar[1024];
	m_list1_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	

	int fno;
	for( fno = 0; fno < forbidnum; fno++ ){
		LRESULT lres;
		sprintf_s( tempchar, 1024, "%d", forbidid[ fno ] );
		lres = m_list1_wnd.SendMessageA( LB_ADDSTRING, 0, (LPARAM)tempchar );
		if( (lres == LB_ERR) || (lres == LB_ERRSPACE) ){
			_ASSERT( 0 );
			return 1;
		}

	}

	return 0;
}