#ifndef ORIGINAL_MOUNT_REQUIREMENTS_H
#define ORIGINAL_MOUNT_REQUIREMENTS_H

#include "Common.h"
#include <sstream>
#include <vector>

const uint32 RIDING_SKILL_ID        = 762;
const uint32 ENGINEERING_SKILL_ID   = 202;
const uint32 TAILORING_SKILL_ID     = 197;
const uint32 APPRENTICE_RIDING_SKILL_RANK = 75;
const uint32 JOURNEYMAN_RIDING_SKILL_RANK = 150;
const uint32 EXPERT_RIDING_SKILL_RANK     = 225;
const uint32 ARTISAN_RIDING_SKILL_RANK    = 300;

struct MountBackup
{
    uint32 ItemID;
    uint32 BuyPrice;
    uint32 SellPrice;
    uint32 RequiredLevel;
    uint32 RequiredSkill;
    uint32 RequiredSkillRank;

    MountBackup() 
        : ItemID(0), BuyPrice(0), SellPrice(0),
          RequiredLevel(0), RequiredSkill(0), RequiredSkillRank(0) {}

    MountBackup(uint32 id, uint32 buy, uint32 sell, uint32 level, uint32 skill, uint32 skillRank)
        : ItemID(id), BuyPrice(buy), SellPrice(sell), RequiredLevel(level), RequiredSkill(skill), RequiredSkillRank(skillRank) { }

    static MountBackup fromCSV(const std::string& line)
    {
        std::vector<std::string> fields;
        std::istringstream stream(line);
        std::string field;

        while (std::getline(stream, field, ',')) {
            fields.push_back(field);
        }

        return MountBackup(
            std::stoul(fields[0]),
            std::stoul(fields[1]),
            std::stoul(fields[2]),
            std::stoul(fields[3]),
            std::stoul(fields[4]),
            std::stoul(fields[5])
        );
    }
};

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

