#ifndef CHARTSVIEWER_H
#define CHARTSVIEWER_H

#include <QDialog>

class QCustomPlot;

namespace Ui {
class ChartsViewer;
}

struct ChartsData
{
    double ek, eph, eps;
};

class ChartsViewer : public QDialog
{
    Q_OBJECT

public:
    explicit ChartsViewer(QWidget *parent = 0);
    ~ChartsViewer();

    void updateChart(QVector<ChartsData> *chartsData);

private:
    Ui::ChartsViewer *ui;

    void setupChart();
};

#endif // CHARTSVIEWER_H
