#ifndef QSGLVISUALIZATIONWINDOW_H
#define QSGLVISUALIZATIONWINDOW_H

#include <QGLWidget>
#include <QScriptEngine>

class QSGLVisualizationWindow : public QGLWidget
{
    Q_OBJECT

public:
    QSGLVisualizationWindow();
    QSGLVisualizationWindow(QWidget * &);
    ~QSGLVisualizationWindow();

    QSize sizeHint() const;
    void setScriptToEval(QString);
    QString getScriptToEval() const;
    void setOutputString(QString);
    QString getOuputString() const;

protected:
    void paintGL();
    void resizeGL(int,int);
    void initializeGL();

private:
    QString scriptToEval;
    QString outputString;
    QScriptEngine qsengine;
    void initializeScripting();

public slots:
    void line(float,float,float,float);
    void rectangle(float,float,float,float);
    void rectangleFilled(float,float,float,float);
    void polygon(QStringList);
    void polygonFilled(QStringList);
    void circle(float, float, float);
    void circleFilled(float, float, float);

    void translate(float, float);
    void scale(float, float);
    void rotate(float);

    void color(float, float, float);

    void window(float, float, float, float);
    void viewport(int, int, int, int);

    void push();
    void pop();

    void print(QString);
    void printTransformationMatrix();
    void printProjectionMatrix();
    void printWindowViewportMatrix();
};

#endif // QSGLVISUALIZATIONWINDOW_H
