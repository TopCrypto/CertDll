// TestDecardLib.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h> 
#pragma comment(lib, "shlwapi.lib")

typedef BOOL (*_Init)(void);
typedef BOOL (*_SetTerminalID)(unsigned char *pInBuf, DWORD InLength); 
typedef BOOL (*_GetTerminalID)(unsigned char *pOutBuf, DWORD * pOutLength); 
typedef BOOL (*_AddCert)(DWORD dwCertType, BYTE * pInBuf, DWORD InLength);
typedef DWORD (*_GetTerminalStatus)(void);

typedef BOOL (*_GenerateRSAKey)(int RsaKeyIndex);
typedef BOOL (*_ExportPubKeyRsa)(int CertType, int RsaKeyIndex, unsigned char *OutPubKey);
typedef BOOL (*_PriKeyRsaCompute)(int RsaKeyIndex, unsigned char *pInBuf, DWORD InLength, unsigned char *pOutBuf, DWORD *OutLength);
typedef void (*_DigestData_SHA1)(unsigned char *pInBuf, DWORD InLength, unsigned char Digest[20]);

#define  MAX_CET 2048

BOOL ReadCertFile(char *strPath, BYTE * pCert, DWORD* dwCertLength)
{
 
	FILE *fp = fopen(strPath, "rb");
	fseek(fp, 0, SEEK_END);
	*dwCertLength = (DWORD)ftell(fp);

	fseek(fp, 0, SEEK_SET);
	fread(pCert,1, *dwCertLength, fp);
	fclose(fp);
	return TRUE;
}

BOOL LoadData(char *fname, BYTE * pCert, DWORD *dwCertLength)
{
	BOOL bResult = FALSE;

	char selfpath[MAX_PATH];  
	GetModuleFileName(NULL, selfpath, MAX_PATH);  
	PathRemoveFileSpec(selfpath); 

	strcat(selfpath,"\\");
	strcat(selfpath, fname);

	memset(pCert, 0, MAX_CET);
	*dwCertLength = 0;
	bResult = ReadCertFile(selfpath, pCert, dwCertLength);
	if (!bResult)
	{
		goto END;
	}

END:
	return bResult;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_Init   test_Init;
	_SetTerminalID test_SetTerminalID;
	_GetTerminalID test_ReadTerminalID;
	_AddCert test_AddCert;
	_GetTerminalStatus t_GetTerminalStatus;

    _GenerateRSAKey  test_GenerateRSAKey;
	_ExportPubKeyRsa test_ExportPubKeyRsa;
	_PriKeyRsaCompute  test_PrivateRsaCompute;
	_DigestData_SHA1  test_DigestDataSHA1;

  HINSTANCE hDll;
  hDll = ::LoadLibrary("DecardMinipay.dll");
  if(hDll != NULL)
  {
	   test_Init = (_Init) ::GetProcAddress(hDll, "Init");
	   test_SetTerminalID = (_SetTerminalID) ::GetProcAddress(hDll, "SetTerminalID");
	   test_ReadTerminalID = (_GetTerminalID) ::GetProcAddress(hDll, "GetTerminalID");
	   test_AddCert = (_AddCert) ::GetProcAddress(hDll, "AddCert");
	   t_GetTerminalStatus = (_GetTerminalStatus) ::GetProcAddress(hDll, "GetTerminalStatus");

	   test_GenerateRSAKey = (_GenerateRSAKey) ::GetProcAddress(hDll, "GenerateRSAKey");
	   test_ExportPubKeyRsa = (_ExportPubKeyRsa) ::GetProcAddress(hDll, "ExportPubKeyRsa");
	   test_PrivateRsaCompute = (_PriKeyRsaCompute) ::GetProcAddress(hDll, "PriKeyRsaCompute");
	   test_DigestDataSHA1 = (_DigestData_SHA1) ::GetProcAddress(hDll, "DigestData_SHA1");
    
	   unsigned char SetID[23]; 
	   unsigned char GetID[23];
	   DWORD len = 0;
       memcpy(SetID, "00121038009150200000088", 23);
       test_SetTerminalID(SetID, 23);
	   test_ReadTerminalID(GetID, &len);

	   BYTE pCert[MAX_CET] = {0};
	   DWORD dwCertLength = 0; 

	   //清空证书数据
	   test_Init();   

       LoadData("CA.cer", pCert, &dwCertLength);
	   test_AddCert(1, pCert, dwCertLength);
	   LoadData("Pin.cer", pCert, &dwCertLength);
	   test_AddCert(2, pCert, dwCertLength);
	   LoadData("Term.cer", pCert, &dwCertLength);
	   test_AddCert(0, pCert, dwCertLength);


	    BYTE pOutKey[800] = {0};
		BYTE pIn[300] = {1,2,3,4,5,6,7};
		DWORD pInLen = 300;
		BYTE pOut[300] = {0};
		DWORD pOutLen = 0;
	    BYTE  digest[20] = {0};
	   
        test_Init();
		test_GenerateRSAKey(1);
		test_ExportPubKeyRsa(0, 1, pOutKey);
		test_PrivateRsaCompute(1, pIn, pInLen, pOut, &pOutLen);
		test_DigestDataSHA1(pOut, pOutLen, digest);

		test_Init();
		test_GenerateRSAKey(2);
		test_ExportPubKeyRsa(0, 2, pOutKey);
		test_PrivateRsaCompute(2, pIn, pInLen, pOut, &pOutLen);
		test_DigestDataSHA1(pOut, pOutLen, digest);
      
  //   DWORD ret = 0;
	 //while(1)
	 //{
  //       ret = t_GetTerminalStatus();
		// switch(ret)
		// {
		// case 1:
		//	 printf("设备插入\r\n");
		//	 break;
		// case 2:
		//	 printf("设备拔出\r\n");
		//	 break;
		// case 3:
		//	 printf("设备存在\r\n");
		//	 break;
		// case 4:
		//	 printf("设备不存在\r\n");
		//	 break;
		// default:
		//	 break;

		// }

	 //}





  }
  ::FreeLibrary(hDll);

  return 0;
}

