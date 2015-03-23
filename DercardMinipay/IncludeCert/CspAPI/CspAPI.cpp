#include "stdafx.h"
#include "CspAPI.h"

#define USER_PIN	"\x31\x32\x33\x34\x35\x36"

BOOL ConnectDev()
{
	BOOL ret = FALSE;
	if(API_Open(g_CurDevSel) == 0)
	{
		ret = TRUE;
	}
	return ret;
}
BOOL CloseDev()
{
	BOOL ret = FALSE;
	if(API_Close(g_CurDevSel) == 0 )
	{
		ret = TRUE;
	}
	return ret;
}
BOOL InitData()
{
	BOOL ret = FALSE;
	ret = LCardSendCommand(g_CurDevSel, (PBYTE)"\xFF\x10\x00\x01\x00", 5);  //切到SAM卡
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardSendCommand(g_CurDevSel, (PBYTE)"\x00\x08\x00\x00\x00", 5);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}
BOOL CertRsaImport(_In_ DWORD dwIndex, _In_ BYTE * pCert, _In_ DWORD dwCertLength, _In_ BYTE * pContainer)
{
	BOOL ret = FALSE;
	DWORD dwBinaryFileID = 0;
	DWORD dwRecordFileID = 0;
	DWORD dwAdfFileID = 0;
	BYTE pRecord[1 + CONTAINER_BUFF_SIZE] = {0};

	int offset = 0;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwBinaryFileID = ADF1_TermilCertFile_FILE_ID;
			dwRecordFileID = ADF1_ContainerFix_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwBinaryFileID = ADF1_CARootCertFile_FILE_ID;
			dwRecordFileID = ADF1_ContainerFix_FILE_ID_CA;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwBinaryFileID = ADF1_TempFile_FILE_ID;
			dwRecordFileID = ADF1_ContainerFix_FILE_ID_PIN;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	// 清理记录信息
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwRecordFileID), LOBYTE(dwRecordFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardUpdateRecord(g_CurDevSel, 1, pRecord, CONTAINER_BUFF_SIZE + 1);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	// 修改二进制文件
	offset = 0;
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwBinaryFileID), LOBYTE(dwBinaryFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	while(1)
	{	
		if (offset >= (int)dwCertLength)
		{
			break;
		}
		else if((offset + 0x80) <= (int)dwCertLength)
		{
			ret = LCardUpdateBinary(g_CurDevSel, offset, pCert + offset, 0x80);
			if(ret != SCARD_S_SUCCESS)
			{
				ret = FALSE;
				goto END;
			}
			offset += 0x80;
		}
		else
		{
			ret = LCardUpdateBinary(g_CurDevSel, offset, pCert + offset, dwCertLength - offset);
			if(ret != SCARD_S_SUCCESS)
			{
				ret = FALSE;
				goto END;
			}
			offset += dwCertLength - offset;
		}
	}
	// 改写标记
	pRecord[0] = SCI_CON_ENC_CERT_OK | SCI_CON_EXIST;
	memcpy(&pRecord[1], pContainer, CONTAINER_BUFF_SIZE);

	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwRecordFileID), LOBYTE(dwRecordFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardUpdateRecord(g_CurDevSel, 1, pRecord, CONTAINER_BUFF_SIZE + 1);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = TRUE;
END:
	return ret;
}

