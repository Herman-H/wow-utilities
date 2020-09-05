#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <string>
#include <QString>
#include "questdata.h"

enum class action_type
{
    pick_up_quest,
    return_quest,
    set_hearthstone,
    use_hearthstone,
    discover_flightmaster,
    use_flightmaster,
    vendor,
    training,
    next_comment,
    reach_xp_breakpoint,
    bank_transaction
};



enum class hearthstone_location
{
    Deathknell,
    Brill,
    Undercity,
    Sepulcher,
    Orgrimmar,
    RazorHill,
    Ratchet,
    Crossroads,
    CampTaurajo,
    BloodhoofVillage,
    ThunderBluff,
    SunRockRetreat,
    FreewindPost,
    SplintertreePost,
    TarrenMill,
    Gadgetzan,
    BootyBay,
    GromgolBaseCamp,
    ShadowpreyVillage,
    Hammerfall,
    Kargath,
    Stonard,
    CampMojache,
    RevantuskVillage,
    Everlook,
    CenarionHold,
    LightsHopeChapel,

    COUNT
};

static QString hearthstone_location_string(hearthstone_location h)
{
    switch (h)
    {
    case hearthstone_location::BootyBay:
        return QString{"Booty Bay"};
    case hearthstone_location::BloodhoofVillage:
        return QString{"Bloodhoof Village"};
    case hearthstone_location::Brill:
        return QString{"Gallows\' End Tavern"};
    case hearthstone_location::CampMojache:
        return QString{"Camp Mojache"};
    case hearthstone_location::CampTaurajo:
        return QString{"Camp Taurajo"};
    case hearthstone_location::CenarionHold:
        return QString{"Cenarion Hold"};
    case hearthstone_location::Crossroads:
        return QString{"The Crossroads"};
    case hearthstone_location::Deathknell:
        return QString{"Deathknell"};
    case hearthstone_location::Everlook:
        return QString{"Everlook"};
    case hearthstone_location::FreewindPost:
        return QString{"Freewind Post"};
    case hearthstone_location::Gadgetzan:
        return QString{"Gadgetzan"};
    case hearthstone_location::GromgolBaseCamp:
        return QString{"Grom\'gol Base Camp"};
    case hearthstone_location::Hammerfall:
        return QString{"Hammerfall"};
    case hearthstone_location::Kargath:
        return QString{"Kargath"};
    case hearthstone_location::LightsHopeChapel:
        return QString{"Light\'s Hope Chapel"};
    case hearthstone_location::Orgrimmar:
        return QString{"Orgrimmar"};
    case hearthstone_location::Ratchet:
        return QString{"Ratchet"};
    case hearthstone_location::RazorHill:
        return QString{"Razor Hill"};
    case hearthstone_location::RevantuskVillage:
        return QString{"Revantusk Village"};
    case hearthstone_location::Sepulcher:
        return QString{"Sepulcher"};
    case hearthstone_location::ShadowpreyVillage:
        return QString{"Shadowprey Village"};
    case hearthstone_location::SplintertreePost:
        return QString{"Splintertree Post"};
    case hearthstone_location::Stonard:
        return QString{"Stonard"};
    case hearthstone_location::SunRockRetreat:
        return QString{"Sun Rock Retreat"};
    case hearthstone_location::TarrenMill:
        return QString{"Tarren Mill"};
    case hearthstone_location::ThunderBluff:
        return QString{"Thunder Bluff"};
    case hearthstone_location::Undercity:
        return QString{"Undercity"};
    }
    return QString{""};
}

