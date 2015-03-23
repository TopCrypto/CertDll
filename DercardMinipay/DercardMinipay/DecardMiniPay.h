#ifndef _DECARDMINIPAY_H_
#define _DECARDMINIPAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************** 
** 函数名称:  GetTerminalStatus();  
** 功能描述:  获取迷你付终端设备状态
** 输入参数:  无
** 输出参数:  无               
** 返 回 值:  1 => 设备插入
              2 => 设备拔出
			  3 => 设备存在
			  4 => 设备不存在
			  其它值 => 未知错误
***************************************************************************/
_declspec(dllexport)	DWORD GetTerminalStatus(); 


/*************************************************************************** 
** 函数名称:  Init();  
** 功能描述:  初始化证书存储区数据，清空所有证书数据，
** 输入参数:  无
** 输出参数:  无               
** 返 回 值:  TRUE  => 成功
              FALSE => 失败
** 备注: 该命令会擦除包括终端、CA和Pin在内的所有的证书数据和公私密钥对，初次导入证书前要，要执行该命令，
**       清空所有与证书有关的数据，以便导入新的证书。
***************************************************************************/
_declspec(dllexport)	BOOL  Init();


/*************************************************************************** 
** 函数名称:  SetTerminalID(unsigned char *pInBuf, DWORD InLength);  
** 功能描述:  设置终端ID号
** 输入参数:  
**            pInBuf :  输入数据
**            InLength: 输入数据的长度
** 输出参数:  无               
** 返 回 值:  TRUE  => 成功
              FALSE => 失败
** 备注：    应为ID号正常为23字节，函数中限定了输入长度为23个字节,否则设置不成功
***************************************************************************/
_declspec(dllexport)	BOOL  SetTerminalID(unsigned char *pInBuf, DWORD InLength); 


/*************************************************************************** 
** 函数名称:  GetTerminalID(unsigned char *pOutBuf, DWORD * pOutLength);  
** 功能描述:  读取终端ID号
** 输入参数:  无
** 输出参数:  
**            pOutBuf :   输出数据
**            pOutLength: 输出数据长度
** 返 回 值:  TRUE  => 成功
              FALSE => 失败
***************************************************************************/
_declspec(dllexport)	BOOL  GetTerminalID(unsigned char *pOutBuf, DWORD * pOutLength); 



/*************************************************************************** 
** 函数名称:  AddCert(int CertType, BYTE *pInBuf, DWORD InLength);
** 功能描述:  终端添加证书数据
** 输入参数:  
**             CertType :   证书索引值
**                          0 => Terminal证书
**                          1 => CA证书索引
**                          2 => Pin证书索引
**                          其它值 => 非法索引

**             pInBuf : 需要写入的证书数据
**            InLength: 需要写入的证书数据长度
** 输出参数:  无               
** 返 回 值:  TRUE  => 成功
              FALSE => 失败
***************************************************************************/
_declspec(dllexport)	BOOL  AddCert(int CertType, BYTE *pInBuf, DWORD InLength);



/*************************************************************************** 
** 函数名称:  GenerateRSAKey(int RsaKeyIndex);
** 功能描述:  产生终端设备公私钥对
** 输入参数:  
**             RsaKeyIndex :   RSA索引
**                          1 => RSA 1024
**                          2 => RSA 2048
**                          其它值 => 非法索引
** 输出参数:  无               
** 返 回 值:  TRUE  => 成功
              FALSE => 失败
***************************************************************************/
_declspec(dllexport)	BOOL GenerateRSAKey(int RsaKeyIndex);


/*************************************************************************** 
** 函数名称:  ExportRSAKey(int CertType, int RsaKeyIndex, unsigned char *OutPubKey);
** 功能描述:  导出设备公钥
** 输入参数:  
**             CertType :   证书索引
**                          0 => 终端证书
**                          1 => CA证书
**                          2 => Pin证书
**                          其它值 => 非法索引
**
**             RsaKeyIndex :   RSA索引
**                          1 => RSA 1024
**                          2 => RSA 2048
** 输出参数:   
**            OutPubKey:   导出公钥数据
**                 
** 返 回 值:  TRUE  => 成功
              FALSE => 失败

***************************************************************************/
_declspec(dllexport)    BOOL ExportPubKeyRsa(int CertType, int RsaKeyIndex, unsigned char *OutPubKey);


/*************************************************************************** 
** 函数名称:  PriKeyRsaCompute(int RsaKeyIndex, unsigned char *pInBuf, DWORD InLength, unsigned char *pOutBuf, DWORD *OutLength);
** 功能描述:  终端私钥计算，用于签名数据
** 输入参数:               RsaKeyIndex :   证书索引值
**                          1 => RSA 1024
**                          2 => RSA 2048
**                        pInBuf:     输入数据
**                        InLength:   输入数据长度
** 输出参数:  
**            pOutBuf: 签名后的数据
**            OutLength: 签名后的数据长度
** 返 回 值:  TRUE  => 成功
              FALSE => 失败
***************************************************************************/   
_declspec(dllexport)    BOOL PriKeyRsaCompute(int RsaKeyIndex, unsigned char *pInBuf, DWORD InLength, unsigned char *pOutBuf, DWORD *OutLength);


/*************************************************************************** 
** 函数名称:  DigestData_SHA1(unsigned char *pInBuf, DWORD InLength, unsigned char Digest[20]);
** 功能描述:  计算数据的SHA1摘要值
** 输入参数:  
**            pInBuf: 输入数据
**            InLength:输入数据长度
** 输出参数:   
              Digest: 摘要值
** 返 回 值:  无
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











