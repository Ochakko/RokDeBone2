#include "stdafx.h"

#include "SigEditWnd.h"

#include "ShdParamDlg.h"
#include "JointLocDlg.h"
#include "NewJointDlg.h"
#include "InfluenceDlg.h"
#include "SelScopeDlg.h"

#include "viewer.h"

#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <D3DX9.h>

//#define NOTWINDOWSH
#include <coef.h>

#define DBGH
#include <dbg.h>
#include "inidata.h"
#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <mothandler.h>

#include <morph.h>
#include "motdlg.h"

#include <commctrl.h>

extern CIniData* g_inidata;
extern CMotDlg* g_motdlg;

#define CHKHEIGHT	25

CSigEditWnd::CSigEditWnd() : 
	m_ctlSysTreeView32(_T("SysTreeView32"), this, SIGEDIT_MSGMAP)
{
	m_papp = 0;
	m_apphwnd = 0;
	m_menu = 0;

	m_thandler = 0;
	m_shandler = 0;
	m_mhandler = 0;


	m_hImageList = NULL;
	m_PartImageNo = -1;
	m_PartSelectNo = -1;
	m_DispImageNo = -1;
	m_DispSelectNo = -1;
	m_BoneImageNo = -1;
	m_BoneSelectNo = -1;

	m_TI = 0;

	m_selecteditem = 0;
	m_selectedno = 0;

	m_clicked_lbutton = 0;

	m_dropitem = 0;
	m_dropno = 0;

	ZeroMemory( m_movelist, sizeof( HTREEITEM ) * 256 );
	m_movenum = 0;

	m_invalidate_handler = 0;
	m_onitembutton = 0;
	m_usercallflag = 0;
	m_forbidDD = 0;
}

CSigEditWnd::~CSigEditWnd()
{
	if( m_menu != NULL ){
		DestroyMenu( m_menu );
		m_menu = NULL;
	}

//	if( ::IsWindow( m_ctlSysTreeView32.m_hWnd ) ){
//		m_ctlSysTreeView32.DestroyWindow();
//	}

	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}

	if( m_hImageList ){
		ImageList_Destroy( m_hImageList );
		m_hImageList = 0;
	}

}


LRESULT CSigEditWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//	m_menu = LoadMenu( (HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE( IDR_DLGMENU ) );
//	if( m_menu == NULL ){
//		_ASSERT( 0 );
//		return 1;
//	}
//	BOOL bret;
//	bret = ::SetMenu( m_hWnd, m_menu );
//	if( bret == 0 ){
//		_ASSERT( 0 );
//		return 1;
//	}

	InitCommonControls();

	RECT rc;
//	GetWindowRect(&rc);
	GetClientRect( &rc );
	rc.right -= rc.left;
	rc.bottom -= rc.top;
	rc.top = rc.left = 0;


	RECT rcb;
	rcb.top = 0;
	rcb.left = 0;
	rcb.bottom = CHKHEIGHT;
	rcb.right = rc.right;
	m_chkbutton.Create( "Button", m_hWnd, rcb, NULL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX );
	m_chkbutton.SetWindowTextA( "ドラッグ＆ドロップ禁止" );


	RECT rct;
	rct.top = rcb.bottom;
	rct.bottom = rc.bottom;
	rct.left = rc.left;
	rct.right = rc.right;

	m_ctlSysTreeView32.Create( m_hWnd, rct, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER |  
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES);


	HBITMAP hBitmap;
	m_hImageList = ImageList_Create( 16, 16, ILC_COLOR, 6, 10 );
	if( m_hImageList == NULL ){
		DbgOut( "SigEditWnd : OnCreate : ImageList_Create error !!!\n" );
	}
	
	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_PART_IMAGE ) );
	if( hBitmap == NULL ){
		DbgOut( "SigEditWnd : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_PartImageNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_PartImageNo == -1 ){
		DbgOut( "SigEditWnd : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_PART_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "SigEditWnd : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_PartSelectNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_PartSelectNo == -1 ){
		DbgOut( "SigEditWnd : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_DISP_IMAGE ) );
	if( hBitmap == NULL ){
		DbgOut( "SigEditWnd : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_DispImageNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_DispImageNo == -1 ){
		DbgOut( "SigEditWnd : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_DISP_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "SigEditWnd : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_DispSelectNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_DispSelectNo == -1 ){
		DbgOut( "SigEditWnd : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_IMAGE ) );
	if( hBitmap == NULL ){
		DbgOut( "SigEditWnd : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_BoneImageNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_BoneImageNo == -1 ){
		DbgOut( "SigEditWnd : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "SigEditWnd : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_BoneSelectNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_BoneSelectNo == -1 ){
		DbgOut( "SigEditWnd : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_ctlSysTreeView32.m_hWnd,
		m_hImageList, TVSIL_NORMAL );


	return 0;
}

LRESULT CSigEditWnd::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );

	return 0;
}
LRESULT CSigEditWnd::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );

	return 0;
}
LRESULT CSigEditWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );

	return 0;
}
LRESULT CSigEditWnd::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	if( m_apphwnd ){
		::SendMessage( m_apphwnd, WM_USER_MOVE, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );
	}
	if( m_ctlSysTreeView32.IsWindow() ){
		m_ctlSysTreeView32.DefWindowProc();
	}


	DefWindowProc();

	RECT rect;
	GetWindowRect( &rect );
	g_inidata->sigdlg_posx = rect.left;
	g_inidata->sigdlg_posy = rect.top;

	return 0;
}
LRESULT CSigEditWnd::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_MOVING, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );
	DefWindowProc();

	return 0;
}
LRESULT CSigEditWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rect1;
	GetClientRect( &rect1 );
	rect1.right -= rect1.left;
	rect1.bottom -= rect1.top;
	rect1.top = 0;
	rect1.left = 0;

	if( m_chkbutton.IsWindow() ){
		m_chkbutton.SetWindowPos( HWND_TOP, 0, 0, rect1.right, CHKHEIGHT, SWP_NOMOVE | SWP_NOZORDER );
	}

	if( m_ctlSysTreeView32.IsWindow() ){
		m_ctlSysTreeView32.SetWindowPos( HWND_TOP, 0, CHKHEIGHT, rect1.right, rect1.bottom - CHKHEIGHT, SWP_NOMOVE | SWP_NOZORDER );
	}

	if( IsWindow() ){
		DefWindowProc();
	}

	return 0;
}

