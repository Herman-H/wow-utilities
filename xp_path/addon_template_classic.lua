
@[Title]_is_loaded = false;
@[Title]_gossip_shown = 0;
@[Title]_greeted = 0;
@[Title]_quest_shown = 0;
@[Title]_quest_progress_shown = 0;
@[Title]_quest_completed_shown = 0;
@[Title]_taxi_map_shown = 0;
@[Title]_trainer_menu_shown = 0;
@[Title]_bank_frame_shown = 0;
local tryProgressPickupQuestState;
local tryProgressReturnQuestState;
local tryProgressSetHearthstoneState;
local tryProgressUseHearthstoneState;
local tryProgressDiscoverFPState;
local tryProgressTakeTaxiState;
local tryProgressVendorState;
local tryProgressTrainerState;
local tryProgressBankTransaction;
local checkIfNextStateForPickupQuests;
local checkIfNextStateForReturnQuests;
local checkIfNextStateForSetHearthstone;
local checkIfNextStateForUseHearthstone;
local checkIfNextStateForDiscoverFP;
local checkIfNextStateForTakeTaxi;
local checkIfNextStateForVendor;
local checkIfNextStateForTrainer;
local checkIfXPBreakpointReached;
local nextComment;
local getNextState;

local filterToBank;
local filterFromBank;
local filterFunctionTo;
local filterFunctionFrom;
local atLeastRemaining;
local atLeastPush;
local pushAll;
local bankTransaction;


local function printComment(comment)
    DEFAULT_CHAT_FRAME:AddMessage(comments[comment])
end

local function get_req_xp(value)
    return math.fmod(value,300000);
end

local function get_req_level(value)
    return math.floor(value/300000);
end

local function printState(state)
    local t = types[state];
    local extra = extras[state];
    local name = names[state];
    local quest = questtitles[state];
    local questreward = questrewards[state];
    local s = "|cFF00FF00State " .. state .. ":|r ";
    if t == 0 then
        s = s .. "Pick up quest ";
        s = s .. quest .. " from ";
        if extra == 0 then
            s = s .. "questgiver ";
            s = s .. name .. ".";
        elseif extra == 1 then
            s = s .. "item ";
            s = s .. name .. ".";
        elseif extra == 2 then
            s = s .. "a gameobject.";
        end
    elseif t == 1 then
        s = s .. "Return quest ";
        s = s .. quest .. " to ";
        if extra == 0 then
            s = s .. "questgiver ";
            s = s .. name;
        elseif extra == 2 then
            s = s .. "a gameobject";
        end
        if questreward ~= nil then
            s = s .. " and pick up " .. questreward;
        end
        s = s .. ".";
    elseif t == 2 then
        s = s .. "Set hearthstone to innkeeper " .. name .. " in " .. quest .. ".";
    elseif t == 3 then
        s = s .. "Use hearthstone to " .. name .. ".";
    elseif t == 4 then
        s = s .. "Discover node " .. quest .. " at flight master " .. name .. ".";
    elseif t == 5 then
        s = s .. "Take flight from flight master " .. name .. " towards " .. quest .. ".";
    elseif t == 6 then
        s = s .. "Use vendor " .. name .. ".";
    elseif t == 7 then
        s = s .. "Use trainer " .. name .. ".";
    elseif t == 8 then
        s = comments[extra];
    elseif t == 9 then
        s = s .. "Reach " .. get_req_xp(extra) .. " into level " .. get_req_level(extra) .. ".";
    elseif t == 10 then
        s = s .. "Use banker " .. name .. " to ";
        local transaction = transactions[extra];
        if transaction.To then
            s = s .. "deposit ";
            for idx,v in ipairs(transaction.To) do
                if v.Function == 0 then
                    s = s .. "all ";
                elseif v.Function == 1 then
                    s = s .. "all but " .. v.Arg .. " ";
                end
                s = s .. "[" .. v.Item .. "]";
                if idx ~= getn(transaction.To) then s = s .. ", "; end
            end
            if transaction.From then s = s .. " and " end
        end
        if transaction.From then
            s = s .. "withdraw ";
            for _,v in ipairs(transaction.From) do
                if v.Function == 0 then
                    s = s .. "all ";
                elseif v.Function == 1 then
                    s = s .. "at least " .. v.Arg .. " ";
                end
                s = s .. "[" .. v.Item .. "]";
                if idx ~= getn(transaction.From) then s = s .. ", "; end
            end
        end
    end
    DEFAULT_CHAT_FRAME:AddMessage(s)
