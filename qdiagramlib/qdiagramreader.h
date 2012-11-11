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
#ifndef QDIAGRAMREADER_H
#define QDIAGRAMREADER_H

#include <qdiagramlib_global.h>

#include <QByteArray>
#include <QList>

#include <qdiagramiohandler.h>

class QAbstractDiagram;
class QDiagram;
class QDiagramIOPlugin;

class QFile;
class QIODevice;

class QDiagramReaderPrivate;

//! The QDiagramReader class provides a format independent interface for reading diagrams from files or other devices
class QDIAGRAMLIBSHARED_EXPORT QDiagramReader
{
public:
    /**
      * This enum describes the different types of errors that can occur when reading diagrams with QDiagramReader.
      */
    enum DiagramReaderError {
        FileNotFoundError = 1/*!< QDiagramReader was used with a file name, but not file was found with that name. This can also happen if the file name contained no extension, and the file with the correct extension is not supported by QDiagram. */,
        DeviceError = 2 /*!< QDiagramReader encountered a device error when reading the diagram. You can consult your particular device for more details on what went wrong. */,
        UnsupportedFormatError = 3 /*!< QDiagram does not support the requested diagram format. */,
        InvalidDataError = 4 /*!< The diagram data was invalid, and QDiagramReader was unable to read a diagram from it. The can happen if the diagram file is damaged. */,
        UnknownError = 0 /*!< An unknown error occurred. If you get this value after calling read(), it is most likely caused by a bug in QDiagramReader. */
    };
    /**
      * Constructs an empty QDiagramReader object. Before reading a diagram, call setDevice() or setFileName().
      */
    QDiagramReader();
    /**
      * Constructs a QDiagramReader object with the device @p device and the diagram format @p format.
      */
    QDiagramReader(QIODevice * device, const QByteArray & format = QByteArray());
    /**
      * Constructs a QDiagramReader object with the file name @p fileName and the diagram format @p format.
      */
    QDiagramReader(const QString & fileName, const QByteArray & format = QByteArray());
    /**
      * Destructs the QDiagramReader object.
      */
    ~QDiagramReader();
    /**
      * Returns true if a diagram can be read for the device (i.e., the diagram format is supported, and the device seems to contain valid data); otherwise returns false.
      *
      * canRead() is a lightweight function that only does a quick test to see if the diagram data is valid. read() may still return false after canRead() returns true, if the diagram data is corrupt.
      */
    bool canRead() const;
    /**
      * Returns the device currently assigned to QDiagramReader, or 0 if no device has been assigned.
      * @see setDevice()
      */
    QIODevice* devive() const;
    /**
      * Returns the type of error that occurred last.
      * @see DiagramReaderError errorString()
      */
    DiagramReaderError error() const;
    /**
      * Returns a human readable description of the last error that occurred.
      * @see error()
      */
    QString errorString() const;
    /**
      * If the currently assigned device is a QFile, or if setFileName() has been called, this function returns the name of the file QDiagramReader reads from. Otherwise (i.e., if no device has been assigned or the device is not a QFile), an empty QString is returned.
      * @see setFileName()
      */
    QString fileName() const;
    /**
      * Returns the format QImageReader uses for reading diagrams.
      *
      * You can call this function after assigning a device to the reader to determine the format of the device. For example:
      * @code
      * QDiagramReader reader("diagram.json");
      * // reader.format() == "json"
      * @endcode
      * If the reader cannot read any diagram from the device (e.g., there is no diagram there, or the diagram has already been read), or if the format is unsupported, this function returns an empty QByteArray().
      * @see setFormat() supportedDiagramFormats()
      */
    QByteArray format() const;
    /**
      * Reads a diagram from the device. On success, the diagram that was read is returned; otherwise, a null pointer is returned.
      * You can then call error() to find the type of error that occurred, or errorString() to get a human readable description of the error.
      */
    QDiagram* read(QObject* parent = 0);
    /**
      * Sets QDiagramReader's device to @p device. If a device has already been set, the old device is removed from QDiagramReader and is otherwise left unchanged.
      *
      * If the device is not already open, QDiagramReader will attempt to open the device in QIODevice::ReadOnly mode by calling open(). Note that this does not work for certain devices, such as QProcess, QTcpSocket and QUdpSocket, where more logic is required to open the device.
      * @see device() setFileName()
      */
    void setDevice(QIODevice* device);
    /**
      * Sets the file name of QDiagramReader to @p fileName. Internally, QDiagramReader will create a QFile object and open it in QIODevice::ReadOnly mode, and use this when reading diagrams.
      *  @see fileName(), setDevice() supportedImageFormats()
      */
    void setFileName(const QString & fileName);
    /**
      * Sets the format QDiagramReader will use when reading diagrams, to @p format. format is a case insensitive text string. Example:
      * @code
      * QDiagramReader reader;
      * reader.setFormat("jsn"); // same as reader.setFormat("JSN");
      * @endcode
      * You can call supportedDiagramFormats() for the full list of formats QImageReader supports.
      * @see format()
      */
    void setFormat(const QByteArray & format);
    /**
      * Returns the list of diagram formats supported by QDiagramReader.
      * <table>
      * <tr><th>Format</th><th>Description</th></tr>
      * <tr><td>JSON</td><td>JavaScript Object Notation</td></tr>
      * </table>
      */
    static QList<QByteArray> supportedDiagramFormats();
private:
    QDiagramIOHandler* initHandler();
    QDiagramIOPlugin* plugin(const QByteArray & format);

    QDiagramReaderPrivate* d;
};

Q_DECLARE_METATYPE(QDiagramReader)
Q_DECLARE_METATYPE(QDiagramReader*)

#endif // QDIAGRAMREADER_H
