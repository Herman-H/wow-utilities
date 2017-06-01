#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "actionselecterdialog.h"
#include "vendorconditionselecter.h"
#include "generateaddon.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(QSqlDatabase::addDatabase("QMYSQL")),
    d(db),
    am(d),
    working_set(&d),
    completed_set(&d),
    vendordontsellmodel(db,QString{"Minimum"}),
    vendorbuymodel(db,QString{"Maximum"}),
    state(&d,&working_set,&completed_set),
    settings("settings.ini",QSettings::IniFormat),
    isSimulationStarted(false),
    isSimulationEnded(false)
{
    ui->setupUi(this);
    db.setHostName(QString{"127.0.0.1"});
    db.setDatabaseName(QString{"mangosclassic"});
    db.setUserName(QString{"root"});
    db.setPassword(QString{"hummer10"});
    db.open();

    ui->resetSimButton->setEnabled(false);

    ui->actionListView->setModel(am.getModel());
    ui->actionListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->actionListView->setSelectionMode(QAbstractItemView::SingleSelection);
    enableRunToSelButton();

    ui->workingSetTable->setModel(working_set.getModel());
    ui->workingSetTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->completedSetTable->setModel(completed_set.getModel());
    ui->completedSetTable->setSelectionMode(QAbstractItemView::NoSelection);

    ui->buyTableView->setModel(vendorbuymodel.getModel());
    ui->buyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->buyTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->buyTableView->setColumnHidden(3,true);
    ui->buyTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dontSellTableView->setModel(vendordontsellmodel.getModel());
    ui->dontSellTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dontSellTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dontSellTableView->setColumnHidden(3,true);
    ui->dontSellTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    load_actions_data(settings.value("actions_file", "").toString());
    load_buy_data(settings.value("buy_file", "").toString());
    load_sell_data(settings.value("sell_file", "").toString());

    ui->hearthstoneOutputLabel->setText(hearthstone_location_string(state.getCurrentHearthstoneLocation()));

    ui->dontSellEditButton->setEnabled(ui->dontSellTableView->selectionModel()->hasSelection());
    ui->buyEditButton->setEnabled(ui->buyTableView->selectionModel()->hasSelection());

    QObject::connect(ui->actionListView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(enableRunToSelButton()));
    QObject::connect(ui->actionListView,SIGNAL(clicked(QModelIndex)),this,SLOT(enableRunToSelButton()));
    QObject::connect(ui->dontSellTableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(onDontSellSelection()));
    QObject::connect(ui->buyTableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(onBuySelection()));

    QObject::connect(ui->actionActions,SIGNAL(triggered(bool)),this,SLOT(loadActionsFile()));
    QObject::connect(ui->actionBuy_List,SIGNAL(triggered(bool)),this,SLOT(loadBuyListFile()));
    QObject::connect(ui->actionSell_List,SIGNAL(triggered(bool)),this,SLOT(loadSellListFile()));

    QObject::connect(ui->actionGenerate_Addon, SIGNAL(triggered(bool)),this,SLOT(onGenerateAddon()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_insertButton_clicked()
{
    ActionSelecterDialog selecter(db);
    selecter.exec();

    if(selecter.isSelected())
    {
        action a = selecter.getSelectedAction();
        // Just insert selecter.getSelectedAction()
        int row;
        if(ui->actionListView->selectionModel()->hasSelection())
        {
            row = ui->actionListView->currentIndex().row()+1;
        }
        else
        {
            row = am.getModel()->rowCount();
        }
        am.insertItemOnRow(row,a);
        on_resetSimButton_clicked(); // The state could have been invalidated at this point, simply reset
    }
}

void MainWindow::on_removeButton_clicked()
{
    if(ui->actionListView->selectionModel()->hasSelection())
    {
        am.getModel()->removeRow(ui->actionListView->currentIndex().row());
    }
}

void MainWindow::step()
{
    state.executeAction(am.getSelectedAction(simStepIndex));

    // Update state view
    ui->minLevelReqOutputLabel->setText(QString::number(state.getMinLevel()));
    ui->workingSetStatusOutputLabel->setText(QString::number(state.getWorkingSetSize()));
    ui->completedSetStatusOutputLabel->setText(QString::number(state.getCompletedSetSize()));
    ui->hearthstoneOutputLabel->setText(hearthstone_location_string(state.getCurrentHearthstoneLocation()));

    displayErrors();
}

void MainWindow::on_stepSimButton_clicked()
{
    if(isSimulationStarted)
    {
        if(!ui->actionListView->selectionModel()->hasSelection())
            return;
        if(!state.wasLastActionPermitted())
            return;
        if(!isSimulationEnded)
        {
            step();
            ui->actionListView->setCurrentIndex(simStepIndex);
            ui->resetSimButton->setEnabled(true);
        }
        if(simStepIndex.row() < am.getModel()->rowCount() - 1)
        {
            simStepIndex = simStepIndex.sibling(simStepIndex.row()+1,0);
        }
        else
        {
            isSimulationEnded = true;
            ui->stepSimButton->setEnabled(false);
            ui->runToSelSimButton->setEnabled(false);
            ui->runSimButton->setEnabled(false);
        }
    }
    else
    {
        if(am.getModel()->rowCount() == 0)
            return;
        ui->actionListView->setCurrentIndex(simStepIndex = am.getModel()->index(0,0));
        isSimulationStarted = true;
        on_stepSimButton_clicked();
    }
}

void MainWindow::displayErrors()
{
    for(int i = 0; i < errorLabels.size(); ++i)
    {
        ui->errorDisplayLayout->removeWidget(errorLabels.at(i));
        delete errorLabels.at(i);
    }
    errorLabels.clear();
    errorLabels.push_back(new QLabel{state.getErrorMessageHeader()});

    QPalette palette;
    if(state.wasLastActionPermitted())
    {
        palette.setColor(QPalette::WindowText,Qt::green);
        errorLabels.at(0)->setPalette(palette);
    }
    else
    {
        palette.setColor(QPalette::WindowText,Qt::red);
        errorLabels.at(0)->setPalette(palette);

        QStringList l = state.getErrorMessages();
        QString s{QString::number(l.size())};
        s.append(l.size() > 1 ? " errors." : " error.");
        errorLabels.push_back(new QLabel{s});

        for(int k = 0; k < l.size(); ++k)
        {
            errorLabels.push_back(new QLabel{l.at(k)});
        }
    }

    for(int i = 0; i < errorLabels.size(); ++i)
    {
        ui->errorDisplayLayout->addWidget(errorLabels.at(i));
    }
}

void MainWindow::on_resetSimButton_clicked()
{
    isSimulationStarted = false;
    isSimulationEnded = false;
    state.reset();
    displayErrors();
    ui->minLevelReqOutputLabel->setText("1");
    ui->workingSetStatusOutputLabel->setText("0");
    ui->stepSimButton->setEnabled(true);
    ui->runSimButton->setEnabled(true);
    ui->resetSimButton->setEnabled(false);
}

void MainWindow::on_runToSelSimButton_clicked()
{
    if(ui->actionListView->selectionModel()->hasSelection())
    {
        QModelIndex i = ui->actionListView->currentIndex();
        if(!isSimulationStarted)
        {
            on_stepSimButton_clicked();
        }
        for(int j = i.row() + 1 - simStepIndex.row(); j != 0; --j)
        {
            on_stepSimButton_clicked();
            if(!state.wasLastActionPermitted())
                break;
        }
        ui->runToSelSimButton->setEnabled(false);
    }
}

void MainWindow::enableRunToSelButton()
{
    bool enable = ui->actionListView->selectionModel()->hasSelection();
    QModelIndex i = ui->actionListView->currentIndex();
    enable = enable && (!isSimulationStarted || (isSimulationStarted && simStepIndex.row() < i.row() + 1));
    ui->runToSelSimButton->setEnabled(enable);
}

void MainWindow::on_runSimButton_clicked()
{
    if(!isSimulationStarted)
    {
        on_stepSimButton_clicked();
    }

    for(int j = am.getModel()->rowCount() - simStepIndex.row(); j != 0; --j)
    {
        on_stepSimButton_clicked();
        if(!state.wasLastActionPermitted())
            break;
    }
}

void MainWindow::on_dontSellAddButton_clicked()
{
    vendorconditionselecter selecter{db};
    selecter.exec();
    if(selecter.isSelected())
    {
        vendor_item a;
        a.itemId = selecter.getItemId();
        a.condition = selecter.getCondition();
        a.count = selecter.getCount();

        // Just insert selecter.getSelectedAction()
        int row;
        if(ui->dontSellTableView->selectionModel()->hasSelection())
        {
            row = ui->dontSellTableView->currentIndex().row()+1;
        }
        else
        {
            row = vendordontsellmodel.getModel()->rowCount();
        }
        vendordontsellmodel.insertItemOnRow(row,a);
        ui->dontSellTableView->setColumnHidden(3,true);
    }
}

void MainWindow::on_dontSellRemoveButton_clicked()
{
    if(ui->dontSellTableView->selectionModel()->hasSelection())
    {
        vendordontsellmodel.getModel()->removeRow(ui->dontSellTableView->currentIndex().row());
    }
}

void MainWindow::on_buyAddButton_clicked()
{
    vendorconditionselecter selecter{db};
    selecter.exec();
    if(selecter.isSelected())
    {
        vendor_item a;
        a.itemId = selecter.getItemId();
        a.condition = selecter.getCondition();
        a.count = selecter.getCount();

        // Just insert selecter.getSelectedAction()
        int row;
        if(ui->buyTableView->selectionModel()->hasSelection())
        {
            row = ui->buyTableView->currentIndex().row()+1;
        }
        else
        {
            row = vendorbuymodel.getModel()->rowCount();
        }
        vendorbuymodel.insertItemOnRow(row,a);
        ui->buyTableView->setColumnHidden(3,true);
    }
}

void MainWindow::on_buyRemoveButton_clicked()
{
    if(ui->buyTableView->selectionModel()->hasSelection())
    {
        vendorbuymodel.getModel()->removeRow(ui->buyTableView->currentIndex().row());
    }
}

void MainWindow::onDontSellSelection()
{
    ui->dontSellEditButton->setEnabled(ui->dontSellTableView->selectionModel()->hasSelection());
}

void MainWindow::onBuySelection()
{
    ui->buyEditButton->setEnabled(ui->buyTableView->selectionModel()->hasSelection());
}

void MainWindow::on_dontSellEditButton_clicked()
{
    vendorconditionselecter selecter{db};
    QModelIndex m = ui->dontSellTableView->selectionModel()->selectedIndexes().at(0);
    vendor_item v = vendordontsellmodel.getSelectedVendorItem(m);
    selecter.set(v.itemId,v.condition,v.count);
    selecter.exec();
    if(selecter.isSelected())
    {
        vendor_item a;
        a.itemId = selecter.getItemId();
        a.condition = selecter.getCondition();
        a.count = selecter.getCount();

        // Just insert selecter.getSelectedAction()
        int row;
        if(ui->dontSellTableView->selectionModel()->hasSelection())
        {
            row = ui->dontSellTableView->currentIndex().row();
        }
        else
        {
            row = vendordontsellmodel.getModel()->rowCount();
        }
        vendordontsellmodel.getModel()->removeRow(row);
        vendordontsellmodel.insertItemOnRow(row,a);
        ui->dontSellTableView->setColumnHidden(3,true);
    }
}

void MainWindow::on_buyEditButton_clicked()
{
    vendorconditionselecter selecter{db};
    QModelIndex m = ui->buyTableView->selectionModel()->selectedIndexes().at(0);
    vendor_item v = vendorbuymodel.getSelectedVendorItem(m);
    selecter.set(v.itemId,v.condition,v.count);
    selecter.exec();
    if(selecter.isSelected())
    {
        vendor_item a;
        a.itemId = selecter.getItemId();
        a.condition = selecter.getCondition();
        a.count = selecter.getCount();

        // Just insert selecter.getSelectedAction()
        int row;
        if(ui->buyTableView->selectionModel()->hasSelection())
        {
            row = ui->buyTableView->currentIndex().row();
        }
        else
        {
            row = vendorbuymodel.getModel()->rowCount();
        }
        vendorbuymodel.getModel()->removeRow(row);
        vendorbuymodel.insertItemOnRow(row,a);
        ui->buyTableView->setColumnHidden(3,true);
    }
}

void MainWindow::load_actions_data(const QString & fileName)
{
    if(fileName.length() != 0)
    {
        file.setFileName(fileName);
        am.loadActionsFromFile(file);
    }
}

void MainWindow::load_buy_data(const QString &fileName)
{
    if(fileName.length() != 0)
    {
        file2.setFileName(fileName);
        vendorbuymodel.loadVendorDataFromFile(file2);
        ui->buyTableView->setColumnHidden(3,true);
    }
}

void MainWindow::load_sell_data(const QString &fileName)
{
    if(fileName.length() != 0)
    {
        file1.setFileName(fileName);
        vendordontsellmodel.loadVendorDataFromFile(file1);
        ui->dontSellTableView->setColumnHidden(3,true);
    }
}

void MainWindow::loadActionsFile()
{
    // Set the settings to the selected file and the load this file into the UI
    QString fileName = QFileDialog::getOpenFileName(this, "Select Action Sequence File", "", "(*.seq)");
    if(fileName.length() > 0)
    {
        settings.setValue("actions_file",fileName);
        load_actions_data(fileName);
    }
}
void MainWindow::loadBuyListFile()
{
    // Set the settings to the selected file and the load this file into the UI
    QString fileName = QFileDialog::getOpenFileName(this, "Select Buy List File", "", "(*.ven)");
    if(fileName.length() > 0)
    {
        settings.setValue("buy_file",fileName);
        load_buy_data(fileName);
    }
}
void MainWindow::loadSellListFile()
{
    // Set the settings to the selected file and the load this file into the UI
    QString fileName = QFileDialog::getOpenFileName(this, "Select Sell List File", "", "(*.ven)");
    if(fileName.length() > 0)
    {
        settings.setValue("sell_file",fileName);
        load_sell_data(fileName);
    }
}

void MainWindow::on_saveActionsButton_clicked()
{
    if(QFile::exists(file.fileName()) ? true : (file.setFileName(QFileDialog::getSaveFileName(this, "Create File"))
                                     ,(QFile::exists(file.fileName()))))
    {
        settings.setValue("actions_file",file.fileName());
        am.saveActionsToFile(file);
        qDebug("Save Succeeded");
    }
    else
    {
        qDebug("Save Failed");
    }
}

void MainWindow::on_buySaveButton_clicked()
{
    if(QFile::exists(file.fileName()) ? true : (file2.setFileName(QFileDialog::getSaveFileName(this, "Create File"))
                                     ,(QFile::exists(file2.fileName()))))
    {
        settings.setValue("buy_file",file2.fileName());
        vendorbuymodel.saveVendorDataToFile(file2);
        qDebug("Save Succeeded");
    }
    else
    {
        qDebug("Save Failed");
    }
}

void MainWindow::on_dontSellSaveButton_clicked()
{
    if(QFile::exists(file.fileName()) ? true : (file1.setFileName(QFileDialog::getSaveFileName(this, "Create File"))
                                     ,(QFile::exists(file1.fileName()))))
    {
        settings.setValue("sell_file",file1.fileName());
        vendordontsellmodel.saveVendorDataToFile(file1);
        qDebug("Save Succeeded");
    }
    else
    {
        qDebug("Save Failed");
    }
}

void MainWindow::onGenerateAddon()
{
    generateAddon addonGenerator{am,vendordontsellmodel,vendorbuymodel};
    QString path{"C:/Spel/World of Warcraft 1.12/Interface/AddOns"};
    addonGenerator.generate(path, "Test", "Sol", "Just testing", "0.1");
}