LRESULT CSigEditWnd::OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( IsWindow() && m_papp ){
		RECT rect;
		GetWindowRect( &rect );

		rect.right -= rect.left;
		rect.bottom -= rect.top;
		rect.left = 0;
		rect.top = 0;

		int ret;
		m_papp->m_bReady = FALSE;
		ret = m_papp->ComputeRect( 1, &rect );
		_ASSERT( !ret );

		ret = m_papp->Resize3DEnvironment();
		_ASSERT( !ret );
		m_papp->m_bReady = TRUE;

		BOOL dummy;
		OnSize( 0, 0, 0, dummy );

	}
	return 0;
}

//	LRESULT CSigEditWnd::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT CSigEditWnd::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


int CSigEditWnd::OnMenuShdParam()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );
	CTreeElem2* telem;
	telem = (*m_thandler)( m_selectedno );


	CShdElem* melem;
	melem = IsMorphObj( selem );
	if( (selem->type != SHDMORPH) && melem ){
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, "モーフフォルダの子供はエディットできません。",
			"エディット禁止",
			MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}


	CShdParamDlg dlg( telem, selem, m_thandler, m_shandler, m_mhandler, this );

	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, (WPARAM)0, 0 );
	int dlgret;
	dlgret = (int)dlg.DoModal(m_hWnd);


	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, (WPARAM)1, 0 );

	return 0;
}
int CSigEditWnd::OnMenuDelJoint( int srcdel, int dlgflag )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	int ret;


	int delno;
	CShdElem* delelem;
	if( srcdel <= 0 ){
		delno = m_selectedno;
	}else{
		delno = srcdel;
	}
	delelem = (*m_shandler)( delno );
	_ASSERT( delelem );


	if( !delelem->IsJoint() || (delelem->type == SHDMORPH) ){
		if( dlgflag ){
			::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
			ret = ::MessageBox( m_hWnd,
				(LPCTSTR)"選択中の要素は、ジョイントではないので、削除できません。",
				(LPCTSTR)"削除できません。",
				MB_OK );
			::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		}
		return 0;//!!!!!!!!!!!!!!!
	}

	if( dlgflag ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( ret != IDOK ){
			return 0;
		}
	}

	LRESULT lres;
	lres = ::SendMessage( m_apphwnd, WM_USER_DEL_JOINT, (WPARAM)delno, 0 );
	if( lres ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CSigEditWnd::OnMenuConv2Scope()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	int ret;

	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );
	_ASSERT( selem );

	if( (selem->type != SHDPOLYMESH) && (selem->type != SHDPOLYMESH2) ){
		::MessageBox( m_hWnd, "選択している要素は、表示用オブジェクトではありません。\n三角マークの要素を選んで、再試行してください。", "選択エラー", MB_OK );
		return 0;
	}

	CSelScopeDlg dlg( m_thandler, m_shandler, m_apphwnd, m_selectedno );
	ret = (int)dlg.DoModal();

	return ret;
}

