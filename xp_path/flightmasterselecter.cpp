#include "flightmasterselecter.h"
#include "ui_flightmasterselecter.h"
#include "action.h"
#include <QLabel>

flightmasterSelecter::flightmasterSelecter(const bool isFlightPathSelecter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flightmasterSelecter),
    isFlightPathSelecter(isFlightPathSelecter),
    isSelected_(false),
    isTargetSelected_(false)
{
    ui->setupUi(this);
    fm = new QComboBox{};
    if(isFlightPathSelecter)
    {
        ui->l1->addWidget(new QLabel{"From:"});
        ui->l2->addWidget(new QLabel{"To:"});
        ui->l1->addWidget(fm);
        tar = new QComboBox{};
        ui->l2->addWidget(tar);
        fm->addItem("Select Flight Master...");
        tar->addItem("Select Flight Master...");
        for(unsigned int i = 0; i < (unsigned int)(flightmaster_location::COUNT); ++i)
        {
            fm->addItem(flightmaster_location_string(static_cast<flightmaster_location>(i)));
            tar->addItem(flightmaster_location_string(static_cast<flightmaster_location>(i)));
        }

        QObject::connect(fm,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelect(int)));
        QObject::connect(tar,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelectTarget(int)));
        QObject::connect(this,SIGNAL(rejected()),this,SLOT(onCancel()));
    }
    else
    {
        ui->l1->addWidget(fm);
        fm->addItem("Select Flight Master...");
        for(unsigned int i = 0; i < (unsigned int)(flightmaster_location::COUNT); ++i)
        {
            fm->addItem(flightmaster_location_string(static_cast<flightmaster_location>(i)));
        }
        QObject::connect(fm,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelect(int)));
        QObject::connect(this,SIGNAL(rejected()),this,SLOT(onCancel()));
    }

}

flightmasterSelecter::~flightmasterSelecter()
{
    delete ui;
}

void flightmasterSelecter::onSelect(int index)
{
    if(index == 0)
    {
        isSelected_ = false;
    }
    else
    {
        isSelected_ = true;
        flightmaster = static_cast<flightmaster_location>(index-1);
        if(isTargetSelected_ || !isFlightPathSelecter)
            accept();
    }
}

void flightmasterSelecter::onSelectTarget(int index)
{
    if(index == 0)
    {
        isTargetSelected_ = false;
    }
    else
    {
        isTargetSelected_ = true;
        target = static_cast<flightmaster_location>(index-1);
        if(isSelected_ || !isFlightPathSelecter)
            accept();
    }
}

void flightmasterSelecter::onCancel()
{
    isSelected_ = false;
    isTargetSelected_ = false;
}

void flightmasterSelecter::setFlightmaster(flightmaster_location loc)
{
    fm->setCurrentIndex(static_cast<int>(loc)+1);
}

void flightmasterSelecter::setTarget(flightmaster_location loc)
{
    tar->setCurrentIndex(static_cast<int>(loc)+1);
}
