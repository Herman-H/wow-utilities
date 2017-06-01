local   has_completed;
local   is_selling_completed;
local   ds_item_count = {};
local   ds_save_count = {};
local   ds_sell_count = {};
local   b_item_count = {};
local   b_need_count = {};
local   b_buy_count = {};
local   is_invalidated;
local   need_to_scan;
local   need_to_split;
local   split_was_performed;
local   number_of_free_slots;
local   free_bags = {};
local   free_slots = {};
local   unable_to_split;
local   unsplit_items = {};
local   has_vendor_menu;
local   has_gossip_menu;
local   locked_items = {};
local   locked_empty_slots = {};
local   locked_sold_items = {};
local   transaction_complete;

local   vendor_count = {};
local   vendor_condition = {};

local QuestStateMap = {
     [348]=931,
    [375]=59,
    [379]=1052,
    [429]=147,
    [703]=741,
    [847]=711,
    [862]=391,
    [879]=387,
    [893]=381,
    [1188]=1029,
    [1218]=869,
    [1511]=356,
    [3561]=1248,
    [4449]=1150,
    [4490]=819,
    [7321]=633,
    [7842]=1115,
    [8419]=0
};

local function playerLevel()         return UnitLevel("player"); end
local function isQuestCompleted(id)  return Test_state > QuestStateMap[id]; end
local function skillOfProfession(profession)
    for idx = 1, GetNumSkillLines() do
        local skillName, isHeader, _, skillRank = GetSkillLineInfo(idx)
        if not isHeader and skillName == profession then
            return skillRank;
        end
    end
    return 0;
end
local function bagSlots()
    local s = 16;
    for i=1,4 do s = s + GetContainerNumSlots(i); end
    return s;
end
local function itemCount(item)
    if ds_item_count[item] ~= nil then return ds_item_count[item]; end
    return 0;
end
local function hasPetLearnedSpell(spell)
    local i = 1
    while true do
        local spellName, spellRank = GetSpellName(i, BOOKTYPE_PET)
        if not spellName then
            return false;
        end
        if spellName .. '(' .. spellRank .. ')' == spell then
            return true;
        end
        i = i + 1
    end
    return false;
end
local function playerGold()
    return GetMoney()*0.0001;
