/*++
Copyright (c) Microsoft Corporation

Module Name:
- VtConsole.hpp

Abstract:
- This serves as an abstraction to allow for a test connection to a conhost.exe running
  in VT server mode. It's abstracted to allow multiple simultaneous connections to multiple
  conhost.exe servers.

Author(s):
- Mike Griese (MiGrie) 2017
--*/


#include <windows.h>
#include <wil\result.h>
#include <wil\resource.h>

#include <string>


typedef void(*PipeReadCallback)(BYTE* buffer, DWORD dwRead);

class VtConsole
{
public:
    VtConsole(PipeReadCallback const pfnReadCallback);
    void spawn();
    void spawn(const std::wstring& command);
    
    HANDLE inPipe();
    HANDLE outPipe();

    static const DWORD sInPipeOpenMode = PIPE_ACCESS_DUPLEX;
    static const DWORD sOutPipeOpenMode = PIPE_ACCESS_INBOUND;

    static const DWORD sInPipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;
    static const DWORD sOutPipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;

    DWORD getReadOffset();
    void incrementReadOffset(DWORD offset);

    void activate();
    void deactivate();
    
    static DWORD StaticOutputThreadProc(LPVOID lpParameter);

    bool WriteInput(std::string& seq);

private:
    PROCESS_INFORMATION pi;

    HANDLE _outPipe;
    HANDLE _inPipe;
    std::wstring _inPipeName;
    std::wstring _outPipeName;
    
    HANDLE _outPipeConhostSide;
    HANDLE _inPipeConhostSide;
    
    bool _connected = false;
    DWORD _offset = 0;
    bool _active = false;

    PipeReadCallback _pfnReadCallback;

    DWORD _dwOutputThreadId;
    HANDLE _hOutputThread = INVALID_HANDLE_VALUE;

    void _openConsole1();
    void _openConsole2(const std::wstring& command);
    void _openConsole3(const std::wstring& command);

    void _spawn1();
    void _spawn2(const std::wstring& command);
    void _spawn3(const std::wstring& command);

    DWORD _OutputThread();

};