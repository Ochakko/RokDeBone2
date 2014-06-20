#include "stdafx.h"

#include "KeyMenuMain.h"

#include <treehandler2.h>
#include <treeelem2.h>


#define DBGH
#include <dbg.h>

#include <crtdbg.h>

#include "resource.h"


CKeyMenuMain::CKeyMenuMain()
{
	InitParams();
}
CKeyMenuMain::~CKeyMenuMain()
{

}

int CKeyMenuMain::InitParams()
{
	m_rmenu = 0;
	m_rsubmenu = 0;
	return 0;
}

int CKeyMenuMain::Create( HWND srchwnd, CTreeHandler2* lpth, int* findseri, int findnum )
{

	m_rmenu = LoadMenu( (HINSTANCE)GetWindowLong( srchwnd, GWL_HINSTANCE ), MAKEINTRESOURCE( IDR_TEXMENU ) );
	m_rsubmenu = GetSubMenu( m_rmenu, 0 );

	int menuno, menunum;
	menunum = GetMenuItemCount( m_rsubmenu );
	for( menuno = 0; menuno < menunum; menuno++ ){
		RemoveMenu( m_rsubmenu, 0, MF_BYPOSITION);
	}

	int setno = 0;
	int findno;
	for( findno = 0; findno < findnum; findno++ ){
		int seri;
		seri = *( findseri + findno );
		if( (seri > 0) && (seri < lpth->s2e_leng) ){
			CTreeElem2* te;
			te = (*lpth)( seri );
			_ASSERT( te );

			AppendMenu( m_rsubmenu, MF_STRING, 65000 + setno, te->name );
			setno++;
		}
	}


	return 0;
}


int CKeyMenuMain::Destroy()
{
	DestroyMenu( m_rmenu );
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CKeyMenuMain::TrackPopupMenu( HWND srchwnd, POINT pt )
{
	Params2Dlg();


///////
	int ret;
	ret = ::TrackPopupMenu( m_rsubmenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, srchwnd, NULL);


	return ret;
}

int CKeyMenuMain::Params2Dlg()
{


	return 0;
}


