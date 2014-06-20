
//text fileによるロードの廃止につき、
//ダミー関数のみに変更
#include "StdAfx.h"
#include <stdio.h>
//#include	<io.h>

//#include <coef.h>

#include <windows.h>
//#include <wchar.h>

#include <treehandler2.h>
#include <shdhandler.h>
#include <mothandler.h>
#include <matrix2.h>
//#include <basedat.h>


//#define	DBGH
//#include "dbg.h"


int LoadShdData( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, LPCTSTR fname );
void FreeShdLexBuff();


int LoadShdData( CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, LPCTSTR fname )
{
	return 1;
}
void FreeShdLexBuff()
{
}