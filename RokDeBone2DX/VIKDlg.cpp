// VIKDlg.cpp : CVIKDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "VIKDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "MotParamDlg.h"
#define DBGH
#include <dbg.h>

#include "viewer.h"
#include "MotDlg.h"
#include "GetDlgParams.h"

///// extern

extern CMotDlg* g_motdlg;
extern int g_endapp;


/////////////////////////////////////////////////////////////////////////////
// CVIKDlg

#define VIFbxTimeR 2000

CVIKDlg::CVIKDlg( CMotParamDlg* motparamdlg )
{
	m_d3dapp = 0;
	m_motparamdlg = motparamdlg;

	text_selname_wnd = 0;
	text_curx_wnd = 0;
	text_cury_wnd = 0;
	text_curz_wnd = 0;
	text_dist_wnd = 0;
	edit_div_wnd = 0;
	edit_tx_wnd = 0;
	edit_ty_wnd = 0;
	edit_tz_wnd = 0;
	start_ik_wnd = 0;
	start_shiftik_wnd = 0;
	shifttree_wnd = 0;
	stop_ik_wnd = 0;


	m_selname[0] = 0;
	m_curpos.x = 0.0f;
	m_curpos.y = 0.0f;
	m_curpos.z = 0.0f;
	m_tpos.x = 0.0f;
	m_tpos.y = 0.0f;
	m_tpos.z = 0.0f;
	m_ikdiv = 3;
	m_cmdshow = SW_HIDE;

	m_underik = 0;

	m_timerid = 0;
	m_selectedno = 0;
	m_dist = 0.0f;

	m_frameno = 0;

	m_timerworking = 0;
}

CVIKDlg::~CVIKDlg()
{
	/***
	int ret;

	if( m_timerid != 0 ){

		CallOnStopIK();

		ret = KillTimer( m_timerid );
		_ASSERT( ret );
		m_timerid = 0;
	}
	***/
}

int CVIKDlg::DestroyTimer()
{
	int ret;

	if( m_timerid != 0 ){

		CallOnStopIK();
		
		if( ::IsWindow( m_hWnd ) ){
			ret = KillTimer( m_timerid );
			_ASSERT( ret );
		}
		m_timerid = 0;

	}

	m_timerworking = 0;

	return 0;
}


LRESULT CVIKDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_timerid == 0 ){
		m_timerid = (int)SetTimer( VIFbxTimeR, 16 );
		//DbgOut( "vikdlg :OnInitDialog : m_timerid %d\n", m_timerid );
		_ASSERT( m_timerid == VIFbxTimeR );
	}
	m_timerworking = 1;

	SetWnd();

	GetCurrentValueFromMotParam();


	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

void CVIKDlg::SetWnd()
{
	text_selname_wnd = GetDlgItem( IDC_TEXT_SELNAME );
	text_curx_wnd = GetDlgItem( IDC_TEXT_CURX );
	text_cury_wnd = GetDlgItem( IDC_TEXT_CURY );
	text_curz_wnd = GetDlgItem( IDC_TEXT_CURZ );
	text_dist_wnd = GetDlgItem( IDC_TEXT_DIST );
	edit_div_wnd = GetDlgItem( IDC_EDIT_DIV );
	edit_tx_wnd = GetDlgItem( IDC_EDIT_TX );
	edit_ty_wnd = GetDlgItem( IDC_EDIT_TY );
	edit_tz_wnd = GetDlgItem( IDC_EDIT_TZ );
	start_ik_wnd = GetDlgItem( IDC_START_IK );
	start_shiftik_wnd = GetDlgItem( IDC_START_SHIFTIK );
	shifttree_wnd = GetDlgItem( IDC_TREESHIFT );
	stop_ik_wnd = GetDlgItem( IDC_STOP_IK );
}

