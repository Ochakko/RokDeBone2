#ifndef D3DAPP_H
#define D3DAPP_H

//715827882

// basetsd.h defines INT_PTR (used below).  It is not included by default
// under VC 5.0.  If you are using VC6 or later, it is included via Windows.h.
#include <basetsd.h> 

#include <d3dx9.h>

#include <coef.h>
#include "resource.h"       // ÉÅÉCÉì ÉVÉìÉ{Éã
#include <atlhost.h>
#include <atlctl.h>

#include "quaternion.h"

#include "inidata.h"
//#include <xfont.h>

#include <vector>


class CTreeHandler2;
class CShdHandler;
class CMotHandler;

class CD3DFont;

//class CCameraDlg;
class CSigEditDlg;
class CSigEditWnd;
class CGraphWnd;
class COptDlg;
class CMotDlg;
class CParsDlg;
class CBoneLineDlg;
class CLoadNameDlg;
class CItiModeDlg;
class CItiDlg;
class CLightDlg;
class CSetColDlg;
class CCreateBoneDlg;
class CInfScopeDlg;
class CPaintDlg;
class CToon0Dlg;
class CMotChangeDlg;
class CWriteMAFDlg;
class CMaterialDlg;
class CBonePropDlg;
class CZa4Dlg;
class CGlowViewDlg;
class CSyncDlg;

class CBgDisp2;
class CShdElem;
class CLightData;

class CSoundBank;
class CHuSound;

class CWriteBldDlg;

class CPluginElem;
class CPluginKinect;

#define MAXPLUGIN	255


//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
/***
//coef.hÇ…à⁄ìÆ
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESIZEFAILED        0x8200000c
***/

typedef struct tag_DispElem
{
	IDirect3DSurface9*	pZSurf;
	IDirect3DSurface9*	pBackBuffer;
	IDirect3DTexture9*	pTex;
	int width;
	int height;
} DISPELEM;



//-----------------------------------------------------------------------------
// Name: struct D3DModeInfo
// Desc: Structure for holding information about a display mode
//-----------------------------------------------------------------------------
struct D3DModeInfo
{
    DWORD      Width;      // Screen width in this mode
    DWORD      Height;     // Screen height in this mode
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
};




//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Structure for holding information about a Direct3D device, including
//       a list of modes compatible with this device
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
    // Device data
    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
    D3DCAPS9     d3dCaps;         // Capabilities of this device
    const TCHAR* strDesc;         // Name of this device
    BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

    // Modes for this device
    DWORD        dwNumModes;
    D3DModeInfo  modes[150];

    // Current state
    DWORD        dwCurrentMode;
    BOOL         bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleType;
};




//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Structure for holding information about an adapter, including a list
//       of devices available on this adapter
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
    // Adapter data
    D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
    D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter

    // Devices for this adapter
    DWORD          dwNumDevices;
    D3DDeviceInfo  devices[5];

    // Current state
    DWORD          dwCurrentDevice;
};