end
vendor_condition[0] = function() return playerLevel() < 16; end
vendor_condition[1] = function() return skillOfProfession("Tailoring") < 85; end
vendor_condition[2] = function() return skillOfProfession("Tailoring") < 85; end
vendor_condition[3] = function() return skillOfProfession("First Aid") < 80; end
vendor_condition[4] = function() return playerLevel() < 13; end
vendor_condition[5] = function() return playerLevel() < 20; end
vendor_condition[6] = function() return playerLevel() < 25; end
vendor_condition[7] = function() return playerLevel() < 33; end
vendor_condition[8] = function() return skillOfProfession("Tailoring") < 125; end
vendor_condition[9] = function() return skillOfProfession("Tailoring") < 125; end
vendor_condition[10] = function() return skillOfProfession("First Aid") < 150; end
vendor_condition[11] = function() return skillOfProfession("First Aid") < 210; end
vendor_condition[12] = function() return skillOfProfession("Tailoring") < 205; end
vendor_condition[13] = function() return skillOfProfession("Tailoring") < 205; end
vendor_condition[14] = function() return skillOfProfession("First Aid") < 260; end
vendor_condition[15] = function() return skillOfProfession("Tailoring") <= 250; end
vendor_condition[16] = function() return true; end
vendor_condition[17] = function() return not isQuestCompleted(4449); end
vendor_condition[18] = function() return not isQuestCompleted(8419); end
vendor_condition[19] = function() return true; end
vendor_condition[20] = function() return true; end
vendor_condition[21] = function() return playerLevel() < 25; end
vendor_condition[22] = function() return true; end
vendor_condition[23] = function() return true; end
vendor_condition[24] = function() return true; end
vendor_condition[25] = function() return true; end
vendor_condition[26] = function() return true; end
vendor_condition[27] = function() return true; end
vendor_condition[28] = function() return true; end
vendor_condition[29] = function() return true; end
vendor_condition[30] = function() return true; end
vendor_condition[31] = function() return true; end
vendor_condition[32] = function() return not isQuestCompleted(429); end
vendor_condition[33] = function() return not isQuestCompleted(879); end
vendor_condition[34] = function() return playerLevel() < 45; end
vendor_condition[35] = function() return playerLevel() < 53; end
vendor_condition[36] = function() return true; end
vendor_condition[37] = function() return true; end
vendor_condition[38] = function() return true; end
vendor_condition[39] = function() return true; end
vendor_condition[40] = function() return not isQuestCompleted(893); end
vendor_condition[41] = function() return not isQuestCompleted(893); end
vendor_condition[42] = function() return not isQuestCompleted(893); end
vendor_condition[43] = function() return not isQuestCompleted(862); end
vendor_condition[44] = function() return not isQuestCompleted(7321) and playerLevel() >= 33; end
vendor_condition[45] = function() return not isQuestCompleted(703) and isQuestCompleted(847); end
vendor_condition[46] = function() return true; end
vendor_condition[47] = function() return not isQuestCompleted(348); end
vendor_condition[48] = function() return not isQuestCompleted(7842); end
vendor_condition[49] = function() return not isQuestCompleted(3561); end
vendor_condition[50] = function() return true; end
vendor_condition[51] = function() return true; end
vendor_condition[52] = function() return true; end
vendor_condition[53] = function() return true; end
vendor_condition[54] = function() return true; end
vendor_condition[55] = function() return true; end
vendor_condition[56] = function() return true; end
vendor_condition[57] = function() return true; end
vendor_condition[58] = function() return true; end
vendor_condition[59] = function() return not isQuestCompleted(379) and isQuestCompleted(1188); end
vendor_condition[60] = function() return true; end
vendor_condition[61] = function() return true; end
vendor_condition[62] = function() return true; end
vendor_condition[63] = function() return true; end
vendor_condition[64] = function() return true; end
vendor_condition[65] = function() return true; end
vendor_condition[66] = function() return true; end
vendor_condition[67] = function() return true; end
vendor_condition[68] = function() return true; end
vendor_condition[69] = function() return true; end
vendor_count[0] = function () return 1; end
vendor_count[1] = function () return 2*(85-skillOfProfession("Tailoring")); end
vendor_count[2] = function () return 4*(85-skillOfProfession("Tailoring"))-2*itemCount("Bolt of Linen Cloth"); end
vendor_count[3] = function () return 16 + (80-skillOfProfession("First Aid"))*4; end
vendor_count[4] = function () return 40; end
vendor_count[5] = function () return 40; end
vendor_count[6] = function () return 40; end
vendor_count[7] = function () return 40; end
vendor_count[8] = function () return 3*(125-skillOfProfession("Tailoring")); end
vendor_count[9] = function () return 9*(125-skillOfProfession("Tailoring"))-3*itemCount("Bolt of Woolen Cloth"); end
vendor_count[10] = function () return 16 + (150-skillOfProfession("First Aid"))*4; end
vendor_count[11] = function () return 16 + (210-skillOfProfession("First Aid"))*4; end
vendor_count[12] = function () return 16*(205-skillOfProfession("Tailoring"))- 4*itemCount("Bolt of Silk Cloth"); end
vendor_count[13] = function () return 4*(205-skillOfProfession("Tailoring")); end
vendor_count[14] = function () return 16 + (260-skillOfProfession("First Aid"))*4; end
vendor_count[15] = function () return 8 + 4*(250-skillOfProfession("Tailoring")); end
vendor_count[16] = function () return 40-itemCount("Heavy Runecloth Bandage") + (300-skillOfProfession("First Aid"))*4; end
vendor_count[17] = function () return 15; end
vendor_count[18] = function () return 1; end
vendor_count[19] = function () return 20; end
vendor_count[20] = function () return 20; end
vendor_count[21] = function () return 5; end
vendor_count[22] = function () return 20; end
vendor_count[23] = function () return 20; end
vendor_count[24] = function () return 20; end
vendor_count[25] = function () return 20; end
vendor_count[26] = function () return 20; end
vendor_count[27] = function () return 20; end
vendor_count[28] = function () return 20; end
vendor_count[29] = function () return 20; end
vendor_count[30] = function () return 20; end
vendor_count[31] = function () return 5; end
vendor_count[32] = function () return 6; end
vendor_count[33] = function () return 41; end
vendor_count[34] = function () return 5; end
vendor_count[35] = function () return 5; end
vendor_count[36] = function () return 5; end
vendor_count[37] = function () return 1; end
vendor_count[38] = function () return 1; end
vendor_count[39] = function () return 1; end
vendor_count[40] = function () return 1; end
vendor_count[41] = function () return 1; end
vendor_count[42] = function () return 1; end
vendor_count[43] = function () return 8; end
vendor_count[44] = function () return 10; end
vendor_count[45] = function () return 4; end
vendor_count[46] = function () return 20; end
vendor_count[47] = function () return 10; end
vendor_count[48] = function () return 10; end
vendor_count[49] = function () return 2; end
vendor_count[50] = function () return 100; end
vendor_count[51] = function () return 10; end
vendor_count[52] = function () return 20; end
vendor_count[53] = function () return 2; end
vendor_count[54] = function () return 20; end
vendor_count[55] = function () return 1; end
vendor_count[56] = function () return 1; end
vendor_count[57] = function () return 10; end
vendor_count[58] = function () return 1; end
vendor_count[59] = function () return 5; end
vendor_count[60] = function () return 100; end
vendor_count[61] = function () return 200; end
vendor_count[62] = function () return 100; end
vendor_count[63] = function () return 100; end
vendor_count[64] = function () return 100; end
vendor_count[65] = function () return 1; end
vendor_count[66] = function () return 100; end
vendor_count[67] = function () return 100; end
vendor_count[68] = function () return 8; end
vendor_count[69] = function () return 10; end
local ds_get_condition_functions = { 
    ["Blood Shard"]={33},
    ["Bolt of Linen Cloth"]={1},
    ["Bolt of Mageweave"]={15,62},
    ["Bolt of Runecloth"]={63},
    ["Bolt of Silk Cloth"]={13},
    ["Bolt of Woolen Cloth"]={8},
    ["Buzzard Wing"]={45},
    ["Charred Razormane Wand"]={41},
    ["Dig Rat"]={43},
    ["Discolored Worg Heart"]={32},
    ["Essence of Undeath"]={67},
    ["Essence of Water"]={66},
    ["Expert First Aid - Under Wraps"]={37},
    ["Felcloth"]={18,60},
    ["Formula: Enchant Weapon - Crusader"]={69},
    ["Goblin Fishing Pole"]={68},
    ["Gorilla Fang"]={47},
    ["Greater Healing Potion"]={25},
    ["Greater Mana Potion"]={26},
    ["Healing Potion"]={24},
    ["Heart of the Wild"]={57},
    ["Heavy Linen Bandage"]={5},
    ["Heavy Wool Bandage"]={7},
    ["Hippogryph Egg"]={58},
    ["Ironweb Spider Silk"]={64},
    ["Jade"]={56},
    ["Lesser Healing Potion"]={22},
    ["Lesser Mana Potion"]={23},
    ["Linen Bandage"]={4},
    ["Linen Cloth"]={2,3},
    ["Long Elegant Feather"]={48},
    ["Mageweave Cloth"]={14},
    ["Major Healing Potion"]={29},
    ["Major Mana Potion"]={30},
    ["Manual: Heavy Silk Bandage"]={38},
    ["Manual: Mageweave Bandage"]={39},
    ["Minor Healing Potion"]={19},
    ["Minor Mana Potion"]={20},
    ["Mithril Casing"]={65},
    ["Morrowgrain"]={51},
    ["Packet of Tharlendris Seeds"]={52},
    ["Razormane Backstabber"]={40},
    ["Razormane War Shield"]={42},
    ["Runecloth"]={16,61},
    ["Scroll of Spirit"]={21},
    ["Scroll of Spirit II"]={34},
    ["Scroll of Spirit III"]={35},
    ["Scroll of Spirit IV"]={36},
    ["Shadow Silk"]={54},
    ["Silk Cloth"]={11,12,17},
    ["Smoldering Wand"]={0},
    ["Superior Healing Potion"]={27},
    ["Superior Mana Potion"]={28},
    ["Thick Leather"]={53},
    ["Truesilver Bar"]={55},
    ["Turtle Meat"]={44},
    ["Un'Goro Soil"]={50},
    ["Undamaged Hippogryph Feather"]={49},
    ["Wastewander Water Pouch"]={59},
    ["Weak Troll's Blood Potion"]={31},
    ["Wildvine"]={46},
    ["Wool Bandage"]={6},
    ["Wool Cloth"]={9,10}
};

