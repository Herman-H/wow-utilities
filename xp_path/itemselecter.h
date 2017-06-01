#ifndef ITEMSELECTER_H
#define ITEMSELECTER_H

#include <QSqlDatabase>
#include <QDialog>

namespace Ui {
class itemselecter;
}

class itemselecter : public QDialog
{
    Q_OBJECT

public:
    explicit itemselecter(QSqlDatabase & db, QWidget *parent = 0);
    ~itemselecter();

    bool isSelected() const
    {
        return isSelected_;
    }

    int selectedItemId() const
    {
        return itemId_;
    }

    QString getItemName(int itemid);


private slots:
    void onDoubleClickRow();
    void onOk();
    void onCancel();

    void on_searchButton_clicked();

private:
    Ui::itemselecter *ui;

    void appendCommaIfNotFirst(QString & s, bool & b)
    {
        if(b)
            s.append(", ");
        b = true;
    }

    QSqlDatabase &db;
    bool isSelected_;
    int itemId_;
};


#endif // ITEMSELECTER_H
