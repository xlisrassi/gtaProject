// Anti Vehicle Mod Crashes v1.0 by Amit_B
#include "a_samp.inc"
new bool:VMC_mod[2][MAX_PLAYERS] = {{false,...},{false,...}}, lastcomp[MAX_PLAYERS] = {-1,...}, warns[MAX_PLAYERS] = {0,...};
stock VMC_Reset(playerid) return (VMC_mod[0][playerid] = false, VMC_mod[1][playerid] = false), 1;
stock bool:VMC_OnVehicleMod(playerid, vehicleid, componentid)
{
	if(VMC_mod[1][playerid]) VMC_mod[1][playerid] = false;
	else
	{
		new bool:b = false;
		if(!VMC_mod[0][playerid]) b = true;
		else
		{
			if(lastcomp[playerid] == -1) lastcomp[playerid] = GetTickCount();
			else
			{
				new x = (GetTickCount() - lastcomp[playerid]);
				if(x <= 500)
				{
					if(x <= 100) b = true;
					else
					{
						warns[playerid]++;
						if(warns[playerid] >= 5) b = true;
					}
				}
				else if(warns[playerid] > 0) warns[playerid]--;
			}
		}
		if(b)
		{
			BanEx(playerid,"Vehicle Mod Crashes"); // You can choose here what to do with the hacker
			RemoveVehicleComponent(vehicleid,componentid);
			return true;
		}
	}
	return false;
}
stock VMC_OnEnterExitModShop(playerid, enterexit, interiorid=0)
#pragma unused interiorid
return (VMC_mod[0][playerid] = bool:enterexit), 1;
stock VMC_AddVehicleComponent(vehicleid, componentid, playerid = -1)
{
	if(playerid == -1) for(new i = 0; i < MAX_PLAYERS && playerid == -1; i++) if(IsPlayerConnected(i) && IsPlayerInVehicle(i,vehicleid) && GetPlayerState(i) == PLAYER_STATE_DRIVER) playerid = i;
	if(playerid != -1) VMC_mod[1][playerid] = true;
	return AddVehicleComponent(vehicleid,componentid);
}
#define AddVehicleComponent VMC_AddVehicleComponent
