#include "includes.h"
#include "globals.h"
#include "utils.h"
#include "hooks.h"
#include "csgo.h"
#include "math.h"
#include <TlHelp32.h>

FILE* file;
void Cleanup(HMODULE hModule)
{
    

    FreeConsole();
    if (file)
        fclose(file);
    FreeLibraryAndExitThread(g_hModule, 0);
    FreeLibraryAndExitThread(hModule, 0);
}

HWND g_hWnd = 0;

BOOL WINAPI MainThread(HMODULE hModule)
{
	I::PrintList();

    g_hWnd = FindWindowA(NULL, "Counter-Strike: Global Offensive - Direct3D 9");
    Sleep(5);


    std::cout << "g_hWnd: " << g_hWnd << "\n";

    std::cout << "m_flFlashDuration: " << DT::DT_CSPlayer::m_flFlashDuration << "\n"; // 66672   0x10470
    std::cout << "m_flFlashMaxAlpha: " << DT::DT_CSPlayer::m_flFlashMaxAlpha << "\n"; // 66668   0x1046C

    std::cout << "rcs s" << DT::DT_CSLocalPlayerExclusive::m_iShotsFired << "\n";
    std::cout << "rcs p" << DT::DT_Local::m_aimPunchAngle << "\n";

    std::cout << "localplyayer: " << I::IEntityList->GetClientEntity(I::IEngineClient->GetLocalPlayer()) << "\n";

    g_Hooks = new Hooks();

    VMTHookManager* test = new VMTHookManager(*(void***)I::CHLClient);

	g_Globals = **(CGlobalVarsBase***)((DWORD)test->GetVFunc(11) + 0xA);
    //oTest = test->Hook<tTest>(reinterpret_cast<PVOID>(&hkTest), 5);

    //oHudMsg = test->Hook<tTest>(reinterpret_cast<PVOID>(&hkHUDMsg), 13);

	VMTHookManager* EngineTools = new VMTHookManager(*(void***)I::IEngineTool);
	//EngineTools->Hook(reinterpret_cast<PVOID>(&hkRenderView), 19);

	VMTHookManager* ClientModeShared = new VMTHookManager(*(void***)I::ClientModeShared);
	oCreateMove = ClientModeShared->Hook<tCreateMove>(reinterpret_cast<PVOID>(&hkCreateMove), 24);

	VMTHookManager* IPanel = new VMTHookManager(*(void***)I::IPanel);
	oPaintTraverse = IPanel->Hook<tPaintTraverse>(reinterpret_cast<PVOID>(&hkPaintTraverse), 41);


    void* pShaderBaseClass = **(void***)(Utils::FindSignature("shaderapidx9.dll", "\xA1\x0\x0\x0\x0\x50\x8B\x08\xFF\x51\x0C", "x????xxxxxx") + 1);
    VMTHookManager* ShaderBaseClass = new VMTHookManager(*(void***)pShaderBaseClass);
    oEndScene = ShaderBaseClass->Hook<tEndScene>(reinterpret_cast<PVOID>(&hkEndScene), 42);

    std::cout << "oEndScene: " << oEndScene << "\n";

    MH_EnableHook(MH_ALL_HOOKS);

    while (!(GetAsyncKeyState(VK_END) & 1)) 
    { 
		Sleep(100);
    }

	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(200);
	MH_RemoveHook(MH_ALL_HOOKS);
	Sleep(200);
	MH_Uninitialize();

    SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG)oWndProc);

    Sleep(200);
    Cleanup(hModule);
    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        g_hModule = hModule;
        AllocConsole();
        freopen_s(&file, "CONOUT$", "w", stdout);

        HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0);
        if (handle)
            CloseHandle(handle);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}