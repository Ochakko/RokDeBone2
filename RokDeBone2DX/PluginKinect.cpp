
#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "PluginKinect.h"

#define DBGH
#include <dbg.h>

#include <crtdbg.h>

#include "RDB2Version.h"

#include "txtsetting.h"
#include "rpsfile.h"

#include "viewer.h"
#include <shdhandler.h>
#include <shdelem.h>

#include "ConvSetDlg.h"
#include "FilterType.h"
#include "FilterDlg.h"

#include "MotDlg.h"
#include "MotParamDlg.h"
#include "MultiCapDlg.h"

#include "viewer.h"

#include <BoneProp.h>

#include <mothandler.h>
#include <motionctrl.h>
#include <motioninfo.h>
#include <motionpoint2.h>
#include <quaternion.h>



extern CMotDlg* g_motdlg;


CPluginKinect::CPluginKinect()
{
	ZeroMemory( m_filepath, sizeof( char ) * _MAX_PATH );
	m_pluginversion = 0.0f;
	ZeroMemory( m_filename, sizeof( char ) * _MAX_PATH );
	strcpy_s( m_filename, _MAX_PATH, "KinecoDX.dll" );

	m_validflag = 0;//!!!!!!!!!!!!!!!!!!!!!!
	m_hModule = NULL;

	OpenNIInit = NULL;
	OpenNIClean = NULL;
	OpenNIDrawDepthMap = NULL;
	OpenNIDepthTexture = NULL;
	OpenNIGetSkeltonJointPosition = NULL;
	OpenNIIsTracking = NULL;
	OpenNIGetVersion = NULL;

	m_pdev = NULL;
	m_ptex = NULL;
	m_hwnd = NULL;

	m_texwidth = 256;
	m_texheight = 256;

	m_rendercnt = 0;

	m_ts = 0;
	m_rps = 0;

	m_capmode = 0;
	m_capmotid = 0;
	m_capstartframe = 0;
	m_capendframe = 0;
	m_capframe = 0;
	m_capstep = 20;

	m_timermax = 9 * 30;
	m_curtimer = m_timermax;

	m_dlg = 0;
}
void CPluginKinect::DestroyRps()
{
	if( m_rps ){
		delete m_rps;
		m_rps = 0;
	}

}
void CPluginKinect::DestroyTxtSetting()
{
	if( m_ts ){
		delete m_ts;
		m_ts = 0;
	}
}


CPluginKinect::~CPluginKinect()
{
	UnloadPlugin();
	DestroyRps();
	DestroyTxtSetting();

	if( m_dlg ){
		m_dlg->ShowDlg( SW_HIDE, 0, 0 );
		m_dlg->DestroyWindow();
		delete m_dlg;
		m_dlg = 0;
	}		
}

