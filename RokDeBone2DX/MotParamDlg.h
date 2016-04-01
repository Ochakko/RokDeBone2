// MotParamDlg.h : CMotParamDlg の宣言

#ifndef __MOTPARAMDLG_H_
#define __MOTPARAMDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>

#include <D3DX9.h>

#include <motionpoint2.h>
#include <quaternion.h>
#include <coef.h>

class CTreeHandler2;
class CShdHandler;
class CShdElem;
class CMotHandler;
//class CMotionPoint2;
class CD3DFont;
class CMyD3DApplication;
class CLoadNameDlg;
class CPasteTypeDlg;
class CMotionCtrl;
class CMKMFile;
class CTexKeyDlg;
class CAlpKeyDlg;
class CDSKeyDlg;
class CGraphRotDlg;
class CMorphKeyDlg;
class CCamKeyDlg;
class CGPKeyDlg;
class CMCHandler;
class CMOEKeyDlg;
class CSndKeyDlg;
class CEQUFile;
class COpTargetDlg;

#define DLGROTRANGEMIN	(-180)
#define DLGROTRANGEMAX	180

typedef struct _tag_hmp
{
	CMotionPoint2 mp;
	_tag_hmp* parhmp;
	_tag_hmp* chilhmp;
	_tag_hmp* brohmp;
}HMP;

typedef struct _tag_undo_buf
{
	int frameno;
	int* seriarray;
	CMotionPoint2* mparray;
}UNDO_BUF;

typedef struct _tag_copyelem
{
	char name[256];
	CMotionPoint2 mp;
}COPYELEM;


/////////////////////////////////////////////////////////////////////////////
// CMotParamDlg
class CMotParamDlg : 
	public CAxDialogImpl<CMotParamDlg>
{
public:
	CMotParamDlg( int iklevelope );
	~CMotParamDlg();
	
	//int SetHandler( long lpth, long lpsh, long lpmh );
	int SetParams( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, HWND papp, CMyD3DApplication* d3dapp );
	int SetAppWnd( HWND appwnd );

	int GetMotParams( int* mcookieptr, int* framenoptr );
	int GetFrameLeng( int motid, int* lengptr );
	int DestroyMotionObj();

	int Redraw();
	int RemakeTreeView( int selno );
	int Rename( int srcseri, char* srcname );

	int OnUserSelchange( int selno );

	int SetDisplayFlag( int dflag );

	int InvalidateHandler();

	int LoadMotion( CLoadNameDlg* namedlg, char* srcfilename, D3DXVECTOR3 srcquamult, int callmoa = 0 );
	int LoadEQUMotion( CEQUFile* equfile, int animcnt, float quamult );
	int ImportMotion( CLoadNameDlg* namedlg );
	int LoadMKM( char* filename, CMKMFile* mkmfile, CMKMFile* inimkm, int motno, float tramult );

	int SetMotionStep( int srcmotstep );
	int LoadBVHFile( char* srcfilename );
	int SaveMotion( CLoadNameDlg* namedlg, char* srcfilename );
	int WriteBmp( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont );
	int WriteAvi( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont );

	int InverseMotionXZ();

	enum { IDD = IDD_MOTPARAMDLG };

	static DWORD WINAPI	ThreadFunc( LPVOID	lpThreadParam );
	int SetNewPose();
	int SetNewPoseByMoa();
	int SetNewPoseSnd();

	int UpdateAllMatrix( int spallkind );
	int UpdateMatrix( int srcseri, int spallkind );
	int UpdateMatrixSelected( int srcseri, int curframeonly, int spallkind );

	int SetCurrentMotion();

	//int SetMotionPointIK( int mvboneno, D3DXVECTOR3 normal, float rad, BOOL needredraw );
	int SetMotionPointIK( int frameno, D3DXMATRIX matWorld, int mvboneno, CQuaternion multq, BOOL needredraw, int* forcemove, CMotionPoint2* calcmp );
	int SetMotionPointIK( int frameno, int mvboneno, float srcmvx, float srcmvy, float srcmvz, CMotionPoint2* calcmp );

	int GetQ( int boneno, CQuaternion* dstq );
	int SetQ( int boneno, CQuaternion srcq );
	int GetEul( int boneno, D3DXVECTOR3* dsteul );
	int SetEul( int boneno, D3DXVECTOR3 srceul );


	int CopyUndoBuffer( int cpframe = -1 );
	int PasteUndoBuffer( int undocnt, int undodlg );

	int ExpandAndShiftMotion( int shiftnum );

	int DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec );

	int GetBonePos( int srcseri, D3DXMATRIX wmat, int mode, D3DXVECTOR3* dstpos );
	int GetBonePosOnFrame( int frameno, int srcseri, D3DXMATRIX wmat, int mode, D3DXVECTOR3* dstpos );
	int GetSelectedJointPos( int* setflag, D3DXVECTOR3* jposptr );

	int ShowDlg( int cmdshow, int stopflag = 1 );

	int GetSelectedJoint( int* selno, char** selname );

	int MoveBone( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent );
	int MoveBone( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 tpos, int* lastparent, int defaxis, D3DXVECTOR3 srcaxis, D3DXVECTOR3* axisdone, int iklevel );
	int MoveBoneZa4( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent );
	int MoveBoneZa4( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 tpos, int* lastparent, int iklevel );

	void Rotate2IKTargetReq( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 rotaxis, int broflag );
	void Rotate2IKTargetZa4Req( D3DXMATRIX matWorld, int mvno, int broflag );


	int ShiftBone( int shiftno, float srcx, float srcy, int t2flag, int za4flag );
	int ShiftBone( int shiftno, int shiftkind, float delta, int t2flag, int localflag, CQuaternion localq, int za4flag );
	int ShiftBone( int frameno, int shiftno, D3DXVECTOR3 targetobj, int* setnoptr, int za4flag, CMotionPoint2* calcmp );
	int ShiftBoneTree( int shiftno, D3DXVECTOR3 targetobj, int za4flag );
