#include "actionitemmodel.h"
#include <QMimeData>
#include <set>

actiontablemodel::actiontablemodel(questData & d) :
    QAbstractItemModel(),
    d(d),
    m_data()
{
}

int actiontablemodel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return m_data.size();
}
int actiontablemodel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return 3;
}

Qt::DropActions actiontablemodel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QModelIndex actiontablemodel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent))
    {
        return QModelIndex{};
    }
    if(parent.isValid())
        return QModelIndex{};

    return createIndex(row,column);
}

QVariant actiontablemodel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if(!index.isValid())
        return QVariant{};

    if(role == Qt::DisplayRole)
    {
        if(col == 0)
        {
            int level = m_data[row].m_level;
            if(level > 0)
                return QString::number(level);
        }
        else if(col == 1)
        {
            return actionLabel(m_data[row].m_action,d);
        }
        else if(col == 2)
        {
            return m_data[row].m_comment;
        }
    }
    else if(col == 2 && role == Qt::EditRole)
    {
        return m_data[row].m_comment;
    }
    else if(role == Qt::BackgroundRole)
    {
        switch(m_data[row].m_action.type)
        {
        case action_type::pick_up_quest:
            return QBrush{QColor{225,225,0,100}};
        case action_type::return_quest:
            return QBrush{QColor{0,225,0,100}};
        case action_type::set_hearthstone:
            return QBrush{QColor{0,225,225,100}};
        case action_type::use_hearthstone:
            return QBrush{QColor{255,0,225,100}};
        case action_type::discover_flightmaster:
            return QBrush{QColor{128,0,0,100}};
        case action_type::use_flightmaster:
            return QBrush{QColor{255,0,0,100}};
        case action_type::vendor:
            return QBrush{QColor{128,128,128,100}};
        case action_type::training:
            return QBrush{QColor{128,64,255,100}};
        case action_type::reach_xp_breakpoint:
            return QBrush{QColor{128,0,128}};
        case action_type::next_comment:
            return QBrush{QColor{0,0,0,100}};
        case action_type::bank_transaction:
            return QBrush{QColor{128,255,128,100}};
        }
    }
    return QVariant{};
}

bool actiontablemodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();
    if(col == 0 && m_data[row].m_action.type == action_type::return_quest && role == Qt::EditRole)
    {
        int v = value.toInt();
        if(v >= 0)
        {
            m_data[row].m_level = v;
            emit dataChanged(index,index);
            return true;
        }

    }
    else if(col == 2 && role == Qt::EditRole)
    {
        QString s = value.toString();
        m_data[row].m_comment = s;
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

Qt::ItemFlags actiontablemodel::flags(const QModelIndex &index) const
{
    int col = index.column();
    int row = index.row();
    if(row >= m_data.size())
        return 0;

    if(col == 0)
    {
        return  Qt::ItemIsSelectable        |
                Qt::ItemIsEditable          |
                Qt::ItemIsEnabled           |
                Qt::ItemNeverHasChildren    |
                Qt::ItemIsDragEnabled;
    }
    else if(col == 1)
    {
        return  Qt::ItemIsSelectable        |
                Qt::ItemIsEnabled           |
                Qt::ItemNeverHasChildren    |
                Qt::ItemIsDragEnabled;
    }
    else if(col == 2)
    {
        return  Qt::ItemIsSelectable        |
                Qt::ItemIsEditable          |
                Qt::ItemIsEnabled           |
                Qt::ItemNeverHasChildren    |
                Qt::ItemIsDragEnabled;
    }
    return Qt::ItemIsDropEnabled;
}

QStringList actiontablemodel::mimeTypes() const
{
    QStringList types = QAbstractItemModel::mimeTypes();
    types.push_back("customtype/tablerows");
    return types;
}

bool actiontablemodel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int, const QModelIndex &parent)
{
    if(action == Qt::MoveAction && !parent.isValid())
    {
        QByteArray encodedData = data->data("customtype/tablerows");
        QDataStream stream(&encodedData,QIODevice::QIODevice::ReadOnly);

        QVector<int> rows;

        while(!stream.atEnd())
        {
            int i;
            stream >> i;
            rows.append(i);
        }
        int first = 0;
        int last = 0;
        if (rows.size())
        {
            first = rows[0];
            last = rows[rows.size()-1];
        }
        beginMoveRows(QModelIndex{},first,last,QModelIndex{},row);
        for(int i = rows.size(); i != 0; --i)
        {
            int r = rows[i-1];
            if(first < row)
            {
                moveRow(QModelIndex{},r,parent,row-rows.size()+i-1);
            }
            else
            {
                moveRow(QModelIndex{},r+rows.size()-i,parent,row);
            }
        }
        endMoveRows();

        return true;
    }
    return false;
}

