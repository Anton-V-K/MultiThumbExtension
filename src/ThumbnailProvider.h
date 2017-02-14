#ifndef ThumbnailProvider_h_
#define ThumbnailProvider_h_

#pragma once

class CThumbnailProvider : IThumbnailProvider, IObjectWithSite, IInitializeWithStream
{
private:
  LONG m_cRef;
  IUnknown* m_pSite;

  ULARGE_INTEGER m_StreamSize;
  const BYTE*    m_pStreamData;

  CThumbnailProvider();
  ~CThumbnailProvider();

public:
  static HRESULT CreateInstance(REFIID riid, void** ppvObject);

protected:
  // IUnknown methods
  STDMETHOD(QueryInterface)(REFIID, void**);
  STDMETHOD_(ULONG, AddRef)();
  STDMETHOD_(ULONG, Release)();

  // IInitializeWithSteam methods
  STDMETHOD(Initialize)(IStream*, DWORD);

  // IThumbnailProvider methods
  STDMETHOD(GetThumbnail)(UINT, HBITMAP*, WTS_ALPHATYPE*);

  // IObjectWithSite methods
  STDMETHOD(GetSite)(REFIID, void**);
  STDMETHOD(SetSite)(IUnknown*);
};

#endif // ThumbnailProvider_h_
