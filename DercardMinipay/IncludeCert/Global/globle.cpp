#include "stdafx.h"

#include "globle.h"
//
//const stuListBox g_AsymKeyAlgList[MAX_LIST] = {
//	{eSafeChannelAlgorithm_ASYMKEY_ALG_ECC, _T("ECC")},
//	{eSafeChannelAlgorithm_ASYMKEY_ALG_RSA, _T("RSA1024")},
//	{eSafeChannelAlgorithm_ASYMKEY_ALG_RSA_2048, _T("RSA2048")}
//};
//
//const stuListBox g_AsymKeyIndexList[MAX_LIST] = {
//	{eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil, _T("终端")},
//	{eSafeChannelAlgorithm_ASYMKEY_INDEX_CAROOT, _T("CA")},
//	{eSafeChannelAlgorithm_ASYMKEY_INDEX_Pin, _T("Pin加密")},
//	{eSafeChannelAlgorithm_ASYMKEY_INDEX_Server, _T("服务器")}
//};
//
//const stuListBox g_ObjectTypeList[MAX_LIST] = {
//	{eSafeChannelAlgorithm_OBJECT_TYPE_Cert, _T("证书")},
//	{eSafeChannelAlgorithm_OBJECT_TYPE_PubKeyFile, _T("公钥")},
//	{eSafeChannelAlgorithm_OBJECT_TYPE_PriKeyFile, _T("私钥")},
//};

GUID GUID_DEVINTERFACE_SMARTCARD = {0x50dd5230L, 0xba8a, 0x11d1, 0xbf, 0x5d, 0x00, 0x00, 0xf8, 0x05, 0xf5, 0x30};

int g_CurAsymKeyAlgListSel = eSafeChannelAlgorithm_ASYMKEY_ALG_RSA;
int g_CurAsymKeyIndexListSel = eSafeChannelAlgorithm_ASYMKEY_INDEX_Termil;
int g_CurObjectTypeListSel = eSafeChannelAlgorithm_OBJECT_TYPE_Cert;

int g_CurDevSel = 0;
