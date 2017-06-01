--[[

	Clock: a simple in-game clock window
		copyright 2004 by Telo

	- Displays the time in a small, movable window
	- Displays time-based character information in a tooltip on mouseover
	* Updated for 1.10 by Redmumba

]]

--------------------------------------------------------------------------------------------------
-- Localizable strings
--------------------------------------------------------------------------------------------------

CLOCK = "Clock";
BINDING_NAME_TOGGLECLOCK = "Toggle Clock";
TIME_PLAYED_SESSION = "Time played this session: %s"; -- The amount of time played this session
CLOCK_TIME_DAY = "%d day";
CLOCK_TIME_HOUR = "%d hour";
CLOCK_TIME_MINUTE = "%d minute";
CLOCK_TIME_SECOND = "%d second";
EXP_PER_HOUR_LEVEL = "Experience per hour this level: %.2f";
EXP_PER_HOUR_SESSION = "Experience per hour this session: %.2f";
EXP_TO_LEVEL = "Experience to level: %d (%.2f%% to go)";
TIME_TO_LEVEL_LEVEL = "Time to level at this level's rate: %s";
TIME_TO_LEVEL_SESSION = "Time to level at this session's rate: %s";
TIME_INFINITE = "infinite";
HEALTH_PER_SECOND = "Health regenerated per second: %d";
MANA_PER_SECOND = "Mana regenerated per second: %d";

CLOCK_MONEY				= "Money earned per hour this session: %s";
CLOCK_MONEY_PER_HOUR			= "%s / hour";
CLOCK_MONEY_PER_MINUTE			= "%s / minute";
CLOCK_MONEY_UNAVAILABLE			= "Unavailable";
CLOCK_MONEY_SEPERATOR			= ", ";
CLOCK_MONEY_GOLD			= "%d gold";
CLOCK_MONEY_SILVER			= "%d silver";
CLOCK_MONEY_COPPER			= "%d copper";
CLOCK_MONEY_GOLD_SHORT			= "%d g";
CLOCK_MONEY_SILVER_SHORT		= "%d s";
CLOCK_MONEY_COPPER_SHORT		= "%d c";

COPPER_PER_GOLD				= 10000;
COPPER_PER_SILVER			= 100;

CLOCK_HELP = "help";			-- must be lowercase; displays help
CLOCK_STATUS = "status";		-- must be lowercase; shows status
CLOCK_FREEZE = "freeze";		-- must be lowercase; freezes the clock in position
CLOCK_UNFREEZE = "unfreeze";	-- must be lowercase; unfreezes the clock so that it can be dragged
CLOCK_RESET = "reset";			-- must be lowercase; resets the clock to its default position
CLOCK_24_HOUR = "24-hour";		-- must be lowercase; sets the clock to 24 hour time display
CLOCK_12_HOUR = "12-hour";		-- must be lowercase; sets the clock to 12 hour time display

CLOCK_STATUS_HEADER = "|cffffff00Clock status:|r";
CLOCK_FROZEN = "Clock: frozen in place";
CLOCK_UNFROZEN = "Clock: unfrozen and can be dragged";
CLOCK_RESET_DONE = "Clock: position reset to default";
CLOCK_SET_24 = "Clock: 24-hour time display";
CLOCK_SET_12 = "Clock: 12-hour time display";
CLOCK_TIME_OFFSET = "Clock: displayed time is offset by %s%02d:%02d";
CLOCK_TIME_ERROR = "Clock: unable to determine a valid offset from that input";

CLOCK_HELP_TEXT0 = " ";
CLOCK_HELP_TEXT1 = "|cffffff00Clock command help:|r";
CLOCK_HELP_TEXT2 = "|cff00ff00Use |r|cffffffff/clock <command>|r|cff00ff00 to perform the following commands:|r";
CLOCK_HELP_TEXT3 = "|cffffffff"..CLOCK_HELP.."|r|cff00ff00: displays this message.|r";
CLOCK_HELP_TEXT4 = "|cffffffff"..CLOCK_STATUS.."|r|cff00ff00: displays status information about the current option settings.|r";
CLOCK_HELP_TEXT5 = "|cffffffff"..CLOCK_FREEZE.."|r|cff00ff00: freezes the Clock so that it can't be dragged.|r";
CLOCK_HELP_TEXT6 = "|cffffffff"..CLOCK_UNFREEZE.."|r|cff00ff00: unfreezes the Clock so that it can be dragged.|r";
CLOCK_HELP_TEXT7 = "|cffffffff"..CLOCK_RESET.."|r|cff00ff00: resets the Clock to its default position.|r";
CLOCK_HELP_TEXT8 = "|cffffffff"..CLOCK_24_HOUR.."|r|cff00ff00: displays the time in 24-hour format.|r";
CLOCK_HELP_TEXT9 = "|cffffffff"..CLOCK_12_HOUR.."|r|cff00ff00: displays the time in 12-hour format.|r";
CLOCK_HELP_TEXT10 = "|cff00ff00Anything else is interpreted as the offset from server time that is applied to the time displayed.|r";
CLOCK_HELP_TEXT11 = "|cff00ff00Supported formats include -:30, +11, 5:30, 0, etc.|r";
CLOCK_HELP_TEXT12 = " ";
CLOCK_HELP_TEXT13 = "|cff00ff00For example: |r|cffffffff/clock +2|r|cff00ff00 will adjust the displayed time to two hours later than server time.|r";

