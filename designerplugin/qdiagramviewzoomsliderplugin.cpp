#include "stdafx.h"
#include "qdiagramviewzoomsliderplugin.h"
#include <qdiagramviewzoomslider.h>

#include <QtCore/QtPlugin>

QDiagramViewZoomSliderPlugin::QDiagramViewZoomSliderPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QDiagramViewZoomSliderPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QDiagramViewZoomSliderPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QDiagramViewZoomSliderPlugin::createWidget(QWidget *parent)
{
    return new QDiagramViewZoomSlider(parent);
}

QString QDiagramViewZoomSliderPlugin::name() const
{
    return QLatin1String("QDiagramViewZoomSlider");
}

QString QDiagramViewZoomSliderPlugin::group() const
{
    return QLatin1String("QDiagramToolkit");
}

QIcon QDiagramViewZoomSliderPlugin::icon() const
{
    return QIcon(":/qdiagramdesignerplugin/qdiagramviewzoomslider");
}

QString QDiagramViewZoomSliderPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QDiagramViewZoomSliderPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QDiagramViewZoomSliderPlugin::isContainer() const
{
    return false;
}

QString QDiagramViewZoomSliderPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QDiagramViewZoomSlider\" name=\"zoomSlider\">\n</widget>\n");
}

QString QDiagramViewZoomSliderPlugin::includeFile() const
{
    return QLatin1String("qdiagramviewzoomslider.h");
}
