// InfScopeDlg.h : CInfScopeDlg の宣言

#ifndef __InfScopeDlg_H_
#define __InfScopeDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <commctrl.h>
#include <D3DX9.h>

class CMotHandler;
class CTreeHandler2;
class CShdHandler;


class CMyD3DApplication;

/////////////////////////////////////////////////////////////////////////////
// CInfScopeDlg
class CInfScopeDlg : 
	public CAxDialogImpl<CInfScopeDlg>
{
public:
	CInfScopeDlg( CMyD3DApplication* papp );
	~CInfScopeDlg();

	enum { IDD = IDD_INFSCOPEDLG };

BEGIN_MSG_MAP(CInfScopeDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_TREE1, TVN_SELCHANGED, OnSelchangedTree1)
	COMMAND_ID_HANDLER(IDC_LIST1, OnSelchangeList1)

	COMMAND_ID_HANDLER(IDC_CREATE, OnCreateMesh)
	COMMAND_ID_HANDLER(IDC_DELETE, OnDeleteMesh)

	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	NOTIFY_HANDLER(IDC_SLIDER_BONE, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_bone)
	NOTIFY_HANDLER(IDC_SLIDER_VERT, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_vert)
	NOTIFY_HANDLER(IDC_SLIDER_HOR, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_hor)
	NOTIFY_HANDLER(IDC_SLIDER_ALL, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_all)


	//MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	COMMAND_ID_HANDLER(IDC_REFCOL, OnRefCol)

	COMMAND_ID_HANDLER(IDC_RADIO_MVALL, OnRadioMVALL)
	COMMAND_ID_HANDLER(IDC_RADIO_MVVERT, OnRadioMVVERT)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangeList1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCreateMesh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeleteMesh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_bone(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_vert(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_hor(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_all(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


	//LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnRefCol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnRadioMVALL(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioMVVERT(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int ShowDlg( int cmdshow );

	int MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftface, int* shifvert, int shifvertnum, float pointx, float pointy );
	int MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftface, int* shifvert, int shifvertnum, int kind, float delta );

	int SetPoint();

	int OnUserSelChange( int selno );

private:
	int SetNewWindowProc();

	int FillTree();
	void AddBoneToTree( int srcseri, int addbroflag, int addtolast );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	void CreateImageList();
	int ParamsToDlg();

	int CreateList();

	int DestroyObjs();
	int InitTreeAndList();

	int MultPoint( float mult, int kind );
	int PaintRect( CWindow srcwnd, HDC hdc, HBRUSH brush );

	int AddItem2List2( int isindex );
	int DeleteItemFromList2( int isindex );
	int CheckItemInList2();

	int SetDispFlagFromList2();


	int MoveVertexScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int* shifvert, int shifvertnum, float pointx, float pointy );
	int MoveVertexRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int* shifvert, int shifvertnum, D3DXVECTOR3 relvec );
	int MoveVertexDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int* shifvert, int shifvertnum, int kind, float delta );

	int MovePartScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, float pointx, float pointy );
	int MovePartDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int kind, float delta );

	int GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir );
	int DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec );

public:
	int m_isindex;
	int m_object;
	D3DXVECTOR3 m_worldpos;
	int m_selbone;

private:
	CMyD3DApplication* m_papp;
	int m_cmdshow;
	
	CWindow m_dlg_wnd;
	CWindow m_tree_wnd;
	CWindow m_list_wnd;
	CWindow m_list2_wnd;

	CWindow m_create_wnd;
	CWindow m_delete_wnd;

	CWindow m_sl_bone_wnd;
	CWindow m_sl_vert_wnd;
	CWindow m_sl_hor_wnd;
	CWindow m_sl_all_wnd;

	CWindow m_col_wnd;

	HIMAGELIST m_hImageList;
	int m_iImage;
	int m_iSelect;

	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	

	//CTreeHandler2* m_thandler;
	//CShdHandler* m_shandler;
	int m_seldisp;

	HWND	m_apphwnd;
	int* m_topflag;


	int m_pointnum;
	D3DXVECTOR3* m_point;

	HDC m_col_dc;
	HBRUSH m_col_b;
	COLORREF m_col;

	int m_timerid;
	int m_paramstodlg;

	D3DXVECTOR3 m_center;

	int m_createlistflag;
	int m_savepolydisp;

};

#endif //__InfScopeDlg_H_
