#pragma once
#include"IocpCore.h"
#include"IocpEvent.h"
#include "NetAddress.h"
#include"RecvBuffer.h"
/*----------------
	  Session
------------------*/
class Service;
class Session:public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;
	enum
	{
		BUFFER_SIZE=0x10000,
	};
public:
	Session();
	virtual ~Session();

	/*�ܺ� ��� �Լ�*/
public:
	void Send(SendBufferRef sendBuffer);
	bool Connect();
	void Disconnect(const WCHAR* cause);

	shared_ptr<Service> GetService() { return _service.lock(); }
	void SetService(shared_ptr<Service>service) { _service = service; }

public:
	/*���� ���� �Լ�*/
	void SetNetAddress(NetAddress netaddress) { _netAddress = netaddress; }
	NetAddress GetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }
	bool IsConnected(){return _connected; }
	SessionRef GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
	virtual HANDLE GetHandle()override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes)override;

private:
	/* ���� ���� �Լ� */
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errorCode);

protected:
	/* ������ �ڵ忡�� ������ */
	virtual void OnConnected(){}
	virtual int32 OnRecv(BYTE* buffer, int32 len){ return len; }
	virtual void OnSend(int32 len){}
	virtual void OnDisconnected(){}


private:
	weak_ptr<Service> _service;
	SOCKET			_socket = INVALID_SOCKET;//Ŭ���̾�Ʈ ����
	NetAddress		_netAddress = {};//���������� �ּ�
	Atomic<bool>	_connected = false;//���ӻ��� üũ


private:
	USE_LOCK;
	/*���� ����*/
	RecvBuffer _recvBuffer;

	/*�۽� ����*/
	Queue<SendBufferRef> _sendQueue;

	Atomic<bool> _sendRegistered = false;
private:

	ConnectEvent	_connectEvent;
	DisconnectEvent _disconnectEvent;
	RecvEvent		_recvEvent;
	SendEvent		_sendEvent;

};


/*---------------------
	  PacketSession
-----------------------*/
//� �����ε� ��ü ��Ŷ�� ������ �Ǿ����� üũ
// //����� ���� �Ľ��� �ؼ�(�ּ� 4����Ʈ�� ���� ����)����� � ���� ����� �����ؼ� ���ݱ��� ���� �����Ͱ� ������� ū�� Ȯ��
//[size(2)][id(2)][data...]
struct PacketHeader
{
	//�������� ������ ������ size�� ���� �޾ƾ���
	uint16 size;//�츮�� ���� ��ü ��Ŷ�� ������
	uint16 id;//�������� id
};

class PacketSession :public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32 OnRecv(BYTE* buffer, int32 len)sealed;
	virtual void OnRecvPacket(BYTE* buffer, int32 len)abstract;

private:

};
