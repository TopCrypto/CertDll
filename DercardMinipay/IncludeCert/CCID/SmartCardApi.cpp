#include "stdafx.h"

#include <Winscard.h>

#include "tool.h"
#include "SmartCardApi.h"
#pragma comment(lib, "Winscard.lib")



#pragma warning( disable: 4996) 

SCARDCONTEXT g_sc;



// ȫ�ֱ�������===========================================================

SMARTCARD_DEVICE g_DevList[MAX_READERS];
//int g_DevNum = 0;
int g_dwAp = 0;


s_Error error_list[ERROR_NUM] = 
{
	{0x90, 0x00, SCARD_S_SUCCESS, 0},	// �ɹ�
	// ===========================================================
	{0x61, 0xFF, SCARD_W_RESPONSE, TRANSFORMABLE_SW2},	// ����������Ҫ����
	{0x62, 0x83, SCARD_E_NO_FILE, 0},
	{0x65, 0x81, SCARD_E_EPROM_ERROR, 0},
	{0x67, 0x00, SCARD_E_INVALID_VALUE, 0},
	{0x69, 0x82, SCARD_W_SECURITY_VIOLATION, 0},
	{0x69, 0x85, SCARD_E_FILE_EXISTS, 0},
	{0x6a, 0x82, SCARD_E_FILE_NOT_FOUND, 0},
	{0x6a, 0x83, SCARD_W_EOF, 0},
	{0x6a, 0x84, SCARD_E_WRITE_TOO_MANY, 0},
	{0x6b, 0x00, SCARD_E_INVALID_PARAMETER, 0},
	{0x6d, 0x00, SCARD_E_INVALID_INS, 0},
	{0x6d, 0x81, SCARD_E_INVALID_INSTRUCTION, 0},
	{0x6d, 0x82, SCARD_E_VM_ADDRESS_ERROR, 0},
	{0x6d, 0x8f, SCARD_E_ZERO_DIVIDE, 0},
	{0x6e, 0x00, SCARD_E_INVALID_CLA, 0},
	{0x6f, 0x81, SCARD_E_UNEXPECTED, 0},
	
};
DWORD ErrorCodeConvert(BYTE sw1, BYTE sw2)
{
	for (int i=0;i<ERROR_NUM;i++)
	{
		if (sw1 == error_list[i].sw1)
		{
			if (error_list[i].mode & TRANSFORMABLE_SW2) 
				return(error_list[i].error_code);
			if (sw2 == error_list[i].sw2) 
				return(error_list[i].error_code);
		}
	}
	return(SCARD_F_UNKNOWN_ERROR);
}



//////////////////////////////////////////////////////////////////////////
// EXTERN_C int WINAPI API_Find();
// ���ܣ��ڵ�ǰ�����²����Ѿ����ӵģ����ҿ��������򿪵�����ָ���豸��
// �������ҵ����豸������       
// ��������
// ���أ�
//		һ������0��ֵ	��������,
//		-1	ִ��ʧ��
//////////////////////////////////////////////////////////////////////////

int API_Find()
{
	LONG result;
	LPTSTR pmszReaders = NULL;
	DWORD cch = SCARD_AUTOALLOCATE;
	LPTSTR pReader = NULL;
	DWORD dwActiveProtocol = 0; 
	SCARDHANDLE  hSCard;
	DWORD dwRet;
	int NameLen =0;
	int offset =0;
	int DevNum = 0;
	//char *pDefaultName = "D&C W2310 USBKEY 0";
    //char *pDefaultName = "DECARD DC VD100 READER 0";
    //char *pDefaultName = "Decard Smart Card Reader 0";
	char *pDefaultName = "Gemplus USB Smart Card Reader 0";
	int DefaultNameLen = (int)strlen(pDefaultName);
	


	//DEBUG_Printf_String("\r\n%s\r\n", "//--------------------- [9/24/2014 seraph]");
	//DEBUG_Printf_String("API_Find()");
	//DEBUG_Printf_String("%s", "\r\n");


	result = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &g_sc);
	if(result != SCARD_S_SUCCESS)
	{
		return -1;
	}
   
	result = SCardListReaders(g_sc, NULL, (LPTSTR)&pmszReaders, &cch);
	if(result != SCARD_S_SUCCESS)
	{
		dwRet = result;
		return -1;
	}
	pReader = pmszReaders;
	while(*pReader != 0)
	{
		//if(memcmp(pReader, pDefaultName, DefaultNameLen - 2)==0)
		//if(1)
		{
			result = SCardConnect(g_sc, (LPCSTR)pReader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0|SCARD_PROTOCOL_T1, &hSCard, &dwActiveProtocol);
			if(result == SCARD_S_SUCCESS)
			{
				strcpy(g_DevList[DevNum].strName, pReader);
				SCardDisconnect(hSCard, SCARD_LEAVE_CARD);
				DevNum++;
				if(DevNum == MAX_READERS)
					break;
			}
		}
		NameLen = (int)strlen(pReader) + 1;
		pReader = pReader + strlen(pReader) + 1;
		offset+=NameLen;
	}
	SCardFreeMemory(g_sc, pmszReaders);

	return DevNum;
}


