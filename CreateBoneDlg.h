// CreateBoneDlg.h : CCreateBoneDlg の宣言

#ifndef __CreateBoneDlg_H_
#define __CreateBoneDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <coef.h>

#define CREATENAMELEN	256

class CMyD3DApplication;
class CShdElem;

/////////////////////////////////////////////////////////////////////////////
// CCreateBoneDlg
class CCreateBoneDlg : 
	public CAxDialogImpl<CCreateBoneDlg>
{
public:
	CCreateBoneDlg( CMyD3DApplication* papp );
	~CCreateBoneDlg();

	enum { IDD = IDD_CREATEBONEDLG };

BEGIN_MSG_MAP(CCreateBoneDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_SETNAME, OnSetName)

	COMMAND_ID_HANDLER(IDC_RADIO_CREATE, OnRadioCreate)
	COMMAND_ID_HANDLER(IDC_RADIO_POS, OnRadioPos)

	COMMAND_ID_HANDLER(IDC_DELBONE, OnDelBone)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSetName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnRadioCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRadioPos(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	int ShowDlg( int cmdshow );
	int SetCurrentName( char* srcname );
	CShdElem* GetSelectedJoint( int* isjoint );

	int m_ope;


private:
	CMyD3DApplication* m_papp;
	char name[CREATENAMELEN];

	CWindow m_dlg_wnd;
	CWindow m_name_wnd;

	int m_cmdshow;


private:
	void SetWnd();
	int ParamsToDlg();


};

#endif //__CreateBoneDlg_H_
