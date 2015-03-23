
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SafeChannel_Tool.h"



SCN_ULONG toInt(SCN_UCHAR pArray[], SCN_ULONG start, SCN_ULONG length)
{
	SCN_ULONG i = 0, value = 0;

	for (i = 0; i < length; i++) {
		value *= 256;
		value += ((SCN_ULONG)pArray[i + start] & 0xff);
	}
	return value;
}

void adjust(SCN_UCHAR *pDerStr, SCN_ULONG *size, SCN_ULONG *length)
{
	if (((*(pDerStr + 1)) & 0x80) == 0x80) {
		*size = (*(pDerStr + 1) & 0xf);
		*length = toInt(pDerStr, 2, *size);
	} else {
		*size = 0;
		*length = (*(pDerStr + 1) & 0xff);
	}
}

void computeOffset(SCN_UCHAR *pDerStr, item_t *item, SCN_ULONG offset)
{
	SCN_ULONG size = 0, length = 0;

	adjust(pDerStr, &size, &length);

	item->nDataOffset = offset;
	item->nDataLength = 2 + size + length;
	
	item->nValueOffset = 2 + size + offset;
	item->nValueLength = length;
}


void SafeChannel_ScanCertRSA(SCN_UCHAR * pDer, x509_cert_t *x509)
{
	SCN_ULONG position = 0, size = 0, length = 0;
	SCN_ULONG nSignatureValue_Offset = 0, nSignatureValue_Length = 0; 
	position = 0;
	/* scan total certificate for size and length */
	/* get length size and length */
	adjust(pDer, &size, &length);
	x509->pDer = pDer;
	x509->nDerLength = size + length + 2;

	/* 2 for type and lengthbit length */
	position += 2 + size;
	/* scan tbs size and length */
	adjust(pDer + position, &size, &length);
	x509->tbs.nDataOffset = position;
	x509->tbs.nDataLength = length + 2 + size;
	x509->tbs.nValueOffset = position + 2 + size;
	x509->tbs.nValueLength = length;

	position += 2 + size + length;
	/* get signature size and length */
	adjust(pDer + position, &size, &length);
	x509->signature.nDataOffset = position;
	x509->signature.nDataLength = length + 2 + size;
	x509->signature.nValueOffset = position + 2 + size;
	x509->signature.nValueLength = length;

	position += 2 + size + length;
	/* get signatureValue size and length */
	adjust(pDer + position, &size, &length);
	x509->signatureValue.nDataOffset = position;
	x509->signatureValue.nDataLength = length + 2 + size;
	x509->signatureValue.nValueOffset = position + 2 + size;
	x509->signatureValue.nValueLength = length;

	SafeChannel_ScanCertTbsRSA(pDer, x509, x509->tbs.nDataOffset);

	SafeChannel_ScanCertSignatureValueRSA(pDer, x509, &nSignatureValue_Offset, &nSignatureValue_Length, x509->signatureValue.nDataOffset);
	
	return;
}

// 扫描证书主体部分
void SafeChannel_ScanCertTbsRSA(SCN_UCHAR *pDer, x509_cert_t *x509, SCN_ULONG offset)
{
	SCN_ULONG position = 0, size = 0, length = 0;
	position = offset;
	
	computeOffset(pDer + position, &x509->tbs, position);
	position = x509->tbs.nValueOffset;

	/* version */
	if ((pDer[position] & 0xFF) == 0xA0) {
		position += 2;
		computeOffset(pDer + position, &x509->version, position );
		position = x509->version.nValueLength + x509->version.nValueOffset;
	}

	/* SerialNumber */
	computeOffset(pDer + position, &x509->serialNumber, position);
	position = x509->serialNumber.nValueLength + x509->serialNumber.nValueOffset;

	/* TBSCertificate.Signature */
	computeOffset(pDer + position, &x509->tbssignature, position);
	position = x509->tbssignature.nValueLength + x509->tbssignature.nValueOffset;

	/* TBSCertificate.Issuer */
	computeOffset(pDer + position, &x509->issuerName, position);
	position = x509->issuerName.nValueLength + x509->issuerName.nValueOffset;

	/* TBSCertificate.Validity */
	computeOffset(pDer + position, &x509->notAfter, position);
	position = x509->notAfter.nValueLength + x509->notAfter.nValueOffset;

	/* TBSCertificate.Subject */
	computeOffset(pDer + position, &x509->subjectName, position);
	position = x509->subjectName.nValueLength + x509->subjectName.nValueOffset;

	/* SubjectPublicKeyInfo */
	computeOffset(pDer + position, &x509->publicKey, position);
	position = x509->publicKey.nValueLength + x509->publicKey.nValueOffset;

	/* issuerUniqueID, for A1 */
	if ((pDer[position] & 0xFF) == 0xA1) {
		computeOffset(pDer + position, &x509->issuerUniqueID, position);
		position = x509->issuerUniqueID.nValueLength + x509->issuerUniqueID.nValueOffset;
	}

	/* subjectUniqueID, for A2 */
	if ((pDer[position] & 0xFF) == 0xA2) {
		computeOffset(pDer + position, &x509->subjectUniqueID, position);
		position = x509->subjectUniqueID.nValueLength + x509->subjectUniqueID.nValueOffset;
	}

	/* extensions, for A3 */
	if ((pDer[position] & 0xFF) == 0xA3) {
		computeOffset(pDer + position, &x509->extension, position);
		position = x509->extension.nValueLength + x509->extension.nValueOffset;
	}

	/* notAfter && notBefore 

		SEQUENCE{
			UTC TIME notBefore,
			UTC TIME notAfter
		} 
	*/

	position = x509->notAfter.nValueOffset;
	computeOffset(pDer + position, &x509->notBefore, position);

	position = x509->notBefore.nValueLength + x509->notBefore.nValueOffset ;
	computeOffset(pDer + position, &x509->notAfter, position);

	return;
}

