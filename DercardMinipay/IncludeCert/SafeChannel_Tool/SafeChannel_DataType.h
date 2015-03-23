#ifndef _SAFE_CHANNEL_DATA_TYPE_H_
	#define _SAFE_CHANNEL_DATA_TYPE_H_

	#include "SafeChannel_ParaDef.h"
	//#include "Algorithm_Interface.h"
	//---------------------------------------------------------------------- 
	// 数据类型标示说明	   
	// 宏描述													说明
	#ifndef SCN_RETURN
		typedef long SCN_RETURN;								// 函数返回值
	#endif 
	
	#ifndef SCN_UCHAR
		typedef unsigned char SCN_UCHAR, *SCN_PUCHAR;			// unsigned char
	#endif 
	
	#ifndef SCN_VOID
		typedef void SCN_VOID;									// void
	#endif 
	
	#ifndef SCN_ULONG
		typedef unsigned long SCN_ULONG, *SCN_PULONG;			// unsigned long
	#endif
	
	#ifndef SCN_BOOL
		typedef unsigned long SCN_BOOL;							// BOOL
	#endif
	
	#ifndef FALSE
		#define FALSE			0								// FALSE
	#endif
	#ifndef TRUE
		#define TRUE			1								// TRUE
	#endif
	
	#ifndef ASSERT
		#define ASSERT(a)		(a)								// ASSERT
	#endif
	//---------------------------------------------------------------------- 
	// 输入输出参数标示说明	   
	// 宏描述										说明
	#define _SCN_IN_								// 输入参数
	#define _SCN_OUT_								// 输出参数
	#define _SCN_IN_OUT_							// 输入输出参数


	//////////////////////////////////////////////////////////////////////////
	//
	typedef struct{
		// 数据体
		DWORD nDataOffset;
		DWORD nDataLength;
		
	    DWORD nValueOffset;
	    DWORD nValueLength;

	}item_t;

	/* X509 certificate's structure. */
	typedef struct _x509_cert {
	    unsigned char *pDer;     /* the certificate's content */
	    DWORD nDerLength;
		
		// all 
		item_t tbs;             /* tbs offset */
		item_t signature;       /* algorithm of certificate's signature */
		item_t signatureValue;  /* Value of signature */
		
		//// tbs info
		item_t version;         /* Certificate's version */
		item_t serialNumber;    /* Serial number */
		item_t tbssignature;    /* algorithm of certificate's signature */
		item_t issuerName;      /* issuer information */
	    item_t notBefore;       /* Not before */
		item_t notAfter;        /* Not after */
		item_t subjectName;     /* subject information */
		item_t publicKey;       /* certificate's public key */
		//// tbs extension info
		item_t extension;       /* extension's of certitficate */
	    item_t pkOid;           /* PKCS# */
	    item_t issuerUniqueID;  /* issuer's unique information */
	    item_t subjectUniqueID; /* subject's unique information */
	} x509_cert_t;

	//////////////////////////////////////////////////////////////////////////
	// 
	typedef enum
	{
		eSafeChannelAlgorithm_ASYMKEY_ALG_ECC = 0, 
		eSafeChannelAlgorithm_ASYMKEY_ALG_RSA = 1, 
		eSafeChannelAlgorithm_ASYMKEY_ALG_RSA_2048 = 2,
	} eSafeChannelAlgorithm_ASYMKEY_ALG;

	typedef enum
	{
		eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil = 0, 
		eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT = 1, 
		eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin = 2, 
		eSafeChannelAlgorithm_ASYMKEY_INDEX_Server = 3
	} eSafeChannelAlgorithm_ASYMKEY_INDEX;

	typedef enum
	{
		eSafeChannelAlgorithm_SYMKEY_ALG_TDES = 0, 
		eSafeChannelAlgorithm_SYMKEY_ALG_SM4 = 1
	} eSafeChannelAlgorithm_SYMKEY_ALG;

	typedef enum
	{
		eSafeChannelAlgorithm_DIGESHASH_ALG_SHA1 = 0, 
		eSafeChannelAlgorithm_DIGESHASH_ALG_SM3 = 1
	} eSafeChannelAlgorithm_DIGESHASH_ALG;

	typedef enum
	{
		eSafeChannelAlgorithm_OBJECT_TYPE_Cert = 0, 
		eSafeChannelAlgorithm_OBJECT_TYPE_PubKeyFile = 1, 
		eSafeChannelAlgorithm_OBJECT_TYPE_PriKeyFile = 2
	} eSafeChannelAlgorithm_OBJECT_TYPE;


	typedef enum
	{
		eSafeChannelAlgorithm_SESSION_STATUS_PowerOn = 0,
		eSafeChannelAlgorithm_SESSION_STATUS_Trade = 1,
		eSafeChannelAlgorithm_SESSION_STATUS_Manager = 2
	}eSafeChannelAlgorithm_SESSION_STATUS;

