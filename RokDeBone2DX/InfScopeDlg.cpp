// InfScopeDlg.cpp : CInfScopeDlg のインプリメンテーション
#include "stdafx.h"
#include "InfScopeDlg.h"

#include "viewer.h"

#include <commctrl.h>

#include <mothandler.h>
#include <treehandler2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <quaternion.h>
#include <treeelem2.h>
#include <InfScope.h>

#include "inidata.h"

#include "MotDlg.h"
#include "ColDlg.h"


#define RENDERISTIMER	11004


enum {
	PMULT_BONE,
	PMULT_VERT,
	PMULT_HOR,
	PMULT_ALL,
	PMULT_MAX
};


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// extern
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern CMotDlg* g_motdlg;
extern CColDlg g_coldlg;
extern CIniData* g_inidata;


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// static
static int CALLBACK MyCompProc(LPARAM, LPARAM, LPARAM);
static HWND s_list2_wnd = NULL;

static LONG s_treefunc = 0;
static HWND s_tree_wnd = 0;


static LRESULT CALLBACK TreeProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  );

/////////////////////////////////////////////////////////////////////////////
// CInfScopeDlg

CInfScopeDlg::CInfScopeDlg( CMyD3DApplication* papp )
{
	m_timerid = 0;

	m_papp = papp;

	m_hImageList = 0;
	m_iImage = 0;
	m_iSelect = 0;

	m_TI = 0;
	m_selecteditem = 0;
	m_selbone = 0;
	
	//m_thandler = lpth;
	//m_shandler = lpsh;

	m_seldisp = 0;

	m_topflag = 0;

	m_cmdshow = SW_HIDE;
	m_isindex = -1;

	m_pointnum = 0;
	m_point = 0;

	m_col_dc = NULL;
	m_col_b = NULL;
	m_col = RGB( 0, 0, 255 );
	
	m_paramstodlg = 0;

	m_object = ITIOBJ_PART;

	m_worldpos.x = 0.0f;
	m_worldpos.y = 0.0f;
	m_worldpos.z = 0.0f;

	m_center.x = 0.0f;
	m_center.y = 0.0f;
	m_center.z = 0.0f;

	m_createlistflag = 0;

	m_savepolydisp = POLYDISP_ALL;
}

CInfScopeDlg::~CInfScopeDlg()
{
}

LRESULT CInfScopeDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	if( m_timerid != 0 ){
		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;
		//Sleep( 100 );//!!!!!!!!!!!!!!!!!!!
	}


	DestroyObjs();

	if( m_col_dc ){
		::ReleaseDC( m_col_wnd, m_col_dc );
		m_col_dc = NULL;
	}
	return 0;
}


int CInfScopeDlg::DestroyObjs()
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

	if( m_point ){
		delete [] m_point;
		m_point = 0;
	}
	m_pointnum = 0;

	return 0;
}

int CInfScopeDlg::InitTreeAndList()
{
	int ret;
	DestroyObjs();

	m_selbone = 0;
	m_seldisp = 0;
	m_isindex = -1;

/////////

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
//////////
	ListView_DeleteAllItems( m_list2_wnd );
	
	if( m_createlistflag == 0 ){
		LV_COLUMN lvcol;
		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 150;
		lvcol.pszText = "ボーン名";
		lvcol.iSubItem = 0;
		ListView_InsertColumn(m_list2_wnd, 0, &lvcol);

		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 150;
		lvcol.pszText = "表示obj名";
		lvcol.iSubItem = 1;
		ListView_InsertColumn(m_list2_wnd, 1, &lvcol);
	}
	m_createlistflag = 1;


	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( iselem ){
		int scno;


DbgOut( "infscopedlg : InitTreeAndList : scopenum %d\n", iselem->scopenum );

		for( scno = 0; scno < iselem->scopenum; scno++ ){
			ret = AddItem2List2( scno );
			if( ret ){
				DbgOut( "infscopedlg : InitTreeAndList : AddItem2List2 error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		//if( iselem->scopenum >= 2 ){
		//	ListView_SortItems(m_list2_wnd, MyCompProc, 0);
		//}

	}


	ret = ParamsToDlg();
	_ASSERT( !ret );

	return 0;
}

int CInfScopeDlg::AddItem2List2( int isindex )
{
	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		return 0;
	}

	if( (isindex < 0) || (isindex >= iselem->scopenum) ){
		DbgOut( "infscopedlg : AddItem2List2 : index error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	CInfScope* curis;
	curis = *( iselem->ppscope + isindex );
	_ASSERT( curis );

	int applyno, targetno;
	_ASSERT( curis->applychild );
	_ASSERT( curis->target );
	applyno = curis->applychild->serialno;
	targetno = curis->target->serialno;

	CTreeElem2* applyte;
	CTreeElem2* targette;

	applyte = (*m_papp->m_thandler)( applyno );
	_ASSERT( applyte );
	targette = (*m_papp->m_thandler)( targetno );
	_ASSERT( targette );

	LV_ITEM item;
	memset(&item, 0, sizeof(LV_ITEM));
	LV_ITEM item2;
	memset(&item2, 0, sizeof(LV_ITEM));

	item.mask = LVIF_TEXT;
	item.pszText = applyte->name;
	item.iItem = isindex;
	item.iSubItem = 0;
	ListView_InsertItem(m_list2_wnd, &item);

	item.pszText = targette->name;
	item.iItem = isindex;
	item.iSubItem = 1;
	ListView_SetItem(m_list2_wnd, &item);

	return 0;
			
}


int CALLBACK MyCompProc(LPARAM lp1, LPARAM lp2, LPARAM lp3)
{
    static LV_FINDINFO lvf;
    static int nItem1, nItem2;
    static char buf1[2048], buf2[2048];

    lvf.flags = LVFI_PARAM;
    lvf.lParam = lp1;
    nItem1 = ListView_FindItem(s_list2_wnd, -1, &lvf);

    lvf.lParam = lp2;
    nItem2 = ListView_FindItem(s_list2_wnd, -1, &lvf);
    
    ListView_GetItemText(s_list2_wnd, nItem1, (int)lp3, buf1, sizeof(buf1));
    ListView_GetItemText(s_list2_wnd, nItem2, (int)lp3, buf2, sizeof(buf2));
    //if (sortsubno[(int)lp3] == UP)
        return(strcmp(buf1, buf2));
    //else
    //    return(strcmp(buf1, buf2) * -1);
}



int CInfScopeDlg::ShowDlg( int cmdshow )
{
	int ret;
	m_cmdshow = cmdshow;
	ShowWindow( m_cmdshow );

	if( cmdshow != SW_HIDE ){
		m_savepolydisp = g_inidata->opt_polydisp;
		g_inidata->opt_polydisp = POLYDISP_SEL;//!!!!!!!!!!!!!!
		if( m_papp->m_shandler && (m_papp->m_shandler->s2shd_leng > 2) ){
			ret = m_papp->m_shandler->SetIsSelected( 1, 1, 0 );
			_ASSERT( !ret );
		}

		InitTreeAndList();

		m_sl_bone_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );
		m_sl_vert_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );
		m_sl_hor_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );
		m_sl_all_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	}else{
		if( (m_papp->m_bActive) && m_papp->m_shandler && (m_papp->m_shandler->s2shd_leng > 1) ){
			m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );

			SendMessage( m_papp->m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)1 );
			SendMessage( m_papp->m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)1 );

			m_papp->m_shandler->SetDispFlag( 1 );//!!!!!!!
		}
		g_inidata->opt_polydisp = m_savepolydisp;


	}

	return 0;
}

