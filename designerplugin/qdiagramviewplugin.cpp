#include "qdiagramview.h"
#include "qdiagramviewplugin.h"

#include <QtCore/QtPlugin>

QDiagramViewPlugin::QDiagramViewPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QDiagramViewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QDiagramViewPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QDiagramViewPlugin::createWidget(QWidget *parent)
{
    return new QDiagramView(parent);
}

QString QDiagramViewPlugin::name() const
{
    return QLatin1String("QDiagramView");
}

QString QDiagramViewPlugin::group() const
{
    return QLatin1String("QDiagram");
}

QIcon QDiagramViewPlugin::icon() const
{
    return QIcon(":/qdiagramdesignerplugin/qdiagramview");
}

QString QDiagramViewPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QDiagramViewPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QDiagramViewPlugin::isContainer() const
{
    return false;
}

QString QDiagramViewPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QDiagramView\" name=\"diagramView\">\n</widget>\n");
}

QString QDiagramViewPlugin::includeFile() const
{
    return QLatin1String("qdiagramview.h");
}
