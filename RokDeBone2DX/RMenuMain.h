#ifndef RMENU1H
#define RMENU1H

#include <coef.h>


class CMotParamDlg;
class CShdElem;
class CTreeElem2;

class CRMenuMain
{
public:
	CRMenuMain( CMotParamDlg* srcdlg );
	~CRMenuMain();

	int Create( HWND srchwnd );
	int Destroy();

	int TrackPopupMenu( CTreeElem2* telem, CShdElem* selem, HWND srchwnd, int srcikfk, POINT pt );

	int ToggleIKTrans();
	int ToggleIKSkip();

	int SetIKMode( int srcid );
	int SetIKLevel( int srcid );

	int SetIKFK( int srcflag );

	int SetIKTargetParams();

	int SetSymMotionPoint( int symtype );

private:
	int InitParams();
	int CheckLevelMenu( int checkid );
	int Params2Dlg();

public:
	int m_iktrans;
	int m_ikskip;

	int m_ikmode;
	int m_level;

	int m_ikfk;

	CMotParamDlg* m_motparamdlg;

	CShdElem* m_curselem;
	CTreeElem2* m_curtelem;

private:
	HMENU m_rmenu;
	HMENU m_rsubmenu;


};

#endif