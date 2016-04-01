//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "RDBPlugin.h"

#include <crtdbg.h> // <---- _ASSERTマクロ


///////////////////

//Version
#if RDBPLUGIN_VERSION >= 2005
	int (*RDBGetVersion)( int* verptr );
#endif

//ID
int (*RDBGetCurrentHSID)( int* hsidptr );
int (*RDBGetCurrentMotID)( int hsid, int* motidptr );
#if RDBPLUGIN_VERSION >= 5214
	int (*RDBGetSelectedJoint)( int* hsidptr, int* jointnoptr );
#endif
#if RDBPLUGIN_VERSION >= 5505
	int (*RDBSelectJoint)( int hsid, int jointno );
#endif


//階層
int (*RDBGetParent)( int hsid, int partno, int* parentnoptr );
int (*RDBGetChild)( int hsid, int partno, int* childnoptr, int arrayleng, int* childnumptr );

//名前
int (*RDBGetName)( int hsid, int partno, char* nameptr, int strleng );
int (*RDBGetPartNoByName)( int hsid, char* nameptr, int isjoint, int* partnoptr );

//表示オブジェクト名前、情報
int (*RDBGetDispObjNum)( int hsid, int* numptr );
int (*RDBGetDispObjInfo)( int hsid, int dispobjmaxnum, char* dispobjname, int* dispobjinfo, int* getnumptr );
int (*RDBGetClockwise)( int hsid, int partno, int* clockwiseptr );

//ジョイント名前、情報、階層
int (*RDBGetJointNum)( int hsid, int* numptr );
int (*RDBGetJointInfo)( int hsid, int jointmaxnum, char* jointname, int* jointinfo, int* getnumptr );
#if RDBPLUGIN_VERSION >= 2005
	int (*RDBIsJoint)( int hsid, int partno, int* isjoint );
#endif
#if RDBPLUGIN_VERSION >= 5214
	int (*RDBCreateBone)( int hsid, char* nameptr, int parentid, int* seriptr );
	int (*RDBDestroyBone)( int hsid, int boneno );
	int (*RDBSetJointInitialPos)( int hsid, int jointno, RDBPoint pos, int calcflag, int excludeflag );
#endif
//頂点位置
int (*RDBGetVertNumOfPart)( int hsid, int partno, int* numptr );
int (*RDBGetVertPos)( int hsid, int partno, int vertno, RDBPoint* posptr );
int (*RDBGetVertPos2)( int hsid, int partno, int vertno, int motid, int frameno, RDBPoint* posptr );
int (*RDBGetVertNoOfFace)( int hsid, int partno, int faceno, int* vptr1, int* vptr2, int* vptr3 );
int (*RDBGetSamePosVert)( int hsid, int partno, int vertno, int* samenoptr, int arrayleng, int* samenumptr );
#if RDBPLUGIN_VERSION >= 1077
	int (*RDBGetNormal)( int hsid, int partno, int motid, int frameno, RDBPoint* nptr, int arrayleng );
#endif
#if RDBPLUGIN_VERSION >= 1115
	int (*RDBGetOrgVertNo)( int hsid, int partno, int vertno, int* orgnoarray, int arrayleng, int* getnumptr );
#endif

//頂点影響度
int (*RDBGetInfElemNum)( int hsid, int partno, int vertno, int* numptr );
int (*RDBGetInfElem)( int hsid, int partno, int vertno, int infno, int* bonenoptr, float* rateptr );
#if RDBPLUGIN_VERSION >= 1115
	int (*RDBGetInfElem2)( int hsid, int partno, int vertno, int infno, int* bonenoptr, int* childnoptr, float* rateptr );
	int (*RDBSetInfElem)( int hsid, int partno, int vertno, int childjointno, int calcmode, int paintmode, int normalizeflag, float userrate, float directval );
	int (*RDBChkIM2Status)( int hsid, int* status );
#endif

