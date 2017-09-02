#if defined H_INPUT_INCLUDED
	#endinput
#else
	#define H_INPUT_INCLUDED
#endif
#tryinclude <a_samp>
#pragma deprecated
native InputInit();

#define HID_INPUT_PLAYLIST 1
#define HID_INPUT_KEYPAD 2
#define HID_INPUT_VEHCOL 3
#define HID_INPUT_KEYBOARD 4

forward OnPlayerSelectPlayer(playerid, targetid, event);
forward OnPlayerKeypadInput(playerid, input[], event);

/*      native time
native InputFromPlayerList(playerid, event, bool:allowcancel);
native InputFromKeypad(playerid, event, len, bool:allowcancel);
native CancelInput(playerid);
*/

stock InputFromPlayerList(playerid, event, bool:allowcancel)
{
		//process init
	SetPVarInt(playerid, "hid_input_event", event);
	switch(GetPVarInt(playerid, "hid_input_active"))
	{
	    case HID_INPUT_PLAYLIST:
	    {
			DeletePVar(playerid, "hid_input_playlist_list");
			Hid_Input_SetUpList(playerid);
			return;
		}
		case HID_INPUT_KEYPAD:
		{
		
		}
	}
	SetPVarInt(playerid, "hid_input_active", HID_INPUT_PLAYLIST);
	
		//actual init
	new PlayerText:td;
	new str[30];
	
	td = CreatePlayerTextDraw(playerid, 320.000000, 90.000000, "~n~Player List~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 0xFF);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, td, -1);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawUseBox(playerid, td, true);
	PlayerTextDrawBoxColor(playerid, td, 150);
	PlayerTextDrawTextSize(playerid, td, 10.000000, 250.000000);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_playlist_td_box", _:td);
	
	for(new b; b != 20; b++)
	{
		td = CreatePlayerTextDraw(playerid, 220.0, 120.0 + b * 10.0, "");
		PlayerTextDrawBackgroundColor(playerid, td, 0xFF);
		PlayerTextDrawFont(playerid, td, 2);
		PlayerTextDrawLetterSize(playerid, td, 0.239999, 1.000000);
		PlayerTextDrawSetOutline(playerid, td, 0);
		PlayerTextDrawSetProportional(playerid, td, 1);
		PlayerTextDrawSetShadow(playerid, td, 1);
		PlayerTextDrawTextSize(playerid, td, 420.000000, 11.00000);
		format(str, sizeof str, "hid_input_playlist_td_%i", b);
		SetPVarInt(playerid, str, _:td);
	}

	td = CreatePlayerTextDraw(playerid, 320.000000, 330.000000, "_");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 0xFF);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, td, -1);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_playlist_td_mid", _:td);

	td = CreatePlayerTextDraw(playerid, 240.000000, 330.000000, "Previous 20");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 0xFF);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 0.200000, 1.000000);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawTextSize(playerid, td, 18.0, 100.0);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_playlist_td_prev", _:td);

	td = CreatePlayerTextDraw(playerid, 400.000000, 330.000000, "Next 20");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 0xFF);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 0.200000, 1.000000);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawTextSize(playerid, td, 18.0, 55.0);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_playlist_td_next", _:td);

	if(allowcancel == true)
	{
		td = CreatePlayerTextDraw(playerid, 435.000000, 93.000000, "X");
		PlayerTextDrawAlignment(playerid, td, 2);
		PlayerTextDrawBackgroundColor(playerid, td, 255);
		PlayerTextDrawFont(playerid, td, 2);
		PlayerTextDrawLetterSize(playerid, td, 0.500000, 1.000000);
		PlayerTextDrawColor(playerid, td, 0x707070FF);
		PlayerTextDrawSetOutline(playerid, td, 0);
		PlayerTextDrawSetProportional(playerid, td, 1);
		PlayerTextDrawSetShadow(playerid, td, 0);
		PlayerTextDrawUseBox(playerid, td, 1);
		PlayerTextDrawBoxColor(playerid, td, 842150655);
		PlayerTextDrawTextSize(playerid, td, 11.000000, 11.000000);
		PlayerTextDrawSetSelectable(playerid, td, true);
		PlayerTextDrawShow(playerid, td);
		SetPVarInt(playerid, "hid_input_td_close", _:td);
	}

	SelectTextDraw(playerid, 0xFFFFFFFF);
	Hid_Input_SetUpList(playerid);
}

