#include "qsglvisualizationwindow.h"

#include <cmath>

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QVector>
#include <QStringList>
#include <QDebug>

QSGLVisualizationWindow::QSGLVisualizationWindow()
{
    initializeScripting();
}

QSGLVisualizationWindow::QSGLVisualizationWindow(QWidget * & w) :
        QGLWidget(w)
{
    initializeScripting();
}

QSGLVisualizationWindow::~QSGLVisualizationWindow()
{
}

QSize QSGLVisualizationWindow::sizeHint() const
{
    return QSize(800,600);
}

void QSGLVisualizationWindow::setScriptToEval(QString newscript)
{
    scriptToEval = newscript;

    return;
}
QString QSGLVisualizationWindow::getScriptToEval() const
{
    return scriptToEval;
}

void QSGLVisualizationWindow::setOutputString(QString newoutput)
{
    outputString = newoutput;

    return;
}

QString QSGLVisualizationWindow::getOuputString() const
{
    return outputString;
}

void QSGLVisualizationWindow::initializeScripting()
{
    QScriptValue objval = qsengine.newQObject(this);
    qsengine.globalObject().setProperty("_SGL", objval);
    qsengine.globalObject().setProperty(tr("line"), qsengine.evaluate("_SGL.line"));
    qsengine.globalObject().setProperty(tr("rectangle"), qsengine.evaluate("_SGL.rectangle"));
    qsengine.globalObject().setProperty(tr("rectangleFilled"), qsengine.evaluate("_SGL.rectangleFilled"));
    qsengine.globalObject().setProperty(tr("polygon"), qsengine.evaluate("_SGL.polygon"));
    qsengine.globalObject().setProperty(tr("filledPolygon"), qsengine.evaluate("_SGL.polygonFilled"));
    qsengine.globalObject().setProperty(tr("circle"), qsengine.evaluate("_SGL.circle"));
    qsengine.globalObject().setProperty(tr("circleFilled"), qsengine.evaluate("_SGL.circleFilled"));

    qsengine.globalObject().setProperty(tr("trasladar"), qsengine.evaluate("_SGL.translate"));
    qsengine.globalObject().setProperty(tr("scale"), qsengine.evaluate("_SGL.scale"));
    qsengine.globalObject().setProperty(tr("rotate"), qsengine.evaluate("_SGL.rotate"));

    qsengine.globalObject().setProperty(tr("color"), qsengine.evaluate("_SGL.color"));

    qsengine.globalObject().setProperty(tr("window"), qsengine.evaluate("_SGL.window"));
    qsengine.globalObject().setProperty(tr("viewport"), qsengine.evaluate("_SGL.viewport"));

    qsengine.globalObject().setProperty(tr("print"), qsengine.evaluate("_SGL.print"));
    qsengine.globalObject().setProperty(tr("printTransformationMatrix"), qsengine.evaluate("_SGL.printTransformationMatrix"));

    return;
}

void QSGLVisualizationWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    outputString.clear();

    QScriptValue evaluationResult = qsengine.evaluate(scriptToEval);
    if (qsengine.hasUncaughtException())
        outputString = evaluationResult.toString();

    return;
}

void QSGLVisualizationWindow::resizeGL(int w, int h)
{
    float aspectratio = (float)h/(float)w;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5.0, 5.0, -5.0*aspectratio, 5.0*aspectratio);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return;
}

void QSGLVisualizationWindow::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    return;
}

void QSGLVisualizationWindow::line(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();

    return;
}

void QSGLVisualizationWindow::rectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_STRIP);
    glVertex2f(x1,y1);
    glVertex2f(x1,y2);
    glVertex2f(x2,y2);
    glVertex2f(x2,y1);
    glVertex2f(x1,y1);
    glEnd();

    return;
}

void QSGLVisualizationWindow::rectangleFilled(float x1, float y1, float x2, float y2)
{
    glBegin(GL_POLYGON);
    glVertex2f(x1,y1);
    glVertex2f(x1,y2);
    glVertex2f(x2,y2);
    glVertex2f(x2,y1);
    glVertex2f(x1,y1);
    glEnd();

    return;
}

