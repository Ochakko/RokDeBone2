#include "stdafx.h"

#include "TexMenuMain.h"

#include <mqomaterial.h>


#define DBGH
#include <dbg.h>

#include <crtdbg.h>

#include "resource.h"


CTexMenuMain::CTexMenuMain()
{
	InitParams();
}
CTexMenuMain::~CTexMenuMain()
{

}

int CTexMenuMain::InitParams()
{
	m_rmenu = 0;
	m_rsubmenu = 0;
	return 0;
}

int CTexMenuMain::Create( HWND srchwnd, CMQOMaterial* srcmat )
{

	m_rmenu = LoadMenu( (HINSTANCE)GetWindowLong( srchwnd, GWL_HINSTANCE ), MAKEINTRESOURCE( IDR_TEXMENU ) );
	m_rsubmenu = GetSubMenu( m_rmenu, 0 );

	int menuno, menunum;
	menunum = GetMenuItemCount( m_rsubmenu );
	for( menuno = 0; menuno < menunum; menuno++ ){
		RemoveMenu( m_rsubmenu, 0, MF_BYPOSITION);
	}

	int setno = 0;
	AppendMenu( m_rsubmenu, MF_STRING, 65000 + setno, "Ý’è‚È‚µ" );
	setno++;

	if( srcmat->tex[0] != 0 ){
		AppendMenu( m_rsubmenu, MF_STRING, 65000 + setno, srcmat->tex );
		setno++;
	}

	int extno;
	for( extno = 0; extno < srcmat->exttexnum; extno++ ){
		AppendMenu( m_rsubmenu, MF_STRING, 65000 + setno, *( srcmat->exttexname + extno ) );
		setno++;
	}

	return 0;
}


int CTexMenuMain::Destroy()
{
	DestroyMenu( m_rmenu );
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CTexMenuMain::TrackPopupMenu( HWND srchwnd, POINT pt )
{
	Params2Dlg();


///////
	int ret;
	ret = ::TrackPopupMenu( m_rsubmenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, srchwnd, NULL);


	return ret;
}

int CTexMenuMain::Params2Dlg()
{


	return 0;
}