//面、インデックス
int (*RDBGetFaceNum)( int hsid, int partno, int* numptr );
int (*RDBGetTriIndices)( int hsid, int partno, WORD* buf, int wordnum, int* setwordnumptr, int* revflagptr );
#if RDBPLUGIN_VERSION >= 5100
	int (*RDBGetTriIndicesInt)( int hsid, int partno, int* buf, int intnum, int* setintnumptr, int* revflagptr );
#endif

//色
int (*RDBGetAlpha)( int hsid, int partno, int vertno, int* alphaptr );
int (*RDBGetDiffuse)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
int (*RDBGetAmbient)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
int (*RDBGetSpecular)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
#if RDBPLUGIN_VERSION >= 1079
	int (*RDBGetEmissive)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
	int (*RDBGetSpecularPower)( int hsid, int partno, int vertno, float* powerptr );
	int (*RDBGetShaderType)( int hsid, int* shaderptr, int* overflowptr );
#endif

//テクスチャ
int (*RDBGetTextureInfo)( int hsid, int partno, char* nameptr, int* repuptr, int* repvptr, float* animuptr, float* animvptr, int* transparentptr );
int (*RDBGetExtTextureInfo)( int hsid, int partno, char* extnameptr, int* texnumptr, int* texmodeptr, int* repflagptr, int* stepptr );
#if RDBPLUGIN_VERSION >= 1077
	int (*RDBGetUV)( int hsid, int partno, int vertno, float* uptr, float* vptr );
#endif

//ボーン位置、モーション
int (*RDBGetBonePos)( int hsid, int boneno, int poskind, int motid, int frameno, RDBPoint* posptr );
int (*RDBGetBoneMatrix)( int hsid, int boneno, int motid, int frameno, int mode, RDBMatrix* matrixptr );
int (*RDBGetBoneQuaternion)( int hsid, int boneno, int motid, int frameno, int mode, RDBQuaternion* qptr );
int (*RDBGetFrameLeng)( int hsid, int motid, int* lengptr );
int (*RDBGetKeyFrameNo)( int hsid, int boneno, int motid, int* framearray, int arrayleng, int* framenumptr );
int (*RDBGetKeyFrameSRT)( int hsid, int boneno, int motid, int frameno, RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr );
int (*RDBGetOffsetMatrix)( int hsid, int boneno, RDBMatrix* matptr );
int (*RDBGetCurrentFrameNo)( int hsid, int* framenoptr );
#if RDBPLUGIN_VERSION >= 3040
	int (*RDBGetMotionNum)( int hsid, int* numptr );
	int (*RDBGetMotionName)( int hsid, int motid, char* motname );
	int (*RDBGetKeyFrameSRT2)( int hsid, int boneno, int motid, int frameno, RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr );
#endif
#if RDBPLUGIN_VERSION >= 5217
	int (*RDBSetMotionFrameNo)( int hsid, int motid, int frameno );
#endif
#if RDBPLUGIN_VERSION >= 5505
	int (*RDBSetKeyFrameSRT2)( int hsid, int boneno, int motid, int frameno, 
			RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr );
	int (*RDBFillUpMotion)( int hsid, int motid, int topboneno, int startframe, int endframe );
	int (*RDBGetSplineParams)( int hsid, int boneno, int motid, int frameno, RDBSPPARAMSET* sppptr );
	int (*RDBSetSplineParams)( int hsid, int boneno, int motid, int frameno, int setflag, RDBSPPARAMSET* sppptr );
	int (*RDBCalcSplineParams)( int hsid, int boneno, int motid );
#endif



//デバッグ出力
int (*RDBDbgOut)( char* lpFormat, ... );


//ファイル
#if RDBPLUGIN_VERSION >= 1115
	int (*RDBSaveSigFile)( int hsid, char* filename );
#endif


//プログレスバー
#if RDBPLUGIN_VERSION >= 1117
	int (*RDBCreateProgressBar)();
	int (*RDBSetProgressBar)( int pos );
	int (*RDBDestroyProgressBar)();
#endif

//マテリアル
#if RDBPLUGIN_VERSION >= 4001
	int (*RDBGetMaterialNum)( int hsid, int* matnumptr );
	int (*RDBGetMaterial)( int hsid, int matno, RDBMaterial* matptr );
	int (*RDBGetMaterialNo)( int hsid, int partno, int faceno, int* matnoptr );
