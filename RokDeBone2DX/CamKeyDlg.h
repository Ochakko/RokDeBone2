// CamKeyDlg.h : CCamKeyDlg の宣言

#ifndef __CamKeyDlg_H_
#define __CamKeyDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <d3dx9.h>
#include <commctrl.h>

#include <coef.h>

class CMyD3DApplication;
class CMotParamDlg;
class CCameraAnimHandler;
class CCameraAnim;
class CCameraKey;
class CCameraSWKey;
class CTargetBoneDlg;

class CShdHandler;

/////////////////////////////////////////////////////////////////////////////
// CCamKeyDlg
class CCamKeyDlg : 
	public CAxDialogImpl<CCamKeyDlg>
{
public:
	enum { IDD = IDD_CAMERAKEYDLG };


	CCamKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp );
	~CCamKeyDlg();
	
	int SetAppWnd( HWND appwnd );
	int Redraw();
	//int OnUserSelchange();
	int ShowDlg( int cmdshow, int stopflag = 1 );
	int DestroyTimer();
	int InitDlg( int atshowflag );
	int ResetParams( int atshowflag );
	int GetCurCamera( CAMERAELEM* dstcamera );

	int GetCameraAnimNum();
	int LoadCameraFile( char* filename );

	MOTID* GetCameraMotIDArray();
	char* GetCameraAnimName( int animno );
	int GetCurCameraAnimIndex();
	int GetCurCameraAnimNo();
	int SelectCameraAnim( int animindex );

	int DestroyCameraAnim( int delanimno );

	int SetCurCamera( CAMERAELEM srccamera );
	int SetPreviewButton();

	int CalcMotionPointOnFrame( int animno, int camno, int frameno, CCameraKey* ckptr, int* hasmpflag );
	int ExistCameraKey( int animno, int camno, int frameno, CCameraKey** ppck );
	int SetFillUpMode( int animno, int camno, int frameno, int setfu );

	int PlayAnim( int srcframe, int totalflag = 0 );
	int SetCameraKAE();

	int SetStartAndEnd( int srcstart, int srcend );

	int OnUserSelchange( int selno );
	int ResetCamTargetBone();

BEGIN_MSG_MAP(CCamKeyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)

	//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_PREVIEW, OnPreview)
	COMMAND_ID_HANDLER(IDC_STOP, OnStop)
	COMMAND_ID_HANDLER(IDC_COPY, OnFrameCopy)
	COMMAND_ID_HANDLER(IDC_PASTE, OnFramePaste)

	COMMAND_ID_HANDLER(IDC_CRE1, OnCreate0)
	COMMAND_ID_HANDLER(IDC_CRE2, OnCreate1)
	COMMAND_ID_HANDLER(IDC_CRE3, OnCreate2)

	COMMAND_ID_HANDLER(IDC_DEL1, OnDel0)
	COMMAND_ID_HANDLER(IDC_DEL2, OnDel1)
	COMMAND_ID_HANDLER(IDC_DEL3, OnDel2)

	COMMAND_ID_HANDLER(IDC_BEFORE_MP, OnBeforeMP)
	COMMAND_ID_HANDLER(IDC_NEXT_MP, OnNextMP)

	COMMAND_ID_HANDLER(IDC_SELHEN, OnSelHen)
	COMMAND_ID_HANDLER(IDC_SEL1, OnSel1)
	COMMAND_ID_HANDLER(IDC_SEL2, OnSel2)
	COMMAND_ID_HANDLER(IDC_SEL3, OnSel3)
	COMMAND_ID_HANDLER(IDC_SELKAE, OnSelKae)

	COMMAND_ID_HANDLER(IDC_EDITCAM, OnEditCam)
	COMMAND_ID_HANDLER(IDC_EDITSW, OnEditSW)

	COMMAND_ID_HANDLER(IDC_VPROJ, OnProjPers)
	COMMAND_ID_HANDLER(IDC_VORTHO, OnProjOrtho)
	

//	COMMAND_ID_HANDLER(IDC_COMBO_MODE, OnComboMode)
//	COMMAND_ID_HANDLER(IDC_COMBO_BONE, OnComboBone)
	COMMAND_ID_HANDLER(IDC_COMBO_G, OnComboG)

	COMMAND_ID_HANDLER(IDC_SETTING, OnSetting)

	COMMAND_ID_HANDLER(IDC_APPLYDIST, OnApplyDist)
	COMMAND_ID_HANDLER(IDC_APPLY, OnApply)
	COMMAND_ID_HANDLER(IDC_APPLYDRAG, OnApplyDrag)

	COMMAND_ID_HANDLER(IDC_FILLUPUP, OnFillupUp)
	COMMAND_ID_HANDLER(IDC_CHUCHU, OnChuChu)


	NOTIFY_HANDLER(IDC_SP_MP, UDN_DELTAPOS, OnSpin_Sp_mp )
	NOTIFY_HANDLER(IDC_SL_MP, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_mp)


	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_MOVING, OnMoving)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)

	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterMove)
	MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitMove)

