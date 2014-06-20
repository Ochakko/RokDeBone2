
#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "PluginElem.h"

#define DBGH
#include <dbg.h>

#include <crtdbg.h>

#include "RDB2Version.h"


CPluginElem::CPluginElem()
{
	ZeroMemory( filepath, sizeof( char ) * _MAX_PATH );
	pluginversion = 0;
	product = 0;
	id = 0;

	menuid = 0;

	ZeroMemory( pluginname, sizeof( char ) * _MAX_PATH );
	
	validflag = 0;//!!!!!!!!!!!!!!!!!!!!!!

	RDBOnSelectPlugin = NULL;
	RDBOnClose = NULL;
	RDBOnPose = NULL;

	hModule = NULL;

}
CPluginElem::~CPluginElem()
{
	UnloadPlugin();
}

int CPluginElem::CallOnSelectPlugin()
{
	int ret = 0;

	if( (validflag == 1) && RDBOnSelectPlugin ){//！！！！！！！ validflagに注意！！！！！！
		ret = RDBOnSelectPlugin();
	}

	return ret;
}
int CPluginElem::CallOnClose()
{
	int ret = 0;

	if( (validflag == 1) && RDBOnClose ){//！！！！！！！ validflagに注意！！！！！！
		ret = RDBOnClose();
	}

	return ret;
}
int CPluginElem::CallOnPose( int motid )
{
	int ret = 0;

	if( (validflag == 1) && RDBOnPose ){//！！！！！！！ validflagに注意！！！！！！
		ret = RDBOnPose( motid );
	}

	return ret;
}

int CPluginElem::SetFilePath( char* pluginpath )
{
	int leng;
	leng = (int)strlen( pluginpath );

	if( (leng <= 0) || (leng >= _MAX_PATH) ){
		DbgOut( "pluginelem : SetFilePath : leng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	strcpy_s( filepath, _MAX_PATH, pluginpath );

	return 0;
}


#define GPA(proc) \
	*(FARPROC *)&proc = GetProcAddress(hModule, #proc);


int CPluginElem::LoadPlugin()
{
	validflag = 0;//!!!!!!!!!!!!

	hModule = LoadLibrary( filepath );
	if(hModule == NULL){
		DbgOut( "pluginelem : LoadPlugin : LoadLibrary error %s!!!\r\n", filepath );
		_ASSERT( 0 );
		return 0;
	}

	GPA(RDBGetPlugInName);
	GPA(RDBGetPlugInID);
	GPA(RDBCheckVersion);
	GPA(RDBInit);

	if( !RDBGetPlugInName || !RDBGetPlugInID || !RDBCheckVersion || !RDBInit ){
		DbgOut( "pluginelem : LoadPlugin : this dll is not for RDB2 %s\r\n", filepath );
		DbgOut( "%x %x %x %x\r\n", RDBGetPlugInName, RDBGetPlugInID, RDBCheckVersion, RDBInit );
		return 0;//!!!!!!!!!!!!!!!!!
	}

/////////////

	if( RDBGetPlugInName ){
		const char* nameptr = RDBGetPlugInName();
		if( !nameptr ){
			DbgOut( "pluginelem : LoadPlugin : RDBGetPlugInName return NULL error %s\r\n", filepath );
			_ASSERT( 0 );
			return 0;
		}

		int leng;
		leng = (int)strlen( nameptr );

		if( leng >= _MAX_PATH )
			leng = _MAX_PATH - 1;

		strncpy_s( pluginname, _MAX_PATH, nameptr, leng );
		pluginname[leng] = 0;
	}

////////////
	if( RDBGetPlugInID ){
		RDBGetPlugInID( &product, &id );
	}
////////////
	if( RDBCheckVersion ){
		RDBCheckVersion( (DWORD)RDB2VERSION, &pluginversion );
		if( pluginversion == 0 ){
			DbgOut( "pluginelem : LoadPlugin : this plugin is not supported by RDB2 ver%d : %s\r\n", RDB2VERSION, filepath );
			_ASSERT( 0 );
			return 0;
		}
	}


/////////////
	int ret;
	if( RDBInit ){

		char filename[2048];
		ZeroMemory( filename, 2048 );

		ret = GetModuleFileName( NULL, filename, 2048 );
		if( ret == 0 ){
			_ASSERT( 0 );
			DbgOut( "pluginelem : LoadPlugin : GetModuleFileName error !!!\n" );
			return 1;
		}

		ret = RDBInit( filename );
		if( ret ){
			DbgOut( "pluginelem : LoadPlugin : RDBInit error %s\r\n", filepath );
			_ASSERT( 0 );
			return 0;
		}
	}

////////////
	validflag = 1;//!!!!!!!!!!!!!!!!!!!!!!!!!	


////////////
	GPA(RDBOnSelectPlugin);
	GPA(RDBOnClose);
	GPA(RDBOnPose);

	return 0;
}
int CPluginElem::UnloadPlugin()
{
	validflag = 0;//!!!!!!!!!!!!!!!!

	if( hModule ){
		FreeLibrary( hModule );
		hModule = NULL;
	}
	
	return 0;
}