BOOL CertRsaExport(_In_ DWORD dwIndex, _Out_ BYTE * pCert, _Out_opt_ DWORD& dwCertLength, _Out_opt_ BOOL& bExist, _Out_ BYTE * pContainer)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwBinaryFileID = 0;
	DWORD dwRecordFileID = 0;
	BYTE pRecord[1 + CONTAINER_BUFF_SIZE] = {0};
	BYTE bLc = 0;

	int offset = 0;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwBinaryFileID = ADF1_TermilCertFile_FILE_ID;
			dwRecordFileID = ADF1_ContainerFix_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwBinaryFileID = ADF1_CARootCertFile_FILE_ID;
			dwRecordFileID = ADF1_ContainerFix_FILE_ID_CA;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwBinaryFileID = ADF1_TempFile_FILE_ID;
			dwRecordFileID = ADF1_ContainerFix_FILE_ID_PIN;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}
	
	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwRecordFileID), LOBYTE(dwRecordFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardReadRecord(g_CurDevSel, 1, pRecord, CONTAINER_BUFF_SIZE + 1);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	if ((pRecord[0]& SCI_CON_ENC_CERT_OK) == SCI_CON_ENC_CERT_OK)
	{
		bExist = TRUE;
		memcpy(pContainer, &pRecord[1], CONTAINER_BUFF_SIZE);
	}
	else
	{
		bExist = FALSE;
		ret = TRUE;
		goto END;
	}

	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwBinaryFileID), LOBYTE(dwBinaryFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardReadBinary(g_CurDevSel, 0, pCert, 4);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	dwCertLength = (pCert[2] << 8) + pCert[3];
	if (dwCertLength > 2048)
	{
		ret = FALSE;
		goto END;
	}
	dwCertLength += 4;
	offset = 0;
	while(1)
	{	
		if (offset >= (int)dwCertLength)
		{
			break;
		}
		if ((dwCertLength - offset) > 0x80)
		{
			bLc = 0x80;
		}
		else
		{
			bLc = (BYTE)(dwCertLength - offset);
		}
		ret = LCardReadBinary(g_CurDevSel, offset, pCert + offset, bLc);
		if(ret != SCARD_S_SUCCESS)
		{
			ret = FALSE;
			goto END;
		}
		offset += 0x80;
	}

	ret = TRUE;
END:
	return ret;
}


BOOL PubKeyRsaCreate()
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;
	DWORD dwPriFileID = 0;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	dwPubFileID = ADF1_RSAEncPubFile_FILE_ID;
	dwPriFileID = ADF1_RSAEncPriFile_FILE_ID;

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardGenerateRSAKey(g_CurDevSel, 
						FALSE, 
						HIBYTE(dwPubFileID), LOBYTE(dwPubFileID),
						HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = TRUE;
END:
	return ret;
}

BOOL PubKeyRsaImport(_In_ DWORD dwIndex, _In_ BYTE * pPubKeyBlob, _In_ DWORD dwPubKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;
	stuSafeChannelChildStuRsaPublicKeyBlob stuPublicKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwPubFileID = ADF1_RSAEncPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwPubFileID = ADF1_CARootPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwPubFileID = ADF1_PinEncPubFile_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	memcpy(&stuPublicKeyBlob, pPubKeyBlob, dwPubKeyBlobLength);

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardVerifyAdminPin(g_CurDevSel, (BYTE * )USER_PIN, 6);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPubFileID), LOBYTE(dwPubFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_N_Tag,
		stuPublicKeyBlob.m_pModulus,
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_E_Tag,
		stuPublicKeyBlob.m_pExponent,
		0x04);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = TRUE;
END:
	return ret;
}

BOOL PubKeyRsaExport(_In_ DWORD dwIndex, _Out_ BYTE * pPubKeyBlob, _Out_opt_ DWORD& dwPubKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;
	stuSafeChannelChildStuRsaPublicKeyBlob stuPublicKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwPubFileID = ADF1_RSAEncPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwPubFileID = ADF1_CARootPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwPubFileID = ADF1_PinEncPubFile_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPubFileID), LOBYTE(dwPubFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_N_Tag,
		stuPublicKeyBlob.m_pModulus,
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_E_Tag,
		stuPublicKeyBlob.m_pExponent,
		0x04);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	dwPubKeyBlobLength = sizeof(stuSafeChannelChildStuRsaPublicKeyBlob);
	memcpy(pPubKeyBlob, &stuPublicKeyBlob, dwPubKeyBlobLength);
	ret = TRUE;
END:
	return ret;
}

BOOL PriKeyRsaImport(_In_ BYTE * pPriKeyBlob, _In_ DWORD dwPriKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPriFileID = 0;
	stuSafeChannelChildStuRsaPrivateKeyBlob stuPriKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	dwPriFileID = ADF1_RSAEncPriFile_FILE_ID;
	
	memcpy(&stuPriKeyBlob, pPriKeyBlob, dwPriKeyBlobLength);

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardVerifyAdminPin(g_CurDevSel, (BYTE * )USER_PIN, 6);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_P_Tag,
		stuPriKeyBlob.m_prime[0],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_Q_Tag,
		stuPriKeyBlob.m_prime[1],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_DP_Tag,
		stuPriKeyBlob.m_primeExponent[0],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_DQ_Tag,
		stuPriKeyBlob.m_primeExponent[1],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_QINVP_Tag,
		stuPriKeyBlob.m_pCoefficient,
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}

