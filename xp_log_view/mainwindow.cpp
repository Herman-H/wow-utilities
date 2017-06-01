#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(QString{"settings.ini"},QSettings::IniFormat),
    data(nullptr),
    plot(nullptr)
{
    ui->setupUi(this);
    QObject::connect(ui->actionLoad,SIGNAL(triggered(bool)),this,SLOT(onFileLoadAction(void)));

    load_file();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_file()
{
    QString file_path = settings.value("file_path","").toString();
    if(file_path.length() > 0)
    {
        if(QFile::exists(file_path))
        {
            if(data == nullptr || (*data).file_name() != file_path)
            {
                QFile f{file_path};
                if(f.open(QFile::ReadOnly | QFile::Text))
                {
                    data = std::unique_ptr<xp_vector>{new xp_vector{f}};
                    f.close();
                    plot = std::unique_ptr<XP_Plot>{new XP_Plot{&(*data),this}};
                    ui->verticalLayout->addWidget(&(*plot));
                }
            }
        }
    }
}

void MainWindow::onFileLoadAction(void)
{
    QString dir = settings.value("file_directory","").toString();
    QString fileName = QFileDialog::getOpenFileName(this, "Open Script", dir, "Lua Script (*.lua)");

    if(fileName.length() > 0)
    {
        settings.setValue("file_directory", QFileInfo(fileName).dir().absolutePath());
        settings.setValue("file_path", fileName);

        load_file();
    }
}


