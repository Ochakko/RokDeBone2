// NewJointDlg.cpp : CNewJointDlg �̃C���v�������e�[�V����
#include "stdafx.h"
#include "NewJointDlg.h"
#include "GetDlgParams.h"

/////////////////////////////////////////////////////////////////////////////
// CNewJointDlg

CNewJointDlg::CNewJointDlg()
{
	jointnum = 1;
}

CNewJointDlg::~CNewJointDlg()
{
}

LRESULT CNewJointDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_num_wnd = GetDlgItem( IDC_EDIT_NUM );
	
	char textnum[256];
	ZeroMemory( textnum, 256 );
	sprintf_s( textnum, 256, "%d", jointnum );
	m_num_wnd.SetWindowText( textnum );


	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

LRESULT CNewJointDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int tempint = 0;
	ret = GetInt( m_num_wnd, &tempint );
	if( ret || (tempint < 1) || (tempint > 10) ){
		MessageBox( "�ǉ�����W���C���g�̐����s���ł��B(int 1�`10)",
			"���̓G���[", MB_OK );
		return 0;
	}
	jointnum = tempint;

	EndDialog(wID);
	return 0;
}

LRESULT CNewJointDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	jointnum = 1;
	EndDialog(wID);
	return 0;
}

