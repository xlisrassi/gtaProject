/*
	Paddy's Map Loader (PML) - A include that can load .map and/ or .pml map files.
	
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	(The text above is not stolen from the GNU GPL xD)
	
	Feel free to edit this code to fit your needs.
	(If you do so, please give me credits for my work)
	
	Author: Paddy_Corleone
	Version: 0.2
	Creation Date: 2013/03/04
	
	Changes in 0.2:
	[ADD] Added LoadMTAMapToArrays/ LoadSAMPMapToArrays so you can load maps to arrays in you Gamemode (usefull for gates and stuff...)
	[ADD] Added a define for vehicle respawn-rate for LoadMTAMap/ LoadMTAMapToArrays
	[FIX] You can now spawn vehicles at any time with LoadMTAMap/ LoadMTAMapToArrays
	
	For updates and requests visit: http://forum.sa-mp.com/member.php?u=199151
*/

#if defined _PML_INCLUDED
	#endinput
#endif
#define _PML_INCLUDED
#define PML_VERSION "0.2"
#define PML_MAX_MAPS 128
#define PML_MAX_MAP_OBJECTS 500
#define PML_MAX_MAP_VEHICLES 128
#define PML_MTA_RESPAWN_RATE 120 // Respawn rate of MTA map vehicles in seconds

enum Maps
{
	mapname[64],
	objects[PML_MAX_MAP_OBJECTS],
	vehicles[PML_MAX_MAP_VEHICLES]
};
new MI[PML_MAX_MAPS][Maps], curmap;

/*
	native LoadMTAMap(src[], Float:dist = 90.0);
	native LoadSAMPMap(src[], Float:dist = 90.0);
	native LoadMTAMapToArrays(src[], objar[], vehar[], Float:dist = 90.0);
	native LoadSAMPMapToArrays(src[], objar[], vehar[], Float:dist = 90.0);
	native RemoveMapObjects(map[]);
	native RemoveMapVehicles(map[]);
	native RespawnMapVehicles(map[]);
	native RemoveMap(map[]);
*/

stock pmlsplit(src[], dest[][], del = '|')
{
	new start, cur;
	for(new i; i < strlen(src) + 1; i++)
	    if(src[i] == del || i == strlen(src))
	    {
    		strmid(dest[cur], src, start, i, i - start + 1);
    		cur++;
    		start = i + 1;
	    }
}

stock GetFreeMap()
{
	for(new i = 1; i < PML_MAX_MAPS; i++)
	{
		if(MI[i][objects][0] > 0 || MI[i][vehicles][0] > 0)continue;
		curmap = i;
		return 1;
	}
	return 0;
}

stock GetMapByName(map[])
{
	for(new i = 1; i < PML_MAX_MAPS; i++)
	{
		if(!strcmp(MI[i][mapname], map) && strlen(MI[i][mapname]) > 0 && strlen(map) > 0)return i;
	}
	return 0;
}

