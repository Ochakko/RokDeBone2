// CreateBoneDlg.cpp : CCreateBoneDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#include "CreateBoneDlg.h"

#include "viewer.h"
#include "shdhandler.h"
#include "shdelem.h"
#include "treehandler2.h"
#include "treeelem2.h"

#include "sigeditwnd.h"
#include "motdlg.h"
#include "itidlg.h"

//#include <Commdlg.h>
//#include <Afxdlgs.h>

#include "GetDlgParams.h"
#include "inidata.h"

/// extern 
extern CMotDlg* g_motdlg;
extern CIniData* g_inidata;


/////////////////////////////////////////////////////////////////////////////
// CCreateBoneDlg

CCreateBoneDlg::CCreateBoneDlg( CMyD3DApplication* papp )
{
	ZeroMemory( name, CREATENAMELEN );
	m_papp = papp;
	m_cmdshow = SW_HIDE;
	m_ope = BONEOPE_CREATE;
}

CCreateBoneDlg::~CCreateBoneDlg()
{
}

LRESULT CCreateBoneDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();
	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CCreateBoneDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	ShowDlg( SW_HIDE );
	//EndDialog(wID);
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_papp->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	return 0;
}

LRESULT CCreateBoneDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	ShowDlg( SW_HIDE );
	if( g_inidata->opt_rdlg == CHKDLG_NES ){
		::MessageBox( m_papp->m_hWnd, "編集モードを、回転(R)に変更します。", "モード変更", MB_OK );
	}
	SendMessage( m_papp->m_hWnd, WM_COMMAND, (WPARAM)IDTBB_R, 0 );

	//EndDialog(wID);
	return 0;
}

void CCreateBoneDlg::SetWnd()
{
	m_dlg_wnd = m_hWnd;

	m_name_wnd = GetDlgItem( IDC_NAME );
}

int CCreateBoneDlg::ParamsToDlg()
{	
	m_name_wnd.SetWindowText( name );	

	switch( m_ope ){
	case BONEOPE_CREATE:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_CREATE, IDC_RADIO_POS, IDC_RADIO_CREATE );
		break;
	case BONEOPE_POS:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_CREATE, IDC_RADIO_POS, IDC_RADIO_POS );
		break;
	default:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_CREATE, IDC_RADIO_POS, IDC_RADIO_CREATE );
		m_ope = BONEOPE_CREATE;
		break;
	}

	return 0;
}

LRESULT CCreateBoneDlg::OnSetName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int isjoint;
	CShdElem* seljoint;
	seljoint = GetSelectedJoint( &isjoint );
	if( !seljoint )
		return 0;

	int ret;
	char tempchar[CREATENAMELEN];	
	ret = GetName( m_name_wnd, tempchar );
	if( ret || !(tempchar[0]) ){
		MessageBox( "名前が不正です。(1文字以上255文字以下)", "入力エラー", MB_OK );
		return 0;		
	}
	strcpy_s( name, CREATENAMELEN, tempchar );


	CTreeElem2* telem;
	telem = (*m_papp->m_thandler)( seljoint->serialno );
	if( !telem )
		return 0;

	telem->SetName( name );

//	if( isjoint && g_motdlg ){
//		ret = g_motdlg->Rename( seljoint->serialno, name );
//		_ASSERT( !ret );
//	}

	m_papp->m_sigeditwnd->Rename( seljoint->serialno, name );


	return 0;
}



int CCreateBoneDlg::ShowDlg( int cmdshow )
{

	m_cmdshow = cmdshow;	
	ShowWindow( m_cmdshow );
		
	ParamsToDlg();

	return 0;
}

CShdElem* CCreateBoneDlg::GetSelectedJoint( int* isjoint )
{
	int selno;
	//selno = m_papp->m_selectedno;
	m_papp->m_sigeditwnd->GetSelectedNo( &selno );
	if( (selno <= 0) || (selno >= m_papp->m_shandler->s2shd_leng) ){
		_ASSERT( 0 );
		*isjoint = 0;
		return 0;
	}

	CShdElem* selem;
	selem = (*m_papp->m_shandler)( selno );
	if( !selem ){
		*isjoint = 0;
		return 0;
	}

	if( (selem->IsJoint()) && (selem->type != SHDMORPH) ){
		*isjoint = 1;
		return selem;
	}else{
		*isjoint = 0;
		return selem;
	}
	
}

int CCreateBoneDlg::SetCurrentName( char* srcname )
{
	int leng;
	leng = (int)strlen( srcname );

	int cpleng;
	cpleng = min( 255, leng );

	strncpy_s( name, CREATENAMELEN, srcname, cpleng );
	name[ cpleng ] = 0;

	ParamsToDlg();

	return 0;
}

LRESULT CCreateBoneDlg::OnRadioCreate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_ope = BONEOPE_CREATE;
	ParamsToDlg();
	return 0;
}
LRESULT CCreateBoneDlg::OnRadioPos(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_ope = BONEOPE_POS;
	ParamsToDlg();
	return 0;
}

LRESULT CCreateBoneDlg::OnDelBone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	ret = (int)m_papp->m_sigeditwnd->OnMenuDelJoint( -1, 1 );
	if( ret ){
		DbgOut( "createbonedlg : OnDelBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int isjoint;
	CShdElem* selelem;
	selelem = GetSelectedJoint( &isjoint );

	if( isjoint ){
		m_papp->m_itidlg->m_object = ITIOBJ_JOINT;
		m_papp->m_itidlg->m_itimode = 0;
		ret = m_papp->m_itidlg->MoveObject( m_papp, m_papp->m_shandler, m_papp->m_mhandler, 
			selelem->serialno, 0, 0, 0, 0, 0.0f );
			//m_shiftbone, m_shiftface, m_shiftvert, 0, 0.0f );
		if( ret ){
			DbgOut( "createbonedlg : MoveObject error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}
