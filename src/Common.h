#ifndef MTE_Common_h_
#define MTE_Common_h_

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN  // We always want minimal includes
#endif

#include <windows.h>
//#include <rpcndr.h> // MIDL_INTERFACE is used in GdiplusImaging.h

#include <gdiplus.h>
//#include <shlobj.h>
#include <shlwapi.h>
#include <tchar.h>
#include <thumbcache.h>

STDAPI_(ULONG) DllAddRef();
STDAPI_(ULONG) DllRelease();
STDAPI_(HINSTANCE) DllInstance();

// {09E44A39-7A8F-450C-8796-826F19E21720}
#define szCLSID_MultiThumbnailProvider L"{09E44A39-7A8F-450C-8796-826F19E21720}"
DEFINE_GUID(CLSID_MultiThumbnailProvider,
0x09E44A39, 0x7A8F, 0x450C, 0x87, 0x96, 0x82, 0x6F, 0x19, 0xE2, 0x17, 0x20);

#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL // we'll use statically linked library
#include <glog/logging.h>

extern ULONG_PTR g_GdiPlusToken;

#endif
