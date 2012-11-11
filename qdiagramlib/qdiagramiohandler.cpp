#include "stdafx.h"
#include "qdiagramiohandler.h"

#include "json.h"

#include "qabstractdiagram.h"
#include "qabstractdiagramgraphicsitem.h"
#include "qdiagrampluginloader.h"

QDiagramIOHandler::QDiagramIOHandler()
{
    m_dev = 0;
}

QDiagramIOHandler::~QDiagramIOHandler()
{
}

QIODevice *QDiagramIOHandler::device() const
{
    return m_dev;
}

QByteArray QDiagramIOHandler::format() const
{
    return m_format;
}

void QDiagramIOHandler::setDevice(QIODevice *device)
{
    m_dev = device;
}

void QDiagramIOHandler::setErrorString(const QString & text)
{
	m_errorString = text;
}

void QDiagramIOHandler::setFormat(const QByteArray &format)
{
    m_format = format;
}

bool QDiagramIOHandler::write(QAbstractDiagram *diagram)
{
    return false;
}
