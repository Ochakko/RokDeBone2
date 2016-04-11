#include "stdafx.h"


//#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <mmsystem.h>
#include <stdio.h>
#include <io.h>
#include <tchar.h>
#include <D3D9.h>
#include "viewer.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DRes.h"

#include <intrin.h>

#include "MotDlg.h"
#include "RegistDlg.h"

#include <commctrl.h>

#include "resource.h"

#include <coef.h>

#define DBGH
#include <dbg.h>

#include "inidata.h" // for ini file


static const int s_appindex = 2;

static const char s_appserial[5][37] = { 
	"CD8A0EE5-D92C-4147-BCAE-571BB9528F32",
	"8B213FFD-3D5D-4258-9518-499F7A414A25",
	"605FD046-EF70-402f-9F56-5D80AF09F898",
	"2938BC47-AF34-47ec-9897-32DC5599B8AA",
	"3A4AFF7B-DDAC-4901-8675-B2BC40443A56"
};

static const char s_appkey[5][37] = {
	"70E0E6B6-50D0-4782-9552-89167D2D2880",
	"93A97712-9849-4cd1-8C0E-70CF8C7F1DED",
	"E585CFDE-80C8-421d-A0B2-24D4811310EB",
	"73D34227-107A-4d8c-AEB9-8A6DDD6A36FB",
	"E18F2F5B-568F-4c83-AB2F-ED0BE88C0800"
};

//////// application

//!!!!!!!!!!!!!!!!!!!!!!!!!!!
// キャストのwaringを非表示にする
//!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#pragma warning(disable:4311)


//#include "CameraDlg.h"
//#include "SigEditDlg.h"
#include "SigEditWnd.h"
#include "GraphWnd.h"
#include "OptDlg.h"
#include "MotDlg.h"
#include "BgDlg.h"
#include "VersionDlg.h"
#include "LdSigDlg.h"
#include "LdEsiDlg.h"
#include "LdEquDlg.h"
#include "WriteBFDlg.h"
#include "WriteFBXDlg.h"
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
#include "Toon0Dlg.h"
//#include "LdFBXDlg.h"
#include "LdXDlg.h"
#include "MotChangeDlg.h"
#include "OverWriteDlg.h"
#include "LdQuaDlg.h"
#include "ExtSigDlg.h"
#include "MaterialDlg.h"
#include "GlowViewDlg.h"
#include "NormalMapDlg.h"
#include "BonePropDlg.h"
#include "LdMkmDlg.h"
#include "ShadowParamDlg.h"
#include "Za4Dlg.h"
#include "FrameCopyDlg.h"
#include "CreateKeyDlg.h"
#include "MFolderDlg.h"
#include "motparamdlg.h"
#include "GraphRotDlg.h"
#include "SymBoneDlg.h"
#include "SymJPosDlg.h"
#include "SoundBankDlg.h"
#include "SoundBank.h"
#include "HuSound.h"
#include "SyncDlg.h"
#include "WriteBldDlg.h"
#include "InfoDlg.h"

#include "PluginElem.h"
#include "PluginKinect.h"

#include <SymmOpe.h>

//#define STRICT
//#include <windowsx.h>
#include <windows.h>
//#include <winuser.h>
#include <tchar.h>
#include <assert.h>
#include <mmsystem.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <D3DX9.h>


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

#include <CameraAnimHandler.h>
#include <CameraAnim.h>

#include <GPAnimHandler.h>
#include <GPAnim.h>

#include <MOEAnimHandler.h>
#include <MOEAnim.h>

#include <MCHandler.h>

#include <sigfile.h>
#include <rokfile.h>
#include <mqofile.h>
#include <BoneFile.h>
#include <XFile.h>
#include <MAFile.h>
#include <LimFile.h>
#include <BVHFile.h>
#include <mkmfile.h>
#include <Toon1File.h>
#include <PoseFile.h>
#include <CameraFile.h>
#include <GPFile.h>
#include <MOEFile.h>
#include <SndSetFile.h>
#include <BLDFile.h>
#include <ESIFile.h>
#include <EQUFile.h>
//#include "txtsetting.h"

//#include <FBXFile.h>

//#include <BoneRateFile.h>

#include <dxerr.h>

#include <Commdlg.h>

#include <bgdisp2.h>

#include <basedat.h>
#include <dibfile.h>

#include <lightdata.h>

#include "MQODlg.h"

#include "inidata.h" // for ini file

#include <im2file.h>
#include <cpuid.h>
#include <asmfunc.h>

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

#include <LiSPSM.h>
#include <Vec3List.h>

#include <morph.h>
#include <MMotElem.h>

#include <FBXExporter.h>


///////////////////
///////////////////
///// application
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

enum {
	CAMOPE_NONE,
	CAMOPE_KAKU,
	CAMOPE_I,
	CAMOPE_KAI,
	CAMOPE_MAX
};

//keyanddrag
enum {
	KAD_NORMAL,
	KAD_T,
	KAD_T2,
	KAD_IKT,
	KAD_MAX
};
//	if( g_KeyBuffer['1'] & 0x80 ){
//	}

//-------- define
//#define	RADINITVAL	0.0f
//#define RAD2INITVAL	0.0f

#define TIMERINTERVAL	16


static LPITEMIDLIST GetNextItemID(LPCITEMIDLIST pidl);


//toolbar button id
//IDTBB_*はcoef.hへ
// tool bar
#define ID_TOOLBAR	500

TBBUTTON tbb[27] = {
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
		{13, IDTBB_4, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},	
				
		{14, IDTBB_FACE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{15, IDTBB_LINE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{16, IDTBB_POINT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{17, IDTBB_TEN2, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

		{18, IDTBB_ZA1, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{19, IDTBB_ZA2, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{20, IDTBB_ZA3, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{21, IDTBB_ZA4, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

		{22, IDTBB_COLP, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{23, IDTBB_COLT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{24, IDTBB_COLPP, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

		{25, IDTBB_SHA, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
		{26, IDTBB_GLOW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0}
};
TBBUTTON tb_space = {0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0};

HWND g_hToolBar = NULL;


//------- global
int g_loadingflag = 0;
int g_endapp = 0;
SLispStatus	g_sStatus;//!< LiSPSMステータス

ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
ID3DXEffect*			g_pPostEffect = NULL;

int g_rendertarget = TARGET_DEFAULT;
float g_specular = 0.0f;
float g_ambient = 0.3f;

float g_proj_far = 40000.0f;//<---- RokDeBone2では、m_proj_farを使う、d3ddisp.cppのために、ここで、定義。
//float g_proj_far = 100000.0f;//<---- RokDeBone2では、m_proj_farを使う、d3ddisp.cppのために、ここで、定義。

int g_fogenable = 0;
D3DCOLOR g_fogcolor = 0x00FFFFFF;
float g_fogstart = 0.0f;
float g_fogend = g_proj_far;	
float g_fogtype = 0.0f;

int g_index32bit = 0;

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

CCpuid g_cpuinfo;

HWND g_hStatus = 0;
#define ID_STATUSBAR 100

CColDlg g_coldlg;

CRITICAL_SECTION g_crit_tempmp; 

int g_rendercnt = 0;

int g_LightOfBump[3] = { 0, -1, -1 };
int g_lightnum = 0;
int g_bumplightnum = 0;
unsigned char g_KeyBuffer[256];
int g_keyboardcnt[256];
int g_validkey[256];

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
D3DXHANDLE g_hRenderSceneNormalShadow = 0;//vs2
D3DXHANDLE g_hRenderSceneBumpShadow = 0;//vs2
D3DXHANDLE g_hRenderSceneToon1Shadow = 0;//vs2
D3DXHANDLE g_hRenderSceneToon0Shadow = 0;//vs2
D3DXHANDLE g_hRenderSceneBone4PP = 0;
D3DXHANDLE g_hRenderSceneBone0PP = 0;
D3DXHANDLE g_hRenderScenePPShadow = 0;

D3DXHANDLE g_hRenderZN_2path_NormalBone = 0;
D3DXHANDLE g_hRenderZN_2path_NormalNoBone = 0;


D3DXHANDLE g_hmWorldMatrixArray = 0;
D3DXHANDLE g_hmView = 0;
D3DXHANDLE g_hmoldView = 0;
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
D3DXHANDLE g_hDecaleTex1 = 0;//vs2
D3DXHANDLE g_hmLP = 0;//vs2
D3DXHANDLE g_hmLPB = 0;//vs2
D3DXHANDLE g_hShadowMap = 0;//vs2
D3DXHANDLE g_hmShadowCoef = 0;//vs2
D3DXHANDLE g_hmMinAlpha = 0;
D3DXHANDLE g_hmGlowMult = 0;
D3DXHANDLE g_hmToonDiffuse = 0;
D3DXHANDLE g_hmToonAmbient = 0;
D3DXHANDLE g_hmToonSpecular = 0;


D3DXHANDLE g_hPostEffectBlur0 = 0;
D3DXHANDLE g_hPostEffectGauss = 0;
D3DXHANDLE g_hPostEffectTLV = 0;

D3DXHANDLE g_hpeWidth0 = 0;
D3DXHANDLE g_hpeHeight0 = 0;
D3DXHANDLE g_hpeWidth1 = 0;
D3DXHANDLE g_hpeHeight1 = 0;
D3DXHANDLE g_hpeTexture0 = 0;

D3DXHANDLE g_hpeWeight = 0;
D3DXHANDLE g_hpeOffsetX = 0;
D3DXHANDLE g_hpeOffsetY = 0;
D3DXHANDLE g_hpeSampleWeight = 0;
D3DXHANDLE g_hpeSampleOffset = 0;
D3DXHANDLE g_hpeColMult = 0;


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
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------

int g_useGPU = 0;
int g_usePS = 0;
int g_chkVS = 0;

int g_bumpEnable = 0;
int g_bumpUse = 0;
int g_shadowEnable = 0;//ハードウェア的に可能かどうか
int g_shadowUse = 0;//実際に使うかどうか



int MAXSKINMATRIX = 42;//!!!!!!!!!
int MAXBLURMATRIX = 19;

//--------- static 
static CMyD3DApplication* g_pD3DApp = NULL;


static int s_registflag = 0;
static HKEY s_hkey;
static int RegistKey();
static int IsRegist();

int RegistKey()
{
	CRegistDlg dlg;
	dlg.DoModal();

	if( strcmp( s_appkey[ s_appindex ], dlg.m_regkey ) == 0 ){
		if( s_registflag == 0 ){
			LONG lret;
			DWORD dwret;
			lret = RegCreateKeyEx( HKEY_CURRENT_USER, "Software\\OchakkoLab\\RokDeBone2DX", 0, "",
				REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &s_hkey, &dwret );
			if( dwret == REG_CREATED_NEW_KEY ){
				lret = RegSetValueEx( s_hkey, "registkey", 0, REG_SZ, (LPBYTE)(dlg.m_regkey), sizeof(char) * 36 );
				if( lret != ERROR_SUCCESS ){
					::MessageBox( NULL, "エラー　：　レジストに失敗しました。", "レジストエラー", MB_OK );
				}
				RegCloseKey( s_hkey );
				::MessageBox( NULL, "成功　：　レジストに成功しました。", "レジスト成功", MB_OK );
			}else{
				::MessageBox( NULL, "エラー　：　レジストに失敗しました。", "レジストエラー", MB_OK );
			}
		}
	}else{
		::MessageBox( NULL, "エラー　：　不正なレジストキーです。", "レジストエラー", MB_OK );
	}

	return 0;
}

int IsRegist()
{
	/*
	s_registflag = 0;

	LONG lret;
	DWORD dwret;
	lret = RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\OchakkoLab\\RokDeBone2DX", 0, KEY_ALL_ACCESS, &s_hkey );
	if( lret == ERROR_SUCCESS ){
		DWORD dwtype;
		char strkey[37] = {0};
		DWORD dwsize = 37;
		lret = RegQueryValueEx( s_hkey, "registkey", NULL, &dwtype, (LPBYTE)strkey, &dwsize );
		if( lret == ERROR_SUCCESS ){
			if( dwtype == REG_SZ ){
				if( strncmp( strkey, s_appkey[ s_appindex ], 36 ) == 0 ){
					s_registflag = 1;
				}
			}
		}
		RegCloseKey( s_hkey );
	}
	*/

	s_registflag = 1;

	return 0;

}

//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Constructor
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication() : m_3dwnd(_T("Static"), this, MAIN3D_MSGMAP)
{
	m_camope = CAMOPE_NONE;
	m_HS = 0;

	m_hbmp = NULL;
	m_InitFlag = 0;

	g_pD3DApp           = this;

    m_dwNumAdapters     = 0;
    m_dwAdapter         = 0L;
    m_pD3D              = NULL;
    m_pd3dDevice        = NULL;
//    m_hWnd              = NULL;
    m_hWndFocus         = NULL;
    m_bActive           = FALSE;
    m_bReady            = FALSE;
    m_dwCreateFlags     = 0L;
	m_screensave		= 0;

    m_bFrameMoving      = TRUE;
    m_bSingleStep       = FALSE;
    m_fFPS              = 0.0f;
    m_strDeviceStats[0] = _T('\0');
    m_strFrameStats[0]  = _T('\0');

    m_strWindowTitle    = _T("D3D8 Application");
    //m_dwCreationWidth   = 400;
    //m_dwCreationHeight  = 300;
//m_bUseDepthBuffer   = FALSE;
m_bUseDepthBuffer = TRUE;

	m_dwMinDepthBits    = 16;
    m_dwMinStencilBits  = 0;
    m_bShowCursorWhenFullscreen = FALSE;


	HWND hWnd;
	RECT rect;
    hWnd = ::GetDesktopWindow();
    ::GetWindowRect(hWnd, &rect);
    //m_dwCreationWidth   = (DWORD)((float)(rect.right - rect.left) * 0.6f);
    //m_dwCreationHeight  = (DWORD)((float)(rect.bottom - rect.top) * 0.6f) ;    


	OpenDbgFile();

	//moduledirのセット
	SetModuleDir();

	g_inidata->LoadParams( "rdb2.ini" );//!!!!!!!!!!!!
	m_dwCreationWidth = (DWORD)( g_inidata->main_width );
	m_dwCreationHeight = (DWORD)( g_inidata->main_height );


	m_bluebrush = NULL;

	m_writebmpflag = 0;

	m_force_render = 0;
	m_rendertime = 0;

	m_lbutton = 0;
	m_movebone = 0;
	m_pickgp = 0;

	m_rbutton = 0;
	m_mbutton = 0;
	m_shiftbone = 0;
	m_shiftpart = -1;
	m_shifvertnum = 0;
	ZeroMemory( m_shifvert, sizeof( int ) * PICKVERTMAX );
	m_shiftface = -1;

	m_onselectplugin = 0;
	m_kinectflag = 0;

	m_intormenu = 0;
	m_scalebone = 0;

	m_lrbutton = 0;

	m_vikflag = 0;

	m_pZSurf = 0;
	m_pBackBuffer = 0;
	m_resizeflag = 0;


	m_preview_flag = 0;

//	m_MultiSampleType = D3DMULTISAMPLE_NONE;

	m_menu = NULL;
	m_animmenu = NULL;
	m_cameramenu = NULL;
	m_gpmenu = NULL;
	m_moemenu = NULL;

///////////////
// application
    InitializeCriticalSection( &g_crit_tempmp );

	m_strWindowTitle    = _T("RokDeBone2DX");
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

	m_dtri_th = 0;
	m_dtri_sh = 0;
	m_dtri_mh = 0;

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
	m_sel_ringxno = 0;//"ring_X"
	m_sel_ringyno = 0;//"ring_Y"
	m_sel_ringzno = 0;//"ring_Z"



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

	m_ei3_th = 0;
	m_ei3_sh = 0;
	m_ei3_mh = 0;

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

//	m_cameradlg = 0;
	m_sigeditwnd = 0;
	m_graphwnd = 0;
	m_optdlg = 0;
	m_bonelinedlg = 0;
	m_loadnamedlg = 0;
	m_wblddlg = 0;
	m_itimodedlg = 0;
	m_itidlg = 0;
	m_lightdlg = 0;
	m_setcoldlg = 0;
	m_bonedlg = 0;
	m_za4dlg = 0;
	m_infscopedlg = 0;
	m_paintdlg = 0;
	m_Toon0Dlg = 0;
	m_syncdlg = 0;
	m_syncdisp = 0;
	m_mcdlg = 0;
	m_wmafdlg = 0;
	m_materialdlg = 0;
	m_glowdlg = 0;
	g_bonepropdlg = 0;

	//m_disp_cameradlg = 0;
	//m_disp_sigdlg = 0;
	//m_disp_motdlg = 0;
	m_refuge_motdlg = 0;

	m_moving_cameradlg = 0;
	m_moving_sigdlg = 0;


//	m_parsdlg = 0;
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
	m_proj_far = 40000.0f;
//m_proj_far = 100000.0f;
	m_fov = 45.0f;
	m_ortho = 0;
	m_orthosize = 3000.0f;

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
	m_tbb_4 = 0;

	m_tbb_face = 1;
	m_tbb_line = 0;
	m_tbb_point = 0;
	m_tbb_ten2 = 0;

	m_tbb_za1 = 1;
	m_tbb_za2 = 0;
	m_tbb_za3 = 0;
	m_tbb_za4 = 0;

	m_tbb_colP = 1;
	m_tbb_colT = 0;
	m_tbb_colPP = 0;
	m_tbb_sha = 0;
	m_tbb_glow = 0;

	m_za_kind = ZA_1;

	m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

	m_g_disp = 1;

	m_plugin = 0;
	m_kinect = 0;

	ZeroMemory( &m_vEyePt, sizeof( D3DXVECTOR3 ) );
	m_camup.x = 0.0f;
	m_camup.y = 1.0f;
	m_camup.z = 0.0f;

	m_wheeldelta = 0.0f;

	m_movebone = 0;
	m_pickgp = 0;
	m_shiftbone = 0;
	m_scalebone = 0;

	ZeroMemory( m_signame, sizeof( char ) * 1024 );

	m_shadowelem.pZSurf = 0;
	m_shadowelem.pBackBuffer = 0;
	m_shadowelem.pTex = 0;
	m_shadowelem.width = 0;
	m_shadowelem.height = 0;

	m_createshadowflag = 0;
	m_shadowcoef[0] = 0.005f;// [0]: bias
	m_shadowcoef[1] = 1.0f;// [1]: darkness

	m_effectdecl = 0;
	m_tlvdecl = 0;

	m_remaketree_calc0 = 0;

	//ZeroMemory( m_autosavedir, sizeof( char ) * MAX_PATH );	//SetModuleDir-->SetMediaDirで初期化
	m_camera.looktype = CAML_NORMAL;
	m_camera.pos = D3DXVECTOR3( 0.0f, m_eye_y, -CAMDISTINITVAL );
	m_camera.target = D3DXVECTOR3( 0.0f, m_eye_y, 0.0f );
	m_camera.up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_camera.dist = CAMDISTINITVAL;
	m_camera.hsid = -1;
	m_camera.boneno = -1;
	m_camera.nearz = 100.0f;
	m_camera.farz = 40000.0f;
	m_camera.fov = 45.0f;
	m_camera.ortho = 0;
	m_camera.orthosize = 3000.0f;

	int vkno;
	for( vkno = 0; vkno < 4; vkno++ ){
		m_savecamera[vkno].looktype = CAML_NORMAL;
		m_savecamera[vkno].pos = D3DXVECTOR3( 0.0f, m_eye_y, -CAMDISTINITVAL );
		m_savecamera[vkno].target = D3DXVECTOR3( 0.0f, m_eye_y, 0.0f );
		m_savecamera[vkno].up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		m_savecamera[vkno].dist = CAMDISTINITVAL;
		m_savecamera[vkno].hsid = -1;
		m_savecamera[vkno].boneno = -1;
		m_savecamera[vkno].nearz = 100.0f;
		m_savecamera[vkno].farz = 40000.0f;
		m_savecamera[vkno].fov = 45.0f;
		m_savecamera[vkno].ortho = 0;
		m_savecamera[vkno].orthosize = 3000.0f;
	}

	m_soundbank = 0;

	D3DXMatrixIdentity( &m_inimat );

	ResetKeyboardCnt();

	m_freq.QuadPart = 0;
	m_rendercnt.QuadPart = 0;
	m_curcnt.QuadPart = 0;
	m_dfreq = 0.0;
	m_drenderfreq = 0.0;
	m_drendercnt = 0.0;
	m_dcurcnt = 0.0;

	ZeroMemory( &m_camkaku_rect, sizeof( RECT ) );
	ZeroMemory( &m_cami_rect, sizeof( RECT ) );
	ZeroMemory( &m_camkai_rect, sizeof( RECT ) );

	m_destroyflag = 0;
	m_keyanddrag = KAD_NORMAL;
	m_miniflag = 0;

//	m_dlgfont = 0;
}

CMyD3DApplication::~CMyD3DApplication()
{

//	if( m_menu != NULL ){//wm_closeで削除する
//		DestroyMenu( m_menu );
//		m_menu = NULL;
//	}

}


//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
//LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//    return g_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
//}




//-----------------------------------------------------------------------------
// Name: Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::CreateWnd( HINSTANCE hInstance )
{
    HRESULT hr;

//	HANDLE hThread;
//	hThread = GetCurrentThread();
//	SetThreadAffinityMask( hThread, 1 );

	QueryPerformanceFrequency( &m_freq );
	m_dfreq = (double)m_freq.QuadPart;


	DbgOut( "Create : freq : Hight %d, Low %d, %f\n", m_freq.HighPart, m_freq.LowPart, m_dfreq );

	InitCommonControls();//!!!!!!!!!!!


    // Create the Direct3D object
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( m_pD3D == NULL )
        return DisplayErrorMsg( D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT );

    // Build a list of Direct3D adapters, modes and devices. The
    // ConfirmDevice() callback is used to confirm that only devices that
    // meet the app's requirements are considered.
    if( FAILED( hr = BuildDeviceList() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }


	//char mes[512];

    // Unless a substitute hWnd has been specified, create a window to
    // render into
    if( m_hWnd == NULL)
    {

		if( m_bluebrush == NULL ){
			//COLORREF bluecol = RGB( 0, 0, 255 );
			//m_bluebrush = ::CreateSolidBrush( bluecol );
			//_ASSERT( m_bluebrush != NULL );

			unsigned char colr, colg, colb;
			colr = GetRValue( g_inidata->bg_color );
			colg = GetGValue( g_inidata->bg_color );
			colb = GetBValue( g_inidata->bg_color );
			COLORREF bluecol = RGB( colr, colg, colb );
			m_bluebrush = ::CreateSolidBrush( bluecol );
			_ASSERT( m_bluebrush != NULL );
		}


        // Register the windows class
//		WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
//							  //LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
//							  LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON1) ),
//							  LoadCursor( NULL, IDC_ARROW ),
//							  //(HBRUSH)GetStockObject(WHITE_BRUSH),
//							  m_bluebrush,
//							  NULL, _T("D3D Window") };
//RegisterClass( &wndClass );

        // Set the window's initial style
        m_dwWindowStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
                          WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPCHILDREN;

        // Set the window's initial width
        RECT rc;
        //SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );
//		SetRect( &rc, 
//			g_inidata->main_posx, 
//			g_inidata->main_posy, 
//			g_inidata->main_posx + g_inidata->main_width, 
//			g_inidata->main_posy + g_inidata->main_height );
		rc.left = g_inidata->main_posx;
		rc.top = g_inidata->main_posy;
		rc.right = g_inidata->main_posx + g_inidata->main_width;
		rc.bottom = g_inidata->main_posy + g_inidata->main_height;
//        AdjustWindowRect( &rc, m_dwWindowStyle, TRUE );//!!!!!!!!!!!!!!!!!!!!!!!!!! サイズが変わるため取り除いた

//sprintf_s( mes, 512, "time1 x %d, y %d, width %d, height %d",
//		  rc.left, rc.top,
//		  rc.right - rc.left,
//		  rc.bottom - rc.top );
//::MessageBox( NULL, mes, "check", MB_OK );

        // Create the render window
        //m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle, m_dwWindowStyle,
        //                       //CW_USEDEFAULT, CW_USEDEFAULT,
		//					   g_inidata->main_posx, g_inidata->main_posy,
        //                       g_inidata->main_width, g_inidata->main_height, 0L,
        //                       LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) ),
        //                       hInstance, 0L );


		HWND createwnd;
		createwnd = Create( NULL, rc, "RokDeBone2", m_dwWindowStyle );
		if( !createwnd ){
			_ASSERT( 0 );
			return 1;
		}


		if( m_menu == NULL ){
			//m_menu = LoadMenu( (HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE( IDR_MENU ) );
			m_menu = LoadMenu( (HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE( 141 ) );
			if( m_menu == NULL ){
				_ASSERT( 0 );
				return 1;
			}
			BOOL bret;
			bret = ::SetMenu( m_hWnd, m_menu );
			if( bret == 0 ){
				_ASSERT( 0 );
				return 1;
			}
		}
		//UpdateWindow( m_hWnd );


	}
	::DragAcceptFiles( m_hWnd, TRUE );


	DestroyAppToolBar();
	int ret;
	ret = CreateAppToolBar();
	if( ret ){
		DbgOut( "d3dapp : CreateAppToolBar error !!!\n" );
        return DisplayErrorMsg( -1, MSGERR_APPMUSTEXIT );
	}

//RECT rc0;
//GetWindowRect( &rc0 );
//sprintf_s( mes, 512, "time2 x %d, y %d, width %d, height %d",
//		  rc0.left, rc0.top,
//		  rc0.right - rc0.left,
//		  rc0.bottom - rc0.top );
//::MessageBox( NULL, mes, "check", MB_OK );


    // Save window properties
    m_dwWindowStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );
    ::GetWindowRect( m_hWnd, &m_rcWindowBounds );
    ::GetClientRect( m_hWnd, &m_rcWindowClient );


	ComputeRect( 0, 0 );


	HWND newwnd;
//	newwnd = m_3dwnd.Create( m_hWnd, m_rc3dwnd, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
	newwnd = m_3dwnd.Create( m_hWnd, m_rc3dwnd, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
	if( !newwnd ){
		DbgOut( "viewer : CreateWnd : 3dwnd Create error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	m_sigeditwnd = new CSigEditWnd();
	if( !m_sigeditwnd ){
		DbgOut( "SigEditWnd alloc error" );
		_ASSERT( 0 );
		return 1;
	}
	m_sigeditwnd->Create(
	   m_hWnd,
	   m_rcsigedit,
	   NULL,
	   WS_CHILD | WS_VISIBLE | WS_THICKFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	   WS_EX_WINDOWEDGE
	);
	m_sigeditwnd->SetAppWnd( m_hWnd, this );
	m_sigeditwnd->SetForbidDD( g_inidata->forbidDD );
	
	
	// The focus window can be a specified to be a different window than the
    // device window.  If not, use the device window as the focus window.
    if( m_hWndFocus == NULL )
		m_hWndFocus = m_3dwnd.m_hWnd;
        //m_hWndFocus = m_hWnd;



    // Initialize the app's custom scene stuff
    if( FAILED( hr = OneTimeSceneInit() ) )
	{
        //SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }
	
    // Initialize the 3D environment for the app
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }


    // Setup the app so it can support single-stepping
    DXUtil_Timer( TIMER_START );

    // The app is ready to go
    m_bReady = TRUE;
	m_InitFlag = 1;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList).
//-----------------------------------------------------------------------------
int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
    D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

    if( p1->Format > p2->Format )   return -1;
    if( p1->Format < p2->Format )   return +1;
    if( p1->Width  < p2->Width )    return -1;
    if( p1->Width  > p2->Width )    return +1;
    if( p1->Height < p2->Height )   return -1;
    if( p1->Height > p2->Height )   return +1;

    return 0;
}




//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::BuildDeviceList()
{
	HRESULT hr;
    const DWORD dwNumDeviceTypes = 2;
    const TCHAR* strDeviceDescs[] = { _T("HAL"), _T("REF") };
    const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

    BOOL bHALExists = FALSE;
    BOOL bHALIsWindowedCompatible = FALSE;
    BOOL bHALIsDesktopCompatible = FALSE;
    BOOL bHALIsSampleCompatible = FALSE;

	DWORD m, f;

    // Loop through all the adapters on the system (usually, there's just one
    // unless more than one graphics card is present).
    for( UINT iAdapter = 0; iAdapter < m_pD3D->GetAdapterCount(); iAdapter++ )
    {
        // Fill in adapter info
        D3DAdapterInfo* pAdapter  = &m_Adapters[m_dwNumAdapters];
        hr = m_pD3D->GetAdapterIdentifier( iAdapter, 0, &pAdapter->d3dAdapterIdentifier );
		if( hr != D3D_OK ){
			DbgOut( "app : BuildDeviceList : GetAdapterIdentifier error !!! %x\r\n", hr );
			_ASSERT( 0 );
		}
        hr = m_pD3D->GetAdapterDisplayMode( iAdapter, &pAdapter->d3ddmDesktop );
		if( hr != D3D_OK ){
			DbgOut( "app : BuildDeviceList : GetAdapterDisplayMode error !!! %x\r\n", hr );
			_ASSERT( 0 );
		}

        pAdapter->dwNumDevices    = 0;
        pAdapter->dwCurrentDevice = 0;

		D3DFORMAT supportfmt[5] = {
			D3DFMT_A8R8G8B8,
			D3DFMT_X8R8G8B8,
			D3DFMT_A1R5G5B5,
			D3DFMT_R5G6B5,
			D3DFMT_X1R5G5B5
		};
		D3DFORMAT curfmt;


        // Enumerate all display modes on this adapter
        D3DDISPLAYMODE modes[100];
        D3DFORMAT      formats[20];
        DWORD dwNumFormats      = 0;
        DWORD dwNumModes        = 0;
        DWORD dwNumAdapterModes = 0;
		
		int fmtno;
		for( fmtno = 0; fmtno < 5; fmtno++ ){
			curfmt = supportfmt[fmtno];

			dwNumAdapterModes += m_pD3D->GetAdapterModeCount( iAdapter, curfmt );

			// Add the adapter's current desktop format to the list of formats
			formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

			for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
			{
				// Get the display mode attributes
				D3DDISPLAYMODE DisplayMode;
				hr = m_pD3D->EnumAdapterModes( iAdapter, curfmt, iMode, &DisplayMode );
				if( hr != D3D_OK ){
					//DbgOut( "app : BuildDeviceList : EnumAdapterModes error !!! %x\r\n", hr );
					//_ASSERT( 0 );
					continue;
				}


				// Filter out low-resolution modes
				if( DisplayMode.Width  < 640 || DisplayMode.Height < 400 )
					continue;

				// Check if the mode already exists (to filter out refresh rates)
				for( m=0L; m<dwNumModes; m++ )
				{
					if( ( modes[m].Width  == DisplayMode.Width  ) &&
						( modes[m].Height == DisplayMode.Height ) &&
						( modes[m].Format == DisplayMode.Format ) )
						break;
				}

				// If we found a new mode, add it to the list of modes
				if( m == dwNumModes )
				{
					modes[dwNumModes].Width       = DisplayMode.Width;
					modes[dwNumModes].Height      = DisplayMode.Height;
					modes[dwNumModes].Format      = DisplayMode.Format;
					modes[dwNumModes].RefreshRate = 0;					
					
					dwNumModes++;

					// Check if the mode's format already exists
					for( f=0; f<dwNumFormats; f++ )
					{
						if( DisplayMode.Format == formats[f] )
							break;
					}

					// If the format is new, add it to the list
					if( f== dwNumFormats )
						formats[dwNumFormats++] = DisplayMode.Format;
				}
			}
		}

        // Sort the list of display modes (by format, then width, then height)
        qsort( modes, dwNumModes, sizeof(D3DDISPLAYMODE), SortModesCallback );

        // Add devices to adapter
        for( UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++ )
        {
            // Fill in device info
            D3DDeviceInfo* pDevice;
            pDevice                 = &pAdapter->devices[pAdapter->dwNumDevices];
            pDevice->DeviceType     = DeviceTypes[iDevice];
            m_pD3D->GetDeviceCaps( iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps );
            pDevice->strDesc        = strDeviceDescs[iDevice];
            pDevice->dwNumModes     = 0;
            pDevice->dwCurrentMode  = 0;
            pDevice->bCanDoWindowed = FALSE;
            pDevice->bWindowed      = FALSE;
            pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

            // Examine each format supported by the adapter to see if it will
            // work with this device and meets the needs of the application.
            BOOL  bFormatConfirmed[20];
            DWORD dwBehavior[20];
            D3DFORMAT fmtDepthStencil[20];

            for( f=0; f<dwNumFormats; f++ )
            {
                bFormatConfirmed[f] = FALSE;
                fmtDepthStencil[f] = D3DFMT_UNKNOWN;

                // Skip formats that cannot be used as render targets on this device
                if( FAILED( m_pD3D->CheckDeviceType( iAdapter, pDevice->DeviceType,
                                                     formats[f], formats[f], FALSE ) ) )
                    continue;

                if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                {
                    // This system has a HAL device
                    bHALExists = TRUE;

                    //if( pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED )
                    //{
                        // HAL can run in a window for some mode
                    //    bHALIsWindowedCompatible = TRUE;

                    //    if( f == 0 )
                    //    {
                            // HAL can run in a window for the current desktop mode
                            bHALIsDesktopCompatible = TRUE;
                    //    }
                    //}
                }

                // Confirm the device/format for HW vertex processing
                if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
                {
                    if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
                                        D3DCREATE_PUREDEVICE;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }
                }

                // Confirm the device/format for SW vertex processing
                if( FALSE == bFormatConfirmed[f] )
                {
                    dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

                    if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                  formats[f] ) ) )
                        bFormatConfirmed[f] = TRUE;
                }

                // Find a suitable depth/stencil buffer format for this device/format
                if( bFormatConfirmed[f] && m_bUseDepthBuffer )
                {
                    if( !FindDepthStencilFormat( iAdapter, pDevice->DeviceType,
                        formats[f], &fmtDepthStencil[f] ) )
                    {
                        bFormatConfirmed[f] = FALSE;
                    }
                }
            }

            // Add all enumerated display modes with confirmed formats to the
            // device's list of valid modes
            for( m=0L; m<dwNumModes; m++ )
            {
                for( f=0; f<dwNumFormats; f++ )
                {
                    if( modes[m].Format == formats[f] )
                    {
                        if( bFormatConfirmed[f] == TRUE )
                        {
                            // Add this mode to the device's list of valid modes
                            pDevice->modes[pDevice->dwNumModes].Width      = modes[m].Width;
                            pDevice->modes[pDevice->dwNumModes].Height     = modes[m].Height;
                            pDevice->modes[pDevice->dwNumModes].Format     = modes[m].Format;
                            pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
                            pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
                            pDevice->dwNumModes++;

                            if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                                bHALIsSampleCompatible = TRUE;
                        }
                    }
                }
            }

            // Select any 640x480 mode for default (but prefer a 16-bit mode)
            for( m=0; m<pDevice->dwNumModes; m++ )
            {
                if( pDevice->modes[m].Width==640 && pDevice->modes[m].Height==480 )
                {
                    pDevice->dwCurrentMode = m;
                    if( pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
                        pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
                        pDevice->modes[m].Format == D3DFMT_A1R5G5B5 )
                    {
                        break;
                    }
                }
            }

            // Check if the device is compatible with the desktop display mode
            // (which was added initially as formats[0])
            //if( bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) )
            if( bFormatConfirmed[0] )
			{
                pDevice->bCanDoWindowed = TRUE;
                pDevice->bWindowed      = TRUE;
            }

            // If valid modes were found, keep this device
            if( pDevice->dwNumModes > 0 )
                pAdapter->dwNumDevices++;
        }

        // If valid devices were found, keep this adapter
        if( pAdapter->dwNumDevices > 0 )
            m_dwNumAdapters++;
    }

    // Return an error if no compatible devices were found
    if( 0L == m_dwNumAdapters )
        return D3DAPPERR_NOCOMPATIBLEDEVICES;

    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
    for( DWORD a=0; a<m_dwNumAdapters; a++ )
    {
        for( DWORD d=0; d < m_Adapters[a].dwNumDevices; d++ )
        {
            if( m_Adapters[a].devices[d].bWindowed )
            {
                m_Adapters[a].dwCurrentDevice = d;
                m_dwAdapter = a;
                m_bWindowed = TRUE;

                // Display a warning message
                if( m_Adapters[a].devices[d].DeviceType == D3DDEVTYPE_REF )
                {
                    if( !bHALExists )
                        DisplayErrorMsg( D3DAPPERR_NOHARDWAREDEVICE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsSampleCompatible )
                        DisplayErrorMsg( D3DAPPERR_HALNOTCOMPATIBLE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsWindowedCompatible )
                        DisplayErrorMsg( D3DAPPERR_NOWINDOWEDHAL, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsDesktopCompatible )
                        DisplayErrorMsg( D3DAPPERR_NODESKTOPHAL, MSGWARN_SWITCHEDTOREF );
                    else // HAL is desktop compatible, but not sample compatible
                        DisplayErrorMsg( D3DAPPERR_NOHALTHISMODE, MSGWARN_SWITCHEDTOREF );
                }

                return S_OK;
            }
        }
    }

    return D3DAPPERR_NOWINDOWABLEDEVICES;
}




//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL CMyD3DApplication::FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
    D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat )
{
    if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D16;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24S8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X4S4;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D32;
                return TRUE;
            }
        }
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
	
	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_bFrameMoving )
		DXUtil_Timer( TIMER_STOP );

	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DbgOut( "checksize : viewer : OnSize : uMsg %d, wParam %d, lParam %d, MAXHIDE %d, MINIMIZED %d\r\n",
		uMsg, wParam, lParam, SIZE_MAXHIDE, SIZE_MINIMIZED );

	if( wParam == SIZE_MINIMIZED ){
		m_miniflag = 1;
	}


    if( (SIZE_MAXHIDE==wParam) || (SIZE_MINIMIZED==wParam) )
        m_bActive = FALSE;
    else
        m_bActive = TRUE;

	if( g_hStatus && ::IsWindow( g_hStatus ) ){
		SendMessage( g_hStatus, WM_SIZE, wParam, lParam );
	}
	if( m_graphwnd && g_inidata->graph_disp ){
		BOOL dummy;
		m_graphwnd->OnSizeDlg( WM_MOVE, 0, 0, dummy );
	}


	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_graphwnd && g_inidata->graph_disp ){
		BOOL dummy;
		m_graphwnd->OnSizeDlg( WM_MOVE, 0, 0, dummy );
	}

	return DefWindowProc();
}


LRESULT CMyD3DApplication::OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HRESULT hr;
    if( m_bFrameMoving )
        DXUtil_Timer( TIMER_START );

	if( g_motdlg )
		g_motdlg->AxisSetInvalidateFlag( 1 );//!!!!!!!!!!!


    if( m_bActive && m_bWindowed )
    {
        RECT rcClientOld;
        rcClientOld = m_rcWindowClient;

        // Update window properties
        ::GetWindowRect( m_hWnd, &m_rcWindowBounds );
        ::GetClientRect( m_hWnd, &m_rcWindowClient );

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//clientエリアが０になると、サーフェスの作成がエラーになるので、小さくなりすぎないようにする。
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		int extwidth = 0;
		int extheight = 0;
		//if( (m_rcWindowClient.right - m_rcWindowClient.left) < 350 ){
//		if( (m_rcWindowClient.right - m_rcWindowClient.left) < 550 ){
//			extwidth = 1;
//		}

//		int graphwidth;
//		if( g_inidata->graph_disp ){
//			graphwidth = 250;
//		}else{
//			graphwidth = 0;
//		}
//		int diffw = 0;
//		diffw = (m_rcsigedit.right + graphwidth) - ( m_rcWindowClient.right - m_rcWindowClient.left );
//		if( diffw > 150 ){
//			extwidth = 0;
//		}else{
//			extwidth = 150 - diffw;
//		}

		if( (m_rcWindowClient.right - m_rcWindowClient.left) < 570 ){
			extwidth = 1;
		}

		if( (m_rcWindowClient.bottom - m_rcWindowClient.top) < 300 ){
			extheight = 1;
		}
		
		if( (extwidth != 0) || (extheight == 1) ){
			WINDOWPLACEMENT place;
			place.length = sizeof( WINDOWPLACEMENT );
			::GetWindowPlacement( m_hWnd, &place );


			int diffwidth, diffheight;
			diffwidth = (m_rcWindowBounds.right - m_rcWindowBounds.left) - (m_rcWindowClient.right - m_rcWindowClient.left);
			diffheight = (m_rcWindowBounds.bottom - m_rcWindowBounds.top) - (m_rcWindowClient.bottom - m_rcWindowClient.top);

			if( extwidth == 1 ){
				//place.rcNormalPosition.right = place.rcNormalPosition.left + diffwidth + 350;
				place.rcNormalPosition.right = place.rcNormalPosition.left + diffwidth + 570;
				//place.rcNormalPosition.right += extwidth;
			}
			if( extheight == 1 ){
				place.rcNormalPosition.bottom = place.rcNormalPosition.top + diffheight + 300;
			}

			::SetWindowPlacement( m_hWnd, &place );


			::GetWindowRect( m_hWnd, &m_rcWindowBounds );
			::GetClientRect( m_hWnd, &m_rcWindowClient );
		}


//		::SendMessage( g_hToolBar, TB_AUTOSIZE, 0, 0 );


//        if( (rcClientOld.right - rcClientOld.left !=
//            m_rcWindowClient.right - m_rcWindowClient.left) ||
//            (rcClientOld.bottom - rcClientOld.top !=
//            m_rcWindowClient.bottom - m_rcWindowClient.top) )
//        {

		if( m_bReady && ::IsWindow( m_hWnd ) ){		
		
			// A new window size will require a new backbuffer
            // size, so the 3D structures must be changed accordingly.
            m_bReady = FALSE;

			ComputeRect( 1, 0 );
			hr = Resize3DEnvironment();
			if( hr != D3D_OK ){
				DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
				return 0;
			}

//            m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
//            m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

//            // Resize the 3D environment
//            if( FAILED( hr = Resize3DEnvironment() ) )
//            {
//                DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
//                return 0;
//            }

            m_bReady = TRUE;
		}

	}

	if( g_motdlg )
		g_motdlg->AxisSetInvalidateFlag( 0 );//!!!!!!!!!!!

	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( m_bActive && m_bReady && !m_bWindowed )
    {
        SetCursor( NULL );
        if( m_bShowCursorWhenFullscreen )
            m_pd3dDevice->ShowCursor( TRUE );
        return TRUE; // prevent Windows from setting cursor to window class cursor
    }

	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( m_bActive && m_bReady && m_pd3dDevice != NULL )
    {
        POINT ptCursor;
        GetCursorPos( &ptCursor );
        ::ScreenToClient( m_hWnd, &ptCursor );
        m_pd3dDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0L );
    }

	m_wheeldelta = 0.0f;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	return DefWindowProc();
}
LRESULT CMyD3DApplication::OnEnterMenuLoop(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( g_motdlg && (m_intormenu == 0) ){
	    g_motdlg->OnStop();
		g_motdlg->VIKStopVIK();
    }

	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( !m_bWindowed )
        return HTCLIENT;

	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnPowerBroadcast(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    switch( wParam )
    {
        #ifndef PBT_APMQUERYSUSPEND
            #define PBT_APMQUERYSUSPEND 0x0000
        #endif
        case PBT_APMQUERYSUSPEND:
            // At this point, the app should save any data for open
            // network connections, files, etc., and prepare to go into
            // a suspended mode.
            return TRUE;

        #ifndef PBT_APMRESUMESUSPEND
            #define PBT_APMRESUMESUSPEND 0x0007
        #endif
        case PBT_APMRESUMESUSPEND:
            // At this point, the app should recover any data, network
            // connections, files, etc., and resume running from when
            // the app was suspended.
            return TRUE;
    }
	return DefWindowProc();
}
LRESULT CMyD3DApplication::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	DbgOut( "checksize : OnSysCommand : uMsg %d, wParam %d, lParam %d\r\n", uMsg, wParam, lParam );
//	DbgOut( "MOVE %d, SIZE %d, MAXIMIZE %d, KEYMENU %d, MONITORPOWER %d, SCREENSAVE %d\r\n",
//		SC_MOVE, SC_SIZE, SC_MAXIMIZE, SC_KEYMENU, SC_MONITORPOWER, SC_SCREENSAVE );

	HRESULT hr;

    // Prevent moving/sizing and power loss in fullscreen mode
    switch( wParam )
    {
        case SC_MOVE:
        case SC_SIZE:
        case SC_KEYMENU:
        case SC_MONITORPOWER:
            if( FALSE == m_bWindowed )
                return 1;
            break;
		case SC_SCREENSAVE:
			DbgOut( "D3DApp : SC_SCREENSAVE %d %d\n", m_bActive, m_bReady );
			m_screensave = 1;
			break;



        case SC_MAXIMIZE:
		case SC_RESTORE:
            if( FALSE == m_bWindowed )
                return 1;
			
			DefWindowProc();

            m_bReady = FALSE;
			ComputeRect( 1, 0 );
			hr = Resize3DEnvironment();
			if( hr != D3D_OK ){
				DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
				return 0;
			}
            m_bReady = TRUE;
			return 0;

			break;
    }
	return DefWindowProc();
}

//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Initialize3DEnvironment()
{


    HRESULT hr;

    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Prepare window for possible windowed/fullscreen change
    AdjustWindowForChange();


    // Set up the presentation parameters
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
m_d3dpp.BackBufferCount        = 1;
//m_d3dpp.BackBufferCount        = 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!手動 

	//m_d3dpp.MultiSampleType        = pDeviceInfo->MultiSampleType;
	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;

    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
m_d3dpp.EnableAutoDepthStencil = m_bUseDepthBuffer;
//m_d3dpp.EnableAutoDepthStencil = FALSE;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!手動

    m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
//    m_d3dpp.hDeviceWindow          = m_hWnd;
    m_d3dpp.hDeviceWindow          = m_3dwnd.m_hWnd;


	//!!!!!!!!!!!!!!!!!!!!!!!
	// バックバッファをロックするために必要なフラグ。
	//!!!!!!!!!!!!!!!!!!!!!!!
	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;


//    if( m_bWindowed )
//    {
//        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
//        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferWidth  = m_rc3dwnd.right - m_rc3dwnd.left;
        m_d3dpp.BackBufferHeight = m_rc3dwnd.bottom - m_rc3dwnd.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
//    }
//    else
//    {
//        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
//        m_d3dpp.BackBufferHeight = pModeInfo->Height;
//        m_d3dpp.BackBufferFormat = pModeInfo->Format;
//    }


m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


    // Create the device



// multithread

//    hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
//                               m_hWndFocus, pModeInfo->dwBehavior | D3DCREATE_MULTITHREADED, &m_d3dpp,
//                               &m_pd3dDevice );    
    hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
                               m_3dwnd.m_hWnd, pModeInfo->dwBehavior | D3DCREATE_MULTITHREADED, &m_d3dpp,
                               &m_pd3dDevice );    
	if( SUCCEEDED(hr) )
    {
        // When moving from fullscreen to windowed mode, it is important to
        // adjust the window size after recreating the device rather than
        // beforehand to ensure that you get the window size you want.  For
        // example, when switching from 640x480 fullscreen to windowed with
        // a 1000x600 window on a 1024x768 desktop, it is impossible to set
        // the window size to 1000x600 until after the display mode has
        // changed to 1024x768, because windows cannot be larger than the
        // desktop.
		if( m_bWindowed )
		{
//			::SetWindowPos( m_hWnd, HWND_NOTOPMOST,
//						  //m_rcWindowBounds.left, m_rcWindowBounds.top,
//						  g_inidata->main_posx, g_inidata->main_posy,
//						  g_inidata->main_width,
//						  g_inidata->main_height,
//						  SWP_NOSIZE | SWP_NOZORDER );
//						  //SWP_SHOWWINDOW );


	  }

        // Store device Caps
        m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );


		DbgOut( "d3dapp : Initialize3DEnvironment : MaxVertexShaderConst %d\r\n", m_d3dCaps.MaxVertexShaderConst );


		LARGE_INTEGER tmpcnt;
		QueryPerformanceCounter( &tmpcnt );
		if( tmpcnt.QuadPart == 0 ){
			::MessageBox( m_hWnd, "マシンが高精度タイマーに対応していないため\nこのソフトは実行できません。", "終了します", MB_OK );
			return -1;
		}

		if( m_d3dCaps.MaxStreams < 3 ){
			::MessageBox( m_hWnd, "ビデオカードがマルチストリームに対応していないため、\nこのソフトは実行できません。", "終了します", MB_OK );
			return -1;
		}
		if( g_index32bit != 0 ){
			if( m_d3dCaps.MaxVertexIndex <= 0x0000FFFF ){
				::MessageBox( m_hWnd, "ビデオカードが32bitインデックスに対応していないため、\n32bitインデックスモードでは実行できません。", "終了します", MB_OK );
				return -1;
			}
		}

		if( m_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0) ){
			g_bumpEnable = 1;
			g_shadowEnable = 1;
			g_usePS = 1;
		}else{
			::MessageBox( m_hWnd, "ビデオカードにピクセルシェーダー2.0以降がないため\nこのソフトは実行できません。", "終了します", MB_OK );
			g_bumpEnable = 0;
			g_shadowEnable = 0;
			g_usePS = 0;
			return -1;//!!!!!!!!!!!!!!!!!!!!!!
		}
		g_useGPU = 1;
		MAXSKINMATRIX = 42;

		DbgOut( "d3dapp : check Bump Enable %d : VS %d, PS %d, streams %d\r\n",
			g_bumpEnable, m_d3dCaps.VertexShaderVersion, m_d3dCaps.PixelShaderVersion, m_d3dCaps.MaxStreams );


		if( (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0)) && (m_d3dCaps.MaxVertexShaderConst >= 256) ){
			DbgOut( "d3dapp : HW Vertex Pixel Processing enable" );
			g_chkVS = 20;
		}else{
			DbgOut( "d3dapp : SW Vertex Processing, HW Pixel Processing" );
			::MessageBox( m_hWnd, "ビデオカードに頂点シェーダー２以降がありません。\nソフトウェア頂点処理します。\n描画速度は遅くなります。", "確認", MB_OK );

			m_pd3dDevice->Release();

			hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
							   m_hWndFocus, 
							   //pModeInfo->dwBehavior | D3DCREATE_MULTITHREADED, 
							   //D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
							   &m_d3dpp,
							   &m_pd3dDevice );    
			if( hr != D3D_OK ){
				DbgOut( "d3dapp : Initialize3DEnvironment : CreateDevice SoftWare error !!!\n" );
				_ASSERT( 0 );
				return hr;//!!!!!!!!!!!!!!!!!!!!!!
			}
			g_chkVS = 0;
		}
		

		g_bumpUse = g_bumpEnable;//!!!!!!!!!!!!
		g_shadowUse = g_shadowEnable;

		DbgOut( "Initialize3DEnvironment : MaxStreamStride %d\r\n", m_d3dCaps.MaxStreamStride ); 

		DbgOut( "d3dapp : check texture caps : MaxTextureBlendStages %d, MaxSimultaneousTextures %d\r\n",
			m_d3dCaps.MaxTextureBlendStages, m_d3dCaps.MaxSimultaneousTextures );


        m_dwCreateFlags = pModeInfo->dwBehavior;

        // Store device description
        if( pDeviceInfo->DeviceType == D3DDEVTYPE_REF )
            lstrcpy( m_strDeviceStats, TEXT("REF") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
            lstrcpy( m_strDeviceStats, TEXT("HAL") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_SW )
            lstrcpy( m_strDeviceStats, TEXT("SW") );

        if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
            pModeInfo->dwBehavior & D3DCREATE_PUREDEVICE )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
        {
            lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
        }

        if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
        {
            lstrcat( m_strDeviceStats, TEXT(": ") );
            lstrcat( m_strDeviceStats, (LPCTSTR)pAdapterInfo->d3dAdapterIdentifier.Description );
        }
///////////////////////////////
		/***
		//バックバッファとＺばっふぁの作成
		if( !m_pBackBuffer ){
			hr = m_pd3dDevice->CreateRenderTarget( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 
				pModeInfo->Format, m_d3dpp.MultiSampleType, TRUE, &m_pBackBuffer );
			if( hr != D3D_OK ){
				DbgOut( "d3dapp : Initialize3DEnvironment : dev CreateRenderTarget error %x\r\n", hr );
				_ASSERT( 0 );
				return hr;
			}
		}

		if( !m_pZSurf ){
			D3DFORMAT fmt;
			fmt = D3DFMT_D16;	
			hr = m_pd3dDevice->CreateDepthStencilSurface( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 
				fmt, m_d3dpp.MultiSampleType, &m_pZSurf );
			if( hr != D3D_OK ){
				DbgOut( "d3dapp : Initialize3DEnvironment : dev CreateDepthStencilSurface error %x !!!\n", hr );
				_ASSERT( 0 );
				return hr;
			}
		}
		***/

        // Store render target surface desc
        LPDIRECT3DSURFACE9 pBackBuffer;
        m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
        pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
        pBackBuffer->Release();

///////!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		hr = m_pd3dDevice->GetDepthStencilSurface( &m_pZSurf );
		if( hr != D3D_OK ){
			DbgOut( "d3dapp : Initialize3DEnvironment : dev GetDepthStencilSurface error !!!\n" );
			_ASSERT( 0 );
			return hr;
		}
		

		hr = m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
		if( hr != D3D_OK ){
			DbgOut( "d3dapp : Initialize3DEnvironment : dev GetBackBuffer error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		
		
		
		// Set up the fullscreen cursor
        if( m_bShowCursorWhenFullscreen && !m_bWindowed )
        {
            HCURSOR hCursor;
#ifdef _WIN64
            hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
            hCursor = (HCURSOR)GetClassLong( m_hWnd, GCL_HCURSOR );
#endif

            D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor );
            m_pd3dDevice->ShowCursor( TRUE );
        }

        // Initialize the app's device-dependent objects
        hr = InitDeviceObjects();
        if( SUCCEEDED(hr) )
        {
            hr = RestoreDeviceObjects( 1 );
            if( SUCCEEDED(hr) )
            {
                m_bActive = TRUE;
                return S_OK;
            }
        }

        // Cleanup before we try again
        InvalidateDeviceObjects( 0, 1 );
        DeleteDeviceObjects();
        SAFE_RELEASE( m_pd3dDevice );
    }

    // If that failed, fall back to the reference rasterizer
    if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
    {

		::MessageBox( m_hWnd, "ビデオカードにハードウェアアクセラレータがないため、\nこのソフトは実行できません。", "終了します", MB_OK );
		return -1;

    }

    return hr;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Resize3DEnvironment()
{
	m_resizeflag = 1;

	DbgOut( "d3dapp : Resize3DEnvironment\r\n" );
	
    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];


	HRESULT hr;

    // Release all vidmem objects
	int resetflag = 1;
    if( FAILED( hr = InvalidateDeviceObjects( resetflag, 0 ) ) )
        return hr;


/// 作り直す。
	if( m_pZSurf ){
		m_pZSurf->Release();
		m_pZSurf = 0;
	}
	if( m_pBackBuffer ){
		m_pBackBuffer->Release();
		m_pBackBuffer = 0;
	}

//	if( g_pEffect ){
		//g_pEffect->Release();
		//g_pEffect = 0;
//	}


    // Reset the device
	if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) ){
		DbgOut( "d3dapp : Resize3DEnvironment : dev Reset error %x, (%x %x %x)\r\n", hr, D3DERR_INVALIDCALL, D3DERR_OUTOFVIDEOMEMORY, E_OUTOFMEMORY );
		_ASSERT( 0 );
        return hr;
	}


	hr = m_pd3dDevice->GetDepthStencilSurface( &m_pZSurf );
	if( hr != D3D_OK ){
		DbgOut( "d3dapp : Initialize3DEnvironment : dev GetDepthStencilSurface error !!!\n" );
		_ASSERT( 0 );
		return hr;
	}
		

	hr = m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	if( hr != D3D_OK ){
		DbgOut( "d3dapp : Initialize3DEnvironment : dev GetBackBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


    // Set up the fullscreen cursor
    if( m_bShowCursorWhenFullscreen && !m_bWindowed )
    {
        HCURSOR hCursor;
#ifdef _WIN64
        hCursor = (HCURSOR)(GetClassLongPtr( m_hWnd, GCLP_HCURSOR ));
#else
        hCursor = (HCURSOR)(GetClassLong( m_hWnd, GCL_HCURSOR ));
#endif
        D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor );
        m_pd3dDevice->ShowCursor( TRUE );
    }

    // Initialize the app's device-dependent objects
    hr = RestoreDeviceObjects( 0, resetflag );
	if( FAILED(hr) ){
		_ASSERT( 0 );
        return hr;
	}

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

	if( g_hToolBar && ::IsWindow( g_hToolBar ) ){
		::SendMessage( g_hToolBar, TB_AUTOSIZE, 0, 0 );
	}

	m_resizeflag = 0;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ToggleFullscreen()
{
    // Get access to current adapter, device, and mode
    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Need device change if going windowed and the current device
    // can only be fullscreen
    if( !m_bWindowed && !pDeviceInfo->bCanDoWindowed )
        return ForceWindowed();

    m_bReady = FALSE;

    // Toggle the windowed state
    m_bWindowed = !m_bWindowed;
    pDeviceInfo->bWindowed = m_bWindowed;

    // Prepare window for windowed/fullscreen change
    AdjustWindowForChange();

    // Set up the presentation parameters
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
    
	//m_d3dpp.MultiSampleType        = pDeviceInfo->MultiSampleType;
	m_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
    
	m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
//    m_d3dpp.hDeviceWindow          = m_hWnd;
	m_d3dpp.hDeviceWindow          = m_3dwnd.m_hWnd;
    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = m_rc3dwnd.right - m_rc3dwnd.left;
        m_d3dpp.BackBufferHeight = m_rc3dwnd.bottom - m_rc3dwnd.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
    }

    // Resize the 3D device
    if( FAILED( Resize3DEnvironment() ) )
    {
        if( m_bWindowed )
            return ForceWindowed();
        else
            return E_FAIL;
    }

    // When moving from fullscreen to windowed mode, it is important to
    // adjust the window size after resetting the device rather than
    // beforehand to ensure that you get the window size you want.  For
    // example, when switching from 640x480 fullscreen to windowed with
    // a 1000x600 window on a 1024x768 desktop, it is impossible to set
    // the window size to 1000x600 until after the display mode has
    // changed to 1024x768, because windows cannot be larger than the
    // desktop.
    if( m_bWindowed )
    {
//        ::SetWindowPos( m_hWnd, HWND_NOTOPMOST,
//                      m_rcWindowBounds.left, m_rcWindowBounds.top,
//                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
//                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
//                      SWP_SHOWWINDOW );
//			::SetWindowPos( m_hWnd, HWND_NOTOPMOST,
//						  //m_rcWindowBounds.left, m_rcWindowBounds.top,
//						  g_inidata->main_posx, g_inidata->main_posy,
//						  g_inidata->main_width,
//						  g_inidata->main_height,
//						  SWP_NOZORDER );
//						  //SWP_SHOWWINDOW );

	}

    m_bReady = TRUE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ForceWindowed()
{
    HRESULT hr;
    D3DAdapterInfo* pAdapterInfoCur = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfoCur  = &pAdapterInfoCur->devices[pAdapterInfoCur->dwCurrentDevice];
    BOOL bFoundDevice = FALSE;

    if( pDeviceInfoCur->bCanDoWindowed )
    {
        bFoundDevice = TRUE;
    }
    else
    {
        // Look for a windowable device on any adapter
        D3DAdapterInfo* pAdapterInfo;
        DWORD dwAdapter;
        D3DDeviceInfo* pDeviceInfo;
        DWORD dwDevice;
        for( dwAdapter = 0; dwAdapter < m_dwNumAdapters; dwAdapter++ )
        {
            pAdapterInfo = &m_Adapters[dwAdapter];
            for( dwDevice = 0; dwDevice < pAdapterInfo->dwNumDevices; dwDevice++ )
            {
                pDeviceInfo = &pAdapterInfo->devices[dwDevice];
                if( pDeviceInfo->bCanDoWindowed )
                {
                    m_dwAdapter = dwAdapter;
                    pDeviceInfoCur = pDeviceInfo;
                    pAdapterInfo->dwCurrentDevice = dwDevice;
                    bFoundDevice = TRUE;
                    break;
                }
            }
            if( bFoundDevice )
                break;
        }
    }

    if( !bFoundDevice )
        return E_FAIL;

    pDeviceInfoCur->bWindowed = TRUE;
    m_bWindowed = TRUE;

    // Now destroy the current 3D device objects, then reinitialize

    m_bReady = FALSE;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects( 1, 0 );
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Create the new device
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    m_bReady = TRUE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Prepare the window for a possible change between windowed mode and
//       fullscreen mode.  This function is virtual and thus can be overridden
//       to provide different behavior, such as switching to an entirely
//       different window for fullscreen mode (as in the MFC sample apps).
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        // Set windowed-mode style
        ::SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle );
    }
    else
    {
        // Set fullscreen-mode style
        ::SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::UserSelectNewDevice()
{
    HRESULT hr;

    // Can't display dialogs in fullscreen mode
    if( m_bWindowed == FALSE )
    {
        if( FAILED( ToggleFullscreen() ) )
        {
            DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
            return E_FAIL;
        }
    }

    // Prompt the user to change the mode
    if( IDOK != DialogBoxParam( (HINSTANCE)GetModuleHandle(NULL),
                                MAKEINTRESOURCE(IDD_SELECTDEVICE), m_hWnd,
                                SelectDeviceProc, (LPARAM)this ) )
        return S_OK;

    // Get access to the newly selected adapter, device, and mode
    DWORD dwDevice;
    dwDevice  = m_Adapters[m_dwAdapter].dwCurrentDevice;
    m_bWindowed = m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects( 1, 0 );
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Inform the display class of the change. It will internally
    // re-create valid surfaces, a d3ddevice, etc.
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SelectDeviceProc()
// Desc: Windows message handling function for the device select dialog
//-----------------------------------------------------------------------------
INT_PTR CALLBACK CMyD3DApplication::SelectDeviceProc( HWND hDlg, UINT msg,
                                                    WPARAM wParam, LPARAM lParam )
{
    // Get access to the UI controls
    HWND hwndAdapterList        = ::GetDlgItem( hDlg, IDC_ADAPTER_COMBO );
    HWND hwndDeviceList         = ::GetDlgItem( hDlg, IDC_DEVICE_COMBO );
    HWND hwndFullscreenModeList = ::GetDlgItem( hDlg, IDC_FULLSCREENMODES_COMBO );
    HWND hwndWindowedRadio      = ::GetDlgItem( hDlg, IDC_WINDOW );
    HWND hwndFullscreenRadio    = ::GetDlgItem( hDlg, IDC_FULLSCREEN );
    HWND hwndMultiSampleList    = ::GetDlgItem( hDlg, IDC_MULTISAMPLE_COMBO );
    BOOL bUpdateDlgControls     = FALSE;

    // Static state for adapter/device/mode selection
    static CMyD3DApplication* pd3dApp;
    static DWORD  dwOldAdapter, dwNewAdapter;
    static DWORD  dwOldDevice,  dwNewDevice;
    static DWORD  dwOldMode,    dwNewMode;
    static BOOL   bOldWindowed, bNewWindowed;
    static D3DMULTISAMPLE_TYPE OldMultiSampleType, NewMultiSampleType;

    // Working variables
    D3DAdapterInfo* pAdapter;
    D3DDeviceInfo*  pDevice;

    // Handle the initialization message
    if( WM_INITDIALOG == msg )
    {
        // Old state
        pd3dApp      = (CMyD3DApplication*)lParam;
        dwOldAdapter = pd3dApp->m_dwAdapter;
        pAdapter     = &pd3dApp->m_Adapters[dwOldAdapter];

        dwOldDevice  = pAdapter->dwCurrentDevice;
        pDevice      = &pAdapter->devices[dwOldDevice];

        dwOldMode    = pDevice->dwCurrentMode;
        bOldWindowed = pDevice->bWindowed;
        OldMultiSampleType = pDevice->MultiSampleType;

        // New state is initially the same as the old state
        dwNewAdapter = dwOldAdapter;
        dwNewDevice  = dwOldDevice;
        dwNewMode    = dwOldMode;
        bNewWindowed = bOldWindowed;
        NewMultiSampleType = OldMultiSampleType;

        // Set flag to update dialog controls below
        bUpdateDlgControls = TRUE;
    }

    if( WM_COMMAND == msg )
    {
        // Get current UI state
        bNewWindowed  = Button_GetCheck( hwndWindowedRadio );

        if( IDOK == LOWORD(wParam) )
        {
            // Handle the case when the user hits the OK button. Check if any
            // of the options were changed
            if( dwNewAdapter != dwOldAdapter || dwNewDevice  != dwOldDevice  ||
                dwNewMode    != dwOldMode    || bNewWindowed != bOldWindowed ||
                NewMultiSampleType != OldMultiSampleType )
            {
                pd3dApp->m_dwAdapter = dwNewAdapter;

                pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
                pAdapter->dwCurrentDevice = dwNewDevice;

                pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
                pAdapter->devices[dwNewDevice].bWindowed     = bNewWindowed;
                pAdapter->devices[dwNewDevice].MultiSampleType = NewMultiSampleType;

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
        else if( CBN_SELENDOK == HIWORD(wParam) )
        {
            if( LOWORD(wParam) == IDC_ADAPTER_COMBO )
            {
                dwNewAdapter = ComboBox_GetCurSel( hwndAdapterList );
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = pAdapter->dwCurrentDevice;
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_DEVICE_COMBO )
            {
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = ComboBox_GetCurSel( hwndDeviceList );
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_FULLSCREENMODES_COMBO )
            {
                dwNewMode = ComboBox_GetCurSel( hwndFullscreenModeList );
            }
            else if( LOWORD(wParam) == IDC_MULTISAMPLE_COMBO )
            {
                DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
                NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
            }
        }
        // Keep the UI current
        bUpdateDlgControls = TRUE;
    }

    // Update the dialog controls
    if( bUpdateDlgControls )
    {
        // Reset the content in each of the combo boxes
        ComboBox_ResetContent( hwndAdapterList );
        ComboBox_ResetContent( hwndDeviceList );
        ComboBox_ResetContent( hwndFullscreenModeList );
        ComboBox_ResetContent( hwndMultiSampleList );

        pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
        pDevice  = &pAdapter->devices[dwNewDevice];

        // Add a list of adapters to the adapter combo box
        for( DWORD a=0; a < pd3dApp->m_dwNumAdapters; a++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndAdapterList,
                             pd3dApp->m_Adapters[a].d3dAdapterIdentifier.Description );

            // Set the item data to identify this adapter
            ComboBox_SetItemData( hwndAdapterList, dwItem, a );

            // Set the combobox selection on the current adapater
            if( a == dwNewAdapter )
                ComboBox_SetCurSel( hwndAdapterList, dwItem );
        }

        // Add a list of devices to the device combo box
        for( DWORD d=0; d < pAdapter->dwNumDevices; d++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndDeviceList,
                                               pAdapter->devices[d].strDesc );

            // Set the item data to identify this device
            ComboBox_SetItemData( hwndDeviceList, dwItem, d );

            // Set the combobox selection on the current device
            if( d == dwNewDevice )
                ComboBox_SetCurSel( hwndDeviceList, dwItem );
        }

        // Add a list of modes to the mode combo box
		DWORD m;
        for( m=0; m < pDevice->dwNumModes; m++ )
        {
            DWORD BitDepth = 16;
            if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_R8G8B8 )
            {
                BitDepth = 32;
            }

            // Add mode desc to the combo box
            TCHAR strMode[80];
            _stprintf_s( strMode, 80, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
                                                       pDevice->modes[m].Height,
                                                       BitDepth );
            DWORD dwItem = ComboBox_AddString( hwndFullscreenModeList, strMode );

            // Set the item data to identify this mode
            ComboBox_SetItemData( hwndFullscreenModeList, dwItem, m );

            // Set the combobox selection on the current mode
            if( m == dwNewMode )
                ComboBox_SetCurSel( hwndFullscreenModeList, dwItem );
        }

        // Add a list of multisample modes to the multisample combo box
        for( m=0; m <= 16; m++ )
        {
            TCHAR strDesc[50];

            D3DFORMAT fmt;
            if( bNewWindowed )
                fmt = pd3dApp->m_Adapters[dwNewAdapter].d3ddmDesktop.Format;
            else
                fmt = pDevice->modes[dwNewMode].Format;

            if ( m == 1 ) // 1 is not a valid multisample type
                continue;

			DWORD multisampleQ = 0;
            if( SUCCEEDED( pd3dApp->m_pD3D->CheckDeviceMultiSampleType( dwNewAdapter,
                pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m, &multisampleQ ) ) )
            {
                if( (m == 0) || (multisampleQ == 0) )
                    lstrcpy( strDesc, _T("none") );
                else
                    wsprintf( strDesc, _T("%d samples"), m );

                // Add device name to the combo box
                DWORD dwItem = ComboBox_AddString( hwndMultiSampleList, strDesc );

                // Set the item data to identify this multisample type
                ComboBox_SetItemData( hwndMultiSampleList, dwItem, m );

                // Set the combobox selection on the current multisample type
                if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleType || m == 0 )
                    ComboBox_SetCurSel( hwndMultiSampleList, dwItem );
            }
        }
        DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
        NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
        ::EnableWindow( hwndMultiSampleList, ComboBox_GetCount( hwndMultiSampleList ) > 1);
        ::EnableWindow( hwndWindowedRadio, pDevice->bCanDoWindowed );

        if( bNewWindowed )
        {
            Button_SetCheck( hwndWindowedRadio,   TRUE );
            Button_SetCheck( hwndFullscreenRadio, FALSE );
            ::EnableWindow( hwndFullscreenModeList, FALSE );
        }
        else
        {
            Button_SetCheck( hwndWindowedRadio,   FALSE );
            Button_SetCheck( hwndFullscreenRadio, TRUE );
            ::EnableWindow( hwndFullscreenModeList, TRUE );
        }
        return TRUE;
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: Run()
// Desc:
//-----------------------------------------------------------------------------
INT CMyD3DApplication::Run()
{
    // Load keyboard accelerators
    HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_ACCELERATOR1) );

	int ret;
	ret = CMotionInfo::CreateTempMP();
	_ASSERT( !ret );

    // Now we're ready to recieve and process Windows messages.
    BOOL bGotMsg;
    MSG  msg;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

    while( WM_QUIT != msg.message  )
    {
        // Use PeekMessage() if the app is active, so we can use idle time to
        // render the scene. Else, use GetMessage() to avoid eating CPU time.
        if( m_bActive )
            bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
        else
            bGotMsg = GetMessage( &msg, NULL, 0U, 0U );

        if( bGotMsg )
        {
            // Translate and dispatch the message
            if( 0 == TranslateAccelerator( m_hWnd, hAccel, &msg ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        {
			if( g_endapp == 0 ){
				// Render a frame during idle time (no messages are waiting)
				if( m_bActive && m_bReady && (m_writebmpflag == 0) 
					&& ( (m_lbutton == 0) || (m_movebone <= 0) )
					&& ( (m_lbutton == 0) || (m_shiftbone <= 0) ) 
					&& ( (m_lbutton == 0) || (m_scalebone <= 0) ) 
					&& ( (m_lbutton == 0) || (m_pickgp <= 0) )
					&& !m_vikflag 
					&& (m_preview_flag == 0)
					&& (m_onselectplugin == 0) )
				{
					if( FAILED( Render3DEnvironment() ) )
						SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
					//UpdateWindow( m_hWnd );

				}
			}
        }
    }

    return (INT)msg.wParam;
}




//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render3DEnvironment( int skipflag )
{
	if( g_endapp == 1 ){
		return 0;
	}

	static int s_callflag = 0;

	if( s_callflag == 1 )
		return 0;

	s_callflag = 1;


    HRESULT hr;

    // Test the cooperative level to see if it's okay to render
    if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
    {
        // If the device was lost, do not render until we get it back
		if( D3DERR_DEVICELOST == hr ){
			s_callflag = 0;
            return S_OK;
		}

        // Check if the device needs to be resized.
        if( D3DERR_DEVICENOTRESET == hr )
        {
            // If we are windowed, read the desktop mode and use the same format for
            // the back buffer
            if( m_bWindowed )
            {
                D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
                m_pD3D->GetAdapterDisplayMode( m_dwAdapter, &pAdapterInfo->d3ddmDesktop );
                m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
            }

			if( FAILED( hr = Resize3DEnvironment() ) ){
				s_callflag = 0;
                return hr;
			}
        }
		s_callflag = 0;
        return hr;
    }

    // Get the app's time, in seconds. Skip rendering if no time elapsed

	float fAppTime        = DXUtil_Timer( TIMER_GETAPPTIME );
    float fElapsedAppTime = DXUtil_Timer( TIMER_GETELAPSEDTIME );
	//if( ( 0.0f == fElapsedAppTime ) && m_bFrameMoving ){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	s_callflag = 0;
    //    return S_OK;
	//}

    // FrameMove (animate) the scene
    if( m_bFrameMoving || m_bSingleStep )
    {
        // Store the time for the app
        m_fTime        = fAppTime;
        m_fElapsedTime = fElapsedAppTime;

        // Frame move the scene
		if( skipflag == 0 ){
			if( FAILED( hr = FrameMove() ) ){
				s_callflag = 0;
				return hr;
			}
		}

        m_bSingleStep = FALSE;
    }


//LARGE_INTEGER beftime, afttime;
//DWORD render_time_low, render_time_high;
//QueryPerformanceCounter( &beftime );


    // Render the scene as normal
	if( FAILED( hr = Render() ) ){
		s_callflag = 0;
        return hr;
	}

    // Keep track of the frame count
    {
        static float fLastTime = 0.0f;
        static DWORD dwFrames  = 0L;
        float fTime = DXUtil_Timer( TIMER_GETABSOLUTETIME );
        ++dwFrames;

        // Update the scene stats once per second
        if( fTime - fLastTime > 1.0f )
        {
            //m_fFPS    = dwFrames / (fTime - fLastTime);//viewer.cppでセット。
            fLastTime = fTime;
            dwFrames  = 0L;

            // Get adapter's current mode so we can report
            // bit depth (back buffer depth may be unknown)
            D3DDISPLAYMODE mode;
            m_pD3D->GetAdapterDisplayMode(m_dwAdapter, &mode);

            _stprintf_s( m_strFrameStats, 40, _T("%.02f fps (%dx%dx%d)"), m_fFPS,
                       m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
                       mode.Format==D3DFMT_X8R8G8B8?32:16 );
            if( m_bUseDepthBuffer )
            {
                D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
                D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
                D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

                switch( pModeInfo->DepthStencilFormat )
                {
                case D3DFMT_D16:
                    lstrcat( m_strFrameStats, _T(" (D16)") );
                    break;
                case D3DFMT_D15S1:
                    lstrcat( m_strFrameStats, _T(" (D15S1)") );
                    break;
                case D3DFMT_D24X8:
                    lstrcat( m_strFrameStats, _T(" (D24X8)") );
                    break;
                case D3DFMT_D24S8:
                    lstrcat( m_strFrameStats, _T(" (D24S8)") );
                    break;
                case D3DFMT_D24X4S4:
                    lstrcat( m_strFrameStats, _T(" (D24X4S4)") );
                    break;
                case D3DFMT_D32:
                    lstrcat( m_strFrameStats, _T(" (D32)") );
                    break;
                }
            }
        }
    }

    // Show the frame on the primary surface.
	//if( m_writebmpflag == 0 )
	//	m_pd3dDevice->Present( NULL, NULL, m_3dwnd.m_hWnd, NULL );


//QueryPerformanceCounter( &afttime );
//render_time_low = afttime.LowPart - beftime.LowPart;
//render_time_high = afttime.HighPart - beftime.HighPart;

//DbgOut( "Render3DEnv : present : high %d, low %d\n", render_time_high, render_time_low );

	s_callflag = 0;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::Pause( BOOL bPause )
{
    static DWORD dwAppPausedCount = 0L;

    dwAppPausedCount += ( bPause ? +1 : -1 );
    m_bReady          = ( dwAppPausedCount ? FALSE : TRUE );

    // Handle the first pause request (of many, nestable pause requests)
    if( bPause && ( 1 == dwAppPausedCount ) )
    {
        // Stop the scene from animating
        if( m_bFrameMoving )
            DXUtil_Timer( TIMER_STOP );
    }

    if( 0 == dwAppPausedCount )
    {
        // Restart the timers
        if( m_bFrameMoving )
            DXUtil_Timer( TIMER_START );
    }
}




//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::Cleanup3DEnvironment()
{
	//int ret;

    m_bActive = FALSE;
    m_bReady  = FALSE;

DbgOut( "viewer Cleanup3DEnv 0\r\n" );

	OnPluginClose();

DbgOut( "viewer Cleanup3DEnv 1\r\n" );

    InvalidateDeviceObjects( 0, 1 );
    DeleteDeviceObjects();

DbgOut( "viewer Cleanup3DEnv 2\r\n" );

	if( m_pZSurf ){
		m_pZSurf->Release();
		m_pZSurf = 0;
	}
	if( m_pBackBuffer ){
		m_pBackBuffer->Release();
		m_pBackBuffer = 0;
	}

	if( m_effectdecl ){
		m_effectdecl->Release();
		m_effectdecl = 0;
	}
	if( m_tlvdecl ){
		m_tlvdecl->Release();
		m_tlvdecl = 0;
	}
DbgOut( "viewer Cleanup3DEnv 3\r\n" );

	if( g_pEffect ){
		g_pEffect->Release();
		g_pEffect = 0;
	}
	if( g_pPostEffect ){
		g_pPostEffect->Release();
		g_pPostEffect = 0;
	}

DbgOut( "viewer Cleanup3DEnv 4\r\n" );

	if( m_pd3dDevice != NULL ){
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
	if( m_pD3D != NULL ){
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	DbgOut( "viewer Cleanup3DEnv 5\r\n" );

    FinalCleanup();

	DbgOut( "viewer Cleanup3DEnv 6\r\n" );
}



//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DisplayErrorMsg( HRESULT hr, DWORD dwType, char* mesptr )
{
    TCHAR strMsg[512];

    switch( hr )
    {
        case D3DAPPERR_NODIRECT3D:
            _tcscpy_s( strMsg, 512, _T("Could not initialize Direct3D. You may\n")
                             _T("want to check that the latest version of\n")
                             _T("DirectX is correctly installed on your\n")
                             _T("system.  Also make sure that this program\n")
                             _T("was compiled with header files that match\n")
                             _T("the installed DirectX DLLs.") );
            break;

        case D3DAPPERR_NOCOMPATIBLEDEVICES:
            _tcscpy_s( strMsg, 512, _T("Could not find any compatible Direct3D\n")
                             _T("devices.") );
            break;

        case D3DAPPERR_NOWINDOWABLEDEVICES:
            _tcscpy_s( strMsg, 512, _T("This sample cannot run in a desktop\n")
                             _T("window with the current display settings.\n")
                             _T("Please change your desktop settings to a\n")
                             _T("16- or 32-bit display mode and re-run this\n")
                             _T("sample.") );
            break;

        case D3DAPPERR_NOHARDWAREDEVICE:
            _tcscpy_s( strMsg, 512, _T("No hardware-accelerated Direct3D devices\n")
                             _T("were found.") );
            break;

        case D3DAPPERR_HALNOTCOMPATIBLE:
            _tcscpy_s( strMsg, 512, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator.") );
            break;

        case D3DAPPERR_NOWINDOWEDHAL:
            _tcscpy_s( strMsg, 512, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_NODESKTOPHAL:
            _tcscpy_s( strMsg, 512, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window with the current\n")
                             _T("desktop display settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_NOHALTHISMODE:
            _tcscpy_s( strMsg, 512, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator with the current desktop display\n")
                             _T("settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_MEDIANOTFOUND:
            _tcscpy_s( strMsg, 512, _T("Could not load required media." ) );
            break;

        case D3DAPPERR_RESIZEFAILED:
            _tcscpy_s( strMsg, 512, _T("Could not reset the Direct3D device." ) );
            break;

        case D3DAPPERR_NONZEROREFCOUNT:
            _tcscpy_s( strMsg, 512, _T("A D3D object has a non-zero reference\n")
                             _T("count (meaning things were not properly\n")
                             _T("cleaned up).") );
            break;

        case E_OUTOFMEMORY:
            _tcscpy_s( strMsg, 512, _T("Not enough memory.") );
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
            _tcscpy_s( strMsg, 512, _T("Not enough video memory.") );
            break;

        default:
            _tcscpy_s( strMsg, 512, _T("Generic application error. Enable\n")
                             _T("debug output for detailed information.") );
    }

    if( MSGERR_APPMUSTEXIT == dwType )
    {
		if( mesptr ){
			_tcscat_s( strMsg, 512, _T("\n") );
			_tcscat_s( strMsg, 512, mesptr );
		}
        _tcscat_s( strMsg, 512, _T("\n\nThis sample will now exit.") );
        ::MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONERROR|MB_OK );

        // Close the window, which shuts down the app
        if( m_hWnd )
            SendMessage( m_hWnd, WM_CLOSE, 0, (LPARAM)dwType );
    }
    else
    {
		if( mesptr ){
			_tcscat_s( strMsg, 512, _T("\n") );
			_tcscat_s( strMsg, 512, mesptr );
		}

        if( MSGWARN_SWITCHEDTOREF == dwType )
            _tcscat_s( strMsg, 512, _T("\n\nSwitching to the reference rasterizer,\n")
                             _T("a software device that implements the entire\n")
                             _T("Direct3D feature set, but runs very slowly.") );
        ::MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONWARNING|MB_OK );
    }

    return hr;
}

int CMyD3DApplication::SetModuleDir()
{
	char filename[1024];
	ZeroMemory( filename, 1024 );
	int ret = 0;

	ret = GetModuleFileName( NULL, filename, 1024 );
	if( ret == 0 ){
		_ASSERT( 0 );
		DbgOut( "d3dapp : SetModuleDir : GetModuleFileName error !!!\n" );
		return 1;
	}
	int ch = '\\';
	char* lasten;
	lasten = strrchr( filename, ch );
	if( !lasten ){
		_ASSERT( 0 );
		DbgOut( "d3dapp : SetModuleDir : strrchr error !!!\n" );
		return 1;
	}
	int leng;
	char moduledir[1024];
	ZeroMemory( moduledir, 1024 );
	leng = (int)(lasten - filename + 1);
	strncpy_s( moduledir, 1024, filename, leng );

	::SetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPCTSTR)moduledir );
	
	DbgOut( "CMyD3DApplication : SetModuleDir : %s\n", moduledir );


	SetMediaDir();

	return 0;
}

int CMyD3DApplication::SetMediaDir()
{
	char filename[1024];
	char* endptr = 0;

	ZeroMemory( filename, 1024 );
	int ret;

	ret = GetModuleFileName( NULL, filename, 1024 );
	if( ret == 0 ){
		_ASSERT( 0 );
		DbgOut( "SetMediaDir : GetModuleFileName error !!!\n" );
		return 1;
	}

	int ch = '\\';
	char* lasten = 0;
	lasten = strrchr( filename, ch );
	if( !lasten ){
		_ASSERT( 0 );
		DbgOut( "SetMediaDir : strrchr error !!!\n" );
		return 1;
	}

	*lasten = 0;

	char* last2en = 0;
	char* last3en = 0;
	last2en = strrchr( filename, ch );
	if( last2en ){
		*last2en = 0;
		last3en = strrchr( filename, ch );
		if( last3en ){
			if( (strcmp( last2en + 1, "debug" ) == 0) ||
				(strcmp( last2en + 1, "Debug" ) == 0) ||
				(strcmp( last2en + 1, "DEBUG" ) == 0) ||
				(strcmp( last2en + 1, "release" ) == 0) ||
				(strcmp( last2en + 1, "Release" ) == 0) ||
				(strcmp( last2en + 1, "RELEASE" ) == 0)
				){

				endptr = last2en;
			}else{
				endptr = lasten;
			}
		}else{
			endptr = lasten;
		}
	}else{
		endptr = lasten;
	}

	*lasten = '\\';
	if( last2en )
		*last2en = '\\';
	if( last3en )
		*last3en = '\\';

	int leng;
	char resdir[1024];
	ZeroMemory( resdir, 1024 );
	ZeroMemory( m_autosavedir, sizeof( char ) * MAX_PATH );
	leng = (int)( endptr - filename + 1 );
	strncpy_s( resdir, 1024, filename, leng );
	strncpy_s( m_autosavedir, MAX_PATH, filename, leng );
	strcat_s( resdir, 1024, "Media\\" );
	strcat_s( m_autosavedir, MAX_PATH, "AutoSave\\" );

	::SetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPCTSTR)resdir );

	DbgOut( "SetMediaDir : %s\n", resdir );
	DbgOut( "autosavedir : %s\n", m_autosavedir );

	return 0;
}

int CMyD3DApplication::CreateAppToolBar()
{
	if( g_hToolBar != NULL )
		return 0;

	HINSTANCE hInst;
	hInst = (HINSTANCE)GetModuleHandle(NULL);

	g_hToolBar = CreateWindowEx(
			0,
			TOOLBARCLASSNAME,
			NULL,
			WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | WS_CLIPCHILDREN,
			0, 0,
			0, 0,
			m_hWnd,
			(HMENU)ID_TOOLBAR,
			hInst,
			NULL);

	SendMessage( g_hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );


	char bmpname[2048];
	int leng;
	ZeroMemory( bmpname, 2048 );
	leng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)bmpname, 2048 );
	_ASSERT( leng );
	char tbname[256];
	strcpy_s( tbname, 256, "tb27.bmp" );
	strcat_s( bmpname, 2048, tbname );



	int buttonnum = 27;//!!!!!!!!!!!!!!!!



	TBADDBITMAP tbab;
    tbab.hInst = NULL;
	m_hbmp = LoadImage( NULL, bmpname, IMAGE_BITMAP, 16 * buttonnum, 15, LR_LOADFROMFILE );
	if( m_hbmp == NULL ){
		DbgOut( "d3dapp : CreateAppToolBar : LoadImage error !!!\n" );
		char strmes[1024];
		sprintf_s( strmes, 1024, "Mediaフォルダに%sが無いかもしくは不正です。\nツールバーが作れないので終了します。", tbname );
		::MessageBox( m_hWnd, strmes, "エラー", MB_OK );
		_ASSERT( 0 );
		return 1;
	}

    tbab.nID = (UINT_PTR)m_hbmp;
    SendMessage( g_hToolBar, TB_ADDBITMAP, buttonnum, (LPARAM)&tbab );
    SendMessage( g_hToolBar, TB_ADDBUTTONS, (WPARAM)buttonnum, (LPARAM)&tbb);


	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                25, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                25, (LPARAM)&tb_space);

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                22, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                22, (LPARAM)&tb_space);

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                18, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                18, (LPARAM)&tb_space);

	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                14, (LPARAM)&tb_space);
	SendMessage(g_hToolBar, TB_INSERTBUTTON,
                14, (LPARAM)&tb_space);

    //SendMessage(g_hToolBar, TB_ENABLEBUTTON, IDTBB_L, MAKELONG(0, 0));//!!!!!!!!!!!!!!!!!


	::SendMessage( g_hToolBar, TB_AUTOSIZE, 0, 0 );

	return 0;
}



int CMyD3DApplication::DestroyAppToolBar()
{

	if( g_hToolBar != NULL ){
		::DestroyWindow( g_hToolBar );
		g_hToolBar = NULL;
	}

//	if( m_hbmp != NULL ){
//		DeleteObject( m_hbmp );
//	}

	return 0;
}

/////////////////////////
// application

//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------

HRESULT CMyD3DApplication::OneTimeSceneInit()
{

//	DbgOut( "check !!! : MAX_PATH %d, _MAX_PATH %d\r\n", MAX_PATH, _MAX_PATH );

//	HANDLE hproc;
//	hproc = GetCurrentProcess();
//	SetProcessAffinityMask( hproc, 1 );


	InitCommonControls();


	IsRegist();

	CQuaternion2 tempq;
	tempq.InitTempQ();


	int ret;
	ERRORMES errormes;

	g_cpuinfo.CheckCPU();
	DbgOut( 
		"CPU Vender: \"%s\"\n"
		"Family: %d  Model: %d  Stepping ID: %d\n"
		"Supported CPUID: %d\n"
		"Supported MMX: %d\n"
		"Supported SSE: %d\n"
		"Supported SSE2: %d\n"
		"Supported 3DNow!: %d\n"
		"Supported Enhanced 3DNow!: %d\n"
		, g_cpuinfo.vd.id
		, g_cpuinfo.dwFamily, g_cpuinfo.dwModel, g_cpuinfo.dwSteppingId
		, g_cpuinfo.bCPUID, g_cpuinfo.bMMX, g_cpuinfo.bSSE, g_cpuinfo.bSSE2, g_cpuinfo.b3DNow, g_cpuinfo.bE3DNow
	);


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

	if( !m_soundbank ){
		m_soundbank = new CSoundBank();
		if( !m_soundbank ){
			DbgOut( "OneTimeSceneInit : new SoundBank error !!!\n" );
			_ASSERT( 0 );

			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "SoundBank alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );

			return -1;
		}
	}

	if( !m_HS ){
		m_HS = new CHuSound();
		if( !m_HS ){
			DbgOut( "OneTimeSceneInit : new HuSound error !!!\n" );
			_ASSERT( 0 );

			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "HuSound alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );

			return -1;
		}

		int ret;
		ret = m_HS->Init( m_hWnd, 1 );
		if( ret ){
			DbgOut( "OneTimeSceneInit : HS Init error !!!\n" );
			_ASSERT( 0 );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "HuSound Init error";
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
	if( !m_itidlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ItiDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_itidlg->Create( m_hWnd );


//	m_cameradlg = new CCameraDlg( m_hWnd );
//	if( !m_cameradlg ){
//		errormes.errorcode = E_OUTOFMEMORY;
//		errormes.type = MSGERR_APPMUSTEXIT;
//		errormes.mesptr = "CameraDlg alloc error";
//		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
//		_ASSERT( 0 );
//		return -1;
//	}
//	m_cameradlg->Create( m_hWnd );
//	m_cameradlg->SetParams( m_degxz, m_eye_y, m_camdist, m_targetpos, m_degy );
//	int vpno;
//	for( vpno = 0; vpno < 4; vpno++ ){
//		m_cameradlg->m_viewparam[vpno] = g_inidata->viewparam[vpno];
//	}

/***
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
***/

	m_optdlg = new COptDlg();
	if( !m_optdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OptDlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_syncdlg = new CSyncDlg( this );
	if( !m_syncdlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "syncdlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_syncdlg->Create( m_hWnd );


	m_optdlg->m_bonedisp = g_inidata->opt_jointdisp;//!!!!!!!!
	m_optdlg->m_grounddisp = g_inidata->opt_gdisp;//!!!!!!!!
	m_optdlg->m_quatype = g_inidata->opt_namequa;
	m_optdlg->m_bmdisp = g_inidata->opt_bmdisp;
	//m_optdlg->m_previewfps = g_inidata->opt_previewfps;
	m_syncdlg->m_fps = g_inidata->opt_previewfps;
	m_optdlg->m_motstep = g_inidata->opt_motstep;
	m_optdlg->m_ikfk = g_inidata->opt_ikfk;
	m_optdlg->m_iktdisp = g_inidata->opt_iktdisp;

	m_optdlg->m_MouseL = g_inidata->opt_MouseL;
	m_optdlg->m_MouseR = g_inidata->opt_MouseR;
	m_optdlg->m_MouseW = g_inidata->opt_MouseW;

	m_optdlg->m_usekinect = g_inidata->usekinect;

	if( g_inidata->opt_polydisp == 0 ){
		m_optdlg->m_polydisp = 1;//
	}else{
		m_optdlg->m_polydisp = 0;//
	}
	
	m_optdlg->m_jointsize = g_inidata->opt_jointsize;
	m_optdlg->m_jointalpha = g_inidata->opt_jointalpha;
	m_optdlg->m_undodlg = g_inidata->opt_undodlg;
	m_optdlg->m_rdlg = g_inidata->opt_rdlg;
	m_optdlg->m_infdlg = g_inidata->opt_infdlg;
	m_optdlg->m_modlg = g_inidata->opt_modlg;

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


//	m_parsdlg = new CParsDlg( PARSMODE_PARS, 45, 3000 );
//	if( !m_parsdlg ){
//		errormes.errorcode = E_OUTOFMEMORY;
//		errormes.type = MSGERR_APPMUSTEXIT;
//		errormes.mesptr = "ParsDlg alloc error";
//		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
//		_ASSERT( 0 );
//		return -1;
//	}
	

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

	m_za4dlg = new CZa4Dlg( this );
	if( !m_za4dlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Za4Dlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_za4dlg->Create( m_hWnd );


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

	m_Toon0Dlg = new CToon0Dlg( this );
	if( !m_Toon0Dlg ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Toon0Dlg alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	m_Toon0Dlg->Create( m_hWnd );


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
	motmenu = GetSubMenu( m_appmenu, 3 );
    //m_animmenu = GetSubMenu( motmenu, 4 );
	m_animmenu = GetSubMenu( motmenu, 7 );
	_ASSERT( m_animmenu );
	

	HMENU cammenu;
	cammenu = GetSubMenu( m_appmenu, 4 );
	m_cameramenu = GetSubMenu( cammenu, 4 );
	_ASSERT( m_cameramenu );
	//EnableMenuItem( m_appmenu, 59002, MF_BYCOMMAND | MF_GRAYED );

	HMENU gpmenu;
	gpmenu = GetSubMenu( m_appmenu, 5 );
	m_gpmenu = GetSubMenu( gpmenu, 4 );
	_ASSERT( m_gpmenu );

	HMENU moemenu;
	moemenu = GetSubMenu( m_appmenu, 6 );
	m_moemenu = GetSubMenu( moemenu, 4 );
	_ASSERT( m_moemenu );

//	m_kinectmenu = GetSubMenu( m_appmenu, 12 );
//	_ASSERT( m_kinectmenu );

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
///// dummytriの読み込み
	CMQOFile* dtrimqofile;
	dtrimqofile = new CMQOFile( m_hWnd );
	if( !dtrimqofile ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit MQOFile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	chkleng = leng + (int)strlen( "dummytri.mqo" );
	if( chkleng >= 2048 ){
		errormes.errorcode = E_OUTOFMEMORY;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "OneTimeSceneInit moduledir filename too long error !!!";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;	
	}

	char dtrifilename[2048];
	strcpy_s( dtrifilename, 2048, moduledir );
	strcat_s( dtrifilename, 2048, "dummytri.mqo" );

	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 rot10( 0.0f, 0.0f, 0.0f );
	ret = dtrimqofile->LoadMQOFile( 4.0f, dtrifilename, m_dtri_th, m_dtri_sh, m_dtri_mh, 0, 0, BONETYPE_RDB2, 0, offsetpos, rot10 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "dtri mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		delete dtrimqofile;
		return -1;
	}
	//m_select_sh->m_TLmode = TLMODE_D3D;//!!!  


	ret = m_dtri_sh->CreateBoneInfo( m_dtri_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : dtri : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "dtri CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		delete dtrimqofile;
		return -1;
	}	

	delete dtrimqofile;

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
///// ei3.mqo
	CMQOFile ei3mqofile( m_hWnd );
	char ei3filename[2048];
	strcpy_s( ei3filename, 2048, moduledir );
	strcat_s( ei3filename, 2048, s_ei3mqo );

	//D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
	//D3DXVECTOR3 rot3( 0.0f, 0.0f, 0.0f );
	ret = ei3mqofile.LoadMQOFile( 1.0f, ei3filename, m_ei3_th, m_ei3_sh, m_ei3_mh, 0, 0, BONETYPE_RDB2, 0, offsetpos, rot3 );
	if( ret ){
		errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ei3 mqo file open error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = m_ei3_sh->CreateBoneInfo( m_ei3_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : OneTimeSceneInit : ei3 : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ei3 CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
	}	


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
	D3DXVECTOR3 rot20( 0.0f, 0.0f, 0.0f );
	ret = sqsigfile->LoadSigFileFromBuf( lpData, (int)dwResSize, m_sq_th, m_sq_sh, m_sq_mh, 0, 1.0f, 0, offsetpos, rot20, 0 );
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
	ret = sigfile->LoadSigFile( arrowfilename, m_arrow_th, m_arrow_sh, m_arrow_mh, 0, D3DXVECTOR3(0.6f, 0.6f, 0.6f), 0, offsetpos, rot11, 0 );
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

	m_timerid = (int)::SetTimer( m_hWnd, MOVEBONETIMER, TIMERINTERVAL, NULL );
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
	if( g_endapp == 1 ){
		return 0;
	}


	if( m_destroyflag != 0 ){
		return 0;
	}

	RECT tmprect;
	::GetClientRect( m_3dwnd.m_hWnd, &tmprect );
	if( (tmprect.bottom <= 0) || (tmprect.right <= 0) ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	m_clirect = tmprect;
	m_clientWidth = m_clirect.right - m_clirect.left;
	m_clientHeight = m_clirect.bottom - m_clirect.top;


	int ret;
	ERRORMES errormes;
	static int bmpflag = 0;

//	GetKeyboardState( g_KeyBuffer );
	GetKeyboardCnt();

	if( (g_KeyBuffer['G'] & 0x80) && (g_KeyBuffer[VK_LEFT] & 0x80) ){
		if( m_graphwnd ){
			m_graphwnd->ShiftRange( -1 );
			Sleep( 60 );
		}
	}else if( (g_KeyBuffer['G'] & 0x80) && (g_KeyBuffer[VK_RIGHT] & 0x80) ){
		if( m_graphwnd ){
			m_graphwnd->ShiftRange( 1 );
			Sleep( 60 );
		}
	}


	if( (m_preview_flag == 0) && (m_writebmpflag == 0) ){
		if( (g_KeyBuffer['S'] & 0x80) && (g_KeyBuffer['1'] & 0x80) ){
			g_motdlg->SaveFrame( SAVEKEY_0 );
			Sleep( 60 );
		}else if( (g_KeyBuffer['S'] & 0x80) && (g_KeyBuffer['2'] & 0x80) ){
			g_motdlg->SaveFrame( SAVEKEY_1 );
			Sleep( 60 );
		}else if( (g_KeyBuffer['S'] & 0x80) && (g_KeyBuffer['3'] & 0x80) ){
			g_motdlg->SaveFrame( SAVEKEY_2 );
			Sleep( 60 );
		}else if( (g_KeyBuffer['R'] & 0x80) && (g_KeyBuffer['1'] & 0x80) ){
			g_motdlg->RestoreFrameSave( SAVEKEY_0 );
			Sleep( 60 );
		}else if( (g_KeyBuffer['R'] & 0x80) && (g_KeyBuffer['2'] & 0x80) ){
			g_motdlg->RestoreFrameSave( SAVEKEY_1 );
			Sleep( 60 );
		}else if( (g_KeyBuffer['R'] & 0x80) && (g_KeyBuffer['3'] & 0x80) ){
			g_motdlg->RestoreFrameSave( SAVEKEY_2 );
			Sleep( 60 );
		}
	}


//	if( (g_KeyBuffer['1'] & 0x80) && (g_KeyBuffer['2'] & 0x80) ){
//		char chkmes[1024];
//		CShdElem* mfelem = (*m_shandler)( 75 );
//		_ASSERT( mfelem );
//		CShdElem* faceelem = (*m_shandler)( 7 );
//		_ASSERT( faceelem );
//		sprintf_s( chkmes, 1024, "mfolder %d, %d, face %d, %d", 
//			mfelem->dispflag, mfelem->isselected, 
//			faceelem->dispflag, faceelem->isselected );
//		::MessageBox( m_hWnd, chkmes, "check", MB_OK );
//	}

//	if( (g_KeyBuffer['1'] & 0x80) && (g_KeyBuffer['2'] & 0x80) ){
//		ret = SaveEQUFile();
//		_ASSERT( !ret );
//		::MessageBox( m_hWnd, "SaveEQUFile", "check", MB_OK );
//	}
//	if( (g_KeyBuffer['1'] & 0x80) && (g_KeyBuffer['2'] & 0x80) ){
//		ret = LoadESIFile();
//		_ASSERT( !ret );
//		::MessageBox( m_hWnd, "LoadESIFile", "check", MB_OK );
//	}
//	if( (g_KeyBuffer['1'] & 0x80) && (g_KeyBuffer['2'] & 0x80) ){
//		ret = DelDispObj();
//		_ASSERT( !ret );
//		::MessageBox( m_hWnd, "DelMorph", "check", MB_OK );
//	}
//	if( (g_KeyBuffer['1'] & 0x80) && (g_KeyBuffer['2'] & 0x80) ){
//		ret = CloneDispObj();
//		_ASSERT( !ret );
//		::MessageBox( m_hWnd, "CloneDispObj", "check", MB_OK );
//	}

//	if( m_parsdlg->parsmode == PARSMODE_PARS ){
//		float fov = (float)m_parsdlg->parsfov * (float)DEG2PAI;
//		float aspect;
//		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
//		D3DXMatrixPerspectiveFovLH( &m_matProjX, fov, aspect, m_proj_near, m_proj_far );
//
//	}else{
//		float vw, vh;
//		vw = (float)m_parsdlg->orthosize;
//		vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
//		D3DXMatrixOrthoLH( &m_matProjX, vw, vh, m_proj_near, m_proj_far );
//	}

	//Shift+F5 - Shift+F8, F5 - F8
/***
	if( g_KeyBuffer[VK_SHIFT] & 0x80 ){
		if( g_KeyBuffer[VK_F5] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 0 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ５キーに保存しました。", "保存確認", MB_OK );
		}else if( g_KeyBuffer[VK_F6] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 1 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ６キーに保存しました。", "保存確認", MB_OK );
		}else if( g_KeyBuffer[VK_F7] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 2 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ７キーに保存しました。", "保存確認", MB_OK );
		}else if( g_KeyBuffer[VK_F8] & 0x80 ){
			m_cameradlg->SetCurrent2ViewParam( 3 );
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ８キーに保存しました。", "保存確認", MB_OK );
		}
	}else{
		int setflag = 0;
		if( g_KeyBuffer[VK_F5] & 0x80 ){
			m_cameradlg->SetViewParam2Current( 0 );
			setflag = 1;
		}else if( g_KeyBuffer[VK_F6] & 0x80 ){
			m_cameradlg->SetViewParam2Current( 1 );
			setflag = 1;
		}else if( g_KeyBuffer[VK_F7] & 0x80 ){
			m_cameradlg->SetViewParam2Current( 2 );
			setflag = 1;
		}else if( g_KeyBuffer[VK_F8] & 0x80 ){
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
***/



	//shift+Dキーを押して放したときにm_drawtextをトグル。
	static int dbgDflag = 0;
	int shiftD = 0;
	if( (g_KeyBuffer[VK_SHIFT] & 0x80) && (g_KeyBuffer['D'] & 0x80) ){
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



//    D3DXMatrixIdentity( &m_matWorld );

	if( m_tbb_r || m_tbb_t || m_tbb_t2 ){
		if( m_shandler ){
			GPELEM curgpe;
			g_motdlg->m_gpdlg->GetCurGP( &curgpe );
			m_shandler->m_gpdata.SetGPE( &curgpe );
			m_shandler->m_gpdata.GetMatWorld( &m_matWorld );
		}else{
			//m_shandler->m_gpdata.m_matWorld = m_inimat;
			m_matWorld = m_inimat;
		}
	}else{
		if( m_shandler ){
			m_shandler->m_gpdata.InitParams();
		}
		m_matWorld = m_inimat;
	}
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

	
    // Rotate the camera about the y-axis

	/***
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
	D3DXMatrixLookAtLH( &m_matView, &m_vEyePt, &m_targetpos, &m_camup );
	***/

	UpdateCameraFromDlg();


	if( m_ortho == 0 ){
		float fov = m_fov * (float)DEG2PAI;
		float aspect;
		aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		D3DXMatrixPerspectiveFovLH( &m_matProjX, fov, aspect, m_proj_near, m_proj_far );
	}else{
		float vw, vh;
		vw = m_orthosize;
		vh = m_orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		D3DXMatrixOrthoLH( &m_matProjX, vw, vh, m_proj_near, m_proj_far );
	}
	m_lightdlg->ParamsToLight();


	if( g_motdlg ){
		g_motdlg->AxisSetRenderParams( m_degy, m_degxz, m_gband, m_ground_sh, m_ground_mh, m_arrow_sh, m_arrow_mh, m_pFont );
	}


    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;


	if( g_pPostEffect && m_kinect && (m_kinectflag != 0) ){
		ret = m_kinect->Update();
		if( ret == 2 ){
			m_kinectflag = 0;
		}else if( ret != 0 ){
			_ASSERT( 0 );
			m_kinectflag = 0;
		}
	}


	LARGE_INTEGER beftime, afttime;

	QueryPerformanceCounter( &beftime );


	if( m_tbb_sha == 0 ){
		if( g_inidata->opt_gdisp && (srcseri == 0) ){
			ret = m_ground_sh->ChkInView( m_ground_mh, &m_inimat, &m_matView, &m_matProjX );

			ret = m_ground_sh->TransformDispData( m_vEyePt, m_ground_mh, &m_inimat, &m_matView, &m_matProjX, 1, 0 );
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
			ret = m_g_sh->ChkInView( m_g_mh, &m_inimat, &m_matView, &m_matProjX );

			ret = m_g_sh->TransformDispData( m_vEyePt, m_g_mh, &m_inimat, &m_matView, &m_matProjX, 1, 0 );
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
	}

	if( g_motdlg ){
		if( (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) ){

			if( g_motdlg->m_disptabno == TAB_GP ){
				m_selectedno = 0;
				if( m_shandler ){
					m_selectedpos = m_shandler->m_gpdata.m_gpe.pos;
				}else{
					m_selectedpos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
				}
			}else{
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
	if( (m_selectedno > 0) || 
		( (g_motdlg->m_disptabno == TAB_GP) && ((g_motdlg->GetGPAnimNum() > 0) ) || (g_motdlg->m_gpdlg->m_gpmode == GP_HEN)) ){
//selectの表示倍率を決定する。

		ret = CalcSelectMat();

		ret = m_select_sh->ChkInView( m_select_mh, &m_select_mat, &m_matView, &m_matProjX );

		ret = m_select_sh->TransformDispData( m_vEyePt, m_select_mh, &m_select_mat, &m_matView, &m_matProjX, 1, 0 );
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
	if( m_tbb_4 ){
		m_za4dlg->UpdateParams( m_selectedno );
	}


	if( m_tbb_sha == 0 ){	
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

			ret = m_shandler->ChkInView( m_mhandler, &m_matWorld, &m_matView, &m_matProjX );

			ret = m_shandler->TransformDispData( m_vEyePt, m_mhandler, &m_matWorld, &m_matView, &m_matProjX, 1, tra_boneonly );
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
	}


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



	if( g_motdlg ){
		g_motdlg->AxisSetRenderParams( m_degy, m_degxz, m_gband, m_ground_sh, m_ground_mh, m_arrow_sh, m_arrow_mh, m_pFont );
	}

	if( m_tbb_r == 1 ){
		if( (g_KeyBuffer['I'] & 0x80) || (g_KeyBuffer['T'] & 0x80) || (g_KeyBuffer['2'] & 0x80) ){
			ValidSelectCircle( 0 );
		}else{
			ValidSelectCircle( 1 );
		}
	}else if( m_tbb_iti == 1 ){
		if( m_itidlg->m_object == ITIOBJ_JOINTROT ){
			ValidSelectCircle( 1 );
		}else{
			ValidSelectCircle( 0 );
		}
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
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_destroyflag != 0 ){
		return 0;
	}


    // Update the camera here rather than in FrameMove() so you can
    // move the camera even when the scene is paused
    //UpdateCamera();
	int ret = 0;
	ERRORMES errormes;

	//ウインドウタイトルのセット（sigファイル名を記述）
	int writenameflag = 0;
	if( *(m_loadnamedlg->m_signame) ){
		writenameflag = strcmp( m_signame, m_loadnamedlg->m_signame );
		if( writenameflag ){
			strcpy_s( m_signame, 1024, m_loadnamedlg->m_signame );
		}
	}else{
		if( m_signame[0] != 0 ){
			writenameflag = 1;
			m_signame[0] = 0;
		}
	}
	if( writenameflag != 0 ){
		char mesname[1024];
		if( m_signame[0] != 0 ){
			sprintf_s( mesname, 1024, "RokDeBone2[%s]", m_signame );
		}else{
			strcpy_s( mesname, 1024, "RokDeBone2[NoData]" );
		}

		::SetWindowText( m_hWnd, mesname );
	}


	if( m_exit )
		return S_OK;


	ret = Render3D( m_pBackBuffer, m_pZSurf, &m_viewport, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : Render3D : error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "Render3D error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

///////// sleep
//	LARGE_INTEGER	m_freq;
//	LARGE_INTEGER	m_rendercnt;
//	LARGE_INTEGER	m_curcnt;
//	double			m_dfreq;
//	double			m_drenderfreq;
//	double			m_drendercnt;
//	double			m_dcurcnt;
	
	_ASSERT( m_dfreq > 0.0 );
	m_drenderfreq = m_dfreq / (double)m_syncdlg->m_fps;

	double ddiff;

	QueryPerformanceCounter( &m_curcnt );
	m_dcurcnt = (double)m_curcnt.QuadPart;

	ddiff = m_dcurcnt - m_drendercnt;

	if( m_preview_flag == 1 ){
		while( (ddiff > 0.0) && (ddiff < m_drenderfreq) ){
			QueryPerformanceCounter( &m_curcnt );
			m_dcurcnt = (double)m_curcnt.QuadPart;
			ddiff = m_dcurcnt - m_drendercnt;

			Sleep( 0 );
		}
	}else{
		double ddiff0 = ddiff;

		int waitms;
		int tmpms;
		if( ddiff0 > 0.0 ){
			tmpms = (int)( (1000.0 / (float)m_syncdlg->m_fps) - (ddiff0 / m_dfreq * 1000.0) );
			waitms = min( 500, max( 0, tmpms - 5 ) );
		}else{
			waitms = 0;
		}

//DbgOut( "check !!! : viewer render : ddiff0 %f, dfreq %f, tmpms %d, waitms %d\r\n", ddiff0, m_dfreq, tmpms, waitms );

		timeBeginPeriod( 1 );
		Sleep( waitms );
		timeEndPeriod( 1 );

		ddiff = m_dcurcnt - m_drendercnt;
		while( (ddiff > 0.0) && (ddiff < m_drenderfreq) ){
			QueryPerformanceCounter( &m_curcnt );
			m_dcurcnt = (double)m_curcnt.QuadPart;
			ddiff = m_dcurcnt - m_drendercnt;
			Sleep( 0 );
		}

		ddiff = m_dcurcnt - m_drendercnt;
	}

	QueryPerformanceCounter( &m_curcnt );
	m_rendercnt.QuadPart = m_curcnt.QuadPart;
	m_drendercnt = m_dcurcnt;

	if( ddiff != 0.0 )
		m_fFPS = (float)( 1.0 / (ddiff / m_dfreq) );
	else
		m_fFPS = 0.0f;


//DbgOut( "check !!! : ddiff %f, fps %f\r\n\r\n", ddiff, m_fFPS );


    return S_OK;
}

int CMyD3DApplication::Render3D( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int presentflag )
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_destroyflag != 0 ){
		return 0;
	}

	int ret;

	if( (m_tbb_sha == 1) && (m_createshadowflag == 1) && m_shadowelem.pTex && m_shadowelem.pBackBuffer && m_shadowelem.pZSurf ){
		ret = RenderWithShadow( psurf, pzsurf, pviewport, presentflag );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderWithShadow : error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		ret = RenderNormal( psurf, pzsurf, pviewport, presentflag );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderNormal : error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	return 0;
}


int CMyD3DApplication::RenderNormal( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int presentflag )
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_destroyflag != 0 ){
		return 0;
	}

	int ret;
	HRESULT hres;
	D3DXMATRIX matProjX;
	if( m_ortho == 0 ){
		float fov = (float)m_fov * (float)DEG2PAI;
		float aspect;
		//aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		aspect = (float)pviewport->Width / (float)pviewport->Height;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = m_orthosize;
		//vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		vh = m_orthosize * ((float)pviewport->Height / (float)pviewport->Width);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}


	D3DCOLOR clearcol;
	unsigned char colr, colg, colb;
	colr = GetRValue( g_inidata->bg_color );
	colg = GetGValue( g_inidata->bg_color );
	colb = GetBValue( g_inidata->bg_color );

	clearcol = D3DCOLOR_ARGB( 0, colr, colg, colb );

//DbgOut( "viewer : Render : clearcol %d %d %d\n", colr, colg, colb );

	// Clear the viewport

//DbgOut( "viewer : Render : bakcbuffer %x, pZSurf %x\r\n", pBackBuffer, m_pZSurf );


	hres = m_pd3dDevice->SetRenderTarget( 0, psurf );
	if( hres != D3D_OK ){
		DbgOut( "viewer : render : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	hres = m_pd3dDevice->SetDepthStencilSurface( pzsurf );
	if( hres != D3D_OK ){
		DbgOut( "viewer : render : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	g_rendertarget = TARGET_MAIN;//!!!!!!!!


	hres = m_pd3dDevice->SetViewport( pviewport );
	if( hres != D3D_OK ){
		DbgOut( "viewer : Render : dev SetViewport error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = SetShaderConst( 0, &matProjX );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return S_OK;


	D3DRECT clrc;
//	clrc.x1 = m_clirect.left;
//	clrc.y1 = m_clirect.top;
//	clrc.x2 = m_clirect.right;
//	clrc.y2 = m_clirect.bottom;
	clrc.x1 = 0;
	clrc.y1 = 0;
	clrc.x2 = pviewport->Width;
	clrc.y2 = pviewport->Height;

	hres = m_pd3dDevice->Clear( 1, &clrc, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						 clearcol,
						 1.0f, 0L );
	if( hres != D3D_OK ){
		DbgOut( "viewer : Render : dev Clear error %x !!!\n", hres );
		_ASSERT( 0 );
		return 1;
	}


	//Z奥は最後に描画。したいところだが、RenderBoneより後だと、Boneのスプライト表示がうまくいかない。
	ret = m_bgdisp->Render( m_pd3dDevice );
	if( ret ){
		DbgOut( "CD3DApplication : m_bgdisp->Render : error !!!\n" );
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


	if( g_inidata->opt_gdisp ){

		ret = m_ground_sh->Render( 0, 0, m_pd3dDevice, 0, POLYDISP_ALL, m_ground_mh, 0, m_inimat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "CMyD3DApplication : Render Ground: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_g_sh && m_g_disp && (m_tbb_r || m_tbb_t || m_tbb_t2) ){
		ret = m_g_sh->Render( 0, 0, m_pd3dDevice, 0, POLYDISP_ALL, m_g_mh, 0, m_inimat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "CMyD3DApplication : Render G: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

//		ret = m_g_sh->Render( 0, 0, m_pd3dDevice, 1, POLYDISP_ALL, m_g_mh, 0, m_inimat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
//		if( ret ){
//			DbgOut( "CMyD3DApplication : Render G: error !!!\n" );
//			_ASSERT( 0 );
//			return 1;
//		}

	}

	if( m_ExistData ){

		if( m_tbb_face == 1){
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

			ret = m_shandler->Render( 0, 0, m_pd3dDevice, 0, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			//半透明
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			ret = m_shandler->Render( 0, 0, m_pd3dDevice, 1, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render with alpha: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

		}


		ret = m_shandler->SaveToDispTempDiffuse();
		if( ret ){
			DbgOut( "viewer : sh SaveToDispTempDiffuse error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		if( (m_tbb_line == 1) && presentflag ){
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

			ret = m_shandler->Render( 0, 0, m_pd3dDevice, 0, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			//半透明
			ret = m_shandler->Render( 0, 0, m_pd3dDevice, 1, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render with alpha: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

		}

		if( ((m_tbb_point == 1) || (m_tbb_ten2 == 1)) && presentflag ){
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

			ret = m_shandler->Render( 0, 0, m_pd3dDevice, 0, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			//半透明
			ret = m_shandler->Render( 0, 0, m_pd3dDevice, 1, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render with alpha: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

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

	if( m_g_sh && m_g_disp && (m_tbb_r || m_tbb_t || m_tbb_t2) ){
		//地面半透明
		ret = m_g_sh->Render( 0, 0, m_pd3dDevice, 1, POLYDISP_ALL, m_g_mh, 0, m_inimat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
		if( ret ){
			DbgOut( "CMyD3DApplication : Render G: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}


	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );


	// iktarget
	if( m_ExistData && (g_inidata->opt_iktdisp != 0) && presentflag ){

		m_ikt_sh->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_ALWAYS );

		DWORD dwClipWidth  = m_clientWidth/2;
		DWORD dwClipHeight = m_clientHeight/2;


//		ret = m_ikt_sh->TransformAndRenderIKT( m_vEyePt, m_pd3dDevice, dwClipWidth, dwClipHeight, m_ikt_mh,
//			m_shandler, m_mhandler,
//			m_bm_sh, 
//			g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), 
//			m_matWorld, m_matView, m_matProjX, m_matProjX, m_smallgband );
		ret = m_ikt_sh->TransformAndRenderIKT( m_vEyePt, m_pd3dDevice, pviewport->Width, pviewport->Height, m_ikt_mh,
			m_shandler, m_mhandler,
			m_bm_sh, 
			g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), 
			m_matWorld, m_matView, matProjX, matProjX, m_smallgband );

		if( ret ){
			DbgOut( "CMyD3DApplication : Render ikt: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


	}



	if( m_ExistData && presentflag ){

		hres = m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
		g_renderstate[ D3DRS_ZFUNC ] = D3DCMP_ALWAYS;

		if( g_inidata->opt_bmdisp ){

			m_sigeditwnd->SaveForbidSelect();


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

			ret = m_bm_sh->RenderBoneMark( m_pd3dDevice, m_shandler, m_mhandler, m_matWorld, m_matView, matProjX, m_bonelinedlg->m_selcol, m_bonelinedlg->m_unselcol, selboneno );
			if( ret ){
				DbgOut( "viewer : Render : bm : sh RenderBoneMark error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}			
		}

		if( ( g_inidata->opt_jointdisp && (m_selectedno > 0) && 
			  (m_tbb_bone == 0 || ((m_tbb_bone == 1) && (m_bonedlg->m_ope == BONEOPE_POS))) ) ||
			 
			  ( g_inidata->opt_jointdisp &&
			    (m_preview_flag == 0) && (g_motdlg->m_disptabno == TAB_GP) && 
			    ((g_motdlg->GetGPAnimNum() > 0) || (g_motdlg->m_gpdlg->m_gpmode == GP_HEN)) ) 
		){

			m_select_sh->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_ALWAYS );

			ret = m_select_sh->Render( 0, 0, m_pd3dDevice, 0, POLYDISP_ALL, m_select_mh, 0, m_select_mat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render Select 0: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			
			ret = m_select_sh->Render( 0, 0, m_pd3dDevice, 1, POLYDISP_ALL, m_select_mh, 0, m_select_mat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : Render Select 1: error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			
		}
		
		hres = m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
		g_renderstate[ D3DRS_ZFUNC ] = D3DCMP_LESSEQUAL;

	}

	//スプライトは最後じゃないと、うまくいかない（なぞ）
	if( m_ExistData && presentflag ){

		if( (m_tbb_iti == 1) && ((m_itidlg->m_object == ITIOBJ_JOINT) || (m_itidlg->m_object == ITIOBJ_JOINTROT))){
			int curboneonly;
			if( m_itidlg->m_childjoint == 0 ){
				curboneonly = 1;
			}else{
				curboneonly = 0;
			}

			ret = m_shandler->SetIsSelected( m_selectedno, 1, 1, curboneonly );
			_ASSERT( !ret );
		}


		ret = m_shandler->RenderBone( m_pd3dDevice, m_d3dxsprite, (float)m_optdlg->m_jointsize * 0.01f, m_optdlg->m_jointalpha,
			pviewport->Width / 2, pviewport->Height / 2, m_mhandler,
			m_matWorld, m_matView, matProjX );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderBone : error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope != PAINTOPE_BONE) ){
//			DWORD dwClipWidth  = m_clientWidth/2;
//			DWORD dwClipHeight = m_clientHeight/2;
			DWORD dwClipWidth  = pviewport->Width / 2;
			DWORD dwClipHeight = pviewport->Height / 2;

			if( m_paintdlg->m_targetdisp > 0 ){

				if( m_paintdlg->m_brushtype == 1 ){
					D3DXMATRIX scmat, tramat, worldmat;
					float scval;
					scval = m_paintdlg->m_brushsize / 100.0f;
					D3DXMatrixIdentity( &scmat );
					scmat._11 = scval;
					scmat._22 = scval;
					scmat._33 = scval;
					D3DXMatrixIdentity( &tramat );
					tramat._41 = m_paintdlg->m_pickpos.x;
					tramat._42 = m_paintdlg->m_pickpos.y;
					tramat._43 = m_paintdlg->m_pickpos.z;
					worldmat = scmat * tramat;

					//D3DXMatrixIdentity( &worldmat );

					ret = m_ei3_sh->ChkInView( m_ei3_mh, &worldmat, &m_matView, &matProjX );
					ret = m_ei3_sh->TransformDispData( m_vEyePt, m_ei3_mh, &worldmat, &m_matView, &matProjX, 1, 0 );
					if( ret ){
						DbgOut( "viewer1.cpp : Render : m_ei3_sh->TransformDispData error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					ret = m_ei3_sh->Render( 0, 0, m_pd3dDevice, 1, POLYDISP_ALL, m_ei3_mh, 0, worldmat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
					if( ret ){
						DbgOut( "CMyD3DApplication : Render ei3 : error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}
			}

			ret = m_shandler->RenderSelVert( m_mhandler, m_pd3dDevice, m_d3dxsprite, 
				m_paintdlg->m_targetdisp, m_paintdlg->m_targetvert, m_paintdlg->m_targetvertnum, m_paintdlg->m_brushtype, 
				m_paintdlg->m_markpart, m_paintdlg->m_markvert,
				pviewport->Width / 2, pviewport->Height / 2, m_matWorld, m_matView, matProjX );
			if( ret ){
				DbgOut( "CMyD3DApplication : RenderSelVert : error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

	}


	if( g_pPostEffect && m_kinect && (m_kinectflag != 0) ){

		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_renderstate[ D3DRS_ALPHABLENDENABLE ] = TRUE;

		//-------------------------------------------------
		// シェーダの設定
		//-------------------------------------------------
		hres = g_pPostEffect->SetTechnique( g_hPostEffectTLV );
		_ASSERT( hres == D3D_OK );

//		float fw = (float)m_kinect->m_texwidth;
//		float fh = (float)m_kinect->m_texheight;
		float fw = (float)pviewport->Width;
		float fh = (float)pviewport->Height;
		hres = g_pPostEffect->SetValue( g_hpeWidth0, &fw, sizeof( float ) );
		if( hres != D3D_OK ){
			_ASSERT( 0 );
			return 1;
		}
		hres = g_pPostEffect->SetValue( g_hpeHeight0, &fh, sizeof( float ) );
		if( hres != D3D_OK ){
			_ASSERT( 0 );
			return 1;
		}
		hres = g_pPostEffect->SetTexture( g_hpeTexture0, m_kinect->m_ptex );
		if( hres != D3D_OK ){
			_ASSERT( 0 );
			return 1;
		}

		hres = g_pPostEffect->Begin( NULL, 0 );
		_ASSERT( hres == D3D_OK );
		hres = g_pPostEffect->BeginPass( 0 );
		_ASSERT( hres == D3D_OK );
		float offsetu1 = 0.0f;
		float offsetv1 = 0.0f;

		TLVERTEX Vertex1[4] = {
			//   x    y     z      w     diffuse F4 u0 v0
			{0.1f, 0.1f, 0.0001f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f,    0.0f + offsetu1, 0.0f + offsetv1},
			{+1.0f, 0.1f, 0.0001f, 1.0f,  1.0f, 1.0f, 1.0f, 0.5f,    1.0f + offsetu1, 0.0f + offsetv1},
			{+1.0f, -1.0f, 0.0001f, 1.0f,  1.0f, 1.0f, 1.0f, 0.5f,    1.0f + offsetu1, 1.0f + offsetv1},
			{0.1f, -1.0f, 0.0001f, 1.0f,  1.0f, 1.0f, 1.0f, 0.5f,    0.0f + offsetu1, 1.0f + offsetv1}
		};

		hres = m_pd3dDevice->SetVertexDeclaration( m_tlvdecl );
		_ASSERT( hres == D3D_OK );

		hres = m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
						, 2, Vertex1, sizeof( TLVERTEX ) );
		_ASSERT( hres == D3D_OK );

		hres = g_pPostEffect->EndPass();
		_ASSERT( hres == D3D_OK );

		hres = g_pPostEffect->End();
		_ASSERT( hres == D3D_OK );


		if( ( (m_kinect->m_capmode == 1) || (m_kinect->m_capmode == 2) )&& (m_kinect->m_curtimer > 0) ){
			hres = g_pPostEffect->SetTechnique( g_hPostEffectTLV );
			_ASSERT( hres == D3D_OK );


			int tno;
			tno = m_kinect->m_curtimer / 30 + 1;
			if( tno < 0 ){
				tno = 0;
			}
			if( tno > 9 ){
				tno = 9;
			}
			char tname[256];
			sprintf_s( tname, 256, "%d.png", tno );
			LPDIRECT3DTEXTURE9 texnum;
			texnum = g_texbnk->GetTexData( tname, 0 );
			_ASSERT( texnum );

			hres = g_pPostEffect->SetTexture( g_hpeTexture0, texnum );
			if( hres != D3D_OK ){
				_ASSERT( 0 );
				return 1;
			}

			hres = g_pPostEffect->Begin( NULL, 0 );
			_ASSERT( hres == D3D_OK );
			hres = g_pPostEffect->BeginPass( 0 );
			_ASSERT( hres == D3D_OK );
			float offsetu1 = 0.0f;
			float offsetv1 = 0.0f;

			TLVERTEX Vertex1[4] = {
				//   x    y     z      w     diffuse F4 u0 v0
				{-1.0f, 1.0f, 0.0001f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f,    0.0f + offsetu1, 0.0f + offsetv1},
				{0.0f, 1.0f, 0.0001f, 1.0f,  1.0f, 1.0f, 1.0f, 0.5f,    1.0f + offsetu1, 0.0f + offsetv1},
				{0.0f, 0.0f, 0.0001f, 1.0f,  1.0f, 1.0f, 1.0f, 0.5f,    1.0f + offsetu1, 1.0f + offsetv1},
				{-1.0f, 0.0f, 0.0001f, 1.0f,  1.0f, 1.0f, 1.0f, 0.5f,    0.0f + offsetu1, 1.0f + offsetv1}
			};

			hres = m_pd3dDevice->SetVertexDeclaration( m_tlvdecl );
			_ASSERT( hres == D3D_OK );

			hres = m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
							, 2, Vertex1, sizeof( TLVERTEX ) );
			_ASSERT( hres == D3D_OK );

			hres = g_pPostEffect->EndPass();
			_ASSERT( hres == D3D_OK );

			hres = g_pPostEffect->End();
			_ASSERT( hres == D3D_OK );
		}

	}



	if( (m_preview_flag == 0) && (m_writebmpflag == 0) && presentflag && m_ExistData ){
		ret = RenderCameraSprite();
		if( ret ){
			DbgOut( "viewer : RenderCameraSprite error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	// End the scene.
	m_pd3dDevice->EndScene();


	if( (m_writebmpflag == 0) && presentflag ){
		m_pd3dDevice->Present( NULL, NULL, m_3dwnd.m_hWnd, NULL );
		g_rendercnt = 0;
	}

	return 0;
}


int CMyD3DApplication::RenderWithShadow( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int presentflag )
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_destroyflag != 0 ){
		return 0;
	}

	int ret;
	HRESULT hres;

	D3DXMATRIX matProjX;
	float fov = (float)m_fov * (float)DEG2PAI;
	if( m_ortho == 0 ){
		float aspect;
		//aspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
		aspect = (float)pviewport->Width / (float)pviewport->Height;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = m_orthosize;
		//vh = (float)m_parsdlg->orthosize * ((float)m_d3dpp.BackBufferHeight / (float)m_d3dpp.BackBufferWidth);
		vh = m_orthosize * ((float)pviewport->Height / (float)pviewport->Width);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return S_OK;


//shadowmap SetRenderTarget
	hres = m_pd3dDevice->SetRenderTarget( 0, m_shadowelem.pBackBuffer );
	if( hres != D3D_OK ){
		DbgOut( "RenderWithShadow : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	hres = m_pd3dDevice->SetDepthStencilSurface( m_shadowelem.pZSurf );
	if( hres != D3D_OK ){
		DbgOut( "RenderWithShadow : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	g_rendertarget = TARGET_SHADOWMAP;//!!!!!!!!

	D3DVIEWPORT9 viewport;
    viewport.X = 0;
    viewport.Y = 0;
    viewport.Height = m_shadowelem.height;
    viewport.Width =  m_shadowelem.width;
    viewport.MaxZ = 1.0f;
    viewport.MinZ = 0.0f;

    hres = m_pd3dDevice->SetViewport(&viewport);
	if( hres != D3D_OK ){
		DbgOut( "RenderWithShadow : dev SetViewport error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	hres = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
						 0xFFFFFFFF,
						 1.0f, 0L );
	if( hres != D3D_OK ){
		_ASSERT( 0 );
		DbgOut( "RenderWithShadow : Clear error !!!\n" );
		return 1;
	}
//////// shadowmap chkinview( normal params )
	if( g_inidata->opt_gdisp ){
		ret = m_ground_sh->ChkInView( m_ground_mh, &m_inimat, &m_matView, &matProjX );
		_ASSERT( !ret );
	}
	if( m_g_sh ){
		ret = m_g_sh->ChkInView( m_g_mh, &m_inimat, &m_matView, &matProjX );
		_ASSERT( !ret );
	}

	if( m_ExistData ){
		ret = m_shandler->ChkInView( m_mhandler, &m_matWorld, &m_matView, &matProjX );
		_ASSERT( !ret );
	}

//////// shadowmap LiSPSM view proj
	float aspect0;
	//aspect0 = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
	aspect0 = (float)pviewport->Width / (float)pviewport->Height;
	CVec3List	cList;
	// ビューボリュームを求める
	//float fov = (float)m_fov * (float)DEG2PAI;
	CalcPointsOfViewBolume( &cList, m_matView, fov, aspect0, m_proj_near, m_proj_far );

	if( m_ExistData ){
		ret = m_shandler->SetPointListForShadow( &cList );
		if( ret ){
			DbgOut( "RenderWithShadow : sh SetPointListForShadow error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	// 変換行列を求める
	g_sStatus.vEyePos = m_vEyePt;
	D3DXVECTOR3 vdir = m_targetpos - m_vEyePt;
	D3DXVec3Normalize( &vdir, &vdir );
	g_sStatus.vViewDir = vdir;
	g_sStatus.fNearDist = m_proj_near;
	g_sStatus.vLightDir = g_lighthead->orgDirection;
	CalcLispMatrix(&g_sStatus, &cList);
	m_matViewShadow = g_sStatus.matView;
	m_matProjShadow = g_sStatus.matProj;

	// 射影空間から、テクスチャーの空間に変換する
	float fOffsetX = 0.5f + (0.5f / (float)m_shadowelem.width);
	float fOffsetY = 0.5f + (0.5f / (float)m_shadowelem.height);
	m_matBiasShadow = D3DXMATRIX(	0.5f,     0.0f,     0.0f,   0.0f,
							0.0f,    -0.5f,     0.0f,   0.0f,
							0.0f,     0.0f,     0.0f,	0.0f,
							fOffsetX, fOffsetY, 0.0f,   1.0f );
////////// 	
	ret = SetShaderConst( 1, &matProjX );
	if( ret ){
		DbgOut( "RenderWithShadow : SetShaderConst 1 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
////////// shadowmap chkinview and Transform (shadow params)	
	if( g_inidata->opt_gdisp ){
		ret = m_ground_sh->ChkInView( m_ground_mh, &m_inimat, &m_matViewShadow, &m_matProjShadow );
		_ASSERT( !ret );
		ret = m_ground_sh->TransformDispData( g_sStatus.newpos, m_ground_mh, &m_inimat, &m_matViewShadow, &m_matProjShadow, 1, 0 );
		_ASSERT( !ret );
	}

	if( m_g_sh && (m_tbb_r || m_tbb_t || m_tbb_t2) ){
		ret = m_g_sh->ChkInView( m_g_mh, &m_inimat, &m_matViewShadow, &m_matProjShadow );
		_ASSERT( !ret );
		ret = m_g_sh->TransformDispData( g_sStatus.newpos, m_g_mh, &m_inimat, &m_matViewShadow, &m_matProjShadow, 1, 0 );
		_ASSERT( !ret );
	}

	if( m_ExistData ){
		ret = m_shandler->ChkInView( m_mhandler, &m_matWorld, &m_matViewShadow, &m_matProjShadow );
		_ASSERT( !ret );
		ret = m_shandler->TransformDispData( g_sStatus.newpos, m_mhandler, &m_matWorld, &m_matViewShadow, &m_matProjShadow, 1, 0 );
		_ASSERT( !ret );
	}
///////////// shadowmap render to texture
	if( g_rendercnt == 0 ){
		//スプライト描画でステートが不定になるのでここでの呼び出しが必要
		m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_renderstate[ D3DRS_SRCBLEND ] = D3DBLEND_SRCALPHA;
		m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		g_renderstate[ D3DRS_DESTBLEND ] = D3DBLEND_INVSRCALPHA;
		m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
		g_renderstate[ D3DRS_ALPHATESTENABLE ] = 0;
	}
	g_rendercnt++;

	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	g_renderstate[ D3DRS_ALPHATESTENABLE ] = TRUE;

	m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
	g_renderstate[ D3DRS_ALPHAREF ] = 0x08;

	m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	g_renderstate[ D3DRS_ALPHAFUNC ] = D3DCMP_GREATEREQUAL;

	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	g_renderstate[ D3DRS_ALPHABLENDENABLE ] = FALSE;

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,  D3DZB_TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ZFUNC,  D3DCMP_LESSEQUAL );
	g_renderstate[ D3DRS_ZENABLE ] = D3DZB_TRUE;
	g_renderstate[ D3DRS_ZWRITEENABLE ] = TRUE;
	g_renderstate[ D3DRS_ZFUNC ] = D3DCMP_LESSEQUAL;

	if( m_ExistData ){
		ret = m_shandler->RenderShadowMap0( m_pd3dDevice, POLYDISP_ALL, m_mhandler, 
			&m_matWorld, &m_matViewShadow, &m_matProjShadow, g_sStatus.newpos, 0, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}
	if( g_inidata->opt_gdisp ){
		ret = m_ground_sh->RenderShadowMap0( m_pd3dDevice, POLYDISP_ALL, m_ground_mh, 
			&m_inimat, &m_matViewShadow, &m_matProjShadow, g_sStatus.newpos, 0, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}
	if( m_g_sh && (m_tbb_r || m_tbb_t || m_tbb_t2) ){
		ret = m_g_sh->RenderShadowMap0( m_pd3dDevice, POLYDISP_ALL, m_g_mh, 
			&m_inimat, &m_matViewShadow, &m_matProjShadow, g_sStatus.newpos, 0, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}
///////////////// screen SetRenderTarget
		D3DCOLOR clearcol;
		unsigned char colr, colg, colb;
		colr = GetRValue( g_inidata->bg_color );
		colg = GetGValue( g_inidata->bg_color );
		colb = GetBValue( g_inidata->bg_color );
		clearcol = D3DCOLOR_ARGB( 0, colr, colg, colb );

		hres = m_pd3dDevice->SetRenderTarget( 0, psurf );
		if( hres != D3D_OK ){
			DbgOut( "RenderWithShadow : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
			_ASSERT( 0 );
			return 1;
		}
		hres = m_pd3dDevice->SetDepthStencilSurface( pzsurf );
		if( hres != D3D_OK ){
			DbgOut( "RenderWithShadow : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
			_ASSERT( 0 );
			return 1;
		}
		g_rendertarget = TARGET_MAIN;//!!!!!!!!

		hres = m_pd3dDevice->SetViewport( pviewport );
		if( hres != D3D_OK ){
			DbgOut( "viewer : Render : dev SetViewport error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = SetShaderConst( 0, &matProjX );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		D3DRECT clrc;
//		clrc.x1 = m_clirect.left;
//		clrc.y1 = m_clirect.top;
//		clrc.x2 = m_clirect.right;
//		clrc.y2 = m_clirect.bottom;
		clrc.x1 = 0;
		clrc.y1 = 0;
		clrc.x2 = pviewport->Width;
		clrc.y2 = pviewport->Height;

		hres = m_pd3dDevice->Clear( 1, &clrc, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
							 clearcol,
							 1.0f, 0L );
		if( hres != D3D_OK ){
			DbgOut( "RenderWithShadow : dev Clear error %x !!!\n", hres );
			_ASSERT( 0 );
			return 1;
		}
///////////////////////// screen chkinview transform (normal params)
	if( g_inidata->opt_gdisp ){
		ret = m_ground_sh->ChkInView( m_ground_mh, &m_inimat, &m_matView, &matProjX );
		_ASSERT( !ret );
		ret = m_ground_sh->TransformDispData( m_vEyePt, m_ground_mh, &m_inimat, &m_matView, &matProjX, 1, 0 );
		_ASSERT( !ret );
	}

	if( m_g_sh && (m_tbb_r || m_tbb_t || m_tbb_t2) ){
		ret = m_g_sh->ChkInView( m_g_mh, &m_inimat, &m_matView, &matProjX );
		_ASSERT( !ret );
		ret = m_g_sh->TransformDispData( m_vEyePt, m_g_mh, &m_inimat, &m_matView, &matProjX, 1, 0 );
		_ASSERT( !ret );
	}

	if( m_ExistData ){
		ret = m_shandler->ChkInView( m_mhandler, &m_matWorld, &m_matView, &matProjX );
		_ASSERT( !ret );
		ret = m_shandler->TransformDispData( m_vEyePt, m_mhandler, &m_matWorld, &m_matView, &matProjX, 1, 0 );
		_ASSERT( !ret );
	}
////////////////////// screen RenderShadowMap1(withalpha 0)
	int withalpha = 0;
	if( m_ExistData ){
		ret = m_shandler->RenderShadowMap1( m_pd3dDevice, withalpha, POLYDISP_ALL, m_mhandler, 
			&m_matWorld, &m_matView, &matProjX, m_vEyePt, 0, 0 );
		_ASSERT( !ret );
	}
	if( g_inidata->opt_gdisp ){
		ret = m_ground_sh->RenderShadowMap1( m_pd3dDevice, withalpha, POLYDISP_ALL, m_ground_mh,
			&m_inimat, &m_matView, &matProjX, m_vEyePt, 0, 0 );
		_ASSERT( !ret );
	}
	if( m_g_sh && (m_tbb_r || m_tbb_t || m_tbb_t2) ){
		ret = m_g_sh->RenderShadowMap1( m_pd3dDevice, withalpha, POLYDISP_ALL, m_g_mh,
			&m_inimat, &m_matView, &matProjX, m_vEyePt, 0, 0 );
		_ASSERT( !ret );
	}


////////////////////// screen RenderBG
	ret = m_bgdisp->Render( m_pd3dDevice );
	_ASSERT( !ret );

////////////////////// screen RenderShadowMap1(withalpha 1)
	withalpha = 1;
	if( m_ExistData ){
		ret = m_shandler->RenderShadowMap1( m_pd3dDevice, withalpha, POLYDISP_ALL, m_mhandler, 
			&m_matWorld, &m_matView, &matProjX, m_vEyePt, 0, 0 );
		_ASSERT( !ret );
	}
	if( g_inidata->opt_gdisp ){
		ret = m_ground_sh->RenderShadowMap1( m_pd3dDevice, withalpha, POLYDISP_ALL, m_ground_mh,
			&m_inimat, &m_matView, &matProjX, m_vEyePt, 0, 0 );
		_ASSERT( !ret );
	}
	if( m_g_sh && (m_tbb_r || m_tbb_t || m_tbb_t2) ){
		ret = m_g_sh->RenderShadowMap1( m_pd3dDevice, withalpha, POLYDISP_ALL, m_g_mh,
			&m_inimat, &m_matView, &matProjX, m_vEyePt, 0, 0 );
		_ASSERT( !ret );
	}
/////////////////////// screen render select
	if( m_ExistData && presentflag ){
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

			ret = m_bm_sh->RenderBoneMark( m_pd3dDevice, m_shandler, m_mhandler, m_matWorld, m_matView, matProjX, m_bonelinedlg->m_selcol, m_bonelinedlg->m_unselcol, selboneno );
			if( ret ){
				DbgOut( "RenderWithShadow : bm : sh RenderBoneMark error !!!\n" );
				_ASSERT( 0 );
				return -1;
			}			
		}

		if( (g_inidata->opt_jointdisp) && ( (m_selectedno > 0) || ( (g_motdlg->m_disptabno == TAB_GP) && (g_motdlg->GetGPAnimNum() > 0) ) ) && 
			(m_tbb_bone == 0 || ((m_tbb_bone == 1) && (m_bonedlg->m_ope == BONEOPE_POS))) ){

			m_select_sh->SetElemRenderState( -1, D3DRS_ZFUNC, D3DCMP_ALWAYS );

			ret = m_select_sh->Render( 0, 0, m_pd3dDevice, 0, POLYDISP_ALL, m_select_mh, 0, m_select_mat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : RenderWithShadow Select 0: error !!!\n" );
				_ASSERT( 0 );
				return -1;
			}
			
			ret = m_select_sh->Render( 0, 0, m_pd3dDevice, 1, POLYDISP_ALL, m_select_mh, 0, m_select_mat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
			if( ret ){
				DbgOut( "CMyD3DApplication : RenderWithShadow Select 1: error !!!\n" );
				_ASSERT( 0 );
				return -1;
			}
			
		}
		
		hres = m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
		g_renderstate[ D3DRS_ZFUNC ] = D3DCMP_LESSEQUAL;

	//スプライトは最後じゃないと、うまくいかない（なぞ）
		//ret = m_shandler->RenderBone( m_pd3dDevice, m_d3dxsprite, (float)m_optdlg->m_jointsize * 0.01f, m_optdlg->m_jointalpha,
		//	m_clientWidth / 2, m_clientHeight / 2, m_mhandler,
		//	m_matWorld, m_matView, m_matProjX );
		ret = m_shandler->RenderBone( m_pd3dDevice, m_d3dxsprite, (float)m_optdlg->m_jointsize * 0.01f, m_optdlg->m_jointalpha,
			pviewport->Width / 2, pviewport->Height / 2, m_mhandler,
			m_matWorld, m_matView, matProjX );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderBone : error !!!\n" );
			_ASSERT( 0 );
			return -1;
		}
		if( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope != PAINTOPE_BONE) ){
			//DWORD dwClipWidth  = m_clientWidth/2;
			//DWORD dwClipHeight = m_clientHeight/2;
			DWORD dwClipWidth  = pviewport->Width / 2;
			DWORD dwClipHeight = pviewport->Height / 2;


			if( m_paintdlg->m_targetdisp > 0 ){
				if( m_paintdlg->m_brushtype == 1 ){
					D3DXMATRIX scmat, tramat, worldmat;
					float scval;
					scval = m_paintdlg->m_brushsize / 100.0f;
					D3DXMatrixIdentity( &scmat );
					scmat._11 = scval;
					scmat._22 = scval;
					scmat._33 = scval;
					D3DXMatrixIdentity( &tramat );
					tramat._41 = m_paintdlg->m_pickpos.x;
					tramat._42 = m_paintdlg->m_pickpos.y;
					tramat._43 = m_paintdlg->m_pickpos.z;
					worldmat = scmat * tramat;

					//D3DXMatrixIdentity( &worldmat );

					ret = m_ei3_sh->ChkInView( m_ei3_mh, &worldmat, &m_matView, &matProjX );
					ret = m_ei3_sh->TransformDispData( m_vEyePt, m_ei3_mh, &worldmat, &m_matView, &matProjX, 1, 0 );
					if( ret ){
						DbgOut( "viewer1.cpp : Render : m_ei3_sh->TransformDispData error !!!\n" );
						_ASSERT( 0 );
						return -1;
					}
					ret = m_ei3_sh->Render( 0, 0, m_pd3dDevice, 1, POLYDISP_ALL, m_ei3_mh, 0, worldmat, m_matView, matProjX, m_vEyePt, 0, 0, NOGLOW );
					if( ret ){
						DbgOut( "CMyD3DApplication : Render ei3 : error !!!\n" );
						_ASSERT( 0 );
						return -1;
					}
				}
			}

			ret = m_shandler->RenderSelVert( m_mhandler, m_pd3dDevice, m_d3dxsprite, 
				m_paintdlg->m_targetdisp, m_paintdlg->m_targetvert, m_paintdlg->m_targetvertnum, m_paintdlg->m_brushtype, 
				m_paintdlg->m_markpart, m_paintdlg->m_markvert,
				pviewport->Width, pviewport->Height, m_matWorld, m_matView, matProjX );
				//dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
			if( ret ){
				DbgOut( "CMyD3DApplication : RenderSelVert : error !!!\n" );
				_ASSERT( 0 );
				return -1;
			}
		}

	}

	if( (m_preview_flag == 0) && (m_writebmpflag == 0) && presentflag ){
		ret = RenderCameraSprite();
		if( ret ){
			DbgOut( "viewer : RenderCameraSprite error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


//////////////////////
	// End the scene.
	m_pd3dDevice->EndScene();
	if( (m_writebmpflag == 0) && presentflag ){
		m_pd3dDevice->Present( NULL, NULL, m_3dwnd.m_hWnd, NULL );
		g_rendercnt = 0;
	}

	return 0;
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
	::GetClientRect( m_3dwnd.m_hWnd, &m_clirect );
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

			char fxname[2048];
			char moduledir[2048];
			int leng;
			ZeroMemory( moduledir, 2048 );
			ZeroMemory( fxname, 2048 );
			leng = GetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPTSTR)moduledir, 2048 );
			if( (leng <= 0) || (leng >= 2048) ){
				DbgOut( "e3dhsp : E3DInit : GetEnvironmentVariable error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			char* endptr = 0;
			int ch = '\\';
			char* lasten = 0;
			lasten = strrchr( moduledir, ch );
			if( !lasten ){
				_ASSERT( 0 );
				return 1;
			}
			*lasten = 0;
			char* last2en = 0;
			char* last3en = 0;
			last2en = strrchr( moduledir, ch );
			if( last2en ){
				*last2en = 0;
				last3en = strrchr( moduledir, ch );
				if( last3en ){
					if( (strcmp( last2en + 1, "debug" ) == 0) ||
						(strcmp( last2en + 1, "Debug" ) == 0) ||
						(strcmp( last2en + 1, "DEBUG" ) == 0) ||
						(strcmp( last2en + 1, "release" ) == 0) ||
						(strcmp( last2en + 1, "Release" ) == 0) ||
						(strcmp( last2en + 1, "RELEASE" ) == 0)
						){

						endptr = last2en;
					}else{
						endptr = lasten;
					}
				}else{
					endptr = lasten;
				}
			}else{
				endptr = lasten;
			}

			*lasten = '\\';
			if( last2en )
				*last2en = '\\';
			if( last3en )
				*last3en = '\\';

			int cpleng;
			cpleng = (int)( endptr - moduledir + 1 );
			strncpy_s( fxname, 2048, moduledir, cpleng );
			strcat_s( fxname, 2048, "E3D_HLSL\\posteffect.fx" );


			int existflag;
			existflag = _access( fxname, 0 );
			if( existflag != 0 ){
				::MessageBox( NULL, "fxファイルが見つかりません。\ndllと同じ場所にE3D_HLSLフォルダをコピーしてください。",
					"エラー", MB_OK );
				_ASSERT( 0 );
				return 1;
			}



			WCHAR widename[MAX_PATH];
			ZeroMemory( widename, sizeof( WCHAR ) * MAX_PATH );
			ret = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, fxname, (int)strlen( fxname ), widename, MAX_PATH );
			_ASSERT( ret != 0 );
			LPD3DXBUFFER pErr;
			hr = D3DXCreateEffectFromFileW(
				m_pd3dDevice, widename, NULL, NULL, 
				D3DXSHADER_DEBUG , NULL, &g_pPostEffect, &pErr );
			if( hr != D3D_OK ){
				::MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
				DbgOut( "\n\nPostEffect File error !!! %x\n", hr );
				_ASSERT( 0 );
				return 1;
			}

/***
			LPD3DXBUFFER pErr;
			hr = D3DXCreateEffectFromFile(
				m_pd3dDevice, fxname, NULL, NULL, 
				D3DXSHADER_DEBUG , NULL, &g_pPostEffect, &pErr );
			if( hr != D3D_OK ){
				::MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
				DbgOut( "\n\nPostEffect File error !!! %x\n", hr );
				_ASSERT( 0 );
				return 1;
			}
***/


		}else{
				::MessageBox( NULL, "ハードウェアが要件を満たさないので実行できません。\n終了します。", "確認", MB_OK );
				DbgOut( "\n\nHardware too old error !!!\n" );
				_ASSERT( 0 );
				g_useGPU = 0;
				return 1;
		}

		ret = GetShaderHandle();
		if( ret ){
			DbgOut( "viewer : GetShaderHandle error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{
		g_pEffect->OnResetDevice();
		g_pPostEffect->OnResetDevice();
	}

	ret = SetEffectDecl( m_pd3dDevice );
	_ASSERT( !ret );



	if( m_createmotflag == 0 ){

	//effect
		//if( g_useGPU == 1 ){

		//}
//		m_dlgfont = new CFont();
//		if( !m_dlgfont ){
//			errormes.errorcode = E_OUTOFMEMORY;
//			errormes.type = MSGERR_APPMUSTEXIT;
//			errormes.mesptr = "dlgfont alloc error";
//			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
//			_ASSERT( 0 );
//			return -1;
//		}
//		m_dlgfont->CreatePointFont(9, _T("ＭＳ ゴシック"), NULL);

		g_motdlg = new CMotDlg( this, m_pd3dDevice, m_d3dpp, g_inidata->iklevelope );
		if( !g_motdlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MotDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		g_motdlg->Create( m_hWnd,
			m_rcmot,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			WS_EX_WINDOWEDGE
		);
		g_motdlg->SetAppWnd( m_hWnd );
		_ASSERT( g_motdlg );
		_ASSERT( g_motdlg->m_motparamdlg );
		g_motdlg->m_motparamdlg->m_mch = m_mcdlg->m_mch;
		

		m_wblddlg = new CWriteBldDlg( this, g_motdlg->m_motparamdlg );
		if( !m_wblddlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "WriteBldDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}


		m_graphwnd = new CGraphWnd( this );
		if( !m_graphwnd ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "GraphWnd alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		m_graphwnd->Create( m_hWnd,
			m_rcgraph,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			WS_EX_WINDOWEDGE
		);

		_ASSERT( m_graphwnd->m_grotdlg );
		_ASSERT( g_motdlg->m_camdlg );
		_ASSERT( g_motdlg->m_gpdlg );
		m_graphwnd->m_grotdlg->m_camdlg = g_motdlg->m_camdlg;//!!!!!!!!!!!
		m_graphwnd->m_grotdlg->m_gpdlg = g_motdlg->m_gpdlg;//!!!!!!!!!!!!!


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


		m_glowdlg = new CGlowViewDlg( m_pd3dDevice, this, g_inidata, g_lighthead );
		if( !m_glowdlg ){
			errormes.errorcode = E_OUTOFMEMORY;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "GlowViewDlg alloc error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
		m_glowdlg->Create( m_hWnd );


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

		if( g_inidata->graph_disp ){
			::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)7001, (LPARAM)isfirst );
			::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)7001, 0 );
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

		ret = m_glowdlg->CreateSwapChain( m_pd3dDevice );
		if( ret ){
			DbgOut( "viewer : restore : glowdlg CreateSwapChain error !!!\n" );
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "RestoreDeviceObjects glowdlg CreateSwapChain error";
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
//    m_viewport.Height = m_clientHeight;
//    m_viewport.Width =  m_clientWidth;
    m_viewport.Height = m_rc3dwnd.bottom - m_rc3dwnd.top;
    m_viewport.Width =  m_rc3dwnd.right - m_rc3dwnd.left;
  
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

	UV startuv = { 0.0f, 0.0f };
	UV enduv = { 1.0f, 1.0f };
	m_bgdisp = new CBgDisp2( 0, m_pd3dDevice, g_inidata->bg_tex1, 0, g_inidata->bg_tex2, 0, g_inidata->bg_mvu, g_inidata->bg_mvv, 
		g_inidata->bg_color, 0, g_proj_far, startuv, enduv, 
		g_inidata->bg_fixsize );
	if( !m_bgdisp ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : m_bgdisp alloc error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects m_bgdisp alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = m_dtri_sh->RestoreExtLineColor();
	if( ret ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : RestoreExtLineColor error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects RestoreExtLineColor error";
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
	if( ret || (m_sel_xno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "obj_Y", &m_sel_yno );
	if( ret || (m_sel_yno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "obj_Z", &m_sel_zno );
	if( ret || (m_sel_zno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "obj_CENTER", &m_sel_cenno );
	if( ret || (m_sel_cenno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "ring_X", &m_sel_ringxno );
	if( ret || (m_sel_ringxno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "ring_Y", &m_sel_ringyno );
	if( ret || (m_sel_ringyno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}
	ret = m_select_th->GetPartNoByName( "ring_Z", &m_sel_ringzno );
	if( ret || (m_sel_ringzno <= 0) ){
		DbgOut( "CD3DApplication : RestoreDeviceObjects : select : GetPartNoByName error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "RestoreDeviceObjects select : GetPartNoByName error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	if( (m_sel_xno <= 0) || (m_sel_yno <= 0) || (m_sel_zno <= 0) || (m_sel_cenno <= 0) || 
		(m_sel_ringxno <= 0) || (m_sel_ringyno <= 0) || (m_sel_ringzno <= 0) ){
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

	if( m_createshadowflag != 0 ){
		ret = RestoreShadowElem();
		_ASSERT( !ret );
	}

	ret = CreateCameraTexture();
	_ASSERT( !ret );

	ret = CreateNumTexture();
	_ASSERT( !ret );


	if( m_kinect ){
		m_kinect->m_papp = this;
		m_kinect->m_hwnd = m_hWnd;
		m_kinect->m_pdev = m_pd3dDevice;
	}

    return S_OK;
}

int CMyD3DApplication::RestoreShadowElem()
{
	int fmtok = 0;
	int rtsize;
	int okflag0 = 0;
	int okflag1 = 0;
	int okflag2 = 0;
	int okflag3 = 0;
	int okflag4 = 0;
	int okflag5 = 0;

	CheckRTFormat( D3DFMT_A16B16G16R16, &fmtok );
	if( fmtok != 0 ){
		int fmt0 = D3DFMT_A16B16G16R16;
		rtsize = 1024;
		CreateShadowElem( fmt0, rtsize, &okflag0 );
		if( !okflag0 ){
			rtsize = 512;
			CreateShadowElem( fmt0, rtsize, &okflag1 );
			if( !okflag1 ){
				rtsize = 256;
				CreateShadowElem( fmt0, rtsize, &okflag2 );
			}
		}
	}

	if( (fmtok == 0) || ((okflag0 == 0) && (okflag1 == 0) && (okflag2 == 0)) ){
		int fmt0 = D3DFMT_A8R8G8B8;
		rtsize = 1024;
		CreateShadowElem( fmt0, rtsize, &okflag3 );
		if( !okflag3 ){
			rtsize = 512;
			CreateShadowElem( fmt0, rtsize, &okflag4 );
			if( !okflag4 ){
				rtsize = 256;
				CreateShadowElem( fmt0, rtsize, &okflag5 );
				if( !okflag5 ){
					DbgOut( "RestoreShadowElem : CreateShadowElem error !!!\n" );
					::MessageBox( m_hWnd, "シャドウ用のテクスチャ作成に失敗しました。\n影は表示できません。", "エラー", MB_OK );
					_ASSERT( 0 );
				}
			}
		}
	}


	return 0;
}

int CMyD3DApplication::CreateShadowElem( int srcfmt, int rtsize, int* okflag )
{
	*okflag = 0;//!!!!!!!!!

	DestroyShadowElem();

	HRESULT hr;

	hr = m_pd3dDevice->CreateTexture(
		rtsize, rtsize, 1, 
		D3DUSAGE_RENDERTARGET, 
		(D3DFORMAT)srcfmt,
		D3DPOOL_DEFAULT, &(m_shadowelem.pTex), NULL );
	if( (hr != D3D_OK) || !m_shadowelem.pTex ){
		DbgOut( "CreateShadowElem : pdev CreateTexture error %d!!!\n", rtsize );
		_ASSERT( 0 );
		return 1;
	}

	
	hr = m_shadowelem.pTex->GetSurfaceLevel( 0, &(m_shadowelem.pBackBuffer) );
	if( (hr != D3D_OK) || !m_shadowelem.pBackBuffer ){
		DbgOut( "CreateShadowElem : tex GetSrufaceLevel error !!!\n" );
		_ASSERT( 0 );
		DestroyShadowElem();
		return 1;
	}

	m_shadowelem.width = rtsize;
	m_shadowelem.height = rtsize;


	D3DFORMAT fmt;
	fmt = D3DFMT_D16;
//	fmt = D3DFMT_D24S8;
//	fmt = D3DFMT_D32;
	hr = m_pd3dDevice->CreateDepthStencilSurface( rtsize, rtsize, fmt, 
		D3DMULTISAMPLE_NONE,
		0,
		FALSE, &(m_shadowelem.pZSurf), NULL ); 
	if( (hr != D3D_OK) || !m_shadowelem.pZSurf ){
		DbgOut( "CreateShadowElem : dev CreateDepthStencilSurface error %x !!!\n", hr );
		_ASSERT( 0 );
		DestroyShadowElem();
		return 1;
	}

	m_createshadowflag = 1;
	*okflag = 1;//!!!!!!!!!!!!!!!

	return 0;
}
int CMyD3DApplication::DestroyShadowElem()
{

	if( m_shadowelem.pZSurf ){
		m_shadowelem.pZSurf->Release();
		m_shadowelem.pZSurf = 0;
	}
	if( m_shadowelem.pBackBuffer ){
		m_shadowelem.pBackBuffer->Release();
		m_shadowelem.pBackBuffer = 0;
	}
	if( m_shadowelem.pTex ){
		m_shadowelem.pTex->Release();
		m_shadowelem.pTex = 0;
	}

	return 0;
}


int CMyD3DApplication::CheckRTFormat( int srcfmt, int* okflag )
{
	HRESULT hr;
    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

	hr = m_pD3D->CheckDeviceFormat( 
		m_dwAdapter,
		pDeviceInfo->DeviceType,
		pModeInfo->Format,
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_TEXTURE,
		(D3DFORMAT)srcfmt );

	if( hr == D3D_OK ){
		*okflag = 1;
	}else{
		*okflag = 0;
	}

	return 0;
}

int CMyD3DApplication::RestoreHandler( int needcalc, int dispwarnflag )
{
	int ret = 0;

	if( m_shandler ){




		ret = m_shandler->CreateTexture( m_pd3dDevice, m_hWnd, 1, g_miplevels, g_mipfilter, dispwarnflag, 0 );
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
//		ret = m_shandler->CreateMorphObj( m_pd3dDevice, m_mhandler );
//		if( ret ){
//			DbgOut( "CMyD3DApplication : RestoreHandler : m_shandler->CreateMorphObj error !!!\n" );
//			_ASSERT( 0 );
//			return -1;
//		}
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
		ret = m_ext_sh->CreateTexture( m_pd3dDevice, m_hWnd, 1, g_miplevels, g_mipfilter, dispwarnflag, 0 );
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
//		ret = m_ext_sh->CreateMorphObj( m_pd3dDevice, m_ext_mh );
//		if( ret ){
//			DbgOut( "CMyD3DApplication : RestoreExtHandler : m_shandler->CreateMorphObj error !!!\n" );
//			_ASSERT( 0 );
//			return -1;
//		}
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
		ret += m_g_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//		ret += m_g_sh->CreateMorphObj( m_pd3dDevice, m_g_mh );
//		if( ret ){
//			DbgOut( "CMyD3DApplication : RestoreHandler : m_g_sh->CreateMorphObj error !!!\n" );
//			_ASSERT( 0 );
//			return -1;
//		}
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

	ret += m_ground_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//	ret += m_ground_sh->CreateMorphObj( m_pd3dDevice, m_ground_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_ground_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
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
	ret += m_arrow_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//	ret += m_arrow_sh->CreateMorphObj( m_pd3dDevice, m_arrow_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_arrow_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
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
	ret += m_select_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//	ret += m_select_sh->CreateMorphObj( m_pd3dDevice, m_select_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_select_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
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
	ret += m_ikt_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//	ret += m_ikt_sh->CreateMorphObj( m_pd3dDevice, m_ikt_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_ikt_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
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
	ret += m_sph_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//	ret += m_sph_sh->CreateMorphObj( m_pd3dDevice, m_sph_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
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
///// ei3
	ret += m_ei3_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ei3_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ei3_sh->CreateDispObj( m_pd3dDevice, m_ei3_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_ei3_sh->SetTexTransparent();
	_ASSERT( !ret );
//	ret += m_ei3_sh->CreateMorphObj( m_pd3dDevice, m_ei3_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
	ret += m_ei3_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_sph_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}


///// bm
	ret += m_bm_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//	ret += m_bm_sh->CreateMorphObj( m_pd3dDevice, m_bm_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_bm_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
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
	ret += m_sq_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
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
//	ret += m_sq_sh->CreateMorphObj( m_pd3dDevice, m_sq_mh );
//	if( ret ){
//		DbgOut( "CMyD3DApplication : RestoreHandler : m_sq_sh->CreateMorphObj error !!!\n" );
//		_ASSERT( 0 );
//		return -1;
//	}
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


	ret += m_dtri_sh->CreateTexture( m_pd3dDevice, m_hWnd, 0, g_miplevels, g_mipfilter, 1, 0 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_dtri_sh->CreateTexture error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_dtri_sh->CreateDispObj( m_pd3dDevice, m_dtri_mh, 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_dtri_sh->CreateDispObj error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}
	ret += m_dtri_sh->SetTexTransparent();
	_ASSERT( !ret );
	ret += m_dtri_sh->SetDispFlag( 1 );
	if( ret ){
		DbgOut( "CMyD3DApplication : RestoreHandler : m_ground_sh->SetDispFlag error !!!\n" );
		_ASSERT( 0 );
		return -1;
	}

	return ret;
}
//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects( int resetflag, int flushname )
{
	int ret;
	ERRORMES errormes;

	if( g_pEffect ){
		g_pEffect->OnLostDevice();
	}
	if( g_pPostEffect ){
		HRESULT hres;
		hres = g_pPostEffect->OnLostDevice();
		_ASSERT( hres == D3D_OK );
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
	ret = InvalidateHandler( resetflag, flushname );
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
	if( m_glowdlg ){
		m_glowdlg->DestroySwapChain();
	}

	DestroyShadowElem();


    return S_OK;
}

int CMyD3DApplication::InvalidateHandler( int resetflag, int flushname )
{
	int ret = 0;

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	if( m_shandler ){
		ret += m_shandler->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_shandler->InvalidateTexture( resetflag );
		_ASSERT( !ret );
	}

	if( m_loadnamedlg && flushname ){
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

	if( m_dtri_sh ){
		ret += m_dtri_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_dtri_sh->InvalidateTexture( resetflag );
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
/// ei3
	if( m_ei3_sh ){
		ret += m_ei3_sh->InvalidateDispObj();
		_ASSERT( !ret );
		ret += m_ei3_sh->InvalidateTexture( resetflag );
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
	m_destroyflag = 1;

	if( g_motdlg ){
		g_motdlg->SelectMotParamTab();
	}


	CQuaternion2 tempq;
	tempq.DestroyTempQ();

	if( m_plugin ){
		delete [] m_plugin;
		m_plugin = 0;
	}
	if( m_kinect ){
		delete m_kinect;
		m_kinect = 0;
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
	ret = ::KillTimer( m_hWnd, m_timerid );
	_ASSERT( ret );


	RECT drect;
	HWND dhWnd;
	dhWnd = ::GetDesktopWindow();
	::GetWindowRect(dhWnd, &drect);
	int maxwinw;
	int maxwinh;
	maxwinw = drect.right - drect.left;
	maxwinh = drect.bottom - drect.top;

	RECT newrect;
	ret = ::GetWindowRect( m_hWnd, &newrect );

//	char chkmes[1024];
//	sprintf_s( chkmes, 1024, "FinalCleanUp : rect left %d, right %d, top %d, bottom %d",
//		newrect.left, newrect.right, newrect.top, newrect.bottom );
//	::MessageBoxA( m_hWnd, chkmes, "check", MB_OK );

	if( ret != 0 ){
		g_inidata->main_posx = max( 0, newrect.left );
		g_inidata->main_posy = max( 0, newrect.top );
		g_inidata->main_width =  newrect.right - newrect.left;
		g_inidata->main_height = newrect.bottom - newrect.top;
		g_inidata->main_width = min( g_inidata->main_width, maxwinw );
		g_inidata->main_height = min( g_inidata->main_height, maxwinh );
	}else{
		g_inidata->main_posx = 0;
		g_inidata->main_posy = 0;
		g_inidata->main_width = 800;
		g_inidata->main_height = 600;
		DbgOut( "viewer : FinalCleanUp : GetWindowRect error : SetDefault ini data !!!\r\n" );
	}
	if( m_sigeditwnd ){
		g_inidata->forbidDD = m_sigeditwnd->m_forbidDD;
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


//	if( m_cameradlg ){
//		m_cameradlg->DestroyWindow();
//		delete m_cameradlg;
//		m_cameradlg = 0;
//	}

	if( m_bonedlg ){
		m_bonedlg->ShowDlg( SW_HIDE );
		m_bonedlg->DestroyWindow();
		delete m_bonedlg;
		m_bonedlg = 0;
	}

	if( m_za4dlg ){
		m_za4dlg->ShowDlg( SW_HIDE );
		m_za4dlg->DestroyWindow();
		delete m_za4dlg;
		m_za4dlg = 0;
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

	if( m_Toon0Dlg ){
		m_Toon0Dlg->ShowDlg( SW_HIDE );
		m_Toon0Dlg->DestroyWindow();
		delete m_Toon0Dlg;
		m_Toon0Dlg = 0;
	}

	if( m_syncdlg ){
		m_syncdlg->ShowDlg( SW_HIDE );
		m_syncdlg->DestroyWindow();
		delete m_syncdlg;
		m_syncdlg = 0;
	}


	if( m_mcdlg ){
		m_mcdlg->ShowDlg( SW_HIDE );
		m_mcdlg->DestroyWindow();
		delete m_mcdlg;
		m_mcdlg = 0;
	}

	if( m_wmafdlg ){
		//m_wmafdlg->DestroyWindow();
		delete m_wmafdlg;
		m_wmafdlg = 0;
	}



	if( m_sigeditwnd ){
		m_sigeditwnd->DestroyWindow();
		delete m_sigeditwnd;
		m_sigeditwnd = 0;
	}

	if( m_bonelinedlg ){
		//m_bonelinedlg->DestroyWindow();
		delete m_bonelinedlg;
		m_bonelinedlg = 0;
	}

	if( m_loadnamedlg ){
		//m_loadnamedlg->DestroyWindow();
		delete m_loadnamedlg;
		m_loadnamedlg = 0;
	}

	if( m_wblddlg ){
		delete m_wblddlg;
		m_wblddlg = 0;
	}

DbgOut( "checkend : viewer : FinalCleanUp 0 \r\n" );

	if( g_motdlg ){
		g_motdlg->OnStop();
		Sleep( 150 );
		g_endapp = 1;//!!!!!!!!!!
		g_motdlg->m_motparamdlg->DestroyTimer();
		if( m_graphwnd ){
			m_graphwnd->m_grotdlg->DestroyTimer();
		}
		g_motdlg->DestroyWindow();
		delete g_motdlg;
		g_motdlg = 0;
	}

DbgOut( "checkend : viewer : FinalCleanUp 1 \r\n" );

	if( m_graphwnd ){
		m_graphwnd->DestroyWindow();
		delete m_graphwnd;
		m_graphwnd = 0;
	}

DbgOut( "checkend : viewer : FinalCleanUp 2 \r\n" );

//	if( g_motdlg ){
//		g_motdlg->OnStop();
//		//Sleep( 500 );
//		g_motdlg->DestroyWindow();
//		delete g_motdlg;
//		g_motdlg = 0;
//	}


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
	if( m_glowdlg ){
		m_glowdlg->ShowDlg( SW_HIDE );
		m_glowdlg->DestroyWindow();
		delete m_glowdlg;
		m_glowdlg = 0;
	}
	if( g_bonepropdlg ){
		g_bonepropdlg->ShowDlg( SW_HIDE );
		g_bonepropdlg->DestroyWindow();
		delete g_bonepropdlg;
		g_bonepropdlg = 0;
	}


	if( m_optdlg ){
		//m_optdlg->DestroyWindow();
		delete m_optdlg;
		m_optdlg = 0;
	}

//	if( m_parsdlg ){
//		delete m_parsdlg;
//		m_parsdlg = 0;
//	}


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

DbgOut( "checkend : viewer : FinalCleanUp 3 \r\n" );

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

DbgOut( "checkend : viewer : FinalCleanUp 4 \r\n" );

	DestroyHandler();
	DestroyGroundHandler();
	DestroyGHandler();
	DestroyExtHandler();

	CMotionInfo::DestroyTempMP();

DbgOut( "checkend : viewer : FinalCleanUp 5 \r\n" );

	if( g_texbnk ){
		delete g_texbnk;
		g_texbnk = 0;
	}
DbgOut( "checkend : viewer : FinalCleanUp 6 \r\n" );

	if( m_soundbank ){
		delete m_soundbank;
		m_soundbank = 0;
	}

DbgOut( "checkend : viewer : FinalCleanUp 7 \r\n" );

	if( m_HS ){
		delete m_HS;
		m_HS = 0;
	}

DbgOut( "checkend : viewer : FinalCleanUp 8 \r\n" );

	DestroyProgressBar();

DbgOut( "checkend : viewer : FinalCleanUp end \r\n" );


	//CloseDbgFile();

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
	m_dtri_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_dtri_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_dtri_th alloc error !!!\n" );
		return 1;
	}
	m_dtri_sh = new CShdHandler( m_dtri_th );
	if( !m_dtri_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : shandler alloc error !!!\n" );
		return 1;
	}
	m_dtri_mh = new CMotHandler( m_dtri_th );
	if( !m_dtri_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : mhandler alloc error !!!\n" );
		return 1;
	}

	m_dtri_sh->m_inRDBflag = 1;

//////////
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
////////// ei3
	m_ei3_th = new CTreeHandler2( TMODE_IMPORT | TMODE_MULT );
	if( !m_ei3_th ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler : m_ei3_th alloc error !!!\n" );
		return 1;
	}
	m_ei3_sh = new CShdHandler( m_ei3_th );
	if( !m_ei3_sh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler ei3 : shandler alloc error !!!\n" );
		return 1;
	}
	m_ei3_mh = new CMotHandler( m_ei3_th );
	if( !m_ei3_mh ){
		DbgOut( "CD3DMyApplication : CreateGroundHandler ei3 : mhandler alloc error !!!\n" );
		return 1;
	}

	m_ei3_sh->m_inRDBflag = 1;


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

	if( m_dtri_mh ){
		ret = m_dtri_mh->DestroyMotionObj();
	}

	if( m_dtri_th ){
		delete m_dtri_th;
		m_dtri_th = 0;
	}

	if( m_dtri_sh ){
		delete m_dtri_sh;
		m_dtri_sh = 0;
	}

	if( m_dtri_mh ){
		delete m_dtri_mh;
		m_dtri_mh = 0;
	}

////////
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
///// ei3
	if( m_ei3_mh ){
		ret = m_ei3_mh->DestroyMotionObj();
	}

	if( m_ei3_th ){
		delete m_ei3_th;
		m_ei3_th = 0;
	}

	if( m_ei3_sh ){
		delete m_ei3_sh;
		m_ei3_sh = 0;
	}

	if( m_ei3_mh ){
		delete m_ei3_mh;
		m_ei3_mh = 0;
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

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	if( g_motdlg ){
		ret = g_motdlg->SelectMotParamTab();
		_ASSERT( !ret );
		g_motdlg->ResetCamTargetBone();
		g_motdlg->InvalidateHandler();
	}
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

		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}


	if( m_shandler && g_texbnk ){
		ret = g_texbnk->ResetDirtyFlag();
		_ASSERT( !ret );

		//地面のテクスチャは解放しない。
		if( m_g_sh ){
			ret = m_g_sh->SetTextureDirtyFlag( -1 );
			_ASSERT( !ret );
		}

		ret = g_texbnk->DestroyNotDirtyTexture( 1 );
		_ASSERT( !ret );

		ret = g_texbnk->ResetDirtyFlag();
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
        HWND hwndMotStep   = ::GetDlgItem( hDlg, IDC_COMBO1 );

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
    HWND hwndMotStep         = ::GetDlgItem( hDlg, IDC_COMBO1 );

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

	ret += m_dtri_sh->UpdateVertexBuffer( 0 );
	_ASSERT( !ret );


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
int CMyD3DApplication::Add2Tree( char* newname, int serino, int srctype, int depth, int hasmotion, int createflag )
{
	int ret = 0;
	if( createflag ){
		int retseri;
		retseri = m_thandler->AddTree( newname, serino );
		if( retseri != serino ){
			_ASSERT( 0 );
			return 1;
		}
	}else{
		ret = (*m_thandler)( serino )->SetName( newname );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}
	ret = m_thandler->SetElemType( serino, srctype );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	//////////////
	if( createflag ){
		ret = m_shandler->AddShdElem( serino, depth );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}else{
		(*m_shandler)( serino )->type = srctype;
	}
	ret = m_shandler->SetClockwise( serino, 2 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	(*m_shandler)( serino )->notuse = 0;//!!!!!!!!!!!

	//////////////
	if( createflag ){
		ret = m_mhandler->AddMotionCtrl( serino, depth );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}	
	}else{
		(*m_mhandler)( serino )->type = srctype;
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

	if( g_inidata->opt_modlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( g_loadingflag ){
		ret = IDOK;
	}
	if( ret != IDOK ){
		return 0;
	}

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}


	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );

	//////////////////
	m_ExistData = FALSE;

	m_sigeditwnd->SaveForbidSelect();


	int saveparent;
	m_shandler->GetParent( srcconvno, &saveparent );
	if( saveparent <= 1 ){
		saveparent = 1;
	}

	///////////////////

	InvalidateHandler( 1, 0 );

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
		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
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
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}


	//////////////////
	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );


	m_sigeditwnd->SaveForbidSelect();


	int saveparent;
	m_shandler->GetParent( delno, &saveparent );
	if( saveparent <= 1 ){
		saveparent = 1;
	}

	///////////////////

	if( restoreflag ){
		m_ExistData = FALSE;

		InvalidateHandler( 1, 0 );

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

			ret = g_motdlg->RemakeMOEAH();
			if( ret ){
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

int CMyD3DApplication::DelMorph( int srcdel, int dlgflag )
{
	int ret;
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	int delseri;
	if( srcdel <= 0 ){
		delseri = m_sigeditwnd->m_selectedno;
	}else{
		delseri = srcdel;
	}

	if( delseri <= 0 ){
		return 0;
	}

	CShdElem* delelem;
	delelem = (*m_shandler)( delseri );
	if( delelem->type != SHDMORPH ){
		return 0;
	}
	CMorph* delmorph;
	delmorph = delelem->morph;

	CShdElem* parelem = (*m_shandler)( 1 );

	int saveparent;
	m_shandler->GetParent( delseri, &saveparent );
	if( saveparent <= 1 ){
		saveparent = 1;
	}

	if( dlgflag ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return 0;
		}
	}
	//////////////////
	m_ExistData = FALSE;

	if( m_mcdlg->m_firstmake ){
		ret = m_mcdlg->DestroyMCElem( -1 );
		if( ret ){
			DbgOut( "viewer : DelMorph : mcdlg DestroyMCElem all error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( g_motdlg )
		g_motdlg->InvalidateHandler();

	ret = m_mhandler->DestroyMotionObj();
	if( ret ){
		DbgOut( "viewer : DelMorph all : mh DestroyMotionObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = g_motdlg->RemakeMOEAH();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	////////////////////

	/////////////////////
	//baseとtargetを退避
	//morphからのドラッグ

	int tno;
	if( delmorph->m_targetnum >= 1 ){
		for( tno = (delmorph->m_targetnum - 1); tno >= 0; tno-- ){
			CShdElem* curtarelem = (delmorph->m_ptarget + tno)->selem;
			ret = curtarelem->LeaveFromChain();// child情報は、そのままで、チェインから離す。
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			ret = curtarelem->PutToChain( parelem, 1 );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			curtarelem->dispflag = 1;
		}
	}
	ret = delmorph->DeleteAllTarget();
	_ASSERT( !ret );


	CShdElem* baseelem = delmorph->m_baseelem;
	ret = baseelem->LeaveFromChain();// child情報は、そのままで、チェインから離す。
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = baseelem->PutToChain( parelem, 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	baseelem->dispflag = 1;
	baseelem->m_mtype = M_NONE;

	/////////////////////
	//morphをdestroy
	delelem->DestroyObjs();
	delelem->type = SHDDESTROYED;//!!!!!!!!!!!!!!!!!!

	CMotionCtrl* delmc;
	delmc = (*m_mhandler)( delseri );
	_ASSERT( delmc );
	delmc->type = SHDDESTROYED;//!!!!!!!!!!!!!!

	CTreeElem2* delte;
	delte = (*m_thandler)( delseri );
	_ASSERT( delte );
	delte->type = SHDDESTROYED;//!!!!!!!!!!!!!!!
	
	///////////////////////
	ret = m_shandler->CalcDepth();
	if( ret )
		return 1;
	
	ret = m_mhandler->CopyChainFromShd( m_shandler );
	if( ret )
		return 1;

	ret = m_mhandler->CalcDepth();
	if( ret )
		return 1;


	//////////////////////
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
				DbgOut( "viewer : DelMorph : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}
	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)saveparent );
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)saveparent );
	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );
	m_paintdlg->InitUndoBuf();

	g_motdlg->ResetMorphDlg();

	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::DelPart( int srcdel, int dlgflag )
{
	int ret;
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	int delseri;
	if( srcdel <= 0 ){
		delseri = m_sigeditwnd->m_selectedno;
	}else{
		delseri = srcdel;
	}

	if( delseri <= 0 ){
		return 0;
	}

	CShdElem* delelem;
	delelem = (*m_shandler)( delseri );
	if( delelem->type != SHDPART ){
		return 0;
	}
	if( delelem->serialno == 1 ){
		return 0;
	}

	int saveparent;
	m_shandler->GetParent( delseri, &saveparent );
	if( saveparent <= 1 ){
		saveparent = 1;
	}


	/////////////////////

	if( dlgflag ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return 0;
		}
	}


	//////////////////
	m_ExistData = FALSE;

	if( m_mcdlg->m_firstmake ){
		ret = m_mcdlg->DestroyMCElem( -1 );
		if( ret ){
			DbgOut( "viewer : DelPart : mcdlg DestroyMCElem all error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( g_motdlg )
		g_motdlg->InvalidateHandler();

	ret = m_mhandler->DestroyMotionObj();
	if( ret ){
		DbgOut( "viewer : DelPart : mh DestroyMotionObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = g_motdlg->RemakeMOEAH();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	////////////////////
	ret = m_shandler->DeletePart( delseri, m_thandler, m_mhandler );
	_ASSERT( !ret );

	//////////////////////
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
				DbgOut( "viewer : DelPart : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}
	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)saveparent );
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)saveparent );
	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );
	m_paintdlg->InitUndoBuf();

	m_ExistData = TRUE;

	return 0;


}

int CMyD3DApplication::DelDispObj( int srcdel, int dlgflag )
{
	int ret;
	int targetflag = 0;
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	int delseri;
	if( srcdel <= 0 ){
		delseri = m_sigeditwnd->m_selectedno;
	}else{
		delseri = srcdel;
	}

	if( delseri <= 0 ){
		return 0;
	}

	CShdElem* delelem;
	delelem = (*m_shandler)( delseri );
	if( (delelem->type != SHDPOLYMESH) && (delelem->type != SHDPOLYMESH2) ){
		return 0;
	}
	if( delelem->m_mtype == M_BASE ){
		::MessageBox( m_hWnd, "モーフのベースは削除できません。", "禁止", MB_OK );
		return 0;
	}

	int saveparent;
	m_shandler->GetParent( delseri, &saveparent );
	if( saveparent <= 1 ){
		saveparent = 1;
	}


	/////////////////////

	if( dlgflag ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return 0;
		}
	}


	//////////////////
	m_ExistData = FALSE;

	if( m_mcdlg->m_firstmake ){
		ret = m_mcdlg->DestroyMCElem( -1 );
		if( ret ){
			DbgOut( "viewer : DelDispObj : mcdlg DestroyMCElem all error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( g_motdlg )
		g_motdlg->InvalidateHandler();

	ret = m_mhandler->DestroyMotionObj();
	if( ret ){
		DbgOut( "viewer : DelDispObj : mh DestroyMotionObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = g_motdlg->RemakeMOEAH();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	////////////////////
	if( delelem->m_mtype == M_TARGET ){

		CShdElem* morphelem = (*m_shandler)( saveparent );
		_ASSERT( morphelem );
		if( morphelem->type != SHDMORPH ){
			_ASSERT( 0 );
			return 1;
		}
		CMorph* curmorph = morphelem->morph;
		_ASSERT( curmorph );

		ret = curmorph->DeleteMorphTarget( delelem );
		_ASSERT( !ret );

		targetflag = 1;
	}

	ret = m_shandler->DeleteDispObj( delseri, m_thandler, m_mhandler );
	_ASSERT( !ret );

	//////////////////////
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
				DbgOut( "viewer : DelDispObj : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}
	if( m_sigeditwnd ){
		m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
	}
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)saveparent );
	SendMessage( m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_SIGDLG, (LPARAM)saveparent );
	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );
	m_paintdlg->InitUndoBuf();

	if( targetflag ){
		g_motdlg->ResetMorphDlg();
	}
	m_ExistData = TRUE;

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
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	//////////////////
	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );


	m_sigeditwnd->SaveForbidSelect();


	if( restoreflag ){
		m_ExistData = FALSE;

		InvalidateHandler( 1, 0 );

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

			ret = g_motdlg->RemakeMOEAH();
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

		}
	}

	int firstparno = 1;
	ret = m_shandler->GetFirstParentNo( &firstparno );
	_ASSERT( !ret );
	CShdElem* firstselem = (*m_shandler)( firstparno );
	_ASSERT( firstselem );

	int serino = m_shandler->s2shd_leng;
	int srctype = SHDBALLJOINT;

	char newname[256];
	ZeroMemory( newname, 256 );


	m_thandler->Start( serino - 1 );

	int createcnt = 0;
	int addno;
	for( addno = 0; addno < addnum; addno++ ){
		int destseri = -1;
		ret = m_shandler->GetDestroyedSeri( &destseri );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		ZeroMemory( newname, 256 );
		sprintf_s( newname, 256, "追加ボールジョイント%d", newno );
		CVec3f temploc;
		temploc.x = 0.0f;
		temploc.y = 0.0f;
		temploc.z = 0.0f;

		CShdElem* curselem;
		int depth;

		if( destseri < 0 ){
			m_thandler->Begin();
			depth = m_thandler->GetCurDepth();

			ret = Add2Tree( newname, serino + createcnt, SHDBALLJOINT, depth, 1 );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			m_thandler->End();

			ret = ((*m_shandler)( serino + createcnt ))->SetJointLoc( &temploc );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
///////////////
			curselem = (*m_shandler)( serino + createcnt );
			_ASSERT( curselem );
			ret = curselem->LeaveFromChain();
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			ret = curselem->PutToChain( firstselem, 1 );
			if( ret ){
				DbgOut( "viewer : NewJoint : PutToChain error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = m_shandler->CalcDepth();
			if( ret ){
				DbgOut( "viewer : NewJoint : shandler CalcDepth error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			
			ret = m_mhandler->CopyChainFromShd( m_shandler );
			if( ret ){
				DbgOut( "viewer : NewJoint : mhandler CopyChainFromShd error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = m_mhandler->CalcDepth();
			if( ret ){
				DbgOut( "viewer : NewJoint : mhandler CalcDepth error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			createcnt++;
		}else{
			depth = 2;
			ret = Add2Tree( newname, destseri, SHDBALLJOINT, depth, 1, 0 );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			ret = ((*m_shandler)( destseri ))->SetJointLoc( &temploc );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
////////////////
			curselem = (*m_shandler)( destseri );
			_ASSERT( curselem );
			ret = curselem->LeaveFromChain();
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			ret = curselem->PutToChain( firstselem, 1 );
			if( ret ){
				DbgOut( "viewer : NewJoint : PutToChain error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = m_shandler->CalcDepth();
			if( ret ){
				DbgOut( "viewer : NewJoint : shandler CalcDepth error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			
			ret = m_mhandler->CopyChainFromShd( m_shandler );
			if( ret ){
				DbgOut( "viewer : NewJoint : mhandler CopyChainFromShd error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			ret = m_mhandler->CalcDepth();
			if( ret ){
				DbgOut( "viewer : NewJoint : mhandler CalcDepth error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
		newno++;

	}

	newpartno++;
	///////////////////

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

int CMyD3DApplication::ImportSigFile( char* srcfilename )
{
	m_materialdlg->ShowDlg( SW_HIDE );	

	if( g_motdlg ){
		g_motdlg->SelectMotParamTab();
	}


	int ret = 0;
	if( m_thandler || m_shandler || m_mhandler ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return 0;
		}
	}
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、Loadを行う。
		ret = LoadSigFile( srcfilename );
		return ret;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	m_sigeditwnd->SaveForbidSelect();

	int dlgret;
	CLDSigDlg dlg( srcfilename );
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

	InvalidateHandler( 1, 1 );

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

		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
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
	ret = sigfile->LoadSigFile( dlg.name, m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng, dlg.multvec, 0, offsetpos, rot12, 0 );
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
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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

	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );
	//ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );
	
	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, dlg.name );
	}

	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::LoadBLDFile( char* srcfilename )
{
	int ret;
	CBLDFile bldfile;

	ret = bldfile.LoadBLDFile( srcfilename );
	if( ret ){
		DbgOut( "viewer : LoadBLDFile : bldfile LoadBLDFile error !!!\n" );
		_ASSERT( 0 );
		::MessageBox( m_hWnd, "bldファイル読み込みでエラーになりました。", "エラー", MB_OK );
		ZeroMemory( m_wblddlg->m_bldname, sizeof( char ) * MAX_PATH );
		return 1;
	}
	strcpy_s( m_wblddlg->m_bldname, MAX_PATH, srcfilename );


	ret = LoadSigFile( bldfile.m_sigbld.name, 1, bldfile.m_sigbld.mult, 0 );
	if( ret ){
		DbgOut( "viewer : LoadBLDFile : LoadSigFile error !!!\n" );
		_ASSERT( 0 );
		::MessageBox( m_hWnd, "sigファイル読み込みでエラーになりました。", "エラー", MB_OK );
		ZeroMemory( m_wblddlg->m_signame, sizeof( char ) * MAX_PATH );
		return 1;
	}
	strcpy_s( m_wblddlg->m_signame, MAX_PATH, bldfile.m_sigbld.name );
	m_wblddlg->m_sigmult = bldfile.m_sigbld.mult;

	if( bldfile.m_gbld.flag == 1 ){
		ret = LoadGroundData( &(bldfile.m_gbld) );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : LoadGroundData error !!!\n" );
			_ASSERT( 0 );
			::MessageBox( m_hWnd, "地面ファイル読み込みでエラーになりました。", "エラー", MB_OK );
			ZeroMemory( m_wblddlg->m_gname, sizeof( char ) * MAX_PATH );
			return 1;
		}
		char wgname[MAX_PATH];
		ZeroMemory( wgname, sizeof( char ) * MAX_PATH );
		strcpy_s( wgname, MAX_PATH, bldfile.m_gbld.name );
		char* mqoptr;
		mqoptr = strstr( wgname, ".mqo" );
		if( mqoptr ){
			strcpy( mqoptr, ".sig" );
		}

		m_wblddlg->m_gflag = 1;
		strcpy_s( m_wblddlg->m_gname, MAX_PATH, wgname );
		if( mqoptr ){
			m_wblddlg->m_gmult = 1.0f;
		}else{
			m_wblddlg->m_gmult = bldfile.m_gbld.mult;
		}
	}else{
		ZeroMemory( m_wblddlg->m_gname, sizeof( char ) * MAX_PATH );
	}

	if( bldfile.m_moabld.flag == 1 ){
		ret = LoadMAFile( bldfile.m_moabld.name, 0 );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : LoadMAFile error !!!\n" );
			_ASSERT( 0 );
			::MessageBox( m_hWnd, "moaファイル読み込みでエラーになりました。", "エラー", MB_OK );
			ZeroMemory( m_wblddlg->m_moaname, sizeof( char ) * MAX_PATH );
			return 1;
		}

		strcpy_s( m_wblddlg->m_moaname, MAX_PATH, bldfile.m_moabld.name );
	}else{
		ZeroMemory( m_wblddlg->m_moaname, sizeof( char ) * MAX_PATH );
	}

	if( bldfile.m_cambld.flag == 1 ){
		ret = DestroyCameraAnim( 1 );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : DestroyCameraAnim error !!!\n" );
			_ASSERT( 0 );
			m_wblddlg->m_chkcam = 0;
			ZeroMemory( m_wblddlg->m_camname, sizeof( char ) * MAX_PATH );
			return 1;
		}

		ret = LoadCamera( bldfile.m_cambld.name );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : LoadCamera error !!!\n" );
			_ASSERT( 0 );
			::MessageBox( m_hWnd, "camファイル読み込みでエラーになりました。", "エラー", MB_OK );
			m_wblddlg->m_chkcam = 0;
			ZeroMemory( m_wblddlg->m_camname, sizeof( char ) * MAX_PATH );
			return 1;
		}
		if( bldfile.m_cambld.mode == 1 ){
			ret = g_motdlg->m_camdlg->SetCameraKAE();
			_ASSERT( !ret );
		}

		m_wblddlg->m_camflag = 1;
		m_wblddlg->m_chkcam = 1;
		strcpy_s( m_wblddlg->m_camname, MAX_PATH, bldfile.m_cambld.name );
		if( bldfile.m_cambld.mode == 1 ){
			m_wblddlg->m_cammode = 0;
		}else{
			m_wblddlg->m_cammode = 1;
		}
	}else{
		m_wblddlg->m_chkcam = 0;
		ZeroMemory( m_wblddlg->m_camname, sizeof( char ) * MAX_PATH );
	}

	if( bldfile.m_gpabld.flag == 1 ){
		ret = DestroyGPAnim( 1 );
		if( ret ){
			DbgOut( "viewere : LoadBLDFile : DestroyGPAnim error !!!\n" );
			_ASSERT( 0 );
			m_wblddlg->m_chkgpa = 0;
			ZeroMemory( m_wblddlg->m_gpaname, sizeof( char ) * MAX_PATH );
			return 1;
		}
		ret = LoadGP( bldfile.m_gpabld.name );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : LoadGP error !!!\n" );
			_ASSERT( 0 );
			::MessageBox( m_hWnd, "gpaファイル読み込みでエラーになりました。", "エラー", MB_OK );
			m_wblddlg->m_chkgpa = 0;
			ZeroMemory( m_wblddlg->m_gpaname, sizeof( char ) * MAX_PATH );
			return 1;
		}

		m_wblddlg->m_gpaflag = 1;
		m_wblddlg->m_chkgpa = 1;
		strcpy_s( m_wblddlg->m_gpaname, MAX_PATH, bldfile.m_gpabld.name );
	}else{
		m_wblddlg->m_chkgpa = 0;
		ZeroMemory( m_wblddlg->m_gpaname, sizeof( char ) * MAX_PATH );
	}


	if( bldfile.m_moebld.flag == 1 ){
		ret = DestroyMOEAnim( 1 );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : DestroyMOEAnim error !!!\n" );
			_ASSERT( 0 );
			m_wblddlg->m_chkmoe = 0;
			ZeroMemory( m_wblddlg->m_moename, sizeof( char ) * MAX_PATH );
			return 1;
		}
		ret = LoadMOE( bldfile.m_moebld.name );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : LoadMOE error !!!\n" );
			_ASSERT( 0 );
			::MessageBox( m_hWnd, "moeファイル読み込みでエラーになりました。", "エラー", MB_OK );
			m_wblddlg->m_chkmoe = 0;
			ZeroMemory( m_wblddlg->m_moename, sizeof( char ) * MAX_PATH );
			return 1;
		}
		m_wblddlg->m_moeflag = 1;
		m_wblddlg->m_chkmoe = 1;
		strcpy_s( m_wblddlg->m_moename, MAX_PATH, bldfile.m_moebld.name );

	}else{
		m_wblddlg->m_chkmoe = 0;
		ZeroMemory( m_wblddlg->m_moename, sizeof( char ) * MAX_PATH );
	}

	if( bldfile.m_ssbld.flag == 1 ){
		ret = LoadSndSetFile( bldfile.m_ssbld.name );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : LoadSndSetFile error !!!\n" );
			_ASSERT( 0 );

			m_wblddlg->m_chkssf = 0;
			ZeroMemory( m_wblddlg->m_ssfname, sizeof( char ) * MAX_PATH );

			if( m_soundbank ){
				delete m_soundbank;
				m_soundbank = 0;

				m_soundbank = new CSoundBank();
				if( !m_soundbank ){
					_ASSERT( 0 );
					return 1;
				}
			}

			g_motdlg->m_snddlg->RemakeSndAH();
			::MessageBox( m_hWnd, "ssfファイル読み込みでエラーになりました。", "エラー", MB_OK );
			return 1;
		}

		m_wblddlg->m_ssfflag = 1;
		m_wblddlg->m_chkssf = 1;
		strcpy_s( m_wblddlg->m_ssfname, MAX_PATH, bldfile.m_ssbld.name );

	}else{
		m_wblddlg->m_chkssf = 0;
		ZeroMemory( m_wblddlg->m_ssfname, sizeof( char ) * MAX_PATH );
	}

	if( bldfile.m_sabld.flag == 1 ){
		ret = LoadSndAnimFile( bldfile.m_sabld.name, 0 );
		if( ret ){
			DbgOut( "viewer : LoadBLDFile : LoadSndAnimFile error !!!\n" );
			_ASSERT( 0 );
			::MessageBox( m_hWnd, "safファイル読み込みでエラーになりました。", "エラー", MB_OK );

			m_wblddlg->m_chksaf = 0;
			ZeroMemory( m_wblddlg->m_safname, sizeof( char ) * MAX_PATH );

			return 1;
		}
		m_wblddlg->m_safflag = 1;
		m_wblddlg->m_chksaf = 1;
		strcpy_s( m_wblddlg->m_safname, MAX_PATH, bldfile.m_sabld.name );

	}else{
		m_wblddlg->m_chksaf = 0;
		ZeroMemory( m_wblddlg->m_safname, sizeof( char ) * MAX_PATH );
	}

	int dlgret;
	dlgret = (int)::MessageBox( m_hWnd, "ボーンマークの表示をオフにしますか？\nアニメを見るだけの人は通常オフにします。",
		"確認", MB_OKCANCEL );
	if( dlgret == IDOK ){
		m_optdlg->m_bonedisp = 0;
		g_inidata->opt_jointdisp = 0;

		m_optdlg->m_bmdisp = 0;
		g_inidata->opt_bmdisp = 0;
		g_motdlg->SetSaveBMDisp( 0 );
	}else{
		m_optdlg->m_bonedisp = 1;
		g_inidata->opt_jointdisp = 1;

		m_optdlg->m_bmdisp = 1;
		g_inidata->opt_bmdisp = 1;
		g_motdlg->SetSaveBMDisp( 1 );
	}
	SetBoneDisp();//savejdispはここでセット

	return 0;
}


int CMyD3DApplication::LoadSigFile( char* srcfilename, int multflag, float srcmult, int dlgwarnflag )
{
	m_materialdlg->ShowDlg( SW_HIDE );	


	int ret = 0;

	if( (m_thandler || m_shandler || m_mhandler) && (dlgwarnflag == 1) ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return -2;
		}
	}

	m_sigeditwnd->SaveForbidSelect();

	
	char ldname[MAX_PATH];
	ZeroMemory( ldname, sizeof( char ) * MAX_PATH );
	D3DXVECTOR3 ldmultvec = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	if( multflag == 0 ){
		int dlgret;
		CLDSigDlg dlg( srcfilename );
		dlgret = (int)dlg.DoModal();
		if( dlgret != IDOK )
			return -2;

		strcpy_s( ldname, MAX_PATH, dlg.name );
		ldmultvec = dlg.multvec;
	}else{
		strcpy_s( ldname, MAX_PATH, srcfilename );
		ldmultvec = D3DXVECTOR3( srcmult, srcmult, srcmult );
	}

	ERRORMES errormes;


////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler( 0, 1 );
	
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

	ret = SetRESDIR( ldname, m_shandler );
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
	ret = sigfile->LoadSigFile( ldname, m_thandler, m_shandler, m_mhandler, 0, ldmultvec, 0, offsetpos, rot13, 0 );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = ldname;
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
		CAMERAELEM inice;
		RotateVKCamera( ID_KEY_ALL_RESET, inice );

		//ChangeViewParam( ID_KEY_ALL_RESET );
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
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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

	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );

	//ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, ldname );
	}

	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::LoadESIFile( char* srcfilename )
{
	int dlgwarnflag = 1;
	m_materialdlg->ShowDlg( SW_HIDE );	
	int ret = 0;

	if( (m_thandler || m_shandler || m_mhandler) && (dlgwarnflag == 1) ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return -2;
		}
	}

	m_sigeditwnd->SaveForbidSelect();

	
	float ldmult = 1.0f;
	int dlgret;
	CLdEsiDlg dlg;
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK )
		return -2;
	ldmult = dlg.mult;



	ERRORMES errormes;
////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler( 0, 1 );
	
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

	ret = SetRESDIR( srcfilename, m_shandler );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadSigFile : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}


	CESIFile esifile;
	ret = esifile.LoadESIFile( m_pd3dDevice, srcfilename, m_thandler, m_shandler, m_mhandler, ldmult );
	if( ret ){
		if( ret == D3DAPPERR_MEDIANOTFOUND ){
			errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
			errormes.type = MSG_NONE;
			errormes.mesptr = srcfilename;
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		}else{
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "LoadESIFile : LoadESIFile error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			_ASSERT( 0 );
			return -1;
		}
	}

	if( m_shandler->s2shd_leng <= 1 ){
		DbgOut( "CMyD3DApplication : LoadESIFile : s2shd_leng <= 1\n" ); 
		
		DestroyHandler();
		if( g_motdlg )
			g_motdlg->InvalidateHandler();
		if( m_sigeditwnd )
			m_sigeditwnd->InvalidateHandler();
		CAMERAELEM inice;
		RotateVKCamera( ID_KEY_ALL_RESET, inice );

		//ChangeViewParam( ID_KEY_ALL_RESET );
		return 0;
	}

	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

/***
	//esifile内で呼ばれる！！！！！！！！！！！！！！！！！！！！

	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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
		DbgOut( "CMyD3DApplication : LoadESIFile : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadESIFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = RestoreHandler( 1 );
	_ASSERT( !ret );
***/

	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

	
	if( g_motdlg ){
		DbgOut( "CMyD3DApplication : LoadESIFile : motdlg->SetParams\n" );

		ret = g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : LoadESIFile : mcdlg InitMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}		

	}

	if( m_sigeditwnd ){
		DbgOut( "CMyD3DApplication : LoadESIFile : sigeditwnd->SetParams %x, %x, %x, leng %d\n",
			m_thandler, m_shandler, m_mhandler, m_shandler->s2shd_leng );

		ret = m_sigeditwnd->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd );
		if( ret ){
			DbgOut( "CMyD3DApplication : LoadESIFile : sigeditwnd->SetParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	

	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );

	//ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

//	if( m_loadnamedlg ){
//		strcpy_s( m_loadnamedlg->m_signame, 2048, ldname );
//	}

	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::externRestore()
{
	int ret;

	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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

	ERRORMES errormes;
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadESIFile : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadESIFile : CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	ret = RestoreHandler( 1 );
	_ASSERT( !ret );

	return 0;

}


//抽出読み込み
int CMyD3DApplication::ExtractBoneData()
{
	int ret = 0;

	if( !m_ExistData || !m_thandler || !m_shandler || !m_mhandler ){
		return 0;//!!!!!!!!
	}

	if( g_inidata->opt_modlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( g_loadingflag ){
		ret = IDOK;
	}
	if( ret != IDOK ){
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );

	m_sigeditwnd->SaveForbidSelect();


	char im2name[MAX_PATH];
	sprintf_s( im2name, MAX_PATH, "%stempim2.im2", m_autosavedir );
	CIM2File im2file;
	ret = im2file.WriteIM2File( im2name, 0, m_thandler, m_shandler );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	CExtSigDlg dlg;
	int dlgret = (int)dlg.DoModal();
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
	D3DXVECTOR3 sigmult = D3DXVECTOR3( dlg.mult, dlg.mult, dlg.mult );
	ret = sigfile->LoadSigFile( dlg.name, m_ext_th, m_ext_sh, m_ext_mh, 0, sigmult, 0, offsetpos, rot13, 0 );
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
	case COL_PPHONG:
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
	InvalidateHandler( 1, 0 );
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

		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

	}

	if( dlg.chkds == 1 ){
		//display switchのコピー
		int chks;
		for( chks = 0; chks < m_shandler->s2shd_leng; chks++ ){
			CShdElem* chkse = (*m_shandler)( chks );
			CTreeElem2* chkte = (*m_thandler)( chks );

			if( (chkse->type == SHDPOLYMESH2) || (chkse->type == SHDPOLYMESH2) ){
				int exts = -1;
				ret = m_ext_th->GetDispObjNoByName( chkte->name, &exts, m_ext_sh, 0 );
				if( !ret && (exts > 0) ){
					CShdElem* extse = (*m_ext_sh)( exts );
					chkse->dispswitchno = extse->dispswitchno;
				}
			}
		}
	}


	SERICONV* sericonv = 0;
	SAVEPOS* savepos = 0;
	int setno;

	if( dlg.chkbone ){
		//元のジョイントの位置を保存
		savepos = (SAVEPOS*)malloc( sizeof( SAVEPOS ) * curjointnum );
		if( !savepos ){
			DbgOut( "viewer : ExtractBoneData : savepos alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( savepos, sizeof( SAVEPOS ) * curjointnum );

		int csetno = 0;
		int cseri;
		for( cseri = 0; cseri < m_shandler->s2shd_leng; cseri++ ){
			CShdElem* cselem;
			cselem = (*m_shandler)( cseri );
			if( cselem->IsJoint() ){
				CTreeElem2* ctelem;
				ctelem = (*m_thandler)( cseri );
				strcpy_s( (savepos + csetno)->name, 256, ctelem->name );

				D3DXVECTOR3* dstpos;
				dstpos = &((savepos + csetno)->pos);
				CPart* cpart;
				cpart = cselem->part;
				if( cpart ){
					dstpos->x = cpart->jointloc.x;
					dstpos->y = cpart->jointloc.y;
					dstpos->z = cpart->jointloc.z;
				}else{
					dstpos->x = 0.0f;
					dstpos->y = 0.0f;
					dstpos->z = 0.0f;					
				}

//				D3DXMATRIX iniw;
//				D3DXMatrixIdentity( &iniw );
//				ret = m_shandler->GetBonePos( cseri, 0, -1, 0, &((savepos + csetno)->pos), m_mhandler, iniw, 1 );
//				if( ret ){
//					DbgOut( "viewer : ExtractBoneData : GetBonePos error !!!\n" );
//					_ASSERT( 0 );
//					return 1;
//				}
				
				csetno++;
			}
		}


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

			if( dlg.chknotjpos == 0 ){
				selem->part->jointloc = *locptr;
			}else{
				int findindex = -1;
				int saveno;
				for( saveno = 0; saveno < curjointnum; saveno++ ){
					int cmp0;
					cmp0 = strcmp( exttelem->name, (savepos + saveno)->name );
					if( cmp0 == 0 ){
						findindex = saveno;
						break;
					}
				}
				if( findindex >= 0 ){
					D3DXVECTOR3 cpos;
					cpos = (savepos + findindex)->pos;
					selem->part->jointloc.x = cpos.x;
					selem->part->jointloc.y = cpos.y;
					selem->part->jointloc.z = cpos.z;
				}else{
					selem->part->jointloc = *locptr;
				}

			}
			selem->notuse = extselem->notuse;//!!!!!!!!!

			selem->m_setmikobonepos = extselem->m_setmikobonepos;
			selem->m_mikobonepos = extselem->m_mikobonepos;


			selem->m_notransik = extselem->m_notransik;
			selem->m_ikskip = extselem->m_ikskip;
			selem->m_notsel = extselem->m_notsel;			

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
				//if( extparent->type == SHDPART ){
				if( extparent->IsJoint() == 0 ){
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
					//_ASSERT( 0 );
				}else if( extsister->type == SHDINFSCOPE ){
					selem->sister = firstparent;					
				}else{
					FindSeriByExtSeri( sericonv, extjointnum, extsister->serialno, &selem->sister );
					//_ASSERT( selem->sister );
					if( !selem->sister ){
						selem->sister = 0;
					}
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
		ret = im2file.LoadIM2File( im2name, m_thandler, m_shandler, m_mhandler );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		int dispno;
		for( dispno = 0; dispno < extdispnum; dispno++ ){
			seri = (dispconv + dispno)->seri;
			extseri = (dispconv + dispno)->extseri;

			if( (seri > 0) && (extseri > 0) ){
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

			}else{
				ret = m_shandler->CreateSkinMat( seri, 1 );
				if( ret ){
					DbgOut( "viewer : ExtractBoneData : sh CreateSkinMat error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
		}
		SendMessage( m_hWnd, WM_COMMAND, 6003, 2 );
	}

///////////
///////////

	goto extractboneaft;

extractboneaft:

	DestroyExtHandler();
	if( sericonv ){
		free( sericonv );
	}
	if( dispconv ){
		free( dispconv );
	}
	if( savepos ){
		free( savepos );
	}
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
	int firstparentno = 1;
	ret = m_shandler->GetFirstParentNo( &firstparentno );
	_ASSERT( !ret );
	int seri;

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



int CMyD3DApplication::LoadXFile( char* srcfilename )
{
	int ret = 0;

	if( m_thandler || m_shandler || m_mhandler ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return 0;
		}
	}

	m_sigeditwnd->SaveForbidSelect();


///////////

	int dlgret;
	CLdXDlg dlg( srcfilename );
	dlgret = (int)dlg.DoModal();
	if( dlgret != IDOK )
		return 0;


	ERRORMES errormes;


////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler( 0, 1 );
	
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
		CAMERAELEM inice;
		RotateVKCamera( ID_KEY_ALL_RESET, inice );

//		ChangeViewParam( ID_KEY_ALL_RESET );
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
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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


	ret = m_shandler->InvalidateDispObj();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

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
	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );
	
//	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, dlg.name );
	}

	ret = RemakeAllDispObj();
	_ASSERT( !ret );

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

	m_sigeditwnd->SaveForbidSelect();


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
		CAMERAELEM inice;
		RotateVKCamera( ID_KEY_ALL_RESET, inice );

//		ChangeViewParam( ID_KEY_ALL_RESET );
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
	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );

//	ChangeViewParam( ID_KEY_ALL_RESET );

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

int CMyD3DApplication::ImportMqoFile( LOADFILEINFO* ldinfo, char* srcfilename )
{
	m_materialdlg->ShowDlg( SW_HIDE );	

	int ret = 0;

	if( ldinfo == 0 ){
		if( m_thandler || m_shandler || m_mhandler ){
			if( g_inidata->opt_modlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
			}else if( g_inidata->opt_modlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( g_loadingflag ){
				ret = IDOK;
			}
			if( ret != IDOK ){
				return 0;
			}
		}
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );


	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、Loadを行う。
		ret = LoadMQOFile( ldinfo, 0 );
		return ret;
	}

	m_sigeditwnd->SaveForbidSelect();


	char buf[_MAX_PATH];
	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
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


	InvalidateHandler( 1, 1 );

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

		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
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
		
		CMQODlg dlg( g_inidata->mqo_mult, g_inidata->mqo_bonetype );
		ret = (int)dlg.DoModal();
		
		mqomult = dlg.m_mult;
		bonetype = dlg.m_bonetype;
		g_inidata->mqo_mult = dlg.m_mult;
		g_inidata->mqo_bonetype = dlg.m_bonetype;

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
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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
	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );

//	ChangeViewParam( ID_KEY_ALL_RESET );

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
int CMyD3DApplication::LoadGroundData( BLD* gbld )
{
	int ret;
	ERRORMES errormes;

	char ldname[MAX_PATH];
	ZeroMemory( ldname, sizeof( char ) * MAX_PATH );
	D3DXVECTOR3 ldmult = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	if( gbld == 0 ){

		CLdGroundDlg dlg;
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = (int)dlg.DoModal();		
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret != IDOK )
			return 0;

		strcpy_s( ldname, MAX_PATH, dlg.name );
		ldmult = D3DXVECTOR3( dlg.mult, dlg.mult, dlg.mult );

	}else{
		strcpy_s( ldname, MAX_PATH, gbld->name );
		ldmult = D3DXVECTOR3( gbld->mult, gbld->mult, gbld->mult );
	}
////////

DbgOut( "viewer : LoadGroundData : %s, mult %f\r\n", ldname, ldmult );

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

	ret = SetRESDIR( ldname, m_g_sh );
	if( ret ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "LoadGroundData : SetRESDIR error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return 1;
	}

/////////
	int ch = '.';
	char* extptr = 0;
	extptr = strrchr( ldname, ch );
	if( extptr ){
		int cmpsig, cmpmqo;
		cmpsig = strcmp( extptr, ".sig" );
		cmpmqo = strcmp( extptr, ".mqo" );


		if( cmpsig == 0 ){
			CSigFile* sigfile;
			sigfile = new CSigFile();
			_ASSERT( sigfile );
			D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
			D3DXVECTOR3 rot6( 0.0f, 0.0f, 0.0f );
			ret = sigfile->LoadSigFile( ldname, m_g_th, m_g_sh, m_g_mh, 0, ldmult, 0, offsetpos, rot6, 1 );
			if( ret ){
				errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "ground sig file open error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				delete sigfile;
				return -1;
			}
			delete sigfile;

DbgOut( "viewer : LoadGroundData : LoadSigFile : %s, %f\r\n", ldname, ldmult );
		}else if( cmpmqo == 0 ){
			CMQOFile mqofile( m_hWnd );
			D3DXVECTOR3 offsetpos( 0.0f, 0.0f, 0.0f );
			D3DXVECTOR3 rot6( 0.0f, 0.0f, 0.0f );
			ret = mqofile.LoadMQOFile( ldmult.x, ldname, m_g_th, m_g_sh, m_g_mh, 0, 1, BONETYPE_RDB2, 0, offsetpos, rot6 );
			if( ret ){
				errormes.errorcode = D3DAPPERR_MEDIANOTFOUND;
				errormes.type = MSGERR_APPMUSTEXIT;
				errormes.mesptr = "ground mqo file open error";
				SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
				_ASSERT( 0 );
				return -1;
			}
		}
	}
	m_g_sh->m_TLmode = TLMODE_D3D;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	ret = m_g_sh->CreateBoneInfo( m_g_mh );
	if( ret ){
		DbgOut( "CMyD3DApplication : LoadGroundData : ground : CreateBoneInfo error !!!\n" );
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "ground CreateBoneInfo error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		return -1;
	}	
	
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

int CMyD3DApplication::LoadMQOFile( LOADFILEINFO* ldinfo, char* srcfilename )
{
	m_materialdlg->ShowDlg( SW_HIDE );	


	int ret = 0;
	int ret2;

	m_sigeditwnd->SaveForbidSelect();

	char buf[_MAX_PATH];

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		if( ldinfo == 0 ){
			if( m_thandler || m_shandler || m_mhandler ){
				if( g_inidata->opt_modlg == CHKDLG_NES ){
					ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
				}else if( g_inidata->opt_modlg == CHKDLG_YES ){
					ret = IDOK;
				}else{
					ret = IDCANCEL;
				}
				if( g_loadingflag ){
					ret = IDOK;
				}
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
	}

	ERRORMES errormes;

	char* mqonameptr;
	if( ldinfo == 0 ){
		mqonameptr = buf;
	}else{
		mqonameptr = ldinfo->filename;
	}


////////////


	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );



	float mqomult;
	int bonetype;
	int mikoblendtype;

	if( ldinfo == 0 ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

		CMQODlg dlg( g_inidata->mqo_mult, g_inidata->mqo_bonetype );
		ret = (int)dlg.DoModal();
		if( ret != IDOK ){
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
			return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}

		mqomult = dlg.m_mult;
		bonetype = dlg.m_bonetype;
		g_inidata->mqo_mult = dlg.m_mult;
		g_inidata->mqo_bonetype = dlg.m_bonetype;

		if( bonetype == BONETYPE_MIKO ){
			CMikoBlendDlg dlg2;
			ret2 = (int)dlg2.DoModal();
			if( ret2 != IDOK ){
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
				return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
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

	m_ExistData = FALSE;

	InvalidateHandler( 0, 1 );
	
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
		CAMERAELEM inice;
		RotateVKCamera( ID_KEY_ALL_RESET, inice );

//		ChangeViewParam( ID_KEY_ALL_RESET );
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
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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
	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );

//	ChangeViewParam( ID_KEY_ALL_RESET );

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

int CMyD3DApplication::ImportRokFile( char* srcfilename )
{

	m_materialdlg->ShowDlg( SW_HIDE );	

	if( g_motdlg ){
		g_motdlg->SelectMotParamTab();
	}


	int ret = 0;

	if( m_thandler || m_shandler || m_mhandler ){

		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}
		if( ret != IDOK ){
			return 0;
		}
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		ret = LoadRokFile( srcfilename );
		return ret;
	}

	m_sigeditwnd->SaveForbidSelect();

///////////
	OPENFILENAME ofn;
	char buf[_MAX_PATH];

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
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


	InvalidateHandler( 0, 1 );
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

		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
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
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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
	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );

//	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, buf );
	}


	m_ExistData = TRUE;

	return 0;
}
int CMyD3DApplication::LoadRokFile( char* srcfilename )
{

	m_materialdlg->ShowDlg( SW_HIDE );	

	int ret;

	if( m_thandler || m_shandler || m_mhandler ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
		}
		if( g_loadingflag ){
			ret = IDOK;
		}

		if( ret != IDOK ){
			return 0;
		}
	}

	m_sigeditwnd->SaveForbidSelect();


///////////
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
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
	}

	ERRORMES errormes;

////////////

	m_ExistData = FALSE;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );


	InvalidateHandler( 0, 1 );
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
		CAMERAELEM inice;
		RotateVKCamera( ID_KEY_ALL_RESET, inice );

//		ChangeViewParam( ID_KEY_ALL_RESET );
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
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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
	CAMERAELEM inice;
	RotateVKCamera( ID_KEY_ALL_RESET, inice );

//	ChangeViewParam( ID_KEY_ALL_RESET );

	SetLight( 0 );
	SetBoneDisp();

	SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );

	if( m_loadnamedlg ){
		strcpy_s( m_loadnamedlg->m_signame, 2048, buf );
	}


	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::SaveGSigFile( char* filename )
{
	if( !m_g_th || !m_g_sh || !m_g_mh )
		return 1;

	if( !m_ExistData )
		return 1;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
	ERRORMES errormes;

	CSigFile* sigfile;
	sigfile = new CSigFile();
	if( !sigfile ){
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "SaveGSigFile : sigfile alloc error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	int ret;
	ret = sigfile->WriteSigFile( filename, m_g_th, m_g_sh, m_g_mh );
	if( ret ){
		delete sigfile;
		errormes.errorcode = -1;
		errormes.type = MSGERR_APPMUSTEXIT;
		errormes.mesptr = "WriteGSigFile error";
		SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
		_ASSERT( 0 );
		return -1;
	}

	delete sigfile;

	return 0;
}

int CMyD3DApplication::SaveSigFile( char* filename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;

	if( !m_ExistData )
		return 0;


	m_sigeditwnd->SaveForbidSelect();


	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	char buf[_MAX_PATH];
	//buf[0] = 0;
	strcpy_s( buf, _MAX_PATH, "default.sig" );

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

int CMyD3DApplication::RotateVKCamera( int keyflag, CAMERAELEM curcamera )
{
	//int ret = 0;
	//float rad;
	//float step;
	//D3DXMATRIX invview;
	//D3DXVECTOR3 upvec( 0.0f, 1.0f, 0.0f );
	//D3DXVECTOR3 newup;

	if( ( (m_lbutton == 1) && (m_movebone > 0) ) 
		|| ( (m_rbutton == 1) && (m_shiftbone > 0) ) 
	){
		return 0;
	}

	float degxz, degy;
	CamElem2Deg( curcamera, &degxz, &degy );
	degy = max( 90.005f, degy );
	degy = min( 269.995f, degy );
	float radxz, rady;
	D3DXVECTOR3 diff;

	int setflag = 0;
	switch( keyflag ){
	case ID_KEY_ALL_RESET:
		curcamera.looktype = CAML_NORMAL;
		curcamera.pos = D3DXVECTOR3( 0.0f, m_eye_y, -CAMDISTINITVAL );
		curcamera.target = D3DXVECTOR3( 0.0f, m_eye_y, 0.0f );
		curcamera.up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		curcamera.dist = CAMDISTINITVAL;
		curcamera.hsid = -1;
		curcamera.boneno = -1;
		curcamera.nearz = 100.0f;
		curcamera.farz = 40000.0f;
		curcamera.fov = 45.0f;
		curcamera.ortho = 0;
		curcamera.orthosize = 3000.0f;
		setflag = 1;
		break;
	case ID_KEY_90LEFT:
DbgOut( "check !!! RotateVKCamera : bef : degxz %f, degy %f\n", degxz, degy );
		if( degxz > 270.1f ){
			degxz = 270.0f;
		}else if( degxz > 180.1f ){
			degxz = 180.0f;
		}else if( degxz > 90.1f ){
			degxz = 90.0f;
		}else if( degxz > 0.1f ){
			degxz = 0.0f;
		}else{
			degxz = 270.0f;
		}
DbgOut( "check !!! RotateVKCamera : aft : degxz %f, degy %f\n", degxz, degy );
		radxz = degxz * (float)DEG2PAI;
		rady = degy * (float)DEG2PAI;
		curcamera.pos.x = sinf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.x;
		curcamera.pos.y = curcamera.target.y + curcamera.dist * sinf( rady );
		curcamera.pos.z = -cosf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.z;
		diff = curcamera.target - curcamera.pos;
		curcamera.dist = D3DXVec3Length( &diff );
		setflag = 1;
		break;

	case ID_KEY_90RIGHT:
		if( degxz < 90.0f ){
			degxz = 90.0f;
		}else if( degxz < 180.0f ){
			degxz = 180.0f;
		}else if( degxz < 270.0f ){
			degxz = 270.0f;
		}else if( degxz < 360.0f ){
			degxz = 0.0f;
		}else{
			degxz = 90.0f;
		}
		radxz = degxz * (float)DEG2PAI;
		rady = degy * (float)DEG2PAI;
		curcamera.pos.x = sinf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.x;
		curcamera.pos.y = curcamera.target.y + curcamera.dist * sinf( rady );
		curcamera.pos.z = -cosf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.z;
		diff = curcamera.target - curcamera.pos;
		curcamera.dist = D3DXVec3Length( &diff );
		setflag = 1;
		break;

	case ID_KEY_90UP:
		if( degy < 90.005f ){
			degy = 90.005f;
		}else if( degy < 180.0f ){
			degy = 180.0f;
		}else if( degy < 269.995f ){
			degy = 269.955f;
		}
		radxz = degxz * (float)DEG2PAI;
		rady = degy * (float)DEG2PAI;
		curcamera.pos.x = sinf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.x;
		curcamera.pos.y = curcamera.target.y + curcamera.dist * sinf( rady );
		curcamera.pos.z = -cosf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.z;
		diff = curcamera.target - curcamera.pos;
		curcamera.dist = D3DXVec3Length( &diff );
		setflag = 1;
		break;
	case ID_KEY_90DOWN:
		if( degy > 269.995f ){
			degy = 269.995f;
		}else if( degy > 180.0f ){
			degy = 180.0f;
		}else if( degy > 90.005f ){
			degy = 90.005f;
		}
		radxz = degxz * (float)DEG2PAI;
		rady = degy * (float)DEG2PAI;
		curcamera.pos.x = sinf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.x;
		curcamera.pos.y = curcamera.target.y + curcamera.dist * sinf( rady );
		curcamera.pos.z = -cosf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.z;
		diff = curcamera.target - curcamera.pos;
		curcamera.dist = D3DXVec3Length( &diff );
		setflag = 1;
		break;
	case ID_KEY_SYMMX:
		degxz = 360.0f - degxz;
		if( degxz > 360.0f )
			degxz -= 360.0f;
		if( degxz < 0.0f )
			degxz += 360.0f;
		curcamera.target.x *= -1.0f;

		radxz = degxz * (float)DEG2PAI;
		rady = degy * (float)DEG2PAI;
		curcamera.pos.x = sinf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.x;
		curcamera.pos.y = curcamera.target.y + curcamera.dist * sinf( rady );
		curcamera.pos.z = -cosf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.z;
		diff = curcamera.target - curcamera.pos;
		curcamera.dist = D3DXVec3Length( &diff );
		setflag = 1;
		break;
	default:
		break;
	}

	m_targetpos = curcamera.target;
	m_eye_y = curcamera.target.y;
	m_vEyePt = curcamera.pos;
	m_camup = curcamera.up;
	m_proj_near = curcamera.nearz;
	m_proj_far = curcamera.farz;
	m_fov = curcamera.fov;
	m_ortho = curcamera.ortho;
	m_orthosize = curcamera.orthosize;

	CamElem2Deg( curcamera, &m_degxz, &m_degy );
	m_degy = max( 90.005f, m_degy );
	m_degy = min( 269.995f, m_degy );

	if( setflag ){
		g_motdlg->m_camdlg->SetCurCamera( curcamera );
	}

	return 0;
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

	CBgDlg dlg( g_inidata->bg_tex1, g_inidata->bg_tex2, g_inidata->bg_mvu, g_inidata->bg_mvv, g_inidata->bg_color, 
		g_inidata->bg_fixsize );
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
	g_inidata->bg_fixsize = dlg.m_fixsize;

	UV startuv = { 0.0f, 0.0f };
	UV enduv = { 1.0f, 1.0f };

	m_bgdisp = new CBgDisp2( 0, m_pd3dDevice, g_inidata->bg_tex1, 0, g_inidata->bg_tex2, 0, g_inidata->bg_mvu, g_inidata->bg_mvv, 
		g_inidata->bg_color, 0, g_proj_far, startuv, enduv, 
		g_inidata->bg_fixsize );
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
		g_motdlg->SetSaveJointDisp( 1 );

	}else{
		ret1 = m_shandler->SetDispFlag( 0 );
		if( ret1 ){
			_ASSERT( 0 );
		}
		g_motdlg->SetSaveJointDisp( 0 );
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
		ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
		ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
		ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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

int CMyD3DApplication::PickVertSelDisp( int seldisp, WORD srcx, WORD srcy, int* pickpart, int* pickvert, int* pickvertnum )
{
	*pickpart = -1;
	*pickvertnum = 0;


	if( !m_thandler || !m_shandler || !m_mhandler )
		return -1;
	if( m_ExistData == FALSE )
		return -1;
	if( !g_motdlg )
		return -1;

	int ret;


	int part, vert;
	int getnum = 0;
	int partarray[PICKVERTMAX];
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
	calaspect = (float)dwClipWidth / (float)dwClipHeight;

	//ret = m_shandler->PickVert3( chkcampos, m_mhandler, srcx, srcy, 5, 5, partarray, vertarray, PICKVERTMAX, &getnum );
	ret = m_shandler->PickVertSelDisp( seldisp, m_mhandler, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX,
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
			ret = m_shandler->ChkConfLineAndFaceSelDisp( seldisp, m_clientWidth, m_clientHeight, 0, m_mhandler,
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
		*pickpart = part;

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
		ret = m_shandler->GetScreenPos( 0, m_mhandler, part, vert, 0, &shiftscx, &shiftscy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
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
		ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
		ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
		ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
			ret = m_shandler->GetScreenPos( 0, m_mhandler, *pickpart, *pickvert, 0, &shiftscx, &shiftscy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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

		if( selem->IsJoint() && (selem->type != SHDMORPH) && (selem->m_iktarget == 1) ){
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_xno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_yno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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
	ret = m_select_sh->GetScreenPos( 0, m_select_mh, m_sel_zno, -1, 1, &scx, &scy, dwClipWidth, dwClipHeight, m_select_mat, m_matView, m_matProjX );
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

int CMyD3DApplication::PickSelect( WORD srcx, WORD srcy, int* selectptr )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_ExistData == FALSE )
		return 0;
	if( !g_motdlg )
		return 0;
	int ret;

    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;
	*selectptr = SELECT_NONE;//!!!!!!!!!!

	float calaspect;
	calaspect = (float)dwClipWidth / (float)dwClipHeight;
			
	CalcSelectMat();
	ret = m_select_sh->ChkInView( m_select_mh, &m_select_mat, &m_matView, &m_matProjX );

	float maxdist = m_proj_far + 10000.0f;
	int cenpart = -1;
	int cenface = -1;
	D3DXVECTOR3 cenpos, cenn;
	float cendist = 0.0f;
	ret = m_select_sh->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_select_mh, m_select_mat, m_matView, m_matProjX,
		&m_sel_cenno, 1, (int)srcx, (int)srcy, &cenpart, &cenface, &cenpos, &cenn, &cendist, 1 );
	_ASSERT( !ret );
	

	if( cenpart == m_sel_cenno ){
		*selectptr = SELECT_CENTER;
	}else{
		int objpart[6];
		objpart[0] = m_sel_xno;
		objpart[1] = m_sel_ringxno;
		objpart[2] = m_sel_yno;
		objpart[3] = m_sel_ringyno;
		objpart[4] = m_sel_zno;
		objpart[5] = m_sel_ringzno;

		int ringpart = -1;
		int ringface = -1;
		D3DXVECTOR3 ringpos, ringn;
		float ringdist;
		ret = m_select_sh->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_select_mh, m_select_mat, m_matView, m_matProjX,
			objpart, 6, (int)srcx, (int)srcy, &ringpart, &ringface, &ringpos, &ringn, &ringdist, 1 );
		_ASSERT( !ret );
		if( (ringpart == m_sel_xno) || (ringpart == m_sel_ringxno) ){
			*selectptr = SELECT_X;
		}else if( (ringpart == m_sel_yno) || (ringpart == m_sel_ringyno) ){
			*selectptr = SELECT_Y;
		}else if( (ringpart == m_sel_zno) || (ringpart == m_sel_ringzno) ){
			*selectptr = SELECT_Z;
		}
	}

	if( *selectptr == SELECT_NONE ){
		return 0;
	}else{
		return 1;
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
	calaspect = (float)dwClipWidth / (float)dwClipHeight;

	int pickno;
	if( g_motdlg->GetSellockno() == 0 ){
		pickno = m_shandler->PickBone( m_mhandler, (int)srcx, (int)srcy, dwClipWidth, dwClipHeight, m_matWorld, m_matView, m_matProjX, calaspect );

		if( (pickno > 0) && m_sigeditwnd ){
			int forbidflag;
			forbidflag = m_sigeditwnd->ForbidSelect( pickno );
			if( forbidflag ){
				pickno = 0;
			}
		}

	}else{
		pickno = g_motdlg->GetSellockno();
	}

	float mag = 1e8;

	if( pickno > 0 ){
		*selectptr = SELECT_CENTER;
		//return pickno;
		//m_selectedno = pickno;

		//CShdElem* shiftbone = (*m_shandler)( m_selectedno );

//int CShdHandler::GetScreenPos3( 0, CMotHandler* lpmh, int srcseri, int vertno, int calcmode, int* scxptr, int* scyptr, float* sczptr, 
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
		ret = m_shandler->GetScreenPos3( 0, m_mhandler, pickno, 0, 1, &shiftscx, &shiftscy, &shiftscz, dwClipWidth, dwClipHeight,
			m_matWorld, m_matView, m_matProjX );
		if( ret != 0 )
			return 0;//!!!!!!!!!

		mag = (float)( (srcx - shiftscx) * (srcx - shiftscx) + (srcy - shiftscy) * (srcy - shiftscy) );

		offsetptr->x = (int)( srcx - shiftscx );//!!!!!!!!!!!!!!!!!!!!!!!
		offsetptr->y = (int)( srcy - shiftscy );

	}



/////////
	int shiftflag = 0;
	//GetKeyboardState( g_KeyBuffer );
	if( g_KeyBuffer[VK_SHIFT] & 0x80 ){
		shiftflag = 1;
	}else{
		shiftflag = 0;
	}


/////////////
	if( (selaxis == 1) && (shiftflag == 0) && (m_selectedno > 0) ){
			
		CalcSelectMat();

		ret = m_select_sh->ChkInView( m_select_mh, &m_select_mat, &m_matView, &m_matProjX );

//int PickFace( float maxdist, DWORD dwWidth, DWORD dwHeight, int groundflag, CMotHandler* srclpmh, D3DXMATRIX matWorld, D3DXMATRIX matView, 
//D3DXMATRIX matProj, int* partarray, int partnum, int pos2x, int pos2y, int* partptr, int* faceptr, 
//D3DXVECTOR3* pos3ptr, D3DXVECTOR3* nptr, float* distptr, int calcmode );
		float maxdist = m_proj_far + 10000.0f;
		int cenpart = -1;
		int cenface = -1;
		D3DXVECTOR3 cenpos, cenn;
		float cendist = 0.0f;
		//m_clientWidth, m_clientHeight
		ret = m_select_sh->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_select_mh, m_select_mat, m_matView, m_matProjX,
			&m_sel_cenno, 1, (int)srcx, (int)srcy, &cenpart, &cenface, &cenpos, &cenn, &cendist, 1 );
		_ASSERT( !ret );
		
		if( cenpart == m_sel_cenno ){
			*selectptr = SELECT_CENTER;
			pickno = m_selectedno;


		}else{
			int objpart[6];
			objpart[0] = m_sel_xno;
			objpart[1] = m_sel_ringxno;
			objpart[2] = m_sel_yno;
			objpart[3] = m_sel_ringyno;
			objpart[4] = m_sel_zno;
			objpart[5] = m_sel_ringzno;

			int ringpart = -1;
			int ringface = -1;
			D3DXVECTOR3 ringpos, ringn;
			float ringdist;
			ret = m_select_sh->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_select_mh, m_select_mat, m_matView, m_matProjX,
				objpart, 6, (int)srcx, (int)srcy, &ringpart, &ringface, &ringpos, &ringn, &ringdist, 1 );
			_ASSERT( !ret );
			if( (ringpart == m_sel_xno) || (ringpart == m_sel_ringxno) ){
				*selectptr = SELECT_X;
			}else if( (ringpart == m_sel_yno) || (ringpart == m_sel_ringyno) ){
				*selectptr = SELECT_Y;
			}else if( (ringpart == m_sel_zno) || (ringpart == m_sel_ringzno) ){
				*selectptr = SELECT_Z;
			}
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
//int CShdHandler::GetScreenPos3( 0, CMotHandler* lpmh, int srcseri, int vertno, int calcmode, int* scxptr, int* scyptr, float* sczptr, 
//							   DWORD dwClipWidth, DWORD dwClipHeight, D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj )

			int shiftscx, shiftscy;
			float shiftscz;
			ret = m_shandler->GetScreenPos3( 0, m_mhandler, m_selectedno, 0, 1, &shiftscx, &shiftscy, &shiftscz, dwClipWidth, dwClipHeight,
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

    ::GetClientRect(m_hWnd, &rcClient); 
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
		::DestroyWindow( m_pbwnd );
		m_pbwnd = 0;
	}

	return 0;
}

int CMyD3DApplication::WriteMQOFile( char* srcfilename )
{
	if( !m_thandler || !m_shandler ){
		::MessageBox( m_hWnd, "データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
		return 0;
	}

	CWriteMQODlg dlg( srcfilename );
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

	KEYFRAMENO* keyframe;
	keyframe = (KEYFRAMENO*)malloc( sizeof( KEYFRAMENO ) * (rangemax + 1) );
	if( !keyframe ){
		DbgOut( "viewer : WriteMQOFile : keyframe alloc error !!!\n" );
		_ASSERT( 0 );
		delete mqofile;
		return 1;
	}
	ZeroMemory( keyframe, sizeof( KEYFRAMENO ) * (rangemax + 1) );

	int keynum = 0;
//	ret = g_motdlg->GetKeyFrameNo( g_motdlg->GetMotCookie(), keyframe, &keynum );//motid 0
//	if( ret ){
//		DbgOut( "viewer : WriteMQOFile : GetKeyFrameNo error !!!\n" );
//		_ASSERT( 0 );
//		delete mqofile;
//		free( keyframe );
//		return 1;
//	}
	ret = m_mhandler->GetKeyframeNo( g_motdlg->GetMotCookie(), 0, keyframe, rangemax + 1, &keynum );
	if( ret ){
		DbgOut( "viewer : WriteMQOFile : mh GetKeyframeNo error !!!\n" );
		_ASSERT( 0 );
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

			ret = mqofile->SaveMQOFileOnFrame( dlg.m_chkmorph, g_motdlg->GetMotCookie(), frameno, outname, m_thandler, m_shandler, m_mhandler, dlg.m_mult, dlg.m_outbone );
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
			frameno = ( keyframe + saveno )->frameno;

			rate = frameno * 100 / rangemax;
			::SendMessage( m_hWnd, WM_USER_SET_PB, (WPARAM)rate, 0 );


			char strframe5[10];
			ZeroMemory( strframe5, sizeof( char ) * 10 );
			Conv5Digits( frameno, strframe5 );
			sprintf_s( outname, 4098, "%s_%s.mqo", dlg.m_head, strframe5 );

			ret = mqofile->SaveMQOFileOnFrame( dlg.m_chkmorph, g_motdlg->GetMotCookie(), frameno, outname, m_thandler, m_shandler, m_mhandler, dlg.m_mult, dlg.m_outbone );
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
		ret = WriteBoneFile( 0 );
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
		::MessageBox( m_hWnd, "データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
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

	//::MessageBox( m_hWnd, dlg.m_parent, dlg.m_mafname, MB_OK );

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


int CMyD3DApplication::WriteBoneFile( char* srcfilename )
{
	if( !m_thandler || !m_shandler ){
		::MessageBox( m_hWnd, "データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
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

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		strcpy_s( buf, _MAX_PATH, "default.bon" );
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
	ret = bonefile->WriteBoneFile( buf, m_thandler, m_shandler, bfdlg->m_bftype, bfdlg->m_relativeflag, bfdlg->m_mqoflag );
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

int CMyD3DApplication::LoadMAFile( char* srcfilename, int dlgwarnflag )
{
	if( !m_thandler || !m_shandler ){
		::MessageBox( m_hWnd, "形状データが読み込まれていないので、\nmoaを入力できません。", "入力できません", MB_OK );
		return 0;
	}

	int ret;
	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );


	int ret2 = 0;
	if( dlgwarnflag == 1 ){
		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret2 = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret2 = IDOK;
		}else{
			ret2 = IDCANCEL;
		}
	}else{
		ret2 = IDOK;
	}
	if( g_loadingflag ){
		ret2 = IDOK;
	}

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	if( ret2 != IDOK ){						
		return 0;//!!!!!!!!!!!
	}else{
		ret2 = DestroyMotion( 1 );
		if( ret ){
			DbgOut( "viewer : LoadMAFile : DestroyMotion error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

//		ret = g_motdlg->RemakeMOEAH();
//		if( ret ){
//			_ASSERT( 0 );
//			return 1;
//		}

	}


	char buf[_MAX_PATH];
	OPENFILENAME ofn;
	buf[0] = 0;

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
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

		DestroyMotion( 2 );

		return 1;
	}


	//motparamdlg->SetParamsで作成された、空モーション(先頭モーション)を削除する。
	if( (g_motdlg->GetMotionNum() >= 2) && (mafile->m_loadtrunknum >= 1) ){

		ret = g_motdlg->SelectCurrentMotion( 0 );
		if( ret ){
			DbgOut( "viewer : LoadMAFile : SelectCurrentMotion error !!!\n" );
			_ASSERT( 0 );
			delete mafile;

			DestroyMotion( 2 );

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

	ret = m_mcdlg->InitMC();// これをしないと「モ」ボタンを１回も押さないで上書き保存した場合にアイドリングモーションが設定されない。
	if( ret ){
		DbgOut( "viewer : LoadMAFile : mcdlg InitMC error !!!\n" );
		_ASSERT( 0 );

		DestroyMotion( 2 );

		return 1;
	}


	return 0;
}


int CMyD3DApplication::LoadBoneFile( LOADFILEINFO* ldinfo )
{

	if( !m_thandler || !m_shandler ){
		::MessageBox( m_hWnd, "形状データが読み込まれていないので、\nbonファイルを入力できません。", "入力できません", MB_OK );
		return 0;
	}

	int ret;
	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	m_sigeditwnd->SaveForbidSelect();


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

	InvalidateHandler( 1, 0 );

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

		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
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

//		m_cameradlg->GetWindowRect( &newrect );
//		g_inidata->camdlg_posx = newrect.left;
//		g_inidata->camdlg_posy = newrect.top;
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


int CMyD3DApplication::WriteIM2File( char* srcfilename )
{

	if( !m_thandler || !m_shandler ){
		::MessageBox( m_hWnd, "形状データが読み込まれていないので、\n出力できません。", "出力できません", MB_OK );
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


	CWriteIM2Dlg dlg( srcfilename );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	ret = (int)dlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( ret != IDOK ){
		return 0;
	}


	//if( (dlg.m_formqo == 0) && (m_shandler->m_im2enableflag == 0) ){
	//	::MessageBox( m_hWnd, "出力できません。\n詳しくは、http://www5d.biglobe.ne.jp/~ochikko/rdb2_im2file.htm をご覧ください。", "出力できません", MB_OK );
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

int CMyD3DApplication::LoadIM2File( int reloadflag, char* srcfilename )
{
	if( !m_thandler || !m_shandler ){
		::MessageBox( m_hWnd, "形状データが読み込まれていないので、\nim2ファイルを入力できません。", "入力できません", MB_OK );
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
		::MessageBox( m_hWnd, "im2ファイルを入力できません。\n詳しくは、http://www5d.biglobe.ne.jp/~ochikko/rdb2_im2file.htm をご覧ください。", "入力できません", MB_OK );
		return 0;
	}


	if( srcfilename && *srcfilename ){
		CIM2File im2file;

		ret = im2file.LoadIM2File( srcfilename, m_thandler, m_shandler, m_mhandler );
		if( ret ){
			DbgOut( "viewer : LoadIM2File : bimfile LoadIM2File error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( m_loadnamedlg ){
			strcpy_s( m_loadnamedlg->m_im2name, 2048, srcfilename );
		}

	}else{	
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
	}

	ret = m_shandler->CreateSkinMat( -1, 1 );
	if( ret ){
		DbgOut( "viewer : LoadIM2File : sh CreateSkinMat error !!!\n" );
		_ASSERT( 0 );
		return 1;
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
		g_motdlg->SetSaveBMDisp( 0 );

	}else{
		m_optdlg->m_bonedisp = 1;
		g_inidata->opt_jointdisp = 1;

		m_optdlg->m_bmdisp = 1;
		g_inidata->opt_bmdisp = 1;
		g_motdlg->SetSaveBMDisp( 1 );
	}
	SetBoneDisp();//savejdispはここでセット

	return 0;
}

int CMyD3DApplication::CopyUndoBuffer( int cpframe )
{
	int ret;
	if( g_motdlg ){
		ret = g_motdlg->CopyUndoBuffer( cpframe );
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
	//step = 2.0f * (float)PAI / 2500.0f;

	delta = diffx * step;

	*dstdelta = delta;

//DbgOut( "viewer : GetRotateDelta : befpos %d %d, aftpos %d %d, delta %f\r\n",
//	   befpos.x, befpos.y,
//	   aftpos.x, aftpos.y,
//	   delta );

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
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_4, MAKELONG(m_tbb_4, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_FACE, MAKELONG(m_tbb_face, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_LINE, MAKELONG(m_tbb_line, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_POINT, MAKELONG(m_tbb_point, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_TEN2, MAKELONG(m_tbb_ten2, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ZA1, MAKELONG(m_tbb_za1, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ZA2, MAKELONG(m_tbb_za2, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ZA3, MAKELONG(m_tbb_za3, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_ZA4, MAKELONG(m_tbb_za4, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_COLP, MAKELONG(m_tbb_colP, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_COLT, MAKELONG(m_tbb_colT, 0));
	SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_COLPP, MAKELONG(m_tbb_colPP, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_SHA, MAKELONG(m_tbb_sha, 0));
    SendMessage(g_hToolBar, TB_CHECKBUTTON, IDTBB_GLOW, MAKELONG(m_tbb_glow, 0));

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

	ret = (int)::MessageBox( m_hWnd, "形状データとモーションデータを削除します。\nよろしいですか？", "確認", MB_OKCANCEL );
	if( ret != IDOK ){
		return 0;
	}

	m_ExistData = FALSE;

	InvalidateHandler( 0, 1 );

	InvalidateGHandler();

	if( m_thandler || m_shandler || m_mhandler ){
		DestroyHandler();
	}
	
	if( m_g_th || m_g_sh || m_g_mh ){
		DestroyGHandler();
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
		::MessageBox( m_hWnd, "形状または、モーションデータが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}

	
	if( g_inidata->opt_modlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( g_loadingflag ){
		ret = IDOK;
	}

	if( ret != IDOK ){
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );


	m_sigeditwnd->SaveForbidSelect();


///////////

	DestroyMotion( 1 );

	ret = g_motdlg->RemakeMOEAH();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

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
		::MessageBox( m_hWnd, "形状または、モーションデータが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}

	if( m_shandler->m_bonetype != BONETYPE_MIKO ){
		::MessageBox( m_hWnd, "mikotoデータを読み込んでいないので、この操作は出来ません。", "操作できません", MB_OK );
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
		::MessageBox( m_hWnd, "形状または、モーションデータが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}

	if( g_inidata->opt_modlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( g_loadingflag ){
		ret = IDOK;
	}

	if( ret != IDOK ){
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );


	m_sigeditwnd->SaveForbidSelect();


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

		ret = g_motdlg->RemakeMOEAH();
		if( ret ){
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
///////
	D3DXVECTOR3 scale1( 1.0f, 1.0f, 1.0f );
	D3DXVECTOR3 scale2( 1.8f, 1.8f, 1.8f );
	
	if( (m_tbb_iti == 1) && 
		( (m_itidlg->m_object == ITIOBJ_JOINT) || (m_itidlg->m_object == ITIOBJ_JOINTROT) ) ){
		m_select_sh->SetDispScale( m_sel_cenno, scale2, 2, m_matWorld );
	}else{
		m_select_sh->SetDispScale( m_sel_cenno, scale1, 2, m_matWorld );
	}

	if( (srcflag == 0) || (srcflag == 1 ) ){
		ret = m_select_sh->SetValidFlag( m_sel_ringxno, srcflag );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		ret = m_select_sh->SetValidFlag( m_sel_ringyno, srcflag );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		ret = m_select_sh->SetValidFlag( m_sel_ringzno, srcflag );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		ret = m_select_sh->SetValidFlag( m_sel_cenno, 1 );//center 有効
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

	}else{
		ret = m_select_sh->SetValidFlag( m_sel_ringxno, 1 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		ret = m_select_sh->SetValidFlag( m_sel_ringyno, 1 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		ret = m_select_sh->SetValidFlag( m_sel_ringzno, 1 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		ret = m_select_sh->SetValidFlag( m_sel_cenno, 0 );// idtbb_4のときはセンター無効
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

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

///////////

	ret = m_select_sh->SetDispDiffuse( m_sel_xno, GetRValue( rcol ), GetGValue( rcol ), GetBValue( rcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetDispDiffuse( m_sel_ringxno, GetRValue( rcol ), GetGValue( rcol ), GetBValue( rcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_select_sh->SetDispDiffuse( m_sel_yno, GetRValue( gcol ), GetGValue( gcol ), GetBValue( gcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetDispDiffuse( m_sel_ringyno, GetRValue( gcol ), GetGValue( gcol ), GetBValue( gcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_select_sh->SetDispDiffuse( m_sel_zno, GetRValue( bcol ), GetGValue( bcol ), GetBValue( bcol ), 0, -1, 1 );
	if( ret ){
		DbgOut( "viewer : SetSelectColor : SetDispDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_select_sh->SetDispDiffuse( m_sel_ringzno, GetRValue( bcol ), GetGValue( bcol ), GetBValue( bcol ), 0, -1, 1 );
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

int CMyD3DApplication::IKTallON()
{
	int ret;
	int seri;
	D3DXVECTOR3 jointpos;

	if( !m_shandler || !m_ExistData ){
		return 0;
	}

	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		_ASSERT( selem );
		if( selem->IsJoint() ){
			if( (selem->m_iktarget == 1) || (selem->m_iktarget == 2) ){
				ret = g_motdlg->m_motparamdlg->GetBonePos( selem->serialno, m_matWorld, 1, &jointpos );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
				
				selem->m_iktarget = 1;
				selem->m_iktargetpos = jointpos;
			}
		}
	}

	return 0;
}



int CMyD3DApplication::IKTToggle()
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
			switch( selem->m_iktarget ){
			case 0:
				selem->m_iktarget = 1;
				break;
			case 1:
				selem->m_iktarget = 2;
				break;
			case 2:
				selem->m_iktarget = 0;
				break;
			default:
				selem->m_iktarget = 0;
				break;
			}
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
			selem->m_iktarget = 2;
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

	if( s_registflag == 0 ){
		return 0;
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


	m_pluginmenu = GetSubMenu(m_appmenu, 10);
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

////////////////////
	m_kinect = new CPluginKinect();
	if( !m_kinect ){
		DbgOut( "viewer : InitPluginMenu : plugin kinect alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


///////////
	if( g_inidata->usekinect == 1 ){
		char kindir[2048];
		ZeroMemory( kindir, 2048 );
		leng = GetEnvironmentVariable( (LPCTSTR)"MODULEDIR", (LPTSTR)kindir, 2048 );
		_ASSERT( leng );

		findrel = 0;
		findrel = strstr( kindir, reldir );
		finddbg = 0;
		finddbg = strstr( kindir, dbgdir );

		if( findrel ){
			*(findrel + 1) = 0;
		}else if( finddbg ){
			*(finddbg + 1) = 0;
		}
		strcat_s( kindir, 2048, "Kinect" );

		m_kinect->SetFilePath( kindir );
		m_kinect->LoadPlugin( m_hWnd );
	}else{
		int mid;
		for( mid = 54011; mid <= 54017; mid++ ){
			EnableMenuItem( m_appmenu, mid, MF_BYCOMMAND | MF_GRAYED );
		}
	}

	return 0;
}

int CMyD3DApplication::CamElem2Deg( CAMERAELEM camelem, float* degxz, float* degy )
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


int CMyD3DApplication::OnCameraRot( POINT ptCursor, POINT befpos )
{
	if( m_preview_flag && (g_motdlg->m_camdlg->m_camerabutton != CAMB_HEN) ){
		return 0;
	}

	float diffx;
	float diffy;
	diffx = (float)ptCursor.x - (float)befpos.x;
	diffy = (float)ptCursor.y - (float)befpos.y;


	RECT clirect;
	float rotxz, roty;
	::GetClientRect( m_3dwnd.m_hWnd, &clirect );
//	rotxz = ( 360.0f * diffx * g_inidata->camdlg_move / 20.0f )  / ((float)(clirect.right - clirect.left) / 2.0f);//半分の幅で一回転
//	roty = ( 180.0f * diffy * g_inidata->camdlg_move / 20.0f ) / ((float)(clirect.bottom - clirect.top) / 2.0f);//半分の高さで１８０度回転
	rotxz = ( 360.0f * diffx )  / ((float)(clirect.right - clirect.left) / 2.0f) * (m_camdist / CAMDISTINITVAL);//半分の幅で一回転
	roty = ( 180.0f * diffy ) / ((float)(clirect.bottom - clirect.top) / 2.0f) * (m_camdist / CAMDISTINITVAL);//半分の高さで１８０度回転


	CAMERAELEM curcamera;
	g_motdlg->m_camdlg->GetCurCamera( &curcamera );

	/***
	D3DXVECTOR3 saveup, newup;
	saveup = curcamera.up;
	D3DXMATRIX rotmatz, rotmaty, rotmat;
	D3DXMatrixRotationZ( &rotmatz, -roty * (float)DEG2PAI );
	D3DXMatrixRotationY( &rotmaty, -rotxz * (float)DEG2PAI );
	//rotmat = rotmatz * rotmaty;
	rotmat = rotmaty * rotmatz;
	D3DXVec3TransformCoord( &newup, &saveup, &rotmat );
	D3DXVec3Normalize( &newup, &newup );
	curcamera.up = newup;
	***/

	float degxz, degy;
	CamElem2Deg( curcamera, &degxz, &degy );

	degxz -= rotxz;
	degy -= roty;

	degy = max( 90.005f, degy );
	degy = min( 269.995f, degy );

	float radxz, rady;
	radxz = degxz * (float)DEG2PAI;
	rady = degy * (float)DEG2PAI;

    curcamera.pos.x = sinf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.x;
    curcamera.pos.y = curcamera.target.y + curcamera.dist * sinf( rady );
	curcamera.pos.z = -cosf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.z;

	D3DXVECTOR3 diff;
	diff = curcamera.target - curcamera.pos;
	curcamera.dist = D3DXVec3Length( &diff );
	m_camdist = curcamera.dist;

	m_targetpos = curcamera.target;
	m_eye_y = curcamera.target.y;
	m_vEyePt = curcamera.pos;
	m_camup = curcamera.up;
	m_proj_near = curcamera.nearz;
	m_proj_far = curcamera.farz;
	m_fov = curcamera.fov;
	m_ortho = curcamera.ortho;
	m_orthosize = curcamera.orthosize;

	g_motdlg->m_camdlg->SetCurCamera( curcamera );


	return 0;
}

int CMyD3DApplication::OnCameraMove( POINT ptCursor, POINT befpos )
{

	if( m_preview_flag && (g_motdlg->m_camdlg->m_camerabutton != CAMB_HEN) ){
		return 0;
	}


	float diffx;
	float diffy;

//	diffx = (float)ptCursor.x - (float)m_rpos.x;
//	diffy = (float)ptCursor.y - (float)m_rpos.y;

	diffx = (float)ptCursor.x - (float)befpos.x;
	diffy = (float)ptCursor.y - (float)befpos.y;


	RECT clirect;
	float heri, vert;
	::GetClientRect( m_3dwnd.m_hWnd, &clirect );
//	heri = m_camdist * diffx / (float)(clirect.right - clirect.left);
//	vert = m_camdist * diffy / (float)(clirect.bottom - clirect.top);

	heri = diffx * g_inidata->camdlg_move * ( 0.5f * m_camdist / CAMDISTINITVAL);
	vert = diffy * g_inidata->camdlg_move * ( 0.5f * m_camdist / CAMDISTINITVAL);


	CAMERAELEM curcamera;
	g_motdlg->m_camdlg->GetCurCamera( &curcamera );


	float degxz, degy;
	CamElem2Deg( curcamera, &degxz, &degy );

	degy = max( 90.005f, degy );
	degy = min( 269.995f, degy );

//////
	float radheri;
	radheri = (degxz - 90.0f) * (float)DEG2PAI;
	curcamera.target.x -= heri * (float)sin( radheri );
	curcamera.target.z += heri * (float)cos( radheri );
//////
	D3DXMATRIX invview;
	invview = m_matView;
	invview._41 = 0.0f;
	invview._42 = 0.0f;
	invview._43 = 0.0f;
	D3DXMatrixInverse( &invview, NULL, &invview );

	D3DXVECTOR3 newup;
	D3DXVec3TransformCoord( &newup, &(curcamera.up), &invview );

	curcamera.target.x += vert * newup.x;
	curcamera.target.y += vert * newup.y;
	curcamera.target.z += vert * newup.z;

///////
	float radxz, rady;
	radxz = degxz * (float)DEG2PAI;
	rady = degy * (float)DEG2PAI;

    curcamera.pos.x = sinf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.x;
    curcamera.pos.y = curcamera.target.y + curcamera.dist * sinf( rady );
	curcamera.pos.z = -cosf(radxz) * curcamera.dist * cosf( rady ) + curcamera.target.z;

	D3DXVECTOR3 diff;
	diff = curcamera.target - curcamera.pos;
	curcamera.dist = D3DXVec3Length( &diff );
	m_camdist = curcamera.dist;

	m_targetpos = curcamera.target;
	m_eye_y = curcamera.target.y;
	m_vEyePt = curcamera.pos;
	m_camup = curcamera.up;
	m_proj_near = curcamera.nearz;
	m_proj_far = curcamera.farz;
	m_fov = curcamera.fov;
	m_ortho = curcamera.ortho;
	m_orthosize = curcamera.orthosize;

	g_motdlg->m_camdlg->SetCurCamera( curcamera );

	return 0;
}

int CMyD3DApplication::OnCameraDist( float vert )
{

	if( m_preview_flag && (g_motdlg->m_camdlg->m_camerabutton != CAMB_HEN) ){
		return 0;
	}

	vert = vert * g_inidata->camdlg_move / 20.0f;


	CAMERAELEM curcamera;
	g_motdlg->m_camdlg->GetCurCamera( &curcamera );

	if( m_ortho == 0 ){
		D3DXVECTOR3 diff;
		diff = curcamera.pos - curcamera.target;
		D3DXVECTOR3 ndiff;
		D3DXVec3Normalize( &ndiff, &diff );

		float tmpdist;
		tmpdist = D3DXVec3Length( &diff ) + vert;
		if( tmpdist > 0.0f ){
			curcamera.dist = tmpdist;
			curcamera.pos = curcamera.target + curcamera.dist * ndiff;
		}else{
			curcamera.target = curcamera.target - 1000.0f * ndiff;
			D3DXVECTOR3 diff2 = curcamera.pos - curcamera.target;
			curcamera.dist = D3DXVec3Length( &diff2 );
			D3DXVECTOR3 ndiff2;
			D3DXVec3Normalize( &ndiff2, &diff2 );
			curcamera.pos = curcamera.target + curcamera.dist * ndiff2;
		}
	}else{
		curcamera.orthosize += vert;
		if( curcamera.orthosize < 500.0f ){
			curcamera.orthosize = 500.0f;
		}
	}

	D3DXVECTOR3 diff2;
	diff2 = curcamera.pos - curcamera.target;
	float curdist = D3DXVec3Length( &diff2 );
	m_camdist = curdist;
	curcamera.dist = curdist;

	m_targetpos = curcamera.target;
	m_eye_y = curcamera.target.y;
	m_vEyePt = curcamera.pos;
	m_camup = curcamera.up;
	m_proj_near = curcamera.nearz;
	m_proj_far = curcamera.farz;
	m_fov = curcamera.fov;
	m_ortho = curcamera.ortho;
	m_orthosize = curcamera.orthosize;

	g_motdlg->m_camdlg->SetCurCamera( curcamera );

/***
	if( vert >= 0.0f ){
		ChangeViewParam( ID_KEY_ZOOMOUT, vert );
	}else{
		ChangeViewParam( ID_KEY_ZOOMIN, -vert );
	}
***/				
	//正射影のビューポートの大きさの変更					
//	m_parsdlg->orthosize += (int)vert;
//	if( m_parsdlg->orthosize < 500 ){
//		m_parsdlg->orthosize = 500;
//	}

	return 0;
}

int CMyD3DApplication::CreateBone( POINT pt )
{
//::MessageBox( m_hWnd, "check00", "check", MB_OK );


	m_sigeditwnd->SaveForbidSelect();

	int isjoint;
	CShdElem* parelem;
	parelem = m_bonedlg->GetSelectedJoint( &isjoint );

//char mes[512];
//if( parelem ){
//	sprintf_s( mes, 512, "seri %d, isjoint %d", parelem->serialno, isjoint );
//}else{
//	sprintf_s( mes, 512, "seri pointer %x, isjoint %d", parelem, isjoint );
//}
//::MessageBox( m_hWnd, mes, "check", MB_OK );


	::GetClientRect( m_3dwnd.m_hWnd, &m_clirect );
	m_clientWidth = m_clirect.right - m_clirect.left;
	m_clientHeight = m_clirect.bottom - m_clirect.top;
    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;
//int CShdHandler::GetScreenPos3( 0, CMotHandler* lpmh, int srcseri, int vertno, int calcmode, int* scxptr, int* scyptr, float* sczptr, 
//							   DWORD dwClipWidth, DWORD dwClipHeight, D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj )

	D3DXVECTOR3 newpos;
	int ret;
	if( parelem && isjoint ){
//::MessageBox( m_hWnd, "check1", "check", MB_OK );

		/***
		float shiftscx, shiftscy, shiftscz, wp;	
		ret = parelem->GetBoneScreenPos( &shiftscx, &shiftscy, &shiftscz, &wp );
		if( ret != 0 )
			return 0;
		***/
		int shiftscx, shiftscy;
		float shiftscz;
		ret = m_shandler->GetScreenPos3( 0, m_mhandler, parelem->serialno, 0, 1, &shiftscx, &shiftscy, &shiftscz, dwClipWidth, dwClipHeight,
			m_matWorld, m_matView, m_matProjX );

//::MessageBox( m_hWnd, "check2", "check", MB_OK );

		if( ret != 0 )
			return 0;//!!!!!!!

		D3DXVECTOR3 targetsc;
		targetsc.x = (float)pt.x;
		targetsc.y = (float)pt.y;
		targetsc.z = shiftscz;//!!!!!

		ret = ScreenToObj( &newpos, targetsc );
		if( ret )
			return 0;


//::MessageBox( m_hWnd, "check10", "check", MB_OK );
	}else{

//::MessageBox( m_hWnd, "check11", "check", MB_OK );

		D3DXVECTOR3 zerovec( 0.0f, 0.0f, 0.0f );
		float scx, scy, scz, wp;
		ret = GetScreenPos( zerovec, &scx, &scy, &scz, &wp );
		if( ret )
			return 0;	

//::MessageBox( m_hWnd, "check12", "check", MB_OK );

		D3DXVECTOR3 targetsc;
		targetsc.x = (float)pt.x;
		targetsc.y = (float)pt.y;
		targetsc.z = scz;//!!!!!

		ret = ScreenToObj( &newpos, targetsc );
		if( ret )
			return 0;	

//::MessageBox( m_hWnd, "check20", "check", MB_OK );
	}

	int newid = -1;
	ret = CreateBone( 0, newpos, parelem, &newid );
	if( ret || (newid < 0) ){
		DbgOut( "viewer : CreateBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::CloneDispObj()
{
	int ret;
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	//////////////////
	m_ExistData = FALSE;

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

	////////////////////
	int orgseri;
	orgseri = m_sigeditwnd->m_selectedno;
	if( orgseri <= 0 ){
		return 0;
	}

	CShdElem* orgelem;
	orgelem = (*m_shandler)( orgseri );
	if( (orgelem->type != SHDPOLYMESH) && (orgelem->type != SHDPOLYMESH2) ){
		return 0;
	}

	CTreeElem2* orgtelem;
	orgtelem = (*m_thandler)( orgseri );

	CShdElem* parelem;
	parelem = (*m_shandler)( 1 );

	//////////////////
	int serino = m_shandler->s2shd_leng;
	int srctype = orgelem->type;
	int depth;	
	char newname[256];
	ZeroMemory( newname, 256 );
	sprintf_s( newname, "Clone%s", orgtelem->name );

	m_thandler->Start( serino - 1 );
	m_thandler->Begin();
	depth = m_thandler->GetCurDepth();
//	ret = Add2Tree( newname, serino, srctype, depth, 1 );
//	if( ret ){
//		_ASSERT( 0 );
//		return 1;
//	}

///////////
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

//	ret = m_shandler->SetClockwise( serino, 2 );
//	if( ret ){
//		_ASSERT( 0 );
//		return 1;
//	}
	ret = m_mhandler->AddMotionCtrl( serino, depth );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}	
//	ret = m_mhandler->SetHasMotion( serino, hasmotion );
//	if( ret ){
//		_ASSERT( 0 );
//		return 1;
//	}
	///////////////

	m_thandler->End();

	CShdElem* curelem;
	curelem = (*m_shandler)( serino );
	_ASSERT( curelem );
	ret = curelem->LeaveFromChain();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = curelem->PutToChain( parelem, 1 );
	if( ret ){
		DbgOut( "viewer : CloneDispObj : PutToChain error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_shandler->CalcDepth();
	if( ret ){
		DbgOut( "viewer : CloneDispObj : shandler CalcDepth error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	ret = m_mhandler->CopyChainFromShd( m_shandler );
	if( ret ){
		DbgOut( "viewer : CloneDispObj : mhandler CopyChainFromShd error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_mhandler->CalcDepth();
	if( ret ){
		DbgOut( "viewer : CloneDispObj : mhandler CalcDepth error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
//////////////
	curelem->facet = orgelem->facet;
	curelem->alpha = orgelem->alpha;
	curelem->clockwise = orgelem->clockwise;
	curelem->transparent = orgelem->transparent;
	curelem->setalphaflag = orgelem->setalphaflag;
	curelem->texrule = orgelem->texrule;
	curelem->noskinning = orgelem->noskinning;
	curelem->blendmode = orgelem->blendmode;
	curelem->m_minfilter = orgelem->m_minfilter;
	curelem->m_magfilter = orgelem->m_magfilter;
	curelem->m_loadbimflag = orgelem->m_loadbimflag;
	curelem->m_enabletexture = orgelem->m_enabletexture;
	curelem->m_anchorflag = orgelem->m_anchorflag;
	curelem->m_mikodef = orgelem->m_mikodef;

_ASSERT( ::IsWindow( m_hWnd ) );

	ret = curelem->CloneDispObj( orgelem );
	if( ret ){
		DbgOut( "viewer : CloneDispObj : se CloneDispObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

_ASSERT( ::IsWindow( m_hWnd ) );

	////////////////////
	////////////////////
	ret = RestoreHandler( 0 );
	if( ret ){
		DbgOut( "viewer : CloneDispObj : RestoreHandler error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

_ASSERT( ::IsWindow( m_hWnd ) );

	//作り直す前に、invalidateフラグを立てる。
		//作り直し中のSelChangeなどでのエラー回避のため。
	if( g_motdlg )
		g_motdlg->InvalidateHandler();
	if( m_sigeditwnd )
		m_sigeditwnd->InvalidateHandler();

_ASSERT( ::IsWindow( m_hWnd ) );
	
	if( g_motdlg ){
_ASSERT( ::IsWindow( m_hWnd ) );

		g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
			m_hWnd, this );
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->InitMC();
			if( ret ){
				DbgOut( "viewer : CloneDispObj : mcdlg InitMC error !!!\n" );
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
	
	m_ExistData = TRUE;

	return 0;
}


int CMyD3DApplication::CreateBone( char* nameptr, D3DXVECTOR3 newpos, CShdElem* parelem, int* seriptr, int restoreflag )
{
////////////
	int ret;
	*seriptr = -1;

	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}


//	ret = g_motdlg->SelectMotParamTab();
//	_ASSERT( !ret );


	//////////////////
	m_ExistData = FALSE;

	if( restoreflag ){
		InvalidateHandler( 1, 0 );

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

			ret = g_motdlg->RemakeMOEAH();
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

		}
	}
	//////////////////
	int destseri = -1;
	ret = m_shandler->GetDestroyedSeri( &destseri );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	int serino;
	int srctype = SHDBALLJOINT;
	int depth;	
	char newname[256];
	ZeroMemory( newname, 256 );

	if( destseri < 0 ){
		serino = m_shandler->s2shd_leng;

		m_thandler->Start( serino - 1 );
		m_thandler->Begin();
		depth = m_thandler->GetCurDepth();
		char* setnameptr;
		if( nameptr ){
			setnameptr = nameptr;
		}else{
			sprintf_s( newname, 256, "NewJoint%d", serino );
			setnameptr = newname;
		}

		ret = Add2Tree( setnameptr, serino, SHDBALLJOINT, depth, 1 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		m_thandler->End();

	}else{
		serino = destseri;
		depth = 2;
		char* setnameptr;
		if( nameptr ){
			setnameptr = nameptr;
		}else{
			sprintf_s( newname, 256, "NewJoint%d", serino );
			setnameptr = newname;
		}

		int createflag = 0;
		ret = Add2Tree( setnameptr, serino, SHDBALLJOINT, depth, 1, createflag );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
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

	CShdElem* curelem;
	curelem = (*m_shandler)( serino );
	_ASSERT( curelem );
	ret = curelem->LeaveFromChain();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	if( parelem ){
		ret = curelem->PutToChain( parelem, 1 );
		if( ret ){
			DbgOut( "viewer : CreateBone : PutToChain error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
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

	if( restoreflag ){
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
	}

	*seriptr = serino;//!!!!!!!!!!

	m_ExistData = TRUE;

	return 0;
}

int CMyD3DApplication::TempPickVert( int seldisp, int* shiftpart, int* shiftvert, int* shiftvertnum )
{
	if( !m_shandler || (m_ExistData != TRUE) ){
		*shiftpart = -1;
		*shiftvertnum = 0;
		return 0;
	}


	POINT ptCursor;
	GetCursorPos( &ptCursor );
	::ScreenToClient( m_3dwnd.m_hWnd, &ptCursor );
	
	PickVertSelDisp( seldisp, (WORD)ptCursor.x, (WORD)ptCursor.y, shiftpart, shiftvert, shiftvertnum );

	return 0;
}

int CMyD3DApplication::TempPickFace( int srcpart, int* pickpart, int* pickface, D3DXVECTOR3* pickpos )
{
	*pickpart = -1;
	*pickface = -1;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_ExistData == FALSE )
		return 0;
	if( !g_motdlg )
		return 0;

	POINT ptCursor;
	GetCursorPos( &ptCursor );
	::ScreenToClient( m_3dwnd.m_hWnd, &ptCursor );

	int ret;
	float maxdist = m_proj_far + 10000.0f;
	D3DXVECTOR3 n;
	float facedist;
	ret = m_shandler->PickFace( maxdist, m_clientWidth, m_clientHeight, 0, m_mhandler, m_matWorld, m_matView, m_matProjX, 
		&srcpart, 1,
		ptCursor.x, ptCursor.y, pickpart, pickface, pickpos, &n, &facedist, 1 );
	if( ret ){
		DbgOut( "viewer : TempPickFace : shandler PickFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}



int CMyD3DApplication::SaveXFile( char* srcfilename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;

	if( !m_ExistData )
		return 0;


	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

/////////////
	int mnum = g_motdlg->GetMotionNum();
	MOTID* motid = g_motdlg->GetMotIDArray();

	CWriteXFileDlg dlg( m_mhandler, srcfilename, motid, mnum );
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
	ret = xfile->WriteXFile( dlg.m_fullbone, m_sigeditwnd, dlg.wnum, dlg.ppni, dlg.xfilename, dlg.mult, dlg.convface, m_thandler, m_shandler, m_mhandler );
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

	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	m_setcoldlg->InitUndoBuf();
	m_paintdlg->InitUndoBuf();

	int motnum;
	motnum = g_motdlg->GetMotionNum();

	if( (allflag == 1) || (allflag == 2) || (motnum <= 1) ){

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

//	if( allflag == 2 ){
//		if( g_motdlg )
//			g_motdlg->InvalidateHandler();
//		
//		if( g_motdlg ){
//			g_motdlg->SetParams( m_thandler, m_shandler, m_mhandler,
//				m_hWnd, this );
//		}
//	}

	return 0;
}
int CMyD3DApplication::MotCut()
{
	if( !m_shandler || !m_mhandler || !g_motdlg ){
		return 0;
	}
	int ret;
	ret = g_motdlg->MotCut();
	if( ret ){
		_ASSERT( !ret );
		return 1;
	}

	return 0;
}
int CMyD3DApplication::MotMabiki()
{
	if( !m_shandler || !m_mhandler || !g_motdlg ){
		return 0;
	}
	int ret;
	ret = g_motdlg->MotMabiki();
	if( ret ){
		_ASSERT( !ret );
		return 1;
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
		ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), 0, 0, 0, 0, 0, 0 );
		if( ret ){
			DbgOut( "viewer : newmotion : mcdlg AddParentMC error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}
int CMyD3DApplication::DestroyCameraAnim( int allflag )
{
	int ret;
	if( !g_motdlg ){
		return 0;
	}

	int motnum;
	motnum = g_motdlg->GetCameraAnimNum();

	if( (allflag == 1) || (motnum <= 1) ){
		if( g_motdlg ){
			ret = g_motdlg->OnStop();
			_ASSERT( !ret );
		}

		ret = g_motdlg->DestroyCameraAnim( -1 );
		if( ret ){
			DbgOut( "viewer : DestroyCameraAnim : motdlg DestroyCameraAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{

		int delcookie;
		delcookie = g_motdlg->GetCurCameraAnimNo();
		if( delcookie > 0 ){
			ret = g_motdlg->DestroyCameraAnim( delcookie );
			if( ret ){
				DbgOut( "viewer : DestroyCameraAnim : motdlg DestroyCameraAnim error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}
int CMyD3DApplication::NewCameraAnim()
{
	if( !g_motdlg ){
		return 0;
	}

	int ret;
	ret = g_motdlg->NewCameraAnim();
	if( ret ){
		DbgOut( "viewer : NewCameraAnim : motdlg NewCameraAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::LoadCameraMulti()
{
	int ret;

	OPENFILENAME ofn;
	char buf[ 260 * MAXMOTIONNUM ];

	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Camera File (*.cam)\0*.cam\0";
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
		ret = LoadCamera( buf );
		if( ret ){
			DbgOut( "viewer : LoadCameraMulti : LoadCamera error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int leng2;
		char fname[512];
		while( *topchar != 0 ){
			//DbgOut( "%d--%s\\%s\r\n", namecnt, buf, topchar );
			sprintf_s( fname, 512, "%s\\%s", buf, topchar );
			ret = LoadCamera( fname );
			if( ret ){
				DbgOut( "viewer : LoadCameraMulti : LoadCamera error !!!\n" );
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
int CMyD3DApplication::LoadCamera( char* filename )
{
	int ret;

	if( g_motdlg ){

		if( m_tbb_iti ){
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
		}

		if( g_motdlg->GetCameraAnimNum() >= MAXMOTIONNUM ){
			char messtr[1024];
			sprintf_s( messtr, 1024, "読み込めるカメラアニメは、%d個までです。\nこれ以上、読み込めません。", MAXMOTIONNUM );
			::MessageBox( m_hWnd, messtr, "エラー", MB_OK );
			return 0;
		}

		//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = g_motdlg->LoadCameraFile( filename );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}


		MOTID* motidptr;
		motidptr = g_motdlg->GetCameraMotIDArray();
		MOTID* curmotid;
		curmotid = motidptr + g_motdlg->GetCurCameraAnimIndex();
		if( curmotid->id != g_motdlg->GetCurCameraAnimNo() ){
			_ASSERT( 0 );
			return 1;
		}

	}else{
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		return 0;
	}


	return 0;
}

int CMyD3DApplication::DestroyMOEAnim( int allflag )
{
	int ret;
	if( !g_motdlg ){
		return 0;
	}

	int motnum;
	motnum = g_motdlg->GetMOEAnimNum();

	if( (allflag == 1) || (motnum <= 1) ){
		if( g_motdlg ){
			ret = g_motdlg->OnStop();
			_ASSERT( !ret );
		}

		ret = g_motdlg->DestroyMOEAnim( -1 );
		if( ret ){
			DbgOut( "viewer : DestroyMOEAnim : motdlg DestroyMOEAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{

		int delcookie;
		delcookie = g_motdlg->GetCurMOEAnimNo();
		if( delcookie > 0 ){
			ret = g_motdlg->DestroyMOEAnim( delcookie );
			if( ret ){
				DbgOut( "viewer : DestroyMOEAnim : motdlg DestroyMOEAnim error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}
int CMyD3DApplication::NewMOEAnim()
{
	if( !g_motdlg ){
		return 0;
	}

	int ret;
	ret = g_motdlg->NewMOEAnim();
	if( ret ){
		DbgOut( "viewer : NewMOEAnim : motdlg NewGPAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


int CMyD3DApplication::DestroyGPAnim( int allflag )
{
	int ret;
	if( !g_motdlg ){
		return 0;
	}

	int motnum;
	motnum = g_motdlg->GetGPAnimNum();

	if( (allflag == 1) || (motnum <= 1) ){
		if( g_motdlg ){
			ret = g_motdlg->OnStop();
			_ASSERT( !ret );
		}

		ret = g_motdlg->DestroyGPAnim( -1 );
		if( ret ){
			DbgOut( "viewer : DestroyGPAnim : motdlg DestroyGPAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

	}else{

		int delcookie;
		delcookie = g_motdlg->GetCurGPAnimNo();
		if( delcookie > 0 ){
			ret = g_motdlg->DestroyGPAnim( delcookie );
			if( ret ){
				DbgOut( "viewer : DestroyGPAnim : motdlg DestroyGPAnim error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}
int CMyD3DApplication::NewGPAnim()
{
	if( !g_motdlg ){
		return 0;
	}

	int ret;
	ret = g_motdlg->NewGPAnim();
	if( ret ){
		DbgOut( "viewer : NewGPAnim : motdlg NewGPAnim error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::LoadMOEMulti()
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler || !g_motdlg ){
		::MessageBox( m_hWnd, "モデルデータを読み込んでから再試行してください。", "エラー", MB_OK );
		return 0;
	}

	int idlingid = -1;
	if( g_motdlg->m_motparamdlg->m_mch ){
		idlingid = g_motdlg->m_motparamdlg->m_mch->GetIdlingMotID();
	}
	if( idlingid < 0 ){
		::MessageBox( m_hWnd, "moaのイベントモーションを設定してから再試行してください。", "エラー", MB_OK );
		return 0;
	}

	OPENFILENAME ofn;
	char buf[ 260 * MAXMOTIONNUM ];

	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"MOE File (*.moe)\0*.moe\0";
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
		ret = LoadMOE( buf );
		if( ret ){
			DbgOut( "viewer : LoadMOEMulti : LoadMOE error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int leng2;
		char fname[512];
		while( *topchar != 0 ){
			//DbgOut( "%d--%s\\%s\r\n", namecnt, buf, topchar );
			sprintf_s( fname, 512, "%s\\%s", buf, topchar );
			ret = LoadMOE( fname );
			if( ret ){
				DbgOut( "viewer : LoadMOEMulti : LoadMOE error !!!\n" );
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
int CMyD3DApplication::LoadMOE( char* filename )
{
	int ret;

	if( g_motdlg && m_shandler && m_mhandler ){

		if( m_tbb_iti ){
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
		}

		if( g_motdlg->GetMOEAnimNum() >= MAXMOTIONNUM ){
			char messtr[1024];
			sprintf_s( messtr, 1024, "読み込めるmoaイベントアニメは、%d個までです。\nこれ以上、読み込めません。", MAXMOTIONNUM );
			::MessageBox( m_hWnd, messtr, "エラー", MB_OK );
			return 0;
		}

		//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = g_motdlg->LoadMOEFile( filename );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}


		MOTID* motidptr;
		motidptr = g_motdlg->GetMOEMotIDArray();
		MOTID* curmotid;
		curmotid = motidptr + g_motdlg->GetCurMOEAnimIndex();
		if( curmotid->id != g_motdlg->GetCurMOEAnimNo() ){
			_ASSERT( 0 );
			return 1;
		}

	}else{
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		return 0;
	}


	return 0;
}


int CMyD3DApplication::LoadGPMulti()
{
	int ret;

	OPENFILENAME ofn;
	char buf[ 260 * MAXMOTIONNUM ];

	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"GPA File (*.gpa)\0*.gpa\0";
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
		ret = LoadGP( buf );
		if( ret ){
			DbgOut( "viewer : LoadGPMulti : LoadGP error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		int leng2;
		char fname[512];
		while( *topchar != 0 ){
			//DbgOut( "%d--%s\\%s\r\n", namecnt, buf, topchar );
			sprintf_s( fname, 512, "%s\\%s", buf, topchar );
			ret = LoadGP( fname );
			if( ret ){
				DbgOut( "viewer : LoadGPMulti : LoadGP error !!!\n" );
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
int CMyD3DApplication::LoadGP( char* filename )
{
	int ret;

	if( g_motdlg ){

		if( m_tbb_iti ){
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
		}

		if( g_motdlg->GetGPAnimNum() >= MAXMOTIONNUM ){
			char messtr[1024];
			sprintf_s( messtr, 1024, "読み込めるGlobalPositionアニメは、%d個までです。\nこれ以上、読み込めません。", MAXMOTIONNUM );
			::MessageBox( m_hWnd, messtr, "エラー", MB_OK );
			return 0;
		}

		//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = g_motdlg->LoadGPFile( filename );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}


		MOTID* motidptr;
		motidptr = g_motdlg->GetGPMotIDArray();
		MOTID* curmotid;
		curmotid = motidptr + g_motdlg->GetCurGPAnimIndex();
		if( curmotid->id != g_motdlg->GetCurGPAnimNo() ){
			_ASSERT( 0 );
			return 1;
		}

	}else{
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		return 0;
	}


	return 0;
}

int CMyD3DApplication::LoadMotionMulti()
{

	int ret;

	if( m_ExistData == 0 ){
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		return 0;
	}


	if( g_motdlg && m_mhandler ){

		OPENFILENAME ofn;
		char buf[ 260 * MAXMOTIONNUM ];

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
			ret = LoadMotion( buf, dlg.multvec, 0, 0, 0, 0, 0, 0 );
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
				ret = LoadMotion( fname, dlg.multvec, 0, 0, 0, 0, 0, 0 );
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
	}else{
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		return 0;
	}

	return 0;
}

int CMyD3DApplication::LoadEQUFile( char* srcfilename )
{
	int ret;

	int idling = 0;
	int ev0idle = 0;
	int commonid = 0;
	int forbidnum = 0;
	int* forbidid = 0;
	int notfu = 0;


	if( g_motdlg && m_mhandler ){

		float ldmult = 1.0f;
		int dlgret;
		CLdEquDlg dlg;
		dlgret = (int)dlg.DoModal();
		if( dlgret != IDOK )
			return -2;
		ldmult = dlg.mult;


		CEQUFile equfile;
		ret = equfile.LoadEQUFile( srcfilename );
		if( ret ){
			DbgOut( "viewer : LoadEQUFile : equfile LoadEQUFile error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		if( equfile.m_animnum <= 0 ){
			return 0;
		}

		int animcnt;
		for( animcnt = 0; animcnt < equfile.m_animnum; animcnt++ ){
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

			
			ret = g_motdlg->LoadEQUMotion( &equfile, animcnt, ldmult );
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

			m_setcoldlg->InitUndoBuf();
			m_paintdlg->InitUndoBuf();

			if( ret ){
				DbgOut( "viewer : LoadEQUFile : motdlg LoadEQUMotion error !!!\n" );
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

			
			ret = ChangeZaKindFromMotion( g_motdlg->GetMotCookie() );
			_ASSERT( !ret );


			if( m_mcdlg->m_firstmake ){
				ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), idling, ev0idle, commonid, forbidnum, forbidid, notfu );
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
			curmotid->notfu = notfu;

		}

		ret = g_motdlg->GetSelectedJointPos( &m_selectedno, &m_selectedpos );
		_ASSERT( !ret );
		ret = g_motdlg->OnUserSelchange( m_selectedno );//TexKeyDlgのリスト更新
		_ASSERT( !ret );
	}else{
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		_ASSERT( 0 );
		return 0;
	}

	return 0;
}


int CMyD3DApplication::LoadMotion( char* filename, D3DXVECTOR3 quamult, int idling, int ev0idle, int commonid, int forbidnum, int* forbidid, int notfu, int callmoa )
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

		ret = g_motdlg->LoadMotion( m_loadnamedlg, filename, quamult, callmoa );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

		m_setcoldlg->InitUndoBuf();
		m_paintdlg->InitUndoBuf();

		if( ret ){
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

		ret = ChangeZaKindFromMotion( g_motdlg->GetMotCookie() );
		_ASSERT( !ret );


		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), idling, ev0idle, commonid, forbidnum, forbidid, notfu );
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
		curmotid->notfu = notfu;

		//curmotid->forbidcommonid = forbidcommonid;


//DbgOut( "viewer : 5010 : end LoadMotion\r\n" );
	}else{
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		return 0;
	}

	return 0;

}

int CMyD3DApplication::LoadMKM()
{
	int ret;

	if( !m_shandler || !m_thandler || !m_mhandler || !g_motdlg ){
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
		return 0;
	}
	if( m_ExistData == 0 ){
		::MessageBox( m_hWnd, "形状データを読み込んでから再試行してください。", "警告", MB_OK );
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

		ret = ChangeZaKindFromMotion( g_motdlg->GetMotCookie() );
		_ASSERT( !ret );


		int idling = 0;
		int ev0idle = 0;
		int commonid = 0;
		int forbidnum = 0;
		int* forbidid = 0;
		int notfu = 0;
		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), idling, ev0idle, commonid, forbidnum, forbidid, notfu );
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

	/***
	if( menuid == IDM_G_LEFT ){
		if( m_graphwnd ){
			m_graphwnd->ShiftRange( -1 );
		}
	}else if( menuid == IDM_G_RIGHT ){
		if( m_graphwnd ){
			m_graphwnd->ShiftRange( 1 );
		}
	}else 
	***/
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
		::MessageBox( m_hWnd, "形状データが無いので、実行できません。", "実行できません", MB_OK );
		return 0;
	}	
	if( g_inidata->opt_modlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( g_loadingflag ){
		ret = IDOK;
	}

	if( ret != IDOK ){
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );


	m_sigeditwnd->SaveForbidSelect();


///////////

	DestroyMotion( 1 );

	ret = g_motdlg->RemakeMOEAH();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

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

	if( !m_shandler || !g_motdlg ){
		_ASSERT( 0 );
		return 0;
	}


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

	int orgmot = -1;

	D3DXVECTOR3 mult = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	int leng;
	int namecnt = 0;
	leng = (int)strlen( buf );
	char* topchar = buf + leng + 1;
	if( *topchar == 0 ){
		
		ret = LoadMotion( buf, mult, 0, 0, 0, 0, 0, 0 );
		if( ret ){
			DbgOut( "viewer : ConvSym error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		orgmot = g_motdlg->m_motparamdlg->m_mot_cookie;
		int newleng = g_motdlg->m_motparamdlg->m_sl_mp_rangemax + 1;
		ret = g_motdlg->m_motparamdlg->ConvSymMotion();
		if( ret ){
			DbgOut( "viewer : ConvSym : mpdlg COnvSymMotion error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		char outname[MAX_PATH];
		ZeroMemory( outname, sizeof( char ) * MAX_PATH );
		int ch2 = '.';
		char* conmaptr = 0;
		conmaptr = strrchr( buf, ch2 );
		if( conmaptr ){
			int cpleng;
			cpleng = (int)( conmaptr - buf );
			strncpy_s( outname, MAX_PATH, buf, cpleng );
			strcat_s( outname, MAX_PATH, "_inv.qua" );
		}else{
			sprintf_s( outname, MAX_PATH, "%s_inv.qua", buf );
		}
		CQuaFile quafile;
		ret = quafile.SaveQuaFile( g_inidata->opt_namequa, m_thandler, m_shandler, m_mhandler, 
			outname, orgmot, 0, newleng );
		_ASSERT( !ret );


		ret = g_motdlg->GetSelectedJointPos( &m_selectedno, &m_selectedpos );
		_ASSERT( !ret );
		ret = g_motdlg->OnUserSelchange( m_selectedno );//TexKeyDlgのリスト更新
		_ASSERT( !ret );

	}else{
		int leng2;
		char fname[512];
		while( *topchar != 0 ){
			//DbgOut( "%d--%s\\%s\r\n", namecnt, buf, topchar );
			sprintf_s( fname, 512, "%s\\%s", buf, topchar );
			ret = LoadMotion( fname, mult, 0, 0, 0, 0, 0, 0 );
			if( ret ){
				DbgOut( "viewer : ConvSym error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			orgmot = g_motdlg->m_motparamdlg->m_mot_cookie;
			int newleng = g_motdlg->m_motparamdlg->m_sl_mp_rangemax + 1;
			ret = g_motdlg->m_motparamdlg->ConvSymMotion();
			if( ret ){
				DbgOut( "viewer : ConvSym : mpdlg COnvSymMotion error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			char outname[MAX_PATH];
			ZeroMemory( outname, sizeof( char ) * MAX_PATH );
			int ch2 = '.';
			char* conmaptr = 0;
			conmaptr = strrchr( fname, ch2 );
			if( conmaptr ){
				int cpleng;
				cpleng = (int)( conmaptr - fname );
				strncpy_s( outname, MAX_PATH, fname, cpleng );
				strcat_s( outname, MAX_PATH, "_inv.qua" );
			}else{
				sprintf_s( outname, MAX_PATH, "%s_inv.qua", fname );
			}
			CQuaFile quafile;
			ret = quafile.SaveQuaFile( g_inidata->opt_namequa, m_thandler, m_shandler, m_mhandler, 
				outname, orgmot, 0, newleng );
			_ASSERT( !ret );


			ret = g_motdlg->GetSelectedJointPos( &m_selectedno, &m_selectedpos );
			_ASSERT( !ret );
			ret = g_motdlg->OnUserSelchange( m_selectedno );//TexKeyDlgのリスト更新
			_ASSERT( !ret );

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

int CMyD3DApplication::SaveLimFile( char* srcfilename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( !m_ExistData )
		return 0;

	char buf[_MAX_PATH];
	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		strcpy_s( buf, _MAX_PATH, "default.lim" );
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
int CMyD3DApplication::LoadLimFile( char* srcfilename )
{
	char buf[_MAX_PATH];

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		OPENFILENAME ofn;
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

int CMyD3DApplication::SaveBVHFile( char* srcfilename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( !m_ExistData )
		return 0;

	char buf[_MAX_PATH];

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		strcpy_s( buf, _MAX_PATH, "default.bvh" );
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
int CMyD3DApplication::SetShaderConst( int shadowflag, D3DXMATRIX* pmatProj )
{
	HRESULT hres;


	if( shadowflag == 0 ){

		hres = g_pEffect->SetMatrix( g_hmView, &m_matView );
		if( hres != D3D_OK ){
			DbgOut( "SetShaderConst : view set error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		hres = g_pEffect->SetMatrix( g_hmProj, pmatProj );
		if( hres != D3D_OK ){
			DbgOut( "SetShaderConst : proj set error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		hres = g_pEffect->SetValue( g_hmEyePos, (void*)(&m_vEyePt), sizeof( float ) * 3 );
		if( hres != D3D_OK ){
			DbgOut( "SetShaderConst : eyepos set error !!!\n" );
			_ASSERT( 0 );
			return 1;	
		}


	//float3 mLightParams[5][7];
	//0: x=validflag, y=type, z=divRange
	//1: x=cosPhi, y=divCosPhi
	//2: xyz=diffuse
	//3: xyz=specular
	//4: xyz=position
	//5: xyz=direction

		float tmplparam[5][6][4];
		ZeroMemory( tmplparam, sizeof( float ) * 5 * 6 * 4 );

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

				switch( curl->Type ){
				case D3DLIGHT_DIRECTIONAL:
					tmplparam[lno][0][1] = 0.0f;

					tmplparam[lno][5][0] = curl->orgDirection.x;
					tmplparam[lno][5][1] = curl->orgDirection.y;
					tmplparam[lno][5][2] = curl->orgDirection.z;

					break;
				case D3DLIGHT_POINT:
					tmplparam[lno][0][1] = 1.0f;

					tmplparam[lno][0][2] = curl->divRange;


					tmplparam[lno][4][0] = curl->orgPosition.x;
					tmplparam[lno][4][1] = curl->orgPosition.y;
					tmplparam[lno][4][2] = curl->orgPosition.z;

					break;
				case D3DLIGHT_SPOT:
					tmplparam[lno][0][1] = 2.0f;

					tmplparam[lno][0][2] = curl->divRange;
					tmplparam[lno][1][0] = curl->cosPhi;
					tmplparam[lno][1][1] = curl->divcosPhi;

					tmplparam[lno][4][0] = curl->orgPosition.x;
					tmplparam[lno][4][1] = curl->orgPosition.y;
					tmplparam[lno][4][2] = curl->orgPosition.z;

					tmplparam[lno][5][0] = curl->orgDirection.x;
					tmplparam[lno][5][1] = curl->orgDirection.y;
					tmplparam[lno][5][2] = curl->orgDirection.z;

					break;
				}
					
				curl = curl->nextlight;
				setlno++;

			}else{
				tmplparam[lno][0][0] = 0.0f;
				tmplparam[lno][0][1] = -1.0f;
			}
		}

		float lbump[3][6][4];
		ZeroMemory( lbump, sizeof( float ) * 3 * 6 * 4 );

		int bulno;
		int bulsetno = 0;
		for( bulno = 0; bulno < 3; bulno++ ){
			CLightData* lbptr;
			lbptr = GetLightData( g_LightOfBump[ bulno ] );
			if( (bulno == 0) && !lbptr && g_lighthead ){
				lbptr = g_lighthead;//存在しなかったら先頭ライト
			}

			if( lbptr ){
				lbump[bulsetno][0][0] = 1.0f;

				lbump[bulsetno][2][0] = lbptr->Diffuse.x;
				lbump[bulsetno][2][1] = lbptr->Diffuse.y;
				lbump[bulsetno][2][2] = lbptr->Diffuse.z;

				lbump[bulsetno][3][0] = lbptr->Specular.x;
				lbump[bulsetno][3][1] = lbptr->Specular.y;
				lbump[bulsetno][3][2] = lbptr->Specular.z;

				switch( lbptr->Type ){
				case D3DLIGHT_DIRECTIONAL:
					lbump[bulsetno][0][1] = 0.0f;

					lbump[bulsetno][5][0] = lbptr->orgDirection.x;
					lbump[bulsetno][5][1] = lbptr->orgDirection.y;
					lbump[bulsetno][5][2] = lbptr->orgDirection.z;

					break;
				case D3DLIGHT_POINT:
					lbump[bulsetno][0][1] = 1.0f;

					lbump[bulsetno][0][2] = lbptr->divRange;


					lbump[bulsetno][4][0] = lbptr->orgPosition.x;
					lbump[bulsetno][4][1] = lbptr->orgPosition.y;
					lbump[bulsetno][4][2] = lbptr->orgPosition.z;

					break;
				case D3DLIGHT_SPOT:
					lbump[bulsetno][0][1] = 2.0f;

					lbump[bulsetno][0][2] = lbptr->divRange;
					lbump[bulsetno][1][0] = lbptr->cosPhi;
					lbump[bulsetno][1][1] = lbptr->divcosPhi;

					lbump[bulsetno][4][0] = lbptr->orgPosition.x;
					lbump[bulsetno][4][1] = lbptr->orgPosition.y;
					lbump[bulsetno][4][2] = lbptr->orgPosition.z;

					lbump[bulsetno][5][0] = lbptr->orgDirection.x;
					lbump[bulsetno][5][1] = lbptr->orgDirection.y;
					lbump[bulsetno][5][2] = lbptr->orgDirection.z;

					break;
				}

				bulsetno++;
			}
		}
		g_bumplightnum = bulsetno;//!!!!!!!!!!!!!!!!!!!!!!!!!!!

		if( g_bumplightnum == 0 ){

			//dummy data
			lbump[0][0][0] = 1.0f;

			lbump[0][2][0] = 0.0f;
			lbump[0][2][1] = 0.0f;
			lbump[0][2][2] = 0.0f;

			lbump[0][3][0] = 0.0f;
			lbump[0][3][1] = 0.0f;
			lbump[0][3][2] = 0.0f;

			///////directional
			lbump[0][0][1] = 0.0f;

			lbump[0][5][0] = 0.0f;
			lbump[0][5][1] = 0.0f;
			lbump[0][5][2] = 1.0f;

			g_bumplightnum = 1;
		}



		if( (g_chkVS >= 20) || (g_chkVS == 0) ){
			hres = g_pEffect->SetValue( g_hmLightParams, &(tmplparam[0][0][0]), sizeof( float ) * 3 * 6 * 4 );
			if( hres != D3D_OK ){
				DbgOut( "SetShaderConst : mLightParam SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			hres = g_pEffect->SetValue( g_hmBumpLight, &(lbump[0][0][0]), sizeof( float ) * 3 * 6 * 4 );
			if( hres != D3D_OK ){
				DbgOut( "SetShaderConst : mBumpLight SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

		}else{
			hres = g_pEffect->SetValue( g_hmLightParams, &(tmplparam[0][0][0]), sizeof( float ) * 1 * 6 * 4 );
			if( hres != D3D_OK ){
				DbgOut( "SetShaderConst : mLightParam SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		g_lightnum = setlno;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	}else{
		//shadowmap
		if( (g_chkVS >= 20) || (g_chkVS == 0) ){
			D3DXMATRIX matLP;
			matLP = m_matViewShadow * m_matProjShadow;
			hres = g_pEffect->SetMatrix( g_hmLP, &matLP );
			if( hres != D3D_OK ){
				DbgOut( "SetShaderConst : mLP SetMatrix error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			D3DXMATRIX matLPB;
			matLPB = matLP * m_matBiasShadow;
			hres = g_pEffect->SetMatrix( g_hmLPB, &matLPB );
			if( hres != D3D_OK ){
				DbgOut( "SetShaderConst : mLPB SetMatrix error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			hres = g_pEffect->SetValue( g_hmShadowCoef, &(m_shadowcoef[0]), sizeof( float ) * 2 );
			if( hres != D3D_OK ){
				DbgOut( "SetShaderConst : mShadowCoef SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			hres = g_pEffect->SetTexture( g_hShadowMap, m_shadowelem.pTex );
			if( hres != D3D_OK ){
				DbgOut( "SetShaderConst : ShadowMap SetTexture error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

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
	if( !g_pEffect || !g_pPostEffect ){
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

	g_hRenderZN_2path_NormalBone = g_pEffect->GetTechniqueByName( "RenderZN_2path_NormalBone" );
	g_hRenderZN_2path_NormalNoBone = g_pEffect->GetTechniqueByName( "RenderZN_2path_NormalNoBone" );


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


	g_hRenderSceneBone4Bump = g_pEffect->GetTechniqueByName( "RenderSceneBone4Bump" );//vs2
	g_hRenderSceneBone1Bump = g_pEffect->GetTechniqueByName( "RenderSceneBone1Bump" );//vs2
	g_hRenderSceneBone0Bump = g_pEffect->GetTechniqueByName( "RenderSceneBone0Bump" );//vs2
	g_hRenderSceneNormalShadow = g_pEffect->GetTechniqueByName( "RenderSceneNormalShadow" );//vs2
	g_hRenderSceneBumpShadow = g_pEffect->GetTechniqueByName( "RenderSceneBumpShadow" );//vs2
	g_hRenderSceneToon1Shadow = g_pEffect->GetTechniqueByName( "RenderSceneToon1Shadow" );//vs2
	g_hRenderSceneToon0Shadow = g_pEffect->GetTechniqueByName( "RenderSceneToon0Shadow" );//vs2
	g_hRenderSceneBone4PP = g_pEffect->GetTechniqueByName( "RenderSceneBone4PP" );
	g_hRenderSceneBone0PP = g_pEffect->GetTechniqueByName( "RenderSceneBone0PP" );
	g_hRenderScenePPShadow = g_pEffect->GetTechniqueByName( "RenderScenePPShadow" );

	g_hmBumpLight = g_pEffect->GetParameterByName( NULL, "mBumpLight" );//vs2
	g_hNormalMap = g_pEffect->GetParameterByName( NULL, "NormalMap" );//vs2
	g_hDecaleTex = g_pEffect->GetParameterByName( NULL, "DecaleTex" );//vs2
	g_hDecaleTex1 = g_pEffect->GetParameterByName( NULL, "DecaleTex1" );//vs2
	g_hmLP = g_pEffect->GetParameterByName( NULL, "mLP" );//vs2
	g_hmLPB = g_pEffect->GetParameterByName( NULL, "mLPB" );//vs2
	g_hShadowMap = g_pEffect->GetParameterByName( NULL, "ShadowMap" );//vs2
	g_hmShadowCoef = g_pEffect->GetParameterByName( NULL, "mShadowCoef" );//vs2
	g_hmoldView = g_pEffect->GetParameterByName( NULL, "moldView" );
	g_hmMinAlpha = g_pEffect->GetParameterByName( NULL, "mMinAlpha" );//vs2
	g_hmGlowMult = g_pEffect->GetParameterByName( NULL, "mGlowMult" );
	g_hmToonDiffuse = g_pEffect->GetParameterByName( NULL, "mToonDiffuse" );
	g_hmToonAmbient = g_pEffect->GetParameterByName( NULL, "mToonAmbient" );
	g_hmToonSpecular = g_pEffect->GetParameterByName( NULL, "mToonSpecular" );

/////////////// PostEffect
	g_hPostEffectBlur0 = g_pPostEffect->GetTechniqueByName( "PostEffectBlur0" );
	g_hPostEffectGauss = g_pPostEffect->GetTechniqueByName( "PostEffectBlurGauss" );
	g_hPostEffectTLV = g_pPostEffect->GetTechniqueByName( "PostEffectTLV" );

	g_hpeWidth0 = g_pPostEffect->GetParameterByName( NULL, "WIDTH0" );
	g_hpeHeight0 = g_pPostEffect->GetParameterByName( NULL, "HEIGHT0" );
	g_hpeWidth1 = g_pPostEffect->GetParameterByName( NULL, "WIDTH1" );
	g_hpeHeight1 = g_pPostEffect->GetParameterByName( NULL, "HEIGHT1" );
	g_hpeTexture0 = g_pPostEffect->GetParameterByName( NULL, "Texture0" );
	g_hpeWeight = g_pPostEffect->GetParameterByName( NULL, "weight" );
	g_hpeOffsetX = g_pPostEffect->GetParameterByName( NULL, "offsetX" );
	g_hpeOffsetY = g_pPostEffect->GetParameterByName( NULL, "offsetY" );
	g_hpeSampleWeight = g_pPostEffect->GetParameterByName( NULL, "avSampleWeights" );
	g_hpeSampleOffset = g_pPostEffect->GetParameterByName( NULL, "avSampleOffsets" );
	g_hpeColMult = g_pPostEffect->GetParameterByName( NULL, "colmult" );

	return 0;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------

LRESULT CMyD3DApplication::OnNeedText( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
//	static LPTOOLTIPTEXT lptip;
//	lptip = (LPTOOLTIPTEXT)pnmh;
//	lptip->hinst = (HINSTANCE)GetModuleHandle(NULL);

	static LPNMTTDISPINFO lptip;
	lptip = (LPNMTTDISPINFO)pnmh;
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
	case IDTBB_4:
		lptip->lpszText = MAKEINTRESOURCE(IDS_TB_4);
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
	case IDTBB_ZA4:
		lptip->lpszText = MAKEINTRESOURCE(IDS_TB_ZA4);
		break;
	case IDTBB_COLP:
		lptip->lpszText = MAKEINTRESOURCE(IDS_TB_COLP);
		break;
	case IDTBB_COLT:
		lptip->lpszText = MAKEINTRESOURCE(IDS_TB_COLT);
		break;
	case IDTBB_COLPP:
		lptip->lpszText = MAKEINTRESOURCE(IDS_TB_COLPP);
		break;
	case IDTBB_SHA:
		lptip->lpszText = MAKEINTRESOURCE(IDS_TB_SHA);
		break;
	case IDTBB_GLOW:
		lptip->lpszText = MAKEINTRESOURCE(IDS_TB_GLOW);
		break;

	default:
		break;
	}
	return DefWindowProc();
}
LRESULT CMyD3DApplication::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int type = (int)lParam;
	if( (type == MSGERR_APPMUSTEXIT) || (m_menucnt == 0) ){
DbgOut( "checkend : viewer : OnClose 0 \r\n" );
        Cleanup3DEnvironment();
DbgOut( "checkend : viewer : OnClose 1 \r\n" );
		if( ::IsWindow( m_hWnd ) ){
			::DestroyMenu( ::GetMenu(m_hWnd) );
		}
DbgOut( "checkend : viewer : OnClose 2 \r\n" );
		if( ::IsWindow( m_3dwnd ) ){
			::DestroyWindow( m_3dwnd );
		}
DbgOut( "checkend : viewer : OnClose 3 \r\n" );
		if( ::IsWindow( m_hWnd ) ){
			::DestroyWindow( m_hWnd );
		}
DbgOut( "checkend : viewer : OnClose 4 \r\n" );
        PostQuitMessage(0);
DbgOut( "checkend : viewer : OnClose end \r\n" );

		return 0;
	}else{
		return 0;
	}

}
LRESULT CMyD3DApplication::OnActivateApp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DbgOut( "checksize : OnActivateApp : uMsg %d, wParam %d, lParam %d, m_miniflag %d\r\n",
		uMsg, wParam, lParam, m_miniflag );

	BOOL kind;
	kind = (BOOL)wParam;

	DbgOut( "viewer : WM_ACTIVATEAPP %d %d\n", kind, m_bReady );

	if( g_endapp == 0 ){
		if( kind == 0 ){
			if( m_screensave == 1 ){

				InvalidateDeviceObjects( 1, 0 );
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
	}
	
	if( (m_miniflag == 1) && (wParam == 0) ){
		m_miniflag = 2;
		DbgOut( "checksize : OnActivateApp : miniflag 2 set\r\n" );
	}

	DefWindowProc();

	if( m_miniflag == 2 ){
		/***
		BOOL dummy;
		OnSysCommand( WM_SYSCOMMAND, SC_RESTORE, 0, dummy );

		BOOL dummy2;
		LPARAM lsize = 0;
		WORD wwidth;
		DWORD dheight;
		wwidth = (WORD)m_clientWidth;
		dheight = (DWORD)m_clientHeight;
		lsize = (dheight << 16) | wwidth;
		OnSize( WM_SIZE, 0, lsize, dummy2 );

		m_miniflag = 0;//!!!!!!!!!!
		DbgOut( "viewer : return from minimized\n" );
		***/
		m_miniflag = 0;//!!!!!!!!!!
	}
			
	return 0;
}
LRESULT CMyD3DApplication::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//::MessageBox( m_hWnd, "OnLButtonDown", "check", MB_OK );

	WORD kind = (WORD)wParam;
	//int ret;


	POINT ptCursor;
	GetCursorPos( &ptCursor );
	::ScreenToClient( m_3dwnd.m_hWnd, &ptCursor );


//	::SetForegroundWindow( m_3dwnd.m_hWnd );
	::SetCapture( m_3dwnd.m_hWnd );

	LButtonDownFunc( kind, (WORD)ptCursor.x, (WORD)ptCursor.y );


//	WORD xPos, yPos;
//	xPos = GET_X_LPARAM(lParam); 
//	yPos = GET_Y_LPARAM(lParam); 
//	
//	LButtonDownFunc( kind, xPos, yPos );

	return DefWindowProc();

}

int CMyD3DApplication::LButtonDownFunc( int kind, WORD xPos, WORD yPos )
{
	int ret;

//enum {
//	KAD_NORMAL,
//	KAD_T,
//	KAD_T2,
//	KAD_IKT,
//	KAD_MAX
//};

	m_keyanddrag = KAD_NORMAL;
	if( (m_tbb_r == 1) && (g_KeyBuffer['I'] & 0x80) ){
		m_keyanddrag = KAD_IKT;
	}else if( (m_tbb_r == 1) && (g_KeyBuffer['T'] & 0x80) ){
		m_keyanddrag = KAD_T;
	}else if( (m_tbb_r == 1) && (g_KeyBuffer['2'] & 0x80) ){
		m_keyanddrag = KAD_T2;
	}

	if( (kind & MK_LBUTTON) && (m_rbutton == 1) && (m_vikflag == 0) ){
		m_lrbutton = 1;
		
		m_lbutton = 0;
		m_movebone = 0;
		m_pickgp = 0;
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

		m_camope = CheckCamOpe( m_lpos );
		if( m_camope != CAMOPE_NONE ){
			return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}



		int targetjoint = 0;

		if( m_preview_flag == 0 ){
			int pickno = 0;
			
			if( (m_keyanddrag == KAD_T) || (m_keyanddrag == KAD_T2) ){

				pickno = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
				targetjoint = pickno;

			}else if( m_keyanddrag == KAD_IKT ){

				pickno = PickIKT( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone );
			
			}else if( (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_col == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) ){

				if( g_motdlg->m_disptabno == TAB_GP ){
					pickno = -2;
					m_pickgp = PickSelect( xPos, yPos, &m_select_kind );
				}else{
					pickno = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
					targetjoint = pickno;
				}

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
				case ITIOBJ_JOINTROT:
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

					pickno = 0;
					m_shiftpart = 0;
					ret = m_paintdlg->OnClickVert( m_paintdlg->m_targetdisp, m_paintdlg->m_targetvert, m_paintdlg->m_targetvertnum, 1 );
					if( ret ){
						DbgOut( "viewer : paintdlg : OnClickVert error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}else{
					m_shiftpart = PickBone( xPos, yPos, &m_select_kind, &m_clickoffset, &m_clickbone, 1 );
					pickno = m_shiftpart;
					targetjoint = pickno;
				}

			}


			if( pickno > 0 ){
				if( ((m_tbb_iti == 1) && ( (m_itidlg->m_object == ITIOBJ_JOINT) || (m_itidlg->m_object == ITIOBJ_JOINTROT) )) || 
					(m_tbb_bone == 1) || ((m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_BONE)) ){
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

				if( (m_keyanddrag == KAD_T) || (m_keyanddrag == KAD_T2) ){
					m_shiftbone = pickno;
				}else if( m_keyanddrag == KAD_IKT ){
					m_shiftbone = pickno;
				}else if( (m_tbb_r == 1) || ((m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_BONE)) || (m_tbb_4 == 1) ){
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

				if( (m_keyanddrag != KAD_IKT) &&
					(
					 ( (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_col == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) ) 
					 ||  ( (m_tbb_ei3 == 1) && (m_paintdlg->m_ope = PAINTOPE_BONE) ) 
					)
					){
					

					CShdElem* pickelem;
					pickelem = (*m_shandler)( pickno );
					_ASSERT( pickelem );

					ret = m_shandler->CalcZa4LocalQ( m_mhandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo() );
					if( ret ){
						DbgOut( "viewer : aft PickBone : sh CalcZa4LocalQ error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}


					CQuaternion selq;
					ret = m_mhandler->GetTotalQOnFrame( m_shandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), pickno, &selq );
					if( ret ){
						DbgOut( "viewer : aft PickBone : mh GetTotalQOnFrame error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					if( (m_tbb_za4 == 1) || (m_tbb_4 == 1) ){

						CShdElem* selelem;
						selelem = (*m_shandler)( pickno );
						
						ret = selelem->GetZa4LocalQ( &m_localq );//sh CalcZa4LocalQで計算。
						if( ret ){
							DbgOut( "viewer : aft PickBone : se GetZa4LocalQ error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
						
						//m_localq = m_shandler->m_gpdata.m_q * m_localq;
					}else if( m_tbb_za3 == 1 ){

						CQuaternion iniq;
						ret = m_shandler->GetInitialBoneQ( pickno, &iniq );
						if( ret ){
							DbgOut( "viewer : aft PickBone : sh GetInitialBoneQ error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}

						//m_localq = m_shandler->m_gpdata.m_q * selq * iniq;
						m_localq = selq * iniq;
						//m_localq = iniq * selq;

					}else{
						//m_localq = m_shandler->m_gpdata.m_q * selq;
						m_localq = selq;
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

			}else if( m_pickgp == 1 ){

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
				if( m_shandler ){
					m_localq = m_shandler->m_gpdata.m_q;
				}else{
					m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );
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
	return 0;

}

LRESULT CMyD3DApplication::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_keyanddrag = KAD_NORMAL;

	::SetCapture( m_3dwnd.m_hWnd );


	//wparam MK_RBUTTON
	//lparam low word x
	//lparam high word y
	WORD kind = (WORD)wParam;
	//int ret;

//	WORD xPos, yPos;
//	xPos = GET_X_LPARAM(lParam); 
//	yPos = GET_Y_LPARAM(lParam); 
//
//	RButtonDownFunc( kind, xPos, yPos );

	POINT ptCursor;
	GetCursorPos( &ptCursor );
	::ScreenToClient( m_3dwnd.m_hWnd, &ptCursor );

	RButtonDownFunc( kind, (WORD)ptCursor.x, (WORD)ptCursor.y );


	return DefWindowProc();
}

int CMyD3DApplication::RButtonDownFunc( int kind, WORD xPos, WORD yPos )
{
	//int ret;

	if( (kind & MK_RBUTTON) && (m_lbutton == 1) && (m_vikflag == 0) ){
		m_lrbutton = 1;

		m_lbutton = 0;
		m_movebone = 0;
		m_pickgp = 0;
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

				pickno = 0;
				m_shiftpart = 0;
				int ret;
				ret = m_paintdlg->OnClickVert( m_paintdlg->m_targetdisp, m_paintdlg->m_targetvert, m_paintdlg->m_targetvertnum, 0 );
				if( ret ){
					DbgOut( "viewer : paintdlg : OnClickVert error !!!\n" );
					_ASSERT( 0 );
					return 1;
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
					::ClientToScreen( m_3dwnd.m_hWnd, &pt );
					
					CShdElem* selem;
					selem = (*m_shandler)( pickno );

					CTreeElem2* telem;
					telem = (*m_thandler)( pickno );

					m_intormenu = 1;

					s_rmenumain->TrackPopupMenu( telem, selem, m_hWnd, g_inidata->opt_ikfk, pt );

					m_rbutton = 0;//!!!! ここで解除しないと押しっぱなし状態になる。

				}else{
					CTreeElem2* selte;
					selte = (*m_thandler)( pickno );
					_ASSERT( selte );

					m_bonedlg->SetCurrentName( selte->name );
				}

			}
		}
	}

	return 0;
}

LRESULT CMyD3DApplication::OnMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_keyanddrag = KAD_NORMAL;

	::SetCapture( m_3dwnd.m_hWnd );
	WORD kind = (WORD)wParam;

	WORD xPos, yPos;
	xPos = GET_X_LPARAM(lParam); 
	yPos = GET_Y_LPARAM(lParam); 

	MButtonDownFunc( kind, xPos, yPos );

	return DefWindowProc();
}

int CMyD3DApplication::MButtonDownFunc( int kind, WORD xPos, WORD yPos )
{
	if( (m_mbutton == 0) && (kind & MK_MBUTTON) && (m_vikflag == 0) ){
		m_mbutton = 1;

		m_mpos.x = xPos;
		m_mpos.y = yPos;
	}

	return 0;
}

LRESULT CMyD3DApplication::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	float vert;

	if( (m_tbb_iti == 1) && (m_itidlg->m_object == ITIOBJ_JOINT) && (m_itidlg->m_wheelscale == 1) ){

		if( m_shandler && m_ExistData ){
			float srcscale = 1.0f;

			int curdelta;
			curdelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if( curdelta >= 0 ){
				srcscale = 1.05f;
			}else{
				srcscale = 0.95f;
			}

			ret = m_itidlg->ScaleBoneTreePos( m_shandler, srcscale );
			_ASSERT( !ret );
		}

	}else{
		switch( g_inidata->opt_MouseW ){
		case MOUSEOPE_BONETWIST:
			if( (m_keyanddrag != KAD_IKT) && (m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) ){

				if( g_motdlg && m_shandler && (m_wheeldelta == 0.0f) ){
					if( (m_selectedno > 0) && (m_tbb_ei2 == 0) ){
						CopyUndoBuffer();
					}
				}

				//m_wheeldelta = (float)GET_WHEEL_DELTA_WPARAM(wParam) * (float)DEG2PAI * 0.02f;

				int curdelta;
				curdelta = GET_WHEEL_DELTA_WPARAM(wParam);
				if( curdelta >= 0 ){
					m_wheeldelta = (float)g_motdlg->GetRotRad() * (float)DEG2PAI;
				}else{
					m_wheeldelta = -1.0f * (float)g_motdlg->GetRotRad() * (float)DEG2PAI;
				}

				if( (m_selectedno > 0) && (m_tbb_ei2 == 0) ){
					ret = g_motdlg->OnTwist( m_matWorld, m_selectedno, m_wheeldelta, m_tbb_za4 );
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
	}

	return DefWindowProc();
}
LRESULT CMyD3DApplication::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_destroyflag != 0 ){
		return 0;
	}

	int timerid = (int)wParam;
	if( (timerid == MOVEBONETIMER) ){

		//status barへの表示
		char strjoint[512];
		sprintf_s( strjoint, 512, "FPS : %.02f", m_fFPS );
		SendMessage(g_hStatus, SB_SETTEXT, 0 | 0, (LPARAM)strjoint);
		
		if( g_motdlg ){
			//D3DXVECTOR3 jpos;
			//int setflag;

			//g_motdlg->GetSelectedJointPos( &m_selectedno, &jpos );//worldmat == inimat// <---- FrameMoveへ移動。
			if( g_motdlg->m_disptabno == TAB_GP ){
				if( m_shandler ){
					sprintf_s( strjoint, 512, "selected joint pos : (%f, %f, %f)", m_selectedpos.x, m_selectedpos.y, m_selectedpos.z );
				}else{
					sprintf_s( strjoint, 512, "selected joint pos : (not selected)" );
				}
			}else{
				if( (m_tbb_ikt == 0) && (m_keyanddrag != KAD_IKT) && (m_tbb_ei3 == 0) ){			
					if( m_selectedno > 0 ){
						sprintf_s( strjoint, 512, "selected joint pos : (%f, %f, %f)", m_selectedpos.x, m_selectedpos.y, m_selectedpos.z );
					}else{
						sprintf_s( strjoint, 512, "selected joint pos : (not selected)" );
					}
				}else if( (m_tbb_ikt == 1) || (m_keyanddrag == KAD_IKT) ){
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
//						_ASSERT( 0 );
						return 1;
					}

					char* szName;
					int cookie = g_motdlg->GetMotCookie();
					if( (cookie >= 0) && (cookie < m_mhandler->m_kindnum) ){
						szName = *(miptr->motname + cookie );
						SendMessage( g_hStatus, SB_SETTEXT, 2 | 0, (LPARAM)szName );
					}
				}
			}

		}

	///////////
		
		POINT ptCursor;
		GetCursorPos( &ptCursor );
		::ScreenToClient( m_3dwnd.m_hWnd, &ptCursor );


		CQuaternion biniq( 1.0f, 0.0f, 0.0f, 0.0f );
		int localflag = 0;

		int ret;

		if( m_camope != CAMOPE_NONE ){

			ret = CamOpe( ptCursor );
			_ASSERT( !ret );

		}else{
			if( g_motdlg->m_disptabno == TAB_GP ){
				if( (m_lbutton == 1) && (m_pickgp == 1) ){
					if( !m_thandler || !m_shandler || !m_mhandler )
						return -1;
					if( m_ExistData == FALSE )
						return -1;
					if( !g_motdlg )
						return 0;
					localflag = 1;

					if( m_tbb_r ){

						switch( m_select_kind ){
						case SELECT_X:
							ret = GetRotateDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );
							if( ret ){
								DbgOut( "viewer : GetRotateDelta error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
							if( m_select_delta != 0.0f ){
								ret = g_motdlg->m_gpdlg->RotateGP( 1, m_select_delta, m_localq );//Yaxis
								if( ret ){
									DbgOut( "viewer : motdlg RotateGP error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}
							}
							m_lpos.x = ptCursor.x;
							m_lpos.y = ptCursor.y;
							break;
						case SELECT_Y:
							ret = GetRotateDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );
							if( ret ){
								DbgOut( "viewer : GetRotateDelta error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
							if( m_select_delta != 0.0f ){
								ret = g_motdlg->m_gpdlg->RotateGP( 2, m_select_delta, m_localq );//Zaxis
								if( ret ){
									DbgOut( "viewer : motdlg RotateGP error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}
							}
							m_lpos.x = ptCursor.x;
							m_lpos.y = ptCursor.y;
							break;
						case SELECT_Z:
							ret = GetRotateDelta( SELECT_Z, m_lpos, ptCursor, &m_select_delta );
							if( ret ){
								DbgOut( "viewer : GetRotateDelta error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
							if( m_select_delta != 0.0f ){
								ret = g_motdlg->m_gpdlg->RotateGP( 0, m_select_delta, m_localq );//Xaxis
								if( ret ){
									DbgOut( "viewer : motdlg RotateGP error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}
							}
							m_lpos.x = ptCursor.x;
							m_lpos.y = ptCursor.y;
							break;
						default:
							break;
						}					
						FrameMove();
						if( m_bActive && m_bReady ){
							Render3DEnvironment( 1 );
						}

					}else if( m_tbb_t || m_tbb_t2 ){
						switch( m_select_kind ){
						case SELECT_X:
							GetShiftDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );
							m_select_delta *= g_inidata->gpa_move;
							ret = g_motdlg->m_gpdlg->ShiftGP( 0, m_select_delta, m_localq );
							if( ret ){
								DbgOut( "viewer : motdlg ShiftGP error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
							m_lpos.x = ptCursor.x;
							m_lpos.y = ptCursor.y;
							break;
						case SELECT_Y:
							GetShiftDelta( SELECT_Y, m_lpos, ptCursor, &m_select_delta );
							m_select_delta *= g_inidata->gpa_move;
							ret = g_motdlg->m_gpdlg->ShiftGP( 1, m_select_delta, m_localq );
							if( ret ){
								DbgOut( "viewer : motdlg ShiftGP error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
							m_lpos.x = ptCursor.x;
							m_lpos.y = ptCursor.y;
							break;
						case SELECT_Z:
							GetShiftDelta( SELECT_Z, m_lpos, ptCursor, &m_select_delta );
							m_select_delta *= g_inidata->gpa_move;
							ret = g_motdlg->m_gpdlg->ShiftGP( 2, m_select_delta, m_localq );
							if( ret ){
								DbgOut( "viewer : motdlg ShiftGP error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
							m_lpos.x = ptCursor.x;
							m_lpos.y = ptCursor.y;
							break;
						default:
							break;
						}
						FrameMove();
						if( m_bActive && m_bReady ){
							Render3DEnvironment( 1 );
						}
					}
				}
			}else{
				if( (m_lbutton == 1) && (m_rbutton == 0) && (m_movebone > 0) && (m_tbb_4 == 0) ){

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

							float scx, scy;
							scx = (float)( ptCursor.x - m_clickoffset.x );
							scy = (float)( ptCursor.y - m_clickoffset.y );


							if( m_tbb_za4 ){
								ret = g_motdlg->MoveBoneZa4( m_inimat, m_movebone, scx, scy, &lastparent );
								if( ret ){
									DbgOut( "viewer : motdlg MoveBone error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}
							}else{
								ret = g_motdlg->MoveBone( m_inimat, m_movebone, scx, scy, &lastparent );
								if( ret ){
									DbgOut( "viewer : motdlg MoveBone error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}
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

						if( m_select_delta != 0.0f ){
							ret = g_motdlg->RotateBone( m_inimat, m_movebone, 1, m_select_delta, &lastparent, localflag, m_localq, g_inidata->opt_ikfk, m_tbb_za4 );//Yaxis
							if( ret ){
								DbgOut( "viewer : motdlg RotateBone error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
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


						if( m_select_delta != 0.0f ){
							if( m_tbb_za3 == 0 ){
								ret = g_motdlg->RotateBone( m_inimat, m_movebone, 2, m_select_delta, &lastparent, localflag, m_localq, g_inidata->opt_ikfk, m_tbb_za4 );//Zaxis
								if( ret ){
									DbgOut( "viewer : motdlg RotateBone error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}
							}else{
								//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
								ret = g_motdlg->OnTwist( m_matWorld, m_movebone, m_select_delta, m_tbb_za4 );
								if( ret ){
									DbgOut( "viewer : motdlg OnTwist error !!!\n" );
									_ASSERT( 0 );
									return 1;
								}
								//_ASSERT( 0 );
							}
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

						if( m_select_delta != 0.0f ){
							ret = g_motdlg->RotateBone( m_inimat, m_movebone, 0, m_select_delta, &lastparent, localflag, m_localq, g_inidata->opt_ikfk, m_tbb_za4 );//Xaxis
							if( ret ){
								DbgOut( "viewer : motdlg RotateBone error !!!\n" );
								_ASSERT( 0 );
								return 1;
							}
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
				}else if( (m_lbutton == 1) && (m_rbutton == 0) && 
					((m_shiftbone > 0) && (m_tbb_iti == 0) && (m_tbb_ikt == 0) && (m_keyanddrag != KAD_IKT) && (m_tbb_col == 0) && (m_tbb_bone == 0) && 
					(m_tbb_ei2 == 0) && (m_tbb_ei3 == 0) && (m_tbb_4 == 0)) ){

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

					int shiftall = 0;
					if( (m_tbb_t2 == 1) || (m_keyanddrag == KAD_T2) ){
						shiftall = 1;
					}

					switch( m_select_kind ){
					case SELECT_CENTER:

						if( (abs( m_clickbone.x - (ptCursor.x - m_clickoffset.x) ) >= 2) || (abs( m_clickbone.y - (ptCursor.y - m_clickoffset.y) ) >= 2) ){

							ret = g_motdlg->ShiftBone( m_shiftbone, (float)( ptCursor.x - m_clickoffset.x ), (float)( ptCursor.y - m_clickoffset.y ), shiftall, m_tbb_za4 );
							_ASSERT( !ret );

							//m_lpos.x = ptCursor.x;
							//m_lpos.y = ptCursor.y;
						}
						break;
					case SELECT_X:
						GetShiftDelta( SELECT_X, m_lpos, ptCursor, &m_select_delta );

						ret = g_motdlg->ShiftBone( m_shiftbone, 0, m_select_delta, shiftall, localflag, m_localq, m_tbb_za4 );
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

						ret = g_motdlg->ShiftBone( m_shiftbone, 1, m_select_delta, shiftall, localflag, m_localq, m_tbb_za4 );
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

						ret = g_motdlg->ShiftBone( m_shiftbone, 2, m_select_delta, shiftall, localflag, m_localq, m_tbb_za4 );
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
				}else if( (m_lbutton == 1) && (m_rbutton == 0) && 
					((m_shiftbone > 0) && (m_tbb_ikt == 0) && (m_keyanddrag != KAD_IKT) && (m_tbb_ei2 == 0) && 
					((m_tbb_iti == 1) || (m_tbb_bone == 1)) && (m_tbb_col == 0)) ){


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

				}else if( (m_lbutton == 1) && (m_rbutton == 0) && 
					((m_shiftbone > 0) && (m_tbb_iti == 0) && ((m_tbb_ikt == 1) || (m_keyanddrag == KAD_IKT)) && (m_tbb_col == 0)) ){


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
			}
		}
///////////      カメラの操作

		if( (m_lrbutton == 1) && (m_movebone <= 0) && (m_shiftbone <= 0) && (m_scalebone <= 0)){

			float diffx, diffy;
			RECT clirect;
			float vert;

			diffx = (float)ptCursor.x - (float)m_lrpos.x;
			diffy = (float)ptCursor.y - (float)m_lrpos.y;
			::GetClientRect( m_3dwnd.m_hWnd, &clirect );
			//vert = 2000.0f * diffy / (float)(clirect.bottom - clirect.top);
			//vert = 2000.0f * (diffx + diffy) / (float)(clirect.bottom - clirect.top);
			vert = (diffx + diffy) * g_inidata->camdlg_move;

			OnCameraDist( vert );

			m_lrpos = ptCursor;


		}else if( (m_lbutton == 1) && (m_rbutton == 0) && (m_movebone <= 0) && (m_shiftbone <= 0) && (m_scalebone <= 0)){

			m_lpos = ptCursor;

			Render3DEnvironment( 0 );


		}else if( (m_rbutton == 1) && (m_lbutton == 0) && (m_movebone <= 0) && (m_shiftbone <= 0) && (m_scalebone <= 0) && (m_intormenu <= 0)){
			//float diffx, diffy;
			//RECT clirect;
			//float vert;
			OnCameraRot( ptCursor, m_rpos );
/***
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
					::GetClientRect( m_3dwnd.m_hWnd, &clirect );
					//vert = 2000.0f * diffy / (float)(clirect.bottom - clirect.top);
					vert = 2000.0f * (diffx + diffy) / (float)(clirect.bottom - clirect.top);
					OnCameraDist( vert );
					break;
				default:
					break;
				}
			}
***/
			m_rpos = ptCursor;

		}else if( (m_mbutton == 1) && (m_intormenu <= 0)){
			OnCameraMove( ptCursor, m_mpos );
			m_mpos = ptCursor;
		}
	}
	return DefWindowProc();
}
LRESULT CMyD3DApplication::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//::MessageBox( m_hWnd, "OnLButtonUp", "check", MB_OK );

	if( m_lbutton == 1 ){
		if( m_tbb_r || m_tbb_t || m_tbb_t2 ){
			if( (m_movebone > 0) || (m_shiftbone > 0) || (m_scalebone > 0) ){
				OnPluginPose();
			}
		}
	}



	m_keyanddrag = KAD_NORMAL;

	WORD kind = (WORD)wParam;

	if( m_lbutton == 1 ){
		WORD xPos, yPos;
		xPos = GET_X_LPARAM(lParam); 
		yPos = GET_Y_LPARAM(lParam); 

		//DbgOut( "viewer : WM_LBUTTONUP %d %d\n", xPos, yPos );

		//if( (m_movebone > 0) || (m_shiftbone > 0) || (m_scalebone > 0) ){			
			g_motdlg->UpdateAllMatrix( 0 );//ik後の更新
		//}
		m_lbutton = 0;
		m_movebone = 0;
		m_pickgp = 0;
		m_shiftbone = 0;
		m_scalebone = 0;

		m_localq.SetParams( 1.0f, 0.0f, 0.0f, 0.0f );

		if( m_tbb_ei2 == 1 ){
			m_infscopedlg->SetPoint();
		}

	}
	m_lrbutton = 0;
	m_select_kind = SELECT_NONE;

	m_camope = CAMOPE_NONE;

	SetSelectColor( 0 );

	ReleaseCapture();
	
	return DefWindowProc();

}
LRESULT CMyD3DApplication::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_keyanddrag = KAD_NORMAL;

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

		//if( (m_movebone > 0) || (m_shiftbone > 0) || (m_scalebone > 0) ){			
			g_motdlg->UpdateAllMatrix( 0 );//ik後の更新
		//}

	}
	m_lrbutton = 0;
	m_select_kind = SELECT_NONE;

	m_intormenu = 0;
	m_camope = CAMOPE_NONE;
	
	SetSelectColor( 0 );

	ReleaseCapture();

	return DefWindowProc();
}
LRESULT CMyD3DApplication::OnMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_keyanddrag = KAD_NORMAL;

	if( m_mbutton == 1 ){
		WORD xPos, yPos;
		xPos = GET_X_LPARAM(lParam); 
		yPos = GET_Y_LPARAM(lParam); 

		m_mbutton = 0;
		//g_motdlg->UpdateAllMatrix();//ik後の更新

	}
	
	m_camope = CAMOPE_NONE;	
	//m_select_kind = SELECT_NONE;
	//SetSelectColor( 0 );
	ReleaseCapture();

	return DefWindowProc();
}

LRESULT CMyD3DApplication::OnUserCameraMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int selindex;
	selindex = (int)wParam;

	if( selindex < 0 ){
		return 0;//!!!!!!!!!
	}

	_ASSERT( m_cameramenu );

	int iAnimSet, cAnimSets;
	cAnimSets = GetMenuItemCount( m_cameramenu );
	for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
	{
		RemoveMenu(m_cameramenu, 0, MF_BYPOSITION);
	}

	cAnimSets = g_motdlg->GetCameraAnimNum();

	if( cAnimSets <= 0 ){
		return 0;//!!!!!!!!!!!!!!!!!!!
	}

	MOTID* motidarray;
	motidarray = g_motdlg->GetCameraMotIDArray();
	_ASSERT( motidarray );


	char* szName;
	for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
	{
		int cookie;
		cookie = (motidarray + iAnimSet)->id;
		szName = g_motdlg->GetCameraAnimName( cookie );

		if (szName != NULL)
			AppendMenu(m_cameramenu, MF_STRING, 60500 + iAnimSet, szName);
		else
			AppendMenu(m_cameramenu, MF_STRING, 60500 + iAnimSet, "<No Animation Name>");
	}

	if( cAnimSets > 0 ){
		CheckMenuItem(m_appmenu, 60500 + selindex, MF_CHECKED);
	}

	return 0;
}

LRESULT CMyD3DApplication::OnUserMOEMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int selindex;
	selindex = (int)wParam;

	if( selindex < 0 ){
		return 0;//!!!!!!!!!
	}

	_ASSERT( m_moemenu );

	int iAnimSet, cAnimSets;
	cAnimSets = GetMenuItemCount( m_moemenu );
	for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
	{
		RemoveMenu(m_moemenu, 0, MF_BYPOSITION);
	}

	cAnimSets = g_motdlg->GetMOEAnimNum();

	if( cAnimSets <= 0 ){
		return 0;//!!!!!!!!!!!!!!!!!!!
	}

	MOTID* motidarray;
	motidarray = g_motdlg->GetMOEMotIDArray();
	_ASSERT( motidarray );


	char* szName;
	for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
	{
		int cookie;
		cookie = (motidarray + iAnimSet)->id;
		szName = g_motdlg->GetMOEAnimName( cookie );

		if (szName != NULL)
			AppendMenu(m_moemenu, MF_STRING, 61800 + iAnimSet, szName);
		else
			AppendMenu(m_moemenu, MF_STRING, 61800 + iAnimSet, "<No Animation Name>");
	}

	if( cAnimSets > 0 ){
		CheckMenuItem(m_appmenu, 61800 + selindex, MF_CHECKED);
	}

	return 0;
}

LRESULT CMyD3DApplication::OnUserGPMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int selindex;
	selindex = (int)wParam;

	if( selindex < 0 ){
		return 0;//!!!!!!!!!
	}

	_ASSERT( m_gpmenu );

	int iAnimSet, cAnimSets;
	cAnimSets = GetMenuItemCount( m_gpmenu );
	for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
	{
		RemoveMenu(m_gpmenu, 0, MF_BYPOSITION);
	}

	cAnimSets = g_motdlg->GetGPAnimNum();

	if( cAnimSets <= 0 ){
		return 0;//!!!!!!!!!!!!!!!!!!!
	}

	MOTID* motidarray;
	motidarray = g_motdlg->GetGPMotIDArray();
	_ASSERT( motidarray );


	char* szName;
	for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
	{
		int cookie;
		cookie = (motidarray + iAnimSet)->id;
		szName = g_motdlg->GetGPAnimName( cookie );

		if (szName != NULL)
			AppendMenu(m_gpmenu, MF_STRING, 61000 + iAnimSet, szName);
		else
			AppendMenu(m_gpmenu, MF_STRING, 61000 + iAnimSet, "<No Animation Name>");
	}

	if( cAnimSets > 0 ){
		CheckMenuItem(m_appmenu, 61000 + selindex, MF_CHECKED);
	}

	return 0;
}


LRESULT CMyD3DApplication::OnUserAnimMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
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

	return 0;
}
LRESULT CMyD3DApplication::OnUserBoneDisp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ToggleBoneDisp();
	return 0;
}
LRESULT CMyD3DApplication::OnUserError(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES* mes = (ERRORMES*)lParam;

	SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

	DisplayErrorMsg( (HRESULT)(mes->errorcode), mes->type, mes->mesptr );
	
	if( (mes->type != MSGERR_APPMUSTEXIT) && m_hWnd )
		SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	return 0;


}
LRESULT CMyD3DApplication::OnUserDisplay(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_bActive && m_bReady 
		//&& !m_moving_cameradlg
		//&& !m_moving_sigdlg 
		){

		if( g_endapp == 1 ){
			return 0;
		}
		Render3DEnvironment();
	}		
	return 0;
}
LRESULT CMyD3DApplication::OnUserEnableMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
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
	return 0;
}
/***
LRESULT CMyD3DApplication::OnUserKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_bActive && m_bReady ){
		int keyflag = (int)wParam;
		ChangeViewParam( keyflag );
		//Render3DEnvironment();
	}
	return 0;
}
***/

LRESULT CMyD3DApplication::OnUserMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
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

//	if( redrawflag[0] && g_inidata->camdlg_disp ){
//		m_cameradlg->Redraw();
//	}
	if( redrawflag[1] && g_inidata->sigdlg_disp ){
		m_sigeditwnd->Redraw();
	}
	if( redrawflag[2] && g_inidata->motdlg_disp ){
		if( g_motdlg )
			g_motdlg->Redraw();
	}
	return 0;
}
LRESULT CMyD3DApplication::OnUserSelchange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret = 0;
	int selno = (int)lParam;
	int msgfrom = (int)wParam;
	int pmsonomama;
	if( (m_tbb_ei2 == 1) || (m_tbb_ei3 == 1) ){
		pmsonomama = 1;
	}else{
		pmsonomama = 0;
	}

	ret = m_shandler->SetIsSelected( selno, 1, pmsonomama );
	if( ret ){
		DbgOut( "MsgProc : WM_USER_SELCHANGE : SetIsSelected error !!!\n" );
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

	return 0;
}
LRESULT CMyD3DApplication::OnUserInfluence(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int serino = (int)wParam;
	int ret = 0;
	ERRORMES errormes;
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

	return 0;
}
LRESULT CMyD3DApplication::OnUserRemakeTree(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//BoneInfoの作り直し。
	int ret;
	ERRORMES errormes;
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
		
		if( m_remaketree_calc0 == 0 ){
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

	return 0;
}
LRESULT CMyD3DApplication::OnUserChangeNotUse(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//BoneInfoの作り直し。
	int ret;
	ERRORMES errormes;
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserNewJoint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret = 0;
	int addnum = (int)wParam;
	ERRORMES errormes;

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
	return 0;
}
LRESULT CMyD3DApplication::OnUserDelJoint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserConv2Scope(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserChangeJointLoc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
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
		ret = g_motdlg->UpdateAllMatrix( 0 );
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

}
LRESULT CMyD3DApplication::OnUserCalcEul(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );//!!!!!!!!!!!!!!!!!
	_ASSERT( !ret );
	return 0;


}
LRESULT CMyD3DApplication::OnUserRemakeTexture(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
	if( m_shandler ){
		int ret = 0;
		int serino = (int)wParam;
		//CShdElem* selem = (*m_shandler)( serino );
		//ret += selem->SetTexName( (char*)lParam );
		//_ASSERT( !ret );
		
		ret += m_shandler->InvalidateTexture();
		_ASSERT( !ret );

		ret += m_shandler->CreateTexture( m_pd3dDevice, m_hWnd, 1, g_miplevels, g_mipfilter, 1, 0 );
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserRename(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
	if( m_thandler ){
		int ret = 0;
		int serino = (int)wParam;
		CTreeElem2* telem = (*m_thandler)( serino );
		ret += telem->SetName( (char*)lParam );
		_ASSERT( !ret );

//		if( g_motdlg ){
//			ret += g_motdlg->Rename( serino, (char*)lParam );
//			_ASSERT( !ret );
//		}

		if( ret ){
			errormes.errorcode = -1;
			errormes.type = MSGERR_APPMUSTEXIT;
			errormes.mesptr = "MsgProc : WM_USER_RENAME error";
			SendMessage( m_hWnd, WM_USER_ERROR, 0, (LPARAM)&errormes );
			return -1;
		}


		if( !telem->IsJoint() ){
			//「-」の名前に対処するために再計算をする。
			//リネームしたパーツがジョイントだった場合は再計算しない。
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}
	}
	return 0;
}
int CMyD3DApplication::RemakeAllDispObj()
{
	int ret;
	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* selem;
		selem = (*m_shandler)( seri );
		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){
			ret = selem->InvalidateDispObj();
			_ASSERT( !ret );

			ret = m_shandler->SetMikoAnchorApply( m_thandler );
			_ASSERT( !ret );

			ret = selem->CreateDispObj( m_shandler->m_seri2boneno, m_pd3dDevice, m_mhandler, m_shandler, s_tlmode, 1 );
			_ASSERT( !ret );
		}
	}
	return 0;
}


LRESULT CMyD3DApplication::OnUserRemakeDispObj(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserRemakeUV(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserEndDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int dlgno = (int)wParam;
	switch( dlgno ){
	case ID_MSG_FROM_CAMERADLG:
//		if( g_inidata->camdlg_disp ){
//			::CheckMenuItem( m_appmenu, IDMENU_CAMERA, MF_UNCHECKED );
//			UpdateDialogIniData( dlgno );
//
//			g_inidata->camdlg_disp = 0;
//		}
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

	return 0;
}
LRESULT CMyD3DApplication::OnUserChangeColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserPreview(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_preview_flag = (int)lParam;
	return 0;
}
LRESULT CMyD3DApplication::OnUserSelLock(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_sellockflag = (int)wParam;
	char* nameptr = (char*)lParam;

	if( m_sellockflag ){
		_stprintf_s( m_strSellock, 1024, _T("!! Under Select Lock !!") );
	}else{
		_stprintf_s( m_strSellock, 1024, _T("") );
	}

	DbgOut( "viewer : WM_USER_SELLOCK : m_sellockflag %d, nameptr %s\n", m_sellockflag, nameptr );

	return 0;
}
LRESULT CMyD3DApplication::OnUserInitUndoBuf(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( lParam == 1 ){
		EnableMenuItem( m_appmenu, IDMENU_IKUNDO, MF_BYCOMMAND | MF_GRAYED );
		EnableMenuItem( m_appmenu, IDMENU_REVIKUNDO, MF_BYCOMMAND | MF_GRAYED );

		m_ik_cnt = 0;
		m_undo_cnt = 0;
		m_undo_init_flag = 1;
	}else{
		//m_undo_cnt++;
	}
	return 0;
}
LRESULT CMyD3DApplication::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
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

	return 0;
}
LRESULT CMyD3DApplication::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
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
	return 0;
}
LRESULT CMyD3DApplication::OnUserCreatePB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = CreateProgressBar();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}
	return 0;
}
LRESULT CMyD3DApplication::OnUserSetPB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	int newpos = (int)wParam;
	ret = SetProgressBar( newpos );
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}
	return 0;
}
LRESULT CMyD3DApplication::OnUserDestroyPB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = DestroyProgressBar();
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}
	return 0;
}
LRESULT CMyD3DApplication::OnUserImportMQO(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = ImportMqoFile( (LOADFILEINFO*)lParam, 0 );
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}
	return 0;
}
LRESULT CMyD3DApplication::OnUserLoadBon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;
	ret = LoadBoneFile( (LOADFILEINFO*)lParam );
	if( ret ){
		_ASSERT( 0 );
		return -1;
	}
	return 0;
}
LRESULT CMyD3DApplication::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ERRORMES errormes;
	BOOL dummy;
	NMUPDOWN nmud;
	int ret;

	WORD menuid;
	menuid = LOWORD( wParam );

	//55000
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
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}

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
	if( (menuid >= 60500) && (menuid <= (60500 + MAXMOTIONNUM - 1)) ){
		int selindex;
		selindex = menuid - 60500;
		
		int motnum;
		motnum = g_motdlg->GetCameraAnimNum();

		if( selindex >= motnum ){
			return 0;//!!!!!!!!!!!!!
		}

		ret = g_motdlg->SelectCameraAnim( selindex );
		if( ret ){
			DbgOut( "viewer : CameraMenu : SelectCameraAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		return 0;
	}
	if( (menuid >= 61000) && (menuid <= (61000 + MAXMOTIONNUM - 1)) ){
		int selindex;
		selindex = menuid - 61000;
		
		int motnum;
		motnum = g_motdlg->GetGPAnimNum();

		if( selindex >= motnum ){
			return 0;//!!!!!!!!!!!!!
		}

		ret = g_motdlg->SelectGPAnim( selindex );
		if( ret ){
			DbgOut( "viewer : GPMenu : SelectGPAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		return 0;
	}
	if( (menuid >= 61800) && (menuid <= (61800 + MAXMOTIONNUM - 1)) ){
		int selindex;
		selindex = menuid - 61800;
		
		int motnum;
		motnum = g_motdlg->GetMOEAnimNum();

		if( selindex >= motnum ){
			return 0;//!!!!!!!!!!!!!
		}

		ret = g_motdlg->SelectMOEAnim( selindex );
		if( ret ){
			DbgOut( "viewer : MOEMenu : SelectMOEAnim error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		return 0;
	}

	///

	int selindex, curindex, motnum;

	CInfoDlg infodlg;

	switch( menuid )
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///59900からMAXMOTIONNUM(300)分、つまり60200までは使用禁止。アニメーションメニュー
///60500からMAXMOTIONNUM(300)分、つまり60800までは使用禁止。カメラアニメーションメニュー
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
	case 5430:
		ret = 0;
		if( m_shandler && (s_registflag == 1) ){
			ret = SaveFBXFile();
			_ASSERT( !ret );
		}
		break;
	case 54030:
		ret = 0;
		RegistKey();
		break;
	case 54014:
		ret = 0;
		if( m_kinect && m_shandler && (g_motdlg->GetMotionNum() > 0) && (m_kinectflag == 0) ){
			ret = m_kinect->StartCapture( 0 );
			if( ret == 0 ){
				m_kinectflag = 1;
			}else{
				m_kinectflag = 0;
				_ASSERT( 0 );
			}
		}
		break;
	case 54015:
		ret = 0;
		if( m_kinect && m_shandler && (g_motdlg->GetMotionNum() > 0) && (m_kinectflag == 0) ){
			ret = m_kinect->StartCapture( 1 );
			if( ret == 0 ){
				m_kinectflag = 2;
			}else{
				m_kinectflag = 0;
				_ASSERT( 0 );
			}
		}
		break;
	case 54017:
		ret = 0;
		if( m_kinect && m_shandler && (g_motdlg->GetMotionNum() > 0) && (m_kinectflag == 0) ){
			ret = m_kinect->StartCapture( 2 );
			if( ret == 0 ){
				m_kinectflag = 3;
			}else{
				m_kinectflag = 0;
				_ASSERT( 0 );
			}
		}
		break;

	case 54011:
		ret = 0;
		if( m_kinect && (m_kinectflag == 1) ){
			m_kinect->EndCapture();
			m_kinectflag = 0;
		}
		break;
	case 54012:
		ret = 0;
		if( m_kinect ){
			ret = m_kinect->LoadTxtSetting();
		}
		break;
	case 54013:
		ret = 0;
		if( m_kinect ){
			ret = m_kinect->EditTxtSetting();
		}
		break;
	case 54016:
		ret = 0;
		if( m_kinect && m_shandler && (g_motdlg->GetMotionNum() > 0) ){
			ret = m_kinect->ApplyFilter();
		}
		break;

	case 54020:
		ret = 0;
		m_optdlg->m_usekinect = 1;
		g_inidata->usekinect = 1;

		::MessageBoxA( m_hWnd, "Kinectを使用する設定にしました。\nキャプチャ開始のメニューが選択できない場合は\nRokDeBone2を再起動してください。", "設定しました", MB_OK );
		break;
	case 54021:
		ret = 0;

		m_optdlg->m_usekinect = 0;
		g_inidata->usekinect = 0;

		::MessageBoxA( m_hWnd, "Kinectを使用しない設定にしました。次回起動時から反映されます。", "設定しました", MB_OK );
		break;

	case 5300:
		ret = 0;
		if( m_sigeditwnd ){
			ret = m_sigeditwnd->OnMenuShdParam();
			_ASSERT( !ret );
		}
		break;
	case 5301:
		ret = 0;
		//if( m_sigeditwnd ){
		//	ret = m_sigeditwnd->OnMenuDelJoint();
		//	_ASSERT( !ret );
		//}
		ret = DelTreeElem( -1, 1 );
		_ASSERT( !ret );
		break;
	case 5302:
		if( m_sigeditwnd ){
			m_sigeditwnd->OnMenuConv2Scope();
		}
		break;
	case 5303:
		ret = DelTree();
		_ASSERT( !ret );
		break;
	case 51010:
		SetShadowParams();
		break;

	case 54000:
		SetSoundBank();
		break;

	case 59000://新規空
		NewMotion();
		break;
	case 59001://全削除
		ret = (int)::MessageBox( m_hWnd, "全てのモーションを削除します。\nよろしいですか？", "確認", MB_OKCANCEL );
		if( ret == IDOK ){
			DestroyMotion( 1 );
		}
		break;
	case 59002://カレント削除
		DestroyMotion( 0 );
		break;
	case 59005:
		MotCut();
		break;
	case 59006:
		MotMabiki();
		break;

	case 59010:
		ret = NewCameraAnim();
		_ASSERT( !ret );
		break;
	case 59011:
		ret = (int)::MessageBox( m_hWnd, "全てのカメラアニメを削除します。\nよろしいですか？", "確認", MB_OKCANCEL );
		if( ret == IDOK ){
			DestroyCameraAnim( 1 );
		}
		break;
	case 59012:
		ret = DestroyCameraAnim( 0 );
		_ASSERT( !ret );
		break;

	case 59020:
		ret = NewGPAnim();
		_ASSERT( !ret );
		break;
	case 59021:
		ret = (int)::MessageBox( m_hWnd, "全てのGlobalPositionアニメを削除します。\nよろしいですか？", "確認", MB_OKCANCEL );
		if( ret == IDOK ){
			DestroyGPAnim( 1 );
		}
		break;
	case 59022:
		ret = DestroyGPAnim( 0 );
		_ASSERT( !ret );
		break;

	case 59030:
		ret = NewMOEAnim();
		_ASSERT( !ret );
		break;
	case 59031:
		ret = (int)::MessageBox( m_hWnd, "全てのmoaイベントアニメを削除します。\nよろしいですか？", "確認", MB_OKCANCEL );
		if( ret == IDOK ){
			DestroyMOEAnim( 1 );
		}
		break;
	case 59032:
		ret = DestroyMOEAnim( 0 );
		_ASSERT( !ret );
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
	case 5042:
		ret = SetSymmJointPos();
		_ASSERT( !ret );
		break;

	case 6100:
		LoadGroundData( 0 );
		break;

	case 5050:
		if( g_motdlg && m_mhandler && m_ExistData ){

			if( m_tbb_iti ){
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}

			//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
			int ret;
			ret = g_motdlg->ImportMotion( m_loadnamedlg );
			::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );


			ret = ChangeZaKindFromMotion( g_motdlg->GetMotCookie() );
			_ASSERT( !ret );

			m_setcoldlg->InitUndoBuf();
			m_paintdlg->InitUndoBuf();

		}else{
		
			::MessageBox( m_hWnd, "形状データが読み込まれていないので、\nquaを入力できません。", "入力できません", MB_OK );
			return 0;
		}

		break;
	case 5060:
		if( g_motdlg && m_mhandler ){
			ret = g_motdlg->MultMotion2_0();
			_ASSERT( !ret );
		}
		break;
	case 5061:
		if( g_motdlg && m_mhandler ){
			ret = g_motdlg->MultMotion0_5();
			_ASSERT( !ret );
		}
		break;
	case 5062:
		if( g_motdlg && m_mhandler ){
			ret = g_motdlg->DeleteInitialKey();
			_ASSERT( !ret );
		}
		break;
	case 5063:
		if( g_motdlg && m_mhandler ){
			ret = g_motdlg->DeleteSameKey();
			_ASSERT( !ret );
		}
		break;
	case 5090:
		ret = LoadFile();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの読み込みに失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		break;
	case 5091:
		ret = LoadMotionMulti();
		if( ret ){
			::MessageBox( m_hWnd, "qua読み込みに失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5092:
		if( g_motdlg ){
			ret = LoadMKM();
			_ASSERT( !ret );
			return ret;
		}
		break;
	case 5093:
		ret = ImportFile();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルのインポートに失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5094:
		ret = SaveFile();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの保存に失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5095:
		ret = WriteMAFile( 0, 0 );
		_ASSERT( !ret );
		return 0;
		break;
	case 5096:
		if( g_motdlg ){

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
	case 5097: // AVI出力
		if( g_motdlg && (m_preview_flag == 0) ){
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
	case 5098: //qua出力
		ret = SaveQuaFile();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの保存に失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5099:
		ret = SaveBldFile( 0 );
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの保存に失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5031:
		ret = SaveBldFile( 1 );
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの保存に失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5100://cam load
		ret = LoadCameraMulti();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの読み込みに失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5101://cam write
		ret = SaveCameraFile();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの保存に失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5102:
		ret = LoadGPMulti();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの読み込みに失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5103:
		ret = SaveGPFile();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの保存に失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5104:
		ret = LoadMOEMulti();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの読み込みに失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
		break;
	case 5105:
		ret = SaveMOEFile();
		if( ret ){
			::MessageBox( m_hWnd, "ファイルの保存に失敗しました。", "エラー", MB_OK );
			_ASSERT( 0 );
		}
		return 0;
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
			int excludeflag;
			int onlymfolder;
			if( (int)lParam == 1 ){
				excludeflag = 0;
				onlymfolder = 0;
			}else if( (int)lParam == 2 ){
				excludeflag = 1;
				onlymfolder = 1;
			}else{
				excludeflag = 1;//メニューから呼ばれた場合も、lParamは０
				onlymfolder = 0;
			}

			ret = AutoSaveSigFile();
			_ASSERT( !ret );

			if( onlymfolder == 0 ){
				ret = m_shandler->SetMikoAnchorApply( m_thandler );
				if( ret ){
					DbgOut( "viewer : MENU : CalcInfElem : SetMikoAnchorApply error !!!\n" );
					return ret;
				}
			}
			ret = m_shandler->CalcInfElem( -1, 1, excludeflag, onlymfolder );

			//ret = m_shandler->UpdateVertexBuffer( 0 );

			return ret;
		}
		break;
	case 6010:
		ret = SetMikoBlendType();
		return ret;
		break;

	case 6020:
		ret = AddMorphFolder( 0, 0 );
		_ASSERT( !ret );
		return 0;
		break;
	case 6021:
		ret = CheckDestroyedNum();
		_ASSERT( !ret );
		return 0;
		break;
	case 6022:
		ret = CreateSymBone();
		_ASSERT( !ret );
		return 0;
		break;

	case IDMENU_CAMERA:
		/***
		if( g_inidata->camdlg_disp ){
			g_inidata->camdlg_disp = 0;
			m_cameradlg->ShowWindow( SW_HIDE );
			::CheckMenuItem( m_appmenu, IDMENU_CAMERA, MF_UNCHECKED );

			if( lParam == 0 ){
				RECT newrect;
				m_cameradlg->GetWindowRect( &newrect );
				g_inidata->camdlg_posx = newrect.left;
				g_inidata->camdlg_posy = newrect.top;
			}

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
		***/
		break;
	case IDMENU_TREEEDIT:
		if( g_inidata->sigdlg_disp ){
			g_inidata->sigdlg_disp = 0;
			m_sigeditwnd->ShowWindow( SW_HIDE );
			::CheckMenuItem( m_appmenu, IDMENU_TREEEDIT, MF_UNCHECKED );

			if( lParam == 0 ){
				RECT newrect;
				m_sigeditwnd->GetWindowRect( &newrect );
				g_inidata->sigdlg_posx = newrect.left;
				g_inidata->sigdlg_posy = newrect.top;
			}

		}else{
			m_sigeditwnd->ShowWindow( SW_SHOWNORMAL );
			::CheckMenuItem( m_appmenu, IDMENU_TREEEDIT, MF_CHECKED );

			RECT oldrect;
			m_sigeditwnd->GetWindowRect( &oldrect );
			int winwidth, winheight;
			winwidth = oldrect.right - oldrect.left;
			winheight = oldrect.bottom - oldrect.top;
			//m_sigeditwnd->SetWindowPos( HWND_TOP, g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, winwidth, winheight, SWP_NOSIZE | SWP_NOZORDER );
			//m_sigeditwnd->MoveWindow( g_inidata->sigdlg_posx, g_inidata->sigdlg_posy, winwidth, winheight, 1 );

			g_inidata->sigdlg_disp = 1;
		}
		if( m_pd3dDevice && m_bReady ){
			HRESULT hr;
			m_bReady = FALSE;
			ComputeRect( 1, 0 );
			hr = Resize3DEnvironment();
			_ASSERT( hr == D3D_OK );
			m_bReady = TRUE;
		}
		break;
	case IDMENU_MOTEDIT:
		if( g_inidata->motdlg_disp ){
			//g_motdlgptr->HideDlg();
			//g_motdlg->ShowWindow( SW_HIDE );
			//g_motdlg->SetDisplayFlag( 0 );
			g_inidata->motdlg_disp = 0;
			g_motdlg->ShowDlg( SW_HIDE );
			::CheckMenuItem( m_appmenu, IDMENU_MOTEDIT, MF_UNCHECKED );

			if( lParam == 0 ){
				RECT newrect;
				g_motdlg->GetWindowRect( &newrect );
				g_inidata->motdlg_posx = newrect.left;
				g_inidata->motdlg_posy = newrect.top;
			}

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
				//g_motdlg->SetWindowPos( HWND_TOP, g_inidata->motdlg_posx, g_inidata->motdlg_posy, winwidth, winheight, SWP_NOSIZE | SWP_NOZORDER );


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
		if( m_pd3dDevice && m_bReady ){
			HRESULT hr;
			m_bReady = FALSE;
			ComputeRect( 1, 0 );
			hr = Resize3DEnvironment();
			_ASSERT( hr == D3D_OK );
			m_bReady = TRUE;
		}
		break;
	case 7001://graphwnd
		if( g_inidata->graph_disp ){
			g_inidata->graph_disp = 0;
			m_graphwnd->ShowDlg( SW_HIDE );
			::CheckMenuItem( m_appmenu, 7001, MF_UNCHECKED );
		}else{
			m_graphwnd->ShowDlg( SW_SHOW );
			::CheckMenuItem( m_appmenu, 7001, MF_CHECKED );

			g_inidata->graph_disp = 1;
		}
		if( m_pd3dDevice && m_bReady ){
			HRESULT hr;
			m_bReady = FALSE;
			ComputeRect( 1, 0 );
			hr = Resize3DEnvironment();
			_ASSERT( hr == D3D_OK );
			m_bReady = TRUE;
		}
		break;
	case 7002://syncdlg
		if( m_syncdisp ){
			m_syncdisp = 0;
			m_syncdlg->ShowDlg( SW_HIDE );
			::CheckMenuItem( m_appmenu, 7002, MF_UNCHECKED );
		}else{
			m_syncdlg->ShowDlg( SW_SHOW );
			::CheckMenuItem( m_appmenu, 7002, MF_CHECKED );
			m_syncdisp = 1;
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
			g_inidata->opt_namequa = m_optdlg->m_quatype;

			g_inidata->opt_jointsize = m_optdlg->m_jointsize;
			g_inidata->opt_jointalpha = m_optdlg->m_jointalpha;
			g_inidata->opt_undodlg = m_optdlg->m_undodlg;
			g_inidata->opt_rdlg = m_optdlg->m_rdlg;
			g_inidata->opt_infdlg = m_optdlg->m_infdlg;
			g_inidata->opt_modlg = m_optdlg->m_modlg;
			g_inidata->opt_bmdisp = m_optdlg->m_bmdisp;
			g_motdlg->SetSaveBMDisp( m_optdlg->m_bmdisp );

			//g_inidata->opt_previewfps = m_optdlg->m_previewfps;
			g_inidata->opt_previewfps = m_syncdlg->m_fps;
			g_inidata->opt_motstep = m_optdlg->m_motstep;
			g_inidata->opt_ikfk = m_optdlg->m_ikfk;
			g_inidata->opt_iktdisp = m_optdlg->m_iktdisp;

			g_inidata->opt_MouseL = m_optdlg->m_MouseL;
			g_inidata->opt_MouseR = m_optdlg->m_MouseR;
			g_inidata->opt_MouseW = m_optdlg->m_MouseW;

			g_inidata->usekinect = m_optdlg->m_usekinect;

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



//	case 51000://パースペクティブ
//		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
//		ret = (int)m_parsdlg->DoModal( m_hWnd );
//		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
//
//		break;
	case 51001://面マテリアル

		m_materialdlg->ShowDlg( SW_SHOW );

		break;
	case 51002://法線マップ
		ret = ConvNormalMap();
		_ASSERT( !ret );
		break;
	//case IDTBB_R:
	case IDM_BAKEMOTION:
		if( g_motdlg ){
			g_motdlg->BakeMotion();
		}
		break;
	case IDM_HCOPY:
		if( g_motdlg ){
			g_motdlg->HCopy();
		}
		break;
	case IDM_HPASTE:
		if( g_motdlg ){
			g_motdlg->HPaste();
		}
		break;
	case IDTBB_R:
	case IDM_ALT_R:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}


		if( m_ExistData ){
			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
				if( g_inidata->opt_infdlg == CHKDLG_NES ){
					ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
					ret = IDOK;
				}else{
					ret = IDCANCEL;
				}
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}


			}else if (m_shandler && m_tbb_ei2 ){
				if( g_inidata->opt_infdlg == CHKDLG_NES ){
					ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
					ret = IDOK;
				}else{
					ret = IDCANCEL;
				}
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
			m_tbb_4 = 0;
		}
		CheckTBB();


		break;
	case IDTBB_T:
	case IDM_ALT_T:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
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
		if (m_tbb_ei2 ){
			m_infscopedlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_ei3 ){
			m_paintdlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_toon1 ){
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}

		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();


		break;
	case IDTBB_T2:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
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
		if (m_tbb_ei2 ){
			m_infscopedlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_ei3 ){
			m_paintdlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_toon1 ){
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();
		break;

	case IDTBB_S:
	case IDM_ALT_S:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
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
		if (m_tbb_ei2 ){
			m_infscopedlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_ei3 ){
			m_paintdlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_toon1 ){
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();

		break;
	case IDTBB_ITI:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}

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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && m_tbb_bone ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();


////////////////
		if( m_tbb_iti == 1 ){

			if( m_ExistData ){

				m_itimodedlg->DoModal();

				if( m_itimodedlg->m_itimode == 0 ){
					//motionの削除
					if( g_inidata->opt_modlg == CHKDLG_NES ){
						ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
					}else if( g_inidata->opt_modlg == CHKDLG_YES ){
						ret = IDOK;
					}else{
						ret = IDCANCEL;
					}
					if( g_loadingflag ){
						ret = IDOK;
					}

					if( ret != IDOK ){
						
						if( g_inidata->opt_rdlg == CHKDLG_NES ){
							::MessageBox( m_hWnd, "キャンセルされたので、編集モードを、回転(R)に変更します。", "キャンセル", MB_OK );
						}
						SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
					}else{
						ret = g_motdlg->SelectMotParamTab();
						_ASSERT( !ret );

						ret = DestroyMotion( 1 );
						if( ret ){
							DbgOut( "viewer : ITDTBB_ITI : DestroyMotion error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}

						ret = g_motdlg->RemakeMOEAH();
						if( ret ){
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
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}

		break;

	case IDTBB_IKT:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}

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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
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
				::MessageBox( m_hWnd, "有効にしているＩＫターゲットが、１つもありません。\n現在、このモードで編集できるオブジェクトはありません。", "有効データ無し", MB_OK );
			}
		}


		g_inidata->opt_iktdisp = 1;//!!!!!!!!!!!!!
		m_optdlg->m_iktdisp = 1;


		ValidSelectCircle( 0 );

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();
		break;

	case IDTBB_COL:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}
		
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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}

		if( m_shandler && m_shandler->m_shader == COL_TOON1 ){
			::MessageBox( m_hWnd, "トゥーン色では、この編集は行えません。「色P」に切り替えます。", "色の切り替え", MB_OK );
			m_tbb_colP = 0;
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );

		}

		if( m_tbb_sha ){
			//影をオフにする。
			m_tbb_sha = 1;
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_SHA, 0 );
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
			m_tbb_4 = 0;
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
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}
		break;

	case IDTBB_BONE:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}

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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && ( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}

		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();


////////////////
		if( m_tbb_bone == 1 ){

			if( m_ExistData ){

				//motionの削除
				if( g_inidata->opt_modlg == CHKDLG_NES ){
					ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
				}else if( g_inidata->opt_modlg == CHKDLG_YES ){
					ret = IDOK;
				}else{
					ret = IDCANCEL;
				}
				if( g_loadingflag ){
					ret = IDOK;
				}

				if( ret != IDOK ){
					if( g_inidata->opt_rdlg == CHKDLG_NES ){
						::MessageBox( m_hWnd, "キャンセルされたので、編集モードを、回転(R)に変更します。", "キャンセル", MB_OK );
					}						
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}else{
					ret = g_motdlg->SelectMotParamTab();
					_ASSERT( !ret );


					ret = DestroyMotion( 1 );
					if( ret ){
						DbgOut( "viewer : ITDTBB_BONE : DestroyMotion error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					ret = g_motdlg->RemakeMOEAH();
					if( ret ){
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
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}

		break;

	case IDTBB_EI1:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
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
		if (m_tbb_ei2 ){
			m_infscopedlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_ei3 ){
			m_paintdlg->ShowDlg( SW_HIDE );
			m_selectedno = 0;
		}
		if( m_tbb_toon1 ){
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();

		if( m_tbb_ei1 == 1 ){
			if( m_ExistData ){

				CInfluenceDlg dlg( m_thandler, m_shandler, m_hWnd );

				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
				dlg.DoModal();
				::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

			}else{
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}


		break;

	case IDTBB_EI2:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}

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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();

		if( m_tbb_ei2 == 1 ){
			if( m_ExistData ){

				if( g_inidata->opt_modlg == CHKDLG_NES ){
					ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
				}else if( g_inidata->opt_modlg == CHKDLG_YES ){
					ret = IDOK;
				}else{
					ret = IDCANCEL;
				}
				if( ret != IDOK ){
						
					if( g_inidata->opt_rdlg == CHKDLG_NES ){
						::MessageBox( m_hWnd, "キャンセルされたので、編集モードを、回転(R)に変更します。", "キャンセル", MB_OK );
					}						
					SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
				}else{
					ret = g_motdlg->SelectMotParamTab();
					_ASSERT( !ret );

					ret = DestroyMotion( 1 );
					if( ret ){
						DbgOut( "viewer : ITDTBB_EI2 : DestroyMotion error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}

					ret = g_motdlg->RemakeMOEAH();
					if( ret ){
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
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}

		break;

	case IDTBB_EI3:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}
		
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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}

		if( m_shandler && m_shandler->m_shader == COL_TOON1 ){
			::MessageBox( m_hWnd, "トゥーン色では、この編集は行えません。「色P」に切り替えます。", "色の切り替え", MB_OK );
			m_tbb_colP = 0;
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );

		}

		if( m_tbb_sha ){
			//影をオフにする。
			m_tbb_sha = 1;
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_SHA, 0 );
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
			m_tbb_4 = 0;
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
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}

		break;
	case IDTBB_TOON1:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
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
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();

		if( m_tbb_toon1 == 1 ){
			if( m_ExistData ){

				m_Toon0Dlg->ShowDlg( SW_SHOW );

			}else{
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}

		break;

	case IDTBB_MO:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}

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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_4 ){
			m_za4dlg->ShowDlg( SW_HIDE );
		}

		g_motdlg->SelectMotParamTab();

		if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
			if( ret == IDOK ){
				SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
			}
		}else if (m_shandler && m_tbb_ei2 ){
			if( g_inidata->opt_infdlg == CHKDLG_NES ){
				ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
			}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
				ret = IDOK;
			}else{
				ret = IDCANCEL;
			}
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
			m_tbb_4 = 0;
		}
		CheckTBB();

		if( m_tbb_mo == 1 ){
			if( m_ExistData ){
	

				m_mcdlg->ShowDlg( SW_SHOW );

			}else{
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
				SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
			}
		}
		break;
	case IDTBB_4:
		if( m_graphwnd ){
			m_graphwnd->CloseSpDlg();
		}

		ValidSelectCircle( 2 );
		
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
			m_Toon0Dlg->ShowDlg( SW_HIDE );
		}
		if( m_tbb_mo ){
			m_mcdlg->ShowDlg( SW_HIDE );
		}

		if( g_motdlg->m_disptabno == TAB_GP ){
			g_motdlg->SelectMotParamTab();
		}

		if( m_ExistData ){
			if( m_shandler && (( m_tbb_iti && (m_itimodedlg->m_itimode == 0) ) || m_tbb_bone ) ){
				if( g_inidata->opt_infdlg == CHKDLG_NES ){
					ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
					ret = IDOK;
				}else{
					ret = IDCANCEL;
				}
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}

			}else if (m_shandler && m_tbb_ei2 ){
				if( g_inidata->opt_infdlg == CHKDLG_NES ){
					ret = (int)::MessageBox( m_hWnd, "ボーンの影響範囲が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
				}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
					ret = IDOK;
				}else{
					ret = IDCANCEL;
				}
				if( ret == IDOK ){
					SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
				}
			}
		}

		if( m_tbb_4 ){
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
			m_tbb_mo = 0;
			m_tbb_4 = 1;
		}
		CheckTBB();

		if( m_tbb_4 ){
			if( m_ExistData ){
				ret = m_za4dlg->ShowDlg( SW_SHOW );
				_ASSERT( !ret );
			}else{
				if( g_inidata->opt_rdlg == CHKDLG_NES ){
					::MessageBox( m_hWnd, "形状データを読み込んでいないので、\nこのモードは無効です。\n編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
				}
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
			m_tbb_za4 = 0;
		}
		m_za_kind = ZA_1;
		CheckTBB();

		if( m_ExistData ){
			ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );
			_ASSERT( !ret );

			ret = m_mhandler->FillUpMotion( m_shandler, g_motdlg->GetMotCookie(), -1, 0, -1, 0 );
			if( ret ){
				_ASSERT( 0 );
			}
		}

		break;

	case IDTBB_ZA2:
		if( m_tbb_za2 ){
			//none
		}else{
			m_tbb_za1 = 0;
			m_tbb_za2 = 1;
			m_tbb_za3 = 0;
			m_tbb_za4 = 0;
		}
		m_za_kind = ZA_2;
		CheckTBB();

		if( m_ExistData ){
			ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );
			_ASSERT( !ret );

			ret = m_mhandler->FillUpMotion( m_shandler, g_motdlg->GetMotCookie(), -1, 0, -1, 0 );
			if( ret ){
				_ASSERT( 0 );
			}
		}


		break;

	case IDTBB_ZA3:
		if( m_tbb_za3 ){
			//none
		}else{
			m_tbb_za1 = 0;
			m_tbb_za2 = 0;
			m_tbb_za3 = 1;
			m_tbb_za4 = 0;
		}
		m_za_kind = ZA_3;
		CheckTBB();

		if( m_ExistData ){
			ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );
			_ASSERT( !ret );

			ret = m_mhandler->FillUpMotion( m_shandler, g_motdlg->GetMotCookie(), -1, 0, -1, 0 );
			if( ret ){
				_ASSERT( 0 );
			}
		}

		break;

	case IDTBB_ZA4:
		if( m_tbb_za4 ){
			//none
		}else{
			m_tbb_za1 = 0;
			m_tbb_za2 = 0;
			m_tbb_za3 = 0;
			m_tbb_za4 = 1;
		}
		m_za_kind = ZA_4;
		CheckTBB();

		if( m_ExistData ){
			ret = m_mhandler->CalcMPEuler( m_shandler, g_motdlg->GetMotCookie(), m_za_kind );
			_ASSERT( !ret );

			ret = m_mhandler->FillUpMotion( m_shandler, g_motdlg->GetMotCookie(), -1, 0, -1, 0 );
			if( ret ){
				_ASSERT( 0 );
			}
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
			m_tbb_colPP = 0;
		}
		CheckTBB();
		break;
	case IDTBB_COLPP:
		if( m_tbb_colPP ){
			//none
		}else{
			if( m_shandler ){
				m_shandler->m_shader = COL_PPHONG;

				ret = m_shandler->SetIndexBufColP();
				if( ret ){
					DbgOut( "viewer : ColorChange : IDTBB_COLPP : COL_PPHONG : sh SetIndexBufColP error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}

			m_tbb_colP = 0;
			m_tbb_colT = 0;
			m_tbb_colPP = 1;
		}
		CheckTBB();
		break;

	case IDTBB_COLT:
		if( m_tbb_colT ){
			//none
		}else{
			if( m_shandler ){
				m_shandler->m_shader = COL_TOON1;

				ret = m_shandler->CreateToon1Buffer( m_pd3dDevice, 1 );
				if( ret ){
					DbgOut( "viewer : ColorChange : IDTBB_COLT : COL_TOON1 : sh CreateToon1Buffer error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}

			m_tbb_colP = 0;
			m_tbb_colT = 1;
			m_tbb_colPP = 0;
		}
		CheckTBB();
		break;

	case IDTBB_SHA:
		if( m_tbb_sha ){
			m_tbb_sha = 0;
			m_proj_near = 100.0f;
		}else{
			if( (g_shadowUse != 0) && (m_tbb_col == 0) && (m_tbb_ei3 == 0) ){
				m_tbb_sha = 1;
				m_proj_near = 1000.0f;
			}
			if( g_shadowUse == 0 ){
				::MessageBox( m_hWnd, "ハードウェアが要件を満たさないので影は表示できません。", "表示できません", MB_OK );
			}
		}

		if( m_tbb_sha == 1 ){
			if( (m_createshadowflag == 0) || !m_shadowelem.pTex || !m_shadowelem.pBackBuffer || !m_shadowelem.pZSurf ){
				RestoreShadowElem();
				
				if( !m_shadowelem.pTex || !m_shadowelem.pBackBuffer || !m_shadowelem.pZSurf ){
					::MessageBox( m_hWnd, "メモリ不足のため影は表示できません。", "メモリ不足", MB_OK );
					m_tbb_sha = 0;//!!!!!!!!!!!
					m_proj_near = 100.0f;
				}
			}
		}
		CheckTBB();
		break;

	case IDTBB_GLOW:
		if( m_tbb_glow ){
			m_tbb_glow = 0;
			m_glowdlg->ShowDlg( SW_HIDE );
		}else{
			m_tbb_glow = 1;
			m_glowdlg->ShowDlg( SW_SHOW );
		}

		CheckTBB();
		break;

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
		m_sigeditwnd->OnMenuShdParam();
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
		if( m_tbb_mo == 0 ){
			SelChangeBone( menuid );
		}
		break;
	case IDM_SHIFT_UP:
	case IDM_SHIFT_DOWN:
	case IDM_SHIFT_LEFT:
	case IDM_SHIFT_RIGHT:
	case IDM_TARGET_UP:
	case IDM_TARGET_DOWN:
	case IDM_TARGET_LEFT:
	case IDM_TARGET_RIGHT:
		//CameraByKey( menuid );
		break;

	case IDM_CTRLO:
		TogglePars();
		break;
	case IDM_CTRLN:
		ToggleNotUse();
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
	case IDM_CTRLSHIFTT:
		IKTallON();
		break;
	case IDM_IKTON:
		IKTON();
		break;
	case IDM_IKTTOGGLE:
		IKTToggle();
		break;

	case IDM_CTRLB:
		if( *(m_loadnamedlg->m_im2name) ){
			LoadIM2File( 1, 0 );
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
	case IDM_CTRL_YAUP:
		motnum = g_motdlg->GetMotionNum();
		if( !m_shandler || (motnum <= 0) ){
			return 0;
		}
		curindex = g_motdlg->GetMotCookie();
		selindex = curindex - 1;
		if( selindex < 0 ){
			selindex = motnum - 1;
		}
		ret = g_motdlg->SelectCurrentMotion( selindex );
		if( ret ){
			DbgOut( "viewer : IDM_CTRL_YAUP : SelectCurrentMotion error !!!\n" );
			_ASSERT( 0 );
		}
		m_setcoldlg->InitUndoBuf();
		m_paintdlg->InitUndoBuf();


		break;
	case IDM_CTRL_YADOWN:

		motnum = g_motdlg->GetMotionNum();
		if( !m_shandler || (motnum <= 0) ){
			return 0;
		}
		curindex = g_motdlg->GetMotCookie();
		selindex = curindex + 1;
		if( selindex >= motnum ){
			selindex = 0;
		}
		ret = g_motdlg->SelectCurrentMotion( selindex );
		if( ret ){
			DbgOut( "viewer : IDM_CTRL_YADOWN : SelectCurrentMotion error !!!\n" );
			_ASSERT( 0 );
		}
		m_setcoldlg->InitUndoBuf();
		m_paintdlg->InitUndoBuf();

		break;

	case IDM_PAUSE:
		if( m_tbb_mo == 0 ){
			if( m_preview_flag == 1 ){
				ret = g_motdlg->OnStop();
				_ASSERT( !ret );
			}else{
				ret = g_motdlg->OnPreview();
				_ASSERT( !ret );
			}
		}
		break;
		
	case IDM_CTRLI:
		if( m_sigeditwnd ){
			m_sigeditwnd->OnMenuShdParam();
		}
		break;
/////////////
	default:
		break;
	}
	return DefWindowProc();
}

int CMyD3DApplication::ComputeRect( int resize3d, RECT* treerect )
{
	RECT winrect;
	::GetWindowRect( m_hWnd, &winrect );

	RECT rect0;
	::GetClientRect( m_hWnd, &rect0 );

	RECT toolrect;
	::GetClientRect( g_hToolBar, &toolrect );

	int motheight;
	if( g_inidata->motdlg_disp ){
		//motheight = 190;
		motheight = g_inidata->motdlg_height;
	}else{
		motheight = 0;
	}

	int graphwidth;
	if( g_inidata->graph_disp ){
		graphwidth = 250;
	}else{
		graphwidth = 0;
	}

	int diffh = 0;

	if( g_inidata->sigdlg_disp ){
		if( treerect == 0 ){
			m_rcsigedit.left = 0;
			m_rcsigedit.top = toolrect.bottom;
			m_rcsigedit.right = 250;
			//m_rcsigedit.bottom = rect0.bottom - motheight;
			m_rcsigedit.bottom = rect0.bottom - 25;
		}else{
			m_rcsigedit = *treerect;
			m_rcsigedit.top += toolrect.bottom;
			m_rcsigedit.bottom += toolrect.bottom;

			if( m_rcsigedit.right > (rect0.right - rect0.left - 150) ){
				m_rcsigedit.right = rect0.right - rect0.left - 150;
			}
			//diffh = (m_rcsigedit.bottom + motheight) - (rect0.bottom - rect0.top);
			diffh = (m_rcsigedit.bottom + 25) - (rect0.bottom - rect0.top);
		}

		m_rc3dwnd.left = m_rcsigedit.right;
		m_rc3dwnd.top = toolrect.bottom;
		//m_rc3dwnd.right = rect0.right;
		m_rc3dwnd.right = rect0.right - graphwidth;
		m_rc3dwnd.bottom = m_rcsigedit.bottom - motheight;

		m_rcmot.left = m_rcsigedit.right;
		m_rcmot.top = m_rc3dwnd.bottom;
		//m_rcmot.right = rect0.right;
		m_rcmot.right = rect0.right;
		m_rcmot.bottom = m_rc3dwnd.bottom + motheight;

		m_rcgraph.top = toolrect.bottom;
		m_rcgraph.left = rect0.right - graphwidth;
		m_rcgraph.right = rect0.right;
		m_rcgraph.bottom = m_rc3dwnd.bottom;

	}else{
		m_rcsigedit.left = 0;
		m_rcsigedit.top = toolrect.bottom;
		m_rcsigedit.right = 0;
		m_rcsigedit.bottom = rect0.bottom - 25;

		m_rc3dwnd.left = 0;
		m_rc3dwnd.top = toolrect.bottom;
		//m_rc3dwnd.right = rect0.right;
		m_rc3dwnd.right = rect0.right - graphwidth;
		m_rc3dwnd.bottom = rect0.bottom - motheight - 25;

		m_rcmot.left = 0;
		m_rcmot.top = m_rc3dwnd.bottom;
		//m_rcmot.right = rect0.right;
		m_rcmot.right = rect0.right;
		m_rcmot.bottom = m_rc3dwnd.bottom + motheight;

		m_rcgraph.top = toolrect.bottom;
		m_rcgraph.left = rect0.right - graphwidth;
		m_rcgraph.right = rect0.right;
		m_rcgraph.bottom = m_rc3dwnd.bottom;


	}

	DbgOut( "viewer : ComputeRect : 3dwnd : w %d, h %d\r\n",
		m_rc3dwnd.right - m_rc3dwnd.left,
		m_rc3dwnd.bottom - m_rc3dwnd.top );

	int diffw = 0;
	int extw = 0;

//	diffw = (m_rcsigedit.right + graphwidth) - ( rect0.right - rect0.left );
//	if( diffw > 150 ){
//		extw = 0;
//	}else{
//		extw = 150 - diffw;
//	}

	m_d3dpp.BackBufferWidth  = m_rc3dwnd.right - m_rc3dwnd.left;
	m_d3dpp.BackBufferHeight = m_rc3dwnd.bottom - m_rc3dwnd.top;

	if( resize3d ){

		if( treerect && ::IsWindow( m_hWnd ) ){
//			::MoveWindow( m_hWnd, winrect.left, winrect.top,
//				winrect.right,
//				winrect.bottom + diffh, 1 );

			WINDOWPLACEMENT place;
			place.length = sizeof( WINDOWPLACEMENT );
			::GetWindowPlacement( m_hWnd, &place );

			place.rcNormalPosition.bottom += diffh;
			place.rcNormalPosition.right += extw;
			::SetWindowPlacement( m_hWnd, &place );

			::GetWindowRect( m_hWnd, &m_rcWindowBounds );
			::GetClientRect( m_hWnd, &m_rcWindowClient );
		}

		::MoveWindow( m_3dwnd, m_rc3dwnd.left, m_rc3dwnd.top,
			m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1 );

//		WPARAM wp = SIZE_RESTORED;
//		LPARAM lp;
//		WORD px, py;
//		px = m_d3dpp.BackBufferWidth;
//		py = m_d3dpp.BackBufferHeight;
//		lp = (py << 16) | px;
//		::SendMessage( g_hStatus, WM_SIZE, wp, lp );


		if( g_inidata->sigdlg_disp ){
			::MoveWindow( m_sigeditwnd->m_hWnd, m_rcsigedit.left, m_rcsigedit.top,
				m_rcsigedit.right - m_rcsigedit.left,
				m_rcsigedit.bottom - m_rcsigedit.top,
				1 );
		}

		if( g_inidata->motdlg_disp ){
			::MoveWindow( g_motdlg->m_hWnd, m_rcmot.left, m_rcmot.top,
				m_rcmot.right - m_rcmot.left,
				m_rcmot.bottom - m_rcmot.top,
				1 );
			BOOL dummy;
			g_motdlg->OnSelchange_tab( 0, 0, dummy ); 
		}

		if( g_inidata->graph_disp ){
			::MoveWindow( m_graphwnd->m_hWnd, m_rcgraph.left, m_rcgraph.top,
				m_rcgraph.right - m_rcgraph.left,
				m_rcgraph.bottom - m_rcgraph.top,
				1 );
			BOOL dummy1;
			m_graphwnd->OnSelchange_tab( 0, 0, dummy1 ); 

			BOOL dummy2;
			m_graphwnd->OnSizeDlg( WM_MOVE, 0, 0, dummy2 );

		}
	}

	int spritesize = 32;
	int spacesize = 5;

	m_camkaku_rect.top = spacesize;
	m_camkaku_rect.bottom = spacesize + spritesize;
	m_camkaku_rect.right = m_rc3dwnd.right - m_rc3dwnd.left - 2 * spacesize;
	m_camkaku_rect.left = m_camkaku_rect.right - spritesize;

	m_cami_rect.top = spacesize;
	m_cami_rect.bottom = spacesize + spritesize;
	m_cami_rect.right = m_camkaku_rect.left - spacesize;
	m_cami_rect.left = m_cami_rect.right - spritesize;

	m_camkai_rect.top = spacesize;
	m_camkai_rect.bottom = spacesize + spritesize;
	m_camkai_rect.right = m_cami_rect.left - spacesize;
	m_camkai_rect.left = m_camkai_rect.right - spritesize;

	DbgOut( "viewer : ComputeRect : rc3dwnd left %d, right %d, kaku top %d, left %d\r\n",
		m_rc3dwnd.left, m_rc3dwnd.right, m_camkaku_rect.top, m_camkaku_rect.left );

	return 0;
}


LRESULT CMyD3DApplication::OnMainLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD kind = (WORD)wParam;

	WORD xPos, yPos;
	xPos = GET_X_LPARAM(lParam); 
	yPos = GET_Y_LPARAM(lParam); 

	if( (xPos >= m_rc3dwnd.left) && (xPos <= m_rc3dwnd.right) && (yPos >= m_rc3dwnd.top) && (yPos <= m_rc3dwnd.bottom) ){
		::SetCapture( m_3dwnd.m_hWnd );
	
		LButtonDownFunc( kind, xPos - (WORD)m_rc3dwnd.left, yPos - (WORD)m_rc3dwnd.top );
	}


	return 0;
}
LRESULT CMyD3DApplication::OnMainRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD kind = (WORD)wParam;

	WORD xPos, yPos;
	xPos = GET_X_LPARAM(lParam); 
	yPos = GET_Y_LPARAM(lParam); 

	if( (xPos >= m_rc3dwnd.left) && (xPos <= m_rc3dwnd.right) && (yPos >= m_rc3dwnd.top) && (yPos <= m_rc3dwnd.bottom) ){
		::SetCapture( m_3dwnd.m_hWnd );
	
		RButtonDownFunc( kind, xPos - (WORD)m_rc3dwnd.left, yPos - (WORD)m_rc3dwnd.top );
	}



	return 0;
}
LRESULT CMyD3DApplication::OnMainMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD kind = (WORD)wParam;

	WORD xPos, yPos;
	xPos = GET_X_LPARAM(lParam); 
	yPos = GET_Y_LPARAM(lParam); 

	if( (xPos >= m_rc3dwnd.left) && (xPos <= m_rc3dwnd.right) && (yPos >= m_rc3dwnd.top) && (yPos <= m_rc3dwnd.bottom) ){
		::SetCapture( m_3dwnd.m_hWnd );

		MButtonDownFunc( kind, xPos - (WORD)m_rc3dwnd.left, yPos - (WORD)m_rc3dwnd.top );

	}

	return 0;
}

int CMyD3DApplication::SetShadowParams()
{
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	
	int ret;
	CShadowParamDlg dlg( m_shadowcoef[0], m_shadowcoef[1] );
	ret = (int)dlg.DoModal();
	if( ret == IDOK ){
		m_shadowcoef[0] = dlg.m_bias;
		m_shadowcoef[1] = dlg.m_darkrate;
	}
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	return 0;
}

int CMyD3DApplication::TogglePars()
{
	if( g_motdlg->m_camdlg ){
		g_motdlg->m_camdlg->ToggleOrtho();
	}

	return 0;
}
int CMyD3DApplication::ToggleNotUse()
{
	if( !m_shandler || !m_sigeditwnd ){
		return 0;
	}

	int selno = 0;

	m_sigeditwnd->GetSelectedNo( &selno );
	if( selno > 0 ){
		CShdElem* selem;
		selem = (*m_shandler)( selno );
		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){
			if( selem->notuse == 0 ){
				selem->notuse = 1;
			}else{
				selem->notuse = 0;
			}
		}
	}


	return 0;
}

int CMyD3DApplication::AutoSaveSigFile()
{
	int ret;

	// Auto Save
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	char autosavename[_MAX_PATH];
	sprintf_s( autosavename, _MAX_PATH, "%ssave_%d_%d_%d_%d_%d_%d_%d.sig",
		m_autosavedir,
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
	);

	CSigFile sigfile;
	ret = sigfile.WriteSigFile( autosavename, m_thandler, m_shandler, m_mhandler );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	
	return 0;
}

/***
int CMyD3DApplication::CameraByKey( WORD menuid )
{

	int keyflag;
	if( menuid == IDM_SHIFT_UP ){
		keyflag = ID_KEY_RAD2_UP;
	}else if( menuid == IDM_SHIFT_DOWN ){
		keyflag = ID_KEY_RAD2_DOWN;
	}else if( menuid == IDM_SHIFT_LEFT ){
		keyflag = ID_KEY_LEFT;
	}else if( menuid == IDM_SHIFT_RIGHT ){
		keyflag = ID_KEY_RIGHT;
	}else if( menuid == IDM_TARGET_UP ){
		keyflag = ID_KEY_TARGET_UP;
	}else if( menuid == IDM_TARGET_DOWN ){
		keyflag = ID_KEY_TARGET_DOWN;
	}else if( menuid == IDM_TARGET_LEFT ){
		keyflag = ID_KEY_TARGET_LEFT;
	}else if( menuid == IDM_TARGET_RIGHT ){
		keyflag = ID_KEY_TARGET_RIGHT;
	}
	ChangeViewParam( keyflag );

	return 0;
}
***/

int CMyD3DApplication::CalcSelectMat()
{
	int ret;

    DWORD dwClipWidth  = m_clientWidth/2;
    DWORD dwClipHeight = m_clientHeight/2;

	D3DXMATRIX view2;
	view2 = m_matView;
	view2._41 = 0.0f;
	view2._42 = 0.0f;
	view2._43 = 0.0f;
	D3DXMatrixInverse( &m_sq_mat, NULL, &view2 );
	m_sq_mat._41 = m_selectedpos.x;
	m_sq_mat._42 = m_selectedpos.y;
	m_sq_mat._43 = m_selectedpos.z;
	
	ret = m_sq_sh->ChkInView( m_sq_mh, &m_sq_mat, &m_matView, &m_matProjX );
	if( ret ){
		DbgOut( "viewer : FrameMove : sq ChkInView error !!!\n" );
		_ASSERT( 0 );
	}

	ret = m_sq_sh->TransformDispData( m_vEyePt, m_sq_mh, &m_sq_mat, &m_matView, &m_matProjX, 1, 0 );
	if( ret ){
		DbgOut( "viewer : FrameMove : sq TransformDispData error !!!\n" );
		_ASSERT( 0 );
	}

	int outflag = 1;
	int scx0, scy0;
	float scz0;
	ret = m_sq_sh->GetScreenPos3( 0, m_sq_mh, 6, 0, 1, &scx0, &scy0, &scz0,
		dwClipWidth, dwClipHeight, m_sq_mat, m_matView, m_matProjX, outflag );
	if( ret ){
		DbgOut( "viewer : FrameMove : sq GetScreenPos3 0 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int scx1, scy1;
	float scz1;
	ret = m_sq_sh->GetScreenPos3( 0, m_sq_mh, 7, 0, 1, &scx1, &scy1, &scz1,
		dwClipWidth, dwClipHeight, m_sq_mat, m_matView, m_matProjX, outflag );
	if( ret ){
		DbgOut( "viewer : FrameMove : sq GetScreenPos3 1 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	float mag, dist, selectscale;

	mag = (float)( (scx0 - scx1) * (scx0 - scx1) + (scy0 - scy1) * (scy0 - scy1) );
	if( mag != 0.0f ){
		dist = sqrtf( mag );
	}else{
		dist = 1.0f;//!!!!!!!
	}
	selectscale = 30.0f / dist;

	if( selectscale >= 8.0f )
		selectscale = 8.0f;

//////////////
	if( ((m_tbb_ikt == 0) && (m_tbb_iti == 0) && (m_tbb_bone == 0) && (m_tbb_ei2 == 0)) 
		|| ((m_tbb_ei3 == 1) && (m_paintdlg->m_ope == PAINTOPE_BONE)) ){

		if( g_motdlg->m_disptabno == TAB_GP ){
			if( m_shandler ){
				m_select_mat = m_shandler->m_gpdata.m_q.MakeRotMatX();
			}else{
				D3DXMatrixIdentity( &m_select_mat );
			}

		}else{
			CShdElem* selelem;
			selelem = (*m_shandler)( m_selectedno );

			CQuaternion selq;
			ret = m_mhandler->GetTotalQOnFrame( m_shandler, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), m_selectedno, &selq );
			if( ret ){
				DbgOut( "viewer : FrameMove : mh GetTotalQOnFrame error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			selq = m_shandler->m_gpdata.m_q * selq;


			if( (m_tbb_4 == 1) || (m_tbb_za4 == 1) ){
				if( m_selectedno > 0 ){
					CShdElem* selelem;
					selelem = (*m_shandler)( m_selectedno );
					int za4type = ZA_1;
					ret = selelem->GetZa4Type( &za4type );
					_ASSERT( !ret );

					CQuaternion za4q;
					ret = selelem->GetZa4Q( &za4q );
					_ASSERT( !ret );

					CQuaternion setq;

					if( za4type == ZA_1 ){
						setq = selq * za4q;
						//setq = za4q * selq;
					}else if( za4type == ZA_2 ){
						setq = za4q;
					}else if( za4type == ZA_3 ){
						CQuaternion iniq;
						ret = m_shandler->GetInitialBoneQ( m_selectedno, &iniq );
						if( ret ){
							DbgOut( "viewer : FrameMove : sh GetInitialBoneQ error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}
						//setq = selq * iniq * za4q;
						setq = za4q * iniq * selq;
					}else{
						_ASSERT( 0 );
						setq.SetParams( 1.0f, 0.0, 0.0f, 0.0f );
					}
					m_select_mat = setq.MakeRotMatX();
				}else{
					D3DXMatrixIdentity( &m_select_mat );
				}
			}else{
				if( m_tbb_za1 == 1 ){
					m_select_mat = selq.MakeRotMatX();
				}else if( m_tbb_za2 == 1 ){
					D3DXMatrixIdentity( &m_select_mat );
				}else if( m_tbb_za3 == 1 ){
					CQuaternion iniq;
					ret = m_shandler->GetInitialBoneQ( m_selectedno, &iniq );
					if( ret ){
						DbgOut( "viewer : FrameMove : sh GetInitialBoneQ error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
					CQuaternion za3selq;
					za3selq = selq * iniq;
					//za3selq = iniq * selq;
					m_select_mat = za3selq.MakeRotMatX();
				}else{
					_ASSERT( 0 );
					D3DXMatrixIdentity( &m_select_mat );
				}
			}
		}
	}else{
		D3DXMatrixIdentity( &m_select_mat );
	}

	D3DXMATRIX scalemat;
	D3DXMatrixIdentity( &scalemat );
	scalemat._11 = selectscale;
	scalemat._22 = selectscale;
	scalemat._33 = selectscale;

	m_select_mat = scalemat * m_select_mat;//!!!!!!!!!!!!!!

	m_select_mat._41 = m_selectedpos.x;
	m_select_mat._42 = m_selectedpos.y;
	m_select_mat._43 = m_selectedpos.z;

	return 0;
}

int CMyD3DApplication::SaveToon1File( char* srcfilename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( !m_ExistData )
		return 0;


	CToon1File toon1file( TO1_SAVE );
	int ret;

	char buf[_MAX_PATH];
	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		strcpy_s( buf, _MAX_PATH, "default.to1" );
		OPENFILENAME ofn;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"Toon1 File (*.to1)\0*.to1\0";
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
		ofn.lpstrDefExt ="to1";
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( !GetSaveFileName(&ofn) ){
			return 0;
		}
	}

	m_tbb_colT = 0;
	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );/// トゥーン表示に切り替え

	ret = toon1file.SaveToon1File( buf, m_thandler, m_shandler );
	if( ret ){
		DbgOut( "viewer : SaveToon1File : toon1file SaveToon1File error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CMyD3DApplication::LoadToon1File( char* srcfilename )
{

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	CToon1File toon1file( TO1_LOAD );
	int ret;

	char buf[_MAX_PATH];
	ZeroMemory( buf, sizeof( char ) * _MAX_PATH );

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		OPENFILENAME ofn;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"Toon1 File (*.to1)\0*.to1\0";
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
		if( !GetOpenFileName(&ofn) ){
			return 0;
		}
	}

	m_tbb_colT = 0;
	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );/// トゥーン表示に切り替え

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );



	ret = toon1file.LoadToon1File( m_hWnd, buf, m_thandler, m_shandler );
	if( ret ){
		DbgOut( "viewer : LoadToon1File : toon1file LoadToon1File error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::SavePoseFile( char* srcfilename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( !m_ExistData )
		return 0;


	int ret;
	int motid = -1;
	int frameno = -1;
	ret = g_motdlg->GetMotParams( &motid, &frameno );
	if( ret || (motid < 0) || (frameno < 0) ){
		return 0;
	}


	char buf[_MAX_PATH];
	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		strcpy_s( buf, _MAX_PATH, "default.pos" );
		OPENFILENAME ofn;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"Pose File (*.pos)\0*.pos\0";
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
		ofn.lpstrDefExt ="pos";
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( !GetSaveFileName(&ofn) ){
			return 0;
		}
	}

	CFrameCopyDlg fcdlg( m_thandler, m_shandler, m_hWnd, g_motdlg->m_motparamdlg->m_sl_mp_rangemax );
	ret = fcdlg.CreateLists();
	if( ret ){
		DbgOut( "viewer : SavePoseFile : fcdlg CreateLists error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	int dlgret;
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	dlgret = (int)fcdlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( dlgret != IDOK ){
		return 0;
	}

	CCreateKeyDlg ckdlg;
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
	dlgret = (int)ckdlg.DoModal( m_hWnd );
	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
	if( dlgret != IDOK ){
		return 0;
	}

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	CPoseFile posefile( 0 );
	ret = posefile.SavePoseFile( m_thandler, m_shandler, m_mhandler, buf, motid, frameno, fcdlg.m_cplist, ckdlg.m_create );
	if( ret ){
		DbgOut( "viewer : SavePoseFile : posefile SavePoseFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CMyD3DApplication::LoadPoseFile( char* srcfilename )
{
	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;

	if( !m_ExistData )
		return 0;

	int ret;
	int motid = -1;
	int frameno = -1;
	ret = g_motdlg->GetMotParams( &motid, &frameno );
	if( ret || (motid < 0) || (frameno < 0) ){
		return 0;
	}

	char buf[_MAX_PATH];
	ZeroMemory( buf, sizeof( char ) * _MAX_PATH );

	if( srcfilename && *srcfilename ){
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}else{
		OPENFILENAME ofn;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"Pose File (*.pos)\0*.pos\0";
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
		if( !GetOpenFileName(&ofn) ){
			return 0;
		}
	}
	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	CPoseFile posefile( 1 );
	ret = posefile.LoadPoseFile( m_thandler, m_shandler, m_mhandler, buf, motid, frameno );
	if( ret ){
		DbgOut( "viewer : LoadPoseFile : posefile LoadPoseFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_mhandler->MakeRawmat( motid, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "viewer : LoadPoseFile : MakeRawmat error" );
		_ASSERT( 0 );
		return 1;
	}
	ret = m_mhandler->RawmatToMotionmat( motid, m_shandler, -1, -1 );
	if( ret ){
		DbgOut( "viewer : LoadPoseFile : RawmatToMotionmat error" );
		_ASSERT( 0 );
		return 1;
	}

	ret = m_mhandler->CalcMPEuler( m_shandler, motid, m_za_kind );//!!!!!!!!!!!!!!!!!
	_ASSERT( !ret );

	ret = g_motdlg->SetCurrentMotion();
	_ASSERT( !ret );

	ret = g_motdlg->Redraw();
	_ASSERT( !ret );

	::SendMessage( m_hWnd, WM_USER_DISPLAY, 0, 0 );
	

	return 0;
}

int CMyD3DApplication::GetDispObjNum( int* numptr )
{

	*numptr = 0;
	
	if( !m_shandler || !m_thandler )
		return 0;


	int findcnt = 0;
	CShdElem* selem;
	int seri;

	for( seri = 1; seri < m_shandler->s2shd_leng; seri++ ){
		selem = (*m_shandler)( seri );
		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){

			findcnt++;
		}

	}

	*numptr = findcnt;

	return 0;
}

int CMyD3DApplication::GetDispObjInfo( DISPOBJINFO* doiarray, int arrayleng, int* getnumptr )
{

	*getnumptr = 0;

	if( !m_shandler || !m_thandler )
		return 0;

	int setcnt = 0;
	CShdElem* selem;
	CTreeElem2* telem;
	int seri;

	int namelen;

	DISPOBJINFO* curdoi;

	for( seri = 1; seri < m_shandler->s2shd_leng; seri++ ){
		selem = (*m_shandler)( seri );
		telem = (*m_thandler)( seri );

		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){

			if( setcnt >= arrayleng ){
				DbgOut( "handlerset: GetDispObjInfo : arrayleng too short error %d !!!\n", arrayleng );
				_ASSERT( 0 );
				return 1;
			}

			curdoi = doiarray + setcnt;

			namelen = (int)strlen( telem->name );
			if( namelen > 256 ){
				DbgOut( "handlerset : GetDispObjInfo : namelen too large warning !!!\n" );
				_ASSERT( 0 );
				strcpy_s( curdoi->name, 256, "エラー。名前が長すぎて格納できません。" );
			}else{
				strcpy_s( curdoi->name, 256, telem->name );
			}

			curdoi->serialno = selem->serialno;
			curdoi->notuse = selem->notuse;
			curdoi->dispswitch = selem->dispswitchno;
			curdoi->invisible = selem->invisibleflag;

			setcnt++;
		}
	}

	*getnumptr = setcnt;

	return 0;


}


LRESULT CMyD3DApplication::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if( g_motdlg ){
		g_motdlg->SelectMotParamTab();
	}

	HDROP hDrop;
	UINT dropnum;
	int i;
	char szFileName[MAX_PATH];
	
	char shapename[MAX_PATH];
	char quaname[MAXMOTIONNUM][MAX_PATH];
	char bonname[MAX_PATH];
	char im2name[MAX_PATH];
	char moaname[MAX_PATH];
	char limname[MAX_PATH];
	char to1name[MAX_PATH];
	char posname[MAX_PATH];

	int shapetype = SHAPEF_SIG;

	int quanum = 0;

	ZeroMemory( szFileName, sizeof( char ) * MAX_PATH );
	ZeroMemory( shapename, sizeof( char ) * MAX_PATH );
	ZeroMemory( quaname, sizeof( char ) * MAX_PATH * MAXMOTIONNUM );
	ZeroMemory( bonname, sizeof( char ) * MAX_PATH );
	ZeroMemory( im2name, sizeof( char ) * MAX_PATH );
	ZeroMemory( moaname, sizeof( char ) * MAX_PATH );
	ZeroMemory( limname, sizeof( char ) * MAX_PATH );
	ZeroMemory( to1name, sizeof( char ) * MAX_PATH );
	ZeroMemory( posname, sizeof( char ) * MAX_PATH );

	hDrop = (HDROP)wParam;

    dropnum = DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0);
//	DbgOut( "viewer : OnDropFiles : dropnum %d\r\n", dropnum );

	for(i = 0; i < (int)dropnum; i++) {
        DragQueryFile(hDrop, i, szFileName, MAX_PATH);
//		DbgOut( "viewer : OnDropFiles : %s\r\n", szFileName );

		int ch = '.';
		char* extptr = 0;
		extptr = strrchr( szFileName, ch );
		if( extptr ){
			int cmpsig, cmprok, cmpmqo, cmpx, cmpqua, cmpbon, cmpim2, cmpmoa, cmplim, cmpto1, cmppos;
			cmpsig = strcmp( extptr, ".sig" );
			cmprok = strcmp( extptr, ".rok" );
			cmpmqo = strcmp( extptr, ".mqo" );
			cmpx = strcmp( extptr, ".x" );
			cmpqua = strcmp( extptr, ".qua" );
			cmpbon = strcmp( extptr, ".bon" );
			cmpim2 = strcmp( extptr, ".im2" );
			cmpmoa = strcmp( extptr, ".moa" );
			cmplim = strcmp( extptr, ".lim" );
			cmpto1 = strcmp( extptr, ".to1" );
			cmppos = strcmp( extptr, ".pos" );

			if( cmpsig == 0 ){
				strcpy_s( shapename, MAX_PATH, szFileName );
				shapetype = SHAPEF_SIG;
			}
			if( cmprok == 0 ){
				strcpy_s( shapename, MAX_PATH, szFileName );
				shapetype = SHAPEF_ROK;
			}
			if( cmpmqo == 0 ){
				strcpy_s( shapename, MAX_PATH, szFileName );
				shapetype = SHAPEF_MQO;
			}
			if( cmpx == 0 ){
				strcpy_s( shapename, MAX_PATH, szFileName );
				shapetype = SHAPEF_X;
			}
			if( cmpqua == 0 ){
				if( quanum < MAXMOTIONNUM ){
					strcpy_s( &(quaname[quanum][0]), MAX_PATH, szFileName );
					quanum++;
				}
			}
			if( cmpbon == 0 ){
				strcpy_s( bonname, MAX_PATH, szFileName );
			}
			if( cmpim2 == 0 ){
				strcpy_s( im2name, MAX_PATH, szFileName );
			}
			if( cmpmoa == 0 ){
				strcpy_s( moaname, MAX_PATH, szFileName );
			}
			if( cmplim == 0 ){
				strcpy_s( limname, MAX_PATH, szFileName );
			}
			if( cmpto1 == 0 ){
				strcpy_s( to1name, MAX_PATH, szFileName );
			}
			if( cmppos == 0 ){
				strcpy_s( posname, MAX_PATH, szFileName );
			}
		}
	}
    DragFinish(hDrop);

	int ret;
	if( shapename[0] ){
		switch( shapetype ){
		case SHAPEF_SIG:
			ret = LoadSigFile( shapename );
			if( ret ){
				return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
			break;
		case SHAPEF_MQO:
			ret = LoadMQOFile( 0, shapename );
			if( ret ){
				return 0;
			}
			break;
		case SHAPEF_ROK:
			ret = LoadRokFile( shapename );
			if( ret ){
				return 0;
			}
			break;
		case SHAPEF_X:
			ret = LoadXFile( shapename );
			if( ret ){
				return 0;
			}
			break;
		default:
			return 0;
			break;
		}
	}

	if( bonname[0] ){
		LOADFILEINFO ldinfo;
		ZeroMemory( &ldinfo, sizeof( LOADFILEINFO ) );
		ldinfo.filename = bonname;

		ret = LoadBoneFile( &ldinfo );
		if( !ret ){
			strcpy_s( m_loadnamedlg->m_bonname, 2048, bonname );
		}
	}

	if( im2name[0] ){
		ret = LoadIM2File( 0, im2name );
		_ASSERT( !ret );
		
	}

	if( limname[0] ){
		ret = LoadLimFile( limname );
		_ASSERT( !ret );
	}

	if( to1name[0] ){
		ret = LoadToon1File( to1name );
		_ASSERT( !ret );
	}

	if( moaname[0] ){
		ret = LoadMAFile( moaname );
		_ASSERT( !ret );
	}

	if( (moaname[0] == 0) && (quanum > 0) ){

		CLdQuaDlg dlg;
		ret = (int)dlg.DoModal( m_hWnd );
		if( ret != IDOK )
			return 0;

		int quano;
		for( quano = 0; quano < quanum; quano++ ){
			char* curname;
			curname = &( quaname[quano][0] );
			if( *curname ){
				ret = LoadMotion( curname, dlg.multvec, 0, 0, 0, 0, 0, 0 );
				if( ret ){
					DbgOut( "viewer : LoadMotionMulti : LoadMotion error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
		}

		ret = g_motdlg->GetSelectedJointPos( &m_selectedno, &m_selectedpos );
		_ASSERT( !ret );
		ret = g_motdlg->OnUserSelchange( m_selectedno );//TexKeyDlgのリスト更新
		_ASSERT( !ret );
	}

	if( posname[0] ){
		ret = LoadPoseFile( posname );
		_ASSERT( !ret );
	}

	return 0;
}

int CMyD3DApplication::LoadFile()
{

	if( g_motdlg ){
		g_motdlg->SelectMotParamTab();
		g_motdlg->ResetCamTargetBone();
	}


	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = 
(LPCTSTR)"形状(sig(標準),mqo,rok,x)\0*.sig;*.mqo;*.rok;*.x\0モーション(biovision社)bvh\0*.bvh\0ボーンテキスト情報(bon)\0*.bon\0ボーン影響度(im2)\0*.im2\0モーションアクセラレータ(moa)\0*.moa\0制限角度(lim)\0*.lim\0トゥーン１(to1)\0*.to1\0ポーズ(pos)\0*.pos\0音セット(ssf)\0*.ssf\0音アニメ(saf)\0*.saf\0バッチ読込(bld)\0*.bld\0ModToolShape(esi)\0*.esi\0KinectSetting(kst)\0*.kst\0";
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

	int nodataflag = 0;

	m_ExistData = FALSE;

	int ret = 0;
	int ch = '.';
	char* extptr = 0;
	extptr = strrchr( buf, ch );
	if( extptr ){
		int cmpsig, cmprok, cmpmqo, cmpx, cmpbvh, cmpbon, cmpim2, cmpmoa, cmplim, cmpto1, cmppos, cmpssf, cmpsaf, cmpbld, cmpesi, cmpequ, cmpkst;
		int cmpsigA, cmprokA, cmpmqoA, cmpbvhA, cmpbonA, cmpim2A, cmpmoaA, cmplimA, cmpto1A, cmpposA, cmpssfA, cmpsafA, cmpbldA, cmpesiA, cmpequA, cmpkstA;
		cmpsig = strcmp( extptr, ".sig" );
		cmprok = strcmp( extptr, ".rok" );
		cmpmqo = strcmp( extptr, ".mqo" );
		cmpx = strcmp( extptr, ".x" );
		if( cmpx != 0 ){
			cmpx = strcmp( extptr, ".X" );
		}
		cmpbvh = strcmp( extptr, ".bvh" );
		cmpbon = strcmp( extptr, ".bon" );
		cmpim2 = strcmp( extptr, ".im2" );
		cmpmoa = strcmp( extptr, ".moa" );
		cmplim = strcmp( extptr, ".lim" );
		cmpto1 = strcmp( extptr, ".to1" );
		cmppos = strcmp( extptr, ".pos" );
		cmpssf = strcmp( extptr, ".ssf" );
		cmpsaf = strcmp( extptr, ".saf" );
		cmpbld = strcmp( extptr, ".bld" );
		cmpesi = strcmp( extptr, ".esi" );
		cmpequ = strcmp( extptr, ".equ" );
		cmpkst = strcmp( extptr, ".kst" );


		cmpsigA = strcmp( extptr, ".SIG" );
		cmprokA = strcmp( extptr, ".ROK" );
		cmpmqoA = strcmp( extptr, ".MQO" );
		cmpbvhA = strcmp( extptr, ".BVH" );
		cmpbonA = strcmp( extptr, ".BON" );
		cmpim2A = strcmp( extptr, ".IM2" );
		cmpmoaA = strcmp( extptr, ".MOA" );
		cmplimA = strcmp( extptr, ".LIM" );
		cmpto1A = strcmp( extptr, ".TO1" );
		cmpposA = strcmp( extptr, ".POS" );
		cmpssfA = strcmp( extptr, ".SSF" );
		cmpsafA = strcmp( extptr, ".SAF" );
		cmpbldA = strcmp( extptr, ".BLD" );
		cmpesiA = strcmp( extptr, ".ESI" );
		cmpequA = strcmp( extptr, ".EQU" );
		cmpkstA = strcmp( extptr, ".KST" );

		if( !cmpsigA || !cmprokA || !cmpmqoA || !cmpbvhA || !cmpbonA || !cmpim2A ||
			!cmpmoaA || !cmplimA || !cmpto1A || !cmpposA || !cmpssfA || !cmpsafA ||
			!cmpbldA || !cmpesiA || !cmpequA || !cmpkstA ){

			::MessageBox( m_hWnd, "ファイルの拡張子は小文字にしてください。", "エラー", MB_OK );

			if( m_shandler && (m_shandler->s2shd_leng > 1) ){
				m_ExistData = TRUE;
			}else{
				m_ExistData = FALSE;
			}

			return 1;
		}



		if( cmpsig == 0 ){
			g_loadingflag = 1;

			ret = LoadSigFile( buf );
			if( ret ){
				_ASSERT( 0 );
				nodataflag = 1;
			}

		}else if( cmprok == 0 ){
			g_loadingflag = 1;

			ret = LoadRokFile( buf );
			if( ret ){
				_ASSERT( 0 );
				nodataflag = 1;
			}
		}else if( cmpmqo == 0 ){
			g_loadingflag = 1;

			ret = LoadMQOFile( 0, buf );
			if( ret ){
				_ASSERT( 0 );
				nodataflag = 1;
			}
		}else if( cmpx == 0 ){
			g_loadingflag = 1;

			ret = LoadXFile( buf );
			if( ret ){
				_ASSERT( 0 );
				nodataflag = 1;
			}
		}else if( cmpbvh == 0 ){
			g_loadingflag = 1;

			ret = LoadBVHFile( buf );
			_ASSERT( !ret );
		}else if( cmpbon == 0 ){
			g_loadingflag = 1;

			LOADFILEINFO ldinfo;
			ZeroMemory( &ldinfo, sizeof( LOADFILEINFO ) );
			ldinfo.filename = buf;

			ret = LoadBoneFile( &ldinfo );
			if( !ret ){
				strcpy_s( m_loadnamedlg->m_bonname, 2048, buf );
			}
			_ASSERT( !ret );
		}else if( cmpim2 == 0 ){
			g_loadingflag = 1;

			ret = LoadIM2File( 0, buf );
			_ASSERT( !ret );
		}else if( cmpmoa == 0 ){
			g_loadingflag = 1;

			ret = LoadMAFile( buf );
			_ASSERT( !ret );
		}else if( cmplim == 0 ){
			g_loadingflag = 1;

			ret = LoadLimFile( buf );
			_ASSERT( !ret );
		}else if( cmpto1 == 0 ){
			g_loadingflag = 1;

			ret = LoadToon1File( buf );
			_ASSERT( !ret );
		}else if( cmppos == 0 ){
			g_loadingflag = 1;

			ret = LoadPoseFile( buf );
			_ASSERT( !ret );
		}else if( cmpssf == 0 ){
			g_loadingflag = 1;

			ret = LoadSndSetFile( buf );
			_ASSERT( !ret );
		}else if( cmpsaf == 0 ){
			g_loadingflag = 1;

			ret = LoadSndAnimFile( buf, 1 );
			_ASSERT( !ret );
		}else if( cmpbld == 0 ){
			g_loadingflag = 1;

			ret = LoadBLDFile( buf );
		}else if( cmpesi == 0 ){
			g_loadingflag = 1;

			ret = LoadESIFile( buf );
		}else if( cmpequ == 0 ){
			//ret = LoadEQUFile( buf );
		}
	}

	if( nodataflag == 1 ){
		m_ExistData = FALSE;
	}else{
		if( m_shandler && (m_shandler->s2shd_leng > 1) ){
			m_ExistData = TRUE;
		}else{
			m_ExistData = FALSE;
		}
	}

	g_loadingflag = 0;


	return ret;
}
int CMyD3DApplication::ImportFile()
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"形状(sig,mqo,rok)\0*.sig;*.mqo;*.rok\0";
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

	int ret = 0;
	int ch = '.';
	char* extptr = 0;
	extptr = strrchr( buf, ch );
	if( extptr ){
		int cmpsig, cmprok, cmpmqo;
		cmpsig = strcmp( extptr, ".sig" );
		cmprok = strcmp( extptr, ".rok" );
		cmpmqo = strcmp( extptr, ".mqo" );

		if( cmpsig == 0 ){
			ret = ImportSigFile( buf );
			_ASSERT( !ret );
		}else if( cmprok == 0 ){
			ret = ImportRokFile( buf );
			_ASSERT( !ret );
		}else if( cmpmqo == 0 ){
			ret = ImportMqoFile( 0, buf );
			_ASSERT( !ret );
		}
	}

	return ret;
}
int CMyD3DApplication::SaveCameraFile( char* srcfilename )
{
	if( !g_motdlg->m_camdlg->m_cah ){
		_ASSERT( 0 );
		return 0;
	}
	if( g_motdlg->m_camdlg->m_cah->m_animnum <= 0 ){
		_ASSERT( 0 );
		return 0;
	}

	CCameraAnim* anim;
	anim = g_motdlg->m_camdlg->m_cah->GetCurCameraAnim();
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	if( srcfilename == 0 ){
		sprintf_s( buf, _MAX_PATH, "%s.cam", anim->m_animname );

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"カメラアニメcam\0*.cam\0";
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
		ofn.lpstrDefExt =(LPCTSTR)"cam\0";
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( GetSaveFileName(&ofn) == 0 ){
			return 0;
		}
	}else{
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}

	int ret;
	CCameraFile camfile;
	ret = camfile.WriteCameraFile( buf, anim );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	return 0;

}
int CMyD3DApplication::SaveMOEFile( char* srcfilename )
{
	if( !g_motdlg->m_moedlg->m_moeah ){
		_ASSERT( 0 );
		return 0;
	}
	if( g_motdlg->m_moedlg->m_moeah->m_animnum <= 0 ){
		_ASSERT( 0 );
		return 0;
	}

	CMOEAnim* anim;
	anim = g_motdlg->m_moedlg->m_moeah->GetCurMOEAnim();
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	if( srcfilename == 0 ){
		sprintf_s( buf, _MAX_PATH, "%s.moe", anim->m_animname );

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"moaイベントアニメmoe\0*.moe\0";
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
		ofn.lpstrDefExt =(LPCTSTR)"moe\0";
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( GetSaveFileName(&ofn) == 0 ){
			return 0;
		}
	}else{
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}

	int ret;
	CMOEFile moefile;
	ret = moefile.WriteMOEFile( m_thandler, m_shandler, buf, anim );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}


int CMyD3DApplication::SaveGPFile( char* srcfilename )
{
	if( !g_motdlg->m_gpdlg->m_gpah ){
		_ASSERT( 0 );
		return 0;
	}
	if( g_motdlg->m_gpdlg->m_gpah->m_animnum <= 0 ){
		_ASSERT( 0 );
		return 0;
	}

	CGPAnim* anim;
	anim = g_motdlg->m_gpdlg->m_gpah->GetCurGPAnim();
	if( !anim ){
		_ASSERT( 0 );
		return 0;
	}

	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	if( srcfilename == 0 ){
		sprintf_s( buf, _MAX_PATH, "%s.gpa", anim->m_animname );

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = 0;
		ofn.lpstrFilter = (LPCTSTR)"GlobalPositionアニメgpa\0*.gpa\0";
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
		ofn.lpstrDefExt =(LPCTSTR)"gpa\0";
		ofn.lCustData = NULL;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
		if( GetSaveFileName(&ofn) == 0 ){
			return 0;
		}
	}else{
		strcpy_s( buf, _MAX_PATH, srcfilename );
	}

	int ret;
	CGPFile gpfile;
	ret = gpfile.WriteGPFile( buf, anim );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	return 0;

}

int CMyD3DApplication::SaveQuaFile()
{
	char* motname = 0;
	if( m_mhandler){
		CMotionCtrl* mcptr = (*m_mhandler)( 1 );
		if( mcptr ){
			CMotionInfo* miptr = mcptr->motinfo;
			if( miptr ){
				int curmotid;
				curmotid = g_motdlg->GetMotCookie();
				if( curmotid >= 0 ){
					motname = *(miptr->motname + curmotid);
				}
			}
		}
	}

	OPENFILENAME ofn;
	char buf[_MAX_PATH];

	if( motname == 0 ){
		strcpy_s( buf, _MAX_PATH, "default.qua" );
	}else{
		sprintf_s( buf, _MAX_PATH, "%s.qua", motname );
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"モーション(標準)qua\0*.qua\0";
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
	ofn.lpstrDefExt =(LPCTSTR)"qua\0";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 ){
		return 0;
	}

	int ret = 0;
	if( g_motdlg && m_mhandler ){
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		ret = g_motdlg->SaveMotion( m_loadnamedlg, buf );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		_ASSERT( !ret );
	}

	return ret;
}


int CMyD3DApplication::SaveFile()
{

	OPENFILENAME ofn;
	char buf[_MAX_PATH] = "default.sig";

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = 
(LPCTSTR)"形状(標準)sig\0*.sig\0連番mqo\0*.mqo\0マルチトラックXfile\0*.x\0モーション(biovision社)bvh\0*.bvh\0ボーンテキスト情報(bon)\0*.bon\0ボーン影響度(im2)\0*.im2\0制限角度(lim)\0*.lim\0トゥーン１(to1)\0*.to1\0ポーズ(pos)\0*.pos\0音セット(ssf)\0*.ssf\0音アニメ(saf)\0*.saf\0";
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
	ofn.lpstrDefExt =(LPCTSTR)"sig\0mqo\0x\0bvh\0bon\0im2\0lim\0to1\0pos\0ssf\0saf\0";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 ){
		return 0;
	}

	int ret = 0;
	int ch = '.';
	char* extptr = 0;
	extptr = strrchr( buf, ch );
	if( extptr ){
		int cmpsig, cmpmqo, cmpx, cmpbvh, cmpbon, cmpim2, cmplim, cmpto1, cmppos, cmpssf, cmpsaf;
		cmpsig = strcmp( extptr, ".sig" );
		//cmpqua = strcmp( extptr, ".qua" );
		cmpmqo = strcmp( extptr, ".mqo" );
		cmpx = strcmp( extptr, ".x" );
		cmpbvh = strcmp( extptr, ".bvh" );
		cmpbon = strcmp( extptr, ".bon" );
		cmpim2 = strcmp( extptr, ".im2" );
		cmplim = strcmp( extptr, ".lim" );
		cmpto1 = strcmp( extptr, ".to1" );
		cmppos = strcmp( extptr, ".pos" );
		cmpssf = strcmp( extptr, ".ssf" );
		cmpsaf = strcmp( extptr, ".saf" );

		if( cmpsig == 0 ){
			ret = SaveSigFile( buf );
			_ASSERT( !ret );
		//}else if( cmpqua == 0 ){
		//	if( g_motdlg && m_mhandler ){
		//		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		//		ret = g_motdlg->SaveMotion( m_loadnamedlg, buf );
		//		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		//		_ASSERT( !ret );
		//	}
		}else if( cmpmqo == 0 ){
			if( m_writebmpflag == 1 )
				return 0;
			m_writebmpflag = 1;
			ret = WriteMQOFile( buf );
			m_writebmpflag = 0;
			_ASSERT( !ret );

		}else if( cmpx == 0 ){
			ret = SaveXFile( buf );
			_ASSERT( !ret );
		}else if( cmpbvh == 0 ){
			ret = SaveBVHFile( buf );
			_ASSERT( !ret );
		}else if( cmpbon == 0 ){
			ret = WriteBoneFile( buf );
			_ASSERT( !ret );
		}else if( cmpim2 == 0 ){
			ret = WriteIM2File( buf );
			_ASSERT( !ret );
		}else if( cmplim == 0 ){
			ret = SaveLimFile( buf );
			_ASSERT( !ret );
		}else if( cmpto1 == 0 ){
			ret = SaveToon1File( buf );
			_ASSERT( !ret );
		}else if( cmppos == 0 ){
			ret = SavePoseFile( buf );
			_ASSERT( !ret );
		}else if( cmpssf == 0 ){
			ret = SaveSndSetFile( buf );
			_ASSERT( !ret );
		}else if( cmpsaf == 0 ){
			ret = SaveSndAnimFile( buf );
			_ASSERT( !ret );
		}
	}

	return ret;
}

int CMyD3DApplication::LoadBVHFile( char* srcfilename )
{

	if( g_motdlg ){

		if( g_inidata->motdlg_disp == 0 ){
			//motparamdlgを表示しない状態で、BVHを読むと、LoadMQOFileで、mothandler に有効な値が入らない。
			::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDMENU_MOTEDIT, 0 );
		}

		if( m_tbb_iti ){
			SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );
		}

		if( g_motdlg->GetMotionNum() >= MAXMOTIONNUM ){
			char strmes[512];
			sprintf_s( strmes, 512, "読み込めるモーションは、%d個までです。\nこれ以上、読み込めません。", MAXMOTIONNUM );
			::MessageBox( m_hWnd, strmes, "エラー", MB_OK );
			return 0;
		}


		//motion関係のメモリのアロケート、デリートはmotdlgで行うため、処理を投げる。
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );
		int ret;
		ret = g_motdlg->LoadBVHFile( srcfilename );
		::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );
		_ASSERT( !ret );

		ret = ChangeZaKindFromMotion( g_motdlg->GetMotCookie() );
		_ASSERT( !ret );

		m_setcoldlg->InitUndoBuf();
		m_paintdlg->InitUndoBuf();

		if( m_mcdlg->m_firstmake ){
			ret = m_mcdlg->AddParentMC( g_motdlg->GetMotCookie(), 0, 0, 0, 0, 0, 0 );
			if( ret ){
				DbgOut( "viewer : load bvh 5011 : mcdlg AddParentMC error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}

int CMyD3DApplication::AddMorphFolder( int baseid, int boneid )
{
	int ret;

	if( !m_ExistData || !m_thandler || !m_shandler || !m_mhandler ){
		::MessageBox( m_hWnd, "形状データを読み込んでいないので作成できません。", "エラー", MB_OK );
		return 0;
	}
	if( m_shandler->s2shd_leng <= 1 ){
		//データが無い時は、何もしない。
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	if( g_motdlg->m_disptabno == TAB_MOE ){
		ret = g_motdlg->SelectMotParamTab();
		_ASSERT( !ret );
	}

	int jointnum = 0;
	ret = m_shandler->GetJointNum( &jointnum );
	_ASSERT( !ret );
	if( jointnum <= 0 ){
		::MessageBox( m_hWnd, "ジョイントが１つも無いので作成できません。", "エラー", MB_OK );
		return 0;
	}

	CShdElem* baseelem = 0;
	CMotionCtrl* motionelem = 0;
	CTreeElem2* basetelem = 0;
	int createokflag = 0;

	if( (baseid <= 0) || (boneid <= 0) ){
		CMFolderDlg dlg( m_thandler, m_shandler );
		ret = (int)dlg.DoModal();
		if( (ret == IDOK) && (dlg.m_baseseri > 0) && (dlg.m_motionseri > 0) ){
			createokflag = 1;

			basetelem = (*m_thandler)( dlg.m_baseseri );
			_ASSERT( basetelem );
			baseelem = (*m_shandler)( dlg.m_baseseri );
			_ASSERT( baseelem );
			motionelem = (*m_mhandler)( dlg.m_motionseri );
			_ASSERT( motionelem );

		}else{
			createokflag = 0;
		}

	}else{
		createokflag = 1;

		basetelem = (*m_thandler)( baseid );
		_ASSERT( basetelem );
		baseelem = (*m_shandler)( baseid );
		_ASSERT( baseelem );
		motionelem = (*m_mhandler)( boneid );
		_ASSERT( motionelem );
	}
	

	if( createokflag ){
		if( baseelem->m_mtype != M_NONE ){
			DbgOut( "viewer : AddMorphFolder : baseelem already in morph error !!!\n" );
			//_ASSERT( 0 );
			return 0;
		}


		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

		if( g_inidata->opt_modlg == CHKDLG_NES ){
			ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
		}else if( g_inidata->opt_modlg == CHKDLG_YES ){
			ret = IDOK;
		}else{
			ret = IDCANCEL;
			if( g_loadingflag ){
				ret = IDOK;
			}

			return 0;//!!!!!!!!!!
		}
		if( ret == IDOK ){
			ret = m_mhandler->DestroyMotionObj();
			if( ret ){
				DbgOut( "viewer : AddMorphFolder : DestroyMotionObj error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			ret = g_motdlg->RemakeMOEAH();
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

		}else{
			::MessageBox( m_hWnd, "モーションを保存してから再試行してください。", "確認", MB_OK );
			return 0;
		}

		//////////////////
		m_ExistData = FALSE;

		int serino = m_shandler->s2shd_leng;
		int srctype = SHDMORPH;

		int depth;	
		char newname[256];
		m_thandler->Start( serino - 1 );
		m_thandler->Begin();
		depth = m_thandler->GetCurDepth();
		ZeroMemory( newname, 256 );
		sprintf_s( newname, 256, "MFolder:%s", basetelem->name );

		ret = Add2Tree( newname, serino, SHDMORPH, depth, 0 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		m_thandler->End();


		CShdElem* curelem;
		curelem = (*m_shandler)( serino );
		_ASSERT( curelem );
		CMorph* morph;
		morph = curelem->morph;
		_ASSERT( morph );
		ret = morph->SetBaseParams( m_shandler, m_mhandler, baseelem, motionelem );
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : morph SetBaseParams error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		CShdElem* topelem;
		topelem = (*m_shandler)( 1 );
		_ASSERT( topelem );
		ret = curelem->PutToChain( topelem, 0 );
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : PutToChain error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = m_shandler->CalcDepth();
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : shandler CalcDepth 0 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = baseelem->LeaveFromChain();
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : base LeaveFromChain error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = baseelem->PutToChain( curelem, 1 );
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : PutToChain error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ret = m_shandler->CalcDepth();
		if( ret ){
			DbgOut( "viewer : CreateBone : shandler CalcDepth 1 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		
		ret = m_mhandler->CopyChainFromShd( m_shandler );
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : mhandler CopyChainFromShd error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = m_mhandler->CalcDepth();
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : mhandler CalcDepth error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		////////////////////
		////////////////////
//		ret = m_shandler->CreateBoneInfo( m_mhandler );
//		if( ret ){
//			DbgOut( "viewer : AddMorphFolder : CreateBoneInfo error !!!\n" );
//			_ASSERT( 0 );
//			return 1;
//		}


		ret = RestoreHandler( 1 );
		if( ret ){
			DbgOut( "viewer : AddMorphFolder : RestoreHandler error !!!\n" );
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
					DbgOut( "viewer : AddMorphFolder : mcdlg InitMC error !!!\n" );
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
//		SendMessage( m_hWnd, WM_USER_INIT_UNDOBUF, 0, 1 );
//		m_bonedlg->SetCurrentName( newname );

		g_motdlg->ResetMorphDlg();


		m_ExistData = TRUE;

	}

	return 0;
}

int CMyD3DApplication::SetEffectDecl( LPDIRECT3DDEVICE9 pdev )
{
	D3DVERTEXELEMENT9 ve[] = {
		//pos[4]
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

		//tex1[2]//tex0
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		D3DDECL_END()

	};
	
	D3DVERTEXELEMENT9 vetlv[] = {
		//pos[4]
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

		//diffuse
		{ 0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

		//tex0
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		D3DDECL_END()

	};


	if( m_effectdecl ){
		m_effectdecl->Release();
		m_effectdecl = 0;
	}
	if( m_tlvdecl ){
		m_tlvdecl->Release();
		m_tlvdecl = 0;
	}

	HRESULT hr;
	hr = pdev->CreateVertexDeclaration( ve, &m_effectdecl );
	if( hr != D3D_OK ){
		DbgOut( "SetEffectDecl : CreateVertexDeclaration effectdecl error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	hr = pdev->CreateVertexDeclaration( vetlv, &m_tlvdecl );
	if( hr != D3D_OK ){
		DbgOut( "SetEffectDecl : CreateVertexDeclaration tlv error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}

int CMyD3DApplication::RenderFullScreenTLV( LPDIRECT3DSURFACE9 dstsurf, D3DVIEWPORT9* dstviewport, 
	LPDIRECT3DTEXTURE9 srctex, int texwidth, int texheight, int texf )
{
	int ret;
	HRESULT hres;
	LPDIRECT3DDEVICE9 pdev = m_pd3dDevice;
	if( !pdev ){
		_ASSERT( 0 );
		return 1;
	}
	if( !g_pPostEffect ){
		_ASSERT( 0 );
		return 1;
	}
	int skipbeginscene = 0;
	int needz = 0;
	ret = RTBeginScene( pdev, dstsurf, 0, dstviewport, skipbeginscene, needz );
	if( ret ){
		DbgOut( "RenderFullScreenTLV : RTBeginScene error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	float screenw, screenh;
	screenw = (float)dstviewport->Width;
	screenh = (float)dstviewport->Height;

	ret = SetRTRenderState( pdev, needz );
	_ASSERT( !ret );

	pdev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_renderstate[ D3DRS_ALPHABLENDENABLE ] = TRUE;
	
	pdev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	g_renderstate[ D3DRS_BLENDOP ] = D3DBLENDOP_ADD;

	pdev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	g_renderstate[ D3DRS_SRCBLEND ] = D3DBLEND_ONE;

	pdev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	g_renderstate[ D3DRS_DESTBLEND ] = D3DBLEND_ONE;

	hres = g_pPostEffect->SetTechnique( g_hPostEffectTLV );
	_ASSERT( hres == D3D_OK );
	hres = g_pPostEffect->Begin( NULL, 0 );
	_ASSERT( hres == D3D_OK );

///////////
	float fw1 = (float)texwidth;
	float fh1 = (float)texheight;
	hres = g_pPostEffect->SetValue( g_hpeWidth0, &fw1, sizeof( float ) );
	if( hres != D3D_OK ){
		DbgOut( "RenderFullScreenTLV :width0 SetValue error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	hres = g_pPostEffect->SetValue( g_hpeHeight0, &fh1, sizeof( float ) );
	if( hres != D3D_OK ){
		DbgOut( "RenderFullScreenTLV :height0 SetValue error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	hres = g_pPostEffect->SetTexture( g_hpeTexture0, srctex );
	if( hres != D3D_OK ){
		DbgOut( "RenderFullScreenTLV :texture0 SetTexture error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int passno1;
	float offsetu1, offsetv1;
	if( texf == D3DTEXF_POINT ){
		passno1 = 0;
		offsetu1 = 0.5f / screenw;
		offsetv1 = 0.5f / screenh;
	}else{
		passno1 = 1;
		offsetu1 = 0.0f;
		offsetv1 = 0.0f;
	}
	hres = g_pPostEffect->BeginPass( passno1 );
	_ASSERT( hres == D3D_OK );


	EFFECTTLV Vertex1[4] = {
		{{-1.0f, +1.0f, 0.2f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f + offsetu1, 0.0f + offsetv1} },
		{{+1.0f, +1.0f, 0.2f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f + offsetu1, 0.0f + offsetv1} },
		{{+1.0f, -1.0f, 0.2f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f + offsetu1, 1.0f + offsetv1} },
		{{-1.0f, -1.0f, 0.2f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f + offsetu1, 1.0f + offsetv1} }
	};

	hres = pdev->SetVertexDeclaration( m_tlvdecl );
	_ASSERT( hres == D3D_OK );

	hres = pdev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
					, 2, Vertex1, sizeof( EFFECTTLV ) );
	_ASSERT( hres == D3D_OK );

	hres = g_pPostEffect->EndPass();
	_ASSERT( hres == D3D_OK );

	hres = g_pPostEffect->End();
	_ASSERT( hres == D3D_OK );

    pdev->EndScene();

	return 0;
}

int CMyD3DApplication::RTBeginScene( LPDIRECT3DDEVICE9 pdev, LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, 
	int skipbeginscene, int needz )
{
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//needz 0 --> setなしclearなし。　1 --> set有りclar有り　2 --> set有りclearなし。
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	HRESULT hres;
	hres = pdev->SetRenderTarget( 0, psurf );
	if( hres != D3D_OK ){
		DbgOut( "RTBeginScene : 3ddev SetRenderTarget error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
		_ASSERT( 0 );
		return 1;
	}
	if( needz != 1 ){
		hres = pdev->SetDepthStencilSurface( pzsurf );
		if( hres != D3D_OK ){
			DbgOut( "RTBeginScene : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		hres = pdev->SetDepthStencilSurface( NULL );
		if( hres != D3D_OK ){
			DbgOut( "RTBeginScene : 3ddev SetDepthStencilSurface error %x, %x!!!\n", hres, D3DERR_INVALIDCALL );
			_ASSERT( 0 );
			return 1;
		}
	}

    hres = pdev->SetViewport( pviewport );
	if( hres != D3D_OK ){
		DbgOut( "RTBeginScene : dev SetViewport error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( skipbeginscene == 0 ){
		if( FAILED( pdev->BeginScene() ) ){
			_ASSERT( 0 );
			return 1;
		}
	}

    // Clear the viewport
	if( needz == 1 ){
		hres = pdev->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
							 0,
							 1.0f, 0L );
	}else{
		hres = pdev->Clear( 0L, NULL, D3DCLEAR_TARGET,// | D3DCLEAR_ZBUFFER, 
							 0,
							 1.0f, 0L );
	}
	if( hres != D3D_OK ){
		_ASSERT( 0 );
		DbgOut( "RTBeginScene : Clear error !!!\n" );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::SetRTRenderState( LPDIRECT3DDEVICE9 pdev, int needz )
{
	if( needz ){
		pdev->SetRenderState( D3DRS_ZENABLE,  D3DZB_TRUE );
		pdev->SetRenderState( D3DRS_ZWRITEENABLE,  TRUE );
		pdev->SetRenderState( D3DRS_ZFUNC,  D3DCMP_LESSEQUAL );
		g_renderstate[ D3DRS_ZENABLE ] = D3DZB_TRUE;
		g_renderstate[ D3DRS_ZWRITEENABLE ] = TRUE;
		g_renderstate[ D3DRS_ZFUNC ] = D3DCMP_LESSEQUAL;
	}else{
		pdev->SetRenderState( D3DRS_ZENABLE, FALSE );
		g_renderstate[D3DRS_ZENABLE] = 0;	
	}


	if( g_cop0 != D3DTOP_MODULATE ){
		pdev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_cop0 = D3DTOP_MODULATE;
	}
	if( g_cop1 != D3DTOP_DISABLE ){
		pdev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE ); 
		g_cop1 = D3DTOP_DISABLE;
	}
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	if( g_aop0 != D3DTOP_MODULATE ){
		pdev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );//!!!!
		g_aop0 = D3DTOP_MODULATE;
	}
	if( g_aop1 != D3DTOP_DISABLE ){
		pdev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );//!!!!
		g_aop1 = D3DTOP_DISABLE;
	}

	return 0;
}

int CMyD3DApplication::RenderGlow( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int zclear )
{
	int ret;
	D3DXMATRIX matProjX;
	if( m_ortho == 0 ){
		float fov = (float)m_fov * (float)DEG2PAI;
		float aspect;
		aspect = (float)pviewport->Width / (float)pviewport->Height;
		D3DXMatrixPerspectiveFovLH( &matProjX, fov, aspect, m_proj_near, m_proj_far );

	}else{
		float vw, vh;
		vw = m_orthosize;
		vh = m_orthosize * ((float)pviewport->Height / (float)pviewport->Width);
		D3DXMatrixOrthoLH( &matProjX, vw, vh, m_proj_near, m_proj_far );
	}


	LPDIRECT3DDEVICE9 pdev = m_pd3dDevice;
	if( !pdev ){
		_ASSERT( 0 );
		return 1;
	}
	if( !g_pEffect ){
		_ASSERT( 0 );
		return 1;
	}

	ret = SetShaderConst( 0, &matProjX );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	int skipbeginscene = 0;
	int needz;
	if( zclear == 0 ){
		if( pzsurf ){
			needz = 2;
		}else{
			needz = 0;
		}
	}else{
		if( pzsurf ){
			needz = 1;
		}else{
			needz = 0;
		}
	}
	ret = RTBeginScene( pdev, psurf, 0, pviewport, skipbeginscene, needz );
	if( ret ){
		DbgOut( "RenderFullScreenTLV : RTBeginScene error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	float screenw, screenh;
	screenw = (float)pviewport->Width;
	screenh = (float)pviewport->Height;

	pdev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	g_renderstate[ D3DRS_ALPHATESTENABLE ] = FALSE;
//	pdev->SetRenderState( D3DRS_ALPHAREF, 0 );
//	g_renderstate[ D3DRS_ALPHAREF ] = 0;
//	pdev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
//	g_renderstate[ D3DRS_ALPHAFUNC ] = D3DCMP_GREATEREQUAL;


	if( g_inidata->opt_gdisp ){
		ret = m_ground_sh->Render( 0, 0, m_pd3dDevice, 0, POLYDISP_ALL, m_ground_mh, 0, m_inimat, m_matView, matProjX, m_vEyePt, 0, 0, WITHGLOW );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderGlow Ground: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_g_sh && m_g_disp ){
		ret = m_g_sh->Render( 0, 0, m_pd3dDevice, 0, POLYDISP_ALL, m_g_mh, 0, m_inimat, m_matView, matProjX, m_vEyePt, 0, 0, WITHGLOW );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderGlow G: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	if( m_ExistData ){
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		ret = m_shandler->Render( 0, 0, m_pd3dDevice, 0, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, WITHGLOW );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderGlow: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		//半透明
		ret = m_shandler->Render( 0, 0, m_pd3dDevice, 1, g_inidata->opt_polydisp, m_mhandler, g_motdlg->GetMotCookie(), m_matWorld, m_matView, matProjX, m_vEyePt, 0, 0, WITHGLOW );
		if( ret ){
			DbgOut( "CMyD3DApplication : RenderGlow with alpha: error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

    pdev->EndScene();
	
	return 0;
}

int CMyD3DApplication::BlendRTTexture( LPDIRECT3DSURFACE9 dstsurf, D3DVIEWPORT9* dstviewport, 
	LPDIRECT3DTEXTURE9 ptex1, int width1, int height1, E3DCOLOR4F col1, int texf1,
	LPDIRECT3DTEXTURE9 ptex2, int width2, int height2, E3DCOLOR4F col2, int texf2, 
	int blendop, int srcblend, int destblend )
{
	int ret;
	HRESULT hres;
	LPDIRECT3DDEVICE9 pdev = m_pd3dDevice;
	if( !pdev ){
		_ASSERT( 0 );
		return 1;
	}

	int needz = 0;
	ret = RTBeginScene( pdev, dstsurf, 0, dstviewport, 0, needz );
	if( ret ){
		DbgOut( "BlendRTTexture : RTBeginScene error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	float screenw, screenh;
	screenw = (float)dstviewport->Width;
	screenh = (float)dstviewport->Height;

	if( g_pPostEffect != NULL )
	{
		ret = SetRTRenderState( pdev, needz );
		_ASSERT( !ret );

		pdev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_renderstate[ D3DRS_ALPHABLENDENABLE ] = TRUE;
		
		pdev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		g_renderstate[ D3DRS_BLENDOP ] = D3DBLENDOP_ADD;

		pdev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		g_renderstate[ D3DRS_SRCBLEND ] = D3DBLEND_ONE;

		pdev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		g_renderstate[ D3DRS_DESTBLEND ] = D3DBLEND_ONE;

		//-------------------------------------------------
		// シェーダの設定
		//-------------------------------------------------
		hres = g_pPostEffect->SetTechnique( g_hPostEffectTLV );
		_ASSERT( hres == D3D_OK );
		hres = g_pPostEffect->Begin( NULL, 0 );
		_ASSERT( hres == D3D_OK );

	///////////
		if( ptex1 ){
			float fw1 = (float)width1;
			float fh1 = (float)height1;
			hres = g_pPostEffect->SetValue( g_hpeWidth0, &fw1, sizeof( float ) );
			if( hres != D3D_OK ){
				DbgOut( "E3DBlendRTTexture :width0 SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			hres = g_pPostEffect->SetValue( g_hpeHeight0, &fh1, sizeof( float ) );
			if( hres != D3D_OK ){
				DbgOut( "E3DBlendRTTexture :height0 SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			hres = g_pPostEffect->SetTexture( g_hpeTexture0, ptex1 );
			if( hres != D3D_OK ){
				DbgOut( "E3DBlendRTTexture :texture0 SetTexture error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			int passno1;
			float offsetu1, offsetv1;
			if( texf1 == D3DTEXF_POINT ){
				passno1 = 0;
				offsetu1 = 0.5f / screenw;
				offsetv1 = 0.5f / screenh;
			}else{
				passno1 = 1;
				offsetu1 = 0.0f;
				offsetv1 = 0.0f;
			}
			hres = g_pPostEffect->BeginPass( passno1 );
			_ASSERT( hres == D3D_OK );


			EFFECTTLV Vertex1[4] = {
				//   x    y     z      w     u0 v0
				{{-1.0f, +1.0f, 0.2f, 1.0f}, {col1.r, col1.g, col1.b, col1.a}, {0.0f + offsetu1, 0.0f + offsetv1} },
				{{+1.0f, +1.0f, 0.2f, 1.0f}, {col1.r, col1.g, col1.b, col1.a}, {1.0f + offsetu1, 0.0f + offsetv1} },
				{{+1.0f, -1.0f, 0.2f, 1.0f}, {col1.r, col1.g, col1.b, col1.a}, {1.0f + offsetu1, 1.0f + offsetv1} },
				{{-1.0f, -1.0f, 0.2f, 1.0f}, {col1.r, col1.g, col1.b, col1.a}, {0.0f + offsetu1, 1.0f + offsetv1} }
			};

			hres = pdev->SetVertexDeclaration( m_tlvdecl );
			_ASSERT( hres == D3D_OK );

			hres = pdev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
							, 2, Vertex1, sizeof( EFFECTTLV ) );
			_ASSERT( hres == D3D_OK );

			hres = g_pPostEffect->EndPass();
			_ASSERT( hres == D3D_OK );

		}
////////////
		if( ptex2 ){

			pdev->SetRenderState( D3DRS_BLENDOP, blendop );
			g_renderstate[ D3DRS_BLENDOP ] = blendop;

			pdev->SetRenderState( D3DRS_SRCBLEND, srcblend );
			g_renderstate[ D3DRS_SRCBLEND ] = srcblend;

			pdev->SetRenderState( D3DRS_DESTBLEND, destblend );
			g_renderstate[ D3DRS_DESTBLEND ] = destblend;

			float fw2 = (float)width2;
			float fh2 = (float)height2;
			hres = g_pPostEffect->SetValue( g_hpeWidth0, &fw2, sizeof( float ) );
			if( hres != D3D_OK ){
				DbgOut( "E3DBlendRTTexture :width0 SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			hres = g_pPostEffect->SetValue( g_hpeHeight0, &fh2, sizeof( float ) );
			if( hres != D3D_OK ){
				DbgOut( "E3DBlendRTTexture :height0 SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			hres = g_pPostEffect->SetTexture( g_hpeTexture0, ptex2 );
			if( hres != D3D_OK ){
				DbgOut( "E3DBlendRTTexture :texture0 SetTexture error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			int passno2;
			float offsetu2, offsetv2;
			if( texf2 == D3DTEXF_POINT ){
				passno2 = 0;
				offsetu2 = 0.5f / screenw;
				offsetv2 = 0.5f / screenh;
			}else{
				passno2 = 1;
				offsetu2 = 0.0f;
				offsetv2 = 0.0f;
			}
			hres = g_pPostEffect->BeginPass( passno2 );
			_ASSERT( hres == D3D_OK );


			EFFECTTLV Vertex2[4] = {
				//   x    y     z      w     u0 v0
				{{-1.0f, +1.0f, 0.2f, 1.0f}, {col2.r, col2.g, col2.b, col2.a}, {0.0f + offsetu2, 0.0f + offsetv2} },
				{{+1.0f, +1.0f, 0.2f, 1.0f}, {col2.r, col2.g, col2.b, col2.a}, {1.0f + offsetu2, 0.0f + offsetv2} },
				{{+1.0f, -1.0f, 0.2f, 1.0f}, {col2.r, col2.g, col2.b, col2.a}, {1.0f + offsetu2, 1.0f + offsetv2} },
				{{-1.0f, -1.0f, 0.2f, 1.0f}, {col2.r, col2.g, col2.b, col2.a}, {0.0f + offsetu2, 1.0f + offsetv2} }
			};

			
			hres = pdev->SetVertexDeclaration( m_tlvdecl );
			_ASSERT( hres == D3D_OK );

			hres = pdev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
							, 2, Vertex2, sizeof( EFFECTTLV ) );
			_ASSERT( hres == D3D_OK );

			hres = g_pPostEffect->EndPass();
			_ASSERT( hres == D3D_OK );
		}
////////////

		hres = g_pPostEffect->End();
		_ASSERT( hres == D3D_OK );
	}else{
		_ASSERT( 0 );
	}

    pdev->EndScene();

	return 0;
}


int CMyD3DApplication::BlurTextureGauss( LPDIRECT3DSURFACE9 finalsurf, D3DVIEWPORT9* finalviewport,
	LPDIRECT3DSURFACE9 worksurf, LPDIRECT3DTEXTURE9 worktex, D3DVIEWPORT9* workviewport,
	LPDIRECT3DTEXTURE9 srctex, D3DVIEWPORT9* srcviewport, float dispersion )
{
	int ret;
	HRESULT hres;

	LPDIRECT3DDEVICE9 pdev = m_pd3dDevice;
	if( !pdev ){
		_ASSERT( 0 );
		return 1;
	}

/////////////
// １パス目
	int skipbeginscene = 0;
	int needz = 0;
	ret = RTBeginScene( pdev, worksurf, 0, workviewport, skipbeginscene, needz );
	if( ret ){
		DbgOut( "BlurTextureGauss : RTBeginScene workscid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int srcwidth, srcheight;
	srcwidth = srcviewport->Width;
	srcheight = srcviewport->Height;

	if( g_pPostEffect != NULL )
	{
		ret = SetRTRenderState( pdev, 0 );
		_ASSERT( !ret );

		//-------------------------------------------------
		// シェーダの設定
		//-------------------------------------------------
		hres = g_pPostEffect->SetTechnique( g_hPostEffectGauss );
		_ASSERT( hres == D3D_OK );

		ret = SetGaussWeight( dispersion );
		_ASSERT( !ret );


		float fw = (float)srcwidth;
		float fh = (float)srcheight;
		hres = g_pPostEffect->SetValue( g_hpeWidth0, &fw, sizeof( float ) );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :width0 SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		hres = g_pPostEffect->SetValue( g_hpeHeight0, &fh, sizeof( float ) );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :height0 SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		hres = g_pPostEffect->SetTexture( g_hpeTexture0, srctex );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :texture0 SetTexture error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		D3DXVECTOR2 offsetX;
		offsetX.x = 16.0f / fw;
		offsetX.y = 0.0f / fh;
		hres = g_pPostEffect->SetValue( g_hpeOffsetX, &offsetX, sizeof( float ) * 2 );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :offsetX SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		float colmult = 1.0f;
		hres = g_pPostEffect->SetValue( g_hpeColMult, &colmult, sizeof( float ) );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :ColMult SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		

		hres = g_pPostEffect->Begin( NULL, 0 );
		_ASSERT( hres == D3D_OK );
		hres = g_pPostEffect->BeginPass( 0 );
		_ASSERT( hres == D3D_OK );

		TVERTEX Vertex1[4] = {
			//   x    y     z      w     u0 v0
			{-1.0f, +1.0f, 0.1f,  1.0f, 0.0f, 0.0f},
			{+1.0f, +1.0f, 0.1f,  1.0f, 1.0f, 0.0f},
			{+1.0f, -1.0f, 0.1f,  1.0f, 1.0f, 1.0f},
			{-1.0f, -1.0f, 0.1f,  1.0f, 0.0f, 1.0f}
		};

		hres = pdev->SetVertexDeclaration( m_effectdecl );
		_ASSERT( hres == D3D_OK );

		hres = pdev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
						, 2, Vertex1, sizeof( TVERTEX ) );
		_ASSERT( hres == D3D_OK );

		hres = g_pPostEffect->EndPass();
		_ASSERT( hres == D3D_OK );

/////////////////////
// ２パス目
		skipbeginscene = 1;
		needz = 0;
		ret = RTBeginScene( pdev, finalsurf, 0, finalviewport, skipbeginscene, needz );
		if( ret ){
			DbgOut( "BlurTextureGauss : RTBeginScene finalscid error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		int workwidth, workheight;
		workwidth = workviewport->Width;
		workheight = workviewport->Height;

		float fw2 = (float)workwidth;
		float fh2 = (float)workheight;
		hres = g_pPostEffect->SetValue( g_hpeWidth0, &fw2, sizeof( float ) );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :width0 2 SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		hres = g_pPostEffect->SetValue( g_hpeHeight0, &fh2, sizeof( float ) );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :height0 2 SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		hres = g_pPostEffect->SetTexture( g_hpeTexture0, worktex );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :texture0 2 SetTexture error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		D3DXVECTOR2 offsetY;
		offsetY.x = 0.0f / fw;
		offsetY.y = 16.0f / fh;
		hres = g_pPostEffect->SetValue( g_hpeOffsetY, &offsetY, sizeof( float ) * 2 );
		if( hres != D3D_OK ){
			DbgOut( "E3DBlurTextureGauss :offsetY SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		hres = g_pPostEffect->BeginPass( 1 );
		_ASSERT( hres == D3D_OK );

		hres = pdev->SetVertexDeclaration( m_effectdecl );
		_ASSERT( hres == D3D_OK );

		hres = pdev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
						, 2, Vertex1, sizeof( TVERTEX ) );
		_ASSERT( hres == D3D_OK );

		hres = g_pPostEffect->EndPass();
		_ASSERT( hres == D3D_OK );

/////////////


		hres = g_pPostEffect->End();
		_ASSERT( hres == D3D_OK );
	}else{
		_ASSERT( 0 );
	}

    pdev->EndScene();

	return 0;
}


int CMyD3DApplication::SetGaussWeight( float dispersion )
{
    DWORD i;
	float tbl[8];

    float total=0.0f;
    for( i=0; i<8; i++ ){
        float pos = 1.0f + 2.0f * (float)i;
        tbl[i] = expf( -0.5f * (float)( pos * pos ) / dispersion );
        total += 2.0f * tbl[i];
    }
    // 規格化
	for( i=0; i<8; i++ ){
		tbl[i] /= total;
	}

	if( g_pPostEffect){
		HRESULT hres;
		hres = g_pPostEffect->SetValue( g_hpeWeight, tbl, sizeof( float ) * 8 );
		if( hres != D3D_OK ){
			DbgOut( "SetGaussWeight : posteffect setvalue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		_ASSERT( 0 );
		return 1;
	}

	return 0;

}

int CMyD3DApplication::CheckDestroyedNum()
{
	
	int ret;
	int num = 0;
	if( m_shandler ){
		ret = m_shandler->GetDestroyedNum( &num );
		_ASSERT( !ret );
	}

	char mes[256];
	sprintf_s( mes, 256, "Destroyedの数は%d個です。", num );

	::MessageBox( m_hWnd, mes, "チェック", MB_OK );

	return 0;
}

int CMyD3DApplication::SetSymmJointPos()
{
	int ret;

	if( !m_shandler || !m_thandler || !m_mhandler ){
		return 0;
	}
	if( m_shandler->s2shd_leng <= 1 ){
		return 0;
	}

	if( g_inidata->opt_modlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( g_loadingflag ){
		ret = IDOK;
	}

	if( ret != IDOK ){
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	
	CSymJPosDlg dlg;
	ret = (int)dlg.DoModal();
	if( ret != IDOK ){
		return 0;
	}

	m_ExistData = FALSE;


	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );
	ret = DestroyMotion( 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = g_motdlg->RemakeMOEAH();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


	int* createmark;
	createmark = (int*)malloc( sizeof( int ) * m_shandler->s2shd_leng );
	if( !createmark ){
		DbgOut( "viewer : CreateSymBone : createmark alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( createmark, sizeof( int ) * m_shandler->s2shd_leng );

	ret = SetCreateMark2SymBone( dlg.m_createtype, createmark );
	if( ret ){
		DbgOut( "viewer : CreateSymBone : SetCreateMark2SymBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int seri;
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		CShdElem* curselem = (*m_shandler)( seri );
		int curmark = *( createmark + seri );
		if( (curmark != 0) && curselem->IsJoint() ){

			char symname[256];
			ZeroMemory( symname, sizeof( char ) * 256 );
			CVec3f symloc( 0.0f, 0.0f, 0.0f );
			int setflag = 0;

			ret = SOGetSymXName( m_thandler, m_shandler, seri, &setflag, symname, 256, &symloc );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}
			if( setflag != 0 ){

				int jointseri = -1;
				ret = m_thandler->GetBoneNoByName( symname, &jointseri, m_shandler, 0 );
				if( (ret == 0) && (jointseri > 0) ){
					D3DXVECTOR3 symlocx;
					symlocx.x = symloc.x;
					symlocx.y = symloc.y;
					symlocx.z = symloc.z;

					ret = m_shandler->ChangeJointInitialPos( jointseri, symlocx );
					if( ret ){
						_ASSERT( 0 );
						return 1;
					}
				}
			}
		}
	}

	if( createmark ){
		free( createmark );
	}

	if( g_inidata->opt_infdlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "オブジェクトの位置が調整されました。影響度の再計算をしますか？？", "再計算の確認", MB_OKCANCEL );
	}else if( g_inidata->opt_infdlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( ret == IDOK ){
		SendMessage( m_hWnd, WM_COMMAND, 6003, 0 );
	}

	m_ExistData = TRUE;

	return 0;
}
int CMyD3DApplication::CreateSymBone()
{
	int ret;

	if( !m_shandler || !m_thandler || !m_mhandler ){
		return 0;
	}
	if( m_shandler->s2shd_leng <= 1 ){
		return 0;
	}

	if( g_inidata->opt_modlg == CHKDLG_NES ){
		ret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		ret = IDOK;
	}else{
		ret = IDCANCEL;
	}
	if( g_loadingflag ){
		ret = IDOK;
	}

	if( ret != IDOK ){
		return 0;
	}
	if( m_graphwnd ){
		m_graphwnd->CloseSpDlg();
	}

	ret = g_motdlg->SelectMotParamTab();
	_ASSERT( !ret );

	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 0, 0 );

	CSymBoneDlg dlg;
	ret = (int)dlg.DoModal();

	::SendMessage( m_hWnd, WM_USER_ENABLE_MENU, 1, 0 );

	if( ret != IDOK )
		return 0;

	int* delmark;
	delmark = (int*)malloc( sizeof( int ) * m_shandler->s2shd_leng );
	if( !delmark ){
		DbgOut( "viewer : CreateSymBone : delmark alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( delmark, sizeof( int ) * m_shandler->s2shd_leng );

	int* createmark;
	createmark = (int*)malloc( sizeof( int ) * m_shandler->s2shd_leng );
	if( !createmark ){
		DbgOut( "viewer : CreateSymBone : createmark alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( createmark, sizeof( int ) * m_shandler->s2shd_leng );

////////////
	m_ExistData = FALSE;

	InvalidateHandler( 1, 0 );

	ret = m_shandler->DestroyBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : CreateSymBone : DestroyBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( m_mcdlg->m_firstmake ){
		ret = m_mcdlg->DestroyMCElem( -1 );
		if( ret ){
			DbgOut( "viewer : CreateSymBone : mcdlg DestroyMCElem all error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	ret = m_mhandler->DestroyMotionObj();
	if( ret ){
		DbgOut( "viewer : CreateSymBone : DestroyMotionObj error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = g_motdlg->RemakeMOEAH();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

///////////////


	ret = SetDelMark2SymBone( dlg.m_createtype, delmark );
	if( ret ){
		DbgOut( "viewer : CreateSymBone : SetMark2SymBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//int chk;
//for( chk = 0; chk < m_thandler->s2e_leng; chk++ ){
//	CTreeElem2* chkt;
//	chkt = (*m_thandler)( chk );
//	int chkmark;
//	chkmark = *( delmark + chk );
//	if( chkmark ){
//		DbgOut( "check !!!: delmark : name %s, delmark %d\r\n", chkt->name, chkmark );
//	}
//}
//DbgOut( "\r\n" );


	ret = DeleteBoneByMark( delmark );
	if( ret ){
		DbgOut( "viewer : CreateSymBone : DeleteBoneByMark error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = SetCreateMark2SymBone( dlg.m_createtype, createmark );
	if( ret ){
		DbgOut( "viewer : CreateSymBone : SetCreateMark2SymBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//for( chk = 0; chk < m_thandler->s2e_leng; chk++ ){
//	CTreeElem2* chkt;
//	chkt = (*m_thandler)( chk );
//	int chkmark;
//	chkmark = *( createmark + chk );
//	if( chkmark ){
//		DbgOut( "check !!!: createmark : name %s, createmark %d\r\n", chkt->name, chkmark );
//	}
//}


	int errorcnt = 0;
	CreateBoneByMarkReq( 0, createmark, (*m_shandler)( 0 ), &errorcnt );
	if( errorcnt > 0 ){
		DbgOut( "viewer : CreateSymBone : CreateBoneByMarkReq error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
////////////
	if( delmark ){
		free( delmark );
	}
	if( createmark ){
		free( createmark );
	}

/////////////
	ret = m_shandler->CreateBoneInfo( m_mhandler );
	if( ret ){
		DbgOut( "viewer : CreateSymBone : CreateBoneInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = RestoreHandler( 1 );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = m_shandler->CalcInfElem( -1, 1 );
	if( ret ){
		DbgOut( "viewer : CreateSymBone : CalcInfElem error !!!\n" );
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
				DbgOut( "viewer : CreateSymBone : mcdlg InitMC error !!!\n" );
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

int CMyD3DApplication::SetDelMark2SymBone( int type, int* markptr )
{
	int ret;
	int seri;

	//削除するボーンが見つかったらmarkに１をセット。
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		int findflag = 0;
		if( type == CREATE_MINUS ){
			ret = SOIsSymXM( m_thandler, seri, &findflag );
			_ASSERT( !ret );
		}else if( type == CREATE_PLUS ){
			ret = SOIsSymXP( m_thandler, seri, &findflag );
			_ASSERT( !ret );
		}else{
			_ASSERT( 0 );
			return 1;
		}

		if( findflag ){
			*( markptr + seri ) = 1;
		}else{
			*( markptr + seri ) = 0;
		}
	}

	//markがついていない要素について
	//親をさかのぼって削除ボーンが見つかった場合はmarkに２をセット。
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		int curmark;
		curmark = *( markptr + seri );

		if( curmark == 0 ){
			CShdElem* curselem;
			curselem = (*m_shandler)( seri );
			_ASSERT( curselem );

			CShdElem* parselem = 0;
			int findflag2 = 0;
			while( curselem ){
				parselem = m_shandler->FindUpperJoint( curselem );
				if( parselem ){
					int parmark;
					parmark = *( markptr + parselem->serialno );
					if( parmark != 0 ){
						findflag2 = 1;
						break;
					}
				}
				curselem = parselem;
			}

			if( findflag2 != 0 ){
				*( markptr + seri ) = 2;
			}
		}
	}


	return 0;
}

int CMyD3DApplication::DeleteBoneByMark( int* delmark )
{
	int ret;
	int seri;
	int restoreflag = 0;

	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		int curmark;
		curmark = *( delmark + seri );
		if( curmark != 0 ){
			ret = DelJoint( seri, restoreflag );
			if( ret ){
				DbgOut( "viewer : DeleteBoneByMark : DelJoint error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}

int CMyD3DApplication::SetCreateMark2SymBone( int type, int* markptr )
{
	int ret;
	int seri;

	//作成するボーンが見つかったらmarkに１をセット。
	for( seri = 0; seri < m_shandler->s2shd_leng; seri++ ){
		int findflag = 0;
		if( type == CREATE_MINUS ){
			ret = SOIsSymXP( m_thandler, seri, &findflag );
			_ASSERT( !ret );
		}else if( type == CREATE_PLUS ){
			ret = SOIsSymXM( m_thandler, seri, &findflag );
			_ASSERT( !ret );
		}else{
			_ASSERT( 0 );
			return 1;
		}

		if( findflag ){
			*( markptr + seri ) = 1;
		}else{
			*( markptr + seri ) = 0;
		}
	}
	return 0;
}


void CMyD3DApplication::CreateBoneByMarkReq( int seri, int* markptr, CShdElem* parelem, int* errorptr )
{
	if( *errorptr != 0 ){
		return;
	}

	CShdElem* curselem;
	curselem = (*m_shandler)( seri );
	int curmark;
	curmark = *( markptr + seri );

	CShdElem* newselem = 0;
	CShdElem* nextparelem = 0;

	int ret;
	if( (curmark != 0) && curselem->IsJoint() ){
		if( !parelem ){
			_ASSERT( 0 );
			(*errorptr)++;
			return;
		}

		char symname[256];
		ZeroMemory( symname, sizeof( char ) * 256 );
		CVec3f symloc( 0.0f, 0.0f, 0.0f );
		int setflag = 0;

		ret = SOGetSymXName( m_thandler, m_shandler, seri, &setflag, symname, 256, &symloc );
		if( ret ){
			_ASSERT( 0 );
			(*errorptr)++;
			return;
		}

		if( setflag == 0 ){
			CTreeElem2* curtelem;
			curtelem = (*m_thandler)( seri );
			_ASSERT( curtelem );

			strcpy_s( symname, 256, curtelem->name );
			strcat_s( symname, 256, "_2" );
			CPart* curpartptr;
			curpartptr = curselem->part;
			if( !curpartptr ){
				_ASSERT( 0 );
				(*errorptr)++;
				return;
			}
			symloc.x = -curpartptr->jointloc.x;
			symloc.y = curpartptr->jointloc.y;
			symloc.z = curpartptr->jointloc.z;
		}

		D3DXVECTOR3 symlocx;
		symlocx.x = symloc.x;
		symlocx.y = symloc.y;
		symlocx.z = symloc.z;

		int newseri = -1;
		int restoreflag = 0;
		ret = CreateBone( symname, symlocx, parelem, &newseri, restoreflag );
		if( ret || (newseri < 0) ){
			_ASSERT( 0 );
			(*errorptr)++;
			return;
		}

		newselem = (*m_shandler)( newseri );
		_ASSERT( newselem );
	}

	if( curselem->child ){
		if( newselem ){
			nextparelem = newselem;
		}else{
			nextparelem = curselem;
		}
		CreateBoneByMarkReq( curselem->child->serialno, markptr, nextparelem, errorptr );
	}

	if( curselem->brother ){
		nextparelem = parelem;
		CreateBoneByMarkReq( curselem->brother->serialno, markptr, nextparelem, errorptr );
	}

	return;
}

int CMyD3DApplication::GetCameraElem( CAMERAELEM* dstce )
{
/***
	dstce->looktype = CAML_NORMAL;
	dstce->target = m_targetpos;
	dstce->pos = m_vEyePt;
	D3DXVECTOR3 diff = m_targetpos - m_vEyePt;
	dstce->dist = D3DXVec3Length( &diff );
	dstce->up = m_camup;
	
	dstce->hsid = -1;
	dstce->boneno = -1;
	
	dstce->nearz = m_proj_near;
	dstce->farz = m_proj_far;
	//dstce->fov = (float)m_parsdlg->parsfov;
	dstce->fov = m_fov;
***/

	*dstce = m_camera;

	return 0;
}

int CMyD3DApplication::SetCameraElem( CAMERAELEM srcce )
{
	/***
	m_targetpos = srcce.target;
	m_eye_y = srcce.target.y;
	m_vEyePt = srcce.pos;
	m_camup = srcce.up;

	m_proj_near = srcce.nearz;
	m_proj_far = srcce.farz;
	//m_parsdlg->parsfov = srcce.fov;
	m_fov = srcce.fov;
	***/

	m_camera = srcce;

	return 0;
}

int CMyD3DApplication::GetKeyboardCnt()
{
	GetKeyboardState( g_KeyBuffer );

	int cno;
	for( cno = 0; cno < 256; cno++ ){
		if( g_KeyBuffer[cno] & 0x80 ){
			(g_keyboardcnt[cno])++;
		}else{
			g_keyboardcnt[cno] = 0;
		}
	}

	return 0;
}
int CMyD3DApplication::ResetKeyboardCnt()
{
	ZeroMemory( g_keyboardcnt, sizeof( int ) * 256 );

	ZeroMemory( g_validkey, sizeof( int ) * 256 );
	g_validkey[VK_LEFT] = 1;
	g_validkey[VK_RIGHT] = 1;
	g_validkey[VK_UP] = 1;
	g_validkey[VK_DOWN] = 1;

	g_validkey['1'] = 1;
	g_validkey['2'] = 1;
	g_validkey['3'] = 1;
	g_validkey['4'] = 1;
	g_validkey['5'] = 1;
	g_validkey['6'] = 1;
	g_validkey['7'] = 1;
	g_validkey['8'] = 1;
	g_validkey['9'] = 1;
	g_validkey['0'] = 1;

	g_validkey['A'] = 1;
	g_validkey['B'] = 1;
	g_validkey['C'] = 1;
	g_validkey['D'] = 1;
	g_validkey['E'] = 1;
	g_validkey['F'] = 1;
	g_validkey['G'] = 1;
	g_validkey['H'] = 1;
	g_validkey['I'] = 1;
	g_validkey['J'] = 1;
	g_validkey['K'] = 1;
	g_validkey['L'] = 1;
	g_validkey['M'] = 1;
	g_validkey['N'] = 1;
	g_validkey['O'] = 1;
	g_validkey['P'] = 1;
	g_validkey['Q'] = 1;
	g_validkey['R'] = 1;
	g_validkey['S'] = 1;
	g_validkey['T'] = 1;
	g_validkey['U'] = 1;
	g_validkey['V'] = 1;
	g_validkey['W'] = 1;
	g_validkey['X'] = 1;
	g_validkey['Y'] = 1;
	g_validkey['Z'] = 1;

	return 0;
}

int CMyD3DApplication::SetSoundBank()
{
	CSoundBankDlg dlg( m_HS, m_soundbank );

	int ret;
	ret = (int)dlg.DoModal();


	return 0;
}

int CMyD3DApplication::SaveSndSetFile( char* srcfilename, char* ssfdir )
{
	if( !m_HS || !m_soundbank ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	CSndSetFile ssfile( m_HS, m_soundbank );
	ret = ssfile.WriteSndSetFile( srcfilename, ssfdir );
	if( ret ){
		DbgOut( "viewer : SaveSndSetFile : ssf WriteSndSetFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
int CMyD3DApplication::SaveSndAnimFile( char* srcfilename )
{
	int ret;
	ret = g_motdlg->WriteSndAnimFile( srcfilename );
	if( ret ){
		DbgOut( "viewer : SaveSndAnimFile : motdlg WriteSndAnimFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}
int CMyD3DApplication::LoadSndSetFile( char* srcfilename )
{
	if( !m_HS || !m_soundbank ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;

	
	if( m_soundbank ){
		delete m_soundbank;
		m_soundbank = 0;

		m_soundbank = new CSoundBank();
		if( !m_soundbank ){
			DbgOut( "viewer : LoadSndSetFile : new SoundBank error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	ret = g_motdlg->m_snddlg->RemakeSndAH();
	if( ret ){
		DbgOut( "viewer : LoadSndSetFile : snddlg RemakeSndAH error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	CSndSetFile ssfile( m_HS, m_soundbank );
	ret = ssfile.LoadSndSetFile( srcfilename );
	if( ret ){
		DbgOut( "viewer : LoadSndSetFile : ssf LoadSndSetFile error !!!\n" );
		_ASSERT( 0 );

		if( m_soundbank ){
			delete m_soundbank;
			m_soundbank = 0;

			m_soundbank = new CSoundBank();
			if( !m_soundbank ){
				_ASSERT( 0 );
			}
		}
		g_motdlg->m_snddlg->RemakeSndAH();

		return 1;
	}

	return 0;
}
int CMyD3DApplication::LoadSndAnimFile( char* srcfilename, int dlgwarnflag )
{
	int ret;
	ret = g_motdlg->LoadSndAnimFile( srcfilename, dlgwarnflag );
	if( ret ){
		DbgOut( "viewer : LoadSndAnimFile : motdlg LoadSndAnimFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}

int CMyD3DApplication::CreateNumTexture()
{
	if( !g_texbnk ){
		g_texbnk = new CTexBank( g_miplevels, g_mipfilter );
		if( !g_texbnk ){
			DbgOut( "viewer : CreateNumTexture : new CTexBank error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	int ret;
	//MEDIADIR
	DWORD dirleng;
	char dirname[MAX_PATH];
	ZeroMemory( dirname, MAX_PATH );
	dirleng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)dirname, MAX_PATH );
	if( (dirleng == 0) || (dirleng >= MAX_PATH) ){
		DbgOut( "viewer : CreateNumTexture : MEDIADIR too long error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int texno;
	for( texno = 0; texno <= 9; texno++ ){
		char tname[MAX_PATH];
		sprintf_s( tname, MAX_PATH, "%d.png", texno );
		ret = g_texbnk->AddName( dirname, tname );
		if( ret < 0 ){
			_ASSERT( 0 );
			return 1;
		}
	}

	int j;
	for( j = 0; j < g_texbnk->namenum; j++ ){
		ret = g_texbnk->CreateTexData( j, m_pd3dDevice );
		if( ret && (ret != -2) ){
			int ret2;
			ret2 = g_texbnk->SetTexData( j, NULL );
			if( ret2 ){
				_ASSERT( 0 );
				return 1;
			}
		}
	}

	return 0;
}


int CMyD3DApplication::CreateCameraTexture()
{
	if( !g_texbnk ){
		g_texbnk = new CTexBank( g_miplevels, g_mipfilter );
		if( !g_texbnk ){
			DbgOut( "viewer : CreateCameraTexture : new CTexBank error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
	int ret;
	//MEDIADIR
	DWORD dirleng;
	char dirname[MAX_PATH];
	ZeroMemory( dirname, MAX_PATH );
	dirleng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)dirname, MAX_PATH );
	if( (dirleng == 0) || (dirleng >= MAX_PATH) ){
		DbgOut( "viewer : CreateCameraTexture : MEDIADIR too long error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ret = g_texbnk->AddName( dirname, s_cam_kaku, 0 );
	if( ret < 0 ){
		_ASSERT( 0 );
		return 1;
	}
	ret = g_texbnk->AddName( dirname, s_cam_i, 0 );
	if( ret < 0 ){
		_ASSERT( 0 );
		return 1;
	}
	ret = g_texbnk->AddName( dirname, s_cam_kai, 0 );
	if( ret < 0 ){
		_ASSERT( 0 );
		return 1;
	}

/***
	//CreateNumTextureで実行！！！！
	int j;
	for( j = 0; j < g_texbnk->namenum; j++ ){
		ret = g_texbnk->CreateTexData( j, m_pd3dDevice );
		if( ret && (ret != -2) ){
			int ret2;
			ret2 = g_texbnk->SetTexData( j, NULL );
			if( ret2 ){
				_ASSERT( 0 );
				return 1;
			}
		}
	}
***/
	return 0;
}

int CMyD3DApplication::RenderCameraSprite()
{
	HRESULT hr;		
	hr = m_d3dxsprite->Begin( D3DXSPRITE_ALPHABLEND );
	if( hr != D3D_OK ){
		_ASSERT( 0 );
		return 1;
	}
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF, 0 );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );

	LPDIRECT3DTEXTURE9 tex_kaku, tex_i, tex_kai;
	tex_kaku = g_texbnk->GetTexData( s_cam_kaku, 0 );
	_ASSERT( tex_kaku );
	tex_i = g_texbnk->GetTexData( s_cam_i, 0 );
	_ASSERT( tex_i );
	tex_kai = g_texbnk->GetTexData( s_cam_kai, 0 );
	_ASSERT( tex_kai );

	//int ret;
	D3DXVECTOR3	tra_kaku, tra_i, tra_kai;
	D3DXVECTOR2	scl;
	D3DCOLOR col;
	col = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	scl.x = 1.0f;
	scl.y = 1.0f;

	tra_kaku.x = (float)m_camkaku_rect.left;
	tra_kaku.y = (float)m_camkaku_rect.top;
	tra_i.x = (float)m_cami_rect.left;
	tra_i.y = (float)m_cami_rect.top;
	tra_kai.x = (float)m_camkai_rect.left;
	tra_kai.y = (float)m_camkai_rect.top;

	D3DXMATRIX posmat;
	D3DXMatrixIdentity( &posmat );

	posmat._41 = tra_kaku.x;
	posmat._42 = tra_kaku.y;
	m_d3dxsprite->SetTransform( &posmat );
	hr = m_d3dxsprite->Draw( tex_kaku, NULL, NULL, NULL, col );
	_ASSERT( hr == D3D_OK );
	

	posmat._41 = tra_i.x;
	posmat._42 = tra_i.y;
	m_d3dxsprite->SetTransform( &posmat );
	hr = m_d3dxsprite->Draw( tex_i, NULL, NULL, NULL, col );
	_ASSERT( hr == D3D_OK );


	posmat._41 = tra_kai.x;
	posmat._42 = tra_kai.y;
	m_d3dxsprite->SetTransform( &posmat );
	hr = m_d3dxsprite->Draw( tex_kai, NULL, NULL, NULL, col );
	_ASSERT( hr == D3D_OK );

	hr = m_d3dxsprite->End();
	if( hr != D3D_OK ){
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}

int CMyD3DApplication::CheckCamOpe( POINT srcpt )
{
	if( (srcpt.x >= m_camkaku_rect.left) && (srcpt.x <= m_camkaku_rect.right) && 
		(srcpt.y >= m_camkaku_rect.top) && (srcpt.y <= m_camkaku_rect.bottom) ){
		return CAMOPE_KAKU;
	}

	if( (srcpt.x >= m_cami_rect.left) && (srcpt.x <= m_cami_rect.right) && 
		(srcpt.y >= m_cami_rect.top) && (srcpt.y <= m_cami_rect.bottom) ){
		return CAMOPE_I;
	}

	if( (srcpt.x >= m_camkai_rect.left) && (srcpt.x <= m_camkai_rect.right) && 
		(srcpt.y >= m_camkai_rect.top) && (srcpt.y <= m_camkai_rect.bottom) ){
		return CAMOPE_KAI;
	}

	return CAMOPE_NONE;
}

int CMyD3DApplication::CamOpe( POINT ptCursor )
{
	if( m_camope == CAMOPE_KAKU ){

		float diffx, diffy;
		RECT clirect;
		float vert;

		diffx = (float)ptCursor.x - (float)m_lpos.x;
		diffy = (float)ptCursor.y - (float)m_lpos.y;
		::GetClientRect( m_3dwnd.m_hWnd, &clirect );
		//vert = 2000.0f * (diffx + diffy) / (float)(clirect.bottom - clirect.top);
		vert = (diffx + diffy) * g_inidata->camdlg_move;

		OnCameraDist( vert );

		m_lpos = ptCursor;

	}else if( m_camope == CAMOPE_KAI ){
		OnCameraRot( ptCursor, m_lpos );
		m_lpos = ptCursor;

	}else if( m_camope == CAMOPE_I ){
		OnCameraMove( ptCursor, m_lpos );
		m_lpos = ptCursor;
	}

	return 0;
}

int CMyD3DApplication::DelTreeElem( int srcdel, int dlgflag )
{
	int ret;
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}

	int delseri;
	if( srcdel <= 0 ){
		delseri = m_sigeditwnd->m_selectedno;
	}else{
		delseri = srcdel;
	}

	if( delseri <= 0 ){
		return 0;
	}

	CShdElem* delelem;
	delelem = (*m_shandler)( delseri );

	if( delelem->IsJoint() ){
		if( m_sigeditwnd ){
			ret = m_sigeditwnd->OnMenuDelJoint( delseri, dlgflag );
			if( ret ){
				DbgOut( "viewer : DelTreeElem : sigeditwnd OnMenuDelJoint error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}else if( delelem->type == SHDMORPH ){
		ret = DelMorph( delseri, dlgflag );
		if( ret ){
			DbgOut( "viewer : DelTreeElem : DelMorph error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else if( (delelem->type == SHDPOLYMESH) || (delelem->type == SHDPOLYMESH2) ){
		ret = DelDispObj( delseri, dlgflag );
		if( ret ){
			DbgOut( "viewer : DelTreeElem : DelDispObj error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else if( (delelem->type == SHDPART) && (delelem->serialno != 1) ){
		ret = DelPart( delseri, dlgflag );
		if( ret ){
			DbgOut( "viewer : DelTreeElem : DelPart error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

int CMyD3DApplication::DelTree()
{
	if( !m_shandler || (m_shandler->s2shd_leng <= 1) ){
		//データが無い時は、何もしない。
		return 0;
	}

	int delseri;
	delseri = m_sigeditwnd->m_selectedno;
	if( delseri <= 0 ){
		return 0;
	}

	int dlgret;
	if( g_inidata->opt_modlg == CHKDLG_NES ){
		dlgret = (int)::MessageBox( m_hWnd, "モーションを全て削除します。よろしいですか？", "確認", MB_OKCANCEL );
	}else if( g_inidata->opt_modlg == CHKDLG_YES ){
		dlgret = IDOK;
	}else{
		dlgret = IDCANCEL;
	}
	if( g_loadingflag ){
		dlgret = IDOK;
	}

	if( dlgret != IDOK ){
		return 0;
	}

	DelTreeReq( delseri, 0 );

	return 0;
}

void CMyD3DApplication::DelTreeReq( int delseri, int broflag )
{
	int ret;
	if( delseri <= 0 ){
		return;
	}

	CShdElem* delelem;
	CShdElem* chilelem;
	CShdElem* broelem;
	delelem = (*m_shandler)( delseri );
	_ASSERT( delelem );
	chilelem = delelem->child;
	broelem = delelem->brother;

	ret = DelTreeElem( delseri, 0 );
	if( ret ){
		DbgOut( "viewer : DelTreeReq : DelTreeElem error !!!\n" );
		_ASSERT( 0 );
		return;
	}

	if( chilelem ){
		DelTreeReq( chilelem->serialno, 1 );
	}
	if( broflag && broelem ){
		DelTreeReq( broelem->serialno, 1 );
	}

}

int CMyD3DApplication::GetRelatedPath( char* basepath, char* srcpath, char* dstpath, int dstleng )
{
	//int ret;

	int ch = '\\';
	char* lastenptr = 0;
	lastenptr = strrchr( basepath, ch );
	if( !lastenptr ){
		DbgOut( "viewer : GetRelatedPath : basepath not full path error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int baseleng;
	baseleng = (int)( lastenptr - basepath );
	int cmp = 1;
	cmp = strncmp( basepath, srcpath, baseleng );
	if( cmp != 0 ){
		DbgOut( "viewer : GetRelatedPath : srcpath not include basepath error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	char* dstfirst = 0;
	dstfirst = srcpath + baseleng + 1;
	if( !dstfirst ){
		DbgOut( "viewer : GetRelatedPath : srcpath not include filename error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int srcleng;
	srcleng = (int)strlen( dstfirst );

	if( srcleng >= dstleng ){
		DbgOut( "viewer : GetRelatedPath : dstleng too short error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	strcpy_s( dstpath, dstleng, dstfirst );

	return 0;
}

int CMyD3DApplication::SaveBldFile( int overwflag )
{
	if( (m_ExistData == 0) || !m_thandler || !m_shandler || !m_mhandler || !m_mcdlg->m_mch ){
		return 0;
	}

	int ret;
	CMAFile* mafile;
	mafile = new CMAFile();
	if( !mafile ){
		DbgOut( "viewer : SaveBldFile : mafile alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	mafile->m_mh = m_mhandler;
	mafile->m_mch = m_mcdlg->m_mch;
	
	ret = mafile->CheckSameMotionName();
	if( ret ){
		::MessageBox( m_hWnd, "moa内に同じモーション名が存在します。\n名前を変えてから再試行してください。", "エラー", MB_OK );
		delete mafile;
		return 0;
	}
	ret = mafile->CheckIdlingMotion( m_mcdlg->m_mch );
	if( ret ){
		::MessageBox( m_hWnd, "moaに１つだけアイドリングモーションを設定してから再試行してください。", "エラー", MB_OK );
		delete mafile;
		return 0;
	}
//	ret = mafile->CheckCommonid( m_hWnd );
//	if( ret ){
//		DbgOut( "mafile : SaveMAFile : CheckCommonid error !!!\n" );
//		_ASSERT( 0 );
//		return 1;
//	}
	
//////////

	m_wblddlg->InitMult();//書き出し時のmult初期値は常に1.0倍。こうしないと書き出すたびにデータが大きくなる。

	if( overwflag == 0 ){
		int dlgret;
		dlgret = (int)m_wblddlg->DoModal();
		if( dlgret != IDOK ){
			delete mafile;
			return 0;
		}
	}
///////////	
	if( !m_wblddlg->m_signame[0] ){
		_ASSERT( 0 );
		delete mafile;
		return 1;		
	}
	ret = SaveSigFile( m_wblddlg->m_signame );
	if( ret ){
		DbgOut( "viewer : SaveBldFile : SaveSigFile error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}
///////////
	if( m_wblddlg->m_gflag ){
		if( !m_wblddlg->m_gname[0] ){
			_ASSERT( 0 );
			delete mafile;
			return 1;		
		}
		ret = SaveGSigFile( m_wblddlg->m_gname );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : SaveGSigFile error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
	}
///////////
	if( !m_wblddlg->m_moaname[0] ){
		_ASSERT( 0 );
		delete mafile;
		return 1;		
	}


	char dirname[MAX_PATH];
	char moaname[MAX_PATH];
	ZeroMemory( dirname, sizeof( char ) * MAX_PATH );
	ZeroMemory( moaname, sizeof( char ) * MAX_PATH );

	strcpy_s( dirname, MAX_PATH, m_wblddlg->m_moaname );
	int ch = '\\';
	char* dirlasten;
	dirlasten = strrchr( dirname, ch );
	if( !dirlasten ){
		DbgOut( "viewer : SaveBldFile : dirlasten NULL error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}
	*dirlasten = 0;

	char* moalasten;
	moalasten = strrchr( m_wblddlg->m_moaname, ch );
	if( !moalasten ){
		DbgOut( "viewer : SaveBldFile : moalasten NULL error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}
	strcpy_s( moaname, MAX_PATH, moalasten + 1 );

	char moaext[10] = ".moa";
	char* moapatptr;
	moapatptr = strstr( moaname, moaext );
	if( !moapatptr ){
		DbgOut( "viewer : SaveBldFile : moapatptr NULL error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}
	*moapatptr = 0;
	
	ret = WriteMAFile( dirname, moaname );
	if( ret ){
		DbgOut( "viewer : SaveBldFile : WriteMAFile error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}
////////////////
	if( m_wblddlg->m_chkgpa ){
		ret = SaveGPFile( m_wblddlg->m_gpaname );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : SaveGPFile error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
	}
///////////////
	if( m_wblddlg->m_chkmoe ){
		ret = SaveMOEFile( m_wblddlg->m_moename );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : SaveMOEFile error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
	}
////////////////
	if( m_wblddlg->m_chkcam ){
		ret = SaveCameraFile( m_wblddlg->m_camname );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : SaveCameraFile error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
	}
////////////////
	if( m_wblddlg->m_chkssf ){
		char ssfdir[MAX_PATH];
		ZeroMemory( ssfdir, sizeof( char ) * MAX_PATH );
		strcpy_s( ssfdir, MAX_PATH, m_wblddlg->m_ssfname );
		char* ssflasten;
		int ch = '\\';
		ssflasten = strrchr( ssfdir, ch );
		if( !ssflasten ){
			DbgOut( "viewer : SaveBldFile : ssflasten NULL error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		*ssflasten = 0;

		ret = SaveSndSetFile( m_wblddlg->m_ssfname, ssfdir );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : SaveSndSetFile error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		
		if( m_wblddlg->m_chksaf ){
			ret = SaveSndAnimFile( m_wblddlg->m_safname );
			if( ret ){
				DbgOut( "viewer : SaveBldFile : SaveSndAnimFile error !!!\n" );
				_ASSERT( 0 );
				delete mafile;
				return 1;
			}
		}
	}
///////////////////

	CBLDFile bldfile;
	BLD sigbld, gbld, moabld, gpabld, moebld, cambld, ssbld, sabld;
	ZeroMemory( &sigbld, sizeof( BLD ) );
	ZeroMemory( &gbld, sizeof( BLD ) );
	ZeroMemory( &moabld, sizeof( BLD ) );
	ZeroMemory( &gpabld, sizeof( BLD ) );
	ZeroMemory( &moebld, sizeof( BLD ) );
	ZeroMemory( &cambld, sizeof( BLD ) );
	ZeroMemory( &ssbld, sizeof( BLD ) );
	ZeroMemory( &sabld, sizeof( BLD ) );

	ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_signame, &(sigbld.name[0]), MAX_PATH );
	if( ret ){
		DbgOut( "viewer : SaveBldFile : GetRelatedPath sig error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}
	sigbld.flag = 1;
	sigbld.mult = m_wblddlg->m_sigmult;

	if( m_wblddlg->m_gflag ){
		ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_gname, &(gbld.name[0]), MAX_PATH );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : GetRelatedPath g error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		gbld.flag = 1;
		gbld.mult = m_wblddlg->m_gmult;
	}

	ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_moaname, &(moabld.name[0]), MAX_PATH );
	if( ret ){
		DbgOut( "viewer : SaveBldFile : GetRelatedPath moa error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}
	moabld.flag = 1;
	
	if( m_wblddlg->m_chkgpa ){
		ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_gpaname, &(gpabld.name[0]), MAX_PATH );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : GetRelatedPath gpa error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		gpabld.flag = 1;
	}
	if( m_wblddlg->m_chkmoe ){
		ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_moename, &(moebld.name[0]), MAX_PATH );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : GetRelatedPath moe error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		moebld.flag = 1;
	}
	if( m_wblddlg->m_chkcam ){
		ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_camname, &(cambld.name[0]), MAX_PATH );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : GetRelatedPath cam error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		cambld.flag = 1;
		if( m_wblddlg->m_cammode == 0 ){
			cambld.mode = 1;
		}else{
			cambld.mode = 0;
		}
	}
	if( m_wblddlg->m_chkssf ){
		ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_ssfname, &(ssbld.name[0]), MAX_PATH );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : GetRelatedPath ssf error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		ssbld.flag = 1;
	}
	if( m_wblddlg->m_chksaf ){
		ret = GetRelatedPath( m_wblddlg->m_bldname, m_wblddlg->m_safname, &(sabld.name[0]), MAX_PATH );
		if( ret ){
			DbgOut( "viewer : SaveBldFile : GetRelatedPath saf error !!!\n" );
			_ASSERT( 0 );
			delete mafile;
			return 1;
		}
		sabld.flag = 1;
	}

	ret = bldfile.SetBLD( &sigbld, &gbld, &moabld, &gpabld, &moebld, &cambld, &ssbld, &sabld );
	_ASSERT( !ret );

	ret = bldfile.WriteBLDFile( m_wblddlg->m_bldname );
	if( ret ){
		DbgOut( "viewer : SaveBldFile : bldfile WriteBLDFile error !!!\n" );
		_ASSERT( 0 );
		delete mafile;
		return 1;
	}


	delete mafile;


	return 0;
}

int CMyD3DApplication::SaveESIFile()
{
	int ret;
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;

	if( !m_ExistData )
		return 0;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.esi" );

	OPENFILENAME ofn;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"ESI File (*.esi)\0*.esi\0";
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
	ofn.lpstrDefExt ="esi";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return S_OK;
	}


	CESIFile esifile;
	ret = esifile.WriteESIFileOnFrame( buf, g_motdlg->GetMotCookie(), g_motdlg->GetCurrentFrameNo(), 
		m_thandler, m_shandler, m_mhandler );
	if( ret ){
		DbgOut( "viewer : SaveESIFile : esifile WriteESIFileOnFrame error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

int CMyD3DApplication::SaveEQUFile()
{
	int ret;
	if( !m_thandler || !m_shandler || !m_mhandler )
		return S_OK;

	if( !m_ExistData )
		return 0;

	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.equ" );

	OPENFILENAME ofn;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"EQU File (*.equ)\0*.equ\0";
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
	ofn.lpstrDefExt ="equ";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( !GetSaveFileName(&ofn) ){
		return S_OK;
	}

	int motnum = 0;
	motnum = g_motdlg->GetMotionNum();

	if( motnum <= 0 ){
		return 0;
	}

	int* arid = (int*)malloc( sizeof( int ) * motnum );
	if( !arid ){
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( arid, sizeof( int ) * motnum );

	int motcnt;
	for( motcnt = 0; motcnt < motnum; motcnt++ ){
		*( arid + motcnt ) = motcnt;
	}

	CEQUFile equfile;
	ret = equfile.WriteEQUFile( buf, arid, motnum, m_thandler, m_shandler, m_mhandler );
	if( ret ){
		DbgOut( "viewer : SaveEQUFile : equfile WriteEQUFile error !!!\n" );
		_ASSERT( 0 );
		free( arid );
		return 1;
	}

	free( arid );

	return 0;
}

int CMyD3DApplication::ChangeZaKindFromMotion( int srcmotid )
{
	int zakind;
	zakind = m_mhandler->GetZaType( srcmotid );


	if( zakind == ZA_1 ){
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_ZA1, 0 );
	}else if( zakind == ZA_2 ){
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_ZA2, 0 );
	}else if( zakind == ZA_3 ){
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_ZA3, 0 );
	}else if( zakind == ZA_4 ){
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_ZA4, 0 );
	}else{
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_ZA1, 0 );
	}

	return 0;
}

int CMyD3DApplication::OnPluginClose()
{
	if( !m_plugin ){
		return 0;
	}

	int pluginno;
	for( pluginno = 0; pluginno < MAXPLUGIN; pluginno++ ){
		if( (m_plugin + pluginno)->validflag == 1 ){
			(m_plugin + pluginno)->CallOnClose();
		}
	}

	return 0;
}

int CMyD3DApplication::OnPluginPose()
{
	if( !m_plugin ){
		return 0;
	}

	if( !m_shandler || (m_mhandler->m_kindnum <= 0) ){
		return 0;
	}
	int motid;
	motid = g_motdlg->GetMotCookie();
	if( motid < 0 ){
		return 0;
	}

	int pluginno;
	for( pluginno = 0; pluginno < MAXPLUGIN; pluginno++ ){
		if( (m_plugin + pluginno)->validflag == 1 ){
			(m_plugin + pluginno)->CallOnPose( motid );
		}
	}

	return 0;
}

int CMyD3DApplication::UpdateCameraFromDlg()
{
	CAMERAELEM curcamera;
	g_motdlg->m_camdlg->GetCurCamera( &curcamera );


	// mouseでボーンをドラッグ中は、キーイン禁止!!!
	if( m_lbutton == 0 ){
		int keyflag = 0;
		if( g_KeyBuffer[VK_F1] & 0x80 ){
			keyflag = ID_KEY_90LEFT;
		}else if( g_KeyBuffer[VK_F2] & 0x80 ){
			keyflag = ID_KEY_90RIGHT;
		}else if( g_KeyBuffer[VK_F3] & 0x80 ){
			keyflag = ID_KEY_90UP;
		}else if( g_KeyBuffer[VK_F4] & 0x80 ){
			keyflag = ID_KEY_90DOWN;
		}else if( g_KeyBuffer[VK_F9] & 0x80 ){
			keyflag = ID_KEY_SYMMX;
		}else{
			keyflag = 0;
		}
		RotateVKCamera( keyflag, curcamera );

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

	if( g_KeyBuffer[VK_SHIFT] & 0x80 ){
		if( g_KeyBuffer[VK_F5] & 0x80 ){
			m_savecamera[0] = curcamera;
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ５キーに保存しました。", "保存確認", MB_OK );
		}else if( g_KeyBuffer[VK_F6] & 0x80 ){
			m_savecamera[1] = curcamera;
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ６キーに保存しました。", "保存確認", MB_OK );
		}else if( g_KeyBuffer[VK_F7] & 0x80 ){
			m_savecamera[2] = curcamera;
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ７キーに保存しました。", "保存確認", MB_OK );
		}else if( g_KeyBuffer[VK_F8] & 0x80 ){
			m_savecamera[3] = curcamera;
			::MessageBox( m_hWnd, "現在のカメラ位置を、Ｆ８キーに保存しました。", "保存確認", MB_OK );
		}
	}else{
		int setflag = 0;
		if( g_KeyBuffer[VK_F5] & 0x80 ){
			curcamera = m_savecamera[0];
			setflag = 1;
		}else if( g_KeyBuffer[VK_F6] & 0x80 ){
			curcamera = m_savecamera[1];
			setflag = 1;
		}else if( g_KeyBuffer[VK_F7] & 0x80 ){
			curcamera = m_savecamera[2];
			setflag = 1;
		}else if( g_KeyBuffer[VK_F8] & 0x80 ){
			curcamera = m_savecamera[3];
			setflag = 1;
		}
		if( setflag ){
			g_motdlg->m_camdlg->SetCurCamera( curcamera );
		}
	}

	g_motdlg->m_camdlg->GetCurCamera( &curcamera );

	m_targetpos = curcamera.target;
	m_eye_y = curcamera.target.y;
	m_vEyePt = curcamera.pos;
	m_camup = curcamera.up;
	m_proj_near = curcamera.nearz;
	m_proj_far = curcamera.farz;
	m_fov = curcamera.fov;
	m_ortho = curcamera.ortho;
	m_orthosize = curcamera.orthosize;
	D3DXMatrixLookAtLH( &m_matView, &m_vEyePt, &m_targetpos, &m_camup );
	
	CamElem2Deg( curcamera, &m_degxz, &m_degy );

	return 0;

}


int CMyD3DApplication::SaveFBXFile()
{

	CWriteFBXDlg fbxdlg;
	int dlgret = (int)fbxdlg.DoModal();
	if( dlgret != IDOK ){
		return 0;
	}

	int saveshader = m_shandler->m_shader;

	/*
	m_shandler->m_shader = COL_TOON1;
	m_tbb_colT = 0;
	SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLT, 0 );

	int ret;
	if( m_dtri_sh ){
		m_dtri_sh->m_shader = COL_TOON1;
		ret = m_dtri_sh->CreateToon1Buffer( m_pd3dDevice, 1 );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
	}
	*/

	//forceflag = 1でCreateToon1Bufferを呼び出す。
	int ret;
	if (m_shandler){
		m_shandler->m_shader = COL_TOON1;
		ret = m_shandler->CreateToon1Buffer(m_pd3dDevice, 1);
		if (ret){
			DbgOut("viewer : ColorChange : IDTBB_COLT : COL_TOON1 : sh CreateToon1Buffer error !!!\n");
			_ASSERT(0);
			return 1;
		}
	}
	if (m_dtri_sh){
		m_dtri_sh->m_shader = COL_TOON1;
		ret = m_dtri_sh->CreateToon1Buffer(m_pd3dDevice, 1);
		if (ret){
			_ASSERT(0);
			return 1;
		}
	}

	m_tbb_colP = 0;
	m_tbb_colT = 1;
	m_tbb_colPP = 0;
	CheckTBB();



	WCHAR wname[MAX_PATH];
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, fbxdlg.name, MAX_PATH, wname, MAX_PATH );

	char fbxpath[MAX_PATH] = {0};
	WideCharToMultiByte( CP_UTF8, 0, wname, -1, fbxpath, MAX_PATH, NULL, NULL );	

	int ret2;
	ret2 = WriteFBXFile( m_pd3dDevice, m_thandler, m_shandler, m_mhandler, fbxpath, fbxdlg.mult, m_dtri_th, m_dtri_sh, fbxdlg.createbunkiflag );
	if( ret2 ){
		_ASSERT( 0 );
		MessageBox( "FBX出力に失敗しました。", "エラー", MB_OK );
		return 1;
	}

	/*
	m_shandler->m_shader = saveshader;

	switch( m_shandler->m_shader ){
	case COL_PHONG:
	case COL_OLD:
	case COL_BLINN:
	case COL_SCHLICK:
	case COL_MQCLASSIC:
		m_tbb_colP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLP, 0 );
		break;
	case COL_PPHONG:
		m_tbb_colPP = 0;
		SendMessage( m_hWnd, WM_COMMAND, (WPARAM)IDTBB_COLPP, 0 );
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
	*/

	MessageBox( "FBX出力に成功しました。", "成功", MB_OK );

	return 0;
}
