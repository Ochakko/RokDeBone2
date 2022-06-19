#include "stdafx.h"

#include "DSMenuMain.h"


#define DBGH
#include <dbg.h>

#include <crtdbg.h>

#include "resource.h"


CDSMenuMain::CDSMenuMain()
{
	InitParams();
}
CDSMenuMain::~CDSMenuMain()
{

}

int CDSMenuMain::InitParams()
{
	m_rmenu = 0;
	m_rsubmenu = 0;
	return 0;
}

int CDSMenuMain::Create( HWND srchwnd )
{

	m_rmenu = LoadMenu( (HINSTANCE)GetWindowLong( srchwnd, GWLP_HINSTANCE ), MAKEINTRESOURCE( IDR_DSMENU ) );
	m_rsubmenu = GetSubMenu( m_rmenu, 0 );

	return 0;
}


int CDSMenuMain::Destroy()
{
	DestroyMenu( m_rmenu );
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CDSMenuMain::TrackPopupMenu( HWND srchwnd, POINT pt )
{
	Params2Dlg();


///////
	int ret;
	ret = ::TrackPopupMenu( m_rsubmenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, srchwnd, NULL);


	return ret;
}

int CDSMenuMain::Params2Dlg()
{


	return 0;
}


