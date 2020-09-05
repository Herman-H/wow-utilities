#ifndef TRAINERTABLEMODEL_H
#define TRAINERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include <vector>
#include <QMap>
#include <QSqlDatabase>
#include <QTableView>

class trainertablemodel : public QAbstractTableModel
{
private:
    const bool m_skill_req;

    struct entry {
        int     reqlevel;
        QString subtext;
        QString name;
        int     cost;
        int     input;
    };

    std::vector<entry> m_entries;

    QString key(const entry& e) const;

public:

    static void initializeView(QTableView& v);

    trainertablemodel(QList<int> trainer_ids, bool skill_req, const QSqlDatabase& db);

    ~trainertablemodel(){}

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex{}) const;
    QModelIndex parent(const QModelIndex &) const { return QModelIndex{}; }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QMap<QString, int> result() const;
    void load(const QMap<QString,int>& from);
};

#endif // TRAINERTABLEMODEL_H
