#include "xprewardviewer.h"
#include "ui_xprewardviewer.h"
#include "levels.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>


XpRewardViewer::XpRewardViewer(std::vector<std::tuple<int, questEntry> > data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XpRewardViewer)
{
    ui->setupUi(this);

    window()->setWindowTitle("XP Reward Allocation");

    QLabel* bars[59];
    levels_data levelData;


    ui->gridLayout->addWidget(new QLabel{"Level"},0,0);
    ui->gridLayout->addWidget(new QLabel{"Required"},0,1);
    ui->gridLayout->addWidget(new QLabel{"Total"},0,3);

    // Generate 59 bars each representing one level
    for(int i = 0; i < 59; ++i)
    {
        // Add level and xp requirement labels
        QString level = QString::number(i+1) + QString{"-"} + QString::number(i+2);
        QLabel * levelLabel = new QLabel(level);
        levelLabel->setStyleSheet("QLabel { background-color : blue; min-height : 20; max-width : 50;}");
        levelLabel->setAlignment(Qt::AlignCenter);

        QString xpReq = QString::number(levelData.xp_per_level[i]);
        QLabel * xpReqLabel = new QLabel(xpReq);
        xpReqLabel->setStyleSheet("QLabel { background-color : green; min-height : 20; max-width : 50;}");
        xpReqLabel->setAlignment(Qt::AlignCenter);

        QString xpAcc = QString::number(levelData.total_xp_to_level_d[i+1]);
        QLabel * xpAccLabel = new QLabel(xpAcc);
        xpAccLabel->setStyleSheet("QLabel { background-color : yellow; min-height : 20; max-width : 50;}");
        xpAccLabel->setAlignment(Qt::AlignCenter);

        // Add the level bar
        QLabel * levelBar = new QLabel{};
        levelBar->setStyleSheet("QLabel { background-color : gold; min-height : 20;}");
        levelBar->setLayout(new QHBoxLayout{});
        levelBar->layout()->setMargin(0);
        levelBar->layout()->setSpacing(0);
        levelBar->setMargin(0);
        bars[i] = levelBar;

        ui->gridLayout->addWidget(levelLabel,i+1,0);
        ui->gridLayout->addWidget(xpReqLabel,i+1,1);
        ui->gridLayout->addWidget(levelBar,i+1,2);
        ui->gridLayout->addWidget(xpAccLabel,i+1,3);
    }

    int currentLevel = 1;
    int currentXpIntoLevel = 0;

    bool alternate = false;

    for(auto tpl : data)
    {
        int preference = std::get<0>(tpl);
        questEntry quest = std::get<1>(tpl);
        int questLevel = quest.questLevel;
        int value = quest.rewMoneyMaxLevel;
        if(preference > currentLevel)
        {
            // Add a stretchfactor to the level
            int thisLevel = levelData.xp_per_level[currentLevel-1];
            int remaining = thisLevel - currentXpIntoLevel;
            float factor = float(remaining)/float(thisLevel);
            QHBoxLayout * layout_ = reinterpret_cast<QHBoxLayout*>(bars[currentLevel-1]->layout());
            layout_->addStretch(255*factor);
            currentLevel = preference;
            currentXpIntoLevel = 0;
        }

        int xp = xpReward(currentLevel,questLevel,value);
        while(xp > 0 && currentLevel < 60)
        {
            int thisLevel = levelData.xp_per_level[currentLevel-1];
            int remaining = thisLevel - currentXpIntoLevel;
            if(xp > remaining)
            {
                float factor = float(remaining)/float(thisLevel);
                QLabel * lbl = new QLabel{QString::fromStdString(quest.questName)};
                lbl->setMinimumWidth(0);
                auto sp = lbl->sizePolicy();
                sp.setHorizontalPolicy(QSizePolicy::Ignored);
                lbl->setSizePolicy(sp);
                if(alternate)
                    lbl->setStyleSheet("QLabel { background-color : gray; max-height : 20}");
                else
                    lbl->setStyleSheet("QLabel { background-color : white; max-height : 20}");
                QHBoxLayout * layout_ = reinterpret_cast<QHBoxLayout*>(bars[currentLevel-1]->layout());
                layout_->addWidget(lbl,255*factor < 1 ? 1 : 255*factor);
                xp = xp-remaining;
                currentXpIntoLevel = 0;
                ++currentLevel;
            }
            else
            {
                float factor = float(xp)/float(thisLevel);
                QLabel * lbl = new QLabel{QString::fromStdString(quest.questName)};
                lbl->setMinimumWidth(0);
                auto sp = lbl->sizePolicy();
                sp.setHorizontalPolicy(QSizePolicy::Ignored);
                lbl->setSizePolicy(sp);
                if(alternate)
                    lbl->setStyleSheet("QLabel { background-color : gray; max-height : 20}");
                else
                    lbl->setStyleSheet("QLabel { background-color : white; max-height : 20}");
                QHBoxLayout * layout_ = reinterpret_cast<QHBoxLayout*>(bars[currentLevel-1]->layout());
                layout_->addWidget(lbl,255*factor < 1 ? 1 : 255*factor);
                currentXpIntoLevel+=xp;
                xp = 0;
            }
        }
        alternate = !alternate;
    }

    int thisLevel = levelData.xp_per_level[currentLevel-1];
    int remaining = thisLevel - currentXpIntoLevel;
    float factor = float(remaining)/float(thisLevel);
    QHBoxLayout * layout_ = reinterpret_cast<QHBoxLayout*>(bars[currentLevel-1]->layout());
    layout_->addStretch(255*factor);
}

int XpRewardViewer::xpReward(int playerLevel, int questLevel, int value) const
{
    if (value > 0)
    {
        float fullxp = 0;
        if (questLevel >= 65)
            fullxp = value / 6.0f;
        else if (questLevel == 64)
            fullxp = value / 4.8f;
        else if (questLevel == 63)
            fullxp = value / 3.6f;
        else if (questLevel == 62)
            fullxp = value / 2.4f;
        else if (questLevel == 61)
            fullxp = value / 1.2f;
        else if (questLevel > 0 && questLevel <= 60)
            fullxp = value / 0.6f;

        if (playerLevel <= questLevel +  5)
            return int(ceilf(fullxp));
        else if (playerLevel == questLevel +  6)
            return int(ceilf(fullxp * 0.8f));
        else if (playerLevel == questLevel +  7)
            return int(ceilf(fullxp * 0.6f));
        else if (playerLevel == questLevel +  8)
            return int(ceilf(fullxp * 0.4f));
        else if (playerLevel == questLevel +  9)
            return int(ceilf(fullxp * 0.2f));
        else
            return int(ceilf(fullxp * 0.1f));
    }
    else
    {
        return 0;
    }
}

XpRewardViewer::~XpRewardViewer()
{
    delete ui;
}
