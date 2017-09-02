/*
	Explosive Include V1.0

	This include provides some explosion releated functions.

	All scripting work was done by Mauzen, msoll(a)web.de, 2011
	Feel free to use this include or to modify it for your own use.
	Do not claim this as your own, or re-release it without my permission.
	Do not remove this comment header.
	Give me some credits if you like (Or not, I cant control it anyways)

	native CreateExplosionEx(playerid, Float:x, Float:y, Float:z, Float:radius, Float:maxdamage, type=EXPLOSION_TYPE_MEDIUM, reason=51, playsound=true);
	native DamagePlayer(playerid, Float:amount, damagerid, reason);
*/


#if !defined CreateObject
	#error "Use this include in gamemodes or filterscripts only."
#endif
#if defined EXPLOSIVE_INCLUDED
	#error "Only include this include once to avoid confilcts."
#endif

#define EXPLOSIVE_INCLUDED                          (true)

/*
	Different defines to support some other includes,
	to provide maximum compatibility
*/

#if defined MapAndreas_Init                         //MapAndreas is not used yet
	#define MAPANDREAS_AVAILABLE 					(true)
#else
	#define MAPANDREAS_AVAILABLE                	(false)
#endif

#if defined foreach
	#define FOREACH_AVAILABLE                   	(true)
#else
	#define FOREACH_AVAILABLE                   	(false)
#endif

#if defined CreateDynamicObject
	#define USE_STREAMER                            (true)
#else
	#define USE_STREAMER                            (false)
#endif



#define EXPLOSION_TYPE_FUEL_CAR                 	(0)
#define EXPLOSION_TYPE_LARGE                    	(1)
#define EXPLOSION_TYPE_MEDIUM                       (2)
#define EXPLOSION_TYPE_MOLOTOV                      (3)
#define EXPLOSION_TYPE_SMALL                        (4)
#define EXPLOSION_TYPE_TINY                         (5)


// -----------------------------------------------------------------------------
// ---------------------------- MAIN STOCKS ------------------------------------

