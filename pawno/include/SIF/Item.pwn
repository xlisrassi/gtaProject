/*==============================================================================

Southclaw's Interactivity Framework (SIF) (Formerly: Adventure API)

	SIF Version: 1.3.0
	Module Version: 1.8.2


	SIF/Overview
	{
		SIF is a collection of high-level include scripts to make the
		development of interactive features easy for the developer while
		maintaining quality front-end gameplay for players.
	}

	SIF/Item/Description
	{
		A complex and flexible script to replace the use of pickups as a means
		of displaying objects that the player can pick up and use. Item offers
		picking up, dropping and even giving items to other players. Items in
		the game world consist of static objects combined with buttons from
		SIF/Button to provide a means of interacting.

		Item aims to be an extremely flexible script offering a callback for
		almost every action the player can do with an item. The script also
		allows the ability to add the standard GTA:SA weapons as items that can
		be dropped, given and anything else you script items to do.

		When picked up, items will appear on the character model bone specified
		in the item definition. This combines the visible aspect of weapons and
		items that are already in the game with the scriptable versatility of
		server created and scriptable entities.
	}

	SIF/Item/Dependencies
	{
		SIF/Button
		Streamer Plugin
		YSI\y_hooks
		YSI\y_timers
	}

	SIF/Item/Credits
	{
		SA:MP Team						- Amazing mod!
		SA:MP Community					- Inspiration and support
		Incognito						- Very useful streamer plugin
		Y_Less							- YSI framework
		Patrik356b						- Testing
		Kevin							- Testing
		Cagatay							- Testing
	}

	SIF/Item/Constants
	{
		These can be altered by defining their values before the include line.

		ITM_MAX
			Maximum amount of items that can be created.

		ITM_MAX_TYPES
			Maximum amount of item types that can be defined.

		ITM_MAX_NAME
			Maximum string length for item type names.

		ITM_MAX_TEXT
			Maximum string length for item specific extra text.

		ITM_ATTACH_INDEX
			Item attachment index for SetPlayerAttachedObject native.

		ITM_DROP_ON_DEATH
			Places an item at a players death position when true.
	}

	SIF/Item/Core Functions
	{
		The functions that control the core features of this script.

		native -
		native SIF/Item/Core
		native -

		native CreateItem(ItemType:type, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 1000.0, Float:ry = 1000.0, Float:rz = 1000.0, Float:zoffset = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1)
		{
			Description:
				Creates an item in the game world at the specified coordinates
				with the specified rotation with options for world, interior and
				whether or not to display a 3D text label above the item.

			Parameters:
				<type> (int, ItemType)
					An item type defined with DefineItemType to determine what
					model the item will use and what characteristics it has.

				<x>, <y>, <z> (float)
					The position to create the object and button of the item.

				<rx>, <ry>, <rz> (float)
					The rotation value of the object, overrides item type data.

				<zoffset> (float)
					How high from the object the button will be created, this is
					to ensure the item will always be in pickup range when
					created on the floor.

				<world> (int)
					The virtual world in which the object, button and label will
					appear, only players in this world can see or pick up the
					item.

				<interior> (int)
					Interior world, same as above but for interior worlds.

				<label> (boolean)
					True to show a label with the item name at the item.

				<applyrotoffsets> (boolean)
					False to make <rx>, <ry>, <rz> rotation values absolute.

			Returns:
				(int, itemid)
					Item ID handle of the newly created item.

				INVALID_ITEM_ID
					If the item index is full and no more items can be created.
		}

		native DestroyItem(itemid)
		{
			Description:
				Destroys an item.

			Parameters:
				<itemid> (int, itemid)
					The item handle ID to delete.

			Returns:
				1
					If destroying the item was successful

				0
					If <itemid> is an invalid item ID handle.
		}

		native ItemType:DefineItemType(name[], model, size, Float:rotx = 0.0, Float:roty = 0.0, Float:rotz = 0.0, Float:zoffset = 0.0, Float:attx = 0.0, Float:atty = 0.0, Float:attz = 0.0, Float:attrx = 0.0, Float:attry = 0.0, Float:attrz = 0.0, colour = -1, boneid = 6)
		{
			Description:
				Defines a new item type with the specified name and model. Item
				types are the fundamental pieces of data that give items
				specific characteristics. At least one item definition must
				exist or CreateItem will have no data to use.

			Parameters:
				<name> (string)
					The name of the item, this will be displayed on the label
					of items created using this type (if one is present)

				<model> (int, valid GTA:SA model)
					The GTA:SA model id to use when the item is visible in the
					game world or attached to a player.

				<size> (int, pre-defined size definitions)
					The size of the item has no current use in this script,
					though it can be used in external scripts. It is used in
					SIF/Inventory to disallow large objects in a player's
					inventory.

				<rotx>, <roty>, <rotz> (float)
					The default rotation the item object will have when dropped.

				<zoffset>
					Z offset from the item world position to create item model.

				<attx>, <atty>, <attz>, <attrx>, <attry>, <attrz> (float)
					The attachment coordinates to use when the object is picked
					up and held by a player.

				<colour>
					Item model texture colour.

				<boneid> (int, valid SA:MP bones)
					The attachment bone to use, by default this is a hand but
					it's added for special circumstances such as an animation
					problem with held and drinkable beer bottles.

			Returns:
				(int, ItemType)
					Item Type ID handle of the newly defined item type.

				INVALID_ITEM_TYPE
					If the item type definition index is full and no more item
					types can be defined.
		}

		native ShiftItemTypeIndex(ItemType:start, amount)
		{
			Description:
				Shifts the entire item definition index to create <amount> empty
				cells from cell <start>. This can be used to create free spaces
				in the index for items that you want to have a specific ID. For 
				instance, weapons that start from 1 and end at 46.

			Parameters:
				<start> (int, cell index)
					The start cell to shift from, all definitions before this
					won't be affected in any way. All definitions after this
					will be moved up.

				<amount> (int)
					The amount of cells to shift the definitions up by.

			Returns:
				1
					If the shift was successful.

				0
					If the entered values would result in an out-of-bounds
					error with the index.
		}

		native PlayerPickUpItem(playerid, itemid, animtype)
		{
			Description:
				A function to directly make a player pick up an item, regardless
				of whether he is within the button range.

			Parameters:
				<playerid> (int)
					The player to force into picking up something.

				<itemid> (int, itemid)
					The item ID handle to force the player to pick up.

				<animtype> (int, pre-defined)
					The animation type is internally determined by the height
					of the item when the player picks up an item. With this
					function you can specify which animation to use.
						0 item on the ground (bend down)
						1 item in front (reach forward)

			Returns:
		}

		native PlayerDropItem(playerid)
		{
			Description:
				Force a player to drop his currently held item.

			Parameters:
				<playerid> (int)
					The player to force into dropping his item.

			Returns:
				1
					If the function was called successfully

				0
					If the player isn't holding an item, of the function was
					stopped by a return of 1 in OnPlayerDropItem.
		}

		native PlayerGiveItem(playerid, targetid, call)
		{
			Description:
				Forces a player to directly give his currently held item to
				another player regardless of distance.

			Parameters:
				<playerid> (int)
					The player who will give his item.

				<targetid> (int)
					The player who will receive the item.

				<call> (bool)
					Determines whether OnPlayerGiveItem is called.

			Returns:
				1
					If the give was successful.

				0
					If the player isn't holding an item, of the function was
					stopped by a return of 1 in OnPlayerGiveItem.

				-1
					If the target player was already holding an item.
		}

		native PlayerUseItem(playerid)
		{
			Description:
				Forces a player to use his current item, resulting in a call to
				OnPlayerUseItem.

			Parameters:
				-

			Returns:
				Whatever OnPlayerUseItem returns.
		}

		native GiveWorldItemToPlayer(playerid, itemid, call = 1)
		{
			Description:
				Give a world item to a player.

			Parameters:
				<playerid> (int)
					The player to give the item to.

				<itemid> (int, itemid)
					The ID handle of the item to give to the player.

				<call> (bool)
					Determines whether OnPLayerPickUpItem is called.

			Returns:
				0
					If the item ID is invalid or the item is already being held.
		}

		native RemoveCurrentItem(playerid)
		{
			Description:
				Removes the player's currently held item and places it in the
				world.

			Parameters:
				<playerid> (int)
					Player to remove item from.

			Returns:
				INVALID_ITEM_ID
					If the player ID is invalid or the player isn't holding an
					item.
		}

		native RemoveItemFromWorld(itemid)
		{	
			Description:
				Removes an item from the world. Deletes all physical elements
				but keeps the item in memory with a valid ID  and removes the
				ID from the world index. Effectively makes the item a "virtual"
				item, as in it still exists in the server memory but it doesn't
				exist physically in the game world.

			Parameters:
				<itemid> (int)
					The ID handle of the item to remove.

			Returns:
				1
					On success

				0
					If the item is invalid or not in the world.
		}
	}

	SIF/Item/Events
	{
		Events called by player actions done by using features from this script.

		native -
		native SIF/Item/Events
		native -

		native OnItemTypeDefined(ItemType:itemtype)
		{
			Called:
				After an item type is defined.

			Parameters:
				<itemtype> (int, ItemType)
					The ID handle of the newly defined item type.

			Returns:
				(nothing)
		}

		native OnItemCreate(itemid)
		{
			Called:
				As an item is created.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the newly created item.

			Returns:
				(nothing)
		}

		native OnItemCreated(itemid)
		{
			Called:
				After an item is created.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the newly created item.

			Returns:
				(nothing)
		}

		native OnItemDestroy(itemid)
		{
			Called:
				Before an item is destroyed, the itemid is still valid and
				existing.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the destroyed item.

			Returns:
				(nothing)
		}

		native OnItemDestroyed(itemid)
		{
			Called:
				After an item is destroyed, itemid is now invalid.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the destroyed item.

			Returns:
				(nothing)
		}

		native OnItemCreateInWorld(itemid)
		{
			Called:
				After an existing (already created with CreateItem) item is
				created in the game world (for instance, after a player drops
				the item, or directly after it is created with CreateItem)

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the created item.

			Returns:
				(nothing)
		}

		native OnPlayerUseItem(playerid, itemid)
		{
			Called:
				When a player presses F/Enter while holding an item.

			Parameters:
				<playerid> (int)
					The player who pressed F to use an item.

				<itemid> (int, itemid)
					The ID handle of the item the player is holding.

			Returns:
				(nothing)
		}

		native OnPlayerUseItemWithItem(playerid, itemid, withitemid)
		{
			Called:
				When a player uses a held item with an item in the world.

			Parameters:
				<playerid> (int)
					The player who used his item with a world item.

				<itemid> (int, itemid)
					The item the player is holding.

				<withitemid>
					The world item that the player used his item with.

			Returns:
				(nothing)
		}

		native OnPlayerUseItemWithButton(playerid, buttonid, itemid)
		{
			Called:
				When a player uses an item while in the area of a button from an
				item that is in the game world.

			Parameters:
				<playerid> (int)
					The player who used his item with a button.

				<buttonid> (int, buttonid)
					The button the player used the item with.

				<itemid> (int, itemid)
					The item the player used with the button.

			Returns:
				(nothing)
		}

		native OnPlayerPickUpItem(playerid, itemid)
		{
			Called:
				When a player presses the button to pick up an item.

			Parameters:
				<playerid> (int)
					The player who is requesting to pick up an item.

				<itemid> (int, itemid)
					The ID handle of the item the player pressed F at.

			Returns:
				1
					To cancel the pickup request, no animation will play.
		}

		native OnPlayerPickedUpItem(playerid, itemid)
		{
			Called:
				When a player finishes the picking up animation.

			Parameters:
				<playerid> (int)
					The player who picked up the item.

				<itemid> (int, itemid)
					The ID handle of the item the player picked up.

			Returns:
				1
					To cancel giving the item ID to the player.
		}

		native OnPlayerGetItem(playerid, itemid)
		{
			Called:
				When a player acquires an item from any source.

			Parameters:
				<playerid> (int)
					The player who received the item.

				<itemid> (int, itemid)
					The ID handle of the item the player acquired.

			Returns:
				(nothing)
		}

		native OnPlayerDropItem(playerid, itemid)
		{
			Called:
				When a player presses the button to drop an item.

			Parameters:
				<playerid> (int)
					The player who pressed F to request to drop his item.

				<itemid> (int, itemid)
					The ID handle of the item the player requested to drop.

			Returns:
				1
					To cancel the drop, no animation will play and the player
					will keep his item.
		}

		native OnPlayerDroppedItem(playerid, itemid)
		{
			Called:
				When a player finishes the animation for dropping an item.

			Parameters:
				<playerid> (int)
					The player who dropped his item.

				<itemid> (int, itemid)
					The ID handle of the item the player dropped.

			Returns:
				1
					To cancel removing the item from the player.
		}

		native OnPlayerGiveItem(playerid, targetid, itemid)
		{
			Called:
				When a player presses the button to give an item to another
				player.

			Parameters:
				<playerid> (int)
					The player who pressed F to request giving an item.

				<targetid> (int)
					The target player who will receive the item.

				<itemid> (int, itemid)
					The ID handle of the item that will be given.

			Returns:
				1
					To cancel the give request, no animations will play.
		}

		native OnPlayerGivenItem(playerid, targetid, itemid)
		{
			Called:
				When a player finishes the animation for giving an item to
				another player.

			Parameters:
				<playerid> (int)
					The player who gave his item.

				<targetid> (int)
					The target player who received the item.

				<itemid> (int, itemid)
					The ID handle of the item that was given.

			Returns:
				1
					To cancel removing the item from the giver and the target
					receiving the item.
		}

		native OnItemRemovedFromPlayer(playerid, itemid)
		{
			Called:
				When an item is removed from a player's hands without him
				dropping it (through a script action)

			Parameters:
				<playerid> (int)
					The player who's item was removed.

				<itemid> (int, itemid)
					The ID handle of the item that was removed.

			Returns:
				(none)
		}

		native OnItemNameRender(itemid)
		{
			Called:
				When the function GetItemName is called, so an additional piece
				of text can be added to items giving more information unique to
				that specific item.

			Parameters:
				<itemid>
					The ID handle of the item that had it's name requested.

			Returns:
				(none)
		}
	}

	SIF/Item/Interface Functions
	{
		Functions to get or set data values in this script without editing
		the data directly. These include automatic ID validation checks.

		native -
		native - SIF/Item/Interface
		native -

		native IsValidItem(itemid)
		{
			Description:
				Returns whether the entered value is a valid item ID handle.

			Parameters:
				<itemid> (int, itemid)
					Item ID value to check.

			Returns:
				1
					If the item ID is valid.

				0
					If the item Id is invalid.
		}

		native GetItemObjectID(itemid)
		{
			Description:
				Returns the streamed object ID for a world item.

			Parameters:
				<itemid> (int, itemid)
					The item ID to get the object ID of (must be an item that is
					in the game world and not a virtual item such as one held
					by a player.)

			Returns:
				(int)
					The ID of the streamed object used for the item.

				INVALID_OBJECT_ID
					If the item is invalid or not a world item.
		}

		native GetItemButtonID(itemid)
		{
			Description:
				Returns the button ID of a world item.

			Parameters:
				<itemid> (int, itemid)
					The item ID to get the button ID of (must be an item that is
					in the game world and not a virtual item such as one held
					by a player.)

			Returns:
				(int)
					The ID of the button used for the item.

				INVALID_BUTTON_ID
					If the item is invalid or not a world item.
		}

		native SetItemLabel(itemid, text[], colour = 0xFFFF00FF, Float:range = 10.0)
		{
			Description:
				Creates or updates a 3D text label above the item.
				This is actually the label which is associated with the button
				for the item, so you could just call GetItemButtonID then use
				SetButtonLabel but this is just here for convenience.

			Parameters:
				<itemid> (int, itemid)
					The item ID to set the label for.

				<text> (string)
					The text to display in the label.

				<colour> (int)
					The colour to set the label to.

				<range> (float)
					The stream distance for the label.

			Returns:
				1
					If the function was successful.

				0
					If the ID handle of the item is invalid.
		}

		native GetItemType(itemid)
		{
			Description:
				Returns the item type of an item.

			Parameters:
				<itemid> (int)
					The ID handle of the item to get the type of.

			Returns:
				(int, ItemType)
					Item type of the item.

				INVALID_ITEM_TYPE
					If the entered item ID handle is invalid.
		}

		native GetItemPos(itemid, &Float:x, &Float:y, &Float:z)
		{
			Description:
				Returns the position of a world item. If used on a non-world
				item such as an item being held by a player, it will return the
				last position of the item.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to get the position of.

				<x>, <y>, <z> (float, absolute world position)
					The position variables passed by reference.

			Returns:
				0
					If the entered item ID handle is invalid.
		}

		native SetItemPos(itemid, Float:x, Float:y, Float:z, Float:zoffset = 0.0)
		{

			Description:
				Returns the position of a world item. If used on a non-world
				item such as an item being held by a player, it will return the
				last position of the item.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to get the position of.

				<x>, <y>, <z> (float, absolute world position)
					The position variables passed by reference.

				<zoffset> (float)
					A height offset for the item's button position.

			Returns:
				0
					If the entered item ID handle is invalid.
		}

		native GetItemRot(itemid, &Float:rx, &Float:ry, &Float:rz)
		{
			Description:
				Returns the rotation of a world item.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to get the rotation of.

				<x>, <y>, <z> (float)
					The rotation variables passed by reference.

			Returns:
				0
					If the entered item ID handle is invalid.
		}

		native SetItemRot(itemid, Float:rx, Float:ry, Float:rz, bool:offsetfromdefaults)
		{
			Description:
				Sets the rotation of a world item object regardless of the item
				type rotation offset values.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to set the rotation of.

				<rx>, <ry>, <rz> (float, euler angles)
					The rotation values to set the object to.

				<offsetfromdefaults> (boolean)
					Set to true to turn the entered values into offsets from the
					default rotation values of the item type.

			Returns:
				0
					If the entered item ID handle is invalid.
		}

		native SetItemWorld(itemid, world)
		{
			Description:
				Sets an item's virtual world.

			Parameters:
				-

			Returns:
				-
		}

		native GetItemWorld(itemid)
		{
			Description:
				Returns an item's virtual world.

			Parameters:
				-

			Returns:
				-
		}

		native SetItemInterior(itemid, interior)
		{
			Description:
				Sets an item's interior ID.

			Parameters:
				-

			Returns:
				-
		}

		native GetItemInterior(itemid)
		{
			Description:
				Returns an item's interior ID.

			Parameters:
				-

			Returns:
				-
		}

		native SetItemExtraData(itemid, data)
		{
			Description:
				Sets the item's extra data field, this is one cell of data space
				allocated for each item, this value can be a simple value or
				point to a cell in a more complex set of data to act as extra
				characteristics for items.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to set the extra data of.

				<data> (int)
					A single 32 bit integer cell to store with the item.

			Returns:
				0
					If the entered item ID handle is invalid.
		}

		native GetItemExtraData(itemid)
		{
			Description:
				Retrieves the integer assigned to the item set with
				SetItemExtraData.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to retrieve the data of.

			Returns:
				(int)
					The integer stored with the item.

				0
					If the entered item ID handle is invalid.
		}

		native SetItemNameExtra(itemid, string[])
		{
			Description:
				Gives the item a unique string of text.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to set the text of.

				<string> (string)
					The string to give to the item.

			Returns:
				0
					If the entered item ID handle is invalid.
		}

		native GetItemNameExtra(itemid, string[])
		{
			Description:
				Retrieves the unique string of text assigned to an item.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to get the text of.

				<string> (string)
					A variable to put the retrieved string into.

			Returns:
				0
					If the entered item ID handle is invalid.
		}

		native IsValidItemType(ItemType:itemtype)
		{
			Description:
				Checks whether a value is a valid item type.

			Parameters:
				<itemtype> (int, ItemType)
					The value to check.

			Returns:
				1
					If the entered value is a valid item type.
				0
					If the entered value is not in the item type index.
		}

		native GetItemTypeName(ItemType:itemtype, string[])
		{
			Description:
				Retrieves the name of an item type.

			Parameters:
				<itemtype> (int, ItemType)
					The item type to get the name of.

				<string> (string)
					The string to put the name into.

			Returns:
				0
					If itemtype is an invalid item type.
		}

		native GetItemTypeModel(ItemType:itemtype)
		{
			Description:
				Returns the model assigned to an item type.

			Parameters:
				<itemtype> (int, ItemType)
					Item type to get the model of.

			Returns:
				0
					If the item type is not in the item type index.
		}

		native GetItemTypeSize(ItemType:itemtype)
		{
			Description:
				Returns the defined size of an item type.

			Parameters:
				<itemtype> (int, ItemType)
					The item type to get the size of.

			Returns:
				0
					If the item type is not in the item type index.	
		}

		native GetItemTypeColour(ItemType:itemtype)
		{
			Description:
				Returns the default colour of an item type.

			Parameters:
				<itemtype> (int, ItemType)

			Returns:
				(int)
					Item type colour in ARGB order.
		}

		native GetItemTypeBone(itemid)
		{
			Description:
				Returns the bone that an item type will attach the mesh to.

			Parameters:
				-

			Returns:
				(int)
					A bone ID.
		}

		native GetItemHolder(itemid)
		{
			Description:
				Returns the ID of the player who is holding an item.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to get the holder of.

			Returns:
				0
					If the item ID handle is invalid.
		}

		native GetPlayerItem(playerid)
		{
			Description:
				Returns the item ID handle of the item a player is holding.

			Parameters:
				<playerid> (int)
					
			Returns:
				INVALID_ITEM_ID
					If the player isn't holding something or is an invalid
					player ID. There is no IsPlayerConnected check here.
		}

		native IsItemInWorld(itemid)
		{
			Description:
				Checks if an item is in the game world.

			Parameters:
				<itemid> (int)
					The ID handle of the item to check.
					
			Returns:
				1
					If the item is in the world.

				0
					If the item is not in the world.
		}

		native GetItemName(itemid, string[])
		{
			Description:
				Returns the name of the type of the specified item ID and
				appends the unique text assigned to the item to the end.

			Parameters:
				<itemid> (int, itemid)
					The ID handle of the item to get the name and text of.

				<string> (string)
					A variable to store the retrieved name and text in.
					
			Returns:
				0
					If the item ID handle is invalid.
		}

		native GetPlayerInteractingItem(playerid)
		{
			Description:
				Returns the ID handle of the item that <playerid> is interacting
				with. This means either picking up, dropping or giving.

			Parameters:
				<playerid> (int)
					The player ID to check.

			Returns:
				(int)
					ID handle of the item that <playerid> is interacting with.
		}

		native GetNextItemID()
		{
			Description:
				Returns the next item ID in the index that is unused.
				Useful for determining what ID an item will have before calling
				CreateItem and thus OnItemCreated.

			Parameters:
				-

			Returns:
				(int)
					An item ID between 0 and ITM_MAX that is not yet assigned.
		}
	}

	SIF/Item/Internal Functions
	{
		Internal events called by player actions done by using features from
		this script.

		CreateItemInWorld(itemid, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:zoffset = 0.0, world = 0, interior = 0, label = 1)
		{
			Description:
				Creates an item that is already added to the item index in the
				world. This means it is given an object and a button. This
				function is called by CreateItem.
		}

		internal_OnPlayerUseItem
		{
			Description:
				Called internally before the public OnPlayerUseItem to determine
				if the player is near any buttons to call
				OnPlayerUseItemWithButton instead.
		}

		PickUpItemDelay
		{
			Description:
				The timer function to activate the return to idle animation if
				needed and give the item to the player.
		}

		DropItemDelay
		{
			Description:
				The timer function to activate the return to idle animation and
				remove the item from the player and put it in the game world.
		}

		GiveItemDelay
		{
			Description:
				The timer function to give the recipient the given item and
				remove it from the giver.
		}
	}

	SIF/Item/Hooks
	{
		Hooked functions or callbacks, either SA:MP natives or from other
		scripts or plugins.

		SAMP/OnFilterScriptInit
		{
			Reason:
				Zero initialised array cells.
		}

		SAMP/OnPlayerConnect
		{
			Reason:
				Zero initialised array cells.
		}

		SAMP/OnPlayerKeyStateChange
		{
			Reason:
				Detect if the player presses F to use his held item or N to drop
				or give his held item.
		}

		SAMP/OnPlayerDeath
		{
			Reason:
				To remove items from the player and drop them at his death
				position. (may remove and let developers choose death activity)
		}

		YSI/OnScriptInit
		{
			Reason:
				Zero initialised array cells.
		}

		SIF/Core/OnPlayerEnterPlayerArea
		{
			Reason:
				To show a give item prompt.
		}

		SIF/Core/OnPlayerLeavePlayerArea
		{
			Reason:
				To hide the give item prompt.
		}

		SIF/Button/OnButtonPress
		{
			Reason:
				For picking up world items.
		}
	}

==============================================================================*/


