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
#ifndef QLOGICGATESHAPE_P_H
#define QLOGICGATESHAPE_P_H

#include <qabstractdiagramshapeconnectionpoint.h>

class QLogicGateInputConnectionPoint : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QLogicGateInputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections);
    ~QLogicGateInputConnectionPoint();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    int m_index;
};

class QLogicGateOutputConnectionPoint : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QLogicGateOutputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int maxConnections);
    ~QLogicGateOutputConnectionPoint();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
};

#endif // QLOGICGATESHAPE_P_H
