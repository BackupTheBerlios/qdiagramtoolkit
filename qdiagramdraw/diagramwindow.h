#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QWidget>

class QAbstractDiagram;
class QDiagramView;

namespace Ui {
    class DiagramWindow;
}

class DiagramWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DiagramWindow(QWidget *parent = 0);
    ~DiagramWindow();

    QAbstractDiagram* diagram() const;

    QDiagramView *diagramView() const;

    void setDiagram(QAbstractDiagram* diagram);
public slots:
	void group();

    void print(QPrinter* printer);

    void save();

	void saveAsImage();

    void undo();

	void ungroup();

    void redo();
private slots:
    void diagramContentsChanged();
private:
    QAbstractDiagram* cDiagram;
    Ui::DiagramWindow *ui;
};

#endif // DIAGRAMWINDOW_H
