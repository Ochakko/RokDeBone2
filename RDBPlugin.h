#ifndef RDBPLUGINH
#define RDBPLUGINH


#ifndef RDBPLUGIN_VERSION
#define RDBPLUGIN_VERSION	5505
#endif


#define RDBPLUGIN_EXPORT extern "C" __declspec(dllexport)
#define RDBPLUGIN_API	extern "C" 

#include <math.h>

/////////////////////////////////////////////
// RDBPointは、メタセコイアのプラグインＳＤＫの、MQPointと同じものです。
/////////////////////////////////////////////

typedef struct RDBPoint
{
public:
	float x,y,z;

	// constructor
	inline RDBPoint() { }
	inline RDBPoint(float nx, float ny, float nz) {x=nx; y=ny; z=nz;}

	// operator
	inline RDBPoint& operator = (const RDBPoint& p)
		{x=p.x; y=p.y; z=p.z; return *this;}
	inline RDBPoint& operator = (const float& s)
		{x=y=z=s; return *this;}
	inline RDBPoint& operator += (const RDBPoint& p)
		{x+=p.x; y+=p.y; z+=p.z; return *this;}
	inline RDBPoint& operator += (const float& s)
		{x+=s; y+=s; z+=s; return *this;}
	inline RDBPoint& operator -= (const RDBPoint& p)
		{x-=p.x; y-=p.y; z-=p.z; return *this;}
	inline RDBPoint& operator -= (const float& s)
		{x-=s; y-=s; z-=s; return *this;}
	inline RDBPoint& operator *= (const RDBPoint& p)
		{x*=p.x; y*=p.y; z*=p.z; return *this;}
	inline RDBPoint& operator *= (float s)
		{x*=s; y*=s; z*=s; return *this;}
	inline RDBPoint& operator /= (float s)
		{x/=s; y/=s; z/=s; return *this;}
	friend inline RDBPoint operator + (const RDBPoint& p1, const RDBPoint& p2)
		{return RDBPoint(p1.x+p2.x, p1.y+p2.y, p1.z+p2.z);}
	friend inline RDBPoint operator + (const RDBPoint& p, const float& s)
		{return RDBPoint(p.x+s, p.y+s, p.z+s);}
	friend inline RDBPoint operator + (const float& s, const RDBPoint& p)
		{return RDBPoint(p.x+s, p.y+s, p.z+s);}
	friend inline RDBPoint operator - (const RDBPoint& p1, const RDBPoint& p2)
		{return RDBPoint(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);}
	friend inline RDBPoint operator - (const RDBPoint& p, const float& s)
		{return RDBPoint(p.x-s, p.y-s, p.z-s);}
	friend inline RDBPoint operator - (const float& s, const RDBPoint& p)
		{return RDBPoint(s-p.x, s-p.y, s-p.z);}
	friend inline RDBPoint operator * (const RDBPoint& p1, const RDBPoint& p2)
		{return RDBPoint(p1.x*p2.x, p1.y*p2.y, p1.z*p2.z);}
	friend inline RDBPoint operator * (const RDBPoint& p, const float& s)
		{return RDBPoint(p.x*s, p.y*s, p.z*s);}
	friend inline RDBPoint operator * (const float& s, const RDBPoint& p)
		{return RDBPoint(p.x*s, p.y*s, p.z*s);}
	friend inline RDBPoint operator / (const RDBPoint& p, const float& s)
		{return RDBPoint(p.x/s, p.y/s, p.z/s);}
	friend inline bool operator == (const RDBPoint& p1, const RDBPoint& p2)
		{return (p1.x==p2.x && p1.y==p2.y && p1.z==p2.z);}
	friend inline bool operator != (const RDBPoint& p1, const RDBPoint& p2)
		{return (p1.x!=p2.x || p1.y!=p2.y || p1.z!=p2.z);}

	inline void zero() { x=y=z=0.0f; }
	inline void one() { x=y=z=1.0f; }
	inline float norm() const { return x*x + y*y + z*z; }
	inline float abs() const { return (float)sqrt(norm()); }
	inline void normalize() {
		float a = abs();
		if(a > 0.0f) *this /= a;
		else zero();
	}
} RDBPoint;

typedef struct RDBMatrix
{
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	// constructor
	inline RDBMatrix(){
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
	}
	
	inline RDBMatrix& operator = (const RDBMatrix& m)
	{
		_11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
		_21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
		_31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
		_41 = m._41; _42 = m._42; _43 = m._43; _44 = m._44;
		return *this;
	}

} RDBMatrix;

