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
#include "stdafx.h"
#include "qdiagramgraphicstextitem.h"

#include "qabstractdiagram.h"
#include "qabstractdiagramgraphicsitem.h"
#include "qabstractdiagramshape.h"
#include "qdiagrammetadata.h"

QDiagramGraphicsTextItem::QDiagramGraphicsTextItem(QGraphicsItem* parent)
	: QGraphicsTextItem(parent)
{
	m_defaultTextAlignment = Qt::AlignCenter;
	m_textProperty = "text";
	m_textAlignmentProperty = "textAlignment";
	m_textFontProperty = "textFont";
	connect(document(), SIGNAL(contentsChanged()), SLOT(documentContentsChanged()));
}

QDiagramGraphicsTextItem::QDiagramGraphicsTextItem(const QString & name, QGraphicsItem* parent)
	: QGraphicsTextItem(parent)
{
	m_defaultTextAlignment = Qt::AlignCenter;
	m_textProperty = name;
	m_textAlignmentProperty = "textAlignment";
	m_textFontProperty = "textFont";
	connect(document(), SIGNAL(contentsChanged()), SLOT(documentContentsChanged()));
	setTextWidth(parent->boundingRect().width());
}

QDiagramGraphicsTextItem::~QDiagramGraphicsTextItem()
{
}

QAbstractDiagram* QDiagramGraphicsTextItem::diagram() const
{
	if (parentItem()){
		QAbstractDiagramGraphicsItem* i = qgraphicsitem_cast<QAbstractDiagramGraphicsItem*>(parentItem());
		if (i){
			return i->diagram();
		}
	}
	return 0;
}

void QDiagramGraphicsTextItem::documentContentsChanged()
{
	updatePosition();
}

void QDiagramGraphicsTextItem::focusOutEvent(QFocusEvent* event)
{
	QGraphicsTextItem::focusOutEvent(event);
	setTextInteractionFlags(Qt::NoTextInteraction);
	// Clear current selection
	QTextCursor c = textCursor();
	c.clearSelection();
	setTextCursor(c);
	QAbstractDiagramShape* p = qgraphicsitem_cast<QAbstractDiagramShape*>(parentItem());
	if (p && !m_textProperty.isEmpty()){
		if (textPropertyType() == QDiagramToolkit::Text){
			p->setProperty(m_textProperty, document()->toHtml());
		} else if (textPropertyType() == QDiagramToolkit::String){
			p->setProperty(m_textProperty, document()->toPlainText());
		}
	}
}

void QDiagramGraphicsTextItem::itemPropertyHasChanged(QAbstractDiagramGraphicsItem* item, const QString & name, const QVariant & value)
{
	if (item == 0){
		return;
	}
	if (name == m_textColorProperty){
		setDefaultTextColor(qvariant_cast<QColor>(value));
	} else if (name == m_textFontProperty){
		QFont f = qdiagramproperty_cast<QFont>(QDiagramProperty(QDiagramToolkit::Font, value));
		if (parentShape()){
			f = parentShape()->pointToPixel(f);
		}
		setFont(f);
		QTextCursor c = textCursor();
		c.setVisualNavigation(false);
		c.select(QTextCursor::Document);
		QTextCharFormat cf;
		cf.setFont(f);
		c.setCharFormat(cf);
	} else if (name == m_textProperty){
		if (textPropertyType() == QDiagramToolkit::Text){
			setHtml(item->property(m_textProperty).toString());
		} else if (textPropertyType() == QDiagramToolkit::String){
			setPlainText(item->property(m_textProperty).toString());
		}
	} else if (name == m_textAlignmentProperty){
		updatePosition();
		setTextAlignment(static_cast<Qt::Alignment>(item->property(m_textAlignmentProperty).toInt()));
		updateTextOption(static_cast<Qt::Alignment>(item->property(m_textAlignmentProperty).toInt()));
	} else if (name == "geometry"){
		updatePosition();
	}
}

QAbstractDiagramShape* QDiagramGraphicsTextItem::parentShape() const
{
	if (parentItem()){
		return qgraphicsitem_cast<QAbstractDiagramShape*>(parentItem());

	}
	return 0;
}

QDiagramToolkit::PropertyType QDiagramGraphicsTextItem::textPropertyType() const
{
	QAbstractDiagramShape* p = qgraphicsitem_cast<QAbstractDiagramShape*>(parentItem());
	if (p){
		QDiagramMetaProperty m = p->metaData()->property(p->metaData()->indexOfProperty(m_textProperty));
		return m.type();
	}
	return QDiagramToolkit::PropertyTypeInvalid;
}