stock InputFromKeypad(playerid, event, minlen, maxlen, bool:allowcancel)
{
		//process init
	SetPVarInt(playerid, "hid_input_event", event);
	if(minlen > maxlen)
	{
		new temp = minlen;
		minlen = maxlen;
		maxlen = temp;
	}
	if(minlen <= 0 || maxlen > 16) minlen = 1;
	if(maxlen > 0 && maxlen <= 16) SetPVarInt(playerid, "hid_input_keypad_maxlen", maxlen);
	else SetPVarInt(playerid, "hid_input_keypad_maxlen", 16);
	SetPVarInt(playerid, "hid_input_keypad_minlen", minlen);
	new PlayerText:td;
   	new str[30];
	switch(GetPVarInt(playerid, "hid_input_active"))
	{
		case HID_INPUT_PLAYLIST:
		{
			CancelInput(playerid);

		}
	    case HID_INPUT_KEYPAD:
	    {
	        DeletePVar(playerid, "hid_input_keypad_input");
	        PlayerTextDrawSetString(playerid, PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_input"), "-");
			return;
		}
	}
	
	    //actual init
	SetPVarInt(playerid, "hid_input_active", HID_INPUT_KEYPAD);
	
	td = CreatePlayerTextDraw(playerid, 320.000000, 90.000000, "~n~Keypad~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 255);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, td, -1);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawUseBox(playerid, td, 1);
	PlayerTextDrawBoxColor(playerid, td, 150);
	PlayerTextDrawTextSize(playerid, td, 10.000000, 250.000000);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_keypad_td_box", _:td);

	for(new b = 0; b != 10; b++)
	{
	    switch(b)
	    {
			case 0: td = CreatePlayerTextDraw(playerid, 250.000000, 235.000000, "0");
			case 1: td = CreatePlayerTextDraw(playerid, 250.000000, 205.000000, "1");
			case 2: td = CreatePlayerTextDraw(playerid, 295.000000, 205.000000, "2");
			case 3: td = CreatePlayerTextDraw(playerid, 340.000000, 205.000000, "3");
			case 4: td = CreatePlayerTextDraw(playerid, 250.000000, 175.000000, "4");
			case 5: td = CreatePlayerTextDraw(playerid, 295.000000, 175.000000, "5");
			case 6: td = CreatePlayerTextDraw(playerid, 340.000000, 175.000000, "6");
			case 7: td = CreatePlayerTextDraw(playerid, 250.000000, 145.000000, "7");
			case 8: td = CreatePlayerTextDraw(playerid, 295.000000, 145.000000, "8");
			case 9: td = CreatePlayerTextDraw(playerid, 340.000000, 145.000000, "9");
		}
		PlayerTextDrawAlignment(playerid, td, 2);
		PlayerTextDrawBackgroundColor(playerid, td, 255);
		PlayerTextDrawFont(playerid, td, 2);
		PlayerTextDrawLetterSize(playerid, td, 0.659999, 2.599999);
		PlayerTextDrawColor(playerid, td, 0x808080FF);
		PlayerTextDrawSetOutline(playerid, td, 0);
		PlayerTextDrawSetProportional(playerid, td, 1);
		PlayerTextDrawSetShadow(playerid, td, 1);
		PlayerTextDrawUseBox(playerid, td, 1);
		PlayerTextDrawBoxColor(playerid, td, 255);
		PlayerTextDrawTextSize(playerid, td, 34.000000, 34.000000);
		PlayerTextDrawSetSelectable(playerid, td, true);
		PlayerTextDrawShow(playerid, td);
		format(str, sizeof str, "hid_input_keypad_td_%i", b);
		SetPVarInt(playerid, str, _:td);
	}

	td = CreatePlayerTextDraw(playerid, 405.000000, 165.000000, "C");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 255);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 1.059999, 2.599999);
	PlayerTextDrawColor(playerid, td, -16776961);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawUseBox(playerid, td, 1);
	PlayerTextDrawBoxColor(playerid, td, 255);
	PlayerTextDrawTextSize(playerid, td, 34.000000, 34.000000);
	PlayerTextDrawSetSelectable(playerid, td, true);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_keypad_td_clear", _:td);

	td = CreatePlayerTextDraw(playerid, 405.000000, 214.000000, "Ok");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 255);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 0.570000, 2.899999);
	PlayerTextDrawColor(playerid, td, 16711935);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawUseBox(playerid, td, 1);
	PlayerTextDrawBoxColor(playerid, td, 255);
	PlayerTextDrawTextSize(playerid, td, 34.000000, 34.000000);
	PlayerTextDrawSetSelectable(playerid, td, true);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_keypad_td_confirm", _:td);

	td = CreatePlayerTextDraw(playerid, 320.000000, 120.000000, "-");
	PlayerTextDrawAlignment(playerid, td, 2);
	PlayerTextDrawBackgroundColor(playerid, td, 255);
	PlayerTextDrawFont(playerid, td, 2);
	PlayerTextDrawLetterSize(playerid, td, 0.450000, 1.800000);
	PlayerTextDrawColor(playerid, td, -1);
	PlayerTextDrawSetOutline(playerid, td, 0);
	PlayerTextDrawSetProportional(playerid, td, 1);
	PlayerTextDrawSetShadow(playerid, td, 1);
	PlayerTextDrawUseBox(playerid, td, 1);
	PlayerTextDrawBoxColor(playerid, td, 255);
	PlayerTextDrawTextSize(playerid, td, 420.000000, 230.000000);
	PlayerTextDrawShow(playerid, td);
	SetPVarInt(playerid, "hid_input_keypad_td_input", _:td);

	if(allowcancel == true)
	{
		td = CreatePlayerTextDraw(playerid, 435.000000, 93.000000, "X");
		PlayerTextDrawAlignment(playerid, td, 2);
		PlayerTextDrawBackgroundColor(playerid, td, 255);
		PlayerTextDrawFont(playerid, td, 2);
		PlayerTextDrawLetterSize(playerid, td, 0.500000, 1.000000);
		PlayerTextDrawColor(playerid, td, 0x707070FF);
		PlayerTextDrawSetOutline(playerid, td, 0);
		PlayerTextDrawSetProportional(playerid, td, 1);
		PlayerTextDrawSetShadow(playerid, td, 0);
		PlayerTextDrawUseBox(playerid, td, 1);
		PlayerTextDrawBoxColor(playerid, td, 842150655);
		PlayerTextDrawTextSize(playerid, td, 11.000000, 11.000000);
		PlayerTextDrawSetSelectable(playerid, td, true);
		PlayerTextDrawShow(playerid, td);
		SetPVarInt(playerid, "hid_input_td_close", _:td);
	}
	SelectTextDraw(playerid, 0xFFFFFFFF);
    GetPVarString(playerid, "hid_input_keypad_input", str, 16);
	    
}

