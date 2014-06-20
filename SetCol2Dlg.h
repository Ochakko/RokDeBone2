// SetCol2Dlg.h : CSetCol2Dlg の宣言

#ifndef __SetCol2Dlg_H_
#define __SetCol2Dlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

#include <D3DX9.h>

/////////////////////////////////////////////////////////////////////////////
// CSetCol2Dlg



class CSetCol2Dlg : 
	public CAxDialogImpl<CSetCol2Dlg>
{
public:
	//CSetCol2Dlg( int alpha, int beta, COLORREF srccol, int srclblend );
	CSetCol2Dlg( MATERIAL0* wmatptr, MPALETTE* palptr, int palno );
	~CSetCol2Dlg();

	enum { IDD = IDD_SETCOL2DLG };

BEGIN_MSG_MAP(CSetCol2Dlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	//MESSAGE_HANDLER(WM_PAINT, OnPaint)

	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	MESSAGE_HANDLER(WM_DESTROY, OnDestroyDlg)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnPaintOrg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:


private:
	int SetWnd();
	int ParamsToDlg();

	int PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush, COLORREF srccol );

private:
	HDC m_diffuse_dc;
	HDC m_specular_dc;
	HDC m_ambient_dc;
	HDC m_emissive_dc;

	HBRUSH m_diffuse_b;
	HBRUSH m_specular_b;
	HBRUSH m_ambient_b;
	HBRUSH m_emissive_b;


	int m_timerid;
	
	int m_ope;
	MATERIAL0* m_wmptr;
	MPALETTE* m_mpptr;
	int m_palno;

	CWindow m_dlg_wnd;
	CWindow m_diffuse_wnd;
	CWindow m_specular_wnd;
	CWindow m_ambient_wnd;
	CWindow m_emissive_wnd;
	CWindow m_power_wnd;
	CWindow m_name_wnd;

};

#endif //__SetCol2Dlg_H_
