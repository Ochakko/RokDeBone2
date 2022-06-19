// TexSetting2Dlg.cpp : CTexSetting2Dlg のインプリメンテーション
//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

//#define NOTWINDOWSH

#include "TexSetting2Dlg.h"

//#include <stdio.h>
/**
#define STRICT
#include <windowsx.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <d3dx9.h>
**/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <coef.h>
//#include <basedat.h>
//#include <treehandler2.h>
//#include <treeelem2.h>
#include <shdhandler.h>
//#include <mothandler.h>
//#include <d3ddisp.h>
#include <mqomaterial.h>

#include <Commdlg.h>

#include "GetDlgParams.h"
#include "resid.h"

//#include <Afxdlgs.h>

/////////////////////////////////////////////////////////////////////////////
// CTexSetting2Dlg

CTexSetting2Dlg::CTexSetting2Dlg( CMQOMaterial* srcmat )
{
	m_mqomat = srcmat;

	ZeroMemory( m_exttexname, 256 * MAXEXTTEXNUM );
	int texno;
	for( texno = 0; texno < MAXEXTTEXNUM; texno++ ){
		m_exttexlist[texno] = m_exttexname[texno];
	}

	m_exttexnum = 0;
	m_exttexmode = 0;
	m_exttexrep = 0;
	m_exttexstep = 1;

	ZeroMemory( m_texname, 256 );
	ZeroMemory( m_bumpname, 256 );
	m_texrepx = 0;
	m_texrepy = 0;
	m_transparent = 0;
	m_uanime = 0.0f;
	m_vanime = 0.0f;
	m_texrule = 0;
	m_texanimtype = TEXANIM_WRAP;

	m_isfullpath = 0;

	m_glowmult.a = 1.0f;
	m_glowmult.r = 1.0f;
	m_glowmult.g = 1.0f;
	m_glowmult.b = 1.0f;
}

CTexSetting2Dlg::~CTexSetting2Dlg()
{
}


LRESULT CTexSetting2Dlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_dlg_wnd = m_hWnd;

	m_list_texname_wnd = GetDlgItem( IDC_LIST_TEXNAME );
	
	m_combo_texrule_wnd = GetDlgItem( IDC_COMBO_TEXRULE );
	//m_combo_anim_rep_wnd = GetDlgItem( IDC_COMBO_ANIM_REP );

	m_texrepu_wnd = GetDlgItem( IDC_TEXREPU );
	m_texrepv_wnd = GetDlgItem( IDC_TEXREPV );
	m_uanim_wnd = GetDlgItem( IDC_UANIM );
	m_vanim_wnd = GetDlgItem( IDC_VANIM );
	//m_anim_step_wnd = GetDlgItem( IDC_ANIM_STEP );

	m_radio_multi_wnd = GetDlgItem( IDC_RADIO_TEXMULTI );
	//m_check_transparent_wnd = GetDlgItem( IDC_CHECK_TRANSPARENT );


	m_ok_wnd = GetDlgItem( IDOK );
	m_cancel_wnd = GetDlgItem( IDCANCEL );
	m_name_add_wnd = GetDlgItem( IDC_NAME_ADD );
	m_name_del_wnd = GetDlgItem( IDC_NAME_DEL );
	m_name_up_wnd = GetDlgItem( IDC_NAME_UP );
	m_name_down_wnd = GetDlgItem( IDC_NAME_DOWN );

	m_bump_wnd = GetDlgItem( IDC_BUMPMAP );

	m_glowmultR_wnd = GetDlgItem( IDC_GLOWMULT_R );
	m_glowmultG_wnd = GetDlgItem( IDC_GLOWMULT_G );
	m_glowmultB_wnd = GetDlgItem( IDC_GLOWMULT_B );

//////////////
	
	m_radio_multi_wnd.EnableWindow( FALSE );
	//m_check_transparent_wnd.EnableWindow( FALSE );
	m_texrepu_wnd.EnableWindow( FALSE );
	m_texrepv_wnd.EnableWindow( FALSE );

	m_list_texname_wnd.SendMessage( LB_SETHORIZONTALEXTENT, (WPARAM)14 * 256, 0 );