// ====================================================================
// EXTERN_C int WINAPI API_Open(int index);
// ���ܣ�����index������ָ�����豸����������0��ʼ��
// �����豸index
// ������
// ������	����	In/out	����ѡ��
// index	�豸��ȫ���豸�б��е�������	In	�������ɡ�0����ʼ
// ���أ�
//			һ����С��0��ֵ	�ɹ�
//			-1	ʧ��
// ====================================================================
int API_Open(int index)
{
	LONG lReturn;
	SCARDHANDLE hKey;
	DWORD        dwAP;
	//g_DevList[index].mutex.EnterSynCode();

	//DEBUG_Printf_String("%s", "\r\n");
	//DEBUG_Printf_String("Open Dev = %s)", g_DevList[index].strName);
	//DEBUG_Printf_String("%s", "\r\n");
	//DEBUG_Printf_String("%s", "\r\n");

	lReturn = SCardConnect(g_sc,
		g_DevList[index].strName,
		SCARD_SHARE_SHARED,
		SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
		&hKey,
		&dwAP);
	if(SCARD_S_SUCCESS != lReturn)
	{
		g_DevList[index].bOpened=0;
		//g_DevList[index].mutex.LeaveSynCode();
		return -1;
	}
		
	g_DevList[index].hCard = (HANDLE)hKey;
	g_DevList[index].bOpened = 1;
	g_DevList[index].dwActiveProtocol = dwAP;
	g_dwAp = dwAP;
	//g_DevList[index].mutex.LeaveSynCode();
	return index;		
}


// ====================================================================
// EXTERN_C int WINAPI API_Close(int index);
// ����: �ر�indexָ�����豸
// 	������
// 		index	�豸��ȫ���豸�б��е�������
// 	���أ�
// 		0	��������
// 		-1	ִ��ʧ��
// ====================================================================
int API_Close(int index)
{
	LONG lReturn =-1;
	g_DevList[index].bOpened=0;
	//g_DevList[index].mutex.LeaveSynCode();


	//DEBUG_Printf_String("%s", "\r\n");
	//DEBUG_Printf_String("%s", "\r\n");
	//DEBUG_Printf_String("Close Dev");
	//DEBUG_Printf_String("%s", "\r\n");
	//DEBUG_Printf_String("%s", "\r\n");

	if (g_DevList[index].hCard)
	{
		lReturn = SCardDisconnect((SCARDHANDLE)g_DevList[index].hCard, SCARD_LEAVE_CARD);
		if(lReturn != SCARD_S_SUCCESS)
			return -1;
	}
	
	return SCARD_S_SUCCESS;
}

//---------------------------------------------------------------------------
//	ժҪ:
//	  �ڳ������ʱʱ����Ҫ���������Թ����������ڲ�����ʹ��
//	����ֵ:
//	  ����0��ʾ��ȷ�������ļ��������
//---------------------------------------------------------------------------

int API_Release()
{
	LONG lReturn=-1;
	lReturn = SCardReleaseContext(g_sc);
	if ( SCARD_S_SUCCESS != lReturn )
		return -1;
	g_sc = NULL;
	return SCARD_S_SUCCESS;
	
}



// ====================================================================
// ����APDU,�����ؽ��
//���룺
//	   hDev	�豸�ľ��
//     bApdu:apdu���ݰ�
//���:
// ����ֵ:

// ====================================================================

