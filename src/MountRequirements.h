#ifndef MOUNT_REQUIREMENTS_H
#define MOUNT_REQUIREMENTS_H

#include "OriginalMountRequirements.h"
#include "Common.h"
#include "Config.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "ItemTemplate.h"
#include "SharedDefines.h"
#include "SpellMgr.h"

class MountRequirements
{
    private:
        bool debug_Out;
        bool MountRequirementsEnabled;

        // Riding Skills
        uint32 ApprenticeRidingSkillBuyPrice;
        uint32 ApprenticeRidingSkillRequiredLevel;

        uint32 JourneymanRidingSkillBuyPrice;
        uint32 JourneymanRidingSkillRequiredLevel;

        uint32 ExpertRidingSkillBuyPrice;
        uint32 ExpertRidingSkillRequiredLevel;

        uint32 ArtisanRidingSkillBuyPrice;
        uint32 ArtisanRidingSkillRequiredLevel;

        uint32 ColdWeatherFlyingSkillBuyPrice;
        uint32 ColdWeatherFlyingSkillRequiredLevel;

        // Mounts
        uint32 ApprenticeRacialMountsBuyPrice;
        uint32 ApprenticeRacialMountsSellPrice;
        uint32 ApprenticeRacialMountsRequiredLevel;

        uint32 JourneymanRacialMountsBuyPrice;
        uint32 JourneymanRacialMountsSellPrice;
        uint32 JourneymanRacialMountsRequiredLevel;

        uint32 ExpertFactionMountsBuyPrice;
        uint32 ExpertFactionMountsSellPrice;
        uint32 ExpertFactionMountsRequiredLevel;
        
        uint32 ArtisanFactionMountsBuyPrice;
        uint32 ArtisanFactionMountsSellPrice;
        uint32 ArtisanFactionMountsRequiredLevel;

        uint32 TomeOfColdWeatherFlightBuyPrice;
        uint32 TomeOfColdWeatherFlightSellPrice;
        uint32 TomeOfColdWeatherFlightRequiredLevel;
    
        // std::vector<MountBackup> MountBackups;
        MountRequirements();

    public:
        static MountRequirements* instance()
        {
            static MountRequirements instance;
            return &instance;
        }

        ~MountRequirements();
        void InitializeConfiguration();
        void UpdateMountRequirements();
        void ApplyCustomMountRequirements();
        void RestoreOriginalMountRequirements();

        std::string BuildItemUpdateQuery(const std::vector<uint32> ids, const uint32 buy, const uint32 sell, const uint32 level);
        std::string BuildSpellUpdateQuery(const uint32 id, const uint32 buy, const uint32 level);
        std::string VectorToCSV(const std::vector<uint32>& v);
};

#define mountRequirements MountRequirements::instance()

#endif