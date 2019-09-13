// Number of iterations to make a strong password.
#define ITERATIONS 1000

// Macros.
#define MAX_CHARACTERS_PER_ACCOUNT  1
#define MAX_ACCOUNT_NAME_LENGTH     42
#define MAX_EMAIL_LENGTH            128
#define MAX_SALT_LENGTH             128
#define MIN_PASSWORD_LENGTH         8
#define MAX_PASSWORD_LENGTH         64

// Trying to include y_iterate with a few ways.
#if !defined _INC_y_iterate
    #tryinclude <YSI-Data\y_iterate>
#endif
#if !defined _INC_y_iterate
    #error "y_iterate (YSI 4.x) is required."
#endif

// Trying to include y_remote with a few ways.
#if !defined _INC_y_remote
    #tryinclude <YSI-Coding\y_remote>
#endif
#if !defined _INC_y_remote
    #error "y_remote (YSI 4.x) is required."
#endif

// Trying to include hash-plugin with a few ways.
#if !defined _hash_included
    #tryinclude "hash-plugin\hash"
#endif
#if !defined _hash_included
    #tryinclude <hash>
#endif
#if !defined _hash_included
    #error "Mellnik's hash-plugin is required. Get it by 'sampctl package install Mellnik/hash-plugin' or go to https://forum.sa-mp.com/showthread.php?t=496953 or https://github.com/Mellnik/hash-plugin"
#endif

// Include internal's error codes.
#include "includes/errors.inc"

// Enumeration for accout's player
enum e_PlayerAccount
{
    // Account ID
    AID,
    // Character name
    Name[MAX_ACCOUNT_NAME_LENGTH + 1],
    // E-mail address that binded to this account
    eMail[MAX_EMAIL_LENGTH + 1],
    // Date and time (formed in UNIX timestamp) the account is created
    CreatedAt,
    // Date and time (formed in UNIX timestamp) the account is last updated
    UpdatedAt,
    // Date and time (formed in UNIX timestamp) the account is activated
    ActivatedAt,
    // Number of character's slot that active right now (playing).
    CharacterActive
};

// Enumeration for player account's character.
enum e_PlayerCharacter
{
    // Character ID
    CID,
    // Account ID
    AID,
    // Character name
    Name[MAX_PLAYER_NAME + 1],
    // Amount of money hold with that character
    Money,
    // Character's score
    Score,
    // Kills number of character
    Kills,
    // Deaths number of character
    Deaths,
    // Date and time (formed in UNIX timestamp) the character is created
    CreatedAt,
    // Date and time (formed in UNIX timestamp) the character is last updated
    UpdatedAt
};

new
    gSalt[MAX_SALT_LENGTH + 1],
    // Create an array to store account's player data.
    PlayerAccount[MAX_PLAYERS][e_PlayerAccount],
    // Create an array to store character's player data.
    PlayerData[MAX_PLAYERS][MAX_CHARACTERS_PER_ACCOUNT][e_PlayerCharacter],
    // Iteration for logged in players.
    Iterator:LoggedInPlayers<MAX_PLAYERS>;

#include "includes/utilities.inc"

// ONLY one of method storage allowed and it must defined.
#if defined USE_MYSQL
    #include "includes/storages/mysql"
#endif
#if defined USE_SQLITE
    #include "includes/storages/sqlite"
#endif
#if !defined STORAGE_METHOD_INCLUDED
    #error "One of storage method is required. Please #define USE_MYSQL to use MySQL or #define USE_SQLITE to use SQLite."
#endif

hook OnFilterScriptInit()
{
    // Initialize iteration for logged in players.
    Iter_Init(LoggedInPlayers);

    // Initialize the system. (depends on storage method)
    InitAccountSystem();
}

hook OnFilterScriptExit()
{
    P:4("Start to save logged in's player data");

    // Loop each logged in players.
    foreach (new playerid : LoggedInPlayers)
    {
        P:4("Saving player data with Player ID: %i ...", playerid);

        // Save each player's character that logged in and selected a character.
        new saveable = localfunc SavePlayerCharacterData(playerid);

        P:4("Save player data with Player ID: %i result --> %i", saveable);
    }

    // Close the connection to database.
    CloseAccountSystem();
}

