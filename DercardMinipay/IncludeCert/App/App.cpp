#include "stdafx.h"
#include "App.h"


////////////////////////////////////////////////////////////////////////////////////////////////
//// 发送接收
DWORD LCardSendCommand(__in KEYHANDLE  hScard, __in PBYTE pbCmd, __in DWORD dwCmdSize)
{
	DWORD	dwReturn = SCARD_S_SUCCESS;
	APDUEx	apdu;
	BYTE	recvbuf[300];
	DWORD	recvlen = sizeof(recvbuf);
	BYTE	SW1, SW2;
	DWORD	dwret=0;

	memset((void*)&apdu,0,sizeof(APDUEx));
	apdu.cla = pbCmd[0];
	apdu.ins = pbCmd[1];
	apdu.p1  = pbCmd[2];
	apdu.p2  = pbCmd[3];
	if(pbCmd[4] != 0 && dwCmdSize == 5)
	{
		apdu.lc  = 0x00;
		apdu.le  = pbCmd[4];
	}
	else
	{
		apdu.lc  = pbCmd[4];
		memcpy(apdu.sendbuf,&pbCmd[5],apdu.lc);
		apdu.le  = 0x00;
	}

	__try
	{
		dwReturn = API_SendApdu(hScard,&apdu);
		if ( dwReturn != SCARD_S_SUCCESS && dwReturn!=SCARD_W_RESPONSE )
		{
			goto END;
		}
		SW1 = apdu.recvbuf[apdu.le];
		SW2 = apdu.recvbuf[apdu.le+1];
		if ( ( SW1 == 0x90 ) && ( SW2 == 0x00 ) )
		{
		}
		else if ( (SW1 == 0x61))
		{
			apdu.cla = 0x00;
			apdu.ins = 0xc0;
			apdu.p1  = 0x00;
			apdu.p2  = 0x00;
			apdu.lc  = 0x00;
			apdu.le  = SW2;
			dwReturn = API_SendApdu(hScard,&apdu);
			if ( dwReturn != SCARD_S_SUCCESS && dwReturn!=SCARD_W_RESPONSE  )
			{
				goto END;
			}
			SW1 = apdu.recvbuf[apdu.le];
			SW2 = apdu.recvbuf[apdu.le+1];

			if ( ( SW1 == 0x90 ) && ( SW2 == 0x00 ) )
			{
				memcpy(recvbuf,apdu.recvbuf,apdu.le+2);
				dwReturn = SCARD_S_SUCCESS;
				goto END;
			}
			else
			{
				dwReturn =SW1*256+SW2;
				goto END;
			}
		}
		else
		{
			dwret=SW1*256+SW2;
			dwReturn=dwret;
			goto END;
		}
	}
	__finally
	{
	}
END:	
	return dwReturn;
}
DWORD LCardGetData(__in KEYHANDLE  hScard, 
				   __in PBYTE pbCmd, __in DWORD dwCmdSize,
				   __out PBYTE pbResponse, __out_opt PDWORD pdwResponseSize)
{

	DWORD	dwReturn = SCARD_S_SUCCESS;
	BYTE	recvbuf[2048];
	DWORD	recvlen = sizeof(recvbuf);
	BYTE	SW1, SW2;
	DWORD	dwDataSize = 0,dwGetSize=0;
	APDUEx	apdu;

	memset((void*)&apdu,0,sizeof(APDUEx));
	apdu.cla = pbCmd[0];
	apdu.ins = pbCmd[1];
	apdu.p1  = pbCmd[2];
	apdu.p2  = pbCmd[3];
	apdu.lc  = 0x00;
	apdu.le  = pbCmd[4];	

	__try
	{
		dwReturn = API_SendApdu(hScard,&apdu);		
		if (dwReturn != SCARD_S_SUCCESS && dwReturn!=SCARD_W_RESPONSE )
		{
			goto END;
		}
		SW1 = apdu.recvbuf[apdu.le];
		SW2 = apdu.recvbuf[apdu.le+1];
		*pdwResponseSize = apdu.le;
		dwDataSize = apdu.le;
		memcpy(pbResponse, apdu.recvbuf, apdu.le);

		if ( SW1 == 0x6C )
		{
			apdu.le = apdu.sw2;
			dwReturn = API_SendApdu(hScard,&apdu);		
			if (dwReturn != SCARD_S_SUCCESS && dwReturn!=SCARD_W_RESPONSE )
			{
				goto END;
			}
			SW1 = apdu.recvbuf[apdu.le];
			SW2 = apdu.recvbuf[apdu.le+1];
			*pdwResponseSize = apdu.le;
			dwDataSize = apdu.le;
			memcpy(pbResponse, apdu.recvbuf, apdu.le);
		}

		if ( ( SW1 == 0x90 ) && ( SW2 == 0x00 ) )
		{
			dwReturn = SCARD_S_SUCCESS;
			goto END;
		}
		if (SW1!=0x61)
		{
			dwReturn = SW1*256+SW2;
			goto END;
		}
		while (SW1==0x61)
		{
			apdu.cla = 0x7E;
			apdu.ins = 0x24;
			apdu.p1  = 0x00;
			apdu.p2  = 0x00;
			apdu.lc  = 0x00;
			apdu.le  = SW2;
			dwReturn = API_SendApdu(hScard,&apdu);
			if ( dwReturn != SCARD_S_SUCCESS && dwReturn!=SCARD_W_RESPONSE  )
			{
				goto END;
			}
			memcpy(recvbuf+dwGetSize,apdu.recvbuf,apdu.le+2);
			dwGetSize += SW2;
			SW1 = apdu.recvbuf[apdu.le];
			SW2 = apdu.recvbuf[apdu.le+1];

			if ( ( SW1 == 0x90 ) && ( SW2 == 0x00 ) )
			{
				memcpy(pbResponse+dwDataSize,recvbuf,dwGetSize);
				*pdwResponseSize = dwDataSize+dwGetSize;
				dwReturn = SCARD_S_SUCCESS;
				goto END;
			}
			if(SW1!=0x61)//not 9000 not 61XX
			{
				memcpy(pbResponse+dwDataSize,recvbuf,dwGetSize);
				*pdwResponseSize = dwDataSize+dwGetSize;
				dwReturn = SW1*256+SW2;
				goto END;
			}

		}

	}
	__finally
	{
	}
END:	
	return dwReturn;
}


