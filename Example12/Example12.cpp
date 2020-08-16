/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "RNLobby/Inc/BackendClient.h"
#include "RNLobby/Inc/BackendServer.h"
#include "RNLobby/Inc/UserClient.h"
#include "RNLobby/Inc/StatClient.h"

using namespace RNReplicaNet;
using namespace RNLobby;

class MyUserClient : public RNReplicaNet::RNLobby::UserClient
{
public:
	MyUserClient() : mLoginID(-1) , mAlreadyExists(false) , mConnectionEstablished(false) ,
		mCallbackNewStatsAvailableACK(false) , mCallbackGotVariableACK(0)
	{
	}

	int mLoginID;

	RNReplicaNet::MutexClass mMutex;
	bool mAlreadyExists;
	bool mConnectionEstablished;
	std::string mUserNonce;
	bool mCallbackNewStatsAvailableACK;
	int mCallbackGotVariableACK;

protected:
	void CallbackDisconnected(Transport *transport)
	{
		printf("$%p %d MyUserClient::CallbackDisconnected\n",(void*)this,mLoginID);
		exit(-1);
	}

	void CallbackConnectionEstablished(RNReplicaNet::Transport *transport)
	{
		printf("$%p %d MyUserClient::CallbackConnectionEstablished\n",(void*)this,mLoginID);
		mConnectionEstablished = true;
	}

	void CallbackUserLogin(const int loginID)
	{
		mLoginID = loginID;
		printf("$%p %d MyUserClient::CallbackUserLogin %d name '%s' last online '%s'\n",(void*)this,mLoginID,loginID,GetLoginName().c_str(),GetLastOnlineTime().c_str());
	}

	void CallbackUserLoginFailed(const Reason reason)
	{
		mLoginID = -1;
		printf("$%p %d MyUserClient::CallbackUserLoginFailed %d\n",(void*)this,mLoginID,reason);
		exit(-1);
	}

	void CallbackUserLogout(const Reason reason)
	{
		printf("$%p %d MyUserClient::CallbackUserLogout %d\n",(void*)this,mLoginID,reason);
		mLoginID = -1;
		exit(-1);
	}

	void CallbackUserLoginFromOtherClient(void)
	{
		printf("$%p %d MyUserClient::CallbackUserLoginFromOtherClient\n",(void*)this,mLoginID);
		mLoginID = -1;
		exit(-1);
	}

	void CallbackStateChange(const int previousState,const int newState)
	{
		printf("$%p %d MyUserClient::CallbackStateChange %d %d\n",(void*)this,mLoginID,previousState,newState);
		if (newState & kState_Online)
		{
			printf("	Online\n");
		}
		if (newState & kState_FriendRequest)
		{
			printf("	Friend request\n");
		}
		if (newState & kState_GameInvite)
		{
			printf("	Game invite\n");
		}
		if (newState & kState_Message)
		{
			printf("	Message\n");
		}
	}

	void CallbackGotUserNonce(const char *userNonce,const int seconds)
	{
		printf("$%p %d MyUserClient::CallbackGotUserNonce %s : %d\n",(void*)this,mLoginID,userNonce,seconds);
		mUserNonce = userNonce;
	}

	void CallbackNewStatsAvailable(const int lastUpdatedTitleID)
	{
		printf("$%p %d MyUserClient::CallbackNewStatsAvailable %d\n",(void*)this,mLoginID,lastUpdatedTitleID);
		mCallbackNewStatsAvailableACK = true;
	}

	void CallbackGotVariable(const int loginID,const int section,const char *name,const char *value,const VariableAccess access)
	{
		printf("$%p %d MyUserClient::CallbackGotVariable %s %s\n",(void*)this,mLoginID,name,value);
		mCallbackGotVariableACK++;
	}
};

class MyStatClient : public RNReplicaNet::RNLobby::StatClient
{
public:
	MyStatClient()
	{
		mCallbackConnectionEstablished = false;
		mCallbackUploadAuthenticationFailure = false;
		mCallbackUploadAuthenticationSuccess = false;
		mCallbackUploadACK = false;
		mCallbackBeginSessionACK = false;
		mCallbackEndSessionACK = false;
		mCallbackEndSessionScriptNotFoundACK = false;
		mCallbackEndSessionScriptNotCompletedACK = false;
	}

	bool mCallbackConnectionEstablished;
	bool mCallbackUploadAuthenticationFailure;
	bool mCallbackUploadAuthenticationSuccess;
	bool mCallbackUploadACK;
	bool mCallbackBeginSessionACK;
	bool mCallbackEndSessionACK;
	bool mCallbackEndSessionScriptNotFoundACK;
	bool mCallbackEndSessionScriptNotCompletedACK;

protected:
	void CallbackDisconnected(Transport *transport)
	{
		printf("$%p MyStatClient::CallbackDisconnected\n",this);
		exit(-1);
	}

	void CallbackConnectionEstablished(RNReplicaNet::Transport *transport)
	{
		printf("$%p MyStatClient::CallbackConnectionEstablished\n",this);
		mCallbackConnectionEstablished = true;
	}

	void CallbackUploadAuthenticationFailure(void)
	{
		printf("$%p MyStatClient::CallbackUploadAuthenticationFailure\n",this);
		mCallbackUploadAuthenticationFailure = true;
		exit(-1);
	}

	void CallbackUploadAuthenticationSuccess(void)
	{
		printf("$%p MyStatClient::CallbackUploadAuthenticationSuccess\n",this);
		mCallbackUploadAuthenticationSuccess = true;
	}

