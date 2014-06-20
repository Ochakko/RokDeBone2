#ifndef ATLBASE_PICKH
#define ATLBASE_PICKH

#include <atlbase.h>

template <class T>
class ATL_NO_VTABLE CAtlModule2T : public CAtlModule
{
public :
	CAtlModule2T() throw()
	{
		T::InitLibId();
	}

	static void InitLibId() throw()
	{
	}

	HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL) throw()
	{
		pCLSID;
		bRegTypeLib;

		HRESULT hr = S_OK;

#ifndef _ATL_NO_COM_SUPPORT

		hr = _AtlComModule.RegisterServer(bRegTypeLib, pCLSID);

#endif	// _ATL_NO_COM_SUPPORT


#ifndef _ATL_NO_PERF_SUPPORT

		if (SUCCEEDED(hr) && _pPerfRegFunc != NULL)
			hr = (*_pPerfRegFunc)(_AtlBaseModule.m_hInst);

#endif

		return hr;
	}

	HRESULT UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL) throw()
	{
		bUnRegTypeLib;
		pCLSID;

		HRESULT hr = S_OK;

#ifndef _ATL_NO_PERF_SUPPORT

		if (_pPerfUnRegFunc != NULL)
			hr = (*_pPerfUnRegFunc)();

#endif

#ifndef _ATL_NO_COM_SUPPORT

		if (SUCCEEDED(hr))
			hr = _AtlComModule.UnregisterServer(bUnRegTypeLib, pCLSID);

#endif	// _ATL_NO_COM_SUPPORT

		return hr;

	}

	static HRESULT WINAPI UpdateRegistryAppId(BOOL /*bRegister*/) throw()
	{
		return S_OK;
	}
	HRESULT RegisterAppId() throw()
	{
		return T::UpdateRegistryAppId(TRUE);
	}

	HRESULT UnregisterAppId() throw()
	{
		return T::UpdateRegistryAppId(FALSE);
	}

	virtual HRESULT AddCommonRGSReplacements(IRegistrarBase* pRegistrar) throw()
	{
		return pRegistrar->AddReplacement(L"APPID", T::GetAppId());
	}
	static LPCOLESTR GetAppId() throw()
	{
		return OLESTR("");
	}
};


/////////////////
/////////////////

template <class T>
class ATL_NO_VTABLE CAtlExeModule2T : public CAtlModule2T<T>
{
public :
#ifndef _ATL_NO_COM_SUPPORT

	DWORD m_dwMainThreadID;
	HANDLE m_hEventShutdown;
	DWORD m_dwTimeOut;
	DWORD m_dwPause;
	bool m_bDelayShutdown;
	bool m_bActivity;

#endif // _ATL_NO_COM_SUPPORT

	CAtlExeModule2T() throw()

#ifndef _ATL_NO_COM_SUPPORT

		: m_dwMainThreadID(::GetCurrentThreadId()),
		m_dwTimeOut(5000),
		m_dwPause(1000),
		m_hEventShutdown(NULL),
		m_bDelayShutdown(true)

#endif // _ATL_NO_COM_SUPPORT

	{
		HRESULT hr = T::InitializeCom();
		if (FAILED(hr))
		{
		CAtlBaseModule::m_bInitFailed =	 true;
			return;
		}

#if !defined(_ATL_NO_COM_SUPPORT)

		_AtlComModule.ExecuteObjectMain(true);

#endif	//  !defined(_ATL_NO_COM_SUPPORT)

	}

	~CAtlExeModule2T() throw()
	{
#ifndef _ATL_NO_COM_SUPPORT

		_AtlComModule.ExecuteObjectMain(false);

#endif

		// Call term functions before COM is uninitialized
		Term();

#ifndef _ATL_NO_COM_SUPPORT

		// Clean up AtlComModule before COM is uninitialized
		_AtlComModule.Term();

#endif // _ATL_NO_COM_SUPPORT

		T::UninitializeCom();
	}

	static HRESULT InitializeCom() throw()
	{

#if ((_WIN32_WINNT >= 0x0400 ) || defined(_WIN32_DCOM)) & defined(_ATL_FREE_THREADED)
		return CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
		return CoInitialize(NULL);
#endif
	}

	static void UninitializeCom() throw()
	{
		CoUninitialize();
	}

	LONG Unlock() throw()
	{
		LONG lRet = CComGlobalsThreadModel::Decrement(&m_nLockCnt);

#ifndef _ATL_NO_COM_SUPPORT

		if (lRet == 0)
		{
			if (m_bDelayShutdown)
			{
				m_bActivity = true;
				::SetEvent(m_hEventShutdown); // tell monitor that we transitioned to zero
			}
			else
			{
				::PostThreadMessage(m_dwMainThreadID, WM_QUIT, 0, 0);
			}
		}

#endif	// _ATL_NO_COM_SUPPORT

		return lRet;
	}

	void MonitorShutdown() throw()
	{
		while (1)
		{
			::WaitForSingleObject(m_hEventShutdown, INFINITE);
			DWORD dwWait = 0;
			do
			{
				m_bActivity = false;
				dwWait = ::WaitForSingleObject(m_hEventShutdown, m_dwTimeOut);
			} while (dwWait == WAIT_OBJECT_0);
			// timed out
			if (!m_bActivity && m_nLockCnt == 0) // if no activity let's really bail
			{
#if ((_WIN32_WINNT >= 0x0400 ) || defined(_WIN32_DCOM)) & defined(_ATL_FREE_THREADED)
				::CoSuspendClassObjects();
				if (m_nLockCnt == 0)
#endif
					break;
			}
		}
		::CloseHandle(m_hEventShutdown);
		::PostThreadMessage(m_dwMainThreadID, WM_QUIT, 0, 0);
	}

