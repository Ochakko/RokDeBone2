// WriteMQODlg.h : CWriteMQODlg の宣言

#ifndef __WRITEMQODLG_H_
#define __WRITEMQODLG_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CWriteMQODlg
class CWriteMQODlg : 
	public CAxDialogImpl<CWriteMQODlg>
{
public:
	CWriteMQODlg( char* srcfilename );
	~CWriteMQODlg();

	enum { IDD = IDD_WRITEMQODLG };

BEGIN_MSG_MAP(CWriteMQODlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REF_HEAD, OnRefHead)
	//COMMAND_ID_HANDLER(IDC_REF_HEAD, OnRefBoneFile)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRefHead(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//LRESULT OnRefBoneFile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


public:
	int m_outmode;
	char m_head[2048];
	float m_mult;
	int m_outbone;

	//char m_bonefilename[2048];
	int m_outbonefile;
	int m_chkmorph;

private:
	CWindow m_dlg_wnd;
	CWindow m_edit_head_wnd;
	CWindow m_edit_mult_wnd;
	CWindow m_outbone_wnd;

	//CWindow m_edit_bonefile_wnd;


};

#endif //__WriteMQODlg_H_
