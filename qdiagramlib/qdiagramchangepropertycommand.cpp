#include "stdafx.h"
#include "qdiagramchangepropertycommand.h"

QDiagramChangePropertyCommand::QDiagramChangePropertyCommand(QAbstractDiagram* diagram, QDiagramShape* item, const QString & name, const QVariant & oldValue, const QVariant & newValue, QUndoCommand* parent) :
    QDiagramUndoCommand(diagram, item->uuid(), "default", parent)
{
    m_old = oldValue;
    m_name = name;
    m_new = newValue;
    setText(QObject::tr("Change Property %1").arg(m_name));
}

bool QDiagramChangePropertyCommand::mergeWith(const QUndoCommand* other)
{
    if (id() != other->id()){
        return false;
    }
    const QDiagramChangePropertyCommand* cmd = static_cast<const QDiagramChangePropertyCommand *>(other);
    if (cmd->uuid() != uuid() || cmd->name() != m_name){
        return false;
    }
    QAbstractDiagramGraphicsItem* item = diagram()->findItemByUuid(uuid());
    if (item == 0){
        return false;
    }
    m_new = cmd->m_new;
    setText(QObject::tr("Change Property %1").arg(m_name));
	
    return true;
}

QString QDiagramChangePropertyCommand::name() const
{
    return m_name;
}

void QDiagramChangePropertyCommand::undo()
{
    QAbstractDiagramGraphicsItem* i = diagram()->findItemByUuid(uuid());
    if (i){
        i->blockUndoCommands(true);
        i->changeProperty(m_name, m_old);
        i->blockUndoCommands(false);
    }
}

void QDiagramChangePropertyCommand::redo()
{
    QAbstractDiagramGraphicsItem* i = diagram()->findItemByUuid(uuid());
    if (i){
        i->blockUndoCommands(true);
        i->changeProperty(m_name, m_new);
        i->blockUndoCommands(false);
    }
}
