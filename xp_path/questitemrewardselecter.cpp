#include "questitemrewardselecter.h"
#include "ui_questitemrewardselecter.h"

QuestItemRewardSelecter::QuestItemRewardSelecter(std::pair<QList<int>, QStringList> p, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestItemRewardSelecter),
    ids(p.first),
    items(p.second)
{
    ui->setupUi(this);
    setWindowTitle("Select Quest Reward");
    for(int i = 0; i < ids.size(); ++i)
    {
        ui->listWidget->addItem(items[i]);
    }

    selectedId = 0;
    isSelected_ = ids.size() == 0;

    QObject::connect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setId(QModelIndex)));
    QObject::connect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
}

QuestItemRewardSelecter::~QuestItemRewardSelecter()
{
    QObject::disconnect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setId(QModelIndex)));
    QObject::disconnect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    delete ui;
}


void QuestItemRewardSelecter::setId(QModelIndex i)
{
    isSelected_ = true;
    selectedId = ids[i.row()];
}

