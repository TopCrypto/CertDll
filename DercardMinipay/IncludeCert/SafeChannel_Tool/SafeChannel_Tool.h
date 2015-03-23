#ifndef _SAFE_CHANNEL_TOOL_H_
	#define _SAFE_CHANNEL_TOOL_H_
	
	#include "SafeChannel_DataType.h"

	// 扫描完整证书
	void SafeChannel_ScanCertRSA(SCN_UCHAR * pDer, x509_cert_t *x509);
	void SafeChannel_ScanCertSM2(SCN_UCHAR * pDer, x509_cert_t *x509);
	// 扫描证书主体部分
	void SafeChannel_ScanCertTbsRSA(SCN_UCHAR * pDer, x509_cert_t *x509, SCN_ULONG offset);
	void SafeChannel_ScanCertTbsSM2(SCN_UCHAR * pDer, x509_cert_t *x509, SCN_ULONG offset);
	// 扫描证书签名值部分
	void SafeChannel_ScanCertSignatureValueRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnSignatureValue_Offset, SCN_ULONG *pnSignatureValue_Length, 
		SCN_ULONG offset);
	void SafeChannel_ScanCertSignatureValueSM2(SCN_UCHAR * pDer, x509_cert_t *x509, SCN_ULONG offset);

	// 扫描获取公钥
	void SafeChannel_ScanCertPubKeyRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnPubkeyModeN_Offset, SCN_ULONG *pnPubkeyModeN_Length, 
		SCN_ULONG *pnPubkeyModeE_Offset, SCN_ULONG *pnPubkeyModeE_Length, 
		SCN_ULONG offset);
	void SafeChannel_ScanCertPubKeySM2(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnPubkeyModeX_Offset, SCN_ULONG *pnPubkeyModeX_Length, 
		SCN_ULONG *pnPubkeyModeY_Offset, SCN_ULONG *pnPubkeyModeY_Length, 
		SCN_ULONG offset);

	// 获取OU
	void SafeChannel_ScanCertOURSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnOU_Offset, SCN_ULONG *pnOU_Length, 
		SCN_ULONG offset);
	void SafeChannel_ScanCertOUSM2(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnOU_Offset, SCN_ULONG *pnOU_Length, 
		SCN_ULONG offset);

	// 获取证书序列号
	void SafeChannel_ScanCertSerialNumberRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnSerialNumber_Offset, SCN_ULONG *pnSerialNumber_Length, 
		SCN_ULONG offset);
	void SafeChannel_ScanCertSerialNumberSM2(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnSerialNumber_Offset, SCN_ULONG *pnSerialNumber_Length, 
		SCN_ULONG offset);

	// 获取证书有效期
	void SafeChannel_ScanCertValidDataRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnValidData_Offset, SCN_ULONG *pnValidData_Length,
		SCN_ULONG offset);
	void SafeChannel_ScanCertValidDataSM2(SCN_UCHAR * pDer, x509_cert_t *x509, 
		SCN_ULONG *pnValidData_Offset, SCN_ULONG *pnValidData_Length,
		SCN_ULONG offset);
	//////////////////////////////////////////////////////////////////////////

	// 2字节取高低位
	#ifndef HIBYTE
		#define HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))
	#endif

	#ifndef LOBYTE
		#define LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
	#endif

	// 32-bit integer manipulation macros (big endian)
	#ifndef GET_UINT32
		#define GET_UINT32(n, b, i)		    \
		{					    \
		(n) = ( (unsigned long)(b)[(i)	 ] << 24)   \
		| ( (unsigned long)(b)[(i) + 1] << 16)   \
		| ( (unsigned long)(b)[(i) + 2] <<  8)   \
		| ( (unsigned long)(b)[(i) + 3]	);  \
		}
	#endif

	#ifndef PUT_UINT32
		#define PUT_UINT32(n,b,i)		  \
		{					  \
		(b)[(i)    ] = (unsigned char)((n) >> 24);   \
		(b)[(i) + 1] = (unsigned char)((n) >> 16);   \
		(b)[(i) + 2] = (unsigned char)((n) >>  8);   \
		(b)[(i) + 3] = (unsigned char)((n)      );   \
		}
	#endif

	#ifndef GET_UINT16
		#define GET_UINT16(n, b, i)		    \
		{					    \
		(n) = ( (unsigned short)(b)[(i)	 ] << 8)   \
		| ( (unsigned short)(b)[(i) + 1]);  \
		}
	#endif

	#ifndef PUT_UINT16
		#define PUT_UINT16(n,b,i)		  \
		{					  \
		(b)[(i)    ] = (unsigned char)((n) >> 8);   \
		(b)[(i) + 1] = (unsigned char)((n) >> 0);   \
		}
	#endif

	#ifndef _GETXOR_DEFINED
		#define GetXor(a,b,c) \
		{						\
		*(c)=(*(a))^(*(b));\
		*(c+1)=(*(a+1))^(*(b+1));\
		*(c+2)=(*(a+2))^(*(b+2));\
		*(c+3)=(*(a+3))^(*(b+3));\
		*(c+4)=(*(a+4))^(*(b+4));\
		*(c+5)=(*(a+5))^(*(b+5));\
		*(c+6)=(*(a+6))^(*(b+6));\
		*(c+7)=(*(a+7))^(*(b+7));\
		}
		#define _GETXOR_DEFINED
	#endif

	#ifdef USE_ANSI
		#define R_memset(x, y, z)	memset(x, y, z)
		#define R_memcpy(x, y, z)	memcpy(x, y, z)
		#define R_memcmp(x, y, z)	memcmp(x, y, z)
		#define R_free(x)			free(x)
		#define R_malloc(x)			malloc(x)
	#else
		void R_memset(void *, int, unsigned int);
		void R_memcpy(void *, void *, unsigned int);
		int R_memcmp(void *, void *, unsigned int);
		void R_free(void * p);
		void *R_malloc(int);
	#endif

#endif
