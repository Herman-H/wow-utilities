local   has_completed;
local   is_selling_completed;
local   ds_item_count = {};
local   ds_save_count = {};
local   ds_sell_count = {};
local   b_item_count = {};
local   b_need_count = {};
local   b_buy_count = {};
local   b_buy_time = {};
local   number_of_free_slots;
local   free_bags = {};
local   free_slots = {};
local   has_vendor_menu;
local   has_gossip_menu;
local   need_to_scan;
local   sellOrder = {};
local   sellState;
local   sellCompleted;

local   getItemId;

local   vendor_count = {};
local   vendor_condition = {};

local   splitTable = {};

@[QuestStateMap]
local function playerLevel()         return UnitLevel("player"); end
local function isQuestCompleted(id)  return @[Title]_state > QuestStateMap[id]; end
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
        local spellName, spellRank = GetSpellBookItemName(i, BOOKTYPE_PET)
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
@[VendorData]

local function hasCondition(map,item)
    return map[item] ~= nil;
end

local function evaluateCondition(c)
    return vendor_condition[c]();
end

function @[TitleVendor]_OnLoad(self)
    self:RegisterEvent("MERCHANT_SHOW");
    self:RegisterEvent("MERCHANT_CLOSED");
    self:RegisterEvent("ITEM_LOCK_CHANGED");
    self:RegisterEvent("GOSSIP_SHOW");
    self:RegisterEvent("GOSSIP_CLOSED");
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

function @[TitleVendor]_EventHandler(self,event,...)
    if event == "MERCHANT_SHOW" then has_vendor_menu = true;
    elseif event == "MERCHANT_CLOSED" then has_vendor_menu = false;
    elseif event == "GOSSIP_SHOW" then has_gossip_menu = true;
    elseif event == "GOSSIP_CLOSED" then has_gossip_menu = false;
    end
end

function @[TitleVendor]_init()
    has_completed = false;
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
    sellOrder = {};
    sellState = 0;
    sellCompleted = false;
end

local function scanBags()
    need_to_scan = false;
    number_of_free_slots = 0;
    local t = GetTime();
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
        elseif b_buy_time[k] and b_buy_time[k] > t - 5 then
            b_buy_count[k] = 0;
        else
            b_buy_count[k] = b_need_count[k] - b_item_count[k];
        end
        if ds_save_count[k] ~= nil then
            ds_sell_count[k] = 0;
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

getItemId = function(link)
    if (type(link) == "string") then
        _,_,itemId = string.find(link, "item:(%d+):");
        if(itemId) then
            return itemId;
        end
    end
end

local function couldBeUpgrade(link)
    local itemId = nil;
    local quality,reqLvl,type_,subtype;
    local b = false;
    _,_,quality,reqLvl,type_,subtype = GetItemInfo(getItemId(link));
    b = couldBeUpgrade_(quality,reqLvl,type_,subtype);
    return b;
end

function CouldBeUpgrade(link)
    return couldBeUpgrade(link);
end

local function bti(b)
    if b == true then
        return 1;
    else
        return 0;
    end
end

local function buy()
    local t = GetTime();
    if has_vendor_menu then
        for vendorindex = 1,GetMerchantNumItems() do
            local name,_,_,quantity,_,_ = GetMerchantItemInfo(vendorindex);
            if b_buy_count[name] ~= nil then
                if b_buy_count[name] ~= 0 then
                    local count = b_buy_count[name];
                    local stacksize = GetMerchantItemMaxStack(vendorindex);
                    while count > 0 do
                        local amount = math.min(count,stacksize);
                        BuyMerchantItem(vendorindex,amount);
                        count = count - amount;
                    end
                    b_buy_count[name] = 0;
                    b_buy_time[name] = t;
                end
            end
        end
    end
end