DWORD LCardSendCommandGetData(__in KEYHANDLE  hScard, 
							  __in PBYTE pbCmd, __in DWORD dwCmdSize,
							  __out PBYTE pbResponse, __out_opt PDWORD pdwResponseSize)
{
	DWORD	dwReturn = SCARD_S_SUCCESS;
	APDUEx	apdu;
	BYTE	recvbuf[300];
	DWORD	recvlen = sizeof(recvbuf);
	BYTE	SW1, SW2;
	DWORD   dwDataSize =0,dwGetSize =0;

	memset((void*)&apdu,0,sizeof(APDUEx));
	apdu.cla = pbCmd[0];
	apdu.ins = pbCmd[1];
	apdu.p1  = pbCmd[2];
	apdu.p2  = pbCmd[3];
	apdu.lc  = pbCmd[4];
	apdu.le  = 0x0;
	memcpy(apdu.sendbuf,&pbCmd[5],apdu.lc);
	__try
	{
		dwReturn = API_SendApdu(hScard,&apdu);		
		if ( dwReturn != SCARD_S_SUCCESS && dwReturn!=SCARD_W_RESPONSE  )
		{
			goto END;
		}
		SW1 = apdu.recvbuf[apdu.le];
		SW2 = apdu.recvbuf[apdu.le+1];
		*pdwResponseSize = apdu.le;
		dwDataSize = apdu.le;
		memcpy(pbResponse, apdu.recvbuf, apdu.le);
		if ( ( SW1 == 0x90 ) && ( SW2 == 0x00 ) )
		{
			dwReturn = SCARD_S_SUCCESS;
			goto END;
		}
		else if (SW1!=0x61)
		{
			dwReturn = SW1*256+SW2;
			goto END;
		}
		while (SW1==0x61)
		{
			apdu.cla = 0x00;
			apdu.ins = 0xc0;
			apdu.p1  = 0x00;
			apdu.p2  = 0x00;
			apdu.lc  = 0x00;
			apdu.le  = SW2;
			dwReturn = API_SendApdu(hScard,&apdu);
			if ( dwReturn != SCARD_S_SUCCESS && dwReturn!=SCARD_W_RESPONSE )
			{
				goto END;
			}
			memcpy(recvbuf+dwGetSize,apdu.recvbuf,apdu.le+2);
			dwGetSize += SW2;
			SW1 = apdu.recvbuf[apdu.le];
			SW2 = apdu.recvbuf[apdu.le+1];

			if ( ( SW1 == 0x90 ) && ( SW2 == 0x00 ) )
			{
				memcpy(pbResponse+dwDataSize,recvbuf,dwGetSize);
				*pdwResponseSize = dwDataSize+dwGetSize;
				dwReturn = SCARD_S_SUCCESS;
				goto END;
			}
			if(SW1!=0x61)//not 9000 not 61XX
			{
				memcpy(pbResponse+dwDataSize,recvbuf,dwGetSize);
				*pdwResponseSize = dwDataSize+dwGetSize;
				dwReturn = SW1*256+SW2;
				goto END;
			}
		}


	}
	__finally
	{

	}
END:	
	return dwReturn;

}

