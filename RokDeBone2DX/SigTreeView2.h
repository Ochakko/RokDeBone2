// SigTreeView2.h : CSigTreeView2 の宣言
#pragma once
#include "resource.h"       // メイン シンボル
#include <atlctl.h>
#include "RokDeBone2.h"
#include <commctrl.h>

#define DBGH
#include <dbg.h>

class CTreeHandler2;
class CShdHandler;
class CMotHandler;

class CShdElem;



// CSigTreeView2
class ATL_NO_VTABLE CSigTreeView2 : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ISigTreeView2, &IID_ISigTreeView2, &LIBID_RokDeBone2Lib, /*wMajor =*/1, /*wMinor =*/0>,
	public IPersistStreamInitImpl<CSigTreeView2>,
	public IOleControlImpl<CSigTreeView2>,
	public IOleObjectImpl<CSigTreeView2>,
	public IOleInPlaceActiveObjectImpl<CSigTreeView2>,
	public IViewObjectExImpl<CSigTreeView2>,
	public IOleInPlaceObjectWindowlessImpl<CSigTreeView2>,
	public ISupportErrorInfo,
	public IPersistStorageImpl<CSigTreeView2>,
	public ISpecifyPropertyPagesImpl<CSigTreeView2>,
	public IQuickActivateImpl<CSigTreeView2>,
	public IDataObjectImpl<CSigTreeView2>,
	public IProvideClassInfo2Impl<&CLSID_SigTreeView2, NULL, &LIBID_RokDeBone2Lib>,
	public CComCoClass<CSigTreeView2, &CLSID_SigTreeView2>,
	public CComControl<CSigTreeView2>
{
public:
	CContainedWindow* m_ctlSysTreeView32;

#pragma warning(push)
#pragma warning(disable: 4355) // 'this' : ベース メンバ初期化リストで使用されました。

	CSigTreeView2();
	~CSigTreeView2();

#pragma warning(pop)

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_SIGTREEVIEW2)

BEGIN_COM_MAP(CSigTreeView2)
	COM_INTERFACE_ENTRY(ISigTreeView2)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CSigTreeView2)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// エントリの例
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CSigTreeView2)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnSelChanged)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	CHAIN_MSG_MAP(CComControl<CSigTreeView2>)
ALT_MSG_MAP(1)
	// スーパークラス SysTreeView32 のメッセージ マップ エントリに置き換えます。
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);(& )

	BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
	{


		//if( pMsg->message == WM_KEYDOWN ){
		//	DbgOut( "SigTreeView2 : PreTranslateAccelerator : KEYDOWN : %d, ctrl %d, shift %d\n", pMsg->wParam, VK_CONTROL, VK_SHIFT );
		//}else if( pMsg->message == WM_KEYUP ){
		//	DbgOut( "SigTreeView2 : PreTranslateAccelerator : KEYUP : %d, ctrl %d, shift %d\n", pMsg->wParam, VK_CONTROL, VK_SHIFT );
		//}


		if(pMsg->message == WM_KEYDOWN)
		{
			switch(pMsg->wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
			case VK_HOME:
			case VK_END:
			case VK_NEXT:
			case VK_PRIOR:
				hRet = S_FALSE;
				return TRUE;
			}
		}
		//TODO: アクセラレータ初期コードをここに追加してください。
		return FALSE;
	}

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = CComControl<CSigTreeView2>::OnSetFocus(uMsg, wParam, lParam, bHandled);
		if (m_bInPlaceActive)
		{
			if(!IsChild(::GetFocus())){
				if( m_ctlSysTreeView32 ){
					m_ctlSysTreeView32->SetFocus();
				}
			}
		}
		return lRes;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	STDMETHOD(SetObjectRects)(LPCRECT prcPos,LPCRECT prcClip);

	LRESULT OnSelChanged( int idCtrl, LPNMHDR pnmh, BOOL& bHandled );

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_ISigTreeView2,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ISigTreeView2

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease();
public:
	STDMETHOD(ShowInfluenceDlg)();
	STDMETHOD(NewJoint)();
	STDMETHOD(DelJoint)();
	STDMETHOD(Conv2Scope)();
	STDMETHOD(ShowJointLocDlg)();
	STDMETHOD(ShowShdParamDlg)();
	STDMETHOD(InvalidateHandler)();
	STDMETHOD(OnUserSelchange)(long selno);
	STDMETHOD(OnMove)();
	STDMETHOD(Redraw)();
	STDMETHOD(SetDbgFile)(long dbgf);
	STDMETHOD(SetParams)(int sx, int sy, long lpth, long lpsh, long lpmh, long appwnd);
	STDMETHOD(Rename)( int seri, long srcname );
	STDMETHOD(GetSelectedNo)( int* selnoptr );
	STDMETHOD(SizeChange)( int posx, int posy, int newx, int newy );
	STDMETHOD(GetCtrlSize)( int* cxptr, int* cyptr );
	STDMETHOD(UDWindow)();

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	int ChangeTVParams( int editflag, char* shdname );

	CTreeHandler2* m_thandler;
	CShdHandler* m_shandler;
	CMotHandler* m_mhandler;
	HTREEITEM* m_TI;
	HTREEITEM m_selecteditem;
	int m_selectedno;


	int m_clicked_lbutton;
	HTREEITEM m_dropitem;
	int m_dropno;

	HTREEITEM m_movelist[256];
	int m_movenum;

	int m_invalidate_handler;
private:
	
	//int ShowShdParamDlg();
	int FillTree();
	void AddTree( int srcseri, int addbroflag, int addtolast );
	HTREEITEM TVAdd( HTREEITEM parentTI, char* srcname, int srcno, int imageno, int selectno, int addtolast );

	int MakeMoveList();
	void SetMoveListReq( CShdElem* srcselem );
	int IsValidMove();//自分の子項目への移動の禁止、自分の親への移動(意味がない)の禁止。
	int MoveTree();

	int IsMorphObj( CShdElem* srcselem );
	void ErrorMes( int errorcode, int type, char* mes );


	HIMAGELIST m_hImageList;
	//int m_iImage;
	//int m_iSelect;
	int m_PartImageNo;
	int m_PartSelectNo;
	int m_DispImageNo;
	int m_DispSelectNo;
	int m_BoneImageNo;
	int m_BoneSelectNo;


	HWND m_apphwnd;
	int m_onitembutton;
};

OBJECT_ENTRY_AUTO(__uuidof(SigTreeView2), CSigTreeView2)