/*
	The main function of this include
	Creates an explosion with a lot of custom parameters

	    playerid:       "Owner" <a href="http://cityadspix.com/tsclick-MIQCWPUV-GECAQBFF?url=http%3A%2F%2Fwww.sotmarket.ru%2Fproduct%2Fpartner-night.html&sa=wp&bt=20&pt=9&lt=2&tl=3&im=ODI1LTAtMTQxMTY0MzUyMy0xMTA5OTM2OQ==&kw=of%20the" target="_blank" alt="Partners of the Night" title="Partners of the Night" style="">of the</a> explosion
	    x/z/z:          Coordinates
	    radius:         Max. damage radius
	    maxdamage:      Max. damage
	    type:           Type <a href="http://cityadspix.com/tsclick-MIQCWPUV-GECAQBFF?url=http%3A%2F%2Fwww.sotmarket.ru%2Fproduct%2Fpartner-night.html&sa=wp&bt=20&pt=9&lt=2&tl=3&sa=wp&im=ODI1LTAtMTQxMTY0MzUyMy0xMTk5Njk4MA==&kw=of%20the" target="_blank" alt="Partners of the Night" title="Partners of the Night" style="">of the</a> explosion (determines the style)
	    reason:         Deathreason for SendDeathMessage if someone is killed
	    playsound:      Play an explosion sound to all players or not.

		returns:        nothing
*/
stock CreateExplosionEx(playerid, Float:x, Float:y, Float:z, Float:radius, Float:maxdamage, type=EXPLOSION_TYPE_MEDIUM, reason=51, playsound=true)
{
	if(type < 0 || type > 5) return;
	new objectid;
	#if (USE_STREAMER == true)
		objectid = CreateDynamicObject(18681 + type, x, y, z - 1.5, 0.0, 0.0, 0.0);
	#else
	    objectid = CreateObject(18681 + type, x, y, z - 1.5, 0.0, 0.0, 0.0);
	#endif
	SetTimerEx("DestroyObjectPub", 5000, 0, "i", objectid);
	if(type == 1 || type == 2 || type == 4) CreateExplosion(x, y, z, 13, 0.0);

	if(playsound)       //Sound
	{
	    #if (FOREACH_AVAILABLE == true)
	        foreach(Player, i)
	        {
	    #else
	        for(new i = 0, j = GetMaxPlayers(); i < j; i ++)
	        {
				if(!IsPlayerConnected(playerid)) continue;
	    #endif

	    	PlayerPlaySound(i, 1159, x, y, z);
        }
	}

	if(maxdamage > 0.0 && radius > 0.0)
	{
	    new Float:radiussquare = radius * radius;
	    new Float:pdist;
	    #if (FOREACH_AVAILABLE == true)
	        foreach(Player, i)
	        {
	    #else
	        for(new i = 0, j = GetMaxPlayers(); i < j; i ++)
	        {
				if(!IsPlayerConnected(playerid)) continue;
	    #endif

			pdist = M_GetPlayerToPointSquare(i, x, y, z);
    		if(pdist > radiussquare) continue;
    		pdist = floatsqroot(pdist);
    		DamagePlayer(i, (1 - (pdist / radius)) * maxdamage, playerid, reason);
        }

	}
}


// -----------------------------------------------------------------------------
// ------------------------- GENERAL FUNCTIONS ---------------------------------

stock DamagePlayer(playerid, Float:amount, damagerid, reason)
{
	new Float:armour, Float:health;

	if(IsPlayerInAnyVehicle(playerid))
    {
		amount *= 2.5;
		GetVehicleHealth(GetPlayerVehicleID(playerid), health);
		SetVehicleHealth(GetPlayerVehicleID(playerid), health - amount);
		if(health - amount <= 250.0)
		{
			// Damage destroyed the vehicle
			SetPVarInt(playerid, "ExpDamager", damagerid + 1);
		    SetPVarInt(playerid, "ExpReason", reason);
		    SetTimerEx("ResetCustomDeath", 12500, 0, "i", playerid);
		}
        return;
    }
	GetPlayerHealth(playerid, health);
	GetPlayerArmour(playerid, armour);

	if(armour > 0.0)
	{
	    if(armour >= amount)
	    {
	        SetPlayerArmour(playerid, armour - amount);
	        amount = 0.0;
	    } else
	    {
	        amount = amount - armour;
			SetPlayerArmour(playerid, 0.0);
		}
	}
	if(amount > 0.0)
	{
		if(amount >= health)
		{
		    SetPVarInt(playerid, "ExpDamager", damagerid + 1);
		    SetPVarInt(playerid, "ExpReason", reason);
		}
	    SetPlayerHealth(playerid, health - amount);
	}
}

forward Float:M_GetPlayerToPointSquare(playerid, Float:x, Float:y, Float:z);
stock Float:M_GetPlayerToPointSquare(playerid, Float:x, Float:y, Float:z)
{
	new Float:px, Float:py, Float:pz;
	if(IsPlayerInAnyVehicle(playerid)) GetVehiclePos(GetPlayerVehicleID(playerid), px, py, pz);
		else GetPlayerPos(playerid, px, py, pz);
	px -= x;
	py -= y;
	pz -= z;

	return px * px + py + py + pz * pz;
}

forward DestroyObjectPub(objectid);
public DestroyObjectPub(objectid)
{
    #if (USE_STREAMER == true)
		DestroyDynamicObject(objectid);
	#else
	    DestroyObject(objectid);
	#endif
}

forward ResetCustomDeath(playerid);
public ResetCustomDeath(playerid)
{
	SetPVarInt(playerid, "ExpDamager", 0);
}

// -----------------------------------------------------------------------------
// --------------------------- CALLBACK HOOKS ----------------------------------

/*
	Put this somewhere in your main() function
*/
stock EXPL_main()
{
	print("Running Mauzen's \"Explosive Include V1.0\"");
	new text[64];
	format(text, sizeof(text), "MapAndreas: %d, foreach: %d, Streamer: %d", MAPANDREAS_AVAILABLE, FOREACH_AVAILABLE, USE_STREAMER);
	print(text);
}

/*
	Put this at the top(!) of your OnPlayerDeath function
*/
stock EXPL_OnPlayerDeath(playerid, &killerid, &reason)
{
	if(killerid == INVALID_PLAYER_ID)
	{
	    if(GetPVarInt(playerid, "ExpDamager") > 0)
	    {
	        killerid = GetPVarInt(playerid, "ExpDamager") - 1;
	        reason = GetPVarInt(playerid, "ExpReason");
            SetPVarInt(playerid, "ExpDamager", 0);
		}
		if(killerid == playerid) killerid = INVALID_PLAYER_ID;
	}
}
