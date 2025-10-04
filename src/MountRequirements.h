#ifndef MOUNT_REQUIREMENTS_H
#define MOUNT_REQUIREMENTS_H

#include "OriginalMountRequirements.h"

#include "DatabaseEnvFwd.h"
#include "Define.h"

#include <vector>
#include <unordered_map>
#include <string>

class MountRequirements
{
    private:
        bool debug_Out {false};
        bool MountRequirementsEnabled {true};

        // Riding Skills
        uint32 ApprenticeRidingSkillBuyPrice {40000};
        uint32 ApprenticeRidingSkillRequiredLevel {20};

        uint32 JourneymanRidingSkillBuyPrice {500000};
        uint32 JourneymanRidingSkillRequiredLevel {40};

        uint32 ExpertRidingSkillBuyPrice {2500000};
        uint32 ExpertRidingSkillRequiredLevel {60};

        uint32 ArtisanRidingSkillBuyPrice {50000000};
        uint32 ArtisanRidingSkillRequiredLevel {70};

        uint32 ColdWeatherFlyingSkillBuyPrice {10000000};
        uint32 ColdWeatherFlyingSkillRequiredLevel {77};

        uint32 TomeOfColdWeatherFlightBuyPrice {10000000};
        uint32 TomeOfColdWeatherFlightSellPrice {0};
        uint32 TomeOfColdWeatherFlightRequiredLevel {68};

        // Mounts
        uint32 ApprenticeRacialMountsBuyPrice {10000};
        uint32 ApprenticeRacialMountsSellPrice {2500};
        uint32 ApprenticeRacialMountsRequiredLevel {20};

        uint32 JourneymanRacialMountsBuyPrice {100000};
        uint32 JourneymanRacialMountsSellPrice {25000};
        uint32 JourneymanRacialMountsRequiredLevel {40};

        uint32 ExpertFactionMountsBuyPrice {500000};
        uint32 ExpertFactionMountsSellPrice {125000};
        uint32 ExpertFactionMountsRequiredLevel {60};
        
        uint32 ArtisanFactionMountsBuyPrice {1000000};
        uint32 ArtisanFactionMountsSellPrice {250000};
        uint32 ArtisanFactionMountsRequiredLevel {70};

        uint32 ApprenticePaladinClassMountsBuyPrice {3500};
        uint32 ApprenticePaladinClassMountsRequiredLevel {20};
        uint32 JourneymanPaladinClassMountsBuyPrice {20000};
        uint32 JourneymanPaladinClassMountsRequiredLevel {40};
        uint32 ApprenticeWarlockClassMountsBuyPrice {10000};
        uint32 ApprenticeWarlockClassMountsRequiredLevel {20};
        uint32 JourneymanWarlockClassMountsBuyPrice {100000};
        uint32 JourneymanWarlockClassMountsRequiredLevel {40};
        uint32 ExpertDruidClassMountsBuyPrice {34000};
        uint32 ExpertDruidClassMountsRequiredLevel {60};
        uint32 ArtisanDruidClassMountsBuyPrice {200000};
        uint32 ArtisanDruidClassMountsRequiredLevel {71};
        uint32 ExpertDeathKnightClassMountsBuyPrice {1000000};
        uint32 ExpertDeathKnightClassMountsSellPrice {250000};
        uint32 ExpertDeathKnightClassMountsRequiredLevel {70};
        
        std::vector<MountInfo> MiscMountsData;
        std::string MountsOverrides {""};

    public:
        static MountRequirements* instance()
        {
            static MountRequirements instance;
            return &instance;
        }

        void InitializeConfiguration();
        void UpdateMountRequirements();
        void LoadMiscMountsData();
        void ApplyCustomMountRequirements();
        void RestoreOriginalMountRequirements();

        std::string BuildItemUpdateQuery(const std::vector<uint32> ids, const uint32 buy, const uint32 sell, const uint32 level);
        std::string BuildItemUpdateQuery(const uint32 id, const uint32 buy, const uint32 sell, const uint32 level);
        void AppendMiscMountUpdate(
            WorldDatabaseTransaction t, const MountInfo m,
            const uint32 apprMountBuyPrice, const uint32 apprMountSellPrice, const uint32 apprMountReqLevel,
            const uint32 jourMountBuyPrice, const uint32 jourMountSellPrice, const uint32 jourMountReqLevel,
            const uint32 exprMountBuyPrice, const uint32 exprMountSellPrice, const uint32 exprMountReqLevel,
            const uint32 artiMountBuyPrice, const uint32 artiMountSellPrice, const uint32 artiMountReqLevel
        );
        void AppendCustomMountsOverrides(WorldDatabaseTransaction t);
        std::unordered_map<uint32, MountInfo> GetOverridenMountsInfo();
        std::string BuildSpellUpdateQuery(const std::vector<uint32> ids, const uint32 buy, const uint32 level);
        std::string BuildSpellUpdateQuery(const uint32 id, const uint32 buy, const uint32 level);
        std::string VectorToCSV(const std::vector<uint32>& v);

        MountRequirements() = default;
        ~MountRequirements() = default;
};

#define mountRequirements MountRequirements::instance()
#endif
