/******************************************************************************
** Copyright (C) 2011-2012 Martin Hoppe martin@2x2hoppe.de
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
#include "StdAfx.h"
#include "qdiagramgraphicstextitem.h"

#include "qabstractdiagramgraphicsitem.h"
#include "qabstractdiagramshape.h"

QDiagramGraphicsTextItem::QDiagramGraphicsTextItem(const QString & property, QGraphicsItem* parent)
	: QGraphicsTextItem(parent)
{
	m_property = property;
	connect(document(), SIGNAL(contentsChanged()), SLOT(documentContentsChanged()));
}

QDiagramGraphicsTextItem::~QDiagramGraphicsTextItem()
{
}

void QDiagramGraphicsTextItem::documentContentsChanged()
{
	QAbstractDiagramShape* p = qgraphicsitem_cast<QAbstractDiagramShape*>(parentItem());
	if (p && !m_property.isEmpty()){
		p->setProperty(m_property, document()->toPlainText());
	}
}

void QDiagramGraphicsTextItem::focusOutEvent(QFocusEvent* event)
{
	//setTextInteractionFlags(Qt::NoTextInteraction);
	QGraphicsTextItem::focusOutEvent(event);
}