//	int ShiftBoneToParent( int shiftno, D3DXVECTOR3 targetobj );

	int GetTopJointNo( int srcseri, int* topseri );
	int ShiftBoneDiff( int frameno, int shiftno, D3DXVECTOR3 diff, CMotionPoint2* calcmp );


	int ScaleBone( int scaleno, int scalekind, float delta );





	int DestroyTimer();

	int RotateBone( D3DXMATRIX matWorld, int mvno, int rotkind, float delta, int* lastparent, int localflag, CQuaternion localq, int ikfkflag, int za4flag );

	int UpdateRadioDlg();
	int UpdateComboDlg();


	int CopyLowerMotion( CShdElem* srcselem );

	int GetRangeMax( int motid, int* rangemax );
	int GetKeyFrameNo( int motid, int* keyframe, int* framenum );

	int InitParBoneArray();
	int SetParBoneArray( int mvno );
	int ExistInParBoneArray( int chkno );

	int OnTwist( D3DXMATRIX matWorld, int twistno, float fdelta, int za4flag );

	int SetSymMotionPoint( CShdElem* srcselem, int symmtype );

	int SelectCurrentMotion( int srcindex );
	int InitDlg();
	int DestroyCurrentMotion();

	int SetCurrentMotionPoint( int boneno, CMotionPoint2* srcmp, int hasmpflag );

	int GetRotRad();

	int MultMotion2_0();
	int MultMotion0_5();
	int MotCut();
	int MotMabiki();


	int DeleteInitialKey();
	int DeleteSameKey();

	int SetMPbyEul( int boneno, CMotionPoint2* mpptr, D3DXVECTOR3 neweul );
	int SetFillUpMode( int setseri, int setframe, int setfu );
	int SetMotionFrameNo( int motid, int frameno );
	int PlayAnim( int srcframe, int totalflag = 0 );

	int OnJumpFrame( int srcframe );
	int SetStartAndEnd( int srcstart, int srcend );
	int GetStandardQ( int boneno, CQuaternion* dstq );

	int CalcSplineParam( int allmkind, int calcseri, int motcookie = -1 );

BEGIN_MSG_MAP(CMotParamDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)

	//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_PREVIEW, OnPreview)
	COMMAND_ID_HANDLER(IDC_FRAMECOPY, OnFrameCopy)
	COMMAND_ID_HANDLER(IDC_FRAMECOPYONE, OnFrameCopyOne)
	COMMAND_ID_HANDLER(IDC_FRAMECOPYALL, OnFrameCopyAll)
	COMMAND_ID_HANDLER(IDC_FRAMECOPYARI, OnFrameCopyAri)
	COMMAND_ID_HANDLER(IDC_FRAMEPASTE, OnFramePaste)
	COMMAND_ID_HANDLER(IDC_FRAMEPASTE2, OnFramePasteSym)
	COMMAND_ID_HANDLER(IDC_FRAMEDEL, OnFrameDel)
	COMMAND_ID_HANDLER(IDC_PARAMSET, OnParamSet)
	COMMAND_ID_HANDLER(IDC_STOP, OnStop)
	COMMAND_ID_HANDLER(IDC_ALLDELETE, OnAllDelete)
