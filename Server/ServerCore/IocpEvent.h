#pragma once
class Session;
enum class EventType :uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv, ���ι���Ʈ���ú�
	Recv,
	Send
};
/*-----------------
	 IocpEvent
-------------------*/
//OverlappedEx�� ����ߴ���
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);
	void			Init();
public:
	EventType eventType;
	IocpObjectRef owner;//using IocpObjectRef = std::shared_ptr<class IocpObject>;
	//iocpobject=(session)
};

/*-------------------
	 ConnectEvent
---------------------*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent():IocpEvent(EventType::Connect){}
};
/*-------------------
	 DisconnectEvent
---------------------*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() :IocpEvent(EventType::Disconnect) {}
};

/*-------------------
	 AcceptEvent
---------------------*/
//accept�� �ϸ鼭 �������Ë� clientSocket�� ����༭ �� ������ ����߾���
//acceptex�� ���캸�� ���� �߿� listen���ϰ� client������ ���� �޾��ִ°��� �� �� �ִ�
//�׷��� ���ڰ� �߰������� ���� ���� �ְ� ���� ���� �ִ�
class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() :IocpEvent(EventType::Accept)
	{}

public:
	//TODO
	SessionRef session = nullptr;//Ŭ�󼼼�
};
/*-------------------
	 RecvEvent
---------------------*/
class RecvEvent : public IocpEvent
{
public:
	RecvEvent() :IocpEvent(EventType::Recv) {}
};

/*-------------------
	 SendEvent
---------------------*/
class SendEvent : public IocpEvent
{
public:
	SendEvent() :IocpEvent(EventType::Send) {}
	Vector<SendBufferRef> sendBuffers;
};