LRESULT CInfScopeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//InitCommonControls();

	m_dlg_wnd = m_hWnd;

	m_tree_wnd = GetDlgItem( IDC_TREE1 );
	s_tree_wnd = m_tree_wnd;
	m_list_wnd = GetDlgItem( IDC_LIST1 );
	
	
	m_list2_wnd = GetDlgItem( IDC_LIST2 );
	s_list2_wnd = m_list2_wnd;
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle( m_list2_wnd );
    dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;// | LVS_EX_HEADERDRAGDROP;
    ListView_SetExtendedListViewStyle(m_list2_wnd, dwStyle);


	m_create_wnd = GetDlgItem( IDC_CREATE );
	m_delete_wnd = GetDlgItem( IDC_DELETE );

	m_sl_bone_wnd = GetDlgItem( IDC_SLIDER_BONE );
	m_sl_vert_wnd = GetDlgItem( IDC_SLIDER_VERT );
	m_sl_hor_wnd = GetDlgItem( IDC_SLIDER_HOR );
	m_sl_all_wnd = GetDlgItem( IDC_SLIDER_ALL );

	m_sl_bone_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_bone_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000 );
	m_sl_bone_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	m_sl_vert_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_vert_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000 );
	m_sl_vert_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	m_sl_hor_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_hor_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000 );
	m_sl_hor_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	m_sl_all_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_all_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000 );
	m_sl_all_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );


	m_col_wnd = GetDlgItem( IDC_COL );
	m_col_dc = ::GetDC( m_col_wnd );

	m_col_b = ::CreateSolidBrush( m_col );


	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( RENDERISTIMER, 100 );
		DbgOut( "InfScopeDlg : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == RENDERISTIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}
	
	int ret;
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



int CInfScopeDlg::SetNewWindowProc()
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

LRESULT CInfScopeDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_papp->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);
	return 0;
}

LRESULT CInfScopeDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_papp->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);
	return 0;
}

