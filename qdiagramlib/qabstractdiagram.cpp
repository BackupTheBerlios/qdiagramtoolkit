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
#include "qabstractdiagram.h"

#include "qabstractdiagramplugin.h"
#include "qabstractdiagramshapeconnector.h"
#include "qdiagramsheet.h"
#include "qdiagrammetadata.h"
#include "qdiagrampluginloader.h"
#include "qdiagramstylesheet.h"
#include <qdiagramsheet.h>

#include <qdiagraminsertshapecommand.h>
#include <qdiagrammoveshapecommand.h>
#include <qdiagrampluginloader.h>

#include <qdiagramshapeconnector.h>
#include <qdiagramremoveitemcommand.h>
#include <qdiagramshapeconnectionpoint.h>
#include <qdiagramundocommand.h>
#include <qdiagramconnectshapescommand.h>

QAbstractDiagram::QAbstractDiagram(const QString & pluginName, QObject *parent) :
    QObject(parent)
{
	m_index = 0;
	m_pluginName = pluginName;
	m_styleSheet = new QDiagramStyleSheet(this);
	blockSignals(true);
	addSheet(tr("Diagram1"));
	blockSignals(false);
	m_undostack = new QUndoStack(this);

    connect(m_undostack, SIGNAL(indexChanged(int)), SLOT(undoStackIndexChanged(int)));
}

QAbstractDiagram::~QAbstractDiagram()
{
	delete m_styleSheet;
}

void QAbstractDiagram::addConnection(QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style)
{
	QVariantMap metaData;
	metaData["plugin"] = style.plugin();
	metaData["itemType"] = "Connector";
	metaData["itemClass"] = style.shape();
	QVariantMap properties;
	properties = from->customConnectorProperties();
	undoStack()->push(new QDiagramConnectShapesCommand(this, QUuid::createUuid().toString(), metaData, properties, from, to));
}

void QAbstractDiagram::addItemContextMenuAction(QAction *action)
{
    m_standardItemContextMenuActions.append(action);
}

QString QAbstractDiagram::addShape(const QString & plugin, const QString & id, const QPointF & pos)
{
    QUuid uuid = QUuid::createUuid();
	//QVariantMap m;
	QVariantMap metaData;
    QAbstractDiagramPlugin* p = QDiagramPluginLoader::load(plugin);
	if (p == 0){
		return QString::null;
	}
	QVariantMap dp = p->defaultProperties(id);
	QPointF np(pos.x() + p->hotSpot(id).x(), pos.y() + p->hotSpot(id).y());
	if (np.x() < 0){
		np.setX(0);
	}
	if (np.y() < 0){
		np.setY(0);
	}
	QVariantMap g;
	g["x"] = np.x();
	g["y"] = np.y();
	dp["geometry"] = g;

	metaData = p->metaData(id);
	metaData["plugin"] = plugin;
	dp["uuid"] = uuid.toString();
	undoStack()->push(new QDiagramInsertShapeCommand(this, uuid.toString(), metaData, dp));
    emit shapeAdded(uuid.toString());
	emit contentsChanged(indexOf(currentSheet()));
    return uuid.toString();

	//QVariantMap g = dp.value("geometry").toMap();
	//if (g.isEmpty()){
	//	g = properties.value("geometry").toMap();
	//}

	//QPointF np(pos.x() + p->hotSpot(metaData, properties).x(), pos.y() + p->hotSpot(metaData, properties).y());
	//if (np.x() < 0){
	//	np.setX(0);
	//}
	//if (np.y() < 0){
	//	np.setY(0);
	//}
	//g["x"] = np.x();
	//g["y"] = np.y();
	//m["geometry"] = g;
	//QMapIterator<QString,QVariant> it(dp);
	//while(it.hasNext()){
	//	it.next();
	//	if (!m.contains(it.key())){
	//		m[it.key()] = it.value();
	//	}
	//}
	//m["uuid"] = uuid.toString();
	//undoStack()->push(new QDiagramInsertShapeCommand(this, uuid.toString(), metaData, m));
 //   emit shapeAdded(uuid.toString());
	//emit contentsChanged(currentSheet());
    //return uuid.toString();
}

