#if defined FOREIGNS_INCLUDED
	#endinput
#endif
#define FOREIGNS_INCLUDED

foreign bool:IsPlayerLoggedIn(playerid);
foreign bool:IsPlayerCharacterSelected(playerid);
foreign CreatePlayerAccount(const string:username[], const string:password[], const string:email[]);
foreign CreateCharacterAccount(const account_id, const string:nickname[]);
// foreign LoadPlayerData(playerid);
foreign LogInPlayerAccount(playerid, const string:username[], const string:password[]);
foreign LoadPlayerCharacter(playerid);
foreign SetPlayerData(playerid, const string:data[], const string:value[]);
foreign LogOutFromPlayerAccount(playerid);
foreign SavePlayerCharacterData(playerid);