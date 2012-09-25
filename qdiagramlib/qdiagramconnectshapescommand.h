#ifndef QDIAGRAMCONNECTSHAPESCOMMAND_H
#define QDIAGRAMCONNECTSHAPESCOMMAND_H

#include <qdiagramundocommand.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramConnectShapesCommand : public QDiagramUndoCommand
{
public:
    QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to);
};

#endif // QDIAGRAMCONNECTSHAPESCOMMAND_H
