#ifndef VIEWERH
#define VIEWERH

#include "D3DApp.h"

#include <atlctl.h>
#include <coef.h>

#include "quaternion.h"

#include "inidata.h"
//#include <xfont.h>

class CTreeHandler2;
class CShdHandler;
class CMotHandler;

class CD3DFont;

class CCameraDlg;
class CSigEditDlg;
class CSigEditWnd;
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
class CToon1Dlg;
class CMotChangeDlg;
class CWriteMAFDlg;
class CMaterialDlg;
class CBonePropDlg;

class CBgDisp2;
class CShdElem;
class CLightData;

class CPluginElem;

#define MAXPLUGIN	255



class CMyD3DApplication : public CD3DApplication
{
	LPD3DXSPRITE		m_d3dxsprite;

    CD3DFont*          m_pFont;
    CD3DFont*          m_pFontSmall;

	int					m_dirlightid;

    BYTE               m_bKey[256];
	int				   m_exit;
	HWND				m_pbwnd;

    HRESULT ConfirmDevice( D3DCAPS9*, DWORD, D3DFORMAT );

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

	CTreeHandler2*			m_g_th;
	CShdHandler*			m_g_sh;
	CMotHandler*			m_g_mh;

	CTreeHandler2*			m_ext_th;
    CShdHandler*			m_ext_sh;
	CMotHandler*			m_ext_mh;

	int						m_sel_xno;//"obj_X"
	int						m_sel_yno;//"obj_Y"
	int						m_sel_zno;//"obj_Z"
	int						m_sel_cenno;//"obj_CENTER"

	int						m_select_kind;
	//int						m_selectedno;//---> public‚Ö
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
	CCameraDlg*				m_cameradlg;
	//CSigEditDlg*			m_sigeditdlg;// <--- public‚Ö
	COptDlg*				m_optdlg;
	CParsDlg*				m_parsdlg;
	CBoneLineDlg*			m_bonelinedlg;
	CLoadNameDlg*			m_loadnamedlg;
	CItiModeDlg*			m_itimodedlg;
	//CItiDlg*				m_itidlg;//<-- public‚Ö
	CLightDlg*				m_lightdlg;
	CSetColDlg*				m_setcoldlg;
	CCreateBoneDlg*			m_bonedlg;
	CInfScopeDlg*			m_infscopedlg;
	CPaintDlg*				m_paintdlg;
	CToon1Dlg*				m_toon1dlg;
	//CMotChangeDlg*			m_mcdlg;//<-- public‚Ö
	CWriteMAFDlg*			m_wmafdlg;
	CMaterialDlg*			m_materialdlg;
	//CBonePropDlg*			m_bonepropdlg;//<-- global‚Ö


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

	int						m_control_key;
	int						m_shift_key;
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

	int						m_ikcalccnt;

	int						m_tbb_r;
	int						m_tbb_t;
	int						m_tbb_t2;
	int						m_tbb_s;
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
	int						m_tbb_mo;

	int						m_tbb_face;
	int						m_tbb_line;
	int						m_tbb_point;
	int						m_tbb_ten2;

	int						m_tbb_za1;
	int						m_tbb_za2;
	int						m_tbb_za3;

	//int						m_tbb_colR;
	int						m_tbb_colP;
	//int						m_tbb_colB;
	//int						m_tbb_colS;
	//int						m_tbb_colM;
	int						m_tbb_colT;

	//int						m_tbb_ofc;
	//int						m_tbb_ofs;
	//int						m_tbb_ofo;

	int						m_za_kind;


	CQuaternion				m_localq;
	int						m_g_disp;

	CPluginElem*			m_plugin;
	D3DXVECTOR3				m_vEyePt;

	float					m_wheeldelta;


	//int SetSkipMatrix();
	//int UpdateVertexBuffer();

	int LoadSigFile();
	//int LoadFBXFile();
	int LoadXFile();
	int LoadRokFile();
	int LoadMQOFile( LOADFILEINFO* ldinfo );
	int LoadGroundData();
	int ImportSigFile();
	int ImportRokFile();
	int ImportMqoFile( LOADFILEINFO* ldinfo );
	int SaveSigFile( char* filename );
	int SaveXFile();
	int ExtractBoneData();

