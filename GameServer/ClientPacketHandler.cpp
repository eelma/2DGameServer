#include "pch.h"
#include "ClientPacketHandler.h"
#include"GameSession.h"
#include"Player.h"
#include"Room.h"
PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	//잘못된 패킷이 왔을때 패킷헤더를 까서 살펴 본 다음 로그 확인
	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	//TODO : Validation 체크

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	//DB에서 플레이 정보를 긁어온다
	//Gamesession에 플레이 정보를 저장(메모리)
	//보통 account라는 클래스를 만들어서 저장함

	//ID 발급(DB아이디가 아니고 인게임 아이디)
	static Atomic<uint64>idGenerator = 1;

	{
		auto player = loginPkt.add_players();
		player->set_name(u8"긁어온 이름");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;
		//세션이라는 존재자체에서도 내가 들고있는 플레이어들을 메모리상에서 들고있어야한다
	
		gameSession->_players.push_back(playerRef);

	}
	{
		auto player = loginPkt.add_players();
		player->set_name(u8"긁어온 이름2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;
		//세션이라는 존재자체에서도 내가 들고있는 플레이어들을 메모리상에서 들고있어야한다

		gameSession->_players.push_back(playerRef);

	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	//MakeSendBuffer를 호출해서 loginPkt를 건내주면 버퍼에다가 기입
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{

	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();
	//TODO : Vadliation

	//굳이 Handle_C_LOGIN에서 메모리에 넣어둔 이유가 이 부분 때문
	//굳이 DB에 두번 접근할 필요가 없고 메모리에 들고있는 다음 두번째로 index가 왔으면 해당 캐릭터 정보를 꺼내줌
	PlayerRef player = gameSession->_players[index];//READ_ONLY
	GRoom.Enter(player);//입장시켜줌

	//성공적으로 EnterGame이 됐다고 답장을 줄것
	Protocol::S_ENTER_GAME enterGamePkt;//입장했다고 답변
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

	GRoom.Broadcast(sendBuffer);//wrtir_lock 다수의 스레드가 c_chat에 들어와서 건들수도 있다

	return true;
}