stock LoadMTAMap(src[], Float:dist = 90.0)
{
	if(!fexist(src))return printf("[PML %s] LoadMTAMap(%s): This file does not exist.", PML_VERSION, src);
	if(strfind(src, ".map") == -1)return printf("[PML %s] LoadMTAMap(%s): You need to load a MTA .map file!", PML_VERSION, src);
	if(GetMapByName(src) > 0)return printf("[PML %s] LoadMTAMap(%s): A map with this name already exists.", PML_VERSION, src); // Check for a map
	if(!GetFreeMap())return printf("[PML %s] LoadMTAMap(%s): Couldn't load the map (to many maps are loaded).", PML_VERSION, src); // Setting the map ID
	new File:srcfl = fopen(src, io_read), str[300], obj, Float:objf[6], count[2];
	while(fread(srcfl, str))
        if(strfind(str, "<map") == -1 && strfind(str, "</map>") == -1)
        {
	        new substr[16], mode, clr[2][4];
         	if(strfind(str, "<object") != -1)mode = 1;
			if(strfind(str, "<vehicle") != -1)mode = 2;
			if(mode > 0)
			{
				strmid(substr, str, strfind(str, "model=\"") + 7, strfind(str, "\"", false, strfind(str, "model=\"") + 7));
				obj = strval(substr); // model
				strmid(substr, str, strfind(str, "posX=\"") + 6, strfind(str, "\"", false, strfind(str, "posX=\"") + 6));
				objf[0] = floatstr(substr); // X position
				strmid(substr, str, strfind(str, "posY=\"") + 6, strfind(str, "\"", false, strfind(str, "posY=\"") + 6));
				objf[1] = floatstr(substr); // Y position
				strmid(substr, str, strfind(str, "posZ=\"") + 6, strfind(str, "\"", false, strfind(str, "posZ=\"") + 6));
				objf[2] = floatstr(substr); // Z position
				strmid(substr, str, strfind(str, "rotX=\"") + 6, strfind(str, "\"", false, strfind(str, "rotX=\"") + 6));
				objf[3] = floatstr(substr); // X rotation (objects only)
				strmid(substr, str, strfind(str, "rotY=\"") + 6, strfind(str, "\"", false, strfind(str, "rotY=\"") + 6));
				objf[4] = floatstr(substr); // Y rotation (objects only) 
				strmid(substr, str, strfind(str, "rotZ=\"") + 6, strfind(str, "\"", false, strfind(str, "rotZ=\"") + 6));
				objf[5] = floatstr(substr); // Z rotation
				if(mode == 1)
				{
					MI[curmap][objects][(count[0] + 1)] = CreateObject(obj, objf[0], objf[1], objf[2], objf[3], objf[4], objf[5], dist);
					if(MI[curmap][objects][(count[0] + 1)] == INVALID_OBJECT_ID)
						printf("[PML %s] LoadMTAMap(%s): Object No. %i couldn't be loaded.", PML_VERSION, src, count[0]);
					else
						count[0]++;
				}else{
					strmid(substr, str, strfind(str, "color=\"") + 7, strfind(str, "\"", false, strfind(str, "color=\"") + 7));
					pmlsplit(substr, clr, ','); // colors
					MI[curmap][vehicles][(count[1] + 1)] = CreateVehicle(obj, objf[0], objf[1], objf[2], objf[5], strval(clr[0]), strval(clr[1]), PML_MTA_RESPAWN_RATE);
					if(MI[curmap][vehicles][(count[1] + 1)] == INVALID_VEHICLE_ID)
						printf("[PML %s] LoadMTAMap(%s): Vehicle No. %i couldn't be loaded.", PML_VERSION, src, count[1]);
					else
						count[1]++;
				}
			}
        }
    fclose(srcfl);
	MI[curmap][objects][0] = count[0]; // How many objects?
	MI[curmap][vehicles][0] = count[1]; // How many vehicles?
	format(MI[curmap][mapname], 64, src); // Set the map title
    return printf("[PML %s] LoadMTAMap(%s): Loaded %i objects and %i vehicles (Map ID: %i).", PML_VERSION, src, count[0], count[1], curmap);
}

