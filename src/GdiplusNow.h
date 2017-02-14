#pragma once

class GdiplusNow
{
private:
  static ULONG_PTR g_GdiplusToken;
public:
  static void Startup();
  static void Shutdown();
};
