#include "diagramwindow.h"
#include "ui_diagramwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QImageWriter>
#include <QMessageBox>
#include <QSvgGenerator>

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

void DiagramWindow::saveAsImage()
{
    QString dir;
    QString filter;
	QStringList formats;
	Q_FOREACH(QByteArray f, QImageWriter::supportedImageFormats()){
		formats.append("*." + f);
	}
	formats.append("*.svg");
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save as Image"), dir, tr("Image Formats (%1)").arg(formats.join(", ")), &filter);
    if (!fileName.isEmpty()){
		QFileInfo fileInfo(fileName);
		if (fileInfo.suffix() == "svg"){
			QSvgGenerator generator;
			generator.setFileName(fileName);
			QPainter p(&generator);
			diagram()->scene()->render(&p);
			p.end();
		} else {
			QImage image(diagram()->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
			QPainter p(&image);
			diagram()->scene()->render(&p);
			p.end();
			image.save(fileName);
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

