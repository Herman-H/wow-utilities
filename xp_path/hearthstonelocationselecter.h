#ifndef HEARTHSTONELOCATIONSELECTER_H
#define HEARTHSTONELOCATIONSELECTER_H

#include <QDialog>
#include "action.h"

namespace Ui {
class hearthstonelocationselecter;
}

class hearthstonelocationselecter : public QDialog
{
    Q_OBJECT

public:
    explicit hearthstonelocationselecter(QWidget *parent = 0);
    ~hearthstonelocationselecter();

    hearthstone_location selectedLocation() const
    {
        return hs;
    }

    bool isSelected() const
    {
        return isSelected_;
    }

private slots:
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void onClose();
private:
    Ui::hearthstonelocationselecter *ui;
    bool isSelected_;
    hearthstone_location hs;
};

#endif // HEARTHSTONELOCATIONSELECTER_H
