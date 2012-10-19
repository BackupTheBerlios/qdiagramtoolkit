#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QSignalMapper>

#include <qabstractdiagramgraphicsitem.h>
#include <qabstractdiagramshapeconnector.h>
#include "qdiagrampluginloader.h"


#include "diagramwindow.h"
#include "qdiagramreader.h"
#include "qdiagramview.h"

#include "qlogiccircuitsimulator.h"

#define CALL_ACTION(___a___) if (ui->mdiArea->activeSubWindow()){ \
    DiagramWindow* mWindow = qobject_cast<DiagramWindow*>(ui->mdiArea->activeSubWindow()->widget()); \
    if (mWindow){ \
        mWindow->___a___; \
    } \
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)), this, SLOT(clipboardChanged(QClipboard::Mode)));

    qDebug() << QDiagramPluginLoader::plugins();
	    qDebug() << QDiagramPluginLoader::load("Logic Circuit")->diagrams();

    qDebug() << "QDiagramIO" << QDiagramReader::supportedDiagramFormats();

    updateWindowMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QDiagram* MainWindow::activeDiagram() const
{
    if (ui->mdiArea->activeSubWindow() == 0){
        return 0;
    }
    DiagramWindow* w = qobject_cast<DiagramWindow*>(ui->mdiArea->activeSubWindow()->widget());
    if (w){
        return w->diagram();
    }
    return 0;
}

DiagramWindow *MainWindow::activeDiagramWindow() const
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow()){
         return qobject_cast<DiagramWindow *>(activeSubWindow->widget());
    }
     return 0;
 }

void MainWindow::clipboardChanged(QClipboard::Mode mode)
{
    if (ui->mdiArea->activeSubWindow()){
        DiagramWindow* w = qobject_cast<DiagramWindow*>(ui->mdiArea->activeSubWindow()->widget());
        if (w){
            ui->pasteAction->setEnabled(w->diagramView()->canPaste());
        }
    } else {
        ui->pasteAction->setEnabled(false);
    }
}

void MainWindow::copyActionTriggered()
{
    if (ui->mdiArea->activeSubWindow()){
        DiagramWindow* w = qobject_cast<DiagramWindow*>(ui->mdiArea->activeSubWindow()->widget());
        if (w){
            w->diagramView()->copy();
        }
    }
}

void MainWindow::cutActionTriggered()
{
    if (ui->mdiArea->activeSubWindow()){
        DiagramWindow* w = qobject_cast<DiagramWindow*>(ui->mdiArea->activeSubWindow()->widget());
        if (w){
            w->diagramView()->cut();
        }
    }
    //    QDiagram* diagram = activeDiagram();
//    if (diagram){
//        if (!diagram->selectedItems().isEmpty()){
//            QString uuid = diagram->selectedItems().first()->uuid();
//            diagram->removeItem(uuid);
//        }
//    }
}

void MainWindow::diagramViewContextMenuRequested(const QPoint & point, const QPointF & scenePos)
{
    QDiagramView* view = qobject_cast<QDiagramView*>(sender());
    if (view){
		QList<QAction*> actions;
		QAbstractDiagramShape* shape = view->shapeAt(point);
        QList<QGraphicsItem*> items = view->graphicsView()->items(point);
        QMenu menu;
		if (shape){
			actions = shape->createActions(&menu);
		}
        if (items.isEmpty()){
            menu.addAction(ui->pasteAction);
        } else if (items.size() == 1){
            menu.addAction(ui->cutAction);
            menu.addAction(ui->copyAction);
            menu.addAction(ui->pasteAction);
			if (!actions.isEmpty()){
				menu.addSeparator();
				menu.addActions(actions);
			}
        } else {

        }
        menu.addSeparator();
        menu.addAction(ui->refreshAction);

        ui->pasteAction->setData(scenePos);
        QAction* a = menu.exec(view->graphicsView()->viewport()->mapToGlobal(point));
		if (a && shape){
			shape->triggerAction(a->objectName(), a->data());
		}
    }
}

void MainWindow::diagramItemAdded(QAbstractDiagramGraphicsItem *item)
{
//    QMessageBox::information(this, tr("Item added"), QString(tr("UUID %1")).arg(item->uuid()), QMessageBox::Ok);
}

void MainWindow::exitActionTriggered()
{
    ui->mdiArea->closeAllSubWindows();
    qApp->exit();
}

void  MainWindow::newActionTriggered()
{
	QMap<QString,QStringList> types;
	Q_FOREACH(QString n, QDiagramPluginLoader::plugins()){
		QAbstractDiagramPlugin* p = QDiagramPluginLoader::plugin(n);
		types[n] = p->diagrams();
	}

	bool ok;
	QString pn = QInputDialog::getItem(this, tr("Select Plugin"), tr("Name"), types.keys(), 0, false, &ok);
	if (!ok){
		return;
	}
	QString dn = QInputDialog::getItem(this, tr("Select Diagram"), tr("Name"), types.value(pn), 0, false, &ok);
	if (!ok){
		return;
	}
	QAbstractDiagramPlugin* p = QDiagramPluginLoader::plugin(pn);
	QDiagram* d = p->diagram(dn);
	connect(d, SIGNAL(itemRestored(QAbstractDiagramGraphicsItem*)), this, SLOT(diagramItemAdded(QAbstractDiagramGraphicsItem*)));

	DiagramWindow* w = new DiagramWindow();
	w->setDiagram(d);
	ui->mdiArea->addSubWindow(w);
	connect(w->diagramView(), SIGNAL(graphicsViewContextMenuRequested(QPoint,QPointF)), this, SLOT(diagramViewContextMenuRequested(QPoint,QPointF)));
	w->show();
	if (ui->mdiArea->subWindowList().size() == 1){
		w->showMaximized();
	}
}

