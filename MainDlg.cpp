#include "stdafx.h"


#include "Rps2Qua.h"

#include "MainDlg.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <easy3d.h>

#include "GetDlgParams.h"

#include "LDSigDlg.h"
#include "LDRpsDlg.h"
#include "FilterDlg.h"
#include "MabikiDlg.h"
#include "InfoDlg.h"

#include "rpsfile.h"
#include "txtsetting.h"

#include <crtdbg.h>
#include "SaveQua.h"

///////////////////

extern int g_renderflag;

/////////////////////////////////////////////////////////////////////////////
// CMainDlg

CMainDlg::CMainDlg( CRps2QuaModule* papp )
{
	m_papp = papp;
	m_scid = -1;
	m_hsid = -1;
	m_motid = -1;
	m_maxframe = 0;
	m_retfps = 0.0;
	m_lid = -1;

	int spno;
	for( spno = 0; spno < CAMSP_MAX; spno++ ){
		m_camsp[spno].id = -1;
		m_camsp[spno].width = 1;
		m_camsp[spno].height = 1;
		ZeroMemory( &(m_camsp[spno].tra), sizeof( D3DXVECTOR3 ) );
	}

	m_cammode = CAMM_NONE;
	m_lpos.x = 0;
	m_lpos.y = 0;

	m_curcamera.looktype = CAML_NORMAL;
	m_curcamera.pos = D3DXVECTOR3( 0.0f, EYEYINITVAL, -CAMDISTINITVAL );
	m_curcamera.target = D3DXVECTOR3( 0.0f, EYEYINITVAL, 0.0f );
	m_curcamera.up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_curcamera.dist = CAMDISTINITVAL;
	m_curcamera.hsid = -1;
	m_curcamera.boneno = -1;
	m_curcamera.nearz = 100.0f;
	m_curcamera.farz = 40000.0f;
	m_curcamera.fov = 45.0f;
	m_curcamera.ortho = 0;
	m_curcamera.orthosize = 3000.0f;

	m_rpsfile = 0;
	m_filterdlg = 0;
	m_mabikidlg = 0;
	m_txtsetting = 0;
}

CMainDlg::~CMainDlg()
{
	if( m_rpsfile ){
		delete m_rpsfile;
		m_rpsfile = 0;
	}

	if( m_txtsetting ){
		delete m_txtsetting;
		m_txtsetting = 0;
	}

	if( m_filterdlg ){
		if( m_filterdlg->IsWindow() ){
			m_filterdlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );
			m_filterdlg->DestroyWindow();
		}
		delete m_filterdlg;
		m_filterdlg = 0;
	}
	if( m_mabikidlg ){
		if( m_mabikidlg->IsWindow() ){
			m_mabikidlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );
			m_mabikidlg->DestroyWindow();
		}
		delete m_mabikidlg;
		m_mabikidlg = 0;
	}
}


LRESULT CMainDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_dlg_wnd = m_hWnd;

	m_3dwnd = GetDlgItem( IDC_3DWND );
	_ASSERT( m_3dwnd );

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

