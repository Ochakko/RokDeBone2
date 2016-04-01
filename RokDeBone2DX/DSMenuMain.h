#ifndef DSMenuMainH
#define DSMenuMainH

#include <coef.h>

class CDSMenuMain
{
public:
	CDSMenuMain();
	~CDSMenuMain();

	int Create( HWND srchwnd );
	int Destroy();

	int TrackPopupMenu( HWND srchwnd, POINT pt );

private:
	int InitParams();
	int Params2Dlg();


private:
	HMENU m_rmenu;
	HMENU m_rsubmenu;

};

#endif