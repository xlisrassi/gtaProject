/*

	Dynamic Vehicle System 1.0 - by Tee

*/

#include <a_samp>
#include <zcmd>
#include <a_mysql>
#include <GetVehicleColor>
#include <sscanf2>
#include <zone>
/*
---------------------------------
Natives

native LoaddVehicles();
native GetFreedID();
native IsdVehicle(vehicleid);
native GetdVehicleID(vehicleid);
native AdddMods(dvehid);
native SavedMods(dvehid);
---------------------------------
*/

#define Grey                      0xC0C0C0FF
#define MAX_D_VEH                 300
#define VEHICLE_TUNE              1

enum dveh
{
 	dVehicle,
	dModel,
	Float:dX,
	Float:dY,
	Float:dZ,
	Float:dA,
	dCol1,
	dCol2,
	dPlate[8],
	dMod[14],
	dLocation[25],
}
new dVeh[MAX_D_VEH][dveh];
new dstring[128],dquery[300],c1,c2,dfield[128][32],drow[128],dmodvehicle[MAX_PLAYERS];

public OnPlayerConnect(playerid)
{
    dmodvehicle[playerid] = 0;
 	return CallLocalFunction("d_OnPlayerConnect", "d", playerid);
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	new v = GetdVehicleID(vehicleid);
	if(vehicleid == dVeh[v][dVehicle] && IsPlayerAdmin(playerid) && dmodvehicle[playerid] == 1)
	{
	    print("vehid complete");
		for(new i=0; i<14; i++)
		{
			if(GetVehicleComponentType(componentid) == i)
		   	{
        		print("veh component complete");
		    	dVeh[v][dMod][i] = componentid;
			}
		}
		SavedMods(v);
		print("saving mods");
	}
	return CallLocalFunction("d_OnVehicleMod", "ddd", playerid, vehicleid, componentid);
}

public OnVehicleSpawn(vehicleid)
{
	if(vehicleid == dVeh[GetdVehicleID(vehicleid)][dVehicle])
	{
	    AdddMods(GetdVehicleID(vehicleid));
	}
	return CallLocalFunction("d_OnVehicleSpawn", "d", vehicleid);
}

COMMAND:setdveh(playerid, params[])
{
	if(!IsPlayerAdmin(playerid))return 0;
	new v = GetFreedID();
	if(GetPlayerVehicleID(playerid) == 0)return SendClientMessage(playerid,Grey,"You are not in a vehicle.");
	dVeh[v][dModel] = GetVehicleModel(GetPlayerVehicleID(playerid));
	GetVehiclePos(GetPlayerVehicleID(playerid),dVeh[v][dX],dVeh[v][dY],dVeh[v][dZ]);
	GetVehicleZAngle(GetPlayerVehicleID(playerid),dVeh[v][dA]);
	GetVehicleColor(GetPlayerVehicleID(playerid),dVeh[v][dCol1],dVeh[v][dCol2]);
	GetPlayerZone(playerid,dstring,sizeof(dstring));
	format(dVeh[v][dLocation],25,"%s",dstring);

	dVeh[v][dVehicle] = CreateVehicle(dVeh[v][dModel],dVeh[v][dX],dVeh[v][dY],dVeh[v][dZ],dVeh[v][dA],dVeh[v][dCol1],dVeh[v][dCol2],-1);

	format(dquery,sizeof(dquery),"INSERT INTO dvehicles (`ID`,Model,X,Y,Z,Angle,Color1,Color2,Location) VALUES (%d,%d,%f,%f,%f,%f,%d,%d,'%s')",
	GetFreedID(),dVeh[v][dModel],dVeh[v][dX],dVeh[v][dY],dVeh[v][dZ],dVeh[v][dA],dVeh[v][dCol1],dVeh[v][dCol2],dVeh[v][dLocation]);
	mysql_query(dquery);

	SendClientMessage(playerid,0xFFFFA8FF,"Dynamic Vehicle - Vehicle Created");
	return 1;
}

