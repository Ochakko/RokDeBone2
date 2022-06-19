// SndKeyDlg.h : CSndKeyDlg の宣言
#ifndef __SndKeyDlg_H_
#define __SndKeyDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <d3dx9.h>
#include <commctrl.h>

#include <coef.h>
#include <quaternion.h>

#include <ATagHandler.h>
#include <ATag.h>

class CMyD3DApplication;
class CMotParamDlg;
class CSndAnimHandler;
class CSndAnim;
class CSndKey;
class CSndFullFrame;
class CSndFrame;
class CSoundBank;
class CSoundSet;


/////////////////////////////////////////////////////////////////////////////
// CSndKeyDlg
class CSndKeyDlg : 
	public CAxDialogImpl<CSndKeyDlg>
{
public:
	enum { IDD = IDD_SNDKEYDLG };


	CSndKeyDlg( CMotParamDlg* srcmpdlg, CMyD3DApplication* srcapp );
	~CSndKeyDlg();
	
	int SetAppWnd( HWND appwnd );
	int Redraw();
	//int OnUserSelchange();
	int ShowDlg( int cmdshow, int stopflag = 1 );
	int DestroyTimer();
	int InitDlg( int atshowflag );
	int ResetParams( int atshowflag );

	int GetSndAnimNum();
	int LoadSndFile( char* filename, int dlgwarnflag = 1 );

	MOTID* GetSndMotIDArray();
	char* GetSndAnimName( int animno );
	int GetCurSndAnimIndex();
	int GetCurSndAnimNo();
	int SelectSndAnim( int animindex );

	int DestroySndAnim( int delanimno );

//	int SetCurSnd( SNDELEM srcsnde );
	int SetPreviewButton();

//	int CalcMotionPointOnFrame( int animno, int frameno, CMState** ppms, int* hasmpflag );
	int ExistSndKey( int animno, int frameno, CSndKey** ppsndk );
	int CalcFrameData();
	int ChangeFPS( int srcfps );

	int GetSndFrame( int befstartcnt, int* dstframe, int* dstdiff );
	int StartPreview( int srcframe );
	int StopBGM();
	int PlayAnim( int srcframe );
	int GetBGMStopCnt( int* cntptr );
	int GetBGMStartCnt( int* cntptr );

	int GetCurTag( const int animno, const double srctime, std::vector<CATag>& rettag );


BEGIN_MSG_MAP(CSndKeyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)

	//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_PREVIEW, OnPreview)
	COMMAND_ID_HANDLER(IDC_STOP, OnStop)
	COMMAND_ID_HANDLER(IDC_FRAMECOPY, OnFrameCopy)
	COMMAND_ID_HANDLER(IDC_FRAMEPASTE, OnFramePaste)

	COMMAND_ID_HANDLER(IDC_BEFORE_MP, OnBeforeMP)
	COMMAND_ID_HANDLER(IDC_NEXT_MP, OnNextMP)

	COMMAND_ID_HANDLER(IDC_FRAMEDEL, OnDelete)
	COMMAND_ID_HANDLER(IDC_CREATEKEY, OnCreate)
	COMMAND_ID_HANDLER(IDC_ALLDELETE, OnAllDelete)

	COMMAND_ID_HANDLER(IDC_SETTING, OnSetting)
	COMMAND_ID_HANDLER(IDC_CALC, OnCalc)

	COMMAND_ID_HANDLER(IDC_ADDELEM, OnAddElem)
	COMMAND_ID_HANDLER(IDC_DELELEM, OnDelElem)
	COMMAND_ID_HANDLER(IDC_ELEMLIST, OnSelElem)

	COMMAND_ID_HANDLER(IDC_ADDTAG, OnAddTag)
	COMMAND_ID_HANDLER(IDC_DELTAG, OnDelTag)
	COMMAND_ID_HANDLER(IDC_TAGLIST, OnSelTag)
	COMMAND_ID_HANDLER(IDC_ENDTAG, OnEndTag)

	COMMAND_ID_HANDLER(IDC_SSCOMBO, OnSSCombo)
	COMMAND_ID_HANDLER(IDC_SNDSYNC, OnSyncFlag)

	NOTIFY_HANDLER(IDC_SP_MP, UDN_DELTAPOS, OnSpin_Sp_mp )
	NOTIFY_HANDLER(IDC_SL_MP, NM_RELEASEDCAPTURE, OnReleasedcaptureSl_mp)


	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_MOVING, OnMoving)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)

	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterMove)
	MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitMove)

