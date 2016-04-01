// FrameCopyDlg.cpp : CFrameCopyDlg のインプリメンテーション
#include "stdafx.h"
#include "FrameCopyDlg.h"

#include <treehandler2.h>
#include <shdhandler.h>
#include <shdelem.h>

#include "GetDlgParams.h"

//extern HINSTANCE g_resinstance;

/////////////////////////////////////////////////////////////////////////////
// CFrameCopyDlg

CFrameCopyDlg::CFrameCopyDlg( CTreeHandler2* lpth, CShdHandler* lpsh, HWND appwnd, int srcmaxframe )
{
	m_hImageList = 0;
	m_iImage = 0;
	m_iSelect = 0;

	m_TI = 0;
	m_selecteditem = 0;
	m_selectedno = 0;
	
	m_thandler = lpth;
	m_shandler = lpsh;

	m_apphwnd = appwnd;

	m_influencenum = 0;
	ZeroMemory( m_influencelist, sizeof( int ) * FRAMECOPYLISTLENG );

	m_ignorenum = 0;
	ZeroMemory( m_ignorelist, sizeof( int ) * FRAMECOPYLISTLENG );


	m_validelemlist = 0;
	m_invalidelemlist = 0;
	m_cplist = 0;

	m_ismulti = 0;
	m_maxframe = srcmaxframe;
	m_startframe = 0;
	m_endframe = 0;

}

CFrameCopyDlg::~CFrameCopyDlg()
{
	ImageList_Destroy( m_hImageList );
	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}
	
	if( m_validelemlist ){
		free( m_validelemlist );
		m_validelemlist = 0;
	}

	if( m_invalidelemlist ){
		free( m_invalidelemlist );
		m_invalidelemlist = 0;
	}

	if( m_cplist ){
		free( m_cplist );
		m_cplist = 0;
	}

}
	
LRESULT CFrameCopyDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	//InitCommonControls();
	m_dlg_wnd = m_hWnd;
	m_tree_wnd = GetDlgItem( IDC_TREE1 );
	m_list_wnd = GetDlgItem( IDC_LIST1 );
	m_list2_wnd = GetDlgItem( IDC_LIST2 );
	m_start_wnd = GetDlgItem( IDC_STARTFRAME );
	m_end_wnd = GetDlgItem( IDC_ENDFRAME );
	m_ismulti_wnd = GetDlgItem( IDC_MULTIFRAME );

	CreateImageList();

	ret = FillTree();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = ParamsToDlg();
	_ASSERT( !ret );


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CFrameCopyDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//外部クラスから、wNotifyCode = 999 で呼び出されることがある。（ダイアログは表示されていない状態）


	if( !m_validelemlist || !m_invalidelemlist || !m_cplist ){
		DbgOut( "FrameCopyDlg : OnOK : list not allocated error !!!\n" );
		_ASSERT( 0 );

		if( wNotifyCode != 999 )
			EndDialog(wID);
		return 1;
	}
		
	ZeroMemory( m_validelemlist, sizeof( int ) * m_shandler->s2shd_leng );
	ZeroMemory( m_invalidelemlist, sizeof( int ) * m_shandler->s2shd_leng );
	ZeroMemory( m_cplist, sizeof( int ) * m_shandler->s2shd_leng );

	int i, validno;
	for( i = 0; i < m_influencenum; i++ ){
		validno = m_influencelist[ i ];
		SetTree2ListReq( validno, m_validelemlist, 0 );
	}


	int j, invalidno;
	for( j = 0; j < m_ignorenum; j++ ){
		invalidno = m_ignorelist[ j ];
		SetTree2ListReq( invalidno, m_invalidelemlist, 0 );
	}

	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		if( (*(m_validelemlist + seri) == 1) && (*(m_invalidelemlist + seri) == 0) ){
			*( m_cplist + seri ) = 1;
		}else{
			*( m_cplist + seri ) = 0;
		}
	}