////////////////////////////////////////////////////////////////////////////////////////////////
//// HardCrypt

DWORD LCardGetRandom(__in KEYHANDLE  hScard,
					 __out PBYTE     pbDataOut,
					 __in  DWORD     dwDataOutLen)
{
	BYTE pbGetrand[] = {0x00,0x84,0x00,0x00,0x08};
	BYTE pbResponse[256];
	DWORD dwResponseSize;
	DWORD dwRet;
	DWORD dwGetrandSize = sizeof(pbGetrand);

	pbGetrand[4]=(BYTE)dwDataOutLen;
	dwRet=LCardGetData(hScard,pbGetrand,dwGetrandSize,pbResponse,&dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	memcpy(pbDataOut,pbResponse,dwDataOutLen);
END:  
	return dwRet;
}


////////////////////////////////////////////////////////////////////////////////////////////////
DWORD LCardSelectFile(__in KEYHANDLE hScard,
					  __in DWORD       dwFileType,
					  __in BYTE        bFileIDH,
					  __in BYTE        bFileIDL)
{
	BYTE pbCmd[5+2]={0x00,0xA4,0x00,0x00,0x02};
	DWORD dwRet;

	pbCmd[2]=(BYTE)dwFileType;
	pbCmd[5]=bFileIDH;
	pbCmd[6]=bFileIDL;
	dwRet=LCardSendCommand(hScard,pbCmd,5+2);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}

DWORD LCardVerifyUserPin(__in KEYHANDLE  hScard,
						 __in PBYTE       pbDataIn,
						 __in  DWORD     dwDataInLen)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x00,0x20,0x00,0x81,0x00};

	pbCmd[4]=(BYTE)dwDataInLen;

	memcpy(pbCmd+5,pbDataIn,dwDataInLen);
	dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataInLen);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}

DWORD LCardVerifyAdminPin(__in KEYHANDLE  hScard,
						  __in PBYTE       pbDataIn,
						  __in  DWORD     dwDataInLen)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x00,0x20,0x00,0x82,0x00};

	pbCmd[4]=(BYTE)dwDataInLen;

	memcpy(pbCmd+5,pbDataIn,dwDataInLen);
	dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataInLen);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
////


DWORD LCardUpdateBinary(__in  KEYHANDLE hScard,
						__in  DWORD       dwoffset,        
						__in  PBYTE       pbBinData,
						__in  DWORD       dwDataSize
						)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x00,0xD6,0x00,0x00,0x08};
	pbCmd[2] = (BYTE)((dwoffset&0xFF00)>>8);
	pbCmd[3]= (BYTE)dwoffset;
	pbCmd[4]=(BYTE)dwDataSize;

	memcpy(pbCmd+5,pbBinData,dwDataSize);
	dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}