stock Hid_Input_SetUpList(playerid)
{
	new list = GetPVarInt(playerid, "hid_input_playlist_list");
	new str[30];
	new PlayerText:td;
	for(new b = list; b != list + 20; b++)
	{
		format(str, sizeof str, "hid_input_playlist_td_%i", b % 20);
		td = PlayerText:GetPVarInt(playerid, str);
		if(b >= MAX_PLAYERS)
		{
			PlayerTextDrawHide(playerid, td);
			continue;
		}
		if(IsPlayerConnected(b))
		{
		    GetPlayerName(b, str, MAX_PLAYER_NAME);
		    format(str, sizeof str, "%i. %s", b, str);
		    PlayerTextDrawSetString(playerid, td, str);
		    PlayerTextDrawColor(playerid, td, 0xA3B4C5FF);
		    PlayerTextDrawSetSelectable(playerid, td, true);
		}
		else
		{
		    format(str, sizeof str, "%i. Not connected", b);
		    PlayerTextDrawSetString(playerid, td, str);
		    PlayerTextDrawColor(playerid, td, 0x505050FF);
		    PlayerTextDrawSetSelectable(playerid, td, false);
		}
		PlayerTextDrawShow(playerid, td);
	}
	    //navback
	td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_prev");
	if(list != 0)
	{
	    PlayerTextDrawSetSelectable(playerid, td, true);
	    PlayerTextDrawColor(playerid, td, 0xA3B4C5FF);
		PlayerTextDrawShow(playerid, td);
	}
	else
	{
	    PlayerTextDrawSetSelectable(playerid, td, false);
	    PlayerTextDrawColor(playerid, td, 0x505050FF);
		PlayerTextDrawShow(playerid, td);
	}
		//navfront
	td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_next");
	if((list + 20) < MAX_PLAYERS)
	{
	    PlayerTextDrawSetSelectable(playerid, td, true);
	    PlayerTextDrawColor(playerid, td, 0xA3B4C5FF);
		PlayerTextDrawShow(playerid, td);
	}
	else
	{
	    PlayerTextDrawSetSelectable(playerid, td, false);
	    PlayerTextDrawColor(playerid, td, 0x505050FF);
		PlayerTextDrawShow(playerid, td);
	}
	    //  mid (0-19)
	td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_mid");
	format(str, sizeof str, "%i-%i", list, list + 19);
	PlayerTextDrawSetString(playerid, td, str);
	PlayerTextDrawShow(playerid, td);
}

