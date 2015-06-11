#include "glwidget.h"
#include <GL/GLU.h>
#include "hopper.h"
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    zoomFactor(1.0),
    hopperX(0.0),
    hopperY(0.0),
    hopperZ(0.0),
    hopperAlfa(0.0),
    hopperBeta(0.0),
    hopperLength(0.0),
    rotValue(0.0,0.0)
{
    setMouseTracking(true);
}

void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (float)width/(float)height, 1, 500);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void GLWidget::updateHopperData(HopperData newHopperData)
{
    this->hopperX = newHopperData.x;
    this->hopperY = newHopperData.y;
    this->hopperZ = newHopperData.z;
    this->hopperAlfa = newHopperData.alfa;
    this->hopperBeta = newHopperData.beta;
    this->hopperLength = newHopperData.length;
}

void GLWidget::paintGL()
{
    //delete color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt( 0, 20, 70, 0, 0, 0, 0, 1, 0);

    glRotatef(this->rotValue.y(), 1.0f, 0.0f, 0.0f);
    glRotatef(this->rotValue.x(), 0.0f, 1.0, 0.0f);

    glColor3f(0.5,0.5,0.5);

    paintPlane(0.0, 0.0, 0.0, 1000.0);

    // Move hopper to position acquired from model
    glScalef(10.0, 10.0, 10.0);

    // Move hopper to position acquired from model
    glTranslatef(this->hopperX, this->hopperY, this->hopperZ);

    // Rotate hopper by beta
    glRotatef(-this->hopperBeta, 0.0f, 1.0f, 0.0f);

    // Store first value of length (max)
    static bool flag = true;
    static double lengthMax = 0.0;
    if (flag && hopperLength != 0.0)
    {
        lengthMax = hopperLength;
        flag = false;
    }

    glRotatef(-this->hopperAlfa, 0.0f, 0.0f, 1.0f);

    // Paint hopper
    paintHopper(lengthMax/10.0, this->hopperLength, lengthMax/10.0);
}

void GLWidget::paintHopper(double a, double b, double c)
{
    glScalef(a,b,c);
    glTranslatef(0.0,0.15,0.0);

    paintCuboid(0.5,0.3,0.5);

    glTranslatef(0.0,0.35,0.0);
    paintCuboid(1.0,0.4,1.0);

    glTranslatef(0.0,0.35,0.0);
    paintCuboid(0.5,0.3,0.5);
}

void GLWidget::paintCuboid(double a, double b, double c)
{
    const double coords[6][4][3] = {
        { { +(a/2.0), -(b/2.0), -(c/2.0) }, { -(a/2.0), -(b/2.0), -(c/2.0) }, { -(a/2.0), +(b/2.0), -(c/2.0) }, { +(a/2.0), +(b/2.0), -(c/2.0) } },
        { { +(a/2.0), +(b/2.0), -(c/2.0) }, { -(a/2.0), +(b/2.0), -(c/2.0) }, { -(a/2.0), +(b/2.0), +(c/2.0) }, { +(a/2.0), +(b/2.0), +(c/2.0) } },
        { { +(a/2.0), -(b/2.0), +(c/2.0) }, { +(a/2.0), -(b/2.0), -(c/2.0) }, { +(a/2.0), +(b/2.0), -(c/2.0) }, { +(a/2.0), +(b/2.0), +(c/2.0) } },
        { { -(a/2.0), -(b/2.0), -(c/2.0) }, { -(a/2.0), -(b/2.0), +(c/2.0) }, { -(a/2.0), +(b/2.0), +(c/2.0) }, { -(a/2.0), +(b/2.0), -(c/2.0) } },
        { { +(a/2.0), -(b/2.0), +(c/2.0) }, { -(a/2.0), -(b/2.0), +(c/2.0) }, { -(a/2.0), -(b/2.0), -(c/2.0) }, { +(a/2.0), -(b/2.0), -(c/2.0) } },
        { { -(a/2.0), -(b/2.0), +(c/2.0) }, { +(a/2.0), -(b/2.0), +(c/2.0) }, { +(a/2.0), +(b/2.0), +(c/2.0) }, { -(a/2.0), +(b/2.0), +(c/2.0) } }
    };

    for (int i = 0; i < 6; ++i) {
        glColor3f((i+1)&4,(i+1)&2,(i+1)&1);

        glBegin(GL_QUADS);
        for (int j = 0; j < 4; ++j) {
            glVertex3f( coords[i][j][0], coords[i][j][1], coords[i][j][2]);
        }
        glEnd();
    }
}

void GLWidget::paintPlane(double posX, double posY, double posZ, double size)
{
    glBegin(GL_QUADS);

    glVertex3f( posX + (size/2.0), posY, posZ + (size/2.0));
    glVertex3f( posX + (size/2.0), posY, posZ - (size/2.0));
    glVertex3f( posX - (size/2.0), posY, posZ - (size/2.0));
    glVertex3f( posX - (size/2.0), posY, posZ + (size/2.0));

    glEnd();
}

void GLWidget::mouseMoveEvent(QMouseEvent * event)
{
    static boolean mouseLook = false;

    static QPoint tmpMousePos;
    static QPoint tmpRotValue;

    if (event->buttons() == Qt::MidButton)
    {
        // was it already down? If not, store the current coords
        if (!mouseLook)
        {
            tmpMousePos = event->pos();
            tmpRotValue = this->rotValue;
            mouseLook = true;
        }

        // update the rotation values depending on the relative mouse position
        this->rotValue.setX( tmpRotValue.x() + (tmpMousePos.x() - event->pos().x()) * 0.2 );
        this->rotValue.setY( tmpRotValue.y() + (tmpMousePos.y() - event->pos().y()) * -0.2 );
    }
    else
    {
        // turn off mouse look
        mouseLook = false;
    }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    this->zoomFactor -= (double)(event->delta()) / 10000.0;

    setProjectionMatrix(this->width(), this->height());
}

void GLWidget::setProjectionMatrix (int width, int height)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective (50.0*zoomFactor, (float)width/(float)height, 1, 500);
}

void GLWidget::setHopperLength(double newHopperLength)
{
    this->hopperLength = newHopperLength;
}
