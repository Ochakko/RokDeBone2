// CameraDlg.h : CCameraDlg の宣言

#ifndef __CAMERADLG_H_
#define __CAMERADLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>
#include <D3DX9.h>

#include <coef.h>


/////////////////////////////////////////////////////////////////////////////
// CCameraDlg
class CCameraDlg : 
	public CAxDialogImpl<CCameraDlg>
{
public:
	CCameraDlg( HWND srchwnd );
	~CCameraDlg();

	int SetCurrent2ViewParam( int vpno );
	int SetViewParam2Current( int vpno );

	enum { IDD = IDD_CAMERADLG };

BEGIN_MSG_MAP(CCameraDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_UD_RESET, OnUdReset)
	COMMAND_ID_HANDLER(IDC_ROT_RESET, OnRotReset)
	COMMAND_ID_HANDLER(IDC_ZOOM_RESET, OnZoomReset)
	COMMAND_ID_HANDLER(IDC_TARGET_RESET, OnTargetReset)
	COMMAND_ID_HANDLER(IDC_RAD2_RESET, OnRad2Reset)

	COMMAND_ID_HANDLER(IDC_SAVE_F5, OnSaveF5)
	COMMAND_ID_HANDLER(IDC_SAVE_F6, OnSaveF6)
	COMMAND_ID_HANDLER(IDC_SAVE_F7, OnSaveF7)
	COMMAND_ID_HANDLER(IDC_SAVE_F8, OnSaveF8)

	COMMAND_ID_HANDLER(IDC_CAMROT, OnCamRot)
	COMMAND_ID_HANDLER(IDC_CAMMOVE, OnCamMove)
	COMMAND_ID_HANDLER(IDC_CAMZOOM, OnCamZoom)

	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	NOTIFY_HANDLER(IDC_SP_ROT, UDN_DELTAPOS, OnDeltaposSp_rot)
	NOTIFY_HANDLER(IDC_SP_UD, UDN_DELTAPOS, OnDeltaposSp_ud)
	NOTIFY_HANDLER(IDC_SP_ZOOM, UDN_DELTAPOS, OnDeltaposSp_zoom)
	NOTIFY_HANDLER(IDC_SP_TARGET, UDN_DELTAPOS, OnDeltaposSp_target)
	NOTIFY_HANDLER(IDC_SP_RAD2, UDN_DELTAPOS, OnDeltaposSp_rad2)

	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_MOVING, OnMoving)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUdReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRotReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZoomReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnTargetReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRad2Reset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSaveF5(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSaveF6(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSaveF7(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSaveF8(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCamRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCamMove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCamZoom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDeltaposSp_rot(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDeltaposSp_ud(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDeltaposSp_zoom(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDeltaposSp_target(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDeltaposSp_rad2(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int SetParams( float degxz, float eyey, float dist, D3DXVECTOR3 targetpos, float degy );
	int Redraw();

	int SetSymmXParams();

private:
	int CheckCameraButton();


public:
	float		m_degxz;
	float		m_degy;

	float		m_eye_y;
	float		m_camdist;
	D3DXVECTOR3	m_targetpos;

	VIEWPARAM	m_viewparam[4];
	
	int			m_mouseope;


private:
	int ParamsToDlg();

	//float		m_rad;
	//float		m_rad2;

	HWND		m_apphwnd;
	CWindow		m_sp_ud_wnd;
	CWindow		m_sp_rot_wnd;
	CWindow		m_sp_zoom_wnd;
	CWindow		m_sp_target_wnd;
	CWindow		m_sp_rad2_wnd;

	CWindow		m_eyey_wnd;
	CWindow		m_rad_wnd;
	CWindow		m_dist_wnd;
	CWindow		m_target_wnd;
	CWindow		m_rad2_wnd;

	CWindow		m_camrot_wnd;
	CWindow		m_cammove_wnd;
	CWindow		m_camzoom_wnd;

};

#endif //__CAMERADLG_H_
