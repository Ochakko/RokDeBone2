// MFolderDlg.cpp : CMFolderDlg のインプリメンテーション
#include "stdafx.h"
#include "MFolderDlg.h"

#include <treehandler2.h>
#include <shdhandler.h>
#include <shdelem.h>

//extern HINSTANCE g_resinstance;

static LONG s_treefunc = 0;
static HWND s_tree_wnd = 0;

static LRESULT CALLBACK TreeProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  );

/////////////////////////////////////////////////////////////////////////////
// CMFolderDlg

CMFolderDlg::CMFolderDlg( CTreeHandler2* lpth, CShdHandler* lpsh )
{
	m_hImageList = 0;
	m_iImage = 0;
	m_iSelect = 0;

	m_TI = 0;
	m_selecteditem = 0;
	
	m_thandler = lpth;
	m_shandler = lpsh;

	m_baseseri = 0;
	m_motionseri = 0;

}

CMFolderDlg::~CMFolderDlg()
{
	TreeView_DeleteAllItems( m_tree_wnd );

	ImageList_Destroy( m_hImageList );
	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}
	
}
	
LRESULT CMFolderDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	//InitCommonControls();

	HWND temphwnd;
	temphwnd = GetDlgItem( IDC_TREE1 );
	m_tree_wnd = temphwnd;
	s_tree_wnd = m_tree_wnd;

	m_list3_wnd = GetDlgItem( IDC_LIST3 );

	CreateImageList();

	ret = FillTree();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = CreateList3();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = ParamsToDlg();
	_ASSERT( !ret );

	ret = SetNewWindowProc();
	_ASSERT( !ret );


	return 1;  // システムにフォーカスを設定させます
}

///////////////
LRESULT CALLBACK TreeProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  ){
	if( (uMsg != WM_KEYDOWN) && (uMsg != WM_KEYUP) ){
		return CallWindowProc(
		  (WNDPROC)s_treefunc,  // pointer to previous procedure
		  s_tree_wnd,              // handle to window
		  uMsg,               // message
		  wParam,          // first message parameter
		  lParam           // second message parameter
		);		
	}else{
		//key は無効にする。

		//if( uMsg == WM_KEYDOWN ){
		//	DbgOut( "MotParamDlg : TreeMotProc : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}else if( uMsg == WM_KEYUP ){
		//	DbgOut( "MotParamDlg : TreeMotProc : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
		//}


		return 0;
	}
}

