#include "stdafx.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <memory.h>

#include <windows.h>

#include <crtdbg.h>


#include <coef.h>
#define DBGH
#include <dbg.h>

#include "inidata.h"


extern int g_index32bit;//!!!!!!!!!!


CIniData::CIniData()
{
	InitParams();
}
CIniData::~CIniData()
{

	DestroyObjs();

}

int CIniData::InitParams()
{
	//file操作用
	m_hfile = INVALID_HANDLE_VALUE;
	m_buf = 0;
	m_pos = 0;
	m_bufleng = 0;

//DbgOut( "inidata : InitParams 0 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );
	InitLoadFlag();
//DbgOut( "inidata : InitParams 1 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );
	InitDefaultValue();
//DbgOut( "inidata : InitParams 2 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );
	SetDefaultValue( 1 );
//DbgOut( "inidata : InitParams 3 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );

	return 0;
}

int CIniData::InitDefaultValue()
{
	//default値を定義
	d_index32bit = 0;
	//メインウインドウ
	d_main_posx = 0;
	d_main_posy = 0;
	d_main_width = 800;
	d_main_height = 700;

	//モーションダイアログ
	d_motdlg_disp = 1;
	d_motdlg_posx = 200;
	d_motdlg_posy = 600;
	d_motdlg_calcsp = 0;
//	d_motdlg_height = 220;
	d_motdlg_height = 265;

	//階層構造エディタ
	d_sigdlg_disp = 1;
	d_sigdlg_posx = 0;
	d_sigdlg_posy = 0;
	
	//カメラダイアログ
	d_camdlg_disp = 0;
	d_camdlg_posx = 0;
	d_camdlg_posy = 0;
	d_camdlg_move = 20.0f;

	//gpa
	d_gpa_move = 1.0f;

	//graphwnd
	d_graph_disp = 1;

	d_usekinect = 0;

	//背景
	dd_bg_color = RGB( 30, 65, 90 );
	//dd_bg_color = (255 << 24) | (90 << 16) | (65 << 8) | 30;
//DbgOut( "inidata : InitDefaultValue : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );

	d_bg_tex1[0] = 0;
	d_bg_tex2[0] = 0;
	d_bg_mvu = 0.0f;
	d_bg_mvv = 0.0f;
	d_bg_fixsize = 0;

	//平行光源
	d_dirlight_col = RGB( 255, 255, 255 );

	//オプション
	d_opt_jointdisp = 1;
	d_opt_polydisp = POLYDISP_ALL;
	d_opt_gdisp = 1;
	d_opt_namequa = QUATYPE_NAME;

	d_opt_jointsize = 100;
	d_opt_jointalpha = 255;
	d_opt_undodlg = CHKDLG_NES;
	d_opt_rdlg = CHKDLG_NES;
	d_opt_infdlg = CHKDLG_NES;
	d_opt_modlg = CHKDLG_NES;

	d_opt_bmdisp = 1;
	d_opt_previewfps = 60;
	d_opt_motstep = 1;
	d_opt_ikfk = 0;
	d_opt_iktdisp = 1;

	d_opt_MouseL = MOUSEOPE_CAMROT;
	d_opt_MouseR = MOUSEOPE_CAMMOVE;
	d_opt_MouseW = MOUSEOPE_BONETWIST;


	d_opt_unselline = RGB( 229, 127, 178 );
	d_opt_selline = RGB( 29, 229, 215 );
	d_opt_unselpoint = RGB( 229, 127, 178 );
	d_opt_selpoint = RGB( 29, 229, 215 );
	d_opt_pointsize = 5;

/////////////
//DbgOut( "inidata : InitDefaultValue end : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );

	//customcolor
	int ccno;
	for( ccno = 0; ccno < 16; ccno++ ){
		d_cc[ccno] = RGB( 255, 255, 255 );
	}


	//boneline
	d_selectcol = RGB( 255, 0, 0 );
	d_unselectcol = RGB( 64, 128, 128 );


	//viewparam
	int vpno;
	for( vpno = 0; vpno < 4; vpno++ ){
		d_viewparam[vpno].degxz = DEGXZINITVAL;
		d_viewparam[vpno].degy = DEGYINITVAL;
		d_viewparam[vpno].eye_y = EYEYINITVAL;
		d_viewparam[vpno].camdist = CAMDISTINITVAL;

		d_viewparam[vpno].targetpos.x = 0.0f;
		d_viewparam[vpno].targetpos.y = EYEYINITVAL;
		d_viewparam[vpno].targetpos.z = 0.0f;		
	}

	d_iklevelope = 5;
	d_forbidDD = 0;

	d_mqo_bonetype = BONETYPE_RDB2;
	d_mqo_mult = 1.0f;

	d_toonedgecol = 0.6f;
	d_toonedgew = 8.0f;

	return 0;
}
int CIniData::InitLoadFlag()
{
	//fileから読み込んだかどうかのフラグ
	f_index32bit = 0;
	//メインウインドウ
	f_main_posx = 0;
	f_main_posy = 0;
	f_main_width = 0;
	f_main_height = 0;

	//モーションダイアログ
	f_motdlg_disp = 0;
	f_motdlg_posx = 0;
	f_motdlg_posy = 0;
	f_motdlg_calcsp = 0;
	f_motdlg_height = 0;

	//階層構造エディタ
	f_sigdlg_disp = 0;
	f_sigdlg_posx = 0;
	f_sigdlg_posy = 0;
	
	//カメラダイアログ
	f_camdlg_disp = 0;
	f_camdlg_posx = 0;
	f_camdlg_posy = 0;
	f_camdlg_move = 0;

	//gpa
	f_gpa_move = 0;

	//graphwnd
	f_graph_disp = 0;

	f_usekinect = 0;

	//背景
	f_bg_color = 0;
	f_bg_tex1 = 0;
	f_bg_tex2 = 0;
	f_bg_mvu = 0;
	f_bg_mvv = 0;
	f_bg_fixsize = 0;

	//平行光源
	f_dirlight_col = 0;

	//オプション
	f_opt_jointdisp = 0;
	f_opt_polydisp = 0;
	f_opt_gdisp = 0;
	f_opt_namequa = 0;

	f_opt_jointsize = 0;
	f_opt_jointalpha = 0;
	f_opt_undodlg = 0;
	f_opt_rdlg = 0;
	f_opt_infdlg = 0;
	f_opt_modlg = 0;

	f_opt_bmdisp = 0;
	f_opt_previewfps = 0;
	f_opt_motstep = 0;
	f_opt_ikfk = 0;
	f_opt_iktdisp = 0;

	f_opt_MouseL = 0;
	f_opt_MouseR = 0;
	f_opt_MouseW = 0;

	f_opt_unselline = 0;
	f_opt_selline = 0;
	f_opt_unselpoint = 0;
	f_opt_selpoint = 0;
	f_opt_pointsize = 0;

	//customcolor
	int ccno;
	for( ccno = 0; ccno < 16; ccno++ ){
		f_cc[ccno] = 0;
	}

	//boneline
	f_selectcol = 0;
	f_unselectcol = 0;

	//viewparam
	int vpno;
	for( vpno = 0; vpno < 4; vpno++ ){
		f_viewparam[vpno] = 0;
	}

	f_iklevelope = 0;
	f_forbidDD = 0;

	f_mqo_bonetype = 0;
	f_mqo_mult = 0;

	f_toonedgecol = 0;
	f_toonedgew = 0;

	return 0;
}
int CIniData::SetDefaultValue( int flag )
//flag == 0のときは、読み込みフラグ==0のものだけセット、flag == 1の時は、全てセット。
{
	if( flag == 0 ){
		if( f_index32bit == 0 )
			index32bit = d_index32bit;
		//メインウインドウ
		if( f_main_posx == 0 )
			main_posx = d_main_posx;
		if( f_main_posy == 0 )
			main_posy = d_main_posy;
		if( f_main_width == 0 )
			main_width = d_main_width;
		if( f_main_height == 0 )
			main_height = d_main_height;

		//モーションダイアログ
		if( f_motdlg_disp == 0 )
			motdlg_disp = d_motdlg_disp;
		if( f_motdlg_posx == 0 )
			motdlg_posx = d_motdlg_posx;
		if( f_motdlg_posy == 0 )
			motdlg_posy = d_motdlg_posy;
		if( f_motdlg_calcsp == 0 )
			motdlg_calcsp = d_motdlg_calcsp;
		if( f_motdlg_height == 0 )
			motdlg_height = d_motdlg_height;

		//階層構造エディタ
		if( f_sigdlg_disp == 0 )
			sigdlg_disp = d_sigdlg_disp;
		if( f_sigdlg_posx == 0 )
			sigdlg_posx = d_sigdlg_posx;
		if( f_sigdlg_posy == 0 )
			sigdlg_posy = d_sigdlg_posy;
		
		//カメラダイアログ
		if( f_camdlg_disp == 0 )
			camdlg_disp = d_camdlg_disp;
		if( f_camdlg_posx == 0 )
			camdlg_posx = d_camdlg_posx;
		if( f_camdlg_posy == 0 )
			camdlg_posy = d_camdlg_posy;
		if( f_camdlg_move == 0 )
			camdlg_move = d_camdlg_move;
		
		//gpa
		if( f_gpa_move == 0 )
			gpa_move = d_gpa_move;

		//graphwnd
		if( f_graph_disp == 0 )
			graph_disp = d_graph_disp;

		if( f_usekinect == 0 )
			usekinect = d_usekinect;


		//背景
		if( f_bg_color == 0 ){ 
//DbgOut( "inidata : SetDefaultValue : bg_color %d %d %d\n", GetRValue( bg_color ), GetGValue( bg_color ), GetBValue( bg_color ) );
//DbgOut( "inidata : SetDefaultValue : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );
			bg_color = dd_bg_color;
		}
		if( f_bg_tex1 == 0 ){
			if( *d_bg_tex1 )
				MoveMemory( bg_tex1, d_bg_tex1, strlen( d_bg_tex1 ) );
			else
				*bg_tex1 = 0;
		}
		if( f_bg_tex2 == 0 ){
			if( *d_bg_tex2 )
				MoveMemory( bg_tex2, d_bg_tex2, strlen( d_bg_tex2 ) );
			else
				*bg_tex2 = 0;
		}
		if( f_bg_mvu == 0 )
			bg_mvu = d_bg_mvu;
		if( f_bg_mvv == 0 )
			bg_mvv = d_bg_mvv;
		if( f_bg_fixsize == 0 )
			bg_fixsize = d_bg_fixsize;

		//平行光源
		if( f_dirlight_col == 0 )
			dirlight_col = d_dirlight_col;

		//オプション
		if( f_opt_jointdisp == 0 )
			opt_jointdisp = d_opt_jointdisp;
		if( f_opt_polydisp == 0 )
			opt_polydisp = d_opt_polydisp;
		if( f_opt_gdisp == 0 )
			opt_gdisp = d_opt_gdisp;
		if( f_opt_namequa == 0 )
			opt_namequa = d_opt_namequa;

		if( f_opt_jointsize == 0 )
			opt_jointsize = d_opt_jointsize;

		if( f_opt_jointalpha == 0 )
			opt_jointalpha = d_opt_jointalpha;

		if( f_opt_undodlg == 0 )
			opt_undodlg = d_opt_undodlg;
		if( f_opt_rdlg == 0 )
			opt_rdlg = d_opt_rdlg;
		if( f_opt_infdlg == 0 )
			opt_infdlg = d_opt_infdlg;
		if( f_opt_modlg == 0 )
			opt_modlg = d_opt_modlg;

		if( f_opt_bmdisp == 0 )
			opt_bmdisp = d_opt_bmdisp;
		
		if( f_opt_previewfps == 0 )
			opt_previewfps = d_opt_previewfps;

		if( f_opt_motstep == 0 )
			opt_motstep = d_opt_motstep;

		if( f_opt_ikfk == 0 )
			opt_ikfk = d_opt_ikfk;

		if( f_opt_iktdisp == 0 )
			opt_iktdisp = d_opt_iktdisp;

		if( f_opt_MouseL == 0 )
			opt_MouseL = d_opt_MouseL;
		if( f_opt_MouseR == 0 )
			opt_MouseR = d_opt_MouseR;
		if( f_opt_MouseW == 0 )
			opt_MouseW = d_opt_MouseW;


		if( f_opt_unselline == 0 )
			opt_unselline = d_opt_unselline;
		if( f_opt_selline == 0 )
			opt_selline = d_opt_selline;
	
		if( f_opt_unselpoint == 0 )
			opt_unselpoint = d_opt_unselpoint;
		if( f_opt_selpoint == 0 )
			opt_selpoint = d_opt_selpoint;

		//customcolor
		int ccno;
		for( ccno = 0; ccno < 16; ccno++ ){
			if( f_cc[ccno] == 0 ){
				cc[ccno] = d_cc[ccno];
			}
		}

		//boneline
		if( f_selectcol == 0 )
			selectcol = d_selectcol;
		if( f_unselectcol == 0 )
			unselectcol = d_unselectcol;

		//viewparam
		int vpno;
		for( vpno = 0; vpno < 4; vpno++ ){
			if( f_viewparam[vpno] == 0 )
				viewparam[vpno] = d_viewparam[vpno];
		}

		if( f_iklevelope == 0 ){
			iklevelope = d_iklevelope;
		}

		if( f_forbidDD == 0 ){
			forbidDD = d_forbidDD;
		}

		if( f_mqo_bonetype == 0 ){
			mqo_bonetype = d_mqo_bonetype;
		}
		if( f_mqo_mult == 0 ){
			mqo_mult = d_mqo_mult;
		}
		if( f_toonedgecol == 0 ){
			toonedgecol = d_toonedgecol;
		}
		if( f_toonedgew == 0 ){
			toonedgew = d_toonedgew;
		}
	}else{
		index32bit = d_index32bit;
		//メインウインドウ
		main_posx = d_main_posx;
		main_posy = d_main_posy;
		main_width = d_main_width;
		main_height = d_main_height;

		//モーションダイアログ
		motdlg_disp = d_motdlg_disp;
		motdlg_posx = d_motdlg_posx;
		motdlg_posy = d_motdlg_posy;
		motdlg_calcsp = d_motdlg_calcsp;
		motdlg_height = d_motdlg_height;

		//階層構造エディタ
		sigdlg_disp = d_sigdlg_disp;
		sigdlg_posx = d_sigdlg_posx;
		sigdlg_posy = d_sigdlg_posy;
		
		//カメラダイアログ
		camdlg_disp = d_camdlg_disp;
		camdlg_posx = d_camdlg_posx;
		camdlg_posy = d_camdlg_posy;
		camdlg_move = d_camdlg_move;

		//gpa
		gpa_move = d_gpa_move;

		//graphwnd
		graph_disp = d_graph_disp;

		usekinect = d_usekinect;

		//背景
//DbgOut( "inidata : SetDefaultValue 1 : bg_color %d %d %d\n", GetRValue( bg_color ), GetGValue( bg_color ), GetBValue( bg_color ) );
//DbgOut( "inidata : SetDefaultValue 1 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );
		bg_color = dd_bg_color;

		if( *d_bg_tex1 )
			MoveMemory( bg_tex1, d_bg_tex1, strlen( d_bg_tex1 ) );
		else
			*bg_tex1 = 0;
		if( *d_bg_tex2 )
			MoveMemory( bg_tex2, d_bg_tex2, strlen( d_bg_tex2 ) );
		else
			*bg_tex2 = 0;
		bg_mvu = d_bg_mvu;
		bg_mvv = d_bg_mvv;
		bg_fixsize = d_bg_fixsize;

		//平行光源
		dirlight_col = d_dirlight_col;

		//オプション
		opt_jointdisp = d_opt_jointdisp;
		opt_polydisp = d_opt_polydisp;
		opt_gdisp = d_opt_gdisp;
		opt_namequa = d_opt_namequa;

		opt_jointsize = d_opt_jointsize;
		opt_jointalpha = d_opt_jointalpha;
		opt_undodlg = d_opt_undodlg;
		opt_rdlg = d_opt_rdlg;
		opt_infdlg = d_opt_infdlg;
		opt_modlg = d_opt_modlg;

		opt_bmdisp = d_opt_bmdisp;
		opt_previewfps = d_opt_previewfps;
		opt_motstep = d_opt_motstep;
		opt_ikfk = d_opt_ikfk;
		opt_iktdisp = d_opt_iktdisp;

		opt_MouseL = d_opt_MouseL;
		opt_MouseR = d_opt_MouseR;
		opt_MouseW = d_opt_MouseW;


		opt_unselline = d_opt_unselline;
		opt_selline = d_opt_selline;
		opt_unselpoint = d_opt_unselpoint;
		opt_selpoint = d_opt_selpoint;
		opt_pointsize = d_opt_pointsize;

		//customcolor
		int ccno;
		for( ccno = 0; ccno < 16; ccno++ ){
			cc[ccno] = d_cc[ccno];
		}

		//boneline
		selectcol = d_selectcol;
		unselectcol = d_unselectcol;

		//viewparam
		//viewparam
		int vpno;
		for( vpno = 0; vpno < 4; vpno++ ){
			viewparam[vpno] = d_viewparam[vpno];
		}

		iklevelope = d_iklevelope;
		forbidDD = d_forbidDD;

		mqo_bonetype = d_mqo_bonetype;
		mqo_mult = d_mqo_mult;

		toonedgecol = d_toonedgecol;
		toonedgew = d_toonedgew;

	}

	return 0;
}


