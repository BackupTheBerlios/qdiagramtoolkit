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
#include "qdiagramview.h"
#include "ui_qdiagramview.h"

#include <qabstractdiagramplugin.h>
#include <qdiagrampluginloader.h>

#include <json.h>

QDiagramView::QDiagramView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QDiagramView)
{
    ui->setupUi(this);
    m_diagram = 0;
    m_snapToGrid = true;
    m_snapSize = QSizeF(13, 13);

    ui->modePushButton->setIcon(QIcon(":/qdiagram/cursor.default"));
    QButtonGroup* g = new QButtonGroup(this);
    g->addButton(ui->showGridToolButton);
    g->setId(ui->showGridToolButton, 0);
    ui->showGridToolButton->setChecked(true);
    g->addButton(ui->hideGridToolButton);
    g->setId(ui->hideGridToolButton, 1);
    g->setExclusive(true);

    connect(g, SIGNAL(buttonClicked(int)), SLOT(gridToolButtonChanged(int)));

    ui->mousePositionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->mousePositionLabel->setFixedWidth(QFontMetrics(QApplication::font(ui->mousePositionLabel)).width("000,000"));
    ui->mousePositionLabel->setFixedWidth(QFontMetrics(QApplication::font(ui->mousePositionLabel)).width("000,000"));

    ui->zoomLabel->setFixedWidth(QFontMetrics(QApplication::font(ui->zoomLabel)).width("100%"));

    connect(ui->diagramGraphicsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(graphicsViewContextMenuRequestHandler(QPoint)));
    connect(ui->diagramGraphicsView, SIGNAL(mouseScenePositionChanged(QPointF)), this, SLOT(mouseScenePositionChanged(QPointF)));
    connect(ui->diagramGraphicsView, SIGNAL(zoomChanged(int)), this, SLOT(zoomChanged(int)));
    connect(ui->zoomInToolButton, SIGNAL(clicked()), ui->diagramGraphicsView, SLOT(zoomIn()));
    connect(ui->zoomOutToolButton, SIGNAL(clicked()), ui->diagramGraphicsView, SLOT(zoomOut()));
}

QDiagramView::~QDiagramView()
{
    delete ui;
}

void QDiagramView::addConnector(const QDiagramConnectorStyle & style)
{
    QAction* a = 0;
    if (ui->modePushButton->menu() == 0){
        QMenu* m = new QMenu(this);
        ui->modePushButton->setMenu(m);
        a = m->addAction(QIcon(":/qdiagram/cursor.default"), tr("Selection"), this, SLOT(modeMenuActionTriggered()));
        a->setData("invalid");
    }
    a = ui->modePushButton->menu()->addAction(style.icon(), style.name());
    a->setData(qVariantFromValue(style));
    connect(a, SIGNAL(triggered()), SLOT(modeMenuActionTriggered()));
}

void QDiagramView::addConnector(const QIcon & icon, const QString & text, const QString & shape)
{
    QAction* a = 0;
    if (ui->modePushButton->menu() == 0){
        QMenu* m = new QMenu(this);
        ui->modePushButton->setMenu(m);
        a = m->addAction(QIcon(":/qdiagram/cursor.default"), tr("Selection"), this, SLOT(modeMenuActionTriggered()));
        a->setData("invalid");
    }
    a = ui->modePushButton->menu()->addAction(icon, text);
    a->setData(shape);
    connect(a, SIGNAL(triggered()), SLOT(modeMenuActionTriggered()));
}

Qt::Alignment QDiagramView::alignment() const
{
    return ui->diagramGraphicsView->alignment();
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

void QDiagramView::cut()
{
    copy();
    Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram()->selectedItems()){
        diagram()->removeItem(i->uuid());
    }
}

QDiagram* QDiagramView::diagram() const
{
	return m_diagram;
}

QDiagramGraphicsView* QDiagramView::graphicsView() const
{
    return ui->diagramGraphicsView;
}

void QDiagramView::gridToolButtonChanged(int id)
{
    ui->diagramGraphicsView->showGrid(id == 0 ? true : false);
}

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
        uuid = m_diagram->addShape(it.value().toMap().value("shape").toString(), p, it.value().toMap(), it.value().toMap().value("plugin").toString());
        uuidMap[it.value().toMap().value("uuid").toString()] = uuid;
    }
}