QMimeData *actiontablemodel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = QAbstractItemModel::mimeData(indexes);
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    std::set<int> rows;
    foreach (const QModelIndex &index, indexes) {
        rows.insert(index.row());
    }
    for(auto r : rows)
    {
        stream << r;
    }

    mimeData->setData("customtype/tablerows", encodedData);
    return mimeData;
}

QVariant actiontablemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return QString{"At"};
        case 1:
            return QString{"Action"};
        case 2:
            return QString{"Comment"};
        }
    }
    else
    {
        return QAbstractItemModel::headerData(section,orientation,role);
    }
    return QString{""};
}

bool actiontablemodel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count-1);
    for(int i = 0; i < count; ++i)
    {
        actiontabledata newdata;
        newdata.m_level = 0;
        newdata.m_comment = QString{};
        m_data.insert(row,newdata);
    }
    endInsertRows();

    return true;
}

bool actiontablemodel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row+count);
    m_data.erase(m_data.begin()+row,m_data.begin()+(row+count));
    endRemoveRows();

    return true;
}

bool actiontablemodel::moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild)
{
    if(!sourceParent.isValid() && !destinationParent.isValid() && destinationChild >= 0)
    {
        m_data.move(sourceRow,destinationChild);
        return true;
    }
    return false;
}

void actiontablemodel::insertItemOnRow(int row, action a)
{
    insertRows(row,1,QModelIndex{});
    m_data[row].m_action = a;
}

void actiontablemodel::editItemOnRow(int row, action a)
{
    if (a.type == action_type::bank_transaction)
    {
        delete m_data[row].m_action.bankTransaction;
    }
    m_data[row].m_action = a;
    dataChanged(index(row,1),index(row,1));
}

action actiontablemodel::getSelectedAction(const QModelIndex &index)
{
    return m_data[index.row()].m_action;
}

std::vector<std::tuple<int,questEntry>> actiontablemodel::xpRewardSequence() const
{
    std::vector<std::tuple<int,questEntry>> data;

    for(int i = 0; i < m_data.size(); ++i)
    {
        actiontabledata data_ = m_data[i];
        if(data_.m_action.type == action_type::return_quest)
        {
            int questId = data_.m_action.questId;
            int preferredReturnLevel = data_.m_level;
            auto quest = d.get_quest(questId);

            data.push_back(std::tuple<int,questEntry>{preferredReturnLevel,quest});
        }
    }

    return data;
}

