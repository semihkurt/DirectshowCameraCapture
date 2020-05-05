#include "samplegrabbercallback.h"

static int counter = 0;

HRESULT SampleGrabberCallback::SampleCB(double SampleTime, IMediaSample *pSample){
    ::std::lock_guard<::std::mutex> lock(mMutex);
    cout << "Sample time: " << std::to_string(SampleTime) << endl;
    return 1;
}

HRESULT SampleGrabberCallback::BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen){
    ::std::lock_guard<::std::mutex> lock(mMutex);
    if(pBuffer){
        char* tBuffer = new char[BufferLen];
        memcpy(tBuffer,pBuffer,BufferLen);
        delete[] tBuffer;
    }
    cout << "Buffer Length: " << std::to_string(BufferLen) << " " << std::to_string(counter++) << endl;
    return 1;
}
