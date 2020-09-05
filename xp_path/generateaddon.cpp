#include "generateaddon.h"
#include <QDir>
#include <QTextStream>
#include <map>
#include <QRegularExpression>

generateAddon::generateAddon(actiontablemodel & atm, vendorConditionModel & dontsell, vendorConditionModel & buy, std::vector<trainertablemodel*> & trainers) :
    atm(atm),
    dontsell(dontsell),
    buy(buy),
    trainers(trainers)
{

}

void generateAddon::generate112(QString path, QString addonName, QString author, QString notes, QString version)
{
    QString addonDirectory = path + QString{"/"} + addonName;
    QDir dir;
    dir.mkdir(addonDirectory);
    bool folderExists = dir.exists(addonDirectory);
    if(!folderExists)
    {
        dir.mkdir(addonDirectory);
    }
    QString filePath = addonDirectory + QString{"/"};
    QFile toc{filePath + addonName + QString{".toc"}};
    QFile mainxml{filePath + addonName + QString{".xml"}};
    QFile mainlua{filePath + addonName + QString{".lua"}};
    QFile vendorxml{filePath + addonName + QString{"Vendor.xml"}};
    QFile vendorlua{filePath + addonName + QString{"Vendor.lua"}};

    generateAddonToC(toc,addonName,author,notes,version);
    generateActionsXML(mainxml,addonName);
    generateVendorActionXML(vendorxml,addonName);
    saveActionsToScript("",mainlua,addonName);
    saveVendorActionsToScript("",vendorlua,addonName);
}

void generateAddon::generateClassic(QString path, QString addonName, QString author, QString notes, QString version)
{
    QString addonDirectory = path + QString{"/"} + addonName;
    QDir dir;
    dir.mkdir(addonDirectory);
    bool folderExists = dir.exists(addonDirectory);
    if(!folderExists)
    {
        dir.mkdir(addonDirectory);
    }
    QString filePath = addonDirectory + QString{"/"};
    QFile toc{filePath + addonName + QString{".toc"}};
    QFile mainxml{filePath + addonName + QString{".xml"}};
    QFile mainlua{filePath + addonName + QString{".lua"}};
    QFile vendorxml{filePath + addonName + QString{"Vendor.xml"}};
    QFile vendorlua{filePath + addonName + QString{"Vendor.lua"}};

    generateAddonClassicToC(toc,addonName,author,notes,version);
    generateActionsClassicXML(mainxml,addonName);
    generateVendorActionClassicXML(vendorxml,addonName);
    saveActionsToScript("Classic",mainlua,addonName);
    saveVendorActionsToScript("Classic",vendorlua,addonName);
}

QString generateAddon::trainerData() const
{
    QMap<QString, QString> map;
    auto include = [&map](trainertablemodel* m)
    {
        QMap<QString,int> r =  m->result();
        for(QString key : r.keys())
        {
            map.insert(key, QString::number(r[key]));
        }
    };
    include(trainers[0]);
    include(trainers[1]);

    QString returnValue = "local trainingTable = {";

    auto it = map.begin();
    while(it != map.end())
    {
        returnValue.append(QString{"[\""} + it.key() + QString{"\"]="} + it.value());
        ++it;
        if(it != map.end())
            returnValue.append(",");
    }
    returnValue.append("}");

    return returnValue;
}

QString generateAddon::replaceSymbols(QString s, QString title) const
{
    QString v{title};
    v.append("Vendor");
    s.replace(QString{"@[Title]"},title);
    s.replace(QString{"@[TitleVendor]"},v);
    s.replace(QString{"@[TitleTrainerData]"},trainerData());
    s.replace(QString{"@[TitleSlashCmd]"},
              QString{"SLASH_"}+ title.toUpper() + QString{"1 = \"/"} + title.toLower() + QString{"\""});
    s.replace(QString{"@[TitleSlashCmdRegister]"},
              QString{"SlashCmdList[\""}+ title.toUpper() + QString{"\"] = "} + title + QString{"_AdvanceX"});
    return s;
}
QString generateAddon::replaceVendorSymbols(QString s, QString title) const
{
    QString v{title};
    v.append("Vendor");
    s.replace(QString{"@[Title]"},title);
    s.replace(QString{"@[TitleVendor]"},v);
    s.replace(QString{"@[QuestStateMap]"},generateVendorQuestReference());
    s.replace(QString{"@[VendorData]"},generateVendorModelData());

    return s;
}

