/******************************************************************************
** Copyright (C) 2012 Martin Hoppe martin@2x2hoppe.de
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
#include "qdiagramview.h"
#include "ui_qdiagramview.h"

#include <qabstractdiagramplugin.h>
#include <qdiagrampluginloader.h>
#include <qdiagramsheet.h>

#include <json.h>

QDiagramView::QDiagramView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QDiagramView)
{
    ui->setupUi(this);
    m_diagram = 0;
	m_interactive = true;
    m_snapSize = QSizeF(15, 15);
	connect(ui->tabWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(diagramTabContextMenuRequested(QPoint)));
}

QDiagramView::~QDiagramView()
{
    delete ui;
}

Qt::Alignment QDiagramView::alignment() const
{
    return m_alignment;
}

bool QDiagramView::canInsertFromMimeData(const QMimeData *source) const
{
    if (source){
        return source->hasFormat("application/qdiagram.selection");
    }
    return false;
}

bool QDiagramView::canPaste()
{
    return canInsertFromMimeData(QApplication::clipboard()->mimeData());
}

void QDiagramView::copy()
{
	QByteArray d;
	QVariantMap m;
	Q_FOREACH(QAbstractDiagramGraphicsItem* i, m_diagram->selectedItems()){
		if (!i->property("locked").toBool()){
			m[i->uuid()] = i->properties();
		}
	}
	bool success;
	d = QtJson::Json::serialize(m, success);
	if (success){
		QMimeData* md = new QMimeData();
		md->setData("application/qdiagram.selection", d);
		QClipboard* c = QApplication::clipboard();
		c->setMimeData(md);
    }
}

void QDiagramView::currentSheetChanged(int index)
{
	ui->tabWidget->setCurrentIndex(index);
}

QDiagramGraphicsView* QDiagramView::currentView() const
{
	return ui->tabWidget->findChild<QDiagramGraphicsView*>();
}

void QDiagramView::cut()
{
    copy();
    Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram()->selectedItems()){
        diagram()->removeItem(i->uuid());
    }
}

QAbstractDiagram* QDiagramView::diagram() const
{
	return m_diagram;
}
//
//QDiagramGraphicsView* QDiagramView::graphicsView() const
//{
//    return ui->diagramGraphicsView;
//}

void QDiagramView::insertFromMimeData(const QMimeData *source, const QPointF &scenePos)
{
    if (source == 0){
        return;
    }
    QVariantMap m;
//    QPointF pastePos = ui->diagramGraphicsView->mapToScene(ui->diagramGraphicsView->viewport()->mapFromGlobal(QCursor::pos()));
    QPointF p;
    QPointF firstItemPos;
    QMap<QString,QString> uuidMap;
    QString uuid;
    m = QtJson::Json::parse(source->data("application/qdiagram.selection")).toMap();
    QMapIterator<QString,QVariant> it(m);
    while(it.hasNext()){
        it.next();
        if (firstItemPos.isNull()){
            p = scenePos;
            firstItemPos.setX(it.value().toMap().value("geometry").toMap().value("x").toDouble());
            firstItemPos.setY(it.value().toMap().value("geometry").toMap().value("y").toDouble());
        } else {
            QPointF itemPos;
            itemPos.setX(it.value().toMap().value("geometry").toMap().value("x").toDouble());
            itemPos.setY(it.value().toMap().value("geometry").toMap().value("y").toDouble());
            p = itemPos - firstItemPos + scenePos;

        }
		// TODO
//        uuid = m_diagram->addShape(it.value().toMap().value("shape").toString(), p, it.value().toMap(), it.value().toMap().value("plugin").toString());
        uuidMap[it.value().toMap().value("uuid").toString()] = uuid;
    }
}

void QDiagramView::insertPage()
{
	diagram()->addSheet(tr("Diagram%1").arg(diagram()->sheetCount() + 1));
}

bool QDiagramView::isInteractive() const
{
    return m_interactive;
}

bool QDiagramView::isGridVisible() const
{
	QDiagramGraphicsView* v = currentView();
	if (v){
		return v->isGridVisible();
	}
	return false;
}

bool QDiagramView::isSnapEnabled() const
{
	if (m_diagram){
		return m_diagram->isSnapEnabled();
	}
	return false;
}

void QDiagramView::itemRestored(QAbstractDiagramGraphicsItem* item)
{
	if (currentView()->mode() == QDiagramToolkit::ConnectItemsPointer){
		QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(item);
		if (s){
			s->setConnectionPointsVisible(true);
		}
	}
}

QPointF QDiagramView::mapToGrid(const QPointF &point)
{
    QPointF p(point);
    p.setX(m_snapSize.width() * (int)(p.x() / m_snapSize.width()));
    p.setY(m_snapSize.height() * (int)(p.y() / m_snapSize.height()));
    return p;
}

void QDiagramView::modeMenuActionTriggered()
{
    //QAction* mAction = qobject_cast<QAction*>(sender());
    //if (mAction){
    //    ui->modePushButton->setIcon(mAction->icon());
    //    QDiagramConnectorStyle mStyle = qvariant_cast<QDiagramConnectorStyle>(mAction->data());
    //    if (mStyle.isNull()){
    //        ui->diagramGraphicsView->setMode(QDiagramGraphicsView::Select);
    //    } else {
    //        ui->diagramGraphicsView->setConnectorStyle(mStyle);
    //        ui->diagramGraphicsView->setMode(QDiagramGraphicsView::Connect);
    //    }
    //}
}

void QDiagramView::pageAdded(int index)
{
	QWidget* p = new QWidget();
	p->setObjectName(QString::fromUtf8("page%1").arg(index));
	QVBoxLayout* l = new QVBoxLayout(p);
	l->setContentsMargins(0, 0, 0, 0);
	l->setObjectName(QString::fromUtf8("verticalLayoutPage%1").arg(index));
	QDiagramGraphicsView* v = new QDiagramGraphicsView(p);
	v->setObjectName(QString::fromUtf8("diagramGraphicsViewPage%1").arg(index));
	v->setMinimumSize(QSize(250, 0));
	v->setContextMenuPolicy(Qt::CustomContextMenu);
	v->setDragMode(QGraphicsView::RubberBandDrag);
	v->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	v->setRubberBandSelectionMode(Qt::ContainsItemShape);
	l->addWidget(v);

	connect(v, SIGNAL(zoomChanged(int)), this, SIGNAL(zoomChanged(int)));
	connect(v, SIGNAL(mouseScenePositionChanged(QPointF)), this, SIGNAL(mousePositionChanged(QPointF)));
	connect(v, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(graphicsViewContextMenuRequestHandler(QPoint)));

	ui->tabWidget->addTab(p, m_diagram->sheet(index)->name());
	v->setScene(m_diagram->sheet(index));
	v->setAlignment(m_alignment);
	v->ensureVisible(0, 0, 1, 1);
}

void QDiagramView::paste()
{
    QPointF p;
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        p = a->data().toPointF();
    }
    if (p.isNull()){
        p = currentView()->mapToScene(currentView()->viewport()->mapFromGlobal(QCursor::pos()));
    }
	// TODO snapToGrid
    //if (m_snapToGrid){
    //    p = mapToGrid(p);
    //}
    insertFromMimeData(QApplication::clipboard()->mimeData(), p);
}

void QDiagramView::setAlignment(Qt::Alignment alignment)
{
	m_alignment = alignment;
}

void QDiagramView::setDiagram(QAbstractDiagram *diagram)
{
    if (m_diagram == diagram){
        return;
    }
    if (m_diagram != 0){
        m_diagram->disconnect();
    }
	if (diagram == 0){
		m_diagram = 0;
		return;
	}
    m_diagram = diagram;

	connect(m_diagram, SIGNAL(currentSheetChanged(int)), this, SLOT(currentSheetChanged(int)));
	connect(m_diagram, SIGNAL(pageAdded(int)), this, SLOT(pageAdded(int)));
	connect(m_diagram, SIGNAL(itemRestored(QAbstractDiagramGraphicsItem*)), this, SLOT(itemRestored(QAbstractDiagramGraphicsItem*)));
	while(ui->tabWidget->count() > 0){
		delete ui->tabWidget->widget(0);
	}
	for (int i = 0; i < m_diagram->sheetCount(); i++){
		QWidget* p = new QWidget();
		p->setObjectName(QString::fromUtf8("page%1").arg(i));
        QVBoxLayout* l = new QVBoxLayout(p);
        l->setContentsMargins(0, 0, 0, 0);
        l->setObjectName(QString::fromUtf8("verticalLayoutPage%1").arg(i));
        QDiagramGraphicsView* v = new QDiagramGraphicsView(p);
        v->setObjectName(QString::fromUtf8("diagramGraphicsViewPage%1").arg(i));
        v->setMinimumSize(QSize(250, 0));
        v->setContextMenuPolicy(Qt::CustomContextMenu);
        v->setDragMode(QGraphicsView::RubberBandDrag);
        v->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        v->setRubberBandSelectionMode(Qt::ContainsItemShape);
        l->addWidget(v);
		
		connect(v, SIGNAL(zoomChanged(int)), this, SIGNAL(zoomChanged(int)));
	    connect(v, SIGNAL(mouseScenePositionChanged(QPointF)), this, SIGNAL(mousePositionChanged(QPointF)));
	    connect(v, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(graphicsViewContextMenuRequestHandler(QPoint)));

		ui->tabWidget->addTab(p, m_diagram->sheet(i)->name());
		v->setScene(m_diagram->sheet(i));
        v->setAlignment(m_alignment);
		v->ensureVisible(0, 0, 1, 1);
	}
}

void QDiagramView::setInteractive(bool allowed)
{
	m_interactive = allowed;

	for (int i = 0; i < ui->tabWidget->count(); i++){
		QDiagramGraphicsView* v = ui->tabWidget->findChild<QDiagramGraphicsView*>();
		if (v){
			v->setInteractive(m_interactive);
		}
	}
}

void QDiagramView::setSnapSize(const QSizeF &size)
{
    m_snapSize = size;
}

QAbstractDiagramShape* QDiagramView::shapeAt(const QPoint & pos) const
{
	return qgraphicsitem_cast<QAbstractDiagramShape*>(currentView()->itemAt(pos));
}

QSizeF QDiagramView::snapSize() const
{
    return m_snapSize;
}

void QDiagramView::setConnectorStyle(const QDiagramConnectorStyle & style)
{
	QDiagramGraphicsView* v = currentView();
	if (v){
	    v->setConnectorStyle(style);
	}
}

void QDiagramView::setGridVisible(bool visible)
{
	QDiagramGraphicsView* v = currentView();
	if (v){
		v->showGrid(visible);
	}
}

void QDiagramView::setMode(QDiagramToolkit::PointerMode mode)
{
	QDiagramGraphicsView* v = currentView();
	if (v){
	    v->setMode(mode);
	}
}

void QDiagramView::setSnapEnabled(bool on)
{
	if (m_diagram){
	    m_diagram->setSnapEnabled(on);
	}
}

void QDiagramView::graphicsViewContextMenuRequestHandler(const QPoint & pos)
{
	QDiagramGraphicsView* v = qobject_cast<QDiagramGraphicsView*>(sender());
	if (v){
		emit graphicsViewContextMenuRequested(pos, v->mapToScene(pos));
	}
}

void QDiagramView::setZoom(int percent)
{
	QDiagramGraphicsView* v = currentView();
	if (v){
		v->setZoom(percent);
	}
}

int QDiagramView::zoom() const
{
	QDiagramGraphicsView* v = currentView();
	if (v){
		return v->zoom();
	}
	return 100;
}
