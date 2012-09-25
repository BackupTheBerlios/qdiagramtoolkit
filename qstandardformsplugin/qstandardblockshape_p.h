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
#ifndef QSTANDARDBLOCKSHAPE_P_H
#define QSTANDARDBLOCKSHAPE_P_H

class QStandardBlockShapeConnectionPoint : public QAbstractDiagramShapeConnectionPoint
{
public:
    enum Position {
        Bottom = 0x0001,
        BottomRight = 0x0002,
        BottomLeft = 0x0004,
        Left = 0x0008,
        Right = 0x0010,
        Top = 0x0020,
        TopLeft = 0x0040,
        TopRight = 0x0080
    };
    explicit QStandardBlockShapeConnectionPoint(QAbstractDiagramShape* shape, const QString & id, QStandardBlockShapeConnectionPoint::Position position);
    ~QStandardBlockShapeConnectionPoint();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    Position m_pos;
};

#endif // QSTANDARDBLOCKSHAPE_P_H
