#ifndef XPBREAKPOINTSELECTER_H
#define XPBREAKPOINTSELECTER_H

#include <QDialog>

namespace Ui {
class XPBreakpointSelecter;
}

class XPBreakpointSelecter : public QDialog
{
    Q_OBJECT

public:
    explicit XPBreakpointSelecter(QWidget *parent = 0);
    ~XPBreakpointSelecter();

    int GetLevel() const;
    int GetXP() const;
    bool isSelected() const { return ok; }

private slots:

    void setValues();
    void onCancelButton();

private:
    bool ok;
    int xp;
    int level;
    Ui::XPBreakpointSelecter *ui;
};

#endif // XPBREAKPOINTSELECTER_H
