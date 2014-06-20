#ifndef RpsFileH
#define RpsFileH

#include <d3dx9.h>
#include <usercoef.h>

#define RPSLINELEN	1024

class CTraQ;
class CQuaternion;
class CShdHandler;

class CMyD3DApplication;
class CMotionPoint2;

class CRpsFile
{
public:
	CRpsFile( CMyD3DApplication* papp );
	~CRpsFile();

	int CreateParams();
	int CalcTraQ( TSELEM* tsptr, int chksym );
	int SetMotion( TSELEM* tsptr, int motid, int setframe );
//	int AvrgMotion( TSELEM* tselem, int type, int filter_size );
//	int OnMabiki( float rotval, float mvval );

	int SetRpsElem( int frameno, D3DXVECTOR3* srcpos );
	int InitArMp( TSELEM* tsptr, int motid, int frameno );



private:
	int InitParams();
	int DestroyObjs();

	int SetBuffer( char* filename );

	int CreateRpsElem();

	int SetSkipFlag();
	int ResetOutputFlag( D3DXVECTOR3* keyeul, D3DXVECTOR3* keymv );

	int SetSym( int skelno, int sksym, TSELEM* tsptr, CShdHandler* lpsh );
	int CalcSymPose( TSELEM* tsptr, CShdHandler* lpsh, CTraQ* traq, 
		CQuaternion* newqptr, D3DXVECTOR3* newtraptr, D3DXVECTOR3* neweulptr, D3DXVECTOR3 befeul );

	int CreateMotionPoints( int cookie );

private:
	CMyD3DApplication* m_papp;

	//file操作用
	HANDLE m_hfile;
	char* m_buf;
	DWORD m_pos;
	DWORD m_bufleng;


	char m_line[ RPSLINELEN ];
	RPSELEM* m_pelem;
	CTraQ* m_traq;
//	int m_framenum;

//	int m_sampling;
//	int m_keystep;

	int m_skipflag[ SKEL_MAX ];

	//ガウシアンフィルタ用
	int combi( int N, int rp );


	CMotionPoint2* m_armp[ SKEL_MAX ];


};


#endif