#ifndef COLDLGH_
#define COLDLGH_

#include <coef.h>


class CColDlg
{
public:
	CColDlg();
	~CColDlg();

	int Choose( HWND srchwnd, COLORREF* dstcol );

	int SetCustomColor( COLORREF* srccol );

private:
	int InitParams();

public:
	CHOOSECOLOR m_cc;
	COLORREF m_custom[16];



};

#endif