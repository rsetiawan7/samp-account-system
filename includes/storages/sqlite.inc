#include <YSI_Core\y_debug>

#if defined STORAGE_METHOD_INCLUDED
    #error "You cannot include SQLite's storage method because one of storage method already defined. Please choose ONLY one method."
#else
    #define STORAGE_METHOD_INCLUDED
#endif

new
    // For handling DB connection.
    DB:db_account;

InitAccountSystem()
{
    P:4("[SQLITE] InitAccountSystem called.");

    // Read .db file.
    db_account = db_open("account-system.db");

    if (db_account == DB:0)
    {
        // There's something error IF the script cannot connect to DB file.
        print("Failed to open a connection to \"account-system.db\".");
    }
    else {
        // Success connect to DB file.
        print("Successfully created a connection to \"account-system.db\".");

        // Run query to create `accounts` tables IF not available.
        db_free_result(db_query(db_account, "\
        CREATE TABLE IF NOT EXISTS accounts (\
        id INTEGER PRIMARY KEY ASC AUTOINCREMENT,\
        name STRING (42) UNIQUE NOT NULL,\
        password TEXT NOT NULL,\
        salt TEXT NOT NULL,\
        email STRING (128) UNIQUE NOT NULL,\
        created_at INTEGER NOT NULL DEFAULT (CAST (strftime('%s', 'now') AS INT)),\
        updated_at INTEGER NOT NULL DEFAULT (CAST (strftime('%s', 'now') AS INT)),\
        activated_at INTEGER);"));

        // Run query to create `characters` tables IF not available.
        db_free_result(db_query(db_account, "\
        CREATE TABLE IF NOT EXISTS characters (\
        id INTEGER PRIMARY KEY ASC AUTOINCREMENT,\
        account INTEGER REFERENCES accounts (id) ON DELETE SET NULL ON UPDATE CASCADE NOT NULL,\
        name STRING (24) NOT NULL UNIQUE,\
        money INTEGER NOT NULL DEFAULT (0),\
        score INTEGER NOT NULL DEFAULT (0),\
        kills INTEGER NOT NULL DEFAULT (0),\
        deaths INTEGER NOT NULL DEFAULT (0),\
        created_at INTEGER NOT NULL DEFAULT (CAST (strftime('%s', 'now') AS INT)),\
        updated_at INTEGER NOT NULL DEFAULT (CAST (strftime('%s', 'now') AS INT)));"));

        // Run query to create `characters` tables IF not available.
        db_free_result(db_query(db_account, "\
        CREATE TABLE IF NOT EXISTS settings (\
        name STRING (32) PRIMARY KEY NOT NULL,\
        description TEXT NOT NULL,\
        value TEXT NOT NULL);"));

        new
            // Store query result from settings table.
            DBResult:setting;

        // Get value where name = 'salt'.
        setting = db_query(db_account, "SELECT `value` FROM `settings` WHERE `name` = 'salt';");

        if (db_num_rows(setting) > 0)
        {
            // Load the value for password's salt.
            db_get_field_assoc(setting, "value", gSalt, sizeof(gSalt));

            // Frees the result.
            db_free_result(setting);
        }
        else
        {
            // Frees the result before create insert new row.
            db_free_result(setting);

            static 
                // Format the query first.
                query[216];

            // Generate a random string for salt.
            GenerateRandomString(gSalt, sizeof(gSalt));

            // Prepare the query.
            format(query, sizeof(query), "INSERT INTO `settings` VALUES ('salt', 'Generate password more unique', '%q');", gSalt);
            // Create a setting for salt IF not exist.
            db_free_result(db_query(db_account, query));
        }
    }

    // Return 1 IF success.
    return 1;
}

CloseAccountSystem()
{
    // Just close the DB connection IF its a valid connection.
    if (db_account) db_close(db_account);
}

bool:IsUsernameAvailable(const string:username[])
{
    // Store query result.
    static
        query[128],
        DBResult:result,
        bool:IsAvailable;

    // Format the query and escape the string to avoid SQL injection.
    format (query, sizeof(query), "SELECT `id` FROM `accounts` WHERE `name` = '%q';", username);
    P:4("[SQLITE] IsUsernameAvailable -> query: %s", query);
    // Do the query.
    result = db_query(db_account, query);
    // Check if the query result return a valid result.
    IsAvailable = db_num_rows(result) > 0;
    // Frees the result
    db_free_result(result);

    return IsAvailable;
}

