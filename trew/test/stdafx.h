// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")

#include "targetver.h"

#include <windows.h>
#include <process.h>



#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlenc.h>
#include <atlbase.h>
#include <atlstr.h>

// TODO: reference additional headers your program requires here
