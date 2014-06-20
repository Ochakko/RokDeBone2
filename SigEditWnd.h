#ifndef SIGEDITWNDH
#define SIGEDITWNDH

#include <coef.h>
#include "resource.h"       // メイン シンボル
#include <atlhost.h>

class CTreeHandler2;
class CShdHandler;
class CMotHandler;
class CShdElem;
class CMyD3DApplication;

class CSigEditWnd :
	public CWindowImpl<CSigEditWnd>
{

public:

BEGIN_MSG_MAP(CSigEditWnd)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_MOVING, OnMoving)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
//	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

//	MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
//	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
//	COMMAND_ID_HANDLER(IDMENU_SHDPARAM, OnMenuShdParam)
//	COMMAND_ID_HANDLER(IDMENU_JOINTLOC, OnMenuJointLoc)
//	COMMAND_ID_HANDLER(IDMENU_NEWJOINT, OnMenuNewJoint)
//	COMMAND_ID_HANDLER(IDMENU_INFLUENCE, OnMenuInfluence)
//	COMMAND_ID_HANDLER(IDMENU_JOINTDEL, OnMenuDelJoint)
//	COMMAND_ID_HANDLER(IDMENU_CONV2SCOPE, OnMenuConv2Scope)

	NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnSelChanged)
	NOTIFY_CODE_HANDLER(TVN_SELCHANGING, OnSelChanging)

	MESSAGE_HANDLER(WM_COMMAND, OnCommand)

ALT_MSG_MAP(SIGEDIT_MSGMAP)
	// スーパークラス SysTreeView32 のメッセージ マップ エントリに置き換えます。
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)

END_MSG_MAP()


	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


//	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnSelChanged( int idCtrl, LPNMHDR pnmh, BOOL& bHandled );
	LRESULT OnSelChanging( int idCtrl, LPNMHDR pnmh, BOOL& bHandled );



	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	int ChangeTVParams( int editflag, char* shdname );

public:
	CSigEditWnd();
	~CSigEditWnd();

//	int SetDbgFile( long dbgf );
	int SetAppWnd( HWND appwnd, CMyD3DApplication* papp );
	int SetParams( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, HWND appwnd );
	int Redraw();
	int OnUserSelchange( long selno, int usercall = 1 );
	int InvalidateHandler();
	int Rename( int seri, char* srcname );
	int GetSelectedNo( int* selnoptr );

	int OnMenuShdParam();
	int OnMenuDelJoint( int srcdel, int dlgflag );
	int OnMenuConv2Scope();

	int ForbidSelect( int selno );
	int SaveForbidSelect();

	int SetForbidDD( int flag );

private:
	int FillTree();
	void AddTree( int srcseri, int addbroflag, int addtolast );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	int MakeMoveList();
	void SetMoveListReq( CShdElem* srcselem );
	int IsValidMove();//自分の子項目への移動の禁止、自分の親への移動(意味がない)の禁止。
	int MoveTree();

	CShdElem* IsMorphObj( CShdElem* srcselem );
	void ErrorMes( int errorcode, int type, char* mes );

public:
	int m_forbidDD;
	int m_selectedno;
	int m_clicked_lbutton;
	int m_onitembutton;

private:
	CMyD3DApplication* m_papp;
	HWND	m_apphwnd;
	HMENU m_menu;
	CContainedWindow m_ctlSysTreeView32;
	CWindow m_chkbutton;


	CTreeHandler2* m_thandler;
	CShdHandler* m_shandler;
	CMotHandler* m_mhandler;


	//for treeview
	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	HTREEITEM m_dropitem;
	int m_dropno;

	HTREEITEM m_movelist[256];
	int m_movenum;
	int m_invalidate_handler;

	HIMAGELIST m_hImageList;
	int m_PartImageNo;
	int m_PartSelectNo;
	int m_DispImageNo;
	int m_DispSelectNo;
	int m_BoneImageNo;
	int m_BoneSelectNo;

	int m_usercallflag;
};




#endif