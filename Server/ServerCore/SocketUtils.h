#pragma once
//���� �ʱ�ȭ, �񵿱� connect disconnect���� �׳� ����� �� ���� ó���� ���α׷��� ���� �� �� ��Ÿ���� �ּҸ� ���;��Ѵ�
#include"NetAddress.h"

/*---------------
	SocketUtils
-----------------*/
class SocketUtils
{
public:
	static LPFN_CONNECTEX		ConnectEx; //CONNECTEX �Լ��� ������
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	static void Init();
	static void Clear();

	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);//� ���̵��������� �Լ� �ּҸ� �޾���
	static SOCKET CreateSocket();//������ tcp���� ����� �Լ�

	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, int32 flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	static bool Bind(SOCKET socket, NetAddress netAddr);
	static bool BindAnyAddress(SOCKET socket, uint32 port);//socket�� ������ ip�ּҸ� ���ε��ϰڴ�
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

template<typename T>
static inline bool SetSocketOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}