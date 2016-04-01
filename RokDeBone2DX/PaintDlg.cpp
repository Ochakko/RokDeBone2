// PaintDlg.cpp : CPaintDlg のインプリメンテーション
#include "stdafx.h"
#include "PaintDlg.h"


#include <coef.h>
#include "viewer.h"
#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <shdelem.h>

#include <InfElem.h>

#include "OneOrAllDlg.h"

#define PAINTDLGTIMER	11009

#include "EditInfDlg.h"
#include "EditListInfDlg.h"

#include "GetDlgParams.h"

#include "inidata.h"

#include "viewer.h"
#include <shdhandler.h>
#include <polymesh.h>
#include <polymesh2.h>
#include <basedat.h>

enum {
	DISP_BONE,
	DISP_SYM
};

enum {
	BRUSH_1VERT,
	BRUSH_SPH,
	BRUSH_MAX
};


static LONG s_treefunc = 0;
static HWND s_tree_wnd = 0;

static LRESULT CALLBACK TreeProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
  );

//extern HINSTANCE g_resinstance;
extern CIniData* g_inidata;

/////////////////////////////////////////////////////////////////////////////
// CPaintDlg

CPaintDlg::CPaintDlg( CMyD3DApplication* papp )
{
	m_symaxis = SYMAXIS_X;
	m_symdist = 100.0f;
	
	m_sympart = -1;
	m_symvert = -1;

	m_timerid = 0;

	m_cmdshow = SW_HIDE;

	m_hImageList = 0;
	m_iImage = 0;
	m_iSelect = 0;

	m_TI = 0;
	m_selecteditem = 0;
	m_selbone = 0;

	m_papp = papp;

	m_topflag = 0;
	m_createlistflag = 0;

	m_calcmode = CALCMODE_ONESKIN0;
	m_paintmode = PAINT_NOR;
	m_per = 100.0f;
	m_saveper = m_per;

	m_seldisp = 0;
	m_selvertnum = 0;

	m_ope = PAINTOPE_SETINF;
	m_check_normalize = 1;
	
	m_directval = 1.0f;

	m_syminfnum = 0;
	ZeroMemory( &m_symIE, sizeof( INFELEM ) * INFNUMMAX );

	//InitUndoBuf();//<--これはfreeが入る
	m_undopos = 0;
	int undono;
	for( undono = 0; undono < INFPAINTUNDOMAX; undono++ ){
		m_undo[undono].partno = -1;
		m_undo[undono].vertnum = 0;
		m_undo[undono].pvertno = 0;
		m_undo[undono].selbone = -1;
		m_undo[undono].pinfnum = 0;
		m_undo[undono].pinf = 0;
	}


	m_editwnd = 0;

	m_mcap = 0;
	m_lbutton = 0;
	m_moving = 0;

	m_dispmode = DISP_BONE;

	m_undersymrenew = 0;

	m_brushtype = BRUSH_1VERT;
	m_brushsize = 100.0f;


	m_dispobjnum = 0;
	m_doi = 0;

	m_savepolydisp = POLYDISP_ALL;

	m_targetdisp = 0;
	ZeroMemory( m_targetvert, sizeof( int ) * VERTNUMMAX );
	m_targetvertnum = 0;

	m_seldisp = 0;
	ZeroMemory( m_selvert, sizeof( int ) * VERTNUMMAX );
	m_selvertnum = 0;

	ZeroMemory( &m_pickpos, sizeof( D3DXVECTOR3 ) );

	m_columnnum = 0;

	m_markpart = -1;
	m_markvert = -1;

}

CPaintDlg::~CPaintDlg()
{

	DestroyObjs();

}

int CPaintDlg::DestroyObjs()
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

	DestroyDOI();

	InitUndoBuf();

	return 0;
}

int CPaintDlg::DestroyDOI()
{
	if( m_doi ){
		free( m_doi );
		m_doi = 0;
	}
	m_dispobjnum = 0;

	return 0;
}

LRESULT CPaintDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//int ret;

	//InitCommonControls();
	m_dlg_wnd = m_hWnd;
	m_tree_wnd = GetDlgItem( IDC_TREE1 );
	s_tree_wnd = m_tree_wnd;

	m_list_wnd = GetDlgItem( IDC_LIST1 );
//	_ASSERT( m_list_wnd );
//	LONG listws;
//	listws = ::GetWindowLong( m_list_wnd, GWL_STYLE );
//	listws |= LVS_EDITLABELS;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	::SetWindowLong( m_list_wnd, GWL_STYLE, listws );


	m_nor_wnd = GetDlgItem( IDC_NOR );
	m_exc_wnd = GetDlgItem( IDC_EXC );
	m_add_wnd = GetDlgItem( IDC_ADD );
	m_sub_wnd = GetDlgItem( IDC_SUB );
	m_era_wnd = GetDlgItem( IDC_ERA );

	m_sl_per_wnd = GetDlgItem( IDC_SL_PER );
	m_per_wnd = GetDlgItem( IDC_PER );

	m_sl_per_wnd.SendMessage( TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0 );
	m_sl_per_wnd.SendMessage( TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000 );
	m_sl_per_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	m_direct_wnd = GetDlgItem( IDC_DISPINF );


	m_symx_wnd = GetDlgItem( IDC_SYMX );
	m_symy_wnd = GetDlgItem( IDC_SYMY );
	m_symz_wnd = GetDlgItem( IDC_SYMZ );
	m_symdist_wnd = GetDlgItem( IDC_SYMDIST );

	m_seldisp_wnd = GetDlgItem( IDC_COMBO_SELDISP );
	m_brush_wnd = GetDlgItem( IDC_COMBO_BRUSH );
	m_brushsize_wnd = GetDlgItem( IDC_BRUSHSIZE );

	int ret;
	ret = SetNewWindowProc();
	_ASSERT( !ret );


	ret = InitBrushCombo();
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

int CPaintDlg::SetNewWindowProc()
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



int CPaintDlg::ShowDlg( int cmdshow )
{
	int ret;
	m_cmdshow = cmdshow;
	if( cmdshow != SW_HIDE ){
		SaveMaterial();// !!!! ShowWinowより前じゃないと駄目。selchangedが呼ばれて、SetMaterialFromInfElemが実行されるため。
	}
	ShowWindow( m_cmdshow );


	if( cmdshow != SW_HIDE ){
		m_ope = PAINTOPE_SETINF;

		m_markpart = -1;
		m_markvert = -1;

		m_selbone = 0;
		m_seldisp = 0;
		m_selvertnum = 0;

		m_targetdisp = 0;
		ZeroMemory( m_targetvert, sizeof( int ) * VERTNUMMAX );
		m_targetvertnum = 0;

		m_check_normalize = 1;
		m_directval = 1.0f;
	
		m_dispmode = DISP_BONE;

		m_undersymrenew = 0;

		InitTree();
		InitList();
		ret = InitSeldispCombo();
		_ASSERT( !ret );

		m_sl_per_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

		ParamsToDlg();

		CreateTimer();
		
		m_papp->m_shandler->EnableTexture( -1, 0 );

		if( m_dispmode == DISP_BONE ){
			m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, -1, -1 );
		}else{
			m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );
		}

		InitUndoBuf();

		m_savepolydisp = g_inidata->opt_polydisp;
		g_inidata->opt_polydisp = POLYDISP_SEL;//!!!!!!!!!!!!!!
		if( m_doi && (m_seldisp > 0) && m_papp->m_shandler && (m_papp->m_shandler->s2shd_leng > 2) ){
			ret = m_papp->m_shandler->SetIsSelected( m_seldisp, 0, 0 );
			_ASSERT( !ret );
			
			m_papp->m_shandler->SetDispFlag( 1 );//!!!!!!!!!!!!!!
			(*m_papp->m_shandler)( m_seldisp )->dispflag = 1;//!!!!!!!!!!!
		}

	}else{
		DestroyTimer();

		if( m_papp->m_shandler && (m_papp->m_shandler->s2shd_leng > 1) ){
			RestoreMaterial();
			m_papp->m_shandler->EnableTexture( -1, 1 );
			m_papp->m_shandler->SetDispFlag( 1 );//!!!!!!!!!!!!!!

			::SendMessage( m_papp->m_hWnd, WM_COMMAND, 6003, 2 );//morphだけ再計算
		}

		ReleaseCapture();
		m_mcap = 0;
		m_lbutton = 0;
		m_moving = 0;

		g_inidata->opt_polydisp = m_savepolydisp;
	}

	return 0;
}

