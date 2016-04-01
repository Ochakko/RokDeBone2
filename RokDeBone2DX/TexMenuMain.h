#ifndef TEXMENUMAINH
#define TEXMENUMAINH

#include <coef.h>

class CMQOMaterial;

class CTexMenuMain
{
public:
	CTexMenuMain();
	~CTexMenuMain();

	int Create( HWND srchwnd, CMQOMaterial* srcmat );
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