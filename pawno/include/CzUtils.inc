#define MAX_STRING 255

#define HOLDING(%0) ((newkeys & (%0)) == (%0))
#define PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#define RELEASED(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

#define IsEven(%0)	(!((%0) % 2))
//*********************************MACROS***************************************
#define PUBLIC:%0(%1)   forward %0(%1); public %0(%1)
#if !defined FALSE
    stock bool:FALSE = false;
#endif

#define SendClientMessageEx(%0,%1,%2,%3)				\
    do{													\
        new strr[256];									\
        format(strr, 256, (%2), %3);					\
        SendClientMessage((%0), (%1), strr);			\
    }													\
    while (FALSE)

//General

stock IsNumeric(const string[])
{
	for(new i, j=strlen(string); i<j; i++)if (string[i] > '9' || string[i] < '0')return 0;
	return 1;
}

stock bool:IntToBool(integer) return (integer > 0) ? (true) : (false);

stock BoolToInt(bool:value) return (value == true) ? (1) : (0);

//Users

stock _Kick(playerid, text[]="|-1|")
{
	if(strcmp(text, "|-1|", true))SendClientMessage(playerid, 0xFF0000FF, text);
	return Kick(playerid);
}
#define Kick    _Kick

stock IsPlayerInArea(playerid, Float:max_x, Float:min_x, Float:max_y, Float:min_y)
{
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X, Y, Z);
	if(X <= max_x && X >= min_x && Y <= max_y && Y >= min_y) return 1;
	return 0;
}

stock GetXYInFrontOfPlayer(playerid, &Float:x, &Float:y, Float:distance)
{
    new Float:a;

    GetPlayerPos(playerid, x, y, a);
    GetPlayerFacingAngle(playerid, a);

    if(GetPlayerVehicleID(playerid))GetVehicleZAngle(GetPlayerVehicleID(playerid), a);

    x += (distance * floatsin(-a, degrees));
    y += (distance * floatcos(-a, degrees));
}

stock GetXYBehindPlayer(playerid, &Float:x, &Float:y, Float:distance)
{
    new Float:a;

    GetPlayerPos(playerid, x, y, a);
    GetPlayerFacingAngle(playerid, a);

    if(GetPlayerVehicleID(playerid))GetVehicleZAngle(GetPlayerVehicleID(playerid), a);

    x -= (distance * floatsin(-a, degrees));
    y -= (distance * floatcos(-a, degrees));
}

stock GetXYZBehindPlayer(playerid, &Float:x, &Float:y, &Float:z, Float:distance)
{
    new Float:a;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    if(GetPlayerVehicleID(playerid))GetVehicleZAngle(GetPlayerVehicleID(playerid), a);

    x -= (distance * floatsin(-a, degrees));
    y -= (distance * floatcos(-a, degrees));
}

GetXYZNearPlayer(playerid, &Float:x, &Float:y, &Float:z, Float:distance)
{
    new Float:a;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    if(GetPlayerVehicleID(playerid))GetVehicleZAngle(GetPlayerVehicleID(playerid), a);

    x += (distance * floatsin((-a+90.0), degrees));
    y += (distance * floatcos((-a+90.0), degrees));
}

stock GetXYZInFrontOfPlayerCamera(playerid, &Float:X, &Float:Y, &Float:Z, Float:radio)
{
	new Float:P[7];
	GetPlayerCameraPos(playerid, P[0], P[1], P[2]);
	GetPlayerCameraFrontVector(playerid, P[3], P[4], P[5]);
	
	X = P[0] + floatmul(P[3], radio);
	Y = P[1] + floatmul(P[4], radio);
	Z = P[2] + floatmul(P[5], radio);
	
	return 1;
}

stock GetDistanceBetweenPlayers2D(player1, player2)
{
    if(!IsPlayerConnected(player1) || !IsPlayerConnected(player2))return -1
    new Float:P[2][2], Float:Z;
    GetPlayerPos(player1, P[0][0], P[0][1], Z);
    GetPlayerPos(player2, P[1][0], P[1][1], Z);
    return floatsqroot(floatpower(floatabs(floatsub(P[0][0],P[1][0])),2)+floatpower(floatabs(floatsub(P[0][1],P[1][1])),2));
}

