// Weapon Var

#define GivePlayerWeapon(%1,%2,%3) GivePlayerGun(%1,%2,%3)

new
	Brass[MAX_PLAYERS],
	Golf[MAX_PLAYERS],
	NiteStick[MAX_PLAYERS],
	Knife[MAX_PLAYERS],
	BaseBall[MAX_PLAYERS],
	Shovel[MAX_PLAYERS],
	Pool[MAX_PLAYERS],
	Katana[MAX_PLAYERS],
	ChainSaw[MAX_PLAYERS],
	PurpleDil[MAX_PLAYERS],
	SmallWhiteDil[MAX_PLAYERS],
	LargeWhiteDil[MAX_PLAYERS],
	SilverDil[MAX_PLAYERS],
	Flowers[MAX_PLAYERS],
	Cane[MAX_PLAYERS],
	Grenade[MAX_PLAYERS],
	TearGas[MAX_PLAYERS],
	Molitov[MAX_PLAYERS],
	Ninemm[MAX_PLAYERS],
	Silenced9mm[MAX_PLAYERS],
	DesertEagle[MAX_PLAYERS],
	Shotgun[MAX_PLAYERS],
	SawnOffShotgun[MAX_PLAYERS],
	CombatShotgun[MAX_PLAYERS],
	MicroSMG[MAX_PLAYERS],
	SMG[MAX_PLAYERS],
	AK47[MAX_PLAYERS],
	M4[MAX_PLAYERS],
	Tec9[MAX_PLAYERS],
	CountryRifle[MAX_PLAYERS],
	SniperRifle[MAX_PLAYERS],
	Rocket[MAX_PLAYERS],
	HeatSeakingRocket[MAX_PLAYERS],
	FlameThrower[MAX_PLAYERS],
	MiniGun[MAX_PLAYERS],
	Satchel[MAX_PLAYERS],
	Detonator[MAX_PLAYERS],
	SprayCan[MAX_PLAYERS],
	Fire[MAX_PLAYERS],
	Camera[MAX_PLAYERS],
	NightVision[MAX_PLAYERS],
	ThermalVision[MAX_PLAYERS],
	Parachute[MAX_PLAYERS];
	
stock ResetPlayerWeaponStats(playerid)
{
    Parachute[playerid] = 0;
	ThermalVision[playerid] = 0;
	NightVision[playerid] = 0;
	Camera[playerid] = 0;
	Fire[playerid] = 0;
	SprayCan[playerid] = 0;
	Detonator[playerid] = 0;
	Satchel[playerid] = 0;
	MiniGun[playerid] = 0;
	FlameThrower[playerid] = 0;
	HeatSeakingRocket[playerid] = 0;
	Rocket[playerid] = 0;
	SniperRifle[playerid] = 0;
	CountryRifle[playerid] = 0;
	Tec9[playerid] = 0;
	M4[playerid] = 0;
	AK47[playerid] = 0;
	SMG[playerid] = 0;
	MicroSMG[playerid] = 0;
	CombatShotgun[playerid] = 0;
	SawnOffShotgun[playerid] = 0;
	Shotgun[playerid] = 0;
	DesertEagle[playerid] = 0;
	Silenced9mm[playerid] = 0;
	Ninemm[playerid] = 0;
	Molitov[playerid] = 0;
	TearGas[playerid] = 0;
	Grenade[playerid] = 0;
	Cane[playerid] = 0;
	Flowers[playerid] = 0;
	SilverDil[playerid] = 0;
	LargeWhiteDil[playerid] = 0;
	SmallWhiteDil[playerid] = 0;
	PurpleDil[playerid] = 0;
	ChainSaw[playerid] = 0;
	Katana[playerid] = 0;
	Pool[playerid] = 0;
	Shovel[playerid] = 0;
	BaseBall[playerid] = 0;
	Knife[playerid] = 0;
	NiteStick[playerid] = 0;
	Golf[playerid] = 0;
	Brass[playerid] = 0;
}

stock StartServerSideWeapon()
{
    SetTimer("UpdateWeapons", 1000, 1);
}

