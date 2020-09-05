#include "questselecter.h"
#include "ui_questselecter.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "questitemrewardselecter.h"

QuestSelecter::QuestSelecter(QSqlDatabase &db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestSelecter),
    db(db),
    isSelected_(false)
{
    ui->setupUi(this);
    QSqlQueryModel * model = new QSqlQueryModel{};
    model->setQuery(QString{"SELECT entry,Title,MinLevel,RewMoneyMaxLevel/0.6 FROM quest_template"},db);
    model->setHeaderData(0,Qt::Horizontal,QString{"Entry"});
    model->setHeaderData(1,Qt::Horizontal,QString{"Title"});
    model->setHeaderData(2,Qt::Horizontal,QString{"Minimum Level"});
    model->setHeaderData(3,Qt::Horizontal,QString{"XP Reward"});
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QObject::connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onDoubleClickRow()));
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(onOk()));
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(onCancel()));
    QObject::connect(this,SIGNAL(rejected()),this,SLOT(onCancel()));
}

QuestSelecter::~QuestSelecter()
{
    delete ui;
}

void QuestSelecter::on_searchPushButton_clicked()
{
    QString query{"SELECT entry,Title,MinLevel,RewMoneyMaxLevel/0.6 FROM quest_template"};
    // Set the filter
    QString filter;
    bool isnotfirst = false;
    if(ui->minLevelReq->text().size() && ui->maxLevelReq->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("MinLevel > ");
        filter.append(ui->minLevelReq->text());
        filter.append(" AND MinLevel < ");
        filter.append(ui->maxLevelReq->text());
    }
    else if(ui->maxLevelReq->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("MinLevel < ");
        filter.append(ui->maxLevelReq->text());
    }
    else if(ui->minLevelReq->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("MinLevel > ");
        filter.append(ui->minLevelReq->text());
    }
    if(ui->questTitle->text().size())
    {
        appendCommaIfNotFirst(filter,isnotfirst);
        filter.append("Title LIKE \'%");
        filter.append(ui->questTitle->text());
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
    model->setHeaderData(3,Qt::Horizontal,QString{"XP Reward"});
    model->setQuery(query,db);
}

void QuestSelecter::onCancel()
{
    isSelected_ = false;
}

QString QuestSelecter::getItemName(int itemid)
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

std::pair<QList<int>,QStringList> QuestSelecter::getQuestRewardChoiceList(int questid)
{
    QString q{"SELECT RewChoiceItemId1,RewChoiceItemId2,RewChoiceItemId3,RewChoiceItemId4,RewChoiceItemId5,RewChoiceItemId6"
              " FROM quest_template WHERE entry = "};
    q.append(QString::number(questid)).append(";");

    QSqlQuery query{db};
    query.exec(q);

    QList<int> l1;
    QStringList l2;
    if(query.next())
    {
        for(int i = 0; i < 6; ++i)
        {
            int r = query.value(i).toInt();
            if(r != 0)
            {
                l1.push_back(r);
                l2.push_back(getItemName(r));
            }
        }
    }
    return std::pair<QList<int>,QStringList>{l1,l2};
}


void QuestSelecter::onOk()
{
    if(ui->tableView->selectionModel()->hasSelection())
    {
        auto model = static_cast<QSqlQueryModel*>(ui->tableView->model());
        questId_ = model->data(model->index(ui->tableView->selectionModel()->selectedRows().at(0).row(),0)).toInt();

        if(!isPickupQuest)
        {
            QuestItemRewardSelecter rws{getQuestRewardChoiceList(questId_)};
            if(!rws.isSelected())
                rws.exec();
            rewardId_ = rws.selectedQuestItemReward();
            isSelected_ = rws.isSelected();
        }
        else
        {
            isSelected_ = true;
        }

        if(isSelected_)
            accept();
    }
    else
    {
        isSelected_ = false;
        reject();
    }
}

void QuestSelecter::onDoubleClickRow()
{
    onOk();
}




