void QSGLVisualizationWindow::polygon(QStringList vertex)
{
    QVector<float> points;

    if(vertex.size()%2)
    {
        qsengine.abortEvaluation();
        return;
    }

    for(int i=0; i<vertex.size(); i++)
    {
        bool ok = false;
        points.append(vertex[i].toFloat(&ok));
        if(!ok)
        {
            qsengine.abortEvaluation();
            return;
        }
    }

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<points.size(); i+=2)
        glVertex2f(points[i],points[i+1]);
    glVertex2f(points[0],points[1]);
    glEnd();

    return;
}

void QSGLVisualizationWindow::polygonFilled(QStringList vertex)
{
    QVector<float> points;

    if(vertex.size()%2)
    {
        qsengine.abortEvaluation();
        return;
    }

    for(int i=0; i<vertex.size(); i++)
    {
        bool ok = false;
        points.append(vertex[i].toFloat(&ok));
        if(!ok)
        {
            qsengine.abortEvaluation();
            return;
        }
    }

    glBegin(GL_POLYGON);
    for(int i=0; i<points.size(); i+=2)
        glVertex2f(points[i],points[i+1]);
    glVertex2f(points[0],points[1]);
    glEnd();

    return;
}

void QSGLVisualizationWindow::circle(float cx, float cy, float r)
{
    int precision = 1000;
    QStringList points;

    for(int i=0; i<precision; i++)
    {
        points.append(QString("%1").arg(cx + r*cos(2*i*M_PI/precision)));
        points.append(QString("%1").arg(cy + r*sin(2*i*M_PI/precision)));
    }

    polygon(points);

    return;
}

void QSGLVisualizationWindow::circleFilled(float cx, float cy, float r)
{
    int precision = 1000;
    QStringList points;

    for(int i=0; i<precision; i++)
    {
        points.append(QString("%1").arg(cx + r*cos(2*i*M_PI/precision)));
        points.append(QString("%1").arg(cy + r*sin(2*i*M_PI/precision)));
    }

    polygonFilled(points);

    return;
}

void QSGLVisualizationWindow::translate(float x, float y)
{
    glTranslatef(x, y, 0.0f);

    return;
}

void QSGLVisualizationWindow::scale(float scaleX, float scaleY)
{
    glScalef(scaleX,scaleY,1.0f);

    return;
}

void QSGLVisualizationWindow::rotate(float angle)
{
    glRotatef(angle, 0, 0, 1);

    return;
}

void QSGLVisualizationWindow::color(float r, float g, float b)
{
    glColor3f(r,g,b);

    return;
}

void QSGLVisualizationWindow::window(float left, float right, float bottom, float top)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);

    glMatrixMode(GL_MODELVIEW);

    return;
}

void QSGLVisualizationWindow::viewport(int x1, int y1, int width, int height)
{
    glViewport(x1,y1,width,height);

    return;
}

void QSGLVisualizationWindow::push()
{
    glPushMatrix();

    return;
}

void QSGLVisualizationWindow::pop()
{
    glPopMatrix();

    return;
}

void QSGLVisualizationWindow::print(QString toPrint)
{
    outputString.append(toPrint);

    return;
}

void QSGLVisualizationWindow::printTransformationMatrix()
{
    GLdouble transformationMatrix[16];
    QString output("Transformation Matrix\n|%1\t%5\t%9\t%13|\n|%2\t%6\t%10\t%14|\n|%3\t%7\t%11\t%15|\n|%4\t%8\t%12\t%16|\n-------\n");

    glGetDoublev(GL_MODELVIEW_MATRIX, transformationMatrix);

    for(int i=0; i<16; i++)
        output = output.arg(transformationMatrix[i]);

    outputString.append(output);

    return;
}

void QSGLVisualizationWindow::printProjectionMatrix()
{
    GLdouble projectionMatrix[16];
    QString output("Transformation Matrix\n|%1\t%5\t%9\t%13|\n|%2\t%6\t%10\t%14|\n|%3\t%7\t%11\t%15|\n|%4\t%8\t%12\t%16|\n-------\n");

    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

    for(int i=0; i<16; i++)
        output = output.arg(projectionMatrix[i]);

    outputString.append(output);

    return;
}

void QSGLVisualizationWindow::printWindowViewportMatrix()
{
    return;
}
