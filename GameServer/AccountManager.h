#pragma once
class AccountManager
{
	USE_LOCK;

public:
	void AcountThenPlayer();
	void Lock();
};

extern AccountManager GAccountManager;