bool QDiagramView::isInteractive() const
{
    return ui->diagramGraphicsView->isInteractive();
}

void QDiagramView::itemRestored(QAbstractDiagramGraphicsItem* item)
{
	if (ui->diagramGraphicsView->mode() == QDiagramGraphicsView::Connect){
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
    QAction* mAction = qobject_cast<QAction*>(sender());
    if (mAction){
        ui->modePushButton->setIcon(mAction->icon());
        QDiagramConnectorStyle mStyle = qvariant_cast<QDiagramConnectorStyle>(mAction->data());
        if (mStyle.isNull()){
            ui->diagramGraphicsView->setMode(QDiagramGraphicsView::Select);
        } else {
            ui->diagramGraphicsView->setConnectorStyle(mStyle);
            ui->diagramGraphicsView->setMode(QDiagramGraphicsView::Connect);
        }
    }
}

void QDiagramView::mouseScenePositionChanged(const QPointF & pos)
{
    ui->mousePositionLabel->setText(QString("%1,%2").arg(pos.toPoint().x()).arg(pos.toPoint().y()));
}

void QDiagramView::paste()
{
    QPointF p;
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        p = a->data().toPointF();
    }
    if (p.isNull()){
        p = ui->diagramGraphicsView->mapToScene(ui->diagramGraphicsView->viewport()->mapFromGlobal(QCursor::pos()));
    }
    if (m_snapToGrid){
        p = mapToGrid(p);
    }
    insertFromMimeData(QApplication::clipboard()->mimeData(), p);
}

void QDiagramView::setAlignment(Qt::Alignment alignment)
{
	ui->diagramGraphicsView->setAlignment(alignment);
}

void QDiagramView::setDiagram(QDiagram *diagram)
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
    ui->diagramGraphicsView->setScene(m_diagram->scene());
//    ui->diagramGraphicsView->horizontalScrollBar()->setValue(0);
//    ui->diagramGraphicsView->verticalScrollBar()->setValue(0);
//    connect(ui->diagramGraphicsView, SIGNAL(connectItems(QDiagramShape*,QDiagramShape*)), cDiagram->scene(), SLOT(connectItems(QDiagramShape*,QDiagramShape*)));
    // Clear current mode selection menu
	delete ui->modePushButton->menu();
	ui->modePushButton->setMenu(0);
    QAbstractDiagramPlugin* mPlugin = 0;
    Q_FOREACH(QString mName, m_diagram->plugins()){
        mPlugin = QDiagramPluginLoader::plugin(mName, true);
        if (mPlugin){
            Q_FOREACH(QDiagramConnectorStyle mStyle, mPlugin->connectors()){
                addConnector(mStyle);
            }
        }
    }
	connect(m_diagram, SIGNAL(itemRestored(QAbstractDiagramGraphicsItem*)), this, SLOT(itemRestored(QAbstractDiagramGraphicsItem*)));
}

void QDiagramView::setInteractive(bool allowed)
{
    ui->diagramGraphicsView->setInteractive(allowed);
}

void QDiagramView::setSnapSize(const QSizeF &size)
{
    m_snapSize = size;
}

void QDiagramView::setSnapToGrid(bool on)
{
    m_snapToGrid = on;
}

QSizeF QDiagramView::snapSize() const
{
    return m_snapSize;
}

bool QDiagramView::snapToGrid() const
{
    return m_snapToGrid;
}

void QDiagramView::setMode(QDiagramGraphicsView::Mode mode)
{
    ui->diagramGraphicsView->setMode(mode);
}

void QDiagramView::graphicsViewContextMenuRequestHandler(const QPoint &pos)
{
    emit graphicsViewContextMenuRequested(pos, ui->diagramGraphicsView->mapToScene(pos));
}

void QDiagramView::zoomChanged(int percent)
{
    ui->zoomSlider->blockSignals(true);
    if (percent < 200){
        ui->zoomSlider->setValue(percent / 25 - 1);
    } else {
        ui->zoomSlider->setValue(percent / 200 + 6);
    }
    ui->zoomSlider->blockSignals(false);
    ui->zoomLabel->setText(QString("%1%").arg(percent));
}

void QDiagramView::zoomSliderValueChanged(int value)
{
    if (value < 7){
        ui->diagramGraphicsView->setZoom(25 * (value + 1));
    } else {
        ui->diagramGraphicsView->setZoom(200 * (value - 6));
    }
}
