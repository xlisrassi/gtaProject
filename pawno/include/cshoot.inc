/*  cshoot.inc
 *
 *  (c) Copyright 2012, Emilijo "Correlli" Lovrich
 *
 *  Credits: - Y_Less for foreach & randomEx function,
			 - whoever created the IsPlayerAimingAt and other functions for it.
*/

#if defined _cshoot_included
	#endinput
#endif
#define _cshoot_included

#if !defined _samp_included
	#error "You'll need to include the a_samp.inc include file before the cshoot.inc file!"
#endif

/* ----- */

#if !defined MAX_COWS
	#define MAX_COWS (3)
#endif

#if !defined INVALID_COW_ID
	#define INVALID_COW_ID (-1)
#endif

#if !defined ENABLE_SOUND
	#define ENABLE_SOUND (0)
#endif

#if !defined MIN_COW_SPEED
	#define MIN_COW_SPEED (1.0)
#endif

#if !defined MAX_COW_SPEED
	#define MAX_COW_SPEED (5.0)
#endif

#if !defined MIN_COW_HEALTH
	#define MIN_COW_HEALTH (10)
#endif

#if !defined MAX_COW_HEALTH
	#define MAX_COW_HEALTH (200)
#endif

#if !defined C_MOVE_DELAY
	#define C_MOVE_DELAY (3)
#endif

#if !defined RespawnCow
	#define RespawnCow(%0,%1) ResurrectCow(%0,%1)
#endif

/* ----- */

static
			bool:gOPF = false;

Itter_Create(Cows, MAX_COWS);

forward OnCowDeath(cowid, killerid, reason);
forward OnCowTakeDamage(cowid, shooterid, reason, damage);

enum dataCow
{
	ObjectID,
	Float:Pos[3],
	Health,
	Float:Speed,
	Float:Range
}

new
		cowID = INVALID_COW_ID, cowData[MAX_COWS][dataCow],
		bool:firstCow = false, cTimer[2] = 0;

/* ----- */

stock CreateCow(Float:x, Float:y, Float:z, health, Float:speed, Float:range)
{
	cowID++;
	if(cowID == MAX_COWS)
	{
		printf("Error: Limit of cows exceeded. Open the cshoot.inc and change the \"MAX_COWS\" define to bigger value if you want to have more cows.\
				\nDon't forget that object limit in the current SA:MP version (0.3e) is 1000, so it's a good thing to use an object-streamer if you have many cows on server.");
		cowID--;
		return INVALID_COW_ID;
	}
	new
			toCowID = GetFreeCowID();
	if(toCowID == INVALID_COW_ID)
	{
		print("Error: There are no cow IDs free.");
		cowID--;
		return INVALID_COW_ID;
	}
	for(new a = 0; a < MAX_COWS; a++)
	{
		if(cowData[a][Pos][0] == x)
		{
			cowID--;
			print("Error: There is already a cow at that position.");
			return INVALID_COW_ID;
		}
	}
	Itter_Add(Cows, toCowID);
	cowData[toCowID][Pos][0]      =        x;
	cowData[toCowID][Pos][1]      =        y;
	cowData[toCowID][Pos][2]      =        z;
	if(health > MAX_COW_HEALTH) cowData[toCowID][Health]         = MAX_COW_HEALTH;
	else if(health < MIN_COW_HEALTH) cowData[toCowID][Health]    = MIN_COW_HEALTH;
	else cowData[toCowID][Health] =   health;
	if(speed > MAX_COW_SPEED) cowData[toCowID][Speed]            =  MAX_COW_SPEED;
	else if(speed < MIN_COW_SPEED) cowData[toCowID][Speed]       =  MIN_COW_SPEED;
	else cowData[toCowID][Speed]  =    speed;
	cowData[toCowID][Range]       =    range;
	cowData[toCowID][ObjectID] = CreateObject(16442, x, y, z, 0.0, 0.0, 0.0);
	if(!firstCow)
	{
		firstCow = true;
		cTimer[0] = SetTimer("cowTimer", 1000, true);
	}
	return toCowID;
}