int CMainDlg::OneTimeSceneInit( HINSTANCE hinst, HWND hwnd, char* mediadir )
{
	m_cammode = CAMM_NONE;

	int ret;
	ret = E3DInit( hinst, hwnd, 0, 16, 0, 1, 1, 0, 0, &m_scid );
	if( ret ){
		_ASSERT( 0 );
		return FALSE;
	}

	D3DXVECTOR3 campos = D3DXVECTOR3( 0.0f, 800.0f, -2000.0 );
	ret = E3DSetCameraPos( campos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = E3DSetCameraDeg( 180.0f, 0.0f );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = E3DSetProjection( 1000.0f, 30000.0f, 45.0f );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	
	ret = E3DCreateLight( &m_lid );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	D3DXVECTOR3 ldir( 0.0f, 0.0f, 1.0f );
	E3DCOLOR4UC col= {255, 255, 255, 255 };
	ret = E3DSetDirectionalLight( m_lid, ldir, col );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
//////////
	char spname[MAX_PATH];
	ZeroMemory( spname, sizeof( char ) * MAX_PATH );
	sprintf_s( spname, MAX_PATH, "%scam_kai.png", mediadir );
	ret = E3DCreateSprite( spname, 0, &col, &(m_camsp[CAMSP_KAI].id) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = E3DGetSpriteSize( m_camsp[CAMSP_KAI].id, &(m_camsp[CAMSP_KAI].width), &(m_camsp[CAMSP_KAI].height) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ZeroMemory( spname, sizeof( char ) * MAX_PATH );
	sprintf_s( spname, MAX_PATH, "%scam_i.png", mediadir );
	ret = E3DCreateSprite( spname, 0, &col, &(m_camsp[CAMSP_I].id) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = E3DGetSpriteSize( m_camsp[CAMSP_KAI].id, &(m_camsp[CAMSP_I].width), &(m_camsp[CAMSP_I].height) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ZeroMemory( spname, sizeof( char ) * MAX_PATH );
	sprintf_s( spname, MAX_PATH, "%scam_kaku.png", mediadir );
	ret = E3DCreateSprite( spname, 0, &col, &(m_camsp[CAMSP_KAKU].id) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = E3DGetSpriteSize( m_camsp[CAMSP_KAKU].id, &(m_camsp[CAMSP_KAKU].width), &(m_camsp[CAMSP_KAKU].height) );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	RECT rc3dwnd;
	::GetClientRect( hwnd, &rc3dwnd );
	int spritesize = m_camsp[CAMSP_KAI].width;
	int spacesize = 5;

	m_camsp[CAMSP_KAKU].tra.x = (float)( rc3dwnd.right - rc3dwnd.left - 2 * spacesize - spritesize );
	m_camsp[CAMSP_KAKU].tra.y = (float)spacesize;
	m_camsp[CAMSP_KAKU].tra.z = 0.0f;

	m_camsp[CAMSP_I].tra.x = (float)( m_camsp[CAMSP_KAKU].tra.x - spacesize - spritesize );
	m_camsp[CAMSP_I].tra.y = (float)spacesize;
	m_camsp[CAMSP_I].tra.z = 0.0f;

	m_camsp[CAMSP_KAI].tra.x = (float)( m_camsp[CAMSP_I].tra.x - spacesize - spritesize );
	m_camsp[CAMSP_KAI].tra.y = (float)spacesize;
	m_camsp[CAMSP_KAI].tra.z = 0.0f;

////
	m_filterdlg = new CFilterDlg();
	if( !m_filterdlg ){
		_ASSERT( 0 );
		return 1;
	}
	m_filterdlg->Create( m_hWnd );


	m_mabikidlg = new CMabikiDlg();
	if( !m_mabikidlg ){
		_ASSERT( 0 );
		return 1;
	}
	m_mabikidlg->Create( m_hWnd );

	return 0;
}


int CMainDlg::ParamsToDlg()
{


	return 0;
}



LRESULT CMainDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_papp ){
//		E3DBye();
		m_scid = -1;
		m_hsid = -1;
		m_papp->OnDestroy();
	}

	return 0;
}

LRESULT CMainDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_papp ){
//		E3DBye();
		m_scid = -1;
		m_hsid = -1;
		m_papp->OnDestroy();
	}

	return 0;
}

LRESULT CMainDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return 0;
}

int CMainDlg::Render3D()
{

	int ret;

	if( g_renderflag == 0 ){
		return 0;
	}

	if( (m_scid >= 0) && (m_hsid > 0) ){
		ret = SetCamera();
		_ASSERT( !ret );

		if( m_motid >= 0 ){
			int curframe;
			ret = E3DSetNewPose( m_hsid, &curframe );
			_ASSERT( !ret );
		}


		ret = E3DBeginScene( m_scid, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

			int status;
			ret = E3DChkInView( m_scid, m_hsid, &status );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			ret = E3DRender( m_scid, m_hsid, 0, 1, 0, 0 );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			ret = E3DRender( m_scid, m_hsid, 1, 1, 1, 0 );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			ret = RenderCamSp();
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

//			ret = DrawText();
//			if( ret ){
//				_ASSERT( 0 );
//				return 1;
///			}
			
		ret = E3DEndScene();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		ret = E3DPresent( m_scid );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		ret = E3DPCWait( 30, &m_retfps );
//		ret = E3DPCWait( 28, &m_retfps );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CMainDlg::RenderCamSp()
{
	int ret;

	if( (m_camsp[CAMSP_KAI].id >= 0) || (m_camsp[CAMSP_I].id >= 0) || (m_camsp[CAMSP_KAKU].id >= 0) ){
		ret = E3DBeginSprite();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_camsp[CAMSP_KAI].id >= 0 ){
		ret = E3DRenderSprite( m_camsp[CAMSP_KAI].id, 1.0f, 1.0f, m_camsp[CAMSP_KAI].tra );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}
	if( m_camsp[CAMSP_I].id >= 0 ){
		ret = E3DRenderSprite( m_camsp[CAMSP_I].id, 1.0f, 1.0f, m_camsp[CAMSP_I].tra );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}
	if( m_camsp[CAMSP_KAKU].id >= 0 ){
		ret = E3DRenderSprite( m_camsp[CAMSP_KAKU].id, 1.0f, 1.0f, m_camsp[CAMSP_KAKU].tra );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}

	if( (m_camsp[CAMSP_KAI].id >= 0) || (m_camsp[CAMSP_I].id >= 0) || (m_camsp[CAMSP_KAKU].id >= 0) ){
		ret = E3DEndSprite();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}


LRESULT CMainDlg::OnLoadShape(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	g_renderflag = 0;

	if( m_filterdlg ){
		m_filterdlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );
	}
	if( m_mabikidlg ){
		m_mabikidlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );
	}

	m_cammode = CAMM_NONE;

	if( m_hsid > 0 ){
		E3DDestroyHandlerSet( m_hsid );
		m_hsid = -1;
	}

	m_motid = -1;
	m_maxframe = 0;

	int ret;
	CLDSigDlg dlg;
	ret = (int)dlg.DoModal();
	if( ret == IDOK ){
		if( dlg.name[0] ){
			ret = E3DSigLoad( dlg.name, 0, 1.0f, &m_hsid );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	g_renderflag = 1;

	return 0;
}
LRESULT CMainDlg::OnLoadMotion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	g_renderflag = 0;

	if( m_filterdlg ){
		m_filterdlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );
	}
	if( m_mabikidlg ){
		m_mabikidlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );
	}

	m_cammode = CAMM_NONE;

	if( m_rpsfile ){
		delete m_rpsfile;
		m_rpsfile = 0;
	}
	if( m_txtsetting ){
		delete m_txtsetting;
		m_txtsetting = 0;
	}
	Sleep( 300 );


	int ret;
	if( (m_hsid > 0) && (m_motid >= 0) ){
		IDCHANGE idchg;
		ZeroMemory( &idchg, sizeof( IDCHANGE ) );
		idchg.befid = m_motid;
		ret = E3DDestroyMotion( m_hsid, m_motid, &idchg, 1 );
		if( ret ){
			_ASSERT( 0 );
			m_motid = -1;
			g_renderflag = 1;
			return 1;
		}
		m_motid = -1;
		m_maxframe = 0;
	}

	m_txtsetting = new CTxtSetting();
	if( !m_txtsetting ){
		_ASSERT( 0 );
		return 1;
	}

	CLDRpsDlg dlg( m_hsid, m_txtsetting );
	ret = (int)dlg.DoModal();
	if( ret != IDOK ){
		g_renderflag = 1;
		return 0;
	}

	m_rpsfile = new CRpsFile();
	if( !m_rpsfile ){
		_ASSERT( 0 );
		return 1;
	}

	ret = m_rpsfile->LoadParams( dlg.m_rpsname, dlg.m_sampling, dlg.m_keystep );
	if( ret == 1 ){
		_ASSERT( 0 );
		g_renderflag = 1;
		return 1;
	}else if( ret == 2 ){
		::MessageBox( m_hWnd, "生成できるモーション長は36000フレームまでです。\nサンプリング、キーステップを調整して再試行してください。", "フレーム長エラー", MB_OK );
		_ASSERT( 0 );
		g_renderflag = 1;
		return 0;
	}

	ret = m_txtsetting->LoadParams( dlg.m_txtname, m_hsid );
	if( ret ){
		_ASSERT( 0 );
		g_renderflag = 1;
		return 1;
	}
	
	ret = SetKinectUpVec( m_txtsetting->m_elem, m_hsid );
	if( ret ){
		_ASSERT( 0 );
		g_renderflag = 1;
		return 1;
	}

	ret = m_rpsfile->CalcTraQ( m_hsid, m_txtsetting->m_elem, dlg.m_chksym );
	if( ret ){
		_ASSERT( 0 );
		g_renderflag = 1;
		return 1;
	}


	ret = m_rpsfile->SetMotion( m_txtsetting->m_elem, m_hsid, &m_motid, &m_maxframe, 1 );
	if( ret ){
		_ASSERT( 0 );
		g_renderflag = 1;
		return 1;
	}

	ret = E3DSetMotionKind( m_hsid, m_motid );
	if( ret ){
		_ASSERT( 0 );
		g_renderflag = 1;
		return 1;
	}
	
/***	
	char tmpoutname[MAX_PATH];
	sprintf_s( tmpoutname, "%stmpout.qua", m_papp->m_szMediaDir );
	ret = E3DSaveQuaFile( QUATYPE_NAME, m_hsid, m_motid, tmpoutname );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
***/


	g_renderflag = 1;
/***
	double dot005, dot0001, dot0;
	dot005 = 0.999999619;
	dot0001 = 0.9999999998;
	dot0 = 1.0;
	float deg005, deg0001, deg0;
	deg005 = (float)( acos( dot005 ) * PAI2DEG );
	deg0001 = (float)( acos( dot0001 ) * PAI2DEG );
	deg0 = (float)( acos( dot0 ) * PAI2DEG );

	char strmes[1024];
	sprintf_s( strmes, 1024, "deg005 %f\ndeg0001 %f\ndeg0 %f", deg005, deg0001, deg0 );
	::MessageBox( m_hWnd, strmes, "check acos", MB_OK );
***/
/***	
	if( dot >= 0.999999619f ){
		retrad = 0.0f;
	}else if( dot <= -0.999999619f ){
		retrad = (float)PI;
	}else{
***/

	return 0;
}

LRESULT CMainDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetCapture();


	POINT srcpt;
	GetCursorPos( &srcpt );
	::ScreenToClient( m_3dwnd.m_hWnd, &srcpt );

	m_cammode = CAMM_NONE;
	if( ((float)srcpt.x >= m_camsp[CAMSP_KAKU].tra.x) && ((float)srcpt.x <= (m_camsp[CAMSP_KAKU].tra.x + m_camsp[CAMSP_KAKU].width)) && 
		((float)srcpt.y >= m_camsp[CAMSP_KAKU].tra.y) && ((float)srcpt.y <= m_camsp[CAMSP_KAKU].tra.y + m_camsp[CAMSP_KAKU].height) ){
		m_cammode = CAMM_KAKU;
	}
	if( ((float)srcpt.x >= m_camsp[CAMSP_KAI].tra.x) && ((float)srcpt.x <= (m_camsp[CAMSP_KAI].tra.x + m_camsp[CAMSP_KAI].width)) && 
		((float)srcpt.y >= m_camsp[CAMSP_KAI].tra.y) && ((float)srcpt.y <= m_camsp[CAMSP_KAI].tra.y + m_camsp[CAMSP_KAI].height) ){
		m_cammode = CAMM_KAI;
	}
	if( ((float)srcpt.x >= m_camsp[CAMSP_I].tra.x) && ((float)srcpt.x <= (m_camsp[CAMSP_I].tra.x + m_camsp[CAMSP_I].width)) && 
		((float)srcpt.y >= m_camsp[CAMSP_I].tra.y) && ((float)srcpt.y <= m_camsp[CAMSP_I].tra.y + m_camsp[CAMSP_I].height) ){
		m_cammode = CAMM_I;
	}

	m_lpos = srcpt;

	return 0;
}
LRESULT CMainDlg::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ReleaseCapture();

	m_cammode = CAMM_NONE;

	return 0;
}
LRESULT CMainDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptCursor;
	GetCursorPos( &ptCursor );
	::ScreenToClient( m_3dwnd.m_hWnd, &ptCursor );

	if( m_cammode == CAMM_KAKU ){

		float diffx, diffy;
		RECT clirect;
		float vert;

		diffx = (float)ptCursor.x - (float)m_lpos.x;
		diffy = (float)ptCursor.y - (float)m_lpos.y;
		::GetClientRect( m_3dwnd.m_hWnd, &clirect );
		vert = 2000.0f * (diffx + diffy) / (float)(clirect.bottom - clirect.top);
		//vert = (diffx + diffy) * g_inidata->camdlg_move;
		//OnCameraDist( vert );

		D3DXVECTOR3 diff;
		diff = m_curcamera.pos - m_curcamera.target;
		D3DXVECTOR3 ndiff;
		D3DXVec3Normalize( &ndiff, &diff );

		float tmpdist;
		tmpdist = D3DXVec3Length( &diff ) + vert;
		if( tmpdist > 0.0f ){
			m_curcamera.dist = tmpdist;
			m_curcamera.pos = m_curcamera.target + m_curcamera.dist * ndiff;
		}else{
			m_curcamera.target = m_curcamera.target - 1000.0f * ndiff;
			D3DXVECTOR3 diff2 = m_curcamera.pos - m_curcamera.target;
			m_curcamera.dist = D3DXVec3Length( &diff2 );
			D3DXVECTOR3 ndiff2;
			D3DXVec3Normalize( &ndiff2, &diff2 );
			m_curcamera.pos = m_curcamera.target + m_curcamera.dist * ndiff2;
		}

		m_lpos = ptCursor;

	}else if( m_cammode == CAMM_KAI ){
		//OnCameraRot( ptCursor, m_lpos );
		float diffx;
		float diffy;
		diffx = (float)ptCursor.x - (float)m_lpos.x;
		diffy = (float)ptCursor.y - (float)m_lpos.y;


		RECT clirect;
		float rotxz, roty;
		::GetClientRect( m_3dwnd.m_hWnd, &clirect );
		rotxz = 360.0f * diffx / ((float)(clirect.right - clirect.left) / 2.0f);//半分の幅で一回転
		roty = 180.0f * diffy / ((float)(clirect.bottom - clirect.top) / 2.0f);//半分の高さで１８０度回転

		float degxz, degy;
		CamElem2Deg( m_curcamera, &degxz, &degy );

		degxz -= rotxz;
		degy -= roty;

		degy = max( 90.005f, degy );
		degy = min( 269.995f, degy );

		float radxz, rady;
		radxz = degxz * (float)DEG2PAI;
		rady = degy * (float)DEG2PAI;

		m_curcamera.pos.x = sinf(radxz) * m_curcamera.dist * cosf( rady ) + m_curcamera.target.x;
		m_curcamera.pos.y = m_curcamera.target.y + m_curcamera.dist * sinf( rady );
		m_curcamera.pos.z = -cosf(radxz) * m_curcamera.dist * cosf( rady ) + m_curcamera.target.z;

		D3DXVECTOR3 diff;
		diff = m_curcamera.target - m_curcamera.pos;
		m_curcamera.dist = D3DXVec3Length( &diff );

		m_lpos = ptCursor;

	}else if( m_cammode == CAMM_I ){
		//OnCameraMove( ptCursor, m_lpos );
		D3DXMATRIX matView;
		D3DXMatrixLookAtLH( &matView, &m_curcamera.pos, &m_curcamera.target, &m_curcamera.up );

		float diffx;
		float diffy;
		diffx = (float)ptCursor.x - (float)m_lpos.x;
		diffy = (float)ptCursor.y - (float)m_lpos.y;

		RECT clirect;
		float heri, vert;
		::GetClientRect( m_3dwnd.m_hWnd, &clirect );
		heri = m_curcamera.dist * diffx / (float)(clirect.right - clirect.left);
		vert = m_curcamera.dist * diffy / (float)(clirect.bottom - clirect.top);

		float degxz, degy;
		CamElem2Deg( m_curcamera, &degxz, &degy );

		degy = max( 90.005f, degy );
		degy = min( 269.995f, degy );

	//////
		float radheri;
		radheri = (degxz - 90.0f) * (float)DEG2PAI;
		m_curcamera.target.x -= heri * (float)sin( radheri );
		m_curcamera.target.z += heri * (float)cos( radheri );
	//////
		D3DXMATRIX invview;
		invview = matView;
		invview._41 = 0.0f;
		invview._42 = 0.0f;
		invview._43 = 0.0f;
		D3DXMatrixInverse( &invview, NULL, &invview );

		D3DXVECTOR3 newup;
		D3DXVec3TransformCoord( &newup, &(m_curcamera.up), &invview );

		m_curcamera.target.x += vert * newup.x;
		m_curcamera.target.y += vert * newup.y;
		m_curcamera.target.z += vert * newup.z;

	///////
		float radxz, rady;
		radxz = degxz * (float)DEG2PAI;
		rady = degy * (float)DEG2PAI;

		m_curcamera.pos.x = sinf(radxz) * m_curcamera.dist * cosf( rady ) + m_curcamera.target.x;
		m_curcamera.pos.y = m_curcamera.target.y + m_curcamera.dist * sinf( rady );
		m_curcamera.pos.z = -cosf(radxz) * m_curcamera.dist * cosf( rady ) + m_curcamera.target.z;

		D3DXVECTOR3 diff;
		diff = m_curcamera.target - m_curcamera.pos;
		m_curcamera.dist = D3DXVec3Length( &diff );

		m_lpos = ptCursor;
	}
	
	return 0;
}

