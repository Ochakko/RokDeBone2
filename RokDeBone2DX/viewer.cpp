//-----------------------------------------------------------------------------
// OrgFile: PointSprites.cpp
//
// Desc: Sample showing how to use point sprites to do particle effects
//
// Copyright (c) 1997-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


#include "stdafx.h"


//!!!!!!!!!!!!!!!!!!!!!!!!!!!
// キャストのwaringを非表示にする
//!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#pragma warning(disable:4311)


#include "CameraDlg.h"
//#include "SigEditDlg.h"
#include "SigEditWnd.h"
#include "OptDlg.h"
#include "MotDlg.h"
#include "BgDlg.h"
#include "VersionDlg.h"
#include "LdSigDlg.h"
#include "WriteBFDlg.h"
#include "ParsDlg.h"
#include "BoneLineDlg.h"
#include "WriteMQODlg.h"
#include "WriteIM2Dlg.h"
#include "MikoBlendDlg.h"
#include "LoadNameDlg.h"
#include "LdGroundDlg.h"
#include "ItiModeDlg.h"
#include "ItiDlg.h"
#include "LightDlg.h"
#include "SetColDlg.h"
#include "CreateBoneDlg.h"
#include "InfluenceDlg.h"
#include "InfScopeDlg.h"
#include "PaintDlg.h"
#include "WriteXFileDlg.h"
#include "Toon1Dlg.h"
//#include "LdFBXDlg.h"
#include "LdXDlg.h"
#include "MotChangeDlg.h"
#include "OverWriteDlg.h"
#include "LdQuaDlg.h"
#include "ExtSigDlg.h"
#include "MaterialDlg.h"
#include "NormalMapDlg.h"
#include "BonePropDlg.h"
#include "LdMkmDlg.h"

#include "PluginElem.h"

#include <SymmOpe.h>

#define STRICT
//#include <windowsx.h>
#include <windows.h>
//#include <winuser.h>
#include <tchar.h>
#include <assert.h>
#include <mmsystem.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <d3dx9.h>


//#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "resource.h"

#include "viewer.h"

#include <coef.h>

#define DBGH
#include <dbg.h>

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdhandler.h>
#include <mothandler.h>

#include <sigfile.h>
#include <rokfile.h>
#include <mqofile.h>
#include <BoneFile.h>
#include <XFile.h>
#include <MAFile.h>
#include <LimFile.h>
#include <BVHFile.h>
#include <mkmfile.h>

//#include <FBXFile.h>

//#include <BoneRateFile.h>

#include <dxerr8.h>

#include <Commdlg.h>

#include <bgdisp2.h>

#include <basedat.h>
#include <dibfile.h>

#include <lightdata.h>

#include "MQODlg.h"

#include "inidata.h" // for ini file

#include <im2file.h>
#include <cpuid.h>
//#include <asmfunc.h>

#include "ColDlg.h"

#include "resid.h"
#include "RMenuMain.h"

#include <texbank.h>
#include <quaternion2.h>
#include <motionctrl.h>
#include <motioninfo.h>
#include <InfScope.h>
#include <quafile.h>

//#include <Zmouse.h>

//#include "AxisDlg.h"

#include "WriteMAFDlg.h"

enum {
	SELECT_NONE,
	SELECT_X,
	SELECT_Y,
	SELECT_Z,
	SELECT_CENTER,
	SELECT_MAX
};


static char strselect[SELECT_MAX][60] = {
	"SELECT_NONE",
	"SELECT_X",
	"SELECT_Y",
	"SELECT_Z",
	"SELECT_CENTER"
};

//-------- define
//#define	RADINITVAL	0.0f
//#define RAD2INITVAL	0.0f

#define TIMERINTERVAL	16


static LPITEMIDLIST GetNextItemID(LPCITEMIDLIST pidl);


//toolbar button id
//IDTBB_*はcoef.hへ

TBBUTTON tbb[22] = {
		{0, IDTBB_R, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{1, IDTBB_T, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{2, IDTBB_T2, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{3, IDTBB_S, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{4, IDTBB_ITI, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{5, IDTBB_IKT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{6, IDTBB_COL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{7, IDTBB_BONE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{8, IDTBB_EI1, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{9, IDTBB_EI2, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{10, IDTBB_EI3, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{11, IDTBB_TOON1, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{12, IDTBB_MO, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},	
		{13, IDTBB_FACE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{14, IDTBB_LINE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{15, IDTBB_POINT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{16, IDTBB_TEN2, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{17, IDTBB_ZA1, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{18, IDTBB_ZA2, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{19, IDTBB_ZA3, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

		{20, IDTBB_COLP, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{21, IDTBB_COLT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
};
TBBUTTON tb_space = {0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0};

HWND g_hToolBar = NULL;


//------- global
ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface

int g_rendertarget = TARGET_DEFAULT;
float g_specular = 0.0f;
float g_ambient = 0.3f;

float g_proj_far = 50000.0f;//<---- RokDeBone2では、m_proj_farを使う、d3ddisp.cppのために、ここで、定義。

int g_fogenable = 0;
D3DCOLOR g_fogcolor = 0x00FFFFFF;
float g_fogstart = 0.0f;
float g_fogend = g_proj_far;	


//CAxisDlg* g_axisdlg = 0;
CMotDlg* g_motdlg = 0;
CBonePropDlg*	g_bonepropdlg = 0;


DWORD g_renderstate[ D3DRS_BLENDOP + 1 ];
int g_rsindex[80] = {
    7,8,9,10,14,15,16,19,20,22,
    23,24,25,26,27,28,29,30,34,35,
    36,37,38,40,47,48,52,53,54,55,
    56,57,58,59,60,128,129,130,131,132,
    133,134,135,136,137,139,140,141,142,143,
    145,146,147,148,151,152,153,154,155,156,
    157,158,159,160,161,162,163,164,165,166,
    167,168,170,171,-1,-1,-1,-1,-1,-1
};


UINT g_miplevels = 0;
DWORD g_mipfilter = D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR;

DWORD g_minfilter = D3DTEXF_LINEAR;
DWORD g_magfilter = D3DTEXF_LINEAR;

//CCpuid g_cpuinfo;

HWND g_hStatus = 0;
#define ID_STATUSBAR 100

CColDlg g_coldlg;

CRITICAL_SECTION g_crit_tempmp; 

int g_rendercnt = 0;

int g_LightOfBump = 0;
int g_lightnum = 0;

//------------------------
//shader
//------------------------
D3DXHANDLE g_hRenderSceneBone4 = 0;
D3DXHANDLE g_hRenderSceneBone1 = 0;
D3DXHANDLE g_hRenderSceneBone0 = 0;
D3DXHANDLE g_hRenderSceneBone4Toon = 0;
D3DXHANDLE g_hRenderSceneBone1Toon = 0;
D3DXHANDLE g_hRenderSceneBone0Toon = 0;
D3DXHANDLE g_hRenderSceneBone4ToonNoTex = 0;
D3DXHANDLE g_hRenderSceneBone1ToonNoTex = 0;
D3DXHANDLE g_hRenderSceneBone0ToonNoTex = 0;
D3DXHANDLE g_hRenderSceneBone4Toon1 = 0;
D3DXHANDLE g_hRenderSceneBone1Toon1 = 0;
D3DXHANDLE g_hRenderSceneBone0Toon1 = 0;
D3DXHANDLE g_hRenderSceneBone4Bump = 0;//vs2
D3DXHANDLE g_hRenderSceneBone1Bump = 0;//vs2
D3DXHANDLE g_hRenderSceneBone0Bump = 0;//vs2
D3DXHANDLE g_hRenderSceneBone4Edge0 = 0;
D3DXHANDLE g_hRenderSceneBone1Edge0 = 0;
D3DXHANDLE g_hRenderSceneBone0Edge0 = 0;

D3DXHANDLE g_hmWorldMatrixArray = 0;
D3DXHANDLE g_hmView = 0;
D3DXHANDLE g_hmProj = 0;
D3DXHANDLE g_hmEyePos = 0;
D3DXHANDLE g_hmLightParams = 0;
D3DXHANDLE g_hmBumpLight = 0;//vs2
D3DXHANDLE g_hmFogParams = 0;
D3DXHANDLE g_hmToonParams = 0;
D3DXHANDLE g_hmLightNum = 0;
D3DXHANDLE g_hmEdgeCol0 = 0;
D3DXHANDLE g_hmAlpha = 0;
D3DXHANDLE g_hNormalMap = 0;//vs2
D3DXHANDLE g_hDecaleTex = 0;//vs2


//-------- extern
extern int g_useGPU;
extern int g_chkVS;
extern CLightData* g_lighthead;//d3dapp.cppで定義
	//extern rokdebone2.cpp
extern CIniData* g_inidata;

extern CTexBank* g_texbnk;

extern int FreeLexBuf();
//extern HINSTANCE g_hinstance;

extern DWORD g_cop0;
extern DWORD g_cop1;
extern DWORD g_aop0;
extern DWORD g_aop1;


//-------- static member ----
static int s_tlmode = TLMODE_ORG;
static D3DXMATRIX s_inimat;

static unsigned char s_KeyBuffer[256];
static DWORD s_MotStep = 0;


//static DWORD s_trans_time = 0;
//static DWORD s_update_time = 0;
//static DWORD s_render_time = 0;

static DWORD s_trans_time_low = 0;
static DWORD s_trans_time_high = 0;
static DWORD s_update_time_low = 0;
static DWORD s_update_time_high = 0;
static DWORD s_render_time_low = 0;
static DWORD s_render_time_high = 0;

static DWORD s_present_time_low = 0;
static DWORD s_present_time_high = 0;

static DWORD s_lock_time_low = 0;
static DWORD s_lock_time_high = 0;

static CRMenuMain* s_rmenumain = 0;

//-------- static func ------
//static BOOL CALLBACK ChangeMotStepProc( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam );
static VOID UpdateDialogControls( HWND hDlg, DWORD dwMotStep );


// Helper function to stuff a float into a DWORD argument
inline DWORD FtoDW( float f ) { return *((DWORD*)&f); }

//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------



/***
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    CMyD3DApplication d3dApp;

    if( FAILED( d3dApp.Create( hInst ) ) )
        return 0;

    return d3dApp.Run();
}
***/



//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{

    InitializeCriticalSection( &g_crit_tempmp );

	m_strWindowTitle    = _T("RokDeBone2");
    m_bUseDepthBuffer   = TRUE;

	m_d3dxsprite = NULL;

    m_pFont                = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
    m_pFontSmall           = new CD3DFont( _T("Arial"), 9, D3DFONT_BOLD );

	m_dirlightid = -1;

    ZeroMemory( m_bKey, 256 );
	m_exit = 0;
	m_ExistData = FALSE;

	m_thandler = 0;
	m_shandler = 0;
	m_mhandler = 0;

	m_ext_th = 0;
	m_ext_sh = 0;
	m_ext_mh = 0;

	m_ground_th = 0;
	m_ground_sh = 0;
	m_ground_mh = 0;

	m_g_th = 0;
	m_g_sh = 0;
	m_g_mh = 0;

	m_arrow_th = 0;
	m_arrow_sh = 0;
	m_arrow_mh = 0;

	m_select_th = 0;
	m_select_sh = 0;
	m_select_mh = 0;
	D3DXMatrixIdentity( &m_select_mat );
	m_sel_xno = 0;//"obj_X"
	m_sel_yno = 0;//"obj_Y"
	m_sel_zno = 0;//"obj_Z"
	m_sel_cenno = 0;//"obj_CENTER"

	m_select_kind = SELECT_NONE;
	m_selectedno = 0;
	m_selectedpos.x = 0.0f;
	m_selectedpos.y = 0.0f;
	m_selectedpos.z = 0.0f;
	m_select_delta = 0.0f;

	m_ikt_th = 0;
	m_ikt_sh = 0;
	m_ikt_mh = 0;

	m_sph_th = 0;
	m_sph_sh = 0;
	m_sph_mh = 0;


	m_bm_th = 0;
	m_bm_sh = 0;
	m_bm_mh = 0;

	m_sq_th = 0;
	m_sq_sh = 0;
	m_sq_mh = 0;
	D3DXMatrixIdentity( &m_sq_mat );

	m_curcookie = -1;

	m_motcookie0 = -1;
	ZeroMemory( m_motname0, 256 );
	m_framemax0 = 0;
	m_mottype0 = 0;

	m_motcookie1 = -1;
	ZeroMemory( m_motname1, 256 );
	m_framemax1 = 0;
	m_mottype1 = 0;

	m_degxz = DEGXZINITVAL;
	m_degy = DEGYINITVAL;
	m_eye_y = EYEYINITVAL;
	m_camdist = CAMDISTINITVAL;

	m_targetpos.x = 0.0f;
	m_targetpos.y = m_eye_y;
	m_targetpos.z = 0.0f;

	m_create3dobj = 0;

	//g_motdlg = 0;

	m_cameradlg = 0;
	m_sigeditwnd = 0;
	m_optdlg = 0;
	m_bonelinedlg = 0;
	m_loadnamedlg = 0;
	m_itimodedlg = 0;
	m_itidlg = 0;
	m_lightdlg = 0;
	m_setcoldlg = 0;
	m_bonedlg = 0;
	m_infscopedlg = 0;
	m_paintdlg = 0;
	m_toon1dlg = 0;
	m_mcdlg = 0;
	m_wmafdlg = 0;
	m_materialdlg = 0;
	g_bonepropdlg = 0;

	//m_disp_cameradlg = 0;
	//m_disp_sigdlg = 0;
	//m_disp_motdlg = 0;
	m_refuge_motdlg = 0;

	m_moving_cameradlg = 0;
	m_moving_sigdlg = 0;


	m_parsdlg = 0;
	//m_polydispmode = POLYDISP_ALL;
	//m_grounddisp = 1;

	m_menucnt = 0;

	m_appmenu = NULL;
	m_dummymenu = NULL;
	m_pluginmenu = NULL;

	m_gband.maxrhw = 1e8;
	m_gband.left = -2048.0f;
	m_gband.top = -2048.0f;
	m_gband.right = 2047.0f;
	m_gband.bottom = 2047.0f;

	m_smallgband = m_gband;

	m_clientWidth = 0;
	m_clientHeight = 0;

	m_bgdisp = 0;

	//ZeroMemory( m_bgname1, _MAX_PATH );
	//ZeroMemory( m_bgname2, _MAX_PATH );
	//m_bgmvu = 0.0f;
	//m_bgmvv = 0.0f;
	//m_bgcolor = 0x00ff0000;
	//m_bgcolor = RGB( 30, 65, 90 );


	//m_light_alpha = 180;
	//m_light_beta = -45;


	ZeroMemory( &m_viewport, sizeof( D3DVIEWPORT9 ) );

    D3DXMatrixIdentity( &m_matWorld );
    D3DXMatrixIdentity( &m_matView );
    D3DXMatrixIdentity( &m_matProjX );

	//m_preview_flag = 0;
	
	m_sellockflag = 0;
	_stprintf_s( m_strSellock, 1024, _T("") );

	ZeroMemory( m_bonearray, sizeof(int) * (MAXBONENUM + 1) );

	m_ik_cnt = 0;
	m_undo_cnt = 0;
	m_undo_init_flag = 1;
	
	m_control_key = 0;
	m_shift_key = 0;

	m_pbwnd = 0;

	D3DXMatrixIdentity( &s_inimat );

	m_lpos.x = 0;
	m_lpos.y = 0;
	m_rpos.x = 0;
	m_rpos.y = 0;
	m_mpos.x = 0;
	m_mpos.y = 0;
	m_clickoffset.x = 0;
	m_clickoffset.y = 0;
	m_clickbone.x = 0;
	m_clickbone.y = 0;

	m_clirect.top = 0;
	m_clirect.bottom = 0;
	m_clirect.left = 0;
	m_clirect.right = 0;

	m_drawtext = 0;
	m_createmotflag = 0;

	m_proj_near = 100.0f;
	m_proj_far = 50000.0f;

	m_ikcalccnt = 0;

	m_tbb_r = 1;
	m_tbb_t = 0;
	m_tbb_t2 = 0;
	m_tbb_s = 0;
	//m_tbb_g = 1;
	//m_tbb_l = 0;

	m_tbb_iti = 0;
	m_tbb_ikt = 0;
	m_tbb_col = 0;
	m_tbb_bone = 0;
	m_tbb_ei1 = 0;
	m_tbb_ei2 = 0;
	m_tbb_ei3 = 0;
	m_tbb_toon1 = 0;
	m_tbb_mo = 0;

	m_tbb_face = 1;
	m_tbb_line = 0;
	m_tbb_point = 0;
	m_tbb_ten2 = 0;

	m_tbb_za1 = 1;
	m_tbb_za2 = 0;
	m_tbb_za3 = 0;

	m_tbb_colP = 1;
	m_tbb_colT = 0;


	m_za_kind = ZA_1;

	m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

	m_g_disp = 1;

	m_plugin = 0;

	ZeroMemory( &m_vEyePt, sizeof( D3DXVECTOR3 ) );

	m_wheeldelta = 0.0f;

	m_movebone = 0;
	m_shiftbone = 0;
	m_scalebone = 0;

	//m_tempnameptr = 0;
}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{

	HANDLE hproc;
	hproc = GetCurrentProcess();
	SetProcessAffinityMask( hproc, 1 );


	InitCommonControls();

	CQuaternion2 tempq;
	tempq.InitTempQ();


	int ret;
	ERRORMES errormes;

//	g_cpuinfo.CheckCPU();
//	DbgOut( 
//		"CPU Vender: \"%s\"\n"
//		"Family: %d  Model: %d  Stepping ID: %d\n"
//		"Supported CPUID: %d\n"
//		"Supported MMX: %d\n"
//		"Supported SSE: %d\n"
//		"Supported SSE2: %d\n"
//		"Supported 3DNow!: %d\n"
//		"Supported Enhanced 3DNow!: %d\n"
//		, g_cpuinfo.vd.id
//		, g_cpuinfo.dwFamily, g_cpuinfo.dwModel, g_cpuinfo.dwSteppingId
//		, g_cpuinfo.bCPUID, g_cpuinfo.bMMX, g_cpuinfo.bSSE, g_cpuinfo.bSSE2, g_cpuinfo.b3DNow, g_cpuinfo.bE3DNow
//	);


	ret = g_coldlg.SetCustomColor( g_inidata->cc );
	_ASSERT( !ret );



//////////


	g_hStatus = CreateStatusWindow(
                WS_CHILD | WS_VISIBLE | CCS_BOTTOM,
                "status", // 第１区画に表示される文字列
                m_hWnd, //　親ウィンドウ
                ID_STATUSBAR);

	int sb_size[3];
	sb_size[0] = 100;
	sb_size[1] = 400;
	sb_size[2] = -1;
	SendMessage(g_hStatus, SB_SETPARTS, 3, (LPARAM)sb_size);
			
//	UpdateWindow( m_hWnd );
	

	//ret = OpenDbgFile();
	//if( ret ){
	//	errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
	//	errormes.type = MSGERR_APPMUSTEXIT;
	//	errormes.mesptr = "dbg.txt open error";
	//	SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
	//	return -1;
	//}
	

	//INITCOMMONCONTROLSEX cominfo;
	//cominfo.dwSize = sizeof( INITCOMMONCONTROLSEX );
	//cominfo.dwICC = ICC_PROGRESS_CLASS;
	//InitCommonControlsEx( &cominfo ); 	// for progress bar
	
//	InitCommonControls();


////////////
	if( !g_texbnk ){
		g_texbnk = new CTexBank( g_miplevels, g_mipfilter );
		if( !g_texbnk ){
			DbgOut( "OneTimeSceneInit : new CTexBank error !!!\n" );
			_ASSERT( 0 );

			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "TexBank alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );

			return -1;
		}
	}



////////////

	SetDefaultRenderState();

	//SetModuleDirは、d3dappに移動！！！！
	//ret = SetModuleDir();
	//if( ret ){
	//	errormes.errorcode = -1;
	//	errormes.type = MSGERR_APPMUSTEXIT;
	//	errormes.mesptr = "SetModuleDir error";
	//	SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
	//	return -1;
	//}

	//light
	CLightData* newlight;
	if( !g_lighthead ){
		newlight = new CLightData( &m_dirlightid );
		if( !newlight ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "CLightData alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			return -1;
		}

		D3DXVECTOR3 ldir( 0.0f, 0.0f, 1.0f );
		CVec3f difcol;
		//difcol.x = 1.0f; difcol.y = 1.0f; difcol.z = 1.0f;
		difcol.x = (float)GetRValue( g_inidata->dirlight_col ) / 255.0f;
		difcol.y = (float)GetGValue( g_inidata->dirlight_col ) / 255.0f;
		difcol.z = (float)GetBValue( g_inidata->dirlight_col ) / 255.0f;

		ret = newlight->SetDirectionalLight( ldir, difcol );
		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "OneTimeSceneInit : SetDirectionalLight error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			return -1;
		}

		g_lighthead = newlight;
	}

	/***
	g_motdlg = new CMotDlg();
	if( !g_motdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "MotDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	g_motdlg->Create( m_hWnd );
	***/

	m_itimodedlg = new CItiModeDlg();
	if( !m_itimodedlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ItiModeDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	m_itidlg = new CItiDlg( this );
	if( !m_itimodedlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ItiDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_itidlg->Create( m_hWnd );


	m_cameradlg = new CCameraDlg( m_hWnd );
	if( !m_cameradlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "CameraDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_cameradlg->Create( m_hWnd );
	m_cameradlg->SetParams( m_degxz, m_eye_y, m_camdist, m_targetpos, m_degy );
	int vpno;
	for( vpno = 0; vpno < 4; vpno++ ){
		m_cameradlg->m_viewparam[vpno] = g_inidata->viewparam[vpno];
	}

	m_sigeditwnd = new CSigEditWnd();
	if( !m_sigeditwnd ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "SigEditWnd alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	RECT rect0;
	rect0.left = 0;
	rect0.right = 300;
	rect0.top = 0;
	rect0.bottom = 500;
	m_sigeditwnd->Create(
	   m_hWnd,
	   rect0,
	   NULL,
	   WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
	   WS_EX_WINDOWEDGE
	);
	m_sigeditwnd->SetAppWnd( m_hWnd );

	m_optdlg = new COptDlg();
	if( !m_optdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OptDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_optdlg->m_bonedisp = g_inidata->opt_jointdisp;//!!!!!!!!
	m_optdlg->m_grounddisp = g_inidata->opt_gdisp;//!!!!!!!!
	m_optdlg->m_bmdisp = g_inidata->opt_bmdisp;
	m_optdlg->m_previewfps = g_inidata->opt_previewfps;
	m_optdlg->m_motstep = g_inidata->opt_motstep;
	m_optdlg->m_ikfk = g_inidata->opt_ikfk;
	m_optdlg->m_iktdisp = g_inidata->opt_iktdisp;

	m_optdlg->m_MouseL = g_inidata->opt_MouseL;
	m_optdlg->m_MouseR = g_inidata->opt_MouseR;
	m_optdlg->m_MouseW = g_inidata->opt_MouseW;


	if( g_inidata->opt_polydisp == 0 ){
		m_optdlg->m_polydisp = 1;//
	}else{
		m_optdlg->m_polydisp = 0;//
	}
	
	m_optdlg->m_jointsize = g_inidata->opt_jointsize;
	m_optdlg->m_jointalpha = g_inidata->opt_jointalpha;
	m_optdlg->m_undodlg = g_inidata->opt_undodlg;

	m_optdlg->m_UnselLine = g_inidata->opt_unselline;
	m_optdlg->m_SelLine = g_inidata->opt_selline;
	m_optdlg->m_UnselPoint = g_inidata->opt_unselpoint;
	m_optdlg->m_SelPoint = g_inidata->opt_selpoint;
	m_optdlg->m_PointSize = g_inidata->opt_pointsize;


	m_bonelinedlg = new CBoneLineDlg();
	if( !m_bonelinedlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "BoneLineDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_bonelinedlg->m_selcol = g_inidata->selectcol;
	m_bonelinedlg->m_unselcol = g_inidata->unselectcol;

	m_loadnamedlg = new CLoadNameDlg();
	if( !m_loadnamedlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadNameDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	m_parsdlg = new CParsDlg( PARSMODE_PARS, 45, 3000 );
	if( !m_parsdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ParsDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	

	m_bonedlg = new CCreateBoneDlg( this );
	if( !m_bonedlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "BoneDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_bonedlg->Create( m_hWnd );

	m_infscopedlg = new CInfScopeDlg( this );
	if( !m_infscopedlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "InfScopeDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_infscopedlg->Create( m_hWnd );

	m_paintdlg = new CPaintDlg( this );
	if( !m_paintdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "PaintDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_paintdlg->Create( m_hWnd );

	m_toon1dlg = new CToon1Dlg( this );
	if( !m_toon1dlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Toon1Dlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_toon1dlg->Create( m_hWnd );


	m_mcdlg = new CMotChangeDlg( this );
	if( !m_mcdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "MotChangeDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_mcdlg->Create( m_hWnd );


	m_wmafdlg = new CWriteMAFDlg( m_hWnd, m_loadnamedlg );
	if( !m_wmafdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "WriteMAFDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	m_appmenu = ::GetMenu( m_hWnd );

	m_dummymenu = LoadMenu( (HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE( IDR_DUMMYMENU ) );
	if( m_dummymenu == NULL ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadMenu error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	EnableMenuItem( m_appmenu, IDMENU_IKUNDO, MF_BYCOMMAND | MF_GRAYED );
	EnableMenuItem( m_appmenu, IDMENU_REVIKUNDO, MF_BYCOMMAND | MF_GRAYED );
	m_undo_init_flag = 1;


	ret = InitPluginMenu();
	_ASSERT( !ret );

	//animmenu
	HMENU motmenu;
    motmenu = GetSubMenu( m_appmenu, 8 );
    m_animmenu = GetSubMenu( motmenu, 4 );
	_ASSERT( m_animmenu );

	//EnableMenuItem( m_appmenu, 59002, MF_BYCOMMAND | MF_GRAYED );


// ground	
	ret = CreateGroundHandler();
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "CreateGroundHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	CMQOFile* mqofile;
	mqofile = new CMQOFile( m_hWnd );
	if( !mqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit MQOFile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	char moduledir[2048];
	int leng;
	ZeroMemory( moduledir, 2048 );
	leng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)moduledir, 1024 );
	_ASSERT( leng );
	char gfilename[2048];
	ZeroMemory( gfilename, 2048 );

	int chkleng;
	chkleng = leng + (int)strlen( s_groundmqo );
	if( chkleng >= 2048 ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit moduledir filename too long error !!!";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;	
	}

	strcpy_s( gfilename, 2048, moduledir );
	strcat_s( gfilename, 2048, s_groundmqo );

	D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot1( 0.0f, 0.0f, 0.0f );
	ret = mqofile->LoadMQOFile( 10.0f, gfilename, m_ground_th, m_ground_sh, m_ground_mh, 0, 0, BONETYPE_RDB2, 0, offsetpos, rot1 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ground mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		delete mqofile;
		return -1;
	}
	m_ground_sh->m_TLmode = TLMODE_D3D;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	ret = m_ground_sh->CreateBoneInfo( m_ground_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : ground : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ground CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		delete mqofile;
		return -1;
	}	

	delete mqofile;

///// selectの読み込み
	CMQOFile* selmqofile;
	selmqofile = new CMQOFile( m_hWnd );
	if( !selmqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit MQOFile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	chkleng = leng + (int)strlen( s_selectmqo );
	if( chkleng >= 2048 ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit moduledir filename too long error !!!";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;	
	}

	char selfilename[2048];
	strcpy_s( selfilename, 2048, moduledir );
	strcat_s( selfilename, 2048, s_selectmqo );

	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot2( 0.0f, 0.0f, 0.0f );
	ret = selmqofile->LoadMQOFile( 4.0f, selfilename, m_select_th, m_select_sh, m_select_mh, 0, 0, BONETYPE_RDB2, 0, offsetpos, rot2 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "select mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		delete selmqofile;
		return -1;
	}
	//m_select_sh->m_TLmode = TLMODE_D3D;//!!!  


	ret = m_select_sh->CreateBoneInfo( m_select_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : select : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "select CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		delete selmqofile;
		return -1;
	}	

	delete selmqofile;

////// iktargetの読み込み
	CMQOFile* iktmqofile;
	iktmqofile = new CMQOFile( m_hWnd );
	if( !iktmqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit MQOFile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	chkleng = leng + (int)strlen( s_iktmqo );
	if( chkleng >= 2048 ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit moduledir filename too long error !!!";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;	
	}

	char iktfilename[2048];
	strcpy_s( iktfilename, 2048, moduledir );
	strcat_s( iktfilename, 2048, s_iktmqo );

	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot3( 0.0f, 0.0f, 0.0f );
	ret = iktmqofile->LoadMQOFile( 0.3f, iktfilename, m_ikt_th, m_ikt_sh, m_ikt_mh, 0, 0, BONETYPE_RDB2, 0, offsetpos, rot3 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ikt mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		delete iktmqofile;
		return -1;
	}

	ret = m_ikt_sh->CreateBoneInfo( m_ikt_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : ikt : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ikt CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		delete iktmqofile;
		return -1;
	}	

	delete iktmqofile;

////// sphere0の読み込み
	CMQOFile* sphmqofile;
	sphmqofile = new CMQOFile( m_hWnd );
	if( !sphmqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit MQOFile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	chkleng = leng + (int)strlen( s_sphmqo );
	if( chkleng >= 2048 ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit moduledir filename too long error !!!";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;	
	}

	char sphfilename[2048];
	strcpy_s( sphfilename, 2048, moduledir );
	strcat_s( sphfilename, 2048, s_sphmqo );

	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	//D3DXVECTOR3 rot3( 0.0f, 0.0f, 0.0f );
	ret = sphmqofile->LoadMQOFile( 1.0f, sphfilename, m_sph_th, m_sph_sh, m_sph_mh, 0, 0, BONETYPE_RDB2, 0, offsetpos, rot3 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "sph mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		delete sphmqofile;
		return -1;
	}

	ret = m_sph_sh->CreateBoneInfo( m_sph_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : sph : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "sph CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		delete sphmqofile;
		return -1;
	}	

	delete sphmqofile;


///// bonemarkの読み込み
	CMQOFile* bmmqofile;
	bmmqofile = new CMQOFile( m_hWnd );
	if( !bmmqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit MQOFile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	chkleng = leng + (int)strlen( s_bonemarkmqo );
	if( chkleng >= 2048 ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit moduledir filename too long error !!!";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;	
	}

	char bmfilename[2048];
	strcpy_s( bmfilename, 2048, moduledir );
	strcat_s( bmfilename, 2048, s_bonemarkmqo );

	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot4( 0.0f, 0.0f, 0.0f );
	ret = bmmqofile->LoadMQOFile( 1.0f, bmfilename, m_bm_th, m_bm_sh, m_bm_mh, 0, 0, BONETYPE_RDB2, 0, offsetpos, rot4 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "bm mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		delete bmmqofile;
		return -1;
	}
	//m_bm_sh->m_TLmode = TLMODE_D3D;//!!!  


	ret = m_bm_sh->CreateBoneInfo( m_bm_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : bm : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "bm CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		delete bmmqofile;
		return -1;
	}	

	delete bmmqofile;

///// sq100の読み込み。
    //HMODULE hExe;
    HRSRC hRs;
    HGLOBAL hMem;
    char* lpData;
    DWORD dwResSize;

    //hExe = LoadLibrary(szExeName); 
    hRs = FindResource( NULL, "SQ100_2", "SIG" );
    if( hRs == NULL ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : sq100 : FindResource error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "sq100 FindResource error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
    }
    dwResSize = SizeofResource( NULL, hRs );
    hMem = LoadResource( NULL, hRs );
    lpData = (char*)LockResource( hMem );

	CSigFile* sqsigfile;
	sqsigfile = new CSigFile();
	if( !sqsigfile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit : sqsigfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
//DbgOut( "viewer : OneTime : sqload : buf %x, size %d\r\n", lpData, dwResSize );
	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot10( 0.0f, 0.0f, 0.0f );
	ret = sqsigfile->LoadSigFileFromBuf( lpData, (int)dwResSize, m_sq_th, m_sq_sh, m_sq_mh, 0, 1.0f, 0, offsetpos, rot10, 0 );
	if( ret ){
		delete sqsigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit : LoadSigFileFromBuf sq error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = m_sq_sh->CreateBoneInfo( m_sq_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit sq : CreateBoneInfo error !!!\n" );
		delete sqsigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit : CreateBoneInfo sq error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	delete sqsigfile;	


///// arrowの読み込み
	CSigFile* sigfile;
	sigfile = new CSigFile();
	if( !sigfile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit : sigfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );

	char arrowfilename[2048];
	strcpy_s( arrowfilename, 2048, moduledir );
	strcat_s( arrowfilename, 2048, "arrow.sig" );
	D3DXVECTOR3 rot11( 0.0f, 0.0f, 0.0f );
	ret = sigfile->LoadSigFile( arrowfilename, m_arrow_th, m_arrow_sh, m_arrow_mh, 0, 0.6f, 0, offsetpos, rot11, 0 );
	if( ret ){
		delete sigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit : LoadSigFile arrow error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = m_arrow_sh->CreateBoneInfo( m_arrow_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit arrow : CreateBoneInfo error !!!\n" );
		delete sigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit : CreateBoneInfo arrow error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	delete sigfile;	




	/***
	TRACKMOUSEEVENT mevent;
	mevent.cbSize = sizeof( TRACKMOUSEEVENT );
	mevent.dwFlags = TME_HOVER;
	mevent.dwHoverTime = 50;
	mevent.hwndTrack = m_hWnd;

	ret = TrackMouseEvent( &mevent );
	_ASSERT( ret );
	***/

	m_timerid = (int)SetTimer( m_hWnd, MOVEBONETIMER, TIMERINTERVAL, NULL );
	DbgOut( "viewer : OneTimeSceneInit : m_timerid %d\n", m_timerid );
	_ASSERT( m_timerid == MOVEBONETIMER );


	int isfirst = 1;

	if( g_inidata->camdlg_disp ){
		::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_CAMERA, (LPARAM)isfirst );
		::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_CAMERA, 0 );
	}


	if( g_inidata->sigdlg_disp ){
		::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_TREEEDIT, (LPARAM)isfirst );
		::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_TREEEDIT, 0 );
	}

	/***
	//ここでは、まだ、motdlgは出来ていないので、Restoreでする。
	if( g_inidata->motdlg_disp ){
		::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_MOTEDIT, (LPARAM)isfirst );
		::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_MOTEDIT, 0 );
	}
	***/
	


	// ここでは、まだ m_pd3dDevice はゼロ


	/***
	{
		//bmp file の　headerのチェック

		BITMAPFILEHEADER hdro, hdrx;
		BITMAPINFO bio, bix;
		HANDLE hfileo, hfilex;

		char filenameo[256];
		char filenamex[256];
		
		/////
		strcpy( filenameo, "c:\\hand\\output_256_188_0_otherappl.bmp" );
		hfileo = CreateFile( (LPCTSTR)filenameo, GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( hfileo == INVALID_HANDLE_VALUE ){
			_ASSERT( 0 );
			return 1;
		}	
		
		DWORD rleng, readleng;

		rleng = sizeof( BITMAPFILEHEADER );
		ReadFile( hfileo, (void*)(&hdro), rleng, &readleng, NULL );
		if( rleng != readleng ){
			_ASSERT( 0 );
			return 1;
		}

		rleng = sizeof( BITMAPINFO );
		ReadFile( hfileo, (void*)(&bio), rleng, &readleng, NULL );
		if( rleng != readleng ){
			_ASSERT( 0 );
			return 1;
		}
		
		/////
		strcpy( filenamex, "c:\\hand\\output_256_188_0.bmp" );

		hfilex = CreateFile( (LPCTSTR)filenamex, GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( hfilex == INVALID_HANDLE_VALUE ){
			_ASSERT( 0 );
			return 1;
		}	
		

		rleng = sizeof( BITMAPFILEHEADER );
		ReadFile( hfilex, (void*)(&hdrx), rleng, &readleng, NULL );
		if( rleng != readleng ){
			_ASSERT( 0 );
			return 1;
		}

		rleng = sizeof( BITMAPINFO );
		ReadFile( hfilex, (void*)(&bix), rleng, &readleng, NULL );
		if( rleng != readleng ){
			_ASSERT( 0 );
			return 1;
		}

		////////////
		////////////

		DbgOut( "bmpcheck : hdro : bfType %x, bfSize %d, bfOffBits %d, bfReserved1 %d, bfReserved2 %d\n",
			hdro.bfType, hdro.bfSize, hdro.bfOffBits, hdro.bfReserved1, hdro.bfReserved2 );
		
		DbgOut( "bmpcheck : hdrx : bfType %x, bfSize %d, bfOffBits %d, bfReserved1 %d, bfReserved2 %d\n",
			hdrx.bfType, hdrx.bfSize, hdrx.bfOffBits, hdrx.bfReserved1, hdrx.bfReserved2 );
		
		DbgOut( "\n" );



		DbgOut( "bmpcheck : bio : biSize %d, biWidth %d, biHeight %d, biPlanes %d, biBitCount %d, biCompression %d, biSizeImage %d, biXPelsPerMeter %d, biYPelsPerMeter %d, biClrUsed %d, biClrImportant %d\n",
			bio.bmiHeader.biSize, bio.bmiHeader.biWidth, bio.bmiHeader.biHeight, bio.bmiHeader.biPlanes, bio.bmiHeader.biBitCount, bio.bmiHeader.biCompression, bio.bmiHeader.biSizeImage, bio.bmiHeader.biXPelsPerMeter, bio.bmiHeader.biYPelsPerMeter, bio.bmiHeader.biClrUsed, bio.bmiHeader.biClrImportant );
		
		DbgOut( "bmpcheck : bio : biSize %d, biWidth %d, biHeight %d, biPlanes %d, biBitCount %d, biCompression %d, biSizeImage %d, biXPelsPerMeter %d, biYPelsPerMeter %d, biClrUsed %d, biClrImportant %d\n",
			bix.bmiHeader.biSize, bix.bmiHeader.biWidth, bix.bmiHeader.biHeight, bix.bmiHeader.biPlanes, bix.bmiHeader.biBitCount, bix.bmiHeader.biCompression, bix.bmiHeader.biSizeImage, bix.bmiHeader.biXPelsPerMeter, bix.bmiHeader.biYPelsPerMeter, bix.bmiHeader.biClrUsed, bix.bmiHeader.biClrImportant );


		//bi.bmiColors = NULL;
				


		////////////
		CloseHandle( hfileo );
		CloseHandle( hfilex );
	}
	***/


	/****
	{
		// little endian check

		//フラグのビットが、どのバイト位置に入るのかをチェック！！
		// short の場合、2バイト目に入るようだ！！

		unsigned char testdat1[10];
		unsigned char testdat2[10];
		short int* iptr1;
		short int* iptr2;
		unsigned char* ucptr1;
		unsigned char* ucptr2;

		ZeroMemory( testdat1, 10 );
		ZeroMemory( testdat2, 10 );


		iptr1 = (short int*)testdat1;
		iptr2 = (short int*)testdat2;

		//*iptr1 = 64;
		//*iptr2 = -64;

		*iptr1 = 16384;
		*iptr2 = -16384;


		ucptr1 = testdat1;
		ucptr2 = testdat2;

		int i;
		unsigned char* ucptr = ucptr1;
		DbgOut( "OneTimeSceneInit : check LE : testdat1 : " );
		for( i = 0; i < 4; i++ ){
			DbgOut( "%x ", *ucptr );
			ucptr++;
		}
		DbgOut( "\n\n" );

		ucptr = ucptr2;
		DbgOut( "OneTimeSceneInit : check LE : testdat2 : " );
		for( i = 0; i < 4; i++ ){
			DbgOut( "%x ", *ucptr );
			ucptr++;
		}
		DbgOut( "\n\n" );
	}
	****/



	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	DbgOut( "Performance Frequency %d\n", freq );


    return S_OK;
}





//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove( int srcseri, int tra_boneonly )
{
    // Slow things down for the REF device
    //if( m_d3dCaps.DeviceType == D3DDEVTYPE_REF )
    //    m_fElapsedTime = 0.05f;

	int ret;
	ERRORMES errormes;
	static int bmpflag = 0;

	GetKeyboardState( s_KeyBuffer );


//	if( s_KeyBuffer['1'] & 0x80 ){
//		::MessageBox( m_hWnd, "debug print", "debug", MB_OK );
//
//		CShdElem* iselem;
//		iselem = m_shandler->GetInfScopeElem();
//		_ASSERT( iselem );
//
//		DbgOut( "check!!! : infscopenum %d, alpha %f, transparent %d, serialno %d\r\n", iselem->scopenum, iselem->alpha, iselem->transparent, iselem->serialno );
//
//		int isno;
//		for( isno = 0; isno < iselem->scopenum; isno++ ){
//			CInfScope* curis;
//			curis = *( iselem->ppscope + isno );
//			DbgOut( "check!!! : scope %d : pm %x, pm2 %x, applychild %x, target %x, d3ddisp %x\r\n",
//				isno, curis->polymesh, curis->polymesh2, curis->applychild, curis->target, curis->d3ddisp );
//		}
//	}



	if( m_parsdlg->parsmode == PARSMODE_PARS ){
		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &m_matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = (float)m_parsdlg->orthosize;
		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &m_matProjX, vw, vh, m_proj_near, m_proj_far );
	}

	/***
	//toon1作り直しのテスト
	if( (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['T'] & 0x80) ){
		if( m_shandler ){

			::MessageBox( m_hWnd, "remake toon1 test", "テスト", MB_OK );

			m_shandler->m_shader = COL_TOON1;

			ret = m_shandler->CreateToon1Buffer( m_pd3dDevice, 1 );
			if( ret ){
				DbgOut( "viewer : FrameMove : Remake Toon1 : sh CreateToon1Buffer error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			m_tbb_colP = 0;
			m_tbb_colT = 1;
			CheckTBB();
		}
	}
	***/

	// Expand and shiftの仮機能
	/***
	if( (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['S'] & 0x80) ){
		int dlgret;
		CLDSigDlg dlg;
		dlgret = (int)dlg.DoModal();
		if( (dlgret == IDOK) && g_motdlg ){
			ret = g_motdlg->ExpandAndShiftMotion( (int)dlg.mult );
			if( ret ){
				DbgOut( "viewer : FrameMove : motdlg ExpandAndShiftMotion error !!!\n" );
				_ASSERT( 0 );
				return -1;
			}
		}
	}
	***/

	/***
	// Export BoneRateFile の仮機能
	if( (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['R'] & 0x80) && (s_KeyBuffer['A'] & 0x80) ){
		int dlgret;
		CLDSigDlg dlg;
		dlgret = (int)dlg.DoModal();
		if( dlgret == IDOK ){
			CBoneRateFile ratfile;
			ret = ratfile.WriteBoneRateFile( dlg.name, m_thandler, m_shandler );
			if( ret ){
				DbgOut( "viewer : FrameMove : ratfile WriteBoneRateFile error !!!\n" );
				_ASSERT( 0 );
				return -1;
			}
		}
	}
	***/

	/***
	if( s_KeyBuffer['C'] & 0x80 ){
		CHOOSECOLOR cc;
		COLORREF custcol[16];
		int colno;
		for( colno = 0; colno < 16; colno++ ){
			custcol[colno] = RGB( 255, 255, 255 );
		}

		COLORREF rgbresult = RGB( 255, 255, 255 );
		cc.lStructSize = sizeof( CHOOSECOLOR );
		cc.hwndOwner = m_hWnd;
		cc.hInstance = 0;
		cc.rgbResult = rgbresult;
		cc.lpCustColors = custcol;
		cc.Flags = CC_FULLOPEN;
		cc.lCustData = 0;
		cc.lpfnHook = 0;
		cc.lpTemplateName = 0;

		char resultstr[256];

		ChooseColor( &cc );
		sprintf( resultstr, "%x", cc.rgbResult );
		::MessageBox( m_hWnd, resultstr, "rgbResult", MB_OK );

		ChooseColor( &cc );
		sprintf( resultstr, "%x", cc.rgbResult );
		::MessageBox( m_hWnd, resultstr, "rgbResult", MB_OK );

	}
	***/


	//if( s_KeyBuffer['D'] & 0x80 ){
	//	DbgOut( "viewer : FrameMove : srcseri %d, tra_boneonly %d m_polydispmode %d : m_lbutton %d, m_movebone %d, m_rbutton %d, m_shiftbone %d\n",
	//		srcseri, tra_boneonly, m_polydispmode, m_lbutton, m_movebone, m_rbutton, m_shiftbone );
	//}


	//if( s_KeyBuffer['Q'] & 0x80 ){
	//	ChangeMotionStep();
	//}

	//if( s_KeyBuffer[VK_MENU] & 0x80 ){
	//	;
	//}
	
	/***
	static int dbgcnt = 0;
	static int doinverse = 0;
	if( (doinverse == 0) && ((dbgcnt % 10) == 0) && (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['I'] & 0x80) ){
		doinverse = 1;
		::MessageBox( m_hWnd, "モーションの変換を行います。", "確認ダイアログ", MB_OK );
		ret = g_motdlg->InverseMotionXZ();
		if( ret ){
			_ASSERT( 0 );
			return -1;
		}
		doinverse = 0;
	}
	dbgcnt++;
	***/

	/***
	//shift+Dキーを押して放したときにdebug出力。
	static int dbgflag = 0;
	int shiftD = 0;
	if( (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['D'] & 0x80) ){
		shiftD = 1;
	}
	if( (dbgflag == 1) && (shiftD == 0) ){
		int minzobjno = 0;
		D3DXVECTOR3 objloc[3];
		ZeroMemory( objloc, sizeof( D3DXVECTOR3 ) * 3 );
		m_shandler->CheckMinZObj( &minzobjno, objloc );

		char objname[1024];
		ZeroMemory( objname, 1024 );
		if( minzobjno > 0 ){
			strcpy( objname, ((*m_thandler)( minzobjno ))->name );
		}
		
		DbgOut( "viewer : CheckMinZObj %d %s ! %f %f %f ! %f %f %f ! %f %f %f\n",
			minzobjno, objname, 
			objloc[0].x, objloc[0].y, objloc[0].z,
			objloc[1].x, objloc[1].y, objloc[1].z,
			objloc[2].x, objloc[2].y, objloc[2].z
		);
	}
	dbgflag = shiftD;
	***/

	/***	
	//shift+Dキーを押して放したときにdebug出力。
	static int dbgflag = 0;
	int shiftD = 0;
	if( (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['D'] & 0x80) ){
		shiftD = 1;
	}
	if( (dbgflag == 1) && (shiftD == 0) ){
		m_shandler->CheckRestVert();
	}
	dbgflag = shiftD;
	//////////
	***/

	/***
	//shift+Dキーを押して放したときにdebug。
	static int dbgflag = 0;
	int shiftD = 0;
	if( (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['D'] & 0x80) ){
		shiftD = 1;
	}
	if( (dbgflag == 1) && (shiftD == 0) ){
		m_shandler->DebugPolymeshM();
		::MessageBox( m_hWnd, "変換されました。", "確認ダイアログ", MB_OK );
	}
	dbgflag = shiftD;
	***/

/***
	if( s_KeyBuffer['M'] & 0x80 ){
		static int s_motindex = 0;

		int motnum;
		motnum = g_motdlg->GetMotionNum();

		if( motnum > 1 ){
			int nextmotno;
			nextmotno = s_motindex + 1;
			if( nextmotno >= motnum ){
				nextmotno = 0;
			}
			s_motindex = nextmotno;

			char tempchar[256];
			sprintf( tempchar, "motindex %d", s_motindex );
			::MessageBox( m_hWnd, tempchar, "モーションセレクト", MB_OK );

			ret = g_motdlg->SelectCurrentMotion( s_motindex );
			_ASSERT( !ret );

		}
	}
***/

	/***
	if( m_shandler && (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['I'] & 0x80) && (s_KeyBuffer['E'] & 0x80) ){
		int dlgret;
		dlgret = ::MessageBox( m_hWnd, "対象コピーで壊れた影響度を修復しますか？", "確認", MB_OK ); 
		if( dlgret == IDOK ){
			ret = m_shandler->RepairInfElem( m_pd3dDevice );
			if( ret ){
				DbgOut( "viewer : FrameMove : RepairInfElem error !!!\n" );
				_ASSERT( 0 );
				return -1;
			}

			::MessageBox( m_hWnd, "修復が完了しました。", "完了", MB_OK );
		}
	}
	***/


	//Shift+F5 - Shift+F8, F5 - F8
	if( s_KeyBuffer[VK_SHIFT] & 0x80 ){
		if( s_KeyBuffer[VK_F5] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 0 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ５キーに保存しました。", "保存確認", MB_OK );
		}else if( s_KeyBuffer[VK_F6] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 1 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ６キーに保存しました。", "保存確認", MB_OK );
		}else if( s_KeyBuffer[VK_F7] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 2 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ７キーに保存しました。", "保存確認", MB_OK );
		}else if( s_KeyBuffer[VK_F8] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 3 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ８キーに保存しました。", "保存確認", MB_OK );
		}
	}else{
		int setflag = 0;
		if( s_KeyBuffer[VK_F5] & 0x80 ){
			m_cameradlg->SetViewParam2Current( 0 );
			setflag = 1;
		}else if( s_KeyBuffer[VK_F6] & 0x80 ){
			m_cameradlg->SetViewParam2Current( 1 );
			setflag = 1;
		}else if( s_KeyBuffer[VK_F7] & 0x80 ){
			m_cameradlg->SetViewParam2Current( 2 );
			setflag = 1;
		}else if( s_KeyBuffer[VK_F8] & 0x80 ){
			m_cameradlg->SetViewParam2Current( 3 );
			setflag = 1;
		}

		if( setflag ){
			m_degxz = m_cameradlg->m_degxz;
			m_degy = m_cameradlg->m_degy;
			m_eye_y = m_cameradlg->m_eye_y;
			m_camdist = m_cameradlg->m_camdist;
			m_targetpos = m_cameradlg->m_targetpos;

			if( g_motdlg ){
				g_motdlg->AxisSetRenderParams( m_degy, m_degxz, m_gband, m_ground_sh, m_ground_mh, m_arrow_sh, m_arrow_mh, m_pFont );
			}

		}

	}


	//shift+Dキーを押して放したときにm_drawtextをトグル。
	static int dbgDflag = 0;
	int shiftD = 0;
	if( (s_KeyBuffer[VK_SHIFT] & 0x80) && (s_KeyBuffer['D'] & 0x80) ){
		shiftD = 1;
	}
	if( (dbgDflag == 1) && (shiftD == 0) ){
		if( m_drawtext == 0 ){
			m_drawtext = 1;
		}else{
			m_drawtext = 0;
		}
	}
	dbgDflag = shiftD;


	// mouseでボーンをドラッグ中は、キーイン禁止!!!
	if( m_lbutton == 0 ){
		int keyflag;
		/***
		if( s_KeyBuffer[VK_LEFT] & 0x80 ){
			if( s_KeyBuffer[VK_SHIFT] & 0x80 )
				keyflag = ID_KEY_TARGET_LEFT;
			else
				keyflag = ID_KEY_LEFT;
		}else if( s_KeyBuffer[VK_RIGHT] & 0x80 ){
			if( s_KeyBuffer[VK_SHIFT] & 0x80 )
				keyflag = ID_KEY_TARGET_RIGHT;
			else
				keyflag = ID_KEY_RIGHT;
		}else if( s_KeyBuffer[VK_UP] & 0x80 ){
			if( s_KeyBuffer[VK_SHIFT] & 0x80 ){
				if( s_KeyBuffer['Z'] & 0x80 ){
					keyflag = ID_KEY_RAD2_UP;
				}else{
					keyflag = ID_KEY_ZOOMIN;
				}
			}else{
				keyflag = ID_KEY_UP;
			}
		}else if( s_KeyBuffer[VK_DOWN] & 0x80 ){
			if( s_KeyBuffer[VK_SHIFT] & 0x80 ){
				if( s_KeyBuffer['Z'] & 0x80 ){
					keyflag = ID_KEY_RAD2_DOWN;
				}else{
					keyflag = ID_KEY_ZOOMOUT;
				}
			}else{
				keyflag = ID_KEY_DOWN;
			}
		}else if( s_KeyBuffer[VK_F1] & 0x80 ){
		***/
		if( s_KeyBuffer[VK_F1] & 0x80 ){
			keyflag = ID_KEY_90LEFT;
		}else if( s_KeyBuffer[VK_F2] & 0x80 ){
			keyflag = ID_KEY_90RIGHT;
		}else if( s_KeyBuffer[VK_F3] & 0x80 ){
			keyflag = ID_KEY_90UP;
		}else if( s_KeyBuffer[VK_F4] & 0x80 ){
			keyflag = ID_KEY_90DOWN;
		}else if( s_KeyBuffer[VK_F9] & 0x80 ){
			keyflag = ID_KEY_SYMMX;
		}else{
			keyflag = 0;
		}
		ChangeViewParam( keyflag );
		
		switch( keyflag ){
		case ID_KEY_90LEFT:
		case ID_KEY_90RIGHT:
		case ID_KEY_90UP:
		case ID_KEY_90DOWN:
		case ID_KEY_SYMMX:
			::Sleep( 500 );
			break;
		default:
			break;
		}
	}

    D3DXMatrixIdentity( &m_matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

	
    // Rotate the camera about the y-axis
    //D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, m_eye_y, 0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );


	//！！！！　vEyePtを変更する際は、MoveBoneのnormalも変更する必要有り！！！！
	
	float radxz, rady;
	radxz = m_degxz * (float)DEG2PAI;
	if( (m_degy != 90.0f) && (m_degy != -90.0f) )
		rady = m_degy * (float)DEG2PAI;
	else if(m_degy == 90.0f)
		rady = 89.995f * (float)DEG2PAI;
	else if( m_degy == -90.0f)
		rady = -89.995f * (float)DEG2PAI;

    m_vEyePt.x = sinf(radxz) * m_camdist * cosf( rady ) + m_targetpos.x;
    m_vEyePt.y = m_eye_y + m_camdist * sinf( rady );
	m_vEyePt.z = -cosf(radxz) * m_camdist * cosf( rady ) + m_targetpos.z;


    //vEyePt.x = (sinf(m_rad) * m_camdist + m_targetpos.x) * cosf( m_rad2 );
    //vEyePt.y = m_eye_y + m_camdist * sinf( m_rad2 );
	//vEyePt.z = (-cosf(m_rad) * m_camdist + m_targetpos.z) * cosf( m_rad2 );

    // Set the app view matrix for normal viewing
	D3DXMatrixLookAtLH( &m_matView, &m_vEyePt, &m_targetpos, &vUpVec );
	
	m_lightdlg->ParamsToLight();


    //SetAppViewMatrix( m_matView );

	//m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, &m_matView );

	
	//D3DMATRIX matProj;
	//D3DUtil_SetIdentityMatrix( matProj );
    //D3DXMatrixIdentity( &m_matProj );
	

	////w = (float)1/tan(fov_horiz*0.5);  // 1/tan(x) == cot(x)
    ////h = (float)1/tan(fov_vert*0.5);   // 1/tan(x) == cot(x)

	//float    h, w, Q;

	if( m_parsdlg->parsmode == PARSMODE_PARS ){

		//w = (float)( 1.0f / tan( (float)m_parsdlg->parsfov * 0.5f * DEG2PAI ));
		//h = w;
		//Q = m_proj_far /(m_proj_far - m_proj_near);
	 
		//m_matProj._11 = w;
		//m_matProj._22 = h;
		//m_matProj._33 = Q;
		//m_matProj._34 = 1.0f;
		//m_matProj._43 = -Q * m_proj_near;//101.01
		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &m_matProjX, fov, aspect, m_proj_near, m_proj_far );
	}else{
		float vw, vh;
		vw = (float)m_parsdlg->orthosize;
		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &m_matProjX, vw, vh, m_proj_near, m_proj_far );

		//D3DXMatrixOrthoLH( &m_matProj, (float)m_parsdlg->orthosize, (float)m_parsdlg->orthosize, m_proj_near, m_proj_far );
	}

	//m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj );


    //D3DXMATRIX mat = matWorld * matView * matProj;
	
	/***
	if( s_KeyBuffer['T'] & 0x80 ){
		//DbgOut( "FrameMove : check time : trans %d, update %d, render %d\n", s_trans_time, s_update_time, s_render_time );
		DbgOut( "FrameMove : check time : trans high %d trans low %d, update high %d update low %d, render high %d render low %d\n", 
			s_trans_time_high, s_trans_time_low, s_update_time_high, s_update_time_low, s_render_time_high, s_render_time_low );
	}
	***/


    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;

	LARGE_INTEGER beftime, afttime;

	QueryPerformanceCounter( &beftime );

	if( g_inidata->opt_gdisp && (srcseri == 0) ){
		ret = m_ground_sh->ChkInView( m_ground_mh, m_matWorld, m_matView, m_matProjX );


		ret = m_ground_sh->TransformDispData( m_vEyePt, dwClipWidth, dwClipHeight, m_ground_mh, m_matWorld, m_matView, m_matProjX, PROJ_NORMAL, 0, m_smallgband );
		if( ret ){
			DbgOut( "viewer1.cpp : FrameMove : m_gournd_sh->TransformDispData error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "FrameMove ground TransformDispData error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	if( m_g_sh ){
		ret = m_g_sh->ChkInView( m_g_mh, m_matWorld, m_matView, m_matProjX );


		ret = m_g_sh->TransformDispData( m_vEyePt, dwClipWidth, dwClipHeight, m_g_mh, m_matWorld, m_matView, m_matProjX, PROJ_NORMAL, 0, m_smallgband );
		if( ret ){
			DbgOut( "viewer1.cpp : FrameMove : m_g_sh->TransformDispData error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "FrameMove g TransformDispData error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}


	if( g_motdlg ){
		if( (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) ){
			ret = g_motdlg->GetSelectedJointPos( &m_selectedno, &m_selectedpos );
			if( ret ){
				DbgOut( "viewer1.cpp : FrameMove : motdlg GetSelectedJointPos error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "FrameMove : motdlg GetSelectedJointPos error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		}else if( m_tbb_ikt == 1 ){
			//無効になっていないかチェックする
			if( m_selectedno > 0 ){
				if( (*m_shandler)( m_selectedno )->m_iktarget == 0 ){
					m_selectedno = 0;
				}
			}

			if( m_selectedno > 0 ){
				m_selectedpos = (*m_shandler)( m_selectedno )->m_iktargetpos;
			}
		}else if( (m_tbb_iti == 1) || ((m_tbb_bone == 1) && (m_bonedlg->m_ope == BONEOPE_POS)) ){
			m_selectedno = m_shiftpart;
			m_selectedpos = m_itidlg->m_worldpos;
		}else if( m_tbb_ei2 == 1 ){
			if( ((m_infscopedlg->m_object == ITIOBJ_PART) && (m_shiftface >= 0)) 
			|| ((m_infscopedlg->m_object == ITIOBJ_VERTEX) && (m_shifvertnum > 0)) ){
	
				m_selectedno = m_infscopedlg->m_isindex + 1;
			}else{
				m_selectedno = 0;
			}
			m_selectedpos = m_infscopedlg->m_worldpos;
		}else if( m_tbb_ei3 == 1){
			if( m_paintdlg->m_ope != PAINTOPE_BONE ){
				m_selectedno = 0;
			}else{
				ret = g_motdlg->GetSelectedJointPos( &m_selectedno, &m_selectedpos );
				_ASSERT( !ret );
			}
		}
	}


///////////////////
	if( m_selectedno > 0 ){
//selectの表示倍率を決定する。
		D3DXMATRIX view2;
		view2 = m_matView;
		view2._41 = 0.0f;
		view2._42 = 0.0f;
		view2._43 = 0.0f;
		D3DXMatrixInverse( &m_sq_mat, NULL, &view2 );
		m_sq_mat._41 = m_selectedpos.x;
		m_sq_mat._42 = m_selectedpos.y;
		m_sq_mat._43 = m_selectedpos.z;
		
		ret = m_sq_sh->ChkInView( m_sq_mh, m_sq_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : FrameMove : sq ChkInView error !!!\n" );
			_ASSERT( 0 );
		}
		ret = m_sq_sh->TransformDispData( m_vEyePt, dwClipWidth, dwClipHeight, m_sq_mh, m_sq_mat, m_matView, m_matProjX, PROJ_NORMAL, 0, m_smallgband );
		if( ret ){
			DbgOut( "viewer : FrameMove : sq TransformDispData error !!!\n" );
			_ASSERT( 0 );
		}



//int CShdHandler::GetScreenPos3( CMotHandler* lpmh, int srcseri, int vertno, int calcmode, int* scxptr, int* scyptr, float* sczptr, 
//							   DWORD dwClipWidth, DWORD dwClipHeight, D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj )

		int outflag = 1;
		int scx0, scy0;
		float scz0;
		ret = m_sq_sh->GetScreenPos3( m_sq_mh, 6, 0, 1, &scx0, &scy0, &scz0,
			dwClipWidth, dwClipHeight, m_sq_mat, m_matView, m_matProjX, outflag );
		if( ret ){
			DbgOut( "viewer : FrameMove : sq GetScreenPos3 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int scx1, scy1;
		float scz1;
		ret = m_sq_sh->GetScreenPos3( m_sq_mh, 7, 0, 1, &scx1, &scy1, &scz1,
			dwClipWidth, dwClipHeight, m_sq_mat, m_matView, m_matProjX, outflag );
		if( ret ){
			DbgOut( "viewer : FrameMove : sq GetScreenPos3 1 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		/***
		CShdElem* bone0 = (*m_sq_sh)( 6 );
		_ASSERT( bone0 );
		float scx0, scy0, scz0, wp0;	
		ret = bone0->GetBoneScreenPos( &scx0, &scy0, &scz0, &wp0 );
		if( ret ){
			DbgOut( "viewer : FrameMove : sq GetBonePos 0 error !!!\n" );
			_ASSERT( 0 );
		}

		CShdElem* bone1 = (*m_sq_sh)( 7 );
		_ASSERT( bone1 );
		float scx1, scy1, scz1, wp1;	
		ret = bone1->GetBoneScreenPos( &scx1, &scy1, &scz1, &wp1 );
		if( ret ){
			DbgOut( "viewer : FrameMove : sq GetBonePos 1 error !!!\n" );
			_ASSERT( 0 );
		}
		***/
		float mag, dist, selectscale;

		mag = (float)( (scx0 - scx1) * (scx0 - scx1) + (scy0 - scy1) * (scy0 - scy1) );
		if( mag != 0.0f ){
			dist = sqrtf( mag );
		}else{
			dist = 1.0f;//!!!!!!!
		}
		selectscale = 30.0f / dist;
		if( selectscale >= 1.0f )
			selectscale = 1.0f;

//////////////
		if( ((m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0)) 
			|| ((m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_BONE)) ){

			CShdElem* selelem;
			//CShdElem* markelem;
			//int markno;

			selelem = (*m_shandler)( m_selectedno );

			/***
			markelem = m_shandler->FindUpperJoint( selelem );
			if( markelem ){
				markno = markelem->serialno;
			}else{
				markno = m_selectedno;
			}
			***/


			CQuaternion selq;
			//ret = m_mhandler->GetTotalQOnFrame( m_shandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), m_selectedno, &selq );
			//ret = m_mhandler->GetTotalQOnFrame( m_shandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), markno, &selq );
			ret = m_mhandler->GetTotalQOnFrame( m_shandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), m_selectedno, &selq );
			if( ret ){
				DbgOut( "viewer : FrameMove : mh GetTotalQOnFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( m_tbb_za1 == 1 ){
				m_select_mat = selq.MakeRotMatX();
			}else if( m_tbb_za2 == 1 ){
				D3DXMatrixIdentity( &m_select_mat );
			}else{

				if( (m_movebone > 0) || (m_shiftbone > 0) || (m_scalebone > 0) ){
					//IK中は、マニピュレータの向きが変わらないようにする。
					m_select_mat = m_localq.MakeRotMatX();

				}else{
					CQuaternion iniq;
					ret = m_shandler->GetInitialBoneQ( m_selectedno, &iniq );
					if( ret ){
						DbgOut( "viewer : FrameMove : sh GetInitialBoneQ error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					CQuaternion za3selq;
					za3selq = selq * iniq;
					m_select_mat = za3selq.MakeRotMatX();
				}
			}
		}else{
			D3DXMatrixIdentity( &m_select_mat );
		}

		//m_select_mat._11 *= selectscale;
		//m_select_mat._22 *= selectscale;
		//m_select_mat._33 *= selectscale;

		D3DXMATRIX scalemat;
		D3DXMatrixIdentity( &scalemat );
		scalemat._11 = selectscale;
		scalemat._22 = selectscale;
		scalemat._33 = selectscale;

		m_select_mat = scalemat * m_select_mat;//!!!!!!!!!!!!!!

		m_select_mat._41 = m_selectedpos.x;
		m_select_mat._42 = m_selectedpos.y;
		m_select_mat._43 = m_selectedpos.z;

/***
		m_select_mat._11 = selectscale;
		m_select_mat._22 = selectscale;
		m_select_mat._33 = selectscale;

		m_select_mat._41 = m_selectedpos.x;
		m_select_mat._42 = m_selectedpos.y;
		m_select_mat._43 = m_selectedpos.z;
***/

		ret = m_select_sh->ChkInView( m_select_mh, m_select_mat, m_matView, m_matProjX );

		ret = m_select_sh->TransformDispData( m_vEyePt, dwClipWidth, dwClipHeight, m_select_mh, m_select_mat, m_matView, m_matProjX, PROJ_NORMAL, 0, m_smallgband );
		if( ret ){
			DbgOut( "viewer1.cpp : FrameMove : m_select_sh->TransformDispData error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "FrameMove select TransformDispData error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	if( m_ExistData && (m_movebone <= 0) && (m_shiftbone <= 0) && (m_scalebone <= 0) ){
		ret = m_mhandler->SetBoneAxisQ( m_shandler, -1, m_za_kind, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo() );
		if( ret ){
			DbgOut( "viewer : FrameMove : mh SetBoneAxisQ error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
	}

	g_bonepropdlg->UpdateParams( m_selectedno, m_za_kind );

	
	if( m_ExistData ){

		int lightflag;
		int* bonearray;

		if( srcseri == 0 ){
			lightflag = 1;
			bonearray = 0;
		}else{
			lightflag = 0;
			bonearray = m_bonearray;
		}

		ret = m_shandler->ChkInView( m_mhandler, m_matWorld, m_matView, m_matProjX );

		ret = m_shandler->TransformDispData( m_vEyePt, dwClipWidth, dwClipHeight, m_mhandler, m_matWorld, m_matView, m_matProjX, PROJ_NORMAL, 1, m_smallgband, bonearray, tra_boneonly );
		if( ret ){
			DbgOut( "viewer1.cpp : FrameMove : m_shandler->TransformDispData error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "FrameMove TransformDispData error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

	}

	QueryPerformanceCounter( &afttime );
	s_trans_time_low = afttime.LowPart - beftime.LowPart;
	s_trans_time_high = afttime.HighPart - beftime.HighPart;
	


	ret = m_bgdisp->SetBgSize( m_clientWidth, m_clientHeight );
	if( ret ){
		DbgOut( "FrameMove : m_bgdisp->SetBgSize error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "FrameMove m_bgdisp->SetBgSize error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = m_bgdisp->MoveTexUV( m_matView );
	_ASSERT( !ret );

	ret = m_bgdisp->UpdateVertexBuffer();
	_ASSERT( !ret );


	QueryPerformanceCounter( &beftime );

	//if( srcseri == 0 ){
	//	ret = UpdateVertexBuffer();
	//	if( ret ){
	//		DbgOut( "CMyD3DApplication : FrameMove : UpdateVretexBuffer : error !!!\n" );
	//		errormes.errorcode = -1;
	//		errormes.type = MSGERR_APPMUSTEXIT;
	//		errormes.mesptr = "FrameMove : UpdateVretexBuffer error";
	//		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
	//		_ASSERT( 0 );
	//		return -1;
	//	}
	//}

	QueryPerformanceCounter( &afttime );

	s_update_time_low = afttime.LowPart - beftime.LowPart;
	s_update_time_high = afttime.HighPart - beftime.HighPart;

	
////////////
	//QueryPerformanceCounter( &beftime );
	//if( srcseri == 0 ){
	//	ret = LockAndUnlock3DBuffer();
	//	if( ret ){
	//		DbgOut( "CMyD3DApplication : FrameMove : LockAndUnlock3DBuffer : error !!!\n" );
	//		errormes.errorcode = -1;
	//		errormes.type = MSGERR_APPMUSTEXIT;
	//		errormes.mesptr = "FrameMove : LockAndUnlock3DBuffer error";
	//		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
	//		_ASSERT( 0 );
	//		return -1;
	//	}
	//}
	//QueryPerformanceCounter( &afttime );
	//s_lock_time_low = afttime.LowPart - beftime.LowPart;
	//s_lock_time_high = afttime.HighPart - beftime.HighPart;


	if( g_motdlg ){
		g_motdlg->AxisSetRenderParams( m_degy, m_degxz, m_gband, m_ground_sh, m_ground_mh, m_arrow_sh, m_arrow_mh, m_pFont );
	}

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
    // Update the camera here rather than in FrameMove() so you can
    // move the camera even when the scene is paused
    //UpdateCamera();
	int ret = 0;
	ERRORMES errormes;


	LARGE_INTEGER beftime, afttime;
	QueryPerformanceCounter( &beftime );


	if( m_exit )
		return S_OK;

	D3DCOLOR clearcol;
	unsigned char colr, colg, colb;
	colr = GetRValue( g_inidata->bg_color );
	colg = GetGValue( g_inidata->bg_color );
	colb = GetBValue( g_inidata->bg_color );

	clearcol = D3DCOLOR_ARGB( 0, colr, colg, colb );

//DbgOut( "viewer : Render : clearcol %d %d %d\n", colr, colg, colb );

    // Clear the viewport
	HRESULT hres;

//DbgOut( "viewer : Render : bakcbuffer %x, pZSurf %x\r\n", pBackBuffer, m_pZSurf );


	hres = m_pd3dDevice->SetRenderTarget( 0, m_pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "viewer : render : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	hres = m_pd3dDevice->SetDepthStencilSurface( m_pZSurf );
	if( hres != D3D_OK ){
		DbgOut( "viewer : render : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	g_rendertarget = TARGET_MAIN;//!!!!!!!!


    hres = m_pd3dDevice->SetViewport(&m_viewport);
	if( hres != D3D_OK ){
		DbgOut( "viewer : Render : dev SetViewport error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = SetShaderConst();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	if( FAILED( m_pd3dDevice->BeginScene() ) )
        return S_OK;


	D3DRECT clrc;
	clrc.x1 = m_clirect.left;
	clrc.y1 = m_clirect.top;
	clrc.x2 = m_clirect.right;
	clrc.y2 = m_clirect.bottom;

	hres = m_pd3dDevice->Clear( 1, &clrc, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    //hres = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
	//hres = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 
                         //0x000000ff,
						 //0x00ffffff,
						 clearcol,
						 1.0f, 0L );
	//_ASSERT( hres == D3D_OK );
	if( hres != D3D_OK ){
		DbgOut( "viewer : Render : dev Clear error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	//Z奥は最後に描画。したいところだが、RenderBoneより後だと、Boneのスプライト表示がうまくいかない。
	ret = m_bgdisp->Render( m_pd3dDevice );
	if( ret ){
		DbgOut( "CD3DApplication : m_bgdisp->Render : error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Render m_bgdisp->Render error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
	} 


	static int sellockcnt = 0;
	if( m_writebmpflag == 0 ){

		if( m_drawtext ){
			// Output statistics
			m_pFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
			m_pFont->DrawText( 2, 20, D3DCOLOR_ARGB(255,255,255,0), m_strDeviceStats );
			//m_pFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,0,0,255), m_strFrameStats );
			//m_pFont->DrawText( 2, 20, D3DCOLOR_ARGB(255,0,0,255), m_strDeviceStats );
		}
		if( m_sellockflag && ((sellockcnt % 50) < 25) ){
			m_pFont->DrawText( 2, 40, D3DCOLOR_ARGB(255,255,255,0), m_strSellock );
		}
		if( m_vikflag && ((sellockcnt % 50) < 25) ){
			m_pFont->DrawText( 2, 60, D3DCOLOR_ARGB(255,255,255,0), "Under IK calculation !!!" );
		}
    }
	sellockcnt++;

	//char strdelta[1024];
	//sprintf( strdelta, "%s, delta %f", strselect[m_select_kind], m_select_delta );
	//m_pFont->DrawText( 2, 80, D3DCOLOR_ARGB(255,255,255,0), strdelta );



	//D3DXMATRIX matProjX;

	if( g_inidata->opt_gdisp ){

		ret = m_ground_sh->Render( m_pd3dDevice, 0, POLYDISP_ALL, m_ground_mh, 0, m_matWorld, m_matView, m_matProjX, m_vEyePt );
		if( ret ){
			DbgOut( "CMyD3DApplication : Render Ground: error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "Render ground Render error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	if( m_g_sh && m_g_disp ){
		ret = m_g_sh->Render( m_pd3dDevice, 0, POLYDISP_ALL, m_g_mh, 0, m_matWorld, m_matView, m_matProjX, m_vEyePt );
		if( ret ){
			DbgOut( "CMyD3DApplication : Render G: error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "Render g Render error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}


	//ret = m_sq_sh->Render( m_pd3dDevice, 0, POLYDISP_ALL, m_sq_mh, 0, m_sq_mat, m_matView, m_matProj );
	//if( ret ){
	//	DbgOut( "viewer : Render : sq Render error !!!\n" );
	//	_ASSERT( 0 );
	//}


	if( m_ExistData ){


//if( s_KeyBuffer['D'] & 0x80 ){
//	DbgOut( "viewer : Render : D key\n" );
//}

		if( m_tbb_face == 1){
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

			ret = m_shandler->Render( m_pd3dDevice, 0, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, m_matProjX, m_vEyePt );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render: error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render with alpha 0 error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}

			//半透明
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			ret = m_shandler->Render( m_pd3dDevice, 1, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, m_matProjX, m_vEyePt );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render with alpha: error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render with alpha 1 error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}

		}


		ret = m_shandler->SaveToDispTempDiffuse();
		if( ret ){
			DbgOut( "viewer : sh SaveToDispTempDiffuse error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		if( m_tbb_line == 1){
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			DWORD linecol;
			linecol = (0xFF << 24) | (GetRValue( m_optdlg->m_UnselLine ) << 16) | (GetGValue( m_optdlg->m_UnselLine ) << 8) | GetBValue( m_optdlg->m_UnselLine );
			ret = m_shandler->FillDispColor( -1, linecol );
			if( ret ){
				DbgOut( "viewer : sh FillDispColor : line error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = m_shandler->Render( m_pd3dDevice, 0, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, m_matProjX, m_vEyePt );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render: error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render with alpha 0 error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}

			//半透明
			ret = m_shandler->Render( m_pd3dDevice, 1, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, m_matProjX, m_vEyePt );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render with alpha: error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render with alpha 1 error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}


			/***
			if( (m_tbb_iti == 1) && (m_shiftpart > 0) ){ 
				DWORD sellinecol;
				sellinecol = (0xFF << 24) | (GetRValue( m_optdlg->m_SelLine ) << 16) | (GetGValue( m_optdlg->m_SelLine ) << 8) | GetBValue( m_optdlg->m_SelLine );
				CShdElem* selelem;
				selelem = (*m_shandler)( m_shiftpart );

				ret = selelem->FillDispColor( sellinecol );
				if( ret ){
					DbgOut( "viewer : selelem FillDispColor : selline error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				if( m_itidlg->m_object == ITIOBJ_PART ){
					ret = selelem->Render( m_pd3dDevice, m_shandler, 0 );
					if( ret ){
						DbgOut( "CMyD3DApplication : Render: error !!!\n" );
						errormes.errorcode = -1;
						errormes.type = MSGERR_APPMUSTEXIT;
						errormes.mesptr = "Render with alpha 0 error";
						SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
						_ASSERT( 0 );
						return -1;
					}
				}else if( m_itidlg->m_object == ITIOBJ_FACE ){

					ret = selelem->Copy2IndexBufferNotCulling();
					_ASSERT( !ret );

					ret = selelem->Render( m_pd3dDevice, m_shiftface );
					if( ret ){
						DbgOut( "CMyD3DApplication : Render: error !!!\n" );
						errormes.errorcode = -1;
						errormes.type = MSGERR_APPMUSTEXIT;
						errormes.mesptr = "Render with alpha 0 error";
						SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
						_ASSERT( 0 );
						return -1;
					}

					ret = selelem->Copy2IndexBuffer( 0 );
					_ASSERT( !ret );
				}
			}
			***/
			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

		}

		if( (m_tbb_point == 1) || (m_tbb_ten2 == 1) ){
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_POINT );
			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			if( m_tbb_ten2 == 1 ){
				m_shandler->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_ALWAYS );
			}

			DWORD pointcol;
			pointcol = (0xFF << 24) | (GetRValue( m_optdlg->m_UnselPoint ) << 16) | (GetGValue( m_optdlg->m_UnselPoint ) << 8) | GetBValue( m_optdlg->m_UnselPoint );
			ret = m_shandler->FillDispColor( -1, pointcol );
			if( ret ){
				DbgOut( "viewer : sh FillDispColor : point error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			float PointSize = (float)m_optdlg->m_PointSize;
			m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&PointSize));

			ret = m_shandler->Render( m_pd3dDevice, 0, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, m_matProjX, m_vEyePt );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render: error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render with alpha 0 error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
			//半透明
			ret = m_shandler->Render( m_pd3dDevice, 1, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, m_matProjX, m_vEyePt );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render with alpha: error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render with alpha 1 error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}

			/***
			if( (m_tbb_iti == 1) && (m_shiftpart > 0) ){ 
				DWORD selpointcol;
				selpointcol = (0xFF << 24) | (GetRValue( m_optdlg->m_SelPoint ) << 16) | (GetGValue( m_optdlg->m_SelPoint ) << 8) | GetBValue( m_optdlg->m_SelPoint );
				CShdElem* selelem;
				selelem = (*m_shandler)( m_shiftpart );

				ret = selelem->FillDispColor( selpointcol );
				if( ret ){
					DbgOut( "viewer : selelem FillDispColor : selline error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				if( m_itidlg->m_object == ITIOBJ_PART ){
					ret = selelem->Render( m_pd3dDevice, m_shandler, 0 );
					if( ret ){
						DbgOut( "CMyD3DApplication : Render: error !!!\n" );
						errormes.errorcode = -1;
						errormes.type = MSGERR_APPMUSTEXIT;
						errormes.mesptr = "Render with alpha 0 error";
						SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
						_ASSERT( 0 );
						return -1;
					}
				}else if( m_itidlg->m_object == ITIOBJ_FACE ){
					ret = selelem->Copy2IndexBufferNotCulling();
					_ASSERT( !ret );

					ret = selelem->Render( m_pd3dDevice, m_shiftface );
					if( ret ){
						DbgOut( "CMyD3DApplication : Render: error !!!\n" );
						errormes.errorcode = -1;
						errormes.type = MSGERR_APPMUSTEXIT;
						errormes.mesptr = "Render with alpha 0 error";
						SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
						_ASSERT( 0 );
						return -1;
					}

					ret = selelem->Copy2IndexBuffer( 0 );
					_ASSERT( !ret );

				}
			}else if( (m_tbb_ei3 == 1) && (m_shiftpart > 0) ){


			}
			***/

			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

			if( m_tbb_ten2 == 1 ){
				m_shandler->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
			}
		}

		ret = m_shandler->RestoreDispTempDiffuse();
		if( ret ){
			DbgOut( "viewer : Render : sh RestoreDispTempDiffuse error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}

	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );


	// iktarget
	if( m_ExistData && (g_inidata->opt_iktdisp != 0) ){

		m_ikt_sh->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_ALWAYS );

		DWORD dwClipWidth  = m_clientWidth/2;
		DWORD dwClipHeight = m_clientHeight/2;


		ret = m_ikt_sh->TransformAndRenderIKT( m_vEyePt, m_pd3dDevice, dwClipWidth, dwClipHeight, m_ikt_mh,
			m_shandler, m_mhandler,
			m_bm_sh, 
			g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), 
			m_matWorld, m_matView, m_matProjX, m_matProjX, m_smallgband );
		if( ret ){
			DbgOut( "CMyD3DApplication : Render ikt: error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "Render ikt error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


	}


/***
	//スプライトは最後じゃないと、うまくいかない（なぞ）
	if( m_ExistData ){
		ret = m_shandler->RenderBone( m_pd3dDevice, m_d3dxsprite, (float)m_optdlg->m_jointsize * 0.01f, m_optdlg->m_jointalpha,
			m_clientWidth / 2, m_clientHeight / 2, m_mhandler,
			m_matWorld, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderBone : error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RenderBone error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		if( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope != PAINTOPE_BONE) && (m_paintdlg->m_targetdisp > 0) ){
			DWORD dwClipWidth  = m_clientWidth/2;
			DWORD dwClipHeight = m_clientHeight/2;

			ret = m_shandler->RenderSelVert( m_mhandler, m_pd3dDevice, m_d3dxsprite, m_paintdlg->m_targetdisp, m_paintdlg->m_targetvert[0], dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
			if( ret ){
				DbgOut( "CMyD3DApplication : RenderSelVert : error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "RenderSelVert error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		}

	}
***/

	if( m_ExistData ){

		hres = m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
		g_renderstate[ D3DRS_ZFUNC ] = D3DCMP_ALWAYS;

		if( g_inidata->opt_bmdisp ){
			D3DXMATRIX iniw;
			D3DXMatrixIdentity( &iniw );

			m_bm_sh->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_ALWAYS );

			int selboneno;
			if( (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) )
				selboneno = m_selectedno;
			else if( m_tbb_ei2 == 1 )
				selboneno = m_infscopedlg->m_selbone;
			else if( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope != PAINTOPE_BONE) )
				selboneno = m_paintdlg->m_selbone;
			else if( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_BONE) )
				selboneno = m_selectedno;

			ret = m_bm_sh->RenderBoneMark( m_pd3dDevice, m_shandler, m_mhandler, iniw, m_matView, m_matProjX, m_bonelinedlg->m_selcol, m_bonelinedlg->m_unselcol, selboneno );
			if( ret ){
				DbgOut( "viewer : Render : bm : sh RenderBoneMark error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render bonemark Render error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}			
		}

		if( (g_inidata->opt_jointdisp) && (m_selectedno > 0) && (m_tbb_bone == 0 || ((m_tbb_bone == 1) && (m_bonedlg->m_ope == BONEOPE_POS))) ){

			m_select_sh->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_ALWAYS );

			ret = m_select_sh->Render( m_pd3dDevice, 0, POLYDISP_ALL, m_select_mh, 0, m_select_mat, m_matView, m_matProjX, m_vEyePt );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render Select: error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "Render select Render error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		}
		
		hres = m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
		g_renderstate[ D3DRS_ZFUNC ] = D3DCMP_LESSEQUAL;

	}

	//スプライトは最後じゃないと、うまくいかない（なぞ）
	if( m_ExistData ){
		ret = m_shandler->RenderBone( m_pd3dDevice, m_d3dxsprite, (float)m_optdlg->m_jointsize * 0.01f, m_optdlg->m_jointalpha,
			m_clientWidth / 2, m_clientHeight / 2, m_mhandler,
			m_matWorld, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderBone : error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RenderBone error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		if( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope != PAINTOPE_BONE) && (m_paintdlg->m_targetdisp > 0) ){
			DWORD dwClipWidth  = m_clientWidth/2;
			DWORD dwClipHeight = m_clientHeight/2;

			ret = m_shandler->RenderSelVert( m_mhandler, m_pd3dDevice, m_d3dxsprite, m_paintdlg->m_targetdisp, m_paintdlg->m_targetvert[0], dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
			if( ret ){
				DbgOut( "CMyD3DApplication : RenderSelVert : error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "RenderSelVert error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		}

	}


    // End the scene.
    m_pd3dDevice->EndScene();


	if( m_writebmpflag == 0 ){
		m_pd3dDevice->Present( NULL, NULL, m_hWnd, NULL );
		g_rendercnt = 0;
	}
	QueryPerformanceCounter( &afttime );

	s_render_time_low = afttime.LowPart - beftime.LowPart;
	s_render_time_high = afttime.HighPart - beftime.HighPart;

///////// sleep


	DWORD curtime, difftime;
	//DWORD sleepms;
	int drawflag = 0;

	DWORD wait_preview;
	wait_preview = (DWORD)(1000.0f / (float)g_inidata->opt_previewfps);
	DWORD wait_normal;
	wait_normal = (DWORD)(1000.0f / 60.0f);
	//wait_normal = 1000 / 60;

	if( m_force_render == 0 ){

		if( m_timerkind == 0 ){
			timeBeginPeriod( 1 );
			curtime = timeGetTime();
			timeEndPeriod( 1 );
							
			difftime = curtime - m_rendertime;
			//m_rendertime = curtime;

			if( m_preview_flag == 0 ){
				if( difftime > wait_normal )
					difftime = wait_normal - 1;//ＣＰＵをあけるために、1msスリープする

				while( difftime <= wait_normal ){

					timeBeginPeriod( 1 );
					::MsgWaitForMultipleObjects( 0, NULL, FALSE, wait_normal - difftime, 0 );
					curtime = timeGetTime();
					timeEndPeriod( 1 );
									
					difftime = curtime - m_rendertime;
				}
			}else{
				while( difftime <= wait_preview ){
					timeBeginPeriod( 1 );
					::MsgWaitForMultipleObjects( 0, NULL, FALSE, wait_preview - difftime, 0 );
					curtime = timeGetTime();
					timeEndPeriod( 1 );
									
					difftime = curtime - m_rendertime;
				}
			}

			timeBeginPeriod( 1 );
			curtime = timeGetTime();
			timeEndPeriod( 1 );
			difftime = curtime - m_rendertime;
			m_rendertime = curtime;

			m_fFPS = 1000.0f / (float)difftime;

		}else{
			DWORD curmsec;
			LONGLONG diffcnt;
			QueryPerformanceCounter( &m_curcnt );

			//HighPart の　diff は、０と仮定！！！
			if( m_curcnt.LowPart > m_rendercnt.LowPart )
				diffcnt = m_curcnt.LowPart - m_rendercnt.LowPart;
			else
				diffcnt = m_rendercnt.LowPart;

			curmsec = (DWORD)((double)diffcnt / (double)m_freq.LowPart * 1000.0f );



			if( m_preview_flag == 0 ){

				if( curmsec > wait_normal )
					curmsec = wait_normal - 1;//ＣＰＵをあけるために、1msスリープする

				while( wait_normal >= curmsec ){
					timeBeginPeriod( 1 );
					::MsgWaitForMultipleObjects( 0, NULL, FALSE, wait_normal - curmsec, 0 );
					timeEndPeriod( 1 );

					QueryPerformanceCounter( &m_curcnt );
					//HighPart の　diff は、０と仮定！！！
					if( m_curcnt.LowPart > m_rendercnt.LowPart )
						diffcnt = m_curcnt.LowPart - m_rendercnt.LowPart;
					else
						diffcnt = m_rendercnt.LowPart;
					curmsec = (DWORD)((double)diffcnt / (double)m_freq.LowPart * 1000.0f );
				}

			}else{
				while( wait_preview >= curmsec ){
					timeBeginPeriod( 1 );
					::MsgWaitForMultipleObjects( 0, NULL, FALSE, wait_preview - curmsec, 0 );
					timeEndPeriod( 1 );

					QueryPerformanceCounter( &m_curcnt );
					//HighPart の　diff は、０と仮定！！！
					if( m_curcnt.LowPart > m_rendercnt.LowPart )
						diffcnt = m_curcnt.LowPart - m_rendercnt.LowPart;
					else
						diffcnt = m_rendercnt.LowPart;
					curmsec = (DWORD)((double)diffcnt / (double)m_freq.LowPart * 1000.0f );
				}
			}


////////////
			QueryPerformanceCounter( &m_curcnt );

			//HighPart の　diff は、０と仮定！！！
			if( m_curcnt.LowPart > m_rendercnt.LowPart )
				diffcnt = m_curcnt.LowPart - m_rendercnt.LowPart;
			else
				diffcnt = m_rendercnt.LowPart;			
			
			if( diffcnt != 0 )
				m_fFPS = (float)( 1.0 / ((double)diffcnt / (double)m_freq.LowPart) );
			else
				m_fFPS = 0.0f;
			
			//m_fFPS = (float)m_freq.LowPart / (float)diffcnt;


			m_rendercnt = m_curcnt;
		}
	}


    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UpdateCamera()
// Desc: 
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::UpdateCamera()
{
}


//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
    //HRESULT hr;
	//int ret;
    // Set up the fonts and textures
    m_pFont->InitDeviceObjects( m_pd3dDevice );
    m_pFontSmall->InitDeviceObjects( m_pd3dDevice );

	

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
//----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects( int needcalc, int resetflag )
{
	HRESULT hr;
	int ret;
	ERRORMES errormes;

	float maxvertex;

	maxvertex = m_d3dCaps.MaxVertexW;
	if( maxvertex == 0.0f ){
		m_gband.maxrhw = 1e8;
	}else{
		m_gband.maxrhw = 1.0f / maxvertex;
	}
	
	//RECT clirect;
	GetClientRect( m_hWnd, &m_clirect );
	m_clientWidth = m_clirect.right - m_clirect.left;
	m_clientHeight = m_clirect.bottom - m_clirect.top;
	
	m_gband.left = min( 0.0f, m_d3dCaps.GuardBandLeft );
	m_gband.top = min( 0.0f, m_d3dCaps.GuardBandTop );
	m_gband.right = max( (float)m_clientWidth, m_d3dCaps.GuardBandRight );
	m_gband.bottom = max( (float)m_clientHeight, m_d3dCaps.GuardBandBottom );
	
	
	float extmult = 1.20f;
	m_smallgband.maxrhw = m_gband.maxrhw;

	m_smallgband.left = max( m_gband.left, -extmult * m_clientWidth );
	m_smallgband.top = max( m_gband.top, -extmult * m_clientHeight );
	m_smallgband.right = min( m_gband.right, extmult * m_clientWidth );
	m_smallgband.bottom = min( m_gband.bottom, extmult * m_clientHeight );



	DWORD dwShaderFlags = 0;
	#ifdef DEBUG_VS
		dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
	#endif
	#ifdef DEBUG_PS
		dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
	#endif
	//dwShaderFlags |= D3DXSHADER_NO_PRESHADER;
	//if( !g_bEnablePreshader )
	//	dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

/***
		hr = D3DXCreateEffectFromResource( m_pd3dDevice, NULL, MAKEINTRESOURCE(IDR_RCDATA2), NULL, NULL, dwShaderFlags, NULL, &g_pEffect, NULL );
		if( hr != D3D_OK ){
			::MessageBox( NULL, "エフェクトファイルにエラーがあります。\n終了します。", "確認", MB_OK );
			DbgOut( "\n\nEffect File error %x !!!\n", hr );
			_ASSERT( 0 );
			g_useGPU = 0;
			return -1;
		}
***/

	if( !g_pEffect ){
		if( (g_chkVS >= 20) || (g_chkVS == 0) ){
			hr = D3DXCreateEffectFromResource( m_pd3dDevice, NULL, MAKEINTRESOURCE(IDR_RCDATA1), NULL, NULL, dwShaderFlags, NULL, &g_pEffect, NULL );
			if( hr != D3D_OK ){
				::MessageBox( NULL, "エフェクトファイルにエラーがあります。\n終了します。", "確認", MB_OK );
				DbgOut( "\n\nEffect File error %x !!!\n", hr );
				_ASSERT( 0 );
				g_useGPU = 0;
				return -1;
			}
		}else if( g_chkVS >= 11 ){
			hr = D3DXCreateEffectFromResource( m_pd3dDevice, NULL, MAKEINTRESOURCE(IDR_RCDATA2), NULL, NULL, dwShaderFlags, NULL, &g_pEffect, NULL );
			if( hr != D3D_OK ){
				::MessageBox( NULL, "エフェクトファイルにエラーがあります。\n終了します。", "確認", MB_OK );
				DbgOut( "\n\nEffect File error %x !!!\n", hr );
				_ASSERT( 0 );
				g_useGPU = 0;
				return -1;
			}
		}

		ret = GetShaderHandle();
		if( ret ){
			DbgOut( "viewer : GetShaderHandle error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{
		g_pEffect->OnResetDevice();
	}

	if( m_createmotflag == 0 ){

	//effect
		//if( g_useGPU == 1 ){

		//}


		g_motdlg = new CMotDlg( m_pd3dDevice, m_d3dpp, g_inidata->iklevelope );
		if( !g_motdlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MotDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		g_motdlg->Create( m_hWnd );

		
		m_lightdlg = new CLightDlg( m_pd3dDevice, this, g_inidata, g_lighthead );
		if( !m_lightdlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LightDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		m_lightdlg->Create( m_hWnd );
		//m_lightdlg->ShowDlg( SW_HIDE );

		m_setcoldlg = new CSetColDlg( m_pd3dDevice, this, g_inidata, g_lighthead );
		if( !m_setcoldlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "SetColDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		m_setcoldlg->Create( m_hWnd );

		m_materialdlg = new CMaterialDlg( m_pd3dDevice, this, g_inidata, g_lighthead );
		if( !m_materialdlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MaterialDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		m_materialdlg->Create( m_hWnd );


		g_bonepropdlg = new CBonePropDlg( this );
		if( !g_bonepropdlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "BonePropDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		g_bonepropdlg->Create( m_hWnd );


		m_createmotflag = 1;


		int isfirst = 1;
		if( g_inidata->motdlg_disp ){
			::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_MOTEDIT, (LPARAM)isfirst );
			::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_MOTEDIT, 0 );
		}

		s_rmenumain = new CRMenuMain( g_motdlg->m_motparamdlg );
		if( !s_rmenumain || !g_motdlg->m_motparamdlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RMenuMain alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;		
		}

		ret = s_rmenumain->Create( m_hWnd );
		if( ret ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RMenuMain Create error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;		
		}


	}else{
		ret = g_motdlg->AxisCreateSwapChain( m_pd3dDevice, m_d3dpp );
		if( ret ){
			DbgOut( "viewer : restore : axisdlg CreateSwapChain error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RestoreDeviceObjects axisdlg CreateSwapChain error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_lightdlg->CreateSwapChain( m_pd3dDevice );
		if( ret ){
			DbgOut( "viewer : restore : lightdlg CreateSwapChain error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RestoreDeviceObjects lightdlg CreateSwapChain error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_setcoldlg->CreateSwapChain( m_pd3dDevice );
		if( ret ){
			DbgOut( "viewer : restore : setcoldlg CreateSwapChain error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RestoreDeviceObjects setcoldlg CreateSwapChain error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_materialdlg->CreateSwapChain( m_pd3dDevice );
		if( ret ){
			DbgOut( "viewer : restore : materialdlg CreateSwapChain error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RestoreDeviceObjects materialdlg CreateSwapChain error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

	}


	/***
	m_gband.left = 0.0f;
	m_gband.top = 0.0f;
	m_gband.right = (float)dwWidth;
	m_gband.bottom = (float)dwHeight;
	***/

	int cliptlverts = 0;
	cliptlverts = m_d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_CLIPTLVERTS;

	DbgOut( "Restore : Caps : GuardBand : left %f, top %f, right %f, bottom %f\n",
		m_gband.left, m_gband.top, m_gband.right, m_gband.bottom );

	DbgOut( "Restore : Caps : !! Small !! GuardBand  : left %f, top %f, right %f, bottom %f\n",
		m_smallgband.left, m_smallgband.top, m_smallgband.right, m_smallgband.bottom );


	DbgOut( "Restore : Caps : CLIPTLVERTS : %d, MaxVertexW %f\n", 
		cliptlverts, maxvertex );


	DbgOut( "Restore : Caps : MaxPrimitiveCount %d\n", m_d3dCaps.MaxPrimitiveCount );
	DbgOut( "Restore : Caps : MaxVertexIndex %d\n", m_d3dCaps.MaxVertexIndex );


	/*** viewport ***/
    m_viewport.X = 0;
    m_viewport.Y = 0;
    m_viewport.Height = m_clientHeight;
    m_viewport.Width =  m_clientWidth;
    m_viewport.MaxZ = 1.0f;
    m_viewport.MinZ = 0.0f;

	/***
    hr = m_pd3dDevice->SetViewport(&m_viewport);
	if( hr != D3D_OK ){
		DbgOut( "CMyD3DApplication : SetViewport error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects SetViewport error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	***/

	if( m_d3dxsprite ){
		//m_d3dxsprite->OnLostDevice();
		m_d3dxsprite->OnResetDevice();
	}else{

		hr = D3DXCreateSprite( m_pd3dDevice, &m_d3dxsprite );
		if( hr != D3D_OK ){
			DbgOut( "CMyD3DApplication : D3DXCreateSprite error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RestoreDeviceObjects CreateSprite error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	//if( m_ExistData ){
	//	ret = RestoreHandler();
	//	_ASSERT( !ret );
	//}
	ret = RestoreGroundHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreGroundHnadler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects RestoreGourndHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = RestoreGHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreGHnadler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects RestoreGHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = RestoreHandler( needcalc );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHnadler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	m_bgdisp = new CBgDisp2( m_pd3dDevice, g_inidata->bg_tex1, g_inidata->bg_tex2, g_inidata->bg_mvu, g_inidata->bg_mvv, g_inidata->bg_color, 0, g_proj_far );
	if( !m_bgdisp ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : m_bgdisp alloc error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects m_bgdisp alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	
	ret = m_ground_sh->RestoreExtLineColor();
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : RestoreExtLineColor error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects RestoreExtLineColor error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	
	ret = m_select_sh->RestoreExtLineColor();
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : RestoreExtLineColor error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : RestoreExtLineColor error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = m_bm_sh->RestoreExtLineColor();
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : bm : RestoreExtLineColor error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects bm : RestoreExtLineColor error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


///// select partno 取得
	ret = m_select_th->GetPartNoByName( "obj_X", &m_sel_xno );
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "obj_Y", &m_sel_yno );
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "obj_Z", &m_sel_zno );
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "obj_CENTER", &m_sel_cenno );
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	if( (m_sel_xno <= 0) || (m_sel_yno <= 0) || (m_sel_zno <= 0) || (m_sel_cenno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : Object Name error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : Object Name error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


//DbgOut( "viewer : RestoreDeviceObject : bg_color %d %d %d\n", GetRValue( g_inidata->bg_color ), GetGValue( g_inidata->bg_color ), GetBValue( g_inidata->bg_color ) );

	//ret = m_bgdisp->SetUVAnime( 0.005f, 0.0f );
	//_ASSERT( !ret );


    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    m_pd3dDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_ANISOTROPIC );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_ANISOTROPIC );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
    //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	if( g_cop0 != D3DTOP_MODULATE ){
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_cop0 = D3DTOP_MODULATE;
	}
	if( g_aop0 != D3DTOP_MODULATE ){
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );//!!!!
		g_aop0 = D3DTOP_MODULATE;
	}

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );


		// ver1.0.2.2で、WRAPモードに変更
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );


    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
	//m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
    //m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );
	m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	

	m_pd3dDevice->SetRenderState( D3DRS_WRAP0, 0 );//!!!!!! texruleにより変更する。
    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );//<-----------------------
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,        D3DZB_TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,        0x40404040 );
	m_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS,  FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );

//	if( m_tbb_face ){
//		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
//	}else{
//		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
//	}
	

	SetLight( 0 );
	SetBoneDisp();

	m_pFont->InitDeviceObjects( m_pd3dDevice );//!!!!!!!!!!
	m_pFontSmall->InitDeviceObjects( m_pd3dDevice );//!!!!!!!!

	m_pFont->RestoreDeviceObjects();
	m_pFontSmall->RestoreDeviceObjects();


	if( resetflag == 1 ){
		SetDefaultRenderState();//!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

    return S_OK;
}

int CMyD3DApplication::RestoreHandler( int needcalc, int dispwarnflag )
{
	int ret = 0;

	if( m_shandler ){
		ret = m_shandler->CreateTexture( m_pd3dDevice, m_hWnd, 1, g_miplevels, g_mipfilter, dispwarnflag );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_shandler->CreateTexture error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_shandler->SetMikoAnchorApply( m_thandler );
		if( ret ){
			DbgOut( "viewer : RestoreHandler : sh SetMikoAnchorApply error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_shandler->CreateDispObj( m_pd3dDevice, m_mhandler, needcalc );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_shandler->CreateDispObj error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ret = m_shandler->SetTexTransparent();
		_ASSERT( !ret );
		ret = m_shandler->CreateMorphObj( m_pd3dDevice, m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_shandler->CreateMorphObj error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ret = m_shandler->SetDispFlag( 1 );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_shandler->SetDispFlag error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}

		/***
		ret = m_shandler->SetBSphereData( m_mhandler, -1, 0, 0 );
		if( ret ){
			DbgOut( "viewer : RestoreHandler : shandler SetBSphereData error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		***/
		//COL_TOON1
		if( m_shandler->m_shader == COL_TOON1 ){
			m_tbb_colT = 0;
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		}

	}

	return ret;
}
int CMyD3DApplication::RestoreExtHandler( int needcalc )
{
	int ret = 0;

	if( m_ext_sh ){
		int dispwarnflag = 0;
		ret = m_ext_sh->CreateTexture( m_pd3dDevice, m_hWnd, 1, g_miplevels, g_mipfilter, dispwarnflag );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreExtHandler : m_ext_sh->CreateTexture error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_ext_sh->SetMikoAnchorApply( m_ext_th );
		if( ret ){
			DbgOut( "viewer : RestoreExtHandler : sh SetMikoAnchorApply error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_ext_sh->CreateDispObj( m_pd3dDevice, m_ext_mh, needcalc );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreExtHandler : m_shandler->CreateDispObj error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ret = m_ext_sh->SetTexTransparent();
		_ASSERT( !ret );
		ret = m_ext_sh->CreateMorphObj( m_pd3dDevice, m_ext_mh );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreExtHandler : m_shandler->CreateMorphObj error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ret = m_ext_sh->SetDispFlag( 1 );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreExtHandler : m_shandler->SetDispFlag error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		//COL_TOON1
		if( m_ext_sh->m_shader == COL_TOON1 ){
			ret = m_ext_sh->CreateToon1Buffer( m_pd3dDevice );
			if( ret ){
				DbgOut( "viewer : RestoreExtHandler : IDTBB_COLT : COL_TOON1 : sh CreateToon1Buffer error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return ret;
}



int CMyD3DApplication::RestoreGHandler()
{
	int ret = 0;
//// g
	if( m_g_sh ){
		ret += m_g_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_g_sh->CreateTexture error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ret += m_g_sh->CreateDispObj( m_pd3dDevice, m_g_mh, 1 );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_g_sh->CreateDispObj error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ret += m_g_sh->SetTexTransparent();
		_ASSERT( !ret );
		ret += m_g_sh->CreateMorphObj( m_pd3dDevice, m_g_mh );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_g_sh->CreateMorphObj error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		ret += m_g_sh->SetDispFlag( 1 );
		if( ret ){
			DbgOut( "CMyD3DApplication : RestoreHandler : m_g_sh->SetDispFlag error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		/***
		ret = m_g_sh->SetBSphereData( m_g_mh, -1, 0, 0 );
		if( ret ){
			DbgOut( "viewer : RestoreGroundHandler : g SetBSphereData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		***/
	}
	return 0;

}
int CMyD3DApplication::RestoreGroundHandler()
{

	int ret = 0;

	ret += m_ground_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ground_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ground_sh->CreateDispObj( m_pd3dDevice, m_ground_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ground_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ground_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_ground_sh->CreateMorphObj( m_pd3dDevice, m_ground_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ground_sh->CreateMorphObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ground_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ground_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	/***
	ret = m_ground_sh->SetBSphereData( m_ground_mh, -1, 0, 0 );
	if( ret ){
		DbgOut( "viewer : RestoreGroundHandler : ground SetBSphereData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/

//// arrrow
	ret += m_arrow_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_arrow_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_arrow_sh->CreateDispObj( m_pd3dDevice, m_arrow_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_arrow_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_arrow_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_arrow_sh->CreateMorphObj( m_pd3dDevice, m_arrow_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_arrow_sh->CreateMorphObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_arrow_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_arrow_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	/***
	ret = m_arrow_sh->SetBSphereData( m_arrow_mh, -1, 0, 0 );
	if( ret ){
		DbgOut( "viewer : RestoreGroundHandler arrow : SetBSphereData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/

///// select
	ret += m_select_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_select_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_select_sh->CreateDispObj( m_pd3dDevice, m_select_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_select_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_select_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_select_sh->CreateMorphObj( m_pd3dDevice, m_select_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_select_sh->CreateMorphObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_select_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_select_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	/***
	ret = m_select_sh->SetBSphereData( m_select_mh, -1, 0, 0 );
	if( ret ){
		DbgOut( "viewer : RestoreGroundHandler select : SetBSphereData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/
	SetSelectColor( 0 );//!!!!!!!!!!!!!!!

///// ikt
	ret += m_ikt_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ikt_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ikt_sh->CreateDispObj( m_pd3dDevice, m_ikt_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ikt_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ikt_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_ikt_sh->CreateMorphObj( m_pd3dDevice, m_ikt_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ikt_sh->CreateMorphObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ikt_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ikt_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	/***
	ret = m_ikt_sh->SetBSphereData( m_ikt_mh, -1, 0, 0 );
	if( ret ){
		DbgOut( "viewer : RestoreGroundHandler ikt : SetBSphereData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/
///// sph
	ret += m_sph_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_sph_sh->CreateDispObj( m_pd3dDevice, m_sph_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_sph_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_sph_sh->CreateMorphObj( m_pd3dDevice, m_sph_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->CreateMorphObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_sph_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	/***
	ret = m_sph_sh->SetBSphereData( m_sph_mh, -1, 0, 0 );
	if( ret ){
		DbgOut( "viewer : RestoreGroundHandler sph : SetBSphereData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/

///// bm
	ret += m_bm_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_bm_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_bm_sh->CreateDispObj( m_pd3dDevice, m_bm_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_bm_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_bm_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_bm_sh->CreateMorphObj( m_pd3dDevice, m_bm_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_bm_sh->CreateMorphObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_bm_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_bm_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	/***
	ret = m_bm_sh->SetBSphereData( m_bm_mh, -1, 0, 0 );
	if( ret ){
		DbgOut( "viewer : RestoreGroundHandler bm : SetBSphereData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/
/// sq
	ret += m_sq_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sq_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_sq_sh->CreateDispObj( m_pd3dDevice, m_sq_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sq_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_sq_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_sq_sh->CreateMorphObj( m_pd3dDevice, m_sq_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sq_sh->CreateMorphObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_sq_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sq_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	/***
	ret = m_sq_sh->SetBSphereData( m_sq_mh, -1, 0, 0 );
	if( ret ){
		DbgOut( "viewer : RestoreGroundHandler sq : SetBSphereData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/

	return ret;
}
//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects( int resetflag )
{
	int ret;
	ERRORMES errormes;

	if( g_pEffect ){
		g_pEffect->OnLostDevice();
	}

	if( m_d3dxsprite ){
		m_d3dxsprite->OnLostDevice();
	}

	if( m_pFont != NULL )
		m_pFont->InvalidateDeviceObjects();
	if( m_pFontSmall != NULL )
		m_pFontSmall->InvalidateDeviceObjects();


	//if( m_ExistData ){
	//	ret = InvalidateHandler();
	//	_ASSERT( !ret );
	//}
	ret = InvalidateHandler( resetflag );
	if( ret ){
		DbgOut( "InvalidateDeviceObjects : InvalidateHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "InvalidateDeviceObjects InvalidateHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
	}

	ret = InvalidateGroundHandler( resetflag );
	if( ret ){
		DbgOut( "InvalidateDeviceObjects : InvalidateGroundHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "InvalidateDeviceObjects InvalidateGroundHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
	}


	ret = InvalidateGHandler( resetflag );
	if( ret ){
		DbgOut( "InvalidateDeviceObjects : InvalidateGHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "InvalidateDeviceObjects InvalidateGHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
	}


	if( m_bgdisp ){
		delete m_bgdisp;
		m_bgdisp = 0;
	}

	if( g_motdlg ){
		g_motdlg->AxisDestroySwapChain();
	}
	if( m_lightdlg ){
		m_lightdlg->DestroySwapChain();
	}
	if( m_setcoldlg ){
		m_setcoldlg->DestroySwapChain();
	}
	if( m_materialdlg ){
		m_materialdlg->DestroySwapChain();
	}


    return S_OK;
}

int CMyD3DApplication::InvalidateHandler( int resetflag )
{
	int ret = 0;
	if( m_shandler ){
		ret += m_shandler->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_shandler->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}

	if( m_loadnamedlg ){
		*(m_loadnamedlg->m_signame) = 0;
		*(m_loadnamedlg->m_quaname) = 0;
		*(m_loadnamedlg->m_im2name) = 0;
		*(m_loadnamedlg->m_bonname) = 0;
		*(m_loadnamedlg->m_moaname) = 0;
	}

	return ret;
}
int CMyD3DApplication::InvalidateGHandler( int resetflag )
{
	int ret = 0;
/// g
	if( m_g_sh ){
		ret += m_g_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_g_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}
	return ret;
}

int CMyD3DApplication::InvalidateGroundHandler( int resetflag )
{
	int ret = 0;
	if( m_ground_sh ){
		ret += m_ground_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_ground_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}

/// arrow
	if( m_arrow_sh ){
		ret += m_arrow_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_arrow_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}

/// select
	if( m_select_sh ){
		ret += m_select_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_select_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}

/// ikt
	if( m_ikt_sh ){
		ret += m_ikt_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_ikt_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}
/// sph
	if( m_sph_sh ){
		ret += m_sph_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_sph_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}
/// bm
	if( m_bm_sh ){
		ret += m_bm_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_bm_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}
/// sq
	if( m_sq_sh ){
		ret += m_sq_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_sq_sh->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}
	return ret;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
	if( m_pFont != NULL )
		m_pFont->DeleteDeviceObjects();
	if( m_pFontSmall != NULL )
		m_pFontSmall->DeleteDeviceObjects();

    return S_OK;
}





//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{

	CQuaternion2 tempq;
	tempq.DestroyTempQ();

	if( m_plugin ){
		delete [] m_plugin;
		m_plugin = 0;
	}

	if( g_motdlg && g_inidata ){
		g_inidata->iklevelope = g_motdlg->GetIKLevelOpe();//!!!!!!
	}

	if( s_rmenumain ){
		s_rmenumain->Destroy();
		delete s_rmenumain;
	}

	if( g_motdlg )
		g_motdlg->ShowDlg( SW_HIDE );
	if( m_lightdlg )
		m_lightdlg->ShowDlg( SW_HIDE );
	if( m_setcoldlg )
		m_setcoldlg->ShowDlg( SW_HIDE );
	if( m_infscopedlg )
		m_infscopedlg->ShowDlg( SW_HIDE );
	if( m_paintdlg )
		m_paintdlg->ShowDlg( SW_HIDE );

	int ret;
	ret = KillTimer( m_hWnd, m_timerid );
	_ASSERT( ret );


	RECT newrect;
	ret = GetWindowRect( m_hWnd, &newrect );
	if( ret != 0 ){
		g_inidata->main_posx = newrect.left;
		g_inidata->main_posy = newrect.top;
		g_inidata->main_width = newrect.right - newrect.left;
		g_inidata->main_height = newrect.bottom - newrect.top;
	}else{
		g_inidata->main_posx = 0;
		g_inidata->main_posy = 0;
		g_inidata->main_width = 400;
		g_inidata->main_height = 400;
		DbgOut( "viewer : FinalCleanUp : GetWindowRect error : SetDefault ini data !!!\r\n" );
	}

	if( m_InitFlag ){
		UpdateDialogIniData( ID_MSG_FROM_CAMERADLG );
		UpdateDialogIniData( ID_MSG_FROM_SIGDLG );
		UpdateDialogIniData( ID_MSG_FROM_MOTDLG );

		ret = g_inidata->SaveParams( "rdb2.ini" );//!!!!!!!!!!!!!!!!!
		_ASSERT( !ret );
	}


    SAFE_DELETE( m_pFont );
    SAFE_DELETE( m_pFontSmall );

	if( m_itimodedlg ){
		//m_itimodedlg->DestroyWindow();
		delete m_itimodedlg;
		m_itimodedlg = 0;
	}

	if( m_itidlg ){
		m_itidlg->ShowDlg( SW_HIDE );
		m_itidlg->DestroyWindow();
		delete m_itidlg;
		m_itidlg = 0;
	}


	if( m_cameradlg ){
		m_cameradlg->DestroyWindow();
		delete m_cameradlg;
		m_cameradlg = 0;
	}

	if( m_bonedlg ){
		m_bonedlg->ShowDlg( SW_HIDE );
		m_bonedlg->DestroyWindow();
		delete m_bonedlg;
		m_bonedlg = 0;
	}

	if( m_infscopedlg ){
		m_infscopedlg->ShowDlg( SW_HIDE );
		m_infscopedlg->DestroyWindow();
		delete m_infscopedlg;
		m_infscopedlg = 0;
	}

	if( m_paintdlg ){
		m_paintdlg->ShowDlg( SW_HIDE );
		m_paintdlg->DestroyWindow();
		delete m_paintdlg;
		m_paintdlg = 0;
	}

	if( m_toon1dlg ){
		m_toon1dlg->ShowDlg( SW_HIDE );
		m_toon1dlg->DestroyWindow();
		delete m_toon1dlg;
		m_toon1dlg = 0;
	}

	if( m_mcdlg ){
		m_mcdlg->ShowDlg( SW_HIDE );
		m_mcdlg->DestroyWindow();
		delete m_mcdlg;
		m_mcdlg = 0;
	}

	if( m_wmafdlg ){
		delete m_wmafdlg;
		m_wmafdlg = 0;
	}

	if( m_sigeditwnd ){
		m_sigeditwnd->DestroyWindow();
		delete m_sigeditwnd;
		m_sigeditwnd = 0;
	}

	if( m_bonelinedlg ){
		delete m_bonelinedlg;
		m_bonelinedlg = 0;
	}

	if( m_loadnamedlg ){
		delete m_loadnamedlg;
		m_loadnamedlg = 0;
	}

	if( g_motdlg ){
		g_motdlg->OnStop();
		//Sleep( 500 );
		g_motdlg->DestroyWindow();
		delete g_motdlg;
		g_motdlg = 0;
	}
	if( m_lightdlg ){
		m_lightdlg->DestroyWindow();
		delete m_lightdlg;
		m_lightdlg = 0;
	}
	if( m_setcoldlg ){
		m_setcoldlg->ShowDlg( SW_HIDE );//!!!!!!pmlのセーブ
		m_setcoldlg->DestroyWindow();
		delete m_setcoldlg;
		m_setcoldlg = 0;
	}
	if( m_materialdlg ){
		m_materialdlg->ShowDlg( SW_HIDE );
		m_materialdlg->DestroyWindow();
		delete m_materialdlg;
		m_materialdlg = 0;
	}
	if( g_bonepropdlg ){
		g_bonepropdlg->ShowDlg( SW_HIDE );
		g_bonepropdlg->DestroyWindow();
		delete g_bonepropdlg;
		g_bonepropdlg = 0;
	}


	if( m_optdlg ){
		delete m_optdlg;
		m_optdlg = 0;
	}

	if( m_parsdlg ){
		delete m_parsdlg;
		m_parsdlg = 0;
	}


	FreeLexBuf();

	if( m_dummymenu != NULL ){
		DestroyMenu( m_dummymenu );
		m_dummymenu = NULL;
	}
	//if( m_appmenu != NULL ){
	//	DestroyMenu( m_appmenu );
	//	m_appmenu = NULL;
	//}

	//::DestroyMenu( m_appmenu );

	//DbgOut( "MyD3DApplication : FinalCleanup : DestroyHandler\n" );


	if( m_d3dxsprite != NULL ){
		m_d3dxsprite->Release();
		m_d3dxsprite = NULL;
	}


	if( m_bgdisp ){
		delete m_bgdisp;
		m_bgdisp = 0;
	}

	CLightData* curlight = g_lighthead;
	while( curlight ){
		CLightData* nextlight;
		nextlight = curlight->nextlight;

		delete curlight;
		curlight = nextlight;
	}
	g_lighthead = 0;


	DestroyHandler();
	DestroyGroundHandler();
	DestroyGHandler();
	DestroyExtHandler();

	if( g_texbnk ){
		delete g_texbnk;
		g_texbnk = 0;
	}

	DestroyProgressBar();

	CloseDbgFile();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    // Make sure device can do ONE:ONE alphablending
    if( 0 == ( pCaps->SrcBlendCaps & D3DPBLENDCAPS_ONE ) )
        return E_FAIL;
    if( 0 == ( pCaps->DestBlendCaps & D3DPBLENDCAPS_ONE ) )
        return E_FAIL;

    // Make sure HW TnL devices can do point sprites
    if( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ) ||
        (dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING ) )
    {
        if( pCaps->MaxPointSize <= 0.0f )
            return E_FAIL;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam )
{
	ERRORMES errormes;

   static LPTOOLTIPTEXT lptip;

   //char tempmes[256];

	if( uMsg == WM_NOTIFY ){
		switch (((LPNMHDR)lParam)->code) {
		case TTN_NEEDTEXT:
			lptip = (LPTOOLTIPTEXT)lParam;
			lptip->hinst = (HINSTANCE)GetModuleHandle(NULL);
			
			//sprintf( tempmes, "WM_NOTIFY : %d", lptip->hdr.idFrom );	
			//::MessageBox( NULL, tempmes, "check", MB_OK );

			switch (lptip->hdr.idFrom) {
			case IDTBB_R:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_R);
				break;
			case IDTBB_T:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_T);
				break;
			case IDTBB_T2:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_T2);
				break;
			case IDTBB_S:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_S);
				break;
			case IDTBB_ITI:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_ITI);
				break;
			case IDTBB_IKT:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_IKT);
				break;
			case IDTBB_COL:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_COL);
				break;
			case IDTBB_BONE:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_BONE);
				break;
			case IDTBB_EI1:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_EI1);
				break;
			case IDTBB_EI2:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_EI2);
				break;
			case IDTBB_EI3:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_EI3);
				break;
			case IDTBB_TOON1:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_TOON1);
				break;
			case IDTBB_MO:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_MO);
				break;
			case IDTBB_FACE:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_FACE);
				break;
			case IDTBB_LINE:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_LINE);
				break;
			case IDTBB_POINT:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_POINT);
				break;
			case IDTBB_TEN2:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_TEN2);
				break;
			case IDTBB_ZA1:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_ZA1);
				break;
			case IDTBB_ZA2:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_ZA2);
				break;
			case IDTBB_ZA3:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_ZA3);
				break;
			case IDTBB_COLP:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_COLP);
				break;
			case IDTBB_COLT:
				lptip->lpszText = MAKEINTRESOURCE(IDS_TB_COLT);
				break;

			default:
				break;
			}
			break;
		default:
			break;
		}
 	
	}else if( uMsg == WM_CLOSE ){
		int type = (int)lParam;
		if( (type == MSGERR_APPMUSTEXIT) || (m_menucnt == 0) ){
	        Cleanup3DEnvironment();
            DestroyMenu( GetMenu(hWnd) );
            DestroyWindow( hWnd );
            PostQuitMessage(0);
            return 0;
		}else{
			return 0;
		}
	}else if( uMsg == WM_ACTIVATEAPP ){
		BOOL kind;
		kind = (BOOL)wParam;

		DbgOut( "viewer : WM_ACTIVATEAPP %d %d\n", kind, m_bReady );
		if( kind == 0 ){
			if( m_screensave == 1 ){

				InvalidateDeviceObjects();
				m_pd3dDevice->Reset( &m_d3dpp );
				m_bReady = 0;
				DbgOut( "viewer : Invalidate for screensave\n" );
			}
		}else{
			if( m_screensave == 1 ){
				Resize3DEnvironment();
				
				SetLight( 0 );
				SetBoneDisp();

				m_bReady = 1;
				m_screensave = 0;
				DbgOut( "viewer : return from screensave\n" );
			}
		}

	/***
	}else if( uMsg == WM_ACTIVATE ){
		WORD activatekind;
		activatekind = LOWORD( wParam );

		if( activatekind == WA_ACTIVE ){
			DbgOut( "viewer : WA_ACTIVATE %d %d\n", m_bActive, m_bReady );
		}else if( activatekind == WA_INACTIVE ){
			DbgOut( "viewer : WA_INACTIVATE %d %d\n", m_bActive, m_bReady );
		}
		
		//if( m_bActive && m_bReady ){
		//	DbgOut( "MyD3DApplication : MsgProc : WM_ACTIVATE\n" );
		//	Render3DEnvironment();
		//}
	***/
	}else if( uMsg == WM_LBUTTONDOWN ){
		
//		if( g_motdlg ){
//			BOOL dummyb;
//			g_motdlg->OnLButtonDown( uMsg, wParam, lParam, dummyb );
//		}

		//wparam MK_LBUTTON
		//lparam low word x
		//lparam high word y
		WORD kind = (WORD)wParam;
		int ret;

		SetCapture( m_hWnd );

		WORD xPos, yPos;
		xPos = GET_X_LPARAM(lParam); 
		yPos = GET_Y_LPARAM(lParam); 
		
		if( (kind & MK_LBUTTON) && (m_rbutton == 1) && (m_vikflag == 0) ){
			m_lrbutton = 1;
			
			m_lbutton = 0;
			m_movebone = 0;
			m_rbutton = 0;
			m_shiftbone = 0;
			m_scalebone = 0;
		
			m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

//DbgOut( "LRBUTTON DOWN!!!\r\n" );


			m_lrpos.x = xPos;
			m_lrpos.y = yPos;

//		}else if( (m_lbutton == 0) && (kind & MK_LBUTTON) && (m_rbutton == 0) && (m_preview_flag == 0) && (m_vikflag == 0) && 
//			!( (m_tbb_bone == 1) && (m_bonedlg->m_ope == BONEOPE_CREATE) ) ){


		}else if( (m_lbutton == 0) && (kind & MK_LBUTTON) && (m_rbutton == 0) && (m_vikflag == 0) && 
			!( (m_tbb_bone == 1) && (m_bonedlg->m_ope == BONEOPE_CREATE) ) ){


			m_lbutton = 1;

			m_lpos.x = xPos;
			m_lpos.y = yPos;

			//DbgOut( "viewer : WM_LBUTTONDOWN %d %d\n", xPos, yPos );

			int targetjoint = 0;

			if( m_preview_flag == 0 ){
				int pickno = 0;

				if( (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_col == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) ){
					pickno = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
					targetjoint = pickno;
				}else if( m_tbb_ikt == 1 ){
					pickno = PickIKT( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone );
				}else if( m_tbb_iti == 1 ){

					switch( m_itidlg->m_object ){
					case ITIOBJ_VERTEX:
						PickVert( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, &m_shiftpart, m_shifvert, &m_shifvertnum, 1 );
						pickno = m_shiftpart;
						break;
					case ITIOBJ_FACE:
						PickFace( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, &m_shiftpart, &m_shiftface );
						pickno = m_shiftpart;
						break;
					case ITIOBJ_PART:
						PickDispObj( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, &m_shiftpart );
						pickno = m_shiftpart;
						break;
					case ITIOBJ_JOINT:
						m_shiftpart = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
						pickno = m_shiftpart;
						targetjoint = pickno;
						break;
					default:
						_ASSERT( 0 );
						pickno = 0;
						break;
					}
				}else if( m_tbb_ei2 == 1 ){
					switch( m_infscopedlg->m_object ){
					case ITIOBJ_PART:
						PickInfScopeFace( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, m_infscopedlg->m_isindex, &m_shiftface );
						if( m_shiftface >= 0 )
							pickno = m_infscopedlg->m_isindex + 1;
						else
							pickno = 0;
						break;
					case ITIOBJ_VERTEX:
						PickInfScopeVert( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, m_infscopedlg->m_isindex, m_shifvert, &m_shifvertnum );
						if( m_shifvertnum > 0 )
							pickno = m_infscopedlg->m_isindex + 1;
						else
							pickno = 0;
						break;
					default:
						_ASSERT( 0 );
						m_shiftpart = 0;
						m_shifvertnum = 0;
						pickno = 0;
						break;
					}

				}else if( m_tbb_col == 1 ){
					PickVert( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, &m_shiftpart, m_shifvert, &m_shifvertnum, 0 );
					pickno = m_shiftpart;
				
					if( m_shiftpart >= 0 ){
						ret = m_setcoldlg->OnClickVert( m_shiftpart, m_shifvert, m_shifvertnum );
						if( ret ){
							DbgOut( "viewer : setcoldlg : OnClickVert error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
					}
				}else if( m_tbb_bone == 1 ){
					if( m_bonedlg->m_ope == BONEOPE_POS ){
						m_shiftpart = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
						pickno = m_shiftpart;
						targetjoint = pickno;

						m_itidlg->m_object = ITIOBJ_JOINT;//!!!!!!!!!!
						m_itidlg->m_itimode = 0;//!!!!!!!!!!!!!!!!!!!
					}
				}else if( m_tbb_ei3 == 1 ){

					if( m_paintdlg->m_ope != PAINTOPE_BONE ){

						PickVert( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, &m_shiftpart, m_shifvert, &m_shifvertnum, 0 );
						pickno = m_shiftpart;

						if( m_shiftpart >= 0 ){
							ret = m_paintdlg->OnClickVert( m_shiftpart, m_shifvert, m_shifvertnum, 1 );
							if( ret ){
								DbgOut( "viewer : paintdlg : OnClickVert error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}
					}else{
						m_shiftpart = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
						pickno = m_shiftpart;
						targetjoint = pickno;
					}

				}


				if( pickno > 0 ){
					if( ((m_tbb_iti == 1) && (m_itidlg->m_object == ITIOBJ_JOINT)) || (m_tbb_bone == 1) || ((m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_BONE)) ){
						//選択状態の更新
						if( m_sigeditwnd ){
							ret = m_sigeditwnd->OnUserSelchange( (long)pickno );
							_ASSERT( !ret );
						}
						if( g_motdlg ){
							ret = g_motdlg->OnUserSelchange( pickno );
							_ASSERT( !ret );
						}
						Render3DEnvironment();//!!!!!!!!!!!!!!!!!

						if( m_tbb_bone == 1 ){
							CTreeElem2* selte;
							selte = (*m_thandler)( pickno );
							_ASSERT( selte );

							m_bonedlg->SetCurrentName( selte->name );
						}
					}


					if( (m_tbb_r == 1) || ((m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_BONE)) ){
						m_movebone = pickno;
					}else if( (m_tbb_t == 1) || (m_tbb_t2 == 1) || (m_tbb_iti == 1) || (m_tbb_ikt == 1) || (m_tbb_col == 1) || (m_tbb_bone == 1) || (m_tbb_ei2 == 1) ){
						m_shiftbone = pickno;
					}else if( m_tbb_s == 1 ){
						m_scalebone = pickno;
					}


					switch( m_select_kind ){
					case SELECT_NONE:
						SetSelectColor( 0 );
						break;
					case SELECT_X:
						SetSelectColor( 1 );
						break;
					case SELECT_Y:
						SetSelectColor( 2 );
						break;
					case SELECT_Z:
						SetSelectColor( 3 );
						break;
					default:
						SetSelectColor( 0 );
						break;
					}

					if( ( (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_col == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) ) 
						||  ( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope = PAINTOPE_BONE) ) ){
						

						CShdElem* pickelem;
						pickelem = (*m_shandler)( pickno );
						_ASSERT( pickelem );

						/***
						int totalqno;
						CShdElem* totalelem;
						totalelem = m_shandler->FindUpperJoint( pickelem );
						if( totalelem ){
							totalqno = totalelem->serialno;
						}else{
							totalqno = pickno;
						}
						***/
						
						if( m_tbb_za3 == 0 ){
							ret = m_mhandler->GetTotalQOnFrame( m_shandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), pickno, &m_localq );
							if( ret ){
								DbgOut( "viewer : aft PickBone : mh GetTotalQOnFrame error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
						}else{

							CQuaternion tmpq;
							ret = m_mhandler->GetTotalQOnFrame( m_shandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), pickno, &tmpq );
							if( ret ){
								DbgOut( "viewer : aft PickBone : mh GetTotalQOnFrame error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}

							CQuaternion iniq;
							ret = m_shandler->GetInitialBoneQ( pickno, &iniq );
							if( ret ){
								DbgOut( "viewer : aft PickBone : sh GetInitialBoneQ error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}

							m_localq = tmpq * iniq;

						}
						//DbgOut( "viewer : WM_LBUTTONDOWN : pick %d : %s\n", 
						//	pickno, (*m_thandler)( pickno )->name );

						if( m_sigeditwnd ){
							ret = m_sigeditwnd->OnUserSelchange( (long)pickno );
							_ASSERT( !ret );
						}
						if( g_motdlg ){
							ret = g_motdlg->OnUserSelchange( pickno );
							_ASSERT( !ret );

							CopyUndoBuffer();
						}

						//if( targetjoint > 0 ){
						//	ChangeViewParam( ID_KEY_JOINTTARGET );
						//	Render3DEnvironment();//!!!!!!!!!!!!!!!!!
							//char strchk[ 512 ];
							//sprintf_s( strchk, "pickno %d, targetjoint %d", pickno, targetjoint );
							//::MessageBox( m_hWnd, strchk, "チェック", MB_OK );
						//}

					}else{
						m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
					}


					//ハンドルを選択位置に表示するために、一回０移動のMoveObjectをする。
					if( ((m_tbb_iti == 1) || (m_tbb_bone == 1)) && (m_shiftpart > 0) ){
						ret = m_itidlg->MoveObject( this, m_shandler, m_mhandler, m_shiftbone, m_shiftface, m_shifvert, m_shifvertnum, 0, 0.0f );
						_ASSERT( !ret );
					}else if( (m_tbb_ei2 == 1) && (pickno > 0) ){
						ret = m_infscopedlg->MoveObject( this, m_shandler, m_mhandler, m_shiftface, m_shifvert, m_shifvertnum, 0, 0.0f );
						_ASSERT( !ret );
					}

				}

			}

		}else if( (m_lbutton == 0) && (kind & MK_LBUTTON) && (m_rbutton == 0) && (m_preview_flag == 0) && (m_vikflag == 0) && 
			(m_tbb_bone == 1) && (m_bonedlg->m_ope == BONEOPE_CREATE) ){
			m_lbutton = 1;

			m_lpos.x = xPos;
			m_lpos.y = yPos;
			
			ret = CreateBone( m_lpos );
			if( ret ){
				_ASSERT( 0 );
				return 0;
			}

		}


	}else if( uMsg == WM_RBUTTONDOWN ){

		SetCapture( m_hWnd );


		//wparam MK_RBUTTON
		//lparam low word x
		//lparam high word y
		WORD kind = (WORD)wParam;
		//int ret;

		WORD xPos, yPos;
		xPos = GET_X_LPARAM(lParam); 
		yPos = GET_Y_LPARAM(lParam); 


		if( (kind & MK_RBUTTON) && (m_lbutton == 1) && (m_vikflag == 0) ){
			m_lrbutton = 1;

			m_lbutton = 0;
			m_movebone = 0;
			m_rbutton = 0;
			m_shiftbone = 0;
			m_scalebone = 0;
//DbgOut( "LRBUTTON DOWN!!!\r\n" );
			m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

			m_lrpos.x = xPos;
			m_lrpos.y = yPos;

//		}else if( (m_rbutton == 0) && (kind & MK_RBUTTON) && (m_lbutton == 0) && (m_preview_flag == 0) && (m_vikflag == 0) ){

		}else if( (m_rbutton == 0) && (kind & MK_RBUTTON) && (m_lbutton == 0) && (m_vikflag == 0) ){

			m_rbutton = 1;

			m_rpos.x = xPos;
			m_rpos.y = yPos;

			//DbgOut( "viewer : WM_RBUTTONDOWN %d %d\n", xPos, yPos );

			if( m_preview_flag == 0 ){

				int pickno = 0;
				if( (m_tbb_bone == 0) && (m_tbb_ei3 == 0) ){
					pickno = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
				}else if( m_tbb_bone == 1 ){
					pickno = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 0 );
				}else if( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_SETINF) ){
					PickVert( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, &m_shiftpart, m_shifvert, &m_shifvertnum, 0 );
					pickno = m_shiftpart;

					if( m_shiftpart > 0 ){
						int ret;
						ret = m_paintdlg->OnClickVert( m_shiftpart, m_shifvert, m_shifvertnum, 0 );
						if( ret ){
							DbgOut( "viewer : paintdlg : OnClickVert error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
					}

				}
				if( (pickno > 0) && (m_select_kind == SELECT_CENTER) && (m_tbb_ei3 == 0) ){

					int ret;

					if( m_sigeditwnd ){
						ret = m_sigeditwnd->OnUserSelchange( (long)pickno );
						_ASSERT( !ret );
					}
					if( g_motdlg ){
						ret = g_motdlg->OnUserSelchange( pickno );
						_ASSERT( !ret );
						//CopyUndoBuffer();
					}

					Render3DEnvironment();//!!!!!!!!!!!!!!!!!
				
					if( m_tbb_bone != 1 ){

						POINT pt;
						pt = m_rpos;
						ClientToScreen( m_hWnd, &pt );
						
						CShdElem* selem;
						selem = (*m_shandler)( pickno );

						CTreeElem2* telem;
						telem = (*m_thandler)( pickno );

						m_intormenu = 1;

						s_rmenumain->TrackPopupMenu( telem, selem, m_hWnd, g_inidata->opt_ikfk, pt );

					}else{
						CTreeElem2* selte;
						selte = (*m_thandler)( pickno );
						_ASSERT( selte );

						m_bonedlg->SetCurrentName( selte->name );
					}

				}
			}
		}
		/***
		else if( (m_rbutton == 0) && (kind & MK_RBUTTON) && (m_lbutton == 0) && (m_preview_flag == 0) && (m_vikflag == 0) ){
			m_rbutton = 1;

			m_rpos.x = xPos;
			m_rpos.y = yPos;

			//DbgOut( "viewer : WM_RBUTTONDOWN %d %d\n", xPos, yPos );

			int pickno = 0;
			pickno = PickBone( xPos, yPos, &m_select_kind );
			if( pickno > 0 ){
				m_shiftbone = pickno;


				//DbgOut( "viewer : WM_RBUTTONDOWN : pick %d : %s\n", 
				//	pickno, (*m_thandler)( pickno )->name );

				if( m_sigeditwnd ){
					ret = m_sigeditwnd->OnUserSelchange( (long)pickno );
					_ASSERT( !ret );
				}
				if( g_motdlg ){
					ret = g_motdlg->OnUserSelchange( pickno );
					_ASSERT( !ret );

					CopyUndoBuffer();
				}

			}

		}
		***/
//	}else if( uMsg == MSH_MOUSEWHEEL ){
//
//DbgOut( "viewer : MSH_MOUSEWHEEL \r\n" );

	}else if( uMsg == WM_MBUTTONDOWN ){

		SetCapture( m_hWnd );
		WORD kind = (WORD)wParam;

		WORD xPos, yPos;
		xPos = GET_X_LPARAM(lParam); 
		yPos = GET_Y_LPARAM(lParam); 

		if( (m_mbutton == 0) && (kind & MK_MBUTTON) && (m_vikflag == 0) ){
			m_mbutton = 1;

			m_mpos.x = xPos;
			m_mpos.y = yPos;
		}
	}else if( uMsg == WM_VSCROLL ){

	}else if( uMsg == WM_MOUSEMOVE ){

		m_wheeldelta = 0.0f;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	}else if( uMsg == WM_MOUSEWHEEL ){
		int ret;
		float vert;

		switch( g_inidata->opt_MouseW ){
		case MOUSEOPE_BONETWIST:
			if( (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) ){

				if( g_motdlg && m_shandler && (m_wheeldelta == 0.0f) ){
					if( (m_selectedno > 0) && (m_tbb_ei2 == 0) ){
						CopyUndoBuffer();
					}
				}

				m_wheeldelta = (float)GET_WHEEL_DELTA_WPARAM(wParam) * (float)DEG2PAI * 0.02f;

				if( (m_selectedno > 0) && (m_tbb_ei2 == 0) ){
					ret = g_motdlg->OnTwist( m_selectedno, m_wheeldelta );
					if( ret ){
						_ASSERT( 0 );
					}
					return 0;
				}
			}

			break;
		case MOUSEOPE_CAMDIST:
			//vert = 2000.0f * diffy / (float)(clirect.bottom - clirect.top);
			vert = (float)GET_WHEEL_DELTA_WPARAM(wParam) * -3.0f;
			OnCameraDist( vert );
			return 0;

			break;
		default:
			break;
		}

	}else if( uMsg == WM_TIMER ){
		int timerid;
		//int ret;
		//static POINT befpos = {0, 0};
		//int movestate;

		timerid = (int)wParam;
		if( (timerid == MOVEBONETIMER) ){

			//status barへの表示
			char strjoint[512];
			sprintf_s( strjoint, 512, "FPS : %.02f", m_fFPS );
			SendMessage(g_hStatus, SB_SETTEXT, 0 | 0, (LPARAM)strjoint);
			
			if( g_motdlg ){
				//D3DXVECTOR3 jpos;
				//int setflag;

				//g_motdlg->GetSelectedJointPos( &m_selectedno, &jpos );//worldmat == inimat// <---- FrameMoveへ移動。
				if( (m_tbb_ikt == 0) && (m_tbb_ei3 == 0) ){			
					if( m_selectedno > 0 ){
						sprintf_s( strjoint, 512, "selected joint pos : (%f, %f, %f)", m_selectedpos.x, m_selectedpos.y, m_selectedpos.z );
					}else{
						sprintf_s( strjoint, 512, "selected joint pos : (not selected)" );
					}
				}else if( m_tbb_ikt == 1 ){
					if( m_selectedno > 0 ){
						sprintf_s( strjoint, 512, "selected ikt pos : (%f, %f, %f)", m_selectedpos.x, m_selectedpos.y, m_selectedpos.z );
					}else{
						sprintf_s( strjoint, 512, "selected ikt pos : (not selected)" );
					}
				}else if( m_tbb_ei3 == 1 ){
					if( m_paintdlg->m_ope != PAINTOPE_BONE ){
						sprintf_s( strjoint, 512, "selected joint pos : (not selected)" );
					}else{
						if( m_selectedno > 0 ){
							sprintf_s( strjoint, 512, "selected joint pos : (%f, %f, %f)", m_selectedpos.x, m_selectedpos.y, m_selectedpos.z );
						}else{
							sprintf_s( strjoint, 512, "selected joint pos : (not selected)" );
						}
					}
				}

				SendMessage( g_hStatus, SB_SETTEXT, 1 | 0, (LPARAM)strjoint );


				if( m_mhandler && (m_mhandler->m_kindnum >= 1) ){
					if( m_mhandler->s2mot_leng > 1 ){
						CMotionCtrl* mcptr = (*m_mhandler)( 1 );
						if( !mcptr ){
							_ASSERT( 0 );
							return 1;
						}
						CMotionInfo* miptr = mcptr->motinfo;
						if( !miptr ){
							_ASSERT( 0 );
							return 1;
						}
						if( !(miptr->motname) ){
							_ASSERT( 0 );
							return 1;
						}

						char* szName;
						int cookie = g_motdlg->GetMotCookie();
						if( cookie >= 0 ){
							szName = *(miptr->motname + cookie );
							SendMessage( g_hStatus, SB_SETTEXT, 2 | 0, (LPARAM)szName );
						}
					}
				}

			}

		///////////
			
			POINT ptCursor;
			GetCursorPos( &ptCursor );
			ScreenToClient( m_hWnd, &ptCursor );


			CQuaternion biniq( 1.0f, 0.0f, 0.0f, 0.0f );
			int localflag = 0;

			int ret;
			if( (m_lbutton == 1) && (m_rbutton == 0) && (m_movebone > 0) ){

				if( !m_thandler || !m_shandler || !m_mhandler )
					return -1;
				if( m_ExistData == FALSE )
					return -1;
				if( !g_motdlg )
					return 0;


				if( m_tbb_za1 == 1 ){
					localflag = 1;
				}else if( m_tbb_za2 == 1 ){
					localflag = 0;
				}else{
					localflag = 1;
				}

				int lastparent = 0;

				switch( m_select_kind ){
				case SELECT_CENTER:

//DbgOut( "viewer : timer : movebone : offset %d %d, ptcursor %d %d\r\n", m_clickoffset.x, m_clickoffset.y, ptCursor.x, ptCursor.y );

					if( (abs( m_clickbone.x - (ptCursor.x - m_clickoffset.x) ) >= 2) || (abs( m_clickbone.y - (ptCursor.y - m_clickoffset.y) ) >= 2) ){
						ret = g_motdlg->MoveBone( m_matWorld, m_movebone, (float)( ptCursor.x - m_clickoffset.x ), (float)( ptCursor.y - m_clickoffset.y ), &lastparent );
						if( ret ){
							DbgOut( "viewer : motdlg MoveBone error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
					}
					break;
				case SELECT_X:
					//lastparent = m_movebone;

					ret = GetRotateDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );
					if( ret ){
						DbgOut( "viewer : GetRotateDelta error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					ret = g_motdlg->RotateBone( m_matWorld, m_movebone, 1, m_select_delta, &lastparent, localflag, m_localq, g_inidata->opt_ikfk );//Yaxis
					if( ret ){
						DbgOut( "viewer : motdlg RotateBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Y:
					//lastparent = m_movebone;

					ret = GetRotateDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );
					if( ret ){
						DbgOut( "viewer : GetRotateDelta error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}


					if( m_tbb_za3 == 0 ){
						ret = g_motdlg->RotateBone( m_matWorld, m_movebone, 2, m_select_delta, &lastparent, localflag, m_localq, g_inidata->opt_ikfk );//Zaxis
						if( ret ){
							DbgOut( "viewer : motdlg RotateBone error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
					}else{
						//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						ret = g_motdlg->OnTwist( m_movebone, m_select_delta );
						if( ret ){
							DbgOut( "viewer : motdlg OnTwist error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
						//_ASSERT( 0 );
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Z:
					//lastparent = m_movebone;

					ret = GetRotateDelta( SELECT_Z, m_lpos, ptCursor, &m_select_delta );
					if( ret ){
						DbgOut( "viewer : GetRotateDelta error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					ret = g_motdlg->RotateBone( m_matWorld, m_movebone, 0, m_select_delta, &lastparent, localflag, m_localq, g_inidata->opt_ikfk );//Xaxis
					if( ret ){
						DbgOut( "viewer : motdlg RotateBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				default:
					_ASSERT( 0 );
					lastparent = m_movebone;
					break;
				}
				//::SendMessage( m_hWnd, WM_USER_IKDISPLAY, (WPARAM)lastparent, (LPARAM)1 );

				
				SetBoneArray( lastparent );
				FrameMove( lastparent, 0 );

				//UpdateVertexBuffer();
					
				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}
				
				//befpos = ptCursor;
			//}else if( (m_rbutton == 1) && (m_lbutton == 0) && (m_shiftbone > 0) ){
			}else if( (m_lbutton == 1) && (m_rbutton == 0) && ((m_shiftbone > 0) && (m_tbb_iti == 0) && (m_tbb_ikt == 0) && (m_tbb_col == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0)) ){

				if( !m_thandler || !m_shandler || !m_mhandler )
					return -1;
				if( m_ExistData == FALSE )
					return -1;

				if( !m_pd3dDevice )
					return -1;

				if( !g_motdlg )
					return 0;
					
				if( m_tbb_za1 == 1 ){
					localflag = 1;
				}else if( m_tbb_za2 == 1 ){
					localflag = 0;
				}else{
					localflag = 1;
				}

				switch( m_select_kind ){
				case SELECT_CENTER:

					if( (abs( m_clickbone.x - (ptCursor.x - m_clickoffset.x) ) >= 2) || (abs( m_clickbone.y - (ptCursor.y - m_clickoffset.y) ) >= 2) ){

						ret = g_motdlg->ShiftBone( m_shiftbone, (float)( ptCursor.x - m_clickoffset.x ), (float)( ptCursor.y - m_clickoffset.y ), m_tbb_t2 );
						_ASSERT( !ret );

						//m_lpos.x = ptCursor.x;
						//m_lpos.y = ptCursor.y;
					}
					break;
				case SELECT_X:
					GetShiftDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );

					ret = g_motdlg->ShiftBone( m_shiftbone, 0, m_select_delta, m_tbb_t2, localflag, m_localq );
					if( ret ){
						DbgOut( "viewer : motdlg ShiftBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Y:
					GetShiftDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );

					ret = g_motdlg->ShiftBone( m_shiftbone, 1, m_select_delta, m_tbb_t2, localflag, m_localq );
					if( ret ){
						DbgOut( "viewer : motdlg ShiftBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Z:
					GetShiftDelta( SELECT_Z, m_lpos, ptCursor, &m_select_delta );

					ret = g_motdlg->ShiftBone( m_shiftbone, 2, m_select_delta, m_tbb_t2, localflag, m_localq );
					if( ret ){
						DbgOut( "viewer : motdlg ShiftBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;

					break;
				default:
					_ASSERT( 0 );
					break;
				}

				//::SendMessage( m_hWnd, WM_USER_IKDISPLAY, (WPARAM)m_shiftbone, (LPARAM)1 );


				FrameMove();
				//UpdateVertexBuffer();
				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}

				/***
				CShdElem* shiftelem;
				shiftelem = (*m_shandler)( m_shiftbone );
				CShdElem* shiftparelem;
				
				int updateno;
				shiftparelem = m_shandler->FindUpperJoint( shiftelem );
				if( shiftparelem ){
					updateno = shiftparelem->serialno;
				}else{
					updateno = m_shiftbone;
				}

				SetBoneArray( updateno );
				FrameMove( updateno );

				////UpdateVertexBuffer();

				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}
				***/
			}else if( (m_lbutton == 1) && (m_rbutton == 0) && ((m_shiftbone > 0) && (m_tbb_ikt == 0) && (m_tbb_ei2 == 0) && ((m_tbb_iti == 1) || (m_tbb_bone == 1)) && (m_tbb_col == 0)) ){


				if( !m_thandler || !m_shandler || !m_mhandler )
					return -1;
				if( m_ExistData == FALSE )
					return -1;

				if( !m_pd3dDevice )
					return -1;

				if( !g_motdlg )
					return 0;

				POINT curpoint;
				curpoint.x = ptCursor.x - m_clickoffset.x;
				curpoint.y = ptCursor.y - m_clickoffset.y;


				if( m_tbb_bone == 1 ){
					m_itidlg->m_object = ITIOBJ_JOINT;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					m_itidlg->m_itimode = 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}


				//ret = ChangeJointInitialPos( m_shiftbone, curpoint );
				//if( ret ){
				//	DbgOut( "viewer : timer : ChangeJointInitialPos error !!!\n" );
				//	_ASSERT( 0 );
				//	return 1;
				//}

				switch( m_select_kind ){
				case SELECT_CENTER:

					if( (abs( m_clickbone.x - (ptCursor.x - m_clickoffset.x) ) >= 1) || (abs( m_clickbone.y - (ptCursor.y - m_clickoffset.y) ) >= 1) ){

						ret = m_itidlg->MoveObject( this, m_shandler, m_mhandler, m_shiftpart, m_shiftface, m_shifvert, m_shifvertnum, (float)curpoint.x, (float)curpoint.y );
						_ASSERT( !ret );

						//m_lpos.x = ptCursor.x;
						//m_lpos.y = ptCursor.y;
					}
					break;
				case SELECT_X:
					GetShiftDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );

					ret = m_itidlg->MoveObject( this, m_shandler, m_mhandler, m_shiftbone, m_shiftface, m_shifvert, m_shifvertnum, 0, m_select_delta );
					_ASSERT( !ret );

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Y:
					GetShiftDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );

					ret = m_itidlg->MoveObject( this, m_shandler, m_mhandler, m_shiftbone, m_shiftface, m_shifvert, m_shifvertnum, 1, m_select_delta );
					_ASSERT( !ret );

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Z:
					GetShiftDelta( SELECT_Z, m_lpos, ptCursor, &m_select_delta );

					ret = m_itidlg->MoveObject( this, m_shandler, m_mhandler, m_shiftbone, m_shiftface, m_shifvert, m_shifvertnum, 2, m_select_delta );
					_ASSERT( !ret );

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;

					break;
				default:
					_ASSERT( 0 );
					break;
				}
				
				m_lpos.x = curpoint.x;
				m_lpos.y = curpoint.y;


				FrameMove();
				UpdateVertexBuffer();
				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}

			}else if( (m_lbutton == 1) && (m_rbutton == 0) && (m_shiftbone > 0) && (m_tbb_ei2 == 1) ){


				if( !m_thandler || !m_shandler || !m_mhandler )
					return -1;
				if( m_ExistData == FALSE )
					return -1;

				if( !m_pd3dDevice )
					return -1;

				if( !g_motdlg )
					return 0;

				POINT curpoint;
				curpoint.x = ptCursor.x - m_clickoffset.x;
				curpoint.y = ptCursor.y - m_clickoffset.y;

				switch( m_select_kind ){
				case SELECT_CENTER:

					if( (abs( m_clickbone.x - (ptCursor.x - m_clickoffset.x) ) >= 1) || (abs( m_clickbone.y - (ptCursor.y - m_clickoffset.y) ) >= 1) ){

						ret = m_infscopedlg->MoveObject( this, m_shandler, m_mhandler, m_shiftface, m_shifvert, m_shifvertnum, (float)curpoint.x, (float)curpoint.y );
						_ASSERT( !ret );

						//m_lpos.x = ptCursor.x;
						//m_lpos.y = ptCursor.y;
					}
					break;
				case SELECT_X:
					GetShiftDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );

					ret = m_infscopedlg->MoveObject( this, m_shandler, m_mhandler, m_shiftface, m_shifvert, m_shifvertnum, 0, m_select_delta );
					_ASSERT( !ret );

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Y:
					GetShiftDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );

					ret = m_infscopedlg->MoveObject( this, m_shandler, m_mhandler, m_shiftface, m_shifvert, m_shifvertnum, 1, m_select_delta );
					_ASSERT( !ret );

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Z:
					GetShiftDelta( SELECT_Z, m_lpos, ptCursor, &m_select_delta );

					ret = m_infscopedlg->MoveObject( this, m_shandler, m_mhandler, m_shiftface, m_shifvert, m_shifvertnum, 2, m_select_delta );
					_ASSERT( !ret );

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;

					break;
				default:
					_ASSERT( 0 );
					break;
				}
				
				m_lpos.x = curpoint.x;
				m_lpos.y = curpoint.y;


				FrameMove();
				UpdateVertexBuffer();
				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}

			}else if( (m_lbutton == 1) && (m_rbutton == 0) && ((m_shiftbone > 0) && (m_tbb_iti == 0) && (m_tbb_ikt == 1) && (m_tbb_col == 0)) ){


				if( !m_thandler || !m_shandler || !m_mhandler )
					return -1;
				if( m_ExistData == FALSE )
					return -1;

				if( !m_pd3dDevice )
					return -1;

				if( !g_motdlg )
					return 0;

				POINT curpoint;
				curpoint.x = ptCursor.x - m_clickoffset.x;
				curpoint.y = ptCursor.y - m_clickoffset.y;

				ret = ChangeIKTPos( m_shiftbone, curpoint );
				if( ret ){
					DbgOut( "viewer : timer : ChangeIKTPos error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				m_lpos.x = curpoint.x;
				m_lpos.y = curpoint.y;

				FrameMove();
				UpdateVertexBuffer();
				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}

			}else if( (m_lbutton == 1) && (m_rbutton == 0) && (m_scalebone > 0) ){


				if( !m_thandler || !m_shandler || !m_mhandler )
					return -1;
				if( m_ExistData == FALSE )
					return -1;

				if( !m_pd3dDevice )
					return -1;

				if( !g_motdlg )
					return 0;


				switch( m_select_kind ){
				case SELECT_CENTER:
					//ret = g_motdlg->ShiftBone( m_scalebone, (float)ptCursor.x, (float)ptCursor.y );
					//_ASSERT( !ret );

					//m_lpos.x = ptCursor.x;
					//m_lpos.y = ptCursor.y;


					/***
					POINT curpoint;
					curpoint.x = ptCursor.x - m_clickoffset.x;
					curpoint.y = ptCursor.y - m_clickoffset.y;

					GetScaleDelta( SELECT_Y, m_lpos, curpoint, &m_select_delta );

					ret = g_motdlg->ScaleBone( m_scalebone, 3, m_select_delta );
					if( ret ){
						DbgOut( "viewer : motdlg ScaleBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = curpoint.x;
					m_lpos.y = curpoint.y;
					***/
					GetScaleDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );

					ret = g_motdlg->ScaleBone( m_scalebone, 3, m_select_delta );
					if( ret ){
						DbgOut( "viewer : motdlg ScaleBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;


					break;
				case SELECT_X:
					GetScaleDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );

					ret = g_motdlg->ScaleBone( m_scalebone, 0, m_select_delta );
					if( ret ){
						DbgOut( "viewer : motdlg ScaleBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Y:
					GetScaleDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );


//DbgOut( "y : scaledelta %f\r\n", m_select_delta );

					ret = g_motdlg->ScaleBone( m_scalebone, 1, m_select_delta );
					if( ret ){
						DbgOut( "viewer : motdlg ScaleBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;
					break;
				case SELECT_Z:
					GetScaleDelta( SELECT_Z, m_lpos, ptCursor, &m_select_delta );

//DbgOut( "z : scaledelta %f\r\n", m_select_delta );

					ret = g_motdlg->ScaleBone( m_scalebone, 2, m_select_delta );
					if( ret ){
						DbgOut( "viewer : motdlg ScaleBone error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					m_lpos.x = ptCursor.x;
					m_lpos.y = ptCursor.y;

					break;
				default:
					_ASSERT( 0 );
					break;
				}

				//::SendMessage( m_hWnd, WM_USER_IKDISPLAY, (WPARAM)m_shiftbone, (LPARAM)1 );
				CShdElem* scaleelem;
				scaleelem = (*m_shandler)( m_scalebone );
				CShdElem* scaleparelem;
				
				int updateno;
				scaleparelem = m_shandler->FindUpperJoint( scaleelem );
				if( scaleparelem ){
					updateno = scaleparelem->serialno;
				}else{
					updateno = m_scalebone;
				}


				FrameMove();
				//UpdateVertexBuffer();
				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}

			}
		
///////////      カメラの操作

			if( (m_lrbutton == 1) && (m_movebone <= 0) && (m_shiftbone <= 0) && (m_scalebone <= 0)){

				float diffx, diffy;
				RECT clirect;
				float vert;

				diffx = (float)ptCursor.x - (float)m_lrpos.x;
				diffy = (float)ptCursor.y - (float)m_lrpos.y;
				GetClientRect( m_hWnd, &clirect );
				//vert = 2000.0f * diffy / (float)(clirect.bottom - clirect.top);
				vert = 2000.0f * (diffx + diffy) / (float)(clirect.bottom - clirect.top);

				OnCameraDist( vert );

				m_lrpos = ptCursor;


			}else if( (m_lbutton == 1) && (m_rbutton == 0) && (m_movebone <= 0) && (m_shiftbone <= 0) && (m_scalebone <= 0)){

				m_lpos = ptCursor;

			}else if( (m_rbutton == 1) && (m_lbutton == 0) && (m_movebone <= 0) && (m_shiftbone <= 0) && (m_scalebone <= 0) && (m_intormenu <= 0)){
				float diffx, diffy;
				RECT clirect;
				float vert;

				if( m_cameradlg ){
					switch( m_cameradlg->m_mouseope ){
					case MOUSEOPE_CAMROT:
						OnCameraRot( ptCursor, m_rpos );
						break;
					case MOUSEOPE_CAMMOVE:
						OnCameraMove( ptCursor, m_rpos );
						break;
					case MOUSEOPE_CAMDIST:
						diffx = (float)ptCursor.x - (float)m_rpos.x;
						diffy = (float)ptCursor.y - (float)m_rpos.y;
						GetClientRect( m_hWnd, &clirect );
						//vert = 2000.0f * diffy / (float)(clirect.bottom - clirect.top);
						vert = 2000.0f * (diffx + diffy) / (float)(clirect.bottom - clirect.top);
						OnCameraDist( vert );
						break;
					default:
						break;
					}
				}
				m_rpos = ptCursor;

			}else if( (m_mbutton == 1) && (m_intormenu <= 0)){
				OnCameraMove( ptCursor, m_mpos );
				m_mpos = ptCursor;
			}
		}
/***	
	}else if( uMsg == WM_MOUSEMOVE ){

			int ret;
			POINT ptCursor;
			GetCursorPos( &ptCursor );
			ScreenToClient( m_hWnd, &ptCursor );

			
			if( (ptCursor.x > (int)m_viewport.Width) || (ptCursor.y > (int)m_viewport.Height) ){
				if( m_lbutton == 1 ){
					m_lbutton = 0;
					m_movebone = 0;
				}
				if( m_rbutton == 1 ){
					m_rbutton = 0;
					m_shiftbone = 0;
				}
			}

			if( (m_lbutton == 1) && (m_movebone > 0) ){
				int lastparent = 0;

				static DWORD rendertime = 0;
				DWORD curtime, difftime;
				int renderflag = 0;

				timeBeginPeriod( 1 );
				curtime = timeGetTime();
				timeEndPeriod( 1 );
						
				difftime = curtime - rendertime;
				if( difftime < 100 ){
					renderflag = 0;
				}else{
					renderflag = 1;
					rendertime = curtime;
				}


				ret = MoveBone( m_movebone, (float)ptCursor.x, (float)ptCursor.y, &lastparent );
				_ASSERT( !ret );


				//SetBoneArray( lastparent );
				//FrameMove( lastparent, 0 );
				FrameMove();

				if( renderflag && m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}

				//befpos = ptCursor;
			}else if( (m_rbutton == 1) && (m_shiftbone > 0) ){
 
				ret = ShiftBone( m_shiftbone, (float)ptCursor.x, (float)ptCursor.y );
				_ASSERT( !ret );

				if( m_bActive && m_bReady ){
					Render3DEnvironment( 1 );
				}
			}else{

			}
***/
		

/***
	}else if( uMsg == WM_MOUSEMOVE ){
		WORD xPos, yPos;
		xPos = GET_X_LPARAM(lParam); 
		yPos = GET_Y_LPARAM(lParam); 
		int ret;
		if( (m_lbutton == 1) && (m_movebone > 0) ){
			ret = MoveBone( m_movebone, (float)xPos, (float)yPos );
			_ASSERT( !ret );

		}else if( (m_rbutton == 1) && (m_shiftbone > 0) ){
 
			ret = ShiftBone( m_shiftbone, (float)xPos, (float)yPos );
			_ASSERT( !ret );
		}
***/

	}else if( uMsg == WM_LBUTTONUP ){
		WORD kind = (WORD)wParam;

		if( m_lbutton == 1 ){
			WORD xPos, yPos;
			xPos = GET_X_LPARAM(lParam); 
			yPos = GET_Y_LPARAM(lParam); 

			//DbgOut( "viewer : WM_LBUTTONUP %d %d\n", xPos, yPos );

			if( (m_movebone > 0) || (m_shiftbone > 0) || (m_scalebone > 0) ){			
				g_motdlg->UpdateAllMatrix();//ik後の更新
			}
			m_lbutton = 0;
			m_movebone = 0;
			m_shiftbone = 0;
			m_scalebone = 0;

			m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

			if( m_tbb_ei2 == 1 ){
				m_infscopedlg->SetPoint();
			}

		}
		m_lrbutton = 0;
		m_select_kind = SELECT_NONE;

		SetSelectColor( 0 );

		ReleaseCapture();

	}else if( uMsg == WM_RBUTTONUP ){

		if( m_rbutton == 1 ){
			WORD xPos, yPos;
			xPos = GET_X_LPARAM(lParam); 
			yPos = GET_Y_LPARAM(lParam); 

			//DbgOut( "viewer : WM_RBUTTONUP %d %d\n", xPos, yPos );
			m_rbutton = 0;
			//m_shiftbone = 0;

			//FrameMove();	
			//if( m_bActive && m_bReady ){
			//	Render3DEnvironment( 1 );
			//}

			if( (m_movebone > 0) || (m_shiftbone > 0) || (m_scalebone > 0) ){			
				g_motdlg->UpdateAllMatrix();//ik後の更新
			}

		}
		m_lrbutton = 0;
		m_select_kind = SELECT_NONE;

		m_intormenu = 0;

		SetSelectColor( 0 );

		ReleaseCapture();

	}else if( uMsg == WM_MBUTTONUP ){

		if( m_mbutton == 1 ){
			WORD xPos, yPos;
			xPos = GET_X_LPARAM(lParam); 
			yPos = GET_Y_LPARAM(lParam); 

			m_mbutton = 0;
			//g_motdlg->UpdateAllMatrix();//ik後の更新

		}
		//m_select_kind = SELECT_NONE;
		//SetSelectColor( 0 );
		ReleaseCapture();

	}else if( uMsg == WM_USER_ANIMMENU ){


		int selindex;
		selindex = (int)wParam;

		if( selindex < 0 ){
			return 0;//!!!!!!!!!
		}

		_ASSERT( m_animmenu );

		int iAnimSet, cAnimSets;
		cAnimSets = GetMenuItemCount( m_animmenu );
		for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
		{
			RemoveMenu(m_animmenu, 0, MF_BYPOSITION);
		}


		if( !m_mhandler || !m_shandler ){
			return 0;//!!!!!!!!!!!!!!!!!!
		}

		if( m_mhandler->s2mot_leng <= 1 ){
			return 0;//!!!!!!!!!!!!!!!!!!
		}

		cAnimSets = g_motdlg->GetMotionNum();

		if( cAnimSets <= 0 ){
			return 0;//!!!!!!!!!!!!!!!!!!!
		}


		CMotionCtrl* mcptr = (*m_mhandler)( 1 );
		if( !mcptr ){
			_ASSERT( 0 );
			return 1;
		}
		CMotionInfo* miptr = mcptr->motinfo;
		if( !miptr ){
			_ASSERT( 0 );
			return 1;
		}
		if( !(miptr->motname) ){
			_ASSERT( 0 );
			return 1;
		}

		MOTID* motidarray;
		motidarray = g_motdlg->GetMotIDArray();
		_ASSERT( motidarray );


		char* szName;
		for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
		{
			int cookie;
			cookie = (motidarray + iAnimSet)->id;
			szName = *(miptr->motname + cookie);

			if (szName != NULL)
				AppendMenu(m_animmenu, MF_STRING, 59900 + iAnimSet, szName);
			else
				AppendMenu(m_animmenu, MF_STRING, 59900 + iAnimSet, "<No Animation Name>");
		}

		if( cAnimSets > 0 ){
			CheckMenuItem(m_appmenu, 59900 + selindex, MF_CHECKED);
		}

	}else if( uMsg == WM_USER_BONEDISP ){

		ToggleBoneDisp();

	}else if( uMsg == WM_USER_ERROR ){
		ERRORMES* mes = (ERRORMES*)lParam;

		SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

		DisplayErrorMsg( (HRESULT)(mes->errorcode), mes->type, mes->mesptr );
		
		if( (mes->type != MSGERR_APPMUSTEXIT) && m_hWnd )
			SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

		return 0;

	}else if( uMsg == WM_USER_DISPLAY ){
		if( m_bActive && m_bReady 
			//&& !m_moving_cameradlg
			//&& !m_moving_sigdlg 
			){

			Render3DEnvironment();
		}		

//	}else if( uMsg == WM_USER_IKDISPLAY ){
//		int parno;
//		int skipflag;

//		parno = (int)wParam;
//		skipflag = (int)lParam;

//		SetBoneArray( parno );
//		FrameMove( parno, 0 );

////		UpdateVertexBuffer();
				
//		if( m_bActive && m_bReady ){
//			Render3DEnvironment( skipflag );
//		}
//		return 0;

	}else if( uMsg == WM_USER_ENABLE_MENU ){
		int flag = (int)wParam;
		if( flag )
			m_menucnt--;
		else
			m_menucnt++;
		
		_ASSERT( m_menucnt >= 0 );

		if( m_menucnt == 0 ){
			::SetMenu( m_hWnd, m_appmenu );
		}else if( m_menucnt == 1 ){
			//::SetMenu( m_hWnd, NULL );
			::SetMenu( m_hWnd, m_dummymenu );
		}

	}else if( uMsg == WM_USER_KEY ){
		if( m_bActive && m_bReady ){
			int keyflag = (int)wParam;
			ChangeViewParam( keyflag );
			//Render3DEnvironment();
		}
	}else if( uMsg == WM_USER_MOVING ){
		int msgfrom = (int)wParam;
		switch( msgfrom ){
		case ID_MSG_FROM_CAMERADLG:
			m_moving_cameradlg = 1;
			break;
		case ID_MSG_FROM_SIGDLG:
			m_moving_sigdlg = 1;
			break;
		default:
			break;
		}
//DbgOut( "wm_user_moving %d\r\n", msgfrom );

	}else if( uMsg == WM_USER_MOVE ){

		//if( m_bActive && m_bReady ){
		//	Render3DEnvironment();
		//}		
		int msgfrom = (int)wParam;
		int redrawflag[3] = {0, 0, 0};		
		switch( msgfrom ){
		case ID_MSG_FROM_CAMERADLG:
			redrawflag[0] = 0;
			redrawflag[1] = 1;
			redrawflag[2] = 1;
			m_moving_cameradlg = 0;
			break;
		case ID_MSG_FROM_SIGDLG:
			redrawflag[0] = 1;
			redrawflag[1] = 0;
			redrawflag[2] = 1;
			m_moving_sigdlg = 0;
			break;
		case ID_MSG_FROM_MOTDLG:
			redrawflag[0] = 1;
			redrawflag[1] = 1;
			redrawflag[2] = 0;
			break;
		default:
			break;
		}

//DbgOut( "wm_user_move %d, %d, %d\r\n", msgfrom, m_moving_sigdlg, m_moving_cameradlg );

		if( redrawflag[0] && g_inidata->camdlg_disp ){
			m_cameradlg->Redraw();
		}
		if( redrawflag[1] && g_inidata->sigdlg_disp ){
			m_sigeditwnd->Redraw();
		}
		if( redrawflag[2] && g_inidata->motdlg_disp ){
			if( g_motdlg )
				g_motdlg->Redraw();
		}

	}else if( uMsg == WM_USER_SELCHANGE ){
		int ret = 0;
		int selno = (int)lParam;
		int msgfrom = (int)wParam;

		int pmsonomama;
		if( m_tbb_ei2 == 1 ){
			pmsonomama = 1;
		}else{
			pmsonomama = 0;
		}

		ret = m_shandler->SetIsSelected( selno, 1, pmsonomama );
		if( ret ){
			DbgOut( "MsgProc : WM_USER_SELCHANGE : SetIsSelected error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_SELCHANGE : SetIsSelected error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			return -1;	
		}

//	char messtr[1024];
//	sprintf_s( messtr, 1024, "viewer : USER_SELCHANGE : from %d, seri %d", msgfrom, selno );
//	::MessageBox( m_hWnd, messtr, "確認", MB_OK );


		int selchangeflag[ ID_MSG_FROM_MAX ];
		int dlgno;
		for( dlgno = 0; dlgno < ID_MSG_FROM_MAX; dlgno++ ){
			selchangeflag[ dlgno ] = 1;
		}

		if( msgfrom >= 0 ){
			selchangeflag[ msgfrom ] = 0;//!!!!!!!
		}

		if( selchangeflag[ ID_MSG_FROM_MOTDLG ] ){
			ret = g_motdlg->OnUserSelchange( selno );
			_ASSERT( !ret );
		}
		if( selchangeflag[ ID_MSG_FROM_SIGDLG ] ){
			ret = m_sigeditwnd->OnUserSelchange( (long)selno );
			_ASSERT( !ret );
		}
		if( selchangeflag[ ID_MSG_FROM_BONEDLG ] ){
			if( m_tbb_bone == 1 ){
				CTreeElem2* selte;
				selte = (*m_thandler)( selno );
				_ASSERT( selte );
				m_bonedlg->SetCurrentName( selte->name );
			}
		}

		if( selchangeflag[ ID_MSG_FROM_INFSCOPEDLG ] ){
			if( m_tbb_ei2 == 1 ){
				ret = m_infscopedlg->OnUserSelChange( selno );
				_ASSERT( !ret );
			}
		}

		if( selchangeflag[ ID_MSG_FROM_PAINTDLG ] ){
			if( m_tbb_ei3 == 1 ){
				ret = m_paintdlg->OnUserSelChange( selno );
				_ASSERT( !ret );
			}
		}
		//if( g_motdlg ){
		//	ChangeViewParam( ID_KEY_JOINTTARGET );
		//}

	}else if( uMsg == WM_USER_INFLUENCE ){
		
		int serino = (int)wParam;
		int ret = 0;
		//BoneElemの計算しなおし
		CShdElem* selem;
		selem = (*m_shandler)( serino );

		selem->m_loadbimflag = 0;//!!!!!!!!!!!!!!!!!!!!

		ret = m_shandler->SetMikoAnchorApply( m_thandler );
		if( ret ){
			DbgOut( "viewer : MsgProc : WM_USER_INFLUENCE : SetMikoAnchorApply error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_INFLUENCE : CalcInfElem error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_shandler->CalcInfElem( serino, 1 );
		if( ret ){
			DbgOut( "CMyD3DApplication : MsgProc : WM_USER_INFLUENCE : CalcInfElem error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_INFLUENCE : CalcInfElem error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


	}else if( (uMsg == WM_USER_REMAKE_TREE) 
		|| (uMsg == WM_USER_CHANGE_NOTUSE) ){
		//BoneInfoの作り直し。
		int ret;

		int selno = (int)lParam;
		CShdElem* selelem;
		selelem = (*m_shandler)( selno );

		
		if( (uMsg == WM_USER_REMAKE_TREE) || selelem->IsJoint() ){

			ret = m_shandler->DestroyBoneInfo( m_mhandler );
			if( ret ){
				DbgOut( "CMyD3DApplication : MsgProc : WM_USER_REMAKE_TREE : DestroyBoneInfo error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_REMAKE_TREE : DestroyBoneInfo error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
			
			ret = m_shandler->CreateBoneInfo( m_mhandler );
			if( ret ){
				DbgOut( "CMyD3DApplication : MsgProc : WM_USER_REMAKE_TREE : CreateBoneInfo error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_REMAKE_TREE : CreateBoneInfo error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
			

			ret = m_shandler->SetMikoAnchorApply( m_thandler );
			if( ret ){
				DbgOut( "viewer : MsgProc : WM_USER_REMAKE_TREE : SetMikoAnchorApply error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_INFLUENCE : CalcInfElem error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}

			//BoneElemの計算しなおし
			ret = m_shandler->CalcInfElem( -1, 1 );
			if( ret ){
				DbgOut( "CMyD3DApplication : MsgProc : WM_USER_REMAKE_TREE : CalcInfElem error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_REMAKE_TREE : CalcInfElem error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		
		}

		//motdlgのtreeの作り直し。
		if( g_motdlg ){
			ret = g_motdlg->RemakeTreeView( selno );
			if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_REMAKE_TREE : RemakeTreeView error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
			}
		}

		m_paintdlg->InitUndoBuf();

	}else if( uMsg == WM_USER_NEW_JOINT ){
		int ret = 0;
		int addnum = (int)wParam;

		ret = NewJoint( addnum );
		if( ret ){
			DbgOut( "CMyD3DApplication : MsgProc : WM_USER_NEW_JOINT error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_NEW_JOINT error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}else if( uMsg == WM_USER_DEL_JOINT ){
		int ret;
		int delno;
		delno = (int)wParam;
		
		ret = DelJoint( delno );
		if( ret ){
			DbgOut( "CMyD3DApplication : MsgProc : WM_USER_DEL_JOINT error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_DEL_JOINT error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}else if( uMsg == WM_USER_CONV2SCOPE ){
		int ret;
		int* arrayptr;

		arrayptr = (int*)lParam;

		ret = Conv2Scope( *arrayptr, *(arrayptr + 1), *(arrayptr + 2) );
		if( ret ){
			DbgOut( "CMyD3DApplication : MsgProc : WM_USER_CONV2SCOPE error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_CONV2SCOPE error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

	}else if( uMsg == WM_USER_CHANGE_JOINTLOC ){
		int ret = 0;
		int serino = (int)wParam;
		CVec3f* newloc;
		if( lParam != 0 ){
			newloc = (CVec3f*)lParam;
		}else{
			newloc = 0;
		}

		if( (wParam != 0xFFFF) && (lParam != 0) ){
			ret = m_shandler->ChangeJointLoc( serino, newloc );
			if( ret ){
				DbgOut( "CMyD3DApplication : MsgProc : WM_USER_CHANGE_JOINTLOC error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_CHANGE_JOINTLOC error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		}

		//BoneInfoの作り直し。

		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : MsgProc : WM_USER_CHANGE_JOINTLOC : DestroyBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_CHANGE_JOINTLOC : DestroyBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		
		ret = m_shandler->CreateBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : MsgProc : WM_USER_CHANGE_JOINTLOC : CreateBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_CHANGE_JOINTLOC : CreateBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


		ret = m_shandler->SetMikoAnchorApply( m_thandler );
		if( ret ){
			DbgOut( "viewer : MsgProc : WM_USER_CHANGE_JOINTLOC : SetMikoAnchorApply error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_INFLUENCE : CalcInfElem error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


		//BoneElemの計算しなおし
		ret = m_shandler->CalcInfElem( -1, 1 );
		if( ret ){
			DbgOut( "CMyD3DApplication : MsgProc : WM_USER_CHANGE_JOINTLOC : CalcInfElem error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_CHANGE_JOINTLOC : CalcInfElem error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


		if( g_motdlg ){
			//ret = g_motdlg->UpdateMatrix( serino );
			//ret += g_motdlg->SetCurrentMotion();
			ret = g_motdlg->UpdateAllMatrix();
			if( ret ){
				DbgOut( "CMyD3DApplication : MsgProc : WM_USER_CHANGE_JOINTLOC : MotDlg func error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_CHANGE_JOINTLOC : MotDlg func error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		}

		if( m_bActive && m_bReady 
			&& !m_moving_cameradlg
			&& !m_moving_sigdlg ){

			//DbgOut( "WM_USER_CHANGE_JOINTLOC : Redraw\n" );

			FrameMove();
			Render3DEnvironment();
		}		

		return 0;

	}else if( uMsg == WM_USER_CALCEUL ){
		int ret;
		ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );//!!!!!!!!!!!!!!!!!
		_ASSERT( !ret );
		return 0;

	}else if( uMsg == WM_USER_REMAKE_TEXTURE ){
		if( m_shandler ){
			int ret = 0;
			int serino = (int)wParam;
			//CShdElem* selem = (*m_shandler)( serino );
			//ret += selem->SetTexName( (char*)lParam );
			//_ASSERT( !ret );
			
			ret += m_shandler->InvalidateTexture();
			_ASSERT( !ret );

			ret += m_shandler->CreateTexture( m_pd3dDevice, m_hWnd, 1, g_miplevels, g_mipfilter );
			_ASSERT( !ret );

			ret += m_shandler->SetTexTransparent();
			_ASSERT( !ret );

			ret += m_shandler->ChkAlphaNum();
			_ASSERT( !ret );

			if( ret ){
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_REMAKE_TEXTURE error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				return -1;
			}

		}
	}else if( uMsg == WM_USER_RENAME ){
		if( m_thandler ){
			int ret = 0;
			int serino = (int)wParam;
			CTreeElem2* telem = (*m_thandler)( serino );
			ret += telem->SetName( (char*)lParam );
			_ASSERT( !ret );
	
			if( g_motdlg ){
				ret += g_motdlg->Rename( serino, (char*)lParam );
				_ASSERT( !ret );
			}

			if( ret ){
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "MsgProc : WM_USER_RENAME error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				return -1;
			}

			ret = (int)MessageBox( m_hWnd, "影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}

		}
	}else if( uMsg == WM_USER_REMAKE_DISPOBJ ){
		int ret = 0;
		int serino = (int)wParam;
		CShdElem* selem = (*m_shandler)( serino );
		ret += selem->InvalidateDispObj();
		_ASSERT( !ret );

		ret += m_shandler->SetMikoAnchorApply( m_thandler );
		_ASSERT( !ret );

		ret += selem->CreateDispObj( m_shandler->m_seri2boneno, m_pd3dDevice, m_mhandler, m_shandler, s_tlmode, 1 );
		_ASSERT( !ret );

		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_REMAKE_DISPOBJ error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			return -1;
		}

	}else if( uMsg == WM_USER_REMAKE_UV ){
		int ret = 0;
		int serino = (int)wParam;
		if( serino > 0 ){
			CShdElem* selem = (*m_shandler)( serino );
			ret = selem->SetUV( m_shandler->m_mathead );
			_ASSERT( !ret );
			ret = selem->UpdateVertexBuffer( 0, TLMODE_ORG, m_shandler->m_shader );
			_ASSERT( !ret );
		}else{
			int seri;
			for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
				CShdElem* selem;
				selem = (*m_shandler)( seri );
				if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){
					ret = selem->SetUV( m_shandler->m_mathead );
					_ASSERT( !ret );
					ret = selem->UpdateVertexBuffer( 0, TLMODE_ORG, m_shandler->m_shader );
					_ASSERT( !ret );
				}
			}
		}

		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_REMAKE_UV error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			return -1;
		}

	}else if( uMsg == WM_USER_REFUGE_MOTDLG ){
		;
	}else if( uMsg == WM_USER_ENDDIALOG ){
		int dlgno = (int)wParam;
		switch( dlgno ){
		case ID_MSG_FROM_CAMERADLG:
			if( g_inidata->camdlg_disp ){
				::CheckMenuItem( m_appmenu, IDMENU_CAMERA, MF_UNCHECKED );
				UpdateDialogIniData( dlgno );

				g_inidata->camdlg_disp = 0;
			}
			break;
		case ID_MSG_FROM_SIGDLG:
			if( g_inidata->sigdlg_disp ){
				::CheckMenuItem( m_appmenu, IDMENU_TREEEDIT, MF_UNCHECKED );
				UpdateDialogIniData( dlgno );

				g_inidata->sigdlg_disp = 0;
			}
			break;
		case ID_MSG_FROM_MOTDLG:
			if( g_inidata->motdlg_disp ){
				::CheckMenuItem( m_appmenu, IDMENU_MOTEDIT, MF_UNCHECKED );
				UpdateDialogIniData( dlgno );

				g_inidata->motdlg_disp = 0;
			}
			break;
		default:
			break;
		}


	}else if( uMsg == WM_USER_PUTBACK_MOTDLG ){
		;
	}else if( uMsg == WM_USER_CHANGE_COLOR ){
		int ret = 0;
		int serino = (int)wParam;
		//CShdElem* selem = (*m_shandler)( serino );
		//ret = selem->InitColor( m_mhandler );

		ret = m_shandler->InitColor( m_vEyePt, m_mhandler, serino );
		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_CHANGE_COLOR error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			return -1;
		}
	}else if( uMsg == WM_USER_PREVIEW ){
		m_preview_flag = (int)lParam;

	}else if( uMsg == WM_USER_SELLOCK ){
		m_sellockflag = (int)wParam;
		char* nameptr = (char*)lParam;

		if( m_sellockflag ){
			_stprintf_s( m_strSellock, 1024, _T("!! Under Select Lock !!") );
		}else{
			_stprintf_s( m_strSellock, 1024, _T("") );
		}

		DbgOut( "viewer : WM_USER_SELLOCK : m_sellockflag %d, nameptr %s\n", m_sellockflag, nameptr );

	}else if( uMsg == WM_USER_INIT_UNDOBUF ){

		if( lParam == 1 ){
			EnableMenuItem( m_appmenu, IDMENU_IKUNDO, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem( m_appmenu, IDMENU_REVIKUNDO, MF_BYCOMMAND | MF_GRAYED );

			m_ik_cnt = 0;
			m_undo_cnt = 0;
			m_undo_init_flag = 1;
		}else{
			//m_undo_cnt++;
		}
	}else if( uMsg == WM_KEYDOWN ){
		int downkey = (int)wParam;
		static int keycnt = 0;
		WORD rep;
		DWORD state;
		//int ret;

//DbgOut( "viewer : KEYDOWN : %d, ctrl %d, shift %d\n", wParam, VK_CONTROL, VK_SHIFT );

		unsigned char tempkeybuf[256];
		GetKeyboardState( tempkeybuf );

		if( tempkeybuf[VK_CONTROL] & 0x80 ){
			m_control_key = 1;
		}else{
			m_control_key = 0;
		}
		if( tempkeybuf[VK_SHIFT] & 0x80 ){
			m_shift_key = 1;
		}else{
			m_shift_key = 0;
		}

		rep = (WORD)( lParam & 0x0000FFFF );
		state = (DWORD)( lParam & 0x40000000 );

/******
		//}else if( (downkey == 'Z') && (state == 0) && (m_control_key == 1) && (m_shift_key == 1) ){
		if( (downkey == 'X') && (state == 0) && (m_control_key == 1) ){
		    
			if( g_motdlg ){
				if( m_ik_cnt > 0 ){
					int undono, undomax;

					if( m_undo_init_flag == 0 ){
						m_undo_cnt--;
					}
					m_undo_init_flag = 0;

					undomax = min( m_ik_cnt, MAXUNDONUM );

					if( m_undo_cnt < 0 ){
						m_undo_cnt = undomax + m_undo_cnt;
					}
					undono = abs( m_undo_cnt ) % undomax;

					ret = g_motdlg->PasteUndoBuffer( undono, m_optdlg->m_undodlg );
					_ASSERT( !ret );
				}
			}

		}else if( (downkey == 'Z') && (state == 0) && (m_control_key == 1) ){

			if( g_motdlg ){
				if( m_ik_cnt > 0 ){
					int undono, undomax;
					
					if( m_undo_init_flag == 0 )
						m_undo_cnt++;

					m_undo_init_flag = 0;

					undomax = min( m_ik_cnt, MAXUNDONUM );
					undono = abs( m_undo_cnt ) % undomax;

					ret = g_motdlg->PasteUndoBuffer( undono, m_optdlg->m_undodlg );
					_ASSERT( !ret );
				}
			}

		}else if( (downkey == 'J') && (state == 0) && (m_control_key == 1) ){
			//ジョイントの表示、非表示の切り替え
			
			ToggleBoneDisp();
		}else if( (downkey == 'C') && (state == 0) && (m_control_key == 1) ){
			ret = g_motdlg->OnFrameCopy();
			_ASSERT( !ret );
		}else if( (downkey == 'V') && (state == 0) && (m_control_key == 1) ){
			ret = g_motdlg->OnFramePaste();
			_ASSERT( !ret );
		}else if( (downkey == VK_HOME) && (state == 0) ){
			BOOL dummy;
			NMUPDOWN nmud;
			nmud.iDelta = -10000;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
		}else if( (downkey == VK_END) && (state == 0) ){
			BOOL dummy;
			NMUPDOWN nmud;
			nmud.iDelta = 10000;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
		}else if( (downkey == VK_PRIOR) && (state == 0) ){
			BOOL dummy;
			NMUPDOWN nmud;
			nmud.iDelta = -1;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
		}else if( (downkey == VK_NEXT) && (state == 0) ){
			BOOL dummy;
			NMUPDOWN nmud;
			nmud.iDelta = 1;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
		}

		keycnt++;
*****/

	}else if( uMsg == WM_KEYUP ){
		int upkey = (int)wParam;

		//KEYUPメッセージでは、Ctrlやshiftなどが、wParam == 13 (VK_PAUSE)に化けてしまうので、使用をさける。

		/***

DbgOut( "viewer : KEYUP : %d, ctrl %d, shift %d\n", upkey, VK_CONTROL, VK_SHIFT );

		if( upkey == VK_CONTROL ){
			DbgOut( "viewer : KEYUP : VK_CONTROL\n" );
			m_control_key = 0;
		}
		if( upkey == VK_SHIFT ){
			DbgOut( "viewer : KEYUP : VK_SHIFT\n" );
			m_shift_key = 0;
		}
		***/

	}else if( uMsg == WM_USER_CREATE_PB ){
		int ret;
		ret = CreateProgressBar();
		if( ret ){
			_ASSERT( 0 );
			return -1;
		}
	}else if( uMsg == WM_USER_SET_PB ){
		int ret;
		int newpos = (int)wParam;
		ret = SetProgressBar( newpos );
		if( ret ){
			_ASSERT( 0 );
			return -1;
		}
	}else if( uMsg == WM_USER_DESTROY_PB ){
		int ret;
		ret = DestroyProgressBar();
		if( ret ){
			_ASSERT( 0 );
			return -1;
		}
	}else if( uMsg == WM_USER_IMPORTMQO ){
		int ret;
		ret = ImportMqoFile( (LOADFILEINFO*)lParam );
		if( ret ){
			_ASSERT( 0 );
			return -1;
		}
	}else if( uMsg == WM_USER_LOADBON ){
		int ret;
		ret = LoadBoneFile( (LOADFILEINFO*)lParam );
		if( ret ){
			_ASSERT( 0 );
			return -1;
		}
	}else if( uMsg == WM_COMMAND ){
		BOOL dummy;
		NMUPDOWN nmud;
		int ret;

		WORD menuid;
		menuid = LOWORD( wParam );

		if( m_plugin && (menuid >= ID_MENU_PLUGIN) && (menuid <= (ID_MENU_PLUGIN + MAXPLUGIN)) ){
			m_onselectplugin = 1;

			int pluginno;
			for( pluginno = 0; pluginno < MAXPLUGIN; pluginno++ ){
				if( (m_plugin + pluginno)->menuid == menuid ){
					//DbgOut( "viewer : OnSelectPlugin : pluginno %d, menuid %d\r\n", pluginno, menuid );
					ret = (m_plugin + pluginno)->CallOnSelectPlugin();
					_ASSERT( !ret );
				}
			}

			m_onselectplugin = 0;

			//_ASSERT( 0 );
			return 0;//!!!!!!!
		}

		/// AnimMenu
		if( (menuid >= 59900) && (menuid <= (59900 + MAXMOTIONNUM - 1)) ){
			int selindex;
			selindex = menuid - 59900;
			
			int motnum;
			motnum = g_motdlg->GetMotionNum();

			if( selindex >= motnum ){
				return 0;//!!!!!!!!!!!!!
			}

			ret = g_motdlg->SelectCurrentMotion( selindex );
			if( ret ){
				DbgOut( "viewer : AnimMenu : SelectCurrentMotion error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			m_setcoldlg->InitUndoBuf();
			m_paintdlg->InitUndoBuf();


			return 0;
		}

		///





		switch( menuid )
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///59900からMAXMOTIONNUM(300)分、つまり60200までは使用禁止。アニメーションメニュー
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
		case 59000://新規空
			NewMotion();
			break;
		case 59001://全削除
			DestroyMotion( 1 );
			break;
		case 59002://カレント削除
			DestroyMotion( 0 );
			break;


		case 59200:
			LoadMAFile();
			break;
		case 59201:
			WriteMAFile( 0, 0 );
			break;

		case 59202:
			SaveLimFile();
			break;
		case 59203:
			LoadLimFile();
			break;
		case 59204:
			SaveBVHFile();
			break;

		case IDMENU_BG:
			SetBg();
			break;
		case IDMENU_LIGHT:
			SetLight( 1 );
			break;
		//case IDMENU_VERSION:
		case 32790:		
			ShowVersionDlg();
			break;
		case IDMENU_READ_SIG:
			LoadSigFile();
			break;
		//case 5020:
		//	LoadFBXFile();
		//	break;
		case 5021:
			LoadXFile();
			break;
		case 5022://抽出読み込み
			ret = ExtractBoneData();
			if( ret ){
				_ASSERT( 0 );
				return -1;
			}
			break;
		case 5030://上書き保存
			ret = OverWriteFiles();
			_ASSERT( !ret );
			if( ret ){
				::MessageBox( m_hWnd, "上書き保存に失敗しました。", "エラー", MB_OK );
			}
			break;
		case 5040:
			ret = ConvSymmXShape();
			_ASSERT( !ret );
			if( ret ){
				::MessageBox( m_hWnd, "変換に失敗しました。", "エラー", MB_OK );
			}
			break;
		case 5041:
			ret = ConvSymmXMotion();
			_ASSERT( !ret );
			if( ret ){
				::MessageBox( m_hWnd, "変換に失敗しました。", "エラー", MB_OK );
			}
			break;
		case IDMENU_READ_ROK:
			LoadRokFile();
			break;
		case IDMENU_READ_MQO:
			LoadMQOFile( 0 );
			break;
		case 6100:
			LoadGroundData();
			break;
		//case IDMENU_READ_ACT:
		case 5010:

//DbgOut( "viewer : 5010 : start LoadMotion\r\n" );
			//ret = LoadMotion( 0, 0, 0, 0, 0 );
			ret = LoadMotionMulti();
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}else{
				return 0;
			}
			break;

		case 5050:
			if( g_motdlg && m_mhandler ){

				if( m_tbb_iti ){
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}

				//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
				int ret;
				ret = g_motdlg->ImportMotion( m_loadnamedlg );
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

				ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );//!!!!!!!!!!!!!!!!!
				_ASSERT( !ret );

				m_setcoldlg->InitUndoBuf();
				m_paintdlg->InitUndoBuf();

			}

			break;

		case 5011:
			if( g_motdlg ){

				if( g_inidata->motdlg_disp == 0 ){
					//motparamdlgを表示しない状態で、BVHを読むと、LoadMQOFileで、mothandler に有効な値が入らない。
					::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_MOTEDIT, 0 );
				}

				if( m_tbb_iti ){
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}

				if( g_motdlg->GetMotionNum() >= MAXMOTIONNUM ){
					::MessageBox( m_hWnd, "読み込めるモーションは、５０個までです。\nこれ以上、読み込めません。", "エラー", MB_OK );
					return 0;
				}


				//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
				int ret;
				ret = g_motdlg->LoadBVHFile();
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

				ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );//!!!!!!!!!!!!!!!!!
				_ASSERT( !ret );

				m_setcoldlg->InitUndoBuf();
				m_paintdlg->InitUndoBuf();

				if( m_mcdlg->m_firstmake ){
					ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), 0, 0, 0, 0, 0 );
					if( ret ){
						DbgOut( "viewer : load bvh 5011 : mcdlg AddParentMC error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}


			}
			break;
		case 5012:
			if( g_motdlg ){
				ret = LoadMKM();
				_ASSERT( !ret );
				return ret;
			}
			break;
		case 6001:

			ret = DestroyDispAndMot();
			return ret;
			break;
		case 6002:

			ret = SetCurrentPose2OrgData();
			return ret;
			break;
		case 6004:

			ret = SetInvPose2OrgData();
			return ret;
			break;

		case 6003:
			if( m_shandler ){
				ret = m_shandler->SetMikoAnchorApply( m_thandler );
				if( ret ){
					DbgOut( "viewer : MENU : CalcInfElem : SetMikoAnchorApply error !!!\n" );
					return ret;
				}
				
				int excludeflag;
				if( (int)lParam == 1 ){
					excludeflag = 0;
				}else{
					excludeflag = 1;//メニューから呼ばれた場合も、lParamは０
				}

				//char chkmes[256];
				//sprintf( chkmes, "6003 : lParam %d, excludeflag %d", (int)lParam, excludeflag );
				//::MessageBox( m_hWnd, chkmes, "check", MB_OK );

				ret = m_shandler->CalcInfElem( -1, 1, excludeflag );

				//ret = m_shandler->UpdateVertexBuffer( 0 );

				return ret;
			}
			break;
		case 6010:
			ret = SetMikoBlendType();
			return ret;
			break;

		case IDMENU_IMPORTSIG:
			ImportSigFile();
			break;
		case IDMENU_IMPORTROK:
			ImportRokFile();
			break;
		case IDMENU_IMPORTMQO:
			ImportMqoFile( 0 );
			break;
		case IDMENU_WRITE_SIG:
			SaveSigFile( 0 );
			break;
		case IDMENU_WRITE_ACT:
			if( g_motdlg && m_mhandler ){
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

				int ret;
				ret = g_motdlg->SaveMotion( m_loadnamedlg );
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			}
			break;
		case IDMENU_CAMERA:
			if( g_inidata->camdlg_disp ){
				m_cameradlg->ShowWindow( SW_HIDE );
				::CheckMenuItem( m_appmenu, IDMENU_CAMERA, MF_UNCHECKED );

				if( lParam == 0 ){
					RECT newrect;
					m_cameradlg->GetWindowRect( &newrect );
					g_inidata->camdlg_posx = newrect.left;
					g_inidata->camdlg_posy = newrect.top;
				}

				g_inidata->camdlg_disp = 0;
			}else{
				m_cameradlg->ShowWindow( SW_SHOWNORMAL );
				::CheckMenuItem( m_appmenu, IDMENU_CAMERA, MF_CHECKED );				

				RECT oldrect;
				m_cameradlg->GetWindowRect( &oldrect );
				int winwidth, winheight;
				winwidth = oldrect.right - oldrect.left;
				winheight = oldrect.bottom - oldrect.top;
				m_cameradlg->SetWindowPos( HWND_TOP, g_inidata->camdlg_posx, g_inidata->camdlg_posy, winwidth, winheight, SWP_NOSIZE | SWP_NOZORDER );

				g_inidata->camdlg_disp = 1;
			}
			break;
		case IDMENU_TREEEDIT:
			if( g_inidata->sigdlg_disp ){
				m_sigeditwnd->ShowWindow( SW_HIDE );
				::CheckMenuItem( m_appmenu, IDMENU_TREEEDIT, MF_UNCHECKED );

				if( lParam == 0 ){
					RECT newrect;
					m_sigeditwnd->GetWindowRect( &newrect );
					g_inidata->sigdlg_posx = newrect.left;
					g_inidata->sigdlg_posy = newrect.top;
				}

				g_inidata->sigdlg_disp = 0;
			}else{
				m_sigeditwnd->ShowWindow( SW_SHOWNORMAL );
				::CheckMenuItem( m_appmenu, IDMENU_TREEEDIT, MF_CHECKED );

				RECT oldrect;
				m_sigeditwnd->GetWindowRect( &oldrect );
				int winwidth, winheight;
				winwidth = oldrect.right - oldrect.left;
				winheight = oldrect.bottom - oldrect.top;
				m_sigeditwnd->SetWindowPos( HWND_TOP, g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, winwidth, winheight, SWP_NOSIZE | SWP_NOZORDER );
				//m_sigeditwnd->MoveWindow( g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, winwidth, winheight, 1 );

				g_inidata->sigdlg_disp = 1;
			}
			break;
		case IDMENU_MOTEDIT:
			if( g_inidata->motdlg_disp ){
				//g_motdlgptr->HideDlg();
				//g_motdlg->ShowWindow( SW_HIDE );
				//g_motdlg->SetDisplayFlag( 0 );
				g_motdlg->ShowDlg( SW_HIDE );
				::CheckMenuItem( m_appmenu, IDMENU_MOTEDIT, MF_UNCHECKED );

				if( lParam == 0 ){
					RECT newrect;
					g_motdlg->GetWindowRect( &newrect );
					g_inidata->motdlg_posx = newrect.left;
					g_inidata->motdlg_posy = newrect.top;
				}

				g_inidata->motdlg_disp = 0;
			}else{
				if( m_refuge_motdlg == 0 ){
					//g_motdlgptr->StartDlg( (long)m_hWnd );
					g_motdlg->SetAppWnd( m_hWnd );
					//g_motdlg->ShowWindow( SW_SHOWNORMAL );
					//g_motdlg->SetDisplayFlag( 1 );
					g_motdlg->ShowDlg( SW_SHOW );

					::CheckMenuItem( m_appmenu, IDMENU_MOTEDIT, MF_CHECKED );

					RECT oldrect;
					g_motdlg->GetWindowRect( &oldrect );
					int winwidth, winheight;
					winwidth = oldrect.right - oldrect.left;
					winheight = oldrect.bottom - oldrect.top;
					g_motdlg->SetWindowPos( HWND_TOP, g_inidata->motdlg_posx, g_inidata->motdlg_posy, winwidth, winheight, SWP_NOSIZE | SWP_NOZORDER );


					g_inidata->motdlg_disp = 1;
				}else{
					::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
					::MessageBox( m_hWnd, 
						(LPCTSTR)"SIG Params ダイアログを閉じてから、再試行してください。",
						(LPCTSTR)"MotEditダイアログは避難中。",
						MB_OK );
					::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
				}
			}
			break;
		case 7000:
			if( m_loadnamedlg ){
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
				m_loadnamedlg->DoModal();
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			}

			break;

		case IDMENU_OPTION:
			int ret;
			//int bonedisp;
			int polydisp;
			//if( m_shandler && m_optdlg ){
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
			ret = (int)m_optdlg->DoModal( m_hWnd );
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			if( ret == IDOK ){

				SetBoneDisp();

				polydisp = m_optdlg->m_polydisp;
				if( polydisp )
					g_inidata->opt_polydisp = POLYDISP_ALL;
				else
					g_inidata->opt_polydisp = POLYDISP_SEL;

				g_inidata->opt_gdisp = m_optdlg->m_grounddisp;

				g_inidata->opt_jointsize = m_optdlg->m_jointsize;
				g_inidata->opt_jointalpha = m_optdlg->m_jointalpha;
				g_inidata->opt_undodlg = m_optdlg->m_undodlg;
				g_inidata->opt_bmdisp = m_optdlg->m_bmdisp;
				g_inidata->opt_previewfps = m_optdlg->m_previewfps;
				g_inidata->opt_motstep = m_optdlg->m_motstep;
				g_inidata->opt_ikfk = m_optdlg->m_ikfk;
				g_inidata->opt_iktdisp = m_optdlg->m_iktdisp;

				g_inidata->opt_MouseL = m_optdlg->m_MouseL;
				g_inidata->opt_MouseR = m_optdlg->m_MouseR;
				g_inidata->opt_MouseW = m_optdlg->m_MouseW;

				g_inidata->opt_unselline = m_optdlg->m_UnselLine;
				g_inidata->opt_selline = m_optdlg->m_SelLine;
				g_inidata->opt_unselpoint = m_optdlg->m_UnselPoint;
				g_inidata->opt_selpoint = m_optdlg->m_SelPoint;
				g_inidata->opt_pointsize = m_optdlg->m_PointSize;

				if( g_motdlg ){
					ret = g_motdlg->SetMotionStep( g_inidata->opt_motstep );
					if( ret ){
						DbgOut( "viewer : IDMENU_OPTION : motdlg SetMotionStep error !!!\n" );
						_ASSERT( 0 );
					}
				}

				//DbgOut( "viewer : optdlg : bmdisp %d\r\n", g_inidata->opt_bmdisp );

			}
				/***
			}else{
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
				::MessageBox( m_hWnd,
					"形状データを読み込んでから再試行して下さい。", 
					"形状データがありません。", 
					MB_OK );
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

			}
			***/
			break;
		case IDMENU_WRITE_BMP:
			if( g_motdlg ){
				//m_bReady = FALSE;

				if( m_writebmpflag == 1 )
					return 0;

				m_writebmpflag = 1;


				int ret = 0;
				ret = g_motdlg->WriteBmp( m_pd3dDevice, m_pFont );
				if( ret ){
					errormes.errorcode = -1;
					//errormes.type = MSGERR_APPMUSTEXIT;
					errormes.type = MSG_NONE;
					errormes.mesptr = "MsgProc : IDMENU_WRITE_BMP error";
					SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
					m_writebmpflag = 0;
					return -1;
				}
				m_writebmpflag = 0;

			}
			return 0;


			break;

		case 40000: // AVI出力
			if( g_motdlg ){
				//m_bReady = FALSE;

				if( m_writebmpflag == 1 )
					return 0;

				m_writebmpflag = 1;

				int ret = 0;
				ret = g_motdlg->WriteAvi( m_pd3dDevice, m_pFont );
				if( ret ){
					errormes.errorcode = -1;
					//errormes.type = MSGERR_APPMUSTEXIT;
					errormes.type = MSG_NONE;
					errormes.mesptr = "MsgProc :  : Avi write error";
					SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
					m_writebmpflag = 0;
					return -1;
				}
				//m_bReady = TRUE;
				m_writebmpflag = 0;

			}
			return 0;

			break;

		case 40030:
			if( g_motdlg ){
				int ret;
				ret = SaveXFile();
				if( ret ){
					errormes.errorcode = -1;
					//errormes.type = MSGERR_APPMUSTEXIT;
					errormes.type = MSG_NONE;
					errormes.mesptr = "MsgProc :  : X File save error";
					SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
					return -1;
				}
			}
			return 0;

			break;

		//case 40001://bim out
		//	ret = WriteBIMFile();
		//	if( ret ){
		//		errormes.errorcode = -1;
		//		//errormes.type = MSGERR_APPMUSTEXIT;
		//		errormes.type = MSG_NONE;
		//		errormes.mesptr = "MsgProc :  : BIM write error";
		//		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		//		return -1;			
		//	}
		//	break;

		//case 40013://bim in
		//	ret = LoadBIMFile();

		//	//_ASSERT( 0 );

		//	if( ret ){
		//		errormes.errorcode = -1;
		//		//errormes.type = MSGERR_APPMUSTEXIT;
		//		errormes.type = MSG_NONE;
		//		errormes.mesptr = "MsgProc :  : BIM Load error";
		//		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		//		return -1;			
		//	}

		//	//return 0;//!!!!!!!!!!!!!

		//	break;

		case 40001://im2 out
			ret = WriteIM2File();
			if( ret ){
				errormes.errorcode = -1;
				//errormes.type = MSGERR_APPMUSTEXIT;
				errormes.type = MSG_NONE;
				errormes.mesptr = "MsgProc :  : IM2 write error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				return -1;			
			}
			break;

		case 40010://im2 in
			ret = LoadIM2File( 0 );
			if( ret ){
				errormes.errorcode = -1;
				//errormes.type = MSGERR_APPMUSTEXIT;
				errormes.type = MSG_NONE;
				errormes.mesptr = "MsgProc :  : IM2 load error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				return -1;			
			}
			break;

		case IDMENU_IKUNDO:
			if( g_motdlg ){
				int undono, undomax;
				
				if( m_undo_init_flag == 0 )
					m_undo_cnt++;
				m_undo_init_flag = 0;

				undomax = min( m_ik_cnt, MAXUNDONUM );
				undono = abs ( m_undo_cnt ) % undomax;

				ret = g_motdlg->PasteUndoBuffer( undono, m_optdlg->m_undodlg );
				_ASSERT( !ret );

			}
			break;
		case IDMENU_REVIKUNDO:
			if( g_motdlg ){
				int undono, undomax;

				if( m_undo_init_flag == 0 ){
					m_undo_cnt--;
				}
				m_undo_init_flag = 0;

				undomax = min( m_ik_cnt, MAXUNDONUM );
				if( m_undo_cnt < 0 ){
					m_undo_cnt = undomax + m_undo_cnt;
				}
				undono = abs( m_undo_cnt ) % undomax;

				ret = g_motdlg->PasteUndoBuffer( undono, m_optdlg->m_undodlg );
				_ASSERT( !ret );

			}
			break;

		case 5001:
			ret = WriteBoneFile();
			if( ret ){
				_ASSERT( 0 );
				::MessageBox( m_hWnd, "ボーンファイルの書き出しに失敗しました。", "失敗しました。", MB_OK );
			}

			break;
		case 5002:
			ret = LoadBoneFile( 0 );
			if( ret ){
				_ASSERT( 0 );
				::MessageBox( m_hWnd, "ボーンファイルの読み込みに失敗しました。", "失敗しました。ファイルを確認してください。", MB_OK );
			}
			break;

		case 5005:

			if( m_writebmpflag == 1 )
				return 0;

			m_writebmpflag = 1;
			//::EnableMenuItem( m_appmenu, 5005, 0 );

			ret = WriteMQOFile();
			if( ret ){
				_ASSERT( 0 );
				::MessageBox( m_hWnd, "連番ＭＱＯファイルの書き出しに失敗しました。", "失敗しました", MB_OK );
			}

			m_writebmpflag = 0;

			//::EnableMenuItem( m_appmenu, 5005, 1 );

			break;

		case 51000://パースペクティブ
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
			ret = (int)m_parsdlg->DoModal( m_hWnd );
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

			break;
		case 51001://面マテリアル

			m_materialdlg->ShowDlg( SW_SHOW );

			break;
		case 51002://法線マップ
			ret = ConvNormalMap();
			_ASSERT( !ret );
			break;
		case IDTBB_R:

			ValidSelectCircle( 1 );
			
			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_ExistData ){
				if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

					ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
					if( ret == IDOK ){
						SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
					}
				}else if (m_shandler && m_tbb_ei2 ){
					ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
					if( ret == IDOK ){
						SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
					}

				}
			}

			if( m_tbb_r ){
				//none
			}else{
				m_tbb_r = 1;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();


			break;
		case IDTBB_T:
			
			ValidSelectCircle( 0 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_tbb_t ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 1;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();


			break;
		case IDTBB_T2:
			ValidSelectCircle( 0 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_tbb_t2 ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 1;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();
			break;

		case IDTBB_S:

			ValidSelectCircle( 0 );
			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_tbb_s ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 1;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();

			break;
		case IDTBB_ITI:

			ValidSelectCircle( 0 );

			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && m_tbb_bone ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}


			if( m_tbb_iti ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 1;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();


////////////////
			if( m_tbb_iti == 1 ){

				if( m_ExistData ){

					m_itimodedlg->DoModal();

					if( m_itimodedlg->m_itimode == 0 ){
						//motionの削除
						ret = (int)MessageBox( m_hWnd, "このモードにすると、読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
						if( ret != IDOK ){
							
							MessageBox( m_hWnd, "キャンセルされたので、編集モードを、回転(R)に変更します。", "キャンセル", MB_OK );
								
							SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
						}else{
							/***
							if( m_mhandler ){

								if( g_motdlg )
									g_motdlg->InvalidateHandler();

								ret = m_mhandler->DestroyMotionObj();
								if( ret ){
									DbgOut( "viewer : lbutton : mh DestroyMotionObj error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}

								if( g_motdlg ){
									ret = g_motdlg->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
										(long)m_hWnd, this );
								}

								if( g_motdlg ){
									ret = g_motdlg->OnStop();
									_ASSERT( !ret );
								}

							}
							***/
							ret = DestroyMotion( 1 );
							if( ret ){
								DbgOut( "viewer : ITDTBB_ITI : DestroyMotion error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}

							m_itidlg->ShowDlg( SW_SHOW );
							m_itidlg->SetItiMode( m_itimodedlg->m_itimode );

							m_tbb_face = 0;
							SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_FACE, 0 );
							m_tbb_line = 0;
							SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_LINE, 0 );
							m_tbb_point = 0;
							SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_POINT, 0 );
						}
					}else{
						m_itidlg->ShowDlg( SW_SHOW );
						m_itidlg->SetItiMode( m_itimodedlg->m_itimode );

						m_tbb_face = 0;
						SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_FACE, 0 );
						m_tbb_line = 0;
						SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_LINE, 0 );
						m_tbb_point = 0;
						SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_POINT, 0 );
					}

				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}

			break;

		case IDTBB_IKT:

			m_selectedno = 0;//!!!!!!!!!!!!!!!!!!!

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_ExistData ){
				int seri;
				CShdElem* selem;
				int iktcnt = 0;
				for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
					selem = (*m_shandler)( seri );
					if( selem->IsJoint() && (selem->type != SHDMORPH) && (selem->m_iktarget != 0) ){
						iktcnt++;
					}
				}

				if( iktcnt <= 0 ){
					MessageBox( m_hWnd, "有効にしているＩＫターゲットが、１つもありません。\n現在、このモードで編集できるオブジェクトはありません。", "有効データ無し", MB_OK );
				}
			}


			g_inidata->opt_iktdisp = 1;//!!!!!!!!!!!!!
			m_optdlg->m_iktdisp = 1;


			ValidSelectCircle( 0 );

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_tbb_ikt ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 1;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();
			break;

		case IDTBB_COL:
			
			ValidSelectCircle( 0 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_shandler && m_shandler->m_shader == COL_TOON1 ){
				MessageBox( m_hWnd, "トゥーン色では、この編集は行えません。「色P」に切り替えます。", "色の切り替え", MB_OK );
				m_tbb_colP = 0;
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );

			}


			if( m_tbb_col ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 1;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();

			if( m_tbb_col == 1 ){
				if( m_ExistData ){

					m_setcoldlg->ShowDlg( SW_SHOW );

					m_tbb_face = 0;
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_FACE, 0 );
					//m_tbb_line = 0;
					//SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_LINE, 0 );
					m_tbb_point = 0;
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_POINT, 0 );

				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}
			break;

		case IDTBB_BONE:

			ValidSelectCircle( 0 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}


			if( m_shandler && ( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}


			if( m_tbb_bone ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 1;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();


////////////////
			if( m_tbb_bone == 1 ){

				if( m_ExistData ){

					//motionの削除
					ret = (int)MessageBox( m_hWnd, "このモードにすると、読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
					if( ret != IDOK ){
							
						MessageBox( m_hWnd, "キャンセルされたので、編集モードを、回転(R)に変更します。", "キャンセル", MB_OK );
								
						SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
					}else{
						/***
						if( m_mhandler ){

							if( g_motdlg )
								g_motdlg->InvalidateHandler();

							ret = m_mhandler->DestroyMotionObj();
							if( ret ){
								DbgOut( "viewer : lbutton : mh DestroyMotionObj error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}

							if( g_motdlg ){
								ret = g_motdlg->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
									(long)m_hWnd, this );
							}

							if( g_motdlg ){
								ret = g_motdlg->OnStop();
								_ASSERT( !ret );
							}

						}
						***/
						ret = DestroyMotion( 1 );
						if( ret ){
							DbgOut( "viewer : ITDTBB_BONE : DestroyMotion error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}


						m_bonedlg->ShowDlg( SW_SHOW );

						//ボーン表示をオン
						m_optdlg->m_bonedisp = 1;
						g_inidata->opt_jointdisp = 1;

						m_optdlg->m_bmdisp = 1;
						g_inidata->opt_bmdisp = 1;
						SetBoneDisp();

						//ツリービューを出す
						if( g_inidata->sigdlg_disp == 0 ){
							::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_TREEEDIT, 0 );
						}


					}
				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}

			break;

		case IDTBB_EI1:
			
			ValidSelectCircle( 0 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_tbb_ei1 ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 1;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();

			if( m_tbb_ei1 == 1 ){
				if( m_ExistData ){

					CInfluenceDlg dlg( m_thandler, m_shandler, m_hWnd );

					::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
					dlg.DoModal();
					::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

					MessageBox( m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}


			break;

		case IDTBB_EI2:

			if( m_tbb_ei2 ){
				return 0;//!!!!!!!!!! 複数回 連続で呼ばれないように。
			}

			ValidSelectCircle( 0 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}

			if( m_tbb_ei2 ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 1;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();

			if( m_tbb_ei2 == 1 ){
				if( m_ExistData ){

					ret = (int)MessageBox( m_hWnd, "このモードにすると、読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
					if( ret != IDOK ){
							
						MessageBox( m_hWnd, "キャンセルされたので、編集モードを、回転(R)に変更します。", "キャンセル", MB_OK );
								
						SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
					}else{
						ret = DestroyMotion( 1 );
						if( ret ){
							DbgOut( "viewer : ITDTBB_EI2 : DestroyMotion error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}

		
						m_infscopedlg->ShowDlg( SW_SHOW );

						if( g_motdlg ){
							int curjoint = 0;
							D3DXVECTOR3 curjointpos;
							ret = g_motdlg->GetSelectedJointPos( &curjoint, &curjointpos );
							if( (ret == 0) && (curjoint > 0)  ){
								m_infscopedlg->OnUserSelChange( curjoint );
							}
						}

					}

				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}

			break;

		case IDTBB_EI3:
			
			if( m_tbb_ei3 ){
				return 0;//!!!!!!!!!! RestoreMaterialが複数回呼ばれないように。
			}

			ValidSelectCircle( 1 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_shandler && m_shandler->m_shader == COL_TOON1 ){
				MessageBox( m_hWnd, "トゥーン色では、この編集は行えません。「色P」に切り替えます。", "色の切り替え", MB_OK );
				m_tbb_colP = 0;
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );

			}

			if( m_tbb_ei3 ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 1;
				m_tbb_toon1 = 0;
				m_tbb_mo = 0;
			}
			CheckTBB();

			if( m_tbb_ei3 == 1 ){
				if( m_ExistData ){
		

					m_paintdlg->ShowDlg( SW_SHOW );

					m_tbb_face = 0;
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_FACE, 0 );
					//m_tbb_line = 0;
					//SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_LINE, 0 );
					m_tbb_point = 0;
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_POINT, 0 );

					if( g_motdlg ){
						int curjoint = 0;
						D3DXVECTOR3 curjointpos;
						ret = g_motdlg->GetSelectedJointPos( &curjoint, &curjointpos );
						if( (ret == 0) && (curjoint > 0)  ){
							m_paintdlg->OnUserSelChange( curjoint );
						}
					}

				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}

			break;
		case IDTBB_TOON1:
			
			ValidSelectCircle( 0 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_mo ){
				m_mcdlg->ShowDlg( SW_HIDE );
			}


			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			m_tbb_colT = 0;
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );/// トゥーン表示に切り替え


			if( m_tbb_toon1 ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 1;
				m_tbb_mo = 0;
			}
			CheckTBB();

			if( m_tbb_toon1 == 1 ){
				if( m_ExistData ){

					m_toon1dlg->ShowDlg( SW_SHOW );

				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}

			break;

		case IDTBB_MO:
			if( m_tbb_mo ){
				return 0;//!!!!!!!!!! 複数回呼ばれないように。
			}

			ValidSelectCircle( 1 );

			if( m_tbb_iti ){
				m_itidlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_col ){
				m_setcoldlg->ShowDlg( SW_HIDE );
			}
			if( m_tbb_bone ){
				m_bonedlg->ShowDlg( SW_HIDE );
			}
			if (m_tbb_ei2 ){
				m_infscopedlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_ei3 ){
				m_paintdlg->ShowDlg( SW_HIDE );
				m_selectedno = 0;
			}
			if( m_tbb_toon1 ){
				m_toon1dlg->ShowDlg( SW_HIDE );
			}

			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){

				ret = (int)MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}else if (m_shandler && m_tbb_ei2 ){
				ret = (int)MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}

			if( m_tbb_mo ){
				//none
			}else{
				m_tbb_r = 0;
				m_tbb_t = 0;
				m_tbb_t2 = 0;
				m_tbb_s = 0;
				m_tbb_iti = 0;
				m_tbb_ikt = 0;
				m_tbb_col = 0;
				m_tbb_bone = 0;
				m_tbb_ei1 = 0;
				m_tbb_ei2 = 0;
				m_tbb_ei3 = 0;
				m_tbb_toon1 = 0;
				m_tbb_mo = 1;
			}
			CheckTBB();

			if( m_tbb_mo == 1 ){
				if( m_ExistData ){
		

					m_mcdlg->ShowDlg( SW_SHOW );

				}else{
					MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );

					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}
			}
			break;


		case IDTBB_FACE:
			if( m_tbb_face ){
				m_tbb_face = 0;
			}else{
				m_tbb_face = 1;
			}
			CheckTBB();

			break;
		case IDTBB_LINE:
			if( m_tbb_line ){
				m_tbb_line = 0;
			}else{
				m_tbb_line = 1;
			}
			CheckTBB();

			break;
		case IDTBB_POINT:
			if( m_tbb_point ){
				m_tbb_point = 0;
			}else{
				m_tbb_point = 1;
				m_tbb_ten2 = 0;
			}
			CheckTBB();

			break;
		case IDTBB_TEN2:
			if( m_tbb_ten2 ){
				m_tbb_ten2 = 0;
			}else{
				m_tbb_ten2 = 1;
				m_tbb_point = 0;
			}
			CheckTBB();
			break;

		case IDTBB_ZA1:
			if( m_tbb_za1 ){
				//none
			}else{
				m_tbb_za1 = 1;
				m_tbb_za2 = 0;
				m_tbb_za3 = 0;
			}
			m_za_kind = ZA_1;
			CheckTBB();

			if( m_ExistData ){
				ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );
				_ASSERT( !ret );
			}

			break;

		case IDTBB_ZA2:
			if( m_tbb_za2 ){
				//none
			}else{
				m_tbb_za1 = 0;
				m_tbb_za2 = 1;
				m_tbb_za3 = 0;
			}
			m_za_kind = ZA_2;
			CheckTBB();

			if( m_ExistData ){
				ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );
				_ASSERT( !ret );
			}


			break;

		case IDTBB_ZA3:
			if( m_tbb_za3 ){
				//none
			}else{
				m_tbb_za1 = 0;
				m_tbb_za2 = 0;
				m_tbb_za3 = 1;
			}
			m_za_kind = ZA_3;
			CheckTBB();

			if( m_ExistData ){
				ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );
				_ASSERT( !ret );
			}

			break;


		case IDTBB_COLP:
			if( m_tbb_colP ){
				//none
			}else{
				if( m_shandler ){
					m_shandler->m_shader = COL_PHONG;

					ret = m_shandler->SetIndexBufColP();
					if( ret ){
						DbgOut( "viewer : ColorChange : IDTBB_COLP : COL_PHONG : sh SetIndexBufColP error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}

				m_tbb_colP = 1;
				m_tbb_colT = 0;
			}
			CheckTBB();
			break;
		case IDTBB_COLT:
			if( m_tbb_colT ){
				//none
			}else{
				if( m_shandler ){
					m_shandler->m_shader = COL_TOON1;

					ret = m_shandler->CreateToon1Buffer( m_pd3dDevice );
					if( ret ){
						DbgOut( "viewer : ColorChange : IDTBB_COLT : COL_TOON1 : sh CreateToon1Buffer error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}

				m_tbb_colP = 0;
				m_tbb_colT = 1;
			}
			CheckTBB();
			break;


/***
		case IDTBB_G:
			if( m_tbb_g ){
				//none
			}else{
				m_tbb_g = 1;
				m_tbb_l = 0;
			}
			CheckTBB();
			break;
		case IDTBB_L:
			if( m_tbb_l ){
				//none
			}else{
				m_tbb_g = 0;
				m_tbb_l = 1;
			}
			CheckTBB();
			break;
***/
////////////////
		// RMENU
		case ID_RMENU_IKTARGET:
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
			s_rmenumain->SetIKTargetParams();
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			break;
		case ID_RMENU_IKTRANS:
			s_rmenumain->ToggleIKTrans();
			break;
		case ID_RMENU_IKSKIP:
			s_rmenumain->ToggleIKSkip();
			break;

		case ID_RMENU_SYMX:
			CopyUndoBuffer();
			s_rmenumain->SetSymMotionPoint( SYMMTYPE_X );
			break;
		case ID_RMENU_SYMY:
			CopyUndoBuffer();
			s_rmenumain->SetSymMotionPoint( SYMMTYPE_Y );
			break;
		case ID_RMENU_SYMZ:
			CopyUndoBuffer();
			s_rmenumain->SetSymMotionPoint( SYMMTYPE_Z );
			break;

		case ID_RMENU_BONEMARK:
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
			m_bonelinedlg->SetSelemParam( s_rmenumain->m_curselem );
			ret = (int)m_bonelinedlg->DoModal();
			if( ret == IDOK ){
				g_inidata->selectcol = m_bonelinedlg->m_selcol;
				g_inidata->unselectcol = m_bonelinedlg->m_unselcol;
			}
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			break;
		case ID_RMENU_COPY:
			g_motdlg->CopyLowerMotion( s_rmenumain->m_curselem );
			break;
		case ID_RMENU_SIGPARAMS:
			m_sigeditwnd->OnMenuShdParam( 0, 0, 0, dummy );
			break;
		case ID_RMENU_MODE_JOINT:
		case ID_RMENU_MODE_OPE:
//DbgOut( "viewer : wm_command : rmenu mode %d\r\n", LOWORD( wParam ) );
			s_rmenumain->SetIKMode( LOWORD(wParam) );
			break;
		case ID_RMENU_LEVEL1:
		case ID_RMENU_LEVEL2:
		case ID_RMENU_LEVEL3:
		case ID_RMENU_LEVEL4:
		case ID_RMENU_LEVEL5:
		case ID_RMENU_LEVEL6:
		case ID_RMENU_LEVEL7:
		case ID_RMENU_LEVEL8:
		case ID_RMENU_LEVEL9:
		case ID_RMENU_LEVEL10:
		case ID_RMENU_LEVEL11:
		case ID_RMENU_LEVEL12:
		case ID_RMENU_LEVEL13:
		case ID_RMENU_LEVEL14:
		case ID_RMENU_LEVEL15:
		case ID_RMENU_LEVELINF:

//DbgOut( "viewer : wm_command : rmenu level %d\r\n", LOWORD( wParam ) );
			s_rmenumain->SetIKLevel( LOWORD(wParam) );
			break;

		case ID_RMENU_IKDRAG:
			s_rmenumain->SetIKFK( 0 );
			g_inidata->opt_ikfk = 0;
			m_optdlg->m_ikfk = 0;
			break;

		case ID_RMENU_FKDRAG:
			s_rmenumain->SetIKFK( 1 );
			g_inidata->opt_ikfk = 1;
			m_optdlg->m_ikfk = 1;
			break;

		case ID_RMENU_BONEPROP:
			g_bonepropdlg->UpdateParams( s_rmenumain->m_curselem->serialno, m_za_kind );
			g_bonepropdlg->ShowDlg( SW_SHOW );
			break;

////////////
//////////// accelerator
		//}else if( (downkey == 'Z') && (state == 0) && (m_control_key == 1) && (m_shift_key == 1) ){

		case IDM_UP:
		case IDM_DOWN:
		case IDM_RIGHT:
		case IDM_LEFT:
			SelChangeBone( menuid );
			break;

		case IDM_ZOOM:
			
//DbgOut( "viewer : IDM_ZOOM\r\n" );
			break;

		case IDM_GDISP:

			if( m_g_disp == 0 ){
				m_g_disp = 1;
			}else{
				m_g_disp = 0;
			}

			break;
		case IDM_CTRLT:
			DisableIKT();
			break;
		case IDM_IKTON:
			IKTON();
			break;
		case IDM_IKTOFF:
			IKTOFF();
			break;

		case IDM_CTRLB:
			if( *(m_loadnamedlg->m_im2name) ){
				LoadIM2File( 1 );
			}

			break;
		case IDM_CTRLX:
		    
			if( g_motdlg ){
				if( m_ik_cnt > 0 ){
					int undono, undomax;

					if( m_undo_init_flag == 0 ){
						m_undo_cnt--;
					}
					m_undo_init_flag = 0;

					undomax = min( m_ik_cnt, MAXUNDONUM );

					if( m_undo_cnt < 0 ){
						m_undo_cnt = undomax + m_undo_cnt;
					}
					undono = abs( m_undo_cnt ) % undomax;

					ret = g_motdlg->PasteUndoBuffer( undono, m_optdlg->m_undodlg );
					_ASSERT( !ret );
				}
			}
			break;

		case IDM_CTRLZ:

			if( g_motdlg ){
				if( m_ik_cnt > 0 ){
					int undono, undomax;
					
					if( m_undo_init_flag == 0 )
						m_undo_cnt++;

					m_undo_init_flag = 0;

					undomax = min( m_ik_cnt, MAXUNDONUM );
					undono = abs( m_undo_cnt ) % undomax;

					ret = g_motdlg->PasteUndoBuffer( undono, m_optdlg->m_undodlg );
					_ASSERT( !ret );
				}
			}
			break;

		case IDM_CTRLJ:
			//ジョイントの表示、非表示の切り替え
			
			ToggleBoneDisp();
			break;

		case IDM_CTRLC:
			ret = g_motdlg->OnFrameCopy();
			_ASSERT( !ret );
			break;

		case IDM_CTRLV:
			ret = g_motdlg->OnFramePaste();
			_ASSERT( !ret );
			break;
		case IDM_OVERWRITE:
			ret = OverWriteFiles();
			_ASSERT( !ret );
			break;


		case IDM_HOME:
			nmud.iDelta = -10000;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
			break;

		case IDM_END:
			nmud.iDelta = 10000;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
			break;

		case IDM_PRIOR:
			nmud.iDelta = -1;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
			break;

		case IDM_NEXT:
			nmud.iDelta = 1;
			ret = g_motdlg->OnSpin_Sp_mp( 999, &nmud, dummy );
			_ASSERT( !ret );
		
			break;

		case IDM_CTRLUP:
			ret = g_motdlg->OnBeforeMP();
			_ASSERT( !ret );
			break;
		case IDM_CTRLDOWN:
			ret = g_motdlg->OnNextMP();
			_ASSERT( !ret );
			break;


		case IDM_PAUSE:
			if( m_preview_flag == 1 ){
				ret = g_motdlg->OnStop();
				_ASSERT( !ret );
			}else{
				ret = g_motdlg->OnPreview();
				_ASSERT( !ret );
			}

			break;
			
		case IDM_CTRLI:
			if( m_sigeditwnd ){
				BOOL dummy;
				m_sigeditwnd->OnMenuShdParam(0, 0, 0, dummy);
			}
			break;
/////////////
		default:
			break;
		}
	}


    return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}

//////////////////////////
int CMyD3DApplication::CreateGHandler()
{
////////// g
	m_g_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_g_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_g_th alloc error !!!\n" );
		return 1;
	}
	m_g_sh = new CShdHandler( m_g_th );
	if( !m_g_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : shandler alloc error !!!\n" );
		return 1;
	}
	m_g_mh = new CMotHandler( m_g_th );
	if( !m_g_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : mhandler alloc error !!!\n" );
		return 1;
	}

	m_g_sh->m_inRDBflag = 1;

	return 0;
}

int CMyD3DApplication::CreateGroundHandler()
{
	m_ground_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_ground_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_ground_th alloc error !!!\n" );
		return 1;
	}
	m_ground_sh = new CShdHandler( m_ground_th );
	if( !m_ground_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : shandler alloc error !!!\n" );
		return 1;
	}
	m_ground_mh = new CMotHandler( m_ground_th );
	if( !m_ground_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : mhandler alloc error !!!\n" );
		return 1;
	}

	m_ground_sh->m_inRDBflag = 1;

////////// arrow
	m_arrow_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_arrow_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_arrow_th alloc error !!!\n" );
		return 1;
	}
	m_arrow_sh = new CShdHandler( m_arrow_th );
	if( !m_arrow_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler arrow : shandler alloc error !!!\n" );
		return 1;
	}
	m_arrow_mh = new CMotHandler( m_arrow_th );
	if( !m_arrow_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler arrow : mhandler alloc error !!!\n" );
		return 1;
	}

	m_arrow_sh->m_inRDBflag = 1;

////////// select
	m_select_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_select_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_select_th alloc error !!!\n" );
		return 1;
	}
	m_select_sh = new CShdHandler( m_select_th );
	if( !m_select_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler select : shandler alloc error !!!\n" );
		return 1;
	}
	m_select_mh = new CMotHandler( m_select_th );
	if( !m_select_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler select : mhandler alloc error !!!\n" );
		return 1;
	}

	m_select_sh->m_inRDBflag = 1;

////////// ikt
	m_ikt_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_ikt_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_ikt_th alloc error !!!\n" );
		return 1;
	}
	m_ikt_sh = new CShdHandler( m_ikt_th );
	if( !m_ikt_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler ikt : shandler alloc error !!!\n" );
		return 1;
	}
	m_ikt_mh = new CMotHandler( m_ikt_th );
	if( !m_ikt_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler ikt : mhandler alloc error !!!\n" );
		return 1;
	}

	m_ikt_sh->m_inRDBflag = 1;
	
////////// sph
	m_sph_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_sph_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_sph_th alloc error !!!\n" );
		return 1;
	}
	m_sph_sh = new CShdHandler( m_sph_th );
	if( !m_sph_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler sph : shandler alloc error !!!\n" );
		return 1;
	}
	m_sph_mh = new CMotHandler( m_sph_th );
	if( !m_sph_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler sph : mhandler alloc error !!!\n" );
		return 1;
	}

	m_sph_sh->m_inRDBflag = 1;
	
////////// bm
	m_bm_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_bm_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_bm_th alloc error !!!\n" );
		return 1;
	}
	m_bm_sh = new CShdHandler( m_bm_th );
	if( !m_bm_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler bm : shandler alloc error !!!\n" );
		return 1;
	}
	m_bm_mh = new CMotHandler( m_bm_th );
	if( !m_bm_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler bm : mhandler alloc error !!!\n" );
		return 1;
	}

	m_bm_sh->m_inRDBflag = 1;
//////////// sq
	m_sq_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_sq_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_sq_th alloc error !!!\n" );
		return 1;
	}
	m_sq_sh = new CShdHandler( m_sq_th );
	if( !m_sq_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler sq : shandler alloc error !!!\n" );
		return 1;
	}
	m_sq_mh = new CMotHandler( m_sq_th );
	if( !m_sq_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler sq : mhandler alloc error !!!\n" );
		return 1;
	}

	m_sq_sh->m_inRDBflag = 1;


	return 0;
}

int CMyD3DApplication::DestroyGHandler()
{
	int ret;
//// g
	if( m_g_mh ){
		ret = m_g_mh->DestroyMotionObj();
	}

	if( m_g_th ){
		delete m_g_th;
		m_g_th = 0;
	}

	if( m_g_sh ){
		delete m_g_sh;
		m_g_sh = 0;
	}

	if( m_g_mh ){
		delete m_g_mh;
		m_g_mh = 0;
	}

	return 0;
}

int CMyD3DApplication::DestroyGroundHandler()
{
	int ret;
	if( m_ground_mh ){
		ret = m_ground_mh->DestroyMotionObj();
	}

	if( m_ground_th ){
		delete m_ground_th;
		m_ground_th = 0;
	}

	if( m_ground_sh ){
		delete m_ground_sh;
		m_ground_sh = 0;
	}

	if( m_ground_mh ){
		delete m_ground_mh;
		m_ground_mh = 0;
	}

	
//// arrow
	if( m_arrow_mh ){
		ret = m_arrow_mh->DestroyMotionObj();
	}

	if( m_arrow_th ){
		delete m_arrow_th;
		m_arrow_th = 0;
	}

	if( m_arrow_sh ){
		delete m_arrow_sh;
		m_arrow_sh = 0;
	}

	if( m_arrow_mh ){
		delete m_arrow_mh;
		m_arrow_mh = 0;
	}
///// select
	if( m_select_mh ){
		ret = m_select_mh->DestroyMotionObj();
	}

	if( m_select_th ){
		delete m_select_th;
		m_select_th = 0;
	}

	if( m_select_sh ){
		delete m_select_sh;
		m_select_sh = 0;
	}

	if( m_select_mh ){
		delete m_select_mh;
		m_select_mh = 0;
	}

///// ikt
	if( m_ikt_mh ){
		ret = m_ikt_mh->DestroyMotionObj();
	}

	if( m_ikt_th ){
		delete m_ikt_th;
		m_ikt_th = 0;
	}

	if( m_ikt_sh ){
		delete m_ikt_sh;
		m_ikt_sh = 0;
	}

	if( m_ikt_mh ){
		delete m_ikt_mh;
		m_ikt_mh = 0;
	}
///// sph
	if( m_sph_mh ){
		ret = m_sph_mh->DestroyMotionObj();
	}

	if( m_sph_th ){
		delete m_sph_th;
		m_sph_th = 0;
	}

	if( m_sph_sh ){
		delete m_sph_sh;
		m_sph_sh = 0;
	}

	if( m_sph_mh ){
		delete m_sph_mh;
		m_sph_mh = 0;
	}
	
//// bm
	if( m_bm_mh ){
		ret = m_bm_mh->DestroyMotionObj();
	}

	if( m_bm_th ){
		delete m_bm_th;
		m_bm_th = 0;
	}

	if( m_bm_sh ){
		delete m_bm_sh;
		m_bm_sh = 0;
	}

	if( m_bm_mh ){
		delete m_bm_mh;
		m_bm_mh = 0;
	}
///// sq
	if( m_sq_mh ){
		ret = m_sq_mh->DestroyMotionObj();
	}

	if( m_sq_th ){
		delete m_sq_th;
		m_sq_th = 0;
	}

	if( m_sq_sh ){
		delete m_sq_sh;
		m_sq_sh = 0;
	}

	if( m_sq_mh ){
		delete m_sq_mh;
		m_sq_mh = 0;
	}

	return 1;
}

int CMyD3DApplication::CreateExtHandler()
{

	m_ext_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_ext_th ){
		DbgOut( "CD3DMyApplication : CreateHandler : m_ext_th alloc error !!!\n" );
		return 1;
	}
	m_ext_sh = new CShdHandler( m_ext_th );
	if( !m_ext_sh ){
		DbgOut( "CD3DMyApplication : CreateShdHandler : m_ext_sh alloc error !!!\n" );
		return 1;
	}

	m_ext_sh->m_inRDBflag = 1;//!!!!!

	m_ext_mh = new CMotHandler( m_ext_th );
	if( !m_ext_mh ){
		DbgOut( "CD3DMyApplication : CreateHandler : m_ext_mh alloc error !!!\n" );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::DestroyExtHandler()
{
	/////// extract
	if( m_ext_th ){
		delete m_ext_th;
		m_ext_th = 0;
	}
	if( m_ext_sh ){
		delete m_ext_sh;
		m_ext_sh = 0;
	}
	if( m_ext_mh ){
		delete m_ext_mh;
		m_ext_mh = 0;
	}
	return 0;
}

int CMyD3DApplication::CreateHandler()
{

	m_thandler = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_thandler ){
		DbgOut( "CD3DMyApplication : CreateHandler : m_thandler alloc error !!!\n" );
		return 1;
	}
	m_shandler = new CShdHandler( m_thandler );
	if( !m_shandler ){
		DbgOut( "CD3DMyApplication : CreateShdHandler : shandler alloc error !!!\n" );
		return 1;
	}

	m_shandler->m_inRDBflag = 1;//!!!!!

	m_mhandler = new CMotHandler( m_thandler );
	if( !m_mhandler ){
		DbgOut( "CD3DMyApplication : CreateHandler : mhandler alloc error !!!\n" );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::DestroyHandler()
{
	DbgOut( "CMyD3DApplication : DestroyHandler\n" );

	int ret;

	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();


	if( m_mhandler && g_motdlg ){
		//mothandlerのdestroyの前に、必ず呼ぶ。
		DbgOut( "MyD3DApplication : FinalCleanup : DestroyMotionObj\n" );
		
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : DestroyHandler : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		
		ret = m_mhandler->DestroyMotionObj();
		//g_motdlg->DestroyMotionObj();
	}


	if( m_shandler && g_texbnk ){
		ret = g_texbnk->ResetDirtyFlag();
		_ASSERT( !ret );

		ret = g_texbnk->DestroyNotDirtyTexture( 1 );
		_ASSERT( !ret );

	}

	if( m_thandler ){
		delete m_thandler;
		m_thandler = 0;
	}

	if( m_shandler ){
		delete m_shandler;
		m_shandler = 0;
	}

	if( m_mhandler ){
		delete m_mhandler;
		m_mhandler = 0;
	}
	return 1;
}


/***
int CMyD3DApplication::ChangeMotionStep()
{
	DWORD curstep;
	int ret;

	curstep = s_MotStep;

    if( IDOK == DialogBoxParam( (HINSTANCE)GetModuleHandle(NULL),
                                MAKEINTRESOURCE(IDD_CHANGEMOTSTEP),
                               GetForegroundWindow(),
                                ChangeMotStepProc, 0 ) ){
		
		if( curstep != s_MotStep ){
			ret = SetMotionStep();
			_ASSERT( !ret );
		}

        return S_OK;
	}

    return -1;
}
int CMyD3DApplication::SetMotionStep()
{
	int ret;
	
	//s_MotStep は{"1","2","3"}のindex

	if( m_motcookie0 >= 0 ){
		ret = m_mhandler->SetMotionStep( m_motcookie0, s_MotStep + 1 );
		_ASSERT( !ret );
	}

	if( m_motcookie1 >= 0 ){
		ret = m_mhandler->SetMotionStep( m_motcookie1, s_MotStep + 1 );
		_ASSERT( !ret );
	}

	return ret;
}
***/
///////////////
/***
BOOL CALLBACK ChangeMotStepProc( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    static DWORD dwMotStep;

    // Handle the initialization message
    if( WM_INITDIALOG == uiMsg )
    {
		dwMotStep = s_MotStep;

        UpdateDialogControls( hDlg, dwMotStep );

        return TRUE;
    }
    else if( WM_COMMAND == uiMsg )
    {
        HWND hwndMotStep   = GetDlgItem( hDlg, IDC_COMBO1 );

        // Get current UI state
		DWORD dwMotStep = ComboBox_GetCurSel( hwndMotStep );
        
        if( IDOK == LOWORD(wParam) )
        {
            // Handle the case when the user hits the OK button. Check if any
            // of the options were changed
            if( dwMotStep != s_MotStep )
            {
                // Return the newly selected device and its new properties

				s_MotStep = dwMotStep;

				DbgOut( "CMyD3DApplication : ChangeMotStepProc : motstep changed to %d\n", s_MotStep );

                EndDialog( hDlg, IDOK );
            }
            else
                EndDialog( hDlg, IDCANCEL );

            return TRUE;
        }
        else if( IDCANCEL == LOWORD(wParam) )
        {
            // Handle the case when the user hits the Cancel button
            EndDialog( hDlg, IDCANCEL );
            return TRUE;
        }

        // Keep the UI current
        UpdateDialogControls( hDlg, dwMotStep );
        return TRUE;
    }

    return FALSE;

}
VOID UpdateDialogControls( HWND hDlg, DWORD dwMotStep )
{
	DWORD stepmax = 3;
	char strstep[3][5] = { "1", "2", "3" };

    // Access to UI controls
    HWND hwndMotStep         = GetDlgItem( hDlg, IDC_COMBO1 );

    // Reset the content in each of the combo boxes
    ComboBox_ResetContent( hwndMotStep );

    // Add a list of devices to the device combo box
	for( DWORD step = 0; step < stepmax; step++ )
	{
        // Add device name to the combo box
        DWORD dwItem = ComboBox_AddString( hwndMotStep, strstep[step] );
        
		if( dwMotStep == step ){
			// Set the combobox selection on the current device
		    ComboBox_SetCurSel( hwndMotStep, dwItem );
		}
	}
}
***/

/***
int CMyD3DApplication::LockAndUnlock3DBuffer()
{
	int ret = 0;

	if( m_ExistData ){
		ret += m_shandler->Lock3DBuffer();
		_ASSERT( !ret );
	}

	ret += m_ground_sh->Lock3DBuffer();
	_ASSERT( !ret );
	
	////ret += m_bgdisp->UpdateVertexBuffer();
	//_ASSERT( !ret );

	if( m_ExistData ){
		ret += m_shandler->Unlock3DBuffer();
		_ASSERT( !ret );
	}

	ret += m_ground_sh->Unlock3DBuffer();
	_ASSERT( !ret );

	////ret += m_bgdisp->UpdateVertexBuffer();
	//_ASSERT( !ret );

	return ret;
}
***/

int CMyD3DApplication::UpdateVertexBuffer()
{
	int ret = 0;

	if( m_ExistData ){
		ret += m_shandler->UpdateVertexBuffer( 0 );
		_ASSERT( !ret );
	}

	ret += m_ground_sh->UpdateVertexBuffer( 0 );
	_ASSERT( !ret );
	
	if( m_g_sh ){
		ret += m_g_sh->UpdateVertexBuffer( 0 );
		_ASSERT( !ret );
	}

	ret += m_bgdisp->UpdateVertexBuffer();
	_ASSERT( !ret );

	ret += m_select_sh->UpdateVertexBuffer( 0 );
	_ASSERT( !ret );

	ret += m_ikt_sh->UpdateVertexBuffer( 0 );
	_ASSERT( !ret );


	ret += m_sq_sh->UpdateVertexBuffer( 0 );
	_ASSERT( !ret );
	
	return ret;
}
int CMyD3DApplication::Add2Tree( char* newname, int serino, int srctype, int depth, int hasmotion )
{
	int ret = 0;
	int retseri;
	retseri = m_thandler->AddTree( newname, serino );
	if( retseri != serino ){
		_ASSERT( 0 );
		return 1;
	}

	ret = m_thandler->SetElemType( serino, srctype );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	//////////////
	ret = m_shandler->AddShdElem( serino, depth );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_shandler->SetClockwise( serino, 2 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	//////////////
	ret = m_mhandler->AddMotionCtrl( serino, depth );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}	
	ret = m_mhandler->SetHasMotion( serino, hasmotion );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	///////////////
	CMeshInfo tempinfo;
	tempinfo.type = srctype;
	tempinfo.SetMem( 0, MESHI_HASCOLOR );
	ret = m_shandler->Init3DObj( serino, &tempinfo );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}

int CMyD3DApplication::Conv2Scope( int srcboneno, int srcdispno, int srcconvno )
{
	int ret = 0;
	ERRORMES errormes;

	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}

	ret = (int)::MessageBox( m_hWnd, "この操作をすると、読み込み済みの全てのモーションが破棄されます。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( ret != IDOK ){
		::MessageBox( m_hWnd, "変換処理を中止します。", "中止します", MB_OK );
		return 0;
	}


	//////////////////
	m_ExistData = FALSE;


	int saveparent;
	m_shandler->GetParent( srcconvno, &saveparent );
	if( saveparent <= 1 ){
		saveparent = 1;
	}

	///////////////////

	InvalidateHandler();

	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : Conv2Scope : DestroyBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "DelJoint : DestroyBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : Conv2Scope : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}


		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : Conv2Scope : DestroyMotionObj error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "DelJoint : DestroyMotionObj error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}

	//////////////////

	ret = m_shandler->MeshConv2InfScope( m_thandler, m_mhandler, srcboneno, srcdispno, srcconvno );
	if( ret ){
		DbgOut( "CMyD3DApplication : Conv2Scope : MeshConv2InfScope error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Conv2Scope : MeshConv2InfScope error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	///////////////////

	//chain は、sh MeshConv2InfScope内で修正する。thandlerはそのまま。

	////////////////////
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : Conv2Scope : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Conv2Scope : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	ret = RestoreHandler( 1 );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Conv2Scope : RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = m_shandler->CalcInfElem( -1, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : Conv2Scope : CalcInfElem error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Conv2Scope : CalcInfElem error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : Conv2Scope : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler, m_hWnd );
	}
	/////////////////////////
	/////////////////////////
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)saveparent );
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)saveparent );

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	m_ExistData = TRUE;


	return 0;
}


int CMyD3DApplication::DelJoint( int delno, int restoreflag )
{
	int ret = 0;
	ERRORMES errormes;

	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}
	//////////////////



	int saveparent;
	m_shandler->GetParent( delno, &saveparent );
	if( saveparent <= 1 ){
		saveparent = 1;
	}

	///////////////////

	if( restoreflag ){
		m_ExistData = FALSE;

		InvalidateHandler();

		if( m_thandler || m_shandler || m_mhandler ){
			ret = m_shandler->DestroyBoneInfo( m_mhandler );
			if( ret ){
				DbgOut( "CMyD3DApplication : DelJoint : DestroyBoneInfo error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "DelJoint : DestroyBoneInfo error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return 1;
			}

			if( m_mcdlg->m_firstmake ){
				ret = m_mcdlg->DestroyMCElem( -1 );
				if( ret ){
					DbgOut( "viewer : DelJoint : mcdlg DestroyMCElem all error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}


			ret = m_mhandler->DestroyMotionObj();
			if( ret ){
				DbgOut( "CMyD3DApplication : DelJoint : DestroyMotionObj error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "DelJoint : DestroyMotionObj error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return 1;
			}
		}
	}
	//////////////////

	ret = m_shandler->DeleteJoint( delno, m_thandler, m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : DelJoint : DeleteJoint error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "DelJoint : DeleteJoint error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	///////////////////

	//chain は、sh DeleteJoint内で修正する。thandlerはそのまま。

	////////////////////

	if( restoreflag ){
		ret = m_shandler->CreateBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : DelJoint : CreateBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "DelJoint : CreateBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


		ret = RestoreHandler( 1 );
		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "DelJoint : RestoreHandler error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}

		ret = m_shandler->CalcInfElem( -1, 1 );
		if( ret ){
			DbgOut( "CMyD3DApplication : DelJoint : CalcInfElem error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "DelJoint : CalcInfElem error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


		//作り直す前に、invalidateフラグを立てる。
			//作り直し中のSelChangeなどでのエラー回避のため。
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();

		
		if( g_motdlg ){
			g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
				m_hWnd, this );

			if( m_mcdlg->m_firstmake ){
				ret = m_mcdlg->InitMC();
				if( ret ){
					DbgOut( "viewer : DelJoint : mcdlg InitMC error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}		

		}

		if( m_sigeditwnd ){
			m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
				m_hWnd );
		}
		/////////////////////////
		/////////////////////////
		SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)saveparent );
		SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)saveparent );

		SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

		m_paintdlg->InitUndoBuf();


		m_ExistData = TRUE;
	}

	return 0;
}


int CMyD3DApplication::NewJoint( int addnum, int restoreflag )
{
	// 変更する場合は、LoadBoneFile もチェック

	static int newno = 0;
	static int newpartno = 0;

	int ret = 0;
	ERRORMES errormes;

	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}
	//////////////////

	if( restoreflag ){
		m_ExistData = FALSE;

		InvalidateHandler();

		if( m_thandler || m_shandler || m_mhandler ){
			ret = m_shandler->DestroyBoneInfo( m_mhandler );
			if( ret ){
				DbgOut( "CMyD3DApplication : NewJoint : DestroyBoneInfo error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "NewJoint : DestroyBoneInfo error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return 1;
			}
			if( m_mcdlg->m_firstmake ){
				ret = m_mcdlg->DestroyMCElem( -1 );
				if( ret ){
					DbgOut( "viewer : NewJoint : mcdlg DestroyMCElem all error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}


			ret = m_mhandler->DestroyMotionObj();
			if( ret ){
				DbgOut( "CMyD3DApplication : NewJoint : DestroyMotionObj error !!!\n" );
				errormes.errorcode = -1;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "NewJoint : DestroyMotionObj error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	//////////////////
	int serino = m_shandler->s2shd_leng;
	int srctype = SHDBALLJOINT;

	m_thandler->Start( serino - 1 );
	m_thandler->Begin();
	int depth;	
	depth = m_thandler->GetCurDepth();

	char newname[256];
	ZeroMemory( newname, 256 );
	sprintf_s( newname, 256, "追加パート%d", newpartno );

	ret = Add2Tree( newname, serino, SHDPART, depth, 0 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	int addno;
	for( addno = 0; addno < addnum; addno++ ){
		m_thandler->Begin();
		depth = m_thandler->GetCurDepth();
		ZeroMemory( newname, 256 );
		sprintf_s( newname, 256, "追加ボールジョイント%d", newno );

		ret = Add2Tree( newname, serino + 1 + addno, SHDBALLJOINT, depth, 1 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		CVec3f temploc;
		temploc.x = 0.0f;
		temploc.y = 0.0f;
		temploc.z = 0.0f;
		ret = ((*m_shandler)( serino + 1 + addno ))->SetJointLoc( &temploc );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		m_thandler->End();
		newno++;
	}

	newpartno++;
	///////////////////
	ret = m_shandler->SetChain( serino );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->SetChain( serino );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	////////////////////
	////////////////////

	if( restoreflag ){

		ret = m_shandler->CreateBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : NewJoint : CreateBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "NewJoint : CreateBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


		ret = RestoreHandler( 1 );
		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "NewJoint : RestoreHandler error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
		}

		//作り直す前に、invalidateフラグを立てる。
			//作り直し中のSelChangeなどでのエラー回避のため。
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();

		
		if( g_motdlg ){
			g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
				m_hWnd, this );
			if( m_mcdlg->m_firstmake ){
				ret = m_mcdlg->InitMC();
				if( ret ){
					DbgOut( "viewer : NewJoint : mcdlg InitMC error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}		

		}

		if( m_sigeditwnd ){
			m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
				m_hWnd );
		}
		/////////////////////////
		/////////////////////////
		SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)serino );
		SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)serino );

		SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

		m_paintdlg->InitUndoBuf();


		m_ExistData = TRUE;
	}

	return 0;
}

/***
int CMyD3DApplication::NewJoint()
{

	static int newno = 0;
	int ret = 0;
	ERRORMES errormes;

	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}
	//////////////////
	m_ExistData = FALSE;

	InvalidateHandler();

	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : NewJoint : DestroyBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "NewJoint : DestroyBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : NewJoint : DestroyMotionObj error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "NewJoint : DestroyMotionObj error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}

	//////////////////
	int serino = m_shandler->s2shd_leng;
	int srctype = SHDBALLJOINT;

	m_thandler->Start( serino - 1 );
	m_thandler->Begin();
	int depth;	
	depth = m_thandler->GetCurDepth();


	int retseri;
	char newname[256];
	ZeroMemory( newname, 256 );
	sprintf( newname, "追加ボールジョイント%d", newno );
	retseri = m_thandler->AddTree( newname, serino );
	if( retseri != serino ){
		_ASSERT( 0 );
		return 1;
	}

	ret = m_thandler->SetElemType( serino, srctype );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	//////////////
	ret = m_shandler->AddShdElem( serino, depth );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_shandler->SetClockwise( serino, 2 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	//////////////
	ret = m_mhandler->AddMotionCtrl( serino, depth );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}	
	ret = m_mhandler->SetHasMotion( serino, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	///////////////
	CMeshInfo tempinfo;
	tempinfo.type = srctype;
	tempinfo.SetMem( 0, MESHI_HASCOLOR );
	ret = m_shandler->Init3DObj( serino, &tempinfo );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	CVec3f temploc;
	temploc.x = 0.0f;
	temploc.y = 0.0f;
	temploc.z = 0.0f;
	ret = ((*m_shandler)( serino ))->SetJointLoc( &temploc );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	///////////////////
	ret = m_shandler->SetChain( serino );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->SetChain( serino );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	////////////////////
	////////////////////
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : NewJoint : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "NewJoint : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	ret = RestoreHandler();
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "NewJoint : RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
	}

	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
			(long)m_hWnd );
	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
			(long)m_hWnd );
	}
	/////////////////////////
	/////////////////////////
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)serino );
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)serino );

	m_ExistData = TRUE;

	newno++;
	return 0;
}
***/
int CMyD3DApplication::ImportSigFile()
{
	m_materialdlg->ShowDlg( SW_HIDE );	

	int ret = 0;
	if( m_thandler || m_shandler || m_mhandler ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"形状ファイルのインポートを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
			(LPCTSTR)"確認ダイアログ",
			MB_OKCANCEL );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK ){
			return 0;
		}
	}
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、Loadを行う。
		ret = LoadSigFile();
		return ret;
	}

	/***
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"SIG File (*.sig)\0*.sig\0";
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
		return S_OK;
	}
	***/

	int dlgret;
	CLDSigDlg dlg;
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK )
		return 0;


	ERRORMES errormes;

	ret = SetRESDIR( dlg.name, m_shandler );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportSigFile : SetResDir error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}

////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	InvalidateHandler();

	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : ImportSigFile : DestroyBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportSigFile : DestroyBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : ImportSigFile : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : ImportSigFile : DestroyMotionObj error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportSigFile : DestroyMotionObj error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}

	CSigFile* sigfile;
	sigfile = new CSigFile();
	if( !sigfile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportSigFile : sigfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot12( 0.0f, 0.0f, 0.0f );
	ret = sigfile->LoadSigFile( dlg.name, m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng, dlg.mult, 0, offsetpos, rot12, 0 );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = dlg.name;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			delete sigfile;
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportSigFile : LoadSigFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}
	
	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	}


	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadSigFile : CreateBoneInfo error !!!\n" );
		delete sigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportSigFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	ret = RestoreHandler( 1 );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportSigFile : RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
	}


	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : ImportSigFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	
	delete sigfile;//!!!!!

	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );
	
	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, dlg.name );
	}

	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::LoadSigFile()
{
	m_materialdlg->ShowDlg( SW_HIDE );	


	int ret = 0;

	if( m_thandler || m_shandler || m_mhandler ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"形状ファイルの読み込みを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
			(LPCTSTR)"確認ダイアログ",
			MB_OKCANCEL );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK ){
			return 0;
		}
	}

///////////
	/***
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"SIG File (*.sig)\0*.sig\0";
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
		return S_OK;
	}
	***/

	int dlgret;
	CLDSigDlg dlg;
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK )
		return 0;


	ERRORMES errormes;


////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler();
	
	if( m_thandler || m_shandler || m_mhandler ){
		DestroyHandler();
	}
	
	ret = CreateHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadSigFile : CreateHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadSigFile : CreateHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = SetRESDIR( dlg.name, m_shandler );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadSigFile : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}


	CSigFile* sigfile;
	sigfile = new CSigFile();
	if( !sigfile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadSigFile : sigfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot13( 0.0f, 0.0f, 0.0f );
	ret = sigfile->LoadSigFile( dlg.name, m_thandler, m_shandler, m_mhandler, 0, dlg.mult, 0, offsetpos, rot13, 0 );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = dlg.name;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			delete sigfile;
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadSigFile : LoadSigFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	if( m_shandler->s2shd_leng <= 1 ){
		DbgOut( "CMyD3DApplication : LoadSigFile : s2shd_leng <= 1\n" ); 
		
		delete sigfile;
		DestroyHandler();
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();
		ChangeViewParam( ID_KEY_ALL_RESET );
		return 0;
	}

	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	}


	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadSigFile : CreateBoneInfo error !!!\n" );
		delete sigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadSigFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = RestoreHandler( 1 );
	_ASSERT( !ret );


	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		DbgOut( "CMyD3DApplication : LoadSigFile : motdlg->SetParams\n" );

		ret = g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : LoadSigFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		DbgOut( "CMyD3DApplication : LoadSigFile : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
			m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : LoadSigFile : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	
	delete sigfile;//!!!!!


	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, dlg.name );
	}



	m_ExistData = TRUE;

	return 0;
}

//抽出読み込み
int CMyD3DApplication::ExtractBoneData()
{
	int ret = 0;

	if( !m_ExistData || !m_thandler || !m_shandler || !m_mhandler ){
		return 0;//!!!!!!!!
	}

	int dlgret;
	dlgret = (int)MessageBox( m_hWnd, "この操作をすると、読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( dlgret != IDOK ){						
		return 0;//!!!!!!!!!!!
	}

	CExtSigDlg dlg;
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK )
		return 0;

/***
	ret = SetRESDIR( dlg.name );
	if( ret ){
		DbgOut( "viewer : ExtractBoneData : SetRESDIR error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
***/

////////////	
	if( m_ext_th || m_ext_sh || m_ext_mh ){
		DestroyExtHandler();
	}
	
	ret = CreateExtHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : ExtractBoneData : CreateExtHandler error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	
	CSigFile* sigfile;
	sigfile = new CSigFile();
	if( !sigfile ){
		DbgOut( "viewer : ExtractBoneData : sigfile alloc error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}

	D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot13( 0.0f, 0.0f, 0.0f );
	ret = sigfile->LoadSigFile( dlg.name, m_ext_th, m_ext_sh, m_ext_mh, 0, 1.0f, 0, offsetpos, rot13, 0 );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			return 0;
		}else{
			delete sigfile;
			DbgOut( "viewer : ExtractBoneData : sigfile LoadSigFile error !!!\n" );
			_ASSERT( 0 );
			DestroyExtHandler();
			return -1;
		}
	}

	if( m_ext_sh->s2shd_leng <= 1 ){
		DbgOut( "CMyD3DApplication : ExtractBoneData : s2shd_leng <= 1\n" ); 		
		delete sigfile;
		DestroyExtHandler();
		return 0;
	}

	switch( m_ext_sh->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_ext_sh->m_shader = COL_PHONG;
		ret = m_ext_sh->SetIndexBufColP();
		if( ret ){
			DbgOut( "viewer : ExtractBoneData : IDTBB_COLP : COL_PHONG : sh SetIndexBufColP error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_ext_sh->m_shader = COL_PHONG;
		ret = m_ext_sh->SetIndexBufColP();
		if( ret ){
			DbgOut( "viewer : ExtractBoneData : IDTBB_COLP : COL_PHONG : sh SetIndexBufColP error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		break;
	}

	ret = m_ext_sh->CreateBoneInfo( m_ext_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : ExtractBoneData : CreateBoneInfo error !!!\n" );
		delete sigfile;
		_ASSERT( 0 );
		return -1;
	}
	ret = RestoreExtHandler( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : ExtractBoneData : RestoreExtHandler error !!!\n" );
		delete sigfile;
		_ASSERT( 0 );
		return -1;
	}
	delete sigfile;//!!!!!

///////////
///////////
	int extjointnum = 0;
	int extseri;
	CShdElem* extselem;
	for( extseri = 0; extseri < m_ext_sh->s2shd_leng; extseri++ ){
		extselem = (*m_ext_sh)( extseri );
		if( extselem->IsJoint() ){
			extjointnum++;
		}
	}

	int curjointnum = 0;
	int delstartseri = -1;
	int seri;
	CShdElem* selem;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		selem = (*m_shandler)( seri );
		if( selem->IsJoint() ){
			if( curjointnum == extjointnum ){
				delstartseri = seri;
			}
			curjointnum++;
		}
	}

//DbgOut( "!!!check!!!: extractbonedata : extjointnum %d, curjontnum %d\r\n", extjointnum, curjointnum );

	if( extjointnum == 0 ){
		DestroyExtHandler();
		return 0;//!!!!!!!!!!
	}


	// Invalidate
	m_ExistData = FALSE;
	InvalidateHandler();
	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : ExtractBoneData : DestroyBoneInfo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : ExtractBoneData : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : ExtractBoneData : DestroyMotionObj error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	SERICONV* sericonv = 0;
	int setno;

	if( dlg.chkbone ){

		//ボーンの数を合わせる
		int newjointnum;
		newjointnum = extjointnum - curjointnum;
		if( newjointnum > 0 ){

//DbgOut( "!!!check!!!: ExtractBonedata : newjointnum %d, NewJoint\r\n", newjointnum );

			ret = NewJoint( newjointnum, 0 );
			if( ret ){
				DbgOut( "viewer : ExtractBoneData : NewJoint error !!!\n" );
				_ASSERT( 0 );
				DestroyExtHandler();
				return 1;
			}
		}else if( newjointnum < 0 ){

//DbgOut( "!!!check!!!: ExtractBonedata : newjointnum %d, DelJoint\r\n", newjointnum );

			if( delstartseri > 1 ){
				for( seri = delstartseri; seri < m_shandler->s2shd_leng; seri++ ){
					selem = (*m_shandler)( seri );
					if( selem->IsJoint() ){
						ret = DelJoint( seri, 0 );
						if( ret ){
							DbgOut( "viewer : ExtractBoneData : DelJoint error !!!\n" );
							_ASSERT( 0 );
							DestroyExtHandler();
							return 1;
						}
					}
				}
			}
		}


		//ボーンのシリアル番号の対応表を作成する
		sericonv = (SERICONV*)malloc( sizeof( SERICONV ) * extjointnum );
		if( !sericonv ){
			DbgOut( "viewer : ExtractBoneData : sericonv alloc error !!!\n" );
			_ASSERT( 0 );
			DestroyExtHandler();
			return 1;
		}
		ZeroMemory( sericonv, sizeof( SERICONV ) * extjointnum );

		setno = 0;
		int curextseri = 0;
		for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
			selem = (*m_shandler)( seri );
			if( selem->IsJoint() ){
				CShdElem* findextelem = 0;
				for( extseri = curextseri; extseri < m_ext_sh->s2shd_leng; extseri++ ){
					extselem = (*m_ext_sh)( extseri );
					if( extselem->IsJoint() ){
						curextseri = extseri + 1;//!!!!!!
						findextelem = extselem;
						break;
					}
				}

				if( !findextelem ){
					DbgOut( "viewer : ExtractBoneData : findextelem error !!!\n" );
					_ASSERT( 0 );
					DestroyExtHandler();
					return 1;
				}
					
				(sericonv + setno)->seri = seri;
				(sericonv + setno)->extseri = findextelem->serialno;
				setno++;
			}
		}
		if( setno != extjointnum ){
			DbgOut( "viewer : ExtractBoneData : setno error !!!\n" );
			_ASSERT( 0 );
			DestroyExtHandler();
			free( sericonv );
			return 1;
		}

		//ジョイントの名前と位置をセット
		int jointno;
		for( jointno = 0; jointno < extjointnum; jointno++ ){
			seri = (sericonv + jointno)->seri;
			extseri = (sericonv + jointno)->extseri;

			CTreeElem2* telem;
			telem = (*m_thandler)( seri );
			_ASSERT( telem );
			CTreeElem2* exttelem;
			exttelem = (*m_ext_th)( extseri );
			_ASSERT( exttelem );

			ret = telem->SetName( exttelem->name );
			if( ret ){
				DbgOut( "viewer : ExtractBoneData : SetName error !!!\n" );
				_ASSERT( 0 );
				DestroyExtHandler();
				free( sericonv );
				return 1;
			}

			extselem = (*m_ext_sh)( extseri );
			_ASSERT( extselem );
			CVec3f* locptr;
			if( !extselem->part ){
				DbgOut( "viewer : ExtractBoneData : ext part error !!!\n" );
				_ASSERT( 0 );
				DestroyExtHandler();
				free( sericonv );
				return 1;
			}
			locptr = &( extselem->part->jointloc );

			selem = (*m_shandler)( seri );
			_ASSERT( selem );
			if( !selem->part ){
				DbgOut( "viewer : ExtractBoneData : part error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			selem->part->jointloc = *locptr;

			selem->notuse = extselem->notuse;//!!!!!!!!!
		}

		//階層構造をフラットにする
		CShdElem* firstparent = 0;
		ret = MakeAllJointTreeFlat( &firstparent );
		if( ret ){
			DbgOut( "viewer : ExtractBoneData : MakeAllJointFlat error !!!\n" );
			_ASSERT( 0 );
			DestroyExtHandler();
			free( sericonv );
			return 1;
		}

		//階層構造をコピー
		for( jointno = 0; jointno < extjointnum; jointno++ ){
			seri = (sericonv + jointno)->seri;
			extseri = (sericonv + jointno)->extseri;

			CShdElem* selem = (*m_shandler)( seri );
			_ASSERT( selem );
			CShdElem* extselem = (*m_ext_sh)( extseri );
			_ASSERT( extselem );

			CShdElem* extparent;
			extparent = extselem->parent;
			CShdElem* extchild;
			extchild = extselem->child;
			CShdElem* extsister;
			extsister = extselem->sister;
			CShdElem* extbrother;
			extbrother = extselem->brother;
			
			if( extparent ){
				if( extparent->type == SHDPART ){
					selem->parent = firstparent;


					// firstparentの親子関係設定
					if( extparent->child && extparent->child->IsJoint() ){
						FindSeriByExtSeri( sericonv, extjointnum, extparent->child->serialno, &firstparent->child );
						_ASSERT( firstparent->child );
					}
					if( extparent->parent && extparent->parent->IsJoint() ){
						FindSeriByExtSeri( sericonv, extjointnum, extparent->parent->serialno, &firstparent->parent );
						_ASSERT( firstparent->parent );
					}
					if( extparent->sister && extparent->sister->IsJoint() ){
						FindSeriByExtSeri( sericonv, extjointnum, extparent->sister->serialno, &firstparent->sister );
						_ASSERT( firstparent->sister );
					}
					if( extparent->brother && extparent->brother->IsJoint() ){
						FindSeriByExtSeri( sericonv, extjointnum, extparent->brother->serialno, &firstparent->brother );
						_ASSERT( firstparent->brother );
					}

				}else{	
					FindSeriByExtSeri( sericonv, extjointnum, extparent->serialno, &selem->parent );
					_ASSERT( selem->parent );
				}
			}else{
				selem->parent = 0;
				_ASSERT( 0 );
			}

			if( extchild ){
				if( extchild->type == SHDPART ){
					selem->child = firstparent;
					_ASSERT( 0 );
				}else{	
					FindSeriByExtSeri( sericonv, extjointnum, extchild->serialno, &selem->child );
					_ASSERT( selem->child );
				}
			}else{
				selem->child = 0;
			}

			if( extsister ){
				if( extsister->type == SHDPART ){
					selem->sister = firstparent;
					_ASSERT( 0 );
				}else{	
					FindSeriByExtSeri( sericonv, extjointnum, extsister->serialno, &selem->sister );
					_ASSERT( selem->sister );
				}
			}else{
				selem->sister = 0;
			}

			if( extbrother ){
				if( extbrother->type == SHDPART ){
					selem->brother = firstparent;
					_ASSERT( 0 );
				}else{	
					FindSeriByExtSeri( sericonv, extjointnum, extbrother->serialno, &selem->brother );
					_ASSERT( selem->brother );
				}
			}else{
				selem->brother = 0;
			}

/***
	DbgOut( "!!!check!!!: extseri %d\r\n", extseri );
	if( selem->parent ){
		DbgOut( "!!!check!!!: selem->parent %d, %d\r\n", selem->parent->serialno, selem->parent->IsJoint() );
	}
	if( selem->child ){
		DbgOut( "!!!check!!!: selem->child %d, %d\r\n", selem->child->serialno, selem->child->IsJoint() );
	}
	if( selem->sister ){
		DbgOut( "!!!check!!!: selem->sister %d, %d\r\n", selem->sister->serialno, selem->sister->IsJoint() );
	}
	if( selem->brother ){
		DbgOut( "!!!check!!!: selem->brother %d, %d\r\n", selem->brother->serialno, selem->brother->IsJoint() );
	}
	DbgOut( "\r\n" );
***/

		}

/***	
	DbgOut( "!!!check!!!: firstparent %d\r\n", firstparent->serialno );
	if( firstparent->parent ){
		DbgOut( "!!!check!!!: firstparent->parent %d, %d\r\n", firstparent->parent->serialno, firstparent->parent->IsJoint() );
	}
	if( firstparent->child ){
		DbgOut( "!!!check!!!: firstparent->child %d, %d\r\n", firstparent->child->serialno, firstparent->child->IsJoint() );
	}
	if( firstparent->sister ){
		DbgOut( "!!!check!!!: firstparent->sister %d, %d\r\n", firstparent->sister->serialno, firstparent->sister->IsJoint() );
	}
	if( firstparent->brother ){
		DbgOut( "!!!check!!!: firstparent->brother %d, %d\r\n", firstparent->brother->serialno, firstparent->brother->IsJoint() );
	}
	DbgOut( "\r\n" );
***/


		m_shandler->CalcDepth();
		m_mhandler->CopyChainFromShd( m_shandler );//!!!!!!!!!!
		m_mhandler->CalcDepth();
	}else{

		//ボーンのシリアル番号の対応表を作成する
		sericonv = (SERICONV*)malloc( sizeof( SERICONV ) * extjointnum );
		if( !sericonv ){
			DbgOut( "viewer : ExtractBoneData : sericonv alloc error !!!\n" );
			_ASSERT( 0 );
			DestroyExtHandler();
			return 1;
		}
		int convno;
		for( convno = 0; convno < extjointnum; convno++ ){
			(sericonv + convno)->seri = 0;
			(sericonv + convno)->extseri = 0;
		}

		setno = 0;
		for( extseri = 0; extseri < m_ext_sh->s2shd_leng; extseri++ ){
			extselem = (*m_ext_sh)( extseri );
			if( extselem->IsJoint() ){
				CShdElem* findelem = 0;
				char* extjointname;
				extjointname = (*m_ext_th)( extseri )->name;
				int boneno = 0;
				ret = m_thandler->GetBoneNoByName( extjointname, &boneno, m_shandler, 0 );
				if( ret ){
					DbgOut( "viewer : ExtractBoneData : th GetBoneNoByName error !!!\n" );
					_ASSERT( 0 );
					DestroyExtHandler();
					free( sericonv );
					return 1;
				}

				//if( boneno > 0 ){
					if( setno >= extjointnum ){
						DbgOut( "viewer : ExtractBoneData : setno extjointnum error !!!\n" );
						_ASSERT( 0 );
						DestroyExtHandler();
						free( sericonv );
						return 1;
					}

					( sericonv + setno )->seri = boneno;
					( sericonv + setno )->extseri = extseri;
					setno++;
				//}
			}
		}
	}
////////////
// 表示パーツの変換表を作る。　noskinningのコピー
	int extdispnum = 0;
	for( extseri = 0; extseri < m_ext_sh->s2shd_leng; extseri++ ){
		extselem = (*m_ext_sh)( extseri );
		if( (extselem->type == SHDPOLYMESH) || (extselem->type == SHDPOLYMESH2) ){
			extdispnum++;
		}
	}

	if( extdispnum == 0 ){
		goto extractboneaft;//!!!!!!!!!!!!!
	}

	setno = 0;
	SERICONV* dispconv;
	dispconv = (SERICONV*)malloc( sizeof( SERICONV ) * extdispnum );
	int convno;
	for( convno = 0; convno < extdispnum; convno++ ){
		( dispconv + convno )->seri = 0;
		( dispconv + convno )->extseri = 0;
	}
	for( extseri = 0; extseri < m_ext_sh->s2shd_leng; extseri++ ){
		extselem = (*m_ext_sh)( extseri );

		if( (extselem->type == SHDPOLYMESH) || (extselem->type == SHDPOLYMESH2) ){
			char* extnameptr;
			extnameptr = (*m_ext_th)( extseri )->name;

			int dispno = -1;
			ret = m_thandler->GetDispObjNoByName( extnameptr, &dispno, m_shandler, 1 );
			if( ret ){
				DbgOut( "viewer : ExtractBoneData : GetDispObjNoByName error !!!\n" );
				_ASSERT( 0 );
				DestroyExtHandler();
				free( sericonv );
				free( dispconv );
				return 1;
			}

			//if( dispno > 0 ){

				if( setno >= extdispnum ){
					DbgOut( "viewer : ExtractBoneData : setno extdispnum error !!!\n" );
					_ASSERT( 0 );
					DestroyExtHandler();
					free( sericonv );
					free( dispconv );
					return 1;
				}

				(dispconv + setno)->extseri = extseri;
				(dispconv + setno)->seri = dispno;

				if( dispno > 0 ){
					selem = (*m_shandler)( dispno );
					selem->noskinning = extselem->noskinning;//!!!!!!!!!!!!!!!!! noskinningコピー
				}
				setno++;
			//}
		}
	}

///////////
/////////// EI1
	if( dlg.chkei1 ){
		int dispno;
		for( dispno = 0; dispno < extdispnum; dispno++ ){
			seri = (dispconv + dispno)->seri;
			extseri = (dispconv + dispno)->extseri;

			if( seri > 0 ){
				selem = (*m_shandler)( seri );
				extselem = (*m_ext_sh)( extseri );

				if( extselem->influencebonenum != 0 ){
					selem->influencebone = (int*)realloc( selem->influencebone, sizeof( int ) * extselem->influencebonenum );
					int infno;
					setno = 0;
					for( infno = 0; infno < extselem->influencebonenum; infno++ ){
						CShdElem* infselem = 0;
						FindSeriByExtSeri( sericonv, extjointnum, *( extselem->influencebone + infno ), &infselem );
						int setinfno;
						if( infselem ){
							setinfno = infselem->serialno;
							*( selem->influencebone + setno ) = setinfno;
							setno++;
						}
					}
					if( setno != extselem->influencebonenum ){
						selem->influencebone = (int*)realloc( selem->influencebone, sizeof( int ) * setno );
					}
					selem->influencebonenum = setno;
				}else{
					if( selem->influencebone ){
						free( selem->influencebone );
						selem->influencebone = 0;
					}
					selem->influencebonenum = 0;
				}

				if( extselem->ignorebonenum != 0 ){
					selem->ignorebone = (int*)realloc( selem->ignorebone, sizeof( int ) * extselem->ignorebonenum );
					//MoveMemory( selem->ignorebone, extselem->ignorebone, sizeof( int ) * extselem->ignorebonenum );
					int igno;
					setno = 0;
					for( igno = 0; igno < extselem->ignorebonenum; igno++ ){
						CShdElem* igselem = 0;
						FindSeriByExtSeri( sericonv, extjointnum, *( extselem->ignorebone + igno ), &igselem );
						int setigno;
						if( igselem ){
							setigno = igselem->serialno;
							*( selem->ignorebone + igno ) = setigno;
							setno++;
						}
					}
					if( setno != extselem->ignorebonenum ){
						selem->ignorebone = (int*)realloc( selem->ignorebone, sizeof( int ) * setno );
					}
					selem->ignorebonenum = setno;
				}else{
					if( selem->ignorebone ){
						free( selem->ignorebone );
						selem->ignorebone = 0;
					}
					selem->ignorebonenum = 0;
				}

				selem->notuse = extselem->notuse;//!!!!!!!!!
			}
		}
	}

////////////
////////// EI2
	if( dlg.chkei2 ){

		CShdElem* iselem;
		iselem = m_shandler->GetInfScopeElem();
		CShdElem* extiselem;
		extiselem = m_ext_sh->GetInfScopeElem();
		int extisnum;
		extisnum = extiselem->scopenum;

		if( extisnum > 0 ){
			int isindex;
			for( isindex = 0; isindex < extisnum; isindex++ ){
				CInfScope* extis;
				extis = *( extiselem->ppscope + isindex );
				CShdElem* extapplychild;
				CShdElem* exttarget;
				extapplychild = extis->applychild;
				exttarget = extis->target;
				_ASSERT( extapplychild );
				_ASSERT( exttarget );

				int applychild = 0;
				int target = 0;
				CShdElem* acelem = 0;
				CShdElem* tarelem = 0;
				FindSeriByExtSeri( sericonv, extjointnum, extapplychild->serialno, &acelem );
				if( acelem ){
					applychild = acelem->serialno;
				}
				FindSeriByExtSeri( dispconv, extdispnum, exttarget->serialno, &tarelem );
				if( tarelem ){
					target = tarelem->serialno;
				}

				if( (applychild > 0) && (target > 0) ){
					int chkindex;
					chkindex = m_shandler->ExistInfScope( applychild, target );
					if( chkindex >= 0 ){
						ret = iselem->DeleteInfScopeElemByIndex( chkindex );
						if( ret ){
							DbgOut( "viewer : ExtractBoneData : iselem DeleteInfScopeElemByIndex error !!!\n" );
							_ASSERT( 0 );
							DestroyExtHandler();
							free( sericonv );
							free( dispconv );
							return 1;
						}
					}

					if( extis->type == SHDPOLYMESH ){
						CPolyMesh* pm;
						pm = extis->polymesh;
						pm->m_mikoapplychild = acelem;
						pm->m_mikotarget = tarelem;
						
						ret = iselem->AddInfScope( pm, 0 );
						if( ret ){
							DbgOut( "viewer : ExtractBoneData : iselem AddInfScope pm error !!!\n" );
							_ASSERT( 0 );
							DestroyExtHandler();
							free( sericonv );
							free( dispconv );
							return 1;
						}
						
						extis->polymesh = 0;

					}else if( extis->type == SHDPOLYMESH2 ){
						CPolyMesh2* pm2;
						pm2 = extis->polymesh2;
						pm2->m_mikoapplychild = acelem;
						pm2->m_mikotarget = tarelem;
						pm2->m_anchormaterialno = 0;//!!!!!!

						ret = iselem->AddInfScope( pm2, 0, extis->pm2facet );
						if( ret ){
							DbgOut( "viewer : ExtractBoneData : iselem AddInfScope pm2 error !!!\n" );
							_ASSERT( 0 );
							DestroyExtHandler();
							free( sericonv );
							free( dispconv );
							return 1;
						}

						extis->polymesh2 = 0;
					}

				}
			}
		}
	}
		
//////////////
//////////// restore and default inf set
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : ExtractBoneData : CreateBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int dispwarnflag = 0;
	ret = RestoreHandler( 1, dispwarnflag );
	if( ret ){
		DbgOut( "viewer : ExtractBoneData : RestoreHandler error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( dlg.chkei1 || dlg.chkei2 ){
		ret = m_shandler->CalcInfElem( -1, 1 );
		if( ret ){
			DbgOut( "viewer : ExtractBoneData : sh CalcInfElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

/////////////
/////////// EI3
	if( dlg.chkei3 ){
		int dispno;
		for( dispno = 0; dispno < extdispnum; dispno++ ){
			seri = (dispconv + dispno)->seri;
			extseri = (dispconv + dispno)->extseri;

			if( seri > 0 ){
				selem = (*m_shandler)( seri );
				extselem = (*m_ext_sh)( extseri );
		
				if( (selem->type == SHDPOLYMESH) && (extselem->type == SHDPOLYMESH) ){
					CPolyMesh* pm;
					CPolyMesh* extpm;
					pm = selem->polymesh;
					extpm = extselem->polymesh;
					_ASSERT( pm );
					_ASSERT( extpm );

					ret = pm->CopyNearestInfElem( extpm, sericonv, extjointnum );
					if( ret ){
						DbgOut( "viewer : ExtractBoneData : pm CopyNearestInfElem error !!!\n" );
						_ASSERT( 0 );
						DestroyExtHandler();
						free( sericonv );
						free( dispconv );
						return 1;
					}
				}else if( (selem->type == SHDPOLYMESH2) && (extselem->type == SHDPOLYMESH2) ){
					CPolyMesh2* pm2;
					CPolyMesh2* extpm2;
					pm2 = selem->polymesh2;
					extpm2 = extselem->polymesh2;
					_ASSERT( pm2 );
					_ASSERT( extpm2 );

					ret = pm2->CopyNearestInfElem( extpm2, sericonv, extjointnum );
					if( ret ){
						DbgOut( "viewer : ExtractBoneData : pm2 CopyNearestInfElem error !!!\n" );
						_ASSERT( 0 );
						DestroyExtHandler();
						free( sericonv );
						free( dispconv );
						return 1;
					}
				}

				ret = m_shandler->CreateSkinMat( seri, 1 );
				if( ret ){
					DbgOut( "viewer : ExtractBoneData : sh CreateSkinMat error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

			}
		}
	}

///////////
///////////

	goto extractboneaft;

extractboneaft:

	DestroyExtHandler();
	free( sericonv );
	free( dispconv );

////////////
////////////

	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : ExtractBoneData : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	/////////////////////////
	/////////////////////////
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)1 );
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)1 );

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	m_paintdlg->InitUndoBuf();

	m_ExistData = TRUE;


	return 0;
}

int CMyD3DApplication::FindSeriByExtSeri( SERICONV* sericonv, int convnum, int extseri, CShdElem** ppselem )
{
	*ppselem = 0;

	int convno;
	int seri;
	for( convno = 0; convno < convnum; convno++ ){
		if( extseri == (sericonv + convno)->extseri ){
			seri = (sericonv + convno)->seri;
			if( seri > 0 ){
				*ppselem = (*m_shandler)( seri );
			}else{
				*ppselem = 0;
			}
			break;
		}
	}
	return 0;
}


int CMyD3DApplication::MakeAllJointTreeFlat( CShdElem** ppfirstparent )
{
	int ret;

	//SHDPARTの中で、一番serialの大きいものを、firstparentnoにセット
	int firstparentno;
	firstparentno = 1;//!!!!!
	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		if( selem->type == SHDPART ){
			if( seri > firstparentno ){
				firstparentno = seri;
			}
		}
	}

//DbgOut( "!!!check!!!: MakeAllJointTreeFlag : firstparentno %d\r\n", firstparentno );

// newparentのtreeを初期化
	CShdElem* newparent;
	newparent = (*m_shandler)( firstparentno );
	if( !newparent ){
		DbgOut( "viewer : MakeAllJointFlat : newparent error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	*ppfirstparent = newparent;


	CShdElem* curselem;
// jointのTreeを０にリセット
	for( seri = 1; seri < m_shandler->s2shd_leng; seri++ ){
		curselem = (*m_shandler)( seri );
		_ASSERT( curselem );

		if( curselem->IsJoint() ){
			curselem->parent = 0;
			curselem->child = 0;
			curselem->brother = 0;
			curselem->sister = 0;
		}else{
			if( curselem->type == SHDPART ){
				CShdElem* chkchild;
				chkchild = curselem->child;

				if( chkchild && (chkchild->IsJoint()) ){
					curselem->child = 0;
				}
			}
		}
	}


// 全てのjointをnewparentの子供にセットする
	for( seri = 1; seri < m_shandler->s2shd_leng; seri++ ){
		curselem = (*m_shandler)( seri );
		_ASSERT( curselem );

		if( curselem->IsJoint() ){
			ret = curselem->PutToChain( newparent, 1 );
			if( ret ){
				DbgOut( "viewer : MakeAllJointFlat : PutToChain error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}



int CMyD3DApplication::LoadXFile()
{
	int ret = 0;

	if( m_thandler || m_shandler || m_mhandler ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"形状ファイルの読み込みを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
			(LPCTSTR)"確認ダイアログ",
			MB_OKCANCEL );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK ){
			return 0;
		}
	}

///////////

	int dlgret;
	CLdXDlg dlg;
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK )
		return 0;


	ERRORMES errormes;


////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler();
	
	if( m_thandler || m_shandler || m_mhandler ){
		DestroyHandler();
	}
	
	ret = CreateHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadXFile : CreateHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadXFile : CreateHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = SetRESDIR( dlg.name, m_shandler );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadXFile : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}
	
	CXFile xfile;

	ret = xfile.LoadXFile( m_pd3dDevice, dlg.name, m_thandler, m_shandler, m_mhandler, 0, dlg.mult );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = dlg.name;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadXFile : LoadXFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	if( m_shandler->s2shd_leng <= 1 ){
		DbgOut( "CMyD3DApplication : LoadXFile : s2shd_leng <= 1\n" ); 
		
		DestroyHandler();
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();
		ChangeViewParam( ID_KEY_ALL_RESET );
		return 0;
	}

	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	}



	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadXFile : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadXFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = RestoreHandler( 1 );
	_ASSERT( !ret );


	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		DbgOut( "CMyD3DApplication : LoadXFile : motdlg->SetParams\n" );

		ret = g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : LoadXFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		DbgOut( "CMyD3DApplication : LoadXFile : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
			m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : LoadXFile : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	
	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, dlg.name );
	}

	m_ExistData = TRUE;

	return 0;
}

/***
int CMyD3DApplication::LoadFBXFile()
{
	int ret = 0;

	if( m_thandler || m_shandler || m_mhandler ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"形状ファイルの読み込みを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
			(LPCTSTR)"確認ダイアログ",
			MB_OKCANCEL );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK ){
			return 0;
		}
	}

///////////

	int dlgret;
	CLdFBXDlg dlg;
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK )
		return 0;


	ERRORMES errormes;

	ret = SetRESDIR( dlg.name );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadFBXFile : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}

////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler();
	
	if( m_thandler || m_shandler || m_mhandler ){
		DestroyHandler();
	}
	
	ret = CreateHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadFBXFile : CreateHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadFBXFile : CreateHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	
	CFBXFile fbxfile;

	ret = fbxfile.LoadFBXFile( dlg.name, m_thandler, m_shandler, m_mhandler, 0, dlg.mult );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = dlg.name;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadFBXFile : LoadFBXFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			//fbxfile.UnLoad();
			return -1;
		}
	}

	if( m_shandler->s2shd_leng <= 1 ){
		DbgOut( "CMyD3DApplication : LoadFBXFile : s2shd_leng <= 1\n" ); 
		
		DestroyHandler();
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();
		ChangeViewParam( ID_KEY_ALL_RESET );
		//fbxfile.UnLoad();
		return 0;
	}

	switch( m_shandler->m_shader ){
	case COL_OLD:
		m_tbb_colR = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLR, 0 );
		break;
	case COL_PHONG:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_BLINN:
		m_tbb_colB = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLB, 0 );
		break;
	case COL_SCHLICK:
		m_tbb_colS = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLS, 0 );
		break;
	case COL_MQCLASSIC:
		m_tbb_colM = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLM, 0 );
		break;
	case COL_TOON1:
//		m_tbb_colT = 0;
//		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colR = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLR, 0 );
		break;
	}

	switch( m_shandler->m_overflow ){
	case OVERFLOW_CLAMP:
		m_tbb_ofc = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_OFC, 0 );
		break;
	case OVERFLOW_SCALE:
		m_tbb_ofs = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_OFS, 0 );
		break;
	case OVERFLOW_ORG:
		m_tbb_ofo = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_OFO, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_ofo = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_OFO, 0 );
		break;
	}

	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();


	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadFBXFile : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadFBXFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		//fbxfile.UnLoad();
		return -1;
	}

	ret = RestoreHandler();
	_ASSERT( !ret );


	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		DbgOut( "CMyD3DApplication : LoadFBXFile : motdlg->SetParams\n" );

		ret = g_motdlg->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
			(long)m_hWnd, this );
	}

	if( m_sigeditwnd ){
		DbgOut( "CMyD3DApplication : LoadFBXFile : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
			m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
			(long)m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : LoadFBXFile : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, dlg.name );
	}



	m_ExistData = TRUE;

	return 0;
}
***/

int CMyD3DApplication::ImportMqoFile( LOADFILEINFO* ldinfo )
{
	m_materialdlg->ShowDlg( SW_HIDE );	

	int ret = 0;

	if( ldinfo == 0 ){
		if( m_thandler || m_shandler || m_mhandler ){
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
			ret = ::MessageBox( m_hWnd,
				(LPCTSTR)"形状ファイルのインポートを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
				(LPCTSTR)"確認ダイアログ",
				MB_OKCANCEL );
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			if( ret != IDOK ){
				return 0;
			}
		}
	}

	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、Loadを行う。
		ret = LoadMQOFile( ldinfo );
		return ret;
	}

	char buf[_MAX_PATH];
	if( ldinfo == 0 ){
		OPENFILENAME ofn;
		buf[0] = 0;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"MQO File (*.mqo)\0*.mqo\0";
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
			return S_OK;
		}
	}


	ERRORMES errormes;

	char* mqonameptr;
	if( ldinfo == 0 ){
		mqonameptr = buf;
	}else{
		mqonameptr = ldinfo->filename;
	}


	if( !ldinfo || (ldinfo && ldinfo->needsetresdir) ){
		ret = SetRESDIR( mqonameptr, m_shandler );
		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportMqoFile : SetResDir error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}
////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler();

	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : ImportMqoFile : DestroyBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportMqoFile : DestroyBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : ImportMqoFile : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}


		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : ImportMqoFile : DestroyMotionObj error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportMqoFile : DestroyMotionObj error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}

	CMQOFile* mqofile;
	mqofile = new CMQOFile( m_hWnd );
	if( !mqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportMqoFile : mqofile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	float mqomult;
	int bonetype;
	int mikoblendtype;

	if( ldinfo == 0 ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		
		CMQODlg dlg;
		ret = (int)dlg.DoModal();
		
		mqomult = dlg.m_mult;
		bonetype = dlg.m_bonetype;
		
		if( dlg.m_bonetype == BONETYPE_MIKO ){		
			CMikoBlendDlg dlg2;
			ret = (int)dlg2.DoModal();
			
			mikoblendtype = dlg2.m_mikoblendtype;
		}else{
			mikoblendtype = MIKOBLEND_MIX;
		}

		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );


	}else{
		mqomult = ldinfo->mult;
		bonetype = BONETYPE_RDB2;
		mikoblendtype = MIKOBLEND_MIX;
	}

	D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot5( 0.0f, 0.0f, 0.0f );
	ret = mqofile->LoadMQOFile( mqomult, mqonameptr, m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng, 0, bonetype, 0, offsetpos, rot5 );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = mqonameptr;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			delete mqofile;
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportMqoFile : LoadMQOFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}
	
	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	}


	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadMQOFile : CreateBoneInfo error !!!\n" );
		delete mqofile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportMqoFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	m_shandler->m_mikoblendtype = mikoblendtype;//!!!!!!!!!!!!!!!!!!!


	ret = RestoreHandler( 1 );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportMqoFile : RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
	}



	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : ImportMqoFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	
	delete mqofile;//!!!!!

	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );
	
	m_ExistData = TRUE;


	if( ldinfo ){
		ldinfo->lpth = m_thandler;
		ldinfo->lpsh = m_shandler;
		ldinfo->lpmh = m_mhandler;
	}

	if( !ldinfo && m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, mqonameptr );
	}

	return 0;
}
int CMyD3DApplication::LoadGroundData()
{
	int ret;
	ERRORMES errormes;

	CLdGroundDlg dlg;

	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal();		
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( ret != IDOK )
		return 0;



////////
	InvalidateGHandler();
	
	if( m_g_th || m_g_sh || m_g_mh ){
		DestroyGHandler();
	}
	
	ret = CreateGHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadGroundData : CreateHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadGroundData : CreateHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = SetRESDIR( dlg.name, m_g_sh );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadGroundData : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}

/////////

	CMQOFile* mqofile;
	mqofile = new CMQOFile( m_hWnd );
	if( !mqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadGroundData MQOFile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot6( 0.0f, 0.0f, 0.0f );
	ret = mqofile->LoadMQOFile( dlg.mult, dlg.name, m_g_th, m_g_sh, m_g_mh, 0, 1, BONETYPE_RDB2, 0, offsetpos, rot6 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ground mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		delete mqofile;
		return -1;
	}
	m_g_sh->m_TLmode = TLMODE_D3D;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	ret = m_g_sh->CreateBoneInfo( m_g_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadGroundData : ground : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ground CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		delete mqofile;
		return -1;
	}	

	delete mqofile;
	
//////////

	ret = RestoreGHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadGroundData : ground : RestoreGHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ground RestoreGHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
	}	


	return 0;
}

int CMyD3DApplication::LoadMQOFile( LOADFILEINFO* ldinfo )
{
	m_materialdlg->ShowDlg( SW_HIDE );	


	int ret = 0;
	
	char buf[_MAX_PATH];

	if( ldinfo == 0 ){
		if( m_thandler || m_shandler || m_mhandler ){
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
			ret = ::MessageBox( m_hWnd,
				(LPCTSTR)"形状ファイルの読み込みを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
				(LPCTSTR)"確認ダイアログ",
				MB_OKCANCEL );
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			if( ret != IDOK ){
				return 0;
			}
		}

	
		OPENFILENAME ofn;
		buf[0] = 0;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"MQO File (*.mqo)\0*.mqo\0";
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
			return S_OK;
		}
	}


	ERRORMES errormes;

	char* mqonameptr;
	if( ldinfo == 0 ){
		mqonameptr = buf;
	}else{
		mqonameptr = ldinfo->filename;
	}


////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	InvalidateHandler();
	
	if( m_thandler || m_shandler || m_mhandler ){
		DestroyHandler();
	}
	
	ret = CreateHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadMQOFile : CreateHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadMQOFile : CreateHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	if( !ldinfo || (ldinfo && ldinfo->needsetresdir) ){
		ret = SetRESDIR( mqonameptr, m_shandler );
		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadMQOFile : SetRESDIR error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}
	
	CMQOFile* mqofile;
	mqofile = new CMQOFile( m_hWnd );
	if( !mqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadMQOFile : mqofile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	float mqomult;
	int bonetype;
	int mikoblendtype;

	if( ldinfo == 0 ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

		CMQODlg dlg;
		ret = (int)dlg.DoModal();

		mqomult = dlg.m_mult;
		bonetype = dlg.m_bonetype;

		if( bonetype == BONETYPE_MIKO ){
			CMikoBlendDlg dlg2;
			ret = (int)dlg2.DoModal();
			mikoblendtype = dlg2.m_mikoblendtype;
		}else{
			mikoblendtype = MIKOBLEND_MIX;
		}

		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	}else{
		mqomult = ldinfo->mult;
		bonetype = BONETYPE_RDB2;
		mikoblendtype = MIKOBLEND_MIX;
	}

//bonetype = BONETYPE_RDB2;//!!!!!!!!!!!!!!!!!!

	::SendMessage( m_hWnd, WM_USER_CREATE_PB, 0, 0 );
	::SendMessage( m_hWnd, WM_USER_SET_PB, 30, 0 );

	D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot7( 0.0f, 0.0f, 0.0f );
	ret = mqofile->LoadMQOFile( mqomult, mqonameptr, m_thandler, m_shandler, m_mhandler, 0, 0, bonetype, 0, offsetpos, rot7 );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = mqonameptr;
			::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			//_ASSERT( 0 );
		}else{
			delete mqofile;
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadMQOFile : LoadMQOFile error";
			::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	::SendMessage( m_hWnd, WM_USER_SET_PB, 60, 0 );


	if( m_shandler->s2shd_leng <= 1 ){
		DbgOut( "CMyD3DApplication : LoadMQOFile : s2shd_leng <= 1\n" ); 
		
		delete mqofile;
		DestroyHandler();
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();
		ChangeViewParam( ID_KEY_ALL_RESET );
		::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
		return 0;
	}

	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	}


	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
		DbgOut( "CMyD3DApplication : LoadMQOFile : CreateBoneInfo error !!!\n" );
		delete mqofile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadMQOFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	m_shandler->m_mikoblendtype = mikoblendtype;//!!!!!!!!!!!!!!!!!!!!


	::SendMessage( m_hWnd, WM_USER_SET_PB, 80, 0 );

	ret = RestoreHandler( 1 );
	_ASSERT( !ret );

/***
	ret = m_shandler->Material2Color();
	if( ret ){
		::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
		DbgOut( "cmyd3dapp : LoadMQOFile : sh Material2Color error !!!\n" );
		delete mqofile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadMQOFile : sh Material2Color error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
***/

	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		DbgOut( "CMyD3DApplication : LoadMQOFile : motdlg->SetParams\n" );

		ret = g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : LoadMQOFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		DbgOut( "CMyD3DApplication : LoadMQOFile : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
			m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
		if( ret ){
			::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
			DbgOut( "CMyD3DApplication : LoadMQOFile : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	
	delete mqofile;//!!!!!

	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	m_ExistData = TRUE;

	if( ldinfo ){
		ldinfo->lpth = m_thandler;
		ldinfo->lpsh = m_shandler;
		ldinfo->lpmh = m_mhandler;
	}

	if( !ldinfo && m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, mqonameptr );
	}


	::SendMessage( m_hWnd, WM_USER_DISPLAY, 0, 0 );


	::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );


	return 0;
}

/***
int CMyD3DApplication::SetModuleDir()
{
	char filename[1024];
	ZeroMemory( filename, 1024 );
	int ret = 0;

	ret = GetModuleFileName( NULL, filename, 1024 );
	if( ret == 0 ){
		_ASSERT( 0 );
		DbgOut( "CMyD3DApplication : SetModuleDir : GetModuleFileName error !!!\n" );
		return 1;
	}
	int ch = '\\';
	char* lasten;
	lasten = strrchr( filename, ch );
	if( !lasten ){
		_ASSERT( 0 );
		DbgOut( "CMyD3DApplication : SetModuleDir : strrchr error !!!\n" );
		return 1;
	}
	int leng;
	char moduledir[1024];
	ZeroMemory( moduledir, 1024 );
	leng = (int)(lasten - filename + 1);
	strncpy( moduledir, filename, leng );

	::SetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPCTSTR)moduledir );
	
	DbgOut( "CMyD3DApplication : SetModuleDir : %s\n", moduledir );
	return 0;
}
***/

int CMyD3DApplication::SetRESDIR( char* pathname, CShdHandler* srclpsh )
{
	char* lastmark;
	lastmark = strrchr( pathname, '\\' );
	if( !lastmark )
		return 1;

	int dirleng;
	char dirname[_MAX_PATH];
	dirleng = (int)(lastmark - pathname + 1);
	ZeroMemory( dirname, _MAX_PATH );
	strncpy_s( dirname, _MAX_PATH, pathname, dirleng );

	::SetEnvironmentVariable( (LPCTSTR)"RESDIR", (LPCTSTR)dirname );
	
	DbgOut( "CMyD3DApplication : SetRESDIR : %s\n", dirname );

	if( srclpsh ){
		int ret;
		ret = srclpsh->SetDirName( pathname );
		if( ret ){
			DbgOut( "viewer : SetRESDIR : sh SetDirName error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CMyD3DApplication::ImportRokFile()
{

	m_materialdlg->ShowDlg( SW_HIDE );	

	int ret = 0;

	if( m_thandler || m_shandler || m_mhandler ){

		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"形状ファイルのインポートを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
			(LPCTSTR)"確認ダイアログ",
			MB_OKCANCEL );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK ){
			return 0;
		}
	}
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		ret = LoadRokFile();
		return ret;
	}


///////////
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"ROK File (*.rok)\0*.rok\0";
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
		return S_OK;
	}

	ERRORMES errormes;

	ret = SetRESDIR( buf, m_shandler );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportRokFile : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler();
	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : ImportSigFile : DestroyBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportRokFile : DestroyBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : ImportSigFile : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : ImportSigFile : DestroyMotionObj error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportRokFile : DestroyMotionObj error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}

	CRokFile* rokfile;
	rokfile = new CRokFile();
	if( !rokfile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportRokFile : rokfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = rokfile->LoadRokFile( buf, m_thandler, m_shandler, m_mhandler, m_hWnd, m_shandler->s2shd_leng );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = buf;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			delete rokfile;
			DbgOut( "CMyD3DApplication : LoadRokFile : rokfile->LoadRokFile error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "ImportRokFile : LoadRokFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	if( m_shandler->s2shd_leng <= 1 ){
		DestroyHandler();
		return 0;
	}

	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	}


	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadSigFile : CreateBoneInfo error !!!\n" );
		delete rokfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportRokFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
	}


	ret = RestoreHandler( 1 );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ImportRokFile : RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}



	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : ImportRokFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	
	delete rokfile;//!!!!!

	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, buf );
	}


	m_ExistData = TRUE;

	return 0;
}
int CMyD3DApplication::LoadRokFile()
{

	m_materialdlg->ShowDlg( SW_HIDE );	

	int ret;

	if( m_thandler || m_shandler || m_mhandler ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = ::MessageBox( m_hWnd,
			(LPCTSTR)"形状ファイルの読み込みを実行すると、\n編集中のモーションは失われます。\nよろしいですか?",
			(LPCTSTR)"確認ダイアログ",
			MB_OKCANCEL );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK ){
			return 0;
		}
	}

///////////
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"ROK File (*.rok)\0*.rok\0";
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
		return S_OK;
	}

	ERRORMES errormes;

////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler();
	if( m_thandler || m_shandler || m_mhandler ){
		DestroyHandler();
	}
	
	ret = CreateHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadRokFile : CreateHandler error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadRokFile : CreateHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = SetRESDIR( buf, m_shandler );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadRokFile : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	
	CRokFile* rokfile;
	rokfile = new CRokFile();
	if( !rokfile ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadRokFile : rokfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = rokfile->LoadRokFile( buf, m_thandler, m_shandler, m_mhandler, m_hWnd );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = buf;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			DbgOut( "CMyD3DApplication : LoadRokFile : rokfile->LoadRokFile error !!!\n" );
			delete rokfile;
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadRokFile : LoadRokFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}
	
	if( m_shandler->s2shd_leng <= 1 ){
		delete rokfile;
		DestroyHandler();
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();
		ChangeViewParam( ID_KEY_ALL_RESET );
		return 0;
	}
	
	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_TOON1:
	//	m_tbb_colT = 0;
	//	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );
		break;
	default:
		_ASSERT( 0 );
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	}


	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadRokFile : CreateBoneInfo error !!!\n" );
		delete rokfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadRokFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	ret = RestoreHandler( 1 );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadRokFile : RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}



	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : LoadRokFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	
	delete rokfile;//!!!!!

	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, buf );
	}


	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::SaveSigFile( char* filename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;

	if( !m_ExistData )
		return 0;


	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	char buf[_MAX_PATH];
	buf[0] = 0;

	if( !filename ){
		OPENFILENAME ofn;

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"SIG File (*.sig)\0*.sig\0";
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
		ofn.lpstrDefExt ="sig";
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( !GetSaveFileName(&ofn) ){
			return S_OK;
		}
	}else{
		strcpy_s( buf, _MAX_PATH, filename );
	}


////////////
	ERRORMES errormes;

	CSigFile* sigfile;
	sigfile = new CSigFile();
	if( !sigfile ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "SaveSigFile : sigfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	int ret;
	ret = sigfile->WriteSigFile( buf, m_thandler, m_shandler, m_mhandler );
	if( ret ){
		delete sigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "WriteSigFile error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	delete sigfile;

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, buf );
	}


	return 0;
}

int CMyD3DApplication::ChangeViewParam( int keyflag, float srcstep )
{
	int ret;
	float rad;
	float step;
	D3DXMATRIX invview;
	D3DXVECTOR3 upvec( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 newup;

	int curjoint;
	D3DXVECTOR3 jointpos;

	if( ( (m_lbutton == 1) && (m_movebone > 0) ) 
		|| ( (m_rbutton == 1) && (m_shiftbone > 0) ) 
	){
		return 0;
	}


	switch( keyflag ){
	case ID_KEY_RIGHT:
		if( srcstep >= 1000000.0f ){
			step = 5.0f;
		}else{
			step = srcstep;
		}
		m_degxz += step;
		if( m_degxz > 360.0f )
			m_degxz -= 360.0f;
		break;
	case ID_KEY_LEFT:
		if( srcstep >= 1000000.0f ){
			step = 5.0f;
		}else{
			step = srcstep;
		}
		m_degxz -= step;
		if( m_degxz < 0.0f )
			m_degxz += 360.0f;
		break;
	case ID_KEY_RAD2_UP:
		if( srcstep >= 1000000.0f ){
			step = 5.0f;
		}else{
			step = srcstep;
		}
		m_degy += step;
		if( m_degy > 90.0f )
			m_degy = 90.0f;
		break;
	case ID_KEY_RAD2_DOWN:
		if( srcstep >= 1000000.0f ){
			step = 5.0f;
		}else{
			step = srcstep;
		}
		m_degy -= step;
		if( m_degy < -90.0f )
			m_degy = -90.0f;
		break;
	case ID_KEY_UP:
		if( srcstep >= 1000000.0f ){
			step = 25.0f;
		}else{
			step = srcstep;
		}
		m_eye_y += step;
		m_targetpos.y = m_eye_y;
		break;
	case ID_KEY_DOWN:
		if( srcstep >= 1000000.0f ){
			step = 25.0f;
		}else{
			step = srcstep;
		}
		m_eye_y -= step;
		m_targetpos.y = m_eye_y;
		break;
	case ID_KEY_ZOOMIN:
		if( srcstep >= 1000000.0f ){
			step = 50.0f;
		}else{
			step = srcstep;
		}
		m_camdist -= step;
		if( m_camdist < 10.0f )
			m_camdist = 10.0f;
		break;
	case ID_KEY_ZOOMOUT:
		if( srcstep >= 1000000.0f ){
			step = 50.0f;
		}else{
			step = srcstep;
		}
		m_camdist += step;
		break;
	case ID_KEY_TARGET_LEFT:
		if( srcstep >= 1000000.0f ){
			step = 50.0f;
		}else{
			step = srcstep;
		}
		rad = (m_degxz + 90.0f) * (float)DEG2PAI;
		m_targetpos.x += step * (float)sin( rad );
		m_targetpos.z -= step * (float)cos( rad );
		break;
	case ID_KEY_TARGET_RIGHT:
		if( srcstep >= 1000000.0f ){
			step = 50.0f;
		}else{
			step = srcstep;
		}
		rad = (m_degxz - 90.0f) * (float)DEG2PAI;
		m_targetpos.x += step * (float)sin( rad );
		m_targetpos.z -= step * (float)cos( rad );
		break;
	case ID_KEY_TARGET_UP:
		if( srcstep >= 1000000.0f ){
			step = 50.0f;
		}else{
			step = srcstep;
		}
		invview = m_matView;
		invview._41 = 0.0f;
		invview._42 = 0.0f;
		invview._43 = 0.0f;
		D3DXMatrixInverse( &invview, NULL, &invview );

		upvec.x = 0.0f;
		upvec.y = 1.0f;
		upvec.z = 0.0f;
		D3DXVec3TransformCoord( &newup, &upvec, &invview );

		m_targetpos.x += step * newup.x;
		m_targetpos.y += step * newup.y;
		m_targetpos.z += step * newup.z;

		m_eye_y = m_targetpos.y;//!!!!!!!!

		break;
	case ID_KEY_TARGET_DOWN:
		if( srcstep >= 1000000.0f ){
			step = 50.0f;
		}else{
			step = srcstep;
		}
		invview = m_matView;
		invview._41 = 0.0f;
		invview._42 = 0.0f;
		invview._43 = 0.0f;
		D3DXMatrixInverse( &invview, NULL, &invview );

		upvec.x = 0.0f;
		upvec.y = -1.0f;
		upvec.z = 0.0f;
		D3DXVec3TransformCoord( &newup, &upvec, &invview );

		m_targetpos.x += step * newup.x;
		m_targetpos.y += step * newup.y;
		m_targetpos.z += step * newup.z;

		m_eye_y = m_targetpos.y;//!!!!!!!!

		break;
	case ID_KEY_UD_RESET:
		m_eye_y = EYEYINITVAL;
		break;
	case ID_KEY_ROT_RESET:
		m_degxz = DEGXZINITVAL;
		break;
	case ID_KEY_RAD2_RESET:
		m_degy = DEGYINITVAL;
		break;
	case ID_KEY_ZOOM_RESET:
		m_camdist = CAMDISTINITVAL;
		break;
	case ID_KEY_TARGET_RESET:
		m_targetpos.x = 0.0f;
		m_targetpos.y = m_eye_y;
		m_targetpos.z = 0.0f;
		break;
	case ID_KEY_ALL_RESET:
		m_eye_y = EYEYINITVAL;
		m_degxz = DEGXZINITVAL;
		m_degy = DEGYINITVAL;
		m_camdist = CAMDISTINITVAL;
		m_targetpos.x = 0.0f;
		m_targetpos.y = m_eye_y;
		m_targetpos.z = 0.0f;
		break;
	case ID_KEY_90LEFT:
		if( m_degxz > 270.0f ){
			m_degxz = 270.0f;
		}else if( m_degxz > 180.0f ){
			m_degxz = 180.0f;
		}else if( m_degxz > 90.0f ){
			m_degxz = 90.0f;
		}else if( m_degxz > 0.0f ){
			m_degxz = 0.0f;
		}else if( m_degxz == 0.0f ){
			m_degxz = 270.0f;
		}
		break;
	case ID_KEY_90RIGHT:
		if( m_degxz < 90.0f ){
			m_degxz = 90.0f;
		}else if( m_degxz < 180.0f ){
			m_degxz = 180.0f;
		}else if( m_degxz < 270.0f ){
			m_degxz = 270.0f;
		}else if( m_degxz < 360.0f ){
			m_degxz = 0.0f;
		}else if( m_degxz == 360.0f ){
			m_degxz = 90.0f;
		}

		break;
	case ID_KEY_90UP:
		if( m_degy < 0.0f ){
			m_degy = 0.0f;
		}else if( m_degy < 90.0f ){
			m_degy = 90.0f;
		}else if( m_degy == 90.0f ){
			m_degy = 90.0f;
		}
		break;
	case ID_KEY_90DOWN:
		if( m_degy > 0.0f ){
			m_degy = 0.0f;
		}else if( m_degy > -90.0f ){
			m_degy = -90.0f;
		}else if( m_degy == -90.0f ){
			m_degy = -90.0f;
		}
		break;
	case ID_KEY_SYMMX:
		m_degxz = 360.0f - m_degxz;
		if( m_degxz > 360.0f )
			m_degxz -= 360.0f;
		if( m_degxz < 0.0f )
			m_degxz += 360.0f;

		m_targetpos.x *= -1.0f;
		break;
	case ID_KEY_JOINTTARGET:
		if( g_motdlg ){
			ret = g_motdlg->GetSelectedJointPos( &curjoint, &jointpos );
			_ASSERT( !ret );
			if( curjoint > 0 ){
				m_targetpos = jointpos;
			}
		}
		//char strchk[512];
		//sprintf_s( strchk, "motdlg %x, joint %d, pos %f, %f, %f", g_motdlg, curjoint, jointpos.x, jointpos.y, jointpos.z );
		//::MessageBox( m_hWnd, strchk, "確認", MB_OK );

		break;
	default:
		break;
	}

	ret = m_cameradlg->SetParams( m_degxz, m_eye_y, m_camdist, m_targetpos, m_degy );
	_ASSERT( !ret );

	if( g_motdlg ){
		g_motdlg->AxisSetRenderParams( m_degy, m_degxz, m_gband, m_ground_sh, m_ground_mh, m_arrow_sh, m_arrow_mh, m_pFont );
	}



	return ret;

}

int CMyD3DApplication::ShowVersionDlg()
{
	CVersionDlg dlg;
	int ret;
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
	
	return 0;
}

int CMyD3DApplication::SetBg()
{
	ERRORMES errormes;

	CBgDlg dlg( g_inidata->bg_tex1, g_inidata->bg_tex2, g_inidata->bg_mvu, g_inidata->bg_mvv, g_inidata->bg_color );
	int ret;
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( ret != IDOK )
		return 0;

	delete m_bgdisp;
	
	ZeroMemory( g_inidata->bg_tex1, _MAX_PATH );
	ZeroMemory( g_inidata->bg_tex2, _MAX_PATH );
	if( *(dlg.name1) ){
		strcpy_s( g_inidata->bg_tex1, _MAX_PATH, dlg.name1 );
	}
	if( *(dlg.name2) ){
		strcpy_s( g_inidata->bg_tex2, _MAX_PATH, dlg.name2 );
	}
	g_inidata->bg_mvu = dlg.mvu;
	g_inidata->bg_mvv = dlg.mvv;
	g_inidata->bg_color = dlg.m_color;
	
	m_bgdisp = new CBgDisp2( m_pd3dDevice, g_inidata->bg_tex1, g_inidata->bg_tex2, g_inidata->bg_mvu, g_inidata->bg_mvv, g_inidata->bg_color, 0, g_proj_far );
	if( !m_bgdisp ){
		DbgOut( "CD3DApplication : SetBg : m_bgdisp alloc error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "SetBg m_bgdisp alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

int CMyD3DApplication::SetLight( int modalflag )
{

	if( modalflag != 0 )
		m_lightdlg->ShowDlg( SW_SHOWNORMAL );
	else
		m_lightdlg->ParamsToLight();


	return 0;
}

int CMyD3DApplication::SetBoneDisp()
{
	if( !m_shandler )
		return 0;

	g_inidata->opt_jointdisp = m_optdlg->m_bonedisp;//!!!!!!!!

	int bonedisp;
	bonedisp = m_optdlg->m_bonedisp;
	int ret1;
	if( bonedisp ){
		ret1 = m_shandler->SetDispFlag( 1 );
		if( ret1 ){
			_ASSERT( 0 );
		}
	}else{
		ret1 = m_shandler->SetDispFlag( 0 );
		if( ret1 ){
			_ASSERT( 0 );
		}
	}

	return 0;
}

int CMyD3DApplication::PickInfScopeVert( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int isindex, int* pickvert, int* pickvertnum )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;

	if( isindex < 0 ){
		*pickvertnum = 0;
		return 0;//!!!!!!!!!!!!!!
	}


	offsetptr->x = 0;
	offsetptr->y = 0;


	*selectptr = SELECT_NONE;//!!!!!!!!!!

	int part, vert;
	int getnum = 0;
	//int partarray[10];
	int vertarray[PICKVERTMAX];

	part = -1;
	vert = -1;

	D3DXMATRIX invview;
	D3DXMatrixInverse( &invview, NULL, &m_matView );

	D3DXVECTOR3 chkcampos;
	chkcampos.x = invview._41;
	chkcampos.y = invview._42;
	chkcampos.z = invview._43;

	DWORD dwClipWidth  = m_clientWidth/2;
	DWORD dwClipHeight = m_clientHeight/2;

	float calaspect;
//	if( m_matProj._43 < -1.0f ){
//		calaspect = 1.0f;
//	}else{
		//正射影
		calaspect = (float)dwClipWidth / (float)dwClipHeight;
//	}

	ret = m_shandler->PickVertInfScope( isindex, dwClipWidth, dwClipHeight, m_mhandler,
		m_matWorld, m_matView, m_matProjX, calaspect,
		srcx, srcy, 5, 5, vertarray, PICKVERTMAX, &getnum );
	if( ret ){
		DbgOut( "viewer : PickInfScopeVert : shandler PickVertInfScope error !!!\n" );
		_ASSERT( 0 );
		*pickvertnum = 0;
		return 1;
	}
	if( getnum > 0 ){
		vert = vertarray[0];
	}

	if( vert >= 0 ){

		D3DXVECTOR3 chkvpos;
		ret = m_shandler->GetVertPos2InfScope( isindex, m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
			vert, &chkvpos, 1 );
		if( ret ){
			DbgOut( "viewer : PickInfScopeVert : sh GetVertPos2 error !!!\n" );
			_ASSERT( 0 );
			*pickvertnum = 0;
			return 1;
		}

		//int chkpart, chkface, chkrev;
		int chkface, chkrev;
		D3DXVECTOR3 chkdstpos, chkdstn;
		ret = m_shandler->ChkConfLineAndFaceInfScope( isindex, m_clientWidth, m_clientHeight, 0, m_mhandler,
			m_matWorld, m_matView, m_matProjX,
			chkcampos, chkvpos, &chkface, &chkdstpos, &chkdstn, &chkrev );
		if( ret ){
			DbgOut( "viewer : PickInfScopeVert : shandler  ChkConfLineAndFace error !!!\n" );
			_ASSERT( 0 );
			*pickvertnum = 0;
			return 1;
		}

		if( chkface >= 0 ){
			int v1, v2, v3;
			ret = m_shandler->GetVertNoOfFaceInfScope( isindex, chkface, &v1, &v2, &v3 );
			if( ret ){
				DbgOut( "viewer : PickInfScopeVert : sh GetVertNoOfFace error !!!\n" );
				_ASSERT( 0 );
				*pickvertnum = 0;
				return 1;
			}
			
			D3DXVECTOR3 xv1, xv2, xv3;
			m_shandler->GetVertPos2InfScope( isindex, m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
				v1, &xv1, 1 );;
			m_shandler->GetVertPos2InfScope( isindex, m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
				v2, &xv2, 1 );;
			m_shandler->GetVertPos2InfScope( isindex, m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
				v3, &xv3, 1 );;

			if( (chkvpos != xv1) && (chkvpos != xv2) && (chkvpos != xv3) ){
				//とりけし
				vert = -1;
			}

			//if( (v1 != vert) && (v2 != vert) && (v3 != vert) ){
			//	//とりけし
			//	vert = -1;
			//}
		}
	}

	if( vert >= 0 ){
		*pickvertnum = getnum;
		MoveMemory( pickvert, vertarray, sizeof( int ) * getnum );
	}


//////////////
	float mag = 1e8;

	if( vert >= 0 ){
		*selectptr = SELECT_CENTER;


		int shiftscx, shiftscy;
		ret = m_shandler->GetScreenPosInfScope( m_mhandler, isindex, vert, &shiftscx, &shiftscy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickInfScopeVert : shandler GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			*pickvertnum = 0;
			return 1;
		}

		mag = ( ((float)srcx - (float)shiftscx) * ((float)srcx - (float)shiftscx) + ((float)srcy - (float)shiftscy) * ((float)srcy - (float)shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}



/////////


/////////////


	int scx, scy;
	int range = 6;

	float curmag;


	//// X軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickInfScopeVert : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_X;
		
			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : X %d : src %d %d, calc %d %d\r\n", m_sel_xno, srcx, srcy, scx, scy );

	/////　Ｙ軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickInfScopeVert : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_Y;
			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : Y %d : src %d %d, calc %d %d\r\n", m_sel_yno, srcx, srcy, scx, scy );

	/////　Ｚ軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickInfScopeVert : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_Z;

			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : Z %d : src %d %d, calc %d %d\r\n\r\n", m_sel_zno, srcx, srcy, scx, scy );


////////

	if( *selectptr != SELECT_NONE ){
		if( *selectptr == SELECT_CENTER ){
			m_selectedno = isindex + 1;//!!!!!!!!!!!!!!!!
			
		}else{
			offsetptr->x = 0;//!!!!!!!!!!!!!!!!!!!
			offsetptr->y = 0;
		}

		if( m_selectedno > 0 ){

			int shiftscx, shiftscy;
			ret = m_shandler->GetScreenPosInfScope( m_mhandler, isindex, *pickvert, &shiftscx, &shiftscy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
			if( ret ){
				DbgOut( "viewer : PickInfScopeVert : shandler GetScreenPos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			clickboneptr->x = shiftscx;
			clickboneptr->y = shiftscy;
		}

		return m_selectedno;
	}else{

		clickboneptr->x = 0;
		clickboneptr->y = 0;

		*pickvertnum = 0;

		return 0;
	}


	return 0;
}

int CMyD3DApplication::PickVert( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart, int* pickvert, int* pickvertnum, int selaxis )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;


	offsetptr->x = 0;
	offsetptr->y = 0;


	*selectptr = SELECT_NONE;//!!!!!!!!!!

	int part, vert;
	int getnum = 0;
	int partarray[PICKVERTMAX];
	int vertarray[PICKVERTMAX];

	part = -1;
	vert = -1;


	/***
	float maxdist = m_proj_far + 10000.0f;
	int face;
	D3DXVECTOR3 pos, n;
	float facedist;

	D3DXMATRIX matProjX;
	if( m_parsdlg->parsmode == PARSMODE_PARS ){
		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = (float)m_parsdlg->orthosize;
		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}

	ret = m_shandler->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_mhandler, m_matWorld, m_matView, matProjX, 
		srcx, srcy, &part, &face, &pos, &n, &facedist, 0 );
	if( ret ){
		DbgOut( "viewer : PickVert : shandler PickFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (part >= 0) && (face >= 0) ){
		ret = m_shandler->PickVert2( part, face, m_mhandler, srcx, srcy, 5, 5, partarray, vertarray, 10, &getnum );
		if( ret ){
			DbgOut( "viewer : PickVert : shandler PickVert error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( getnum > 0 ){
			part = partarray[0];
			vert = vertarray[0];
		}
	}
	***/

	//float maxdist = m_proj_far + 10000.0f;
	//int face;
	//D3DXVECTOR3 pos, n;
	//float facedist;

	/***
	D3DXMATRIX matProjX;
	if( m_parsdlg->parsmode == PARSMODE_PARS ){
		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = (float)m_parsdlg->orthosize;
		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}
	***/

	D3DXMATRIX invview;
	D3DXMatrixInverse( &invview, NULL, &m_matView );

	D3DXVECTOR3 chkcampos;
	chkcampos.x = invview._41;
	chkcampos.y = invview._42;
	chkcampos.z = invview._43;

	DWORD dwClipWidth  = m_clientWidth/2;
	DWORD dwClipHeight = m_clientHeight/2;

	float calaspect;
//	if( m_matProj._43 < -1.0f ){
//		calaspect = 1.0f;
//	}else{
		//正射影
		calaspect = (float)dwClipWidth / (float)dwClipHeight;
//	}

	//ret = m_shandler->PickVert3( chkcampos, m_mhandler, srcx, srcy, 5, 5, partarray, vertarray, PICKVERTMAX, &getnum );
	ret = m_shandler->PickVert( m_mhandler, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX,
		calaspect, srcx, srcy, 5, 5, partarray, vertarray, PICKVERTMAX, &getnum );
	if( ret ){
		DbgOut( "viewer : PickVert : shandler PickVert error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( getnum > 0 ){
		part = partarray[0];
		vert = vertarray[0];
	}

	if( m_tbb_ten2 == 0 ){
		if( (part > 0) && (vert >= 0) ){

			D3DXVECTOR3 chkvpos;
			ret = m_shandler->GetVertPos2( m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
				part, vert, &chkvpos, 1 );
			if( ret ){
				DbgOut( "viewer : PickVert : sh GetVertPos2 error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			int chkpart, chkface, chkrev;
			D3DXVECTOR3 chkdstpos, chkdstn;
			ret = m_shandler->ChkConfLineAndFace( m_clientWidth, m_clientHeight, 0, m_mhandler,
				m_matWorld, m_matView,// matProjX,
				chkcampos, chkvpos, 1, &chkpart, &chkface, &chkdstpos, &chkdstn, &chkrev );
			if( ret ){
				DbgOut( "viewer : PickVert : shandler  ChkConfLineAndFace error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			if( (chkpart >= 0) && (chkface >= 0) ){
				if( chkpart == part ){
					int v1, v2, v3;
					ret = m_shandler->GetVertNoOfFace( part, chkface, &v1, &v2, &v3 );
					if( ret ){
						DbgOut( "viewer : PickVert : sh GetVertNoOfFace error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					
					D3DXVECTOR3 xv1, xv2, xv3;
					m_shandler->GetVertPos2( m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
						part, v1, &xv1, 1 );;
					m_shandler->GetVertPos2( m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
						part, v2, &xv2, 1 );;
					m_shandler->GetVertPos2( m_mhandler, m_matWorld, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(),
						part, v3, &xv3, 1 );;

					if( (chkvpos != xv1) && (chkvpos != xv2) && (chkvpos != xv3) ){
						//とりけし
						part = -1;
						vert = -1;
					}

					//if( (v1 != vert) && (v2 != vert) && (v3 != vert) ){
					//	//とりけし
					//	part = -1;
					//	vert = -1;
					//}
				}else{
					//とりけし
					part = -1;
					vert = -1;
				}
			}

		}
	}


	if( (part >= 0) && (vert >= 0) ){
		int setno = 0;
		int gno;
		for( gno = 0; gno < getnum; gno++ ){
			if( partarray[gno] == part ){
				*( pickvert + setno ) = vertarray[ gno ];
				setno++;
			}
		}
		*pickvertnum = setno;
	}
	

//////////////
	float mag = 1e8;

	if( part >= 0 ){
		*selectptr = SELECT_CENTER;


		int shiftscx, shiftscy;
		ret = m_shandler->GetScreenPos( m_mhandler, part, vert, 0, &shiftscx, &shiftscy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickVert : shandler GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		mag = ( ((float)srcx - (float)shiftscx) * ((float)srcx - (float)shiftscx) + ((float)srcy - (float)shiftscy) * ((float)srcy - (float)shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}



/////////


/////////////
//    DWORD dwClipWidth  = m_clientWidth/2;
//    DWORD dwClipHeight = m_clientHeight/2;
	int scx, scy;
	int range = 6;

	float curmag;

	if( selaxis == 1 ){

	//// X軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_X;
		
			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : X %d : src %d %d, calc %d %d\r\n", m_sel_xno, srcx, srcy, scx, scy );

	/////　Ｙ軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_Y;
			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : Y %d : src %d %d, calc %d %d\r\n", m_sel_yno, srcx, srcy, scx, scy );

	/////　Ｚ軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_Z;

			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : Z %d : src %d %d, calc %d %d\r\n\r\n", m_sel_zno, srcx, srcy, scx, scy );

	}
////////

	if( *selectptr != SELECT_NONE ){
		if( *selectptr == SELECT_CENTER ){
			m_selectedno = part;//!!!!!!!!!!!!!!!!
			
			*pickpart = part;//---> m_shiftpart
			//*pickvert = vert;//---> m_shiftvert

		}else{
			offsetptr->x = 0;//!!!!!!!!!!!!!!!!!!!
			offsetptr->y = 0;
		}

		if( *pickpart > 0 ){

			int shiftscx, shiftscy;
			ret = m_shandler->GetScreenPos( m_mhandler, *pickpart, *pickvert, 0, &shiftscx, &shiftscy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
			if( ret ){
				DbgOut( "viewer : PickVert : shandler GetScreenPos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			clickboneptr->x = shiftscx;
			clickboneptr->y = shiftscy;
		}

		return m_selectedno;
	}else{

		clickboneptr->x = 0;
		clickboneptr->y = 0;

		*pickpart = -1;
		*pickvertnum = 0;

		return 0;
	}

}
int CMyD3DApplication::PickInfScopeFace( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int isindex, int* pickface )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;

	if( isindex < 0 ){
		*pickface = -1;
		return 0;//!!!!!!!!!!!!!
	}


	offsetptr->x = 0;
	offsetptr->y = 0;


	*selectptr = SELECT_NONE;//!!!!!!!!!!

	float maxdist = m_proj_far + 10000.0f;
	int face = -1;
	D3DXVECTOR3 pos, n;
	float facedist;

	/***
	D3DXMATRIX matProjX;
	if( m_parsdlg->parsmode == PARSMODE_PARS ){
		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = (float)m_parsdlg->orthosize;
		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}
	***/

	ret = m_shandler->PickInfScopeFace( isindex, maxdist, m_clientWidth, m_clientHeight, 0, m_mhandler, m_matWorld, m_matView, m_matProjX, 
		srcx, srcy, &face, &pos, &n, &facedist );
	if( ret ){
		DbgOut( "viewer : PickInfScopeFace : shandler PickInfScopeFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//////////////
	float mag = 1e8;

	if( face >= 0 ){
		*selectptr = SELECT_CENTER;


		D3DXVECTOR3 partpos;
		ret = m_shandler->GetInitialInfScopeCenter( isindex, &partpos, m_matWorld );
		if( ret ){
			DbgOut( "viewer : PickInfScopeFace : shandler GetInfScopeCenterr error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		float shiftscx, shiftscy, shiftscz, wp;
		ret = GetScreenPos( partpos, &shiftscx, &shiftscy, &shiftscz, &wp );
		if( ret ){
			DbgOut( "viewer : PickInfScopeFace : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		mag = ( ((float)srcx - (float)shiftscx) * ((float)srcx - (float)shiftscx) + ((float)srcy - (float)shiftscy) * ((float)srcy - (float)shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}

/////////////
	DWORD dwClipWidth  = m_clientWidth/2;
	DWORD dwClipHeight = m_clientHeight/2;

	int scx, scy;
	int range = 6;

	float curmag;

//// X軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickInfScopeFace : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_X;
	
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : X %d : src %d %d, calc %d %d\r\n", m_sel_xno, srcx, srcy, scx, scy );

/////　Ｙ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickInfScopeFace : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Y;
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Y %d : src %d %d, calc %d %d\r\n", m_sel_yno, srcx, srcy, scx, scy );

/////　Ｚ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickInfScopeFace : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Z;

		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Z %d : src %d %d, calc %d %d\r\n\r\n", m_sel_zno, srcx, srcy, scx, scy );

////////


	if( *selectptr != SELECT_NONE ){
		if( *selectptr == SELECT_CENTER ){
			m_selectedno = isindex + 1;//!!!!!!!!!!!!!!!!
			
			*pickface = face;// ---> m_shiftpart

		}else{

			offsetptr->x = 0;//!!!!!!!!!!!!!!!!!!!
			offsetptr->y = 0;
		}

//DbgOut( "viewer : PickDispObj : part %d, select %d, m_selectedno %d\r\n", part, *selectptr, m_selectedno );


		if( m_selectedno > 0 ){


			D3DXVECTOR3 partpos;
			ret = m_shandler->GetInitialInfScopeCenter( isindex, &partpos, m_matWorld );
			if( ret ){
				DbgOut( "viewer : PickInfScopeFace : shandler GetInitialFacePos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			float shiftscx, shiftscy, shiftscz, wp;
			ret = GetScreenPos( partpos, &shiftscx, &shiftscy, &shiftscz, &wp );
			if( ret ){
				DbgOut( "viewer : PickInfScopeFace : GetScreenPos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			clickboneptr->x = (int)shiftscx;
			clickboneptr->y = (int)shiftscy;

		}

		return m_selectedno;
	}else{

		clickboneptr->x = 0;
		clickboneptr->y = 0;

		*pickface = -1;

		return 0;
	}


	return 0;
}



int CMyD3DApplication::PickFace( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart, int* pickface )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;


	offsetptr->x = 0;
	offsetptr->y = 0;


	*selectptr = SELECT_NONE;//!!!!!!!!!!

	float maxdist = m_proj_far + 10000.0f;
	int part, face;
	D3DXVECTOR3 pos, n;
	float facedist;


	/***
	D3DXMATRIX matProjX;
	if( m_parsdlg->parsmode == PARSMODE_PARS ){
		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = (float)m_parsdlg->orthosize;
		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}
	***/

	ret = m_shandler->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_mhandler, m_matWorld, m_matView, m_matProjX, 
		0, 0,
		srcx, srcy, &part, &face, &pos, &n, &facedist, 1 );
	if( ret ){
		DbgOut( "viewer : PickVert : shandler PickFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//////////////
	float mag = 1e8;

	if( part > 0 ){
		*selectptr = SELECT_CENTER;


		D3DXVECTOR3 facepos;
		ret = m_shandler->GetInitialFacePos( part, face, &facepos, m_matWorld );
		if( ret ){
			DbgOut( "viewer : PickVert : shandler GetInitialFacePos error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		float shiftscx, shiftscy, shiftscz, wp;
		ret = GetScreenPos( facepos, &shiftscx, &shiftscy, &shiftscz, &wp );
		if( ret ){
			DbgOut( "viewer : PickVert : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		mag = ( ((float)srcx - (float)shiftscx) * ((float)srcx - (float)shiftscx) + ((float)srcy - (float)shiftscy) * ((float)srcy - (float)shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}



/////////


/////////////
	DWORD dwClipWidth  = m_clientWidth/2;
	DWORD dwClipHeight = m_clientHeight/2;

	int scx, scy;
	int range = 6;

	float curmag;

//// X軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_X;
	
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : X %d : src %d %d, calc %d %d\r\n", m_sel_xno, srcx, srcy, scx, scy );

/////　Ｙ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Y;
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Y %d : src %d %d, calc %d %d\r\n", m_sel_yno, srcx, srcy, scx, scy );

/////　Ｚ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Z;

		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Z %d : src %d %d, calc %d %d\r\n\r\n", m_sel_zno, srcx, srcy, scx, scy );

////////

	if( *selectptr != SELECT_NONE ){
		if( *selectptr == SELECT_CENTER ){
			m_selectedno = part;//!!!!!!!!!!!!!!!!
			
			*pickpart = part;// ---> m_shiftpart
			*pickface = face;// ---> m_shiftface

		}else{
			offsetptr->x = 0;//!!!!!!!!!!!!!!!!!!!
			offsetptr->y = 0;
		}

		if( m_selectedno > 0 ){


			D3DXVECTOR3 facepos;
			ret = m_shandler->GetInitialFacePos( m_shiftpart, m_shiftface, &facepos, m_matWorld );
			if( ret ){
				DbgOut( "viewer : PickVert : shandler GetInitialFacePos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			float shiftscx, shiftscy, shiftscz, wp;
			ret = GetScreenPos( facepos, &shiftscx, &shiftscy, &shiftscz, &wp );
			if( ret ){
				DbgOut( "viewer : PickVert : GetScreenPos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			clickboneptr->x = (int)shiftscx;
			clickboneptr->y = (int)shiftscy;
		}

		return m_selectedno;
	}else{

		clickboneptr->x = 0;
		clickboneptr->y = 0;

		*pickpart = -1;
		*pickface = -1;

		return 0;
	}
}
int CMyD3DApplication::PickDispObj( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;


	offsetptr->x = 0;
	offsetptr->y = 0;


	*selectptr = SELECT_NONE;//!!!!!!!!!!

	float maxdist = m_proj_far + 10000.0f;
	int part, face;
	D3DXVECTOR3 pos, n;
	float facedist;

	/***
	D3DXMATRIX matProjX;
	if( m_parsdlg->parsmode == PARSMODE_PARS ){
		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = (float)m_parsdlg->orthosize;
		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}
	***/

	ret = m_shandler->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_mhandler, m_matWorld, m_matView, m_matProjX, 
		0, 0,	
		srcx, srcy, &part, &face, &pos, &n, &facedist, 1 );
	if( ret ){
		DbgOut( "viewer : PickDispObj : shandler PickFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//DbgOut( "viewer : pickdispobj %d %d\r\n", part, face );

//////////////
	float mag = 1e8;

	if( part > 0 ){
		*selectptr = SELECT_CENTER;


		D3DXVECTOR3 partpos;
		ret = m_shandler->GetInitialPartPos( part, &partpos, m_matWorld );
		if( ret ){
			DbgOut( "viewer : PickVert : shandler GetInitialPartPos error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		float shiftscx, shiftscy, shiftscz, wp;
		ret = GetScreenPos( partpos, &shiftscx, &shiftscy, &shiftscz, &wp );
		if( ret ){
			DbgOut( "viewer : PickVert : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		mag = ( ((float)srcx - (float)shiftscx) * ((float)srcx - (float)shiftscx) + ((float)srcy - (float)shiftscy) * ((float)srcy - (float)shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}



/////////


/////////////
	DWORD dwClipWidth  = m_clientWidth/2;
	DWORD dwClipHeight = m_clientHeight/2;

	int scx, scy;
	int range = 6;

	float curmag;

//// X軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_X;
	
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : X %d : src %d %d, calc %d %d\r\n", m_sel_xno, srcx, srcy, scx, scy );

/////　Ｙ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Y;
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Y %d : src %d %d, calc %d %d\r\n", m_sel_yno, srcx, srcy, scx, scy );

/////　Ｚ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Z;

		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Z %d : src %d %d, calc %d %d\r\n\r\n", m_sel_zno, srcx, srcy, scx, scy );

////////


	if( *selectptr != SELECT_NONE ){
		if( *selectptr == SELECT_CENTER ){
			m_selectedno = part;//!!!!!!!!!!!!!!!!
			
			*pickpart = part;// ---> m_shiftpart

		}else{

			offsetptr->x = 0;//!!!!!!!!!!!!!!!!!!!
			offsetptr->y = 0;
		}

//DbgOut( "viewer : PickDispObj : part %d, select %d, m_selectedno %d\r\n", part, *selectptr, m_selectedno );


		if( m_selectedno > 0 ){


			D3DXVECTOR3 partpos;
			ret = m_shandler->GetInitialPartPos( m_shiftpart, &partpos, m_matWorld );
			if( ret ){
				DbgOut( "viewer : PickVert : shandler GetInitialFacePos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			float shiftscx, shiftscy, shiftscz, wp;
			ret = GetScreenPos( partpos, &shiftscx, &shiftscy, &shiftscz, &wp );
			if( ret ){
				DbgOut( "viewer : PickVert : GetScreenPos error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			clickboneptr->x = (int)shiftscx;
			clickboneptr->y = (int)shiftscy;

		}

		return m_selectedno;
	}else{

		clickboneptr->x = 0;
		clickboneptr->y = 0;

		*pickpart = -1;

		return 0;
	}
}



int CMyD3DApplication::PickIKT( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;


	offsetptr->x = 0;
	offsetptr->y = 0;


	*selectptr = SELECT_NONE;//!!!!!!!!!!

	int pickno = 0;
	int seri;
	CShdElem* selem;
	float chkmag;
	float minmag = 1e6;
	float pickrange = 100.0f;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		selem = (*m_shandler)( seri );

		if( selem->IsJoint() && (selem->type != SHDMORPH) && (selem->m_iktarget != 0) ){
			float scx, scy, scz, wp;
			ret = GetScreenPos( selem->m_iktargetpos, &scx, &scy, &scz, &wp );
			if( ret ){
				DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			chkmag = ( (float)srcx - scx ) * ( (float)srcx - scx ) + ( (float)srcy - scy ) * ( (float)srcy - scy );
			if( (chkmag <= minmag) && (chkmag <= pickrange) ){
				pickno = seri;
				minmag = chkmag;
			}
		}
	}


//////////////
	float mag = 1e8;

	if( pickno > 0 ){
		*selectptr = SELECT_CENTER;
		//return pickno;
		//m_selectedno = pickno;

		//CShdElem* shiftbone = (*m_shandler)( m_selectedno );
		CShdElem* shiftikt = (*m_shandler)( pickno );
		if( !shiftikt )
			return 0;

		float shiftscx, shiftscy, shiftscz, wp;
		ret = GetScreenPos( shiftikt->m_iktargetpos, &shiftscx, &shiftscy, &shiftscz, &wp );
		if( ret ){
			DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return 0;
		}

		mag = ( ((float)srcx - shiftscx) * ((float)srcx - shiftscx) + ((float)srcy - shiftscy) * ((float)srcy - shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}



/////////


/////////////
    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;
	int scx, scy;
	int range = 6;

	float curmag;

//// X軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_X;
	
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : X %d : src %d %d, calc %d %d\r\n", m_sel_xno, srcx, srcy, scx, scy );

/////　Ｙ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Y;
		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Y %d : src %d %d, calc %d %d\r\n", m_sel_yno, srcx, srcy, scx, scy );

/////　Ｚ軸
	ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
	if( ret ){
		DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
		curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
		if( curmag < mag )
			*selectptr = SELECT_Z;

		//return m_selectedno;
	}

//DbgOut( "viewer : PickBone : Z %d : src %d %d, calc %d %d\r\n\r\n", m_sel_zno, srcx, srcy, scx, scy );

////////

	if( *selectptr != SELECT_NONE ){
		if( *selectptr == SELECT_CENTER ){
			m_selectedno = pickno;//!!!!!!!!!!!!!!!!
		}else{
			offsetptr->x = 0;//!!!!!!!!!!!!!!!!!!!
			offsetptr->y = 0;
		}

		if( m_selectedno > 0 ){

			CShdElem* curikt = (*m_shandler)( m_selectedno );

			float shiftscx, shiftscy, shiftscz, wp;
			ret = GetScreenPos( curikt->m_iktargetpos, &shiftscx, &shiftscy, &shiftscz, &wp );
			if( ret ){
				DbgOut( "viewer : PickIKT : GetScreenPos error !!!\n" );
				_ASSERT( 0 );
				return 0;
			}

			clickboneptr->x = (int)shiftscx;
			clickboneptr->y = (int)shiftscy;
		}

		return m_selectedno;
	}else{

		clickboneptr->x = 0;
		clickboneptr->y = 0;

		return 0;
	}
}


int CMyD3DApplication::PickBone( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int selaxis )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;


	offsetptr->x = 0;
	offsetptr->y = 0;

    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;

	*selectptr = SELECT_NONE;//!!!!!!!!!!

	float calaspect;
//	if( m_matProj._43 < -1.0f ){
//		calaspect = 1.0f;
//	}else{
		//正射影
		calaspect = (float)dwClipWidth / (float)dwClipHeight;
//	}

	int pickno;
	if( g_motdlg->GetSellockno() == 0 ){
		pickno = m_shandler->PickBone( m_mhandler, (int)srcx, (int)srcy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX, calaspect );
	}else{
		pickno = g_motdlg->GetSellockno();
	}

	float mag = 1e8;

	if( pickno > 0 ){
		*selectptr = SELECT_CENTER;
		//return pickno;
		//m_selectedno = pickno;

		//CShdElem* shiftbone = (*m_shandler)( m_selectedno );

//int CShdHandler::GetScreenPos3( CMotHandler* lpmh, int srcseri, int vertno, int calcmode, int* scxptr, int* scyptr, float* sczptr, 
//							   DWORD dwClipWidth, DWORD dwClipHeight, D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj )

		/***
		CShdElem* shiftbone = (*m_shandler)( pickno );
		if( !shiftbone )
			return 0;
		float shiftscx, shiftscy, shiftscz, wp;	
		ret = shiftbone->GetBoneScreenPos( &shiftscx, &shiftscy, &shiftscz, &wp );
		if( ret != 0 )
			return 0;
		***/
		int shiftscx, shiftscy;
		float shiftscz;
		ret = m_shandler->GetScreenPos3( m_mhandler, pickno, 0, 1, &shiftscx, &shiftscy, &shiftscz, dwClipWidth, dwClipHeight,
			m_matWorld, m_matView, m_matProjX );
		if( ret != 0 )
			return 0;//!!!!!!!!!

		mag = (float)( (srcx - shiftscx) * (srcx - shiftscx) + (srcy - shiftscy) * (srcy - shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}



/////////


/////////////
	int scx, scy;
	int range = 6;

	float curmag;

	if( selaxis == 1 ){

	//// X軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickBone : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_X;
		
			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : X %d : src %d %d, calc %d %d\r\n", m_sel_xno, srcx, srcy, scx, scy );

	/////　Ｙ軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickBone : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_Y;
			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : Y %d : src %d %d, calc %d %d\r\n", m_sel_yno, srcx, srcy, scx, scy );

	/////　Ｚ軸
		ret = m_select_sh->GetScreenPos( m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
		if( ret ){
			DbgOut( "viewer : PickBone : GetScreenPos error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (scx > (srcx - range)) && (scx < (srcx + range)) &&  (scy > (srcy - range)) && (scy < (srcy + range)) ){
			curmag = (float)(( scx - srcx ) * ( scx - srcx ) + ( scy - srcy ) * ( scy - srcy ));
			if( curmag < mag )
				*selectptr = SELECT_Z;

			//return m_selectedno;
		}

	//DbgOut( "viewer : PickBone : Z %d : src %d %d, calc %d %d\r\n\r\n", m_sel_zno, srcx, srcy, scx, scy );

	////////
	}


	if( *selectptr != SELECT_NONE ){
		if( *selectptr == SELECT_CENTER ){
			m_selectedno = pickno;//!!!!!!!!!!!!!!!!
		}else{
			offsetptr->x = 0;//!!!!!!!!!!!!!!!!!!!
			offsetptr->y = 0;
		}

		if( m_selectedno > 0 ){

			/***
			CShdElem* curbone = (*m_shandler)( m_selectedno );

			float shiftscx, shiftscy, shiftscz, wp;	
			ret = curbone->GetBoneScreenPos( &shiftscx, &shiftscy, &shiftscz, &wp );
			if( ret != 0 )
				return 0;
			***/
//int CShdHandler::GetScreenPos3( CMotHandler* lpmh, int srcseri, int vertno, int calcmode, int* scxptr, int* scyptr, float* sczptr, 
//							   DWORD dwClipWidth, DWORD dwClipHeight, D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj )

			int shiftscx, shiftscy;
			float shiftscz;
			ret = m_shandler->GetScreenPos3( m_mhandler, m_selectedno, 0, 1, &shiftscx, &shiftscy, &shiftscz, dwClipWidth, dwClipHeight,
				m_matWorld, m_matView, m_matProjX );
			if( ret != 0 )
				return 0;//!!!!!!!
			clickboneptr->x = (int)shiftscx;
			clickboneptr->y = (int)shiftscy;
		}

		return m_selectedno;
	}else{

		clickboneptr->x = 0;
		clickboneptr->y = 0;

		return 0;
	}
	//*selectptr = SELECT_NONE;
	//return 0;
}



int CMyD3DApplication::ScreenToObj( D3DXVECTOR3* objpos, D3DXVECTOR3 scpos )
{

	float clipw, cliph, asp;
	clipw = (float)m_viewport.Width / 2.0f;
	cliph = (float)m_viewport.Height / 2.0f;
	//asp = m_shandler->m_aspect;

	//float calaspect;
//	if( m_matProj._43 < -1.0f ){
//		asp = m_shandler->m_aspect;
//	}else{
		//正射影
		asp = clipw / cliph;
//	}


	D3DXMATRIX mat, invmat;
	D3DXMATRIX* retmat;

	mat = m_matWorld * m_matView * m_matProjX;
	retmat = D3DXMatrixInverse( &invmat, NULL, &mat );
	if( retmat == NULL ){
		_ASSERT( 0 );
		return 1;
	}
	

	float res1;
	D3DXVECTOR3 screenpos;

	screenpos.x = ( scpos.x - clipw ) / ( asp * cliph );
	screenpos.y = ( cliph - scpos.y ) / cliph;
	screenpos.z = scpos.z;

	objpos->x = screenpos.x * invmat._11 + screenpos.y * invmat._21 + screenpos.z * invmat._31 + invmat._41;
	objpos->y = screenpos.x * invmat._12 + screenpos.y * invmat._22 + screenpos.z * invmat._32 + invmat._42;
	objpos->z = screenpos.x * invmat._13 + screenpos.y * invmat._23 + screenpos.z * invmat._33 + invmat._43;
	res1 =		screenpos.x * invmat._14 + screenpos.y * invmat._24 + screenpos.z * invmat._34 + invmat._44;

	
	if( res1 == 0.0f ){
		_ASSERT( 0 );
		return 1;
	}

	objpos->x /= res1;
	objpos->y /= res1;
	objpos->z /= res1;


	return 0;
}


int CMyD3DApplication::SetBoneArray( int srcseri )
{
	//ZeroMemory( m_bonearray, sizeof(int) * (MAXBONENUM + 1) );

	int setnum = 0;
	CShdElem* selem;
	selem = (*m_shandler)( srcseri );

	SetBoneArrayReq( selem, &setnum, 0 );

	m_bonearray[ setnum ] = 0;
	setnum++;

	return 0;
}


void CMyD3DApplication::SetBoneArrayReq( CShdElem* selem, int* setnum, int broflag )
{
	if( selem->IsJoint() && (selem->type != SHDMORPH) ){
		if( *setnum < MAXBONENUM ){
			m_bonearray[ *setnum ] = selem->serialno;
			(*setnum)++;
		}else{
			DbgOut( "CMyD3DApplication : SetBoneArray : setnum overflow !!!\n" );
			_ASSERT( 0 );
			return;
		}
	}

	if( broflag ){
		CShdElem* broelem;
		broelem = selem->brother;
		if( broelem ){
			SetBoneArrayReq( broelem, setnum, 1 );
		}
	}

	CShdElem* chilelem;
	chilelem = selem->child;
	if( chilelem ){
		SetBoneArrayReq( chilelem, setnum, 1 );
	}

}

int CMyD3DApplication::CreateProgressBar()
{
	if( !m_hWnd ){
		DbgOut( "viewer : CreateProgress Bar : parent wnd NULL error !!!\n" );
		return 1;
	}
	if( m_pbwnd ){
		DbgOut( "viewer : CreateProgress Bar : pbwnd already exist warning !!!\n" );
		return 0;//!!!!!
	}

    RECT rcClient;  // Client area of parent window 
    int cyVScroll;  // Height of scroll bar arrow 

	HINSTANCE hinst = (HINSTANCE)GetModuleHandle(NULL);

    GetClientRect(m_hWnd, &rcClient); 
    cyVScroll = GetSystemMetrics(SM_CYVSCROLL); 
    m_pbwnd = CreateWindowEx(0, PROGRESS_CLASS,
	         (LPSTR)NULL, WS_CHILD | WS_VISIBLE,
             rcClient.left, rcClient.bottom - cyVScroll,
             rcClient.right - rcClient.left, cyVScroll, 
             m_hWnd, (HMENU) 0, hinst, NULL); 

	if( !m_pbwnd ){
		DbgOut( "viewer : CreateProgress Bar : CreateWindowEx error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CMyD3DApplication::SetProgressBar( int newpos )
{
	if( newpos < 0 )
		newpos = 0;
	if( newpos > 100 )
		newpos = 100;

	if( !m_pbwnd ){
		DbgOut( "viewer : SetProgressBar : Progress bar still not exist error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	SendMessage( m_pbwnd, PBM_SETPOS, newpos, 0 );

	return 0;
}
int CMyD3DApplication::DestroyProgressBar()
{
	if( m_pbwnd ){
		DestroyWindow( m_pbwnd );
		m_pbwnd = 0;
	}

	return 0;
}

int CMyD3DApplication::WriteMQOFile()
{
	if( !m_thandler || !m_shandler ){
		MessageBox( m_hWnd, "データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
		return 0;
	}

	CWriteMQODlg dlg;
	int ret;

	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( ret != IDOK )
		return 0;

	CMQOFile* mqofile;
	mqofile = new CMQOFile( m_hWnd );
	if( !mqofile ){
		DbgOut( "viewer : WriteMQOFile : mqofile alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int rangemax;
	ret = g_motdlg->GetRangeMax( g_motdlg->GetMotCookie(), &rangemax );//motid 0
	if( ret ){
		DbgOut( "viewer : WriteMQOFile : GetRangeMax error !!!\n" );
		_ASSERT( 0 );
		delete mqofile;
		return 1;
	}

	int* keyframe;
	keyframe = (int*)malloc( sizeof( int ) * (rangemax + 1) );
	if( !keyframe ){
		DbgOut( "viewer : WriteMQOFile : keyframe alloc error !!!\n" );
		_ASSERT( 0 );
		delete mqofile;
		return 1;
	}

	int keynum = 0;
	ret = g_motdlg->GetKeyFrameNo( g_motdlg->GetMotCookie(), keyframe, &keynum );//motid 0
	if( ret ){
		DbgOut( "viewer : WriteMQOFile : GetKeyFrameNo error !!!\n" );
		_ASSERT( 0 );
		delete mqofile;
		free( keyframe );
		return 1;
	}

//DbgOut( "viewer : WriteMQOFile : GetKeyFrameNo %d\r\n", keynum );

	char outname[4098];

	::SendMessage( m_hWnd, WM_USER_CREATE_PB, 0, 0 );
	::SendMessage( m_hWnd, WM_USER_SET_PB, 0, 0 );

	int rate;

	if( dlg.m_outmode == 0 ){
		//all

		int frameno;
		for( frameno = 0; frameno <= rangemax; frameno++ ){

			if( rangemax != 0 ){
				rate = frameno * 100 / rangemax;
			}else{
				rate = 50;
			}
			::SendMessage( m_hWnd, WM_USER_SET_PB, (WPARAM)rate, 0 );

			char strframe5[10];
			ZeroMemory( strframe5, sizeof( char ) * 10 );
			Conv5Digits( frameno, strframe5 );
			sprintf_s( outname, 4098, "%s_%s.mqo", dlg.m_head, strframe5 );

			ret = mqofile->SaveMQOFileOnFrame( g_motdlg->GetMotCookie(), frameno, outname, m_thandler, m_shandler, m_mhandler, dlg.m_mult, dlg.m_outbone );
			if( ret ){
				DbgOut( "viewer : WriteMQOFile : SaveMQOFileOnFrame error !!!\n" );
				_ASSERT( 0 );
				delete mqofile;
				free( keyframe );
				::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
				return 1;
			}
		}

	}else{
		//key
		
		int saveno;
		int frameno;
		for( saveno = 0; saveno < keynum; saveno++ ){
			frameno = *( keyframe + saveno );

			rate = frameno * 100 / rangemax;
			::SendMessage( m_hWnd, WM_USER_SET_PB, (WPARAM)rate, 0 );


			char strframe5[10];
			ZeroMemory( strframe5, sizeof( char ) * 10 );
			Conv5Digits( frameno, strframe5 );
			sprintf_s( outname, 4098, "%s_%s.mqo", dlg.m_head, strframe5 );

			ret = mqofile->SaveMQOFileOnFrame( g_motdlg->GetMotCookie(), frameno, outname, m_thandler, m_shandler, m_mhandler, dlg.m_mult, dlg.m_outbone );
			if( ret ){
				DbgOut( "viewer : WriteMQOFile : SaveMQOFileOnFrame error !!!\n" );
				_ASSERT( 0 );
				delete mqofile;
				free( keyframe );
				::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
				return 1;
			}

		}
		
		if( keynum == 0 ){
			::MessageBox( m_hWnd, "キーフレームが一個もないので、出力できません。", "キーフレ無し", MB_OK );
		}


	}

	delete mqofile;
	free( keyframe );



	::SendMessage( m_hWnd, WM_USER_DESTROY_PB, 0, 0 );


	g_motdlg->OnStop();// frameno, curmat を最初のフレームに戻す。

/////////
	if( dlg.m_outbonefile ){
		ret = WriteBoneFile();
		if( ret ){
			DbgOut( "viewer : WriteMQOFile : WriteBoneFile error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CMyD3DApplication::Conv5Digits( int srcseri, char* dstptr )
{
	//if( (srcseri < 0) || (srcseri > 9999) ){
	if( srcseri > 99999 ){
		DbgOut( "BoneFile : Conv4Digits : srcseri range error !!!\n" );
		_ASSERT( 0 );
		*dstptr = 0;
		return 1;
	}
	
	int minusflag;
	if( srcseri < 0 ){
		minusflag = 1;
		srcseri *= -1;//!!!!!!!!
	}else{
		minusflag = 0;
	}

	int zerohead;
	if( srcseri < 10 ){
		zerohead = 4;
	}else if( srcseri < 100 ){
		zerohead = 3;
	}else if( srcseri < 1000 ){
		zerohead = 2;
	}else if( srcseri < 10000 ){
		zerohead = 1;
	}else{
		zerohead = 0;
	}

	*dstptr = 0;//!!!!

	if( minusflag == 1 ){
		strcat_s( dstptr, 6, "-" );//!!!!!!!!!
	}

	int headno;
	for( headno = 0; headno < zerohead; headno++ ){
		strcat_s( dstptr, 6, "0" );
	}

	char numstr[6];
	sprintf_s( numstr, 6, "%d", srcseri );

	strcat_s( dstptr, 6, numstr );

	return 0;
}



int CMyD3DApplication::WriteMAFile( char* dirname, char* moaname )
{
	if( !m_thandler || !m_shandler ){
		MessageBox( m_hWnd, "データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
		return 0;
	}
	
	int ret;

	//CWriteMAFDlg dlg;
	char curdirname[MAX_PATH];
	char curmoaname[MAX_PATH];
	int overwriteflag;

	if( !dirname || !moaname ){
		int dlgret;
		dlgret = (int)m_wmafdlg->DoModal( m_hWnd );
		if( dlgret != IDOK ){
			return 0;//!!!!!!!!!!!!!!!
		}
		strcpy_s( curdirname, MAX_PATH, m_wmafdlg->m_newdirname );
		strcpy_s( curmoaname, MAX_PATH, m_wmafdlg->m_mafname );

		overwriteflag = 0;
	}else{
		strcpy_s( curdirname, MAX_PATH, dirname );
		strcpy_s( curmoaname, MAX_PATH, moaname );

		overwriteflag = 1;
	}

	//MessageBox( m_hWnd, dlg.m_parent, dlg.m_mafname, MB_OK );

	ret = m_mcdlg->SaveMAFile( curdirname, curmoaname, overwriteflag );
	if( ret ){
		DbgOut( "viewer : WriteMAFile : mcdlg SaveMAFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	if(m_loadnamedlg){
		sprintf_s( m_loadnamedlg->m_moaname, 2048, "%s\\%s.moa", curdirname, curmoaname );
	}
	
	return 0;
}



LPITEMIDLIST GetNextItemID(LPCITEMIDLIST pidl) 
   { 
   // Check for valid pidl.
   if(pidl == NULL)
      return NULL;

   // Get the size of the specified item identifier. 
   int cb = pidl->mkid.cb; 

   // If the size is zero, it is the end of the list. 
   if (cb == 0) 
      return NULL; 

   // Add cb to pidl (casting to increment by bytes). 
   pidl = (LPITEMIDLIST) (((LPBYTE) pidl) + cb); 

   // Return NULL if it is null-terminating, or a pidl otherwise. 
   return (pidl->mkid.cb == 0) ? NULL : (LPITEMIDLIST) pidl; 
   } 


int CMyD3DApplication::WriteBoneFile()
{
	if( !m_thandler || !m_shandler ){
		MessageBox( m_hWnd, "データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
		return 0;
	}

	CBoneFile* bonefile;
	bonefile = new CBoneFile();
	if( !bonefile ){
		DbgOut( "viewer3 : WriteBoneFile : bonefile alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	OPENFILENAME ofn;
	char buf[_MAX_PATH];

	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Bone File (*.bon)\0*.bon\0";
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
	ofn.lpstrDefExt = "bon";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 ){
		delete bonefile;
		return S_OK;
	}

	CWriteBFDlg* bfdlg;
	bfdlg = new CWriteBFDlg();
	if( !bfdlg ){
		DbgOut( "viewer3 : WriteBoneFile : bfdlg alloc error !!!\n" );
		_ASSERT( 0 );
		delete bonefile;
		return 1;
	}

	bfdlg->DoModal( m_hWnd );


	int ret;
	ret = bonefile->WriteBoneFile( buf, m_thandler, m_shandler, bfdlg->m_bftype, bfdlg->m_relativeflag );
	if( ret ){
		DbgOut( "viewer3 : WriteBoneFile : bonefile WriteBoneFile error !!!\n" );
		_ASSERT( 0 );
		delete bonefile;
		delete bfdlg;
		return 1;
	}

	delete bonefile;
	delete bfdlg;

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_bonname, 2048, buf );
	}

	return 0;
}

int CMyD3DApplication::LoadMAFile()
{
	if( !m_thandler || !m_shandler ){
		MessageBox( m_hWnd, "形状データが読み込まれていないので、\n入力できません。", "入力できません", MB_OK );
		return 0;
	}

	int ret;

	ret = (int)MessageBox( m_hWnd, "このモードにすると、読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( ret != IDOK ){						
		return 0;//!!!!!!!!!!!
	}else{
		ret = DestroyMotion( 1 );
		if( ret ){
			DbgOut( "viewer : LoadMAFile : DestroyMotion error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	char buf[_MAX_PATH];
	OPENFILENAME ofn;
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"MOA File (*.moa)\0*.moa\0";
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
		return S_OK;
	}


	CMAFile* mafile;
	mafile = new CMAFile();
	if( !mafile ){
		DbgOut( "viewer : LoadMAFile : mafile alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = mafile->LoadMAFile( buf, this );
	if( ret ){
		DbgOut( "viewer : LoadMAFile : mafile LoadMAFile error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}


	//motparamdlg->SetParamsで作成された、空モーション(先頭モーション)を削除する。
	if( (g_motdlg->GetMotionNum() >= 2) && (mafile->m_loadtrunknum >= 1) ){

		ret = g_motdlg->SelectCurrentMotion( 0 );
		if( ret ){
			DbgOut( "viewer : LoadMAFile : SelectCurrentMotion error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}

		ret = DestroyMotion( 0 );
		_ASSERT( !ret );

	}

	delete mafile;
	mafile = 0;

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_moaname, 2048, buf );
		m_loadnamedlg->m_quaname[0] = 0;//!!!!!!!!!!!!!!!!!
	}


	return 0;
}


int CMyD3DApplication::LoadBoneFile( LOADFILEINFO* ldinfo )
{

	if( !m_thandler || !m_shandler ){
		MessageBox( m_hWnd, "形状データが読み込まれていないので、\n入力できません。", "入力できません", MB_OK );
		return 0;
	}

	// 変更する場合は、NewJoint もチェック
	char buf[_MAX_PATH];
	if( ldinfo == 0 ){
		OPENFILENAME ofn;
		buf[0] = 0;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"BONE File (*.bon)\0*.bon\0";
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
			return S_OK;
		}
	}

	char* mqonameptr;
	if( ldinfo == 0 ){
		mqonameptr = buf;
	}else{
		mqonameptr = ldinfo->filename;
	}


	int ret;

//bonefile の読み込みと、Treeの設定
	CBoneFile* bonefile;
	bonefile = new CBoneFile();
	if( !bonefile ){
		DbgOut( "viewer3 : LoadBoneFile : bonefile alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = bonefile->LoadBoneFile( mqonameptr, m_thandler, m_shandler, m_mhandler );
	if( ret ){
		DbgOut( "viewer3 : LoadBoneFile : bonefile LoadBoneFile error !!!\n" );
		_ASSERT( 0 );
		delete bonefile;
		return 1;
	}

	delete bonefile;

//SigTreeView, MotParamDlg のTreeの作り直し
	// NewJoint 関数から一部、コピペ

	ERRORMES errormes;

	m_ExistData = FALSE;

	InvalidateHandler();

	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : LoadBoneFile : DestroyBoneInfo error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadBoneFile : DestroyBoneInfo error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : LoadBoneFile : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : LoadBoneFile : DestroyMotionObj error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadBoneFile : DestroyMotionObj error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return 1;
		}
	}

	//////////////////
	////////////////////
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadBoneFile : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadBoneFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}


	ret = RestoreHandler( 1 );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadBoneFile : RestoreHandler error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
	}

	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : LoadBoneFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	/////////////////////////
	/////////////////////////

	//SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)serino );
	//SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)serino );

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( !ldinfo && m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_bonname, 2048, mqonameptr );
	}


	m_ExistData = TRUE;
	
	return 0;
}


int CMyD3DApplication::SetDefaultRenderState()
{
	ZeroMemory( g_renderstate, sizeof( int ) * (D3DRS_BLENDOP + 1) );


	float fogdensity = 1.0f;
	float pointsize = 1.0f;
	float pointscale = 1.0f;
	float pointscaleb = 0.0f;
	float pointscalec = 0.0f;
	float numsegments = 0.0f;
	float tweenfactor = 1.0f;

	g_renderstate[D3DRS_ZENABLE] = D3DZB_TRUE;
	g_renderstate[D3DRS_FILLMODE] =	D3DFILL_SOLID;
	g_renderstate[D3DRS_SHADEMODE] = D3DSHADE_GOURAUD;
	//g_renderstate[D3DRS_LINEPATTERN] = 0;
	g_renderstate[D3DRS_ZWRITEENABLE] = TRUE;
	g_renderstate[D3DRS_ALPHATESTENABLE] = TRUE;
	g_renderstate[D3DRS_LASTPIXEL] = TRUE;
	//g_renderstate[D3DRS_SRCBLEND] = D3DBLEND_ONE;
	//g_renderstate[D3DRS_DESTBLEND] = D3DBLEND_ZERO ;
	g_renderstate[D3DRS_SRCBLEND] = D3DBLEND_SRCALPHA;
	g_renderstate[D3DRS_DESTBLEND] = D3DBLEND_INVSRCALPHA;
	g_renderstate[D3DRS_CULLMODE] = D3DCULL_CCW;

	g_renderstate[D3DRS_ZFUNC] = D3DCMP_LESSEQUAL;
	g_renderstate[D3DRS_ALPHAREF] = 0x08;
	g_renderstate[D3DRS_ALPHAFUNC] = D3DCMP_GREATEREQUAL;
	g_renderstate[D3DRS_DITHERENABLE] = FALSE;//<------------
	g_renderstate[D3DRS_ALPHABLENDENABLE] = FALSE;
	g_renderstate[D3DRS_FOGENABLE] = FALSE;
	g_renderstate[D3DRS_SPECULARENABLE] = TRUE;//<----------
	//g_renderstate[D3DRS_ZVISIBLE] = 0;
	g_renderstate[D3DRS_FOGCOLOR] = 0;
	g_renderstate[D3DRS_FOGTABLEMODE] = D3DFOG_NONE;

	g_renderstate[D3DRS_FOGSTART] = 0;
	g_renderstate[D3DRS_FOGEND] = 0;
	g_renderstate[D3DRS_FOGDENSITY ] = *((DWORD*)&fogdensity);
	//g_renderstate[D3DRS_EDGEANTIALIAS] = FALSE;
	//g_renderstate[D3DRS_ZBIAS] = 0;
	g_renderstate[D3DRS_RANGEFOGENABLE] = FALSE;
	g_renderstate[D3DRS_STENCILENABLE] = FALSE;
	g_renderstate[D3DRS_STENCILFAIL] = D3DSTENCILOP_KEEP;
	g_renderstate[D3DRS_STENCILZFAIL] = D3DSTENCILOP_KEEP;
	g_renderstate[D3DRS_STENCILPASS] = D3DSTENCILOP_KEEP;

	g_renderstate[D3DRS_STENCILFUNC] = D3DCMP_ALWAYS;
	g_renderstate[D3DRS_STENCILREF] = 0;
	g_renderstate[D3DRS_STENCILMASK] = 0xFFFFFFFF;
	g_renderstate[D3DRS_STENCILWRITEMASK] = 0xFFFFFFFF;
	g_renderstate[D3DRS_TEXTUREFACTOR] = 0xFFFFFFFF;//!!!
	g_renderstate[D3DRS_WRAP0] = 0;
	g_renderstate[D3DRS_WRAP1] = 0;
	g_renderstate[D3DRS_WRAP2] = 0;
	g_renderstate[D3DRS_WRAP3] = 0;
	g_renderstate[D3DRS_WRAP4] = 0;


	g_renderstate[D3DRS_WRAP5] = 0;
	g_renderstate[D3DRS_WRAP6] = 0;
	g_renderstate[D3DRS_WRAP7] = 0;
	g_renderstate[D3DRS_CLIPPING] = TRUE;
	g_renderstate[D3DRS_LIGHTING] = FALSE;
	g_renderstate[D3DRS_AMBIENT] = 0x40404040;
	g_renderstate[D3DRS_FOGVERTEXMODE] = D3DFOG_NONE;
	g_renderstate[D3DRS_COLORVERTEX] = TRUE;
	g_renderstate[D3DRS_LOCALVIEWER] = TRUE;
	g_renderstate[D3DRS_NORMALIZENORMALS] = FALSE;

	g_renderstate[D3DRS_DIFFUSEMATERIALSOURCE] = D3DMCS_COLOR1;
	g_renderstate[D3DRS_SPECULARMATERIALSOURCE] = D3DMCS_COLOR2;
	g_renderstate[D3DRS_AMBIENTMATERIALSOURCE] = D3DMCS_COLOR2;
	g_renderstate[D3DRS_EMISSIVEMATERIALSOURCE] = D3DMCS_MATERIAL;
	g_renderstate[D3DRS_VERTEXBLEND] = D3DVBF_DISABLE;
	g_renderstate[D3DRS_CLIPPLANEENABLE] = 0;
	//g_renderstate[D3DRS_SOFTWAREVERTEXPROCESSING] = FALSE;//!!!
	g_renderstate[D3DRS_POINTSIZE] = *((DWORD*)&pointsize);
	g_renderstate[D3DRS_POINTSIZE_MIN] = *((DWORD*)&pointsize);
	g_renderstate[D3DRS_POINTSPRITEENABLE] = FALSE;


	g_renderstate[D3DRS_POINTSCALEENABLE] = FALSE;
	g_renderstate[D3DRS_POINTSCALE_A] = *((DWORD*)&pointscale);
	g_renderstate[D3DRS_POINTSCALE_B] = *((DWORD*)&pointscaleb);
	g_renderstate[D3DRS_POINTSCALE_C] = *((DWORD*)&pointscalec);
	g_renderstate[D3DRS_MULTISAMPLEANTIALIAS] = FALSE;//!!!
	g_renderstate[D3DRS_MULTISAMPLEMASK] = 0xFFFFFFFF;
	g_renderstate[D3DRS_PATCHEDGESTYLE] = 0;
	//g_renderstate[D3DRS_PATCHSEGMENTS] = *((DWORD*)&numsegments);
	g_renderstate[D3DRS_DEBUGMONITORTOKEN] = D3DDMT_ENABLE;
	g_renderstate[D3DRS_POINTSIZE_MAX] = *((DWORD*)&pointsize);


	g_renderstate[D3DRS_INDEXEDVERTEXBLENDENABLE] = TRUE;//!!!
	g_renderstate[D3DRS_COLORWRITEENABLE] = 0x0000000F;
	g_renderstate[D3DRS_TWEENFACTOR] = *((DWORD*)&tweenfactor);
	g_renderstate[D3DRS_BLENDOP] = D3DBLENDOP_ADD;

////////
	return 0;

}

int CMyD3DApplication::UpdateDialogIniData( int dlgno )
{
	RECT newrect;

	switch( dlgno ){
	case ID_MSG_FROM_CAMERADLG:

		m_cameradlg->GetWindowRect( &newrect );
		g_inidata->camdlg_posx = newrect.left;
		g_inidata->camdlg_posy = newrect.top;
		break;

	case ID_MSG_FROM_SIGDLG:
		m_sigeditwnd->GetWindowRect( &newrect );
		g_inidata->sigdlg_posx = newrect.left;
		g_inidata->sigdlg_posy = newrect.top;
		break;

	case ID_MSG_FROM_MOTDLG:
		g_motdlg->GetWindowRect( &newrect );
		g_inidata->motdlg_posx = newrect.left;
		g_inidata->motdlg_posy = newrect.top;

		break;

	default:
		break;
	}
	return 0;
}

int CMyD3DApplication::DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec )
{
	float mag;
	mag = srcvec->x * srcvec->x + srcvec->y * srcvec->y + srcvec->z * srcvec->z;

	float sqmag;
	sqmag = (float)sqrt( mag );

	if( sqmag != 0.0f ){
		float magdiv;
		magdiv = 1.0f / sqmag;
		dstvec->x *= magdiv;
		dstvec->y *= magdiv;
		dstvec->z *= magdiv;

	}else{
		//DbgOut( "viewer : DXVec3Normalize : zero warning !!!\n" );
		dstvec->x = 0.0f;
		dstvec->y = 0.0f;
		dstvec->z = 0.0f;
		_ASSERT( 0 );
	}

	return 0;
}


int CMyD3DApplication::WriteIM2File()
{

	if( !m_thandler || !m_shandler ){
		MessageBox( m_hWnd, "形状データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
		return 0;
	}

	/***
	int nocnt = 0;
	int ret;
	ret = m_shandler->CheckOrgNoBuf( &nocnt );
	if( ret ){
		DbgOut( "viewer : WriteIM2File : CheckOrgNoBuf error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/

	int ret;


	CWriteIM2Dlg dlg;
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK ){
		return 0;
	}


	//if( (dlg.m_formqo == 0) && (m_shandler->m_im2enableflag == 0) ){
	//	MessageBox( m_hWnd, "出力できません。\n詳しくは、http://www5d.biglobe.ne.jp/~ochikko/rdb2_im2file.htm をご覧ください。", "出力できません", MB_OK );
	//	return 0;
	//}


	CIM2File im2file;
	ret = im2file.WriteIM2File( dlg.m_im2name, dlg.m_formqo, m_thandler, m_shandler );
	if( ret ){
		DbgOut( "viewer : WriteIM2File : bimfile WriteIM2File error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_im2name, 2048, dlg.m_im2name );
	}

	return 0;
}

int CMyD3DApplication::LoadIM2File( int reloadflag )
{
	if( !m_thandler || !m_shandler ){
		MessageBox( m_hWnd, "形状データが読み込まれていないので、\n入力できません。", "入力できません", MB_OK );
		return 0;
	}

	/***
	int nocnt = 0;
	int ret;
	ret = m_shandler->CheckOrgNoBuf( &nocnt );
	if( ret ){
		DbgOut( "viewer : LoadIM2File : CheckOrgNoBuf error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/


	int ret;
	if( m_shandler->m_im2enableflag == 0 ){
		MessageBox( m_hWnd, "入力できません。\n詳しくは、http://www5d.biglobe.ne.jp/~ochikko/rdb2_im2file.htm をご覧ください。", "入力できません", MB_OK );
		return 0;
	}


	if( reloadflag == 0 ){
		OPENFILENAME ofn;
		char buf[_MAX_PATH];
		buf[0] = 0;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"IM2 File (*.im2)\0*.im2\0";
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
		ofn.lpstrDefExt = NULL;
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( GetOpenFileName(&ofn) == 0 ){
			return S_OK;
		}

		CIM2File im2file;


		ret = im2file.LoadIM2File( buf, m_thandler, m_shandler, m_mhandler );
		if( ret ){
			DbgOut( "viewer : LoadIM2File : bimfile LoadIM2File error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( m_loadnamedlg ){
			strcpy_s( m_loadnamedlg->m_im2name, 2048, buf );
		}

	}else{

		CIM2File im2file;

		ret = im2file.LoadIM2File( m_loadnamedlg->m_im2name, m_thandler, m_shandler, m_mhandler );
		if( ret ){
			DbgOut( "viewer : LoadIM2File : bimfile LoadIM2File error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}


	return 0;
}

int CMyD3DApplication::ToggleBoneDisp()
{
//DbgOut( "viewer : ToggleBoneDisp \r\n" );

	if( m_optdlg->m_bonedisp ){
		m_optdlg->m_bonedisp = 0;
		g_inidata->opt_jointdisp = 0;

		m_optdlg->m_bmdisp = 0;
		g_inidata->opt_bmdisp = 0;
	}else{
		m_optdlg->m_bonedisp = 1;
		g_inidata->opt_jointdisp = 1;

		m_optdlg->m_bmdisp = 1;
		g_inidata->opt_bmdisp = 1;
	}
	SetBoneDisp();

	return 0;
}

int CMyD3DApplication::CopyUndoBuffer()
{
	int ret;
	if( g_motdlg ){
		ret = g_motdlg->CopyUndoBuffer();
		_ASSERT( !ret );

		EnableMenuItem( m_appmenu, IDMENU_IKUNDO, MF_BYCOMMAND | MF_ENABLED );
		EnableMenuItem( m_appmenu, IDMENU_REVIKUNDO, MF_BYCOMMAND | MF_ENABLED );

		m_ik_cnt++;
		m_undo_cnt = 0;
		m_undo_init_flag = 1;
	}

	return 0;
}


int CMyD3DApplication::GetScreenPos( D3DXVECTOR3 vec3d, float* scxptr, float* scyptr, float* sczptr, float* wpptr )
{

	D3DXMATRIX matSet;
	matSet = m_matWorld * m_matView * m_matProjX;

	float x, y, z, wp;
	x = vec3d.x;
	y = vec3d.y;
	z = vec3d.z;

	float xp, yp, zp;//, wp;
	xp = matSet._11*x + matSet._21*y + matSet._31*z + matSet._41;
	yp = matSet._12*x + matSet._22*y + matSet._32*z + matSet._42;
	zp = matSet._13*x + matSet._23*y + matSet._33*z + matSet._43;
	wp = matSet._14*x + matSet._24*y + matSet._34*z + matSet._44;


// d3ddisp.h ConvScreenPos()

    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;

	float aspect = (float)m_clientWidth / (float)m_clientHeight;

	float invwp;

	if( wp != 0.0f )
		invwp = 1.0f / wp;
	else
		invwp = 1e6;

	float xp2, yp2;
	xp2 = xp * invwp;
	yp2 = yp * invwp;

	*scxptr  = ( 1.0f + xp2 ) * aspect * (float)dwClipHeight + (float)dwClipWidth - aspect * (float)dwClipHeight;
	*scyptr  = ( 1.0f - yp2 ) * (float)dwClipHeight;
	*sczptr  = zp * invwp;
	//dsttlv->rhw = wp;
	*wpptr = wp;

	return 0;
}

/***
int CMyD3DApplication::GetScreenPos( D3DXVECTOR3 vec3d, int* x2dptr, int* y2dptr, int* validptr )
{

    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;


	CBSphere bs;
	bs.tracenter = vec3d;

	float calaspect;
	if( m_matProj._43 < -1.0f ){
		calaspect = 1.0f;
	}else{
		//正射影
		calaspect = (float)dwClipWidth / (float)dwClipHeight;
	}

	int ret;
	ret = bs.Transform2ScreenPos( dwClipWidth, dwClipHeight, m_matView, m_matProj, calaspect, x2dptr, y2dptr );
	if( ret ){
		DbgOut( "viewer : GetScreenPos : bs Transform2ScreenPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (*x2dptr == 0xFFFFFFFF) && (*y2dptr == 0xFFFFFFFF) ){
		*validptr = 0;
	}else{
		*validptr = 1;
	}

	return 0;
}
***/


int CMyD3DApplication::GetRotateDelta( int rotkind, POINT befpos, POINT aftpos, float* dstdelta )
{

	float diffx, delta;
	diffx = (float)( aftpos.x - befpos.x );

	float step;
	//step = 2.0f * (float)PAI / 1000.0f * 5.0f;
	step = 2.0f * (float)PAI / 1000.0f;

	delta = diffx * step;

	*dstdelta = delta;

//DbgOut( "viewer : GetRotateDelta : befpos %d %d, aftpos %d %d, dot %f, delta %f\r\n",
//	   befpos.x, befpos.y,
//	   aftpos.x, aftpos.y,
//	   dot, delta );

	return 0;
}


/***
int CMyD3DApplication::GetRotateDelta( int rotkind, POINT befpos, POINT aftpos, float* dstdelta )
{
	//int ret;
	D3DXVECTOR3 xvec( 100.0f, 0.0f, 0.0f );
	D3DXVECTOR3 yvec( 0.0f, 100.0f, 0.0f );
	D3DXVECTOR3 zvec( 0.0f, 0.0f, 100.0f );
	D3DXVECTOR3 mvpos;

	D3DXVECTOR2 axisvec;
	D3DXVECTOR2 diffvec;


	float dot;
	float delta;

	int validflag;
	int selscx, selscy;
	GetScreenPos( m_selectedpos, &selscx, &selscy, &validflag );
	if( validflag == 0 ){
		*dstdelta = 0.0f;
		return 0;
	}
	int mvscx, mvscy;

	switch( rotkind ){
	case SELECT_X:
		mvpos = m_selectedpos + xvec;
		break;
	case SELECT_Y:
		mvpos = m_selectedpos + yvec;
		break;
	case SELECT_Z:
		mvpos = m_selectedpos + zvec;
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	GetScreenPos( mvpos, &mvscx, &mvscy, &validflag );
	if( validflag == 0 ){
		*dstdelta = 0.0f;
		return 0;
	}
		
	axisvec.x = (float)(mvscx - selscx);
	axisvec.y = (float)(mvscy - selscy);
	D3DXVec2Normalize( &axisvec, &axisvec );//!!!!!!!!!!

	diffvec.x = (float)(aftpos.x - befpos.x);
	diffvec.y = (float)(aftpos.y - befpos.y);

	dot = D3DXVec2Dot( &axisvec, &diffvec );

	float step;
	step = 2.0f * (float)PAI / 1000.0f * 5.0f;

	delta = dot * step;

	*dstdelta = delta;

//DbgOut( "viewer : GetRotateDelta : befpos %d %d, aftpos %d %d, dot %f, delta %f\r\n",
//	   befpos.x, befpos.y,
//	   aftpos.x, aftpos.y,
//	   dot, delta );

	return 0;
}
***/

int CMyD3DApplication::GetShiftDelta( int shiftkind, POINT befpos, POINT aftpos, float* dstdelta )
{

	/***
	D3DXVECTOR3 xvec( 100.0f, 0.0f, 0.0f );
	D3DXVECTOR3 yvec( 0.0f, 100.0f, 0.0f );
	D3DXVECTOR3 zvec( 0.0f, 0.0f, 100.0f );
	D3DXVECTOR3 mvpos;

	D3DXVECTOR2 axisvec;
	D3DXVECTOR2 diffvec;


	float dot;
	float delta;

	float selscx, selscy, selscz, selwp;
	GetScreenPos( m_selectedpos, &selscx, &selscy, &selscz, &selwp );

	float mvscx, mvscy, mvscz, mvwp;

	switch( shiftkind ){
	case SELECT_X:
		mvpos = m_selectedpos + xvec;
		break;
	case SELECT_Y:
		mvpos = m_selectedpos + yvec;
		break;
	case SELECT_Z:
		mvpos = m_selectedpos + zvec;
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	GetScreenPos( mvpos, &mvscx, &mvscy, &mvscz, &mvwp );
		
	axisvec.x = mvscx - selscx;
	axisvec.y = mvscy - selscy;
	D3DXVec2Normalize( &axisvec, &axisvec );//!!!!!!!

	diffvec.x = (float)(aftpos.x - befpos.x);
	diffvec.y = (float)(aftpos.y - befpos.y);

	dot = D3DXVec2Dot( &axisvec, &diffvec );

	float step;
	//step = 800.0f / 1000.0f;
	step = 2.5f;


	delta = dot * step;

	*dstdelta = delta;
	***/

	float diffx, delta;
	diffx = (float)( aftpos.x - befpos.x );

	float step;
	step = 2.5f;

	delta = diffx * step;

	*dstdelta = delta;

	return 0;
}

int CMyD3DApplication::GetScaleDelta( int shiftkind, POINT befpos, POINT aftpos, float* dstdelta )
{

	D3DXVECTOR3 xvec( 100.0f, 0.0f, 0.0f );
	D3DXVECTOR3 yvec( 0.0f, 100.0f, 0.0f );
	D3DXVECTOR3 zvec( 0.0f, 0.0f, 100.0f );
	D3DXVECTOR3 mvpos;

	D3DXVECTOR2 axisvec;
	D3DXVECTOR2 diffvec;


	float dot;
	float delta;

	float selscx, selscy, selscz, selwp;
	GetScreenPos( m_selectedpos, &selscx, &selscy, &selscz, &selwp );

	float mvscx, mvscy, mvscz, mvwp;

	switch( shiftkind ){
	case SELECT_X:
		mvpos = m_selectedpos + xvec;
		break;
	case SELECT_Y:
		mvpos = m_selectedpos + yvec;
		break;
	case SELECT_Z:
		mvpos = m_selectedpos + zvec;
		break;
	default:
		_ASSERT( 0 );
		break;
	}

	GetScreenPos( mvpos, &mvscx, &mvscy, &mvscz, &mvwp );
		
	axisvec.x = mvscx - selscx;
	axisvec.y = mvscy - selscy;
	D3DXVec2Normalize( &axisvec, &axisvec );//!!!!!!!

	diffvec.x = (float)(aftpos.x - befpos.x);
	diffvec.y = (float)(aftpos.y - befpos.y);

	dot = D3DXVec2Dot( &axisvec, &diffvec );

	float step;
	//step = 800.0f / 1000.0f;
	//step = 2.5f;

	step = 0.05f;//!!!!!!!!!!!!!!!!!!


	delta = dot * step;

	*dstdelta = delta;

	return 0;
}




int CMyD3DApplication::CheckTBB()
{
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_R, MAKELONG(m_tbb_r, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_T, MAKELONG(m_tbb_t, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_T2, MAKELONG(m_tbb_t2, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_S, MAKELONG(m_tbb_s, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ITI, MAKELONG(m_tbb_iti, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_IKT, MAKELONG(m_tbb_ikt, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_COL, MAKELONG(m_tbb_col, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_BONE, MAKELONG(m_tbb_bone, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_EI1, MAKELONG(m_tbb_ei1, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_EI2, MAKELONG(m_tbb_ei2, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_EI3, MAKELONG(m_tbb_ei3, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_TOON1, MAKELONG(m_tbb_toon1, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_MO, MAKELONG(m_tbb_mo, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_FACE, MAKELONG(m_tbb_face, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_LINE, MAKELONG(m_tbb_line, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_POINT, MAKELONG(m_tbb_point, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_TEN2, MAKELONG(m_tbb_ten2, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ZA1, MAKELONG(m_tbb_za1, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ZA2, MAKELONG(m_tbb_za2, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ZA3, MAKELONG(m_tbb_za3, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_COLP, MAKELONG(m_tbb_colP, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_COLT, MAKELONG(m_tbb_colT, 0));

//	SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_G, MAKELONG(m_tbb_g, 0));
//	SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_L, MAKELONG(m_tbb_l, 0));
	
	return 0;
}

int CMyD3DApplication::DestroyDispAndMot()
{
	int ret;
	if( !m_thandler && !m_shandler && !m_mhandler ){
		return 0;
	}

	ret = (int)MessageBox( m_hWnd, "形状データとモーションデータを削除します。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( ret != IDOK ){
		return 0;
	}

	m_ExistData = FALSE;

	InvalidateHandler();
	
	if( m_thandler || m_shandler || m_mhandler ){
		DestroyHandler();
	}

	ret = CreateHandler();
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadSigFile : CreateHandler error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
////////
	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();


	::SendMessage( m_hWnd, WM_USER_ANIMMENU, 0, 0 );

/***
	if( g_motdlg ){
		//DbgOut( "CMyD3DApplication : DestroyDispAndMot : motdlg->SetParams\n" );

		ret = g_motdlg->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
			(long)m_hWnd, this );
	}

	if( m_sigeditwnd ){
		//DbgOut( "CMyD3DApplication : DestroyDispAndMot : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
		//	m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
			(long)m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : DestroyDispAndMot : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
***/

	return 0;
}

int CMyD3DApplication::SetInvPose2OrgData()
{
	int ret;


	if( !m_thandler || !m_shandler || !m_mhandler ){
		MessageBox( m_hWnd, "形状または、モーションデータが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}

	
	ret = (int)MessageBox( m_hWnd, "実行すると、現在読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( ret != IDOK ){
		
		return 0;
	}

///////////

	DestroyMotion( 1 );
	/***
	if( g_motdlg )
		g_motdlg->InvalidateHandler();

	if( m_mhandler ){
		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "viewer : SetInvPose2OrgData : mh DestroyMotionObj error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	if( g_motdlg && m_mhandler ){
		ret = g_motdlg->SetParams( (long)m_thandler, (long)m_shandler, (long)m_mhandler,
			(long)m_hWnd, this );

		g_motdlg->OnStop();
	}
	***/

///////////


	CQuaternion y180q;
	y180q.SetRotation( 0.0f, 180.0f, 0.0f );

	ret = m_shandler->SetCurrentPose2OrgData( m_mhandler, &y180q );
	if( ret ){
		DbgOut( "viewer : SetInvPose2OrgData : sh SetCurrentPose2OrgData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

///////////

	ret = m_shandler->InvalidateDispObj();
	if( ret ){
		DbgOut( "viewer : SetInvPose2OrgData : sh InvalidateDispObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_shandler->DestroyBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : SetInvPose2OrgData : sh DestroyBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//////////

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : SetInvPose2OrgData : sh CreateBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_shandler->SetMikoAnchorApply( m_thandler );
	if( ret ){
		DbgOut( "viewer : SetInvPose2OrgData : sh SetMikoAnchorApply error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_shandler->CreateDispObj( m_pd3dDevice, m_mhandler, 0 );
	if( ret ){
		DbgOut( "viewer : SetInvPose2OrgData : sh CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


////////
	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。

	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	if( m_sigeditwnd ){
		//DbgOut( "CMyD3DApplication : DestroyDispAndMot : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
		//	m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : SetInvPose2OrgData : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;

}

int CMyD3DApplication::SetMikoBlendType()
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler ){
		MessageBox( m_hWnd, "形状または、モーションデータが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}

	if( m_shandler->m_bonetype != BONETYPE_MIKO ){
		MessageBox( m_hWnd, "mikotoデータを読み込んでいないので、この操作は出来ません。", "操作できません", MB_OK );
		return 0;
	}

	CMikoBlendDlg dlg;
	dlg.m_mikoblendtype = m_shandler->m_mikoblendtype;

	ret = (int)dlg.DoModal();
	if( ret == IDOK ){
		m_shandler->m_mikoblendtype = dlg.m_mikoblendtype;

		ret = m_shandler->CalcInfElem( -1, 1 );
		if( ret ){
			DbgOut( "viewer : SetMikoBlendType : sh CalcInfElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}

	return 0;
}


int CMyD3DApplication::SetCurrentPose2OrgData()
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler ){
		MessageBox( m_hWnd, "形状または、モーションデータが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}

	ret = (int)MessageBox( m_hWnd, "実行すると、現在読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( ret != IDOK ){
		return 0;
	}



	ret = m_shandler->SetCurrentPose2OrgData( m_mhandler, 0 );
	if( ret ){
		DbgOut( "viewer : SetCurrentPose2OrgData : sh SetCurrentPose2OrgData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//////////
	ret = m_shandler->InvalidateDispObj();
	if( ret ){
		DbgOut( "viewer : SetCurrentPose2OrgData : sh InvalidateDispObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_shandler->DestroyBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : SetCurrentPose2OrgData : sh DestroyBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

///////////
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : SetCurrentPose2OrgData : sh CreateBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_shandler->SetMikoAnchorApply( m_thandler );
	if( ret ){
		DbgOut( "viewer : SetCurrentPose2OrgData : sh SetMikoAnchorApply error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}


	ret = m_shandler->CreateDispObj( m_pd3dDevice, m_mhandler, 0 );
	if( ret ){
		DbgOut( "viewer : SetCurrentPose2OrgData : sh CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
/////////

	if( m_mcdlg->m_firstmake ){
		ret = m_mcdlg->DestroyMCElem( -1 );
		if( ret ){
			DbgOut( "viewer : SetCurrentPose2OrgData : mcdlg DestroyMCElem all error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	if( g_motdlg )
		g_motdlg->InvalidateHandler();

	if( m_mhandler ){
		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "viewer : SetCurrentPose2OrgData : mh DestroyMotionObj error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

////////
	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	//if( g_motdlg )
	//	g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	if( g_motdlg ){
		//DbgOut( "CMyD3DApplication : DestroyDispAndMot : motdlg->SetParams\n" );

		ret = g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : SetCurrentPose2OrgData : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		//DbgOut( "CMyD3DApplication : DestroyDispAndMot : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
		//	m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : SetCurrentPose2OrgData : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CMyD3DApplication::ChangeIKTPos( int iktno, POINT srcpos )
{
	int ret;

	if( !g_motdlg || !m_shandler ){
		return 0;
	}


	CShdElem* shiftikt;
	shiftikt = (*m_shandler)( iktno );
	if( !shiftikt )
		return 0;


	float shiftscx, shiftscy, shiftscz, shiftwp;	
	D3DXVECTOR3 target, targetsc;


	D3DXVECTOR3 iktpos;
	D3DXMATRIX inimat;
	D3DXMatrixIdentity( &inimat );

	switch( m_select_kind ){
	case SELECT_CENTER:


		ret = GetScreenPos( shiftikt->m_iktargetpos, &shiftscx, &shiftscy, &shiftscz, &shiftwp );
		if( ret != 0 )
			return 0;

		targetsc.x = (float)srcpos.x;
		targetsc.y = (float)srcpos.y;
		targetsc.z = shiftscz;//!!!!!

		ret = ScreenToObj( &target, targetsc );
		if( ret )
			return 0;	

		break;
	case SELECT_X:
	case SELECT_Y:
	case SELECT_Z:

		iktpos = shiftikt->m_iktargetpos;

		GetShiftDelta( m_select_kind, m_lpos, srcpos, &m_select_delta );

		if( m_select_kind == SELECT_X ){

			target.x = iktpos.x + m_select_delta;
			target.y = iktpos.y;
			target.z = iktpos.z;

		}else if( m_select_kind == SELECT_Y ){
			target.x = iktpos.x;
			target.y = iktpos.y + m_select_delta;
			target.z = iktpos.z;
		}else if( m_select_kind == SELECT_Z ){
			target.x = iktpos.x;
			target.y = iktpos.y;
			target.z = iktpos.z + m_select_delta;
		}

		break;
	default:
		_ASSERT( 0 );
		break;
	}

	shiftikt->m_iktargetpos = target;

//DbgOut( "viewer : ChangeIKTPos : %f %f %f\r\n", target.x, target.y, target.z );

	return 0;

}

int CMyD3DApplication::ValidSelectCircle( int srcflag )
{
	int ret;

	int xzno, xyno, yzno;

	ret = m_select_th->GetPartNoByName( "EXTLINE_CIRCLE_XZ", &xzno );
	if( ret || (xzno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_th->GetPartNoByName( "EXTLINE_CIRCLE_XY", &xyno );
	if( ret || (xyno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_th->GetPartNoByName( "EXTLINE_CIRCLE_YZ", &yzno );
	if( ret || (yzno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
///////
	ret = m_select_sh->SetValidFlag( xzno, srcflag );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetValidFlag( xyno, srcflag );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetValidFlag( yzno, srcflag );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

//	DbgOut( "ValidSelectCircle : %d, %d %d %d,  %d %d,   %d %d %d\r\n", 
//		srcflag, 
//		xzno, xyno, yzno, 
//		SHDEXTLINE, (*m_select_sh)( xzno )->type,
//		(*m_select_sh)( xzno )->notuse, (*m_select_sh)( xyno )->notuse, (*m_select_sh)( yzno )->notuse );

	return 0;
}

int CMyD3DApplication::SetSelectColor( int rgbflag )
{
	int ret;
	int sprno, spbno, spgno;
	int crno, cbno, cgno;

	ret = m_select_th->GetPartNoByName( "obj_X", &sprno );
	if( ret || (sprno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_th->GetPartNoByName( "obj_Y", &spgno );
	if( ret || (spgno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_th->GetPartNoByName( "obj_Z", &spbno );
	if( ret || (spbno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
////////	

	ret = m_select_th->GetPartNoByName( "EXTLINE_CIRCLE_XZ", &crno );
	if( ret || (crno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_th->GetPartNoByName( "EXTLINE_CIRCLE_XY", &cgno );
	if( ret || (cgno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_th->GetPartNoByName( "EXTLINE_CIRCLE_YZ", &cbno );
	if( ret || (cbno < 0) ){
		_ASSERT( 0 );
		return 1;
	}
/////////

	COLORREF rcol, gcol, bcol;
	switch( rgbflag ){
	case 0:
		//選択無し
		rcol = RGB( 150, 0, 0 );
		gcol = RGB( 0, 150, 0 );
		bcol = RGB( 0, 0, 150 );
		break;
	case 1:
		//赤選択
		rcol = RGB( 255, 0, 0 );
		gcol = RGB( 0, 150, 0 );
		bcol = RGB( 0, 0, 150 );
		break;
	case 2:
		//緑選択
		rcol = RGB( 150, 0, 0 );
		gcol = RGB( 0, 255, 0 );
		bcol = RGB( 0, 0, 150 );
		break;
	case 3:
		//青選択
		rcol = RGB( 150, 0, 0 );
		gcol = RGB( 0, 150, 0 );
		bcol = RGB( 0, 0, 255 );
		break;
	default:
		rcol = RGB( 150, 0, 0 );
		gcol = RGB( 0, 150, 0 );
		bcol = RGB( 0, 0, 150 );
		break;
	}

/////////

	ret = m_select_sh->SetExtLineColor( crno, 255, GetRValue( rcol ), GetGValue( rcol ), GetBValue( rcol ) );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetExtLineColor error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetExtLineColor( cgno, 255, GetRValue( gcol ), GetGValue( gcol ), GetBValue( gcol ) );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetExtLineColor error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetExtLineColor( cbno, 255, GetRValue( bcol ), GetGValue( bcol ), GetBValue( bcol ) );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetExtLineColor error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

///////////

	ret = m_select_sh->SetDispDiffuse( sprno, GetRValue( rcol ), GetGValue( rcol ), GetBValue( rcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetDispDiffuse( spgno, GetRValue( gcol ), GetGValue( gcol ), GetBValue( gcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetDispDiffuse( spbno, GetRValue( bcol ), GetGValue( bcol ), GetBValue( bcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
/////////////
	/***
	m_select_sh->SetElemRenderState( sprno, D3DRS_LIGHTING, 0 );
	m_select_sh->SetElemRenderState( spgno, D3DRS_LIGHTING, 0 );
	m_select_sh->SetElemRenderState( spbno, D3DRS_LIGHTING, 0 );
	***/

	return 0;
}

int CMyD3DApplication::IKTON()
{
	int ret;
	int selno;
	D3DXVECTOR3 selpos;
	ret = g_motdlg->GetSelectedJointPos( &selno, &selpos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	if( selno > 0 ){
		CShdElem* selem;
		selem = (*m_shandler)( selno );
		_ASSERT( selem );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			selem->m_iktarget = 1;
			selem->m_iktargetpos = selpos;
		}
	}

	return 0;
}

int CMyD3DApplication::IKTOFF()
{
	int ret;
	int selno;
	D3DXVECTOR3 selpos;
	ret = g_motdlg->GetSelectedJointPos( &selno, &selpos );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	if( selno > 0 ){
		CShdElem* selem;
		selem = (*m_shandler)( selno );
		_ASSERT( selem );

		if( selem->IsJoint() && (selem->type != SHDMORPH) ){
			selem->m_iktarget = 0;
		}
	}


	return 0;
}

int CMyD3DApplication::DisableIKT()
{
	if( !m_shandler )
		return 0;

	int seri;
	CShdElem* selem;

	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		selem = (*m_shandler)( seri );

		if( selem->m_iktarget != 0 )
			selem->m_iktarget = 0;
	}

	return 0;
}

int CMyD3DApplication::InitPluginMenu()
{

	m_plugin = new CPluginElem[ MAXPLUGIN ];
	if( !m_plugin ){
		DbgOut( "viewer : InitPluginMenu : plugin alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


///////////
	char plugindir[2048];
	int leng;
	ZeroMemory( plugindir, 2048 );
	leng = GetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPTSTR)plugindir, 2048 );
	_ASSERT( leng );

	char reldir[] = "\\Release\\";
	char dbgdir[] = "\\Debug\\";

	char* findrel = 0;
	findrel = strstr( plugindir, reldir );
	char* finddbg = 0;
	finddbg = strstr( plugindir, dbgdir );

	if( findrel ){
		*(findrel + 1) = 0;
	}else if( finddbg ){
		*(finddbg + 1) = 0;
	}
	strcat_s( plugindir, 2048, "Plugins\\" );


	char finddir[_MAX_PATH];
	finddir[0] = 0;
	strcpy_s( finddir, _MAX_PATH, plugindir );
	strcat_s( finddir, _MAX_PATH, "*.dll" );

///////////
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile( finddir, &FindFileData );
	int pluginno = 0;

	if( hFind != INVALID_HANDLE_VALUE ){
		char pluginpath[_MAX_PATH];
		strcpy_s( pluginpath, _MAX_PATH, plugindir );
		strcat_s( pluginpath, _MAX_PATH, FindFileData.cFileName );
		DbgOut( "InitPluginMenu : FindFirst : %s\r\n", FindFileData.cFileName );

		(m_plugin + pluginno)->SetFilePath( pluginpath );
		(m_plugin + pluginno)->LoadPlugin();
		pluginno++;

		BOOL bret = 1;
		while( bret != 0 ){
			bret = FindNextFile( hFind, &FindFileData );
			if( bret != 0 ){

				strcpy_s( pluginpath, _MAX_PATH, plugindir );
				strcat_s( pluginpath, _MAX_PATH, FindFileData.cFileName );
				DbgOut( "InitPluginMenu : FindNext : %s\r\n", FindFileData.cFileName );

				(m_plugin + pluginno)->SetFilePath( pluginpath );
				(m_plugin + pluginno)->LoadPlugin();

				pluginno++;
			}
		}

		FindClose( hFind );
	}


	m_pluginmenu = GetSubMenu(m_appmenu, 7);
	if( !m_pluginmenu ){
		DbgOut( "viewer : InitPluginMenu : GetSubMenu error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int menunum;
    menunum = GetMenuItemCount(m_pluginmenu);
	int menuno;
    for (menuno = 0; menuno < menunum; menuno++)
    {
        RemoveMenu(m_pluginmenu, 0, MF_BYPOSITION);
    }


	int setno = 0;
	for( pluginno = 0; pluginno < MAXPLUGIN; pluginno++ ){
		if( (m_plugin + pluginno)->validflag == 1 ){
			int menuid = ID_MENU_PLUGIN + setno;
			AppendMenu( m_pluginmenu, MF_STRING, menuid, (m_plugin + pluginno)->pluginname );

			(m_plugin + pluginno)->menuid = menuid;

			setno++;
		}

	}

	return 0;
}

int CMyD3DApplication::OnCameraRot( POINT ptCursor, POINT befpos )
{

	float diffx;
	float diffy;

//	diffx = (float)ptCursor.x - (float)m_lpos.x;
//	diffy = (float)ptCursor.y - (float)m_lpos.y;

	diffx = (float)ptCursor.x - (float)befpos.x;
	diffy = (float)ptCursor.y - (float)befpos.y;


	RECT clirect;
	float rotxz, roty;
	GetClientRect( m_hWnd, &clirect );
	rotxz = 360.0f * diffx / ((float)(clirect.right - clirect.left) / 2.0f);//半分の幅で一回転
	roty = 180.0f * diffy / ((float)(clirect.bottom - clirect.top) / 2.0f);//半分の高さで１８０度回転


//DbgOut( "rotxz %f, roty %f\r\n", rotxz, roty );

	if( rotxz >= 0.0f ){
		ChangeViewParam( ID_KEY_LEFT, rotxz );	
	}else{
		ChangeViewParam( ID_KEY_RIGHT, -rotxz );
	}
	if( roty >= 0.0f ){
		ChangeViewParam( ID_KEY_RAD2_UP, roty );
	}else{
		ChangeViewParam( ID_KEY_RAD2_DOWN, -roty );
	}

	return 0;
}

int CMyD3DApplication::OnCameraMove( POINT ptCursor, POINT befpos )
{

	float diffx;
	float diffy;

//	diffx = (float)ptCursor.x - (float)m_rpos.x;
//	diffy = (float)ptCursor.y - (float)m_rpos.y;

	diffx = (float)ptCursor.x - (float)befpos.x;
	diffy = (float)ptCursor.y - (float)befpos.y;


	RECT clirect;
	float heri, vert;
	GetClientRect( m_hWnd, &clirect );
	heri = m_camdist * diffx / (float)(clirect.right - clirect.left);
	vert = m_camdist * diffy / (float)(clirect.bottom - clirect.top);


//DbgOut( "heri %f, vert %f\r\n", heri, vert );
	if( heri >= 0.0f ){
		ChangeViewParam( ID_KEY_TARGET_RIGHT, heri );	
	}else{
		ChangeViewParam( ID_KEY_TARGET_LEFT, -heri );
	}
				
	if( vert >= 0.0f ){
		ChangeViewParam( ID_KEY_TARGET_UP, vert );
	}else{
		ChangeViewParam( ID_KEY_TARGET_DOWN, -vert );
	}

	return 0;
}

int CMyD3DApplication::OnCameraDist( float vert )
{

//DbgOut( "heri %f, vert %f\r\n", heri, vert );
				
	if( vert >= 0.0f ){
		ChangeViewParam( ID_KEY_ZOOMOUT, vert );
	}else{
		ChangeViewParam( ID_KEY_ZOOMIN, -vert );
	}
				

	//正射影のビューポートの大きさの変更
	//if( m_parsdlg->parsmode == PARSMODE_ORTHO ){
		//int diffsize;
		//diffsize = (int)( 2000.0f * diffy / (float)(clirect.bottom - clirect.top) );
					
		m_parsdlg->orthosize += (int)vert;
		if( m_parsdlg->orthosize < 500 ){
			m_parsdlg->orthosize = 500;
		}
	//}

	return 0;
}

int CMyD3DApplication::CreateBone( POINT pt )
{
	int isjoint;
	CShdElem* parelem;
	parelem = m_bonedlg->GetSelectedJoint( &isjoint );

    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;
//int CShdHandler::GetScreenPos3( CMotHandler* lpmh, int srcseri, int vertno, int calcmode, int* scxptr, int* scyptr, float* sczptr, 
//							   DWORD dwClipWidth, DWORD dwClipHeight, D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj )

	D3DXVECTOR3 newpos;
	int ret;
	if( parelem && isjoint ){

		/***
		float shiftscx, shiftscy, shiftscz, wp;	
		ret = parelem->GetBoneScreenPos( &shiftscx, &shiftscy, &shiftscz, &wp );
		if( ret != 0 )
			return 0;
		***/
		int shiftscx, shiftscy;
		float shiftscz;
		ret = m_shandler->GetScreenPos3( m_mhandler, parelem->serialno, 0, 1, &shiftscx, &shiftscy, &shiftscz, dwClipWidth, dwClipHeight,
			m_matWorld, m_matView, m_matProjX );
		if( ret != 0 )
			return 0;//!!!!!!!

		D3DXVECTOR3 targetsc;
		targetsc.x = (float)pt.x;
		targetsc.y = (float)pt.y;
		targetsc.z = shiftscz;//!!!!!

		ret = ScreenToObj( &newpos, targetsc );
		if( ret )
			return 0;	
	}else{

		D3DXVECTOR3 zerovec( 0.0f, 0.0f, 0.0f );
		float scx, scy, scz, wp;
		ret = GetScreenPos( zerovec, &scx, &scy, &scz, &wp );
		if( ret )
			return 0;	

		D3DXVECTOR3 targetsc;
		targetsc.x = (float)pt.x;
		targetsc.y = (float)pt.y;
		targetsc.z = scz;//!!!!!

		ret = ScreenToObj( &newpos, targetsc );
		if( ret )
			return 0;	
	}

////////////
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}
	//////////////////
	m_ExistData = FALSE;

	InvalidateHandler();

	if( m_thandler || m_shandler || m_mhandler ){
		ret = m_shandler->DestroyBoneInfo( m_mhandler );
		if( ret ){
			DbgOut( "CMyD3DApplication : CreateBone : DestroyBoneInfo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : CreateBone : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "CMyD3DApplication : CreateBone : DestroyMotionObj error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	//////////////////
	int serino = m_shandler->s2shd_leng;
	int srctype = SHDBALLJOINT;

	int depth;	
	char newname[256];
	m_thandler->Start( serino - 1 );
	m_thandler->Begin();
	depth = m_thandler->GetCurDepth();
	ZeroMemory( newname, 256 );
	sprintf_s( newname, 256, "NewJoint%d", serino );

	ret = Add2Tree( newname, serino, SHDBALLJOINT, depth, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	CVec3f temploc;
	temploc.x = newpos.x;
	temploc.y = newpos.y;
	temploc.z = newpos.z;
	ret = ((*m_shandler)( serino ))->SetJointLoc( &temploc );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	m_thandler->End();

	///////////////////
	/***
	ret = m_shandler->SetChain( serino );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->SetChain( serino );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	***/

	if( parelem ){
		CShdElem* curelem;
		curelem = (*m_shandler)( serino );
		_ASSERT( curelem );
		ret = curelem->PutToChain( parelem, 1 );
		if( ret ){
			DbgOut( "viewer : CreateBone : PutToChain error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		CShdElem* curelem;
		curelem = (*m_shandler)( serino );
		_ASSERT( curelem );

		CShdElem* topelem;
		topelem = (*m_shandler)( 1 );
		_ASSERT( topelem );
		ret = curelem->PutToChain( topelem, 1 );
		if( ret ){
			DbgOut( "viewer : CreateBone : PutToChain error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	ret = m_shandler->CalcDepth();
	if( ret ){
		DbgOut( "viewer : CreateBone : shandler CalcDepth error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	ret = m_mhandler->CopyChainFromShd( m_shandler );
	if( ret ){
		DbgOut( "viewer : CreateBone : mhandler CopyChainFromShd error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_mhandler->CalcDepth();
	if( ret ){
		DbgOut( "viewer : CreateBone : mhandler CalcDepth error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	////////////////////
	////////////////////
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : CreateBone : CreateBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = RestoreHandler( 1 );
	if( ret ){
		DbgOut( "viewer : CreateBone : RestoreHandler error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : CreateBone : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	/////////////////////////
	/////////////////////////
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_BONEDLG, (LPARAM)serino );

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );


	m_bonedlg->SetCurrentName( newname );



	m_ExistData = TRUE;


	return 0;
}

int CMyD3DApplication::TempPickVert( int* shiftpart, int* shiftvert, int* shiftvertnum )
{
	if( !m_shandler || (m_ExistData != TRUE) ){
		*shiftpart = -1;
		*shiftvertnum = 0;
		return 0;
	}


	POINT ptCursor;
	GetCursorPos( &ptCursor );
	ScreenToClient( m_hWnd, &ptCursor );
	
	int kind;
	POINT clickoffset, clickbone;

	PickVert( (WORD)ptCursor.x, (WORD)ptCursor.y, &kind, &clickoffset, &clickbone, shiftpart, shiftvert, shiftvertnum, 0 );

	return 0;
}

int CMyD3DApplication::SaveXFile()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;

	if( !m_ExistData )
		return 0;


	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

/////////////
	CWriteXFileDlg dlg;
	int dlgret;
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK ){
		return 0;
	}

////////////

	ERRORMES errormes;

	CXFile* xfile;
	xfile = new CXFile();
	if( !xfile ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "SaveXFile : xfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	int ret;
	ret = xfile->WriteXFile( dlg.xfilename, dlg.mult, dlg.convface, m_thandler, m_shandler, m_mhandler );
	if( ret ){
		delete xfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "WriteXFile error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	delete xfile;


	return 0;
}
int CMyD3DApplication::DestroyMotion( int allflag )
{
	int ret;
	if( !m_shandler || !m_mhandler || !g_motdlg ){
		return 0;
	}

	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	int motnum;
	motnum = g_motdlg->GetMotionNum();

	if( (allflag == 1) || (motnum <= 1) ){

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( -1 );
			if( ret ){
				DbgOut( "viewer : DestroyMotion : mcdlg DestroyMCElem all error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		if( g_motdlg )
			g_motdlg->InvalidateHandler();

		ret = m_mhandler->DestroyMotionObj();
		if( ret ){
			DbgOut( "viewer : DestroyMotion all : mh DestroyMotionObj error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( g_motdlg ){
			ret = g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
				m_hWnd, this );

		}

		if( g_motdlg ){
			ret = g_motdlg->OnStop();
			_ASSERT( !ret );
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : DestroyMotion all : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}else{

		int delcookie;
		delcookie = g_motdlg->GetMotCookie();

		ret = g_motdlg->DestroyCurrentMotion();
		if( ret ){
			DbgOut( "viewer : DestroyMotion : selecct : motdlg DestroyCurrentMotion error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->DestroyMCElem( delcookie );
			if( ret ){
				DbgOut( "viewer : DestroyMotion : mcdlg DestroyMCElem error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}
int CMyD3DApplication::NewMotion()
{
	if( !m_shandler || !m_mhandler || !g_motdlg ){
		return 0;
	}

	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	int ret;
	ret = g_motdlg->NewMotion();
	if( ret ){
		DbgOut( "viewer : NewMotion : motdlg NewMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_mcdlg->m_firstmake ){
		ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), 0, 0, 0, 0, 0 );
		if( ret ){
			DbgOut( "viewer : newmotion : mcdlg AddParentMC error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CMyD3DApplication::LoadMotionMulti()
{

	int ret;

	if( g_motdlg && m_mhandler ){

		OPENFILENAME ofn;
		char buf[ 2560 ];

		buf[0] = 0;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"QUA File (*.qua)\0*.qua\0";
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = (LPTSTR)buf;
		ofn.nMaxFile =sizeof(buf);
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = NULL;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
		ofn.nFileOffset = 0;
		ofn.nFileExtension = 0;
		ofn.lpstrDefExt =NULL;
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( GetOpenFileName(&ofn) == 0 ){
			return 0;
		}

		CLdQuaDlg dlg;
		ret = (int)dlg.DoModal( m_hWnd );
		if( ret != IDOK )
			return 0;

		//DbgOut( "!!! check !!! : LoadMotionMulti\r\n" );
		int leng;
		int namecnt = 0;
		leng = (int)strlen( buf );
		char* topchar = buf + leng + 1;
		if( *topchar == 0 ){
			//DbgOut( "%d--%s\r\n", namecnt, buf );
			ret = LoadMotion( buf, dlg.mult, 0, 0, 0, 0, 0 );
			if( ret ){
				DbgOut( "viewer : LoadMotionMulti : LoadMotion error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else{
			int leng2;
			char fname[512];
			while( *topchar != 0 ){
				//DbgOut( "%d--%s\\%s\r\n", namecnt, buf, topchar );
				sprintf_s( fname, 512, "%s\\%s", buf, topchar );
				ret = LoadMotion( fname, dlg.mult, 0, 0, 0, 0, 0 );
				if( ret ){
					DbgOut( "viewer : LoadMotionMulti : LoadMotion error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

				leng2 = (int)strlen( topchar );
				topchar = topchar + leng2 + 1;
				namecnt++;
			}
		}

		ret = g_motdlg->GetSelectedJointPos( &m_selectedno, &m_selectedpos );
		_ASSERT( !ret );
		ret = g_motdlg->OnUserSelchange( m_selectedno );//TexKeyDlgのリスト更新
		_ASSERT( !ret );
	}

	return 0;
}




int CMyD3DApplication::LoadMotion( char* filename, float quamult, int idling, int ev0idle, int commonid, int forbidnum, int* forbidid )
{
	int ret;

	if( g_motdlg && m_mhandler ){

		if( m_tbb_iti ){
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
		}

		if( g_motdlg->GetMotionNum() >= MAXMOTIONNUM ){
			char messtr[1024];
			sprintf_s( messtr, 1024, "読み込めるモーションは、%d個までです。\nこれ以上、読み込めません。", MAXMOTIONNUM );
			::MessageBox( m_hWnd, messtr, "エラー", MB_OK );
			return 0;
		}

		//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

		ret = g_motdlg->LoadMotion( m_loadnamedlg, filename, quamult );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

		m_setcoldlg->InitUndoBuf();
		m_paintdlg->InitUndoBuf();

		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

//		ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), idling, ev0idle, commonid, forbidcommonid );
//		if( ret ){
//			DbgOut( "viewer : LoadMotion : mcdlg AddParentMC error !!!\n" );
//			_ASSERT( 0 );
//			return 1;
//		}

		MOTID* motidptr;
		motidptr = g_motdlg->GetMotIDArray();
		MOTID* curmotid;
		curmotid = motidptr + g_motdlg->GetMotCookie();
		if( curmotid->id != g_motdlg->GetMotCookie() ){
			_ASSERT( 0 );
			return 1;
		}


		ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );//!!!!!!!!!!!!!!!!!
		_ASSERT( !ret );


		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), idling, ev0idle, commonid, forbidnum, forbidid );
			if( ret ){
				DbgOut( "viewer : load qua 5010 : mcdlg AddParentMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else{
			if( idling != 0 ){
				//m_mcdlg->m_idlingid = g_motdlg->GetMotCookie();
				strcpy_s( m_mcdlg->m_idlingname, MAX_PATH, curmotid->filename );
			}
		}


		curmotid->ev0idle = ev0idle;
		curmotid->commonid = commonid;

		if( curmotid->forbidid ){
			free( curmotid->forbidid );
			curmotid->forbidid = 0;
		}
		curmotid->forbidnum = 0;

		if( forbidid && (forbidnum > 0) ){
			curmotid->forbidid = (int*)malloc( sizeof( int ) * forbidnum );
			if( !curmotid->forbidid ){
				DbgOut( "viewer : LoadMotion : forbidid alloc error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			memcpy( curmotid->forbidid, forbidid, sizeof( int ) * forbidnum );
			curmotid->forbidnum = forbidnum;
		}

		//curmotid->forbidcommonid = forbidcommonid;


//DbgOut( "viewer : 5010 : end LoadMotion\r\n" );
	}

	return 0;

}

int CMyD3DApplication::LoadMKM()
{
	int ret;

	if( !m_shandler || !m_thandler || !m_mhandler || !g_motdlg ){
		return 0;
	}
	if( m_ExistData == 0 ){
		return 0;
	}

	if( m_tbb_iti ){
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
	}

	/***
	char buf[_MAX_PATH];
	OPENFILENAME ofn;
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"mkm File (*.mkm)\0*.mkm\0";
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
		return S_OK;
	}
	***/

	CLdMkmDlg dlg;
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( ret != IDOK ){
		return 0;//!!!!!!!!!!
	}

	CMKMFile mkmfile;
	ret = mkmfile.LoadMKMFile( dlg.mkmname );
	if( ret ){
		::MessageBox( m_hWnd, "mkmの読み込み中にエラーが発生しました。\n読み込みの注意についてはマニュアルをご覧ください。", "エラー", MB_OK );
		DbgOut( "viewer : LoadMKMFile : mkmfile LoadMKMFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMKMFile inimkm;
	ret = inimkm.LoadMKMFile( dlg.initname );
	if( ret ){
		::MessageBox( m_hWnd, "初期mkmの読み込み中にエラーが発生しました。\n読み込みの注意についてはマニュアルをご覧ください。", "エラー", MB_OK );
		DbgOut( "viewer : LoadMKMFile : inimkm LOadMKMFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
/////////

	int motno;
	for( motno = 0; motno < mkmfile.m_motnum; motno++ ){

		if( g_motdlg->GetMotionNum() >= MAXMOTIONNUM ){
			char messtr[1024];
			sprintf_s( messtr, 1024, "読み込めるモーションは、%d個までです。\nこれ以上、読み込めません。", MAXMOTIONNUM );
			::MessageBox( m_hWnd, messtr, "エラー", MB_OK );
			return 0;
		}

		//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

		//ret = g_motdlg->LoadMotion( m_loadnamedlg, filename, quamult );
		ret = g_motdlg->LoadMKM( dlg.mkmname, &mkmfile, &inimkm, motno, dlg.mult );

		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

		m_setcoldlg->InitUndoBuf();
		m_paintdlg->InitUndoBuf();

		if( ret ){
			::MessageBox( m_hWnd, "モーション生成中にエラーが発生しました。\n読み込みの注意についてはマニュアルをご覧ください。", "エラー", MB_OK );
			_ASSERT( 0 );
			return 1;
		}


		MOTID* motidptr;
		motidptr = g_motdlg->GetMotIDArray();
		MOTID* curmotid;
		curmotid = motidptr + g_motdlg->GetMotCookie();
		if( curmotid->id != g_motdlg->GetMotCookie() ){
			_ASSERT( 0 );
			return 1;
		}

		ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );//!!!!!!!!!!!!!!!!!
		_ASSERT( !ret );


		int idling = 0;
		int ev0idle = 0;
		int commonid = 0;
		int forbidnum = 0;
		int* forbidid = 0;
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), idling, ev0idle, commonid, forbidnum, forbidid );
			if( ret ){
				DbgOut( "viewer : LoadMKM : mcdlg AddParentMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else{
			if( idling != 0 ){
				//m_mcdlg->m_idlingid = g_motdlg->GetMotCookie();
				strcpy_s( m_mcdlg->m_idlingname, MAX_PATH, curmotid->filename );
			}
		}


		//curmotid->ev0idle = ev0idle;
		//curmotid->commonid = commonid;
		//curmotid->forbidcommonid = forbidcommonid;

	}

	return 0;
}




int CMyD3DApplication::OverWriteFiles()
{
	int ret;

	COverWriteDlg dlg( m_loadnamedlg );
	int dlgret;

	dlgret = (int)dlg.DoModal();
	if( dlgret == IDOK ){
		if( dlg.m_check_sig ){
			ret = SaveSigFile( m_loadnamedlg->m_signame );
			if( ret ){
				DbgOut( "viewer : OverWriteFiles : SaveSigFile error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		if( dlg.m_check_moa ){
			char tmpdirname[MAX_PATH];
			char tmpmoaname[MAX_PATH];

			char* lasten = 0;
			lasten = strrchr( m_loadnamedlg->m_moaname, '\\' );
			if( lasten ){
				int headlen;
				headlen = (int)( lasten - m_loadnamedlg->m_moaname );
				strncpy_s( tmpdirname, MAX_PATH, m_loadnamedlg->m_moaname, headlen );
				tmpdirname[ headlen ] = 0;

				strcpy_s( tmpmoaname, MAX_PATH, lasten + 1 );
				char* extptr = 0;
				extptr = strstr( tmpmoaname, ".moa" );
				if( extptr ){
					*extptr = 0;
				}

				ret = WriteMAFile( tmpdirname, tmpmoaname );
				if( ret ){
					DbgOut( "viewer : OverWriteFiles : WriteMAFile error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
		}
	}

	return 0;
}

int CMyD3DApplication::SelChangeBone( WORD menuid )
{
	if( !m_shandler || !g_motdlg )
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!

	int ret;
	int curjoint = 0;
	D3DXVECTOR3 curjointpos;

	ret = g_motdlg->GetSelectedJointPos( &curjoint, &curjointpos );
	if( ret ){
		_ASSERT( 0 );
		return 0;
	}

	if( curjoint <= 0 )
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!

	CShdElem* selem;
	selem = (*m_shandler)( curjoint );
	_ASSERT( selem );

	CShdElem* nextjoint = 0;
	int nextseri = 0;

	if( menuid == IDM_UP ){
		nextjoint = m_shandler->FindUpperJoint( selem );
		if( nextjoint )
			nextseri = nextjoint->serialno;
	}else if( menuid == IDM_DOWN ){
		nextjoint = m_shandler->GetValidChild( selem->child );
		if( nextjoint && nextjoint->IsJoint() ){
			nextseri = nextjoint->serialno;
		}else{
			nextseri = 0;
		}
	}else if( (menuid == IDM_RIGHT) || (menuid == IDM_LEFT) ){
		int symmtype;
		ret = SOGetSymmNo( m_thandler, m_shandler, selem->serialno, &nextseri, &symmtype );
		_ASSERT( !ret );

		if( nextseri <= 0 ){
			ret = SOGetSymmNo2( m_thandler, m_shandler, selem->serialno, &nextseri, &symmtype );
			_ASSERT( !ret );
		}
	}

	if( nextseri > 0 ){
		::SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)-1, (LPARAM)nextseri );
	}

	return 0;
}

int CMyD3DApplication::ConvSymmXShape()
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler ){
		MessageBox( m_hWnd, "形状データが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}	
	ret = (int)MessageBox( m_hWnd, "実行すると、現在読み込み済みの全てのモーションが削除されます。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( ret != IDOK ){	
		return 0;
	}

///////////

	DestroyMotion( 1 );

///////////


/***
	CQuaternion y180q;
	y180q.SetRotation( 0.0f, 180.0f, 0.0f );

	ret = m_shandler->SetCurrentPose2OrgData( m_mhandler, &y180q );
	if( ret ){
		DbgOut( "viewer : SetInvPose2OrgData : sh SetCurrentPose2OrgData error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
***/
	ret = m_shandler->ConvSymmXShape();
	if( ret ){
		DbgOut( "viewer : ConvSymmXShape : sh ConvSymmXShape error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

///////////

	ret = m_shandler->InvalidateDispObj();
	if( ret ){
		DbgOut( "viewer : ConvSymmXShape : sh InvalidateDispObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_shandler->DestroyBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : ConvSymmXShape : sh DestroyBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//////////

	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : ConvSymmXShape : sh CreateBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_shandler->SetMikoAnchorApply( m_thandler );
	if( ret ){
		DbgOut( "viewer : ConvSymmXShape : sh SetMikoAnchorApply error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = m_shandler->CreateDispObj( m_pd3dDevice, m_mhandler, 0 );
	if( ret ){
		DbgOut( "viewer : ConvSymmXShape : sh CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


////////
	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。

	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	if( m_sigeditwnd ){
		//DbgOut( "CMyD3DApplication : DestroyDispAndMot : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
		//	m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : ConvSymmXShape : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}
int CMyD3DApplication::ConvSymmXMotion()
{
	int ret;

	ret = (int)::MessageBox( m_hWnd, "この機能は、左右対称変換した形状データ用のモーションを変換するものです。\nquaは複数選択できます。\n元のquaに_invをつけた名前で保存します", "確認", MB_OK );
	if( ret != IDOK ){
		return 0;//!!!!!!!!
	}


	OPENFILENAME ofn;
	char buf[ 2560 ];

	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"QUA File (*.qua)\0*.qua\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 ){
		return 0;
	}



	int leng;
	int namecnt = 0;
	leng = (int)strlen( buf );
	char* topchar = buf + leng + 1;
	if( *topchar == 0 ){
		//DbgOut( "%d--%s\r\n", namecnt, buf );
		CQuaFile quafile;
		ret = quafile.ConvSymmXMotion( buf );
		if( ret ){
			DbgOut( "viewer : ConvSymmXMotion : quafile ConvSymmXMotion error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int leng2;
		char fname[512];
		while( *topchar != 0 ){
			//DbgOut( "%d--%s\\%s\r\n", namecnt, buf, topchar );
			sprintf_s( fname, 512, "%s\\%s", buf, topchar );
			CQuaFile quafile;
			ret = quafile.ConvSymmXMotion( fname );
			if( ret ){
				DbgOut( "viewer : ConvSymmXMotion : quafile ConvSymmXMotion error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			leng2 = (int)strlen( topchar );
			topchar = topchar + leng2 + 1;
			namecnt++;
		}
	}

	return 0;
}

int CMyD3DApplication::ConvNormalMap()
{
	CNormalMapDlg dlg;

	int ret;
	HRESULT hr;

	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal();
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( ret == IDOK ){
		LPDIRECT3DTEXTURE9 phmap = NULL;

		hr = D3DXCreateTextureFromFileEx( m_pd3dDevice, (LPTSTR)dlg.hmapname, 
							D3DX_DEFAULT, D3DX_DEFAULT, 
							1,//!!!!!!! 
							0, D3DFMT_A8R8G8B8, 
							D3DPOOL_MANAGED, 
							D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
							//D3DX_FILTER_POINT | D3DX_FILTER_MIRROR,
							g_mipfilter,//!!!!!!! 
							0, NULL, NULL, &phmap );
		if( hr != D3D_OK ){
			DbgOut( "viewer : ConvNormalMap : height map texture create error !!!\n" );
			::MessageBox( m_hWnd, "白黒テクスチャの作成に失敗しました", "エラー", MB_OK );
			_ASSERT( 0 );
			return 1;
		}

		D3DSURFACE_DESC desc;
		hr = phmap->GetLevelDesc( 0, &desc );
		if( hr != D3D_OK ){
			DbgOut( "viewer : ConvNormalMap : hmap GetLevelDesc error !!!\n" );
			::MessageBox( m_hWnd, "高さマップの情報取得に失敗しました", "エラー", MB_OK );
			_ASSERT( 0 );
			phmap->Release();
			return 1;
		}
DbgOut( "check !!!, viewer ConvNormalMap, width %d, height %d\r\n", desc.Width, desc.Height );


		LPDIRECT3DTEXTURE9 pnmap = NULL;
		hr = D3DXCreateTexture( m_pd3dDevice, desc.Width, desc.Height, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pnmap );
		if( hr != D3D_OK ){
			DbgOut( "viewer : ConvNormalMap : normal map texture create error !!!\n" );
			::MessageBox( m_hWnd, "法線テクスチャの作成に失敗しました", "エラー", MB_OK );
			_ASSERT( 0 );
			phmap->Release();
			return 1;
		}

		_ASSERT( phmap );
		_ASSERT( pnmap );

DbgOut( "check !!!, viewer ConvNormalMap, dlg height %f\r\n", dlg.height );

		hr = D3DXComputeNormalMap( pnmap, phmap, NULL, 0, D3DX_CHANNEL_RED, dlg.height );
		if( hr != D3D_OK ){
			DbgOut( "viewer : ConvNormalMap : normal map convert error !!!\n" );
			::MessageBox( m_hWnd, "法線マップの変換に失敗しました", "エラー", MB_OK );
			_ASSERT( 0 );
			phmap->Release();
			pnmap->Release();
			return 1;
		}

		hr = D3DXSaveTextureToFile( (LPTSTR)dlg.nmapname,
			D3DXIFF_BMP,
			(LPDIRECT3DBASETEXTURE9)pnmap,
			NULL );
		if( hr != D3D_OK ){
			DbgOut( "viewer : ConvNormalMap : normal map write error !!!\n" );
			::MessageBox( m_hWnd, "法線マップの書き出しに失敗しました", "エラー", MB_OK );
			_ASSERT( 0 );
			phmap->Release();
			pnmap->Release();
			return 1;
		}
		

		phmap->Release();
		pnmap->Release();
	}

	return 0;
}

int CMyD3DApplication::SaveLimFile()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( !m_ExistData )
		return 0;

	char buf[_MAX_PATH];
	buf[0] = 0;
	OPENFILENAME ofn;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Lim File (*.lim)\0*.lim\0";
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
	ofn.lpstrDefExt ="lim";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}


	CLimFile limfile;
	int ret;

	ret = limfile.SaveLimFile( m_shandler, m_thandler, buf );
	if( ret ){
		DbgOut( "viewer : SaveLimFile : limfile SaveLimFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CMyD3DApplication::LoadLimFile()
{

	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Lim File (*.lim)\0*.lim\0";
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

	CLimFile limfile;
	ret = limfile.LoadLimFile( m_hWnd, m_shandler, m_thandler, buf );
	if( ret ){
		DbgOut( "viewer : LoadLimFile : limfile LoadLimFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::SaveBVHFile()
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( !m_ExistData )
		return 0;

	char buf[_MAX_PATH];
	buf[0] = 0;
	OPENFILENAME ofn;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"BVH File (*.bvh)\0*.bvh\0";
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
	ofn.lpstrDefExt ="bvh";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return 0;
	}

	CBVHFile bvhfile;

	int ret;
	ret = bvhfile.SaveBVHFile( m_thandler, m_shandler, m_mhandler, g_motdlg->GetMotCookie(), buf, 1.0f );
	if( ret ){
		DbgOut( "viewer : SaveBVHFile : bvhfile SaveBVHFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::SetShaderConst()
{
	HRESULT hres;
	hres = g_pEffect->SetMatrix( g_hmView, &m_matView );
	if( hres != D3D_OK ){
		DbgOut( "SetShaderConst : view set error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	hres = g_pEffect->SetMatrix( g_hmProj, &m_matProjX );
	if( hres != D3D_OK ){
		DbgOut( "SetShaderConst : proj set error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


//float3 mLightParams[5][7];
//0: x=validflag, y=type, z=divRange
//1: x=cosPhi, y=divCosPhi
//2: xyz=diffuse
//3: xyz=specular
//4: xyz=ambient
//5: xyz=position
//6: xyz=direction

	float tmplparam[5][7][4];
	ZeroMemory( tmplparam, sizeof( float ) * 5 * 7 * 4 );

	CLightData* curl;
	curl = g_lighthead;
	int setlno = 0;
	int lno;
	for( lno = 0; lno < 5; lno++ ){
		if( curl ){
			tmplparam[lno][0][0] = 1.0f;

			tmplparam[lno][2][0] = curl->Diffuse.x;
			tmplparam[lno][2][1] = curl->Diffuse.y;
			tmplparam[lno][2][2] = curl->Diffuse.z;

			tmplparam[lno][3][0] = curl->Specular.x;
			tmplparam[lno][3][1] = curl->Specular.y;
			tmplparam[lno][3][2] = curl->Specular.z;

			tmplparam[lno][4][0] = curl->Ambient.x;
			tmplparam[lno][4][1] = curl->Ambient.y;
			tmplparam[lno][4][2] = curl->Ambient.z;

			switch( curl->Type ){
			case D3DLIGHT_DIRECTIONAL:
				tmplparam[lno][0][1] = 0.0f;

				tmplparam[lno][6][0] = curl->orgDirection.x;
				tmplparam[lno][6][1] = curl->orgDirection.y;
				tmplparam[lno][6][2] = curl->orgDirection.z;

				break;
			case D3DLIGHT_POINT:
				tmplparam[lno][0][1] = 1.0f;

				tmplparam[lno][0][2] = curl->divRange;


				tmplparam[lno][5][0] = curl->orgPosition.x;
				tmplparam[lno][5][1] = curl->orgPosition.y;
				tmplparam[lno][5][2] = curl->orgPosition.z;

				break;
			case D3DLIGHT_SPOT:
				tmplparam[lno][0][1] = 2.0f;

				tmplparam[lno][0][2] = curl->divRange;
				tmplparam[lno][1][0] = curl->cosPhi;
				tmplparam[lno][1][1] = curl->divcosPhi;

				tmplparam[lno][5][0] = curl->orgPosition.x;
				tmplparam[lno][5][1] = curl->orgPosition.y;
				tmplparam[lno][5][2] = curl->orgPosition.z;

				tmplparam[lno][6][0] = curl->orgDirection.x;
				tmplparam[lno][6][1] = curl->orgDirection.y;
				tmplparam[lno][6][2] = curl->orgDirection.z;

				break;
			}
				
			curl = curl->nextlight;
			setlno++;

		}else{
			tmplparam[lno][0][0] = 0.0f;
			tmplparam[lno][0][1] = -1.0f;
		}
	}

	float lbump[7][4];
	ZeroMemory( lbump, sizeof( float ) * 7 );
	CLightData* lbptr;
	lbptr = GetLightData( g_LightOfBump );
	if( !lbptr && g_lighthead ){
		lbptr = g_lighthead;//存在しなかったら先頭ライト
	}

	if( lbptr ){
		lbump[0][0] = 1.0f;

		lbump[2][0] = lbptr->Diffuse.x;
		lbump[2][1] = lbptr->Diffuse.y;
		lbump[2][2] = lbptr->Diffuse.z;

		lbump[3][0] = lbptr->Specular.x;
		lbump[3][1] = lbptr->Specular.y;
		lbump[3][2] = lbptr->Specular.z;

		lbump[4][0] = lbptr->Ambient.x;
		lbump[4][1] = lbptr->Ambient.y;
		lbump[4][2] = lbptr->Ambient.z;

		switch( lbptr->Type ){
		case D3DLIGHT_DIRECTIONAL:
			lbump[0][1] = 0.0f;

			lbump[6][0] = lbptr->orgDirection.x;
			lbump[6][1] = lbptr->orgDirection.y;
			lbump[6][2] = lbptr->orgDirection.z;

			break;
		case D3DLIGHT_POINT:
			lbump[0][1] = 1.0f;

			lbump[0][2] = lbptr->divRange;


			lbump[5][0] = lbptr->orgPosition.x;
			lbump[5][1] = lbptr->orgPosition.y;
			lbump[5][2] = lbptr->orgPosition.z;

			break;
		case D3DLIGHT_SPOT:
			lbump[0][1] = 2.0f;

			lbump[0][2] = lbptr->divRange;
			lbump[1][0] = lbptr->cosPhi;
			lbump[1][1] = lbptr->divcosPhi;

			lbump[5][0] = lbptr->orgPosition.x;
			lbump[5][1] = lbptr->orgPosition.y;
			lbump[5][2] = lbptr->orgPosition.z;

			lbump[6][0] = lbptr->orgDirection.x;
			lbump[6][1] = lbptr->orgDirection.y;
			lbump[6][2] = lbptr->orgDirection.z;

			break;
		}
	}else{
		//dummy data
		lbump[0][0] = 1.0f;

		lbump[2][0] = 0.0f;
		lbump[2][1] = 0.0f;
		lbump[2][2] = 0.0f;

		lbump[3][0] = 0.0f;
		lbump[3][1] = 0.0f;
		lbump[3][2] = 0.0f;

		lbump[4][0] = 0.0f;
		lbump[4][1] = 0.0f;
		lbump[4][2] = 0.0f;
		///////directional
		lbump[0][1] = 0.0f;

		lbump[6][0] = 0.0f;
		lbump[6][1] = 0.0f;
		lbump[6][2] = 1.0f;

	}

	if( (g_chkVS >= 20) || (g_chkVS == 0) ){
		hres = g_pEffect->SetValue( g_hmLightParams, &(tmplparam[0][0][0]), sizeof( float ) * 3 * 7 * 4 );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriPhong : mLightParam SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		hres = g_pEffect->SetValue( g_hmBumpLight, &(lbump[0][0]), sizeof( float ) * 7 * 4 );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriPhong : mBumpLight SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		hres = g_pEffect->SetValue( g_hmLightParams, &(tmplparam[0][0][0]), sizeof( float ) * 1 * 7 * 4 );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriPhong : mLightParam SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	g_lightnum = setlno;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	return 0;
}

CLightData* CMyD3DApplication::GetLightData( int lid )
{
	CLightData* retlight = 0;
	CLightData* curlight = g_lighthead;

	while( curlight ){
		if( curlight->lightid == lid ){
			retlight = curlight;
			break;
		}
		curlight = curlight->nextlight;
	}

	return retlight;
}

int CMyD3DApplication::GetShaderHandle()
{
	if( !g_pEffect ){
		_ASSERT( 0 );
		return 1;
	}

	g_hRenderSceneBone4 = g_pEffect->GetTechniqueByName( "RenderSceneBone4" );
	g_hRenderSceneBone1 = g_pEffect->GetTechniqueByName( "RenderSceneBone1" );
	g_hRenderSceneBone0 = g_pEffect->GetTechniqueByName( "RenderSceneBone0" );
	g_hRenderSceneBone4Toon = g_pEffect->GetTechniqueByName( "RenderSceneBone4Toon" );
	g_hRenderSceneBone1Toon = g_pEffect->GetTechniqueByName( "RenderSceneBone1Toon" );
	g_hRenderSceneBone0Toon = g_pEffect->GetTechniqueByName( "RenderSceneBone0Toon" );
	g_hRenderSceneBone4ToonNoTex = g_pEffect->GetTechniqueByName( "RenderSceneBone4ToonNoTex" );
	g_hRenderSceneBone1ToonNoTex = g_pEffect->GetTechniqueByName( "RenderSceneBone1ToonNoTex" );
	g_hRenderSceneBone0ToonNoTex = g_pEffect->GetTechniqueByName( "RenderSceneBone0ToonNoTex" );
	g_hRenderSceneBone4Toon1 = g_pEffect->GetTechniqueByName( "RenderSceneBone4Toon1" );
	g_hRenderSceneBone1Toon1 = g_pEffect->GetTechniqueByName( "RenderSceneBone1Toon1" );
	g_hRenderSceneBone0Toon1 = g_pEffect->GetTechniqueByName( "RenderSceneBone0Toon1" );
	g_hRenderSceneBone4Edge0 = g_pEffect->GetTechniqueByName( "RenderSceneBone4Edge0" );
	g_hRenderSceneBone1Edge0 = g_pEffect->GetTechniqueByName( "RenderSceneBone1Edge0" );
	g_hRenderSceneBone0Edge0 = g_pEffect->GetTechniqueByName( "RenderSceneBone0Edge0" );

	g_hmWorldMatrixArray = g_pEffect->GetParameterByName( NULL, "mWorldMatrixArray" );
	g_hmView = g_pEffect->GetParameterByName( NULL, "mView" );
	g_hmProj = g_pEffect->GetParameterByName( NULL, "mProj" );
	g_hmEyePos = g_pEffect->GetParameterByName( NULL, "mEyePos" );
	g_hmLightParams = g_pEffect->GetParameterByName( NULL, "mLightParams" );
	g_hmFogParams = g_pEffect->GetParameterByName( NULL, "mFogParams" );
	g_hmToonParams = g_pEffect->GetParameterByName( NULL, "mToonParams" );
	g_hmLightNum = g_pEffect->GetParameterByName( NULL, "mLightNum" );
	g_hmEdgeCol0 = g_pEffect->GetParameterByName( NULL, "mEdgeCol0" );
	g_hmAlpha = g_pEffect->GetParameterByName( NULL, "mAlpha" );


	if( (g_chkVS >= 20) || (g_chkVS == 0) ){
		g_hRenderSceneBone4Bump = g_pEffect->GetTechniqueByName( "RenderSceneBone4Bump" );//vs2
		g_hRenderSceneBone1Bump = g_pEffect->GetTechniqueByName( "RenderSceneBone1Bump" );//vs2
		g_hRenderSceneBone0Bump = g_pEffect->GetTechniqueByName( "RenderSceneBone0Bump" );//vs2

		g_hmBumpLight = g_pEffect->GetParameterByName( NULL, "mBumpLight" );//vs2
		g_hNormalMap = g_pEffect->GetParameterByName( NULL, "NormalMap" );//vs2
		g_hDecaleTex = g_pEffect->GetParameterByName( NULL, "DecaleTex" );//vs2
	}
	return 0;
}
