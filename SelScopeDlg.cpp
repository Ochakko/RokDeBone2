// SelScopeDlg.cpp : CSelScopeDlg のインプリメンテーション
#include "stdafx.h"
#include "SelScopeDlg.h"

#include <treehandler2.h>
#include <shdhandler.h>
#include <shdelem.h>

//extern HINSTANCE g_resinstance;

/////////////////////////////////////////////////////////////////////////////
// CSelScopeDlg

CSelScopeDlg::CSelScopeDlg( CTreeHandler2* lpth, CShdHandler* lpsh, HWND appwnd, int srcseri )
{
	m_hImageList = 0;
	m_iImage = 0;
	m_iSelect = 0;

	m_TI = 0;
	m_selecteditem = 0;
	m_selbone = 0;
	
	m_thandler = lpth;
	m_shandler = lpsh;

	m_apphwnd = appwnd;

	m_seldisp = 0;

	m_convno = srcseri;
	m_topflag = 0;
}

CSelScopeDlg::~CSelScopeDlg()
{
	TreeView_DeleteAllItems( m_tree_wnd );

	ImageList_Destroy( m_hImageList );
	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}
	
	if( m_topflag ){
		free( m_topflag );
		m_topflag = 0;
	}
}
	
LRESULT CSelScopeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	//InitCommonControls();

	m_tree_wnd = GetDlgItem( IDC_TREE1 );
	m_list_wnd = GetDlgItem( IDC_LIST1 );

	CreateImageList();

	ret = FillTree();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = CreateList();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = ParamsToDlg();
	_ASSERT( !ret );


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSelScopeDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( m_selbone <= 0 ){
		::MessageBox( m_hWnd, "ボーンが選択されていません。\n選択してから、再試行してください。", "未選択", MB_OK );
		return 0;
	}
	if( (m_selbone < 0) || (m_selbone >= m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		::MessageBox( m_hWnd, "エラーが生じました。処理を終了します。", "エラー", MB_OK );
		EndDialog(wID);
		return 0;
	}

	
	int selindex;
	selindex = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		::MessageBox( m_hWnd, "表示オブジェクトを選択してから、\n再試行してください。", "未選択", MB_OK );
		return 0;
	}
	m_seldisp = (int)m_list_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );
	if( (m_seldisp < 0) || (m_seldisp >= m_shandler->s2shd_leng) || (m_seldisp == m_convno) ){
		_ASSERT( 0 );
		::MessageBox( m_hWnd, "エラーが生じました。処理を終了します。", "エラー", MB_OK );
		EndDialog(wID);
		return 0;
	}


	int existindex;
	existindex = m_shandler->ExistInfScope( m_selbone, m_seldisp );
	if( existindex >= 0 ){
		ret = (int)::MessageBox( m_hWnd, "すでに指定したパラメータの影響範囲は存在します。\n形状データを既存のものと置き換えますか?", "確認", MB_OKCANCEL );
		if( ret != IDOK ){
			::MessageBox( m_hWnd, "パラメータを選択し直して、再試行してください。", "やり直し", MB_OK );
			return 0;
		}
	}

	int mesarray[3];
	mesarray[0] = m_selbone;
	mesarray[1] = m_seldisp;
	mesarray[2] = m_convno;

	::SendMessage( m_apphwnd, WM_USER_CONV2SCOPE, 0, (LPARAM)mesarray );


	EndDialog(wID);
	return 0;
}

LRESULT CSelScopeDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CSelScopeDlg::FillTree()
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
	
	m_topflag = (int*)malloc( sizeof( int ) * shdnum );
	if( !m_topflag ){
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( m_topflag, sizeof( int ) * shdnum );

	int seri;
	for( seri = 0; seri < shdnum; seri++ ){
		CShdElem* curelem;
		curelem = (*m_shandler)( seri );
		_ASSERT( curelem );

		CShdElem* parelem;
		parelem = m_shandler->FindUpperJoint( curelem );
		if( parelem ){
			*( m_topflag + seri ) = 0;
		}else{
			*( m_topflag + seri ) = 1;
		}
	}



	*m_TI = hRoot;

	m_selbone = 0;
	m_selecteditem = 0;
	//AddBoneToTree( 1, 1, 1 );
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
void CSelScopeDlg::AddBoneToTree( int srcseri, int addbroflag, int addtolast )
{
	//addtolastは初回のTVAddのみを制御する。
	//( CShdElemのTree構造と同期するため。)

	char* shdname;
	CShdElem* curselem;
	//CShdElem* parselem;
	HTREEITEM parTI;
	//static int s_isfirst = 1;

	curselem = (*m_shandler)( srcseri );
		
	// jointのものだけTreeにAddする。

	//!!!!! 親の無いジョイントはaddしない。 !!!!!!!
	
	if( curselem->IsJoint() && (curselem->type != SHDMORPH)){
		CPart* partptr = curselem->part;
		_ASSERT( partptr );

		//bonenum 0 もadd!!!!!

		if( *( m_topflag + srcseri ) == 0 ){
			CShdElem* parelem;
			parelem = m_shandler->FindUpperJoint( curselem );
			if( parelem && (*(m_topflag + parelem->serialno) == 0) ){
				parTI = *( m_TI + parelem->serialno );
			}else{
				parTI = *m_TI;
			}

			shdname = (*m_thandler)( srcseri )->name;

			HTREEITEM newTI;
			newTI = TVAdd( parTI, shdname, srcseri, m_iImage, m_iSelect, addtolast );
			if( !newTI ){
				DbgOut( "SelScopeDlg : AddBoneToTree : TVAdd error %d %s!!!\n", srcseri, shdname );
				_ASSERT( 0 );
				return;
			}
			*(m_TI + srcseri) = newTI;
			if( m_selbone == 0 ){
				m_selecteditem = newTI;
				m_selbone = srcseri;

				TreeView_SelectItem( m_tree_wnd, m_selecteditem );
			}
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
HTREEITEM CSelScopeDlg::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{
	//ここのDbg文は取り除かない。
	DbgOut( "SelScopeDlg : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
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

void CSelScopeDlg::CreateImageList()
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
		DbgOut( "SelScopeDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iImage = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iImage == -1 ){
		DbgOut( "SelScopeDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "SelScopeDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iSelect = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iSelect == -1 ){
		DbgOut( "SelScopeDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_tree_wnd,
		m_hImageList, TVSIL_NORMAL );

	
}
int CSelScopeDlg::ParamsToDlg()
{
	return 0;
}

LRESULT CSelScopeDlg::OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;

	m_selecteditem = tvi.hItem;

	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !TreeView_GetItem( m_tree_wnd, &tvi ) )
		return 0;

	m_selbone = (int)(tvi.lParam);

	return 0;
}


int CSelScopeDlg::CreateList()
{
	
	int seri;
	for( seri = 1; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );


		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){

			CTreeElem2* telem;
			telem = (*m_thandler)( seri );
			_ASSERT( telem );


			if( seri != m_convno ){//変換元データは、表示しない
				int newindex;
				newindex = (int)m_list_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)telem->name );
				if( (newindex == LB_ERR) || (newindex == LB_ERRSPACE) ){
					_ASSERT( 0 );
					return 1;
				}

				int ret;
				ret = (int)m_list_wnd.SendMessage( LB_SETITEMDATA, newindex, selem->serialno );
				if( ret == LB_ERR ){
					_ASSERT( 0 );
					return 1;
				}
			}
		}
	}

	return 0;
}
