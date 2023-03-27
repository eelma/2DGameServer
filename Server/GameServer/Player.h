#pragma once
class Player
{

public:
	
	uint64 playerId = 0;
	//플레이어 아이디를 발급해서 관리

	string					name;
	Protocol::PlayerType	type = Protocol::PLAYER_TYPE_NONE;
	GameSessionRef			ownerSession;
	//내가 소속된 게임세션
	//근데 쉐어드 포인터로 만들어주면 서로 알기 때문에 사이클 문제가 생긴다
};

