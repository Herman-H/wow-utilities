#ifndef XP_PLOT_H
#define XP_PLOT_H

#include <QWidget>
#include <QtCharts>
#include "xp_vector.h"
#include "qcustomplot/qcustomplot.h"

using namespace QtCharts;

namespace Ui {
class XP_Plot;
}

class XP_Plot : public QWidget
{
    Q_OBJECT
public:
    explicit XP_Plot(xp_vector *v, QWidget *parent = 0);
    ~XP_Plot();

private:
    Ui::XP_Plot *ui;
    xp_vector * vec;

    QChart * chart;

    void initialize();

    QCustomPlot * plot() const;

    std::vector<std::pair<int,double>> generateZoneBreakpoints();

};

#endif // XP_PLOT_H
