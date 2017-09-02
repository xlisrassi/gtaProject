/*--------------------------------------------------------------------------------------------------------------------*-
// Natives
// ---------------
native CreateZone();
native DeleteZone();
native GetZoneName();
native GetZonePos();
native GetZoneColor();
native GetZoneCount();
native GetPlayerZoneID();
native IsZoneActive();
native IsZoneFlashForPlayer();
native IsZoneLockedForPlayer();
native IsZoneVisibleForPlayer();
native IsZoneWeaponsAllowedForPlayer();
native IsXYInZone();
native LoadZonesFromFile();
native SaveZonesInFile();
native SetZoneColor();
native SetZoneName();
native SetZoneNote();
native SetZonePos();
native ReturnZoneName();
native ReturnZoneNote();
native ToggleZoneVisibleForPlayer();
native ToggleZoneVisibleForAll();
native ToggleZoneFlashForPlayer();
native ToggleZoneFlashForAll();
native ToggleZoneLockedForPlayer();
native ToggleZoneLockedForAll();
native ToggleZoneWeaponsForPlayer();
-*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
// Definies
// ---------------
#define MAX_ZONES (64)
#define MAX_ZONE_NAME (32)
#define INVALID_ZONE_ID (-1)
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
// Enums
// ---------------
enum samp_kowaz_zEnum
{
	samp_kowaz_zActive,
	samp_kowaz_zName[MAX_ZONE_NAME],
	samp_kowaz_zVisible[MAX_PLAYERS],
	samp_kowaz_zFlash[MAX_PLAYERS],
	samp_kowaz_zLocked[MAX_PLAYERS],
	samp_kowaz_zZone,
	samp_kowaz_zColor,
	Float:samp_kowaz_zPos[4],
	samp_kowaz_zGun[MAX_PLAYERS],
	samp_kowaz_zNote1[32],
	samp_kowaz_zNote2[32],
	samp_kowaz_zNote3[32]
};
enum samp_kowaz_zpEnum
{
	Float:samp_kowaz_zpPos[3],
	samp_kowaz_zpZone,
	samp_kowaz_zpWeapon[15],
	samp_kowaz_zpAmmo[15]
};
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
// Variables
// ---------------
static samp_kowaz_zData[MAX_ZONES][samp_kowaz_zEnum];
static samp_kowaz_zpData[MAX_PLAYERS][samp_kowaz_zpEnum];
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
// Forwards
// ---------------
forward samp_kowaz_zone_loop();
forward OnPlayerEnterZone(playerid, zoneid);
forward OnPlayerLeaveZone(playerid, zoneid);
/*--------------------------------------------------------------------------------------------------------------------*/

samp_ZoneOnGameModeInit()
{
	samp_kowaz_zone_loop();
	SetTimer("samp_kowaz_zone_loop", 500, 1);
}

samp_ZoneOnPlayerSpawn(playerid)
{
	if (!IsPlayerConnected(playerid)) return;
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	samp_kowaz_zpData[playerid][samp_kowaz_zpZone] = INVALID_ZONE_ID;
	for (new index; index < MAX_ZONES; index++)
	{
		if (samp_kowaz_zData[index][samp_kowaz_zVisible][playerid] && IsZoneActive(index))
		{
			GangZoneShowForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], samp_kowaz_zData[index][samp_kowaz_zColor]);
			if (samp_kowaz_zData[index][samp_kowaz_zFlash][playerid])
			{
				GangZoneFlashForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], 0x00000000);
			}
		}
		if (IsXYInZone(x, y, index))
		{
			samp_kowaz_zpData[playerid][samp_kowaz_zpZone] = index;
			CallRemoteFunction("OnPlayerEnterZone", "ii", playerid, index);
		}
	}
}

stock IsZoneActive(index)
{
	if (index >= 0 && index < MAX_ZONES) return samp_kowaz_zData[index][samp_kowaz_zActive];
	return 0;
}