end


function @[Title]_OnLoad(self)
    @[TitleSlashCmd];
    @[TitleSlashCmdRegister];
    self:RegisterEvent("ADDON_LOADED");
    self:RegisterEvent("GOSSIP_SHOW");
    self:RegisterEvent("GOSSIP_CLOSED");
    self:RegisterEvent("QUEST_PROGRESS");
    self:RegisterEvent("QUEST_COMPLETE");
    self:RegisterEvent("QUEST_DETAIL");
    self:RegisterEvent("QUEST_FINISHED");
    self:RegisterEvent("QUEST_GREETING");
    self:RegisterEvent("CONFIRM_BINDER");
    self:RegisterEvent("TAXIMAP_OPENED");
    self:RegisterEvent("TAXIMAP_CLOSED");
    self:RegisterEvent("TRAINER_SHOW");
    self:RegisterEvent("TRAINER_CLOSED");
    self:RegisterEvent("CHAT_MSG_SYSTEM");
    self:RegisterEvent("UI_INFO_MESSAGE");
    self:RegisterEvent("BANKFRAME_OPENED");
    self:RegisterEvent("BANKFRAME_CLOSED");
    if DEFAULT_CHAT_FRAME then DEFAULT_CHAT_FRAME:AddMessage("@[Title] was loaded.") end
end

local function init()
    if @[Title]_state == nil then
        @[Title]_state = 1;
    end
    if @[Title]_comment == nil then
        @[Title]_comment = 1;
    end
    @[Title]_is_loaded = true;
    if types[@[Title]_state] == 6 then
        @[TitleVendor]_init();
    end
    printState(@[Title]_state);
end

function @[Title]_EventHandler(self,event,...)
    if event == "ADDON_LOADED" and select(1,...) == "@[Title]" then
        init();
    elseif event == "GOSSIP_SHOW" then
        @[Title]_gossip_shown = 1;
        @[Title]_greeted = 0;
        @[Title]_quest_shown = 0;
    elseif event == "QUEST_GREETING" then
        @[Title]_gossip_shown = 0;
        @[Title]_greeted = 1;
        @[Title]_quest_shown = 0;
    elseif event == "GOSSIP_CLOSED" then
        @[Title]_gossip_shown = 0;
    elseif event == "QUEST_FINISHED" then
        @[Title]_gossip_shown = 0;
        @[Title]_quest_progress_shown = 0;
        @[Title]_quest_completed_shown = 0;
        @[Title]_quest_shown = 0;
    elseif event == "QUEST_DETAIL" then
        @[Title]_quest_shown = 1;
    elseif event == "QUEST_PROGRESS" then
        @[Title]_quest_progress_shown = 1;
    elseif event == "QUEST_COMPLETE" then
        @[Title]_quest_completed_shown = 1;
    elseif event == "CONFIRM_BINDER" then
        ConfirmBinder();
    elseif event == "TAXIMAP_OPENED" then
        @[Title]_taxi_map_shown = 1;
    elseif event == "TAXIMAP_CLOSED" then
        @[Title]_taxi_map_shown = 0;
        checkIfNextStateForTakeTaxi();
    elseif event == "TRAINER_SHOW" then
        @[Title]_trainer_menu_shown = 1;
    elseif event == "TRAINER_CLOSED" then
        @[Title]_trainer_menu_shown = 0;
    elseif event == "CHAT_MSG_SYSTEM" then
    elseif event == "UI_INFO_MESSAGE" then
        if select(2,...) == ERR_NEWTAXIPATH then
            checkIfNextStateForDiscoverFP();
        end
    elseif event == "BANKFRAME_OPENED" then
        @[Title]_bank_frame_shown = 1;
    elseif event == "BANKFRAME_CLOSED" then
        @[Title]_bank_frame_shown = 0;
    end
end

local function selectQuestWithNameGossipX(name,...)
    for i = 1, select('#', ...) do
        msg = select(i, ...)
        if math.fmod(i-1,7) == 0 then
            if name == msg then
                return (i/7) + 1;
            end
        end
    end
    return nil;
end

local function selectQuestWithNameGossip(name)
    local index = selectQuestWithNameGossipX(name,GetGossipAvailableQuests());
    if index ~= nil then
        SelectGossipAvailableQuest(index);
    end
end