#if defined _SIF_ITEM_INCLUDED
	#endinput
#endif

#if !defined _SIF_DEBUG_INCLUDED
	#include <SIF\Debug.pwn>
#endif

#if !defined _SIF_CORE_INCLUDED
	#include <SIF\Core.pwn>
#endif

#if !defined _SIF_BUTTON_INCLUDED
	#include <SIF\Button.pwn>
#endif

#if defined DEBUG_LABELS_ITEM
	#include <SIF\extensions/DebugLabels.inc>
#endif

#include <YSI\y_iterate>
#include <YSI\y_timers>
#include <YSI\y_hooks>
#include <streamer>

#define _SIF_ITEM_INCLUDED


/*==============================================================================

	Setup

==============================================================================*/


#if !defined ITM_MAX
	#define ITM_MAX				(8192)
#endif

#if !defined ITM_MAX_TYPES
	#define ITM_MAX_TYPES		(ItemType:256)
#endif

#if !defined ITM_MAX_NAME
	#define ITM_MAX_NAME		(32)
#endif

#if !defined ITM_MAX_TEXT
	#define ITM_MAX_TEXT		(32)
#endif

#if !defined ITM_ATTACH_INDEX
	#define ITM_ATTACH_INDEX	(0)
#endif

#if !defined ITM_DROP_ON_DEATH
	#define ITM_DROP_ON_DEATH	true