vendor_condition[70] = function() return not isQuestCompleted(375); end
vendor_condition[71] = function() return playerLevel() > 15 and skillOfProfession("Tailoring") < 72; end
vendor_condition[72] = function() return skillOfProfession("Tailoring") >= 72; end
vendor_condition[73] = function() return skillOfProfession("Tailoring") < 100 and isQuestCompleted(1511); end
vendor_condition[74] = function() return skillOfProfession("Tailoring") >= 100 and skillOfProfession("Tailoring") < 110; end
vendor_condition[75] = function() return skillOfProfession("Tailoring") >= 100 and skillOfProfession("Tailoring") < 110; end
vendor_condition[76] = function() return skillOfProfession("Tailoring") >= 110 and skillOfProfession("Tailoring") < 125; end
vendor_condition[77] = function() return skillOfProfession("Tailoring") >= 140 and skillOfProfession("Tailoring") < 145; end
vendor_condition[78] = function() return skillOfProfession("Tailoring") < 150 and skillOfProfession("Tailoring") >= 145; end
vendor_condition[79] = function() return skillOfProfession("Tailoring") < 150 and skillOfProfession("Tailoring") >= 145; end
vendor_condition[80] = function() return skillOfProfession("Tailoring") >= 150 and skillOfProfession("Tailoring") < 155; end
vendor_condition[81] = function() return skillOfProfession("Tailoring") >= 150 and skillOfProfession("Tailoring") <= 155; end
vendor_condition[82] = function() return skillOfProfession("Tailoring") < 165 and skillOfProfession("Tailoring") >= 150; end
vendor_condition[83] = function() return skillOfProfession("Tailoring") >= 150 and skillOfProfession("Tailoring") < 165; end
vendor_condition[84] = function() return skillOfProfession("Tailoring") >= 165 and skillOfProfession("Tailoring") < 170; end
vendor_condition[85] = function() return skillOfProfession("Tailoring") >= 170 and skillOfProfession("Tailoring") < 175; end
vendor_condition[86] = function() return skillOfProfession("Tailoring") >= 170 and skillOfProfession("Tailoring") < 175; end
vendor_condition[87] = function() return skillOfProfession("Tailoring") <= 155; end
vendor_condition[88] = function() return skillOfProfession("Tailoring") >= 150 and skillOfProfession("Tailoring") <= 180; end
vendor_condition[89] = function() return skillOfProfession("Tailoring") > 180 and skillOfProfession("Tailoring") < 195; end
vendor_condition[90] = function() return skillOfProfession("Tailoring") > 180 and skillOfProfession("Tailoring") < 195; end
vendor_condition[91] = function() return skillOfProfession("Tailoring") >= 195 and skillOfProfession("Tailoring") < 205; end
vendor_condition[92] = function() return skillOfProfession("Tailoring") >= 195 and skillOfProfession("Tailoring") < 205; end
vendor_condition[93] = function() return skillOfProfession("Tailoring") >= 205 and skillOfProfession("Tailoring") < 220; end
vendor_condition[94] = function() return skillOfProfession("Tailoring") >= 205 and skillOfProfession("Tailoring") < 225; end
vendor_condition[95] = function() return skillOfProfession("Tailoring") >= 205 and skillOfProfession("Tailoring") < 225; end
vendor_condition[96] = function() return skillOfProfession("Tailoring") >= 225 and skillOfProfession("Tailoring") < 230; end
vendor_condition[97] = function() return skillOfProfession("Tailoring") <= 225 and isQuestCompleted(4490); end
vendor_condition[98] = function() return skillOfProfession("Tailoring") <= 230; end
vendor_condition[99] = function() return skillOfProfession("Tailoring") <= 235; end
vendor_condition[100] = function() return skillOfProfession("Tailoring") >= 225 and skillOfProfession("Tailoring") < 235; end
vendor_condition[101] = function() return skillOfProfession("Tailoring") >= 225 and skillOfProfession("Tailoring") < 235; end
vendor_condition[102] = function() return skillOfProfession("Tailoring") >= 235 and skillOfProfession("Tailoring") < 240; end
vendor_condition[103] = function() return skillOfProfession("Tailoring") >= 235 and skillOfProfession("Tailoring")  < 240; end
vendor_condition[104] = function() return skillOfProfession("Tailoring") <= 240; end
vendor_condition[105] = function() return skillOfProfession("Tailoring") <= 245; end
vendor_condition[106] = function() return playerLevel() < 15; end
vendor_condition[107] = function() return playerLevel() > 18 and playerLevel() < 22; end
vendor_condition[108] = function() return playerLevel() > 28 and playerLevel() < 32; end
vendor_condition[109] = function() return GetContainerNumSlots(1) < 8 and playerLevel() >= 12; end
vendor_condition[110] = function() return GetContainerNumSlots(2) < 8 and playerLevel() >= 12; end
vendor_condition[111] = function() return GetContainerNumSlots(3) < 8 and playerLevel() >= 12; end
vendor_condition[112] = function() return GetContainerNumSlots(4) < 8 and playerLevel() >= 12; end
vendor_condition[113] = function() return GetContainerNumSlots(1) < 10 and playerGold() > 10; end
vendor_condition[114] = function() return GetContainerNumSlots(2) < 10 and playerGold() > 12; end
vendor_condition[115] = function() return GetContainerNumSlots(3) < 10 and playerGold() > 14; end
vendor_condition[116] = function() return GetContainerNumSlots(4) < 10 and playerGold() > 16; end
vendor_condition[117] = function() return GetContainerNumSlots(1) < 12 and playerGold() > 30; end
vendor_condition[118] = function() return GetContainerNumSlots(2) < 12 and playerGold() > 40; end
vendor_condition[119] = function() return GetContainerNumSlots(3) < 12 and playerGold() > 50; end
vendor_condition[120] = function() return GetContainerNumSlots(4) < 12 and playerGold() > 60; end
vendor_condition[121] = function() return not isQuestCompleted(893); end
vendor_condition[122] = function() return not isQuestCompleted(893); end
vendor_condition[123] = function() return not isQuestCompleted(893); end
vendor_condition[124] = function() return not hasPetLearnedSpell("Firebolt(Rank 2)") and playerLevel() >= 8 and playerLevel() <= 11; end
vendor_condition[125] = function() return not hasPetLearnedSpell("Sacrifice(Rank 1)") and playerLevel() >= 16 and playerLevel() <= 18; end
vendor_condition[126] = function() return not hasPetLearnedSpell("Consume Shadows(Rank 1)") and playerLevel() >= 16 and playerLevel() <= 18; end
vendor_condition[127] = function() return not hasPetLearnedSpell("Torment(Rank 2)") and playerLevel() >= 19 and playerLevel() <= 23; end
vendor_condition[128] = function() return not hasPetLearnedSpell("Suffering(Rank 1)") and playerLevel() >= 24 and playerLevel() <= 28; end
vendor_condition[129] = function() return not hasPetLearnedSpell("Sacrifice(Rank 2)") and playerLevel() >= 24 and playerLevel() <= 28; end
vendor_condition[130] = function() return not hasPetLearnedSpell("Consume Shadows(Rank 2)") and playerLevel() >= 24 and playerLevel() <= 28; end
vendor_condition[131] = function() return not hasPetLearnedSpell("Torment(Rank 3)") and playerLevel() > 29 and playerLevel() <= 33; end
vendor_condition[132] = function() return not hasPetLearnedSpell("Sacrifice(Rank 3)") and playerLevel() > 32 and playerLevel() <= 35; end
vendor_condition[133] = function() return not hasPetLearnedSpell("Consume Shadows(Rank 3)") and playerLevel() > 32 and playerLevel() <= 35; end
vendor_condition[134] = function() return not hasPetLearnedSpell("Torment(Rank 4)") and playerLevel() >= 40 and playerLevel() <= 44; end
vendor_condition[135] = function() return not hasPetLearnedSpell("Sacrifice(Rank 4)") and playerLevel() >= 40 and playerLevel() <= 44; end
vendor_condition[136] = function() return not hasPetLearnedSpell("Consume Shadows(Rank 3)") and playerLevel() >= 42 and playerLevel() <= 46; end
vendor_condition[137] = function() return not hasPetLearnedSpell("Suffering(Rank 2)") and playerLevel() >= 40 and playerLevel() <= 46; end
vendor_condition[138] = function() return not hasPetLearnedSpell("Sacrifice(Rank 5)") and playerLevel() >= 48 and playerLevel() <= 52; end
vendor_condition[139] = function() return not hasPetLearnedSpell("Suffering(Rank 3)") and playerLevel() >= 48 and playerLevel() <= 52; end
vendor_condition[140] = function() return not hasPetLearnedSpell("Torment(Rank 5)") and playerLevel() >= 50 and playerLevel() <= 54; end
vendor_condition[141] = function() return not hasPetLearnedSpell("Consume Shadows(Rank 5)") and playerLevel() >= 50 and playerLevel() <= 54; end
vendor_condition[142] = function() return not hasPetLearnedSpell("Sacrifice(Rank 6)") and playerLevel() >= 56 and playerLevel() <= 60; end
vendor_condition[143] = function() return not hasPetLearnedSpell("Consume Shadows(Rank 6)") and playerLevel() >= 58 and playerLevel() <= 60; end
vendor_condition[144] = function() return not isQuestCompleted(7321) and playerLevel() >= 33; end
vendor_condition[145] = function() return not isQuestCompleted(1218) and playerLevel() >= 40; end
vendor_count[70] = function () return 1; end
vendor_count[71] = function () return 40; end
vendor_count[72] = function () return (85-skillOfProfession("Tailoring"))*3; end
vendor_count[73] = function () return 2; end
vendor_count[74] = function () return 10 - (skillOfProfession("Tailoring") - 100); end
vendor_count[75] = function () return 10 - (skillOfProfession("Tailoring")-100); end
vendor_count[76] = function () return min(20,30-2*(skillOfProfession("Tailoring")-110)); end
vendor_count[77] = function () return 10 - 2*(145-skillOfProfession("Tailoring")); end
vendor_count[78] = function () return 5-(skillOfProfession("Tailoring")-145); end
vendor_count[79] = function () return 10 - 2*(skillOfProfession("Tailoring")-145); end
vendor_count[80] = function () return 155-skillOfProfession("Tailoring"); end
vendor_count[81] = function () return 2*(155-skillOfProfession("Tailoring")); end
vendor_count[82] = function () return min(10,165-skillOfProfession("Tailoring")); end
vendor_count[83] = function () return min(10,2*(165-skillOfProfession("Tailoring"))); end
vendor_count[84] = function () return 2*(170-skillOfProfession("Tailoring")); end
vendor_count[85] = function () return 175-skillOfProfession("Tailoring"); end
vendor_count[86] = function () return 2*(175-skillOfProfession("Tailoring")); end
vendor_count[87] = function () return 1; end
vendor_count[88] = function () return 1; end
vendor_count[89] = function () return min(20,2*(195-skillOfProfession("Tailoring"))); end
vendor_count[90] = function () return min(10, 2*(195-skillOfProfession("Tailoring"))); end
vendor_count[91] = function () return 2*(205-skillOfProfession("Tailoring")); end
vendor_count[92] = function () return 2*(205-skillOfProfession("Tailoring")); end
vendor_count[93] = function () return 3*(220-skillOfProfession("Tailoring")); end
vendor_count[94] = function () return min(5, 225-skillOfProfession("Tailoring")); end
vendor_count[95] = function () return min(10,2*(225-skillOfProfession("Tailoring"))); end
vendor_count[96] = function () return 2*(230-skillOfProfession("Tailoring")); end
vendor_count[97] = function () return 1; end
vendor_count[98] = function () return 1; end
vendor_count[99] = function () return 1; end
vendor_count[100] = function () return min(10,2*(235-skillOfProfession("Tailoring"))); end
vendor_count[101] = function () return min(10, 2*(235-skillOfProfession("Tailoring"))); end
vendor_count[102] = function () return 240-skillOfProfession("Tailoring") ; end
vendor_count[103] = function () return 240 - skillOfProfession("Tailoring") ; end
vendor_count[104] = function () return 1; end
vendor_count[105] = function () return 1; end
vendor_count[106] = function () return 1; end
vendor_count[107] = function () return 1; end
vendor_count[108] = function () return 1; end
vendor_count[109] = function () return 1; end
vendor_count[110] = function () return 1; end
vendor_count[111] = function () return 1; end
vendor_count[112] = function () return 1; end
vendor_count[113] = function () return 1; end
vendor_count[114] = function () return 1; end
vendor_count[115] = function () return 1; end
vendor_count[116] = function () return 1; end
vendor_count[117] = function () return 1; end
vendor_count[118] = function () return 1; end
vendor_count[119] = function () return 1; end
vendor_count[120] = function () return 1; end
vendor_count[121] = function () return 1; end
vendor_count[122] = function () return 1; end
vendor_count[123] = function () return 1; end
vendor_count[124] = function () return 1; end
vendor_count[125] = function () return 1; end
vendor_count[126] = function () return 1; end
vendor_count[127] = function () return 1; end
vendor_count[128] = function () return 1; end
vendor_count[129] = function () return 1; end
vendor_count[130] = function () return 1; end
vendor_count[131] = function () return 1; end
vendor_count[132] = function () return 1; end
vendor_count[133] = function () return 1; end
vendor_count[134] = function () return 1; end
vendor_count[135] = function () return 1; end
vendor_count[136] = function () return 1; end
vendor_count[137] = function () return 1; end
vendor_count[138] = function () return 1; end
vendor_count[139] = function () return 1; end
vendor_count[140] = function () return 1; end
vendor_count[141] = function () return 1; end
vendor_count[142] = function () return 1; end
vendor_count[143] = function () return 1; end
vendor_count[144] = function () return 1; end
vendor_count[145] = function () return 3; end

