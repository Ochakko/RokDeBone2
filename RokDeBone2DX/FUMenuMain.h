#ifndef FUMENUMAINH
#define FUMENUMAINH

#include <coef.h>

class CFUMenuMain
{
public:
	CFUMenuMain();
	~CFUMenuMain();

	int Create( HWND srchwnd );
	int Destroy();

	int TrackPopupMenu( HWND srchwnd, POINT pt, int srckind );

private:
	int InitParams();
	int Params2Dlg( int srckind );

private:
	HMENU m_rmenu;
	HMENU m_rsubmenu;
};

#endif