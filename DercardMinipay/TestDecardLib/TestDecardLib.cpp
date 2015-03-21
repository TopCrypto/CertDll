// TestDecardLib.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h> 
#pragma comment(lib, "shlwapi.lib")

typedef BOOL (*Fun_Init)(void);
typedef BOOL (*Fun_SetTerminalID)(unsigned char *pInBuf, DWORD InLength); 
typedef BOOL (*Fun_GetTerminalID)(unsigned char *pOutBuf, DWORD * pOutLength); 
typedef BOOL (*Fun_AddCert)(DWORD dwCertType, BYTE * pInBuf, DWORD InLength);
typedef DWORD (*Fun_GetTerminalStatus)(void);


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
	Fun_Init   t_Init;
	Fun_SetTerminalID t_SetTermID;
	Fun_GetTerminalID t_ReadTermID;
	Fun_AddCert t_ImportCert;
	Fun_GetTerminalStatus t_GetStatus;

  HINSTANCE hDll;
  hDll = ::LoadLibrary("DecardMinipay.dll");
  if(hDll != NULL)
  {
	   t_Init = (Fun_Init) ::GetProcAddress(hDll, "Init");
	   t_SetTermID = (Fun_SetTerminalID) ::GetProcAddress(hDll, "SetTerminalID");
	   t_ReadTermID = (Fun_GetTerminalID) ::GetProcAddress(hDll, "GetTerminalID");
	   t_ImportCert = (Fun_AddCert) ::GetProcAddress(hDll, "AddCert");
	   t_GetStatus = (Fun_GetTerminalStatus) ::GetProcAddress(hDll, "GetTerminalStatus");

	   unsigned char SetID[23]; 
	   unsigned char GetID[23];
	   DWORD len = 0;
       memcpy(SetID, "00121038009150200000088", 23);
       t_SetTermID(SetID, 23);
	   t_ReadTermID(GetID, &len);

	   //清空证书数据,初始化时只调用一次.
       t_Init();   

	   BYTE pCert[MAX_CET] = {0};
	   DWORD dwCertLength = 0; 

       LoadData("CA.cer", pCert, &dwCertLength);
	   t_ImportCert(1, pCert, dwCertLength);
	   LoadData("Pin.cer", pCert, &dwCertLength);
	   t_ImportCert(2, pCert, dwCertLength);
      
     DWORD ret = 0;
	 while(1)
	 {
         ret = t_GetStatus();
		 switch(ret)
		 {
		 case 1:
			 printf("设备插入\r\n");
			 break;
		 case 2:
			 printf("设备拔出\r\n");
			 break;
		 case 3:
			 printf("设备存在\r\n");
			 break;
		 case 4:
			 printf("设备不存在\r\n");
			 break;
		 default:
			 break;

		 }

	 }
  }
  ::FreeLibrary(hDll);

  return 0;
}