//ALT_MSG_MAP(2)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)

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
	LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCreate0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreate1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreate2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDel0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDel1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDel2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelHen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSel1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSel2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSel3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelKae(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnEditCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditSW(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnProjPers(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnProjOrtho(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

//	LRESULT OnComboMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//	LRESULT OnComboBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnComboG(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyDist(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyDrag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFillupUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChuChu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int ParamsToDlg();
	int SetNewCamera();
	int SetCurrentMotion( int isfirst );
	int OnJumpFrame( int srcframe );
	int ToggleOrtho();

	int SetCameraButton();
	int SetEditButton();
	int SetProjButton();


private:
	int InitParams();
	int DestroyObjs( int delcah );

	int SetWnd();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int DrawCameraKeys();
	int DrawSWKeys();

	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );

	int SetNewWindowProc();

	int InitCombo();
//	int InitBone2Combo();

	int CaptureM();


	int DeleteKey( int camno );
	int CreateKey( int camno );

	int CreateKeyNo();
	int DestroyKeyNo();
	int InitKeyNo();
	int SetKeyNo();


	int ApplyParams( int distflag );
	int TargetToBone( CAMERAELEM* dstcamera );

public:
	CCameraAnimHandler* m_cah;
	LONG m_current;
	LONG m_startframe;
	LONG m_endframe;
	int m_sl_mp_rangemax;
	LONG m_previewframe;

	int m_ismoving;
	CWindow m_sl_mp_wnd;
	LONG m_animno;
	int m_camerabutton;

	int m_gtype;
	int m_ortho;

	int m_ctrldrag;
	int m_altdrag;
	int m_dragx;

	int m_savebmdisp;
	int m_savejdisp;

	int m_fillupup;

	CAMERAELEM m_camera;

private:
	CMotParamDlg* m_dlg;
	CMyD3DApplication* m_app;	
	CTargetBoneDlg* m_targetdlg;

	CCameraKey* m_dragck;
	int m_dragcamno;
	int m_dragframe;
	int m_dragflag;
	CCameraKey* m_cpck;

	CCameraSWKey* m_dragcswk;
	int m_swdragcamno;
	int m_swdragframe;
	int m_swdragflag;
	CCameraSWKey* m_cpcswk;

	int m_editbutton;
	MOTID m_motid[ MAXMOTIONNUM ];


	CWindow m_dlg_wnd;
	CWindow m_tex_current_wnd;

	CWindow m_selhen_wnd;
	CWindow m_sel1_wnd;
	CWindow m_sel2_wnd;
	CWindow m_sel3_wnd;
	CWindow m_selkae_wnd;

//	CWindow m_combomode_wnd;
//	CWindow m_combobone_wnd;
	CWindow m_combog_wnd;

	CWindow m_mp_wnd;
	CWindow m_sw_wnd;
	CWindow m_sp_mp_wnd;

	CWindow m_tarx_wnd;
	CWindow m_tary_wnd;
	CWindow m_tarz_wnd;

	CWindow m_posx_wnd;
	CWindow m_posy_wnd;
	CWindow m_posz_wnd;

	CWindow m_dist_wnd;

	CWindow m_upx_wnd;
	CWindow m_upy_wnd;
	CWindow m_upz_wnd;

	CWindow m_near_wnd;
	CWindow m_far_wnd;
	CWindow m_fov_wnd;

	CWindow m_preview_wnd;
	CWindow m_stop_wnd;

	CWindow m_editcam_wnd;
	CWindow m_editsw_wnd;

	CWindow m_pers_wnd;
	CWindow m_ortho_wnd;
	
	CWindow m_drag_wnd;

	HPEN	m_black_pen;
	HPEN	m_red_pen;
	HPEN	m_blue_pen;
	HBRUSH	m_black_brush;
	HBRUSH	m_white_brush;
	HBRUSH	m_red_brush;
	HBRUSH	m_blue_brush;


	int m_timerworking;
	int m_cmdshow;
	int m_mcap;
	int m_moving;

//	int* m_bone2combo;
//	int* m_combo2bone;
//	int m_indexleng;
//	int m_jointnum;


	int* m_keyno[ CAMERANUMMAX ];
	int* m_totalkeyno;
	int* m_swkeyno;
	int m_swkeygetnum;
	int m_keynoleng;
};

#endif //__CamKeyDlg_H_
