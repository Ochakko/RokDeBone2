#ifndef RDBPLUGINH
#define RDBPLUGINH


// �v���v���Z�b�T��RDBPLUGIN_VERSION����`����Ă��Ȃ����ver1.0.7.6�Ƃ��Ĉ���
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