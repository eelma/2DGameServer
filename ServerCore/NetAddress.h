#pragma once
//Ŭ���̾�Ʈ�� ip�ּ� ���� �����ϰ� ���� �� ����Ұ�
/*------------------
	 NetAddress
--------------------*/
class NetAddress
{
public:
	NetAddress() {};
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(wstring ip, uint16 port);

	SOCKADDR_IN& GetSockAddr() { return _sockAddr; }
	wstring GetIpAddress();
	uint16 GetPort() { return ::ntohs(_sockAddr.sin_port); }

public:
	static IN_ADDR Ip2Address(const WCHAR* ip);//ip�ּҸ� �̿��ؼ� IN_ADDR�� ��ȯ�ϰ� �ʹ�
private:
	SOCKADDR_IN _sockAddr = {};
};

