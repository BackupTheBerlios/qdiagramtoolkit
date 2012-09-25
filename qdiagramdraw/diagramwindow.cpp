#include "diagramwindow.h"
#include "ui_diagramwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <qdiagramwriter.h>

DiagramWindow::DiagramWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagramWindow)
{
    cDiagram = 0;
    ui->setupUi(this);
}

DiagramWindow::~DiagramWindow()
{
    delete ui;
}

QDiagram* DiagramWindow::diagram() const
{
    return cDiagram;
}

QDiagramView *DiagramWindow::diagramView() const
{
    return ui->diagramView;
}

void DiagramWindow::print(QPrinter *printer)
{
    QPainter painter(printer);
    painter.setRenderHint(QPainter::Antialiasing);
    ui->diagramView->render(&painter);
}

void DiagramWindow::undo()
{
    cDiagram->undoStack()->undo();
}

void DiagramWindow::redo()
{
    cDiagram->undoStack()->redo();
}

void DiagramWindow::diagramContentsChanged()
{
    if (cDiagram->isModified()){
        setWindowTitle(QString("%1*").arg(cDiagram->title()));
    } else {
        setWindowTitle(QString("%1").arg(cDiagram->title()));
    }
}

void DiagramWindow::save()
{
    QString dir;
    QString filter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), dir, tr("Formats (*.jsn *.xml)"), &filter);
    if (!fileName.isEmpty()){
        QDiagramWriter writer(fileName);
        if (!writer.write(cDiagram)){
            if (writer.error() == QDiagramWriter::DeviceError){
                QMessageBox::critical(this, tr("Device Error"), writer.device()->errorString());
            } else {
                QMessageBox::critical(this, tr("I/O Error"), writer.errorString());
            }
        }
    }
}

void DiagramWindow::setDiagram(QDiagram* diagram)
{
    delete cDiagram;
    cDiagram = diagram;
    ui->diagramView->setDiagram(cDiagram);
    connect(cDiagram, SIGNAL(contentsChanged()), this, SLOT(diagramContentsChanged()));
    setWindowTitle(cDiagram->title());
}

