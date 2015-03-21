#ifndef _SelfDefine_H_
#define _SelfDefine_H_

#define CLA   0
#define INS   1
#define P1    2
#define P2    3
#define Lc    4
#define Le    4
#define DataAddr 5

#define CLA_SYSTME_CMD_NONE                0xFF
#define INS_SYSTME_CMD                     0x70
#define P1_SYSTEM_CMD                      0x25
#define P2_SYSTEM_CMD                      0x7B
#define LC_SYSTEM_CMD                      0x00

#define Data_CLA                            0xFE
#define Data_INS_SetTerminalInfo            0x11
#define Data_INS_SetDeviceKey               0xA3
#define Data_P1_DEV                         0x00
#define Data_P1_EID                         0x01
#define Data_P1_MiniPay                     0x02 
#define Data_P1_CCB                         0x03
#define Data_P1_SelfCheck                   0x04

#define Data_P2                             0x00
#define Data_Lc                             0x00
#define Data_Offset                         0x0A

#define  P2_TerminalID                      0x01

#endif 

