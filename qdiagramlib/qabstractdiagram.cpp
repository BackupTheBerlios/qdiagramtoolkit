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
#include "qabstractdiagramscene.h"
#include "qdiagrammetadata.h"
#include "qdiagrampluginloader.h"
#include "qdiagramstylesheet.h"

QAbstractDiagram::QAbstractDiagram(QObject *parent) :
    QObject(parent)
{
	m_index = 0;
	m_styleSheet = new QDiagramStyleSheet(this);
	blockSignals(true);
	addPage(tr("Diagram1"));
	blockSignals(false);
	//m_pages.at(m_index) = new QAbstractDiagramScene(this);
 //   m_pages.at(m_index).page->setSceneRect(0, 0, 841, 1189);
	m_undostack = new QUndoStack(this);

	//m_layers = new QDiagramLayers(this);

    connect(m_undostack, SIGNAL(indexChanged(int)), SLOT(undoStackIndexChanged(int)));

    //connect(m_pages.at(m_index), SIGNAL(itemMoved(QGraphicsItem*,QPointF,QPointF)), this, SLOT(itemMoved(QGraphicsItem*,QPointF,QPointF)));
    //// Forward signals
    //connect(m_pages.at(m_index), SIGNAL(selectionChanged()), SIGNAL(selectionChanged()));
}

QAbstractDiagram::~QAbstractDiagram()
{
	delete m_styleSheet;
}

void QAbstractDiagram::addItemContextMenuAction(QAction *action)
{
    m_standardItemContextMenuActions.append(action);
}

