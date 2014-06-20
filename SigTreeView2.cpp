// SigTreeView2.cpp : CSigTreeView2 �̎���
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"
#include "RokDeBone2.h"
#include "SigTreeView2.h"

#pragma comment(lib, "comctl32.lib")

#include "ShdParamDlg.h"
#include "JointLocDlg.h"
#include "NewJointDlg.h"
#include "InfluenceDlg.h"
#include "SelScopeDlg.h"

#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <D3DX9.h>

//#define NOTWINDOWSH
#include <coef.h>

#define DBGH
#include <dbg.h>

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <mothandler.h>

#include <sigfile.h>

// CSigTreeView2

//CSigTreeView2::CSigTreeView2() :	
//	m_ctlSysTreeView32(_T("SysTreeView32"), this, 1)
CSigTreeView2::CSigTreeView2()
{
	m_ctlSysTreeView32 = 0;

	m_bWindowOnly = TRUE;

	m_thandler = 0;
	m_shandler = 0;
	m_mhandler = 0;

	m_hImageList = NULL;
	//m_iImage = -1;
	//m_iSelect = -1;
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

	m_apphwnd = 0;

	m_invalidate_handler = 0;
	m_onitembutton = 0;


}

CSigTreeView2::~CSigTreeView2()
{
	FinalRelease();
}


void CSigTreeView2::FinalRelease()
{
	if( m_ctlSysTreeView32 ){
		if( ::IsWindow( m_ctlSysTreeView32->m_hWnd ) ){
			m_ctlSysTreeView32->DestroyWindow();
		}
		delete m_ctlSysTreeView32;
		m_ctlSysTreeView32 = 0;
	}

	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}


	ImageList_Destroy( m_hImageList );
	//DestroyHandler();
	//CloseDbgFile();

}

