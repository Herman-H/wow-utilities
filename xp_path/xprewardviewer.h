#ifndef XPREWARDVIEWER_H
#define XPREWARDVIEWER_H

#include <QDialog>
#include "questdata.h"

namespace Ui {
class XpRewardViewer;
}

class QHBoxLayout;

class XpRewardViewer : public QDialog
{
    Q_OBJECT

public:
    explicit XpRewardViewer(std::vector<std::tuple<int, questEntry> > data, QWidget *parent = 0);
    ~XpRewardViewer();
private:
    Ui::XpRewardViewer *ui;

    std::vector<QHBoxLayout*> levelLayouts;

    int xpReward(int playerLevel, int questLevel, int value) const;
};

#endif // XPREWARDVIEWER_H