int QAbstractDiagram::addPage(const QString & name)
{
	pageData p;
	p.layers = new QDiagramLayers(this);
	p.page = new QAbstractDiagramScene(this);
	if (name.isNull()){
		p.page->setName(tr("Diagram%1").arg(m_pages.size() + 1));
	} else {
		p.page->setName(name);
	}
    p.page->setSceneRect(0, 0, 841, 1189);
	m_pages << p;

    connect(p.page, SIGNAL(itemMoved(QGraphicsItem*,QPointF,QPointF)), this, SLOT(itemMoved(QGraphicsItem*,QPointF,QPointF)));
    connect(p.page, SIGNAL(selectionChanged()), SIGNAL(selectionChanged()));

	emit pageAdded(m_pages.size() - 1);
	return m_pages.size() - 1;
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

QStringList QAbstractDiagram::blockedShapes() const
{
	return QStringList();
}

void QAbstractDiagram::clearSelection()
{
    Q_FOREACH(QGraphicsItem* i, m_pages.at(m_index).page->items()){
		i->setSelected(false);
	}
}

QList<QAbstractDiagramShapeConnector*> QAbstractDiagram::connectors() const
{
    QList<QAbstractDiagramShapeConnector*> mConnections;
    Q_FOREACH(QGraphicsItem* mItem, m_pages.at(m_index).page->items()){
        QAbstractDiagramShapeConnector* mConnector = dynamic_cast<QAbstractDiagramShapeConnector*>(mItem);
        if (mConnector){
            mConnections.append(mConnector);
        }
    }
    return mConnections;
}

int QAbstractDiagram::currentPage() const
{
	return m_index;
}

QVariant QAbstractDiagram::defaultValue(QDiagramToolkit::PropertyType type) const
{
	return QDiagramProperty::defaultValue(type);
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

QPointF QAbstractDiagram::gridPos(const QPointF & point) const
{
    QPointF p(point);
	if (m_pages.at(m_index).page->isSnapToGridEnabled()){
        p.setX(m_pages.at(m_index).page->gridSize().width() * (int)(p.x() / m_pages.at(m_index).page->gridSize().width()));
        p.setY(m_pages.at(m_index).page->gridSize().height() * (int)(p.y() / m_pages.at(m_index).page->gridSize().height()));
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

int QAbstractDiagram::indexOf(QAbstractDiagramScene* page) const
{
	for (int i = 0; i < m_pages.size(); i++){
		if (m_pages.at(i).page == page){
			return i;
		}
	}
	return -1;
}

bool QAbstractDiagram::isModified() const
{
    return !m_undostack->isClean();
}

bool QAbstractDiagram::isSnapToGridEnabled() const
{
	return m_pages.at(m_index).page->isSnapToGridEnabled();
}

QList<QAbstractDiagramGraphicsItem*> QAbstractDiagram::items(int page) const
{
    QList<QAbstractDiagramGraphicsItem*> items;
	for (int i = 0; i < m_pages.size(); i++){
		if (page == -1 || i == page){
			Q_FOREACH(QGraphicsItem* mGraphicsItem, m_pages.at(i).page->items()){
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
	if (page < m_pages.size()){
		return m_pages.at(page).layers;
	}
	return 0;
}

QAbstractDiagramScene* QAbstractDiagram::page(int index) const
{
	if (index < m_pages.size()){
		return m_pages.at(index).page;
	}
	return 0;
}

int QAbstractDiagram::pageCount() const
{
	return m_pages.size();
}

QStringList QAbstractDiagram::plugins() const
{
    return m_plugins;
}

void QAbstractDiagram::print(QPrinter* printer)
{
//    pdfPainter = QPainter(printer)
//    paperRectMM = printer.pageRect(QPrinter.Millimeter)
//    paperRectPixel = printer.pageRect(QPrinter.DevicePixel)
//    c.render(pdfPainter, paperRectPixel, paperRectMM)
//    pdfPainter.end()

//    QPainter mPainter(printer);
////    QRectF mSourceRect = printer->pageRect(QPrinter::Point).adjusted(-printer->pageRect(QPrinter::Point).x(), -printer->pageRect(QPrinter::Point).y(), 0, 0);
////    scene()->render(&mPainter, QRectF(), mSourceRect/*printer->pageRect(QPrinter::Point)*/);
//    QRectF mPaperRectMM = printer->pageRect(QPrinter::Millimeter);
//    QRectF mPaperRectPixel = printer->pageRect(QPrinter::DevicePixel);
//    scene()->render(&mPainter, mPaperRectPixel, mPaperRectMM);
//    mPainter.end();
//    scene()->render(&mPainter);

    QPainter painter(printer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(painter.window(), QColor(255, 255, 255, 0));
    scene()->render(&painter);


//    QPainter painter;
//    painter.begin(printer);
//    double xscale = printer->pageRect().width()/double(myWidget->width());
//    double yscale = printer->pageRect().height()/double(myWidget->height());
//    double scale = qMin(xscale, yscale);
//    painter.translate(printer->paperRect().x() + printer.pageRect().width()/2,
//                       printer.paperRect().y() + printer.pageRect().height()/2);
//    painter.scale(scale, scale);
    //    painter.translate(-width()/2, -height()/2);
}

void QAbstractDiagram::undoStackIndexChanged(int index)
{
    Q_UNUSED(index);
    emit contentsChanged();
}

QAbstractDiagramScene* QAbstractDiagram::scene(int page) const
{
    return m_pages.at(m_index).page;
}

void QAbstractDiagram::setAuthor(const QString &author)
{
    m_author = author;
}

void QAbstractDiagram::setCurrentIndex(int index)
{
	if (index < m_pages.size()){
		m_index = index;
		emit currentPageChanged(m_index);
		emit currentPageChanged(m_pages.at(m_index).page->name());
	}
}

void QAbstractDiagram::setTitle(const QString & title)
{
	m_title = title;
}

QAbstractDiagramShape* QAbstractDiagram::shape(const QString & uuid) const
{
    Q_FOREACH(QGraphicsItem* i, m_pages.at(m_index).page->items()){
        QAbstractDiagramShape* s = dynamic_cast<QAbstractDiagramShape*>(i);
        if (s && s->uuid() == uuid){
            return s;
        }
    }
    return 0;
}

QList<QAbstractDiagramShape*> QAbstractDiagram::shapes() const
{
	QList<QAbstractDiagramShape*> l;
    Q_FOREACH(QGraphicsItem* i, m_pages.at(m_index).page->items()){
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
    Q_FOREACH(QGraphicsItem* i, m_pages.at(m_index).page->items()){
		i->setSelected(true);
	}
}

QList<QAbstractDiagramGraphicsItem*> QAbstractDiagram::selectedItems() const
{
    QList<QAbstractDiagramGraphicsItem*> items;
    Q_FOREACH(QGraphicsItem* i, m_pages.at(m_index).page->selectedItems()){
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

void QAbstractDiagram::setSnapToGridEnabled(bool on)
{
	m_pages.at(m_index).page->setSnapToGridEnabled(on);
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
	m_pages.at(m_index).layers->remove(item);
    m_pages.at(m_index).page->removeItem(item);
}

QString QAbstractDiagram::title() const
{
	return m_title;
}

QUndoStack* QAbstractDiagram::undoStack() const
{
	return m_undostack;
}
