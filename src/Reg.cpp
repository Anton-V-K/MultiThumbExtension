#define INITGUID
#include "Common.h"

#include <strsafe.h>

LONG g_cRef = 0;

struct REGKEY_DELETEKEY
{
  HKEY hKey;
  LPCWSTR lpszSubKey;
};

struct REGKEY_SUBKEY_AND_VALUE
{
  HKEY      hKey;
  LPCWSTR   lpszSubKey;
  LPCWSTR   lpszValue;
  DWORD     dwType;
  DWORD_PTR dwData;
};

STDAPI_(ULONG) DllAddRef()
{
   LONG cRef = InterlockedIncrement(&g_cRef);
   return cRef;
}

STDAPI_(ULONG) DllRelease()
{
   LONG cRef = InterlockedDecrement(&g_cRef);
   if (0 > cRef)
      cRef = 0;
   return cRef;
}

STDAPI DllCanUnloadNow()
{
   return g_cRef ? S_FALSE : S_OK;
}

STDAPI CreateRegistryKey(const REGKEY_SUBKEY_AND_VALUE* pKey)
{
  size_t cbData;
  LPVOID pvData = NULL;
  HRESULT hr = S_OK;

  switch(pKey->dwType)
  {
  case REG_DWORD:
    pvData = (LPVOID)(LPDWORD)&pKey->dwData;
    cbData = sizeof(DWORD);
    break;

  case REG_SZ:
  case REG_EXPAND_SZ:
    hr = StringCbLength((LPCWSTR)pKey->dwData, STRSAFE_MAX_CCH, &cbData);
    if (SUCCEEDED(hr))
    {
      pvData = (LPVOID)(LPCWSTR)pKey->dwData;
      cbData += sizeof(WCHAR);
    }
    break;

  default:
    hr = E_INVALIDARG;
  }

  if (SUCCEEDED(hr))
  {
    LSTATUS status = SHSetValue(pKey->hKey, pKey->lpszSubKey, pKey->lpszValue, pKey->dwType, pvData, (DWORD)cbData);
    if (NOERROR != status)
    {
      hr = HRESULT_FROM_WIN32(status);
    }
  }

  return hr;
}

STDAPI CreateRegistryKeys(const REGKEY_SUBKEY_AND_VALUE* aKeys, ULONG cKeys)
{
  HRESULT hr = S_OK;

  for (ULONG iKey = 0; iKey < cKeys; iKey++)
  {
    HRESULT hrTemp = CreateRegistryKey(&aKeys[iKey]);
    if (FAILED(hrTemp))
    {
      hr = hrTemp;
    }
  }
  return hr;
}

STDAPI DeleteRegistryKeys(const REGKEY_DELETEKEY* aKeys, ULONG cKeys)
{
  HRESULT hr = S_OK;
  LSTATUS status;

  for (ULONG iKey = 0; iKey < cKeys; iKey++)
  {
    status = RegDeleteTree(aKeys[iKey].hKey, aKeys[iKey].lpszSubKey);
    if (NOERROR != status)
    {
      hr = HRESULT_FROM_WIN32(status);
    }
  }
  return hr;
}

STDAPI DllRegisterServer()
{
  WCHAR szModule[MAX_PATH];
  ZeroMemory(szModule, sizeof(szModule));
  GetModuleFileName(DllInstance(), szModule, ARRAYSIZE(szModule));

  const REGKEY_SUBKEY_AND_VALUE keys[] = {
    {HKEY_CLASSES_ROOT, L"CLSID\\" szCLSID_MultiThumbnailProvider, NULL, REG_SZ, (DWORD_PTR)L"Sample Thumbnail Provider"},
    {HKEY_CLASSES_ROOT, L"CLSID\\" szCLSID_MultiThumbnailProvider L"\\InprocServer32", NULL, REG_SZ, (DWORD_PTR)szModule},
    {HKEY_CLASSES_ROOT, L"CLSID\\" szCLSID_MultiThumbnailProvider L"\\InprocServer32", L"ThreadingModel", REG_SZ, (DWORD_PTR)L"Apartment"},
    {HKEY_CLASSES_ROOT, L".STL\\shellex\\{E357FCCD-A995-4576-B01F-234630154E96}", NULL, REG_SZ, (DWORD_PTR)szCLSID_MultiThumbnailProvider}
  };
  return CreateRegistryKeys(keys, ARRAYSIZE(keys));
}

STDAPI DllUnregisterServer()
{
  const REGKEY_DELETEKEY keys[] = {{HKEY_CLASSES_ROOT, L"CLSID\\" szCLSID_MultiThumbnailProvider}};
  return DeleteRegistryKeys(keys, ARRAYSIZE(keys));
}
