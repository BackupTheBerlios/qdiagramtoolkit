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
#ifndef QDIAGRAMTEXTSTYLE_H
#define QDIAGRAMTEXTSTYLE_H

#include <QMetaType>

#include <qdiagramlib_global.h>
#include <qdiagramstyle.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramTextStyle : public QDiagramStyle
{
public:
    QDiagramTextStyle();
    QDiagramTextStyle(const QString & name, const QString & plugin);

    Qt::Alignment alignment() const;

    bool bold() const;
    /**
      * Return the text color.
      */
    QColor color() const;

    QString family() const;

    QFont font() const;

    bool italic() const;

    void setAlignment(Qt::Alignment alignment);

    void setBold(bool on);

    void setColor(const QColor & color);

    void setFamily(const QString & name);

    void setItalic(bool on);

    void setSize(int value);

    void setStrikeOut(bool on);

    void setUnderline(bool on);

    int size() const;

    bool strikeOut() const;

    bool underline() const;
private:
    Qt::Alignment m_alignment;
    QFont m_font;
};

Q_DECLARE_METATYPE(QDiagramTextStyle)

#endif // QDIAGRAMTEXTSTYLE_H
