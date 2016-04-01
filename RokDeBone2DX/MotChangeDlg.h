// InfluenceDlg.h : CMotChangeDlg の宣言

#ifndef __MOTCHANGEDLG_H_
#define __MOTCHANGEDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;
class CShdHandler;

class CMyD3DApplication;
class CMCHandler;
class CMCRMenu;
class CEventKey;

#include <coef.h>

/////////////////////////////////////////////////////////////////////////////
// CMotChangeDlg
class CMotChangeDlg : 
	public CAxDialogImpl<CMotChangeDlg>
{
public:
	CMotChangeDlg( CMyD3DApplication* papp );
	~CMotChangeDlg();

	//int CreateLists();

	enum { IDD = IDD_MOTCHANGEDLG };

BEGIN_MSG_MAP(CMotChangeDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(ID_MCRMENU_ADD, OnAdd)
	COMMAND_ID_HANDLER(ID_MCRMENU_DEL, OnDel)
	COMMAND_ID_HANDLER(ID_MCRMENU_PROP, OnProp)
	COMMAND_ID_HANDLER(ID_MCRMENU_COPY, OnCopy)
	COMMAND_ID_HANDLER(ID_MCRMENU_PASTE, OnPaste)

	NOTIFY_HANDLER(IDC_TREE1, TVN_SELCHANGED, OnSelchangedTree1)
	NOTIFY_HANDLER(IDC_TREE1, TVN_ITEMEXPANDED, OnExpandedTree1)

	COMMAND_ID_HANDLER(IDC_ADD, OnAddList)
	COMMAND_ID_HANDLER(IDC_DEFAULT10, OnDefault10)
	COMMAND_ID_HANDLER(IDC_DEL, OnDelList)
	COMMAND_ID_HANDLER(IDC_ALLDEL, OnAllDelList)
	COMMAND_ID_HANDLER(IDC_APPLY, OnApplyFULeng)
	COMMAND_ID_HANDLER(IDC_PLAY, OnPlay)
	COMMAND_ID_HANDLER(IDC_STOP, OnStop)
	COMMAND_ID_HANDLER(IDC_COMBOIDLE, OnChangeIdle)
	

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnProp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnExpandedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnAddList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDefault10(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyFULeng(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlay(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeIdle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);



public:
	int ShowDlg( int cmdshow );

	int DestroyMCElem( int delcookie );
	int InitMC();

	int AddParentMC( int addcookie, int srcidling, int srcev0idle, int srccommonid, int srcforbidnum, int* srcforbidid, int srcnotfu );
	int AddChildMC( int parentcookie, MCELEM childmc );


	int SaveMAFile( char* srcdirname, char* srcmafname, int overwriteflag );

private:
	int FillTree();
	//void AddBoneToTree( int srcseri, int addbroflag, int addtolast );
	int AddMCToTree( HTREEITEM hRoot );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	void CreateImageList();
	int ParamsToDlg();

	int OnUserSelchange( int selno );

	int WriteQuaFile( char* srcfilename, int srccookie, int srcstart, int srcend, int overwriteflag );

	int InitComboKey();
	int InitList();

	int InitComboIdle();

public:
	int m_firstmake;
//	int m_idlingid;
	char m_idlingname[ MAX_PATH ];
	CMCHandler* m_mch;

private:
	int m_cmdshow;
	CWindow m_dlg_wnd;
	CWindow m_tree_wnd;
	CWindow m_list_wnd;
	CWindow m_event_wnd;
	CWindow m_combokey_wnd;
	CWindow m_comboidle_wnd;
	CWindow m_fuleng_wnd;

	int m_eventno;
	int m_combono;
	int m_fuleng;

	HIMAGELIST m_hImageList;
	int m_iImage;
	int m_iSelect;

	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	int m_selectedno;
	
	CMyD3DApplication* m_papp;

	HWND	m_apphwnd;

	//int* m_serial2cookie;

	CMCRMenu* m_rmenu;

	int m_undertreeedit;

	int m_overwritekind;

	int m_cpelemnum;
	MCELEM m_cpelem[MAXMCCOPYNUM];

	int m_singleevent;
};

#endif //__MotChangeDlg_H_
