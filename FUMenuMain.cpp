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
		AppendMenu( m_rsubmenu, MF_STRING, 65200, "MP単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65201, "MP単位：スプライン補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65202, "ボーン単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65203, "ボーン単位：スプライン補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65204, "モーション単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65205, "モーション単位：スプライン補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65206, "スプライン：制御点設定" );
		AppendMenu( m_rsubmenu, MF_STRING, 65207, "スプライン自動設定[全区間]" );
		AppendMenu( m_rsubmenu, MF_STRING, 65208, "スプライン自動設定[１区間]" );
		AppendMenu( m_rsubmenu, MF_STRING, 65209, "スプライン制御点全ロック" );
		AppendMenu( m_rsubmenu, MF_STRING, 65210, "スプライン制御点全ロック解除" );
	}else if( srckind == 1 ){
		AppendMenu( m_rsubmenu, MF_STRING, 65200, "キー単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65201, "キー単位：スプライン補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65202, "カメラ単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65203, "カメラ単位：スプライン補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65204, "アニメ単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65205, "アニメ単位：スプライン補間" );
	}else{
		AppendMenu( m_rsubmenu, MF_STRING, 65200, "キー単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65201, "キー単位：スプライン補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65204, "アニメ単位：線形補間" );
		AppendMenu( m_rsubmenu, MF_STRING, 65205, "アニメ単位：スプライン補間" );
	}

//	if( cAnimSets > 0 ){
//		CheckMenuItem(m_appmenu, 59900 + selindex, MF_CHECKED);
//	}

	return 0;
}


