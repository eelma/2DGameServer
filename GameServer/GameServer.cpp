#include "pch.h"
#include "ThreadManager.h"

#include"Service.h"
#include"Session.h"
#include"GameSession.h"
#include"GameSessionManager.h"
#include"BufferWriter.h"
#include"ClientPacketHandler.h"
#include<tchar.h>
#include"Protocol.pb.h"

//패킷 직렬화(Serialization)



int main()
{
	ClientPacketHandler::Init();
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 10000),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		1000);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 1; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}


	GThreadManager->Join();
}