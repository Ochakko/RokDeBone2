#ifndef BYTEORDERH
#define BYTEORDERH


#include <d3dx9.h>


#define DBGH
#include <dbg.h>

	int CheckDataSize();

	short N2HS( short orgshort );
	int N2HI( int orgint );

	unsigned short N2HUS( unsigned short orgshort );
	unsigned int N2HUI( unsigned int orgint );
	
	float N2HF( float orgfloat );


#endif