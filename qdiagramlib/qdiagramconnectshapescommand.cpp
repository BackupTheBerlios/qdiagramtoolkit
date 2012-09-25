#include "qdiagramconnectshapescommand.h"

QDiagramConnectShapesCommand::QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to) :
    QDiagramUndoCommand(diagram)
{
}
