#ifndef _GLOBLE_H_
#define _GLOBLE_H_

#include "App.h"
#include "CspAPI.h"
#include "SmartCardApi.h"
#include "SmartDef.h"
#include "tool.h"
// dbt
#include "dbt.h"
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")

#include <string>

//typedef struct _tagListBox{
//	int  index;
//	CString strName;
//}stuListBox;

#define MAX_LIST	10
//extern const stuListBox g_AsymKeyAlgList[MAX_LIST];
//extern const stuListBox g_AsymKeyIndexList[MAX_LIST];
//extern const stuListBox g_ObjectTypeList[MAX_LIST];

extern GUID GUID_DEVINTERFACE_SMARTCARD;
extern int g_CurDevSel;


extern int g_CurAsymKeyAlgListSel;
extern int g_CurAsymKeyIndexListSel;
extern int g_CurObjectTypeListSel;


#endif