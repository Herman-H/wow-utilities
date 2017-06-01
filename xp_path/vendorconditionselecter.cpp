#include "vendorconditionselecter.h"
#include "ui_vendorconditionselecter.h"
#include "itemselecter.h"
#include "luastatementedit.h"

vendorconditionselecter::vendorconditionselecter(QSqlDatabase & db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vendorconditionselecter),
    db(db)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(rejected()));
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    isItemSelected_ = false;
    isConditionStmtSelected_ = false;
    isCountStmtSelected_ = false;
    enableOk();
}

vendorconditionselecter::~vendorconditionselecter()
{
    delete ui;
}

void vendorconditionselecter::enableOk()
{
    if(isItemSelected_ == true && isConditionStmtSelected_ == true && isCountStmtSelected_ == true)
    {
        ui->buttonBox->buttons().at(0)->setEnabled(true);
    }
    else
    {
        ui->buttonBox->buttons().at(0)->setEnabled(false);
    }
}

void vendorconditionselecter::on_itemButton_clicked()
{
    itemselecter is{db};
    is.exec();
    if(is.isSelected())
    {
        isItemSelected_ = true;
        itemId = is.selectedItemId();
        ui->itemButton->setText(is.getItemName(itemId));
    }
    else
    {
    }
    enableOk();
}

void vendorconditionselecter::on_conditionCode_clicked()
{
    LUAStatementEdit luaedit{QString{"condition : () -> { false | true }"}};
    if(isConditionStmtSelected_)
    {
        luaedit.setText(conditionStmt);
    }
    luaedit.exec();
    if(luaedit.isSelected())
    {
        isConditionStmtSelected_ = true;
        conditionStmt = luaedit.getStatement();
        ui->conditionCode->setText(conditionStmt.left(40));
    }
    else
    {
    }
    enableOk();
}

void vendorconditionselecter::on_countCode_clicked()
{
    LUAStatementEdit luaedit{QString{"count : () -> N"}};
    if(isCountStmtSelected_)
    {
        luaedit.setText(countStmt);
    }
    luaedit.exec();
    if(luaedit.isSelected())
    {
        isCountStmtSelected_ = true;
        countStmt = luaedit.getStatement();
        ui->countCode->setText(countStmt.left(40));
    }
    else
    {
    }
    enableOk();
}

void vendorconditionselecter::set(int itemId, const QString &condition, const QString &count)
{
    isItemSelected_ = true;
    isConditionStmtSelected_ = true;
    isCountStmtSelected_ = true;
    itemselecter is{db};
    this->itemId = itemId;
    ui->itemButton->setText(is.getItemName(itemId));
    conditionStmt = condition;
    ui->conditionCode->setText(conditionStmt.left(40));
    countStmt = count;
    ui->countCode->setText(countStmt.left(40));
    enableOk();
}