DWORD LCardReadBinary(__in KEYHANDLE hScard,
					  __in DWORD      dwoffset,
					  __out PBYTE     pbDataOut,
					  __in DWORD      dwLe)
{
	DWORD dwRet;
	BYTE  pbCmd[5]={0x00,0xB0,0x00,0x00,0x08};
	BYTE pbResponse[256];
	DWORD dwResponseSize=0;
	pbCmd[2] = (BYTE)((dwoffset&0xFF00)>>8);
	pbCmd[3]= (BYTE)dwoffset;
	pbCmd[4]=(BYTE)dwLe;

	dwRet=LCardGetData(hScard,pbCmd,5,pbResponse,&dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	memcpy(pbDataOut,pbResponse,dwResponseSize);

END:
	return dwRet;
}

//增加记录
DWORD LCardAppendRecord(__in  KEYHANDLE hScard,       
						__in  PBYTE       pbBinData,
						__in  DWORD       dwDataSize
						)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x00,0xE2,0x00,0x00,0x08};
	pbCmd[2] = 0x00;//记录号
	pbCmd[3]= 0x00;//当前选择的文件
	pbCmd[4]=(BYTE)dwDataSize;

	memcpy(pbCmd+5,pbBinData,dwDataSize);
	dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;

}

DWORD LCardUpdateRecord(__in  KEYHANDLE hScard,
						__in  DWORD       dwRN,        
						__in  PBYTE       pbBinData,
						__in  DWORD       dwDataSize
						)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x00,0xDC,0x00,0x00,0x08};
	pbCmd[2] = (BYTE)dwRN;//记录号
	pbCmd[3]= 0x04;//当前选择的文件
	pbCmd[4]=(BYTE)dwDataSize;

	memcpy(pbCmd+5,pbBinData,dwDataSize);
	dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}

DWORD LCardReadRecord(__in KEYHANDLE hScard,
					  __in DWORD      dwRN,
					  __out PBYTE     pbDataOut,
					  __in DWORD      dwLe)
{
	DWORD dwRet;
	BYTE  pbCmd[5]={0x00,0xB2,0x00,0x00,0x08};
	BYTE pbResponse[256];
	DWORD dwResponseSize=0;
	pbCmd[2] = (BYTE)(dwRN);
	pbCmd[3]= 0x04;//当前选择文件
	pbCmd[4]= 0x00;

	dwRet=LCardGetData(hScard,pbCmd,5,pbResponse,&dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	memcpy(pbDataOut,pbResponse,dwResponseSize);

END:
	return dwRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////

//true是2048 false是1024

DWORD LCardGenerateRSAKey(__in KEYHANDLE hScard,
						  __in BOOL        bRSA2048,
						  __in BYTE        bPubFileIDH,
						  __in BYTE        bPubFileIDL,
						  __in BYTE        bPriFileIDH,
						  __in BYTE        bPriFileIDL)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xc6,0x00,0x00,0x04};

	pbCmd[5]=bPubFileIDH;
	pbCmd[6]=bPubFileIDL;
	pbCmd[7]=bPriFileIDH;
	pbCmd[8]=bPriFileIDL;

	if(bRSA2048==TRUE)
	{
		pbCmd[2]=0x81;
	}
	else
	{
		pbCmd[2]=0x80;
	}

	dwRet=LCardSendCommand(hScard,pbCmd,5+4);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}

