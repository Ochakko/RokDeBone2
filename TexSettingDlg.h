// ShdParamDlg.h : CTexSettingDlg の宣言

#ifndef __TEXSETTINGDLG_H_
#define __TEXSETTINGDLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>
class CTreeElem2;
class CShdElem;


/////////////////////////////////////////////////////////////////////////////
// CTexSettingDlg
class CTexSettingDlg : 
	public CAxDialogImpl<CTexSettingDlg>
{
public:
	CTexSettingDlg( CShdElem* lpselem );
	~CTexSettingDlg();

	//int DoModal( HWND hWndParent = ::GetActiveWindow(), 
	//	LPARAM dwInitParam = NULL );

	enum { IDD = IDD_TEXSETTINGDLG };

BEGIN_MSG_MAP(CTexSettingDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_NAME_ADD, OnNameAdd)
	COMMAND_ID_HANDLER(IDC_NAME_DEL, OnNameDel)
	COMMAND_ID_HANDLER(IDC_NAME_UP, OnNameUp)
	COMMAND_ID_HANDLER(IDC_NAME_DOWN, OnNameDown)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnNameAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNameUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNameDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	


public:
	CShdElem* m_selem;


	char m_texname[256];
	char* m_exttexlist[MAXEXTTEXNUM];
	char m_exttexname[MAXEXTTEXNUM][256];

	unsigned char m_exttexnum;
	unsigned char m_exttexmode;
	unsigned char m_exttexrep;
	unsigned char m_exttexstep;

	int m_texrepx;
	int m_texrepy;
	int m_transparent;
	float m_uanime;
	float m_vanime;
	int m_texrule;

	CWindow m_dlg_wnd;
	
	CWindow m_list_texname_wnd;
	
	CWindow m_combo_texrule_wnd;
	CWindow m_combo_anim_rep_wnd;

	CWindow m_texrepu_wnd;
	CWindow m_texrepv_wnd;
	CWindow m_uanim_wnd;
	CWindow m_vanim_wnd;
	CWindow m_anim_step_wnd;

	CWindow m_radio_multi_wnd;
	//CWindow m_check_transparent_wnd;

	CWindow m_ok_wnd;
	CWindow m_cancel_wnd;
	CWindow m_name_add_wnd;
	CWindow m_name_del_wnd;
	CWindow m_name_up_wnd;
	CWindow m_name_down_wnd;

	int m_isfullpath;

private:

	int Params2Dlg();
	int Get3Digit( int srcint, char* dstptr );

	int EnableButtons( int srcflag );
	int TextureParamsToDlg();

	int CheckIsFullPath();

};

#endif //__SHDPARAMDLG_H_
