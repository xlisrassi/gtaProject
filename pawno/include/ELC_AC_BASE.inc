/*------------------------------------------------------------------------------
			######## ##        ######             ###     ######
			##       ##       ##    ##           ## ##   ##    ##
			##       ##       ##                ##   ##  ##
			######   ##       ##               ##     ## ##
			##       ##       ##               ######### ##
			##       ##       ##    ##         ##     ## ##    ##
			######## ########  ######  ####### ##     ##  ######


 							########  ##    ##
							##     ##  ##  ##
							##     ##   ####
							########     ##
							##     ##    ##
							##     ##    ##
							########     ##


		######## ##        #######   ######  ######## ########   #######
		##       ##       ##     ## ##    ##    ##    ##     ## ##     ##
		##       ##       ##     ## ##          ##    ##     ## ##     ##
		######   ##       ##     ## ##          ##    ########  ##     ##
		##       ##       ##     ## ##          ##    ##   ##   ##     ##
		##       ##       ##     ## ##    ##    ##    ##    ##  ##     ##
		######## ########  #######   ######     ##    ##     ##  #######

                             Anti-Cheat(ELC_AC)

                                PROTECTIONS :
                              Anti-Weapon HACK
                               Anti-Ammo HACK
                            Anti-Ammo Block HACK
                               Anti-Money HACKF
                               Anti-Speed HACK
				     	Anti-Teleport & Anti-Airbreak
				     		  Anti-Health HACK
				     		  Anti-Armour HACK
				  		  Anti-VEHICLE TELEPORT HACK

                                    BUG :
               					  Nothing

                                  VERSION :
							        V4.1

							     COMPATIBLE :
							     SA-MP 0.3e

                                  CREDITS :
                              ELOCTRO (SCRIPT)
                         SIM (GetTickCount for linux)

				          ELC_AC THE BEST PROTECTION
		                              !

                                    \**/
/*/- Anti-Cheat By Eloctro -----------------------------------------------------

                            ELC PRODUCTION(FRANCE)

                         http://eloctro.blogspot.com/

*/
#include <a_samp>
/*----------------------------------------------------------------------------*/
//Anti-Cheat Statut defaut :
#define ANTI_WEAPON_&_AMMO_HACK     1
#define ANTI_AMMO_BLOCK_HACK        1
#define ANTI_MONEY_HACK             1
#define ANTI_SPEED_HACK             1
#define ANTI_TELEPORT/AIBREAK_HACK  1
#define ANTI_HEALTH_HACK            0//Not fix
#define ANTI_ARMOUR_HACK            0//Not fix
//Configuration :
#define MAX_SPEED_VEHICLE                   380.0 // SPEED VEHICLE MAX(380=default)
#define MAX_VEHICLE_DISTANCE_PER_SECOND     180   // DISTANCE PARCOUR MAX VEHICLE PER SECOND(180=default)
#define MAX_PED_DISTANCE_PER_SECOND         100   // DISTANCE PARCOUR MAX AT PED PER SECOND(100=default)
#define SPRINT_SPEED                        30.0  // (30=DEFAULT/BETA)
#define TIRE_RATE                           2000  // (2000=default)
#define TIMER_CHEAT_RATE                    3500
#define TIME_GRANULITY                      50    // Granulity GetTickCount
#define DELAY_LOAD_AFTER_SPAWN              5000
#define MAX_DELAY_SEND_STATS                2000
#define MAX_DELAY_WAIT_AFTER_SPAWN          3000
#define MAX_HEALTH                          100   // Max HEALTH
#define MAX_ARMOUR                          100   // Max ARMOUR
#define MAX_VEHICLE_DISTANCE_UCM            10.0//MAX_VEHICLE_DISTANCE_UNOCCUPIED_CAN_MOVE
//STATUS MODE :
//#define NO_TICK_COUNT  //uncomment If GetTickCount() bug(linux?)
#define INCLUDE_BASE_MODE //uncomment to pass in mod include
#define MODE_DELAY_SEND_STATS //uncomment if the sending of stats is a bit slow(RECOMMANDED)
//Calcul(DON'T TOUCH)----------------------------------------------------------
#define XMAX_VEHICLE_DISTANCE_PER_SECOND MAX_VEHICLE_DISTANCE_PER_SECOND*(TIMER_CHEAT_RATE/1000)
#define XMAX_PED_DISTANCE_PER_SECOND MAX_PED_DISTANCE_PER_SECOND*(TIMER_CHEAT_RATE/1000)
#define SaveTime(%0,%1) %0=%1
#define IsPassTime(%0,%1) ((MAX_DELAY_SEND_STATS+%0)<%1)//Time Action ,Time Now
#define IsPlayerCheatPos(%0) (!IsPlayerInRangeOfPoint(%0,XMAX_VEHICLE_DISTANCE_PER_SECOND,CheatPlayerInfo[%0][aX],CheatPlayerInfo[%0][aY],CheatPlayerInfo[%0][aZ]))||((GetPlayerSurfingVehicleID(%0)==INVALID_VEHICLE_ID && !IsPlayerInAnyVehicle(%0)) && !IsPlayerInRangeOfPoint(%0,XMAX_PED_DISTANCE_PER_SECOND,CheatPlayerInfo[%0][aX],CheatPlayerInfo[%0][aY],CheatPlayerInfo[%0][aZ]))
#define ELC_AC_IsCreatedVehicle(%0) (GetVehicleModel(%0)!=0)
/*----------------------------------------------------------------------------*/
enum aELCp
{
	aMoney,
	aAmmo[13],
	aWeapon[13],
	aTickFire,
	aAmmoFire,
	aWeaponFire,
	Float:aHealth,
	Float:aGainHealth,
	Float:aArmour,
	aTimer,
	Float:aX,
	Float:aY,
	Float:aZ,
	aAntiTeleportHack,
	aAntiWeaponHack,
	aAntiAmmoBlockHack,
	aAntiMoneyHack,
	aAntiSpeedHack,
	aAntiHealthHack,
	aAntiArmourHack,
#if defined MODE_DELAY_SEND_STATS
	aGiveWeaponTime,
	aGiveMoneyTime,
	aGiveHealthTime,
	aGiveArmourTime,
#endif
	aSetPositionTime,
	aPlayerEnterTime,
	aPossibleVehicleHack
};
new CheatPlayerInfo[MAX_PLAYERS][aELCp];
enum aELCv
{
	Float:vHealth,
	Float:vX,
	Float:vY,
	Float:vZ,
	Float:vAngle,
	vSetPositionTime,
	vPossiblePlayerHack,
	vPossibleHackTime
};
new CheatVehicleInfo[MAX_VEHICLES][aELCv];
#if defined NO_TICK_COUNT
enum aELCs
{
	TickCount,
	TimerTick
};
new CheatServerInfo[aELCs];
#endif
/*stock SENDPOS(playerid)
{
	new Float:Px[3],Ps[128];
	GetPlayerPos(playerid,Px[0],Px[1],Px[2]);
	format(Ps, 128,"ACUTEL : %f,%f,%f | AVANT : %f,%f,%f",Px[0],Px[1],Px[2],CheatPlayerInfo[playerid][aX],CheatPlayerInfo[playerid][aY],CheatPlayerInfo[playerid][aZ]);
	SendClientMessageToAll(0xBD0000FF,Ps);
}*/
/*----------------------------------------------------------------------------*/
stock Float:GetPlayerSpeedXY(playerid)
{
new Float:SpeedX, Float:SpeedY, Float:SpeedZ;
new Float:Speed;
if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid), SpeedX, SpeedY, SpeedZ);
else GetPlayerVelocity(playerid, SpeedX, SpeedY, SpeedZ);
Speed = floatsqroot(floatadd(floatpower(SpeedX, 2.0), floatpower(SpeedY, 2.0)));
return floatmul(Speed, 200.0);
}
#if defined NO_TICK_COUNT
	#define GetTickCount()                          (CheatServerInfo[TickCount])
	forward TimeUpdate();
	public TimeUpdate()
	{
		CheatServerInfo[TickCount] +=TIME_GRANULITY;
		return 1;
	}
