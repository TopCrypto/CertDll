#ifndef _SAFE_CHANNEL_SAFE_CHIP_H_
	#define _SAFE_CHANNEL_SAFE_CHIP_H_



	//////////////////////////////////////////////////////////////////////////
	// 文件结构定义

	#define NULL_FILE_ID									0x0000
	#define MF_FILE_ID										0x3F00

	#define ADF1_FILE_ID					0x5000
	// ADF1下安全文件
	#define ADF1_SF_FILE_ID					0x5001
	//ADF1终端证书
	#define ADF1_TermilCertFile_FILE_ID		0x5005
	// ADF1下RSA加密公钥文件 500C
	#define ADF1_RSAEncPubFile_FILE_ID		0x500C
	// ADF1下RSA加密私钥文件 500B
	#define ADF1_RSAEncPriFile_FILE_ID		0x500B
	// ADF1下CA根证书
	#define ADF1_CARootCertFile_FILE_ID		0x5025
	// ADF1下CA Root 公钥文件 502C
	#define ADF1_CARootPubFile_FILE_ID		0x502C
	// ADF1下Pin加密公钥文件 503C
	#define ADF1_PinEncPubFile_FILE_ID		0x503C
	// ADF1下缓存数据
	#define ADF1_TempFile_FILE_ID			0x5035

	#define ADF1_ContainerFix_FILE_ID		0x5002
	#define ADF1_ContainerFix_FILE_ID_CA	0x5012
	#define ADF1_ContainerFix_FILE_ID_PIN	0x5022
	#define ADF1_FileFix_FILE_ID			0x5003
	#define ADF1_PairsFix_FILE_ID			0x5004


	#define ADF2_FILE_ID					0x6000	
	// ADF2下安全文件
	#define ADF2_SF_FILE_ID					0x6001
	// ADF2终端证书
	#define ADF2_TermilCertFile_FILE_ID		0x6005
	// ADF2下ECC加密公钥文件 
	#define ADF2_ECCEncPubFile_FILE_ID		0x600C
	// ADF2下RSA加密私钥文件
	#define ADF2_ECCEncPriFile_FILE_ID		0x600B
	// ADF2下CA根证书
	#define ADF2_CARootCertFile_FILE_ID		0x6025
	// ADF2下CA Root 公钥文件
	#define ADF2_CARootPubFile_FILE_ID		0x602C
	// ADF2下Pin加密公钥文件
	#define ADF2_PinEncPubFile_FILE_ID		0x603C
	// ADF2下缓存数据
	#define ADF2_TempFile_FILE_ID			0x6035

	#define ADF2_ContainerFix_FILE_ID		0x6002
	#define ADF2_ContainerFix_FILE_ID_CA	0x6012
	#define ADF2_ContainerFix_FILE_ID_PIN	0x6022
	#define ADF2_FileFix_FILE_ID			0x6003
	#define ADF2_PairsFix_FILE_ID			0x6004


	#define ADF3_FILE_ID					ADF1_FILE_ID

	// ADF3下安全文件
	#define ADF3_SF_FILE_ID 				ADF1_SF_FILE_ID
	// ADF3终端证书
	#define ADF3_TermilCertFile_FILE_ID 	ADF1_TermilCertFile_FILE_ID
	// ADF3下RSA加密公钥文件 
	#define ADF3_RSAEncPubFile_FILE_ID		ADF1_RSAEncPubFile_FILE_ID
	// ADF3下RSA加密私钥文件 
	#define ADF3_RSAEncPriFile_FILE_ID		ADF1_RSAEncPriFile_FILE_ID
	// ADF3下CA根证书
	#define ADF3_CARootCertFile_FILE_ID 	ADF1_CARootCertFile_FILE_ID
	// ADF3下CA Root 公钥文件 
	#define ADF3_CARootPubFile_FILE_ID		ADF1_CARootPubFile_FILE_ID
	// ADF3下Pin加密公钥文件 
	#define ADF3_PinEncPubFile_FILE_ID		ADF1_PinEncPubFile_FILE_ID
	// ADF3下缓存数据
	#define ADF3_TempFile_FILE_ID			ADF1_TempFile_FILE_ID

	#define ADF3_ContainerFix_FILE_ID		ADF1_ContainerFix_FILE_ID
	#define ADF3_ContainerFix_FILE_ID_CA	ADF1_ContainerFix_FILE_ID_CA
	#define ADF3_ContainerFix_FILE_ID_PIN	ADF1_ContainerFix_FILE_ID_PIN
	#define ADF3_FileFix_FILE_ID			ADF1_FileFix_FILE_ID
	#define ADF3_PairsFix_FILE_ID			ADF1_PairsFix_FILE_ID



	#define MAX_CERTFICATIFICATE_LENGTH		0x0802


#endif
