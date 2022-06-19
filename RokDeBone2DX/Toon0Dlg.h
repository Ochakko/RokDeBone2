// Toon0Dlg.h : CToon0Dlg の宣言

#ifndef __Toon0Dlg_H_
#define __Toon0Dlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>
#include <d3dx9.h>

class CMotHandler;
class CTreeHandler2;
class CShdHandler;
class CToon1Params;

class CMyD3DApplication;

/////////////////////////////////////////////////////////////////////////////
// CToon0Dlg
class CToon0Dlg : 
	public CAxDialogImpl<CToon0Dlg>
{
public:
	CToon0Dlg( CMyD3DApplication* papp );
	~CToon0Dlg();

	enum { IDD = IDD_TOON0DLG };

BEGIN_MSG_MAP(CToon0Dlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_LIST1, OnSelchangeList1)
	COMMAND_ID_HANDLER(IDC_LIST2, OnSelchangeList2)

	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)

	NOTIFY_HANDLER(IDC_SLDARKDEG, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_darkdeg)
	NOTIFY_HANDLER(IDC_SLBRIGHTDEG, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_brightdeg)

	//MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	COMMAND_ID_HANDLER(IDC_DIFFUSE, OnDiffuse)
	COMMAND_ID_HANDLER(IDC_AMBIENT, OnAmbient)
	COMMAND_ID_HANDLER(IDC_SPECULAR, OnSpecular)
	COMMAND_ID_HANDLER(IDC_CHANGENAME, OnChangeName)

	COMMAND_ID_HANDLER(IDC_B_EDGECOL0, OnEdgeCol0)
	COMMAND_ID_HANDLER(IDC_DISPEDGE0, OnDispEdge0)
	COMMAND_ID_HANDLER(IDC_INVEDGE0, OnInvEdge0)
	COMMAND_ID_HANDLER(IDC_B_EDGEWIDTH0, OnEdgeWidth0)


END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangeList1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeList2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_darkdeg(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_brightdeg(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


	//LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnDiffuse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAmbient(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSpecular(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnEdgeCol0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDispEdge0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnInvEdge0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEdgeWidth0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


public:
	int ShowDlg( int cmdshow );

private:
	int ParamsToDlg();

	int CreateList1();
	int CreateList2();

	int DestroyObjs();

	int PaintTexture();
	int PaintEdgeCol0();

	int Toon1ToParams();

public:
	int m_seldisp;
	int m_selmaterial;
	CToon1Params* m_toon1;


private:
	CMyD3DApplication* m_papp;
	int m_cmdshow;
	
	CWindow m_dlg_wnd;
	CWindow m_list1_wnd;
	CWindow m_list2_wnd;

	CWindow m_colamb_wnd;
	CWindow m_coldif_wnd;
	CWindow m_colspc_wnd;
	CWindow m_name_wnd;

	CWindow m_sl_darkdeg_wnd;
	CWindow m_sl_brightdeg_wnd;
	CWindow m_darkdeg_wnd;
	CWindow m_brightdeg_wnd;

	CWindow m_diffuse_wnd;
	CWindow m_ambient_wnd;
	CWindow m_specular_wnd;

	CWindow m_edgecol0_wnd;
	CWindow m_text_edgecol0_wnd;
	CWindow m_edgewidth0_wnd;

	HDC m_colamb_dc;
	HDC m_coldif_dc;
	HDC m_colspc_dc;
	HDC m_edgecol0_dc;

	HBRUSH m_diffuse_brush;
	HBRUSH m_ambient_brush;
	HBRUSH m_specular_brush;
	HBRUSH m_edgecol0_brush;
	HPEN m_diffuse_pen;
	HPEN m_ambient_pen;
	HPEN m_specular_pen;
	HPEN m_white_pen;
	HPEN m_edgecol0_pen;
	COLORREF m_diffuse;
	COLORREF m_ambient;
	COLORREF m_specular;
	COLORREF m_edgecol0;

	float m_darkdeg;
	float m_brightdeg;

	char m_name[32];

	int m_timerid;

	int m_dispedge0;
	int m_invedge0;
	float m_edgewidth0;
};

#endif //__Toon0Dlg_H_
