// stdafx.cpp : 標準インクルードのみを含むソース ファイルです。
// RokDeBone2.pch は、プリコンパイル済みヘッダーになります。
//  stdafx.obj にはプリコンパイル型情報が含まれます。

//#define NEEDMFCH
//#define NEEDAFXDLGSH
#include "stdafx.h"

#define COEFHSTRING
#include <coef.h>


int (WINAPIV* __vsnprintf)(char*, size_t, const char*, va_list) = _vsnprintf;

