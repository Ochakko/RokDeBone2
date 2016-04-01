%{
	/***
	rok fileのデータを、CRokFileのメンバに読み込む。

	!!!!! 入力fileの　改行は　\r !!!!!!!!!!!!!
	!!!!! 検索パターンの小数点は　[\.]  !!!!!!

	!!!!! 改行以外の一文字を表す'.'は、\rを含んでしまうので注意。

	***/

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include <math.h>
	#include <malloc.h>
	
	#include <coef.h>
	#define	DBGH
	#include <dbg.h>
	#include <rokfile.h>

	#define	STOCKMAX	1024			

	//global
	int LoadRokFile( char* fname, CRokFile* rfptr );
	int FreeLexBuf();

	// static
	static CRokFile* rokfile;
	static FILE* rfile = 0;
	static char	tempchar[STOCKMAX];

	static int InitVars();
	static int CpTemp( int leng );
	static int CpTemp2( int headleng, int leng );
	

%}

%s	NEUTRAL
%s	LOADPOINT
%s	LOADLINE
%s	LOADFACE
%s	LOADPALLET

%%


<NEUTRAL>"POINT\r"	{
	BEGIN	LOADPOINT;
}
<NEUTRAL>"PALC0\r"[^\r]+\r[^\r]+\r	{
	BEGIN	LOADPALLET;
}
<NEUTRAL>"ROKU4\r"[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r {
	int ret;
	ret = CpTemp( yyleng );
	_ASSERT( !ret );
	ret = rokfile->ReadRokHeader( tempchar );
	_ASSERT( !ret );	
}

<LOADPOINT>"LINE0\r"	{
	BEGIN	LOADLINE;
}
<LOADPOINT>[0-9]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r	{
	int ret;
	ret = CpTemp( yyleng );
	_ASSERT( !ret );
	ret = rokfile->ReadPoint( tempchar );
	_ASSERT( !ret );
}


<LOADLINE>"FACE0\r"	{
	BEGIN LOADFACE;
}
<LOADLINE>[0-9]+\r[^\r]+\r[^\r]+\r[^\r]+\r {
	int ret;
	ret = CpTemp( yyleng );
	_ASSERT( !ret );
	ret = rokfile->ReadLine( tempchar );
	_ASSERT( !ret );
}


<LOADFACE>"END00\r"	{
	BEGIN NEUTRAL;
}
<LOADFACE>"4\r"[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r	{
	int ret;
	ret = CpTemp( yyleng );
	ret = rokfile->ReadFace( tempchar );
	_ASSERT( !ret );
}

<LOADFACE>"3\r"[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r	{
	int ret;
	ret = CpTemp( yyleng );
	_ASSERT( !ret );
	ret = rokfile->ReadFace( tempchar );
	_ASSERT( !ret );
}


<LOADPALLET>"ENDP0\r"	{
	BEGIN NEUTRAL;
}
<LOADPALLET>[0-9]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r[^\r]+\r	{
	int ret;
	ret = CpTemp( yyleng );
	_ASSERT( !ret );
	ret = rokfile->ReadPallet( tempchar );
	_ASSERT( !ret );
}


%%
static int yywrap()
{
	return 1;
}


int InitVars()
{
	rokfile = 0;
	rfile = 0;
	ZeroMemory( tempchar, STOCKMAX );

	return 0;
}

int LoadRokFile( char* fname, CRokFile* rfptr )
{
	int ret;
	ret = InitVars();
	_ASSERT( !ret );
	if( !rfptr ){
		DbgOut( "LoadRokFile : rokfile NULL error !!!\n" );
		return 1;
	}
	
	rokfile = rfptr;

	rfile = fopen( fname, "r" );
	if( !rfile ){
		DbgOut( "loadrok.l : LoadRokFile : rfile fopen error !!!\n" );
		return D3DAPPERR_MEDIANOTFOUND;
	}

	BEGIN	NEUTRAL;

	yyrestart( rfile );
	yylex();

	if( rfile )
		fclose( rfile );

	return 0;
}
int FreeLexBuf()
{
	if( yy_current_buffer )
		yy_delete_buffer( yy_current_buffer );
	return 0;
}

int CpTemp2( int headleng, int leng )
{
	int cpleng = 0;
	int	ret = 0;

	// 個々の引数のrange check は省略

	ZeroMemory( tempchar, STOCKMAX );
	if( headleng + leng <= STOCKMAX ){
		cpleng = leng;
	}else{
		_ASSERT( 0 );
		return 1;
	}
	strncpy( tempchar, yytext + headleng, cpleng );
	return 0;
}

int CpTemp( int leng )
{
	int ret;
	ret = CpTemp2( 0, leng );
	return ret;
}

