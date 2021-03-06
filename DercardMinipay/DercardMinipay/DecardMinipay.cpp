// DercardMinipay.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#include "SafeChannel_Tool.h"
#include "SelfDefine.h"
#include "globle.h"
#include <string>
#include "DecardMiniPay.h"
#include "sha1.h"

BOOL  bPreviousStatus = FALSE;

DWORD GetTerminalStatus()
{
   DWORD dwStart, dwEnd;
   dwStart= GetTickCount();

   for(;;)
   {   
	   dwEnd = GetTickCount();
	   if(dwEnd - dwStart > 500)
	    break;
   }

	BOOL bPresentStatus = FALSE;
	int num = 0;
	int i = 0, j = 0;

	for ( i = 0; i < MAX_READERS; i++)
	{
		for ( j = 0; j < 256; j++)
		{
			memset(g_DevList[i].strName, 0x00, 256);
		}
	}
	API_Close(g_CurDevSel);
	API_Release();

	num = API_Find();
	if (num > 0)
	{
		for ( i = 0; i < MAX_READERS; i++)
		{
			for ( j = 0; j < 256; j++)
			{
				if(g_DevList[i].strName[j] != 0  && memcmp(g_DevList[i].strName, "CUP", 3) == 0 )
				{
					break;
				}
			}
			if(j != 256)
			{
				g_CurDevSel = i;
				bPresentStatus = TRUE;
				break;
			}
		}
	}

	if(bPreviousStatus == FALSE && bPresentStatus == TRUE)  
	{
		bPreviousStatus = bPresentStatus;
		return  1;                           //Insert                    
	}
	else if(bPreviousStatus == TRUE && bPresentStatus == FALSE)
	{
		bPreviousStatus = bPresentStatus;
		return 2;                           //Out 

	}else if(bPreviousStatus == TRUE && bPresentStatus == TRUE)
	{
		bPreviousStatus = bPresentStatus;      
		return 3;                           //Exist;           
	}
	else if(bPreviousStatus == FALSE && bPresentStatus == FALSE)
	{
		bPreviousStatus = bPresentStatus;
		return  4;                         //NotExist;s  
	}

	return 0;
}

void FlushDevList(void)
{
	int num = 0;
	int i = 0, j = 0;

	for ( i = 0; i < MAX_READERS; i++)
	{
		for ( j = 0; j < 256; j++)
		{
			memset(g_DevList[i].strName, 0x00, 256);
		}
	}
	API_Close(g_CurDevSel);
	API_Release();


	num = API_Find();
	if (num > 0)
	{
		for ( i = 0; i < MAX_READERS; i++)
		{
			for ( j = 0; j < 256; j++)
			{
				if(g_DevList[i].strName[j] != 0  && memcmp(g_DevList[i].strName, "CUP", 3) == 0 )
				{
					break;
				}
			}
			if(j != 256)
			{
				g_CurDevSel = i;
				break;
			}
		}
	}
}

BOOL  Init( )
{
	BOOL ret = FALSE;

	FlushDevList();

	ret =ConnectDev();
	if(!ret)
	{
		goto END;
	}

	ret = InitData();
    if(!ret)
	{
		goto END;
	}

    ret = TRUE;
END:
	CloseDev();
	return ret;
}

