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
#include "qdiagramwriter.h"

#include "qdiagramiohandler.h"
#include "qdiagramioplugin.h"

static QDiagramIOHandler* createWriteHandler(QIODevice *device, const QByteArray &format)
{
    QByteArray f = format.toLower();
    QDiagramIOHandler* handler = 0;
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
                    if (p->capabilities(device, f).testFlag(QDiagramIOPlugin::CanWrite)){
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

class QDiagramWriterPrivate
{
public:
    QDiagramWriterPrivate(QDiagramWriter* dw);
    ~QDiagramWriterPrivate();

    QIODevice* device;
    bool deleteDevice;
    QDiagramWriter::DiagramWriterError diagramWriterError;
    QString errorString;
    QByteArray format;
    QDiagramWriter* w;
    QDiagramIOHandler* handler;
};

QDiagramWriterPrivate::QDiagramWriterPrivate(QDiagramWriter* dw)
{
    device = 0;
    deleteDevice = false;
    handler = 0;
    diagramWriterError = QDiagramWriter::UnknownError;
    errorString = QLatin1String(QT_TRANSLATE_NOOP(QDiagramWriter, "Unknown error"));

    w = dw;
}

QDiagramWriterPrivate::~QDiagramWriterPrivate()
{
    if (deleteDevice){
        delete device;
    }
    delete handler;
}

QDiagramWriter::QDiagramWriter() :
    d(new QDiagramWriterPrivate(this))
{
}

QDiagramWriter::QDiagramWriter(QIODevice* device, const QByteArray & format) :
    d(new QDiagramWriterPrivate(this))
{
    d->device = device;
    d->format = format;
}

QDiagramWriter::QDiagramWriter(const QString & fileName, const QByteArray & format) :
    d(new QDiagramWriterPrivate(this))
{
    QFile* file = new QFile(fileName);
    d->device = file;
    d->deleteDevice = true;
    d->format = format;
}

QDiagramWriter::~QDiagramWriter()
{
    delete d;
}

bool QDiagramWriter::canWrite() const
{
    if (d->device && !d->device->isOpen()){
        d->device->open(QIODevice::WriteOnly);
    }
    if (!d->device || !d->device->isWritable()) {
        d->diagramWriterError = QDiagramWriter::DeviceError;
        d->errorString = QT_TRANSLATE_NOOP(QDiagramWriter, QLatin1String("Device not writable"));
        return false;
    }
    if (!d->handler && (d->handler = ::createWriteHandler(d->device, d->format)) == 0) {
        d->diagramWriterError = QDiagramWriter::UnsupportedFormatError;
        d->errorString = QT_TRANSLATE_NOOP(QDiagramWriter, QLatin1String("Unsupported image format"));
        return false;
    }
    return true;
}

QIODevice *QDiagramWriter::device() const
{
    return d->device;
}

QDiagramWriter::DiagramWriterError QDiagramWriter::error() const
{
    return d->diagramWriterError;
}

QString QDiagramWriter::errorString() const
{
    return d->errorString;
}

QString QDiagramWriter::fileName() const
{
    QFile *file = qobject_cast<QFile *>(d->device);
    return file ? file->fileName() : QString();
}

QByteArray QDiagramWriter::format() const
{
    return d->format;
}

void QDiagramWriter::setDevice(QIODevice *device)
{
    if (d->device && d->deleteDevice){
        delete d->device;
    }
    d->device = device;
    d->deleteDevice = false;
    delete d->handler;
    d->handler = 0;
}

void QDiagramWriter::setFileName(const QString &fileName)
{
    setDevice(new QFile(fileName));
    d->deleteDevice = true;
}

void QDiagramWriter::setFormat(const QByteArray & format)
{
    d->format = format;
}

QList<QByteArray> QDiagramWriter::supportedDiagramFormats()
{
    QList<QByteArray> l;

    QStringList filters;
    QDir dir;

    filters << "*.*";
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
    return l;
}

bool QDiagramWriter::write(QAbstractDiagram *diagram)
{
    if (!canWrite())
        return false;

    if (!d->handler->write(diagram)){
        return false;
    }
    if (d->deleteDevice){
        qobject_cast<QFile *>(d->device)->flush();
    }
    return true;
}
