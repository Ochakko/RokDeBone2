// InfluenceDlg.h : CInfluenceDlg の宣言

#ifndef __INFLUENCEDLG_H_
#define __INFLUENCEDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;
class CShdHandler;

#define INFLUENCELISTLENG	1024

/////////////////////////////////////////////////////////////////////////////
// CInfluenceDlg
class CInfluenceDlg : 
	public CAxDialogImpl<CInfluenceDlg>
{
public:
	CInfluenceDlg( CTreeHandler2* lpth, CShdHandler* lpsh, HWND appwnd );
	~CInfluenceDlg();

	enum { IDD = IDD_INFLUENCEDLG };

BEGIN_MSG_MAP(CInfluenceDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_TREE1, TVN_SELCHANGED, OnSelchangedTree1)
	COMMAND_ID_HANDLER(IDC_ADD, OnAdd)
	COMMAND_ID_HANDLER(IDC_DELETE, OnDelete)
	COMMAND_ID_HANDLER(IDC_ALLDELETE, OnAllDelete)

	COMMAND_ID_HANDLER(IDC_ADD2, OnAdd2)
	COMMAND_ID_HANDLER(IDC_DELETE2, OnDelete2)
	COMMAND_ID_HANDLER(IDC_ALLDELETE2, OnAllDelete2)

	COMMAND_ID_HANDLER(IDC_LIST3, OnSelDisp)


//	COMMAND_ID_HANDLER(IDC_SELDISP, OnSelDisp)


END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnApply();
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAdd2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

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

private:
	CWindow m_tree_wnd;
	CWindow m_list_wnd;
	CWindow m_list2_wnd;
	CWindow m_list3_wnd;
	CWindow m_dispname_wnd;

	HIMAGELIST m_hImageList;
	int m_iImage;
	int m_iSelect;

	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	int m_selectedno;
	


	CTreeHandler2* m_thandler;
	CShdHandler* m_shandler;
	int m_serialno;

	HWND	m_apphwnd;

	char m_dispname[_MAX_PATH];

public:
	int m_influencenum;
	int m_influencelist[INFLUENCELISTLENG];

	int m_ignorenum;
	int m_ignorelist[INFLUENCELISTLENG];

};

#endif //__INFLUENCEDLG_H_
