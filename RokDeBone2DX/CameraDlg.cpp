// CameraDlg.cpp : CCameraDlg のインプリメンテーション
#include "stdafx.h"
#include "CameraDlg.h"

#include "coef.h"

#include "inidata.h"
extern CIniData* g_inidata;


/////////////////////////////////////////////////////////////////////////////
// CCameraDlg

CCameraDlg::CCameraDlg( HWND srchwnd )
{
	m_apphwnd = srchwnd;
	m_sp_ud_wnd = 0;
	m_sp_rot_wnd = 0;
	m_sp_zoom_wnd = 0;
	m_sp_target_wnd = 0;
	m_sp_rad2_wnd = 0;

	m_degxz = 0.0f;
	m_degy = 0.0f;
	m_eye_y = 0.0f;
	m_camdist = 0.0f;
	m_targetpos.x = 0.0f;
	m_targetpos.y = 0.0f;
	m_targetpos.z = 0.0f;

	int vpno;
	for( vpno = 0; vpno < 4; vpno++ ){
		m_viewparam[vpno].degxz = DEGXZINITVAL;
		m_viewparam[vpno].degy = DEGYINITVAL;
		m_viewparam[vpno].eye_y = EYEYINITVAL;
		m_viewparam[vpno].camdist = CAMDISTINITVAL;

		m_viewparam[vpno].targetpos.x = 0.0f;
		m_viewparam[vpno].targetpos.y = EYEYINITVAL;
		m_viewparam[vpno].targetpos.z = 0.0f;		
	}

	m_mouseope = MOUSEOPE_CAMROT;

}

CCameraDlg::~CCameraDlg()
{
}

LRESULT CCameraDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	InitCommonControls();
	
	HWND temphwnd;

	temphwnd = GetDlgItem( IDC_SP_UD );
	m_sp_ud_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SP_ROT );
	m_sp_rot_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SP_ZOOM );
	m_sp_zoom_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SP_TARGET );
	m_sp_target_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_SP_RAD2 );
	m_sp_rad2_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT_EYEY );
	m_eyey_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT_RAD );
	m_rad_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT_DIST );
	m_dist_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT_TARGET );
	m_target_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT_RAD2 );
	m_rad2_wnd = temphwnd;

	m_camrot_wnd = GetDlgItem( IDC_CAMROT );
	m_cammove_wnd = GetDlgItem( IDC_CAMMOVE );
	m_camzoom_wnd = GetDlgItem( IDC_CAMZOOM );

	CheckCameraButton();

	return 1;  // システムにフォーカスを設定させます
}

int CCameraDlg::SetParams( float degxz, float eyey, float dist, D3DXVECTOR3 targetpos, float degy )
{
	m_degxz = degxz;
	m_degy = degy;
	m_eye_y = eyey;
	m_camdist = dist;
	m_targetpos = targetpos;

	int ret;
	ret = ParamsToDlg();
	_ASSERT( !ret );

	return ret;
}

int CCameraDlg::ParamsToDlg()
{
	char tempchar[1024];

	//m_rad
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%.2f°", m_degxz );
	m_rad_wnd.SetWindowText( tempchar );

	//m_rad2
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%.2f°", m_degy );
	m_rad2_wnd.SetWindowText( tempchar );

	//m_eye_y
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%.2f", m_eye_y );
	m_eyey_wnd.SetWindowText( tempchar );

	//m_camdist
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "%.2f", m_camdist );
	m_dist_wnd.SetWindowText( tempchar );

	//m_targetpos
	ZeroMemory( tempchar, 1024 );
	sprintf_s( tempchar, 1024, "(%.1f, %.1f, %.1f)", m_targetpos.x, m_targetpos.y, m_targetpos.z );
	m_target_wnd.SetWindowText( tempchar );

	return 0;
}

LRESULT CCameraDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_CAMERADLG, 0 );

	return 0;
}

LRESULT CCameraDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//EndDialog(wID);
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_CAMERADLG, 0 );
	return 0;
}
LRESULT CCameraDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	ShowWindow( SW_HIDE );
	::SendMessage( m_apphwnd, WM_USER_ENDDIALOG, (WPARAM)ID_MSG_FROM_CAMERADLG, 0 );
	return 0;
}

/***
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;

	// defaultで、左ボタンがプラスになっているので、反転する。
	int delta = -(pnmud->iDelta);

	//DbgOut( "CMotParamDlg : OnSpin_Sp_rotx : delta %d\n", delta );

	// スピンコントロールのiPosがクランプしないように、初期状態に戻す。
	m_sp_rotx_wnd.SendMessage( UDM_SETPOS, 0, 0 );
***/
LRESULT CCameraDlg::OnDeltaposSp_target(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;

	int delta = pnmud->iDelta;
	int keyflag;
	if( delta > 0 ){
		keyflag = ID_KEY_TARGET_LEFT;
	}else if( delta < 0 ){
		keyflag = ID_KEY_TARGET_RIGHT;
	}else{
		keyflag = 0;
	}
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)keyflag, 0 );	

	return 0;
}

LRESULT CCameraDlg::OnDeltaposSp_rad2(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;

	int delta = pnmud->iDelta;
	int keyflag;
	if( delta > 0 ){
		keyflag = ID_KEY_RAD2_UP;
	}else if( delta < 0 ){
		keyflag = ID_KEY_RAD2_DOWN;
	}else{
		keyflag = 0;
	}
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)keyflag, 0 );	

	return 0;

}

