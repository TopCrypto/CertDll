#ifndef _APP_H_
#define _APP_H_

#include "SmartCardApi.h"


////////////////////////////////////////////////////////////////////////////////////////////////
//// 发送接收
DWORD LCardSendCommand(__in KEYHANDLE  hScard, __in PBYTE pbCmd, __in DWORD dwCmdSize);
DWORD LCardGetData(__in KEYHANDLE  hScard, 
				   __in PBYTE pbCmd, __in DWORD dwCmdSize,
				   __out PBYTE pbResponse, __out_opt PDWORD pdwResponseSize);
DWORD LCardSendCommandGetData(__in KEYHANDLE  hScard, 
							  __in PBYTE pbCmd, __in DWORD dwCmdSize,
							  __out PBYTE pbResponse, __out_opt PDWORD pdwResponseSize);
////////////////////////////////////////////////////////////////////////////////////////////////
//// HardCrypt

#define GetXor(a,b,c) \
{						\
	*(c)=(*(a))^(*(b));\
	*(c+1)=(*(a+1))^(*(b+1));\
	*(c+2)=(*(a+2))^(*(b+2));\
	*(c+3)=(*(a+3))^(*(b+3));\
	*(c+4)=(*(a+4))^(*(b+4));\
	*(c+5)=(*(a+5))^(*(b+5));\
	*(c+6)=(*(a+6))^(*(b+6));\
	*(c+7)=(*(a+7))^(*(b+7));\
}

DWORD LCardGetRandom(__in KEYHANDLE  hScard,
					 __out PBYTE     pbDataOut,
					 __in  DWORD     dwDataOutLen);
////////////////////////////////////////////////////////////////////////////////////////////////
DWORD LCardSelectFile(__in KEYHANDLE hScard,
					  __in DWORD       dwFileType,
					  __in BYTE        bFileIDH,
					  __in BYTE        bFileIDL);
DWORD LCardVerifyUserPin(__in KEYHANDLE  hScard,
						 __in PBYTE       pbDataIn,
						 __in  DWORD     dwDataInLen);
DWORD LCardVerifyAdminPin(__in KEYHANDLE  hScard,
						  __in PBYTE       pbDataIn,
						  __in  DWORD     dwDataInLen);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////

DWORD LCardUpdateBinary(__in  KEYHANDLE hScard,
						__in  DWORD       dwoffset,        
						__in  PBYTE       pbBinData,
						__in  DWORD       dwDataSize
						);
DWORD LCardReadBinary(__in KEYHANDLE hScard,
					  __in DWORD      dwoffset,
					  __out PBYTE     pbDataOut,
					  __in DWORD      dwLe);
//增加记录
DWORD LCardAppendRecord(__in  KEYHANDLE hScard,       
						__in  PBYTE       pbBinData,
						__in  DWORD       dwDataSize
						);
DWORD LCardUpdateRecord(__in  KEYHANDLE hScard,
						__in  DWORD       dwRN,        
						__in  PBYTE       pbBinData,
						__in  DWORD       dwDataSize
						);
DWORD LCardReadRecord(__in KEYHANDLE hScard,
					  __in DWORD      dwRN,
					  __out PBYTE     pbDataOut,
					  __in DWORD      dwLe);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Rsa
DWORD LCardGenerateRSAKey(__in KEYHANDLE hScard,
						  __in BOOL        bRSA2048,
						  __in BYTE        bPubFileIDH,
						  __in BYTE        bPubFileIDL,
						  __in BYTE        bPriFileIDH,
						  __in BYTE        bPriFileIDL);
DWORD LCardExportRSAKey(__in KEYHANDLE hScard,
						__in BOOL       bRSA2048,
						__in DWORD      dwParmameter,
						__out PBYTE     pbDataOut,
						__in DWORD      dwLe);
DWORD LCardImportRSAKey(__in  KEYHANDLE hScard,
						__in  BOOL        bRSA2048,
						__in  DWORD       dwParmameter,        
						__in  PBYTE       pbData,
						__in  DWORD       dwDataSize
						);