CLOCK_HELP_TEXT = {
	CLOCK_HELP_TEXT0,
	CLOCK_HELP_TEXT1,
	CLOCK_HELP_TEXT2,
	CLOCK_HELP_TEXT3,
	CLOCK_HELP_TEXT4,
	CLOCK_HELP_TEXT5,
	CLOCK_HELP_TEXT6,
	CLOCK_HELP_TEXT7,
	CLOCK_HELP_TEXT8,
	CLOCK_HELP_TEXT9,
	CLOCK_HELP_TEXT10,
	CLOCK_HELP_TEXT11,
	CLOCK_HELP_TEXT12,
	CLOCK_HELP_TEXT13,
};

--------------------------------------------------------------------------------------------------
-- Local variables
--------------------------------------------------------------------------------------------------

local TotalTimePlayed = 0;
local LevelTimePlayed = 0;
local SessionTimePlayed = 0;
local ElapsedSinceLastPlayedMessage = 0;
local NeedPlayedMessage = 1;

local localInitialXP;
local localSessionXP = 0;
local localRolloverXP = 0;

local localHealthSecondTimer = 0;
local localInitialHealth;
local localHealthPerSecond;
local localManaSecondTimer = 0;
local localInitialMana;
local localManaPerSecond;

local localInCombat;
local localLastPosition;
local localTravelTime = 0;
local localTravelDist = 0;

local lBeingDragged;

local getDefaultTableEntry;

local isQuest;
local getXP;
local getName;
local roundToPrecision;

local Ptr0TimeDiff = 300;
local Ptr1TimeDiff = 3600;
local Ptr4TimeDiff = 14400;

local EndPtr;

local isTableInit = 0;

local initializePtrs;
local getNormalizedXP;

local UpdateMeasures;

-- added by sarf for money
local Clock_ShowMoney = 0;
local localMoneyOld = nil;
local localMoneyIncreases = 0;

-- the current server
local lServer;

--------------------------------------------------------------------------------------------------
-- Global variables
--------------------------------------------------------------------------------------------------

-- Constants
CLOCK_UPDATE_RATE = 0.1;

--------------------------------------------------------------------------------------------------
-- Internal functions
--------------------------------------------------------------------------------------------------

local function Clock_Status()
	DEFAULT_CHAT_FRAME:AddMessage(CLOCK_STATUS_HEADER);
	if( ClockState ) then
		if( ClockState.Freeze ) then
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_FROZEN);
		else
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_UNFROZEN);
		end
		if( ClockState.MilitaryTime ) then
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_SET_24);
		else
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_SET_12);
		end
		local hour;
		local minute;
		local sign;
		if( ClockState.Servers[lServer].OffsetHour ) then
			hour = ClockState.Servers[lServer].OffsetHour;
		else
			hour = 0;
		end
		if( ClockState.Servers[lServer].OffsetMinute ) then
			minute = ClockState.Servers[lServer].OffsetMinute;
		else
			minute = 0;
		end
		if( hour < 0 or minute < 0 ) then
			sign = "-";
		else
			sign = "+";
		end
		DEFAULT_CHAT_FRAME:AddMessage(format(CLOCK_TIME_OFFSET, sign, hour, minute));
	else
		DEFAULT_CHAT_FRAME:AddMessage(CLOCK_UNFROZEN);
		DEFAULT_CHAT_FRAME:AddMessage(CLOCK_SET_12);
		DEFAULT_CHAT_FRAME:AddMessage(format(CLOCK_TIME_OFFSET, "+", 0, 0));
	end
end

local function Clock_Reset()
	ClockFrame:ClearAllPoints();
	ClockFrame:SetPoint("TOP", "UIParent", "TOP", 0, 0);
end

function Clock_SlashCommandHandler(msg)
	if( msg ) then
		local command = string.lower(msg);
		if( command == "" or command == CLOCK_HELP ) then
			local index;
			local value;
			for index, value in CLOCK_HELP_TEXT do
				DEFAULT_CHAT_FRAME:AddMessage(value);
			end
		elseif( command == CLOCK_STATUS ) then
			Clock_Status();
		elseif( command == CLOCK_FREEZE ) then
			ClockState.Freeze = 1;
			Clock_OnDragStop();
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_FROZEN);
		elseif( command == CLOCK_UNFREEZE ) then
			ClockState.Freeze = nil;
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_UNFROZEN);
		elseif( command == CLOCK_RESET ) then
			Clock_Reset();
			Clock_OnDragStop();
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_RESET_DONE);
		elseif( command == CLOCK_24_HOUR ) then
			ClockState.MilitaryTime = 1;
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_SET_24);
		elseif( command == CLOCK_12_HOUR ) then
			ClockState.MilitaryTime = nil;
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_SET_12);
		else
			local s, e, sign, hour, minute = string.find(command, "^([+-]?)(%d*):?(%d*)$");
			if( hour or minute ) then
				if( not hour or hour == "" ) then
					hour = 0;
				end
				if( not minute or minute == "" ) then
					minute = 0;
				end
				if( string.len(hour) <= 2 and string.len(minute) <= 2 ) then
					if( sign and sign == "-" ) then
						ClockState.Servers[lServer].OffsetHour = -(hour + 0);
						ClockState.Servers[lServer].OffsetMinute = -(minute + 0);
					else
						sign = "+";
						ClockState.Servers[lServer].OffsetHour = hour + 0;
						ClockState.Servers[lServer].OffsetMinute = minute + 0;
					end
					DEFAULT_CHAT_FRAME:AddMessage(format(CLOCK_TIME_OFFSET, sign, hour, minute));
					return;
				end
			end
			DEFAULT_CHAT_FRAME:AddMessage(CLOCK_TIME_ERROR);
		end
	end
