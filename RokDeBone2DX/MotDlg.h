// MotDlg.h : CMotDlg の宣言

#ifndef __MOTDLG_H_
#define __MOTDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>

#include <d3dx9.h>

#include <coef.h>
#include "quaternion.h"

class CTreeHandler2;
class CShdHandler;
class CMotHandler;
class CMotionPoint2;
class CD3DFont;
class CMyD3DApplication;
class CLoadNameDlg;
class CMKMFile;
class CEQUFile;

class CMyD3DApplication;

#include "MotParamDlg.h"
#include "AxisDlg.h"
#include "VIKDlg.h"
#include "TexKeyDlg.h"
#include "DSKeyDlg.h"
#include "MorphKeyDlg.h"
#include "AlpKeyDlg.h"
#include "CamKeyDlg.h"
#include "GPKeyDlg.h"
#include "MOEKeyDlg.h"
#include "SndKeyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMotDlg
class CMotDlg : 
	public CWindowImpl<CMotDlg>
{
public:
	CMotDlg( CMyD3DApplication* papp, LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp, int iklevelope );
	~CMotDlg();

	enum { IDD = IDD_MOTDLG };

///// common
	int ShowDlg( int cmdshow );
	int OnJumpFrame( int srcframe, int quaflag, int camflag, int gpaflag, int moeflag, int sndflag );
	int SetSaveBMDisp( int srcdisp );
	int SetSaveJointDisp( int srcdisp );

///// for motparamdlg
	int SetParams( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, HWND papp, CMyD3DApplication* d3dapp );
	int SetAppWnd( HWND appwnd );

	int GetMotParams( int* mcookieptr, int* framenoptr );
	int GetFrameLeng( int motid, int* lengptr );
	int DestroyMotionObj();

	int Redraw();
	int RemakeTreeView( int selno );
//	int Rename( int srcseri, char* srcname );

	int OnUserSelchange( int selno );

	int SetDisplayFlag( int dflag );

	int InvalidateHandler();

	int LoadMotion( CLoadNameDlg* namedlg, char* srcfilename, D3DXVECTOR3 srcquamult, int callmoa = 0 );
	int LoadEQUMotion( CEQUFile* equfile, int animcnt, float quamult );
	int LoadMKM( char* filename, CMKMFile* mkmfile, CMKMFile* inimkm, int motno, float tramult );

	int ImportMotion( CLoadNameDlg* namedlg );
	int SetMotionStep( int srcmotstep );
	int LoadBVHFile( char* srcfilename );
	int SaveMotion( CLoadNameDlg* namedlg, char* srcfilename );
	int WriteBmp( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont );
	int WriteAvi( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont );

	int InverseMotionXZ();


	static DWORD WINAPI	ThreadFunc( LPVOID	lpThreadParam );
	int SetNewPose();

	int UpdateAllMatrix( int spallkind );
	int UpdateMatrix( int srcseri, int spallkind );
	//int UpdateMatrixSelected( int srcseri );

	int SetCurrentMotion();

	//int SetMotionPointIK( int mvboneno, CQuaternion multq, BOOL needredraw );
	//int SetMotionPointIK( int mvboneno, float srcmvx, float srcmvy, float srcmvz );

	//int GetQ( int boneno, CQuaternion* dstq );
	//int SetQ( int boneno, CQuaternion srcq );

	int CopyUndoBuffer( int cpframe = -1 );
	int PasteUndoBuffer( int undocnt, int undodlg );

	int ExpandAndShiftMotion( int shiftnum );


	int GetBonePos( int srcseri, D3DXMATRIX wmat, int mode, D3DXVECTOR3* dstpos );
	int GetSelectedJointPos( int* setflag, D3DXVECTOR3* jposptr );


	int GetMotCookie();
	int GetCurrentFrameNo();
	int GetPreviewFlag();
	int GetSellockno();
	//int GetIKBonenum();
		//m_mot_cookie
		//m_preview_flag
		//m_sellockno
		//m_ik_bonenum
	

	int MoveBone( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent );
	int MoveBone( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 tpos, int* lastparent, int defaxis, D3DXVECTOR3 srcaxis, D3DXVECTOR3* axisdone, int iklevel );
	int MoveBoneZa4( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent );
	int MoveBoneZa4( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 tpos, int* lastparent, int iklevel );


	int ShiftBone( int shiftno, float srcx, float srcy, int t2flag, int za4flag );
	int ShiftBone( int frameno, int shiftno, D3DXVECTOR3 tpos, int* setnoptr, int za4flag );
	int ShiftBone( int shiftno, int shiftkind, float delta, int t2flag, int localflag, CQuaternion localq, int za4flag );
	int ShiftBoneTree( int shiftno, D3DXVECTOR3 targetobj, int za4flag );

	int RotateBone( D3DXMATRIX matWorld, int mvno, int rotkind, float delta, int* lastparent, int localflag, CQuaternion localq, int ikfkflag, int za4flag );

	int ScaleBone( int scaleno, int scalekind, float delta );

	int CopyLowerMotion( CShdElem* srcselem );

	int OnFrameCopy();
	int OnFramePaste();

	int OnSpin_Sp_mp( int idctrl, NMUPDOWN* pnmud, BOOL& dummy );


	int OnStop();
	int OnPreview();

	int GetRangeMax( int motid, int* rangemax );
	int GetKeyFrameNo( int motid, int* keyframe, int* framenum );

	void Rotate2IKTargetReq( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 rotaxis, int broflag );
	void Rotate2IKTargetZa4Req( D3DXMATRIX matWorld, int mvno, int broflag );

	int SetParBoneArray( int mvno );

	int OnTwist( D3DXMATRIX matWorld, int twistno, float fdelta, int za4flag );

	int OnBeforeMP();
	int OnNextMP();

	int GetIKLevelOpe();
	int GetMotionNum();
	MOTID* GetMotIDArray();
	int SelectCurrentMotion( int srcindex );

	int NewMotion();
	int DestroyCurrentMotion();

	int SetCurrentMotionPoint( int boneno, CMotionPoint2* srcmp, int hasmpflag );
	int GetRotRad();

	int MultMotion2_0();
	int MultMotion0_5();
	int DeleteInitialKey();
	int DeleteSameKey();
	int SetMotionFrameNo( int motid, int frameno );

	int MotCut();
	int MotMabiki();
	int ChangeCurMotTotalFrame( int newleng );
	int BakeMotion();
	int HCopy();
	int HPaste();

///////////// for mdlg
	int ResetMorphDlg();

///////////// for axisdlg
	int AxisSetInvalidateFlag( int flag );
	int AxisSetRenderParams( float degy, float degxz, GUARDBAND gband, 
		CShdHandler* groundsh, CMotHandler* groundmh, CShdHandler* arrowsh, CMotHandler* arrowmh,
		CD3DFont* pfont );
	int AxisCreateSwapChain( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp );
	int AxisDestroySwapChain();
	float AxisGetArrowDegxz();
	float AxisGetArrowDegy();
	int	AxisGetArrowMode();

///////////// for VIKDlg
	int VIKStopVIK();

///////////// for camdlg
	int GetCameraAnimNum();
	int LoadCameraFile( char* filename );
	MOTID* GetCameraMotIDArray();
	char* GetCameraAnimName( int animno );
	int GetCurCameraAnimIndex();
	int GetCurCameraAnimNo();
	int SelectCameraAnim( int animindex );
	int NewCameraAnim();
	int DestroyCameraAnim( int delanimno );
	int ResetCamTargetBone();
///////////// for gpdlg
	int GetGPAnimNum();
	int LoadGPFile( char* filename );
	MOTID* GetGPMotIDArray();
	char* GetGPAnimName( int animno );
	int GetCurGPAnimIndex();
	int GetCurGPAnimNo();
	int SelectGPAnim( int animindex );
	int NewGPAnim();
	int DestroyGPAnim( int delanimno );
///////////// for moedlg
	int GetMOEAnimNum();
	int LoadMOEFile( char* filename );
	MOTID* GetMOEMotIDArray();
	char* GetMOEAnimName( int animno );
	int GetCurMOEAnimIndex();
	int GetCurMOEAnimNo();
	int SelectMOEAnim( int animindex );
	int NewMOEAnim();
	int DestroyMOEAnim( int delanimno );
	int RemakeMOEAH();
///////////// for snddlg
	int WriteSndAnimFile( char* filename );
	int LoadSndAnimFile( char* filename, int dlgwarnflag );



BEGIN_MSG_MAP(CMotDlg)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)

