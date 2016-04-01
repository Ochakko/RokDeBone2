#include "stdafx.h"

#include "MCRMenu.h"

#define DBGH
#include <dbg.h>

#include <crtdbg.h>

//#include "resid.h"

#include "resource.h"


CMCRMenu::CMCRMenu()
{
	InitParams();
}
CMCRMenu::~CMCRMenu()
{

}

int CMCRMenu::InitParams()
{
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CMCRMenu::Create( HWND srchwnd )
{

	m_rmenu = LoadMenu( (HINSTANCE)GetWindowLong( srchwnd, GWL_HINSTANCE ), MAKEINTRESOURCE( IDR_MCR_MENU ) );
	m_rsubmenu = GetSubMenu( m_rmenu, 0 );

	return 0;
}


int CMCRMenu::Destroy()
{
	DestroyMenu( m_rmenu );
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CMCRMenu::TrackPopupMenu( int isparent, int cpnum, HWND srchwnd, POINT pt )
{

	Params2Dlg( isparent, cpnum );

	::TrackPopupMenu( m_rsubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, srchwnd, NULL);


	return 0;
}

int CMCRMenu::Params2Dlg( int isparent, int cpnum )
{

	if( isparent ){
		EnableMenuItem( m_rmenu, ID_MCRMENU_ADD, MF_ENABLED );
		EnableMenuItem( m_rmenu, ID_MCRMENU_DEL, MF_GRAYED );
		EnableMenuItem( m_rmenu, ID_MCRMENU_PROP, MF_ENABLED );

		EnableMenuItem( m_rmenu, ID_MCRMENU_COPY, MF_ENABLED );
		if( cpnum > 0 ){
			EnableMenuItem( m_rmenu, ID_MCRMENU_PASTE, MF_ENABLED );
		}else{
			EnableMenuItem( m_rmenu, ID_MCRMENU_PASTE, MF_GRAYED );
		}
	}else{
		EnableMenuItem( m_rmenu, ID_MCRMENU_ADD, MF_GRAYED );
		EnableMenuItem( m_rmenu, ID_MCRMENU_DEL, MF_ENABLED );
		EnableMenuItem( m_rmenu, ID_MCRMENU_PROP, MF_ENABLED );

		EnableMenuItem( m_rmenu, ID_MCRMENU_COPY, MF_ENABLED );
		EnableMenuItem( m_rmenu, ID_MCRMENU_PASTE, MF_GRAYED );
	}

	return 0;
}
