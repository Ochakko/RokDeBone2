// VersionDlg.h : CVersionDlg �̐錾

#ifndef __VERSIONDLG_H_
#define __VERSIONDLG_H_

#include "resource.h"       // ���C�� �V���{��
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CVersionDlg
class CVersionDlg : 
	public CAxDialogImpl<CVersionDlg>
{
public:
	CVersionDlg()
	{
	}

	~CVersionDlg()
	{
	}

	enum { IDD = IDD_VERSIONDLG };

BEGIN_MSG_MAP(CVersionDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// �n���h���̃v���g�^�C�v:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};

#endif //__VERSIONDLG_H_
