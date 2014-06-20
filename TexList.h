#ifndef TEXLISTH
#define TEXLISTH

#include "resource.h"       // ÉÅÉCÉì ÉVÉìÉ{Éã
#include <atlhost.h>

#include <commctrl.h>

#include <coef.h>

class CTexKeyDlg;

/////////////////////////////////////////////////////////////////////////////
// CTexList
class CTexList : 
	public CWindowImpl<CTexList>
{
//public:
	CTexList( CTexKeyDlg* srcdlg );
//	~CTexList();

//	int SetParams( CTexKeyDlg* srcdlg, HWND srchwnd );


//BEGIN_MSG_MAP(CTexList)
//	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
//END_MSG_MAP()
//
//	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CTexKeyDlg* m_texkeydlg;

};

#endif