LRESULT CCameraDlg::OnDeltaposSp_rot(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;

	int delta = pnmud->iDelta;
	int keyflag;
	if( delta > 0 ){
		keyflag = ID_KEY_LEFT;
	}else if( delta < 0 ){
		keyflag = ID_KEY_RIGHT;
	}else{
		keyflag = 0;
	}
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)keyflag, 0 );	

	return 0;
}
LRESULT CCameraDlg::OnDeltaposSp_ud(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;

	int delta = pnmud->iDelta;
	int keyflag;
	if( delta > 0 ){
		keyflag = ID_KEY_DOWN;
	}else if( delta < 0 ){
		keyflag = ID_KEY_UP;
	}else{
		keyflag = 0;
	}
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)keyflag, 0 );	
	return 0;
}
LRESULT CCameraDlg::OnDeltaposSp_zoom(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : ｺﾝﾄﾛｰﾙの通知ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。
	NMUPDOWN* pnmud = (NMUPDOWN*)pnmh;

	int delta = pnmud->iDelta;
	int keyflag;
	if( delta > 0 ){
		keyflag = ID_KEY_ZOOMOUT;
	}else if( delta < 0 ){
		keyflag = ID_KEY_ZOOMIN;
	}else{
		keyflag = 0;
	}
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)keyflag, 0 );	

	return 0;
}

LRESULT CCameraDlg::OnTargetReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)ID_KEY_TARGET_RESET, 0 );
	return 0;
}

LRESULT CCameraDlg::OnUdReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)ID_KEY_UD_RESET, 0 );
	return 0;
}

LRESULT CCameraDlg::OnRad2Reset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)ID_KEY_RAD2_RESET, 0 );
	return 0;
}

LRESULT CCameraDlg::OnRotReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)ID_KEY_ROT_RESET, 0 );
	return 0;
}
LRESULT CCameraDlg::OnZoomReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_apphwnd, WM_USER_KEY, (WPARAM)ID_KEY_ZOOM_RESET, 0 );
	return 0;
}

int CCameraDlg::Redraw()
{
	UpdateWindow();
	return 0;
}
LRESULT CCameraDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	::SendMessage( m_apphwnd, WM_USER_MOVE, (WPARAM)ID_MSG_FROM_CAMERADLG, 0 );
	return 0;
}

LRESULT CCameraDlg::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : ﾒｯｾｰｼﾞ ﾊﾝﾄﾞﾗ用のｺｰﾄﾞを追加してください。必要に応じて DefWindowProc を呼び出してください。
	if( m_apphwnd )
		::SendMessage( m_apphwnd, WM_USER_MOVING, (WPARAM)ID_MSG_FROM_CAMERADLG, 0 );

	DefWindowProc();
	return 0;
}

int CCameraDlg::SetViewParam2Current( int vpno )
{
	if( (vpno < 0) || (vpno >= 4) ){
		_ASSERT( 0 );
		return 1;
	}

	m_degxz = m_viewparam[vpno].degxz;
	m_degy = m_viewparam[vpno].degy;
	m_eye_y = m_viewparam[vpno].eye_y;
	m_camdist = m_viewparam[vpno].camdist;
	m_targetpos = m_viewparam[vpno].targetpos;

	return 0;
}
int CCameraDlg::SetSymmXParams()
{
	float newxz;
	newxz = 360.0f - m_degxz;
	if( newxz < 0.0f ){
		newxz += 360.0f;
	}
	m_degxz = newxz;

	m_targetpos.x *= -1.0f;

	return 0;
}


int CCameraDlg::SetCurrent2ViewParam( int vpno )
{
	if( (vpno < 0) || (vpno >= 4) ){
		_ASSERT( 0 );
		return 1;
	}
	m_viewparam[vpno].degxz = m_degxz;
	m_viewparam[vpno].degy = m_degy;
	m_viewparam[vpno].eye_y = m_eye_y;
	m_viewparam[vpno].camdist = m_camdist;
	m_viewparam[vpno].targetpos = m_targetpos;

	g_inidata->viewparam[vpno] = m_viewparam[vpno];//!!!!!!!!!!!!!!!!

	return 0;
}

LRESULT CCameraDlg::OnSaveF5(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetCurrent2ViewParam( 0 );
	
	return 0;
}

LRESULT CCameraDlg::OnSaveF6(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetCurrent2ViewParam( 1 );
	
	return 0;
}

LRESULT CCameraDlg::OnSaveF7(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetCurrent2ViewParam( 2 );
	
	return 0;
}

LRESULT CCameraDlg::OnSaveF8(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetCurrent2ViewParam( 3 );
	
	return 0;
}

int CCameraDlg::CheckCameraButton()
{

	int checkflag[3];
	ZeroMemory( checkflag, sizeof( int ) * 3 );

	switch( m_mouseope ){
	case MOUSEOPE_CAMROT:
		checkflag[0] = 1;
		break;
	case MOUSEOPE_CAMMOVE:
		checkflag[1] = 1;
		break;
	case MOUSEOPE_CAMDIST:
		checkflag[2] = 1;
		break;
	default:
		break;
	}

	SendMessage( m_camrot_wnd, BM_SETSTATE, checkflag[0], 0 );
	SendMessage( m_cammove_wnd, BM_SETSTATE, checkflag[1], 0 );
	SendMessage( m_camzoom_wnd, BM_SETSTATE, checkflag[2], 0 );

	return 0;
}

LRESULT CCameraDlg::OnCamRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_mouseope = MOUSEOPE_CAMROT;
	CheckCameraButton();

	return 0;
}
LRESULT CCameraDlg::OnCamMove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_mouseope = MOUSEOPE_CAMMOVE;
	CheckCameraButton();

	return 0;
}
LRESULT CCameraDlg::OnCamZoom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_mouseope = MOUSEOPE_CAMDIST;
	CheckCameraButton();

	return 0;
}