// Every other mount in the game from item_template
// entry, buyprice, sellprice, requiredlevel, requiredskill, requiredskillrank
static const char* MountBackupData = R"(
38265,0,0,60,762,150
47179,0,0,40,762,150
47180,0,0,40,762,150
44690,20000000,0,70,762,300
44689,20000000,0,70,762,300
32458,1000000,0,70,762,300
33809,0,0,70,762,150
38576,100000,0,40,762,150
49282,100000,0,40,762,150
43599,0,0,20,762,75
40777,1000000,0,60,762,150
44225,7500000,0,40,762,150
44226,7500000,0,40,762,150
43964,1000000,0,40,0,0
44224,100000,0,40,762,150
43963,1000000,0,40,0,0
43962,100000,0,40,762,150
18243,0,0,40,762,75
29465,0,0,40,762,150
18241,0,0,40,762,75
29468,0,0,40,762,150
37827,100000,0,20,0,0
33976,100000,0,20,762,75
37828,1000000,0,40,762,150
33977,1000000,0,40,762,150
54811,0,0,20,762,75
49098,0,0,40,762,150
49096,0,0,40,762,150
45591,0,0,40,762,150
13335,0,0,40,762,150
44843,20000000,0,70,762,300
44842,20000000,0,70,762,300
46814,0,0,70,762,300
44178,2000000,0,70,762,300
43952,2000000,0,70,762,300
43986,2000000,0,70,762,300
43953,2000000,0,70,762,300
43951,2000000,0,70,762,300
49636,2000000,0,70,762,300
43955,20000000,0,70,762,300
43954,2000000,0,70,762,300
45590,0,0,40,762,150
46756,5000000,1250000,40,762,150
46745,5000000,1250000,40,762,150
35906,0,0,40,762,150
30480,0,0,40,762,150
13325,0,0,30,762,75
33176,0,0,60,762,225
44554,0,0,60,197,300
34060,0,0,60,202,300
45597,0,0,40,762,150
54797,2000000,0,70,197,425
43516,2000000,0,70,762,300
46708,2000000,0,70,762,300
46171,2000000,0,70,762,300
34092,2000000,0,70,762,300
51954,2000000,0,70,762,300
51955,2000000,0,70,762,300
47840,2000000,0,70,762,300
37676,2000000,0,70,762,300
50435,2000000,0,70,762,300
45589,0,0,40,762,150
45596,0,0,40,762,150
46816,0,0,40,762,150
46761,5000000,1250000,40,762,150
46751,5000000,1250000,40,762,150
35513,1000000,0,40,762,150
45725,0,0,70,762,300
54069,100000,0,60,762,225
33999,20000000,0,70,762,300
46813,0,0,70,762,300
50818,0,0,20,762,75
18247,0,0,40,762,75
29466,0,0,40,762,150
46750,5000000,1250000,40,762,150
46755,5000000,1250000,40,762,150
45592,0,0,40,762,150
44221,1000000,0,77,0,0
44229,1000000,0,77,0,0
37011,0,0,10,762,75
46778,1000000,0,60,762,150
49290,1000000,0,40,762,150
44558,2000000,0,70,197,425
43956,0,0,40,762,150
44077,0,0,40,762,150
43959,1000000,0,40,762,150
44083,1000000,0,40,762,150
43961,100000000,25000000,40,762,150
44086,100000000,25000000,40,762,150
43958,10000000,2500000,40,762,150
44080,10000000,2500000,40,762,150
44235,200000000,0,40,762,150
44234,200000000,0,40,762,150
44230,0,0,40,762,150
44231,0,0,40,762,150
41508,50000,12500,40,762,150
44413,50000,0,40,762,150
45693,2000000,0,70,762,300
30609,2000000,0,70,762,300
32319,2000000,0,70,762,300
32314,2000000,0,70,762,300
32316,2000000,0,70,762,300
32317,2000000,0,70,762,300
32318,2000000,0,70,762,300
32858,2000000,0,70,762,300
32859,2000000,0,70,762,300
32857,2000000,0,70,762,300
32860,2000000,0,70,762,300
32861,2000000,0,70,762,300
32862,2000000,0,70,762,300
33183,0,0,10,762,75
44164,2000000,0,70,762,300
44151,2000000,0,70,762,300
44707,2000000,0,70,762,300
45801,2000000,0,70,762,300
44175,2000000,0,70,762,300
44160,2000000,0,70,762,300
45802,2000000,0,70,762,300
44168,2000000,0,70,762,300
44177,2000000,0,70,762,300
21176,1000000,0,40,762,150
21218,1000000,0,40,762,75
21323,1000000,0,40,762,75
21321,1000000,0,40,762,75
21324,1000000,0,40,762,75
46815,0,0,40,762,150
18244,0,0,40,762,75
29467,0,0,40,762,150
45586,0,0,40,762,150
46748,5000000,1250000,40,762,150
46762,5000000,1250000,40,762,150
45593,0,0,40,762,150
46743,5000000,1250000,40,762,150
46760,5000000,1250000,40,762,150
19872,0,0,40,762,150
18246,0,0,40,762,75
29472,0,0,40,762,150
52200,0,0,20,762,75
32768,0,0,40,762,150
13086,1000000,250000,40,762,75
50250,0,0,80,762,150
49285,1000000,0,60,762,225
35225,1000000,0,60,762,225
35226,2000000,0,70,762,300
49286,2000000,0,70,762,300
54860,100000,0,60,762,225
46109,0,0,0,762,75
46101,0,0,60,762,150
18248,0,0,40,762,75
29470,0,0,40,762,150
46764,5000000,1250000,40,762,150
46746,5000000,1250000,40,762,150
19030,0,0,40,762,150
45125,0,0,40,762,150
44557,0,0,70,197,450
33182,0,0,70,762,300
39303,0,0,70,197,420
46758,5000000,1250000,40,762,150
46752,5000000,1250000,40,762,150
33184,0,0,40,762,150
44555,0,0,70,197,450
46744,5000000,1250000,40,762,150
46759,5000000,1250000,40,762,150
44556,0,0,70,197,450
34129,0,0,40,762,150
37719,100000,0,40,762,150
37598,100000,0,40,0,0
31829,700000,0,40,762,150
31830,700000,0,40,762,150
29102,1000000,0,40,762,150
29227,1000000,0,40,762,150
28915,0,0,40,762,150
29228,0,0,40,762,150
31831,700000,0,40,762,150
31832,700000,0,40,762,150
29104,1000000,0,40,762,150
29229,1000000,0,40,762,150
31833,700000,0,40,762,150
31834,700000,0,40,762,150
29105,1000000,0,40,762,150
29230,1000000,0,40,762,150
31835,700000,0,40,762,150
31836,700000,0,40,762,150
29103,1000000,0,40,762,150
29231,1000000,0,40,762,150
37012,0,0,20,762,75
18242,0,0,40,762,75
29471,0,0,40,762,150
33224,10000,0,20,762,75
49283,10000,0,20,762,75
49284,100000,0,40,762,150
33225,100000,0,40,762,150
19902,0,0,40,762,150
34061,0,0,70,202,375
46747,5000000,1250000,40,762,150
46763,5000000,1250000,40,762,150
40775,10000000,0,60,762,225
18245,0,0,40,762,75
29469,0,0,40,762,150
19029,0,0,40,762,150
45595,0,0,40,762,150
46757,5000000,1250000,40,762,150
46749,5000000,1250000,40,762,150
54068,100000,0,40,762,150
29225,0,0,40,0,0
)";