/////////////
	char* srcname;
	int srcleng, cpleng;
	char* dstname;


	if( *m_mqomat->tex != 0 ){
		srcleng = (int)strlen( m_mqomat->tex );
		if( srcleng >= 256 ){
			DbgOut( "TexSetting2Dlg : OnInitDialog : srcleng error !!! %s\n", m_mqomat->tex );
			_ASSERT( 0 );
		}
		cpleng = min( srcleng, 255 );

		strncpy_s( m_texname, 256, m_mqomat->tex, cpleng );
		m_texname[cpleng] = 0;
	}

	if( *m_mqomat->bump != 0 ){
		srcleng = (int)strlen( m_mqomat->bump );
		if( srcleng >= 256 ){
			DbgOut( "TexSetting2Dlg : OnInitDialog : srcleng error !!! %s\n", m_mqomat->bump );
			_ASSERT( 0 );
		}
		cpleng = min( srcleng, 255 );

		strncpy_s( m_bumpname, 256, m_mqomat->bump, cpleng );
		m_bumpname[cpleng] = 0;
	}


//	m_texrepx = m_mqomat->texrepx;
//	m_texrepy = m_mqomat->texrepy;
	m_transparent = m_mqomat->transparent;
	m_uanime = m_mqomat->uanime;
	m_vanime = m_mqomat->vanime;
	m_texrule = m_mqomat->texrule; //
	m_texanimtype = m_mqomat->texanimtype;

	m_exttexnum = m_mqomat->exttexnum;
	m_exttexmode = m_mqomat->exttexmode;
	m_exttexrep = m_mqomat->exttexrep;
	m_exttexstep = m_mqomat->exttexstep;

	int texno;
	for( texno = 0; texno < m_exttexnum; texno++ ){
		srcname = *( m_mqomat->exttexname + texno );
		srcleng = (int)strlen( srcname );
		if( srcleng >= 256 ){
			DbgOut( "TexSetting2Dlg : OnInitDialog : srcleng error !!! %s\n", srcname );
			_ASSERT( 0 );
		}
		cpleng = min( srcleng, 255 );
		
		dstname = m_exttexname[texno];
		strncpy_s( dstname, 256, srcname, cpleng );
		*( dstname + cpleng ) = 0;

	}

	m_glowmult.r = m_mqomat->glowmult[0];
	m_glowmult.g = m_mqomat->glowmult[1];
	m_glowmult.b = m_mqomat->glowmult[2];

///////////////
	int ret;

	ret = Params2Dlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}




	return 1;  // システムにフォーカスを設定させます
}

int CTexSetting2Dlg::Params2Dlg()
{
	char tempchar[1024];

//radio
	switch( m_exttexmode ){
	case EXTTEXMODE_NORMAL:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_TEXONE, IDC_RADIO_TEXMULTI, IDC_RADIO_TEXONE );
		break;
	case EXTTEXMODE_ANIM:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_TEXONE, IDC_RADIO_TEXMULTI, IDC_RADIO_TEXPALA );
		break;
	case EXTTEXMODE_GLOW:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_TEXONE, IDC_RADIO_TEXMULTI, IDC_RADIO_TEXGLOW );
		break;
	case EXTTEXMODE_GLOWALPHA:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_TEXONE, IDC_RADIO_TEXMULTI, IDC_RADIO_TEXGLOWALPHA );
		break;
	case EXTTEXMODE_MULTI:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_TEXONE, IDC_RADIO_TEXMULTI, IDC_RADIO_TEXMULTI );
		break;
	default:
		_ASSERT( 0 );
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_TEXONE, IDC_RADIO_TEXMULTI, IDC_RADIO_TEXONE );
		m_exttexmode = EXTTEXMODE_NORMAL;
		break;
	}

	switch( m_isfullpath ){
	case 0:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_FULLPATH, IDC_RADIO_ONLYNAME, IDC_RADIO_ONLYNAME );
		break;
	case 1:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_FULLPATH, IDC_RADIO_ONLYNAME, IDC_RADIO_FULLPATH );
		break;
	default:
		_ASSERT( 0 );
		break;
	}


	switch( m_transparent ){
	case 0:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NOALPHA, IDC_RADIO_WITHTRANS, IDC_RADIO_NOALPHA );
		break;
	case 2://!!!
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NOALPHA, IDC_RADIO_WITHTRANS, IDC_RADIO_WITHALPHA );
		break;
	case 1://!!!
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NOALPHA, IDC_RADIO_WITHTRANS, IDC_RADIO_WITHTRANS );
		break;
	default:
		_ASSERT( 0 );
		m_transparent = 0;
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NOALPHA, IDC_RADIO_WITHTRANS, IDC_RADIO_NOALPHA );
		break;
	}


	switch( m_texanimtype ){
	case TEXANIM_WRAP:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_WRAP, IDC_RADIO_NOLIMIT, IDC_RADIO_WRAP );
		break;
	case TEXANIM_NOLIMIT:
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_WRAP, IDC_RADIO_NOLIMIT, IDC_RADIO_NOLIMIT );
		break;
	default:
		_ASSERT( 0 );
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_WRAP, IDC_RADIO_NOLIMIT, IDC_RADIO_WRAP );
		m_texanimtype = TEXANIM_WRAP;
		break;
	}


