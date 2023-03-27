#include "pch.h"
#include "AccountManager.h"
#include"PlayerManager.h"
AccountManager GAccountManager;

void AccountManager::AcountThenPlayer()
{
	WRITE_LOCK;
	GPLayerManager.Lock();
}
void AccountManager::Lock()
{
	WRITE_LOCK;
}