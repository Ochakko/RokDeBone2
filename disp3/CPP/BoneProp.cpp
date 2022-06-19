#include "stdafx.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <coef.h>
#include <crtdbg.h>

#include <shdelem.h>

#include <quaternion.h>

#include <vector>
#include <algorithm>


#define DBGH
#include <dbg.h>

int GetRound(float srcval)
{
	if (srcval > 0.0f) {
		return (int)(srcval + 0.5f);
	}
	else {
		return (int)(srcval - 0.5f);
	}
}


float vecDotVec( D3DXVECTOR3* vec1, D3DXVECTOR3* vec2 )
{
	return ( vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z );
}

float lengthVec( D3DXVECTOR3* vec )
{
	double mag;
	float leng;
	
	mag = vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
	if( mag == 0.0 ){
		leng = 0.0f;
	}else{
		leng = (float)sqrt( mag );
	}
	return leng;
}

float aCos( float dot )
{
	if( dot > 1.0f )
		dot = 1.0f;
	else if( dot < -1.0f )
		dot = -1.0f;

	double rad;
	rad = acos( dot );

	float degree;
	degree = (float)( rad * PAI2DEG );

	return degree;
}

int vec3RotateY( D3DXVECTOR3* dstvec, float deg, D3DXVECTOR3* srcvec )
{

	int ret;
	CQuaternion dirq;
	D3DXMATRIX	dirm;

	ret = dirq.SetRotation( 0, deg, 0 );
	_ASSERT( !ret );
	dirm = dirq.MakeRotMatX();


	D3DXVECTOR3 tmpsrcvec = *srcvec;//dstvecとsrcvecが同一ポインタの場合に対応

	dstvec->x = (float)((double)dirm._11 * (double)tmpsrcvec.x + (double)dirm._21 * (double)tmpsrcvec.y + (double)dirm._31 * (double)tmpsrcvec.z + (double)dirm._41);
	dstvec->y = (float)((double)dirm._12 * (double)tmpsrcvec.x + (double)dirm._22 * (double)tmpsrcvec.y + (double)dirm._32 * (double)tmpsrcvec.z + (double)dirm._42);
	dstvec->z = (float)((double)dirm._13 * (double)tmpsrcvec.x + (double)dirm._23 * (double)tmpsrcvec.y + (double)dirm._33 * (double)tmpsrcvec.z + (double)dirm._43);

	return 0;
}
int vec3RotateX( D3DXVECTOR3* dstvec, float deg, D3DXVECTOR3* srcvec )
{

	int ret;
	CQuaternion dirq;
	D3DXMATRIX	dirm;

	ret = dirq.SetRotation( deg, 0, 0 );
	_ASSERT( !ret );
	dirm = dirq.MakeRotMatX();

	D3DXVECTOR3 tmpsrcvec = *srcvec;//dstvecとsrcvecが同一ポインタの場合に対応

	dstvec->x = (float)((double)dirm._11 * (double)tmpsrcvec.x + (double)dirm._21 * (double)tmpsrcvec.y + (double)dirm._31 * (double)tmpsrcvec.z + (double)dirm._41);
	dstvec->y = (float)((double)dirm._12 * (double)tmpsrcvec.x + (double)dirm._22 * (double)tmpsrcvec.y + (double)dirm._32 * (double)tmpsrcvec.z + (double)dirm._42);
	dstvec->z = (float)((double)dirm._13 * (double)tmpsrcvec.x + (double)dirm._23 * (double)tmpsrcvec.y + (double)dirm._33 * (double)tmpsrcvec.z + (double)dirm._43);

	return 0;
}
int vec3RotateZ( D3DXVECTOR3* dstvec, float deg, D3DXVECTOR3* srcvec )
{

	int ret;
	CQuaternion dirq;
	D3DXMATRIX	dirm;

	ret = dirq.SetRotation( 0, 0, deg );
	_ASSERT( !ret );
	dirm = dirq.MakeRotMatX();

	D3DXVECTOR3 tmpsrcvec = *srcvec;//dstvecとsrcvecが同一ポインタの場合に対応

	dstvec->x = (float)((double)dirm._11 * (double)tmpsrcvec.x + (double)dirm._21 * (double)tmpsrcvec.y + (double)dirm._31 * (double)tmpsrcvec.z + (double)dirm._41);
	dstvec->y = (float)((double)dirm._12 * (double)tmpsrcvec.x + (double)dirm._22 * (double)tmpsrcvec.y + (double)dirm._32 * (double)tmpsrcvec.z + (double)dirm._42);
	dstvec->z = (float)((double)dirm._13 * (double)tmpsrcvec.x + (double)dirm._23 * (double)tmpsrcvec.y + (double)dirm._33 * (double)tmpsrcvec.z + (double)dirm._43);

	return 0;
}



