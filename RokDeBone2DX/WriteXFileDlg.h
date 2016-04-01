// InfluenceDlg.h : CWriteXFileDlg の宣言

#ifndef __WriteXFileDlg_H_
#define __WriteXFileDlg_H_

#include "resource.h"       // メイン シンボル

#include <atlbase.h>
#include <atlhost.h>

#include <coef.h>

#include "resid.h"

class CMotHandler;

/////////////////////////////////////////////////////////////////////////////
// CWriteXFileDlg
class CWriteXFileDlg : 
	public CAxDialogImpl<CWriteXFileDlg>
{
public:
	CWriteXFileDlg( CMotHandler* srclpmh, char* srcfilename, MOTID* srcmid, int srcmotnum );
	~CWriteXFileDlg();

	enum { IDD = IDD_WRITEXFILEDLG };

BEGIN_MSG_MAP(CWriteXFileDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_REFX, OnRefX)
	COMMAND_ID_HANDLER(IDC_ADD, OnAdd)
	COMMAND_ID_HANDLER(IDC_DEL, OnDel)
	COMMAND_ID_HANDLER(IDC_UP, OnUp)
	COMMAND_ID_HANDLER(IDC_DOWN, OnDown)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnRefX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


private:
	void InitParams();
	int DestroyObjs();
	int ParamsToDlg();
	int InitCombo();
	int InitList();


private:
	CWindow m_xname_wnd;
	CWindow m_mult_wnd;
	CWindow m_convface_wnd;

	CWindow m_combo_wnd;
	CWindow m_list_wnd;

	CWindow m_dlg_wnd;

	CMotHandler* lpmh;

public:
	char xfilename[MAX_PATH];
	float mult;
	int convface;

	int motnum;
	MOTID* motidarray;

	int wnum;
	NAMEID** ppni;

	int m_fullbone;
};

#endif //__WriteXFileDlg_H_
