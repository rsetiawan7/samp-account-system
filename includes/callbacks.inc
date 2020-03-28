#include <YSI_Coding\y_hooks>

// hook OnFilterScriptInit()
hook OnScriptInit()
{
    // Initialize iteration for logged in players.
    Iter_Init(LoggedInPlayers);

    // Initialize the system. (depends on storage method)
    InitAccountSystem();
    // db_debug_openfiles();
    // SetTimer("InitAccountSystem", 2000, false);

    print("\n----------------------------------");
    print("       Account System\n");
    print("         Written by");
    print("         rsetiawan7");
    print("----------------------------------\n");
    return 1;
}

// hook OnFilterScriptExit()
// hook OnScriptExit()
// {
//     P:4("Start to save logged in's player data");

//     // Loop each logged in players.
//     foreach (new playerid : LoggedInPlayers)
//     {
//         P:4("Saving player data with Player ID: %i ...", playerid);

//         // Save each player's character that logged in and selected a character.
//         new saveable = SavePlayerCharacterData(playerid);

//         P:4("Save player data with Player ID: %i result --> %i", saveable);
//     }

//     // Close the connection to database.
//     CloseAccountSystem();

//     return 1;
// }