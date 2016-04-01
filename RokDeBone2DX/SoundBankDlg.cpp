#include "stdafx.h"
#include "SoundBankDlg.h"
#include "GetDlgParams.h"

#include "SoundBank.h"
#include "SoundSet.h"
#include "SoundElem.h"

#define DBGH
#include <dbg.h>

#include <SndPath.h>

#include <crtdbg.h>

//extern HINSTANCE g_resinstance;

#define CHG_SETNAME		1
#define CHG_DIR			2
#define CHG_BGM			4
#define CHG_SOUND		8

#define UPD_SETNAME		(CHG_SETNAME + CHG_DIR + CHG_BGM + CHG_SOUND)
#define UPD_DIR			(CHG_DIR + CHG_BGM + CHG_SOUND )
#define UPD_BGM			(CHG_BGM + CHG_SOUND)
#define UPD_SOUND		CHG_SOUND


/////////////////////////////////////////////////////////////////////////////
// CSoundBankDlg

CSoundBankDlg::CSoundBankDlg( CHuSound* srchs, CSoundBank* srcsb )
{
	InitParams();

	m_soundbank = srcsb;
	m_husound = srchs;
}

CSoundBankDlg::~CSoundBankDlg()
{
	DestroyObjs();
}

void CSoundBankDlg::InitParams()
{
	m_setlistno = -1;
	m_curss = 0;
	ZeroMemory( m_dir, sizeof( char ) * MAX_PATH );
	ZeroMemory( m_pathname, sizeof( char ) * MAX_PATH );

	m_sndcombonum = 0;
	m_sndcombo = 0;

	m_bgmlistno = 0;
	m_sndcomboindex = 0;
	m_sndlistindex = -1;
}

int CSoundBankDlg::DestroyObjs()
{
	DestroySndCombo();

	return 0;
}

int CSoundBankDlg::DestroySndCombo()
{
	if( m_sndcombo ){
		free( m_sndcombo );
		m_sndcombo = 0;
	}
	m_sndcombonum = 0;

	return 0;
}

LRESULT CSoundBankDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int ret;

	InitCommonControls();

//	InitParams();

	m_dlg_wnd = m_hWnd;

	m_setname_wnd = GetDlgItem( IDC_SETNAME );
	_ASSERT( m_setname_wnd );
	m_setlist_wnd = GetDlgItem( IDC_SETLIST );
	_ASSERT( m_setlist_wnd );

	m_dir_wnd = GetDlgItem( IDC_DIR );
	_ASSERT( m_dir_wnd );

	m_bgm_wnd = GetDlgItem( IDC_COMBOBGM );
	_ASSERT( m_bgm_wnd );
	m_bgmrev_wnd = GetDlgItem( IDC_BGMREVERB );
	_ASSERT( m_bgmrev_wnd );

	m_snd_wnd = GetDlgItem( IDC_COMBOSOUND );
	_ASSERT( m_snd_wnd );
	m_sndrev_wnd = GetDlgItem( IDC_REVERB );
	_ASSERT( m_sndrev_wnd );
	m_sndbufnum_wnd = GetDlgItem( IDC_BUFNUM );
	_ASSERT( m_sndbufnum_wnd );

	m_sndlist_wnd = GetDlgItem( IDC_SOUNDLIST );
	_ASSERT( m_sndlist_wnd );


	ret = MakePathName( 1 );
	_ASSERT( !ret );

	ret = ResetBGM( 0 );
	_ASSERT( !ret );

	ret = ResetSound( 0 );
	_ASSERT( !ret );




	ret = ParamsToDlg( UPD_SETNAME );
	_ASSERT( !ret );


	return 1;  // システムにフォーカスを設定させます
}

LRESULT CSoundBankDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
//	int ret;
//	ret = GetName( m_xname_wnd, xfilename );
//	if( ret ){
//		MessageBox( "X File名が不正です。", "エラー", MB_OK );
//		return 0;
//	}

//////////
//	float tempfloat;
//	ret = GetFloat( m_mult_wnd, &tempfloat );
//	if( ret || (tempfloat < 0.0f) ){
//		MessageBox( "倍率の値が不正です。正の小数", "エラー", MB_OK );
//		return 0;
//	}
//	mult = tempfloat;


