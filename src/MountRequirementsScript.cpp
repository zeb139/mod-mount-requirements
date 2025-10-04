/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
*/

#include "MountRequirements.h"
#include "ScriptMgr.h"
#include "Chat.h"

class MountRequirements_WorldScript : public WorldScript
{
    public:
        MountRequirements_WorldScript() : WorldScript("MountRequirements_WorldScript") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        LOG_INFO("server.loading", "\nMountRequirements: Applying MountRequirements Updates...");
        MountRequirements::instance().InitializeConfiguration();
        MountRequirements::instance().UpdateMountRequirements();
    }
};

class MountRequirements_PlayerScript : public PlayerScript
{
    public:
        MountRequirements_PlayerScript() : PlayerScript("MountRequirements_PlayerScript") { }

    void OnPlayerLogin(Player* player) override
    {
        if (!sConfigMgr->GetOption<bool>("MountRequirements.Enable", true))
            return;
        
        if(sConfigMgr->GetOption<bool>("MountRequirements.LoginMessage.ModuleAnnounce.Enable", true))
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Mount Requirements |rmodule.");

        if(sConfigMgr->GetOption<bool>("MountRequirements.LoginMessage.CacheReminder.Enable", true))
        {
            std::string msg = "|cff00ccffNote: If you see outdated tooltip data for mounts, or can't use them " 
                              "despite meeting level requirements, please delete your game's Cache folder.|r";
            ChatHandler(player->GetSession()).SendSysMessage(msg);
        }
    }
};

void AddMountRequirementsScripts()
{
    new MountRequirements_WorldScript();
    new MountRequirements_PlayerScript();
}