//ALT_MSG_MAP(2)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)

END_MSG_MAP()

	//BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnFrameCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFramePaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnBeforeMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNextMP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSetting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAddElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelElem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnAddTag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDelTag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelTag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEndTag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSSCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSyncFlag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSpin_Sp_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnReleasedcaptureSl_mp(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAllDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnterMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int ParamsToDlg();
	int SetCurrentMotion( int isfirst );
	int SetFrameNo( int srcframeno );

	int RemakeSndAH();
	int OnJumpFrame( int srcframe );

	int WriteSndAnimFile( char* filename );
	int LoadSndAnimFile( char* filename, int dlgwarnflag );

	int SetStartAndEnd( int srcstart, int srcend );


private:
	int InitParams();
	int DestroyObjs( int delsndah );

	int SetWnd();
	void SetSliderTic( CWindow dstwnd, int ticfreq, int mintic, int maxtic );

	int DrawSndKeys();

	int Frameno2Windowpos( RECT clirect, int frameno, int* posptr );
	int Windowpos2Frameno( RECT clirect, int posx, int* framenoptr );

	int SetNewWindowProc();

	int InitComboSS();
	int InitComboSE();

	int InitList();

	int CaptureM();
	CSoundSet* GetCurSS();

	int SetSyncButton();
public:
	CSndAnimHandler* m_sndah;
	CATagHandler* m_tagh;
	std::vector<CATag> m_curtag;

	LONG m_current;
	LONG m_startframe;
	LONG m_endframe;
	int m_sl_mp_rangemax;
	LONG m_previewframe;

	int m_ismoving;
	CWindow m_sl_mp_wnd;
	LONG m_animno;
	int m_syncflag;

	int m_ctrldrag;
	int m_altdrag;
	int m_dragx;

	int m_savebmdisp;
	int m_savejdisp;

private:
	CMotParamDlg* m_dlg;
	CMyD3DApplication* m_app;	

	CSndKey* m_dragsndk;
	int m_dragframe;
	int m_dragflag;
	CSndKey* m_cpsndk;

	SNDELEM m_snde;
	SNDELEM m_zerosnde;
	MOTID m_motid[ MAXMOTIONNUM ];

	CWindow m_dlg_wnd;
	CWindow m_tex_current_wnd;

	CWindow m_mp_wnd;
	CWindow m_sp_mp_wnd;

	CWindow m_preview_wnd;
	CWindow m_stop_wnd;

	CWindow m_sndsync_wnd;
	
	CWindow m_sscombo_wnd;
	CWindow m_secombo_wnd;

	CWindow m_elemlist_wnd;
	CWindow m_bgm_wnd;

	CWindow m_edittag_wnd;
	CWindow m_taglist_wnd;

	HPEN	m_black_pen;
	HPEN	m_red_pen;
	HPEN	m_blue_pen;
	HBRUSH	m_black_brush;
	HBRUSH	m_white_brush;
	HBRUSH	m_red_brush;
	HBRUSH	m_blue_brush;

	int m_timerworking;
	int m_cmdshow;
	int m_mcap;
	int m_moving;

	char m_curssname[32];
	int m_sscomboindex;
	int m_secomboindex;
	int m_elemindex;

};

#endif //__SndKeyDlg_H_
