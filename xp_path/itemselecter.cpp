#include "itemselecter.h"
#include "ui_itemselecter.h"
#include <QSqlQueryModel>
#include <QSqlQuery>

itemselecter::itemselecter(QSqlDatabase &db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::itemselecter),
    db(db),
    isSelected_(false)
{
    ui->setupUi(this);
    QSqlQueryModel * model = new QSqlQueryModel{};
    model->setQuery(QString{"SELECT entry,name,RequiredLevel FROM item_template"},db);
    model->setHeaderData(0,Qt::Horizontal,QString{"Entry"});
    model->setHeaderData(1,Qt::Horizontal,QString{"Name"});
    model->setHeaderData(2,Qt::Horizontal,QString{"Required Level"});
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QObject::connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onDoubleClickRow()));
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(onOk()));
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(onCancel()));
    QObject::connect(this,SIGNAL(rejected()),this,SLOT(onCancel()));
}

itemselecter::~itemselecter()
{
    delete ui;
}

void itemselecter::on_searchButton_clicked()
{
    QString query{"SELECT entry,name,required_level FROM item_template"};
    // Set the filter
    QString filter;
    bool isnotfirst = false;
    if(ui->minReqLvl->text().size() && ui->maxReqLvl->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("RequiredLevel > ");
        filter.append(ui->minReqLvl->text());
        filter.append(" AND RequiredLevel < ");
        filter.append(ui->maxReqLvl->text());
    }
    else if(ui->maxReqLvl->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("RequiredLevel < ");
        filter.append(ui->maxReqLvl->text());
    }
    else if(ui->minReqLvl->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("RequiredLevel > ");
        filter.append(ui->minReqLvl->text());
    }
    if(ui->itemName->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("name LIKE \'%");
        filter.append(ui->itemName->text());
        filter.append("%\'");
    }

    if(filter.size())
    {
        query.append(" WHERE ");
        query.append(filter);
    }

    QSqlQueryModel * model = static_cast<QSqlQueryModel*>(ui->tableView->model());
    model->setHeaderData(0,Qt::Horizontal,QString{"Entry"});
    model->setHeaderData(1,Qt::Horizontal,QString{"Title"});
    model->setHeaderData(2,Qt::Horizontal,QString{"Minimum Level"});
    model->setQuery(query,db);
}

void itemselecter::onCancel()
{
    isSelected_ = false;
}

QString itemselecter::getItemName(int itemid)
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

void itemselecter::onOk()
{
    if(ui->tableView->selectionModel()->hasSelection())
    {
        auto model = static_cast<QSqlQueryModel*>(ui->tableView->model());
        itemId_ = model->data(model->index(ui->tableView->selectionModel()->selectedRows().at(0).row(),0)).toInt();
        isSelected_ = true;
        accept();
    }
    else
    {
        isSelected_ = false;
        reject();
    }
}

void itemselecter::onDoubleClickRow()
{
    onOk();
}


