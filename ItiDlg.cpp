// ItiDlg.cpp : CItiDlg のインプリメンテーション
#include "stdafx.h"
#include "ItiDlg.h"

#include <shdhandler.h>
#include <mothandler.h>
#include <shdelem.h>
#include <part.h>

#include "MotDlg.h"
#include "viewer.h"

#include "GetDlgParams.h"

#include "inidata.h"

#include <quaternion.h>
#include <matrix2.h>

extern CIniData* g_inidata;

/////////////////////////////////////////////////////////////////////////////
// CItiDlg


// extern
extern CMotDlg* g_motdlg;


CItiDlg::CItiDlg(  CMyD3DApplication* papp )
{
	m_papp = papp;

	m_inputpos.x = 0.0f;
	m_inputpos.y = 0.0f;
	m_inputpos.z = 0.0f;

	m_worldpos.x = 0.0f;
	m_worldpos.y = 0.0f;
	m_worldpos.z = 0.0f;

	m_movemode = MOVEMODE_ABSOLUTE;

	m_object = ITIOBJ_VERTEX;
	m_itimode = 0;

	m_wheelscale = 0;
	m_childjoint = 0;
}

CItiDlg::~CItiDlg()
{
}

LRESULT CItiDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CItiDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_MOVE_RELATIVE );
	if( checkflag == BST_CHECKED ){
		m_movemode = MOVEMODE_RELATIVE;
	}else if( checkflag == BST_UNCHECKED ){
		m_movemode = MOVEMODE_ABSOLUTE;
	}
	
/////////
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_VERT );
	if( checkflag == BST_CHECKED ){
		m_object = ITIOBJ_VERTEX;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_FACE );
	if( checkflag == BST_CHECKED ){
		m_object = ITIOBJ_FACE;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_PART );
	if( checkflag == BST_CHECKED ){
		m_object = ITIOBJ_PART;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_JOINT );
	if( checkflag == BST_CHECKED ){
		m_object = ITIOBJ_JOINT;
	}
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_JOINTROT );
	if( checkflag == BST_CHECKED ){
		m_object = ITIOBJ_JOINTROT;
	}

