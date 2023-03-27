#pragma once

//iocp�� CP�� ����Ѵ�

/*--------------
	IocpObject
----------------*/
//������ iocp�� ����� �����Ѱ�
//���߿� �긦 ��ӽ����ٰ�
// ���� CreateIoCompletionPort�� �ι� ����ҋ� �ǳ��� �� �ִ� ���� �ΰ�������
// ù��° ������ iocpobject�� �ɰ��̴�(session)�� ���� ����
//iocpEvent�� Overlapped����ü�� ����� �޾Ƽ� read write����� �� �� �־��µ� �� ������ �Ұ��̴�
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
	bool			Register(IocpObjectRef iocpObject);//��� �Լ�
	bool			Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;//�ڽ��� �ڵ�
};
