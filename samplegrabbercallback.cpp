#include "samplegrabbercallback.h"

//SampleGrabberCallback::SampleGrabberCallback(){}

//HRESULT SampleGrabberCallback::SampleCB(double SampleTime, IMediaSample *pSample){
//    cout << "Sample time: " << std::to_string(SampleTime) << endl;
//    return 1;
//}

//HRESULT SampleGrabberCallback::BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen){
//    cout << "Buffer Length: " << std::to_string(BufferLen) << endl;
//    return 1;
//}

//HRESULT SampleGrabberCallback::QueryInterface(const IID &riid, void **ppvObject){cout << "QueryInterface"; return 1;}

//ULONG SampleGrabberCallback::AddRef(){cout << "Added REF" << endl; return 1;}

//ULONG SampleGrabberCallback::Release(){cout << "Released" << endl; return 1;}

HRESULT SampleGrabberCallback::SampleCB(double SampleTime, IMediaSample *pSample){
    cout << "Sample time: " << std::to_string(SampleTime) << endl;
    return 1;
}

HRESULT SampleGrabberCallback::BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen){
    char* tBuffer = new char[BufferLen];
    strcpy(tBuffer,(char*)pBuffer);
    cout << "Buffer Length: " << std::to_string(BufferLen) << endl;
    return 1;
}