DWORD LCardExportRSAKey(__in KEYHANDLE hScard,
						__in BOOL       bRSA2048,
						__in DWORD      dwParmameter,
						__out PBYTE     pbDataOut,
						__in DWORD      dwLe)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xc8,0x00,0x00,0x00};
	BYTE pbResponse[256];
	DWORD dwResponseSize=0;
	DWORD dwGetLen = 0;

	pbCmd[3]= (BYTE) dwParmameter;
	if(bRSA2048==TRUE)
	{

		if(dwParmameter!=RSA_N_Tag)
		{
			pbCmd[2]= 0x81;
			dwRet=LCardGetData(hScard,pbCmd,5,pbResponse,&dwResponseSize);
			if(dwRet!=SCARD_S_SUCCESS)
			{
				goto END;
			}
			memcpy(pbDataOut,pbResponse,dwResponseSize);
		}
		else
		{
			pbCmd[2]= 0x83;
			dwRet=LCardGetData(hScard,pbCmd,5,pbResponse,&dwResponseSize);
			if(dwRet!=SCARD_S_SUCCESS)
			{
				goto END;
			}
			memcpy(pbDataOut,pbResponse,dwResponseSize);
			dwGetLen = dwResponseSize;

			pbCmd[2]= 0x81;
			dwRet=LCardGetData(hScard,pbCmd,5,pbResponse,&dwResponseSize);
			if(dwRet!=SCARD_S_SUCCESS)
			{
				goto END;
			}
			memcpy(pbDataOut+dwGetLen,pbResponse,dwResponseSize);

		}
	}
	else
	{
		pbCmd[2]= 0x80;
		dwRet=LCardGetData(hScard,pbCmd,5,pbResponse,&dwResponseSize);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}
		memcpy(pbDataOut,pbResponse,dwResponseSize);
	}



END:
	return dwRet;
}

DWORD LCardImportRSAKey(__in  KEYHANDLE hScard,
						__in  BOOL        bRSA2048,
						__in  DWORD       dwParmameter,        
						__in  PBYTE       pbData,
						__in  DWORD       dwDataSize
						)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xCA,0x00,0x00,0x00};

	pbCmd[3] = (BYTE)dwParmameter;
	if(bRSA2048 == TRUE)
	{
		if(dwParmameter!=RSA_N_Tag)
		{
			pbCmd[2] = 0x81;
			pbCmd[4] = (BYTE)dwDataSize;
			memcpy(pbCmd+5,pbData,dwDataSize);
			dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataSize);
			if(dwRet!=SCARD_S_SUCCESS)
			{
				goto END;
			}
		}
		else
		{
			pbCmd[2] = 0x81;
			pbCmd[4] = RSA_PRIME_SIZE;
			memcpy(pbCmd+5,pbData+RSA_PRIME_SIZE,RSA_PRIME_SIZE);
			dwRet=LCardSendCommand(hScard,pbCmd,5+RSA_PRIME_SIZE);
			if(dwRet!=SCARD_S_SUCCESS)
			{
				goto END;
			}
			pbCmd[2] = 0x83;
			pbCmd[4] = RSA_PRIME_SIZE;

			memcpy(pbCmd+5,pbData,RSA_PRIME_SIZE);
			dwRet=LCardSendCommand(hScard,pbCmd,5+RSA_PRIME_SIZE);
			if(dwRet!=SCARD_S_SUCCESS)
			{
				goto END;
			}
		}
	}
	else
	{
		pbCmd[2] = 0x80;
		pbCmd[4] = (BYTE)dwDataSize;
		memcpy(pbCmd+5,pbData,dwDataSize);
		dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataSize);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}
	}



END:
	return dwRet;
}

DWORD LCardRSASign(__in KEYHANDLE hScard,
				   __in BOOL        bRSA2048,
				   __in BYTE        bFileIDH,
				   __in BYTE        bFileIDL,
				   __in PBYTE		pbData,
				   __in DWORD      dwDataSize,
				   __out PBYTE     pbDataOut,
				   __out PDWORD		pdwDataOutLen)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xCC,0x00,0x00,0x80};
	BYTE  pbResponse[256];
	DWORD dwResponseSize;

	dwRet = LCardSelectFile(hScard,0x02,bFileIDH,bFileIDL);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	if(bRSA2048==TRUE)
	{
		pbCmd[2] = 0x01;
		pbCmd[3] = 0x00;
		memcpy(pbCmd+5,pbData+RSA_PRIME_SIZE,RSA_PRIME_SIZE);
		dwRet = LCardSendCommand(hScard,pbCmd,5+RSA_PRIME_SIZE);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}

		pbCmd[3] = 0x80;
		memcpy(pbCmd+5,pbData,RSA_PRIME_SIZE);
		dwRet = LCardSendCommandGetData(hScard,pbCmd,5+RSA_PRIME_SIZE,pbResponse,&dwResponseSize);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}
	}
	else
	{
		pbCmd[2] = 0x00;
		pbCmd[3] = 0x00;
		memcpy(pbCmd+5,pbData,128);
		dwRet = LCardSendCommandGetData(hScard,pbCmd,5+128,pbResponse,&dwResponseSize);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}
	}

	memcpy(pbDataOut,pbResponse,dwResponseSize);
	*pdwDataOutLen=dwResponseSize;