#endif


#define FLOOR_OFFSET		(0.96)
#define ITEM_BUTTON_OFFSET	(0.7)
#define INVALID_ITEM_ID		(-1)
#define INVALID_ITEM_TYPE	(ItemType:-1)

#define INVALID_ITEM_SIZE	(-1)
#define ITEM_SIZE_SMALL		(0)
#define ITEM_SIZE_MEDIUM	(1)
#define ITEM_SIZE_LARGE		(2)
#define ITEM_SIZE_CARRY		(3)


enum E_ITEM_DATA
{
			itm_objId,
			itm_button,
ItemType:	itm_type,

Float:		itm_posX,
Float:		itm_posY,
Float:		itm_posZ,
Float:		itm_rotX,
Float:		itm_rotY,
Float:		itm_rotZ,
			itm_world,
			itm_interior,

			itm_exData,
			itm_nameEx[ITM_MAX_TEXT]
}

enum E_ITEM_TYPE_DATA
{
bool:		itm_used,
			itm_name			[ITM_MAX_NAME],
			itm_model,
			itm_size,

Float:		itm_defaultRotX,
Float:		itm_defaultRotY,
Float:		itm_defaultRotZ,
Float:		itm_offsetZ,

Float:		itm_attachPosX,
Float:		itm_attachPosY,
Float:		itm_attachPosZ,

Float:		itm_attachRotX,
Float:		itm_attachRotY,
Float:		itm_attachRotZ,