stock CancelInput(playerid)
{
	switch(GetPVarInt(playerid, "hid_input_active"))
	{
	    case 0: return;
	    case HID_INPUT_PLAYLIST:
	    {
	        new str[30], PlayerText:td;
			td = PlayerText:GetPVarInt(playerid, "hid_input_td_close");
			if(_:td != 0)
			{
			    PlayerTextDrawDestroy(playerid, td);
				DeletePVar(playerid, "hid_input_td_close");
			}
			for(new c; c != 20; c++)
			{
			    format(str, sizeof str, "hid_input_playlist_td_%i", c);
			    td = PlayerText:GetPVarInt(playerid, str);
				PlayerTextDrawDestroy(playerid, td);
				DeletePVar(playerid, str);
			}
			td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_next");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_playlist_td_next");
			td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_prev");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_playlist_td_prev");
			td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_right");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_playlist_td_right");
			td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_mid");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_playlist_td_mid");
			td = PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_box");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_playlist_td_box");
			DeletePVar(playerid, "hid_input_playlist_list");
			DeletePVar(playerid, "hid_input_event");
			DeletePVar(playerid, "hid_input_active");
			CancelSelectTextDraw(playerid);
	    }
	    case HID_INPUT_KEYPAD:
	    {
	        new str[30], PlayerText:td;
	        td = PlayerText:GetPVarInt(playerid, "hid_input_td_close");
			if(_:td != 0)
			{
			    PlayerTextDrawDestroy(playerid, td);
				DeletePVar(playerid, "hid_input_td_close");
			}
			for(new c; c != 10; c++)
			{
			    format(str, sizeof str, "hid_input_keypad_td_%i", c);
			    td = PlayerText:GetPVarInt(playerid, str);
				PlayerTextDrawDestroy(playerid, td);
				DeletePVar(playerid, str);
			}
			td = PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_box");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_keypad_td_box");
			td = PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_clear");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_keypad_td_clear");
			td = PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_confirm");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_keypad_td_confirm");
			td = PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_input");
			PlayerTextDrawDestroy(playerid, td);
			DeletePVar(playerid, "hid_input_keypad_input");
			DeletePVar(playerid, "hid_input_keypad_maxlen");
			DeletePVar(playerid, "hid_input_keypad_minlen");
			DeletePVar(playerid, "hid_input_event");
			DeletePVar(playerid, "hid_input_active");
			CancelSelectTextDraw(playerid);
	    }
	}
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	switch(GetPVarInt(playerid, "hid_input_active"))
	{
		case HID_INPUT_PLAYLIST:
		{
		    if(PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_prev") == playertextid)
		    {
		        new list = GetPVarInt(playerid, "hid_input_playlist_list");
		        if(list != 0) list -= 20;
				SetPVarInt(playerid, "hid_input_playlist_list", list);
		        Hid_Input_SetUpList(playerid);
		        return 1;
			}
		    else if(PlayerText:GetPVarInt(playerid, "hid_input_playlist_td_next") == playertextid)
		    {
		        new list = GetPVarInt(playerid, "hid_input_playlist_list");
		        if((list + 20) < MAX_PLAYERS) list += 20;
				SetPVarInt(playerid, "hid_input_playlist_list", list);
		        Hid_Input_SetUpList(playerid);
		        return 1;
		    }
		    else if(PlayerText:GetPVarInt(playerid, "hid_input_td_close") == playertextid)
		    {
				CallRemoteFunction("OnPlayerSelectPlayer", "iii", playerid, INVALID_PLAYER_ID, GetPVarInt(playerid, "hid_input_event"));
				CancelInput(playerid);
				return 1;
		    }
		    else
		    {
		        new str[30], PlayerText:td;
		        for(new b; b != 20; b++)
		        {
				    format(str, sizeof str, "hid_input_playlist_td_%i", b);
				    td = PlayerText:GetPVarInt(playerid, str);
				    if(td == playertextid)
				    {
						CallRemoteFunction("OnPlayerSelectPlayer", "iii", playerid, b + GetPVarInt(playerid, "hid_input_playlist_list"), GetPVarInt(playerid, "hid_input_event"));
						CancelInput(playerid);
						return 1;
				    }
		        }
		    }
		    
		}
		case HID_INPUT_KEYPAD:
		{
		    if(PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_confirm") == playertextid)
		    {
		        new str[16];
		        GetPVarString(playerid, "hid_input_keypad_input", str, 16);
				if(strlen(str) < GetPVarInt(playerid, "hid_input_keypad_minlen")) return 1;
				CallRemoteFunction("OnPlayerKeypadInput", "isi", playerid, str, GetPVarInt(playerid, "hid_input_event"));
				CancelInput(playerid);
				return 1;
		    }
		    else if(PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_clear") == playertextid)
		    {
		        DeletePVar(playerid, "hid_input_keypad_input");
		        PlayerTextDrawSetString(playerid, PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_input"), "-");
		        return 1;
		    }
		    else if(PlayerText:GetPVarInt(playerid, "hid_input_td_close") == playertextid)
		    {
				CallRemoteFunction("OnPlayerKeypadInput", "iii", playerid, -1, GetPVarInt(playerid, "hid_input_event"));
				CancelInput(playerid);
				return 1;
		    }
		    else
		    {
		        new str[30], PlayerText:td;
		        for(new b; b != 10; b++)
		        {
				    format(str, sizeof str, "hid_input_keypad_td_%i", b);
				    td = PlayerText:GetPVarInt(playerid, str);
				    if(td == playertextid)
				    {
				        for(new i; i != 30; i++) str[i] = '\0';
				        GetPVarString(playerid, "hid_input_keypad_input", str, 16);
				        if(strlen(str) >= GetPVarInt(playerid, "hid_input_keypad_maxlen")) return 1;
						str[strlen(str)] = b + 48;
				        SetPVarString(playerid, "hid_input_keypad_input", str);
				        td = PlayerText:GetPVarInt(playerid, "hid_input_keypad_td_input");
				        PlayerTextDrawSetString(playerid, td, str);
				        PlayerTextDrawShow(playerid, td);
						return 1;
				    }
				}
		    }
		
		}
		default: return CallLocalFunction("S@@_OnPlayerClickPlayerTextDraw", "ii", playerid, _:playertextid);
	}
	return CallLocalFunction("S@@_OnPlayerClickPlayerTextDraw", "ii", playerid, _:playertextid);
}

#if defined ALS_OnPlayerClickPlayerTextDraw
    #undef OnPlayerClickPlayerTextDraw
#else
    #define ALS_OnPlayerClickPlayerTextDraw
#endif
#define OnPlayerClickPlayerTextDraw S@@_OnPlayerClickPlayerTextDraw
forward OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(_:clickedid == 65535 && GetPVarInt(playerid, "hid_input_active") != 0)
	{
		if(GetPVarInt(playerid, "hid_input_td_close") != 0) CancelInput(playerid);
		else SelectTextDraw(playerid, 0xFFFFFFFF);
		return 1;
	}
	return CallLocalFunction("S@@_OnPlayerClickTextDraw", "ii", playerid, _:clickedid);
}

#if defined ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define ALS_OnPlayerClickTextDraw
#endif
#define OnPlayerClickTextDraw S@@_OnPlayerClickTextDraw
forward OnPlayerClickTextDraw(playerid, Text:clickedid);

public OnPlayerDisconnect(playerid, reason)
{
	new list, str[30], substr[30], PlayerText:td;
	format(str, sizeof str, "%i. Not connected", playerid);
	format(substr, sizeof substr, "hid_input_playlist_td_%i", playerid % 20);
	for(new i; i != MAX_PLAYERS; i++)
	{
		if(GetPVarInt(i, "hid_input_active") != HID_INPUT_PLAYLIST) continue;
		list = GetPVarInt(i, "hid_input_playlist_list");
		if(playerid >= list && playerid < (list + 19))
		{
			td = PlayerText:GetPVarInt(i, substr);
			PlayerTextDrawSetString(i, td, str);
			PlayerTextDrawColor(i, td, 0x505050FF);
			PlayerTextDrawSetSelectable(i, td, false);
			PlayerTextDrawShow(i, td);
		}
	}
	return CallLocalFunction("S@@_OnPlayerDisconnect", "ii", playerid, reason);
}

#if defined ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect S@@_OnPlayerDisconnect
forward OnPlayerDisconnect(playerid, reason);

public OnPlayerConnect(playerid)
{
	new list, str[30], substr[30], PlayerText:td;
	GetPlayerName(playerid, str, MAX_PLAYER_NAME);
	format(substr, sizeof substr, "hid_input_playlist_td_%i", playerid % 20);
	for(new i; i != MAX_PLAYERS; i++)
	{
		if(GetPVarInt(i, "hid_input_active") != HID_INPUT_PLAYLIST) continue;
		list = GetPVarInt(i, "hid_input_playlist_list");
		if(playerid >= list && playerid < (list + 19))
		{
			td = PlayerText:GetPVarInt(i, substr);
			PlayerTextDrawSetString(i, td, str);
			PlayerTextDrawColor(i, td, 0xA3B4C5FF);
			PlayerTextDrawSetSelectable(i, td, true);
			PlayerTextDrawShow(i, td);
		}
	}
	return CallLocalFunction("S@@_OnPlayerConnect", "i", playerid);
}

#if defined ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define ALS_OnPlayerConnect
#endif
#define OnPlayerConnect S@@_OnPlayerConnect
forward OnPlayerConnect(playerid, reason);