class CMyD3DApplication : public CWindowImpl<CMyD3DApplication>
{

BEGIN_MSG_MAP(CMyD3DApplication)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_ENTERMENULOOP, OnEnterMenuLoop)
	MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
	MESSAGE_HANDLER(WM_POWERBROADCAST, OnPowerBroadcast)
	MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)


	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMainLButtonDown)
	MESSAGE_HANDLER(WM_RBUTTONDOWN, OnMainRButtonDown)
	MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMainMButtonDown)

	MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)


	NOTIFY_CODE_HANDLER(TTN_NEEDTEXT, OnNeedText)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_ACTIVATEAPP, OnActivateApp)
	MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_USER_ANIMMENU, OnUserAnimMenu)
	MESSAGE_HANDLER(WM_USER_CAMERAMENU, OnUserCameraMenu)
	MESSAGE_HANDLER(WM_USER_GPMENU, OnUserGPMenu)
	MESSAGE_HANDLER(WM_USER_MOEMENU, OnUserMOEMenu)
	MESSAGE_HANDLER(WM_USER_BONEDISP, OnUserBoneDisp)
	MESSAGE_HANDLER(WM_USER_ERROR, OnUserError)
	MESSAGE_HANDLER(WM_USER_DISPLAY, OnUserDisplay)
	MESSAGE_HANDLER(WM_USER_ENABLE_MENU, OnUserEnableMenu)
	//MESSAGE_HANDLER(WM_USER_KEY, OnUserKey)
	MESSAGE_HANDLER(WM_USER_MOVING, OnUserMoving)
	MESSAGE_HANDLER(WM_USER_MOVE, OnUserMove)
	MESSAGE_HANDLER(WM_USER_SELCHANGE, OnUserSelchange)
	MESSAGE_HANDLER(WM_USER_INFLUENCE, OnUserInfluence)
	MESSAGE_HANDLER(WM_USER_REMAKE_TREE, OnUserRemakeTree)
	MESSAGE_HANDLER(WM_USER_CHANGE_NOTUSE, OnUserChangeNotUse)
	MESSAGE_HANDLER(WM_USER_NEW_JOINT, OnUserNewJoint)
	MESSAGE_HANDLER(WM_USER_DEL_JOINT, OnUserDelJoint)
	MESSAGE_HANDLER(WM_USER_CONV2SCOPE, OnUserConv2Scope)
	MESSAGE_HANDLER(WM_USER_CHANGE_JOINTLOC, OnUserChangeJointLoc)
	MESSAGE_HANDLER(WM_USER_CALCEUL, OnUserCalcEul)
	MESSAGE_HANDLER(WM_USER_REMAKE_TEXTURE, OnUserRemakeTexture)
	MESSAGE_HANDLER(WM_USER_RENAME, OnUserRename)
	MESSAGE_HANDLER(WM_USER_REMAKE_DISPOBJ, OnUserRemakeDispObj)
	MESSAGE_HANDLER(WM_USER_REMAKE_UV, OnUserRemakeUV)
	MESSAGE_HANDLER(WM_USER_ENDDIALOG, OnUserEndDialog)
	MESSAGE_HANDLER(WM_USER_CHANGE_COLOR, OnUserChangeColor)
	MESSAGE_HANDLER(WM_USER_PREVIEW, OnUserPreview)
	MESSAGE_HANDLER(WM_USER_SELLOCK, OnUserSelLock)
	MESSAGE_HANDLER(WM_USER_INIT_UNDOBUF, OnUserInitUndoBuf)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
	MESSAGE_HANDLER(WM_USER_CREATE_PB, OnUserCreatePB)
	MESSAGE_HANDLER(WM_USER_SET_PB, OnUserSetPB)
	MESSAGE_HANDLER(WM_USER_DESTROY_PB, OnUserDestroyPB)
	MESSAGE_HANDLER(WM_USER_IMPORTMQO, OnUserImportMQO)
	MESSAGE_HANDLER(WM_USER_LOADBON, OnUserLoadBon)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)

ALT_MSG_MAP(MAIN3D_MSGMAP)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
	MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
	MESSAGE_HANDLER(WM_MBUTTONUP, OnMButtonUp)
