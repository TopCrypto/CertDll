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
	// �Ự
	#define MAX_CERTIFICATE_SIZE	2048					// ֤�������ݴ洢�ռ��С
	#define MAX_SIGNATURE_SIZE		MAX_RSA_MODULUS_LEN		// ǩ�����ݴ洢�ռ��С
	#define MAX_RANDOM_SZIE			32						// ����ͨ��������������ݴ洢�ռ��С
	#define MAX_CIPHERSUITE_SZIE	1						// ����ͨ�������㷨��ʾ���ݴ洢�ռ��С
	#define MAX_SHARESECRET_SZIE	16						// ����ͨ��������������ݴ洢�ռ��С
	#define MAX_HMAC_SIZE			0x20					// HMAC�㷨��������ݴ洢�ռ��С
	#define MAX_RSA_SIZE			MAX_RSA_MODULUS_LEN		// ��ǰRSA�㷨���ݴ洢�ռ��С

	#define MAX_RETRY_COUNT			3						// ��ȫ�Ự�������Դ���		
	//////////////////////////////////////////////////////////////////////////
	// ���

	#define SCI_CERT_NONE					0x00

	#define SCI_CON_EXIST             0x80  // �����ѽ�����ʶ
	#define SCI_CON_SM2_OK		  	  0x40  // ������ΪSM2�㷨,
	#define SCI_CON_RSA2048_OK        0x20  // ������ΪRSA2048�㷨
	#define SCI_CON_RSA1024_OK        0x10  // ������ΪRSA1024�㷨
	#define SCI_CON_ENC_CERT_OK       0x08  // �����м���֤����ڱ�ʶ
	#define SCI_CON_SIGN_CERT_OK      0x04  // ������ǩ��֤����ڱ�ʶ
	#define SCI_CON_KEY_SIGN_OK       0x02  // �����м�����Կ�Դ��ڱ�ʶ
	#define SCI_CON_KEY_ENC_OK        0x01  // ������ǩ����Կ�Դ��ڱ�ʶ

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


	// CipherSuite ��ʾ
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
	#define SCIR_SUCCESS					AIR_SUCCESS					// �ɹ�
	#define SCIR_FAIL						AIR_FAIL					// ʧ��
	#define SCIR_UNKNOWN_ERROR				AIR_UNKNOWN_ERROR			// δ֪����

	#define SCIR_NOT_INITIALIZE_ERROR		AIR_NOT_INITIALIZE_ERROR	// δ��ʼ��
	#define SCIR_OBJECT_ERROR				AIR_OBJECT_ERROR			// �������

	#define SCIR_IN_DATA_LENGTH_ERROR		AIR_IN_DATA_LENGTH_ERROR	// �������ݳ��ȴ���
	#define SCIR_IN_DATA_ERROR				AIR_IN_DATA_ERROR			// �������ݴ���

	#define SCIR_HASH_OBJECT_ERROR			AIR_HASH_OBJECT_ERROR		// Hash �������
	#define SCIR_HASH_NOT_EQUAL_ERROR		AIR_HASH_NOT_EQUAL_ERROR	// Hash ֵ�����
	#define SCIR_PARAMETER_ERROR			AIR_PARAMETER_ERROR			// ��Ч�Ĳ���

	#define SCIR_NO_ROOM_ERROR				AIR_NO_ROOM_ERROR			// �ռ䲻��
	#define SCIR_NO_ACCESS					(AIR_NO_ROOM_ERROR + 1)		// Ȩ�޲���

	#define SCIR_NO_OPT_FLASH_ERROR 		(AIR_NO_ROOM_ERROR + 2) 	// EEP ��������
	#define SCIR_OBJECT_NOT_EXIST			(AIR_NO_ROOM_ERROR + 3)		// �������ݲ�����

	//////////////////////////////////////////////////////////////////////////
	// �ն�������Ӧ״̬���б�
	
	#define SW_RESP_61FF 0x61FF	// ��������																	
	#define SW_RESP_6200 0x6200 // ����Ϣ�ṩ																	
	#define SW_RESP_6281 0x6281	// ���͵����ݿ����д�																	
	#define SW_RESP_6282 0x6282	// �ļ�����<Le																	
	#define SW_RESP_6283 0x6283	// ѡ����ļ���Ч
	#define SW_RESP_6284 0x6284	// FCI ��ʽ��P2 ָ���Ĳ���																	
	#define SW_RESP_6300 0x6300	// ��֤ʧ��
	#define SW_RESP_63CF 0x63CF	// ��֤ʧ�ܣ���ʣ��X �γ���													
	#define SW_RESP_6400 0x6400	// ״̬��־λδ��																	
	#define SW_RESP_6581 0x6581	// �ڴ����	
	#define SW_RESP_6700 0x6700	// ���ȴ���	
	#define SW_RESP_6882 0x6882	// ��֧�ְ�ȫ����																	
	#define SW_RESP_6900 0x6900	// ���ܴ���	
	#define SW_RESP_6901 0x6901	// ������ܣ���Ч״̬��																	
	#define SW_RESP_6981 0x6981	// �������ļ��ṹ������																	
	#define SW_RESP_6982 0x6982	// �����㰲ȫ״̬
	#define SW_RESP_6983 0x6983	// ��֤��������
	#define SW_RESP_6984 0x6984	// ����������Ч																	
	#define SW_RESP_6985 0x6985	// ʹ������������
	#define SW_RESP_6986 0x6986	// ����������ִ�е�����(�ǵ�ǰEF)																	
	#define SW_RESP_6987 0x6987	// ��ȫ���������ʧ
	#define SW_RESP_6988 0x6988	// ��ȫ��Ϣ���ݶ�����ȷ																	
	#define SW_RESP_6A80 0x6A80	//	�������������ȷ																	
	#define SW_RESP_6A81 0x6A81	//	���ܲ�֧��
	#define SW_RESP_6A82 0x6A82	//	δ�ҵ��ļ�																	
	#define SW_RESP_6A83 0x6A83	//	δ�ҵ���¼																	
	#define SW_RESP_6A84 0x6A84	//	�ļ��д洢�ռ䲻��																	
	#define SW_RESP_6A86 0x6A86	//	P1��P2 ��������ȷ
	#define SW_RESP_6A88 0x6A88	//	���������Ҳ���		
	#define SW_RESP_6A89 0x6A89	//	�ļ��Ѵ��ڻ����ļ�����ͻ																	
	#define SW_RESP_6B00 0x6B00	//	��������(ƫ�Ƶ�ַ������EF)																	
	#define SW_RESP_6CFF 0x6CFF	//	���ȴ���(Le ����'XX'Ϊʵ�ʳ���)																	
	#define SW_RESP_6F00 0x6F00	//	������Ч
	#define SW_RESP_6F01 0x6F01	//	δ����PIN
	#define SW_RESP_9000 0x9000	//	�ɹ�ִ�У��޴���
	#define SW_RESP_9301 0x9301	//	����																	
	#define SW_RESP_9302 0x9302	//	MAC ��Ч																	
	#define SW_RESP_9303 0x9303	//	Ӧ��������ס
	#define SW_RESP_9401 0x9401	//	����
	#define SW_RESP_9402 0x9402	//	���׼������������ֵ																	
	#define SW_RESP_9403 0x9403	//	��Կ������֧��
	#define SW_RESP_9406 0x9406	//	����MAC ������																	
	#define SW_RESP_6E00 0x6E00	//	��֧�ֵ��ࣺCLA ��
	#define SW_RESP_6D00 0x6D00	//	��֧�ֵ�ָ�����


	
#endif
