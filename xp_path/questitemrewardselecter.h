#ifndef QUESTITEMREWARDSELECTER_H
#define QUESTITEMREWARDSELECTER_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class QuestItemRewardSelecter;
}

class QuestItemRewardSelecter : public QDialog
{
    Q_OBJECT
private:
    QList<int>  ids;
    QStringList items;
    bool isSelected_;
    int selectedId;
private slots:
    void setId(QModelIndex i);

public:
    explicit QuestItemRewardSelecter(std::pair<QList<int>, QStringList> p,QWidget *parent = 0);
    ~QuestItemRewardSelecter();

    bool isSelected() const
    {
        return isSelected_;
    }
    int selectedQuestItemReward() const
    {
        return selectedId;
    }

private:
    Ui::QuestItemRewardSelecter *ui;
};

#endif // QUESTITEMREWARDSELECTER_H