/////////
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_MULTIFRAME );
	if( ischecked == BST_CHECKED ){
		m_ismulti = 1;
	}else{
		m_ismulti = 0;
	}

	int ret;
	int tmpint;
	ret = GetInt( m_start_wnd, &tmpint );
	if( !ret ){
		m_startframe = tmpint;
	}
	ret = GetInt( m_end_wnd, &tmpint );
	if( !ret ){
		m_endframe = tmpint;
	}

	if( m_startframe < 0 ){
		m_startframe = 0;
	}
	if( m_startframe > m_maxframe ){
		m_startframe = m_maxframe;
	}
	if( m_endframe < m_startframe ){
		m_endframe = m_startframe;
	}
	if( m_endframe > m_maxframe ){
		m_endframe = m_maxframe;
	}

	if( wNotifyCode != 999 )
		EndDialog(wID);

	return 0;
}

LRESULT CFrameCopyDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CFrameCopyDlg::FillTree()
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

	m_selectedno = 0;
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
void CFrameCopyDlg::AddBoneToTree( int srcseri, int addbroflag, int addtolast )
{
	//addtolastは初回のTVAddのみを制御する。
	//( CShdElemのTree構造と同期するため。)

	char* shdname;
	int parentno;
	CShdElem* curselem;
	//CShdElem* parselem;
	HTREEITEM parTI;
	//static int s_isfirst = 1;

	curselem = (*m_shandler)( srcseri );
		
	// joint だけTreeにAddする。
	if( curselem->IsJoint() && (curselem->type != SHDMORPH)){
		CPart* partptr = curselem->part;
		_ASSERT( partptr );
		//int bonenum = partptr->bonenum;
		//if( bonenum ){
			//CBoneInfo* biptr = *(partptr->ppBI);
			//_ASSERT( biptr );
			//parentno = biptr->parentjoint;
			//if( parentno == -1 )
			//	parentno = 0;

			CShdElem* parelem;
			parelem = m_shandler->FindUpperJoint( curselem );
			if( parelem ){
				parentno = parelem->serialno;
			}else{
				parentno = 0;
			}

			parTI = *(m_TI + parentno);
			//_ASSERT( parTI );

			shdname = (*m_thandler)( srcseri )->name;

			HTREEITEM newTI;
			newTI = TVAdd( parTI, shdname, srcseri, m_iImage, m_iSelect, addtolast );
			if( !newTI ){
				DbgOut( "FrameCopyDlg : AddBoneToTree : TVAdd error %d %s!!!\n", srcseri, shdname );
				_ASSERT( 0 );
				return;
			}
			*(m_TI + srcseri) = newTI;
			if( m_selectedno == 0 ){
				m_selecteditem = newTI;
				m_selectedno = srcseri;

				TreeView_SelectItem( m_tree_wnd, m_selecteditem );
			}
		//}
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
HTREEITEM CFrameCopyDlg::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{
	//ここのDbg文は取り除かない。
	DbgOut( "FrameCopyDlg : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
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

void CFrameCopyDlg::CreateImageList()
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
		DbgOut( "FrameCopyDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iImage = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iImage == -1 ){
		DbgOut( "FrameCopyDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "FrameCopyDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iSelect = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iSelect == -1 ){
		DbgOut( "FrameCopyDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_tree_wnd,
		m_hImageList, TVSIL_NORMAL );

	
}
int CFrameCopyDlg::ParamsToDlg()
{
	m_list_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	
	int listno;
	for( listno = 0; listno < m_influencenum; listno++ ){
		//char* name = (*m_thandler)( m_influencelist[listno] )->name;
		LRESULT lres;

		char* addname = (*m_thandler)( m_influencelist[listno] )->name;
		char addstr[2048];
		ZeroMemory( addstr, 2048 );
		sprintf_s( addstr, 2048, "%d : ", m_influencelist[listno] );
		int nameleng;
		nameleng = (int)strlen( addname );
		if( nameleng <= 2000 ){
			strcat_s( addstr, 2048, addname );
		}else{
			strncat_s( addstr, 2048, addname, 2000 );
		}

		lres = m_list_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)addstr );
		if( (lres == LB_ERR) || (lres == LB_ERRSPACE) ){
			_ASSERT( 0 );
			return 1;
		}
	}

	///////////////
	
	m_list2_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	
	int listno2;
	for( listno2 = 0; listno2 < m_ignorenum; listno2++ ){
		//char* name = (*m_thandler)( m_ignorelist[listno2] )->name;
		LRESULT lres;

		char* addname = (*m_thandler)( m_ignorelist[listno2] )->name;
		char addstr[2048];
		ZeroMemory( addstr, 2048 );
		sprintf_s( addstr, 2048, "%d : ", m_ignorelist[listno2] );
		int nameleng;
		nameleng = (int)strlen( addname );
		if( nameleng <= 2000 ){
			strcat_s( addstr, 2048, addname );
		}else{
			strncat_s( addstr, 2048, addname, 2000 );
		}

		lres = m_list2_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)addstr );
		if( (lres == LB_ERR) || (lres == LB_ERRSPACE) ){
			_ASSERT( 0 );
			return 1;
		}
	}


	char strframe[256];
	sprintf_s( strframe, 256, "%d", m_startframe );
	m_start_wnd.SetWindowTextA( strframe );

	sprintf_s( strframe, 256, "%d", m_endframe );
	m_end_wnd.SetWindowTextA( strframe );


	if( m_ismulti == 1 ){
		m_dlg_wnd.CheckDlgButton( IDC_MULTIFRAME, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_MULTIFRAME, BST_UNCHECKED );
	}

	return 0;
}

LRESULT CFrameCopyDlg::OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;

	m_selecteditem = tvi.hItem;

	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !TreeView_GetItem( m_tree_wnd, &tvi ) )
		return 0;

	m_selectedno = (int)(tvi.lParam);

	return 0;
}

