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
#ifndef QDIAGRAMSTYLE_H
#define QDIAGRAMSTYLE_H

#include <qdiagramlib_global.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramStyle
{
public:
    QString id() const;

    bool isValid() const;

    QString key(int index) const;

    QString name() const;

    int propertyCount() const;

    QVariant property(const QString & name) const;

    QVariant property(int index) const;

    QByteArray serialize() const;

    bool setProperty(const QString & name, const QVariant & value);

    QVariant value(const QString & name) const;

    QVariant value(int index) const;
protected:
    QDiagramStyle();
    QDiagramStyle(const QString & name, const QString & plugin);

    void addProperty(const QString & name, const QVariant & value);
private:
    QVariantMap m_properties;
};

#endif // QDIAGRAMSTYLE_H