int CPaintDlg::InitTree()
{
	int ret;
	DestroyObjs();

	m_selbone = 0;

	CreateImageList();

	ret = FillTree();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CPaintDlg::InitList()
{
	m_markpart = -1;
	m_markvert = -1;

	if( m_brushtype == BRUSH_1VERT ){
		InitList1Vert();
	}else{
		InitListMulti();
	}

	return 0;
}

int CPaintDlg::InitListMulti()
{
	int ret;
	ListView_DeleteAllItems( m_list_wnd );

	int icol;
	for( icol = 0; icol < m_columnnum; icol++ ){
		ListView_DeleteColumn( m_list_wnd, 0 );
	}

	if( !m_papp->m_shandler || !m_papp->m_thandler )
		return 0;


	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 60;
	lvcol.pszText = "vertno";
	lvcol.iSubItem = 0;
	ListView_InsertColumn(m_list_wnd, 0, &lvcol);


	int subitem = 1;
	int seri;
	for( seri = 0; seri < m_papp->m_shandler->s2shd_leng; seri++ ){
		HTREEITEM* curti;
		curti = m_TI + seri;
		if( *curti ){
			CTreeElem2* te;
			te = (*m_papp->m_thandler)( seri );
			_ASSERT( te );

			lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvcol.fmt = LVCFMT_LEFT;
			lvcol.cx = 100;
			lvcol.pszText = te->name;
			lvcol.iSubItem = subitem;
			ListView_InsertColumn(m_list_wnd, subitem, &lvcol);

			subitem++;
		}
	}


	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 90;
	lvcol.pszText = "合計";
	lvcol.iSubItem = subitem;
	ListView_InsertColumn(m_list_wnd, subitem, &lvcol);
	subitem++;

	m_columnnum = subitem;

///////////////
	if( m_seldisp < 0 )
		return 0;


	int vno;
	for( vno = 0; vno < m_selvertnum; vno++ ){
		int vertno;
		vertno = m_selvert[ vno ];
		char tempchar[256];
		sprintf_s( tempchar, 256, "vt%d", vertno );

		LVITEM item;
		memset(&item, 0, sizeof(LVITEM));

		item.mask = LVIF_TEXT | LVIF_PARAM;
		item.pszText = tempchar;
		item.iItem = vno;
		item.iSubItem = 0;
		item.lParam = vertno;//!!!!!!!!!!!!!!!!!!!
		ListView_InsertItem(m_list_wnd, &item);


		int infnum;
		ret = m_papp->m_shandler->GetInfElemNum( m_seldisp, vertno, &infnum );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		INFELEM ie[ INFNUMMAX ];
		int infno;
		for( infno = 0; infno < infnum; infno++ ){
			INFELEM* pIE = 0;
			ret = m_papp->m_shandler->GetInfElem( m_seldisp, vertno, infno, &pIE );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			ie[ infno ] = *pIE;
		}


		float suminf = 0.0f;
		subitem = 1;
		for( seri = 0; seri < m_papp->m_shandler->s2shd_leng; seri++ ){
			HTREEITEM* curti;
			curti = m_TI + seri;
			if( *curti ){
				int findno = -1;
				for( infno = 0; infno < infnum; infno++ ){
					if( (seri != 0) && (ie[infno].childno == seri) ){
						findno = infno;
						break;
					}
				}

				if( findno >= 0 ){
					float listinf;
					listinf = ie[ findno ].orginf * ie[ findno ].userrate;
					sprintf_s( tempchar, 256, "*%f", listinf );
					item.mask = LVIF_TEXT;
					item.pszText = tempchar;
					item.iItem = vno;
					item.iSubItem = subitem;
					item.lParam = 0;
					ListView_SetItem(m_list_wnd, &item);

					suminf += listinf;
				}else{
					strcpy_s( tempchar, 256, "0.0" );
					item.mask = LVIF_TEXT;
					item.pszText = tempchar;
					item.iItem = vno;
					item.iSubItem = subitem;
					item.lParam = 0;
					ListView_SetItem(m_list_wnd, &item);
				}

				//_ASSERT( 0 );

				subitem++;
			}
		}

		sprintf_s( tempchar, 256, "%f", suminf );
		item.mask = LVIF_TEXT;
		item.pszText = tempchar;
		item.iItem = vno;
		item.iSubItem = subitem;
		ListView_SetItem(m_list_wnd, &item);
	}


	return 0;
}

int CPaintDlg::InitList1Vert()
{
	int ret;
	ListView_DeleteAllItems( m_list_wnd );

	int icol;
	for( icol = 0; icol < m_columnnum; icol++ ){
		ListView_DeleteColumn( m_list_wnd, 0 );
	}

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 100;
	lvcol.pszText = "ボーン名";
	lvcol.iSubItem = SUBID_NAME;
	ListView_InsertColumn(m_list_wnd, SUBID_NAME, &lvcol);

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 80;
	lvcol.pszText = "計算方法";
	lvcol.iSubItem = SUBID_CALC;
	ListView_InsertColumn(m_list_wnd, SUBID_CALC, &lvcol);

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 50;
	lvcol.pszText = "比重％";
	lvcol.iSubItem = SUBID_RATE;
	ListView_InsertColumn(m_list_wnd, SUBID_RATE, &lvcol);

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 85;
	lvcol.pszText = "正規前影響度";
	lvcol.iSubItem = SUBID_ORGINF;
	ListView_InsertColumn(m_list_wnd, SUBID_ORGINF, &lvcol);

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 80;
	lvcol.pszText = "最終影響度";
	lvcol.iSubItem = SUBID_DISPINF;
	ListView_InsertColumn(m_list_wnd, SUBID_DISPINF, &lvcol);

	m_columnnum = SUBID_DISPINF + 1;//!!!!!!!!!!!!!!!!!!!!


	if( (m_seldisp > 0) && (m_selvertnum > 0) ){
		int infnum;
		ret = m_papp->m_shandler->GetInfElemNum( m_seldisp, m_selvert[0], &infnum );
		if( ret ){
			DbgOut( "paintdlg : InitList : sh GetInfElemNum error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int infno;
		INFELEM* curIE;
		int setno = 0;
		for( infno = 0; infno < infnum; infno++ ){
			ret = m_papp->m_shandler->GetInfElem( m_seldisp, m_selvert[0], infno, &curIE );
			if( ret ){
				DbgOut( "paintdlg : InitList : sh GetInfElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( curIE->bonematno > 0 ){
				ret = AddItem2List( setno, curIE );
				_ASSERT( !ret );
				setno++;
			}
		}

	}

	return 0;
}

int CPaintDlg::AddItem2List( int itemno, INFELEM* IEptr )
{
	if( !IEptr )
		return 0;

	char tempchar[256];

	CTreeElem2* telem;
	telem = (*m_papp->m_thandler)( IEptr->childno );
	_ASSERT( telem );


	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));

	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.pszText = telem->name;
	item.iItem = itemno;
	item.iSubItem = SUBID_NAME;
	item.lParam = telem->serialno;
	ListView_InsertItem(m_list_wnd, &item);


item.mask = LVIF_TEXT;
	item.pszText = calcmodestr[ IEptr->kind ];
	item.iItem = itemno;
	item.iSubItem = SUBID_CALC;
	//item.lParam = telem->serialno;
	ListView_SetItem(m_list_wnd, &item);

	sprintf_s( tempchar, 256, "%.2f", IEptr->userrate * 100.0f );
	item.pszText = tempchar;
	item.iItem = itemno;
	item.iSubItem = SUBID_RATE;
//	item.lParam = telem->serialno;
	ListView_SetItem(m_list_wnd, &item);

	sprintf_s( tempchar, 256, "%f", IEptr->orginf );
	item.pszText = tempchar;
	item.iItem = itemno;
	item.iSubItem = SUBID_ORGINF;
//	item.lParam = telem->serialno;
	ListView_SetItem(m_list_wnd, &item);

	sprintf_s( tempchar, 256, "%f", IEptr->dispinf );
	item.pszText = tempchar;
	item.iItem = itemno;
	item.iSubItem = SUBID_DISPINF;
//	item.lParam = telem->serialno;
	ListView_SetItem(m_list_wnd, &item);

	return 0;
			
}


LRESULT CPaintDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_papp->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);
	return 0;
}



LRESULT CPaintDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_papp->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);
	return 0;
}

int CPaintDlg::FillTree()
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
void CPaintDlg::AddBoneToTree( int srcseri, int addbroflag, int addtolast )
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
				DbgOut( "PaintDlg : AddBoneToTree : TVAdd error %d %s!!!\n", srcseri, shdname );
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
HTREEITEM CPaintDlg::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{
	//ここのDbg文は取り除かない。
	DbgOut( "PaintDlg : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
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

void CPaintDlg::CreateImageList()
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
		DbgOut( "PaintDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iImage = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iImage == -1 ){
		DbgOut( "PaintDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "PaintDlg : CreateImageList : LoadBitmap error !!!\n" );		
	}
	m_iSelect = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_iSelect == -1 ){
		DbgOut( "PaintDlg : CreateImageList : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_tree_wnd,
		m_hImageList, TVSIL_NORMAL );

	
}
int CPaintDlg::ParamsToDlg()
{

	switch( m_calcmode ){
	case CALCMODE_ONESKIN0:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ONESKIN0, IDC_RADIO_SYM, IDC_RADIO_ONESKIN0 );

		m_nor_wnd.EnableWindow( TRUE );
		m_exc_wnd.EnableWindow( TRUE );
		m_add_wnd.EnableWindow( TRUE );
		m_sub_wnd.EnableWindow( TRUE );
		m_era_wnd.EnableWindow( TRUE );

		m_sl_per_wnd.EnableWindow( TRUE );
		break;
	case CALCMODE_ONESKIN1:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ONESKIN0, IDC_RADIO_SYM, IDC_RADIO_ONESKIN1 );

		m_nor_wnd.EnableWindow( TRUE );
		m_exc_wnd.EnableWindow( TRUE );
		m_add_wnd.EnableWindow( TRUE );
		m_sub_wnd.EnableWindow( TRUE );
		m_era_wnd.EnableWindow( TRUE );

		m_sl_per_wnd.EnableWindow( TRUE );
		break;
	case CALCMODE_NOSKIN0:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ONESKIN0, IDC_RADIO_SYM, IDC_RADIO_NOSKIN0 );

		m_nor_wnd.EnableWindow( FALSE );
		m_exc_wnd.EnableWindow( TRUE );
		m_add_wnd.EnableWindow( FALSE );
		m_sub_wnd.EnableWindow( FALSE );
		m_era_wnd.EnableWindow( TRUE );
		if( (m_paintmode == PAINT_NOR) || (m_paintmode == PAINT_ADD) || (m_paintmode == PAINT_SUB) ){
			m_paintmode = PAINT_EXC;//!!!!!!!!!!!!!!!
		}

		m_sl_per_wnd.EnableWindow( FALSE );
		break;
	case CALCMODE_DIRECT0:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ONESKIN0, IDC_RADIO_SYM, IDC_RADIO_DIRECT0 );

		m_nor_wnd.EnableWindow( TRUE );
		m_exc_wnd.EnableWindow( TRUE );
		m_add_wnd.EnableWindow( FALSE );
		m_sub_wnd.EnableWindow( FALSE );
		m_era_wnd.EnableWindow( TRUE );
		if( (m_paintmode == PAINT_ADD) || (m_paintmode == PAINT_SUB) ){
			m_paintmode = PAINT_NOR;//!!!!!!!!!!!!!!!!!
		}

		m_sl_per_wnd.EnableWindow( TRUE );
		break;
	case CALCMODE_SYM:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ONESKIN0, IDC_RADIO_SYM, IDC_RADIO_SYM );

		m_nor_wnd.EnableWindow( FALSE );
		m_exc_wnd.EnableWindow( TRUE );
		m_add_wnd.EnableWindow( FALSE );
		m_sub_wnd.EnableWindow( FALSE );
		m_era_wnd.EnableWindow( FALSE );
		if( (m_paintmode == PAINT_NOR) || (m_paintmode == PAINT_ADD) || (m_paintmode == PAINT_SUB) || (m_paintmode == PAINT_ERA) ){
			m_paintmode = PAINT_EXC;//!!!!!!!!!!!!!!!!!
		}

		m_sl_per_wnd.EnableWindow( FALSE );

		break;
	default:
		m_calcmode = CALCMODE_ONESKIN0;
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_ONESKIN0, IDC_RADIO_SYM, IDC_RADIO_ONESKIN0 );

		m_nor_wnd.EnableWindow( TRUE );
		m_exc_wnd.EnableWindow( TRUE );
		m_add_wnd.EnableWindow( TRUE );
		m_sub_wnd.EnableWindow( TRUE );
		m_era_wnd.EnableWindow( TRUE );

		m_sl_per_wnd.EnableWindow( TRUE );
		break;
	}


	CheckPaintModeButton();
	CheckSymAxisButton();

	char tempchar[256];
	sprintf_s( tempchar, 256, "%f％", m_per );
	m_per_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%f", m_directval );
	m_direct_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 256, "%f", m_symdist );
	m_symdist_wnd.SetWindowText( tempchar );


	switch( m_ope ){
	case PAINTOPE_SETINF:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SETINF, IDC_RADIO_BONE, IDC_RADIO_SETINF );
		break;
	case PAINTOPE_BONE:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SETINF, IDC_RADIO_BONE, IDC_RADIO_BONE );
		break;
	default:
		m_ope = PAINTOPE_SETINF;
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_SETINF, IDC_RADIO_BONE, IDC_RADIO_SETINF );
		break;
	}

	if( m_dispmode == DISP_BONE ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_DISPBONE, IDC_RADIO_DISPSYM, IDC_RADIO_DISPBONE );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_DISPBONE, IDC_RADIO_DISPSYM, IDC_RADIO_DISPSYM );
	}


	if( m_check_normalize ){
		m_dlg_wnd.CheckDlgButton( IDC_CHK_NORM, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHK_NORM, BST_UNCHECKED );
	}

	m_brush_wnd.SendMessage( CB_SETCURSEL, m_brushtype, 0 );

	sprintf_s( tempchar, 256, "%f", m_brushsize );
	m_brushsize_wnd.SetWindowText( tempchar );

	if( (m_dispobjnum > 0) && m_doi && (m_seldisp > 0) ){
		int doino;
		int findindex = -1;
		for( doino = 0; doino < m_dispobjnum; doino++ ){
			DISPOBJINFO* curdoi;
			curdoi = m_doi + doino;
			if( curdoi->serialno == m_seldisp ){
				findindex = doino;
				break;
			}
		}
		if( findindex > 0 ){
			m_seldisp_wnd.SendMessage( CB_SETCURSEL, findindex, 0 );
		}
	}


	return 0;
}