int CMainDlg::CamElem2Deg( CAMERAELEM camelem, float* degxz, float* degy )
{
	D3DXVECTOR3 diff = camelem.pos - camelem.target;
	D3DXVECTOR3 diffxz = D3DXVECTOR3( diff.x, 0.0f, diff.z );
	D3DXVec3Normalize( &diff, &diff );
	D3DXVec3Normalize( &diffxz, &diffxz );
	D3DXVECTOR3 basez = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	float dotxz = D3DXVec3Dot( &diffxz, &basez );
	dotxz = min( 1.0f, dotxz );
	dotxz = max( -1.0f, dotxz );
	float radxz = (float)acos( dotxz );
	if( diff.x > 0.0f ){
		radxz = 2.0f * (float)PAI - radxz;
	}else if( diff.x == 0.0f ){
		if( diff.z < 0.0f ){
			radxz = (float)PAI;
		}else{
			radxz = 0.0f;
		}
	}
	*degxz = radxz * (float)PAI2DEG;

	float doty = diff.y;
	doty = min( 1.0f, doty );
	doty = max( -1.0f, doty );
	float rady = (float)acos( doty );
	
	float tmpdegy;
	tmpdegy = rady * (float)PAI2DEG + 90.0f;
	*degy = tmpdegy;


	return 0;
}


