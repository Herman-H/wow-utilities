#ifndef LUASTATEMENTEDIT_H
#define LUASTATEMENTEDIT_H

#include <QDialog>
#include <QString>

namespace Ui {
class LUAStatementEdit;
}

class LUAStatementEdit : public QDialog
{
    Q_OBJECT

public:
    explicit LUAStatementEdit(QString title, QWidget *parent = 0);
    ~LUAStatementEdit();

    bool isSelected() const { return isSelected_; }
    QString getStatement() const;
    void setText(const QString & text);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::LUAStatementEdit *ui;
    bool isSelected_;
};

#endif // LUASTATEMENTEDIT_H
