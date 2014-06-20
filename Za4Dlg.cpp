// Za4Dlg.cpp : CZa4Dlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "Za4Dlg.h"
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
// CZa4Dlg

CZa4Dlg::CZa4Dlg( CMyD3DApplication* papp )
{
	InitParams();
	m_papp = papp;
	m_cmdshow = SW_HIDE;
	m_createflag = 0;
}

CZa4Dlg::~CZa4Dlg()
{

}

int CZa4Dlg::InitParams()
{
	m_serialno = -1;
	ZeroMemory( &m_rot, sizeof( D3DXVECTOR3 ) );
	m_nameptr = 0;
	m_za4type = ZA_1;
	m_za4rotaxis = ROTAXIS_X;

	return 0;
}

LRESULT CZa4Dlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

//DbgOut( "Za4Dlg : OnInitDialog\r\n" );

	SetWnd();

	if( m_createflag == 0 ){
		
		m_plane_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"青リング平面" );
		m_plane_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"赤リング平面" );
		m_plane_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)"緑リング平面" );

		m_createflag = 1;
	}

	ParamsToDlg();



	return 1;  // システムにフォーカスを設定させます
}

LRESULT CZa4Dlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowDlg( SW_HIDE );

	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	return 0;
}

LRESULT CZa4Dlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowDlg( SW_HIDE );

	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	return 0;
}

void CZa4Dlg::SetWnd()
{
	m_rotx_wnd = GetDlgItem( IDC_EDIT_X );
	m_roty_wnd = GetDlgItem( IDC_EDIT_Y );
	m_rotz_wnd = GetDlgItem( IDC_EDIT_Z );

	m_name_wnd = GetDlgItem( IDC_BONENAME );

	m_za1_wnd = GetDlgItem( IDC_ZA1 );
	m_za2_wnd = GetDlgItem( IDC_ZA2 );
//	m_za3_wnd = GetDlgItem( IDC_ZA3 );

	m_plane_wnd = GetDlgItem( IDC_COMBO_PLANE );

	m_dlg_wnd = m_hWnd;

}

int CZa4Dlg::ParamsToDlg()
{	

	static D3DXVECTOR3 s_rot( 1.2345f, 1.2345f, 1.2345f );
	static int s_za4type = -1;
	static int s_za4rotaxis = -1;

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

	if( m_nameptr && (m_serialno > 0) ){	
		sprintf_s( tempchar, 256, "%s", m_nameptr );
		m_name_wnd.SetWindowText( tempchar );
	}else{
		strcpy_s( tempchar, 256, "有効なボーンが選択されていません。" );
		m_name_wnd.SetWindowText( tempchar );
	}

	if( m_za4rotaxis != s_za4rotaxis ){ 
		m_plane_wnd.SendMessage( CB_SETCURSEL, m_za4rotaxis, 0 );
	}

	if( m_za4type != s_za4type ){
		CheckTypeButton();
	}

	s_rot = m_rot;
	s_za4type = m_za4type;
	s_za4rotaxis = m_za4rotaxis;


	return 0;
}

int CZa4Dlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;
	ShowWindow( m_cmdshow );

	if( cmdshow != SW_HIDE ){
		ParamsToDlg();
	}

	return 0;
}

LRESULT CZa4Dlg::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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


