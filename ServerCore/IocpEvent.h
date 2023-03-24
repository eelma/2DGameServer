#pragma once
class Session;
enum class EventType :uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv, 제로바이트리시브
	Recv,
	Send
};
/*-----------------
	 IocpEvent
-------------------*/
//OverlappedEx로 사용했던것
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
//accept를 하면서 빠져나올떄 clientSocket을 뱉어줘서 그 소켓을 사용했었다
//acceptex를 살펴보면 인자 중에 listen소켓과 client소켓을 같이 받아주는것을 볼 수 있다
//그래서 인자가 추가적으로 있을 수도 있고 없을 수도 있다
class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() :IocpEvent(EventType::Accept)
	{}

public:
	//TODO
	SessionRef session = nullptr;//클라세션
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