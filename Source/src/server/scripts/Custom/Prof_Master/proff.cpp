#include "ScriptPCH.h"
#include "Language.h"
#include "Config.h"

bool   ChargeProfession;
int    ChargeProfessionHONOR;
int	   IdEmblem;
float  pValue;
bool   pProfessionVIP;
uint32 nProfessionVIP;
uint32 qProfessionVIP;
uint32 qProfessionNormal;
uint32 IdItemProfessionVIP;

using namespace std;
struct ProfessionMenu {
	uint8 Menu, Submenu, Icon;
	uint32 ProfValue, pSKILL;
	string Name, Confirm;
};

ProfessionMenu mpData [] = {
	{ 1, 2, 3, 0, 0, "|cff1C1C1C|TInterface/ICONS/achievement_general:30:30:-16:0|t[Primary Professions]|r" },
	{ 1, 3, 3, 0, 0, "|cff90EE90|TInterface/ICONS/achievement_general:30:30:-16:0|t[Secondary Professions]|r" },
	// Primary Professions
	{ 2, 0, 3, pValue, 171, "|TInterface\\icons\\trade_alchemy:30:30:-16:0|t Alchemy.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_ALCHEMY 171
	{ 2, 0, 3, pValue, 164,"|TInterface\\icons\\INV_Ingot_05:30:30:-16:0|t Blacksmithing.", "|cff1E90FFAre you sure you want that profession?|r" }, // SKILL_BLACKSMITHING 164
	{ 2, 0, 3, pValue, 165,"|TInterface\\icons\\INV_Misc_LeatherScrap_02:30:30:-16:0|t Leatherworking.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_LEATHERWORKING 165
	{ 2, 0, 3, pValue, 197,"|TInterface\\icons\\INV_Fabric_Felcloth_Ebon:30:30:-16:0|t Tailoring.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_TAILORING 197
	{ 2, 0, 3, pValue, 202,"|TInterface\\icons\\inv_misc_wrench_01:30:30:-16:0|t Engineering.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_ENGINEERING 202
	{ 2, 0, 3, pValue, 333,"|TInterface\\icons\\trade_engraving:30:30:-16:0|t Enchanting.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_ENCHANTING 333
	{ 2, 0, 3, pValue, 755,"|TInterface\\icons\\inv_misc_gem_01:30:30:-16:0|t Jewelcrafting.", "|cff1E90FFAre you sure you want that profession?|r" }, // SKILL_JEWELCRAFTING 755
	{ 2, 0, 3, pValue, 773,"|TInterface\\icons\\INV_Scroll_08:30:30:-16:0|t Inscription.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_INSCRIPTION 773
	{ 2, 0, 3, pValue, 182, "|TInterface\\icons\\INV_Misc_Herb_07:30:30:-16:0|t Herbalism.", "|cff1E90FFAre you sure you want that profession?|r" }, // SKILL_HERBALISM 182
	{ 2, 0, 3, pValue, 186, "|TInterface\\icons\\trade_mining:30:30:-16:0|t Mining.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_MINING 186
	{ 2, 0, 3, pValue, 393, "|TInterface\\icons\\inv_misc_pelt_wolf_01:30:30:-16:0|t Skinning.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_SKINNING 393
	{ 2, 1, 3, 0, 0, "|cff0000FF|TInterface\\icons\\inv_misc_questionmark:20:20:-16:0|t<< Back" },
	// Secondary Professions		
	{ 3, 0, 3, pValue, 185, "|TInterface/ICONS/INV_Misc_Food_15:30:30:-16:0|t Cooking.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_COOKING 185
	{ 3, 0, 3, pValue, 129, "|TInterface/ICONS/spell_holy_sealofsacrifice:30:30:-16:0|t First Aid.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_FIRST_AID 129
	{ 3, 0, 3, pValue, 356, "|TInterface/ICONS/Trade_Fishing:30:30:-16:0|t Fishing.", "|cff1E90FFAre you sure you want that profession?|r" }, //SKILL_FISHING 356
	{ 3, 1, 3, 0, 0, "|cff0000FF|TInterface\\icons\\inv_misc_questionmark:20:20:-16:0|t<< Back" }
};

class Professions_NPC : public CreatureScript {
public:
	Professions_NPC() : CreatureScript("npc_Professions") {}

	void GetProfessionMenu(Player* player, Creature* creature, uint32 menuId) {
		player->PlayerTalkClass->ClearMenus();
		std::ostringstream Bs;
		bool epMENU = false;
		for (uint8 i = 0; i < (sizeof(mpData) / sizeof(*mpData)); i++)
		{
			if (epMENU && mpData[i].Menu != menuId)
				break;
			if (mpData[i].Menu == menuId )
			{
				if (mpData[i].Submenu != 0)
					AddGossipItemFor(player, mpData[i].Icon, mpData[i].Name, GOSSIP_SENDER_MAIN, i);
				else
					AddGossipItemFor(player, mpData[i].Icon, mpData[i].Name, GOSSIP_SENDER_MAIN, i, mpData[i].Confirm, 0, false);
				
				epMENU = true;
			}
		}
		SendGossipMenuFor(player, 907, creature->GetGUID());
		return;
	}
	bool OnGossipHello(Player *pPlayer, Creature* _creature) {
		GetProfessionMenu(pPlayer, _creature, 1);
		return true;
	}
	bool OnGossipSelect(Player* pPlayer, Creature* _creature, uint32 uiSender, uint32 SKILL) {
		pPlayer->PlayerTalkClass->ClearMenus();
		uint8 ListaMenu;
		switch (SKILL)
		{
		case -1:
			break;
		default:
			ListaMenu = mpData[SKILL].Submenu;
			break;
		}
		if (ListaMenu == 0)
		{
			if (pPlayer->HasSkill(mpData[SKILL].pSKILL)){
				pPlayer->GetSession()->SendNotification("|cffff0000ALERT: You already have this profession!");
			}else{
				CompleteLearnProfession(pPlayer, (SkillType)mpData[SKILL].pSKILL);
			}
			ListaMenu = mpData[SKILL].Menu;
		}
		GetProfessionMenu(pPlayer, _creature, ListaMenu);
		return true;
	}
	bool IsSecondarySkill(SkillType skill) const {
		return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
	}
	void CompleteLearnProfession(Player *pPlayer, SkillType skill) {
		if (PlayerAlreadyHasTwoProfessions(pPlayer) && !IsSecondarySkill(skill))
			pPlayer->GetSession()->SendNotification("|cffff0000ALERT: You already have the maxium professions allowed!");
		else
		{
			if (ChargeProfession == 1)
			{
				if (ChargeProfessionHONOR == 1){
					if (pPlayer->GetHonorPoints() < pValue)
					{
						pPlayer->GetSession()->SendNotification("|cffff0000ALERT: You don't have enough Honor Points!");
					}
					else
					{
						if (!LearnAllRecipesInProfession(pPlayer, skill))
							pPlayer->GetSession()->SendNotification("Internal error occured!");
						    pPlayer->ModifyHonorPoints(-pValue);
							pPlayer->GetSession()->SendNotification("|cff32CD32Congratulations!: Profession acquired with success!");
					}					
				}
				else if (ChargeProfessionHONOR == 2)
				{
					if (pPlayer->HasItemCount(IdEmblem, pValue))
					{
						if (!LearnAllRecipesInProfession(pPlayer, skill))
							pPlayer->GetSession()->SendNotification("Internal error occured!");
						pPlayer->DestroyItemCount(IdEmblem, pValue, true);
						pPlayer->GetSession()->SendNotification("|cff32CD32Congratulations!: Profession acquired with success!");
					}
					else
					{
						pPlayer->GetSession()->SendNotification("|cffff0000ALERT: You don't have enough Item[Token]!");
					}
				}
			}
			else
			{
				if (!LearnAllRecipesInProfession(pPlayer, skill))
					pPlayer->GetSession()->SendNotification("Internal error occured!");
				pPlayer->GetSession()->SendNotification("|cff32CD32Congratulations!: Profession acquired with success!");
			}
		}
	}
	bool LearnAllRecipesInProfession(Player* pPlayer, SkillType skill) {
		SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);

		if (!SkillInfo)
		{
			sLog->outCommand(LANG_SET_SKILL_ERROR, "Profession NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
			return false;
		}

		const char* skill_name = SkillInfo->name[ChatHandler(pPlayer->GetSession()).GetSessionDbcLocale()];

		LearnSkillRecipesHelper(pPlayer, SkillInfo->id);

		pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), 450, 450);
		ChatHandler(pPlayer->GetSession()).PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);
		return true;
	}
	bool PlayerAlreadyHasTwoProfessions(Player *pPlayer) {
		uint32 skillCount = 0;

		if (pPlayer->HasSkill(SKILL_MINING))
			skillCount++;

		if (pPlayer->HasSkill(SKILL_SKINNING))
			skillCount++;

		if (pPlayer->HasSkill(SKILL_HERBALISM))
			skillCount++;

		if (skillCount >= 4)
			return true;

		for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
		{
			SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
			if (!SkillInfo)	continue;

			if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)	continue;

			if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink) continue;

			const uint32 skillID = SkillInfo->id;
			if (pPlayer->HasSkill(skillID))
				skillCount++;

			if (pProfessionVIP == 1 && (pPlayer->HasItemCount(IdItemProfessionVIP, 1, true))){
				if (skillCount >= qProfessionVIP) // Number of the Profession of vip player
					return true;
			}
			else
			{
				if (skillCount >= qProfessionNormal) // Number of the Profession of normal player
					return true;
			}
		}
		return false;
	}
	void LearnSkillRecipesHelper(Player *player, uint32 skill_id) {
		uint32 classmask = player->getClassMask();

		for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
		{
			SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
			if (!skillLine)	continue;

			// wrong skill
			if (skillLine->skillId != skill_id)	continue;

			// not high rank
			if (skillLine->forward_spellid)	continue;

			// skip racial skills
			if (skillLine->racemask != 0) continue;

			// skip wrong class skills
			if (skillLine->classmask && (skillLine->classmask & classmask) == 0) continue;

			SpellInfo const * spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);

			if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
				continue;
			player->LearnSpell(skillLine->spellId, false);
		}
	}
};

class mod_npc_Professions_WorldScript : public WorldScript {
public:
	mod_npc_Professions_WorldScript() : WorldScript("mod_npcprofessi_WorldScript") { }

	void OnConfigLoad(bool /*reload*/)	{
		ChargeProfession       = sConfigMgr->GetBoolDefault("Enabre.Charge.Profession", true);
		ChargeProfessionHONOR  = sConfigMgr->GetIntDefault("Charge.Profession_Honor.or.Item", 0);
		IdEmblem			   = sConfigMgr->GetIntDefault("Id_Item", 0);
		pValue                 = sConfigMgr->GetIntDefault("Value_To_Charge", 0);
		pProfessionVIP         = sConfigMgr->GetBoolDefault("ProfessionVIP", false);
		qProfessionVIP         = sConfigMgr->GetIntDefault("QuantityProfessionVIP", 0);
		qProfessionNormal      = sConfigMgr->GetIntDefault("QuantityProfessionNormal", 0);
		IdItemProfessionVIP    = sConfigMgr->GetIntDefault("Id.ItemVIP", 0);
	}
};

void AddSC_Professions_NPC()
{
	new Professions_NPC();
	new mod_npc_Professions_WorldScript();
}