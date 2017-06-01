#include "luastatementedit.h"
#include "ui_luastatementedit.h"

LUAStatementEdit::LUAStatementEdit(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LUAStatementEdit)
{
    ui->setupUi(this);
    window()->setWindowTitle(title);

    QObject::connect(this,SIGNAL(rejected()),this,SLOT(on_buttonBox_rejected()));
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
}

LUAStatementEdit::~LUAStatementEdit()
{
    delete ui;
}

void LUAStatementEdit::setText(const QString &text)
{
    ui->textEdit->setPlainText(text);
}

QString LUAStatementEdit::getStatement() const
{
    return ui->textEdit->toPlainText();
}

void LUAStatementEdit::on_buttonBox_accepted()
{
    isSelected_ = true;
}

void LUAStatementEdit::on_buttonBox_rejected()
{
    isSelected_ = false;
}
