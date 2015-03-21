#ifndef _DECARDMINIPAY_H_
#define _DECARDMINIPAY_H_
    extern "C" _declspec(dllexport)  DWORD GetTerminalStatus(void);                         
    extern "C" _declspec(dllexport)  BOOL  Init();
	extern "C" _declspec(dllexport)  BOOL  SetTerminalID(unsigned char *pInBuf, DWORD InLength); 
	extern "C" _declspec(dllexport)  BOOL  GetTerminalID(unsigned char *pOutBuf, DWORD * pOutLength); 
	extern "C" _declspec(dllexport)  BOOL  AddCert(DWORD dwCertType, BYTE *pInBuf, DWORD InLength);
#endif




