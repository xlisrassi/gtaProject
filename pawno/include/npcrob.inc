/* RNS V2.0 created by Kreison */

// Defines
#define MAX_ROBNPCS 10 // Max rob NPC number
#define ALARM_STOP 120 // Time to stop alarm (in secs) ~ TO TYPES 2 AND 3
#define ROB_INTERVAL 900 // Time interval to rob again after sucess (in secs)
#define FAILROB_INTERVAL 60 // Time interval to rob again after failure (in secs)
/* ON RNPC NPC NAME DONT WILL APPEAR */
#define NPC_PREFIX "Shopman" // NPC name prefix
#define NPC_SUFFIX "BRL" // NPC name suffix



// Variables
enum e_RobNPCInfo
{
	rnIsCon,
	rnMax,
	rnMin,
	rnInt,
	rnSkin,
	rnWorld,
	rnAlarm,
	Float: rnX,
	Float: rnY,
	Float: rnZ,
	Float: rnAng,
	rnPlayer,
	rnPay,
	rnDelay
}
new RobNPC[MAX_ROBNPCS][e_RobNPCInfo];
new RobNPCS_totaln;
new IsAlarmOn[MAX_PLAYERS];
// -- timers
new timer_aimcheck[MAX_ROBNPCS];
new timer_roubo[MAX_ROBNPCS];
new timer_int[MAX_PLAYERS];



// Stocks
stock IsRobNPC(playerid)
{
	if (IsPlayerNPC(playerid) && RobNPC[playerid][rnIsCon]) { return 1; }
	
	return 0;
}


stock rnRandom(min, max)
{
    //Credits to y_less
    new rand = random(max-min)+min;
    return rand;
}


stock rnIsPlayerInRangeOfPoint(playerid, Float:range, Float:x, Float:y, Float:z, worldid)
{
	if (IsPlayerInRangeOfPoint(playerid, range, x, y, z) && GetPlayerVirtualWorld(playerid) == worldid)
	{
	    return 1;
	}

	return 0;
}


stock AddRobNPC(skin, Float:x, Float:y, Float:z, Float:ang, worldid, interiorid, alarmtype = 0, pay = 1, min = 1000, max = 2000)
{
    if (RobNPCS_totaln >= MAX_ROBNPCS) {
		print("[Robbery NPCs system] ERROR: The actual rob npc numbers exceed the MAX_ROBNPCS number.");
		return 0;
	} else if (min >= max) {
		print("[Robbery NPCs system] ERROR: The minimum amount received by rob is bigger or equal than the maximum!");
	    return 0;
	}

	// Timer to avoid bugs with npc id
	SetTimerEx("AddRobNPC2", 500, false, "iffffiiiiii", skin, Float:x, Float:y, Float:z, Float:ang, worldid, interiorid, alarmtype, pay, min, max);

	return 1;
}


stock PreloadRAnims(playerid)
{
	ApplyAnimation(playerid,"SHOP","null",0.0,0,0,0,0,0);
	ApplyAnimation(playerid,"PED","null",0.0,0,0,0,0,0);
}


stock PlayAlarm(npcid)
{
	new Float: x, Float: y, Float: z;
	GetPlayerPos(npcid, x, y, z);

	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if (rnIsPlayerInRangeOfPoint(i, 50.0, x, y, z, GetPlayerVirtualWorld(npcid)))
		{
		    new ALARM_TYPE = RobNPC[npcid][rnAlarm];
		    if (ALARM_TYPE == 0)
		    {
		        IsAlarmOn[i] = npcid;
				PlayerPlaySound(i, 3401, 0.0, 0.0, 0.0);
		        timer_int[i] = SetTimerEx("IntCheck", 500, true, "ii", i, npcid);
    		} else if (ALARM_TYPE == 1) {
    		    IsAlarmOn[i] = npcid;
				PlayerPlaySound(i, 3401, x, y, z);
		        timer_int[i] = SetTimerEx("IntCheck", 500, true, "ii", i, npcid);
			} else if (ALARM_TYPE == 2) {
			    IsAlarmOn[i] = npcid;
		        PlayerPlaySound(i, 3401, 0.0, 0.0, 0.0);
		        SetTimerEx("StopAlarm", ALARM_STOP*1000, false, "i", i);
			} else if (ALARM_TYPE == 3) {
			    IsAlarmOn[i] = npcid;
		        PlayerPlaySound(i, 3401, x, y, z);
		        SetTimerEx("StopAlarm", ALARM_STOP*1000, false, "i", i);
		    } else if (ALARM_TYPE == 4) {
		        IsAlarmOn[i] = npcid;
		        PlayerPlaySound(i, 3401, 0.0, 0.0, 0.0);
		    } else if (ALARM_TYPE == 5) {
		        IsAlarmOn[i] = npcid;
		        PlayerPlaySound(i, 3401, x, y, z);
		    }
		}
	}
}