END:
	return dwRet;
}

DWORD LCardRSAVerify(__in KEYHANDLE hScard,
					 __in BOOL        bRSA2048,
					 __in BYTE        bFileIDH,
					 __in BYTE        bFileIDL,
					 __in PBYTE      pbData,
					 __in DWORD      dwDataSize,
					 __out PBYTE     pbDataOut,
					 __out PDWORD		pdwDataOutLen)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xCE,0x00,0x00,0x80};
	BYTE  pbResponse[256];
	DWORD dwResponseSize = 0;

	dwRet = LCardSelectFile(hScard,0x02,bFileIDH,bFileIDL);
	if(dwRet!=SCARD_S_SUCCESS)
		goto END;

	if(bRSA2048 == TRUE)
	{
		pbCmd[2] = 0x81;
		pbCmd[3] = 0x00;
		memcpy(pbCmd+5,pbData+RSA_PRIME_SIZE,RSA_PRIME_SIZE);
		dwRet = LCardSendCommand(hScard,pbCmd,5+RSA_PRIME_SIZE);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}
		pbCmd[3] = 0x80;
		memcpy(pbCmd+5,pbData,RSA_PRIME_SIZE);
		dwRet=LCardSendCommandGetData(hScard,pbCmd,5+RSA_PRIME_SIZE,pbResponse,&dwResponseSize);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}
	}
	else
	{
		pbCmd[2] = 0x80;
		pbCmd[3] = 0x00;
		memcpy(pbCmd+5,pbData,128);
		dwRet=LCardSendCommandGetData(hScard,pbCmd,5+128,pbResponse,&dwResponseSize);
		if(dwRet!=SCARD_S_SUCCESS)
		{
			goto END;
		}
	}


	memcpy(pbDataOut,pbResponse,dwResponseSize);
	*pdwDataOutLen=dwResponseSize;
END:
	return dwRet;
}

DWORD LCardRSAEnc(__in KEYHANDLE hScard,
				  __in BOOL        bRSA2048,
				  __in BYTE        bFileIDH,
				  __in BYTE        bFileIDL,
				  __in PBYTE		pbData,
				  __in DWORD      dwDataSize,
				  __out PBYTE     pbDataOut,
				  __out PDWORD		pdwDataOutLen)
{
	DWORD dwRet;

	dwRet = LCardRSAVerify(hScard, bRSA2048, bFileIDH, bFileIDL, pbData, dwDataSize, pbDataOut, pdwDataOutLen);
	return dwRet;
}

DWORD LCardRSADec(__in KEYHANDLE hScard,
				  __in BOOL        bRSA2048,
				  __in BYTE        bFileIDH,
				  __in BYTE        bFileIDL,
				  __in PBYTE		pbData,
				  __in DWORD      dwDataSize,
				  __out PBYTE     pbDataOut,
				  __out PDWORD		pdwDataOutLen)
{
	DWORD dwRet;

	dwRet = LCardRSASign(hScard, bRSA2048, bFileIDH, bFileIDL, pbData, dwDataSize, pbDataOut, pdwDataOutLen);
	return dwRet;
}


