#if defined MAX_PLAYER_NAME
  #undef MAX_PLAYER_NAME
  #define MAX_PLAYER_NAME 24
#else
  #define MAX_PLAYER_NAME 24
#endif

// YCMD:register(playerid, params[], help) {
//   new
//     username[MAX_PLAYER_NAME + 1],
//     password[MAX_PASSWORD_LENGTH + 1];

//   if (help || sscanf(params, "s["#MAX_PLAYER_NAME"] s["#MAX_PASSWORD_LENGTH"]", username, password))
//   {
//     SendClientMessage(playerid, X11_WHITE, "* USAGE: /register [username] [password]");
//     SendClientMessage(playerid, X11_WHITE, "* FUNCTION: Register a new account.");
//     return 1;
//   }

//   return 1;
// }