// Publics
forward AddRobNPC2(skin, Float:x, Float:y, Float:z, Float:ang, worldid, interiorid, alarmtype, pay, min, max);
public AddRobNPC2(skin, Float:x, Float:y, Float:z, Float:ang, worldid, interiorid, alarmtype, pay, min, max)
{
	RobNPCS_totaln++;

	new npcname[24];
	format(npcname, sizeof(npcname), "%s%i_%s", NPC_PREFIX, RobNPCS_totaln, NPC_SUFFIX);
	new npcid = ConnectRNPC(npcname);

	RobNPC[RobNPCS_totaln][rnIsCon] = npcid;
	RobNPC[RobNPCS_totaln][rnSkin] = skin;
	
	if (pay)
	{
	    RobNPC[RobNPCS_totaln][rnPay] = 1;
		RobNPC[RobNPCS_totaln][rnMax] = max;
		RobNPC[RobNPCS_totaln][rnMin] = min;
	} else {
	    RobNPC[RobNPCS_totaln][rnPay] = 0;
	}
	
	RobNPC[RobNPCS_totaln][rnInt] = interiorid;
	RobNPC[RobNPCS_totaln][rnWorld] = worldid;
 	RobNPC[RobNPCS_totaln][rnX] = x;
 	RobNPC[RobNPCS_totaln][rnY] = y;
 	RobNPC[RobNPCS_totaln][rnZ] = z;
 	RobNPC[RobNPCS_totaln][rnAng] = ang;
 	RobNPC[RobNPCS_totaln][rnAlarm] = alarmtype;
}


forward RobP1(npcid);
public RobP1(npcid)
{
    ClearAnimations(npcid);
    timer_roubo[npcid] = SetTimerEx("RobP2", 100, false, "i", npcid);
}


forward RobP2(npcid);
public RobP2(npcid)
{
    ClearAnimations(npcid);
 	ApplyAnimation(npcid,"SHOP","SHP_Rob_GiveCash",4.1,0,1,1,1,0,1);
    timer_roubo[npcid] = SetTimerEx("RobP3", 3900, false, "i", npcid);
}


forward RobP3(npcid);
public RobP3(npcid)
{
    KillTimer(timer_aimcheck[npcid]);
    
    ClearAnimations(npcid);
    ApplyAnimation(npcid,"PED","DUCK_cower",4.1,1,1,1,1,1,1);
    
	PlayAlarm(npcid);
	
	new moneyganho;
	new playerid = RobNPC[npcid][rnPlayer];
	if (RobNPC[npcid][rnPay])
	{
		moneyganho = rnRandom(RobNPC[npcid][rnMin],RobNPC[npcid][rnMax]);
		new rnMsg[35];
		format(rnMsg, sizeof(rnMsg), "~g~You stole~n~$%i", moneyganho);
		GivePlayerMoney(playerid, moneyganho);
		GameTextForPlayer(playerid, rnMsg, 2000, 0);
	} else {
	    moneyganho = 0;
	}
	
	CallLocalFunction("OnPlayerSucessRob", "iii", playerid, npcid, moneyganho);
	
	SetTimerEx("RobDelay", ROB_INTERVAL*1000, false, "i", npcid);
}


forward AimCheck(playerid, npcid);
public AimCheck(playerid, npcid)
{
	new targetplayer = GetPlayerTargetPlayer(playerid);
	if (targetplayer != npcid)
	{
		ClearAnimations(npcid);
		
		KillTimer(timer_roubo[npcid]);
		KillTimer(timer_aimcheck[npcid]);
		
		PlayAlarm(npcid);

        SetTimerEx("RobDelay", FAILROB_INTERVAL*1000, false, "i", npcid);
		SetTimerEx("AimCheck2", 100, false, "i", npcid);
		
		CallLocalFunction("OnPlayerFailRob", "ii", playerid, npcid);
	}
}


forward AimCheck2(npcid);
public AimCheck2(npcid)
{
    ApplyAnimation(npcid,"PED","DUCK_cower",4.1,1,1,1,1,1,1);
    PlayAlarm(npcid);
}


