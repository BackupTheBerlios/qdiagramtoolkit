#include "stdafx.h"
#include "qdiagramremoveitemcommand.h"

QDiagramRemoveItemCommand::QDiagramRemoveItemCommand(QAbstractDiagramGraphicsItem *item, QUndoCommand *parent) :
    QDiagramUndoCommand(item->diagram(), item->uuid(), item->property("shape").toString(), item->properties(), item->property("plugin").toString(), parent)
{
    setText(QObject::tr("Remove %1").arg(uuid()));
}

QDiagramRemoveItemCommand::QDiagramRemoveItemCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QMap<QString,QVariant> & props, QUndoCommand* parent ) :
    QDiagramUndoCommand(diagram, uuid, shape, props, "default", parent)
{
    setText(QObject::tr("Remove %1").arg(shape));
}

void QDiagramRemoveItemCommand::undo()
{
    diagram()->restoreItem(metaData(), properties());
}

void QDiagramRemoveItemCommand::redo()
{
    QAbstractDiagramGraphicsItem* item = diagram()->findItemByUuid(uuid());
    diagram()->takeItem(item);
    delete item;
}