stock IsZoneLockedForPlayer(index, playerid)
{
	if (!IsZoneActive(index) || !IsPlayerConnected(playerid)) return 0;
	return samp_kowaz_zData[index][samp_kowaz_zLocked][playerid];
}

stock IsZoneVisibleForPlayer(index, playerid)
{
	if (!IsZoneActive(index) || !IsPlayerConnected(playerid)) return 0;
	return samp_kowaz_zData[index][samp_kowaz_zVisible][playerid];
}

stock IsZoneFlashForPlayer(index, playerid)
{
	if (!IsZoneActive(index) || !IsPlayerConnected(playerid)) return 0;
	return samp_kowaz_zData[index][samp_kowaz_zFlash][playerid];
}

stock IsZoneWeaponsAllowedForPlayer(index, playerid)
{
	if (!IsZoneActive(index) || !IsPlayerConnected(playerid)) return 0;
	return samp_kowaz_zData[index][samp_kowaz_zGun][playerid];
}

stock CreateZone(name[], Float:minx, Float:miny, Float:maxx, Float:maxy, color)
{
	new index;
	for (index = 0; index < MAX_ZONES; index++) if (!IsZoneActive(index)) break;
	if (index == MAX_ZONES) return INVALID_ZONE_ID;
	samp_kowaz_zData[index][samp_kowaz_zActive] = 1;
	samp_kowaz_zData[index][samp_kowaz_zPos][0] = minx;
	samp_kowaz_zData[index][samp_kowaz_zPos][1] = miny;
	samp_kowaz_zData[index][samp_kowaz_zZone] = GangZoneCreate(minx, miny, maxx, maxy);
	samp_kowaz_zData[index][samp_kowaz_zColor] = color;
	for (new playerid; playerid < MAX_PLAYERS; playerid++)
	{
		samp_kowaz_zData[index][samp_kowaz_zVisible][playerid] = 1;
		samp_kowaz_zData[index][samp_kowaz_zFlash][playerid] = 0;
		samp_kowaz_zData[index][samp_kowaz_zLocked][playerid] = 0;
		samp_kowaz_zData[index][samp_kowaz_zGun][playerid] = 1;
	}
	samp_kowaz_zData[index][samp_kowaz_zPos][2] = maxx;
	samp_kowaz_zData[index][samp_kowaz_zPos][3] = maxy;
	GangZoneShowForAll(samp_kowaz_zData[index][samp_kowaz_zZone], color);
	strmid(samp_kowaz_zData[index][samp_kowaz_zName], name, 0, strlen(name), MAX_ZONE_NAME);
	strmid(samp_kowaz_zData[index][samp_kowaz_zNote1], "changeme", 0, strlen("changeme"), 32);
	strmid(samp_kowaz_zData[index][samp_kowaz_zNote2], "changeme", 0, strlen("changeme"), 32);
	strmid(samp_kowaz_zData[index][samp_kowaz_zNote3], "changeme", 0, strlen("changeme"), 32);
	return index;
}

stock DeleteZone(index)
{
	if (IsZoneActive(index))
	{
		samp_kowaz_zData[index][samp_kowaz_zActive] = 0;
		GangZoneDestroy(samp_kowaz_zData[index][samp_kowaz_zZone]);
		for (new playerid; playerid < MAX_PLAYERS; playerid++)
		{
			if (!IsPlayerConnected(playerid)) continue;
			for (new slot = 0; slot < 15; slot++)
			{
				if (samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] == 0) continue;
				GivePlayerWeapon(playerid, samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot], samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot]);
				samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] = 0;
				samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot] = 0;
			}
		}
		return 1;
	}
	return 0;
}

