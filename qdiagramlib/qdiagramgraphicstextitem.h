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
#ifndef QDIAGRAMGRAPHICSTEXTITEM_H
#define QDIAGRAMGRAPHICSTEXTITEM_H

#include <QObject>
#include <QGraphicsTextItem>
#include <QTextCursor>

#include "qdiagramlib_global.h"

class QAbstractDiagramGraphicsItem;

//! The QDiagramGraphicsTextItem class provides a text item that you can add to a QDiagram to display formatted text.
class QDIAGRAMLIBSHARED_EXPORT QDiagramGraphicsTextItem : public QGraphicsTextItem
{
	Q_OBJECT
public:
    enum {
        Type = QGraphicsItem::UserType + 4204
    };
	/**
	 * Constructs a QDiagramGraphicsTextItem. @p parent is passed to QDiagramGraphicsTextItem's constructor.
	 */
	QDiagramGraphicsTextItem(QGraphicsItem* parent = 0);
	/**
	 * Constructs a QDiagramGraphicsTextItem and sets the name of the property storing the text displayed to @p name. @p parent is passed to QDiagramGraphicsTextItem's constructor.
	 */
	QDiagramGraphicsTextItem(const QString & name, QGraphicsItem* parent = 0);
	/**
	 * Destroys the QDiagramGraphicsTextItem.
	 */
	~QDiagramGraphicsTextItem();
	/**
	 * Restores the QDiagramGraphicsTextItem from the given @p properties.
	 */
	void restoreProperties(const QVariantMap & properties);

	void itemPropertyHasChanged(QAbstractDiagramGraphicsItem* item, const QString & name, const QVariant & value);
	/**
	 * If @p on is true, the edit mode is enabled.
	 */
	void setEditModeEnabled(bool on, QTextCursor::MoveOperation operation = QTextCursor::NoMove);
	/**
	 * Sets the text @p alignment.
	 */
	void setTextAlignment(Qt::Alignment alignment);
	/**
	 * Sets the @p name of the parent's property holding the item's text alignment.
	 */
	void setTextAlignmentProperty(const QString & name);
	/**
	 * Sets the @p name of the parent's property holding the item's text.
	 */
	void setTextProperty(const QString & name);
	/**
	 * Returns the name of the property holding the text alignment.
	 * @see setTextAlignmentProperty()
	 */
	QString textAlignmentProperty() const;
	/**
	 * Returns the name of the property holding the text displayed.
	 * @see setTextPropertyName()
	 */
	QString textProperty() const;
    /**
      * Returns the type of the QAbstractDiagramShapeConnectionPoint as an int.
      * @see QGraphicsItem::type()
      */
    int type() const
    {
        return Type;
    }
	/**
	 * Updates the text item's position according to the current alignment settings.
	 */
	void updatePosition();
protected:
	/**
	 * @reimp QGraphicsTextItem::focusOutEvent()
	 */
	void focusOutEvent(QFocusEvent* event);
private slots:
	void documentContentsChanged();
private:
	void updateAlignment(Qt::Alignment alignment);
	void updateTextOption(Qt::Alignment alignment);

	QString m_textProperty;
	QString m_textAlignmentProperty;
};

#endif // QDIAGRAMGRAPHICSTEXTITEM_H
