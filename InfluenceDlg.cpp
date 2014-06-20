// InfluenceDlg.cpp : CInfluenceDlg のインプリメンテーション
#include "stdafx.h"
#include "InfluenceDlg.h"

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
// CInfluenceDlg

CInfluenceDlg::CInfluenceDlg( CTreeHandler2* lpth, CShdHandler* lpsh, HWND appwnd )
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
	ZeroMemory( m_influencelist, sizeof( int ) * INFLUENCELISTLENG );

	m_ignorenum = 0;
	ZeroMemory( m_ignorelist, sizeof( int ) * INFLUENCELISTLENG );

	//m_serialno = serialno;
	m_serialno = 0;

	//strcpy_s( m_dispname, _MAX_PATH, "表示オブジェクト名を選択してください。" );
}

CInfluenceDlg::~CInfluenceDlg()
{
	TreeView_DeleteAllItems( m_tree_wnd );

	ImageList_Destroy( m_hImageList );
	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}
	
}
	
LRESULT CInfluenceDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	//InitCommonControls();

	HWND temphwnd;
	temphwnd = GetDlgItem( IDC_TREE1 );
	m_tree_wnd = temphwnd;
	s_tree_wnd = m_tree_wnd;

	temphwnd = GetDlgItem( IDC_LIST1 );
	m_list_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_LIST2 );
	m_list2_wnd = temphwnd;


	m_list3_wnd = GetDlgItem( IDC_LIST3 );
	//m_dispname_wnd = GetDlgItem( IDC_DISPNAME );

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

int CInfluenceDlg::SetNewWindowProc()
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


LRESULT CInfluenceDlg::OnApply()
{

	if( m_serialno <= 0 ){
		return 0;
	}


	CShdElem* selem = (*m_shandler)( m_serialno );
	_ASSERT( selem );

	if( selem->influencebonenum != m_influencenum ){
		selem->influencebone = (int*)realloc( selem->influencebone, sizeof( int ) * m_influencenum );
		if( (m_influencenum != 0) && !(selem->influencebone) ){
			_ASSERT( 0 );
			return -1;
		}
		selem->influencebonenum = m_influencenum;
	}
	if( m_influencenum > 0 )
		MoveMemory( selem->influencebone, m_influencelist, sizeof( int ) * m_influencenum );
	///////

	if( selem->ignorebonenum != m_ignorenum ){
		selem->ignorebone = (int*)realloc( selem->ignorebone, sizeof( int ) * m_ignorenum );
		if( (m_ignorenum != 0) && !(selem->ignorebone) ){
			_ASSERT( 0 );
			return -1;
		}
		selem->ignorebonenum = m_ignorenum;
	}
	if( m_ignorenum > 0 )
		MoveMemory( selem->ignorebone, m_ignorelist, sizeof( int ) * m_ignorenum );



	::SendMessage( m_apphwnd, WM_USER_INFLUENCE, (WPARAM)m_serialno, 0 );

	//DbgOut( "InfluenceDlg : OnOk : m_influencenum %d\n", m_influencenum );

	//EndDialog(wID);
	return 0;
}

LRESULT CInfluenceDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CInfluenceDlg::FillTree()
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
void CInfluenceDlg::AddBoneToTree( int srcseri, int addbroflag, int addtolast )
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
		//int bonenum = partptr->bonenum;
		//if( bonenum ){
			//CBoneInfo* biptr = *(partptr->ppBI);
			//_ASSERT( biptr );
			//parentno = biptr->parentjoint;
			//if( parentno == -1 )
			//	parentno = 0;
			//parTI = *(m_TI + parentno);
			////_ASSERT( parTI );

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
				DbgOut( "InfluenceDlg : AddBoneToTree : TVAdd error %d %s!!!\n", srcseri, shdname );
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
HTREEITEM CInfluenceDlg::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{
	//ここのDbg文は取り除かない。
	DbgOut( "InfluenceDlg : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
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

void CInfluenceDlg::CreateImageList()
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
		DbgOut( "InfluenceDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iImage = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iImage == -1 ){
		DbgOut( "InfluenceDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "InfluenceDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iSelect = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iSelect == -1 ){
		DbgOut( "InfluenceDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_tree_wnd,
		m_hImageList, TVSIL_NORMAL );

	
}
int CInfluenceDlg::ParamsToDlg()
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

	////////////////

	//m_dispname_wnd.SetWindowText( m_dispname );


	return 0;
}

LRESULT CInfluenceDlg::OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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

LRESULT CInfluenceDlg::OnSelDisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int selindex;
	selindex = (int)m_list3_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		return 0;
	}

	m_serialno = (int)m_list3_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );

	if( m_serialno < 0 ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	ret = InitListBox();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}



