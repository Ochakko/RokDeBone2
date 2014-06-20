// BonePropDlg.cpp : CBonePropDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "BonePropDlg.h"
#include <BoneProp.h>

#include <Commdlg.h>
//#include <Afxdlgs.h>

#define DBGH
#include <dbg.h>

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <shdelem.h>
#include <mothandler.h>
#include <motionctrl.h>
#include <motioninfo.h>

#include <motionpoint2.h>
#include <quaternion.h>

#include "GetDlgParams.h"
#include "motdlg.h"

#include "viewer.h"

#include <SymmOpe.h>

//////////////////////////
// extern
extern CMotDlg* g_motdlg;


/////////////////////////////////////////////////////////////////////////////
// CBonePropDlg

CBonePropDlg::CBonePropDlg( CMyD3DApplication* papp )
{
	InitParams();
	m_papp = papp;
	m_cmdshow = SW_HIDE;
}

CBonePropDlg::~CBonePropDlg()
{

}

int CBonePropDlg::InitParams()
{
	m_serialno = -1;
	ZeroMemory( &m_rot, sizeof( D3DXVECTOR3 ) );
	ZeroMemory( &m_mv, sizeof( D3DXVECTOR3 ) );
	ZeroMemory( &m_sc, sizeof( D3DXVECTOR3 ) );
	m_nameptr = 0;

	m_lim0.x = -179.0;
	m_lim0.y = -179.0;
	m_lim0.z = -179.0;

	m_lim1.x = 179.0;
	m_lim1.y = 179.0;
	m_lim1.z = 179.0;

	m_ignorelim = 1;

	m_zakind = ZA_1;

	return 0;
}

LRESULT CBonePropDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

//DbgOut( "BonePropDlg : OnInitDialog\r\n" );

	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CBonePropDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowDlg( SW_HIDE );

	return 0;
}

LRESULT CBonePropDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowDlg( SW_HIDE );

	return 0;
}

void CBonePropDlg::SetWnd()
{
	m_rotx_wnd = GetDlgItem( IDC_ROTX );
	m_roty_wnd = GetDlgItem( IDC_ROTY );
	m_rotz_wnd = GetDlgItem( IDC_ROTZ );

	m_limx0_wnd = GetDlgItem( IDC_LIMX0 );
	m_limx1_wnd = GetDlgItem( IDC_LIMX1 );
	m_limy0_wnd = GetDlgItem( IDC_LIMY0 );
	m_limy1_wnd = GetDlgItem( IDC_LIMY1 );
	m_limz0_wnd = GetDlgItem( IDC_LIMZ0 );
	m_limz1_wnd = GetDlgItem( IDC_LIMZ1 );

	m_mvx_wnd = GetDlgItem( IDC_MVX );
	m_mvy_wnd = GetDlgItem( IDC_MVY );
	m_mvz_wnd = GetDlgItem( IDC_MVZ );

	m_scx_wnd = GetDlgItem( IDC_SCX );
	m_scy_wnd = GetDlgItem( IDC_SCY );
	m_scz_wnd = GetDlgItem( IDC_SCZ );

	m_name_wnd = GetDlgItem( IDC_BONENAME );

	m_ignlim_wnd = GetDlgItem( IDC_IGNLIM );

	m_za_wnd = GetDlgItem( IDC_ZA_ROT );

	m_dlg_wnd = m_hWnd;

}

