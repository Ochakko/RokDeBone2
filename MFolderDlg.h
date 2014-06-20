// MFolderDlg.h : CMFolderDlg の宣言

#ifndef __MFolderDlg_H_
#define __MFolderDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;
class CShdHandler;

#define MFLISTLENG	1024

/////////////////////////////////////////////////////////////////////////////
// CMFolderDlg
class CMFolderDlg : 
	public CAxDialogImpl<CMFolderDlg>
{
public:
	CMFolderDlg( CTreeHandler2* lpth, CShdHandler* lpsh );
	~CMFolderDlg();

	enum { IDD = IDD_MFOLDERDLG };

BEGIN_MSG_MAP(CMFolderDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_TREE1, TVN_SELCHANGED, OnSelchangedTree1)
	COMMAND_ID_HANDLER(IDC_LIST3, OnSelDisp)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelDisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	int FillTree();
	void AddBoneToTree( int srcseri, int addbroflag, int addtolast );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	void CreateImageList();
	int ParamsToDlg();

	int CreateList3();
	int InitListBox();

	int SetNewWindowProc();

public:
	int m_baseseri;
	int m_motionseri;

private:
	CWindow m_tree_wnd;
	CWindow m_list3_wnd;

	HIMAGELIST m_hImageList;
	int m_iImage;
	int m_iSelect;

	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	
	CTreeHandler2* m_thandler;
	CShdHandler* m_shandler;

};

#endif //__MFolderDlg_H_
