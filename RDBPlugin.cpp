#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#include <crtdbg.h>

#define DBGH
#include <dbg.h>

#include "RDBPlugin.h"
#include "RDB2Version.h"


#include <math.h>
#include <D3DX9.h>
#include "viewer.h"
#include <coef.h>
#include <treehandler2.h>
#include <shdhandler.h>
#include <mothandler.h>
#include <shdelem.h>
#include <treeelem2.h>

#include <motionctrl.h>
#include <motioninfo.h>
#include <morph.h>
#include <MMotKey.h>
#include <MMotElem.h>

#include <sigfile.h>
#include <rokfile.h>
#include <mqofile.h>
#include <BoneFile.h>
#include <BIMFile.h>

#include <dxerr.h>
#include <Commdlg.h>
#include <bgdisp.h>
#include <basedat.h>
#include <dibfile.h>
#include <lightdata.h>
#include "inidata.h" // for ini file

#include <im2file.h>
#include <cpuid.h>
#include <asmfunc.h>
#include "ColDlg.h"

#include <texbank.h>

#include "MotDlg.h"

#include <sigfile.h>
#include <mqomaterial.h>

#include "sigeditwnd.h"

//////////////////////////////////
// extern
//////////////////////////////////
extern CMyD3DApplication* g_d3dapp;
extern CMotDlg* g_motdlg;

//////////////////////////////////


RDBPLUGIN_EXPORT int RDBGetVersion( int* verptr )
{
	*verptr = RDB2VERSION;//RDB2Version.h !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return 0;
}



RDBPLUGIN_EXPORT int RDBDbgOut( char* lpFormat, ... )
{

	int ret;
	va_list Marker;
	char outchar[4098];
			
	ZeroMemory( outchar, 4098 );

	va_start( Marker, lpFormat );
	ret = vsprintf_s( outchar, 4098, lpFormat, Marker );
	va_end( Marker );

	if( ret < 0 )
		return 1;

	ret = DbgOut( outchar );
	_ASSERT( !ret );

	return ret;
}