AddPlayerAccount(const username[], const password[], const email[]) {
    // Username must be filled.
    if (isnull(username))
    {
        return E_USERNAME_REQUIRED;
    }

    // Password must be filled.
    if (isnull(password))
    {
        return E_PASSWORD_REQUIRED;
    }

    // E-mail address must be filled.
    if (isnull(email))
    {
        return E_EMAIL_REQUIRED;
    }

    // Password's length must be more than MIN_PASSWORD_LENGTH characters.
    if (strlen(password) < MIN_PASSWORD_LENGTH)
    {
        return E_INSUFFICIENT_PASSWORD_LENGTH;
    }

    // Password's length must be less than MAX_PASSWORD_LENGTH characters.
    if (strlen(password) > MAX_PASSWORD_LENGTH)
    {
        return E_EXCEED_PASSWORD_LENGTH;
    }

    return _AddPlayerAccount(username, password, email);
}

ResetPlayerAccount(playerid)
{
    // Reset player account's ID.
    PlayerAccount[playerid][AID] = 0;
    // Reset player's character active.
    PlayerAccount[playerid][CharacterActive] = -1;

    // Reset each player character's account ID and character ID.
    for (new c = 0; c < MAX_CHARACTERS_PER_ACCOUNT; c++)
    {
        PlayerData[playerid][c][AID] = 0;
        PlayerData[playerid][c][CID] = 0;
    }

    // Remove player from logged in player's iteration.
    Iter_Remove(LoggedInPlayers, playerid);
}

remotefunc bool:IsPlayerLoggedIn(playerid)
{
    return PlayerAccount[playerid][AID] < 1 ? false : true;
}

remotefunc bool:IsPlayerCharacterSelected(playerid)
{
    return PlayerAccount[playerid][CharacterActive] < 0 ? false : true;
}

remotefunc GetPlayerData(playerid, const string:data[])
{
    // Check the playerid's connection.
    if (!IsPlayerConnected(playerid))
    {
        return E_PLAYER_NOT_CONNECTED;
    }

    // Check IF the player has logged in.
    if (!localfunc IsPlayerLoggedIn(playerid))
    {
        return E_PLAYER_NOT_LOGGED_IN;
    }

    // Check IF data name is filled.
    if (strlen(data) < 1)
    {
        return E_PLAYER_DATA_NAME_INVALID;
    }

    // Check IF the player has a selected character.
    if (!localfunc IsPlayerCharacterSelected(playerid))
    {
        return E_PLAYER_CHARACTER_INVALID;
    }

    // Get character active.
    new CharActive = PlayerAccount[playerid][CharacterActive];

    // Set player data to variable depends on its name.
    if (strcmp(data, "name", true) == 0)
    {
        return PlayerData[playerid][CharActive][Name];
    }
    else if (strcmp(data, "money", true) == 0)
    {
        return PlayerData[playerid][CharActive][Money];
    }
    else if (strcmp(data, "score", true) == 0)
    {
        return PlayerData[playerid][CharActive][Score];
    }
    else if (strcmp(data, "kills", true) == 0)
    {
        return PlayerData[playerid][CharActive][Kills];
    }
    else if (strcmp(data, "deaths", true) == 0)
    {
        return PlayerData[playerid][CharActive][Deaths];
    }
    else if (strcmp(data, "updatedat", true) == 0)
    {
        return PlayerData[playerid][CharActive][UpdatedAt];
    }
    
    // Return invalid code IF the name doesn't exist.
    return E_PLAYER_DATA_NAME_INVALID;
}

