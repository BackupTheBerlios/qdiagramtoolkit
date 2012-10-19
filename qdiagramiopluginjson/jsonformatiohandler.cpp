/******************************************************************************
** Copyright (C) 2011 - 2012 Martin Hoppe martin@2x2hoppe.de
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
#include "jsonformatiohandler.h"

#include <QtCore>
#include <QtGui>

#include <qabstractdiagram.h>
#include <qabstractdiagramgraphicsitem.h>
#include <qdiagrampluginloader.h>

#include "json.h"

JSONFormatIOHandler::JSONFormatIOHandler()
{
}

bool JSONFormatIOHandler::canRead() const
{
    // {
    if (device()->peek(1) == "{"){
        return true;
    }
    // { "diagram"
    if (device()->peek(10) == "{ \"diagram\"" ){
        return true;
    }

    return false;
}

QAbstractDiagram* JSONFormatIOHandler::read(QObject *parent)
{
    if (device() == 0){
        return 0;
    }
    bool ok;
    QByteArray d = device()->readAll();
    QVariantMap m = QtJson::Json::parse(d, ok).toMap();
    if (!ok || m.isEmpty()){
        return 0;
    }
	QVariantMap dmap = m.value("diagram").toMap();

    if (dmap.value("type").isNull()){
        return 0;
    }
    if (dmap.value("plugin").isNull()){
        return 0;
    }
    QAbstractDiagram* dm = 0;
    dm = QDiagramPluginLoader::diagram(dmap.value("plugin").toString(), dmap.value("type").toString(), parent);
    if (dm == 0){
        return 0;
    }
    Q_FOREACH(QVariant i, dmap.value("plugins").toList()){
        dm->addPlugin(i.toString());
    }

    dm->setTitle(dmap.value("title").toString());
    QVariantMap items = dmap.value("items").toMap();
    QMapIterator<QString,QVariant> it(items);
    while(it.hasNext()){
        it.next();
        dm->restoreItem(it.value().toMap());
    }
    return dm;
}

bool JSONFormatIOHandler::write(QAbstractDiagram *diagram)
{
    QByteArray d;
    bool ok;
    QVariantMap m;
    m["type"] = diagram->type();
    m["plugin"] = diagram->plugin();
    m["plugins"] = diagram->plugins();
    m["title"] = diagram->title();
    QVariantMap i;
    Q_FOREACH(QAbstractDiagramGraphicsItem* item, diagram->items()){
        i[item->uuid()] = item->properties();
    }

    m["items"] = i;
    QVariantMap dm;
    dm["diagram"] = m;
    d = QtJson::Json::serialize(dm, ok);
    if (!ok){
        return false;
    }
    if (device()->write(d) != d.size()){
        return false;
    }
    return true;
}
