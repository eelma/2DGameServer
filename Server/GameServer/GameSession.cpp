#include "pch.h"
#include"GameSession.h"
#include"GameSessionManager.h"
#include"ClientPacketHandler.h"
void GameSession::OnConnected()
{

	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

}

void GameSession::OnDisconnected()
{

	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{

	//자기 자신이 누구인지를 건내줘야하니 PacketsessionRef를 받아서 건내준다
	PacketSessionRef session = GetPacketSessionRef();
	//session으로 추출하면서 refcount를 1증가를 시킴
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	//서버를 여러개로 분산해서 사용할 수 있기 때문에 
	//패킷헤더를 참고해서 아이디 대역대를 확인해서 어느 쪽인지 분리해줄 필요가 있다

	//TODO: PacketId 대역 체크
	ClientPacketHandler::HandlePacket(session, buffer, len);

}

void GameSession::OnSend(int32 len)
{
	
}