// RokDeBone2.cpp : WinMain の実装

//#define NEEDMFCH
//#define NEEDAFXDLGSH

#include <stdlib.h>
#include <crtdbg.h>


#include "stdafx.h"

#include "resource.h"
#include <initguid.h>
#include "RokDeBone2.h"

#include "atlbase_pick.h"


#include "RokDeBone2_i.c"

#include <math.h>
#include <stdio.h>
#include <D3DX9.h>
//#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"

#include "viewer.h"
//#include "SigTreeView2.h"

#include "inidata.h"

#define DBGH
#include <dbg.h>


// global
CMyD3DApplication* g_d3dapp = 0;


class CRokDeBone2Module : public CAtlExeModule2T< CRokDeBone2Module >
{
public :
	DECLARE_LIBID(LIBID_RokDeBone2Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ROKDEBONE2, "{F6F690C3-DFAF-4960-9BFB-89FDC636FCC4}")
};


CRokDeBone2Module _AtlModule;

//BEGIN_OBJECT_MAP(ObjectMap)
//OBJECT_ENTRY(CLSID_SigTreeView2, CSigTreeView2)
//END_OBJECT_MAP()



HINSTANCE g_hinstance = 0;
HINSTANCE g_resinstance = 0;

CIniData*	g_inidata = 0;

//

LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

const DWORD dwTimeOut = 5000; // EXEがシャットダウンするまでのアイドル時間です
const DWORD dwPause = 1500; // スレッドが終わるのを待つ時間です


void myInvalidParameterHandler(const wchar_t* expression,
   const wchar_t* function, 
   const wchar_t* file, 
   unsigned int line, 
   uintptr_t pReserved)
{
	/***
   printf("Invalid parameter detected in function %s."
            " File: %s Line: %d\n", function, file, line);
   printf("Expression: %s\n", expression);
	***/

	DbgOut( "Invalid parameter detected in function %s."
            " File: %s Line: %d   error !!!\n", function, file, line );
	DbgOut( "Expression: %s\n", expression );


   abort();//!!!!!!!!!!!!!!!!!!!!
}


extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR lpCmdLine, int nShowCmd)
{

//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_MAP_ALLOC );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

//_CrtSetBreakAlloc(4031);
//_CrtSetBreakAlloc(9594);
//_CrtSetBreakAlloc(9590);
//	_CrtSetBreakAlloc(64);

	lpCmdLine = GetCommandLine(); // この行は _ATL_MIN_CRT のために必要です

	_AtlModule.InitializeCom();//!!!

    TCHAR szTokens[] = _T("-/");
	HRESULT hRes;

    int nRet = 0;
    BOOL bRun = TRUE;
	
    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
        {
            //_AtlModule.UpdateRegistryFromResource(IDR_ROKDEBONE2, FALSE);
            //nRet = _AtlModule.UnregisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
        {
            //_AtlModule.UpdateRegistryFromResource(IDR_ROKDEBONE2, TRUE);
            //nRet = _AtlModule.RegisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        lpszToken = FindOneOf(lpszToken, szTokens);
    }
	

	// INVALID Parameter Handler !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	_invalid_parameter_handler oldHandler, newHandler;
	newHandler = myInvalidParameterHandler;
	oldHandler = _set_invalid_parameter_handler(newHandler);


    if (bRun)
    {
        _AtlModule.StartMonitor();
/***
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
        hRes = _AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
            REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
        _ASSERTE(SUCCEEDED(hRes));
        hRes = CoResumeClassObjects();
#else
        hRes = _AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
            REGCLS_MULTIPLEUSE);
#endif
***/
        hRes = _AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
            REGCLS_MULTIPLEUSE);

		_ASSERTE(SUCCEEDED(hRes));

		/**
        MSG msg;
        while (GetMessage(&msg, 0, 0, 0))
           DispatchMessage(&msg);
		**/

		g_inidata = new CIniData();
		if( !g_inidata ){
			_ASSERT( g_inidata );
			return 0;
		}


		CMyD3DApplication d3dApp;
		g_d3dapp = &d3dApp;

		if( FAILED( d3dApp.CreateWnd( hInstance ) ) )
			return 0;


		/***		
		// error handler のテスト
		char errtest[256];
		char errtest2[512];
		memset( errtest, 0xFF, 200 );
		errtest[200] = 0;
		memset( errtest2, 0x01, 500 );
		errtest2[500] = 0;
		strcat_s( errtest, 256, errtest2 );
		***/



		d3dApp.Run();
DbgOut( "checkend : rdb2 : aft run \r\n" );		
		if( g_inidata ){
			delete g_inidata;
			g_inidata = 0;
		}

DbgOut( "checkend : rdb2 : inidata delete \r\n" );		

        _AtlModule.RevokeClassObjects();
DbgOut( "checkend : rdb2 : RevokeClassObjects \r\n" );		

        Sleep(dwPause); //スレッドが終了するまで待ちます

DbgOut( "checkend : rdb2 : Sleep \r\n" );		

    }

	//ret = g_inidata->SaveParams( "rdb2.ini" );//!!!!!!!!!!!!!!!!!


    _AtlModule.Term();
    //CoUninitialize();
	_AtlModule.UninitializeCom();//!!!

DbgOut( "checkend : rdb2 : Unini Com \r\n" );		
	CloseDbgFile();

    //return nRet;
	return 0;

    //return _AtlModule.WinMain(nShowCmd);
}