stock ToggleZoneVisibleForPlayer(index, playerid, toggle)
{
	if (!IsZoneActive(index)) return;
	if (toggle)
	{
		if (samp_kowaz_zData[index][samp_kowaz_zVisible][playerid]) return;
		GangZoneShowForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], samp_kowaz_zData[index][samp_kowaz_zColor]);
		samp_kowaz_zData[index][samp_kowaz_zVisible][playerid] = 1;
	}
	else
	{
		if (!samp_kowaz_zData[index][samp_kowaz_zVisible][playerid]) return;
		GangZoneHideForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone]);
		samp_kowaz_zData[index][samp_kowaz_zVisible][playerid] = 0;
	}
}

stock ToggleZoneVisibleForAll(index, toggle)
{
	if (!IsZoneActive(index)) return;
	if (toggle)
	{
		for (new playerid; playerid < MAX_PLAYERS; playerid++)
		{
			GangZoneShowForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], samp_kowaz_zData[index][samp_kowaz_zColor]);
			samp_kowaz_zData[index][samp_kowaz_zVisible][playerid] = 1;
		}
	}
	else
	{
		for (new playerid; playerid < MAX_PLAYERS; playerid++)
		{
			GangZoneHideForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone]);
			samp_kowaz_zData[index][samp_kowaz_zVisible][playerid] = 0;
		}
	}
}

stock ToggleZoneFlashForPlayer(index, playerid, toggle)
{
	if (!IsZoneActive(index)) return;
	if (toggle)
	{
		if (samp_kowaz_zData[index][samp_kowaz_zFlash][playerid]) return;
		GangZoneFlashForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], 0x00000000);
		samp_kowaz_zData[index][samp_kowaz_zFlash][playerid] = 1;
	}
	else
	{
		if (!samp_kowaz_zData[index][samp_kowaz_zFlash][playerid]) return;
		GangZoneStopFlashForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone]);
		samp_kowaz_zData[index][samp_kowaz_zFlash][playerid] = 0;
	}
}

stock ToggleZoneFlashForAll(index, toggle)
{
	if (!IsZoneActive(index)) return;
	if (toggle)
	{
		for (new playerid; playerid < MAX_PLAYERS; playerid++)
		{
			GangZoneFlashForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], 0x00000000);
			samp_kowaz_zData[index][samp_kowaz_zFlash][playerid] = 1;
		}
	}
	else
	{
		for (new playerid; playerid < MAX_PLAYERS; playerid++)
		{
			GangZoneStopFlashForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone]);
			samp_kowaz_zData[index][samp_kowaz_zFlash][playerid] = 0;
		}
	}
}

stock ToggleZoneLockedForPlayer(index, playerid, toggle)
{
	if (!IsZoneActive(index)) return;
	if (toggle)
	{
		if (samp_kowaz_zData[index][samp_kowaz_zLocked][playerid]) return;
		samp_kowaz_zData[index][samp_kowaz_zLocked][playerid] = 1;
	}
	else
	{
		if (!samp_kowaz_zData[index][samp_kowaz_zLocked][playerid]) return;
		samp_kowaz_zData[index][samp_kowaz_zLocked][playerid] = 0;
	}
}

stock ToggleZoneLockedForAll(index, toggle)
{
	if (!IsZoneActive(index)) return;
	if (toggle)
	{
		for (new playerid; playerid < MAX_PLAYERS; playerid++)
		{
			samp_kowaz_zData[index][samp_kowaz_zLocked][playerid] = 1;
		}
	}
	else
	{
		for (new playerid; playerid < MAX_PLAYERS; playerid++)
		{
			samp_kowaz_zData[index][samp_kowaz_zLocked][playerid] = 0;
		}
	}
}

