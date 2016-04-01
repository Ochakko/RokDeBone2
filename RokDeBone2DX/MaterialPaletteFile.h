#ifndef MATERIALPALETTEFILEH
#define MATERIALPALETTEFILEH

#include <coef.h>

#define MPBLOCKLENG	2048

class CMaterialPaletteFile
{
public:
	CMaterialPaletteFile();
	~CMaterialPaletteFile();

	int LoadMaterialPalette( char* filename );
	int WriteMaterialPalette( char* filename, MPALETTE* srcmp, int mpnum );



private:
	int InitParams();
	int DestroyObjs();

	int SetBuffer( char* filename );
	int CheckFileHeader();

	int GetMPBlock( int* lengptr );
	int ParseMPBlock( int srcleng );

	int GetName( char* dstchar, int dstleng, char* srcchar, int pos, int srcleng );
	int GetInt( int* dstint, char* srcchar, int pos, int srcleng, int* stepnum );
	int GetFloat( float* dstfloat, char* srcchar, int pos, int srcleng, int* stepnum );

	int Write2File( char* lpFormat, ... );

public:
	MPALETTE m_mpalette[ MPALETTENUM ];

private:
	HANDLE m_hfile;
	char* m_buf;
	int m_bufleng;
	int m_pos;

	char m_block[ MPBLOCKLENG ];

	int m_mpno;


};


#endif