void generateAddon::generateActionsXML(QFile &f, QString addonName)
{
    QString indent{"    "};
    int r = 0;
    QString contents{"<Ui xmlns=\"http://www.blizzard.com/wow/ui/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.blizzard.com/wow/ui/\">\n"};
    r++;
    contents.append(indent.repeated(r)).append("<Script file=\"").append(addonName).append(".lua\"></Script>\n");
    contents.append(indent.repeated(r)).append("<Frame name=\"").append(addonName).append("Frame\">\n");
    r++;
    contents.append(indent.repeated(r)).append("<Scripts>\n");
    r++;
    contents.append(indent.repeated(r)).append("<OnLoad> ").append(addonName).append("_OnLoad(); </OnLoad>\n");
    contents.append(indent.repeated(r)).append("<OnEvent> ").append(addonName).append("_EventHandler(event); </OnEvent>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Scripts>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Frame>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Ui>\n");

    f.open(QIODevice::WriteOnly);
    f.write(contents.toLocal8Bit().data());
    f.close();
}

void generateAddon::generateActionsClassicXML(QFile &f, QString addonName)
{
    QString indent{"    "};
    int r = 0;
    QString contents{"<Ui xmlns=\"http://www.blizzard.com/wow/ui/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.blizzard.com/wow/ui/\">\n"};
    r++;
    contents.append(indent.repeated(r)).append("<Script file=\"").append(addonName).append(".lua\"></Script>\n");
    contents.append(indent.repeated(r)).append("<Frame name=\"").append(addonName).append("Frame\">\n");
    r++;
    contents.append(indent.repeated(r)).append("<Scripts>\n");
    r++;
    contents.append(indent.repeated(r)).append("<OnLoad> ").append(addonName).append("_OnLoad(self); </OnLoad>\n");
    contents.append(indent.repeated(r)).append("<OnEvent function=\"").append(addonName).append("_EventHandler\"/>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Scripts>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Frame>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Ui>\n");

    f.open(QIODevice::WriteOnly);
    f.write(contents.toLocal8Bit().data());
    f.close();
}

void generateAddon::generateVendorActionXML(QFile &f, QString addonName)
{
    QString indent{"    "};
    int r = 0;
    QString contents{"<Ui xmlns=\"http://www.blizzard.com/wow/ui/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.blizzard.com/wow/ui/\">\n"};
    r++;
    contents.append(indent.repeated(r)).append("<Script file=\"").append(addonName).append("Vendor.lua\"></Script>\n");
    contents.append(indent.repeated(r)).append("<Frame name=\"").append(addonName).append("VendorFrame\">\n");
    r++;
    contents.append(indent.repeated(r)).append("<Scripts>\n");
    r++;
    contents.append(indent.repeated(r)).append("<OnLoad> ").append(addonName).append("Vendor_OnLoad(); </OnLoad>\n");
    contents.append(indent.repeated(r)).append("<OnEvent> ").append(addonName).append("Vendor_EventHandler(event); </OnEvent>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Scripts>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Frame>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Ui>\n");

    f.open(QIODevice::WriteOnly);
    f.write(contents.toLocal8Bit().data());
    f.close();
}