local function scanForSells()
    local selling = ds_sell_count;
    sellOrder = {};
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _, count, _, _, _, _, _, _, noValue = GetContainerItemInfo(bag, slot);
            local link = GetContainerItemLink(bag,slot);
            if link~=nil then
                local a,b = strfind(link,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    local s = strsub(link,a+1,b);
                    if selling[s] ~= nil then
                        if selling[s] == 0 then
                        elseif selling[s] - count < 0 then
                            selling[s] = 0;
                        elseif selling[s] - count >= 0 then
                            selling[s] = selling[s] - count;
                            tinsert(sellOrder, {Bag=bag;Slot=slot;});
                        end
                    elseif b_buy_count[s] ~= nil then
                    elseif couldBeUpgrade(link) then
                    elseif noValue then
                    else
                        tinsert(sellOrder, {Bag=bag;Slot=slot;});
                    end
                end
            end
        end
    end
end

local function currentBagState()
    local bagState = {};
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _, count = GetContainerItemInfo(bag, slot);
            local s = GetContainerItemLink(bag,slot);
            if s~=nil then
                local a,b = strfind(s,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    s = strsub(s,a+1,b);
                    bagState[bag .. slot] = {Item=s;Count=count;};
                else
                    bagState[bag .. slot] = {Item=nil;Count=0;};
                end
            else
                bagState[bag .. slot] = {Item=nil;Count=0;};
            end
        end
    end
    return bagState;
end

local function successfulSellState()
    local bagState = currentBagState();
    for k,v in pairs(sellOrder) do
        sellIdx = v.Bag .. v.Slot;
        bagState[sellIdx].Count = 0;
        bagState[sellIdx].Item = nil;
    end
    return bagState;
end

local function doSell(i,N)
    if i > N then return; end
    if sellOrder[i] then
        local itemAt = sellOrder[i];
        UseContainerItem(itemAt.Bag,itemAt.Slot);
        doSell(i+1,N);
        tremove(sellOrder,i);
    end
    DEFAULT_CHAT_FRAME:AddMessage("Sold items, " .. getn(sellOrder) .. " remaining.");
end

local function take(free)
    for k,v in pairs(free) do
        return_val = v;
        free[k] = nil;
        return return_val;
    end
end

local function makeSplit(free,bag,slot,itemName,count)
    if getn(free) > 0 then
        freeBagSlot = take(free);
        tinsert(splitTable,
        {
            Item=itemName;
            Count=count;
            From=
            {
                Bag=bag;
                Slot=slot;
            };
            To=
            {
                Bag=freeBagSlot.Bag;
                Slot=freeBagSlot.Slot;
            };
        });
        return true;
    else
        return false;
    end
end

local function scanForSplits()
    local free = {};
    for k,v in pairs(free_bags) do
        free[k] = {Bag=v;Slot=free_slots[k];};
    end
    local selling = {};
    for k,v in pairs(ds_sell_count) do
        selling[k] = v;
    end
    splitTable = {};

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
                            splitPossible = makeSplit(free,bag,slot,s,count - selling[s]);
                            if (not splitPossible) then
                                return false;
                            end
                            selling[s] = 0;
                        elseif selling[s] - count > 0 then
                            selling[s] = selling[s] - count;
                        end
                    end
                end
            end
        end
    end
    return true;
end

local function successfulSplitState()
    local bagState = currentBagState();
    for k,v in pairs(splitTable) do
        fromIdx = v.From.Bag .. v.From.Slot;
        toIdx = v.To.Bag .. v.To.Slot;
        bagState[fromIdx].Count = bagState[fromIdx].Count - v.Count;
        if (bagState[fromIdx].Count <= 0) then
            bagState[fromIdx].Item = nil;
        end
        bagState[toIdx].Count = bagState[toIdx].Count + v.Count;
        bagState[toIdx].Item = v.Item;
    end
    return bagState;
end

local function doSplit(i,N)
    if i >= N then return; end
    ClearCursor();
    if splitTable[i] then
        v = splitTable[i];
        SplitContainerItem(v.From.Bag,v.From.Slot,v.Count);
        PickupContainerItem(v.To.Bag,v.To.Slot);
        doSplit(i+1,N);
        tremove(splitTable, i);
    end
end

local toBagState = {};

local function triggerSellStateMachine()
    if sellState == 0 then -- START
        scanForSplits();
        if getn(splitTable) > 0 then
            toBagState = successfulSplitState();
            sellState = 2;
            DEFAULT_CHAT_FRAME:AddMessage("Splitting " .. getn(splitTable) .. " stacks...");
        else
            scanForSells();
            sellState = 1;
            DEFAULT_CHAT_FRAME:AddMessage("No splitting required, selling.");
        end
    end
    if sellState == 1 then -- SELL
        if has_vendor_menu then doSell(1,4); end
        if getn(sellOrder) == 0 then
            sellState = 3;
        end
    end
    if sellState == 2 then -- SPLIT
        doSplit(1,4);
        if getn(splitTable) == 0 then
            sellState = 4;
        end
    end
    if sellState == 3 then -- WAIT FOR SELLING COMPLETION
        local bagState = currentBagState();
        local done = true;
        for k,v in pairs(toBagState) do
            w = bagState[k];
            if v.Item ~= w.Item or v.Count ~= w.Count then
                done = false;
            end
        end
        if done == true then
            scanForSplits();
            if getn(splitTable) > 0 then
                sellState = 2;
            else
                sellState = 5;
            end
        end
    end
    if sellState == 4 then -- WAIT FOR SPLITTING COMPLETION
        local bagState = currentBagState();
        local done = true;
        for k,v in pairs(toBagState) do
            w = bagState[k];
            if v.Item ~= w.Item or v.Count ~= w.Count then
                done = false;
            end
        end
        if done == false then
            scanForSplits();
            if getn(splitTable) > 0 then
                sellState = 2;
            else
                done = true;
            end
        end
        if done == true then
            scanForSells();
            if getn(sellOrder) > 0 then
                sellState = 1;
            else
                sellState = 5;
            end
        end
    end
    if sellState == 5 and has_vendor_menu then -- BUY
        buy();
        sellCompleted = true;
        has_vendor_menu = false;
        CloseMerchant();
    end
end

function @[TitleVendor]_hasCompleted()
    return has_completed;
end

function @[TitleVendor]_SellAndBuy()
    if need_to_scan then scanBags(); end
    if not sellCompleted then
        triggerSellStateMachine();
    end
    if sellCompleted then has_completed = true;
    end
end

function @[TitleVendor]_Debug()
    for idx = 0,getn(vendor_count) do
        local fn = vendor_count[idx];
        fn();
    end
    for idx = 0,getn(vendor_count) do
        local fn = vendor_count[idx];
        fn();
    end
    for k,v in ipairs(QuestStateMap) do
        if v == 0 then
            DEFAULT_CHAT_FRAME:AddMessage("Quest with id '" .. k .. "' is never encountered, but has active conditions for it.");
        end
    end
end