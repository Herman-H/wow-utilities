#ifndef SIMULATION
#define SIMULATION

#include <set>
#include "action.h"
#include "questdata.h"
#include "questsetmodel.h"


struct PickUpQuestErrorData
{
    bool questNotAlreadyCompleted;
    bool questNotPickedUp;
    bool prequestCompleted;
    bool prequestGroupCompleted;
    bool canAddToWorkingSet;
};


static bool canPickUpQuest(const PickUpQuestErrorData e)
{
    return  e.canAddToWorkingSet &&
            e.prequestCompleted &&
            e.prequestGroupCompleted &&
            e.questNotAlreadyCompleted &&
            e.questNotPickedUp;
}

static void pickUpQuestErrorMessages(const PickUpQuestErrorData & e, QStringList & strings)
{
    strings.clear();
    if(!e.questNotPickedUp)
    {
        strings.push_back(QString{"The quest was already picked up."});
    }
    if(!e.prequestCompleted)
    {
        strings.push_back(QString{"The prequest was not completed."});
    }
    if(!e.questNotAlreadyCompleted)
    {
        strings.push_back(QString{"The quest was already completed."});
    }
    if(!e.canAddToWorkingSet)
    {
        strings.push_back(QString{"The quest log is full."});
    }
    if(!e.prequestGroupCompleted)
    {
        strings.push_back(QString{"The required set of prequests has not been completed."});
    }
}


struct ReturnQuestErrorData
{
    // The simulation dont track whether the objectives of the quest was completed or not.
    bool questWasPickedUp;
};

static bool canReturnQuest(const ReturnQuestErrorData & e)
{
    return e.questWasPickedUp;
}

static void returnQuestErrorMessages(const ReturnQuestErrorData & e, QStringList & strings)
{
    strings.clear();
    if(!e.questWasPickedUp)
    {
        strings.push_back(QString{"The quest was not picked up."});
    }
}

struct UseHearthstoneErrorData
{
    bool hsIsSetToLocation;
};

static bool canUseHearthstone(const UseHearthstoneErrorData & e)
{
    return e.hsIsSetToLocation;
}

static void useHearthstoneErrorMessages(const UseHearthstoneErrorData & e, QStringList & strings)
{
    strings.clear();
    if(!e.hsIsSetToLocation)
    {
        strings.push_back(QString{"The hearthstone is set to another location."});
    }
}

struct DiscoverFlightmasterErrorData
{
    bool flightmasterWasNotDiscoveredYet;
};

static bool canDiscoverFlightmaster(const DiscoverFlightmasterErrorData & e)
{
    return e.flightmasterWasNotDiscoveredYet;
}

static void discoverFlightmasterErrorMessages(const DiscoverFlightmasterErrorData & e, QStringList & strings)
{
    strings.clear();
    if(!e.flightmasterWasNotDiscoveredYet)
    {
        strings.push_back(QString{"The flightmaster was already discovered."});
    }
}

struct UseFlightmasterErrorData
{
    bool flightmasterFromLocationWasNotDiscovered;
    bool flightmasterAtTargetLocationWasNotDiscovered;
    //bool flightmasterAlongThePathNotDiscovered; this would require a graph of all flightpaths, data of travel cost for each edge and a shortest path algorithm
};

class simulation_state
{
    std::set<int> completed_quests;
    questsetmodel * completed_quests_model;
    std::set<int> quest_working_set;
    questsetmodel * quest_working_set_model;
    hearthstone_location current_hs_location;
    std::set<flightmaster_location> discovered_flightmasters;
    int minLevel;

    questData * questdata;

    action_type lastActionType;
    bool isActionPermitted_;
    QString errorMessageHeader;
    PickUpQuestErrorData error_data_pickupquest;
    ReturnQuestErrorData error_data_returnquest;
    UseHearthstoneErrorData error_data_usehearthstone;
public:

    simulation_state(questData * d, questsetmodel * working, questsetmodel * completed) :
        completed_quests(),
        completed_quests_model(completed),
        quest_working_set(),
        quest_working_set_model(working),
        current_hs_location(hearthstone_location::Deathknell),
        discovered_flightmasters(),
        minLevel(1),
        questdata(d)
    {
        isActionPermitted_ = true;
        errorMessageHeader = "";
    }
private:
    bool pickUpQuest(int questId, PickUpQuestErrorData & p)
    {
        questEntry q = questdata->get_quest(questId);
        std::vector<int> f;
        questdata->get_prequest_group(q.prequestGrp,f);
        p.questNotAlreadyCompleted = completed_quests.find(questId) == completed_quests.end();
        p.questNotPickedUp = quest_working_set.find(questId) == quest_working_set.end();
        p.prequestCompleted =
                (q.prequestId == 0) ||
                (completed_quests.find(q.prequestId) != completed_quests.end()) ||
                (q.prequestId < 0 && quest_working_set.find(-(q.prequestId)) != quest_working_set.end());
        p.canAddToWorkingSet = (quest_working_set.size() < 20);
        bool prequestGrpCompleted = true;
        for(int i : f)
        {
            prequestGrpCompleted = prequestGrpCompleted && (completed_quests.find(i) != completed_quests.end());
        }
        p.prequestGroupCompleted = prequestGrpCompleted;

        bool canPickupQuest_ = canPickUpQuest(p);

        if(canPickupQuest_)
        {
            quest_working_set.insert(questId);
            quest_working_set_model->addQuest(questId);
            minLevel = (minLevel <= q.minLvl ? q.minLvl : minLevel);
        }

        return canPickupQuest_;
    }