//	COMMAND_ID_HANDLER(IDC_SELLOCK_START, OnSellockStart)
//	COMMAND_ID_HANDLER(IDC_SELLOCK_STOP, OnSellockStop)

	COMMAND_ID_HANDLER(IDC_RESET_ROT, OnResetRot)
	COMMAND_ID_HANDLER(IDC_RESET_MV, OnResetMv)
	COMMAND_ID_HANDLER(IDC_RESET_SC, OnResetSc)
	

//	COMMAND_ID_HANDLER(IDC_DISPSWITCH, OnDispSwitch)
	COMMAND_ID_HANDLER(IDC_BONEDISP, OnBoneDisp)
	COMMAND_ID_HANDLER(IDC_IKTDISP, OnIKTDisp)

	COMMAND_ID_HANDLER(IDC_RADIO_MODEJOINT, OnRadioModeJoint)
	COMMAND_ID_HANDLER(IDC_RADIO_MODEOPE, OnRadioModeOpe)

	COMMAND_ID_HANDLER(IDC_BEFORE_MP, OnBeforeMP)
	COMMAND_ID_HANDLER(IDC_NEXT_MP, OnNextMP)

	COMMAND_ID_HANDLER(IDC_CREATEKEY, OnCreateKey)
	COMMAND_ID_HANDLER(IDC_CREATEKEYALL, OnCreateKeyAll)
	COMMAND_ID_HANDLER(IDC_FRAMESHIFT, OnFrameShift)

	COMMAND_ID_HANDLER(IDC_NEWROT, OnNewRot)
	COMMAND_ID_HANDLER(IDC_OLDROT, OnOldRot)

	COMMAND_ID_HANDLER(IDC_OPTARGET, OnOpTarget)
	COMMAND_ID_HANDLER(IDC_SOUTAI, OnSoutai)
	COMMAND_ID_HANDLER(IDC_ZETTAI, OnZettai)

	COMMAND_ID_HANDLER(IDC_APPLYSTART, OnApplyStart)
	COMMAND_ID_HANDLER(IDC_APPLYRANGE, OnApplyRange)

	COMMAND_ID_HANDLER(IDC_NCRE, OnNotCreate)

	NOTIFY_HANDLER(IDC_SL_MP, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_mp)
	NOTIFY_HANDLER(IDC_SL_RANGE, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_range)

	NOTIFY_HANDLER(IDC_SP_MP, UDN_DELTAPOS, OnSpin_Sp_mp )
	//NOTIFY_HANDLER(IDC_SP_BONENUM, UDN_DELTAPOS, OnSpin_Sp_bonenum)
	NOTIFY_HANDLER(IDC_SP_TWIST, UDN_DELTAPOS, OnSpin_Sp_twist)
	
	NOTIFY_HANDLER(IDC_SP_ROTX, UDN_DELTAPOS, OnSpin_Sp_rotx)
	NOTIFY_HANDLER(IDC_SP_ROTY, UDN_DELTAPOS, OnSpin_Sp_roty)
	NOTIFY_HANDLER(IDC_SP_ROTZ, UDN_DELTAPOS, OnSpin_Sp_rotz)
	NOTIFY_HANDLER(IDC_SP_ROTRAD, UDN_DELTAPOS, OnSpin_Sp_rotrad)

	//NOTIFY_HANDLER(IDC_COMBO_LEVEL, CBN_SELCHANGE, OnCombo_level)//--->なぜか、WM_COMMANDじゃないと、つかまえられなかった。
	//NOTIFY_HANDLER(IDC_COMBO_LEVEL, CBN_SELENDOK, OnCombo_level)
	//NOTIFY_HANDLER(IDC_COMBO_LEVEL, LBN_SELCHANGE, OnCombo_level)

//	NOTIFY_HANDLER(IDC_TREE_MOT, TVN_SELCHANGED, OnSelchangedTree_mot)


	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)

	MESSAGE_HANDLER(WM_MOVING, OnMoving)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonUP)

	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	MESSAGE_HANDLER(WM_COMMAND, OnCusCommand)

END_MSG_MAP()

	//BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);


// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFrameCopyOne(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFrameCopyAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFrameCopyAri(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFramePasteSym(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFrameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnParamSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSellockStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSellockStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnResetRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnResetMv(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnResetSc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

