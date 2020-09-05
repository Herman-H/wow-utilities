#include "trainertablemodel.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QtAlgorithms>
#include <QHeaderView>

trainertablemodel::trainertablemodel(QList<int> trainer_ids, bool skill_req, const QSqlDatabase& db) :
    m_skill_req(skill_req),
    m_entries()
{
    QString q{"SELECT DISTINCT trainer.%1 AS Level, spells.name1 AS Spell, spells.nameSubtext1 AS Rank,"
                "trainer.spellcost / 100 AS Cost FROM spell_template AS spells, npc_trainer AS trainer "
                "WHERE spells.ID = trainer.spell AND trainer.entry IN (%2);"};
    QString trainer_ids_s;
    for(int i = 0; i < trainer_ids.size(); ++i)
    {
        trainer_ids_s.append(QString::number(trainer_ids[i]));
        if(i != trainer_ids.size()-1)
        {
            trainer_ids_s.append(",");
        }
    }
    q = q.arg(m_skill_req ? "reqskillvalue" : "reqlevel", trainer_ids_s);

    QSqlQuery query{db};
    query.exec(q);
    while(query.next())
    {
        m_entries.push_back(entry{query.value(0).toInt(), query.value(2).toString(), query.value(1).toString(), query.value(3).toInt(), query.value(0).toInt()});
    }
    qSort(m_entries.begin(),m_entries.end(), [](const entry& e1, const entry& e2){
        return e1.reqlevel < e2.reqlevel;
    });
}

int trainertablemodel::columnCount(const QModelIndex & parent) const
{
    if(parent.isValid())
        return 0;
    return 5;
}

int trainertablemodel::rowCount(const QModelIndex & parent) const
{
    if(parent.isValid())
        return 0;
    return m_entries.size();
}


QVariant trainertablemodel::data(const QModelIndex & index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        entry entry_ = m_entries[index.row()];
        switch(index.column())
        {
            case 0:
                return QString::number(entry_.reqlevel);
            case 1:
                return entry_.subtext;
            case 2:
                return entry_.name;
            case 3:
                return QString::number(entry_.cost) + "s";
            case 4:
                return QString::number(entry_.input);
        }
    }
    return QVariant{};
}


bool trainertablemodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();
    if(col == 4 && role == Qt::EditRole)
    {
        int v = value.toInt();
        if(v >= 0)
        {
            m_entries[row].input = v;
            emit dataChanged(index,index);
            return true;
        }

    }
    return false;
}


Qt::ItemFlags trainertablemodel::flags(const QModelIndex &index) const
{
    int col = index.column();
    int row = index.row();
    if(row >= m_entries.size())
        return 0;

    if(col >= 0 && col <= 3)
    {
        return  Qt::ItemIsSelectable        |
                Qt::ItemIsEnabled           |
                Qt::ItemNeverHasChildren;
    }
    else if(col == 4)
    {
        return  Qt::ItemIsSelectable        |
                Qt::ItemIsEditable          |
                Qt::ItemIsEnabled           |
                Qt::ItemNeverHasChildren;
    }
    return Qt::ItemIsSelectable;
}


QModelIndex trainertablemodel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent))
    {
        return QModelIndex{};
    }
    if(parent.isValid())
        return QModelIndex{};

    return createIndex(row,column);
}

QVariant trainertablemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return QString{"Level"};
        case 1:
            return QString{"Rank"};
        case 2:
            return QString{"Spell"};
        case 3:
            return QString{"Cost"};
        case 4:
            return QString{"Train"};
        }
    }
    else
    {
        return QAbstractItemModel::headerData(section,orientation,role);
    }
    return QString{""};
}

QMap<QString,int> trainertablemodel::result() const
{
    QMap<QString,int> res{};

    for(entry e : m_entries)
    {
        if(e.reqlevel < e.input)
        {
            res.insert(key(e), e.input);
        }
    }

    return res;
}


QString trainertablemodel::key(const entry& e) const
{
    return e.name + (e.subtext.size() > 0 ? "(" + e.subtext + ")" : "");
}

void trainertablemodel::load(const QMap<QString,int>& from)
{
    for(int i = 0; i < m_entries.size(); ++i)
    {
        auto it = from.find(key(m_entries[i]));
        if(it != from.end())
        {
            m_entries[i].input = *it;
        }
    }
}

void trainertablemodel::initializeView(QTableView& view)
{
    view.setSelectionBehavior(QAbstractItemView::SelectRows);
    view.setSelectionMode(QAbstractItemView::ContiguousSelection);
    view.horizontalHeader()->resizeSection(0,40);
    view.horizontalHeader()->resizeSection(1,80);
    view.horizontalHeader()->resizeSection(2,200);
    view.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    view.verticalHeader()->setDefaultSectionSize(24);
    view.horizontalHeader()->setStretchLastSection(true);
}
