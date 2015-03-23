#ifndef __SMARTDEF_H__
#define __SMARTDEF_H__

#include <winscard.h>
#include <windows.h>

#include "LCardDefine.h"
//#include "DefaultDefData.h"

#ifndef __out
#define __out 
#endif
#ifndef __in
#define __in 
#endif
#ifndef __out_opt
#define __out_opt 
#endif


#define SEND_TYPE_A	0
#define SEND_TYPE_B	1

typedef int KEYHANDLE;

#endif