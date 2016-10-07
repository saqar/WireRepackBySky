#ifndef SC_PRECOMPILED_H
#define SC_PRECOMPILED_H

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Pet.h"
#include "Creature.h"
#ifdef _WIN32
#include <windows.h>
#endif

#endif


class PRESTIGE_NPC : public CreatureScript
{
public:
	PRESTIGE_NPC() : CreatureScript("PRESTIGE_NPC") { }

	bool OnGossipHello(Player* plr, Creature* npc)
	{
		if (plr->IsInCombat())
		{
			plr->GetSession()->SendNotification("You're in combat!");
			return false;
		}

		AddGossipItemFor(plr, 2, "Exchange level 80 to 100k gold", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		AddGossipItemFor(plr, 9, "Nevermind..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);
		SendGossipMenuFor(plr, 1, npc->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* plr, Creature* npc, uint32 sender, uint32 uiAction)
	{
		if (!plr)
			return false;

		ClearGossipMenuFor(plr);

		switch (uiAction)
		{

		case GOSSIP_ACTION_INFO_DEF + 1:
			if (plr->getLevel() != 80)
				plr->GetSession()->SendNotification("You need to be level 80");
			else
			{

				plr->_ApplyAllLevelScaleItemMods(false);
				if (plr->getClass() == CLASS_DEATH_KNIGHT)
				{
					plr->SetLevel(55);
				}
				else
				{
					plr->SetLevel(1);
				}
				plr->ModifyMoney(1000000000);
				for (uint8 slot = SLOT_HEAD; slot < SLOT_EMPTY; slot++)
				{
					if (plr->CanUseItem(plr->GetItemByPos(NULL_BAG, slot)) != EQUIP_ERR_OK)
						plr->SetVisibleItemSlot(slot, NULL);
				}
				plr->InitRunes();
				plr->InitStatsForLevel(true);
				plr->InitTaxiNodesForLevel();
				plr->InitGlyphsForLevel();
				plr->InitTalentForLevel();
				plr->SetUInt32Value(PLAYER_XP, 0);
				plr->ResetSpells(true);

				plr->_ApplyAllLevelScaleItemMods(true);
				plr->GetSession()->LogoutPlayer(true);
			}
			break;
		case GOSSIP_ACTION_INFO_DEF + 99: // Close Function
			CloseGossipMenuFor(plr);
			break;
		}
		return true;
	}
};
void AddSC_PRESTIGE_NPC()
{
	new PRESTIGE_NPC();
}