//////////
//	UINT checkflag;
//	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_CONVFACE );
//	if( checkflag == BST_CHECKED ){
//		convface = 1;
//	}else if( checkflag == BST_UNCHECKED ){
//		convface = 0;
//	}


	EndDialog(wID);
	return 0;
}

LRESULT CSoundBankDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

int CSoundBankDlg::ParamsToDlg( int updflag )
{
	int ret;


	if( updflag & CHG_SETNAME ){
		ret = InitSetList();
		_ASSERT( !ret );
	}

	if( updflag & CHG_DIR ){
		ret = ParamsToDir();
		_ASSERT( !ret );
	}

	if( updflag & CHG_BGM ){
		ret = ParamsToBGM();
		_ASSERT( !ret );
	}

	if( updflag & CHG_SOUND ){
		ret = ParamsToSound();
		_ASSERT( !ret );
	}

	m_dlg_wnd.UpdateWindow();
	m_bgm_wnd.UpdateWindow();
	m_snd_wnd.UpdateWindow();

	return 0;
}

int CSoundBankDlg::ParamsToDir()
{
	if( m_curss ){
		m_dir_wnd.SetWindowText( m_curss->m_dirname );
	}else{
		m_dir_wnd.SetWindowText( "" );
	}

	return 0;
}

int CSoundBankDlg::ParamsToBGM()
{
	::SendMessage( m_bgm_wnd, CB_SETCURSEL, m_bgmlistno, 0 );

	if( !m_curss ){
		return 0;
	}

	//char tempchar[256];
	if( m_curss->m_bgm ){
		if( m_curss->m_bgm->m_reverbflag ){
			m_dlg_wnd.CheckDlgButton( IDC_BGMREVERB, TRUE );
		}else{
			m_dlg_wnd.CheckDlgButton( IDC_BGMREVERB, FALSE );
		}
	}else{
		m_dlg_wnd.CheckDlgButton( IDC_BGMREVERB, TRUE );
	}

	return 0;
}

int CSoundBankDlg::ParamsToSound()
{

	::SendMessage( m_sndlist_wnd, LB_RESETCONTENT, 0, 0 );
	if( !m_curss ){
		return 0;
	}

	if( (m_sndcomboindex >= 0) && (m_sndcomboindex < m_sndcombonum) ){
		::SendMessage( m_snd_wnd, CB_SETCURSEL, m_sndcomboindex, 0 );
	}else if( m_sndcomboindex < 0 ){
		m_sndcomboindex = 0;
		::SendMessage( m_snd_wnd, CB_SETCURSEL, m_sndcomboindex, 0 );
	}

	int sno;
	for( sno = 0; sno < m_curss->m_soundnum; sno++ ){
		CSoundElem* curse = *( m_curss->m_ppsound + sno );
		::SendMessage( m_sndlist_wnd, LB_ADDSTRING, 0, (LPARAM)curse->m_filename );
		::SendMessage( m_sndlist_wnd, LB_SETITEMDATA, sno, curse->m_serialno );
	}

	if( (m_sndlistindex >= 0) && (m_curss->m_soundnum >= 0) ){
		int sndseri;
		sndseri = (int)::SendMessage( m_sndlist_wnd, LB_GETITEMDATA, m_sndlistindex, 0 );
		if( sndseri == CB_ERR ){
			return 0;
		}

		CSoundElem* curse;
		curse = m_curss->GetSoundElem( sndseri );
		if( curse ){
			char tempchar[256];
			sprintf_s( tempchar, 256, "%d", curse->m_bufnum );
			m_sndbufnum_wnd.SetWindowText( tempchar );

			if( curse->m_reverbflag ){
				m_dlg_wnd.CheckDlgButton( IDC_REVERB, TRUE );
			}else{
				m_dlg_wnd.CheckDlgButton( IDC_REVERB, FALSE );
			}

			::SendMessage( m_sndlist_wnd, LB_SETCURSEL, m_sndlistindex, 0 );

		}
	}

	return 0;
}

