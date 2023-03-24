#include "pch.h"
#include "PlayerManager.h"
#include"AccountManager.h"

PlayerManager GPLayerManager;

void PlayerManager::PlayerThrenAccount()
{
	WRITE_LOCK;
	GAccountManager.Lock();
}
void PlayerManager::Lock()
{
	WRITE_LOCK;
}