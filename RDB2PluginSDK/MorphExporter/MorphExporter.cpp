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

#include <crtdbg.h>// <--- _ASSERT�}�N��


static HINSTANCE hInstance;
static HANDLE s_hfile = INVALID_HANDLE_VALUE;

#define BASENUMMAX	100

static int s_basenum = 0;
static RDBMORPHBASE*	s_base = 0;
static RDBMORPHTARGET*	s_target[BASENUMMAX];


static int Write2File( char* lpFormat, ... );
static int DestroyObjs();
static int WriteMorphDispInfo( int hsid );
static int WriteMorphMotion( int hsid, int motid );
static void CloseFile();

//---------------------------------------------------------------------------
//  DllMain
//---------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	//�_�C�A���O�{�b�N�X�̕\���ɕK�v�Ȃ̂ŁA�C���X�^���X��ۑ����Ă���
	hInstance = (HINSTANCE)hModule;


	//�ϐ��̏�����
	s_basenum = 0;
	s_base = 0;
	ZeroMemory( s_target, sizeof( RDBMORPHTARGET* ) * BASENUMMAX );

	return TRUE;
}


//---------------------------------------------------------------------------
//  RDBGetPlugInID
//    �v���O�C��ID��Ԃ��B
//    ���̊֐��͋N�����ɌĂяo�����B
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT int RDBGetPlugInID(DWORD *Product, DWORD *ID)
{
	// �v���_�N�g��(����Җ�)��ID���A�S����64bit�̒l�Ƃ��ĕԂ�
	// �l�͑��Əd�����Ȃ��悤�ȃ����_���Ȃ��̂ŗǂ�

	char* cproduct;
	cproduct = (char*)Product;
	*cproduct = 'o';
	*( cproduct + 1 ) = 'c';
	*( cproduct + 2 ) = 'h';
	*( cproduct + 3 ) = 'i';

	*ID      = 0xDCBA0023;

	return 0;
}


//---------------------------------------------------------------------------
//  RDBGetPlugInName
//    �v���O�C������Ԃ��B
//    RokDeBone2�̃v���O�C�����j���[�ɁA���̖��O���\�������B
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT const char* RDBGetPlugInName(void)
{
	// �v���O�C����
	return "MorphExporter";
}

//---------------------------------------------------------------------------
//  RDBOnClose
//    RokDeBone2���I�����钼�O��RokDeBone2����Ă΂��B
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT int RDBOnClose(void)
{
	return 0;
}

//---------------------------------------------------------------------------
//  RDBOnPose
//    RokDeBone2�Ń��[�U�[���{�[���̎p����ҏW���A�}�E�X�𗣂����Ƃ���RokDeBone2����Ă΂��B
//    ���[�V�����|�C���g�͕ύX����Ă��邪FillUp�͂���Ă��Ȃ���ԂŌĂ΂��B
//---------------------------------------------------------------------------
RDBPLUGIN_EXPORT int RDBOnPose( int motid )
{
	return 0;
}


//----------------------------------------------------------------------------
//  RDBOnSelectPlugin
//  RokDeBone2�̃v���O�C�����j���[�ŁARDBGetPlugInName�̕������I�������Ƃ��ɁA
//  ���̊֐����A���A�Ă΂�܂��B
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
		RDBDbgOut( "morphexporter : file open error %s !!!\n", buf );
		_ASSERT( 0 );
		return 1;
	}

	int ret = 0;
	ret = Write2File( "RokDeBone2 MorphInfoFile ver1001 type0\r\n\r\n" );
	_ASSERT( !ret );

	DestroyObjs();

	int hsid;
	ret = RDBGetCurrentHSID( &hsid );
	if( hsid >= 0 ){

		ret = WriteMorphDispInfo( hsid );
		if( ret ){
			_ASSERT( 0 );
			MessageBox( NULL, "MorphExporter���G���[�I�����܂����B", "MorphExporter", MB_OK );
			DestroyObjs();
			CloseFile();
			return 1;
		}

		if( s_basenum > 0 ){
			int motid = -1;
			ret = RDBGetCurrentMotID( hsid, &motid );
			_ASSERT( !ret );

			if( motid >= 0 ){
				ret = WriteMorphMotion( hsid, motid );
				if( ret ){
					_ASSERT( 0 );
					MessageBox( NULL, "MorphExporter���G���[�I�����܂����B", "MorphExporter", MB_OK );
					DestroyObjs();
					CloseFile();
					return 1;
				}
			}
		}
	}else{
		Write2File( "model data is not loaded return !!!\r\n" );
	}

	ret = Write2File( "#EndOfFile\r\n" );
	_ASSERT( !ret );

	DestroyObjs();

	CloseFile();

	MessageBox( NULL, "MorphExporter�̏������I���܂����B", "MorphExporter", MB_OK );

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



int DestroyObjs()
{
	if( s_base ){
		free( s_base );
		s_base = 0;
	}

	int bno;
	for( bno = 0; bno < BASENUMMAX; bno++ ){
		if( s_target[bno] ){
			free( s_target[bno] );
			s_target[bno] = 0;
		}
	}

	s_basenum = 0;

	return 0;
}

