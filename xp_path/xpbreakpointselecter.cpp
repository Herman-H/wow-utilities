#include "xpbreakpointselecter.h"
#include "ui_xpbreakpointselecter.h"
#include <QValidator>

XPBreakpointSelecter::XPBreakpointSelecter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XPBreakpointSelecter),
    ok(false)
{
    ui->setupUi(this);

    ui->level->setValidator(new QIntValidator{1,59});
    ui->xp->setValidator(new QIntValidator{0,209800});

    QObject::connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(setValues()));
    QObject::connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(onCancelButton()));
}

XPBreakpointSelecter::~XPBreakpointSelecter()
{
    delete ui;
}

int XPBreakpointSelecter::GetLevel() const
{
    return level;
}

int XPBreakpointSelecter::GetXP() const
{
    return xp;
}

void XPBreakpointSelecter::setValues()
{
    level = ui->level->text().toInt();
    xp = ui->xp->text().toInt();
    ok = true;

    accept();
}

void XPBreakpointSelecter::onCancelButton()
{
    reject();
}
