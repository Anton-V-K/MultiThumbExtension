#pragma once

#include "Common.h"

#include "GdiplusNow.h"

ULONG_PTR GdiplusNow::g_GdiplusToken = 0;

void GdiplusNow::Startup()
{
  if (g_GdiplusToken == 0)
  {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    const Gdiplus::Status res = Gdiplus::GdiplusStartup(&g_GdiplusToken, &gdiplusStartupInput, NULL);
    if (res == Gdiplus::Ok)
      LOG(INFO) << "GdiplusStartup(): Ok";
    else
      LOG(ERROR) << "GdiplusStartup() failed with code " << res;
  }
};

void GdiplusNow::Shutdown()
{

}