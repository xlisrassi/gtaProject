/************************************************************ pOneArmedBandit***
*
* Scriptname:
* -» pOneArmedBandit
*
* Author:
* -» Pablo_Borsellino
*
* Creation Date:
* -» 15th October 2011
*
* Release Date:
* -» 16th October 2011
*
* Version:
* -» 1.0
*
* Need to use:
* -» Sa:Mp 0.3d RC5-3 or higher
*
* Language:
* -» English
*
* Description:
* -» Player can play One-armed Bandit without being in the Casino.
*
* Functions:
* -» StartOnArmedBanditForPlayer(playerid);
* -» StopOnArmedBanditForPlayer(playerid);
*
* Public's:
* -» OnPlayerFinishOnArmedBandit(playerid,correct)
*
* Credit's:
* -» n/a
*											Copyright © 2011 by Pablo_Borsellino
*******************************************************************************/

//_____________________________________________________________________Include's
#include <a_samp>

//_______________________________________________________________Variable's & co.
new bool:_OAB_PlayerPlays[MAX_PLAYERS],
	_OAB_TICKS[MAX_PLAYERS],
	_OAB_CURRENT_TD_SLOT_1[MAX_PLAYERS],
	_OAB_CURRENT_TD_SLOT_2[MAX_PLAYERS],
	_OAB_CURRENT_TD_SLOT_3[MAX_PLAYERS],
	Text:_OAB_TD_SLOT_1[MAX_PLAYERS],
	Text:_OAB_TD_SLOT_2[MAX_PLAYERS],
	Text:_OAB_TD_SLOT_3[MAX_PLAYERS];

//_______________________________________________________________User Function's
forward StartOnArmedBanditForPlayer(playerid);
public StartOnArmedBanditForPlayer(playerid)
{
	if(_OAB_PlayerPlays[playerid]==true)return true;
	_OAB_PlayerPlays[playerid]=true;
	_OAB_TICKS[playerid]=30;
	_OAB_CURRENT_TD_SLOT_1[playerid]=random(5-2)+2;
	_OAB_CURRENT_TD_SLOT_2[playerid]=random(5-2)+2;
	_OAB_CURRENT_TD_SLOT_3[playerid]=random(5-2)+2;
    _OAB_TD_SLOT_1[playerid] = TextDrawCreate(274,300,"LD_SLOT:r_69");
    _OAB_TD_SLOT_2[playerid] = TextDrawCreate(200,300,"LD_SLOT:r_69");
    _OAB_TD_SLOT_3[playerid] = TextDrawCreate(348,300,"LD_SLOT:r_69");
    TextDrawFont(_OAB_TD_SLOT_1[playerid],4);
    TextDrawFont(_OAB_TD_SLOT_2[playerid],4);
    TextDrawFont(_OAB_TD_SLOT_3[playerid],4);
    TextDrawColor(_OAB_TD_SLOT_1[playerid],0xFFFFFFFF);
    TextDrawColor(_OAB_TD_SLOT_2[playerid],0xFFFFFFFF);
    TextDrawColor(_OAB_TD_SLOT_3[playerid],0xFFFFFFFF);
    TextDrawTextSize(_OAB_TD_SLOT_1[playerid],75,75);
    TextDrawTextSize(_OAB_TD_SLOT_2[playerid],75,75);
    TextDrawTextSize(_OAB_TD_SLOT_3[playerid],75,75);
    TextDrawShowForPlayer(playerid,_OAB_TD_SLOT_1[playerid]);
    TextDrawShowForPlayer(playerid,_OAB_TD_SLOT_2[playerid]);
    TextDrawShowForPlayer(playerid,_OAB_TD_SLOT_3[playerid]);
    StartOABForPlayer(playerid);
	return true;
}

//______________________________________________________________________________
forward StartOABForPlayer(playerid);
public StartOABForPlayer(playerid)
{
	if(_OAB_PlayerPlays[playerid]==false)return true;
	if(_OAB_TICKS[playerid]>=30)return SetTimerEx("_OAB_PLAYER_TIMER",50,false,"d",playerid);
	else if(_OAB_TICKS[playerid]>=20)return SetTimerEx("_OAB_PLAYER_TIMER",60,false,"d",playerid);
	else if(_OAB_TICKS[playerid]>=10)return SetTimerEx("_OAB_PLAYER_TIMER",70,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==9)return SetTimerEx("_OAB_PLAYER_TIMER",80,false,"d",playerid);
	if(_OAB_TICKS[playerid]==8)return SetTimerEx("_OAB_PLAYER_TIMER",90,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==7)return SetTimerEx("_OAB_PLAYER_TIMER",100,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==6)return SetTimerEx("_OAB_PLAYER_TIMER",110,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==5)return SetTimerEx("_OAB_PLAYER_TIMER",120,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==4)return SetTimerEx("_OAB_PLAYER_TIMER",130,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==3)return SetTimerEx("_OAB_PLAYER_TIMER",140,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==2)return SetTimerEx("_OAB_PLAYER_TIMER",150,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==1)return SetTimerEx("_OAB_PLAYER_TIMER",160,false,"d",playerid);
	else if(_OAB_TICKS[playerid]==0)
	{
		SetTimerEx("StopOnArmedBanditForPlayer",2500,false,"d",playerid);
		_OAB_PLAYER_FINISH_TURN(playerid);
	}
	return true;
}

