#include "MountRequirements.h"
#include "OriginalMountRequirements.h"
#include "Common.h"
#include "Config.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "ItemTemplate.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "Chat.h"

using namespace std;

MountRequirements::MountRequirements() :
    debug_Out(false),
    MountRequirementsEnabled(true),
    ApprenticeRidingSkillBuyPrice(40000),
    ApprenticeRidingSkillRequiredLevel(20),
    JourneymanRidingSkillBuyPrice(500000),
    JourneymanRidingSkillRequiredLevel(40),
    ExpertRidingSkillBuyPrice(2500000),
    ExpertRidingSkillRequiredLevel(60),
    ArtisanRidingSkillBuyPrice(50000000),
    ArtisanRidingSkillRequiredLevel(70),
    ColdWeatherFlyingSkillBuyPrice(10000000),
    ColdWeatherFlyingSkillRequiredLevel(77),

    ApprenticeRacialMountsBuyPrice(10000),
    ApprenticeRacialMountsSellPrice(2500),
    ApprenticeRacialMountsRequiredLevel(20),
    JourneymanRacialMountsBuyPrice(100000),
    JourneymanRacialMountsSellPrice(25000),
    JourneymanRacialMountsRequiredLevel(40),
    ExpertFactionMountsBuyPrice(500000),
    ExpertFactionMountsSellPrice(125000),
    ExpertFactionMountsRequiredLevel(60),
    ArtisanFactionMountsBuyPrice(1000000),
    ArtisanFactionMountsSellPrice(250000),
    ArtisanFactionMountsRequiredLevel(70),

    TomeOfColdWeatherFlightBuyPrice(10000000),
    TomeOfColdWeatherFlightSellPrice(0),
    TomeOfColdWeatherFlightRequiredLevel(68)
{
}

MountRequirements::~MountRequirements() 
{
}

void MountRequirements::UpdateMountRequirements()
{
    if (!MountRequirementsEnabled)
    {
        RestoreOriginalMountRequirements();
        return;
    }
    ApplyCustomMountRequirements();
}


