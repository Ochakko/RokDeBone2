// GPKeyDlg.h : CGPKeyDlg の宣言

#ifndef __GPKeyDlg_H_
#define __GPKeyDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <D3DX9.h>
#include <commctrl.h>

#include <coef.h>
#include <quaternion.h>

class CMyD3DApplication;
class CMotParamDlg;
class CGPAnimHandler;
class CGPAnim;
class CGPKey;

class CShdHandler;

/////////////////////////////////////////////////////////////////////////////
// CGPKeyDlg
class CGPKeyDlg : 
	public CAxDialogImpl<CGPKeyDlg>
{
public:
	enum { IDD = IDD_GPAKEYDLG };


	CGPKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp );
	~CGPKeyDlg();
	
	int SetAppWnd( HWND appwnd );
	int Redraw();
	//int OnUserSelchange();
	int ShowDlg( int cmdshow, int stopflag = 1 );
	int DestroyTimer();
	int InitDlg( int atshowflag );
	int ResetParams( int atshowflag );
	int GetCurGP( GPELEM* dstgpe );

	int GetGPAnimNum();
	int LoadGPFile( char* filename );

	MOTID* GetGPMotIDArray();
	char* GetGPAnimName( int animno );
	int GetCurGPAnimIndex();
	int GetCurGPAnimNo();
	int SelectGPAnim( int animindex );

	int DestroyGPAnim( int delanimno );

	int SetCurGP( GPELEM srcgpe );
	int SetPreviewButton();

	int CalcMotionPointOnFrame( int animno, int frameno, CGPKey* gpkptr, int* hasmpflag );
	int ExistGPKey( int animno, int frameno, CGPKey** ppgpk );
	int SetFillUpMode( int animno, int frameno, int setfu );
	int CalcFrameData();

	int GetCalcGPKey( int frameno, CGPKey* pgpk );

	int RotateGP( int axiskind, float delta, CQuaternion localq );
	int ShiftGP( int axiskind, float delta, CQuaternion localq );

	int PlayAnim( int srcframe, int totalflag = 0 );

BEGIN_MSG_MAP(CGPKeyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)

	//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_PREVIEW, OnPreview)
	COMMAND_ID_HANDLER(IDC_STOP, OnStop)
	COMMAND_ID_HANDLER(IDC_FRAMECOPY, OnFrameCopy)
	COMMAND_ID_HANDLER(IDC_FRAMEPASTE, OnFramePaste)

	COMMAND_ID_HANDLER(IDC_BEFORE_MP, OnBeforeMP)
	COMMAND_ID_HANDLER(IDC_NEXT_MP, OnNextMP)

	COMMAND_ID_HANDLER(IDC_FRAMEDEL, OnDelete)
	COMMAND_ID_HANDLER(IDC_CREATEKEY, OnCreate)
	COMMAND_ID_HANDLER(IDC_ALLDELETE, OnAllDelete)

	COMMAND_ID_HANDLER(IDC_COMBOG, OnComboG)
	COMMAND_ID_HANDLER(IDC_COMBOONG, OnComboOnG)
	COMMAND_ID_HANDLER(IDC_COMBOPLANE, OnComboPlane)

	COMMAND_ID_HANDLER(IDC_SETTING, OnSetting)

	COMMAND_ID_HANDLER(IDC_APPLY, OnApply)
	COMMAND_ID_HANDLER(IDC_DOROT, OnDoRot)
	COMMAND_ID_HANDLER(IDC_CALC, OnCalc)
	COMMAND_ID_HANDLER(IDC_APPLYDRAG, OnApplyDrag)

	COMMAND_ID_HANDLER(IDC_CHKHEN, OnChkHen)

	NOTIFY_HANDLER(IDC_SP_MP, UDN_DELTAPOS, OnSpin_Sp_mp )
	NOTIFY_HANDLER(IDC_SL_MP, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_mp)

	NOTIFY_HANDLER(IDC_SPINROTX, UDN_DELTAPOS, OnSpin_RX )
	NOTIFY_HANDLER(IDC_SPINROTY, UDN_DELTAPOS, OnSpin_RY )
	NOTIFY_HANDLER(IDC_SPINROTZ, UDN_DELTAPOS, OnSpin_RZ )



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

	LRESULT OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnComboG(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnComboOnG(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnComboPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyDrag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDoRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnChkHen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSpin_RX(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpin_RY(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpin_RZ(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


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
	int SetNewGP();
	int SetCurrentMotion( int isfirst );
	int OnJumpFrame( int srcframe );
	int SetStartAndEnd( int srcstart, int srcend );

private:
	int InitParams();
	int DestroyObjs( int delgpah );

	int SetWnd();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int DrawGPKeys();

	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );

	int SetNewWindowProc();

	int InitCombo();

	int CaptureM();


//	int DeleteKey( int camno );
//	int CreateKey( int camno );

//	int ApplyParams();
	int Deg2Q( D3DXVECTOR3 rot, CQuaternion* dstq );

public:
	CGPAnimHandler* m_gpah;
	LONG m_current;
	LONG m_startframe;
	LONG m_endframe;
	int m_sl_mp_rangemax;
	LONG m_previewframe;

	int m_ismoving;
	CWindow m_sl_mp_wnd;
	LONG m_animno;

	int m_gtype;
	int m_planetype;

	int m_ctrldrag;
	int m_altdrag;
	int m_dragx;

	int m_savebmdisp;
	int m_savejdisp;

	int m_gpmode;
private:
	CMotParamDlg* m_dlg;
	CMyD3DApplication* m_app;	

	CGPKey* m_draggpk;
	int m_dragframe;
	int m_dragflag;
	CGPKey* m_cpgpk;

	GPELEM m_gpe;
	GPELEM m_hengpe;
	MOTID m_motid[ MAXMOTIONNUM ];

	CWindow m_dlg_wnd;
	CWindow m_tex_current_wnd;

	CWindow m_combog_wnd;
	CWindow m_comboplane_wnd;
	CWindow m_comboong_wnd;

	CWindow m_mp_wnd;
	CWindow m_sp_mp_wnd;

	CWindow m_posx_wnd;
	CWindow m_posy_wnd;
	CWindow m_posz_wnd;

	CWindow m_rotx_wnd;
	CWindow m_roty_wnd;
	CWindow m_rotz_wnd;
	CWindow m_rotstep_wnd;

	CWindow m_rayy_wnd;
	CWindow m_rayleng_wnd;
	CWindow m_offsety_wnd;

	CWindow m_cenx_wnd;
	CWindow m_ceny_wnd;
	CWindow m_cenz_wnd;
	CWindow m_mvdeg_wnd;
	//CWindow m_mvstep_wnd;
	CWindow m_drag_wnd;

	CWindow m_preview_wnd;
	CWindow m_stop_wnd;

	HPEN	m_black_pen;
	HPEN	m_red_pen;
	HPEN	m_blue_pen;
	HBRUSH	m_black_brush;
	HBRUSH	m_white_brush;
	HBRUSH	m_red_brush;
	HBRUSH	m_blue_brush;

	D3DXVECTOR3 m_center;
	float	m_mvdeg;

	float m_rotstep;
	float m_mvstep;

	int m_timerworking;
	int m_cmdshow;
	int m_mcap;
	int m_moving;

};

#endif //__GPKeyDlg_H_
