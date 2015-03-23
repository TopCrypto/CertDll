#ifndef _DECARDMINIPAY_H_
#define _DECARDMINIPAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************** 
** ��������:  GetTerminalStatus();  
** ��������:  ��ȡ���㸶�ն��豸״̬
** �������:  ��
** �������:  ��               
** �� �� ֵ:  1 => �豸����
              2 => �豸�γ�
			  3 => �豸����
			  4 => �豸������
			  ����ֵ => δ֪����
***************************************************************************/
_declspec(dllexport)	DWORD GetTerminalStatus(); 


/*************************************************************************** 
** ��������:  Init();  
** ��������:  ��ʼ��֤��洢�����ݣ��������֤�����ݣ�
** �������:  ��
** �������:  ��               
** �� �� ֵ:  TRUE  => �ɹ�
              FALSE => ʧ��
** ��ע: ���������������նˡ�CA��Pin���ڵ����е�֤�����ݺ͹�˽��Կ�ԣ����ε���֤��ǰҪ��Ҫִ�и����
**       ���������֤���йص����ݣ��Ա㵼���µ�֤�顣
***************************************************************************/
_declspec(dllexport)	BOOL  Init();


/*************************************************************************** 
** ��������:  SetTerminalID(unsigned char *pInBuf, DWORD InLength);  
** ��������:  �����ն�ID��
** �������:  
**            pInBuf :  ��������
**            InLength: �������ݵĳ���
** �������:  ��               
** �� �� ֵ:  TRUE  => �ɹ�
              FALSE => ʧ��
** ��ע��    ӦΪID������Ϊ23�ֽڣ��������޶������볤��Ϊ23���ֽ�,�������ò��ɹ�
***************************************************************************/
_declspec(dllexport)	BOOL  SetTerminalID(unsigned char *pInBuf, DWORD InLength); 


/*************************************************************************** 
** ��������:  GetTerminalID(unsigned char *pOutBuf, DWORD * pOutLength);  
** ��������:  ��ȡ�ն�ID��
** �������:  ��
** �������:  
**            pOutBuf :   �������
**            pOutLength: ������ݳ���
** �� �� ֵ:  TRUE  => �ɹ�
              FALSE => ʧ��
***************************************************************************/
_declspec(dllexport)	BOOL  GetTerminalID(unsigned char *pOutBuf, DWORD * pOutLength); 



/*************************************************************************** 
** ��������:  AddCert(int CertType, BYTE *pInBuf, DWORD InLength);
** ��������:  �ն����֤������
** �������:  
**             CertType :   ֤������ֵ
**                          0 => Terminal֤��
**                          1 => CA֤������
**                          2 => Pin֤������
**                          ����ֵ => �Ƿ�����

**             pInBuf : ��Ҫд���֤������
**            InLength: ��Ҫд���֤�����ݳ���
** �������:  ��               
** �� �� ֵ:  TRUE  => �ɹ�
              FALSE => ʧ��
***************************************************************************/
_declspec(dllexport)	BOOL  AddCert(int CertType, BYTE *pInBuf, DWORD InLength);



/*************************************************************************** 
** ��������:  GenerateRSAKey(int RsaKeyIndex);
** ��������:  �����ն��豸��˽Կ��
** �������:  
**             RsaKeyIndex :   RSA����
**                          1 => RSA 1024
**                          2 => RSA 2048
**                          ����ֵ => �Ƿ�����
** �������:  ��               
** �� �� ֵ:  TRUE  => �ɹ�
              FALSE => ʧ��
***************************************************************************/
_declspec(dllexport)	BOOL GenerateRSAKey(int RsaKeyIndex);


/*************************************************************************** 
** ��������:  ExportRSAKey(int CertType, int RsaKeyIndex, unsigned char *OutPubKey);
** ��������:  �����豸��Կ
** �������:  
**             CertType :   ֤������
**                          0 => �ն�֤��
**                          1 => CA֤��
**                          2 => Pin֤��
**                          ����ֵ => �Ƿ�����
**
**             RsaKeyIndex :   RSA����
**                          1 => RSA 1024
**                          2 => RSA 2048
** �������:   
**            OutPubKey:   ������Կ����
**                 
** �� �� ֵ:  TRUE  => �ɹ�
              FALSE => ʧ��

***************************************************************************/
_declspec(dllexport)    BOOL ExportPubKeyRsa(int CertType, int RsaKeyIndex, unsigned char *OutPubKey);


/*************************************************************************** 
** ��������:  PriKeyRsaCompute(int RsaKeyIndex, unsigned char *pInBuf, DWORD InLength, unsigned char *pOutBuf, DWORD *OutLength);
** ��������:  �ն�˽Կ���㣬����ǩ������
** �������:               RsaKeyIndex :   ֤������ֵ
**                          1 => RSA 1024
**                          2 => RSA 2048
**                        pInBuf:     ��������
**                        InLength:   �������ݳ���
** �������:  
**            pOutBuf: ǩ���������
**            OutLength: ǩ��������ݳ���
** �� �� ֵ:  TRUE  => �ɹ�
              FALSE => ʧ��
***************************************************************************/   
_declspec(dllexport)    BOOL PriKeyRsaCompute(int RsaKeyIndex, unsigned char *pInBuf, DWORD InLength, unsigned char *pOutBuf, DWORD *OutLength);


/*************************************************************************** 
** ��������:  DigestData_SHA1(unsigned char *pInBuf, DWORD InLength, unsigned char Digest[20]);
** ��������:  �������ݵ�SHA1ժҪֵ
** �������:  
**            pInBuf: ��������
**            InLength:�������ݳ���
** �������:   
              Digest: ժҪֵ
** �� �� ֵ:  ��
***************************************************************************/
_declspec(dllexport)   void DigestData_SHA1(unsigned char *pInBuf, DWORD InLength, unsigned char Digest[20]);



#ifdef __cplusplus
}
#endif
#endif

/*******************************************************************************
**                            End Of File                                     **
**          (C) Copyright ShenZhen Decard SmartCard Tech co.,LTD              **
*******************************************************************************/