void generateAddon::generateVendorActionClassicXML(QFile &f, QString addonName)
{
    QString indent{"    "};
    int r = 0;
    QString contents{"<Ui xmlns=\"http://www.blizzard.com/wow/ui/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.blizzard.com/wow/ui/\">\n"};
    r++;
    contents.append(indent.repeated(r)).append("<Script file=\"").append(addonName).append("Vendor.lua\"></Script>\n");
    contents.append(indent.repeated(r)).append("<Frame name=\"").append(addonName).append("VendorFrame\">\n");
    r++;
    contents.append(indent.repeated(r)).append("<Scripts>\n");
    r++;
    contents.append(indent.repeated(r)).append("<OnLoad> ").append(addonName).append("Vendor_OnLoad(self); </OnLoad>\n");
    contents.append(indent.repeated(r)).append("<OnEvent function=\"").append(addonName).append("Vendor_EventHandler\"/>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Scripts>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Frame>\n");
    r--;
    contents.append(indent.repeated(r)).append("</Ui>\n");

    f.open(QIODevice::WriteOnly);
    f.write(contents.toLocal8Bit().data());
    f.close();
}

void generateAddon::generateAddonToC(QFile &f, QString addonName, QString author, QString notes, QString version)
{
    QString contents{"## Interface: 11200\n"};
    contents.append("## Author: ").append(author).append("\n");
    contents.append("## Title: ").append(addonName).append("\n");
    contents.append("## Notes: ").append(notes).append("\n");
    contents.append("## Version: ").append(version).append("\n");
    contents.append("## SavedVariablesPerCharacter: ").append(addonName).append("_state, ").
            append(addonName).append("_comment\n");
    contents.append(addonName).append(".lua\n");
    contents.append(addonName).append(".xml\n");
    contents.append(addonName).append("Vendor.lua\n");
    contents.append(addonName).append("Vendor.xml");
    f.open(QIODevice::WriteOnly);
    f.write(contents.toLocal8Bit().data());
    f.close();
}
void generateAddon::generateAddonClassicToC(QFile &f, QString addonName, QString author, QString notes, QString version)
{
    QString contents{"## Interface: 11302\n"};
    contents.append("## Author: ").append(author).append("\n");
    contents.append("## Title: ").append(addonName).append("\n");
    contents.append("## Notes: ").append(notes).append("\n");
    contents.append("## Version: ").append(version).append("\n");
    contents.append("## SavedVariablesPerCharacter: ").append(addonName).append("_state, ").
            append(addonName).append("_comment\n");
    contents.append(addonName).append(".lua\n");
    contents.append(addonName).append(".xml\n");
    contents.append(addonName).append("Vendor.lua\n");
    contents.append(addonName).append("Vendor.xml");
    f.open(QIODevice::WriteOnly);
    f.write(contents.toLocal8Bit().data());
    f.close();
}

QString generateAddon::loadActionsTemplate(QString version) const
{
    QFile file{QString{"addon_template"} + (version.size() ? (QString{"_"} + version) : version) + QString{".lua"}};

    if(!file.open(QFile::ReadOnly | QFile::Text))
        return QString{};
    QTextStream s{&file};
    QString r{s.readAll()};
    file.close();
    return r;
}

QString generateAddon::loadVendorActionsTemplate(QString version) const
{
    QFile file{QString{"addon_vendor_template"} + (version.size() ? (QString{"_"} + version) : version) + QString{".lua"}};

    if(!file.open(QFile::ReadOnly | QFile::Text))
        return QString{};
    QTextStream s{&file};
    QString r{s.readAll()};
    file.close();
    return r;
}

void generateAddon::saveVendorActionsToScript(QString version, QFile &f, QString addonName)
{
    QString s{""};

    s.append(replaceVendorSymbols(loadVendorActionsTemplate(version),addonName));

    f.open(QIODevice::WriteOnly);
    f.write(s.toLocal8Bit().data());
    f.close();
}

void generateAddon::scanForReferedQuests(std::set<int> &quests, vendorConditionModel & m) const
{
    QRegularExpression regex{"isQuestCompleted\\(([\\d]+)"};

    QStandardItemModel * model = m.getModel();

    for(int i = 0; i < model->rowCount(); ++i)
    {
        QString condition = model->data(model->index(i,1)).toString();
        QString count = model->data(model->index(i,2)).toString();

        QRegularExpressionMatchIterator it = regex.globalMatch(condition);
        while(it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            int questId = match.captured(1).toInt();
            quests.insert(questId);
        }
        it = regex.globalMatch(count);
        while(it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            int questId = match.captured(1).toInt();
            quests.insert(questId);
        }
    }
}