DWORD SendApdu(HANDLE hDev,pAPDUEx apdu)
{
	BYTE buf[BULK_BUFF_SIZE];
	BYTE pbSend[BULK_BUFF_SIZE],pbRecv[BULK_BUFF_SIZE];
	DWORD dwSendLen =0, dwRecvLen=0,dwOutputLen =0,off = 0;
	BYTE sw1, sw2;        //���ش���
	LONG lReturn = -1;
	DWORD retcode = SCARD_S_SUCCESS;
	
	memset(pbSend,0,sizeof(pbSend));
	memset(buf,0,sizeof(buf));
	
	pbSend[0]=apdu->cla;
	pbSend[1]=apdu->ins;
	pbSend[2]=apdu->p1;
	pbSend[3]=apdu->p2;
	dwSendLen = 4;
	
	if (apdu->lc != 0)
	{
		pbSend[4] = apdu->lc;
		dwSendLen++;
		memcpy(&pbSend[5],apdu->sendbuf,apdu->lc);
		dwSendLen+=apdu->lc;
		pbSend[dwSendLen + 1] = 0x00;
	}
	else
	{
		if (apdu->le != 0)
		{
			pbSend[4] = apdu->le;
			dwSendLen++;
			pbSend[dwSendLen + 1] = 0x00;
		}
		else
		{
			pbSend[4] = 0x00;
			dwSendLen++;
			pbSend[dwSendLen + 1] = 0x00;
		}
	}

	dwRecvLen = BULK_BUFF_SIZE;
	

	PrintSend(pbSend, dwSendLen);

	if (g_dwAp == SCARD_PROTOCOL_T0)
	{
		lReturn = SCardTransmit((SCARDHANDLE)hDev, SCARD_PCI_T0, pbSend, dwSendLen, NULL, pbRecv, &dwRecvLen);
	} 
	else if (g_dwAp == SCARD_PROTOCOL_T1)
	{
		lReturn = SCardTransmit((SCARDHANDLE)hDev, SCARD_PCI_T1, pbSend, dwSendLen, NULL, pbRecv, &dwRecvLen);
	}
	else
	{
		lReturn = SCardTransmit((SCARDHANDLE)hDev, SCARD_PCI_RAW, pbSend, dwSendLen, NULL, pbRecv, &dwRecvLen);
	}
	if ( lReturn != SCARD_S_SUCCESS)
	{
		//DEBUG_Printf_String("%s%x\r\n", "SCardTransmit Error = ", lReturn);
		return lReturn;
	}

	PrintRecv(pbRecv, dwRecvLen);

	if(dwRecvLen<2)
	{
		return SCARD_F_UNKNOWN_ERROR;
	}
 	apdu->le = (BYTE)(dwRecvLen-2);
	memcpy(apdu->recvbuf,pbRecv,dwRecvLen);	
	sw1 = pbRecv[dwRecvLen-2];
	sw2 = pbRecv[dwRecvLen-1];
	apdu->sw1 = sw1;
	apdu->sw2 = sw2;
	retcode = ErrorCodeConvert(sw1,sw2);
	return retcode;
	
}

// ====================================================================
// EXTERN_C DWORD WINAPI API_SendApdu(int index,pAPDUEx apdu);
// ���ܣ�����APDU����ͨѶ��
// ������
// index	�豸��ȫ���豸�б��е�������
// apdu	    APDU���ݰ�
// ���أ�
// SCARD_S_SUCCESS	��������,
// ====================================================================
int API_SendApdu(int index, pAPDUEx apdu)
{
	int retcode;

/*
	apdu1.cla = 0xFF;
	apdu1.ins = 0x10;
	apdu1.p1 = 0x00;
	apdu1.p2 = 0x01;
	apdu1.lc = 0x00;
	apdu1.le = 0x00;
	
	SendApdu(g_DevList[index].hCard, &apdu1);
*/	
	retcode = SendApdu(g_DevList[index].hCard,apdu);

	return retcode;
}

// ====================================================================
//���ATR
// ====================================================================

int API_GetATR(int index, LPBYTE pbAtrBytes, int *pbByte)
{
	LPBYTE pbAttr = NULL;
	ULONG lReturn = -1;
	DWORD cbByte = SCARD_AUTOALLOCATE;
	//g_DevList[index].mutex.EnterSynCode();
	
	lReturn = SCardGetAttrib((SCARDHANDLE)g_DevList[index].hCard, SCARD_ATTR_ATR_STRING, (LPBYTE)&pbAttr, &cbByte);
	if(lReturn != SCARD_S_SUCCESS)
	{
		return -1;
	}
	memcpy(pbAtrBytes,pbAttr,cbByte);
	*pbByte = cbByte;
	
	//g_DevList[index].mutex.LeaveSynCode();
	return SCARD_S_SUCCESS;
}

