/******************************************************************************
** Copyright (C) 2011 Martin Hoppe martin@2x2hoppe.de
**
** This file is part of the QDiagram Toolkit (qdiagramlib)
**
** qdiagramlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as
** published by the Free Software Foundation, either version 3 of the
** License, or (at your option) any later version.
**
** qdiagramlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Leser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with qdialgramlib.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "stdafx.h"
#include "qdiagram.h"
#include "qdiagram_p.h"

#include <qabstractdiagramscene.h>

#include <qdiagraminsertshapecommand.h>
#include <qdiagrammoveshapecommand.h>
#include <qdiagrampluginloader.h>

#include <qdiagramshapeconnector.h>
#include <qdiagramshapeconnectionpoint.h>
#include <qdiagramundocommand.h>

#include <json.h>

QDiagram::QDiagram(QObject* parent) :
    QAbstractDiagram(parent)
{
}

QAbstractDiagramGraphicsItem* QDiagram::addItem(const QString & uuid, const QString & shape, const QMap<QString,QVariant> & properties, const QString & plugin)
{
    QUuid mUuid;
    if (uuid.isEmpty()){
        mUuid = QUuid::createUuid();
    } else {
        mUuid = QUuid(uuid);
    }

    QAbstractDiagramGraphicsItem* mItem = 0;
    QAbstractDiagramPlugin* mPlugin = QDiagramPluginLoader::plugin(plugin);
    if (mPlugin){
        // TODO
//        mItem = mPlugin->createShape(mUuid, shape, properties, scene());
//        scene()->addItem(mItem);
    }
    return mItem;
}

void QDiagram::addConnection(QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style)
{
    undoStack()->push(new QDiagramConnectShapesCommand(this, QUuid::createUuid().toString(), from, to, style));
}

QString QDiagram::addShape(const QString & name, const QPointF & pos, const QMap<QString,QVariant> & properties, const QString & plugin)
{
    QUuid uuid = QUuid::createUuid();
    QVariantMap m(properties);
    QAbstractDiagramPlugin* p = QDiagramPluginLoader::load(plugin);
    if (p){
        QVariantMap dp = p->defaultProperties(name);
		QVariantMap g = dp.value("geometry").toMap();
		if (g.isEmpty()){
			g = properties.value("geometry").toMap();
		}
        g["x"] = pos.x() + p->hotSpot(name).x();
        g["y"] = pos.y() + p->hotSpot(name).y();
        m["geometry"] = g;
        QMapIterator<QString,QVariant> it(dp);
		while(it.hasNext()){
			it.next();
            if (!m.contains(it.key())){
                m[it.key()] = it.value();
            }
		}
	}
    undoStack()->push(new QDiagramInsertShapeCommand(this, uuid.toString(), name, m, plugin));
    emit shapeAdded(uuid.toString());
    emit contentsChanged();
    return uuid.toString();
}

void QDiagram::contextMenuEventHandler(QGraphicsSceneContextMenuEvent *event)
{
    event->ignore();
}

QAbstractDiagramShapeConnector* QDiagram::createConnection(const QString & from, const QString &fromId, const QString & to, const QString &toId, const QDiagramConnectorStyle & style)
{
    QAbstractDiagramShapeConnector* mConnector = 0;
    QAbstractDiagramShape* mFromShape = shape(from);
    QAbstractDiagramShapeConnectionPoint* mFrom = 0;
    QAbstractDiagramShape* mToShape = shape(to);
    QAbstractDiagramShapeConnectionPoint* mTo = 0;
    QAbstractDiagramPlugin* mPlugin = QDiagramPluginLoader::plugin(style.plugin());
    if (mFromShape){
        mFrom = mFromShape->connectionPoint(fromId);
    }
    if (mToShape){
        mTo = mToShape->connectionPoint(toId);
    }
    if (mPlugin && mFrom && mTo){
//        mConnector = mPlugin->createConnection(QUuid::createUuid().toString(), mFrom, mTo, style.shape());
//        scene()->addItem(mConnector);
    }
    return mConnector;
}

QString QDiagram::type() const
{
    return QLatin1String("Standard");
}

QString QDiagram::plugin() const
{
    return QLatin1String("Standard");
}

void QDiagram::beginRestoreDiagram()
{
    blockSignals(true);
}

void QDiagram::endRestoreDiagram()
{
    Q_FOREACH(QAbstractDiagramShapeConnector* mConnector, connectors()){
        qDebug() << "rebuilding connection" << mConnector->uuid();
// @todo        mConnector->rebuildConnection();
    }

    blockSignals(false);
}

QByteArray QDiagram::serialize(QAbstractDiagram *diagram, bool includeItems)
{
    QVariantMap p;
    p["plugins"] = diagram->plugins();
    p["title"] = diagram->title();
    if (includeItems){
        QVariantMap i;
        Q_FOREACH(QAbstractDiagramGraphicsItem* item, diagram->items()){
            i[item->uuid()] = item->properties();
        }

        p["items"] = i;
    }
    return QtJson::Json::serialize(p);
}

void QDiagram::itemRestoredHandler(QAbstractDiagramGraphicsItem* item)
{
    Q_UNUSED(item);
    emit itemRestored(item);
}

QDiagramLineStyle QDiagram::lineStyle(const QString &id)
{
    Q_FOREACH(QDiagramLineStyle style, QDiagram::linesStyles()){
        if (style.id() == id){
            return style;
        }
    }

    return QDiagramLineStyle();
}

QList<QDiagramLineStyle> QDiagram::linesStyles()
{
    QList<QDiagramLineStyle> styles;
    Q_FOREACH(QAbstractDiagramPlugin* plugin, QDiagramPluginLoader::loadedPlugins()){
        styles.append(plugin->lineStyles());
    }
    return styles;
}

void QDiagram::connectItemsEventHandler( QDiagramShape* from, QDiagramShape* to, const QMap<QString,QVariant> & properties)
{
//    itemFactory()->connection(from, to, properties, scene());
}

void QDiagram::itemMoved(QGraphicsItem* item, const QPointF & oldPos, const QPointF & newPos)
{
    QAbstractDiagramShape* mItem = dynamic_cast<QAbstractDiagramShape*>(item);
    if (mItem){
        QDiagramMoveShapeCommand * mCmd = new QDiagramMoveShapeCommand(this, mItem, oldPos, gridPos(newPos));
        undoStack()->push(mCmd);
    }
}

bool QDiagram::removeItem( const QString & uuid )
{
    QAbstractDiagramGraphicsItem* item = findItemByUuid(uuid);
    if (item){
        if (item->undoCommandsBlocked()){
            takeItem(item);
            delete item;
        } else {
            QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(item);
            if (s){
                QList<QAbstractDiagramShapeConnector*> cs = s->connectors();
                if (!cs.isEmpty()){
                    undoStack()->beginMacro("Remove items");
                    Q_FOREACH(QAbstractDiagramShapeConnector* c, cs){
                        undoStack()->push(new QDiagramRemoveItemCommand(c));
                    }
                    undoStack()->push(new QDiagramRemoveItemCommand(item));
                    undoStack()->endMacro();
                } else {
                    undoStack()->push(new QDiagramRemoveItemCommand(item));
                }
            } else {
                undoStack()->push(new QDiagramRemoveItemCommand(item));
            }
        }
        return true;
    }
    return false;
}

QAbstractDiagramGraphicsItem *QDiagram::restoreItem(const QMap<QString, QVariant> & properties)
{
    QAbstractDiagramGraphicsItem* item = 0;
    QAbstractDiagramPlugin* mPlugin = QDiagramPluginLoader::load(properties.value("plugin", "default").toString());
    if (mPlugin){
        item = mPlugin->createItem(properties, scene());
        itemRestoredHandler(item);
    } else {
        qWarning() << "Plugin" << properties.value("plugin", "default").toString() << "not found";
    }
    return item;
}

void QDiagram::dragEnterEventHandler( QGraphicsSceneDragDropEvent* event )
{
    if (event->mimeData()->hasFormat("application/qdiagram.shape")){
         event->accept();
     } else {
         event->ignore();
    }
}

void QDiagram::dragLeaveEventHandler( QGraphicsSceneDragDropEvent* event )
{
}

void QDiagram::dragMoveEventHandler( QGraphicsSceneDragDropEvent* event )
{
    if (event->mimeData()->hasFormat("application/qdiagram.shape")){
         event->accept();
     } else {
         event->ignore();
    }
}

void QDiagram::dropEventHandler( QGraphicsSceneDragDropEvent* event )
{
    if (event->mimeData()->hasFormat("application/qdiagram.shape")) {
        QByteArray data = event->mimeData()->data("application/qdiagram.shape");
        QDataStream dataStream(&data, QIODevice::ReadOnly);

        QVariant plugin;
        QVariant shape;
        QVariant p;
        dataStream >> plugin >> shape >> p;

		addShape(shape.toString(), gridPos(event->scenePos()), p.toMap(), plugin.toString());
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}