local function selectQuestWithNameGreetingX(name)
    for i = 1, GetNumAvailableQuests() do
        if name == GetAvailableTitle(i) then
            return i;
        end
    end
    return;
end

local function selectQuestWithNameGreeting(name)
    local index = selectQuestWithNameGreetingX(name);
    if index ~= nil then
        SelectAvailableQuest(index);
    end
end

local function getQuestFromItem(itemname)
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            s=GetContainerItemLink(bag,slot);
            if s==nil then s="a" end
            a,b = strfind(s,'%[[^[%]]+');
            if a ~= nil and b ~= nil then
                if itemname == strsub(s,a+1,b) then
                    UseContainerItem(bag,slot);
                    return;
                end
            end
        end
    end
end

local function returnQuestGossipX(name,...)
    for i = 1, select('#', ...) do
    	msg = select(i, ...)
        if math.fmod(i-1,7) == 0 then
            if name == msg then
                return (i/7) + 1;
            end
        end
    end
    return nil;
end

local function returnQuestGossip(name)
    local index = returnQuestGossipX(name,GetGossipActiveQuests());
    if index ~= nil then
        SelectGossipActiveQuest(index);
    end
end

local function returnQuestGreetingX(name)
    for i = 1,GetNumActiveQuests() do
        if name == GetActiveTitle(i) then
            return i;
        end
    end
    return nil;
end

local function returnQuestGreeting(name)
    local index = returnQuestGreetingX(name);
    if index ~= nil then
        SelectActiveQuest(index);
    end
end

local function acceptQuest(name)
    if GetTitleText() == name then
        AcceptQuest();
    else
        DeclineQuest();
    end
end

local function progressQuest(name)
    if GetTitleText() == name then
        CompleteQuest();
    else
        DeclineQuest();
    end
end

local function completeQuest(name)
    if GetTitleText() == name then
        if questrewards[@[Title]_state] ~= nil then
            for a=1,GetNumQuestChoices() do
                local choice,_,_,_,_ = GetQuestItemInfo("choice",a);
                if choice == questrewards[@[Title]_state] then
                    GetQuestReward(a);
                    return;
                end
            end
        else
            QuestRewardCompleteButton_OnClick();
            return;
        end
    end
    DeclineQuest();
end

local function hasQuestInQuestLog(questname)
    n = GetNumQuestLogEntries();
    for i = 1,n do
        if questname == GetQuestLogTitle(i) then
            return true;
        end
    end
    return false;
end

local function getGossipOption(name,...)
    for i = 1, select('#', ...) do
    	msg = select(i, ...)
        if math.fmod(i,2) == 0 then
            print(msg)
            if name == msg then
                return (i/2);
            end
        end
    end
    return nil;
end

local function setHearthstone()
    index = getGossipOption("binder",GetGossipOptions());
    if index ~= nil then
        SelectGossipOption(index)
    end
end

local function pickTaxiOption()
    index = getGossipOption("taxi",GetGossipOptions());
    if index ~= nil then
        SelectGossipOption(index)
    end
end

local function takeTaxiNode(name)
    local index = nil;
    for i=1,NumTaxiNodes() do
        if TaxiNodeName(i) == name then
            index = i;
        end
    end
    if index ~= nil then
        TakeTaxiNode(index);
    end
end

local function advance()
    if not @[Title]_is_loaded then
        DEFAULT_CHAT_FRAME:AddMessage("Addon was not loaded.")
        return;
    end
    if @[Title]_state > size then
        DEFAULT_CHAT_FRAME:AddMessage("Sequence completed.")
        return;
    end
    local type = types[@[Title]_state];
    local extra = extras[@[Title]_state];
    local name = names[@[Title]_state];
    local questtitle = questtitles[@[Title]_state];
    if UnitName("target") ~= name and (type ~= 0 or (type == 0 and extra == 0)) then
        --TargetUnit(name);
        print("Target: " .. name);
        CloseGossip(name);
    end
    if type == 0 then
        tryProgressPickupQuestState(extra,name,questtitle);
        checkIfNextStateForPickupQuests();
    elseif type == 1 then
        tryProgressReturnQuestState(name,questtitle);
        checkIfNextStateForReturnQuests();
    elseif type == 2 then
        tryProgressSetHearthstoneState();
        checkIfNextStateForSetHearthstone();
    elseif type == 3 then
        tryProgressUseHearthstoneState();
        checkIfNextStateForUseHearthstone();
    elseif type == 4 then
        tryProgressDiscoverFPState();
    elseif type == 5 then
        tryProgressTakeTaxiState();
    elseif type == 6 then
        tryProgressVendorState();
        checkIfNextStateForVendor();
    elseif type == 7 then
        checkIfNextStateForTrainer();
        tryProgressTrainerState();
    elseif type == 8 then
        nextComment();
    elseif type == 9 then
        checkIfXPBreakpointReached();
    elseif type == 10 then
        tryProgressBankTransaction(extra);
    end