end

local function Clock_ResetHealth()
	localHealthSecondTimer = 0;
	localInitialHealth = UnitHealth("player");
end

local function Clock_ResetMana()
	localManaSecondTimer = 0;
	localInitialMana = UnitMana("player");
end

--------------------------------------------------------------------------------------------------
-- OnFoo functions
--------------------------------------------------------------------------------------------------

function Clock_OnLoad()
	this:RegisterForDrag("LeftButton");

	-- Register our slash command
	SLASH_CLOCK1 = "/clock";
	SlashCmdList["CLOCK"] = function(msg)
		Clock_SlashCommandHandler(msg);
	end

	this:RegisterEvent("PLAYER_MONEY");	
	this:RegisterEvent("VARIABLES_LOADED");
	this:RegisterEvent("TIME_PLAYED_MSG");
	this:RegisterEvent("PLAYER_LEVEL_UP");
	this:RegisterEvent("PLAYER_ENTERING_WORLD");
	this:RegisterEvent("PLAYER_XP_UPDATE");
	this:RegisterEvent("UNIT_HEALTH");
	this:RegisterEvent("UNIT_MANA");
	this:RegisterEvent("PLAYER_ENTER_COMBAT");
	this:RegisterEvent("PLAYER_LEAVE_COMBAT");

        this:RegisterEvent("CHAT_MSG_COMBAT_XP_GAIN");

        ClockText_OnEnter();
	
	ClockFrame.TimeSinceLastUpdate = 0;

	localMoneyOld = GetMoney();

	if( DEFAULT_CHAT_FRAME ) then
		DEFAULT_CHAT_FRAME:AddMessage("Clock AddOn loaded");
	end
	UIErrorsFrame:AddMessage("Clock AddOn loaded", 1.0, 1.0, 1.0, 1.0, UIERRORS_HOLD_TIME);



        MobXPHeader:SetText("Mob XP:");
        QuestXPHeader:SetText("Quest XP:");
        TotalXPHeader:SetText("Total XP:");
        NMobXPHeader:SetText("Mob NXP:");
        NQuestXPHeader:SetText("Quest NXP:");
        NTotalXPHeader:SetText("Total NXP:");
        RatioHeader:SetText("Mob/Quest:");

        Last5MinHeader:SetText("5 min");
        LastHourHeader:SetText("Hr");
        Last4HourHeader:SetText("4 Hrs");
        AllTimeHeader:SetText("All");
end

function Clock_OnUpdate(arg1)
	SessionTimePlayed = SessionTimePlayed + arg1;
	ElapsedSinceLastPlayedMessage = ElapsedSinceLastPlayedMessage + arg1;
	
	localHealthSecondTimer = localHealthSecondTimer + arg1;
	localManaSecondTimer = localManaSecondTimer + arg1;
	
	if( localHealthSecondTimer >= 1 and localInitialHealth ) then
		if( UnitHealth("player") < UnitHealthMax("player") ) then
			local hps = UnitHealth("player") - localInitialHealth;
			if( hps > 0 ) then
				localHealthPerSecond = hps / localHealthSecondTimer;
			end
		end
		Clock_ResetHealth();
	end
	
	if( localManaSecondTimer >= 1 and localInitialMana ) then
		if( UnitMana("player") < UnitManaMax("player") ) then
			local mps = UnitMana("player") - localInitialMana;
			if( mps > 0 ) then
				localManaPerSecond = mps / localManaSecondTimer;
			end
		end
		Clock_ResetMana();
	end
	
	ClockFrame.TimeSinceLastUpdate = ClockFrame.TimeSinceLastUpdate + arg1;
	if( ClockFrame.TimeSinceLastUpdate > CLOCK_UPDATE_RATE ) then
		ClockText:SetText(Clock_GetTimeText());
		if( GameTooltip:IsOwned(this) ) then
                        Clock_SetTooltip();
		end
		ClockFrame.TimeSinceLastUpdate = 0;
                if isTableInit == 1 then
                    UpdateMeasures();
                end
	end

end