void actiontablemodel::loadActionsFromFile(QFile & f)
{
    f.open(QIODevice::ReadOnly);
    size_t sz = f.size();
    alignas (sizeof(int)) char data[f.size()];
    f.read(data,f.size());
    f.close();
    int row = 0;
    int * ptr = reinterpret_cast<int*>(data);

    auto readStr = [](int * ptr_, QString & into) -> int*
    {
        ++ptr_;
        std::size_t sz = *ptr_;
        ++ptr_;
        into = "";
        char * cptr = reinterpret_cast<char*>(ptr_);
        for(std::size_t i = 0; i < sz; ++i)
        {
            into.append(*cptr);
            cptr++;
        }
        ptr_ += sz * sizeof(char) / sizeof(int);
        return ptr_;
    };

    while(reinterpret_cast<ptrdiff_t>(ptr) < reinterpret_cast<ptrdiff_t>(data) + static_cast<ptrdiff_t>(sz))
    {
        action a;
        a.type = (action_type)(*ptr);
        int level = 0;
        QString comment{};
        switch(a.type)
        {
        case action_type::pick_up_quest:
            ++ptr;
            a.questId = *ptr;
            break;
        case action_type::return_quest:
            ++ptr;
            a.questId = *ptr;
            ++ptr;
            a.itemrewardchoice = *ptr;
            ++ptr;
            level = *ptr;
            break;
        case action_type::set_hearthstone:
            ++ptr;
            a.hs = (hearthstone_location)(*ptr);
            break;
        case action_type::use_hearthstone:
            ++ptr;
            a.hs = (hearthstone_location)(*ptr);
            break;
        case action_type::discover_flightmaster:
            ++ptr;
            a.flightmaster = (flightmaster_location)(*ptr);
            break;
        case action_type::use_flightmaster:
            ++ptr;
            a.flightmaster = (flightmaster_location)(*ptr);
            ++ptr;
            a.flightmaster_target = (flightmaster_location)(*ptr);
            break;
        case action_type::vendor:
        case action_type::training:
        case action_type::next_comment:
            ptr = readStr(ptr,a.vendorName);
            break;
        case action_type::reach_xp_breakpoint:
            ++ptr;
            a.setLevel(*ptr);
            ++ptr;
            a.setXp(*ptr);
            break;
        case action_type::bank_transaction:
            a.bankTransaction = new bank_transaction{};
            ptr = readStr(ptr,a.bankTransaction->banker_name);
            ++ptr;
            int to_count = *ptr;
            for(int i = 0; i < to_count; ++i)
            {
                bank_atom_to new_atom;
                ++ptr;
                new_atom.itemId = *ptr;
                ++ptr;
                new_atom.to = (bank_filter_function_to)(*ptr);
                ++ptr;
                new_atom.param = (unsigned int)(*ptr);
                a.bankTransaction->to_bank.push_back(new_atom);
            }
            ++ptr;
            int from_count = *ptr;
            for(int i = 0; i < from_count; ++i)
            {
                bank_atom_from new_atom;
                ++ptr;
                new_atom.itemId = *ptr;
                ++ptr;
                new_atom.from = (bank_filter_function_from)(*ptr);
                ++ptr;
                new_atom.param = (unsigned int)(*ptr);
                a.bankTransaction->from_bank.push_back(new_atom);
            }
            break;
        }
        ptr = readStr(ptr,comment);

        insertItemOnRow(row,a);
        m_data[row].m_level = level;
        m_data[row].m_comment = comment;
        ++ptr;
        ++row;
    }
}

void actiontablemodel::saveActionsToFile(QFile & f)
{
    // Actions are saved in format [action_type,X,X,X,X,X...] where action_type determines the remainder of the entry
    std::vector<int> vec;

    auto saveStr = [&vec](QString str)
    {
        std::size_t sz = str.size();
        vec.push_back((int)sz);
        std::size_t idx = vec.size();
        for(int i = 0; i < sz * sizeof(char)/sizeof(int) + 1; ++i)
        {
            vec.push_back(0);
        }
        memcpy(reinterpret_cast<char*>(vec.data() + idx),str.toStdString().c_str(),sz);
    };

    for(int i = 0; i < m_data.size(); ++i)
    {
        int level           = m_data[i].m_level;
        action action_      = m_data[i].m_action;
        QString comment     = m_data[i].m_comment;
        action_type type    = action_.type;
        vec.push_back((int)type);
        switch(type)
        {
        case action_type::pick_up_quest:
            vec.push_back(action_.questId);
            break;
        case action_type::return_quest:
            vec.push_back(action_.questId);
            vec.push_back((int)action_.itemrewardchoice);
            vec.push_back(level);
            break;
        case action_type::set_hearthstone:
            vec.push_back((int)action_.hs);
            break;
        case action_type::use_hearthstone:
            vec.push_back((int)action_.hs);
            break;
        case action_type::discover_flightmaster:
            vec.push_back((int)action_.flightmaster);
            break;
        case action_type::use_flightmaster:
            vec.push_back((int)action_.flightmaster);
            vec.push_back((int)action_.flightmaster_target);
            break;
        case action_type::vendor:
        case action_type::training:
        case action_type::next_comment:
            saveStr(action_.vendorName);
            break;

        case action_type::reach_xp_breakpoint:
            vec.push_back(action_.getLevel());
            vec.push_back(action_.getXp());
            break;
        case action_type::bank_transaction:
            saveStr(action_.bankTransaction->banker_name);
            vec.push_back(action_.bankTransaction->to_bank.size());
            for(unsigned int i = 0; i < action_.bankTransaction->to_bank.size(); ++i)
            {
                bank_atom_to to = action_.bankTransaction->to_bank[i];
                vec.push_back(to.itemId);
                vec.push_back((int)to.to);
                vec.push_back((int)to.param);
            }
            vec.push_back(action_.bankTransaction->from_bank.size());
            for(unsigned int i = 0; i < action_.bankTransaction->from_bank.size(); ++i)
            {
                bank_atom_from from = action_.bankTransaction->from_bank[i];
                vec.push_back(from.itemId);
                vec.push_back((int)from.from);
                vec.push_back((int)from.param);
            }
            break;
        }
        saveStr(comment);
    }
    f.open(QIODevice::WriteOnly);
    f.write(reinterpret_cast<char*>(vec.data()),vec.size()*sizeof(int)/sizeof(char));
    f.close();
}

