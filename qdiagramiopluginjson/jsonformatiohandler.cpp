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

#include <qabstractdiagramgraphicsitem.h>
#include <qdiagram.h>
#include <qdiagramlayer.h>
#include <qdiagramlayers.h>
#include <qdiagrammetadata.h>
#include <qdiagrammetaproperty.h>
#include <qdiagrampluginloader.h>
#include <qdiagramgraphicsitemshadow.h>

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

QDiagram* JSONFormatIOHandler::read(QObject *parent)
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

	QVariantMap mmap = dmap.value("metaData").toMap();
	QVariantMap lsmap = dmap.value("layers").toMap();

    if (mmap.value("type").isNull()){
        return 0;
    }
    if (mmap.value("plugin").isNull()){
        return 0;
    }
    QDiagram* dm = 0;
    dm = QDiagramPluginLoader::diagram(mmap.value("plugin").toString(), mmap.value("type").toString(), parent);
    if (dm == 0){
        return 0;
    }

	QMapIterator<QString,QVariant> it(lsmap);
	while(it.hasNext()){
		it.next();
		dm->layers()->add(it.key());
	}

    Q_FOREACH(QVariant i, mmap.value("plugins").toList()){
        dm->addPlugin(i.toString());
    }
	QVariantMap pmap = dmap.value("properties").toMap();

    dm->setTitle(dmap.value("title").toString());
    
	QVariantMap items = dmap.value("items").toMap();
    it = QMapIterator<QString,QVariant>(items);
    while(it.hasNext()){
        it.next();
		QVariantMap mm = it.value().toMap().value("metaData").toMap();
		QVariantMap pm = it.value().toMap().value("properties").toMap();
        dm->restoreItem(mm, pm);
    }

	it = QMapIterator<QString,QVariant>(lsmap);
	while(it.hasNext()){
		it.next();
		QDiagramLayer* l = dm->layers()->layer(it.key());
		if (l){
			l->setVisible(it.value().toMap().value("visible").toBool());
			Q_FOREACH(QString uuid, it.value().toMap().value("items").toStringList()){
				QAbstractDiagramGraphicsItem* i = dm->findItemByUuid(uuid);
				if (i){
					l->add(i);
				}
			}
			l->setLocked(it.value().toMap().value("locked").toBool());
		}
	}
    return dm;
}

bool JSONFormatIOHandler::write(QAbstractDiagram *diagram)
{
    QByteArray d;
    bool ok;
    QVariantMap imap;

    Q_FOREACH(QAbstractDiagramGraphicsItem* item, diagram->items()){
		QVariantMap pm;
		QVariantMap im;
		QVariantMap mm;
		mm["plugin"] = item->metaData()->pluginName();
		mm["itemType"] = item->metaData()->itemType();
		mm["itemClass"] = item->metaData()->itemClass();
		im["metaData"] = mm;
		for (int i = 0; i < item->metaData()->propertyCount(); i++){
			if (item->property(item->metaData()->property(i).name()).isValid()){
				pm[item->metaData()->property(i).name()] = item->property(item->metaData()->property(i).name()).value();
			}
		}
		im["properties"] = pm;
        imap[item->uuid()] = im;
    }

	QVariantMap lmap;
	QVariantMap lsmap;

	for (int i = 0; i < diagram->layers()->count(); i++){
		lmap.clear();
		lmap["visible"] = diagram->layers()->layer(i)->isVisible();
		lmap["locked"] = diagram->layers()->layer(i)->isLocked();
		QStringList uuids;
		Q_FOREACH(QAbstractDiagramGraphicsItem* item, diagram->layers()->layer(i)->items()){
			uuids.append(item->uuid());
		}
		lmap["items"] = uuids;
		lsmap[diagram->layers()->layer(i)->name()] = lmap;
	}

    QVariantMap mmap;
	QVariantMap pmap;

    mmap["type"] = diagram->type();
    mmap["plugin"] = diagram->plugin();
    mmap["plugins"] = diagram->plugins();

    pmap["title"] = diagram->title();
	pmap["author"] = diagram->author();

    QVariantMap dmap;
	dmap["metaData"] = mmap;
	dmap["properties"] = pmap;
	dmap["items"] = imap;
	dmap["layers"] = lsmap;

    QVariantMap dm;
	dm["diagram"] = dmap;
    d = QtJson::Json::serialize(dm, ok);
    if (!ok){
        return false;
    }
    if (device()->write(d) != d.size()){
        return false;
    }
    return true;
}
