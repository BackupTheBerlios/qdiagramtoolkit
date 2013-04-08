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
#include "qdiagramreader.h"

#include "qabstractdiagramshapeconnector.h"
#include "qabstractdiagram.h"

#include "qdiagram.h"
#include "qdiagramiohandler.h"
#include "qdiagramioplugin.h"

static QDiagramIOHandler *createReadHandler(QIODevice *device, const QByteArray &format)
{
    QByteArray f = format.toLower();
    QDiagramIOHandler *handler = 0;
    QDiagramIOPlugin* p;
    QStringList filters;
    QDir dir;

    filters << "*.*";
    dir.setPath(QApplication::applicationDirPath() + "/plugins/diagram/formats");
    dir.setFilter(QDir::Files);

    // Check if the device is a file and if so, get the file suffix
    if (device && format.isEmpty()){
        if (QFile* file = qobject_cast<QFile *>(device)){
            if (!(f = QFileInfo(file->fileName()).suffix().toLower().toLatin1()).isEmpty()) {
            }
        }
    }
    // If format f is still empty, try to determine if the format by testing the content
    if (f.isEmpty()){
//        Q_FOREACH(QString fileName, dir.entryList(filters, QDir::Files)){
//            QPluginLoader pluginLoader(dir.absoluteFilePath(fileName));
//            QObject* o = pluginLoader.instance();
//            QDiagramIOHandler* h;
//            if (o){
//                p = qobject_cast<QDiagramIOPlugin*>(o);
//                if (p && p->capabilities(device, ))
//            }
//        }
    } else {
        Q_FOREACH(QString fileName, dir.entryList(filters, QDir::Files)){
            QPluginLoader pluginLoader(dir.absoluteFilePath(fileName));
            QObject* o = pluginLoader.instance();
            if (o){
                p = qobject_cast<QDiagramIOPlugin*>(o);
                if (p && p->keys().contains(f)){
                    if (p->capabilities(device, f).testFlag(QDiagramIOPlugin::CanRead)){
                        handler = p->create(device, f);
                    }
                }
            }
        }
    }
    if (handler == 0){
        qDebug() << "No diagram I/O handler found for format" << f;
        return 0;
    }
    handler->setDevice(device);
    if (!f.isEmpty()){
        handler->setFormat(f);
    }
    return handler;
}

class QDiagramReaderPrivate
{
public:
    QDiagramReaderPrivate(QDiagramReader* dr);
    ~QDiagramReaderPrivate();

    QIODevice* device;
    bool deleteDevice;
    QString errorString;
    QByteArray format;
    QDiagramIOHandler* handler;
    QDiagramReader::DiagramReaderError diagramReaderError;
    QString text;

    bool initHandler();

    QDiagramReader* r;
};

QDiagramReaderPrivate::QDiagramReaderPrivate(QDiagramReader *dr)
{
    device = 0;
    deleteDevice = false;
    handler = 0;
    diagramReaderError = QDiagramReader::UnknownError;
    errorString = QLatin1String(QT_TRANSLATE_NOOP(QDiagramReader, "Unknown error"));

    r = dr;
}

QDiagramReaderPrivate::~QDiagramReaderPrivate()
{
    if (deleteDevice){
        delete device;
    }
    delete handler;
}

bool QDiagramReaderPrivate::initHandler()
{
    // check some preconditions
    if (!device || (!deleteDevice && !device->isOpen())){
        diagramReaderError = QDiagramReader::DeviceError;
        errorString = QLatin1String(QT_TRANSLATE_NOOP(QDiagramReader, "Invalid device"));
        return false;
    }

    // probe the file extension
    if (deleteDevice && !device->isOpen() && !device->open(QIODevice::ReadOnly)) {
        QList<QByteArray> extensions = QDiagramReader::supportedDiagramFormats();
        if (!format.isEmpty()) {
            // Try the most probable extension first
            int currentFormatIndex = extensions.indexOf(format.toLower());
            if (currentFormatIndex > 0){
                extensions.swap(0, currentFormatIndex);
            }
        }

        int currentExtension = 0;

        QFile *file = static_cast<QFile *>(device);
        QString fileName = file->fileName();

        do {
            file->setFileName(fileName + QLatin1Char('.')
                    + QString::fromLatin1(extensions.at(currentExtension++).constData()));
            file->open(QIODevice::ReadOnly);
        } while (!file->isOpen() && currentExtension < extensions.size());

        if (!device->isOpen()) {
            diagramReaderError = QDiagramReader::FileNotFoundError;
            errorString = QLatin1String(QT_TRANSLATE_NOOP(QDiagramReader, "File not found"));
            return false;
        }
    }

    // assign a handler
    if (!handler && (handler = ::createReadHandler(device, format)) == 0) {
        diagramReaderError = QDiagramReader::UnsupportedFormatError;
        errorString = QLatin1String(QT_TRANSLATE_NOOP(QDiagramReader, "Unsupported diagram format"));
        return false;
    }
    return true;}