			itm_colour,
			itm_attachBone
}


static
			itm_Data			[ITM_MAX][E_ITEM_DATA],
			itm_Interactor		[ITM_MAX],
			itm_Holder			[ITM_MAX];
new
Iterator:	itm_Index<ITM_MAX>,
Iterator:	itm_WorldIndex<ITM_MAX>,
			itm_ButtonIndex[BTN_MAX]
	#if defined DEBUG_LABELS_ITEM
		,
			itm_DebugLabelType,
			itm_DebugLabelID[ITM_MAX]
	#endif
		;


static
			itm_TypeData		[ITM_MAX_TYPES][E_ITEM_TYPE_DATA];

static
			itm_Holding			[MAX_PLAYERS],
			itm_Interacting		[MAX_PLAYERS],
Timer:		itm_InteractTimer	[MAX_PLAYERS];


forward OnItemTypeDefined(ItemType:itemtype);
forward OnItemCreate(itemid);
forward OnItemCreated(itemid);
forward OnItemDestroy(itemid);
forward OnItemDestroyed(itemid);
forward OnItemCreateInWorld(itemid);
forward OnPlayerUseItem(playerid, itemid);
forward OnPlayerUseItemWithItem(playerid, itemid, withitemid);
forward OnPlayerUseItemWithButton(playerid, buttonid, itemid);
forward OnPlayerPickUpItem(playerid, itemid);
forward OnPlayerPickedUpItem(playerid, itemid);
forward OnPlayerGetItem(playerid, itemid);
forward OnPlayerDropItem(playerid, itemid);
forward OnPlayerDroppedItem(playerid, itemid);
forward OnPlayerGiveItem(playerid, targetid, itemid);
forward OnPlayerGivenItem(playerid, targetid, itemid);
forward OnItemRemovedFromPlayer(playerid, itemid);
forward OnItemNameRender(itemid);


static ITEM_DEBUG;


/*==============================================================================

	Zeroing

==============================================================================*/


hook OnScriptInit()
{
	ITEM_DEBUG = sif_debug_register_handler("SIF/Item");
	sif_d:SIF_DEBUG_LEVEL_CALLBACKS:ITEM_DEBUG("[OnScriptInit]");

	for(new i; i < MAX_PLAYERS; i++)
	{
		itm_Holding[i] = INVALID_ITEM_ID;
		itm_Interacting[i] = INVALID_ITEM_ID;
	}

	for(new i; i < ITM_MAX; i++)
	{
		itm_Holder[i] = INVALID_PLAYER_ID;
	}

	for(new i; i < BTN_MAX; i++)
	{
		itm_ButtonIndex[i] = INVALID_ITEM_ID;
	}

	#if defined DEBUG_LABELS_ITEM
		itm_DebugLabelType = DefineDebugLabelType("ITEM", 0xFFFF00FF);
	#endif

	return 1;
}

hook OnPlayerConnect(playerid)
{
	sif_d:SIF_DEBUG_LEVEL_CALLBACKS:ITEM_DEBUG("[OnPlayerConnect]");
	itm_Holding[playerid]		= INVALID_ITEM_ID;
	itm_Interacting[playerid]	= INVALID_ITEM_ID;
}


/*==============================================================================

	Core Functions

==============================================================================*/


stock CreateItem(ItemType:type, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 1000.0, Float:ry = 1000.0, Float:rz = 1000.0, Float:zoffset = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1)
{
	sif_d:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[CreateItem]");
	new id = Iter_Free(itm_Index);

	if(id == -1)
	{
		print("ERROR: ITM_MAX reached, please increase this constant!");
		return INVALID_ITEM_ID;
	}

	if(!IsValidItemType(type))
	{
		printf("ERROR: Item creation with undefined typeid (%d) failed.", _:type);
		return INVALID_ITEM_ID;
	}

	Iter_Add(itm_Index, id);

	itm_Data[id][itm_type] = type;

	CallLocalFunction("OnItemCreate", "d", id);

	if(!Iter_Contains(itm_Index, id))
		return INVALID_ITEM_ID;

	CreateItemInWorld(id, x, y, z, rx, ry, rz, zoffset, world, interior, label, applyrotoffsets);

	CallLocalFunction("OnItemCreated", "d", id);

	#if defined DEBUG_LABELS_ITEM
		itm_DebugLabelID[id] = CreateDebugLabel(itm_DebugLabelType, id, x, y, z);
		UpdateItemDebugLabel(id);
	#endif

	return id;
}

stock DestroyItem(itemid, &indexid = -1, &worldindexid = -1)
{
	sif_d:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[DestroyItem]");
	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	CallLocalFunction("OnItemDestroy", "d", itemid);

	if(itm_Holder[itemid] != INVALID_PLAYER_ID)
	{
		if(itm_TypeData[itm_Data[itemid][itm_type]][itm_size] == ITEM_SIZE_CARRY)
			SetPlayerSpecialAction(itm_Holder[itemid], SPECIAL_ACTION_NONE);

		RemovePlayerAttachedObject(itm_Holder[itemid], ITM_ATTACH_INDEX);
		itm_Holding[itm_Holder[itemid]] = INVALID_ITEM_ID;
		itm_Interacting[itm_Holder[itemid]] = INVALID_ITEM_ID;
		stop itm_InteractTimer[itm_Holder[itemid]];
	}
	else
	{
		if(Iter_Contains(itm_WorldIndex, itemid))
		{
			DestroyDynamicObject(itm_Data[itemid][itm_objId]);
			DestroyButton(itm_Data[itemid][itm_button]);
			itm_ButtonIndex[itm_Data[itemid][itm_button]] = INVALID_BUTTON_ID;
		}
	}

	itm_Data[itemid][itm_objId] = -1;
	itm_Data[itemid][itm_button] = INVALID_BUTTON_ID;

	itm_Data[itemid][itm_type] = INVALID_ITEM_TYPE;
	itm_Data[itemid][itm_posX] = 0.0;
	itm_Data[itemid][itm_posY] = 0.0;
	itm_Data[itemid][itm_posZ] = 0.0;
	itm_Data[itemid][itm_rotX] = 0.0;
	itm_Data[itemid][itm_rotY] = 0.0;
	itm_Data[itemid][itm_rotZ] = 0.0;
	itm_Data[itemid][itm_exData] = 0;
	itm_Data[itemid][itm_nameEx][0] = EOS;

	itm_Holder[itemid]			= INVALID_PLAYER_ID;
	itm_Interactor[itemid]		= INVALID_PLAYER_ID;

	Iter_SafeRemove(itm_Index, itemid, indexid);
	Iter_SafeRemove(itm_WorldIndex, itemid, worldindexid);

	CallLocalFunction("OnItemDestroyed", "d", itemid);

	#if defined DEBUG_LABELS_BUTTON
		DestroyDebugLabel(itm_DebugLabelID[itemid]);
	#endif

	return 1;
}

stock ItemType:DefineItemType(name[], model, size, Float:rotx = 0.0, Float:roty = 0.0, Float:rotz = 0.0, Float:zoffset = 0.0, Float:attx = 0.0, Float:atty = 0.0, Float:attz = 0.0, Float:attrx = 0.0, Float:attry = 0.0, Float:attrz = 0.0, colour = -1, boneid = 6)
{
	sif_d:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[DefineItemType]");
	new ItemType:id;

	while(id < ITM_MAX_TYPES && itm_TypeData[id][itm_used])
		id++;

	if(id == ITM_MAX_TYPES)
	{
		print("ERROR: Reached item type limit.");
		return INVALID_ITEM_TYPE;
	}

	itm_TypeData[id][itm_used]			= true;
	format(itm_TypeData[id][itm_name], ITM_MAX_NAME, name);
	itm_TypeData[id][itm_model]			= model;
	itm_TypeData[id][itm_size]			= size;

	itm_TypeData[id][itm_defaultRotX]	= rotx;
	itm_TypeData[id][itm_defaultRotY]	= roty;
	itm_TypeData[id][itm_defaultRotZ]	= rotz;
	itm_TypeData[id][itm_offsetZ]		= zoffset;

	itm_TypeData[id][itm_attachPosX]	= attx;
	itm_TypeData[id][itm_attachPosY]	= atty;
	itm_TypeData[id][itm_attachPosZ]	= attz;

	itm_TypeData[id][itm_attachRotX]	= attrx;
	itm_TypeData[id][itm_attachRotY]	= attry;
	itm_TypeData[id][itm_attachRotZ]	= attrz;

	itm_TypeData[id][itm_colour]		= colour;
	itm_TypeData[id][itm_attachBone]	= boneid;

	CallLocalFunction("OnItemTypeDefined", "d", _:id);

	return id;
}

