// PaintDlg.h : CPaintDlg の宣言

#ifndef __PaintDlg_H_
#define __PaintDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include "resid.h"

#include <coef.h>


#define INFPAINTUNDOMAX	50

/***
typedef struct tag_infpaintundo
{
	int partno;
	int vertnum;
	int vertno[PICKVERTMAX];
	int selbone;
	int infnum;
	INFELEM inf[INFNUMMAX];
}INFPAINTUNDO;
***/
typedef struct tag_infpaintundo
{
	int partno;
	int vertnum;
	int* pvertno;//vertnumの配列
	int selbone;
	int* pinfnum;//vertnumの配列
	INFELEM* pinf;//INFNUMMAX * vertnumの配列
}INFPAINTUNDO;


class CMyD3DApplication;

/////////////////////////////////////////////////////////////////////////////
// CPaintDlg
class CPaintDlg : 
	public CAxDialogImpl<CPaintDlg>
{
public:
	CPaintDlg( CMyD3DApplication* papp );
	~CPaintDlg();

	enum { IDD = IDD_PAINTDLG };

BEGIN_MSG_MAP(CPaintDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_TREE1, TVN_SELCHANGED, OnSelchangedTree1)

	COMMAND_ID_HANDLER(IDC_RADIO_ONESKIN0, OnRadioOneskin0)
	COMMAND_ID_HANDLER(IDC_RADIO_ONESKIN1, OnRadioOneskin1)
	COMMAND_ID_HANDLER(IDC_RADIO_NOSKIN0, OnRadioNoskin0)
	COMMAND_ID_HANDLER(IDC_RADIO_DIRECT0, OnRadioDirect0)
	COMMAND_ID_HANDLER(IDC_RADIO_SYM, OnRadioSym)

	COMMAND_ID_HANDLER(IDC_RADIO_DISPBONE, OnRadioDispBone)
	COMMAND_ID_HANDLER(IDC_RADIO_DISPSYM, OnRadioDispSym)


	COMMAND_ID_HANDLER(IDC_NOR, OnSelNor)
	COMMAND_ID_HANDLER(IDC_EXC, OnSelExc)
	COMMAND_ID_HANDLER(IDC_ADD, OnSelAdd)
	COMMAND_ID_HANDLER(IDC_SUB, OnSelSub)
	COMMAND_ID_HANDLER(IDC_ERA, OnSelEra)

	COMMAND_ID_HANDLER(IDC_SYMX, OnSymx)
	COMMAND_ID_HANDLER(IDC_SYMY, OnSymy)
	COMMAND_ID_HANDLER(IDC_SYMZ, OnSymz)

	COMMAND_ID_HANDLER(IDC_SYMRENEW, OnSymRenew)
	COMMAND_ID_HANDLER(IDC_SYMLUMP, OnSymLump)

	COMMAND_ID_HANDLER(IDC_ZEROV, OnZeroVert)

	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	NOTIFY_HANDLER(IDC_SL_PER, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_per)

	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroyDlg)


	COMMAND_ID_HANDLER(IDC_RADIO_SETINF, OnRadioSetinf)
	COMMAND_ID_HANDLER(IDC_RADIO_BONE, OnRadioBone)

	COMMAND_ID_HANDLER(IDC_CHK_NORM, OnChkNorm)
	COMMAND_ID_HANDLER(IDC_UNDO, OnUndo)
	COMMAND_ID_HANDLER(IDC_DEF, OnDef)
	COMMAND_ID_HANDLER(IDC_NORM, OnNormalize)

	COMMAND_ID_HANDLER(IDC_COMBO_SELDISP, OnSeldisp)
	COMMAND_ID_HANDLER(IDC_COMBO_BRUSH, OnBrush)
	COMMAND_ID_HANDLER(IDC_APPLYBRUSH, OnApplyBrushSize)


	MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterMove)
	MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitMove)
	
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangedTree1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnRadioOneskin0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioOneskin1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioNoskin0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioDirect0(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioSym(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZeroVert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnRadioDispBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioDispSym(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnSelNor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelExc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelSub(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelEra(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSymx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSymy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSymz(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSymRenew(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSymLump(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_per(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnRadioSetinf(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnChkNorm(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNormalize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSeldisp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBrush(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyBrushSize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	int ShowDlg( int cmdshow );
	int OnClickVert( int seldisp, int* selvert, int selvertnum, int isleftclick );
	int InitUndoBuf();

	int OnUserSelChange( int selno );

private:
	int SetNewWindowProc();
	int FillTree();
	void AddBoneToTree( int srcseri, int addbroflag, int addtolast );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	void CreateImageList();
	int ParamsToDlg();

	int DestroyObjs();

	int InitTree();
	int InitList();
	int InitList1Vert();
	int InitListMulti();
	int InitSeldispCombo();
	int InitBrushCombo();
	int GetCalcMode();
	int CheckPaintModeButton();
	int CheckSymAxisButton();
	int AddItem2List( int itemno, INFELEM* IEptr );
	int CreateTimer();
	int DestroyTimer();
	int SaveMaterial();
	int RestoreMaterial();

	int SetInfElem( int forceblue );
	int DeleteInfElem( int* pinfnum );

	int AddUndoBuf( int partno, int* vertno, int vertnum, int selbone, int* pinfnum, INFELEM* pinf );

	int Conv2SymIE( INFELEM* srcIE, int infnum, INFELEM* dstIE );
	int GetSymJointNo( char* srcname, int srcno, int symkind );
	int CopySymInf( int vno );
	int TreeViewSelChange( int srcseri );

	int SetSymIE( int sympart, int symvert, int* syminfnumptr );

	int DestroyDOI();

	int ListPropMulti( POINT inview );
	int ListProp1Vert( POINT inview );

	int ClickOpe( int isleftclick, int forceblue );

public:
	int m_ope;

	int m_targetdisp;
	int m_targetvert[VERTNUMMAX];
	int m_targetvertnum;

	int m_selbone;

	int m_brushtype;
	float m_brushsize;
	D3DXVECTOR3 m_pickpos;

	int m_markpart;
	int m_markvert;

private:
	int m_symaxis;
	float m_symdist;
	int m_timerid;

	int m_cmdshow;
	CWindow m_dlg_wnd;
	CWindow m_tree_wnd;
	CWindow m_list_wnd;

	CWindow m_nor_wnd;
	CWindow m_exc_wnd;
	CWindow m_add_wnd;
	CWindow m_sub_wnd;
	CWindow m_era_wnd;

	CWindow m_sl_per_wnd;
	CWindow m_per_wnd;

	CWindow m_direct_wnd;

	CWindow m_symx_wnd;
	CWindow m_symy_wnd;
	CWindow m_symz_wnd;
	CWindow m_symdist_wnd;

	CWindow m_seldisp_wnd;
	CWindow m_brush_wnd;
	CWindow m_brushsize_wnd;

	HIMAGELIST m_hImageList;
	int m_iImage;
	int m_iSelect;

	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;

	CMyD3DApplication* m_papp;
	int* m_topflag;

	int m_createlistflag;

	int m_calcmode;
	int m_paintmode;
	int m_dispmode;
	float m_per;
	float m_saveper;


	int m_seldisp;
	int m_selvert[VERTNUMMAX];
	int m_selvertnum;

	int m_check_normalize;

	float m_directval;

	INFPAINTUNDO m_undo[ INFPAINTUNDOMAX ];
	int m_undopos;

	int m_sympart;
	int m_symvert;

	int m_syminfnum;
	INFELEM m_symIE[ INFNUMMAX ];

	HWND m_editwnd;


	int m_mcap;
	int m_lbutton;
	int m_moving;

	int m_undersymrenew;

	int m_dispobjnum;
	DISPOBJINFO* m_doi;

	int m_savepolydisp;

	int m_dispvertnum;
	int m_columnnum;
};

#endif //__PaintDlg_H_
