#include "MountRequirements.h"

#include "Config.h"
#include "Log.h"

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
