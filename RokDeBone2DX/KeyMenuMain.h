#ifndef KEYMENUMAINH
#define KEYMENUMAINH

#include <coef.h>

class CMQOMaterial;

class CKeyMenuMain
{
public:
	CKeyMenuMain();
	~CKeyMenuMain();

	int Create( HWND srchwnd, CTreeHandler2* lpth, int* findseri, int findnum );
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