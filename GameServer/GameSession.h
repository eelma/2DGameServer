#pragma once
#include"session.h"
//Listener는 쉐어드포인터로만 활용
class GameSession : public PacketSession
{
public:
	~GameSession() { cout << "~Server" << endl; }
public:
	virtual void OnConnected()override;

	virtual void OnDisconnected()override;

	virtual void OnRecvPacket(BYTE* buffer, int32 len)override;

	virtual void OnSend(int32 len) override;

public:
	Vector<PlayerRef>_players;
};