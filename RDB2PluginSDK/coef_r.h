#ifndef COEFRH
#define COEFRH

#define	DEG2PAI		0.01745329251994


//JointInfo
enum {
	JI_SERIAL = 0, 
	JI_NOTUSE,
	JI_PARENT,
	JI_CHILD,
	JI_BROTHER,
	JI_SISTER,
	JI_MAX
};

//DispObjInfo
enum {
	DOI_SERIAL = 0,
	DOI_NOTUSE,
	DOI_DISPSWITCH,
	DOI_INVISIBLE,
	DOI_MAX
};

//ShaderType
enum {
	COL_OLD,
	COL_PHONG,
	COL_BLINN,
	COL_SCHLICK,
	COL_MQCLASSIC,
	COL_TOON0,
	COL_TOON1,
	COL_MAX
};

//Shaderの色がオーバーフローしたときの処理
enum {
	OVERFLOW_CLAMP,
	OVERFLOW_SCALE,
	OVERFLOW_ORG,
	OVERFLOW_MAX
};

//SetInfElem, GetInfElem
	//SetInfElemのペイントモード
enum {
	PAINT_NOR,//通常セット
	PAINT_EXC,//排他セット
	PAINT_ADD,//加算セット
	PAINT_SUB,//減算セット
	PAINT_ERA,//消去
	PAINT_MAX
};

enum tag_calcmode
{
	CALCMODE_NOSKIN0,//スキニング無し
	CALCMODE_ONESKIN0,//距離と角度でスキニング
	CALCMODE_ONESKIN1,//距離のみでスキニング
	CALCMODE_DIRECT0,//数値直接指定
	CALCMODE_SYM,//対称セット
	CALCMODE_MAX
};

#endif