#include "MountRequirements.h"
#include "OriginalMountRequirements.h"

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