void MountRequirements::ApplyCustomMountRequirements()
{
    if (debug_Out)
        LOG_INFO("module", "Starting MountRequirements Update...");

    WorldDatabaseTransaction trans = WorldDatabase.BeginTransaction();

    // Set Requirements for Riding Skills
    trans->Append(BuildSpellUpdateQuery(ApprenticeRidingTrainerSpellID,  ApprenticeRidingSkillBuyPrice,  ApprenticeRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanRidingTrainerSpellID,  JourneymanRidingSkillBuyPrice,  JourneymanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ExpertRidingTrainerSpellID,      ExpertRidingSkillBuyPrice,      ExpertRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ArtisanRidingTrainerSpellID,     ArtisanRidingSkillBuyPrice,     ArtisanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ColdWeatherFlyingTrainerSpellID, ColdWeatherFlyingSkillBuyPrice, ColdWeatherFlyingSkillRequiredLevel));
    trans->Append(BuildItemUpdateQuery(std::vector<uint32>{TomeOfColdWeatherFlightItemID}, TomeOfColdWeatherFlightBuyPrice, TomeOfColdWeatherFlightSellPrice, TomeOfColdWeatherFlightRequiredLevel));

    // Set Requirements for Mounts
    trans->Append(BuildItemUpdateQuery(ApprenticeRacialMountsIDs, ApprenticeRacialMountsBuyPrice, ApprenticeRacialMountsSellPrice, ApprenticeRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(JourneymanRacialMountsIDs, JourneymanRacialMountsBuyPrice, JourneymanRacialMountsSellPrice, JourneymanRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ExpertFactionMountsIDs,    ExpertFactionMountsBuyPrice,    ExpertFactionMountsSellPrice,    ExpertFactionMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ArtisanFactionMountsIDs,   ArtisanFactionMountsBuyPrice,   ArtisanFactionMountsSellPrice,   ArtisanFactionMountsRequiredLevel));

    try 
    {
        WorldDatabase.CommitTransaction(trans);
    }
    catch (const std::exception& e)
    {
        LOG_INFO("module", "MountRequirements transaction failed: {}", e.what());
        return;
    }

    if (debug_Out)
        LOG_INFO("module", "MountRequirements Update Done");
}


void MountRequirements::RestoreOriginalMountRequirements()
{
    if (debug_Out)
        LOG_INFO("module", "Restoring Original Mount Requirements...");

    WorldDatabaseTransaction trans = WorldDatabase.BeginTransaction();

    // Restore Original Requirements for Riding Skills
    trans->Append(BuildSpellUpdateQuery(ApprenticeRidingTrainerSpellID,  OriginalApprenticeRidingSkillBuyPrice,  OriginalApprenticeRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(JourneymanRidingTrainerSpellID,  OriginalJourneymanRidingSkillBuyPrice,  OriginalJourneymanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ExpertRidingTrainerSpellID,      OriginalExpertRidingSkillBuyPrice,      OriginalExpertRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ArtisanRidingTrainerSpellID,     OriginalArtisanRidingSkillBuyPrice,     OriginalArtisanRidingSkillRequiredLevel));
    trans->Append(BuildSpellUpdateQuery(ColdWeatherFlyingTrainerSpellID, OriginalColdWeatherFlyingSkillBuyPrice, OriginalColdWeatherFlyingSkillRequiredLevel));
    trans->Append(BuildItemUpdateQuery(std::vector<uint32>{TomeOfColdWeatherFlightItemID}, OriginalTomeOfColdWeatherFlightBuyPrice, OriginalTomeOfColdWeatherFlightSellPrice, OriginalTomeOfColdWeatherFlightRequiredLevel));

    // Restore Original Requirements for Mounts
    trans->Append(BuildItemUpdateQuery(ApprenticeRacialMountsIDs, OriginalApprenticeRacialMountsBuyPrice, OriginalApprenticeRacialMountsSellPrice, OriginalApprenticeRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(JourneymanRacialMountsIDs, OriginalJourneymanRacialMountsBuyPrice, OriginalJourneymanRacialMountsSellPrice, OriginalJourneymanRacialMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ExpertFactionMountsIDs,    OriginalExpertFactionMountsBuyPrice,    OriginalExpertFactionMountsSellPrice,    OriginalExpertFactionMountsRequiredLevel));
    trans->Append(BuildItemUpdateQuery(ArtisanFactionMountsIDs,   OriginalArtisanFactionMountsBuyPrice,   OriginalArtisanFactionMountsSellPrice,   OriginalArtisanFactionMountsRequiredLevel));

    try
    {
        WorldDatabase.CommitTransaction(trans);
    }
    catch (const std::exception& e)
    {
        LOG_INFO("module", "MountRequirements transaction failed: {}", e.what());
        return;
    }
    
    if (debug_Out)
        LOG_INFO("module", "Original Mount Requirements Restoration Done.");
}


void MountRequirements::InitializeConfiguration()
{
    // Retrieve Config Values
    MountRequirementsEnabled = sConfigMgr->GetOption<bool>("MountRequirements.Enable", true);
    if (!MountRequirementsEnabled)
        return;
    debug_Out = sConfigMgr->GetOption<bool>("MountRequirements.DEBUG", false);

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

    // Tome of Cold Weather Flight
    TomeOfColdWeatherFlightBuyPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.TomeOfColdWeatherFlight.BuyPrice", 10000000);
    TomeOfColdWeatherFlightSellPrice = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.TomeOfColdWeatherFlight.SellPrice", 0);
    TomeOfColdWeatherFlightRequiredLevel = sConfigMgr->GetOption<uint32>("MountRequirements.Riding.TomeOfColdWeatherFlight.RequiredLevel", 68);

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

    if (debug_Out) 
    {
        LOG_INFO("module", "MountRequirements: DEBUG has been enabled!");
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
    }    
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
    std::string sql = "UPDATE npc_trainer SET "
        "`MoneyCost` = "  + std::to_string(buy) + ", "
        "`ReqLevel` = " + std::to_string(level) + " "
        "WHERE `SpellID` = " + to_string(id);
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