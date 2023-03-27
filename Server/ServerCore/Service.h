#pragma once
#include"NetAddress.h"
#include"IocpCore.h"
#include"Listener.h"
#include <functional>

enum class ServiceType :uint8
{
	Server,
	Client
};
/*--------------
	 Service
----------------*/

using SessionFactory = function<SessionRef(void)>;

class Service :public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress netAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;
	bool CanStart() { return _sessionFactory != nullptr; }

	virtual void CloseService();
	void SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	void Broadcast(SendBufferRef sendBuffer);//모든 세션에 브로드캐스팅
	SessionRef CreateSession();
	void AddSession(SessionRef session);
	void ReleaseSession(SessionRef session);
	int32 GetCurrentSessionCount() { return _sessionCount; }
	int32 GetMaxSessionCount() { return _maxSessionCount; }

public:
	ServiceType GetServiceType() { return _type; }
	NetAddress GetNetAddress() { return _netAddress; }
	IocpCoreRef GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;

	ServiceType _type;//타입
	NetAddress _netAddress = {};//상대방일수도 있고 나일수도 있고
	IocpCoreRef _iocpCore;//내가 어떤 iocp에다가 일감을 넘겨줄지

	Set<SessionRef> _sessions;//지금까지 연결된 세션들을 들고있음
	int32 _sessionCount = 0;//세션이 몇개 있는지
	int32 _maxSessionCount = 0;//최대 세션은 얼마인지
	SessionFactory _sessionFactory;//세션을 생성하는 함수(함수포인터를 받아줌)
};

/*-------------------
	 ClientService
---------------------*/

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}

	virtual bool Start()override;
};


/*-------------------
	 ServerService
---------------------*/


class ServerService : public Service
{
public:
	ServerService(NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {};

	virtual bool Start()override;
	virtual void CloseService()override;

private:
	ListenerRef _listener = nullptr;


};