/*
	ecmd.inc - A rewritten version of the original ecmd.inc.

	Current Version: v0.3.1 (11/10/2012)

	So yeah, I had to rewrite the code. But why? There were a lot of bugs found,
	especially with the built-in parameter system, and eventually, the script
	would sometimes crash!

	Therefore, I've rewritten the include with much more optimization.
	You can still use OnPlayerCommandText and ecmd.inc simultaneously.

	If you're converting your script from zcmd to ecmd, make sure you
	merge OnPlayerCommandReceived and OnPlayerCommandPerformed into
	OnPlayerCommandText!

	This Source Code Form is subject to the terms of the Mozilla Public
 	License, v. 2.0. If a copy of the MPL was not distributed with this
 	file, You can obtain one at [url]http://mozilla.org/MPL/2.0/[/url].
*/

#include <a_samp>

#define ECMD:%0(%1,%2) \
	forward command_%0(%1, %2); public command_%0(%1, %2)

#if !defined CMD
	#define CMD:%0(%1,%2) ECMD:%0(%1,%2)
#endif

#if !defined isnull
	#define isnull(%1) \
		((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

stock str_extract(const string[], const start, const end)
{
	new
		szReturn[128];
	strmid(szReturn, string, start, end, 128);
	return szReturn;
}

stock lowercase(const string[])
{
	new
		szReturn[128];
	strcat(szReturn, string);
    for (new iIter, iLen = strlen(szReturn); iIter < iLen; iIter++)
	{
	    if (szReturn[iIter] < 'A' || szReturn[iIter] > 'Z') continue;
		szReturn[iIter] = tolower(szReturn[iIter]);
	}
	return szReturn;
}

stock Param_Add(const szString[], const chDelim, const szSpecifiers[], {Float,_}:...)
{
	if (isnull(szString))
	    return 0;

	if (numargs() - 3 != strlen(szSpecifiers))
	{
	    printf("ERROR: Argument count(%d) doesn't match up with the specifiers: %s(%d)", numargs() - 3, szSpecifiers, strlen(szSpecifiers));
		return 0;
	}
	new
	    iCount = strlen(szSpecifiers),
	    iDelimCount,
		iSpecifierPos,
		iNumeric,
		iUserCount,
		iUserID,
		iArgumentPos = 3,
		szDelim[2],
		szPlayer[24],
		szParams[244],
		szExtract[144];

	format(szDelim, 2, "%c", chDelim);
	strcat(szParams, szString, sizeof(szParams));
	strcat(szParams, szDelim, sizeof(szParams));
	if (szSpecifiers[strlen(szSpecifiers) - 1] != 's')
	{
		for (new i = 0; i < strlen(szString); i ++)
			if (szString[i] == chDelim) iDelimCount ++;

	    if (iDelimCount + 1 != iCount)
	        return 0;
	}
	else
	{
		for (new i = 0; i < strlen(szString); i ++)
			if (szString[i] == chDelim) iDelimCount ++;

		if (!(iDelimCount + 1 >= iCount))
		    return 0;
	}
	for (new iIter = 0; iIter < strlen(szParams); iIter ++)
	{
	    if (iIter < 0) break;
		if (szParams[iIter] == chDelim)
		{
			strmid(szExtract, szParams, 0, iIter);
			strdel(szParams, 0, iIter + 1);
			switch (szSpecifiers[iSpecifierPos++])
			{
			    case 'd', 'i': setarg(iArgumentPos, .index = 0, .value = strval(szExtract));
			    case 'f': setarg(iArgumentPos, .index = 0, .value = _:floatstr(szExtract));
			    case 'c': setarg(iArgumentPos, .index = 0, .value = szExtract[0]);
			    case 'u':
				{
					for (new i = 0; i < strlen(szExtract); i ++)
					{
						if (szExtract[i] >= '0' && szExtract[i] <= '9') iNumeric = 1;
						else
					 	{
					 	    iNumeric = 0;
						 	break;
					 	}
					}
					if (iNumeric)
					{
					    switch (IsPlayerConnected(strval(szExtract)))
						{
						    case 0: setarg(iArgumentPos, .index = 0, .value = INVALID_PLAYER_ID);
							case 1: setarg(iArgumentPos, .index = 0, .value = strval(szExtract));
						}
					}
					else
					{
						for (new i = 0; i < MAX_PLAYERS; i ++)
						{
						    if (!IsPlayerConnected(i)) continue;
							GetPlayerName(i, szPlayer, sizeof(szPlayer));
							if (strfind(szPlayer, szExtract, false) != -1)
							{
							    iUserCount = iUserCount + 1;
							    iUserID = i;
							}
						}
						setarg(iArgumentPos, .index = 0, .value = (iUserCount == 1) ? (IsPlayerConnected(iUserID)) ? (iUserID) : (INVALID_PLAYER_ID) : (INVALID_PLAYER_ID));
			        }
				}
			    case 's':
			    {
					if (iSpecifierPos == strlen(szSpecifiers))
					{
					    strcat(szExtract, szDelim);
						strins(szParams, szExtract, 0);
					    strmid(szExtract, szParams, 0, strlen(szParams));
					    strdel(szParams, 0, strlen(szParams));
					}
					for (new iLoop = 0, iIndex = 0; iLoop < strlen(szExtract); iLoop ++)
					{
						setarg(iArgumentPos, iIndex++, szExtract[iLoop]);
					}
				}
			}
			iArgumentPos ++;
			iIter -= strlen(szExtract);
		}
	}
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	new szCallName[128]; // 128 cells so we can fix an array out-of-bounds error that can be caused by sending large strings to the callback.
 	format(szCallName, sizeof(szCallName), "command_%s", lowercase(str_extract(cmdtext, 1, (strfind(cmdtext, " ") != -1) ? (strfind(cmdtext, " ")) : strlen(cmdtext))));
	if (strlen(str_extract(szCallName, 8, strlen(szCallName))) > 31)
	{
	    new
			szPlayerName[MAX_PLAYER_NAME],
			iLength = strlen(szCallName);

	    GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
	    strdel(szCallName, 0, 8);
		strdel(szCallName, 20, iLength);
	    printf("Error: Command '%s...' sent by '%s' too large to process (characters: %d/31)", szCallName, szPlayerName, (iLength - 8));
		return 0;
	}
	if (funcidx(szCallName) != -1)
	{
		new szParameters[128];
		if (strfind(cmdtext, " ") != -1)
			strmid(szParameters, cmdtext, strfind(cmdtext, " ") + 1, strlen(cmdtext));
			
		if (isnull(szParameters))
		    CallLocalFunction(szCallName, "ds", playerid, "\1");

		else
			CallLocalFunction(szCallName, "ds", playerid, szParameters);
		return 1;
	}
	return CallLocalFunction("ecmd_OnPlayerCommandText", "ds", playerid, cmdtext);
}

#if defined _ALS_OnPlayerCommandText
	#undef OnPlayerCommandText
#else
	#define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText ecmd_OnPlayerCommandText
forward ecmd_OnPlayerCommandText(playerid, cmdtext[]);