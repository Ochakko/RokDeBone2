#include "stdafx.h"

#include "ColDlg.h"

#define DBGH
#include <dbg.h>


#include <crtdbg.h>

#include "inidata.h"

//extern 
extern CIniData* g_inidata;


CColDlg::CColDlg()
{
	InitParams();
}

CColDlg::~CColDlg()
{

}

int CColDlg::InitParams()
{
	ZeroMemory( &m_cc, sizeof( CHOOSECOLOR ) );

	m_cc.lStructSize = sizeof( CHOOSECOLOR );
	m_cc.lpCustColors = m_custom;
	m_cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	int cusno;
	for( cusno = 0; cusno < 16; cusno++ ){
		m_custom[cusno] = RGB( 255, 255, 255 );
	}


	return 0;
}

int CColDlg::SetCustomColor( COLORREF* srccol )
{
	int cusno;
	for( cusno = 0; cusno < 16; cusno++ ){
		m_custom[cusno] = *(srccol + cusno);
	}

	return 0;
}


int CColDlg::Choose( HWND srchwnd, COLORREF* dstcol )
{
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// OKボタンを押したときは、return 1; それ以外は、return 0
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	m_cc.hwndOwner = srchwnd;
	//m_cc.rgbResult = m_custom[0];
	m_cc.rgbResult = *dstcol;

	BOOL bret;

	bret = ::ChooseColor( &m_cc );
	if( bret != 0 ){
		*dstcol = m_cc.rgbResult;

		MoveMemory( g_inidata->cc, m_custom, sizeof( COLORREF ) * 16 );//!!!!!!!!!!!!!!!!!!

		return 1;
	}else{
		return 0;
	}
}