void QDiagramGraphicsTextItem::restoreProperties(const QVariantMap & properties)
{
	if (properties.contains(m_textColorProperty)){
		setDefaultTextColor(qvariant_cast<QColor>(properties.value(m_textColorProperty)));
	}
	if (properties.contains(m_textFontProperty)){
		QDiagramProperty p(QDiagramToolkit::Font, properties.value(m_textFontProperty));
		QFont f = qdiagramproperty_cast<QFont>(p);
		if (parentShape()){
			f = parentShape()->pointToPixel(f);
		}
		setFont(f);
	}
	if (textPropertyType() == QDiagramToolkit::Text){
		setHtml(properties.value(m_textProperty).toString());
	} else if (textPropertyType() == QDiagramToolkit::String){
		setPlainText(properties.value(m_textProperty).toString());
	}
	Qt::Alignment a = static_cast<Qt::Alignment>(properties.value(m_textAlignmentProperty, int(m_defaultTextAlignment)).toInt());
	updateAlignment(a);
	setTextAlignment(a);
	updateTextOption(a);
}

void QDiagramGraphicsTextItem::setDefaultTextAlignment(Qt::Alignment alignment)
{
	m_defaultTextAlignment = alignment;
}

void QDiagramGraphicsTextItem::setEditModeEnabled(bool on, QTextCursor::MoveOperation operation)
{
	if (on){
		setTextInteractionFlags(Qt::TextEditorInteraction);
		setFocus();
		QTextCursor t = textCursor();
		t.movePosition(QTextCursor::End);
		setTextCursor(t);
	} else {
		setTextInteractionFlags(Qt::NoTextInteraction);
	}
}

void QDiagramGraphicsTextItem::setTextAlignment(Qt::Alignment alignment)
{
	QTextBlock b = document()->firstBlock();
	while(b.isValid()){
		QTextCursor c(b);
		QTextBlockFormat f = c.blockFormat();
		f.setAlignment(alignment);
		c.setBlockFormat(f);
		b = b.next();
	}
}

void QDiagramGraphicsTextItem::setTextAlignmentProperty(const QString & name)
{
	m_textAlignmentProperty = name;
}

void QDiagramGraphicsTextItem::setTextColorProperty(const QString & name)
{
	m_textColorProperty = name;
}

void QDiagramGraphicsTextItem::setTextProperty(const QString & name)
{
	m_textProperty = name;
}

QString QDiagramGraphicsTextItem::textAlignmentProperty() const
{
	return m_textAlignmentProperty;
}

QString QDiagramGraphicsTextItem::textProperty() const
{
	return m_textProperty;
}

void QDiagramGraphicsTextItem::updatePosition()
{
	qreal x = pos().x();
	qreal y = pos().y();
	QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(parentItem());
	if (s){
		QRectF r = s->geometry();
		r.moveTo(0, 0);
		Qt::Alignment a = static_cast<Qt::Alignment>(s->property(m_textAlignmentProperty).toInt());
		if (a == 0){
			a = m_defaultTextAlignment;
		}
		if (a.testFlag(Qt::AlignHCenter)){
			setTextWidth(-1);
			setTextWidth(boundingRect().width());
//			x = parentItem()->boundingRect().center().x() - textWidth() / 2;
			x = r.center().x() - textWidth() / 2;
		} else if (a.testFlag(Qt::AlignLeft)){
			x = 0;
		} else if (a.testFlag(Qt::AlignRight)){
			x = parentItem()->boundingRect().width() - textWidth();
		}
		if (a.testFlag(Qt::AlignBottom)){
			y = parentItem()->boundingRect().bottom() - boundingRect().height();
		} else if (a.testFlag(Qt::AlignTop)){
			y = 0;
		} else if (a.testFlag(Qt::AlignVCenter)){
//			y = parentItem()->boundingRect().center().y() - boundingRect().center().y();
			y = r.center().y() - boundingRect().center().y();
		}
	}
	setPos(x, y);
}

void QDiagramGraphicsTextItem::updateAlignment(Qt::Alignment alignment)
{
	QTextBlock b = document()->firstBlock();
	while(b.isValid()){
		QTextCursor c(b);
		QTextBlockFormat f = c.blockFormat();
		f.setAlignment(alignment);
		c.setBlockFormat(f);
		b = b.next();
	}
}

void QDiagramGraphicsTextItem::updateTextOption(Qt::Alignment alignment)
{
	document()->defaultTextOption().setAlignment(alignment);
}
