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
static HANDLE hfile = INVALID_HANDLE_VALUE;

static int Write2File( char* lpFormat, ... );


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

	//プラグインとしては特に必要な処理はないので、何もせずにTRUEを返す
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

	*ID      = 0xDCBA0001;

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
	return "ShapeExporter";
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

	hfile = CreateFile( buf, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hfile == INVALID_HANDLE_VALUE ){
		RDBDbgOut( "shapeexporter : file open error %s !!!\n", buf );
		return 1;
	}


	int rdb2version;
	RDBGetVersion( &rdb2version );
	Write2File( "RokDeBone2 version : %d\r\n", rdb2version );


	char* dispobjname = 0;
	int* dispobjinfo = 0;
	

	int ret = 0;
	int hsid;
	ret = RDBGetCurrentHSID( &hsid );
	if( hsid >= 0 ){

		RDBCreateProgressBar();
		RDBSetProgressBar( 5 );


		//マテリアルの情報取得
		int matnum;
		ret = RDBGetMaterialNum( hsid, &matnum );
		if( (ret == 0) && (matnum > 0) ){
			RDBMaterial curmat;
			int matno;
			for( matno = 0; matno < matnum; matno++ ){
				ret = RDBGetMaterial( hsid, matno, &curmat );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}

				Write2File( "material %d - %s - %d\r\n", curmat.materialno, curmat.name, curmat.legacyflag );
				Write2File( "\ta : %f, diffuse %f, %f, %f\r\n", curmat.a, curmat.diffuse.r, curmat.diffuse.g, curmat.diffuse.b );
				Write2File( "\ttexture %s\r\n", curmat.tex );
			}
		}
		Write2File( "\r\n" );


		//表示オブジェクトの情報取得
		int dispobjnum;
		ret = RDBGetDispObjNum( hsid, &dispobjnum );
		if( ret ){
			_ASSERT( 0 );
			goto OnSelectExit;
		}
		if( dispobjnum == 0 ){
			Write2File( "dispobj not exist return\r\n" );
			ret = 0;
			goto OnSelectExit;
		}


		int shadertype, overflowtype;
		ret = RDBGetShaderType( hsid, &shadertype, &overflowtype );
		if( ret ){
			_ASSERT( 0 );
			goto OnSelectExit;
		}
		Write2File( "shadertype %d, overflowtype %d\r\n", shadertype, overflowtype );



		dispobjname = (char*)malloc( sizeof( char ) * 256 * dispobjnum );
		if( !dispobjname ){
			_ASSERT( 0 );
			goto OnSelectExit;
		}
		dispobjinfo = (int*)malloc( sizeof( int ) * DOI_MAX * dispobjnum );
		if( !dispobjinfo ){
			_ASSERT( 0 );
			goto OnSelectExit;
		}
		int dispgetnum;
		ret = RDBGetDispObjInfo( hsid, dispobjnum, dispobjname, dispobjinfo, &dispgetnum );
		if( ret ){
			_ASSERT( 0 );
			goto OnSelectExit;
		}

		int dispobjno;
		int partno;
		for( dispobjno = 0; dispobjno < dispobjnum; dispobjno++ ){

			int pbpos = 0;
			if( dispobjnum != 0 )
				pbpos = (int)( (float)dispobjno * 100.0f / (float)dispobjnum );
			RDBSetProgressBar( pbpos );


			partno = *( dispobjinfo + DOI_MAX * dispobjno + DOI_SERIAL );
			_ASSERT( partno > 0 );

			int vertnum;
			ret = RDBGetVertNumOfPart( hsid, partno, &vertnum );
			if( ret ){
				_ASSERT( 0 );
				goto OnSelectExit;
			}
			
			//名前の出力
			Write2File( "\r\n\r\n\r\ndispobjname %s, partno %d, vertex num %d\r\n", (dispobjname + 256 * dispobjno), partno, vertnum );

			int vertno;
			for( vertno = 0; vertno < vertnum; vertno++ ){

				Write2File( "vertexno %d\r\n", vertno );

				//頂点位置の出力
				RDBPoint vpos;
				ret = RDBGetVertPos( hsid, partno, vertno, &vpos );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				Write2File( "position ( %f, %f, %f )\r\n", vpos.x, vpos.y, vpos.z );


				//頂点の色の出力
				int alpha;
				ret = RDBGetAlpha( hsid, partno, vertno, &alpha );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				int dr, dg, db;
				ret = RDBGetDiffuse( hsid, partno, vertno, &dr, &dg, &db );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				int ar, ag, ab;
				ret = RDBGetAmbient( hsid, partno, vertno, &ar, &ag, &ab );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				int sr, sg, sb;
				ret = RDBGetSpecular( hsid, partno, vertno, &sr, &sg, &sb );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				int er, eg, eb;
				ret = RDBGetEmissive( hsid, partno, vertno, &er, &eg, &eb );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				float power;
				ret = RDBGetSpecularPower( hsid, partno, vertno, &power );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				Write2File( "alpha %d\r\n", alpha );
				Write2File( "diffuse (%d %d %d)\r\n", dr, dg, db );
				Write2File( "ambient (%d %d %d)\r\n", ar, ag, ab );
				Write2File( "specular (%d %d %d)\r\n", sr, sg, sb );
				Write2File( "emissive (%d %d %d)\r\n", er, eg, eb );
				Write2File( "specular power %f\r\n", power );

				//影響度の出力
				int infnum;
				ret = RDBGetInfElemNum( hsid, partno, vertno, &infnum );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				int infno;
				for( infno = 0; infno < infnum; infno++ ){
					int boneno;
					float rate;
					ret = RDBGetInfElem( hsid, partno, vertno, infno, &boneno, &rate );
					if( ret ){
						_ASSERT( 0 );
						goto OnSelectExit;
					}
					
					//!!!!!!!! boneno には、-1が、代入されることがある。
					// boneno が　-1のときは、無意味なデータであることに注意。

					if( boneno >= 0 ){
						char jointname[2048];//大きめにとってください。
						ret = RDBGetName( hsid, boneno, jointname, 2047 );
						if( ret ){
							_ASSERT( 0 );
							goto OnSelectExit;
						}
						Write2File( "infelem : boneno %d, bonename %s, rate %f\r\n", boneno, jointname, rate );
					}else{
						Write2File( "infelem : boneno %d  invalid data\r\n", boneno );
					}
				}


				//UV情報の出力
				float u, v;
				ret = RDBGetUV( hsid, partno, vertno, &u, &v );
				if( ret ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}
				Write2File( "texture U : %f, V %f\r\n", u, v );
			}

			Write2File( "\r\n\r\n" );

			//法線情報の出力
			if( vertnum > 0 ){
				RDBPoint* normalptr;
				normalptr = (RDBPoint*)malloc( sizeof( RDBPoint ) * vertnum );
				if( !normalptr ){
					_ASSERT( 0 );
					goto OnSelectExit;
				}

				//初期状態の法線は、motidに-1を指定して取得する
				ret = RDBGetNormal( hsid, partno, -1, 0, normalptr, vertnum );
				if( ret ){
					_ASSERT( 0 );
					free( normalptr );
					goto OnSelectExit;
				}

				int vno;
				for( vno = 0; vno < vertnum; vno++ ){
					RDBPoint* curn;
					curn = normalptr + vno;
					Write2File( "normal : vert %d,  ( %f, %f, %f )\r\n", vno, curn->x, curn->y, curn->z );
				}
					
				free( normalptr );
			}


			Write2File( "\r\n\r\n" );


////////////////////

			//面情報の出力
			int facenum;
			ret = RDBGetFaceNum( hsid, partno, &facenum );
			if( ret ){
				_ASSERT( 0 );
				goto OnSelectExit;
			}

			int* ibuf = 0;
			int intnum, setintnum, revflag;
			ret = RDBGetTriIndicesInt( hsid, partno, ibuf, 0, &setintnum, &revflag );
			if( ret ){
				_ASSERT( 0 );
				goto OnSelectExit;
			}
			ibuf = (int*)malloc( sizeof( int ) * setintnum );
			if( !ibuf ){
				_ASSERT( 0 );
				goto OnSelectExit;
			}

			intnum = setintnum;
			ret = RDBGetTriIndicesInt( hsid, partno, ibuf, intnum, &setintnum, &revflag );
			if( ret ){
				_ASSERT( 0 );
				free( ibuf );
				goto OnSelectExit;
			}

			if( setintnum != (facenum * 3) ){
				_ASSERT( 0 );
				free( ibuf );
				goto OnSelectExit;
			}

			Write2File( "facenum %d\r\n", facenum );
			int faceno;
			for( faceno = 0; faceno < facenum; faceno++ ){
				int i1, i2, i3;
				i1 = *( ibuf + faceno * 3 );
				i2 = *( ibuf + faceno * 3 + 1 );
				i3 = *( ibuf + faceno * 3 + 2 );

				int materialno;
				ret = RDBGetMaterialNo( hsid, partno, faceno, &materialno );
				_ASSERT( !ret );

				Write2File( "faceno %d, index ( %d, %d, %d )  materialno(%d)\r\n", faceno, i1, i2, i3, materialno );	
			}
			if( ibuf )
				free( ibuf );
		}
		
		RDBDestroyProgressBar();

	}else{
		Write2File( "model data is not loaded return !!!\r\n" );
	}


OnSelectExit:
	if( dispobjname ){
		free( dispobjname );
		dispobjname = 0;
	}
	if( dispobjinfo ){
		free( dispobjinfo );
		dispobjinfo = 0;
	}


	if( hfile != INVALID_HANDLE_VALUE ){
		FlushFileBuffers( hfile );
		SetEndOfFile( hfile );
		CloseHandle( hfile );
		hfile = INVALID_HANDLE_VALUE;
	}

	MessageBox( NULL, "ShapeExporterの処理を終わりました。", "ShapeExporter", MB_OK );

	return ret;
}


int Write2File( char* lpFormat, ... )
{
	if( hfile == INVALID_HANDLE_VALUE ){
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
	WriteFile( hfile, outchar, wleng, &writeleng, NULL );
	if( wleng != writeleng ){
		return 1;
	}

	return 0;	
}