function Clock_OnEvent()
	if( event == "PLAYER_MONEY" ) then
		Clock_UpdateMoney();
		return;
	end
	if( event == "TIME_PLAYED_MSG" ) then
		TotalTimePlayed = arg1;
		LevelTimePlayed = arg2;
		ElapsedSinceLastPlayedMessage = 0;
		NeedPlayedMessage = 0;
		
		-- Sync up all of the times to the session time; this makes
		-- the tooltip look nicer as everything changes all at once
		local fraction = SessionTimePlayed - floor(SessionTimePlayed);
		TotalTimePlayed = floor(TotalTimePlayed) + fraction;
		LevelTimePlayed = floor(LevelTimePlayed) + fraction;
		
		if( GameTooltip:IsOwned(this) ) then
			Clock_SetTooltip();
		end
	elseif( event == "PLAYER_LEVEL_UP" ) then
		LevelTimePlayed = SessionTimePlayed - floor(SessionTimePlayed);
		localRolloverXP = localSessionXP;
		localInitialXP = 0;
		if( GameTooltip:IsOwned(this) ) then
			Clock_SetTooltip();
		end
	elseif( event == "PLAYER_ENTERING_WORLD" ) then
		if( not localInitialXP ) then
			localInitialXP = UnitXP("player");
		end
		Clock_ResetHealth();
		Clock_ResetMana();
                RequestTimePlayed();
	elseif( event == "PLAYER_XP_UPDATE" ) then
		if( localInitialXP ) then
			localSessionXP = UnitXP("player") - localInitialXP + localRolloverXP;
		end
	elseif( event == "UNIT_HEALTH" ) then
		if( arg1 == "player" ) then
			if( not localInitialHealth or UnitHealth("player") - localInitialHealth < 0 ) then
				Clock_ResetHealth();
			end
		end
	elseif( event == "UNIT_MANA" ) then
		if( arg1 == "player" ) then
			if( not localInitialMana or UnitMana("player") - localInitialMana < 0 ) then
				Clock_ResetMana();
			end
		end
	elseif( event == "PLAYER_ENTER_COMBAT" ) then
		localInCombat = 1;
	elseif( event == "PLAYER_LEAVE_COMBAT" ) then
		localInCombat = nil;
	elseif( event == "VARIABLES_LOADED" ) then
		if( not ClockState ) then
			ClockState = { };
                end
		if( not ClockState.Servers ) then
			ClockState.Servers = { };
                end

		lServer = GetCVar("realmName");
		if( not ClockState.Servers[lServer] ) then
			ClockState.Servers[lServer] = { };
		end
		
		-- Convert old global time offset data into data for the current server
		if( ClockState.OffsetHour or ClockState.OffsetMinute ) then
			ClockState.Servers[lServer].OffsetHour = ClockState.OffsetHour;
			ClockState.Servers[lServer].OffsetMinute = ClockState.OffsetMinute;
			ClockState.OffsetHour = nil;
			ClockState.OffsetMinute = nil;
                end


                if not Kills then
                        Kills = { };
                end
                if not Kills.Count then
                        Kills.Count = 0;
                        EndPtr = 0;
                end
                if not Kills.Table then
                        Kills.Table = { };
                end

                DEFAULT_CHAT_FRAME:AddMessage("initializing ptrs");
                initializePtrs();


        elseif( event == "CHAT_MSG_COMBAT_XP_GAIN" ) then
                Kills.Count = Kills.Count + 1;
                Kills.Table[Kills.Count] = getDefaultTableEntry();
                Kills.Table[Kills.Count].Time = TotalTimePlayed + ElapsedSinceLastPlayedMessage;
                Kills.Table[Kills.Count].PlayerLevel = UnitLevel("player");
                if isQuest(arg1) then
                    Kills.Table[Kills.Count].IsQuest = 1;
                    Kills.Table[Kills.Count].Name = "Q";
                else
                    Kills.Table[Kills.Count].IsQuest = 0;
                    Kills.Table[Kills.Count].Name = getName(arg1);
                end
                Kills.Table[Kills.Count].XP = getXP(arg1);
                local X,Y = GetPlayerMapPosition("player");
                Kills.Table[Kills.Count].X = X;
                Kills.Table[Kills.Count].Y = Y;
                Kills.Table[Kills.Count].Map = GetCurrentMapZone();
                Kills.Table[Kills.Count].Continent = GetCurrentMapContinent();
        end
end

function ClockText_OnEnter()
        --GameTooltip:SetOwner(ClockFrame, "ANCHOR_NONE");
        GameTooltip:SetPoint("TOP", "UIParent", "TOP", 0, -32);
        RequestTimePlayed();
	Clock_SetTooltip();
        --ClockFrame:SetScale(0.9);
end

function Clock_OnDragStart()
	if( not ClockState or not ClockState.Freeze ) then
		ClockFrame:StartMoving()
		lBeingDragged = 1;
	end
end

function Clock_OnDragStop()
	ClockFrame:StopMovingOrSizing()
	lBeingDragged = nil;
end

-- Helper functions
isQuest = function(str)
    local a,b = strfind(str,'dies,');
    if a ~= nil and b ~= nil then
        return false;
    else
        return true;
    end
end

getXP = function(str, isQuest)
    local a,b = strfind(str,'gain ');
    if a ~= nil and b ~= nil then
        local s = strsub(str,b+1,strlen(str));
        local c,d = strfind(s, '[0-9]+');
        if c ~= nil and d ~= nil then
            return tonumber(strsub(s,c,d));
        else
            return false;
        end
    else
        return 0;
    end
end

getName = function(str)
    local a,b = strfind(str,'dies,');
    if a ~= nil and b ~= nil then
        return strsub(str,0,a-2);
    else
        return "N/A";
    end
end

roundToPrecision = function(num,precision)
    return string.format("%."..precision .."f", num);
end

