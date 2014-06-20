// Za4Dlg.h : CZa4Dlg の宣言

#ifndef __Za4Dlg_H_
#define __Za4Dlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>
#include <d3dx9.h>

class CMyD3DApplication;
class CQuaternion;

/////////////////////////////////////////////////////////////////////////////
// CZa4Dlg
class CZa4Dlg : 
	public CAxDialogImpl<CZa4Dlg>
{
public:
	CZa4Dlg( CMyD3DApplication* papp );
	~CZa4Dlg();

	int UpdateParams( int srcseri );

	int ShowDlg( int cmdshow );

//	int GetLimitedQ( int boneno, CQuaternion* startq, D3DXVECTOR3* befeul, CQuaternion* endq, CQuaternion* dstq, 
//		int* startout, D3DXVECTOR3* dsteul, int notmvflag, int initaxisq );

	enum { IDD = IDD_4DLG };

BEGIN_MSG_MAP(CZa4Dlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_APPLY, OnApply)

	COMMAND_ID_HANDLER(IDC_ZA1, OnZa1)
	COMMAND_ID_HANDLER(IDC_ZA2, OnZa2)
//	COMMAND_ID_HANDLER(IDC_ZA3, OnZa3)

	COMMAND_ID_HANDLER(IDC_COMBO_PLANE, OnPlane)
	

END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnZa1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZa2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//	LRESULT OnZa3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	void SetWnd();
	int ParamsToDlg();
	int InitParams();
	int CheckTypeButton();


//	int GetLimitedEul( float starteul, float endeul, double lim0, double lim1, float* limeul );


public:
	int m_serialno;

	D3DXVECTOR3 m_rot;

	char* m_nameptr;

	int m_za4type;
	int m_za4rotaxis;
private:
	CMyD3DApplication* m_papp;

	CWindow m_rotx_wnd;
	CWindow m_roty_wnd;
	CWindow m_rotz_wnd;

	CWindow m_name_wnd;

	CWindow m_za1_wnd;
	CWindow m_za2_wnd;
//	CWindow m_za3_wnd;

	CWindow m_plane_wnd;

	CWindow m_dlg_wnd;
	
	int m_cmdshow;
	int m_createflag;

};

#endif //__Za4Dlg_H_