void generateAddon::scanForUsedQuests(std::map<int,int> & quests) const
{
    for(int i = 0; i < atm.rowCount(); ++i)
    {
        action a = atm.getSelectedAction(atm.index(i,1));
        if(a.type == action_type::return_quest)
        {
            quests.insert({a.questId,i+1});
        }
    }
}

QString generateAddon::generateVendorQuestReference() const
{
    QString s{""};
    std::set<int> questsReferenced;
    std::map<int,int> questsUsed;
    std::map<int,int> reference;

    scanForReferedQuests(questsReferenced,dontsell);
    scanForReferedQuests(questsReferenced,buy);
    scanForUsedQuests(questsUsed);

    while(!questsReferenced.empty())
    {
        int picked = *(questsReferenced.begin());

        auto find = questsUsed.find(picked);
        if(find != questsUsed.end())
        {
            reference.insert({picked,(*find).second});
        }
        else
        {
            reference.insert({picked,0});
        }
        questsReferenced.erase(picked);
    }

    s.append("local QuestStateMap = {\n ");

    auto it = reference.begin();
    while(it != reference.end())
    {
        QString quest = QString::number((*it).first);
        QString state = QString::number((*it).second);

        s.append("    [").append(quest).append("]=").append(state).append(",\n");
        ++it;
    }
    s.remove(s.size()-2,2);
    s.append("\n};\n");

    return s;
}

QString generateAddon::generateVendorModelData() const
{
    QString s{""};
    int numberOfConditions = 0;
    std::multimap<QString,int> items;
    std::map<int,QString> conditions;
    std::map<int,QString> counts;
    QStandardItemModel * model = dontsell.getModel();
    for(int i = 0; i < model->rowCount(); ++i)
    {
        QString itemName = model->index(i,0).data(Qt::DisplayRole).toString();
        QString condition = model->index(i,1).data(Qt::DisplayRole).toString();
        QString count = model->index(i,2).data(Qt::DisplayRole).toString();
        items.insert({itemName,numberOfConditions});
        conditions.insert({numberOfConditions,condition});
        counts.insert({numberOfConditions,count});
        ++numberOfConditions;
    }
    /* Generate all conditions */
    auto it = conditions.begin();
    while(it != conditions.end())
    {
        QString func{""};
        func.append("vendor_condition[").append(QString::number((*it).first)).
                append("] = function() return ").append((*it).second).append("; end\n");
        s.append(func);
        it++;
    }
    /* Generate all counts */
    it = counts.begin();
    while(it != counts.end())
    {
        QString func{""};
        func.append("vendor_count[").append(QString::number((*it).first)).
                append("] = function () return ").append((*it).second).append("; end\n");
        s.append(func);
        it++;
    }
    s.append("local ds_get_condition_functions = { \n");
    /* Bind all items to their conditions */
    auto mit = items.begin();
    while(mit != items.end())
    {
        QString currentItem = (*mit).first;
        s.append("    [\"").append(currentItem).append("\"]={");
        while(mit != items.upper_bound(currentItem))
        {
            int n = (*mit).second;
            s.append("").append(QString::number(n)).append(",");
            mit++;
        }
        s.remove(s.size()-1,1);
        s.append("},\n");
    }
    s.remove(s.size()-2,2);
    s.append("\n};\n\n");

    items.clear();
    conditions.clear();
    counts.clear();

    model = buy.getModel();
    for(int i = 0; i < model->rowCount(); ++i)
    {
        QString itemName = model->index(i,0).data(Qt::DisplayRole).toString();
        QString condition = model->index(i,1).data(Qt::DisplayRole).toString();
        QString count = model->index(i,2).data(Qt::DisplayRole).toString();
        items.insert({itemName,numberOfConditions});
        conditions.insert({numberOfConditions,condition});
        counts.insert({numberOfConditions,count});
        ++numberOfConditions;
    }

    /* Generate all conditions */
    it = conditions.begin();
    while(it != conditions.end())
    {
        QString func{""};
        func.append("vendor_condition[").append(QString::number((*it).first)).
                append("] = function() return ").append((*it).second).append("; end\n");
        s.append(func);
        it++;
    }
    /* Generate all counts */
    it = counts.begin();
    while(it != counts.end())
    {
        QString func{""};
        func.append("vendor_count[").append(QString::number((*it).first)).
                append("] = function () return ").append((*it).second).append("; end\n");
        s.append(func);
        it++;
    }
    s.append("\nlocal b_get_count = { \n");
    /* Bind each count to its condition */
    it = conditions.begin();

    s.remove(s.size()-2,2);
    s.append("\n};\n\n");
    s.append("local b_get_condition_functions = { \n");
    /* Bind all items to their conditions */
    mit = items.begin();
    while(mit != items.end())
    {
        QString currentItem = (*mit).first;
        s.append("    [\"").append(currentItem).append("\"]={");
        while(mit != items.upper_bound(currentItem))
        {
            int n = (*mit).second;
            s.append("").append(QString::number(n)).append(",");
            mit++;
        }
        s.remove(s.size()-1,1);
        s.append("},\n");
    }
    s.remove(s.size()-2,2);
    s.append("\n};\n\n");

    return s;
}