#endif
forward OnPlayerCheckCheat(playerid);
forward OnPlayerCheat(playerid, cheatid);
forward ELC_AC_EnablePlayerCheatID(playerid,cheatid,enable);
forward ELC_AC_OnPlayerBuy(playerid,buyprice);
forward OnPlayerCheckFaceWall(playerid,Float:wall_x,Float:wall_y,Float:wall_z,Float:nwall_x,Float:nwall_y,Float:wall_he);
forward RemovePlayerWeapon(playerid, weaponid);
/*----------------------------------------------------------------------------*/
forward ELC_AC_GivePlayerMoney(playerid, money);
public ELC_AC_GivePlayerMoney(playerid, money)
{
    if(CheatPlayerInfo[playerid][aAntiMoneyHack]==1)CheatPlayerInfo[playerid][aAntiMoneyHack]=2;
	CheatPlayerInfo[playerid][aMoney] += money;
	GivePlayerMoney(playerid, money);
	#if defined MODE_DELAY_SEND_STATS
	SaveTime(CheatPlayerInfo[playerid][aGiveMoneyTime],GetTickCount());
	#endif
	if(CheatPlayerInfo[playerid][aAntiMoneyHack]==2)CheatPlayerInfo[playerid][aAntiMoneyHack]=1;
	return 1;
}
stock initial_GivePlayerMoney(playerid, money)
{
	return GivePlayerMoney(playerid, money);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney ELC_AC_GivePlayerMoney
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
forward ELC_AC_ResetPlayerMoney(playerid);
public ELC_AC_ResetPlayerMoney(playerid)
{
	if(CheatPlayerInfo[playerid][aAntiMoneyHack]==1)CheatPlayerInfo[playerid][aAntiMoneyHack]=2;
	ResetPlayerMoney(playerid);
	CheatPlayerInfo[playerid][aMoney] = 0;
	#if defined MODE_DELAY_SEND_STATS
	SaveTime(CheatPlayerInfo[playerid][aGiveMoneyTime],GetTickCount());
	#endif
	if(CheatPlayerInfo[playerid][aAntiMoneyHack]==2)CheatPlayerInfo[playerid][aAntiMoneyHack]=1;
	return 1;
}
stock initial_ResetPlayerMoney(playerid)
{
	return ResetPlayerMoney(playerid);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_ResetPlayerMoney
    #undef ResetPlayerMoney
#else
    #define _ALS_ResetPlayerMoney
#endif
#define ResetPlayerMoney ELC_AC_ResetPlayerMoney
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
forward ELC_AC_GetPlayerMoney(playerid);
public ELC_AC_GetPlayerMoney(playerid)
{
	return CheatPlayerInfo[playerid][aMoney];
}
stock initial_GetPlayerMoney(playerid)
{
	return GetPlayerMoney(playerid);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_GetPlayerMoney
    #undef GetPlayerMoney
#else
    #define _ALS_GetPlayerMoney
#endif
#define GetPlayerMoney ELC_AC_GetPlayerMoney
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
forward ELC_AC_SetPlayerPos(playerid, Float:ix, Float:iy, Float:iz);
public ELC_AC_SetPlayerPos(playerid, Float:ix, Float:iy, Float:iz)
{
    if(CheatPlayerInfo[playerid][aAntiTeleportHack]==1)CheatPlayerInfo[playerid][aAntiTeleportHack]=2;
    CheatPlayerInfo[playerid][aPlayerEnterTime]+=221;
	SaveTime(CheatPlayerInfo[playerid][aSetPositionTime],GetTickCount());
	CheatPlayerInfo[playerid][aX]=ix; CheatPlayerInfo[playerid][aY]=iy; CheatPlayerInfo[playerid][aZ]=iz;
	SetPlayerPos(playerid, ix, iy, iz);
    if(CheatPlayerInfo[playerid][aAntiTeleportHack]==2)CheatPlayerInfo[playerid][aAntiTeleportHack]=1;
	return 1;
}
stock initial_SetPlayerPos(playerid, Float:ix, Float:iy, Float:iz)
{
	return SetPlayerPos(playerid, ix, iy, iz);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_SetPlayerPos
    #undef SetPlayerPos
#else
    #define _ALS_SetPlayerPos
#endif
#define SetPlayerPos ELC_AC_SetPlayerPos
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_PutPlayerInVehicle(playerid,vehicleid, seatid);
public ELC_AC_PutPlayerInVehicle(playerid,vehicleid, seatid)
{
    if(CheatPlayerInfo[playerid][aAntiTeleportHack]==1)CheatPlayerInfo[playerid][aAntiTeleportHack]=2;
    CheatPlayerInfo[playerid][aPlayerEnterTime]+=221;
	SaveTime(CheatPlayerInfo[playerid][aSetPositionTime],GetTickCount());
	PutPlayerInVehicle(playerid,vehicleid, seatid);
	if(CheatPlayerInfo[playerid][aAntiTeleportHack]==2)CheatPlayerInfo[playerid][aAntiTeleportHack]=1;
	return 1;
}
stock initial_PutPlayerInVehicle(playerid,vehicleid, seatid)
{
	return PutPlayerInVehicle(playerid,vehicleid, seatid);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_PutPlayerInVehicle
    #undef PutPlayerInVehicle
#else
    #define _ALS_PutPlayerInVehicle
#endif
#define PutPlayerInVehicle ELC_AC_PutPlayerInVehicle
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_RemovePlayerFromVehicle(playerid);
public ELC_AC_RemovePlayerFromVehicle(playerid)
{
    CheatPlayerInfo[playerid][aPlayerEnterTime]+=221;
	RemovePlayerFromVehicle(playerid);
	return 1;
}
stock initial_RemovePlayerFromVehicle(playerid)
{
	return RemovePlayerFromVehicle(playerid);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_RemovePlayerFromVehicle
    #undef RemovePlayerFromVehicle
#else
    #define _ALS_RemovePlayerFromVehicle
#endif
#define RemovePlayerFromVehicle ELC_AC_RemovePlayerFromVehicle
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_TogglePlayerSpectating(playerid, toggle);
public ELC_AC_TogglePlayerSpectating(playerid, toggle)
{
	if(toggle==1)
	{
		if(CheatPlayerInfo[playerid][aAntiSpeedHack]==0) CheatPlayerInfo[playerid][aAntiSpeedHack]=3;
		if(CheatPlayerInfo[playerid][aAntiTeleportHack]==0) CheatPlayerInfo[playerid][aAntiTeleportHack]=3;
	}
	else
	{
		if(CheatPlayerInfo[playerid][aAntiSpeedHack]==3) CheatPlayerInfo[playerid][aAntiSpeedHack]=1;
		if(CheatPlayerInfo[playerid][aAntiTeleportHack]==3)CheatPlayerInfo[playerid][aAntiTeleportHack]=1;
		SaveTime(CheatPlayerInfo[playerid][aSetPositionTime],GetTickCount());
	}
	return TogglePlayerSpectating(playerid, toggle);
}
stock initial_TogglePlayerSpectating(playerid, toggle)
{
	return TogglePlayerSpectating(playerid, toggle);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_TogglePlayerSpectating
    #undef TogglePlayerSpectating
#else
    #define _ALS_TogglePlayerSpectating
#endif
#define TogglePlayerSpectating ELC_AC_TogglePlayerSpectating
//ALS_OFF_SYSTEME ----------------
#endif
//Weapon------------------------------------------------------------------------
/*#define IsNotWeaponNoAmmo(%0) (%0!=0 && %0!=1 && %0!=10)
#define IsNotWeaponVise(%0,%1) (%0>1 && %0!=10 && %0!=6 && %0!=9 && %0!=11 && %0!=12 && %1!=35 && %1!=36 %1!=43)*/
stock IsNotWeaponNoAmmo(WeaponID)
{
	new slot=GetWeaponSlot(WeaponID);
	if(slot!=0 && slot!=1 && slot!=10)return 1;
	return 0;
}
stock IsNotWeaponVise(WeaponID)
{
	new slot=GetWeaponSlot(WeaponID);
	if(slot>1 && slot!=10 && slot!=6 && slot!=9 &&slot!=11 && slot!=12 && slot!=35 && slot!=36 && slot!=43) return 1;
	return 0;
}
stock IsGiveVehicleWeapon(WeaponID)
{
	if(WeaponID==25 || WeaponID==46 || WeaponID==2) return 1;
	return 0;
}
#define IsValidWeapon(%0) (%0>=1 && %0<=18 || %0>=21 && %0<=46)
stock GetWeaponSlot(weaponid)
{
	new slot=-1;
	switch(weaponid)
	{
		case 0,1: slot = 0;
		case 2 .. 9: slot = 1;
		case 10 .. 15: slot = 10;
		case 16 .. 18, 39: slot = 8;
		case 22 .. 24: slot =2;
		case 25 .. 27: slot = 3;
		case 28, 29, 32: slot = 4;
		case 30, 31: slot = 5;
		case 33, 34: slot = 6;
		case 35 .. 38: slot = 7;
		case 40: slot = 12;
		case 41 .. 43: slot = 9;
		case 44 .. 46: slot = 11;
	}
	return slot;
}
/*----------------------------------------------------------------------------*/
forward ELC_AC_GivePlayerWeapon(playerid,Weapon,ammo);
public ELC_AC_GivePlayerWeapon(playerid,Weapon,ammo)
{
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==1)CheatPlayerInfo[playerid][aAntiWeaponHack]=2;
	new slot=GetWeaponSlot(Weapon);
	if(IsValidWeapon(Weapon) && slot!=-1)
	{
		CheatPlayerInfo[playerid][aWeapon][slot] = Weapon;
		CheatPlayerInfo[playerid][aAmmo][slot] += ammo;
		GivePlayerWeapon(playerid,Weapon,ammo);
		#if defined MODE_DELAY_SEND_STATS
		SaveTime(CheatPlayerInfo[playerid][aGiveWeaponTime],GetTickCount());
		#endif
	}
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==2)CheatPlayerInfo[playerid][aAntiWeaponHack]=1;
    return 1;
}
stock initial_GivePlayerWeapon(playerid,Weapon,ammo)
{
	return GivePlayerWeapon(playerid,Weapon,ammo);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon ELC_AC_GivePlayerWeapon
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_SetPlayerAmmo(playerid,weapon,ammo);
public ELC_AC_SetPlayerAmmo(playerid,weapon,ammo)
{
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==1)CheatPlayerInfo[playerid][aAntiWeaponHack]=2;
	new slot=GetWeaponSlot(weapon);
 	if(weapon>0 && 47>weapon && slot!=-1)
 	{
		CheatPlayerInfo[playerid][aAmmo][slot]=ammo;
		SetPlayerAmmo(playerid,weapon,ammo);
		#if defined MODE_DELAY_SEND_STATS
		SaveTime(CheatPlayerInfo[playerid][aGiveWeaponTime],GetTickCount());
		#endif
	}
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==2)CheatPlayerInfo[playerid][aAntiWeaponHack]=1;
    return 1;
}
stock initial_SetPlayerAmmo(playerid,weapon,ammo)
{
	return SetPlayerAmmo(playerid,weapon,ammo);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_SetPlayerAmmo
    #undef SetPlayerAmmo
#else
    #define _ALS_SetPlayerAmmo
#endif
#define SetPlayerAmmo ELC_AC_SetPlayerAmmo
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_ResetPlayerWeapons(playerid);
public ELC_AC_ResetPlayerWeapons(playerid)
{
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==1)CheatPlayerInfo[playerid][aAntiWeaponHack]=2;
	for(new i = 0; i <13; i++)
	{
        CheatPlayerInfo[playerid][aWeapon][i]=0;
        CheatPlayerInfo[playerid][aAmmo][i]=0;
	}
	ResetPlayerWeapons(playerid);
	#if defined MODE_DELAY_SEND_STATS
	SaveTime(CheatPlayerInfo[playerid][aGiveWeaponTime],GetTickCount());
	#endif
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==2)CheatPlayerInfo[playerid][aAntiWeaponHack]=1;
    return 1;
}
stock initial_ResetPlayerWeapons(playerid)
{
	return ResetPlayerWeapons(playerid);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_ResetPlayerWeapons
    #undef ResetPlayerWeapons
#else
    #define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons ELC_AC_ResetPlayerWeapons
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_SetPlayerHealth(playerid,Float:health);
public ELC_AC_SetPlayerHealth(playerid,Float:health)
{
	if(CheatPlayerInfo[playerid][aAntiHealthHack]==1)CheatPlayerInfo[playerid][aAntiHealthHack]=2;
	if(health>MAX_HEALTH)
	{
    	CheatPlayerInfo[playerid][aHealth]=MAX_HEALTH;
		SetPlayerHealth(playerid,MAX_HEALTH);
	}
	else
	{
	    CheatPlayerInfo[playerid][aHealth]=health;
		SetPlayerHealth(playerid,health);
	}
	#if defined MODE_DELAY_SEND_STATS
	SaveTime(CheatPlayerInfo[playerid][aGiveHealthTime],GetTickCount());
	#endif
	if(CheatPlayerInfo[playerid][aAntiHealthHack]==2)CheatPlayerInfo[playerid][aAntiHealthHack]=1;
    return 1;
}
stock initial_SetPlayerHealth(playerid,Float:health)
{
	return SetPlayerHealth(playerid,health);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth ELC_AC_SetPlayerHealth
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_SetPlayerArmour(playerid,Float:armour);
public ELC_AC_SetPlayerArmour(playerid,Float:armour)
{
	if(CheatPlayerInfo[playerid][aAntiArmourHack]==1)CheatPlayerInfo[playerid][aAntiArmourHack]=2;
	if(armour>MAX_ARMOUR)
	{
    	CheatPlayerInfo[playerid][aArmour]=MAX_ARMOUR;
		SetPlayerArmour(playerid,MAX_ARMOUR);
	}
	else
	{
	    CheatPlayerInfo[playerid][aArmour]=armour;
		SetPlayerArmour(playerid,armour);
	}
	#if defined MODE_DELAY_SEND_STATS
	SaveTime(CheatPlayerInfo[playerid][aGiveArmourTime],GetTickCount());
	#endif
	if(CheatPlayerInfo[playerid][aAntiArmourHack]==2)CheatPlayerInfo[playerid][aAntiArmourHack]=1;
    return 1;
}
stock initial_SetPlayerArmour(playerid,Float:armour)
{
	return SetPlayerArmour(playerid,armour);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_SetPlayerArmour
    #undef SetPlayerArmour
#else
    #define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour ELC_AC_SetPlayerArmour
//ALS_OFF_SYSTEME ----------------
#endif
/*----------------------------------------------------------------------------*/
forward ELC_AC_SetVehiclePos(vehicleid,Float:x,Float:y,Float:z);
public ELC_AC_SetVehiclePos(vehicleid,Float:x,Float:y,Float:z)
{
	CheatVehicleInfo[vehicleid][vX]=x;
	CheatVehicleInfo[vehicleid][vY]=y;
	CheatVehicleInfo[vehicleid][vZ]=z;
    SetVehiclePos(vehicleid,x,y,z);
    return 1;
}
stock initial_SetVehiclePos(vehicleid,Float:x,Float:y,Float:z)
{
	return SetVehiclePos(vehicleid,x,y,z);
}
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_SetVehiclePos
    #undef SetVehiclePos
#else
    #define _ALS_SetVehiclePos
#endif
#define SetVehiclePos ELC_AC_SetVehiclePos
//ALS_OFF_SYSTEME ----------------
#endif
//Core--------------------------------------------------------------------------
public OnPlayerCheckCheat(playerid)
{
	new IsTime=GetTickCount();
	#if defined MODE_DELAY_SEND_STATS
	if(CheatPlayerInfo[playerid][aAntiMoneyHack]==1 && IsPassTime(CheatPlayerInfo[playerid][aGiveMoneyTime],IsTime))
	#else
	if(CheatPlayerInfo[playerid][aAntiMoneyHack]==1)
	#endif
	{
		new elc_getmoney=initial_GetPlayerMoney(playerid);
		if(elc_getmoney!=CheatPlayerInfo[playerid][aMoney])
		{
			if (CheatPlayerInfo[playerid][aMoney] > elc_getmoney)
			{
                ELC_AC_OnPlayerBuy(playerid,CheatPlayerInfo[playerid][aMoney]-elc_getmoney);
			    CheatPlayerInfo[playerid][aMoney]=elc_getmoney;
			}
			else
			{
				initial_ResetPlayerMoney(playerid);
				initial_GivePlayerMoney(playerid, CheatPlayerInfo[playerid][aMoney]);
				CallRemoteFunction("OnPlayerCheat", "dd", playerid,1);
			}
		}
	}
	if(CheatPlayerInfo[playerid][aAntiSpeedHack]==1)
	{
		if((GetPlayerSurfingVehicleID(playerid)!=INVALID_VEHICLE_ID || GetPlayerState(playerid) == PLAYER_STATE_DRIVER) && (GetPlayerSpeedXY(playerid) > MAX_SPEED_VEHICLE)) CallRemoteFunction("OnPlayerCheat", "dd", playerid,5);
	}
	if(CheatPlayerInfo[playerid][aAntiTeleportHack]==1)
	{
	    #if defined MODE_DELAY_SEND_STATS
		if(CheatPlayerInfo[playerid][aSetPositionTime]!=0 && IsPassTime(CheatPlayerInfo[playerid][aSetPositionTime],IsTime))
		{
			CheatPlayerInfo[playerid][aSetPositionTime]=0;
			GetPlayerPos(playerid,CheatPlayerInfo[playerid][aX],CheatPlayerInfo[playerid][aY],CheatPlayerInfo[playerid][aZ]);
		}
		else if(IsPlayerCheatPos(playerid))
		#else
		if(IsPlayerCheatPos(playerid))
		#endif
		{
		    CallRemoteFunction("OnPlayerCheat", "dd", playerid,6);
			GetPlayerPos(playerid,CheatPlayerInfo[playerid][aX],CheatPlayerInfo[playerid][aY],CheatPlayerInfo[playerid][aZ]);
		}
		else
		{
		    GetPlayerPos(playerid,CheatPlayerInfo[playerid][aX],CheatPlayerInfo[playerid][aY],CheatPlayerInfo[playerid][aZ]);
		}
	}
	#if defined MODE_DELAY_SEND_STATS
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==1 && IsPassTime(CheatPlayerInfo[playerid][aGiveWeaponTime],IsTime))
	#else
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==1)
	#endif
	{
		//ANTI BLOCK MUNITIONS
		if(CheatPlayerInfo[playerid][aTickFire]!=0 && CheatPlayerInfo[playerid][aAntiAmmoBlockHack]==1)
		{
			if((IsTime-CheatPlayerInfo[playerid][aTickFire]) >= TIRE_RATE)
			{
				if(CheatPlayerInfo[playerid][aWeaponFire]==GetPlayerWeapon(playerid))
				{
					if(CheatPlayerInfo[playerid][aAmmoFire]==GetPlayerAmmo(playerid)) ELC_AC_StartCheckPlayerFaceWall(playerid);
					CheatPlayerInfo[playerid][aTickFire]=0;
					CheatPlayerInfo[playerid][aAmmoFire]=0;
					CheatPlayerInfo[playerid][aWeaponFire]=0;

				}
				else
				{

				    new keys,ud,lr;
				    GetPlayerKeys(playerid,keys,ud,lr);
					if(keys==4 && IsNotWeaponVise(GetPlayerWeapon(playerid)))
					{
						CheatPlayerInfo[playerid][aTickFire]=IsTime;
						CheatPlayerInfo[playerid][aAmmoFire]=GetPlayerAmmo(playerid);
						CheatPlayerInfo[playerid][aWeaponFire]=GetPlayerWeapon(playerid);
					}
					else if(keys==132 && IsNotWeaponNoAmmo(GetPlayerWeapon(playerid)) && (GetPlayerSpeedXY(playerid) < 1))
					{
						CheatPlayerInfo[playerid][aTickFire]=IsTime;
						CheatPlayerInfo[playerid][aAmmoFire]=GetPlayerAmmo(playerid);
						CheatPlayerInfo[playerid][aWeaponFire]=GetPlayerWeapon(playerid);
					}
					else
					{
						CheatPlayerInfo[playerid][aTickFire]=0;
						CheatPlayerInfo[playerid][aAmmoFire]=0;
						CheatPlayerInfo[playerid][aWeaponFire]=0;
					}
				}
			}
		}
		new WA_VAR[2][13];
		new NeedRemove;
		for(new i = 0; i <13; i++)
		{
			GetPlayerWeaponData(playerid,i,WA_VAR[0][i],WA_VAR[1][i]);
			if((WA_VAR[0][i]!=CheatPlayerInfo[playerid][aWeapon][i] && WA_VAR[0][i]!=0 && WA_VAR[1][i]!=0))
			{
				//CHEAT WEAPON
				if(!IsGiveVehicleWeapon(WA_VAR[0][i])) CallRemoteFunction("OnPlayerCheat", "dd", playerid,2);
				NeedRemove=1;
				//SetPlayerAmmo(playerid, WA_VAR[0], 0);
			}
			else if(((CheatPlayerInfo[playerid][aAmmo][i]-WA_VAR[1][i]) < 0) && IsNotWeaponNoAmmo(WA_VAR[0][i]) && WA_VAR[0][i]!=0)
			{
				//CHEAT AMMO
				CallRemoteFunction("OnPlayerCheat", "dd", playerid,3);
				NeedRemove=1;
				//SetPlayerAmmo(playerid, WA_VAR[0], 0);
			}
			else if(CheatPlayerInfo[playerid][aWeapon][i]!=0 && WA_VAR[0][i]==0 && WA_VAR[1][i]==0)
			{
			    CheatPlayerInfo[playerid][aWeapon][i]=0;
			    CheatPlayerInfo[playerid][aAmmo][i]=0;
			}
			else CheatPlayerInfo[playerid][aAmmo][i]=WA_VAR[1][i];
		}
		if(NeedRemove==1)
		{
			new WepUse=GetPlayerWeapon(playerid);
			initial_ResetPlayerWeapons(playerid);
		    for(new i = 0; i <13; i++)if(CheatPlayerInfo[playerid][aWeapon][i] != 0)initial_GivePlayerWeapon(playerid, CheatPlayerInfo[playerid][aWeapon][i], CheatPlayerInfo[playerid][aAmmo][i]);
		    SetPlayerArmedWeapon(playerid,WepUse);
		}
	}
	#if defined MODE_DELAY_SEND_STATS
	if(CheatPlayerInfo[playerid][aAntiHealthHack]==1 && IsPassTime(CheatPlayerInfo[playerid][aGiveHealthTime],IsTime) && IsPassTime(CheatPlayerInfo[playerid][aGiveMoneyTime],IsTime))
	#else
	if(CheatPlayerInfo[playerid][aAntiHealthHack]==1)
	#endif
	{
		new Float:elc_health;
		GetPlayerHealth(playerid,elc_health);
		if(elc_health!=CheatPlayerInfo[playerid][aHealth])
		{
			if(elc_health<=100 && CheatPlayerInfo[playerid][aHealth]==0) CheatPlayerInfo[playerid][aHealth]=elc_health;
			else if(elc_health>CheatPlayerInfo[playerid][aHealth])
			{
			    //Cheat Health
			    initial_SetPlayerHealth(playerid,CheatPlayerInfo[playerid][aHealth]);
			    CallRemoteFunction("OnPlayerCheat", "dd", playerid,7);
			}
			else CheatPlayerInfo[playerid][aHealth]=elc_health;
		}
	}
	#if defined MODE_DELAY_SEND_STATS
	if(CheatPlayerInfo[playerid][aAntiArmourHack]==1 && IsPassTime(CheatPlayerInfo[playerid][aGiveArmourTime],IsTime) && IsPassTime(CheatPlayerInfo[playerid][aGiveMoneyTime],IsTime))
	#else
	if(CheatPlayerInfo[playerid][aAntiArmourHack]==1)
	#endif
	{
		new Float:elc_armour;
		GetPlayerArmour(playerid,elc_armour);
		if(elc_armour!=CheatPlayerInfo[playerid][aArmour])
		{
			if(elc_armour>CheatPlayerInfo[playerid][aArmour])
			{
			    //Cheat Armour
			    initial_SetPlayerArmour(playerid,CheatPlayerInfo[playerid][aArmour]);
			    CallRemoteFunction("OnPlayerCheat", "dd", playerid,8);
			}
			else CheatPlayerInfo[playerid][aArmour]=elc_armour;
		}
	}
	if(GetPlayerVehicleSeat(playerid)==0)
	{
		new vehicleid=GetPlayerVehicleID(playerid);
		if(vehicleid!=0)
		{
			GetVehiclePos(vehicleid, CheatVehicleInfo[vehicleid][vX], CheatVehicleInfo[vehicleid][vY], CheatVehicleInfo[vehicleid][vZ]);
            GetVehicleZAngle(vehicleid,CheatVehicleInfo[vehicleid][vAngle]);
		}
	}
	if(CheatPlayerInfo[playerid][aPossibleVehicleHack]!=0 && IsPassTime(CheatVehicleInfo[CheatPlayerInfo[playerid][aPossibleVehicleHack]][vPossibleHackTime],IsTime))
	{
	    if(ELC_AC_IsCreatedVehicle(CheatPlayerInfo[playerid][aPossibleVehicleHack]))
	    {
	        CallRemoteFunction("OnPlayerCheat", "dd", playerid,9);
		    SetVehiclePos(CheatPlayerInfo[playerid][aPossibleVehicleHack], CheatVehicleInfo[CheatPlayerInfo[playerid][aPossibleVehicleHack]][vX], CheatVehicleInfo[CheatPlayerInfo[playerid][aPossibleVehicleHack]][vY], CheatVehicleInfo[CheatPlayerInfo[playerid][aPossibleVehicleHack]][vZ]);
		    SetVehicleZAngle(CheatPlayerInfo[playerid][aPossibleVehicleHack],CheatVehicleInfo[CheatPlayerInfo[playerid][aPossibleVehicleHack]][vAngle]);
			CheatVehicleInfo[CheatPlayerInfo[playerid][aPossibleVehicleHack]][vPossiblePlayerHack]=0;
			CheatVehicleInfo[CheatPlayerInfo[playerid][aPossibleVehicleHack]][vPossibleHackTime]=0;
		}
        CheatPlayerInfo[playerid][aPossibleVehicleHack]=0;
    }
	return 1;
}
/*----------------------------------------------------------------------------*/
stock ELC_AC_StartCheckPlayerFaceWall(playerid)
{
	if(IsPlayerConnected(playerid))
	{
		if(CheatPlayerInfo[playerid][aAntiAmmoBlockHack]==1)CheatPlayerInfo[playerid][aAntiAmmoBlockHack]=2;
		new Float:x,Float:y,Float:z,Float:a,Float:lx,Float:ly,Float:lhealth;
		GetPlayerPos(playerid, x, y ,z);
		GetPlayerFacingAngle(playerid, a);
		GetPlayerHealth(playerid,lhealth);
		lx = (0.5 * floatsin(-a, degrees));
		ly = (0.5 * floatcos(-a, degrees));
		SetPlayerVelocity(playerid,lx,ly,0.0);
		SetTimerEx("OnPlayerCheckFaceWall",500,0,"iffffff",playerid,x+lx,y+ly,z,x,y,lhealth);
	}
	return 1;
}
/*----------------------------------------------------------------------------*/
public OnPlayerCheckFaceWall(playerid,Float:wall_x,Float:wall_y,Float:wall_z,Float:nwall_x,Float:nwall_y,Float:wall_he)
{
	if(IsPlayerConnected(playerid))
	{
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid, x, y ,z);
		if(IsPlayerInRangeOfPoint(playerid,0.25,wall_x,wall_y,z))
		{
		    //Cheat Block Ammo
			CallRemoteFunction("OnPlayerCheat", "dd", playerid,4);
			initial_ResetPlayerWeapons(playerid);
		}
		SetPlayerHealth(playerid,wall_he);
		initial_SetPlayerPos(playerid, nwall_x,nwall_y,wall_z);
		if(CheatPlayerInfo[playerid][aAntiAmmoBlockHack]==2)CheatPlayerInfo[playerid][aAntiAmmoBlockHack]=1;
	}
	return 1;
}
/*----------------------------------------------------------------------------*/
public ELC_AC_OnPlayerBuy(playerid,buyprice)
{
	if(buyprice==1)
	{
		new Float:elc_health;
		GetPlayerHealth(playerid,elc_health);
		if(elc_health<=CheatPlayerInfo[playerid][aHealth]+35) CheatPlayerInfo[playerid][aGainHealth]+=35;//Buy Soda
	}
	return 1;
}
/*----------------------------------------------------------------------------*/
public OnPlayerUpdate(playerid)
{
	new elc_getmoney=initial_GetPlayerMoney(playerid);
	new Float:elc_health; GetPlayerHealth(playerid,elc_health);
	#if defined MODE_DELAY_SEND_STATS
	if(CheatPlayerInfo[playerid][aMoney] > elc_getmoney && CheatPlayerInfo[playerid][aAntiMoneyHack]!=2 && IsPassTime(CheatPlayerInfo[playerid][aGiveMoneyTime],GetTickCount()))
	#else
	if(CheatPlayerInfo[playerid][aMoney] > elc_getmoney && CheatPlayerInfo[playerid][aAntiMoneyHack]!=2)
	#endif
	{
		//printf("ARGENTV : %d,ARGENT : %d,TIME : %d",CheatPlayerInfo[playerid][aMoney],elc_getmoney,GetTickCount());
		ELC_AC_OnPlayerBuy(playerid,CheatPlayerInfo[playerid][aMoney]-elc_getmoney);
		if(elc_health!=CheatPlayerInfo[playerid][aHealth])
		{
		    if(CheatPlayerInfo[playerid][aGainHealth]+CheatPlayerInfo[playerid][aHealth]>=elc_health && elc_health<=MAX_HEALTH)
		    {
		        CheatPlayerInfo[playerid][aGainHealth]=0;
		        CheatPlayerInfo[playerid][aHealth]=elc_health;
		    }
		    else initial_SetPlayerHealth(playerid,CheatPlayerInfo[playerid][aHealth]);
		}
	    CheatPlayerInfo[playerid][aMoney]=elc_getmoney;

	}
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnPlayerUpdate", "i",playerid);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate ELC_AC_OnPlayerUpdate
forward ELC_AC_OnPlayerUpdate(playerid);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
public OnGameModeInit()
#else
public OnFilterScriptInit()
#endif
{
    #if defined NO_TICK_COUNT
	CheatServerInfo[TimerTick]=SetTimer("TimeUpdate", TIME_GRANULITY, true);
	#endif
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnGameModeInit", "");
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit ELC_AC_OnGameModeInit
forward ELC_AC_OnGameModeInit();
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
public OnGameModeExit()
#else
public OnFilterScriptExit()
#endif
{
	#if defined NO_TICK_COUNT
	KillTimer(CheatServerInfo[TimerTick]);
	#endif
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnGameModeExit", "");
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit ELC_AC_OnGameModeExit
forward ELC_AC_OnGameModeExit();
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnPlayerConnect(playerid)
{
	if (!IsPlayerNPC(playerid))
	{
		CheatPlayerInfo[playerid][aTickFire]=0;
		CheatPlayerInfo[playerid][aAmmoFire]=0;
		CheatPlayerInfo[playerid][aWeaponFire]=0;
		for(new i = 0; i <13; i++)
		{
			CheatPlayerInfo[playerid][aWeapon][i]=0;
			CheatPlayerInfo[playerid][aAmmo][i]=0;
		}
		CheatPlayerInfo[playerid][aMoney] = 0;
		CheatPlayerInfo[playerid][aArmour]=100;
		CheatPlayerInfo[playerid][aHealth]=100;
		CheatPlayerInfo[playerid][aGainHealth]=0;
		CheatPlayerInfo[playerid][aTimer]=SetTimerEx("OnPlayerCheckCheat", TIMER_CHEAT_RATE, 1, "i", playerid);
		CheatPlayerInfo[playerid][aX] = 0; CheatPlayerInfo[playerid][aY] = 0; CheatPlayerInfo[playerid][aZ] = 0;
		CheatPlayerInfo[playerid][aAntiTeleportHack]=0;
		CheatPlayerInfo[playerid][aAntiWeaponHack]=ANTI_WEAPON_&_AMMO_HACK;
		CheatPlayerInfo[playerid][aAntiAmmoBlockHack]=ANTI_AMMO_BLOCK_HACK;
		CheatPlayerInfo[playerid][aAntiMoneyHack]=ANTI_MONEY_HACK;
		CheatPlayerInfo[playerid][aAntiSpeedHack]=0;
		CheatPlayerInfo[playerid][aAntiArmourHack]=ANTI_ARMOUR_HACK;
		CheatPlayerInfo[playerid][aAntiHealthHack]=ANTI_HEALTH_HACK;
		#if defined MODE_DELAY_SEND_STATS
		CheatPlayerInfo[playerid][aGiveWeaponTime]=1;
		CheatPlayerInfo[playerid][aGiveMoneyTime]=1;
		CheatPlayerInfo[playerid][aGiveHealthTime]=1;
		CheatPlayerInfo[playerid][aGiveArmourTime]=1;
		#endif
		CheatPlayerInfo[playerid][aPlayerEnterTime]=0;
		SendClientMessage(playerid,0xBD0000FF,"[ANTI-CHEAT]Ce serveur est protégé par ELC_AC(By Eloctro)");
	}
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnPlayerConnect", "d", playerid);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect ELC_AC_OnPlayerConnect
forward ELC_AC_OnPlayerConnect(playerid);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnPlayerSpawn(playerid)
{
	if (!IsPlayerNPC(playerid))
	{
		SaveTime(CheatPlayerInfo[playerid][aSetPositionTime],GetTickCount()+MAX_DELAY_WAIT_AFTER_SPAWN);
		if(CheatPlayerInfo[playerid][aAntiSpeedHack]!=2)
		{
			CheatPlayerInfo[playerid][aAntiTeleportHack]=ANTI_TELEPORT/AIBREAK_HACK;
			CheatPlayerInfo[playerid][aAntiSpeedHack]=ANTI_SPEED_HACK;
		}
		CheatPlayerInfo[playerid][aHealth]=100;
		CheatPlayerInfo[playerid][aArmour]=100;
	}
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnPlayerSpawn", "d", playerid);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn ELC_AC_OnPlayerSpawn
forward ELC_AC_OnPlayerSpawn(playerid);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnPlayerDeath(playerid, killerid, reason)
{
	if (!IsPlayerNPC(playerid))
	{
	    CheatPlayerInfo[playerid][aPlayerEnterTime]+=221;
		CheatPlayerInfo[playerid][aAntiTeleportHack]=0;
		CheatPlayerInfo[playerid][aAntiSpeedHack]=0;
	}
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnPlayerDeath", "ddd", playerid, killerid, reason);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath ELC_AC_OnPlayerDeath
forward ELC_AC_OnPlayerDeath(playerid, killerid, reason);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnPlayerDisconnect(playerid, reason)
{
	CheatPlayerInfo[playerid][aAntiTeleportHack]=0;
	CheatPlayerInfo[playerid][aAntiWeaponHack]=0;
	CheatPlayerInfo[playerid][aAntiAmmoBlockHack]=0;
	CheatPlayerInfo[playerid][aAntiMoneyHack]=0;
	CheatPlayerInfo[playerid][aAntiSpeedHack]=0;
	CheatPlayerInfo[playerid][aAntiArmourHack]=0;
	CheatPlayerInfo[playerid][aAntiHealthHack]=0;
	if (!IsPlayerNPC(playerid))
	{
		KillTimer(CheatPlayerInfo[playerid][aTimer]);
		CheatPlayerInfo[playerid][aTickFire]=0;
		CheatPlayerInfo[playerid][aAmmoFire]=0;
		CheatPlayerInfo[playerid][aWeaponFire]=0;
	}
	CheatPlayerInfo[playerid][aX] = 0; CheatPlayerInfo[playerid][aY] = 0; CheatPlayerInfo[playerid][aZ] = 0;
	CheatPlayerInfo[playerid][aArmour]=0;
	CheatPlayerInfo[playerid][aHealth]=0;
	CheatPlayerInfo[playerid][aGainHealth]=0;
	#if defined MODE_DELAY_SEND_STATS
	CheatPlayerInfo[playerid][aGiveWeaponTime]=0;
	CheatPlayerInfo[playerid][aGiveMoneyTime]=0;
	CheatPlayerInfo[playerid][aGiveHealthTime]=0;
	CheatPlayerInfo[playerid][aGiveArmourTime]=0;
	#endif
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnPlayerDisconnect", "dd", playerid, reason);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect ELC_AC_OnPlayerDisconnect
forward ELC_AC_OnPlayerDisconnect(playerid, reason);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(IsPlayerConnected(playerid) && !IsPlayerNPC(playerid))
	{
		if(CheatPlayerInfo[playerid][aAntiWeaponHack]==1 && CheatPlayerInfo[playerid][aAntiAmmoBlockHack]==1)
		{
			if(newkeys==4)
			{
				new Float:XPOS[3]; GetPlayerPos(playerid,XPOS[0],XPOS[1],XPOS[2]);
				if(CheatPlayerInfo[playerid][aTickFire]==0 && IsNotWeaponVise(GetPlayerWeapon(playerid)) && CheatPlayerInfo[playerid][aZ]==XPOS[2])
				{
					CheatPlayerInfo[playerid][aTickFire]=GetTickCount();
					CheatPlayerInfo[playerid][aAmmoFire]=GetPlayerAmmo(playerid);
					CheatPlayerInfo[playerid][aWeaponFire]=GetPlayerWeapon(playerid);
				}
			}
			else if(newkeys==132)
			{
				if(CheatPlayerInfo[playerid][aTickFire]==0 && IsNotWeaponNoAmmo(GetPlayerWeapon(playerid)) && (GetPlayerSpeedXY(playerid) < 1))
				{
					CheatPlayerInfo[playerid][aTickFire]=GetTickCount();
					CheatPlayerInfo[playerid][aAmmoFire]=GetPlayerAmmo(playerid);
					CheatPlayerInfo[playerid][aWeaponFire]=GetPlayerWeapon(playerid);
				}
			}
			else if(oldkeys==4&&newkeys!=132 || oldkeys==132&&newkeys!=4)
			{
                if(CheatPlayerInfo[playerid][aTickFire]!=0)
                {
                    if((GetTickCount()-CheatPlayerInfo[playerid][aTickFire]) >= TIRE_RATE)
					{
						new Float:XPOS[3];
			            GetPlayerPos(playerid,XPOS[0],XPOS[1],XPOS[2]);
						if(CheatPlayerInfo[playerid][aZ]==XPOS[2] && CheatPlayerInfo[playerid][aWeaponFire]==GetPlayerWeapon(playerid)&& CheatPlayerInfo[playerid][aAmmoFire]==GetPlayerAmmo(playerid)) ELC_AC_StartCheckPlayerFaceWall(playerid);
					}
					CheatPlayerInfo[playerid][aTickFire]=0;
					CheatPlayerInfo[playerid][aAmmoFire]=0;
					CheatPlayerInfo[playerid][aWeaponFire]=0;
                }
			}
		}
	}
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnPlayerKeyStateChange", "ddd",playerid, newkeys, oldkeys);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange ELC_AC_OnPlayerKeyStateChange
forward ELC_AC_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(newstate == PLAYER_STATE_DRIVER)
    {
        CheatPlayerInfo[playerid][aPlayerEnterTime] = GetTickCount();
    }
    else if(oldstate == PLAYER_STATE_DRIVER)
    {
        if((GetTickCount()-CheatPlayerInfo[playerid][aPlayerEnterTime]) < 220)//player entered and exited vehicle faster than 220 ms.
        {
            CallRemoteFunction("OnPlayerCheat", "dd", playerid,9);
        }
        //GetVehiclePos(vehicleid, CheatVehicleInfo[vehicleid][vX], CheatVehicleInfo[vehicleid][vY], CheatVehicleInfo[vehicleid][vZ]);

    }
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnPlayerStateChange", "ddd",playerid, newstate, oldstate);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange ELC_AC_OnPlayerStateChange
forward ELC_AC_OnPlayerStateChange(playerid, newstate, oldstate);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat)
{
	new Float:oldposx, Float:oldposy, Float:oldposz;
	new Float:tempposx, Float:tempposy, Float:tempposz;
	GetVehiclePos(vehicleid, oldposx, oldposy, oldposz);
	tempposx = (oldposx -CheatVehicleInfo[vehicleid][vX]);
	tempposy = (oldposy -CheatVehicleInfo[vehicleid][vY]);
	tempposz = (oldposz -CheatVehicleInfo[vehicleid][vZ]);
	if (CheatVehicleInfo[vehicleid][vX]==0 || ((tempposx < MAX_VEHICLE_DISTANCE_UCM) && (tempposx > -MAX_VEHICLE_DISTANCE_UCM)) && ((tempposy < MAX_VEHICLE_DISTANCE_UCM) && (tempposy > -MAX_VEHICLE_DISTANCE_UCM)) && ((tempposz < MAX_VEHICLE_DISTANCE_UCM) && (tempposz > -MAX_VEHICLE_DISTANCE_UCM)))
	{
		CheatVehicleInfo[vehicleid][vX]=oldposx;
		CheatVehicleInfo[vehicleid][vY]=oldposy;
		CheatVehicleInfo[vehicleid][vZ]=oldposz;
		GetVehicleZAngle(vehicleid,CheatVehicleInfo[vehicleid][vAngle]);
	}
	else if(playerid!=INVALID_PLAYER_ID && IsPlayerInRangeOfPoint(playerid,MAX_VEHICLE_DISTANCE_UCM,oldposx,oldposy,oldposz))
	{
	    if(CheatVehicleInfo[vehicleid][vPossiblePlayerHack]==0)
	    {
            CheatVehicleInfo[vehicleid][vPossiblePlayerHack]=playerid+1;
            SaveTime(CheatVehicleInfo[vehicleid][vPossibleHackTime],GetTickCount());
            CheatPlayerInfo[playerid][aPossibleVehicleHack]=vehicleid;
		}
		else if(CheatVehicleInfo[vehicleid][vPossiblePlayerHack]!=playerid+1)
		{
            CheatVehicleInfo[vehicleid][vPossiblePlayerHack]=0;
            CheatVehicleInfo[vehicleid][vPossibleHackTime]=0;
            if(IsPlayerConnected(CheatVehicleInfo[vehicleid][vPossiblePlayerHack]-1))CheatPlayerInfo[CheatVehicleInfo[vehicleid][vPossiblePlayerHack]-1][aPossibleVehicleHack]=0;
            SetVehiclePos(vehicleid, CheatVehicleInfo[vehicleid][vX], CheatVehicleInfo[vehicleid][vY], CheatVehicleInfo[vehicleid][vZ]);
            SetVehicleZAngle(vehicleid,CheatVehicleInfo[vehicleid][vAngle]);
		}
	}
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnUnoccupiedVehicle", "ddd",vehicleid, playerid, passenger_seat);
	#else
	return 1;
	#endif
}

/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnUnoccupiedVehicleUpdate
	#undef OnUnoccupiedVehicleUpdate
#else
	#define _ALS_OnUnoccupiedVehicleUpdate
#endif
#define OnUnoccupiedVehicleUpdate ELC_AC_OnUnoccupiedVehicle
forward ELC_AC_OnUnoccupiedVehicle(vehicleid, playerid, passenger_seat);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnVehicleSpawn(vehicleid)
{
    GetVehiclePos(vehicleid, CheatVehicleInfo[vehicleid][vX], CheatVehicleInfo[vehicleid][vY], CheatVehicleInfo[vehicleid][vZ]);
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnVehicleSpawn", "d",vehicleid);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnVehicleSpawn
	#undef OnVehicleSpawn
#else
	#define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn ELC_AC_OnVehicleSpawn
forward ELC_AC_OnVehicleSpawn(vehicleid);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public OnVehicleDeath(vehicleid)
{
    CheatVehicleInfo[vehicleid][vX]=0;
	#if defined INCLUDE_BASE_MODE
	return CallLocalFunction("ELC_AC_OnVehicleDeath", "d",vehicleid);
	#else
	return 1;
	#endif
}
/*----------------------------------------------------------------------------*/
#if defined INCLUDE_BASE_MODE
//ALS_SYSTEME --------------------
#if defined _ALS_OnVehicleDeath
	#undef OnVehicleDeath
#else
	#define _ALS_OnVehicleDeath
#endif
#define OnVehicleDeath ELC_AC_OnVehicleDeath
forward ELC_AC_OnVehicleDeath(vehicleid);
#endif
//ALS_OFF_SYSTEME ----------------
/*----------------------------------------------------------------------------*/
public ELC_AC_EnablePlayerCheatID(playerid,cheatid,enable)
{
	if(cheatid==1)//Money
	{
		if(enable==1)
		{
			CheatPlayerInfo[playerid][aAntiMoneyHack]=1;
		}
		else CheatPlayerInfo[playerid][aAntiMoneyHack]=0;
	}
	else if(cheatid==2)//Weapon
	{
		if(enable==1)
		{
			for(new i = 0; i <13; i++)
			{
				GetPlayerWeaponData(playerid,i,CheatPlayerInfo[playerid][aWeapon][i],CheatPlayerInfo[playerid][aAmmo][i]);
			}
			CheatPlayerInfo[playerid][aAntiWeaponHack]=1;
		}
		else CheatPlayerInfo[playerid][aAntiWeaponHack]=0;
	}
	else if(cheatid==3)//AirBreak/SpeedHack
	{
		if(enable==1) CheatPlayerInfo[playerid][aAntiSpeedHack]=1;
		else CheatPlayerInfo[playerid][aAntiSpeedHack]=0;
	}
	else if(cheatid==4)//Anti-Teleport
	{
		if(enable==1)
		{
			GetPlayerPos(playerid,CheatPlayerInfo[playerid][aX],CheatPlayerInfo[playerid][aY],CheatPlayerInfo[playerid][aZ]);
			CheatPlayerInfo[playerid][aAntiTeleportHack]=1;
		}
		else CheatPlayerInfo[playerid][aAntiTeleportHack]=0;
	}
	else if(cheatid==5)//Anti-Ammo_Block
	{
		if(enable==1)
		{
			CheatPlayerInfo[playerid][aTickFire]=0;
			CheatPlayerInfo[playerid][aAmmoFire]=0;
			CheatPlayerInfo[playerid][aWeaponFire]=0;
			CheatPlayerInfo[playerid][aAntiAmmoBlockHack]=1;
		}
		else CheatPlayerInfo[playerid][aAntiAmmoBlockHack]=0;
	}
	else if(cheatid==6)//Anti_HEALTH_HACK
	{
		if(enable==1)
		{
			CheatPlayerInfo[playerid][aAntiHealthHack]=1;
		}
		else CheatPlayerInfo[playerid][aAntiHealthHack]=0;
	}
	else if(cheatid==7)//Anti_ARMOUR_HACK
	{
		if(enable==1)
		{
			CheatPlayerInfo[playerid][aAntiArmourHack]=1;
		}
		else CheatPlayerInfo[playerid][aAntiArmourHack]=0;
	}
	return 1;
}
public RemovePlayerWeapon(playerid, weaponid)
{
    if(CheatPlayerInfo[playerid][aAntiWeaponHack]==1)CheatPlayerInfo[playerid][aAntiWeaponHack]=2;
    new WeaponsID[13];
    new AmmoID[13];
    new WepUse=GetPlayerWeapon(playerid);
    new slotremove=GetWeaponSlot(weaponid);
    for(new slot = 0; slot <13; slot++)
    {
        if(slotremove!=slot)GetPlayerWeaponData(playerid, slot, WeaponsID[slot], AmmoID[slot]);
    }
    initial_ResetPlayerWeapons(playerid);
    for(new slot = 0; slot <13; slot++)if(WeaponsID[slot]!=0 && AmmoID[slot] != 0)initial_GivePlayerWeapon(playerid, WeaponsID[slot], AmmoID[slot]);
    SetPlayerArmedWeapon(playerid,WepUse);
	if(CheatPlayerInfo[playerid][aAntiWeaponHack]==2)CheatPlayerInfo[playerid][aAntiWeaponHack]=1;
    return 1;
}
/*----------------------------------------------------------------------------*/
#define EnablePlayerCheatID ELC_AC_EnablePlayerCheatID
/*----------------------------------------------------------------------------*/

