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

	/*외부 사용 함수*/
public:
	void Send(SendBufferRef sendBuffer);
	bool Connect();
	void Disconnect(const WCHAR* cause);

	shared_ptr<Service> GetService() { return _service.lock(); }
	void SetService(shared_ptr<Service>service) { _service = service; }

public:
	/*정보 관련 함수*/
	void SetNetAddress(NetAddress netaddress) { _netAddress = netaddress; }
	NetAddress GetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }
	bool IsConnected(){return _connected; }
	SessionRef GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
	virtual HANDLE GetHandle()override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes)override;

private:
	/* 전송 관련 함수 */
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
	/* 컨텐츠 코드에서 재정의 */
	virtual void OnConnected(){}
	virtual int32 OnRecv(BYTE* buffer, int32 len){ return len; }
	virtual void OnSend(int32 len){}
	virtual void OnDisconnected(){}


private:
	weak_ptr<Service> _service;
	SOCKET			_socket = INVALID_SOCKET;//클라이언트 소켓
	NetAddress		_netAddress = {};//접속했을때 주소
	Atomic<bool>	_connected = false;//접속상태 체크


private:
	USE_LOCK;
	/*수신 관련*/
	RecvBuffer _recvBuffer;

	/*송신 관련*/
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
//어떤 식으로든 전체 패킷이 전송이 되었는지 체크
// //헤더를 먼저 파싱을 해서(최소 4바이트를 받은 상태)헤더를 까본 다음 사이즈를 추출해서 지금까지 받은 데이터가 사이즈보다 큰지 확인
//[size(2)][id(2)][data...]
struct PacketHeader
{
	//가변적인 데이터 때문에 size를 먼저 받아야함
	uint16 size;//우리가 보낼 전체 패킷의 사이즈
	uint16 id;//프로토콜 id
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
