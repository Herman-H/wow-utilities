#ifndef QUESTDATA
#define QUESTDATA

#include <vector>
#include <map>
#include <string>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QListWidgetItem>

struct questEntry
{
    std::string questName;
    int         minLvl;
    int         prequestId;
    int         prequestGrp;
    int         rewMoneyMaxLevel;
    int         questLevel;
};

class questData
{
    std::map<int,questEntry> data;
    std::multimap<int,int> prequest_grps;
    QSqlDatabase db;

public:

    questData(QSqlDatabase &db) :
        data(),
        prequest_grps(),
        db(db)
    {
    }

    questEntry get_quest(int questid)
    {
        auto it = data.find(questid);
        if(it != data.end())
        {
            return it->second;
        }
        else
        {
            if(insert_quest(questid))
            {
                it = data.find(questid);
                return it->second;
            }
        }
    }

    QString getQuestGiverName(int questid, bool &hasQuestGiver)
    {
        QString innerStr{"SELECT id FROM creature_questrelation WHERE quest = "};
        innerStr.append(QString::number(questid));

        QString outerStr{"SELECT a.Name FROM creature_template AS a, ("};
        outerStr.append(innerStr);
        outerStr.append(") AS b WHERE a.Entry = b.id;");

        QSqlQuery q{db};
        q.exec(outerStr);

        QString r;
        if(q.next())
        {
            r = q.value(0).toString();
            hasQuestGiver = true;
        }
        else
        {
            hasQuestGiver = false;
        }
        if(q.next())
        {
            std::string e = get_quest(questid).questName.append(" has more than one questgiver.");
            qDebug(e.c_str());
        }
        return r;
    }

    QString getQuestEnderNpcName(int questid, bool &hasQuestEnderNpc)
    {
        QString innerStr{"SELECT id FROM creature_involvedrelation WHERE quest = "};
        innerStr.append(QString::number(questid));

        QString outerStr{"SELECT a.Name FROM creature_template AS a, ("};
        outerStr.append(innerStr);
        outerStr.append(") AS b WHERE a.Entry = b.id;");

        QSqlQuery q{db};
        q.exec(outerStr);

        QString r;
        if(q.next())
        {
            r = q.value(0).toString();
            hasQuestEnderNpc = true;
        }
        else
        {
            hasQuestEnderNpc = false;
        }
        if(q.next())
        {
            std::string e = get_quest(questid).questName.append(" has more than one npc to which the quest can be returned.");
            qDebug(e.c_str());
        }
        return r;
    }


    QString getItemThatStartsQuest(int questid, bool &hasItemThatStartsQuest)
    {
        QString q{"SELECT name FROM item_template WHERE start_quest = "};
        q.append(QString::number(questid));

        QSqlQuery query{db};
        query.exec(q);

        QString r{};
        if(query.next())
        {
            r = query.value(0).toString();
            hasItemThatStartsQuest = true;
        }
        else
        {
            hasItemThatStartsQuest = false;
        }
        if(query.next())
        {
            std::string e = get_quest(questid).questName.append(" is started by more than one item.");
            qDebug(e.c_str());
        }
        return r;
    }

    bool isStartedByGameObject(int questid)
    {
        QString queryStr{"SELECT id FROM gameobject_questrelation WHERE quest = "};
        queryStr.append(QString::number(questid));
        queryStr.append(";");

        QSqlQuery query{db};
        query.exec(queryStr);

        if(query.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool isReturnedToGameObject(int questid)
    {
        QString q{"SELECT id FROM gameobject_involvedrelation WHERE quest = "};
        q.append(QString::number(questid));
        q.append(";");

        QSqlQuery query{db};
        query.exec(q);

        if(query.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    QString getItemName(int itemid)
    {
        QString q{"SELECT name FROM item_template WHERE entry = "};
        q.append(QString::number(itemid)).append(";");

        QSqlQuery query{db};
        query.exec(q);

        if(query.next())
        {
            return query.value(0).toString();
        }
        return QString{""};
    }


    void get_prequest_group(int group, std::vector<int>& f)
    {
        f.clear();
        for(auto it = prequest_grps.lower_bound(group); it != prequest_grps.upper_bound(group); ++it)
        {
            f.push_back(it->second);
        }
    }

    bool insert_quest(int questid)
    {
        QString queryStr{"SELECT Title,MinLevel,PrevQuestId,RewMoneyMaxLevel,QuestLevel FROM quest_template WHERE entry = "};
        queryStr.append(QString::number(questid));
        queryStr.append(";");

        QSqlQuery query(db);
        query.exec(queryStr);


        questEntry newEntry;
        newEntry.minLvl = 0;
        newEntry.questName = "";
        newEntry.prequestId = 0;
        newEntry.prequestGrp = 0;
        newEntry.rewMoneyMaxLevel = 0;
        newEntry.questLevel = 1;

        bool questExists = false;

        if(query.next())
        {
            newEntry.questName = query.record().value(0).toString().toStdString();
            newEntry.minLvl = query.record().value(1).toInt();
            newEntry.prequestId = query.record().value(2).toInt();
            newEntry.rewMoneyMaxLevel = query.record().value(3).toInt();
            newEntry.questLevel = query.record().value(4).toInt();
            questExists = true;
        }

        queryStr = "SELECT a.ExclusiveGroup,a.entry FROM quest_template AS a, (SELECT ExclusiveGroup FROM quest_template WHERE entry = ";
        queryStr.append(QString::number(newEntry.prequestId));
        queryStr.append(") AS b WHERE a.ExclusiveGroup = b.ExclusiveGroup AND b.ExclusiveGroup < 0;");

        query.exec(queryStr);
        while(query.next())
        {
            bool first = true;
            bool doinserts = false;
            int g = query.record().value(0).toInt();
            int entry = query.record().value(1).toInt();
            if(first)
            {
                doinserts = (prequest_grps.find(g) == prequest_grps.end());
                first = false;
            }
            if(doinserts)
            {
                prequest_grps.insert(std::pair<int,int>{g,entry});
            }

            if(data.find(entry) != data.end())
            {
                insert_quest(entry);
            }

            newEntry.prequestGrp = g;
        }
        if(questExists)
        {
            data.insert(std::pair<int,questEntry>{questid,newEntry});
        }
        return questExists;
    }

};

#endif // QUESTDATA