remotefunc SetPlayerData(playerid, const string:data[], const string:value[])
{
    // Check the playerid's connection.
    if (!IsPlayerConnected(playerid))
    {
        return E_PLAYER_NOT_CONNECTED;
    }

    // Check IF the player has logged in.
    if (!localfunc IsPlayerLoggedIn(playerid))
    {
        return E_PLAYER_NOT_LOGGED_IN;
    }

    // Check IF data name is filled.
    if (strlen(data) < 1)
    {
        return E_PLAYER_DATA_NAME_INVALID;
    }

    // Check IF the player has a selected character.
    if (!localfunc IsPlayerCharacterSelected(playerid))
    {
        return E_PLAYER_CHARACTER_INVALID;
    }

    // Get character active.
    new CharActive = PlayerAccount[playerid][CharacterActive];

    // Set player data to variable depends on its name.
    if (strcmp(data, "name", true) == 0)
    {
        format(PlayerData[playerid][CharActive][Name], MAX_PLAYER_NAME, value);
    }
    else if (strcmp(data, "money", true) == 0)
    {
        PlayerData[playerid][CharActive][Money] = strval(value);
    }
    else if (strcmp(data, "score", true) == 0)
    {
        PlayerData[playerid][CharActive][Score] = strval(value);
    }
    else if (strcmp(data, "kills", true) == 0)
    {
        PlayerData[playerid][CharActive][Kills] = strval(value);
    }
    else if (strcmp(data, "deaths", true) == 0)
    {
        PlayerData[playerid][CharActive][Deaths] = strval(value);
    }
    else if (strcmp(data, "updatedat", true) == 0)
    {
        PlayerData[playerid][CharActive][UpdatedAt] = strval(value);
    }
    else
    {
        // Return invalid code IF the name doesn't exist.
        return E_PLAYER_DATA_NAME_INVALID;
    }

    // Return 1 IF success.
    return 1;
}

remotefunc LoadPlayerCharacter(playerid)
{
    // Check the playerid's connection.
    if (!IsPlayerConnected(playerid))
    {
        return E_PLAYER_NOT_CONNECTED;
    }

    // Check IF the player has logged in.
    if (!localfunc IsPlayerLoggedIn(playerid))
    {
        return E_PLAYER_NOT_LOGGED_IN;
    }

    return _LoadPlayerCharacter(playerid);
}

remotefunc LogInPlayerAccount(playerid, const string:username[], const string:password[])
{
    #if !defined RUN_TESTS
    // Check the playerid's connection.
    if (!IsPlayerConnected(playerid))
    {
        return E_PLAYER_NOT_CONNECTED;
    }
    #endif

    // Check IF the player has already logged in.
    if (localfunc IsPlayerLoggedIn(playerid))
    {
        return E_PLAYER_ALREADY_LOGGED_IN;
    }

    // Username must be filled.
    if (strlen(username) < 1)
    {
        return E_USERNAME_REQUIRED;
    }

    // Password must be filled.
    if (strlen(password) < 1)
    {
        return E_PASSWORD_REQUIRED;
    }

    // Password's length must be more than MIN_PASSWORD_LENGTH characters.
    if (strlen(password) < MIN_PASSWORD_LENGTH)
    {
        return E_INSUFFICIENT_PASSWORD_LENGTH;
    }

    // Password's length must be less than MAX_PASSWORD_LENGTH characters.
    if (strlen(password) > MAX_PASSWORD_LENGTH)
    {
        return E_EXCEED_PASSWORD_LENGTH;
    }

    // Check username and password match.
    if (!IsPasswordValid(username, password))
    {
        return E_INVALID_CREDENTIAL;
    }

    return _LoadPlayerAccount(playerid, username);
}

remotefunc LogOutFromPlayerAccount(playerid)
{
    #if defined RUN_TESTS
    // Check the playerid's connection.
    if (!IsPlayerConnected(playerid))
    {
        return E_PLAYER_NOT_CONNECTED;
    }
    #endif

    // Check IF the player has logged in.
    if (!localfunc IsPlayerLoggedIn(playerid))
    {
        return E_PLAYER_NOT_LOGGED_IN;
    }

    // Reset player's account.
    ResetPlayerAccount(playerid);
    return 1;
}

remotefunc SavePlayerCharacterData(playerid)
{
    // Check the playerid's connection.
    if (!IsPlayerConnected(playerid))
    {
        return E_PLAYER_NOT_CONNECTED;
    }

    // Check IF the player has logged in.
    if (!localfunc IsPlayerLoggedIn(playerid))
    {
        return E_PLAYER_NOT_LOGGED_IN;
    }

    // Check IF the player already select a valid character.
    if (!localfunc IsPlayerCharacterSelected(playerid))
    {
        return E_PLAYER_CHARACTER_INVALID;
    }

    new 
        // Convert the timestamp to string.
        timestamp[11];

    format(timestamp, sizeof(timestamp), "%i", gettime());
    // Set UpdatedAt data to current time.
    localfunc SetPlayerData(playerid, "UpdatedAt", timestamp);

    // Save the player current character's data.
    return _SavePlayerCharacterData(playerid);
}

#include <YSI-Coding/y_hooks>