LRESULT CSigTreeView2::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
		
	RECT rc;
	GetWindowRect(&rc);
	rc.right -= rc.left;
	rc.bottom -= rc.top;
	rc.top = rc.left = 0;
	//rc.top = rc.left = 50;
	InitCommonControls();
	//m_ctlSysTreeView32.Create(m_hWnd, rc);


	///////////////////


	HBITMAP hBitmap;
	m_hImageList = ImageList_Create( 16, 16, ILC_COLOR, 6, 10 );
	if( m_hImageList == NULL ){
		DbgOut( "SigTreeView2 : OnCreate : ImageList_Create error !!!\n" );
	}
	
	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_PART_IMAGE ) );
	if( hBitmap == NULL ){
		DbgOut( "SigTreeView2 : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_PartImageNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_PartImageNo == -1 ){
		DbgOut( "SigTreeView2 : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_PART_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "SigTreeView2 : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_PartSelectNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_PartSelectNo == -1 ){
		DbgOut( "SigTreeView2 : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );

	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_DISP_IMAGE ) );
	if( hBitmap == NULL ){
		DbgOut( "SigTreeView2 : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_DispImageNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_DispImageNo == -1 ){
		DbgOut( "SigTreeView2 : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_DISP_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "SigTreeView2 : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_DispSelectNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_DispSelectNo == -1 ){
		DbgOut( "SigTreeView2 : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_IMAGE ) );
	if( hBitmap == NULL ){
		DbgOut( "SigTreeView2 : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_BoneImageNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_BoneImageNo == -1 ){
		DbgOut( "SigTreeView2 : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


	hBitmap = LoadBitmap( (HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE( IDB_BONE_SELECT ) );
	if( hBitmap == NULL ){
		DbgOut( "SigTreeView2 : OnCreate : LoadBitmap error !!!\n" );		
	}
	m_BoneSelectNo = ImageList_Add( m_hImageList, hBitmap, (HBITMAP)0 );
	if( m_BoneSelectNo == -1 ){
		DbgOut( "SigTreeView2 : OnCreate : ImageList_Add error !!!\n" );	
	}
	DeleteObject( hBitmap );


//	HIMAGELIST retIL;
//	retIL = TreeView_SetImageList( m_ctlSysTreeView32.m_hWnd,
//		m_hImageList, TVSIL_NORMAL );
	//if( retIL == NULL ){
	//	DbgOut( "SigTreeView2 : OnCreate : TreeView_SetImageList error !!!\n" );	
	//}


	return 0;
}

STDMETHODIMP CSigTreeView2::SetObjectRects(LPCRECT prcPos,LPCRECT prcClip)
{
	if( !m_ctlSysTreeView32 ){
		return 0;
	}

	IOleInPlaceObjectWindowlessImpl<CSigTreeView2>::SetObjectRects(prcPos, prcClip);
	int cx, cy;
	cx = prcPos->right - prcPos->left;
	cy = prcPos->bottom - prcPos->top;
	::SetWindowPos(m_ctlSysTreeView32->m_hWnd, NULL, 0,
		0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);

	return S_OK;
}
int CSigTreeView2::FillTree()
{
	if( !m_thandler || !m_shandler || !m_mhandler ){
		_ASSERT( 0 );
		return 0;
	}
	if( !m_ctlSysTreeView32 ){
		return 0;
	}

	HTREEITEM hRoot = TreeView_GetRoot( m_ctlSysTreeView32->m_hWnd );
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
			TreeView_Expand( m_ctlSysTreeView32->m_hWnd, *curitem, TVE_EXPAND );
		}
	}
	TreeView_SelectSetFirstVisible( m_ctlSysTreeView32->m_hWnd, m_selecteditem );

	return 0;
}

void CSigTreeView2::AddTree( int srcseri, int addbroflag, int addtolast )
{
	//addtolast�͏����TVAdd�݂̂𐧌䂷��B
	//( CShdElem��Tree�\���Ɠ������邽�߁B)

	if( !m_ctlSysTreeView32 ){
		return;
	}


	char* shdname;
	int parentno;
	CShdElem* curselem;
	CShdElem* parselem;
	HTREEITEM parTI;

	curselem = (*m_shandler)( srcseri );
	shdname = (*m_thandler)( srcseri )->name;

	if( (curselem->type != SHDDESTROYED) && (curselem->type != SHDINFSCOPE) && (curselem->type != SHDBBOX) ){

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
			DbgOut( "SigTreeView2 : AddTree : TVAdd error : srcseri %d, parTI %x!!!\n",
				srcseri, parTI );
			_ASSERT( 0 );
			return;
		}
		*(m_TI + srcseri) = newTI;

		if( m_selectedno == 0 ){
			m_selectedno = srcseri;
			m_selecteditem = newTI;
			TreeView_SelectItem( m_ctlSysTreeView32->m_hWnd,
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

HTREEITEM CSigTreeView2::TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast )
{

	//������Dbg���͎�菜���Ȃ��B
	DbgOut( "SigTreeView2 : TVAdd : srcname %s, srcno %d, imageno %d, selectno %d, addtolast %d\n",
		srcname, srcno, imageno, selectno, addtolast );

	if( !m_ctlSysTreeView32 ){
		return 0;
	}


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

	return TreeView_InsertItem( m_ctlSysTreeView32->m_hWnd, &tvins );
}

LRESULT CSigTreeView2::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	unsigned char KeyBuffer[256];
	GetKeyboardState( KeyBuffer );


	//if( uMsg == WM_KEYDOWN ){
	//	DbgOut( "SigTreeView2 : OnKeyDown : KEYDOWN : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
	//}else if( uMsg == WM_KEYUP ){
	//	DbgOut( "SigTreeView2 : OnKeyDown : KEYUP : %d, ctrl %d, shift %d\n", (int)wParam, VK_CONTROL, VK_SHIFT );
	//}


	if( (KeyBuffer[VK_CONTROL] & 0x80) && (KeyBuffer['I'] & 0x80) ){
		DbgOut( "CSigTreeView2 : Control-I\n" );

		int ret;
		ret = ShowShdParamDlg();
	}

	return 0;
}



STDMETHODIMP CSigTreeView2::NewJoint()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	int ret;

	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = ::MessageBox( m_hWnd,
		(LPCTSTR)"�W���C���g�̒ǉ������s����ƁA\n�ҏW���̃��[�V�����͎����܂��B\n��낵���ł���?",
		(LPCTSTR)"�m�F�_�C�A���O",
		MB_OKCANCEL );
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK ){
		return 0;
	}	

	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	CNewJointDlg dlg;
	ret = (int)dlg.DoModal( m_hWnd );
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK ){
		return 0;
	}

	LRESULT lres;
	lres = ::SendMessage( m_apphwnd, WM_USER_NEW_JOINT, (WPARAM)dlg.jointnum, 0 );
	if( lres ){
		_ASSERT( 0 );
		return 1;
	}

	//ShowJointLocDlg();

	return S_OK;
}

STDMETHODIMP CSigTreeView2::DelJoint()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	int ret;


	CShdElem* delelem;
	delelem = (*m_shandler)( m_selectedno );
	_ASSERT( delelem );

	if( !delelem->IsJoint() || (delelem->type == SHDMORPH) ){
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"�I�𒆂̗v�f�́A�W���C���g�ł͂Ȃ��̂ŁA�폜�ł��܂���B",
			(LPCTSTR)"�폜�ł��܂���B",
			MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		
		return 0;//!!!!!!!!!!!!!!!
	}


	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = ::MessageBox( m_hWnd,
		(LPCTSTR)"�W���C���g�̍폜�����s����ƁA\n�ҏW���̃��[�V�����͎����܂��B\n��낵���ł���?",
		(LPCTSTR)"�m�F�_�C�A���O",
		MB_OKCANCEL );
	::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK ){
		return 0;
	}	


	LRESULT lres;
	lres = ::SendMessage( m_apphwnd, WM_USER_DEL_JOINT, (WPARAM)m_selectedno, 0 );
	if( lres ){
		_ASSERT( 0 );
		return 1;
	}

	return S_OK;
}



STDMETHODIMP CSigTreeView2::ShowInfluenceDlg()
{
	// TODO: ���̈ʒu�ɃC���v�������g�p�̃R�[�h��ǉ����Ă�������
	
	/***
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );

	if( selem->IsJoint() || (selem->dispflag == 0) ){
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, 
			"���̃I�u�W�F�N�g�́A�\���p�ł͂���܂���B\n�O�p�}�[�N�̃I�u�W�F�N�g��I�����Ă��������B",
			"�I���G���[",
			MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;

	}
	if( selem->notuse ){
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, 
			"���̃I�u�W�F�N�g�́A�����ɂȂ��Ă��܂��B\n�\���p�����[�^�ݒ�ŁA�L���ɂ��Ă���Ď��s���ĉ������B",
			"�I���G���[",
			MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	if( selem->m_loadbimflag != 0 ){
		int dlgret;
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		dlgret = ::MessageBox( m_hWnd, 
			"���̑�����s���ƁAbim�t�@�C���œǂݍ��񂾁A���̃p�[�c�̏�񂪎����܂��B\n��낵���ł����H",
			"�m�F�_�C�A���O",
			MB_OKCANCEL );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );

		if( dlgret != IDOK )
			return 0;
	}

	CInfluenceDlg dlg( m_thandler, m_shandler, m_apphwnd, m_selectedno );
	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, (WPARAM)0, 0 );

	int dlgret;
	dlgret = (int)dlg.DoModal(m_hWnd);

	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, (WPARAM)1, 0 );
	***/

	return S_OK;
}

STDMETHODIMP CSigTreeView2::ShowJointLocDlg()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );
	CTreeElem2* telem;
	telem = (*m_thandler)( m_selectedno );

	int ismorph;
	ismorph = IsMorphObj( selem );
	int isjoint;
	isjoint = selem->IsJoint();
	if( !isjoint || ismorph ){
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, 
			"���̃I�u�W�F�N�g�́A�W���C���g�ł͂���܂���B\n�l�p�}�[�N�̃I�u�W�F�N�g��I�����Ă��������B",
			"�I���G���[",
			MB_OK );
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
		return 0;
	}

	CJointLocDlg dlg( telem, selem, m_apphwnd);
	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, (WPARAM)0, 0 );

	int dlgret;
	dlgret = (int)dlg.DoModal(m_hWnd);

	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, (WPARAM)1, 0 );

	return 0;
}

