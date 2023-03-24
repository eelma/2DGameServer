#pragma once

//iocp의 CP를 담당한다

/*--------------
	IocpObject
----------------*/
//무조건 iocp에 등록이 가능한것
//나중에 얘를 상속시켜줄것
// 원래 CreateIoCompletionPort를 두번 사용할떄 건내줄 수 있는 값이 두가지였다
// 첫번째 정보는 iocpobject가 될것이다(session)과 같은 느낌
//iocpEvent는 Overlapped구조체를 상속을 받아서 read write등등을 할 수 있엇는데 그 역할을 할것이다
class IocpObject:public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE	GetHandle()abstract;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes=0)abstract;

private:

};

/*--------------
	IocpCore
----------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE			GetHandle() { return _iocpHandle; }
	bool			Register(IocpObjectRef iocpObject);//등록 함수
	bool			Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;//자신의 핸들
};