int CInfScopeDlg::FillTree()
{
	HTREEITEM hRoot = TreeView_GetRoot( m_tree_wnd );
	//m_selecteditem = hRoot;

	int shdnum;
	shdnum = m_papp->m_shandler->s2shd_leng;

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
		curelem = (*m_papp->m_shandler)( seri );
		_ASSERT( curelem );

		CShdElem* parelem;
		parelem = m_papp->m_shandler->FindUpperJoint( curelem );
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
void CInfScopeDlg::AddBoneToTree( int srcseri, int addbroflag, int addtolast )
{
	//addtolastは初回のTVAddのみを制御する。
	//( CShdElemのTree構造と同期するため。)

	char* shdname;
	CShdElem* curselem;
	//CShdElem* parselem;
	HTREEITEM parTI;
	//static int s_isfirst = 1;

	curselem = (*m_papp->m_shandler)( srcseri );
		
	// jointのものだけTreeにAddする。

	//!!!!! 親の無いジョイントはaddしない。 !!!!!!!
	
	if( curselem->IsJoint() && (curselem->type != SHDMORPH)){
		CPart* partptr = curselem->part;
		_ASSERT( partptr );

		//bonenum 0 もadd!!!!!

		if( *( m_topflag + srcseri ) == 0 ){
			CShdElem* parelem;
			parelem = m_papp->m_shandler->FindUpperJoint( curselem );
			if( parelem && (*(m_topflag + parelem->serialno) == 0) ){
				parTI = *( m_TI + parelem->serialno );
			}else{
				parTI = *m_TI;
			}

			shdname = (*m_papp->m_thandler)( srcseri )->name;

			HTREEITEM newTI;
			newTI = TVAdd( parTI, shdname, srcseri, m_iImage, m_iSelect, addtolast );
			if( !newTI ){
				DbgOut( "InfScopeDlg : AddBoneToTree : TVAdd error %d %s!!!\n", srcseri, shdname );
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
HTREEITEM CInfScopeDlg::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{
	//ここのDbg文は取り除かない。
	DbgOut( "InfScopeDlg : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
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

void CInfScopeDlg::CreateImageList()
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
		DbgOut( "InfScopeDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iImage = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iImage == -1 ){
		DbgOut( "InfScopeDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "InfScopeDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iSelect = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iSelect == -1 ){
		DbgOut( "InfScopeDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_tree_wnd,
		m_hImageList, TVSIL_NORMAL );

	
}
int CInfScopeDlg::ParamsToDlg()
{

	if( m_object == ITIOBJ_PART ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_MVALL, IDC_RADIO_MVVERT, IDC_RADIO_MVALL );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_MVALL, IDC_RADIO_MVVERT, IDC_RADIO_MVVERT );
	}


	if( m_isindex >= 0 ){
		if( (m_selbone > 0) && (m_seldisp > 0) ){
			m_create_wnd.EnableWindow( FALSE );
			m_delete_wnd.EnableWindow( TRUE );
		}else{
			m_create_wnd.EnableWindow( FALSE );
			m_delete_wnd.EnableWindow( FALSE );
		}
	}else{
		if( (m_selbone > 0) && (m_seldisp > 0) ){
			m_create_wnd.EnableWindow( TRUE );
			m_delete_wnd.EnableWindow( FALSE );
		}else{
			m_create_wnd.EnableWindow( FALSE );
			m_delete_wnd.EnableWindow( FALSE );
		}
	}

	m_paramstodlg = 1;
	CheckItemInList2();
	m_paramstodlg = 0;

	return 0;
}

int CInfScopeDlg::CheckItemInList2()
{
	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		return 0;
	}

	int isindex;
	CInfScope* curis;

	int nCount, i;
	char bonename[256];
	char targetname[256];
	int cmp1, cmp2;

	for( isindex = 0; isindex < iselem->scopenum; isindex++ ){
		curis = *( iselem->ppscope + isindex );
		_ASSERT( curis );

		int applyno, targetno;
		_ASSERT( curis->applychild );
		_ASSERT( curis->target );
		applyno = curis->applychild->serialno;
		targetno = curis->target->serialno;

		CTreeElem2* applyte;
		CTreeElem2* targette;

		applyte = (*m_papp->m_thandler)( applyno );
		_ASSERT( applyte );
		targette = (*m_papp->m_thandler)( targetno );
		_ASSERT( targette );


		nCount = ListView_GetItemCount( m_list2_wnd );
		for( i = 0; i < nCount; i++ ){
			ListView_GetItemText( m_list2_wnd, i, 0, bonename, 255 );
			ListView_GetItemText( m_list2_wnd, i, 1, targetname, 255 );

			cmp1 = strcmp( bonename, applyte->name );
			cmp2 = strcmp( targetname, targette->name );

			if( (cmp1 == 0) && (cmp2 == 0) ){
				ListView_SetCheckState( m_list2_wnd, i, curis->dispflag );
				break;
			}

		}
	}

	return 0;
}

int CInfScopeDlg::SetDispFlagFromList2()
{

	if( m_paramstodlg == 1 )
		return 0;//!!!!!!!!!!!!!!!!!!!!


	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	int nCount, i;
	char bonename[256];
	char targetname[256];

	int boneseri, targetseri, isindex;
	int dispflag;
	nCount = ListView_GetItemCount( m_list2_wnd );
	for( i = 0; i < nCount; i++ ){
		ListView_GetItemText( m_list2_wnd, i, 0, bonename, 255 );
		ListView_GetItemText( m_list2_wnd, i, 1, targetname, 255 );

		dispflag = ListView_GetCheckState( m_list2_wnd, i );

		ret = m_papp->m_thandler->GetBoneNoByName( bonename, &boneseri, m_papp->m_shandler, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		ret = m_papp->m_thandler->GetDispObjNoByName( targetname, &targetseri, m_papp->m_shandler, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		if( (boneseri >= 0) && (targetseri >= 0) ){
			isindex = m_papp->m_shandler->ExistInfScope( boneseri, targetseri );
			if( (isindex >= 0) && (isindex < iselem->scopenum) ){
				CInfScope* curis;
				curis = *( iselem->ppscope + isindex );
				_ASSERT( curis );
				curis->dispflag = dispflag;
			}
		}

	}

	return 0;
}



LRESULT CInfScopeDlg::OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;

	m_selecteditem = tvi.hItem;

	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !TreeView_GetItem( m_tree_wnd, &tvi ) ){
		m_isindex = -1;
		m_selbone = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 0;
	}

	m_selbone = (int)(tvi.lParam);
	if( (m_selbone <= 0) || (m_selbone >= m_papp->m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		m_isindex = -1;
		m_selbone = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 1;
	}

	if( m_selbone > 0 ){
		//!!!!!!!!!!!!!!!!!!!!!!!!!
		::SendMessage( m_papp->m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_INFSCOPEDLG, (LPARAM)m_selbone );
	}

/////////
	int selindex;
	selindex = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		m_isindex = -1;
		m_seldisp = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 0;
	}
	m_seldisp = (int)m_list_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );
	if( (m_seldisp <= 0) || (m_seldisp >= m_papp->m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		m_isindex = -1;
		m_seldisp = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 1;
	}

//////////

	int ret;
	m_isindex = m_papp->m_shandler->ExistInfScope( m_selbone, m_seldisp );
	if( m_isindex < 0 ){
		m_isindex = -1;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 0;
	}
		
	ret = m_papp->m_shandler->SetInfScopeDispFlag( m_isindex, 1, 1 );
	if( ret ){
		_ASSERT( 0 );
		m_isindex = -1;
		m_selbone = 0;
		m_seldisp = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 1;
	}

	ret = SetPoint();
	if( ret ){
		DbgOut( "infscopedlg : OnSelchangedTree1 : SetPont error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ParamsToDlg();

	return 0;
}

int CInfScopeDlg::OnUserSelChange( int selno )
{
	if( m_cmdshow == SW_HIDE )
		return 0;//!!!!!!!!!!!!!!!!


	HTREEITEM selitem;
	selitem = *(m_TI + selno );

	if( selitem ){
		m_selbone = selno;
		m_selecteditem = selitem;

		TreeView_SelectItem( m_tree_wnd,
			m_selecteditem );
		
		////////////////
		if( (m_selbone <= 0) || (m_selbone >= m_papp->m_shandler->s2shd_leng) ){
			_ASSERT( 0 );
			m_isindex = -1;
			m_selbone = 0;
			m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
			ParamsToDlg();
			return 0;
		}
	/////////
		int selindex;
		selindex = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
		if( (selindex < 0) || (selindex == LB_ERR) ){
			m_isindex = -1;
			m_seldisp = 0;
			m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
			ParamsToDlg();
			return 0;
		}
		m_seldisp = (int)m_list_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );
		if( (m_seldisp <= 0) || (m_seldisp >= m_papp->m_shandler->s2shd_leng) ){
			_ASSERT( 0 );
			m_isindex = -1;
			m_seldisp = 0;
			m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
			ParamsToDlg();
			return 0;
		}

	//////////

		int ret;
		m_isindex = m_papp->m_shandler->ExistInfScope( m_selbone, m_seldisp );
		if( m_isindex < 0 ){
			m_isindex = -1;
			m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
			ParamsToDlg();
			//_ASSERT( 0 );
			//return 1;
			return 0;
		}
			
		ret = m_papp->m_shandler->SetInfScopeDispFlag( m_isindex, 1, 1 );
		if( ret ){
			_ASSERT( 0 );
			m_isindex = -1;
			m_selbone = 0;
			m_seldisp = 0;
			m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
			ParamsToDlg();
			_ASSERT( 0 );
			return 1;
		}

		ret = SetPoint();
		if( ret ){
			DbgOut( "infscopedlg : OnUserSelChange : SetPont error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ParamsToDlg();

		::UpdateWindow( m_tree_wnd );
	}
	return 0;
}



LRESULT CInfScopeDlg::OnSelchangeList1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( wNotifyCode != LBN_SELCHANGE )
		return 0;

	int selindex;
	selindex = (int)m_list_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( (selindex < 0) || (selindex == LB_ERR) ){
		m_isindex = -1;
		m_seldisp = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 0;
	}
	m_seldisp = (int)m_list_wnd.SendMessage( LB_GETITEMDATA, selindex, 0 );
	if( (m_seldisp <= 0) || (m_seldisp >= m_papp->m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		m_isindex = -1;
		m_seldisp = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 1;
	}else{
		ret = m_papp->m_shandler->SetIsSelected( m_seldisp, 1, 0 );
		_ASSERT( !ret );

		m_papp->m_shandler->SetDispFlag( 1 );//!!!!!!!
		(*m_papp->m_shandler)( m_seldisp )->dispflag = 1;//!!!!!!!!!!!
	}
////////////

	m_isindex = m_papp->m_shandler->ExistInfScope( m_selbone, m_seldisp );
	if( m_isindex < 0 ){
		m_isindex = -1;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 0;
	}
		
	ret = m_papp->m_shandler->SetInfScopeDispFlag( m_isindex, 1, 1 );
	if( ret ){
		_ASSERT( 0 );
		m_isindex = -1;
		m_selbone = 0;
		m_seldisp = 0;
		m_papp->m_shandler->SetInfScopeDispFlag( -1, 0, 0 );
		ParamsToDlg();
		return 1;
	}

	ret = SetPoint();
	if( ret ){
		DbgOut( "infscopedlg : OnSelchangedList1 : SetPont error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ParamsToDlg();
	
	return 0;
}


int CInfScopeDlg::CreateList()
{
	
	m_list_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );

	int seri;
	for( seri = 1; seri < m_papp->m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_papp->m_shandler)( seri );
		_ASSERT( selem );


		if( ((selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2)) &&
			((selem->m_mtype == M_NONE) || (selem->m_mtype == M_BASE)) ){

			CTreeElem2* telem;
			telem = (*m_papp->m_thandler)( seri );
			_ASSERT( telem );


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

	return 0;
}

LRESULT CInfScopeDlg::OnCreateMesh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	if( (m_selbone <= 0) || (m_seldisp <= 0) ){
		_ASSERT( 0 );
		return 0;
	}

	m_papp->m_shiftpart = 0;
	m_papp->m_shifvertnum = 0;//!!!!!!!!! マニピュレータを消す


	//存在したら、削除
	int chkindex;
	chkindex = m_papp->m_shandler->ExistInfScope( m_selbone, m_seldisp );
	if( chkindex >= 0 ){
		CShdElem* iselem;
		iselem = m_papp->m_shandler->GetInfScopeElem();
		_ASSERT( iselem );

		ret = iselem->DeleteInfScopeElemByIndex( chkindex );
		if( ret ){
			DbgOut( "infscopedlg : OnCreateMesh : is DeleteInfScopeElemByIndex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	ret = m_papp->m_shandler->CreateInfScopePM2( m_selbone, m_seldisp );
	if( ret ){
		DbgOut( "infscopedlg : OnCreateMesh : sh CreateInfScopePM2 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_papp->RestoreHandler( 1 );
	if( ret ){
		DbgOut( "infscopedlg : OnCreateMesh : app RestoreHandler error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_seldisp > 0 ){
		(*m_papp->m_shandler)( m_seldisp )->dispflag = 1;//!!!!!!!!!!!
	}

	ret = m_papp->m_shandler->CalcInfElem( -1, 1 );
	if( ret ){
		DbgOut( "infscopedlg : OnCreateMesh : sh CalcInfElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}



////////


	m_isindex = m_papp->m_shandler->ExistInfScope( m_selbone, m_seldisp );
	if( m_isindex >= 0 ){
		ret = m_papp->m_shandler->SetInfScopeDispFlag( m_isindex, 1, 1 );
		_ASSERT( !ret );


		CShdElem* iselem;
		iselem = m_papp->m_shandler->GetInfScopeElem();
		if( iselem ){
			ret = iselem->SetInfScopeDiffuseAmbient( m_isindex, m_col );
			if( ret ){
				DbgOut( "infscopedlg : OnCreate : is SetInfScopeDiffuse error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = AddItem2List2( m_isindex );
		_ASSERT( !ret );

	}else{
		_ASSERT( 0 );
	}

/////////

	ret = SetPoint();
	if( ret ){
		DbgOut( "infscopedlg : OnCreateMesh : SetPont error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ParamsToDlg();

	return 0;
}

LRESULT CInfScopeDlg::OnDeleteMesh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	
	if( (m_isindex < 0) || (m_selbone <= 0) || (m_seldisp <= 0) ){
		_ASSERT( 0 );
		return 0;
	}

	m_papp->m_shiftpart = 0;
	m_papp->m_shifvertnum = 0;//!!!!!!!!! マニピュレータを消す


	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		m_isindex = -1;
		ParamsToDlg();

		return 0;
	}

///////////	
	DeleteItemFromList2( m_isindex );


///////////
	int ret;
	ret = iselem->DeleteInfScopeElemByIndex( m_isindex );
	if( ret ){
		DbgOut( "infscopdlg : is DeleteInfScopeElemByIndex error !!!\n" );
		_ASSERT( 0 );

		m_isindex = -1;
		ParamsToDlg();
		return 1;
	}

	m_isindex = -1;
	ParamsToDlg();

	return 0;
}

int CInfScopeDlg::DeleteItemFromList2( int isindex )
{
	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		return 0;
	}

	if( (isindex < 0) || (isindex >= iselem->scopenum) ){
		_ASSERT( 0 );
		return 1;
	}

	CInfScope* curis;
	curis = *( iselem->ppscope + isindex );
	_ASSERT( curis );

	int applyno, targetno;
	_ASSERT( curis->applychild );
	_ASSERT( curis->target );
	applyno = curis->applychild->serialno;
	targetno = curis->target->serialno;

	CTreeElem2* applyte;
	CTreeElem2* targette;

	applyte = (*m_papp->m_thandler)( applyno );
	_ASSERT( applyte );
	targette = (*m_papp->m_thandler)( targetno );
	_ASSERT( targette );
	
	int nCount, i;
	char bonename[256];
	char targetname[256];
	int cmp1, cmp2;
	nCount = ListView_GetItemCount( m_list2_wnd );
    for( i = 0; i < nCount; i++ ){
		ListView_GetItemText( m_list2_wnd, i, 0, bonename, 255 );
		ListView_GetItemText( m_list2_wnd, i, 1, targetname, 255 );

		cmp1 = strcmp( bonename, applyte->name );
		cmp2 = strcmp( targetname, targette->name );

		if( (cmp1 == 0) && (cmp2 == 0) ){
			ListView_DeleteItem( m_list2_wnd, i );
			break;
		}

	}

	return 0;
}


LRESULT CInfScopeDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static int savecurpos = 0;


	HWND scwnd = (HWND)lParam;
	WORD event;

	event = LOWORD( wParam );

	int curpos, kind;
	float mult;
	int ret;
	if( (event = SB_THUMBTRACK) || (event == SB_THUMBPOSITION ) ){

		if( scwnd == (HWND)m_sl_bone_wnd ){
			curpos = (int)m_sl_bone_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			kind = PMULT_BONE;
		}else if( scwnd == (HWND)m_sl_vert_wnd ){
			curpos = (int)m_sl_vert_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			kind = PMULT_VERT;
		}else if( scwnd == (HWND)m_sl_hor_wnd ){
			curpos = (int)m_sl_hor_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			kind = PMULT_HOR;
		}else if( scwnd == (HWND)m_sl_all_wnd ){
			curpos = (int)m_sl_all_wnd.SendMessage( TBM_GETPOS, 0, 0 );
			kind = PMULT_ALL;
		}else{
			return 0;//!!!!!!!!!!!!
		}

		if( curpos == savecurpos ){
			return 0;//!!!!!!!!!!!!!!!!!!!!!
		}else{
			savecurpos = curpos;
		}


		if( curpos < 500 ){
			mult = 0.25f + 0.75f * ( (float)curpos / 500.0f );
			if( mult < 0.25f )
				mult = 0.25f;
			else if( mult > 4.0f )
				mult = 4.0f;

			ret = MultPoint( mult, kind );
			if( ret ){
				DbgOut( "infscopedlg : OnHScroll : MultPoint all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else if( curpos > 500 ){
			mult = 1.0f + 3.0f * ( (float)(curpos - 500) / 500.0f );
			if( mult < 0.25f )
				mult = 0.25f;
			else if( mult > 4.0f )
				mult = 4.0f;

			ret = MultPoint( mult, kind );
			if( ret ){
				DbgOut( "infscopedlg : OnHScroll : MultPoint error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else{
			//何もしない
		}

	}

	return 0;
}
LRESULT CInfScopeDlg::OnReleasedcaptureSl_bone(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{

	m_sl_bone_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	SetPoint();

	int ret;
	ret = m_papp->m_shandler->CalcInfElem( -1, 1 );
	if( ret ){
		DbgOut( "infscopedlg : OnReleasedcapter bone : sh CalcInfElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CInfScopeDlg::OnReleasedcaptureSl_vert(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	m_sl_vert_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	SetPoint();

	int ret;
	ret = m_papp->m_shandler->CalcInfElem( -1, 1 );
	if( ret ){
		DbgOut( "infscopedlg : OnReleasedcapter vert : sh CalcInfElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CInfScopeDlg::OnReleasedcaptureSl_hor(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	m_sl_hor_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	SetPoint();

	int ret;
	ret = m_papp->m_shandler->CalcInfElem( -1, 1 );
	if( ret ){
		DbgOut( "infscopedlg : OnReleasedcapter hor : sh CalcInfElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CInfScopeDlg::OnReleasedcaptureSl_all(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	m_sl_all_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	SetPoint();
	int ret;
	ret = m_papp->m_shandler->CalcInfElem( -1, 1 );
	if( ret ){
		DbgOut( "infscopedlg : OnReleasedcapter all : sh CalcInfElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CInfScopeDlg::SetPoint()
{
	if( (m_isindex < 0) || (m_selbone <= 0) || (m_seldisp <= 0) ){
		return 0;
	}

	if( m_point ){
		delete [] m_point;
		m_point = 0;
	}
	m_pointnum = 0;


	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;

	ret = iselem->GetInfScopePointNum( m_isindex, &m_pointnum );
	if( ret ){
		DbgOut( "infscopedlg : SetPoint : is GetInfScopePointNum error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_pointnum <= 0 ){
		_ASSERT( 0 );
		return 0;
	}

	m_point = new D3DXVECTOR3[ m_pointnum ];
	if( !m_point ){
		DbgOut( "infscopedlg : SetPoint : point alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = iselem->GetInfScopePointBuf( m_isindex, m_point );
	if( ret ){
		DbgOut( "infscopedlg : SetPoint : is GetInfScopePointBuf error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
///////////////////

	ret = iselem->GetInfScopeCenter( m_isindex, &m_center );
	if( ret ){
		DbgOut( "infscopedlg : MultPoint : is GetInfScopeCenter error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


/////////////////// 色
	ret = iselem->GetInfScopeDiffuse( m_isindex, &m_col );
	if( ret ){
		DbgOut( "infscopedlg : SetPoint : is GetInfScopeDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_col_b )
		DeleteObject( m_col_b );
	m_col_b = ::CreateSolidBrush( m_col );


	return 0;
}

int CInfScopeDlg::MultPoint( float mult, int kind )
{
	if( (m_isindex < 0) || (m_selbone <= 0) || (m_seldisp <= 0) || (m_pointnum <= 0) || !m_point ){
		//_ASSERT( 0 );
		return 0;
	}

	m_papp->m_shiftpart = 0;
	m_papp->m_shifvertnum = 0;//!!!!!!!!! マニピュレータを消す

	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &m_papp->m_matWorld );
	D3DXVECTOR3 localcenter;
	D3DXVec3TransformCoord( &localcenter, &m_center, &invw );


	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	/***
	D3DXVECTOR3 center;
	ret = iselem->GetInfScopeCenter( m_isindex, &center );
	if( ret ){
		DbgOut( "infscopedlg : MultPoint : is GetInfScopeCenter error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/

	D3DXMATRIX befmat, aftmat, befrot, aftrot, scalemat, mat;
	D3DXMatrixIdentity( &befmat );
	D3DXMatrixIdentity( &aftmat );
	D3DXMatrixIdentity( &scalemat );
	D3DXMatrixIdentity( &mat );
	D3DXMatrixIdentity( &befrot );
	D3DXMatrixIdentity( &aftrot );


	D3DXVECTOR3 vecx, vecy, vecz;
	ret = m_papp->m_shandler->GetBone3Vec( m_selbone, &vecx, &vecy, &vecz );
	if( ret ){
		DbgOut( "infscopedlg : MultPoint : sh GetBone3Vec error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 basex( 1.0f, 0.0f, 0.0f );

	CQuaternion befq, aftq;


	int pno;
	float testdot;

	switch( kind ){
	case PMULT_BONE:
		testdot = D3DXVec3Dot( &vecz, &basex );
		if( testdot <= -0.999999 ){
			befq.SetAxisAndRot( vecx, (float)PAI );
		}else{
			befq.RotationArc( vecz, basex );
		}
		befq.inv( &aftq );
		befrot = befq.MakeRotMatX();
		aftrot = aftq.MakeRotMatX();

		befmat._41 = -localcenter.x;
		befmat._42 = -localcenter.y;
		befmat._43 = -localcenter.z;

		aftmat._41 = localcenter.x;
		aftmat._42 = localcenter.y;
		aftmat._43 = localcenter.z;

		scalemat._11 = mult;

		mat = befmat * befrot * scalemat * aftrot * aftmat;

		break;
	case PMULT_VERT:
		testdot = D3DXVec3Dot( &vecy, &basex );
		if( testdot <= -0.999999 ){
			befq.SetAxisAndRot( vecz, (float)PAI );
		}else{
			befq.RotationArc( vecy, basex );
		}
		befq.inv( &aftq );
		befrot = befq.MakeRotMatX();
		aftrot = aftq.MakeRotMatX();

		befmat._41 = -localcenter.x;
		befmat._42 = -localcenter.y;
		befmat._43 = -localcenter.z;

		aftmat._41 = localcenter.x;
		aftmat._42 = localcenter.y;
		aftmat._43 = localcenter.z;

		scalemat._11 = mult;

		mat = befmat * befrot * scalemat * aftrot * aftmat;

		break;
	case PMULT_HOR:

		testdot = D3DXVec3Dot( &vecx, &basex );
		if( testdot <= -0.999999 ){
			befq.SetAxisAndRot( vecy, (float)PAI );
		}else{
			befq.RotationArc( vecx, basex );
		}

		befq.inv( &aftq );
		befrot = befq.MakeRotMatX();
		aftrot = aftq.MakeRotMatX();

		befmat._41 = -localcenter.x;
		befmat._42 = -localcenter.y;
		befmat._43 = -localcenter.z;

		aftmat._41 = localcenter.x;
		aftmat._42 = localcenter.y;
		aftmat._43 = localcenter.z;

		scalemat._11 = mult;

		mat = befmat * befrot * scalemat * aftrot * aftmat;

		break;
	case PMULT_ALL:
		befmat._41 = -localcenter.x;
		befmat._42 = -localcenter.y;
		befmat._43 = -localcenter.z;

		aftmat._41 = localcenter.x;
		aftmat._42 = localcenter.y;
		aftmat._43 = localcenter.z;

		scalemat._11 = mult;
		scalemat._22 = mult;
		scalemat._33 = mult;

		mat = befmat * scalemat * aftmat;

		break;
	default:
		_ASSERT( 0 );
		return 1;
		break;
	}

	D3DXVECTOR3 newp;
	for( pno = 0; pno < m_pointnum; pno++ ){
	
		D3DXVec3TransformCoord( &newp, m_point + pno, &mat );

		ret = iselem->SetInfScopePointBuf( m_isindex, &newp, pno );
		if( ret ){
			DbgOut( "infscopedlg : MultPoint : SetPointBuf error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}

	return 0;
}

int CInfScopeDlg::PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush )
{
	if( !hdc )
		return 0;

	if( !brush )
		return 0;

	BOOL bret;
	RECT winrect;
	bret = srcwnd.GetWindowRect( &winrect );
	if( !bret )
		return 0;

	RECT clirect;
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

LRESULT CInfScopeDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//int ret;

	int timerid = (int)wParam;
	if( (timerid == m_timerid) && (m_cmdshow != SW_HIDE) ){
		PaintRect( m_col_wnd, m_col_dc, m_col_b );

		SetDispFlagFromList2();
	}

	return 0;
}

LRESULT CInfScopeDlg::OnRefCol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = g_coldlg.Choose( m_dlg_wnd, &m_col );

	if( m_col_b )
		DeleteObject( m_col_b );
	m_col_b = ::CreateSolidBrush( m_col );

	CShdElem* iselem;
	iselem = m_papp->m_shandler->GetInfScopeElem();
	if( !iselem ){
		_ASSERT( 0 );
		return 1;
	}

	if( (m_isindex < 0) || (m_selbone <= 0) || (m_seldisp <= 0) ){
		return 0;
	}

	ret = iselem->SetInfScopeDiffuseAmbient( m_isindex, m_col );
	if( ret ){
		DbgOut( "infscopedlg : OnRefCol : is SetInfScopeDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

LRESULT CInfScopeDlg::OnRadioMVALL(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_MVALL );
	if( checkflag == BST_CHECKED ){
		m_object = ITIOBJ_PART;
	}else if( checkflag == BST_UNCHECKED ){
		m_object = ITIOBJ_VERTEX;
	}

	return 0;
}
LRESULT CInfScopeDlg::OnRadioMVVERT(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_MVALL );
	if( checkflag == BST_CHECKED ){
		m_object = ITIOBJ_PART;
	}else if( checkflag == BST_UNCHECKED ){
		m_object = ITIOBJ_VERTEX;
	}

	return 0;
}

int CInfScopeDlg::GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir )
{

		//planepを通り、planedirを法線ベクトルとする平面：ax + by + cz + d = 0;
	float a, b, c, d;
	a = planedir.x; b = planedir.y; c = planedir.z;
	d = -a * planep.x - b * planep.y - c * planep.z;

		//平面ax+by+cz+d=0と、点pobj1を通り方向ベクトルdirectionの直線との交点、targetを求める。
		//OpenGL赤本p402参照
	D3DXMATRIX mat;
	mat._11 = b * srcdir.y + c * srcdir.z;
	mat._12 = -a * srcdir.y;
	mat._13 = -a * srcdir.z;
	mat._14 = 0.0f;

	mat._21 = -b * srcdir.x;
	mat._22 = a * srcdir.x + c * srcdir.z;
	mat._23 = -b * srcdir.z;
	mat._24 = 0.0f;

	mat._31 = -c * srcdir.x;
	mat._32 = -c * srcdir.y;
	mat._33 = a * srcdir.x + b * srcdir.y;
	mat._34 = 0.0f;

	mat._41 = -d * srcdir.x;
	mat._42 = -d * srcdir.y;
	mat._43 = -d * srcdir.z;
	mat._44 = a * srcdir.x + b * srcdir.y + c * srcdir.z;

	if( mat._44 == 0.0f )
		return 1;

	dstshadow->x = (srcp.x * mat._11 + srcp.y * mat._21 + srcp.z * mat._31 + mat._41) / mat._44;
	dstshadow->y = (srcp.x * mat._12 + srcp.y * mat._22 + srcp.z * mat._32 + mat._42) / mat._44;
	dstshadow->z = (srcp.x * mat._13 + srcp.y * mat._23 + srcp.z * mat._33 + mat._43) / mat._44;


	return 0;
}
int CInfScopeDlg::DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec )
{
	float mag;
	mag = srcvec->x * srcvec->x + srcvec->y * srcvec->y + srcvec->z * srcvec->z;

	float sqmag;
	sqmag = (float)sqrt( mag );

	if( sqmag != 0.0f ){
		float magdiv;
		magdiv = 1.0f / sqmag;
//		dstvec->x *= magdiv;
//		dstvec->y *= magdiv;
//		dstvec->z *= magdiv;
		dstvec->x = srcvec->x * magdiv;
		dstvec->y = srcvec->y * magdiv;
		dstvec->z = srcvec->z * magdiv;

	}else{
		//DbgOut( "viewer : DXVec3Normalize : zero warning !!!\n" );
		dstvec->x = 0.0f;
		dstvec->y = 0.0f;
		dstvec->z = 0.0f;
		_ASSERT( 0 );
	}

	return 0;
}

int CInfScopeDlg::MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftface, int* shifvert, int shifvertnum, float pointx, float pointy )
{
	int ret;

	switch( m_object ){
	case ITIOBJ_VERTEX:
		if( shifvertnum > 0 ){
			ret = MoveVertexScreen( papp, lpsh, lpmh, shifvert, shifvertnum, pointx, pointy );
			if( ret ){
				DbgOut( "infscopedlg : MoveObject : MoveVertexScreen error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
		break;
	case ITIOBJ_PART:
		if( shiftface >= 0 ){
			ret = MovePartScreen( papp, lpsh, lpmh, pointx, pointy );
			if( ret ){
				DbgOut( "infscopedlg : MoveObject : MovePartScreen error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	return 0;
}
int CInfScopeDlg::MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftface, int* shifvert, int shifvertnum, int kind, float delta )
{
	int ret;

	switch( m_object ){
	case ITIOBJ_VERTEX:
		if( shifvertnum > 0 ){
			ret = MoveVertexDelta( papp, lpsh, lpmh, shifvert, shifvertnum, kind, delta );
			if( ret ){
				DbgOut( "infscopedlg : MoveObject : MoveVertexDelta error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
		break;
	case ITIOBJ_PART:
		if( shiftface >= 0 ){
			ret = MovePartDelta( papp, lpsh, lpmh, kind, delta );
			if( ret ){
				DbgOut( "infscopedlg : MoveObject : MovePartDelta error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	return 0;
}

int CInfScopeDlg::MoveVertexScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int* shifvert, int shifvertnum, float srcx, float srcy )
{
	if( m_isindex < 0 ){
		return 0;//!!!!!!!!
	}
	if( shifvertnum <= 0 )
		return 0;


	int ret;
	D3DXVECTOR3 mvobj;
	int motid, frameno;
	motid = g_motdlg->GetMotCookie();
	frameno = g_motdlg->GetCurrentFrameNo();

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );


	ret = lpsh->GetVertPos2InfScope( m_isindex, lpmh, inimat, motid, frameno, shifvert[0], &mvobj, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveVertex : GetVertPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	
	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 0.0f;
	ret = papp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 1.0f;
	ret = papp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = papp->m_degxz * (float)DEG2PAI;
	rady = papp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 temptarget;
	ret = GetShadow( &temptarget, planeaxis, mvobj, targetobj0, targetdir );
	if( ret )
		return 0;

	//targetobj = mvobj + 0.333f * ( temptarget - mvobj );
	targetobj = temptarget;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int shiftno;
	for( shiftno = 0; shiftno < shifvertnum; shiftno++ ){
		ret = lpsh->SetVertPos2InfScope( m_isindex, lpmh, motid, frameno, shifvert[ shiftno ], targetobj );
		if( ret ){
			DbgOut( "itidlg : MoveVertex : lpsh MoveVertex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	D3DXVec3TransformCoord( &m_worldpos, &targetobj, &(papp->m_matWorld) );

	return 0;
}

int CInfScopeDlg::MovePartScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, float srcx, float srcy )
{
	if( m_isindex < 0 )
		return 0;


	int ret;
	D3DXVECTOR3 partpos;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );


	ret = lpsh->GetInitialInfScopeCenter( m_isindex, &partpos, inimat );
	if( ret ){
		DbgOut( "itidlg : MovePartScreen : shandler GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	
	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 0.0f;
	ret = papp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 1.0f;
	ret = papp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = papp->m_degxz * (float)DEG2PAI;
	rady = papp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 temptarget;
	ret = GetShadow( &temptarget, planeaxis, partpos, targetobj0, targetdir );
	if( ret )
		return 0;

	//targetobj = mvobj + 0.333f * ( temptarget - mvobj );
	targetobj = temptarget;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	D3DXVECTOR3 diff;
	diff = targetobj - partpos;

	D3DXVECTOR3 worlddiff;
	D3DXVec3TransformCoord( &worlddiff, &diff, &(papp->m_matWorld) );


	int vertnum = 0;
	ret = lpsh->GetVertNumOfPartInfScope( m_isindex, &vertnum );
	if( ret ){
		DbgOut( "itidlg : MovePartScreen : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int vertno;
	for( vertno = 0; vertno < vertnum; vertno++ ){
		ret = MoveVertexRelative( papp, lpsh, lpmh, &vertno, 1, worlddiff );
		if( ret ){
			DbgOut( "itidlg : MovePartScreen : MoveVertexRelative %d error !!!\n", vertno );
			_ASSERT( 0 );
			return 1;
		}
	}

////////
	ret = lpsh->GetInitialInfScopeCenter( m_isindex, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MovePartScreen : sh GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}



//DbgOut( "itidlg : MovePartScreen : vertnum %d, partpos %f %f %f, worldpos %f %f %f\r\n",
//	vertnum, partpos.x, partpos.y, partpos.z, m_worldpos.x, m_worldpos.y, m_worldpos.z );



	return 0;

}

int CInfScopeDlg::MoveVertexRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int* shifvert, int shifvertnum, D3DXVECTOR3 relvec )
{
	if( m_isindex < 0 )
		return 0;
	if( shifvertnum <= 0 )
		return 0;

	int ret;
	D3DXVECTOR3 mvobj;
	int motid, frameno;
	motid = g_motdlg->GetMotCookie();
	frameno = g_motdlg->GetCurrentFrameNo();

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	ret = lpsh->GetVertPos2InfScope( m_isindex, lpmh, inimat, motid, frameno, *shifvert, &mvobj, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveVertex : GetVertPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DXVECTOR3 diff;
	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &diff, &relvec, &invw );

	D3DXVECTOR3 targetobj;

	targetobj = mvobj + diff;


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int shiftno;
	for( shiftno = 0; shiftno < shifvertnum; shiftno++ ){
		ret = lpsh->SetVertPos2InfScope( m_isindex, lpmh, motid, frameno, *( shifvert + shiftno ), targetobj );
		if( ret ){
			DbgOut( "itidlg : MoveVertex : lpsh MoveVertex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	D3DXVec3TransformCoord( &m_worldpos, &targetobj, &(papp->m_matWorld) );

	return 0;
}

int CInfScopeDlg::MoveVertexDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int* shifvert, int shifvertnum, int kind, float delta )
{
	if( m_isindex < 0 )
		return 0;
	if( shifvertnum <= 0 )
		return 0;

	D3DXVECTOR3 tempdiff, diff;
	switch( kind ){
	case 0:
		tempdiff.x = delta;
		tempdiff.y = 0.0f;
		tempdiff.z = 0.0f;
		break;
	case 1:
		tempdiff.x = 0.0f;
		tempdiff.y = delta;
		tempdiff.z = 0.0f;
		break;
	case 2:
		tempdiff.x = 0.0f;
		tempdiff.y = 0.0f;
		tempdiff.z = delta;
		break;
	default:
		_ASSERT( 0 );
		tempdiff.x = delta;
		tempdiff.y = 0.0f;
		tempdiff.z = 0.0f;
		break;
	}

	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &diff, &tempdiff, &invw );


	int ret;
	D3DXVECTOR3 mvobj;
	int motid, frameno;
	motid = g_motdlg->GetMotCookie();
	frameno = g_motdlg->GetCurrentFrameNo();

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	ret = lpsh->GetVertPos2InfScope( m_isindex, lpmh, inimat, motid, frameno, *shifvert, &mvobj, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveVertex : GetVertPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 targetobj;
	targetobj = mvobj + diff;


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int shiftno;
	for( shiftno = 0; shiftno < shifvertnum; shiftno++ ){
		ret = lpsh->SetVertPos2InfScope( m_isindex, lpmh, motid, frameno, *( shifvert + shiftno ), targetobj );
		if( ret ){
			DbgOut( "itidlg : MoveVertex : lpsh MoveVertex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	D3DXVec3TransformCoord( &m_worldpos, &targetobj, &(papp->m_matWorld) );

	return 0;
}

int CInfScopeDlg::MovePartDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int kind, float delta )
{
	if( m_isindex < 0 )
		return 0;


	int ret;
	int vertnum = 0;
	ret = lpsh->GetVertNumOfPartInfScope( m_isindex, &vertnum );
	if( ret ){
		DbgOut( "itidlg : MovePartDelta : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int vertno;
	for( vertno = 0; vertno < vertnum; vertno++ ){
		ret = MoveVertexDelta( papp, lpsh, lpmh, &vertno, 1, kind, delta );
		if( ret ){
			DbgOut( "itidlg : MoveFaceDelta : MoveVertexDelta %d error !!!\n", vertno );
			_ASSERT( 0 );
			return 1;
		}
	}

////////
	ret = lpsh->GetInitialInfScopeCenter( m_isindex, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MovePartDelta : sh GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