end

tryProgressPickupQuestState = function(extra,name,questname)
    if @[Title]_quest_shown == 0 then
        if extra == 0 or extra == 2 then      -- The quest is on an npc or gameobject
            if @[Title]_gossip_shown == 1 then
                selectQuestWithNameGossip(questname);
            elseif @[Title]_greeted == 1 then
                selectQuestWithNameGreeting(questname);
            end
        elseif extra == 1 then  -- The quest is on an item
            DEFAULT_CHAT_FRAME:AddMessage("The quest is on the item " .. name .. ", try to activate it.")
            getQuestFromItem(name);
        end
    else
        acceptQuest(questname);
    end
end

tryProgressReturnQuestState = function(name,questname)
    if @[Title]_quest_progress_shown == 0 and @[Title]_quest_completed_shown == 0 then
        if @[Title]_gossip_shown == 1 then
            returnQuestGossip(questname);
        elseif @[Title]_greeted == 1 then
            returnQuestGreeting(questname);
        end
    elseif @[Title]_quest_completed_shown == 1 then
        completeQuest(questname);
    elseif @[Title]_quest_progress_shown == 1 then
        progressQuest(questname);
    end
end

tryProgressSetHearthstoneState = function()
    if @[Title]_gossip_shown == 1 then
        setHearthstone();
        ConfirmBinder();
    end
end

tryProgressUseHearthstoneState = function (name)
    UseContainerItem(0,1);
end

tryProgressDiscoverFPState = function()
end

tryProgressTakeTaxiState = function()
    if @[Title]_gossip_shown == 1 then
        pickTaxiOption();
    elseif @[Title]_taxi_map_shown == 1 then
        local name = questtitles[@[Title]_state];
        takeTaxiNode(name);
        checkIfNextStateForTakeTaxi();
    end
end

tryProgressVendorState = function()
    if @[Title]_gossip_shown == 1 then
        local index = getGossipOption("vendor",GetGossipOptions());
        if index ~= nil then
            SelectGossipOption(index)
        end
    end
    @[TitleVendor]_SellAndBuy();
end

@[TitleTrainerData]

tryProgressTrainerState = function()
    if @[Title]_trainer_menu_shown == 0 then
        local index = getGossipOption("trainer",GetGossipOptions());
        if index ~= nil then
            SelectGossipOption(index)
        end
    end
    if @[Title]_trainer_menu_shown == 1 then
        SetTrainerServiceTypeFilter("available",1);
        SetTrainerServiceTypeFilter("unavailable",0);
        SetTrainerServiceTypeFilter("used",0);
        local services_available = GetNumTrainerServices();
        for i=1,services_available do
            name,rank,h = GetTrainerServiceInfo(i);
            if h ~= "header"  then
                local fullName = name;
                if rank ~= nil and strlen(rank) > 0 then fullName = fullName .. '(' .. rank .. ')'; end
                local d = trainingTable[fullName];
                if (not d or UnitLevel("player") > d) then
                    BuyTrainerService(i);
                    return;
                end
            end
        end
    end
end

tryProgressBankTransaction = function(idx)
    if @[Title]_bank_frame_shown == 1 then
        local transaction = transactions[idx];
        local to_bank = filterToBank(transaction.To);
        local from_bank = filterFromBank(transaction.From);
        bankTransaction(to_bank,from_bank);
        getNextState();
    end
end

checkIfNextStateForPickupQuests = function()
    local state = @[Title]_state;
    local type = types[state];
    local quest = questtitles[state];
    local hasQuest = hasQuestInQuestLog(quest);
    if type == 0 and hasQuest then
        getNextState();
    end
end

checkIfNextStateForReturnQuests = function()
    local state = @[Title]_state;
    local type = types[state];
    local quest = questtitles[state];
    local hasQuest = hasQuestInQuestLog(quest);
    if type == 1 and not hasQuest then
        getNextState();
    end