LRESULT CFrameCopyDlg::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_selectedno == 0 )
		return 0;

	if( m_influencenum >= FRAMECOPYLISTLENG ){
		MessageBox( "これ以上、指定できません。", "バッファ不足エラー", MB_OK );
		return 0;
	}

	CShdElem* selem = (*m_shandler)( m_selectedno );
	if( selem->notuse ){
		MessageBox( 
			"このボーンは、無効になっています。\n表示パラメータ設定で、有効にしてから再試行して下さい。",
			"選択エラー",
			MB_OK );
		return 0;
	}

	int listno;
	int findflag = 0;
	for( listno = 0; listno < m_influencenum; listno++ ){
		if( m_influencelist[listno] == m_selectedno ){
			findflag = 1;
			break;
		}
	}
	if( findflag == 1 )
		return 0;

	m_influencelist[m_influencenum] = m_selectedno;
	m_influencenum++;

	LRESULT lres;
	char* addname = (*m_thandler)( m_selectedno )->name;
	char addstr[2048];
	ZeroMemory( addstr, 2048 );
	sprintf_s( addstr, 2048, "%d : ", m_selectedno );
	int nameleng;
	nameleng = (int)strlen( addname );
	if( nameleng <= 2000 ){
		strcat_s( addstr, 2048, addname );
	}else{
		strncat_s( addstr, 2048, addname, 2000 );
	}

	lres = m_list_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)addstr );
	if( lres == LB_ERR ){
		_ASSERT( 0 );
		return -1;
	}

	return 0;
}

LRESULT CFrameCopyDlg::OnAdd2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_selectedno == 0 )
		return 0;

	if( m_ignorenum >= FRAMECOPYLISTLENG ){
		MessageBox( "これ以上、指定できません。", "バッファ不足エラー", MB_OK );
		return 0;
	}

	CShdElem* selem = (*m_shandler)( m_selectedno );
	if( selem->notuse ){
		MessageBox( 
			"このボーンは、無効になっています。\n表示パラメータ設定で、有効にしてから再試行して下さい。",
			"選択エラー",
			MB_OK );
		return 0;
	}

	int listno;
	int findflag = 0;
	for( listno = 0; listno < m_ignorenum; listno++ ){
		if( m_ignorelist[listno] == m_selectedno ){
			findflag = 1;
			break;
		}
	}
	if( findflag == 1 )
		return 0;

	m_ignorelist[m_ignorenum] = m_selectedno;
	m_ignorenum++;

	int ret;
	char* addname = (*m_thandler)( m_selectedno )->name;
	char addstr[2048];
	ZeroMemory( addstr, 2048 );
	sprintf_s( addstr, 2048, "%d : ", m_selectedno );
	int nameleng;
	nameleng = (int)strlen( addname );
	if( nameleng <= 2000 ){
		strcat_s( addstr, 2048, addname );
	}else{
		strncat_s( addstr, 2048, addname, 2000 );
	}

	ret = (int)m_list2_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)addstr );
	if( ret == LB_ERR ){
		_ASSERT( 0 );
		return -1;
	}

	return 0;
}