	HANDLE StartMonitor() throw()
	{
		m_hEventShutdown = ::CreateEvent(NULL, false, false, NULL);
		if (m_hEventShutdown == NULL)
			return false;
		DWORD dwThreadID;
		HANDLE h = ::CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
		return h;
	}

	static DWORD WINAPI MonitorProc(void* pv) throw()
	{
		CAtlExeModule2T<T>* p = static_cast<CAtlExeModule2T<T>*>(pv);
		p->MonitorShutdown();
		return 0;
	}

	int WinMain(int nShowCmd) throw()
	{
		if (CAtlBaseModule::m_bInitFailed)
		{
			ATLASSERT(0);
			return -1;
		}
		T* pT = static_cast<T*>(this);
		HRESULT hr = S_OK;

		LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
		if (pT->ParseCommandLine(lpCmdLine, &hr) == true)
			hr = pT->Run(nShowCmd);

#ifdef _DEBUG
		// Prevent false memory leak reporting. ~CAtlWinModule may be too late.
		_AtlWinModule.Term();		
#endif	// _DEBUG
		return hr;
	}

	// Scan command line and perform registration
	// Return value specifies if server should run

	// Parses the command line and registers/unregisters the rgs file if necessary
	bool ParseCommandLine(LPCTSTR lpCmdLine, HRESULT* pnRetCode) throw()
	{
		*pnRetCode = S_OK;

		TCHAR szTokens[] = _T("-/");

		T* pT = static_cast<T*>(this);
		LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
		while (lpszToken != NULL)
		{
			if (WordCmpI(lpszToken, _T("UnregServer"))==0)
			{
				*pnRetCode = pT->UnregisterServer(TRUE);
				if (SUCCEEDED(*pnRetCode))
					*pnRetCode = pT->UnregisterAppId();
				return false;
			}

			// Register as Local Server
			if (WordCmpI(lpszToken, _T("RegServer"))==0)
			{
				*pnRetCode = pT->RegisterAppId();
				if (SUCCEEDED(*pnRetCode))
					*pnRetCode = pT->RegisterServer(TRUE);
				return false;
			}

			lpszToken = FindOneOf(lpszToken, szTokens);
		}

		return true;
	}

	HRESULT PreMessageLoop(int /*nShowCmd*/) throw()
	{
		HRESULT hr = S_OK;
		T* pT = static_cast<T*>(this);
		pT;

#ifndef _ATL_NO_COM_SUPPORT

#if ((_WIN32_WINNT >= 0x0400 ) || defined(_WIN32_DCOM)) & defined(_ATL_FREE_THREADED)

		hr = pT->RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
			REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);

		if (FAILED(hr))
			return hr;

		if (hr == S_OK)
		{
			if (m_bDelayShutdown)
			{
				CHandle h(pT->StartMonitor());
				if (h.m_h == NULL)
				{
					hr = E_FAIL;
				}
				else
				{
					hr = CoResumeClassObjects();
					ATLASSERT(SUCCEEDED(hr));
					if (FAILED(hr))
					{
						::SetEvent(m_hEventShutdown); // tell monitor to shutdown
						::WaitForSingleObject(h, m_dwTimeOut * 2);
					}
				}
			}

			if (FAILED(hr))
				pT->RevokeClassObjects();
		}
		else
		{
			m_bDelayShutdown = false;
		}

#else

		hr = pT->RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
			REGCLS_MULTIPLEUSE);
		if (hr == S_OK)
		{
			if (m_bDelayShutdown && !pT->StartMonitor())
			{
				hr = E_FAIL;
			}
		}
		else
		{
			m_bDelayShutdown = false;
		}


#endif

#endif	// _ATL_NO_COM_SUPPORT

		ATLASSERT(SUCCEEDED(hr));
		return hr;
	}

	HRESULT PostMessageLoop() throw()
	{
		HRESULT hr = S_OK;

#ifndef _ATL_NO_COM_SUPPORT

		T* pT = static_cast<T*>(this);
		hr = pT->RevokeClassObjects();
		if (m_bDelayShutdown)
			Sleep(m_dwPause); //wait for any threads to finish

#endif	// _ATL_NO_COM_SUPPORT

		return hr;
	}

	void RunMessageLoop() throw()
	{
		MSG msg;
		while (GetMessage(&msg, 0, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	HRESULT Run(int nShowCmd = SW_HIDE) throw()
	{
		HRESULT hr = S_OK;

		T* pT = static_cast<T*>(this);
		hr = pT->PreMessageLoop(nShowCmd);

		// Call RunMessageLoop only if PreMessageLoop returns S_OK.
		if (hr == S_OK)
		{
			pT->RunMessageLoop();
		}

		// Call PostMessageLoop if PreMessageLoop returns success.
		if (SUCCEEDED(hr))
		{
			hr = pT->PostMessageLoop();
		}

		ATLASSERT(SUCCEEDED(hr));
		return hr;
	}

	// Register/Revoke All Class Factories with the OS (EXE only)
	HRESULT RegisterClassObjects(DWORD dwClsContext, DWORD dwFlags) throw()
	{
		return AtlComModuleRegisterClassObjects(&_AtlComModule, dwClsContext, dwFlags);
	}
	HRESULT RevokeClassObjects() throw()
	{
		return AtlComModuleRevokeClassObjects(&_AtlComModule);
	}
};

#endif