initializePtrs = function()
    EndPtr = Kills.Count;
    isTableInit = 1;
    if Kills.Count == 0 then
        DEFAULT_CHAT_FRAME:AddMessage("First time use");
        NQuest0XPAcc = 0;
        NQuest1XPAcc = 0;
        NQuest4XPAcc = 0;
        NQuestAllXPAcc = 0;
        Quest0XPAcc = 0;
        Quest1XPAcc = 0;
        Quest4XPAcc = 0;
        QuestAllXPAcc = 0;

        NMob0XPAcc = 0;
        NMob1XPAcc = 0;
        NMob4XPAcc = 0;
        NMobAllXPAcc = 0;
        Mob0XPAcc = 0;
        Mob1XPAcc = 0;
        Mob4XPAcc = 0;
        MobAllXPAcc = 0;

        Ptr0 = Kills.Count;
        Ptr1 = Kills.Count;
        Ptr4 = Kills.Count;



        return;
    end
    --[[
    DEFAULT_CHAT_FRAME:AddMessage("Not first time use");

    while Kills.Table[Ptr1-1].Time > TotalTimePlayed + ElapsedSinceLastPlayedMessage - Ptr1TimeDiff and Ptr1 > 0 do
        Ptr1 = Ptr1 - 1;
        local normalizedXPFactor = getNormalizedXP(Kills.Table[Ptr1].PlayerLevel);
        if Kills.Table[Ptr1].IsQuest == 1 then
            NQuest1XPAcc = NQuest1XPAcc + (Kills.Table[Ptr1].XP / normalizedXPFactor);
            NQuest4XPAcc = NQuest4XPAcc + (Kills.Table[Ptr1].XP / normalizedXPFactor);
            NQuestAllXPAcc = NQuestAllXPAcc + (Kills.Table[Ptr1].XP / normalizedXPFactor);
            Quest1XPAcc = Quest1XPAcc + Kills.Table[Ptr1].XP;
            Quest4XPAcc = Quest4XPAcc + Kills.Table[Ptr1].XP;
            QuestAllXPAcc = QuestAllXPAcc + Kills.Table[Ptr1].XP;
        else
            NMob1XPAcc = NMob1XPAcc + (Kills.Table[Ptr1].XP / normalizedXPFactor);
            NMob4XPAcc = NMob4XPAcc + (Kills.Table[Ptr1].XP / normalizedXPFactor);
            NMobAllXPAcc = NMobAllXPAcc + (Kills.Table[Ptr1].XP / normalizedXPFactor);
            Mob1XPAcc = Mob1XPAcc + Kills.Table[Ptr1].XP;
            Mob4XPAcc = Mob4XPAcc + Kills.Table[Ptr1].XP;
            MobAllXPAcc = MobAllXPAcc + Kills.Table[Ptr1].XP;
        end
    end

    Ptr4 = Ptr1;

    while Kills.Table[Ptr4-1].Time > TotalTimePlayed + ElapsedSinceLastPlayedMessage - Ptr4TimeDiff and Ptr4 > 0 do
        Ptr4 = Ptr4 - 1;
        local normalizedXPFactor = getNormalizedXP(Kills.Table[Ptr4].PlayerLevel);
        if Kills.Table[Ptr4].IsQuest == 1 then
            NQuest4XPAcc = NQuest4XPAcc + (Kills.Table[Ptr4].XP / normalizedXPFactor);
            NQuestAllXPAcc = NQuestAllXPAcc + (Kills.Table[Ptr4].XP / normalizedXPFactor);
            Quest4XPAcc = Quest4XPAcc + Kills.Table[Ptr4].XP;
            QuestAllXPAcc = QuestAllXPAcc + Kills.Table[Ptr4].XP;
        else
            NMob4XPAcc = NMob4XPAcc + (Kills.Table[Ptr4].XP / normalizedXPFactor);
            NMobAllXPAcc = NMobAllXPAcc + (Kills.Table[Ptr4].XP / normalizedXPFactor);
            Mob4XPAcc = Mob4XPAcc + Kills.Table[Ptr4].XP;
            MobAllXPAcc = MobAllXPAcc + Kills.Table[Ptr4].XP;
        end
    end

    local PtrA = Ptr4;

    while PtrA > 0 do
        PtrA = PtrA - 1;
        local normalizedXPFactor = getNormalizedXP(Kills.Table[PtrA].PlayerLevel);
        if Kills.Table[PtrA].IsQuest == 1 then
            NQuestAllXPAcc = NQuestAllXPAcc + (Kills.Table[PtrA].XP / normalizedXPFactor);
            QuestAllXPAcc = QuestAllXPAcc + Kills.Table[PtrA].XP;
        else
            NMobAllXPAcc = NMobAllXPAcc + (Kills.Table[PtrA].XP / normalizedXPFactor);
            MobAllXPAcc = MobAllXPAcc + Kills.Table[PtrA].XP;
        end
    end

    isTableInit = 1;
    DEFAULT_CHAT_FRAME:AddMessage("Ptr1: " .. Ptr1);
    DEFAULT_CHAT_FRAME:AddMessage("Ptr4: " .. Ptr4);
    DEFAULT_CHAT_FRAME:AddMessage("EndPtr: " .. EndPtr);
    DEFAULT_CHAT_FRAME:AddMessage("MobXPAll: " .. MobAllXPAcc);

    ]]--

end

getDefaultTableEntry = function()
    return {
        Time = 0;
        PlayerLevel = 1;
        IsQuest = 0;
        Name = "";
        XP = 0;
        X = 0;
        Y = 0;
        Map = 0;
        Continent = 0;
    };
end

getNormalizedXP = function(lvl)
    return (lvl * 5) + 45;
end