//combo
	//addstring ではsortされて格納されるので、1,2,3の番号を先頭につける。
	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "1:" );
	strcat_s( tempchar, 1024, texrule_str[TEXRULE_X] );
	m_combo_texrule_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "2:" );
	strcat_s( tempchar, 1024, texrule_str[TEXRULE_Y] );
	m_combo_texrule_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "3:" );
	strcat_s( tempchar, 1024, texrule_str[TEXRULE_Z] );
	m_combo_texrule_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "4:" );
	strcat_s( tempchar, 1024, texrule_str[TEXRULE_CYLINDER] );
	m_combo_texrule_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "5:" );
	strcat_s( tempchar, 1024,texrule_str[TEXRULE_SPHERE] );
	m_combo_texrule_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	ZeroMemory( tempchar, 1024 );
	strcpy_s( tempchar, 1024, "6:" );
	strcat_s( tempchar, 1024, texrule_str[TEXRULE_MQ] );
	m_combo_texrule_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	//strcpy( tempchar, "6:" );
	//strcat( tempchar, texrule_str[TEXRULE_RAP] );
	//m_combo_texrule_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	m_combo_texrule_wnd.SendMessage( CB_SETCURSEL, m_texrule, 0 );


	////
//	m_combo_anim_rep_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)exttexrep_str[EXTTEXREP_STOP] );
//	m_combo_anim_rep_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)exttexrep_str[EXTTEXREP_CLAMP] );
//	m_combo_anim_rep_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)exttexrep_str[EXTTEXREP_ROUND] );
//	m_combo_anim_rep_wnd.SendMessage( CB_SETCURSEL, m_exttexrep, 0 );


//edit
	sprintf_s( tempchar, 1024, "%d", m_texrepx );
	m_texrepu_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%d", m_texrepy );
	m_texrepv_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_uanime );
	m_uanim_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_vanime );
	m_vanim_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_glowmult.r );
	m_glowmultR_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_glowmult.g );
	m_glowmultG_wnd.SetWindowText( tempchar );

	sprintf_s( tempchar, 1024, "%f", m_glowmult.b );
	m_glowmultB_wnd.SetWindowText( tempchar );

//	sprintf_s( tempchar, 1024, "%d", m_exttexstep );
//	m_anim_step_wnd.SetWindowText( tempchar );

//check
	//if( m_transparent & 0x01 ){
	//	m_dlg_wnd.CheckDlgButton( IDC_CHECK_TRANSPARENT, BST_CHECKED );
	//}else{
	//	m_dlg_wnd.CheckDlgButton( IDC_CHECK_TRANSPARENT, BST_UNCHECKED );
	//}

//bump
	m_bump_wnd.SetWindowText( m_bumpname );


//list
	int ret;
	ret = TextureParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}