int CVIKDlg::ParamsToDlg()
{	
	char tempchar[1024];

	if( m_selectedno > 0 ){
		text_selname_wnd.SetWindowText( m_selname );

		sprintf_s( tempchar, 1024, "X座標： %.5f", m_curpos.x );
		text_curx_wnd.SetWindowText( tempchar );
		sprintf_s( tempchar, 1024, "Y座標： %.5f", m_curpos.y );
		text_cury_wnd.SetWindowText( tempchar );
		sprintf_s( tempchar, 1024, "Z座標： %.5f", m_curpos.z );
		text_curz_wnd.SetWindowText( tempchar );

		sprintf_s( tempchar, 1024, "%d", m_ikdiv );
		edit_div_wnd.SetWindowText( tempchar );

		sprintf_s( tempchar, 1024, "%.5f", m_tpos.x );
		edit_tx_wnd.SetWindowText( tempchar );
		sprintf_s( tempchar, 1024, "%.5f", m_tpos.y );
		edit_ty_wnd.SetWindowText( tempchar );
		sprintf_s( tempchar, 1024, "%.5f", m_tpos.z );
		edit_tz_wnd.SetWindowText( tempchar );

		if( m_underik == 0 ){
			start_ik_wnd.EnableWindow( TRUE );
			start_shiftik_wnd.EnableWindow( TRUE );
			shifttree_wnd.EnableWindow( TRUE );	
			stop_ik_wnd.EnableWindow( FALSE );
		}else{
			start_ik_wnd.EnableWindow( FALSE );
			start_shiftik_wnd.EnableWindow( FALSE );
			shifttree_wnd.EnableWindow( FALSE );	
			stop_ik_wnd.EnableWindow( TRUE );
		}
	}else{
		text_selname_wnd.SetWindowText( "ジョイントが選択されていません" );
		text_curx_wnd.SetWindowText( "X座標： ----" );
		text_cury_wnd.SetWindowText( "Y座標： ----" );
		text_curz_wnd.SetWindowText( "Z座標： ----" );

		sprintf_s( tempchar, 1024, "%d", m_ikdiv );
		edit_div_wnd.SetWindowText( tempchar );

		sprintf_s( tempchar, 1024, "%.5f", m_tpos.x );
		edit_tx_wnd.SetWindowText( tempchar );
		sprintf_s( tempchar, 1024, "%.5f", m_tpos.y );
		edit_ty_wnd.SetWindowText( tempchar );
		sprintf_s( tempchar, 1024, "%.5f", m_tpos.z );
		edit_tz_wnd.SetWindowText( tempchar );

		start_ik_wnd.EnableWindow( FALSE );
		start_shiftik_wnd.EnableWindow( FALSE );
		shifttree_wnd.EnableWindow( FALSE );	
		stop_ik_wnd.EnableWindow( FALSE );
	}

	if( m_underik ){
		sprintf_s( tempchar, 1024, "現在位置との距離:	　%.5f", m_dist );
		text_dist_wnd.SetWindowText( tempchar );
	}else{
		strcpy_s( tempchar, 1024, "現在位置との距離:	　-------" );
		text_dist_wnd.SetWindowText( tempchar );
	}

	return 0;
}



int CVIKDlg::ShowDlg( int cmdshow )
{
	m_cmdshow = cmdshow;

	if( (m_cmdshow != SW_HIDE) && (text_selname_wnd) ){
		//motparamdlgからカレント値取得
		GetCurrentValueFromMotParam();
		ParamsToDlg();

	}

	//if( (m_cmdshow == SW_HIDE) && (text_selname_wnd) ){
	//	CallOnStopIK();
	//}

	ShowWindow( m_cmdshow );

	return 0;
}

LRESULT CVIKDlg::OnStartIK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_d3dapp || (m_d3dapp->m_ExistData == FALSE) )
		return 0;

