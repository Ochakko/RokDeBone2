#ifndef TxtSettingH
#define TxtSettingH

#include <d3dx9.h>
#include <usercoef.h>

#define TXTLINELEN	2048

class CMyD3DApplication;

class CTxtSetting
{
public:
	CTxtSetting( CMyD3DApplication* papp );
	~CTxtSetting();

	int LoadParams( char* filename );
	int SaveKstFile( char* filename );

private:
	int InitParams();
	int DestroyObjs();

	int SetBuffer( char* filename );

	int GetElemLine( char* strpat );

	char* GetName( char* srcstr, char*dststr, int maxleng, int* dstsetflag );

	int Write2File( char* lpFormat, ... );

public:
	TSELEM m_elem[ SKEL_MAX ];

private:
	CMyD3DApplication* m_papp;

	//file����p
	HANDLE m_hfile;
	char* m_buf;
	DWORD m_pos;
	DWORD m_bufleng;

	int m_filemode;

	char m_line[ TXTLINELEN ];
};

#endif