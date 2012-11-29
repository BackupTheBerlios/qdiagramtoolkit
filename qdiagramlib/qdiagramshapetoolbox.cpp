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
#include "qdiagramshapetoolbox.h"
#include "ui_qdiagramshapetoolbox.h"

#include "qabstractdiagram.h"
#include "qabstractdiagramplugin.h"
#include "qdiagrampluginloader.h"
#include "qdiagramshapelistwidget.h"

QDiagramShapeToolBox::QDiagramShapeToolBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QDiagramShapeToolBox)
{
    ui->setupUi(this);
    while(ui->toolBox->count() > 0){
        delete ui->toolBox->widget(0);
    }
}

QDiagramShapeToolBox::~QDiagramShapeToolBox()
{
    delete ui;
}

void QDiagramShapeToolBox::addShapes(QAbstractDiagram *diagram)
{
    while(ui->toolBox->count() > 0){
        delete ui->toolBox->widget(0);
    }
    Q_FOREACH(QString name, diagram->plugins()){
        QAbstractDiagramPlugin* plugin = QDiagramPluginLoader::plugin(name, false);
        if (plugin){
			addShapes(plugin,diagram->blockedShapes());
        }
    }
}

void QDiagramShapeToolBox::addShapes(QAbstractDiagramPlugin* plugin, const QStringList & blockedShapes)
{
    if (plugin == 0){
        return;
    }
    QDiagramShapeListWidget* w = 0;

    if (plugin->groups().size() < 2){
        w = new QDiagramShapeListWidget(this);
        Q_FOREACH(QString n, plugin->shapes()){
			if (!blockedShapes.contains(n)){
				w->addShape(plugin->icon(n), plugin->title(n), plugin->metaData(n), plugin->defaultProperties(n));
			}
        }
        ui->toolBox->addItem(w, plugin->name());
    } else {
        QToolBox* t = new QToolBox(this);
        while(t->count() > 0){
            delete t->widget(0);
        }
        Q_FOREACH(QString group, plugin->groups()){
            w = new QDiagramShapeListWidget(this);
            Q_FOREACH(QString n, plugin->shapes(group)){
				if (!blockedShapes.contains(n)){
					w->addShape(plugin->icon(n), plugin->title(n), plugin->metaData(n), plugin->defaultProperties(n));
				}
            }
            t->addItem(w, group);
        }
        ui->toolBox->addItem(t, plugin->name());
    }
}

void QDiagramShapeToolBox::removeShape(const QString &name, const QString &plugin)
{
    QToolBox* t = ui->toolBox->findChild<QToolBox*>();
    if (t){
        for (int i = 0; i < t->count(); i++){
            QDiagramShapeListWidget* w = qobject_cast<QDiagramShapeListWidget*>(t->widget(i));
            if (w){
                w->removeShape(name, plugin);
            }
        }
    }
}
