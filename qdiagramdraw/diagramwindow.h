#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QWidget>

class QDiagram;
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

    QDiagram* diagram() const;

    QDiagramView *diagramView() const;

    void setDiagram(QDiagram* diagram);
public slots:
    void print(QPrinter* printer);

    void save();

	void saveAsImage();

    void undo();

    void redo();
private slots:
    void diagramContentsChanged();
private:
    QDiagram* cDiagram;
    Ui::DiagramWindow *ui;
};

#endif // DIAGRAMWINDOW_H
