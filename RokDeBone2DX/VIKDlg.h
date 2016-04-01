// VIKDlg.h : CVIKDlg の宣言

#ifndef __VIKDLG_H_
#define __VIKDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CVIKDlg

class CMotParamDlg;
class CMyD3DApplication;

class CVIKDlg : 
	public CAxDialogImpl<CVIKDlg>
{
public:
	CVIKDlg( CMotParamDlg* motparamdlg );
	~CVIKDlg();

	enum { IDD = IDD_VIKDLG };

BEGIN_MSG_MAP(CVIKDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_START_IK, OnStartIK)
	COMMAND_ID_HANDLER(IDC_START_SHIFTIK, OnStartShiftIK)
	COMMAND_ID_HANDLER(IDC_TREESHIFT, OnTreeShift)
	COMMAND_ID_HANDLER(IDC_STOP_IK, OnStopIK)

	MESSAGE_HANDLER(WM_TIMER, OnTimer)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStartIK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStartShiftIK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnTreeShift(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStopIK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	//LRESULT OnStopIK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	int ShowDlg( int cmdshow );
	int OnUserSelchange( int selno );
	int CallOnStopIK();
	int DestroyTimer();
private:
	void SetWnd();
	int ParamsToDlg();
	int GetCurrentValueFromMotParam();

public:
	CMyD3DApplication* m_d3dapp;
	int m_cmdshow;
	int m_timerworking;
private:
	CMotParamDlg* m_motparamdlg;

	CWindow text_selname_wnd;

	CWindow text_curx_wnd;
	CWindow text_cury_wnd;
	CWindow text_curz_wnd;
	
	CWindow text_dist_wnd;

	CWindow edit_div_wnd;
	CWindow edit_tx_wnd;
	CWindow edit_ty_wnd;
	CWindow edit_tz_wnd;

	CWindow start_ik_wnd;
	CWindow start_shiftik_wnd;
	CWindow shifttree_wnd;
	CWindow stop_ik_wnd;

	char m_selname[1024];
	D3DXVECTOR3 m_curpos;
	D3DXVECTOR3 m_tpos;
	int	m_ikdiv;

	int m_underik;

	int m_timerid;
	int m_selectedno;

	float m_dist;

	int m_frameno;
};

#endif //__VIKDLG_H_