int CMainDlg::SetCamera()
{
	int ret;
	ret = E3DSetCameraPos( m_curcamera.pos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = E3DSetCameraTarget( m_curcamera.target, m_curcamera.up );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

//	ret = E3DSetProjection( 1000.0f, 30000.0f, 45.0f );
//	if( ret ){
//		_ASSERT( 0 );
//		return 1;
//	}

	return 0;
}

int CMainDlg::SetKinectUpVec( TSELEM* tselem, int hsid )
{

	int ret;
	D3DXVECTOR3 xpdir( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 zmdir( 0.0f, 0.0f, -1.0f );
	D3DXVECTOR3 zpdir( 0.0f, 0.0f, 1.0f );

	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_RIGHT_KNEE )->jointno, xpdir );
	_ASSERT( !ret );
	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_RIGHT_FOOT )->jointno, xpdir );
	_ASSERT( !ret );
	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_LEFT_KNEE )->jointno, xpdir );
	_ASSERT( !ret );
	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_LEFT_FOOT )->jointno, xpdir );
	_ASSERT( !ret );


	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_RIGHT_ELBOW )->jointno, zmdir );
	_ASSERT( !ret );
	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_RIGHT_HAND )->jointno, xpdir );
	_ASSERT( !ret );
	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_LEFT_ELBOW )->jointno, zpdir );
	_ASSERT( !ret );
	ret = E3DSetKinectUpVec( hsid, (tselem + SKEL_LEFT_HAND )->jointno, xpdir );
	_ASSERT( !ret );

	return 0;
}