int CPluginKinect::SetFilePath( char* pluginpath )
{
	int leng;
	leng = (int)strlen( pluginpath );

	if( (leng <= 0) || (leng >= _MAX_PATH) ){
		DbgOut( "PluginKinect : SetFilePath : leng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	strcpy_s( m_filepath, _MAX_PATH, pluginpath );

	return 0;
}


#define GPA(proc) \
	*(FARPROC *)&proc = GetProcAddress(m_hModule, #proc);


int CPluginKinect::LoadPlugin( HWND srchwnd )
{
	m_validflag = 0;//!!!!!!!!!!!!

	char dllname[_MAX_PATH];
	sprintf_s( dllname, _MAX_PATH, "%s\\%s", m_filepath, m_filename );

	m_hModule = LoadLibrary( dllname );
	if(m_hModule == NULL){
		DbgOut( "PluginKinect : LoadPlugin : LoadLibrary error %s!!!\r\n", dllname );
		_ASSERT( 0 );
		return 0;
	}

	GPA(OpenNIInit);
	GPA(OpenNIClean);
	GPA(OpenNIDrawDepthMap);
	GPA(OpenNIDepthTexture);
	GPA(OpenNIGetSkeltonJointPosition);
	GPA(OpenNIIsTracking);
	GPA(OpenNIGetVersion);



	if( !OpenNIInit || !OpenNIClean || !OpenNIDrawDepthMap || !OpenNIDepthTexture || !OpenNIGetSkeltonJointPosition || 
		!OpenNIIsTracking || !OpenNIGetVersion ){
		DbgOut( "PluginKinect : LoadPlugin : this dll is not for RDB2 %s\r\n", m_filename );
		return 0;//!!!!!!!!!!!!!!!!!
	}

////////////
	if( OpenNIGetVersion ){
		OpenNIGetVersion( &m_pluginversion );
	}


	m_dlg = new CConvSetDlg();
	if( !m_dlg ){
		_ASSERT( 0 );
		return 1;
	}
	m_dlg->Create( srchwnd );


	m_validflag = 1;//!!!!!!!!!!!!!!!!!!!!!!!!!	


	return 0;
}
int CPluginKinect::UnloadPlugin()
{
	m_validflag = 0;//!!!!!!!!!!!!!!!!

	if( m_hModule ){
		FreeLibrary( m_hModule );
		m_hModule = NULL;
	}
	
	return 0;
}


int CPluginKinect::StartCapture( int capmode )
{
	bool bret;
	if( m_validflag == 0 ){
		::MessageBoxA( m_hwnd, "Kinect用のプラグインが正常に読み込まれなかったので\nキャプチャ出来ません。", "エラー", MB_OK );
		return 1;
	}
	if( !m_ts ){
		::MessageBoxA( m_hwnd, "kstファイルが読み込まれていないのでキャプチャ出来ません。\nkstファイルを読み込んでから再試行してください。", "エラー", MB_OK );
		return 1;
	}
	m_capmode = capmode;
	if( m_capmode != 2 ){
		::MessageBox( m_hwnd, "kinect capture start", "kinect", MB_OK );
	}else{
		CMultiCapDlg dlg( g_motdlg->m_motparamdlg->m_sl_mp_rangemax + 1 );
		int dlgret;
		dlgret = (int)dlg.DoModal();
		if( dlgret != IDOK ){
			return 0;
		}
		m_capstep = dlg.m_framestep;
	}

	m_rendercnt = 0;

	DestroyRps();

	OpenNIClean();
	bret = OpenNIInit( 0, m_hwnd, 0, m_pdev, m_filepath, 0 );
	if( bret == false ){
		_ASSERT( 0 );
		m_validflag = 0;
		return 1;
	}

	OpenNIDepthTexture( &m_ptex );
	if( !m_ptex ){
		_ASSERT( 0 );
		return 1;
	}


	HRESULT hr;
	D3DSURFACE_DESC	sdesc;
	hr = m_ptex->GetLevelDesc( 0, &sdesc );
	if( hr != D3D_OK ){
		DbgOut( "PluginKinect : GetLevelDesc error !!!\n" );
		_ASSERT( 0 );
		m_validflag = 0;
		return 1;
	}
	m_texwidth = sdesc.Width;
	m_texheight = sdesc.Height;


	int ret;

	m_rps = new CRpsFile( m_papp );
	if( !m_rps ){
		_ASSERT( 0 );
		m_validflag = 0;
		return 1;
	}
	ret = m_rps->CreateParams();
	if( ret ){
		_ASSERT( 0 );
		m_validflag = 0;
		return 1;
	}


	D3DXVECTOR3 xpdir( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 zmdir( 0.0f, 0.0f, -1.0f );
	D3DXVECTOR3 zpdir( 0.0f, 0.0f, 1.0f );

	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_KNEE)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_FOOT)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_KNEE)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_FOOT)->jointno, xpdir );
	_ASSERT( !ret );

	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_ELBOW)->jointno, zmdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_HAND)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_ELBOW)->jointno, zpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_HAND)->jointno, xpdir );
	_ASSERT( !ret );


	m_capmotid = g_motdlg->GetMotCookie();
	if( m_capmotid < 0 ){
		_ASSERT( 0 );
		return 0;
	}


	if( m_capmode == 0 ){
		m_capstartframe = 0;
		m_capendframe = g_motdlg->m_motparamdlg->m_sl_mp_rangemax;
		m_capframe = 0;

	}else if( m_capmode == 1 ){
		m_capstartframe = g_motdlg->m_motparamdlg->m_current;
		m_capendframe = m_capstartframe;
		m_capframe = m_capstartframe;

		m_curtimer = m_timermax;
	}else{
		m_capstartframe = g_motdlg->m_motparamdlg->m_current;
		m_capendframe = g_motdlg->m_motparamdlg->m_sl_mp_rangemax;
		m_capframe = m_capstartframe;

		m_curtimer = m_timermax;
	}

	if( m_capmode != 2 ){
		ret = CreateMotionPoints( m_capmotid, m_capstartframe, m_capendframe, 1 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int creframe = m_capframe;
		while( creframe <= m_capendframe ){
			ret = CreateMotionPoints( m_capmotid, creframe, creframe, 1 );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			creframe += m_capstep;
		}
	}

	ret = m_papp->m_mhandler->FillUpMotion( m_papp->m_shandler, m_capmotid, 
		(m_ts->m_elem + SKEL_TOPOFJOINT)->jointno, m_capstartframe, m_capendframe, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
int CPluginKinect::EndCapture()
{
	if( m_validflag == 0 ){
		return 0;
	}

	int ret;
	if( m_rendercnt != 0 ){
		ret = m_papp->m_mhandler->FillUpMotion( m_papp->m_shandler, m_capmotid, 
			(m_ts->m_elem + SKEL_TOPOFJOINT)->jointno, -1, -1, 1 );
		_ASSERT( !ret );
	}

	m_ptex = NULL;
	OpenNIClean();

	return 0;
}

int CPluginKinect::Update()
{
	if( (m_validflag == 0) || !m_ts || !m_rps ){
		_ASSERT( 0 );
		return 0;
	}

	OpenNIDrawDepthMap( 1 );

	bool trflag = false;
	OpenNIIsTracking( &trflag );

	D3DXVECTOR3 tmppos[ SKEL_MAX ];
	int ret;


	if( (m_rendercnt == 0) && trflag ){
		m_capframe = m_capstartframe;
		g_motdlg->m_motparamdlg->m_sl_mp_wnd.SendMessage( TBM_SETPOS, (WPARAM)TRUE, (LPARAM)m_capframe );

		ret = m_rps->InitArMp( m_ts->m_elem, m_capmotid, m_capframe );
		_ASSERT( !ret );
		
		OpenNIGetSkeltonJointPosition( SKEL_MAX, tmppos );
		int skno;
		for( skno = 0; skno < SKEL_MAX; skno++ ){
			tmppos[skno].x *= -1.0f;
		}
		ret = m_rps->SetRpsElem( 0, tmppos );
		_ASSERT( !ret );

		ret = m_rps->CalcTraQ( m_ts->m_elem, 1 );
		_ASSERT( !ret );

		ret = m_rps->SetMotion( m_ts->m_elem, m_capmotid, m_capframe );
		_ASSERT( !ret );

		m_rendercnt++;
	}else if( trflag ){

		int dosetflag = 0;

		if( m_capmode == 0 ){
			m_capframe++;
			if( m_capframe > m_capendframe ){
				m_rendercnt = 0;

				ret = m_papp->m_mhandler->FillUpMotion( m_papp->m_shandler, m_capmotid, 
					(m_ts->m_elem + SKEL_TOPOFJOINT)->jointno, -1, -1, 1 );
				_ASSERT( !ret );
				return 2;//!!!!!!!!!!!!!!!!!終わりの印
			}
			dosetflag = 1;
		}else if( m_capmode == 1 ){
			m_curtimer--;
			if( m_curtimer <= 0 ){
				m_rendercnt = 0;

				ret = m_papp->m_mhandler->FillUpMotion( m_papp->m_shandler, m_capmotid, 
					(m_ts->m_elem + SKEL_TOPOFJOINT)->jointno, -1, -1, 1 );
				_ASSERT( !ret );
				return 2;//!!!!!!!!!!!!!!!!!終わりの印
			}
			dosetflag = 1;
		}else if( m_capmode == 2 ){
			m_curtimer--;
			if( m_curtimer > 0 ){
				dosetflag = 1;
			}else if( m_curtimer == 0 ){
				m_rendercnt = 0;

				if( (m_capframe + m_capstep) > m_capendframe ){
					m_rendercnt = 0;
					ret = m_papp->m_mhandler->FillUpMotion( m_papp->m_shandler, m_capmotid, 
						(m_ts->m_elem + SKEL_TOPOFJOINT)->jointno, -1, -1, 1 );
					_ASSERT( !ret );
					return 2;//!!!!!!!!!!!!!!!!!終わりの印
				}
				dosetflag = 1;
			}else{
				if( m_curtimer > -50 ){
					dosetflag = 0;
				}else{
					dosetflag = 0;
					m_curtimer = m_timermax;
					m_capframe += m_capstep;
				}
			}
		}

		OpenNIGetSkeltonJointPosition( SKEL_MAX, tmppos );
		int skno;
		for( skno = 0; skno < SKEL_MAX; skno++ ){
			tmppos[skno].x *= -1.0f;
		}
		ret = m_rps->SetRpsElem( 2, tmppos );
		_ASSERT( !ret );

		ret = m_rps->CalcTraQ( m_ts->m_elem, 1 );
		_ASSERT( !ret );

		if( dosetflag == 1 ){
			ret = m_rps->SetMotion( m_ts->m_elem, m_capmotid, m_capframe );
			_ASSERT( !ret );
		}
		m_rendercnt++;
	}else{
		m_rendercnt = 0;
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!
	}


	return 0;
}

int CPluginKinect::LoadTxtSetting()
{
	if( m_validflag == 0 ){
		_ASSERT( 0 );
		return 0;
	}

	DestroyTxtSetting();

	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hwnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"kstファイル\0*.kst\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 ){
		return 0;
	}

	int ret;
	m_ts = new CTxtSetting( m_papp );
	if( !m_ts ){
		_ASSERT( 0 );
		m_validflag = 0;
		return 1;
	}
	ret = m_ts->LoadParams( buf );
	if( ret ){
		_ASSERT( 0 );
		m_validflag = 0;
		DestroyTxtSetting();
		return 1;
	}

	return 0;
}

int CPluginKinect::EditTxtSetting()
{


	if( m_validflag == 0 ){
		_ASSERT( 0 );
		return 0;
	}

	DestroyTxtSetting();

	m_ts = new CTxtSetting( m_papp );
	if( !m_ts ){
		_ASSERT( 0 );
		m_validflag = 0;
		return 1;
	}

	m_dlg->ShowDlg( SW_SHOW, m_papp, m_ts );

	return 0;
}


int CPluginKinect::CreateMotionPoints( int cookie, int startframe, int endframe, int forceflag )
{
	//forceflag == 1のときは初期状態のモーションポイントを作る。

	if( m_papp->m_mhandler->s2mot_leng < 1 ){
		_ASSERT( 0 );
		return 1;//!!!!!!!!
	}

	int elemno;//, framenum;
	CMotionCtrl* mcptr;
	CMotionPoint2* curmp;
	CShdElem* selem;

	CQuaternion initq;
	initq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

	int interp;
	m_papp->m_mhandler->GetDefInterp( cookie, &interp );

	int frameno;
	int ret;
	for( frameno = startframe; frameno <= endframe; frameno++ ){
		for( elemno = 1; elemno < m_papp->m_mhandler->s2mot_leng; elemno++ ){
			mcptr = (*m_papp->m_mhandler)( elemno );
			if( !mcptr ){
				_ASSERT( 0 );
				return 1;
			}
			selem = (*m_papp->m_shandler)( elemno );
			_ASSERT( selem );

			if( mcptr->IsJoint() && (mcptr->type != SHDMORPH)){
				curmp = mcptr->IsExistMotionPoint( cookie, frameno );
				if( !curmp ){

					CMotionPoint2 tmpmp;
					tmpmp.InitParams();

					if( forceflag == 0 ){
						int hasmpflag = 0;
						ret = mcptr->CalcMotionPointOnFrame( selem, &tmpmp, cookie, frameno, &hasmpflag,
							m_papp->m_shandler, m_papp->m_mhandler );
						if( ret ){
							_ASSERT( 0 );
							return 1;
						}
					}

					CMotionPoint2* newmp;
					newmp = mcptr->AddMotionPoint( cookie, frameno, tmpmp.m_q, tmpmp.m_mvx, tmpmp.m_mvy, tmpmp.m_mvz,
						0, interp, tmpmp.m_scalex, tmpmp.m_scaley, tmpmp.m_scalez, 0 );
					if( !newmp ){
						_ASSERT( 0 );
						return 1;
					}
				}else{
					if( forceflag == 1 ){
						curmp->m_q.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
						curmp->m_eul = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
						curmp->m_mvx = 0.0f;
						curmp->m_mvy = 0.0f;
						curmp->m_mvz = 0.0f;
						curmp->m_scalex = 1.0f;
						curmp->m_scaley = 1.0f;
						curmp->m_scalez = 1.0f;
					}
				}
			}
		}
	}
		
	return 0;
}

int CPluginKinect::ApplyFilter()
{
	if( m_validflag == 0 ){
		::MessageBoxA( m_hwnd, "Kinect用のプラグインが正常に読み込まれなかったので\n平滑化出来ません。", "エラー", MB_OK );
		return 1;
	}
	if( !m_ts ){
		::MessageBoxA( m_hwnd, "kstファイルが読み込まれていないので平滑化出来ません。\nkstファイルを読み込んでから再試行してください。", "エラー", MB_OK );
		return 1;
	}


	int ret;
	D3DXVECTOR3 xpdir( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 zmdir( 0.0f, 0.0f, -1.0f );
	D3DXVECTOR3 zpdir( 0.0f, 0.0f, 1.0f );

	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_KNEE)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_FOOT)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_KNEE)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_FOOT)->jointno, xpdir );
	_ASSERT( !ret );

	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_ELBOW)->jointno, zmdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_RIGHT_HAND)->jointno, xpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_ELBOW)->jointno, zpdir );
	_ASSERT( !ret );
	ret = m_papp->m_shandler->SetMikoBonePos( (m_ts->m_elem + SKEL_LEFT_HAND)->jointno, xpdir );
	_ASSERT( !ret );




	CFilterDlg dlg;
	ret = (int)dlg.DoModal();
	if( ret != IDOK ){
		return 0;
	}

	int curmotid;
	curmotid = g_motdlg->GetMotCookie();
	if( curmotid < 0 ){
		return 0;
	}

	int frameleng = 0;
	ret = m_papp->m_mhandler->GetMotionFrameLength( curmotid, &frameleng );
	if( ret || (frameleng <= 1) ){
		_ASSERT( 0 );
		return 0;
	}

	ret = CreateMotionPoints( curmotid, 0, frameleng - 1, 0 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	ret = AvrgMotion( curmotid, frameleng, dlg.m_filtertype, dlg.m_filtersize );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = m_papp->m_mhandler->FillUpMotion( m_papp->m_shandler, curmotid, -1, 0, frameleng - 1, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = g_motdlg->SetMotionFrameNo( curmotid, 0 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

/***********************************************************

　　フィルタサイズが大きいほど、
  　たくさんのフレーム間で平滑がなされる。

　　・移動平均：
　　　　通常の線形による平滑化。

　　・加重移動平均：
　　　　徐々に重みを小さくしていく平滑化。
　　　　移動平均よりも、少しピークが残りやすい。

　　・ガウシアン：
　　　　ガウシアンフィルタによる平滑化。
	　　元のモーションの再現率が大きい。（といいなあ）
　
***********************************************************/

int CPluginKinect::AvrgMotion( int motid, int frameleng, int type, int filter_size )
{
	int half_filtersize = filter_size / 2;

	//errno_t err;
	int ret;
	D3DXVECTOR3 tmp_vec3;
	D3DXVECTOR3 tmp_pos3;

	//オイラー角変換用
	D3DXVECTOR3 dirx( 1.0f, 0.0f, 0.0f );
	D3DXVECTOR3 diry( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 dirz( 0.0f, 0.0f, 1.0f );
	CQuaternion qx, qy, qz;

	//オイラー角取得用
	CQuaternion curq;
	CQuaternion axisq;
	CQuaternion invaxisq;

	//オイラー角格納用
	D3DXVECTOR3 *smooth_eul;
	D3DXVECTOR3 *eul;
	D3DXVECTOR3 befeul;
	smooth_eul	= new D3DXVECTOR3[ frameleng ];
	if( !smooth_eul ){
		_ASSERT( 0 );
		return 1;
	}
	eul	= new D3DXVECTOR3[ frameleng ];
	if( !eul ){
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3* tra;
	tra = new D3DXVECTOR3[ frameleng ];
	if( !tra ){
		_ASSERT( 0 );
		return 1;
	}
	D3DXVECTOR3* smooth_tra;
	smooth_tra = new D3DXVECTOR3[ frameleng ];
	if( !smooth_tra ){
		_ASSERT( 0 );
		return 1;
	}



	int skno, seri;
	for( skno = 0; skno < SKEL_MAX; skno++ ){
		seri = m_ts->m_elem[skno].jointno;

		if( (seri <= 0) || (seri >= m_papp->m_mhandler->s2mot_leng) ){
			_ASSERT( 0 );
			continue;
		}

		CMotionCtrl* mcptr;
		mcptr = (*m_papp->m_mhandler)( seri );
		_ASSERT( mcptr );
		if( mcptr->IsJoint() ){
			CMotionPoint2* firstmp;
			firstmp = m_papp->m_mhandler->GetFirstMotionPoint( seri, motid );
			if( !firstmp ){
				_ASSERT( 0 );
				return 1;
			}
			CMotionPoint2* curmp = firstmp;

			//オイラー角初期化
			befeul = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			ZeroMemory( eul, sizeof( D3DXVECTOR3 ) * frameleng );
			ZeroMemory( smooth_eul, sizeof( D3DXVECTOR3 ) * frameleng );
			ZeroMemory( tra, sizeof( D3DXVECTOR3 ) * frameleng );
			ZeroMemory( smooth_tra, sizeof( D3DXVECTOR3 ) * frameleng );
			


			//全クォータニオンに対するオイラー角の取得
			for( int frameno = 0; frameno < frameleng; frameno++ ){
				if( !curmp ){
					_ASSERT( 0 );
					return 1;
				}
				curq = curmp->m_q;
				ret = QtoEul( m_papp->m_shandler, curq, befeul, seri, eul + frameno, &axisq );
				_ASSERT( !ret );
				//eul[frameno] = curmp->m_eul;

				(tra + frameno)->x = curmp->m_mvx;
				(tra + frameno)->y = curmp->m_mvy;
				(tra + frameno)->z = curmp->m_mvz;

				befeul = eul[frameno];
				curmp = curmp->next;
			}


			//平滑化処理
			switch (type) {

			case AVGF_NONE:							//平滑処理なし
				break;

			case AVGF_MOVING:					//移動平均
				//平滑化処理
				for( int frameno = 0; frameno < frameleng; frameno++ ){
					tmp_vec3.x = tmp_vec3.y = tmp_vec3.z = 0.0f;
					tmp_pos3.x = tmp_pos3.y = tmp_pos3.z = 0.0f;
					for( int k=-half_filtersize; k<= half_filtersize; k++ ){
						int index = frameno + k;
						if( ( index < 0 )||( index >= frameleng ) ){
							tmp_vec3 += eul[frameno];
							tmp_pos3 += tra[frameno];
						}
						else{
							tmp_vec3 += eul[index];
							tmp_pos3 += tra[index];
						}
					}
					smooth_eul[frameno] = tmp_vec3 / (float)filter_size;
					smooth_tra[frameno] = tmp_pos3 / (float)filter_size;
				}
				break;

			case AVGF_WEIGHTED_MOVING:			//加重移動平均
			{
				double denomVal = 0.0;
				float weightVal = 0.0;

				int sumd  = 0;
				int coef = 0;
				for( int i=1; i<=filter_size; i++ ){
					sumd += i;
				}
				denomVal = 1.0 / (double)sumd;

				for( int frameno = 0; frameno < frameleng; frameno++ ){
					tmp_vec3.x = tmp_vec3.y = tmp_vec3.z = 0.0f;
					tmp_pos3.x = tmp_pos3.y = tmp_pos3.z = 0.0f;
					for( int k=-filter_size; k<= 0; k++ ){
						int index = frameno + k;
						coef = k + filter_size;
						weightVal = (float)( denomVal * (double)coef );
						if( ( index < 0 )||( index >= frameleng ) ){
							tmp_vec3 = tmp_vec3 + eul[frameno] * weightVal;
							tmp_pos3 = tmp_pos3 + tra[frameno] * weightVal;
						}
						else{
							tmp_vec3 = tmp_vec3 + eul[index] * weightVal;
							tmp_pos3 = tmp_pos3 + tra[index] * weightVal;
						}
					}
					smooth_eul[frameno] = tmp_vec3;
					smooth_tra[frameno] = tmp_pos3;
				}
				break;
			}
								
			case AVGF_GAUSSIAN:					//ガウシアン 

				double normalizeVal = 0.0;
				float normalDistVal = 0.0;

				int N = filter_size - 1;
				int sum = 0;
				int r = 0;
				for( int i=0; i<=N; i++ ){
					sum +=  combi( N, i );
				}
				normalizeVal = 1.0 / (double)sum;

				//for( int i=0; i< (SKEL_MAX*m_framenum); i+=SKEL_MAX ){
				//	for( int j=i; j< (i+SKEL_MAX); j++ ){
				for( int frameno = 0; frameno < frameleng; frameno++ ){
					tmp_vec3.x = tmp_vec3.y = tmp_vec3.z = 0.0f;
					tmp_pos3.x = tmp_pos3.y = tmp_pos3.z = 0.0f;
					for( int k=-half_filtersize; k<= half_filtersize; k++ ){
						int index = frameno + k;
						r = k + half_filtersize;
						normalDistVal = (float)( normalizeVal * (double) combi( N, r ) );
						if( ( index < 0 )||( index >= frameleng ) ){
							tmp_vec3 = tmp_vec3 + eul[frameno] * normalDistVal;
							tmp_pos3 = tmp_pos3 + tra[frameno] * normalDistVal;
						}
						else{
							tmp_vec3 = tmp_vec3 + eul[index] * normalDistVal;
							tmp_pos3 = tmp_pos3 + tra[index] * normalDistVal;
						}
					}
					smooth_eul[frameno] = tmp_vec3;
					smooth_tra[frameno] = tmp_pos3;
				}
				
				break;
			}


			//モーションポイントに計算値をセット
			//全クォータニオンに対するオイラー角の取得
			ret = m_papp->m_shandler->GetInitialBoneQ( seri, &axisq );
			axisq.inv( &invaxisq );
			curmp = firstmp;
			for( int frameno = 0; frameno < frameleng; frameno++ ){
				if( !curmp ){
					_ASSERT( 0 );
					return 1;
				}

				qx.SetAxisAndRot( dirx, smooth_eul[frameno].x * DEG2PAI );
				qy.SetAxisAndRot( diry, smooth_eul[frameno].y * DEG2PAI );
				qz.SetAxisAndRot( dirz, smooth_eul[frameno].z * DEG2PAI );

				curmp->m_q = axisq * qy * qx * qz * invaxisq;

				curmp->m_mvx = smooth_tra[frameno].x;
				curmp->m_mvy = smooth_tra[frameno].y;
				curmp->m_mvz = smooth_tra[frameno].z;

				curmp = curmp->next;
			}
		}
	}

	delete [] smooth_eul;
	delete [] eul;
	delete [] tra;
	delete [] smooth_tra;

	return 0;
}

//ガウシアンフィルタ用
int CPluginKinect::combi( int N, int rp ) {
	int n, r;

	//２次元配列の確保
	int **combi = new int*[N+1];
	combi[0] = new int[(N+1) * (N+1)];
	for(int i = 1; i < N+1; i++){
		combi[i] = combi[0] + i * (N+1);
	}

	//結果を残す方法
	//・nCr = n-1Cr-1 + n-1Cr 
	//・nC0 = 1, nCn = 1 　　　を用いて計算する
	for(n = 0; n <= N; n ++){
		combi[n][0] = combi[n][n] = 1;
		for(r = 1; r < n; r ++){
			combi[n][r] = combi[n - 1][r - 1] + combi[n - 1][r];
		}
	}

	int ret = combi[N][rp];

	delete[] combi[0];
	delete[] combi;

	return ret;
}

int CPluginKinect::QtoEul( CShdHandler* lpsh, CQuaternion srcq, D3DXVECTOR3 befeul, int boneno, D3DXVECTOR3* eulptr, CQuaternion* axisqptr )
{
	int ret;
	ret = lpsh->GetInitialBoneQ( boneno, axisqptr );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	D3DXVECTOR3 cureul( 0.0f, 0.0f, 0.0f );
	ret = qToEulerAxis( *axisqptr, &srcq, &cureul );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = modifyEuler( &cureul, &befeul );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	*eulptr = cureul;

	return 0;
}
