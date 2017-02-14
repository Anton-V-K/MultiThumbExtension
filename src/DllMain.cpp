#include "Common.h"

#include "GdiplusNow.h"

//#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "libglog_static.lib") // put it into /lib/$(Platform)/$(Configuration) (f.i. /lib/x64/Debug)

HINSTANCE g_hinstDll = NULL;

BOOL APIENTRY DllMain(HINSTANCE hinstDll
                     , DWORD dwReason
                     , LPVOID //pvReserved
                     )
{
  switch (dwReason)
  {
  case DLL_PROCESS_ATTACH:
    {
      g_hinstDll = hinstDll;
      CHAR szModule[MAX_PATH];
      ZeroMemory(szModule, sizeof(szModule));
      GetModuleFileNameA(DllInstance(), szModule, ARRAYSIZE(szModule));
      // By default logs are written to %TEMP%, so use FLAGS_log_dir to redirect them to another directory
      google::InitGoogleLogging(szModule);
      LOG(INFO) << "DLL_PROCESS_ATTACH " << hinstDll;
    }
    break;
  case DLL_PROCESS_DETACH:
    {
      LOG(INFO) << "DLL_PROCESS_DETACH " << hinstDll;
      GdiplusNow::Shutdown();
    }
    break;
  }
  return TRUE;
}

STDAPI_(HINSTANCE) DllInstance()
{
   return g_hinstDll;
}
