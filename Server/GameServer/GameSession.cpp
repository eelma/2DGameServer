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

	//�ڱ� �ڽ��� ���������� �ǳ�����ϴ� PacketsessionRef�� �޾Ƽ� �ǳ��ش�
	PacketSessionRef session = GetPacketSessionRef();
	//session���� �����ϸ鼭 refcount�� 1������ ��Ŵ
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	//������ �������� �л��ؼ� ����� �� �ֱ� ������ 
	//��Ŷ����� �����ؼ� ���̵� �뿪�븦 Ȯ���ؼ� ��� ������ �и����� �ʿ䰡 �ִ�

	//TODO: PacketId �뿪 üũ
	ClientPacketHandler::HandlePacket(session, buffer, len);

}

void GameSession::OnSend(int32 len)
{
	
}