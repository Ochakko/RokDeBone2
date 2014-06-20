// MotDlg.cpp : CMotDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "MotDlg.h"
#include "LoadNameDlg.h"
#include "GraphRotDlg.h"
#include "viewer.h"

#include "MotChangeDlg.h"
#include <MCHandler.h>

#include "SoundBank.h"
#include "SyncDlg.h"

#include <CameraAnimHandler.h>
#include <CameraAnim.h>
#include <GPAnimHandler.h>
#include <GPAnim.h>
#include <MOEAnimHandler.h>
#include <MOEAnim.h>
#include <SndAnimHandler.h>
#include <SndAnim.h>

#include <Commdlg.h>
#include <commctrl.h>

#include <coef.h>
#define DBGH
#include <dbg.h>

#include "inidata.h"

extern CIniData* g_inidata;
extern int g_endapp;


//#include <Afxdlgs.h>

/////////////////////////////////////////////////////////////////////////////
// CMotDlg

CMotDlg::CMotDlg( CMyD3DApplication* papp, LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp, int iklevelope )
{
	InitCommonControls();

	 m_app = papp;
	 m_pd3ddev = pd3ddev;
	 m_d3dpp = d3dpp; 


	m_motparamdlg = 0;
	m_axisdlg = 0;
	m_vikdlg = 0;
	m_texdlg = 0;
	m_dsdlg = 0;
	m_mdlg = 0;
	m_alpdlg = 0;
	m_camdlg = 0;
	m_gpdlg = 0;
	m_moedlg = 0;
	m_snddlg = 0;

	m_disptabno = TAB_BASE;

	m_createtabflag = 0;

	m_cmdshow = SW_HIDE;

	tempiklevelope = iklevelope;
	
	m_hfont = 0;

	InitFrameSave();
}

CMotDlg::~CMotDlg()
{
	DestroyTabWindows();
}

/***
BOOL CMotDlg::PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
{

DbgOut( "motdlg : PreTranslateAccelerator\r\n" );

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
***/

int CMotDlg::CreateTabFont()
{
	//int ret;

	HDC hDC;
	hDC = m_tab_wnd.GetDC();
//    SetMapMode( hDC, MM_TEXT );

    INT nHeight = -MulDiv( 8, (INT)(GetDeviceCaps(hDC, LOGPIXELSY)), 72 );
    DWORD dwBold = FW_NORMAL;
    DWORD dwItalic = FALSE;
    m_hfont = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, "ＭＳ ゴシック" );
	if( m_hfont == NULL ){
		DbgOut( "motdlg : CreateTabFont : CreateFont error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_tab_wnd.SetFont( m_hfont, TRUE );

	m_tab_wnd.ReleaseDC( hDC );

	return 0;
}

int CMotDlg::DestroyTabFont()
{
	if( m_hfont ){
		DeleteObject( m_hfont );
		m_hfont = 0;
	}

	return 0;
}

LRESULT CMotDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	VIKStopVIK();

	ShowDlg( SW_HIDE );
	BOOL handled;
	m_motparamdlg->OnCancel( wNotifyCode, wID, hWndCtl, handled );

	return 0;
}


int CMotDlg::ShowDlg( int cmdshow )
{
	if( cmdshow == SW_HIDE ){
		if( m_motparamdlg ){
			m_motparamdlg->ShowDlg( SW_HIDE );
			m_axisdlg->ShowDlg( SW_HIDE );
			m_vikdlg->ShowDlg( SW_HIDE );
			m_texdlg->ShowDlg( SW_HIDE );
			m_dsdlg->ShowDlg( SW_HIDE );
			m_mdlg->ShowDlg( SW_HIDE );
			m_alpdlg->ShowDlg( SW_HIDE );
			m_camdlg->ShowDlg( SW_HIDE );
			m_gpdlg->ShowDlg( SW_HIDE );
			m_moedlg->ShowDlg( SW_HIDE );
			m_snddlg->ShowDlg( SW_HIDE );
		}
	
		if( (m_cmdshow != SW_HIDE) && ::IsWindowEnabled( (HWND)m_vikdlg ) ){
			m_vikdlg->CallOnStopIK();
		}

		ShowWindow( SW_HIDE );
		m_cmdshow = SW_HIDE;
	}else{
		ShowWindow( SW_SHOW );

		if( m_motparamdlg ){
			if( m_disptabno == TAB_BASE ){
				m_motparamdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_AXIS ){
				m_axisdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_VIK ){
				m_vikdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_TEX ){
				m_texdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_DS ){
				m_dsdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_M ){
				m_mdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_ALP ){
				m_alpdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_CAM ){
				m_camdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_GP ){
				m_gpdlg->ShowDlg( SW_SHOW );
			}else if( m_disptabno == TAB_MOE ){
				int idlingid;
				if( m_app->m_mcdlg->m_mch ){
					idlingid = m_app->m_mcdlg->m_mch->GetIdlingMotID();
				}else{
					idlingid = -1;
				}
				if( idlingid >= 0 ){
					m_motparamdlg->m_mch = m_app->m_mcdlg->m_mch;
					m_moedlg->ShowDlg( SW_SHOW );
				}else{
					//::MessageBox( m_hWnd, "moaのアイドリングモーションを設定してから再試行してください。", "エラー", MB_OK );
					m_moedlg->ShowDlg( SW_HIDE );
					m_motparamdlg->ShowDlg( SW_SHOW );
					m_disptabno = TAB_BASE;
				}
			}else if( m_disptabno == TAB_SND ){
				if( !m_app->m_shandler || (m_app->m_soundbank->m_setnum <= 0) ){
					m_snddlg->ShowDlg( SW_HIDE );
					m_motparamdlg->ShowDlg( SW_SHOW );
					m_disptabno = TAB_BASE;
				}else{
					m_snddlg->ShowDlg( SW_SHOW );
				}
			}else{
				_ASSERT( 0 );
			}
		}
		m_cmdshow = SW_SHOW;
	}

	return 0;
}