forward RobDelay(npcid);
public RobDelay(npcid)
{
    RobNPC[npcid][rnDelay] = 0;
    ClearAnimations(npcid);
    
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
	    if(IsAlarmOn[i] == npcid)
	    {
	        StopAlarm(i);
	    }
	}
}


forward IntCheck(playerid, npcid);
public IntCheck(playerid, npcid)
{
	if(GetPlayerInterior(playerid) != RobNPC[npcid][rnInt])
	{
	    StopAlarm(playerid);
	    KillTimer(timer_int[playerid]);
	}
}


forward StopAlarm(playerid);
public StopAlarm(playerid)
{
    IsAlarmOn[playerid] = 0;
    PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
}



// Callbacks SA-MP
public OnPlayerConnect(playerid)
{
    #if defined RN_OnPlayerConnect
        RN_OnPlayerConnect(playerid);
    #endif

	if (!IsPlayerNPC(playerid)) { PreloadRAnims(playerid); }
	return 1;
}


public OnPlayerDisconnect(playerid, reason)
{
    #if defined RN_OnPlayerDisconnect
        RN_OnPlayerDisconnect(playerid, reason);
    #endif
    
    if (!IsPlayerNPC(playerid)) { KillTimer(timer_int[playerid]); }
    return 1;
}


public OnPlayerSpawn(playerid)
{
    #if defined RN_OnPlayerSpawn
        RN_OnPlayerSpawn(playerid);
    #endif
    
    // When NPC spawn, paramaters will be seted
    if(IsRobNPC(playerid))
    {
    	SetPlayerSkin(playerid, RobNPC[playerid][rnSkin]);
    	SetPlayerInterior(playerid, RobNPC[playerid][rnInt]);
		SetPlayerPos(playerid, RobNPC[playerid][rnX], RobNPC[playerid][rnY], RobNPC[playerid][rnZ]);
		SetPlayerFacingAngle(playerid, RobNPC[playerid][rnAng]);
		SetPlayerVirtualWorld(playerid, RobNPC[playerid][rnWorld]);

		PreloadRAnims(playerid);
		
		RNPC_SetShootable(playerid, 0);
    }
    return 1;
}


public OnPlayerUpdate(playerid)
{
    #if defined RN_OnPlayerUpdate
        RN_OnPlayerUpdate(playerid);
    #endif
    
    new targetplayer = GetPlayerTargetPlayer(playerid);
    if(IsRobNPC(targetplayer) && !RobNPC[targetplayer][rnDelay] && GetPlayerWeapon(playerid) >= 22 && GetPlayerWeapon(playerid) <= 38)
    {
		ClearAnimations(targetplayer);
		ApplyAnimation(targetplayer,"SHOP","SHP_Rob_HandsUp",4.1,0,1,1,1,0,1);
		
		RobNPC[targetplayer][rnDelay] = 1;
		RobNPC[targetplayer][rnPlayer] = playerid;
		
		timer_roubo[targetplayer] = SetTimerEx("RobP1", 2000, false, "i", targetplayer);
		timer_aimcheck[targetplayer] = SetTimerEx("AimCheck", 500, true, "ii", playerid, targetplayer);
    }
}


public OnPlayerDeath(playerid, killerid, reason)
{
    #if defined RN_OnPlayerDeath
        RN_OnPlayerDeath(playerid, killerid, reason);
    #endif

    if (IsAlarmOn[playerid]) { StopAlarm(playerid); }

    return 1;
}



// Hooks
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate RN_OnPlayerUpdate
#if defined RN_OnPlayerUpdate
    forward RN_OnPlayerUpdate(playerid);
#endif


#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect RN_OnPlayerConnect
#if defined RN_OnPlayerConnect
    forward RN_OnPlayerConnect(playerid);
#endif


#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect RN_OnPlayerDisconnect
#if defined RN_OnPlayerDisconnect
    forward RN_OnPlayerDisconnect(playerid, reason);
#endif


#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn RN_OnPlayerSpawn
#if defined RN_OnPlayerSpawn
    forward RN_OnPlayerSpawn(playerid);
#endif


#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath RN_OnPlayerDeath
#if defined RN_OnPlayerDeath
    forward RN_OnPlayerDeath(playerid, killerid, reason);
#endif



// Callbacks of include
forward OnPlayerSucessRob(playerid, npcid, value);
forward OnPlayerFailRob(playerid, npcid);