bool:IsPasswordValid(const username[], const password[])
{
    // Store query result.
    static
        query[128],
        // Per-user salt.
        PrivateSalt[MAX_SALT_LENGTH + 1],
        // Per-user hashed password.
        HashedPass[WHIRLPOOL_LENGTH + 1],
        // Hashed password from input password, global salt, and its own salt.
        CurrentHash[WHIRLPOOL_LENGTH + 1],
        DBResult:result;

    P:4("[SQLITE] IsPasswordValid -> is username available: %d", IsUsernameAvailable(username));
    // Check username availability.
    if (!IsUsernameAvailable(username))
    {
        return false;
    }

    // Format the query and escape the string to avoid SQL injection.
    format (query, sizeof(query), "SELECT `password`, `salt` FROM `accounts` WHERE `name` = '%q';", username);
    // Do the query.
    result = db_query(db_account, query);
    P:4("[SQLITE] IsPasswordValid -> query: %s", query);

    // Get the hashed password from the query result.
    db_get_field_assoc(result, "password", HashedPass, sizeof(HashedPass));
    P:4("[SQLITE] IsPasswordValid -> hashed password: %s", HashedPass);
    // Get private salt from the query result.
    db_get_field_assoc(result, "salt", PrivateSalt, sizeof(PrivateSalt));
    P:4("[SQLITE] IsPasswordValid -> private salt: %s", PrivateSalt);

    // Frees the result
    db_free_result(result);

    // Hash the input password by its own salt.
    CurrentHash = HashPassword(password, PrivateSalt);
    P:4("[SQLITE] IsPasswordValid -> hashed input password: %s", CurrentHash);

    // Verify the hash by compare the string.
    P:4("[SQLITE] IsPasswordValid -> hashed input password: %s", (strcmp(CurrentHash, HashedPass) == 0 ? "true" : "false"));
    return (strcmp(CurrentHash, HashedPass) == 0);
}

_LoadPlayerCharacter(playerid)
{
    // Store query result.
    static
        query[128],
        bool:IsAvailable,
        DBResult:result;

    // Format the query and escape the string to avoid SQL injection.
    format (query, sizeof(query), "SELECT * FROM `characters` WHERE `account` = '%i' ORDER BY `id` ASC LIMIT 0, %i;", PlayerAccount[playerid][AID], MAX_CHARACTERS_PER_ACCOUNT);
    // Do the query.
    result = db_query(db_account, query);
    // Check if the query result return a valid result.
    IsAvailable = db_num_rows(result) < 1 ? false : true;

    // Return 1 IF query result not found.
    if (!IsAvailable)
    {
        // Frees the result
        db_free_result(result);
        return 1;
    }
    
    // Store number of characters available.
    new c = 0;
    // Loop each row result.
    while(db_next_row(result))
    {
        // Assign player character's data to variable
        db_get_field_assoc(result, "name", PlayerData[playerid][c][Name], MAX_PLAYER_NAME);
        PlayerData[playerid][c][Money] = db_get_field_assoc_int(result, "money");
        PlayerData[playerid][c][Score] = db_get_field_assoc_int(result, "score");
        PlayerData[playerid][c][Kills] = db_get_field_assoc_int(result, "kills");
        PlayerData[playerid][c][Deaths] = db_get_field_assoc_int(result, "deaths");
        PlayerData[playerid][c][CreatedAt] = db_get_field_assoc_int(result, "created_at");
        PlayerData[playerid][c][UpdatedAt] = db_get_field_assoc_int(result, "updated_at");

        // Assign player account's ID and character's ID set to last sequential to make sure required data is loaded properly.
        PlayerData[playerid][c][CID] = db_get_field_assoc_int(result, "id");
        PlayerData[playerid][c][AID] = db_get_field_assoc_int(result, "account");

        // Next to the character's slot.
        c++;
    }

    // Frees the result
    db_free_result(result);

    // Return number of characters available in that account.
    return 0;
}

