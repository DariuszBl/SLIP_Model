#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

struct HopperData
{
    double x, y, z, alfa, beta, length;
};

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);

    void updateHopperData(HopperData newHopperData);
    void setHopperLength(double newHopperLength);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mouseMoveEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent *event);

private:
    // variables used to define hopper view
    double hopperX, hopperY, hopperZ;
    double hopperAlfa, hopperBeta;
    double hopperLength;

    // variables used for camera
    double zoomFactor;
    QPoint rotValue;

    void paintHopper(double a, double b, double c);
    void paintCuboid(double a, double b, double c);
    void paintPlane(double posX, double posY, double posZ, double size);
    void setProjectionMatrix (int width, int height);
};

#endif // GLWIDGET_H