stock ShiftItemTypeIndex(ItemType:start, amount)
{
	sif_d:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[ShiftItemTypeIndex]");
	if(!(start <= (start + ItemType:amount) < ITM_MAX_TYPES))
		return 0;

	new ItemType:lastfree;
	while(lastfree < ITM_MAX_TYPES && itm_TypeData[lastfree][itm_used])
		lastfree++;

	for(new ItemType:i = lastfree - ItemType:1; i >= start; i--)
	{
		if(!IsValidItemType(i))
			continue;

		if(!(i <= (i + ItemType:amount) < ITM_MAX_TYPES))
			continue;


		itm_TypeData[i + ItemType:amount][itm_used]			= true;
		itm_TypeData[i + ItemType:amount][itm_name]			= itm_TypeData[i][itm_name];
		itm_TypeData[i + ItemType:amount][itm_model]			= itm_TypeData[i][itm_model];
		itm_TypeData[i + ItemType:amount][itm_size]			= itm_TypeData[i][itm_size];

		itm_TypeData[i + ItemType:amount][itm_defaultRotX]			= itm_TypeData[i][itm_defaultRotX];
		itm_TypeData[i + ItemType:amount][itm_defaultRotY]			= itm_TypeData[i][itm_defaultRotY];
		itm_TypeData[i + ItemType:amount][itm_defaultRotZ]			= itm_TypeData[i][itm_defaultRotZ];

		itm_TypeData[i + ItemType:amount][itm_attachBone]	= itm_TypeData[i][itm_attachBone];
		itm_TypeData[i + ItemType:amount][itm_attachPosX]	= itm_TypeData[i][itm_attachPosX];
		itm_TypeData[i + ItemType:amount][itm_attachPosY]	= itm_TypeData[i][itm_attachPosY];
		itm_TypeData[i + ItemType:amount][itm_attachPosZ]	= itm_TypeData[i][itm_attachPosZ];

		itm_TypeData[i + ItemType:amount][itm_attachRotX]	= itm_TypeData[i][itm_attachRotX];
		itm_TypeData[i + ItemType:amount][itm_attachRotY]	= itm_TypeData[i][itm_attachRotY];
		itm_TypeData[i + ItemType:amount][itm_attachRotZ]	= itm_TypeData[i][itm_attachRotZ];


		itm_TypeData[i][itm_used]							= false;
		itm_TypeData[i][itm_name][0]							= EOS;
		itm_TypeData[i][itm_model]							= 0;
		itm_TypeData[i][itm_size]							= 0;

		itm_TypeData[i][itm_defaultRotX]							= 0.0;
		itm_TypeData[i][itm_defaultRotY]							= 0.0;
		itm_TypeData[i][itm_defaultRotZ]							= 0.0;

		itm_TypeData[i][itm_attachBone]						= 0;
		itm_TypeData[i][itm_attachPosX]						= 0.0;
		itm_TypeData[i][itm_attachPosY]						= 0.0;
		itm_TypeData[i][itm_attachPosZ]						= 0.0;

		itm_TypeData[i][itm_attachRotX]						= 0.0;
		itm_TypeData[i][itm_attachRotY]						= 0.0;
		itm_TypeData[i][itm_attachRotZ]						= 0.0;
	}
	
	return 1;
}

stock PlayerPickUpItem(playerid, itemid)
{
	sif_dp:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[PlayerPickUpItem]")<playerid>;
	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	if(Iter_Contains(itm_Index, itm_Holding[playerid]))
		return 0;

	new
		Float:x,
		Float:y,
		Float:z;

	GetPlayerPos(playerid, x, y, z);

	ClearAnimations(playerid);
	SetPlayerPos(playerid, x, y, z);
	SetPlayerFacingAngle(playerid, sif_GetAngleToPoint(x, y, itm_Data[itemid][itm_posX], itm_Data[itemid][itm_posY]));

	if((z - itm_Data[itemid][itm_posZ]) < 0.3) // If the height between the player and the item is below 0.5 units
	{
		if(itm_TypeData[itm_Data[itemid][itm_type]][itm_size] == ITEM_SIZE_CARRY)
			ApplyAnimation(playerid, "CARRY", "liftup105", 5.0, 0, 0, 0, 0, 400);

		else
			ApplyAnimation(playerid, "CASINO", "SLOT_PLYR", 4.0, 0, 0, 0, 0, 0);

		itm_InteractTimer[playerid] = defer PickUpItemDelay(playerid, itemid, 1);
	}
	else
	{
		if(itm_TypeData[itm_Data[itemid][itm_type]][itm_size] == ITEM_SIZE_CARRY)
			ApplyAnimation(playerid, "CARRY", "liftup", 5.0, 0, 0, 0, 0, 400);

		else
			ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 5.0, 0, 0, 0, 0, 450);

		itm_InteractTimer[playerid] = defer PickUpItemDelay(playerid, itemid, 0);
	}

	itm_Interacting[playerid] = itemid;
	itm_Interactor[itemid] = playerid;

	return 1;
}

stock PlayerDropItem(playerid)
{
	sif_dp:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[PlayerDropItem]")<playerid>;
	if(!Iter_Contains(itm_Index, itm_Holding[playerid]))
		return 0;

	if(CallLocalFunction("OnPlayerDropItem", "dd", playerid, itm_Holding[playerid]))
		return 0;

	if(itm_TypeData[itm_Data[itm_Holding[playerid]][itm_type]][itm_size] == ITEM_SIZE_CARRY)
		ApplyAnimation(playerid, "CARRY", "putdwn", 5.0, 0, 0, 0, 0, 0);

	else
		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 5.0, 1, 0, 0, 0, 450);

	itm_InteractTimer[playerid] = defer DropItemDelay(playerid);

	return 1;
}

stock PlayerGiveItem(playerid, targetid, call = true)
{
	sif_dp:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[PlayerGiveItem]")<playerid>;
	if(!(0 <= playerid < MAX_PLAYERS))
		return 0;

	if(!Iter_Contains(itm_Index, itm_Holding[playerid]))
		return 0;

	new itemid = itm_Holding[playerid];

	if(call)
	{
		if(CallLocalFunction("OnPlayerGiveItem", "ddd", playerid, targetid, itemid))
			return 0;
	}

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	if(Iter_Contains(itm_Index, itm_Holding[targetid]))
		return 0;

	new
		Float:x1,
		Float:y1,
		Float:z1,
		Float:x2,
		Float:y2,
		Float:z2,
		Float:angle;

	GetPlayerPos(targetid, x1, y1, z1);
	GetPlayerPos(playerid, x2, y2, z2);

	angle = sif_GetAngleToPoint(x2, y2, x1, y1);

	SetPlayerFacingAngle(playerid, angle);
	SetPlayerFacingAngle(targetid, angle+180.0);

	if(itm_TypeData[itm_Data[itemid][itm_type]][itm_size] != ITEM_SIZE_CARRY)
	{
		ApplyAnimation(playerid, "CASINO", "SLOT_PLYR", 4.0, 0, 0, 0, 0, 450);
		ApplyAnimation(targetid, "CASINO", "SLOT_PLYR", 4.0, 0, 0, 0, 0, 450);
	}
	else
	{
		SetPlayerSpecialAction(targetid, SPECIAL_ACTION_CARRY);
	}

	itm_Interacting[playerid]	= targetid;
	itm_Interacting[targetid]	= playerid;
	itm_Holder[itemid]			= playerid;

	itm_InteractTimer[playerid] = defer GiveItemDelay(playerid, targetid);

	return 1;
}

stock PlayerUseItem(playerid)
{
	sif_dp:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[PlayerUseItem]")<playerid>;
	return internal_OnPlayerUseItem(playerid, itm_Holding[playerid]);
}

