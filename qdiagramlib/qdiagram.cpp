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

#include <qabstractdiagram.h>
#include <qabstractdiagramscene.h>

#include <qdiagraminsertshapecommand.h>
#include <qdiagrammoveshapecommand.h>
#include <qdiagrampluginloader.h>

#include <qdiagramshapeconnector.h>
#include <qdiagramremoveitemcommand.h>
#include <qdiagramshapeconnectionpoint.h>
#include <qdiagramundocommand.h>
#include <qdiagramconnectshapescommand.h>

#include <json.h>

QDiagram::QDiagram(QObject* parent) :
QAbstractDiagram(parent)
{
}

QDiagram::~QDiagram()
{
}

void QDiagram::addConnection(QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style)
{
	QVariantMap metaData;
	metaData["plugin"] = style.plugin();
	metaData["itemType"] = "connector";
	metaData["itemClass"] = style.shape();
	QVariantMap properties;
	properties = from->customConnectorProperties();
	undoStack()->push(new QDiagramConnectShapesCommand(this, QUuid::createUuid().toString(), metaData, properties, from, to));
}

QString QDiagram::addShape(const QString & name, qreal x, qreal y, const QMap<QString,QVariant> & properties, const QString & plugin)
{
	return addShape(name, QPointF(x, y), properties, plugin);
}

QString QDiagram::addShape(const QString & name, const QPointF & pos, const QMap<QString,QVariant> & properties, const QString & plugin)
{
    QUuid uuid = QUuid::createUuid();
	QVariantMap m(properties);
	QVariantMap metaData;
	metaData["plugin"] = plugin;
	metaData["itemType"] = "shape";
	metaData["itemClass"] = name;
    QAbstractDiagramPlugin* p = QDiagramPluginLoader::load(plugin);
	if (p == 0){
		return QString::null;
	}
	QVariantMap dp = p->defaultProperties(name);
	QVariantMap g = dp.value("geometry").toMap();
	if (g.isEmpty()){
		g = properties.value("geometry").toMap();
	}
	QPointF np(pos.x() + p->hotSpot(metaData, properties).x(), pos.y() + p->hotSpot(metaData, properties).y());
	if (np.x() < 0){
		np.setX(0);
	}
	if (np.y() < 0){
		np.setY(0);
	}
	g["x"] = np.x();
	g["y"] = np.y();
	m["geometry"] = g;
	QMapIterator<QString,QVariant> it(dp);
	while(it.hasNext()){
		it.next();
		if (!m.contains(it.key())){
			m[it.key()] = it.value();
		}
	}
	m["uuid"] = uuid.toString();
	undoStack()->push(new QDiagramInsertShapeCommand(this, uuid.toString(), metaData, m));
    emit shapeAdded(uuid.toString());
    emit contentsChanged();
    return uuid.toString();
}

void QDiagram::contextMenuEventHandler(QGraphicsSceneContextMenuEvent *event)
{
    event->ignore();
}

QString QDiagram::connectShapes(const QString & fromUUID, const QString & from, const QString & toUUID, const QString & to, const QDiagramConnectorStyle & style)
{
	QAbstractDiagramShape* fs = shape(fromUUID);
	QAbstractDiagramShape* ts = shape(toUUID);
	if (fs == 0 || ts == 0){
		return QString::null;
	}
    QAbstractDiagramShapeConnectionPoint* fcp = fs->connectionPoint(from);
    QAbstractDiagramShapeConnectionPoint* tcp = ts->connectionPoint(to);
	if (fcp == 0 || tcp == 0){
		return QString::null;
	}
	QString uuid = QUuid::createUuid().toString();
	QVariantMap metaData;
	metaData["plugin"] = style.plugin();
	metaData["itemType"] = "connector";
	metaData["itemClass"] = style.name();
	
	QVariantMap properties;

	undoStack()->push(new QDiagramConnectShapesCommand(this, uuid, metaData, properties, fcp, tcp));
	return uuid;
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
    QAbstractDiagramShape* i = dynamic_cast<QAbstractDiagramShape*>(item);
    if (i){
        QDiagramMoveShapeCommand* c = new QDiagramMoveShapeCommand(this, i, oldPos, gridPos(newPos));
        undoStack()->push(c);
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

QAbstractDiagramGraphicsItem *QDiagram::restoreItem(const QMap<QString,QVariant> & metaData, const QMap<QString, QVariant> & properties)
{
    QAbstractDiagramGraphicsItem* item = 0;
    QAbstractDiagramPlugin* p = QDiagramPluginLoader::load(metaData.value("plugin", "default").toString());
    if (p){
        item = p->createItem(metaData, properties, scene());

		page(currentPage())->addItem(item);
		if (properties.value("layers").isNull()){
			layers()->activeLayer()->add(item);
		}
        itemRestoredHandler(item);
    } else {
        qWarning() << "Plugin" << metaData.value("plugin", "default").toString() << "not found";
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

        QVariant metaData;
        QVariant properties;
        dataStream >> metaData >> properties;

		addShape(metaData.toMap().value("itemClass").toString(), gridPos(event->scenePos()), properties.toMap(), metaData.toMap().value("plugin").toString());
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}