//	NOTIFY_HANDLER(IDC_TAB1, TCN_SELCHANGE, OnSelchange_tab )
	NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnSelchange_tab )

	MESSAGE_HANDLER(WM_SIZE, OnSizeDlg)
//	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnSelchange_tab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSizeDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);
	//LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int SelectMotParamTab();

	FRAMESAVE* GetFrameSavePtr( int savekeyno );
	int RestoreFrameSave( int savekeyno );
	int SaveFrame( int savekeyno );

private:
	int DestroyTabWindows();
	int ParamsToDlg();
	void SetWnd();
	int CreateTabWindows( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp );
	int CreateTabFont();
	int DestroyTabFont();

	int InitFrameSave();


public:
	CMotParamDlg* m_motparamdlg;
	CAxisDlg*	m_axisdlg;
	CVIKDlg*	m_vikdlg;
	CTexKeyDlg* m_texdlg;
	CDSKeyDlg* m_dsdlg;
	CMorphKeyDlg* m_mdlg;
	CAlpKeyDlg* m_alpdlg;
	CCamKeyDlg* m_camdlg;
	CGPKeyDlg* m_gpdlg;
	CMOEKeyDlg* m_moedlg;
	CSndKeyDlg* m_snddlg;

	CWindow m_tab_wnd;

	int		m_disptabno;
	int		m_createtabflag;
	HFONT	m_hfont;

	FRAMESAVE m_framesave[SAVEKEY_MAX][TAB_MAX];

private:
	CMyD3DApplication* m_app;
	LPDIRECT3DDEVICE9 m_pd3ddev;
	 D3DPRESENT_PARAMETERS m_d3dpp; 
	int		m_cmdshow;
	int tempiklevelope;	


};

#endif //__MOTDLG_H_