stock DestroyCow(cowid)
{
	if(!IsCowCreated(cowid)) return false;
	cowID--;
	Itter_Remove(Cows, cowid);
	cowData[cowid][Pos][0] = 0.0;
	cowData[cowid][Pos][1] = 0.0;
	cowData[cowid][Pos][2] = 0.0;
	cowData[cowid][Health] =   0;
	cowData[cowid][Speed]  = 0.0;
	cowData[cowid][Range]  = 0.0;
	DestroyObject(cowData[cowid][ObjectID]);
	if(cowID == INVALID_COW_ID)
	{
		firstCow = false;
		KillTimer(cTimer[0]);
	}
	return true;
}

stock IsCowCreated(cowid)
{
	if(cowData[cowid][Pos][0]) return true;
	return false;
}

stock SetCowHealth(cowid, health)
{
	if(!IsCowCreated(cowid)) return false;
	if(health > MAX_COW_HEALTH) cowData[cowid][Health] = MAX_COW_HEALTH;
	else if(health < 0) cowData[cowid][Health] = 0;
	else cowData[cowid][Health] = health;
	if(!cowData[cowid][Health]) KillCow(cowid);
	return true;
}

stock GetCowHealth(cowid)
{
	if(!IsCowCreated(cowid)) return false;
	return cowData[cowid][Health];
}

stock KillCow(cowid)
{
	if(!IsCowCreated(cowid)) return false;
	new
			Float:cPos[6];
	cowData[cowid][Health] = 0;
	StopObject(cowData[cowid][ObjectID]);
	GetObjectPos(cowData[cowid][ObjectID], cPos[0],       cPos[1],        cPos[2]);
	SetObjectPos(cowData[cowid][ObjectID], cPos[0], cPos[1] + 1.5, cPos[2] - 0.75);
	GetObjectRot(cowData[cowid][ObjectID], cPos[3],       cPos[4],        cPos[5]);
	SetObjectRot(cowData[cowid][ObjectID],   270.0,       cPos[4],        cPos[5]);
	return true;
}

stock ResurrectCow(cowid, health)
{
	if(!IsCowCreated(cowid) || !IsCowDead(cowid)) return false;
	new
			Float:cPos[3];
	if(health > MAX_COW_HEALTH) cowData[cowid][Health] = MAX_COW_HEALTH;
	else if(health < MIN_COW_HEALTH) cowData[cowid][Health] = MIN_COW_HEALTH;
	else cowData[cowid][Health] = health;
	GetObjectPos(cowData[cowid][ObjectID], cPos[0],       cPos[1],                cPos[2]);
	SetObjectPos(cowData[cowid][ObjectID], cPos[0], cPos[1] - 1.5, cowData[cowid][Pos][2]);
	SetObjectRot(cowData[cowid][ObjectID],     0.0,           0.0,                    0.0);
	return true;
}

stock IsCowDead(cowid)
{
	if(!IsCowCreated(cowid)) return false;
	if(!cowData[cowid][Health]) return true;
	return false;
}

stock SetCowAngleToPos(cowid, Float:x, Float:y)
{
	if(!IsCowCreated(cowid) || IsCowDead(cowid)) return false;
	new
			Float:cPos[4];
	GetObjectPos(cowData[cowid][ObjectID], cPos[0], cPos[1], cPos[2]);
	if(y > cPos[1]) cPos[3] = (-acos((x - cPos[0]) / floatsqroot((x - cPos[0]) * (x - cPos[0]) + (y - cPos[1]) * (y - cPos[1]))) - 90.0);
	else if(y < cPos[1] && x < cPos[0]) cPos[3] = (acos((x - cPos[0]) / floatsqroot((x - cPos[0]) * (x - cPos[0]) + (y - cPos[1]) * (y - cPos[1]))) - 450.0);
	else if(y < cPos[1]) cPos[3] = (acos((x - cPos[0]) / floatsqroot((x - cPos[0]) * (x - cPos[0]) + (y - cPos[1]) * (y - cPos[1]))) - 90.0);
	if(x > cPos[0]) cPos[3] = (floatabs(floatabs(cPos[3]) + 180.0));
	else cPos[3] = (floatabs(cPos[3]) - 180.0);
	GetObjectRot(cowData[cowid][ObjectID], cPos[0], cPos[1], cPos[2]);
	SetObjectRot(cowData[cowid][ObjectID], cPos[0], cPos[1], cPos[3] + 90.0);
 	return true;
}

