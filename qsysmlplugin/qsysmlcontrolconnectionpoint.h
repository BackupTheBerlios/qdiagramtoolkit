/******************************************************************************
** Copyright (C) 2013 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QSYSMLCONTROLCONNECTIONPOINT_H
#define QSYSMLCONTROLCONNECTIONPOINT_H

#include <qabstractdiagramshapeconnectionpoint.h>
#include <qdiagramtoolkit.h>

class QSysMLControlConnectionPointIn : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QSysMLControlConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections);
	explicit QSysMLControlConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, QDiagramToolkit::ConnectionPointOrientation orientation, int index, int maxConnections);
    ~QSysMLControlConnectionPointIn();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    int m_index;
};

class QSysMLControlConnectionPointOut : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections);
	explicit QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, QDiagramToolkit::ConnectionPointOrientation orientation, int index, int maxConnections);
    ~QSysMLControlConnectionPointOut();

	QVariantMap customConnectorProperties() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    int m_index;
};

#endif // QSYSMLCONTROLCONNECTIONPOINT_H