// 扫描证书签名值部分
void SafeChannel_ScanCertSignatureValueRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
										   SCN_ULONG *pnSignatureValue_Offset, SCN_ULONG *pnSignatureValue_Length, 
										   SCN_ULONG offset)
{
	SCN_ULONG position = 0, size = 0, length = 0;
	position = offset;

	computeOffset(pDer + position, &x509->signatureValue, position);

	*pnSignatureValue_Offset = x509->signatureValue.nValueOffset;
	*pnSignatureValue_Length = x509->signatureValue.nValueLength;
	if(pDer[*pnSignatureValue_Offset] == 0x00)
	{
		(*pnSignatureValue_Offset)++;
		(*pnSignatureValue_Length)--;
	}

	return;
}

// 扫描获取公钥
void SafeChannel_ScanCertPubKeyRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
								   SCN_ULONG *pnPubkeyModeN_Offset, SCN_ULONG *pnPubkeyModeN_Length, 
								   SCN_ULONG *pnPubkeyModeE_Offset, SCN_ULONG *pnPubkeyModeE_Length, 
								   SCN_ULONG offset)
{

	SCN_ULONG position = 0, size = 0, length = 0;
	position = offset;

	/* publicKey  

		SEQUENCE{
			SEQUENCE{
				OBJECT IDENTIFIER,
				NULL
			},
			BIT STRING 
		} 
	*/
	computeOffset(pDer + position, &x509->publicKey, position);
	
	// jump SEQUENCE
	position = x509->publicKey.nValueOffset;
	adjust(pDer + position, &size, &length);
	
	// 
	position += 2 + size + length;
	adjust(pDer + position, &size, &length);

	// jugg 00
	position += 2 + size;
	if (pDer[position] == 0x00)
	{
		position++;
	}

	adjust(pDer + position, &size, &length);

	// Mode N
	position += 2 + size;
	if (pDer[position] == 0x02)
	{
		adjust(pDer + position, &size, &length);
		position += 2 + size;

		(*pnPubkeyModeN_Offset) = position;
		(*pnPubkeyModeN_Length) = length;

		if (pDer[position] == 0x00)
		{
			(*pnPubkeyModeN_Offset)++;
			(*pnPubkeyModeN_Length)--;
		}
		
		position += length;
	}

	// Mode E
	if (pDer[position] == 0x02)
	{
		adjust(pDer + position, &size, &length);
		position += 2 + size;

		(*pnPubkeyModeE_Offset) = position;
		(*pnPubkeyModeE_Length) = length;
	}

	return;
}


// 获取OU
void SafeChannel_ScanCertOURSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
										 SCN_ULONG *pnOU_Offset, SCN_ULONG *pnOU_Length, 
										 SCN_ULONG offset)
{

	SCN_ULONG position = 0, size = 0, length = 0, nextTag = 0;
	position = offset;

	/* SubjectName  
		SEQUENCE{
			Set{
				SEQUENCE{
					OBJECT IDENTIFIER,
					PRINTABLE STRING
				}
			}
			Set{
				SEQUENCE{
					OBJECT IDENTIFIER,
					PRINTABLE STRING
				}
			}
			...
		}
	*/
	computeOffset(pDer + position, &x509->subjectName, position);
	size = x509->subjectName.nValueOffset;
	length = x509->subjectName.nValueLength;
	// 
	position = x509->subjectName.nValueOffset;
	while(position < (x509->subjectName.nValueOffset + x509->subjectName.nValueLength))
	{
		if (pDer[position] == 0x31)
		{
			adjust(pDer + position, &size, &length);
			position += 2 + size;
			nextTag = position + length;
			if (pDer[position] == 0x30)
			{
				adjust(pDer + position, &size, &length);
				position += 2 + size;
				if (pDer[position] == 0x06)
				{
					adjust(pDer + position, &size, &length);
					position += 2 + size;
					if (length == 3
						&& pDer[position + 0] == 0x55
						&& pDer[position + 1] == 0x04
						&& pDer[position + 2] == 0x0B)
					{
						position += length;
						if (pDer[position] == 0x13)
						{
							adjust(pDer + position, &size, &length);
							(*pnOU_Offset) = position + size + 2;
							(*pnOU_Length) = length;
							return;
						}
					}
					else
					{
						position = nextTag;
						continue;
					}
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
		else
		{
			return;
		}
	}
}

// 获取证书序列号
void SafeChannel_ScanCertSerialNumberRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
										 SCN_ULONG *pnSerialNumber_Offset, SCN_ULONG *pnSerialNumber_Length, 
										 SCN_ULONG offset)
{
	SCN_ULONG position = 0, size = 0, length = 0;
	position = offset;

	computeOffset(pDer + position, &x509->serialNumber, position);

	*pnSerialNumber_Offset = x509->serialNumber.nValueOffset;
	*pnSerialNumber_Length = x509->serialNumber.nValueLength;
}

// 获取证书有效期
void SafeChannel_ScanCertValidDataRSA(SCN_UCHAR * pDer, x509_cert_t *x509, 
										 SCN_ULONG *pnValidData_Offset, SCN_ULONG *pnValidData_Length,
										 SCN_ULONG offset)
{
	SCN_ULONG position = 0, size = 0, length = 0;
	position = offset;

	computeOffset(pDer + position, &x509->notAfter, position);

	*pnValidData_Offset = x509->notAfter.nValueOffset;
	*pnValidData_Length = x509->notAfter.nValueLength;
}