stock GetNumberOfCows() return cowID;

stock GetFreeCowID()
{
	new
			cowid = INVALID_COW_ID;
	for(new a = 0; a < MAX_COWS; a++)
	{
		if(!IsCowCreated(a))
		{
			cowid = a;
			break;
		}
	}
	return cowid;
}

/* ----- */

stock IsObjectInRangeOfPoint(objectid, Float:range, Float:x, Float:y, Float:z)
{
	new
			Float:pos[3];
	GetObjectPos(objectid, pos[0], pos[1], pos[2]);
	pos[0] -= x, pos[1] -= y, pos[2] -= z;
	return ((pos[0] * pos[0]) + (pos[1] * pos[1]) + (pos[2] * pos[2])) < (range * range);
}

stock randomEx(minnum = cellmin, maxnum = cellmax) return random(maxnum - minnum + 1) + minnum;

stock Float:GetPlayerDistanceToPoint(playerid, {Float,_}:...)
{
	if(getarg(1) == 0.0 && getarg(2) == 0.0 && getarg(3) == 0.0) return (-1.0);
	new
			num = numargs(), Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	if(num == 3) return floatsqroot(floatpower(floatabs(floatsub(pos[0], Float:getarg(1))), 2)
						+ floatpower(floatabs(floatsub(pos[1], Float:getarg(2))), 2));
	else if(num == 4) return floatsqroot(floatpower(floatabs(floatsub(pos[0], Float:getarg(1))), 2)
							+ floatpower(floatabs(floatsub(pos[1], Float:getarg(2))), 2)
							+ floatpower(floatabs(floatsub(pos[2], Float:getarg(3))), 2));
	return (-1.0);
}

Float:DistanceCameraTargetToLocation(Float:cam_x, Float:cam_y, Float:cam_z, Float:obj_x, Float:obj_y, Float:obj_z, Float:fr_x, Float:fr_y, Float:fr_z)
{
	new
			Float:pos[4];
	pos[3] = floatsqroot((cam_x - obj_x) * (cam_x - obj_x) + (cam_y - obj_y) * (cam_y - obj_y) + (cam_z - obj_z) * (cam_z - obj_z));
	pos[0] = fr_x * pos[3] + cam_x;
	pos[1] = fr_y * pos[3] + cam_y;
	pos[2] = fr_z * pos[3] + cam_z;
	return floatsqroot((pos[0] - obj_x) * (pos[0] - obj_x) + (pos[1] - obj_y) * (pos[1] - obj_y) + (pos[2] - obj_z) * (pos[2] - obj_z));
}

stock Float:GetPointAngleToPoint(Float:x2, Float:y2, Float:x1, Float:y1)
{
	new
			Float:pos[3];
	pos[0] = floatabs(floatsub(x2, x1));
	pos[1] = floatabs(floatsub(y2, y1));
	if(pos[0] == 0.0 || pos[1] == 0.0)
	{
		if((pos[0] > 0 && pos[1] == 0) || (pos[0] == 0 && pos[1] == 0)) pos[2] = 0.0;
		else if(pos[0] < 0 && pos[1] == 0) pos[2] = 180.0;
		else if(pos[0] == 0 && pos[1] > 0) pos[2] = 90.0;
		else if(pos[0] == 0 && pos[1] < 0) pos[2] = 270.0;
	}
	else
	{
		pos[2] = atan(pos[0] / pos[1]);
		if(x1 > x2 && y1 <= y2) pos[2] += 90.0;
		else if(x1 <= x2 && y1 < y2) pos[2] = floatsub(90.0, pos[2]);
		else if(x1 < x2 && y1 >= y2) pos[2] -= 90.0;
		else if(x1 >= x2 && y1 > y2) pos[2] = floatsub(270.0, pos[2]);
	}
	return floatadd(pos[2], 90.0);
}

