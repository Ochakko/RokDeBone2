// OptDlg.h : COptDlg の宣言

#ifndef __OPTDLG_H_
#define __OPTDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// COptDlg
class COptDlg : 
	public CAxDialogImpl<COptDlg>
{
public:
	COptDlg();
	~COptDlg();

	enum { IDD = IDD_OPTDLG };

	int m_bonedisp;
	int m_polydisp;
	int m_grounddisp;
	int m_quatype;

	int m_jointsize;
	int m_jointalpha;
	int m_undodlg;
	int m_rdlg;
	int m_infdlg;
	int m_modlg;

	int m_bmdisp;
//	int m_previewfps;

	int m_motstep;

	int m_ikfk;//0--> IK, 1-->FK

	int m_iktdisp;

	COLORREF m_UnselLine;
	COLORREF m_SelLine;
	COLORREF m_UnselPoint;
	COLORREF m_SelPoint;
	int m_PointSize;

	int m_MouseL;
	int m_MouseR;
	int m_MouseW;

	int m_usekinect;

BEGIN_MSG_MAP(COptDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	NOTIFY_HANDLER(IDC_SL_SIZE, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_size)
	NOTIFY_HANDLER(IDC_SL_ALPHA, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_alpha)

	COMMAND_ID_HANDLER(IDC_UNSELLINE, OnRefUnselcolLine)
	COMMAND_ID_HANDLER(IDC_SELLINE, OnRefSelcolLine)
	COMMAND_ID_HANDLER(IDC_UNSELPOINT, OnRefUnselcolPoint)
	COMMAND_ID_HANDLER(IDC_SELPOINT, OnRefSelcolPoint)

	COMMAND_ID_HANDLER(IDC_MOUSEDLG, OnMouseDlg)
	COMMAND_ID_HANDLER(IDC_SETCHKDLG, OnSetChkDlg)

	//NOTIFY_HANDLER(IDC_UNSELCOL_LINE, NM_CUSTOMDRAW, OnCustomUnselLine)
	//NOTIFY_HANDLER(IDC_SELCOL_LINE, NM_CUSTOMDRAW, OnCustomSelLine)
	//NOTIFY_HANDLER(IDC_UNSELCOL_POINT, NM_CUSTOMDRAW, OnCustomUnselPoint)
	//NOTIFY_HANDLER(IDC_SELCOL_POINT, NM_CUSTOMDRAW, OnCustomSelPoint)

	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	//MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnReleasedcaptureSl_size(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_alpha(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


	LRESULT OnRefUnselcolLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefSelcolLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefUnselcolPoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefSelcolPoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMouseDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSetChkDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	//LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnCustomUnselLine(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	//LRESULT OnCustomSelLine(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	//LRESULT OnCustomUnselPoint(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	//LRESULT OnCustomSelPoint(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


private:
	CWindow m_dlg_wnd;
	CWindow m_bone_disp_wnd;
	CWindow m_bone_nodisp_wnd;
	CWindow m_bm_on_wnd;
	CWindow m_bm_off_wnd;

	CWindow m_poly_alldisp_wnd;
	CWindow m_poly_seldisp_wnd;
	CWindow m_ground_nodisp_wnd;
	CWindow m_ground_disp_wnd;

	CWindow m_sl_size_wnd;
	CWindow m_sl_alpha_wnd;
	
	CWindow m_text_size_wnd;
	CWindow m_text_alpha_wnd;

//	CWindow m_combo_fps_wnd;

	CWindow m_combo_motstep_wnd;

	CWindow m_unselline_wnd;
	CWindow m_selline_wnd;
	CWindow m_unselpoint_wnd;
	CWindow m_selpoint_wnd;
	CWindow m_pointsize_wnd;

private:
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );


};

#endif //__OPTDLG_H_