LRESULT CPaintDlg::OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;

	m_selecteditem = tvi.hItem;

	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !TreeView_GetItem( m_tree_wnd, &tvi ) )
		return 0;

	m_selbone = (int)(tvi.lParam);

	if( m_cmdshow != SW_HIDE ){
		if( m_dispmode == DISP_BONE ){
			m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, -1, -1 );
		}else{
			m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );
		}

	}

	if( m_selbone > 0 ){
		::SendMessage( m_papp->m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_PAINTDLG, (LPARAM)m_selbone );
	}


	return 0;
}

int CPaintDlg::OnUserSelChange( int selno )
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

		if( m_cmdshow != SW_HIDE ){
			if( m_dispmode == DISP_BONE ){
				m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, -1, -1 );
			}else{
				m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );
			}

			::UpdateWindow( m_tree_wnd );
		}

	}

	return 0;
}


int CPaintDlg::TreeViewSelChange( int srcseri )
{
	//list1の選択により、呼ばれる。
	if( (srcseri < 0) || (srcseri >= m_papp->m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		return 1;
	}

	m_selecteditem = *(m_TI + srcseri);
	m_selbone = srcseri;

	if( m_selecteditem ){
		TreeView_SelectItem( m_tree_wnd, m_selecteditem );

		if( m_cmdshow != SW_HIDE ){
			if( m_dispmode == DISP_BONE ){
				m_papp->m_shandler->SetMaterialFromInfElem( srcseri, -1, -1 );	
			}else{
				m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );
			}
		}
	}

	return 0;
}

int CPaintDlg::GetCalcMode()
{
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_ONESKIN0 );
	if( checkflag == BST_CHECKED ){
		m_calcmode = CALCMODE_ONESKIN0;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_ONESKIN1 );
	if( checkflag == BST_CHECKED ){
		m_calcmode = CALCMODE_ONESKIN1;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_NOSKIN0 );
	if( checkflag == BST_CHECKED ){
		m_calcmode = CALCMODE_NOSKIN0;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_DIRECT0 );
	if( checkflag == BST_CHECKED ){
		m_calcmode = CALCMODE_DIRECT0;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_SYM );
	if( checkflag == BST_CHECKED ){
		m_calcmode = CALCMODE_SYM;
	}

	return 0;
}

LRESULT CPaintDlg::OnRadioOneskin0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetCalcMode();
	ParamsToDlg();// paintmode button のEnable
	return 0;
}
LRESULT CPaintDlg::OnRadioOneskin1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetCalcMode();
	ParamsToDlg();
	return 0;
}
LRESULT CPaintDlg::OnRadioNoskin0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetCalcMode();
	if( m_calcmode == CALCMODE_NOSKIN0 ){
		m_per = 100.0f;
		m_saveper = m_per;
	}
	ParamsToDlg();
	return 0;
}
LRESULT CPaintDlg::OnRadioDirect0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetCalcMode();
	ParamsToDlg();

	return 0;
}
LRESULT CPaintDlg::OnRadioSym(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetCalcMode();
	if( m_calcmode == CALCMODE_SYM ){
		m_per = 100.0f;
		m_saveper = m_per;
	}
	ParamsToDlg();// paintmode button のEnable
	return 0;
}

int CPaintDlg::CheckSymAxisButton()
{
	switch( m_symaxis ){
	case SYMAXIS_X:
		SendMessage( m_symx_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_symy_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_symz_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case SYMAXIS_Y:
		SendMessage( m_symx_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_symy_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_symz_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case SYMAXIS_Z:
		SendMessage( m_symx_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_symy_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_symz_wnd, BM_SETSTATE, TRUE, 0 );
		break;
	default:
		m_symaxis = SYMAXIS_X;

		SendMessage( m_symx_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_symy_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_symz_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	}

	return 0;
}

int CPaintDlg::CheckPaintModeButton()
{
	switch( m_paintmode ){
	case PAINT_NOR:
		SendMessage( m_nor_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_exc_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_add_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_sub_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_era_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case PAINT_EXC:
		SendMessage( m_nor_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_exc_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_add_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_sub_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_era_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case PAINT_ADD:
		SendMessage( m_nor_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_exc_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_add_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_sub_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_era_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case PAINT_SUB:
		SendMessage( m_nor_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_exc_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_add_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_sub_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_era_wnd, BM_SETSTATE, FALSE, 0 );
		break;
	case PAINT_ERA:
		SendMessage( m_nor_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_exc_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_add_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_sub_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_era_wnd, BM_SETSTATE, TRUE, 0 );
		break;
	default:
		m_paintmode = PAINT_NOR;

		SendMessage( m_nor_wnd, BM_SETSTATE, TRUE, 0 );
		SendMessage( m_exc_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_add_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_sub_wnd, BM_SETSTATE, FALSE, 0 );
		SendMessage( m_era_wnd, BM_SETSTATE, FALSE, 0 );

		break;
	}

	return 0;
}

LRESULT CPaintDlg::OnSelNor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_paintmode = PAINT_NOR;
	CheckPaintModeButton();
	return 0;
}
LRESULT CPaintDlg::OnSelExc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_paintmode = PAINT_EXC;
	CheckPaintModeButton();
	return 0;
}
LRESULT CPaintDlg::OnSelAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_paintmode = PAINT_ADD;
	CheckPaintModeButton();
	return 0;
}
LRESULT CPaintDlg::OnSelSub(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_paintmode = PAINT_SUB;
	CheckPaintModeButton();
	return 0;
}
LRESULT CPaintDlg::OnSelEra(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_paintmode = PAINT_ERA;
	CheckPaintModeButton();
	return 0;
}
LRESULT CPaintDlg::OnSymx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_symaxis = SYMAXIS_X;
	CheckSymAxisButton();
	return 0;
}
LRESULT CPaintDlg::OnSymy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_symaxis = SYMAXIS_Y;
	CheckSymAxisButton();
	return 0;
}
LRESULT CPaintDlg::OnSymz(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_symaxis = SYMAXIS_Z;
	CheckSymAxisButton();
	return 0;
}


LRESULT CPaintDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static int savecurpos = 0;


	HWND scwnd = (HWND)lParam;
	WORD event;

	event = LOWORD( wParam );

	int curpos;


	if( (event = SB_THUMBTRACK) || (event == SB_THUMBPOSITION ) ){

		curpos = (int)m_sl_per_wnd.SendMessage( TBM_GETPOS, 0, 0 );

		if( curpos == savecurpos ){
			return 0;//!!!!!!!!!!!!!!!!!!!!!
		}else{
			savecurpos = curpos;
		}

		m_per = m_saveper + (float)( curpos - 500 ) / 10.0f; 
		if( m_per < 0.0f )
			m_per = 0.0f;

		ParamsToDlg();
	}

	return 0;
}

LRESULT CPaintDlg::OnReleasedcaptureSl_per(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	m_sl_per_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)500 );

	m_saveper = m_per;
	return 0;
}