//QString QDiagram::addShape(const QString & name, const QPointF & pos, const QMap<QString,QVariant> & properties, const QString & plugin)
//{
//    QUuid uuid = QUuid::createUuid();
//	QVariantMap m(properties);
//	QVariantMap metaData;
//	metaData["plugin"] = plugin;
//	metaData["itemType"] = "Shape";
//	metaData["itemClass"] = name;
//    QAbstractDiagramPlugin* p = QDiagramPluginLoader::load(plugin);
//	if (p == 0){
//		return QString::null;
//	}
//	QVariantMap dp = p->defaultProperties(name);
//	QVariantMap g = dp.value("geometry").toMap();
//	if (g.isEmpty()){
//		g = properties.value("geometry").toMap();
//	}
//	QPointF np(pos.x() + p->hotSpot(metaData, properties).x(), pos.y() + p->hotSpot(metaData, properties).y());
//	if (np.x() < 0){
//		np.setX(0);
//	}
//	if (np.y() < 0){
//		np.setY(0);
//	}
//	g["x"] = np.x();
//	g["y"] = np.y();
//	m["geometry"] = g;
//	QMapIterator<QString,QVariant> it(dp);
//	while(it.hasNext()){
//		it.next();
//		if (!m.contains(it.key())){
//			m[it.key()] = it.value();
//		}
//	}
//	m["uuid"] = uuid.toString();
//	undoStack()->push(new QDiagramInsertShapeCommand(this, uuid.toString(), metaData, m));
//    emit shapeAdded(uuid.toString());
//	emit contentsChanged(currentSheet());
//    return uuid.toString();
//}

int QAbstractDiagram::addSheet(const QString & name)
{
	sheetData p;
	p.layers = new QDiagramLayers(this);
	p.sheet = new QDiagramSheet(this);
	if (name.isNull()){
		p.sheet->setName(tr("Diagram%1").arg(m_sheets.size() + 1));
	} else {
		p.sheet->setName(name);
	}
	m_sheets << p;

    connect(p.sheet, SIGNAL(itemMoved(QGraphicsItem*,QPointF,QPointF)), this, SLOT(itemMoved(QGraphicsItem*,QPointF,QPointF)));
	connect(p.sheet, SIGNAL(itemPropertyChanged(QAbstractDiagramGraphicsItem*,QString)), this, SIGNAL(itemPropertyChanged(QAbstractDiagramGraphicsItem*,QString)));
    connect(p.sheet, SIGNAL(selectionChanged()), SIGNAL(selectionChanged()));
//    connect(p.sheet, SIGNAL(contentsChanged(int)), this, SLOT(pageContentsChanged()));

	emit pageAdded(m_sheets.size() - 1);
	return m_sheets.size() - 1;
}

void QAbstractDiagram::addPlugin(const QString & name)
{
    if (!m_plugins.contains(name)){
        m_plugins.append(name);
    }
}

QString QAbstractDiagram::author() const
{
    return m_author;
}

void QAbstractDiagram::beginRestoreDiagram()
{
    blockSignals(true);
}

QStringList QAbstractDiagram::blockedShapes() const
{
	return QStringList();
}

void QAbstractDiagram::clearSelection()
{
    Q_FOREACH(QGraphicsItem* i, m_sheets.at(m_index).sheet->items()){
		i->setSelected(false);
	}
}

QList<QAbstractDiagramShapeConnector*> QAbstractDiagram::connectors() const
{
    QList<QAbstractDiagramShapeConnector*> mConnections;
    Q_FOREACH(QGraphicsItem* mItem, m_sheets.at(m_index).sheet->items()){
        QAbstractDiagramShapeConnector* mConnector = dynamic_cast<QAbstractDiagramShapeConnector*>(mItem);
        if (mConnector){
            mConnections.append(mConnector);
        }
    }
    return mConnections;
}

