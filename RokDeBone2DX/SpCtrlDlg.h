// SpCtrlDlg.h : CSpCtrlDlg の宣言

#ifndef __SpCtrlDlg_H_
#define __SpCtrlDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>

#include <coef.h>

class CTreeHandler2;
class CShdHandler;
class CMotHandler;
class CMotionPoint2;

/////////////////////////////////////////////////////////////////////////////
// CSpCtrlDlg
class CSpCtrlDlg : 
	public CAxDialogImpl<CSpCtrlDlg>
{
public:
	enum { IDD = IDD_SPCTRLDLG };


	CSpCtrlDlg();
	~CSpCtrlDlg();
	
	int ShowDlg( int cmdshow, CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh,
		int boneseri, int frameno, int mkind, SPPARAM* spp );

	int InitDlg();
	int ParamsToDlg();

BEGIN_MSG_MAP(CSpCtrlDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)


	COMMAND_ID_HANDLER(IDC_SYNC, OnSync)
	COMMAND_ID_HANDLER(IDC_LOCK, OnLock)
	COMMAND_ID_HANDLER(IDC_AXISKIND, OnSelchangeAxis)

	COMMAND_ID_HANDLER(IDC_TS, OnApplyTS)
	COMMAND_ID_HANDLER(IDC_MAXS, OnApplyMAXS)
	COMMAND_ID_HANDLER(IDC_DS, OnApplyDS)
	COMMAND_ID_HANDLER(IDC_ZEROS, OnZeroS)
	COMMAND_ID_HANDLER(IDC_MINTS, OnApplyMINTS)
	COMMAND_ID_HANDLER(IDC_MAXTS, OnApplyMAXTS)

	COMMAND_ID_HANDLER(IDC_TE, OnApplyTE)
	COMMAND_ID_HANDLER(IDC_MAXE, OnApplyMAXE)
	COMMAND_ID_HANDLER(IDC_DE, OnApplyDE)
	COMMAND_ID_HANDLER(IDC_ZEROE, OnZeroE)
	COMMAND_ID_HANDLER(IDC_MINTE, OnApplyMINTE)
	COMMAND_ID_HANDLER(IDC_MAXTE, OnApplyMAXTE)


	NOTIFY_HANDLER(IDC_SLTS, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_TS)
	NOTIFY_HANDLER(IDC_SLDS, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_DS)
	NOTIFY_HANDLER(IDC_SLTE, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_TE)
	NOTIFY_HANDLER(IDC_SLDE, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_DE)


//	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_MOVING, OnMoving)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_MOVE, OnMove)

END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSync(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnLock(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeAxis(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnApplyTS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMAXS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyDS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZeroS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMINTS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMAXTS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnApplyTE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMAXE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyDE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZeroE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMINTE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyMAXTE(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnReleasedcaptureSl_TS(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_DS(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_TE(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_DE(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	
	int SetWnd();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );
	int SetCombo();

public:
	int m_ismoving;

	CTreeHandler2* m_lpth;
	CShdHandler* m_lpsh;
	CMotHandler* m_lpmh;
	int m_boneseri;
	int m_frameno;
	int m_axiskind;
	int m_mkind;
	SPPARAM* m_curspp;

private:
	CWindow m_dlg_wnd;
	CWindow m_bonename_wnd;
	CWindow m_frameno_wnd;
	CWindow m_axiskind_wnd;
	CWindow m_mkind_wnd;
	CWindow m_sync_wnd;

	CWindow m_slts_wnd;
	CWindow m_editts_wnd;
	CWindow m_editmaxs_wnd;
	CWindow m_slds_wnd;
	CWindow m_editds_wnd;
	CWindow m_editmints_wnd;
	CWindow m_editmaxts_wnd;

	CWindow m_slte_wnd;
	CWindow m_editte_wnd;
	CWindow m_editmaxe_wnd;
	CWindow m_slde_wnd;
	CWindow m_editde_wnd;
	CWindow m_editminte_wnd;
	CWindow m_editmaxte_wnd;

	int m_cmdshow;
	int m_createcomboflag;
	int m_moving;
};

#endif //__SpCtrlDlg_H_