/*______________________________________________________________________________
Example for OnPlayerFinishOnArmedBandit(..) in the Gamemode:
________________________________________________________________________________

public OnPlayerFinishOnArmedBandit(playerid,correct)
{
	if(correct==3) //All three are the same
	{
		GameTextForPlayer(playerid,"~g~Win",2000,6);
		GivePlayerMoney(playerid,OAB_MONEY_AT_WIN_OR_LOSE); //Give for ex. 100$
	}
	else if(correct<=2) //None are the same or only two are the same
	{
		GameTextForPlayer(playerid,"~r~Lose",2000,6);
		GivePlayerMoney(playerid,OAB_MONEY_AT_WIN_OR_LOSE); //Give for ex. -100$
	}
	return 1;
}
______________________________________________________________________________*/
forward StopOnArmedBanditForPlayer(playerid);
public StopOnArmedBanditForPlayer(playerid)
{
	TextDrawDestroy(_OAB_TD_SLOT_1[playerid]);
	TextDrawDestroy(_OAB_TD_SLOT_2[playerid]);
	TextDrawDestroy(_OAB_TD_SLOT_3[playerid]);
	_OAB_PlayerPlays[playerid]=false;
	return true;
}

//______________________________________________________________Script Function's
forward _OAB_PLAYER_FINISH_TURN(playerid);
public _OAB_PLAYER_FINISH_TURN(playerid)
{
	if(_OAB_CURRENT_TD_SLOT_1[playerid]==_OAB_CURRENT_TD_SLOT_2[playerid]&&_OAB_CURRENT_TD_SLOT_1[playerid]!=_OAB_CURRENT_TD_SLOT_3[playerid])return CallRemoteFunction("OnPlayerFinishOnArmedBandit","dd",playerid,2);
	else if(_OAB_CURRENT_TD_SLOT_2[playerid]==_OAB_CURRENT_TD_SLOT_3[playerid]&&_OAB_CURRENT_TD_SLOT_2[playerid]!=_OAB_CURRENT_TD_SLOT_1[playerid])return CallRemoteFunction("OnPlayerFinishOnArmedBandit","dd",playerid,2);
	else if(_OAB_CURRENT_TD_SLOT_1[playerid]==_OAB_CURRENT_TD_SLOT_3[playerid]&&_OAB_CURRENT_TD_SLOT_1[playerid]!=_OAB_CURRENT_TD_SLOT_2[playerid])return CallRemoteFunction("OnPlayerFinishOnArmedBandit","dd",playerid,2);
	else if(_OAB_CURRENT_TD_SLOT_1[playerid]==_OAB_CURRENT_TD_SLOT_2[playerid]&&_OAB_CURRENT_TD_SLOT_1[playerid]==_OAB_CURRENT_TD_SLOT_3[playerid])return CallRemoteFunction("OnPlayerFinishOnArmedBandit","dd",playerid,3);
	else if(_OAB_CURRENT_TD_SLOT_1[playerid]!=_OAB_CURRENT_TD_SLOT_2[playerid]&&_OAB_CURRENT_TD_SLOT_1[playerid]!=_OAB_CURRENT_TD_SLOT_3[playerid])return CallRemoteFunction("OnPlayerFinishOnArmedBandit","dd",playerid,0);
	else return CallRemoteFunction("OnPlayerFinishOnArmedBandit","dd",playerid,1);
}

forward _OAB_PLAYER_TIMER(playerid);
public _OAB_PLAYER_TIMER(playerid)
{
    _OAB_CURRENT_TD_SLOT_1[playerid]++;
    _OAB_CURRENT_TD_SLOT_2[playerid]--;
    _OAB_CURRENT_TD_SLOT_3[playerid]++;
    if(_OAB_CURRENT_TD_SLOT_1[playerid]>6)_OAB_CURRENT_TD_SLOT_1[playerid]=1;
    if(_OAB_CURRENT_TD_SLOT_2[playerid]<1)_OAB_CURRENT_TD_SLOT_2[playerid]=6;
    if(_OAB_CURRENT_TD_SLOT_3[playerid]>6)_OAB_CURRENT_TD_SLOT_3[playerid]=1;
	TextDrawSetString(_OAB_TD_SLOT_1[playerid],_OAB_GET_TEXTURE(_OAB_CURRENT_TD_SLOT_1[playerid]));
	TextDrawSetString(_OAB_TD_SLOT_2[playerid],_OAB_GET_TEXTURE(_OAB_CURRENT_TD_SLOT_2[playerid]));
	TextDrawSetString(_OAB_TD_SLOT_3[playerid],_OAB_GET_TEXTURE(_OAB_CURRENT_TD_SLOT_3[playerid]));
    TextDrawShowForPlayer(playerid,_OAB_TD_SLOT_1[playerid]);
    TextDrawShowForPlayer(playerid,_OAB_TD_SLOT_2[playerid]);
    TextDrawShowForPlayer(playerid,_OAB_TD_SLOT_3[playerid]);
    _OAB_TICKS[playerid]--;
    StartOABForPlayer(playerid);
	return true;
}

stock _OAB_GET_TEXTURE(ID)
{
	new TextureName[20];
	if(ID==1)TextureName="LD_SLOT:r_69";
	if(ID==2)TextureName="LD_SLOT:grapes";
	if(ID==3)TextureName="LD_SLOT:cherry";
	if(ID==4)TextureName="LD_SLOT:bell";
	if(ID==5)TextureName="LD_SLOT:bar2_o";
	if(ID==6)TextureName="LD_SLOT:bar1_o";
	return TextureName;
}
