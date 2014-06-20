// WriteMAFDlg.cpp : CWriteMAFDlg のインプリメンテーション

//#define NOTWINDOWSH
//#define NEEDMFCH
//#define NEEDAFXDLGSH

#include "stdafx.h"

#include <shlobj.h> //shell

#include "WriteMAFDlg.h"

#include <Commdlg.h>
//#include <Afxdlgs.h>


#define DBGH
#include <dbg.h>

#include "GetDlgParams.h"
#include "LoadNameDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CWriteMAFDlg

CWriteMAFDlg::CWriteMAFDlg( HWND appwnd, CLoadNameDlg* namedlg )
{
	m_appwnd = appwnd;
	m_namedlg = namedlg;

	ZeroMemory( m_mafname, MAFNAMELENG );
	ZeroMemory( m_parent, MAX_PATH );
	m_savemethod = 0;

}

CWriteMAFDlg::~CWriteMAFDlg()
{
}

LRESULT CWriteMAFDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWnd();


	if( m_namedlg->m_moaname[0] ){
		char* lasten;
		lasten = strrchr( m_namedlg->m_moaname, '\\' );
		int leng;
		if( lasten ){
			leng = (int)( lasten - m_namedlg->m_moaname );
		}else{
			leng = (int)strlen( m_namedlg->m_moaname );
		}

		strncpy_s( m_parent, MAX_PATH, m_namedlg->m_moaname, leng );
		*( m_parent + leng ) = 0;

		/////////////
		m_savemethod = 1;
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NEW, IDC_RADIO_OLD, IDC_RADIO_OLD );
	}

	ParamsToDlg();

	return 1;  // システムにフォーカスを設定させます
}

LRESULT CWriteMAFDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
///
	int ret = 0;

	char tempchar[MAX_PATH];
	ret = GetText( m_mafname_wnd, tempchar, MAFNAMELENG );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "moaファイル名が不正です(32バイトまで)", "入力エラー", MB_OK );
		return 0;		
	}
	ZeroMemory( m_mafname, MAFNAMELENG );
	if( tempchar[0] ){
		strcpy_s( m_mafname, MAFNAMELENG, tempchar );
	}

	tempchar[0] = 0;
	ret = GetText( m_parent_wnd, tempchar, MAX_PATH );
	if( ret || (tempchar[0] == 0) ){
		MessageBox( "ディレクトリ名が不正です。", "入力エラー", MB_OK );
		return 0;
	}
	strcpy_s( m_parent, MAX_PATH, tempchar );


	UINT ischecked;
	ischecked = m_dlg_wnd.IsDlgButtonChecked( IDC_RADIO_NEW );
	if( ischecked == BST_CHECKED ){
		m_savemethod = 0;
	}else{
		m_savemethod = 1;
	}



	/////////////////
	if( m_savemethod == 0 ){
		int parentleng;
		parentleng = (int)strlen( m_parent );
		int mafleng;
		mafleng = (int)strlen( m_mafname );

		int newfileleng;
		newfileleng = parentleng + mafleng + mafleng + 4 + 2;//parent\mafname\mafname.moa

		if( newfileleng >= MAX_PATH ){
			::MessageBox( m_hWnd, "パス文字列が長すぎます。\nもっと階層の浅い場所を指定しなおしてください。", "文字列長エラー", MB_OK );
			return 0;
		}

		sprintf_s( m_newdirname, MAX_PATH, "%s\\%s", m_parent, m_mafname );
	}else{
		strcpy_s( m_newdirname, MAX_PATH, m_parent );
	}


	DWORD fattr;
	fattr = GetFileAttributes( m_newdirname );

//char messtr[MAX_PATH];
//sprintf_s( messtr, MAX_PATH, "%s-->%x", m_newdirname, fattr );
//::MessageBox( m_hWnd, messtr, "check !!!", MB_OK );

	if( (fattr == -1) || ((fattr & FILE_ATTRIBUTE_DIRECTORY) == 0) ){
		int bret;
		bret = CreateDirectory( m_newdirname, NULL );
		if( bret == 0 ){
			::MessageBox( m_hWnd, "ディレクトリの作成に失敗しました。\nmoaの保存に失敗しました。", "エラー", MB_OK );
			m_newdirname[0] = 0;//!!!!!!!!!!!!!
		}else{
			//::MessageBox( m_hWnd, m_newdirname, "ディレクトリを作成しました。", MB_OK );
		}
	}


	EndDialog(wID);

	return 0;
}

LRESULT CWriteMAFDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CWriteMAFDlg::SetWnd()
{

	m_dlg_wnd = m_hWnd;

	m_mafname_wnd = GetDlgItem( IDC_MAFNAME );
	m_parent_wnd = GetDlgItem( IDC_PARENT );
}

