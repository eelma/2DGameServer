#pragma once

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "Container.h"

#include <iostream>
using namespace std;

#include<WinSock2.h>
#include<mswsock.h>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>

#include "Lock.h"

#include "ObjectPool.h"
#include "TypeCast.h"
#include "memory.h"
#include"SendBuffer.h"
#include"Session.h"