// SA-MP Core
#include <a_samp>

// Debug level. Comment line below to disable debugging.
#define _DEBUG 4

// y_master number to send the function must to go.
// #define MASTER 1

// Use as server because the functions that you want to call is at this script.
// #define YSI_IS_SERVER

// Defines storage method
#define USE_SQLITE

// Third-party includes
#include <hash>
#include <sscanf2>
// #include <YSI-Core/y_master>
#include <YSI-Coding/y_remote>
#include <YSI-Server/y_colours>
#include <YSI-Core/y_debug>
#include <YSI-Data/y_iterate>
#include <YSI-Coding/y_timers>
// #include <YSI-Visual/y_commands>

// Functions
#include "includes/functions.inc"

// Main Function
public OnScriptInit()
{
  print("\n----------------------------------");
  print("       Account System\n");
  print("         Written by");
  print("         rsetiawan7");
  print("----------------------------------\n");
}

// Commands
// #include "includes/commands.inc"

// Callbacks
// #include "lsdm/callbacks.inc"

// Timers
// #include "lsdm/timers.inc"