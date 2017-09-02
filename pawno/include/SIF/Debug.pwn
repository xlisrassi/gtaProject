/*==============================================================================

Southclaw's Interactivity Framework (SIF) (Formerly: Adventure API)

	SIF Version: 1.3.0
	Module Version: 2.1.1


	SIF/Overview
	{
		SIF is a collection of high-level include scripts to make the
		development of interactive features easy for the developer while
		maintaining quality front-end gameplay for players.
	}

	SIF/Debug/Description
	{
		Basic debugging that can be activated/deactivated during runtime.
	}

	SIF/Debug/Dependencies
	{
		YSI\y_hooks
		YSI\y_va
	}

	SIF/Debug/Credits
	{
		SA:MP Team						- Amazing mod!
		SA:MP Community					- Inspiration and support
		Incognito						- Very useful streamer plugin
		Y_Less							- YSI framework
	}

	SIF/Debug/Constants
	{
		These can be altered by defining their values before the include line.

		MAX_DEBUG_HANDLER
			Maximum debug handlers that can be registered.

		MAX_DEBUG_HANDLER_NAME
			Debug handler name length limit.
	}

	SIF/Debug/Core Functions
	{
		The functions that control the core features of this script.

		native -
		native - SIF/Debug/Core
		native -

		native sif_debug_register_handler(name[], initstate = 0)
		{
			Description:
				Registers a new debug handler. A debug handler is an identifier
				for a collection of debug messages. A handler can be activated
				and deactivated during runtime allowing for instant debugging of
				problems found while using a script.

			Parameters:
				<name> (string)
					The name given to the handler. This can also be used to
					search for handlers so developers don't need to remember
					numerical identifiers.

				<initstate> (int)
					Initial debug level of the handler. Default value of zero
					and is generally left this way unless debugging something
					that happens on script initiation.

			Returns:
				(int)
					A debug handler identifier.

				-1
					If the debug handler limit was reached.
		}

		native sif_debug_level(handler, level)
		{
			Description:
				Sets the debug level for a specific debug handler.

			Parameters:
				<handler> (int)
					Debug handler ID to set the level of.

				<level> (int)
					Debug level to set. Higher values generally mean more
					information from debug prints.

			Returns:
				(none)
		}

		native sif_debug_plevel(playerid, handler, level)
		{
			Description:
				(scheduled for depreciation)

			Parameters:
				-

			Returns:
				-
		}

		native sif_debug_print(handler, level, playerid, msg[])
		{
			Description:
				Prints a debug message on the specified handler for the
				specified level. If the level is higher than the active level
				for the handler, nothing will happen.

			Parameters:
				<handler> (int)
					Handler ID.

				<level> (int)
					Debug level to print at.

				<playerid> (int, playerid)
					Player to send a client message to
					(scheduled for depreciation).

				<msg> (string)
					Message to send.

			Returns:
				-
		}

		native sif_debug_printf(handler, level, playerid, string[], va_args<>)
		{
			Description:
				Prints a formatted debug message on the specified handler for
				the	specified level. If the level is higher than the active
				level for the handler, nothing will happen.

			Parameters:
				<handler> (int)
					Handler ID.

				<level> (int)
					Debug level to print at.

				<playerid> (int, playerid)
					Player to send a client message to
					(scheduled for depreciation).

				<msg> (string)
					Message to send.

				<va_args> (variable arguments)
					String format parameters.

			Returns:
				-
		}

		native sif_debug_handler_search(name[])
		{
			Description:
				Searches for a debug handler name and returns the ID.

			Parameters:
				-

			Returns:
				(int)
					A valid debug handler ID if found.

				-1
					If no handler was found from the specified string.
		}

		native sif_debug_get_handler_name(handler, output[])
		{
			Description:
				Returns a debug handler name for the specified ID.

			Parameters:
				-

			Returns:
				0
					If the ID was invalid.
		}
	}

	SIF/Debug/Hooks
	{
		Hooked functions or callbacks, either SA:MP natives or from other
		scripts or plugins.

		SAMP/OnPlayerConnect
		{
			Reason:
				Zeroing some variables and resetting stuff.
		}
	}

==============================================================================*/


#if defined _SIF_DEBUG_INCLUDED
	#endinput
#endif

#include <YSI\y_hooks>
#include <YSI\y_va>

#define _SIF_DEBUG_INCLUDED