stock GetXYInFrontOfPoint(&Float:x, &Float:y, Float:angle, Float:distance)
{
	x += (distance * floatsin(-angle, degrees));
	y += (distance * floatcos(-angle, degrees));
}

stock IsPlayerAimingAt(playerid, Float:x, Float:y, Float:z, Float:distance, Float:radius)
{
	if(GetPlayerDistanceToPoint(playerid, x, y, z) > distance) return false;
	new
			Float:pos[12];
	GetPlayerCameraPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerCameraFrontVector(playerid, pos[3], pos[4], pos[5]);
	switch(GetPlayerWeapon(playerid))
	{
		case 34, 35, 36:
			return (radius >= DistanceCameraTargetToLocation(pos[0], pos[1], pos[2], x, y, z, pos[3], pos[4], pos[5]));
		case 30, 31:
		{
			pos[6] = 4.0;
			pos[7] = -1.6;
		}
		case 33:
		{
			pos[6] = 2.7;
			pos[7] = -1.0;
		}
		default:
		{
			pos[6] = 6.0;
			pos[7] = -2.2;
		}
	}
	pos[8] = GetPointAngleToPoint(0, 0, floatsqroot(pos[3] * pos[3] + pos[4] * pos[4]), pos[5]) - 270.0;
	pos[11] = floatsin(pos[8] + pos[6], degrees);
	GetXYInFrontOfPoint(pos[9], pos[10], GetPointAngleToPoint(0, 0, pos[3], pos[4]) + pos[7], floatcos(pos[8] + pos[6], degrees));
	return (radius >= DistanceCameraTargetToLocation(pos[0], pos[1], pos[2], x, y, z, pos[9], pos[10], pos[11]));
}

stock GetPlayerWeaponSlot(playerid)
{
	new
			slotid = -1;
	switch(GetPlayerWeapon(playerid))
	{
		case         0, 1: slotid =  0;
		case         2..9: slotid =  1;
		case     22 .. 24: slotid =  2;
		case     25 .. 27: slotid =  3;
		case   28, 29, 32: slotid =  4;
		case       30, 31: slotid =  5;
		case       33, 34: slotid =  6;
		case     35 .. 38: slotid =  7;
		case 16 .. 19, 39: slotid =  8;
		case     41 .. 43: slotid =  9;
		case     10 .. 15: slotid = 10;
		case     44 .. 46: slotid = 11;
		case           40: slotid = 12;
	}
	return slotid;
}

stock GetPlayerWeaponAmmo(playerid)
{
	new
			slotid = GetPlayerWeaponSlot(playerid);
	if(slotid == -1) return -1;
	new
			weaponAmmo[2];
	GetPlayerWeaponData(playerid, slotid, weaponAmmo[0], weaponAmmo[1]);
	return weaponAmmo[1];
}

stock Float:GetRangeFromWeapon(weaponid)
{
	new
			Float:distance;
	switch(weaponid)
	{
		case 22..28, 32: distance =  50.0;
		case         29: distance =  65.0;
		case 30, 31, 33: distance =  80.0;
		case         34: distance = 100.0;
		case         37: distance =  15.0;
		case         38: distance =  75.0;
		default:         distance =  -1.0;
	}
	return distance;
}

/* ----- */