static QString hearthstone_innkeeper_string(hearthstone_location h)
{
    switch (h)
    {
    case hearthstone_location::BootyBay:
        return QString{"Innkeeper Skindle"};
    case hearthstone_location::BloodhoofVillage:
        return QString{"Innkeeper Kauth"};
    case hearthstone_location::Brill:
        return QString{"Innkeeper Renee"};
    case hearthstone_location::CampMojache:
        return QString{"Innkeeper Greul"};
    case hearthstone_location::CampTaurajo:
        return QString{"Innkeeper Byula"};
    case hearthstone_location::CenarionHold:
        return QString{"Calandrath"};
    case hearthstone_location::Crossroads:
        return QString{"Innkeeper Boorand Plainswind"};
    case hearthstone_location::Deathknell:
        return QString{""};
    case hearthstone_location::Everlook:
        return QString{"Innkeeper Vizzie"};
    case hearthstone_location::FreewindPost:
        return QString{"Innkeeper Abeqwa"};
    case hearthstone_location::Gadgetzan:
        return QString{"Innkeeper Fizzgrimble"};
    case hearthstone_location::GromgolBaseCamp:
        return QString{"Innkeeper Thulbek"};
    case hearthstone_location::Hammerfall:
        return QString{"Innkeeper Adegwa"};
    case hearthstone_location::Kargath:
        return QString{"Innkeeper Shul\'kar"};
    case hearthstone_location::LightsHopeChapel:
        return QString{"Jessica Chambers"};
    case hearthstone_location::Orgrimmar:
        return QString{"Innkeeper Gryshka"};
    case hearthstone_location::Ratchet:
        return QString{"Innkeeper Wiley"};
    case hearthstone_location::RazorHill:
        return QString{"Innkeeper Grosk"};
    case hearthstone_location::RevantuskVillage:
        return QString{"Lard"};
    case hearthstone_location::Sepulcher:
        return QString{"Innkeeper Bates"};
    case hearthstone_location::ShadowpreyVillage:
        return QString{"Innkeeper Sikewa"};
    case hearthstone_location::SplintertreePost:
        return QString{"Innkeeper Kaylisk"};
    case hearthstone_location::Stonard:
        return QString{"Innkeeper Karakul"};
    case hearthstone_location::SunRockRetreat:
        return QString{"Innkeeper Jayka"};
    case hearthstone_location::TarrenMill:
        return QString{"Innkeeper Shay"};
    case hearthstone_location::ThunderBluff:
        return QString{"Innkeeper Pala"};
    case hearthstone_location::Undercity:
        return QString{"Innkeeper Norman"};
    }
    return QString{""};
}

enum class flightmaster_location
{
    Undercity,
    Sepulcher,
    Orgrimmar,
    Ratchet,
    Crossroads,
    CampTaurajo,
    ThunderBluff,
    SunRockRetreat,
    BrackenwallVillage,
    FreewindPost,
    SplintertreePost,
    ZoramgarOutpost,
    TarrenMill,
    Gadgetzan,
    BootyBay,
    GromgolBaseCamp,
    ShadowpreyVillage,
    Hammerfall,
    Kargath,
    Stonard,
    CampMojache,
    RevantuskVillage,
    ThoriumPoint,
    Valormok,
    MarshalsRefuge,
    BloodvenomPost,
    Moonglade,
    Everlook,
    FlameCrest,
    CenarionHold,
    LightsHopeChapel,

    COUNT
};

