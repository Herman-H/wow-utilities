#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QLabel>
#include <QSettings>

#include "actionitemmodel.h"
#include "trainertablemodel.h"
#include "questsetmodel.h"
#include "vendorconditionmodel.h"

#include "simulation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_insertButton_clicked();
    void on_editButton_clicked();
    void on_removeButton_clicked();
    void on_stepSimButton_clicked();
    void on_resetSimButton_clicked();
    void on_runToSelSimButton_clicked();
    void enableRunToSelButton();
    void on_runSimButton_clicked();
    void on_viewXpRewardButton_clicked();

    void on_dontSellAddButton_clicked();

    void on_dontSellRemoveButton_clicked();

    void on_buyAddButton_clicked();

    void on_buyRemoveButton_clicked();

    void on_dontSellEditButton_clicked();

    void on_buyEditButton_clicked();

    void onDontSellSelection();
    void onBuySelection();

    void loadActionsFile();
    void loadBuyListFile();
    void loadSellListFile();


    void on_saveActionsButton_clicked();

    void on_buySaveButton_clicked();

    void on_dontSellSaveButton_clicked();

    void onGenerateAddon112();
    void onGenerateAddonClassic();

    void on_trainerSavePushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    questData d;
    actiontablemodel atm;
    questsetmodel working_set;
    questsetmodel completed_set;
    vendorConditionModel vendordontsellmodel;
    vendorConditionModel vendorbuymodel;
    simulation_state state;
    QSettings settings;
    QFile file;
    QFile file1;
    QFile file2;
    QFile file3;

    bool isSimulationStarted;
    bool isSimulationEnded;
    QModelIndex simStepIndex;
    QModelIndex simTempIndex;


    QList<QLabel*> errorLabels;
    void displayErrors();
    void step();
    void load_actions_data(const QString & fileName);
    void load_sell_data(const QString & fileName);
    void load_buy_data(const QString & fileName);
    void load_trainer_data(const QString & fileName);

    enum trainerType {
        WARLOCK,
        TAILORING
    };

    std::vector<trainertablemodel*> trainerModels;
};

#endif // MAINWINDOW_H
