// SyncDlg.h : CSyncDlg の宣言

#ifndef __SyncDlg_H_
#define __SyncDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>
#include <coef.h>
/////////////////////////////////////////////////////////////////////////////
// CSyncDlg
class CMyD3DApplication;
class CSndKeyDlg;

class CSyncDlg : 
	public CAxDialogImpl<CSyncDlg>
{
public:
	CSyncDlg( CMyD3DApplication* appptr );
	~CSyncDlg();

	enum { IDD = IDD_SYNCDLG };


BEGIN_MSG_MAP(CSyncDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_FPS, OnFPS)
	COMMAND_ID_HANDLER(IDC_APPLYBPM, OnApplyBPM)
	COMMAND_ID_HANDLER(IDC_APPLYHAKU, OnApplyHaku)
	COMMAND_ID_HANDLER(IDC_APPLYSEC, OnApplySec)
	COMMAND_ID_HANDLER(IDC_APPLYFRAME, OnApplyFrame)
	COMMAND_ID_HANDLER(IDC_JUMP, OnJumpFrame)

	COMMAND_ID_HANDLER(IDC_QUA, OnSelQua)
//	NOTIFY_HANDLER(IDC_QUA, NM_RELEASEDCAPTURE, OnSelQua)

	COMMAND_ID_HANDLER(IDC_CAM, OnSelCam)
	COMMAND_ID_HANDLER(IDC_GPA, OnSelGpa)
	COMMAND_ID_HANDLER(IDC_MOE, OnSelMoe)
	COMMAND_ID_HANDLER(IDC_SND, OnSelSnd)
	COMMAND_ID_HANDLER(IDC_APPLYRANGE, OnApplyRange)

	//NOTIFY_HANDLER(IDC_SELCOL_POINT, NM_CUSTOMDRAW, OnCustomSelPoint)
	//MESSAGE_HANDLER(WM_PAINT, OnPaint)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnFPS(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyBPM(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyHaku(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplySec(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyFrame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnJumpFrame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnSelQua(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//	LRESULT OnSelQua(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelCam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelGpa(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelMoe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelSnd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyRange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	int ShowDlg( int cmdshow );
	int SetSndDlg( CSndKeyDlg* sndptr );

	int ParamsToDlg();
	int SetRangeButton();

public:
	int m_fps;
	int m_bpm;

	int m_quaflag;
	int m_camflag;
	int m_gpaflag;
	int m_moeflag;
	int m_sndflag;

	ANIMTIMING m_timing;


private:
	CWindow m_dlg_wnd;
	CWindow m_combo_fps_wnd;
	CWindow m_bpm_wnd;
	CWindow m_haku_wnd;
	CWindow m_sec_wnd;
	CWindow m_frame_wnd;

	CWindow m_qua_wnd;
	CWindow m_cam_wnd;
	CWindow m_gpa_wnd;
	CWindow m_moe_wnd;
	CWindow m_snd_wnd;

	CWindow m_start_wnd;
	CWindow m_end_wnd;

	int m_cmdshow;

	CMyD3DApplication* m_app;
	CSndKeyDlg* m_snddlg;

	int m_startframe;
	int m_endframe;

};

#endif //__SyncDlg_H_