LRESULT CMainDlg::OnAVG(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_rpsfile || !m_filterdlg || !m_mabikidlg ){
		::MessageBoxA( m_hWnd, "sigとモーションを読み込んでから実行してください。", "警告", MB_OK );
		return 0;
	}

	m_mabikidlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );

	m_filterdlg->ShowDlg( SW_SHOW, m_rpsfile, m_hsid, m_motid, m_maxframe, m_txtsetting );

	return 0;
}

LRESULT CMainDlg::OnMabiki(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_rpsfile || !m_filterdlg || !m_mabikidlg ){
		::MessageBoxA( m_hWnd, "sigとモーションを読み込んでから実行してください。", "警告", MB_OK );
		return 0;
	}

	m_filterdlg->ShowDlg( SW_HIDE, 0, 0, 0, 0, 0 );

	m_mabikidlg->ShowDlg( SW_SHOW, m_rpsfile, m_hsid, &m_motid, m_maxframe, m_txtsetting );

	return 0;
}

/***
LRESULT CMainDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD menuid;
	menuid = LOWORD( wParam );

	if( menuid == IDM_SAVEQUA ){
		::MessageBox( NULL, "onsavequa", "check", MB_OK );
	}

	return 0;
}
***/


int CMainDlg::SaveQua()
{
#ifdef ENABLEQUA
	if( m_hsid <= 0 ){
		return 0;
	}
	if( (m_motid < 0) || (m_txtsetting == 0) || (m_rpsfile == 0) ){
		return 0;
	}

	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.qua" );

	OPENFILENAME ofn;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Motion File (*.qua)\0*.qua\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="qua";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return S_OK;
	}

	int ret;
	ret = E3DSaveQuaFile( QUATYPE_NAME, m_hsid, m_motid, buf );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	return 0;
#else
	return 0;
#endif
}

LRESULT CMainDlg::OnInfo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CInfoDlg dlg;
	dlg.DoModal();

	return 0;
}

LRESULT CMainDlg::OnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ret = SaveQua();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


