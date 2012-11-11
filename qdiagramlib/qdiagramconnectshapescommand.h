#ifndef QDIAGRAMCONNECTSHAPESCOMMAND_H
#define QDIAGRAMCONNECTSHAPESCOMMAND_H

#include <qdiagramundocommand.h>

#include <qabstractdiagramshapeconnector.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramConnectShapesCommand : public QDiagramUndoCommand
{
public:
    QDiagramConnectShapesCommand(QAbstractDiagram* diagram, QAbstractDiagramShapeConnector* connector, const QString & plugin = "default", QUndoCommand* parent = 0);
    QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style, const QString & plugin = "default", QUndoCommand* parent = 0);

	QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, const QVariantMap & metaData, const QVariantMap & properties, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, QUndoCommand* parent = 0);
    void undo();

    void redo();
};

#endif // QDIAGRAMCONNECTSHAPESCOMMAND_H
