// ItiDlg.h : CItiDlg の宣言

#ifndef __ItiDlg_H_
#define __ItiDlg_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <basedat.h>

/////////////////////////////////////////////////////////////////////////////
// CItiDlg

class CMyD3DApplication;
class CShdHandler;
class CMotHandler;
class CQuaternion;

class CItiDlg : 
	public CAxDialogImpl<CItiDlg>
{
public:
	CItiDlg( CMyD3DApplication* papp );
	~CItiDlg();

	int SetItiMode( int itimode );


	int MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, int* shifvert, int shifvertnum, float pointx, float pointy );
	int MoveObject( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, int* shifvert, int shifvertnum, int kind, float delta );

	int ShowDlg( int cmdshow );

	int ScaleBoneTreePos( CShdHandler* lpsh, float srcscale );

	enum { IDD = IDD_ITIDLG };

BEGIN_MSG_MAP(CItiDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)

	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

	COMMAND_ID_HANDLER(IDC_VERT, OnSelVert)
	COMMAND_ID_HANDLER(IDC_FACE, OnSelFace)
	COMMAND_ID_HANDLER(IDC_PART, OnSelPart)
	COMMAND_ID_HANDLER(IDC_JOINT, OnSelJoint)
	COMMAND_ID_HANDLER(IDC_JOINTROT, OnSelJointRot)

	COMMAND_ID_HANDLER(IDC_MOVE_RELATIVE, OnSelRelative)
	COMMAND_ID_HANDLER(IDC_MOVE_ABSOLUTE, OnSelAbsolute)

	COMMAND_ID_HANDLER(IDC_WHESCALE, OnWheScale)
	COMMAND_ID_HANDLER(IDC_CHILDJOINT, OnChildJoint)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelVert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelFace(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelPart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelJointRot(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelRelative(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelAbsolute(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnWheScale(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChildJoint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


private:
	int SetWnd();
	int ParamsToDlg();
	int DlgToParams();
	int DXVec3Normalize( D3DXVECTOR3* dstvec, D3DXVECTOR3* srcvec );
	int GetShadow( D3DXVECTOR3* dstshadow, D3DXVECTOR3 planedir, D3DXVECTOR3 planep, D3DXVECTOR3 srcp, D3DXVECTOR3 srcdir );


	int MoveVertexScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, float pointx, float pointy );
	int MoveVertexRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, D3DXVECTOR3 relvec );
	int MoveVertexAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, D3DXVECTOR3 absvec );
	int MoveVertexDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int* shifvert, int shifvertnum, int kind, float delta );


	int MoveFaceScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, float pointx, float pointy );
	int MoveFaceRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, D3DXVECTOR3 relvec );
	int MoveFaceAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, D3DXVECTOR3 absvec );
	int MoveFaceDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int shiftface, int kind, float delta );


	int MovePartScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, float pointx, float pointy );
	int MovePartRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 relvec );
	int MovePartAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 absvec );
	int MovePartDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int kind, float delta );


	int MoveJointScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, float pointx, float pointy );
	int MoveJointRelative( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 relvec, int mvchild = 1 );
	int MoveJointAbsolute( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 absvec );
	int MoveJointDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int kind, float delta );


	void MoveJointRelativeReq( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, D3DXVECTOR3 relvec, int broflag );

	int RotateJointScreen( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, float pointx, float pointy );
	int RotateJointDelta( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, int shiftpart, int kind, float delta );
	void RotateJointReq( CMyD3DApplication* papp, CShdHandler* lpsh, CMotHandler* lpmh, 
		int shiftpart, D3DXVECTOR3 befpar, CQuaternion multq, int broflag );


	int CheckObjectButton();

public:
	int m_itimode;//itimodedlgのm_itimode
	int m_movemode;
	int m_object;
	D3DXVECTOR3 m_inputpos;
	D3DXVECTOR3 m_worldpos;

	CWindow m_dlg_wnd;
	CWindow m_posx_wnd;
	CWindow m_posy_wnd;
	CWindow m_posz_wnd;

	CWindow m_vert_wnd;
	CWindow m_face_wnd;
	CWindow m_part_wnd;
	CWindow m_joint_wnd;
	CWindow m_jointrot_wnd;

	CMyD3DApplication* m_papp;
	
	int m_cmdshow;

	int m_wheelscale;
	int m_childjoint;
};

#endif //__ItiDlg_H_