LRESULT CFrameCopyDlg::OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int selindex;
	selindex = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( selindex == LB_ERR ){
		//_ASSERT( 0 );
		//return -1;
		
		//何も選択していないときもLB_ERRが返る
		return 0;
	}
	if( (selindex < 0) || (selindex >= m_influencenum) ){
		_ASSERT( 0 );
		return -1;
	}

	int ret;
	ret = (int)m_list_wnd.SendMessage( LB_DELETESTRING, (WPARAM)selindex, 0 );
	if( ret == LB_ERR ){
		_ASSERT( 0 );
		return 0;
	}

	int listno;
	for( listno = selindex; listno < (m_influencenum - 1); listno++ ){
		m_influencelist[listno] = m_influencelist[listno + 1];
	}
	m_influencenum--;


	return 0;
}

LRESULT CFrameCopyDlg::OnDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int selindex;
	selindex = (int)m_list2_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( selindex == LB_ERR ){
		//_ASSERT( 0 );
		//return -1;
		
		//何も選択していないときもLB_ERRが返る
		return 0;
	}
	if( (selindex < 0) || (selindex >= m_ignorenum) ){
		_ASSERT( 0 );
		return -1;
	}

	int ret;
	ret = (int)m_list2_wnd.SendMessage( LB_DELETESTRING, (WPARAM)selindex, 0 );
	if( ret == LB_ERR ){
		_ASSERT( 0 );
		return 0;
	}

	int listno;
	for( listno = selindex; listno < (m_ignorenum - 1); listno++ ){
		m_ignorelist[listno] = m_ignorelist[listno + 1];
	}
	m_ignorenum--;


	return 0;
}

LRESULT CFrameCopyDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_list_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	m_influencenum = 0;
	ZeroMemory( m_influencelist, sizeof( int ) * FRAMECOPYLISTLENG );

	return 0;
}

LRESULT CFrameCopyDlg::OnAllDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_list2_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	m_ignorenum = 0;
	ZeroMemory( m_ignorelist, sizeof( int ) * FRAMECOPYLISTLENG );

	return 0;
}

int CFrameCopyDlg::CreateLists()
{
	if( m_validelemlist || m_invalidelemlist || m_cplist ){
		DbgOut( "FrameCopyDlg : CreateLists : list already allocated error !!!\n" );
		_ASSERT( 0 );
		return 0;
	}

	m_validelemlist = (int*)malloc( sizeof( int ) * m_shandler->s2shd_leng );
	if( !m_validelemlist ){
		DbgOut( "FrameCopyDlg : CreateList : validelemlist alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( m_validelemlist, sizeof( int ) * m_shandler->s2shd_leng );

	m_invalidelemlist = (int*)malloc( sizeof( int ) * m_shandler->s2shd_leng );
	if( !m_invalidelemlist ){
		DbgOut( "FrameCopyDlg : CreateList : invalidelemlist alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( m_invalidelemlist, sizeof( int ) * m_shandler->s2shd_leng );


	m_cplist = (int*)malloc( sizeof( int ) * m_shandler->s2shd_leng );
	if( !m_cplist ){
		DbgOut( "FrameCopyDlg : CreateList : cplist alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( m_cplist, sizeof( int ) * m_shandler->s2shd_leng );

	return 0;
}

int CFrameCopyDlg::SetTree2ListReq( int srcno, int* dstlist, int addbroflag )
{
	CShdElem* curselem = 0;

	if( (srcno >= 0) && (srcno < m_shandler->s2shd_leng) ){
	
		curselem = (*m_shandler)( srcno );

		if(curselem->IsJoint() && (curselem->type != SHDMORPH)){
			*( dstlist + srcno ) = 1;
		}

	}else{
		DbgOut( "FrameCopyDlg : SetTree2ListReq : serialno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	_ASSERT( curselem );


	CShdElem* chilelem;
	chilelem = curselem->child;
	if( chilelem ){
		SetTree2ListReq( chilelem->serialno, dstlist, 1 );
	}

	if( addbroflag == 1 ){
		CShdElem* broelem;
		broelem = curselem->brother;
		if( broelem ){
			SetTree2ListReq( broelem->serialno, dstlist, 1 );
		}
	}

	return 0;
}