stock ToggleZoneWeaponsForPlayer(index, playerid, toggle)
{
	if (!IsZoneActive(index)) return;
	if (toggle)
	{
		if (samp_kowaz_zData[index][samp_kowaz_zGun][playerid]) return;
		samp_kowaz_zData[index][samp_kowaz_zGun][playerid] = 1;
		if (samp_kowaz_zpData[playerid][samp_kowaz_zpZone] == index)
		{
			for (new slot = 0; slot < 15; slot++)
			{
				if (samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] == 0) continue;
				GivePlayerWeapon(playerid, samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot], samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot]);
				samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] = 0;
				samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot] = 0;
			}		
		}
	}
	else
	{
		if (!samp_kowaz_zData[index][samp_kowaz_zGun][playerid]) return;
		samp_kowaz_zData[index][samp_kowaz_zGun][playerid] = 0;
		if (samp_kowaz_zpData[playerid][samp_kowaz_zpZone] == index)
		{
			new weapon, ammo;
			for (new slot = 0; slot < 15; slot++)
			{
				GetPlayerWeaponData(playerid, slot, weapon, ammo);
				samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] = weapon;
				samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot] = ammo;
			}
			ResetPlayerWeapons(playerid);
		}
	}
}

stock SetZoneName(index, name[])
{
	if (!IsZoneActive(index)) return;
	strmid(samp_kowaz_zData[index][samp_kowaz_zName], name, 0, strlen(name), MAX_ZONE_NAME);
}

stock GetZoneName(index, name[], len)
{
	if (!IsZoneActive(index)) return;
	strmid(name, samp_kowaz_zData[index][samp_kowaz_zName], 0, strlen(samp_kowaz_zData[index][samp_kowaz_zName]), MAX_ZONE_NAME);
}

stock ReturnZoneName(index)
{
	new samp_kowaz_retname[MAX_PLAYER_NAME];
	strmid(samp_kowaz_retname, samp_kowaz_zData[index][samp_kowaz_zName], 0, strlen(samp_kowaz_zData[index][samp_kowaz_zName]), MAX_ZONE_NAME);
	return samp_kowaz_retname;
}

stock SetZoneColor(index, color)
{
	if (!IsZoneActive(index)) return;
	samp_kowaz_zData[index][samp_kowaz_zColor] = color;
	for (new playerid; playerid < MAX_PLAYERS; playerid++)
	{
		if (IsPlayerConnected(playerid) && samp_kowaz_zData[index][samp_kowaz_zVisible][playerid])
		{
			GangZoneShowForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], samp_kowaz_zData[index][samp_kowaz_zColor]);
			if (samp_kowaz_zData[index][samp_kowaz_zFlash][playerid])
			{
				GangZoneFlashForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], 0x00000000);
			}
		}
	}
}

stock GetZoneColor(index)
{
	if (!IsZoneActive(index)) return 0x00000000;
	return samp_kowaz_zData[index][samp_kowaz_zColor];
}

stock IsXYInZone(Float:X, Float:Y, index)
{
	if (X < samp_kowaz_zData[index][samp_kowaz_zPos][2] && X >= samp_kowaz_zData[index][samp_kowaz_zPos][0] && Y < samp_kowaz_zData[index][samp_kowaz_zPos][3] && Y >= samp_kowaz_zData[index][samp_kowaz_zPos][1]) return 1;
	return 0;
}

stock GetPlayerZoneID(playerid)
{
	if (!IsPlayerConnected(playerid)) return;
	return samp_kowaz_zpData[playerid][samp_kowaz_zpZone];
}

stock GetZonePos(index, &Float:minx, &Float:miny, &Float:maxx, &Float:maxy)
{
	if (!IsZoneActive(index)) return;
	minx = samp_kowaz_zData[index][samp_kowaz_zPos][0];
	miny = samp_kowaz_zData[index][samp_kowaz_zPos][1];
	maxx = samp_kowaz_zData[index][samp_kowaz_zPos][2];
	maxy = samp_kowaz_zData[index][samp_kowaz_zPos][3];
}