actionitemmodel::actionitemmodel(questData & d) :
    d(d)
{
    model = new QStandardItemModel{0,1};
}

void actionitemmodel::setItemBackground(QStandardItem *item, action_type type)
{
    switch(type)
    {
    case action_type::pick_up_quest:
        item->setBackground(QBrush{QColor{225,225,0,100}});
        break;
    case action_type::return_quest:
        item->setBackground(QBrush{QColor{0,225,0,100}});
        break;
    case action_type::set_hearthstone:
        item->setBackground(QBrush{QColor{0,225,225,100}});
        break;
    case action_type::use_hearthstone:
        item->setBackground(QBrush{QColor{255,0,225,100}});
        break;
    case action_type::discover_flightmaster:
        item->setBackground(QBrush{QColor{128,0,0,100}});
        break;
    case action_type::use_flightmaster:
        item->setBackground(QBrush{QColor{255,0,0,100}});
        break;
    case action_type::vendor:
        item->setBackground(QBrush{QColor{128,128,128,100}});
        break;
    case action_type::training:
        item->setBackground(QBrush{QColor{128,64,255,100}});
        break;
    case action_type::reach_xp_breakpoint:
        item->setBackground(QBrush{QColor{128,0,128}});
        break;
    case action_type::next_comment:
        item->setBackground(QBrush{QColor{0,0,0,100}});
        break;
    case action_type::bank_transaction:
        item->setBackground(QBrush{QColor{128,255,128,100}});
        break;
    }
}

void actionitemmodel::insertItemOnRow(int row, action a)
{
    QStandardItem *desc,*type,*questId,*hs,*qr,*fp,*fpt,*vend;
    desc = new QStandardItem{actionLabel(a,d)};
    desc->setEditable(false);
    setItemBackground(desc,a.type);
    type = new QStandardItem{};
    questId = new QStandardItem{};
    hs = new QStandardItem{};
    qr = new QStandardItem{};
    fp = new QStandardItem{};
    fpt = new QStandardItem{};
    vend = new QStandardItem{};
    type->setData((int)a.type);
    questId->setData(a.questId);
    hs->setData((int)a.hs);
    qr->setData(a.itemrewardchoice);
    fp->setData((int)a.flightmaster);
    fpt->setData((int)a.flightmaster_target);
    vend->setData(a.vendorName);
    QList<QStandardItem*> l;
    l.push_back(desc);
    l.push_back(type);
    l.push_back(questId);
    l.push_back(hs);
    l.push_back(qr);
    l.push_back(fp);
    l.push_back(fpt);
    l.push_back(vend);
    model->insertRow(row,l);
}

action actionitemmodel::getSelectedAction(const QModelIndex &index)
{
    action a;
    auto type = model->item(index.row(),1)->data().toInt();
    auto questid = model->item(index.row(),2)->data().toInt();
    auto hs = model->item(index.row(),3)->data().toInt();
    auto qr = model->item(index.row(),4)->data().toInt();
    auto fp = model->item(index.row(),5)->data().toInt();
    auto fpt = model->item(index.row(),6)->data().toInt();
    auto vend = model->item(index.row(),7)->data().toString();
    a.type = (action_type)type;
    a.questId = questid;
    a.hs = (hearthstone_location)hs;
    a.itemrewardchoice = qr;
    a.flightmaster = (flightmaster_location)fp;
    a.flightmaster_target = (flightmaster_location)fpt;
    a.vendorName = vend;

    return a;
}

