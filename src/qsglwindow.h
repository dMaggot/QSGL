#ifndef QSGLWINDOW_H
#define QSGLWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class QSGLWindow;
}

class QSGLWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSGLWindow(QWidget *parent = 0);
    ~QSGLWindow();

private:
    Ui::QSGLWindow *ui;
    QString currentFileName;

private slots:
    void on_actionAbout_triggered();
    void on_actionExport_triggered();
    void on_actionNew_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_drawButton_clicked();
    void on_clearButton_clicked();
};

#endif // QSGLWINDOW_H