// =====================================================================================================
// EXTERN_C DWORD WINAPI API_EnumDev(LPSTR szNameList, int *ReaderNum,ULONG *pulSize);
// ���ܣ�ö�������Ѿ����ӵģ����ҿ��Գɹ��򿪵������豸�������豸��Ϣ�洢��һ���豸���б���
// ������
// szNameList	�洢�豸�����б�ռ�	Out	ÿ���豸�������Ե�����\0������,��˫��\0����ʾ�б�Ľ�����
// ReaderNum	�����ӿɴ򿪵��豸����	Out	��С��0��ֵ
// pulSize	�豸�����б�洢�ռ��С	In/Out	����ʱ��ʾ�����豸�����б�洢�ռ��С��
//                                              ���ʱ��ʾ����ʵ�ʵ��豸�����б�洢�ռ��С��
// ���أ�
// SCARD_S_SUCCESS	��������,
// ======================================================================================================
int API_EnumDev(LPSTR szNameList, int *ReaderNum, ULONG *pulSize)
{
	DWORD dwRet = SCARD_S_SUCCESS;
	int num = -1;
	int i = 0;
	DWORD NameLen =0,offset =0,listsize=0;
	
	// Establish the context.
	num = API_Find();
	if ( num  <= 0  )
	{
		return SCARD_ERR_OPEN_DEVICE;	
	}
	//find the Application
	for(i=0;i<num;i++)
	{
		if(g_DevList[i].strName[0]!='\0')
		{	
			NameLen = (ULONG)strlen(g_DevList[i].strName)+ 1;
			offset+=NameLen;
		}
	}
	listsize = offset+1;
	if(szNameList==NULL||*pulSize<listsize)
	{
		*pulSize = listsize;	
		return SCARD_E_INSUFFICIENT_BUFFER;
		
	}
	offset =0;
	for(i=0;i<num;i++)
	{
		if(g_DevList[i].strName[0]!='\0')
		{	
			NameLen = (ULONG)strlen(g_DevList[i].strName)+ 1;
			memcpy(szNameList+offset,g_DevList[i].strName,NameLen);
			offset+=NameLen;
		}
	}
	*pulSize = listsize;
	szNameList[listsize-1]='\0';
	*ReaderNum = num;
	return SCARD_S_SUCCESS;
}

// =====================================================================================================
// EXTERN_C int WINAPI API_ConnectDev(LPSTR szName);
// ���ܣ�����ָ�����豸���ƴ��豸 
// �����豸��index
// ������
// szName	ָ�����豸����
// ���أ�
//			һ����С��0��ֵ	��������,
//			-1	����ʧ��
// ======================================================================================================
int API_ConnectDev(LPSTR szName)
{
	int i=0,index = -1;
	LONG lReturn;
	SCARDHANDLE hKey;
	DWORD        dwAP;
	int num = -1;
	
	// Establish the context.
	num = API_Find();
	if ( num  <= 0  )
	{
		return -1;	
	}
	for(i=0;i<MAX_READERS;i++)
	{
		if(strcmp(g_DevList[i].strName,szName)==0)
		{
			index=i;
			break;
		}
	}
	if(i==MAX_READERS)
	{
		return -1;
	}
	
	//g_DevList[index].mutex.EnterSynCode();
	
	lReturn = SCardConnect(g_sc,
		g_DevList[index].strName,
		SCARD_SHARE_SHARED,
		SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
		&hKey,
		&dwAP);
	if(SCARD_S_SUCCESS != lReturn)
	{
		g_DevList[index].bOpened=0;
		//g_DevList[index].mutex.LeaveSynCode();
		return -1;
	}
	
	g_DevList[index].hCard = (HANDLE)hKey;
	g_DevList[index].bOpened=1;
	g_DevList[index].dwActiveProtocol = dwAP;
	g_dwAp = dwAP;
	//g_DevList[index].mutex.LeaveSynCode();
	return index;	
}