local b_get_count = {
};

local b_get_condition_functions = { 
    ["Bleach"]={86},
    ["Blue Dye"]={79,81,90},
    ["Brown Leather Satchel"]={109,110,111,112},
    ["Coarse Thread"]={70,71,72},
    ["Dusk Wand"]={107},
    ["Expert First Aid - Under Wraps"]={121},
    ["Fine Thread"]={73,74,76,77,78,80,82,84,85,89,92},
    ["Gray Dye"]={75,83},
    ["Grimoire of Consume Shadows (Rank 1)"]={126},
    ["Grimoire of Consume Shadows (Rank 2)"]={130},
    ["Grimoire of Consume Shadows (Rank 3)"]={133},
    ["Grimoire of Consume Shadows (Rank 4)"]={136},
    ["Grimoire of Consume Shadows (Rank 5)"]={141},
    ["Grimoire of Consume Shadows (Rank 6)"]={143},
    ["Grimoire of Firebolt (Rank 2)"]={124},
    ["Grimoire of Sacrifice (Rank 1)"]={125},
    ["Grimoire of Sacrifice (Rank 2)"]={129},
    ["Grimoire of Sacrifice (Rank 3)"]={132},
    ["Grimoire of Sacrifice (Rank 4)"]={135},
    ["Grimoire of Sacrifice (Rank 5)"]={138},
    ["Grimoire of Sacrifice (Rank 6)"]={142},
    ["Grimoire of Suffering (Rank 1)"]={128},
    ["Grimoire of Suffering (Rank 2)"]={137},
    ["Grimoire of Suffering (Rank 3)"]={139},
    ["Grimoire of Torment (Rank 2)"]={127},
    ["Grimoire of Torment (Rank 3)"]={131},
    ["Grimoire of Torment (Rank 4)"]={134},
    ["Grimoire of Torment (Rank 5)"]={140},
    ["Heavy Brown Bag"]={113,114,115,116},
    ["Heavy Silken Thread"]={94,96,100,102},
    ["Huge Brown Sack"]={117,118,119,120},
    ["Manual: Heavy Silk Bandage"]={122},
    ["Manual: Mageweave Bandage"]={123},
    ["Orange Dye"]={95},
    ["Pattern: Azure Silk Cloak"]={88},
    ["Pattern: Dark Silk Shirt"]={87},
    ["Pattern: Lavender Mageweave Shirt"]={98},
    ["Pattern: Orange Martial Shirt"]={97},
    ["Pattern: Pink Mageweave Shirt"]={99},
    ["Pattern: Tuxedo Pants"]={105},
    ["Pattern: Tuxedo Shirt"]={104},
    ["Pestilent Wand"]={108},
    ["Pink Dye"]={103},
    ["Purple Dye"]={101},
    ["Red Dye"]={91},
    ["Silken Thread"]={93},
    ["Smoldering Wand"]={106},
    ["Soothing Spices"]={144,145}
};