stock GiveWorldItemToPlayer(playerid, itemid, call = 1)
{
	sif_dp:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[GiveWorldItemToPlayer] playerid: %d itemid: %d call: %d", playerid, itemid, call)<playerid>;
	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	if(Iter_Contains(itm_WorldIndex, itemid))
	{
		if(itm_Holder[itemid] != INVALID_PLAYER_ID)
			return 0;
	}

	new
		ItemType:type = itm_Data[itemid][itm_type];

	itm_Data[itemid][itm_posX]		= 0.0;
	itm_Data[itemid][itm_posY]		= 0.0;
	itm_Data[itemid][itm_posZ]		= 0.0;

	itm_Holding[playerid]			= itemid;
	itm_Holder[itemid]				= playerid;
	itm_Interacting[playerid]		= INVALID_ITEM_ID;
	itm_Interactor[itemid]			= INVALID_PLAYER_ID;

	if(Iter_Contains(itm_WorldIndex, itemid))
	{
		sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] Item in world, destroying object")<playerid>;
		DestroyDynamicObject(itm_Data[itemid][itm_objId]);
		sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] Destroying button")<playerid>;
		DestroyButton(itm_Data[itemid][itm_button]);
		sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] Resetting array data")<playerid>;
		itm_ButtonIndex[itm_Data[itemid][itm_button]] = INVALID_BUTTON_ID;

		itm_Data[itemid][itm_objId] = -1;
		itm_Data[itemid][itm_button] = INVALID_BUTTON_ID;
	}

	SetPlayerAttachedObject(
		playerid, ITM_ATTACH_INDEX, itm_TypeData[type][itm_model], itm_TypeData[type][itm_attachBone],
		itm_TypeData[type][itm_attachPosX], itm_TypeData[type][itm_attachPosY], itm_TypeData[type][itm_attachPosZ],
		itm_TypeData[type][itm_attachRotX], itm_TypeData[type][itm_attachRotY], itm_TypeData[type][itm_attachRotZ],
		.materialcolor1 = itm_TypeData[type][itm_colour], .materialcolor2 = itm_TypeData[type][itm_colour]);

	if(itm_TypeData[type][itm_size] == ITEM_SIZE_CARRY)
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

	sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] Removing item from world index")<playerid>;
	Iter_Remove(itm_WorldIndex, itemid);

	if(call)
	{
		sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] Calling OnPlayerGetItem")<playerid>;
		if(CallLocalFunction("OnPlayerGetItem", "dd", playerid, itemid))
			return 0;

		sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] Checking if item still exists")<playerid>;
		if(!Iter_Contains(itm_Index, itemid))
		{
			sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] Item does not exist, end of function")<playerid>;
			return 0;
		}
	}

	sif_dp:SIF_DEBUG_LEVEL_CORE_DEEP:ITEM_DEBUG("[GiveWorldItemToPlayer] End of function")<playerid>;
	return 1;
}

stock RemoveCurrentItem(playerid)
{
	sif_dp:SIF_DEBUG_LEVEL_CORE:ITEM_DEBUG("[RemoveCurrentItem]")<playerid>;
	if(!(0 <= playerid < MAX_PLAYERS))
		return INVALID_ITEM_ID;

	if(!Iter_Contains(itm_Index, itm_Holding[playerid]))
		return INVALID_ITEM_ID;

	new
		itemid = itm_Holding[playerid],
		Float:x,
		Float:y,
		Float:z,
		Float:r;

	RemovePlayerAttachedObject(playerid, ITM_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	itm_Holding[playerid] = INVALID_ITEM_ID;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	itm_Holder[itemid] = INVALID_PLAYER_ID;
	itm_Interactor[itemid] = INVALID_PLAYER_ID;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, r);

	CreateItemInWorld(itemid,
		x + (0.5 * floatsin(-r, degrees)),
		y + (0.5 * floatcos(-r, degrees)),
		z - FLOOR_OFFSET,
		0.0, 0.0, r,
		FLOOR_OFFSET, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), 1);

	return itemid;

}

stock RemoveItemFromWorld(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERNAL:ITEM_DEBUG("[RemoveItemFromWorld]");
	if(!Iter_Contains(itm_Index, _:itemid))
		return 0;

	if(itm_Holder[itemid] != INVALID_PLAYER_ID)
	{
		CallLocalFunction("OnItemRemovedFromPlayer", "dd", itm_Holder[itemid], itemid);
		RemovePlayerAttachedObject(itm_Holder[itemid], ITM_ATTACH_INDEX);
		itm_Holding[itm_Holder[itemid]] = INVALID_ITEM_ID;
		itm_Interacting[itm_Holder[itemid]] = INVALID_ITEM_ID;
		itm_Holder[itemid] = INVALID_PLAYER_ID;
		itm_Interactor[itemid] = INVALID_PLAYER_ID;
	}
	else
	{
		if(!Iter_Contains(itm_WorldIndex, _:itemid))
			return 0;

		DestroyDynamicObject(itm_Data[itemid][itm_objId]);
		DestroyButton(itm_Data[itemid][itm_button]);
		itm_ButtonIndex[itm_Data[itemid][itm_button]] = INVALID_BUTTON_ID;

		itm_Data[itemid][itm_objId] = -1;
		itm_Data[itemid][itm_button] = INVALID_BUTTON_ID;
	}

	Iter_Remove(itm_WorldIndex, itemid);

	return 1;
}


/*==============================================================================

	Internal Functions and Hooks

==============================================================================*/