int CBonePropDlg::ParamsToDlg()
{	
	static D3DXVECTOR3 s_rot( 1.2345f, 1.2345f, 1.2345f );
	static DVEC3 s_lim0 = { 0.0, 0.0, 0.0 };
	static DVEC3 s_lim1 = { 0.0, 0.0, 0.0 };
	static D3DXVECTOR3 s_mv( 1.2345f, 1.2345f, 1.2345f );
	static D3DXVECTOR3 s_sc( 1.2345f, 1.2345f, 1.2345f );
	static int s_ignorelim = -1;

	char tempchar[256];

	if( m_rot.x != s_rot.x ){
		sprintf_s( tempchar, 256, "%f", m_rot.x );
		m_rotx_wnd.SetWindowText( tempchar );
	}
	if( m_rot.y != s_rot.y ){
		sprintf_s( tempchar, 256, "%f", m_rot.y );
		m_roty_wnd.SetWindowText( tempchar );
	}
	if( m_rot.z != s_rot.z ){
		sprintf_s( tempchar, 256, "%f", m_rot.z );
		m_rotz_wnd.SetWindowText( tempchar );
	}

	if( m_lim0.x != s_lim0.x ){
		sprintf_s( tempchar, 256, "%f", m_lim0.x );
		m_limx0_wnd.SetWindowText( tempchar );
	}
	if( m_lim1.x != s_lim1.x ){
		sprintf_s( tempchar, 256, "%f", m_lim1.x );
		m_limx1_wnd.SetWindowText( tempchar );
	}
	if( m_lim0.y != s_lim0.y ){
		sprintf_s( tempchar, 256, "%f", m_lim0.y );
		m_limy0_wnd.SetWindowText( tempchar );
	}
	if( m_lim1.y != s_lim1.y ){
		sprintf_s( tempchar, 256, "%f", m_lim1.y );
		m_limy1_wnd.SetWindowText( tempchar );
	}
	if( m_lim0.z != s_lim0.z ){
		sprintf_s( tempchar, 256, "%f", m_lim0.z );
		m_limz0_wnd.SetWindowText( tempchar );
	}
	if( m_lim1.z != s_lim1.z ){
		sprintf_s( tempchar, 256, "%f", m_lim1.z );
		m_limz1_wnd.SetWindowText( tempchar );
	}


	if( m_mv.x != s_mv.x ){
		sprintf_s( tempchar, 256, "%f", m_mv.x );
		m_mvx_wnd.SetWindowText( tempchar );
	}
	if( m_mv.y != s_mv.y ){
		sprintf_s( tempchar, 256, "%f", m_mv.y );
		m_mvy_wnd.SetWindowText( tempchar );
	}
	if( m_mv.z != s_mv.z ){
		sprintf_s( tempchar, 256, "%f", m_mv.z );
		m_mvz_wnd.SetWindowText( tempchar );
	}

	if( m_sc.x != s_sc.x ){
		sprintf_s( tempchar, 256, "%f", m_sc.x );
		m_scx_wnd.SetWindowText( tempchar );
	}
	if( m_sc.y != s_sc.y ){
		sprintf_s( tempchar, 256, "%f", m_sc.y );
		m_scy_wnd.SetWindowText( tempchar );
	}
	if( m_sc.z != s_sc.z ){
		sprintf_s( tempchar, 256, "%f", m_sc.z );
		m_scz_wnd.SetWindowText( tempchar );
	}
	if( m_nameptr ){	
		sprintf_s( tempchar, 256, "%s", m_nameptr );
		m_name_wnd.SetWindowText( tempchar );
	}else{
		strcpy_s( tempchar, 256, "有効なボーンが選択されていません。" );
		m_name_wnd.SetWindowText( tempchar );
	}

	if( m_ignorelim != s_ignorelim ){
		if( m_ignorelim == 1 ){
			m_dlg_wnd.CheckDlgButton( IDC_IGNLIM, BST_CHECKED );
		}else if( m_ignorelim == 0 ){
			m_dlg_wnd.CheckDlgButton( IDC_IGNLIM, BST_UNCHECKED );
		}else{
			_ASSERT( 0 );
		}
	}

	if( m_zakind == ZA_1 ){
		strcpy_s( tempchar, 256, "回転の座標系は「座１」です（可変）" );
	}else if( m_zakind == ZA_2 ){
		strcpy_s( tempchar, 256, "回転の座標系は「座２」です（可変）" );
	}else if( m_zakind == ZA_3 ){
		strcpy_s( tempchar, 256, "回転の座標系は「座３」です（可変）" );
	}else if( m_zakind == ZA_4 ){
		strcpy_s( tempchar, 256, "回転の座標系は「座４」です（可変）" );
	}else{
		strcpy_s( tempchar, 256, "回転の座標系は「座１」です（可変）" );
		m_zakind = ZA_1;
		_ASSERT( 0 );
	}
	m_za_wnd.SetWindowText( tempchar );


	s_rot = m_rot;
	s_lim0 = m_lim0;
	s_lim1 = m_lim1;
	s_mv = m_mv;
	s_sc = m_sc;
	s_ignorelim = m_ignorelim;

	return 0;
}

int CBonePropDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;
	ShowWindow( m_cmdshow );

	if( cmdshow != SW_HIDE ){
		ParamsToDlg();
	}

	return 0;
}

LRESULT CBonePropDlg::OnApplyRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_papp ){
		return 0;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		return 0;
	}
	if( !g_motdlg ){
		return 0;
	}
	if( m_serialno <= 0 ){
		return 0;
	}
///////////////////////

	int ret;
	float tmpd;

	ret = GetFloat( m_rotx_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "X軸の回転角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_rot.x = tmpd;

	ret = GetFloat( m_roty_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "Y軸の回転角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_rot.y = tmpd;

	ret = GetFloat( m_rotz_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "Z軸の回転角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_rot.z = tmpd;


	ret = m_papp->CopyUndoBuffer();
	_ASSERT( !ret );


	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_serialno );
	_ASSERT( selem );

	CQuaternion axisQ;
	CQuaternion invaxisQ;
	ret = selem->GetBoneAxisQ( &axisQ );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplyRot : se GetBoneAxisQ error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = axisQ.inv( &invaxisQ );
	_ASSERT( !ret );


	CQuaternion q, qx, qy, qz;
	D3DXVECTOR3 axisX( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 axisY( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 axisZ( 0.0f, 0.0f, 1.0f );
	qx.SetAxisAndRot( axisX, m_rot.x * (float)DEG2PAI );
	qy.SetAxisAndRot( axisY, m_rot.y * (float)DEG2PAI );
	qz.SetAxisAndRot( axisZ, m_rot.z * (float)DEG2PAI );

	q = axisQ * qy * qx * qz * invaxisQ;

	CMotionPoint2 curmp;
	int hasmpflag = 0;
	ret = m_papp->m_mhandler->GetCurrentMotionPoint( m_papp->m_shandler, m_serialno, &curmp, &hasmpflag );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplyRot : mh GetCurrentMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	curmp.m_q = q;
	curmp.m_eul = m_rot;

	ret = g_motdlg->SetCurrentMotionPoint( m_serialno, &curmp, hasmpflag );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplyRot : SetCurrentMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
LRESULT CBonePropDlg::OnApplyMv(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_papp ){
		return 0;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		return 0;
	}
	if( !g_motdlg ){
		return 0;
	}
	if( m_serialno <= 0 ){
		return 0;
	}
///////////////////////

	int ret;
	float tmpfloat;
	
	ret = GetFloat( m_mvx_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "X軸の移動量が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_mv.x = tmpfloat;

	ret = GetFloat( m_mvy_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "Y軸の移動量が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_mv.y = tmpfloat;

	ret = GetFloat( m_mvz_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "Z軸の移動量が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_mv.z = tmpfloat;


	CMotionPoint2 curmp;
	int hasmpflag = 0;
	ret = m_papp->m_mhandler->GetCurrentMotionPoint( m_papp->m_shandler, m_serialno, &curmp, &hasmpflag );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplyMv : mh GetCurrentMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_papp->CopyUndoBuffer();
	_ASSERT( !ret );



	curmp.m_mvx = m_mv.x;
	curmp.m_mvy = m_mv.y;
	curmp.m_mvz = m_mv.z;


	ret = g_motdlg->SetCurrentMotionPoint( m_serialno, &curmp, hasmpflag );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplyMv : SetCurrentMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CBonePropDlg::OnApplySc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_papp ){
		return 0;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		return 0;
	}
	if( !g_motdlg ){
		return 0;
	}
	if( m_serialno <= 0 ){
		return 0;
	}
///////////////////////

	int ret;
	float tmpfloat;
	
	ret = GetFloat( m_scx_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "X軸の拡大量が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_sc.x = tmpfloat;

	ret = GetFloat( m_scy_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "Y軸の拡大量が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_sc.y = tmpfloat;

	ret = GetFloat( m_scz_wnd, &tmpfloat );
	if( ret ){
		::MessageBox( m_hWnd, "Z軸の拡大量が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_sc.z = tmpfloat;


	CMotionPoint2 curmp;
	int hasmpflag = 0;
	ret = m_papp->m_mhandler->GetCurrentMotionPoint( m_papp->m_shandler, m_serialno, &curmp, &hasmpflag );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplySc : mh GetCurrentMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_papp->CopyUndoBuffer();
	_ASSERT( !ret );



	curmp.m_scalex = m_sc.x;
	curmp.m_scaley = m_sc.y;
	curmp.m_scalez = m_sc.z;


	ret = g_motdlg->SetCurrentMotionPoint( m_serialno, &curmp, hasmpflag );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplySc : SetCurrentMotionPoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

LRESULT CBonePropDlg::OnApplyLim(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_papp ){
		return 0;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		return 0;
	}
	if( !g_motdlg ){
		return 0;
	}
	if( m_serialno <= 0 ){
		return 0;
	}
///////////////////////

	int ret;

	double tmpd;

	ret = GetDouble( m_limx0_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "X軸の制限角度の開始角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_lim0.x = tmpd;

	ret = GetDouble( m_limx1_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "X軸の制限角度の終了角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_lim1.x = tmpd;

	if( m_lim0.x > m_lim1.x ){
		::MessageBox( m_hWnd, "X軸の制限角度が不正です。\n開始角度が終了角度より大きいです。", "入力エラー", MB_OK );
		return 0;
	}
////////
	ret = GetDouble( m_limy0_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "Y軸の制限角度の開始角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_lim0.y = tmpd;

	ret = GetDouble( m_limy1_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "Y軸の制限角度の終了角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_lim1.y = tmpd;

	if( m_lim0.y > m_lim1.y ){
		::MessageBox( m_hWnd, "Y軸の制限角度が不正です。\n開始角度が終了角度より大きいです。", "入力エラー", MB_OK );
		return 0;
	}
/////////
	ret = GetDouble( m_limz0_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "Z軸の制限角度の開始角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_lim0.z = tmpd;

	ret = GetDouble( m_limz1_wnd, &tmpd );
	//if( ret || (tmpd > 180.0) || (tmpd < -180.0) ){
	if( ret ){
		::MessageBox( m_hWnd, "Z軸の制限角度の終了角度が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_lim1.z = tmpd;

	if( m_lim0.z > m_lim1.z ){
		::MessageBox( m_hWnd, "Z軸の制限角度が不正です。\n開始角度が終了角度より大きいです。", "入力エラー", MB_OK );
		return 0;
	}
///////////
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_IGNLIM );
	if( checkflag == BST_CHECKED ){
		m_ignorelim = 1;
	}else if( checkflag == BST_UNCHECKED ){
		m_ignorelim = 0;
	}


	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_serialno );
	_ASSERT( selem );

	ret = selem->SetBoneLim01( &m_lim0, &m_lim1 );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplyLim : se SetBoneLim01 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = selem->SetIgnoreLim( m_ignorelim );
	if( ret ){
		DbgOut( "bonepropdlg : OnApplyLim : se SetIgnoreLim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


int CBonePropDlg::UpdateParams( int srcseri, int srczakind )
{
	if( !m_papp ){
		return 0;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		return 0;
	}
	if( !g_motdlg ){
		return 0;
	}
////////////////
	int ret;

	m_zakind = srczakind;

	if( srcseri <= 0 ){
		InitParams();

	}else{
		CTreeElem2* telem;
		telem = (*m_papp->m_thandler)( srcseri );

		CShdElem* selem;
		selem = (*m_papp->m_shandler)( srcseri );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			m_serialno = srcseri;
			m_nameptr = telem->name;		

			CMotionPoint2 curmp;
			int hasmpflag = 0;
			ret = m_papp->m_mhandler->GetCurrentMotionPoint( m_papp->m_shandler, srcseri, &curmp, &hasmpflag );
			if( ret ){
				DbgOut( "bonepropdlg : UpdateParams : mh GetCurrentMotionPoint error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			m_rot = curmp.m_eul;

			m_mv.x = curmp.m_mvx;
			m_mv.y = curmp.m_mvy;
			m_mv.z = curmp.m_mvz;

			m_sc.x = curmp.m_scalex;
			m_sc.y = curmp.m_scaley;
			m_sc.z = curmp.m_scalez;

			ret = selem->GetBoneLim01( &m_lim0, &m_lim1 );
			if( ret ){
				DbgOut( "bonepropdlg : UpdateParams : se GetBoneLim01 error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = selem->GetIgnoreLim( &m_ignorelim );
			if( ret ){
				DbgOut( "bonepropdlg : UpdateParams : se GetIgnoreLim error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

		}else{
			InitParams();
		}
	}


	if( m_cmdshow != SW_HIDE ){
		ParamsToDlg();
	}


	return 0;
}

int CBonePropDlg::GetLimitedQ( int boneno, D3DXVECTOR3 starteul, D3DXVECTOR3 endeul, CQuaternion* aftq, D3DXVECTOR3* afteul )
{
	if( !m_papp ){
		_ASSERT( 0 );
		return 1;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		_ASSERT( 0 );
		return 1;
	}
/////////////////
	int ret;
	CShdElem* selem;
	DVEC3 lim0, lim1;

	selem = (*m_papp->m_shandler)( boneno );
	_ASSERT( selem );



	int ignorelim;
	ret = selem->GetIgnoreLim( &ignorelim );
	if( ret ){
		DbgOut( "bonepropdlg : GetLimitedQ : se GetIgnoreLim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = selem->GetBoneLim01( &lim0, &lim1 );
	if( ret ){
		DbgOut( "bonepropdlg : GetLimitedQ : se GetBoneLim01 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 limeul;
	if( ignorelim == 0 ){
		ret = GetLimitedEul( starteul.x, endeul.x, lim0.x, lim1.x, &limeul.x );
		if( ret ){
			DbgOut( "bonepropdlg : GetLimitedQ : GetLimitedEul error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = GetLimitedEul( starteul.y, endeul.y, lim0.y, lim1.y, &limeul.y );
		if( ret ){
			DbgOut( "bonepropdlg : GetLimitedQ : GetLimitedEul error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = GetLimitedEul( starteul.z, endeul.z, lim0.z, lim1.z, &limeul.z );
		if( ret ){
			DbgOut( "bonepropdlg : GetLimitedQ : GetLimitedEul error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		limeul = endeul;
	}

	CQuaternion axisQ, invaxisQ;
	ret = selem->GetBoneAxisQ( &axisQ );
	if( ret ){
		DbgOut( "bonepropdlg : GetLimitedQ : se GetBoneAxisQ error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = axisQ.inv( &invaxisQ );
	_ASSERT( !ret );

	CQuaternion qx, qy, qz;
	D3DXVECTOR3 axisX( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 axisY( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 axisZ( 0.0f, 0.0f, 1.0f );
	qx.SetAxisAndRot( axisX, limeul.x * (float)DEG2PAI );
	qy.SetAxisAndRot( axisY, limeul.y * (float)DEG2PAI );
	qz.SetAxisAndRot( axisZ, limeul.z * (float)DEG2PAI );

	*aftq = axisQ * qy * qx * qz * invaxisQ;
	*afteul = limeul;

	return 0;
}

int CBonePropDlg::GetLimitedQ( int boneno, CQuaternion* startq, D3DXVECTOR3* befeul, CQuaternion* endq, CQuaternion* dstq, 
	int* startout, D3DXVECTOR3* dsteul, int notmvflag, int initaxisq )
{
	if( !m_papp ){
		_ASSERT( 0 );
		return 1;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		_ASSERT( 0 );
		return 1;
	}
////////////

	int ret;
	CShdElem* selem;
	DVEC3 lim0, lim1;

	selem = (*m_papp->m_shandler)( boneno );
	_ASSERT( selem );



	int ignorelim;
	ret = selem->GetIgnoreLim( &ignorelim );
	if( ret ){
		DbgOut( "bonepropdlg : GetLimitedQ : se GetIgnoreLim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = selem->GetBoneLim01( &lim0, &lim1 );
	if( ret ){
		DbgOut( "bonepropdlg : GetLimitedQ : se GetBoneLim01 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	D3DXVECTOR3 starteul, endeul;
	ret = qToEuler( selem, startq, &starteul );
	if( ret ){
		DbgOut( "bonepropdlg : GetLimitedQ : qToEuler error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = modifyEuler( &starteul, befeul );
	_ASSERT( !ret );


	ret = qToEuler( selem, endq, &endeul );
	if( ret ){
		DbgOut( "bonepropdlg : GetLimitedQ : qToEuler error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = modifyEuler( &endeul, befeul );
	_ASSERT( !ret );


	if( ignorelim != 0 ){
		*dstq = *endq;
		*dsteul = endeul;
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	if( (starteul.x >= (float)lim0.x) && (starteul.x <= (float)lim1.x) 
		&& (starteul.y >= (float)lim0.y) && (starteul.y <= (float)lim1.y) 
		&& (starteul.z >= (float)lim0.z) && (starteul.z <= (float)lim1.z) ){

		*startout = 0;
	}else{
		*startout = 1;
	}


	if( (endeul.x >= (float)lim0.x) && (endeul.x <= (float)lim1.x) 
		&& (endeul.y >= (float)lim0.y) && (endeul.y <= (float)lim1.y) 
		&& (endeul.z >= (float)lim0.z) && (endeul.z <= (float)lim1.z) ){

		*dstq = *endq;
		*dsteul = endeul;

	}else{

		if( notmvflag == 0 ){

			D3DXVECTOR3 limeul;
			ret = GetLimitedEul( starteul.x, endeul.x, lim0.x, lim1.x, &limeul.x );
			if( ret ){
				DbgOut( "bonepropdlg : GetLimitedQ : GetLimitedEul error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			ret = GetLimitedEul( starteul.y, endeul.y, lim0.y, lim1.y, &limeul.y );
			if( ret ){
				DbgOut( "bonepropdlg : GetLimitedQ : GetLimitedEul error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			ret = GetLimitedEul( starteul.z, endeul.z, lim0.z, lim1.z, &limeul.z );
			if( ret ){
				DbgOut( "bonepropdlg : GetLimitedQ : GetLimitedEul error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			//ret = modifyEuler( &limeul, befeul );
			//_ASSERT( !ret );
			//*dsteul = limeul;

			CQuaternion axisQ, invaxisQ;
			if( initaxisq == 0 ){
				ret = selem->GetBoneAxisQ( &axisQ );
				if( ret ){
					DbgOut( "bonepropdlg : GetLimitedQ : se GetBoneAxisQ error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				ret = axisQ.inv( &invaxisQ );
				_ASSERT( !ret );
			}else{
				axisQ.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
				invaxisQ.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
			}


			CQuaternion qx, qy, qz;
			D3DXVECTOR3 axisX( 1.0f, 0.0f, 0.0f );
			D3DXVECTOR3 axisY( 0.0f, 1.0f, 0.0f );
			D3DXVECTOR3 axisZ( 0.0f, 0.0f, 1.0f );
			qx.SetAxisAndRot( axisX, limeul.x * (float)DEG2PAI );
			qy.SetAxisAndRot( axisY, limeul.y * (float)DEG2PAI );
			qz.SetAxisAndRot( axisZ, limeul.z * (float)DEG2PAI );

			*dstq = axisQ * qy * qx * qz * invaxisQ;
			*dsteul = limeul;

		}else{
			*dstq = *startq;
			*dsteul = starteul;
		}
	}

	return 0;
}

int CBonePropDlg::GetLimitedEul( float starteul, float endeul, double lim0, double lim1, float* limeul )
{
	if( (starteul < (float)lim0) || (starteul > (float)lim1) ){
	
		double cmpeul;
		cmpeul = starteul;

		double abs0, abs1;
		abs0 = abs( lim0 - cmpeul );
		abs1 = abs( lim1 - cmpeul );

		if( abs0 <= abs1 ){
			*limeul = (float)lim0 + 0.1f;
		}else{
			*limeul = (float)lim1 - 0.1f;
		}

	}else if( (endeul >= (float)lim0) && (endeul <= (float)lim1) ){
		*limeul = endeul;
	}else{
		double cmpeul;
		//cmpeul = ( starteul + endeul ) * 0.5;
		cmpeul = starteul;

		double abs0, abs1;
		abs0 = abs( lim0 - cmpeul );
		abs1 = abs( lim1 - cmpeul );

		if( abs0 <= abs1 ){
			*limeul = (float)lim0 + 0.1f;
		}else{
			*limeul = (float)lim1 - 0.1f;
		}
	}

	return 0;
}

LRESULT CBonePropDlg::OnSetSymm(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_papp ){
		return 0;
	}
	if( !m_papp->m_shandler || !m_papp->m_thandler || !m_papp->m_mhandler ){
		return 0;
	}

	int ret;
	int seri;
	for( seri = 0; seri < m_papp->m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_papp->m_shandler)( seri );
		_ASSERT( selem );
		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			CTreeElem2* telem;
			telem = (*m_papp->m_thandler)( seri );
			_ASSERT( telem );
			char* xpptr;
			xpptr = strstr( telem->name, "_X+" );
			if( xpptr ){

				int symmno = 0;
				int symmtype = SYMMTYPE_NONE;
				ret = SOGetSymmNo( m_papp->m_thandler, m_papp->m_shandler, seri, &symmno, &symmtype );//SymmOpe.cpp
				if( ret ){
					DbgOut( "BonePropDlg : OnSetSymm : GetSymmNo error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				if( symmno <= 0 ){
					ret = SOGetSymmNo2( m_papp->m_thandler, m_papp->m_shandler, seri, &symmno, &symmtype );//SymmOpe.cpp
					if( ret ){
						DbgOut( "BonePropDlg : OnSetSymm : GetSymmNo2 error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}
				if( symmno > 0 ){
					DVEC3 lim0, lim1;
					ret = selem->GetBoneLim01( &lim0, &lim1 );
					if( ret ){
						DbgOut( "BonePropDlg : OnSetSymm : se GetBoneLim01 error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					
					lim0.y *= -1.0f;
					lim0.z *= -1.0f;
					lim1.y *= -1.0f;
					lim1.z *= -1.0f;

					DVEC3 setlim0, setlim1;
					if( lim0.x <= lim1.x ){
						setlim0.x = lim0.x;
						setlim1.x = lim1.x;
					}else{
						setlim0.x = lim1.x;
						setlim1.x = lim0.x;
					}
					if( lim0.y <= lim1.y ){
						setlim0.y = lim0.y;
						setlim1.y = lim1.y;
					}else{
						setlim0.y = lim1.y;
						setlim1.y = lim0.y;
					}
					if( lim0.z <= lim1.z ){
						setlim0.z = lim0.z;
						setlim1.z = lim1.z;
					}else{
						setlim0.z = lim1.z;
						setlim1.z = lim0.z;
					}

					CShdElem* symmelem;
					symmelem = (*m_papp->m_shandler)( symmno );
					_ASSERT( symmelem );
					ret = symmelem->SetBoneLim01( &setlim0, &setlim1 );
					if( ret ){
						DbgOut( "BonePropDlg : OnSetSymm : se SetBoneLim01 error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					if( symmno == m_serialno ){
						m_lim0.x = setlim0.x;
						m_lim0.y = setlim0.y;
						m_lim0.z = setlim0.z;

						m_lim1.x = setlim1.x;
						m_lim1.y = setlim1.y;
						m_lim1.z = setlim1.z;

						ParamsToDlg();
					}
				}
			}
		}
	}

	return 0;
}
