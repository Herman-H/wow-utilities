#include "actionselecterdialog.h"
#include "ui_actionselecterdialog.h"
#include "questselecter.h"
#include "hearthstonelocationselecter.h"
#include "flightmasterselecter.h"
#include "xpbreakpointselecter.h"
#include <QInputDialog>

ActionSelecterDialog::ActionSelecterDialog(QSqlDatabase &db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActionSelecterDialog),
    db(db),
    isSelected_(false)
{
    ui->setupUi(this);
    setWindowTitle("Select Action");
}

ActionSelecterDialog::~ActionSelecterDialog()
{
    delete ui;
}

void ActionSelecterDialog::on_typeselecter_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->selectButton->setEnabled(false);
        ui->selectButton->setText(QString{"Select..."});
    }
    if(index == 1)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Select Quest"});
        a.type = action_type::pick_up_quest;
    }
    else if(index == 2)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Select Quest"});
        a.type = action_type::return_quest;
    }
    else if(index == 3)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Select Location"});
        a.type = action_type::set_hearthstone;
    }
    else if(index == 4)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Select Location"});
        a.type = action_type::use_hearthstone;
    }
    else if(index == 5)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Select Flight Master"});
        a.type = action_type::discover_flightmaster;
    }
    else if(index == 6)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Select Flight Path"});
        a.type = action_type::use_flightmaster;
    }
    else if(index == 7)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Specify Vendor Name"});
        a.type = action_type::vendor;
    }
    else if(index == 8)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Specify Trainer Name"});
        a.type = action_type::training;
    }
    else if(index == 9)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Set XP and Level"});
        a.type = action_type::reach_xp_breakpoint;
    }
    else if(index == 10)
    {
        ui->selectButton->setEnabled(true);
        ui->selectButton->setText(QString{"Comment"});
        a.type = action_type::next_comment;
    }
}

void ActionSelecterDialog::on_selectButton_clicked()
{
    int index = ui->typeselecter->currentIndex();
    if(index == 1 || index == 2)
    {
        QuestSelecter selecter{db};
        selecter.setWindowTitle("Select Quest");
        if(index == 1)
            selecter.setPickupQuest();
        else
            selecter.setReturnQuest();
        selecter.exec();
        if(selecter.isSelected())
        {
            a.questId = selecter.selectedQuestId();
            a.itemrewardchoice = selecter.selectedQuestRewardItem();
            ui->selectButton->setText(QString::number(a.questId));
            isSelected_ = true;
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
    else if(index == 3 || index == 4)
    {
        hearthstonelocationselecter selecter;
        selecter.setWindowTitle("Select Hearthstone Location");
        selecter.exec();
        if(selecter.isSelected())
        {
            a.hs = selecter.selectedLocation();
            ui->selectButton->setText(hearthstone_location_string(a.hs));
            isSelected_ = true;
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
    else if(index == 5)
    {
        flightmasterSelecter selecter{false};
        selecter.setWindowTitle("Select Flightmaster");
        selecter.exec();
        if(selecter.isSelected())
        {
            a.flightmaster = selecter.selectedFlightmaster();
            ui->selectButton->setText(flightmaster_location_string(a.flightmaster));
            isSelected_ = true;
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
    else if(index == 6)
    {
        flightmasterSelecter selecter{true};
        selecter.setWindowTitle("Select Flight Path");
        selecter.exec();
        if(selecter.isSelected() && selecter.isTargetSelected())
        {
            a.flightmaster = selecter.selectedFlightmaster();
            a.flightmaster_target = selecter.selectedTarget();
            ui->selectButton->setText(flightmaster_location_string(a.flightmaster) + QString{" -> "} +
                                      flightmaster_location_string(a.flightmaster_target));
            isSelected_ = true;
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
    else if(index == 7)
    {
        bool ok;
        QString vendorname = QInputDialog::getText(this, QString{"Specify Vendor Name"},QString{"Vendor Name"},
                                                   QLineEdit::Normal,QString{},&ok);
        if(ok && !vendorname.isEmpty())
        {
            a.vendorName = vendorname;
            isSelected_ = true;
            ui->selectButton->setText(vendorname);
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
    else if(index == 8)
    {
        bool ok;
        QString trainername = QInputDialog::getText(this, QString{"Specify Trainer Name"},QString{"Trainer Name"},
                                                   QLineEdit::Normal,QString{},&ok);
        if(ok && !trainername.isEmpty())
        {
            a.vendorName = trainername;
            isSelected_ = true;
            ui->selectButton->setText(trainername);
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
    else if(index == 9)
    {
        XPBreakpointSelecter selecter;
        selecter.exec();
        if(selecter.isSelected())
        {
            a.setLevel(selecter.GetLevel());
            a.setXp(selecter.GetXP());
            ui->selectButton->setText(QString{"Lvl "} + QString::number(a.getLevel()) + QString{" and "} +
                                      QString::number(a.getXp()) + QString{" xp"});
            isSelected_ = true;
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
    else if(index == 10)
    {
        bool ok;
        QString comment = QInputDialog::getText(this, QString{"Add Comment"},QString{"Comment"},
                                                   QLineEdit::Normal,QString{},&ok);
        if(ok && !comment.isEmpty())
        {
            a.vendorName = comment;
            isSelected_ = true;
            ui->selectButton->setText(comment);
            accept();
        }
        else
        {
            isSelected_ = false;
        }
    }
}