///
	int ret = 0;

	int tempint;
	ret = GetInt( edit_div_wnd, &tempint );
	if( ret || (tempint <= 0) ){
		MessageBox( "移動の細かさの値が不正です。（１以上の整数）", "入力エラー", MB_OK );
		return 0;
	}
	m_ikdiv = tempint;


	float tempfloat = 0.0f;
	ret = GetFloat( edit_tx_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＸの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.x = tempfloat;

	ret = GetFloat( edit_ty_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＹの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.y = tempfloat;
	
	ret = GetFloat( edit_tz_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＺの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.z = tempfloat;

/////////////
	m_d3dapp->m_vikflag = 1;//!!!!!!!!!!!

	m_d3dapp->CopyUndoBuffer();

	start_ik_wnd.EnableWindow( FALSE );
	start_shiftik_wnd.EnableWindow( FALSE );
	shifttree_wnd.EnableWindow( FALSE );	
	stop_ik_wnd.EnableWindow( TRUE );

	m_underik = 1;
	return 0;
}
LRESULT CVIKDlg::OnStartShiftIK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_d3dapp || (m_d3dapp->m_ExistData == FALSE) )
		return 0;

///
	int ret = 0;

	int tempint;
	ret = GetInt( edit_div_wnd, &tempint );
	if( ret || (tempint <= 0) ){
		MessageBox( "移動の細かさの値が不正です。（１以上の整数）", "入力エラー", MB_OK );
		return 0;
	}
	m_ikdiv = tempint;


	float tempfloat = 0.0f;
	ret = GetFloat( edit_tx_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＸの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.x = tempfloat;

	ret = GetFloat( edit_ty_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＹの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.y = tempfloat;
	
	ret = GetFloat( edit_tz_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＺの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.z = tempfloat;

/////////////
	m_d3dapp->m_vikflag = 1;//!!!!!!!!!!!

	m_d3dapp->CopyUndoBuffer();

	start_ik_wnd.EnableWindow( FALSE );
	start_shiftik_wnd.EnableWindow( FALSE );
	shifttree_wnd.EnableWindow( FALSE );	
	stop_ik_wnd.EnableWindow( TRUE );

	m_underik = 2;
	return 0;
}

LRESULT CVIKDlg::OnTreeShift(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_d3dapp || (m_d3dapp->m_ExistData == FALSE) )
		return 0;

///
	int ret = 0;

	int tempint;
	ret = GetInt( edit_div_wnd, &tempint );
	if( ret || (tempint <= 0) ){
		MessageBox( "移動の細かさの値が不正です。（１以上の整数）", "入力エラー", MB_OK );
		return 0;
	}
	m_ikdiv = tempint;


	float tempfloat = 0.0f;
	ret = GetFloat( edit_tx_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＸの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.x = tempfloat;

	ret = GetFloat( edit_ty_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＹの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.y = tempfloat;
	
	ret = GetFloat( edit_tz_wnd, &tempfloat );
	if( ret ){
		MessageBox( "目標座標のＺの値が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	m_tpos.z = tempfloat;

/////////////
	if( m_selectedno <= 0 )
		return 0;

	m_d3dapp->m_vikflag = 1;//!!!!!!!!!!!

	m_d3dapp->CopyUndoBuffer();

	start_ik_wnd.EnableWindow( FALSE );
	start_shiftik_wnd.EnableWindow( FALSE );
	shifttree_wnd.EnableWindow( FALSE );
	stop_ik_wnd.EnableWindow( TRUE );
	m_underik = 3;

///////////////

	/***
	ret = g_motdlg->ShiftBoneTree( m_selectedno, m_tpos );
	if( ret ){
		DbgOut( "vikdlg : ShiftBoneTree error !!!\n" );
		_ASSERT( 0 );
		return 0;
	}

	//motparamdlgからcurpos取得
	//D3DXMATRIX inimat;
	//D3DXMatrixIdentity( &inimat );
	ret = g_motdlg->GetBonePos( m_selectedno, m_d3dapp->m_matWorld, 1, &m_curpos );
	if( ret ){
		DbgOut( "vikdlg : GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 0;
	}
	D3DXVECTOR3 diff;
	diff = m_tpos - m_curpos;
	float mag;
	mag = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	if( mag > 0.0f )
		m_dist = sqrtf( mag );
	else
		m_dist = 0.0f;

	ParamsToDlg();
	***/

	return 0;
}


int CVIKDlg::CallOnStopIK()
{
	BOOL handled;
	OnStopIK( 0, 0, 0, handled );
	return 0;
}

LRESULT CVIKDlg::OnStopIK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_timerworking != 1 ){
		return 0;
	}

	if( ::IsWindow( start_ik_wnd ) ){
		m_underik = 0;

		if( m_d3dapp ){
			m_d3dapp->m_vikflag = 0;//!!!!!!!!!!!

			start_ik_wnd.EnableWindow( TRUE );
			start_shiftik_wnd.EnableWindow( TRUE );
			shifttree_wnd.EnableWindow( TRUE );	
			stop_ik_wnd.EnableWindow( FALSE );

			g_motdlg->UpdateAllMatrix( 1 );

			ParamsToDlg();
		}

	}


	return 0;
}


LRESULT CVIKDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	int timerid = (int)wParam;
	if( timerid == m_timerid ){
		if( m_underik == 1 ){
			if( m_d3dapp->m_ExistData == FALSE ){
				BOOL handled;
				OnStopIK( 0, 0, 0, handled );
				return 0;
			}

			//targetobjの計算
			D3DXVECTOR3 mvobj;
			//D3DXMATRIX inimat;
			//D3DXMatrixIdentity( &inimat );

			ret = g_motdlg->GetBonePos( m_selectedno, m_d3dapp->m_matWorld, 1, &mvobj );
			if( ret ){
				DbgOut( "vikdlg : ik : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}


			D3DXVECTOR3 targetobj0, targetobj;
			float invdiv;
			invdiv = 1.0f / (float)m_ikdiv;
			targetobj0 = mvobj + invdiv * ( m_tpos - mvobj );

			D3DXMATRIX invwmat;
			D3DXMatrixInverse( &invwmat, NULL, &m_d3dapp->m_matWorld );
			D3DXVec3TransformCoord( &targetobj, &targetobj0, &invwmat );
			
			//motparamdlgのＩＫ呼び出し
			int lastparent = 0;


			if( m_d3dapp->m_tbb_za4 == 0 ){
				D3DXVECTOR3 dummyaxis( 1.0f, 0.0f, 0.0f );
				D3DXVECTOR3 axisdone;
				ret = g_motdlg->MoveBone( m_d3dapp->m_inimat, m_selectedno, targetobj, &lastparent, 0, dummyaxis, &axisdone, -1 );
				if( ret ){
					DbgOut( "vikdlg : motdlg MoveBone error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}


				if( m_selectedno > 0 ){
					g_motdlg->SetParBoneArray( m_selectedno );
					D3DXVECTOR3 axisdone;
					g_motdlg->Rotate2IKTargetReq( m_d3dapp->m_inimat, m_selectedno, axisdone, 0 );
				}
			}else{
				ret = g_motdlg->MoveBoneZa4( m_d3dapp->m_inimat, m_selectedno, targetobj, &lastparent, -1 );
				if( ret ){
					DbgOut( "vikdlg : motdlg MoveBone error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}


				if( m_selectedno > 0 ){
					g_motdlg->SetParBoneArray( m_selectedno );
					D3DXVECTOR3 axisdone;
					g_motdlg->Rotate2IKTargetZa4Req( m_d3dapp->m_inimat, m_selectedno, 0 );
				}
			}

			m_d3dapp->SetBoneArray( lastparent );
			m_d3dapp->FrameMove( lastparent, 0 );

			//m_d3dapp->UpdateVertexBuffer();
				
			if( m_d3dapp->m_bActive && m_d3dapp->m_bReady ){
				m_d3dapp->Render3DEnvironment( 1 );
			}

			//motparamdlgからcurpos取得
			//D3DXMATRIX initmat;
			//D3DXMatrixIdentity( &inimat );
			ret = g_motdlg->GetBonePos( m_selectedno, m_d3dapp->m_matWorld, 1, &m_curpos );
			if( ret ){
				DbgOut( "vikdlg : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}
			D3DXVECTOR3 diff;
			diff = m_tpos - m_curpos;
			float mag;
			mag = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if( mag > 0.0f )
				m_dist = sqrtf( mag );
			else
				m_dist = 0.0f;

			ParamsToDlg();
		}else if( m_underik == 2 ){
			//shift
			if( m_d3dapp->m_ExistData == FALSE ){
				BOOL handled;
				OnStopIK( 0, 0, 0, handled );
				return 0;
			}

			int setno;
			D3DXVECTOR3 newtarget0, newtarget;
				
			ret = g_motdlg->GetBonePos( m_selectedno, m_d3dapp->m_matWorld, 1, &m_curpos );
			if( ret ){
				DbgOut( "vikdlg : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			//newtarget = m_curpos + ( m_tpos - m_curpos ) * 0.3f;
			newtarget0 = m_tpos;

			D3DXMATRIX invwmat;
			D3DXMatrixInverse( &invwmat, NULL, &m_d3dapp->m_matWorld );
			D3DXVec3TransformCoord( &newtarget, &newtarget0, &invwmat );


			ret = g_motdlg->ShiftBone( -1, m_selectedno, newtarget, &setno, m_d3dapp->m_tbb_za4 );
			_ASSERT( !ret );

			if( m_selectedno > 0 ){
				g_motdlg->SetParBoneArray( setno );
				D3DXVECTOR3 axisdone;
				if( m_d3dapp->m_tbb_za4 == 0 ){
					g_motdlg->Rotate2IKTargetReq( m_d3dapp->m_inimat, m_selectedno, axisdone, 0 );
				}else{
					g_motdlg->Rotate2IKTargetZa4Req( m_d3dapp->m_inimat, m_selectedno, 0 );
				}
			}



			m_d3dapp->SetBoneArray( setno );
			m_d3dapp->FrameMove( setno );

			//m_d3dapp->UpdateVertexBuffer();

			if( m_d3dapp->m_bActive && m_d3dapp->m_bReady ){
				m_d3dapp->Render3DEnvironment( 1 );
			}

			//motparamdlgからcurpos取得
			//D3DXMATRIX inimat;
			//D3DXMatrixIdentity( &inimat );
			ret = g_motdlg->GetBonePos( m_selectedno, m_d3dapp->m_matWorld, 1, &m_curpos );
			if( ret ){
				DbgOut( "vikdlg : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}
			D3DXVECTOR3 diff;
			diff = m_tpos - m_curpos;
			float mag;
			mag = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if( mag > 0.0f )
				m_dist = sqrtf( mag );
			else
				m_dist = 0.0f;


			ParamsToDlg();
		}else if( m_underik == 3 ){

			D3DXVECTOR3 newtarget0, newtarget;
			ret = g_motdlg->GetBonePos( m_selectedno, m_d3dapp->m_matWorld, 1, &m_curpos );
			if( ret ){
				DbgOut( "vikdlg : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			newtarget0 = m_curpos + ( m_tpos - m_curpos ) / (float)m_ikdiv;
			D3DXMATRIX invwmat;
			D3DXMatrixInverse( &invwmat, NULL, &m_d3dapp->m_matWorld );
			D3DXVec3TransformCoord( &newtarget, &newtarget0, &invwmat );

			ret = g_motdlg->ShiftBoneTree( m_selectedno, newtarget, m_d3dapp->m_tbb_za4 );
			if( ret ){
				DbgOut( "vikdlg : ShiftBoneTree error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			m_d3dapp->SetBoneArray( 0 );
			m_d3dapp->FrameMove();

			//m_d3dapp->UpdateVertexBuffer();

			if( m_d3dapp->m_bActive && m_d3dapp->m_bReady ){
				m_d3dapp->Render3DEnvironment( 1 );
			}


			//motparamdlgからcurpos取得
			//D3DXMATRIX inimat;
			//D3DXMatrixIdentity( &inimat );
			ret = g_motdlg->GetBonePos( m_selectedno, m_d3dapp->m_matWorld, 1, &m_curpos );
			if( ret ){
				DbgOut( "vikdlg : GetBonePos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}
			D3DXVECTOR3 diff;
			diff = m_tpos - m_curpos;
			float mag;
			mag = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if( mag > 0.0f )
				m_dist = sqrtf( mag );
			else
				m_dist = 0.0f;

			ParamsToDlg();

		}

	}

	return 0;
}

int CVIKDlg::OnUserSelchange( int selno )
{

	GetCurrentValueFromMotParam();
	ParamsToDlg();

	return 0;
}


int CVIKDlg::GetCurrentValueFromMotParam()
{

//motparamdlgからカレント情報を取得	
	int ret;
	char* nameptr;
	ret = m_motparamdlg->GetSelectedJoint( &m_selectedno, &nameptr );
	if( ret ){
		DbgOut( "ivkdlg : OnInitDialog : motparam GetSelectedJoint error !!!\n" );
		_ASSERT( 0 );
		return 0;
	}
	if( m_selectedno > 0 ){
		int leng;
		leng = (int)strlen( nameptr );
		int cpleng;
		cpleng = min( leng, 1023 );
		strncpy_s( m_selname, 1024, nameptr, cpleng );
		m_selname[cpleng] = 0;


		int setflag;
		ret = m_motparamdlg->GetSelectedJointPos( &setflag, &m_curpos );
		if( ret ){
			DbgOut( "ivkdlg : OnInitDialog : motparam GetSelectedJointPos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}
		
	}

	m_frameno = m_motparamdlg->m_current;//!!!!!!

	return 0;
}