typedef struct RDBQuaternion
{
public:
	float x, y, z, w;
	
	inline RDBQuaternion(){
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	inline RDBQuaternion& operator = (const RDBQuaternion& q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
		return *this;
	}

} RDBQuaternion;

#ifndef USERCOEFH
typedef struct RDBColor3f
{
public:
	float r;
	float g;
	float b;
} RDBColor3f;
#endif

#ifndef USERCOEFH
enum {
	INDX,
	INDY,
	INDZ,
	INDMAX
};
#endif

typedef struct tag_rdbspctrl
{
	float spt;
	float spdist;
	float distmax;
	float sppoint[2];
	float tmin;
	float tmax;
	int reserved[2];
}RDBSPCTRL;

typedef struct tag_rdbspparam
{
	RDBSPCTRL ctrlS[INDMAX];
	RDBSPCTRL ctrlE[INDMAX];
	int syncflag;
	int lockflag;
	int reserved[3];
}RDBSPPARAM;

typedef struct tag_rdbspparamset
{
	RDBSPPARAM m_rotparam;
	RDBSPPARAM m_mvparam;
	RDBSPPARAM m_scparam;
	int reserved[4];
} RDBSPPARAMSET;

typedef struct RDBMaterial
{
public:
	int materialno;
	int legacyflag;//古いデータのとき１。パーツ単位からマテリアルを作成したときに１をセットする。
	char name[256];

	float a;//透明度
	RDBColor3f diffuse;
	RDBColor3f specular;
	RDBColor3f ambient;
	RDBColor3f emissive;
	float power;
	char tex[256];
	char alpha[256];
	char bump[256];

	int shader;

	int transparent;
	int texrule;
	int blendmode;

	float uanime;
	float vanime;
/////
	unsigned char exttexnum; //exttexnameの名前の数
	unsigned char exttexmode; //重ねるか、パラパラアニメするかどうか。
	unsigned char exttexrep; //繰り返しかどうか
	unsigned char exttexstep; //何フレームごとに、変化するか。
	char** exttexname;//名前のポインタの配列
} RDBMaterial;

#ifndef USERCOEFH
	typedef struct tag_morphbase
	{
		char name[256];
		int dispno;
		int boneno;
		int targetnum;
	}RDBMORPHBASE, E3DMORPHBASE;

	typedef struct tag_morphtarget
	{
		char name[256];
		int dispno;
	}RDBMORPHTARGET, E3DMORPHTARGET;

	typedef struct tag_morphmotion
	{
		int frameno;
		float blendrate;
	}RDBMORPHMOTION, E3DMORPHMOTION;

typedef struct tag_e3dtoon1material
{
	char name[32];

	RDBColor3f diffuse;
	RDBColor3f ambient;
	RDBColor3f specular;

	float darknl;//法線とライトの内積の閾値
	float brightnl;

	RDBColor3f edgecol0;
	int edgevalid0;
	int edgeinv0;
	float edgewidth0;	
}E3DTOON1MATERIAL;

#endif

#ifndef RDBPLUGIN_EXPORTS
//Version
RDBPLUGIN_API int (*RDBGetVersion)( int* verptr );

//ID
RDBPLUGIN_API int (*RDBGetCurrentHSID)( int* hsidptr );
RDBPLUGIN_API int (*RDBGetCurrentMotID)( int hsid, int* motidptr );
RDBPLUGIN_API int (*RDBGetSelectedJoint)( int* hsidptr, int* jointnoptr );
RDBPLUGIN_API int (*RDBSelectJoint)( int hsid, int jointno );

//階層
RDBPLUGIN_API int (*RDBGetParent)( int hsid, int partno, int* parentnoptr );
RDBPLUGIN_API int (*RDBGetChild)( int hsid, int partno, int* childnoptr, int arrayleng, int* childnumptr );

//名前
RDBPLUGIN_API int (*RDBGetName)( int hsid, int partno, char* nameptr, int strleng );
RDBPLUGIN_API int (*RDBGetPartNoByName)( int hsid, char* nameptr, int isjoint, int* partnoptr );

//表示オブジェクト名前、情報
RDBPLUGIN_API int (*RDBGetDispObjNum)( int hsid, int* numptr );
RDBPLUGIN_API int (*RDBGetDispObjInfo)( int hsid, int dispobjmaxnum, char* dispobjname, int* dispobjinfo, int* getnumptr );
RDBPLUGIN_API int (*RDBGetClockwise)( int hsid, int partno, int* clockwiseptr );

//ジョイント名前、情報、階層
RDBPLUGIN_API int (*RDBGetJointNum)( int hsid, int* numptr );
RDBPLUGIN_API int (*RDBGetJointInfo)( int hsid, int jointmaxnum, char* jointname, int* jointinfo, int* getnumptr );
RDBPLUGIN_API int (*RDBIsJoint)( int hsid, int partno, int* isjoint );
RDBPLUGIN_API int (*RDBCreateBone)( int hsid, char* nameptr, int parentid, int* seriptr );
RDBPLUGIN_API int (*RDBDestroyBone)( int hsid, int boneno );
RDBPLUGIN_API int (*RDBSetJointInitialPos)( int hsid, int jointno, RDBPoint pos, int calcflag, int excludeflag );

//頂点位置
RDBPLUGIN_API int (*RDBGetVertNumOfPart)( int hsid, int partno, int* numptr );
RDBPLUGIN_API int (*RDBGetVertPos)( int hsid, int partno, int vertno, RDBPoint* posptr );
RDBPLUGIN_API int (*RDBGetVertPos2)( int hsid, int partno, int vertno, int motid, int frameno, RDBPoint* posptr );
RDBPLUGIN_API int (*RDBGetVertNoOfFace)( int hsid, int partno, int faceno, int* vptr1, int* vptr2, int* vptr3 );
RDBPLUGIN_API int (*RDBGetSamePosVert)( int hsid, int partno, int vertno, int* samenoptr, int arrayleng, int* samenumptr );
RDBPLUGIN_API int (*RDBGetNormal)( int hsid, int partno, int motid, int frameno, RDBPoint* nptr, int arrayleng );
RDBPLUGIN_API int (*RDBGetOrgVertNo)( int hsid, int partno, int vertno, int* orgnoarray, int arrayleng, int* getnumptr );

//頂点影響度
RDBPLUGIN_API int (*RDBGetInfElemNum)( int hsid, int partno, int vertno, int* numptr );
RDBPLUGIN_API int (*RDBGetInfElem)( int hsid, int partno, int vertno, int infno, int* bonenoptr, float* rateptr );
RDBPLUGIN_API int (*RDBGetInfElem2)( int hsid, int partno, int vertno, int infno, int* bonenoptr, int* childnoptr, float* rateptr );
RDBPLUGIN_API int (*RDBSetInfElem)( int hsid, int partno, int vertno, int childjointno, int calcmode, int paintmode, int normalizeflag, float userrate, float directval );
RDBPLUGIN_API int (*RDBChkIM2Status)( int hsid, int* status );

//面、インデックス
RDBPLUGIN_API int (*RDBGetFaceNum)( int hsid, int partno, int* numptr );
RDBPLUGIN_API int (*RDBGetTriIndices)( int hsid, int partno, WORD* buf, int wordnum, int* setwordnumptr, int* revflagptr );
RDBPLUGIN_API int (*RDBGetTriIndicesInt)( int hsid, int partno, int* buf, int intnum, int* setintnumptr, int* revflagptr );

//色
RDBPLUGIN_API int (*RDBGetAlpha)( int hsid, int partno, int vertno, int* alphaptr );
RDBPLUGIN_API int (*RDBGetDiffuse)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
RDBPLUGIN_API int (*RDBGetAmbient)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
RDBPLUGIN_API int (*RDBGetSpecular)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
RDBPLUGIN_API int (*RDBGetEmissive)( int hsid, int partno, int vertno, int* rptr, int* gptr, int* bptr );
RDBPLUGIN_API int (*RDBGetSpecularPower)( int hsid, int partno, int vertno, float* powerptr );
RDBPLUGIN_API int (*RDBGetShaderType)( int hsid, int* shaderptr, int* overflowptr );

//テクスチャ
RDBPLUGIN_API int (*RDBGetTextureInfo)( int hsid, int partno, char* nameptr, int* repuptr, int* repvptr, float* animuptr, float* animvptr, int* transparentptr );
RDBPLUGIN_API int (*RDBGetExtTextureInfo)( int hsid, int partno, char* extnameptr, int* texnumptr, int* texmodeptr, int* repflagptr, int* stepptr );
RDBPLUGIN_API int (*RDBGetUV)( int hsid, int partno, int vertno, float* uptr, float* vptr );

//ボーン位置、モーション
RDBPLUGIN_API int (*RDBGetBonePos)( int hsid, int boneno, int poskind, int motid, int frameno, RDBPoint* posptr );
RDBPLUGIN_API int (*RDBGetBoneMatrix)( int hsid, int boneno, int motid, int frameno, int mode, RDBMatrix* matrixptr );
RDBPLUGIN_API int (*RDBGetBoneQuaternion)( int hsid, int boneno, int motid, int frameno, int mode, RDBQuaternion* qptr );
RDBPLUGIN_API int (*RDBGetFrameLeng)( int hsid, int motid, int* lengptr );
RDBPLUGIN_API int (*RDBGetKeyFrameNo)( int hsid, int boneno, int motid, int* framearray, int arrayleng, int* framenumptr );
RDBPLUGIN_API int (*RDBGetKeyFrameSRT)( int hsid, int boneno, int motid, int frameno, RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr );
RDBPLUGIN_API int (*RDBGetKeyFrameSRT2)( int hsid, int boneno, int motid, int frameno, RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr );
RDBPLUGIN_API int (*RDBGetOffsetMatrix)( int hsid, int boneno, RDBMatrix* matptr );
RDBPLUGIN_API int (*RDBGetCurrentFrameNo)( int hsid, int* framenoptr );
RDBPLUGIN_API int (*RDBGetMotionNum)( int hsid, int* numptr );
RDBPLUGIN_API int (*RDBGetMotionName)( int hsid, int motid, char* motname );
RDBPLUGIN_API int (*RDBSetMotionFrameNo)( int hsid, int motid, int frameno );
RDBPLUGIN_API int (*RDBSetKeyFrameSRT2)( int hsid, int boneno, int motid, int frameno, RDBPoint* scaleptr, RDBQuaternion* rotptr, RDBPoint* traptr );
RDBPLUGIN_API int (*RDBFillUpMotion)( int hsid, int motid, int topboneno, int startframe, int endframe );
RDBPLUGIN_API int (*RDBGetSplineParams)( int hsid, int boneno, int motid, int frameno, RDBSPPARAMSET* sppptr );
RDBPLUGIN_API int (*RDBSetSplineParams)( int hsid, int boneno, int motid, int frameno, int setflag, RDBSPPARAMSET* sppptr );
RDBPLUGIN_API int (*RDBCalcSplineParams)( int hsid, int boneno, int motid );


//デバッグ出力
RDBPLUGIN_API int (*RDBDbgOut)( char* lpFormat, ... );

//ファイル
RDBPLUGIN_API int (*RDBSaveSigFile)( int hsid, char* filename );

//プログレスバー
RDBPLUGIN_API int (*RDBCreateProgressBar)();
RDBPLUGIN_API int (*RDBSetProgressBar)( int pos );
RDBPLUGIN_API int (*RDBDestroyProgressBar)();

//マテリアル
RDBPLUGIN_API int (*RDBGetMaterialNum)( int hsid, int* matnumptr );
RDBPLUGIN_API int (*RDBGetMaterial)( int hsid, int matno, RDBMaterial* matptr );
RDBPLUGIN_API int (*RDBGetMaterialNo)( int hsid, int partno, int faceno, int* matnoptr );

//モーフ
RDBPLUGIN_API int (*RDBGetMorphBaseNum)( int hsid, int* basenumptr );
RDBPLUGIN_API int (*RDBGetMorphBaseInfo)( int hsid, RDBMORPHBASE* baseinfoptr, int arrayleng );
RDBPLUGIN_API int (*RDBGetMorphTargetInfo)( int hsid, int baseid, RDBMORPHTARGET* targetinfoptr, int arrayleng );
RDBPLUGIN_API int (*RDBGetMorphKeyNum)( int hsid, int motid, int boneid, int baseid, int targetid, int* keynumptr );
RDBPLUGIN_API int (*RDBGetMorphKey)( int hsid, int motid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr, int arrayleng );
RDBPLUGIN_API int (*RDBGetCurMorphInfo)( int hsid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr );
RDBPLUGIN_API int (*RDBCreateMorph)( int hsid, int baseid, int boneid );
RDBPLUGIN_API int (*RDBAddMorphTarget)( int hsid, int baseid, int targetid );
RDBPLUGIN_API int (*RDBSetMorphKey)( int hsid, int motid, int boneid, int baseid, int targetid, RDBMORPHMOTION* motioninfoptr );
RDBPLUGIN_API int (*RDBDestroyMorphKey)( int hsid, int motid, int boneid, int frameno, int baseid );

//トゥーン
RDBPLUGIN_API int (*RDBGetToon1MaterialNum)( int hsid, int partno, int* matnumptr );
RDBPLUGIN_API int (*RDBGetToon1Material)( int hsid, int partno, E3DTOON1MATERIAL* toon1ptr, int arrayleng );
RDBPLUGIN_API int (*RDBSetToon1Name)( int hsid, int partno, char* oldname, char* newname );
RDBPLUGIN_API int (*RDBSetToon1Diffuse)( int hsid, int partno, char* matname, RDBColor3f diffuse );
RDBPLUGIN_API int (*RDBSetToon1Specular)( int hsid, int partno, char* matname, RDBColor3f specular );
RDBPLUGIN_API int (*RDBSetToon1Ambient)( int hsid, int partno, char* matname, RDBColor3f ambient );
RDBPLUGIN_API int (*RDBSetToon1NL)( int hsid, int partno, char* matname, float darknl, float brightnl );
RDBPLUGIN_API int (*RDBSetToon1Edge0)( int hsid, int partno, char* matname, RDBColor3f col, int validflag, int invflag, float width );


#endif


#endif