static QString flightmaster_location_string(flightmaster_location w)
{
    switch (w)
    {
    case flightmaster_location::BloodvenomPost:
        return QString{"Bloodvenom Post, Felwood"};
    case flightmaster_location::BootyBay:
        return QString{"Booty Bay, Stranglethorn"};
    case flightmaster_location::BrackenwallVillage:
        return QString{"Brackenwall Village, Dustwallow Marsh"};
    case flightmaster_location::CampMojache:
        return QString{"Camp Mojache, Feralas"};
    case flightmaster_location::CampTaurajo:
        return QString{"Camp Taurajo, The Barrens"};
    case flightmaster_location::CenarionHold:
        return QString{"Cenarion Hold, Silithus"};
    case flightmaster_location::Crossroads:
        return QString{"Crossroads, The Barrens"};
    case flightmaster_location::Everlook:
        return QString{"Everlook, Winterspring"};
    case flightmaster_location::FlameCrest:
        return QString{"Flame Crest, Burning Steppes"};
    case flightmaster_location::FreewindPost:
        return QString{"Freewind Post, Thousand Needles"};
    case flightmaster_location::Gadgetzan:
        return QString{"Gadgetzan, Tanaris"};
    case flightmaster_location::GromgolBaseCamp:
        return QString{"Grom\'gol, Stranglethorn"};
    case flightmaster_location::Hammerfall:
        return QString{"Hammerfall, Arathi"};
    case flightmaster_location::Kargath:
        return QString{"Kargath, Badlands"};
    case flightmaster_location::LightsHopeChapel:
        return QString{"Light\'s Hope Chapel, Eastern Plaguelands"};
    case flightmaster_location::MarshalsRefuge:
        return QString{"Marshal\'s Refuge, Un\'goro Crater"};
    case flightmaster_location::Moonglade:
        return QString{"Moonglade"};
    case flightmaster_location::Orgrimmar:
        return QString{"Orgrimmar, Durotar"};
    case flightmaster_location::Ratchet:
        return QString{"Ratchet, The Barrens"};
    case flightmaster_location::RevantuskVillage:
        return QString{"Revantusk Village, The Hinterlands"};
    case flightmaster_location::Sepulcher:
        return QString{"The Sepulcher, Silverpine Forest"};
    case flightmaster_location::ShadowpreyVillage:
        return QString{"Shadowprey Village, Desolace"};
    case flightmaster_location::SplintertreePost:
        return QString{"Splintertree Post, Ashenvale"};
    case flightmaster_location::Stonard:
        return QString{"Stonard, Swamp of Sorrows"};
    case flightmaster_location::SunRockRetreat:
        return QString{"Sun Rock Retreat, Stonetalon Mountains"};
    case flightmaster_location::TarrenMill:
        return QString{"Tarren Mill, Hillsbrad"};
    case flightmaster_location::ThoriumPoint:
        return QString{"Thorium Point, Searing Gorge"};
    case flightmaster_location::ThunderBluff:
        return QString{"Thunder Bluff, Mulgore"};
    case flightmaster_location::Undercity:
        return QString{"Undercity, Tirisfal"};
    case flightmaster_location::Valormok:
        return QString{"Valormok, Azshara"};
    case flightmaster_location::ZoramgarOutpost:
        return QString{"Zoram\'gar Outpost, Ashenvale"};
    }
    return QString{""};
}

static QString flightmaster_string(flightmaster_location w)
{
    switch (w)
    {
    case flightmaster_location::BloodvenomPost:
        return QString{"Brakkar"};
    case flightmaster_location::BootyBay:
        return QString{"Gringer"};
    case flightmaster_location::BrackenwallVillage:
        return QString{"Shardi"};
    case flightmaster_location::CampMojache:
        return QString{"Shyn"};
    case flightmaster_location::CampTaurajo:
        return QString{"Omusa Thunderhorn"};
    case flightmaster_location::CenarionHold:
        return QString{"Runk Windtamer"};
    case flightmaster_location::Crossroads:
        return QString{"Devrak"};
    case flightmaster_location::Everlook:
        return QString{"Yugrek"};
    case flightmaster_location::FlameCrest:
        return QString{"Vahgruk"};
    case flightmaster_location::FreewindPost:
        return QString{"Nyse"};
    case flightmaster_location::Gadgetzan:
        return QString{"Bulkrek Ragefist"};
    case flightmaster_location::GromgolBaseCamp:
        return QString{"Thysta"};
    case flightmaster_location::Hammerfall:
        return QString{"Urda"};
    case flightmaster_location::Kargath:
        return QString{"Gorrik"};
    case flightmaster_location::LightsHopeChapel:
        return QString{"Georgia"};
    case flightmaster_location::MarshalsRefuge:
        return QString{"Gryfe"};
    case flightmaster_location::Moonglade:
        return QString{"Faustron"};
    case flightmaster_location::Orgrimmar:
        return QString{"Doras"};
    case flightmaster_location::Ratchet:
        return QString{"Bragok"};
    case flightmaster_location::RevantuskVillage:
        return QString{"Gorkas"};
    case flightmaster_location::Sepulcher:
        return QString{"Karos Razok"};
    case flightmaster_location::ShadowpreyVillage:
        return QString{"Thalon"};
    case flightmaster_location::SplintertreePost:
        return QString{"Vhulgra"};
    case flightmaster_location::Stonard:
        return QString{"Breyk"};
    case flightmaster_location::SunRockRetreat:
        return QString{"Tharm"};
    case flightmaster_location::TarrenMill:
        return QString{"Zarise"};
    case flightmaster_location::ThoriumPoint:
        return QString{"Grisha"};
    case flightmaster_location::ThunderBluff:
        return QString{"Tal"};
    case flightmaster_location::Undercity:
        return QString{"Michael Garrett"};
    case flightmaster_location::Valormok:
        return QString{"Kroum"};
    case flightmaster_location::ZoramgarOutpost:
        return QString{"Andruk"};
    }
    return QString{""};
}