/////////
	float tempx, tempy, tempz;
	ret = GetFloat( m_posx_wnd, &tempx );
	if( ret ){
		MessageBox( "X座標の値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_posy_wnd, &tempy );
	if( ret ){
		MessageBox( "Y座標の値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	ret = GetFloat( m_posz_wnd, &tempz );
	if( ret ){
		MessageBox( "Z座標の値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_inputpos.x = tempx;
	m_inputpos.y = tempy;
	m_inputpos.z = tempz;

////////////
////////////

	if( !(m_papp->m_shandler) || !(m_papp->m_mhandler) || (m_papp->m_shiftpart <= 0) ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!
	}

		switch( m_object ){
		case ITIOBJ_VERTEX:
			if( m_movemode == MOVEMODE_ABSOLUTE ){
				ret = MoveVertexAbsolute( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_papp->m_shifvert, m_papp->m_shifvertnum, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MoveVertexAbsolute error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( m_movemode == MOVEMODE_RELATIVE ){
				ret = MoveVertexRelative( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_papp->m_shifvert, m_papp->m_shifvertnum, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MoveVertexRelative error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_FACE:
			if( m_movemode == MOVEMODE_ABSOLUTE ){
				ret = MoveFaceAbsolute( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_papp->m_shiftface, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MoveFaceAbsolute error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( m_movemode == MOVEMODE_RELATIVE ){
				ret = MoveFaceRelative( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_papp->m_shiftface, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MoveFaceRelative error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_PART:
			if( m_movemode == MOVEMODE_ABSOLUTE ){
				ret = MovePartAbsolute( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MovePartAbsolute error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( m_movemode == MOVEMODE_RELATIVE ){
				ret = MovePartRelative( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MovePartRelative error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_JOINT:
			if( m_movemode == MOVEMODE_ABSOLUTE ){
				ret = MoveJointAbsolute( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MoveJointAbsolute error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( m_movemode == MOVEMODE_RELATIVE ){
				ret = MoveJointRelative( m_papp, m_papp->m_shandler, m_papp->m_mhandler, m_papp->m_shiftpart, m_inputpos );
				if( ret ){
					DbgOut( "itidlg : OnOK : MoveJointRelative error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		default:
			_ASSERT( 0 );
			break;
		}
	

	//EndDialog(wID); ！！！！！モードレスダイアログ！！！！！
	return 0;
}


LRESULT CItiDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	//モードレスダイアログ

	ShowWindow( SW_HIDE );

	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_papp->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	//EndDialog(wID);
	return 0;
}


int CItiDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_posx_wnd = GetDlgItem( IDC_POSX );
	m_posy_wnd = GetDlgItem( IDC_POSY );
	m_posz_wnd = GetDlgItem( IDC_POSZ );

	m_vert_wnd = GetDlgItem( IDC_VERT );
	m_face_wnd = GetDlgItem( IDC_FACE );
	m_part_wnd = GetDlgItem( IDC_PART );
	m_joint_wnd = GetDlgItem( IDC_JOINT );
	m_jointrot_wnd = GetDlgItem( IDC_JOINTROT );

	return 0;
}

int CItiDlg::ParamsToDlg()
{
	char tempchar[2048];

	if( m_movemode == MOVEMODE_RELATIVE ){
		m_dlg_wnd.CheckRadioButton( IDC_MOVE_ABSOLUTE, IDC_MOVE_RELATIVE, IDC_MOVE_RELATIVE );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_MOVE_ABSOLUTE, IDC_MOVE_RELATIVE, IDC_MOVE_ABSOLUTE );
	}

	if( m_wheelscale != 0 ){
		m_dlg_wnd.CheckDlgButton( IDC_WHESCALE, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_WHESCALE, BST_UNCHECKED );
	}
	if( m_childjoint != 0 ){
		m_dlg_wnd.CheckDlgButton( IDC_CHILDJOINT, BST_CHECKED );
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_CHILDJOINT, BST_UNCHECKED );
	}


	CheckObjectButton();


	sprintf_s( tempchar, 2048, "%f", m_inputpos.x );
	m_posx_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 2048, "%f", m_inputpos.y );
	m_posy_wnd.SetWindowText( tempchar );
	sprintf_s( tempchar, 2048, "%f", m_inputpos.z );
	m_posz_wnd.SetWindowText( tempchar );
	
	return 0;
}

int CItiDlg::MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, int* shifvert, int shifvertnum, float pointx, float pointy )
{
	int ret;

	if( shiftpart > 0 ){

		switch( m_object ){
		case ITIOBJ_VERTEX:
			ret = MoveVertexScreen( papp, lpsh, lpmh, shiftpart, shifvert, shifvertnum, pointx, pointy );
			if( ret ){
				DbgOut( "ItiDlg : MoveObject : MoveVertexScreen error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			break;
		case ITIOBJ_FACE:
			if( m_itimode == 0 ){
				ret = MoveFaceScreen( papp, lpsh, lpmh, shiftpart, shiftface, pointx, pointy );
				if( ret ){
					DbgOut( "ItiDlg : MoveObject : MoveFaceScreen error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_PART:
			if( m_itimode == 0 ){
				ret = MovePartScreen( papp, lpsh, lpmh, shiftpart, pointx, pointy );
				if( ret ){
					DbgOut( "ItiDlg : MoveObject : MovePartScreen error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_JOINT:
			if( m_itimode == 0 ){
				ret = MoveJointScreen( papp, lpsh, lpmh, shiftpart, pointx, pointy );
				if( ret ){
					DbgOut( "ItiDlg : MoveObject : MoveJointScreen error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_JOINTROT:
			if( m_itimode == 0 ){
				ret = RotateJointScreen( papp, lpsh, lpmh, shiftpart, pointx, pointy );
				if( ret ){
					DbgOut( "ItiDlg : MoveObject : RotateJointScreen error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		default:
			_ASSERT( 0 );
			break;
		}

	}

//////////
	if( m_movemode == MOVEMODE_ABSOLUTE ){
		m_inputpos = m_worldpos;
	}
	
	ParamsToDlg();

	return 0;
}
int CItiDlg::MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, int* shifvert, int shifvertnum, int kind, float delta )
{
	int ret;

	if( shiftpart > 0 ){
		switch( m_object ){
		case ITIOBJ_VERTEX:
			ret = MoveVertexDelta( papp, lpsh, lpmh, shiftpart, shifvert, shifvertnum, kind, delta );
			if( ret ){
				DbgOut( "ItiDlg : MoveObject : MoveVertexDelta error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			break;
		case ITIOBJ_FACE:
			if( m_itimode == 0 ){
				ret = MoveFaceDelta( papp, lpsh, lpmh, shiftpart, shiftface, kind, delta );
				if( ret ){
					DbgOut( "itidlg : MoveObject : MoveFaceDelta error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_PART:
			if( m_itimode == 0 ){
				ret = MovePartDelta( papp, lpsh, lpmh, shiftpart, kind, delta );
				if( ret ){
					DbgOut( "itidlg : MoveObject : MovePartDelta error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_JOINT:
			if( m_itimode == 0 ){
				ret = MoveJointDelta( papp, lpsh, lpmh, shiftpart, kind, delta );
				if( ret ){
					DbgOut( "itidlg : MoveObject : MoveJointDelta error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		case ITIOBJ_JOINTROT:
			if( m_itimode == 0 ){
				ret = RotateJointDelta( papp, lpsh, lpmh, shiftpart, kind, delta );
				if( ret ){
					DbgOut( "itidlg : MoveObject : RotateJointDelta error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			break;
		default:
			_ASSERT( 0 );
			break;
		}
	}
//////////
	if( m_movemode == MOVEMODE_ABSOLUTE ){
		m_inputpos = m_worldpos;
	}
	
	ParamsToDlg();

	return 0;
}

int CItiDlg::MoveJointScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, float srcx, float srcy )
{
	int ret;


	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );
	D3DXVECTOR3 firstobj;
	ret = g_motdlg->GetBonePos( shiftpart, inimat, 1, &firstobj );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}



	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	ret = lpsh->GetBonePos( shiftpart, 1, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), &targetobj,
		lpmh, papp->m_matWorld, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveJointScreen : sh GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	targetsc.x = (float)srcx;
	targetsc.y = (float)srcy;
	targetsc.z = 0.0f;
	ret = papp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = (float)srcx;
	targetsc.y = (float)srcy;
	targetsc.z = 1.0f;
	ret = papp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = papp->m_degxz * (float)DEG2PAI;
	rady = papp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 target;
	ret = GetShadow( &target, planeaxis, targetobj, targetobj0, targetdir );
	if( ret )
		return 0;

	ret = lpsh->ChangeJointInitialPos( shiftpart, target );
	if( ret ){
		DbgOut( "itidlg : MoveJointScreen : sh ChangeJointInitialPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_childjoint == 1 ){
		D3DXVECTOR3 lastobj;
		ret = g_motdlg->GetBonePos( shiftpart, inimat, 1, &lastobj );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		D3DXVECTOR3 mvdiff = lastobj - firstobj;
		CShdElem* selem;
		selem = (*lpsh)( shiftpart );
		CShdElem* childselem;
		childselem = selem->child;
		if( childselem && childselem->IsJoint() ){
			MoveJointRelativeReq( papp, lpsh, lpmh, childselem->serialno, mvdiff, 1 );
		}
	}

////////

	ret = g_motdlg->GetBonePos( shiftpart, papp->m_matWorld, 1, &m_worldpos );
	if( ret ){
		DbgOut( "itidlg : MoveJointDelta : motdlg GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;

}



int CItiDlg::MovePartScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, float srcx, float srcy )
{
	int ret;
	D3DXVECTOR3 partpos;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );


	ret = lpsh->GetInitialPartPos( shiftpart, &partpos, inimat );
	if( ret ){
		DbgOut( "itidlg : MovePartScreen : shandler GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	
	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 0.0f;
	ret = papp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 1.0f;
	ret = papp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = papp->m_degxz * (float)DEG2PAI;
	rady = papp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 temptarget;
	ret = GetShadow( &temptarget, planeaxis, partpos, targetobj0, targetdir );
	if( ret )
		return 0;

	//targetobj = mvobj + 0.333f * ( temptarget - mvobj );
	targetobj = temptarget;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	D3DXVECTOR3 diff;
	diff = targetobj - partpos;

	D3DXVECTOR3 worlddiff;
	D3DXVec3TransformCoord( &worlddiff, &diff, &(papp->m_matWorld) );


	int vertnum = 0;
	ret = lpsh->GetVertNumOfPart( shiftpart, &vertnum );
	if( ret ){
		DbgOut( "itidlg : MovePartScreen : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int vertno;
	for( vertno = 0; vertno < vertnum; vertno++ ){
		ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &vertno, 1, worlddiff );
		if( ret ){
			DbgOut( "itidlg : MovePartScreen : MoveVertexRelative %d error !!!\n", vertno );
			_ASSERT( 0 );
			return 1;
		}
	}

////////
	ret = lpsh->GetInitialPartPos( shiftpart, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MovePartScreen : sh GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}



//DbgOut( "itidlg : MovePartScreen : vertnum %d, partpos %f %f %f, worldpos %f %f %f\r\n",
//	vertnum, partpos.x, partpos.y, partpos.z, m_worldpos.x, m_worldpos.y, m_worldpos.z );



	return 0;

}


int CItiDlg::MoveFaceScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, float srcx, float srcy )
{
	int ret;
	D3DXVECTOR3 facepos;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );


	ret = lpsh->GetInitialFacePos( shiftpart, shiftface, &facepos, inimat );
	if( ret ){
		DbgOut( "itidlg : MoveFaceScreen : shandler GetInitialFacePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	
	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 0.0f;
	ret = papp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 1.0f;
	ret = papp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = papp->m_degxz * (float)DEG2PAI;
	rady = papp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 temptarget;
	ret = GetShadow( &temptarget, planeaxis, facepos, targetobj0, targetdir );
	if( ret )
		return 0;

	//targetobj = mvobj + 0.333f * ( temptarget - mvobj );
	targetobj = temptarget;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	D3DXVECTOR3 diff;
	diff = targetobj - facepos;

	D3DXVECTOR3 worlddiff;
	D3DXVec3TransformCoord( &worlddiff, &diff, &(papp->m_matWorld) );


	int v1, v2, v3;
	ret = lpsh->GetVertNoOfFace( shiftpart, shiftface, &v1, &v2, &v3 );
	if( ret ){
		DbgOut( "itidlg : MoveFaceScreen : sh GetVertNoOfFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &v1, 1, worlddiff );
	if( ret ){
		DbgOut( "itidlg : MoveFaceScreen : MoveVertexRelative 1 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &v2, 1, worlddiff );
	if( ret ){
		DbgOut( "itidlg : MoveFaceScreen : MoveVertexRelative 2 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &v3, 1, worlddiff );
	if( ret ){
		DbgOut( "itidlg : MoveFaceScreen : MoveVertexRelative 3 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

////////
	ret = lpsh->GetInitialFacePos( shiftpart, shiftface, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MoveFaceScreen : sh GetInitialFacePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


int CItiDlg::MoveVertexScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, float srcx, float srcy )
{
	if( shifvertnum <= 0 )
		return 0;//!!!!!!!!!!!!


	int ret;
	D3DXVECTOR3 mvobj;
	int motid, frameno;
	motid = g_motdlg->GetMotCookie();
	frameno = g_motdlg->GetCurrentFrameNo();

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );


	ret = lpsh->GetVertPos2( lpmh, inimat, motid, frameno, shiftpart, *shifvert, &mvobj, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveVertex : GetVertPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	
	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 0.0f;
	ret = papp->ScreenToObj( &targetobj0, targetsc );
	if( ret ){
		_ASSERT( 0 );
		return 0;
	}

	targetsc.x = srcx;
	targetsc.y = srcy;
	targetsc.z = 1.0f;
	ret = papp->ScreenToObj( &targetobj1, targetsc );
	if( ret ){
		_ASSERT( 0 );
		return 0;
	}

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = papp->m_degxz * (float)DEG2PAI;
	rady = papp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 temptarget;
	ret = GetShadow( &temptarget, planeaxis, mvobj, targetobj0, targetdir );
	if( ret ){
		_ASSERT( 0 );
		return 0;
	}

	//targetobj = mvobj + 0.333f * ( temptarget - mvobj );
	targetobj = temptarget;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int shiftno;
	for( shiftno = 0; shiftno < shifvertnum; shiftno++ ){

		ret = lpsh->SetVertPos2( lpmh, motid, frameno, shiftpart, *( shifvert + shiftno ), targetobj );
		if( ret ){
			DbgOut( "itidlg : MoveVertex : lpsh MoveVertex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	D3DXVec3TransformCoord( &m_worldpos, &targetobj, &(papp->m_matWorld) );


	return 0;
}

int CItiDlg::MoveJointAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 absvec )
{
	int ret;

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );
	D3DXVECTOR3 firstobj;
	ret = g_motdlg->GetBonePos( shiftpart, inimat, 1, &firstobj );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	D3DXVECTOR3 targetobj;
	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &targetobj, &absvec, &invw );

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	ret = lpsh->ChangeJointInitialPos( shiftpart, targetobj );
	if( ret ){
		DbgOut( "itidlg : MoveJointAbsolute : sh ChangeJointInitialPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_childjoint == 1 ){
		D3DXVECTOR3 lastobj;
		ret = g_motdlg->GetBonePos( shiftpart, inimat, 1, &lastobj );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		D3DXVECTOR3 mvdiff = lastobj - firstobj;
		CShdElem* selem;
		selem = (*lpsh)( shiftpart );
		CShdElem* childselem;
		childselem = selem->child;
		if( childselem && childselem->IsJoint() ){
			MoveJointRelativeReq( papp, lpsh, lpmh, childselem->serialno, mvdiff, 1 );
		}
	}


////////

	ret = g_motdlg->GetBonePos( shiftpart, papp->m_matWorld, 1, &m_worldpos );
	if( ret ){
		DbgOut( "itidlg : MoveJointAbsolute : motdlg GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;

}

int CItiDlg::MovePartAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 absvec )
{
	D3DXVECTOR3 targetobj;
	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &targetobj, &absvec, &invw );

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	int ret;
	D3DXVECTOR3 partpos;
	ret = lpsh->GetInitialPartPos( shiftpart, &partpos, inimat );
	if( ret ){
		DbgOut( "itidlg : MovePartAbsolute : sh GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 diff, worlddiff;
	diff = targetobj - partpos;
	D3DXVec3TransformCoord( &worlddiff, &diff, &(papp->m_matWorld) );


	int vertnum;
	ret = lpsh->GetVertNumOfPart( shiftpart, &vertnum );
	if( ret ){
		DbgOut( "itidlg : MovePartAbsolute : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int vertno;
	for( vertno = 0; vertno < vertnum; vertno++ ){
		ret = MoveVertexRelative( m_papp, lpsh, lpmh, shiftpart, &vertno, 1, worlddiff );
		if( ret ){
			DbgOut( "itidlg : MovePartAbsolute : MoveVertexRelative %d error !!!\n", vertno );
			_ASSERT( 0 );
			return 1;
		}
	}

//////////
	ret = lpsh->GetInitialPartPos( shiftpart, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MovePartAbsolute : sh GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


int CItiDlg::MoveFaceAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, D3DXVECTOR3 absvec )
{
	D3DXVECTOR3 targetobj;
	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &targetobj, &absvec, &invw );

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	int ret;
	D3DXVECTOR3 facepos;
	ret = lpsh->GetInitialFacePos( shiftpart, shiftface, &facepos, inimat );
	if( ret ){
		DbgOut( "itidlg : MoveFaceAbsolute : sh GetInitialFacePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 diff, worlddiff;
	diff = targetobj - facepos;
	D3DXVec3TransformCoord( &worlddiff, &diff, &(papp->m_matWorld) );

	int v1, v2, v3;
	ret = lpsh->GetVertNoOfFace( shiftpart, shiftface, &v1, &v2, &v3 );
	if( ret ){
		DbgOut( "itidlg : MoveFaceAbsolute : sh GetVertNoOfFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = MoveVertexRelative( m_papp, lpsh, lpmh, shiftpart, &v1, 1, worlddiff );
	if( ret ){
		DbgOut( "itidlg : MoveFaceAbsolute : MoveVertexRelative 1 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexRelative( m_papp, lpsh, lpmh, shiftpart, &v2, 1, worlddiff );
	if( ret ){
		DbgOut( "itidlg : MoveFaceAbsolute : MoveVertexRelative 2 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexRelative( m_papp, lpsh, lpmh, shiftpart, &v3, 1, worlddiff );
	if( ret ){
		DbgOut( "itidlg : MoveFaceAbsolute : MoveVertexRelative 3 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//////////
	ret = lpsh->GetInitialFacePos( shiftpart, shiftface, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MoveFaceAbsolute : sh GetInitialFacePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}



int CItiDlg::MoveVertexAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, D3DXVECTOR3 absvec )
{
	if( shifvertnum <= 0 )
		return 0;//!!!!!!!!!!!!!


	D3DXVECTOR3 targetobj;
	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &targetobj, &absvec, &invw );

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int ret;
	int motid, frameno;
	motid = g_motdlg->GetMotCookie();
	frameno = g_motdlg->GetCurrentFrameNo();

	int shiftno;
	for( shiftno = 0; shiftno < shifvertnum; shiftno++ ){
		ret = lpsh->SetVertPos2( lpmh, motid, frameno, shiftpart, *( shifvert + shiftno ), targetobj );
		if( ret ){
			DbgOut( "itidlg : MoveVertex : lpsh MoveVertex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	D3DXVec3TransformCoord( &m_worldpos, &targetobj, &(papp->m_matWorld) );

	return 0;
}

LRESULT CItiDlg::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	if( (m_object == ITIOBJ_JOINT) && (m_wheelscale == 1) ){

		float srcscale = 1.0f;

		int curdelta;
		curdelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if( curdelta >= 0 ){
			srcscale = 1.05f;
		}else{
			srcscale = 0.95f;
		}

		ret = ScaleBoneTreePos( m_papp->m_shandler, srcscale );
		_ASSERT( !ret );
	}
	return DefWindowProc();
}


int CItiDlg::ScaleBoneTreePos( CShdHandler* lpsh, float srcscale )
{
	int ret;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	int seri;
	CShdElem* selem;
	for( seri = 0; seri < lpsh->s2shd_leng; seri++ ){
		selem = (*lpsh)( seri );
		if( selem->IsJoint() ){
			D3DXVECTOR3 mvobj;
			ret = g_motdlg->GetBonePos( seri, inimat, 1, &mvobj );
			if( ret ){
				DbgOut( "itidlg : ScaleBoneTreePos : motdlg GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			D3DXVECTOR3 targetobj;
			targetobj = srcscale * mvobj;

			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//targetobjは、ボーン変換後のローカル座標
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			ret = lpsh->ChangeJointInitialPos( seri, targetobj );
			if( ret ){
				DbgOut( "itidlg : ScaleBoneTreePos : sh ChangeJointInitialPos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}

int CItiDlg::MoveJointRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 relvec, int mvchild )
{

	int ret;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	D3DXVECTOR3 mvobj;
	ret = g_motdlg->GetBonePos( shiftpart, inimat, 1, &mvobj );
	if( ret ){
		DbgOut( "itidlg : MoveJointAbsolute : motdlg GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DXVECTOR3 diff;
	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &diff, &relvec, &invw );

	D3DXVECTOR3 targetobj;

	targetobj = mvobj + diff;


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	ret = lpsh->ChangeJointInitialPos( shiftpart, targetobj );
	if( ret ){
		DbgOut( "itidlg : MoveJointRelative : sh ChangeJointInitialPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( mvchild && (m_childjoint == 1) ){
		CShdElem* selem;
		selem = (*lpsh)( shiftpart );
		CShdElem* childselem;
		childselem = selem->child;
		if( childselem && childselem->IsJoint() ){
			MoveJointRelativeReq( papp, lpsh, lpmh, childselem->serialno, diff, 1 );
		}
	}

////////

	ret = g_motdlg->GetBonePos( shiftpart, papp->m_matWorld, 1, &m_worldpos );
	if( ret ){
		DbgOut( "itidlg : MoveJointRelative : motdlg GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

int CItiDlg::MovePartRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 relvec )
{
	int ret;
	int vertnum;

	ret = lpsh->GetVertNumOfPart( shiftpart, &vertnum );
	if( ret ){
		DbgOut( "itidlg : MovePartRelative : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int vertno;
	for( vertno = 0; vertno < vertnum; vertno++ ){
		ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &vertno, 1, relvec );
		if( ret ){
			DbgOut( "itidlg : MoveFaceRelative : MoveVertexRelative %d error !!!\n", vertno );
			_ASSERT( 0 );
			return 1;
		}
	}

////////////
	ret = lpsh->GetInitialPartPos( shiftpart, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MovePartRelative : sh GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}



int CItiDlg::MoveFaceRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, D3DXVECTOR3 relvec )
{
	int ret;
	int v1, v2, v3;

	ret = lpsh->GetVertNoOfFace( shiftpart, shiftface, &v1, &v2, &v3 );
	if( ret ){
		DbgOut( "itidlg : MoveFaceRelative : sh GetVertNoOfFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &v1, 1, relvec );
	if( ret ){
		DbgOut( "itidlg : MoveFaceRelative : MoveVertexRelative 1 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &v2, 1, relvec );
	if( ret ){
		DbgOut( "itidlg : MoveFaceRelative : MoveVertexRelative 2 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexRelative( papp, lpsh, lpmh, shiftpart, &v3, 1, relvec );
	if( ret ){
		DbgOut( "itidlg : MoveFaceRelative : MoveVertexRelative 3 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

////////////
	ret = lpsh->GetInitialFacePos( shiftpart, shiftface, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MoveFaceRelative : sh GetInitialFacePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


int CItiDlg::MoveVertexRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, D3DXVECTOR3 relvec )
{
	if( shifvertnum <= 0 )
		return 0;//!!!!!!!!!!!!!

	int ret;
	D3DXVECTOR3 mvobj;
	int motid, frameno;
	motid = g_motdlg->GetMotCookie();
	frameno = g_motdlg->GetCurrentFrameNo();

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	ret = lpsh->GetVertPos2( lpmh, inimat, motid, frameno, shiftpart, *shifvert, &mvobj, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveVertex : GetVertPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DXVECTOR3 diff;
	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &diff, &relvec, &invw );

	D3DXVECTOR3 targetobj;

	targetobj = mvobj + diff;


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int shiftno;
	for( shiftno = 0; shiftno < shifvertnum; shiftno++ ){

		ret = lpsh->SetVertPos2( lpmh, motid, frameno, shiftpart, *( shifvert + shiftno ), targetobj );
		if( ret ){
			DbgOut( "itidlg : MoveVertex : lpsh MoveVertex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	D3DXVec3TransformCoord( &m_worldpos, &targetobj, &(papp->m_matWorld) );

	return 0;
}


int CItiDlg::MoveJointDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int kind, float delta )
{
	int ret;
	D3DXVECTOR3 target, targetsc;
	D3DXVECTOR3 bonepos;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	ret = g_motdlg->GetBonePos( shiftpart, inimat, 1, &bonepos );
	if( ret ){
		DbgOut( "itidlg : MoveJointDelta : motdlg GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 tempdiff, diff;
	switch( kind ){
	case 0:
		tempdiff.x = delta;
		tempdiff.y = 0.0f;
		tempdiff.z = 0.0f;
		break;
	case 1:
		tempdiff.x = 0.0f;
		tempdiff.y = delta;
		tempdiff.z = 0.0f;
		break;
	case 2:
		tempdiff.x = 0.0f;
		tempdiff.y = 0.0f;
		tempdiff.z = delta;
		break;
	default:
		_ASSERT( 0 );
		tempdiff.x = delta;
		tempdiff.y = 0.0f;
		tempdiff.z = 0.0f;
		break;
	}

	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &diff, &tempdiff, &invw );

	target = bonepos + diff;

	ret = lpsh->ChangeJointInitialPos( shiftpart, target );
	if( ret ){
		DbgOut( "itidlg : MoveJointDelta : sh ChangeJointInitialPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_childjoint == 1 ){
		CShdElem* selem;
		selem = (*lpsh)( shiftpart );
		CShdElem* childselem;
		childselem = selem->child;
		if( childselem && childselem->IsJoint() ){
			MoveJointRelativeReq( papp, lpsh, lpmh, childselem->serialno, diff, 1 );
		}
	}

////////

	ret = g_motdlg->GetBonePos( shiftpart, papp->m_matWorld, 1, &m_worldpos );
	if( ret ){
		DbgOut( "itidlg : MoveJointDelta : motdlg GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CItiDlg::MovePartDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int kind, float delta )
{
	int ret;
	int vertnum = 0;
	ret = lpsh->GetVertNumOfPart( shiftpart, &vertnum );
	if( ret ){
		DbgOut( "itidlg : MovePartDelta : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int vertno;
	for( vertno = 0; vertno < vertnum; vertno++ ){
		ret = MoveVertexDelta( papp, lpsh, lpmh, shiftpart, &vertno, 1, kind, delta );
		if( ret ){
			DbgOut( "itidlg : MoveFaceDelta : MoveVertexDelta %d error !!!\n", vertno );
			_ASSERT( 0 );
			return 1;
		}
	}

////////
	ret = lpsh->GetInitialPartPos( shiftpart, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MovePartDelta : sh GetInitialPartPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


int CItiDlg::MoveFaceDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, int kind, float delta )
{
	int ret;
	int v1, v2, v3;
	ret = lpsh->GetVertNoOfFace( shiftpart, shiftface, &v1, &v2, &v3 );
	if( ret ){
		DbgOut( "itidlg : MoveFaceDelta : sh GetVertNoOfFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = MoveVertexDelta( papp, lpsh, lpmh, shiftpart, &v1, 1, kind, delta );
	if( ret ){
		DbgOut( "itidlg : MoveFaceDelta : MoveVertexDelta 1 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexDelta( papp, lpsh, lpmh, shiftpart, &v2, 1, kind, delta );
	if( ret ){
		DbgOut( "itidlg : MoveFaceDelta : MoveVertexDelta 2 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = MoveVertexDelta( papp, lpsh, lpmh, shiftpart, &v3, 1, kind, delta );
	if( ret ){
		DbgOut( "itidlg : MoveFaceDelta : MoveVertexDelta 3 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

////////
	ret = lpsh->GetInitialFacePos( shiftpart, shiftface, &m_worldpos, papp->m_matWorld );
	if( ret ){
		DbgOut( "itidlg : MoveFaceDelta : sh GetInitialFacePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CItiDlg::MoveVertexDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, int kind, float delta )
{
	if( shifvertnum <= 0 )
		return 0;//!!!!!!!!!!!!!


	D3DXVECTOR3 tempdiff, diff;
	switch( kind ){
	case 0:
		tempdiff.x = delta;
		tempdiff.y = 0.0f;
		tempdiff.z = 0.0f;
		break;
	case 1:
		tempdiff.x = 0.0f;
		tempdiff.y = delta;
		tempdiff.z = 0.0f;
		break;
	case 2:
		tempdiff.x = 0.0f;
		tempdiff.y = 0.0f;
		tempdiff.z = delta;
		break;
	default:
		_ASSERT( 0 );
		tempdiff.x = delta;
		tempdiff.y = 0.0f;
		tempdiff.z = 0.0f;
		break;
	}

	D3DXMATRIX invw;
	D3DXMatrixInverse( &invw, NULL, &(papp->m_matWorld) );
	D3DXVec3TransformCoord( &diff, &tempdiff, &invw );


	int ret;
	D3DXVECTOR3 mvobj;
	int motid, frameno;
	motid = g_motdlg->GetMotCookie();
	frameno = g_motdlg->GetCurrentFrameNo();

	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	ret = lpsh->GetVertPos2( lpmh, inimat, motid, frameno, shiftpart, *shifvert, &mvobj, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveVertex : GetVertPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 targetobj;
	targetobj = mvobj + diff;


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//targetobjは、ボーン変換後のローカル座標
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int shiftno;
	for( shiftno = 0; shiftno < shifvertnum; shiftno++ ){
		ret = lpsh->SetVertPos2( lpmh, motid, frameno, shiftpart, *( shifvert + shiftno ), targetobj );
		if( ret ){
			DbgOut( "itidlg : MoveVertex : lpsh MoveVertex error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	D3DXVec3TransformCoord( &m_worldpos, &targetobj, &(papp->m_matWorld) );

	return 0;
}


int CItiDlg::GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir )
{

		//planepを通り、planedirを法線ベクトルとする平面：ax + by + cz + d = 0;
	float a, b, c, d;
	a = planedir.x; b = planedir.y; c = planedir.z;
	d = -a * planep.x - b * planep.y - c * planep.z;

		//平面ax+by+cz+d=0と、点pobj1を通り方向ベクトルdirectionの直線との交点、targetを求める。
		//OpenGL赤本p402参照
	D3DXMATRIX mat;
	mat._11 = b * srcdir.y + c * srcdir.z;
	mat._12 = -a * srcdir.y;
	mat._13 = -a * srcdir.z;
	mat._14 = 0.0f;

	mat._21 = -b * srcdir.x;
	mat._22 = a * srcdir.x + c * srcdir.z;
	mat._23 = -b * srcdir.z;
	mat._24 = 0.0f;

	mat._31 = -c * srcdir.x;
	mat._32 = -c * srcdir.y;
	mat._33 = a * srcdir.x + b * srcdir.y;
	mat._34 = 0.0f;

	mat._41 = -d * srcdir.x;
	mat._42 = -d * srcdir.y;
	mat._43 = -d * srcdir.z;
	mat._44 = a * srcdir.x + b * srcdir.y + c * srcdir.z;

	if( mat._44 == 0.0f )
		return 1;

	dstshadow->x = (srcp.x * mat._11 + srcp.y * mat._21 + srcp.z * mat._31 + mat._41) / mat._44;
	dstshadow->y = (srcp.x * mat._12 + srcp.y * mat._22 + srcp.z * mat._32 + mat._42) / mat._44;
	dstshadow->z = (srcp.x * mat._13 + srcp.y * mat._23 + srcp.z * mat._33 + mat._43) / mat._44;


	return 0;
}
int CItiDlg::DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec )
{
	float mag;
	mag = srcvec->x * srcvec->x + srcvec->y * srcvec->y + srcvec->z * srcvec->z;

	float sqmag;
	sqmag = (float)sqrt( mag );

	if( sqmag != 0.0f ){
		float magdiv;
		magdiv = 1.0f / sqmag;
//		dstvec->x *= magdiv;
//		dstvec->y *= magdiv;
//		dstvec->z *= magdiv;
		dstvec->x = srcvec->x * magdiv;
		dstvec->y = srcvec->y * magdiv;
		dstvec->z = srcvec->z * magdiv;

	}else{
		//DbgOut( "viewer : DXVec3Normalize : zero warning !!!\n" );
		dstvec->x = 0.0f;
		dstvec->y = 0.0f;
		dstvec->z = 0.0f;
		_ASSERT( 0 );
	}

	return 0;
}
LRESULT CItiDlg::OnSelVert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//DbgOut( "itidlg : OnSelVert\r\n" );

	m_object = ITIOBJ_VERTEX;
	CheckObjectButton();

	m_papp->m_shiftpart = -1;
	m_papp->m_shifvertnum = 0;
	m_papp->m_shiftface = -1;

	return 0;
}
LRESULT CItiDlg::OnSelFace(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//DbgOut( "itidlg : OnSelFace\r\n" );

	m_object = ITIOBJ_FACE;
	CheckObjectButton();

	m_papp->m_shiftpart = -1;
	m_papp->m_shifvertnum = 0;
	m_papp->m_shiftface = -1;

	return 0;
}
LRESULT CItiDlg::OnSelPart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_object = ITIOBJ_PART;
	CheckObjectButton();

	m_papp->m_shiftpart = -1;
	m_papp->m_shifvertnum = 0;
	m_papp->m_shiftface = -1;

	return 0;
}
LRESULT CItiDlg::OnSelJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_object = ITIOBJ_JOINT;
	CheckObjectButton();

	m_papp->m_shiftpart = -1;
	m_papp->m_shifvertnum = 0;
	m_papp->m_shiftface = -1;

	return 0;
}
LRESULT CItiDlg::OnSelJointRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_object = ITIOBJ_JOINTROT;
	CheckObjectButton();

	m_papp->m_shiftpart = -1;
	m_papp->m_shifvertnum = 0;
	m_papp->m_shiftface = -1;

	return 0;
}

int CItiDlg::CheckObjectButton()
{
	//BS_PUSHLIKE

	switch( m_object ){
	case ITIOBJ_VERTEX:
		SendMessage( m_vert_wnd, BM_SETSTATE, TRUE, 0 );  
		SendMessage( m_face_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_part_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_joint_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_jointrot_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case ITIOBJ_FACE:
		SendMessage( m_vert_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_face_wnd, BM_SETSTATE, TRUE, 0 );  
		SendMessage( m_part_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_joint_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_jointrot_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case ITIOBJ_PART:
		SendMessage( m_vert_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_face_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_part_wnd, BM_SETSTATE, TRUE, 0 );  
		SendMessage( m_joint_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_jointrot_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case ITIOBJ_JOINT:
		SendMessage( m_vert_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_face_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_part_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_joint_wnd, BM_SETSTATE, TRUE, 0 );  
		SendMessage( m_jointrot_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case ITIOBJ_JOINTROT:
		SendMessage( m_vert_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_face_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_part_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_joint_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_jointrot_wnd, BM_SETSTATE, TRUE, 0 );  
		break;
	default:
		SendMessage( m_vert_wnd, BM_SETSTATE, TRUE, 0 );  
		SendMessage( m_face_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_part_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_joint_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_jointrot_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	}

	//BST_INDETERMINATE

	return 0;
}

int CItiDlg::SetItiMode( int itimode )
{
	m_itimode = itimode;

	if( m_itimode == 0 ){
		m_vert_wnd.EnableWindow( TRUE );
		m_face_wnd.EnableWindow( TRUE );
		m_part_wnd.EnableWindow( TRUE );
		m_joint_wnd.EnableWindow( TRUE );

		CheckObjectButton();

	}else{
		m_object = ITIOBJ_VERTEX;//!!!!!!!!!!!!

		m_dlg_wnd.CheckDlgButton( IDC_VERT, BST_CHECKED );
		m_dlg_wnd.CheckDlgButton( IDC_FACE, BST_UNCHECKED );
		m_dlg_wnd.CheckDlgButton( IDC_PART, BST_UNCHECKED );
		m_dlg_wnd.CheckDlgButton( IDC_JOINT, BST_UNCHECKED );

		CheckObjectButton();

		m_vert_wnd.EnableWindow( TRUE );
		m_face_wnd.EnableWindow( FALSE );
		m_part_wnd.EnableWindow( FALSE );
		m_joint_wnd.EnableWindow( FALSE );
	}

	return 0;
}

LRESULT CItiDlg::OnSelRelative(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_movemode = MOVEMODE_RELATIVE;
	ParamsToDlg();
	return 0;
}
LRESULT CItiDlg::OnSelAbsolute(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_movemode = MOVEMODE_ABSOLUTE;

	m_inputpos = m_worldpos;//!!!!!!!!!!!!!!

	ParamsToDlg();

	return 0;
}

int CItiDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;

	if( m_papp->m_shandler ){
		if( m_cmdshow != SW_HIDE ){
			m_papp->m_shandler->SetMorphDispFlag( 0 );
		}else{
			m_papp->m_shandler->SetMorphDispFlag( 1 );
		}
	}


	ShowWindow( m_cmdshow );
	ParamsToDlg();

	return 0;
}

LRESULT CItiDlg::OnWheScale(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_WHESCALE );
	if( ischecked == BST_CHECKED ){
		m_wheelscale = 1;
	}else{
		m_wheelscale = 0;
	}

	return 0;
}

LRESULT CItiDlg::OnChildJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_CHILDJOINT );
	if( ischecked == BST_CHECKED ){
		m_childjoint = 1;
	}else{
		m_childjoint = 0;
	}

	return 0;
}

void CItiDlg::MoveJointRelativeReq( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 relvec, int broflag )
{
	int ret;
	ret = MoveJointRelative( papp, lpsh, lpmh, shiftpart, relvec, 0 );
	_ASSERT( !ret );

	CShdElem* selem = (*lpsh)( shiftpart );
	if( selem->child && selem->child->IsJoint() ){
		MoveJointRelativeReq( papp, lpsh, lpmh, selem->child->serialno, relvec, 1 );
	}

	if( broflag == 1 ){
		if( selem->brother && selem->brother->IsJoint() ){
			MoveJointRelativeReq( papp, lpsh, lpmh, selem->brother->serialno, relvec, 1 );
		}
	}
}

int CItiDlg::RotateJointScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, float srcx, float srcy )
{
	int ret;

	CShdElem* selem;
	selem = (*lpsh)( shiftpart );
	CShdElem* parselem;
	parselem = lpsh->FindUpperJoint( selem );
	if( !parselem ){
		return 0;
	}

	D3DXVECTOR3 targetsc, targetobj, targetobj0, targetobj1;
	ret = lpsh->GetBonePos( shiftpart, 1, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), &targetobj,
		lpmh, papp->m_matWorld, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveJointScreen : sh GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	targetsc.x = (float)srcx;
	targetsc.y = (float)srcy;
	targetsc.z = 0.0f;
	ret = papp->ScreenToObj( &targetobj0, targetsc );
	if( ret )
		return 0;

	targetsc.x = (float)srcx;
	targetsc.y = (float)srcy;
	targetsc.z = 1.0f;
	ret = papp->ScreenToObj( &targetobj1, targetsc );
	if( ret )
		return 0;

	D3DXVECTOR3 targetdir;
	targetdir = targetobj1 - targetobj0;
	DXVec3Normalize( &targetdir, &targetdir );

	float radxz, rady;
	radxz = papp->m_degxz * (float)DEG2PAI;
	rady = papp->m_degy * (float)DEG2PAI;

	D3DXVECTOR3 planeaxis;
	planeaxis.x = sinf(radxz) * cosf( rady );
	planeaxis.y = sinf( rady );
	planeaxis.z = -cosf(radxz) * cosf( rady );


	D3DXVECTOR3 target;
	ret = GetShadow( &target, planeaxis, targetobj, targetobj0, targetdir );
	if( ret )
		return 0;

	D3DXVECTOR3 parpos;
	ret = lpsh->GetBonePos( parselem->serialno, 1, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), &parpos,
		lpmh, papp->m_matWorld, 1 );
	if( ret ){
		DbgOut( "itidlg : MoveJointScreen : sh GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 befvec, aftvec;
	befvec = targetobj - parpos;
	DXVec3Normalize( &befvec, &befvec );

	aftvec = target - parpos;
	DXVec3Normalize( &aftvec, &aftvec );

	CQuaternion multq;
	multq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
	multq.RotationArc( befvec, aftvec );


///////
	CMatrix2 mat, befrot, aftrot, rot;
	befrot.Identity();
	aftrot.Identity();
	rot.Identity();
	befrot.SetMoveMat( -parpos.x, -parpos.y, -parpos.z );
	aftrot.SetMoveMat( parpos.x, parpos.y, parpos.z );
	rot = multq.MakeRotMat();
	mat = befrot * rot * aftrot;
		

	D3DXVECTOR3 respos;
	respos.x = mat[0][0] * targetobj.x + mat[1][0] * targetobj.y + mat[2][0] * targetobj.z + mat[3][0];
	respos.y = mat[0][1] * targetobj.x + mat[1][1] * targetobj.y + mat[2][1] * targetobj.z + mat[3][1];
	respos.z = mat[0][2] * targetobj.x + mat[1][2] * targetobj.y + mat[2][2] * targetobj.z + mat[3][2];

	ret = lpsh->ChangeJointInitialPos( shiftpart, respos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

///////
	if( m_childjoint ){
		CShdElem* chilj;
		chilj = selem->child;
		if( chilj && chilj->IsJoint() ){
			RotateJointReq( papp, lpsh, lpmh, chilj->serialno, targetobj, multq, 1 );
		}
	}


	ret = g_motdlg->GetBonePos( shiftpart, papp->m_matWorld, 1, &m_worldpos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
int CItiDlg::RotateJointDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int kind, float delta )
{
	CShdElem* selem;
	selem = (*lpsh)( shiftpart );
	CShdElem* parselem;
	parselem = lpsh->FindUpperJoint( selem );
	if( !parselem ){
		return 0;
	}


	CQuaternion multq;
	D3DXVECTOR3 axis;
	switch( kind ){
	case 0:
		axis.x = 0.0f;
		axis.y = 1.0f;
		axis.z = 0.0f;
		break;
	case 1:
		axis.x = 0.0f;
		axis.y = 0.0f;
		axis.z = 1.0f;
		break;
	case 2:
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
		break;
	default:
		_ASSERT( 0 );
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
		break;
	}
	multq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
	multq.SetAxisAndRot( axis, delta * 0.008f );

	int ret;
	D3DXVECTOR3 targetobj;
	ret = lpsh->GetBonePos( shiftpart, 1, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), &targetobj,
		lpmh, papp->m_matWorld, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	D3DXVECTOR3 parpos;
	ret = lpsh->GetBonePos( parselem->serialno, 1, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), &parpos,
		lpmh, papp->m_matWorld, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	CMatrix2 mat, befrot, aftrot, rot;
	befrot.Identity();
	aftrot.Identity();
	rot.Identity();
	befrot.SetMoveMat( -parpos.x, -parpos.y, -parpos.z );
	aftrot.SetMoveMat( parpos.x, parpos.y, parpos.z );
	rot = multq.MakeRotMat();
	mat = befrot * rot * aftrot;
		

	D3DXVECTOR3 respos;
	respos.x = mat[0][0] * targetobj.x + mat[1][0] * targetobj.y + mat[2][0] * targetobj.z + mat[3][0];
	respos.y = mat[0][1] * targetobj.x + mat[1][1] * targetobj.y + mat[2][1] * targetobj.z + mat[3][1];
	respos.z = mat[0][2] * targetobj.x + mat[1][2] * targetobj.y + mat[2][2] * targetobj.z + mat[3][2];

	ret = lpsh->ChangeJointInitialPos( shiftpart, respos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

///////
	if( m_childjoint ){
		CShdElem* chilj;
		chilj = selem->child;
		if( chilj && chilj->IsJoint() ){
			RotateJointReq( papp, lpsh, lpmh, chilj->serialno, targetobj, multq, 1 );
		}
	}


	ret = g_motdlg->GetBonePos( shiftpart, papp->m_matWorld, 1, &m_worldpos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
void CItiDlg::RotateJointReq( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, 
	int shiftpart, D3DXVECTOR3 befpar, CQuaternion multq, int broflag )
{
	int ret;
	CShdElem* selem = (*lpsh)( shiftpart );
	CShdElem* parselem = lpsh->FindUpperJoint( selem );
	D3DXVECTOR3 chilobj = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	if( parselem ){
		D3DXMATRIX inimat;
		D3DXMatrixIdentity( &inimat );
		ret = g_motdlg->GetBonePos( shiftpart, inimat, 0, &chilobj );
		if( ret ){
			_ASSERT( 0 );
			return;
		}
		D3DXVECTOR3 parobj;
		ret = g_motdlg->GetBonePos( parselem->serialno, inimat, 0, &parobj );
		if( ret ){
			_ASSERT( 0 );
			return;
		}

		D3DXVECTOR3 basevec, aftorg;
		basevec = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		aftorg = chilobj - befpar;
		float orgleng;
		orgleng = D3DXVec3Length( &aftorg );
		DXVec3Normalize( &aftorg, &aftorg );
		CQuaternion orgq;
		orgq.RotationArc( basevec, aftorg );

		orgq = multq * orgq;


		CMatrix2 mat, rot;
		rot.Identity();
		rot = orgq.MakeRotMat();

		D3DXVECTOR3 respos;
		respos.x = rot[0][0] * basevec.x + rot[1][0] * basevec.y + rot[2][0] * basevec.z;
		respos.y = rot[0][1] * basevec.x + rot[1][1] * basevec.y + rot[2][1] * basevec.z;
		respos.z = rot[0][2] * basevec.x + rot[1][2] * basevec.y + rot[2][2] * basevec.z;
		respos = respos * orgleng + parobj;

		ret = lpsh->ChangeJointInitialPos( shiftpart, respos );
		if( ret ){
			_ASSERT( 0 );
			return;
		}
	}

	if( selem->child && selem->child->IsJoint() ){
		RotateJointReq( papp, lpsh, lpmh, selem->child->serialno, chilobj, multq, 1 );
	}

	if( broflag == 1 ){
		if( selem->brother && selem->brother->IsJoint() ){
			RotateJointReq( papp, lpsh, lpmh, selem->child->serialno, befpar, multq, 1 );
		}
	}
}

