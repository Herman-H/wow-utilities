#ifndef QUESTSETMODEL_H
#define QUESTSETMODEL_H

#include <QStandardItemModel>
#include "questdata.h"

class questsetmodel
{
private:
    QStandardItemModel * model;
    questData * d;
public:
    questsetmodel(questData * d);
    ~questsetmodel(){}

    QStandardItemModel * getModel() const { return model; }

    void addQuest(int quest);
    void removeQuest(int quest);

    void clear();
};

#endif // QUESTSETMODEL_H
