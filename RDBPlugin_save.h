#ifndef RDBPLUGINH
#define RDBPLUGINH


// プリプロセッサでRDBPLUGIN_VERSIONが定義されていなければver1.0.7.6として扱う
#ifndef RDBPLUGIN_VERSION
#define RDBPLUGIN_VERSION	1076
#endif

#ifndef RDBPLUGIN_EXPORTS
#define RDBPLUGIN_API extern "C" __declspec(dllexport)
#else
#define RDBPLUGIN_API extern "C" __declspec(dllimport)
#endif


RDBPLUGIN_API int RDBDbgOut( char* lpFormat, ... );
RDBPLUGIN_API int RDBGetCurrentHSID( int* hsidptr );


#endif