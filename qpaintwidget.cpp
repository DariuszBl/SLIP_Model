#include "qpaintwidget.h"
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>

QPaintWidget::QPaintWidget(QWidget *parent) :
    QWidget(parent)
{
    elapsed = 0;
    setAutoFillBackground(false);
}

void QPaintWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    repaint();
}

void QPaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    hopper.paint(&painter, event, 1);
    painter.end();
}
