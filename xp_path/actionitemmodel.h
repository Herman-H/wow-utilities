#ifndef ACTIONITEMMODEL_H
#define ACTIONITEMMODEL_H

#include <QFile>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include "questdata.h"
#include "action.h"

struct actiontabledata
{
    int             m_level;
    action          m_action;
    QString         m_comment;
};

class actiontablemodel : public QAbstractItemModel
{
private:
    questData & d;
    QVector<actiontabledata> m_data;

    void setRowBackground(int row, action_type type);

public:
    actiontablemodel(questData & d);

    ~actiontablemodel(){}

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex{});
    bool removeRows(int row, int count, const QModelIndex &);
    bool moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild);

    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int, const QModelIndex &parent);
    QMimeData * mimeData(const QModelIndexList &indexes) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex{}) const;

    void insertItemOnRow(int row, action a);
    void editItemOnRow(int row, action a);
    action getSelectedAction(const QModelIndex &index);

    QModelIndex parent(const QModelIndex &) const { return QModelIndex{}; }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    std::vector<std::tuple<int,questEntry>> xpRewardSequence() const;

    questData & getQuestData() const { return d; }

    void loadActionsFromFile(QFile & f);
    void saveActionsToFile(QFile & f);
};

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

    std::vector<std::tuple<int,questEntry>> xpRewardSequence() const;

    QStandardItemModel * getModel() const { return model; }
    questData & getQuestData() const { return d; }


    void loadActionsFromFile(QFile & f);
    void saveActionsToFile(QFile & f);
};

#endif // ACTIONITEMMODEL_H