RDBPLUGIN_EXPORT int RDBGetCurrentHSID( int* hsidptr )
{

	if( g_d3dapp->m_shandler && g_d3dapp->m_mhandler && g_d3dapp->m_thandler ){
		*hsidptr = 0;
	}else{
		*hsidptr = -1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetCurrentMotID( int hsid, int* motidptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		*motidptr = -1;
		return 0;
	}

	*motidptr = g_motdlg->GetMotCookie();

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetParent( int hsid, int partno, int* parentnoptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		*parentnoptr = -1;
		return 0;
	}

	if( hsid != 0 ){
		*parentnoptr = -1;
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetParent( partno, parentnoptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetParent : sh GetParent error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetChild( int hsid, int partno, int* childnoptr, int arrayleng, int* childnumptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		*childnoptr = -1;
		*childnumptr = 0;
		return 0;
	}

	if( hsid != 0 ){
		*childnoptr = -1;
		*childnumptr = 0;
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetChild( partno, arrayleng, childnoptr, childnumptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetChild : sh GetChild error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetName( int hsid, int partno, char* nameptr, int strleng )
{
	*nameptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_thandler->GetName( partno, nameptr, strleng );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetName : th GetName error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetPartNoByName( int hsid, char* nameptr, int isjoint, int* partnoptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	if( isjoint == 0 ){
		ret = g_d3dapp->m_thandler->GetDispObjNoByName( nameptr, partnoptr, g_d3dapp->m_shandler, 1 );
		if( ret ){
			DbgOut( "rdbplugin : RDBGetPartNoByName : th GetDispObjNoByName error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		ret = g_d3dapp->m_thandler->GetBoneNoByName( nameptr, partnoptr, g_d3dapp->m_shandler, 0 );
		if( ret ){
			DbgOut( "rdbplugin : RDBGetPartNoByName : th GetBoneNoByName error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}


RDBPLUGIN_EXPORT int RDBGetDispObjNum( int hsid, int* numptr )
{	
	*numptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}


	int findcnt = 0;
	CShdElem* selem;
	int seri;

	for( seri = 1; seri < g_d3dapp->m_shandler->s2shd_leng; seri++ ){
		selem = (*g_d3dapp->m_shandler)( seri );
		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){

			findcnt++;
		}

	}

	*numptr = findcnt;

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetDispObjInfo( int hsid, int maxnum, char* strptr, int* seriptr, int* getnumptr )
{

	*getnumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}



	int setcnt = 0;
	CShdElem* selem;
	CTreeElem2* telem;
	int seri;

	int namelen;

	for( seri = 1; seri < g_d3dapp->m_shandler->s2shd_leng; seri++ ){
		selem = (*g_d3dapp->m_shandler)( seri );
		telem = (*g_d3dapp->m_thandler)( seri );

		if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){

			if( setcnt >= maxnum ){
				DbgOut( "rdbplugin: GetDispObjInfo : maxnum too short error %d !!!\n", maxnum );
				_ASSERT( 0 );
				return 1;
			}

			namelen = (int)strlen( telem->name );
			if( namelen >= 256 ){
				DbgOut( "rdbplugin : GetDispObjInfo : namelen too large warning !!!\n" );
				_ASSERT( 0 );
				strcpy_s( strptr + 256 * setcnt, 256, "エラー。名前が長すぎて格納できません。" );
			}else{
				strcpy_s( strptr + 256 * setcnt, 256, telem->name );
			}

			*(seriptr + DOI_MAX * setcnt + DOI_SERIAL) = selem->serialno;

			*(seriptr + DOI_MAX * setcnt + DOI_NOTUSE) = selem->notuse;

			*(seriptr + DOI_MAX * setcnt + DOI_DISPSWITCH) = selem->dispswitchno;
			*(seriptr + DOI_MAX * setcnt + DOI_INVISIBLE) = selem->invisibleflag;

			setcnt++;
		}
	}

	*getnumptr = setcnt;

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetClockwise( int hsid, int partno, int* clockwiseptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBGetClockwise : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	*clockwiseptr = selem->clockwise; 

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetJointNum( int hsid, int* numptr )
{
	*numptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}


	int findcnt = 0;
	CShdElem* selem;
	int seri;

	for( seri = 1; seri < g_d3dapp->m_shandler->s2shd_leng; seri++ ){
		selem = (*g_d3dapp->m_shandler)( seri );
		if( selem->IsJoint() ){

			findcnt++;
		}

	}

	*numptr = findcnt;
	

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetJointInfo( int hsid, int maxnum, char* strptr, int* seriptr, int* getnumptr )
{

	*getnumptr = 0;


	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}


	int setcnt = 0;
	CShdElem* selem;
	CTreeElem2* telem;
	int seri;

	int namelen;

	for( seri = 1; seri < g_d3dapp->m_shandler->s2shd_leng; seri++ ){
		selem = (*g_d3dapp->m_shandler)( seri );
		telem = (*g_d3dapp->m_thandler)( seri );

		if( selem->IsJoint() ){

			if( setcnt >= maxnum ){
				DbgOut( "rdbplugin: GetJointInfo : maxnum too short error %d !!!\n", maxnum );
				_ASSERT( 0 );
				return 1;
			}

			namelen = (int)strlen( telem->name );
			if( namelen >= 256 ){
				DbgOut( "rdbpgluin : GetJointInfo : namelen too large warning !!!\n" );
				_ASSERT( 0 );
				strcpy_s( strptr + 256 * setcnt, 256, "エラー。名前が長すぎて格納できません。" );
			}else{
				strcpy_s( strptr + 256 * setcnt, 256, telem->name );
			}

			*(seriptr + JI_MAX * setcnt + JI_SERIAL) = selem->serialno;

			*(seriptr + JI_MAX * setcnt + JI_NOTUSE) = selem->notuse;


			if( selem->parent && selem->parent->IsJoint() ){//!!!!!!! joint以外の親は、記述しない。
				*(seriptr + JI_MAX * setcnt + JI_PARENT) = selem->parent->serialno;
			}else{
				*(seriptr + JI_MAX * setcnt + JI_PARENT) = -1;
			}

			if( selem->child ){
				*(seriptr + JI_MAX * setcnt + JI_CHILD) = selem->child->serialno;
			}else{
				*(seriptr + JI_MAX * setcnt + JI_CHILD) = -1;
			}

			if( selem->brother ){
				*(seriptr + JI_MAX * setcnt + JI_BROTHER) = selem->brother->serialno;
			}else{
				*(seriptr + JI_MAX * setcnt + JI_BROTHER) = -1;
			}

			if( selem->sister ){
				*(seriptr + JI_MAX * setcnt + JI_SISTER) = selem->sister->serialno;
			}else{
				*(seriptr + JI_MAX * setcnt + JI_SISTER) = -1;
			}

			//*(seriptr + JI_MAX * setcnt + JI_RESERVED1) = 0;
			//*(seriptr + JI_MAX * setcnt + JI_RESERVED2) = 0;
			//*(seriptr + JI_MAX * setcnt + JI_RESERVED3) = 0;

			setcnt++;
		}
	}

	*getnumptr = setcnt;

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetVertNumOfPart( int hsid, int partno, int* numptr )
{
	*numptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetVertNumOfPart( partno, numptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetVertNumOfPart : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetVertPos( int hsid, int partno, int vertno, RDBPoint* posptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}
	
	int ret;
	D3DXVECTOR3 dstpos;
	ret = g_d3dapp->m_shandler->GetVertPos( partno, vertno, &dstpos );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetVertPos : sh GetVertPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	posptr->x = dstpos.x;
	posptr->y = dstpos.y;
	posptr->z = dstpos.z;

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetVertPos2( int hsid, int partno, int vertno, int motid, int frameno, RDBPoint* posptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}
	
	int ret;
	D3DXVECTOR3 dstpos;
	ret = g_d3dapp->m_shandler->GetVertPos2( g_d3dapp->m_mhandler, g_d3dapp->m_matWorld, motid, frameno, partno, vertno, &dstpos, 1 );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetVertPos2 : sh GetVertPos2 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	posptr->x = dstpos.x;
	posptr->y = dstpos.y;
	posptr->z = dstpos.z;

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetVertNoOfFace( int hsid, int partno, int faceno, int* vptr1, int* vptr2, int* vptr3 )
{
	*vptr1 = -1;
	*vptr2 = -1;
	*vptr3 = -1;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetVertNoOfFace( partno, faceno, vptr1, vptr3, vptr3 );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetVertNoOfFace : sh GetVertNoOfFace error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetSamePosVert( int hsid, int partno, int vertno, int* samenoptr, int arrayleng, int* samenumptr )
{
	*samenumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetSamePosVert( partno, vertno, samenoptr, arrayleng, samenumptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetSamePosVert : sh GetSamePosVert error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetInfElemNum( int hsid, int partno, int vertno, int* numptr )
{
	*numptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBGetInfElemNum : partno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	int vnum;
	ret = g_d3dapp->m_shandler->GetVertNumOfPart( partno, &vnum );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetInfElemNum : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (vertno < 0) || (vertno >= vnum) ){
		DbgOut( "rdbplugin : RDBGetInfElemNum : vertno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = g_d3dapp->m_shandler->GetInfElemNum( partno, vertno, numptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetInfElemNum : sh GetInfElemNum error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	/***
	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	if( (selem->type == SHDPOLYMESH) || (selem->type == SHDPOLYMESH2) ){
		*numptr = 2;//!!!!!!!!!!! 
	}else{
		*numptr = 0;
	}
	***/

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetInfElem( int hsid, int partno, int vertno, int infno, int* bonenoptr, float* rateptr )
{
	*bonenoptr = -1;
	*rateptr = 0.0f;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetInfElemParam( partno, vertno, infno, bonenoptr, rateptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetInfElem : sh GetInfElemParam error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetInfElem2( int hsid, int partno, int vertno, int infno, int* bonenoptr, int* childnoptr, float* rateptr )
{
	*bonenoptr = -1;
	*rateptr = 0.0f;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetInfElemParam2( partno, vertno, infno, bonenoptr, childnoptr, rateptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetInfElem : sh GetInfElemParam error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
RDBPLUGIN_EXPORT int RDBSetInfElem( int hsid, int partno, int vertno, int childjointno, int calcmode, int paintmode, int normalizeflag, float userrate, float directval )
{

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}


	float fuserrate;
	fuserrate = userrate / 100.0f;// % --> 数値

	if( fuserrate < 0.0f ){
		_ASSERT( 0 );
		fuserrate = 0.0f;
	}

	if( directval < 0.0f ){
		_ASSERT( 0 );
		directval = 0.0f;
	}

	int ret;

	ret = g_d3dapp->m_shandler->SetInfElem( partno, vertno, childjointno, paintmode, calcmode, fuserrate, normalizeflag, directval );
	if( ret ){
		if( ret != 2 ){
			DbgOut( "rdbplugin : RDBSetInfElem : sh SetInfElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}else{
			//ret 2 は、５個以上の影響度設定をしようとして失敗したとき。
			DbgOut( "rdbplugin : RDBSetInfElem : over 5 influence set warning : this ope is ignored\n" );
		}
	}

	ret = g_d3dapp->m_shandler->CreateSkinMat( partno, 1 );
	if( ret ){
		DbgOut( "rdbplugin : RDBSetInfElem : sh CreateSkinMat error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}



	return 0;
}
RDBPLUGIN_EXPORT int RDBChkIM2Status( int hsid, int* statusptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		*statusptr = 0;
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	*statusptr = g_d3dapp->m_shandler->m_im2enableflag;

	return 0;
}



RDBPLUGIN_EXPORT int RDBGetFaceNum( int hsid, int partno, int* numptr )
{
	*numptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetFaceNum( partno, numptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetFaceNum : sh GetFaceNum error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetTriIndicesInt( int hsid, int partno, int* buf, int intnum, int* setintnumptr, int* revflagptr )
{

	*setintnumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetTriIndicesInt( partno, buf, intnum, setintnumptr, revflagptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetTriIndices : sh GetTriIndices error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}


RDBPLUGIN_EXPORT int RDBGetTriIndices( int hsid, int partno, WORD* buf, int wordnum, int* setwordnumptr, int* revflagptr )
{

	*setwordnumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetTriIndices( partno, buf, wordnum, setwordnumptr, revflagptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetTriIndices : sh GetTriIndices error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetAlpha( int hsid, int partno, int vertno, int* alphaptr )
{

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}
	
	int ret;
	ret = g_d3dapp->m_shandler->GetAlpha( partno, vertno, alphaptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetAlpha : shandler GetAlpha error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetDiffuse( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	ret = g_d3dapp->m_shandler->GetDiffuse( partno, vertno, rptr, gptr, bptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetDiffuse : shandler GetDiffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetAmbient( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	ret = g_d3dapp->m_shandler->GetAmbient( partno, vertno, rptr, gptr, bptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetAmbient : shandler GetAmbient error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetSpecular( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	ret = g_d3dapp->m_shandler->GetSpecular( partno, vertno, rptr, gptr, bptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetSpecular : shandler GetSpecular error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetEmissive( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	ret = g_d3dapp->m_shandler->GetEmissive( partno, vertno, rptr, gptr, bptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetEmissive : shandler GetEmissive error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetSpecularPower( int hsid, int partno, int vertno, float* powerptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	ret = g_d3dapp->m_shandler->GetSpecularPower( partno, vertno, powerptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetSpecularPower : shandler GetSpecularPower error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetShaderType( int hsid, int* shaderptr, int* overflowptr )
{
	
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	*shaderptr = g_d3dapp->m_shandler->m_shader;
	*overflowptr = g_d3dapp->m_shandler->m_overflow;

	return 0;
}


RDBPLUGIN_EXPORT int RDBGetTextureInfo( int hsid, int partno, char* nameptr, int* repuptr, int* repvptr, float* animuptr, float* animvptr, int* transparentptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}


	if( (partno <= 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBGetTextureInfo : partno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	if( !selem ){
		DbgOut( "rdbplugin : RDBGetTextureInfo : selem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = selem->GetTextureInfo( nameptr, repuptr, repvptr, animuptr, animvptr, transparentptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetTextureInfo : selem GetTextureInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetExtTextureInfo( int hsid, int partno, char* extnameptr, int* texnumptr, int* texmodeptr, int* repflagptr, int* stepptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}


	if( (partno <= 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBGetExtTextureInfo : partno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	if( !selem ){
		DbgOut( "rdbplugin : RDBGetExtTextureInfo : selem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = selem->GetExtTextureInfo( extnameptr, texnumptr, texmodeptr, repflagptr, stepptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetExtTextureInfo : selem GetExtTextureInfo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetBonePos( int hsid, int boneno, int poskind, int motid, int frameno, RDBPoint* posptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}
	
	int ret;
	D3DXVECTOR3 dstpos;
	ret = g_d3dapp->m_shandler->GetBonePos( boneno, poskind, motid, frameno, &dstpos, g_d3dapp->m_mhandler, g_d3dapp->m_matWorld, 1 );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetBonePos : shandler GetBonePos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	posptr->x = dstpos.x;
	posptr->y = dstpos.y;
	posptr->z = dstpos.z;

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetBoneMatrix( int hsid, int boneno, int motid, int frameno, int mode, RDBMatrix* matrixptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}
	
	int ret;
	D3DXMATRIX dstmat;
	if( mode == 0 ){
		ret = g_d3dapp->m_mhandler->HuGetRawMatrix( boneno, motid, frameno, &dstmat );
		if( ret ){
			DbgOut( "rdbplugin : RDBGetBoneMatrix : mh HuGetRawMatrix error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		ret = g_d3dapp->m_mhandler->HuGetBoneMatrix( boneno, motid, frameno, &dstmat );
		if( ret ){
			DbgOut( "rdbplugin : RDBGetBoneMatrix : mh HuGetBoneMatrix error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	matrixptr->_11 = dstmat._11;
	matrixptr->_12 = dstmat._12;
	matrixptr->_13 = dstmat._13;
	matrixptr->_14 = dstmat._14;

	matrixptr->_21 = dstmat._21;
	matrixptr->_22 = dstmat._22;
	matrixptr->_23 = dstmat._23;
	matrixptr->_24 = dstmat._24;

	matrixptr->_31 = dstmat._31;
	matrixptr->_32 = dstmat._32;
	matrixptr->_33 = dstmat._33;
	matrixptr->_34 = dstmat._34;

	matrixptr->_41 = dstmat._41;
	matrixptr->_42 = dstmat._42;
	matrixptr->_43 = dstmat._43;
	matrixptr->_44 = dstmat._44;

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetBoneQuaternion( int hsid, int boneno, int motid, int frameno, int mode, RDBQuaternion* qptr )
{
	
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}
	
	int ret;
	CQuaternion dstq;

	ret = g_d3dapp->m_mhandler->GetBoneQ( g_d3dapp->m_shandler, boneno, motid, frameno, mode, &dstq );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetBoneQuaternion : mh GetBoneQ error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CQuaternion invq;
	dstq.inv( &invq );

	qptr->x = invq.x;
	qptr->y = invq.y;
	qptr->z = invq.z;
	qptr->w = invq.w;

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetFrameLeng( int hsid, int motid, int* lengptr )
{
	*lengptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	/***
	if( motid != g_motdlg->GetMotCookie() ){
		DbgOut( "rdbplugin : RDBGetFrameLeng : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	***/

	int ret;
	ret = g_motdlg->GetFrameLeng( motid, lengptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetFrameLeng : motdlg GetFrameLeng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetKeyFrameNo( int hsid, int boneno, int motid, int* framearray, int arrayleng, int* framenumptr )
{
	*framenumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_mhandler->HuGetKeyframeNo( motid, boneno, framearray, arrayleng, framenumptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetKeyFrameNo : mh HuGetKeyFrameNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetKeyFrameSRT( int hsid, int boneno, int motid, int frameno, RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	D3DXVECTOR3 scale, trans;
	D3DXQUATERNION rot;
	ret = g_d3dapp->m_mhandler->HuGetKeyframeSRT( g_d3dapp->m_shandler, motid, boneno, frameno, &scale, &rot, &trans );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetKeyFrameSRT : mh HuGetKeyFrameSRT error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	scaleptr->x = scale.x;
	scaleptr->y = scale.y;
	scaleptr->z = scale.z;

	rotptr->x = rot.x;
	rotptr->y = rot.y;
	rotptr->z = rot.z;
	rotptr->w = rot.w;

	traptr->x = trans.x;
	traptr->y = trans.y;
	traptr->z = trans.z;

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetKeyFrameSRT2( int hsid, int boneno, int motid, int frameno, RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	D3DXVECTOR3 scale, trans;
	D3DXQUATERNION rot;
	ret = g_d3dapp->m_mhandler->GetKeyframeSRT2( g_d3dapp->m_shandler, motid, boneno, frameno, &scale, &rot, &trans );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetKeyFrameSRT : mh HuGetKeyFrameSRT error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	scaleptr->x = scale.x;
	scaleptr->y = scale.y;
	scaleptr->z = scale.z;

	rotptr->x = rot.x;
	rotptr->y = rot.y;
	rotptr->z = rot.z;
	rotptr->w = rot.w;

	traptr->x = trans.x;
	traptr->y = trans.y;
	traptr->z = trans.z;

	return 0;
}


RDBPLUGIN_EXPORT int RDBGetOffsetMatrix( int hsid, int boneno, RDBMatrix* matrixptr )
{

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	D3DXMATRIX dstmat;
	ret = g_d3dapp->m_shandler->HuGetOffsetMatrix( boneno, &dstmat );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetOffsetMatrix : sh HuGetOffsetMatrix error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	matrixptr->_11 = dstmat._11;
	matrixptr->_12 = dstmat._12;
	matrixptr->_13 = dstmat._13;
	matrixptr->_14 = dstmat._14;

	matrixptr->_21 = dstmat._21;
	matrixptr->_22 = dstmat._22;
	matrixptr->_23 = dstmat._23;
	matrixptr->_24 = dstmat._24;

	matrixptr->_31 = dstmat._31;
	matrixptr->_32 = dstmat._32;
	matrixptr->_33 = dstmat._33;
	matrixptr->_34 = dstmat._34;

	matrixptr->_41 = dstmat._41;
	matrixptr->_42 = dstmat._42;
	matrixptr->_43 = dstmat._43;
	matrixptr->_44 = dstmat._44;

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetCurrentFrameNo( int hsid, int* framenoptr )
{
	*framenoptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_motdlg->GetMotCookie() < 0 ){
		DbgOut( "rdbplugin : RDBGetCurrentFrameNo : motion not loaded warning !!!\n" );
		_ASSERT( 0 );
		return 0;//!!!!!!!!!!!!
	}

	int ret;
	int motid;
	ret = g_motdlg->GetMotParams( &motid, framenoptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetCurrentFrameNo : motdlg GetMotParams error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetUV( int hsid, int partno, int vertno, float* uptr, float* vptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetUV( partno, vertno, uptr, vptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetUV : sh GetUV error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetNormal( int hsid, int partno, int motid, int frameno, RDBPoint* nptr, int arrayleng )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBGetNormal : partno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*(g_d3dapp->m_shandler))( partno );
	_ASSERT( selem );

	if( (selem->type != SHDPOLYMESH) && (selem->type != SHDPOLYMESH2) ){
		DbgOut( "rdbplugin : RDBGetNormal : this is not disp data error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int vnum = 0;
	ret = g_d3dapp->m_shandler->GetVertNumOfPart( partno, &vnum );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetNormal : sh GetVertNumOfPart error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( vnum > arrayleng ){
		DbgOut( "rdbplugin : RDBGetNormal : arrayleng too short error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( vnum == 0 ){
		DbgOut( "rdbplugin : RDBGetNormal : no vertex in this object error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//////////

	if( motid >= 0 ){
		int savemotid, saveframeno;
		savemotid = g_motdlg->GetMotCookie();
		saveframeno = g_motdlg->GetCurrentFrameNo();

		ret = g_d3dapp->m_mhandler->SetMotionFrameNo( g_d3dapp->m_shandler, motid, frameno, 1 );
		if( ret ){
			DbgOut( "rdbpluin : RDBGetNormal : mh SetMotionFrameNo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		g_d3dapp->m_mhandler->SetNewPose( g_d3dapp->m_shandler );
		

		ret = g_d3dapp->m_shandler->ChkInView( g_d3dapp->m_mhandler, &(g_d3dapp->m_matWorld), &(g_d3dapp->m_matView), &(g_d3dapp->m_matProjX) );
		if( ret ){
			DbgOut( "rdbplugin : RDBGetNormal : sh ChkInView error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		/***
		DWORD dwClipWidth  = g_d3dapp->m_clientWidth/2;
		DWORD dwClipHeight = g_d3dapp->m_clientHeight/2;
		D3DXMATRIX firstscale;
		g_d3dapp->m_shandler->GetFirstScale( &firstscale );
		
		float calaspect;
		if( g_d3dapp->m_matProj._43 < -1.0f ){
			calaspect = g_d3dapp->m_shandler->m_aspect;
		}else{
			//正射影
			calaspect = (float)dwClipWidth / (float)dwClipHeight;
		}

		ret = selem->TransformDispData( &firstscale, &(g_d3dapp->m_shandler->m_mc), dwClipWidth, dwClipHeight, g_d3dapp->m_mhandler, 
			g_d3dapp->m_matWorld, g_d3dapp->m_matView, g_d3dapp->m_matProj, PROJ_NORMAL, 1, 
			g_d3dapp->m_smallgband, calaspect, 0, 0 );
		if( ret ){
			DbgOut( "rdbplugin : RDBGetNormal : selem TransfomrDispData error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		***/

		ret = selem->TransformOnlyWorld( 0, g_d3dapp->m_mhandler, motid, frameno, g_d3dapp->m_matWorld, CALC_WORLDV, 0 );
		if( ret ){
			DbgOut( "rdbplugin : RDBGetNormal : selem TransformOnlyWorld error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		ret = selem->CalcNormalByWorldV();
		if( ret ){
			DbgOut( "rdbplugin : RDBGetNormal : selem CalcNormalByWorldV error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		// もとに戻す。
		if( savemotid >= 0 ){
			ret = g_d3dapp->m_mhandler->SetMotionFrameNo( g_d3dapp->m_shandler, savemotid, saveframeno, 1 );
			if( ret ){
				DbgOut( "rdbpluin : RDBGetNormal : mh SetMotionFrameNo error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			g_d3dapp->m_mhandler->SetNewPose( g_d3dapp->m_shandler );
		}

	}else{
		ret = selem->CalcInitialNormal();
		if( ret ){
			DbgOut( "rdbplugin : RDBGetNormal : selem CalcInitialNormal error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}
//////////////


	D3DXVECTOR3* normalptr;
	normalptr = new D3DXVECTOR3[ vnum ];
	if( !normalptr ){
		DbgOut( "rdbplugin : RDBGetNormal : normalptr alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	ret = selem->GetNormal( normalptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetNormal : selem GetNormal error !!!\n" );
		_ASSERT( 0 );
		delete [] normalptr;
		return 1;
	}

	int vno;
	for( vno = 0; vno < vnum; vno++ ){
		(nptr + vno)->x = (normalptr + vno)->x;
		(nptr + vno)->y = (normalptr + vno)->y;
		(nptr + vno)->z = (normalptr + vno)->z;
	}

	delete [] normalptr;

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetOrgVertNo( int hsid, int partno, int vertno, int* orgnoarray, int arrayleng, int* getnumptr )
{
	*getnumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	ret = g_d3dapp->m_shandler->GetOrgVertNo( partno, vertno, orgnoarray, arrayleng, getnumptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetOrgVertNo : shandler GetOrgVertNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	return 0;
}

RDBPLUGIN_EXPORT int RDBIsJoint( int hsid, int partno, int* isjoint )
{
	*isjoint = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBIsJoint : partno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );

	int isj;
	isj = selem->IsJoint();

	if( isj && (selem->type != SHDMORPH) ){
		*isjoint = 1;
	}else{
		*isjoint = 0;
	}

	return 0;
}


RDBPLUGIN_EXPORT int RDBSaveSigFile( int hsid, char* filename )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;

	if( !filename || !*filename ){
		DbgOut( "rdbplugin : RDBSaveSigFile : filename error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CSigFile* sigfile;
	sigfile = new CSigFile();
	if( !sigfile ){
		DbgOut( "rdbplugin : RDBSaveSigFile : sigfile alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	
	ret = sigfile->WriteSigFile( filename, g_d3dapp->m_thandler, g_d3dapp->m_shandler, g_d3dapp->m_mhandler );
	if( ret ){
		DbgOut( "rdbplugin : RDBSaveSigFile : sigfile WriteSigFile error !!!\n" );
		_ASSERT( 0 );
		delete sigfile;
		return 1;
	}

	delete sigfile;

	return 0;
}


//プログレスバー
RDBPLUGIN_EXPORT int RDBCreateProgressBar()
{
	::SendMessage( g_d3dapp->m_hWnd, WM_USER_CREATE_PB, 0, 0 );
	::UpdateWindow( g_d3dapp->m_hWnd );
	return 0;
}
RDBPLUGIN_EXPORT int RDBSetProgressBar( int pos )
{
	::SendMessage( g_d3dapp->m_hWnd, WM_USER_SET_PB, pos, 0 );
	::UpdateWindow( g_d3dapp->m_hWnd );
	return 0;
}
RDBPLUGIN_EXPORT int RDBDestroyProgressBar()
{
	::SendMessage( g_d3dapp->m_hWnd, WM_USER_DESTROY_PB, 0, 0 );
	return 0;
}


RDBPLUGIN_EXPORT int RDBGetMotionNum( int hsid, int* numptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		*numptr = 0;
		return 0;
	}

	if( hsid != 0 ){
		*numptr = 0;
		return 1;
	}

	int mhnum = g_d3dapp->m_mhandler->m_kindnum;
	if( mhnum == 0 ){
		*numptr = 0;
	}else{
		*numptr = mhnum - 1;//fill up motionを除く
	}

	return 0;
}


RDBPLUGIN_EXPORT int RDBGetMotionName( int hsid, int motid, char* motname )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		*motname = 0;
		return 0;
	}

	if( hsid != 0 ){
		*motname = 0;
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_mhandler->GetMotionName( motid, motname );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetMotionName : mh GetMotionName error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

//マテリアル
RDBPLUGIN_EXPORT int RDBGetMaterialNum( int hsid, int* matnumptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		*matnumptr = 0;
		return 0;
	}

	if( hsid != 0 ){
		*matnumptr = 0;
		return 1;
	}

	*matnumptr = g_d3dapp->m_shandler->m_materialcnt - 1;

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetMaterial( int hsid, int matno, RDBMaterial* matptr )
{

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	CMQOMaterial* curmat;
	curmat = g_d3dapp->m_shandler->GetMaterialFromNo( g_d3dapp->m_shandler->m_mathead, matno );
	if( !curmat ){
		DbgOut( "rdbplugin : sh GetMaterialFromNo mat NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( matptr ){
		matptr->materialno = curmat->materialno;
		matptr->legacyflag = curmat->legacyflag;//古いデータのとき１。パーツ単位からマテリアルを作成したときに１をセットする。
		strcpy_s( matptr->name, 256, curmat->name );

		matptr->a = curmat->col.a;

		matptr->diffuse.r = curmat->dif4f.r;
		matptr->diffuse.g = curmat->dif4f.g;
		matptr->diffuse.b = curmat->dif4f.b;

		matptr->specular.r = curmat->spc3f.r;
		matptr->specular.g = curmat->spc3f.g;
		matptr->specular.b = curmat->spc3f.b;

		matptr->ambient.r = curmat->amb3f.r;
		matptr->ambient.g = curmat->amb3f.g;
		matptr->ambient.b = curmat->amb3f.b;

		matptr->emissive.r = curmat->emi3f.r;
		matptr->emissive.g = curmat->emi3f.g;
		matptr->emissive.b = curmat->emi3f.b;

		matptr->power = curmat->power;

		strcpy_s( matptr->tex, 256, curmat->tex );
		strcpy_s( matptr->alpha, 256, curmat->alpha );
		strcpy_s( matptr->bump, 256, curmat->bump );

		matptr->shader = curmat->shader;

		matptr->transparent = curmat->transparent;
		matptr->texrule = curmat->texrule;
		matptr->blendmode = curmat->blendmode;

		matptr->uanime = curmat->uanime;
		matptr->vanime = curmat->vanime;
	/////
		matptr->exttexnum = curmat->exttexnum; //exttexnameの名前の数
		matptr->exttexmode = curmat->exttexmode; //重ねるか、パラパラアニメするかどうか。
		matptr->exttexrep = curmat->exttexrep; //繰り返しかどうか
		matptr->exttexstep = curmat->exttexstep; //何フレームごとに、変化するか。
		matptr->exttexname = curmat->exttexname;//名前のポインタの配列
		
	}


	return 0;
}
RDBPLUGIN_EXPORT int RDBGetMaterialNo( int hsid, int partno, int faceno, int* matnoptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		*matnoptr = 0;
		return 0;
	}

	if( hsid != 0 ){
		*matnoptr = 0;
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetMaterialNo( partno, faceno, matnoptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetMaterialNo : sh GetMaterialNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBGetSelectedJoint( int* hsidptr, int* jointnoptr )
{
	*hsidptr = -1;
	*jointnoptr = -1;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	int ret;
	D3DXVECTOR3 tempv;
	ret = g_motdlg->GetSelectedJointPos( jointnoptr, &tempv );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetSelectedJoint : motdlg GetSelectedJointPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	*hsidptr = 0;//!!!!!!!!!!!!!!

	return 0;
}

RDBPLUGIN_EXPORT int RDBSelectJoint( int hsid, int jointno )
{

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}
	if( hsid != 0 ){
		return 1;
	}

	if( (jointno <= 0) || (jointno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBSelectJoint : jointno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( jointno );
	if( !selem->IsJoint() ){
		DbgOut( "rdbplugin : RDBSelectJoint : not joint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	g_motdlg->m_motparamdlg->m_selectedno = jointno;
	g_d3dapp->m_selectedno = jointno;
	g_motdlg->OnUserSelchange( jointno );
	SendMessage( g_d3dapp->m_hWnd, WM_USER_SELCHANGE, (WPARAM)ID_MSG_FROM_MOTDLG, (LPARAM)jointno );

	g_motdlg->Redraw();

	return 0;
}


RDBPLUGIN_EXPORT int RDBCreateBone( int hsid, char* nameptr, int parentid, int* seriptr )
{
	*seriptr = -1;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	int parid = -1;
	CShdElem* parelem = 0;
	if( parentid < 0 ){
		ret = g_d3dapp->m_shandler->GetFirstParentNo( &parid );
		if( ret ){
			DbgOut( "rdbplugin : RDBCreateBone : sh GetFirstParentNo error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		parid = parentid;
	}

	if( (parid < 0) || (parid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBCreateBone : parentid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	parelem = (*g_d3dapp->m_shandler)( parid );
	_ASSERT( parelem );


	D3DXVECTOR3 newpos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	ret = g_d3dapp->CreateBone( 0, newpos, parelem, seriptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBCreateBone : app CreateBone error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

//	int CreateBone( char* nameptr, D3DXVECTOR3 newpos, CShdElem* parelem, int* seriptr );
//	int DelJoint( int delno, int restoreflag = 1 );

RDBPLUGIN_EXPORT int RDBDestroyBone( int hsid, int boneno )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (boneno <= 0) || (boneno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBDestroyBone : boneno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	int ret;

	ret = g_d3dapp->DelJoint( boneno, 1 );
	if( ret ){
		DbgOut( "rdbplugin : RDBDestroyBone : app DelJoint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBSetJointInitialPos( int hsid, int jointno, RDBPoint pos, int calcflag, int excludeflag )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	D3DXVECTOR3 setpos;
	setpos.x = pos.x;
	setpos.y = pos.y;
	setpos.z = pos.z;

	ret = g_d3dapp->m_shandler->ChangeJointInitialPos( jointno, setpos );
	if( ret ){
		DbgOut( "rdbplugin : SetJointInitialPos : sh ChangeJointInitialPos error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( calcflag ){
		ret = g_d3dapp->m_shandler->CalcInfElem( -1, 1, excludeflag );
		if( ret ){
			DbgOut( "rdbplugin : SetJointInitialPos : sh CalcInfElem error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	return 0;
}

//モーフ
RDBPLUGIN_EXPORT int RDBGetMorphBaseNum( int hsid, int* basenumptr )
{
	*basenumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	ret = g_d3dapp->m_shandler->GetMorphElem( 0, basenumptr, 0 );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetMorphBaseNum : sh GetMorphElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
//int CShdHandler::GetMorphElem( CShdElem** ppselem, int* numptr, int arrayleng )

RDBPLUGIN_EXPORT int RDBGetMorphBaseInfo( int hsid, RDBMORPHBASE* baseinfoptr, int arrayleng )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	CShdElem** ppmorphelem = 0;
	ppmorphelem = (CShdElem**)malloc( sizeof( CShdElem* ) * arrayleng );
	if( !ppmorphelem ){
		DbgOut( "rdbplugin : RDBGetMorphBaseInfo : ppmorphelem alloc error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	ZeroMemory( ppmorphelem, sizeof( CShdElem* ) * arrayleng );

	int getnum = 0;
	ret = g_d3dapp->m_shandler->GetMorphElem( ppmorphelem, &getnum, arrayleng );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetMorphBaseInfo : sh GetMorphElem error !!!\n" );
		_ASSERT( 0 );
		free( ppmorphelem );
		return 1;
	}

	if( arrayleng != getnum ){
		DbgOut( "rdbplugin : RDBGetMorphBaseInfo : arrayleng error !!!\n" );
		_ASSERT( 0 );
		free( ppmorphelem );
		return 1;
	}

	int morphcnt;
	for( morphcnt = 0; morphcnt < getnum; morphcnt++ ){
		RDBMORPHBASE* dstinfo = baseinfoptr + morphcnt;

		CShdElem* curmorph;
		curmorph = *( ppmorphelem + morphcnt );
		CMorph* morphptr;
		morphptr = curmorph->morph;
		if( !morphptr ){
			DbgOut( "rdbplugin : RDBGetMorphBaseInfo : morph NULL error !!!\n" );
			_ASSERT( 0 );
			free( ppmorphelem );
			return 1;
		}

		CShdElem* baseelem;
		baseelem = morphptr->m_baseelem;
		_ASSERT( baseelem );

		CTreeElem2* basetelem;
		basetelem = (*g_d3dapp->m_thandler)( baseelem->serialno );
		_ASSERT( basetelem );

		sprintf_s( dstinfo->name, 256, basetelem->name );
		dstinfo->dispno = baseelem->serialno;
		dstinfo->boneno = morphptr->m_boneseri;
		dstinfo->targetnum = morphptr->m_targetnum;

	}

	free( ppmorphelem );

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetMorphTargetInfo( int hsid, int baseid, RDBMORPHTARGET* targetinfoptr, int arrayleng )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	int ret;
	CShdElem* morphelem = 0;
	ret = g_d3dapp->m_shandler->GetMorphElem( &morphelem, baseid );
	if( ret || !morphelem ){
		DbgOut( "pluginsdk : RDBGetMorphTargetInfo : sh GetMorphElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMorph* morphptr;
	morphptr = morphelem->morph;
	_ASSERT( morphptr );

	if( arrayleng != morphptr->m_targetnum ){
		DbgOut( "pluginsdk : RDBGetMorphTargetInfo : arrayleng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int targetcnt;
	for( targetcnt = 0; targetcnt < morphptr->m_targetnum; targetcnt++ ){

		RDBMORPHTARGET* dstinfo = targetinfoptr + targetcnt;

		TARGETPRIM* tprim = morphptr->m_ptarget + targetcnt;
		int targetseri;
		targetseri = tprim->seri;

		CTreeElem2* telem;
		telem = (*g_d3dapp->m_thandler)( targetseri );

		strcpy_s( dstinfo->name, 256, telem->name );
		dstinfo->dispno = targetseri;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetMorphKeyNum( int hsid, int motid, int boneid, int baseid, int targetid, int* keynumptr )
{
	*keynumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBGetMorphKeyNum : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (boneid <= 0) || (boneid >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk : RDBGetMorphKeyNum : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (baseid <= 0) || (baseid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBGetMorphKeyNum : baseid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (targetid <= 0) || (targetid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBGetMorphKeyNum : targetid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMotionCtrl* mcptr;
	mcptr = (*g_d3dapp->m_mhandler)( boneid );
	_ASSERT( mcptr );
	if( !mcptr->IsJoint() ){
		DbgOut( "pluginsdk : RDBGetMorphKeyNum : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CMotionInfo* miptr;
	miptr = mcptr->motinfo;
	if( !miptr ){
		DbgOut( "pluginsdk : RDBGetMorphKeyNum : motioninfo NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CShdElem* baseelem;
	baseelem = (*g_d3dapp->m_shandler)( baseid );
	_ASSERT( baseelem );
	CShdElem* targetelem;
	targetelem = (*g_d3dapp->m_shandler)( targetid );
	_ASSERT( targetelem );

	int ret;
	ret = miptr->GetMMotAnimKeyAll( motid, baseelem, targetelem, 0, 0, keynumptr );
	if( ret ){
		DbgOut( "pluginsdk : RDBGetMorphKeyNum : mi GetMMotAnimKeyAll error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
RDBPLUGIN_EXPORT int RDBGetMorphKey( int hsid, int motid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr, int arrayleng )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBGetMorphKey : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (boneid <= 0) || (boneid >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk : RDBGetMorphKey : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (baseid <= 0) || (baseid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBGetMorphKey : baseid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (targetid <= 0) || (targetid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBGetMorphKey : targetid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMotionCtrl* mcptr;
	mcptr = (*g_d3dapp->m_mhandler)( boneid );
	_ASSERT( mcptr );
	if( !mcptr->IsJoint() ){
		DbgOut( "pluginsdk : RDBGetMorphKey : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CMotionInfo* miptr;
	miptr = mcptr->motinfo;
	if( !miptr ){
		DbgOut( "pluginsdk : RDBGetMorphKey : motioninfo NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CShdElem* baseelem;
	baseelem = (*g_d3dapp->m_shandler)( baseid );
	_ASSERT( baseelem );
	CShdElem* targetelem;
	targetelem = (*g_d3dapp->m_shandler)( targetid );
	_ASSERT( targetelem );

	int ret;
	int getnum = 0;
	ret = miptr->GetMMotAnimKeyAll( motid, baseelem, targetelem, 0, 0, &getnum );
	if( ret ){
		DbgOut( "pluginsdk : RDBGetMorphKey : mi GetMMotAnimKeyAll keynum error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( getnum != arrayleng ){
		DbgOut( "pluginsdk : RDBGetMorphKey : arrayleng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int tmpnum = 0;
	ret = miptr->GetMMotAnimKeyAll( motid, baseelem, targetelem, (E3DMORPHMOTION*)motioninfoptr, arrayleng, &tmpnum );
	if( ret || (tmpnum != arrayleng) ){
		DbgOut( "pluginsdk : RDBGetMorphKey : mi GetMMotAnimKeyAll keyval error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetCurMorphInfo( int hsid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		motioninfoptr->frameno = 0;
		motioninfoptr->blendrate = 0.0f;
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	if( (boneid <= 0) || (boneid >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk : RDBGetCurMorphInfo : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (baseid <= 0) || (baseid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBGetCurMorphInfo : baseid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (targetid <= 0) || (targetid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBGetCurMorphInfo : targetid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMotionCtrl* mcptr;
	mcptr = (*g_d3dapp->m_mhandler)( boneid );
	_ASSERT( mcptr );
	if( !mcptr->IsJoint() ){
		DbgOut( "pluginsdk : RDBGetCurMorphInfo : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CShdElem* baseelem;
	baseelem = (*g_d3dapp->m_shandler)( baseid );
	_ASSERT( baseelem );
	CShdElem* targetelem;
	targetelem = (*g_d3dapp->m_shandler)( targetid );
	_ASSERT( targetelem );

	motioninfoptr->frameno = mcptr->curframeno;//!!!!!!!!!

	CMMotElem* mmeptr = mcptr->GetCurMMotElem( baseelem );
	if( mmeptr ){
		motioninfoptr->blendrate = mmeptr->GetValue( targetelem );
	}else{
		motioninfoptr->blendrate = 0.0f;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBCreateMorph( int hsid, int baseid, int boneid )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (boneid <= 0) || (boneid >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk : RDBCreateMorph : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (baseid <= 0) || (baseid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBCreateMorph : baseid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = g_d3dapp->AddMorphFolder( baseid, boneid );
	if( ret ){
		DbgOut( "pluginsdk : RDBCreateMorph : app AddMorphFolder error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBAddMorphTarget( int hsid, int baseid, int targetid )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (baseid <= 0) || (baseid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBAddMorphTarget : baseid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (targetid <= 0) || (targetid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBAddMorphTarget : targetid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	int ret;
	CShdElem* morphelem = 0;
	ret = g_d3dapp->m_shandler->GetMorphElem( &morphelem, baseid );
	if( ret || !morphelem ){
		DbgOut( "pluginsdk : RDBAddMorphTarget : sh GetMorphElem error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	LRESULT lret;
	BOOL dummy;
	ret = g_d3dapp->m_sigeditwnd->OnUserSelchange( (long)targetid, 1 );
	_ASSERT( !ret );
	g_d3dapp->m_sigeditwnd->m_onitembutton = 0;
	g_d3dapp->m_sigeditwnd->m_clicked_lbutton = 1;//!!!!!!!!!!!!!!!
	lret = g_d3dapp->m_sigeditwnd->OnLButtonUP( 0, (WPARAM)morphelem->serialno, (LPARAM)targetid, dummy );
	if( lret ){
		DbgOut( "pluginsdk : RDBAddMorphTarget : sigeditwnd OnLButtonUP error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBSetMorphKey( int hsid, int motid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBSetMorphKey : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (boneid <= 0) || (boneid >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk : RDBSetMorphKey : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (baseid <= 0) || (baseid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBSetMorphKey : baseid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (targetid <= 0) || (targetid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBSetMorphKey : targetid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMotionCtrl* mcptr;
	mcptr = (*g_d3dapp->m_mhandler)( boneid );
	_ASSERT( mcptr );
	if( !mcptr->IsJoint() ){
		DbgOut( "pluginsdk : RDBSetMorphKey : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CMotionInfo* miptr;
	miptr = mcptr->motinfo;
	if( !miptr ){
		DbgOut( "pluginsdk : RDBSetMorphKey : motioninfo NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CShdElem* baseelem;
	baseelem = (*g_d3dapp->m_shandler)( baseid );
	_ASSERT( baseelem );
	CShdElem* targetelem;
	targetelem = (*g_d3dapp->m_shandler)( targetid );
	_ASSERT( targetelem );

	int ret;
	ret = miptr->SetMMotValue( motid, motioninfoptr->frameno, baseelem, targetelem, motioninfoptr->blendrate );
	if( ret ){
		DbgOut( "pluginsdk : RDBSetMorphKey : mi SetMMotValue error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBDestroyMorphKey( int hsid, int motid, int boneid, int frameno, int baseid )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 0;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBDestroyMorphKey : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (boneid <= 0) || (boneid >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk : RDBDestroyMorphKey : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (baseid <= 0) || (baseid >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "pluginsdk : RDBDestroyMorphKey : baseid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CMotionCtrl* mcptr;
	mcptr = (*g_d3dapp->m_mhandler)( boneid );
	_ASSERT( mcptr );
	if( !mcptr->IsJoint() ){
		DbgOut( "pluginsdk : RDBDestroyMorphKey : boneid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CMotionInfo* miptr;
	miptr = mcptr->motinfo;
	if( !miptr ){
		DbgOut( "pluginsdk : RDBDestroyMorphKey : motioninfo NULL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CShdElem* baseelem;
	baseelem = (*g_d3dapp->m_shandler)( baseid );
	_ASSERT( baseelem );

	int ret;
	if( frameno < 0 ){
		CMMotKey* mmkptr;
		CMMotKey* nextmmk;
		mmkptr = miptr->GetFirstMMotKey( motid );
		while( mmkptr ){
			nextmmk = mmkptr->next;
			ret = miptr->DeleteMMotKey( motid, mmkptr->frameno, baseelem );
			if( ret ){
				DbgOut( "pluginsdk : RDBDestroyMorphKey : mi DeleteMMotKey error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			mmkptr = nextmmk;
		}
	}else{
		ret = miptr->DeleteMMotKey( motid, frameno, baseelem );
		if( ret ){
			DbgOut( "pluginsdk : RDBDestroyMorphKey : mi DeleteMMotKey error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	return 0;
}

RDBPLUGIN_EXPORT int RDBSetMotionFrameNo( int hsid, int motid, int frameno )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBSetMotionFrameNo : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	int frameleng = 0;
	ret = RDBGetFrameLeng( hsid, motid, &frameleng );
	if( ret ){
		DbgOut( "pluginsdk : RDBSetMotionFrameNo : RDBGetFrameLeng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (frameno < 0) || (frameno >= frameleng) ){
		DbgOut( "pluginsdk : RDBSetMotionFrameNo : frameno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = g_motdlg->SetMotionFrameNo( motid, frameno );
	if( ret ){
		DbgOut( "pluginsdk : RDBSetMotionFrameNo : motdlg SetMotionFrameNo error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBSetKeyFrameSRT2( int hsid, int boneno, int motid, int frameno, 
	RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBSetKeyFrameSRT2 : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	int frameleng = 0;
	ret = RDBGetFrameLeng( hsid, motid, &frameleng );
	if( ret ){
		DbgOut( "pluginsdk : RDBSetKeyFrameSRT2 : RDBGetFrameLeng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (frameno < 0) || (frameno >= frameleng) ){
		DbgOut( "pluginsdk : RDBSetKeyFrameSRT2 : frameno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (boneno <= 0) || (boneno >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk :  RDBSetKeyFrameSRT2 : boneno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	CMotionCtrl* mcptr;
	mcptr = (*g_d3dapp->m_mhandler)( boneno );
	if( !mcptr->IsJoint() ){
		DbgOut( "pluginsdk : RDBSetKeyFrameSRT2 : not joint error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CQuaternion rot;
	rot.SetParams( rotptr->w, rotptr->x, rotptr->y, rotptr->z );


	CMotionPoint2* mpptr;
	mpptr = mcptr->IsExistMotionPoint( motid, frameno );
	if( mpptr ){
		mpptr->m_q = rot;
		mpptr->m_mvx = traptr->x;
		mpptr->m_mvy = traptr->y;
		mpptr->m_mvz = traptr->z;
		mpptr->m_scalex = scaleptr->x;
		mpptr->m_scaley = scaleptr->y;
		mpptr->m_scalez = scaleptr->z;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBFillUpMotion( int hsid, int motid, int topboneno, int startframe, int endframe )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBFillUpMotion : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	int frameleng = 0;
	ret = RDBGetFrameLeng( hsid, motid, &frameleng );
	if( ret ){
		DbgOut( "pluginsdk : RDBFillUpMotion : RDBGetFrameLeng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( startframe >= frameleng ){
		DbgOut( "pluginsdk : RDBFillUpMotion : startframe error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (endframe >= frameleng) || (endframe < startframe) ){
		DbgOut( "pluginsdk : RDBFillUpMotion : endframe error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (topboneno <= 0) || (topboneno >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk :  RDBFillUpMotion : boneno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	ret = g_d3dapp->m_mhandler->FillUpMotion( g_d3dapp->m_shandler, motid, topboneno, startframe, endframe, 1 );
	if( ret ){
		DbgOut( "pluginsdk : RDBFillUpMotion : mh FillUpMotion error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
RDBPLUGIN_EXPORT int RDBGetSplineParams( int hsid, int boneno, int motid, int frameno, RDBSPPARAMSET* sppptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBGetSplineParams : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	int frameleng = 0;
	ret = RDBGetFrameLeng( hsid, motid, &frameleng );
	if( ret ){
		DbgOut( "pluginsdk : RDBGetSplineParams : RDBGetFrameLeng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (frameno < 0) || (frameno >= frameleng) ){
		DbgOut( "pluginsdk : RDBGetSplineParams : startframe error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (boneno <= 0) || (boneno >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk :  RDBGetSplineParams : boneno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	CMotionPoint2* mpptr;
	ret = g_d3dapp->m_mhandler->IsExistMotionPoint( motid, boneno, frameno, &mpptr );
	if( (ret == 0) && mpptr && mpptr->m_spp ){
		MoveMemory( &(sppptr->m_rotparam), &(mpptr->m_spp->m_rotparam), sizeof( QUASPPARAM ) );
		MoveMemory( &(sppptr->m_mvparam), &(mpptr->m_spp->m_mvparam), sizeof( QUASPPARAM ) );
		MoveMemory( &(sppptr->m_scparam), &(mpptr->m_spp->m_scparam), sizeof( QUASPPARAM ) );
		ZeroMemory( sppptr->reserved, sizeof( float ) * 4 );
	}else{
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBSetSplineParams( int hsid, int boneno, int motid, int frameno, int setflag, RDBSPPARAMSET* sppptr )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBSetSplineParams : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	int frameleng = 0;
	ret = RDBGetFrameLeng( hsid, motid, &frameleng );
	if( ret ){
		DbgOut( "pluginsdk : RDBSetSplineParams : RDBGetFrameLeng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( (frameno < 0) || (frameno >= frameleng) ){
		DbgOut( "pluginsdk : RDBSetSplineParams : startframe error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( (boneno <= 0) || (boneno >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk :  RDBSetSplineParams : boneno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	CMotionPoint2* mpptr;
	ret = g_d3dapp->m_mhandler->IsExistMotionPoint( motid, boneno, frameno, &mpptr );
	if( (ret == 0) && mpptr && mpptr->m_spp ){
		if( setflag & 1 ){
			MoveMemory( &(mpptr->m_spp->m_rotparam), &(sppptr->m_rotparam), sizeof( QUASPPARAM ) );
		}
		if( setflag & 2 ){
			MoveMemory( &(mpptr->m_spp->m_mvparam), &(sppptr->m_mvparam), sizeof( QUASPPARAM ) );
		}
		if( setflag & 4 ){
			MoveMemory( &(mpptr->m_spp->m_scparam), &(sppptr->m_scparam), sizeof( QUASPPARAM ) );
		}
	}else{
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBCalcSplineParams( int hsid, int boneno, int motid )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler || !g_motdlg ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( g_d3dapp->m_mhandler->m_kindnum <= 0 ){
		_ASSERT( 0 );
		return 1;
	}
	if( (motid < 0) || (motid >= g_d3dapp->m_mhandler->m_kindnum) ){
		DbgOut( "pluginsdk : RDBCalcSplineParams : motid error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( ( (boneno <= 0) && (boneno != -1) ) || (boneno >= g_d3dapp->m_mhandler->s2mot_leng) ){
		DbgOut( "pluginsdk :  RDBCalcSplineParams : boneno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = g_motdlg->m_motparamdlg->CalcSplineParam( 1, boneno, motid );
	if( ret ){
		DbgOut( "pluginsdk : RDBCalcSplineParams : mpdlg CalcSplineParam error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	return 0;
}




RDBPLUGIN_EXPORT int RDBGetToon1MaterialNum( int hsid, int partno, int* matnumptr )
{
	*matnumptr = 0;

	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBGetToon1MaterialNum : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	ret = selem->GetToon1MaterialNum( matnumptr );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetToon1MaterialNum : se GetToon1MaterialNum error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBGetToon1Material( int hsid, int partno, E3DTOON1MATERIAL* toon1ptr, int arrayleng )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBGetToon1Material : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	int getnum = 0;
	ret = selem->GetToon1Material( toon1ptr, arrayleng, &getnum );
	if( ret ){
		DbgOut( "rdbplugin : RDBGetToon1Material : se GetToon1Material error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( arrayleng != getnum ){
		DbgOut( "rdbplugin : RDBGetToon1Material : arrayleng error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBSetToon1Diffuse( int hsid, int partno, char* matname, RDBColor3f diffuse )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBSetToon1Diffuse : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	ret = selem->SetToon1Diffuse( matname, diffuse );
	if( ret ){
		DbgOut( "rdbplugin : RDBSetToon1Diffuse : se SetToon1Diffuse error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


	return 0;
}
RDBPLUGIN_EXPORT int RDBSetToon1Specular( int hsid, int partno, char* matname, RDBColor3f specular )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBSetToon1Specular : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	ret = selem->SetToon1Specular( matname, specular );
	if( ret ){
		DbgOut( "rdbplugin : RDBSetToon1Specular : se SetToon1Specular error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBSetToon1Ambient( int hsid, int partno, char* matname, RDBColor3f ambient )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBSetToon1Ambient : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	ret = selem->SetToon1Ambient( matname, ambient );
	if( ret ){
		DbgOut( "rdbplugin : RDBSetToon1Ambient : se SetToon1Ambient error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBSetToon1NL( int hsid, int partno, char* matname, float darknl, float brightnl )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBSetToon1NL : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	ret = selem->SetToon1NL( matname, darknl, brightnl );
	if( ret ){
		DbgOut( "rdbplugin : RDBSetToon1NL : se SetToon1NL error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
RDBPLUGIN_EXPORT int RDBSetToon1Edge0( int hsid, int partno, char* matname, RDBColor3f col, int validflag, int invflag, float width )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBSetToon1Edge0 : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	ret = selem->SetToon1Edge0( matname, col, validflag, invflag, width );
	if( ret ){
		DbgOut( "rdbplugin : RDBSetToon1Edge0 : se SetToon1Edge0 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

RDBPLUGIN_EXPORT int RDBSetToon1Name( int hsid, int partno, char* oldname, char* newname )
{
	if( !g_d3dapp->m_thandler || !g_d3dapp->m_shandler || !g_d3dapp->m_mhandler ){
		return 0;
	}

	if( hsid != 0 ){
		return 1;
	}

	if( (partno < 0) || (partno >= g_d3dapp->m_shandler->s2shd_leng) ){
		DbgOut( "rdbplugin : RDBSetToon1Name : paratno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CShdElem* selem;
	selem = (*g_d3dapp->m_shandler)( partno );
	_ASSERT( selem );

	int ret;
	ret = selem->SetToon1Name( oldname, newname );
	if( ret ){
		DbgOut( "rdbplugin : RDBSetToon1Name : se SetToon1Name error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
