// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

#pragma once

#ifndef STRICT
#define STRICT
#endif

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B
#ifndef WINVER				// Windows 95 ����� Windows NT 4 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
//#define WINVER 0x0400		// ����� Windows 98 ����� Windows 2000 �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
//#define WINVER 0x0501
#define WINVER 0x0A00
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
//#define _WIN32_WINNT 0x0400	// ����� Windows 2000 �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
//#define _WIN32_WINNT 0x0501
#define _WIN32_WINNT 0x0A00
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
//#define _WIN32_WINDOWS 0x0410 // ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
//#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINDOWS 0x0A00
#endif

#ifndef _WIN32_IE			// IE 4.0 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
//#define _WIN32_IE 0x0400	// ����� IE 5.0  �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
//#define _WIN32_IE 0x0600
#define _WIN32_IE 0x0A00
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

// ��ʓI�Ŗ������Ă����S�� MFC �̌x�����b�Z�[�W�̈ꕔ�̔�\�����������܂��B
#define _ATL_ALL_WARNINGS

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#endif


//MFC
//#ifdef NEEDMFCH
//#include <afxwin.h>
//#include <afxdisp.h>
//#endif

//#ifdef NEEDAFXDLGSH
//#include <Afxdlgs.h>
//#endif


//#include "resource.h"
//#include <atlbase.h>
//#include <atlcom.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

#include <stdio.h>
#include <windows.h>

using namespace ATL;


#ifndef GWLP_WNDPROC
#define GWLP_WNDPROC        (-4)
#endif
#ifndef GWLP_HINSTANCE
#define GWLP_HINSTANCE      (-6)
#endif
#ifndef GWLP_HWNDPARENT
#define GWLP_HWNDPARENT     (-8)
#endif
#ifndef GWLP_USERDATA
#define GWLP_USERDATA       (-21)
#endif
#ifndef GWLP_ID
#define GWLP_ID             (-12)
#endif
#ifndef GWLP_STYLE
#define GWLP_STYLE           (-16)
#endif
#ifndef GWLP_EXSTYLE
#define GWLP_EXSTYLE         (-20)
#endif
//else for _WIN32
#undef GWL_WNDPROC
#undef GWL_HINSTANCE
#undef GWL_HWNDPARENT
#undef GWL_USERDATA
#undef GWL_ID
#undef GWL_STYLE
#undef GWL_EXSTYLE


//#############################
// RokDeBone2 version 6 header
//#############################
#include "../../RokDeBone2DX/ver6.h"