stock SetZonePos(index, Float:minx, Float:miny, Float:maxx, Float:maxy)
{
	if (!IsZoneActive(index)) return;
	samp_kowaz_zData[index][samp_kowaz_zPos][0] = minx;
	samp_kowaz_zData[index][samp_kowaz_zPos][1] = miny;
	samp_kowaz_zData[index][samp_kowaz_zPos][2] = maxx;
	samp_kowaz_zData[index][samp_kowaz_zPos][3] = maxy;
	GangZoneDestroy(samp_kowaz_zData[index][samp_kowaz_zZone]);
	samp_kowaz_zData[index][samp_kowaz_zZone] = GangZoneCreate(minx, miny, maxx, maxy);
	for (new playerid; playerid < MAX_PLAYERS; playerid++)
	{
		if (IsPlayerConnected(playerid) && samp_kowaz_zData[index][samp_kowaz_zVisible][playerid])
		{
			GangZoneShowForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], samp_kowaz_zData[index][samp_kowaz_zColor]);
			if (samp_kowaz_zData[index][samp_kowaz_zFlash][playerid])
			{
				GangZoneFlashForPlayer(playerid, samp_kowaz_zData[index][samp_kowaz_zZone], 0x00000000);
			}
		}
	}
}

stock GetZoneCount()
{
	new samp_kowaz_count;
	for (new i; i < MAX_ZONES; i++)
	{
		if (IsZoneActive(i)) samp_kowaz_count++;
	}
	return samp_kowaz_count;
}

public samp_kowaz_zone_loop()
{
	for (new playerid; playerid < MAX_PLAYERS; playerid++)
	{
		if (!IsPlayerConnected(playerid) || GetPlayerState(playerid) == 0 || GetPlayerState(playerid) == 9 || GetPlayerState(playerid) == 8 || GetPlayerState(playerid) == 7)
		{
			samp_kowaz_zpData[playerid][samp_kowaz_zpZone] = INVALID_ZONE_ID;
			continue;
		}
		new Float:x, Float:y, Float:z, Float:a, zoneid = samp_kowaz_zpData[playerid][samp_kowaz_zpZone];
		GetPlayerPos(playerid, x, y, z);
		if (zoneid != INVALID_ZONE_ID)
		{
			if (IsXYInZone(x, y, zoneid)) continue;
			samp_kowaz_zpData[playerid][samp_kowaz_zpZone] = INVALID_ZONE_ID;
			CallRemoteFunction("OnPlayerLeaveZone", "ii", playerid, zoneid);
			if (samp_kowaz_zData[zoneid][samp_kowaz_zGun][playerid] == 0)
			{
				for (new slot = 0; slot < 15; slot++)
				{
					if (samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] == 0) continue;
					GivePlayerWeapon(playerid, samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot], samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot]);
					samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] = 0;
					samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot] = 0;
				}
			}
		}
		for (zoneid = 0; zoneid < MAX_ZONES; zoneid++)
		{
			if (IsZoneActive(zoneid))
			{
				if (IsXYInZone(x, y, zoneid))
				{
					CallRemoteFunction("OnPlayerEnterZone", "ii", playerid, zoneid);
					if (samp_kowaz_zData[zoneid][samp_kowaz_zLocked][playerid] && x != 0.0)
					{
						if (!IsPlayerInAnyVehicle(playerid))
						{
							GetPlayerFacingAngle(playerid, a);
							SetPlayerPos(playerid, samp_kowaz_zpData[playerid][samp_kowaz_zpPos][0] - 3.0 * floatsin(-a, degrees), samp_kowaz_zpData[playerid][samp_kowaz_zpPos][1] - 1.0 * floatcos(-a, degrees), samp_kowaz_zpData[playerid][samp_kowaz_zpPos][2] + 1.5);
						}
						else
						{
							GetVehicleZAngle(playerid, a);
							SetVehiclePos(GetPlayerVehicleID(playerid), samp_kowaz_zpData[playerid][samp_kowaz_zpPos][0] - 3.0 * floatsin(-a, degrees), samp_kowaz_zpData[playerid][samp_kowaz_zpPos][1] - 1.0 * floatcos(-a, degrees), samp_kowaz_zpData[playerid][samp_kowaz_zpPos][2] + 1.5);
						}
						break;
					}
					samp_kowaz_zpData[playerid][samp_kowaz_zpZone] = zoneid;
					if (samp_kowaz_zData[zoneid][samp_kowaz_zGun][playerid] == 0)
					{
						new weapon, ammo;
						for (new slot = 0; slot < 15; slot++)
						{
							GetPlayerWeaponData(playerid, slot, weapon, ammo);
							samp_kowaz_zpData[playerid][samp_kowaz_zpWeapon][slot] = weapon;
							samp_kowaz_zpData[playerid][samp_kowaz_zpAmmo][slot] = ammo;
						}
						ResetPlayerWeapons(playerid);
					}
					break;
				}
			}
		}
		samp_kowaz_zpData[playerid][samp_kowaz_zpPos][0] = x;
		samp_kowaz_zpData[playerid][samp_kowaz_zpPos][1] = y;
		samp_kowaz_zpData[playerid][samp_kowaz_zpPos][2] = z;
	}
	return 1;
}