enum class bank_filter_function_to
{
    ALL,
    AT_LEAST_REMAIN
};

enum class bank_filter_function_from
{
    ALL,
    AT_LEAST_PUSH
};

static QString bank_filter_function_from_string(bank_filter_function_to t)
{
    switch(t)
    {
        case bank_filter_function_to::ALL:
            return QString{"Push all"};
        case bank_filter_function_to::AT_LEAST_REMAIN:
            return QString{"Push all but"};
    }
}
static QString bank_filter_function_from_string(bank_filter_function_from t)
{
    switch(t)
    {
        case bank_filter_function_from::ALL:
            return QString{"Pull all"};
        case bank_filter_function_from::AT_LEAST_PUSH:
            return QString{"Pull at least"};
    }
}


struct bank_atom_to
{
    int                     itemId;
    bank_filter_function_to to;
    unsigned int            param;
};
struct bank_atom_from
{
    int                         itemId;
    bank_filter_function_from   from;
    unsigned int                param;
};

struct bank_transaction
{
    QString                     banker_name;
    std::vector<bank_atom_to>   to_bank;
    std::vector<bank_atom_from> from_bank;

    /*QString encode() const
    {
        QString r{QString::number(banker_name.size())};
        r.append("|");
        for(int i = 0; i < banker_name.size(); ++i)
        {
            r.append(banker_name[i]);
        }
        r.append(QString::number(to_bank.size()));
        r.append("|");
        for(int i = 0; i < to_bank.size(); ++i)
        {
            r.append(QString::number(to_bank[i].itemId));
            r.append("|");
            r.append(QString::number(static_cast<int>(to_bank[i].to)));
            r.append("|");
            r.append(QString::number(to_bank[i].param));
        }
        r.append("|");
        r.append(QString::number(from_bank.size()));
        r.append("|");
        for(int i = 0; i < from_bank.size(); ++i)
        {
            r.append(QString::number(from_bank[i].itemId));
            r.append("|");
            r.append(QString::number(static_cast<int>(from_bank[i].from)));
            r.append("|");
            r.append(QString::number(from_bank[i].param));
        }
        return r;
    }

    bank_transaction(QString encoded_transaction)
    {
        QRegularExpression sizeRegexp{"([%d]+)"};
        QString nameSizeStr = sizeRegexp.match(encoded_transaction).captured(0);
        int nameSize = nameSizeStr.toInt();

        banker_name = encoded_transaction.mid(nameSizeStr.size()+1,nameSize);

        int begin_to = nameSizeStr.size()+nameSize+2;
        QString toSizeStr = sizeRegexp.match(encoded_transaction,begin_to).captured(0);
        int toSize = toSizeStr.toInt();

        int offset = begin_to + toSizeStr.size() + 1;
        for(int i = 0; i < toSize; ++i)
        {
            QRegularExpression toRegexp{"([%d]+)|([%d]+)|([%d]+)"};
            auto match = toRegexp.match(encoded_transaction, offset);
            QString itemIdStr = match.captured(0);
            QString fnTypeStr = match.captured(1);
            QString paramStr = match.captured(2);
            bank_atom_to to;
            to.itemId = itemIdStr.toInt();
            to.to = static_cast<bank_filter_function_to>(fnTypeStr.toInt());
            to.param = paramStr.toInt();
            to_bank.push_back(to);
            offset += itemIdStr.size() + fnTypeStr.size() + paramStr.size() + 3;
        }
        QString fromSizeStr = sizeRegexp.match(encoded_transaction,offset).captured(0);
        int fromSize = fromSizeStr.toInt();

        offset += fromSizeStr.size() + 1;
        for(int i = 0; i < fromSize; ++i)
        {
            QRegularExpression fromRegexp{"([%d]+)|([%d]+)|([%d]+)"};
            auto match = fromRegexp.match(encoded_transaction, offset);
            QString itemIdStr = match.captured(0);
            QString fnTypeStr = match.captured(1);
            QString paramStr = match.captured(2);
            bank_atom_from from;
            from.itemId = itemIdStr.toInt();
            from.from = static_cast<bank_filter_function_from>(fnTypeStr.toInt());
            from.param = paramStr.toInt();
            from_bank.push_back(from);
            offset += itemIdStr.size() + fnTypeStr.size() + paramStr.size() + 3;
        }

    }*/
};