LRESULT CPaintDlg::OnSymRenew(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	int dispnum;
	dispnum = m_papp->m_shandler->s2shd_leng;
	int dispno;
	for( dispno = 0; dispno < dispnum; dispno++ ){
		CShdElem* selem;
		selem = (*m_papp->m_shandler)( dispno );
		if( ((selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2)) && (selem->notuse == 0) ){
			int vertnum;

			ret = m_papp->m_shandler->GetVertNumOfPart( dispno, &vertnum );
			if( ret ){
				DbgOut( "paintdlg : OnSymRenew : sh GetVertNumOfPart error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			int vertno;
			for( vertno = 0; vertno < vertnum; vertno++ ){
				int calcmode;
				ret = m_papp->m_shandler->GetInfElemCalcmode( dispno, vertno, 0, &calcmode );
				if( ret ){
					DbgOut( "paintdlg : OnSymRenew : sh GetInfElemCalcmode error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				if( calcmode == CALCMODE_SYM ){
					int symaxis;
					float symdist;
					ret = m_papp->m_shandler->GetInfElemSymParams( dispno, vertno, &symaxis, &symdist );
					if( ret ){
						DbgOut( "paintdlg : OnSymRenew : sh GetInfElemSymParams error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					int sympart, symvert;

					ret = m_papp->m_shandler->GetSymVert( dispno, vertno, symaxis, symdist, &sympart, &symvert );
					if( ret ){
						DbgOut( "paintdlg : OnSymRenew : sh GetSymVert error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					if( (sympart > 0) && (symvert >= 0) ){

						int syminfnum = 0;

						ret = SetSymIE( sympart, symvert, &m_syminfnum );
						if( ret ){
							DbgOut( "paintdlg : OnSymRenew : SetSymIE error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}


						ret = m_papp->m_shandler->DestroyIE( dispno, vertno );
						if( ret ){
							DbgOut( "paintdlg : OnSymRenew : sh DestroyIE error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
						int infno;
						for( infno = 0; infno < m_syminfnum; infno++ ){
							ret = m_papp->m_shandler->AddInfElem( dispno, vertno, m_symIE[infno] );
							if( ret ){
								DbgOut( "paintdlg : OnSymRenew : sh AddInfElem error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}
						//ret = m_papp->m_shandler->SetInfElemSymParams( m_seldisp, m_selvert[vno], m_symaxis, m_symdist );
						//if( ret ){
						//	DbgOut( "paintdlg : CopySymInf : sh SetInfElemSymParams error !!!\n" );
						//	_ASSERT( 0 );
						//	return 1;
						//}

					}

				}
			}


			ret = m_papp->m_shandler->CreateSkinMat( dispno, 1 );
			if( ret ){
				DbgOut( "paintdlg : OnSymRenew : sh CreateSkinMat error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			//m_papp->m_shandler->SetMaterialFromInfElem( -1, dispno, -1 );
		}
	}

	if( (m_selbone > 0) && (m_seldisp > 0) ){
		if( m_dispmode == DISP_BONE ){
			m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
		}else{
			//m_papp->m_shandler->SetMaterialFromInfElem( -1, m_seldisp, -1 );
			m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );
		}
	}

	InitList();

	return 0;
}

LRESULT CPaintDlg::OnSymLump(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	int dispnum;
	dispnum = m_papp->m_shandler->s2shd_leng;
	int dispno;
	for( dispno = 0; dispno < dispnum; dispno++ ){
		CShdElem* selem;
		selem = (*m_papp->m_shandler)( dispno );
		if( ((selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2)) && (selem->notuse == 0) ){
			int vertnum;

			ret = m_papp->m_shandler->GetVertNumOfPart( dispno, &vertnum );
			if( ret ){
				DbgOut( "paintdlg : OnSymLump : sh GetVertNumOfPart error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			int vertno;
			for( vertno = 0; vertno < vertnum; vertno++ ){
				D3DXVECTOR3 vpos;
				ret = m_papp->m_shandler->GetVertPos( dispno, vertno, &vpos );
				if( ret ){
					DbgOut( "paintdlg : OnSymLump : sh GetVertPos error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				int doset = 0;
				switch( m_symaxis ){
					case SYMAXIS_X:
						if( vpos.x < 0.0f )
							doset = 1;
						break;
					case SYMAXIS_Y:
						if( vpos.y < 0.0f )
							doset = 1;
						break;
					case SYMAXIS_Z:
						if( vpos.z < 0.0f )
							doset = 1;
						break;
					default:
						break;
				}


				if( doset == 1 ){

					int calcmode;
					ret = m_papp->m_shandler->GetInfElemCalcmode( dispno, vertno, 0, &calcmode );
					if( ret ){
						DbgOut( "paintdlg : OnSymLump : sh GetInfElemCalcmode error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					int sympart, symvert;
					ret = m_papp->m_shandler->GetSymVert( dispno, vertno, m_symaxis, m_symdist, &sympart, &symvert );
					if( ret ){
						DbgOut( "paintdlg : OnSymLump : sh GetSymVert error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					if( (sympart > 0) && (symvert >= 0) ){

						////////////////////
						int syminfnum = 0;

						ret = SetSymIE( sympart, symvert, &m_syminfnum );
						if( ret ){
							DbgOut( "paintdlg : OnSymLump : SetSymIE error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}


						ret = m_papp->m_shandler->DestroyIE( dispno, vertno );
						if( ret ){
							DbgOut( "paintdlg : OnSymLump : sh DestroyIE error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
						int infno;
						for( infno = 0; infno < m_syminfnum; infno++ ){
							ret = m_papp->m_shandler->AddInfElem( dispno, vertno, m_symIE[infno] );
							if( ret ){
								DbgOut( "paintdlg : OnSymLump : sh AddInfElem error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}
						//ret = m_papp->m_shandler->SetInfElemSymParams( m_seldisp, m_selvert[vno], m_symaxis, m_symdist );
						//if( ret ){
						//	DbgOut( "paintdlg : CopySymInf : sh SetInfElemSymParams error !!!\n" );
						//	_ASSERT( 0 );
						//	return 1;
						//}

					}

				}else{

					//プラス側の頂点が対称ペイントになっていたら、数値直接指定（黄色）に変更する。
						
					int infnum;
					ret = m_papp->m_shandler->GetInfElemNum( dispno, vertno, &infnum );
					if( ret ){
						DbgOut( "paintdlg : OnSymLump : sh GetInfElemNum error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					
					int infno;
					for( infno = 0; infno < infnum; infno++ ){

						int calcmode;
						ret = m_papp->m_shandler->GetInfElemCalcmode( dispno, vertno, infno, &calcmode );
						if( ret ){
							DbgOut( "paintdlg : OnSymLump : sh GetInfElemCalcmode error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}

						if( calcmode == CALCMODE_SYM ){
							ret = m_papp->m_shandler->SetInfElemCalcmode( dispno, vertno, infno, CALCMODE_DIRECT0 );
							if( ret ){
								DbgOut( "paintdlg : OnSymLump : sh SetInfElemCalcMode error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}
					}
				}
			}


			ret = m_papp->m_shandler->CreateSkinMat( dispno, 1 );
			if( ret ){
				DbgOut( "paintdlg : OnSymLump : sh CreateSkinMat error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			//m_papp->m_shandler->SetMaterialFromInfElem( -1, dispno, -1 );
		}
	}

	if( m_dispmode == DISP_BONE ){
		if( (m_selbone > 0) && (m_seldisp > 0) ){
			m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
		}
	}else{
		m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );
	}

	/***
	if( (m_selbone > 0) && (m_seldisp > 0) ){
		if( m_dispmode == DISP_BONE ){
			m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
		}else{
			//m_papp->m_shandler->SetMaterialFromInfElem( -1, m_seldisp, -1 );
			m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );
		}
	}
	***/

	InitList();

	return 0;
}



int CPaintDlg::OnClickVert( int seldisp, int* selvert, int selvertnum, int isleftclick )
{
	int ret;

	//m_seldisp = seldisp;//pickで初期化されるとまずい。何も選択されなくなる。
	m_selvertnum = selvertnum;
	if( (m_selvertnum > 0) && (m_selvertnum < VERTNUMMAX ) ){
		MoveMemory( m_selvert, selvert, sizeof( int ) * selvertnum );
	}else{
		m_selvertnum = 0;
		ZeroMemory( m_selvert, sizeof( int ) * selvertnum );
	}

	ret = ClickOpe( isleftclick, 0 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CPaintDlg::ClickOpe( int isleftclick, int forceblue )
{
	int ret;
	if( (m_seldisp <= 0) || (m_selvertnum <= 0) || (m_selvertnum >= VERTNUMMAX) ){
		return 0;
	}

	InitList();

	if( isleftclick == 0 )
		return 0;//!!!!!!!!!!!


///////// for UNDO	
	int* pvertno;
	pvertno = (int*)malloc( sizeof( int ) * m_selvertnum );
	if( !pvertno ){
		DbgOut( "paintdlg : OnClickVert : pvertno alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( pvertno, sizeof( int ) * m_selvertnum );

	int* pinfnum;
	pinfnum = (int*)malloc( sizeof( int ) * m_selvertnum );
	if( !pinfnum ){
		DbgOut( "paintdlg : OnClickVert : pinfnum alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( pinfnum, sizeof( int ) * m_selvertnum );

	INFELEM* pinf;
	pinf = (INFELEM*)malloc( sizeof( INFELEM ) * INFNUMMAX * m_selvertnum );
	if( !pinf ){
		DbgOut( "paintdlg : OnClickVert : pinf alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( pinf, sizeof( INFELEM ) * INFNUMMAX * m_selvertnum );

	int selno;
	for( selno = 0; selno < m_selvertnum; selno++ ){
		*(pvertno + selno) = m_selvert[selno];

		ret = m_papp->m_shandler->GetInfElemNum( m_seldisp, m_selvert[selno], pinfnum + selno );
		if( ret ){
			DbgOut( "paintdlg : OnClickVert : sh GetInfElemNum error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( *(pinfnum + selno) > INFNUMMAX ){
			_ASSERT( 0 );
			*(pinfnum + selno) = INFNUMMAX;//!!!!!!!!!
		}
		int infno;
		INFELEM* pIE;
		for( infno = 0; infno < *(pinfnum + selno); infno++ ){
			ret = m_papp->m_shandler->GetInfElem( m_seldisp, m_selvert[selno], infno, &pIE );
			if( ret || !pIE ){
				DbgOut( "paintdlg : OnClickVert : sh GetInfElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			*( pinf + INFNUMMAX * selno + infno ) = *pIE;
		}
	}
	ret = AddUndoBuf( m_seldisp, pvertno, m_selvertnum, m_selbone, pinfnum, pinf );
	if( ret ){
		DbgOut( "paintdlg : OnClickVert : AddUndoBuf error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

///////// paint

	CShdElem* dispelem;
	dispelem = (*m_papp->m_shandler)( m_seldisp );
	_ASSERT( dispelem );
	dispelem->m_loadbimflag = 2;//!!!!!!!!!!!!!!!!!!!!!

	if( m_calcmode != CALCMODE_SYM ){

		switch( m_paintmode ){
		case PAINT_NOR:
		case PAINT_EXC:
		case PAINT_ADD:
		case PAINT_SUB:
			ret = SetInfElem( forceblue );
			if( ret ){
				DbgOut( "paintdlg : OnClickVert : SetInfElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			break;
		case PAINT_ERA:
			ret = DeleteInfElem( pinfnum );
			if( ret ){
				DbgOut( "paintdlg : OnClickVert : DeleteInfElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			break;
		default:
			break;
		}

	}else{
		ret = GetFloat( m_symdist_wnd, &m_symdist );
		if( ret || (m_directval < 0.0f) ){
			::MessageBox( m_hWnd, "対称操作の距離項目には、正の小数を指定してください。", "入力エラー", MB_OK );
			return 0;
		}

		int vno;
		for( vno = 0; vno < m_selvertnum; vno++ ){
			ret = m_papp->m_shandler->GetSymVert( m_seldisp, m_selvert[vno], m_symaxis, m_symdist, &m_sympart, &m_symvert );
			if( ret ){
				DbgOut( "paintdlg : OnClickVert : sh GetSymVert error !!!\n" );
				_ASSERT( 0 );
				//return 1;
				continue;
			}
			if( (m_sympart <= 0) || (m_symvert < 0) ){
				//::MessageBox( m_hWnd, "指定した距離の対称点はありません。", "見つかりません", MB_OK );
				//return 0;
				continue;
			}

			ret = SetSymIE( m_sympart, m_symvert, &m_syminfnum );
			if( ret ){
				DbgOut( "paintdlg : OnClickVert : SetSymIE error !!!\n" );
				_ASSERT( 0 );
				//return 1;
				continue;
			}

			ret = CopySymInf( vno );
			if( ret ){
				DbgOut( "patindlg : OnClickVert : CopySymInf error !!!\n" );
				_ASSERT( 0 );
				//return 1;
			}
		}

		ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
		if( ret ){
			DbgOut( "paintdlg : CopySymInf : sh CreateSkinMat error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( m_dispmode == DISP_BONE ){
			m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
		}else{
			m_papp->m_shandler->SetMaterialFromInfElem( -1, m_seldisp, -1 );
		}
		InitList();

	}

	return 0;

}


int CPaintDlg::SetSymIE( int sympart, int symvert, int* syminfnumptr )
{

	int ret;
	INFELEM tempIE[ INFNUMMAX ];

	ret = m_papp->m_shandler->GetInfElemNum( sympart, symvert, syminfnumptr );
	if( ret ){
		DbgOut( "paintdlg : SetSymIE : sh GetInfElemNum error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( *syminfnumptr > INFNUMMAX )
		*syminfnumptr = INFNUMMAX;//!!!!!!!!!!

	int sinfno;
	INFELEM* psIE;
	for( sinfno = 0; sinfno < *syminfnumptr; sinfno++ ){
		ret = m_papp->m_shandler->GetInfElem( sympart, symvert, sinfno, &psIE );
		if( ret || !psIE ){
			DbgOut( "paintdlg : SetSymIE : sh GetInfElem sym error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		tempIE[sinfno] = *psIE;
	}

	//////////

	


	ret = Conv2SymIE( tempIE, *syminfnumptr, m_symIE );
	if( ret ){
		DbgOut( "paintdlg : SetSymIE : Conv2SymIE error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


LRESULT CPaintDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	int timerid = (int)wParam;
	if( timerid == m_timerid ){

		if( (m_cmdshow != SW_HIDE) && (m_ope != PAINTOPE_BONE) ){
			if( m_brushtype == BRUSH_1VERT ){
				if( m_seldisp > 0 ){
					ret = m_papp->TempPickVert( m_seldisp, &m_targetdisp, m_targetvert, &m_targetvertnum );
					_ASSERT( !ret );
				}
			}else{
				if( m_seldisp > 0 ){
					int pickpart, pickface;
					ret = m_papp->TempPickFace( m_seldisp, &pickpart, &pickface, &m_pickpos );
					if( !ret && (pickpart == m_seldisp) && (pickface > 0) ){
						m_targetdisp = m_seldisp;

						CShdElem* selem;
						selem = (*m_papp->m_shandler)( m_targetdisp );
						ret = selem->PickVertWorld( m_pickpos, m_brushsize, m_targetvert, VERTNUMMAX, &m_targetvertnum );
						_ASSERT( !ret );

					}else{
						m_targetdisp = 0;
						m_targetvertnum = 0;
					}
				}else{
					m_targetdisp = 0;
					m_targetvertnum = 0;
				}
			}
		}
		BOOL dummy;
		OnMouseMove( 0, 0, 0, dummy );

	}

	return 0;
}
LRESULT CPaintDlg::OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyTimer();
	return 0;
}

int CPaintDlg::CreateTimer()
{
	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( PAINTDLGTIMER, 100 );
		//DbgOut( "PaintDlg :CreateSwapChain : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == PAINTDLGTIMER );
		//DbgOut( "axisdlg : m_timerid %d, %d\r\n", m_timerid, RENDERAXISTIMER );
	}
	return 0;
}
int CPaintDlg::DestroyTimer()
{
	int ret;
	if( m_timerid != 0 ){
		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;
		//Sleep( 100 );//!!!!!!!!!!!!!!!!!!!
	}

	return 0;
}

int CPaintDlg::SaveMaterial()
{
	int ret;
	ret = m_papp->m_shandler->CreateAndSaveTempMaterial();
	if( ret ){
		DbgOut( "paintdlg : SaveMaterial : sh CreateAndSaveTempMaterial error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	/***
	ret = m_papp->m_shandler->SetDispDiffuse( -1, 255, 255, 255, 0, -1, 0 );
	if( ret ){
		DbgOut( "paintdlg : SaveMaterial : sh SetDispDiffuse error !!!\n");
		_ASSERT( 0 );
		return 1;
	}
	ret = m_papp->m_shandler->SetDispAmbient( -1, 64, 64, 64, 0, -1 );
	if( ret ){
		DbgOut( "paintdlg : SaveMaterial : sh SetDispAmbient error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_papp->m_shandler->SetDispSpecular( -1, 0, 0, 0, 0, -1 );
	if( ret ){
		DbgOut( "paintdlg : SaveMaterial : sh SetDispSpecular error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_papp->m_shandler->SetDispEmissive( -1, 0, 0, 0, 0, -1 );
	if( ret ){
		DbgOut( "paintdlg : SaveMaterial : sh SetDispEmissive error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/


	return 0;
}
int CPaintDlg::RestoreMaterial()
{

	int ret;
	ret = m_papp->m_shandler->RestoreAndDestroyTempMaterial();
	if( ret ){
		DbgOut( "paintdlg : RestoreMaterial : sh ResotreAndDestroyTempMaterial error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CPaintDlg::CopySymInf( int vno )
{
	int ret;
	if( (m_selbone <= 0) || (m_seldisp <= 0) || (m_selvertnum <= 0) ){
		return 0;
	}

	//int vno;
	//for( vno = 0; vno < m_selvertnum; vno++ ){
		ret = m_papp->m_shandler->DestroyIE( m_seldisp, m_selvert[vno] );
		if( ret ){
			DbgOut( "paintdlg : CopySymInf : sh DestroyIE error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int infno;
		for( infno = 0; infno < m_syminfnum; infno++ ){
			ret = m_papp->m_shandler->AddInfElem( m_seldisp, m_selvert[vno], m_symIE[infno] );
			if( ret ){
				DbgOut( "paintdlg : CopySymInf : sh AddInfElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = m_papp->m_shandler->SetInfElemSymParams( m_seldisp, m_selvert[vno], m_symaxis, m_symdist );
		if( ret ){
			DbgOut( "paintdlg : CopySymInf : sh SetInfElemSymParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	//}
	
//	ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
//	if( ret ){
//		DbgOut( "paintdlg : CopySymInf : sh CreateSkinMat error !!!\n" );
//		_ASSERT( 0 );
//		return 1;
//	}


//	if( m_dispmode == DISP_BONE ){
//		m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
//	}else{
//		m_papp->m_shandler->SetMaterialFromInfElem( -1, m_seldisp, -1 );
//	}
//	InitList();


	return 0;
}

int CPaintDlg::SetInfElem( int forceblue )
{
	int ret;
	if( (m_selbone <= 0) || (m_seldisp <= 0) || (m_selvertnum <= 0) ){
		return 0;
	}
	
	int setpaintmode;
	int setcalcmode;
	float rate;

	if( forceblue == 0 ){
		setpaintmode = m_paintmode;
		setcalcmode = m_calcmode;
		rate = m_per / 100.0f;
	}else{
		setpaintmode = PAINT_EXC;
		setcalcmode = CALCMODE_NOSKIN0;
		rate = 1.0f;
	}

	if( m_calcmode == CALCMODE_DIRECT0 ){
		ret = GetFloat( m_direct_wnd, &m_directval );
		if( ret || (m_directval < 0.0f) ){
			::MessageBox( m_hWnd, "影響度の直接指定には、正の小数を指定してください。", "入力エラー", MB_OK );
			return 0;
		}
	}

	int vno;
	for( vno = 0; vno < m_selvertnum; vno++ ){
		ret = m_papp->m_shandler->SetInfElem( m_seldisp, m_selvert[vno], m_selbone, setpaintmode, setcalcmode, rate, m_check_normalize, m_directval );
		_ASSERT( !ret );
//		if( ret ){
//			if( ret != 2 ){
//				DbgOut( "paintdlg : SetInfElem : sh SetInfElem error !!!\n" );
//				_ASSERT( 0 );
//				return 1;
//			}else{//ret 2 は、５個以上の影響度設定をしようとして失敗したとき。
//				DbgOut( "paintdlg : warning : SetInfElem sh SetInfElem : over 5 influence warning !!!\n" );
//				::MessageBox( m_hWnd, "影響度は１つの頂点に４個までしか設定できません。", "失敗", MB_OK );
//			}
//		}
	}

	if( m_dispmode == DISP_BONE ){
		m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
	}else{
		m_papp->m_shandler->SetMaterialFromInfElem( -1, m_seldisp, -1 );

	}
	InitList();


	ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
	if( ret ){
		DbgOut( "paintdlg : SetInfElem : sh CreateSkinMat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CPaintDlg::DeleteInfElem( int* pinfnum )
{
	int ret;
	if( (m_selbone <= 0) || (m_seldisp <= 0) || (m_selvertnum <= 0) ){
		return 0;
	}

	int vno;
	for( vno = 0; vno < m_selvertnum; vno++ ){
		int infnum;
		infnum = *(pinfnum + vno);
		if( infnum >= 2 ){
			ret = m_papp->m_shandler->DeleteInfElem( m_seldisp, m_selvert[vno], m_selbone, m_check_normalize );
			if( ret ){
				DbgOut( "paintdlg : SetInfElem : sh DeleteInfElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	if( m_dispmode == DISP_BONE ){
		m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
	}else{
		m_papp->m_shandler->SetMaterialFromInfElem( -1, m_seldisp, -1 );

	}
	InitList();

	ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
	if( ret ){
		DbgOut( "paintdlg : DeleteInfElem : sh CreateSkinMat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

LRESULT CPaintDlg::OnRadioSetinf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_SETINF );
	if( checkflag == BST_CHECKED ){
		m_papp->m_selectedno = 0;//timerが機能する前に初期化
		m_papp->m_shiftpart = 0;
		m_papp->m_shifvertnum = 0;

		m_ope = PAINTOPE_SETINF;

		g_inidata->opt_polydisp = POLYDISP_SEL;//!!!!!!!!!!!!!!
		if( m_doi && (m_seldisp > 0) && m_papp->m_shandler && (m_papp->m_shandler->s2shd_leng > 2) ){
			ret = m_papp->m_shandler->SetIsSelected( m_seldisp, 0, 0 );
			_ASSERT( !ret );
			
			m_papp->m_shandler->SetDispFlag( 1 );//!!!!!!!!!!!!!!
			(*m_papp->m_shandler)( m_seldisp )->dispflag = 1;//!!!!!!!!!!!
		}


	}else{
		m_ope = PAINTOPE_BONE;

		m_papp->m_selectedno = 0;//timerが止まった後で初期化
		m_papp->m_shiftpart = 0;
		m_papp->m_shifvertnum = 0;

		g_inidata->opt_polydisp = POLYDISP_ALL;//!!!!!!!!!!!!!!

	}

	return 0;
}
LRESULT CPaintDlg::OnRadioBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_SETINF );
	if( checkflag == BST_CHECKED ){
		m_papp->m_selectedno = 0;//timerが機能する前に初期化
		m_papp->m_shiftpart = 0;
		m_papp->m_shifvertnum = 0;

		m_ope = PAINTOPE_SETINF;

		g_inidata->opt_polydisp = POLYDISP_SEL;//!!!!!!!!!!!!!!
		if( m_doi && (m_seldisp > 0) && m_papp->m_shandler && (m_papp->m_shandler->s2shd_leng > 2) ){
			ret = m_papp->m_shandler->SetIsSelected( m_seldisp, 0, 0 );
			_ASSERT( !ret );
			m_papp->m_shandler->SetDispFlag( 1 );//!!!!!!!!!!!!!!
			(*m_papp->m_shandler)( m_seldisp )->dispflag = 1;//!!!!!!!!!!!

		}

	}else{
		m_ope = PAINTOPE_BONE;

		m_papp->m_selectedno = 0;//timerが止まった後で初期化
		m_papp->m_shiftpart = 0;
		m_papp->m_shifvertnum = 0;

		g_inidata->opt_polydisp = POLYDISP_ALL;//!!!!!!!!!!!!!!
	}

	return 0;
}

LRESULT CPaintDlg::OnChkNorm(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_check_normalize )
		m_check_normalize = 0;
	else
		m_check_normalize = 1;

	ParamsToDlg();

	return 0;
}


LRESULT CPaintDlg::OnUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	if( m_undopos <= 0 ){
		::MessageBox( m_dlg_wnd, "バッファが空です。Undo出来ません。", "Undo出来ません。", MB_OK );
		return 0;
	}
	
	m_undopos--;

	if( (m_undo[m_undopos].vertnum <= 0) || (m_undo[m_undopos].partno <= 0) )
		return 0;//!!!!!!!!!


	int vno;
	for( vno = 0; vno < m_undo[m_undopos].vertnum; vno++ ){
		ret = m_papp->m_shandler->DestroyIE( m_undo[m_undopos].partno, *(m_undo[m_undopos].pvertno + vno) );
		if( ret ){
			DbgOut( "paintdlg : OnUndo : sh DestroyIE error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int infno;
		for( infno = 0; infno < *(m_undo[m_undopos].pinfnum + vno); infno++ ){
			ret = m_papp->m_shandler->AddInfElem( m_undo[m_undopos].partno, 
			//	*(m_undo[m_undopos].pvertno + vno), *(m_undo[m_undopos].pinf + infno) );
			*(m_undo[m_undopos].pvertno + vno), *(m_undo[m_undopos].pinf + INFNUMMAX * vno + infno) );
			if( ret ){
				DbgOut( "paintdlg : OnUndo : sh AddInfElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}


	m_seldisp = m_undo[m_undopos].partno;
	m_selbone = m_undo[m_undopos].selbone;
	m_selvertnum = m_undo[m_undopos].vertnum;
	MoveMemory( m_selvert, m_undo[m_undopos].pvertno, sizeof( int ) * m_selvertnum );

	if( m_selbone > 0 ){
		TreeView_Select( m_tree_wnd, *( m_TI + m_selbone ), TVGN_CARET );
		if( m_dispmode == DISP_BONE ){
			m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_undo[m_undopos].partno, -1 );	
		}else{
			m_papp->m_shandler->SetMaterialFromInfElem( -1, m_undo[m_undopos].partno, -1 );
		
		}
	}

	InitList();

	ret = m_papp->m_shandler->CreateSkinMat( m_undo[m_undopos].partno, 1 );
	if( ret ){
		DbgOut( "paintdlg : CopySymInf : sh CreateSkinMat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

int CPaintDlg::InitUndoBuf()
{
	m_undopos = 0;

	int undono;
	for( undono = 0; undono < INFPAINTUNDOMAX; undono++ ){
		m_undo[undono].partno = -1;
		m_undo[undono].vertnum = 0;
		if( m_undo[undono].pvertno ){
			free( m_undo[undono].pvertno );
			m_undo[undono].pvertno = 0;
		}
		m_undo[undono].selbone = -1;
		if( m_undo[undono].pinfnum ){
			free( m_undo[undono].pinfnum );
			m_undo[undono].pinfnum = 0;
		}
		if( m_undo[undono].pinf ){
			free( m_undo[undono].pinf );
			m_undo[undono].pinf = 0;
		}
	}

	return 0;
}

int CPaintDlg::AddUndoBuf( int partno, int* pvertno, int vertnum, int selbone, int* pinfnum, INFELEM* pinf )
{
//	if( infnum > INFNUMMAX ){
//		DbgOut( "paintdlg : AddUndoBuf : infnum error !!!\n" );
//		_ASSERT( 0 );
//		return 1;
//	}

	int fullflag = 0;

	if( m_undopos <= ( INFPAINTUNDOMAX - 1 ) ){
		//そのまま		
	}else{
		m_undo[0].partno = -1;
		m_undo[0].vertnum = 0;
		if( m_undo[0].pvertno ){
			free( m_undo[0].pvertno );
			m_undo[0].pvertno = 0;
		}
		m_undo[0].selbone = -1;
		if( m_undo[0].pinfnum ){
			free( m_undo[0].pinfnum );
			m_undo[0].pinfnum = 0;
		}
		if( m_undo[0].pinf ){
			free( m_undo[0].pinf );
			m_undo[0].pinf = 0;
		}


		int undono;
		for( undono = 1; undono < INFPAINTUNDOMAX; undono++ ){
			m_undo[undono - 1] = m_undo[undono];
		}
		m_undopos = INFPAINTUNDOMAX - 1;
		
		fullflag = 1;
	}

	if( fullflag == 0 ){
		if( m_undo[m_undopos].pvertno ){
			free( m_undo[m_undopos].pvertno );
			m_undo[m_undopos].pvertno = 0;
		}
		if( m_undo[m_undopos].pinfnum ){
			free( m_undo[m_undopos].pinfnum );
			m_undo[m_undopos].pinfnum = 0;
		}
		if( m_undo[m_undopos].pinf ){
			free( m_undo[m_undopos].pinf );
			m_undo[m_undopos].pinf = 0;
		}
	}

	m_undo[m_undopos].partno = partno;

	m_undo[m_undopos].vertnum = vertnum;
	m_undo[m_undopos].pvertno = pvertno;//!!!!!!!

	m_undo[m_undopos].selbone = selbone;

	m_undo[m_undopos].pinfnum = pinfnum;//!!!!!!!
	m_undo[m_undopos].pinf = pinf;//!!!!!!!!!!!!!
	
	m_undopos++;

	return 0;
}

LRESULT CPaintDlg::OnDef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	COneOrAllDlg dlg;
	int ret;

	ret = (int)dlg.DoModal();
	if( ret != IDOK )
		return 0;

	if( dlg.m_isone == 0 ){
		ret = m_papp->m_shandler->CalcInfElem( -1, 1 );
		if( ret ){
			DbgOut( "paintdlg : OnDef : sh CalcInfElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = m_papp->m_shandler->UpdateVertexBuffer( 0 );
		if( ret ){
			DbgOut( "paintdlg : OnDef : sh UpdateVertexBuffer error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{
		if( (m_seldisp > 0) && (m_selvertnum > 0) ){

			int vno;
			for( vno = 0; vno < m_selvertnum; vno++ ){
				ret = m_papp->m_shandler->CalcInfElem1Vert( m_seldisp, m_selvert[vno] );
				if( ret ){
					DbgOut( "paintdlg : OnDef : sh CalcInfElem1Vert error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}

			ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
			if( ret ){
				DbgOut( "paintdlg : CopySymInf : sh CreateSkinMat error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


		}
	}

	if( m_dispmode == DISP_BONE ){
		m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, -1, -1 );
	}else{
		m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );

	}
	InitList();


	return 0;
}
LRESULT CPaintDlg::OnNormalize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	COneOrAllDlg dlg;
	int ret;

	ret = (int)dlg.DoModal();
	if( ret != IDOK )
		return 0;

	if( dlg.m_isone == 0 ){
		ret = m_papp->m_shandler->NormalizeInfElem( m_seldisp, -1 );
		if( ret ){
			DbgOut( "paintdlg : OnNormalize : sh NormalizeInfElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		if( (m_seldisp > 0) && (m_selvertnum > 0) ){

			int vno;
			for( vno = 0; vno < m_selvertnum; vno++ ){
				ret = m_papp->m_shandler->NormalizeInfElem( m_seldisp, m_selvert[vno] );
				if( ret ){
					DbgOut( "paintdlg : OnNormalize : sh NormalizeInfElem error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
		}
	}

	if( m_dispmode == DISP_BONE ){
		m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, -1, -1 );
	}else{
		m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );

	}
	InitList();

	ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
	if( ret ){
		DbgOut( "paintdlg : CopySymInf : sh CreateSkinMat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}



	return 0;
}

int CPaintDlg::Conv2SymIE( INFELEM* srcIE, int infnum, INFELEM* dstIE )
{
	//int ret;
	int infno;
	INFELEM* cursrc;
	INFELEM* curdst;

	int childno;
	int symchildno;

	CTreeElem2* childte;
	int childleng;

	int patleng1, patleng2, patleng3, patleng4;
	int cmp1, cmp2, cmp3, cmp4;

	for( infno = 0; infno < infnum; infno++ ){
		int childsymkind = -1;//1:xp, 2:xm, 3:mikoxp, 4:mikoxm, 5:yp, 6:ym, 7:zp, 8:zm
		int bonesymkind = -1;

		cursrc = srcIE + infno;
		curdst = dstIE + infno;

		childno = cursrc->childno;

		if( childno > 0 ){
			childte = (*m_papp->m_thandler)( childno );
			childleng = (int)strlen( childte->name );
		}else{
			childte = 0;
			childleng = 0;
		}

		if( m_symaxis == SYMAXIS_X ){
			patleng1 = (int)strlen( strsymmtype[SYMMTYPE_XP] );
			if( childleng > patleng1 ){
				cmp1 = strcmp( childte->name + childleng - patleng1, strsymmtype[SYMMTYPE_XP] );
				if( cmp1 == 0 ){
					childsymkind = 1;
				}
			}
			patleng2 = (int)strlen( strsymmtype[SYMMTYPE_XM] );
			if( childleng > patleng2 ){
				cmp2 = strcmp( childte->name + childleng - patleng2, strsymmtype[SYMMTYPE_XM] );
				if( cmp2 == 0 ){
					childsymkind = 2;
				}
			}
			patleng3 = (int)strlen( strsymmtype2[SYMMTYPE_XP] );
			if( childleng > patleng3 ){
				cmp3 = strcmp( childte->name + childleng - patleng3, strsymmtype2[SYMMTYPE_XP] );
				if( cmp3 == 0 ){
					childsymkind = 3;
				}
			}
			patleng4 = (int)strlen( strsymmtype2[SYMMTYPE_XM] );
			if( childleng > patleng4 ){
				cmp4 = strcmp( childte->name + childleng - patleng4, strsymmtype2[SYMMTYPE_XM] );
				if( cmp4 == 0 ){
					childsymkind = 4;
				}
			}
		}else if( m_symaxis == SYMAXIS_Y ){
			patleng1 = (int)strlen( strsymmtype[SYMMTYPE_YP] );
			if( childleng > patleng1 ){
				cmp1 = strcmp( childte->name + childleng - patleng1, strsymmtype[SYMMTYPE_YP] );
				if( cmp1 == 0 ){
					childsymkind = 5;
				}
			}
			patleng2 = (int)strlen( strsymmtype[SYMMTYPE_YM] );
			if( childleng > patleng2 ){
				cmp2 = strcmp( childte->name + childleng - patleng2, strsymmtype[SYMMTYPE_YM] );
				if( cmp2 == 0 ){
					childsymkind = 6;
				}
			}
		}else if( m_symaxis == SYMAXIS_Z ){
			patleng1 = (int)strlen( strsymmtype[SYMMTYPE_ZP] );
			if( childleng > patleng1 ){
				cmp1 = strcmp( childte->name + childleng - patleng1, strsymmtype[SYMMTYPE_ZP] );
				if( cmp1 == 0 ){
					childsymkind = 7;
				}
			}
			patleng2 = (int)strlen( strsymmtype[SYMMTYPE_ZM] );
			if( childleng > patleng2 ){
				cmp2 = strcmp( childte->name + childleng - patleng2, strsymmtype[SYMMTYPE_ZM] );
				if( cmp2 == 0 ){
					childsymkind = 8;
				}
			}
		}else{
			_ASSERT( 0 );
		}


//DbgOut( "paintdlg : Conv2SymIE : kind %d, te name %s\r\n", childte->name );


		symchildno = GetSymJointNo( childte->name, childno, childsymkind );

		int symboneno;
		if( symchildno > 0 ){
			CShdElem* childse;
			childse = (*m_papp->m_shandler)( symchildno );
			CShdElem* parentse;
			parentse = m_papp->m_shandler->FindUpperJoint( childse );
			if( parentse ){
				symboneno = parentse->serialno;
			}else{
				symboneno = -1;
			}
		}else{
			symboneno = -1;
		}


		*curdst = *cursrc;
		curdst->kind = CALCMODE_SYM;
		curdst->childno = symchildno;
		//curdst->bonematno = symboneno;
		curdst->bonematno = symchildno;//!!!!!!!!

	}

	return 0;
}

int CPaintDlg::GetSymJointNo( char* srcname, int srcno, int symkind )
{
	int symno = srcno;//!!!!!!!!!!!!!!

	char findname[ 2048 ];
	int patleng;

	int namelen;
	namelen = (int)strlen( srcname );
	if( namelen >= 2048 ){
		DbgOut( "paintdlg : GetSymJointNo : namelen too long error !!!\n" );
		_ASSERT( 0 );
		return symno;//!!!!!!!!!!!!!!
	}


	switch( symkind ){
	case 1:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype[SYMMTYPE_XP] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype[SYMMTYPE_XM] );

		break;
	case 2:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype[SYMMTYPE_XM] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype[SYMMTYPE_XP] );

		break;
	case 3:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype2[SYMMTYPE_XP] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype2[SYMMTYPE_XM] );

		break;
	case 4:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype2[SYMMTYPE_XM] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype2[SYMMTYPE_XP] );

		break;
	case 5:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype[SYMMTYPE_YP] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype[SYMMTYPE_YM] );

		break;
	case 6:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype[SYMMTYPE_YM] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype[SYMMTYPE_YP] );

		break;
	case 7:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype[SYMMTYPE_ZP] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype[SYMMTYPE_ZM] );

		break;
	case 8:
		strcpy_s( findname, 2048, srcname );
		patleng = (int)strlen( strsymmtype[SYMMTYPE_ZM] );
		strcpy_s( findname + namelen - patleng, 2048 - namelen + patleng, strsymmtype[SYMMTYPE_ZP] );

		break;
	default:
		strcpy_s( findname, 2048, srcname );
		break;
	}

	int ret;
	ret = m_papp->m_thandler->GetBoneNoByName( findname, &symno, m_papp->m_shandler, 0 );
	if( ret ){
		DbgOut( "patindlg : GetSymJointNo : th GetBoneNoByName error !!!\n" );
		_ASSERT( 0 );
		symno = srcno;
	}
	if( symno <= 0 )
		symno = srcno;


//DbgOut( "paintdlg : GetSymJointNo : kind %d, findname %s\r\n", symkind, findname );

	return symno;
	
}

/***
LRESULT CPaintDlg::OnBeginLabelEdit(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	bHandled = 1;

	//LV_DISPINFO* lvinfo;
    //lvinfo = (LV_DISPINFO *)pnmh;
    m_editwnd = ListView_GetEditControl(m_list_wnd);

	return 0;
}
LRESULT CPaintDlg::OnEndLabelEdit(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	bHandled = 1;

	LV_DISPINFO* lvinfo;
	char buf[1024];

	if( !m_editwnd ){
		return 0;
	}

    lvinfo = (LV_DISPINFO *)pnmh;
	::GetWindowText(m_editwnd, buf, sizeof(buf));
    ListView_SetItemText(m_list_wnd, lvinfo->item.iItem, 0, buf);

	return 0;
}
***/



LRESULT CPaintDlg::OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 1;

	return 0;
}
LRESULT CPaintDlg::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_moving = 0;

	return 0;
}

LRESULT CPaintDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	RECT viewclient;
	m_list_wnd.GetClientRect( &viewclient );

	POINT inview;
	::GetCursorPos( &inview );
	m_list_wnd.ScreenToClient( &inview );

	if( (m_mcap == 0) && (m_cmdshow != SW_HIDE) ){
		if( (inview.x >= viewclient.left) && (inview.x <= viewclient.right) && (inview.y >= viewclient.top) && (inview.y <= viewclient.bottom) ){
			SetCapture();
			//DbgOut( "lightdlg : MouseMove : SetCapture !!!!!!!!!!!!!!\r\n" );
			m_mcap = 1;
		}
	}else{
		if( (m_moving == 0) && (m_cmdshow != SW_HIDE) ){
			if( (inview.x < viewclient.left) || (inview.x > viewclient.right) || (inview.y < viewclient.top) || (inview.y > viewclient.bottom) ){
			//if( (indlg.x < dlgclient.left) || (indlg.x > dlgclient.right) || (indlg.y < dlgclient.top) || (indlg.y > dlgclient.bottom) ){
				ReleaseCapture();
				//DbgOut( "lightdlg: MouseMove : ReleaseCapture !!!!!!!!!!!\r\n" );
				m_mcap = 0;
				m_lbutton = 0;//!!!!!!!!!!
			}
		}
	}

	return 0;
}

LRESULT CPaintDlg::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//int ret;
	RECT viewclient;
	m_list_wnd.GetClientRect( &viewclient );

	POINT inview;
	::GetCursorPos( &inview );
	m_list_wnd.ScreenToClient( &inview );

	RECT itemrect;
	ZeroMemory( &itemrect, sizeof( RECT ) );
	ListView_GetItemRect( m_list_wnd, 0, &itemrect, LVIR_BOUNDS );

	if( (m_brushtype == BRUSH_SPH) && (inview.x >= viewclient.left) && (inview.x <= viewclient.right) && (inview.y >= itemrect.top) && (inview.y <= viewclient.bottom) ){
		//topは、itemrect.topとの比較！！！

		int hititem = -1;
		int hitsubitem = -1;

		LVHITTESTINFO hitinfo;
		hitinfo.pt = inview;
		hitinfo.flags = LVHT_ONITEM | LVHT_ONITEMLABEL;
		hitinfo.iItem = -1;
		hitinfo.iSubItem = -1;

		int hitindex = -1;
		hitindex = ListView_SubItemHitTest( m_list_wnd, &hitinfo );
		if( (hitinfo.iItem >= 0) && (hitinfo.iSubItem >= 0) ){
			hititem = hitinfo.iItem;
			hitsubitem = hitinfo.iSubItem;

			LVITEM lvitem;
			ZeroMemory( &lvitem, sizeof( LVITEM ) );
			char tempname[256];
			ZeroMemory( tempname, 256 );

			lvitem.iItem = hititem;
			lvitem.iSubItem = hitsubitem;//!!!!!!!!!!
			lvitem.pszText = tempname;
			lvitem.cchTextMax = 256;

			lvitem.mask = LVIF_TEXT | LVIF_PARAM;
			
			BOOL bret;
			bret = ListView_GetItem( m_list_wnd, &lvitem );
			if( bret == TRUE ){
				m_markpart = m_seldisp;
				m_markvert = (int)lvitem.lParam;
			}else{
				m_markpart = -1;
				m_markvert = -1;
			}
		}else{
			m_markpart = -1;
			m_markvert = -1;
		}
	}

	return 0;
}



LRESULT CPaintDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !m_papp->m_shandler || !m_papp->m_thandler )
		return 0;

	int ret;
	RECT viewclient;
	m_list_wnd.GetClientRect( &viewclient );

	POINT inview;
	::GetCursorPos( &inview );
	m_list_wnd.ScreenToClient( &inview );


//DbgOut( "paintdlg : LButtonDown : %d %d\r\n", inview.x, inview.y );

	m_lbutton = 1;//!!!!!!!!!!!!!!

	RECT itemrect;
	ZeroMemory( &itemrect, sizeof( RECT ) );
	ListView_GetItemRect( m_list_wnd, 0, &itemrect, LVIR_BOUNDS );

//DbgOut( "paintdlg: OnLButton : itemrect left %d right %d top %d bottom %d\r\n",
//	itemrect.left, itemrect.right, itemrect.top, itemrect.bottom );



	if( (inview.x >= viewclient.left) && (inview.x <= viewclient.right) && (inview.y >= itemrect.top) && (inview.y <= viewclient.bottom) ){
		//topは、itemrect.topとの比較！！！
		if( m_brushtype == BRUSH_1VERT ){
			ret = ListProp1Vert( inview );
		}else{
			ret = ListPropMulti( inview );
		}
		_ASSERT( !ret );
	}

	return 0;
}
int CPaintDlg::ListPropMulti( POINT inview )
{
	if( m_columnnum <= 0 ){
		return 0;
	}


	int ret;

	int hititem = -1;
	int hitsubitem = -1;

	LVHITTESTINFO hitinfo;
	hitinfo.pt = inview;
	hitinfo.flags = LVHT_ONITEM | LVHT_ONITEMLABEL;
	hitinfo.iItem = -1;
	hitinfo.iSubItem = -1;

	int hitindex = -1;
	hitindex = ListView_SubItemHitTest( m_list_wnd, &hitinfo );
	if( (hitinfo.iItem >= 0) && (hitinfo.iSubItem >= 0) ){
		hititem = hitinfo.iItem;
		hitsubitem = hitinfo.iSubItem;

		LVITEM lvitem;
		ZeroMemory( &lvitem, sizeof( LVITEM ) );
		char tempname[256];
		ZeroMemory( tempname, 256 );

		lvitem.iItem = hititem;
		lvitem.iSubItem = hitsubitem;//!!!!!!!!!!
		lvitem.pszText = tempname;
		lvitem.cchTextMax = 256;

		lvitem.mask = LVIF_TEXT | LVIF_PARAM;
		
		BOOL bret;
		bret = ListView_GetItem( m_list_wnd, &lvitem );
		if( bret == TRUE ){
			int vertno = (int)lvitem.lParam;
			int findseri = -1;
			int seri;
			int subid = 1;
			for( seri = 0; seri < m_papp->m_shandler->s2shd_leng; seri++ ){
				if( *( m_TI + seri ) ){
					if( subid == hitsubitem ){
						findseri = seri;
						break;
					}
					subid++;
				}
			}
			
			if( findseri <= 0 ){
				return 0;
			}

			int infnum;
			ret = m_papp->m_shandler->GetInfElemNum( m_seldisp, vertno, &infnum );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			INFELEM tempie[INFNUMMAX];
			ZeroMemory( tempie, sizeof( INFELEM ) * INFNUMMAX );
			int infno;
			for( infno = 0; infno < infnum; infno++ ){
				INFELEM* pIE;
				ret = m_papp->m_shandler->GetInfElem( m_seldisp, vertno, infno, &pIE );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
				tempie[infno] = *pIE;
			}

			CEditListInfDlg dlg( m_papp, m_seldisp, vertno, findseri, tempie, infnum );
			int dlgret;
			dlgret = (int)dlg.DoModal();
			if( dlgret == IDOK ){
				if( dlg.m_val != 0.0f ){				
					ret = m_papp->m_shandler->SetInfElem( m_seldisp, vertno, findseri, PAINT_NOR, CALCMODE_DIRECT0, 
						1.0f, m_check_normalize, dlg.m_val );
					if( ret ){
						if( ret != 2 ){
							_ASSERT( 0 );
							return 1;
						}else{//ret 2 は、５個以上の影響度設定をしようとして失敗したとき。
							::MessageBox( m_hWnd, "影響度は１つの頂点に４個までしか設定できません。\n必要ないものを削除してから再試行してください。", "失敗", MB_OK );
						}
					}
				}else{

					if( infnum <= 1 ){
						::MessageBox( m_hWnd, "影響ボーンの数を０個にすることは出来ません。\n必要なボーンの影響度を追加してから再試行してください。", "失敗", MB_OK );
					}else{
						ret = m_papp->m_shandler->DeleteInfElem( m_seldisp, vertno, findseri, m_check_normalize );
						if( ret ){
							_ASSERT( 0 );
							return 1;
						}
					}
				}


				if( m_dispmode == DISP_BONE ){
					m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, m_seldisp, -1 );
				}else{
					m_papp->m_shandler->SetMaterialFromInfElem( -1, m_seldisp, -1 );

				}
				InitList();
				ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
				if( ret ){
					DbgOut( "paintdlg : SetInfElem : sh CreateSkinMat error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

			}
		}
	}

	return 0;
}


int CPaintDlg::ListProp1Vert( POINT inview )
{
	int ret;
	int hititem = -1;
	int hitsubitem = -1;

	LVHITTESTINFO hitinfo;
	hitinfo.pt = inview;
	hitinfo.flags = LVHT_ONITEM | LVHT_ONITEMLABEL;
	hitinfo.iItem = -1;
	hitinfo.iSubItem = -1;

	int hitindex = -1;
	hitindex = ListView_SubItemHitTest( m_list_wnd, &hitinfo );
	if( (hitinfo.iItem >= 0) && (hitinfo.iSubItem >= 0) ){
		hititem = hitinfo.iItem;
		hitsubitem = hitinfo.iSubItem;

//DbgOut( "paintdlg : OnLButton : hit !!! : item %d, subitem %d\r\n", hititem, hitsubitem );

		LVITEM lvitem;
		ZeroMemory( &lvitem, sizeof( LVITEM ) );
		char tempname[1024];
		ZeroMemory( tempname, 1024 );

		lvitem.iItem = hititem;
		lvitem.iSubItem = hitsubitem;//!!!!!!!!!!
		lvitem.pszText = tempname;
		lvitem.cchTextMax = 1024;

		lvitem.mask = LVIF_TEXT | LVIF_PARAM;
		
		BOOL bret;
		bret = ListView_GetItem( m_list_wnd, &lvitem );
		if( bret == TRUE ){
//DbgOut( "paintdlg : OnLButton : hit !!! : name %s, serial %d\r\n", lvitem.pszText, lvitem.lParam );
			int curserino;
			curserino = (int)lvitem.lParam;
		
			TreeViewSelChange( curserino );//!!!!!!!!!!!!!!!!!!!!

			if( (hitsubitem != SUBID_RATE) && (hitsubitem != SUBID_ORGINF) ){
				return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}


			LVITEM lvitem2;
			ZeroMemory( &lvitem2, sizeof( LVITEM ) );
			char tempname2[1024];
			ZeroMemory( tempname2, 1024 );

			lvitem2.iItem = hititem;
			lvitem2.iSubItem = SUBID_NAME;//!!!!!!!!!!!!
			lvitem2.pszText = tempname2;
			lvitem2.cchTextMax = 1024;

			lvitem2.mask = LVIF_TEXT;
			
			BOOL bret2;
			bret2 = ListView_GetItem( m_list_wnd, &lvitem2 );
			if( bret2 == TRUE ){

				CEditInfDlg dlg( lvitem2.pszText, hitsubitem, lvitem.pszText );
				int dlgret;
				dlgret = (int)dlg.DoModal();
				if( dlgret == IDOK ){
					ListView_SetItemText( m_list_wnd, hititem, hitsubitem, dlg.m_textval );
					
					int vno;
					INFELEM* curIE;
					if( hitsubitem == SUBID_RATE ){
						for( vno = 0; vno < m_selvertnum; vno++ ){
							
							ret = m_papp->m_shandler->GetInfElem( m_seldisp, m_selvert[vno], hititem, &curIE );
							if( ret ){
								DbgOut( "paintdlg : OnLButtonDown : sh GetInfElem error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						
							float setrate = dlg.m_val * 0.01f;
							ret = m_papp->m_shandler->SetInfElem( m_seldisp, m_selvert[vno], m_selbone, 
								PAINT_NOR, curIE->kind, setrate, m_check_normalize, curIE->orginf );
							if( ret ){
								DbgOut( "paintdlg : OnLButtonDown : sh SetInfElem error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}

					}else if( hitsubitem == SUBID_ORGINF ){
						for( vno = 0; vno < m_selvertnum; vno++ ){

							ret = m_papp->m_shandler->GetInfElem( m_seldisp, m_selvert[vno], hititem, &curIE );
							if( ret ){
								DbgOut( "paintdlg : OnLButtonDown : sh GetInfElem error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						
						
							ret = m_papp->m_shandler->SetInfElem( m_seldisp, m_selvert[vno], m_selbone, 
								PAINT_NOR, CALCMODE_DIRECT0, curIE->userrate, m_check_normalize, dlg.m_val );
							if( ret ){
								DbgOut( "paintdlg : OnLButtonDown : sh SetInfElem error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}
					}
				}

				if( m_dispmode == DISP_BONE ){
					m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, -1, -1 );
				}else{
					m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );

				}
				InitList();


				ret = m_papp->m_shandler->CreateSkinMat( m_seldisp, 1 );
				if( ret ){
					DbgOut( "paintdlg : CopySymInf : sh CreateSkinMat error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
		}
	}
	return 0;
}


LRESULT CPaintDlg::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT inview;
	::GetCursorPos( &inview );
	m_list_wnd.ScreenToClient( &inview );


//DbgOut( "paintdlg : LButtonUp : %d %d\r\n", inview.x, inview.y );


	m_lbutton = 0;//!!!!!!!!!!!!!
	
	return 0;
}


LRESULT CPaintDlg::OnRadioDispBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_dispmode = DISP_BONE;

	m_papp->m_shandler->SetMaterialFromInfElem( m_selbone, -1, -1 );

	return 0;
}
LRESULT CPaintDlg::OnRadioDispSym(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_dispmode = DISP_SYM;

	m_papp->m_shandler->SetMaterialFromInfElem( -1, -1, -1 );

	return 0;
}

int CPaintDlg::InitSeldispCombo()
{
	m_seldisp = 0;//!!!!!!!!!!!!!
	DestroyDOI();

	if( !m_seldisp_wnd.IsWindow() )
		return 0;
	m_seldisp_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );

	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler )
		return 0;

	int ret;
	ret = m_papp->GetDispObjNum( &m_dispobjnum );
	_ASSERT( !ret );

	if( m_dispobjnum > 0 ){
		m_doi = (DISPOBJINFO*)malloc( sizeof( DISPOBJINFO ) * m_dispobjnum );
		if( !m_doi ){
			DbgOut( "paintdlg : InitSeldispCombo : doi malloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( m_doi, sizeof( DISPOBJINFO ) * m_dispobjnum );

		int getnum = 0;
		ret = m_papp->GetDispObjInfo( m_doi, m_dispobjnum, &getnum );
		if( ret ){
			DbgOut( "paintdlg : InitSeldispCombo : GetDispObjInfo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( getnum != m_dispobjnum ){
			DbgOut( "paintdlg : InitSeldispCombo : getnum error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int doino;
		for( doino = 0; doino < m_dispobjnum; doino++ ){
			DISPOBJINFO* curdoi;
			curdoi = m_doi + doino;
			m_seldisp_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)curdoi->name );
		}

		m_seldisp = m_doi->serialno;
		m_seldisp_wnd.SendMessage( CB_SETCURSEL, 0, 0 );

	}

	return 0;
}
int CPaintDlg::InitBrushCombo()
{
	if( !m_brush_wnd.IsWindow() )
		return 0;
	m_brush_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );

	m_brush_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"１頂点" );
	m_brush_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"球形ブラシ" );
	
	m_brushtype = BRUSH_SPH;
	m_brush_wnd.SendMessage( CB_SETCURSEL, m_brushtype, 0 );


	return 0;
}

LRESULT CPaintDlg::OnSeldisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int combono;
	combono = (int)m_seldisp_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	if( (combono < 0) || (combono >= m_dispobjnum) ){
		return 0;
	}
	m_seldisp = (m_doi + combono)->serialno;

	if( m_doi && (m_seldisp > 0) && m_papp->m_shandler && (m_papp->m_shandler->s2shd_leng > 2) ){
		ret = m_papp->m_shandler->SetIsSelected( m_seldisp, 0, 0 );
		_ASSERT( !ret );
		m_papp->m_shandler->SetDispFlag( 1 );//!!!!!!!!!!!!!!
		(*m_papp->m_shandler)( m_seldisp )->dispflag = 1;//!!!!!!!!!!!

	}

	return 0;
}
LRESULT CPaintDlg::OnBrush(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int combono;
	combono = (int)m_brush_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	if( (combono < 0) || (combono >= BRUSH_MAX) ){
		return 0;
	}
	m_brushtype = combono;

	return 0;
}

LRESULT CPaintDlg::OnApplyBrushSize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	float tmpfloat;
	ret = GetFloat( m_brushsize_wnd, &tmpfloat );
	if( ret || (tmpfloat < 0.0f) ){
		::MessageBox( m_hWnd, "ブラシサイズには、正の小数を指定してください。", "入力エラー", MB_OK );
		return 0;
	}

	m_brushsize = tmpfloat;

	return 0;
}


LRESULT CPaintDlg::OnZeroVert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	int ret;

	if( m_seldisp <= 0 ){
		return 0;
	}

	m_selvertnum = 0;

	CShdElem* selem = (*(m_papp->m_shandler))( m_seldisp );
	_ASSERT( selem );

	int vnum = 0;
	CInfElem* infptr = 0;
	if( selem->type == SHDPOLYMESH ){
		infptr = selem->polymesh->m_IE;
		vnum = selem->polymesh->meshinfo->m;
	}else if( selem->type == SHDPOLYMESH2 ){
		infptr = selem->polymesh2->m_IE;
		vnum = selem->polymesh2->optpleng;
	}

	int chkvno;
	for( chkvno = 0; chkvno < vnum; chkvno++ ){
		CInfElem* curie = infptr + chkvno;

		float infsum = 0.0f;
		int ieno;
		for( ieno = 0; ieno < curie->infnum; ieno++ ){
			infsum += (curie->ie + ieno)->dispinf;
		}

		if( infsum == 0.0f ){
			if( m_selvertnum >= VERTNUMMAX ){
				_ASSERT( 0 );
				return 1;
			}
			m_selvert[ m_selvertnum ] = chkvno;
			m_selvertnum++;
		}
	}

	if( m_selvertnum <= 0 ){
		return 0;
	}

	ret = ClickOpe( 1, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

