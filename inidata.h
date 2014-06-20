#ifndef INIDATAH
#define INIDATAH


#include <coef.h>

enum {
	GETTYPE_INT,
	GETTYPE_float,
	GETTYPE_NAME,
	GETTYPE_MAX
};

class CIniData
{
public:
	CIniData();
	~CIniData();

	int LoadParams( char* filename );
	int SaveParams( char* filename );

private:
	int InitParams();
	int InitDefaultValue();
	int InitLoadFlag();
	int SetDefaultValue( int flag );//flag == 0のときは、読み込みフラグ==0のものだけセット、flag == 1の時は、全てセット。

	int DestroyObjs();

	int SetBuffer( char* filename );
	int CheckFileHeader();

	int GetIntParam( char* strtype, int* dstint, int minint, int maxint, int* dstflag );
	int GetFloatParam( char* strtype, float* dstfl, float minfl, float maxfl, int* dstflag );
	int GetNameParam( char* strtype, char* dstchar, int maxleng, int* dstflag );
	int GetColorParam( char* strtype, COLORREF* dstcol, int* dstflag );
	int GetViewParam( char* strtype, VIEWPARAM* dstvp, int* dstflag );

	char* GetDigit( char* srcstr, int* dstint, int* dstsetflag );
	char* GetFloat( char* srcstr, float* dstfloat, int* dstsetflag );

	int CIniData::Write2File( char* lpFormat, ... );


private:

	//file操作用
	HANDLE m_hfile;
	char* m_buf;
	DWORD m_pos;
	DWORD m_bufleng;


	//default値を定義
	int d_index32bit;

	//メインウインドウ
	int d_main_posx;
	int d_main_posy;
	int d_main_width;
	int d_main_height;

	//モーションダイアログ
	int d_motdlg_disp;
	int d_motdlg_posx;
	int d_motdlg_posy;
	int d_motdlg_calcsp;
	int d_motdlg_height;

	//階層構造エディタ
	int d_sigdlg_disp;
	int d_sigdlg_posx;
	int d_sigdlg_posy;
	
	//カメラダイアログ
	int d_camdlg_disp;
	int d_camdlg_posx;
	int d_camdlg_posy;
	float d_camdlg_move;

	//gpa
	float d_gpa_move;


	//GraphWnd
	int d_graph_disp;

	//kinect
	int d_usekinect;

	//背景
	COLORREF dd_bg_color;
	char d_bg_tex1[_MAX_PATH];
	char d_bg_tex2[_MAX_PATH];
	float d_bg_mvu;
	float d_bg_mvv;
	int d_bg_fixsize;

	//平行光源
	COLORREF d_dirlight_col;

	//オプション
	int d_opt_jointdisp;
	int d_opt_polydisp;
	int d_opt_gdisp;
	int d_opt_namequa;

		//2004/12/31
	int d_opt_jointsize;
	int d_opt_jointalpha;
	int d_opt_undodlg;
	int d_opt_rdlg;
	int d_opt_infdlg;
	int d_opt_modlg;

	int d_opt_bmdisp;

	int d_opt_previewfps;
	int d_opt_motstep;
	int d_opt_ikfk;

	int d_opt_iktdisp;

	int d_opt_MouseL;
	int d_opt_MouseR;
	int d_opt_MouseW;

	COLORREF d_opt_unselline;
	COLORREF d_opt_selline;
	COLORREF d_opt_unselpoint;
	COLORREF d_opt_selpoint;
	int	d_opt_pointsize;

	//customcolor
	COLORREF d_cc[16];

	//boneline
	COLORREF d_selectcol;
	COLORREF d_unselectcol;

	//viewparam
	VIEWPARAM d_viewparam[4];

	int d_iklevelope;
	int d_forbidDD;

	int d_mqo_bonetype;
	float d_mqo_mult;

	float d_toonedgecol;
	float d_toonedgew;
/////////////

	//fileから読み込んだかどうかのフラグ
	int f_index32bit;
	//メインウインドウ
	int f_main_posx;
	int f_main_posy;
	int f_main_width;
	int f_main_height;