/***
LRESULT CSigEditWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	unsigned char KeyBuffer[256];
	GetKeyboardState( KeyBuffer );


	//if( uMsg == WM_KEYDOWN ){
	//	DbgOut( "SigEditWnd : OnKeyDown : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
	//}else if( uMsg == WM_KEYUP ){
	//	DbgOut( "SigEditWnd : OnKeyDown : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
	//}


	if( (KeyBuffer[VK_CONTROL] & 0x80) && (KeyBuffer['I'] & 0x80) ){
		DbgOut( "CSigEditWnd : Control-I\n" );

		int ret;
		BOOL dummy;
		ret = OnMenuShdParam(0, 0, 0, dummy);

	}

	::DefWindowProc( m_ctlSysTreeView32.m_hWnd, uMsg, wParam, lParam );


	return 0;
}
***/
LRESULT CSigEditWnd::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_clicked_lbutton == 0 )
		return 0;

	int ret;
	if( uMsg != 0 ){
		TVHITTESTINFO ht;
		POINT htpoint;
		htpoint.x = LOWORD(lParam);
		htpoint.y = HIWORD(lParam);
		ht.pt = htpoint;
		ht.flags = TVHT_ONITEM;
		ht.hItem = 0;
		TreeView_HitTest( m_ctlSysTreeView32.m_hWnd,
			&ht );

		if( !(ht.flags & TVHT_ONITEM) || m_onitembutton ){
			m_clicked_lbutton = 0;
			m_onitembutton = 0;
			return 0;
		}

		if( m_forbidDD != 0 ){
			_ASSERT( 0 );
			m_clicked_lbutton = 0;
			return 0;//!!!!!!!!!!!!
		}

		m_dropitem = ht.hItem;
		TVITEM tvi;
		tvi.hItem = m_dropitem;
		tvi.mask = TVIF_HANDLE | TVIF_PARAM;
		int ret;
		ret = TreeView_GetItem( m_ctlSysTreeView32.m_hWnd, &tvi );

		m_dropno = (int)tvi.lParam;
	}else{
		m_dropno = (int)wParam;
		m_selectedno = (int)lParam;
	}

	if( m_dropno != m_selectedno ){

		int ismorphmove = 0;//!!!!!!!!!!!!!!

		ret = MakeMoveList();
		_ASSERT( !ret );

		ret = IsValidMove();
		if( !ret ){
			_ASSERT( 0 );
			m_clicked_lbutton = 0;
			return 0;
		}

		CShdElem* dropelem = (*m_shandler)( m_dropno );
		CShdElem* dragelem = (*m_shandler)( m_selectedno );
		CShdElem* melem;
		melem = IsMorphObj( dropelem );
		if( melem ){
			CMorph* morph;
			morph = melem->morph;
			_ASSERT( morph );
			int okflag = 0;
			okflag = morph->CheckTargetOK( dragelem );
			if( okflag == 0 ){
				_ASSERT( 0 );
				m_clicked_lbutton = 0;
				::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
				::MessageBox( m_hWnd, 
					"ベースとターゲットの構成が異なるので移動できません。",
					"移動禁止",
					MB_OK );
				::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
				return 0;
			}
			ismorphmove = 1;
		}

		ret = MoveTree();
		if( ret ){
			_ASSERT( 0 );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "CSigEditWnd::OnLButtonUP MoveTree error" );
			return -1;
		}

		if( ismorphmove ){
			m_papp->m_remaketree_calc0 = 1;
		}
		_ASSERT( m_apphwnd );
		::SendMessage( m_apphwnd, WM_USER_REMAKE_TREE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)m_selectedno );
		if( ismorphmove ){
			m_papp->m_remaketree_calc0 = 0;
		}

	}

	m_clicked_lbutton = 0;

	return 0;
}
LRESULT CSigEditWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	if( wParam == MK_LBUTTON )
		m_clicked_lbutton = 1;


	return 0;
}
LRESULT CSigEditWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//DefWindowProc();
	::DefWindowProc( m_ctlSysTreeView32.m_hWnd, uMsg, wParam, lParam );
	bHandled = FALSE;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	
	m_onitembutton = 0;

	//if( m_clicked_lbutton == 1 ){
	//	DbgOut( "SigEditWnd : OnLButtonDown : m_clicked_lbutton == 1 return \n" );
	//	return 0;
	//}

	TVHITTESTINFO ht;
	POINT htpoint;
	htpoint.x = LOWORD(lParam);
	htpoint.y = HIWORD(lParam);
	ht.pt = htpoint;
	ht.flags = TVHT_ONITEM;
	ht.hItem = 0;
	TreeView_HitTest( m_ctlSysTreeView32.m_hWnd,
		&ht );

	if( ht.flags & TVHT_ONITEMBUTTON ){
		m_onitembutton = 1;
		return 0;
	}

	if( ht.flags & TVHT_ONITEM ){
		TVITEM tvi;
		tvi.hItem = ht.hItem;
		tvi.mask = TVIF_HANDLE | TVIF_PARAM;
		int ret;
		ret = TreeView_GetItem( m_ctlSysTreeView32.m_hWnd, &tvi );
		int clickno;
		clickno = (int)tvi.lParam;
	
//DbgOut( "SigEditWnd : OnLButtonDown : clickno %d\n", clickno );

		if( m_selectedno != clickno ){
			OnUserSelchange( clickno, 0 );
		}
	}

	return 0;
}
LRESULT CSigEditWnd::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	POINT clickpos;
	clickpos.x = GET_X_LPARAM(lParam); 
	clickpos.y = GET_Y_LPARAM(lParam); 
	m_ctlSysTreeView32.ScreenToClient( &clickpos );
	
	TVHITTESTINFO hittestinfo;
	hittestinfo.pt = clickpos;
	hittestinfo.flags = TVHT_ONITEMLABEL;
	hittestinfo.hItem = NULL;

	HTREEITEM hititem = NULL;
	hititem = TreeView_HitTest( m_ctlSysTreeView32.m_hWnd, &hittestinfo );

	if( hititem != NULL ){
		
		TVITEM tvi;
		tvi.hItem = hititem;
		tvi.mask = TVIF_HANDLE | TVIF_PARAM;
		ret = TreeView_GetItem( m_ctlSysTreeView32.m_hWnd, &tvi );
		int clickno;
		clickno = (int)tvi.lParam;
	
		if( m_selectedno != clickno ){
			OnUserSelchange( clickno, 0 );
		}

		ret = OnMenuShdParam();
		_ASSERT( !ret );

	}

	return 0;
}

