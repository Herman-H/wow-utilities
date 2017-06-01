#ifndef VENDORCONDITIONSELECTER_H
#define VENDORCONDITIONSELECTER_H

#include <QDialog>
#include <QString>
#include <QSqlDatabase>

namespace Ui {
class vendorconditionselecter;
}

class vendorconditionselecter : public QDialog
{
    Q_OBJECT

public:
    explicit vendorconditionselecter(QSqlDatabase &db, QWidget *parent = 0);
    ~vendorconditionselecter();

    bool isSelected() const { return isItemSelected_ && isCountStmtSelected_ && isConditionStmtSelected_; }

    int getItemId() const { return itemId; }
    QString getCondition() const { return conditionStmt; }
    QString getCount() const { return countStmt; }

    void set(int itemId, const QString & condition, const QString & count);

private slots:
    void on_itemButton_clicked();
    void on_conditionCode_clicked();
    void on_countCode_clicked();
    void rejected() { isItemSelected_ = false;isCountStmtSelected_ = false; isConditionStmtSelected_ = false; }

private:
    Ui::vendorconditionselecter *ui;
    QSqlDatabase & db;
    bool isItemSelected_;
    bool isCountStmtSelected_;
    bool isConditionStmtSelected_;
    int itemId;
    QString countStmt;
    QString conditionStmt;

    void enableOk();
};

#endif // VENDORCONDITIONSELECTER_H