//	LRESULT OnDispSwitch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBoneDisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnIKTDisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnRadioModeJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioModeOpe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCreateKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreateKeyAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	int BakeMotion();

	LRESULT OnFrameShift(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNewRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnOldRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnOpTarget(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSoutai(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZettai(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnApplyStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyRange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnNotCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_range(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	//LRESULT OnSpin_Sp_bonenum(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpin_Sp_twist(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSpin_Sp_rotx(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpin_Sp_roty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpin_Sp_rotz(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpin_Sp_rotrad(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnCombo_level(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

//	LRESULT OnSelchangedTree_mot(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCusCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	int DestroyFUMotion();
	int CreateFUMotion();

	int ChangeCurMotTotalFrame( int newleng );
	int ConvSymMotion();

	int HCopy();
	int HPaste();

private:
	
	int FillTree();
//	void AddBoneToTree( int srcseri, int addbroflag, int addtolast );
//	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

//	void CreateImageList();
	int SetWnd();
	int ParamsToDlg();
	int ParamsFromMotionPoint( CMotionPoint2* srcmp );

	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int DrawMotionPoints();
	int DrawRange();
	int UpdateAllWnd();

	int InitRotAndMv();

	int InitCEBuff();

	int InvalidateTreeView();
	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );
	int Frameno2WindowposRange( RECT clirect, int frameno, int* posptr );


	void ErrorMes( int errorcode, int type, char* mes );

	int SetNewWindowProc();

	int CreateMotionPoints( int srcseri, int motcookie, int frameno );
//	int CreateLackedMotionPoints();

	int InitUndoBuffer();
	int DestroyUndoBuffer();

	int SetRotation( D3DXVECTOR3 axis, int delta, int mvboneno );

	int GetPasteMP( COPYELEM* cebuf, int* pastelist, int serino, CMotionPoint2* retmpptr );
	//int SetSymMotionPoint( int symmtype, CMotionPoint2* srcmp, CMotionPoint2* dstmp, int deginvflag );//!!!SymmOpe.cppへ移動


	int GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir );

	int CopyToClipBoardData( int shdnum, int cpframenum, COPYELEM* srcce, HGLOBAL* cbhgptr );
//	int PasteFromClipBoardData( COPYELEM* dstce, HGLOBAL cbhg );
	int PasteFromClipBoardData( COPYELEM* dstce, int* bufshdnum, int* bufframenum, HGLOBAL cbhg );
	int PasteOnFrame( int pastetype, int* pastelist, int pasteframe );
	int PasteOnFrameOne( int serino, CMotionPoint2* cpmp, int motid, int pasteframe );


	int Bin2Char( unsigned char* srcptr, char* dstptr, int srclen );
	int Char2Hex( char* orgdata, unsigned char* donedata, DWORD charleng );
	int ConvData( char* charptr, unsigned char* ucptr, int pos );

	int GetIKTopParent( int srcseri, int maxlevel, int* topparptr );
	int Conv5Digits( int srcseri, char* dstptr );

	void FindFirstJoint( CShdElem* curselem, CShdElem** findelem );
	int CheckRotKindButton();
	int CheckPreviewButton();
	int CheckSoutaiButton();

	int PreviewMoa();
	int StopMoa();

//	int GetKeyboardCnt();
//	int ResetKeyboardCnt();

	int SetQOpTarget( int boneno, CQuaternion multq, CQuaternion totalq, int skipupdate );
	void DestroyHMPReq( HMP* curhmp );
	void CreateHMPReq( CShdElem* septr, HMP* hmpptr, HMP* parhmp );
	void PasteHMPReq( CShdElem* septr, HMP* hmpptr );

public:
	HMP* m_hmp;

	int m_loadingflag;
	CMyD3DApplication* m_d3dapp;

	CTreeHandler2* m_thandler;
	CShdHandler* m_shandler;
	CMotHandler* m_mhandler;

	HWND	m_apphwnd;

	COpTargetDlg* m_optardlg;

	int m_savebmdisp;
	int m_savejdisp;

	//int m_rotx, m_roty, m_rotz;
	float m_mvx, m_mvy, m_mvz;
	LONG m_current;
	LONG m_previewmotion;
	LONG m_previewcamera;
	LONG m_previewgp;
	LONG m_previewmoa;
	LONG m_previewmoe;
	LONG m_previewsnd;
	LONG m_withsndflag;
	int m_sndframe;


	float m_scalex, m_scaley, m_scalez;//2005/04/24
	int m_userint1;

	//D3DXVECTOR3 m_axis;
	//float m_phai;
	CQuaternion m_q;

	DWORD m_dispswitch;
	int m_interp;

	LONG m_mot_cookie;
	char m_motionname[256];
	int m_motiontype;
	int m_motionjump;
	int m_interpolation;

//	int m_motidarray[ MAXMOTIONNUM ];
	MOTID m_motid[ MAXMOTIONNUM ];
	int m_motionnum;


	int m_displayflag;
	int m_ismoving;
	int m_remaking_tree;//remake中のWM_USER_SELCHANGE発行を避けるためのフラグ。

	int m_sellockno;
	//int m_ik_bonenum;

	int m_ikmode;
	int m_iklevel;
	int m_iklevelope;

	int	m_parbonearray[MAXBONENUM + 1];

	LONG m_preview_flag;

	LONG m_startframe;
	LONG m_endframe;
	LONG m_range;
	LONG m_previewframe;

	int m_selectedno;
	int m_sl_mp_rangemax;

	CTexKeyDlg* m_texkeydlg;
	CAlpKeyDlg* m_alpkeydlg;
	CDSKeyDlg* m_dskeydlg;
	CGraphRotDlg* m_grotdlg;
	CMorphKeyDlg* m_MorphKeyDlg;
	CCamKeyDlg* m_camkeydlg;
	CGPKeyDlg* m_gpkeydlg;
	CMOEKeyDlg* m_moekeydlg;
	CMCHandler* m_mch;
	CSndKeyDlg* m_sndkeydlg;

	CWindow m_sl_mp_wnd;

	int m_ctrldrag;
	int m_altdrag;
	int m_dragx;

	int m_timerworking;

private:
//	HIMAGELIST m_hImageList;
//	int m_iImage;
//	int m_iSelect;

//	HTREEITEM* m_TI;
//	HTREEITEM m_selecteditem;

	//CMotionPoint2* m_MPBuff;
	COPYELEM* m_CEBuff;
	CMotionPoint2* m_dragmp;
	int m_dragframe;


	int m_invalidate_handler;
	//int m_preview_flag;
	//LONG m_preview_flag;

	CWindow m_dlg_wnd;
//	CWindow m_tree_mot_wnd;
	CWindow m_tex_current_wnd;

	CWindow m_mp_wnd;
//	CWindow m_sl_mp_wnd;//publicへ
	CWindow m_sp_mp_wnd;

	CWindow m_paramset_wnd;
	CWindow m_framecopy_wnd;
	CWindow m_framepaste_wnd;
	CWindow m_framedel_wnd;

	CWindow m_preview_wnd;
	CWindow m_stop_wnd;
	CWindow m_alldelete_wnd;

//	CWindow m_sellock_name_wnd;
//	CWindow m_sellock_start_wnd;
//	CWindow m_sellock_stop_wnd;

	//CWindow m_tex_bonenum_wnd;
	//CWindow m_sp_bonenum_wnd;

	CWindow m_sp_twist_wnd;

	CWindow m_reset_rot_wnd;
	CWindow m_reset_mv_wnd;

	CWindow m_sp_rotx_wnd;
	CWindow m_sp_roty_wnd;
	CWindow m_sp_rotz_wnd;
	CWindow m_sp_rotrad_wnd;
	CWindow m_tex_rotrad_wnd;
	
	CWindow m_bonedisp_wnd;

	CWindow m_combo_iklevel_wnd;
	CWindow m_radio_modejoint_wnd;
	CWindow m_radio_modeope_wnd;

	CWindow m_newrot_wnd;
	CWindow m_oldrot_wnd;
	int m_rotkind;

	int m_rotrad;

	CWindow m_soutai_wnd;
	CWindow m_zettai_wnd;
	int m_soutai;

	CWindow m_range_wnd;
	CWindow m_sl_range_wnd;
	CWindow m_editrange_wnd;
	CWindow m_rangestart_wnd;


	//CWindow m_ok_wnd;
	//CWindow m_cancel_wnd;

	HPEN	m_black_pen;
	HPEN	m_red_pen;
	HPEN	m_blue_pen;
	HPEN	m_gray_pen;
	HBRUSH	m_black_brush;
	HBRUSH	m_white_brush;
	HBRUSH	m_red_brush;
	HBRUSH	m_blue_brush;
	HBRUSH	m_gray_brush;

	int m_dragflag;


	UNDO_BUF m_undobuf[MAXUNDONUM];

	int m_cmdshow;


	int m_underdestroyflag;
	int graymovestart;
	int graymove;

	int m_dsfillupmode;
	int m_texfillupmode;

	int m_calcsp;

	LONG m_defframerange;

	int m_notcreate;
};

#endif //__MOTPARAMDLG_H_
