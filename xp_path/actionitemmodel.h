#ifndef ACTIONITEMMODEL_H
#define ACTIONITEMMODEL_H

#include <QFile>
#include <QStandardItem>
#include <QStandardItemModel>
#include "questdata.h"
#include "action.h"

class actionitemmodel
{
private:
    questData & d;
    QStandardItemModel * model;

    void setItemBackground(QStandardItem * item, action_type type);

public:
    actionitemmodel(questData & d);
    ~actionitemmodel(){}

    void insertItemOnRow(int row, action a);
    action getSelectedAction(const QModelIndex &index);

    QStandardItemModel * getModel() const { return model; }
    questData & getQuestData() const { return d; }

    void loadActionsFromFile(QFile & f);
    void saveActionsToFile(QFile & f);
};

#endif // ACTIONITEMMODEL_H