void generateAddon::saveActionsToScript(QString version, QFile &f, QString addonName)
{
    QString script{""};
    std::vector<int> types;
    std::vector<int> extra;
    std::vector<QString> npcOrItemName;
    std::vector<QString> questtitles;
    std::vector<bool> hasquestreward;
    std::vector<QString> questrewards;
    std::vector<QString> comments;
    std::vector<bank_transaction*> transactions;

    questData & d = atm.getQuestData();
    comments.push_back(QString{"DEFAULT COMMENT"});
    int commentId = 2;
    int transactionId = 1;

    for(int i = 0; i < atm.rowCount(); ++i)
    {
        action action_ = atm.getSelectedAction(atm.index(i,1));
        action_type type = action_.type;
        types.push_back((int)type);

        if(type == action_type::pick_up_quest)
        {
            int questid = action_.questId;

            bool isOfferedByQuestGiver;
            bool isOfferedByItem;
            bool isOfferedByGameObject;

            // Assume that the quest is offered exclusively by just one type
            const std::string escaped_quote{"\\\""};
            QString questgivername = d.getQuestGiverName(questid,isOfferedByQuestGiver).replace("\"",QString::fromStdString(escaped_quote));
            QString itemname = d.getItemThatStartsQuest(questid,isOfferedByItem).replace("\"",QString::fromStdString(escaped_quote));
            isOfferedByGameObject = d.isStartedByGameObject(questid);

            // keeping this structure for clarity
            if(isOfferedByQuestGiver && !isOfferedByGameObject && !isOfferedByItem)
            {
                npcOrItemName.push_back(questgivername);
                extra.push_back(0);
            }
            else if(!isOfferedByQuestGiver && isOfferedByGameObject && !isOfferedByItem)
            {
                npcOrItemName.push_back(QString{});
                extra.push_back(2);
            }
            else if(!isOfferedByQuestGiver && !isOfferedByGameObject && isOfferedByItem)
            {
                npcOrItemName.push_back(itemname);
                extra.push_back(1);
            }
            else if(isOfferedByQuestGiver)
            {
                npcOrItemName.push_back(questgivername);
                extra.push_back(0);
            }
            else if(isOfferedByItem)
            {
                npcOrItemName.push_back(itemname);
                extra.push_back(1);
            }
            else
            {
                npcOrItemName.push_back(QString{""});
                extra.push_back(3);
            }
        }
        else if(type == action_type::return_quest)
        {
            int questid = action_.questId;

            bool isReturnedToNpc;
            bool isReturedToGameObject;

            // Assume that the quest is offered exclusively by just one type
            const std::string escaped_quote{"\\\""};
            QString npc = d.getQuestEnderNpcName(questid,isReturnedToNpc).replace("\"",QString::fromStdString(escaped_quote));
            isReturedToGameObject = d.isReturnedToGameObject(questid);

            // keeping this structure for clarity
            if(isReturnedToNpc)
            {
                npcOrItemName.push_back(npc);
                extra.push_back(0);
            }
            else if(isReturedToGameObject)
            {
                npcOrItemName.push_back(QString{""});
                extra.push_back(1);
            }
            else
            {
                npcOrItemName.push_back(QString{""});
                extra.push_back(3);
            }
        }
        else if(type == action_type::set_hearthstone)
        {
            npcOrItemName.push_back(hearthstone_innkeeper_string(action_.hs));
            extra.push_back(0);
        }
        else if(type == action_type::use_hearthstone)
        {
            npcOrItemName.push_back(hearthstone_location_string(action_.hs));
            extra.push_back(0);
        }
        else if(type == action_type::discover_flightmaster || type == action_type::use_flightmaster)
        {
            npcOrItemName.push_back(flightmaster_string(action_.flightmaster));
            extra.push_back(0);
        }
        else if(type == action_type::vendor || type == action_type::training)
        {
            const std::string escaped_quote{"\\\""};
            npcOrItemName.push_back(action_.vendorName.replace("\"",QString::fromStdString(escaped_quote)));
            extra.push_back(0);
        }
        else if(type == action_type::reach_xp_breakpoint)
        {
            npcOrItemName.push_back(QString{""});
            int level = action_.getLevel();
            int xp = action_.getXp();
            extra.push_back(level*300000 + xp);
        }
        else if(type == action_type::next_comment)
        {
            npcOrItemName.push_back(QString{""});
            extra.push_back(commentId);
            ++commentId;
            const std::string escaped_quote{"\\\""};
            comments.push_back(action_.vendorName.replace("\"",QString::fromStdString(escaped_quote)));
        }
        else if(type == action_type::bank_transaction)
        {
            npcOrItemName.push_back(action_.bankTransaction->banker_name);
            extra.push_back(transactionId);
            transactions.push_back(action_.bankTransaction);
            ++transactionId;
        }
        else
        {
            npcOrItemName.push_back(QString{""});
            extra.push_back(3);
        }

        if(type == action_type::pick_up_quest || type == action_type::return_quest)
        {
            const std::string escaped_quote{"\\\""};
            QString questname{d.get_quest(action_.questId).questName.c_str()};
            questtitles.push_back(questname.replace("\"",QString::fromStdString(escaped_quote)));
        }
        else if(type == action_type::set_hearthstone)
        {
            questtitles.push_back(hearthstone_location_string(action_.hs));
        }
        else if(type == action_type::discover_flightmaster)
        {
            questtitles.push_back(flightmaster_location_string(action_.flightmaster));
        }
        else if(type == action_type::use_flightmaster)
        {
            questtitles.push_back(flightmaster_location_string(action_.flightmaster_target));
        }
        else
        {
            questtitles.push_back(QString{""});
        }

        if(type == action_type::return_quest)
        {
            bool b = action_.itemrewardchoice != 0;
            hasquestreward.push_back(b);
            if(b)
            {
                questrewards.push_back(d.getItemName(action_.itemrewardchoice));
            }
            else
            {
                questrewards.push_back(QString{});
            }
        }
        else
        {
            hasquestreward.push_back(false);
            questrewards.push_back(QString{});
        }
    }
    // Write the size as a variable
    script.append("local size = ");
    script.append(QString::number(types.size()));
    script.append(";\n");
    // Write the type vector
    script.append("local types = {");
    if(types.size() > 0)
        script.append(QString::number(types[0]));
    for(int i = 1; i < types.size(); ++i )
    {
        script.append(",");
        script.append(QString::number(types[i]));
    }
    script.append("}\n");
    // Write the extra vector
    script.append("local extras = {");
    if(extra.size() > 0)
        script.append(QString::number(extra[0]));
    for(int i = 1; i < extra.size(); ++i )
    {
        script.append(",");
        script.append(QString::number(extra[i]));
    }
    script.append("}\n");
    // Write the npcOrItemName vector
    script.append("local names = {");
    if(npcOrItemName.size() > 0)
    {
        script.append("\"");
        script.append(npcOrItemName[0]);
        script.append("\"");
    }
    for(int i = 1; i < npcOrItemName.size(); ++i )
    {
        script.append(",\"");
        script.append(npcOrItemName[i]);
        script.append("\"");
    }
    script.append("}\n");
    // Write the questtitles vector
    script.append("local questtitles = {");
    if(questtitles.size() > 0)
    {
        script.append("\"");
        script.append(questtitles[0]);
        script.append("\"");
    }
    for(int i = 1; i < questtitles.size(); ++i )
    {
        script.append(",\"");
        script.append(questtitles[i]);
        script.append("\"");
    }
    script.append("}\n");
    script.append("local questrewards = {");
    if(hasquestreward.size() > 0)
    {
        if(hasquestreward[0])
        {
            script.append("\"");
            script.append(questtitles[0]);
            script.append("\"");
        }
        else
        {
            script.append("nil");
        }
    }
    for(int i = 1; i < hasquestreward.size(); ++i )
    {
        if(hasquestreward[i])
        {
            script.append(",\"");
            script.append(questrewards[i]);
            script.append("\"");
        }
        else
        {
            script.append(",nil");
        }
    }
    script.append("}\n");
    script.append("local comments = {");
    if(comments.size() > 0)
    {
        script.append("\"");
        script.append(comments[0]);
        script.append("\"");
    }
    for(int i = 1; i < comments.size(); ++i )
    {
        script.append(",\"");
        script.append(comments[i]);
        script.append("\"");
    }
    script.append("}\n");
    script.append("local transactions = {");
    if(transactions.size() > 0)
    {
        script.append("{");
        if(transactions[0]->to_bank.size() > 0)
        {
            bank_atom_to atom = transactions[0]->to_bank[0];
            script.append("To={{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.to)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        for(int j = 1; j < transactions[0]->to_bank.size(); ++j)
        {
            bank_atom_to atom = transactions[0]->to_bank[j];
            script.append(",{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.to)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        if(transactions[0]->to_bank.size() > 0)
        {
            script.append("};");
        }
        if(transactions[0]->from_bank.size() > 0)
        {
            bank_atom_from atom = transactions[0]->from_bank[0];
            script.append("From={{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.from)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        for(int j = 1; j < transactions[0]->from_bank.size(); ++j)
        {
            bank_atom_from atom = transactions[0]->from_bank[j];
            script.append(",{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.from)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        if(transactions[0]->from_bank.size() > 0)
        {
            script.append("};");
        }
        script.append("}");
    }
    for(int i = 1; i < transactions.size(); ++i )
    {
        script.append(",{");
        if(transactions[i]->to_bank.size() > 0)
        {
            bank_atom_to atom = transactions[i]->to_bank[0];
            script.append("To={{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.to)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        for(int j = 1; j < transactions[i]->to_bank.size(); ++j)
        {
            bank_atom_to atom = transactions[i]->to_bank[j];
            script.append(",{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.to)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        if(transactions[i]->to_bank.size() > 0)
        {
            script.append("};");
        }
        if(transactions[i]->from_bank.size() > 0)
        {
            bank_atom_from atom = transactions[i]->from_bank[0];
            script.append("From={{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.from)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        for(int j = 1; j < transactions[i]->from_bank.size(); ++j)
        {
            bank_atom_from atom = transactions[i]->from_bank[j];
            script.append(",{");
            script.append("Item=\"").append(d.getItemName(atom.itemId)).append("\";");
            script.append("Function=").append(QString::number((int)atom.from)).append(";");
            script.append("Arg=").append(QString::number((int)atom.param)).append(";}");
        }
        if(transactions[i]->from_bank.size() > 0)
        {
            script.append("};");
        }
        script.append("}");
    }
    script.append("};\n");

    script.append(replaceSymbols(loadActionsTemplate(version),addonName));

    f.open(QIODevice::WriteOnly);
    f.write(script.toLocal8Bit().data());
    f.close();
}