LRESULT CInfluenceDlg::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_selectedno == 0 )
		return 0;

	if( m_serialno <= 0 ){
		::MessageBox( m_hWnd, "表示オブジェクトを選んでから、再試行してください。", "表示オブジェクトが先です", MB_OK );
		return 0;
	}

	if( m_influencenum >= INFLUENCELISTLENG ){
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

	int ret;
	ret = OnApply();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}


	return 0;
}

LRESULT CInfluenceDlg::OnAdd2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_selectedno == 0 )
		return 0;

	if( m_serialno <= 0 ){
		::MessageBox( m_hWnd, "表示オブジェクトを選んでから、再試行してください。", "表示オブジェクトが先です", MB_OK );
		return 0;
	}


	if( m_ignorenum >= INFLUENCELISTLENG ){
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

	ret = OnApply();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}


	return 0;
}

LRESULT CInfluenceDlg::OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

	ret = OnApply();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}

	return 0;
}

LRESULT CInfluenceDlg::OnDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

	ret = OnApply();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}

	return 0;
}

LRESULT CInfluenceDlg::OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_list_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	m_influencenum = 0;
	ZeroMemory( m_influencelist, sizeof( int ) * INFLUENCELISTLENG );


	int ret;
	ret = OnApply();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}

	return 0;
}

LRESULT CInfluenceDlg::OnAllDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_list2_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	m_ignorenum = 0;
	ZeroMemory( m_ignorelist, sizeof( int ) * INFLUENCELISTLENG );

	int ret;
	ret = OnApply();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}


	return 0;
}




int CInfluenceDlg::CreateList3()
{
	
	int seri;
	for( seri = 1; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );


		//if( (selem->IsJoint() == 0) && (selem->dispflag == 1) && (selem->notuse == 0) && 
		if( ((selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2)) && 
			( (selem->m_mtype == M_NONE) || (selem->m_mtype == M_BASE) ) ){

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

int CInfluenceDlg::InitListBox()
{
	m_influencenum = (*m_shandler)( m_serialno )->influencebonenum;
	if( m_influencenum > INFLUENCELISTLENG ){
		_ASSERT( 0 );
		DbgOut( "InfluenceDlg : OnInitDialog : influencebonenum too large !!!\n" );
		m_influencenum = INFLUENCELISTLENG;
	}
	if( m_influencenum > 0 ){
		int* srclist = (*m_shandler)( m_serialno )->influencebone;
		MoveMemory( m_influencelist, srclist, sizeof( int ) * m_influencenum );
	}
	

	m_ignorenum = (*m_shandler)( m_serialno )->ignorebonenum;
	if( m_influencenum > INFLUENCELISTLENG ){
		_ASSERT( 0 );
		DbgOut( "InfluenceDlg : OnInitDialog : ignorebonenum too large !!!\n" );
		m_ignorenum = INFLUENCELISTLENG;
	}
	if( m_ignorenum > 0 ){
		int* srclist = (*m_shandler)( m_serialno )->ignorebone;
		MoveMemory( m_ignorelist, srclist, sizeof( int ) * m_ignorenum );
	}

	ParamsToDlg();


	return 0;
}