int CMotDlg::OnJumpFrame( int srcframe, int quaflag, int camflag, int gpaflag, int moeflag, int sndflag )
{
	if( g_endapp == 1 ){
		return 0;
	}

	if( m_motparamdlg ){
		if( quaflag == 1 ){
			if( m_disptabno == TAB_BASE ){
				m_motparamdlg->OnJumpFrame( srcframe );
			}else if( m_disptabno == TAB_TEX ){
				m_texdlg->OnJumpFrame( srcframe );
			}else if( m_disptabno == TAB_DS ){
				m_dsdlg->OnJumpFrame( srcframe );
			}else if( m_disptabno == TAB_M ){
				m_mdlg->OnJumpFrame( srcframe );
			}else if( m_disptabno == TAB_ALP ){
				m_alpdlg->OnJumpFrame( srcframe );
			}else{
				m_motparamdlg->OnJumpFrame( srcframe );
			}
		}

		if( camflag == 1 ){
			m_camdlg->OnJumpFrame( srcframe );
		}
		if( gpaflag == 1 ){
			m_gpdlg->OnJumpFrame( srcframe );
		}
		if( sndflag == 1 ){
			if( m_app->m_shandler && (m_app->m_soundbank->m_setnum > 0) ){
				m_snddlg->OnJumpFrame( srcframe );
			}
		}
		if( moeflag == 1 ){
			int idlingid;
			if( m_app->m_mcdlg->m_mch ){
				idlingid = m_app->m_mcdlg->m_mch->GetIdlingMotID();
			}else{
				idlingid = -1;
			}
			if( idlingid >= 0 ){
				m_motparamdlg->m_mch = m_app->m_mcdlg->m_mch;
				m_moedlg->OnJumpFrame( srcframe );

				if( (m_disptabno != TAB_BASE) && (m_disptabno != TAB_TEX) && (m_disptabno != TAB_DS) && (m_disptabno != TAB_M) && (m_disptabno != TAB_ALP) ){
					m_moedlg->SetCurrentMotion( 0 );
				}
			}
		}



	}

	return 0;
}
int CMotDlg::SetSaveBMDisp( int srcdisp )
{
	if( m_motparamdlg ){
		m_motparamdlg->m_savebmdisp = srcdisp;
	}
	if( m_moedlg ){
		m_moedlg->m_savebmdisp = srcdisp;
	}

	if( m_camdlg ){
		m_camdlg->m_savebmdisp = srcdisp;
	}
	if( m_gpdlg ){
		m_gpdlg->m_savebmdisp = srcdisp;
	}
	if( m_snddlg ){
		m_snddlg->m_savebmdisp = srcdisp;
	}


	return 0;
}
int CMotDlg::SetSaveJointDisp( int srcdisp )
{
	if( m_motparamdlg ){
		m_motparamdlg->m_savejdisp = srcdisp;
	}
	if( m_moedlg ){
		m_moedlg->m_savejdisp = srcdisp;
	}

	if( m_camdlg ){
		m_camdlg->m_savejdisp = srcdisp;
	}
	if( m_gpdlg ){
		m_gpdlg->m_savejdisp = srcdisp;
	}
	if( m_snddlg ){
		m_snddlg->m_savejdisp = srcdisp;
	}

	return 0;
}



LRESULT CMotDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_createtabflag == 0 ){
		CreateTabWindows( m_pd3ddev, m_d3dpp );
		m_createtabflag = 1;
	}

	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}


void CMotDlg::SetWnd()
{
//	m_tab_wnd = GetDlgItem( IDC_TAB1 );

    RECT rc;
    TC_ITEM tcItem;
    int x, y;

    m_tab_wnd.GetClientRect(&rc);

	x = 0;
	y = 0;

    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = "基本操作";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_BASE, (LPARAM)&tcItem);
    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = "ＩＫ軸";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_AXIS, (LPARAM)&tcItem);
    tcItem.pszText = "数値ＩＫ";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_VIK, (LPARAM)&tcItem);
    tcItem.pszText = "テクスチャ";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_TEX, (LPARAM)&tcItem);
    tcItem.pszText = "スイッチ";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_DS, (LPARAM)&tcItem);
    tcItem.pszText = "モーフ";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_M, (LPARAM)&tcItem);
    tcItem.pszText = "Alpha";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_ALP, (LPARAM)&tcItem);
    tcItem.pszText = "カメラ";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_CAM, (LPARAM)&tcItem);
    tcItem.pszText = "GPA";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_GP, (LPARAM)&tcItem);
    tcItem.pszText = "MOE";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_MOE, (LPARAM)&tcItem);
    tcItem.pszText = "SND";
	::SendMessage(m_tab_wnd, TCM_INSERTITEM, (WPARAM)TAB_SND, (LPARAM)&tcItem);

//	rc.left = 50;
//	rc.top = 50;
	rc.left = 0;
	rc.top = 25;


	m_motparamdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_motparamdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_axisdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_axisdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_vikdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_vikdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_texdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_texdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_dsdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_dsdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_mdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_mdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_alpdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_alpdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_camdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_camdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_gpdlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_gpdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_moedlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_moedlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_snddlg->Create( m_tab_wnd, (LPARAM)m_hWnd );
    m_snddlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	m_motparamdlg->ShowDlg(SW_SHOW);
	m_axisdlg->ShowDlg(SW_HIDE);
	m_vikdlg->ShowDlg(SW_HIDE);
	m_texdlg->ShowDlg(SW_HIDE);
	m_dsdlg->ShowDlg(SW_HIDE);
	m_mdlg->ShowDlg(SW_HIDE);
	m_alpdlg->ShowDlg(SW_HIDE);
	m_camdlg->ShowDlg(SW_HIDE);
	m_gpdlg->ShowDlg(SW_HIDE);
	m_moedlg->ShowDlg(SW_HIDE);
	m_snddlg->ShowDlg(SW_HIDE);
}

int CMotDlg::ParamsToDlg()
{	

	return 0;
}

