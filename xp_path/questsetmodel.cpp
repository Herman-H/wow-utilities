#include "questsetmodel.h"

questsetmodel::questsetmodel(questData *d) :
    d(d)
{
    model = new QStandardItemModel(0,1);
}

void questsetmodel::addQuest(int quest)
{
    QStandardItem * desc, * id;
    QString s{" ("};
    s.append(QString::number(quest));
    s.append(")");
    desc = new QStandardItem{QString{d->get_quest(quest).questName.c_str()}.append(s)};
    desc->setEditable(false);
    id = new QStandardItem{};
    id->setData(quest);

    QList<QStandardItem*> l;
    l.push_back(desc);
    l.push_back(id);

    model->appendRow(l);
}

void questsetmodel::removeQuest(int quest)
{
    for(int i = 0; i < model->rowCount(); ++i)
    {
        if(model->item(i,1)->data().toInt() == quest)
        {
            model->removeRow(i);
        }
    }
}

void questsetmodel::clear()
{
    model->clear();
}