void MainWindow::openActionTriggered()
{
    QString dir;
    QString filter;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), dir, tr("Formats (*.jsn *.xml)"), &filter);
    if (!fileName.isEmpty()){
        QDiagramReader reader(fileName, "jsn");
        QAbstractDiagram* d = reader.read();
        if (d){
            DiagramWindow* w = new DiagramWindow();
            w->setDiagram((QDiagram*)d);
            connect(w->diagramView(), SIGNAL(graphicsViewContextMenuRequested(QPoint,QPointF)), this, SLOT(diagramViewContextMenuRequested(QPoint,QPointF)));

            ui->mdiArea->addSubWindow(w);
            w->show();
        }
    }
}

void MainWindow::pasteActionTriggered()
{
    if (ui->mdiArea->activeSubWindow()){
        DiagramWindow* w = qobject_cast<DiagramWindow*>(ui->mdiArea->activeSubWindow()->widget());
        if (w){
            w->diagramView()->paste();
        }
    }
}

void MainWindow::printPreviewActionTriggered()
{
    if (activeDiagram() == 0){
        return;
    }
    QPrinter p(QPrinter::HighResolution);
    QPrintPreviewDialog* d = new QPrintPreviewDialog(&p, this);

    connect(d, SIGNAL(paintRequested(QPrinter*)), activeDiagram(), SLOT(print(QPrinter*)));
    d->exec();
    delete d;
}

void MainWindow::redoActionTriggered()
{
    CALL_ACTION(redo())
}

void MainWindow::saveActionTriggered()
{
    CALL_ACTION(save())
}

void MainWindow::saveAsImageActionTriggered()
{
    CALL_ACTION(saveAsImage())
}

void MainWindow::selectionChanged()
{
    QDiagram* diagram = activeDiagram();

    ui->copyAction->setEnabled(false);
    ui->cutAction->setEnabled(false);
    if (diagram){
        ui->copyAction->setEnabled(!diagram->selectedItems().isEmpty());
        ui->cutAction->setEnabled(!diagram->selectedItems().isEmpty());

        if (diagram->selectedItems().isEmpty()){
            ui->propertiesView->clear();
        } else {
            ui->propertiesView->showProperties(diagram->selectedItems());
        }
    }
}

void MainWindow::simulatorActionTriggered()
{
    QLogicCircuitSimulator* sim = new QLogicCircuitSimulator(this);
    sim->setDiagram(activeDiagram());
    sim->exec();
    delete sim;
}

void MainWindow::subWindowActivated( QMdiSubWindow* window )
{
    QUndoStack* undoStack = 0;
    ui->closeAction->setEnabled(window != 0);
    ui->closeAllAction->setEnabled(window != 0);
    ui->printAction->setEnabled(window != 0);
    ui->printPreviewAction->setEnabled(window != 0);
    ui->saveAction->setEnabled(window != 0);
	ui->saveAsImageAction->setEnabled(window != 0);
    if (window == 0){
        return;
    }
    DiagramWindow* diagramWindow = qobject_cast<DiagramWindow*>(window->widget());
    if (diagramWindow){
        undoStack = diagramWindow->diagram()->undoStack();

        ui->diagramShapeToolBox->addShapes(diagramWindow->diagram());
        //ui->diagramShapeToolBox->removeShape("output.analog", "Logic Circuit");
        connect(diagramWindow->diagram(), SIGNAL(selectionChanged()), SLOT(selectionChanged()));
    }
    if (undoStack == ui->undoView->stack()){
        return;
    }
    disconnect(ui->undoView->stack());
    connect(undoStack, SIGNAL(canRedoChanged(bool)), SLOT(redoStackCanRedoChanged(bool)));
    connect(undoStack, SIGNAL(canUndoChanged(bool)), SLOT(undoStackCanUndoChanged(bool)));

    ui->undoView->setStack(undoStack);
    ui->redoAction->setEnabled(undoStack && undoStack->index() > 0);
    ui->undoAction->setEnabled(undoStack && undoStack->count() > 0);

    updateWindowMenu();
}

void MainWindow::undoActionTriggered()
{
    CALL_ACTION(undo())
}

void MainWindow::redoStackCanRedoChanged ( bool canRedo )
{
    ui->redoAction->setEnabled(canRedo);
}

void MainWindow::undoStackCanUndoChanged ( bool canUndo )
{
    ui->undoAction->setEnabled(canUndo);
}

void MainWindow::updateWindowMenu()
{
    ui->windowMenu->clear();
    ui->windowMenu->addAction(ui->tileAction);
    ui->windowMenu->addAction(ui->cascadeAction);
    QAction* s = ui->windowMenu->addSeparator();

    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();
    s->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        DiagramWindow* dw= qobject_cast<DiagramWindow *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(dw->diagram()->title());
        } else {
            text = tr("%1 %2").arg(i + 1).arg(dw->diagram()->title());
        }
        QAction* action  = ui->windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(dw == activeDiagramWindow());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}