UpdateMeasures = function()
    local t = TotalTimePlayed + ElapsedSinceLastPlayedMessage;
    local fivemin = Ptr0TimeDiff/3600;
    local fiveminmin = Ptr0TimeDiff/60;
    local onehr = Ptr1TimeDiff/3600;
    local onehrmin = Ptr1TimeDiff/60;
    local fourhrs = Ptr4TimeDiff/3600;
    local fourhrmin = Ptr4TimeDiff/60;
    if t < Ptr0TimeDiff then
        fivemin = t/3600;
        fiveminmin = t/60;
    end
    if t < Ptr1TimeDiff then
        onehr = t/3600;
        onehrmin = t/60;
    end
    if t < Ptr4TimeDiff then
        fourhrs = t/3600;
        fourhrmin = t/60;
    end


    if Kills.Count > 0 then
        while EndPtr < Kills.Count do
            EndPtr = EndPtr + 1;
            local normalizedXPFactor = getNormalizedXP(Kills.Table[EndPtr].PlayerLevel);
            if Kills.Table[EndPtr].IsQuest == 1 then
                NQuest0XPAcc = NQuest0XPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                NQuest1XPAcc = NQuest1XPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                NQuest4XPAcc = NQuest4XPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                NQuestAllXPAcc = NQuestAllXPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                Quest0XPAcc = Quest0XPAcc + Kills.Table[EndPtr].XP;
                Quest1XPAcc = Quest1XPAcc + Kills.Table[EndPtr].XP;
                Quest4XPAcc = Quest4XPAcc + Kills.Table[EndPtr].XP;
                QuestAllXPAcc = QuestAllXPAcc + Kills.Table[EndPtr].XP;
            else
                NMob0XPAcc = NMob0XPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                NMob1XPAcc = NMob1XPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                NMob4XPAcc = NMob4XPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                NMobAllXPAcc = NMobAllXPAcc + (Kills.Table[EndPtr].XP / normalizedXPFactor);
                Mob0XPAcc = Mob0XPAcc + Kills.Table[EndPtr].XP;
                Mob1XPAcc = Mob1XPAcc + Kills.Table[EndPtr].XP;
                Mob4XPAcc = Mob4XPAcc + Kills.Table[EndPtr].XP;
                MobAllXPAcc = MobAllXPAcc + Kills.Table[EndPtr].XP;
            end
        end
        while Ptr0 <= EndPtr-1 and Kills.Table[Ptr0+1].Time < TotalTimePlayed + ElapsedSinceLastPlayedMessage - Ptr0TimeDiff do
            Ptr0 = Ptr0 + 1;
            local normalizedXPFactor = getNormalizedXP(Kills.Table[Ptr0].PlayerLevel);
            if Kills.Table[Ptr0].IsQuest == 1 then
                NQuest0XPAcc = NQuest0XPAcc - (Kills.Table[Ptr0].XP / normalizedXPFactor);
                Quest0XPAcc = Quest0XPAcc - Kills.Table[Ptr0].XP;
            else
                NMob0XPAcc = NMob0XPAcc - (Kills.Table[Ptr0].XP / normalizedXPFactor);
                Mob0XPAcc = Mob0XPAcc - Kills.Table[Ptr0].XP;
            end
        end
        while Ptr1 <= EndPtr-1 and Kills.Table[Ptr1+1].Time < TotalTimePlayed + ElapsedSinceLastPlayedMessage - Ptr1TimeDiff do
            Ptr1 = Ptr1 + 1;
            local normalizedXPFactor = getNormalizedXP(Kills.Table[Ptr1].PlayerLevel);
            if Kills.Table[Ptr1].IsQuest == 1 then
                NQuest1XPAcc = NQuest1XPAcc - (Kills.Table[Ptr1].XP / normalizedXPFactor);
                Quest1XPAcc = Quest1XPAcc - Kills.Table[Ptr1].XP;
            else
                NMob1XPAcc = NMob1XPAcc - (Kills.Table[Ptr1].XP / normalizedXPFactor);
                Mob1XPAcc = Mob1XPAcc - Kills.Table[Ptr1].XP;
            end
        end
        while Ptr4 <= EndPtr-1 and Kills.Table[Ptr4+1].Time < TotalTimePlayed + ElapsedSinceLastPlayedMessage - Ptr4TimeDiff do
            Ptr4 = Ptr4 + 1;
            local normalizedXPFactor = getNormalizedXP(Kills.Table[Ptr4].PlayerLevel);
            if Kills.Table[Ptr4].IsQuest == 1 then
                NQuest4XPAcc = NQuest4XPAcc - (Kills.Table[Ptr4].XP / normalizedXPFactor);
                Quest4XPAcc = Quest4XPAcc - Kills.Table[Ptr4].XP;
            else
                NMob4XPAcc = NMob4XPAcc - (Kills.Table[Ptr4].XP / normalizedXPFactor);
                Mob4XPAcc = Mob4XPAcc - Kills.Table[Ptr4].XP;
            end
        end
    end

    MobXPLast5Min:SetText(roundToPrecision(Mob0XPAcc/fivemin,0));
    MobXPLastHr:SetText(roundToPrecision(Mob1XPAcc/onehr,0));
    MobXPLast4Hrs:SetText(roundToPrecision(Mob4XPAcc/fourhrs,0));
    MobXPAllTime:SetText(roundToPrecision(MobAllXPAcc/(t/3600),0));

    QuestXPLast5Min:SetText(roundToPrecision(Quest0XPAcc/fivemin,0));
    QuestXPLastHr:SetText(roundToPrecision(Quest1XPAcc/onehr,0));
    QuestXPLast4Hrs:SetText(roundToPrecision(Quest4XPAcc/fourhrs,0));
    QuestXPAllTime:SetText(roundToPrecision(QuestAllXPAcc/(t/3600),0));

    TotalXPLast5Min:SetText(roundToPrecision((Mob0XPAcc+Quest0XPAcc)/fivemin,0));
    TotalXPLastHr:SetText(roundToPrecision((Mob1XPAcc+Quest1XPAcc)/onehr,0));
    TotalXPLast4Hrs:SetText(roundToPrecision((Mob4XPAcc+Quest4XPAcc)/fourhrs,0));
    TotalXPAllTime:SetText(roundToPrecision((MobAllXPAcc+QuestAllXPAcc)/(t/3600),0));

    NMobXPLast5Min:SetText(roundToPrecision(NMob0XPAcc/fiveminmin,2));
    NMobXPLastHr:SetText(roundToPrecision(NMob1XPAcc/onehrmin,2));
    NMobXPLast4Hrs:SetText(roundToPrecision(NMob4XPAcc/fourhrmin,2));
    NMobXPAllTime:SetText(roundToPrecision(NMobAllXPAcc/(t/60),2));

    NQuestXPLast5Min:SetText(roundToPrecision(NQuest0XPAcc/fiveminmin,2));
    NQuestXPLastHr:SetText(roundToPrecision(NQuest1XPAcc/onehrmin,2));
    NQuestXPLast4Hrs:SetText(roundToPrecision(NQuest4XPAcc/fourhrmin,2));
    NQuestXPAllTime:SetText(roundToPrecision(NQuestAllXPAcc/(t/60),2));

    NTotalXPLast5Min:SetText(roundToPrecision((NMob0XPAcc+NQuest0XPAcc)/fiveminmin,2));
    NTotalXPLastHr:SetText(roundToPrecision((NMob1XPAcc+NQuest1XPAcc)/onehrmin,2));
    NTotalXPLast4Hrs:SetText(roundToPrecision((NMob4XPAcc+NQuest4XPAcc)/fourhrmin,2));
    NTotalXPAllTime:SetText(roundToPrecision((NMobAllXPAcc+NQuestAllXPAcc)/(t/60),2));

    RatioLast5Min:SetText(roundToPrecision(Mob0XPAcc/(Quest0XPAcc+1),2));
    RatioLastHr:SetText(roundToPrecision(Mob1XPAcc/(Quest1XPAcc+1),2));
    RatioLast4Hrs:SetText(roundToPrecision(Mob4XPAcc/(Quest4XPAcc+1),2));
    RatioAllTime:SetText(roundToPrecision(MobAllXPAcc/(QuestAllXPAcc+1),2));
