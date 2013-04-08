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
#include <qdiagramsheet.h>

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

	QVariantMap mmap = dmap.value("metaData").toMap();
	QVariantMap pages = dmap.value("sheets").toMap();

    if (mmap.value("type").isNull()){
        return 0;
    }
    if (mmap.value("plugin").isNull()){
        return 0;
    }
    QAbstractDiagram* dm = 0;
    dm = QDiagramPluginLoader::diagram(mmap.value("plugin").toString(), mmap.value("type").toString(), parent);
    if (dm == 0){
        return 0;
    }
	// Add plugins
    Q_FOREACH(QVariant i, mmap.value("plugins").toList()){
        dm->addPlugin(i.toString());
    }
	// Set diagram title
    dm->setTitle(dmap.value("title").toString());
	// Set diagram properties
	QVariantMap pmap = dmap.value("properties").toMap();

	QMapIterator<QString,QVariant> sheet(pages);
	while(sheet.hasNext()){
		sheet.next();
		if (!sheet.hasPrevious()){
			dm->sheet(0)->setName(sheet.key());
		} else {
		}
		QVariantMap items = sheet.value().toMap().value("items").toMap();
		QMapIterator<QString,QVariant> item(items);
		while(item.hasNext()){
			item.next();
			QVariantMap metaData = item.value().toMap().value("metaData").toMap();
			QVariantMap properties = item.value().toMap().value("properties").toMap();
			dm->restoreItem(metaData, properties);
		}
		QMapIterator<QString,QVariant> layers(sheet.value().toMap().value("layers").toMap());
		while(layers.hasNext()){
			layers.next();
			dm->layers()->add(layers.key());
			QDiagramLayer* l = dm->layers()->layer(layers.key());
			if (l){
				l->setVisible(layers.value().toMap().value("visible").toBool());
				Q_FOREACH(QString uuid, layers.value().toMap().value("items").toStringList()){
					QAbstractDiagramGraphicsItem* i = dm->findItemByUuid(uuid);
					if (i){
						l->add(i);
					}
				}
				l->setLocked(layers.value().toMap().value("locked").toBool());
			}
		}
		QVariantMap properties = sheet.value().toMap().value("properties").toMap();
		if (properties.value("paperOrientation") == "landscape"){
			dm->currentSheet()->setPaperOrientation(QDiagramToolkit::Landscape);
		}
		if (properties.value("paperSize") == "A0"){
			dm->currentSheet()->setPaperSize(QDiagramToolkit::A0);
		} else if (properties.value("paperSize") == "A1"){
			dm->currentSheet()->setPaperSize(QDiagramToolkit::A1);
		} else if (properties.value("paperSize") == "A2"){
			dm->currentSheet()->setPaperSize(QDiagramToolkit::A2);
		} else if (properties.value("paperSize") == "A3"){
			dm->currentSheet()->setPaperSize(QDiagramToolkit::A3);
		} else if (properties.value("paperSize") == "A4"){
			dm->currentSheet()->setPaperSize(QDiagramToolkit::A4);
		}
	}
    return dm;
}

bool JSONFormatIOHandler::write(QAbstractDiagram *diagram)
{
    QByteArray d;
    bool ok;
	
	QVariantMap pages;
	QStringList order;
	for (int i = 0; i < diagram->sheetCount(); i++){
		QVariantMap page;
	    QVariantMap items;
		//
		Q_FOREACH(QAbstractDiagramGraphicsItem* item, diagram->items(i)){
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
			items[item->uuid()] = im;
		}
		page["items"] = items;

		QVariantMap lmap;
		QVariantMap lsmap;

		for (int j = 0; j < diagram->layers()->count(); j++){
			lmap.clear();
			lmap["visible"] = diagram->layers()->layer(j)->isVisible();
			lmap["locked"] = diagram->layers()->layer(j)->isLocked();
			QStringList uuids;
			Q_FOREACH(QAbstractDiagramGraphicsItem* item, diagram->layers()->layer(j)->items()){
				uuids.append(item->uuid());
			}
			lmap["items"] = uuids;
			lsmap[diagram->layers()->layer(j)->name()] = lmap;
		}
		page["layers"] = lsmap;

		QVariantMap pmap;
		if (diagram->sheet(i)->paperSize() == QDiagramToolkit::A0){
			pmap["paperSize"] = "A0";
		} else if (diagram->sheet(i)->paperSize() == QDiagramToolkit::A1){
			pmap["paperSize"] = "A1";
		} else if (diagram->sheet(i)->paperSize() == QDiagramToolkit::A2){
			pmap["paperSize"] = "A2";
		} else if (diagram->sheet(i)->paperSize() == QDiagramToolkit::A3){
			pmap["paperSize"] = "A3";
		} else if (diagram->sheet(i)->paperSize() == QDiagramToolkit::A4){
			pmap["paperSize"] = "A4";
		}
		if (diagram->sheet(i)->paperOrientation() == QDiagramToolkit::Landscape){
			pmap["paperOrientation"] = "landscape";
		} else {
			pmap["paperOrientation"] = "portrait";
		}
		page["properties"] = pmap;

		pages[diagram->sheet(i)->name()] = page;
		order << diagram->sheet(i)->name();
	}
	pages["order"] = order;

    QVariantMap mmap;
	QVariantMap pmap;

    mmap["type"] = diagram->type();
    mmap["plugin"] = diagram->pluginName();
    mmap["plugins"] = diagram->plugins();

    pmap["title"] = diagram->title();
	pmap["author"] = diagram->author();
	
    QVariantMap dmap;
	dmap["metaData"] = mmap;
	dmap["properties"] = pmap;
	dmap["sheets"] = pages;
	//dmap["items"] = imap;
	//dmap["layers"] = lsmap;

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