//////////

	return 0;
}

int CTexSetting2Dlg::TextureParamsToDlg()
{
	char tempchar[1024];
	m_list_texname_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );
	
	if( m_texname[0] ){
		LRESULT lres;
		tempchar[0] = 0;
		Get3Digit( 0, tempchar );
		strcat_s( tempchar, 1024, m_texname );

		lres = m_list_texname_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)tempchar );
		if( (lres == LB_ERR) || (lres == LB_ERRSPACE) ){
			_ASSERT( 0 );
			return 1;
		}
	}


	int texno;
	for( texno = 0; texno < m_exttexnum; texno++ ){
		LRESULT lres;
		tempchar[0] = 0;
		Get3Digit( texno + 1, tempchar );//!!! texno + 1
		strcat_s( tempchar, 1024, m_exttexname[texno] );

		lres = m_list_texname_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)tempchar );
		if( (lres == LB_ERR) || (lres == LB_ERRSPACE) ){
			_ASSERT( 0 );
			return 1;
		}
	}

	


	return 0;
}

LRESULT CTexSetting2Dlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret = 0;
	int cmpret = 0;
	char errmsg[1024];
	ZeroMemory( errmsg, 1024 );

	//char tempchar[1024];
	int tempint;
	float tempfloat;

//// radio
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_NOALPHA );
	if( ischecked == BST_CHECKED ){
		m_transparent = 0;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_WITHALPHA );
	if( ischecked == BST_CHECKED ){
		m_transparent = 2;//!!!
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_WITHTRANS );
	if( ischecked == BST_CHECKED ){
		m_transparent = 1;//!!!
	}


	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_TEXONE );
	if( ischecked == BST_CHECKED ){
		m_exttexmode = EXTTEXMODE_NORMAL;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_TEXPALA );
	if( ischecked == BST_CHECKED ){
		m_exttexmode = EXTTEXMODE_ANIM;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_TEXMULTI );
	if( ischecked == BST_CHECKED ){
		m_exttexmode = EXTTEXMODE_MULTI;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_TEXGLOW );
	if( ischecked == BST_CHECKED ){
		m_exttexmode = EXTTEXMODE_GLOW;
		m_transparent = 0;//!!!強制
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_TEXGLOWALPHA );
	if( ischecked == BST_CHECKED ){
		m_exttexmode = EXTTEXMODE_GLOWALPHA;
		m_transparent = 2;//!!!強制
	}



	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_WRAP );
	if( ischecked == BST_CHECKED ){
		m_texanimtype = TEXANIM_WRAP;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_NOLIMIT );
	if( ischecked == BST_CHECKED ){
		m_texanimtype = TEXANIM_NOLIMIT;
	}


	CheckIsFullPath();


////combo
	int combono;
	combono = (int)m_combo_texrule_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR )
		return 0;
	m_texrule = combono;

//	combono = (int)m_combo_anim_rep_wnd.SendMessage( CB_GETCURSEL, 0, 0 );
//	if( combono == CB_ERR )
//		return 0;
//	m_exttexrep = combono;
	

////edit
	ret = GetInt( m_texrepu_wnd, &tempint );
	if( ret ){
		strcpy_s( errmsg, 1024, "U方向の繰り返し回数が不正です。正のint" );
		goto endof_onok;
	}
	m_texrepx = tempint;

	ret = GetInt( m_texrepv_wnd, &tempint );
	if( ret ){
		strcpy_s( errmsg, 1024, "V方向の繰り返し回数が不正です。正のint" );
		goto endof_onok;
	}
	m_texrepy = tempint;
	
	ret = GetFloat( m_uanim_wnd, &tempfloat );
	if( ret ){
		strcpy_s( errmsg, 1024, "U方向移動量 が不正です。float" );
		goto endof_onok;
	}
	m_uanime = tempfloat;

	ret = GetFloat( m_vanim_wnd, &tempfloat );
	if( ret ){
		strcpy_s( errmsg, 1024, "V方向移動量 が不正です。float" );
		goto endof_onok;
	}
	m_vanime = tempfloat;

	ret = GetFloat( m_glowmultR_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		strcpy_s( errmsg, 1024, "グロー倍率の赤成分が不正です。正の小数を指定してください。" );
		goto endof_onok;
	}
	m_glowmult.r = tempfloat;

	ret = GetFloat( m_glowmultG_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		strcpy_s( errmsg, 1024, "グロー倍率の緑成分が不正です。正の小数を指定してください。" );
		goto endof_onok;
	}
	m_glowmult.g = tempfloat;

	ret = GetFloat( m_glowmultB_wnd, &tempfloat );
	if( ret || (tempfloat < 0.0f) ){
		strcpy_s( errmsg, 1024, "グロー倍率の青成分が不正です。正の小数を指定してください。" );
		goto endof_onok;
	}
	m_glowmult.b = tempfloat;