end

checkIfNextStateForSetHearthstone = function()
    local state = @[Title]_state;
    local type = types[state];
    local loc = questtitles[state];
    local isLoc = loc == GetBindLocation();
    if type == 2 and isLoc then
        getNextState();
    end
end

checkIfNextStateForUseHearthstone = function()
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            s=GetContainerItemLink(bag,slot);
            if s==nil then s="a" end
            a,b = strfind(s,'%[[^[%]]+');
            if a ~= nil and b ~= nil then
                if "Hearthstone" == strsub(s,a+1,b) then
                    _, duration, _ = GetContainerItemCooldown(bag, slot);
                    if duration == 0 then
                        getNextState();
                    end
                    return;
                end
            end
        end
    end
end

checkIfNextStateForDiscoverFP = function()
    local state = @[Title]_state;
    local type = types[state];
    if type == 4 then
        getNextState();
    end
end

checkIfNextStateForTakeTaxi = function()
    local state = @[Title]_state;
    local type = types[state];
    if type == 5 then
        getNextState();
    end
end

checkIfNextStateForVendor = function()
    if @[TitleVendor]_hasCompleted() then
        getNextState();
        CloseMerchant();
    end
end

checkIfNextStateForTrainer = function()
    if @[Title]_trainer_menu_shown == 1 then
        SetTrainerServiceTypeFilter("available",1);
        SetTrainerServiceTypeFilter("unavailable",0);
        SetTrainerServiceTypeFilter("used",0);
        local services_available = GetNumTrainerServices();
        if services_available == 0 then
            getNextState();
            CloseTrainer();
        else
            for i=1,services_available do
                name,rank,h = GetTrainerServiceInfo(i);
                if h ~= "header" then
                    local fullName = name;
                    if rank ~= nil and strlen(rank) > 0 then fullName = fullName .. '(' .. rank .. ')'; end
                    local d = trainingTable[fullName];
                    if (not d or UnitLevel("player") > d) then
                        return;
                    end
                end
            end
            getNextState();
            CloseTrainer();
        end
    end
end

checkIfXPBreakpointReached = function()
    local extra = extras[@[Title]_state];
    local level = get_req_level(extra);
    local xp = get_req_xp(extra);
    
    local current_level = UnitLevel("player");
    local current_xp = UnitXP("player");
    
    if current_level > level or (current_level == level and current_xp >= xp) then
        getNextState();
    end
    
end

nextComment = function()
    @[Title]_comment = extras[@[Title]_state];
    getNextState();
end

getNextState = function()
    @[Title]_state = @[Title]_state + 1;
    printState(@[Title]_state);
    if types[@[Title]_state] == 6 then
        @[TitleVendor]_init();
    end
end

local function concat_tables(into,from)
    for _,v in ipairs(from) do
        tinsert(into,v);
    end
end

filterFunctionTo = function(fn,param)
    if fn == 0 then
        return pushAll;
    elseif fn == 1 then
        return atLeastRemaining(param);
    end
end
filterFunctionFrom = function(fn,param)
    if fn == 0 then
        return pushAll;
    elseif fn == 1 then
        return atLeastPush(param);
    end
end

filterToBank = function(filterList)
    if not filterList then return nil; end
    local itemList = {};
    for bag = 0,4 do
        for slot = 1,GetContainerNumSlots(bag) do
            local _,count=GetContainerItemInfo(bag,slot);
            local s = GetContainerItemLink(bag,slot);
            if s then
                local a,b = strfind(s,'%[[^[%]]+');
                if a then
                    local item = strsub(s,a+1,b);
                    if itemList[item] then
                        (itemList[item])[getn(itemList[item])+1] = {Bag=bag;Slot=slot;Count=count;};
                    else
                        itemList[item] = {[1] = {Bag=bag;Slot=slot;Count=count;}};
                    end
                end
            end
        end
    end
    local slotList = {};
    for _,filter in ipairs(filterList) do
        if itemList[filter.Item] then
            concat_tables(slotList,(filterFunctionTo(filter.Function,filter.Arg))(itemList[filter.Item]));
        end
    end
    
    return slotList;
end

