#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <memory>
#include "xp_vector.h"
#include "xp_plot.h"

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
    void onFileLoadAction(void);

private:
    Ui::MainWindow *ui;
    QSettings settings;

    std::unique_ptr<xp_vector> data;
    std::unique_ptr<XP_Plot> plot;

    void load_file();
};

#endif // MAINWINDOW_H