int CSigEditWnd::SetAppWnd( HWND appwnd, CMyD3DApplication* papp )
{
	m_apphwnd = appwnd;
	m_papp = papp;

	return 0;
}
int CSigEditWnd::SetParams( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, HWND appwnd )
{

	m_thandler = lpth;
	m_shandler = lpsh;
	m_mhandler = lpmh;
	m_apphwnd = appwnd;

	if( m_thandler || m_shandler || m_mhandler ){
		//既存のTreeを壊す。
		m_invalidate_handler = 1;

		TreeView_DeleteAllItems( m_ctlSysTreeView32.m_hWnd );

		if( m_TI ){
			free( m_TI );
			m_TI = 0;
		}
	}

	m_invalidate_handler = 0;


//	CShdElem* elem9;
//	elem9 = (*m_shandler)( 9 );
//	elem9->m_notsel = 1;


	int ret;
	ret = FillTree();
	if( ret ){
		DbgOut( "SigEditWnd : SetParams : FillTree error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}

//	m_ctlSysTreeView32.UpdateWindow();
//	UpdateWindow();

	return 0;
}
int CSigEditWnd::Redraw()
{
	::UpdateWindow( m_hWnd );
	if( m_ctlSysTreeView32.IsWindow() ){
		::UpdateWindow( m_ctlSysTreeView32.m_hWnd );
	}

	return 0;
}
int CSigEditWnd::OnUserSelchange( long selno, int usercall )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;

	HTREEITEM selitem;
	selitem = *(m_TI + (int)selno);
	if( selitem ){
		m_selectedno = selno;
		m_selecteditem = selitem;

		m_usercallflag = usercall;
		TreeView_SelectItem( m_ctlSysTreeView32.m_hWnd,
			m_selecteditem );

//		TreeView_SelectSetFirstVisible( m_ctlSysTreeView32.m_hWnd, m_selecteditem );

		::UpdateWindow( m_ctlSysTreeView32.m_hWnd );

		::SendMessage( m_apphwnd, WM_USER_DISPLAY, 0, 0 );
	}

	return 0;
}
int CSigEditWnd::InvalidateHandler()
{
	m_invalidate_handler = 1;
	TreeView_DeleteAllItems( m_ctlSysTreeView32.m_hWnd );

	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}
	
	m_thandler = 0;
	m_shandler = 0;
	m_mhandler = 0;
	m_selectedno = 0;
	m_selecteditem = 0;

	return 0;
}
int CSigEditWnd::Rename( int seri, char* srcname )
{
	if( (seri < 0) || !srcname ){
		return 0;
	}
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	char* setname;
	setname = (char*)srcname;

	HTREEITEM selitem;
	selitem = *(m_TI + seri);
	if( selitem ){
		TVITEM tvi;
		tvi.hItem = selitem;
		tvi.pszText = setname;
		tvi.cchTextMax = (int)(strlen( setname ) + 1);
		tvi.mask = TVIF_HANDLE | TVIF_TEXT;
		int ret;
		ret = TreeView_SetItem( m_ctlSysTreeView32.m_hWnd, &tvi );
		_ASSERT( ret > 0 );
	}

	return 0;
}
int CSigEditWnd::GetSelectedNo( int* selnoptr )
{
	*selnoptr = m_selectedno;
	return 0;
}

