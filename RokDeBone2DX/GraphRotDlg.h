// GraphRotDlg.h : CGraphRotDlg の宣言

#ifndef __GraphRotDlg_H_
#define __GraphRotDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>

#include <d3dx9.h>

class CTreeHandler2;
class CShdHandler;
class CShdElem;
class CMotHandler;
class CMyD3DApplication;
class CMotParamDlg;
class CMyD3DApplication;
class CFUMenuMain;

class CCamKeyDlg;
class CGPKeyDlg;

class CSpCtrlDlg;

#include <MotionPoint2.h>
#include <CameraKey.h>
#include <GPKey.h>

typedef struct tag_gmp
{
	CMotionPoint2 mp;
	CCameraKey ck;
	CGPKey		gpk;
	int hasmpflag;
	int inrangeflag;
	POINT xp;
	POINT yp;
	POINT zp;
	int isspline;
	POINT ctrls[3];
	POINT ctrle[3];
} GMP;


/////////////////////////////////////////////////////////////////////////////
// CGraphRotDlg
class CGraphRotDlg : 
	public CAxDialogImpl<CGraphRotDlg>
{
public:
	CGraphRotDlg( CMyD3DApplication* papp, CMotParamDlg* srcmpdlg );
	~CGraphRotDlg();
	
	int Redraw();
	int OnUserSelchange();

	enum { IDD = IDD_GRAPHDLG };

	int ShowDlg( int cmdshow );
	int DestroyTimer();
	int InitDlg();
	int ShiftRange( int srcdiff );

	int CloseSpDlg();
	int DestroySpDlg();

BEGIN_MSG_MAP(CGraphRotDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_DRAGX, OnDragXButton)
	COMMAND_ID_HANDLER(IDC_DRAGY, OnDragYButton)
	COMMAND_ID_HANDLER(IDC_DRAGZ, OnDragZButton)

	COMMAND_ID_HANDLER(IDC_CTRL, OnCheckCtrl)

	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)

	MESSAGE_HANDLER(WM_MOVING, OnMoving)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUP)

	MESSAGE_HANDLER(WM_TIMER, OnTimer)

END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDragXButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDragYButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDragZButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCheckCtrl(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

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



private:
	int SetWnd();
	int ParamsToDlg();

	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int FramenoFL2Windowpos( RECT clirect, float frameno, int* posptr );

	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );

	int Tate2Windowpos( RECT clirect, int tate, int* posptr );
	int TateFL2Windowpos( RECT clirect, float tate, int* posptr );
	int Windowpos2TateFL( RECT clirect, int posy, float* tateptr );


	int CreatePen();
	int DrawYokoAxis();


	int DrawTateAxis();
	int CalcMP();
	int DrawEul();
	int DrawDragVal();
	int PickMP( int dragmode, POINT point, CMotionPoint2** ppmp );

	int DrawTateAxisCam();
	int CalcMPCam();
	int DrawEulCam();
	int DrawDragValCam();
	int PickMPCam( int dragmode, POINT point, CCameraKey** ppck );

	int DrawTateAxisGP();
	int CalcMPGP();
	int DrawEulGP();
	int DrawDragValGP();
	int PickMPGP( int dragmode, POINT point, CGPKey** ppgpk );

	int InitPaint();

	int CheckDragModeButton();

	int SetLockFlag( int boneno, int mkind, int flag );

public:

	LONG m_startframe;
	LONG m_endframe;
	int m_ismoving;

	CCamKeyDlg* m_camdlg;
	CGPKeyDlg* m_gpdlg;

private:
	CMyD3DApplication* m_papp;
	CMotParamDlg* m_dlg;


	CWindow m_dlg_wnd;
	CWindow m_graph_wnd;
	CWindow m_tate_wnd;
	CWindow m_yoko_wnd;
	CWindow m_dragx_wnd;
	CWindow m_dragy_wnd;
	CWindow m_dragz_wnd;
	CWindow m_dragval_wnd;

	HPEN	m_black_pen;
	HPEN	m_red_pen;
	HPEN	m_blue_pen;
	HPEN	m_green_pen;
	HPEN	m_gray_pen;
	HPEN	m_yellow_pen;
	HBRUSH	m_black_brush;
	HBRUSH	m_white_brush;
	HBRUSH	m_red_brush;
	HBRUSH	m_blue_brush;
	HBRUSH	m_green_brush;
	HBRUSH	m_gray_brush;
	HBRUSH	m_yellow_brush;

	int m_timerworking;
	int m_cmdshow;
	int m_dragflag;

	GMP* m_gmp;

	float m_rotmin;
	float m_rotmax;
	float m_tatemin;
	float m_tatemax;

	int m_dragyoko;
	int m_yokoval;

	int m_dragmode;
	int m_dragvalflag;
	CMotionPoint2* m_dragmp;
	CCameraKey* m_dragck;
	CGPKey* m_draggpk;

	CFUMenuMain* m_fumenu;
	CSpCtrlDlg* m_spdlg;
	int m_dispctrl;

};

#endif //__GraphRotDlg_H_
