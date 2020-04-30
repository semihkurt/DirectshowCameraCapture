#include "samplegrabbercallbackclass.h"

SampleGrabberCallbackClass::SampleGrabberCallbackClass(DirectShowSampleGrabber *grabber)
    : m_ref(1)
    , m_grabber(grabber)
{ }

STDMETHODIMP_(ULONG) SampleGrabberCallbackClass::AddRef()
{
    std::cout << "SampleGrabberCallbackClass AddRef" << std::endl;
    return InterlockedIncrement(&m_ref);
}

STDMETHODIMP_(ULONG) SampleGrabberCallbackClass::Release()
{
    ULONG ref = InterlockedDecrement(&m_ref);
    if (ref == 0)
        delete this;
    return ref;
}

HRESULT SampleGrabberCallbackClass::QueryInterface(const IID &riid, void **ppvObject)
{
    std::cout << "SampleGrabberCallbackClass QueryInterface" << std::endl;
    if (nullptr == ppvObject)
        return E_POINTER;
    if (riid == IID_IUnknown /*__uuidof(IUnknown) */ ) {
        std::cout << "SampleGrabberCallbackClass IUnknown" << std::endl;
        AddRef();
        *ppvObject = static_cast<IUnknown *>(this);
        std::cout << "SampleGrabberCallbackClass ISampleGrabberCB" << std::endl;
        return S_OK;
    }
    if (riid == IID_ISampleGrabberCB /*__uuidof(ISampleGrabberCB)*/ ) {
        std::cout << "SampleGrabberCallbackClass ISampleGrabberCB" << std::endl;
        AddRef();
        *ppvObject = static_cast<ISampleGrabberCB *>(this);
        std::cout << "SampleGrabberCallbackClass ISampleGrabberCB" << std::endl;
        return S_OK;
    }
    return E_NOTIMPL;
}

HRESULT SampleGrabberCallbackClass::SampleCB(double time, IMediaSample *mediaSample)
{
    //        if (m_grabber)
    //            Q_EMIT m_grabber->sampleAvailable(time, mediaSample);
    // Get byte pointer
    std::cout << "Sample CB" << std::endl;
    BYTE* pbData(NULL);
    HRESULT hr = mediaSample->GetPointer(&pbData);
    if (FAILED(hr))
    {
    return hr;
    }

    return S_OK;
}

HRESULT SampleGrabberCallbackClass::BufferCB(double time, BYTE *buffer, long bufferLen)
{
    if (m_grabber) {
        // Deep copy, the data might be modified or freed after the callback returns
        //            QByteArray data(reinterpret_cast<const char *>(buffer), bufferLen);
        //            Q_EMIT m_grabber->bufferAvailable(time, data);

        std::cout << "Data come with size: " << std::to_string(bufferLen) << std::endl;
    }
    std::cout << "Buffer CB" << std::endl;
    return S_OK;
}
