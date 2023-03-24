#include "pch.h"
#include "SocketUtils.h"
/*---------------
	SocketUtils
-----------------*/
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;


void SocketUtils::Init()
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), &wsaData)==0);
	/*��Ÿ�ӿ� �ּ� ������ API*/
	//���̷� ������ ���� ���� �� ������ ������� ��� �Լ� ȣ��
	SOCKET dummySocket = CreateSocket();

	//guid��ġ�� ã�ƿ��� ���� �Լ� ã�ƿ�
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));

	Close(dummySocket);
}


void SocketUtils::Clear()
{
	WSACleanup();
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{//��Ÿ�ӿ� ConnectEx, DisconnectEx, AcceptEx �ּҸ� �޾ƿ��� �Լ�
	DWORD bytes = 0;
	return SOCKET_ERROR!=::WSAIoctl(socket,SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT &bytes, NULL,NULL );//��Ÿ�ӿ� �ּҸ� �������� ���� �Լ�

}

SOCKET SocketUtils::CreateSocket()
{
	return::WSASocket(AF_INET,SOCK_STREAM, IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);//overlapped�迭�� �񵿱� �Լ����� ����� �� ����
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_linger = linger;
	option.l_onoff = onoff;
	return SetSocketOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, int32 flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}
//ListenSocket�� Ư���� ClientSocket�� �״�� ����
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR!=::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()),sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint32 port)
{//�ƹ��ų� ���ε� �ش޶�
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = htons(port);

	return SOCKET_ERROR!=::bind(socket, reinterpret_cast<const SOCKADDR*>(&myAddress), sizeof(myAddress));

}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != listen(socket, backlog);
}

void SocketUtils::Close(SOCKET& socket)
{
	if(socket!=INVALID_SOCKET)
	::closesocket(socket);
	socket = INVALID_SOCKET;
}