int CSigEditWnd::ChangeTVParams( int editflag, char* shdname )
{
	//if( dlgret == IDOK ){
	// 名前の更新
		int iseditname = editflag & TV_EDIT_SHDNAME;
		if( iseditname ){
			TVITEM tvi;
			tvi.hItem = m_selecteditem;
			tvi.pszText = shdname;
			tvi.cchTextMax = (int)(strlen( shdname ) + 1);
			tvi.mask = TVIF_HANDLE | TVIF_TEXT;
			int ret;
			ret = TreeView_SetItem( m_ctlSysTreeView32.m_hWnd, &tvi );
			_ASSERT( ret > 0 );

			_ASSERT( m_apphwnd );
			::SendMessage( m_apphwnd, WM_USER_RENAME, (WPARAM)m_selectedno, (LPARAM)shdname );

		}
		//int isedittexname = editflag & TV_EDIT_TEXNAME;
		//if( isedittexname ){
		//	_ASSERT( m_apphwnd );
		//	::SendMessage( m_apphwnd, WM_USER_REMAKE_TEXTURE, (WPARAM)m_selectedno, (LPARAM)dlg.m_texname );
		//}
		int iseditclockwise = editflag & TV_EDIT_CLOCKWISE;
		if( iseditclockwise ){
			_ASSERT( m_apphwnd );
			::SendMessage( m_apphwnd, WM_USER_REMAKE_DISPOBJ, (WPARAM)m_selectedno, 0 );
		}
		//int isedittexrepx = editflag & TV_EDIT_TEXREPX;
		//int isedittexrepy = editflag & TV_EDIT_TEXREPY;
		//int isedittexrule = editflag & TV_EDIT_TEXRULE;
		//if( isedittexrepx || isedittexrepy || isedittexrule ){
		//	_ASSERT( m_apphwnd );
		//	::SendMessage( m_apphwnd, WM_USER_REMAKE_UV, (WPARAM)m_selectedno, 0 );
		//}
			
		int iseditcolor = editflag & TV_EDIT_COLOR;
		if( iseditcolor ){
			_ASSERT( m_apphwnd );
			//::SendMessage( m_apphwnd, WM_USER_CHANGE_COLOR, (WPARAM)m_selectedno, 0 );
			::SendMessage( m_apphwnd, WM_USER_REMAKE_DISPOBJ, (WPARAM)m_selectedno, 0 );
		}

		int iseditnotuse = editflag & TV_EDIT_NOTUSE;
		if( iseditnotuse ){
			_ASSERT( m_apphwnd );
			::SendMessage( m_apphwnd, WM_USER_CHANGE_NOTUSE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)m_selectedno );
		}

		int iseditnoskinning = editflag & TV_EDIT_NOSKINNING;
		if( iseditnoskinning ){
			_ASSERT( m_apphwnd );
			::SendMessage( m_apphwnd, WM_USER_INFLUENCE, (WPARAM)m_selectedno, (LPARAM)0 );
		}

		int iseditallnoskinning = editflag & TV_EDIT_ALL_NOSKINNING;
		if( iseditallnoskinning ){
			m_shandler->SetAllNoSkinning( 1 );

			_ASSERT( m_apphwnd );
			::SendMessage( m_apphwnd, WM_USER_CHANGE_JOINTLOC, (WPARAM)0xFFFF, (LPARAM)0 );
		}

		int iseditallskinning = editflag & TV_EDIT_ALL_SKINNING;
		if( iseditallskinning ){
			m_shandler->SetAllNoSkinning( 0 );

			_ASSERT( m_apphwnd );
			::SendMessage( m_apphwnd, WM_USER_CHANGE_JOINTLOC, (WPARAM)0xFFFF, (LPARAM)0 );
		}

	//}

	//shdparadlgのOKボタンを押さないでも、TexSettingDlgのOKボタンを押せば、それが、反映されるようにする。
	int isedittexture = editflag & TV_EDIT_TEXTURE;
	if( isedittexture ){
		_ASSERT( m_apphwnd );
		::SendMessage( m_apphwnd, WM_USER_REMAKE_TEXTURE, (WPARAM)m_selectedno, 0 );
		::SendMessage( m_apphwnd, WM_USER_REMAKE_UV, (WPARAM)m_selectedno, 0 );
	}

	return 0;
}


