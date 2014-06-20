// JointLocDlg.cpp : CJointLocDlg のインプリメンテーション
#include "stdafx.h"
#include "JointLocDlg.h"

#include <treehandler2.h>
#include <treeelem2.h>
#include <shdelem.h>
#include <part.h>
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CJointLocDlg

CJointLocDlg::CJointLocDlg( CTreeElem2* srcte, CShdElem* srcse, HWND appwnd )
{
	m_te = srcte;
	m_se = srcse;
	m_appwnd = appwnd;

	m_firstloc.x = 0.0f;
	m_firstloc.y = 0.0f;
	m_firstloc.z = 0.0f;

	m_saveloc.x = 0.0f;
	m_saveloc.y = 0.0f;
	m_saveloc.z = 0.0f;

	m_temploc = m_saveloc;
}

CJointLocDlg::~CJointLocDlg()
{
}

LRESULT CJointLocDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();

	int isjoint;
	isjoint = m_se->IsJoint();
	if( isjoint == 0 ){
		_ASSERT( 0 );
		return -1;
	}
	CPart* partptr;
	partptr = m_se->part;
	if( !partptr ){
		_ASSERT( 0 );
		return -1;
	}
	m_firstloc = partptr->jointloc;
	m_saveloc = m_firstloc;
	m_temploc = m_firstloc;

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

int CJointLocDlg::DlgToParams()
{
	int ret;
	float tempx, tempy, tempz;
	ret = GetFloat( m_editx_wnd, &tempx );
	if( ret ){
		MessageBox( "X座標の値が不正です。", "入力エラー", MB_OK );
		return 1;
	}
	ret = GetFloat( m_edity_wnd, &tempy );
	if( ret ){
		MessageBox( "Y座標の値が不正です。", "入力エラー", MB_OK );
		return 1;
	}
	ret = GetFloat( m_editz_wnd, &tempz );
	if( ret ){
		MessageBox( "Z座標の値が不正です。", "入力エラー", MB_OK );
		return 1;
	}

	m_temploc.x = tempx;
	m_temploc.y = tempy;
	m_temploc.z = tempz;

	return 0;

}

LRESULT CJointLocDlg::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	if( (m_temploc.x != m_saveloc.x) || (m_temploc.y != m_saveloc.y) || (m_temploc.z != m_saveloc.z) ){
		::SendMessage( m_appwnd, WM_USER_CHANGE_JOINTLOC, 
			(WPARAM)(m_se->serialno), (LPARAM)&m_temploc );
		
		m_saveloc = m_temploc;
	}

	return 0;
}

LRESULT CJointLocDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;

	ret = DlgToParams();
	if( ret )
		return 0;

	if( (m_temploc.x != m_saveloc.x) || (m_temploc.y != m_saveloc.y) || (m_temploc.z != m_saveloc.z) ){
		::SendMessage( m_appwnd, WM_USER_CHANGE_JOINTLOC, 
			(WPARAM)(m_se->serialno), (LPARAM)&m_temploc ); 
		m_saveloc = m_temploc;
	}

	EndDialog(wID);
	return 0;
}

LRESULT CJointLocDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::SendMessage( m_appwnd, WM_USER_CHANGE_JOINTLOC, 
		(WPARAM)(m_se->serialno), (LPARAM)&m_firstloc ); 

	EndDialog(wID);
	return 0;
}

int CJointLocDlg::SetWnd()
{
	HWND temphwnd;

	temphwnd = GetDlgItem( IDC_TEXT_NAME );
	m_text_name_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_EDITX );
	m_editx_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_EDITY );
	m_edity_wnd = temphwnd;

	temphwnd = GetDlgItem( IDC_EDITZ );
	m_editz_wnd = temphwnd;

	return 0;
}

int CJointLocDlg::ParamsToDlg()
{
	m_text_name_wnd.SetWindowText( m_te->name );

	char tempchar[256];
	
	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", m_temploc.x );
	m_editx_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", m_temploc.y );
	m_edity_wnd.SetWindowText( tempchar );

	ZeroMemory( tempchar, 256 );
	sprintf_s( tempchar, 256, "%f", m_temploc.z );
	m_editz_wnd.SetWindowText( tempchar );
	
	return 0;
}





