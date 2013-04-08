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
#ifndef QDIAGRAMPLUGINLOADER_H
#define QDIAGRAMPLUGINLOADER_H

#include "qdiagramlib_global.h"

#include "qabstractdiagramplugin.h"

//! The QDiagramPluginLoader class loads a diagram plugin at run-time.
class QDIAGRAMLIBSHARED_EXPORT QDiagramPluginLoader
{
public:
    /**
      * Constructs a diagram plugin loader.
      */
    QDiagramPluginLoader();
    ~QDiagramPluginLoader();

    static QAbstractDiagram* diagram(const QString & plugin, const QString & type, QObject* parent = 0, const QString & path = QString::null);

    static QAbstractDiagramPlugin* load(const QString & name, const QString & path = QString::null);

    static QList<QAbstractDiagramPlugin*> loadedPlugins();

    static QAbstractDiagramPlugin* plugin(const QString & name, bool autoload = true);

    static QStringList plugins(const QString & path = QString::null);
};

#endif // QDIAGRAMPLUGINLOADER_H
