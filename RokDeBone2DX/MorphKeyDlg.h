// MorphKeyDlg.h : CMorphKeyDlg の宣言

#ifndef __MorphKeyDlg_H_
#define __MorphKeyDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>

#include <coef.h>

//#include "TexList.h"

class CMotParamDlg;
class CMMotKey;
class CMMotElem;
class CShdElem;
class CMotionCtrl;
class CMotionInfo;

/////////////////////////////////////////////////////////////////////////////
// CMorphKeyDlg
class CMorphKeyDlg : 
	public CAxDialogImpl<CMorphKeyDlg>
{
public:
	enum { IDD = IDD_MORPHKEYDLG };


	CMorphKeyDlg( CMotParamDlg* srcmpdlg );
	~CMorphKeyDlg();
	
	int SetAppWnd( HWND appwnd );

	int Redraw();
	int OnUserSelchange();

	int ShowDlg( int cmdshow, int stopflag = 1 );

	int DestroyTimer();

	int InitDlg();
	int ResetDlg();

BEGIN_MSG_MAP(CMorphKeyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)

	//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_PREVIEW, OnPreview)
	COMMAND_ID_HANDLER(IDC_FRAMECOPY, OnFrameCopy)
	COMMAND_ID_HANDLER(IDC_FRAMEPASTE, OnFramePaste)
	COMMAND_ID_HANDLER(IDC_FRAMEDEL, OnFrameDel)
	COMMAND_ID_HANDLER(IDC_STOP, OnStop)
	COMMAND_ID_HANDLER(IDC_ALLDELETE, OnAllDelete)

	COMMAND_ID_HANDLER(IDC_BEFORE_MP, OnBeforeMP)
	COMMAND_ID_HANDLER(IDC_NEXT_MP, OnNextMP)

	COMMAND_ID_HANDLER(IDC_CREATEKEY, OnCreateKey)
	COMMAND_ID_HANDLER(IDC_COMBO1, OnSelchangeCombo1)

	COMMAND_ID_HANDLER(IDC_APPLYVAL, OnApplyVal)
	COMMAND_ID_HANDLER(IDC_ZERO, OnZero)
	COMMAND_ID_HANDLER(IDC_ONE, OnOne)
	COMMAND_ID_HANDLER(IDC_ALLZERO, OnAllZero)



	NOTIFY_HANDLER(IDC_SL_MP, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_mp)
	NOTIFY_HANDLER(IDC_SP_MP, UDN_DELTAPOS, OnSpin_Sp_mp )
	NOTIFY_HANDLER(IDC_SL_VAL, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_val)

//	NOTIFY_HANDLER(IDC_LIST1, LVN_COLUMNCLICK, OnSelchangeList1)//見出し
//	NOTIFY_HANDLER(IDC_LIST1, LVN_ITEMACTIVATE, OnSelchangeList1)//？
	NOTIFY_HANDLER(IDC_LIST1, LVN_ITEMCHANGED, OnSelchangeList1)

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
	LRESULT OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFrameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCreateKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeCombo1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnApplyVal(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZero(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnOne(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllZero(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_val(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangeList1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



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
	int OnJumpFrame( int srcframe );

private:
	
	int SetWnd();

	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int DrawMMotKeys();

	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );

	int SetNewWindowProc();

	int InitList();
	int AddItem2List( int itemno, CMotionInfo* motinfo, CShdElem* srctarget );


	int OnList1( int hititem, POINT inview );
	int CaptureM();

	int SetCombo1();
	int GetValue( CShdElem* srcbase, CShdElem* srctarget, CMotionCtrl* srcbone, float* valptr );


public:
	int m_ismoving;
	CWindow m_sl_mp_wnd;

	CShdElem* m_base;
	CShdElem* m_target;
	CMotionCtrl* m_boneelem;

	int m_ctrldrag;
	int m_altdrag;
	int m_dragx;

private:
	CShdElem** m_ppselem;
	float m_value;

	CMotParamDlg* m_dlg;
	
	CMMotElem* m_dragmme;
	int m_dragframe;
	int m_dropframe;
	CMMotElem* m_cpmme;

	CWindow m_dlg_wnd;
	CWindow m_list_wnd;
//	CContainedWindow m_list_wnd;
	CWindow m_tex_current_wnd;

	CWindow m_mp_wnd;
	//CWindow m_sl_mp_wnd;//publicへ
	CWindow m_sp_mp_wnd;
	CWindow m_sl_val_wnd;
	CWindow m_edit_val_wnd;
	CWindow m_combo1_wnd;

	HPEN	m_black_pen;
	HPEN	m_red_pen;
	HPEN	m_blue_pen;
	HBRUSH	m_black_brush;
	HBRUSH	m_white_brush;
	HBRUSH	m_red_brush;
	HBRUSH	m_blue_brush;

	int m_dragflag;

	int m_timerworking;

	int m_cmdshow;

	int m_createlistflag;
	int m_mcap;
	int m_moving;
	int m_remakelistflag;

};

#endif //__MorphKeyDlg_H_