int CWriteMAFDlg::ParamsToDlg()
{	
	m_mafname_wnd.SetWindowText( m_mafname );
	m_parent_wnd.SetWindowText( m_parent );

	if( m_savemethod == 0 ){
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NEW, IDC_RADIO_OLD, IDC_RADIO_NEW );
	}else{
		m_dlg_wnd.CheckRadioButton( IDC_RADIO_NEW, IDC_RADIO_OLD, IDC_RADIO_OLD );
	}

	return 0;
}

/***
HRESULT SHGetFolderLocation(          HWND hwndOwner,
    int nFolder,
    HANDLE hToken,
    DWORD dwReserved,
    LPITEMIDLIST *ppidl
);
CSIDL_RECENT




// Overload to get from PIDL.
CString CFileDlgHelper::GetDisplayNameOf(LPCITEMIDLIST pidl, DWORD flags)
{
   CString path;

   // First, get PIDL of current folder by sending CDM_GETFOLDERIDLIST
   // Get length first, then allocate.
   int len = m_pDlg->GetParent()->SendMessage(CDM_GETFOLDERIDLIST, 0,NULL);
   if (len>0) {
      CComQIPtr<IMalloc> malloc;
      SHGetMalloc(&malloc);
      LPCITEMIDLIST pidlFolder = (LPCITEMIDLIST)malloc->Alloc(len);
      ASSERT(pidlFolder);
      m_pDlg->GetParent()->SendMessage(CDM_GETFOLDERIDLIST, len,
         (LPARAM)(void*)pidlFolder);

      // Now get IShellFolder for pidlFolder
      CComQIPtr<IShellFolder> ishDesk;
      SHGetDesktopFolder(&ishDesk);
      CComQIPtr<IShellFolder> ishFolder;
      ishDesk->BindToObject(pidlFolder, NULL,
         IID_IShellFolder, (void**)&ishFolder);

      // finally, get the path name from pidlFolder
      path = GetDisplayNameOf(ishFolder, pidl, flags);

      malloc->Free((void*)pidlFolder);
   }
   return path;
}

LPVOID CoTaskMemAlloc(
  ULONG cb
); 


SFGAO_FOLDER
HRESULT ParseDisplayName(          HWND hwnd,
    LPBC pbc,
    LPOLESTR pwszDisplayName,
    ULONG *pchEaten,
    LPITEMIDLIST *ppidl,
    ULONG *pdwAttributes
);

SHSTDAPI SHParseDisplayName(PCWSTR pszName, IBindCtx *pbc, LPITEMIDLIST *ppidl, SFGAOF sfgaoIn, SFGAOF *psfgaoOut);

***/


LRESULT CWriteMAFDlg::OnRef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	//LPITEMIDLIST lpidl;
	
	//LPITEMIDLIST pidlRoot = 0;
	//SHGetFolderLocation( m_appwnd, CSIDL_RECENT, NULL, NULL, &pidlRoot );
	//SHGetSpecialFolderLocation( m_appwnd, CSIDL_RECENT, &pidlRoot );

	/***
	char mname[2048];
	strcpy_s( mname, 2048, m_namedlg->m_moaname );
	char* lasten = 0;
	lasten = strrchr( mname, '\\' );
	if( lasten ){
		*lasten = 0;
	}
	WCHAR wname[2048];
	int result1;
	result1 = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, mname, -1, wname, 2048 );
	LPITEMIDLIST pidlRoot = 0;
	if( result1 != 0 ){
		HRESULT hr;
		DWORD attrib = SFGAO_FOLDER;
		DWORD attribout = 0;
		//hr = ParseDisplayName( m_appwnd, NULL, wname, NULL, &pidlRoot, &attrib );
		hr = SHParseDisplayName( wname, NULL, &pidlRoot, attrib, &attribout );
		if( hr != S_OK ){
			pidlRoot = 0;
		}
	}
	***/

	BROWSEINFO bi;
	char dispname[MAX_PATH];
	char selectname[MAX_PATH];
	int iImage = 0;

	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;//!!!!!!!
	bi.pszDisplayName = dispname;
	bi.lpszTitle = "親フォルダを選択してください。";
	//bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = iImage;

	LPITEMIDLIST curlpidl = 0;
	curlpidl = SHBrowseForFolder( &bi );
	if( curlpidl ){
		//::MessageBox( m_hWnd, dispname, "フォルダー名", MB_OK );

		BOOL bret;
		bret = SHGetPathFromIDList( curlpidl, selectname );
		if( bret == FALSE ){
			DbgOut( "viewer : WriteMAFile : SHGetPathFromIDList error !!!\n" );
			_ASSERT( 0 );
			if( curlpidl )
				CoTaskMemFree( curlpidl );
			return 1;
		}

		if( curlpidl )
			CoTaskMemFree( curlpidl );

		m_parent[0] = 0;
		strcpy_s( m_parent, MAX_PATH, selectname );
		m_parent_wnd.SetWindowTextA( m_parent );

	}

	return 0;
}