#endif


//モーフ
#if RDBPLUGIN_VERSION >= 5217
	int (*RDBGetMorphBaseNum)( int hsid, int* basenumptr );
	int (*RDBGetMorphBaseInfo)( int hsid, RDBMORPHBASE* baseinfoptr, int arrayleng );
	int (*RDBGetMorphTargetInfo)( int hsid, int baseid, RDBMORPHTARGET* targetinfoptr, int arrayleng );
	int (*RDBGetMorphKeyNum)( int hsid, int motid, int boneid, int baseid, int targetid, int* keynumptr );
	int (*RDBGetMorphKey)( int hsid, int motid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr, int arrayleng );
	int (*RDBGetCurMorphInfo)( int hsid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr );
	int (*RDBCreateMorph)( int hsid, int baseid, int boneid );
	int (*RDBAddMorphTarget)( int hsid, int baseid, int targetid );
	int (*RDBSetMorphKey)( int hsid, int motid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr );
	int (*RDBDestroyMorphKey)( int hsid, int motid, int boneid, int frameno, int baseid );
#endif

//トゥーン
#if RDBPLUGIN_VERSION >= 5220
	int (*RDBGetToon1MaterialNum)( int hsid, int partno, int* matnumptr );
	int (*RDBGetToon1Material)( int hsid, int partno, E3DTOON1MATERIAL* toon1ptr, int arrayleng );
	int (*RDBSetToon1Name)( int hsid, int partno, char* oldname, char* newname );
	int (*RDBSetToon1Diffuse)( int hsid, int partno, char* matname, RDBColor3f diffuse );
	int (*RDBSetToon1Specular)( int hsid, int partno, char* matname, RDBColor3f specular );
	int (*RDBSetToon1Ambient)( int hsid, int partno, char* matname, RDBColor3f ambient );
	int (*RDBSetToon1NL)( int hsid, int partno, char* matname, float darknl, float brightnl );
	int (*RDBSetToon1Edge0)( int hsid, int partno, char* matname, RDBColor3f col, int validflag, int invflag, float width );
#endif
//////////////////


RDBPLUGIN_EXPORT int RDBCheckVersion( DWORD exe_version, DWORD* pluginver )
{
	
#if RDBPLUGIN_VERSION == 1077
	if( exe_version < 1077 )
		*pluginver = 0;// !!!!!!!!!!! 古いRokDeBone2では、使用不能
#endif

	*pluginver = RDBPLUGIN_VERSION;


	return 0;
}


