#include "stdafx.h"
#include "qdiagramconnectshapescommand.h"

QDiagramConnectShapesCommand::QDiagramConnectShapesCommand(QAbstractDiagram* diagram, QAbstractDiagramShapeConnector* connector, const QString & plugin, QUndoCommand* parent) :
    QDiagramUndoCommand(diagram, connector->uuid(), "Connector", connector->properties(), plugin, parent)
{
    setText(QObject::tr("Connect Shape (%1)").arg(connector->property("style").toString()));
}


QDiagramConnectShapesCommand::QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style, const QString & plugin, QUndoCommand* parent) :
    QDiagramUndoCommand(diagram, uuid, "Connector", plugin, parent)
{
    QMap<QString,QVariant> mProperties(properties());

    mProperties["shapeAtStart"] = from->parentShape()->uuid();
    mProperties["pointAtStart"] = from->id();
    mProperties["shapeAtEnd"] = to->parentShape()->uuid();
    mProperties["pointAtEnd"] = to->id();
    mProperties["plugin"] = style.plugin();
    mProperties["style"] = style.shape();
    mProperties["itemType"] = "Connector";
    mProperties["uuid"] = uuid;

    setProperties(mProperties);

    setText(QObject::tr("Add Connector (%1)").arg(uuid));
}

QDiagramConnectShapesCommand::QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, const QVariantMap & metaData, const QVariantMap & properties, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, QUndoCommand* parent) :
	QDiagramUndoCommand(diagram, uuid, metaData, properties, parent)
{
    QMap<QString,QVariant> p(properties);

	QDiagramConnectionPoint cp;
	cp = QDiagramConnectionPoint(from->parentShape()->uuid(), from->id());
	p["start"] = QDiagramProperty::toMap(qVariantFromValue(cp));
	cp = QDiagramConnectionPoint(to->parentShape()->uuid(), to->id());
	p["end"] = QDiagramProperty::toMap(qVariantFromValue(cp));
    p["uuid"] = uuid;

    setProperties(p);

    setText(QObject::tr("Add Connector (%1)").arg(uuid));
}

void QDiagramConnectShapesCommand::undo()
{
    QAbstractDiagramGraphicsItem* mItem = diagram()->findItemByUuid(uuid());
    if (mItem){
        mItem->blockUndoCommands(true);
        diagram()->removeItem(uuid());
    }
}

void QDiagramConnectShapesCommand::redo()
{
    diagram()->restoreItem(metaData(), properties());
}
