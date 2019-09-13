#if !defined _INC_y_remote
	#tryinclude <YSI-Coding\y_remote>
#endif
#if !defined _INC_y_remote
	#error "y_remote (YSI 4.x) is required to use this script."
#endif

remotefunc bool:IsPlayerLoggedIn(playerid);
remotefunc bool:IsPlayerCharacterSelected(playerid);
remotefunc LogInPlayerAccount(playerid, string:username[], string:password[]);
remotefunc LoadPlayerCharacter(playerid);
remotefunc SetPlayerData(playerid, const string:data[], const string:value[]);
remotefunc LogOutFromPlayerAccount(playerid);
remotefunc SavePlayerCharacterData(playerid);

stock LogIn(playerid, username[], password[])
{
	new result = localfunc LogInPlayerAccount(playerid, username, password);
	return result;
}