stock GivePlayerGun(playerid, Gun, Ammo)
{
	if(Gun == 1) { Brass[playerid] = 1; }
	if(Gun == 2) { Golf[playerid] = 1; }
	if(Gun == 3) { NiteStick[playerid] = 1; }
	if(Gun == 4) { Knife[playerid] = 1; }
	if(Gun == 5) { BaseBall[playerid] = 1; }
	if(Gun == 6) { Shovel[playerid] = 1; }
	if(Gun == 7) { Pool[playerid] = 1; }
	if(Gun == 8) { Katana[playerid] = 1; }
	if(Gun == 9) { ChainSaw[playerid] = 1; }
	if(Gun == 10) { PurpleDil[playerid] = 1; }
	if(Gun == 11) { SmallWhiteDil[playerid] = 1; }
	if(Gun == 12) { LargeWhiteDil[playerid] = 1; }
	if(Gun == 13) { SilverDil[playerid] = 1; }
	if(Gun == 14) { Flowers[playerid] = 1; }
	if(Gun == 15) { Cane[playerid] = 1; }
	if(Gun == 16) { Grenade[playerid] = 1; }
	if(Gun == 17) { TearGas[playerid] = 1; }
	if(Gun == 18) { Molitov[playerid] = 1; }
	if(Gun == 22) { Ninemm[playerid] = 1; }
	if(Gun == 23) { Silenced9mm[playerid] = 1; }
	if(Gun == 25) { Shotgun[playerid] = 1; }
	if(Gun == 26) { SawnOffShotgun[playerid] = 1; }
	if(Gun == 27) { CombatShotgun[playerid] = 1; }
	if(Gun == 28) { MicroSMG[playerid] = 1; }
	if(Gun == 29) { SMG[playerid] = 1; }
	if(Gun == 30) { AK47[playerid] = 1; }
	if(Gun == 31) { M4[playerid] = 1; }
	if(Gun == 32) { Tec9[playerid] = 1; }
	if(Gun == 33) { CountryRifle[playerid] = 1; }
	if(Gun == 34) { SniperRifle[playerid] = 1; }
	if(Gun == 35) { Rocket[playerid] = 1; }
	if(Gun == 36) { HeatSeakingRocket[playerid] = 1; }
	if(Gun == 37) { FlameThrower[playerid] = 1; }
	if(Gun == 38) { MiniGun[playerid] = 1; }
	if(Gun == 39) { Satchel[playerid] = 1; }
	if(Gun == 40) { Detonator[playerid] = 1; }
	if(Gun == 41) { SprayCan[playerid] = 1; }
	if(Gun == 42) { Fire[playerid] = 1; }
	if(Gun == 43) { Camera[playerid] = 1; }
	if(Gun == 44) { NightVision[playerid] = 1; }
	if(Gun == 45) { ThermalVision[playerid] = 1; }
	if(Gun == 46) { Parachute[playerid] = 1; }
	GivePlayerWeapon(playerid, Gun, Ammo);
}

forward UpdateWeapons();
public UpdateWeapons()
{
    for(new i=0; i<MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
		{
		    if(Brass[i] == 0 && GetPlayerWeapon(i) == 1)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Golf[i] == 0 && GetPlayerWeapon(i) == 2)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(NiteStick[i] == 0 && GetPlayerWeapon(i) == 3)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Knife[i] == 0 && GetPlayerWeapon(i) == 4)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(BaseBall[i] == 0 && GetPlayerWeapon(i) == 5)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Shovel[i] == 0 && GetPlayerWeapon(i) == 6)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Pool[i] == 0 && GetPlayerWeapon(i) == 7)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Katana[i] == 0 && GetPlayerWeapon(i) == 8)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(ChainSaw[i] == 0 && GetPlayerWeapon(i) == 9)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(PurpleDil[i] == 0 && GetPlayerWeapon(i) == 10)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(SmallWhiteDil[i] == 0 && GetPlayerWeapon(i) == 11)
		    {
                BanEx(i, "Weapon Hacks");
		    }
		    else if(LargeWhiteDil[i] == 0 && GetPlayerWeapon(i) == 12)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(SilverDil[i] == 0 && GetPlayerWeapon(i) == 13)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Flowers[i] == 0 && GetPlayerWeapon(i) == 14)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Cane[i] == 0 && GetPlayerWeapon(i) == 15)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Grenade[i] == 0 && GetPlayerWeapon(i) == 16)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(TearGas[i] == 0 && GetPlayerWeapon(i) == 17)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Molitov[i] == 0 && GetPlayerWeapon(i) == 18)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Ninemm[i] == 0 && GetPlayerWeapon(i) == 22)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Silenced9mm[i] == 0 && GetPlayerWeapon(i) == 23)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(DesertEagle[i] == 0 && GetPlayerWeapon(i) == 24)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Shotgun[i] == 0 && GetPlayerWeapon(i) == 25)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(SawnOffShotgun[i] == 0 && GetPlayerWeapon(i) == 26)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(CombatShotgun[i] == 0 && GetPlayerWeapon(i) == 27)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(MicroSMG[i] == 0 && GetPlayerWeapon(i) == 28)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(SMG[i] == 0 && GetPlayerWeapon(i) == 29)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(AK47[i] == 0 && GetPlayerWeapon(i) == 30)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(M4[i] == 0 && GetPlayerWeapon(i) == 31)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Tec9[i] == 0 && GetPlayerWeapon(i) == 32)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(CountryRifle[i] == 0 && GetPlayerWeapon(i) == 33)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(SniperRifle[i] == 0 && GetPlayerWeapon(i) == 34)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Rocket[i] == 0 && GetPlayerWeapon(i) == 35)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(HeatSeakingRocket[i] == 0 && GetPlayerWeapon(i) == 36)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(FlameThrower[i] == 0 && GetPlayerWeapon(i) == 37)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(MiniGun[i] == 0 && GetPlayerWeapon(i) == 38)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Satchel[i] == 0 && GetPlayerWeapon(i) == 39)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Detonator[i] == 0 && GetPlayerWeapon(i) == 40)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(SprayCan[i] == 0 && GetPlayerWeapon(i) == 41)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Fire[i] == 0 && GetPlayerWeapon(i) == 42)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Camera[i] == 0 && GetPlayerWeapon(i) == 43)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(NightVision[i] == 0 && GetPlayerWeapon(i) == 44)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(ThermalVision[i] == 0 && GetPlayerWeapon(i) == 45)
		    {
		        BanEx(i, "Weapon Hacks");
		    }
		    else if(Parachute[i] == 0 && GetPlayerWeapon(i) == 46)
		    {
				BanEx(i, "Weapon Hacks");
		    }
		}
	}
}