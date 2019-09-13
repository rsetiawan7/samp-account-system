// SA-MP Core
#include <a_samp>

// Debug level. Comment line below to disable debugging.
#define _DEBUG 4

// Defines storage method
#define USE_SQLITE

// Third-party includes
#include <hash>
#include <YSI-Core/y_debug>
#include <YSI-Data/y_iterate>
#include <YSI-Coding/y_remote>
#include <YSI-Coding/y_timers>
#include <YSI-Visual/y_commands>

// Commands
#include "includes/commands.inc"

// Main Function
main()
{
  print("\n----------------------------------");
  print("       Account System\n");
  print("         Written by");
  print("         rsetiawan7");
  print("----------------------------------\n");
}

// Functions
#include "includes/functions.inc"

// Callbacks
// #include "lsdm/callbacks.inc"

// Timers
// #include "lsdm/timers.inc"