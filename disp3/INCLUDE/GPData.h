#ifndef GPDATAH
#define GPDATAH

#include <d3dx9.h>
#include <coef.h>
#include <quaternion.h>

class CGPData
{
public:
	CGPData(int srcrotationorderXYZ);
	~CGPData();

	int InitParams(int srcrotationorderXYZ);
	int SetGPE( GPELEM* srcgpe );
	int GetMatWorld( D3DXMATRIX* dstmat );
	int CalcMatWorld();

private:
	int DestroyObjs();
	int Deg2Q( D3DXVECTOR3 rot, CQuaternion* dstq );

public:
	int m_rotationorderXYZ;
	GPELEM m_gpe;
	CQuaternion m_q;
	D3DXMATRIX m_matWorld;
};

#endif

