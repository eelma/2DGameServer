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
	//내부적으로 돌리다가 Remove까지 호출해서 session을 건들여버림
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}
