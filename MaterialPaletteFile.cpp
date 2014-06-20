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

#include "MaterialPaletteFile.h"


CMaterialPaletteFile::CMaterialPaletteFile()
{
	InitParams();

}

int CMaterialPaletteFile::InitParams()
{

	m_hfile = INVALID_HANDLE_VALUE;
	m_buf = 0;
	m_pos = 0;
	m_bufleng = 0;

	ZeroMemory( m_mpalette, sizeof( MPALETTE ) * MPALETTENUM );
	int palno;
	char tempchar[128];
	for( palno = 0; palno < MPALETTENUM; palno++ ){
		sprintf_s( tempchar, 128, "%d", palno );	
		strcpy_s( m_mpalette[palno].name, 64, tempchar );
	}

	m_mpno = 0;

	ZeroMemory( m_block, sizeof( char ) * MPBLOCKLENG );

	return 0;
}

CMaterialPaletteFile::~CMaterialPaletteFile()
{
	DestroyObjs();
}

int CMaterialPaletteFile::DestroyObjs()
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

int CMaterialPaletteFile::SetBuffer( char* filename )
{
	DestroyObjs();

	char outname[2048];
	ZeroMemory( outname, 2048 );


	char* enptr;
	enptr = strchr( filename, '\\' );
	if( !enptr ){

		char moduledir[2048];
		int leng;
		ZeroMemory( moduledir, 2048 );
		leng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)moduledir, 2048 );
		if( (leng <= 0) || (leng >= 2048) ){
			DbgOut( "mpalettefile : SetBuffer : GetEnvironmentVariable error %d !!!\n", leng );
			_ASSERT( 0 );
			return 1;
		}


		int chkleng;
		chkleng = leng + (int)strlen( filename );

		if( chkleng >= 2048 ){
			DbgOut( "mpalettefile : SetBuffer : chkleng error !!! file name too long !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		strcpy_s( outname, 2048, moduledir );
		strcat_s( outname, 2048, filename );
	}else{

		strcpy_s( outname, 2048, filename );

	}

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
		DbgOut( "mpalettefile : SetBuffer :  GetFileSize error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	if( sizehigh != 0 ){
		DbgOut( "mpalettefile : SetBuffer :  file size too large error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	char* newbuf;
	newbuf = (char*)malloc( sizeof( char ) * bufleng + 1 );//bufleng + 1
	if( !newbuf ){
		DbgOut( "mpalettefile : SetBuffer :  newbuf alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	DWORD rleng, readleng;
	rleng = bufleng;
	ReadFile( m_hfile, (void*)newbuf, rleng, &readleng, NULL );
	if( rleng != readleng ){
		DbgOut( "mpalettefile : SetBuffer :  ReadFile error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	m_buf = newbuf;
	*(m_buf + bufleng) = 0;//!!!!!!!!
	m_bufleng = bufleng;
	m_pos = 0;	

	return 0;
}

int CMaterialPaletteFile::CheckFileHeader()
{
	if( !m_buf || (m_bufleng == 0) ){
		_ASSERT( 0 );
		return 1;
	}

	char srcheader[256];
	strcpy_s( srcheader, 256, "Material Palette for RokDeBone2 ver1001" );

	int headerlen;
	headerlen = (int)strlen( srcheader );

	if( m_bufleng <= headerlen ){
		DbgOut( "mpalettefile : CheckFileHeader : error 0 !!!\n" );
		return 1;
	}else{
		int cmp;
		cmp = strncmp( m_buf, srcheader, headerlen );
		if( cmp != 0 ){
			DbgOut( "mpalettefile : CheckFileHeader : invalid palette file error !!!\n" );
			return 1;
		}else{
			return 0;
		}
	}

	return 0;
}

int CMaterialPaletteFile::LoadMaterialPalette( char* filename )
{
	int ret;

	DestroyObjs();
	InitParams();

	ret = SetBuffer( filename );
	if( ret ){
		DbgOut( "mpalettefile : LoadPalette : SetBuffer error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = CheckFileHeader();
	if( ret != 0 ){
		_ASSERT( 0 );
		goto loadexit;
	}

	while( (m_pos < m_bufleng) && (m_mpno < MPALETTENUM) ){
		int leng;
		ret = GetMPBlock( &leng );
		if( ret == 0 ){
			ParseMPBlock( leng );
			
		}
	}


	goto loadexit;
loadexit:

	DestroyObjs();
	return 0;
}

int CMaterialPaletteFile::GetMPBlock( int* lengptr )
{

	char startpat[20] = "start{";
	char endpat[20] = "}end";

	int startpatleng = (int)strlen( startpat );
	int endpatleng = (int)strlen( endpat );


	int startpos = m_pos;
	int endpos = m_pos;

	char* findstart;
	findstart = strstr( m_buf + m_pos, startpat );
	if( findstart ){
		startpos = (int)( findstart - m_buf );
		m_pos = startpos;
	}else{
		m_pos = m_bufleng;
		return 1;//!!!!!!!!!!!!
	}

	char* findend;
	findend = strstr( m_buf + m_pos, endpat );
	if( findend ){
		endpos = (int)( findend - m_buf );
	}else{
		m_pos = m_bufleng;
		return 1;//!!!!!!!!!!!!
	}

	if( (endpos - startpos) >= MPBLOCKLENG ){
		DbgOut( "mpalettefile : GetMPBlock : blockleng too large error !!!\n" );
		_ASSERT( 0 );
		m_pos = m_bufleng;
		return 1;//!!!!!!!!!!!!
	}

	strncpy_s( m_block, MPBLOCKLENG, m_buf + startpos, endpos - startpos );
	m_block[ endpos - startpos ] = 0;

	*lengptr = endpos - startpos;
	m_pos += *lengptr;

//DbgOut( "mpalette %d\r\n%s\r\n", *lengptr, m_block );


	return 0;
}

int CMaterialPaletteFile::ParseMPBlock( int srcleng )
{

//name "palette1"
//diffuse ( 255, 255, 255 )
//specular ( 0, 0, 0 )
//ambient ( 64, 64, 64 )
//emissive ( 0, 0, 0 )
//power 5.0

	char pat[6][20] = 
	{
		"name:",
		"diffuse:",
		"specular:",
		"ambient:",
		"emissive:",
		"power:"
	};

	char* srcchar = m_block;

	MPALETTE* curmp;
	curmp = &(m_mpalette[ m_mpno ]);

	int patno, patleng;
	int pos = 0;
	//int cmp;
	int ret;
	int stepnum;
	char* findpat;
	for( patno = 0; patno < 6; patno++ ){

		//pos = 0;

		//while( (pos < srcleng) && ( (*(srcchar + pos) == ' ') || (*(srcchar + pos) == '\t') ) ){
		//	pos++;
		//}
		
		int tempr, tempg, tempb;

		patleng = (int)strlen( pat[patno] );
		//cmp = strncmp( pat[patno], srcchar + pos, patleng );
		//if( cmp == 0 ){

		findpat = strstr( srcchar, pat[patno] );
		if( findpat ){

			//pos += patleng;//!!!
			pos = (int)( findpat - srcchar + patleng );


			switch( patno ){
			case 0:
				ret = GetName( curmp->name, 63, srcchar, pos, srcleng );
				if( ret ){
					DbgOut( "mpalettefile : ParseMPBlock : GetName error !!!" );
					_ASSERT( 0 );
					return 1;
				}
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				ret = GetInt( &tempr, srcchar, pos, srcleng, &stepnum );
				if( ret ){
					DbgOut( "mpalettefile : ParseMPBlock : GetInt error !!!" );
					_ASSERT( 0 );
					return 1;
				}
				pos += stepnum;
				if( pos >= srcleng ){
					DbgOut( "mpalettefile : ParseMPBlock : GetInt : pos error !!!" );
					_ASSERT( 0 );
					return 1;
				}

				ret = GetInt( &tempg, srcchar, pos, srcleng, &stepnum );
				if( ret ){
					DbgOut( "mpalettefile : ParseMPBlock : GetInt error !!!" );
					_ASSERT( 0 );
					return 1;
				}
				pos += stepnum;
				if( pos >= srcleng ){
					DbgOut( "mpalettefile : ParseMPBlock : GetInt : pos error !!!" );
					_ASSERT( 0 );
					return 1;
				}

				ret = GetInt( &tempb, srcchar, pos, srcleng, &stepnum );
				if( ret ){
					DbgOut( "mpalettefile : ParseMPBlock : GetInt error !!!" );
					_ASSERT( 0 );
					return 1;
				}

				switch( patno ){
				case 1:
					curmp->mat.diffuse = RGB( tempr, tempg, tempb );
					break;
				case 2:
					curmp->mat.specular = RGB( tempr, tempg, tempb );
					break;
				case 3:
					curmp->mat.ambient = RGB( tempr, tempg, tempb );
					break;
				case 4:
					curmp->mat.emissive = RGB( tempr, tempg, tempb );
					break;
				default:
					break;
				}

				break;
			case 5:
				ret = GetFloat( &curmp->mat.power, srcchar, pos, srcleng, &stepnum );
				if( ret ){
					DbgOut( "mpalettefile : ParseMPBlock : GetFloat error !!!" );
					_ASSERT( 0 );
					return 1;
				}
				break;
			default:
				_ASSERT( 0 );
				break;
			}

		}
	}
	
	m_mpno++;

	return 0;
}

int CMaterialPaletteFile::GetName( char* dstchar, int dstleng, char* srcchar, int pos, int srcleng )
{
	int startpos, endpos;
	startpos = pos;

	while( (startpos < srcleng) && 
		( ( *(srcchar + startpos) == ' ' ) || ( *(srcchar + startpos) == '\t' ) || ( *(srcchar + startpos) == '\"' ) ) 
	){
		startpos++;
	}

	endpos = startpos;
	while( (endpos < srcleng) && 
		( ( *(srcchar + endpos) != ' ' ) && ( *(srcchar + endpos) != '\t' ) && (*(srcchar + endpos) != '\r') && (*(srcchar + endpos) != '\n') && (*(srcchar + endpos) != '\"') )
	){
		endpos++;
	}

	if( (endpos - startpos < dstleng) && (endpos - startpos > 0) ){
		strncpy_s( dstchar, dstleng + 1, srcchar + startpos, endpos - startpos );
		*(dstchar + endpos - startpos) = 0;

	}else{
		_ASSERT( 0 );
	}

	return 0;
}
int CMaterialPaletteFile::GetInt( int* dstint, char* srcchar, int pos, int srcleng, int* stepnum )
{
	int startpos, endpos;
	startpos = pos;

	while( (startpos < srcleng) &&  
		( ( isdigit( *(srcchar + startpos) ) == 0 ) && (*(srcchar + startpos) != '-') ) 
	
	){
		startpos++;
	}

	endpos = startpos;
	while( (endpos < srcleng) && 
		( (isdigit( *(srcchar + endpos) ) != 0) || ( *(srcchar + endpos) == '-' ) )
	){
		endpos++;
	}

	char tempchar[256];
	if( (endpos - startpos < 256) && (endpos - startpos > 0) ){
		strncpy_s( tempchar, 256, srcchar + startpos, endpos - startpos );
		tempchar[endpos - startpos] = 0;

		*dstint = atoi( tempchar );

		*stepnum = endpos - pos;
	}else{
		_ASSERT( 0 );
		*stepnum = endpos - pos;
		return 1;
	}


	return 0;
}
int CMaterialPaletteFile::GetFloat( float* dstfloat, char* srcchar, int pos, int srcleng, int* stepnum )
{
	int startpos, endpos;
	startpos = pos;

	while( (startpos < srcleng) &&  
		( ( isdigit( *(srcchar + startpos) ) == 0 ) && (*(srcchar + startpos) != '-') && (*(srcchar + startpos) != '.') ) 
	
	){
		startpos++;
	}

	endpos = startpos;
	while( (endpos < srcleng) && 
		( (isdigit( *(srcchar + endpos) ) != 0) || ( *(srcchar + endpos) == '-' ) || (*(srcchar + endpos) == '.') )
	){
		endpos++;
	}

	char tempchar[256];
	if( (endpos - startpos < 256) && (endpos - startpos > 0) ){
		strncpy_s( tempchar, 256, srcchar + startpos, endpos - startpos );
		tempchar[endpos - startpos] = 0;

		*dstfloat = (float)atof( tempchar );

		*stepnum = endpos - pos;
	}else{
		_ASSERT( 0 );
		*stepnum = endpos - pos;
		return 1;
	}


	return 0;
}

int CMaterialPaletteFile::WriteMaterialPalette( char* filename, MPALETTE* srcmp, int mpnum )
{
	char outname[2048];
	ZeroMemory( outname, 2048 );


	char* enptr;
	enptr = strchr( filename, '\\' );
	if( !enptr ){

		char moduledir[2048];
		int leng;
		ZeroMemory( moduledir, 2048 );
		leng = GetEnvironmentVariable( (LPCTSTR)"MEDIADIR", (LPTSTR)moduledir, 2048 );
		if( (leng <= 0) || (leng >= 2048) ){
			DbgOut( "mpalettefile : WriteMaterialPalette : GetEnvironmentVariable error %d !!!\n", leng );
			_ASSERT( 0 );
			return 1;
		}


		int chkleng;
		chkleng = leng + (int)strlen( filename );

		if( chkleng >= 2048 ){
			DbgOut( "mpalettefile : WriteMaterialPalette : chkleng error !!! file name too long !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		strcpy_s( outname, 2048, moduledir );
		strcat_s( outname, 2048, filename );
	}else{

		strcpy_s( outname, 2048, filename );

	}

	m_hfile = CreateFile( outname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( m_hfile == INVALID_HANDLE_VALUE ){
		DbgOut( "mpalettefile : WriteMaterialPalette : file open error %s !!!\n", filename );
		_ASSERT( 0 );
		return 1;
	}

	Write2File( "Material Palette for RokDeBone2 ver1001\r\n\r\n" );	

//start{
//name: "palette1"
//diffuse: ( 255, 255, 255 )
//specular: ( 0, 0, 0 )
//ambient: ( 64, 64, 64 )
//emissive: ( 0, 0, 0 )
//power: 5.0
//}end


	int mpno;
	MPALETTE* curmp;
	char tempchar[ 1024 ];
	for( mpno = 0; mpno < mpnum; mpno++ ){
		curmp = srcmp + mpno;

		Write2File( "start{\r\n" );
		
		sprintf_s( tempchar, 1024, "name: \"%s\"\r\n", curmp->name );
		Write2File( tempchar );

		sprintf_s( tempchar, 1024, "diffuse: ( %d, %d, %d )\r\n", GetRValue( curmp->mat.diffuse ), GetGValue( curmp->mat.diffuse ), GetBValue( curmp->mat.diffuse ) );
		Write2File( tempchar );

		sprintf_s( tempchar, 1024, "specular: ( %d, %d, %d )\r\n", GetRValue( curmp->mat.specular ), GetGValue( curmp->mat.specular ), GetBValue( curmp->mat.specular ) );
		Write2File( tempchar );

		sprintf_s( tempchar, 1024, "ambient: ( %d, %d, %d )\r\n", GetRValue( curmp->mat.ambient ), GetGValue( curmp->mat.ambient ), GetBValue( curmp->mat.ambient ) );
		Write2File( tempchar );

		sprintf_s( tempchar, 1024, "emissive: ( %d, %d, %d )\r\n", GetRValue( curmp->mat.emissive ), GetGValue( curmp->mat.emissive ), GetBValue( curmp->mat.emissive ) );
		Write2File( tempchar );

		sprintf_s( tempchar, 1024, "power: %f\r\n", curmp->mat.power );
		Write2File( tempchar );

		Write2File( "}end\r\n\r\n" );
	}

	FlushFileBuffers( m_hfile );
	SetEndOfFile( m_hfile );


	return 0;
}

int CMaterialPaletteFile::Write2File( char* lpFormat, ... )
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
