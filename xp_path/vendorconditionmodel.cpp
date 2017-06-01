#include "vendorconditionmodel.h"
#include <QSqlQuery>
#include <QStandardItem>

vendorConditionModel::vendorConditionModel(QSqlDatabase & db, QString countLabel) :
    db(db)
{
    model = new QStandardItemModel{0,1};
    model->setHorizontalHeaderLabels({QString{"Item"},QString{"Condition"},countLabel});
}

QString vendorConditionModel::getItemName(int id) const
{
    QString q{"SELECT name FROM item_template WHERE entry = "};
    q.append(QString::number(id)).append(";");

    QSqlQuery query{db};
    query.exec(q);

    if(query.next())
    {
        return query.value(0).toString();
    }
    return QString{""};
}

vendor_item vendorConditionModel::getSelectedVendorItem(const QModelIndex &index)
{
    vendor_item v;
    auto condition = model->data(model->index(index.row(),1)).toString();
    auto count = model->data(model->index(index.row(),2)).toString();
    auto itemId = model->item(index.row(),3)->data().toInt();
    v.itemId = itemId;
    v.condition = condition;
    v.count = count;

    return v;
}

void vendorConditionModel::insertItemOnRow(int row, vendor_item v)
{
    QStandardItem *itemName,*itemId,*condition,*count;
    itemName = new QStandardItem{getItemName(v.itemId)};
    condition = new QStandardItem{v.condition};
    count = new QStandardItem{v.count};
    itemId = new QStandardItem{};
    itemId->setData(v.itemId);
    QList<QStandardItem*> l;
    l.push_back(itemName);
    l.push_back(condition);
    l.push_back(count);
    l.push_back(itemId);
    model->insertRow(row,l);
}

void vendorConditionModel::loadVendorDataFromFile(QFile &f)
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
        vendor_item v;
        v.itemId = (int)(*ptr);
        ++ptr;
        size_t string1size = (size_t)(*ptr);
        QString string1{""};
        ++ptr;
        char * cptr = reinterpret_cast<char*>(ptr);
        for(size_t i = 0; i < string1size; ++i)
        {
            string1.append(*cptr);
            cptr++;
        }
        v.condition = string1;
        ptr += string1size * sizeof(char) / sizeof(int) + 1;
        size_t string2size = (size_t)(*ptr);
        QString string2{""};
        ++ptr;
        cptr = reinterpret_cast<char*>(ptr);
        for(size_t i = 0; i < string2size; ++i)
        {
            string2.append(*cptr);
            cptr++;
        }
        v.count = string2;
        insertItemOnRow(row,v);
        ptr += string2size * sizeof(char) / sizeof(int) + 1;
        ++row;
    }
}

void vendorConditionModel::saveVendorDataToFile(QFile &f)
{
    std::vector<int> vec;

    for(int i = 0; i < model->rowCount(); ++i)
    {
        int itemId = model->item(i,3)->data().toInt();
        QString condition = model->item(i,1)->data(Qt::DisplayRole).toString();
        QString count = model->item(i,2)->data(Qt::DisplayRole).toString();
        vec.push_back(itemId);
        /* condition */
        vec.push_back(condition.size());
        int idx = vec.size();
        std::string s = condition.toStdString();
        for(int i = 0; i < condition.size() * sizeof(char) / sizeof(int) + 1; ++i)
        {
            vec.push_back(0);
        }
        memcpy(reinterpret_cast<void*>(vec.data()+idx),s.c_str(),condition.size());
        /* count */
        vec.push_back(count.size());
        idx = vec.size();
        s = count.toStdString();
        for(int i = 0; i < count.size() * sizeof(char) / sizeof(int) + 1; ++i)
        {
            vec.push_back(0);
        }
        memcpy(reinterpret_cast<void*>(vec.data()+idx),s.c_str(),count.size());
    }
    f.open(QIODevice::WriteOnly);
    f.write(reinterpret_cast<char*>(vec.data()),vec.size()*sizeof(int)/sizeof(char));
    f.close();

}
