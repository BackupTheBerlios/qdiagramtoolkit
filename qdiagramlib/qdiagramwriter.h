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
#ifndef QDIAGRAMWRITER_H
#define QDIAGRAMWRITER_H

#include <qdiagramlib_global.h>

class QAbstractDiagram;

class QDiagramIOHandler;
class QDiagramWriterPrivate;
class QFile;
class QIODevice;

//! The QDiagramWriter class provides a format independent interface for writing diagrams to files or other devices.
/**
  *
  * QDiagramWriter supports setting format specific options.
  * To store a diagram, you start by constructing a QDiagramWriter object. Pass either a file name or a device pointer, and the diagram format to QDiagramWriter's constructor.
  * canWrite() returns true if QDiagramWriter can write the diagram (i.e., the diagram format is supported and the device is open for writing). Call write() to write the diagram to the device.
  *
  * If any error occurs when writing the diagram, write() will return false. You can then call error() to find the type of error that occurred, or errorString() to get a human readable description of what went wrong.
  *
  * Call supportedDiagramFormats() for a list of formats that QDiagramWriter can write.
  */
class QDIAGRAMLIBSHARED_EXPORT QDiagramWriter
{
public:
    /**
      * This enum describes errors that can occur when writing diagrams with QDiagramWriter.
      */
    enum DiagramWriterError {
        DeviceError = 1 /*!< mage data. Consult your device for more details on what went wrong. */,
        UnsupportedFormatError = 2 /*!< QDiagramWriter does not support the requested diagram format. */,
        UnknownError = 0 /*!< An unknown error occurred. If you get this value after calling write(), it is most likely caused by a bug in QDiagramWriter. */
    };
    /**
      * Constructs an empty QDiagramWriter object. Before writing, you must call setFormat() to set a diagram format, then setDevice() or setFileName().
      */
    QDiagramWriter();
    /**
      * Constructs a QDiagramWriter object using the device @p device and diagram format @p format.
      */
    QDiagramWriter(QIODevice * device, const QByteArray & format = QByteArray());
    /**
      * Constructs a QDiagramWriter objects that will write to a file with the name @p fileName, using the diagram format @p format. If @p format is not provided, QDiagramWriter will detect the diagrams format by inspecting the extension of @p fileName.
      */
    QDiagramWriter(const QString & fileName, const QByteArray & format = QByteArray());
    /**
      * Destructs the QDiagramWriter object.
      */
    ~QDiagramWriter();
    /**
      * Returns true if QDiagramWriter can write the diagram; i.e., the diagram format is supported and the assigned device is open for reading.
      * @see write() setDevice() setFormat().
      */
    bool canWrite() const;
    /**
      * Returns the device currently assigned to QDiagramWriter, or 0 if no device has been assigned.
      */
    QIODevice* device() const;
    /**
      * Returns the type of error that last occurred.
      */
    DiagramWriterError error() const;
    /**
      * Returns a human readable description of the last error that occurred.
      */
    QString errorString() const;
    /**
      * If the currently assigned device is a QFile, or if setFileName() has been called, this function returns the name of the file QDiagramWriter writes to. Otherwise (i.e., if no device has been assigned or the device is not a QFile), an empty QString is returned.
      * @see setFileName() setDevice()
      */
    QString fileName() const;
    /**
      * Returns the format QDiagramWriter uses for writing diagrams.
      */
    QByteArray format() const;
    /**
      * Sets QDiagramWriter's device to device. If a device has already been set, the old device is removed from QDiagramWriter and is otherwise left unchanged.
      * If the device is not already open, QDiagramWriter will attempt to open the device in QIODevice::WriteOnly mode by calling open(). Note that this does not work for certain devices, such as QProcess, QTcpSocket and QUdpSocket, where more logic is required to open the device.
      * @see device() setFileName()
      */
    void setDevice(QIODevice* device);
    /**
      * Sets the file name of QDiagramWriter to @p fileName. Internally, QDiagramWriter will create a QFile and open it in QIODevice::WriteOnly mode, and use this file when writing diagrams.
      */
    void setFileName(const QString & fileName);
    /**
      * Sets the format QDiagramWriter will use when writing diagrams, to format. format is a case insensitive text string. Example:
      * @code
      * QDiagramWriter writer;
      * writer.setFormat("json"); // same as writer.setFormat("JSON");
      * @endcode
      * You can call supportedDiagramFormats() for the full list of formats QDiagramWriter supports.
      */
    void setFormat(const QByteArray & format);
    /**
      * Returns the list of diagram formats supported by QDiagramWriter.
      * <table>
      * <tr><th>Format</th><th>Description</th></tr>
      * <tr><td>JSON</td><td>JavaScript Object Notation</td></tr>
      * </table>
      */
    static QList<QByteArray> supportedDiagramFormats();
    /**
      * Writes the diagram @p diagram to the assigned device or file name. Returns true on success; otherwise returns false. If the operation fails, you can call error() to find the type of error that occurred, or errorString() to get a human readable description of the error.
      */
    bool write(QAbstractDiagram* diagram);
private:
    QDiagramWriterPrivate* d;
};

#endif // QDIAGRAMWRITER_H