	int InvalidateHandler( int resetflag = 0 );
	int InvalidateGroundHandler( int resetflag = 0 );
	int InvalidateGHandler( int resetflag = 0 );
	int RestoreGroundHandler();
	int RestoreGHandler();
	int RestoreExtHandler( int needcalc );
	int SetRESDIR( char* pathname, CShdHandler* srclpsh );
	//int SetModuleDir();

	int ChangeViewParam( int keyflag, float step = 1000000.0f );

	int CheckTBB();

	int SaveLimFile();
	int LoadLimFile();

	int SaveBVHFile();

protected:
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects( int needcalc, int resetflag = 0 );
    HRESULT InvalidateDeviceObjects( int resetflag = 0 );
    HRESULT DeleteDeviceObjects();
    HRESULT Render();
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
	int DelJoint( int delno, int restoreflag = 1 );
	int Conv2Scope( int srcboneno, int srcdispno, int srcconvno );
	int Add2Tree( char* newname, int serino, int srctype, int depth, int hasmotion );

	int SetBg();
	int SetLight( int modalflag );
	int SetBoneDisp();
	int ShowVersionDlg();

	int DestroyMotion( int allflag );
	int NewMotion();

	int PickBone( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int selaxis );
	int PickIKT( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr );
	int PickVert( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart, int* pickvert, int* pickvertnum, int selaxis );
	int PickFace( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart, int* pickface );
	int PickDispObj( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int* pickpart );

	int PickInfScopeFace( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int isindex, int* pickface );
	int PickInfScopeVert( WORD srcx, WORD srcy, int* selectptr, POINT* offsetptr, POINT* clickboneptr, int isindex, int* pickvert, int* pickvertnum );

	//int MoveBone( int mvno, float srcx, float srcy, int* lastparent );
	//int ShiftBone( int shiftno, float srcx, float srcy );
	//int GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir );


	void SetBoneArrayReq( CShdElem* selem, int* setnum, int broflag );

	int CreateProgressBar();
	int SetProgressBar( int newpos );
	int DestroyProgressBar();

	int WriteBoneFile();
	int LoadBoneFile( LOADFILEINFO* ldinfo );

	int WriteBIMFile();
	int LoadBIMFile();

	int WriteIM2File();
	int LoadIM2File( int reloadflag );

	int WriteMQOFile();

	int WriteMAFile( char* dirname, char* moaname );
	int LoadMAFile();

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
	int IKTOFF();

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

	int OverWriteFiles();

	int MakeAllJointTreeFlat( CShdElem** ppfirstparent );
	int FindSeriByExtSeri( SERICONV* sericonv, int convnum, int extseri, CShdElem** ppselem );

	int SelChangeBone( WORD menuid );

	int ConvSymmXShape();
	int ConvSymmXMotion();

	int ConvNormalMap();
	int LoadMKM();

	int Conv5Digits( int srcseri, char* dstptr );
	int SetShaderConst();
	CLightData* GetLightData( int lid );
	int GetShaderHandle();

public:
    CMyD3DApplication();

	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	int ScreenToObj( D3DXVECTOR3* objpos, D3DXVECTOR3 scpos );
    HRESULT FrameMove( int srcseri = 0, int tra_boneonly = 0 );
	int SetBoneArray( int srcseri );
	int UpdateVertexBuffer();
	int CopyUndoBuffer();

	int RestoreHandler( int needcalc, int dispwarnflag = 1 );
	int TempPickVert( int* shiftpart, int* shiftvert, int* shiftvertnum );

//	int LoadMotion();
	int LoadMotion( char* filename, float quamult, int idling, int ev0idle, int commonid, int borbidnum, int* forbidid );
	int LoadMotionMulti();

	//int ChangeMotionStep();
	//int SetMotionStep();

public:
	CTreeHandler2*			m_thandler;
    CShdHandler*			m_shandler;
	CMotHandler*			m_mhandler;

	CTreeHandler2*			m_sph_th;
	CShdHandler*			m_sph_sh;
	CMotHandler*			m_sph_mh;


	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matView;
	//D3DXMATRIX				m_matProj;
	D3DXMATRIX				m_matProjX;

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
	CItiDlg*				m_itidlg;
	CMotChangeDlg*			m_mcdlg;

	//char*					m_tempnameptr;
};

#endif
