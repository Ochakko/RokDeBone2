#include "stdafx.h"

#include "RMenuMain.h"

#define DBGH
#include <dbg.h>

#include "MotParamDlg.h"
#include <shdelem.h>
#include <treeelem2.h>

#include "IKTargetDlg.h"

#include <crtdbg.h>

#include "resid.h"


CRMenuMain::CRMenuMain( CMotParamDlg* srcdlg )
{
	InitParams();
	m_motparamdlg = srcdlg;
	m_curselem = 0;
	m_curtelem = 0;
}
CRMenuMain::~CRMenuMain()
{

}

int CRMenuMain::InitParams()
{
	m_iktrans = 0;
	m_ikskip = 0;
	m_ikmode = IKMODE_OPE;
	m_level = 5;
	m_motparamdlg = 0;

	m_rmenu = 0;
	m_rsubmenu = 0;

	m_ikfk = 0;

	return 0;
}

int CRMenuMain::Create( HWND srchwnd )
{

	m_rmenu = LoadMenu( (HINSTANCE)GetWindowLong( srchwnd, GWL_HINSTANCE ), MAKEINTRESOURCE( IDR_R_MENU ) );
	m_rsubmenu = GetSubMenu( m_rmenu, 0 );

	return 0;
}


int CRMenuMain::Destroy()
{
	DestroyMenu( m_rmenu );
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CRMenuMain::TrackPopupMenu( CTreeElem2* telem, CShdElem* selem, HWND srchwnd, int srcikfk, POINT pt )
{
	if( !selem )
		return 0;

	m_curselem = selem;
	m_curtelem = telem;
		
	m_iktrans = m_curselem->m_notransik;
	m_ikskip = m_curselem->m_ikskip;
	m_ikmode = m_motparamdlg->m_ikmode;
	if( m_ikmode == IKMODE_JOINT ){
		m_level = m_motparamdlg->m_iklevel;
	}else{
		m_level = m_motparamdlg->m_iklevelope;
	}

	m_ikfk = srcikfk;

	Params2Dlg();


///////
	::TrackPopupMenu( m_rsubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, srchwnd, NULL);


	return 0;
}

int CRMenuMain::Params2Dlg()
{

///////
	if( m_iktrans )
		CheckMenuItem( m_rmenu, ID_RMENU_IKTRANS, MF_CHECKED );
	else
		CheckMenuItem( m_rmenu, ID_RMENU_IKTRANS, MF_UNCHECKED );

	if( m_ikskip )
		CheckMenuItem( m_rmenu, ID_RMENU_IKSKIP, MF_CHECKED );
	else
		CheckMenuItem( m_rmenu, ID_RMENU_IKSKIP, MF_UNCHECKED );

	/***
	HMENU subsub;
	subsub = GetSubMenu( m_rsubmenu, 0 );
	_ASSERT( subsub );

	HMENU subsubsub;
	subsubsub = GetSubMenu( subsub, 1 );
	_ASSERT( subsubsub );

	if( m_ikmode == IKMODE_JOINT ){
		CheckMenuItem( subsubsub, ID_RMENU_MODE_JOINT, MF_CHECKED );
		CheckMenuItem( subsubsub, ID_RMENU_MODE_OPE, MF_UNCHECKED );
	}else{
		CheckMenuItem( subsubsub, ID_RMENU_MODE_JOINT, MF_UNCHECKED );
		CheckMenuItem( subsubsub, ID_RMENU_MODE_OPE, MF_CHECKED );
	}
	***/


	if( m_ikmode == IKMODE_JOINT ){
		CheckMenuItem( m_rmenu, ID_RMENU_MODE_JOINT, MF_CHECKED );
		CheckMenuItem( m_rmenu, ID_RMENU_MODE_OPE, MF_UNCHECKED );
	}else{
		CheckMenuItem( m_rmenu, ID_RMENU_MODE_JOINT, MF_UNCHECKED );
		CheckMenuItem( m_rmenu, ID_RMENU_MODE_OPE, MF_CHECKED );
	}


	switch( m_level ){
		case 1:
			CheckLevelMenu( ID_RMENU_LEVEL1 );
			break;
		case 2:
			CheckLevelMenu( ID_RMENU_LEVEL2 );
			break;
		case 3:
			CheckLevelMenu( ID_RMENU_LEVEL3 );
			break;
		case 4:
			CheckLevelMenu( ID_RMENU_LEVEL4 );
			break;
		case 5:
			CheckLevelMenu( ID_RMENU_LEVEL5 );
			break;
		case 6:
			CheckLevelMenu( ID_RMENU_LEVEL6 );
			break;
		case 7:
			CheckLevelMenu( ID_RMENU_LEVEL7 );
			break;
		case 8:
			CheckLevelMenu( ID_RMENU_LEVEL8 );
			break;
		case 9:
			CheckLevelMenu( ID_RMENU_LEVEL9 );
			break;
		case 10:
			CheckLevelMenu( ID_RMENU_LEVEL10 );
			break;
		case 11:
			CheckLevelMenu( ID_RMENU_LEVEL11 );
			break;
		case 12:
			CheckLevelMenu( ID_RMENU_LEVEL12 );
			break;
		case 13:
			CheckLevelMenu( ID_RMENU_LEVEL13 );
			break;
		case 14:
			CheckLevelMenu( ID_RMENU_LEVEL14 );
			break;
		case 15:
			CheckLevelMenu( ID_RMENU_LEVEL15 );
			break;
		case 255:
			CheckLevelMenu( ID_RMENU_LEVELINF );
			break;
	}

	if( m_ikfk == 0 ){
		CheckMenuItem( m_rmenu, ID_RMENU_IKDRAG, MF_CHECKED );
		CheckMenuItem( m_rmenu, ID_RMENU_FKDRAG, MF_UNCHECKED );
	}else{
		CheckMenuItem( m_rmenu, ID_RMENU_IKDRAG, MF_UNCHECKED );
		CheckMenuItem( m_rmenu, ID_RMENU_FKDRAG, MF_CHECKED );
	}

	return 0;
}


int CRMenuMain::CheckLevelMenu( int checkid )
{
	static int idtable[16] = {
		ID_RMENU_LEVEL1, ID_RMENU_LEVEL2, ID_RMENU_LEVEL3, ID_RMENU_LEVEL4, ID_RMENU_LEVEL5,
		ID_RMENU_LEVEL6, ID_RMENU_LEVEL7, ID_RMENU_LEVEL8, ID_RMENU_LEVEL9, ID_RMENU_LEVEL10,
		ID_RMENU_LEVEL11, ID_RMENU_LEVEL12, ID_RMENU_LEVEL13, ID_RMENU_LEVEL14, ID_RMENU_LEVEL15,
		ID_RMENU_LEVELINF
	};

	int no;
	int curid;
	for( no = 0; no < 16; no++ ){
		curid = idtable[no];

		if( curid == checkid ){
			CheckMenuItem( m_rmenu, curid, MF_CHECKED );
		}else{
			CheckMenuItem( m_rmenu, curid, MF_UNCHECKED );
		}
	}


	return 0;
}

int CRMenuMain::ToggleIKTrans()
{
	if( !m_curselem )
		return 0;

	if( m_iktrans ){
		m_iktrans = 0;		
	}else{
		m_iktrans = 1;
	}

	m_curselem->m_notransik = m_iktrans;

	Params2Dlg();

	return 0;
}
int CRMenuMain::ToggleIKSkip()
{
	if( !m_curselem )
		return 0;

	if( m_ikskip ){
		m_ikskip = 0;
	}else{
		m_ikskip = 1;
	}

	m_curselem->m_ikskip = m_ikskip;

	Params2Dlg();

	return 0;
}

int CRMenuMain::SetIKMode( int srcid )
{
	if( srcid == ID_RMENU_MODE_JOINT ){
		m_ikmode = IKMODE_JOINT;
	}else{
		m_ikmode = IKMODE_OPE;
	}
	m_motparamdlg->m_ikmode = m_ikmode;
	m_motparamdlg->UpdateRadioDlg();
	m_motparamdlg->UpdateComboDlg();

	Params2Dlg();

//DbgOut( "rmenu : SetIKMode : m_ikmode %d, dlg ikmode %d\r\n", m_ikmode, m_motparamdlg->m_ikmode );

	return 0;
}
int CRMenuMain::SetIKLevel( int srcid )
{
	switch( srcid ){
	case ID_RMENU_LEVEL1:
		m_level = 1;
		break;
	case ID_RMENU_LEVEL2:
		m_level = 2;
		break;
	case ID_RMENU_LEVEL3:
		m_level = 3;
		break;
	case ID_RMENU_LEVEL4:
		m_level = 4;
		break;
	case ID_RMENU_LEVEL5:
		m_level = 5;
		break;
	case ID_RMENU_LEVEL6:
		m_level = 6;
		break;
	case ID_RMENU_LEVEL7:
		m_level = 7;
		break;
	case ID_RMENU_LEVEL8:
		m_level = 8;
		break;
	case ID_RMENU_LEVEL9:
		m_level = 9;
		break;
	case ID_RMENU_LEVEL10:
		m_level = 10;
		break;
	case ID_RMENU_LEVEL11:
		m_level = 11;
		break;
	case ID_RMENU_LEVEL12:
		m_level = 12;
		break;
	case ID_RMENU_LEVEL13:
		m_level = 13;
		break;
	case ID_RMENU_LEVEL14:
		m_level = 14;
		break;
	case ID_RMENU_LEVEL15:
		m_level = 15;
		break;
	case ID_RMENU_LEVELINF:
		m_level = 255;
		break;
	}

	if( m_ikmode == IKMODE_JOINT ){
		m_motparamdlg->m_iklevel = m_level;
		m_curselem->m_iklevel = m_level;//!!!!!!!!!!
	}else{
		m_motparamdlg->m_iklevelope = m_level;
	}

	m_motparamdlg->UpdateComboDlg();


	return 0;
}

int CRMenuMain::SetIKFK( int srcflag )
{
	m_ikfk = srcflag;

	if( srcflag == 0 ){
		CheckMenuItem( m_rmenu, ID_RMENU_IKDRAG, MF_CHECKED );
		CheckMenuItem( m_rmenu, ID_RMENU_FKDRAG, MF_UNCHECKED );
	}else{
		CheckMenuItem( m_rmenu, ID_RMENU_IKDRAG, MF_UNCHECKED );
		CheckMenuItem( m_rmenu, ID_RMENU_FKDRAG, MF_CHECKED );
	}

	return 0;
}

int CRMenuMain::SetIKTargetParams()
{
	int ret;

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );
	D3DXVECTOR3 bonepos;

	ret = m_motparamdlg->GetBonePos( m_curselem->serialno, inimat, 1, &bonepos );
	if( ret ){
		DbgOut( "RMenuMain : SetIKTargetParams : GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CIKTargetDlg dlg( m_curtelem, m_curselem, bonepos );
	ret = (int)dlg.DoModal();
	if( ret = IDOK ){

		m_curselem->m_iktarget = dlg.m_iktarget;
		m_curselem->m_iktargetlevel = dlg.m_iktargetlevel;
		m_curselem->m_iktargetcnt = dlg.m_iktargetcnt;
		m_curselem->m_iktargetpos = dlg.m_iktargetpos;

	}

	return 0;
}


int CRMenuMain::SetSymMotionPoint( int symtype )
{
	int ret;


	ret = m_motparamdlg->SetSymMotionPoint( m_curselem, symtype );
	if( ret ){
		DbgOut( "RMenuMain : SetSymMotionPoint : motparamdlg SetSymMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