stock SetZoneNote(index, noteid, string[])
{
	if (!IsZoneActive(index)) return;
	switch (noteid)
	{
		case 1:
		{
			strmid(samp_kowaz_zData[index][samp_kowaz_zNote1], string, 0, strlen(string), 32);
		}
		case 2:
		{
			strmid(samp_kowaz_zData[index][samp_kowaz_zNote2], string, 0, strlen(string), 32);
		}
		case 3:
		{
			strmid(samp_kowaz_zData[index][samp_kowaz_zNote3], string, 0, strlen(string), 32);
		}
		default:
		{
			return;
		}
	}
}

stock ReturnZoneNote(index, noteid)
{
	new samp_kowaz_tmpres[32];
	if (!IsZoneActive(index)) return samp_kowaz_tmpres;
	switch (noteid)
	{
		case 1:
		{
			format(samp_kowaz_tmpres, 32, "%s", samp_kowaz_zData[index][samp_kowaz_zNote1]);
		}
		case 2:
		{
			format(samp_kowaz_tmpres, 32, "%s", samp_kowaz_zData[index][samp_kowaz_zNote2]);
		}
		case 3:
		{
			format(samp_kowaz_tmpres, 32, "%s", samp_kowaz_zData[index][samp_kowaz_zNote3]);
		}
		default:
		{
			return samp_kowaz_tmpres;
		}
	}
	return samp_kowaz_tmpres;
}

stock SaveZonesInFile(filename[])
{
	new File: zFile = fopen(filename, io_write);
	if (zFile)
	{
		new string[256], zoneid, index, r, g, b, a;
		while (zoneid < MAX_ZONES)
		{
			if (IsZoneActive(zoneid))
			{
				HexToRGBA(samp_kowaz_zData[zoneid][samp_kowaz_zColor], r, g, b, a);
				format(string, sizeof(string),"%d,%s,%.2f,%.2f,%.2f,%.2f,%s,%s,%s,%d,%d,%d,%d\n", zoneid,
				samp_kowaz_zData[zoneid][samp_kowaz_zName],
				samp_kowaz_zData[zoneid][samp_kowaz_zPos][0],
				samp_kowaz_zData[zoneid][samp_kowaz_zPos][1],
				samp_kowaz_zData[zoneid][samp_kowaz_zPos][2],
				samp_kowaz_zData[zoneid][samp_kowaz_zPos][3],
				samp_kowaz_zData[zoneid][samp_kowaz_zNote1],
				samp_kowaz_zData[zoneid][samp_kowaz_zNote2],
				samp_kowaz_zData[zoneid][samp_kowaz_zNote3],
				r, g, b, a);
				fwrite(zFile, string);
				index++;
			}
			zoneid++;
		}
		fclose(zFile);
		return 1;
	}
	return 0;
}

