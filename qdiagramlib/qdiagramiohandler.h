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
#ifndef QDIAGRAMIOHANDLER_H
#define QDIAGRAMIOHANDLER_H

#include <qdiagramlib_global.h>

#include <QObject>

class QIODevice;
class QAbstractDiagram;
class QDiagram;

//! The QDiagramIOHandler class defines the common diagram I/O interface for all diagram formats.
class QDIAGRAMLIBSHARED_EXPORT QDiagramIOHandler
{
public:
    /**
      * Constructs a QDiagramIOHandler object.
      */
    QDiagramIOHandler();
    /**
      * Destructs the QDiagramIOHandler object.
      */
    virtual ~QDiagramIOHandler();
    /**
      * Returns true if a diagram can be read from the device (i.e., the diagram format is supported, the device can be read from and the initial header information suggests that the diagram can be read); otherwise returns false.
      *
      * When reimplementing canRead(), make sure that the I/O device (device()) is left in its original state (e.g., by using peek() rather than read()).
      */
    virtual bool canRead() const = 0;
    /**
      * Returns the device currently assigned to the QDiagramIOHandler. If not device has been assigned, 0 is returned.
      * @see setDevice()
      */
    QIODevice* device() const;
    /**
      * Returns the format that is currently assigned to QDiagramIOHandler. If no format has been assigned, an empty string is returned.
      * @see setFormat()
      */
    QByteArray format() const;
    /**
      * Read a diagram from the device, and returns a new instance of QAbstractDiagram. Returns a null pointer if the read fails.
      * @see canRead()
      */
    virtual QAbstractDiagram* read(QObject* parent = 0) = 0;
    /**
      * Sets the device of the QDiagramIOHandler to device. The diagram handler will use this device when reading and writing diagramss.
      * @see device()
      */
    void setDevice(QIODevice* device);
    /**
      * Sets the format of the QDiagramIOHandler to @p format.
      * @see format()
      */
    void setFormat(const QByteArray & format);
    /**
      * Writes the diagram @p diagram to the assigned device. Returns true on success; otherwise returns false.
      *
      * The default implementation does nothing, and simply returns false.
      */
    virtual bool write(QAbstractDiagram* diagram);
protected:
	void setErrorString(const QString & text);
private:
    QIODevice* m_dev;
    QByteArray m_format;
	QString m_errorString;
};

#endif // QDIAGRAMIOHANDLER_H
