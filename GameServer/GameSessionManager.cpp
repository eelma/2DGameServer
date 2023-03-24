#include "pch.h"
#include "GameSessionManager.h"
#include"GameSession.h"
GameSessionManager GSessionManager;
void GameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void GameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void GameSessionManager::BroadCast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	//���������� �����ٰ� Remove���� ȣ���ؼ� session�� �ǵ鿩����
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}