stock LoadZonesFromFile(filename[])
{
	if (fexist(filename))
	{
		new File: zFile = fopen(filename, io_read);
		new string[256], zoneid, index, samp_kowaz_tmpdes[13][64], r, g, b, a;
		while (fread(zFile, string, sizeof(string), false) && (index != MAX_ZONES))
		{
			strsplit(string, samp_kowaz_tmpdes, ',');
			zoneid = strval(samp_kowaz_tmpdes[0]);
			strmid(samp_kowaz_zData[zoneid][samp_kowaz_zName], samp_kowaz_tmpdes[1], 0, strlen(samp_kowaz_tmpdes[1]), MAX_ZONE_NAME);
			samp_kowaz_zData[zoneid][samp_kowaz_zPos][0] = floatstr(samp_kowaz_tmpdes[2]);
			samp_kowaz_zData[zoneid][samp_kowaz_zPos][1] = floatstr(samp_kowaz_tmpdes[3]);
			samp_kowaz_zData[zoneid][samp_kowaz_zPos][2] = floatstr(samp_kowaz_tmpdes[4]);
			samp_kowaz_zData[zoneid][samp_kowaz_zPos][3] = floatstr(samp_kowaz_tmpdes[5]);
			r = strval(samp_kowaz_tmpdes[9]);
			g = strval(samp_kowaz_tmpdes[10]);
			b = strval(samp_kowaz_tmpdes[11]);
			a = strval(samp_kowaz_tmpdes[12]);
			strmid(samp_kowaz_zData[zoneid][samp_kowaz_zNote1], samp_kowaz_tmpdes[6], 0, strlen(samp_kowaz_tmpdes[6]), 32);
			strmid(samp_kowaz_zData[zoneid][samp_kowaz_zNote2], samp_kowaz_tmpdes[7], 0, strlen(samp_kowaz_tmpdes[7]), 32);
			strmid(samp_kowaz_zData[zoneid][samp_kowaz_zNote3], samp_kowaz_tmpdes[8], 0, strlen(samp_kowaz_tmpdes[8]), 32);
			samp_kowaz_zData[zoneid][samp_kowaz_zColor] = RGBAToHex(r, g, b, a);
			samp_kowaz_zData[zoneid][samp_kowaz_zZone] = GangZoneCreate(floatstr(samp_kowaz_tmpdes[2]), floatstr(samp_kowaz_tmpdes[3]), floatstr(samp_kowaz_tmpdes[4]), floatstr(samp_kowaz_tmpdes[5]));
			for (new playerid; playerid < MAX_PLAYERS; playerid++)
			{
				samp_kowaz_zData[zoneid][samp_kowaz_zVisible][playerid] = 1;
				samp_kowaz_zData[zoneid][samp_kowaz_zFlash][playerid] = 0;
				samp_kowaz_zData[zoneid][samp_kowaz_zLocked][playerid] = 0;
				samp_kowaz_zData[zoneid][samp_kowaz_zGun][playerid] = 1;
			}
			samp_kowaz_zData[zoneid][samp_kowaz_zActive] = 1;
			GangZoneShowForAll(samp_kowaz_zData[zoneid][samp_kowaz_zZone], samp_kowaz_zData[zoneid][samp_kowaz_zColor]);
			index++;
		}
		fclose(zFile);
		return 1;
	}
	return 0;
}

// These two cool functions made by Betamaster
stock HexToRGBA(colour, &r, &g, &b, &a) {
r = (colour >> 24) & 0xFF;
g = (colour >> 16) & 0xFF;
b = (colour >> 8) & 0xFF;
a = colour & 0xFF;
}

stock RGBAToHex(r, g, b, a) {
return (r<<24 | g<<16 | b<<8 | a);
}

// And this function from Denver's PENLS script
stock strsplit(const strsrc[], strdest[][], delimiter)
{
	new i, li;
	new aNum;
	new len;

	while(i <= strlen(strsrc)){
	    if(strsrc[i]==delimiter || i==strlen(strsrc)){
	        len = strmid(strdest[aNum], strsrc, li, i, 128);
	        strdest[aNum][len] = 0;
	        li = i+1;
	        aNum++;
		}
		i++;
	}

	return 1;
}