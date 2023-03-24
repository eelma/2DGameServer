#pragma once
//윈속 초기화, 비동기 connect disconnect들은 그냥 사용할 수 없고 처음에 프로그램이 실행 될 때 런타임의 주소를 얻어와야한다
#include"NetAddress.h"

/*---------------
	SocketUtils
-----------------*/
class SocketUtils
{
public:
	static LPFN_CONNECTEX		ConnectEx; //CONNECTEX 함수의 포인터
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	static void Init();
	static void Clear();

	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);//어떤 보이드포인터형 함수 주소를 받아줌
	static SOCKET CreateSocket();//임의의 tcp소켓 만드는 함수

	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, int32 flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	static bool Bind(SOCKET socket, NetAddress netAddr);
	static bool BindAnyAddress(SOCKET socket, uint32 port);//socket에 임의의 ip주소를 바인딩하겠다
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

template<typename T>
static inline bool SetSocketOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}