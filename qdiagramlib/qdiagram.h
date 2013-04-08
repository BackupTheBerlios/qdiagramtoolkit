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

#ifndef QDIAGRAM_H
#define QDIAGRAM_H

#include "qdiagramlib_global.h"

#include <QUndoStack>

#include <qabstractdiagram.h>
#include <qabstractdiagramgraphicsitem.h>

#include <qdiagramconnectorstyle.h>
#include <qdiagramlinestyle.h>
#include <qdiagramshape.h>

class QAbstractDiagramShapeConnectionPoint;

//! The QDiagram class provides a default implementation of a diagram.
/**
  * <b>Reading and Writing Diagram Files</b>
  *
  *
  */
class QDIAGRAMLIBSHARED_EXPORT QDiagram  : public QAbstractDiagram
{
    Q_OBJECT
public:
    /**
      * Constructs a QDiagram with the given @p parent.
      */
    explicit QDiagram(QObject* parent = 0);
	
	~QDiagram();
    /**
      *
      */
	//QString addShape(const QString & name, qreal x, qreal y, const QMap<QString,QVariant> & properties, const QString & plugin);

    //QString addShape(const QString & name, const QPointF & pos, const QMap<QString,QVariant> & properties, const QString & plugin);

    /**
      * Returns the name of the plugin providing this diagram.
      */
    //virtual QString plugin() const;
    /**
      * Returns the diagram type.
      */
    virtual QString type() const;
protected:

protected slots:
private:
};

Q_DECLARE_METATYPE(QDiagram*)

#endif // QDIAGRAM_H