CreateItemInWorld(itemid,
	Float:x = 0.0, Float:y = 0.0, Float:z = 0.0,
	Float:rx = 1000.0, Float:ry = 1000.0, Float:rz = 1000.0,
	Float:zoffset = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1)
{
	sif_d:SIF_DEBUG_LEVEL_INTERNAL:ITEM_DEBUG("[CreateItemInWorld]");
	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	if(Iter_Contains(itm_WorldIndex, itemid))
		return -1;

	new ItemType:itemtype = itm_Data[itemid][itm_type];

	if(!IsValidItemType(itemtype))
		return -2;

	itm_Data[itemid][itm_posX]					= x;
	itm_Data[itemid][itm_posY]					= y;
	itm_Data[itemid][itm_posZ]					= z;
	itm_Data[itemid][itm_rotX]					= rx;
	itm_Data[itemid][itm_rotY]					= ry;
	itm_Data[itemid][itm_rotZ]					= rz;
	itm_Data[itemid][itm_world]					= world;
	itm_Data[itemid][itm_interior]				= interior;

	if(itm_Holder[itemid] != INVALID_PLAYER_ID)
	{
		RemovePlayerAttachedObject(itm_Holder[itemid], ITM_ATTACH_INDEX);
		SetPlayerSpecialAction(itm_Holder[itemid], SPECIAL_ACTION_NONE);

		itm_Holding[itm_Holder[itemid]]			= INVALID_ITEM_ID;
		itm_Interacting[itm_Holder[itemid]]		= INVALID_ITEM_ID;
	}

	itm_Interactor[itemid]						= INVALID_PLAYER_ID;
	itm_Holder[itemid]							= INVALID_PLAYER_ID;

	if(applyrotoffsets)
	{
		itm_Data[itemid][itm_objId]				= CreateDynamicObject(itm_TypeData[itemtype][itm_model],
			x, y, z + itm_TypeData[itemtype][itm_offsetZ],
			(rx == 1000.0) ? (itm_TypeData[itemtype][itm_defaultRotX]) : (rx + itm_TypeData[itemtype][itm_defaultRotX]),
			(ry == 1000.0) ? (itm_TypeData[itemtype][itm_defaultRotY]) : (ry + itm_TypeData[itemtype][itm_defaultRotY]),
			(rz == 1000.0) ? (itm_TypeData[itemtype][itm_defaultRotZ]) : (rz + itm_TypeData[itemtype][itm_defaultRotZ]),
			world, interior, .streamdistance = 100.0);
	}
	else
	{
		itm_Data[itemid][itm_objId]				= CreateDynamicObject(itm_TypeData[itemtype][itm_model],
			x, y, z + itm_TypeData[itemtype][itm_offsetZ],
			(rx == 1000.0) ? (itm_TypeData[itemtype][itm_defaultRotX]) : (rx),
			(ry == 1000.0) ? (itm_TypeData[itemtype][itm_defaultRotY]) : (ry),
			(rz == 1000.0) ? (itm_TypeData[itemtype][itm_defaultRotZ]) : (rz),
			world, interior, .streamdistance = 100.0);
	}


	itm_Data[itemid][itm_button]				= CreateButton(x, y, z + zoffset, "Press F to pick up", world, interior, 1.0);

	if(itm_Data[itemid][itm_button] == INVALID_BUTTON_ID)
	{
		printf("ERROR: Invalid button ID created for item %d.", itemid);
		return -3;
	}

	itm_ButtonIndex[itm_Data[itemid][itm_button]] = itemid;

	if(itm_TypeData[itemtype][itm_colour] != -1)
		SetDynamicObjectMaterial(itm_Data[itemid][itm_objId], 0, itm_TypeData[itemtype][itm_model], "invalid", "invalid", itm_TypeData[itemtype][itm_colour]);

	if(label)
		SetButtonLabel(itm_Data[itemid][itm_button], itm_TypeData[itemtype][itm_name], .range = 2.0);

	Iter_Add(itm_WorldIndex, itemid);

	CallLocalFunction("OnItemCreateInWorld", "d", itemid);

	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	sif_d:SIF_DEBUG_LEVEL_CALLBACKS:ITEM_DEBUG("[OnPlayerKeyStateChange]");
	if(IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
		return 1;

	if(newkeys & KEY_NO)
	{
		_PlayerKeyHandle_Drop(playerid);
	}
	if(newkeys & 16)
	{
		_PlayerKeyHandle_Use(playerid);
	}
	return 1;
}

_PlayerKeyHandle_Drop(playerid)
{
	new animidx = GetPlayerAnimationIndex(playerid);

	if(!sif_IsIdleAnim(animidx))
		return 0;

	if(itm_Interacting[playerid] != INVALID_ITEM_ID)
		return -1;

	if(!Iter_Contains(itm_Index, itm_Holding[playerid]))
		return -2;

	// TODO: Replace this bit with some more abstracted code
	// And improve near-player checks to use button style "near"-indexing.
	foreach(new i : Player)
	{
		if(i == playerid)
			continue;

		if(itm_Holding[i] != INVALID_ITEM_ID)
			continue;

		if(itm_Interacting[i] != INVALID_ITEM_ID)
			continue;

		if(IsPlayerInAnyVehicle(i))
			continue;

		if(IsPlayerInDynamicArea(playerid, gPlayerArea[i]))
		{
			if(PlayerGiveItem(playerid, i, 1))
				return 1;
		}
	}

	PlayerDropItem(playerid);

	return 1;
}

_PlayerKeyHandle_Use(playerid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERNAL:ITEM_DEBUG("[_PlayerKeyHandle_Use]");

	new animidx = GetPlayerAnimationIndex(playerid);

	if(!sif_IsIdleAnim(animidx))
		return 0;

	if(itm_Interacting[playerid] != INVALID_ITEM_ID)
		return 0;

	if(!Iter_Contains(itm_Index, itm_Holding[playerid]))
		return 0;

	return PlayerUseItem(playerid);
}


public OnPlayerEnterPlayerArea(playerid, targetid)
{
	sif_dp:SIF_DEBUG_LEVEL_CALLBACKS:ITEM_DEBUG("[OnPlayerEnterPlayerArea]")<playerid>;
	if(Iter_Contains(itm_Index, itm_Holding[playerid]))
	{
		ShowActionText(playerid, "Press N to give item");
	}

	#if defined itm_OnPlayerEnterPlayerArea
		return itm_OnPlayerEnterPlayerArea(playerid, targetid);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerEnterPlayerArea
	#undef OnPlayerEnterPlayerArea
#else
	#define _ALS_OnPlayerEnterPlayerArea
#endif
#define OnPlayerEnterPlayerArea itm_OnPlayerEnterPlayerArea
#if defined itm_OnPlayerEnterPlayerArea
	forward itm_OnPlayerEnterPlayerArea(playerid, targetid);
#endif

public OnPlayerLeavePlayerArea(playerid, targetid)
{
	sif_dp:SIF_DEBUG_LEVEL_CALLBACKS:ITEM_DEBUG("[OnPlayerLeavePlayerArea]")<playerid>;
	if(Iter_Contains(itm_Index, itm_Holding[playerid]))
	{
		HideActionText(playerid);
	}

	#if defined itm_OnPlayerLeavePlayerArea
		return itm_OnPlayerLeavePlayerArea(playerid, targetid);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerLeavePlayerArea
	#undef OnPlayerLeavePlayerArea
#else
	#define _ALS_OnPlayerLeavePlayerArea
#endif
#define OnPlayerLeavePlayerArea itm_OnPlayerLeavePlayerArea
#if defined itm_OnPlayerLeavePlayerArea
	forward itm_OnPlayerLeavePlayerArea(playerid, targetid);
#endif

internal_OnPlayerUseItem(playerid, itemid)
{
	sif_dp:SIF_DEBUG_LEVEL_INTERNAL:ITEM_DEBUG("[internal_OnPlayerUseItem]")<playerid>;
	new buttonid = GetPlayerButtonID(playerid);

	if(buttonid != -1)
		return CallLocalFunction("OnPlayerUseItemWithButton", "ddd", playerid, buttonid, itm_Holding[playerid]);

	return CallLocalFunction("OnPlayerUseItem", "dd", playerid, itemid);
}


public OnButtonPress(playerid, buttonid)
{
	sif_dp:SIF_DEBUG_LEVEL_CALLBACKS:ITEM_DEBUG("[OnButtonPress]")<playerid>;

	if(_OnButtonPressHandler(playerid, buttonid))
		return 1;

	#if defined itm_OnButtonPress
		return itm_OnButtonPress(playerid, buttonid);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnButtonPress
	#undef OnButtonPress
#else
	#define _ALS_OnButtonPress
#endif
#define OnButtonPress itm_OnButtonPress
#if defined itm_OnButtonPress
	forward itm_OnButtonPress(playerid, buttonid);
#endif

_OnButtonPressHandler(playerid, buttonid)
{
	if(itm_Interacting[playerid] != INVALID_ITEM_ID)
		return 0;

	if(itm_ButtonIndex[buttonid] == INVALID_ITEM_ID)
		return 0;

	if(!Iter_Contains(itm_Index, itm_ButtonIndex[buttonid]))
		return 0;

	new itemid = itm_ButtonIndex[buttonid];

	if(itm_Holder[itemid] != INVALID_PLAYER_ID)
		return 0;

	if(itm_Interactor[itemid] != INVALID_PLAYER_ID)
		return 0;

	if(Iter_Contains(itm_Index, itm_Holding[playerid]))
		return CallLocalFunction("OnPlayerUseItemWithItem", "ddd", playerid, itm_Holding[playerid], itemid);

	if(CallLocalFunction("OnPlayerPickUpItem", "dd", playerid, itemid))
		return 1;

	PlayerPickUpItem(playerid, itemid);

	return 1;
}

timer PickUpItemDelay[400](playerid, id, animtype)
{
	sif_dp:SIF_DEBUG_LEVEL_INTERNAL:ITEM_DEBUG("[PickUpItemDelay]")<playerid>;
	if(animtype == 0)
		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.0, 0, 0, 0, 0, 0);

	HideActionText(playerid);
	
	itm_Interacting[playerid] = INVALID_ITEM_ID;

	if(CallLocalFunction("OnPlayerPickedUpItem", "dd", playerid, id))
		return 1;

	GiveWorldItemToPlayer(playerid, id, 1);
	
	return 1;
}

timer DropItemDelay[400](playerid)
{
	sif_dp:SIF_DEBUG_LEVEL_INTERNAL:ITEM_DEBUG("[DropItemDelay]")<playerid>;
	new
		itemid = itm_Holding[playerid],
		Float:x,
		Float:y,
		Float:z,
		Float:r;

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	RemovePlayerAttachedObject(playerid, ITM_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	itm_Holding[playerid] = INVALID_ITEM_ID;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	itm_Holder[itemid] = INVALID_PLAYER_ID;
	itm_Interactor[itemid] = INVALID_PLAYER_ID;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, r);

	CreateItemInWorld(itemid,
		x + (0.5 * floatsin(-r, degrees)),
		y + (0.5 * floatcos(-r, degrees)),
		z - FLOOR_OFFSET,
		0.0, 0.0, r,
		ITEM_BUTTON_OFFSET, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), 1);

	ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.0, 0, 0, 0, 0, 0);

	Streamer_Update(playerid);

	CallLocalFunction("OnPlayerDroppedItem", "dd", playerid, itemid);

	return 1;
}

timer GiveItemDelay[500](playerid, targetid)
{
	sif_dp:SIF_DEBUG_LEVEL_INTERNAL:ITEM_DEBUG("[GiveItemDelay]")<playerid>;
	if(Iter_Contains(itm_Index, itm_Holding[targetid]))
		return;

	if(!IsPlayerConnected(targetid)) // In case the 'receiver' quits within the 500ms time window.
		return;

	new
		id,
		ItemType:type;

	id = itm_Holding[playerid];

	if(id == -1)
		return;

	type = itm_Data[id][itm_type];

	itm_Holding[playerid] = INVALID_ITEM_ID;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	itm_Interacting[targetid] = INVALID_ITEM_ID;
	RemovePlayerAttachedObject(playerid, ITM_ATTACH_INDEX);

	SetPlayerAttachedObject(
		targetid, ITM_ATTACH_INDEX, itm_TypeData[type][itm_model], itm_TypeData[type][itm_attachBone],
		itm_TypeData[type][itm_attachPosX], itm_TypeData[type][itm_attachPosY], itm_TypeData[type][itm_attachPosZ],
		itm_TypeData[type][itm_attachRotX], itm_TypeData[type][itm_attachRotY], itm_TypeData[type][itm_attachRotZ],
		.materialcolor1 = itm_TypeData[type][itm_colour], .materialcolor2 = itm_TypeData[type][itm_colour]);

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	itm_Holding[targetid] = id;
	itm_Holder[id] = targetid;

	CallLocalFunction("OnPlayerGivenItem", "ddd", playerid, targetid, id);
	CallLocalFunction("OnItemRemovedFromPlayer", "dd", playerid, id);

	return;
}

#if defined ITM_DROP_ON_DEATH

public OnPlayerDeath(playerid, killerid, reason)
{
	sif_dp:SIF_DEBUG_LEVEL_CALLBACKS:ITEM_DEBUG("[OnPlayerDeath]")<playerid>;
	new itemid = itm_Holding[playerid];
	if(Iter_Contains(itm_Index, itemid))
	{
		new
			Float:x,
			Float:y,
			Float:z,
			Float:r;

		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, r);

		RemovePlayerAttachedObject(playerid, ITM_ATTACH_INDEX);
		CreateItemInWorld(itemid,
			x + (0.5 * floatsin(-r, degrees)),
			y + (0.5 * floatcos(-r, degrees)),
			z - FLOOR_OFFSET,
			0.0, 0.0, r,
			FLOOR_OFFSET, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), 1);

		CallLocalFunction("OnPlayerDropItem", "dd", playerid, itemid);
	}

	#if defined itm_OnPlayerDeath
		return itm_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath itm_OnPlayerDeath