	//モーションダイアログ
	int f_motdlg_disp;
	int f_motdlg_posx;
	int f_motdlg_posy;
	int f_motdlg_calcsp;
	int f_motdlg_height;

	//階層構造エディタ
	int f_sigdlg_disp;
	int f_sigdlg_posx;
	int f_sigdlg_posy;
	
	//カメラダイアログ
	int f_camdlg_disp;
	int f_camdlg_posx;
	int f_camdlg_posy;
	int f_camdlg_move;

	//gpa
	int f_gpa_move;

	//graphwnd
	int f_graph_disp;

	int f_usekinect;

	//背景
	int f_bg_color;
	int f_bg_tex1;
	int f_bg_tex2;
	int f_bg_mvu;
	int f_bg_mvv;
	int f_bg_fixsize;

	//平行光源
	int f_dirlight_col;

	//オプション
	int f_opt_jointdisp;
	int f_opt_polydisp;
	int f_opt_gdisp;
	int f_opt_namequa;

		//2004/12/31
	int f_opt_jointsize;
	int f_opt_jointalpha;
	int f_opt_undodlg;
	int f_opt_rdlg;
	int f_opt_infdlg;
	int f_opt_modlg;

	int f_opt_bmdisp;

	int f_opt_previewfps;
	int f_opt_motstep;
	int f_opt_ikfk;
	int f_opt_iktdisp;

	int f_opt_MouseL;
	int f_opt_MouseR;
	int f_opt_MouseW;

	int f_opt_unselline;
	int f_opt_selline;
	int f_opt_unselpoint;
	int f_opt_selpoint;
	int f_opt_pointsize;


	//customcolor
	int f_cc[16];

	//boneline
	int f_selectcol;
	int f_unselectcol;

	//viewparam
	int f_viewparam[4];

	int f_iklevelope;
	int f_forbidDD;

	int f_mqo_bonetype;
	int f_mqo_mult;

	int f_toonedgecol;
	int f_toonedgew;

public:
	int index32bit;
	//メインウインドウ
	int main_posx;
	int main_posy;
	int main_width;
	int main_height;

	//モーションダイアログ
	int motdlg_disp;
	int motdlg_posx;
	int motdlg_posy;
	int motdlg_calcsp;
	int motdlg_height;

	//階層構造エディタ
	int sigdlg_disp;
	int sigdlg_posx;
	int sigdlg_posy;
	
	//カメラダイアログ
	int camdlg_disp;
	int camdlg_posx;
	int camdlg_posy;
	float camdlg_move;

	//gpa
	float gpa_move;

	//graphwnd
	int graph_disp;

	int usekinect;

	//背景
	COLORREF bg_color;
	char bg_tex1[_MAX_PATH];
	char bg_tex2[_MAX_PATH];
	float bg_mvu;
	float bg_mvv;
	int bg_fixsize;

	//平行光源
	COLORREF dirlight_col;

	//オプション
	int opt_jointdisp;
	int opt_polydisp;
	int opt_gdisp;
	int opt_namequa;

		//2004/12/31
	int opt_jointsize;
	int opt_jointalpha;
	int opt_undodlg;
	int opt_rdlg;
	int opt_infdlg;
	int opt_modlg;

	int opt_bmdisp;

	int opt_previewfps;
	int opt_motstep;
	int opt_ikfk;
	int opt_iktdisp;

	int opt_MouseL;
	int opt_MouseR;
	int opt_MouseW;

	COLORREF opt_unselline;
	COLORREF opt_selline;
	COLORREF opt_unselpoint;
	COLORREF opt_selpoint;
	int	opt_pointsize;

	//customcolor
	COLORREF cc[16];


	//boneline
	COLORREF selectcol;
	COLORREF unselectcol;

	//viewparam
	VIEWPARAM viewparam[4];

	int iklevelope;
	int forbidDD;

	int mqo_bonetype;
	float mqo_mult;

	float toonedgecol;
	float toonedgew;
	
};

#endif