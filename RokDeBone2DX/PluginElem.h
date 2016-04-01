#ifndef PLUGINELEMH
#define PLUGINELEMH


class CPluginElem
{
public:
	CPluginElem();
	~CPluginElem();


public:
	HMODULE hModule;
	char filepath[_MAX_PATH];
	DWORD pluginversion;
	DWORD product;
	DWORD id;
	
	int menuid;

	int validflag;

	char pluginname[_MAX_PATH];


	int (*RDBOnSelectPlugin)();
	int (*RDBOnClose)();
	int (*RDBOnPose)( int motid );

public:
	int CallOnSelectPlugin();
	int CallOnClose();
	int CallOnPose( int motid );

	int SetFilePath( char* pluginpath );
	int LoadPlugin();

private:
	int UnloadPlugin();


private:
	const char* (*RDBGetPlugInName)();
	int (*RDBGetPlugInID)(DWORD *Product, DWORD *ID);
	int (*RDBCheckVersion)( DWORD exe_version, DWORD* pluginver );
	int (*RDBInit)( char *exe_name );

};

#endif

