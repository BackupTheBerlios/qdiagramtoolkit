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
#include "stdafx.h"
#include "qdiagrampluginloader.h"

#include "qabstractdiagramplugin.h"
#include "qdiagram.h"

QMap<QString,QAbstractDiagramPlugin*> sPlugins;

QDiagramPluginLoader::QDiagramPluginLoader()
{
}

QDiagramPluginLoader::~QDiagramPluginLoader()
{

}

QAbstractDiagram *QDiagramPluginLoader::diagram(const QString &plugin, const QString & type, QObject *parent, const QString &path)
{
    if (plugin == "Standard"){
        return new QDiagram(parent);
    } else {
        QAbstractDiagramPlugin* p = load(plugin, path);
        if (p){
            return p->diagram(type);
        }
    }
    return 0;
}

QAbstractDiagramPlugin* QDiagramPluginLoader::load(const QString & name, const QString & path)
{
    QAbstractDiagramPlugin* p = sPlugins.value(name, 0);
    if (p != 0){
        return p;
    }

    QStringList mFilters;
    QDir mDir;

    mFilters << "*.dll";
    if (path.isNull()){
        mDir.setPath(QApplication::applicationDirPath() + "/plugins/diagram");
    } else {
        mDir.setPath(path);
    }
    mDir.setFilter(QDir::Files);

    Q_FOREACH(QString mFileName, mDir.entryList(mFilters, QDir::Files)){
        qDebug() << "Found" << mFileName;
        QPluginLoader mPluginLoader(mDir.absoluteFilePath(mFileName));
        QObject *mObject = mPluginLoader.instance();
        if (mObject){
            p = qobject_cast<QAbstractDiagramPlugin*>(mObject);
            if (p){
                if (p->name() == name){
                    qDebug() << "Plugin" << name << "found in" << mDir.absolutePath();
                    sPlugins[name] = p;
					p->initialize();
                    return p;
                }
            }
        }
    }
    return 0;
}

QList<QAbstractDiagramPlugin *> QDiagramPluginLoader::loadedPlugins()
{
    return sPlugins.values();
}

QAbstractDiagramPlugin* QDiagramPluginLoader::plugin(const QString & name, bool autoload)
{
    if (autoload && !sPlugins.contains(name)){
        return load(name);
    }
    return sPlugins.value(name, 0);
}

QStringList QDiagramPluginLoader::plugins(const QString & path)
{
    QStringList mNames;
    QStringList mFilters;
    QDir mDir;

    mFilters << "*.dll";
    if (path.isNull()){
        mDir.setPath(QApplication::applicationDirPath() + "/plugins/diagram");
    } else {
        mDir.setPath(path);
    }
    mDir.setFilter(QDir::Files);

    Q_FOREACH(QString mFileName, mDir.entryList(mFilters, QDir::Files)){
        QPluginLoader mPluginLoader(mDir.absoluteFilePath(mFileName));
        QObject *mObject = mPluginLoader.instance();
        if (mObject){
            QAbstractDiagramPlugin* mPlugin = qobject_cast<QAbstractDiagramPlugin*>(mObject);
            if (mPlugin){
                mNames.append(mPlugin->name());
            }
        }
    }
    return mNames;
}
