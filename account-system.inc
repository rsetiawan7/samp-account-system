#if !defined _INC_y_remote
  #tryinclude <YSI_Coding\y_remote>
#endif
#if !defined _INC_y_remote
  #error "y_remote (YSI 5.x) is required to use this script."
#endif

#include "includes/errors.inc"

// CALLBACKS

forward OnPlayerLoggedIn(playerid, account_id);
forward OnPlayerLoggedOut(playerid, account_id);
forward OnPlayerLoginFailed(playerid, attempt);

remotefunc stock void:INTL_OnPlayerLoggedIn(playerid, account_id)
{
  CallLocalFunction("OnPlayerLoggedIn", "dd", playerid, account_id);
}

remotefunc stock void:INTL_OnPlayerLoginFailed(playerid, attempt)
{
  CallLocalFunction("OnPlayerLoginFailed", "dd", playerid, attempt);
}

remotefunc stock void:INTL_OnPlayerLoggedOut(playerid, account_id)
{
  CallLocalFunction("OnPlayerLoggedOut", "dd", playerid, account_id);
}

// FUNCTIONS

remotefunc stock bool:INTL_IsPlayerLoggedIn(playerid);
remotefunc stock INTL_CreatePlayerAccount(const string:username[], const string:password[], const string:email[]);
remotefunc stock INTL_LoadPlayerData(playerid);
remotefunc stock INTL_LogInPlayerAccount(playerid, const string:username[], const string:password[]);
remotefunc stock INTL_SetPlayerData(playerid, const string:data[], const string:value[]);
remotefunc stock INTL_LogOutFromPlayerAccount(playerid);
// remotefunc bool:IsPlayerCharacterSelected(playerid);
// remotefunc CreateCharacterAccount(const account_id, const string:nickname[]);
// remotefunc LoadPlayerCharacter(playerid);
// remotefunc SavePlayerCharacterData(playerid);

stock bool:IsPlayerLoggedIn(playerid)
{
  return broadcastfunc INTL_IsPlayerLoggedIn(playerid);
}

stock CreatePlayerAccount(const string:username[], const string:password[], const string:email[])
{
  return broadcastfunc INTL_CreatePlayerAccount(username, password, email);
}

stock LogInPlayerAccount(playerid, const string:username[], const string:password[])
{
  return broadcastfunc INTL_LogInPlayerAccount(playerid, username, password);
}

stock LogOutFromPlayerAccount(playerid)
{
  return broadcastfunc INTL_LogOutFromPlayerAccount(playerid);
}

stock LoadPlayerData(playerid)
{
  return broadcastfunc INTL_LoadPlayerData(playerid);
}