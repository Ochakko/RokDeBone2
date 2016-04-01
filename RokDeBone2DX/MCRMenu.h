#ifndef MCRMENU1H
#define MCRMENU1H

#include <coef.h>


class CMCRMenu
{
public:
	CMCRMenu();
	~CMCRMenu();

	int Create( HWND srchwnd );
	int Destroy();

	int TrackPopupMenu( int isparent, int cpnum, HWND srchwnd, POINT pt );

private:
	int InitParams();
	int Params2Dlg( int isparent, int cpnum );

public:

private:
	HMENU m_rmenu;
	HMENU m_rsubmenu;


};

#endif