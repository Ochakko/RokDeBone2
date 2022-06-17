#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <Commdlg.h>

#include "../RDBPlugin.h"
#include "../coef_r.h"

#include <crtdbg.h>// <--- _ASSERTマクロ


static HINSTANCE hInstance;
static HANDLE s_hfile = INVALID_HANDLE_VALUE;


static int Write2File( char* lpFormat, ... );
static void CloseFile();

//---------------------------------------------------------------------------
//  DllMain
//---------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	//ダイアログボックスの表示に必要なので、インスタンスを保存しておく
	hInstance = (HINSTANCE)hModule;


	//変数の初期化




	return TRUE;
}


//---------------------------------------------------------------------------
//  RDBGetPlugInID
//    プラグインIDを返す。
//    この関数は起動時に呼び出される。
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT int RDBGetPlugInID(DWORD *Product, DWORD *ID)
{
	// プロダクト名(制作者名)とIDを、全部で64bitの値として返す
	// 値は他と重複しないようなランダムなもので良い

	char* cproduct;
	cproduct = (char*)Product;
	*cproduct = 'o';
	*( cproduct + 1 ) = 'c';
	*( cproduct + 2 ) = 'h';
	*( cproduct + 3 ) = 'i';

	*ID      = 0xDCBA0040;

	return 0;
}


//---------------------------------------------------------------------------
//  RDBGetPlugInName
//    プラグイン名を返す。
//    RokDeBone2のプラグインメニューに、この名前が表示される。
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT const char* RDBGetPlugInName(void)
{
	// プラグイン名
	return "ToonExporter";
}

//---------------------------------------------------------------------------
//  RDBOnClose
//    RokDeBone2が終了する直前にRokDeBone2から呼ばれる。
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT int RDBOnClose(void)
{
	return 0;
}

//---------------------------------------------------------------------------
//  RDBOnPose
//    RokDeBone2でユーザーがボーンの姿勢を編集し、マウスを離したときにRokDeBone2から呼ばれる。
//    モーションポイントは変更されているがFillUpはされていない状態で呼ばれる。
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT int RDBOnPose( int motid )
{
	return 0;
}


//----------------------------------------------------------------------------
//  RDBOnSelectPlugin
//  RokDeBone2のプラグインメニューで、RDBGetPlugInNameの文字列を選択したときに、
//  この関数が、一回、呼ばれます。
//----------------------------------------------------------------------------