/***
LRESULT CSoundBankDlg::OnRefX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char buf[_MAX_PATH];
	strcpy_s( buf, _MAX_PATH, "default.x" );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = (LPCTSTR)"X FILE (*.x)\0*.x\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPTSTR)buf;
	ofn.nMaxFile =sizeof(buf);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt ="x";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetSaveFileName(&ofn) == 0 )
		return 0;

	//ZeroMemory( name, _MAX_PATH );
	//strcpy( name, buf );
	m_xname_wnd.SetWindowText( buf );

	return 0;
}
***/
/***
int CSoundBankDlg::InitCombo()
{
	int ret;

	m_combo_wnd.SendMessage( CB_RESETCONTENT, 0, 0 );

	int setno = 0;
	char tempchar[256];
	strcpy_s( tempchar, 256, "全て" );
	m_combo_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
	m_combo_wnd.SendMessage( CB_SETITEMDATA, setno, (LPARAM)(MAXMOTIONNUM + 1) );
	setno++;

	int mno;
	for( mno = 0; mno < motnum; mno++ ){
		MOTID* curmotid = motidarray + mno;
		int motid = curmotid->id;
		ZeroMemory( tempchar, sizeof( char ) * 256 );
		ret = lpmh->GetMotionName( motid, tempchar );
		if( ret ){
			DbgOut( "SoundBankDlg : InitCombo : GetMotionName error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		m_combo_wnd.SendMessage( CB_ADDSTRING, 0, (LPARAM)tempchar );
		m_combo_wnd.SendMessage( CB_SETITEMDATA, setno, (LPARAM)motid );
		setno++;
	}

	m_combo_wnd.SendMessage( CB_SETCURSEL, 0, 0 );


	return 0;
}
***/

int CSoundBankDlg::InitSetList()
{
	m_setlist_wnd.SendMessage( LB_RESETCONTENT, 0, 0 );

	if( !m_soundbank ){
		return 0;
	}

	int ssno;
	for( ssno = 0; ssno < m_soundbank->m_setnum; ssno++ ){
		CSoundSet* curss = *( m_soundbank->m_ppset + ssno );

		m_setlist_wnd.SendMessage( LB_ADDSTRING, 0, (LPARAM)curss->m_setname );
		m_setlist_wnd.SendMessage( LB_SETITEMDATA, ssno, curss->m_setid );
	}
	
	if( m_soundbank->m_setnum > 0 ){
		if( (m_setlistno >= 0) && (m_setlistno <= (m_soundbank->m_setnum - 1)) ){
			m_setlist_wnd.SendMessage( LB_SETCURSEL, m_setlistno, 0 );
		}else{
			m_setlistno = 0;
			m_setlist_wnd.SendMessage( LB_SETCURSEL, m_setlistno, 0 );
		}

		m_curss = *( m_soundbank->m_ppset + m_setlistno );
		m_setname_wnd.SetWindowText( m_curss->m_setname );

	}else{
		m_curss = 0;
		m_setlistno = -1;
	}

	return 0;
}