void actionitemmodel::loadActionsFromFile(QFile &f)
{
    f.open(QIODevice::ReadOnly);
    size_t sz = f.size();
    alignas (sizeof(int)) char data[f.size()];
    f.read(data,f.size());
    f.close();
    int row = 0;
    int * ptr = reinterpret_cast<int*>(data);
    while(reinterpret_cast<ptrdiff_t>(ptr) < reinterpret_cast<ptrdiff_t>(data) + static_cast<ptrdiff_t>(sz))
    {
        action a;
        a.type = (action_type)(*ptr);
        switch(a.type)
        {
        case action_type::pick_up_quest:
            ++ptr;
            a.questId = *ptr;
            break;
        case action_type::return_quest:
            ++ptr;
            a.questId = *ptr;
            ++ptr;
            a.itemrewardchoice = *ptr;
            break;
        case action_type::set_hearthstone:
            ++ptr;
            a.hs = (hearthstone_location)(*ptr);
            break;
        case action_type::use_hearthstone:
            ++ptr;
            a.hs = (hearthstone_location)(*ptr);
            break;
        case action_type::discover_flightmaster:
            ++ptr;
            a.flightmaster = (flightmaster_location)(*ptr);
            break;
        case action_type::use_flightmaster:
            ++ptr;
            a.flightmaster = (flightmaster_location)(*ptr);
            ++ptr;
            a.flightmaster_target = (flightmaster_location)(*ptr);
            break;
        case action_type::vendor:
        case action_type::training:
        case action_type::next_comment:
        {
            ++ptr;
            std::size_t sz = *ptr;
            ++ptr;
            a.vendorName = "";
            char * cptr = reinterpret_cast<char*>(ptr);
            for(std::size_t i = 0; i < sz; ++i)
            {
                a.vendorName.append(*cptr);
                cptr++;
            }
            ptr += sz * sizeof(char) / sizeof(int);
        }
            break;
        case action_type::reach_xp_breakpoint:
            ++ptr;
            a.setLevel(*ptr);
            ++ptr;
            a.setXp(*ptr);
            break;
        }
        insertItemOnRow(row,a);
        ++ptr;
        ++row;
    }
}

void actionitemmodel::saveActionsToFile(QFile &f)
{
    // Actions are saved in format [action_type,X,X,X,X,X...] where action_type determines the remainder of the entry
    std::vector<int> vec;

    for(int i = 0; i < model->rowCount(); ++i)
    {
        int type = model->item(i,1)->data().toInt();
        vec.push_back(type);
        switch((action_type)type)
        {
        case action_type::pick_up_quest:
            vec.push_back(model->item(i,2)->data().toInt());
            break;
        case action_type::return_quest:
            vec.push_back(model->item(i,2)->data().toInt());
            vec.push_back(model->item(i,4)->data().toInt());
            break;
        case action_type::set_hearthstone:
            vec.push_back(model->item(i,3)->data().toInt());
            break;
        case action_type::use_hearthstone:
            vec.push_back(model->item(i,3)->data().toInt());
            break;
        case action_type::discover_flightmaster:
            vec.push_back(model->item(i,5)->data().toInt());
            break;
        case action_type::use_flightmaster:
            vec.push_back(model->item(i,5)->data().toInt());
            vec.push_back(model->item(i,6)->data().toInt());
            break;
        case action_type::vendor:
        case action_type::training:
        case action_type::next_comment:
        {
            QString str = model->item(i,7)->data().toString();
            std::size_t sz = str.size();
            vec.push_back((int)sz);
            std::size_t idx = vec.size();
            for(int i = 0; i < sz * sizeof(char)/sizeof(int) + 1; ++i)
            {
                vec.push_back(0);
            }
            memcpy(reinterpret_cast<char*>(vec.data() + idx),str.toStdString().c_str(),sz);
        }
            break;

        case action_type::reach_xp_breakpoint:
            vec.push_back(model->item(i,2)->data().toInt());
            vec.push_back(model->item(i,4)->data().toInt());
            break;
        }
    }
    f.open(QIODevice::WriteOnly);
    f.write(reinterpret_cast<char*>(vec.data()),vec.size()*sizeof(int)/sizeof(char));
    f.close();
}

std::vector<std::tuple<int,questEntry>> actionitemmodel::xpRewardSequence() const
{
    std::vector<std::tuple<int,questEntry>> data;

    for(int i = 0; i < model->rowCount(); ++i)
    {
        if((action_type)(model->item(i,1)->data().toInt()) == action_type::return_quest)
        {
            int questId = model->item(i,2)->data().toInt();
            int preferredReturnLevel = 0;
            auto quest = d.get_quest(questId);

            data.push_back(std::tuple<int,questEntry>{preferredReturnLevel,quest});
        }
    }

    return data;
}