filterFromBank = function(filterList)
    if not filterList then return nil; end
    local itemList = {};
    local bankbags = {-1,5,6,7,8,9,10};
    for _,bag in ipairs(bankbags) do
        for slot = 1,GetContainerNumSlots(bag) do
            local _,count=GetContainerItemInfo(bag,slot);
            local s = GetContainerItemLink(bag,slot);
            if s then
                local a,b = strfind(s,'%[[^[%]]+');
                if a then
                    local item = strsub(s,a+1,b);
                    if itemList[item] then
                        (itemList[item])[getn(itemList[item])+1] = {Bag=bag;Slot=slot;Count=count;};
                    else
                        itemList[item] = {[1] = {Bag=bag;Slot=slot;Count=count;}};
                    end
                end
            end
        end
    end
    local slotList = {};
    for _,filter in ipairs(filterList) do
        if itemList[filter.Item] then
            concat_tables(slotList,(filterFunctionFrom(filter.Function,filter.Arg))(itemList[filter.Item]));
        end
    end
    
    return slotList;
end

atLeastRemaining = function(N)
    return function(slotData)
        -- return the slots such that the least number of slots in the bags remain with N items
        -- don't split any stacks here
        local countRemain = 0;
        local saveIdx = {};
        sort(slotData,function(a,b) return a.Count > b.Count end);
        for i=1,getn(slotData) do
            local v = slotData[i];
            if countRemain + v.Count > N then
                local bestFitIdx = i;
                for j=i+1,getn(slotData) do
                    local v1 = slotData[j];
                    if countRemain + v1.Count == N then
                        bestFitIdx = j;
                        break;
                    elseif countRemain + v1.Count > N then
                        bestFitIdx = j;
                    end
                end
                saveIdx[bestFitIdx] = true;
                countRemain = countRemain + slotData[bestFitIdx].Count;
                break;
            elseif countRemain + v.Count == N then
                saveIdx[i] = true;
                countRemain = countRemain + slotData[i].Count;
                break;
            elseif countRemain + v.Count < N then
                saveIdx[i] = true;
                countRemain = countRemain + slotData[i].Count;
            end
        end
        
        local returnSet = {};
        for i=1,getn(slotData) do
            if not saveIdx[i] then
                tinsert(returnSet,slotData[i]);
            end
        end
        
        return returnSet;
    end
end

atLeastPush = function(N)
    return function(slotData)
        local countPushed = 0;
        local returnSet = {};
        sort(slotData,function(a,b) return a.Count > b.Count end);
        for i=1,getn(slotData) do
            local v = slotData[i];
            if countPushed + v.Count > N then
                local bestFitIdx = i;
                for j=i+1,getn(slotData) do
                    local v1 = slotData[j];
                    if countPushed + v1.Count == N then
                        bestFitIdx = j;
                        break;
                    elseif countPushed + v1.Count > N then
                        bestFitIdx = j;
                    end
                end
                tinsert(returnSet,slotData[bestFitIdx]);
                countPushed = countPushed + slotData[bestFitIdx].Count;
                break;
            elseif countPushed + v.Count == N then
                tinsert(returnSet,slotData[i]);
                countPushed = countPushed + slotData[i].Count;
                break;
            elseif countPushed + v.Count < N then
                tinsert(returnSet,slotData[i]);
                countPushed = countPushed + slotData[i].Count;
            end
        end
        return returnSet;
    end
end

pushAll = function(slotData)
    return slotData;
end

bankTransaction = function(toList,fromList)
    if toList then
        for _,v in ipairs(toList) do
            UseContainerItem(v.Bag,v.Slot);
        end
    end
    if fromList then
        for _,v in ipairs(fromList) do
            UseContainerItem(v.Bag,v.Slot);
        end
    end
end

function @[Title]_AdvanceX(cmd)
    if cmd == "advance" then
        advance();
    elseif cmd == "trysell" then
        @[TitleVendor]_init();
        tryProgressVendorState();
    elseif cmd == "print" then
        printState(@[Title]_state);
    elseif cmd == "skip" then
        getNextState();
    elseif cmd == "comment" then
        printComment(@[Title]_comment);
    elseif cmd == "ncomment" then
        nextComment();
    elseif cmd == "debug" then
        @[TitleVendor]_Debug();
    elseif strsub(cmd,1,7) == "upgrade" then
        local link = strsub(cmd,9,strlen(cmd));
        local is = CouldBeUpgrade(link);
        if is then
            DEFAULT_CHAT_FRAME:AddMessage(link .. " is an upgrade.");
        else
            DEFAULT_CHAT_FRAME:AddMessage(link .. " is not an upgrade.");
        end
    end
end