QDiagramReader::QDiagramReader() :
    d(new QDiagramReaderPrivate(this))
{
}

QDiagramReader::QDiagramReader(QIODevice *device, const QByteArray & format) :
    d(new QDiagramReaderPrivate(this))
{
    d->device = device;
    d->format = format;
}

QDiagramReader::QDiagramReader(const QString & fileName, const QByteArray & format) :
    d(new QDiagramReaderPrivate(this))
{
    QFile* f = new QFile(fileName);
    d->device = f;
    d->deleteDevice = true;
    d->format = format;
}

QDiagramReader::~QDiagramReader()
{
    delete d;
}

bool QDiagramReader::canRead() const
{
    if (!d->initHandler()){
        return false;
    }

    return d->handler->canRead();
}

QIODevice *QDiagramReader::devive() const
{
    return d->device;
}

QDiagramReader::DiagramReaderError QDiagramReader::error() const
{
    return d->diagramReaderError;
}

QString QDiagramReader::errorString() const
{
    return d->errorString;
}

QString QDiagramReader::fileName() const
{
    QFile *file = qobject_cast<QFile *>(d->device);
    return file ? file->fileName() : QString();
}

QByteArray QDiagramReader::format() const
{
    if (d->format.isEmpty()){
         if (!d->initHandler()){
             return QByteArray();
         }
         return d->handler->canRead() ? d->handler->format() : QByteArray();
     }

     return d->format;
}

QAbstractDiagram* QDiagramReader::read(QObject *parent)
{
    QAbstractDiagram* diagram = 0;
    if (!d->handler && !d->initHandler()){
        return 0;
    }

    // read the image
    diagram = d->handler->read(parent);
    if (diagram == 0){
        d->diagramReaderError = InvalidDataError;
        d->errorString = QLatin1String(QT_TRANSLATE_NOOP(QDiagramReader, "Unable to read diagram data"));
	} else {
		Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram->items()){
			QAbstractDiagramShapeConnector* c = qgraphicsitem_cast<QAbstractDiagramShapeConnector*>(i);
			if (c){
				c->reconnect();
			}
		}
	}
	d->errorString = QLatin1String(QT_TRANSLATE_NOOP(QDiagramReader, "No error"));
    return diagram;
}

void QDiagramReader::setDevice(QIODevice *device)
{
    if (d->device && d->deleteDevice){
        delete d->device;
    }
    d->device = device;
    d->deleteDevice = false;
    delete d->handler;
    d->handler = 0;
    d->text.clear();
}

void QDiagramReader::setFileName(const QString & fileName)
{
    setDevice(new QFile(fileName));
    d->deleteDevice = true;
}

void QDiagramReader::setFormat(const QByteArray & format)
{
    d->format = format;
}

QList<QByteArray> QDiagramReader::supportedDiagramFormats()
{
    QList<QByteArray> l;

    QStringList filters;
    QDir dir;

    filters << "*.dll";
    dir.setPath(QApplication::applicationDirPath() + "/plugins/diagram/formats");
    dir.setFilter(QDir::Files);

    Q_FOREACH(QString fileName, dir.entryList(filters, QDir::Files)){
        QPluginLoader pluginLoader(dir.absoluteFilePath(fileName));
        QObject* o = pluginLoader.instance();
        if (o){
            QDiagramIOPlugin* p = qobject_cast<QDiagramIOPlugin*>(o);
            if (p){
                Q_FOREACH(QString k, p->keys()){
                    l.append(QByteArray(k.toLatin1()));
                }
            }
        }
    }
    qSort(l);
    return l;
}

QDiagramIOPlugin* QDiagramReader::plugin(const QByteArray &format)
{
    QDiagramIOPlugin* p;
    QStringList filters;
    QDir dir;

    filters << "*.*";
    dir.setPath(QApplication::applicationDirPath() + "/plugins/diagram/formats");
    dir.setFilter(QDir::Files);

    Q_FOREACH(QString fileName, dir.entryList(filters, QDir::Files)){
        QPluginLoader pluginLoader(dir.absoluteFilePath(fileName));
        QObject* o = pluginLoader.instance();
        if (o){
            p = qobject_cast<QDiagramIOPlugin*>(o);
            if (p && p->keys().contains(format)){
                return p;
            }
        }
    }
    return 0;
}
