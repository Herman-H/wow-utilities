#ifndef QUESTSELECTER_H
#define QUESTSELECTER_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class QuestSelecter;
}

class QuestSelecter : public QDialog
{
    Q_OBJECT

public:
    explicit QuestSelecter(QSqlDatabase & db, QWidget *parent = 0);
    ~QuestSelecter();

    bool isSelected() const
    {
        return isSelected_;
    }

    int selectedQuestId() const
    {
        return questId_;
    }

    int selectedQuestRewardItem() const
    {
        return rewardId_;
    }

    void setPickupQuest()
    {
        isPickupQuest = true;
    }

    void setReturnQuest()
    {
        isPickupQuest = false;
    }



private slots:
    void on_searchPushButton_clicked();
    void onDoubleClickRow();
    void onOk();
    void onCancel();

private:
    bool isPickupQuest;
    QString getItemName(int itemid);
    std::pair<QList<int>,QStringList> getQuestRewardChoiceList(int questid);

    void appendCommaIfNotFirst(QString & s, bool & b)
    {
        if(b)
            s.append(", ");
        b = true;
    }

    Ui::QuestSelecter *ui;
    QSqlDatabase &db;
    bool isSelected_;
    int questId_;
    int rewardId_;
};

#endif // QUESTSELECTER_H
