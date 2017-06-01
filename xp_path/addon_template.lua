
@[Title]_is_loaded = false;
@[Title]_gossip_shown = 0;
@[Title]_greeted = 0;
@[Title]_quest_shown = 0;
@[Title]_quest_progress_shown = 0;
@[Title]_quest_completed_shown = 0;
@[Title]_taxi_map_shown = 0;
@[Title]_trainer_menu_shown = 0;
local tryProgressPickupQuestState;
local tryProgressReturnQuestState;
local tryProgressSetHearthstoneState;
local tryProgressUseHearthstoneState;
local tryProgressDiscoverFPState;
local tryProgressTakeTaxiState;
local tryProgressVendorState;
local tryProgressTrainerState;
local checkIfNextStateForPickupQuests;
local checkIfNextStateForReturnQuests;
local checkIfNextStateForSetHearthstone;
local checkIfNextStateForUseHearthstone;
local checkIfNextStateForDiscoverFP;
local checkIfNextStateForTakeTaxi;
local checkIfNextStateForVendor;
local checkIfNextStateForTrainer;
local getNextState;

local function printState(state)
    local t = types[state];
    local extra = extras[state];
    local name = names[state];
    local quest = questtitles[state];
    local questreward = questrewards[state];
    local s = "State " .. state .. ": ";
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
        s = s .. "Use hearthstone to " .. quest .. ".";
    elseif t == 4 then
        s = s .. "Discover node " .. quest .. " at flight master " .. name .. ".";
    elseif t == 5 then
        s = s .. "Take flight from flight master " .. name .. " towards " .. quest .. ".";
    elseif t == 6 then
        s = s .. "Use vendor " .. name .. ".";
    elseif t == 7 then
        s = s .. "Use trainer " .. name .. ".";
    end
    DEFAULT_CHAT_FRAME:AddMessage(s)
end

function @[Title]_OnLoad()
    SLASH_@[Title]1 = "/@[Title]";
    SlashCmdList["TEST"] = @[Title]_AdvanceX;
    this:RegisterEvent("ADDON_LOADED");
    this:RegisterEvent("GOSSIP_SHOW");
    this:RegisterEvent("GOSSIP_CLOSED");
    this:RegisterEvent("QUEST_PROGRESS");
    this:RegisterEvent("QUEST_COMPLETE");
    this:RegisterEvent("QUEST_DETAIL");
    this:RegisterEvent("QUEST_FINISHED");
    this:RegisterEvent("QUEST_GREETING");
    this:RegisterEvent("CONFIRM_BINDER");
    this:RegisterEvent("TAXIMAP_OPENED");
    this:RegisterEvent("TAXIMAP_CLOSED");
    this:RegisterEvent("TRAINER_SHOW");
    this:RegisterEvent("TRAINER_CLOSED");
    this:RegisterEvent("TAXIMAP_CLOSED");
    this:RegisterEvent("CHAT_MSG_SYSTEM");
    this:RegisterEvent("UI_INFO_MESSAGE");
    if DEFAULT_CHAT_FRAME then DEFAULT_CHAT_FRAME:AddMessage("@[Title] was loaded.") end
end

local function init()
    if @[Title]_state == nil then
        @[Title]_state = 1;
    end
    @[Title]_is_loaded = true;
    printState(@[Title]_state);
end

function @[Title]_EventHandler(event)
    if event == "ADDON_LOADED" and arg1 == "Test" then
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
        if arg1 == ERR_NEWTAXIPATH then
            checkIfNextStateForDiscoverFP();
        end
    end
end

local function selectQuestWithNameGossipX(name,...)
    local a = 0;
    for i,v in ipairs(arg) do
        if math.mod(a,2) == 0 then
            if name == v then
                return (a/2) + 1;
            end
        end
        a = a + 1;
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
    local a = 0;
    for i,v in ipairs(arg) do
        if math.mod(a,2) == 0 then
            if name == v then
                return (a/2) + 1;
            end
        end
        a=a+1;
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
    local a = 0;
    for i,v in ipairs(arg) do
        if math.mod(a,2) == 1 then
            if v == name then
                return (a/2) + 1;
            end
        end
        a=a+1;
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
        TargetByName(name);
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
    elseif @[Title]_quest_progress_shown == 1 then
        progressQuest(questname);
    elseif @[Title]_quest_completed_shown == 1 then
        completeQuest(questname);
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
        local name = questtitles[Test_state];
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
            _,_,h = GetTrainerServiceInfo(i);
            if h ~= "header" then
                BuyTrainerService(i);
                return;
            end
        end
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
    getNextState();
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
        end
    end
end

getNextState = function()
    @[Title]_state = @[Title]_state + 1;
    printState(@[Title]_state);
    if types[@[Title]_state] == 6 then
        @[TitleVendor]_init();
    end
end

function @[Title]_AdvanceX(cmd)
    if cmd == "advance" then
        advance();
    elseif cmd == "trysell" then
        trysell();
    elseif cmd == "print" then
        printState();
    elseif cmd == "skip" then
        getNextState();
    end
end