/*==============================================================================

	Setup

==============================================================================*/


#if !defined MAX_DEBUG_HANDLER
	#define MAX_DEBUG_HANDLER			(128)
#endif

#if !defined MAX_DEBUG_HANDLER_NAME
	#define MAX_DEBUG_HANDLER_NAME		(32)
#endif

#define SIF_IS_VALID_HANDLER(%0)		(0 <= %0 < dbg_Total)
#define sif_dp:%0:%1(%2)<%3>			sif_debug_printf(%1,%0,%3,%2)
#define sif_d:%0:%1(%2)					sif_debug_printf(%1,%0,INVALID_PLAYER_ID,%2)


enum
{
	SIF_DEBUG_LEVEL_NONE,
	SIF_DEBUG_LEVEL_CALLBACKS,
	SIF_DEBUG_LEVEL_CALLBACKS_DEEP,
	SIF_DEBUG_LEVEL_CORE,
	SIF_DEBUG_LEVEL_CORE_DEEP,
	SIF_DEBUG_LEVEL_INTERNAL,
	SIF_DEBUG_LEVEL_INTERNAL_DEEP,
	SIF_DEBUG_LEVEL_INTERFACE,
	SIF_DEBUG_LEVEL_INTERFACE_DEEP,
	SIF_DEBUG_LEVEL_LOOPS
}

static const DEBUG_PREFIX[32] = "$ ";


static
		dbg_Name[MAX_DEBUG_HANDLER][MAX_DEBUG_HANDLER_NAME],
		dbg_Level[MAX_DEBUG_HANDLER] = {255, 0, 0, ...}, // set handler 0 to 255
		dbg_PlayerLevel[MAX_DEBUG_HANDLER][MAX_PLAYERS],
		dbg_Total = 1; // handler 0 is global


/*==============================================================================

	Zeroing

==============================================================================*/


hook OnPlayerConnect(playerid)
{
	for(new i; i < dbg_Total; i++)
		dbg_PlayerLevel[playerid][i] = 0;
}


/*==============================================================================

	Core Functions

==============================================================================*/


stock sif_debug_register_handler(name[], initstate = 0)
{
	strcat(dbg_Name[dbg_Total], name);
	dbg_Level[dbg_Total] = initstate;

	return dbg_Total++;
}

stock sif_debug_level(handler, level)
{
	dbg_Level[handler] = level;

	return 1;
}

stock sif_debug_plevel(playerid, handler, level)
{
	if(!IsPlayerConnected(playerid))
		return 0;

	dbg_PlayerLevel[playerid][handler] = level;

	return 1;
}

stock sif_debug_print(handler, level, playerid, msg[])
{
	if(!SIF_IS_VALID_HANDLER(handler))
		return 0;

	if(playerid != INVALID_PLAYER_ID)
	{
		if(level <= dbg_PlayerLevel[playerid][handler])
		{
			new name[MAX_PLAYER_NAME];

			GetPlayerName(playerid, name, MAX_PLAYER_NAME);

			SendClientMessage(playerid, -1, msg);
		}
	}

	if(level <= dbg_Level[handler])
	{
		printf("%s[%s:%d]: %s", DEBUG_PREFIX, dbg_Name[handler], level, msg);
	}

	return 1;
}

stock sif_debug_printf(handler, level, playerid, string[], va_args<>)
{
	if(!SIF_IS_VALID_HANDLER(handler))
		return 0;

	if(dbg_Level[handler] < level)
		return 0;

	if(playerid != INVALID_PLAYER_ID && dbg_PlayerLevel[playerid][handler] < level)
		return 0;

	new str[256];
	va_format(str, sizeof(str), string, va_start<4>);
	sif_debug_print(handler, level, playerid, str);

	return 1;
}

stock sif_debug_handler_search(name[])
{
	new bestmatch = -1;

	// Needs a better search algorithm...
	for(new i; i < dbg_Total; i++)
	{
		if(strfind(dbg_Name[i], name, true) != -1)
		{
			bestmatch = i;
			break;
		}
	}

	return bestmatch;
}

stock sif_debug_get_handler_name(handler, output[])
{
	if(!SIF_IS_VALID_HANDLER(handler))
		return 0;

	output[0] = EOS;
	strcat(output, dbg_Name[handler], MAX_DEBUG_HANDLER_NAME);

	return 1;
}
