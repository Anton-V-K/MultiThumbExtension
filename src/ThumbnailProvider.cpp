#include "Common.h"

#include "ThumbnailProvider.h"

#include "GdiplusNow.h"

CThumbnailProvider::CThumbnailProvider()
{
  DllAddRef();
  m_cRef = 1;
  m_pSite = NULL;
  m_StreamSize.QuadPart = 0;
  m_pStreamData = NULL;
}

CThumbnailProvider::~CThumbnailProvider()
{
  if (m_pStreamData)
  {
    delete [] m_pStreamData;
  }
  if (m_pSite)
  {
    m_pSite->Release();
    m_pSite = NULL;
  }
  DllRelease();
}

HRESULT CThumbnailProvider::CreateInstance(REFIID riid, void** ppvObject)
{
  *ppvObject = NULL;

  CThumbnailProvider* const ptp = new CThumbnailProvider();
  if (!ptp)
  {
    return E_OUTOFMEMORY;
  }

  HRESULT hr = ptp->QueryInterface(riid, ppvObject);
  ptp->Release();
  return hr;
}


STDMETHODIMP
  CThumbnailProvider::QueryInterface(REFIID riid, void** ppvObject)
{
  static const QITAB qit[] = 
  {
    QITABENT(CThumbnailProvider, IInitializeWithStream),
    QITABENT(CThumbnailProvider, IThumbnailProvider),
    QITABENT(CThumbnailProvider, IObjectWithSite),
    {0},
  };
  return QISearch(this, qit, riid, ppvObject);
}


STDMETHODIMP_(ULONG) CThumbnailProvider::AddRef()
{
  LONG cRef = InterlockedIncrement(&m_cRef);
  return (ULONG)cRef;
}


STDMETHODIMP_(ULONG) CThumbnailProvider::Release()
{
  LONG cRef = InterlockedDecrement(&m_cRef);
  if (0 == cRef)
    delete this;
  return (ULONG)cRef;
}


STDMETHODIMP CThumbnailProvider::Initialize(IStream *pstm, DWORD grfMode)
{
  STATSTG stat;
  if (pstm->Stat(&stat, STATFLAG_DEFAULT) != S_OK)
  {
    return S_FALSE;
  }

  m_StreamSize = stat.cbSize;
  //LOG(INFO) << "Stream size " << stat.cbSize.QuadPart;
  if (m_pStreamData)
  {
    delete [] m_pStreamData;
    m_pStreamData = NULL;
  }
  if (m_StreamSize.QuadPart)
  {
    m_pStreamData = new BYTE[m_StreamSize.QuadPart];
    ULONG len;
    const HRESULT hr = pstm->Read(const_cast<BYTE*>(m_pStreamData), stat.cbSize.QuadPart, &len);
    if (hr != S_OK)
    {
      LOG(ERROR) << "Reading stream of size " << stat.cbSize.QuadPart << " failed with error " << hr;
      return S_FALSE;
    }
    LOG(INFO) << "Read " << len << " bytes from stream of size " << stat.cbSize.QuadPart;
  }

  // TODO Implement

  return S_OK;
}


STDMETHODIMP
CThumbnailProvider::GetThumbnail(UINT cx, HBITMAP *phbmp, WTS_ALPHATYPE *pdwAlpha)
{
  using namespace Gdiplus;
  GdiplusNow::Startup();

  LOG(INFO) << "cx = " << cx;
  *pdwAlpha = WTSAT_ARGB;
  *phbmp = NULL;

  // TESTING BEGIN
  Bitmap bmp(cx, cx, PixelFormat32bppARGB);
  {
    Graphics graph(&bmp);
    {
      wchar_t str[25];
      wsprintf(str, L"%u", m_StreamSize.QuadPart);
      const Font font(L"Arial", 20);
      const RectF layoutRect(0, 0, REAL(cx), REAL(cx));
      StringFormat format;
      format.SetAlignment(StringAlignmentCenter);
      const SolidBrush brush(Color::Yellow);
      const Status res = graph.DrawString(str, -1, &font, layoutRect, &format, &brush);
      if (res != Ok)
        LOG(ERROR) << "DrawString() failed with code " << res;
      graph.DrawRectangle(&Pen(Color::Red, 3), layoutRect);
    }
  }
  const Status res = bmp.GetHBITMAP(Color(), phbmp);
  if (res != Ok)
    LOG(ERROR) << "GetHBITMAP() failed with code " << res;
  // TESTING END

	if( *phbmp != NULL )
		return NOERROR;
	return E_NOTIMPL;

}


STDMETHODIMP CThumbnailProvider::GetSite(REFIID riid, 
                                         void** ppvSite)
{
  if (m_pSite)
  {
    return m_pSite->QueryInterface(riid, ppvSite);
  }
  return E_NOINTERFACE;
}


STDMETHODIMP CThumbnailProvider::SetSite(IUnknown* pUnkSite)
{
  if (m_pSite)
  {
    m_pSite->Release();
    m_pSite = NULL;
  }

  m_pSite = pUnkSite;
  if (m_pSite)
  {
    m_pSite->AddRef();
  }
  return S_OK;
}
