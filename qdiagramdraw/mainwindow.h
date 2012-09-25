#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QClipboard>
#include <QMainWindow>
#include <QMdiSubWindow>

class QAbstractDiagramGraphicsItem;
class QDiagram;
class QSignalMapper;

class DiagramWindow;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QDiagram* activeDiagram() const;

    DiagramWindow* activeDiagramWindow() const;
public slots:
    void clipboardChanged(QClipboard::Mode mode);
    void copyActionTriggered();
    void cutActionTriggered();

    void diagramViewContextMenuRequested(const QPoint & point, const QPointF &scenePos);
    void diagramItemAdded(QAbstractDiagramGraphicsItem* item);

    void exitActionTriggered();

    void newActionTriggered();

    void openActionTriggered();

    void pasteActionTriggered();
    void printPreviewActionTriggered();

    void redoActionTriggered();
    void redoStackCanRedoChanged ( bool canRedo );

    void saveActionTriggered();
    void selectionChanged();
    void simulatorActionTriggered();
    void subWindowActivated( QMdiSubWindow* window );

    void undoActionTriggered();
    void undoStackCanUndoChanged ( bool canUndo );
private:
    void updateWindowMenu();

    QSignalMapper* windowMapper;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
