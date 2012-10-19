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
#include "stdafx.h"
#include "jsonformatioplugin.h"

#include "jsonformatiohandler.h"

QStringList JSONFormatIOPlugin::keys() const
{
    QStringList l;
    l << "json" << "jsn";
    return l;
}

QDiagramIOPlugin::Capabilities JSONFormatIOPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    QDiagramIOPlugin::Capabilities c;
    if (format == "jsn" || format == "json"){
        if (device->isReadable()){
            c |= CanRead;
        } if (device->isWritable()){
            c |= CanWrite;
        }
    }
    if (format.isEmpty()) {
        JSONFormatIOHandler handler;
        handler.setDevice(device);
        if (handler.canRead()){
            return CanRead;
        }
    }
    return c;
}

QDiagramIOHandler* JSONFormatIOPlugin::create(QIODevice *device, const QByteArray &format) const
{
    JSONFormatIOHandler* h = new JSONFormatIOHandler();
    h->setDevice(device);
    h->setFormat(format);
    return h;
}

Q_EXPORT_PLUGIN2(jsonformatioplugin, JSONFormatIOPlugin)