int WriteMorphDispInfo( int hsid )
{
	int ret;
	ret = RDBGetMorphBaseNum( hsid, &s_basenum );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	if( s_basenum <= 0 ){
		return 0;
	}
	if( s_basenum >= BASENUMMAX ){
		RDBDbgOut( "morphexporter : WriteMorphDispInfo : basenum overflow error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = Write2File( "#dispobjpart Start\r\n" );
	_ASSERT( !ret );

	s_base = (RDBMORPHBASE*)malloc( sizeof( RDBMORPHBASE ) * s_basenum );
	if( !s_base ){
		_ASSERT( 0 );
		return 1;
	}

	ret = RDBGetMorphBaseInfo( hsid, s_base, s_basenum );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}

	ret = Write2File( "\tbasenum %d\r\n\r\n", s_basenum );
	_ASSERT( !ret );

	int basecnt;
	for( basecnt = 0; basecnt < s_basenum; basecnt++ ){
		RDBMORPHBASE* curbase = s_base + basecnt;

		ret = Write2File( "\t#base Start\r\n" );
		_ASSERT( !ret );
		ret = Write2File( "\tbasename \"%s\"\r\n", curbase->name );
		_ASSERT( !ret );
		ret = Write2File( "\tbaseid %d\r\n", curbase->dispno );
		_ASSERT( !ret );
		ret = Write2File( "\tboneid %d\r\n", curbase->boneno );
		_ASSERT( !ret );
		ret = Write2File( "\ttargetnum %d\r\n\r\n", curbase->targetnum );
		_ASSERT( !ret );

		s_target[basecnt] = (RDBMORPHTARGET*)malloc( sizeof( RDBMORPHTARGET ) * curbase->targetnum );
		if( !s_target[basecnt] ){
			_ASSERT( 0 );
			return 1;
		}
		ret = RDBGetMorphTargetInfo( hsid, curbase->dispno, s_target[basecnt], curbase->targetnum );
		if( ret ){
			_ASSERT( 0 );
			return 1;
		}

		int targetcnt;
		for( targetcnt = 0; targetcnt < curbase->targetnum; targetcnt++ ){
			RDBMORPHTARGET* curtarget = s_target[basecnt] + targetcnt;

			ret = Write2File( "\t#target Start\r\n" );
			_ASSERT( !ret );
			ret = Write2File( "\t\ttargetname \"%s\"\r\n", curtarget->name );
			_ASSERT( !ret );
			ret = Write2File( "\t\ttargetid %d\r\n", curtarget->dispno );
			_ASSERT( !ret );
			ret = Write2File( "\t#target End\r\n" );
			_ASSERT( !ret );
		}

		ret = Write2File( "\t#base End\r\n\r\n" );
		_ASSERT( !ret );

	}

	ret = Write2File( "#dispobjpart End\r\n\r\n" );
	_ASSERT( !ret );


	return 0;
}
int WriteMorphMotion( int hsid, int motid )
{
	int ret;
	ret = Write2File( "#motionpart Start\r\n" );
	_ASSERT( !ret );

	char motname[256];
	ret = RDBGetMotionName( hsid, motid, motname );
	if( ret ){
		_ASSERT( 0 );
		return 1;
	}
	ret = Write2File( "\tmotionname \"%s\"\r\n\r\n", motname );
	_ASSERT( !ret );

	int basecnt;
	for( basecnt = 0; basecnt < s_basenum; basecnt++ ){
		RDBMORPHBASE* curbase = s_base + basecnt;

		int targetcnt;
		for( targetcnt = 0; targetcnt < curbase->targetnum; targetcnt++ ){
			_ASSERT( s_target[basecnt] );
			RDBMORPHTARGET* curtarget = s_target[basecnt] + targetcnt;
			
			int keynum = 0;
			ret = RDBGetMorphKeyNum( hsid, motid, curbase->boneno, curbase->dispno, curtarget->dispno, &keynum );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			if( keynum > 0 ){
				RDBMORPHMOTION* motioninfo;
				motioninfo = (RDBMORPHMOTION*)malloc( sizeof( RDBMORPHMOTION ) * keynum );
				if( !motioninfo ){
					_ASSERT( 0 );
					return 1;
				}
				ret = RDBGetMorphKey( hsid, motid, curbase->boneno, curbase->dispno, curtarget->dispno,
					motioninfo, keynum );
				if( ret ){
					_ASSERT( 0 );
					free( motioninfo );
					return 1;
				}

				int keycnt;
				for( keycnt = 0; keycnt < keynum; keycnt++ ){
					RDBMORPHMOTION* curmotion = motioninfo + keycnt;

					ret = Write2File( "\t\"%s\", \"%s\", %d, %f\r\n",
						curbase->name, curtarget->name, curmotion->frameno, curmotion->blendrate );
					_ASSERT( !ret );
				}

				free( motioninfo );
			}


/***
	//�S���̃t���[���̃u�����h���i��ԍς݁j���o�͂������ꍇ�͂R�R�U�s�ڂ���R�U�W�s�ڂ��R�����g�A�E�g���A���̃u���b�N��L���ɂ���B
			int frameleng;
			ret = RDBGetFrameLeng( hsid, motid, &frameleng );
			if( ret ){
				_ASSERT( 0 );
				return 1;
			}

			int frameno;
			for( frameno = 0; frameno < frameleng; frameno++ ){
				ret = RDBSetMotionFrameNo( hsid, motid, frameno );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}
				RDBMORPHMOTION tmpmotion;
				ret = RDBGetCurMorphInfo( hsid, curbase->boneno, curbase->dispno, curtarget->dispno, &tmpmotion );
				if( ret ){
					_ASSERT( 0 );
					return 1;
				}

				ret = Write2File( "\t\"%s\", \"%s\", %d, %f\r\n",
					curbase->name, curtarget->name, tmpmotion.frameno, tmpmotion.blendrate );
				_ASSERT( !ret );

			}
***/
		}

	}


	ret = Write2File( "#motionpart End\r\n\r\n" );
	_ASSERT( !ret );

	return 0;
}
