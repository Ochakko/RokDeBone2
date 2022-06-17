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
static char* s_jointname = 0;
static int* s_jointinfo = 0;


static int Write2File( char* lpFormat, ... );
static int WriteEachMotion( int hsid, int motid, int jointnum );
static void SafeFreeAndClose();


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

	*ID      = 0xDCBA0002;

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
	return "MotionExporter";
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
		RDBDbgOut( "motionexporter : file open error %s !!!\n", buf );
		return 1;
	}

	int ret = 0;
	int hsid;
	ret = RDBGetCurrentHSID( &hsid );
	if( hsid >= 0 ){

		int motnum = 0;
		ret = RDBGetMotionNum( hsid, &motnum );
		if( ret ){
			_ASSERT( 0 );
			SafeFreeAndClose();
			return 0;
		}

		int jointnum;
		ret = RDBGetJointNum( hsid, &jointnum );
		if( ret ){
			_ASSERT( 0 );
			SafeFreeAndClose();
			return 0;
		}
		Write2File( "jointnum %d\r\n", jointnum );

		if( jointnum > 0 ){
			s_jointname = (char*)malloc( sizeof( char ) * 256 * jointnum );
			if( !s_jointname ){
				_ASSERT( 0 );
				SafeFreeAndClose();
				return 0;
			}
			s_jointinfo = (int*)malloc( sizeof( int ) * JI_MAX * jointnum );
			if( !s_jointinfo ){
				_ASSERT( 0 );
				SafeFreeAndClose();
				return 0;
			}

			int jointgetnum;
			ret = RDBGetJointInfo( hsid, jointnum, s_jointname, s_jointinfo, &jointgetnum );
			if( ret ){
				_ASSERT( 0 );
				SafeFreeAndClose();
				return 0;
			}

			int motid;
			for( motid = 0; motid < motnum; motid++ ){
				ret = WriteEachMotion( hsid, motid, jointnum );
				if( ret ){
					_ASSERT( 0 );
					SafeFreeAndClose();
					return 0;
				}
			}
		}
	}else{
		Write2File( "model data is not loaded return !!!\r\n" );
	}



	SafeFreeAndClose();


	return 0;

}

void SafeFreeAndClose()
{

	if( s_jointname ){
		free( s_jointname );
		s_jointname = 0;
	}
	if( s_jointinfo ){
		free( s_jointinfo );
		s_jointinfo = 0;
	}

	if( s_hfile != INVALID_HANDLE_VALUE ){
		FlushFileBuffers( s_hfile );
		SetEndOfFile( s_hfile );
		CloseHandle( s_hfile );
		s_hfile = INVALID_HANDLE_VALUE;
	}

	MessageBox( NULL, "MotionExporterの処理を終わりました。", "MotionExporter", MB_OK );
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

int WriteEachMotion( int hsid, int motid, int jointnum )
{
	int ret;
	char motionname[256];
	motionname[0] = 0;

	ret = RDBGetMotionName( hsid, motid, motionname );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	Write2File( "\r\n\r\n///// motion name %s /////\r\n", motionname );

	int frameleng;
	ret = RDBGetFrameLeng( hsid, motid, &frameleng );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
				
	if( frameleng == 0 ){
		Write2File( "frameleng is Zero : motion is not included return !!!\r\n" );
		return 0;
	}


	int jointno;
	for( jointno = 0; jointno < jointnum; jointno++ ){

		//名前、シリアル番号の出力
		int boneno;
		boneno = *( s_jointinfo + JI_MAX * jointno + JI_SERIAL );
		Write2File( "\r\n\r\njoint : name %s, boneno %d\r\n", s_jointname + 256 * jointno, boneno );

		//オフセットマトリックスの出力
		RDBMatrix offsetmat;
		ret = RDBGetOffsetMatrix( hsid, boneno, &offsetmat );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}
		Write2File( "offsetmatrix : _11 %f, _12 %f, _13 %f. _14 %f\r\n\t_21 %f, _22 %f, _23 %f. _24 %f\r\n\t_31 %f, _32 %f, _33 %f. _34 %f\r\n\t_41 %f, _42 %f, _43 %f. _44 %f\r\n",
			offsetmat._11, offsetmat._12, offsetmat._13, offsetmat._14,
			offsetmat._21, offsetmat._22, offsetmat._23, offsetmat._24,
			offsetmat._31, offsetmat._32, offsetmat._33, offsetmat._34,
			offsetmat._41, offsetmat._42, offsetmat._43, offsetmat._44
		);


		//スケールＳ，回転Ｒ、移動Ｔの出力
		int* keyframearray = 0;
		keyframearray = (int*)malloc( sizeof( int ) * frameleng );
		if( !keyframearray ){
			_ASSERT( 0 );
			return 1;
		}

		int keynum;
		ret = RDBGetKeyFrameNo( hsid, boneno, motid, keyframearray, frameleng, &keynum );
		if( ret ){
			_ASSERT( 0 );
			free( keyframearray );
			return 1;
		}

		if( keynum > 0 ){
			int keycnt, frameno;
			for( keycnt = 0; keycnt < keynum; keycnt++ ){
				frameno = *( keyframearray + keycnt );

				RDBPoint scale, trans;
				RDBQuaternion rotq;

				//DirectX9 SDKのSkinnedMeshサンプルにあわせた形式
				ret = RDBGetKeyFrameSRT( hsid, boneno, motid, frameno, &scale, &rotq, &trans );
				if( ret ){
					_ASSERT( 0 );
					free( keyframearray );
					return 1;
				}
				Write2File( "format type 0 : frameno %d\r\nscale %f, %f, %f\r\nquaternion %f, %f, %f, %f\r\ntranslate %f %f %f\r\n",
					frameno,
					scale.x, scale.y, scale.z,
					rotq.x, rotq.y, rotq.z, rotq.w,
					trans.x, trans.y, trans.z
				);

				//RokDeBone2内部での形式
				ret = RDBGetKeyFrameSRT2( hsid, boneno, motid, frameno, &scale, &rotq, &trans );
				if( ret ){
					_ASSERT( 0 );
					free( keyframearray );
					return 1;
				}
				Write2File( "format type 1 : frameno %d\r\nscale %f, %f, %f\r\nquaternion %f, %f, %f, %f\r\ntranslate %f %f %f\r\n",
					frameno,
					scale.x, scale.y, scale.z,
					rotq.x, rotq.y, rotq.z, rotq.w,
					trans.x, trans.y, trans.z
				);

			}
		}else{
			Write2File( "keyframe num is Zero : motion data is not exist in this bone\r\n" );
		}

		free( keyframearray );
	}

	return 0;
}