//	ret = GetInt( m_anim_step_wnd, &tempint );
//	if( ret ){
//		strcpy_s( errmsg, 1024, "何フレームごとに変化させるかの指定　が不正です。正のint" );
//		goto endof_onok;
//	}
//	m_exttexstep = tempint;

//// bump
	ret = GetText( m_bump_wnd, m_bumpname, 256 );
	if( ret ){
		strcpy_s( errmsg, 1024, "バンプマップの名前が不正です。２５５バイトまで" );
		goto endof_onok;
	}


////check box

	//UINT checkflag;
	//checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CHECK_TRANSPARENT );
	//if( checkflag == BST_CHECKED ){
	//	m_transparent |= 0x00000001;
	//}else if( checkflag == BST_UNCHECKED ){
	//	m_transparent &= ~0x00000001;


	goto endof_onok;
endof_onok:
	if( !ret ){
		EndDialog(wID);
		//DestroyWindow();
	}else{
		::MessageBox( m_hWnd, errmsg, "ダイアログのパラメータ不正", MB_OK );
	}

	return 0;
}


int CTexSetting2Dlg::CheckIsFullPath()
{
	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_FULLPATH );
	if( ischecked == BST_CHECKED ){
		m_isfullpath = 1;
	}
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_ONLYNAME );
	if( ischecked == BST_CHECKED ){
		m_isfullpath = 0;
	}

	return 0;
}



LRESULT CTexSetting2Dlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	//DestroyWindow();
	return 0;
}

int CTexSetting2Dlg::Get3Digit( int srcint, char* dstptr )
{
	char tempchar[256];
	sprintf_s( tempchar, 256, "%d : ", srcint );


	if( srcint > 999 ){
		_ASSERT( 0 );
		strcat_s( dstptr, 1024, "---" );
	}else if( srcint >= 100 ){
		strcat_s( dstptr, 1024, tempchar );		
	}else if( srcint >= 10 ){
		strcat_s( dstptr, 1024, "0" );
		strcat_s( dstptr, 1024, tempchar );
	}else if( srcint >= 0 ){
		strcat_s( dstptr, 1024, "00" );
		strcat_s( dstptr, 1024, tempchar );
	}else{
		_ASSERT( 0 );
		strcat_s( dstptr, 1024, "---" );
	}


	return 0;
}

LRESULT CTexSetting2Dlg::OnRefBumpMap(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"Texture File (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 ){
		return 0;
	}

	int nameleng;
	nameleng = (int)strlen( buf );
	if( nameleng >= 256 ){
		DbgOut( "TexSetting2Dlg : OnRefBumpMap : nameleng too long error !!! %s\n", buf );
		_ASSERT( 0 );
		return 0;
	}

	char buf2[256];
	int ch = '\\';
	char* enptr;
	enptr = strrchr( buf, ch );
	if( enptr && *(enptr + 1) ){
		strcpy_s( buf2, 256, enptr + 1 );
	}else{
		buf2[0] = 0;
	}

	strcpy_s( m_bumpname, 256, buf2 );

	m_bump_wnd.SetWindowText( m_bumpname );


	return 0;
}