//	ret = m_papp->CopyUndoBuffer();
//	_ASSERT( !ret );


	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_serialno );
	_ASSERT( selem );

	CQuaternion q, qx, qy, qz;
	D3DXVECTOR3 axisX( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 axisY( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 axisZ( 0.0f, 0.0f, 1.0f );
	qx.SetAxisAndRot( axisX, m_rot.x * (float)DEG2PAI );
	qy.SetAxisAndRot( axisY, m_rot.y * (float)DEG2PAI );
	qz.SetAxisAndRot( axisZ, m_rot.z * (float)DEG2PAI );

	//q = axisQ * qy * qx * qz * invaxisQ;
	q = qy * qx * qz;

	ret = selem->SetZa4Q( q );
	if( ret ){
		DbgOut( "Za4Dlg : OnApply : se SetZa4Q error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = selem->SetZa4RotAxis( m_za4rotaxis );
	_ASSERT( !ret );

	ret = selem->SetZa4Type( m_za4type );
	_ASSERT( !ret );

	return 0;
}


int CZa4Dlg::UpdateParams( int srcseri )
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

//	m_zakind = srczakind;

	if( srcseri <= 0 ){
		InitParams();

	}else if( srcseri != m_serialno ){
		CTreeElem2* telem;
		telem = (*m_papp->m_thandler)( srcseri );

		CShdElem* selem;
		selem = (*m_papp->m_shandler)( srcseri );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			m_serialno = srcseri;
			m_nameptr = telem->name;		

			ret = selem->GetZa4Type( &m_za4type );
			_ASSERT( !ret );

			ret = selem->GetZa4RotAxis( &m_za4rotaxis );
			_ASSERT( !ret );

			CQuaternion za4q;
			ret = selem->GetZa4Q( &za4q );
			_ASSERT( !ret );

			D3DXVECTOR3 befeul = m_rot;
			qToEuler( 0, &za4q, &m_rot );//axisq = ini
			modifyEuler( &m_rot, &befeul );
			
		}else{
			InitParams();
		}
	}


	if( m_cmdshow != SW_HIDE ){
		ParamsToDlg();
	}


	return 0;
}

int CZa4Dlg::CheckTypeButton()
{
	switch( m_za4type ){
	case ZA_1:
		SendMessage( m_za1_wnd, BM_SETSTATE, TRUE, 0 );  
		SendMessage( m_za2_wnd, BM_SETSTATE, FALSE, 0 );  
//		SendMessage( m_za3_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case ZA_2:
		SendMessage( m_za1_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_za2_wnd, BM_SETSTATE, TRUE, 0 );  
//		SendMessage( m_za3_wnd, BM_SETSTATE, FALSE, 0 );  
		break;
	case ZA_3:
		SendMessage( m_za1_wnd, BM_SETSTATE, FALSE, 0 );  
		SendMessage( m_za2_wnd, BM_SETSTATE, FALSE, 0 );  
//		SendMessage( m_za3_wnd, BM_SETSTATE, TRUE, 0 );  
		break;
	default:
		SendMessage( m_za1_wnd, BM_SETSTATE, TRUE, 0 );  
		SendMessage( m_za2_wnd, BM_SETSTATE, FALSE, 0 );  
//		SendMessage( m_za3_wnd, BM_SETSTATE, FALSE, 0 );  
		m_za4type = ZA_1;
		break;
	}

	return 0;
}


LRESULT CZa4Dlg::OnZa1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

	m_za4type = ZA_1;

	CheckTypeButton();

	int ret;
	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_serialno );
	_ASSERT( selem );

	ret = selem->SetZa4Type( m_za4type );
	_ASSERT( !ret );

	CQuaternion iniq;
	iniq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
	ret = selem->SetZa4Q( iniq );

	m_rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	return 0;
}
LRESULT CZa4Dlg::OnZa2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

	m_za4type = ZA_2;

	CheckTypeButton();

	int ret;
	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_serialno );
	_ASSERT( selem );

	ret = selem->SetZa4Type( m_za4type );
	_ASSERT( !ret );

	CQuaternion iniq;
	iniq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
	ret = selem->SetZa4Q( iniq );

	m_rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	return 0;
}
/***
LRESULT CZa4Dlg::OnZa3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

	m_za4type = ZA_3;

	CheckTypeButton();

	int ret;
	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_serialno );
	_ASSERT( selem );

	ret = selem->SetZa4Type( m_za4type );
	_ASSERT( !ret );

	CQuaternion iniq;
	iniq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
	ret = selem->SetZa4Q( iniq );

	m_rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	return 0;
}
***/

LRESULT CZa4Dlg::OnPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
//////////////////

	int combono;
	combono = (int)m_plane_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	
	m_za4rotaxis = combono;

	int ret;
	CShdElem* selem;
	selem = (*m_papp->m_shandler)( m_serialno );
	_ASSERT( selem );

	ret = selem->SetZa4RotAxis( m_za4rotaxis );
	_ASSERT( !ret );


	return 0;
}