RDBPLUGIN_EXPORT int RDBOnSelectPlugin()
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	buf[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"TXT FILE (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="txt";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	s_hfile = CreateFile( buf, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( s_hfile == INVALID_HANDLE_VALUE ){
		RDBDbgOut( "toonexporter : file open error %s !!!\n", buf );
		_ASSERT( 0 );
		return 1;
	}

	int ret = 0;





	int hsid;
	ret = RDBGetCurrentHSID( &hsid );
	if( hsid >= 0 ){
		
		/***
		// set test
		// easy3dに同梱しているmy.sigの操作テスト
		int bodyno = -1;
		ret = RDBGetPartNoByName( hsid, "body", 0, &bodyno );
		_ASSERT( !ret );

		ret = RDBSetToon1Name( hsid, bodyno, "Material2", "hada" );
		_ASSERT( !ret );

		RDBColor3f redcol;
		redcol.r = 1.0f; redcol.g = 0.0f; redcol.b = 0.0f;
		ret = RDBSetToon1Diffuse( hsid, bodyno, "hada", redcol );
		_ASSERT( !ret );

		RDBColor3f gcol;
		gcol.r = 0.0f; gcol.g = 1.0f; gcol.b = 0.0f;
		ret = RDBSetToon1Ambient( hsid, bodyno, "hada", gcol );
		_ASSERT( !ret );

		RDBColor3f bcol;
		bcol.r = 0.0f; bcol.g = 0.0f; bcol.b = 1.0f;
		ret = RDBSetToon1Specular( hsid, bodyno, "hada", bcol );
		_ASSERT( !ret );

		float dnl = (float)cos( 20.0f * DEG2PAI );
		float bnl = (float)cos( 80.0f * DEG2PAI );
		ret = RDBSetToon1NL( hsid, bodyno, "hada", dnl, bnl );
		_ASSERT( !ret );

		RDBColor3f wcol;
		wcol.r = 1.0f; wcol.g = 1.0f; wcol.b = 1.0f;
		ret = RDBSetToon1Edge0( hsid, bodyno, "hada", wcol, 1, 0, 30.0f );
		_ASSERT( !ret );
		***/

		//表示オブジェクトの情報取得
		int dispobjnum;
		ret = RDBGetDispObjNum( hsid, &dispobjnum );
		if( ret ){
			_ASSERT( 0 );
			CloseFile();
			return 1;
		}
		if( dispobjnum == 0 ){
			Write2File( "dispobj not exist return\r\n" );
			ret = 0;
			CloseFile();
			return 0;
		}

		char* dispobjname = (char*)malloc( sizeof( char ) * 256 * dispobjnum );
		if( !dispobjname ){
			_ASSERT( 0 );
			CloseFile();
			return 1;
		}
		int* dispobjinfo = (int*)malloc( sizeof( int ) * DOI_MAX * dispobjnum );
		if( !dispobjinfo ){
			_ASSERT( 0 );
			CloseFile();
			return 1;
		}
		int dispgetnum;
		ret = RDBGetDispObjInfo( hsid, dispobjnum, dispobjname, dispobjinfo, &dispgetnum );
		if( ret ){
			_ASSERT( 0 );
			CloseFile();
			return 1;
		}

		int dispobjno;
		int partno;
		for( dispobjno = 0; dispobjno < dispobjnum; dispobjno++ ){
			partno = *( dispobjinfo + DOI_MAX * dispobjno + DOI_SERIAL );
			_ASSERT( partno > 0 );

			Write2File( "dispobj name : %s\r\n", dispobjname + 256 * dispobjno );

			int matnum = 0;
			ret = RDBGetToon1MaterialNum( hsid, partno, &matnum );
			if( ret ){
				_ASSERT( 0 );
				CloseFile();
				return 1;
			}

			Write2File( "\ttoon1materialnum %d\r\n", matnum );

			if( matnum > 0 ){
				E3DTOON1MATERIAL* toon1ptr;
				toon1ptr = (E3DTOON1MATERIAL*)malloc( sizeof( E3DTOON1MATERIAL ) * matnum );
				if( !toon1ptr ){
					_ASSERT( 0 );
					CloseFile();
					return 1;
				}
				ZeroMemory( toon1ptr, sizeof( E3DTOON1MATERIAL ) * matnum );

				ret = RDBGetToon1Material( hsid, partno, toon1ptr, matnum );
				if( ret ){
					_ASSERT( 0 );
					CloseFile();
					return 1;
				}

				int matno;
				for( matno = 0; matno < matnum; matno++ ){
					E3DTOON1MATERIAL* curtoon1 = toon1ptr + matno;

					Write2File( "\tmaterial name : %s\r\n", curtoon1->name );
					Write2File( "\t\tdiffuse %f %f %f\r\n", curtoon1->diffuse.r, curtoon1->diffuse.g, curtoon1->diffuse.b );
					Write2File( "\t\tambient %f %f %f\r\n", curtoon1->ambient.r, curtoon1->ambient.g, curtoon1->ambient.b );
					Write2File( "\t\tspecular %f %f %f\r\n", curtoon1->specular.r, curtoon1->specular.g, curtoon1->specular.b );

					Write2File( "\t\tdarknl %f, brightnl %f\r\n", curtoon1->darknl, curtoon1->brightnl );

					Write2File( "\t\tedgecol0 %f %f %f\r\n", curtoon1->edgecol0.r, curtoon1->edgecol0.g, curtoon1->edgecol0.b );
					Write2File( "\t\tedgevalid0 %d, edgeinv0 %d, edgewidth0 %f\r\n",
						curtoon1->edgevalid0, curtoon1->edgeinv0, curtoon1->edgewidth0 );
				}

				free( toon1ptr );
			}
		}


		free( dispobjname );
		free( dispobjinfo );

	}else{
		Write2File( "model data is not loaded return !!!\r\n" );
	}

	ret = Write2File( "#EndOfFile\r\n" );
	_ASSERT( !ret );

	CloseFile();

	MessageBox( NULL, "ToonExporterの処理を終わりました。", "MorphExporter", MB_OK );

	return 0;

}

int Write2File( char* lpFormat, ... )
{
	if( s_hfile == INVALID_HANDLE_VALUE ){
		return 0;
	}

	int ret;
	va_list Marker;
	unsigned long wleng, writeleng;
	char outchar[4098];
			
	va_start( Marker, lpFormat );
	ret = vsprintf( outchar, lpFormat, Marker );
	va_end( Marker );

	if( ret < 0 )
		return 1;

	wleng = (unsigned long)strlen( outchar );
	WriteFile( s_hfile, outchar, wleng, &writeleng, NULL );
	if( wleng != writeleng ){
		return 1;
	}

	return 0;	
}

void CloseFile()
{
	if (s_hfile != INVALID_HANDLE_VALUE) {
		FlushFileBuffers(s_hfile);
		SetEndOfFile(s_hfile);
		CloseHandle(s_hfile);
		s_hfile = INVALID_HANDLE_VALUE;
	}
}
