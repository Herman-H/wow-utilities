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
    reach_xp_breakpoint
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
        return QString{"Marhal\'s Refuge, Un\'goro Crater"};
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


struct action
{
    action_type type;
    int questId;
    hearthstone_location hs;
    int itemrewardchoice;
    flightmaster_location flightmaster;
    flightmaster_location flightmaster_target;
    QString vendorName;

    int getLevel() const { return questId; }
    void setLevel(int level) { questId = level; }
    int getXp() const { return itemrewardchoice; }
    void setXp(int xp) { itemrewardchoice = xp; }

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
    }
    return s;
}

#endif // ACTION_H