end

function Clock_GetTimeText()
	--local hour, minute = GetGameTime();
	--local pm;
	local total;
        total = Clock_FormatTimeX(TotalTimePlayed + ElapsedSinceLastPlayedMessage);
	return total;
--[[
	if( ClockState ) then
		if( ClockState.Servers[lServer].OffsetHour ) then
			hour = hour + ClockState.Servers[lServer].OffsetHour;
		end
		if( ClockState.Servers[lServer].OffsetMinute ) then
			minute = minute + ClockState.Servers[lServer].OffsetMinute;
		end
	end
	if( minute > 59 ) then
		minute = minute - 60;
		hour = hour + 1
	elseif( minute < 0 ) then
		minute = 60 + minute;
		hour = hour - 1;
	end
	if( hour > 23 ) then
		hour = hour - 24;
	elseif( hour < 0 ) then
		hour = 24 + hour;
	end
	
	if( ClockState and ClockState.MilitaryTime ) then
		return format(TEXT(TIME_TWENTYFOURHOURS), hour, minute);
	else
		if( hour >= 12 ) then
			pm = 1;
			hour = hour - 12;
		else
			pm = 0;
		end
		if( hour == 0 ) then
			hour = 12;
		end
		if( pm == 1 ) then
			return format(TEXT(TIME_TWELVEHOURPM), hour, minute);
		else
			return format(TEXT(TIME_TWELVEHOURAM), hour, minute);
		end
	end
]]--
end

local function Clock_FormatPart(fmt, val)
	local part;
	
	part = format(TEXT(fmt), val);
	if( val ~= 1 ) then
		part = part.."s";
	end
	
	return part;
end

function Clock_FormatTimeX(time)
        local d, h, m, s;
        local text = "";

	d, h, m, s = ChatFrame_TimeBreakDown(time);
        s = floor(s);
	if( d > 0 ) then
                text = text..d..":";
	end
        if( (h > 9) ) then
                text = text..h..":";
        elseif ( (h >= 0) ) then
                text = text.."0"..h..":";
	end
        if( (m > 9) ) then
                text = text..m..":";
        elseif ( (m >= 0) ) then
                text = text.."0"..m..":";
	end
        if( (s > 9) ) then
                text = text..s;
        elseif ( (m >= 0) ) then
                text = text.."0"..s;
	end
	
	return text;
end

function Clock_FormatTime(time)
        local d, h, m, s;
        local text = "";
        local skip = 1;

        d, h, m, s = ChatFrame_TimeBreakDown(time);
        if( d > 0 ) then
                text = text..Clock_FormatPart(CLOCK_TIME_DAY, d)..", ";
                skip = 0;
        end
        if( (skip == 0) or (h > 0) ) then
                text = text..Clock_FormatPart(CLOCK_TIME_HOUR, h)..", ";
                skip = 0;
        end
        if( (skip == 0) or (m > 0) ) then
                text = text..Clock_FormatPart(CLOCK_TIME_MINUTE, m)..", ";
                skip = 0;
        end
        if( (skip == 0) or (s > 0) ) then
                text = text..Clock_FormatPart(CLOCK_TIME_SECOND, s);
        end

        return text;
end

