// TexKeyDlg.h : CTexKeyDlg の宣言

#ifndef __TexKeyDlg_H_
#define __TexKeyDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>

#include <coef.h>

//#include "TexList.h"

class CMotParamDlg;
class CTexChange;
class CTexKey;
class CMQOMaterial;

/////////////////////////////////////////////////////////////////////////////
// CTexKeyDlg
class CTexKeyDlg : 
	public CAxDialogImpl<CTexKeyDlg>
{
public:
	enum { IDD = IDD_TEXKEYDLG };


	CTexKeyDlg( CMotParamDlg* srcmpdlg );
	~CTexKeyDlg();
	
	int SetAppWnd( HWND appwnd );

	int Redraw();
	int OnUserSelchange();

	int ShowDlg( int cmdshow, int stopflag = 1 );

	int DestroyTimer();

	int InitDlg();

BEGIN_MSG_MAP(CTexKeyDlg)
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


	NOTIFY_HANDLER(IDC_SL_MP, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_mp)
	NOTIFY_HANDLER(IDC_SP_MP, UDN_DELTAPOS, OnSpin_Sp_mp )

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

	LRESULT OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
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

	int DrawTexKeys();

	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );

	int SetNewWindowProc();

	int InitList();
	int AddItem2List( int itemno, CMQOMaterial* srcmat, CTexChange* srctc );


	int OnList1( int hititem, POINT inview );
	int CaptureM();

public:
//	int m_current;

//	int m_displayflag;
	int m_ismoving;
	CWindow m_sl_mp_wnd;

	int m_ctrldrag;
	int m_altdrag;
	int m_dragx;

private:
	CMotParamDlg* m_dlg;
	
	CTexKey* m_dragtk;
	int m_dragframe;

	CWindow m_dlg_wnd;
	CWindow m_list_wnd;
//	CContainedWindow m_list_wnd;
	CWindow m_tex_current_wnd;

	CWindow m_mp_wnd;
	//CWindow m_sl_mp_wnd;//publicへ
	CWindow m_sp_mp_wnd;

	CWindow m_framecopy_wnd;
	CWindow m_framepaste_wnd;
	CWindow m_framedel_wnd;

	CWindow m_preview_wnd;
	CWindow m_stop_wnd;
	CWindow m_alldelete_wnd;

	CWindow m_sellock_name_wnd;

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

	CTexKey* m_cptk;
};

#endif //__TexKeyDlg_H_