LRESULT CTexSetting2Dlg::OnNameAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( m_exttexnum >= MAXEXTTEXNUM ){
		DbgOut( "TexSetting2Dlg : OnNameAdd : m_exttexnum error !!!\n" );
		_ASSERT( 0 );
		return 0;
	}

	EnableButtons( 0 );

	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"画像ファイル\0*.bmp;*.dds;*.dib;*.hdr;*.jpg;*.pfm;*.png;*.ppm;*.tga\0All Files\0*.*\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt =NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName(&ofn) == 0 ){
		EnableButtons( 1 );
		return 0;
	}

	EnableButtons( 1 );


	int nameleng;
	nameleng = (int)strlen( buf );
	if( nameleng >= 256 ){
		DbgOut( "TexSetting2Dlg : OnNameAdd : nameleng too long error !!! %s\n", buf );
		_ASSERT( 0 );
		return 0;
	}


	CheckIsFullPath();//!!!


	char buf2[256];
	if( m_isfullpath == 0 ){
		int ch = '\\';
		char* enptr;
		enptr = strrchr( buf, ch );
		if( enptr && *(enptr + 1) ){
			strcpy_s( buf2, 256, enptr + 1 );
		}else{
			buf2[0] = 0;
		}
	}else{
		strcpy_s( buf2, 256, buf );
	}


	int ret;
	char tempchar[1024];
	int addno;
	if( m_exttexnum > 0 ){

		m_exttexnum++;

		strcpy_s( m_exttexname[m_exttexnum - 1], 256, buf2 );

		addno = m_exttexnum;
		tempchar[0] = 0;
		Get3Digit( addno, tempchar );
		strcat_s( tempchar, 256, m_exttexname[m_exttexnum - 1] );

		ret = (int)m_list_texname_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)tempchar );
		if( ret == LB_ERR ){
			_ASSERT( 0 );
			return 1;
		}


	}else if( m_exttexnum == 0 ){

		if( m_texname[0] == 0 ){
			
			m_exttexnum = 0;

			strcpy_s( m_texname, 256, buf2 );

			addno = 0;
			tempchar[0] = 0;
			Get3Digit( addno, tempchar );
			strcat_s( tempchar, 1024, m_texname );

			ret = (int)m_list_texname_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)tempchar );
			if( ret == LB_ERR ){
				_ASSERT( 0 );
				return 1;
			}
			
		}else{
			m_exttexnum = 1;

			strcpy_s( m_exttexname[m_exttexnum - 1], 256, buf2 );

			addno = m_exttexnum;
			tempchar[0] = 0;
			Get3Digit( addno, tempchar );
			strcat_s( tempchar, 1024, m_exttexname[m_exttexnum - 1] );

			ret = (int)m_list_texname_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)tempchar );
			if( ret == LB_ERR ){
				_ASSERT( 0 );
				return 1;
			}
		}

	}else{
		//exttexnum 負
		DbgOut( "TexSetting2Dlg : OnNameAdd : exttexnum < 0 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
LRESULT CTexSetting2Dlg::OnNameDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int selindex;
	selindex = (int)m_list_texname_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( selindex == LB_ERR ){
		
		//何も選択していないときもLB_ERRが返る
		return 0;
	}

	if( (selindex < 0) || (selindex > m_exttexnum) ){
		_ASSERT( 0 );
		return 1;
	}

	int nextselindex;

	int texno, deltexno;
	if( m_exttexnum > 0 ){
		if( selindex != 0 ){
			deltexno = selindex - 1;
			_ASSERT( deltexno >= 0 );//m_exttexnum > 0 だから、まず大丈夫。
			
			//ひとつずつ、ずらす
			for( texno = deltexno; texno < m_exttexnum - 1; texno++ ){
				strcpy_s( m_exttexname[texno], 256, m_exttexname[texno + 1] );
			}

			m_exttexnum--;
			nextselindex = m_exttexnum;
		}else{
			strcpy_s( m_texname, 256, m_exttexname[0] );

			for( texno = 0; texno < m_exttexnum - 1; texno++ ){
				strcpy_s( m_exttexname[texno], 256, m_exttexname[texno + 1] );
			}

			m_exttexnum--;
			nextselindex = selindex;

		}
	}else if( m_exttexnum == 0 ){
		ZeroMemory( m_texname, 256 );
		nextselindex = -1;
	}else{
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = TextureParamsToDlg();
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	if( nextselindex >= 0 ){
		m_list_texname_wnd.SendMessage( LB_SETCURSEL, (WPARAM)nextselindex, 0 );
	}

	return 0;
}
LRESULT CTexSetting2Dlg::OnNameUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int selindex;
	selindex = (int)m_list_texname_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( selindex == LB_ERR ){
		
		//何も選択していないときもLB_ERRが返る
		return 0;
	}

	if( (selindex < 0) || (selindex > m_exttexnum) ){
		_ASSERT( 0 );
		return 1;
	}

	if( selindex == 0 ){
		//一番上なので、処理しない。
		return 0;

	}else if( selindex == 1 ){
		char tempchar[1024];
		strcpy_s( tempchar, 1024, m_texname );//前を保存
		strcpy_s( m_texname, 256, m_exttexname[0] );
		strcpy_s( m_exttexname[0], 256, tempchar );

		ret = TextureParamsToDlg();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		m_list_texname_wnd.SendMessage( LB_SETCURSEL, (WPARAM)0, 0 );
		
	}else if( selindex >= 2 ){
		//一個前と、交換する
		char tempchar[1024];
		strcpy_s( tempchar, 1024, m_exttexname[selindex - 2] );//前を保存
		strcpy_s( m_exttexname[selindex - 2], 256, m_exttexname[selindex - 1] );
		strcpy_s( m_exttexname[selindex - 1], 256, tempchar );

		ret = TextureParamsToDlg();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		m_list_texname_wnd.SendMessage( LB_SETCURSEL, (WPARAM)(selindex - 1), 0 );
	}


	return 0;
}
LRESULT CTexSetting2Dlg::OnNameDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	int selindex;
	selindex = (int)m_list_texname_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( selindex == LB_ERR ){
		
		//何も選択していないときもLB_ERRが返る
		return 0;
	}

	if( (selindex < 0) || (selindex > m_exttexnum) ){
		_ASSERT( 0 );
		return 1;
	}
	
	if( selindex == m_exttexnum ){
		//一番下なので、処理しない。
		return 0;

	}else if( selindex >= 1 ){
		//一個後と交換する。
		char tempchar[1024];
		strcpy_s( tempchar, 1024, m_exttexname[selindex] );//後を保存
		strcpy_s( m_exttexname[selindex], 256, m_exttexname[selindex - 1] );
		strcpy_s( m_exttexname[selindex - 1], 256, tempchar );

		ret = TextureParamsToDlg();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		m_list_texname_wnd.SendMessage( LB_SETCURSEL, (WPARAM)selindex + 1, 0 );

	}else if( selindex == 0 ){
		char tempchar[1024];
		strcpy_s( tempchar, 1024, m_exttexname[0] );//後を保存
		strcpy_s( m_exttexname[0], 256, m_texname );
		strcpy_s( m_texname, 256, tempchar );

		ret = TextureParamsToDlg();
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		m_list_texname_wnd.SendMessage( LB_SETCURSEL, (WPARAM)1, 0 );

	}



	return 0;
}

int CTexSetting2Dlg::EnableButtons( int srcflag )
{
	if( srcflag == 0 ){
		m_ok_wnd.EnableWindow( FALSE );
		m_cancel_wnd.EnableWindow( FALSE );
		m_name_add_wnd.EnableWindow( FALSE );
		m_name_del_wnd.EnableWindow( FALSE );
		m_name_up_wnd.EnableWindow( FALSE );
		m_name_down_wnd.EnableWindow( FALSE );

	}else{
		m_ok_wnd.EnableWindow( TRUE );
		m_cancel_wnd.EnableWindow( TRUE );
		m_name_add_wnd.EnableWindow( TRUE );
		m_name_del_wnd.EnableWindow( TRUE );
		m_name_up_wnd.EnableWindow( TRUE );
		m_name_down_wnd.EnableWindow( TRUE );

	}

	return 0;
}