struct action
{
    action_type type;
    int questId;
    hearthstone_location hs;
    int itemrewardchoice;
    flightmaster_location flightmaster;
    flightmaster_location flightmaster_target;
    QString vendorName;
    bank_transaction * bankTransaction;

    int getLevel() const { return questId; }
    void setLevel(int level) { questId = level; }
    int getXp() const { return itemrewardchoice; }
    void setXp(int xp) { itemrewardchoice = xp; }

    /*QString encode(const action a) const
    {
        QString r{QString::number(static_cast<int>(a.type))};
        r.append("|");
        r.append(QString::number(a.questId));
        r.append("|");
        r.append(QString::number(static_cast<int>(a.hs)));
        r.append("|");
        r.append(QString::number(a.itemrewardchoice));
        r.append("|");
        r.append(QString::number(static_cast<int>(a.flightmaster)));
        r.append("|");
        r.append(QString::number(static_cast<int>(a.flightmaster_target)));
        r.append("|");
        r.append(QString::number(a.vendorName.size()));
        r.append("|");
        for(int i = 0; i < a.vendorName.size(); ++i)
        {
            r.append(a.vendorName[i]);
        }
        if(a.bankTransaction)
        {
            r.append(a.bankTransaction->encode());
        }
        return r;
    }

    action(QString encoded_action)
    {
        QRegularExpression fromRegexp{"([%d]+)|([%d]+)|([%d]+)|([%d]+)|([%d]+)|([%d]+)|([%d]+)"};
        auto match = fromRegexp.match(encoded_action);
        QString typeStr = match.captured(0);
        QString questIdStr = match.captured(1);
        QString hsStr = match.captured(2);
        QString itemRewStr = match.captured(3);
        QString fmStr = match.captured(4);
        QString fmtStr = match.captured(5);
        QString vendorNameSizeStr = match.captured(6);

        type = static_cast<action_type>(typeStr.toInt());
        questId = questIdStr.toInt();
        hs = static_cast<hearthstone_location>(hsStr.toInt());
        itemrewardchoice = itemRewStr.toInt();
        flightmaster = static_cast<flightmaster_location>(fmStr.toInt());
        flightmaster_target = static_cast<flightmaster_location>(fmtStr.toInt());

        int vendorNameSize = vendorNameSizeStr.toInt();

        int offset = typeStr.size() + questIdStr.size() + hsStr.size() + itemRewStr.size() +
                fmStr.size() + fmtStr.size() + vendorNameSizeStr.size() + 7;

        vendorName = encoded_action.mid(offset,vendorNameSize);

        offset += vendorNameSize;
        if(!(offset >= encoded_action.size()))
        {
             bankTransaction = new bank_transaction(encoded_action.right(encoded_action.size()-offset));
        }
    }*/
};

