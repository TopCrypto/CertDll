#ifndef _SAFE_CHANNEL_PARA_DEF_H_
	#define _SAFE_CHANNEL_PARA_DEF_H_

	//////////////////////////////////////////////////////////////////////////


	#ifndef BULK_BUFF_SIZE
		#define BULK_BUFF_SIZE 				288
	#endif
	
	#ifndef CERT_BUFF_SIZE
		#define CERT_BUFF_SIZE 				(3*1024)
	#endif
	
	#ifndef CONTAINER_BUFF_SIZE
		#define CONTAINER_BUFF_SIZE 		(64)
	#endif


	#ifndef MAX_PIN_BUFF_SIZE
		#define MAX_PIN_BUFF_SIZE 			(16)
	#endif


	#define USE_ECC_BITS					(256)
	#if (USE_ECC_BITS == (256))
		#ifndef MAX_ECC_MODULUS_BITS
			#define MAX_ECC_MODULUS_BITS	USE_ECC_BITS
		#endif

		#ifndef MAX_ECC_MODULUS_LEN
			#define MAX_ECC_MODULUS_LEN		((USE_ECC_BITS + 7) / 8)
		#endif
	#endif

	#define USE_RSA_BITS					(2048)
	//#define USE_RSA_BITS					(2048)
	#ifdef USE_RSA_BITS

		#ifndef MAX_RSA_MODULUS_BITS
			#define MAX_RSA_MODULUS_BITS	USE_RSA_BITS
		#endif

		#ifndef MAX_RSA_MODULUS_LEN
			#define MAX_RSA_MODULUS_LEN		((MAX_RSA_MODULUS_BITS + 7) / 8)
		#endif

		#ifndef MAX_RSA_PRIME_BITS
			#define MAX_RSA_PRIME_BITS		((MAX_RSA_MODULUS_BITS + 1) / 2)
		#endif

		#ifndef MAX_RSA_PRIME_LEN
			#define MAX_RSA_PRIME_LEN		((MAX_RSA_PRIME_BITS + 7) / 8)
		#endif
	#endif

	
	//////////////////////////////////////////////////////////////////////////
	// 会话
	#define MAX_CERTIFICATE_SIZE	2048					// 证我书数据存储空间大小
	#define MAX_SIGNATURE_SIZE		MAX_RSA_MODULUS_LEN		// 签名数据存储空间大小
	#define MAX_RANDOM_SZIE			32						// 加密通道交互随机数数据存储空间大小
	#define MAX_CIPHERSUITE_SZIE	1						// 加密通道交互算法标示数据存储空间大小
	#define MAX_SHARESECRET_SZIE	16						// 加密通道共享随机数数据存储空间大小
	#define MAX_HMAC_SIZE			0x20					// HMAC算法的输出数据存储空间大小
	#define MAX_RSA_SIZE			MAX_RSA_MODULUS_LEN		// 当前RSA算法数据存储空间大小

	#define MAX_RETRY_COUNT			3						// 安全会话最大错误尝试次数		
	//////////////////////////////////////////////////////////////////////////
	// 标记

	#define SCI_CERT_NONE					0x00

	#define SCI_CON_EXIST             0x80  // 容器已建立标识
	#define SCI_CON_SM2_OK		  	  0x40  // 容器中为SM2算法,
	#define SCI_CON_RSA2048_OK        0x20  // 容器中为RSA2048算法
	#define SCI_CON_RSA1024_OK        0x10  // 容器中为RSA1024算法
	#define SCI_CON_ENC_CERT_OK       0x08  // 容器中加密证书存在标识
	#define SCI_CON_SIGN_CERT_OK      0x04  // 容器中签名证书存在标识
	#define SCI_CON_KEY_SIGN_OK       0x02  // 容器中加密密钥对存在标识
	#define SCI_CON_KEY_ENC_OK        0x01  // 容器中签名密钥对存在标识

	#define SCI_CLA_OFFSET					0
	#define SCI_INS_OFFSET					1
	#define SCI_P1_OFFSET					2
	#define SCI_P2_OFFSET					3
	#define SCI_LC_OFFSET					4
	#define SCI_LE_OFFSET					4
	#define SCI_DATA_OFFSET					5

	#define SCI_INS_ADD_CERTIFICATE             0x20
	#define SCI_INS_UPDATE_CERTIFICATE          0x21  
	#define SCI_INS_DELETE_CERTIFICATE          0x22 
	#define SCI_INS_READ_CERTIFICATE            0x23 
	#define SCI_INS_GET_CERTRESPONSE			0x24 
	#define SCI_INS_GET_CLIENT_HELLO            0x25 
	#define SCI_INS_HASH_SERVER_CERTIFICATE     0x26 
	#define SCI_INS_VERIFY_SERVER_CERTIFICATE   0x27 
	#define SCI_INS_CLIENT_SIGN                 0x28 
	#define SCI_INS_EXPORT_MASTERKEY            0x29 
	#define SCI_INS_HMAC                        0x2a 
	#define SCI_INS_TRANSMIT_ENCRYPTED_COMMAND  0x2b 
	#define SCI_INS_RESET_SECURE_CHANNEL        0x2c 
	#define SCI_INS_READ_NON_PBOC_PIN           0x2d 


	// CipherSuite 标示
	#define SCI_CIPHER_RSA                		(1<<0)
	#define SCI_CIPHER_ECC                		(1<<1)
	#define SCI_CIPHER_3DES               	 	(1<<4)
	#define SCI_CIPHER_SM4                		(1<<5)

	#define SCI_SERVER_CERTFLAG_START			0x80
	#define SCI_SERVER_CERTFLAG_END				0x40
	#define SCI_SERVER_CERTFLAG_OU				0x20
	#define SCI_SERVER_CERTFLAG_PUBLICKEY		0x10

	#define SCI_SERVER_CERTSIGNFLAG_START		0x80
	#define SCI_SERVER_CERTSIGNFLAG_END			0x40


	#define SCI_CERTIFICATE_TERMIL				(0)
	#define SCI_CERTIFICATE_CA1					(1<<4)
	#define SCI_CERTIFICATE_CA2					(1<<5)
	#define SCI_CERTIFICATE_PIN					(1<<6)
	#define SCI_CERTIFICATE_RFU					(1<<7)


	//////////////////////////////////////////////////////////////////////////
	// 
	//////////////////////////////////////////////////////////////////////////
	// RETURN VALUE 
	#define SCIR_SUCCESS					AIR_SUCCESS					// 成功
	#define SCIR_FAIL						AIR_FAIL					// 失败
	#define SCIR_UNKNOWN_ERROR				AIR_UNKNOWN_ERROR			// 未知错误

	#define SCIR_NOT_INITIALIZE_ERROR		AIR_NOT_INITIALIZE_ERROR	// 未初始化
	#define SCIR_OBJECT_ERROR				AIR_OBJECT_ERROR			// 对象错误

	#define SCIR_IN_DATA_LENGTH_ERROR		AIR_IN_DATA_LENGTH_ERROR	// 输入数据长度错误
	#define SCIR_IN_DATA_ERROR				AIR_IN_DATA_ERROR			// 输入数据错误

	#define SCIR_HASH_OBJECT_ERROR			AIR_HASH_OBJECT_ERROR		// Hash 对象错误
	#define SCIR_HASH_NOT_EQUAL_ERROR		AIR_HASH_NOT_EQUAL_ERROR	// Hash 值不相等
	#define SCIR_PARAMETER_ERROR			AIR_PARAMETER_ERROR			// 无效的参数

	#define SCIR_NO_ROOM_ERROR				AIR_NO_ROOM_ERROR			// 空间不足
	#define SCIR_NO_ACCESS					(AIR_NO_ROOM_ERROR + 1)		// 权限不足

	#define SCIR_NO_OPT_FLASH_ERROR 		(AIR_NO_ROOM_ERROR + 2) 	// EEP 操作错误
	#define SCIR_OBJECT_NOT_EXIST			(AIR_NO_ROOM_ERROR + 3)		// 对象数据不存在

	//////////////////////////////////////////////////////////////////////////
	// 终端命令响应状态码列表
	
	#define SW_RESP_61FF 0x61FF	// 正常处理																	
	#define SW_RESP_6200 0x6200 // 无信息提供																	
	#define SW_RESP_6281 0x6281	// 回送的数据可能有错																	
	#define SW_RESP_6282 0x6282	// 文件长度<Le																	
	#define SW_RESP_6283 0x6283	// 选择的文件无效
	#define SW_RESP_6284 0x6284	// FCI 格式与P2 指定的不符																	
	#define SW_RESP_6300 0x6300	// 认证失败
	#define SW_RESP_63CF 0x63CF	// 认证失败，还剩下X 次尝试													
	#define SW_RESP_6400 0x6400	// 状态标志位未变																	
	#define SW_RESP_6581 0x6581	// 内存错误	
	#define SW_RESP_6700 0x6700	// 长度错误	
	#define SW_RESP_6882 0x6882	// 不支持安全报文																	
	#define SW_RESP_6900 0x6900	// 不能处理	
	#define SW_RESP_6901 0x6901	// 命令不接受（无效状态）																	
	#define SW_RESP_6981 0x6981	// 命令与文件结构不相容																	
	#define SW_RESP_6982 0x6982	// 不满足安全状态
	#define SW_RESP_6983 0x6983	// 验证方法锁定
	#define SW_RESP_6984 0x6984	// 引用数据无效																	
	#define SW_RESP_6985 0x6985	// 使用条件不满足
	#define SW_RESP_6986 0x6986	// 不满足命令执行的条件(非当前EF)																	
	#define SW_RESP_6987 0x6987	// 安全报文数据项丢失
	#define SW_RESP_6988 0x6988	// 安全信息数据对象不正确																	
	#define SW_RESP_6A80 0x6A80	//	数据域参数不正确																	
	#define SW_RESP_6A81 0x6A81	//	功能不支持
	#define SW_RESP_6A82 0x6A82	//	未找到文件																	
	#define SW_RESP_6A83 0x6A83	//	未找到记录																	
	#define SW_RESP_6A84 0x6A84	//	文件中存储空间不够																	
	#define SW_RESP_6A86 0x6A86	//	P1、P2 参数不正确
	#define SW_RESP_6A88 0x6A88	//	引用数据找不到		
	#define SW_RESP_6A89 0x6A89	//	文件已存在或者文件名冲突																	
	#define SW_RESP_6B00 0x6B00	//	参数错误(偏移地址超出了EF)																	
	#define SW_RESP_6CFF 0x6CFF	//	长度错误(Le 错误；'XX'为实际长度)																	
	#define SW_RESP_6F00 0x6F00	//	数据无效
	#define SW_RESP_6F01 0x6F01	//	未输入PIN
	#define SW_RESP_9000 0x9000	//	成功执行，无错误
	#define SW_RESP_9301 0x9301	//	金额不足																	
	#define SW_RESP_9302 0x9302	//	MAC 无效																	
	#define SW_RESP_9303 0x9303	//	应用永久锁住
	#define SW_RESP_9401 0x9401	//	金额不足
	#define SW_RESP_9402 0x9402	//	交易计数器到达最大值																	
	#define SW_RESP_9403 0x9403	//	密钥索引不支持
	#define SW_RESP_9406 0x9406	//	所需MAC 不可用																	
	#define SW_RESP_6E00 0x6E00	//	不支持的类：CLA 错
	#define SW_RESP_6D00 0x6D00	//	不支持的指令代码


	
#endif
