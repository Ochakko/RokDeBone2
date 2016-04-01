// InfluenceDlg.h : CFrameCopyDlg の宣言

#ifndef __FRAMECOPYDLG_H_
#define __FRAMECOPYDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;
class CShdHandler;

#define FRAMECOPYLISTLENG	1024

/////////////////////////////////////////////////////////////////////////////
// CFrameCopyDlg
class CFrameCopyDlg : 
	public CAxDialogImpl<CFrameCopyDlg>
{
public:
	CFrameCopyDlg( CTreeHandler2* lpth, CShdHandler* lpsh, HWND appwnd, int srcmaxframe );
	~CFrameCopyDlg();

	int CreateLists();

	enum { IDD = IDD_FRAMECOPYDLG };

BEGIN_MSG_MAP(CFrameCopyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_TREE1, TVN_SELCHANGED, OnSelchangedTree1)
	COMMAND_ID_HANDLER(IDC_ADD, OnAdd)
	COMMAND_ID_HANDLER(IDC_DELETE, OnDelete)
	COMMAND_ID_HANDLER(IDC_ALLDELETE, OnAllDelete)

	COMMAND_ID_HANDLER(IDC_ADD2, OnAdd2)
	COMMAND_ID_HANDLER(IDC_DELETE2, OnDelete2)
	COMMAND_ID_HANDLER(IDC_ALLDELETE2, OnAllDelete2)
	
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAdd2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	int FillTree();
	void AddBoneToTree( int srcseri, int addbroflag, int addtolast );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	void CreateImageList();
	int ParamsToDlg();

	int SetTree2ListReq( int srcno, int* dstlist, int addbroflag );


private:
	CWindow m_dlg_wnd;
	CWindow m_tree_wnd;
	CWindow m_list_wnd;
	CWindow m_list2_wnd;
	CWindow m_start_wnd;
	CWindow m_end_wnd;
	CWindow m_ismulti_wnd;

	HIMAGELIST m_hImageList;
	int m_iImage;
	int m_iSelect;

	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	int m_selectedno;
	
	CTreeHandler2* m_thandler;
	CShdHandler* m_shandler;

	HWND	m_apphwnd;

public:
// ユーザーが指定した、treeの先頭番号を格納する。
	int m_influencenum;
	int m_influencelist[FRAMECOPYLISTLENG];

	int m_ignorenum;
	int m_ignorelist[FRAMECOPYLISTLENG];

// ユーザーが指定したtree全体を格納（子供を含む。）	長さは、s2shd_leng, 該当する場合は、１それ以外は０
	int* m_validelemlist;
	int* m_invalidelemlist;


public:

// 長さは、s2shd_leng, 該当する場合は、１それ以外は０
	int* m_cplist;

	int m_ismulti;
	int m_maxframe;
	int m_startframe;
	int m_endframe;

};

#endif //__FRAMECOPYDLG_H_
