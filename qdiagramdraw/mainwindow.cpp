#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QClipboard>
#include <QFile>
#include <QFileDialog>
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

//    QSysMLProjectModel* mProject = new QSysMLProjectModel(this);

//    ui->diagramShapeToolBox->addShapes(QDiagramPluginLoader::load("Logic Circuit"));
//    ui->diagramShapeToolBox->addShapes(QDiagramPluginLoader::load("Standard Forms"));
//    ui->projectTreeView->setModel(mProject);

//    SysMLDiagramWindow* mWindow = new SysMLDiagramWindow();
//    mWindow->setDiagram(mProject->loadDiagram("{d4d34e3f-da7e-4acc-b02a-20a2ce4165ee}"));
//    ui->mdiArea->addSubWindow(mWindow);

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
        QList<QGraphicsItem*> items = view->graphicsView()->items(point);
        QMenu menu;
        if (items.isEmpty()){
            menu.addAction(ui->pasteAction);
        } else if (items.size() == 1){
            menu.addAction(ui->cutAction);
            menu.addAction(ui->copyAction);
            menu.addAction(ui->pasteAction);
        } else {

        }
        menu.addSeparator();
        menu.addAction(ui->refreshAction);

        ui->pasteAction->setData(scenePos);
        menu.exec(view->graphicsView()->viewport()->mapToGlobal(point));
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
    DiagramWindow* w = new DiagramWindow();
    QDiagram* d = new QDiagram(this);
    connect(d, SIGNAL(itemRestored(QAbstractDiagramGraphicsItem*)), this, SLOT(diagramItemAdded(QAbstractDiagramGraphicsItem*)));
    //d->addPlugin("Standard Forms");
    d->addPlugin("Logic Circuit");
    //d->addPlugin("SysML");
    d->setTitle("101");

    d->addItemContextMenuAction(ui->cutAction);
    d->addItemContextMenuAction(ui->copyAction);
    d->addItemContextMenuAction(ui->pasteAction);
    w->setDiagram(d);
    connect(w->diagramView(), SIGNAL(graphicsViewContextMenuRequested(QPoint,QPointF)), this, SLOT(diagramViewContextMenuRequested(QPoint,QPointF)));

    ui->mdiArea->addSubWindow(w);
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