DWORD LCardRSASign(__in KEYHANDLE hScard,
				   __in BOOL        bRSA2048,
				   __in BYTE        bFileIDH,
				   __in BYTE        bFileIDL,
				   __in PBYTE		pbData,
				   __in DWORD      dwDataSize,
				   __out PBYTE     pbDataOut,
				   __out PDWORD		pdwDataOutLen);
DWORD LCardRSAVerify(__in KEYHANDLE hScard,
					 __in BOOL        bRSA2048,
					 __in BYTE        bFileIDH,
					 __in BYTE        bFileIDL,
					 __in PBYTE      pbData,
					 __in DWORD      dwDataSize,
					 __out PBYTE     pbDataOut,
					 __out PDWORD		pdwDataOutLen);
DWORD LCardRSAEnc(__in KEYHANDLE hScard,
				  __in BOOL        bRSA2048,
				  __in BYTE        bFileIDH,
				  __in BYTE        bFileIDL,
				  __in PBYTE		pbData,
				  __in DWORD      dwDataSize,
				  __out PBYTE     pbDataOut,
				  __out PDWORD		pdwDataOutLen);
DWORD LCardRSADec(__in KEYHANDLE hScard,
				  __in BOOL        bRSA2048,
				  __in BYTE        bFileIDH,
				  __in BYTE        bFileIDL,
				  __in PBYTE		pbData,
				  __in DWORD      dwDataSize,
				  __out PBYTE     pbDataOut,
				  __out PDWORD		pdwDataOutLen);


////////////////////////////////////////////////////////////////////////////////////////////////
//// sm2

DWORD LCardInitECC(__in KEYHANDLE hScard);
DWORD LCardGenerateECCKey(__in KEYHANDLE hScard,
						  __in BOOL        bImportFile,
						  __in BYTE        bPubFileIDH,
						  __in BYTE        bPubFileIDL,
						  __in BYTE        bPriFileIDH,
						  __in BYTE        bPriFileIDL);
DWORD LCardExportECCKey(__in KEYHANDLE hScard,
						__in BOOL       bExportFromFile,
						__in DWORD      dwParmameter,
						__out PBYTE     pbDataOut,
						__in DWORD      dwLe);
DWORD LCardImportECCKey(__in  KEYHANDLE hScard,
						__in  BOOL        bImportFromFile,
						__in  DWORD       dwParmameter,        
						__in  PBYTE       pbData,
						__in  DWORD       dwDataSize
						);
DWORD LCardECCEnc(__in KEYHANDLE hScard, __in BOOL bENCFromFile, 
				  __in BYTE bFileIDH, __in BYTE bFileIDL, 
				  __in PBYTE pbData, __in DWORD dwDataSize, 
				  __out PBYTE pbDataOut, __out PDWORD pdwDataOutLen);
DWORD LCardECCDec(__in KEYHANDLE hScard, __in BOOL bDECFromFile, 
				  __in BYTE bFileIDH, __in BYTE bFileIDL, 
				  __in PBYTE pbData, __in DWORD dwDataSize, 
				  __out PBYTE pbDataOut, __out PDWORD pdwDataOutLen);
DWORD LCardECCVerifySign(__in KEYHANDLE hScard, __in BOOL bVerifySignFromFile, 
						 __in BYTE bFileIDH, __in BYTE bFileIDL, 
						 __in PBYTE pbHashData, __in DWORD dwHashDataSize, 
						 __in PBYTE pbSignDataR, __in DWORD dwSignDataSizeR, 
						 __in PBYTE pbSignDataS, __in DWORD dwSignDataSizeS);
DWORD LCardECCSign(__in KEYHANDLE hScard, __in BOOL bSignFromFile, 
				   __in BYTE bFileIDH, __in BYTE bFileIDL, 
				   __in PBYTE pbHashData, __in DWORD dwHashDataSize, 
				   __out PBYTE pbSignDataR, __out PDWORD pdwSignDataSizeR, 
				   __out PBYTE pbSignDataS, __out PDWORD pdwSignDataSizeS);

#endif