////////////////////////////////////////////////////////////////////////////////////////////////
DWORD LCardInitECC(__in KEYHANDLE hScard)
{
	BYTE pbCmd[5+2]={0x80,0xE4,0x00,0x00,0x00};
	DWORD dwRet;

	dwRet=LCardSendCommand(hScard, pbCmd, 5);
	if(dwRet != SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}

DWORD LCardGenerateECCKey(__in KEYHANDLE hScard,
						  __in BOOL        bImportFile,
						  __in BYTE        bPubFileIDH,
						  __in BYTE        bPubFileIDL,
						  __in BYTE        bPriFileIDH,
						  __in BYTE        bPriFileIDL)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xE6,0x00,0x00,0x04};

	pbCmd[5]=bPubFileIDH;
	pbCmd[6]=bPubFileIDL;
	pbCmd[7]=bPriFileIDH;
	pbCmd[8]=bPriFileIDL;

	if(bImportFile==TRUE)
	{
		pbCmd[2]=0x01;
	}
	else
	{
		pbCmd[2]=0x00;
	}

	dwRet=LCardSendCommand(hScard,pbCmd,5+4);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
END:
	return dwRet;
}

DWORD LCardExportECCKey(__in KEYHANDLE hScard,
						__in BOOL       bExportFromFile,
						__in DWORD      dwParmameter,
						__out PBYTE     pbDataOut,
						__in DWORD      dwLe)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xE8,0x00,0x00,0x00};
	BYTE pbResponse[256];
	DWORD dwResponseSize=0;
	DWORD dwGetLen = 0;
	if (bExportFromFile)
	{
		pbCmd[2]= 0x01;
	}
	else
	{
		pbCmd[2]= 0x00;
	}
	pbCmd[3]= (BYTE) dwParmameter;
	pbCmd[4] = 0x20;
	dwRet=LCardGetData(hScard,pbCmd,5,pbResponse,&dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	memcpy(pbDataOut,pbResponse,dwResponseSize);


END:
	return dwRet;
}


DWORD LCardImportECCKey(__in  KEYHANDLE hScard,
						__in  BOOL        bImportFromFile,
						__in  DWORD       dwParmameter,        
						__in  PBYTE       pbData,
						__in  DWORD       dwDataSize
						)
{
	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xEA,0x00,0x00,0x00};

	if (bImportFromFile)
	{
		pbCmd[2]= 0x01;
	}
	else
	{
		pbCmd[2]= 0x00;
	}
	pbCmd[3]= (BYTE) dwParmameter;
	pbCmd[4] = (BYTE)dwDataSize;
	memcpy(pbCmd+5,pbData,dwDataSize);
	dwRet=LCardSendCommand(hScard,pbCmd,5+dwDataSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}

END:
	return dwRet;
}

DWORD LCardECCEnc(__in KEYHANDLE hScard, __in BOOL bENCFromFile, 
				  __in BYTE bFileIDH, __in BYTE bFileIDL, 
				  __in PBYTE pbData, __in DWORD dwDataSize, 
				  __out PBYTE pbDataOut, __out PDWORD pdwDataOutLen)
{

	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xF6,0x00,0x00,0x80};
	BYTE  pbResponse[256];
	DWORD dwResponseSize;

	if(bENCFromFile)
	{
		pbCmd[2] = bFileIDH;
		pbCmd[3] = bFileIDL;
	}
	else
	{
		pbCmd[2] = 0xFF;
		pbCmd[3] = 0xFF;
	}
	if (dwDataSize >= 0xE0)
	{
		return SCARD_F_WAITED_TOO_LONG;
	}
	memcpy(pbCmd+5,pbData,dwDataSize);
	pbCmd[4] = (BYTE)dwDataSize;
	dwRet = LCardSendCommandGetData(hScard, pbCmd, 5+dwDataSize, pbResponse, &dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	memcpy(pbDataOut,pbResponse,dwResponseSize);
	*pdwDataOutLen=dwResponseSize;

END:
	return dwRet;
}