	void CallbackUploadACK(const int scriptID)
	{
		printf("$%p MyStatClient::CallbackUploadACK %d\n",this,scriptID);
		mCallbackUploadACK = true;
	}

	void CallbackBeginSession(void)
	{
		printf("$%p MyStatClient::CallbackBeginSession\n",this);
		mCallbackBeginSessionACK = true;
	}

	void CallbackEndSession(void)
	{
		printf("$%p MyStatClient::CallbackEndSession\n",this);
		mCallbackEndSessionACK = true;
	}

	void CallbackEndSessionScriptNotFound(void)
	{
		printf("$%p MyStatClient::CallbackEndSessionScriptNotFound\n",this);
		mCallbackEndSessionScriptNotFoundACK = true;
		exit(-1);
	}

	void CallbackEndSessionScriptNotCompleted(void)
	{
		printf("$%p MyStatClient::CallbackEndSessionScriptNotCompleted\n",this);
		mCallbackEndSessionScriptNotCompletedACK = true;
		exit(-1);
	}
};

// The example starts here
int main(int argc,char **argv)
{
	const int titleID = 12;
	const char *titleKey = "Example12Key";

	const char *userName = "Example12User";
	const char *password = "Example12Password";

	printf("Attempting to connect");
	BackendClient backend;
	backend.UseReleaseServers(true);
	backend.Start(titleID,titleKey);

	// Authenticate with BackendServer
	while(!backend.GetCompleted())
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}

	printf("\n%s\n",BackendServer::TranslateReply(backend.GetReply()).c_str());

	if (!backend.IsAuthenticated())
	{
		printf("\nFailed to authenticate\n");
		exit(-1);
	}

	printf("\n");

	MyUserClient user;
	MyStatClient stat;

	user.Start();
	stat.Start();

	while(!user.mConnectionEstablished)
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}

	user.LoginUser(userName,password);

	while(user.GetLoginID() == UserInfo::kAnyLoginID)
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}

	// Causes CallbackGotVariable() to display the current value
	// Section 3 is used by Example12Script.txt
	user.GetVariable(3,"TotalValues",UserClient::kPrivateRead);

	printf("\n");

	while(!stat.mCallbackConnectionEstablished)
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}

	printf("\n");

	// Get the a user nonce value to use for the statistics. This user nonce can be used by
	// the client or any other user connected to the StatServer.
	user.mUserNonce.erase();
	user.RequestUserNonce(10);
	while(user.mUserNonce.empty())
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}
	printf("\nUpdating stats - Testing the SimpleScript StatServer interface.\n");

	// Begins a statistics tracking session with scriptID 1
	int scriptID = 1;
	stat.BeginSession(scriptID);
	// For a supplied user nonce begin updating value pairs.
	stat.UpdatePairsBegin(user.mUserNonce.c_str());
	int value1,value2;
	value1 = (rand()&15)+1;
	value2 = (rand()&15)+1;
	printf("Going to add %d + %d to TotalValues\n",value1,value2);
	stat.UpdatePairsValue("RandomValue1",value1);
	stat.UpdatePairsValue("RandomValue2",value2);
	stat.UpdatePairsEnd();

	// End the statistics tracking session and start the script processing on the StatServer.
	stat.EndSession();

	while (!user.mCallbackNewStatsAvailableACK)
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}

	// CallbackGotVariable() will then display the updated value
	user.mCallbackGotVariableACK = 0;
	user.GetVariable(3,"TotalValues",UserClient::kPrivateRead);
	user.GetVariable(0, 3,"TotalGamesPlayed",UserClient::kPublicReadPrivateRead);

	while (user.mCallbackGotVariableACK < 2)
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}


	printf("\nUsing scriptID 2 - Testing the C++ StatServer interface\n");

	// Get another nonce for the user
	user.mUserNonce.erase();
	user.RequestUserNonce(10);
	while(user.mUserNonce.empty())
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}
	// Begins a statistics tracking session with scriptID 2
	scriptID = 2;
	stat.BeginSession(scriptID);
	// For a supplied user nonce begin updating value pairs.
	stat.UpdatePairsBegin(user.mUserNonce.c_str());
	// The ProcessPlayer function in Example12Script2.cpp does TotalValues = SomeValue1 + SomeValue2 - SomeValue3 + SomeValue4
	// So TotalValues should equal 10 + 20 - 3 + 30 which should equal 57
	stat.UpdatePairsValue("SomeValue1",10);
	stat.UpdatePairsValue("SomeValue2",20);
	stat.UpdatePairsValue("SomeValue3",3);
	stat.UpdatePairsValue("SomeValue4",30);
	stat.UpdatePairsEnd();

	// End the statistics tracking session and start the script processing on the StatServer.
	stat.EndSession();

	while (!user.mCallbackNewStatsAvailableACK)
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}

	// CallbackGotVariable() will then display the updated value
	user.mCallbackGotVariableACK = 0;
	user.GetVariable(1,"TotalValues",UserClient::kPublicReadPrivateRead);
	int i;
	// This should print 32 entries with all of them zero except the first which should be the loginID of the Example12User account.
	for ( i = 0 ; i < 32 ; i++ )
	{
		user.GetVariable( 0, 2000 + i , "LoginIDArray" , UserClient::kPublicReadPrivateRead );
	}

	while (user.mCallbackGotVariableACK < 2)
	{
		printf(".");
		CurrentThread::Sleep(1000);
	}

	return 0;
}