LRESULT CSoundBankDlg::OnAddSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	if( !m_soundbank ){
		return 0;
	}

	char setname[32];
	ZeroMemory( setname, sizeof( char ) * 32 );
	ret = GetText( m_setname_wnd, setname, 32 );
	if( ret || (setname[0] == 0) ){
		::MessageBox( m_hWnd, "セット名が不正です。３２バイト以内。", "入力エラー", MB_OK );
		_ASSERT( 0 );
		return 0;
	}

	CSoundSet* samess = 0;
	ret = m_soundbank->GetSoundSet( setname, &samess ); 
	if( ret ){
		DbgOut( "sbdlg : OnAddSet : sb GetSoundSet check same error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	if( samess ){
		::MessageBox( m_hWnd, "このセット名は既に存在します。名前を変えて再試行してください。", "エラー", MB_OK );
		_ASSERT( 0 );
		return 0;
	}

	CSoundSet* newss;
	int newssid = -1;
	newss = m_soundbank->AddSoundSet( setname, &newssid );
	if( ret || (newssid < 0) ){
		DbgOut( "sbdlg : OnAddSet : sb AddSoundSet error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	m_setlistno = m_soundbank->m_setnum - 1;
	m_curss = *( m_soundbank->m_ppset + m_setlistno );
	strcpy_s( m_dir, MAX_PATH, m_curss->m_dirname );

	ret = MakePathName( 0 );
	_ASSERT( !ret );

	ret = ResetBGM( 1 );
	_ASSERT( !ret );

	ret = ResetSound( 1 );
	_ASSERT( !ret );

	ret = ParamsToDlg( UPD_SETNAME );
	if( ret ){
		DbgOut( "sbdlg : OnAddSet : InitSetList error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}
LRESULT CSoundBankDlg::OnDelSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_soundbank ){
		return 0;
	}
	if( m_soundbank->m_setnum <= 0 ){
		return 0;
	}

	int listno;
	listno = (int)m_setlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= m_soundbank->m_setnum) ){
		DbgOut( "SoundBankDlg : OnDelSet listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int itemno;
	itemno = (int)m_setlist_wnd.SendMessage( LB_GETITEMDATA, listno, 0 );
	if( itemno == LB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = m_soundbank->DelSoundSet( itemno );
	if( ret ){
		DbgOut( "SoundBankDlg : OnDelSet : sb DelSoundSet error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( m_soundbank->m_setnum <= 0 ){
		m_setlistno = -1;
	}else{
		m_setlistno = 0;
	}


	ret = ParamsToDlg( UPD_SETNAME );
	_ASSERT( !ret );

	return 0;
}
LRESULT CSoundBankDlg::OnUpSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_soundbank ){
		return 0;
	}
	if( m_soundbank->m_setnum <= 0 ){
		return 0;
	}

	int listno;
	listno = (int)m_setlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= m_soundbank->m_setnum) ){
		DbgOut( "SoundBankDlg : OnUpSet listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int itemno;
	itemno = (int)m_setlist_wnd.SendMessage( LB_GETITEMDATA, listno, 0 );
	if( itemno == LB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = m_soundbank->UpSoundSet( itemno );
	if( ret ){
		DbgOut( "SoundBankDlg : OnUpSet : sb UpSoundSet error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( listno > 0 ){
		m_setlistno = listno - 1;
	}

	ret = ParamsToDlg( UPD_SETNAME );
	_ASSERT( !ret );


	return 0;
}
LRESULT CSoundBankDlg::OnDownSet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_soundbank ){
		return 0;
	}
	if( m_soundbank->m_setnum <= 0 ){
		return 0;
	}

	int listno;
	listno = (int)m_setlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= m_soundbank->m_setnum) ){
		DbgOut( "SoundBankDlg : OnDownSet listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int itemno;
	itemno = (int)m_setlist_wnd.SendMessage( LB_GETITEMDATA, listno, 0 );
	if( itemno == LB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = m_soundbank->DownSoundSet( itemno );
	if( ret ){
		DbgOut( "SoundBankDlg : OnDownSet : sb DownSoundSet error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	if( listno < (m_soundbank->m_setnum - 1) ){
		m_setlistno = listno + 1;
	}

	ret = ParamsToDlg( UPD_SETNAME );
	_ASSERT( !ret );

	return 0;
}
LRESULT CSoundBankDlg::OnSelChangeSetList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_soundbank ){
		m_setlistno = -1;
	}else if( m_soundbank->m_setnum <= 0 ){
		m_setlistno = -1;
	}else if( m_soundbank->m_setnum == 1 ){
		m_setlistno = 0;
	}else{
		int listno;
		listno = (int)m_setlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
		if( (listno == LB_ERR) || (listno < 0) || (listno >= m_soundbank->m_setnum) ){
			m_setlistno = -1;
		}else{
			m_setlistno = listno;
		}
	}
	m_sndcomboindex = 0;
	m_sndlistindex = 0;

	if( m_setlistno >= 0 ){
		m_curss = *( m_soundbank->m_ppset + m_setlistno );
	}else{
		m_curss = 0;
	}


	int ret;

	if( m_setlistno >= 0 ){
		strcpy_s( m_dir, MAX_PATH, m_curss->m_dirname );
	}else{
		ZeroMemory( m_dir, sizeof( char ) * MAX_PATH );
	}

//char temp[512];
//sprintf_s( temp, 512, "selchange : dir %s, listno %d", m_dir, m_setlistno );
//::MessageBox( m_hWnd, temp, "check", MB_OK );
//DbgOut( "check !!! : selchange , dir %s, listno %d\r\n", m_dir, m_setlistno );


	ret = MakePathName( 0 );
	_ASSERT( !ret );


	ret = ResetBGM( 0 );
	_ASSERT( !ret );

	ret = ResetSound( 0 );
	_ASSERT( !ret );

	ParamsToDlg( UPD_SETNAME );
	return 0;
}


LRESULT CSoundBankDlg::OnApplyDir(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	ZeroMemory( m_dir, sizeof( char ) * MAX_PATH );
	ret = GetText( m_dir_wnd, m_dir, MAX_PATH );
	if( ret ){
		::MessageBox( m_hWnd, "ディレクトリ名が不正です。", "エラー", MB_OK );
		return 0;
	}

	m_sndcomboindex = 0;
	m_sndlistindex = 0;

	ret = MakePathName( 0 );
	_ASSERT( !ret );

/////////

	if( m_curss ){
		ret = m_curss->SetDir( m_dir );
		if( ret ){
			DbgOut( "SoundBankDlg : OnApplyDir : ss SetDir error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	ret = ResetBGM( 1 );
	if( ret ){
		DbgOut( "SoundBankDlg : OnApplyDir : ResetBGM error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = ResetSound( 1 );
	if( ret ){
		DbgOut( "SoundBankDlg : OnApplyDir : ResetSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	ret = ParamsToDlg( UPD_DIR );
	_ASSERT( !ret );

	return 0;
}

int CSoundBankDlg::MakePathName( int setdirflag )
{
	int ret;
	if( setdirflag == 1 ){
		ZeroMemory( m_dir, sizeof( char ) * MAX_PATH );
		if( m_soundbank->m_setnum > 0 ){
			m_setlistno = 0;
			m_curss = *( m_soundbank->m_ppset + m_setlistno );
			strcpy_s( m_dir, MAX_PATH, m_curss->m_dirname );
		}else{
			m_setlistno = -1;
			m_curss = 0;
		}
	}

	ZeroMemory( m_pathname, sizeof( char ) * MAX_PATH );

	if( m_curss ){
		ret = SndMakePath( m_dir, m_curss->m_ssfdir, 0, m_pathname );
	}else{
		ret = SndMakePath( m_dir, 0, 0, m_pathname );
	}
	if( ret ){
		DbgOut( "SoundBankDlg : MakePathName : SndMakePath error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

//char temp2[512];
//sprintf_s( temp2, 512, "MakePathName %d, pathname %s", setdirflag, m_pathname );
//::MessageBox( m_hWnd, temp2, "check", MB_OK );


	return 0;
}

int CSoundBankDlg::SetSndCombo( SNDCOMBO* scom, int* dstnum )
{
	//int ret;
	int setno = 0;
	*dstnum = 1;
	if( scom ){
		strcpy_s( (scom + setno)->name, 256, "なし" );
		(scom + setno)->bgmindex = 0;
		(scom + setno)->sndindex = 0;
		//(scom + setno)->isbgm = 0;
	}
	setno++;

	WIN32_FIND_DATA wf;
	HANDLE h;

//	char finddir[MAX_PATH];
//	ZeroMemory( finddir, sizeof( char ) * MAX_PATH );
//	int dirleng;
//	dirleng = (int)strlen( m_pathname );
//	strncpy_s( finddir, MAX_PATH, m_pathname, (dirleng - 1) );// 最後の￥を取る

	h = FindFirstFile( m_pathname, &wf );
	if( h == INVALID_HANDLE_VALUE ){
		DbgOut( "soundbankdlg : SetSndCombo : FindFirstFile error !!!\n" );
		//_ASSERT( 0 );
		return 0;//!!!!!!!!!!!!!
	}
	int issnd = 0;
	issnd = IsSoundFile( &wf );
	if( issnd ){
		if( scom ){
			strcpy_s( (scom + setno)->name, 256, wf.cFileName );
			(scom + setno)->bgmindex = setno;
			(scom + setno)->sndindex = 0;
			//(scom + setno)->isbgm = 0;
		}
		setno++;
	}

	int existfile = 1;
	while( existfile = FindNextFile( h, &wf ) ){		
		issnd = IsSoundFile( &wf );
		if( issnd ){
			if( scom ){
				strcpy_s( (scom + setno)->name, 256, wf.cFileName );
				(scom + setno)->bgmindex = setno;
				(scom + setno)->sndindex = 0;
				//(scom + setno)->isbgm = 0;
			}
			setno++;
		}
	}

	*dstnum = setno;

	return 0;
}

int CSoundBankDlg::FindSndCombo( char* srcname )
{
	if( m_sndcombonum <= 1 ){
		return 0;
	}

	int findindex = 0;
	int cno;
	for( cno = 1; cno < m_sndcombonum; cno++ ){
		int cmp0;
		cmp0 = strcmp( srcname, (m_sndcombo + cno)->name );
		if( cmp0 == 0 ){
			findindex = cno;
			break;
		}
	}

//	_ASSERT( 0 );

	return findindex;
}


int CSoundBankDlg::IsSoundFile( WIN32_FIND_DATA* pwf )
{
	if( pwf->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ){
		return 0;
	}

	char wavpat[10] = ".wav";
	char oggpat[10] = ".ogg";

	char* wavptr = 0;
	char* oggptr = 0;
	wavptr = strstr( pwf->cFileName, wavpat );
	oggptr = strstr( pwf->cFileName, oggpat );

	if( wavptr || oggptr ){
		return 1;
	}else{
		return 0;
	}
}


int CSoundBankDlg::ResetBGM( int delflag )
{
	int ret;

	_ASSERT( m_soundbank );


	//sndcomboの作り直し
	DestroySndCombo();

	ret = SetSndCombo( 0, &m_sndcombonum );
	if( ret ){
		DbgOut( "SoundBankDlg : ResetBGM : SetSndCombo 0 error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	if( m_sndcombonum > 0 ){
		m_sndcombo = (SNDCOMBO*)malloc( sizeof( SNDCOMBO ) * m_sndcombonum );
		if( !m_sndcombo ){
			DbgOut( "SoundBankDlg : ResetBGM : sndcombo alloc error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		ZeroMemory( m_sndcombo, sizeof( SNDCOMBO ) * m_sndcombonum );

		int tmpnum = 0;
		ret = SetSndCombo( m_sndcombo, &tmpnum );
		if( ret ){
			DbgOut( "SoundBankDlg : ResetBGM : SetSndCombo 1 error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
		if( tmpnum != m_sndcombonum ){
			DbgOut( "SoundBankDlg : ResetBGM : sndcombonum error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}


	//comboの初期化
	ret = InitComboBGM();	
	if( ret ){
		DbgOut( "SoundBankDlg : ResetBGM : InitComboBGM error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	//SS削除
	if( delflag && m_curss ){
		ret = m_curss->DelBGM();
		_ASSERT( !ret );

		m_bgmlistno = 0;
	}else{
		if( m_curss ){
			CSoundElem* bgmelem;
			bgmelem = m_curss->m_bgm;
			if( bgmelem ){
				m_bgmlistno = FindSndCombo( bgmelem->m_filename );
			}else{
				m_bgmlistno = 0;
			}
		}else{
			m_bgmlistno = 0;
		}
	}

	return 0;
}

int CSoundBankDlg::InitComboBGM()
{
	::SendMessage( m_bgm_wnd, CB_RESETCONTENT, 0, 0 );

	if( m_curss ){

		int cno;
		for( cno = 0; cno < m_sndcombonum; cno++ ){
			SNDCOMBO* cursc = m_sndcombo + cno;
			LRESULT lret;
			lret = ::SendMessage( m_bgm_wnd, CB_ADDSTRING, 0, (LPARAM)cursc->name );
			if( lret == CB_ERR ){
				DbgOut( "SoundbankDlg : InitComboBGM : AddString error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

	}

	::SendMessage( m_bgm_wnd, CB_SETCURSEL, 0, 0 );

	return 0;
}


int CSoundBankDlg::ResetSound( int delflag )
{
	int ret;

	//comboの初期化
	ret = InitComboSound();
	if( ret ){
		DbgOut( "SoundBankDlg : ResetSound : InitComboSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	//SS削除
	if( delflag || (m_bgmlistno <= 0) ){
		if( m_curss ){
			ret = m_curss->DestroySound();
			_ASSERT( !ret );
		}
		m_sndcomboindex = 0;
		m_sndlistindex = -1;
	}else{
		if( !m_curss ){
			m_sndcomboindex = 0;
			m_sndlistindex = -1;
		}
	}

	return 0;
}

int CSoundBankDlg::InitComboSound()
{
	::SendMessage( m_snd_wnd, CB_RESETCONTENT, 0, 0 );

	if( m_bgmlistno > 0 ){
		int sno;
		int setno = 0;
		for( sno = 0; sno < m_sndcombonum; sno++ ){
			if( sno != m_bgmlistno ){
				::SendMessage( m_snd_wnd, CB_ADDSTRING, 0, (LPARAM)(m_sndcombo + sno)->name );
				::SendMessage( m_snd_wnd, CB_SETITEMDATA, setno, (LPARAM)(DWORD)sno );
				(m_sndcombo + sno)->sndindex = setno;
				setno++;
			}else{
				(m_sndcombo + sno)->sndindex = -1;
			}
		}
	}

	::SendMessage( m_snd_wnd, CB_SETCURSEL, 0, 0 );

	return 0;
}


LRESULT CSoundBankDlg::OnApplyBGM(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;

	if( !m_curss ){
		return 0;
	}
	if( m_sndcombonum <= 1 ){
		return 0;
	}

	int combono;
	combono = (int)::SendMessage( m_bgm_wnd, CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		return 0;
	}

	if( combono == 0 ){
		::MessageBox( m_hWnd, "BGM選択は必須です。", "エラー", MB_OK );
		return 0;
	}
	m_bgmlistno = combono;//!!!!!!!!!!


	int revflag = 0;
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_BGMREVERB );
	if( checkflag == BST_CHECKED ){
		revflag = 1;
	}else{
		revflag = 0;
	}

	int bufnum = 1;
	CSoundElem* newse;
	newse = m_curss->AddSound( m_husound, (m_sndcombo + m_bgmlistno)->name, 1, revflag, bufnum );
	if( !newse ){
		DbgOut( "SoundBankDlg : OnApplyBGM : ss AddSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
/////////

	ret = ResetSound( 1 );
	if( ret ){
		DbgOut( "SoundBankDlg : OnApplyBGM : ResetSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

/////////

	ret = ParamsToDlg( UPD_BGM );
	_ASSERT( !ret );

	return 0;
}

LRESULT CSoundBankDlg::OnAddSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int ret;
	if( !m_soundbank ){
		return 0;
	}
	if( !m_curss ){
		return 0;
	}

	int combono;
	combono = (int)::SendMessage( m_snd_wnd, CB_GETCURSEL, 0, 0 );
	if( combono == CB_ERR ){
		return 0;
	}

	if( combono == 0 ){
		return 0;// 「なし」を選択。
	}

	int sndindex;
	sndindex = (int)::SendMessage( m_snd_wnd, CB_GETITEMDATA, combono, 0 );
	if( (sndindex == CB_ERR) || (sndindex == -1) ){
		_ASSERT( 0 );
		return 0;//!!!!!!!!!!!!!
	}

	if( (sndindex < 0) || (sndindex >= m_sndcombonum) ){
		DbgOut( "SoundBankDlg : OnAddSound : sndindex error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	CSoundElem* samese = 0;
	samese = m_curss->GetSoundElem( (m_sndcombo + sndindex)->name );
	if( samese ){
		::MessageBox( m_hWnd, "この効果音は既に存在します。追加できません。", "エラー", MB_OK );
		return 0;
	}

	int revflag = 1;
	UINT checkflag;
	checkflag = m_dlg_wnd.IsDlgButtonChecked( IDC_REVERB );
	if( checkflag == BST_CHECKED ){
		revflag = 1;
	}else if( checkflag == BST_UNCHECKED ){
		revflag = 0;
	}

	int bufnum = 5;
	ret = GetInt( m_sndbufnum_wnd, &bufnum );
	if( ret || (bufnum <= 0) ){
		::MessageBox( m_hWnd, "効果音のbufnumが不正です。", "エラー", MB_OK );
		return 0;
	}

	CSoundElem*  newse;
	newse = m_curss->AddSound( m_husound, (m_sndcombo + sndindex)->name, 0, revflag, bufnum );
	if( !newse ){
		DbgOut( "SoundBankDlg : OnAddSound : ss AddSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_sndcomboindex = sndindex;
	m_sndlistindex = m_curss->m_soundnum - 1;

	ret = ParamsToDlg( UPD_SOUND );
	if( ret ){
		DbgOut( "sbdlg : OnAddSound : ParamsToDlg error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	
	return 0;
}
LRESULT CSoundBankDlg::OnDelSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_curss ){
		return 0;
	}
	if( m_curss->m_soundnum <= 0 ){
		return 0;
	}

	int listno;
	listno = (int)m_sndlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= m_curss->m_soundnum) ){
		DbgOut( "SoundBankDlg : OnDelSound listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int itemno;
	itemno = (int)m_sndlist_wnd.SendMessage( LB_GETITEMDATA, listno, 0 );
	if( itemno == LB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = m_curss->DelSound( itemno );
	if( ret ){
		DbgOut( "SoundBankDlg : OnDelSound : sb DelSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_sndcomboindex = 0;
	m_sndlistindex = -1;

	ret = ParamsToDlg( UPD_SOUND );
	_ASSERT( !ret );

	return 0;
}
LRESULT CSoundBankDlg::OnUpSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_curss ){
		return 0;
	}
	if( m_curss->m_soundnum <= 0 ){
		return 0;
	}

	int listno;
	listno = (int)m_sndlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= m_curss->m_soundnum) ){
		DbgOut( "SoundBankDlg : OnUpSound listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int itemno;
	itemno = (int)m_sndlist_wnd.SendMessage( LB_GETITEMDATA, listno, 0 );
	if( itemno == LB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = m_curss->UpSound( itemno );
	if( ret ){
		DbgOut( "SoundBankDlg : OnUpSound : ss UpSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_sndcomboindex = 0;
	if( listno > 0 ){
		m_sndlistindex = listno - 1;
	}

	ret = ParamsToDlg( UPD_SOUND );
	_ASSERT( !ret );

	return 0;
}
LRESULT CSoundBankDlg::OnDownSound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !m_curss ){
		return 0;
	}
	if( m_curss->m_soundnum <= 0 ){
		return 0;
	}

	int listno;
	listno = (int)m_sndlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
	if( listno == LB_ERR )
		return 0;

	if( (listno < 0) || (listno >= m_curss->m_soundnum) ){
		DbgOut( "SoundBankDlg : OnDownSound listno error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	int itemno;
	itemno = (int)m_sndlist_wnd.SendMessage( LB_GETITEMDATA, listno, 0 );
	if( itemno == LB_ERR ){
		_ASSERT( 0 );
		return 1;
	}

	int ret;
	ret = m_curss->DownSound( itemno );
	if( ret ){
		DbgOut( "SoundBankDlg : OnDownSound : ss DownSound error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	m_sndcomboindex = 0;
	if( listno < (m_curss->m_soundnum - 1) ){
		m_sndlistindex = listno + 1;
	}

	ret = ParamsToDlg( UPD_SOUND );
	_ASSERT( !ret );

	return 0;
}
LRESULT CSoundBankDlg::OnSelChangeSoundList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	if( !m_curss ){
		m_sndcomboindex = 0;
		m_sndlistindex = -1;
	}else if( m_curss->m_soundnum <= 0 ){
		m_sndcomboindex = 0;
		m_sndlistindex = -1;
	}else{
		int listno;
		listno = (int)m_sndlist_wnd.SendMessage( LB_GETCURSEL, 0, 0 );
		if( (listno == LB_ERR) || (listno < 0) || (listno >= m_curss->m_soundnum) ){
			m_sndcomboindex = 0;
			m_sndlistindex = -1;
		}else{
			m_sndlistindex = listno;

			int sndseri;
			sndseri = (int)::SendMessage( m_sndlist_wnd, LB_GETITEMDATA, m_sndlistindex, 0 );
			if( sndseri == LB_ERR ){
				m_sndcomboindex = 0;
			}else{
				CSoundElem* curse;
				curse = m_curss->GetSoundElem( sndseri );
				if( curse ){
					m_sndcomboindex = GetComboIndexByName( curse->m_filename );
				}else{
					m_sndcomboindex = 0;
				}
			}
		}
	}

	ParamsToDlg( UPD_SOUND );

	return 0;
}

int CSoundBankDlg::GetComboIndexByName( char* srcname )
{
	if( !m_sndcombo || (m_sndcombonum <= 1) ){
		return 0;
	}

	int findno = 0;
	int sno;
	for( sno = 1; sno < m_sndcombonum; sno++ ){
		int cmp0;
		cmp0 = strcmp( (m_sndcombo + sno)->name, srcname );
		if( cmp0 == 0 ){
			findno = (m_sndcombo + sno)->sndindex;
			break;
		}
	}
	return findno;
}