local function hasCondition(map,item)
    return map[item] ~= nil;
end

local function evaluateCondition(c)
    return vendor_condition[c]();
end

function TestVendor_OnLoad()
    this:RegisterEvent("MERCHANT_SHOW");
    this:RegisterEvent("MERCHANT_CLOSED");
    this:RegisterEvent("ITEM_LOCK_CHANGED");
    this:RegisterEvent("GOSSIP_SHOW");
    this:RegisterEvent("GOSSIP_CLOSED");
end

local function getBag(str)
    local a,b = strfind(str,'%d');
    if a ~= nil and b ~= nil then
        str = strsub(str,a,b);
        return tonumber(str);
    end
end

local function getSlot(str)
    local a,b = strfind(str,'[%d]+');
    if a ~= nil and b ~= nil then
        str = strsub(str,a+1,b);
        return tonumber(str);
    end
end

function TestVendor_EventHandler(event)
    if event == "MERCHANT_SHOW" then has_vendor_menu = true;
    elseif event == "MERCHANT_CLOSED" then has_vendor_menu = false;
    elseif event == "GOSSIP_SHOW" then has_gossip_menu = true;
    elseif event == "GOSSIP_CLOSED" then has_gossip_menu = false;
    elseif event == "ITEM_LOCK_CHANGED" then
        if not transaction_complete then
            local b = false;
            local c = false;
            local d = true;
            for k,v in pairs(locked_items) do
                local bag = getBag(k);
                local slot = getSlot(k);
                local _,_,locked = GetContainerItemInfo(bag,slot);
                if locked == nil then locked = 0;
                else locked = 1; end
                if locked == 1 and locked_items[k] == 2 then
                    locked_items[k] = 1;
                     b = true;
                elseif locked == 0 and locked_items[k] == 1 then
                    locked_items[k] = 0;
                    b = true;
                end
            end
            for k,v in pairs(locked_empty_slots) do
                local bag = getBag(k);
                local slot = getSlot(k);
                if not b and locked_empty_slots[k] == 2 and d then
                    locked_empty_slots[k] = 1; b = true; d=false;
                elseif not b and locked_empty_slots[k] == 1 and d then
                    locked_empty_slots[k] = 0; b = true; d=false;
                end
            end
            if b == false then is_invalidated = true; end
            for k,v in pairs(locked_items) do
                if v ~= 0 then c = true; end
            end
            for k,v in pairs(locked_empty_slots) do
                if v ~= 0 then c = true; end
            end
            if not c then
                transaction_complete = true;
                locked_items = {};
                locked_empty_slots = {};
                locked_sold_items = {};
            end
        else
            is_invalidated = true;
        end
    end
