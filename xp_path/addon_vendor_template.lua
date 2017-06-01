
local   has_completed;
local   is_selling_completed;
local   dirty_items = {};
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

local function hasCondition(map,item)
    return map[item] ~= nil;
end

local function evaluateCondition(conditionFunc)
    return conditionFunc();
end

local function getItemTargetCount(map,conditionFunc)
    return map[conditionFunc]();
end

function @[TitleVendor]_OnLoad()
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

function @[TitleVendor]_EventHandler(event)
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

function @[TitleVendor]_init()
    has_completed = false;
    is_invalidated = false;
    dirty_items = {};
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
            local _, itemCount = GetContainerItemInfo(bag, slot);
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
                    ds_item_count[s] = ds_item_count[s] + itemCount;
                    b_item_count[s] = b_item_count[s] + itemCount;
                    if dirty_items[s] == nil then
                        dirty_items[s] = true;
                        if hasCondition(ds_get_condition_functions,s) then
                            local n = 0;
                            for _,v in ipairs(ds_get_condition_functions[s]) do
                                if evaluateCondition(v) then
                                    n = n + getItemTargetCount(ds_get_count,v);
                                end
                            end
                            ds_save_count[s] = n;
                        end
                    end
                    dirty_items = {};
                end
            else
                number_of_free_slots = number_of_free_slots + 1;
                free_bags[number_of_free_slots] = bag;
                free_slots[number_of_free_slots] = slot;
            end
        end
    end
    for ko,vo in pairs(b_get_condition_functions) do
        local isConditionActive = false;
        local n = 0;
        for ki,vi in pairs(vo) do
            if evaluateCondition(vi) then
                isConditionActive = true;
                n = n + getItemTargetCount(b_get_count,vi);
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

local function perform_split(bag,slot,sellCount,itemCount)
    ClearCursor();
    for k,v in pairs(free_bags) do
        local b = free_bags[k];
        local s = free_slots[k];
        local count = sellCount;
        if is_slot_after(b,s,bag,slot) then
            count = itemCount-sellCount;
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
            local _, itemCount = GetContainerItemInfo(bag, slot);
            local s = GetContainerItemLink(bag,slot);
            if s~=nil then
                local a,b = strfind(s,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    s = strsub(s,a+1,b);
                    if selling[s] ~= nil then
                        if selling[s] == 0 then
                        elseif selling[s] - itemCount == 0 then
                            selling[s] = 0;
                        elseif selling[s] - itemCount < 0 then
                            perform_split(bag,slot,selling[s],itemCount);
                            selling[s] = 0;
                        elseif selling[s] - itemCount > 0 then
                            selling[s] = selling[s] - itemCount;
                        end
                    end
                end
            end
        end
    end
end

local function sell()
    local selling = ds_sell_count;
    locked_sold_items = {};
    is_selling_completed = false;
    need_to_sell = false;
    transaction_complete = true;
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _, itemCount = GetContainerItemInfo(bag, slot);
            local s = GetContainerItemLink(bag,slot);
            if s~=nil then
                local a,b = strfind(s,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    s = strsub(s,a+1,b);
                    if selling[s] ~= nil then
                        if selling[s] == 0 then
                        elseif selling[s] - itemCount < 0 then
                            unable_to_split = true;
                            selling[s] = 0;
                        elseif selling[s] - itemCount >= 0 then
                            transaction_complete = false;
                            selling[s] = selling[s] - itemCount;
                            UseContainerItem(bag,slot);
                        end
                    elseif b_buy_count[s] ~= nil then
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

local function sell()
    local selling = ds_sell_count;
    locked_sold_items = {};
    is_selling_completed = false;
    need_to_sell = false;
    transaction_complete = true;
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _, itemCount = GetContainerItemInfo(bag, slot);
            local s = GetContainerItemLink(bag,slot);
            if s~=nil then
                local a,b = strfind(s,'%[[^[%]]+');
                if a ~= nil and b ~= nil then
                    s = strsub(s,a+1,b);
                    if selling[s] ~= nil then
                        if selling[s] == 0 then
                        elseif selling[s] - itemCount < 0 then
                            unable_to_split = true;
                            selling[s] = 0;
                        elseif selling[s] - itemCount >= 0 then
                            transaction_complete = false;
                            selling[s] = selling[s] - itemCount;
                            UseContainerItem(bag,slot);
                        end
                    elseif b_buy_count[s] ~= nil then
                    else
                        transaction_complete = false;
                        UseContainerItem(bag,slot);
                    end
                end
            end
        end
    end
end

function @[TitleVendor]_hasCompleted()
    return has_completed;
end

function @[TitleVendor]_SellAndBuy()
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

