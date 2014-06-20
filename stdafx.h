// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef STRICT
#define STRICT
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows 95 および Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
//#define WINVER 0x0400		// これを Windows 98 および Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
//#define WINVER 0x0501
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
//#define _WIN32_WINNT 0x0400	// これを Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
//#define _WIN32_WINNT 0x0501
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
//#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
//#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINDOWS 0x0500
#endif

#ifndef _WIN32_IE			// IE 4.0 以降のバージョンに固有の機能の使用を許可します。
//#define _WIN32_IE 0x0400	// これを IE 5.0  またはそれ以降のバージョン向けに適切な値に変更してください。
//#define _WIN32_IE 0x0600
#define _WIN32_IE 0x0500
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

// 一般的で無視しても安全な MFC の警告メッセージの一部の非表示を解除します。
#define _ATL_ALL_WARNINGS

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#endif


//MFC
//#ifdef NEEDMFCH
//#include <afxwin.h>
//#include <afxdisp.h>
//#endif

//#ifdef NEEDAFXDLGSH
//#include <Afxdlgs.h>
//#endif


#include "resource.h"
//#include <atlbase.h>
//#include <atlcom.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

#include <stdio.h>
#include <windows.h>

using namespace ATL;