QString QAbstractDiagram::connectShapes(const QString & fromUUID, const QString & from, const QString & toUUID, const QString & to, const QDiagramConnectorStyle & style)
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
	metaData["itemType"] = "Connector";
	metaData["itemClass"] = style.name();
	
	QVariantMap properties;

	undoStack()->push(new QDiagramConnectShapesCommand(this, uuid, metaData, properties, fcp, tcp));
	return uuid;
}

void QAbstractDiagram::contextMenuEventHandler(QGraphicsSceneContextMenuEvent *event)
{
    event->ignore();
}

QDiagramSheet* QAbstractDiagram::currentSheet() const
{
	return m_sheets.at(m_index).sheet;
}

QVariant QAbstractDiagram::defaultValue(QDiagramToolkit::PropertyType type) const
{
	return QDiagramProperty::defaultValue(type);
}

void QAbstractDiagram::dragEnterEventHandler( QGraphicsSceneDragDropEvent* event )
{
    if (event->mimeData()->hasFormat("application/qdiagram.shape")){
         event->accept();
     } else {
         event->ignore();
    }
}

void QAbstractDiagram::dragLeaveEventHandler( QGraphicsSceneDragDropEvent* event )
{
}

void QAbstractDiagram::dropEventHandler( QGraphicsSceneDragDropEvent* event )
{
    if (event->mimeData()->hasFormat("application/qdiagram.shape")) {
        QByteArray d = event->mimeData()->data("application/qdiagram.shape");
        QDataStream s(&d, QIODevice::ReadOnly);

		//QVariant itemClass;
		//QVariant plugin;
		//QVariant id;

		//s >> itemClass >> plugin >> id;
  //      QVariant metaData;
  //      QVariant properties;
  //      s >> metaData >> properties;

		//addShape(metaData.toMap().value("itemClass").toString(), snapPos(event->scenePos()), properties.toMap(), metaData.toMap().value("plugin").toString());
		QVariant id;
		QVariant plugin;
		s >> plugin >> id;
		addShape(plugin.toString(), id.toString(), snapPos(event->scenePos()));
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void QAbstractDiagram::dragMoveEventHandler( QGraphicsSceneDragDropEvent* event )
{
    if (event->mimeData()->hasFormat("application/qdiagram.shape")){
         event->accept();
     } else {
         event->ignore();
    }
}

void QAbstractDiagram::drawDiagramFrame(QPainter* painter, QDiagramSheet* sheet)
{
	Q_UNUSED(painter);
	Q_UNUSED(sheet);
}

QDiagramEndOfLineStyle QAbstractDiagram::endOfLineStyle(const QString &id) const
{
    Q_FOREACH(QString name, m_plugins){
        QAbstractDiagramPlugin* plugin = QDiagramPluginLoader::plugin(name, false);
        if (plugin){
            Q_FOREACH(QDiagramEndOfLineStyle style, plugin->endOfLineStyles()){
                if (style.id() == id){
                    return style;
                }
            }
        }
    }

    return QDiagramEndOfLineStyle();
}

QList<QDiagramEndOfLineStyle> QAbstractDiagram::endOfLineStyles() const
{
    QList<QDiagramEndOfLineStyle> s;
    Q_FOREACH(QString name, m_plugins){
        QAbstractDiagramPlugin* plugin = QDiagramPluginLoader::load(name, false);
        if (plugin){
            s += plugin->endOfLineStyles();
        }
    }
    return s;
}

void QAbstractDiagram::endRestoreDiagram()
{
    Q_FOREACH(QAbstractDiagramShapeConnector* mConnector, connectors()){
        qDebug() << "rebuilding connection" << mConnector->uuid();
// @todo        mConnector->rebuildConnection();
    }

    blockSignals(false);
}

QPointF QAbstractDiagram::snapPos(const QPointF & point) const
{
    QPointF p(point);
	if (m_sheets.at(m_index).sheet->isSnapEnabled()){
        p.setX(m_sheets.at(m_index).sheet->snapSize().width() * (int)(p.x() / m_sheets.at(m_index).sheet->snapSize().width()));
        p.setY(m_sheets.at(m_index).sheet->snapSize().height() * (int)(p.y() / m_sheets.at(m_index).sheet->snapSize().height()));
    }
    return p;
}

QAbstractDiagramGraphicsItem* QAbstractDiagram::findItemByUuid( const QString & uuid ) const
{
    if (uuid.isEmpty()){
        return 0;
    }
    Q_FOREACH(QAbstractDiagramGraphicsItem* i, items()){
        if (i->uuid() == uuid){
            return i;
        }
    }
    return 0;
}

int QAbstractDiagram::indexOf(QDiagramSheet* sheet) const
{
	for (int i = 0; i < m_sheets.size(); i++){
		if (m_sheets.at(i).sheet == sheet){
			return i;
		}
	}
	return -1;
}

bool QAbstractDiagram::isModified() const
{
    return !m_undostack->isClean();
}

bool QAbstractDiagram::isSnapEnabled() const
{
	return m_sheets.at(m_index).sheet->isSnapEnabled();
}

void QAbstractDiagram::itemMoved(QGraphicsItem* item, const QPointF & oldPos, const QPointF & newPos)
{
    QAbstractDiagramShape* i = dynamic_cast<QAbstractDiagramShape*>(item);
    if (i){
        QDiagramMoveShapeCommand* c = new QDiagramMoveShapeCommand(this, i, oldPos, snapPos(newPos));
        undoStack()->push(c);
    }
}

void QAbstractDiagram::itemRestoredHandler(QAbstractDiagramGraphicsItem* item)
{
    emit itemRestored(item);
}

QList<QAbstractDiagramGraphicsItem*> QAbstractDiagram::items(int page) const
{
    QList<QAbstractDiagramGraphicsItem*> items;
	for (int i = 0; i < m_sheets.size(); i++){
		if (page == -1 || i == page){
			Q_FOREACH(QGraphicsItem* mGraphicsItem, m_sheets.at(i).sheet->items()){
				QAbstractDiagramGraphicsItem* mDiagramItem = dynamic_cast<QAbstractDiagramGraphicsItem*>(mGraphicsItem);
				if (mDiagramItem){
					items.append(mDiagramItem);
				}
			}
			if (i == page){
				break;
			}
		}
    }
    return items;
}

QDiagramLayers* QAbstractDiagram::layers(int page) const
{
	if (page < m_sheets.size()){
		return m_sheets.at(page).layers;
	}
	return 0;
}

QDiagramLineStyle QAbstractDiagram::lineStyle(const QString &id)
{
	Q_FOREACH(QDiagramLineStyle style, QAbstractDiagram::linesStyles()){
        if (style.id() == id){
            return style;
        }
    }

    return QDiagramLineStyle();
}

QList<QDiagramLineStyle> QAbstractDiagram::linesStyles()
{
    QList<QDiagramLineStyle> styles;
    Q_FOREACH(QAbstractDiagramPlugin* plugin, QDiagramPluginLoader::loadedPlugins()){
        styles.append(plugin->lineStyles());
    }
    return styles;
}

QDiagramSheet* QAbstractDiagram::sheet(int index) const
{
	if (index < m_sheets.size()){
		return m_sheets.at(index).sheet;
	}
	return 0;
}

void QAbstractDiagram::pageContentsChanged()
{
	QDiagramSheet* s = qobject_cast<QDiagramSheet*>(sender());
	if (s){
		emit contentsChanged(indexOf(s));
	}
}

int QAbstractDiagram::sheetCount() const
{
	return m_sheets.size();
}

QDiagramToolkit::PaperOrientation QAbstractDiagram::paperOrientation(int index) const
{
	if (index < m_sheets.size()){
		return m_sheets.at(index).sheet->paperOrientation();
	}
	return QDiagramToolkit::Portrait;
}

QString QAbstractDiagram::pluginName() const
{
	return m_pluginName;
}

QStringList QAbstractDiagram::plugins() const
{
    return m_plugins;
}

void QAbstractDiagram::print(QPrinter* printer)
{
	clearSelection();
    QPainter painter(printer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(painter.window(), QColor(255, 255, 255, 0));
    currentSheet()->render(&painter);
}

void QAbstractDiagram::undoStackIndexChanged(int index)
{
    Q_UNUSED(index);
    emit contentsChanged(-1);
}

bool QAbstractDiagram::removeItem( const QString & uuid )
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

QAbstractDiagramGraphicsItem* QAbstractDiagram::restoreItem(const QMap<QString,QVariant> & metaData, const QMap<QString, QVariant> & properties)
{
    QAbstractDiagramGraphicsItem* item = 0;
    QAbstractDiagramPlugin* p = QDiagramPluginLoader::load(metaData.value("plugin", "default").toString());
    if (p){
        item = p->createItem(metaData, properties);

		currentSheet()->addItem(item);
		if (properties.value("layers").isNull()){
			layers()->activeLayer()->add(item);
		}
        itemRestoredHandler(item);
    } else {
        qWarning() << "Plugin" << metaData.value("plugin", "default").toString() << "not found";
    }
    return item;
}

void QAbstractDiagram::setAuthor(const QString &author)
{
    m_author = author;
}

void QAbstractDiagram::setCurrentIndex(int index)
{
	if (index < m_sheets.size()){
		m_index = index;
		emit currentSheetChanged(m_index);
		emit currentSheetChanged(m_sheets.at(m_index).sheet->name());
	}
}

void QAbstractDiagram::setTitle(const QString & title)
{
	m_title = title;
}

QAbstractDiagramShape* QAbstractDiagram::shape(const QString & uuid) const
{
    Q_FOREACH(QGraphicsItem* i, m_sheets.at(m_index).sheet->items()){
        QAbstractDiagramShape* s = dynamic_cast<QAbstractDiagramShape*>(i);
        if (s && s->uuid() == uuid){
            return s;
        }
    }
    return 0;
}

QAbstractDiagramShape* QAbstractDiagram::shapeAt(const QPointF & pos) const
{
	QGraphicsItem* i = m_sheets.at(m_index).sheet->itemAt(pos);
	if (i){
		if (i->parentItem()){
			i = i->parentItem();
		}
		return dynamic_cast<QAbstractDiagramShape*>(i);
	}
	return 0;
}

QList<QAbstractDiagramShape*> QAbstractDiagram::shapes() const
{
	QList<QAbstractDiagramShape*> l;
    Q_FOREACH(QGraphicsItem* i, m_sheets.at(m_index).sheet->items()){
        QAbstractDiagramShape* s = dynamic_cast<QAbstractDiagramShape*>(i);
		if (s){
			l << s;
		}
    }
	return l;
}

void QAbstractDiagram::select(const QString & uuid)
{
	QAbstractDiagramGraphicsItem* i = findItemByUuid(uuid);
	if (i){
		i->setSelected(true);
	}
}

void QAbstractDiagram::selectAll()
{
    Q_FOREACH(QGraphicsItem* i, m_sheets.at(m_index).sheet->items()){
		i->setSelected(true);
	}
}

QList<QAbstractDiagramGraphicsItem*> QAbstractDiagram::selectedItems() const
{
    QList<QAbstractDiagramGraphicsItem*> items;
    Q_FOREACH(QGraphicsItem* i, m_sheets.at(m_index).sheet->selectedItems()){
        QAbstractDiagramGraphicsItem* item = dynamic_cast<QAbstractDiagramGraphicsItem*>(i);
        if (item){
             items.append(item);
        }
    }
    return  items;
}

QColor QAbstractDiagram::selectionColor() const
{
    return QColor(Qt::magenta);
}

void QAbstractDiagram::setSnapEnabled(bool on)
{
	m_sheets.at(m_index).sheet->setSnapEnabled(on);
}

QList<QAction *> QAbstractDiagram::standardItemContextMenuActions() const
{
    return m_standardItemContextMenuActions;
}

QDiagramStyleSheet* QAbstractDiagram::styleSheet() const
{
	return m_styleSheet;
}

void QAbstractDiagram::takeItem(QAbstractDiagramGraphicsItem *item)
{
	m_sheets.at(m_index).layers->remove(item);
    m_sheets.at(m_index).sheet->removeItem(item);
}

QString QAbstractDiagram::title() const
{
	return m_title;
}

QUndoStack* QAbstractDiagram::undoStack() const
{
	return m_undostack;
}
