// BoneLineDlg.h : CBoneLineDlg の宣言

#ifndef __BONELINEDLG_H_
#define __BONELINEDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>


class CShdElem;

/////////////////////////////////////////////////////////////////////////////
// CBoneLineDlg
class CBoneLineDlg : 
	public CAxDialogImpl<CBoneLineDlg>
{
public:
	CBoneLineDlg();
	~CBoneLineDlg();

	int SetSelemParam( CShdElem* srcselem );

	enum { IDD = IDD_BONELINEDLG };

BEGIN_MSG_MAP(CBoneLineDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REF_SELCOL, OnRefSelCol)
	COMMAND_ID_HANDLER(IDC_REF_UNSELCOL, OnRefUnSelCol)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefSelCol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefUnSelCol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	COLORREF	m_selcol;
	COLORREF	m_unselcol;

	CShdElem*	m_selem;
	int			m_bonelinedisp;

	int			m_checkchild;

private:
	CWindow m_dlg_wnd;

	CWindow m_selcol_wnd;
	CWindow m_unselcol_wnd;

	CWindow m_checkchild_wnd;

private:
	void SetWnd();
	int InitParams();
	int ParamsToDlg();
	int FillColorGDI( COLORREF srccol, CWindow srcwnd );

};

#endif //__BONELINEDLG_H_
