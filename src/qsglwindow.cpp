#include "qsglwindow.h"
#include "ui_qsglwindow.h"
#include "ui_qsglaboutdialog.h"

#include <QUrl>
#include <QFile>
#include <QDir>
#include <QPixmap>
#include <QPainter>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

QSGLWindow::QSGLWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QSGLWindow)
{
    ui->setupUi(this);
    ui->documentationView->setUrl(QUrl("file:/home/david/workspace/SGLDocumentation/index.html"));
}

QSGLWindow::~QSGLWindow()
{
    delete ui;
}

void QSGLWindow::on_actionQuit_triggered()
{
    close();
}

void QSGLWindow::on_actionOpen_triggered()
{
    QString newFileName;

    newFileName = QFileDialog::getOpenFileName(0,"Open Script", QDir::homePath(), "SGL Scripts (*.sgl);;All Files (*.*)");

    if(newFileName.isEmpty())
        return;

    currentFileName = newFileName;

    QFile file(currentFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox(QMessageBox::Warning, tr("Error Opening File"),QString(tr("Cannot open file %1")).arg(currentFileName));
        return;
    }

    QTextStream fileStream(&file);

    ui->commandsTextEdit->setPlainText(fileStream.readAll());

    return;
}

void QSGLWindow::on_actionSave_triggered()
{
    QString selectedFilter;
    QString newFileName;

    if(currentFileName.isEmpty())
    {
        newFileName = QFileDialog::getSaveFileName(0,"Save Script Contents", QDir::homePath(), "SGL Scripts (*.sgl);;All Files (*.*)", &selectedFilter);

        if(newFileName.isEmpty())
            return;

        if((selectedFilter == "SGL Scripts (*.sgl)") && (!newFileName.endsWith(".sgl")))
            newFileName += ".sgl";

        currentFileName = newFileName;
    }

    QFile file(currentFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QMessageBox(QMessageBox::Warning, tr("Error Saving File"),QString(tr("Cannot save file %1")).arg(currentFileName));
        return;
    }

    QTextStream fileStream(&file);
    fileStream << ui->commandsTextEdit->toPlainText();

    return;
}

void QSGLWindow::on_actionSave_As_triggered()
{
    QString selectedFilter;
    QString newFileName;

    newFileName = QFileDialog::getSaveFileName(0,"Save Script Contents", QDir::homePath(), "SGL Scripts (*.sgl);;All Files (*.*)", &selectedFilter);

    if(newFileName.isEmpty())
        return;

    if((selectedFilter == "SGL Scripts (*.sgl)") && (!newFileName.endsWith(".sgl")))
        newFileName += ".sgl";

    currentFileName = newFileName;

    on_actionSave_triggered();

    return;
}

void QSGLWindow::on_actionNew_triggered()
{
    currentFileName.clear();
    on_clearButton_clicked();

    return;
}

void QSGLWindow::on_drawButton_clicked()
{
    ui->imageWindow->setScriptToEval(ui->commandsTextEdit->toPlainText());
    ui->imageWindow->updateGL();
    ui->outputTextEdit->setPlainText(ui->imageWindow->getOuputString());

    return;
}

void QSGLWindow::on_clearButton_clicked()
{
    ui->commandsTextEdit->clear();
}

void QSGLWindow::on_actionExport_triggered()
{
    QString selectedFilter;
    QString exportFileName = QFileDialog::getSaveFileName(0,"Export Image", QDir::homePath(), "PNG Images (*.png)", &selectedFilter);

    if(exportFileName.isEmpty())
        return;

    if(!exportFileName.endsWith(".png"))
        exportFileName += ".png";

    QPixmap image = ui->imageWindow->renderPixmap();

    if(!image.save(exportFileName))
        QMessageBox(QMessageBox::Warning, tr("Error Exporting Image"),QString(tr("Cannot open file %1")).arg(exportFileName));

    return;
}

void QSGLWindow::on_actionAbout_triggered()
{
    Ui::QSGLAboutDialog * aboutDialog = new Ui::QSGLAboutDialog;
    QDialog * wtf = new QDialog;
    aboutDialog->setupUi(wtf);
    wtf->show();

    return;
}