BOOL ReadCert(int CertType, BYTE * pOutBuf, DWORD *OutLength)
{
	DWORD   dwRet;
	BOOL    bResult = FALSE;
	BYTE    pbCmd[256]={0};
	BYTE    pbRecv[2048]={0};
	DWORD   dwSlen = 0;
	DWORD   dwRlen = 0;
	DWORD   dwLe = 0;
	DWORD   Offset = 0;

	ConnectDev();

	pbCmd[CLA] = 0x7E;
	pbCmd[INS] = 0x23;
	pbCmd[P1] = 0x00;

	switch(CertType)
	{
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil:
		{
			pbCmd[P2] = 0x00;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
		{
			pbCmd[P2] = 0x01;
		}
		break;
	case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
		{
			pbCmd[P2] = 0x03;
		}
		break;
	default:
		goto END;
	}

	dwRet = LCardGetData(g_CurDevSel, pbCmd, dwSlen, pbRecv, &dwRlen);
	if(dwRet != SCARD_S_SUCCESS)
	{
		goto END;
	}

	memcpy(pOutBuf, pbRecv, dwRlen);
	*OutLength = dwRlen;

	bResult = TRUE;
END:
	CloseDev();
	return bResult;
}

BOOL  AddCert(int CertType, BYTE *pInBuf, DWORD InLength)
{

	DWORD dwRet;
	BOOL  bResult = FALSE;
	BYTE pContainer[CONTAINER_BUFF_SIZE] = {0};
	BYTE  pbCmd[512]={0};
	DWORD dwSlen = 0;
	DWORD len = InLength;
	DWORD datalen = 0;
	int   i = 0;
    
	BYTE CertData[2048] = {0};
	DWORD CertLen = 0;


	ConnectDev();
	
	if(CertType == 0)
	{
      bResult = CertRsaImport(eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil, pInBuf, InLength, pContainer);
	  if(!bResult)
	  {
		  bResult = FALSE;
		  goto END;
	  }
	}
	else
	{
		pbCmd[CLA] = 0x7E;
		pbCmd[INS] = 0x20;

		switch(CertType)
		{
		case eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT:
			{
				pbCmd[P1] = 0x10;
			}
			break;
		case eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin:
			{
				pbCmd[P1] = 0x40;
			}
			break;
		default:
			goto END;
		}

		if( len > 0 )
		{ 
			while(len)
			{
				datalen = (len > 0x80) ? 0x80:len;

				if(i == 0)
				{
					pbCmd[P2] = 0x80;

				}else
				{
					pbCmd[P2] = (len > 0x80) ? 0x00:0x40;
				}

				pbCmd[Lc] = (BYTE)datalen;
				memcpy(pbCmd+5, pInBuf+i*0x80, datalen);

				dwSlen = 5 +datalen;
				dwRet = LCardSendCommand(g_CurDevSel, (PBYTE)pbCmd, dwSlen);
				if(dwRet!=SCARD_S_SUCCESS)
				{
					goto END;
				}

				i++;
				len = len - datalen;
			}
		}
	}

    bResult = ReadCert(CertType, CertData, &CertLen);
	if(!bResult)
	{
		bResult = FALSE;
		goto END;
	}
    
	if(CertLen != InLength)
	{
		bResult = FALSE;
		goto END;
	}

    if(memcmp(CertData, pInBuf, CertLen) != 0)
	{
		bResult = FALSE;
		goto END;
	}

	bResult = TRUE;
END:
	CloseDev();
	return bResult;
}

BOOL SetTerminalID(unsigned char *pInBuf, DWORD InLength)
{
	DWORD   dwRet;
	BOOL    bResult = FALSE;
	BYTE    pbCmd[256]={0};
	DWORD   dwSlen = 0;
    
	FlushDevList();
	ConnectDev();

	pbCmd[CLA]=CLA_SYSTME_CMD_NONE;   
	pbCmd[INS]=INS_SYSTME_CMD;
	pbCmd[P1]=P1_SYSTEM_CMD;
	pbCmd[P2]=P2_SYSTEM_CMD;
	pbCmd[Lc]=LC_SYSTEM_CMD;
	pbCmd[DataAddr+CLA]=Data_CLA;
	pbCmd[DataAddr+INS]=Data_INS_SetTerminalInfo;
	pbCmd[DataAddr+P1]=Data_P1_MiniPay;
	pbCmd[DataAddr+P2]=P2_TerminalID; 
	pbCmd[DataAddr+Lc]=Data_Lc;
    
	if(InLength != 23)
	{
		goto END;
	}
   
	memcpy(pbCmd+Data_Offset, pInBuf, InLength);
	pbCmd[Lc] = 5 + InLength;
	pbCmd[DataAddr+Lc] = InLength;
    dwSlen = Data_Offset + InLength;

	dwRet = LCardSendCommand(g_CurDevSel, pbCmd, dwSlen);
	if(dwRet != SCARD_S_SUCCESS)
	{
		goto END;
	}

	bResult = TRUE;
END:
	CloseDev();
	return bResult;
}

BOOL  GetTerminalID(unsigned char *pOutBuf, DWORD * pOutLength)
{
	DWORD   dwRet;
	BOOL    bResult = FALSE;
	BYTE    pbCmd[256]={0};
	BYTE    pbRecv[256]={0};
	DWORD   dwSlen = 0;
	DWORD   dwRlen = 0;

    FlushDevList();
	ConnectDev();

	pbCmd[CLA] = 0x7E;
	pbCmd[INS] = 0x10;
	pbCmd[P1] = 0x00;
	pbCmd[P2] = 0x01;
	pbCmd[Lc] = 0x00;
	dwSlen = 5;
    
	dwRet = LCardGetData(g_CurDevSel, pbCmd, dwSlen, pbRecv, &dwRlen);
	if(dwRet != SCARD_S_SUCCESS)
	{
		goto END;
	}

    BYTE  tag = pbRecv[0];
	BYTE  len = pbRecv[1];
	if( tag != 0x01 || len != 0x17)
	{
		goto END;
	}
    
    memcpy(pOutBuf, pbRecv, len);
	*pOutLength = len;

	bResult = TRUE;
END:
  CloseDev();
  return bResult;
}


BOOL GenerateRSAKey(int RsaKeyIndex)
{
	BOOL ret = FALSE;

	ret = ConnectDev();
	if (!ret)
	{
		goto END;
	}

    if(RsaKeyIndex == eSafeChannelAlgorithm_ASYMKEY_ALG_RSA)
	{
       ret =  PubKeyRsaCreate();

	}else if(RsaKeyIndex == eSafeChannelAlgorithm_ASYMKEY_ALG_RSA_2048)
	{
       ret =  PubKeyRsaCreate2048();
	}

	if(!ret)
	{
		ret = FALSE;
		goto END;
	}

	ret = TRUE;
END:
	return ret;
}

BOOL ExportPubKeyRsa(int CertType, int RsaKeyIndex, unsigned char *OutPubKey)
{
	BOOL ret = FALSE;

	DWORD dwPubKeyBlobLength = 0;
	stuSafeChannelChildStuRsaPublicKeyBlob stuPubKey;
	memset(&stuPubKey, 0, sizeof(stuSafeChannelChildStuRsaPublicKeyBlob));

	ret = ConnectDev();
	if (!ret)
	{
		goto END;
	}
	if (RsaKeyIndex == eSafeChannelAlgorithm_ASYMKEY_ALG_RSA)
	{
		ret = PubKeyRsaExport(CertType, (BYTE *)&stuPubKey, dwPubKeyBlobLength);
	} 
	else if (RsaKeyIndex == eSafeChannelAlgorithm_ASYMKEY_ALG_RSA_2048)
	{
		ret = PubKeyRsaExport2048(CertType, (BYTE *)&stuPubKey, dwPubKeyBlobLength);
	}
	else
	{
		goto END;
	}
	if (!ret)
	{
		goto END;
	}

    memcpy(OutPubKey, &stuPubKey, sizeof(stuSafeChannelChildStuRsaPublicKeyBlob));

	ret = TRUE;
END:
	return ret;

}


BOOL PriKeyRsaCompute(int RsaKeyIndex, unsigned char *pInBuf, DWORD InLength, unsigned char *pOutBuf, DWORD *OutLength)
{

	BOOL ret = FALSE;

	ret = ConnectDev();
	if (!ret)
	{
		goto END;
	}
	if (RsaKeyIndex == eSafeChannelAlgorithm_ASYMKEY_ALG_RSA)
	{
		ret = PriKeyRsaCulca(pInBuf, InLength, pOutBuf, *OutLength);
	} 
	else if (RsaKeyIndex == eSafeChannelAlgorithm_ASYMKEY_ALG_RSA_2048)
	{
		ret = PriKeyRsaCulca2048(pInBuf, InLength, pOutBuf, *OutLength);
	}
	else
	{
		goto END;
	}
	if (!ret)
	{
		goto END;
	}

    ret = TRUE;
END:
	return ret;

}

void DigestData_SHA1(unsigned char *pInBuf, DWORD InLength, unsigned char Digest[20])
{
   SHA1_Data(pInBuf, InLength, Digest);
}