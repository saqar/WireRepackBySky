//CastleDEV
#include "ScriptPCH.h"

class announce_CurrentPlayer : public PlayerScript
{
public:
	announce_CurrentPlayer() : PlayerScript("announce_login") {}

	void OnLogin(Player* player, bool /*loginFirst*/)
	{
		std::string msg;

		if (player->GetTotalPlayedTime() > 5)
		{
			msg += "|cff54b5ffWelcome Back |cffFF7D0A";
			msg += player->GetName();
			msg += "|cff54b5ff|r!";
			sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), 0);
			return;
		}
	}

};

void AddSC_announce_CurrentPlayer()
{
	new announce_CurrentPlayer;
}