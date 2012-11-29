#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QClipboard>
#include <QMainWindow>
#include <QMdiSubWindow>

class QAbstractDiagramGraphicsItem;
class QDiagram;
class QSignalMapper;

class DiagramWindow;
class QDiagramViewControlPanel;

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
	void bringForwardActionTriggered();
	void bringToFrontActionTriggered();

    void clipboardChanged(QClipboard::Mode mode);
    void copyActionTriggered();
    void cutActionTriggered();

    void diagramViewContextMenuRequested(const QPoint & point, const QPointF &scenePos);
    void diagramItemAdded(QAbstractDiagramGraphicsItem* item);
	void diagramTabContextMenuRequested(const QPoint & pos);

    void exitActionTriggered();

	void groupActionTriggered();

	void insertPageActionTriggered();

    void newActionTriggered();

    void openActionTriggered();

    void pasteActionTriggered();
    void printPreviewActionTriggered();

    void redoActionTriggered();
    void redoStackCanRedoChanged ( bool canRedo );

    void saveActionTriggered();
	void saveAsImageActionTriggered();
	void scriptEditorActionTriggered();
    void selectionChanged();
	void sendBackwardActionTriggered();
	void sendToBackActionTriggered();
    void simulatorActionTriggered();
    void subWindowActivated( QMdiSubWindow* window );

    void undoActionTriggered();
    void undoStackCanUndoChanged( bool canUndo );
	void ungroupActionTriggered();
private:
    void updateWindowMenu();

	QDiagramViewControlPanel* controlPanel;
    QSignalMapper* windowMapper;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
