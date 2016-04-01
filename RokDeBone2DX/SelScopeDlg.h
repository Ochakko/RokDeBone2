// SelScopeDlg.h : CSelScopeDlg の宣言

#ifndef __SelScopeDlg_H_
#define __SelScopeDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;
class CShdHandler;

#define INFLUENCELISTLENG	1024

/////////////////////////////////////////////////////////////////////////////
// CSelScopeDlg
class CSelScopeDlg : 
	public CAxDialogImpl<CSelScopeDlg>
{
public:
	CSelScopeDlg( CTreeHandler2* lpth, CShdHandler* lpsh, HWND appwnd, int srcseri );
	~CSelScopeDlg();

	enum { IDD = IDD_SELSCOPEDLG };

BEGIN_MSG_MAP(CSelScopeDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_TREE1, TVN_SELCHANGED, OnSelchangedTree1)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

private:
	int FillTree();
	void AddBoneToTree( int srcseri, int addbroflag, int addtolast );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	void CreateImageList();
	int ParamsToDlg();

	int CreateList();

private:
	CWindow m_tree_wnd;
	CWindow m_list_wnd;

	HIMAGELIST m_hImageList;
	int m_iImage;
	int m_iSelect;

	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	int m_selbone;
	

	CTreeHandler2* m_thandler;
	CShdHandler* m_shandler;
	int m_seldisp;

	HWND	m_apphwnd;

	int m_convno;

	int* m_topflag;

};

#endif //__SelScopeDlg_H_
