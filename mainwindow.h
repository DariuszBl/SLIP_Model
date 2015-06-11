#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "simulation.h"
#include "backgroundsim.h"
#include "chartsviewer.h"

class QTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /* Starts doWork function in steering thread */
    void startBackgroundSimThread();

private:
    ChartsData getCurrentModelData();
    void sendToBackgroundSim(double targetX, double targetZ);

    Ui::MainWindow *ui;
    ChartsViewer *chartsViewer;
    QTimer *timer;
    Simulation simulation;
    QVector<ChartsData> chartsDataVector;
    bool backgroundDataRequested;

    // to be deleted
    bool lazyFlag;

    /* Thread used for searching optimal angle values for steering. Activates after stance phase ends. Ends
      after opimal angle values are found. */
    BackgroundSim *backgroundSim;
    QThread *thread;

private slots:
    void on_Charts_clicked();
    void processing();

    void receiveAngles(double, double);
    void targetRequested();

signals:
    void sendBackgroundSimData(BackgroundSimData backgroundSimDataTemp);
};

#endif // MAINWINDOW_H