end

function TestVendor_init()
    has_completed = false;
    is_invalidated = false;
    ds_item_count = {};
    ds_save_count = {};
    ds_sell_count = {};
    b_item_count = {};
    b_need_count = {};
    b_buy_count = {};
    need_to_scan = true;
    need_to_split = true;
    need_to_sell = true;
    free_bags = {};
    free_slots = {};
    unable_to_split = false;
    is_selling_completed = false;
    split_was_performed = false;
end

local function scanBags()
    need_to_scan = false;
    number_of_free_slots = 0;
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _, count = GetContainerItemInfo(bag, slot);
            local s = GetContainerItemLink(bag,slot);
            if s~=nil then
                local a,b = strfind(s,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    s = strsub(s,a+1,b);
                    if ds_item_count[s] == nil then
                        ds_item_count[s] = 0;
                    end
                    if b_item_count[s] == nil then
                        b_item_count[s] = 0;
                    end
                    ds_item_count[s] = ds_item_count[s] + count;
                    b_item_count[s] = b_item_count[s] + count;
                end
            else
                number_of_free_slots = number_of_free_slots + 1;
                free_bags[number_of_free_slots] = bag;
                free_slots[number_of_free_slots] = slot;
            end
        end
    end
    for k,_ in pairs(ds_item_count) do
        if hasCondition(ds_get_condition_functions,k) then
            local n = 0;
            for _,v in ipairs(ds_get_condition_functions[k]) do
                if evaluateCondition(v) then
                    local temp = vendor_count[v]();
                    if temp < 0 then temp = 0; end
                    n = n + temp;
                end
            end
            ds_save_count[k] = n;
        end
    end
    for ko,vo in pairs(b_get_condition_functions) do
        local isConditionActive = false;
        local n = 0;
        for ki,vi in pairs(vo) do
            if evaluateCondition(vi) then
                isConditionActive = true;
                local temp = vendor_count[vi]();
                if temp < 0 then temp = 0; end
                n = n + temp;
            end
        end
        if isConditionActive then
            b_need_count[ko] = n;
            if b_item_count[ko] == nil then
                b_item_count[ko] = 0;
            end
        end
    end
    for k,v in pairs(ds_save_count) do
        if ds_save_count[k] > ds_item_count[k] then
            ds_sell_count[k] = 0;
        else
            ds_sell_count[k] = ds_item_count[k] - ds_save_count[k];
        end
    end
    for k,v in pairs(b_need_count) do
        if b_need_count[k] < b_item_count[k] then
            b_buy_count[k] = 0;
        else
            b_buy_count[k] = b_need_count[k] - b_item_count[k];
        end
        if ds_save_count[k] ~= nil then
            ds_sell_count[k] = 0;
        end
    end
end

local function is_slot_after(bag1,slot1,bag2,slot2)
    return ((bag1 > bag2) or (bag1 == bag2 and slot1 > slot2));
end

local function perform_split(bag,slot,sellCount,count)
    ClearCursor();
    for k,v in pairs(free_bags) do
        local b = free_bags[k];
        local s = free_slots[k];
        local count = sellCount;
        if is_slot_after(b,s,bag,slot) then
            count = count-sellCount;
        end
        split_was_performed = false;
        transaction_complete = false;
        locked_items["b" .. bag .. slot] = 2;
        locked_empty_slots["b" .. b .. s] = 2;
        SplitContainerItem(bag,slot,count);
        PickupContainerItem(b,s);
        free_bags[k] = nil;
        free_slots[k] = nil;
        return;
    end
end

local function split()
    local selling = {};
    for k,v in pairs(ds_sell_count) do
        selling[k] = v;
    end
    need_to_split = false;
    split_was_performed = true;
    locked_items = {};
    locked_empty_slots = {};
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _, count = GetContainerItemInfo(bag, slot);
            local s = GetContainerItemLink(bag,slot);
            if s~=nil then
                local a,b = strfind(s,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    s = strsub(s,a+1,b);
                    if selling[s] ~= nil then
                        if selling[s] == 0 then
                        elseif selling[s] - count == 0 then
                            selling[s] = 0;
                        elseif selling[s] - count < 0 then
                            perform_split(bag,slot,selling[s],count);
                            selling[s] = 0;
                        elseif selling[s] - count > 0 then
                            selling[s] = selling[s] - count;
                        end
                    end
                end
            end
        end
    end
end

local function couldBeUpgrade_(quality,reqLvl,type,subtype)
    if quality >= 3 then
        return true;
    elseif type == "Armor" and reqLvl > playerLevel() and quality >= 2 then
        if subtype == "Cloth" then
            return true;
        elseif subtype == "Miscellaneous" then
            return true;
        end
    elseif type == "Weapon" and reqLvl > playerLevel() and quality > 0  then
        if subtype == "Staves" then
            return true;
        elseif subtype == "Wands" then
            return true;
        end
    end
    return false;
end

local function couldBeUpgrade(link)
    local itemId = nil;
    local quality,reqLvl,type_,subtype;
    local b = false;
    if (type(link) == "string") then
        _,_,itemId = string.find(link, "item:(%d+):");
        if(itemId) then
            _,_,quality,reqLvl,type_,subtype = GetItemInfo(itemId);
            b = couldBeUpgrade_(quality,reqLvl,type_,subtype);
        end
    end
    return b;
end

function CouldBeUpgrade(link)
    return couldBeUpgrade(link);
end

local function sell()
    local selling = ds_sell_count;
    locked_sold_items = {};
    is_selling_completed = false;
    need_to_sell = false;
    transaction_complete = true;
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _, count = GetContainerItemInfo(bag, slot);
            local link = GetContainerItemLink(bag,slot);
            if link~=nil then
                local a,b = strfind(link,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    local s = strsub(link,a+1,b);
                    if selling[s] ~= nil then
                        if selling[s] == 0 then
                        elseif selling[s] - count < 0 then
                            unable_to_split = true;
                            selling[s] = 0;
                        elseif selling[s] - count >= 0 then
                            transaction_complete = false;
                            selling[s] = selling[s] - count;
                            UseContainerItem(bag,slot);
                        end
                    elseif b_buy_count[s] ~= nil then
                    elseif couldBeUpgrade(link) then
                    else
                        transaction_complete = false;
                        UseContainerItem(bag,slot);
                    end
                end
            end
        end
    end
end

local function bti(b)
    if b == true then
        return 1;
    else
        return 0;
    end
end

local function buy()
    if has_vendor_menu then
        for vendorindex = 1,GetMerchantNumItems() do
            local name,_,_,quantity,_,_ = GetMerchantItemInfo(vendorindex);
            if b_buy_count[name] ~= nil then
                if b_buy_count[name] ~= 0 then
                    local count = b_buy_count[name] / quantity + bti(math.mod(b_buy_count[name],quantity) ~= 0);
                    BuyMerchantItem(vendorindex,count);
                    b_buy_count[name] = 0;
                end
            end
        end
    end
end

function TestVendor_hasCompleted()
    return has_completed;
end

function TestVendor_SellAndBuy()
    if is_invalidated then TestVendor_init(); end
    if need_to_scan then scanBags(); end
    if need_to_split and not need_to_scan then split(); end
    if not need_to_split and transaction_complete then split_was_performed = true; end
    if has_vendor_menu and need_to_sell and split_was_performed then
        sell();
        buy();
    end
    if not need_to_sell and transaction_complete then is_selling_completed = true; end
    if is_selling_completed and not unable_to_split then has_completed = true;
    elseif is_selling_completed and unable_to_split then TestVendor_init();
    end
end

