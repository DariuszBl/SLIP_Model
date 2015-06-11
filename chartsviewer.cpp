#include "chartsviewer.h"
#include "ui_chartsviewer.h"

ChartsViewer::ChartsViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartsViewer)
{
    ui->setupUi(this);
    this->setupChart();
}

ChartsViewer::~ChartsViewer()
{
    delete ui;
}

void ChartsViewer::setupChart()
{
    // create graph and assign data to it:
    ui->chart->addGraph();
//    ui->chart->graph(0)->setData(x, y);
    ui->chart->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph

    ui->chart->addGraph();
//    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    ui->chart->graph(1)->setPen(QPen(Qt::red)); // line color blue for first graph

    ui->chart->addGraph();
//    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    ui->chart->graph(2)->setPen(QPen(Qt::green)); // line color blue for first graph

    ui->chart->addGraph();
//    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    ui->chart->graph(3)->setPen(QPen(Qt::black)); // line color blue for first graph

    // give the axes some labels:
    ui->chart->xAxis->setLabel("x");
    ui->chart->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->chart->xAxis->setRange(0, 1000);
    ui->chart->yAxis->setRange(0, 30);
    ui->chart->replot();
}

void ChartsViewer::updateChart(QVector<ChartsData> *chartsDataVector)
{
    QVector<double> yVector1;
    QVector<double> yVector2;
    QVector<double> yVector3;
    QVector<double> yVector4;
    QVector<double> xVector;
    static double counter = 0;

    for(const ChartsData element : *chartsDataVector)
    {
        yVector1.append(element.ek);
        yVector2.append(element.eph);
        yVector3.append(element.eps);
        yVector4.append(element.ek + element.eph + element.eps);
        xVector.append(counter++);
    }

    ui->chart->graph(0)->setData(xVector, yVector1);
    ui->chart->graph(1)->setData(xVector, yVector2);
    ui->chart->graph(2)->setData(xVector, yVector3);
    ui->chart->graph(3)->setData(xVector, yVector4);
//    customPlot->graph(1)->setData(x, y1);

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
//    ui->chart->graph(0)->rescaleAxes();
    ui->chart->replot();

    counter = 0;
}