int CIniData::DestroyObjs()
{

	if( m_hfile != INVALID_HANDLE_VALUE ){
		CloseHandle( m_hfile );
		m_hfile = INVALID_HANDLE_VALUE;
	}

	if( m_buf ){
		free( m_buf );
		m_buf = 0;
	}

	m_pos = 0;
	m_bufleng = 0;

	return 0;
}


int CIniData::LoadParams( char* filename )
{
	int ret;
	int errorflag = 0;

//DbgOut( "inidata : LoadParams 0 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );

	DestroyObjs();
	InitParams();

//DbgOut( "inidata : LoadParams 1 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );

	ret = SetBuffer( filename );

//DbgOut( "inidata : LoadParams 2 : dd_bg_color %d %d %d\n", GetRValue( dd_bg_color ), GetGValue( dd_bg_color ), GetBValue( dd_bg_color ) );

	if( ret == 0 ){
		int ret1;

		ret1 = CheckFileHeader();
		if( ret1 != 0 ){
			errorflag++;
			_ASSERT( 0 );
			goto inildparamsexit;
		}

		HWND hWnd;
		RECT rect;
		hWnd = ::GetDesktopWindow();
		GetWindowRect(hWnd, &rect);
		int maxwinw;
		int maxwinh;
		maxwinw = rect.right - rect.left;
		maxwinh = rect.bottom - rect.top;

		GetIntParam( "index32bit", &index32bit, 0, 1, &f_index32bit );
		DbgOut( "inidata : index32bit %d %d\n", index32bit, f_index32bit );
		g_index32bit = index32bit;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		GetIntParam( "main_posx", &main_posx, 0, maxwinw, &f_main_posx );
		DbgOut( "inidata : main_posx %d %d\n", main_posx, f_main_posx );
		GetIntParam( "main_posy", &main_posy, 0, maxwinh, &f_main_posy );
		DbgOut( "inidata : main_posy %d %d\n", main_posy, f_main_posy );
		GetIntParam( "main_width", &main_width, 570, maxwinw, &f_main_width );
		DbgOut( "inidata : main_width %d %d\n", main_width, f_main_width );
		GetIntParam( "main_height", &main_height, 200, maxwinh, &f_main_height );
		DbgOut( "inidata : main_height %d %d\n", main_height, f_main_height );

		GetIntParam( "motdlg_disp", &motdlg_disp, 0, 1, &f_motdlg_disp );
//motdlg_disp = 1;//!!!!!!!!!!!!!
		DbgOut( "inidata : motdlg_disp %d %d\n", motdlg_disp, f_motdlg_disp );
		GetIntParam( "motdlg_posx", &motdlg_posx, 0, maxwinw, &f_motdlg_posx );
		DbgOut( "inidata : motdlg_posx %d %d\n", motdlg_posx, f_motdlg_posx );
		GetIntParam( "motdlg_posy", &motdlg_posy, 0, maxwinh, &f_motdlg_posy );
		DbgOut( "inidata : motdlg_posy %d %d\n", motdlg_posy, f_motdlg_posy );
		GetIntParam( "motdlg_calcsp", &motdlg_calcsp, 0, 1, &f_motdlg_calcsp );
		DbgOut( "inidata : motdlg_calcsp %d %d\n", motdlg_calcsp, f_motdlg_calcsp );
		GetIntParam( "motdlg_height", &motdlg_height, 70, 330, &f_motdlg_height );
		DbgOut( "inidata : motdlg_height %d %d\n", motdlg_height, f_motdlg_height );


		GetIntParam( "sigdlg_disp", &sigdlg_disp, 0, 1, &f_sigdlg_disp );
//sigdlg_disp = 1;//!!!!!!!!!!!!!!!
		DbgOut( "inidata : sigdlg_disp %d %d\n", sigdlg_disp, f_sigdlg_disp );
		GetIntParam( "sigdlg_posx", &sigdlg_posx, 0, maxwinw, &f_sigdlg_posx );
		DbgOut( "inidata : sigdlg_posx %d %d\n", sigdlg_posx, f_sigdlg_posx );
		GetIntParam( "sigdlg_posy", &sigdlg_posy, 0, maxwinh, &f_sigdlg_posy );
		DbgOut( "inidata : sigdlg_posy %d %d\n", sigdlg_posy, f_sigdlg_posy );

		GetIntParam( "camdlg_disp", &camdlg_disp, 0, 1, &f_camdlg_disp );
		DbgOut( "inidata : camdlg_disp %d %d\n", camdlg_disp, f_camdlg_disp );
		GetIntParam( "camdlg_posx", &camdlg_posx, 0, maxwinw, &f_camdlg_posx );
		DbgOut( "inidata : camdlg_posx %d %d\n", camdlg_posx, f_camdlg_posx );
		GetIntParam( "camdlg_posy", &camdlg_posy, 0, maxwinh, &f_camdlg_posy );
		DbgOut( "inidata : camdlg_posy %d %d\n", camdlg_posy, f_camdlg_posy );
		GetFloatParam( "camdlg_move", &camdlg_move, 1.0f, 10000.0f, &f_camdlg_move );
		DbgOut( "inidata : camdlg_move %f %d\n", camdlg_move, f_camdlg_move );


		GetFloatParam( "gpa_move", &gpa_move, 1.0f, 10000.0f, &f_gpa_move );
		DbgOut( "inidata : gpa_move %f %d\n", gpa_move, f_gpa_move );


		GetIntParam( "graph_disp", &graph_disp, 0, 1, &f_graph_disp );
//graph_disp = 1;//!!!!!!!!!!!!!!!!!!
		DbgOut( "inidata : graph_disp %d %d\n", graph_disp, f_graph_disp );

		GetIntParam( "usekinect", &usekinect, 0, 1, &f_usekinect );
		DbgOut( "inidata : usekinect %d %d\n", usekinect, f_usekinect );


		GetColorParam( "bg_color", &bg_color, &f_bg_color );////////
		DbgOut( "inidata : bg_color ( %d, %d, %d )  %d\n", GetRValue( bg_color ), GetGValue( bg_color ), GetBValue( bg_color ), f_bg_color );
		GetNameParam( "bg_tex1", bg_tex1, _MAX_PATH, &f_bg_tex1 );
		DbgOut( "inidata : bg_tex1 %s %d\n", bg_tex1, f_bg_tex1 );
		GetNameParam( "bg_tex2", bg_tex2, _MAX_PATH, &f_bg_tex2 );
		DbgOut( "inidata : bg_tex2 %s %d\n", bg_tex2, f_bg_tex2 );
		GetFloatParam( "bg_mvu", &bg_mvu, 0.0f, 1.0f, &f_bg_mvu );
		DbgOut( "inidata : bg_mvu %f %d\n", bg_mvu, f_bg_mvu );
		GetFloatParam( "bg_mvv", &bg_mvv, 0.0f, 1.0f, &f_bg_mvv );
		DbgOut( "inidata : bg_mvv %f %d\n", bg_mvv, f_bg_mvv );
		GetIntParam( "bg_fixsize", &bg_fixsize, 0, 1, &f_bg_fixsize );
		DbgOut( "inidata : bg_fixsize %d %d\n", bg_fixsize, f_bg_fixsize );


		GetColorParam( "dirlight_col", &dirlight_col, &f_dirlight_col );
		DbgOut( "inidata : dirlight_col ( %d, %d, %d )  %d\n", GetRValue( dirlight_col ), GetGValue( dirlight_col ), GetBValue( dirlight_col ), f_dirlight_col );


		GetIntParam( "opt_jointdisp", &opt_jointdisp, 0, 1, &f_opt_jointdisp );
		DbgOut( "inidata : opt_jointdisp : %d %d\n", opt_jointdisp, f_opt_jointdisp );
		GetIntParam( "opt_polydisp", &opt_polydisp, 0, 2, &f_opt_polydisp );//0--->2
		DbgOut( "inidata : opt_polydisp : %d %d\n", opt_polydisp, f_opt_polydisp );
		GetIntParam( "opt_gdisp", &opt_gdisp, 0, 1, &f_opt_gdisp );
		DbgOut( "inidata : opt_gdisp : %d %d\n", opt_gdisp, f_opt_gdisp );
		GetIntParam( "opt_namequa", &opt_namequa, 0, 1, &f_opt_namequa );
		DbgOut( "inidata : opt_namequa : %d %d\n", opt_namequa, f_opt_namequa );

		GetIntParam( "opt_jointsize", &opt_jointsize, 5, 100, &f_opt_jointsize );
		DbgOut( "inidata : opt_jointsize : %d %d\n", opt_jointsize, f_opt_jointsize );
		GetIntParam( "opt_jointalpha", &opt_jointalpha, 0, 255, &f_opt_jointalpha );
		DbgOut( "inidata : opt_jointalpha : %d %d\n", opt_jointalpha, f_opt_jointalpha );
		GetIntParam( "opt_UNDOdlg", &opt_undodlg, CHKDLG_NES, CHKDLG_YES, &f_opt_undodlg );
		DbgOut( "inidata : opt_undodlg : %d %d\n", opt_undodlg, f_opt_undodlg );
		GetIntParam( "opt_Rdlg", &opt_rdlg, CHKDLG_NES, CHKDLG_YES, &f_opt_rdlg );
		DbgOut( "inidata : opt_Rdlg : %d %d\n", opt_rdlg, f_opt_rdlg );
		GetIntParam( "opt_INFdlg", &opt_infdlg, CHKDLG_NES, CHKDLG_NO, &f_opt_infdlg );
		DbgOut( "inidata : opt_infdlg : %d %d\n", opt_infdlg, f_opt_infdlg );
		GetIntParam( "opt_MOdlg", &opt_modlg, CHKDLG_NES, CHKDLG_NO, &f_opt_modlg );
		DbgOut( "inidata : opt_modlg : %d %d\n", opt_modlg, f_opt_modlg );

		GetIntParam( "opt_bmdisp", &opt_bmdisp, 0, 1, &f_opt_bmdisp );
		DbgOut( "inidata : opt_bmdisp : %d %d\n", opt_bmdisp, f_opt_bmdisp );

		GetIntParam( "opt_previewfps", &opt_previewfps, 5, 200, &f_opt_previewfps );
		DbgOut( "inidata : opt_previewfps : %d %d\n", opt_previewfps, f_opt_previewfps );
	
		GetIntParam( "opt_motstep", &opt_motstep, 1, 4, &f_opt_motstep );
		DbgOut( "inidata : opt_motstep : %d %d\n", opt_motstep, f_opt_motstep );

		GetIntParam( "opt_ikfk", &opt_ikfk, 0, 1, &f_opt_ikfk );
		DbgOut( "inidata : opt_ikfk : %d %d\n", opt_ikfk, f_opt_ikfk );

		GetIntParam( "opt_iktdisp", &opt_iktdisp, 0, 1, &f_opt_iktdisp );
		DbgOut( "inidata : opt_iktdisp : %d %d\n", opt_iktdisp, f_opt_iktdisp );

		GetIntParam( "opt_MouseL", &opt_MouseL, MOUSEOPE_NONE, MOUSEOPE_MAX - 1, &f_opt_MouseL );
		DbgOut( "inidata : optMouseL : %d %d\n", opt_MouseL, f_opt_MouseL );

		GetIntParam( "opt_MouseR", &opt_MouseR, MOUSEOPE_NONE, MOUSEOPE_MAX - 1, &f_opt_MouseR );
		DbgOut( "inidata : optMouseR : %d %d\n", opt_MouseR, f_opt_MouseR );

		GetIntParam( "opt_MouseW", &opt_MouseW, MOUSEOPE_NONE, MOUSEOPE_MAX - 1, &f_opt_MouseW );
		DbgOut( "inidata : optMouseW : %d %d\n", opt_MouseW, f_opt_MouseW );

		GetColorParam( "opt_unselline", &opt_unselline, &f_opt_unselline );
		DbgOut( "inidata : opt_unselline : (%d %d %d) %d\n", GetRValue( opt_unselline ), GetGValue( opt_unselline ), GetBValue( opt_unselline ), f_opt_unselline ); 

		GetColorParam( "opt_selline", &opt_selline, &f_opt_selline );
		DbgOut( "inidata : opt_seline : (%d %d %d) %d\n", GetRValue( opt_selline ), GetGValue( opt_selline ), GetBValue( opt_selline ), f_opt_selline ); 

		GetColorParam( "opt_unselpoint", &opt_unselpoint, &f_opt_unselpoint );
		DbgOut( "inidata : opt_unsepoint : (%d %d %d) %d\n", GetRValue( opt_unselpoint ), GetGValue( opt_unselpoint ), GetBValue( opt_unselpoint ), f_opt_unselpoint ); 

		GetColorParam( "opt_selpoint", &opt_selpoint, &f_opt_selpoint );
		DbgOut( "inidata : opt_selpoint : (%d %d %d) %d\n", GetRValue( opt_selpoint ), GetGValue( opt_selpoint ), GetBValue( opt_selpoint ), f_opt_selpoint ); 

		GetIntParam( "opt_pointsize", &opt_pointsize, 0, 100, &f_opt_pointsize );
		DbgOut( "inidata : opt_pointsize : %d %d\n", opt_pointsize, f_opt_pointsize );


		char colname[256];
		int ccno;
		for( ccno = 0; ccno < 16; ccno++ ){
			sprintf_s( colname, 256, "cc%d", ccno );

			GetColorParam( colname, &(cc[ccno]), &(f_cc[ccno] ) );
			DbgOut( "inidata : %s ( %d, %d, %d ) %d\n", colname, GetRValue( cc[ccno] ), GetGValue( cc[ccno] ), GetBValue( cc[ccno] ), f_cc[ccno] );
		}

		GetColorParam( "selectcol", &selectcol, &f_selectcol );
		DbgOut( "inidata : selectcol ( %d, %d, %d ) %d\n", GetRValue( selectcol ), GetGValue( selectcol ), GetBValue( selectcol ), f_selectcol );

		GetColorParam( "unselectcol", &unselectcol, &f_unselectcol );
		DbgOut( "inidata : unselectcol ( %d, %d, %d ) %d\n", GetRValue( unselectcol ), GetGValue( unselectcol ), GetBValue( unselectcol ), f_unselectcol );


		char vpname[256];
		int vpno;
		for( vpno = 0; vpno < 4; vpno++ ){
			sprintf_s( vpname, 256, "viewparamF%d", vpno + 5 );

			GetViewParam( vpname, &(viewparam[vpno]), &(f_viewparam[vpno]) );
			DbgOut( "inidata : %s ( %f, %f, %f, %f, %f, %f, %f ) %d\n",
				vpname, viewparam[vpno].degxz, viewparam[vpno].degy, viewparam[vpno].eye_y, viewparam[vpno].camdist, 
				viewparam[vpno].targetpos.x, viewparam[vpno].targetpos.y, viewparam[vpno].targetpos.z,
				f_viewparam[vpno]
				);
		}

		GetIntParam( "iklevelope", &iklevelope, 1, 255, &f_iklevelope );
		DbgOut( "inidata : iklevelope : %d %d\n", iklevelope, f_iklevelope );
		
		GetIntParam( "forbidDD", &forbidDD, 0, 1, &f_forbidDD );
		DbgOut( "inidata : forbidDD : %d %d\n", forbidDD, f_forbidDD );

		GetIntParam( "mqo_bonetype", &mqo_bonetype, BONETYPE_RDB2, BONETYPE_MIKO, &f_mqo_bonetype );
		DbgOut( "inidata : mqo_bonetype : %d %d\n", mqo_bonetype, f_mqo_bonetype );

		GetFloatParam( "mqo_mult", &mqo_mult, 0.0f, 1000000.0f, &f_mqo_mult );
		DbgOut( "inidata : mqo_mult : %d %d\n", mqo_mult, f_mqo_mult );

		GetFloatParam( "toonedgecol", &toonedgecol, 0.0f, 255.0f, &f_toonedgecol );
		DbgOut( "inidata : toonedgecol : %f %d\n", toonedgecol, f_toonedgecol );

		GetFloatParam( "toonedgew", &toonedgew, 0.0f, 1000.0f, &f_toonedgew );
		DbgOut( "inidata : toonedgew : %f %d\n", toonedgew, f_toonedgew );


		//SetDefaultValue( 1 );

	}else{
		errorflag++;
		DbgOut( "inidata : LoadParams : SetBuffer error !!!\n" );
		_ASSERT( 0 );
	}

	goto inildparamsexit;

inildparamsexit:
	
	ret = SetDefaultValue( 0 );
	if( ret ){
		DbgOut( "inidata : LoadParams : SetDefaultValue error !!!\n" );
		_ASSERT( 0 );
	}

	DestroyObjs();

	return 0;
}