    bool returnQuest(int questId, ReturnQuestErrorData & r)
    {
        questEntry q = questdata->get_quest(questId);

        r.questWasPickedUp = quest_working_set.find(questId) != quest_working_set.end();

        if(canReturnQuest(r))
        {
            quest_working_set.erase(questId);
            quest_working_set_model->removeQuest(questId);
            completed_quests.insert(questId);
            completed_quests_model->addQuest(questId);
        }

        return canReturnQuest(r);
    }

    bool setHearthstoneLocation(hearthstone_location h)
    {
        current_hs_location = h;
        return true;
    }

    bool useHearthstoneToLocation(hearthstone_location h, UseHearthstoneErrorData & e)
    {
        e.hsIsSetToLocation = h == current_hs_location;
        return e.hsIsSetToLocation;
    }

public:
    bool executeAction(const action & a)
    {
        isActionPermitted_ = false;
        switch (a.type)
        {
        case action_type::pick_up_quest:
            lastActionType = action_type::pick_up_quest;
            isActionPermitted_ = pickUpQuest(a.questId,error_data_pickupquest);
            errorMessageHeader = (isActionPermitted_ ? QString{"Quest was picked up."} : QString{"Quest could not be picked up."});
            break;
        case action_type::return_quest:
            lastActionType = action_type::return_quest;
            isActionPermitted_ = returnQuest(a.questId,error_data_returnquest);
            errorMessageHeader = isActionPermitted_ ? QString{"Quest was returned."} : QString{"Quest could not be returned."};
            break;
        case action_type::set_hearthstone:
            lastActionType = action_type::set_hearthstone;
            isActionPermitted_ = setHearthstoneLocation(a.hs);
            errorMessageHeader = "Hearthstone was set.";
            break;
        case action_type::use_hearthstone:
            lastActionType = action_type::use_hearthstone;
            isActionPermitted_ = useHearthstoneToLocation(a.hs,error_data_usehearthstone);
            errorMessageHeader = isActionPermitted_ ? QString{"Hearthstone was used."} : QString{"Hearthstone could not be used."};
            break;
        case action_type::discover_flightmaster:
            lastActionType = action_type::discover_flightmaster;
            isActionPermitted_ = true; // its always permitted
            errorMessageHeader = QString{"Flightmaster was discovered."};
            break;
        case action_type::use_flightmaster:
            lastActionType = action_type::use_flightmaster;
            isActionPermitted_ = true; // its always permitted
            errorMessageHeader = QString{"Flightmaster was used."};
            break;
        case action_type::vendor:
            lastActionType = action_type::vendor;
            isActionPermitted_ = true; // its always permitted
            errorMessageHeader = QString{"Vendor was used."};
            break;
        case action_type::training:
            lastActionType = action_type::training;
            isActionPermitted_ = true; // its always permitted
            errorMessageHeader = QString{"Trainer was used."};
            break;
        case action_type::reach_xp_breakpoint:
            lastActionType = action_type::reach_xp_breakpoint;
            isActionPermitted_ = true; // its always permitted
            errorMessageHeader = QString{"XP Breakpoint was reached."};
            break;
        case action_type::next_comment:
            lastActionType = action_type::next_comment;
            isActionPermitted_ = true; // its always permitted
            errorMessageHeader = QString{"Next comment..."};
            break;
        case action_type::bank_transaction:
            lastActionType = action_type::bank_transaction;
            isActionPermitted_ = true; // its always permitted
            errorMessageHeader = QString{"Performed bank transaction."};
            break;
        }
        return isActionPermitted_;
    }

    bool wasLastActionPermitted() const
    {
        return isActionPermitted_;
    }

    QString getErrorMessageHeader() const
    {
        return errorMessageHeader;
    }

    QStringList getErrorMessages() const
    {
        QStringList a;
        switch(lastActionType)
        {
        case action_type::pick_up_quest:
            pickUpQuestErrorMessages(error_data_pickupquest,a);
            break;
        case action_type::return_quest:
            returnQuestErrorMessages(error_data_returnquest,a);
            break;
        case action_type::set_hearthstone:
            break;
        case action_type::use_hearthstone:
            useHearthstoneErrorMessages(error_data_usehearthstone,a);
            break;
        }
        return a;
    }

    int getMinLevel() const
    {
        return minLevel;
    }
    int getWorkingSetSize() const
    {
        return quest_working_set.size();
    }

    int getCompletedSetSize() const
    {
        return completed_quests.size();
    }

    hearthstone_location getCurrentHearthstoneLocation() const
    {
        return current_hs_location;
    }


    void reset()
    {
        errorMessageHeader = "";
        isActionPermitted_ = true;

        completed_quests.clear();
        quest_working_set.clear();
        completed_quests_model->clear();
        quest_working_set_model->clear();
        minLevel = 1;
        current_hs_location = hearthstone_location::Brill;
    }
};

#endif // SIMULATION