// 	typedef union __tagSafeChannelDigestCtx{
// 		SM3Ctx		m_Sm3;
// 		SHA1Ctx		m_Sha1;
// 		SHA256Ctx	m_Sha256;
// 	}uSafeChannelDigestCtx;
	
	typedef union __tagSafeChannelChildStuVersion{
		SCN_ULONG	m_ulVerValue;
		SCN_UCHAR	m_pVerStr[4];
	}uSafeChannelChildStuVersion;

	typedef struct __tagSafeChannelChildStuRsaPublicKeyBlob{
		SCN_ULONG	m_ulKeyBits;								// length in bits of modulus
		SCN_UCHAR	m_pModulus[MAX_RSA_MODULUS_LEN];			// mode N
		SCN_UCHAR	m_pExponent[MAX_RSA_MODULUS_LEN];			// mode E;
	}stuSafeChannelChildStuRsaPublicKeyBlob;

	typedef struct __tagSafeChannelChildStuRsaPrivateKeyBlob{
		SCN_ULONG	m_ulKeyBits;								// length in bits of modulus
		SCN_UCHAR	m_pModulus[MAX_RSA_MODULUS_LEN];			// mode N
		SCN_UCHAR	m_pPublicExponent[MAX_RSA_MODULUS_LEN];		// mode E
		SCN_UCHAR	m_pPrivateExponent[MAX_RSA_MODULUS_LEN];	// mode D
			
		SCN_UCHAR	m_prime[2][MAX_RSA_PRIME_LEN];   			// mode p q;prime factors
		SCN_UCHAR	m_primeExponent[2][MAX_RSA_PRIME_LEN];		// mode dp dq;exponents for CRT
		SCN_UCHAR	m_pCoefficient[MAX_RSA_PRIME_LEN];			// mode qinv;CRT coefficient
	}stuSafeChannelChildStuRsaPrivateKeyBlob;
	
	typedef struct __tagSafeChannelChildStuSm2PublicKeyBlob{
		SCN_ULONG	m_ulKeyBits;						// 256 bits
		SCN_UCHAR	m_pX[MAX_RSA_MODULUS_LEN];			// mode X
		SCN_UCHAR	m_pY[MAX_RSA_MODULUS_LEN];			// mode Y
	}stuSafeChannelChildStuSm2PublicKeyBlob;

	typedef struct __tagSafeChannelChildStuSm2PrivateKeyBlob{
		SCN_ULONG	m_ulKeyBits;						// 256 bits
		SCN_UCHAR	m_pD[MAX_RSA_MODULUS_LEN];			// mode D
	}stuSafeChannelChildStuSm2PrivateKeyBlob;
	
	typedef struct __tagSafeChannelChildStuAppCertInfo{
		SCN_UCHAR	m_pCerLen[2];								// 证书长度，高位在前，低位在后
		SCN_UCHAR	m_ucCertFlag;								// 证书存在状态
		SCN_UCHAR	m_pContainerName[CONTAINER_BUFF_SIZE];		// 容器名称以'\0'结尾
	}stuSafeChannelChildStuAppCertInfo;

	typedef struct __tagSafeChannelChildStuAppData{
		SCN_UCHAR	m_ucDataLength;								// 数据长度
		SCN_UCHAR	m_pData[CONTAINER_BUFF_SIZE];				// 数据
	}stuSafeChannelChildStuAppData;
	
	typedef struct _tagSafeChannelFlashData{
		// MF   --------------------------------

		
		// -版本
		// --当前结构体版本
		uSafeChannelChildStuVersion	m_uVer;
		
		// -算法说明
		// --算法标识
		SCN_UCHAR	m_pAlgorithmType[2];

		
		// ADF 1--------------------------------
		
		// -证书
		// --终端证书
		SCN_UCHAR	m_pTermilCert[CERT_BUFF_SIZE];
		// --CA证书
		SCN_UCHAR	m_pCARootCert[CERT_BUFF_SIZE];
		// --Pin加密证书
		SCN_UCHAR	m_pPinEncCert[CERT_BUFF_SIZE];
		// -对称密钥
		// --会话加密密钥
		SCN_UCHAR	m_pSessionEncKey[16];
		// --会话Mac密钥
		SCN_UCHAR	m_pSessionMacKey[16];
		
		// -非对称密钥 RSA
		// --终端公钥
		stuSafeChannelChildStuRsaPublicKeyBlob	m_stuTermilPubKey;
		// --根公钥
		stuSafeChannelChildStuRsaPublicKeyBlob	m_stuCARootPubKey;
		// --Pin加密公钥
		stuSafeChannelChildStuRsaPublicKeyBlob	m_stuPinEncPubKey;
		// --服务器公钥
		stuSafeChannelChildStuRsaPublicKeyBlob	m_stuServerPubKey;
		// --终端私钥
		stuSafeChannelChildStuRsaPrivateKeyBlob	m_stuTermilPriKey;
		
		// -非对称密钥 SM2
		// --终端公钥
		stuSafeChannelChildStuSm2PublicKeyBlob	m_stuTermilPubKeySm2;
		// --根公钥
		stuSafeChannelChildStuSm2PublicKeyBlob	m_stuCARootPubKeySm2;
		// --Pin加密公钥
		stuSafeChannelChildStuSm2PublicKeyBlob	m_stuPinEncPubKeySm2;
		// --服务器公钥
		stuSafeChannelChildStuSm2PublicKeyBlob	m_stuServerPubKeySm2;
		// --终端私钥
		stuSafeChannelChildStuSm2PublicKeyBlob	m_stuTermilPriKeySm2;
		
		// -应用数据
		// --终端证书标记信息
		stuSafeChannelChildStuAppCertInfo	m_stuTermilCertInfo;
		// --CA证书标记信息
		stuSafeChannelChildStuAppCertInfo	m_stuCARootCertInfo;
		// --Pin加密证书标记信息
		stuSafeChannelChildStuAppCertInfo	m_stuPinEncCertInfo;
		// --PIN证书序列号
		stuSafeChannelChildStuAppData	m_stuPinEncCertSerialNumber;
		// --CA证书序列号
		stuSafeChannelChildStuAppData	m_stuCARootCertSerialNumber;
		// --终端证书有效期
		stuSafeChannelChildStuAppData	m_stuTermilCertValidDate;
		
		// --
	
	}stuSafeChannelFlashData;

	//((stuSafeChannelFlashData*)0)->m_pTermilCert;

	
	//////////////////////////////////////////////////////////////////////////
	//
	
	typedef struct  __tagSafeChannelSessionCreate_HelloMessage
	{							   
		SCN_UCHAR	m_pCipherSuite[MAX_CIPHERSUITE_SZIE];
		SCN_UCHAR	m_pRandom[MAX_RANDOM_SZIE];
	}stuSafeChannelSessionCreate_HelloMessage;
	
	typedef struct  __tagSafeChannelSessionCreate_Certificate
	{
		SCN_UCHAR	m_pCertificate[MAX_CERTIFICATE_SIZE];
	}stuSafeChannelSessionCreate_Certificate;

	typedef struct __tagSafeChannelSessionCreate_ServerCertificateData{
		SCN_UCHAR	m_pCertificateSubject[MAX_CERTIFICATE_SIZE];		// 主体部分数据
		SCN_ULONG	m_ulCertificateSubjectLength;						// 主体部分数据长度
		
		SCN_UCHAR	m_pCertificateSignature[512];						// 签名部分数据
		SCN_ULONG	m_ulCertificateSignatureLength;						// 签名部分数据长度
		
		stuSafeChannelChildStuRsaPublicKeyBlob	m_stuServerPubKey;		// 服务器公钥 Rsa
		stuSafeChannelChildStuSm2PublicKeyBlob	m_stuServerPubKeySm2;	// 服务器公钥 sm2

	}stuSafeChannelSessionCreate_ServerCertificateData;
	// 过程数据
	typedef struct __tagSafeChannelSessionCreateData{
		stuSafeChannelSessionCreate_HelloMessage			m_stuClientHello;			// R1 = A1 | r1
		stuSafeChannelSessionCreate_HelloMessage			m_stuServerHello;			// R2 = A2 | r2

		// 服务器证书分解数据，用于HashServerCertificate和VerifyServerCertificate，导入服务器公钥，获取服务器OU判别，获取服务器主体的Hash
		stuSafeChannelSessionCreate_ServerCertificateData	*m_pServerCertificateData;	
		// H2 = Hash(ServerCertificate)
		SCN_UCHAR											m_pServerCertificateSubjectHash[MAX_HMAC_SIZE];

		// ClientCert
		stuSafeChannelSessionCreate_Certificate				*m_pClientCertificateData;	
		// H3 = Hash(ClientCertificate)
		SCN_UCHAR											m_pClientCertificateHash[MAX_HMAC_SIZE];
		// R3 = ClientHello||ServerHello --> S1 = RsaSign(Hash(R3))
		SCN_UCHAR											m_pCertificateVerify[MAX_SIGNATURE_SIZE];
		// M1 = MasterSecret --> E1 = RsaEnc(M1)
		SCN_UCHAR											m_pClientKeyExchange[MAX_SIGNATURE_SIZE];
		// T1 = R1 | R2 | H2 | H3 | S1 | E1 --> H4 = Hash(T1)
		SCN_UCHAR											m_pHandshakeMessagesHash[MAX_HMAC_SIZE];
		// master_secret
		SCN_UCHAR											m_pMasterSecret[MAX_SHARESECRET_SZIE];

		// HMAC(master_secret, Finish_label || Hash(handshake_messages))

		// D1 = "SERVER" | H4 -- > F1 = ServerHmac(master_secret, D1)
		SCN_UCHAR											m_pServerFinished[MAX_HMAC_SIZE];

		// D2 = "CLIENT" | H4 -- > F2 = ClientHmac(master_secret, D2)
		SCN_UCHAR											m_pClientFinished[MAX_HMAC_SIZE];
		// X = HMAC(M1, "KEY" | r1 | r2)
		SCN_UCHAR											m_pSessionKeyFinished[MAX_HMAC_SIZE];
	}stuSafeChannelSessionCreateData;
	
	typedef struct __tagSafeChannelSessionData{
		// 当前会话状态
		eSafeChannelAlgorithm_SESSION_STATUS	m_eSessionStatus;
		// 初始化状态标记
		SCN_BOOL		m_bInitFlag;
		// 允许后续指令标记
		SCN_BOOL		m_bServiceCertCheckFlag;
		// hmac校验完成允许生成  Finished
		SCN_BOOL		m_bHmacServiceCheckOK;

		SCN_ULONG		m_ulDigestLength;
		SCN_ULONG		m_ulRsaModulusLength;
		SCN_ULONG		m_ulResponsLength;
		
		SCN_UCHAR		m_pEncryptCmdData[512];
		SCN_ULONG		m_ulEncryptCmdDataLength;

		// 会话密钥
		SCN_UCHAR		m_pSessionEncKey[16];
		SCN_UCHAR		m_pSessionMacKey[16];
		// 计数器
		SCN_UCHAR		m_pSessionSeq[16];
		// 重试次数
		SCN_UCHAR		m_ucRetryChance;
		// 过程数据
		stuSafeChannelSessionCreateData		*m_pSessionCreateDataTemp;
		
	}stuSafeChannelSessionData;
#endif
