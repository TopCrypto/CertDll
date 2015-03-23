#ifndef __LCARDDEFINE_H__
#define __LCARDDEFINE_H__

#include "SmartDef.h"


#define HARDENCRYPT					0
#define HARDDECRYPT					1

#define HARDCRYPTALG_DES			1
#define HARDCRYPTALG_TDES			2
#define HARDCRYPTALG_SSF33			4
#define HARDCRYPTALG_SM1			8

//#define ALG_TYPE					HARDCRYPTALG_DES
#define ALG_TYPE					HARDCRYPTALG_TDES
//#define ALG_TYPE					HARDCRYPTALG_SM1

#if (ALG_TYPE==HARDCRYPTALG_DES)
#define KEY_LEN						8
#else
#define KEY_LEN					16
#endif

#define USER_PIN_LEN				8
#define ADMIN_PIN_LEN				8


//RSA parm
#define RSA_N_Tag		0x00
#define RSA_E_Tag		0x01
#define RSA_P_Tag		0x02
#define RSA_Q_Tag		0x03
#define RSA_DP_Tag		0x04
#define RSA_DQ_Tag		0x05
#define RSA_QINVP_Tag	0x06

//RSA SIZE
#define RSA_N_SIZE      256
#define RSA_E_SIZE      4
#define RSA_PRIME_SIZE  128

//Types of file sytstem
#define FILE_TYPE_MF						0x00
#define FILE_TYPE_DDF						0x01
#define FILE_TYPE_ADF						0x02
#define FILE_TYPE_BIN						0x03
#define FILE_TYPE_RECORD_FIX				0x04
#define FILE_TYPE_RECORD_UNFIX				0x05
#define FILE_TYPE_RECORD_CYCLE				0x06
#define FILE_TYPE_EP						0x07
#define FILE_TYPE_ED						0x08
#define FILE_TYPE_SF						0x09
#define FILE_TYPE_RSA_PUB					0x0A
#define FILE_TYPE_RSA_PRI					0x0B
#define FILE_TYPE_ECC_PARM					0x0C
#define FILE_TYPE_ECC_PUB					0x0D
#define FILE_TYPE_ECC_PRI					0x0E


// CryptCompute direction definition
#define CARD_OPER_RSA_SIGN		0
#define CARD_OPER_RSA_VERYIFY	1


#endif