COMMAND:setdcol(playerid, params[])
{
	if(!IsPlayerAdmin(playerid))return 0;
	new v = GetdVehicleID(GetPlayerVehicleID(playerid));
	if(GetPlayerVehicleID(playerid) == 0)return SendClientMessage(playerid,Grey,"You are not in a vehicle.");
	if(!IsdVehicle(GetPlayerVehicleID(playerid)))return SendClientMessage(playerid,Grey,"This is not a dVehicle.");
	if(sscanf(params,"ii",c1,c2))return SendClientMessage(playerid,Grey,"Usage: /setdcol [color1] [color2]");
	dVeh[v][dCol1] = c1;
	dVeh[v][dCol2] = c2;
	ChangeVehicleColor(GetPlayerVehicleID(playerid),c1,c2);
	
	format(dquery,sizeof(dquery),"UPDATE dvehicles SET Color1 = %d, Color2 = %d WHERE `ID` = %d",c1,c2,v);
	mysql_query(dquery);

	SendClientMessage(playerid,0xFFFFA8FF,"Dynamic Vehicle - Vehicle Color Changed!!");
	return 1;
}

COMMAND:setdplate(playerid, params[])
{
	if(!IsPlayerAdmin(playerid))return 0;
	new v = GetdVehicleID(GetPlayerVehicleID(playerid));
	if(GetPlayerVehicleID(playerid) == 0)return SendClientMessage(playerid,Grey,"You are not in a vehicle.");
	if(!IsdVehicle(GetPlayerVehicleID(playerid)))return SendClientMessage(playerid,Grey,"This is not a dVehicle.");
	if(isnull(params))return SendClientMessage(playerid,Grey,"Usage: /setdplate [text]]");
	if(strlen(params) > 8)return SendClientMessage(playerid,Grey,"Text cannot be more than 8 characters.");
	SetVehicleNumberPlate(GetPlayerVehicleID(playerid),params);
	SetVehicleToRespawn(GetPlayerVehicleID(playerid));
	new Float:p[3];
	GetPlayerPos(playerid,p[0],p[1],p[2]);
	SetPlayerPos(playerid,p[0],p[1],p[2]+3.0);
	
	format(dquery,sizeof(dquery),"UPDATE dvehicles SET Plate = '%s' WHERE `ID` = %d",params,v);
	mysql_query(dquery);
	
	format(dVeh[v][dPlate],8,"%s",params);
	SendClientMessage(playerid,0xFFFFA8FF,"Dynamic Vehicle - Vehicle Plate Changed!!");
	return 1;
}

COMMAND:deletedveh(playerid, params[])
{
	if(!IsPlayerAdmin(playerid))return 0;
	new vehicleid = GetPlayerVehicleID(playerid);
	new v = GetdVehicleID(vehicleid);
	if(vehicleid == 0)return SendClientMessage(playerid,Grey,"You are not in a vehicle.");
	if(!IsdVehicle(vehicleid))return SendClientMessage(playerid,Grey,"This is not a dVehicle.");
	DestroyVehicle(dVeh[v][dVehicle]);
	format(dquery,sizeof(dquery),"DELETE FROM dvehicles WHERE `ID` = %d",v);
	mysql_query(dquery);
	dVeh[v][dModel] = 0;
	dVeh[v][dX] = 0.0;
	dVeh[v][dY] = 0.0;
	dVeh[v][dZ] = 0.0;
	dVeh[v][dA] = 0.0;
	dVeh[v][dCol1] = 0;
	dVeh[v][dCol2] = 0;
	for(new i=0; i<14; i++)
	{
		if(dVeh[v][dMod][i] > 0)
		{
			RemoveVehicleComponent(GetPlayerVehicleID(playerid),dVeh[v][dMod][i]);
	    	dVeh[v][dMod][i] = 0;
		}
	}
    SendClientMessage(playerid,0xFFFFA8FF,"Dynamic Vehicle - Vehicle Deleted!");
	return 1;
}

COMMAND:removedmods(playerid, params[])
{
	if(!IsPlayerAdmin(playerid))return 0;
	new v = GetdVehicleID(GetPlayerVehicleID(playerid));
	if(GetPlayerVehicleID(playerid) == 0)return SendClientMessage(playerid,Grey,"You are not in a vehicle.");
	if(!IsdVehicle(GetPlayerVehicleID(playerid)))return SendClientMessage(playerid,Grey,"This is not a dVehicle.");
	for(new i=0; i<14; i++)
	{
		if(dVeh[v][dMod][i] > 0)
		{
			RemoveVehicleComponent(GetPlayerVehicleID(playerid),dVeh[v][dMod][i]);
            dVeh[v][dMod][i] = 0;
		}
	}
	SavedMods(v);
	SendClientMessage(playerid,0xFFFFA8FF,"Dynamic Vehicle - Modifications Removed!");
	return 1;
}

