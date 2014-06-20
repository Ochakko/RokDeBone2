// MOEKeyDlg.h : CMOEKeyDlg の宣言

#ifndef __MOEKeyDlg_H_
#define __MOEKeyDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <D3DX9.h>
#include <commctrl.h>

#include <coef.h>
#include <quaternion.h>

class CMyD3DApplication;
class CMotParamDlg;
class CMOEAnimHandler;
class CMOEAnim;
class CMOEKey;
class CMState;
class CShdHandler;

/////////////////////////////////////////////////////////////////////////////
// CMOEKeyDlg
class CMOEKeyDlg : 
	public CAxDialogImpl<CMOEKeyDlg>
{
public:
	enum { IDD = IDD_MOEKEYDLG };


	CMOEKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp );
	~CMOEKeyDlg();
	
	int SetAppWnd( HWND appwnd );
	int Redraw();
	//int OnUserSelchange();
	int ShowDlg( int cmdshow, int stopflag = 1 );
	int DestroyTimer();
	int InitDlg( int atshowflag );
	int ResetParams( int atshowflag );
	int GetCurMOE( MOEELEM* dstmoee );

	int GetMOEAnimNum();
	int LoadMOEFile( char* filename );

	MOTID* GetMOEMotIDArray();
	char* GetMOEAnimName( int animno );
	int GetCurMOEAnimIndex();
	int GetCurMOEAnimNo();
	int SelectMOEAnim( int animindex );

	int DestroyMOEAnim( int delanimno );

//	int SetCurMOE( MOEELEM srcmoee );
	int SetPreviewButton();

	int CalcMotionPointOnFrame( int animno, int frameno, CMState** ppms, int* hasmpflag );
	int ExistMOEKey( int animno, int frameno, CMOEKey** ppmoek );
	int CalcFrameData();

	int PlayAnim( int srcframe, int totalflag = 0 );

BEGIN_MSG_MAP(CMOEKeyDlg)
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

	COMMAND_ID_HANDLER(IDC_SETTING, OnSetting)
	COMMAND_ID_HANDLER(IDC_CALC, OnCalc)


	COMMAND_ID_HANDLER(IDC_ADDLIST, OnAddList)
	COMMAND_ID_HANDLER(IDC_DELLIST, OnDelList)
	COMMAND_ID_HANDLER(IDC_ADDNOTLIST, OnAddNotList)
	COMMAND_ID_HANDLER(IDC_DELNOTLIST, OnDelNotList)
	COMMAND_ID_HANDLER(IDC_ADDELEM, OnAddElem)
	COMMAND_ID_HANDLER(IDC_DELELEM, OnDelElem)

	COMMAND_ID_HANDLER(IDC_ELEMLIST, OnSelElem)
	COMMAND_ID_HANDLER(IDC_COMBOIDLE, OnSelIdle)

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

	LRESULT OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAddList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAddNotList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelNotList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAddElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelIdle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

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
	int SetNewMOE();
	int SetCurrentMotion( int isfirst );

	int RemakeMOEAH();
	int OnJumpFrame( int srcframe );
	int SetStartAndEnd( int srcstart, int srcend );

private:
	int InitParams();
	int DestroyObjs( int delmoeah );

	int SetWnd();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int DrawMOEKeys();

	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );

	int SetNewWindowProc();

	int InitCombo();
	int InitBone2Combo();
	int InitList();

	int CaptureM();

public:
	CMOEAnimHandler* m_moeah;
	LONG m_current;
	LONG m_startframe;
	LONG m_endframe;
	int m_sl_mp_rangemax;
	LONG m_previewframe;

	int m_ismoving;
	CWindow m_sl_mp_wnd;
	LONG m_animno;

	int m_savebmdisp;
	int m_savejdisp;

	int m_ctrldrag;
	int m_altdrag;
	int m_dragx;

private:
	CMotParamDlg* m_dlg;
	CMyD3DApplication* m_app;	

	CMOEKey* m_dragmoek;
	int m_dragframe;
	int m_dragflag;
	CMOEKey* m_cpmoek;

	MOEELEM m_moee;
	MOEELEM m_zeromoee;
	MOTID m_motid[ MAXMOTIONNUM ];

	CWindow m_dlg_wnd;
	CWindow m_tex_current_wnd;

	CWindow m_mp_wnd;
	CWindow m_sp_mp_wnd;

	CWindow m_preview_wnd;
	CWindow m_stop_wnd;

	CWindow m_eventno_wnd;

	CWindow m_list_wnd;
	CWindow m_notlist_wnd;
	CWindow m_elemlist_wnd;
	
	CWindow m_comboidle_wnd;
	CWindow m_combolist_wnd;
	CWindow m_combonotlist_wnd;

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

	int* m_bone2combo;
	int* m_combo2bone;
	int m_indexleng;
	int m_jointnum;

	int m_elemindex;

};

#endif //__MOEKeyDlg_H_