int CMFolderDlg::SetNewWindowProc()
{
	s_treefunc = ::GetWindowLong( m_tree_wnd, GWL_WNDPROC );
	if( !s_treefunc ){
		_ASSERT( 0 );
		return 1;
	}

	LONG lret;
	lret = ::SetWindowLong( m_tree_wnd, GWL_WNDPROC, (LONG)TreeProc );
	if( lret == 0 ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


LRESULT CMFolderDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_baseseri <= 0 ){
		::MessageBox( m_hWnd, "ベース形状が選択されていません。", "エラー", MB_OK );
		return 0;
	}
	if( m_motionseri <= 0 ){
		::MessageBox( m_hWnd, "モーション格納ボーンが選択されていません。", "エラー", MB_OK );
		return 0;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CMFolderDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CMFolderDlg::FillTree()
{
	HTREEITEM hRoot = TreeView_GetRoot( m_tree_wnd );
	//m_selecteditem = hRoot;

	int shdnum;
	shdnum = m_shandler->s2shd_leng;

	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}

	m_TI = (HTREEITEM*)malloc( sizeof( HTREEITEM ) * shdnum );
	if( !m_TI ){
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( m_TI, sizeof( HTREEITEM ) * shdnum );
	
	*m_TI = hRoot;

	m_motionseri = 0;
	m_selecteditem = 0;
	AddBoneToTree( 0, 1, 1 );

	int serino;
	for( serino = 0; serino < shdnum; serino++ ){
		HTREEITEM* curitem;
		curitem = m_TI + serino;
		if( curitem ){
			TreeView_Expand( m_tree_wnd, *curitem, TVE_EXPAND );
		}
	}
	TreeView_SelectSetFirstVisible( m_tree_wnd, m_selecteditem );


	return 0;
}
void CMFolderDlg::AddBoneToTree( int srcseri, int addbroflag, int addtolast )
{
	//addtolastは初回のTVAddのみを制御する。
	//( CShdElemのTree構造と同期するため。)

	char* shdname;
	//int parentno;
	CShdElem* curselem;
	//CShdElem* parselem;
	HTREEITEM parTI;
	//static int s_isfirst = 1;

	curselem = (*m_shandler)( srcseri );
		
	// jointをTreeにAddする。
	if( curselem->IsJoint() && (curselem->type != SHDMORPH)){
		CPart* partptr = curselem->part;
		_ASSERT( partptr );

		CShdElem* parselem;
		parselem = m_shandler->FindUpperJoint( curselem );
		if( parselem ){
			parTI = *(m_TI + parselem->serialno);
		}else{
			parTI = *m_TI;
		}


		shdname = (*m_thandler)( srcseri )->name;

		HTREEITEM newTI;
		newTI = TVAdd( parTI, shdname, srcseri, m_iImage, m_iSelect, addtolast );
		if( !newTI ){
			DbgOut( "MFolderDlg : AddBoneToTree : TVAdd error %d %s!!!\n", srcseri, shdname );
			_ASSERT( 0 );
			return;
		}
		*(m_TI + srcseri) = newTI;
		if( m_motionseri == 0 ){
			m_selecteditem = newTI;
			m_motionseri = srcseri;

			TreeView_SelectItem( m_tree_wnd, m_selecteditem );
		}
	}

	///////////
	CShdElem* childselem;
	int childno;
	childselem = curselem->child;
	if( childselem ){
		childno = childselem->serialno;
		AddBoneToTree( childno, 1, 1 );
	}

	///////////
	if( addbroflag ){
		CShdElem* broselem;
		int brono;
		broselem = curselem->brother;
		if( broselem ){
			brono = broselem->serialno;
			AddBoneToTree( brono, 1, 1 );
		}
	}

}
HTREEITEM CMFolderDlg::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{
	//ここのDbg文は取り除かない。
	DbgOut( "MFolderDlg : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
		srcname, srcno, imageno, selectno, addtolast );

	TVITEM tvi;
	tvi.mask = TVIF_TEXT;
	tvi.pszText = srcname;

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

	return TreeView_InsertItem( m_tree_wnd, &tvins );

}

void CMFolderDlg::CreateImageList()
{
	m_tree_wnd.SetWindowLong( GWL_STYLE, 
		WS_CHILD | WS_VISIBLE | WS_BORDER | 
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS
		);

	HBITMAP hBitmap;
	m_hImageList = ImageList_Create( 16, 16, ILC_COLOR, 2, 10 );
	
	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_IMAGE ) );
	if( hBitmap == NULL ){
		DbgOut( "MFolderDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iImage = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iImage == -1 ){
		DbgOut( "MFolderDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "MFolderDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iSelect = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iSelect == -1 ){
		DbgOut( "MFolderDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_tree_wnd,
		m_hImageList, TVSIL_NORMAL );

	
}
int CMFolderDlg::ParamsToDlg()
{
	return 0;
}

LRESULT CMFolderDlg::OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;

	m_selecteditem = tvi.hItem;

	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !TreeView_GetItem( m_tree_wnd, &tvi ) )
		return 0;

	m_motionseri = (int)(tvi.lParam);

	return 0;
}

LRESULT CMFolderDlg::OnSelDisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int selindex;
	selindex = (int)m_list3_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		return 0;
	}

	m_baseseri = (int)m_list3_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );

	if( m_baseseri <= 0 ){
		_ASSERT( 0 );
		return 0;
	}

	return 0;
}



int CMFolderDlg::CreateList3()
{
	
	int seri;
	for( seri = 1; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );


		//if( (selem->IsJoint() == 0) && (selem->dispflag == 1) && (selem->notuse == 0) && (selem->type != SHDINFSCOPE) && (selem->type != SHDBBOX) && (selem->m_mtype == M_NONE) ){
		if( ( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ) && 
			(selem->notuse == 0) && (selem->dispflag == 1) && (selem->m_mtype == M_NONE) ){
			CTreeElem2* telem;
			telem = (*m_thandler)( seri );
			_ASSERT( telem );

			int newindex;
			newindex = (int)m_list3_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)telem->name );
			if( (newindex == LB_ERR) || (newindex == LB_ERRSPACE) ){
				_ASSERT( 0 );
				return 1;
			}

			int ret;
			ret = (int)m_list3_wnd.SendMessage( LB_SETITEMDATA, newindex, selem->serialno );
			if( ret == LB_ERR ){
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}