#endif


/* SQL to lookup all misc. mounts in the game with some sorting

SELECT entry, buyprice, sellprice, requiredlevel, requiredskill, requiredskillrank FROM item_template 
WHERE class = 15 AND subclass = 5
AND NAME NOT LIKE '%NPC EQUIP%'
AND entry NOT IN (1132,2411,2414,5655,5656,5665,5668,5864,5872,5873,8563,8588,8591,8592,8595,8629,8631,8632,12325,12326,12327,13321,13322,13323,13324,13331,13332,13333,15277,15290,28481,28927,29220,29221,29222,29743,29744,46099,46100,46308,47100,8586,12302,12303,12330,12351,12353,12354,13317,13326,13327,13328,13329,13334,15292,15293,18766,18767,18768,18772,18773,18774,18776,18777,18778,18785,18786,18787,18788,18789,18790,18791,18793,18794,18795,18796,18797,18798,18902,28482,28936,29223,29224,29745,29746,29747,44223,46102,47101,49044,49046,25470,25471,25472,25474,25475,25476,25473,25477,25527,25528,25529,25531,25532,25533)
ORDER BY
  CASE
  	WHEN NAME LIKE '%Brewfest%' then 'Brewfest'
    WHEN NAME LIKE '%Gladiator%'  then 'Gladiator'
	WHEN NAME LIKE '%Merciless%'  then 'Gladiator'
	WHEN NAME LIKE '%Vengeful%' then 'Gladiator'
	WHEN NAME LIKE '%Vanquisher%' then 'Gladiator'
	WHEN NAME LIKE '%Brutal%' then 'Gladiator'
    WHEN name LIKE '%Kodo%' THEN 'Kodo'
    WHEN name LIKE '%Wolf%' THEN 'Wolf'
    WHEN name LIKE '%Raptor%' THEN 'Raptor'
    WHEN NAME LIKE '%Skeletal%' then 'Skeletal'
    WHEN NAME LIKE '%Ram%' then 'Ram'
    WHEN NAME LIKE '%Elekk%' then 'Elekk'
    WHEN NAME LIKE '%Bear%' then 'Bear'
    WHEN NAME LIKE '%Tiger%' then 'Tiger'
    WHEN NAME LIKE '%Mammoth%' then 'Mammoth'
    WHEN NAME LIKE '%Hawkstrider%' then 'Hawkstrider'
    WHEN NAME LIKE '%Dragonhawk%' then 'Dragonhawk'
    WHEN NAME LIKE '%Hippogryph%' then 'Hippogryph'
    WHEN NAME LIKE '%Talbuk%' then 'Talbuk'
    WHEN NAME LIKE '%Qiraji%' then 'Qiraji'
    WHEN NAME LIKE '%Rocket%' then 'Rocket'
    WHEN NAME LIKE '%Nether Ray' then 'Nether Ray'
    WHEN NAME LIKE '%Netherwing Drake' then 'Netherwing Drake'
    WHEN NAME LIKE '%Nether Drake%' then 'Nether Drake'
    WHEN NAME LIKE '%Proto-Drake' then 'Proto-Drake'
    WHEN NAME LIKE '%Drake' then 'Drake'
    ELSE name
  END,
  name;

*/