// ShdParamDlg.h : CShdParamDlg の宣言

#ifndef __SHDPARAMDLG_H_
#define __SHDPARAMDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>
class CTreeElem2;
class CShdElem;
class CShdHandler;
class CMotHandler;
class CTreeHandler2;
class CSigEditWnd;
//class CSigTreeView2;

//変更の種類を表すflag
#define TV_EDIT_SHDNAME		0x01;
#define TV_EDIT_CLOCKWISE	0x02;
//#define TV_EDIT_TEXNAME		0x04;
//#define TV_EDIT_TEXREPX		0x08;
//#define TV_EDIT_TEXREPY		0x010;
//#define TV_EDIT_TRANSPARENT	0x020;
#define TV_EDIT_ALPHA		0x040;
//#define TV_EDIT_UANIME		0x080;
//#define TV_EDIT_VANIME		0x0100;
//#define TV_EDIT_TEXRULE		0x0200;
#define	TV_EDIT_COLOR		0x0400;
#define TV_EDIT_NOTUSE		0x0800;

#define TV_EDIT_NOSKINNING		0x1000	
#define TV_EDIT_ALL_NOSKINNING	0x2000
#define TV_EDIT_ALL_SKINNING	0x4000

#define TV_EDIT_DISPSWITCHNO	0x8000

#define TV_EDIT_TEXTURE			0x010000

#define TV_EDIT_NOTRANSIK		0x020000
#define TV_EDIT_IKSKIP			0x040000

/////////////////////////////////////////////////////////////////////////////
// CShdParamDlg
class CShdParamDlg : 
	public CAxDialogImpl<CShdParamDlg>
{
public:
	CShdParamDlg( CTreeElem2* lptelem, CShdElem* lpselem, CTreeHandler2* srclpth, CShdHandler* srclpsh, CMotHandler* srclpmh, CSigEditWnd* stvptr );
	~CShdParamDlg();

	//int DoModal( HWND hWndParent = ::GetActiveWindow(), 
	//	LPARAM dwInitParam = NULL );

	enum { IDD = IDD_SHDPARAMDLG };

BEGIN_MSG_MAP(CShdParamDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	//COMMAND_ID_HANDLER(IDC_ALL_NOSKINNING, OnAllNoSkinning)
	//COMMAND_ID_HANDLER(IDC_ALL_SKINNING, OnAllSkinning)
	COMMAND_ID_HANDLER(IDC_BEFORE, OnBefore)
	COMMAND_ID_HANDLER(IDC_AFTER, OnAfter)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//	LRESULT OnRefDiffuse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//	LRESULT OnRefAmbient(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllNoSkinning(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllSkinning(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//	LRESULT OnTexture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnReleasedcaptureSl_specular(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnBefore(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAfter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	CShdHandler* m_shandler;
	CMotHandler* m_mhandler;
	CTreeHandler2* m_thandler;
	CShdElem* m_selem;
	CTreeElem2* m_telem;
	CSigEditWnd* m_stv;

	char m_shdname[1024];
	int m_serialno;
	int m_shdtype;
	int m_depth;
	int m_clockwise;
	//char m_texname[1024];
	//int m_texrepx;
	//int m_texrepy;
	//int m_transparent;
	float m_alpha;
	//float m_uanime;
	//float m_vanime;
	//int m_texrule;
	int m_notuse;
	int m_notransik;
	int m_noskinning;
	int m_ikskip;
	int m_invisible;

	DWORD m_dispswitchno;

	CWindow m_dlg_wnd;
	CWindow m_shdname_wnd;
	CWindow m_serialno_wnd;
	CWindow m_shdtype_wnd;
	CWindow m_depth_wnd;
	CWindow m_clockwise_wnd;
	//CWindow m_texname_wnd;
	//CWindow m_texrepx_wnd;
	//CWindow m_texrepy_wnd;
	//CWindow m_transparent_wnd;
//	CWindow m_alpha_wnd;
	//CWindow m_uanime_wnd;
	//CWindow m_vanime_wnd;
	//CWindow m_texrule_wnd;
	//CWindow m_colordisp_wnd;
	CWindow m_notuse_wnd;
	CWindow m_notransik_wnd;
	CWindow m_ikskip_wnd;
	CWindow m_invisible_wnd;

	CWindow m_numprim_wnd;

//	CWindow m_tex_diffuse_wnd;
//	CWindow m_tex_ambient_wnd;
//	CWindow m_tex_specular_wnd;

//	CWindow m_disp_diffuse_wnd;
//	CWindow m_disp_ambient_wnd;
	
//	CWindow m_sl_specular_wnd;
	CWindow m_ok_wnd;
	CWindow m_cancel_wnd;

//	CWindow m_ref_diffuse_wnd;
//	CWindow m_ref_ambient_wnd;
	//CWindow m_texref_wnd;
	CWindow m_text_status_wnd;

	CWindow m_check_skinning_wnd;
	CWindow m_all_noskinning_wnd;
	CWindow m_all_skinning_wnd;

	CWindow m_dispswitchno_wnd;

//	CWindow m_texture_wnd;

	HBRUSH	m_diffuse_brush;
	HBRUSH	m_ambient_brush;
	int		m_sl_specular;//sliderの値
	CVec3f	m_diffuse_color;
	CVec3f	m_ambient_color;
	CVec3f	m_specular_color;

	int editflag;


private:
	int ParamsToDlg();
	int ColorParamsToDlg();

	int ApplyParams( char* errmsg );


	int EnableButtons( int flag );




};

#endif //__SHDPARAMDLG_H_
