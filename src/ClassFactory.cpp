#define INITGUID
#include "Common.h"
#include "ClassFactory.h"
#include "ThumbnailProvider.h"

CClassFactory::CClassFactory()
{
  m_cRef = 1;
  DllAddRef();
}

CClassFactory::~CClassFactory()
{
  DllRelease();
}

STDMETHODIMP CClassFactory::QueryInterface(REFIID riid,
                                           void** ppvObject)
{
  static const QITAB qit[] = 
  {
    QITABENT(CClassFactory, IClassFactory),
    {0},
  };
  return QISearch(this, qit, riid, ppvObject);
}


STDMETHODIMP_(ULONG) CClassFactory::AddRef()
{
  const LONG cRef = InterlockedIncrement(&m_cRef);
  return (ULONG)cRef;
}


STDMETHODIMP_(ULONG) CClassFactory::Release()
{
  const LONG cRef = InterlockedDecrement(&m_cRef);
  if (0 == cRef)
    delete this;
  return (ULONG)cRef;
}


STDMETHODIMP CClassFactory::CreateInstance(IUnknown* punkOuter,
                                           REFIID riid,
                                           void** ppvObject)
{
  if (NULL != punkOuter)
    return CLASS_E_NOAGGREGATION;

  return CThumbnailProvider::CreateInstance(riid, ppvObject);
}


STDMETHODIMP CClassFactory::LockServer(BOOL fLock)
{
  return E_NOTIMPL;
}


STDAPI DllGetClassObject(REFCLSID rclsid,
                         REFIID riid,
                         void **ppv)
{
  if (NULL == ppv)
    return E_INVALIDARG;

  if (!IsEqualCLSID(CLSID_MultiThumbnailProvider, rclsid))
    return CLASS_E_CLASSNOTAVAILABLE;

  CClassFactory* const pcf = new CClassFactory();
  if (NULL == pcf)
    return E_OUTOFMEMORY;

  HRESULT hr = pcf->QueryInterface(riid, ppv);
  pcf->Release();
  return hr;
}