COMMAND:dmodcomplete(playerid, params[])
{
    if(!IsPlayerAdmin(playerid))return 0;
    if(dmodvehicle[playerid] == 0)return SendClientMessage(playerid,Grey,"You didn't start to mod a vehicle.");
    SendClientMessage(playerid,0xFFFFA8FF,"Dynamic Vehicle - Modification complete!");
    SendClientMessage(playerid,0xFFFFA8FF,"You can respawn the vehicle to test it.");
    dmodvehicle[playerid] = 0;
    return 1;
}

COMMAND:dmodstart(playerid, params[])
{
    if(!IsPlayerAdmin(playerid))return 0;
    if(dmodvehicle[playerid] == 1)return SendClientMessage(playerid,Grey,"You are already in the modification process.");
	SendClientMessage(playerid,0xFFFFA8FF,"You've chosen to tune this vehicle.");
 	SendClientMessage(playerid,0xBFFFBFFF,"All you need to do now, is go to a tune shop and add the desired modifications.");
  	SendClientMessage(playerid,0xBFFFBFFF,"When you're finished, type {FFFFFF}/dmodcomplete{BFFFBF}.");
	dmodvehicle[playerid] = 1;
	return 1;
}

COMMAND:dvcmds(playerid, params[])
{
    if(!IsPlayerAdmin(playerid))return 0;
	SendClientMessage(playerid,0xFFFFA8FF,"Dynamic Vehicle Commands:");
 	SendClientMessage(playerid,0xBFFFBFFF,"/setdveh, /setdcol, /setdplate, /deletedveh, /removedmods, /dmodstart, /dmodcomplete.");
	return 1;
}

stock GetFreedID()
{
	for(new i=1; i<MAX_D_VEH; i++)
	{
		format(dquery,sizeof(dquery),"SELECT * FROM dvehicles WHERE `ID` = %d LIMIT 1",i);
	 	mysql_query(dquery);
	 	mysql_store_result();
		if(mysql_num_rows() == 0)
		{
		    mysql_free_result();
			return i;
		}
		mysql_free_result();
	}
	return 0;
}

stock IsdVehicle(vehicleid)
{
	for(new i=1; i<MAX_D_VEH; i++)
 	{
  		if(vehicleid == dVeh[i][dVehicle])return 1;
	}
	return 0;
}

stock GetdVehicleID(vehicleid)
{
	for(new i=1; i<MAX_D_VEH; i++)
 	{
  		if(vehicleid == dVeh[i][dVehicle])return i;
	}
	return 0;
}

stock AdddMods(dvehid)
{
	for(new i=0; i<14; i++)
	{
	    if(dVeh[dvehid][dMod][i] > 0)AddVehicleComponent(dVeh[dvehid][dVehicle],dVeh[dvehid][dMod][i]);
	}
}

stock SavedMods(dvehid)
{
	mysql_debug(1);
	format(dquery,sizeof(dquery),"UPDATE dvehicles SET Mod1=%d,Mod2=%d,Mod3=%d,Mod4=%d,Mod5=%d,Mod6=%d,Mod7=%d,Mod8=%d,Mod9=%d,Mod10=%d,Mod11=%d,Mod12=%d,Mod13=%d,Mod14=%d WHERE `ID` = %d",
	dVeh[dvehid][dMod][0],dVeh[dvehid][dMod][1],dVeh[dvehid][dMod][2],dVeh[dvehid][dMod][3],dVeh[dvehid][dMod][4],dVeh[dvehid][dMod][5],dVeh[dvehid][dMod][6],dVeh[dvehid][dMod][7],dVeh[dvehid][dMod][8],
	dVeh[dvehid][dMod][9],dVeh[dvehid][dMod][10],dVeh[dvehid][dMod][11],dVeh[dvehid][dMod][12],dVeh[dvehid][dMod][13],dvehid);
	mysql_query(dquery);
	mysql_debug(0);
}

