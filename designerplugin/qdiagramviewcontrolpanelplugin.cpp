#include "stdafx.h"
#include "qdiagramviewcontrolpanelplugin.h"
#include "qdiagramviewcontrolpanel.h"

#include <QtCore/QtPlugin>

QDiagramViewControlPanelPlugin::QDiagramViewControlPanelPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QDiagramViewControlPanelPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QDiagramViewControlPanelPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QDiagramViewControlPanelPlugin::createWidget(QWidget *parent)
{
    return new QDiagramViewControlPanel(parent);
}

QString QDiagramViewControlPanelPlugin::name() const
{
    return QLatin1String("QDiagramViewControlPanel");
}

QString QDiagramViewControlPanelPlugin::group() const
{
    return QLatin1String("QDiagramToolkit");
}

QIcon QDiagramViewControlPanelPlugin::icon() const
{
    return QIcon(":/qdiagramdesignerplugin/qdiagramviewcontrolpanel");
}

QString QDiagramViewControlPanelPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QDiagramViewControlPanelPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QDiagramViewControlPanelPlugin::isContainer() const
{
    return false;
}

QString QDiagramViewControlPanelPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QDiagramViewControlPanel\" name=\"controlPanel\">\n</widget>\n");
}

QString QDiagramViewControlPanelPlugin::includeFile() const
{
    return QLatin1String("qdiagramviewcontrolpanel.h");
}
