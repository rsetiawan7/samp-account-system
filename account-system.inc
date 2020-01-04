// #define YSI_IS_CLIENT
// #define MASTER 1

// #if !defined _Y_MASTER_INCLUDED
// 	#tryinclude <YSI-Core\y_master>
// #endif
// #if !defined _Y_MASTER_INCLUDED
// 	#error "y_master (YSI 4.x) is required to use this script."
// #endif

#if !defined _INC_y_remote
	#tryinclude <YSI-Coding\y_remote>
#endif
#if !defined _INC_y_remote
	#error "y_remote (YSI 4.x) is required to use this script."
#endif

#define E_USERNAME_REQUIRED             -10001
#define E_PASSWORD_REQUIRED             -10002
#define E_INSUFFICIENT_PASSWORD_LENGTH  -10003
#define E_PLAYER_NOT_CONNECTED          -10004
#define E_INVALID_CREDENTIAL            -10005
#define E_EXCEED_PASSWORD_LENGTH				-10006
#define E_USERNAME_EXIST								-10007
#define E_EMAIL_REQUIRED								-10008
#define E_ACCOUNT_ID_REQUIRED						-10009
#define E_NICKNAME_REQUIRED							-10010
#define E_PLAYER_NOT_LOGGED_IN          -20001
#define E_PLAYER_ALREADY_LOGGED_IN      -20002
#define E_PLAYER_CHARACTER_INVALID      -20003
#define E_PLAYER_DATA_NAME_INVALID      -20004

remotefunc bool:IsPlayerLoggedIn(playerid){} 
remotefunc bool:IsPlayerCharacterSelected(playerid){} 
remotefunc CreatePlayerAccount(const string:username[], const string:password[], const string:email[]){} 
remotefunc CreateCharacterAccount(const account_id, const string:nickname[]){} 
remotefunc LoadPlayerData(playerid){} 
remotefunc LogInPlayerAccount(playerid, const string:username[], const string:password[]){} 
remotefunc LoadPlayerCharacter(playerid){} 
remotefunc SetPlayerData(playerid, const string:data[], const string:value[]){} 
remotefunc LogOutFromPlayerAccount(playerid){} 
remotefunc SavePlayerCharacterData(playerid){} 

stock LogIn(playerid, username[], password[])
{
	new result = localfunc LogInPlayerAccount(playerid, username, password);
	return result;
}