_CreatePlayerAccount(const username[], const password[], const email[])
{
    const queryLen = MAX_ACCOUNT_NAME_LENGTH
                    + WHIRLPOOL_LENGTH
                    + MAX_SALT_LENGTH
                    + MAX_EMAIL_LENGTH
                    + 128;

    P:4("_CreatePlayerAccount called. Params: %s %s %s", username, password, email);

    // Store query result.
    static
        query[queryLen],
        salt[MAX_SALT_LENGTH + 1],
        bool:IsAvailable,
        DBResult:result;

    // Format the query and escape the string to avoid SQL injection.
    format (query, sizeof(query), "SELECT * FROM `accounts` WHERE `name` = '%q';", username);
    P:4("_CreatePlayerAccount -> query to check available user: %s", query);
    // Do the query.
    result = db_query(db_account, query);
    P:4("_CreatePlayerAccount -> query result: %i rows", db_num_rows(result));
    // Check if the query result return a valid result.
    IsAvailable = db_num_rows(result) < 1 ? false : true;
    // Frees the result
    db_free_result(result);

    // Return 0 IF query result found.
    if (IsAvailable)
    {
        return E_USERNAME_EXIST;
    }

    // Format the query and escape the string to avoid SQL injection.
    format (query, sizeof(query), "SELECT * FROM `accounts` WHERE `email` = '%q';", email);
    P:4("_CreatePlayerAccount -> query to check available email: %s", query);
    // Do the query.
    result = db_query(db_account, query);
    P:4("_CreatePlayerAccount -> query result: %i rows", db_num_rows(result));
    // Check if the query result return a valid result.
    IsAvailable = db_num_rows(result) < 1 ? false : true;
    // Frees the result
    db_free_result(result);

    // Return 0 IF query result found.
    if (IsAvailable)
    {
        return E_EMAIL_EXIST;
    }

    // Generate a random string for salt.
    GenerateRandomString(salt, sizeof(salt));

    // Make the query.
    format (query, sizeof(query), "INSERT INTO `accounts` (`name`,`password`,`salt`,`email`) VALUES ('%q','%q','%q','%q');", username, HashPassword(password, salt), salt, email);
    P:4("_CreatePlayerAccount -> query to insert a new account: %s", query);

    result = db_query(db_account, query);
    P:4("_CreatePlayerAccount -> db_num_rows: %i", db_num_rows(result));
    db_free_result(result);

    return 1;
}

_LoadPlayerAccount(playerid, const username[])
{
    // Store query result.
    static
        query[128],
        bool:IsAvailable,
        DBResult:result;

    // Format the query and escape the string to avoid SQL injection.
    format (query, sizeof(query), "SELECT * FROM `accounts` WHERE `name` = '%q';", username);
    // Do the query.
    result = db_query(db_account, query);
    // Check if the query result return a valid result.
    IsAvailable = db_num_rows(result) < 1 ? false : true;

    // Return 0 IF query result not found.
    if (!IsAvailable)
    {
        // Frees the result
        db_free_result(result);
        return 0;
    }
    
    // Assign player account's data to variable.
    db_get_field_assoc(result, "name", PlayerAccount[playerid][Name], MAX_ACCOUNT_NAME_LENGTH);
    db_get_field_assoc(result, "email", PlayerAccount[playerid][eMail], MAX_EMAIL_LENGTH);
    PlayerAccount[playerid][CreatedAt] = db_get_field_assoc_int(result, "created_at");
    PlayerAccount[playerid][UpdatedAt] = db_get_field_assoc_int(result, "updated_at");
    PlayerAccount[playerid][ActivatedAt] = db_get_field_assoc_int(result, "activated_at");

    // Assign player account's ID set to last sequential to make sure required data is loaded properly.
    PlayerAccount[playerid][AID] = db_get_field_assoc_int(result, "id");

    // Frees the result
    db_free_result(result);

    // Add player to logged in player's iteration.
    Iter_Add(LoggedInPlayers, playerid);

    // Return 1 IF the function operated successfully.
    return 1;
}

_SavePlayerCharacterData(playerid)
{
    // Store query result.
    static
        query[128],
        // IsSuccess,
        DBResult:result;

    new CA = PlayerAccount[playerid][CharacterActive];

    // Format the query and escape the string to avoid SQL injection.
    format (query, sizeof(query), "UPDATE `characters` SET \
    `name` = '%q', `money` = '%i', `score` = '%i', `kills` = '%i', `deaths` = '%i', `updated_at` = '%i' \
    WHERE `id` = '%i';"
    , PlayerData[playerid][CA][Name]
    , PlayerData[playerid][CA][Money]
    , PlayerData[playerid][CA][Score]
    , PlayerData[playerid][CA][Kills]
    , PlayerData[playerid][CA][Deaths]
    , PlayerData[playerid][CA][UpdatedAt]
    , PlayerData[playerid][CA][CID]);
    // Do the query.
    result = db_query(db_account, query);
    // Check if the query result return a valid result.
    // IsSuccess = db_num_rows(result);

    // Frees the result
    db_free_result(result);

    // Return the number of affected row.
    return 0;
}