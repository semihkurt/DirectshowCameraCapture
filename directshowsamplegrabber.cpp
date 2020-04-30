#include "directshowsamplegrabber.h"
#include <iostream>
#include <string>
#include <samplegrabbercallbackclass.h>
#include <strsafe.h>

DirectShowSampleGrabber::DirectShowSampleGrabber() :
    mFilter(nullptr),
    mSampleGrabber(nullptr),
    mSampleGrabberCB(nullptr)
{

}

DirectShowSampleGrabber::~DirectShowSampleGrabber()
{
    stop();
//    SAFE_RELEASE(mSampleGrabberCB);
//    SAFE_RELEASE(mFilter);
    //    SAFE_RELEASE(mSampleGrabber);
}

void DirectShowSampleGrabber::create(ISampleGrabber *pSampleGrabber)
{
    // Create sample grabber filter
    mSampleGrabber = pSampleGrabber;
    HRESULT hr;
    hr = mSampleGrabber->QueryInterface(IID_IBaseFilter, reinterpret_cast<void **>(&mFilter));
    if (FAILED(hr))
        std::cout << "Failed to get base filter interface" << std::endl;
}

void DirectShowSampleGrabber::create()
{
    // Create sample grabber filter
    HRESULT hr = CoCreateInstance(CLSID_SampleGrabber, nullptr, CLSCTX_INPROC, IID_ISampleGrabber, reinterpret_cast<void **>(&mSampleGrabber));
    if (FAILED(hr)) {
        std::cout << "Failed to create sample grabber" << std::endl;
        return;
    }
    hr = mSampleGrabber->QueryInterface(IID_IBaseFilter, reinterpret_cast<void **>(&mFilter));
    if (FAILED(hr))
        std::cout << "Failed to get base filter interface" << std::endl;
}

void DirectShowSampleGrabber::stop()
{
    if (!mSampleGrabber)
        return;
    if (FAILED(mSampleGrabber->SetCallback(nullptr, static_cast<long>(mCallbackType)))) {
        std::cout << "Failed to stop sample grabber callback" << std::endl;
        return;
    }
}

bool DirectShowSampleGrabber::isValid() const
{
    return mFilter && mSampleGrabber;
}
bool DirectShowSampleGrabber::getConnectedMediaType(AM_MEDIA_TYPE *mediaType)
{
//    Q_ASSERT(mediaType);
    if (!isValid())
        return false;
    if (FAILED(mSampleGrabber->GetConnectedMediaType(mediaType))) {
        std::cout <<  "Failed to retrieve the connected media type" << std::endl;
        return false;
    }
    return true;
}
bool DirectShowSampleGrabber::setMediaType(const AM_MEDIA_TYPE *mediaType)
{
//    Q_ASSERT(mediaType);
    if (FAILED(mSampleGrabber->SetMediaType(mediaType))) {
         std::cout << "Failed to set media type" << std::endl;
        return false;
    }
    return true;
}

IBaseFilter *DirectShowSampleGrabber::getFilter()
{
    return mFilter;
}

ISampleGrabber *DirectShowSampleGrabber::getSampleGrabber()
{
    return mSampleGrabber;
}

void DirectShowSampleGrabber::start(DirectShowSampleGrabber::CallbackMethod method,
                                    bool oneShot,
                                    bool bufferSamples)
{

    if (!mSampleGrabber){
        return;
    }
    stop();
    if (!mSampleGrabberCB)
        mSampleGrabberCB = new SampleGrabberCallbackClass(this);
    mCallbackType = method;
    mSampleGrabber->SetCallback(mSampleGrabberCB, static_cast<long>(mCallbackType));
    mSampleGrabber->SetOneShot(oneShot);
    mSampleGrabber->SetBufferSamples(bufferSamples);
}