int CIniData::SetBuffer( char* filename )
{
	DestroyObjs();

	char moduledir[2048];
	int leng;
	ZeroMemory( moduledir, 2048 );
	leng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)moduledir, 2048 );
	if( (leng <= 0) || (leng >= 2048) ){
		DbgOut( "inidata : SetBuffer : GetEnvironmentVariable error %d !!!\n", leng );
		_ASSERT( 0 );
		return 1;
	}

	char outname[2048];
	ZeroMemory( outname, 2048 );

	int chkleng;
	chkleng = leng + (int)strlen( filename );

	if( chkleng >= 2048 ){
		DbgOut( "inidata : SetBuffer : chkleng error !!! file name too long !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	strcpy_s( outname, 2048, moduledir );
	strcat_s( outname, 2048, filename );


	m_hfile = CreateFile( (LPCTSTR)outname, GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( m_hfile == INVALID_HANDLE_VALUE ){
		_ASSERT( 0 );
		return 1;
	}	

	DWORD sizehigh;
	DWORD bufleng;
	bufleng = GetFileSize( m_hfile, &sizehigh );
	if( bufleng < 0 ){
		DbgOut( "inidata : SetBuffer :  GetFileSize error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	if( sizehigh != 0 ){
		DbgOut( "inidata : SetBuffer :  file size too large error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	char* newbuf;
	newbuf = (char*)malloc( sizeof( char ) * bufleng + 1 );//bufleng + 1
	if( !newbuf ){
		DbgOut( "inidata : SetBuffer :  newbuf alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	DWORD rleng, readleng;
	rleng = bufleng;
	ReadFile( m_hfile, (void*)newbuf, rleng, &readleng, NULL );
	if( rleng != readleng ){
		DbgOut( "inidata : SetBuffer :  ReadFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	m_buf = newbuf;
	*(m_buf + bufleng) = 0;//!!!!!!!!
	m_bufleng = bufleng;
	m_pos = 0;	

	return 0;
}

int CIniData::CheckFileHeader()
{
	if( !m_buf || (m_bufleng == 0) ){
		_ASSERT( 0 );
		return 1;
	}

	char srcheader[256];
	strcpy_s( srcheader, 256, "rokdebone2 ini file ver1001_" );

	DWORD headerlen;
	headerlen = (DWORD)strlen( srcheader );

	if( m_bufleng <= headerlen ){
		DbgOut( "inidata : CheckFileHeader : error 0 !!!\n" );
		return 1;
	}else{
		int cmp;
		cmp = strncmp( m_buf, srcheader, headerlen );
		if( cmp != 0 ){
			DbgOut( "inidata : CheckFileHeader : invalid ini file error !!!\n" );
			return 1;
		}else{
			return 0;
		}
	}

	return 0;
}


int CIniData::GetIntParam( char* strtype, int* dstint, int minint, int maxint, int* dstflag )
{
	char* typehead;
	typehead = strstr( m_buf, strtype );
	if( typehead != NULL ){
		int srcleng;
		srcleng = (int)strlen( strtype );

		char* valuehead;
		valuehead = typehead + srcleng;
		m_pos = (DWORD)(typehead - m_buf);

		int setflag = 0;
		int tempint = minint - 1;//!!!!!!
		valuehead = GetDigit( valuehead, &tempint, &setflag );

		/***
		//先頭の空白をスキップ
		while( (m_pos < m_bufleng) && isspace( *valuehead ) ){
			valuehead++;
			m_pos++;
		}

		char* valueend = valuehead;
		//valueの終わりをサーチ
		int valueleng = 0;
		while( (m_pos < m_bufleng) && ( isdigit( *valueend ) || (*valueend == '-') ) ){
			valueend++;
			m_pos++;
			valueleng++;
		}


		//char ---> int
		if( valueleng >= 255 ){
			_ASSERT( 0 );
			*dstflag = 0;//
			return 1;
		}
		char tmpchar[256];
		strncpy( tmpchar, valuehead, valueleng );
		tmpchar[valueleng] = 0;

		int tempint;
		tempint = atoi( tmpchar );
		***/

		//check min max
		if( (setflag == 1) && (tempint >= minint) && (tempint <= maxint) ){

			//setvalue
			*dstint = tempint;

			//setflag
			*dstflag = 1;//

		}else{
			DbgOut( "inidata : GetIntParam : value is out of range warning %d %d!!! : set default value\n", tempint, setflag );
			*dstflag = 0;//
		}

	}else{
		*dstflag = 0;//
		return 1;
	}

	return 0;
}

int CIniData::GetFloatParam( char* strtype, float* dstfl, float minfl, float maxfl, int* dstflag )
{
	char* typehead;
	typehead = strstr( m_buf, strtype );
	if( typehead != NULL ){
		int srcleng;
		srcleng = (int)strlen( strtype );

		char* valuehead;
		valuehead = typehead + srcleng;
		m_pos = (DWORD)(typehead - m_buf);

		//先頭の空白をスキップ
		while( (m_pos < m_bufleng) && isspace( *valuehead ) ){
			valuehead++;
			m_pos++;
		}

		char* valueend = valuehead;
		//valueの終わりをサーチ
		int valueleng = 0;
		char conma = '.';
		char minus = '-';
		while( (m_pos < m_bufleng) && ( isdigit( *valueend ) || (*valueend == conma) || (*valueend == minus) ) ){
			valueend++;
			m_pos++;
			valueleng++;
		}

		//char ---> float
		if( valueleng >= 255 ){
			_ASSERT( 0 );
			*dstflag = 0;//
			return 1;
		}
		char tmpchar[256];
		strncpy_s( tmpchar, 256, valuehead, valueleng );
		tmpchar[valueleng] = 0;

		float tempfloat;
		tempfloat = (float)atof( tmpchar );

		//check min max
		if( (tempfloat >= minfl) && (tempfloat <= maxfl) ){

			//setvalue
			*dstfl = tempfloat;

			//setflag
			*dstflag = 1;//

		}else{
			DbgOut( "inidata : GetFloatParam : value is out of range warning %f !!! : set default value\n", tempfloat );
			*dstflag = 0;//
		}

	}else{
		*dstflag = 0;//
		return 1;
	}

	return 0;
}

int CIniData::GetNameParam( char* strtype, char* dstchar, int maxleng, int* dstflag )
{
	char* typehead;
	typehead = strstr( m_buf, strtype );
	if( typehead != NULL ){
		int srcleng;
		srcleng = (int)strlen( strtype );


		char* valuehead;
		//開始の"を検索
		valuehead = strstr( typehead, "\"" );
		if( valuehead == NULL ){
			*dstflag = 0;
			return 1;
		}

		char* valueend;
		//終了の"を検索
		valueend = strstr( valuehead + 1, "\"" );
		if( valueend == NULL ){
			*dstflag = 0;
			return 1;
		}

		int valueleng;
		valueleng = (int)( valueend - (valuehead + 1) );

		if( valueleng >= maxleng ){
			*dstflag = 0;
			return 1;
		}

		strncpy_s( dstchar, maxleng, valuehead + 1, valueleng );
		*( dstchar + valueleng ) = 0;

		*dstflag = 1;

	}else{
		*dstflag = 0;//
		return 1;
	}

	return 0;
}
int CIniData::GetViewParam( char* strtype, VIEWPARAM* dstvp, int* dstflag )
{
	char* typehead;
	typehead = strstr( m_buf, strtype );
	if( typehead != NULL ){
		int srcleng;
		srcleng = (int)strlen( strtype );

		char* valuehead;
		valuehead = typehead + srcleng;
		m_pos = (DWORD)(typehead - m_buf);//!!!


		int setflag;
		valuehead = GetFloat( valuehead, &dstvp->degxz, &setflag );

		if( setflag && valuehead ){
			valuehead = GetFloat( valuehead, &dstvp->degy, &setflag );
		}else{
			*dstflag = 0;
			return 1;
		}
		if( setflag && valuehead ){
			valuehead = GetFloat( valuehead, &dstvp->eye_y, &setflag );
		}else{
			*dstflag = 0;
			return 1;
		}
		if( setflag && valuehead ){
			valuehead = GetFloat( valuehead, &dstvp->camdist, &setflag );
		}else{
			*dstflag = 0;
			return 1;
		}

		if( setflag && valuehead ){
			valuehead = GetFloat( valuehead, &(dstvp->targetpos.x), &setflag );
		}else{
			*dstflag = 0;
			return 1;
		}

		if( setflag && valuehead ){
			valuehead = GetFloat( valuehead, &(dstvp->targetpos.y), &setflag );
		}else{
			*dstflag = 0;
			return 1;
		}

		if( setflag && valuehead ){
			valuehead = GetFloat( valuehead, &(dstvp->targetpos.z), &setflag );
		}else{
			*dstflag = 0;
			return 1;
		}

		*dstflag = 1;
		return 0;
	}else{
		*dstflag = 0;
		return 1;
	}
}


int CIniData::GetColorParam( char* strtype, COLORREF* dstcol, int* dstflag )
{
	char* typehead;
	typehead = strstr( m_buf, strtype );
	if( typehead != NULL ){
		int srcleng;
		srcleng = (int)strlen( strtype );

		char* valuehead;
		valuehead = typehead + srcleng;
		m_pos = (DWORD)(typehead - m_buf);//!!!


		int colr = -100;
		int colg = -100;
		int colb = -100;

		int setflag;
		valuehead = GetDigit( valuehead, &colr, &setflag );

		if( setflag && valuehead )
			valuehead = GetDigit( valuehead, &colg, &setflag );

		if( setflag && valuehead )
			valuehead = GetDigit( valuehead, &colb, &setflag );

		if( (colr >= 0) && (colr <= 255) ){
			if( (colg >= 0) && (colg <= 255) ){
				if( (colb >= 0) && (colb <= 255) ){
					*dstcol = RGB( colr, colg, colb );
					*dstflag = 1;//!!!!!!!

				}else{
					*dstflag = 0;
					return 1;
				}
			}else{
				*dstflag = 0;
				return 1;
			}
		}else{
			*dstflag = 0;
			return 1;
		}
	}else{
		*dstflag = 0;//
		return 1;
	}

	return 0;
}

char* CIniData::GetFloat( char* srcstr, float* dstfloat, int* dstsetflag )
{
	char* valuehead = srcstr;

	//先頭の非数字をスキップ
	while( (m_pos < m_bufleng) && ( (isdigit( *valuehead ) == 0) && (*valuehead != '-'))  ){
		if( isalpha( *valuehead ) != 0 ){
			//illeagal letter
			DbgOut( "inidata : GetDigit : isalpha error !!!\n" );
			*dstsetflag = 0;
			return 0;//!!!!
		}
		valuehead++;
		m_pos++;
	}

	char* valueend = valuehead;
	//valueの終わりをサーチ
	int valueleng = 0;
	while( (m_pos < m_bufleng) && ( isdigit( *valueend ) || (*valueend == '-') ) || (*valueend == '.') ){
		valueend++;
		m_pos++;
		valueleng++;
	}


	//char ---> float
	if( valueleng >= 255 ){
		_ASSERT( 0 );
		*dstsetflag = 0;//
		return 0;
	}
	char tmpchar[256];
	strncpy_s( tmpchar, 256, valuehead, valueleng );
	tmpchar[valueleng] = 0;

	float tempfloat;
	tempfloat = (float)atof( tmpchar );

	*dstfloat = tempfloat;
	*dstsetflag = 1;//

	return valueend;

}



char* CIniData::GetDigit( char* srcstr, int* dstint, int* dstsetflag )
{

	char* valuehead = srcstr;

	//先頭の非数字をスキップ
	while( (m_pos < m_bufleng) && ( (isdigit( *valuehead ) == 0) && (*valuehead != '-'))  ){
		if( isalpha( *valuehead ) != 0 ){
			//illeagal letter
			DbgOut( "inidata : GetDigit : isalpha error !!!\n" );
			*dstsetflag = 0;
			return 0;//!!!!
		}
		valuehead++;
		m_pos++;
	}

	char* valueend = valuehead;
	//valueの終わりをサーチ
	int valueleng = 0;
	while( (m_pos < m_bufleng) && ( isdigit( *valueend ) || (*valueend == '-') ) ){
		valueend++;
		m_pos++;
		valueleng++;
	}


	//char ---> int
	if( valueleng >= 255 ){
		_ASSERT( 0 );
		*dstsetflag = 0;//
		return 0;
	}
	char tmpchar[256];
	strncpy_s( tmpchar, 256, valuehead, valueleng );
	tmpchar[valueleng] = 0;

	int tempint;
	tempint = atoi( tmpchar );

	*dstint = tempint;
	*dstsetflag = 1;//

	return valueend;
}


int CIniData::SaveParams( char* filename )
{
	DestroyObjs();

	char moduledir[2048];
	int leng;
	ZeroMemory( moduledir, 2048 );
	leng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)moduledir, 2048 );
	if( (leng <= 0) || (leng >= 2048) ){
		DbgOut( "inidata : SaveParams : GetEnvironmentVariable error %d !!!\n", leng );
		_ASSERT( 0 );
		return 1;
	}

	char outname[2048];
	ZeroMemory( outname, 2048 );

	int chkleng;
	chkleng = leng + (int)strlen( filename );

	if( chkleng >= 2048 ){
		DbgOut( "inidata : SaveParams : chkleng error !!! file name too long !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	strcpy_s( outname, 2048, moduledir );
	strcat_s( outname, 2048, filename );


	m_hfile = CreateFile( outname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( m_hfile == INVALID_HANDLE_VALUE ){
		DbgOut( "inidata : SavaParams : file open error %s !!!\n", filename );
		_ASSERT( 0 );
		return 1;
	}

	Write2File( "rokdebone2 ini file ver1001_\r\n" );
	Write2File( "//上のコメントは、編集しないでください。\r\n" );
	Write2File( "\r\n" );
	Write2File( "//編集項目と数値には、全て、半角文字を使用してください。\r\n" );
	Write2File( "//項目名と、数値の間には、半角スペースを入れてください。\r\n" );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "index32bit %d\r\n", index32bit );

	Write2File( "//main window\r\n" );
	Write2File( "main_posx %d\r\n", main_posx );
	Write2File( "main_posy %d\r\n", main_posy );
	Write2File( "main_width %d\r\n", main_width );
	Write2File( "main_height %d\r\n", main_height );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "//motion dialog\r\n" );
	Write2File( "motdlg_disp %d\r\n", motdlg_disp );
	Write2File( "motdlg_posx %d\r\n", motdlg_posx );
	Write2File( "motdlg_posy %d\r\n", motdlg_posy );
	Write2File( "motdlg_calcsp %d\r\n", motdlg_calcsp );
	Write2File( "motdlg_height %d\r\n", motdlg_height );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "//sig params dialog\r\n" );
	Write2File( "sigdlg_disp %d\r\n", sigdlg_disp );
	Write2File( "sigdlg_posx %d\r\n", sigdlg_posx );
	Write2File( "sigdlg_posy %d\r\n", sigdlg_posy );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "//camera dialog\r\n" );
	Write2File( "camdlg_disp %d\r\n", camdlg_disp );
	Write2File( "camdlg_posx %d\r\n", camdlg_posx );
	Write2File( "camdlg_posy %d\r\n", camdlg_posy );
	Write2File( "camdlg_move %f\r\n", camdlg_move );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "//gpa dialog\r\n" );
	Write2File( "gpa_move %f\r\n", gpa_move );
	Write2File( "\r\n" );
	Write2File( "\r\n" );
	

	Write2File( "//graph wnd\r\n" );
	Write2File( "graph_disp %d\r\n", graph_disp );
	Write2File( "\r\n" );

	Write2File( "//kinect option\r\n" );
	Write2File( "usekinect %d\r\n", usekinect );
	Write2File( "\r\n" );


	Write2File( "//back ground\r\n" );
	Write2File( "bg_color ( %d, %d, %d )\r\n", GetRValue( bg_color ), GetGValue( bg_color ), GetBValue( bg_color ) );
	Write2File( "bg_tex1 \"%s\"\r\n", bg_tex1 );
	Write2File( "bg_tex2 \"%s\"\r\n", bg_tex2 );
	Write2File( "bg_mvu %f\r\n", bg_mvu );
	Write2File( "bg_mvv %f\r\n", bg_mvv );
	Write2File( "bg_fixsize %d\r\n", bg_fixsize );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "//directional light\r\n" );
	Write2File( "dirlight_col ( %d, %d, %d )\r\n", GetRValue( dirlight_col ), GetGValue( dirlight_col ), GetBValue( dirlight_col ) );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "//option\r\n" );
	Write2File( "opt_jointdisp %d\r\n", opt_jointdisp );
	Write2File( "opt_polydisp %d\r\n", opt_polydisp );
	Write2File( "opt_gdisp %d\r\n", opt_gdisp );
	Write2File( "opt_namequa %d\r\n", opt_namequa );
	Write2File( "opt_jointsize %d\r\n", opt_jointsize );
	Write2File( "opt_jointalpha %d\r\n", opt_jointalpha );
	Write2File( "opt_UNDOdlg %d\r\n", opt_undodlg );
	Write2File( "opt_Rdlg %d\r\n", opt_rdlg );
	Write2File( "opt_INFdlg %d\r\n", opt_infdlg );
	Write2File( "opt_MOdlg %d\r\n", opt_modlg );

	Write2File( "opt_bmdisp %d\r\n", opt_bmdisp );
	Write2File( "opt_previewfps %d\r\n", opt_previewfps );
	Write2File( "opt_motstep %d\r\n", opt_motstep );
	Write2File( "opt_ikfk %d\r\n", opt_ikfk );
	Write2File( "opt_iktdisp %d\r\n", opt_iktdisp );

	Write2File( "opt_MouseL %d\r\n", opt_MouseL );
	Write2File( "opt_MouseR %d\r\n", opt_MouseR );
	Write2File( "opt_MouseW %d\r\n", opt_MouseW );

	Write2File( "opt_unselline ( %d, %d, %d )\r\n", GetRValue( opt_unselline ), GetGValue( opt_unselline ), GetBValue( opt_unselline ) );
	Write2File( "opt_selline ( %d, %d, %d )\r\n", GetRValue( opt_selline ), GetGValue( opt_selline ), GetBValue( opt_selline ) );
	Write2File( "opt_unselpoint ( %d, %d, %d )\r\n", GetRValue( opt_unselpoint ), GetGValue( opt_unselpoint ), GetBValue( opt_unselpoint ) );
	Write2File( "opt_selpoint ( %d, %d, %d )\r\n", GetRValue( opt_selpoint ), GetGValue( opt_selpoint ), GetBValue( opt_selpoint ) );
	Write2File( "opt_pointsize %d\r\n", opt_pointsize );

	Write2File( "\r\n" );
	Write2File( "\r\n" );


	Write2File( "//custom color\r\n" );
	int ccno;
	char colname[256];
	for( ccno = 0; ccno < 16; ccno++ ){
		sprintf_s( colname, 256, "cc%d", ccno );
		Write2File( "%s ( %d, %d, %d )\r\n", colname, GetRValue( cc[ccno] ), GetGValue( cc[ccno] ), GetBValue( cc[ccno] ) );
	}
	Write2File( "\r\n" );
	Write2File( "\r\n" );


	Write2File( "//bonemark(line) selected color\r\n" );
	Write2File( "selectcol ( %d, %d, %d )\r\n", GetRValue( selectcol ), GetGValue( selectcol ), GetBValue( selectcol ) );
	Write2File( "unselectcol ( %d, %d, %d )\r\n", GetRValue( unselectcol ), GetGValue( unselectcol ), GetBValue( unselectcol ) );
	Write2File( "\r\n" );
	Write2File( "\r\n" );



	Write2File( "//viewparam\r\n" );
	int vpno;
	char vpname[256];
	for( vpno = 0; vpno < 4; vpno++ ){
		sprintf_s( vpname, 256, "viewparamF%d", vpno + 5 );
		Write2File( "%s ( %f, %f, %f, %f, %f, %f, %f )\r\n", 
			vpname,
			viewparam[vpno].degxz, viewparam[vpno].degy, viewparam[vpno].eye_y, viewparam[vpno].camdist,
			viewparam[vpno].targetpos.x, viewparam[vpno].targetpos.y, viewparam[vpno].targetpos.z 
			);
	}
	Write2File( "\r\n" );
	Write2File( "\r\n" );


	Write2File( "iklevelope %d\r\n", iklevelope );
	Write2File( "forbidDD %d\r\n", forbidDD );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "mqo_bonetype %d\r\n", mqo_bonetype );
	Write2File( "mqo_mult %f\r\n", mqo_mult );
	Write2File( "\r\n" );
	Write2File( "\r\n" );

	Write2File( "toonedgecol %f\r\n", toonedgecol );
	Write2File( "toonedgew %f\r\n", toonedgew );
	Write2File( "\r\n" );
	Write2File( "\r\n" );



	Write2File( "//end of file\r\n" );

	
	FlushFileBuffers( m_hfile );
	SetEndOfFile( m_hfile );
	

	DestroyObjs();

	return 0;
}

int CIniData::Write2File( char* lpFormat, ... )
{
	if( !m_hfile ){
		_ASSERT( 0 );
		return 0;
	}

	int ret;
	va_list Marker;
	unsigned long wleng, writeleng;
	char outchar[2048];
			
	ZeroMemory( outchar, 2048 );

	va_start( Marker, lpFormat );
	ret = vsprintf_s( outchar, 2048, lpFormat, Marker );
	va_end( Marker );

	if( ret < 0 )
		return 1;

	wleng = (unsigned long)strlen( outchar );
	WriteFile( m_hfile, outchar, wleng, &writeleng, NULL );
	if( wleng != writeleng ){
		return 1;
	}

	return 0;	
}
