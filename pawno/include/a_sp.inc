/*
	OnPlayerShootPos by Emmet_
*/
static stock
	g_WeaponData[MAX_PLAYERS][2]
;

static const Float:g_WeaponDist[] =
{
	35.0,
	35.0,
	35.0,
	40.0,
	35.0,
	40.0,
	35.0,
	35.0,
	45.0,
	70.0,
	90.0,
	100.0,
	100.0,
	0.0,
	0.0,
	1.0,
	75.0
};

stock Float:GetPointZPos(const Float:fX, const Float:fY, &Float:fZ = 0.0)
{
	if(!((-3000.0 < fX < 3000.0) && (-3000.0 < fY < 3000.0))) {
		return 0.0;
	}
	static
		File: s_hMap
	;
	if(!s_hMap) {
		s_hMap = fopen("SAfull.hmap", io_read);

		if(!s_hMap) {
			return 0.0;
		}
	}
	new
		afZ[1]
	;
	fseek(s_hMap, ((6000 * (-floatround(fY, floatround_tozero) + 3000) + (floatround(fX, floatround_tozero) + 3000)) << 1));
	fblockread(s_hMap, afZ);

	return (fZ = ((afZ[0] >>> 16) * 0.01));
}

public OnPlayerUpdate(playerid)
{
	new weaponid = GetPlayerWeapon(playerid);

	if (weaponid != g_WeaponData[playerid][0] && weaponid > 1)
	{
		g_WeaponData[playerid][0] = weaponid;
		g_WeaponData[playerid][1] = GetPlayerAmmo(playerid);
	}
	else if (weaponid > 1 && weaponid == g_WeaponData[playerid][0] && GetPlayerAmmo(playerid) < g_WeaponData[playerid][1])
	{
	    if ((weaponid >= 22 && weaponid <= 38) && (weaponid != 35 && weaponid != 36))
	    {
	        new
				Float:x[3],
				Float:y[3],
				Float:z[4],
				Float:range,
				Float:angle,
				Float:dist = (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DUCK) ? (g_WeaponDist[weaponid - 22] + 5.0) : (g_WeaponDist[weaponid - 22])
			;
			GetPlayerFacingAngle(playerid, angle);
	 		GetPlayerCameraPos(playerid, x[0], y[0], z[0]);
	   		GetPlayerCameraFrontVector(playerid, x[1], y[1], z[1]);

			switch (weaponid)
			{
			    case 22..29, 32:
					range = 0.110;

				case 30, 31:
					range = 0.07;
			}
			for (new Float:i = 0.0; i < dist; i += 0.5)
			{
				x[2] = floatadd(x[0], (i * floatsin(-angle, degrees)));
				y[2] = floatadd(y[0], (i * floatcos(-angle, degrees)));
				z[2] = (z[1] + range) * i + z[0];

				GetPointZPos(x[2], y[2], z[3]);

				if (z[3] >= z[2])
				{
				    CallLocalFunction("OnPlayerShootPos", "dffff", playerid, x[2], y[2], z[2], i);
				    break;
				}
			}
		}
        g_WeaponData[playerid][0] = GetPlayerWeapon(playerid);
		g_WeaponData[playerid][1] = GetPlayerAmmo(playerid);
	}
	return CallLocalFunction("SP_OnPlayerUpdate", "d", playerid);
}

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate SP_OnPlayerUpdate

forward SP_OnPlayerUpdate(playerid);
forward OnPlayerShootPos(playerid, Float:x, Float:y, Float:z, Float:distance);
