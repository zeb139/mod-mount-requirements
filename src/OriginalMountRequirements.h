#ifndef ORIGINAL_MOUNT_REQUIREMENTS_H
#define ORIGINAL_MOUNT_REQUIREMENTS_H

#include "Common.h"
#include <vector>

// struct MountBackup
// {
//     uint32 ItemID;
//     uint32 BuyPrice;
//     uint32 SellPrice;
//     uint32 RequiredLevel;

//     MountBackup(uint32 id, uint32 buy, uint32 sell, uint32 level)
//         : ItemID(id), BuyPrice(buy), SellPrice(sell), RequiredLevel(level) { }
// };

// Riding Skills Requirements
const uint32 ApprenticeRidingTrainerSpellID = 33388;
const uint32 OriginalApprenticeRidingSkillBuyPrice = 40000;
const uint32 OriginalApprenticeRidingSkillRequiredLevel = 20;

const uint32 JourneymanRidingTrainerSpellID = 33391;
const uint32 OriginalJourneymanRidingSkillBuyPrice = 500000;
const uint32 OriginalJourneymanRidingSkillRequiredLevel = 40;

const uint32 ExpertRidingTrainerSpellID = 34090;
const uint32 OriginalExpertRidingSkillBuyPrice = 2500000;
const uint32 OriginalExpertRidingSkillRequiredLevel = 60;

const uint32 ArtisanRidingTrainerSpellID = 34091;
const uint32 OriginalArtisanRidingSkillBuyPrice = 50000000;
const uint32 OriginalArtisanRidingSkillRequiredLevel = 70;

const uint32 ColdWeatherFlyingTrainerSpellID = 54197;
const uint32 OriginalColdWeatherFlyingSkillBuyPrice = 10000000;
const uint32 OriginalColdWeatherFlyingSkillRequiredLevel = 77;

const uint32 TomeOfColdWeatherFlightItemID = 49177;
const uint32 OriginalTomeOfColdWeatherFlightBuyPrice = 10000000;
const uint32 OriginalTomeOfColdWeatherFlightSellPrice = 0;
const uint32 OriginalTomeOfColdWeatherFlightRequiredLevel = 68;

// "Standard" Mounts Requirements
static const std::vector<uint32> ApprenticeRacialMountsIDs = {1132,2411,2414,5655,5656,5665,5668,5864,5872,5873,8563,8588,8591,8592,8595,8629,8631,8632,12325,12326,12327,13321,13322,13323,13324,13331,13332,13333,15277,15290,28481,28927,29220,29221,29222,29743,29744,46099,46100,46308,47100};
const uint32 OriginalApprenticeRacialMountsBuyPrice = 10000;
const uint32 OriginalApprenticeRacialMountsSellPrice = 2500;
const uint32 OriginalApprenticeRacialMountsRequiredLevel = 20;

static const std::vector<uint32> JourneymanRacialMountsIDs = {8586,12302,12303,12330,12351,12353,12354,13317,13326,13327,13328,13329,13334,15292,15293,18766,18767,18768,18772,18773,18774,18776,18777,18778,18785,18786,18787,18788,18789,18790,18791,18793,18794,18795,18796,18797,18798,18902,28482,28936,29223,29224,29745,29746,29747,44223,46102,47101,49044,49046};
const uint32 OriginalJourneymanRacialMountsBuyPrice = 100000;
const uint32 OriginalJourneymanRacialMountsSellPrice = 25000;
const uint32 OriginalJourneymanRacialMountsRequiredLevel = 40;

static const std::vector<uint32> ExpertFactionMountsIDs = {25470,25471,25472,25474,25475,25476};
const uint32 OriginalExpertFactionMountsBuyPrice = 500000;
const uint32 OriginalExpertFactionMountsSellPrice = 125000;
const uint32 OriginalExpertFactionMountsRequiredLevel = 60;

static const std::vector<uint32> ArtisanFactionMountsIDs = {25473,25477,25527,25528,25529,25531,25532,25533};
const uint32 OriginalArtisanFactionMountsBuyPrice = 1000000;
const uint32 OriginalArtisanFactionMountsSellPrice = 250000;
const uint32 OriginalArtisanFactionMountsRequiredLevel = 70;

// Class Mounts Requirements
static const std::vector<uint32> ApprenticePaladinClassMountsIDs = {13820,34768};
const uint32 OriginalApprenticePaladinClassMountsBuyPrice = 3500;
const uint32 OriginalApprenticePaladinClassMountsRequiredLevel = 20;

static const std::vector<uint32> JourneymanPaladinClassMountsIDs = {23214,34767};
const uint32 OriginalJourneymanPaladinClassMountsBuyPrice = 20000;
const uint32 OriginalJourneymanPaladinClassMountsRequiredLevel = 40;

static const std::vector<uint32> ApprenticeWarlockClassMountsIDs = {1710};
const uint32 OriginalApprenticeWarlockClassMountsBuyPrice = 10000;
const uint32 OriginalApprenticeWarlockClassMountsRequiredLevel = 20;

static const std::vector<uint32> JourneymanWarlockClassMountsIDs = {23161};
const uint32 OriginalJourneymanWarlockClassMountsBuyPrice = 100000;
const uint32 OriginalJourneymanWarlockClassMountsRequiredLevel = 40;

static const std::vector<uint32> ExpertDruidClassMountsIDs = {33950};
const uint32 OriginalExpertDruidClassMountsBuyPrice = 34000;
const uint32 OriginalExpertDruidClassMountsRequiredLevel = 60;

static const std::vector<uint32> ArtisanDruidClassMountsIDs = {40120};
const uint32 OriginalArtisanDruidClassMountsBuyPrice = 200000;
const uint32 OriginalArtisanDruidClassMountsRequiredLevel = 71;

#endif