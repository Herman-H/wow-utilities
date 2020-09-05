#ifndef ACTIONSELECTERDIALOG_H
#define ACTIONSELECTERDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include "action.h"

namespace Ui {
class ActionSelecterDialog;
}

class ActionSelecterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActionSelecterDialog(QSqlDatabase &db, QWidget *parent = 0);
    ~ActionSelecterDialog();

    bool isSelected() const
    {
        return isSelected_;
    }

    action getSelectedAction() const
    {
        return a;
    }

    void edit(action a_);

private slots:
    void on_typeselecter_currentIndexChanged(int index);
    void on_selectButton_clicked();

private:
    Ui::ActionSelecterDialog *ui;
    QSqlDatabase &db;
    bool isSelected_;
    action a;

};

#endif // ACTIONSELECTERDIALOG_H
