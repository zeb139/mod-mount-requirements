/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
*/

#include "MountRequirements.h"
#include "Log.h"
#include "WorldSession.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "Config.h"
#include "ItemTemplate.h"

class MountRequirements_WorldScript : public WorldScript
{
    public:
        MountRequirements_WorldScript() : WorldScript("MountRequirements_WorldScript") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        LOG_INFO("server.loading", "\nMountRequirements: Applying MountRequirements Updates...");
        mountRequirements->InitializeConfiguration();
        mountRequirements->UpdateMountRequirements();
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
            ChatHandler(player->GetSession()).PSendSysMessage("MountRequirements module is enabled.");

        if(sConfigMgr->GetOption<bool>("MountRequirements.LoginMessage.CacheReminder.Enable", true))
        {
            std::string msg = "Note: If you see outdated tooltip data for mounts, " 
                "or can't use them despite meeting level requirements, please delete your game's Cache folder.";
            ChatHandler(player->GetSession()).PSendSysMessage(msg);
        }
    }
};

void AddMountRequirementsScripts()
{
    new MountRequirements_WorldScript();
    new MountRequirements_PlayerScript();
}