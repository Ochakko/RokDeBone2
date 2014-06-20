#include "stdafx.h"

#include "FUMenuMain.h"

#define DBGH
#include <dbg.h>

#include <crtdbg.h>

#include "resource.h"


CFUMenuMain::CFUMenuMain()
{
	InitParams();
}
CFUMenuMain::~CFUMenuMain()
{

}

int CFUMenuMain::InitParams()
{
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CFUMenuMain::Create( HWND srchwnd )
{

	m_rmenu = LoadMenu( (HINSTANCE)GetWindowLong( srchwnd, GWL_HINSTANCE ), MAKEINTRESOURCE( IDR_FUMENU ) );
	m_rsubmenu = GetSubMenu( m_rmenu, 0 );

	return 0;
}


int CFUMenuMain::Destroy()
{
	DestroyMenu( m_rmenu );
	m_rmenu = 0;
	m_rsubmenu = 0;

	return 0;
}

int CFUMenuMain::TrackPopupMenu( HWND srchwnd, POINT pt, int srckind )
{

	Params2Dlg( srckind );

	int ret;
	ret = ::TrackPopupMenu( m_rsubmenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, srchwnd, NULL);
	return ret;
}

int CFUMenuMain::Params2Dlg( int srckind )
{

///////
//	if( m_iktrans )
//		CheckMenuItem( m_rmenu, ID_RMENU_IKTRANS, MF_CHECKED );
//	else
//		CheckMenuItem( m_rmenu, ID_RMENU_IKTRANS, MF_UNCHECKED );

	int iAnimSet, cAnimSets;
	cAnimSets = GetMenuItemCount( m_rsubmenu );
	for (iAnimSet = 0; iAnimSet < cAnimSets; iAnimSet++)
	{
		RemoveMenu(m_rsubmenu, 0, MF_BYPOSITION);
	}


	if( srckind == 0 ){
		AppendMenu( m_rsubmenu, MF_STRING, 65200, "MP�P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65201, "MP�P�ʁF�X�v���C�����" );
		AppendMenu( m_rsubmenu, MF_STRING, 65202, "�{�[���P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65203, "�{�[���P�ʁF�X�v���C�����" );
		AppendMenu( m_rsubmenu, MF_STRING, 65204, "���[�V�����P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65205, "���[�V�����P�ʁF�X�v���C�����" );
		AppendMenu( m_rsubmenu, MF_STRING, 65206, "�X�v���C���F����_�ݒ�" );
		AppendMenu( m_rsubmenu, MF_STRING, 65207, "�X�v���C�������ݒ�[�S���]" );
		AppendMenu( m_rsubmenu, MF_STRING, 65208, "�X�v���C�������ݒ�[�P���]" );
		AppendMenu( m_rsubmenu, MF_STRING, 65209, "�X�v���C������_�S���b�N" );
		AppendMenu( m_rsubmenu, MF_STRING, 65210, "�X�v���C������_�S���b�N����" );
	}else if( srckind == 1 ){
		AppendMenu( m_rsubmenu, MF_STRING, 65200, "�L�[�P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65201, "�L�[�P�ʁF�X�v���C�����" );
		AppendMenu( m_rsubmenu, MF_STRING, 65202, "�J�����P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65203, "�J�����P�ʁF�X�v���C�����" );
		AppendMenu( m_rsubmenu, MF_STRING, 65204, "�A�j���P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65205, "�A�j���P�ʁF�X�v���C�����" );
	}else{
		AppendMenu( m_rsubmenu, MF_STRING, 65200, "�L�[�P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65201, "�L�[�P�ʁF�X�v���C�����" );
		AppendMenu( m_rsubmenu, MF_STRING, 65204, "�A�j���P�ʁF���`���" );
		AppendMenu( m_rsubmenu, MF_STRING, 65205, "�A�j���P�ʁF�X�v���C�����" );
	}

//	if( cAnimSets > 0 ){
//		CheckMenuItem(m_appmenu, 59900 + selindex, MF_CHECKED);
//	}

	return 0;
}


