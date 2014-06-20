#ifndef ROKDEBONE2MODULEH
#define ROKDEBONE2MODULEH

#include "stdafx.h"
#include "resource.h"
#include "RokDeBone2.h"
#include <atlbase.h>

class CRokDeBone2Module : public CAtlExeModule2T< CRokDeBone2Module >
{
public :
	DECLARE_LIBID(LIBID_RokDeBone2Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ROKDEBONE2, "{F6F690C3-DFAF-4960-9BFB-89FDC636FCC4}")
};

#endif