// =====================================================================================================
// EXTERN_C int WINAPI API_ConnectOneDev();
// ���ܣ�Ĭ�ϴ򿪵�һ�������豸�������豸index
// ��������
// ���أ�
//		һ����С��0��ֵ	��������,
//		-1	ʧ��
// ======================================================================================================
int API_ConnectOneDev()
{
	LONG lReturn;
	SCARDHANDLE hKey;
	DWORD        dwAP;
	int num = -1;
	
	// Establish the context.
	num = API_Find();
	if ( num  <= 0  )
	{
		return -1;
	}
	if(g_DevList[0].strName[0]!='\0')
	{
		//g_DevList[0].mutex.EnterSynCode();
		
		lReturn = SCardConnect(g_sc,
			g_DevList[0].strName,
			SCARD_SHARE_SHARED,
			SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
			&hKey,
			&dwAP);
		if(SCARD_S_SUCCESS != lReturn)
		{
			g_DevList[0].bOpened=0;
			//g_DevList[0].mutex.LeaveSynCode();
			return -1;
		}
		
		g_DevList[0].hCard = (HANDLE)hKey;
		g_DevList[0].bOpened=1;
		g_DevList[0].dwActiveProtocol = dwAP;
		g_dwAp = dwAP;
		//g_DevList[0].mutex.LeaveSynCode();
		return 0;	
	}
	return -1;
}


// =====================================================================================================
// EXTERN_C int WINAPI API_ConnectDecardDev();
// ���ܣ�Ĭ�ϴ򿪵�һ�������豸�������豸index
// ��������
// ���أ�
//		һ����С��0��ֵ	��������,
//		-1	ʧ��
// ======================================================================================================
int API_ConnectDecardDev()
{
	LONG lReturn;
	SCARDHANDLE hKey;
	DWORD        dwAP;
	int num = -1;
	
	// Establish the context.
	num = API_Find();
	if ( num  <= 0  )
	{
		return -1;
	}
	if(g_DevList[0].strName[0]!='\0')
	{
		//g_DevList[0].mutex.EnterSynCode();
		
		lReturn = SCardConnect(g_sc,
			g_DevList[0].strName,
			SCARD_SHARE_SHARED,
			SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
			&hKey,
			&dwAP);
		if(SCARD_S_SUCCESS != lReturn)
		{
			g_DevList[0].bOpened=0;
			//g_DevList[0].mutex.LeaveSynCode();
			return -1;
		}
		
		g_DevList[0].hCard = (HANDLE)hKey;
		g_DevList[0].bOpened=1;
		g_DevList[0].dwActiveProtocol = dwAP;
		g_dwAp = dwAP;

#if 0
#define DECARD_DEV_VD10
#ifdef DECARD_DEV_X1 
		APDUEx apdu1, apdu2;

		apdu1.cla = 0xFF;
		apdu1.ins = 0x10;
		apdu1.p1 = 0x00;
		apdu1.p2 = 0x01;
		apdu1.lc = 0x00;
		apdu1.le = 0x00;

		lReturn = API_SendApdu(0, &apdu1);

		apdu2.cla = 0xFF;
		apdu2.ins = 0x62;
		apdu2.p1 = 0x00;
		apdu2.p2 = 0x00;
		apdu2.lc = 0x00;
		apdu2.le = 0x00;

		lReturn = API_SendApdu(0, &apdu2);

#else
#ifdef DECARD_DEV_VD10
		APDUEx apdu1, apdu2;

		apdu1.cla = 0xFF;
		apdu1.ins = 0x00;
		apdu1.p1 = 0x00;
		apdu1.p2 = 0x00;
		apdu1.lc = 0x02;
		apdu1.sendbuf[0]=0xFD;
		apdu1.sendbuf[1]=0x01;
		apdu1.le = 0x00;

		lReturn = API_SendApdu(0, &apdu1);

		apdu2.cla = 0xFF;
		apdu2.ins = 0x00;
		apdu2.p1 = 0x00;
		apdu2.p2 = 0x00;
		apdu2.lc = 0x02;
		apdu2.sendbuf[0]=0xFE;
		apdu2.sendbuf[1]=0x00;
		apdu2.le = 0x00;

		lReturn = API_SendApdu(0, &apdu2);
#else

#endif
#endif
#endif	
		//g_DevList[0].mutex.LeaveSynCode();
		return 0;	
	}
	return -1;
}