BOOL PriKeyRsaExport(_Out_ BYTE * pPriKeyBlob, _Out_opt_ DWORD& dwPriKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPriFileID = 0;
	stuSafeChannelChildStuRsaPrivateKeyBlob stuPriKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	dwPriFileID = ADF1_RSAEncPriFile_FILE_ID;

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_P_Tag,
		stuPriKeyBlob.m_prime[0],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_Q_Tag,
		stuPriKeyBlob.m_prime[1],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_DP_Tag,
		stuPriKeyBlob.m_primeExponent[0],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_DQ_Tag,
		stuPriKeyBlob.m_primeExponent[1],
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		FALSE, 
		RSA_QINVP_Tag,
		stuPriKeyBlob.m_pCoefficient,
		0x40);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	
	dwPriKeyBlobLength = sizeof(stuSafeChannelChildStuRsaPrivateKeyBlob);
	memcpy(pPriKeyBlob, &stuPriKeyBlob, dwPriKeyBlobLength);

	ret = TRUE;
END:
	return ret;
}


BOOL PubKeyRsaCulca(_In_ DWORD dwIndex, 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwPubFileID = ADF1_RSAEncPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwPubFileID = ADF1_CARootPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwPubFileID = ADF1_PinEncPubFile_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardRSAEnc(g_CurDevSel, 
		FALSE, 
		HIBYTE(dwPubFileID), LOBYTE(dwPubFileID),
		pIndata, dwInDataLength,
		pOutdata, &dwOutDataLength);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}