stock LoadSAMPMap(src[], Float:dist = 90.0)
{
	if(!fexist(src))return printf("[PML %s] LoadSAMPMap(%s): This file does not exist.", PML_VERSION, src);
	if(strfind(src, ".pml") == -1)return printf("[PML %s] LoadSAMPMap(%s): You need to load a .pml map file!", PML_VERSION, src);
	if(GetMapByName(src) > 0)return printf("[PML %s] LoadSAMPMap(%s): A map with this name already exists.", PML_VERSION, src); // Check for a map
	if(!GetFreeMap())return printf("[PML %s] LoadSAMPMap(%s): Couldn't load the map (to many maps are loaded).", PML_VERSION, src); // Setting the map ID
	new File:srcfl = fopen(src, io_read), str[256], count[2];
	while(fread(srcfl, str))
	{
		new vars[8][32];
		pmlsplit(str, vars, ',');
		if(strfind(str, "CreateObject(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "CreateObject("), strfind(vars[0], "CreateObject(") + 13);
			MI[curmap][objects][(count[0] + 1)] = 
				CreateObject(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), floatstr(vars[5]), floatstr(vars[6]), dist);
			count[0]++;
		}else if(strfind(str, "CreateVehicle(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "CreateVehicle("), strfind(vars[0], "CreateVehicle(") + 14);
			if(strfind(vars[7], ");"))strdel(vars[7], strfind(vars[7], ");"), strlen(vars[7]) - 2);
			MI[curmap][vehicles][(count[1] + 1)] = 
				CreateVehicle(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), strval(vars[5]), strval(vars[6]), strval(vars[7]));
			count[1]++;
		}else if(strfind(str, "AddStaticVehicle(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "AddStaticVehicle("), strfind(vars[0], "AddStaticVehicle(") + 17);
			if(strfind(vars[6], ");"))strdel(vars[6], strfind(vars[6], ");"), strlen(vars[6]) - 2);
			MI[curmap][vehicles][(count[1] + 1)] = 
				AddStaticVehicle(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), strval(vars[5]), strval(vars[6]));
			count[1]++;
		}else if(strfind(str, "AddStaticVehicleEx(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "AddStaticVehicleEx("), strfind(vars[0], "AddStaticVehicleEx(") + 19);
			if(strfind(vars[7], ");"))strdel(vars[7], strfind(vars[7], ");"), strlen(vars[7]) - 2);
			MI[curmap][vehicles][(count[1] + 1)] = 
				AddStaticVehicleEx(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), strval(vars[5]), strval(vars[6]), strval(vars[7]));
			count[1]++;
		}
	}
	fclose(srcfl);
	MI[curmap][objects][0] = count[0]; // How many objects?
	MI[curmap][vehicles][0] = count[1]; // How many vehicles?
	format(MI[curmap][mapname], 64, src); // Set the map title
    return printf("[PML %s] LoadSAMPMap(%s): Loaded %i objects and %i vehicles (Map ID: %i).", PML_VERSION, src, count[0], count[1], curmap);
}

stock LoadMTAMapToArrays(src[], objar[], vehar[], Float:dist = 90.0)
{
	if(!fexist(src))return printf("[PML %s] LoadMTAMapToArrays(%s): This file does not exist.", PML_VERSION, src);
	if(strfind(src, ".map") == -1)return printf("[PML %s] LoadMTAMapToArrays(%s): You need to load a MTA .map file!", PML_VERSION, src);
	if(GetMapByName(src) > 0)return printf("[PML %s] LoadMTAMapToArrays(%s): A map with this name already exists.", PML_VERSION, src); // Check for a map
	if(!GetFreeMap())return printf("[PML %s] LoadMTAMapToArrays(%s): Couldn't load the map (to many maps are loaded).", PML_VERSION, src); // Setting the map ID
	new File:srcfl = fopen(src, io_read), str[300], obj, Float:objf[6], count[2];
	while(fread(srcfl, str))
        if(strfind(str, "<map") == -1 && strfind(str, "</map>") == -1)
        {
	        new substr[16], mode, clr[2][4];
         	if(strfind(str, "<object") != -1)mode = 1;
			if(strfind(str, "<vehicle") != -1)mode = 2;
			if(mode > 0)
			{
				strmid(substr, str, strfind(str, "model=\"") + 7, strfind(str, "\"", false, strfind(str, "model=\"") + 7));
				obj = strval(substr); // model
				strmid(substr, str, strfind(str, "posX=\"") + 6, strfind(str, "\"", false, strfind(str, "posX=\"") + 6));
				objf[0] = floatstr(substr); // X position
				strmid(substr, str, strfind(str, "posY=\"") + 6, strfind(str, "\"", false, strfind(str, "posY=\"") + 6));
				objf[1] = floatstr(substr); // Y position
				strmid(substr, str, strfind(str, "posZ=\"") + 6, strfind(str, "\"", false, strfind(str, "posZ=\"") + 6));
				objf[2] = floatstr(substr); // Z position
				strmid(substr, str, strfind(str, "rotX=\"") + 6, strfind(str, "\"", false, strfind(str, "rotX=\"") + 6));
				objf[3] = floatstr(substr); // X rotation (objects only)
				strmid(substr, str, strfind(str, "rotY=\"") + 6, strfind(str, "\"", false, strfind(str, "rotY=\"") + 6));
				objf[4] = floatstr(substr); // Y rotation (objects only) 
				strmid(substr, str, strfind(str, "rotZ=\"") + 6, strfind(str, "\"", false, strfind(str, "rotZ=\"") + 6));
				objf[5] = floatstr(substr); // Z rotation
				if(mode == 1)
				{
					objar[(count[0])] = MI[curmap][objects][(count[0] + 1)] = CreateObject(obj, objf[0], objf[1], objf[2], objf[3], objf[4], objf[5], dist);
					if(MI[curmap][objects][(count[0] + 1)] == INVALID_OBJECT_ID)
						printf("[PML %s] LoadMTAMapToArrays(%s): Object No. %i couldn't be loaded.", PML_VERSION, src, count[0]);
					else
						count[0]++;
				}else{
					strmid(substr, str, strfind(str, "color=\"") + 7, strfind(str, "\"", false, strfind(str, "color=\"") + 7));
					pmlsplit(substr, clr, ','); // colors
					vehar[(count[1])] = MI[curmap][vehicles][(count[1] + 1)] = CreateVehicle(obj, objf[0], objf[1], objf[2], objf[5], strval(clr[0]), strval(clr[1]), PML_MTA_RESPAWN_RATE);
					if(MI[curmap][vehicles][(count[1] + 1)] == INVALID_VEHICLE_ID)
						printf("[PML %s] LoadMTAMapToArrays(%s): Vehicle No. %i couldn't be loaded.", PML_VERSION, src, count[1]);
					else
						count[1]++;
				}
			}
        }
    fclose(srcfl);
	MI[curmap][objects][0] = count[0]; // How many objects?
	MI[curmap][vehicles][0] = count[1]; // How many vehicles?
	format(MI[curmap][mapname], 64, src); // Set the map title
    return printf("[PML %s] LoadMTAMapToArrays(%s): Loaded %i objects and %i vehicles (Map ID: %i).", PML_VERSION, src, count[0], count[1], curmap);
}

stock LoadSAMPMapToArrays(src[], objar[], vehar[], Float:dist = 90.0)
{
	if(!fexist(src))return printf("[PML %s] LoadSAMPMapToArrays(%s): This file does not exist.", PML_VERSION, src);
	if(strfind(src, ".pml") == -1)return printf("[PML %s] LoadSAMPMapToArrays(%s): You need to load a .pml map file!", PML_VERSION, src);
	if(GetMapByName(src) > 0)return printf("[PML %s] LoadSAMPMapToArrays(%s): A map with this name already exists.", PML_VERSION, src); // Check for a map
	if(!GetFreeMap())return printf("[PML %s] LoadSAMPMapToArrays(%s): Couldn't load the map (to many maps are loaded).", PML_VERSION, src); // Setting the map ID
	new File:srcfl = fopen(src, io_read), str[256], count[2];
	while(fread(srcfl, str))
	{
		new vars[8][32];
		pmlsplit(str, vars, ',');
		if(strfind(str, "CreateObject(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "CreateObject("), strfind(vars[0], "CreateObject(") + 13);
			objar[(count[0])] = MI[curmap][objects][(count[0] + 1)] = 
				CreateObject(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), floatstr(vars[5]), floatstr(vars[6]), dist);
			count[0]++;
		}else if(strfind(str, "CreateVehicle(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "CreateVehicle("), strfind(vars[0], "CreateVehicle(") + 14);
			if(strfind(vars[7], ");"))strdel(vars[7], strfind(vars[7], ");"), strlen(vars[7]) - 2);
			vehar[(count[1])] = MI[curmap][vehicles][(count[1] + 1)] = 
				CreateVehicle(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), strval(vars[5]), strval(vars[6]), strval(vars[7]));
			count[1]++;
		}else if(strfind(str, "AddStaticVehicle(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "AddStaticVehicle("), strfind(vars[0], "AddStaticVehicle(") + 17);
			if(strfind(vars[6], ");"))strdel(vars[6], strfind(vars[6], ");"), strlen(vars[6]) - 2);
			vehar[(count[1])] = MI[curmap][vehicles][(count[1] + 1)] = 
				AddStaticVehicle(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), strval(vars[5]), strval(vars[6]));
			count[1]++;
		}else if(strfind(str, "AddStaticVehicleEx(") != -1)
		{
			strdel(vars[0], strfind(vars[0], "AddStaticVehicleEx("), strfind(vars[0], "AddStaticVehicleEx(") + 19);
			if(strfind(vars[7], ");"))strdel(vars[7], strfind(vars[7], ");"), strlen(vars[7]) - 2);
			vehar[(count[1])] = MI[curmap][vehicles][(count[1] + 1)] = 
				AddStaticVehicleEx(strval(vars[0]), floatstr(vars[1]), floatstr(vars[2]), floatstr(vars[3]), floatstr(vars[4]), strval(vars[5]), strval(vars[6]), strval(vars[7]));
			count[1]++;
		}
	}
	fclose(srcfl);
	MI[curmap][objects][0] = count[0]; // How many objects?
	MI[curmap][vehicles][0] = count[1]; // How many vehicles?
	format(MI[curmap][mapname], 64, src); // Set the map title
    return printf("[PML %s] LoadSAMPMapToArrays(%s): Loaded %i objects and %i vehicles (Map ID: %i).", PML_VERSION, src, count[0], count[1], curmap);
}

stock RemoveMapObjects(map[])
{
	new mid = GetMapByName(map);
	if(!mid)return printf("[PML %s] RemoveMapObjects(%s): The Map wasn't found.", PML_VERSION, map);
	for(new i = 1; i < MI[mid][objects][0] + 1; i++)
		if(MI[mid][objects][i] != INVALID_OBJECT_ID)
		{
			DestroyObject(MI[mid][objects][i]);
			MI[mid][objects][i] = 0;
		}
	MI[mid][objects][0] = 0;
	return printf("[PML %s] RemoveMapObjects(%s): All objects have been removed.", PML_VERSION, map);
}

stock RemoveMapVehicles(map[])
{
	new mid = GetMapByName(map);
	if(!mid)return printf("[PML %s] RemoveMapVehicles(%s): The Map wasn't found.", PML_VERSION, map);
	for(new i = 1; i < MI[mid][vehicles][0] + 1; i++)
		if(MI[mid][vehicles][i] != INVALID_VEHICLE_ID)
		{
			DestroyVehicle(MI[mid][vehicles][i]);
			MI[mid][vehicles][i] = 0;
		}
	MI[mid][vehicles][0] = 0;
	return printf("[PML %s] RemoveMapVehicles(%s): All vehicles have been removed.", PML_VERSION, map);
}

stock RespawnMapVehicles(map[])
{
	new mid = GetMapByName(map);
	if(!mid)return printf("[PML %s] RespawnMapVehicles(%s): The Map wasn't found.", PML_VERSION, map);
	for(new i = 1; i < MI[mid][vehicles][0] + 1; i++)
		if(MI[mid][vehicles][i] != INVALID_VEHICLE_ID)SetVehicleToRespawn(MI[mid][vehicles][i]);
	return printf("[PML %s] RespawnMapVehicles(%s): All vehicles have been respawned.", PML_VERSION, map);
}

stock RemoveMap(map[])
{
	new mid = GetMapByName(map);
	if(!mid)return printf("[PML %s] RemoveMap(%s): The Map wasn't found.", PML_VERSION, map);
	RemoveMapObjects(map);
	RemoveMapVehicles(map);
	return 1;
}