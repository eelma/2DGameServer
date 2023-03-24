#pragma once
class PlayerManager
{
	USE_LOCK;

public:
	void PlayerThrenAccount();
	void Lock();
};
extern PlayerManager GPLayerManager;