STDMETHODIMP CSigTreeView2::GetSelectedNo( int* selnoptr )
{
	*selnoptr = m_selectedno;
	return 0;
}

STDMETHODIMP CSigTreeView2::Rename( int seri, long srcname )
{
	if( (seri < 0) || !srcname ){
		return 0;
	}
	if( !m_ctlSysTreeView32 ){
		return 0;
	}

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
		ret = TreeView_SetItem( m_ctlSysTreeView32->m_hWnd, &tvi );
		_ASSERT( ret > 0 );
	}

	return 0;
}


STDMETHODIMP CSigTreeView2::ShowShdParamDlg()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	CShdElem* selem;
	selem = (*m_shandler)( m_selectedno );
	CTreeElem2* telem;
	telem = (*m_thandler)( m_selectedno );

	int ismorph;
	ismorph = IsMorphObj( selem );
	if( ismorph ){
		::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
		::MessageBox( m_hWnd, "���[�t�B���O�I�u�W�F�N�g�̓G�f�B�b�g�ł��܂���B",
			"�G�f�B�b�g�֎~",
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

int CSigTreeView2::ChangeTVParams( int editflag, char* shdname )
{
	if( !m_ctlSysTreeView32 ){
		return 0;
	}


	//if( dlgret == IDOK ){
	// ���O�̍X�V
		int iseditname = editflag & TV_EDIT_SHDNAME;
		if( iseditname ){
			TVITEM tvi;
			tvi.hItem = m_selecteditem;
			tvi.pszText = shdname;
			tvi.cchTextMax = (int)(strlen( shdname ) + 1);
			tvi.mask = TVIF_HANDLE | TVIF_TEXT;
			int ret;
			ret = TreeView_SetItem( m_ctlSysTreeView32->m_hWnd, &tvi );
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

	//shdparadlg��OK�{�^���������Ȃ��ł��ATexSettingDlg��OK�{�^���������΁A���ꂪ�A���f�����悤�ɂ���B
	int isedittexture = editflag & TV_EDIT_TEXTURE;
	if( isedittexture ){
		_ASSERT( m_apphwnd );
		::SendMessage( m_apphwnd, WM_USER_REMAKE_TEXTURE, (WPARAM)m_selectedno, 0 );
		::SendMessage( m_apphwnd, WM_USER_REMAKE_UV, (WPARAM)m_selectedno, 0 );
	}

	return 0;
}


LRESULT CSigTreeView2::OnSelChanged( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( m_invalidate_handler )
		return 0;
	
	if( !m_ctlSysTreeView32 ){
		return 0;
	}


	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pnmh;
	TVITEM tvi = pnmtv->itemNew;

	m_selecteditem = tvi.hItem;

	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	if( !TreeView_GetItem( m_ctlSysTreeView32->m_hWnd, &tvi ) )
		return 0;

	m_selectedno = (int)(tvi.lParam);

	CTreeElem2* seltelem;
	seltelem = (*m_thandler)( m_selectedno );

	::SendMessage( m_apphwnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)m_selectedno );

	DbgOut( "CSigTreeView2 : selected %s : %d\n",
		seltelem->name, m_selectedno );

	return 0;
}
LRESULT CSigTreeView2::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B

	if( !m_ctlSysTreeView32 ){
		return 0;
	}

	//DefWindowProc();
	::DefWindowProc( m_ctlSysTreeView32->m_hWnd, uMsg, wParam, lParam );
	bHandled = FALSE;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	
	m_onitembutton = 0;

	//if( m_clicked_lbutton == 1 ){
	//	DbgOut( "SigTreeView2 : OnLButtonDown : m_clicked_lbutton == 1 return \n" );
	//	return 0;
	//}

	TVHITTESTINFO ht;
	POINT htpoint;
	htpoint.x = LOWORD(lParam);
	htpoint.y = HIWORD(lParam);
	ht.pt = htpoint;
	ht.flags = TVHT_ONITEM;
	ht.hItem = 0;
	TreeView_HitTest( m_ctlSysTreeView32->m_hWnd,
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
		ret = TreeView_GetItem( m_ctlSysTreeView32->m_hWnd, &tvi );
		int clickno;
		clickno = (int)tvi.lParam;
	
//DbgOut( "SigTreeView2 : OnLButtonDown : clickno %d\n", clickno );

		if( m_selectedno != clickno ){
			OnUserSelchange( clickno );
		}
	}
	
	return 0;
}

LRESULT CSigTreeView2::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_clicked_lbutton == 0 )
		return 0;
	if( !m_ctlSysTreeView32 ){
		return 0;
	}


	TVHITTESTINFO ht;
	POINT htpoint;
	htpoint.x = LOWORD(lParam);
	htpoint.y = HIWORD(lParam);
	ht.pt = htpoint;
	ht.flags = TVHT_ONITEM;
	ht.hItem = 0;
	TreeView_HitTest( m_ctlSysTreeView32->m_hWnd,
		&ht );

	if( !(ht.flags & TVHT_ONITEM) || m_onitembutton ){
		m_clicked_lbutton = 0;
		m_onitembutton = 0;
		return 0;
	}


	m_dropitem = ht.hItem;
	TVITEM tvi;
	tvi.hItem = m_dropitem;
	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	int ret;
	ret = TreeView_GetItem( m_ctlSysTreeView32->m_hWnd, &tvi );

	m_dropno = (int)tvi.lParam;

	if( m_dropno != m_selectedno ){
		ret = MakeMoveList();
		_ASSERT( !ret );

		ret = IsValidMove();
		if( !ret ){
			m_clicked_lbutton = 0;
			return 0;
		}
		
		CShdElem* dropelem = (*m_shandler)( m_dropno );
		int ismorph;
		ismorph = IsMorphObj( dropelem );
		if( ismorph ){
			m_clicked_lbutton = 0;
			::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 0, 0 );
			::MessageBox( m_hWnd, 
				"���[�t�B���O�I�u�W�F�N�g�̉��ɂ́A�ړ��ł��܂���B",
				"�ړ��֎~",
				MB_OK );
			::SendMessage( m_apphwnd, WM_USER_ENABLE_MENU, 1, 0 );
			return 0;
		}

		ret = MoveTree();
		if( ret ){
			_ASSERT( 0 );
			ErrorMes( -1, MSGERR_APPMUSTEXIT, "CSigTreeView2::OnLButtonUP MoveTree error" );
			return -1;
		}

		_ASSERT( m_apphwnd );
		::SendMessage( m_apphwnd, WM_USER_REMAKE_TREE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)m_selectedno );

	}

	m_clicked_lbutton = 0;
	return 0;
}

