#include "LoadEnvDTE.h"
#include "Mystic/Logging/Log.h"

//NOTE: /MP is disabled for this file via premake5.lua

// from https://msdn.microsoft.com/en-us/library/yf86a8ts.aspx
#pragma warning( disable : 4278 )
#pragma warning( disable : 4146 )
#import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" version("8.0") lcid("0") raw_interfaces_only named_guids	//The following #import imports EnvDTE based on its LIBID.
#pragma warning( default : 4146 )
#pragma warning( default : 4278 )

namespace Mystic::VSEnv
{
	EnvDTE::Process* FindVSProcess(DWORD TargetPID)
	{
		HRESULT hr;

		static const wchar_t* ProgID = L"VisualStudio.DTE";

		CLSID Clsid;
		CLSIDFromProgID(ProgID, &Clsid);

		IUnknown* Unknown;
		hr = GetActiveObject(Clsid, 0, &Unknown);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::_DTE* Interface;

		hr = Unknown->QueryInterface(&Interface);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::Debugger* Debugger;
		hr = Interface->get_Debugger(&Debugger);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::Processes* Processes;
		hr = Debugger->get_LocalProcesses(&Processes);
		if (FAILED(hr))
			return nullptr;

		long Count = 0;
		hr = Processes->get_Count(&Count);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::Process* Result = nullptr;
		for (int i = 1; i < Count; ++i)	// index 0 is invalid, hr == DISP_E_BADINDEX
		{
			EnvDTE::Process* Process;

			// get the process, but sometimes may fail,
			// so we re-try a number of times to get it...
			const int	retryTime = 5000;	// ms
			const int	retryInterval = 10;	// ms
			const int	retryCntMax = retryTime / retryInterval;
			int			retryCnt = 0;
			do
			{
				hr = Processes->Item(variant_t(i), &Process);
				if (FAILED(hr))	// usually return RPC_E_CALL_REJECTED if failed
				{				// so wait a bit to let it get ready
					Sleep(retryInterval);
					++retryCnt;
				}
				else
					retryCnt = retryCntMax;
			} while (retryCnt < retryCntMax);

			if (FAILED(hr))
				continue;

			long ProcessID;
			hr = Process->get_ProcessID(&ProcessID);

			if (SUCCEEDED(hr) && ProcessID == TargetPID)
			{
				Result = Process;
				break;
			}
		}

		return Result;
	}

	void Init()
	{
		HRESULT hr = CoInitialize(0);
		if (FAILED(hr))
		{
			Log::MError("Could not CoInitialize COM");
		}
	}

	void AttachVS()
	{
		DWORD			TargetPID = GetCurrentProcessId();
		EnvDTE::Process* Process = FindVSProcess(TargetPID);
		if (Process)
		{
			Log::Info("Attached to Visual Studio");
			Process->Attach();
		}
		else
		{
			Log::Warning("Could not attach to Visual Studio");
		}
	}

	void DetachVS(bool waitForBreakOrEnd)
	{
		DWORD			TargetPID = GetCurrentProcessId();
		EnvDTE::Process* Process = FindVSProcess(TargetPID);
		if (Process)
		{
			Log::Info("Detached from Visual Studio");
			Process->Detach(variant_t(waitForBreakOrEnd));
		}
		else
		{
			Log::Warning("Could not detach from Visual Studio");
		}
	}

	void DoThing()
	{
		FindVSProcess(0);
	}
}