forward cowTimer();
public cowTimer()
{
	if(cTimer[1] == C_MOVE_DELAY)
	{
		foreach(Cows, a)
		{
			if(!IsCowDead(a))
			{
				new
						Float:cPos[2];
				if(IsObjectInRangeOfPoint(cowData[a][ObjectID],
					cowData[a][Range],
					cowData[a][Pos][0],
					cowData[a][Pos][1],
					cowData[a][Pos][2]))
				{
					cPos[0] = cowData[a][Pos][0] + float(randomEx(floatround(cowData[a][Range] / 2), floatround(cowData[a][Range])));
					cPos[1] = cowData[a][Pos][1] + float(randomEx(floatround(cowData[a][Range] / 2), floatround(cowData[a][Range])));
					MoveObject(cowData[a][ObjectID], cPos[0], cPos[1], cowData[a][Pos][2], cowData[a][Speed]);
				}
				else
				{
					cPos[0] = cowData[a][Pos][0];
					cPos[1] = cowData[a][Pos][1];
					MoveObject(cowData[a][ObjectID], cPos[0], cPos[1], cowData[a][Pos][2], cowData[a][Speed]);
				}
				SetCowAngleToPos(a, cPos[0], cPos[1]);
			}
		}
		cTimer[1] = 0;
	}
	else cTimer[1]++;
	return true;
}

forward OnPlayerAmmoChange(playerid, slotid, newammo, oldammo);
public OnPlayerAmmoChange(playerid, slotid, newammo, oldammo)
{
	if(newammo < oldammo)
	{
		if(cowID != INVALID_COW_ID)
		{
			new
					keys[3];
			GetPlayerKeys(playerid, keys[0], keys[1], keys[2]);
			if(keys[0] & KEY_HANDBRAKE)
			{
				new
						Float:cPos[3], weaponid = GetPlayerWeapon(playerid), damage;
				switch(weaponid)
				{
					case     22, 23: damage = randomEx(15, 20);
					case         24: damage = randomEx(25, 30);
					case         25: damage = randomEx(30, 40);
					case         26: damage = randomEx(20, 25);
					case     27, 33: damage = randomEx(30, 35);
					case 28, 29, 32: damage = randomEx(10, 15);
					case     30, 31: damage = randomEx(15, 25);
					case         34: damage = randomEx(60, 75);
					case         37: damage = randomEx(15, 30);
					case         38: damage = randomEx(40, 50);
				}
				foreach(Cows, a)
				{
					GetObjectPos(cowData[a][ObjectID], cPos[0], cPos[1], cPos[2]);
					if(!IsCowDead(a))
					{
						if(IsPlayerAimingAt(playerid, cPos[0], cPos[1], cPos[2], GetRangeFromWeapon(weaponid), 1.75))
						{
							#if ENABLE_SOUND
								PlayAudioStreamForPlayer(playerid, "http://k006.kiwi6.com/hotlink/p78afaje3s/moo.mp3");
							#endif
							new
									nHealth = GetCowHealth(a) - damage;
							SetCowHealth(a, nHealth);
							if(nHealth <= 0)
							{
								KillCow(a);
								CallLocalFunction("OnCowDeath", "iii", a, playerid, weaponid);
							}
							else CallLocalFunction("OnCowTakeDamage", "iiii", a, playerid, weaponid, damage);
							break;
						}
					}
				}
			}
		}
	}
	return true;
}

public OnPlayerUpdate(playerid)
{
	new
			keys[3], pAmmo[2];
	GetPlayerKeys(playerid, keys[0], keys[1], keys[2]);
	SetPVarInt(playerid, "Player_NewAmmo", GetPlayerWeaponAmmo(playerid));
	pAmmo[0] = GetPVarInt(playerid, "Player_NewAmmo");
	pAmmo[1] = GetPVarInt(playerid, "Player_OldAmmo");
	if(pAmmo[0] != pAmmo[1] && keys[0] & KEY_FIRE)
	{
		OnPlayerAmmoChange(playerid, GetPlayerWeaponSlot(playerid), pAmmo[0], pAmmo[1]);
		SetPVarInt(playerid, "Player_OldAmmo", pAmmo[0]);
	}

	if(gOPF) CallLocalFunction("cshoot_OnPlayerUpdate", "i", playerid);
	return true;
}

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate cshoot_OnPlayerUpdate

forward cshoot_OnPlayerUpdate(playerid);



public OnGameModeInit()
{
	gOPF = (funcidx("cshoot_OnPlayerUpdate") != -1);
	CallLocalFunction("cshoot_OnGameModeInit", "");
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit cshoot_OnGameModeInit

forward cshoot_OnGameModeInit();