static QString actionLabel(action a, questData& d)
{
    QString s;
    switch(a.type)
    {
    case action_type::pick_up_quest:
        s.append("Pick up quest: ");
        s.append(d.get_quest(a.questId).questName.c_str());
        s.append(" (").append(QString::number(a.questId)).append(")");
        break;
    case action_type::return_quest:
        s.append("Return quest: ");
        s.append(d.get_quest(a.questId).questName.c_str());
        if(a.itemrewardchoice != 0)
        {
            s.append(" and pick up item reward ");
            s.append(d.getItemName(a.itemrewardchoice));
        }
        s.append(" (").append(QString::number(a.questId)).append(")");
        break;
    case action_type::set_hearthstone:
        s.append("Set HS to: ");
        s.append(hearthstone_location_string(a.hs));
        break;
    case action_type::use_hearthstone:
        s.append("Use HS to: ");
        s.append(hearthstone_location_string(a.hs));
        break;
    case action_type::discover_flightmaster:
        s.append("Discover flight master: ");
        s.append(flightmaster_location_string(a.flightmaster));
        break;
    case action_type::use_flightmaster:
        s.append("Fly from ");
        s.append(flightmaster_location_string(a.flightmaster));
        s.append(" to ");
        s.append(flightmaster_location_string(a.flightmaster_target));
        break;
    case action_type::vendor:
        s.append("Use Vendor: ").append(a.vendorName);
        break;
    case action_type::training:
        s.append("Use Trainer: ").append(a.vendorName);
        break;
    case action_type::reach_xp_breakpoint:
        s.append("Reach ").append(QString::number(a.getXp()))
                .append(" xp into level ").append(QString::number(a.getLevel()));
    case action_type::next_comment:
        s.append(a.vendorName);
        break;
    case action_type::bank_transaction:
        s.append("Use banker ").append(a.bankTransaction->banker_name).append(" to ");
        if (a.bankTransaction->to_bank.size() > 0)
        {
            s.append("deposit ");
            for(unsigned int i = 0; i < a.bankTransaction->to_bank.size(); ++i)
            {
                bank_atom_to to = a.bankTransaction->to_bank[i];
                if(to.to == bank_filter_function_to::ALL)
                {
                    s.append("all [").append(d.getItemName(to.itemId)).append("]");
                }
                else if(to.to == bank_filter_function_to::AT_LEAST_REMAIN)
                {
                    s.append("all but ").append(QString::number(to.param));
                    s.append(" [").append(d.getItemName(to.itemId)).append("]");
                }
                if(i != a.bankTransaction->to_bank.size() - 1)
                {
                    s.append(", ");
                }
                else
                {
                    s.append(" to the bank");
                }
            }
            if(a.bankTransaction->from_bank.size() > 0)
            {
                s.append(" and ");
            }
        }
        if (a.bankTransaction->from_bank.size() > 0)
        {
            s.append("withdraw ");
            for(unsigned int i = 0; i < a.bankTransaction->from_bank.size(); ++i)
            {
                bank_atom_from from = a.bankTransaction->from_bank[i];
                if(from.from == bank_filter_function_from::ALL)
                {
                    s.append("all [").append(d.getItemName(from.itemId)).append("]");
                }
                else if(from.from == bank_filter_function_from::AT_LEAST_PUSH)
                {
                    s.append("at least ").append(QString::number(from.param));
                    s.append(" [").append(d.getItemName(from.itemId)).append("]");
                }
                if(i != a.bankTransaction->from_bank.size() - 1)
                {
                    s.append(", ");
                }
                else
                {
                    s.append(" from the bank");
                }
            }
        }
        break;
    }
    return s;
}



#endif // ACTION_H