END_MSG_MAP()


	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnterMenuLoop(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPowerBroadcast(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnMainLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMainRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMainMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnNeedText( int idCtrl, LPNMHDR pnmh, BOOL& bHandled );
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnActivateApp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserAnimMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserCameraMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserGPMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserMOEMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserBoneDisp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserError(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserDisplay(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserEnableMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserSelchange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserInfluence(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserRemakeTree(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserChangeNotUse(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserNewJoint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserDelJoint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserConv2Scope(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserChangeJointLoc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserCalcEul(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserRemakeTexture(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserRename(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserRemakeDispObj(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserRemakeUV(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserEndDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserChangeColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserPreview(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserSelLock(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserInitUndoBuf(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserCreatePB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserSetPB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserDestroyPB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserImportMQO(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserLoadBon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);




public:
    CMyD3DApplication();
	~CMyD3DApplication();

	
    HRESULT CreateWnd( HINSTANCE hInstance );
    INT     Run();
//    virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    VOID    Pause( BOOL bPause );
	HRESULT FrameMove( int srcseri = 0, int tra_boneonly = 0 );	
	HRESULT Render3DEnvironment( int skipflag = 0 );
    HRESULT Resize3DEnvironment();
	int ComputeRect( int resize3d, RECT* treerect );

	int SetShaderConst( int shadowflag, D3DXMATRIX* pmatProj );
	
	int OnPluginClose();
	int OnPluginPose();

protected:
	int CMyD3DApplication::RemakeAllDispObj();

	// Internal error handling function
    HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType, char* mesptr = 0 );

    // Internal functions to manage and render the 3D scene
    HRESULT BuildDeviceList();
    BOOL    FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
                D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );
    HRESULT Initialize3DEnvironment();
    HRESULT ToggleFullscreen();
    HRESULT ForceWindowed();
    HRESULT UserSelectNewDevice();
    VOID    Cleanup3DEnvironment();
    //HRESULT Render3DEnvironment();
    virtual HRESULT AdjustWindowForChange();
    static INT_PTR CALLBACK SelectDeviceProc( HWND hDlg, UINT msg, 
                WPARAM wParam, LPARAM lParam );

	int SetModuleDir();
	int SetMediaDir();
	int CreateAppToolBar();
	int DestroyAppToolBar();
	int SetShadowParams();

	int TogglePars();
	int ToggleNotUse();

	int CalcSelectMat();
	int SetEffectDecl( LPDIRECT3DDEVICE9 pdev );
public:
	DISPELEM		m_shadowelem;
    BOOL              m_bActive;
    BOOL              m_bReady;
	int				m_InitFlag;
//    HWND              m_hWnd;              // The main app window
    LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device
    D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
	int m_writebmpflag;

	int m_force_render;
	DWORD m_rendertime;

	int						m_lbutton;
	int						m_movebone;
	int						m_pickgp;
	int						m_onselectplugin;
	int						m_kinectflag;

	int						m_rbutton;
	int						m_mbutton;
	int						m_shiftbone;
	int						m_shiftpart;
	int						m_shifvertnum;
	int						m_shifvert[PICKVERTMAX];
	int						m_shiftface;

	int						m_scalebone;

	int						m_lrbutton;

	int						m_intormenu;
	int						m_vikflag;

	IDirect3DSurface9*		m_pZSurf;
	IDirect3DSurface9*		m_pBackBuffer;
	int						m_resizeflag;

	int						m_preview_flag;

	RECT					m_rc3dwnd;
	CContainedWindow		m_3dwnd;
	RECT					m_rcsigedit;
	RECT					m_rcmot;
	RECT					m_rcgraph;

	char					m_autosavedir[MAX_PATH];

	int						m_tbb_za4;
	int						m_tbb_glow;
protected:
	HMENU			m_menu;
	HANDLE			m_hbmp;
    // Internal variables for the state of the app
    D3DAdapterInfo    m_Adapters[10];
    DWORD             m_dwNumAdapters;
    DWORD             m_dwAdapter;
    BOOL              m_bWindowed;
	int				  m_screensave;

    // Internal variables used for timing
    BOOL              m_bFrameMoving;
    BOOL              m_bSingleStep;

	LARGE_INTEGER	m_freq;
	LARGE_INTEGER	m_rendercnt;
	LARGE_INTEGER	m_curcnt;
	double			m_dfreq;
	double			m_drenderfreq;
	double			m_drendercnt;
	double			m_dcurcnt;


	// Main objects used for creating and rendering the 3D scene
    //D3DPRESENT_PARAMETERS m_d3dpp;//<--- publicÇ÷         // Parameters for CreateDevice/Reset
    HWND              m_hWndFocus;         // The D3D focus window (usually same as m_hWnd)
    LPDIRECT3D9       m_pD3D;              // The main D3D object
    D3DCAPS9          m_d3dCaps;           // Caps for the device
    D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer
    DWORD             m_dwCreateFlags;     // Indicate sw or hw vertex processing
    DWORD             m_dwWindowStyle;     // Saved window style for mode switches
    RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
    RECT              m_rcWindowClient;    // Saved client area size for mode switches


	HBRUSH			m_bluebrush;

    // Variables for timing
    float             m_fTime;             // Current time in seconds
    float             m_fElapsedTime;      // Time elapsed since last frame
    float             m_fFPS;              // Instanteous frame rate
    TCHAR             m_strDeviceStats[90];// String to hold D3D device stats
    TCHAR             m_strFrameStats[40]; // String to hold frame stats

    // Overridable variables for the app
    TCHAR*            m_strWindowTitle;    // Title for the app's window
    BOOL              m_bUseDepthBuffer;   // Whether to autocreate depthbuffer
    DWORD             m_dwMinDepthBits;    // Minimum number of bits needed in depth buffer
    DWORD             m_dwMinStencilBits;  // Minimum number of bits needed in stencil buffer
    DWORD             m_dwCreationWidth;   // Width used to create window
    DWORD             m_dwCreationHeight;  // Height used to create window
    BOOL              m_bShowCursorWhenFullscreen; // Whether to show cursor when fullscreen    


////////////
// application
public:
	int						m_control_key;
	int						m_shift_key;
	
	//	CFont*					m_dlgfont;
	CHuSound*				m_HS;

	CTreeHandler2*			m_thandler;
    CShdHandler*			m_shandler;
	CMotHandler*			m_mhandler;

	CTreeHandler2*			m_sph_th;
	CShdHandler*			m_sph_sh;
	CMotHandler*			m_sph_mh;

	CTreeHandler2*			m_ei3_th;
	CShdHandler*			m_ei3_sh;
	CMotHandler*			m_ei3_mh;

	CTreeHandler2*			m_g_th;
	CShdHandler*			m_g_sh;
	CMotHandler*			m_g_mh;

	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matView;
	D3DXMATRIX				m_matProjX;

	D3DXMATRIX				m_matViewShadow;
	D3DXMATRIX				m_matProjShadow;
	D3DXMATRIX				m_matBiasShadow;
	float					m_shadowcoef[2];

	float					m_degxz;
	float					m_degy;
	BOOL				m_ExistData;

	GUARDBAND				m_gband;
	GUARDBAND				m_smallgband;
	D3DVIEWPORT9			m_viewport;
	DWORD					m_clientWidth;
	DWORD					m_clientHeight;

	int						m_selectedno;
//	CSigEditDlg*			m_sigeditdlg;
	CSigEditWnd*			m_sigeditwnd;
	CGraphWnd*				m_graphwnd;
	CItiDlg*				m_itidlg;
	CMotChangeDlg*			m_mcdlg;
	CSoundBank*				m_soundbank;
	CSyncDlg*				m_syncdlg;
	int						m_syncdisp;

	int						m_tbb_r;//publicÇ÷
	int						m_tbb_t;//publicÇ÷
	int						m_tbb_t2;//publicÇ÷
	int						m_tbb_s;//publicÇ÷
	int						m_tbb_mo;//publicÇ÷

	IDirect3DVertexDeclaration9* m_effectdecl;
	IDirect3DVertexDeclaration9* m_tlvdecl;

	int						m_remaketree_calc0;

	CAMERAELEM				m_camera;
	CAMERAELEM				m_savecamera[4];
	D3DXMATRIX				m_inimat;

	int						m_destroyflag;
	int						m_za_kind;

	COptDlg*				m_optdlg;

protected:
	LPD3DXSPRITE		m_d3dxsprite;

    CD3DFont*          m_pFont;
    CD3DFont*          m_pFontSmall;

	int					m_dirlightid;

    BYTE               m_bKey[256];
	int				   m_exit;
	HWND				m_pbwnd;

    HRESULT ConfirmDevice( D3DCAPS9*, DWORD, D3DFORMAT );

	CTreeHandler2*			m_dtri_th;
	CShdHandler*			m_dtri_sh;
	CMotHandler*			m_dtri_mh;

	CTreeHandler2*			m_ground_th;
	CShdHandler*			m_ground_sh;
	CMotHandler*			m_ground_mh;

	CTreeHandler2*			m_arrow_th;
	CShdHandler*			m_arrow_sh;
	CMotHandler*			m_arrow_mh;

	CTreeHandler2*			m_select_th;
	CShdHandler*			m_select_sh;
	CMotHandler*			m_select_mh;
	D3DXMATRIX				m_select_mat;//matworld

	CTreeHandler2*			m_ikt_th;
	CShdHandler*			m_ikt_sh;
	CMotHandler*			m_ikt_mh;

	CTreeHandler2*			m_ext_th;
    CShdHandler*			m_ext_sh;
	CMotHandler*			m_ext_mh;

	int						m_sel_xno;//"obj_X"
	int						m_sel_yno;//"obj_Y"
	int						m_sel_zno;//"obj_Z"
	int						m_sel_cenno;//"obj_CENTER"
	int						m_sel_ringxno;//obj_ringX
	int						m_sel_ringyno;//obj_ringY
	int						m_sel_ringzno;//obj_ringZ

	int						m_select_kind;
	//int						m_selectedno;//---> publicÇ÷
	D3DXVECTOR3				m_selectedpos;
	float					m_select_delta;

	CTreeHandler2*			m_bm_th;
	CShdHandler*			m_bm_sh;
	CMotHandler*			m_bm_mh;
	

	CTreeHandler2*			m_sq_th;
	CShdHandler*			m_sq_sh;
	CMotHandler*			m_sq_mh;
	D3DXMATRIX				m_sq_mat;


	int						m_curcookie;
	int						m_motcookie0;
	char					m_motname0[256];
	int						m_framemax0;
	int						m_mottype0;

	int						m_motcookie1;
	char					m_motname1[256];
	int						m_framemax1;
	int						m_mottype1;
	
	float					m_eye_y;
	float					m_camdist;
	D3DXVECTOR3				m_targetpos;

	int						m_create3dobj;

	//CComPtr<IMotDlgHandler> m_motdlgptr;
	//CMotDlg*			m_motdlg;
//	CCameraDlg*				m_cameradlg;
	//CSigEditDlg*			m_sigeditdlg;// <--- publicÇ÷
//	CParsDlg*				m_parsdlg;
	CBoneLineDlg*			m_bonelinedlg;
	CLoadNameDlg*			m_loadnamedlg;
	CWriteBldDlg*			m_wblddlg;
	CItiModeDlg*			m_itimodedlg;
	//CItiDlg*				m_itidlg;//<-- publicÇ÷
	CLightDlg*				m_lightdlg;
	CSetColDlg*				m_setcoldlg;
	CCreateBoneDlg*			m_bonedlg;
	CInfScopeDlg*			m_infscopedlg;
	CPaintDlg*				m_paintdlg;
	CToon0Dlg*				m_Toon0Dlg;
	//CMotChangeDlg*			m_mcdlg;//<-- publicÇ÷
	CWriteMAFDlg*			m_wmafdlg;
	CMaterialDlg*			m_materialdlg;
	CGlowViewDlg*			m_glowdlg;
	//CBonePropDlg*			m_bonepropdlg;//<-- globalÇ÷
	CZa4Dlg*				m_za4dlg;

	CBgDisp2*				m_bgdisp;

	//int						m_disp_cameradlg;
	//int						m_disp_sigdlg;
	//int						m_disp_motdlg;
	int						m_refuge_motdlg;
	HMENU					m_appmenu;
	HMENU					m_dummymenu;
	HMENU					m_pluginmenu;
	int						m_menucnt;
	HMENU					m_animmenu;
	HMENU					m_cameramenu;
	HMENU					m_gpmenu;
	HMENU					m_moemenu;

	int						m_moving_cameradlg;
	int						m_moving_sigdlg;

	//char					m_bgname1[_MAX_PATH];
	//char					m_bgname2[_MAX_PATH];
	//float					m_bgmvu;
	//float					m_bgmvv;
	//COLORREF				m_bgcolor;

	//int						m_light_alpha;
	//int						m_light_beta;

	//int						m_lbutton;
	//int						m_movebone;
	//int						m_rbutton;
	//int						m_shiftbone;


	int						m_timerid;

	int						m_sellockflag;
	TCHAR					m_strSellock[1024];

	int						m_bonearray[MAXBONENUM + 1];
	
	int						m_ik_cnt;
	int						m_undo_cnt;
	int						m_undo_init_flag;

	//CXFont*					m_xfont;

	POINT					m_lpos;
	POINT					m_rpos;
	POINT					m_mpos;
	POINT					m_lrpos;
	POINT					m_clickoffset;
	POINT					m_clickbone;

	RECT					m_clirect;

	int						m_drawtext;
	int						m_createmotflag;

	float					m_proj_near;
	float					m_proj_far;
	float					m_fov;
	int						m_ortho;
	float					m_orthosize;

	int						m_ikcalccnt;

//	int						m_tbb_r;//publicÇ÷
//	int						m_tbb_t;//publicÇ÷
//	int						m_tbb_t2;//publicÇ÷
//	int						m_tbb_s;//publicÇ÷
	//int						m_tbb_g;
	//int						m_tbb_l;
	int						m_tbb_iti;
	int						m_tbb_ikt;
	int						m_tbb_col;
	int						m_tbb_bone;
	int						m_tbb_ei1;
	int						m_tbb_ei2;
	int						m_tbb_ei3;
	int						m_tbb_toon1;
//	int						m_tbb_mo;//publicÇ÷
	int						m_tbb_4;

	int						m_tbb_face;
	int						m_tbb_line;
	int						m_tbb_point;
	int						m_tbb_ten2;

	int						m_tbb_za1;
	int						m_tbb_za2;
	int						m_tbb_za3;
//	int						m_tbb_za4;// publicÇ÷

	//int						m_tbb_colR;
	int						m_tbb_colP;
	//int						m_tbb_colB;
	//int						m_tbb_colS;
	//int						m_tbb_colM;
	int						m_tbb_colT;
	int						m_tbb_colPP;
	int						m_tbb_sha;

	//int						m_tbb_ofc;
	//int						m_tbb_ofs;
	//int						m_tbb_ofo;

	CQuaternion				m_localq;
	int						m_g_disp;

	CPluginElem*			m_plugin;
	CPluginKinect*			m_kinect;
	D3DXVECTOR3				m_vEyePt;
	D3DXVECTOR3				m_camup;

	float					m_wheeldelta;

	char					m_signame[1024];
	int						m_createshadowflag;

	RECT					m_camkaku_rect;
	RECT					m_cami_rect;
	RECT					m_camkai_rect;
	int						m_camope;

	int						m_keyanddrag;
	int						m_miniflag;

public:

	int UpdateCameraFromDlg();
	int ScreenToObj( D3DXVECTOR3* objpos, D3DXVECTOR3 scpos );
	int SetBoneArray( int srcseri );
	int UpdateVertexBuffer();
	int CopyUndoBuffer( int cpframe = -1 );

	int RestoreHandler( int needcalc, int dispwarnflag = 1 );
	int TempPickVert( int seldisp, int* shiftpart, int* shiftvert, int* shiftvertnum );
	int TempPickFace( int srcpart, int* pickpart, int* pickface, D3DXVECTOR3* pickpos );

//	int LoadMotion();
	int LoadMotion( char* filename, D3DXVECTOR3 quamult, int idling, int ev0idle, int commonid, int borbidnum, int* forbidid, int notfu, int callmoa = 0 );
	int LoadMotionMulti();
	int LoadCameraMulti();
	int LoadCamera( char* filename );
	int LoadGPMulti();
	int LoadGP( char* filename );
	int LoadMOEMulti();
	int LoadMOE( char* filename );
	

	//int ChangeMotionStep();
	//int SetMotionStep();

	int GetDispObjNum( int* numptr );
	int GetDispObjInfo( DISPOBJINFO* doiarray, int arrayleng, int* getnumptr );

	int Render3D( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int presentflag );
	int RenderFullScreenTLV( LPDIRECT3DSURFACE9 dstsurf, D3DVIEWPORT9* dstviewport, 
		LPDIRECT3DTEXTURE9 srctex, int texwidth, int texheight, int texf );
	int RTBeginScene( LPDIRECT3DDEVICE9 pdev, LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, 
		int skipbeginscene, int needz );
	int RenderGlow( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int zclear );
	int BlendRTTexture( LPDIRECT3DSURFACE9 dstsurf, D3DVIEWPORT9* dstviewport, 
		LPDIRECT3DTEXTURE9 ptex1, int width1, int height1, E3DCOLOR4F col1, int texf1,
		LPDIRECT3DTEXTURE9 ptex2, int width2, int height2, E3DCOLOR4F col2, int texf2, 
		int blendop, int srcblend, int destblend );
	int BlurTextureGauss( LPDIRECT3DSURFACE9 finalsurf, D3DVIEWPORT9* finalviewport,
		LPDIRECT3DSURFACE9 worksurf, LPDIRECT3DTEXTURE9 worktex, D3DVIEWPORT9* workviewport,
		LPDIRECT3DTEXTURE9 srctex, D3DVIEWPORT9* srcviewport, float dispersion );

	int CreateBone( char* nameptr, D3DXVECTOR3 newpos, CShdElem* parelem, int* seriptr, int restoreflag = 1 );
	int DelJoint( int delno, int restoreflag = 1 );
	int AddMorphFolder( int baseid, int boneid );
	int GetCameraElem( CAMERAELEM* dstce );
	int SetCameraElem( CAMERAELEM srcce );

	int GetKeyboardCnt();
	int ResetKeyboardCnt();

	int GetRelatedPath( char* basepath, char* srcpath, char* dstpath, int dstleng );
	int externRestore();

	int ChangeZaKindFromMotion( int srcmotid );

protected:
	//int SetSkipMatrix();
	//int UpdateVertexBuffer();

	int LoadFile();
	int ImportFile();
	int SaveFile();
	int SaveQuaFile();
	int SaveCameraFile( char* srcfilename = 0 );
	int SaveGPFile( char* srcfilename = 0 );
	int SaveMOEFile( char* srcfilename = 0 );
	int SaveBldFile( int overwflag );
	int SaveESIFile();
	int LoadESIFile( char* srcfilename );
	int SaveEQUFile();
	int LoadEQUFile( char* srcfilename );
	int LoadConvSetting( char* srcfilename );



	int LoadSigFile( char* srcfilename, int multflag = 0, float srcmult = 1.0f, int dlgwarnflag = 1 );
	//int LoadFBXFile();
	int LoadXFile( char* srcfilename );
	int LoadRokFile( char* srcfilename );
	int LoadMQOFile( LOADFILEINFO* ldinfo, char* srcfilename );
	int LoadGroundData( BLD* gbld = 0 );
	int ImportSigFile( char* srcfilename );
	int ImportRokFile( char* srcfilename );
	int ImportMqoFile( LOADFILEINFO* ldinfo, char* srcfilename );
	int SaveSigFile( char* filename );
	int SaveGSigFile( char* filename );
	int SaveXFile( char* srcfilename );
	int SaveFBXFile();
	int ExtractBoneData();
	int AutoSaveSigFile();
	int SaveToon1File( char* srcfilename );
	int LoadToon1File( char* srcfilename );
	int SavePoseFile( char* srcfilename );
	int LoadPoseFile( char* srcfilename );
	int LoadBVHFile( char* srcfilename );
	int SaveSndSetFile( char* srcfilename, char* ssfdir = 0 );
	int SaveSndAnimFile( char* srcfilename );
	int LoadSndSetFile( char* srcfilename );
	int LoadSndAnimFile( char* srcfilename, int dlgwarnflag );
	int LoadBLDFile( char* srcfilename );

	int InvalidateHandler( int resetflag, int flushname );
	int InvalidateGroundHandler( int resetflag = 0 );
	int InvalidateGHandler( int resetflag = 0 );
	int RestoreGroundHandler();
	int RestoreGHandler();
	int RestoreExtHandler( int needcalc );
	int RestoreShadowElem();
	int CreateShadowElem( int srcfmt, int rtsize, int* okflag );
	int DestroyShadowElem();
	int CheckRTFormat( int srcfmt, int* okflag );
	int SetRESDIR( char* pathname, CShdHandler* srclpsh );
	//int SetModuleDir();

	HRESULT Render();
	int RenderNormal( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int presentflag );
	int RenderWithShadow( LPDIRECT3DSURFACE9 psurf, LPDIRECT3DSURFACE9 pzsurf, D3DVIEWPORT9* pviewport, int presentflag );
	int SetRTRenderState( LPDIRECT3DDEVICE9 pdev, int needz );
	int SetGaussWeight( float dispersion );


	int ChangeViewParam( int keyflag, float step = 1000000.0f );
	int RotateVKCamera( int keyflag, CAMERAELEM curcamera );

	int CheckTBB();

	int SaveLimFile( char* srcfilename );
	int LoadLimFile( char* srcfilename );

	int SaveBVHFile( char* srcfilename );

    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects( int needcalc, int resetflag = 0 );
    HRESULT InvalidateDeviceObjects( int resetflag, int flushname );
    HRESULT DeleteDeviceObjects();
    HRESULT FinalCleanup();
    VOID    UpdateCamera();

	int CreateHandler();
	int DestroyHandler();
	int LoadData();

	int CreateGroundHandler();
	int CreateGHandler();
	int CreateExtHandler();
	int DestroyGroundHandler();
	int DestroyGHandler();
	int DestroyExtHandler();
	int NewJoint( int addnum, int restoreflag = 1 );
	int Conv2Scope( int srcboneno, int srcdispno, int srcconvno );
	int Add2Tree( char* newname, int serino, int srctype, int depth, int hasmotion, int createflag = 1 );

	int SetBg();
	int SetLight( int modalflag );
	int SetBoneDisp();
	int ShowVersionDlg();

	int DestroyMotion( int allflag );
	int NewMotion();

	int DestroyCameraAnim( int allflag );
	int NewCameraAnim();

	int DestroyGPAnim( int allflag );
	int NewGPAnim();

	int DestroyMOEAnim( int allflag );
	int NewMOEAnim();


	int PickBone( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int selaxis );
	int PickIKT( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr );
	int PickVert( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart, int* pickvert, int* pickvertnum, int selaxis );
	int PickVertSelDisp( int seldisp, WORD srcx, WORD srcy, int* pickpart, int* pickvert, int* pickvertnum );
	int PickFace( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart, int* pickface );
	int PickDispObj( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart );

	int PickInfScopeFace( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int isindex, int* pickface );
	int PickInfScopeVert( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int isindex, int* pickvert, int* pickvertnum );

	int PickSelect( WORD srcx, WORD srcy, int* selectptr );


	//int MoveBone( int mvno, float srcx, float srcy, int* lastparent );
	//int ShiftBone( int shiftno, float srcx, float srcy );
	//int GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir );


	void SetBoneArrayReq( CShdElem* selem, int* setnum, int broflag );

	int CreateProgressBar();
	int SetProgressBar( int newpos );
	int DestroyProgressBar();

	int WriteBoneFile( char* srcfilename );
	int LoadBoneFile( LOADFILEINFO* ldinfo );

	int WriteBIMFile();
	int LoadBIMFile();

	int WriteIM2File( char* srcfilename );
	int LoadIM2File( int reloadflag, char* srcfilename );

	int WriteMQOFile( char* srcfilename );

	int WriteMAFile( char* dirname, char* moaname );
	int LoadMAFile( char* srcfilename, int dlgwarnflag = 1 );

	int LockAndUnlock3DBuffer();
	int SetDefaultRenderState();
	int UpdateDialogIniData( int dlgno );

	int DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec );

	
	int CreateFont( LPDIRECT3DDEVICE9 pdev, int height, int width, int weight, int bItalic, int bUnderline, int bStrikeout, char* fontname );
	int DestroyFont();
	int DrawTextByFontID( HWND apphwnd, int posx, int posy, char* textstr, D3DCOLOR col );

	int ToggleBoneDisp();
	int DisableIKT();
	int IKTON();
	int IKTallON();
	int IKTToggle();

	//int GetScreenPos( D3DXVECTOR3 vec3d, int* x2dptr, int* y2dptr, int* validptr );
	int GetScreenPos( D3DXVECTOR3 vec3d, float* scxptr, float* scyptr, float* sczptr, float* wpptr );
	int GetRotateDelta( int rotkind, POINT befpos, POINT aftpos, float* dstdelta );
	int GetShiftDelta( int shiftkind, POINT befpos, POINT aftpos, float* dstdelta );
	int GetScaleDelta( int scalekind, POINT befpos, POINT aftpos, float* dstdelta );

	int DestroyDispAndMot();
	int SetCurrentPose2OrgData();
	int SetInvPose2OrgData();

	int ChangeJointInitialPos( int jointno, POINT srcpos );
	int ChangeIKTPos( int iktno, POINT srcpos );

	int SetMikoBlendType();

	int ValidSelectCircle( int srcflag );

	int SetSelectColor( int rgbflag );
	int InitPluginMenu();


	int OnCameraRot( POINT ptCursor, POINT befpos );
	int OnCameraMove( POINT ptCursor, POINT befpos );
	int OnCameraDist( float vert );

	int CreateBone( POINT pt );
	int CloneDispObj();

	int DelTree();
	int DelTreeElem( int srcdel, int dlgflag );
	void DelTreeReq( int delseri, int broflag );

	int DelMorph( int srcdel, int dlgflag );
	int DelDispObj( int srcdel, int dlgflag );
	int DelPart( int srcdel, int dlgflag );

	int OverWriteFiles();

	int MakeAllJointTreeFlat( CShdElem** ppfirstparent );
	int FindSeriByExtSeri( SERICONV* sericonv, int convnum, int extseri, CShdElem** ppselem );

	int SelChangeBone( WORD menuid );
//	int CameraByKey( WORD menuid );

	int ConvSymmXShape();
	int ConvSymmXMotion();
	int SetSymmJointPos();

	int ConvNormalMap();
	int LoadMKM();

	int Conv5Digits( int srcseri, char* dstptr );
	CLightData* GetLightData( int lid );
	int GetShaderHandle();

	int LButtonDownFunc( int kind, WORD xPos, WORD yPos );
	int RButtonDownFunc( int kind, WORD xPos, WORD yPos );
	int MButtonDownFunc( int kind, WORD xPos, WORD yPos );

	int CheckDestroyedNum();

	int CreateSymBone();
	int SetDelMark2SymBone( int type, int* markptr );
	int DeleteBoneByMark( int* delmark );
	int SetCreateMark2SymBone( int type, int* markptr );
//	void FindCreateMarkReq( int seri, int* markptr, int* findflagptr, int findbro );
	void CreateBoneByMarkReq( int seri, int* markptr, CShdElem* parelem, int* errorptr );

	int CamElem2Deg( CAMERAELEM camelem, float* degxz, float* degy );

	int SetSoundBank();

	int CreateCameraTexture();
	int RenderCameraSprite();
	int CreateNumTexture();


	int CheckCamOpe( POINT srcpt );
	int CamOpe( POINT ptCursor );

	int MotCut();
	int MotMabiki();

//	typedef void (*PFUNC)();
//	std::vector < PFUNC > Callback_StreamEnds;
//	void SetCallback_StreamEnd( PFUNC pFunc ){ Callback_StreamEnds.push_back(pFunc); };

};

#endif