// InfluenceDlg.h : CSoundBankDlg の宣言

#ifndef __SoundBankDlg_H_
#define __SoundBankDlg_H_

#include "resource.h"       // メイン シンボル

#include <atlbase.h>
#include <atlhost.h>

#include <coef.h>


typedef struct tag_sndcombo
{
	char name[256];
	int bgmindex;
	int sndindex;
} SNDCOMBO;

class CHuSound;
class CSoundBank;
class CSoundSet;

/////////////////////////////////////////////////////////////////////////////
// CSoundBankDlg
class CSoundBankDlg : 
	public CAxDialogImpl<CSoundBankDlg>
{
public:
	CSoundBankDlg( CHuSound* srchs, CSoundBank* srcsb );
	~CSoundBankDlg();

	enum { IDD = IDD_SOUNDDLG };

BEGIN_MSG_MAP(CSoundBankDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_ADDSET, OnAddSet)
	COMMAND_ID_HANDLER(IDC_DELSET, OnDelSet)
	COMMAND_ID_HANDLER(IDC_UPSET, OnUpSet)
	COMMAND_ID_HANDLER(IDC_DOWNSET, OnDownSet)
	COMMAND_ID_HANDLER(IDC_SETLIST, OnSelChangeSetList)
	COMMAND_ID_HANDLER(IDC_APPLYDIR, OnApplyDir)
	COMMAND_ID_HANDLER(IDC_APPLYBGM, OnApplyBGM)

	COMMAND_ID_HANDLER(IDC_ADDSOUND, OnAddSound)
	COMMAND_ID_HANDLER(IDC_DELSOUND, OnDelSound)
	COMMAND_ID_HANDLER(IDC_UPSOUND, OnUpSound)
	COMMAND_ID_HANDLER(IDC_DOWNSOUND, OnDownSound)
	COMMAND_ID_HANDLER(IDC_SOUNDLIST, OnSelChangeSoundList)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAddSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUpSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDownSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelChangeSetList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnApplyDir(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnApplyBGM(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAddSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUpSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDownSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelChangeSoundList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	void InitParams();
	int DestroyObjs();
	int DestroySndCombo();

	int ParamsToDlg( int updflag );
	int InitSetList();
	int ParamsToDir();
	int ParamsToBGM();
	int ParamsToSound();

	int SetSndCombo( SNDCOMBO* scom, int* dstnum );
	int FindSndCombo( char* srcname );


	int ResetBGM( int delflag );
	int ResetSound( int delflag );

	int InitComboBGM();
	int InitComboSound();


	int IsSoundFile( WIN32_FIND_DATA* pwf );
	int MakePathName( int setdirflag );

	int GetComboIndexByName( char* srcname );

private:
	CWindow m_dlg_wnd;

	CWindow m_setname_wnd;
	CWindow m_setlist_wnd;

	CWindow m_dir_wnd;
	
	CWindow m_bgm_wnd;
	CWindow m_bgmrev_wnd;

	CWindow m_snd_wnd;
	CWindow m_sndlist_wnd;
	CWindow m_sndrev_wnd;
	CWindow m_sndbufnum_wnd;

	CSoundBank* m_soundbank;
	CHuSound* m_husound;
public:
	int m_setlistno;
	CSoundSet* m_curss;


	char m_dir[MAX_PATH];
	char m_pathname[MAX_PATH];

	int m_sndcombonum;
	SNDCOMBO* m_sndcombo;

	int m_bgmlistno;
	int m_sndcomboindex;
	int m_sndlistindex;
};

#endif //__SoundBankDlg_H_
