#include "MountRequirements.h"
#include "OriginalMountRequirements.h"

#include "Config.h"
#include "DatabaseEnv.h"
#include "Define.h"
#include "Log.h"
#include "QueryResult.h"

#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

void MountRequirements::UpdateMountRequirements()
{
    if (MountRequirementsEnabled)
        ApplyCustomMountRequirements();
    else
        RestoreOriginalMountRequirements();
}


void MountRequirements::ApplyCustomMountRequirements()
{
    if (debug_Out)
        LOG_INFO("module", "MountRequirements: Starting MountRequirements Update...");

    WorldDatabaseTransaction trans = WorldDatabase.BeginTransaction();

    // Set Requirements for Riding Skills
    trans->Append(BuildSpellUpdateQuery(ApprenticeRidingTrainerSpellID,  ApprenticeRidingSkillBuyPrice,  ApprenticeRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanRidingTrainerSpellID,  JourneymanRidingSkillBuyPrice,  JourneymanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ExpertRidingTrainerSpellID,      ExpertRidingSkillBuyPrice,      ExpertRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ArtisanRidingTrainerSpellID,     ArtisanRidingSkillBuyPrice,     ArtisanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ColdWeatherFlyingTrainerSpellID, ColdWeatherFlyingSkillBuyPrice, ColdWeatherFlyingSkillRequiredLevel));
    trans->Append(BuildItemUpdateQuery(TomeOfColdWeatherFlightItemID, TomeOfColdWeatherFlightBuyPrice, TomeOfColdWeatherFlightSellPrice, TomeOfColdWeatherFlightRequiredLevel));

    // Set Requirements for Mounts
    trans->Append(BuildItemUpdateQuery(ApprenticeRacialMountsIDs, ApprenticeRacialMountsBuyPrice, ApprenticeRacialMountsSellPrice, ApprenticeRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(JourneymanRacialMountsIDs, JourneymanRacialMountsBuyPrice, JourneymanRacialMountsSellPrice, JourneymanRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ExpertFactionMountsIDs,    ExpertFactionMountsBuyPrice,    ExpertFactionMountsSellPrice,    ExpertFactionMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ArtisanFactionMountsIDs,   ArtisanFactionMountsBuyPrice,   ArtisanFactionMountsSellPrice,   ArtisanFactionMountsRequiredLevel));
    
    trans->Append(BuildSpellUpdateQuery(ApprenticePaladinClassMountsIDs, ApprenticePaladinClassMountsBuyPrice,  ApprenticePaladinClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanPaladinClassMountsIDs, JourneymanPaladinClassMountsBuyPrice,  JourneymanPaladinClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ApprenticeWarlockClassMountsIDs, ApprenticeWarlockClassMountsBuyPrice,  ApprenticeWarlockClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanWarlockClassMountsIDs, JourneymanWarlockClassMountsBuyPrice,  JourneymanWarlockClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ExpertDruidClassMountsIDs, ExpertDruidClassMountsBuyPrice,  ExpertDruidClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ArtisanDruidClassMountsIDs, ArtisanDruidClassMountsBuyPrice,  ArtisanDruidClassMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ExpertDeathKnightClassMountsIDs, ExpertDeathKnightClassMountsBuyPrice, ExpertDeathKnightClassMountsSellPrice, ExpertDeathKnightClassMountsRequiredLevel));

    // Set Requirements for Misc Mounts
    for (MountInfo mI : MiscMountsData) 
    {
        AppendMiscMountUpdate(trans, mI,
            ApprenticeRacialMountsBuyPrice, ApprenticeRacialMountsSellPrice, ApprenticeRacialMountsRequiredLevel,
            JourneymanRacialMountsBuyPrice, JourneymanRacialMountsSellPrice, JourneymanRacialMountsRequiredLevel,
            ExpertFactionMountsBuyPrice,    ExpertFactionMountsSellPrice,    ExpertFactionMountsRequiredLevel,
            ArtisanFactionMountsBuyPrice,   ArtisanFactionMountsSellPrice,   ArtisanFactionMountsRequiredLevel
        );
    }

    // Set Requirements for Custom Overrides
    if (!MountsOverrides.empty())
        AppendCustomMountsOverrides(trans);

    try 
    {
        WorldDatabase.CommitTransaction(trans);
    }
    catch (const std::exception& e)
    {
        LOG_INFO("module", "MountRequirements: MountRequirements transaction failed: {}", e.what());
        return;
    }

    if (debug_Out)
        LOG_INFO("module", "MountRequirements: MountRequirements Update Done");
}

void MountRequirements::AppendCustomMountsOverrides(WorldDatabaseTransaction t)
{
    std::unordered_map<uint32, MountInfo> overriddenMounts = GetOverridenMountsInfo();
    for (auto it : overriddenMounts)
        t->Append(BuildItemUpdateQuery(it.second.ItemID, it.second.BuyPrice, it.second.SellPrice, it.second.RequiredLevel));
}

std::unordered_map<uint32, MountInfo> MountRequirements::GetOverridenMountsInfo()
{
    // Parse MountsOverrides String data
    std::unordered_map<uint32, MountInfo> overrides;
    std::stringstream ss(MountsOverrides);
    std::string overriddenMount;
    std::ostringstream overriddenMountItemIDs;
    bool first = true;

    while (std::getline(ss, overriddenMount, ','))
    {
        if (overriddenMount.empty())
            continue;

        std::stringstream fieldStream(overriddenMount);
        std::vector<std::string> fields;
        std::string field;

        if (overriddenMount.find(":") != std::string::npos)
        {
            while (std::getline(fieldStream, field, ':'))
            {
                fields.push_back(field);
            }

            if (fields.size() != 4) 
            {
                LOG_INFO("module", "MountRequirements: Invalid mount override format {}", overriddenMount);
                continue;
            }

            MountInfo mI;
            mI.ItemID        = std::stoul(fields[0]);
            mI.BuyPrice      = std::stoul(fields[1]);
            mI.SellPrice     = std::stoul(fields[2]);
            mI.RequiredLevel = std::stoul(fields[3]);

            overrides[mI.ItemID] = mI;

            if (!first)
                overriddenMountItemIDs << ',';
            overriddenMountItemIDs << mI.ItemID;
            first = false;
        }
    }

    // Remove entries that are not mounts
    QueryResult result = WorldDatabase.Query("SELECT `entry`, `class`, `subclass`, `name` FROM `item_template` WHERE entry IN ({})", overriddenMountItemIDs.str());
    if (!result || result->GetRowCount() == 0)
    {
        LOG_INFO("module", "MountRequirements: Something went wrong while comparing MountRequirements.OverrideMounts to item_template database. Skipping overrides.");
        std::unordered_map<uint32, MountInfo> emptyMap;
        return emptyMap;
    }
    do
    {
        Field* fields = result->Fetch();
        uint32 entry         = fields[0].Get<uint32>();
        uint32 itemClass     = fields[1].Get<uint32>();
        uint32 itemSubclass  = fields[2].Get<uint32>();
        std::string itemName = fields[3].Get<std::string>();
        
        if (itemClass != 15 || itemSubclass != 5)
        {
            LOG_INFO("module", "MountRequirements: Did not override values for {}, entry: {}. It is not a mount.", itemName, entry);
            overrides.erase(entry);
        }
    } while (result->NextRow());

    return overrides;
}

void MountRequirements::RestoreOriginalMountRequirements()
{
    if (debug_Out)
        LOG_INFO("module", "MountRequirements: Restoring Original Mount Requirements...");

    WorldDatabaseTransaction trans = WorldDatabase.BeginTransaction();

    // Restore Original Requirements for Riding Skills
    trans->Append(BuildSpellUpdateQuery(ApprenticeRidingTrainerSpellID,  OriginalApprenticeRidingSkillBuyPrice,  OriginalApprenticeRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanRidingTrainerSpellID,  OriginalJourneymanRidingSkillBuyPrice,  OriginalJourneymanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ExpertRidingTrainerSpellID,      OriginalExpertRidingSkillBuyPrice,      OriginalExpertRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ArtisanRidingTrainerSpellID,     OriginalArtisanRidingSkillBuyPrice,     OriginalArtisanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ColdWeatherFlyingTrainerSpellID, OriginalColdWeatherFlyingSkillBuyPrice, OriginalColdWeatherFlyingSkillRequiredLevel));
    trans->Append(BuildItemUpdateQuery(TomeOfColdWeatherFlightItemID, OriginalTomeOfColdWeatherFlightBuyPrice, OriginalTomeOfColdWeatherFlightSellPrice, OriginalTomeOfColdWeatherFlightRequiredLevel));

    // Restore Original Requirements for Mounts
    trans->Append(BuildItemUpdateQuery(ApprenticeRacialMountsIDs, OriginalApprenticeRacialMountsBuyPrice, OriginalApprenticeRacialMountsSellPrice, OriginalApprenticeRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(JourneymanRacialMountsIDs, OriginalJourneymanRacialMountsBuyPrice, OriginalJourneymanRacialMountsSellPrice, OriginalJourneymanRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ExpertFactionMountsIDs,    OriginalExpertFactionMountsBuyPrice,    OriginalExpertFactionMountsSellPrice,    OriginalExpertFactionMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ArtisanFactionMountsIDs,   OriginalArtisanFactionMountsBuyPrice,   OriginalArtisanFactionMountsSellPrice,   OriginalArtisanFactionMountsRequiredLevel));
    
    trans->Append(BuildSpellUpdateQuery(ApprenticePaladinClassMountsIDs, OriginalApprenticePaladinClassMountsBuyPrice,  OriginalApprenticePaladinClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanPaladinClassMountsIDs, OriginalJourneymanPaladinClassMountsBuyPrice,  OriginalJourneymanPaladinClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ApprenticeWarlockClassMountsIDs, OriginalApprenticeWarlockClassMountsBuyPrice,  OriginalApprenticeWarlockClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanWarlockClassMountsIDs, OriginalJourneymanWarlockClassMountsBuyPrice,  OriginalJourneymanWarlockClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ExpertDruidClassMountsIDs, OriginalExpertDruidClassMountsBuyPrice,  OriginalExpertDruidClassMountsRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ArtisanDruidClassMountsIDs, OriginalArtisanDruidClassMountsBuyPrice,  OriginalArtisanDruidClassMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ExpertDeathKnightClassMountsIDs, OriginalExpertDeathKnightClassMountsBuyPrice, OriginalExpertDeathKnightClassMountsSellPrice, OriginalExpertDeathKnightClassMountsRequiredLevel));

    // Restore Original Requirements for Misc Mounts
    for (MountInfo mI : MiscMountsData)
        trans->Append(BuildItemUpdateQuery(mI.ItemID, mI.BuyPrice, mI.SellPrice, mI.RequiredLevel));

    try
    {
        WorldDatabase.CommitTransaction(trans);
    }
    catch (const std::exception& e)
    {
        LOG_INFO("module", "MountRequirements: MountRequirements transaction failed: {}", e.what());
        return;
    }
    
    if (debug_Out)
        LOG_INFO("module", "MountRequirements: Original Mount Requirements Restoration Done.");
}


void MountRequirements::AppendMiscMountUpdate(
        WorldDatabaseTransaction t, const MountInfo m,
        const uint32 apprMountBuyPrice, const uint32 apprMountSellPrice, const uint32 apprMountReqLevel,
        const uint32 jourMountBuyPrice, const uint32 jourMountSellPrice, const uint32 jourMountReqLevel,
        const uint32 exprMountBuyPrice, const uint32 exprMountSellPrice, const uint32 exprMountReqLevel,
        const uint32 artiMountBuyPrice, const uint32 artiMountSellPrice, const uint32 artiMountReqLevel
)
{
    if (m.RequiredSkill == RIDING_SKILL_ID)
    {
        switch (m.RequiredSkillRank)
        {
            case APPRENTICE_RIDING_SKILL_RANK:
                t->Append(BuildItemUpdateQuery(m.ItemID, apprMountBuyPrice, apprMountSellPrice, apprMountReqLevel));
            case JOURNEYMAN_RIDING_SKILL_RANK:
                t->Append(BuildItemUpdateQuery(m.ItemID, jourMountBuyPrice, jourMountSellPrice, jourMountReqLevel));
            case EXPERT_RIDING_SKILL_RANK:
                t->Append(BuildItemUpdateQuery(m.ItemID, exprMountBuyPrice, exprMountSellPrice, exprMountReqLevel));
            case ARTISAN_RIDING_SKILL_RANK:
                t->Append(BuildItemUpdateQuery(m.ItemID, artiMountBuyPrice, artiMountSellPrice, artiMountReqLevel));
            default:
                break;
        }
    }
    else if (m.RequiredSkill == ENGINEERING_SKILL_ID)
    {
        if (m.RequiredSkillRank == 300)
            t->Append(BuildItemUpdateQuery(m.ItemID, m.BuyPrice, m.SellPrice, exprMountReqLevel));
        else if (m.RequiredSkillRank == 375)
            t->Append(BuildItemUpdateQuery(m.ItemID, m.BuyPrice, m.SellPrice, artiMountReqLevel));
    }
    else if (m.RequiredSkill == TAILORING_SKILL_ID)
    {
        // Do nothing? Costs are determined by Recipe
    }
    else if (m.RequiredSkill == 0)
    {
        if (m.RequiredLevel == 20)
            t->Append(BuildItemUpdateQuery(m.ItemID, apprMountBuyPrice, apprMountSellPrice, apprMountReqLevel));
        else if (m.RequiredLevel == 40)
            t->Append(BuildItemUpdateQuery(m.ItemID, jourMountBuyPrice, jourMountSellPrice, jourMountReqLevel));
        else if (m.RequiredLevel == 77)
            t->Append(BuildItemUpdateQuery(m.ItemID, exprMountBuyPrice, exprMountSellPrice, exprMountReqLevel));
    }
}

void MountRequirements::InitializeConfiguration()
{
    // Retrieve Config Values
    MountRequirementsEnabled = sConfigMgr->GetOption<bool>("MountRequirements.Enable", true);
    debug_Out = sConfigMgr->GetOption<bool>("MountRequirements.DEBUG", false);

    // LoadMiscMountsData before checking MountRequirementsEnabled so we can revert changes if needed
    LoadMiscMountsData();

    // If module disabled
    if (!MountRequirementsEnabled)
        return;

    // Skills
    ApprenticeRidingSkillBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Apprentice.BuyPrice", 40000);
    ApprenticeRidingSkillRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Apprentice.RequiredLevel", 20);
    JourneymanRidingSkillBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Journeyman.BuyPrice", 500000);
    JourneymanRidingSkillRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Journeyman.RequiredLevel", 40);
    ExpertRidingSkillBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Expert.BuyPrice", 2500000);
    ExpertRidingSkillRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Expert.RequiredLevel", 60);
    ArtisanRidingSkillBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Artisan.BuyPrice", 50000000);
    ArtisanRidingSkillRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.Artisan.RequiredLevel", 70);
    ColdWeatherFlyingSkillBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.ColdWeatherFlying.BuyPrice", 10000000);
    ColdWeatherFlyingSkillRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.ColdWeatherFlying.RequiredLevel", 77);

    // Mounts
    ApprenticeRacialMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Racial.Apprentice.BuyPrice", 10000);
    ApprenticeRacialMountsSellPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Racial.Apprentice.SellPrice", 2500);
    ApprenticeRacialMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Racial.Apprentice.RequiredLevel", 20);
    JourneymanRacialMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Racial.Journeyman.BuyPrice", 100000);
    JourneymanRacialMountsSellPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Racial.Journeyman.SellPrice", 25000);
    JourneymanRacialMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Racial.Journeyman.RequiredLevel", 40);
    ExpertFactionMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Faction.Expert.BuyPrice", 500000);
    ExpertFactionMountsSellPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Faction.Expert.SellPrice", 125000);
    ExpertFactionMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Faction.Expert.RequiredLevel", 60);
    ArtisanFactionMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Faction.Artisan.BuyPrice", 1000000);
    ArtisanFactionMountsSellPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Faction.Artisan.SellPrice", 250000);
    ArtisanFactionMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.Faction.Artisan.RequiredLevel", 70);

    ApprenticePaladinClassMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.PaladinClass.Apprentice.BuyPrice", 3500);
    ApprenticePaladinClassMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.PaladinClass.Apprentice.RequiredLevel", 20);
    JourneymanPaladinClassMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.PaladinClass.Journeyman.BuyPrice", 20000);
    JourneymanPaladinClassMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.PaladinClass.Journeyman.RequiredLevel", 40);
    ApprenticeWarlockClassMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.WarlockClass.Apprentice.BuyPrice", 3500);
    ApprenticeWarlockClassMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.WarlockClass.Apprentice.RequiredLevel", 20);
    JourneymanWarlockClassMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.WarlockClass.Journeyman.BuyPrice", 20000);
    JourneymanWarlockClassMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.WarlockClass.Journeyman.RequiredLevel", 40);
    ExpertDruidClassMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.DruidClass.Expert.BuyPrice", 34000);
    ExpertDruidClassMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.DruidClass.Expert.RequiredLevel", 60);
    ArtisanDruidClassMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.DruidClass.Artisan.BuyPrice", 200000);
    ArtisanDruidClassMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.DruidClass.Artisan.RequiredLevel", 71);
    ExpertDeathKnightClassMountsBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.DeathKnightClass.Expert.BuyPrice", 1000000);
    ExpertDeathKnightClassMountsSellPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.DeathKnightClass.Expert.SellPrice", 250000);
    ExpertDeathKnightClassMountsRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Mount.DeathKnightClass.Expert.RequiredLevel", 70);

    // Tome of Cold Weather Flight
    TomeOfColdWeatherFlightBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.TomeOfColdWeatherFlight.BuyPrice", 10000000);
    TomeOfColdWeatherFlightSellPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.TomeOfColdWeatherFlight.SellPrice", 0);
    TomeOfColdWeatherFlightRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.TomeOfColdWeatherFlight.RequiredLevel", 68);

    // Mount Overrides string
    MountsOverrides = sConfigMgr->GetOption<std::string>("MountRequirements.OverrideMounts", "");

    // Skills' requirements can't be invalid
    if (ApprenticeRidingSkillBuyPrice < 0)       ApprenticeRidingSkillBuyPrice = 0;
    if (ApprenticeRidingSkillRequiredLevel < 1)  ApprenticeRidingSkillRequiredLevel = 1;
    if (JourneymanRidingSkillBuyPrice < 0)       JourneymanRidingSkillBuyPrice = 0;
    if (JourneymanRidingSkillRequiredLevel < 1)  JourneymanRidingSkillRequiredLevel = 1;
    if (ExpertRidingSkillBuyPrice < 0)           ExpertRidingSkillBuyPrice = 0;
    if (ExpertRidingSkillRequiredLevel < 1)      ExpertRidingSkillRequiredLevel = 1;
    if (ArtisanRidingSkillBuyPrice < 0)          ArtisanRidingSkillBuyPrice = 0;
    if (ArtisanRidingSkillRequiredLevel < 1)     ArtisanRidingSkillRequiredLevel = 1;
    if (ColdWeatherFlyingSkillBuyPrice < 0)      ColdWeatherFlyingSkillBuyPrice = 0;
    if (ColdWeatherFlyingSkillRequiredLevel < 1) ColdWeatherFlyingSkillRequiredLevel = 1;
    
    // Mounts' requirements can't be invalid
    if (ApprenticeRacialMountsBuyPrice < 0)      ApprenticeRacialMountsBuyPrice = 0;
    if (ApprenticeRacialMountsSellPrice < 0)     ApprenticeRacialMountsSellPrice = 0;
    if (ApprenticeRacialMountsRequiredLevel < 1) ApprenticeRacialMountsRequiredLevel = 1;
    if (JourneymanRacialMountsBuyPrice < 0)      JourneymanRacialMountsBuyPrice = 0;
    if (JourneymanRacialMountsSellPrice < 0)     JourneymanRacialMountsSellPrice = 0;
    if (JourneymanRacialMountsRequiredLevel < 1) JourneymanRacialMountsRequiredLevel = 1;
    if (ExpertFactionMountsBuyPrice < 0)         ExpertFactionMountsBuyPrice = 0;
    if (ExpertFactionMountsSellPrice < 0)        ExpertFactionMountsSellPrice = 0;
    if (ExpertFactionMountsRequiredLevel < 1)    ExpertFactionMountsRequiredLevel = 1;
    if (ArtisanFactionMountsBuyPrice < 0)        ArtisanFactionMountsBuyPrice = 0;
    if (ArtisanFactionMountsSellPrice < 0)       ArtisanFactionMountsSellPrice = 0;
    if (ArtisanFactionMountsRequiredLevel < 1)   ArtisanFactionMountsRequiredLevel = 1;

    if (ApprenticePaladinClassMountsBuyPrice < 0)      ApprenticePaladinClassMountsBuyPrice = 0;
    if (ApprenticePaladinClassMountsRequiredLevel < 1) ApprenticePaladinClassMountsRequiredLevel = 1;
    if (JourneymanPaladinClassMountsBuyPrice < 0)      JourneymanPaladinClassMountsBuyPrice = 0;
    if (JourneymanPaladinClassMountsRequiredLevel < 1) JourneymanPaladinClassMountsRequiredLevel = 1;
    if (ApprenticeWarlockClassMountsBuyPrice < 0)      ApprenticeWarlockClassMountsBuyPrice = 0;
    if (ApprenticeWarlockClassMountsRequiredLevel < 1) ApprenticeWarlockClassMountsRequiredLevel = 1;
    if (JourneymanWarlockClassMountsBuyPrice < 0)      JourneymanWarlockClassMountsBuyPrice = 0;
    if (JourneymanWarlockClassMountsRequiredLevel < 1) JourneymanWarlockClassMountsRequiredLevel = 1;
    if (ExpertDruidClassMountsBuyPrice < 0)      ExpertDruidClassMountsBuyPrice = 0;
    if (ExpertDruidClassMountsRequiredLevel < 1) ExpertDruidClassMountsRequiredLevel = 1;
    if (ArtisanDruidClassMountsBuyPrice < 0)      ArtisanDruidClassMountsBuyPrice = 0;
    if (ArtisanDruidClassMountsRequiredLevel < 1) ArtisanDruidClassMountsRequiredLevel = 1;
    if (ExpertDeathKnightClassMountsBuyPrice < 0)      ExpertDeathKnightClassMountsBuyPrice = 0;
    if (ExpertDeathKnightClassMountsSellPrice < 0)     ExpertDeathKnightClassMountsSellPrice = 0;
    if (ExpertDeathKnightClassMountsRequiredLevel < 1) ExpertDeathKnightClassMountsRequiredLevel = 1;

    if (debug_Out) 
    {
        LOG_INFO("module", "MountRequirements: Apprentice Riding buy price:  {}", ApprenticeRidingSkillBuyPrice);
        LOG_INFO("module", "MountRequirements: Journeyman Riding buy price:  {}", JourneymanRidingSkillBuyPrice);
        LOG_INFO("module", "MountRequirements: Expert Riding     buy price:  {}", ExpertRidingSkillBuyPrice);
        LOG_INFO("module", "MountRequirements: Artisan Riding    buy price:  {}", ArtisanRidingSkillBuyPrice);
        LOG_INFO("module", "MountRequirements: Apprentice Riding required level:  {}", ApprenticeRidingSkillRequiredLevel);
        LOG_INFO("module", "MountRequirements: Journeyman Riding required level:  {}", JourneymanRidingSkillRequiredLevel);
        LOG_INFO("module", "MountRequirements: Expert Riding     required level:  {}", ExpertRidingSkillRequiredLevel);
        LOG_INFO("module", "MountRequirements: Artisan Riding    required level:  {}", ArtisanRidingSkillRequiredLevel);
        LOG_INFO("module", "---");
        LOG_INFO("module", "MountRequirements: Apprentice Racial Mounts buy price:  {}", ApprenticeRacialMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Journeyman Racial Mounts buy price:  {}", JourneymanRacialMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Expert Faction Mounts    buy price:  {}", ExpertFactionMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Artisan Faction Mounts   buy price:  {}", ArtisanFactionMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Apprentice Racial Mounts sell price: {}", ApprenticeRacialMountsSellPrice);
        LOG_INFO("module", "MountRequirements: Journeyman Racial Mounts sell price: {}", JourneymanRacialMountsSellPrice);
        LOG_INFO("module", "MountRequirements: Expert Faction Mounts    sell price: {}", ExpertFactionMountsSellPrice);
        LOG_INFO("module", "MountRequirements: Artisan Faction Mounts   sell price: {}", ArtisanFactionMountsSellPrice);
        LOG_INFO("module", "MountRequirements: Apprentice Racial Mounts required level: {}", ApprenticeRacialMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Journeyman Racial Mounts required level: {}", JourneymanRacialMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Expert Faction Mounts    required level: {}", ExpertFactionMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Artisan Faction Mounts   required level: {}", ArtisanFactionMountsRequiredLevel);
        LOG_INFO("module", "---");
        LOG_INFO("module", "MountRequirements: Apprentice Paladin Class Mounts buy price:  {}", ApprenticePaladinClassMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Journeyman Paladin Class Mounts buy price:  {}", JourneymanPaladinClassMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Apprentice Paladin Class Mounts required level: {}", ApprenticePaladinClassMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Journeyman Paladin Class Mounts required level: {}", JourneymanPaladinClassMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Apprentice Warlock Class Mounts buy price:  {}", ApprenticeWarlockClassMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Journeyman Warlock Class Mounts buy price:  {}", JourneymanWarlockClassMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Apprentice Warlock Class Mounts required level: {}", ApprenticeWarlockClassMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Journeyman Warlock Class Mounts required level: {}", JourneymanWarlockClassMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Expert  Druid Class Mounts buy price:  {}", ExpertDruidClassMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Artisan Druid Class Mounts buy price:  {}", ArtisanDruidClassMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Expert  Druid Class Mounts required level: {}", ExpertDruidClassMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Artisan Druid Class Mounts required level: {}", ArtisanDruidClassMountsRequiredLevel);
        LOG_INFO("module", "MountRequirements: Expert DeathKnight Class Mounts buy price: {}", ExpertDeathKnightClassMountsBuyPrice);
        LOG_INFO("module", "MountRequirements: Expert DeathKnight Class Mounts sell price: {}", ExpertDeathKnightClassMountsSellPrice);
        LOG_INFO("module", "MountRequirements: Expert DeathKnight Class Mounts required level: {}", ExpertDeathKnightClassMountsRequiredLevel);
        LOG_INFO("module", "---");
        LOG_INFO("module", "MountRequirements: Loaded the following overrides: {}", MountsOverrides);
    }    
}

void MountRequirements::LoadMiscMountsData()
{
    std::istringstream mountBackupDataStream(MountBackupData);
    std::string mountData;

    while (std::getline(mountBackupDataStream, mountData))
    {
        if (mountData.empty())
            continue;
        
        MiscMountsData.push_back(MountInfo::fromCSV(mountData));
    }

    if (debug_Out)
    {
        LOG_INFO("module", "\nMountRequirements: Loaded the following miscellaneous mounts:");
        for (MountInfo mI : MiscMountsData) 
        {
            LOG_INFO("module", "MountRequirements: ItemID: {}, BuyPrice: {}, SellPrice: {}, RequiredLevel: {}, RequiredSkill: {}, RequiredSkillRank: {}",
            mI.ItemID, mI.BuyPrice, mI.SellPrice, mI.RequiredLevel, mI.RequiredSkill, mI.RequiredSkillRank);
        }
        LOG_INFO("module", "\n");
    }
}

std::string MountRequirements::BuildItemUpdateQuery(const uint32 id, const uint32 buy, const uint32 sell, const uint32 level)
{
    return MountRequirements::BuildItemUpdateQuery(std::vector<uint32>{id}, buy, sell, level);
}

std::string MountRequirements::BuildItemUpdateQuery(const std::vector<uint32> ids, const uint32 buy, const uint32 sell, const uint32 level)
{
    std::string entriesCSV = VectorToCSV(ids);
    std::string sql = "UPDATE item_template SET "
        "`BuyPrice` = "  + std::to_string(buy) + ", "
        "`SellPrice` = " + std::to_string(sell) + ", "
        "`RequiredLevel` = " + std::to_string(level) + " "
        "WHERE entry IN (" + entriesCSV + ")";
    return sql;
}

std::string MountRequirements::BuildSpellUpdateQuery(const uint32 id, const uint32 buy, const uint32 level)
{
    return MountRequirements::BuildSpellUpdateQuery(std::vector<uint32>{id}, buy, level);
}

std::string MountRequirements::BuildSpellUpdateQuery(const std::vector<uint32> ids, const uint32 buy, const uint32 level)
{
    std::string entriesCSV = VectorToCSV(ids);
    std::string sql = "UPDATE npc_trainer SET "
        "`MoneyCost` = "  + std::to_string(buy) + ", "
        "`ReqLevel` = " + std::to_string(level) + " "
        "WHERE `SpellID` IN (" + entriesCSV + ")";
    return sql;
}

std::string MountRequirements::VectorToCSV(const std::vector<uint32>& v)
{
    std::ostringstream oss;
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (i > 0)
            oss << ",";
        oss << v[i];
    }
    return oss.str();
}
