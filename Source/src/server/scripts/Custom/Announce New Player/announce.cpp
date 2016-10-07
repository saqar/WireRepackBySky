//CastleDEV
#include "ScriptPCH.h"

class announce_NewPlayer : public PlayerScript
{
public:
	announce_NewPlayer() : PlayerScript("announce_login") {}

	void OnLogin(Player* player, bool /*loginFirst*/)
	{
		std::string msg;

		if (player->GetTotalPlayedTime() < 2)
		{
			msg += "|cff54b5ffWelcome To Our New Player |cffFFF569";
			msg += player->GetName();
			msg += "|cff54b5ff|r!";
			sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), 0);
			player->CastSpell(player, 55420);
			return;
		}
	}

};

void AddSC_announce_NewPlayer()
{
	new announce_NewPlayer;
}