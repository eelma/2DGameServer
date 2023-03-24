#pragma once
//클라이언트의 ip주소 등을 추출하고 싶을 때 사용할것
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
	static IN_ADDR Ip2Address(const WCHAR* ip);//ip주소를 이용해서 IN_ADDR로 반환하고 싶다
private:
	SOCKADDR_IN _sockAddr = {};
};

