-- BackendServer.cpp
CREATE TABLE Backend(TitleID INTEGER PRIMARY KEY, EncryptionKey TEXT, SupportExpired INTEGER, 
		RLSrvAdvertiseAddress TEXT, RLSrvAdvertisePort INTEGER, RLSrvAdvertiseCrypto TEXT, 
		RLSrvCodeAddress TEXT, RLSrvCodePort INTEGER, RLSrvCodeCrypto TEXT, 
		RLSrvNATAddress TEXT, RLSrvNATPort INTEGER, RLSrvNATCrypto TEXT, 
		RLSrvNATDetect1Address TEXT, RLSrvNATDetect1Port INTEGER, RLSrvNATDetect2Address TEXT, RLSrvNATDetect2Port INTEGER, RLSrvNATDetectCrypto TEXT, 
		RLSrvStatAddress TEXT, RLSrvStatPort INTEGER, RLSrvStatCrypto TEXT, 
		RLSrvUserAddress TEXT, RLSrvUserPort INTEGER, RLSrvUserCrypto TEXT, 
		RLSrvCloudAddress TEXT, RLSrvCloudPort INTEGER, RLSrvCloudCrypto TEXT
	);

-- Web
CREATE TABLE UserWebNonces(LoginID INTEGER, WebNonce TEXT, ExpireTime BIGINT);

CREATE TABLE ProductKeys(ProductKey TEXT, ProductFeature INTEGER DEFAULT 0, LoginID INTEGER DEFAULT 0, DateUpdated TIMESTAMP DEFAULT NOW());

CREATE TABLE ForgottenWebNonces(LoginID INTEGER, WebNonce TEXT, ExpireTime BIGINT);

CREATE TABLE LogAdminUsers(LoginID INTEGER, ForTitleID INTEGER, ReadAccess INTEGER, WriteAccess INTEGER);

-- Used by the services when they start
CREATE TABLE LogsServiceStartup(ID SERIAL PRIMARY KEY, ServiceName TEXT, Version TEXT, SourceRequest TEXT, DateUpdated TIMESTAMP DEFAULT NOW());

-- Log.cpp - Standard services logs used by the web interface
CREATE TABLE LogsAdvertiseServer(ID SERIAL PRIMARY KEY, Entry TEXT, LoggedTime TEXT );
CREATE TABLE LogsBackendServer(ID SERIAL PRIMARY KEY, Entry TEXT, LoggedTime TEXT );
CREATE TABLE LogsUserServer(ID SERIAL PRIMARY KEY, Entry TEXT, LoggedTime TEXT );
CREATE TABLE LogsStatServer(ID SERIAL PRIMARY KEY, Entry TEXT, LoggedTime TEXT );
CREATE TABLE LogsSecurity(ID SERIAL PRIMARY KEY, Entry TEXT, LoggedTime TEXT );

-- AdvertiseServer.cpp
CREATE TABLE GameListings(ID SERIAL PRIMARY KEY, TitleID INTEGER, LoginID INTEGER DEFAULT 0, Url TEXT, UserBlock TEXT, ExternalIP TEXT, LastTime TIMESTAMP);

CREATE TABLE ServerRules(AdvertisedID INTEGER, Rule TEXT, Data TEXT);

CREATE TABLE ServerPlayers(AdvertisedID INTEGER, Name TEXT, Data TEXT, LoginID INTEGER DEFAULT 0);

-- Security.cpp
CREATE TABLE IPBlockListFromLoginFail(ID SERIAL PRIMARY KEY, TheIP TEXT, UserAccount TEXT , TimeUpdated TIMESTAMP DEFAULT NOW());

--CREATE TABLE IPBlockListTotal(TheIP TEXT, NumTimes INTEGER , TimeUpdated TIMESTAMP DEFAULT NOW());

-- StatServer.cpp
-- This table defines access for a loginID to be able to upload data for a TitleID that appears in ForTitleID.
CREATE TABLE StatAdminUsers(LoginID INTEGER, ForTitleID INTEGER);

-- UserServer.cpp
CREATE TABLE Users(ID SERIAL PRIMARY KEY, TitleID INTEGER, Name TEXT, PasswordHash TEXT, EmailAddress TEXT, HiddenFromEveryone INTEGER DEFAULT 0, HiddenFromFriends INTEGER DEFAULT 0, Online INTEGER DEFAULT 0, LastOnlineTime TIMESTAMP, AccountCreated TIMESTAMP, PlayingAGame INTEGER DEFAULT 0, PlayingAGameURL TEXT, NewStatsAvailable INTEGER DEFAULT 0, MachineUniqueID TEXT, CannotChangePassword INTEGER DEFAULT 0, CannotChangeEmail INTEGER DEFAULT 0);

CREATE TABLE UserRelationship(ID SERIAL PRIMARY KEY, UpdatedRecently INTEGER DEFAULT 1, LoginID INTEGER, OtherID INTEGER, Authorised INTEGER DEFAULT 0, HasBeenAdded INTEGER DEFAULT 1, MarkForDelete INTEGER DEFAULT 0, IsFriend INTEGER DEFAULT 0, IsIgnored INTEGER DEFAULT 0, GameInvite TEXT , TimeUpdated TIMESTAMP DEFAULT NOW() );

CREATE TABLE OfflineMessages(ID SERIAL PRIMARY KEY, FromLoginID INTEGER, ToLoginID INTEGER, Message TEXT, TimeUpdated TIMESTAMP DEFAULT NOW() );

CREATE TABLE Variables(TitleID INTEGER, LoginID INTEGER, Section INTEGER, Access INTEGER, Name TEXT, Value TEXT, TimeUpdated TIMESTAMP DEFAULT NOW() );

-- If the LoginID appears in this table then the user is able to be administrator of any room.
CREATE TABLE SuperRoomUsers(LoginID INTEGER, TimeUpdated TIMESTAMP DEFAULT NOW() );

CREATE TABLE AccountBlocked(LoginID INTEGER, ByWhom INTEGER, Reason TEXT, BlockedWhen TIMESTAMP DEFAULT NOW());

CREATE TABLE UserNonces(LoginID INTEGER, UserNonce TEXT, ExpireTime BIGINT);

CREATE TABLE UserNetworkAddressHistory(ID SERIAL PRIMARY KEY, LoginID INTEGER, NetworkAddress TEXT, FromService TEXT, LoggedTime TIMESTAMP DEFAULT NOW());
