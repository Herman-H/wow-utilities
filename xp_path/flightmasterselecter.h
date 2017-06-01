#ifndef FLIGHTMASTERSELECTER_H
#define FLIGHTMASTERSELECTER_H

#include <QDialog>
#include <QComboBox>
#include "action.h"

namespace Ui {
class flightmasterSelecter;
}

class flightmasterSelecter : public QDialog
{
    Q_OBJECT

public:
    explicit flightmasterSelecter(const bool isFlightPathSelecter, QWidget *parent = 0);
    ~flightmasterSelecter();
    bool isSelected() const
    {
        return isSelected_;
    }
    bool isTargetSelected() const
    {
        return isTargetSelected_;
    }

    flightmaster_location selectedFlightmaster() const
    {
        return flightmaster;
    }

    flightmaster_location selectedTarget() const
    {
        return target;
    }

private slots:
    void onSelect(int index);
    void onSelectTarget(int index);
    void onCancel();
private:
    Ui::flightmasterSelecter *ui;
    QComboBox * fm;
    QComboBox * tar;
    const bool isFlightPathSelecter;
    bool isSelected_;
    bool isTargetSelected_;
    flightmaster_location flightmaster;
    flightmaster_location target;
};

#endif // FLIGHTMASTERSELECTER_H