#define GetAdr(proc) \
	*(FARPROC *)&proc = GetProcAddress(hModule, #proc); \
	if(proc == NULL){ _ASSERT( 0 ); goto RDBINIT_EXIT; }



RDBPLUGIN_EXPORT int RDBInit( char *exe_name )
{
	HMODULE hModule = LoadLibrary( exe_name );
	if(hModule == NULL)
		return 1;

	int result = 1;


	GetAdr(RDBGetCurrentHSID);
	GetAdr(RDBGetCurrentMotID);
	GetAdr(RDBGetParent);
	GetAdr(RDBGetChild);
	GetAdr(RDBGetName);
	GetAdr(RDBGetPartNoByName);
	GetAdr(RDBGetDispObjNum);
	GetAdr(RDBGetDispObjInfo);
	GetAdr(RDBGetClockwise);
	GetAdr(RDBGetJointNum);
	GetAdr(RDBGetJointInfo);
	GetAdr(RDBGetVertNumOfPart);
	GetAdr(RDBGetVertPos);
	GetAdr(RDBGetVertPos2);
	GetAdr(RDBGetVertNoOfFace);
	GetAdr(RDBGetSamePosVert);
	GetAdr(RDBGetInfElemNum);
	GetAdr(RDBGetInfElem);
	GetAdr(RDBGetFaceNum);
	GetAdr(RDBGetTriIndices);
	GetAdr(RDBGetAlpha);
	GetAdr(RDBGetDiffuse);
	GetAdr(RDBGetAmbient);
	GetAdr(RDBGetSpecular);
	GetAdr(RDBGetTextureInfo);
	GetAdr(RDBGetExtTextureInfo);
	GetAdr(RDBGetBonePos);
	GetAdr(RDBGetBoneMatrix);
	GetAdr(RDBGetBoneQuaternion);
	GetAdr(RDBGetFrameLeng);
	GetAdr(RDBGetKeyFrameNo);
	GetAdr(RDBGetKeyFrameSRT);
	GetAdr(RDBGetOffsetMatrix);
	GetAdr(RDBGetCurrentFrameNo);
	GetAdr(RDBDbgOut);

#if RDBPLUGIN_VERSION >= 1077
	GetAdr(RDBGetNormal);
	GetAdr(RDBGetUV);
#endif

#if RDBPLUGIN_VERSION >= 1079
	GetAdr(RDBGetEmissive);
	GetAdr(RDBGetSpecularPower);
	GetAdr(RDBGetShaderType);
#endif

#if RDBPLUGIN_VERSION >= 1115
	GetAdr(RDBGetOrgVertNo);
	GetAdr(RDBGetInfElem2);
	GetAdr(RDBChkIM2Status);
	GetAdr(RDBSetInfElem);
	GetAdr(RDBSaveSigFile);
#endif

#if RDBPLUGIN_VERSION >= 1117
	GetAdr(RDBCreateProgressBar);
	GetAdr(RDBSetProgressBar);
	GetAdr(RDBDestroyProgressBar);
#endif

#if RDBPLUGIN_VERSION >= 2005
	GetAdr(RDBGetVersion);
	GetAdr(RDBIsJoint);
#endif

#if RDBPLUGIN_VERSION >= 3040
	GetAdr(RDBGetMotionNum);
	GetAdr(RDBGetMotionName);
	GetAdr(RDBGetKeyFrameSRT2);
#endif

#if RDBPLUGIN_VERSION >= 4001
	GetAdr(RDBGetMaterialNum);
	GetAdr(RDBGetMaterial);
	GetAdr(RDBGetMaterialNo);
#endif

#if RDBPLUGIN_VERSION >= 5100
	GetAdr(RDBGetTriIndicesInt);
#endif

#if RDBPLUGIN_VERSION >= 5214
	GetAdr(RDBGetSelectedJoint);
	GetAdr(RDBCreateBone);
	GetAdr(RDBDestroyBone);
	GetAdr(RDBSetJointInitialPos);
#endif

#if RDBPLUGIN_VERSION >= 5217
	GetAdr(RDBSetMotionFrameNo);
	GetAdr(RDBGetMorphBaseNum);
	GetAdr(RDBGetMorphBaseInfo);
	GetAdr(RDBGetMorphTargetInfo);
	GetAdr(RDBGetMorphKeyNum);
	GetAdr(RDBGetMorphKey);
	GetAdr(RDBGetCurMorphInfo);
	GetAdr(RDBCreateMorph);
	GetAdr(RDBAddMorphTarget);
	GetAdr(RDBSetMorphKey);
	GetAdr(RDBDestroyMorphKey);
#endif

#if RDBPLUGIN_VERSION >= 5220
	GetAdr(RDBGetToon1MaterialNum);
	GetAdr(RDBGetToon1Material);
	GetAdr(RDBSetToon1Name);
	GetAdr(RDBSetToon1Diffuse);
	GetAdr(RDBSetToon1Specular);
	GetAdr(RDBSetToon1Ambient);
	GetAdr(RDBSetToon1NL);
	GetAdr(RDBSetToon1Edge0);
#endif

#if RDBPLUGIN_VERSION >= 5505
	GetAdr(RDBSetKeyFrameSRT2)
	GetAdr(RDBSelectJoint)
	GetAdr(RDBFillUpMotion)
	GetAdr(RDBGetSplineParams)
	GetAdr(RDBSetSplineParams)
	GetAdr(RDBCalcSplineParams)
#endif

	result = 0;

RDBINIT_EXIT:
	FreeLibrary(hModule);
	return result;
}