stock GetDistanceBetweenPlayers3D(player1, player2)
{
    if(!IsPlayerConnected(player1) || !IsPlayerConnected(player2))return -1
    new Float:P[2][3];
    GetPlayerPos(player1, P[0][0], P[0][1], P[0][2]);
    GetPlayerPos(player2, P[1][0], P[1][1], P[1][2]);
    return floatsqroot(floatpower(floatabs(floatsub(P[0][0],P[1][0])),2)+floatpower(floatabs(floatsub(P[0][1],P[1][1])),2)+floatpower(floatabs(floatsub(P[0][2],P[1][2])),2));
}

stock GetIDFromName(name[]){
	new i;
	for(; i<GetMaxPlayers(); i++)if(IsPlayerConnected(i) && !strcmp(name, I[i][pName], false))break;
	if(strcmp(name, I[i][pName], false))return INVALID_PLAYER_ID;
	return i;
}

stock IsVehicleBusy(vehicleid)
{
    for(new i; i<GetMaxPlayers(); i++)
        if(IsPlayerConnected(i) && IsPlayerInVehicle(i, vehicleid))
            return 1;
    return 0;
}

//Message Functions

stock SendClientMessageToAllEx(playerid, color, message[]){
	for(new i=0; i<GetMaxPlayers(); i++){
	    if(IsPlayerConnected(i) && !IsPlayerNPC(playerid) && i != playerid)SendClientMessage(i, color, message);
	}
	return 1;
}

//Otros

stock CAOS_Hash(text[])
{
    new i, j, Ix;
    new str[255], tmp[64][5], tmp2;
    new K[5], K2[5], Ks, K2s;
    format(K, 5, "chaoz");
    format(K2, 5, "CH4ØZ");
    for(j=0; j<strlen(K)+1; j++)Ks = Ks + K[j];
    for(j=0; j<strlen(K2)+1; j++)K2s = K2s + K2[j];
    while(i<floatround((strlen(text)+1)/4, floatround_ceil)){
        strmid(tmp[i], text, Ix, Ix+4);
        for(j=0; j<5; j++){
			if(strlen(tmp[i])<4){
				if(tmp[i][j] == '\0'){
					if(j<=2)tmp[i][j] = K[j];
					else tmp[i][j] = K2[j];
				}
            }
            if(i==0)tmp2 = tmp[i][j] + Ks;
            else tmp2 = tmp[i][j] + tmp2;
            format(str, sizeof(str), "%s%i", str, tmp2);
        }
        format(str, sizeof(str), "%i", strval(str) << ((tmp2/K2s)*tmp[i][0]));
        if(strlen(str)>65){
            format(str, sizeof(str), "%i", strval(str) >> (((tmp2/Ks)*K2s)/tmp[i][0]));
            strmid(str, str, 0, 65, sizeof(str));
        }
        i++;
        Ix += 4;
    }
    if(str[0] == '-')format(str, sizeof(str), "%s", str[1]);
	return str;
}

stock LoadAnimLib(playerid, Lib[]) return ApplyAnimation(playerid, Lib, "null", 0.0, 0, 0, 0, 0, 0);

stock Factorial(num)
{
	if(num==0)return 1;
	new res, j=num;
	res = j;
	while(j>1){
	    j--;
	    res = res*(j);
	}
	return res;
}

stock RemovePlayerFromVehicleEx(playerid)
{
	new veh = GetPlayerVehicleID(playerid);
	if(!veh)return 0;
	new Float:P[3];
	if(IsEven(GetPlayerVehicleSeat(playerid))){
		GetXYZNearPlayer(playerid, P[0], P[1], P[2], -1.5);
		SetPlayerPos(playerid, P[0], P[1], P[2]);
		ApplyAnimation(playerid, "ped", "CAR_getoutL_LHS", 2.0, 0, 1, 1, 0, 0, 1);
	}
	else{
		GetXYZNearPlayer(playerid, P[0], P[1], P[2], 1.5);
		SetPlayerPos(playerid, P[0], P[1], P[2]);
		ApplyAnimation(playerid, "ped", "CAR_getoutL_RHS", 2.0, 0, 1, 1, 0, 0, 1);
	}return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_EXIT_VEHICLE);
}