DWORD LCardECCDec(__in KEYHANDLE hScard, __in BOOL bDECFromFile, 
				  __in BYTE bFileIDH, __in BYTE bFileIDL, 
				  __in PBYTE pbData, __in DWORD dwDataSize, 
				  __out PBYTE pbDataOut, __out PDWORD pdwDataOutLen)
{

	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xF8,0x00,0x00,0x80};
	BYTE  pbResponse[256];
	DWORD dwResponseSize;

	if(bDECFromFile)
	{
		pbCmd[2] = bFileIDH;
		pbCmd[3] = bFileIDL;
	}
	else
	{
		pbCmd[2] = 0xFF;
		pbCmd[3] = 0xFF;
	}
	if (dwDataSize <= 0xE0)
	{
		return SCARD_W_EOF;
	}
	memcpy(pbCmd+5,pbData,dwDataSize);
	pbCmd[4] = (BYTE)dwDataSize;
	dwRet = LCardSendCommandGetData(hScard, pbCmd, 5+dwDataSize, pbResponse, &dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	memcpy(pbDataOut,pbResponse,dwResponseSize);
	*pdwDataOutLen=dwResponseSize;

END:
	return dwRet;
}


DWORD LCardECCVerifySign(__in KEYHANDLE hScard, __in BOOL bVerifySignFromFile, 
						 __in BYTE bFileIDH, __in BYTE bFileIDL, 
						 __in PBYTE pbHashData, __in DWORD dwHashDataSize, 
						 __in PBYTE pbSignDataR, __in DWORD dwSignDataSizeR, 
						 __in PBYTE pbSignDataS, __in DWORD dwSignDataSizeS)
{

	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xEE,0x00,0x00,0x80};
	BYTE  pbResponse[256];
	DWORD dwResponseSize;

	if(bVerifySignFromFile)
	{
		pbCmd[2] = bFileIDH;
		pbCmd[3] = bFileIDL;
	}
	else
	{
		pbCmd[2] = 0xFF;
		pbCmd[3] = 0xFF;
	}

	memcpy(pbCmd + 5, pbHashData, dwHashDataSize);
	memcpy(pbCmd + 5 + dwHashDataSize, pbSignDataR, dwSignDataSizeR);
	memcpy(pbCmd + 5 + dwSignDataSizeR, pbSignDataS, dwSignDataSizeS);

	pbCmd[4] = (BYTE)(dwHashDataSize + dwSignDataSizeR + dwSignDataSizeS);

	dwRet = LCardSendCommandGetData(hScard, pbCmd, 5 + pbCmd[4], pbResponse, &dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
END:
	return dwRet;
}


DWORD LCardECCSign(__in KEYHANDLE hScard, __in BOOL bSignFromFile, 
				   __in BYTE bFileIDH, __in BYTE bFileIDL, 
				   __in PBYTE pbHashData, __in DWORD dwHashDataSize, 
				   __out PBYTE pbSignDataR, __out PDWORD pdwSignDataSizeR, 
				   __out PBYTE pbSignDataS, __out PDWORD pdwSignDataSizeS)
{

	DWORD dwRet;
	BYTE  pbCmd[255]={0x80,0xEC,0x00,0x00,0x80};
	BYTE  pbResponse[256];
	DWORD dwResponseSize;

	if(bSignFromFile)
	{
		pbCmd[2] = bFileIDH;
		pbCmd[3] = bFileIDL;
	}
	else
	{
		pbCmd[2] = 0xFF;
		pbCmd[3] = 0xFF;
	}
	memcpy(pbCmd+5, pbHashData, dwHashDataSize);
	pbCmd[4] = (BYTE)dwHashDataSize;
	dwRet = LCardSendCommandGetData(hScard, pbCmd, 5+dwHashDataSize, pbResponse, &dwResponseSize);
	if(dwRet!=SCARD_S_SUCCESS)
	{
		goto END;
	}
	memcpy(pbSignDataR, pbResponse,(dwResponseSize/2));
	memcpy(pbSignDataS, pbResponse + (dwResponseSize/2),dwResponseSize/2);
	*pdwSignDataSizeR = dwResponseSize/2;
	*pdwSignDataSizeS = dwResponseSize/2;

END:
	return dwRet;
}