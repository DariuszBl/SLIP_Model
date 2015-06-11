#ifndef BACKGROUNDSIM_H
#define BACKGROUNDSIM_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include "control.h"

class BackgroundSim : public QObject
{
    Q_OBJECT

public:
    explicit BackgroundSim(QObject *parent = 0);
    /* It is thread safe as it uses #mutex to protect access to #_working variable. */
    void requestWork();
    /* Requests the process to abort
     * It is thread safe as it uses #mutex to protect access to #_abort variable. */
    void abortFun();
    void findAngles();
    double simToTarget(double alfa, double beta);

private:
    /* Process is aborted when @em true */
    bool abort;
    /* true when Worker is doing work */
    bool working;
    /* Protects access to #_abort*/
    QMutex mutex;
    /* Stores data needed for finding optimal steering*/
    BackgroundSimData simData;

    bool anglesFound;
    bool targetReached;

    Control control;
    QWaitCondition condition;

signals:
    /* signal is emitted when the Worker request to Work */
    void workRequested();
    /* This signal is emitted when counted value is changed (every sec) */
    void valueChanged(const QString &value);
    /* This signal is emitted when process is finished (either by counting 60 sec or being aborted) */
    void finished();

    void dataWasReceived();
    void sendAnglesToMain(double alfa, double beta);
    void requestNewTarget();

public slots:
    /* Counts 60 sec in this example.
     * Counting is interrupted if #_aborted is set to true.  */
    void doWork();

    /* Receive data. */
    void receiveData(BackgroundSimData backgroundSimData);
//    void startProcessing();

};

#endif // BACKGROUNDSIM_H
