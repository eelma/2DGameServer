#include "pch.h"
#include "ClientPacketHandler.h"
#include"GameSession.h"
#include"Player.h"
#include"Room.h"
PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	//�߸��� ��Ŷ�� ������ ��Ŷ����� � ���� �� ���� �α� Ȯ��
	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	//TODO : Validation üũ

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	//DB���� �÷��� ������ �ܾ�´�
	//Gamesession�� �÷��� ������ ����(�޸�)
	//���� account��� Ŭ������ ���� ������

	//ID �߱�(DB���̵� �ƴϰ� �ΰ��� ���̵�)
	static Atomic<uint64>idGenerator = 1;

	{
		auto player = loginPkt.add_players();
		player->set_name(u8"�ܾ�� �̸�");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;
		//�����̶�� ������ü������ ���� ����ִ� �÷��̾���� �޸𸮻󿡼� ����־���Ѵ�
	
		gameSession->_players.push_back(playerRef);

	}
	{
		auto player = loginPkt.add_players();
		player->set_name(u8"�ܾ�� �̸�2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;
		//�����̶�� ������ü������ ���� ����ִ� �÷��̾���� �޸𸮻󿡼� ����־���Ѵ�

		gameSession->_players.push_back(playerRef);

	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	//MakeSendBuffer�� ȣ���ؼ� loginPkt�� �ǳ��ָ� ���ۿ��ٰ� ����
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{

	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();
	//TODO : Vadliation

	//���� Handle_C_LOGIN���� �޸𸮿� �־�� ������ �� �κ� ����
	//���� DB�� �ι� ������ �ʿ䰡 ���� �޸𸮿� ����ִ� ���� �ι�°�� index�� ������ �ش� ĳ���� ������ ������
	PlayerRef player = gameSession->_players[index];//READ_ONLY
	GRoom.Enter(player);//���������

	//���������� EnterGame�� �ƴٰ� ������ �ٰ�
	Protocol::S_ENTER_GAME enterGamePkt;//�����ߴٰ� �亯
	enterGamePkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	player->ownerSession->Send(sendBuffer);


	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;
	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	GRoom.Broadcast(sendBuffer);//wrtir_lock �ټ��� �����尡 c_chat�� ���ͼ� �ǵ���� �ִ�

	return true;
}