stock LoaddVehicles()
{
	new ccc;
	for(new i=1; i<MAX_D_VEH; i++)
	{
		format(dquery,55,"SELECT * FROM dvehicles WHERE `ID` = %d LIMIT 1",i);
	 	mysql_query(dquery);
		mysql_store_result();
		if(mysql_num_rows() != 0)
		{
		    ccc ++;
			mysql_fetch_row_format(drow, "|");
	  		d_explode(drow, dfield, "|");
	  		dVeh[i][dModel] = strval(dfield[1]);
	  		dVeh[i][dX] = floatstr(dfield[2]);
	  		dVeh[i][dY] = floatstr(dfield[3]);
	  		dVeh[i][dZ] = floatstr(dfield[4]);
	  		dVeh[i][dA] = floatstr(dfield[5]);
	  		dVeh[i][dCol1] = strval(dfield[6]);
	  		dVeh[i][dCol2] = strval(dfield[7]);
	  		format(dVeh[i][dPlate],8,"%s",dfield[8]);
	  		dVeh[i][dMod][0] = strval(dfield[9]);
	  		dVeh[i][dMod][1] = strval(dfield[10]);
	  		dVeh[i][dMod][2] = strval(dfield[11]);
	  		dVeh[i][dMod][3] = strval(dfield[12]);
	  		dVeh[i][dMod][4] = strval(dfield[13]);
	  		dVeh[i][dMod][5] = strval(dfield[14]);
	  		dVeh[i][dMod][6] = strval(dfield[15]);
	  		dVeh[i][dMod][7] = strval(dfield[16]);
	  		dVeh[i][dMod][8] = strval(dfield[17]);
	  		dVeh[i][dMod][9] = strval(dfield[18]);
	  		dVeh[i][dMod][10] = strval(dfield[19]);
	  		dVeh[i][dMod][11] = strval(dfield[20]);
	  		dVeh[i][dMod][12] = strval(dfield[21]);
	  		dVeh[i][dMod][13] = strval(dfield[22]);
	  		dVeh[i][dVehicle] = CreateVehicle(dVeh[i][dModel],dVeh[i][dX],dVeh[i][dY],dVeh[i][dZ],dVeh[i][dA],dVeh[i][dCol1],dVeh[i][dCol2],-1);
			AdddMods(i);
			SetVehicleNumberPlate(dVeh[i][dVehicle],dVeh[i][dPlate]);
			SetVehicleToRespawn(dVeh[i][dVehicle]);
			mysql_free_result();
		}
	}
	return printf("Total Vehicles Loaded: %d",ccc);
}

stock d_explode(const sSource[], aExplode[][], const sDelimiter[] = " ", iVertices = sizeof aExplode, iLength = sizeof aExplode[]) // Created by Westie
{
	new
		iNode,
		iPointer,
		iPrevious = -1,
		iDelimiter = strlen(sDelimiter);

	while(iNode < iVertices)
	{
		iPointer = strfind(sSource, sDelimiter, false, iPointer);

		if(iPointer == -1)
		{
			strmid(aExplode[iNode], sSource, iPrevious, strlen(sSource), iLength);
			break;
		}
		else
		{
			strmid(aExplode[iNode], sSource, iPrevious, iPointer, iLength);
		}

		iPrevious = (iPointer += iDelimiter);
		++iNode;
	}
	return iPrevious;
}

//Hooking

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect d_OnPlayerConnect

forward d_OnPlayerConnect(playerid);

#if defined _ALS_OnVehicleMod
    #undef OnVehicleMod
#else
    #define _ALS_OnVehicleMod
#endif
#define OnVehicleMod d_OnVehicleMod

forward d_OnVehicleMod(playerid, vehicleid, componentid);

#if defined _ALS_OnVehicleSpawn
    #undef OnVehicleSpawn
#else
    #define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn d_OnVehicleSpawn

forward d_OnVehicleSpawn(vehicleid);

/*forward d_AddVehiclesToDatabase();
public d_AddVehiclesToDatabase()
{
	for(new i; i<MAX_VEHICLES; i++)
	{
		new Float:p[5];
		GetVehiclePos(i,p[0],p[1],p[2]);
		GetVehicleZAngle(i,p[3]);
		GetVehicleColor(i,c1,c2);

		format(dquery,sizeof(dquery),"INSERT INTO dvehicles (ID,Model,X,Y,Z,Angle,Color1,Color2) VALUES  (%d,%d,%f,%f,%f,%f,c1,c2)",
	 	GetFreedID(),GetVehicleModel(i),p[0],p[1],p[2],p[3],c1,c2);
		mysql_query(dquery);
	}
	return 1;
}
*/