BOOL PriKeyRsaCulca( 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPriFileID = 0;
	//stuSafeChannelChildStuRsaPrivateKeyBlob stuPriKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF1_FILE_ID;
	dwPriFileID = ADF1_RSAEncPriFile_FILE_ID;

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardRSADec(g_CurDevSel, 
		FALSE, 
		HIBYTE(dwPriFileID), LOBYTE(dwPriFileID),
		pIndata, dwInDataLength,
		pOutdata, &dwOutDataLength);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// 2048
//////////////////////////////////////////////////////////////////////////


BOOL CertRsaImport2048(_In_ DWORD dwIndex, _In_ BYTE * pCert, _In_ DWORD dwCertLength, _In_ BYTE * pContainer)
{
	BOOL ret = FALSE;
	DWORD dwBinaryFileID = 0;
	DWORD dwRecordFileID = 0;
	DWORD dwAdfFileID = 0;
	BYTE pRecord[1 + CONTAINER_BUFF_SIZE] = {0};

	int offset = 0;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwBinaryFileID = ADF3_TermilCertFile_FILE_ID;
			dwRecordFileID = ADF3_ContainerFix_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwBinaryFileID = ADF3_CARootCertFile_FILE_ID;
			dwRecordFileID = ADF3_ContainerFix_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwBinaryFileID = ADF3_TempFile_FILE_ID;
			dwRecordFileID = ADF3_ContainerFix_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	// 清理记录信息
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwRecordFileID), LOBYTE(dwRecordFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardUpdateRecord(g_CurDevSel, 1, pRecord, CONTAINER_BUFF_SIZE + 1);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	// 修改二进制文件
	offset = 0;
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwBinaryFileID), LOBYTE(dwBinaryFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	while(1)
	{	
		if (offset >=(int) dwCertLength)
		{
			break;
		}
		ret = LCardUpdateBinary(g_CurDevSel, offset, pCert + offset, 0x80);
		if(ret != SCARD_S_SUCCESS)
		{
			ret = FALSE;
			goto END;
		}
		offset += 0x80;
	}
	// 改写标记
	pRecord[0] = SCI_CON_ENC_CERT_OK | SCI_CON_EXIST;
	memcpy(&pRecord[1], pContainer, CONTAINER_BUFF_SIZE);

	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwRecordFileID), LOBYTE(dwRecordFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardUpdateRecord(g_CurDevSel, 1, pRecord, CONTAINER_BUFF_SIZE + 1);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = TRUE;
END:
	return ret;
}

BOOL CertRsaExport2048(_In_ DWORD dwIndex, _Out_ BYTE * pCert, _Out_opt_ DWORD& dwCertLength, _Out_opt_ BOOL& bExist, _Out_ BYTE * pContainer)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwBinaryFileID = 0;
	DWORD dwRecordFileID = 0;
	BYTE pRecord[1 + CONTAINER_BUFF_SIZE] = {0};
	BYTE bLc = 0;

	int offset = 0;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwBinaryFileID = ADF3_TermilCertFile_FILE_ID;
			dwRecordFileID = ADF3_ContainerFix_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwBinaryFileID = ADF3_CARootCertFile_FILE_ID;
			dwRecordFileID = ADF3_ContainerFix_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwBinaryFileID = ADF3_TempFile_FILE_ID;
			dwRecordFileID = ADF3_ContainerFix_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwRecordFileID), LOBYTE(dwRecordFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardReadRecord(g_CurDevSel, 1, pRecord, CONTAINER_BUFF_SIZE + 1);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	if ((pRecord[0] & SCI_CON_ENC_CERT_OK) == SCI_CON_ENC_CERT_OK)
	{
		bExist = TRUE;
		memcpy(pContainer, &pRecord[1], CONTAINER_BUFF_SIZE);
	}
	else
	{
		bExist = FALSE;
		ret = TRUE;
		goto END;
	}

	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwBinaryFileID), LOBYTE(dwBinaryFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardReadBinary(g_CurDevSel, 0, pCert, 4);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	dwCertLength = (pCert[2] << 8) + pCert[3];
	if (dwCertLength > 2048)
	{
		ret = FALSE;
		goto END;
	}
	dwCertLength += 4;
	offset = 0;
	while(1)
	{	
		if (offset >= (int)dwCertLength)
		{
			break;
		}
		if ((dwCertLength - offset) > 0x80)
		{
			bLc = 0x80;
		}
		else
		{
			bLc =(BYTE) (dwCertLength - offset);
		}
		ret = LCardReadBinary(g_CurDevSel, offset, pCert + offset, bLc);
		if(ret != SCARD_S_SUCCESS)
		{
			ret = FALSE;
			goto END;
		}
		offset += 0x80;
	}

	ret = TRUE;
END:
	return ret;
}


BOOL PubKeyRsaCreate2048()
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;
	DWORD dwPriFileID = 0;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	dwPubFileID = ADF3_RSAEncPubFile_FILE_ID;
	dwPriFileID = ADF3_RSAEncPriFile_FILE_ID;

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardGenerateRSAKey(g_CurDevSel, 
		TRUE, 
		HIBYTE(dwPubFileID), LOBYTE(dwPubFileID),
		HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = TRUE;
END:
	return ret;
}

BOOL PubKeyRsaImport2048(_In_ DWORD dwIndex, _In_ BYTE * pPubKeyBlob, _In_ DWORD dwPubKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;
	stuSafeChannelChildStuRsaPublicKeyBlob stuPublicKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwPubFileID = ADF3_RSAEncPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwPubFileID = ADF3_CARootPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwPubFileID = ADF3_PinEncPubFile_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	memcpy(&stuPublicKeyBlob, pPubKeyBlob, dwPubKeyBlobLength);

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardVerifyAdminPin(g_CurDevSel, (BYTE * )USER_PIN, 6);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPubFileID), LOBYTE(dwPubFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_N_Tag,
		stuPublicKeyBlob.m_pModulus,
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_E_Tag,
		stuPublicKeyBlob.m_pExponent,
		0x04);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = TRUE;
END:
	return ret;
}

BOOL PubKeyRsaExport2048(_In_ DWORD dwIndex, _Out_ BYTE * pPubKeyBlob, _Out_opt_ DWORD& dwPubKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;
	stuSafeChannelChildStuRsaPublicKeyBlob stuPublicKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwPubFileID = ADF3_RSAEncPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwPubFileID = ADF3_CARootPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwPubFileID = ADF3_PinEncPubFile_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPubFileID), LOBYTE(dwPubFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_N_Tag,
		stuPublicKeyBlob.m_pModulus,
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_E_Tag,
		stuPublicKeyBlob.m_pExponent,
		0x04);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	dwPubKeyBlobLength = sizeof(stuSafeChannelChildStuRsaPublicKeyBlob);
	memcpy(pPubKeyBlob, &stuPublicKeyBlob, dwPubKeyBlobLength);
	ret = TRUE;