int CMotDlg::CreateTabWindows( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp )
{
	if( m_motparamdlg ){
		return 0;
	}

	RECT rect0;
	GetClientRect( &rect0 );
	m_tab_wnd.Create( "SysTabControl32", m_hWnd, rect0, NULL, WS_CHILD | WS_VISIBLE );

	int ret;
	ret = CreateTabFont();
	if( ret ){
		DbgOut( "motdlg : CreateTabWindows : CreateTabFont error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	m_motparamdlg = new CMotParamDlg( tempiklevelope );
	if( !m_motparamdlg ){
		DbgOut( "motdlg : motparamdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_axisdlg = new CAxisDlg( pd3ddev, d3dpp );
	if( !m_axisdlg ){
		DbgOut( "motdlg : axisdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_vikdlg = new CVIKDlg( m_motparamdlg );
	if( !m_vikdlg ){
		DbgOut( "motdlg : vikdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_texdlg = new CTexKeyDlg( m_motparamdlg );
	if( !m_texdlg ){
		DbgOut( "motdlg : texdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_dsdlg = new CDSKeyDlg( m_motparamdlg );
	if( !m_dsdlg ){
		DbgOut( "motdlg : dsdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_mdlg = new CMorphKeyDlg( m_motparamdlg );
	if( !m_mdlg ){
		DbgOut( "motdlg : mdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_alpdlg = new CAlpKeyDlg( m_motparamdlg );
	if( !m_alpdlg ){
		DbgOut( "motdlg : alpdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_camdlg = new CCamKeyDlg( m_motparamdlg, m_app );
	if( !m_camdlg ){
		DbgOut( "motdlg : camdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_gpdlg = new CGPKeyDlg( m_motparamdlg, m_app );
	if( !m_gpdlg ){
		DbgOut( "motdlg : gpdlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_moedlg = new CMOEKeyDlg( m_motparamdlg, m_app );
	if( !m_moedlg ){
		DbgOut( "motdlg : moedlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_snddlg = new CSndKeyDlg( m_motparamdlg, m_app );
	if( !m_snddlg ){
		DbgOut( "motdlg : snddlg alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_app->m_syncdlg->SetSndDlg( m_snddlg );


	return 0;
}

int CMotDlg::DestroyTabWindows()
{
//	if( m_motparamdlg ){
//		m_motparamdlg->DestroyTimer();
		//myviewerですでに呼んでいる
//	}
DbgOut( "checkend : motdlg : DestroyTabWindow 0 \r\n" );
	if( m_axisdlg ){
		m_axisdlg->DestroyTimer();
	}
	
	if( m_vikdlg ){
		m_vikdlg->DestroyTimer();
	}

	if( m_texdlg ){
		m_texdlg->DestroyTimer();
	}

	if( m_dsdlg ){
		m_dsdlg->DestroyTimer();
	}

	if( m_mdlg ){
		m_mdlg->DestroyTimer();
	}

	if( m_alpdlg ){
		m_alpdlg->DestroyTimer();
	}

	if( m_camdlg ){
		m_camdlg->DestroyTimer();
	}

	if( m_gpdlg ){
		m_gpdlg->DestroyTimer();
	}

	if( m_moedlg ){
		m_moedlg->DestroyTimer();
	}

	if( m_snddlg ){
		m_snddlg->DestroyTimer();
	}

//////////////
//	if( m_motparamdlg ){
//		delete m_motparamdlg;
//		m_motparamdlg = 0;
//	}

DbgOut( "checkend : motdlg : DestroyTabWindow 1 \r\n" );

	if( m_axisdlg ){
		delete m_axisdlg;
		m_axisdlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 2 \r\n" );

	if( m_vikdlg ){
		delete m_vikdlg;
		m_vikdlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 3 \r\n" );

	if( m_texdlg ){
		delete m_texdlg;
		m_texdlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 4 \r\n" );

	if( m_dsdlg ){
		delete m_dsdlg;
		m_dsdlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 5 \r\n" );

	if( m_mdlg ){
		delete m_mdlg;
		m_mdlg = 0;
	}

	if( m_alpdlg ){
		delete m_alpdlg;
		m_alpdlg = 0;
	}

	
DbgOut( "checkend : motdlg : DestroyTabWindow 6 \r\n" );

	if( m_camdlg ){
		delete m_camdlg;
		m_camdlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 7 \r\n" );

	if( m_gpdlg ){
		delete m_gpdlg;
		m_gpdlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 8 \r\n" );


	if( m_moedlg ){
		delete m_moedlg;
		m_moedlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 9 \r\n" );


	if( m_snddlg ){
		delete m_snddlg;
		m_snddlg = 0;
	}

DbgOut( "checkend : motdlg : DestroyTabWindow 10 \r\n" );

	if( m_motparamdlg ){
		delete m_motparamdlg;
		m_motparamdlg = 0;
	}

	int ret;
	ret = DestroyTabFont();
	if( ret ){
		DbgOut( "motdlg : DestroyTabWindwos : DestroyTabFont error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
DbgOut( "checkend : motdlg : DestroyTabWindow end \r\n" );

	return 0;
}

int CMotDlg::SelectMotParamTab()
{
	if( !IsWindow() ){
		return 0;
	}
	if( !m_motparamdlg ){
		return 0;
	}

	int stopflag;
	if( m_motparamdlg->m_preview_flag ){
		stopflag = 1;
	}else{
		stopflag = 0;
	}

	m_motparamdlg->ShowDlg(SW_HIDE, stopflag);

	m_axisdlg->ShowDlg(SW_HIDE);
	if( m_vikdlg->m_cmdshow == SW_SHOW ){
		m_vikdlg->CallOnStopIK();
	}
	m_vikdlg->ShowDlg( SW_HIDE );

	m_texdlg->ShowDlg( SW_HIDE );

	m_dsdlg->ShowDlg( SW_HIDE );

	m_mdlg->ShowDlg( SW_HIDE );
	m_mdlg->m_base = 0;//!!!!!!!!!!!!
	m_mdlg->m_target = 0;//!!!!!!!!!!!!
	m_mdlg->m_boneelem = 0;//!!!!!!!!!!!!

	m_alpdlg->ShowDlg( SW_HIDE );

	m_camdlg->ShowDlg( SW_HIDE );
	m_gpdlg->ShowDlg( SW_HIDE );

	m_moedlg->ShowDlg( SW_HIDE );

	m_snddlg->ShowDlg( SW_HIDE );


	m_motparamdlg->ShowDlg(SW_SHOW);
	m_disptabno = TAB_BASE;


	::SendMessage( m_tab_wnd, TCM_SETCURSEL, (WPARAM)0, 0 );


	return 0;
}


LRESULT CMotDlg::OnSelchange_tab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int cursel = (int)::SendMessage( m_tab_wnd, TCM_GETCURSEL, 0, 0 );

	if( m_app->m_tbb_mo == 1 ){
		if( cursel != 0 ){
			::SendMessage( m_tab_wnd, TCM_SETCURSEL, 0, 0 );
		}
		m_disptabno = TAB_BASE;
		return 0;//!!!!!!!!!!!!!!!!!!!!
	}


	int stopflag;
	if( m_motparamdlg->m_preview_flag ){
		stopflag = 1;
	}else{
		stopflag = 0;
	}
	m_motparamdlg->ShowDlg(SW_HIDE, stopflag);

	m_axisdlg->ShowDlg(SW_HIDE);
	if( m_vikdlg->m_cmdshow == SW_SHOW ){
		m_vikdlg->CallOnStopIK();
	}
	m_vikdlg->ShowDlg( SW_HIDE );

	m_texdlg->ShowDlg( SW_HIDE );

	m_dsdlg->ShowDlg( SW_HIDE );

	m_mdlg->ShowDlg( SW_HIDE );

	m_alpdlg->ShowDlg( SW_HIDE );

	m_camdlg->ShowDlg( SW_HIDE );

	m_gpdlg->ShowDlg( SW_HIDE );

	m_moedlg->ShowDlg( SW_HIDE );

	m_snddlg->ShowDlg( SW_HIDE );


	int moeupdate = 0;
	int moeframeno = m_moedlg->m_current;

	int idlingid;
	switch(cursel) {
		case TAB_BASE:
			m_motparamdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_BASE;
			break;
		case TAB_AXIS:
			m_axisdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_AXIS;
			break;
		case TAB_VIK:
			m_vikdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_VIK;
			break;
		case TAB_TEX:
			m_texdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_TEX;
			break;
		case TAB_DS:
			m_dsdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_DS;
			break;
		case TAB_M:
			m_mdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_M;
			break;
		case TAB_ALP:
			m_alpdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_ALP;
			break;
		case TAB_CAM:
			m_camdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_CAM;
			if( (m_app->m_syncdlg->m_moeflag != 0) && (m_app->m_syncdlg->m_camflag != 0) ){
				moeupdate = 1;
				moeframeno = m_camdlg->m_current;
			}
			break;
		case TAB_GP:
			m_gpdlg->ShowDlg(SW_SHOW);
			m_disptabno = TAB_GP;
			if( (m_app->m_syncdlg->m_moeflag != 0) && (m_app->m_syncdlg->m_gpaflag != 0) ){
				moeupdate = 1;
				moeframeno = m_gpdlg->m_current;
			}
			break;
		case TAB_MOE:
			if( m_app->m_mcdlg->m_mch ){
				idlingid = m_app->m_mcdlg->m_mch->GetIdlingMotID();
			}else{
				idlingid = -1;
			}
			if( idlingid >= 0 ){
				m_motparamdlg->m_mch = m_app->m_mcdlg->m_mch;
				m_moedlg->ShowDlg( SW_SHOW );
				m_disptabno = TAB_MOE;

				moeupdate = 1;
				moeframeno = m_moedlg->m_current;

			}else{
				::MessageBox( m_hWnd, "moaのアイドリングモーションを設定してから再試行してください。", "エラー", MB_OK );
				m_moedlg->ShowDlg( SW_HIDE );
				m_motparamdlg->ShowDlg( SW_SHOW );
				m_disptabno = TAB_BASE;
			}
			break;
		case TAB_SND:
			if( !m_app->m_shandler || (m_app->m_soundbank->m_setnum <= 0) ){
				::MessageBox( m_hWnd, "形状を読み込み音セットを設定してから再試行してください。", "エラー", MB_OK );
				m_snddlg->ShowDlg( SW_HIDE );
				m_motparamdlg->ShowDlg( SW_SHOW );
				m_disptabno = TAB_BASE;
			}else{
				m_snddlg->ShowDlg(SW_SHOW);
				m_disptabno = TAB_SND;
				if( (m_app->m_syncdlg->m_moeflag != 0) && (m_app->m_syncdlg->m_sndflag != 0) ){
					moeupdate = 1;
					moeframeno = m_snddlg->m_current;
				}
			}
			break;
		default:
			_ASSERT( 0 );
			break;
	}

	if( moeupdate == 1 ){
		m_moedlg->OnJumpFrame( moeframeno );
		m_moedlg->SetCurrentMotion( 0 );
	}

//DbgOut( "checkmoe!!! : motdlg : OnSelchange_tab : moeupdate %d, moeframeno %d\r\n",
//	moeupdate, moeframeno );


	m_motparamdlg->m_grotdlg->Redraw();

	//char selectstr[256];
	//sprintf( selectstr, "selchange : %d", cursel );
	//MessageBox( selectstr, "selchange", MB_OK );

	bHandled = 0;

	return 0;
}

/***
LRESULT CMotDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
DbgOut( "motdlg : KeyDown \r\n" );
	unsigned char KeyBuffer[256];
	GetKeyboardState( KeyBuffer );

	if( (KeyBuffer[VK_LEFT] & 0x80) || (KeyBuffer[VK_RIGHT] & 0x80) ){
		DbgOut( "motdlg : ignore arrow key!!!\r\n" );
		bHandled = 1;
	}
	

	return 0;
}
***/

LRESULT CMotDlg::OnSizeDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rect0;
	GetClientRect( &rect0 );

	m_tab_wnd.MoveWindow( rect0.left, rect0.top, 
		rect0.right - rect0.left,
		rect0.bottom - rect0.top,
		1 );

	RECT rc;
	m_tab_wnd.GetWindowRect( &rc );

//	rc.left = 50;
//	rc.top = 50;
	rc.left = 0;
	rc.top = 25;

	m_motparamdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_axisdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_vikdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_texdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_dsdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_mdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_alpdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_camdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_gpdlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_moedlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );
    m_snddlg->SetWindowPos( HWND_TOP, &rc, SWP_NOSIZE | SWP_SHOWWINDOW );

	return 0;
}


/////// for motparamdlg
int CMotDlg::SetParams( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, HWND papp, CMyD3DApplication* d3dapp )
{
	m_vikdlg->m_d3dapp = d3dapp;//!!!!!!!!!!!

	int ret;
	ret = m_motparamdlg->SetParams( lpth, lpsh, lpmh, papp, d3dapp );
	_ASSERT( !ret );

	return 0;
}
int CMotDlg::SetAppWnd( HWND appwnd )
{
	return m_motparamdlg->SetAppWnd( appwnd );
}
int CMotDlg::GetMotParams( int* mcookieptr, int* framenoptr )
{
	return m_motparamdlg->GetMotParams( mcookieptr, framenoptr );
}
int CMotDlg::GetFrameLeng( int motid, int* lengptr )
{
	return m_motparamdlg->GetFrameLeng( motid, lengptr );
}

int CMotDlg::DestroyMotionObj()
{

	return m_motparamdlg->DestroyMotionObj();
}
int CMotDlg::Redraw()
{

	return m_motparamdlg->Redraw();
}
int CMotDlg::RemakeTreeView( int selno )
{

	return m_motparamdlg->RemakeTreeView( selno );
}
//int CMotDlg::Rename( int srcseri, char* srcname )
//{
//
//	return m_motparamdlg->Rename( srcseri, srcname );
//}
int CMotDlg::OnUserSelchange( int selno )
{

	m_motparamdlg->OnUserSelchange( selno );
	m_vikdlg->OnUserSelchange( selno );
	m_camdlg->OnUserSelchange( selno );

	return 0;
}
int CMotDlg::SetDisplayFlag( int dflag )
{

	return m_motparamdlg->SetDisplayFlag( dflag );
}
int CMotDlg::InvalidateHandler()
{

	return m_motparamdlg->InvalidateHandler();
}
int CMotDlg::LoadMotion( CLoadNameDlg* namedlg, char* srcfilename, D3DXVECTOR3 srcquamult, int callmoa )
{

	return m_motparamdlg->LoadMotion( namedlg, srcfilename, srcquamult, callmoa );
}
int CMotDlg::LoadEQUMotion( CEQUFile* equfile, int animcnt, float quamult )
{
	return m_motparamdlg->LoadEQUMotion( equfile, animcnt, quamult );
}
int CMotDlg::LoadMKM( char* filename, CMKMFile* mkmfile, CMKMFile* inimkm, int motno, float tramult )
{
	return m_motparamdlg->LoadMKM( filename, mkmfile, inimkm, motno, tramult );
}

int CMotDlg::ImportMotion( CLoadNameDlg* namedlg )
{

	return m_motparamdlg->ImportMotion( namedlg );
}
int CMotDlg::SetMotionStep( int srcmotstep )
{

	return m_motparamdlg->SetMotionStep( srcmotstep );
}


int CMotDlg::LoadBVHFile( char* srcfilename )
{

	return m_motparamdlg->LoadBVHFile( srcfilename );
}

int CMotDlg::SaveMotion( CLoadNameDlg* namedlg, char* srcfilename )
{

	return m_motparamdlg->SaveMotion( namedlg, srcfilename );
}
int CMotDlg::WriteBmp( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont )
{

	return m_motparamdlg->WriteBmp( pd3dDevice, pFont );
}
int CMotDlg::WriteAvi( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont )
{

	return m_motparamdlg->WriteAvi( pd3dDevice, pFont );
}
int CMotDlg::InverseMotionXZ()
{

	return m_motparamdlg->InverseMotionXZ();
}
int CMotDlg::SetNewPose()
{

	return m_motparamdlg->SetNewPose();
}
int CMotDlg::UpdateAllMatrix( int spallkind )
{
	return m_motparamdlg->UpdateAllMatrix( spallkind );
}
int CMotDlg::UpdateMatrix( int srcseri, int spallkind )
{

	return m_motparamdlg->UpdateMatrix( srcseri, spallkind );
}
//int CMotDlg::UpdateMatrixSelected( int srcseri )
//{
//
//	return m_motparamdlg->UpdateMatrixSelected( srcseri );
//}
int CMotDlg::SetCurrentMotion()
{

	return m_motparamdlg->SetCurrentMotion();
}
int CMotDlg::CopyUndoBuffer( int cpframe )
{

	return m_motparamdlg->CopyUndoBuffer( cpframe );
}
int CMotDlg::PasteUndoBuffer( int undocnt, int undodlg )
{

	return m_motparamdlg->PasteUndoBuffer( undocnt, undodlg );
}
int CMotDlg::ExpandAndShiftMotion( int shiftnum )
{

	return m_motparamdlg->ExpandAndShiftMotion( shiftnum );
}
int CMotDlg::GetBonePos( int srcseri, D3DXMATRIX wmat, int mode, D3DXVECTOR3* dstpos )
{
	return m_motparamdlg->GetBonePos( srcseri, wmat, mode, dstpos );
}

int CMotDlg::GetSelectedJointPos( int* setflag, D3DXVECTOR3* jposptr )
{

	return m_motparamdlg->GetSelectedJointPos( setflag, jposptr );
}

int CMotDlg::GetMotCookie()
{
	return m_motparamdlg->m_mot_cookie;
}
int CMotDlg::GetCurrentFrameNo()
{
	return m_motparamdlg->m_current;
}

int CMotDlg::GetPreviewFlag()
{
	return m_motparamdlg->m_preview_flag;

}
int CMotDlg::GetSellockno()
{
	return m_motparamdlg->m_sellockno;
}


int CMotDlg::MoveBone( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent )
{
	return m_motparamdlg->MoveBone( matWorld, mvno, srcx, srcy, lastparent );
}
int CMotDlg::MoveBone( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 tpos, int* lastparent, int defaxis, D3DXVECTOR3 srcaxis, D3DXVECTOR3* axisdone, int iklevel )
{
	return m_motparamdlg->MoveBone( matWorld, mvno, tpos, lastparent, defaxis, srcaxis, axisdone, iklevel );
}
int CMotDlg::MoveBoneZa4( D3DXMATRIX matWorld, int mvno, float srcx, float srcy, int* lastparent )
{
	return m_motparamdlg->MoveBoneZa4( matWorld, mvno, srcx, srcy, lastparent );
}
int CMotDlg::MoveBoneZa4( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 tpos, int* lastparent, int iklevel )
{
	return m_motparamdlg->MoveBoneZa4( matWorld, mvno, tpos, lastparent, iklevel );
}
int CMotDlg::ShiftBone( int shiftno, float srcx, float srcy, int t2flag, int za4flag )
{
	return m_motparamdlg->ShiftBone( shiftno, srcx, srcy, t2flag, za4flag );
}
int CMotDlg::ShiftBone( int frameno, int shiftno, D3DXVECTOR3 tpos, int* setnoptr, int za4flag )
{
	return m_motparamdlg->ShiftBone( frameno, shiftno, tpos, setnoptr, za4flag, 0 );
}
int CMotDlg::ShiftBone( int shiftno, int shiftkind, float delta, int t2flag, int localflag, CQuaternion localq, int za4flag )
{
	return m_motparamdlg->ShiftBone( shiftno, shiftkind, delta, t2flag, localflag, localq, za4flag );
}
int CMotDlg::ShiftBoneTree( int shiftno, D3DXVECTOR3 targetobj, int za4flag )
{
	return m_motparamdlg->ShiftBoneTree( shiftno, targetobj, za4flag );
}


int CMotDlg::RotateBone( D3DXMATRIX matWorld, int mvno, int rotkind, float delta, int* lastparent, int localflag, CQuaternion localq, int ikfkflag, int za4flag )
{
	return m_motparamdlg->RotateBone( matWorld, mvno, rotkind, delta, lastparent, localflag, localq, ikfkflag, za4flag );
}

int CMotDlg::ScaleBone( int scaleno, int scalekind, float delta )
{
	return m_motparamdlg->ScaleBone( scaleno, scalekind, delta );
}
int CMotDlg::CopyLowerMotion( CShdElem* srcselem )
{
	return m_motparamdlg->CopyLowerMotion( srcselem );
}
int CMotDlg::OnFrameCopy()
{
	BOOL dummy;
	return (int)m_motparamdlg->OnFrameCopy( 0, 0, 0, dummy );
}
int CMotDlg::OnFramePaste()
{
	BOOL dummy;
	return (int)m_motparamdlg->OnFramePaste( 0, 0, 0, dummy );
}

int CMotDlg::OnSpin_Sp_mp( int idctrl, NMUPDOWN* pnmud, BOOL& dummy )
{
	m_motparamdlg->OnSpin_Sp_mp( idctrl, (NMHDR*)pnmud, dummy );
	return 0;
}

int CMotDlg::OnStop()
{
	BOOL dummy;
	m_motparamdlg->OnStop( 0, 0, 0, dummy );
	return 0;
}
int CMotDlg::OnPreview()
{
	BOOL dummy;
	m_motparamdlg->OnPreview( 0, 0, 0, dummy );
	return 0;
}

int CMotDlg::GetRangeMax( int motid, int* rangemax )
{
	return m_motparamdlg->GetRangeMax( motid, rangemax );
}
int CMotDlg::GetKeyFrameNo( int motid, int* keyframe, int* framenum )
{
	return m_motparamdlg->GetKeyFrameNo( motid, keyframe, framenum );
}

void CMotDlg::Rotate2IKTargetReq( D3DXMATRIX matWorld, int mvno, D3DXVECTOR3 rotaxis, int broflag )
{
	m_motparamdlg->Rotate2IKTargetReq( matWorld, mvno, rotaxis, broflag );
	return;
}
void CMotDlg::Rotate2IKTargetZa4Req( D3DXMATRIX matWorld, int mvno, int broflag )
{
	m_motparamdlg->Rotate2IKTargetZa4Req( matWorld, mvno, broflag );
	return;
}
int CMotDlg::SetParBoneArray( int mvno )
{
	return m_motparamdlg->SetParBoneArray( mvno );
}

int CMotDlg::OnTwist( D3DXMATRIX matWorld, int twistno, float fdelta, int za4flag )
{
	return m_motparamdlg->OnTwist( matWorld, twistno, fdelta, za4flag );
}

int CMotDlg::OnBeforeMP()
{
	BOOL dummy;
	return m_motparamdlg->OnBeforeMP( 0, 0, 0, dummy );
}
int CMotDlg::OnNextMP()
{
	BOOL dummy;
	return m_motparamdlg->OnNextMP( 0, 0, 0, dummy );
}

int CMotDlg::GetIKLevelOpe()
{
	return m_motparamdlg->m_iklevelope;
}

int CMotDlg::GetMotionNum()
{
	return m_motparamdlg->m_motionnum;
}
MOTID* CMotDlg::GetMotIDArray()
{
	return m_motparamdlg->m_motid;
}
int CMotDlg::SelectCurrentMotion( int srcindex )
{
	int ret;
	ret = m_motparamdlg->SelectCurrentMotion( srcindex );
	_ASSERT( !ret );

	ret = m_dsdlg->OnUserSelchange();
	_ASSERT( !ret );

	ret = m_texdlg->OnUserSelchange();
	_ASSERT( !ret );

	ret = m_mdlg->OnUserSelchange();
	_ASSERT( !ret );

	ret = m_alpdlg->OnUserSelchange();
	_ASSERT( !ret );

	return 0;

}
int CMotDlg::NewMotion()
{
	int ret;
	ret = m_motparamdlg->InitDlg();
	_ASSERT( !ret );

	return 0;
}
int CMotDlg::DestroyCurrentMotion()
{
	int ret;
	ret = m_motparamdlg->DestroyCurrentMotion();
	_ASSERT( !ret );

	ret = m_dsdlg->OnUserSelchange();
	_ASSERT( !ret );

	ret = m_texdlg->OnUserSelchange();
	_ASSERT( !ret );

	ret = m_mdlg->OnUserSelchange();
	_ASSERT( !ret );

	ret = m_alpdlg->OnUserSelchange();
	_ASSERT( !ret );

	return 0;
}

int CMotDlg::SetCurrentMotionPoint( int boneno, CMotionPoint2* srcmp, int hasmpflag )
{
	return m_motparamdlg->SetCurrentMotionPoint( boneno, srcmp, hasmpflag );
}
int CMotDlg::GetRotRad()
{
	return m_motparamdlg->GetRotRad();
}

int CMotDlg::MultMotion2_0()
{
	return m_motparamdlg->MultMotion2_0();
}
int CMotDlg::MultMotion0_5()
{
	return m_motparamdlg->MultMotion0_5();
}
int CMotDlg::DeleteInitialKey()
{
	return m_motparamdlg->DeleteInitialKey();
}
int CMotDlg::DeleteSameKey()
{
	return m_motparamdlg->DeleteSameKey();
}
int CMotDlg::SetMotionFrameNo( int motid, int frameno )
{
	return m_motparamdlg->SetMotionFrameNo( motid, frameno );
}
int CMotDlg::MotCut()
{
	return m_motparamdlg->MotCut();
}
int CMotDlg::MotMabiki()
{
	return m_motparamdlg->MotMabiki();
}
int CMotDlg::ChangeCurMotTotalFrame( int newleng )
{
	return m_motparamdlg->ChangeCurMotTotalFrame( newleng );
}
int CMotDlg::BakeMotion()
{
	return m_motparamdlg->BakeMotion();
}

///////////////// for morphdlg
int CMotDlg::ResetMorphDlg()
{
	return m_mdlg->ResetDlg();
}

///////////////// for axisdlg
int CMotDlg::AxisSetInvalidateFlag( int flag )
{
	return m_axisdlg->SetInvalidateFlag( flag );
}
int CMotDlg::AxisSetRenderParams( float degy, float degxz, GUARDBAND gband, 
		CShdHandler* groundsh, CMotHandler* groundmh, CShdHandler* arrowsh, CMotHandler* arrowmh,
		CD3DFont* pfont )
{
	return m_axisdlg->SetRenderParams( degy, degxz, gband, groundsh, groundmh, arrowsh, arrowmh, pfont );
}
int CMotDlg::AxisCreateSwapChain( LPDIRECT3DDEVICE9 pd3ddev, D3DPRESENT_PARAMETERS d3dpp )
{
	return m_axisdlg->CreateSwapChain( pd3ddev, d3dpp );
}
int CMotDlg::AxisDestroySwapChain()
{
	return m_axisdlg->DestroySwapChain();
}
float CMotDlg::AxisGetArrowDegxz()
{
	return m_axisdlg->m_arrow_degxz;
}
float CMotDlg::AxisGetArrowDegy()
{
	return m_axisdlg->m_arrow_degy;
}
int	CMotDlg::AxisGetArrowMode()
{
	return m_axisdlg->m_arrowmode;
}

///////////////// for vikdlg
int CMotDlg::VIKStopVIK()
{
	if( m_vikdlg && m_vikdlg->m_cmdshow == SW_SHOW ){
		BOOL handled;
		return (int)m_vikdlg->OnStopIK( 0, 0, 0, handled );
	}
	return 0;
}

//////////////// for camdlg

int CMotDlg::GetCameraAnimNum()
{
	return m_camdlg->GetCameraAnimNum();
}
int CMotDlg::LoadCameraFile( char* filename )
{
	return m_camdlg->LoadCameraFile( filename );
}

MOTID* CMotDlg::GetCameraMotIDArray()
{
	return m_camdlg->GetCameraMotIDArray();
}
char* CMotDlg::GetCameraAnimName( int animno )
{
	return m_camdlg->GetCameraAnimName( animno );
}
int CMotDlg::GetCurCameraAnimIndex()
{
	return m_camdlg->GetCurCameraAnimIndex();
}
int CMotDlg::GetCurCameraAnimNo()
{
	return m_camdlg->GetCurCameraAnimNo();
}
int CMotDlg::SelectCameraAnim( int animindex )
{
	return m_camdlg->SelectCameraAnim( animindex );
}
int CMotDlg::NewCameraAnim()
{
	return m_camdlg->InitDlg( 0 );
}
int CMotDlg::DestroyCameraAnim( int delanimno )
{
	return m_camdlg->DestroyCameraAnim( delanimno );
}
int CMotDlg::ResetCamTargetBone()
{
	if( m_camdlg ){
		return m_camdlg->ResetCamTargetBone();
	}else{
		return 0;
	}
}

//////////////// for gpdlg

int CMotDlg::GetGPAnimNum()
{
	return m_gpdlg->GetGPAnimNum();
}
int CMotDlg::LoadGPFile( char* filename )
{
	return m_gpdlg->LoadGPFile( filename );
}

MOTID* CMotDlg::GetGPMotIDArray()
{
	return m_gpdlg->GetGPMotIDArray();
}
char* CMotDlg::GetGPAnimName( int animno )
{
	return m_gpdlg->GetGPAnimName( animno );
}
int CMotDlg::GetCurGPAnimIndex()
{
	return m_gpdlg->GetCurGPAnimIndex();
}
int CMotDlg::GetCurGPAnimNo()
{
	return m_gpdlg->GetCurGPAnimNo();
}
int CMotDlg::SelectGPAnim( int animindex )
{
	return m_gpdlg->SelectGPAnim( animindex );
}
int CMotDlg::NewGPAnim()
{
	return m_gpdlg->InitDlg( 0 );
}
int CMotDlg::DestroyGPAnim( int delanimno )
{
	return m_gpdlg->DestroyGPAnim( delanimno );
}

/////////// for moedlg
int CMotDlg::GetMOEAnimNum()
{
	return m_moedlg->GetMOEAnimNum();
}
int CMotDlg::LoadMOEFile( char* filename )
{
	return m_moedlg->LoadMOEFile( filename );
}

MOTID* CMotDlg::GetMOEMotIDArray()
{
	return m_moedlg->GetMOEMotIDArray();
}
char* CMotDlg::GetMOEAnimName( int animno )
{
	return m_moedlg->GetMOEAnimName( animno );
}
int CMotDlg::GetCurMOEAnimIndex()
{
	return m_moedlg->GetCurMOEAnimIndex();
}
int CMotDlg::GetCurMOEAnimNo()
{
	return m_moedlg->GetCurMOEAnimNo();
}
int CMotDlg::SelectMOEAnim( int animindex )
{
	return m_moedlg->SelectMOEAnim( animindex );
}
int CMotDlg::NewMOEAnim()
{
	return m_moedlg->InitDlg( 0 );
}
int CMotDlg::DestroyMOEAnim( int delanimno )
{
	return m_moedlg->DestroyMOEAnim( delanimno );
}
int CMotDlg::RemakeMOEAH()
{
	return m_moedlg->RemakeMOEAH();
}

///////////// for snddlg
int CMotDlg::WriteSndAnimFile( char* filename )
{
	return m_snddlg->WriteSndAnimFile( filename );
}
int CMotDlg::LoadSndAnimFile( char* filename, int dlgwarnflag )
{
	return m_snddlg->LoadSndAnimFile( filename, dlgwarnflag );
}


//////////// for framesave
int CMotDlg::InitFrameSave()
{
	ZeroMemory( m_framesave, sizeof( FRAMESAVE ) * SAVEKEY_MAX * TAB_MAX );

	return 0;
}
FRAMESAVE* CMotDlg::GetFrameSavePtr( int savekeyno )
{
	if( (savekeyno >= 0) && (savekeyno < SAVEKEY_MAX) ){
		return &(m_framesave[savekeyno][0]);
	}else{
		return 0;
	}
}
int CMotDlg::RestoreFrameSave( int savekeyno )
{
	FRAMESAVE* curfs = GetFrameSavePtr( savekeyno );
	if( !curfs ){
		return 0;
	}

	if( m_motparamdlg->m_preview_flag ){
		return 0;
	}
	int ret;
	if( (curfs + TAB_BASE)->flag && m_motparamdlg ){
		ret = m_motparamdlg->SetStartAndEnd( (curfs + TAB_BASE)->startframe, (curfs + TAB_BASE)->endframe );
		_ASSERT( !ret );
	}
	if( (curfs + TAB_CAM)->flag && m_camdlg ){
		ret = m_camdlg->SetStartAndEnd( (curfs + TAB_CAM)->startframe, (curfs + TAB_CAM)->endframe );
		_ASSERT( !ret );
	}
	if( (curfs + TAB_GP)->flag && m_gpdlg ){
		ret = m_gpdlg->SetStartAndEnd( (curfs + TAB_GP)->startframe, (curfs + TAB_GP)->endframe );
		_ASSERT( !ret );
	}
	if( (curfs + TAB_MOE)->flag && m_moedlg ){
		ret = m_moedlg->SetStartAndEnd( (curfs + TAB_MOE)->startframe, (curfs + TAB_MOE)->endframe );
		_ASSERT( !ret );
	}
	if( (curfs + TAB_SND)->flag && m_snddlg ){
		ret = m_snddlg->SetStartAndEnd( (curfs + TAB_SND)->startframe, (curfs + TAB_SND)->endframe );
		_ASSERT( !ret );
	}

////////// SetStartAndEndでframeno同期が掛かるので、後から分けて呼ぶ必要がある。

	if( (curfs + TAB_BASE)->flag && m_motparamdlg ){
		if( m_disptabno == TAB_BASE ){
			m_motparamdlg->OnJumpFrame( (curfs + TAB_BASE)->frameno );
		}else if( m_disptabno == TAB_TEX ){
			m_texdlg->OnJumpFrame( (curfs + TAB_BASE)->frameno );
		}else if( m_disptabno == TAB_DS ){
			m_dsdlg->OnJumpFrame( (curfs + TAB_BASE)->frameno );
		}else if( m_disptabno == TAB_M ){
			m_mdlg->OnJumpFrame( (curfs + TAB_BASE)->frameno );
		}else if( m_disptabno == TAB_ALP ){
			m_alpdlg->OnJumpFrame( (curfs + TAB_BASE)->frameno );
		}else{
			m_motparamdlg->OnJumpFrame( (curfs + TAB_BASE)->frameno );
		}
	}
	if( (curfs + TAB_CAM)->flag && m_camdlg ){
		m_camdlg->OnJumpFrame( (curfs + TAB_CAM)->frameno );
	}
	if( (curfs + TAB_GP)->flag && m_gpdlg ){
		m_gpdlg->OnJumpFrame( (curfs + TAB_GP)->frameno );
	}
	if( (curfs + TAB_MOE)->flag && m_moedlg ){
		int idlingid;
		if( m_app->m_mcdlg->m_mch ){
			idlingid = m_app->m_mcdlg->m_mch->GetIdlingMotID();
		}else{
			idlingid = -1;
		}
		if( idlingid >= 0 ){
			m_motparamdlg->m_mch = m_app->m_mcdlg->m_mch;
			m_moedlg->OnJumpFrame( (curfs + TAB_MOE)->frameno );

			if( (m_disptabno != TAB_BASE) && (m_disptabno != TAB_TEX) && (m_disptabno != TAB_DS) && (m_disptabno != TAB_M) && (m_disptabno != TAB_ALP) ){
				m_moedlg->SetCurrentMotion( 0 );
			}
		}
	}
	if( (curfs + TAB_SND)->flag && m_snddlg ){
		if( m_app->m_shandler && (m_app->m_soundbank->m_setnum > 0) ){
			m_snddlg->OnJumpFrame( (curfs + TAB_SND)->frameno );
		}
	}

	return 0;
}

int CMotDlg::SaveFrame( int savekeyno )
{
	if( (savekeyno < 0) || (savekeyno >= SAVEKEY_MAX) ){
		return 0;
	}

	FRAMESAVE* curfs = GetFrameSavePtr( savekeyno );
	if( !curfs ){
		return 0;
	}

	if( !m_motparamdlg || !m_camdlg || !m_gpdlg || !m_moedlg || !m_snddlg ){
		return 0;
	}

	if( m_motparamdlg->m_preview_flag ){
		return 0;
	}


	if( m_motparamdlg->m_mot_cookie < 0 ){
		ZeroMemory( curfs, sizeof( FRAMESAVE ) * TAB_MAX );		
	}else{
		(curfs + TAB_BASE)->flag = 1;
		(curfs + TAB_BASE)->frameno = m_motparamdlg->m_current;
		(curfs + TAB_BASE)->startframe = m_motparamdlg->m_startframe;
		(curfs + TAB_BASE)->endframe = m_motparamdlg->m_endframe;

//////////
		CCameraAnim* camanim = 0;
		if( m_camdlg->m_cah ){
			camanim = m_camdlg->m_cah->GetCameraAnim( m_camdlg->m_animno );
		}
		if( camanim ){
			(curfs + TAB_CAM)->flag = 1;
			(curfs + TAB_CAM)->frameno = m_camdlg->m_current;
			(curfs + TAB_CAM)->startframe = m_camdlg->m_startframe;
			(curfs + TAB_CAM)->endframe = m_camdlg->m_endframe;
		}else{
			ZeroMemory( curfs + TAB_CAM, sizeof( FRAMESAVE ) );
		}
//////////
		CGPAnim* gpanim = 0;
		if( m_gpdlg->m_gpah ){
			gpanim = m_gpdlg->m_gpah->GetGPAnim( m_gpdlg->m_animno );
		}
		if( gpanim ){
			(curfs + TAB_GP)->flag = 1;
			(curfs + TAB_GP)->frameno = m_gpdlg->m_current;
			(curfs + TAB_GP)->startframe = m_gpdlg->m_startframe;
			(curfs + TAB_GP)->endframe = m_gpdlg->m_endframe;
		}else{
			ZeroMemory( curfs + TAB_GP, sizeof( FRAMESAVE ) );
		}
////////////
		CMOEAnim* moeanim = 0;
		if( m_moedlg->m_moeah ){
			moeanim = m_moedlg->m_moeah->GetMOEAnim( m_moedlg->m_animno );
		}
		if( moeanim ){
			(curfs + TAB_MOE)->flag = 1;
			(curfs + TAB_MOE)->frameno = m_moedlg->m_current;
			(curfs + TAB_MOE)->startframe = m_moedlg->m_startframe;
			(curfs + TAB_MOE)->endframe = m_moedlg->m_endframe;
		}else{
			ZeroMemory( curfs + TAB_MOE, sizeof( FRAMESAVE ) );
		}
////////////
		CSndAnim* sndanim = 0;
		if( m_snddlg->m_sndah ){
			sndanim = m_snddlg->m_sndah->GetSndAnim( m_snddlg->m_animno );
		}
		if( sndanim ){
			(curfs + TAB_SND)->flag = 1;
			(curfs + TAB_SND)->frameno = m_snddlg->m_current;
			(curfs + TAB_SND)->startframe = m_snddlg->m_startframe;
			(curfs + TAB_SND)->endframe = m_snddlg->m_endframe;
		}else{
			ZeroMemory( curfs + TAB_SND, sizeof( FRAMESAVE ) );
		}
	}

	return 0;
}