LRESULT CSigTreeView2::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;

	if( wParam == MK_LBUTTON )
		m_clicked_lbutton = 1;

	return 0;
}
int CSigTreeView2::MakeMoveList()
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

void CSigTreeView2::SetMoveListReq( CShdElem* srcselem )
{
	if( m_movenum > 256 ){
		DbgOut( "CSigTreeView2 : SetMoveListReq : movenum range error !!!\n" );
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

int CSigTreeView2::IsValidMove()
{	
	if( !m_ctlSysTreeView32 ){
		return 0;
	}

	//int parentno;
	CShdElem* curselem = (*m_shandler)( m_selectedno );
	CShdElem* parentselem = curselem->parent;
	if( !parentselem || (parentselem->serialno == 0) ){
		//parent�̂Ȃ����[�g�͈ړ��ł��Ȃ��B
		return 0;
	}

	/***
	if( parentselem && (parentselem->serialno != 0) ){
		parentno = parentselem->serialno;
		if( m_dropno == parentno ){
			//�����̐e�ւ̈ړ�(�Ӗ����Ȃ�)�̋֎~�B
			return 0;
		}
	}else{
		//parent�̂Ȃ����[�g�͈ړ��ł��Ȃ��B
		return 0;
	}
	***/

	//�����̎q���ڂւ̈ړ��̋֎~
	//m_dropno��m_movelist�ɂȂ����`�F�b�N�B
	int listno;
	for( listno = 0; listno < m_movenum; listno++ ){
		TVITEM tvi;
		tvi.hItem = m_movelist[listno];
		tvi.mask = TVIF_HANDLE | TVIF_PARAM;
		if( !TreeView_GetItem( m_ctlSysTreeView32->m_hWnd, &tvi ) )
			return 0;

		int curno = (int)(tvi.lParam);
		if( curno == m_dropno )
			return 0;
	}

	return 1;
}

int CSigTreeView2::MoveTree()
{
	if( !m_ctlSysTreeView32 ){
		return 0;
	}

	//ShdElem�̕ύX
	CShdElem* mvselem = (*m_shandler)( m_selectedno );
	int ret;
	ret = mvselem->LeaveFromChain();// child���́A���̂܂܂ŁA�`�F�C�����痣���B
	if( ret )
		return 1;

	CShdElem* dropselem = (*m_shandler)( m_dropno );
	ret = mvselem->PutToChain( dropselem );
	if( ret )
		return 1;

	ret = m_shandler->CalcDepth();
	if( ret )
		return 1;
	
	//MotElem�̕ύX
	ret = m_mhandler->CopyChainFromShd( m_shandler );
	if( ret )
		return 1;

	ret = m_mhandler->CalcDepth();
	if( ret )
		return 1;

	//TreeElem�̕ύX�͂Ȃ��I�I

	//TreeView�̕ύX
		//Tree�̒ǉ�
	AddTree( m_selectedno, 0, 0 );	


		//selected�̍X�V
	m_selecteditem = *(m_TI + m_selectedno);
	TreeView_SelectItem( m_ctlSysTreeView32->m_hWnd,
		m_selecteditem );
	
		//Tree�̍폜
	int delno;
	for( delno = 0; delno < m_movenum; delno++ ){
		HTREEITEM delitem = m_movelist[delno];

		TreeView_DeleteItem( m_ctlSysTreeView32->m_hWnd,
			delitem );
	}


	return 0;
}


STDMETHODIMP CSigTreeView2::SetParams(int sx, int sy, long lpth, long lpsh, long lpmh, long appwnd)
{
	// TODO: ���̈ʒu�ɃC���v�������g�p�̃R�[�h��ǉ����Ă�������
	if( m_thandler || m_shandler || m_mhandler ){
		//������Tree���󂷁B
		m_invalidate_handler = 1;

		//TreeView_DeleteAllItems( m_ctlSysTreeView32.m_hWnd );
		if( m_ctlSysTreeView32 ){
			m_ctlSysTreeView32->DestroyWindow();
			delete m_ctlSysTreeView32;
			m_ctlSysTreeView32 = 0;
		}
		if( m_TI ){
			free( m_TI );
			m_TI = 0;
		}
	}

	m_thandler = (CTreeHandler2*)lpth;
	m_shandler = (CShdHandler*)lpsh;
	m_mhandler = (CMotHandler*)lpmh;

	m_apphwnd = (HWND)appwnd;

	m_invalidate_handler = 0;


	m_ctlSysTreeView32 = new CContainedWindow( _T("SysTreeView32"), this, 1 );
	if( !m_ctlSysTreeView32 ){
		DbgOut( "sigtreeview2 : treeview alloc error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = sx;
	rc.bottom = sy;
	LONG lret;
	HWND tvwnd;
	lret = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NOSCROLL;
	tvwnd = m_ctlSysTreeView32->Create( m_hWnd, rc, NULL, lret );
	if( !tvwnd ){
		DbgOut( "SigTreeView2 : SetParams : treeview Create error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}

	HIMAGELIST retIL;
	retIL = TreeView_SetImageList( m_ctlSysTreeView32->m_hWnd,
		m_hImageList, TVSIL_NORMAL );


	int ret;
	ret = FillTree();
	if( ret ){
		DbgOut( "SigTreeView2 : SetParams : FillTree error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}

//	m_ctlSysTreeView32.UpdateWindow();
//	UpdateWindow();


	return 0;
}

STDMETHODIMP CSigTreeView2::SetDbgFile(long dbgf)
{
	// TODO: ���̈ʒu�ɃC���v�������g�p�̃R�[�h��ǉ����Ă�������
	dbgfile = (HWND)dbgf;
	return S_OK;
}

STDMETHODIMP CSigTreeView2::Redraw()
{
	// TODO: ���̈ʒu�ɃC���v�������g�p�̃R�[�h��ǉ����Ă�������
	::UpdateWindow( m_hWnd );
	if( m_ctlSysTreeView32 ){
		::UpdateWindow( m_ctlSysTreeView32->m_hWnd );
	}
	return S_OK;
}

LRESULT CSigTreeView2::OnMove()
{
	// TODO : ү���� ����חp�̺��ނ�ǉ����Ă��������B�K�v�ɉ����� DefWindowProc ���Ăяo���Ă��������B

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( m_apphwnd ){
		::SendMessage( m_apphwnd, WM_USER_MOVE, (WPARAM)ID_MSG_FROM_SIGDLG, 0 );
	}
	DefWindowProc();
	return S_OK;
}


STDMETHODIMP CSigTreeView2::OnUserSelchange(long selno)
{
	// TODO: ���̈ʒu�ɃC���v�������g�p�̃R�[�h��ǉ����Ă�������
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;
	if( !m_ctlSysTreeView32 ){
		return 0;
	}
	

	HTREEITEM selitem;
	selitem = *(m_TI + (int)selno);
	if( selitem ){
		m_selectedno = selno;
		m_selecteditem = selitem;

		TreeView_SelectItem( m_ctlSysTreeView32->m_hWnd,
			m_selecteditem );

//		TreeView_SelectSetFirstVisible( m_ctlSysTreeView32.m_hWnd, m_selecteditem );

		::UpdateWindow( m_ctlSysTreeView32->m_hWnd );

		::SendMessage( m_apphwnd, WM_USER_DISPLAY, 0, 0 );
	}
	return S_OK;
}

STDMETHODIMP CSigTreeView2::InvalidateHandler()
{
	// TODO: ���̈ʒu�ɃC���v�������g�p�̃R�[�h��ǉ����Ă�������
	m_invalidate_handler = 1;
//	TreeView_DeleteAllItems( m_ctlSysTreeView32.m_hWnd );
	if( m_ctlSysTreeView32 ){
		m_ctlSysTreeView32->DestroyWindow();
		delete m_ctlSysTreeView32;
		m_ctlSysTreeView32 = 0;
	}

	if( m_TI ){
		free( m_TI );
		m_TI = 0;
	}
	

	m_thandler = 0;
	m_shandler = 0;
	m_mhandler = 0;
	m_selectedno = 0;
	m_selecteditem = 0;

	return S_OK;
}

int CSigTreeView2::IsMorphObj( CShdElem* srcselem )
{
	//�����A�܂��͐e�̃p�[�g�ɁASHDMORPH������ꍇ�́A�P��Ԃ��B
	int morphflag = 0;
	CShdElem* chkelem = srcselem;	

	while( chkelem ){
		if( chkelem->type == SHDMORPH ){
			morphflag = 1;
			break;
		}
		chkelem = chkelem->parent;
	}

	return morphflag;
}

void CSigTreeView2::ErrorMes( int errorcode, int type, char* mes )
{
	ERRORMES errormes;
	if( m_apphwnd ){
		errormes.errorcode = errorcode;
		errormes.type = type;
		errormes.mesptr = mes;
		SendMessage( m_apphwnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
	}
}

STDMETHODIMP CSigTreeView2::Conv2Scope()
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
		::MessageBox( m_hWnd, "�I�����Ă���v�f�́A�\���p�I�u�W�F�N�g�ł͂���܂���B\n�O�p�}�[�N�̗v�f��I��ŁA�Ď��s���Ă��������B", "�I���G���[", MB_OK );
		return 0;
	}

	CSelScopeDlg dlg( m_thandler, m_shandler, m_apphwnd, m_selectedno );
	ret = (int)dlg.DoModal();

	return ret;

}

LRESULT CSigTreeView2::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	if( !m_ctlSysTreeView32 ){
		return 0;
	}


	POINT clickpos;
	clickpos.x = GET_X_LPARAM(lParam); 
	clickpos.y = GET_Y_LPARAM(lParam); 
	m_ctlSysTreeView32->ScreenToClient( &clickpos );
	
	TVHITTESTINFO hittestinfo;
	hittestinfo.pt = clickpos;
	hittestinfo.flags = TVHT_ONITEMLABEL;
	hittestinfo.hItem = NULL;

	HTREEITEM hititem = NULL;
	hititem = TreeView_HitTest( m_ctlSysTreeView32->m_hWnd, &hittestinfo );

	if( hititem != NULL ){
		
		TVITEM tvi;
		tvi.hItem = hititem;
		tvi.mask = TVIF_HANDLE | TVIF_PARAM;
		ret = TreeView_GetItem( m_ctlSysTreeView32->m_hWnd, &tvi );
		int clickno;
		clickno = (int)tvi.lParam;
	
		if( m_selectedno != clickno ){
			OnUserSelchange( clickno );
		}

		ret = ShowShdParamDlg();
		_ASSERT( !ret );

	}

	return 0;
}

STDMETHODIMP CSigTreeView2::SizeChange( int posx, int posy, int newx, int newy )
{
/***
	ScrollWindow(0, 0, NULL, NULL);
	m_ctlSysTreeView32.ScrollWindow(0, 0, NULL, NULL);
	m_ctlSysTreeView32.UpdateWindow();
	UpdateWindow();
***/

//	MoveWindow( 0, 0, newx, newy, 1 );
//	m_ctlSysTreeView32.MoveWindow( 0, 0, newx, newy, 1 );


//	SetWindowPos( HWND_TOP, 0, 0, newx, newy, SWP_NOMOVE | SWP_NOZORDER );
//	m_ctlSysTreeView32.SetWindowPos( HWND_TOP, 0, 0, newx, newy, SWP_NOMOVE | SWP_NOZORDER );


	MoveWindow( 0, 0, newx, newy, 1 );
	if( m_ctlSysTreeView32 && ::IsWindow( m_ctlSysTreeView32->m_hWnd ) ){
		m_ctlSysTreeView32->MoveWindow( 0, 0, newx, newy, 1 );
	}


//	RECT rect1, rect2;
//	GetWindowRect( &rect1 );
//	MoveWindow( rect1.left, rect1.top, newx, newy, 1 );
//
//	m_ctlSysTreeView32.GetWindowRect( &rect2 );
//	m_ctlSysTreeView32.MoveWindow( rect2.left, rect2.top, newx, newy, 1 );

//	UpdateWindow();
//	m_ctlSysTreeView32.UpdateWindow();

	/***
	if( m_shandler && m_TI ){
		int serino;
		for( serino = 0; serino < m_shandler->s2shd_leng; serino++ ){
			HTREEITEM* curitem;
			curitem = m_TI + serino;
			if( curitem ){
				TreeView_Expand( m_ctlSysTreeView32.m_hWnd, *curitem, TVE_EXPAND );
			}
		}
	}
	***/
/***	
	SIZE sz;
	sz.cx = newx;
	sz.cy = newy;
    // �s�N�Z�����n�C���g���b�N�ɕϊ�
    AtlPixelToHiMetric (&sz, &m_sizeExtent);
    // �Œ�̕����T�C�Y���L��
    m_sizeNatural = m_sizeExtent;
***/
	return 0;
}

STDMETHODIMP CSigTreeView2::GetCtrlSize( int* cxptr, int* cyptr )
{
	/***
	RECT rect;
	m_ctlSysTreeView32.GetWindowRect( &rect );

	*cxptr = rect.right - rect.left;
	*cyptr = rect.bottom - rect.top;
	***/

/***
	SIZE sz;
	AtlHiMetricToPixel( &m_sizeExtent, &sz );
//	AtlHiMetricToPixel( &m_sizeNatural, &sz );
	*cxptr = sz.cx;
	*cyptr = sz.cy;
***/


	//m_ctlSysTreeView32
	if( m_ctlSysTreeView32 ){
		int itemnum;
		itemnum = TreeView_GetCount( m_ctlSysTreeView32->m_hWnd );

		int itemh;
		itemh = TreeView_GetItemHeight( m_ctlSysTreeView32->m_hWnd );

		*cyptr = itemnum * itemh * 3 / 2;

		int indent;
		indent = TreeView_GetIndent( m_ctlSysTreeView32->m_hWnd );

		*cxptr = indent * itemnum;
	//	*cxptr = 1000;
	}else{
		*cxptr = 300;
		*cyptr = 500;

	}
	return 0;
}

STDMETHODIMP CSigTreeView2::UDWindow()
{

	UpdateWindow();
	if( m_ctlSysTreeView32 ){
		m_ctlSysTreeView32->UpdateWindow();
	}

	return 0;
}

