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
#ifndef QSTANDARDFORMSPLUGIN_H
#define QSTANDARDFORMSPLUGIN_H

#include "qstandardformsplugin_global.h"

#include <qabstractdiagramplugin.h>

#include <qdiagramlinestyle.h>

//! @ingroup plugins The QStandardFormsPlugin provides a set of basic shapes.
/**
  * 
  * 
  * 
  */
class QSTANDARDFORMSPLUGINSHARED_EXPORT QStandardFormsPlugin : public QObject, public QAbstractDiagramPlugin
{
    Q_OBJECT
    Q_INTERFACES(QAbstractDiagramPlugin)
public:
    QList<QDiagramConnectorStyle> connectors() const;

    QAbstractDiagramGraphicsItem* createItem(const QMap<QString,QVariant> & metaData, const QMap<QString,QVariant> & properties);

    QAbstractDiagram* diagram(const QString & type = "default", QObject* parent = 0) const;

    QStringList diagrams() const;

    QList<QDiagramEndOfLineStyle> endOfLineStyles() const;

    QStringList groups(QAbstractDiagram* diagram = 0) const;

    QIcon icon(const QString & name) const;

    void initializeProperties(QAbstractDiagramGraphicsItem* item);

    QList<QDiagramLineStyle> lineStyles() const;

	QVariantMap metaData(const QString & name) const;

    QString name() const;

    QAbstractDiagramShape* restoreShape(const QString & uuid, const QString & style, const QMap<QString,QVariant> & properties, QGraphicsScene* scene);

    QMap<QString,QVariant> defaultProperties(const QString & name) const;

    QStringList shapes(const QString & group = QString::null, QAbstractDiagram* diagram = 0) const;

	static QString staticName();

    QString title(const QString & name) const;

    QString toolTip(const QString & shape) const;
};

#endif // QSTANDARDFORMSPLUGIN_H