#if defined itm_OnPlayerDeath
	forward itm_OnPlayerDeath(playerid, killerid, reason);
#endif

#endif

#if defined DEBUG_LABELS_BUTTON
	UpdateItemDebugLabel(buttonid)
	{
		new string[64];

		format(string, sizeof(string), "EXDATA: %d", itm_Data[buttonid][itm_exData]);

		UpdateDebugLabelString(itm_DebugLabelID[buttonid], string);
	}
#endif


/*==============================================================================

	Interface Functions

==============================================================================*/


stock IsValidItem(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[IsValidItem]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	return 1;
}

// itm_objId
stock GetItemObjectID(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemObjectID]");

	if(!Iter_Contains(itm_Index, itemid))
		return INVALID_OBJECT_ID;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return INVALID_OBJECT_ID;

	return itm_Data[itemid][itm_objId];
}

// itm_button
stock GetItemButtonID(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemButtonID]");

	if(!Iter_Contains(itm_Index, itemid))
		return INVALID_BUTTON_ID;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return INVALID_BUTTON_ID;

	return itm_Data[itemid][itm_button];
}
stock SetItemLabel(itemid, text[], colour = 0xFFFF00FF, Float:range = 10.0)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[SetItemLabel]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	SetButtonLabel(itm_Data[itemid][itm_button], text, colour, range);

	return 1;
}

// itm_type
stock ItemType:GetItemType(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemType]");

	if(!Iter_Contains(itm_Index, itemid))
		return INVALID_ITEM_TYPE;

	return itm_Data[itemid][itm_type];
}

// itm_posX
// itm_posY
// itm_posZ
stock GetItemPos(itemid, &Float:x, &Float:y, &Float:z)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemPos]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	x = itm_Data[itemid][itm_posX];
	y = itm_Data[itemid][itm_posY];
	z = itm_Data[itemid][itm_posZ];

	return 1;
}
stock SetItemPos(itemid, Float:x, Float:y, Float:z, Float:zoffset = 0.0)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[SetItemPos]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	itm_Data[itemid][itm_posX] = x;
	itm_Data[itemid][itm_posY] = y;
	itm_Data[itemid][itm_posZ] = z;

	SetButtonPos(itm_Data[itemid][itm_button], x, y, z + zoffset);
	SetDynamicObjectPos(itm_Data[itemid][itm_objId], x, y, z);

	return 1;
}
stock GetItemRot(itemid, &Float:rx, &Float:ry, &Float:rz)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemRot]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	rx = itm_Data[itemid][itm_rotX];
	ry = itm_Data[itemid][itm_rotY];
	rz = itm_Data[itemid][itm_rotZ];

	return 1;
}
stock SetItemRot(itemid, Float:rx, Float:ry, Float:rz, bool:offsetfromdefaults = false)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[SetItemRot]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return 0;

	if(offsetfromdefaults)
	{
		SetDynamicObjectRot(itm_Data[itemid][itm_objId],
			itm_TypeData[itm_Data[itemid][itm_type]][itm_defaultRotX] + rx,
			itm_TypeData[itm_Data[itemid][itm_type]][itm_defaultRotY] + ry,
			itm_TypeData[itm_Data[itemid][itm_type]][itm_defaultRotZ] + rz);

		itm_Data[itemid][itm_rotX] = itm_TypeData[itm_Data[itemid][itm_type]][itm_defaultRotX] + rx;
		itm_Data[itemid][itm_rotY] = itm_TypeData[itm_Data[itemid][itm_type]][itm_defaultRotY] + ry;
		itm_Data[itemid][itm_rotZ] = itm_TypeData[itm_Data[itemid][itm_type]][itm_defaultRotZ] + rz;
	}
	else
	{
		SetDynamicObjectRot(itm_Data[itemid][itm_objId], rx, ry, rz);

		itm_Data[itemid][itm_rotX] = rx;
		itm_Data[itemid][itm_rotY] = ry;
		itm_Data[itemid][itm_rotZ] = rz;
	}

	return 1;	
}

// itm_world
stock SetItemWorld(itemid, world)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[SetItemWorld]");

	if(!Iter_Contains(itm_Index, itemid))
		return -1;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return -1;

	SetButtonWorld(itm_Data[itemid][itm_button], world);
	itm_Data[itemid][itm_world] = world;

	return 1;
}
stock GetItemWorld(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemWorld]");

	if(!Iter_Contains(itm_Index, itemid))
		return -1;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return -1;

	return itm_Data[itemid][itm_world];
}

// itm_interior
stock SetItemInterior(itemid, interior)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[SetItemInterior]");

	if(!Iter_Contains(itm_Index, itemid))
		return -1;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return -1;

	SetButtonInterior(itm_Data[itemid][itm_button], interior);
	itm_Data[itemid][itm_interior] = interior;

	return 1;
}
stock GetItemInterior(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemInterior]");

	if(!Iter_Contains(itm_Index, itemid))
		return -1;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return -1;

	return itm_Data[itemid][itm_interior];
}

// itm_exData
stock SetItemExtraData(itemid, data)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[SetItemExtraData]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	itm_Data[itemid][itm_exData] = data;

	return 1;
}
stock GetItemExtraData(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemExtraData]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	return itm_Data[itemid][itm_exData];
}

// itm_nameEx
stock SetItemNameExtra(itemid, string[])
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[SetItemNameExtra]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	itm_Data[itemid][itm_nameEx][0] = EOS;
	strcat(itm_Data[itemid][itm_nameEx], string, ITM_MAX_TEXT);

	return 1;
}

stock GetItemNameExtra(itemid, string[])
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemNameExtra]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	string[0] = EOS;
	strcat(string, itm_Data[itemid][itm_nameEx], ITM_MAX_TEXT);

	return 1;
}

// itm_used
stock IsValidItemType(ItemType:itemtype)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[IsValidItemType]");

	if(ItemType:0 <= itemtype < ITM_MAX_TYPES)
		return itm_TypeData[itemtype][itm_used];

	return false;
}

// itm_name
stock GetItemTypeName(ItemType:itemtype, string[])
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemTypeName]");

	if(!IsValidItemType(itemtype))
		return 0;
	
	string[0] = EOS;
	strcat(string, itm_TypeData[itemtype][itm_name], ITM_MAX_NAME);
	
	return 1;
}

// itm_model
stock GetItemTypeModel(ItemType:itemtype)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemTypeModel]");

	if(!IsValidItemType(itemtype))
		return 0;

	return itm_TypeData[itemtype][itm_model];
}

// itm_size
stock GetItemTypeSize(ItemType:itemtype)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemTypeSize]");

	if(!IsValidItemType(itemtype))
		return INVALID_ITEM_SIZE;

	return itm_TypeData[itemtype][itm_size];
}

// itm_colour
stock GetItemTypeColour(ItemType:itemtype)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemTypeColour]");

	if(!IsValidItemType(itemtype))
		return INVALID_ITEM_SIZE;

	return itm_TypeData[itemtype][itm_colour];
}

// itm_attachBone
stock GetItemTypeBone(ItemType:itemtype)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemTypeBone]");

	if(!IsValidItemType(itemtype))
		return INVALID_ITEM_SIZE;

	return itm_TypeData[itemtype][itm_attachBone];
}

// itm_Holder
stock GetItemHolder(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemHolder]");

	if(!Iter_Contains(itm_Index, itemid))
		return INVALID_PLAYER_ID;

	return itm_Holder[itemid];
}

// itm_Holding
stock GetPlayerItem(playerid)
{
	sif_dp:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetPlayerItem]")<playerid>;

	if(!Iter_Contains(itm_Index, itm_Holding[playerid]))
		return INVALID_ITEM_ID;

	if(!(0 <= playerid < MAX_PLAYERS))
		return INVALID_ITEM_ID;

	return itm_Holding[playerid];
}

stock IsItemInWorld(itemid)
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[IsItemInWorld]");

	if(!Iter_Contains(itm_Index, itemid))
		return -1;

	if(!Iter_Contains(itm_WorldIndex, itemid))
		return 0;

	return 1;
}

stock GetItemName(itemid, string[])
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetItemName]");

	if(!Iter_Contains(itm_Index, itemid))
		return 0;

	string[0] = EOS;
	strcat(string, itm_TypeData[itm_Data[itemid][itm_type]][itm_name], ITM_MAX_NAME);

	CallLocalFunction("OnItemNameRender", "d", itemid);

	if(!isnull(itm_Data[itemid][itm_nameEx]))
	{
		strcat(string, " (", ITM_MAX_TEXT);
		strcat(string, itm_Data[itemid][itm_nameEx], ITM_MAX_TEXT);
		strcat(string, ")", ITM_MAX_TEXT);
	}

	return 1;
}

stock GetPlayerInteractingItem(playerid)
{
	sif_dp:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetPlayerInteractingItem]")<playerid>;

	if(!IsPlayerConnected(playerid))
		return INVALID_ITEM_ID;

	return itm_Interacting[playerid];
}

stock GetNextItemID()
{
	sif_d:SIF_DEBUG_LEVEL_INTERFACE:ITEM_DEBUG("[GetNextItemID]");

	return Iter_Free(itm_Index);
}
