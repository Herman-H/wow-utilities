#ifndef VENDORCONDITIONMODEL_H
#define VENDORCONDITIONMODEL_H

#include <QStandardItemModel>
#include <QString>
#include <QSqlDatabase>
#include <QFile>

struct vendor_item
{
    int itemId;
    QString condition;
    QString count;
};


class vendorConditionModel
{
private:
    QStandardItemModel * model;
    QSqlDatabase & db;
    QString getItemName(int id) const;
public:
    vendorConditionModel(QSqlDatabase &db, QString countLabel);
    void insertItemOnRow(int row, vendor_item a);
    vendor_item getSelectedVendorItem(const QModelIndex &index);
    QStandardItemModel * getModel() const { return model; }
    void loadVendorDataFromFile(QFile & f);
    void saveVendorDataToFile(QFile & f);
};

#endif // VENDORCONDITIONMODEL_H
