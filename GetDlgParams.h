#ifndef GETDLGPARAMSH
#define GETDLGPARAMSH

#include <atlhost.h>

int GetName( CWindow srcwnd, char* dstptr );
int GetText( CWindow srcwnd, char* dstptr, int maxleng );
int GetFloat( CWindow srcwnd, float* dstptr );
int GetInt( CWindow srcwnd, int* dstptr );
int GetDouble( CWindow srcwnd, double* dstptr );


#endif