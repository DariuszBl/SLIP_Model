#ifndef QPAINTWIDGET_H
#define QPAINTWIDGET_H

#include <QWidget>
#include "hopper.h"

class QPaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPaintWidget(QWidget *parent = 0);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int elapsed;
    Hopper hopper;
};

#endif // QPAINTWIDGET_H