function Clock_SetTooltip()
	local total, level, session;
	local xpPerHourLevel, xpPerHourSession;
	local xpTotal, xpCurrent, xpToLevel;
	local text;
	
	total = format(TEXT(TIME_PLAYED_TOTAL), Clock_FormatTime(TotalTimePlayed + ElapsedSinceLastPlayedMessage));
	level = format(TEXT(TIME_PLAYED_LEVEL), Clock_FormatTime(LevelTimePlayed + ElapsedSinceLastPlayedMessage));
	session = format(TEXT(TIME_PLAYED_SESSION), Clock_FormatTime(SessionTimePlayed));
	
	if( NeedPlayedMessage == 1 ) then
		text = session;
	else
		text = total.."\n"..level.."\n"..session;
	end
	
	if( (LevelTimePlayed + ElapsedSinceLastPlayedMessage > 0) or SessionTimePlayed > 0 ) then
		text = text.."\n";
	end
	if( LevelTimePlayed + ElapsedSinceLastPlayedMessage > 0 ) then
		xpPerHourLevel = UnitXP("player") / ((LevelTimePlayed + ElapsedSinceLastPlayedMessage) / 3600);
		text = text.."\n"..format(TEXT(EXP_PER_HOUR_LEVEL), xpPerHourLevel);
	else
		xpPerHourLevel = 0;
	end
	if( SessionTimePlayed > 0 ) then
		xpPerHourSession = localSessionXP / (SessionTimePlayed / 3600);
		text = text.."\n"..format(TEXT(EXP_PER_HOUR_SESSION), xpPerHourSession);
	else
		xpPerHourSession = 0;
	end
	
	xpTotal = UnitXPMax("player");
	xpCurrent = UnitXP("player");
	if( xpCurrent < xpTotal ) then
		xpToLevel = xpTotal - xpCurrent;
		text = text.."\n"..format(TEXT(EXP_TO_LEVEL), xpToLevel, (xpToLevel / xpTotal) * 100);
		if( xpPerHourLevel > 0 ) then
			text = text.."\n"..format(TEXT(TIME_TO_LEVEL_LEVEL), Clock_FormatTime((xpToLevel / xpPerHourLevel) * 3600));
		else
			text = text.."\n"..format(TEXT(TIME_TO_LEVEL_LEVEL), TEXT(TIME_INFINITE));
		end
		if( xpPerHourSession > 0 ) then
			text = text.."\n"..format(TEXT(TIME_TO_LEVEL_SESSION), Clock_FormatTime((xpToLevel / xpPerHourSession) * 3600));
		else
			text = text.."\n"..format(TEXT(TIME_TO_LEVEL_SESSION), TEXT(TIME_INFINITE));
		end
	end
	
	if( localHealthPerSecond or localManaPerSecond ) then
		text = text.."\n";
	end
	if( localHealthPerSecond ) then
		text = text.."\n"..format(TEXT(HEALTH_PER_SECOND), localHealthPerSecond);
	end
	if( localManaPerSecond ) then
		text = text.."\n"..format(TEXT(MANA_PER_SECOND), localManaPerSecond);
	end
	
	if ( localMoneyIncreases > 0 ) and ( SessionTimePlayed > 0 ) then
		text = text.."\n\n"..Clock_GetMoneyGainAsString();
	end

	GameTooltip:SetText(text);
end

function Clock_GetMoneyGainAsString()
	local formatStr = TEXT(CLOCK_MONEY);
	local valueStr = TEXT(CLOCK_MONEY_UNAVAILABLE);
	if ( localMoneyIncreases > 0 ) and ( SessionTimePlayed > 0 ) then
		local moneyFormat = TEXT(CLOCK_MONEY_PER_HOUR);
		local moneyGain = math.floor((localMoneyIncreases/SessionTimePlayed)*3600);
		if ( true ) then
			moneyFormat = TEXT(CLOCK_MONEY_PER_HOUR);
			moneyGain = math.floor((localMoneyIncreases/SessionTimePlayed)*3600);
		else
			moneyFormat = TEXT(CLOCK_MONEY_PER_MINUTE);
			moneyGain = math.floor((localMoneyIncreases/SessionTimePlayed)*60);
		end
		valueStr = format(moneyFormat, Clock_GetMoneyAsString(moneyGain));
	end
	return format(formatStr, valueStr);
end

function Clock_GetCurrencyString(currencyType)
	local name = "CLOCK_MONEY_";
	if ( currencyType == "gold" ) then
		name = name.."GOLD";
	end
	if ( currencyType == "silver" ) then
		name = name.."SILVER";
	end
	if ( currencyType == "copper" ) then
		name = name.."COPPER";
	end
	if ( not Clock_UseBigCurrencyDescriptions) or ( Clock_UseBigCurrencyDescriptions ~= 1 ) then
		name = name.."_SHORT";
	end
	return getglobal(name);
end

function Clock_GetMoneyAsString(amount)
	local gold = math.floor(amount / COPPER_PER_GOLD);
	amount = amount - gold * COPPER_PER_GOLD;
	local silver = math.floor(amount / COPPER_PER_SILVER);
	amount = amount - silver * COPPER_PER_SILVER;
	local copper = amount;
	local str = "";
	if ( gold > 0 ) then
		str = str..format(Clock_GetCurrencyString("gold"), gold);
		if ( silver > 0 ) or ( copper > 0 ) then
			str = str..TEXT(CLOCK_MONEY_SEPERATOR);
		end
	end
	if ( silver > 0 ) then
		str = str..format(Clock_GetCurrencyString("silver"), silver);
		if ( copper > 0 ) then
			str = str..TEXT(CLOCK_MONEY_SEPERATOR);
		end
	end
	if ( copper > 0 ) then
		str = str..format(Clock_GetCurrencyString("copper"), copper);
	end
	return str;
end

function Clock_UpdateMoney()
	local currentMoney = GetMoney();
	if ( not currentMoney ) then
		return false;
	end
	if ( localMoneyOld ) then
		local diff = currentMoney-localMoneyOld;
		if ( diff > 0 ) then
			localMoneyIncreases = localMoneyIncreases + diff;
		end
	end
	localMoneyOld = currentMoney;
	if ( not localMoneyOld ) then
		return false;
	else
		return true;
	end
end