int CSigEditWnd::FillTree()
{
	if( !m_thandler || !m_shandler || !m_mhandler ){
		_ASSERT( 0 );
		return 0;
	}

	HTREEITEM hRoot = TreeView_GetRoot( m_ctlSysTreeView32.m_hWnd );
	m_selecteditem = hRoot;

	int shdnum;
	shdnum = m_shandler->s2shd_leng;

	m_TI = (HTREEITEM*)malloc( sizeof( HTREEITEM ) * shdnum );
	if( !m_TI ){
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( m_TI, sizeof( HTREEITEM ) * shdnum );
	
	*m_TI = hRoot;


	m_selectedno = 0;
	m_selecteditem = 0;
	AddTree( 1, 1, 1 );
	//AddTree( 0, 1, 1 );


	int serino;
	for( serino = 0; serino < shdnum; serino++ ){
		HTREEITEM* curitem;
		curitem = m_TI + serino;
		if( curitem ){
			TreeView_Expand( m_ctlSysTreeView32.m_hWnd, *curitem, TVE_EXPAND );
	
			CShdElem* curselem;
			curselem = ( *m_shandler )( serino );
			_ASSERT( curselem );
			if( curselem->m_notsel == 0 ){
				TreeView_SetCheckState( m_ctlSysTreeView32.m_hWnd, *curitem, 0 );
			}else{
				TreeView_SetCheckState( m_ctlSysTreeView32.m_hWnd, *curitem, 1 );
			}

		}
	}
	TreeView_SelectSetFirstVisible( m_ctlSysTreeView32.m_hWnd, m_selecteditem );




	return 0;
}
void CSigEditWnd::AddTree( int srcseri, int addbroflag, int addtolast )
{
	//addtolastは初回のTVAddのみを制御する。
	//( CShdElemのTree構造と同期するため。)

	char* shdname;
	int parentno;
	CShdElem* curselem;
	CShdElem* parselem;
	HTREEITEM parTI;

	curselem = (*m_shandler)( srcseri );
	shdname = (*m_thandler)( srcseri )->name;

	if( (curselem->type != SHDDESTROYED) && (curselem->type != SHDINFSCOPE) && (curselem->type != SHDBBOX) ){
//	if( (curselem->type != SHDINFSCOPE) && (curselem->type != SHDBBOX) ){

		parselem = curselem->parent;
		if( parselem ){
			parentno = parselem->serialno;
		}else{
			parentno = 0;
		}
		parTI = *(m_TI + parentno);

		int imageno, selectno;
		if( curselem->IsJoint() ){
			imageno = m_BoneImageNo;
			selectno = m_BoneSelectNo;
		}else if( curselem->dispflag ){
			imageno = m_DispImageNo;
			selectno = m_DispSelectNo;
		}else{
			imageno = m_PartImageNo;
			selectno = m_PartSelectNo;
		}

		HTREEITEM newTI;
		newTI = TVAdd( parTI, shdname, srcseri, imageno, selectno, addtolast );
		if( !newTI ){
			DbgOut( "SigEditWnd : AddTree : TVAdd error : srcseri %d, parTI %x!!!\n",
				srcseri, parTI );
			_ASSERT( 0 );
			return;
		}
		*(m_TI + srcseri) = newTI;

		if( m_selectedno == 0 ){
			m_selectedno = srcseri;
			m_selecteditem = newTI;
			TreeView_SelectItem( m_ctlSysTreeView32.m_hWnd,
				m_selecteditem );
		}

	}

	///////////
	CShdElem* childselem;
	int childno;
	childselem = curselem->child;
	if( childselem ){
		childno = childselem->serialno;
		AddTree( childno, 1, 1 );
	}

	///////////
	if( addbroflag ){
		CShdElem* broselem;
		int brono;
		broselem = curselem->brother;
		if( broselem ){
			brono = broselem->serialno;
			AddTree( brono, 1, 1 );
		}
	}
}
HTREEITEM CSigEditWnd::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{
	//ここのDbg文は取り除かない。
	DbgOut( "SigEditWnd : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
		srcname, srcno, imageno, selectno, addtolast );


	TVITEM tvi;
	tvi.mask = TVIF_TEXT;
	tvi.pszText = srcname;
	//tvi.cchTextMax = strlen( srcname ) + 1;

	tvi.mask |= TVIF_PARAM;
	tvi.lParam = (LPARAM)srcno;

	if( imageno != -1 ){
		tvi.mask |= TVIF_IMAGE;
		tvi.iImage = imageno;
	}

	if( selectno != -1 ){
		tvi.mask |= TVIF_SELECTEDIMAGE;
		tvi.iSelectedImage = selectno;
	}

	TVINSERTSTRUCT tvins;
	tvins.item = tvi;
	if( addtolast )
		tvins.hInsertAfter = TVI_LAST;
	else
		tvins.hInsertAfter = TVI_FIRST;

	tvins.hParent = parentTI;

	HTREEITEM retitem;
	retitem = TreeView_InsertItem( m_ctlSysTreeView32.m_hWnd, &tvins );

	return retitem;
}

int CSigEditWnd::MakeMoveList()
{
	ZeroMemory( m_movelist, sizeof( HTREEITEM ) * 256 );
	m_movenum = 0;

	m_movelist[0] = m_selecteditem;
	m_movenum++;

	CShdElem* curselem;
	curselem = (*m_shandler)( m_selectedno );
	CShdElem* childselem;
	childselem = curselem->child;
	if( childselem ){
		SetMoveListReq( childselem );
	}

	return 0;
}
void CSigEditWnd::SetMoveListReq( CShdElem* srcselem )
{
	if( m_movenum > 256 ){
		DbgOut( "CSigEditWnd : SetMoveListReq : movenum range error !!!\n" );
		_ASSERT( 0 );
		return;
	}
	int elemno = srcselem->serialno;
	HTREEITEM curitem = *(m_TI + elemno);
	m_movelist[m_movenum] = curitem;
	m_movenum++;
	
	CShdElem* childselem = srcselem->child;
	if( childselem ){
		SetMoveListReq( childselem );
	}

	CShdElem* broselem = srcselem->brother;
	if( broselem ){
		SetMoveListReq( broselem );
	}

}
int CSigEditWnd::IsValidMove()
{
	//int parentno;
	CShdElem* curselem = (*m_shandler)( m_selectedno );
	CShdElem* parentselem = curselem->parent;
	if( !parentselem || (parentselem->serialno == 0) ){
		//parentのないルートは移動できない。
		return 0;
	}

	/***
	if( parentselem && (parentselem->serialno != 0) ){
		parentno = parentselem->serialno;
		if( m_dropno == parentno ){
			//自分の親への移動(意味がない)の禁止。
			return 0;
		}
	}else{
		//parentのないルートは移動できない。
		return 0;
	}
	***/

	//自分の子項目への移動の禁止
	//m_dropnoがm_movelistにないかチェック。
	int listno;
	for( listno = 0; listno < m_movenum; listno++ ){
		TVITEM tvi;
		tvi.hItem = m_movelist[listno];
		tvi.mask = TVIF_HANDLE | TVIF_PARAM;
		if( !TreeView_GetItem( m_ctlSysTreeView32.m_hWnd, &tvi ) )
			return 0;

		int curno = (int)(tvi.lParam);
		if( curno == m_dropno )
			return 0;
	}

	return 1;
}
int CSigEditWnd::MoveTree()
{
	//ShdElemの変更
	int ret;
	CShdElem* mvselem = (*m_shandler)( m_selectedno );
	CShdElem* dropselem = (*m_shandler)( m_dropno );
	CShdElem* dropmelem;
	dropmelem = IsMorphObj( dropselem );
	CShdElem* mvmelem;
	mvmelem = IsMorphObj( mvselem );


	//morphへのドラッグ
	if( dropmelem ){
		if( mvselem->m_mtype != M_NONE ){
			//_ASSERT( 0 );
			return 0;
		}
		CMorph* morph;
		morph = dropmelem->morph;
		if( morph ){
			ret = morph->AddMorphTarget( mvselem );
			_ASSERT( !ret );
			mvselem->dispflag = 0;//!!!!!!!!!
		}else{
			_ASSERT( 0 );
		}
	}
	
	//morphからのドラッグ
	if( mvmelem ){
		if( !dropmelem || (mvmelem != dropmelem) ){
			if( mvselem->m_mtype == M_BASE ){
				_ASSERT( 0 );
				return 0;//baseは外に移動できない。
			}

			CMorph* morph2;
			morph2 = mvmelem->morph;
			ret = morph2->DeleteMorphTarget( mvselem );
			_ASSERT( !ret );
			mvselem->dispflag = 1;//!!!!!!!!!!!
		}
	}

	ret = mvselem->LeaveFromChain();// child情報は、そのままで、チェインから離す。
	if( ret )
		return 1;

	ret = mvselem->PutToChain( dropselem );
	if( ret )
		return 1;

	ret = m_shandler->CalcDepth();
	if( ret )
		return 1;
	
	//MotElemの変更
	ret = m_mhandler->CopyChainFromShd( m_shandler );
	if( ret )
		return 1;

	ret = m_mhandler->CalcDepth();
	if( ret )
		return 1;

	//TreeElemの変更はなし！！

//	if( editmorphflag == 1 ){
//		ret = m_shandler->SetDispFlag( 1 );
//		_ASSERT( !ret );
//	}



	//TreeViewの変更
		//Treeの追加
	AddTree( m_selectedno, 0, 0 );	


		//selectedの更新
	m_selecteditem = *(m_TI + m_selectedno);
	TreeView_SelectItem( m_ctlSysTreeView32.m_hWnd,
		m_selecteditem );
	
		//Treeの削除
	int delno;
	for( delno = 0; delno < m_movenum; delno++ ){
		HTREEITEM delitem = m_movelist[delno];

		TreeView_DeleteItem( m_ctlSysTreeView32.m_hWnd,
			delitem );
	}

	g_motdlg->ResetMorphDlg();


	return 0;
}

CShdElem* CSigEditWnd::IsMorphObj( CShdElem* srcselem )
{
	//自分、または親のパートに、SHDMORPHがある場合はそのモーフ要素を返す。
	CShdElem* retelem = 0;
	CShdElem* chkelem = srcselem;	

	while( chkelem ){
		if( chkelem->type == SHDMORPH ){
			retelem = chkelem;
			break;
		}
		chkelem = chkelem->parent;
	}

	return retelem;

}
void CSigEditWnd::ErrorMes( int errorcode, int type, char* mes )
{
	ERRORMES errormes;
	if( m_apphwnd ){
		errormes.errorcode = errorcode;
		errormes.type = type;
		errormes.mesptr = mes;
		SendMessage( m_apphwnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
	}
}

LRESULT CSigEditWnd::OnSelChanging( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( m_invalidate_handler )
		return 0;

	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;

	UINT state;
	state = tvi.state;

	if( m_usercallflag == 0 ){
		if( ((state >> 12) - 1) > 0 ){
			return 1;
		}else{
			return 0;
		}
	}else{
		//::MessageBox( m_hWnd, "usercall", "check", MB_OK );
		m_usercallflag = 0;
		return 0;
	}

//	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
//	UINT state;
//	state = pnmtv->itemNew.state;
//
//	if( ((state >> 12) - 1) > 0 ){
//		return 1;
//	}else{
//		return 0;
//	}
}


LRESULT CSigEditWnd::OnSelChanged( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( m_invalidate_handler )
		return 0;
	

	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;
	m_selecteditem = tvi.hItem;

	tvi.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_STATE;
	if( !TreeView_GetItem( m_ctlSysTreeView32.m_hWnd, &tvi ) )
		return 0;

	m_selectedno = (int)(tvi.lParam);

	CTreeElem2* seltelem;
	seltelem = (*m_thandler)( m_selectedno );
	_ASSERT( seltelem );

	/***
	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );
	_ASSERT( selem );

	UINT state;
	state = tvi.state;
	if( ((state >> 12) - 1) > 0 ){
		selem->m_notsel = 1;
	}else{
		selem->m_notsel = 0;
	}
	***/

	TreeView_EnsureVisible( m_ctlSysTreeView32.m_hWnd, m_selecteditem );


	::SendMessage( m_apphwnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)m_selectedno );

	DbgOut( "CSigEditWnd : selected %s : %d\n",
		seltelem->name, m_selectedno );

	return 0;
}

int CSigEditWnd::ForbidSelect( int selno )
{
	HTREEITEM selti;

	selti = *(m_TI + selno);
	if( selti ){

		TVITEM tvi;
		tvi.hItem = selti;
		tvi.mask = TVIF_HANDLE | TVIF_STATE;
		if( !TreeView_GetItem( m_ctlSysTreeView32.m_hWnd, &tvi ) )
			return 0;

		UINT state;
		state = tvi.state;
		if( ((state >> 12) - 1) > 0 ){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}

int CSigEditWnd::SaveForbidSelect()
{

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( m_invalidate_handler )
		return 0;

	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		int forbid;
		forbid = ForbidSelect( seri );

		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );

		selem->m_notsel = forbid;
	}

	return 0;
}



LRESULT CSigEditWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD code = HIWORD( wParam );
	HWND hwnd = (HWND)lParam;

	if( (code == BN_CLICKED) && (hwnd == m_chkbutton.m_hWnd) ){
		//check button

		if( m_forbidDD == 0 ){
			m_chkbutton.SendMessageA( BM_SETCHECK, BST_CHECKED, 0 );

			m_forbidDD = 1;
		}else{
			m_chkbutton.SendMessageA( BM_SETCHECK, BST_UNCHECKED, 0 );

			m_forbidDD = 0;
		}
	}

	return 0;
}

int CSigEditWnd::SetForbidDD( int flag )
{
	m_forbidDD = flag;

	if( m_forbidDD == 0 ){
		m_chkbutton.SendMessageA( BM_SETCHECK, BST_UNCHECKED, 0 );
	}else{
		m_chkbutton.SendMessageA( BM_SETCHECK, BST_CHECKED, 0 );
	}

	return 0;
}