#ifdef ROKDEBONE2_VER6

int qToEuler(CShdElem* selem, CQuaternion* srcq, D3DXVECTOR3* reteul)
{

	int ret;
	CQuaternion axisQ, invaxisQ, EQ;
	if (selem) {
		ret = selem->GetBoneAxisQ(&axisQ);
		if (ret) {
			DbgOut("qToEuler : se GetBoneAxisQ error !!!\n");
			_ASSERT(0);
			return 1;
		}
		ret = axisQ.inv(&invaxisQ);
		_ASSERT(!ret);
		EQ = invaxisQ * *srcq * axisQ;
	}
	else {
		EQ = *srcq;
	}

	D3DXVECTOR3 Euler;

	CQuaternion iniq;

	CQuaternion QinvZ;
	CQuaternion EQinvZ;
	D3DXVECTOR3 EinvZ;

	CQuaternion QinvY;
	CQuaternion EQinvYZ;
	D3DXVECTOR3 EinvY;

	D3DXVECTOR3 axisXVec(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axisYVec(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axisZVec(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 targetVec, shadowVec;
	D3DXVECTOR3 tmpVec;

	EQ.Rotate(&targetVec, axisXVec);
	shadowVec.x = (float)vecDotVec(&targetVec, &axisXVec);
	shadowVec.y = (float)vecDotVec(&targetVec, &axisYVec);
	shadowVec.z = 0.0f;
	if (lengthVec(&shadowVec) <= 1e-5) {
		Euler.z = 90.0f;
	}
	else {
		Euler.z = (float)aCos(vecDotVec(&shadowVec, &axisXVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisYVec) < 0.0) {
		Euler.z = -Euler.z;
	}
	//if (vecDotVec(&shadowVec, &axisYVec) > 0.0f) {
	//	Euler.z = -Euler.z;
	//}

	EinvZ = D3DXVECTOR3(0.0f, 0.0f, -Euler.z);
	QinvZ.SetRotationXYZ(&iniq, EinvZ);
	EQinvZ = QinvZ * EQ;
	EQinvZ.Rotate(&targetVec, axisXVec);
	shadowVec.x = (float)vecDotVec(&targetVec, &axisXVec);
	shadowVec.y = 0.0f;
	shadowVec.z = (float)vecDotVec(&targetVec, &axisZVec);

	//vec3RotateZ(&tmpVec, -Euler.z, &targetVec);
	//shadowVec.x = (float)vecDotVec(&tmpVec, &axisXVec);
	//shadowVec.y = 0.0f;
	//shadowVec.z = (float)vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) <= 1e-5) {
		Euler.y = 90.0f;
	}
	else {
		Euler.y = (float)aCos(vecDotVec(&shadowVec, &axisXVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisZVec) > 0.0) {
		Euler.y = -Euler.y;
	}
	//if (vecDotVec(&shadowVec, &axisZVec) < 0.0f) {
	//	Euler.y = -Euler.y;
	//}


	EinvY = D3DXVECTOR3(0.0f, -Euler.y, 0.0f);
	QinvY.SetRotationXYZ(&iniq, EinvY);
	EQinvYZ = QinvY * QinvZ * EQ;
	EQinvYZ.Rotate(&targetVec, axisZVec);
	shadowVec.x = 0.0f;
	shadowVec.y = (float)vecDotVec(&targetVec, &axisYVec);
	shadowVec.z = (float)vecDotVec(&targetVec, &axisZVec);

	//EQ.Rotate(&targetVec, axisZVec);
	//vec3RotateZ(&tmpVec, -Euler.z, &targetVec);
	//targetVec = tmpVec;
	//vec3RotateY(&tmpVec, -Euler.y, &targetVec);
	//shadowVec.x = 0.0f;
	//shadowVec.y = (float)vecDotVec(&tmpVec, &axisYVec);
	//shadowVec.z = (float)vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) <= 1e-5) {
		Euler.x = 90.0f;
	}
	else {
		Euler.x = (float)aCos(vecDotVec(&shadowVec, &axisZVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisYVec) > 0.0) {
		Euler.x = -Euler.x;
	}
	//if (vecDotVec(&shadowVec, &axisYVec) < 0.0f) {
	//	Euler.x = -Euler.x;
	//}

	//ModifyEuler(&Euler, &befeul);
	//ModifyEulerXYZ(&Euler, &befeul, isfirstbone, isendbone, notmodifyflag);
	*reteul = Euler;

	return 0;
}

int qToEulerAxis(CQuaternion axisQ, CQuaternion* srcq, D3DXVECTOR3* reteul)
{

	int ret;
	CQuaternion invaxisQ, EQ;
	ret = axisQ.inv(&invaxisQ);
	_ASSERT(!ret);
	EQ = invaxisQ * *srcq * axisQ;

	D3DXVECTOR3 Euler;

	CQuaternion iniq;

	CQuaternion QinvZ;
	CQuaternion EQinvZ;
	D3DXVECTOR3 EinvZ;

	CQuaternion QinvY;
	CQuaternion EQinvYZ;
	D3DXVECTOR3 EinvY;

	D3DXVECTOR3 axisXVec(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axisYVec(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axisZVec(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 targetVec, shadowVec;
	D3DXVECTOR3 tmpVec;

	EQ.Rotate(&targetVec, axisXVec);
	shadowVec.x = (float)vecDotVec(&targetVec, &axisXVec);
	shadowVec.y = (float)vecDotVec(&targetVec, &axisYVec);
	shadowVec.z = 0.0f;
	if (lengthVec(&shadowVec) <= 1e-5) {
		Euler.z = 90.0f;
	}
	else {
		Euler.z = (float)aCos(vecDotVec(&shadowVec, &axisXVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisYVec) < 0.0) {
		Euler.z = -Euler.z;
	}
	//if (vecDotVec(&shadowVec, &axisYVec) > 0.0f) {
	//	Euler.z = -Euler.z;
	//}

	EinvZ = D3DXVECTOR3(0.0f, 0.0f, -Euler.z);
	QinvZ.SetRotationXYZ(&iniq, EinvZ);
	EQinvZ = QinvZ * EQ;
	EQinvZ.Rotate(&targetVec, axisXVec);
	shadowVec.x = (float)vecDotVec(&targetVec, &axisXVec);
	shadowVec.y = 0.0f;
	shadowVec.z = (float)vecDotVec(&targetVec, &axisZVec);

	//vec3RotateZ(&tmpVec, -Euler.z, &targetVec);
	//shadowVec.x = (float)vecDotVec(&tmpVec, &axisXVec);
	//shadowVec.y = 0.0f;
	//shadowVec.z = (float)vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) <= 1e-5) {
		Euler.y = 90.0f;
	}
	else {
		Euler.y = (float)aCos(vecDotVec(&shadowVec, &axisXVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisZVec) > 0.0) {
		Euler.y = -Euler.y;
	}
	//if (vecDotVec(&shadowVec, &axisZVec) < 0.0f) {
	//	Euler.y = -Euler.y;
	//}


	EinvY = D3DXVECTOR3(0.0f, -Euler.y, 0.0f);
	QinvY.SetRotationXYZ(&iniq, EinvY);
	EQinvYZ = QinvY * QinvZ * EQ;
	EQinvYZ.Rotate(&targetVec, axisZVec);
	shadowVec.x = 0.0f;
	shadowVec.y = (float)vecDotVec(&targetVec, &axisYVec);
	shadowVec.z = (float)vecDotVec(&targetVec, &axisZVec);

	//EQ.Rotate(&targetVec, axisZVec);
	//vec3RotateZ(&tmpVec, -Euler.z, &targetVec);
	//targetVec = tmpVec;
	//vec3RotateY(&tmpVec, -Euler.y, &targetVec);
	//shadowVec.x = 0.0f;
	//shadowVec.y = (float)vecDotVec(&tmpVec, &axisYVec);
	//shadowVec.z = (float)vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) <= 1e-5) {
		Euler.x = 90.0f;
	}
	else {
		Euler.x = (float)aCos(vecDotVec(&shadowVec, &axisZVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisYVec) > 0.0) {
		Euler.x = -Euler.x;
	}
	//if (vecDotVec(&shadowVec, &axisYVec) < 0.0f) {
	//	Euler.x = -Euler.x;
	//}

	//ModifyEuler(&Euler, &befeul);
	//ModifyEulerXYZ(&Euler, &befeul, isfirstbone, isendbone, notmodifyflag);
	*reteul = Euler;

	return 0;

}


int modifyEuler(D3DXVECTOR3* eulerA, D3DXVECTOR3* eulerB)
{

	//オイラー角Aの値をオイラー角Bの値に近い表示に修正
	double tmpX0, tmpY0, tmpZ0;
	double tmpX1, tmpY1, tmpZ1;
	double tmpX2, tmpY2, tmpZ2;
	double tmpX3, tmpY3, tmpZ3;
	double tmpX4, tmpY4, tmpZ4;
	//double tmpX5, tmpY5, tmpZ5;
	double s0, s1, s2, s3, s4;// , s5;
	//double newX, newY, newZ;

	tmpX0 = eulerA->x;
	tmpY0 = eulerA->y;
	tmpZ0 = eulerA->z;

	//予想される角度1
	tmpX1 = (double)eulerA->x + 360.0 * GetRound((float)(((double)eulerB->x - (double)eulerA->x) / 360.0));
	tmpY1 = (double)eulerA->y + 360.0 * GetRound((float)(((double)eulerB->y - (double)eulerA->y) / 360.0));
	tmpZ1 = (double)eulerA->z + 360.0 * GetRound((float)(((double)eulerB->z - (double)eulerA->z) / 360.0));


	//##########################################################################################
	// ModifyEuler いままでの試行錯誤のまとめ
	//####################################
	// 
	// 
	// 	notmodifyflag == 1//!!!! bvh-->fbx書き出し時にはmodifyeulerで裏返りチェックをするが、それ以外の時は２重に処理しないように裏返りチェックをしない
	// 
	// 
	// 
	// 試行錯誤時の症状と数学を思い出し、結局次のようになった.
	// ノイズが乗っているときにも今までで一番きれいなオイラーグラフになった.つま先に関しても改善.
	// 
	//座標系合わせ。軸の向きが座標系に合うように１８０度回転チェック。座標系を合わせるにはbefeulに近づければ良い.
	// 
	// 
	// クォータニオンからオイラー角を計算するとき、ボーン軸に関して１８０度ねじれるようなクオータニオンのオイラー角と同じオイラー角になる.
	// 例えばZが１８０度回転した時、XとYは-X, -Yになる.(180 - X)では無かった.
	// 裏返ったオイラー角の方がbefeulに近い場合を検出してオイラー角を裏返返す処理をする.
	// 
	// 
	//##########################################################################################


	//X + 180のとき
	tmpX2 = tmpX0 + 180.0;
	if (tmpX2 > 180.0) {
		tmpX2 -= 360.0;
	}
	tmpY2 = -tmpY0;
	tmpZ2 = -tmpZ0;
	//tmpY2 = 180.0 - tmpY0;
	//tmpZ2 = 180.0 - tmpZ0;

	//Y + 180のとき
	tmpY3 = tmpY0 + 180.0;
	if (tmpY3 > 180.0) {
		tmpY3 -= 360.0;
	}
	tmpX3 = -tmpX0;
	tmpZ3 = -tmpZ0;
	//tmpX3 = 180.0 - tmpX0;
	//tmpZ3 = 180.0 - tmpZ0;

	//Z + 180のとき
	tmpZ4 = tmpZ0 + 180.0;
	if (tmpZ4 > 180.0) {
		tmpZ4 -= 360.0;
	}
	tmpX4 = -tmpX0;
	tmpY4 = -tmpY0;
	//tmpX4 = 180.0 - tmpX0;
	//tmpY4 = 180.0 - tmpY0;



	//角度変化の大きさ
	s0 = ((double)eulerB->x - tmpX0) * ((double)eulerB->x - tmpX0) + ((double)eulerB->y - tmpY0) * ((double)eulerB->y - tmpY0) + ((double)eulerB->z - tmpZ0) * ((double)eulerB->z - tmpZ0);
	s1 = ((double)eulerB->x - tmpX1) * ((double)eulerB->x - tmpX1) + ((double)eulerB->y - tmpY1) * ((double)eulerB->y - tmpY1) + ((double)eulerB->z - tmpZ1) * ((double)eulerB->z - tmpZ1);
	s2 = ((double)eulerB->x - tmpX2) * ((double)eulerB->x - tmpX2) + ((double)eulerB->y - tmpY2) * ((double)eulerB->y - tmpY2) + ((double)eulerB->z - tmpZ2) * ((double)eulerB->z - tmpZ2);
	s3 = ((double)eulerB->x - tmpX3) * ((double)eulerB->x - tmpX3) + ((double)eulerB->y - tmpY3) * ((double)eulerB->y - tmpY3) + ((double)eulerB->z - tmpZ3) * ((double)eulerB->z - tmpZ3);
	s4 = ((double)eulerB->x - tmpX4) * ((double)eulerB->x - tmpX4) + ((double)eulerB->y - tmpY4) * ((double)eulerB->y - tmpY4) + ((double)eulerB->z - tmpZ4) * ((double)eulerB->z - tmpZ4);

	typedef struct tag_chkeul
	{
		double s;
		int index;
		double X, Y, Z;
		bool operator<(const tag_chkeul& right) const {
			return s == right.s ? index < right.index : s < right.s;
		}
	}CHKEUL;

	std::vector<struct tag_chkeul> vecchkeul;
	CHKEUL tmpchkeul;
	tmpchkeul.s = s0;
	tmpchkeul.index = 0;
	tmpchkeul.X = tmpX0;
	tmpchkeul.Y = tmpY0;
	tmpchkeul.Z = tmpZ0;
	vecchkeul.push_back(tmpchkeul);
	tmpchkeul.s = s1;
	tmpchkeul.index = 1;
	tmpchkeul.X = tmpX1;
	tmpchkeul.Y = tmpY1;
	tmpchkeul.Z = tmpZ1;
	vecchkeul.push_back(tmpchkeul);
	tmpchkeul.s = s2;
	tmpchkeul.index = 2;
	tmpchkeul.X = tmpX2;
	tmpchkeul.Y = tmpY2;
	tmpchkeul.Z = tmpZ2;
	vecchkeul.push_back(tmpchkeul);
	tmpchkeul.s = s3;
	tmpchkeul.index = 3;
	tmpchkeul.X = tmpX3;
	tmpchkeul.Y = tmpY3;
	tmpchkeul.Z = tmpZ3;
	vecchkeul.push_back(tmpchkeul);
	tmpchkeul.s = s4;
	tmpchkeul.index = 4;
	tmpchkeul.X = tmpX4;
	tmpchkeul.Y = tmpY4;
	tmpchkeul.Z = tmpZ4;
	vecchkeul.push_back(tmpchkeul);

	std::sort(vecchkeul.begin(), vecchkeul.end());

	CHKEUL mineul = vecchkeul[0];

	//if ((notmodifyflag == 0) && (isfirstbone == 0)) {

		eulerA->x = (float)mineul.X; eulerA->y = (float)mineul.Y; eulerA->z = (float)mineul.Z;

	//}
	//else {
	//	//if (s0 <= s1) {
	//	eulerA->x = (float)tmpX0; eulerA->y = (float)tmpY0; eulerA->z = (float)tmpZ0;
	//	//}
	//	//else {
	//	//	eulerA->x = (float)tmpX1; eulerA->y = (float)tmpY1; eulerA->z = (float)tmpZ1;
	//	//}
	//}

	return 0;
}

#else

int qToEuler(CShdElem* selem, CQuaternion* srcq, D3DXVECTOR3* Euler)
{

	int ret;
	CQuaternion axisQ, invaxisQ, EQ;
	if (selem) {
		ret = selem->GetBoneAxisQ(&axisQ);
		if (ret) {
			DbgOut("qToEuler : se GetBoneAxisQ error !!!\n");
			_ASSERT(0);
			return 1;
		}
		ret = axisQ.inv(&invaxisQ);
		_ASSERT(!ret);
		EQ = invaxisQ * *srcq * axisQ;
	}
	else {
		EQ = *srcq;
	}

	D3DXVECTOR3 axisXVec(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axisYVec(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axisZVec(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 targetVec, shadowVec;
	D3DXVECTOR3 tmpVec;

	EQ.Rotate(&targetVec, axisZVec);
	shadowVec.x = vecDotVec(&targetVec, &axisXVec);
	shadowVec.y = 0.0f;
	shadowVec.z = vecDotVec(&targetVec, &axisZVec);
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->y = 90.0f;
	}
	else {
		Euler->y = aCos(vecDotVec(&shadowVec, &axisZVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisXVec) < 0.0f) {
		Euler->y = -Euler->y;
	}

	/***
		E3DMultQVec targetQ,0.0,0.0,1.0,targetVec.0,targetVec.1,targetVec.2
		shadowVec.0= vecDotVec(targetVec,axisXVec)
		shadowVec.1= 0.0
		shadowVec.2= vecDotVec(targetVec,axisZVec)
		if( lengthVec(shadowVec) == 0 ){
			eulerY= 90.0
		}else{
			E3DACos ( vecDotVec(shadowVec,axisZVec)/ lengthVec(shadowVec) ), eulerY		//Y軸回転度数
		}
		if( vecDotVec(shadowVec,axisXVec) < 0.0 ){ eulerY= -eulerY }				//右回りに修正
	***/
	vec3RotateY(&tmpVec, -Euler->y, &targetVec);
	shadowVec.x = 0.0f;
	shadowVec.y = vecDotVec(&tmpVec, &axisYVec);
	shadowVec.z = vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->x = 90.0f;
	}
	else {
		Euler->x = aCos(vecDotVec(&shadowVec, &axisZVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisYVec) > 0.0f) {
		Euler->x = -Euler->x;
	}
	/***
		E3DVec3RotateY targetVec.0,targetVec.1,targetVec.2,(-eulerY),shadowVec.0,shadowVec.1,shadowVec.2
		if( lengthVec(shadowVec) == 0 ){
			eulerX= 90.0
		}else{
			E3DACos ( vecDotVec(shadowVec,axisZVec)/ lengthVec(shadowVec) ), eulerX		//X軸回転度数
		}
		if( vecDotVec(shadowVec,axisYVec) > 0.0 ){ eulerX= -eulerX }				//右回りに修正
	***/

	EQ.Rotate(&targetVec, axisYVec);
	vec3RotateY(&tmpVec, -Euler->y, &targetVec);
	targetVec = tmpVec;
	vec3RotateX(&tmpVec, -Euler->x, &targetVec);
	shadowVec.x = vecDotVec(&tmpVec, &axisXVec);
	shadowVec.y = vecDotVec(&tmpVec, &axisYVec);
	shadowVec.z = 0.0f;
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->z = 90.0f;
	}
	else {
		Euler->z = aCos(vecDotVec(&shadowVec, &axisYVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisXVec) > 0.0f) {
		Euler->z = -Euler->z;
	}
	/***
		E3DMultQVec targetQ,0.0,1.0,0.0,targetVec.0,targetVec.1,targetVec.2
		E3DVec3RotateY (targetVec.0+0.0),(targetVec.1+0.0),(targetVec.2+0.0),(-eulerY),targetVec.0,targetVec.1,targetVec.2
		E3DVec3RotateY targetVec.2,targetVec.0,targetVec.1,(-eulerX),shadowVec.2,shadowVec.0,shadowVec.1
		　　----> 引数がZ,X,YになっているためRotateXと同じ！！！

		if( lengthVec(shadowVec) == 0 ){
			eulerZ= 90.0
		}else{
			E3DACos ( vecDotVec(shadowVec,axisYVec)/ lengthVec(shadowVec) ), eulerZ		//Z軸回転度数
		}
		if( vecDotVec(shadowVec,axisXVec) > 0.0 ){ eulerZ= -eulerZ }				//右回りに修正

	***/
	return 0;
}

int qToEulerAxis(CQuaternion axisQ, CQuaternion* srcq, D3DXVECTOR3* Euler)
{

	int ret;
	CQuaternion invaxisQ, EQ;
	ret = axisQ.inv(&invaxisQ);
	_ASSERT(!ret);
	EQ = invaxisQ * *srcq * axisQ;


	D3DXVECTOR3 axisXVec(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axisYVec(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axisZVec(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 targetVec, shadowVec;
	D3DXVECTOR3 tmpVec;

	EQ.Rotate(&targetVec, axisZVec);
	shadowVec.x = vecDotVec(&targetVec, &axisXVec);
	shadowVec.y = 0.0f;
	shadowVec.z = vecDotVec(&targetVec, &axisZVec);
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->y = 90.0f;
	}
	else {
		Euler->y = aCos(vecDotVec(&shadowVec, &axisZVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisXVec) < 0.0f) {
		Euler->y = -Euler->y;
	}

	/***
		E3DMultQVec targetQ,0.0,0.0,1.0,targetVec.0,targetVec.1,targetVec.2
		shadowVec.0= vecDotVec(targetVec,axisXVec)
		shadowVec.1= 0.0
		shadowVec.2= vecDotVec(targetVec,axisZVec)
		if( lengthVec(shadowVec) == 0 ){
			eulerY= 90.0
		}else{
			E3DACos ( vecDotVec(shadowVec,axisZVec)/ lengthVec(shadowVec) ), eulerY		//Y軸回転度数
		}
		if( vecDotVec(shadowVec,axisXVec) < 0.0 ){ eulerY= -eulerY }				//右回りに修正
	***/
	vec3RotateY(&tmpVec, -Euler->y, &targetVec);
	shadowVec.x = 0.0f;
	shadowVec.y = vecDotVec(&tmpVec, &axisYVec);
	shadowVec.z = vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->x = 90.0f;
	}
	else {
		Euler->x = aCos(vecDotVec(&shadowVec, &axisZVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisYVec) > 0.0f) {
		Euler->x = -Euler->x;
	}
	/***
		E3DVec3RotateY targetVec.0,targetVec.1,targetVec.2,(-eulerY),shadowVec.0,shadowVec.1,shadowVec.2
		if( lengthVec(shadowVec) == 0 ){
			eulerX= 90.0
		}else{
			E3DACos ( vecDotVec(shadowVec,axisZVec)/ lengthVec(shadowVec) ), eulerX		//X軸回転度数
		}
		if( vecDotVec(shadowVec,axisYVec) > 0.0 ){ eulerX= -eulerX }				//右回りに修正
	***/

	EQ.Rotate(&targetVec, axisYVec);
	vec3RotateY(&tmpVec, -Euler->y, &targetVec);
	targetVec = tmpVec;
	vec3RotateX(&tmpVec, -Euler->x, &targetVec);
	shadowVec.x = vecDotVec(&tmpVec, &axisXVec);
	shadowVec.y = vecDotVec(&tmpVec, &axisYVec);
	shadowVec.z = 0.0f;
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->z = 90.0f;
	}
	else {
		Euler->z = aCos(vecDotVec(&shadowVec, &axisYVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisXVec) > 0.0f) {
		Euler->z = -Euler->z;
	}
	/***
		E3DMultQVec targetQ,0.0,1.0,0.0,targetVec.0,targetVec.1,targetVec.2
		E3DVec3RotateY (targetVec.0+0.0),(targetVec.1+0.0),(targetVec.2+0.0),(-eulerY),targetVec.0,targetVec.1,targetVec.2
		E3DVec3RotateY targetVec.2,targetVec.0,targetVec.1,(-eulerX),shadowVec.2,shadowVec.0,shadowVec.1
		　　----> 引数がZ,X,YになっているためRotateXと同じ！！！

		if( lengthVec(shadowVec) == 0 ){
			eulerZ= 90.0
		}else{
			E3DACos ( vecDotVec(shadowVec,axisYVec)/ lengthVec(shadowVec) ), eulerZ		//Z軸回転度数
		}
		if( vecDotVec(shadowVec,axisXVec) > 0.0 ){ eulerZ= -eulerZ }				//右回りに修正

	***/
	return 0;
}


int qToEulerYXZ(CShdElem* selem, CQuaternion* srcq, D3DXVECTOR3* Euler)
{

	//YXZは、座１のみに対応

	D3DXVECTOR3 axisXVec(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axisYVec(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axisZVec(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 targetVec, shadowVec;
	D3DXVECTOR3 tmpVec;

	srcq->Rotate(&targetVec, axisYVec);
	shadowVec.x = vecDotVec(&targetVec, &axisXVec);
	shadowVec.y = vecDotVec(&targetVec, &axisYVec);
	shadowVec.z = 0.0f;
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->z = 90.0f;
	}
	else {
		Euler->z = aCos(vecDotVec(&shadowVec, &axisYVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisXVec) > 0.0f) {
		Euler->z = -Euler->z;
	}

	/***
	E3DMultQVec targetQ, 0.0, 1.0, 0.0, targetVec.0, targetVec.1, targetVec.2
	shadowVec.0 = vecDotVec( targetVec, axisXVec )
	shadowVec.1 = vecDotVec( targetVec, axisYVec )
	shadowVec.2 = 0.0
	if( lengthVec( shadowVec ) == 0.0 ){
		eulerZ = 90.0
	}else{
		E3DACos( vecDotVec( shadowVec, axisYVecc ) / lengthVec( shadowVec ) ), eulerZ
	}
	if( vecDotVec( shadowVec, axisXVec ) > 0.0 ){ eulerZ= -eulerZ }
	***/
	vec3RotateZ(&tmpVec, -Euler->z, &targetVec);
	shadowVec.x = 0.0f;
	shadowVec.y = vecDotVec(&tmpVec, &axisYVec);
	shadowVec.z = vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->x = 90.0f;
	}
	else {
		Euler->x = aCos(vecDotVec(&shadowVec, &axisYVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisZVec) < 0.0f) {
		Euler->x = -Euler->x;
	}
	/***
	Vec3RotateZ targetVec.0, targetVec.1, targetVec.2, (-eulerZ), tempVec.0, tempVec.1, tempVec.2
	shadowVec.x = 0.0
	shadowVec.y = vecDotVec( tempVec, axisYVec )
	shadowVec.z = vecDotVec( tempVec, axisZVec )
	if( lengthVec( shadowVec ) == 0 ){
		eulerX= 90.0
	}else{
		E3DACos ( vecDotVec( shadowVec, axisYVec )/ lengthVec( shadowVec ) ), eulerX
	}
	if( vecDotVec(shadowVec,axisZVec) < 0.0 ){ eulerX= -eulerX }
	***/
	srcq->Rotate(&targetVec, axisZVec);
	vec3RotateZ(&tmpVec, -Euler->z, &targetVec);
	targetVec = tmpVec;
	vec3RotateX(&tmpVec, -Euler->x, &targetVec);
	shadowVec.x = vecDotVec(&tmpVec, &axisXVec);
	shadowVec.y = 0.0f;
	shadowVec.z = vecDotVec(&tmpVec, &axisZVec);
	if (lengthVec(&shadowVec) == 0.0f) {
		Euler->y = 90.0f;
	}
	else {
		Euler->y = aCos(vecDotVec(&shadowVec, &axisZVec) / lengthVec(&shadowVec));
	}
	if (vecDotVec(&shadowVec, &axisXVec) < 0.0f) {
		Euler->y = -Euler->y;
	}
	/***
	E3DMultQVec targetQ, 0.0, 0.0, 1.0, tempVec.0, tempVec.1, tempVec.2
	Vec3RotateZ tempVec.0, tempVec.1, tempVec.2, (-eulerZ), tempVec2.0, tempVec2.1, tempVec2.2
	Vec3RotateX tempVec2.0, tempVec2.1, tempVec2.2, (-eulerX), targetVec.0, targetVec.1, targetVec.2
	shadowVec.0 = vecDotVec( targetVec, axisXVec )
	shadowVec.1 = 0.0
	shadowVec.2 = vecDotVec( targetVec, axisZVec )
	if( lengthVec( shadowVec ) == 0 ){
		eulerY= 90.0
	}else{
		E3DACos ( vecDotVec( shadowVec, axisZVec )/ lengthVec( shadowVec ) ), eulerY
	}
	if( vecDotVec(shadowVec,axisXVec) < 0.0 ){ eulerY= -eulerY }
	***/
	return 0;
}

int modifyEuler(D3DXVECTOR3* eulerA, D3DXVECTOR3* eulerB)
{

	//オイラー角Aの値をオイラー角Bの値に近い表示に修正
	float tmpX1, tmpY1, tmpZ1;
	float tmpX2, tmpY2, tmpZ2;
	float s1, s2;

	//予想される角度1
	tmpX1 = eulerA->x + 360.0f * GetRound((eulerB->x - eulerA->x) / 360.0f);
	tmpY1 = eulerA->y + 360.0f * GetRound((eulerB->y - eulerA->y) / 360.0f);
	tmpZ1 = eulerA->z + 360.0f * GetRound((eulerB->z - eulerA->z) / 360.0f);

	//予想される角度2
	tmpX2 = 180.0f - eulerA->x + 360.0f * GetRound((eulerB->x + eulerA->x - 180.0f) / 360.0f);
	tmpY2 = eulerA->y + 180.0f + 360.0f * GetRound((eulerB->y - eulerA->y - 180.0f) / 360.0f);
	tmpZ2 = eulerA->z + 180.0f + 360.0f * GetRound((eulerB->z - eulerA->z - 180.0f) / 360.0f);

	//角度変化の大きさ
	s1 = (eulerB->x - tmpX1) * (eulerB->x - tmpX1) + (eulerB->y - tmpY1) * (eulerB->y - tmpY1) + (eulerB->z - tmpZ1) * (eulerB->z - tmpZ1);
	s2 = (eulerB->x - tmpX2) * (eulerB->x - tmpX2) + (eulerB->y - tmpY2) * (eulerB->y - tmpY2) + (eulerB->z - tmpZ2) * (eulerB->z - tmpZ2);

	//変化の少ない方に修正
	if (s1 < s2) {
		eulerA->x = tmpX1; eulerA->y = tmpY1; eulerA->z = tmpZ1;
	}
	else {
		eulerA->x = tmpX2; eulerA->y = tmpY2; eulerA->z = tmpZ2;
	}

	return 0;
}

#endif



float QuaternionLimitPhai(float srcphai)
{
	//#######################################################
	//srcphaiを-180度から180度に直してからクォータニオンの設定をする
	//#######################################################

	float retphai = 0.0f;
	float tmpphai = srcphai;
	int dbgcnt = 0;

	if (srcphai > 0.0f) {
		//retphai = srcphai - (float)((int)(srcphai / (2.0f * (float)PAI))) * (2.0f * (float)PAI);
		//retphai = srcphai - (float)(((int)(srcphai / (float)PAI)) + 1) * (float)PAI;

		//for example : 190 --> -170
		while ((tmpphai >= -(float)PAI) && (tmpphai <= (float)PAI)) {
			tmpphai -= (2.0 * (float)PAI);
			dbgcnt++;
			if (dbgcnt >= 100) {
				tmpphai = srcphai;
				break;
			}
		}

		return tmpphai;
	}
	else if (srcphai < 0.0f) {
		//retphai = srcphai + (float)(((int)(srcphai / (float)PAI)) + 1) * (float)PAI;

		//for example : -190 --> 170

		while ((tmpphai >= -(float)PAI) && (tmpphai <= (float)PAI)) {
			tmpphai += (2.0 * (float)PAI);
			dbgcnt++;
			if (dbgcnt >= 100) {
				tmpphai = srcphai;
				break;
			}
		}

		return tmpphai;
	}
	else {
		return 0.0f;
	}
}




