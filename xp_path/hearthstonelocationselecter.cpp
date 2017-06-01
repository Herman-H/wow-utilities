#include "hearthstonelocationselecter.h"
#include "ui_hearthstonelocationselecter.h"

#include "action.h"

hearthstonelocationselecter::hearthstonelocationselecter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hearthstonelocationselecter),
    isSelected_(false)
{
    ui->setupUi(this);

    for(int i = 0; i < (int)hearthstone_location::COUNT; ++i)
    {
        ui->listWidget->addItem(hearthstone_location_string((hearthstone_location)i));
    }

    QObject::connect(this,SIGNAL(rejected()),this,SLOT(onClose()));
}

hearthstonelocationselecter::~hearthstonelocationselecter()
{
    delete ui;
}

void hearthstonelocationselecter::onClose()
{
    isSelected_ = false || isSelected_;
}

void hearthstonelocationselecter::on_listWidget_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    isSelected_ = true;
    int row = index.row();
    hs = (hearthstone_location)row;

    close();
}
