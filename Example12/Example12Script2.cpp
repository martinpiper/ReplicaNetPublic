// This is executed by the StatServer
#include <stdio.h>
#include <stdlib.h>
#include "RNLobby/Inc/Stat.h"

// Testing globally initialised data
static int sNumPlayers = 0;
static int sPlayerIDArray[32];	// Up to 32 players

namespace RNReplicaNet
{

namespace RNLobby
{

namespace StatServerSession
{

void ProcessPlayer( const int loginID )
{
	int value = GetResult( loginID , "SomeValue1" , 0 ) + GetResult( loginID , "SomeValue2" , 0 ) - GetResult( loginID , "SomeValue3" , 0 ) + GetResult( loginID , "SomeValue4" , 0 );
	SetVariable( loginID , 1 , UserClient::kPublicReadPrivateRead , "TotalValues" , value );

	if (sNumPlayers < 32)
	{
		sPlayerIDArray[sNumPlayers] = loginID;
		sNumPlayers++;
	}
}

void ProcessGame( void )
{
	int i;
	for ( i = 0 ; i < sNumPlayers ; i++ )
	{
		SetVariable( 0 , 2000 + i  , UserClient::kPublicReadPrivateRead , "LoginIDArray" , sPlayerIDArray[i] );
	}

	// Zero out any other entries in the variables
	for ( ; i < 32 ; i++ )
	{
		SetVariable( 0 , 2000 + i  , UserClient::kPublicReadPrivateRead , "LoginIDArray" , 0 );
	}
}

} // namespace StatServerSession

} // namespace RNLobby

} // namespace RNReplicaNet
