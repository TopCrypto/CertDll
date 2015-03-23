#ifndef _CSP_API_H_
#define _CSP_API_H_


#include "globle.h"
#include "SafeChannel_DataType.h"
#include "SafeChannel_SafeChip.h"

BOOL ConnectDev();
BOOL CloseDev();
BOOL InitData();

BOOL CertRsaImport(_In_ DWORD dwIndex, _In_ BYTE * pCert, _In_ DWORD dwCertLength, _In_ BYTE * pContainer);
BOOL CertRsaExport(_In_ DWORD dwIndex, _Out_ BYTE * pCert, _Out_opt_ DWORD& dwCertLength, _Out_opt_ BOOL& bExist, _Out_ BYTE * pContainer);

BOOL PubKeyRsaCreate();
BOOL PubKeyRsaImport(_In_ DWORD dwIndex, _In_ BYTE * pPubKeyBlob, _In_ DWORD dwPubKeyBlobLength);
BOOL PubKeyRsaExport(_In_ DWORD dwIndex, _Out_ BYTE * pPubKeyBlob, _Out_opt_ DWORD& dwPubKeyBlobLength);
BOOL PriKeyRsaImport( _In_ BYTE * pPriKeyBlob, _In_ DWORD dwPriKeyBlobLength);
BOOL PriKeyRsaExport(_Out_ BYTE * pPriKeyBlob, _Out_opt_ DWORD& dwPriKeyBlobLength);

BOOL PubKeyRsaCulca(_In_ DWORD dwIndex, 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength);
BOOL PriKeyRsaCulca( 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength);

//////////////////////////////////////////////////////////////////////////
// 2048
//////////////////////////////////////////////////////////////////////////

BOOL CertRsaImport2048(_In_ DWORD dwIndex, _In_ BYTE * pCert, _In_ DWORD dwCertLength, _In_ BYTE * pContainer);
BOOL CertRsaExport2048(_In_ DWORD dwIndex, _Out_ BYTE * pCert, _Out_opt_ DWORD& dwCertLength, _Out_opt_ BOOL& bExist, _Out_ BYTE * pContainer);

BOOL PubKeyRsaCreate2048();
BOOL PubKeyRsaImport2048(_In_ DWORD dwIndex, _In_ BYTE * pPubKeyBlob, _In_ DWORD dwPubKeyBlobLength);
BOOL PubKeyRsaExport2048(_In_ DWORD dwIndex, _Out_ BYTE * pPubKeyBlob, _Out_opt_ DWORD& dwPubKeyBlobLength);
BOOL PriKeyRsaImport2048( _In_ BYTE * pPriKeyBlob, _In_ DWORD dwPriKeyBlobLength);
BOOL PriKeyRsaExport2048(_Out_ BYTE * pPriKeyBlob, _Out_opt_ DWORD& dwPriKeyBlobLength);

BOOL PubKeyRsaCulca2048(_In_ DWORD dwIndex, 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength);
BOOL PriKeyRsaCulca2048( 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength);
#endif