END:
	return ret;
}

BOOL PriKeyRsaImport2048(_In_ BYTE * pPriKeyBlob, _In_ DWORD dwPriKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPriFileID = 0;
	stuSafeChannelChildStuRsaPrivateKeyBlob stuPriKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	dwPriFileID = ADF3_RSAEncPriFile_FILE_ID;

	memcpy(&stuPriKeyBlob, pPriKeyBlob, dwPriKeyBlobLength);

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardVerifyAdminPin(g_CurDevSel, (BYTE * )USER_PIN, 6);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_P_Tag,
		stuPriKeyBlob.m_prime[0],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_Q_Tag,
		stuPriKeyBlob.m_prime[1],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_DP_Tag,
		stuPriKeyBlob.m_primeExponent[0],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_DQ_Tag,
		stuPriKeyBlob.m_primeExponent[1],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardImportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_QINVP_Tag,
		stuPriKeyBlob.m_pCoefficient,
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}

BOOL PriKeyRsaExport2048(_Out_ BYTE * pPriKeyBlob, _Out_opt_ DWORD& dwPriKeyBlobLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPriFileID = 0;
	stuSafeChannelChildStuRsaPrivateKeyBlob stuPriKeyBlob;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	dwPriFileID = ADF3_RSAEncPriFile_FILE_ID;

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_P_Tag,
		stuPriKeyBlob.m_prime[0],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_Q_Tag,
		stuPriKeyBlob.m_prime[1],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_DP_Tag,
		stuPriKeyBlob.m_primeExponent[0],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_DQ_Tag,
		stuPriKeyBlob.m_primeExponent[1],
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardExportRSAKey(g_CurDevSel, 
		TRUE, 
		RSA_QINVP_Tag,
		stuPriKeyBlob.m_pCoefficient,
		0x80);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	dwPriKeyBlobLength = sizeof(stuSafeChannelChildStuRsaPrivateKeyBlob);
	memcpy(pPriKeyBlob, &stuPriKeyBlob, dwPriKeyBlobLength);

	ret = TRUE;
END:
	return ret;
}


BOOL PubKeyRsaCulca2048(_In_ DWORD dwIndex, 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPubFileID = 0;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	switch(dwIndex)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			dwPubFileID = ADF3_RSAEncPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			dwPubFileID = ADF3_CARootPubFile_FILE_ID;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			dwPubFileID = ADF3_PinEncPubFile_FILE_ID;
		}
		break;
	default:
		ret = FALSE;
		goto END;
	}

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardRSAEnc(g_CurDevSel, 
		TRUE, 
		HIBYTE(dwPubFileID), LOBYTE(dwPubFileID),
		pIndata, dwInDataLength,
		pOutdata, &dwOutDataLength);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}

BOOL PriKeyRsaCulca2048( 
					_In_ BYTE * pIndata, _In_ DWORD dwInDataLength, 
					_Out_ BYTE * pOutdata, _Out_opt_ DWORD& dwOutDataLength)
{
	BOOL ret = FALSE;
	DWORD dwAdfFileID = 0;
	DWORD dwPriFileID = 0;

	// 判断文件标示
	dwAdfFileID = ADF3_FILE_ID;
	dwPriFileID = ADF3_RSAEncPriFile_FILE_ID;

	// 选择 ADF
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwAdfFileID), LOBYTE(dwAdfFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	ret = LCardSelectFile(g_CurDevSel, 0, HIBYTE(dwPriFileID), LOBYTE(dwPriFileID));
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = LCardRSADec(g_CurDevSel, 
		TRUE, 
		HIBYTE(dwPriFileID), LOBYTE(dwPriFileID),
		pIndata, dwInDataLength,
		pOutdata, &dwOutDataLength);
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}
	if(ret != SCARD_S_SUCCESS)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}


